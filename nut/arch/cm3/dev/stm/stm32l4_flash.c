/*
 * Copyright (C) 2015 - 2020 Uwe Bonnes
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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/nutdebug.h>
#include <sys/atom.h>

#include <cfg/arch.h>
#include <cfg/eeprom.h>

#include <arch/cm3.h>
#include <dev/iap_flash.h>

#define ERASED_PATTERN_64  ((uint64_t)FLASH_ERASED_PATTERN32 << 32 | FLASH_ERASED_PATTERN32)

uint32_t program_end_raw;

static uint8_t flash_page_shift = 11;
static uint16_t flash_size;
#undef FLASH_PAGE_MASK
#define FLASH_PAGE_MASK ((1 << flash_page_shift) -1)
#undef FLASH_PAGE_SIZE
#define FLASH_PAGE_SIZE  (1 << flash_page_shift)

#define MAX_PAGES_IN_BANK_MASK (FLASH_CR_PNB_Msk >> FLASH_CR_PNB_Pos)
#if defined(FLASH_CR_BKER)
# define NUM_PAGES (2 * (MAX_PAGES_IN_BANK_MASK + 1))
#else
# define NUM_PAGES      (MAX_PAGES_IN_BANK_MASK + 1)
#endif
static uint32_t pagelist[NUM_PAGES / 32];

#define FLASH_KEY1 0x45670123L
#define FLASH_KEY2 0xCDEF89ABL

#define FLASH_OPTKEY1 0x08192A3B
#define FLASH_OPTKEY2 0x4C5D6E7F

#if defined NUT_CONFIG_STM32_IAP
# if !defined(FLASH_CONF_SIZE)
#  define FLASH_CONF_SIZE 256
# elif FLASH_CONF_SIZE > 512
#  warning Only up to 512 byte config space supported
# endif
#endif

#define FLASH_ALIGNMENT 7

/* Called from init. */
void FlashUntouch(void)
{
    flash_size = (*(__I uint16_t *) FLASHSIZE_BASE & 0xfff);
#if defined(FLASH_OPTR_DBANK)
# if defined(FLASH_OPTR_DB1M)
    if (!(FLASH->OPTR & FLASH_OPTR_DBANK)) {
        flash_page_shift = 13;
    } else {
        flash_page_shift = 12;
    }
# else
    if (!(FLASH->OPTR & FLASH_OPTR_DBANK)) {
        flash_page_shift = 12;
    } else {
        flash_page_shift = 11;
    }
# endif
#endif
    memset(pagelist, 0, sizeof(pagelist));
}

static uint32_t FlashEnd(void)
{
    uint32_t size;
    /* Early silicon has only 12 valid bits for the flash size*/
    size = flash_size * 1024;
    return FLASH_BASE + size;
}

size_t IapFlashEnd(void)
{
    uint32_t prog_flash_end;
    prog_flash_end = FlashEnd();
#if defined(NUT_CONFIG_STM32_IAP)
/* Always reserve space for a full parameter set*/
    prog_flash_end -= FLASH_PAGE_SIZE;
#endif
    return prog_flash_end - 1;
}

/*!
 * \brief Get start address in first size page above program storage.
 *
 * \param NONE
 * \return Start address in first page above program storage.
 */
extern size_t __end_rom;

size_t IapProgramEnd(void)
{
    size_t program_end = (size_t)&__end_rom;
    program_end +=  FLASH_PAGE_MASK;
    program_end &=  FLASH_PAGE_MASK;
    return program_end;
}

/*!
 * \brief Return pagesize of given address.
 *
 * \param addr Address
 * \return Size of current page.
 */
size_t IapPageSize(size_t addr)
{
    return FLASH_PAGE_SIZE;
}

/*!
  * \brief  Unlocks the FLASH Program Erase Controller.
  * \retval 0 on success, FLASH_LOCKED else.
  */
