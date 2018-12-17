/*
 * Copyright (C) 2013, 2015-17 by Uwe Bonnes
 *                             (bon@elektron.ikp.physik.tu-darmstadt.de)
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
 *
 */

/*
 * $Id$
 */

/*!
 * \file include/arch/cm3/stm/stm32timertran.h
 * \brief Compile time STM32 timer translations.
 *
 * This header file determines the target specific timer entities,
 * by a simple configured timer identifier.
 *
 * Unlike most other header files, this one may be included several
 * times within a single source file, typically once for each configured
 * identifier.
 *
 * \code
 * #undef  STM32TIMER_ID
 * #define STM32TIMER_ID STM32TIM_OWI0_TIMER_ID
 * #include <arch/cm3/stm/stm32timertran.h>
 * #define STM32_OWITIMER_BASE STM32TIMER_BASE
 * #define STM32_OWITIMER_SIG  STM32TIMER_SIG
 * #define STM32_OWITIMER_CLK() STM32TIMER_CLK()
 * #define STM32_OWITIMER_RST() STM32TIMER_RST()
 * #define STM32_OWITIMER_SW STM32TIMER_SW
 * #define STM32_OWITIMER_NCH  STM32TIMER_NCH
 * #if defined(MCU_STM32F1)
 * #define STM32_OWITIMER_REMAP_REG    STM32TIMER_REMAP_REG
 * #define STM32_OWITIMER_REMAP_MASK   STM32TIMER_REMAP_MASK
 * #define STM32_OWITIMER_REMAP_SHIFT  STM32TIMER_REMAP_SHIFT
 * #define STM32_OWITIMER_REMAP_VALUE  STM32TIMER_REMAP_VALUE
 * #endif
 * #define STM32_OWITIMER_WIDTH STM32TIMER_WIDTH
 *
 * Provided entities
 * - STM32TIMER_BASE  The base address of the timer
 * - STM32TIMER_SIG   The signal for the timer.
 *                    FIXME: Multiple/Chained interrupts for TIM1, 9-17!
 * - STM32TIMER_CLK() Function to enable the clock for the timer.
 * - STM32TIMER_RST() Function to reset the timer.
 * - STM32TIMER_NCH   Compare/Capture channels available for the timer.
 * - STM32TIMER_BDTR  Timer has Break and dead-time register.
 * (STM32F3)
 * - STM32TIMER_SW    RCC_CFGR3 Bit indicating PLLCLKx2 is selected.
 * (STM32F1)
 * - STM32TIMER_REMAP_REG   Involved MAPR register.
 * - STM32TIMER_REMAP_MASK  Mask to clear MAPR.
 * - STM32TIMER_REMAP_SHIFT Shift value for user provided REMAP Value.
 * - STM32TIMER_REMAP_VALUE Global remap value defined fot the choosen timer.
 *(else)
 * - STM32TIMER_WIDTH The width of the timer in bits.
 */

#include <cfg/arch.h>
#include <cfg/devices.h>
#if defined(MCU_STM32F1)
# include <cfg/timer.h>
#endif
#include <arch/cm3/stm/stm32xxxx.h>
#include <arch/cm3/stm/stm32_clk.h>

/* In the Cube headers, smaller devices that don't have GPIOX don't
 * defined that GPIOX_BASE. This confused out heuristic!
 */
#if defined(MCU_STM32F3) && !defined(GPIOE_BASE)
# define GPIOE_BASE 0
#endif
/*
 * Remove any previously defined register names.
 */

#undef STM32TIMER_BASE
#undef STM32TIMER_MASK
#undef STM32TIMER_SIG
#undef STM32TIMER_CLK
#undef STM32TIMER_RST
#undef STM32TIMER_INIT
#undef STM32TIMER_SW
#undef STM32TIMER_NCH
#undef STM32TIMER_BDTR
#if defined(MCU_STM32F1)
#undef STM32TIMER_REMAP_REG
#undef STM32TIMER_REMAP_MASK
#undef STM32TIMER_REMAP_SHIFT
#undef STM32TIMER_REMAP_VALUE
#endif
#undef STM32TIMER_WIDTH

