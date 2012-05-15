/*
 * Copyright (C) 2012 by Ole Reinhardt (ole.reinhardt@embedded-it.de)
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


/*
 * \verbatim
 * $Id: $
 * \endverbatim
 */

#include <cfg/arch.h>
#include <cfg/uart.h>

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <dev/irqreg.h>
#include <dev/gpio.h>
#include <dev/usart.h>

#include <arch/cm3/nxp/lpc177x_8x.h>
#include <arch/cm3/nxp/lpc177x_8x_clk.h>
#include <arch/cm3/nxp/lpc17xx_usart.h>

/*!
 * \addtogroup xgNutArchCm3Lpc176xUsart
 */
/*@{*/

/*
 * USART0 device control block structure.
 */
static USARTDCB dcb_usart0 = {
    0,                          /* dcb_modeflags */
    0,                          /* dcb_statusflags */
    0,                          /* dcb_rtimeout */
    0,                          /* dcb_wtimeout */
    { 0,0,0,0,0,0,0,0,0},       /* dcb_tx_rbf */
    { 0,0,0,0,0,0,0,0,0},       /* dcb_rx_rbf */
    0,                          /* dbc_last_eol */
    Lpc17xxUsartInit,           /* dcb_init */
    Lpc17xxUsartDeinit,         /* dcb_deinit */
    Lpc17xxUsartTxStart,        /* dcb_tx_start */
    Lpc17xxUsartRxStart,        /* dcb_rx_start */
    Lpc17xxUsartSetFlowControl, /* dcb_set_flow_control */
    Lpc17xxUsartGetFlowControl, /* dcb_get_flow_control */
    Lpc17xxUsartSetSpeed,       /* dcb_set_speed */
    Lpc17xxUsartGetSpeed,       /* dcb_get_speed */
    Lpc17xxUsartSetDataBits,    /* dcb_set_data_bits */
    Lpc17xxUsartGetDataBits,    /* dcb_get_data_bits */
    Lpc17xxUsartSetParity,      /* dcb_set_parity */
    Lpc17xxUsartGetParity,      /* dcb_get_parity */
    Lpc17xxUsartSetStopBits,    /* dcb_set_stop_bits */
    Lpc17xxUsartGetStopBits,    /* dcb_get_stop_bits */
    Lpc17xxUsartSetStatus,      /* dcb_set_status */
    Lpc17xxUsartGetStatus,      /* dcb_get_status */
    Lpc17xxUsartSetClockMode,   /* dcb_set_clock_mode */
    Lpc17xxUsartGetClockMode,   /* dcb_get_clock_mode */
};

/*!
 * \name LPC17xx USART0 Device
 */
/*@{*/

/*!
 * \brief USART0 device information structure.
 *
 * An application must pass a pointer to this structure to
 * NutRegisterDevice() before using the serial communication
 * driver of the LPC17xx's on-chip USART0.
 *
 * The device is named usart0.
 *
 * \showinitializer
 */
NUTDEVICE devUsartLpc17xx_0 = {
    0,                          /* Pointer to next device, dev_next. */
    {'u', 's', 'a', 'r', 't', '0', 0, 0, 0},    /* Unique device name, dev_name. */
    IFTYP_CHAR,                 /* Type of device, dev_type. */
    LPC_UART0_BASE,             /* Base address, dev_base. */
    UART0_IRQn,                 /* First interrupt number, dev_irq. */
    NULL,                       /* Interface control block, dev_icb. */
    &dcb_usart0,                /* Driver control block, dev_dcb. */
    UsartInit,                  /* Driver initialization routine, dev_init. */
    UsartIOCtl,                 /* Driver specific control function, dev_ioctl. */
    UsartRead,                  /* Read from device, dev_read. */
    UsartWrite,                 /* Write to device, dev_write. */
    UsartOpen,                  /* Open a device or file, dev_open. */
    UsartClose,                 /* Close a device or file, dev_close. */
    UsartSize                   /* Request file size, dev_size. */
};

/*@}*/

/*!
 * \brief USART0 GPIO configuartion and assignment.
 */

#define TX_GPIO_PORT    NUTGPIO_PORT0
#define TX_GPIO_PIN     2
#define TX_GPIO_PIN_CFG GPIO_CFG_OUTPUT | GPIO_CFG_PERIPHERAL1
#define RX_GPIO_PORT    NUTGPIO_PORT0
#define RX_GPIO_PIN     3
#define RX_GPIO_PIN_CFG GPIO_CFG_INPUT | GPIO_CFG_PERIPHERAL1

/*!
 * \brief USART0 base configuration.
 */

#ifdef USART0_INIT_BAUDRATE
#define USART_INIT_BAUTRATE USART0_INIT_BAUDRATE
#endif


/* USART0 does not support hardware handshake */
#define USART_HWFLOWCTRL USART_HardwareFlowControl_None
#undef  US_MODE_HWHANDSHAKE

#ifdef USART0_XONXOFF_CONTROL
#define USART_XONXOFF_CONTROL
#else
#undef  USART_XONXOFF_CONTROL
#endif

#ifdef USART0_MODE_IRDA
#define USART_MODE_IRDA
#else
#undef  USART_MODE_IRDA
#endif

/* TODO: DMA Support */
/*
#ifdef USART0_SUPPORT_DMA
#define UART_DMA_TXCHANNEL  DMA1_C4
#define UART_DMA_RXCHANNEL  DMA1_C5
#define UART_DMA_TXIRQ      sig_DMA1_CH4
#define UART_DMA_RXIRQ      sig_DMA1_CH5
#else
#undef UART_DMA_TXCHANNEL
#undef UART_DMA_RXCHANNEL
#endif
*/

#define USARTn      LPC_UART0
#define USARTnBase  LPC_UART0_BASE
#define USARTirqn   UART0_IRQn

#define SigUSART    sig_USART0
#define DcbUSART    dcb_usart0

/*@}*/
#include "lpc17xx_usart.c"
