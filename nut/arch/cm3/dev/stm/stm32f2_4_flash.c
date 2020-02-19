/*
 * Copyright (C) 2012, 2013, 2015 - 2018 Uwe Bonnes
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 */

#include <stdint.h>
#include <string.h>

#include <cfg/arch.h>
#include <cfg/clock.h>
#include <cfg/memory.h>
#include <cfg/eeprom.h>
#include <sys/nutdebug.h>
#include <sys/heap.h>
#include <dev/iap_flash.h>

#if defined(STM32F413xx)
#define MAX_SECTOR_IN_BANK 16
#else
#define MAX_SECTOR_IN_BANK 12
#endif

/* How many smallest sectors fit into the selected sector*/
static const uint8_t sector2mult[MAX_SECTOR_IN_BANK] = {
    1,  1,  1,  1,  4,  8,  8,  8,  8,  8,  8,  8,
#if defined(STM32F413xx)
    8,  8,  8,  8
#endif
};
/* How many smallest sectors fit up to the selected sector */
static const uint8_t sector2start[MAX_SECTOR_IN_BANK + 1] = {
    0,  1,  2,  3,  4,  8, 16, 24, 32, 40, 48, 56, 64
#if defined(STM32F413xx)
    , 72, 80, 88
#endif
};

# define FLASH_PSIZE_8    0
# define FLASH_PSIZE_16   FLASH_CR_PSIZE_0
# define FLASH_PSIZE_32   FLASH_CR_PSIZE_1
# define FLASH_PSIZE_64   (FLASH_CR_PSIZE_0 | FLASH_CR_PSIZE_1)
# define FLASH_PSIZE_MASK (FLASH_PSIZE_64)
# define ERASED_PATTERN_32 0xffffffff

#define FLASH_CR_SNB_MASK (FLASH_CR_SNB_0 | FLASH_CR_SNB_1 | FLASH_CR_SNB_2 |\
                           FLASH_CR_SNB_3)

#if (STM32_EXTERNAL_VPP == ENABLE)
#define FLASH_TYPE_CAST   (volatile uint64_t *)
#define FLASH_PSIZE       FLASH_PSIZE_64
#define FLASH_LEN_MASK    7
/* If no other value is given, use 32-bit access*/
#elif  (STM32_SUPPLY_MINIMUM < 2100)
#define FLASH_PSIZE       FLASH_PSIZE_8
#define FLASH_TYPE_CAST   (volatile uint8_t *)
#define FLASH_LEN_MASK    0
#elif (STM32_SUPPLY_MINIMUM < 2700)
#define FLASH_PSIZE       FLASH_PSIZE_16
#define FLASH_TYPE_CAST   (volatile uint16_t *)
#define FLASH_LEN_MASK    1
#else
#define FLASH_TYPE_CAST   (volatile uint32_t *)
#define FLASH_PSIZE       FLASH_PSIZE_32
#define FLASH_LEN_MASK    3
#endif

#define FLASH_KEY1 0x45670123L
#define FLASH_KEY2 0xCDEF89ABL

#define FLASH_OPTKEY1 0x08192A3B
#define FLASH_OPTKEY2 0x4C5D6E7F

/*! \brief Size of the configuration area.
 *
 * During write operations a buffer with this size is allocated
 * from heap and may cause memory problems with large FLASH_CONF_SIZE.
 * Thus, this value may be less than the size of the configuration
 * sector. When write to an already written area in the conf page is
 * requested, the content of the old page with the updated values is
 * written to the either the next conf page, invalidating the old page or
 * after erasing of the sectorto the first page.
 *
 * Currently only 1 sector can be used for system configurations.
 */
#ifndef FLASH_CONF_SIZE
#define FLASH_CONF_SIZE         256
#elif  ((FLASH_CONF_SIZE != 256) && (FLASH_CONF_SIZE != 512) && \
        (FLASH_CONF_SIZE != 1024) && (FLASH_CONF_SIZE != 2048) && \
        (FLASH_CONF_SIZE != 4096) && (FLASH_CONF_SIZE != 8192))
