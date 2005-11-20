/*
 * Copyright (C) 2005 by egnite Software GmbH. All rights reserved.
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
 */

/*
 * $Log$
 * Revision 1.1  2005/11/20 14:40:28  haraldkipp
 * Added interrupt driven UART driver for AT91.
 *
 */

#include <arch/arm.h>

#include <string.h>

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <dev/irqreg.h>
#include <dev/usartat91.h>

/*
 * Local function prototypes.
 */
static u_long At91UsartGetSpeed(void);
static int At91UsartSetSpeed(u_long rate);
static u_char At91UsartGetDataBits(void);
static int At91UsartSetDataBits(u_char bits);
static u_char At91UsartGetParity(void);
static int At91UsartSetParity(u_char mode);
static u_char At91UsartGetStopBits(void);
static int At91UsartSetStopBits(u_char bits);
static u_long At91UsartGetFlowControl(void);
static int At91UsartSetFlowControl(u_long flags);
static u_long At91UsartGetStatus(void);
static int At91UsartSetStatus(u_long flags);
static u_char At91UsartGetClockMode(void);
static int At91UsartSetClockMode(u_char mode);
static void At91UsartTxStart(void);
static void At91UsartRxStart(void);
static int At91UsartInit(void);
static int At91UsartDeinit(void);

/*!
 * \addtogroup xgUsartAt91
 */
/*@{*/

/*!
 * \brief USART1 device control block structure.
 */
static USARTDCB dcb_usart1 = {
    0,                          /* dcb_modeflags */
    0,                          /* dcb_statusflags */
    0,                          /* dcb_rtimeout */
    0,                          /* dcb_wtimeout */
    {0, 0, 0, 0, 0, 0, 0, 0},   /* dcb_tx_rbf */
    {0, 0, 0, 0, 0, 0, 0, 0},   /* dcb_rx_rbf */
    0,                          /* dbc_last_eol */
    At91UsartInit,              /* dcb_init */
    At91UsartDeinit,            /* dcb_deinit */
    At91UsartTxStart,           /* dcb_tx_start */
    At91UsartRxStart,           /* dcb_rx_start */
    At91UsartSetFlowControl,    /* dcb_set_flow_control */
    At91UsartGetFlowControl,    /* dcb_get_flow_control */
    At91UsartSetSpeed,          /* dcb_set_speed */
    At91UsartGetSpeed,          /* dcb_get_speed */
    At91UsartSetDataBits,       /* dcb_set_data_bits */
    At91UsartGetDataBits,       /* dcb_get_data_bits */
    At91UsartSetParity,         /* dcb_set_parity */
    At91UsartGetParity,         /* dcb_get_parity */
    At91UsartSetStopBits,       /* dcb_set_stop_bits */
    At91UsartGetStopBits,       /* dcb_get_stop_bits */
    At91UsartSetStatus,         /* dcb_set_status */
    At91UsartGetStatus,         /* dcb_get_status */
    At91UsartSetClockMode,      /* dcb_set_clock_mode */
    At91UsartGetClockMode,      /* dcb_get_clock_mode */
};

/*!
 * \name AT91 USART1 Device
 */
/*@{*/
/*!
 * \brief USART1 device information structure.
 *
 * An application must pass a pointer to this structure to
 * NutRegisterDevice() before using the serial communication
 * driver of the AT91's on-chip USART1.
 *
 * The device is named \b uart1.
 *
 * \showinitializer
 */
NUTDEVICE devUsartAt911 = {
    0,                          /* Pointer to next device, dev_next. */
    {'u', 'a', 'r', 't', '1', 0, 0, 0, 0},    /* Unique device name, dev_name. */
    IFTYP_CHAR,                 /* Type of device, dev_type. */
    1,                          /* Base address, dev_base (not used). */
    0,                          /* First interrupt number, dev_irq (not used). */
    0,                          /* Interface control block, dev_icb (not used). */
    &dcb_usart1,                /* Driver control block, dev_dcb. */
    UsartInit,                  /* Driver initialization routine, dev_init. */
    UsartIOCtl,                 /* Driver specific control function, dev_ioctl. */
    UsartRead,                  /* Read from device, dev_read. */
    UsartWrite,                 /* Write to device, dev_write. */
    UsartOpen,                  /* Open a device or file, dev_open. */
    UsartClose,                 /* Close a device or file, dev_close. */
    UsartSize                   /* Request file size, dev_size. */
};

/*@}*/

/*@}*/

#define USARTn_BASE     USART1_BASE
#define US_ID           US1_ID
#define US_GPIO_PINS    0x00600000
#define SIG_UART        sig_UART1
#define dcb_usart       dcb_usart1

#include "usartat91.c"
