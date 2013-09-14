/*
 * Copyright (C) 2012 Uwe Bonnes
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
#include <cfg/flash.h>
#include <sys/nutdebug.h>
#include <sys/heap.h>
#include <dev/iap_flash.h>

#if defined(MCU_STM32F2)
#include <arch/cm3/stm/vendor/stm32f2xx.h>
#elif defined(MCU_STM32F4)
#include <arch/cm3/stm/vendor/stm32f4xx.h>
#else
#warning "STM32 family has no F2/F4 compatible FLASH"
#endif

#define FLASH_PSIZE_8    0
#define FLASH_PSIZE_16   FLASH_CR_PSIZE_0
#define FLASH_PSIZE_32   FLASH_CR_PSIZE_1
#define FLASH_PSIZE_64   (FLASH_CR_PSIZE_0 | FLASH_CR_PSIZE_1)
#define FLASH_PSIZE_MASK (FLASH_PSIZE_64)
#define ERASED_PATTERN_32 0xffffffff

#define FLASH_CR_SNB_MASK (FLASH_CR_SNB_0 | FLASH_CR_SNB_1 | FLASH_CR_SNB_2 |\
                           FLASH_CR_SNB_3)

/* Datashet gives same register collision for flashsize and unique id (20121022)
 * Use 512k as safe workaround
 */
#if defined(FLASH_CONF_SECTOR)
#define FLASH_END_ADDR  (FLASH_BASE * 3 * 128 *1024)
#else
#define FLASH_END_ADDR  (FLASH_BASE * 4 * 128 *1024)
#endif

#if (FLASH_PE_PARALLELISM == 8)
#define FLASH_PSIZE       FLASH_PSIZE_8
#define FLASH_TYPE_CAST   (volatile uint8_t *)
#define FLASH_LEN_MASK    0
#elif (FLASH_PE_PARALLELISM == 16)
#define FLASH_PSIZE       FLASH_PSIZE_16
#define FLASH_TYPE_CAST   (volatile uint16_t *)
#define FLASH_LEN_MASK    1
#else
#if (FLASH_PE_PARALLELISM == 64)
#define FLASH_TYPE_CAST   (volatile uint64_t *)
#define FLASH_PSIZE       FLASH_PSIZE_64
#define FLASH_LEN_MASK    7
#else
#define FLASH_TYPE_CAST   (volatile uint32_t *)
#define FLASH_PSIZE       FLASH_PSIZE_32
#define FLASH_LEN_MASK    3
#endif
#endif

#define FLASH_KEY1 0x45670123L
#define FLASH_KEY2 0xCDEF89ABL

#define FLASH_OPTKEY1 0x08192A3B
#define FLASH_OPTKEY2 0x4C5D6E7F

/*!
 *\brief Bitmask of sectors either empty or active erased when first touched
 */
static uint32_t sectorlist = 0;

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
 *\brief Calculate sector number and sector length form address
 * \param Addr Address
 * \param length Pointer to write length of sector, if given
 *
 * \return Sector Number
 */
