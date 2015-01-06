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
 * #define STM32_OWITIMER_CLK() STM32TIMER_CLK()
 * #define STM32_OWITIMER_RST() STM32TIMER_RST()
 * #define STM32_OWITIMER_PCLK STM32TIMER_PCLK
 * #define STM32_OWITIMER_NCH  STM32TIMER_NCH
 * #if defined(MCU_STM32F1)
 * #define STM32_OWITIMER_REMAP_REG    STM32TIMER_REMAP_REG
 * #define STM32_OWITIMER_REMAP_MASK   STM32TIMER_REMAP_MASK
 * #define STM32_OWITIMER_REMAP_SHIFT  STM32TIMER_REMAP_SHIFT
 * #else
 * #define STM32_OWITIMER_AF  STM32TIMER_AF(STM32TIM_OWI_PORT, STM32TIM_OWI0_PIN)
 * #endif
 * #define STM32_OWITIMER_WIDTH STM32TIMER_WIDTH
 *
 * Provided entities
 * - STM32TIMER_BASE  The base address of the timer
 * - STM32TIMER_SIG   The signal for the timer.
 *                    FIXME: Multiple/Chained interrupts for TIM1, 9-17!
 * - STM32TIMER_CLK() Function to enable the clock for the timer.
 * - STM32TIMER_RST() Function to reset the timer.
 * - STM32TIMER_PCLK  The Timer input clock CK_IN from the clock tree in Hertz.
 * - STM32TIMER_NCH   Compare/Capture channels available for the timer.
 * - STM32TIMER_BDTR  Timer has Break and dead-time register.
 * (STM32F1)
 * - STM32TIMER_REMAP_REG   Involved MAPR register.
 * - STM32TIMER_REMAP_MASK  Mask to clear MAPR.
 * - STM32TIMER_REMAP_SHIFT Shift value for user provided REMAP Value.
 *(else)
 * - STM32TIMER_AF    The alternate function index to connect timer to pin.
 *                    FIXME: Handle some F3 corner cases
 * - STM32TIMER_WIDTH The width of the timer in bits.
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
#if defined(MCU_STM32F1)
#undef STM32TIMER_REMAP_REG
#undef STM32TIMER_REMAP_MASK
#undef STM32TIMER_REMAP_SHIFT
#else
#undef STM32TIMER_AF
#endif
#undef STM32TIMER_WIDTH

#define CM3BBPULSE(base, regstruct, reg, bit) do {      \
    CM3BBSET(base, regstruct, reg, bit);                \
    CM3BBCLR(base, regstruct, reg, bit); } while(0)

/* What did the F3 designers smoke when they distributed the AF so random? */
/* We leave the F1 remapping to the user */

#if defined(RCC_APB2ENR_TIM1EN) && defined(RCC_APB2RSTR_TIM1RST) && (STM32TIMER_ID == 1)
#define STM32TIMER_BASE TIM1_BASE
//#define STM32TIMER_SIG sig_TIM1
#define STM32TIMER_SIG sig_TIM1_UP_TIM10
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM1EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM1RST))
#define STM32TIMER_BDTR
#if defined(MCU_STM32F3)
#define STM32TIMER_NCH 6
#else
#define STM32TIMER_NCH 4
#endif
#if defined(MCU_STM32F3)
/* RCC_CFGR3_TIM1SW may only be set with AHBdiv = ABP2DIV = 1, so timerclock is 2 * PCLK
   with RCC_CFGR3_TIM1SW or RCC_CFGR_PPRE2_2 set*/
#define STM32TIMER_PCLK \
    (((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR3, _BI32(RCC_CFGR3_TIM1SW))) || \
      (CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2))))? \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#else
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#endif
#if defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 1
#elif defined(MCU_STM32F3)
#define STM32TIMER_AF(port, pin) (port == GPIOE_BASE)?                               2 : \
    (((port == GPIOB_BASE) && (pin == 15))||((port == GPIOC_BASE) && (pin == 13)))?  4 : \
    (( port == GPIOA_BASE) && (pin == 15))?                                          9 : \
    (((port == GPIOA_BASE) && (pin == 11))||((port == GPIOB_BASE) && (pin ==  8)))? 12 : 6
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM1_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM1_REMAP_0))
#endif

#elif defined (RCC_APB1ENR_TIM2EN) && defined(RCC_APB1RSTR_TIM2RST) && (STM32TIMER_ID == 2)
#define STM32TIMER_BASE TIM2_BASE
#define STM32TIMER_SIG sig_TIM2
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM2EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM2RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE1_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK1)) : (NutClockGet(NUT_HWCLK_PCLK1)))
#define STM32TIMER_NCH 4
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 1
#elif defined(MCU_STM32F3)
#define STM32TIMER_AF(port, pin) \
    (port == GPIOD_BASE) ? 2 :                                          \
    (((port == GPIOA_BASE) && (pin ==  9))||((port == GPIOA_BASE) && (pin == 10)))? 10 : 1
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM2_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM2_REMAP_0))
#endif
#if defined(MCU_STM32F2) ||defined(MCU_STM32F3)||defined(MCU_STM32F4)
#define STM32TIMER_WIDTH 32
#endif