#error FLASH_CONF_SIZE has to be power of 2
#endif

/*!
 *\brief Bitmask of sectors either empty or active erased when first touched
 */
static uint32_t sectorlist;
static uint32_t flashsize;
static uint32_t sectorsize; /* Size of smallest sector.*/
static int      is_dual_bank;
static int      bank_split;

/*!
 *\brief FlashUntouch
 *
 * Mark all sectors as untouch and gather information about
 * flash size and partitioning
 * \param  None
 *
 * \return None
 */
void FlashUntouch(void)
{
    sectorlist = 0;
    flashsize = *(uint16_t*)FLASHSIZE_BASE << 10;
    sectorsize = 1 << 14;
    is_dual_bank = 0;
    bank_split = MAX_SECTOR_IN_BANK;
#if defined(FLASH_OPTCR_DB1M)
    if (flashsize > 0x100000) {
        is_dual_bank = 1;
    } else if (FLASH->OPTCR & FLASH_OPTCR_DB1M) {
        is_dual_bank = 1;
        bank_split = 8;
    }
#elif defined(FLASH_OPTCR_nDBANK)
    is_dual_bank = (FLASH->OPTCR & FLASH_OPTCR_nDBANK)? 0 : 1;
    if (is_dual_bank == 0) {
        sectorsize = 1 << 15;
    }
#endif
}

/*!
  * \brief  Unlocks the FLASH Program Erase Controller.
  * \retval 0 on success, FLASH_LOCKED else.
  */
static FLASH_Status FLASH_Unlock(void)
{

    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
    return (FLASH->CR & FLASH_CR_LOCK)?FLASH_LOCKED:FLASH_COMPLETE;
}

/*!
 *\brief Calculate sector number and sector length form address
 * Second bank starts at sector 16!
 * \param Addr Address
 *
 * \return Sector Number
 */
static uint32_t FlashAddr2Sector(void* Addr)
{
    uint32_t res = 0;
    uint32_t offset = (uint32_t)Addr - FLASH_BASE;
    uint32_t sector = offset /sectorsize;
    int i;

    if (is_dual_bank)  {
        if (sector >= sector2start[bank_split]) {
            res = 16;
            sector -= sector2start[bank_split];
        }
    }
    for (i = 0; i <= sizeof(sector2start); i++) {
        if (sector <= sector2start[i]) {
            res += i;
            break;
        }
    }
    return res;
}

/*!
 *\brief Calculate address from sectors
 * Second bank starts at sector 16!
 * \param sector Sector
 *
 * \return Addr Adress
 */
static uint32_t FlashSector2Addr(int sector)
{
    uint32_t res = 0;

    if (sector >= 16) {
        res = sector2start[bank_split];
    }
    res += sector2start[sector % 16];
    res *= sectorsize;
    return res;
}

/*!
  * \brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
  * \param  Timeout: FLASH progamming Timeout in Microseconds
  *
  * \retval FLASH Status: FLASH_COMPLETE or appropriate error.
  */
static FLASH_Status FLASH_GetStatus(void)
{
    FLASH_Status rs = FLASH_COMPLETE;

    /* Decode the Flash Status
     * Check BSY last, so maybe it has completed meanwhile*/
    if (FLASH->SR & (FLASH_SR_PGSERR | FLASH_SR_PGPERR | FLASH_SR_PGAERR)) {
        rs = FLASH_ERROR_PG;
    } else if (FLASH->SR & FLASH_SR_WRPERR) {
        rs = FLASH_ERROR_WRP;
    } else if (FLASH->SR & FLASH_SR_BSY) {
        rs = FLASH_BUSY;
    }
    /* Return the Flash Status */
    return rs;
}

/*!
  * \brief  Waits for a Flash operation to complete
  *
  * \retval FLASH Status: FLASH_COMPLETE or appropriate error.
  * Every flash access stalls while FLASH erase/program is running
  * The erase/program process however will finish at some point
  * and may indicate failure then
  */
