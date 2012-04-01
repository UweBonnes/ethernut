#ifndef _DEV_USART_LPC17xx_H_
#define _DEV_USART_LPC17xx_H_

/*
 * Copyright (C) 2012 by Ole Reinhardt <ole.reinhardt@embedded-it.de>
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
 * $Id:$
 *
 */


#include <sys/device.h>
#include <dev/uart.h>
#include <dev/usart.h>

/*!
 * \file dev/usart_lpc17xx.h
 * \brief Synchronous/asynchronous serial device definitions.
 */

/* define UARTS (not all accesable at the board) */

#ifndef DEV_UART0
#define DEV_UART0       devUsartLpc17xx_0
#endif

#ifndef DEV_UART0_NAME
#define DEV_UART0_NAME  DEV_UART0.dev_name
#endif


#ifndef DEV_UART1
#define DEV_UART1       devUsartLpc17xx_1
#endif

#ifndef DEV_UART1_NAME
#define DEV_UART1_NAME  DEV_UART1.dev_name
#endif


#ifndef DEV_UART2
#define DEV_UART2       devUsartLpc17xx_2
#endif

#ifndef DEV_UART2_NAME
#define DEV_UART2_NAME  DEV_UART2.dev_name
#endif


#ifndef DEV_UART3
#define DEV_UART3       devUsartLpc17xx_3
#endif

#ifndef DEV_UART3_NAME
#define DEV_UART3_NAME  DEV_UART3.dev_name
#endif

extern NUTDEVICE devUsartLpc17xx_0;
extern NUTDEVICE devUsartLpc17xx_1;
extern NUTDEVICE devUsartLpc17xx_2;
extern NUTDEVICE devUsartLpc17xx_3;

#endif