static uint32_t FlashAddr2Sector(void* Addr, uint32_t *length)
{
    uint32_t len;
    uint32_t sector;
    uint32_t addr = (uint32_t) Addr;
    if (addr < (FLASH_BASE + 0x10000))
    {
        len = 0x4000;
        sector = (addr - FLASH_BASE)/0x4000;
    }
    else if (addr < (FLASH_BASE + 0x20000))
    {
        len = 0x10000;
        sector = 4;
    }
    else
    {
        len = 0x20000;
        sector = ((addr - FLASH_BASE)/0x20000) + 4;
    }
    if (length)
        *length = len;
    return sector;
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
    if (FLASH->SR & (FLASH_SR_PGSERR | FLASH_SR_PGPERR | FLASH_SR_PGAERR))
        rs = FLASH_ERROR_PG;
    else if (FLASH->SR & FLASH_SR_WRPERR)
        rs = FLASH_ERROR_WRP;
    else if (FLASH->SR & FLASH_SR_BSY)
        rs = FLASH_BUSY;

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
    while(status == FLASH_BUSY);

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
    FLASH_Status rs = FLASH_COMPLETE;
    uint32_t cr = FLASH->CR & ~(FLASH_CR_SNB_MASK);

    /* Wait for last operation to be completed */
    rs = FlashWaitReady();
    if(rs == FLASH_COMPLETE) {
        /* if the previous operation is completed, proceed to erase the page */
        FLASH->CR = cr |FLASH_CR_SER | (sector * FLASH_CR_SNB_0);
        FLASH->CR |= FLASH_CR_STRT;

        /* Wait for last operation to be completed */
        rs = FlashWaitReady();
        if(rs != FLASH_TIMEOUT) {
            /* if the erase operation is completed, disable the PER Bit */
            FLASH->CR &= ~FLASH_CR_SER;
        }
    }

    /* Return the Erase Status */
    return (int)rs;
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
 *
 * \return FLASH Status: FLASH_COMPLETE or appropriate error.
 */
FLASH_Status IapFlashWrite( void* dst, void* src, size_t len)
{
    FLASH_Status rs = FLASH_COMPLETE;
    uint32_t cr = FLASH->CR & ~FLASH_PSIZE_MASK;
    uint32_t sector_start, sector_end;
    int i;
    uint32_t optcr = FLASH->OPTCR;
    void *wptr = dst;
    void *rptr = src;
    uint32_t length = len;

    /* we need to run with at least 1 MHz for flashing*/
    NUTASSERT((SysTick->LOAD +1)/NUT_TICK_FREQ > 0);

    if (len == 0)
        return FLASH_COMPLETE;

    /* Check top boundary */
    if ((((uint32_t)dst+len) > FLASH_END_ADDR) || ((uint32_t)dst < FLASH_BASE))
    {
        return FLASH_BOUNDARY;
    }

    /* Unlock related banks */
    if ((rs = FLASH_Unlock()) != FLASH_COMPLETE)
    {
        /* Unlocking failed for any reason */
        return FLASH_LOCKED;
    }

    /* Check for write protected sectors */
    sector_start = FlashAddr2Sector(dst, NULL);
    sector_end = FlashAddr2Sector(dst+len, NULL);
    for (i = sector_start; i < sector_end; i++)
        if ((optcr & (1<<(i +_BI32(FLASH_OPTCR_nWRP_0)))) == 0)
            return FLASH_ERROR_WRP;
    if (src == NULL)
        /* Only a check for write protection was requested */
        return  FLASH_COMPLETE;

    /* Set Program/erase parallelism for all subsequent actions.
     * FLASH_PSIZE defined the maximum width of the flash access,
     * smaller access is possible
     */
    FLASH->CR = cr | FLASH_PSIZE;

    while( (length) && (rs==FLASH_COMPLETE))
    {
        uint32_t sector_length;
        uint32_t sector_nr;
        uint32_t current_length = length;
        sector_nr = FlashAddr2Sector(wptr, &sector_length);
        if (current_length > sector_length)
            current_length = sector_length;
        length -= current_length;
        /* Check if sector needs erase */
        if ((sectorlist & (1<<sector_nr)) == 0)
        {
            volatile uint32_t *ptr = wptr;
            /* FLASH_PSIZE only affects erase/programming */
            for (i = 0; i< (sector_length/4); i++)
                if ( *ptr++ != ERASED_PATTERN_32)
                    break;
            if ((i < (sector_length/4)) &&
                ((rs = FlashEraseSector(sector_nr)) != FLASH_COMPLETE))
                /* Unlocking failed for any reason */
                return rs;
            sectorlist |= (1<<sector_nr);
        }
        /* Program the sector */
        rs = FlashWaitReady();
        if (rs != FLASH_COMPLETE)
            return rs;
        else
        {
             /* Enable Programming Mode */
            FLASH->CR =  cr | FLASH_CR_PG ;

            /* Write data to page */
            /* First, align to 2/4/8 Byte boundary */
            /* Run loop control before waiting for command to finish*/
            while(current_length && ((uint32_t)wptr & FLASH_LEN_MASK))
            {
                rs = FlashWaitReady();
                *(volatile uint8_t*)wptr++ = *(volatile uint8_t*)rptr++;
                current_length--;
            }
            /* Write Bulk of data in requested width*/
            while((current_length > FLASH_LEN_MASK) && (rs == FLASH_COMPLETE))
            {
                rs = FlashWaitReady();
                * FLASH_TYPE_CAST wptr++ = * FLASH_TYPE_CAST rptr++;
                current_length -= (FLASH_LEN_MASK +1);
            }
            while ((current_length > 0) && (rs == FLASH_COMPLETE))
            {
                rs = FlashWaitReady();
                *(volatile uint8_t*)wptr++ = *(volatile uint8_t*)rptr++;
                current_length --;
            }

            rs = FlashWaitReady();
            FLASH->CR = cr & ~FLASH_CR_PG;
            if(rs != FLASH_COMPLETE)
                return rs;
        }
    }

    /* Lock the FLASH again */
    FLASH->CR |= FLASH_CR_LOCK;

    /* Check the written data */
    wptr = dst;
    rptr = src;
    length = len;
    /* Align flash access to 4 Byte Boundary*/
    while (length && ((uint32_t)wptr & FLASH_LEN_MASK) && (rs==FLASH_COMPLETE))
        if(*(volatile uint8_t*)wptr++ != *(uint8_t*)rptr++)
        {
            rs = FLASH_COMPARE;
            length--;
        }
    /* Now compare 1/2/4 Byte at a time*/
    while (length > FLASH_LEN_MASK && (rs==FLASH_COMPLETE))
        if( * FLASH_TYPE_CAST wptr++  != * FLASH_TYPE_CAST rptr++)
        {
            rs = FLASH_COMPARE;
            length -= (FLASH_LEN_MASK+1);
        }
    while (length && (rs==FLASH_COMPLETE))
        if((*(volatile uint8_t*)wptr++) != *(uint8_t*)rptr++)
        {
            rs = FLASH_COMPARE;
            length--;
         }

    return rs;
}

/*!
 * \brief Nut/OS specific handling for parameters in FLASH.
 *
 * This function enables to read system specific parameters
 * from processors FLASH. The sectors used for storage are
 * configureable via nutconf.
 * As the upper sectors of the F2/4 are 128 kByte big, we
 * use a rolling scheme to write  a FLASH_CONF_SIZE
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
FLASH_Status FlashParamRead(uint32_t pos, void *data, size_t len)
{
#if !defined(FLASH_CONF_SECTOR) || !defined(FLASH_CONF_SIZE)
    return FLASH_NOT_IMPLEMENTED;
#else
    uint8_t  conf_page = 0;
    uint32_t marker = *(uint32_t*) (FLASH_CONF_SECTOR + conf_page * FLASH_CONF_SIZE - sizeof(ERASED_PATTERN_32));

    if (len == 0)
        return FLASH_COMPLETE;

    /* Check boundaries */
    if (pos + len + sizeof(ERASED_PATTERN_32) > FLASH_CONF_SIZE)
    {
        return FLASH_BOUNDARY;
    }

    /* Find configuration page in CONF_SECTOR*/
    while ((marker !=  ERASED_PATTERN_32) && conf_page < (1<<17/FLASH_CONF_SIZE)) {
        conf_page++;
        marker = *(uint32_t*)(FLASH_CONF_SECTOR + conf_page * FLASH_CONF_SIZE - sizeof(ERASED_PATTERN_32));
    }
    if (marker !=  ERASED_PATTERN_32)
        /* no page sizes unit in CONF_SECTOR has a valid mark */
        return FLASH_ERR_CONF_LAYOUT;

    memcpy( data, (uint8_t *)((uint8_t*)FLASH_CONF_SECTOR + conf_page * FLASH_CONF_SIZE + pos), len);

    /* Return success or fault code */
    return FLASH_COMPLETE;
#endif
}