static FLASH_Status FlashWaitReady(void)
{
    FLASH_Status status;
    do
        status = FLASH_GetStatus();
    while (status == FLASH_BUSY);

    /* Return the operation status */
    return status;
}

/*!
 * \brief Erase specified FLASH sector.
 *
 * \param sector Sector to erase.
 *
 * \return FLASH Status: FLASH_COMPLETE or appropriate error.
 */
static FLASH_Status FlashEraseSector(uint32_t sector)
{
    uint32_t flash_cr = FLASH_CR_SER;
    FLASH_Status rs = FLASH_COMPLETE;

    uint32_t addr;
    uint64_t *data, *end;
    int size;

    addr = FLASH_BASE + FlashSector2Addr(sector);

    size = sector2mult[sector % 16] * sectorsize;
    /* Check if sector is already erased by reading DWORDS*/
    data = (uint64_t*) addr;
    end =  (uint64_t*) (addr + size);
    for (; data < end ; ) {
        if (*data++ != -1LL) {
            break;
        }
    }
    if (data >= end) {
        goto erase_done;
    }
    /* Wait for last operation to be completed */
    rs = FlashWaitReady();
    flash_cr |= sector * FLASH_CR_SNB_0;

    if (rs == FLASH_COMPLETE) {
        /* if the previous operation is completed, proceed to erase the page */
        FLASH->CR = flash_cr;
        FLASH->CR = flash_cr | FLASH_CR_STRT;
        __DSB();

        /* Wait for last operation to be completed */
        rs = FlashWaitReady();
        FLASH->CR = 0;
    }
erase_done:
    if (rs != FLASH_COMPLETE) {
        sectorlist &= ~(1 << sector);
    } else {
        sectorlist |= (1 << sector);
    }
    /* Return the Erase Status */
    return rs;
}

/*!
 * \brief Program any data to FLASH.
 *
 * This function writes data from source address to FLASH.
 * It handles erasing and assembling of data automatically.
 * On F2/F4 We need to handle sectors with up to 128 kByte,
 * so we can't keep a copy.
 *
 * Lets do some heuristic for F2/F4: We keep count of the sectors
 * we have touched.
 * If we touch a sector for the first time, we look if the sector is
 * clean and if not we erase the sector. Further access to this sector
 * doesn't erase until reset.
 * Attention: It is the users responsibility to not write multiple times
 * to the same range.
 *
 * \param dst Pointer to address anywhere in FLASH.
 * \param src Pointer to source data. With SRC == NULL, the region
 *        is checked for write protection
 * \param len Number of bytes to be written/checked.
 * \param mode Erase mode (Always, on first access to block, never).
 *
 * \return FLASH Status: FLASH_COMPLETE or appropriate error.
 */