#elif defined (RCC_APB1ENR_TIM3EN) && defined(RCC_APB1RSTR_TIM3RST) && (STM32TIMER_ID == 3)
#define STM32TIMER_BASE TIM3_BASE
#define STM32TIMER_SIG sig_TIM3
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM3EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM3RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE1_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK1)) : (NutClockGet(NUT_HWCLK_PCLK1)))
#define STM32TIMER_NCH 4
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 2
/* STM32F30: PA4/CH2/AF2, PA6/CH1/AF2,PA7/CH3/AF2,
             PB0/CH3/AF2, PB1/CH4/AF2, PB5/CH2/AF2,
             PB3/ETR/AF10
             PC6/CH1/AF2, PC7/CH2/AF2, PC8/CH3/AF2, PC9/CH4/AF2,
             PD2/CH1/AF2,
             PE2/CH1/AF2, PE3/CH3/AF2, PE4/CH3/AF2, PE5/CH4/AF2
   STM32F37: PA4/CH2/AF2, PA6/CH1/AF2,PA7/CH3/AF2,
             PB0/CH2/AF10, PB3/ETR/AF10, PB6/CH2/AF10, PB6/CH2/AF10,
             PB0/CH3/AF2, PB1/CH4/AF2, PB5/CH2/AF2,
             PC6/CH1/AF2, PC7/CH2/AF2, PC8/CH3/AF2, PC9/CH4/AF2,
             PD2/ETR/AF2
*/
#elif defined(STM32F30X)
#define STM32TIMER_AF(port, pin) \
    ((port == GPIOB_BASE) && (pin ==  3))? 10 : 2
#elif defined(STM32F37X)
#define STM32TIMER_AF(port, pin) \
    ((port == GPIOB_BASE) && ((pin ==  0) || (pin == 3) || (pin == 6) || (pin == 7)))? 10 : 2
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM3_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM3_REMAP_0))
#else
#warning Illegal pin mapping for TIM3
#endif

#elif defined (RCC_APB1ENR_TIM4EN) && defined(RCC_APB1RSTR_TIM4RST) && (STM32TIMER_ID == 4)
#define STM32TIMER_BASE TIM4_BASE
#define STM32TIMER_SIG sig_TIM4
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM4EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM4RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE1_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK1)) : (NutClockGet(NUT_HWCLK_PCLK1)))
#define STM32TIMER_NCH 4
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 2
#elif defined(MCU_STM32F3)
#define STM32TIMER_AF(port, pin) (port == GPIOA_BASE)? 10 : 2
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM4_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM4_REMAP))
#endif

#elif defined (RCC_APB1ENR_TIM5EN) && defined(RCC_APB1RSTR_TIM5RST) && (STM32TIMER_ID == 5)
#define STM32TIMER_BASE TIM5_BASE
#define STM32TIMER_SIG sig_TIM5
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM5EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM5RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE1_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK1)) : (NutClockGet(NUT_HWCLK_PCLK1)))
#define STM32TIMER_NCH 4
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 2
#elif defined(MCU_STM32F1)
/* Only valid for channel 4*/
#define STM32TIMER_REMAP_REG   MAPR
#define STM32TIMER_REMAP_MASK  AFIO_MAPR_TIM5CH4_IREMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR_TIM5CH4_IREMAP))
#endif
#if defined(MCU_STM32F2) || defined(MCU_STM32F4)
#define STM32TIMER_WIDTH 32
#endif

#elif defined (RCC_APB1ENR_TIM6EN) && defined(RCC_APB1RSTR_TIM6RST) && (STM32TIMER_ID == 6)
#define STM32TIMER_BASE TIM6_BASE
#define STM32TIMER_SIG sig_TIM6
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM6EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM6RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE1_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK1)) : (NutClockGet(NUT_HWCLK_PCLK1)))
#define STM32TIMER_NCH 0

#elif defined (RCC_APB1ENR_TIM7EN) && defined(RCC_APB1RSTR_TIM7RST) && (STM32TIMER_ID == 7 )
#define STM32TIMER_BASE TIM7_BASE
#define STM32TIMER_SIG sig_TIM7
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM7EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM7RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE1_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK1)) : (NutClockGet(NUT_HWCLK_PCLK1)))
#define STM32TIMER_NCH 0

