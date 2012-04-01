/**
  ******************************************************************************
  * @file    stm32f10x_dma.h
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   This file contains all the functions prototypes for the DMA firmware 
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
#ifndef __STM32F10x_DMA_H
#define __STM32F10x_DMA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <sys/event.h>
#include <sys/types.h>

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup DMA
  * @{
  */

/** @defgroup DMA_data_transfer_direction 
  * @{
  */

#define DMA_DIR_PeripheralDST              ((uint32_t)0x00000010)
#define DMA_DIR_PeripheralSRC              ((uint32_t)0x00000000)
/**
  * @}
  */

/** @defgroup DMA_peripheral_incremented_mode 
  * @{
  */

#define DMA_PeripheralInc_Enable           ((uint32_t)0x00000040)
#define DMA_PeripheralInc_Disable          ((uint32_t)0x00000000)
/**
  * @}
  */

/** @defgroup DMA_memory_incremented_mode 
  * @{
  */

#define DMA_MemoryInc_Enable               ((uint32_t)0x00000080)
#define DMA_MemoryInc_Disable              ((uint32_t)0x00000000)
/**
  * @}
  */

/** @defgroup DMA_peripheral_data_size 
  * @{
  */

#define DMA_PeripheralDataSize_Byte        ((uint32_t)0x00000000)
#define DMA_PeripheralDataSize_HalfWord    ((uint32_t)0x00000100)
#define DMA_PeripheralDataSize_Word        ((uint32_t)0x00000200)
/**
  * @}
  */

/** @defgroup DMA_memory_data_size 
  * @{
  */

#define DMA_MemoryDataSize_Byte            ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord        ((uint32_t)0x00000400)
#define DMA_MemoryDataSize_Word            ((uint32_t)0x00000800)
/**
  * @}
  */

/** @defgroup DMA_circular_normal_mode 
  * @{
  */

#define DMA_Mode_Circular                  ((uint32_t)0x00000020)
#define DMA_Mode_Normal                    ((uint32_t)0x00000000)
/**
  * @}
  */

/** @defgroup DMA_priority_level 
  * @{
  */

#define DMA_Priority_VeryHigh              ((uint32_t)0x00003000)
#define DMA_Priority_High                  ((uint32_t)0x00002000)
#define DMA_Priority_Medium                ((uint32_t)0x00001000)
#define DMA_Priority_Low                   ((uint32_t)0x00000000)
/**
  * @}
  */

/** @defgroup DMA_memory_to_memory 
  * @{
  */

#define DMA_M2M_Enable                     ((uint32_t)0x00004000)
#define DMA_M2M_Disable                    ((uint32_t)0x00000000)


#define DMA_Enable_Flag			   ((uint32_t)0x00000001)
/**
  * @}
  */

/** @defgroup DMA_interrupts_definition 
  * @{
  */

#define DMA_IT_TC                          ((uint32_t)0x00000002)
#define DMA_IT_HT                          ((uint32_t)0x00000004)
#define DMA_IT_TE                          ((uint32_t)0x00000008)

#define DMA1_IT_GL1                        ((uint32_t)0x00000001)
#define DMA1_IT_TC1                        ((uint32_t)0x00000002)
#define DMA1_IT_HT1                        ((uint32_t)0x00000004)
#define DMA1_IT_TE1                        ((uint32_t)0x00000008)
#define DMA1_IT_GL2                        ((uint32_t)0x00000010)
#define DMA1_IT_TC2                        ((uint32_t)0x00000020)
#define DMA1_IT_HT2                        ((uint32_t)0x00000040)
#define DMA1_IT_TE2                        ((uint32_t)0x00000080)
#define DMA1_IT_GL3                        ((uint32_t)0x00000100)
#define DMA1_IT_TC3                        ((uint32_t)0x00000200)
#define DMA1_IT_HT3                        ((uint32_t)0x00000400)
#define DMA1_IT_TE3                        ((uint32_t)0x00000800)
#define DMA1_IT_GL4                        ((uint32_t)0x00001000)
#define DMA1_IT_TC4                        ((uint32_t)0x00002000)
#define DMA1_IT_HT4                        ((uint32_t)0x00004000)
#define DMA1_IT_TE4                        ((uint32_t)0x00008000)
#define DMA1_IT_GL5                        ((uint32_t)0x00010000)
#define DMA1_IT_TC5                        ((uint32_t)0x00020000)
#define DMA1_IT_HT5                        ((uint32_t)0x00040000)
#define DMA1_IT_TE5                        ((uint32_t)0x00080000)
#define DMA1_IT_GL6                        ((uint32_t)0x00100000)
#define DMA1_IT_TC6                        ((uint32_t)0x00200000)
#define DMA1_IT_HT6                        ((uint32_t)0x00400000)
#define DMA1_IT_TE6                        ((uint32_t)0x00800000)
#define DMA1_IT_GL7                        ((uint32_t)0x01000000)
#define DMA1_IT_TC7                        ((uint32_t)0x02000000)
#define DMA1_IT_HT7                        ((uint32_t)0x04000000)
#define DMA1_IT_TE7                        ((uint32_t)0x08000000)