static FLASH_Status FlashWrite(void* dst, const void* src, size_t len,
                                FLASH_ERASE_MODE mode)
{
    FLASH_Status rs = FLASH_COMPLETE;
    uint32_t sector_start, sector_end;
    int i;
    void *wptr = dst;
    const void *rptr = src;
    uint32_t length = len;
    uint32_t flash_end_addr = FLASH_BASE + flashsize;
    uint32_t last_sector_nr = -1;

    /* we need to run with at least 1 MHz for flashing*/
    NUTASSERT((SysTick->LOAD++) / NUT_TICK_FREQ > 1000);

    if (len == 0) {
        return FLASH_COMPLETE;
    }
    /* Check top boundary */
    if ((((uint32_t)dst + len) > flash_end_addr) ||
        ((uint32_t)dst < FLASH_BASE)) {
        return FLASH_BOUNDARY;
    }

    /* Check for write protected sectors */
    sector_start = FlashAddr2Sector(dst);
    sector_end = FlashAddr2Sector(dst + len - 1);
#if defined(MCU_STM32F2) ||defined(MCU_STM32F4)
    if (sector_end >= 16) {
        uint32_t optcr = FLASH->OPTCR1;
        uint32_t bank2_start = (sector_start < 16) ? 16 : sector_start;
        for (i = bank2_start;  i <= sector_end; i++) {
            uint32_t bit = 1 << (i - 16 + FLASH_OPTCR_nWRP_Pos);
            if ((optcr & bit) == 0) {
                return FLASH_ERROR_WRP;
            }
        }
        sector_end = bank_split - 1;
    }
    uint32_t optcr = FLASH->OPTCR;
# if defined(STM32F413xx)
    if (sector_end >= bank_split - 2) {
        if (optcr & FLASH_OPTCR_nWRP_14) {
            return FLASH_ERROR_WRP;
        }
    }
    sector_end -= 2;
# endif
    for (i = sector_start; i <= sector_end; i++) {
        uint32_t bit = 1 << (i + FLASH_OPTCR_nWRP_Pos);
        if ((optcr & bit) == 0) {
            return FLASH_ERROR_WRP;
        }
    }
#elif defined(MCU_STM32F7)
    uint32_t optcr = FLASH->OPTCR;
    if (!is_dual_bank) {
        for (i = sector_start; i <= sector_end; i++) {
            uint32_t bit = 1 << (i + FLASH_OPTCR_nWRP_Pos);
            if ((optcr & bit) == 0) {
                return FLASH_ERROR_WRP;
            }
        }
    } else {
        /* Each bit protects two sectors! */
        if (sector_end >= 16) {
            int bank2_start = (sector_start < 16) ? 16 : sector_start;
            for (i = bank2_start;  i <= sector_end; i = i + 2) {
                uint32_t bit  = 1 << ((i - 16 + bank_split) / 2 * FLASH_OPTCR_nWRP_Pos);
                if ((optcr & bit) == 0) {
                    return FLASH_ERROR_WRP;
                }
            }
            sector_end = bank_split - 1;
            for (i = sector_start; i <= sector_end; i = i + 2) {
                uint32_t bit  = 1 << (i / 2 + FLASH_OPTCR_nWRP_Pos);
                if ((optcr & bit) == 0) {
                    return FLASH_ERROR_WRP;
            }
            }
        }
    }
#endif
    if (src == NULL) {
        /* Only a check for write protection was requested */
        return  FLASH_COMPLETE;
    }
    /* Unlock related banks */
    rs = FLASH_Unlock();
    if (rs != FLASH_COMPLETE) {
        /* Unlocking failed for any reason */
        return FLASH_LOCKED;
    }
    while ((length) && (rs == FLASH_COMPLETE)) {
        uint32_t sector_nr = FlashAddr2Sector(wptr);
        uint32_t sector_length = sector2mult[sector_nr % 16] * sectorsize;
        uint32_t current_length = length;

        if (((uint32_t)wptr & (sector_length -1)) + length > sector_length) {
            current_length = sector_length -
                ((uint32_t)wptr & (sector_length -1));
        }
        length -= current_length;
        /* Check if sector needs erase */
        if (((mode == FLASH_ERASE_ALWAYS) && (sector_nr != last_sector_nr)) ||
            ((mode == FLASH_ERASE_FIRST_TOUCH) &&
             (sectorlist & (1 << sector_nr)) == 0)) {
            rs = FlashEraseSector(sector_nr);
            if (rs != FLASH_COMPLETE)
                goto done;
            last_sector_nr = sector_nr;
        }
        /* Program the sector */
        rs = FlashWaitReady();
        if (rs != FLASH_COMPLETE) {
            goto done;
        } else {
            /* Size written must correspond the size announced!  */
            /* Enable Programming Mode */
            FLASH->CR =  FLASH_CR_PG | FLASH_PSIZE_8;

            /* Write data to page */
            /* First, align to 2/4/8 Byte boundary */
            /* Run loop control before waiting for command to finish*/
            while (current_length && ((uint32_t)wptr & FLASH_LEN_MASK)) {
                rs = FlashWaitReady();
                *(volatile uint8_t*)wptr++ = *(volatile uint8_t*)rptr++;
                __DSB();
                current_length--;
            }
            /* Write Bulk of data in requested width*/
            rs = FlashWaitReady();
            FLASH->CR =  FLASH_CR_PG | FLASH_PSIZE ;
            while ((current_length > FLASH_LEN_MASK) &&
                   (rs == FLASH_COMPLETE)) {
                rs = FlashWaitReady();
                * FLASH_TYPE_CAST wptr = * FLASH_TYPE_CAST rptr  ;
                __DSB();
                current_length -= (FLASH_LEN_MASK +1);
                wptr += (FLASH_LEN_MASK + 1);
                rptr += (FLASH_LEN_MASK + 1);
            }
            rs = FlashWaitReady();
            FLASH->CR =  FLASH_CR_PG | FLASH_PSIZE_8;
            while ((current_length > 0) && (rs == FLASH_COMPLETE)) {
                rs = FlashWaitReady();
                *(volatile uint8_t*)wptr++ = *(volatile uint8_t*)rptr++;
                __DSB();
                current_length --;
            }
            rs = FlashWaitReady();
            FLASH->CR = 0;
            if (rs != FLASH_COMPLETE) {
                goto done;
            }
        }
    }

    /* Check the written data */
    wptr = dst;
    rptr = src;
    length = len;
    /* Align flash access to 4 Byte Boundary*/
    while (length && ((uint32_t)wptr & FLASH_LEN_MASK)) {
        if (*(volatile uint8_t*)wptr++ != *(uint8_t*)rptr++) {
            goto cmp_err;
        }
        length--;
    }
    /* Now compare 32-bit  at a time*/
    while (length > 3) {
        if (*(volatile uint32_t*)wptr != *(uint32_t*)rptr) {
            goto cmp_err;
        }
        length -= 4;
        wptr   += 4;
        rptr   += 4;
    }
    while (length) {
        if ((*(volatile uint8_t*)wptr++) != *(uint8_t*)rptr++) {
            goto cmp_err;
        }
        length--;
    }
    goto done;
cmp_err:
    rs = FLASH_COMPARE;

done:
    /* Lock the FLASH again */
    FLASH->CR = FLASH_CR_LOCK;

    return rs;
}

