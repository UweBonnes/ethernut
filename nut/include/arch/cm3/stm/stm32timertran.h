/*
 * Copyright (C) 2013 by Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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
 * #define STM32_OWITIMER_CLK  STM32TIMER_CLK
 * #define STM32_OWITIMER_RST  STM32TIMER_RST
 * #define STM32_OWITIMER_PCLK STM32TIMER_PCLK
 * #define STM32_OWITIMER_NCH  STM32TIMER_NCH
 * #if defined(MCU_STM32F1)
 * #define STM32_OWITIMER_REMAP_MASK   STM32TIMER_REMAP_MASK
 * #define STM32_OWITIMER_REMAP_SHIFT  STM32TIMER_REMAP_SHIFT
 * #else
 * #define STM32_OWITIMER_AF  STM32TIMER_AF(STM32TIM_OWI_PORT, STM32TIM_OWI0_PIN)
 * #endif
 *
 * Provided entities
 * - STM32TIMER_BASE  The base address of the timer
 * - STM32TIMER_SIG   The signal for the timer.
 *                    FIXME: Multiple/Chained interrupts for TIM1, 9-17!
 * - STM32TIMER_CLK   The bitband register for the clock of the timer.
 * - STM32TIMER_RST   The bitband register to reset the timer.
 * - STM32TIMER_PCLK  The index for the timer clock
 * - STM32TIMER_NCH   Compare/Capture channels available for the timer.
 * - STM32TIMER_BDTR  Timer has Break and dead-time register.
 * (STM32F1)
 * - STM32TIMER_REMAP_MASK  Mask and shift value to remap the timer in AFIO_MAPR
 * - STM32TIMER_REMAP_SHIFT Mask and shift value to remap the timer in AFIO_MAPR
 *                    FIXME: The actual remap value must be given from the programmer.
 *(else)
 * - STM32TIMER_AF    The alternate function index to connect timer to pin.
 *                    FIXME: Handle some F3 corner cases
 */

#include <cfg/arch.h>
#include <arch/cm3/stm/stm32xxxx.h>

/*
 * Remove any previously defined register names.
 */

#undef STM32TIMER_BASE
#undef STM32TIMER_SIG
#undef STM32TIMER_CLK
#undef STM32TIMER_RST
#undef STM32TIMER_PCLK
#undef STM32TIMER_NCH
#undef STM32TIMER_BDTR
#undef STM32TIMER_AF
#undef STM32TIMER_REMAP_MASK
#undef STM32TIMER_REMAP_SHIFT

/* What did the F3 designers smoke when they distributed the AF so random? */
/* We leave the F1 remapping to the user */

#if defined(TIM1_BASE) && (STM32TIMER_ID == 0)
#define STM32TIMER_BASE TIM1_BASE
#define STM32TIMER_SIG sig_TIM1
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM1EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM1RST)))
#define STM32TIMER_BDTR
#if defined(MCU_STM32F30)
#define STM32TIMER_NCH 6
#else
#define STM32TIMER_NCH 4
#endif
#define STM32TIMER_PCLK NUT_HWCLK_PCLK2
#if defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 1
#elif defined(MCU_STM32F30)
#define STM32TIMER_AF(port, pin) (port == GPIOE_BASE)?                               2 : \
    (((port == GPIOB_BASE) && (pin == 15))||((port == GPIOC_BASE) && (pin == 13)))?  4 : \
    (( port == GPIOA_BASE) && (pin == 15))?                                          9 : \
    (((port == GPIOA_BASE) && (pin == 11))||((port == GPIOb_BASE) && (pin ==  8)))? 12 : 6
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM1_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM1_REMAP_0))
#endif

#elif defined (TIM2_BASE) && (STM32TIMER_ID == 2)
#define STM32TIMER_BASE TIM2_BASE
#define STM32TIMER_SIG sig_TIM2
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM2EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM2RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK1
#define STM32TIMER_NCH 4
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 1
#elif defined(MCU_STM32F30)
#define STM32TIMER_AF(port, pin) \
    (port == GPIOD_BASE) ? 2 :                                          \
    (((port == GPIOA_BASE) && (pin ==  9))||((port == GPIOA_BASE) && (pin == 10)))? 10 : 1
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM2_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM2_REMAP_0))
#endif

#elif defined (TIM3_BASE) && (STM32TIMER_ID == 3)
#define STM32TIMER_BASE TIM3_BASE
#define STM32TIMER_SIG sig_TIM3
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM3EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM3RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK1
#define STM32TIMER_NCH 4
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 2
#elif defined(MCU_STM32F30)
#define STM32TIMER_AF(port, pin) \
    (((port == GPIOB_BASE) && (pin ==  3)||((port == GPIOB_BASE) && (pin ==  7)))? 10 : 2
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM3_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM3_REMAP_0))
#endif

#elif defined (TIM4_BASE) && (STM32TIMER_ID == 4)
#define STM32TIMER_BASE TIM4_BASE
#define STM32TIMER_SIG sig_TIM4
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM4EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM4RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK1
#define STM32TIMER_NCH 4
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 2
#elif defined(MCU_STM32F30)
#define STM32TIMER_AF(port, pin) (port == GPIOA_BASE)? 10 : 2
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM4_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM4_REMAP))
#endif

#elif defined (TIM5_BASE) && (STM32TIMER_ID == 5)
#define STM32TIMER_BASE TIM5_BASE
#define STM32TIMER_SIG sig_TIM5
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM5EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM5RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK1
#define STM32TIMER_NCH 4
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 2
#endif