#define CM3BBPULSE(base, regstruct, reg, bit) do {      \
    CM3BBSET(base, regstruct, reg, bit);                \
    CM3BBCLR(base, regstruct, reg, bit); } while(0)

/* What did the F3 designers smoke when they distributed the AF so random? */
/* We leave the F1 remapping to the user */

#if defined(RCC_APB2ENR_TIM1EN) && defined(RCC_APB2RSTR_TIM1RST) && (STM32TIMER_ID == 1)
#define STM32TIMER_BASE TIM1_BASE
#define STM32TIMER_MASK RCC_APB2ENR_TIM1EN
#if defined(HW_BRK_STM32)
# define STM32TIMER_SIG sig_TIM1_UP
#elif defined(HW_TIM1_BRK_UP_TRG_COM_STM32)
# define STM32TIMER_SIG sig_TIM1_BRK_UP_TRG_COM
#elif defined(HW_TIM1_BRK_TIM9_STM32)
# define STM32TIMER_SIG sig_TIM1_UP_TIM10
#elif defined(HW_TIM1_BRK_TIM15_STM32)
# define STM32TIMER_SIG sig_TIM1_UP_TIM16
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM1EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM1RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM1EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM1RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM1RST)); } while(0)
#define STM32TIMER_BDTR
#if defined(MCU_STM32F3)
#define STM32TIMER_NCH 6
#else
#define STM32TIMER_NCH 4
#endif
# if defined(RCC_CFGR3_TIM1SW_PLL)
#  define STM32TIMER_SW RCC_CFGR3_TIM1SW_PLL
# endif
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM1_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM1_REMAP_0))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM1_REMAP
#endif

#elif defined (RCC_APB1ENR_TIM2EN) && defined(RCC_APB1RSTR_TIM2RST) && (STM32TIMER_ID == 2)
#define STM32TIMER_BASE TIM2_BASE
#define STM32TIMER_MASK RCC_APB1ENR_TIM2EN
#define STM32TIMER_SIG sig_TIM2
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM2EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM2RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM2EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM2RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM2RST)); } while(0)
# if defined(RCC_CFGR3_TIM2SW_PLL)
#  define STM32TIMER_SW RCC_CFGR3_TIM2SW_PLL
# endif
#define STM32TIMER_NCH 4
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM2_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM2_REMAP_0))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM2_REMAP
#endif
#if defined(HW_TIM2_32BIT_STM32)
#define STM32TIMER_WIDTH 32
#endif

#elif defined (RCC_APB1ENR_TIM3EN) && defined(RCC_APB1RSTR_TIM3RST) && (STM32TIMER_ID == 3)
#define STM32TIMER_BASE TIM3_BASE
#define STM32TIMER_MASK RCC_APB1ENR_TIM3EN
#define STM32TIMER_SIG sig_TIM3
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM3EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM3RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM3EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM3RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM3RST)); } while(0)
# if defined(RCC_CFGR3_TIM34SW_PLL)
#  define STM32TIMER_SW RCC_CFGR3_TIM34SW_PLL
# endif
#define STM32TIMER_NCH 4
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM3_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM3_REMAP_0))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM3_REMAP
#endif

#elif defined (RCC_APB1ENR_TIM4EN) && defined(RCC_APB1RSTR_TIM4RST) && (STM32TIMER_ID == 4)
#define STM32TIMER_BASE TIM4_BASE
#define STM32TIMER_MASK RCC_APB1ENR_TIM4EN
#define STM32TIMER_SIG sig_TIM4
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM4EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM4RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM4EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM4RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM4RST)); } while(0)
# if defined(RCC_CFGR3_TIM34SW_PLL)
#  define STM32TIMER_SW RCC_CFGR3_TIM34SW_PLL
# endif
#define STM32TIMER_NCH 4
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM4_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM4_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM4_REMAP
#endif

