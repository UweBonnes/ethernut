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

#if !defined(HSE_VALUE)
# define HSE_VALUE 0
#endif

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
#define PLLCLK_HSE_PLL2     7
#define PLLCLK_MSI          8

/* RTC clock sources */
#define RTCCLK_NONE   0
#define RTCCLK_LSE    1
#define RTCCLK_LSI    2
#define RTCCLK_HSE    3
#define RTCCLK_LSEBYP 4

#if !defined(HSE_VALUE)
# define HSE_VALUE 0
#endif

#if !defined(LSE_VALUE)
# define LSE_VALUE 0
#endif

/* MSI clock ranges*/
typedef enum
{
    MSI_OFF = -2,
    MSI_FAILURE,
#if   defined(MCU_STM32L0) || defined(MCU_STM32L1)
    MSI_65_5k,
    MSI_131k,
    MSI_262k,
    MSI_524k,
    MSI_1050k,
    MSI_2100k,
    MSI_4200k,
#elif defined(MCU_STM32L4)
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
#endif
}msi_range_t;

typedef enum
{
    HWCLK_CPU = 0,
    HWCLK_APB1,
    HWCLK_APB1_TIMER,
#if defined(APB2PERIPH_BASE) || defined(MCU_STM32L0)
    HWCLK_APB2,
    HWCLK_APB2_TIMER,
#endif
    HWCLK_SYS,
#if defined(RCC_CFGR3_USART1SW) || defined(RCC_DCKCFGR2_USART1SEL) ||\
    defined(RCC_CCIPR_USART1SEL)
    HWCLK_HSI,
    HWCLK_LSE,
#endif
#if defined(MCU_STM32L4)
    HWCLK_MSI,
#endif
    HWCLK_MAX,
}clock_index_t;

#if defined(APB2PERIPH_BASE)
# define BASE2CLKSRC(base) ((base < APB2PERIPH_BASE) ? HWCLK_APB1 : HWCLK_APB2)
# define BASE2TCLKSRC(base) ((base < APB2PERIPH_BASE) ? \
                             HWCLK_APB1_TIMER : HWCLK_APB2_TIMER)
#else
# define BASE2CLKSRC(base) HWCLK_APB1
# define BASE2TCLKSRC(base) HWCLK_APB1_TIMER
#endif

extern uint32_t Stm32ClockGet(clock_index_t idx);

/* Control functions for the separate clocks */
extern int CtlHseClock(int ena);
extern int CtlHsiClock(int ena);
extern int CtlPllClock( uint8_t ena);

/* Selection functions for the clock sources */
extern int SetPllClock( int src);
extern int SetPllClockSource( int src);
extern int SetSysClock(void);
extern uint32_t SysCtlClockGet(void);

extern void SetRtcClockSource(int source);
extern int EnableRtcClock(int source);

#if (LSE_VALUE) && (defined(RCC_CR1_MSIPLLEN) || !defined(NO_LSEDRV))
# define NUT_INIT_DEV
extern void NutDeviceInit(void);
#endif

#endif /* _STM32_CLK_H_ */