#define DMA2_IT_GL1                        ((uint32_t)0x10000001)
#define DMA2_IT_TC1                        ((uint32_t)0x10000002)
#define DMA2_IT_HT1                        ((uint32_t)0x10000004)
#define DMA2_IT_TE1                        ((uint32_t)0x10000008)
#define DMA2_IT_GL2                        ((uint32_t)0x10000010)
#define DMA2_IT_TC2                        ((uint32_t)0x10000020)
#define DMA2_IT_HT2                        ((uint32_t)0x10000040)
#define DMA2_IT_TE2                        ((uint32_t)0x10000080)
#define DMA2_IT_GL3                        ((uint32_t)0x10000100)
#define DMA2_IT_TC3                        ((uint32_t)0x10000200)
#define DMA2_IT_HT3                        ((uint32_t)0x10000400)
#define DMA2_IT_TE3                        ((uint32_t)0x10000800)
#define DMA2_IT_GL4                        ((uint32_t)0x10001000)
#define DMA2_IT_TC4                        ((uint32_t)0x10002000)
#define DMA2_IT_HT4                        ((uint32_t)0x10004000)
#define DMA2_IT_TE4                        ((uint32_t)0x10008000)
#define DMA2_IT_GL5                        ((uint32_t)0x10010000)
#define DMA2_IT_TC5                        ((uint32_t)0x10020000)
#define DMA2_IT_HT5                        ((uint32_t)0x10040000)
#define DMA2_IT_TE5                        ((uint32_t)0x10080000)

/**
  * @}
  */

/** @defgroup DMA_flags_definition 
  * @{
  */
#define DMA_FLAG_GL(x)		((uint32_t)(1<<(x*4)))
#define DMA_FLAG_TC(x)		((uint32_t)(1<<(x*4+1)))
#define DMA_FLAG_HT(x)		((uint32_t)(1<<(x*4+2)))
#define DMA_FLAG_TE(x)		((uint32_t)(1<<(x*4+3)))


#define DMA1_FLAG_GL1                      ((uint32_t)0x00000001)
#define DMA1_FLAG_TC1                      ((uint32_t)0x00000002)
#define DMA1_FLAG_HT1                      ((uint32_t)0x00000004)
#define DMA1_FLAG_TE1                      ((uint32_t)0x00000008)
#define DMA1_FLAG_GL2                      ((uint32_t)0x00000010)
#define DMA1_FLAG_TC2                      ((uint32_t)0x00000020)
#define DMA1_FLAG_HT2                      ((uint32_t)0x00000040)
#define DMA1_FLAG_TE2                      ((uint32_t)0x00000080)
#define DMA1_FLAG_GL3                      ((uint32_t)0x00000100)
#define DMA1_FLAG_TC3                      ((uint32_t)0x00000200)
#define DMA1_FLAG_HT3                      ((uint32_t)0x00000400)
#define DMA1_FLAG_TE3                      ((uint32_t)0x00000800)
#define DMA1_FLAG_GL4                      ((uint32_t)0x00001000)
#define DMA1_FLAG_TC4                      ((uint32_t)0x00002000)
#define DMA1_FLAG_HT4                      ((uint32_t)0x00004000)
#define DMA1_FLAG_TE4                      ((uint32_t)0x00008000)
#define DMA1_FLAG_GL5                      ((uint32_t)0x00010000)
#define DMA1_FLAG_TC5                      ((uint32_t)0x00020000)
#define DMA1_FLAG_HT5                      ((uint32_t)0x00040000)
#define DMA1_FLAG_TE5                      ((uint32_t)0x00080000)
#define DMA1_FLAG_GL6                      ((uint32_t)0x00100000)
#define DMA1_FLAG_TC6                      ((uint32_t)0x00200000)
#define DMA1_FLAG_HT6                      ((uint32_t)0x00400000)
#define DMA1_FLAG_TE6                      ((uint32_t)0x00800000)
#define DMA1_FLAG_GL7                      ((uint32_t)0x01000000)
#define DMA1_FLAG_TC7                      ((uint32_t)0x02000000)
#define DMA1_FLAG_HT7                      ((uint32_t)0x04000000)
#define DMA1_FLAG_TE7                      ((uint32_t)0x08000000)