#elif defined (RCC_APB1ENR_TIM5EN) && defined(RCC_APB1RSTR_TIM5RST) && (STM32TIMER_ID == 5)
#define STM32TIMER_BASE TIM5_BASE
#define STM32TIMER_MASK RCC_APB1ENR_TIM5EN
#define STM32TIMER_SIG sig_TIM5
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM5EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM5RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM5EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM5RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM5RST)); } while(0)
#define STM32TIMER_NCH 4
#if defined(MCU_STM32F1)
/* Only valid for channel 4*/
#define STM32TIMER_REMAP_REG   AFIO->MAPR
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM5CH4_IREMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM5CH4_IREMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM5_REMAP
#endif
#if defined(HW_TIM5_32BIT_STM32)
#define STM32TIMER_WIDTH 32
#endif

#elif defined (RCC_APB1ENR_TIM6EN) && defined(RCC_APB1RSTR_TIM6RST) && (STM32TIMER_ID == 6)
#define STM32TIMER_BASE TIM6_BASE
#define STM32TIMER_MASK RCC_APB1ENR_TIM6EN
#if defined(HW_TIM6_DAC1_STM32) || defined(HW_TIM6_DAC_STM32)
# define STM32TIMER_SIG sig_TIM6_DAC1
#else
# define STM32TIMER_SIG sig_TIM6
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM6EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM6RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM6EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM6RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM6RST)); } while(0)
#define STM32TIMER_NCH 0

#elif defined (RCC_APB1ENR_TIM7EN) && defined(RCC_APB1RSTR_TIM7RST) && (STM32TIMER_ID == 7 )
#define STM32TIMER_BASE TIM7_BASE
#define STM32TIMER_MASK RCC_APB1ENR_TIM7EN
#define STM32TIMER_SIG sig_TIM7
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM7EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM7RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM7EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM7RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM7RST)); } while(0)
#define STM32TIMER_NCH 0

#elif defined (RCC_APB2ENR_TIM8EN) && defined(RCC_APB2RSTR_TIM8RST) && (STM32TIMER_ID == 8 )
#define STM32TIMER_BASE TIM8_BASE
#define STM32TIMER_MASK RCC_APB2ENR_TIM8EN
#if defined(HW_TIM8_BRK_TIM12_STM32)
# define STM32TIMER_SIG sig_TIM8_UP_TIM13
#else
# define STM32TIMER_SIG sig_TIM8
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM8EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM8RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM8EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM8RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM8RST)); } while(0)
# if defined(RCC_CFGR3_TIM8SW_PLL)
#  define STM32TIMER_SW RCC_CFGR3_TIM8SW_PLL
# endif
#define STM32TIMER_BDTR
#if defined(MCU_STM32F3)
#define STM32TIMER_NCH 6
#else
#define STM32TIMER_NCH 4
#endif

#elif defined (RCC_APB2ENR_TIM9EN) && defined(RCC_APB2RSTR_TIM9RST) && (STM32TIMER_ID == 9 )
#define STM32TIMER_BASE TIM9_BASE
#define STM32TIMER_MASK RCC_APB2ENR_TIM9EN
#if defined(HW_TIM1_BRK_TIM9_STM32)
# define STM32TIMER_SIG sig_TIM1_BRK_TIM9
#else
# define STM32TIMER_SIG sig_TIM9
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM9EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM9RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM9EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM9RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM9RST)); } while(0)
#define STM32TIMER_NCH 2
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM9_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM9_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM9_REMAP
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM10_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM10_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM10_REMAP
#endif

#elif defined (RCC_APB2ENR_TIM10EN) && defined(RCC_APB2RSTR_TIM10RST) && (STM32TIMER_ID == 10 )
#define STM32TIMER_BASE TIM10_BASE
#define STM32TIMER_MASK RCC_APB2ENR_TIM10EN
#if defined(HW_TIM1_BRK_TIM9_STM32)
# define STM32TIMER_SIG sig_TIM1_UP_TIM10
#else
# define STM32TIMER_SIG sig_TIM10
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM10EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM10RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM10EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM10RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM10RST)); } while(0)
#define STM32TIMER_NCH 2
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM10_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM10_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM10_REMAP
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM10_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM10_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM10_REMAP
#endif

