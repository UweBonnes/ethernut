/*
 * Copyright (C) 2015 by Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
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
 * \file dev/owibus1stm32tim.c
 * \brief Configuration of owiBus1Stm32Tim with dual channel STM32
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

#include <cfg/arch.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <dev/owibus.h>
#include <dev/gpio.h>
#include <arch/cm3/stm/stm32_gpio.h>
#include <arch/cm3/stm/stm32_timer.h>
#include <cfg/owi.h>

/* Compile code only when needed definitions are available */
#if !defined(STM32TIM_OWI1_TIMER_ID) && !defined(STM32TIM_OWI1_CHANNEL) &&\
    !defined(STM32TIM_OWI1_GPIO)
#elif defined(STM32TIM_OWI1_TIMER_ID) && defined(STM32TIM_OWI1_CHANNEL) &&\
    defined(STM32TIM_OWI1_GPIO)

static HANDLE STM32TIM_OWI1_MUTEX;
#define STM32TIM_OWI_MUTEX STM32TIM_OWI1_MUTEX

#if defined(STM32TIM_OWI1_GPIO)
#define STM32TIM_OWI_GPIO STM32TIM_OWI1_GPIO
#endif

#if defined(STM32TIM_OWI1_CHANNEL)
#define STM32TIM_OWI_CHANNEL STM32TIM_OWI1_CHANNEL
#endif

#if defined  (STM32TIM_OWI1_TIMER_ID)
#undef  STM32TIMER_ID
#define STM32TIMER_ID STM32TIM_OWI1_TIMER_ID
#include <arch/cm3/stm/stm32timertran.h>
#define STM32_OWITIMER_BASE STM32TIMER_BASE
#define STM32_OWITIMER_SIG  STM32TIMER_SIG
#define STM32_OWITIMER_INIT() STM32TIMER_INIT()
#if defined(MCU_STM32F1)
#define STM32_OWITIMER_REMAP_REG    STM32TIMER_REMAP_REG
#define STM32_OWITIMER_REMAP_MASK   STM32TIMER_REMAP_MASK
#define STM32_OWITIMER_REMAP_SHIFT  STM32TIMER_REMAP_SHIFT
#define STM32_OWITIMER_REMAP_VALUE  STM32TIMER_REMAP_VALUE
#endif
#define STM32_OWITIMER_AF  STM32TIMER_AF(STM32TIM_OWI_GPIO)
#endif

/* Forward declaration */
static int Stm32TimOwiSetup(NUTOWIBUS *bus);
static int Stm32TimOwiTouchReset(NUTOWIBUS *bus);
static int Stm32TimOwiReadBlock(NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len);
static int Stm32TimOwiWriteBlock(NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len);

/*!
 * \brief Library compile time configured OWI bus driver for dual-channel
 *        STM32 hardware timer.
 *
 */
 NUTOWIBUS owiBus1Stm32Tim = {
    0,                   /*!< \brief OWIBUSBUS::owibus_info */
    OWI_MODE_NORMAL,     /*!< \brief OWIBUSBUS::mode */
    Stm32TimOwiSetup,      /*!< \brief OWIBUSBUS::OwiSetup */
    Stm32TimOwiTouchReset, /*!< \brief OWIBUSBUS::OwiTouchReset*/
    Stm32TimOwiReadBlock,  /*!< \brief OWIBUSBUS::OwiReadBlock */
    Stm32TimOwiWriteBlock  /*!< \brief OWIBUSBUS::OwiWriteBlock */
 };
#include "owibus_stm32tim.c"
#else
/* Warn user if some defines are messing*/
#warning Some OWI defines missing
#endif

/*@}*/