#define DMA2_FLAG_GL1                      ((uint32_t)0x10000001)
#define DMA2_FLAG_TC1                      ((uint32_t)0x10000002)
#define DMA2_FLAG_HT1                      ((uint32_t)0x10000004)
#define DMA2_FLAG_TE1                      ((uint32_t)0x10000008)
#define DMA2_FLAG_GL2                      ((uint32_t)0x10000010)
#define DMA2_FLAG_TC2                      ((uint32_t)0x10000020)
#define DMA2_FLAG_HT2                      ((uint32_t)0x10000040)
#define DMA2_FLAG_TE2                      ((uint32_t)0x10000080)
#define DMA2_FLAG_GL3                      ((uint32_t)0x10000100)
#define DMA2_FLAG_TC3                      ((uint32_t)0x10000200)
#define DMA2_FLAG_HT3                      ((uint32_t)0x10000400)
#define DMA2_FLAG_TE3                      ((uint32_t)0x10000800)
#define DMA2_FLAG_GL4                      ((uint32_t)0x10001000)
#define DMA2_FLAG_TC4                      ((uint32_t)0x10002000)
#define DMA2_FLAG_HT4                      ((uint32_t)0x10004000)
#define DMA2_FLAG_TE4                      ((uint32_t)0x10008000)
#define DMA2_FLAG_GL5                      ((uint32_t)0x10010000)
#define DMA2_FLAG_TC5                      ((uint32_t)0x10020000)
#define DMA2_FLAG_HT5                      ((uint32_t)0x10040000)
#define DMA2_FLAG_TE5                      ((uint32_t)0x10080000)

#define IS_DMA_CLEAR_FLAG(FLAG) (((((FLAG) & 0xF0000000) == 0x00) || (((FLAG) & 0xEFF00000) == 0x00)) && ((FLAG) != 0x00))

#define IS_DMA_GET_FLAG(FLAG) (((FLAG) == DMA1_FLAG_GL1) || ((FLAG) == DMA1_FLAG_TC1) || \
                               ((FLAG) == DMA1_FLAG_HT1) || ((FLAG) == DMA1_FLAG_TE1) || \
                               ((FLAG) == DMA1_FLAG_GL2) || ((FLAG) == DMA1_FLAG_TC2) || \
                               ((FLAG) == DMA1_FLAG_HT2) || ((FLAG) == DMA1_FLAG_TE2) || \
                               ((FLAG) == DMA1_FLAG_GL3) || ((FLAG) == DMA1_FLAG_TC3) || \
                               ((FLAG) == DMA1_FLAG_HT3) || ((FLAG) == DMA1_FLAG_TE3) || \
                               ((FLAG) == DMA1_FLAG_GL4) || ((FLAG) == DMA1_FLAG_TC4) || \
                               ((FLAG) == DMA1_FLAG_HT4) || ((FLAG) == DMA1_FLAG_TE4) || \
                               ((FLAG) == DMA1_FLAG_GL5) || ((FLAG) == DMA1_FLAG_TC5) || \
                               ((FLAG) == DMA1_FLAG_HT5) || ((FLAG) == DMA1_FLAG_TE5) || \
                               ((FLAG) == DMA1_FLAG_GL6) || ((FLAG) == DMA1_FLAG_TC6) || \
                               ((FLAG) == DMA1_FLAG_HT6) || ((FLAG) == DMA1_FLAG_TE6) || \
                               ((FLAG) == DMA1_FLAG_GL7) || ((FLAG) == DMA1_FLAG_TC7) || \
                               ((FLAG) == DMA1_FLAG_HT7) || ((FLAG) == DMA1_FLAG_TE7) || \
                               ((FLAG) == DMA2_FLAG_GL1) || ((FLAG) == DMA2_FLAG_TC1) || \
                               ((FLAG) == DMA2_FLAG_HT1) || ((FLAG) == DMA2_FLAG_TE1) || \
                               ((FLAG) == DMA2_FLAG_GL2) || ((FLAG) == DMA2_FLAG_TC2) || \
                               ((FLAG) == DMA2_FLAG_HT2) || ((FLAG) == DMA2_FLAG_TE2) || \
                               ((FLAG) == DMA2_FLAG_GL3) || ((FLAG) == DMA2_FLAG_TC3) || \
                               ((FLAG) == DMA2_FLAG_HT3) || ((FLAG) == DMA2_FLAG_TE3) || \
                               ((FLAG) == DMA2_FLAG_GL4) || ((FLAG) == DMA2_FLAG_TC4) || \
                               ((FLAG) == DMA2_FLAG_HT4) || ((FLAG) == DMA2_FLAG_TE4) || \
                               ((FLAG) == DMA2_FLAG_GL5) || ((FLAG) == DMA2_FLAG_TC5) || \
                               ((FLAG) == DMA2_FLAG_HT5) || ((FLAG) == DMA2_FLAG_TE5))
