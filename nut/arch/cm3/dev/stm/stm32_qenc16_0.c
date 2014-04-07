/*!
 * Copyright (C) 2014 Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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

/*!
 * \file arch/cm3/dev/stm/stm32_qenc16_0.c
 * \brief STM 32 bit quadrature encoder device 0 using 16-bit timer.
 *
 * Implement a 32 bit quadrature encode with an STM32 16-bit timer with at
 * least two channels. Quadrature pulses must be connected to Ch1/CH2.
 *
 * .._get() must be run before 1<<15 counts have elapsed, or otherwise
 * a revolution will be lost. No need to care if total range is
 * limited to < (1<<16) - 1. Some interrupt generated from encoder
 * signals, like timer overflow or index impulse may cause interrupt storms.
 * Using such interrupt as source for update is not recommended.
 *
 * Attention : Configured values for the used pins are not checked here!
 *
 * Typical usage:
 * \code
 *  #include <arch/cm3/stm/stm32_qenc16.h>
 *  stm32_qenc16_0_init();
 *  ...
 *  stm32_qenc16_0_update();
 *  ...
 *  value = stm32_qenc16_0_get();
 *  ...
 * \endcode
 *
 * ..set() may be used e.g. to set the counter value to some know value when
 * some limit switch has been hit.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <cfg/arch.h>
#include <arch/cm3/stm/stm32xxxx.h>
#include <cfg/qenc.h>
#include <sys/atom.h>
#include <dev/gpio.h>
#include <arch/cm3/stm/stm32_gpio.h>
#include <dev/hwtimer_stm32.h>

/* Only compile code if needed defines are given*/
#if defined(STM32_QENC16_0_I_PORT) && defined(STM32_QENC16_0_I_PIN) &&\
    defined(STM32_QENC16_0_Q_PORT) && defined(STM32_QENC16_0_Q_PIN) &&\
    defined(STM32_QENC16_0_TIMER_ID)

#if defined(STM32_QENC16_0_I_PORT)
#define STM32_QENC16_I_PORT STM32_QENC16_0_I_PORT
#endif

#if defined(STM32_QENC16_0_I_PIN)
#define STM32_QENC16_I_PIN STM32_QENC16_0_I_PIN
#endif

#if defined(STM32_QENC16_0_Q_PORT)
#define STM32_QENC16_Q_PORT STM32_QENC16_0_Q_PORT
#endif

#if defined(STM32_QENC16_0_Q_PIN)
#define STM32_QENC16_Q_PIN STM32_QENC16_0_Q_PIN
#endif

#if defined(STM32_QENC16_0_INVERT)
#define STM32_QENC16_INVERT STM32_QENC16_0_INVERT
#endif

#if defined(STM32_QENC16_0_TIMER_ID)
#undef STM32TIMER_ID
#define STM32TIMER_ID STM32_QENC16_0_TIMER_ID
#include <arch/cm3/stm/stm32timertran.h>
#define STM32_QENC16_BASE  STM32TIMER_BASE
#define STM32_QENC16_CLK() STM32TIMER_CLK()
#define STM32_QENC16_RST   STM32TIMER_RST
#define STM32_QENC16_I_AF  STM32TIMER_AF(STM32_QENC16_I_PORT, STM32_QENC16_I_PIN)
#define STM32_QENC16_Q_AF  STM32TIMER_AF(STM32_QENC16_Q_PORT, STM32_QENC16_Q_PIN)

static uint16_t stm32_qenc16_0_last_value;
static  int32_t stm32_qenc16_0_total_value;

#define QENC16_LAST_VALUE  stm32_qenc16_0_last_value
#define QENC16_TOTAL_VALUE stm32_qenc16_0_total_value

#endif

/*!
 * \brief Initialize the quadrature encoder
 *
 * No checking is done here!
 *
 * \param  None.
 *
 * \return None.
 */

void stm32_qenc16_0_init(void)
{
    STM32_QENC16_CLK();
    STM32_QENC16_RST = 1;
    STM32_QENC16_RST = 0;
    QENC16_LAST_VALUE = 0;

    /* AN4013.pdf*/
    /* Select Input T1->TI1FP1, T2->TI2FP2*/
    /* Input polarity TI1FP1 non-inverted, TI1FP1=TI1, TI1FP2 non-inverted, TI1FP2= TI2*/
    /* Register reset already done by reset */
    /* Encoder mode 3*/
#if defined(STM32_QENC16_INVERT)
    TIM_CCEnable(STM32_QENC16_BASE) = TIM_CCER_CC1P;
#endif
    TIM_SlaveModeControl(STM32_QENC16_BASE)=  TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0;
    TIM_AutoReloadValue(STM32_QENC16_BASE) =  (uint32_t) -1;
    TIM_EventGeneration(STM32_QENC16_BASE) = TIM_EGR_UG;

    GpioPinConfigSet(STM32_QENC16_I_PORT, STM32_QENC16_I_PIN,
                     GPIO_CFG_PULLDOWN | GPIO_CFG_PERIPHAL);
    GpioPinConfigSet(STM32_QENC16_Q_PORT, STM32_QENC16_Q_PIN,
                     GPIO_CFG_PULLDOWN | GPIO_CFG_PERIPHAL);
    GPIO_PinAFConfig((GPIO_TypeDef*)STM32_QENC16_I_PORT, STM32_QENC16_I_PIN,
                     STM32_QENC16_I_AF );
    GPIO_PinAFConfig((GPIO_TypeDef*)STM32_QENC16_Q_PORT, STM32_QENC16_Q_PIN,
                     STM32_QENC16_Q_AF );
    TIM_StartTimer(STM32_QENC16_BASE);
}

/*!
 * \brief Return quadrature encoder value
 *
 * \param  None.
 *
 * \return .
 */

int32_t stm32_qenc16_0_get(void)
{
    uint16_t actual_value =  TIM_Counter(STM32_QENC16_BASE);
    int16_t diff;

    NutEnterCritical();
    diff = actual_value - QENC16_LAST_VALUE;
    QENC16_LAST_VALUE = actual_value;
    QENC16_TOTAL_VALUE += diff;
    NutExitCritical();
    return  QENC16_TOTAL_VALUE;
}

/*!
 * \brief Set quadrature encoder value
 *
 * \param  Value to set.
 *
 * \return None.
 */
void stm32_qenc16_0_set(int32_t value)
{
    TIM_StopTimer(STM32_QENC16_BASE);
    TIM_Counter(STM32_QENC16_BASE) = 0;
    NutEnterCritical();
    QENC16_TOTAL_VALUE = value;
    NutExitCritical();
    TIM_Update(STM32_QENC16_BASE);
    TIM_StartTimer(STM32_QENC16_BASE);
}
#endif
