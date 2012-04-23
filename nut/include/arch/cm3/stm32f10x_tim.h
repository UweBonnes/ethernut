/**
  ******************************************************************************
  * @file    stm32f10x_tim.h
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   This file contains all the functions prototypes for the TIM firmware
  *          library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_TIM_H
#define __STM32F10x_TIM_H

#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


typedef struct
{
  __IO uint32_t CR1;
  __IO uint32_t CR2;
  __IO uint32_t SMCR;
  __IO uint32_t DIER;
  __IO uint32_t SR;
  __IO uint32_t EGR;
  __IO uint32_t CCMR1;
  __IO uint32_t CCMR2;
  __IO uint32_t CCER;
  __IO uint32_t CNT;
  __IO uint32_t PSC;
  __IO uint32_t ARR;
  __IO uint32_t RESERVED1;
  __IO uint32_t CCR1;
  __IO uint32_t CCR2;
  __IO uint32_t CCR3;
  __IO uint32_t CCR4;
  __IO uint32_t RESERVED2;
  __IO uint32_t DCR;
  __IO uint32_t DMAR;
} TIM_TypeDef;

/* CEN: Counter enable */
#define TIM_CR1_CEN                     0

/* UIE: Update interrupt enable */
#define TIM_DIER_UIE                    0

/* UIF: Update interrupt flag */
#define TIM_SR_UIF                      0

#define TIM_BASE            (STM32_APB1PERIPH_BASE)
#define TIM2_BASE           (TIM_BASE + 0x0000)
#define TIM3_BASE           (TIM_BASE + 0x0400)
#define TIM4_BASE           (TIM_BASE + 0x0800)
#define TIM5_BASE           (TIM_BASE + 0x0C00)
#define TIM6_BASE           (TIM_BASE + 0x1000)
#define TIM7_BASE           (TIM_BASE + 0x1400)

#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM2                ((TIM_TypeDef *) TIM2_BASE)

#ifdef __cplusplus
}
#endif

void TIM_Init( TIM_TypeDef *TIM );
void TIM_PrescalerSet( TIM_TypeDef *TIM, uint16_t scale );
void TIM_AutoReloadValueSet( TIM_TypeDef *TIM, uint16_t value );
void TIM_IRQEnable( TIM_TypeDef *TIM );
void TIM_IRQDisable( TIM_TypeDef *TIM );
void TIM_StartTimer( TIM_TypeDef *TIM );
void TIM_StopTimer( TIM_TypeDef *TIM );
void TIM_ClearInterruptFlag( TIM_TypeDef *TIM );
void TIM_CounterSet( TIM_TypeDef *TIM, uint16_t value );


#endif /* __STM32F10x_TIM_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