/*!
 * \brief Nut/OS specific handling for parameters in FLASH.
 *
 * This function enables to store system specific parameters
 * in processors FLASH. The sectors used for storage are
 * configurable via nutconf.
 *
 * FIXME: At the F2/F4 last sector is quite much bigger than
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
FLASH_Status FlashParamWrite(unsigned int pos, const void *data,
                                  size_t len)
{
#if !defined(FLASH_CONF_SECTOR) || !defined(FLASH_CONF_SIZE)
    return FLASH_NOT_IMPLEMENTED;
#else
    FLASH_Status rs = 0;
    uint8_t *buffer;
    uint8_t  conf_page = 0, *men;
    uint32_t marker = *(uint32_t*) (FLASH_CONF_SECTOR + conf_page * FLASH_CONF_SIZE - sizeof(ERASED_PATTERN_32));
    int i;

    if (len == 0)
        return FLASH_COMPLETE;

    /* Check top boundaries */
    if (pos + len + sizeof(ERASED_PATTERN_32) > FLASH_CONF_SIZE)
    {
        return FLASH_BOUNDARY;
    }

    /* Find configuration page in CONF_SECTOR*/
    while ((marker !=  ERASED_PATTERN_32) && conf_page < (1<<17/FLASH_CONF_SIZE))
        conf_page++;
    if (marker !=  ERASED_PATTERN_32) {
        /* no page sizes unit in CONF_SECTOR has a valid mark
         * Erase Sector and write provided data to pso at first sector */

        /* Mark the sector as not yet erases to force erase*/
        sectorlist &= ~(1<<FlashAddr2Sector(FLASH_CONF_SECTOR, NULL));
        /* write the onlye the data fiven to this function*/
        rs = FlashWrite( FLASH_CONF_SECTOR + pos, data, len);
        /* Return success or fault code */
        return rs;
    }

    /* Check if target area is erased.
     * It seems no C standard function provides this functionality!
     */
    mem = (uint8_t*) (FLASH_CONF_SECTOR + conf_page * FLASH_CONF_SIZE + pos);
    for (i = 0; i < len; i++) {
        if (mem[i] != 0xff)
            break;
    }
    if (i >= len) {
        /* Needed area is erased, simply write the data to the requested area*/
        rs = FlashWrite( FLASH_CONF_SECTOR + conf_page * FLASH_CONF_SIZE + pos, data, len);
        return rs;
    }

    /* Check if content needs no update. */
    if (memcmp(FLASH_CONF_SECTOR + conf_page * FLASH_CONF_SIZE + pos, data, len) == 0)
        return FLASH_COMPLETE;

    /* Save configuration page in RAM and write updated data to next configuration page,
     * eventually erasing the sector wrapping to the first page
     */
    buffer = NutHeapAlloc(FLASH_CONF_SIZE);
    if (buffer == NULL)
    {
        /* Not enough memory */
        return FLASH_OUT_OF_MEMORY;
    }
    /* Get the content of the whole config sector*/
    memcpy( buffer, FLASH_CONF_SECTOR + conf_page * FLASH_CONF_SIZE , FLASH_CONF_SIZE);
    /* Overwrite new data region*/
    memcpy (buffer + pos, data, len);
    conf_page++;
    if (conf_page > 1<<17/FLASH_CONF_SIZE) {
        /* Mark the sector as not yet erases to force erase*/
        sectorlist &= ~(1<<FlashAddr2Sector(FLASH_CONF_SECTOR, NULL));
        rs = FlashWrite( FLASH_CONF_SECTOR + conf_page * FLASH_CONF_SIZE , buffer, FLASH_CONF_SIZE);
        conf_page = 0;
    }
    else {
        uint32_t indicator = 0;
        /* invalidate old configuration page */
        rs = FlashWrite( FLASH_CONF_SECTOR + conf_page * FLASH_CONF_SIZE - 4, &indicator, 0);
    }
    /* write the Page */
    rs = FlashWrite( FLASH_CONF_SECTOR + conf_page * FLASH_CONF_SIZE , buffer, FLASH_CONF_SIZE);
    NutHeapFree(buffer);
    /* Return success or fault code */
    return rs;
}
#endif
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

    if (len == 0)
        return FLASH_COMPLETE;

    /* Check top boundary */
    if ((((uint32_t)dst+len) > FLASH_END_ADDR) || ((uint32_t)dst < FLASH_BASE))
    {
        return FLASH_BOUNDARY;
    }

    /* Wait for last operation to be completed */
    rs = FlashWaitReady();
    if(rs != FLASH_COMPLETE)
        return rs;
    FLASH->OPTKEYR = FLASH_OPTKEY1;
    FLASH->OPTKEYR = FLASH_OPTKEY2;
    optcr = FLASH->OPTCR;
    if (optcr & FLASH_OPTCR_OPTLOCK)
        return FLASH_ERROR_PG;
    sector_start = FlashAddr2Sector(dst, NULL);
    sector_end = FlashAddr2Sector(dst+len, NULL);
    for (i = sector_start; i < sector_end; i++)
        if (ena)
            optcr &= ~(1<<(i + _BI32(FLASH_OPTCR_nWRP_0)));
        else
            optcr |=  (1<<(i + _BI32(FLASH_OPTCR_nWRP_0)));
    FLASH->OPTCR = optcr;
    FLASH->OPTCR = optcr |FLASH_OPTCR_OPTSTRT;
    /* Wait for last operation to be completed */
    rs = FlashWaitReady();
    FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;
    return rs;
}
