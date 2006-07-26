/*
 * Copyright (C) 2006 by egnite Software GmbH. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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

/*!
 * \file arch/arm/dev/at91_efc.c
 * \brief SAM7 embedded flash controller support.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/07/26 11:20:57  haraldkipp
 * Added non-volatile configuration memory support for SAM7X, using upper
 * 16k region of on-chip flash.
 *
 *
 * \endverbatim
 */

#include <stdlib.h>
#include <string.h>
#include <sys/atom.h>
#include <dev/nvmem.h>

#include <arch/arm/at91_efc.h>

/*! \brief Base address of the flash memory chip.
 */
#ifndef FLASH_CHIP_BASE
#define FLASH_CHIP_BASE  0x00100000
#endif

/*! \brief Address offset of the configuration sector.
 */
#ifndef FLASH_CONF_SECTOR
#define FLASH_CONF_SECTOR  0x0003FF00
#endif

/*! \brief Size of the configuration area.
 *
 * During write operations a buffer with this size is allocated
 * from heap and may cause memory problems with large sectors.
 * Thus, this value may be less than the size of the configuration 
 * sector, in which case the rest of the sector is unused.
 *
 * Currently only 1 sector can be used for system configurations.
 */
#ifndef FLASH_CONF_SIZE
#define FLASH_CONF_SIZE         256
#endif

#ifndef FLASH_WRITE_WAIT
#define FLASH_WRITE_WAIT        60000
#endif

#ifndef FLASH_ERASE_WAIT
#define FLASH_ERASE_WAIT        60000
#endif

#ifndef FLASH_CHIP_ERASE_WAIT
#define FLASH_CHIP_ERASE_WAIT   600000
#endif


typedef u_long flashdat_t;
typedef unsigned long flashadr_t;
typedef volatile flashdat_t *flashptr_t;

/*!
 * \brief Execute flash controller command.
 *
 * This routine must not be located in internal flash memory.
 *
 */
RAMFUNC int At91EfcCmd(u_int cmd, u_long tmo)
{
    int rc = 0;
    u_int fsr;

    /* Make sure that the previous command has finished. */
    while ((inr(MC_FSR) & MC_FRDY) == 0) {
        if (tmo && --tmo < 1) {
            return -1;
        }
    }

    /* IRQ handlers are located in flash. Disable them. */
    NutEnterCritical();

    /* Write command. */
    outr(MC_FCR, MC_KEY | cmd);

    /* Wait for ready flag set. */
    while (((fsr = inr(MC_FSR)) & MC_FRDY) == 0) {
        if (tmo && --tmo < 1) {
            rc = -1;
            break;
        }
    }

    /* Flash command finished. Re-enable IRQ handlers. */
    NutExitCritical();

    /* Check result. */
    if (fsr & (MC_LOCKE | MC_PROGE)) {
        rc = -1;
    }
    return rc;
}

/*!
 * \brief Read data from flash memory.
 *
 * \param off  Start location within the chip, starting at 0.
 * \param data Points to a buffer that receives the data.
 * \param len  Number of bytes to read.
 *
 * \return Always 0.
 */
int At91EfcSectorRead(u_int off, void *data, u_int len)
{
    memcpy(data, (void *) (uptr_t) (FLASH_CHIP_BASE + off), len);

    return 0;
}

/*!
 * \brief Write data into flash memory.
 *
 * The related sector will be automatically erased before writing.
 *
 * \param off  Start location within the chip, starting at 0.
 * \param data Points to a buffer that contains the bytes to be written.
 *             If this is a NULL pointer, then the sector will be reased.
 * \param len  Number of bytes to write, 1 full sector max.
 *
 * \return 0 on success or -1 in case of an error.
 */
int At91EfcSectorWrite(u_int off, CONST void *data, u_int len)
{
    flashptr_t dp = (flashptr_t) (uptr_t) (FLASH_CHIP_BASE + off);
    int rc;
    u_int i;

    if (data) {
        flashptr_t sp = (flashptr_t) data;

        /* Copy data to the flash write buffer. */
        for (i = 0; i < len; i += sizeof(flashdat_t)) {
            *dp++ = *sp++;
        }
    }
    else {
        /* All bits set to emulate sector erasing. */
        for (i = 0; i < len; i += sizeof(flashdat_t)) {
            *dp++ = (flashdat_t)(-1);
        }
    }

    /* Clear NEBP in mode register for automatic erasure. */
    outr(MC_FMR, (i = inr(MC_FMR)) & ~MC_NEBP);
    /* Execute page write command. */
    rc = At91EfcCmd((off & MC_PAGEN_MASK) | MC_FCMD_WP, FLASH_WRITE_WAIT);
    /* Restore mode register. */
    outr(MC_FMR, i);

    return rc;
}

/*!
 * \brief Erase sector at the specified offset.
 */
int At91EfcSectorErase(u_int off)
{
    return At91EfcSectorWrite(off, NULL, 256);
}

/*!
 * \brief Lock specified region.
 *
 * \param off Location within the region to be locked.
 *
 * \return 0 on success or -1 in case of an error.
 */
int At91EfcRegionLock(u_int off)
{
    return At91EfcCmd((off & MC_PAGEN_MASK) | MC_FCMD_SLB, FLASH_WRITE_WAIT);
}

/*!
 * \brief Unlock specified region.
 *
 * \param off Location within the region to be unlocked.
 *
 * \return 0 on success or -1 in case of an error.
 */
int At91EfcRegionUnlock(u_int off)
{
    return At91EfcCmd((off & MC_PAGEN_MASK) | MC_FCMD_CLB, FLASH_WRITE_WAIT);
}

/*!
 * \brief Load configuration parameters from embedded flash memory.
 *
 * Applications should call NutNvMemLoad().
 *
 * \param pos   Start location within configuration sector.
 * \param buff  Points to a buffer that receives the contents.
 * \param len   Number of bytes to read.
 *
 * \return Always 0.
 */
int At91EfcParamRead(u_int pos, void *data, u_int len)
{
    return At91EfcSectorRead(FLASH_CONF_SECTOR + pos, data, len);
}

/*!
 * \brief Store configuration parameters in embedded flash memory.
 *
 * Applications should call NutNvMemSave().
 *
 * The region that contains the configuration sector will be automatically 
 * locked.
 *
 * \param pos   Start location within configuration sector.
 * \param data  Points to a buffer that contains the bytes to store.
 * \param len   Number of bytes to store.
 *
 * \return 0 on success or -1 in case of an error.
 */
int At91EfcParamWrite(u_int pos, CONST void *data, u_int len)
{
    int rc = -1;
    u_char *buff;

    /* Load the complete configuration area. */
    if ((buff = malloc(FLASH_CONF_SIZE)) != NULL) {

        rc = At91EfcSectorRead(FLASH_CONF_SECTOR, buff, FLASH_CONF_SIZE);
        /* Compare old with new contents. */
        if (memcmp(buff + pos, data, len)) {
            /* New contents differs. Copy it into the sector buffer. */
            memcpy(buff + pos, data, len);
            /* Write back new data. Maintain region lock. */
            if (At91EfcRegionUnlock(FLASH_CONF_SECTOR) == 0) {
                rc = At91EfcSectorWrite(FLASH_CONF_SECTOR, buff, FLASH_CONF_SIZE);
                At91EfcRegionLock(FLASH_CONF_SECTOR);
            }
        }
        free(buff);
    }
    return rc;
}

/*@}*/