static FLASH_Status FLASH_Unlock( void )
{
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
    return (FLASH->CR & FLASH_CR_LOCK)?FLASH_LOCKED:FLASH_COMPLETE;
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
    const uint32_t pg_mask = FLASH_SR_PROGERR | FLASH_SR_SIZERR | FLASH_SR_PGAERR |
        FLASH_SR_PGSERR;

    /* Decode the Flash Status
     * Check BSY last, so maybe it has completed meanwhile*/
    uint32_t sr = FLASH->SR;
    if (sr & pg_mask) {
        rs |= FLASH_ERROR_PG;
    }
    if (sr & FLASH_SR_WRPERR) {
        rs |= FLASH_ERROR_WRP;
    }
    if (sr & FLASH_SR_BSY) {
        rs |= FLASH_BUSY;
    }
    FLASH->SR = sr; /* Clear Errors */
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
    while (status & FLASH_BUSY);

    /* Return the operation status */
    return status;
}

/*!
 * \brief Erase specified FLASH sector. Use "Program memory page erase"
 *
 * \param address_in_page: Some address in the page to erase.
 *
 * \return FLASH Status: FLASH_COMPLETE or appropriate error.
 */
static FLASH_Status FlashErasePage(int sector)
{
    FLASH_Status rs = FLASH_COMPLETE;
    uint64_t *content;
    int i;

    content = (uint64_t*)((sector << flash_page_shift) + FLASH_BASE);
    /* Check, if page really needs erase */
    for (i = 0; i < (1 << (flash_page_shift - 3)) ; i++) {
        if (content[i] != ERASED_PATTERN_64)
            break;
    }
    if ( i >= (1 << (flash_page_shift -3))) {
        rs = FLASH_COMPLETE;
    } else {
        /* Wait for last operation to be completed */
        rs = FlashWaitReady();
        if (rs == FLASH_COMPLETE) {
            uint32_t cr;

            cr = FLASH_CR_PER | ((sector & MAX_PAGES_IN_BANK_MASK) << 3);
            if (sector > MAX_PAGES_IN_BANK_MASK) {
#if defined(FLASH_CR_BKER)
                cr |= FLASH_CR_BKER;
 #else
                return FLASH_ERROR_PG;
#endif
            }
            FLASH->CR = cr;
            FLASH->CR = cr | FLASH_CR_STRT;
            rs = FlashWaitReady();
#if defined(FLASH_ACR_DCEN)
            /* D-Cache was filled with page empty check above.
             * Reset D-Cache so that compare gets good data.*/
            uint32_t acr = FLASH->ACR;
            FLASH->ACR = acr & ~FLASH_ACR_DCEN;
            FLASH->ACR |= FLASH_ACR_DCRST;
            FLASH->ACR = acr;
#endif
            if (rs != FLASH_COMPLETE) {
                return rs;
            }
        }
    }
    FLASH->CR = 0;
    if (rs != FLASH_COMPLETE ) {
        pagelist[sector / 32] &= ~(1 <<(sector % 32));
    } else {
        pagelist[sector / 32] |=  (1 <<(sector % 32));
    }
    /* Return the Erase Status */
    return rs;
}

/*!
 * \brief Program any data to FLASH.
 *
 * \param dst Pointer to address anywhere in FLASH.
 * \param src Pointer to source data. With SRC == NULL, the region
 *        is checked for write protection
 * \param len Number of bytes to be written/checked.
 * \param mode Erase mode (Always, on first access to block, never).
 *
 * \return FLASH Status: FLASH_COMPLETE or appropriate error.
 */