#elif defined (RCC_APB2ENR_TIM8EN) && defined(RCC_APB2RSTR_TIM8RST) && (STM32TIMER_ID == 8 )
#define STM32TIMER_BASE TIM8_BASE
#define STM32TIMER_SIG sig_TIM8
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM8EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM8RST))
#if defined(MCU_STM32F3)
/* RCC_CFGR3_TIM8SW may only be set with AHBdiv = ABP2DIV = 1, so timerclock is 2 * PCLK
   with RCC_CFGR3_TIM8SW or RCC_CFGR_PPRE2_2 set*/
#define STM32TIMER_PCLK \
    (((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR3, _BI32(RCC_CFGR3_TIM8SW))) || \
      (CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2))))? \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#else
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#endif
#define STM32TIMER_BDTR
#if defined(MCU_STM32F3)
#define STM32TIMER_NCH 6
#else
#define STM32TIMER_NCH 4
#endif
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 3
#elif defined(MCU_STM32F3)
/* FIXME: This doesn't map TIM*_BKIN2 */
#define STM32TIMER_AF(port, pin) ((port == GPIOC_BASE) || (port == GPIOD_BASE))?     4 : \
    (( port == GPIOA_BASE) && (pin ==  0))?                                          9 : \
    (( port == GPIOA_BASE) && (pin == 10))?                                         11 : \
    (( port == GPIOA_BASE) && (pin == 15))?                                          2 : \
    (( port == GPIOB_BASE) && (pin ==  5))?                                          3 : \
    (( port == GPIOA_BASE) && (pin == 14))?                                          5 : \
    (( port == GPIOB_BASE) && (pin ==  6))?                                          5 : \
    (( port == GPIOB_BASE) && (pin ==  7))?                                          5 : \
    (( port == GPIOB_BASE) && ((pin ==  8) || (pin == 9)))?                         10 : 4

#endif

#elif defined (RCC_APB2ENR_TIM9EN) && defined(RCC_APB2RSTR_TIM9RST) && (STM32TIMER_ID == 9 )
#define STM32TIMER_BASE TIM9_BASE
#define STM32TIMER_SIG sig_TIM9
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM9EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM9RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#define STM32TIMER_NCH 2
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 4
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM9_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM9_REMAP))
#endif

#elif defined (RCC_APB2ENR_TIM10EN) && defined(RCC_APB2RSTR_TIM10RST) && (STM32TIMER_ID == 10 )
#define STM32TIMER_BASE TIM10_BASE
#define STM32TIMER_SIG sig_TIM10
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM10EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM10RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#define STM32TIMER_NCH 1
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 3
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM10_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM10_REMAP))
#endif

#elif defined (RCC_APB2ENR_TIM11EN) && defined(RCC_APB2RSTR_TIM11RST) && (STM32TIMER_ID == 11 )
#define STM32TIMER_BASE TIM11_BASE
#define STM32TIMER_SIG sig_TIM11
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM11EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM11RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#define STM32TIMER_NCH 1
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 3
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM11_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM11_REMAP))
#endif

#elif defined (RCC_APB1ENR_TIM12EN) && defined(RCC_APB1RSTR_TIM12RST) && (STM32TIMER_ID == 12)
#define STM32TIMER_BASE TIM12_BASE
#define STM32TIMER_SIG sig_TIM12
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM12EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM12RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE1_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK1)) : (NutClockGet(NUT_HWCLK_PCLK1)))
#define STM32TIMER_NCH 2
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 9
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM12_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM12_REMAP))
#endif

#elif defined (RCC_APB1ENR_TIM13EN) && defined(RCC_APB1RSTR_TIM13RST) && (STM32TIMER_ID == 13 )
#define STM32TIMER_BASE TIM13_BASE
#define STM32TIMER_SIG sig_TIM13
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM13EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM13RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE1_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK1)) : (NutClockGet(NUT_HWCLK_PCLK1)))
#define STM32TIMER_NCH 1
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 9
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM13_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM13_REMAP))
#endif

#elif defined (RCC_APB1ENR_TIM14EN) && defined(RCC_APB1RSTR_TIM14RST) && (STM32TIMER_ID == 14 )
#define STM32TIMER_BASE TIM14_BASE
#define STM32TIMER_SIG sig_TIM14
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM14EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM14RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE1_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK1)) : (NutClockGet(NUT_HWCLK_PCLK1)))
#define STM32TIMER_NCH 1
#if defined(MCU_STM32L1) || defined(MCU_STM32F2) ||defined(MCU_STM32F4)
#define STM32TIMER_AF(port, pin) 9
#elif defined(STM32F37X)
#define STM32TIMER_AF(port, pin) (port == GPIOF_BASE)? 2: 9
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM14_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM14_REMAP))
#else
#warning Illegal AF for Tim14
#endif

