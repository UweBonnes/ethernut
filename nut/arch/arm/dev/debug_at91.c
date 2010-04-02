/*
 * Copyright (C) 2001-2006 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.13  2008/08/11 06:59:07  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.12  2008/08/06 12:51:00  haraldkipp
 * Added support for Ethernut 5 (AT91SAM9XE reference design).
 *
 * Revision 1.11  2008/02/15 16:58:41  haraldkipp
 * Spport for AT91SAM7SE512 added.
 *
 * Revision 1.10  2007/10/04 19:55:33  olereinhardt
 * Support for SAM7S256 added
 *
 * Revision 1.9  2006/08/31 19:01:08  haraldkipp
 * Using devDebug2 for the DBGU output was a bad idea. Some AT91 chips
 * provide more than two UARTs. We now use devDebug to specify the DBGU
 * device. Baudrate calculations failed on CPUs running on a processor
 * clock, which differs from a futher divided main clock. This had been
 * fixed.
 *
 * Revision 1.8  2006/08/05 11:54:06  haraldkipp
 * Special register functions should not be based on MCU definitions but on
 * register definitions.
 *
 * Revision 1.7  2006/07/05 07:55:23  haraldkipp
 * Daidai's support for AT91SAM7X added.
 *
 * Revision 1.6  2006/06/28 17:18:40  haraldkipp
 * Temporarly exclude AT91R40008 specific register settings from building
 * for AT91SAM7X.
 *
 * Revision 1.5  2006/03/16 19:03:48  haraldkipp
 * Added ioctl to set baudrate.
 *
 * Revision 1.4  2006/01/05 16:46:01  haraldkipp
 * Baudrate calculation is now based on NutGetCpuClock().
 * The AT91_US_BAUD macro had been marked deprecated.
 *
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
#include <cfg/clock.h>

#include <dev/debug.h>
#include <sys/device.h>
#include <sys/file.h>
#include <sys/timer.h>

/*!
 * \addtogroup xgDevDebugAt91
 */
/*@{*/

#if defined(DBGU_BASE)
static NUTFILE dbgfile;
#else
static NUTFILE dbgfile0;
static NUTFILE dbgfile1;
#endif

#if defined(DBGU_BASE)
/*!
 * \brief Handle I/O controls for debug device 2.
 *
 * The debug device supports UART_SETSPEED only.
 *
 * \return 0 on success, -1 otherwise.
 */
static int DebugIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    if(req == UART_SETSPEED) {
        outr(DBGU_BRGR, (NutArchClockGet(NUT_HWCLK_PERIPHERAL) / (8 * (*((uint32_t *)conf))) + 1) / 2);
        return 0;
    }
    return -1;
}

/*!
 * \brief Initialize debug device 2.
 *
 * \return Always 0.
 */
static int DebugInit(NUTDEVICE * dev)
{
    /* Disable GPIO on UART tx/rx pins. */
#if defined (PA9_DRXD_A) && defined (PA10_DTXD_A)
    outr(PIOA_PDR, _BV(PA9_DRXD_A) | _BV(PA10_DTXD_A));
#elif defined (PA27_DRXD_A) && defined (PA28_DTXD_A)
    outr(PIOA_PDR, _BV(PA27_DRXD_A) | _BV(PA28_DTXD_A));
#endif
    /* Reset UART. */
    outr(DBGU_CR, US_RSTRX | US_RSTTX | US_RXDIS | US_TXDIS);
    /* Disable all UART interrupts. */
    outr(DBGU_IDR, 0xFFFFFFFF);
    /* Set UART baud rate generator register. */
    outr(DBGU_BRGR, (NutArchClockGet(NUT_HWCLK_PERIPHERAL) / (8 * (115200)) + 1) / 2);
    /* Set UART mode to 8 data bits, no parity and 1 stop bit. */
    outr(DBGU_MR, US_CHMODE_NORMAL | US_CHRL_8 | US_PAR_NO | US_NBSTOP_1);
    /* Enable UART receiver and transmitter. */
    outr(DBGU_CR, US_RXEN | US_TXEN);

    return 0;
}

#else /* !DBGU_BASE */

/*!
 * \brief Handle I/O controls for debug device 0.
 *
 * The debug device supports UART_SETSPEED only.
 *
 * \return 0 on success, -1 otherwise.
 */
static int Debug0IOCtl(NUTDEVICE * dev, int req, void *conf)
{
    if(req == UART_SETSPEED) {
        outr(US0_BRGR, (NutArchClockGet(NUT_HWCLK_PERIPHERAL) / (8 * (*((uint32_t *)conf))) + 1) / 2);
        return 0;
    }
    return -1;
}

/*!
 * \brief Handle I/O controls for debug device 1.
 *
 * The debug device supports UART_SETSPEED only.
 *
 * \return 0 on success, -1 otherwise.
 */
