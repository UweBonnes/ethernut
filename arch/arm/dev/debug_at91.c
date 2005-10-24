/*
 * Copyright (C) 2001-2005 by egnite Software GmbH. All rights reserved.
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
 * \file arch/arm/dev/debug_at91.c
 * \brief AT91 debug output device.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.3  2005/10/24 08:26:58  haraldkipp
 * Use AT91 header file. Allow to use both USARTs.
 *
 * Revision 1.2  2005/08/02 17:46:45  haraldkipp
 * Major API documentation update.
 *
 * Revision 1.1  2005/07/26 18:02:26  haraldkipp
 * Moved from dev.
 *
 * Revision 1.2  2005/04/05 17:49:05  haraldkipp
 * Make it work on Wolf, but breaks AT91EB40A.
 *
 * Revision 1.1  2004/09/08 10:53:13  haraldkipp
 * Our first device for the EB40A
 *
 * \endverbatim
 */

#include <cfg/os.h>
#include <dev/debug.h>
#include <sys/device.h>
#include <sys/file.h>

/*!
 * \addtogroup xgDevDebugAt91
 */
/*@{*/

static NUTFILE dbgfile0;
static NUTFILE dbgfile1;

/*!
 * \brief Handle I/O controls for debug device 0.
 *
 * The debug device doesn't support any.
 *
 * \return Always -1.
 */
static int DebugIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    return -1;
}

/*!
 * \brief Initialize debug device 0.
 *
 * \return Always 0.
 */
static int Debug0Init(NUTDEVICE * dev)
{
    /* Enable UART clock. */
    outr(PS_PCER, _BV(US0_ID));
    /* Disable GPIO on UART tx/rx pins. */
    outr(PIO_PDR, _BV(14) | _BV(15));
    /* Reset UART. */
    outr(US0_CR, US_RSTRX | US_RSTTX | US_RXDIS | US_TXDIS);
    /* Disable all UART interrupts. */
    outr(US0_IDR, 0xFFFFFFFF);
    /* Clear UART counter registers. */
    outr(US0_RCR, 0);
    outr(US0_TCR, 0);
    /* Set UART baud rate generator register. */
    outr(US0_BRGR, AT91_US_BAUD(115200));
    /* Set UART mode to 8 data bits, no parity and 1 stop bit. */
    outr(US0_MR, US_CHMODE_NORMAL | US_CHRL_8 | US_PAR_NO | US_NBSTOP_1);
    /* Enable UART receiver and transmitter. */
    outr(US0_CR, US_RXEN | US_TXEN);

    return 0;
}

/*!
 * \brief Initialize debug device 1.
 *
 * \return Always 0.
 */
static int Debug1Init(NUTDEVICE * dev)
{
    /* Enable UART clock. */
    outr(PS_PCER, _BV(US1_ID));
    /* Disable GPIO on UART tx/rx pins. */
    outr(PIO_PDR, _BV(21) | _BV(22));
    /* Reset UART. */
    outr(US1_CR, US_RSTRX | US_RSTTX | US_RXDIS | US_TXDIS);
    /* Disable all UART interrupts. */
    outr(US1_IDR, 0xFFFFFFFF);
    /* Clear UART counter registers. */
    outr(US1_RCR, 0);
    outr(US1_TCR, 0);
    /* Set UART baud rate generator register. */
    outr(US1_BRGR, AT91_US_BAUD(115200));
    /* Set UART mode to 8 data bits, no parity and 1 stop bit. */
    outr(US1_MR, US_CHMODE_NORMAL | US_CHRL_8 | US_PAR_NO | US_NBSTOP_1);
    /* Enable UART receiver and transmitter. */
    outr(US1_CR, US_RXEN | US_TXEN);

    return 0;
}

/*!
 * \brief Send a single character to debug device 0.
 *
 * A carriage return character will be automatically appended 
 * to any linefeed.
 */
static void DebugPut(CONST NUTDEVICE * dev, char ch)
{
    while ((inr(dev->dev_base + US_CSR_OFF) & US_TXRDY) == 0);
    outr(dev->dev_base + US_THR_OFF, ch);
    if (ch == '\n') {
        DebugPut(dev, '\r');
    }
}

/*!
 * \brief Send characters to debug device 0.
 *
 * A carriage return character will be automatically appended 
 * to any linefeed.
 *
 * \return Number of characters sent.
 */
static int DebugWrite(NUTFILE * fp, CONST void *buffer, int len)
{
    int c = len;
    CONST char *cp = buffer;

    while (c--) {
        DebugPut(fp->nf_dev, *cp++);
    }
    return len;
}

/*!
 * \brief Open debug device 0.
 *
 * \return Pointer to a static NUTFILE structure.
 */
static NUTFILE *DebugOpen(NUTDEVICE * dev, CONST char *name, int mode, int acc)
{
    NUTFILE *fp = (NUTFILE *) (dev->dev_dcb);

    fp->nf_next = 0;
    fp->nf_dev = dev;
    fp->nf_fcb = 0;

    return fp;
}

/*! 
 * \brief Close debug device 0.
 *
 * \return Always 0.
 */
static int DebugClose(NUTFILE * fp)
{
    return 0;
}

/*!
 * \brief Debug device 0 information structure.
 */
NUTDEVICE devDebug0 = {
    0,                          /*!< Pointer to next device, dev_next. */
    {'u', 'a', 'r', 't', '0', 0, 0, 0, 0}
    ,                           /*!< Unique device name, dev_name. */
    0,                          /*!< Type of device, dev_type. */
    USART0_BASE,                /*!< Base address, dev_base. */
    0,                          /*!< First interrupt number, dev_irq. */
    0,                          /*!< Interface control block, dev_icb. */
    &dbgfile0,                  /*!< Driver control block, dev_dcb. */
    Debug0Init,                 /*!< Driver initialization routine, dev_init. */
    DebugIOCtl,                 /*!< Driver specific control function, dev_ioctl. */
    0,                          /*!< dev_read. */
    DebugWrite,                 /*!< dev_write. */
    DebugOpen,                  /*!< dev_opem. */
    DebugClose,                 /*!< dev_close. */
    0                           /*!< dev_size. */
};

/*!
 * \brief Debug device 1 information structure.
 */
NUTDEVICE devDebug1 = {
    0,                          /*!< Pointer to next device, dev_next. */
    {'u', 'a', 'r', 't', '1', 0, 0, 0, 0}
    ,                           /*!< Unique device name, dev_name. */
    0,                          /*!< Type of device, dev_type. */
    USART1_BASE,                /*!< Base address, dev_base. */
    0,                          /*!< First interrupt number, dev_irq. */
    0,                          /*!< Interface control block, dev_icb. */
    &dbgfile1,                  /*!< Driver control block, dev_dcb. */
    Debug1Init,                 /*!< Driver initialization routine, dev_init. */
    DebugIOCtl,                 /*!< Driver specific control function, dev_ioctl. */
    0,                          /*!< dev_read. */
    DebugWrite,                 /*!< dev_write. */
    DebugOpen,                  /*!< dev_opem. */
    DebugClose,                 /*!< dev_close. */
    0                           /*!< dev_size. */
};

/*@}*/