#elif defined (RCC_APB2ENR_TIM11EN) && defined(RCC_APB2RSTR_TIM11RST) && (STM32TIMER_ID == 11 )
#define STM32TIMER_BASE TIM11_BASE
#define STM32TIMER_MASK RCC_APB2ENR_TIM11EN
#if defined(HW_TIM1_BRK_TIM9_STM32)
# define STM32TIMER_SIG sig_TIM1_TRG_COM_TIM11
#else
# define STM32TIMER_SIG sig_TIM11
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM11EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM11RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB2ENR_TIM11EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB2RSTR_TIM11RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB2RSTR_TIM11RST)); } while(0)
#define STM32TIMER_NCH 1
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM11_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM11_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM11_REMAP
#endif

#elif defined (RCC_APB1ENR_TIM12EN) && defined(RCC_APB1RSTR_TIM12RST) && (STM32TIMER_ID == 12)
#define STM32TIMER_BASE TIM12_BASE
#define STM32TIMER_MASK RCC_APB1ENR_TIM12EN
#if defined(HW_TIM8_BRK_TIM12_STM32)
# define STM32TIMER_SIG sig_TIM8_BRK_TIM12
#else
# define STM32TIMER_SIG sig_TIM12
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM12EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM12RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM12EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM12RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM12RST)); } while(0)
#define STM32TIMER_NCH 2
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM12_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM12_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM12_REMAP
#endif

#elif defined (RCC_APB1ENR_TIM13EN) && defined(RCC_APB1RSTR_TIM13RST) && (STM32TIMER_ID == 13 )
#define STM32TIMER_BASE TIM13_BASE
#define STM32TIMER_MASK RCC_APB1ENR_TIM13EN
#if defined(HW_TIM8_BRK_TIM12_STM32)
# define STM32TIMER_SIG sig_TIM8_UP_TIM13
#else
# define STM32TIMER_SIG sig_TIM13
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM13EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM13RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM13EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM13RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM13RST)); } while(0)
#define STM32TIMER_NCH 1
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM13_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM13_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM13_REMAP
#endif

#elif defined (RCC_APB1ENR_TIM14EN) && defined(RCC_APB1RSTR_TIM14RST) && (STM32TIMER_ID == 14 )
#define STM32TIMER_BASE TIM14_BASE
#define STM32TIMER_MASK RCC_APB1ENR_TIM14EN
#if defined(HW_TIM8_BRK_TIM12_STM32)
# define STM32TIMER_SIG sig_TIM8_TRG_COM_TIM14
#else
# define STM32TIMER_SIG sig_TIM14
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM14EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM14RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM14EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM14RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM14RST)); } while(0)
#define STM32TIMER_NCH 1
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM14_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM14_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM14_REMAP
#endif

#elif defined (RCC_APB2ENR_TIM15EN) && defined(RCC_APB2RSTR_TIM15RST) && (STM32TIMER_ID == 15 )
#define STM32TIMER_BASE TIM15_BASE
#define STM32TIMER_MASK RCC_APB2ENR_TIM15EN
#if defined(HW_TIM1_BRK_TIM15_STM32)
# define STM32TIMER_SIG sig_TIM1_BRK_TIM15
#else
# define STM32TIMER_SIG sig_TIM15
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM15EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM15RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM15EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM15RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM15RST)); } while(0)
# if defined(RCC_CFGR3_TIM15SW_PLL)
#  define STM32TIMER_SW RCC_CFGR3_TIM15SW_PLL
# endif
#define STM32TIMER_NCH 2
#define STM32TIMER_BDTR
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM15_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM15_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM15_REMAP
#endif

