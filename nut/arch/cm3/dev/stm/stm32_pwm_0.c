/*!
 * Copyright (C) 2013 Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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
 * $Id$
 */
#include <stdint.h>
#include <cfg/arch.h>
#include <arch/cm3/stm/stm32xxxx.h>
#include <cfg/pwm.h>
#include <dev/gpio.h>
#include <arch/cm3/stm/stm32_gpio.h>
#include <sys/timer.h>
#include <dev/hwtimer_stm32.h>

/* Compile code only when needed definitions are available */
#if defined(STM32_PWM0_TIMER_ID) && defined(STM32_PWM0_PORT) && defined(STM32_PWM0_PIN)\
    && defined(STM32_PWM0_TIMER_CHANNEL)

#if defined(STM32_PWM0_TIMER_ID)
#define STM32_PWM_TIMER_ID STM32_PWM0_TIMER_ID
#endif

#if defined(STM32_PWM0_TIMER_CHANNEL)
#define STM32_PWM_TIMER_CHANNEL STM32_PWM0_TIMER_CHANNEL
#endif

#if defined(STM32_PWM0_PORT)
#define STM32_PWM_PORT STM32_PWM0_PORT
#endif

#if defined(STM32_PWM0_PIN)
#define STM32_PWM_PIN STM32_PWM0_PIN
#endif

#if defined(STM32_PWM_TIMER_ID)
#undef STM32TIMER_ID
#define STM32TIMER_ID STM32_PWM_TIMER_ID
#include <arch/cm3/stm/stm32timertran.h>
#define STM32_PWM_BASE  STM32TIMER_BASE
#define STM32_PWM_PCLK  STM32TIMER_PCLK
#define STM32_PWM_CLK() STM32TIMER_CLK()
#define STM32_PWM_RST() STM32TIMER_RST()
#define STM32_PWM_NCH   STM32TIMER_NCH
#define STM32_PWM_BTDR  STM32TIMER_BTDR
#define STM32_PWM_AF    STM32TIMER_AF(STM32_PWM_PORT, STM32_PWM_PIN)
#endif

/* Use PWM Mode 1 (0x110 on OCxM), Only reload on update event (OCxPE set) */
#if defined(STM32_PWM_TIMER_CHANNEL)
#if STM32_PWM_TIMER_CHANNEL == 1
#define STM32_PWM_OUTPUT_EN() CM3BBSET(STM32_PWM_BASE, TIM_TypeDef, CCER ,  _BI32(TIM_CCER_CC1E))
#define STM32_PWM_MODE()      TIM_CCMode1(STM32_PWM_BASE) = TIM_CCMR1_OC1M_2|TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1PE
#define STM32_PWM_VALUE       TIM_Compare1(STM32_PWM_BASE)
#elif STM32_PWM_TIMER_CHANNEL == 2
#define STM32_PWM_OUTPUT_EN() CM3BBSET(STM32_PWM_BASE, TIM_TypeDef, CCER ,  _BI32(TIM_CCER_CC2E))
#define STM32_PWM_MODE()      TIM_CCMode1(STM32_PWM_BASE) = TIM_CCMR1_OC2M_2|TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2PE
#define STM32_PWM_VALUE       TIM_Compare2(STM32_PWM_BASE)
#elif STM32_PWM_TIMER_CHANNEL == 3
#define STM32_PWM_OUTPUT_EN() CM3BBSET(STM32_PWM_BASE, TIM_TypeDef, CCER ,  _BI32(TIM_CCER_CC3E))
#define STM32_PWM_MODE()      TIM_CCMode2(STM32_PWM_BASE) = TIM_CCMR2_OC3M_2|TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3PE
#define STM32_PWM_VALUE       TIM_Compare3(STM32_PWM_BASE)
#elif STM32_PWM_TIMER_CHANNEL == 4
#define STM32_PWM_OUTPUT_EN() CM3BBSET(STM32_PWM_BASE, TIM_TypeDef, CCER ,  _BI32(TIM_CCER_CC4E))
#define STM32_PWM_MODE()      TIM_CCMode2(STM32_PWM_BASE) = TIM_CCMR2_OC4M_2|TIM_CCMR2_OC4M_1|TIM_CCMR2_OC4PE
#define STM32_PWM_VALUE       TIM_Compare4(STM32_PWM_BASE)
#else
#warning Unknown PWM Timer Channel
#endif
#endif

/*!
 * \brief Initialize a PWM output
 *
 * Only few checks is done here!
 *
 * \param  freq Pointer to requested frequency in Hz, on return the achieved PWM frequency.
 *              Function fails is achievable frequency is of 25 % of requested frequency.
 * \param  bits Bits of the PWM set value.
 *
 * \return 0 on success, -1 else.
 */

int stm32_pwm0_init(uint32_t *freq, int bits)
{
#if (STM32_PWM_TIMER_CHANNEL == 0) || (STM32_PWM_TIMER_CHANNEL > STM32_PWM_NCH)
    /* We requested a channel not available */
    return -1;
#else
/* Try to find prescaler value that gives requested frequency with 25 % tolerance */
    uint32_t pclk = STM32_PWM_PCLK;
    uint32_t psc0 = ((pclk <<1)/ (*freq)) >> bits;
    uint32_t psc1 = (pclk / (*freq)) >> bits;

    if (psc0 & 1) psc1++; /* Round up*/
    *freq = pclk/psc1 >> bits;
    if (( 5 * (*freq << bits)  < 4 * pclk) || ( 5 * pclk  < 4 * (*freq << bits)))
        return -1;
    STM32_PWM_CLK();
    STM32_PWM_RST();
    /* All registers now reset*/
    STM32_PWM_OUTPUT_EN();
    STM32_PWM_MODE();
    TIM_AutoReloadValue(STM32_PWM_BASE) = (1<<bits) -1;
    STM32_PWM_VALUE = 0;
#if defined(STM32_PWM_BTDR)
    TIM_Break_Deadtime(STM32_PWM_BASE) = TIM_BDTR_MOE;
#endif
    TIM_AutoReload(STM32_PWM_BASE);
    TIM_Update(STM32_PWM_BASE);
    GpioPinConfigSet(STM32_PWM_PORT, STM32_PWM_PIN, GPIO_CFG_PERIPHAL);
    GPIO_PinAFConfig((GPIO_TypeDef*)STM32_PWM_PORT, STM32_PWM_PIN, STM32_PWM_AF);
    TIM_StartTimer(STM32_PWM_BASE);
    return 0;
#endif
}

/*!
 * \brief Set PWM value
 *
 *
 * \param  value PWM value to set.
 *
 * \return None.
 */

void stm32_pwm0_set(uint32_t value)
{
    if (value > TIM_AutoReloadValue(STM32_PWM_BASE))
        /* Clip at maximum*/
        STM32_PWM_VALUE = TIM_AutoReloadValue(STM32_PWM_BASE);
    else
        STM32_PWM_VALUE =  value;
}

/*!
 * \brief Return current set PWM value
 *
 *
 * \return Set PWM value.
 */
uint32_t stm32_pwm0_get(void)
{
    return STM32_PWM_VALUE;
}

#endif
