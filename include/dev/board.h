#ifndef _DEV_ETHERNET_H_
#define _DEV_ETHERNET_H_

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
 */

/*
 * $Log$
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

#elif defined(ETHERNUT3) || defined(WOLF)

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
#elif defined(ETHERNUT3)
#include <dev/dm9000e.h>
#elif defined(WOLF)
#include <dev/ax88796.h>
#endif

#ifndef DEV_ETHER
#define DEV_ETHER       devEth0
#endif
#ifndef DEV_ETHER_NAME
#define DEV_ETHER_NAME  "eth0"
#endif

#endif