FLASH_Status IapFlashWrite( void* dst, const void* src, size_t len,
                                FLASH_ERASE_MODE mode)
{
    FLASH_Status rs = FLASH_COMPLETE;
    uint16_t sector, sector_start, sector_end;
    uint64_t *wptr = (uint64_t *)dst;
    const uint64_t *rptr = (const uint64_t *)src;

    if (len == 0) {
        return FLASH_COMPLETE;
    }
    if ((uint32_t)dst & FLASH_ALIGNMENT ) {
        return FLASH_ERR_ALIGNMENT;
    }
    if ((mode == FLASH_ERASE_NEVER) && (len & FLASH_ALIGNMENT)) {
        /* Check length only when no erase was performed.
         * Otherwise extent even as we write garbage.
         * We can not overwrite in any case.*/
        return FLASH_ERR_ALIGNMENT;
    }
    /* Check top boundary */
    if ((((uint32_t)dst - 1 + len) > FlashEnd()) ||
        ((uint32_t)dst < FLASH_BASE)) {
        return FLASH_BOUNDARY;
    }

    /* Check for write protected sectors */
    sector_start = ((uint32_t)dst - FLASH_BASE) >> flash_page_shift;
    sector_end = ((uint32_t)dst + len - FLASH_BASE - 1) >> flash_page_shift;
    if (sector_end <= MAX_PAGES_IN_BANK_MASK) {
        uint8_t area1a_start;
        uint8_t area1a_end;
        uint8_t area1b_start;
        uint8_t area1b_end;

        area1a_start =  FLASH->WRP1AR & FLASH_WRP1AR_WRP1A_STRT;
        area1a_end   = (FLASH->WRP1AR & FLASH_WRP1AR_WRP1A_END) >> 16;
        area1b_start =  FLASH->WRP1BR & FLASH_WRP1BR_WRP1B_STRT;
        area1b_end   = (FLASH->WRP1BR & FLASH_WRP1BR_WRP1B_END) >> 16;
        if (((sector_start >= area1a_start) && (sector_end <= area1a_end)) ||
            ((sector_start >= area1b_start) && (sector_end <= area1b_end))) {
            rs = FLASH_ERROR_WRP;
            goto done;
        }
#if defined(FLASH_CR_BKER)
    } else if (sector_start > MAX_PAGES_IN_BANK_MASK) {
        uint8_t area2a_start;
        uint8_t area2a_end;
        uint8_t area2b_start;
        uint8_t area2b_end;

        sector_start &= MAX_PAGES_IN_BANK_MASK;
        sector_end &= MAX_PAGES_IN_BANK_MASK;
        area2a_start =  FLASH->WRP2AR & FLASH_WRP2AR_WRP2A_STRT;
        area2a_end   = (FLASH->WRP2AR & FLASH_WRP2AR_WRP2A_END) >> 16;
        area2b_start =  FLASH->WRP2BR & FLASH_WRP2BR_WRP2B_STRT;
        area2b_end   = (FLASH->WRP2BR & FLASH_WRP2BR_WRP2B_END) >> 16;
        if (((sector_start >= area2a_start) && (sector_end <= area2a_end)) ||
            ((sector_start >= area2b_start) && (sector_end <= area2b_end))) {
            rs = FLASH_ERROR_WRP;
            goto done;
        }
    } else {
        uint8_t area1a_start;
        uint8_t area1a_end;
        uint8_t area1b_start;
        uint8_t area1b_end;
        uint8_t area2a_start;
        uint8_t area2a_end;
        uint8_t area2b_start;
        uint8_t area2b_end;


        area1a_start =  FLASH->WRP1AR & FLASH_WRP1AR_WRP1A_STRT;
        area1a_end   = (FLASH->WRP1AR & FLASH_WRP1AR_WRP1A_END) >> 16;
        area1b_start =  FLASH->WRP1BR & FLASH_WRP1BR_WRP1B_STRT;
        area1b_end   = (FLASH->WRP1BR & FLASH_WRP1BR_WRP1B_END) >> 16;
        area2a_start =  FLASH->WRP2AR & FLASH_WRP2AR_WRP2A_STRT;
        area2a_end   = (FLASH->WRP2AR & FLASH_WRP2AR_WRP2A_END) >> 16;
        area2b_start =  FLASH->WRP2BR & FLASH_WRP2BR_WRP2B_STRT;
        area2b_end   = (FLASH->WRP2BR & FLASH_WRP2BR_WRP2B_END) >> 16;

        if (((sector_start >= area1a_start) && (area1a_start <= area1a_end)) ||
            ((sector_start >= area1b_start) && (area1b_start <= area1b_end))) {
            rs = FLASH_ERROR_WRP;
            goto done;
        }

        sector_start &= MAX_PAGES_IN_BANK_MASK;
        sector_end &= MAX_PAGES_IN_BANK_MASK;
        if (((area2a_start <= sector_end) && (area2a_start <= area2a_end)) ||
            ((area2b_start <= sector_end) && (area2b_start <= area2b_end))) {
            rs = FLASH_ERROR_WRP;
            goto done;
        }
#endif
    }

    if (src == NULL) {
        /* Only a check for write protection was requested */
        rs =  FLASH_COMPLETE;
        goto done;
    }

    rs = FLASH_Unlock();
    if (rs != FLASH_COMPLETE)
    {
        /* Unlocking failed for any reason */
        return FLASH_LOCKED;
    }

    /* Erase related pages*/
    sector_start = ((uint32_t)dst - FLASH_BASE) >> flash_page_shift;
    sector_end = ((uint32_t)dst + len - FLASH_BASE - 1) >> flash_page_shift;
    for (sector = sector_start; sector <= sector_end; sector ++) {
        int page_was_touched;

        page_was_touched = pagelist[sector / 32] & (1 <<(sector % 32));
        if (( mode == FLASH_ERASE_ALWAYS) ||
            ((mode == FLASH_ERASE_FIRST_TOUCH) && !(page_was_touched))) {
            rs = FlashErasePage(sector);
            if (rs != FLASH_COMPLETE) {
                goto done;
            }
        }
    }

    /* Write content*/
    wptr = (uint64_t *)dst;
    rptr = (const uint64_t *)src;
    rs = FlashWaitReady();
    uint32_t length = (len + FLASH_ALIGNMENT) & ~FLASH_ALIGNMENT;
    NutEnterCritical();
    FLASH->CR = FLASH_CR_PG; /* Start memory programming.*/
    while ((length) && (rs == FLASH_COMPLETE)) {
        /* Check for same data. Care for D-Cache in page erase.*/
        if (1 || (*wptr != *rptr)) {
            *wptr = *rptr;
            rs = FlashWaitReady();
            if ((rs != FLASH_COMPLETE) || (*wptr != *rptr)){
                /* FIXME: Programming failed*/
                if (*wptr == ERASED_PATTERN_64) { /* Try again */
                    *wptr = *rptr;
                    rs = FlashWaitReady();
                }
            }
        }
        wptr++;
        rptr++;
        length -= 8;
    }
    FLASH->CR = 0; /* Stop memory programming.*/
    NutExitCritical();
    if (rs != FLASH_COMPLETE) {
        return rs;
    }
    /* Check the written data */
    wptr = (uint64_t *)dst;
    rptr = (const uint64_t *)src;
    length = (len + FLASH_ALIGNMENT) & ~FLASH_ALIGNMENT;
    while (length) {
        if (*wptr != *rptr) {
            rs = FLASH_COMPARE;
            goto done;
        }
        wptr++;
        rptr++;
        length -= 8;
    }
    rs = FLASH_COMPLETE;
done:
    FLASH->CR = FLASH_CR_LOCK;
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
    return FLASH_NOT_IMPLEMENTED;
}

