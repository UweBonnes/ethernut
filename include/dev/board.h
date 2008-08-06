#ifndef _DEV_BOARD_H_
#define _DEV_BOARD_H_

/*
 * Copyright (C) 2001-2007 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.12  2008/08/06 12:51:12  haraldkipp
 * Added support for Ethernut 5 (AT91SAM9XE reference design).
 *
 * Revision 1.11  2008/02/15 17:09:44  haraldkipp
 * Added support for the Elektor Internet Radio.
 *
 * Revision 1.10  2007/10/08 06:37:51  hwmaier
 * Added RTC DS1307 for XNUT-100 and XNUT-105 units
 *
 * Revision 1.9  2007/10/04 20:32:24  olereinhardt
 * Support for SAM7S256 added
 *
 * Revision 1.8  2007/06/03 08:51:30  haraldkipp
 * RTC_CHIP definition now visible for ETHERNUT3 and MMNET02 only.
 *
 * Revision 1.7  2006/10/05 17:18:49  haraldkipp
 * Hardware independant RTC layer added.
 *
 * Revision 1.6  2006/08/31 19:01:08  haraldkipp
 * Using devDebug2 for the DBGU output was a bad idea. Some AT91 chips
 * provide more than two UARTs. We now use devDebug to specify the DBGU
 * device. Baudrate calculations failed on CPUs running on a processor
 * clock, which differs from a futher divided main clock. This had been
 * fixed.
 *
 * Revision 1.5  2006/07/05 07:45:29  haraldkipp
 * Split on-chip interface definitions.
 *
 * Revision 1.4  2006/06/28 17:17:50  haraldkipp
 * Added initial support for Atmel's AT91SAM7X-EK.
 *
 * Revision 1.3  2006/02/23 15:34:00  haraldkipp
 * Support for Philips LPC2xxx Family and LPC-E2294 Board from Olimex added.
 * Many thanks to Michael Fischer for this port.
 *
 * Revision 1.2  2005/11/22 09:19:03  haraldkipp
 * Include condition corrected.
 *
 * Revision 1.1  2005/11/20 14:43:38  haraldkipp
 * First check-in
 *
 */

/*
 * Debug device.
 */
#include <dev/debug.h>

#if defined(GBA)
#define DEV_DEBUG_NAME  "con"
#endif

#if defined(OLIMEX_LPCE2294)
#define DEV_DEBUG       devDebug1
#define DEV_DEBUG_NAME  "uart1"
#endif

#if defined(DBGU_BASE)
#define DEV_DEBUG       devDebug
#define DEV_DEBUG_NAME  "dbgu"
#endif

#ifndef DEV_DEBUG
#define DEV_DEBUG       devDebug0
#endif
#ifndef DEV_DEBUG_NAME
#define DEV_DEBUG_NAME  "uart0"
#endif

/*
 * UART device.
 */
#if defined(__AVR__) || defined(__linux__) || defined(__APPLE__) || defined(__CYGWIN__)

#include <dev/usartavr.h>
#define DEV_UART0       devUsartAvr0
#define DEV_UART1       devUsartAvr1
#define DEV_UART1_NAME  "uart1"

#elif defined(ETHERNUT3) || defined(WOLF) || defined(AT91SAM7X_EK) || defined(AT91SAM9260_EK) || defined(AT91SAM7S) || defined(AT91SAM7SE) || defined(ELEKTOR_IR1) || defined(MCU_AT91SAM9XE512)

#include <dev/usartat91.h>
#define DEV_UART0       devUsartAt910
#define DEV_UART1       devUsartAt911
#define DEV_UART1_NAME  "uart1"

#endif

#ifndef DEV_UART0
#define DEV_UART0       devUart0
#endif
#ifndef DEV_UART0_NAME
#define DEV_UART0_NAME  "uart0"
#endif

#ifndef DEV_UART
#define DEV_UART        DEV_UART0
#endif
#ifndef DEV_UART_NAME
#define DEV_UART_NAME   DEV_UART0_NAME
#endif

/*
 * Ethernet device.
 */
#if defined(ETHERNUT1) || defined(CHARON2) || defined(XNUT_100) || defined(XNUT_105)
#include <dev/nicrtl.h>
#elif defined(ETHERNUT2)
#include <dev/lanc111.h>
#elif defined(ETHERNUT3) || defined(ELEKTOR_IR1)
#include <dev/dm9000e.h>
#elif defined(WOLF)
#include <dev/ax88796.h>
#elif defined(OLIMEX_LPCE2294)
#include <dev/cs8900a.h>
#elif defined(AT91SAM7X_EK) || defined(AT91SAM9260_EK) || defined(ETHERNUT5)
#include <dev/at91sam7x_emac.h>
#endif

#ifndef DEV_ETHER
#define DEV_ETHER       devEth0
#endif
#ifndef DEV_ETHER_NAME
#define DEV_ETHER_NAME  "eth0"
#endif

/*
 * RTC chip.
 */
#if defined(ETHERNUT3)
#define RTC_CHIP rtcX12x6
#include <dev/x12rtc.h>
#elif defined(MMNET02) || defined(XNUT_100) || defined(XNUT_105)
#define RTC_CHIP rtcDs1307
#include <dev/ds1307rtc.h>
#elif defined(ELEKTOR_IR1)
#define RTC_CHIP rtcPcf8563
#include <dev/pcf8563.h>
#endif

#endif