/**
  * @}
  */

/** @defgroup DMA_Buffer_Size 
  * @{
  */

#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x1) && ((SIZE) < 0x10000))

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup DMA_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Exported_Functions
  * @{
  */

/**
  * @}
  */
#define DMA1_BASE             (STM32_AHBPERIPH_BASE + 0x0000)
#define DMA1_Channel1_BASE    (STM32_AHBPERIPH_BASE + 0x0008)
#define DMA1_Channel2_BASE    (STM32_AHBPERIPH_BASE + 0x001C)
#define DMA1_Channel3_BASE    (STM32_AHBPERIPH_BASE + 0x0030)
#define DMA1_Channel4_BASE    (STM32_AHBPERIPH_BASE + 0x0044)
#define DMA1_Channel5_BASE    (STM32_AHBPERIPH_BASE + 0x0058)
#define DMA1_Channel6_BASE    (STM32_AHBPERIPH_BASE + 0x006C)
#define DMA1_Channel7_BASE    (STM32_AHBPERIPH_BASE + 0x0080)
#define DMA2_BASE             (STM32_AHBPERIPH_BASE + 0x0400)
#define DMA2_Channel1_BASE    (STM32_AHBPERIPH_BASE + 0x0408)
#define DMA2_Channel2_BASE    (STM32_AHBPERIPH_BASE + 0x041C)
#define DMA2_Channel3_BASE    (STM32_AHBPERIPH_BASE + 0x0430)
#define DMA2_Channel4_BASE    (STM32_AHBPERIPH_BASE + 0x0444)
#define DMA2_Channel5_BASE    (STM32_AHBPERIPH_BASE + 0x0458)


typedef struct
{
  __IO uint32_t CCR;
  __IO uint32_t CNDTR;
  __IO uint32_t CPAR;
  __IO uint32_t CMAR;
} DMA_Channel_TypeDef;

typedef struct
{
  __IO uint32_t ISR;
  __IO uint32_t IFCR;
} DMA_TypeDef;

#define DMA1                ((DMA_TypeDef *) DMA1_BASE)
#define DMA2                ((DMA_TypeDef *) DMA2_BASE)
#define DMA1_Channel1       ((DMA_Channel_TypeDef *) DMA1_Channel1_BASE)
#define DMA1_Channel2       ((DMA_Channel_TypeDef *) DMA1_Channel2_BASE)
#define DMA1_Channel3       ((DMA_Channel_TypeDef *) DMA1_Channel3_BASE)
#define DMA1_Channel4       ((DMA_Channel_TypeDef *) DMA1_Channel4_BASE)
#define DMA1_Channel5       ((DMA_Channel_TypeDef *) DMA1_Channel5_BASE)
#define DMA1_Channel6       ((DMA_Channel_TypeDef *) DMA1_Channel6_BASE)
#define DMA1_Channel7       ((DMA_Channel_TypeDef *) DMA1_Channel7_BASE)
#define DMA2_Channel1       ((DMA_Channel_TypeDef *) DMA2_Channel1_BASE)
#define DMA2_Channel2       ((DMA_Channel_TypeDef *) DMA2_Channel2_BASE)
#define DMA2_Channel3       ((DMA_Channel_TypeDef *) DMA2_Channel3_BASE)
#define DMA2_Channel4       ((DMA_Channel_TypeDef *) DMA2_Channel4_BASE)
#define DMA2_Channel5       ((DMA_Channel_TypeDef *) DMA2_Channel5_BASE)


/* DMA2 FLAG mask */
#define DMA2_FLAG_Mask                ((uint32_t)0x10000000)


void DMA_Setup(DMA_Channel_TypeDef* channel,void* mem,void* periph,uint16_t count,uint32_t flags);
void DMA_Enable(DMA_Channel_TypeDef* chan);
void DMA_Disable(DMA_Channel_TypeDef* chan);
void DMA_Register_Interrupt(DMA_Channel_TypeDef* channel,HANDLE* handle);
void DMA_Init(void);
void DMA_ClearFlag(uint32_t DMA_FLAG);//FIXME: rewrite

#endif /*__STM32F10x_DMA_H */
/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