/*!
 * \brief Get Upper limit of Flash.
 *
 * This function return upper limit of flash, excluding parameter storage.
 *
 * \param NONE
 * \return Last Flash Address.
 */
size_t IapFlashEnd(void)
{
    uint32_t area_end = FLASH_BASE + flashsize - 1;
#if defined(NUT_CONFIG_STM32_IAP)
    area_end -= 8 * sectorsize;
#endif
    return area_end;
}

/*!
 * \brief Get address of first full size sector above program storage.
 *
 * \param NONE
 * \return Address of first full size sector above program storage.
 */
extern uint32_t __end_rom;

size_t IapProgramEnd(void)
{
    uint32_t sector;

    sector = FlashAddr2Sector(&__end_rom);
    sector++;
    return FLASH_BASE + sector2start[sector] * sectorsize;
}

/*!
 * \brief Return pagesize of given address.
 *
 * \param addr Address
 * \return Size of current page.
 */
size_t IapPageSize(size_t addr)
{
    uint32_t sector;

    sector = FlashAddr2Sector((void*)addr);
    return sector2mult[sector] * sectorsize;
}

/*!
 * \brief Program any data to FLASH.
 *
 * This function writes data from source address to FLASH.
 * It handles erasing and assembling of data automatically.
 * On F2/F4 We need to handle sectors with up to 128 kByte,
 * so we can't keep a copy. If a configuration sector is used,
 * this sector is not allowed for writing.
 *
 * \param dst Pointer to address anywhere in FLASH.
 * \param src Pointer to source data. With SRC == NULL, the region
 *        is checked for write protection
 * \param len Number of bytes to be written/checked.
 * \param mode Erase mode (Always, on first access to block, never).
 *
 * \return FLASH Status: FLASH_COMPLETE or appropriate error.
 */
