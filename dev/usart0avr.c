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
 * The 9-bit communication had been contributed by Brett Abbott, 
 * Digital Telemetry Limited.
 *
 * Dave Smart contributed the synchronous mode support.
 */

/*
 * $Log$
 * Revision 1.2  2004/05/24 20:17:15  drsung
 * Added function UsartSize to return number of chars in input buffer.
 *
 * Revision 1.1  2003/12/15 19:25:33  haraldkipp
 * New USART driver added
 *
 */

#include <string.h>

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <cfg/modem.h>
#include <dev/irqreg.h>
#include <dev/usartavr.h>

/*
 * Local function prototypes.
 */
static u_long AvrUsartGetSpeed(void);
static int AvrUsartSetSpeed(u_long rate);
static u_char AvrUsartGetDataBits(void);
static int AvrUsartSetDataBits(u_char bits);
static u_char AvrUsartGetParity(void);
static int AvrUsartSetParity(u_char mode);
static u_char AvrUsartGetStopBits(void);
static int AvrUsartSetStopBits(u_char bits);
static u_long AvrUsartGetFlowControl(void);
static int AvrUsartSetFlowControl(u_long flags);
static u_long AvrUsartGetStatus(void);
static int AvrUsartSetStatus(u_long flags);
static u_char AvrUsartGetClockMode(void);
static int AvrUsartSetClockMode(u_char mode);
static void AvrUsartTxStart(void);
static void AvrUsartRxStart(void);
static int AvrUsartInit(void);
static int AvrUsartDeinit(void);

/*!
 * \addtogroup xgUsartAvr
 */
/*@{*/

/*!
 * \brief USART0 device control block structure.
 */
static USARTDCB dcb_usart0 = {
    0,                          /* dcb_modeflags */
    0,                          /* dcb_statusflags */
    0,                          /* dcb_rtimeout */
    0,                          /* dcb_wtimeout */
    {0, 0, 0, 0, 0, 0, 0, 0},   /* dcb_tx_rbf */
    {0, 0, 0, 0, 0, 0, 0, 0},   /* dcb_rx_rbf */
    0,                          /* dbc_last_eol */
    AvrUsartInit,               /* dcb_init */
    AvrUsartDeinit,             /* dcb_deinit */
    AvrUsartTxStart,            /* dcb_tx_start */
    AvrUsartRxStart,            /* dcb_rx_start */
    AvrUsartSetFlowControl,     /* dcb_set_flow_control */
    AvrUsartGetFlowControl,     /* dcb_get_flow_control */
    AvrUsartSetSpeed,           /* dcb_set_speed */
    AvrUsartGetSpeed,           /* dcb_get_speed */
    AvrUsartSetDataBits,        /* dcb_set_data_bits */
    AvrUsartGetDataBits,        /* dcb_get_data_bits */
    AvrUsartSetParity,          /* dcb_set_parity */
    AvrUsartGetParity,          /* dcb_get_parity */
    AvrUsartSetStopBits,        /* dcb_set_stop_bits */
    AvrUsartGetStopBits,        /* dcb_get_stop_bits */
    AvrUsartSetStatus,          /* dcb_set_status */
    AvrUsartGetStatus,          /* dcb_get_status */
    AvrUsartSetClockMode,       /* dcb_set_clock_mode */
    AvrUsartGetClockMode,       /* dcb_get_clock_mode */
};

/*!
 * \name AVR USART0 Device
 */
/*@{*/
/*!
 * \brief USART0 device information structure.
 *
 * An application must pass a pointer to this structure to 
 * NutRegisterDevice() before using the serial communication
 * driver of the AVR's on-chip USART0.
 *
 * The device is named \b uart0.
 *
 * \showinitializer
 */
