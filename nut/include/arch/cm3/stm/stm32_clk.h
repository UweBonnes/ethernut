#ifndef _STM32_CLK_H_
#define _STM32_CLK_H_

/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
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
 * $Id: stm32_gpio.c 3182 2010-10-17 21:46:04Z Astralix $
 * \endverbatim
 */
#include <stdint.h>

/* STM32 Clock source selectors */
#define SYSCLK_HSI    1
#define SYSCLK_PLL    2
#define SYSCLK_HSE    3
#define SYSCLK_HSI48  4
#define SYSCLK_MSI    5
#define SYSCLK_HSIDIV 6
#define SYSCLK_PLLR   7

#define PLLCLK_AUTO         0
#define PLLCLK_HSI_DIV2     1
#define PLLCLK_HSI_PREDIV   2
#define PLLCLK_HSE_PREDIV   3
#define PLLCLK_HSI48_PREDIV 4
#define PLLCLK_HSI          5
#define PLLCLK_HSE          6
#define PLLCLK_PLL2         7
#define PLLCLK_MSI          8

/* RTC clock sources */
#define RTCCLK_NONE   0
#define RTCCLK_LSE    1
#define RTCCLK_LSI    2
#define RTCCLK_HSE    3
#define RTCCLK_LSEBYP 4

/* MSI clock ranges*/
typedef enum
{
    MSI_OFF = -1,
    MSI_100k,
    MSI_200k,
    MSI_400k,
    MSI_800k,
    MSI_1M,
    MSI_2M,
    MSI_4M,
    MSI_8M,
    MSI_16M,
    MSI_24M,
    MSI_32M,
    MSI_48M
}msi_range_t;


/* Retrieve CPU/AHB/APBx clock*/
extern uint32_t STM_ClockGet(int idx);

/* Control functions for the separate clocks */
extern int CtlHseClock(int ena);
extern int CtlHsiClock(int ena);
extern int CtlPllClock( uint8_t ena);

/* Selection functions for the clock sources */
extern int SetPllClock( int src);
extern int SetPllClockSource( int src);
extern int SetSysClock(void);
extern uint32_t SysCtlClockGet(void);
uint32_t Stm32ClockSwitchGet(int bi);

extern int SetRtcClock(int source);

#endif /* _STM32_CLK_H_ */