FLASH_Status IapFlashWrite(void* dst, const void* src, size_t len,
                            FLASH_ERASE_MODE mode)
{
    uint32_t flash_end_addr = FLASH_BASE + flashsize;
#if defined(NUT_CONFIG_STM32_IAP)
    flash_end_addr -= 8 * sectorsize;
#endif
    if ((uint32_t)dst + len > flash_end_addr) {
        return FLASH_BOUNDARY;
    } else {
        return FlashWrite(dst, src, len, mode);
    }
}

/*!
 * \brief Find the Conf sector
 * Look at last position in each conf page.
 *
 * \param flash_conf_sector Sector with conf pages.
 *
 * \return page index of active conf page, -1 if no valid sector
 */
 static int Stm32FlashFindConfSector(void *flash_conf_sector)
{
    int conf_page = 0;
    uint32_t *marker = (uint32_t*)
        (flash_conf_sector + FLASH_CONF_SIZE  - sizeof(ERASED_PATTERN_32));
    /* Find configuration page in CONF_SECTOR. Check limit before
    * memory access!*/
    while ((conf_page < (8 * sectorsize / FLASH_CONF_SIZE)) &&
           (*marker !=  ERASED_PATTERN_32) ) {
        conf_page++;
        marker += FLASH_CONF_SIZE / 4;
    }
    if (conf_page >= (8 * sectorsize / FLASH_CONF_SIZE)) {
         conf_page = -1;
        }
    return conf_page;
}

/*!
 * \brief Nut/OS specific handling for parameters in FLASH.
 *
 * This function enables to read system specific parameters
 * from processors FLASH. The sectors used for storage are
 * configureable via nutconf.
 * The last sectors of F2/4 is 128 kByte and 256/128k for F7. we
 * use a rolling scheme to write a FLASH_CONF_SIZE
 * configuration page. The upper 32-bit word in a configuration
 * page is used to indicated the present used configuration page.
 * The first page with this value erases is considered the page
 * in use.
 *
 * \param pos Offset of parameter(s) in configured page(s).
 * \param data Pointer where to copy data from flash to.
 * \param len Number of bytes to be copied.
 *
 * \return FLASH_Status: FLASH_COMPLETE or appropriate error.
 */
FLASH_Status Stm32FlashParamRead(uint32_t pos, void *data, size_t len)
{
    void *flash_conf_sector = (void*)FLASH_BASE + flashsize - 8 * sectorsize;
    if (len == 0) {
        return FLASH_COMPLETE;
    }
    /* Check boundaries */
    if (pos + len + sizeof(ERASED_PATTERN_32) > FLASH_CONF_SIZE) {
        return FLASH_BOUNDARY;
    }
    int conf_page = Stm32FlashFindConfSector(flash_conf_sector);
    if (conf_page < 0) {
        /* no page sizes unit in CONF_SECTOR has a valid mark.
         * Erase Sector and return page 0 as result. */
        int rs;
        uint32_t sector;
        sector = FlashAddr2Sector(flash_conf_sector);
        rs = FlashEraseSector(sector);
        if (rs) {
            return rs;
        }
        conf_page = 0;
    }
    memcpy(data, (uint8_t *)(flash_conf_sector +
                              conf_page * FLASH_CONF_SIZE + pos), len);

    /* Return success or fault code */
    return FLASH_COMPLETE;
}

/*!
 * \brief Nut/OS specific handling for parameters in FLASH.
 *
 * This function enables to store system specific parameters
 * in processors FLASH. The sectors used for storage are
 * configurable via nutconf.
 *
 * FIXME: As the F2/F4 last sector is quite much bigger than
 * a FLASH_CONF_SIZE we can handle, implement some rolling scheme
 * E.g. the last word == 0xffffffff in the first FLASH_CONF_SIZE
 * unit starting at FLASH_CONF_SECTOR marks a valid CONF_PAGE. When
 * writing a new sector, we set the MARK of the last CONF_SECTOR to
 * 0. If CONF_SECTOR wraps the sectorsize, we start all over.
 *
 * \param pos Offset of parameter(s) in configured page(s).
 * \param data Pointer to source data.
 * \param len Number of bytes to be written.
 *
 * \return FLASH_Status: FLASH_COMPLETE or appropriate error.
 */