NUTDEVICE devUsartAvr0 = {
    0,                          /* Pointer to next device, dev_next. */
    {'u', 'a', 'r', 't', '0', 0, 0, 0, 0},    /* Unique device name, dev_name. */
    IFTYP_CHAR,                 /* Type of device, dev_type. */
    0,                          /* Base address, dev_base (not used). */
    0,                          /* First interrupt number, dev_irq (not used). */
    0,                          /* Interface control block, dev_icb (not used). */
    &dcb_usart0,                /* Driver control block, dev_dcb. */
    UsartInit,                  /* Driver initialization routine, dev_init. */
    UsartIOCtl,                 /* Driver specific control function, dev_ioctl. */
    UsartRead,                  /* Read from device, dev_read. */
    UsartWrite,                 /* Write to device, dev_write. */
    UsartWrite_P,               /* Write data from program space to device, dev_write_P. */
    UsartOpen,                  /* Open a device or file, dev_open. */
    UsartClose,                 /* Close a device or file, dev_close. */
    UsartSize                   /* Request file size, dev_size. */
};
/*@}*/

/*!
 * \name UART0 RTS Handshake Control
 *
 * \ref UART0_RTS_BIT must be defined in modem.h
 */
/*@{*/

#ifdef UART0_RTS_BIT

/*! Port output register of \ref UART_RTS_BIT. */
#define UART_RTS_PORT   UART0_RTS_PORT
/*! Data direction register of \ref UART_RTS_BIT. */
#define UART_RTS_DDR    UART0_RTS_DDR
/*! RTS handshake control bit. */
#define UART_RTS_BIT    UART0_RTS_BIT

#endif

/*@}*/


/*!
 * \name UART0 CTS Handshake Sense
 *
 * \ref UART0_CTS_BIT must be defined in modem.h
 */
/*@{*/

#ifdef UART0_CTS_BIT

/*! Port output register of \ref UART_CTS_BIT. */
#define UART_CTS_PORT   UART0_CTS_PORT
/*! Port input register of \ref UART_CTS_BIT. */
#define UART_CTS_PIN    UART0_CTS_PIN
/*! Data direction register of \ref UART_CTS_BIT. */
#define UART_CTS_DDR    UART0_CTS_DDR
/*! Interrupt signal of \ref UART_CTS_BIT. */
#define UART_CTS_SIGNAL UART0_CTS_SIGNAL
/*! CTS handshake sense bit. */
#define UART_CTS_BIT    UART0_CTS_BIT

#endif

/*@}*/


/*!
 * \name UART0 Half Duplex Control
 *
 * \ref UART0_HDX_BIT must be defined in modem.h
 */
/*@{*/

#ifdef UART0_HDX_BIT

/*! Port output register of \ref UART_HDX_BIT. */
#define UART_HDX_PORT   UART0_HDX_PORT
/*! Data direction register of \ref UART_HDX_BIT. */
#define UART_HDX_DDR    UART0_HDX_DDR
/*! Half duplex control bit. */
#define UART_HDX_BIT    UART0_HDX_BIT

#endif

/*@}*/
/*@}*/


#ifdef __AVR_ATmega128__

#define UDRn    UDR0
#define UCSRnA  UCSR0A
#define UCSRnB  UCSR0B
#define UCSRnC  UCSR0C
#define UBRRnL  UBRR0L
#define UBRRnH  UBRR0H

#ifdef __IMAGECRAFT__
#define TXB8    TXB80
#define UMSEL   UMSEL0
#define U2X     U2X0
#define UCSZ2   UCSZ02
#define UCSZ1   UCSZ01
#define UCSZ0   UCSZ00
#define UPM0    UPM00
#define UPM1    UPM01
#define USBS    USBS0
#define UPE     UPE0
#define MPCM    MPCM0
#define UCPOL   UCPOL0
#endif

#else

#define UDRn    UDR
#define UCSRnA  USR
#define UCSRnB  UCR
#define UBRRnL  UBRR
#define UCSZ2   CHR9

#endif

#define sig_UART_RECV   sig_UART0_RECV
#define sig_UART_DATA   sig_UART0_DATA
#define sig_UART_TRANS  sig_UART0_TRANS

#define dcb_usart   dcb_usart0

#include "usartavr.c"
