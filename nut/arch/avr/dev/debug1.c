/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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

/*
 * $Log$
 * Revision 1.5  2008/08/11 06:59:14  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.4  2006/10/08 16:48:07  haraldkipp
 * Documentation fixed
 *
 * Revision 1.3  2005/10/17 08:46:53  hwmaier
 * Setting baudrate function changed: For CPUs w/ 12 and 16 MHz xtal double rate mode is now used (only if set by NUT_CPU_FREQ)
 *
 * Revision 1.2  2005/08/02 17:46:45  haraldkipp
 * Major API documentation update.
 *
 * Revision 1.1  2005/07/26 18:02:27  haraldkipp
 * Moved from dev.
 *
 * Revision 1.3  2005/02/06 16:36:59  haraldkipp
 * Fixes ICCAVR V7 baudrate miscalculation.
 *
 * Revision 1.2  2004/02/25 16:19:10  haraldkipp
 * Support baudrate settings
 *
 * Revision 1.1.1.1  2003/05/09 14:40:37  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.2  2003/05/06 18:29:49  harald
 * ICCAVR port
 *
 * Revision 1.1  2003/04/07 12:15:27  harald
 * First release
 *
 */

#include <dev/debug.h>

#include <cfg/os.h>
#include <cfg/uart.h>
#include <sys/timer.h>
#include <sys/device.h>
#include <sys/file.h>

/*!
 * \addtogroup xgDevDebugAvr
 */
/*@{*/

#ifdef __AVR_ENHANCED__

static NUTFILE dbgfile;

static int DebugIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    if(req == UART_SETSPEED) {
#if defined(__AVR_ENHANCED__) && ((NUT_CPU_FREQ == 12000000) || (NUT_CPU_FREQ == 16000000))
        /* On enhanced MCUs we use double rate mode, so we can use 115200 bps
        * with 12.0 and 16.0 crystals.
        */
        sbi(UCSR1A, U2X1);
        outb(UBRR1L, (uint8_t) ((((2UL * NutGetCpuClock()) / (*((uint32_t *)conf) * 8UL)) + 1UL) / 2UL) - 1UL);
#else
        outb(UBRR1L, (uint8_t) ((((2UL * NutGetCpuClock()) / (*((uint32_t *)conf) * 16UL)) + 1UL) / 2UL) - 1UL);
#endif
        return 0;
    }
    return -1;
}

static int DebugInit(NUTDEVICE * dev)
{
    /* Note: Default baudrate has been set in nutinit.c */
    UCSR1B = BV(RXEN) | BV(TXEN);
    return 0;
}

static void DebugPut(char ch)
{
    if(ch == '\n') {
        while((UCSR1A & BV(UDRE)) == 0);
        UDR1 = '\r';
    }
    while((UCSR1A & BV(UDRE)) == 0);
    UDR1 = ch;
}

static int DebugWrite(NUTFILE * fp, const void *buffer, int len)
{
    int c = len;
    const char *cp = buffer;

    while(c--)
        DebugPut(*cp++);
    return len;
}

static int DebugWrite_P(NUTFILE * fp, PGM_P buffer, int len)
{
    int c = len;
    PGM_P cp = buffer;

    while(c--) {
        DebugPut(PRG_RDB(cp));
        cp++;
    }
    return len;
}

static NUTFILE *DebugOpen(NUTDEVICE * dev, const char *name, int mode, int acc)
{
    dbgfile.nf_next = 0;
    dbgfile.nf_dev = dev;
    dbgfile.nf_fcb = 0;

    return &dbgfile;
}

#ifdef NUT_DEV_DEBUG_READ
/*!
 * \brief Read characters from debug device.
 *
 * This function is called by the low level input routines of the
 * \ref xrCrtLowio "C runtime library", using the _NUTDEVICE::dev_read
 * entry.
 *
 * The function will block the calling thread until at least one
 * character has been received.
 *
 * \param fp     Pointer to a \ref _NUTFILE structure, obtained by a
 *               previous call to At91DevDebugOpen().
 * \param buffer Pointer to the buffer that receives the data. If zero,
 *               then all characters in the input buffer will be
 *               removed.
 * \param size   Maximum number of bytes to read.
 *
 * \return The number of bytes read, which may be less than the number
 *         of bytes specified. A return value of -1 indicates an error,
 *         while zero is returned in case of a timeout.
 */
int DebugRead(NUTFILE * fp, void *buffer, int size)
{
    int rc;
    unsigned int ch;
    char *bp = (char *) buffer;

    /* Wait for the first character, forever. */
    for (rc = 0; rc < size; rc++) {
        while ((inb(UCSR1A) & _BV(RXC)) == 0) {
            NutSleep(1);
            if ((rc || bp == NULL) && (inb(UCSR1A) & _BV(RXC1)) == 0) {
                return rc;
            }
        }
        ch = inb(UDR1);
        if (bp) {
            if (ch == '\r') {
                *bp++ = '\n';
            } else {
                *bp++ = (char) ch;
            }
        }
    }
    return rc;
}

/*!
 * \brief Retrieves the number of characters in input buffer.
 *
 * This function is called by the low level size routine of the C runtime
 * library, using the _NUTDEVICE::dev_size entry.
 *
 * \param fp     Pointer to a \ref _NUTFILE structure, obtained by a
 *               previous call to UsartOpen().
 *
 * \return The number of bytes currently stored in input buffer.
 */
long DebugSize(NUTFILE *fp)
{
    while (inb(UCSR1A) & _BV(RXC1)) {
        return 1;
    }
    return 0;
}

#endif

/*!
 * \brief Close a device or file.
 */
static int DebugClose(NUTFILE * fp)
{
    return 0;
}

/*!
 * \brief UART 1 Device information structure.
 */
NUTDEVICE devDebug1 = {
    0,                          /*!< Pointer to next device. */
    {'u', 'a', 'r', 't', '1', 0, 0, 0, 0},      /*!< Unique device name. */
    0,                          /*!< Type of device. */
    0,                          /*!< Base address. */
    0,                          /*!< First interrupt number. */
    0,                          /*!< Interface control block. */
    0,                          /*!< Driver control block. */
    DebugInit,                  /*!< Driver initialization routine. */
    DebugIOCtl,                 /*!< Driver specific control function. */
#ifdef NUT_DEV_DEBUG_READ
    DebugRead,
#else
    NULL,
#endif
    DebugWrite,
    DebugWrite_P,
    DebugOpen,
    DebugClose,
#ifdef NUT_DEV_DEBUG_READ
    DebugSize
#else
    NULL
#endif
};

#endif

/*@}*/
