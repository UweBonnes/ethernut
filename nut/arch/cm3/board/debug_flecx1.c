/*
 * Copyright (C) 2011 by egnite GmbH
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

#include <cfg/uart.h>
#include <dev/debug.h>
#include <sys/timer.h>

#define PS0_P0_2_TXD0           (1 << 4)
#define PS0_P0_3_RXD0           (1 << 6)

#include "lpc_uart.h"
#include "lpc_sc.h"
#include "lpc_pincon.h"

/*!
 * \addtogroup xgDevDebugLpc1700
 */
/*@{*/

/*!
 * \brief Handle I/O controls for debug device 2.
 *
 * The debug device supports UART_SETSPEED only.
 *
 * \return 0 on success, -1 otherwise.
 */
static int DevDebugIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    return -1;
}

/*!
 * \brief Send a single character to debug device 0.
 *
 * A newline character will be automatically prepended
 * by a carriage return.
 */
static void DebugPut(CONST NUTDEVICE * dev, char ch)
{
    if (ch == '\n') {
        DebugPut(dev, '\r');
    }
    while ((inr(UART0_LSR) & UART_THRE) == 0);	
    outr(UART0_THR, ch);
}

/*!
 * \brief Send characters to debug device 0.
 *
 * A newline character will be automatically prepended
 * by a carriage return.
 *
 * \return Number of characters sent.
 */
static int DevDebugWrite(NUTFILE * fp, CONST void *buffer, int len)
{
    int c = len;
    CONST char *cp = buffer;

    while (c--) {
        DebugPut(fp->nf_dev, *cp++);
    }
    return len;
}

/*!
 * \brief Open debug device.
 *
 * \return Pointer to a static NUTFILE structure.
 */
static NUTFILE *DevDebugOpen(NUTDEVICE * dev, CONST char *name, int mode, int acc)
{
    NUTFILE *fp = (NUTFILE *) (dev->dev_dcb);

    fp->nf_next = 0;
    fp->nf_dev = dev;
    fp->nf_fcb = 0;

    return fp;
}

/*!
 * \brief Close debug device.
 *
 * \return Always 0.
 */
static int DevDebugClose(NUTFILE * fp)
{
    return 0;
}

static int DevDebugInit(NUTDEVICE * dev)
{
    /* Set UART PCLK divider to 2 and determine the clock rate. */
    outr(SC_PCLKSEL0, (inr(SC_PCLKSEL0) & ~SC_PCLK_UART0) | (SC_PCLK_DIV4 << SC_PCLK_UART0_LSB));

    outr(UART0_LCR, inr(UART0_LCR) | UART_DLAB);
    outr(UART0_DLM, 0);
    outr(UART0_DLL, 7);
    outr(UART0_LCR, (inr(UART0_LCR) & ~UART_DLAB) | UART_WLEN_8);

    outr(UART0_FCR, UART_FIFO_TXRST | UART_FIFO_RXRST | UART_FIFO_EN);

    outr(UART0_FDR, 237);
    /* Enable peripheral pins. */
    outr(PINSEL(0), inr(PINSEL(0)) | PS0_P0_2_TXD0 | PS0_P0_3_RXD0);

    return 0;
}

static NUTFILE dbgfile;

NUTDEVICE devDebug0 = {
    0,                          /*!< Pointer to next device, dev_next. */
    {'u', 'a', 'r', 't', '0', 0, 0, 0, 0}
    ,                           /*!< Unique device name, dev_name. */
    0,                          /*!< Type of device, dev_type. */
    LPC_UART0_BASE,             /*!< Base address, dev_base. */
    0,                          /*!< First interrupt number, dev_irq. */
    0,                          /*!< Interface control block, dev_icb. */
    &dbgfile,                   /*!< Driver control block, dev_dcb. */
    DevDebugInit,               /*!< Driver initialization routine, dev_init. */
    DevDebugIOCtl,              /*!< Driver specific control function, dev_ioctl. */
    0,                          /*!< dev_read. */
    DevDebugWrite,              /*!< dev_write. */
    DevDebugOpen,               /*!< dev_open. */
    DevDebugClose,              /*!< dev_close. */
    0                           /*!< dev_size. */
};

/*@}*/
