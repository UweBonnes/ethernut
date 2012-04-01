/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * Copyright (C) 2010 by Nikolaj Zamotaev. All rights reserved.
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
 
#include <cfg/os.h>
#include <cfg/clock.h>
#include <cfg/arch.h>
#include <cfg/arch/gpio.h>

#include <arch/cm3.h>
#if defined(MCU_STM32F1)
#include <arch/cm3/stm/stm32f10x.h>
#include <arch/cm3/stm/stm32f10x_rcc.h>
#include <arch/cm3/stm/stm32f10x_tim.h>
#else
#warning "Unknown STM32 family"
#endif

void TIM_Init( TIM_TypeDef *TIM )
{
  //enable clock
  switch( ( uint32_t ) TIM ) {
    case (uint32_t) TIM2:
      RCC->APB1ENR |= RCC_APB1Periph_TIM2; 
      break;
    case (uint32_t) TIM3:
      RCC->APB1ENR |= RCC_APB1Periph_TIM3; 
      break;
    case (uint32_t) TIM4:
      RCC->APB1ENR |= RCC_APB1Periph_TIM4; 
      break;
    case (uint32_t) TIM5:
      RCC->APB1ENR |= RCC_APB1Periph_TIM5; 
      break;
    case (uint32_t) TIM6:
      RCC->APB1ENR |= RCC_APB1Periph_TIM6; 
      break;
    case (uint32_t) TIM7:
      RCC->APB1ENR |= RCC_APB1Periph_TIM7; 
      break;
  }
}

void TIM_StartTimer( TIM_TypeDef *TIM )
{
  //enable counter
  sbi( &TIM->CR1, TIM_CR1_CEN );
}

void TIM_StopTimer( TIM_TypeDef *TIM )
{
  //disable counter
  cbi( &TIM->CR1, TIM_CR1_CEN );
}

void TIM_CounterSet( TIM_TypeDef *TIM, uint16_t value )
{
  *( (uint16_t *) &TIM->CNT ) = value;
}

void TIM_PrescalerSet( TIM_TypeDef *TIM, uint16_t scale )
{
  *( (uint16_t *) &TIM->PSC ) = scale;
}

void TIM_AutoReloadValueSet( TIM_TypeDef *TIM, uint16_t value )
{
  *( (uint16_t *) &TIM->ARR ) = value;
}

void TIM_IRQEnable( TIM_TypeDef *TIM )
{
  sbi( &TIM->DIER, TIM_DIER_UIE );
}

void TIM_IRQDisable( TIM_TypeDef *TIM )
{
  cbi( &TIM->DIER, TIM_DIER_UIE );
}

void TIM_ClearInterruptFlag( TIM_TypeDef *TIM )
{
  cbi( &TIM2->SR, TIM_SR_UIF );
}