#elif defined (RCC_APB2ENR_TIM16EN) && defined(RCC_APB2RSTR_TIM16RST) && (STM32TIMER_ID == 16 )
#define STM32TIMER_BASE TIM16_BASE
#define STM32TIMER_MASK RCC_APB2ENR_TIM16EN
#if defined(HW_TIM1_BRK_TIM15_STM32)
# define STM32TIMER_SIG sig_TIM1_UP_TIM16
#else
# define STM32TIMER_SIG sig_TIM16
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM16EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM16RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM16EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM16RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM16RST)); } while(0)
# if defined(RCC_CFGR3_TIM16SW_PLL)
#  define STM32TIMER_SW RCC_CFGR3_TIM16SW_PLL
# endif
#define STM32TIMER_NCH 1
#define STM32TIMER_BDTR
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM16_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM16_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM16_REMAP
#endif

#elif defined (RCC_APB2ENR_TIM17EN) && defined(RCC_APB2RSTR_TIM17RST) && (STM32TIMER_ID == 17 )
#define STM32TIMER_BASE TIM17_BASE
#define STM32TIMER_MASK RCC_APB2ENR_TIM17EN
#if defined(HW_TIM1_BRK_TIM15_STM32)
# define STM32TIMER_SIG sig_TIM1_UP_TRG_COM_TIM17
#else
# define STM32TIMER_SIG sig_TIM17
#endif
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM17EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM17RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM17EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM17RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM17RST)); } while(0)
# if defined(RCC_CFGR3_TIM17SW_PLL)
#  define STM32TIMER_SW RCC_CFGR3_TIM17SW_PLL
# endif
#define STM32TIMER_NCH 1
#define STM32TIMER_BDTR
#if defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   AFIO->MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM17_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM17_REMAP))
#define STM32TIMER_REMAP_VALUE STM32F1_TIM17_REMAP
#endif

#elif defined (RCC_APB2ENR_TIM18EN) && defined(RCC_APB2RSTR_TIM18RST) && (STM32TIMER_ID == 18 )
#define STM32TIMER_BASE TIM18_BASE
#define STM32TIMER_MASK RCC_APB2ENR_TIM18EN
#define STM32TIMER_SIG sig_TIM18
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM18EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM18RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM18EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM18RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_TIM18RST)); } while(0)
#define STM32TIMER_NCH 0

#elif defined (RCC_APB2ENR_TIM20EN) && defined(RCC_APB2RSTR_TIM20RST) && (STM32TIMER_ID == 20 )
#define STM32TIMER_BASE TIM20_BASE
#define STM32TIMER_SIG sig_TIM20
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM20EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM20RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM20EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM20RST)); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB2RSTR, _BI32(RCC_APB2RSTR_TIM20RST)); } while(0)
/* Tim 20 only available in F303E chip. Always 6 ch..*/
#define STM32TIMER_BDTR
#define STM32TIMER_SW RCC_CFGR3_TIM20SW_PLL
#define STM32TIMER_NCH 6

#elif defined (RCC_APB2ENR_TIM19EN) && defined(RCC_APB2RSTR_TIM19RST) && (STM32TIMER_ID == 19 )
#define STM32TIMER_BASE TIM19_BASE
#define STM32TIMER_SIG sig_TIM19
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM19EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM19RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB2ENR_TIM19EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB2RSTR_TIM19RST); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB2RSTR_TIM19RST)); } while(0)
#define STM32TIMER_NCH 4

/* Only on L0  as of June 2015 */
#elif defined (RCC_APB2ENR_TIM21EN) && defined(RCC_APB2RSTR_TIM21RST) && (STM32TIMER_ID == 21 )
#define STM32TIMER_BASE TIM21_BASE
#define STM32TIMER_MASK RCC_APB2ENR_TIM21EN
#define STM32TIMER_SIG sig_TIM21
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM21EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM21RST))
#define STM32TIMER_INIT() do{                                           \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB2ENR_TIM21EN)); \
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB2RSTR_TIM21RST); \
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB2RSTR_TIM21RST)); } while(0)
#define STM32TIMER_NCH 2

#else
#warning No match
#endif

#define STM32TIMER ((TIM_TypeDef*)STM32TIMER_BASE)

/* Only few timers are 32 bit. Use this "catch all else" to reduce duplication*/
#if !defined(STM32TIMER_WIDTH)
#define STM32TIMER_WIDTH 16
#endif