#elif defined (RCC_APB2ENR_TIM15EN) && defined(RCC_APB2RSTR_TIM15RST) && (STM32TIMER_ID == 15 )
#define STM32TIMER_BASE TIM15_BASE
#define STM32TIMER_SIG sig_TIM15
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM15EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM15RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#define STM32TIMER_NCH 2
#define STM32TIMER_BDTR
/* F3: PA1/CH1N/AF9, PA2/CH1/AF9, PA3/CH2/AF9 PA9/BKIN/AF9
   F30: PB14/CH1/AF1, PB15/CH2/AF1, PB15/CH1_N/AF2, PF9/CH1/AF3, PF10/CH2/AF3
   F37: PB4/CH1_N/AF9, PB6/CH1/AF9, PB7/CH1/AF9, PB14/CH1/AF1, PB15/CH2/AF1
   FIXME: This doesn't map TIM15 CH1N
*/
#if defined(STM32F30X)
#define STM32TIMER_AF(port, pin) (port == GPIOF_BASE)?  3 : (port == GPIOB_BASE)? 1 : 9
#endif
#if defined(STM32F37X)
#define STM32TIMER_AF(port, pin) ((port == GPIOB_BASE) && ((pin == 14) || (pin == 14)))? 1 : 9
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM15_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM15_REMAP))
#endif

#elif defined (RCC_APB2ENR_TIM16EN) && defined(RCC_APB2RSTR_TIM16RST) && (STM32TIMER_ID == 16 )
#define STM32TIMER_BASE TIM16_BASE
#define STM32TIMER_SIG sig_TIM16
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM16EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM16RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#define STM32TIMER_NCH 1
#define STM32TIMER_BDTR
#if defined(MCU_STM32F3)
#define STM32TIMER_AF(port, pin) (port == GPIOE_BASE)?  4 :  1
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM16_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM16_REMAP))
#endif

#elif defined (RCC_APB2ENR_TIM17EN) && defined(RCC_APB2RSTR_TIM17RST) && (STM32TIMER_ID == 17 )
#define STM32TIMER_BASE TIM17_BASE
#define STM32TIMER_SIG sig_TIM17
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM17EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM17RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#define STM32TIMER_NCH 1
#define STM32TIMER_BDTR
#if defined(STM32F30X)
#define STM32TIMER_AF(port, pin) (port == GPIOE_BASE)?           4 : \
    (  port == GPIOA_BASE)?                                      1 : \
    (( port == GPIOB_BASE) && ((pin ==  7) || (pin ==  9)))?     1 : \
    (( port == GPIOA_BASE) && ((pin ==  4) || (pin ==  5)))?    10 : 1
#elif defined(STM32F37X)
#define STM32TIMER_AF(port, pin) (  port == GPIOA_BASE)    ?     1 : \
    (( port == GPIOB_BASE) && ((pin ==  7) || (pin ==  9)))?     1 : \
    (( port == GPIOB_BASE) && ((pin ==  4) || (pin ==  5)))?    10 : 1
#elif defined(MCU_STM32F1)
#define STM32TIMER_REMAP_REG   MAPR2
#define STM32TIMER_REMAP_MASK  AFIO_MAPR2_TIM17_REMAP
#define STM32TIMER_REMAP_SHIFT (_BI32(AFIO_MAPR2_TIM17_REMAP))
#endif

#elif defined (RCC_APB2ENR_TIM18EN) && defined(RCC_APB2RSTR_TIM18RST) && (STM32TIMER_ID == 18 )
#define STM32TIMER_BASE TIM18_BASE
#define STM32TIMER_SIG sig_TIM18
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_TIM18EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB1RSTR,  _BI32(RCC_APB1RSTR_TIM18RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#define STM32TIMER_NCH 0

#elif defined (RCC_APB2ENR_TIM19EN) && defined(RCC_APB2RSTR_TIM19RST) && (STM32TIMER_ID == 19 )
#define STM32TIMER_BASE TIM19_BASE
#define STM32TIMER_SIG sig_TIM19
#define STM32TIMER_CLK() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR,  _BI32(RCC_APB2ENR_TIM19EN))
#define STM32TIMER_RST() CM3BBPULSE(RCC_BASE, RCC_TypeDef, APB2RSTR,  _BI32(RCC_APB2RSTR_TIM19RST))
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#define STM32TIMER_NCH 4
#define STM32TIMER_PCLK \
    ((CM3BBGET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PPRE2_2)))?  \
     (2 * NutClockGet(NUT_HWCLK_PCLK2)) : (NutClockGet(NUT_HWCLK_PCLK2)))
#define STM32TIMER_AF(port, pin) (((port == GPIOC_BASE) || (port == GPIOD_BASE))?     2 : 11)

#else
#warning No match
#endif

/* Only few timers are 32 bit. Use this "catch all else" to reduce duplication*/
#if !defined(STM32TIMER_WIDTH)
#define STM32TIMER_WIDTH 16
#endif
