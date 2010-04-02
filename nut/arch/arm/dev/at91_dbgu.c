/*
 * Copyright (C) 2001-2006 by egnite Software GmbH
 * Copyright (C) 2010 by egnite GmbH
 *
 * All rights reserved.
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
 */

/*!
 * \file arch/arm/dev/at91_dbgu.c
 * \brief AT91 debug output device using DBGU.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <arch/arm/debug_at91.h>

#if defined(DBGU_BASE)

/*!
 * \addtogroup xgDevDebugAt91
 */
/*@{*/

#ifndef NUT_DEV_DEBUG_SPEED
#define NUT_DEV_DEBUG_SPEED 115200
#endif

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
#if NUT_DEV_DEBUG_SPEED
    /* Set UART baud rate generator register. */
    outr(DBGU_BRGR, At91BaudRateDiv(NUT_DEV_DEBUG_SPEED));
#endif
    /* Set UART mode to 8 data bits, no parity and 1 stop bit. */
    outr(DBGU_MR, US_CHMODE_NORMAL | US_CHRL_8 | US_PAR_NO | US_NBSTOP_1);
    /* Enable UART receiver and transmitter. */
    outr(DBGU_CR, US_RXEN | US_TXEN);

    return 0;
}

static NUTFILE dbgfile;

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
    At91DevDebugIOCtl,          /*!< Driver specific control function, dev_ioctl. */
    0,                          /*!< dev_read. */
    At91DevDebugWrite,          /*!< dev_write. */
    At91DevDebugOpen,           /*!< dev_opem. */
    At91DevDebugClose,          /*!< dev_close. */
    0                           /*!< dev_size. */
};

#endif /* DBGU_BASE */

/*@}*/
