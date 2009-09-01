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
 * Revision 1.11  2008/10/23 08:50:43  haraldkipp
 * Prepared AT91 UART hardware handshake.
 *
 * Revision 1.10  2008/10/05 16:38:06  haraldkipp
 * UART driver was broken on SAM7S and SAM7SE.
 *
 * Revision 1.9  2008/08/11 06:59:13  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.8  2008/08/06 12:51:01  haraldkipp
 * Added support for Ethernut 5 (AT91SAM9XE reference design).
 *
 * Revision 1.7  2008/07/31 09:43:24  haraldkipp
 * Initializing peripheral control registers in a more general way.
 * Fixes bug #2032960.
 *
 * Revision 1.6  2008/04/18 13:24:55  haraldkipp
 * Added Szemzo Andras' RS485 patch.
 *
 * Revision 1.5  2008/02/15 16:59:11  haraldkipp
 * Spport for AT91SAM7SE512 added.
 *
 * Revision 1.4  2007/10/04 20:04:11  olereinhardt
 * Support for SAM7S256 added
 *
 * Revision 1.3  2006/07/05 07:55:23  haraldkipp
 * Daidai's support for AT91SAM7X added.
 *
 * Revision 1.2  2006/01/05 16:46:52  haraldkipp
 * Baudrate calculation is now based on NutGetCpuClock().
 *
 * Revision 1.1  2005/11/20 14:40:28  haraldkipp
 * Added interrupt driven UART driver for AT91.
 *
 */

#define NUT_DEPRECATED

#include <cfg/os.h>
#include <cfg/clock.h>
#include <cfg/arch.h>
#include <cfg/uart.h>

#include <string.h>

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <dev/irqreg.h>
#include <dev/usartat91.h>

#ifndef NUT_CPU_FREQ
#ifdef NUT_PLL_CPUCLK
#include <dev/cy2239x.h>
#else /* !NUT_PLL_CPUCLK */
#define NUT_CPU_FREQ    73728000UL
#endif /* !NUT_PLL_CPUCLK */
#endif /* !NUT_CPU_FREQ */

/*
 * Local function prototypes.
 */
static uint32_t At91UsartGetSpeed(void);
static int At91UsartSetSpeed(uint32_t rate);
static uint8_t At91UsartGetDataBits(void);
static int At91UsartSetDataBits(uint8_t bits);
static uint8_t At91UsartGetParity(void);
static int At91UsartSetParity(uint8_t mode);
static uint8_t At91UsartGetStopBits(void);
static int At91UsartSetStopBits(uint8_t bits);
static uint32_t At91UsartGetFlowControl(void);
static int At91UsartSetFlowControl(uint32_t flags);
static uint32_t At91UsartGetStatus(void);
static int At91UsartSetStatus(uint32_t flags);
static uint8_t At91UsartGetClockMode(void);
static int At91UsartSetClockMode(uint8_t mode);
static void At91UsartTxStart(void);
static void At91UsartRxStart(void);
static int At91UsartInit(void);
static int At91UsartDeinit(void);

/*!
 * \addtogroup xgNutArchArmAt91Us
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
 * \name AT91 USART0 Device
 */
/*@{*/
/*!
 * \brief USART0 device information structure.
 *
 * An application must pass a pointer to this structure to
 * NutRegisterDevice() before using the serial communication
 * driver of the AT91's on-chip USART0.
 *
 * The device is named \b uart0.
 *
 * \showinitializer
 */
NUTDEVICE devUsartAt910 = {
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
    UsartOpen,                  /* Open a device or file, dev_open. */
    UsartClose,                 /* Close a device or file, dev_close. */
    UsartSize                   /* Request file size, dev_size. */
};

/*@}*/

/*@}*/

/*
** SAM9260 and SAM9XE pins.
*/
#if defined(MCU_AT91SAM9260) || defined(MCU_AT91SAM9XE)
#if defined(UART0_MODEM_CONTROL)
#define US_PIOB_PINS_A  ( \
    _BV(PB4_TXD0_A) | _BV(PB5_RXD0_A) | _BV(PB27_CTS0_A) | _BV(PB26_RTS0_A) \
    | _BV(PB25_RI0_A) | _BV(PB22_DSR0_A) | _BV(PB23_DCD0_A) | _BV(PB24_DTR0_A) \
)
#elif defined(UART0_HARDWARE_HANDSHAKE)
#define US_PIOB_PINS_A  ( \
    _BV(PB4_TXD0_A) | _BV(PB5_RXD0_A) | _BV(PB27_CTS0_A) | _BV(PB26_RTS0_A) \
)
#else
#define US_PIOB_PINS_A  (_BV(PB4_TXD0_A) | _BV(PB5_RXD0_A))
#endif
#define US_PIOB_PINS    US_PIOB_PINS_A
#endif

/*
** SAM7S and SAM7SE pins.
*/
#if defined(MCU_AT91SAM7S) || defined(MCU_AT91SAM7SE)
#if defined(UART0_HARDWARE_HANDSHAKE)
#define US_PIOA_PINS_A  ( \
    _BV(PA6_TXD0_A) | _BV(PA5_RXD0_A) | _BV(PA8_CTS0_A) | _BV(PA7_RTS0_A) \
)
#else
#define US_PIOA_PINS_A  (_BV(PA5_RXD0_A) | _BV(PA6_TXD0_A))
#endif
#define US_PIOA_PINS    US_PIOA_PINS_A
#endif

/*
** SAM7X pins.
*/
#if defined(MCU_AT91SAM7X)
#if defined(UART0_HARDWARE_HANDSHAKE)
#define US_PIOA_PINS_A  ( \
    _BV(PA1_TXD0_A) | _BV(PA0_RXD0_A) | _BV(PA4_CTS0_A) | _BV(PA3_RTS0_A) \
)
#else
#define US_PIOA_PINS_A  (_BV(PA1_TXD0_A) | _BV(PA0_RXD0_A))
#endif
#define US_PIOA_PINS    US_PIOA_PINS_A
#endif

/*
** X40 pins.
*/
#if defined(MCU_AT91R40008)
#define US_PIO_PINS     (_BV(P15_RXD0) | _BV(P14_TXD0))
#endif

/*
** Historical settings from Szemzo Andras for RS485.
** Not sure if we must keep this.
*/
#ifdef AT91_UART0_RS485
#if defined(MCU_AT91SAM7X)
#undef US_PIOA_PINS_A
#define US_PIOA_PINS_A  (_BV(PA0_RXD0_A) | _BV(PA1_TXD0_A) | _BV(PA3_RTS0_A))
#undef AT91_UART_RS485_MODE
#define AT91_UART_RS485_MODE
#undef US_PIOA_PINS
#define US_PIOA_PINS    US_PIOA_PINS_A
#endif
#endif /* AT91_UART0_RS485 */


#define USARTn_BASE     USART0_BASE
#define US_ID           US0_ID
#define SIG_UART        sig_UART0
#define dcb_usart       dcb_usart0

#include "usartat91.c"
