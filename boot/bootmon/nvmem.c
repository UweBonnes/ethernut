/*
 * Copyright (C) 2005-2007 by egnite Software GmbH. All rights reserved.
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
 */

/*
 * $Log$
 * Revision 1.1  2007/08/17 13:16:32  haraldkipp
 * Checked in.
 *
 */

#include "utils.h"
#include "twbbi.h"
#include "nvmem.h"

#ifndef EEPROM_PAGE_SIZE
#define EEPROM_PAGE_SIZE    64
#endif

#ifndef I2C_SLA_RTC
#define I2C_SLA_RTC     0x6F
#endif

#ifndef I2C_SLA_EEPROM
#define I2C_SLA_EEPROM  0x57
#endif

static int NvMemWriteEnable(int on)
{
    int rc;
    unsigned char buf[3];

    buf[0] = 0;
    buf[1] = 0x3F;
    if (on) {
        buf[2] = 0x02;
        if ((rc = TwMasterTransact(I2C_SLA_RTC, buf, 3, 0, 0, 0)) == 0) {
            buf[2] = 0x06;
            rc = TwMasterTransact(I2C_SLA_RTC, buf, 3, 0, 0, 0);
        }
    } else {
        buf[2] = 0x00;
        rc = TwMasterTransact(I2C_SLA_RTC, buf, 3, 0, 0, 0);
    }
    return rc;
}

static int NvMemWaitReady(void)
{
    unsigned char poll;
    int cnt = 20;

    /* Poll for write cycle finished. */
    while (--cnt && TwMasterTransact(I2C_SLA_EEPROM, 0, 0, &poll, 1, 0) == -1) {
        Delay(1);
    }
    return cnt ? 0 : -1;
}

/*!
 * \brief Read contents from non-volatile EEPROM.
 *
 * \param addr  Start location.
 * \param buff  Points to a buffer that receives the contents.
 * \param len   Number of bytes to read.
 *
 * \return 0 on success or -1 in case of an error.
 */
int NvMemRead(unsigned int addr, void *buff, unsigned int len)
{
    int rc = -1;
    unsigned char wbuf[2];

    wbuf[0] = (unsigned char) (addr >> 8);
    wbuf[1] = (unsigned char) addr;
    if (TwMasterTransact(I2C_SLA_EEPROM, wbuf, 2, buff, len, 0) == len) {
        rc = 0;
    }
    return rc;
}

/*!
 * \brief Store buffer contents in non-volatile EEPROM.
 *
 * The EEPROM of the X122x has a capacity of 512 bytes, while the X1286 is
 * able to store 32 kBytes.
 *
 * \param addr  Storage start location.
 * \param buff  Points to a buffer that contains the bytes to store.
 * \param len   Number of valid bytes in the buffer.
 *
 * \return 0 on success or -1 in case of an error.
 */
unsigned char wbuf[128];

int NvMemWrite(unsigned int addr, void *buff, unsigned int len)
{
    int rc = 0;
    unsigned int wlen;
    unsigned char *wp = buff;

    /*
     * Loop for each page to be written to.
     */
    while (len) {
        /* Do not cross page boundaries. */
        wlen = EEPROM_PAGE_SIZE - (addr & (EEPROM_PAGE_SIZE - 1));
        if (wlen > len) {
            wlen = len;
        }

        /* Set a TWI write buffer. */
        wbuf[0] = (unsigned char) (addr >> 8);
        wbuf[1] = (unsigned char) addr;
        memcpy_(wbuf + 2, wp, wlen);

        /* Enable EEPROM write access and send the write buffer. */
        if ((rc = NvMemWriteEnable(1)) == 0) {
            rc = TwMasterTransact(I2C_SLA_EEPROM, wbuf, wlen + 2, 0, 0, 0);
        }

        /* Check the result. */
        if (rc) {
            break;
        }
        len -= wlen;
        addr += wlen;
        wp += wlen;

        /* Poll for write cycle finished. */
        if ((rc = NvMemWaitReady()) != 0) {
            break;
        }
    }
    NvMemWriteEnable(0);

    return rc;
}

/*!
 * \brief Initialize the interface to serial EEPROM.
 */
void NvMemInit(void)
{
    TwInit();
}