FLASH_Status Stm32FlashParamWrite(unsigned int pos, void *data,
                                  size_t len)
{
    FLASH_Status rs = 0;
    uint8_t *buffer;
    uint8_t *mem;
    void* flash_conf_sector = (void*)FLASH_BASE + flashsize - 8 * sectorsize;
    int i;
    FLASH_ERASE_MODE mode;
    uint32_t indicator;

    if (len == 0) {
        return FLASH_COMPLETE;
    }
    /* Check top boundaries */
    if (pos + len + sizeof(ERASED_PATTERN_32) > FLASH_CONF_SIZE) {
        return FLASH_CONF_OVERFLOW;
    }
    int conf_page = Stm32FlashFindConfSector(flash_conf_sector);
    if (conf_page < 0) {
        /* no page sizes unit in CONF_SECTOR has a valid mark
         * Erase Sector sector by flagging FLASH_ERASE_ALWAYS
         * and write to page 0*/

        rs = FlashWrite(flash_conf_sector + pos, data, len, FLASH_ERASE_ALWAYS);
        /* Return success or fault code */
        return rs;
    }
    /* Check if target area is erased.
     * It seems no C standard function provides this functionality!
     */
    mem = (uint8_t*) (flash_conf_sector + conf_page * FLASH_CONF_SIZE + pos);
    for (i = 0; i < len; i++) {
        if (mem[i] != (ERASED_PATTERN_32 & 0xff)) {
            break;
        }
    }
    if (i >= len) {
        /* Needed area is erased, simply write the data to the requested area*/
        rs = FlashWrite(flash_conf_sector + conf_page * FLASH_CONF_SIZE + pos,
                         data, len, FLASH_ERASE_NEVER);
        return rs;
    }
    /* Check if content needs no update. */
    if (memcmp(flash_conf_sector + conf_page * FLASH_CONF_SIZE + pos,
               data, len) == 0) {
        return FLASH_COMPLETE;
    }
    /* Save configuration page in RAM and write updated data to next
     * configuration page, eventually erasing the sector wrapping to the
     * first page
     */
    buffer = NutHeapAlloc(FLASH_CONF_SIZE);
    if (buffer == NULL) {
        /* Not enough memory */
        return FLASH_OUT_OF_MEMORY;
    }
    /* Get the content of the whole config page*/
    memcpy(buffer, flash_conf_sector + conf_page * FLASH_CONF_SIZE,
           FLASH_CONF_SIZE);
    /* Overwrite new data in buffer*/
    memcpy (buffer + pos, data, len);
    /* Invalidate old  conf_page by deleting the indicator pattern */
    indicator = ~ERASED_PATTERN_32;
    rs = FlashWrite(flash_conf_sector + ((conf_page + 1) * FLASH_CONF_SIZE) -
                     sizeof(indicator), &indicator, sizeof(indicator),
                     FLASH_ERASE_NEVER);
    if (rs) {
        goto done;
    }
    do {
        conf_page++;
        if (conf_page < 8 * sectorsize / FLASH_CONF_SIZE) {
            mode = FLASH_ERASE_NEVER;
        } else {
            /* All pages used, force sector erase.*/
            sectorlist &= ~(1 << FlashAddr2Sector(flash_conf_sector));
            mode = FLASH_ERASE_ALWAYS;
            conf_page = 0;
        }
        rs = FlashWrite(flash_conf_sector + (conf_page * FLASH_CONF_SIZE),
                        buffer, FLASH_CONF_SIZE - sizeof(ERASED_PATTERN_32),
                        mode);
    } while ((rs != FLASH_COMPLETE) && conf_page);
done:
    NutHeapFree(buffer);
    /* Return success or fault code */
    return rs;
}

