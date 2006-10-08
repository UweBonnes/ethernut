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
 * \file dev/at45db.c
 * \brief Routines for Atmel AT45 serial dataflash memory chips.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.2  2006/10/08 16:48:09  haraldkipp
 * Documentation fixed
 *
 * Revision 1.1  2006/09/29 12:41:55  haraldkipp
 * Added support for AT45 serial DataFlash memory chips. Currently limited
 * to AT91 builds.
 *
 *
 * \endverbatim
 */

#include <cfg/os.h>
#include <cfg/memory.h>

#include <sys/timer.h>

#include <string.h>
#include <stdlib.h>

#include <dev/at91_spi.h>
#include <dev/at45db.h>

#ifndef MAX_AT45_DEVICES
#define MAX_AT45_DEVICES        1
#endif

#ifndef MAX_AT45_CMDLEN
#define MAX_AT45_CMDLEN         8
#endif

#ifndef AT45_CONF_DFSPI
#define AT45_CONF_DFSPI         SPI0_BASE
#endif

#ifndef AT45_CONF_DFPCS
#define AT45_CONF_DFPCS         1
#endif

#ifndef AT45_ERASE_WAIT
#define AT45_ERASE_WAIT         3000
#endif

#ifndef AT45_CHIP_ERASE_WAIT
#define AT45_CHIP_ERASE_WAIT    50000
#endif

#ifndef AT45_WRITE_POLLS
#define AT45_WRITE_POLLS        1000
#endif

#define DFCMD_READ_PAGE         0xD2    /* Read main memory page. */
#define DFCMD_READ_STATUS       0xD7    /* Read status register. */
#define DFCMD_CONT_READ         0xE8    /* Continuos read. */
#define DFCMD_PAGE_ERASE        0x81    /* Page erase. */
#define DFCMD_BUF1_WRITE        0x84    /* Buffer 1 write. */
#define DFCMD_BUF1_FLASH        0x83    /* Buffer 1 flash with page erase. */

/*!
 * \brief Known device type entry.
 */
typedef struct _AT45_DEVTAB {
    u_long devt_pages;
    u_int devt_pagsiz;
    u_int devt_offs;
    u_char devt_srmsk;
    u_char devt_srval;
} AT45_DEVTAB;

/*!
 * \brief Active device entry.
 */
typedef struct _AT45DB_DCB {
    AT45_DEVTAB *dcb_devt;
    u_int dcb_spibas;
    u_int dcb_spipcs;
    u_char dcb_cmdbuf[MAX_AT45_CMDLEN];
} AT45DB_DCB;

/*!
 * \brief Table of known Dataflash types.
 */
AT45_DEVTAB at45_devt[] = {
    {512, 264, 9, 0x3C, 0x0C},  /* AT45DB011B - 128kB */
    {1025, 264, 9, 0x3C, 0x14}, /* AT45DB021B - 256kB */
    {2048, 264, 9, 0x3C, 0x1C}, /* AT45DB041B - 512kB */
    {4096, 264, 9, 0x3C, 0x24}, /* AT45DB081B - 1MB */
    {4096, 528, 10, 0x3C, 0x2C},        /* AT45DB0161B - 2MB */
    {8192, 528, 10, 0x3C, 0x34},        /* AT45DB0321B - 4MB */
    {8192, 1056, 11, 0x38, 0x38},       /* AT45DB0642 - 8MB */
    {0, 0, 0, 0, 0}             /* End of table */
};

/*!
 * \brief Table of active devices.
 */
static AT45DB_DCB dcbtab[MAX_AT45_DEVICES];

/* Number of active chips. */
static int dcbnum;

/* Chip used for parameter storage. */
static int dd_param = -1;

int At45dbSendCmd(int dd, u_char op, u_long parm, int len, CONST void *tdata, void *rdata, int datalen)
{
    u_char *cb = dcbtab[dd].dcb_cmdbuf;

    if (len > MAX_AT45_CMDLEN) {
        return -1;
    }
    memset(cb, 0, len);
    cb[0] = op;
    if (parm) {
        cb[1] = (u_char) (parm >> 16);
        cb[2] = (u_char) (parm >> 8);
        cb[3] = (u_char) parm;
    }
    return At91SpiTransfer2(dcbtab[dd].dcb_spibas, dcbtab[dd].dcb_spipcs, cb, cb, len, tdata, rdata, datalen);
}

u_char At45dbGetStatus(int dd)
{
    u_char buf[2] = { DFCMD_READ_STATUS, 0xFF };

    if (At91SpiTransfer2(dcbtab[dd].dcb_spibas, dcbtab[dd].dcb_spipcs, buf, buf, 2, NULL, NULL, 0)) {
        return (u_char) - 1;
    }
    return buf[1];
}

/*!
 * \brief Wait until flash memory cycle finished.
 *
 * \return 0 on success or -1 in case of an error.
 */
int At45dbWaitReady(int dd, u_long tmo, int poll)
{
    u_char sr;

    while (((sr = At45dbGetStatus(dd)) & 0x80) == 0) {
        if (!poll) {
            NutSleep(1);
        }
        if (tmo-- == 0) {
            return -1;
        }
    }
    return 0;
}

/*!
 * \brief Initialize dataflash at specified interface and chip select.
 */