#elif defined (TIM6_BASE) && (STM32TIMER_ID == 6)
#define STM32TIMER_BASE TIM6_BASE
#define STM32TIMER_SIG sig_TIM6
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM6EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM6RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK1
#define STM32TIMER_NCH 0

#elif defined (TIM7_BASE) && (STM32TIMER_ID == 7 )
#define STM32TIMER_BASE TIM7_BASE
#define STM32TIMER_SIG sig_TIM7
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM7EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM7RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK1
#define STM32TIMER_NCH 0

#elif defined (TIM8_BASE) && (STM32TIMER_ID == 8 )
#define STM32TIMER_BASE TIM8_BASE
#define STM32TIMER_SIG sig_TIM8
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM9EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM8RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK2
#define STM32TIMER_BDTR
#if defined(MCU_STM32F30)
#define STM32TIMER_NCH 6
#else
#define STM32TIMER_NCH 4
#endif
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 3
#elif defined(MCU_STM32F30)
/* FIXME: This doesn't map TIM*_BKIN2 */
#define STM32TIMER_AF(port, pin) ((port == GPIOC_BASE) || (port == GPIOD_BASE)?      4 : \
    (( port == GPIOA_BASE) && (pin ==  0))?                                          9 : \
    (( port == GPIOA_BASE) && (pin == 10))?                                         11 : \
    (( port == GPIOA_BASE) && (pin == 15))?                                          2 : \
    (( port == GPIOB_BASE) && (pin ==  5))?                                          3 : \
    (( port == GPIOA_BASE) && (pin == 14))?                                          5 : \
    (( port == GPIOB_BASE) && (pin ==  6))?                                          5 : \
    (( port == GPIOB_BASE) && (pin ==  7))?                                          5 : \
    (( port == GPIOB_BASE) && ((pin ==  8) || (pin == 9)))?                         10 : 4

#endif

#elif defined (TIM9_BASE) && (STM32TIMER_ID == 9 )
#define STM32TIMER_BASE TIM9_BASE
#define STM32TIMER_SIG sig_TIM9
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM9EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM9RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK2
#define STM32TIMER_NCH 2
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 4
#endif

#elif defined (TIM10_BASE) && (STM32TIMER_ID == 10 )
#define STM32TIMER_BASE TIM10_BASE
#define STM32TIMER_SIG sig_TIM10
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM10EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM10RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK2
#define STM32TIMER_NCH 1
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 3
#endif

#elif defined (TIM11_BASE) && (STM32TIMER_ID == 11 )
#define STM32TIMER_BASE TIM11_BASE
#define STM32TIMER_SIG sig_TIM11
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM11EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM11RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK2
#define STM32TIMER_NCH 1
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 3
#endif

#elif defined (TIM12_BASE) && (STM32TIMER_ID == 12)
#define STM32TIMER_BASE TIM12_BASE
#define STM32TIMER_SIG sig_TIM12
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM12EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM12RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK1
#define STM32TIMER_NCH 2
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 9
#endif

#elif defined (TIM13_BASE) && (STM32TIMER_ID == 13 )
#define STM32TIMER_BASE TIM13_BASE
#define STM32TIMER_SIG sig_TIM13
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM13EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM13RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK1
#define STM32TIMER_NCH 1
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 9
#endif

#elif defined (TIM14_BASE) && (STM32TIMER_ID == 14 )
#define STM32TIMER_BASE TIM14_BASE
#define STM32TIMER_SIG sig_TIM14
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM14EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM14RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK1
#define STM32TIMER_NCH 1
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 9
#endif

#elif defined (TIM15_BASE) && (STM32TIMER_ID == 15 )
#define STM32TIMER_BASE TIM15_BASE
#define STM32TIMER_SIG sig_TIM15
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM15EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM15RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK2
#define STM32TIMER_NCH 2
#if defined(MCU_STM32F30)
/* FIXME: This doesn't map TIM15 CH1N*/
#define STM32TIMER_AF(port, pin) (port == GPIOA_BASE)?  9 : (port == GPIOB_BASE) ? 1 :3
#endif

#elif defined (TIM16_BASE) && (STM32TIMER_ID == 16 )
#define STM32TIMER_BASE TIM16_BASE
#define STM32TIMER_SIG sig_TIM16
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM16EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM16RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK2
#define STM32TIMER_NCH 1
#if defined(MCU_STM32F30)
#define STM32TIMER_AF(port, pin) (port == GPIOE_BASE)?  4 :  1
#endif

#elif defined (TIM17_BASE) && (STM32TIMER_ID == 17 )
#define STM32TIMER_BASE TIM17_BASE
#define STM32TIMER_SIG sig_TIM17
#define STM32TIMER_CLK (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM17EN)))
#define STM32TIMER_RST (CM3BBREG(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM17RST)))
#define STM32TIMER_PCLK NUT_HWCLK_PCLK2
#define STM32TIMER_NCH 1
#if defined(MCU_STM32F30)
#define STM32TIMER_AF(port, pin) (port == GPIOE_BASE)?           4 : \
    (  port == GPIOA_BASE)                                       1 : \
    (( port == GPIOB_BASE) && ((pin ==  7))?                     1 : \
    (( port == GPIOA_BASE) && ((pin ==  4) || (pin ==  5))?     10 : 1
#endif
#else
#warning No match
#endif