#if defined(NUT_CONFIG_STM32_IAP)
/*!
 * \brief Nut/OS specific handling for parameters in FLASH.
 *
 * This function enables to read system specific parameters
 * from processors FLASH. The last flash sectors is used
 * for this area, and user configurable FLASH_CONF_SIZE must be
 * less than FLASH_PAGE_SIZE/2.
 *
 * As on L4 only DWORDs may be written, writing bytes would
 * always require an erase cycle. So we write DWORDs with the
 * content
 *  ~(pos & ~3) << 48 | (pos & ~3) << 32 | data[pos + 3] << 24 |
 *   data[pos +2] << 16 | data[pos + 1] << 8 | data[pos].
 * First write happens in ordered mode with DWORD[(pos & ~3)] = content.
 * Any change in data[pos] to data[pos +3] will cause:
 * - A lookup for (pos & ~3) in data area
 * - Read the content there
 * - Invalidate the old entry by writing 0LL
 * - Update data to new value
 * - Writing new data to the first erased entry above the
 *   ordered area
 *
 * If no free entry is available in the first page:
 * - Move data from unordered area to ordered area
 * - Erase original page
 * - Write reordered data to this page
 *
 *
 * \param pos Offset of parameter(s) in configured page(s).
 * \param data Pointer where to copy data from flash to.
 * \param len Number of bytes to be copied.
 *
 * \return FLASH_Status: FLASH_COMPLETE or appropriate error.
 */
