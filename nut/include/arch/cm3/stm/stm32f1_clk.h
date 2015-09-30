/*
  * Copyright (C) 2015 by Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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
 * $Id$
 * \endverbatim
 */

#if !defined(HSI_VALUE)
#define HSI_VALUE 8000000
#endif

#if !defined(HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT 0x5000
#endif

#if defined(MCU_STM32F1_CL)
# define HSE_MAX  25000000
#elif defined(MCU_STM32F100)
# define HSE_MAX  24000000
#else
# define HSE_MAX  16000000
#endif

#if   defined(MCU_STM32F100)
# define SYSCLK_MAX 24000000
#elif defined(MCU_STM32F101)
# define SYSCLK_MAX 36000000
#elif defined(MCU_STM32F102)
# define SYSCLK_MAX 48000000
#else
# define SYSCLK_MAX 72000000
#endif

#if defined(HSE_VALUE)
# if HSE_VALUE < 4000000
#  warning HSE_VALUE too low
# elif HSE_VALUE > HSE_MAX
#  warning HSE_VALUE too high
# endif
#endif

/* TODO: Provide more heuristics*/
#if !defined(SYSCLK_FREQ)
# undef PLLCLK_SOURCE
# undef SYSCLK_SOURCE
# undef PLLCLK_DIV
# undef PLLCLK_MULT
# undef PLL2CLK_DIV
# undef PLL2CLK_MULT
# if   defined(MCU_STM32F100)
#  define SYSCLK_FREQ         24000000
#  if defined(HSE_VALUE) && HSE_VALUE == 24000000
#   define SYSCLK_SOURCE     SYSCLK_HSE
#  endif
# elif defined(MCU_STM32F101)
#  define SYSCLK_FREQ         36000000
# elif defined(MCU_STM32F102)
#   define SYSCLK_FREQ        48000000
# elif defined(MCU_STM32F103)
#  if defined(HSE_VALUE)
#   define SYSCLK_FREQ        72000000
#  else
#   define SYSCLK_FREQ        64000000
#  endif
# elif defined(MCU_STM32F1_CL)
#  define SYSCLK_SOURCE     SYSCLK_PLL
#  if defined(HSE_VALUE)
#   define SYSCLK_FREQ 72000000
#   if (HSE_VALUE == 25000000)
/* Values From STM32F105xx datasheet*/
#    define PLLCLK_SOURCE     PLLCLK_PLL2
#    define PLL2CLK_PREDIV    5
#    define PLL2CLK_MULT      8
#    define PLLCLK_DIV        5
#    define PLLCLK_MULT       9
#   elif (HSE_VALUE == 14745600)
#    define PLLCLK_SOURCE     PLLCLK_PLL2
#    define PLL2CLK_PREDIV    4
#    define PLL2CLK_MULT      12
#    define PLLCLK_DIV        4
#    define PLLCLK_MULT       (RCC_CFGR_PLLMULL6_5 / RCC_CFGR_PLLMULL_0)
#   elif (HSE_VALUE == 8000000)
#    define PLLCLK_SOURCE     PLLCLK_HSE
#    define PLLCLK_PREDIV     1
#    define PLLCLK_MULT       9
#   else
#    warning Please supply fitting values
#   endif
#  else
#   define SYSCLK_FREQ        36000000
#   define PLLCLK_SOURCE      PLLCLK_HSI
#   define PLLCLK_MULT        9
#  endif
# else
#  warning Unhandled device
# endif
/* Now handle Values for all devices but STM32F1_CL */
# if !defined(SYSCLK_SOURCE)
#  if defined(HSE_VALUE)
#   define SYSCLK_SOURCE    SYSCLK_PLL
#   if (HSE_VALUE ==  8000000)
#    define PLLCLK_SOURCE  PLLCLK_HSE
#    define PLLCLK_DIV     1
#    define PLLCLK_MULT    (SYSCLK_FREQ / HSE_VALUE)
#   else
#    warning Please supply fitting values
#   endif
#  else
#   define PLLCLK_SOURCE  PLLCLK_HSI
#   define PLLCLK_MULT       (SYSCLK_FREQ / HSI_VALUE)
#  endif
# endif
#endif

/* Some checks*/
#if SYSCLK_FREQ > SYSCLK_MAX
# warning Requested SYSCLK_FREQ too high
#endif

/* Provide Fallback values*/
#if !defined(PLL2CLK_PREDIV)
#define PLL2CLK_PREDIV 1
#endif
#if !defined(PLLCLK_PREDIV)
#define PLLCLK_PREDIV 1
#endif
