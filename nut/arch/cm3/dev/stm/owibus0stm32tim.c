/*
 * Copyright (C) 2013 by Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
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

#include <cfg/arch.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <dev/owibus.h>
#include <dev/gpio.h>
#include <arch/cm3/stm/stm32_owitimer.h>
#include <arch/cm3/stm/stm32_gpio.h>
#include <dev/hwtimer_stm32.h>
#include <cfg/owi.h>

/*!
 * \file dev/owibus0stm32tim.c
 * \brief Configuration of owiBus0Stm32Tim with dual channel STM32
 *        timer, configured at library compile time.
 *
 * The OWI Bus needs to be connected of CH1|2 or CH3|4 of some timer. The
 * base pin is set to opendrain, and CNT and CCR1|2 or CCR3|4 are used to
 * generate the pulse timing and capture the rising edge.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

/* Compile code only when needed definitions are available */
#if defined(STM32TIM_OWI0_TIMER_ID) && defined(STM32TIM_OWI0_PORT) && defined(STM32TIM_OWI0_PIN)\
    && defined(STM32TIM_OWI0_CHANNEL)

static HANDLE STM32TIM_OWI0_MUTEX;
#define STM32TIM_OWI_MUTEX STM32TIM_OWI0_MUTEX

#if defined(STM32TIM_OWI0_PORT)
#define STM32TIM_OWI_PORT STM32TIM_OWI0_PORT
#endif

#if defined(STM32TIM_OWI0_PIN)
#define STM32TIM_OWI_PIN STM32TIM_OWI0_PIN
#endif

#if defined(STM32TIM_OWI0_CHANNEL)
#define STM32TIM_OWI_CHANNEL STM32TIM_OWI0_CHANNEL
#endif

#if defined(STM32TIM_OWI0_REMAP)
#define STM32TIM_OWI_REMAP STM32TIM_OWI0_REMAP
#endif

#if defined  (STM32TIM_OWI0_TIMER_ID)
#undef  STM32TIMER_ID
#define STM32TIMER_ID STM32TIM_OWI0_TIMER_ID
#include <arch/cm3/stm/stm32timertran.h>
#define STM32_OWITIMER_BASE STM32TIMER_BASE
#define STM32_OWITIMER_SIG  STM32TIMER_SIG
#define STM32_OWITIMER_CLK() STM32TIMER_CLK()
#define STM32_OWITIMER_RST() STM32TIMER_RST()
#define STM32_OWITIMER_PCLK() STM32TIMER_PCLK()
#define STM32_OWITIMER_NCH  STM32TIMER_NCH
#if defined(MCU_STM32F1)
#define STM32_OWITIMER_REMAP_MASK   STM32TIMER_REMAP_MASK
#define STM32_OWITIMER_REMAP_SHIFT  STM32TIMER_REMAP_SHIFT
#else
#define STM32_OWITIMER_AF  STM32TIMER_AF(STM32TIM_OWI_PORT, STM32TIM_OWI0_PIN)
#endif
#endif

/* Forward declaration */
static int Stm32Tim_OwiSetup(NUTOWIBUS *bus);
static int Stm32Tim_OwiTouchReset(NUTOWIBUS *bus);
static int Stm32Tim_OwiReadBlock(NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len);
static int Stm32Tim_OwiWriteBlock(NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len);

/*!
 * \brief Library compile time configured OWI bus driver for dual-channel
 *        STM32 hardware timer.
 *
 */
 NUTOWIBUS owiBus0Stm32Tim = {
    0,                   /*!< \brief OWIBUSBUS::owibus_info */
    OWI_MODE_NORMAL,     /*!< \brief OWIBUSBUS::mode */
    Stm32Tim_OwiSetup,      /*!< \brief OWIBUSBUS::OwiSetup */
    Stm32Tim_OwiTouchReset, /*!< \brief OWIBUSBUS::OwiTouchReset*/
    Stm32Tim_OwiReadBlock,  /*!< \brief OWIBUSBUS::OwiReadBlock */
    Stm32Tim_OwiWriteBlock  /*!< \brief OWIBUSBUS::OwiWriteBlock */
 };
#include "owibus_stm32tim.c"
}
#endif

/*@}*/