/*!
 * \brief Try to protect/unprotect the requested flash region.
 *
 * \param dst Pointer to address anywhere in FLASH.
 * \param len Length of region in bytes.
 * \param ena 0 disables write protection anything else write-protects.
 *
 * \return FLASH_Status: FLASH_COMPLETE or appropriate error.
 */
FLASH_Status IapFlashWriteProtect(void *dst, size_t len, int ena)
{
    uint32_t sector_start, sector_end;
    int i;
    uint32_t optcr;
    FLASH_Status rs = FLASH_COMPLETE;
    uint32_t flash_end_addr = FLASH_BASE + flashsize - 1;
#if defined(NUT_CONFIG_STM32_IAP)
    flash_end_addr -= 8 * sectorsize;
#endif
#if defined(FLASH_OPTCR_DB1M)
    uint32_t optcr1;
#endif
    /* Check boundaries */
    if ((((uint32_t)dst + len) > flash_end_addr) ||
        ((uint32_t)dst < FLASH_BASE)) {
        return FLASH_BOUNDARY;
    }

    if (len == 0) {
        return FLASH_COMPLETE;
    }
    /* Wait for last operation to be completed */
    rs = FlashWaitReady();
    if (rs != FLASH_COMPLETE) {
        return rs;
    }
    FLASH->OPTKEYR = FLASH_OPTKEY1;
    FLASH->OPTKEYR = FLASH_OPTKEY2;
    optcr = FLASH->OPTCR;
    if (optcr & FLASH_OPTCR_OPTLOCK) {
        return FLASH_ERROR_PG;
    }
#if defined(FLASH_OPTCR_DB1M)
    optcr1 = FLASH->OPTCR1;
    if (optcr1 & FLASH_OPTCR_OPTLOCK) {
        return FLASH_ERROR_PG;
    }
#endif
    sector_start = FlashAddr2Sector(dst);
    sector_end = FlashAddr2Sector(dst + len -1);
#if defined(MCU_STM32F2) ||defined(MCU_STM32F4)
    if (sector_end >= 16) {
        uint32_t bank2_start = (sector_start < 16) ? 16 : sector_start;
        for (i = bank2_start; i <= sector_end; i++) {
            uint32_t bit = 1 << (i - 15 + FLASH_OPTCR_nWRP_Pos);
            optcr |= bit;
        }
        sector_end = bank_split - 1;
    }
# if defined(STM32F413xx)
    if (sector_end >- bank_split - 2) {
        optcr |= FLASH_OPTCR_nWRP_14;
    }
# endif
    for (i = sector_start; i < sector_end; i++) {
        uint32_t bit = 1 << (i - 15 + FLASH_OPTCR_nWRP_Pos);
        optcr |= bit;
    }
# elif defined(MCU_STM32F7)
    if (!is_dual_bank) {
        for (i = sector_start; i <= sector_end; i++) {
            uint32_t bit = 1 << (i + FLASH_OPTCR_nWRP_Pos);
            optcr |= bit;
        }
    } else {
        /* Each bit protects two sectors! */
        if (sector_end >= 16) {
            int bank2_start = (sector_start < 16) ? 16 : sector_start;
            for (i = bank2_start;  i <= sector_end; i = i + 2) {
                uint32_t bit  = 1 << ((i - 16 + bank_split) / 2 * FLASH_OPTCR_nWRP_Pos);
                optcr |= bit;
            }
        }
        sector_end = bank_split - 1;
        for (i = sector_start; i <= sector_end; i = i + 2) {
            uint32_t bit  = 1 << (i / 2 + FLASH_OPTCR_nWRP_Pos);
            if ((optcr & bit) == 0) {
                return FLASH_ERROR_WRP;
            }
        }
    }
# endif
    FLASH->OPTCR = optcr;
#if defined(FLASH_OPTCR_DB1M)
    FLASH->OPTCR1 = optcr1;
#endif
    FLASH->OPTCR = optcr |FLASH_OPTCR_OPTSTRT;
    /* Wait for last operation to be completed */
    rs = FlashWaitReady();
    FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;
    return rs;
}