int At45dbInit(u_int spibas, u_int spipcs)
{
    int dd = -1;
    u_char sr;
    int i;

    for (i = 0; i < dcbnum; i++) {
        if (dcbtab[i].dcb_spibas == spibas && dcbtab[i].dcb_spipcs == spipcs) {
            return i;
        }
    }

    if (dcbnum >= MAX_AT45_DEVICES) {
        return -1;
    }

    At91SpiInit(spibas);
    At91SpiReset(spibas);
    At91SpiInitChipSelects(spibas, _BV(spipcs));
    At91SpiSetRate(spibas, spipcs, 1000000);
    At91SpiSetModeFlags(spibas, spipcs, SPIMF_MASTER | SPIMF_SCKIAHI | SPIMF_CAPRISE);

    dcbtab[dcbnum].dcb_spibas = spibas;
    dcbtab[dcbnum].dcb_spipcs = spipcs;
    sr = At45dbGetStatus(dcbnum);

    for (i = 0; at45_devt[i].devt_pages; i++) {
        if ((sr & at45_devt[i].devt_srmsk) == at45_devt[i].devt_srval) {
            dcbtab[dcbnum].dcb_devt = &at45_devt[i];
            dd = dcbnum++;
            break;
        }
    }
    return dd;
}

/*!
 * \brief Erase sector at the specified offset.
 */
int At45dbPageErase(int dd, u_int pgn)
{
    return At45dbSendCmd(dd, DFCMD_PAGE_ERASE, pgn, 4, NULL, NULL, 0);
}

/*!
 * \brief Erase entire flash memory chip.
 */
int At45dbChipErase(void)
{
    return -1;
}

/*!
 * \brief Read data from flash memory.
 *
 * \param dd   Device descriptor.
 * \param pgn  Page number to read, starting at 0.
 * \param data Points to a buffer that receives the data.
 * \param len  Number of bytes to read.
 *
 * \return 0 on success or -1 in case of an error.
 */
int At45dbPageRead(int dd, u_long pgn, void *data, u_int len)
{
    pgn <<= dcbtab[dd].dcb_devt->devt_offs;
    return At45dbSendCmd(dd, DFCMD_CONT_READ, pgn, 8, data, data, len);
}

/*!
 * \brief Write data into flash memory.
 *
 * The related sector must have been erased before calling this function.
 *
 * \param dd   Device descriptor.
 * \param pgn  Start location within the chip, starting at 0.
 * \param data Points to a buffer that contains the bytes to be written.
 * \param len  Number of bytes to write.
 *
 * \return 0 on success or -1 in case of an error.
 */
int At45dbPageWrite(int dd, u_int pgn, CONST void *data, u_int len)
{
    int rc = -1;
    void *rp;

    if ((rp = malloc(len)) != NULL) {
        /* Copy data to dataflash RAM buffer. */
        if (At45dbSendCmd(dd, DFCMD_BUF1_WRITE, 0, 4, data, rp, len) == 0) {
            /* Flash RAM buffer. */
            pgn <<= dcbtab[dd].dcb_devt->devt_offs;
            if (At45dbSendCmd(dd, DFCMD_BUF1_FLASH, pgn, 4, NULL, NULL, 0) == 0) {
                rc = At45dbWaitReady(dd, AT45_WRITE_POLLS, 1);
            }
        }
        free(rp);
    }
    return rc;
}

u_long At45dbParamPage(void)
{
#ifdef AT45_CONF_PAGE
    return AT45_CONF_PAGE;
#else
    return dcbtab[dd_param].dcb_devt->devt_pages - 1;
#endif
}

int At45dbParamSize(void)
{
    int rc;

    if (dd_param == -1 && (dd_param = At45dbInit(AT45_CONF_DFSPI, AT45_CONF_DFPCS)) == -1) {
        return -1;
    }
#ifdef AT45_CONF_SIZE
    rc = AT45_CONF_SIZE;
#else
    rc = dcbtab[dd_param].dcb_devt->devt_pagsiz;
#endif
    return rc;
}

/*!
 * \brief Load configuration parameters from flash memory.
 *
 * \param pos  Start location within configuration sector.
 * \param data Points to a buffer that receives the contents.
 * \param len  Number of bytes to read.
 *
 * \return Always 0.
 */
int At45dbParamRead(u_int pos, void *data, u_int len)
{
    int rc = -1;
    u_char *buff;
    int csize = At45dbParamSize();
    u_long cpage = At45dbParamPage();

    /* Load the complete configuration area. */
    if (csize > len && (buff = malloc(csize)) != NULL) {
        rc = At45dbPageRead(dd_param, cpage, buff, csize);
        /* Copy requested contents to caller's buffer. */
        memcpy(data, buff + pos, len);
        free(buff);
    }
    return rc;
}

/*!
 * \brief Store configuration parameters in flash memory.
 *
 * \param pos   Start location within configuration sector.
 * \param data  Points to a buffer that contains the bytes to store.
 * \param len   Number of bytes to store.
 *
 * \return 0 on success or -1 in case of an error.
 */
int At45dbParamWrite(u_int pos, CONST void *data, u_int len)
{
    int rc = -1;
    u_char *buff;
    int csize = At45dbParamSize();
    u_long cpage = At45dbParamPage();

    /* Load the complete configuration area. */
    if (csize > len && (buff = malloc(csize)) != NULL) {
        rc = At45dbPageRead(dd_param, cpage, buff, csize);
        /* Compare old with new contents. */
        if (memcmp(buff + pos, data, len)) {
            /* New contents differs. Copy it into the sector buffer. */
            memcpy(buff + pos, data, len);
            /* Erase sector and write new data. */
            rc = At45dbPageWrite(dd_param, cpage, buff, csize);
        }
        free(buff);
    }
    return rc;
}
