#ifndef _OWI_STM32TIM_H_
#define _OWI_STM32TIM_H_
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

/*!
 * \file dev/stm32_owitimer.c
 * \brief Header for the One-Wire API Implementation for STM32 Timers
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */
#include <stdint.h>
#include <sys/types.h>

/*!
 * \brief OWI runtime controlblock container.
 *
 * This is installed in heap at initialization.
 */
struct _NUTOWIINFO_STM32TIM {
    HANDLE volatile owi_irq_done;     /*!< Primitive done event.*/
    IRQ_HANDLER *owi_ev;
    uint32_t timer;
#if defined(MCU_STM32F1)
    __IO uint16_t *compare;
    __IO uint16_t *capture;
#else
    __IO uint32_t *compare;
    __IO uint32_t *capture;
#endif
#if defined(MCU_STM32F3)
    __IO uint32_t *ccmr;
#else
    __IO uint16_t *ccmr;
#endif
    int ccmr_shift;
};

typedef struct _NUTOWIINFO_STM32TIM NUTOWIINFO_STM32TIM;

int NutRegisterOwiBus_STM32TIM(NUTOWIBUS *bus, int tx_port, uint_fast8_t tx_pin, uint32_t timer, uint32_t channel);

/*@}*/
#endif