static int Debug1IOCtl(NUTDEVICE * dev, int req, void *conf)
{
    if(req == UART_SETSPEED) {
        outr(US1_BRGR, (NutArchClockGet(NUT_HWCLK_PERIPHERAL) / (8 * (*((uint32_t *)conf))) + 1) / 2);
        return 0;
    }
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
#if defined (PS_PCER)
    outr(PS_PCER, _BV(US0_ID));
#elif defined (PMC_PCER)
    outr(PMC_PCER, _BV(US0_ID));
#endif
    /* Disable GPIO on UART tx/rx pins. */
#if defined (P15_RXD0) && defined (P14_TXD0)
    outr(PIO_PDR, _BV(P15_RXD0) | _BV(P14_TXD0));
#elif defined (PA0_RXD0_A) && defined (PA1_TXD0_A)
    outr(PIOA_PDR, _BV(PA0_RXD0_A) | _BV(PA1_TXD0_A));
#elif defined (PA5_RXD0_A) && defined (PA6_TXD0_A)
    outr(PIOA_PDR, _BV(PA5_RXD0_A) | _BV(PA6_TXD0_A));
#endif
    /* Reset UART. */
    outr(US0_CR, US_RSTRX | US_RSTTX | US_RXDIS | US_TXDIS);
    /* Disable all UART interrupts. */
    outr(US0_IDR, 0xFFFFFFFF);
#if defined (US0_RCR) && defined(US0_TCR)
    /* Clear UART counter registers. */
    outr(US0_RCR, 0);
    outr(US0_TCR, 0);
#endif
    /* Set UART baud rate generator register. */
    outr(US0_BRGR, (NutArchClockGet(NUT_HWCLK_PERIPHERAL) / (8 * (115200)) + 1) / 2);
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
#if defined (PS_PCER)
    outr(PS_PCER, _BV(US1_ID));
#elif defined (PMC_PCER)
    outr(PMC_PCER, _BV(US1_ID));
#endif
    /* Disable GPIO on UART tx/rx pins. */
#if defined (P22_RXD1) && defined (P21_TXD1)
    outr(PIO_PDR, _BV(P22_RXD1) | _BV(P21_TXD1));
#elif defined (PA5_RXD1_A) && defined (PA6_TXD1_A)
    outr(PIOA_PDR, _BV(PA5_RXD1_A) | _BV(PA6_TXD1_A));
#elif defined (PA21_RXD1_A) && defined (PA22_TXD1_A)
    outr(PIOA_PDR, _BV(PA21_RXD1_A) | _BV(PA22_TXD1_A));
#endif
    /* Reset UART. */
    outr(US1_CR, US_RSTRX | US_RSTTX | US_RXDIS | US_TXDIS);
    /* Disable all UART interrupts. */
    outr(US1_IDR, 0xFFFFFFFF);
#if defined (US1_RCR) && defined(US1_TCR)
    /* Clear UART counter registers. */
    outr(US1_RCR, 0);
    outr(US1_TCR, 0);
#endif
    /* Set UART baud rate generator register. */
    outr(US1_BRGR, (NutArchClockGet(NUT_HWCLK_PERIPHERAL) / (8 * (115200)) + 1) / 2);
    /* Set UART mode to 8 data bits, no parity and 1 stop bit. */
    outr(US1_MR, US_CHMODE_NORMAL | US_CHRL_8 | US_PAR_NO | US_NBSTOP_1);
    /* Enable UART receiver and transmitter. */
    outr(US1_CR, US_RXEN | US_TXEN);

    return 0;
}

#endif


/*!
 * \brief Send a single character to debug device 0.
 *
 * A newline character will be automatically prepended
 * by a carriage return.
 */
static void DebugPut(CONST NUTDEVICE * dev, char ch)
{
    if (ch == '\n') {
	    while ((inr(dev->dev_base + US_CSR_OFF) & US_TXRDY) == 0);
        outr(dev->dev_base + US_THR_OFF, '\r');
    }
    while ((inr(dev->dev_base + US_CSR_OFF) & US_TXRDY) == 0);
    outr(dev->dev_base + US_THR_OFF, ch);
}

/*!
 * \brief Send characters to debug device 0.
 *
 * A newline character will be automatically prepended
 * by a carriage return.
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

#if defined(DBGU_BASE)

/*!
 * \brief Debug device information structure.
 */
NUTDEVICE devDebug = {
    0,                          /*!< Pointer to next device, dev_next. */
    {'d', 'b', 'g', 'u', 0, 0, 0, 0, 0}
    ,                           /*!< Unique device name, dev_name. */
    0,                          /*!< Type of device, dev_type. */
    DBGU_BASE,                  /*!< Base address, dev_base. */
    0,                          /*!< First interrupt number, dev_irq. */
    0,                          /*!< Interface control block, dev_icb. */
    &dbgfile,                   /*!< Driver control block, dev_dcb. */
    DebugInit,                  /*!< Driver initialization routine, dev_init. */
    DebugIOCtl,                 /*!< Driver specific control function, dev_ioctl. */
    0,                          /*!< dev_read. */
    DebugWrite,                 /*!< dev_write. */
    DebugOpen,                  /*!< dev_opem. */
    DebugClose,                 /*!< dev_close. */
    0                           /*!< dev_size. */
};

#else   /* !DBGU_BASE */

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
    Debug0IOCtl,                /*!< Driver specific control function, dev_ioctl. */
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
    Debug1IOCtl,                /*!< Driver specific control function, dev_ioctl. */
    0,                          /*!< dev_read. */
    DebugWrite,                 /*!< dev_write. */
    DebugOpen,                  /*!< dev_opem. */
    DebugClose,                 /*!< dev_close. */
    0                           /*!< dev_size. */
};

#endif  /* !DBGU_BASE */

/*@}*/
