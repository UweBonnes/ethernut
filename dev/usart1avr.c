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
 * Revision 1.3  2004/09/22 08:14:48  haraldkipp
 * Made configurable
 *
 * Revision 1.2  2004/05/24 20:17:15  drsung
 * Added function UsartSize to return number of chars in input buffer.
 *
 * Revision 1.1  2003/12/15 19:25:33  haraldkipp
 * New USART driver added
 *
 */

#include <cfg/arch/avr.h>
#include <cfg/arch/avrpio.h>

/*!
 * \name UART1 RTS Handshake Control
 *
 * \ref UART1_RTS_BIT must be defined in arch/avr.h
 */
#ifdef UART1_RTS_BIT

#if (PIO_NAME(UART1_RTS_PORT) == PIO_PORTB)
#define UART_RTS_DDR   DDRB

#elif (PIO_NAME(UART1_RTS_PORT) == PIO_PORTD)
#define UART_RTS_DDR   DDRD

#elif (PIO_NAME(UART1_RTS_PORT) == PIO_PORTE)
#define UART_RTS_DDR   DDRE

#elif (PIO_NAME(UART1_RTS_PORT) == PIO_PORTF)
#define UART_RTS_DDR   DDRF

#endif
#define UART_RTS_PORT   UART1_RTS_PORT
#define UART_RTS_BIT    UART1_RTS_BIT

#endif /* UART1_RTS_BIT */

/*!
 * \name UART1 Half Duplex Control
 *
 * \ref UART1_HDX_BIT must be defined in arch/avr.h
 */
#ifdef UART1_HDX_BIT

#if (PIO_NAME(UART1_HDX_PORT) == PIO_PORTB)
#define UART_HDX_DDR   DDRB

#elif (PIO_NAME(UART1_HDX_PORT) == PIO_PORTD)
#define UART_HDX_DDR   DDRD

#elif (PIO_NAME(UART1_HDX_PORT) == PIO_PORTE)
#define UART_HDX_DDR   DDRE

#elif (PIO_NAME(UART1_HDX_PORT) == PIO_PORTF)
#define UART_HDX_DDR   DDRF

#endif
#define UART_HDX_PORT   UART1_HDX_PORT
#define UART_HDX_BIT    UART1_HDX_BIT

#endif /* UART1_HDX_BIT */


#include <string.h>

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <cfg/modem.h>
#include <dev/irqreg.h>
#include <dev/usartavr.h>

#ifdef __AVR_ATmega128__

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
 * \name AVR USART1 Device
 */
/*@{*/
/*!
 * \brief USART1 device information structure.
 *
 * An application must pass a pointer to this structure to 
 * NutRegisterDevice() before using the serial communication
 * driver of the AVR's on-chip USART1.
 *
 * The device is named \b uart1.
 *
 * \showinitializer
 */
NUTDEVICE devUsartAvr1 = {
    0,                          /* Pointer to next device, dev_next. */
    {'u', 'a', 'r', 't', '1', 0, 0, 0, 0},    /* Unique device name, dev_name. */
    IFTYP_CHAR,                 /* Type of device, dev_type. */
    0,                          /* Base address, dev_base (not used). */
    0,                          /* First interrupt number, dev_irq (not used). */
    0,                          /* Interface control block, dev_icb (not used). */
    &dcb_usart1,                /* Driver control block, dev_dcb. */
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
 * \name UART1 CTS Handshake Sense
 *
 * \ref UART1_CTS_IRQ must be defined in arch/avr.h
 */
#if (UART1_CTS_IRQ == INT0)
#define UART_CTS_SIGNAL sig_INTERRUPT0
#define UART_CTS_BIT    0
#define UART_CTS_PORT   PORTD
#define UART_CTS_PIN    PIND
#define UART_CTS_DDR    DDRD

#elif (UART1_CTS_IRQ == INT1)
#define UART_CTS_SIGNAL sig_INTERRUPT1
#define UART_CTS_BIT    1
#define UART_CTS_PORT   PORTD
#define UART_CTS_PIN    PIND
#define UART_CTS_DDR    DDRD

#elif (UART1_CTS_IRQ == INT2)
#define UART_CTS_SIGNAL sig_INTERRUPT2
#define UART_CTS_BIT    2
#define UART_CTS_PORT   PORTD
#define UART_CTS_PIN    PIND
#define UART_CTS_DDR    DDRD

#elif (UART1_CTS_IRQ == INT3)
#define UART_CTS_SIGNAL sig_INTERRUPT3
#define UART_CTS_BIT    3
#define UART_CTS_PORT   PORTD
#define UART_CTS_PIN    PIND
#define UART_CTS_DDR    DDRD

#elif (UART1_CTS_IRQ == INT4)
#define UART_CTS_SIGNAL sig_INTERRUPT4
#define UART_CTS_BIT    4
#define UART_CTS_PORT   PORTE
#define UART_CTS_PIN    PINE
#define UART_CTS_DDR    DDRE

#elif (UART1_CTS_IRQ == INT5)
#define UART_CTS_SIGNAL sig_INTERRUPT5
#define UART_CTS_BIT    5
#define UART_CTS_PORT   PORTE
#define UART_CTS_PIN    PINE
#define UART_CTS_DDR    DDRE

#elif (UART1_CTS_IRQ == INT6)
#define UART_CTS_SIGNAL sig_INTERRUPT6
#define UART_CTS_BIT    6
#define UART_CTS_PORT   PORTE
#define UART_CTS_PIN    PINE
#define UART_CTS_DDR    DDRE

#elif (UART1_CTS_IRQ == INT7)
#define UART_CTS_SIGNAL sig_INTERRUPT7
#define UART_CTS_BIT    7
#define UART_CTS_PORT   PORTE
#define UART_CTS_PIN    PINE
#define UART_CTS_DDR    DDRE

#endif


/*@}*/
/*@}*/

#define UDRn    UDR1
#define UCSRnA  UCSR1A
#define UCSRnB  UCSR1B
#define UCSRnC  UCSR1C
#define UBRRnL  UBRR1L
#define UBRRnH  UBRR1H

#ifdef __IMAGECRAFT__
#define TXB8    TXB81
#define UMSEL   UMSEL1
#define U2X     U2X1
#define UCSZ2   UCSZ12
#define UCSZ1   UCSZ11
#define UCSZ0   UCSZ10
#define UPM0    UPM10
#define UPM1    UPM11
#define USBS    USBS1
#define UPE     UPE1
#define MPCM    MPCM1
#define UCPOL   UCPOL1
#endif

#define sig_UART_RECV   sig_UART1_RECV
#define sig_UART_DATA   sig_UART1_DATA
#define sig_UART_TRANS  sig_UART1_TRANS

#define dcb_usart   dcb_usart1

#include "usartavr.c"

#endif