FLASH_Status Stm32FlashParamRead(uint32_t pos, void *data, size_t len)
{
    uint32_t confpage;
    uint16_t current_pos, slot;
    int offset, length;

    if (len == 0) {
        return FLASH_COMPLETE;
    }
    if ((pos + len) > FLASH_CONF_SIZE) {
        return FLASH_BOUNDARY;
    }
    /* Return 0xff for data not found*/
    memset(data, 0xff, len);

    confpage = IapFlashEnd() + 1;
    offset = pos & 3;
    length = len;
    current_pos = pos & ~3;
    for (; length > 0; current_pos = current_pos + 4) {
        uint64_t value;
        uint64_t *slotdata = (uint64_t*) confpage;
        size_t n_bytes;

        for (slot = 0; slot  < FLASH_PAGE_SIZE / 8; slot++) {
            value = slotdata[slot];
            if (( (value >> 32) & 0xffff) != current_pos) {
                continue;
            }
            if ((~(value >> 48) & 0xffff) != current_pos) {
                /* Invalid ~slot/slot combination.*/
                continue;
            }
            break;
        }

        n_bytes = length;
        if (n_bytes > 4) {
            n_bytes = 4;
        }
        if (n_bytes > 4 - offset) {
            n_bytes = 4 - offset;
        }
        if (slot < FLASH_PAGE_SIZE / 8) {
            memcpy(data, &value + offset, n_bytes);
        }
        data   += n_bytes;
        length -= n_bytes;
        offset = 0;
    }
    return FLASH_COMPLETE;
}

/*!
 * \brief Nut/OS specific handling for parameters in FLASH.
 *
 * This function enables to store system specific parameters
 * in processors FLASH. The number of bytes used for storage is
 * configurable via nutconf as FLASH_CONF_SIZE.
 *
 * Data is written as 8-byte words, containing 4 bytes of config
 * data. Structure is as
 * ~(pos & ~3) << 48 | (pos & ~3) << 32| byte[3] << 24 |
 *    byte[2] << 16 | byte[1] << 8 | byte[0]
 *
 * Empty tuples are all 0xff, erase data tuples with  all 0.
 *
 * Tuples are writting from bottom up. If a tuple is to be replaced,
 * it's content is read and the content replaced with the new bytes.
 * A empty slot is searched from bottom up and the new tuple stored there.
 * If no empty slot is found, data is consolidated and flash sector written
 * with consolidated data after erase.
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
    uint32_t confpage;
    uint16_t current_pos;
    int rs, offset, length;
    const uint64_t null_tuple = 0;

    if (len == 0) {
        return FLASH_COMPLETE;
    }
    if ((pos + len) > FLASH_CONF_SIZE) {
        return FLASH_BOUNDARY;
    }

    confpage = IapFlashEnd() + 1;

    offset = pos & 3;
    length = len;
    current_pos = pos & ~3;
    rs = 0;
    for (; length > 0 && !rs;
         current_pos += 4, offset = 0) {
        uint16_t slot, moveslot;
        uint32_t slotdata = 0;
        uint64_t data_tuple;
        uint64_t *confdata;
        uint64_t *consolidated;
        int fitslot;
        int n_bytes;

        fitslot = -1;
        confdata = (uint64_t*) confpage;
        for (slot = 0; slot < FLASH_PAGE_SIZE / 8; slot++) {
            uint16_t *slotaddress = (uint16_t*) confpage;
            uint16_t search_pos;
            /* Skip erased slots */
            if (confdata[slot] == ERASED_PATTERN_64) {
                continue;
            }
            if (!confdata[slot]) {
                continue;
            }
            search_pos = slotaddress[4 * slot + 2];
            if ((~slotaddress[4 * slot + 3] & 0xffff) != search_pos) {
                /* Invalid ~slot/slot combination. Erase data!*/
                rs = IapFlashWrite((void*)confpage + 8 *slot, &null_tuple,
                                       8, FLASH_ERASE_NEVER);
                continue;
            }
            if (search_pos != current_pos) {
                continue;
            }
            if (fitslot != -1) {
                /* We found a double data set. Erase earlier occurance*/
                rs = IapFlashWrite((void*)confpage + 8 * fitslot,
                                   &null_tuple, 8, FLASH_ERASE_NEVER);
            }
            fitslot = slot;
        }
        n_bytes = length;
        if (n_bytes > 4) {
            n_bytes = 4;
        }
        if (n_bytes > 4 - offset) {
            n_bytes = 4 - offset;
        }
        if (fitslot != - 1) {
            uint32_t *slotcontent = (uint32_t*)confpage;
            slotdata = slotcontent[fitslot * 2];
            rs = memcmp((void*)&slotdata + offset, data, n_bytes);
            if (rs) {
                int res;
                res = IapFlashWrite((void*)confpage + 8 * fitslot, &null_tuple,
                                   8, FLASH_ERASE_NEVER);
                if (res) {
                    continue;
                }
            }
        } else {
            rs = 1;
        }
        memcpy((void*)&slotdata + offset, data, n_bytes);
        data += n_bytes;
        length -= n_bytes;
        if (!rs) {
            /* Data didn't change. No need to write*/
            continue;
        }
        data_tuple = (uint64_t)~current_pos << 48 |
            (uint64_t)current_pos << 32 | slotdata;
        for (slot = 0; slot < FLASH_PAGE_SIZE / 8; slot++){
            if (confdata[slot] == ERASED_PATTERN_64) {
               break;
            }
        }
        if (slot <  FLASH_PAGE_SIZE / 8) {
           rs = IapFlashWrite((void*)confpage + 8 * slot, &data_tuple, 8,
                              FLASH_ERASE_NEVER);
           continue;
        }
        /* No more free slots. Move old data to consolidated and
           write data with FLASH_ERASE_ALWAYS*/
        consolidated = malloc(FLASH_CONF_SIZE * 2);
        if (!consolidated) {
            return FLASH_OUT_OF_MEMORY;
        }
        memset(consolidated, 0xff, FLASH_CONF_SIZE * 2);
        slot = 0;
        consolidated[slot++] = data_tuple;
        for (moveslot = 0; moveslot < FLASH_PAGE_SIZE / 8; moveslot++) {
            uint64_t movedata;
            movedata = confdata[moveslot];
            if (!movedata) {
                continue;
            }
            if (movedata == ERASED_PATTERN_64) {
                continue;
            }
            if ((~(movedata >> 48) & 0xffff) != ((movedata >> 32) & 0xffff)) {
                continue;
            }
            consolidated[slot++] = movedata;
        }
        rs = IapFlashWrite(confdata, (const void*) consolidated,
                           FLASH_CONF_SIZE * 2, FLASH_ERASE_ALWAYS);
        free(consolidated);
    }
    return rs;
}
#endif
