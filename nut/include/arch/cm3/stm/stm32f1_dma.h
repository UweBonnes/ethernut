#ifndef _STM32F1_DMA_H_
#define _STM32F1_DMA_H_
/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
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

/*!
 * \verbatim
 * $Id: ih_stm32_usart1.c 3131 2010-09-22 21:47:23Z Astralix $
 * \endverbatim
 */

/*!
 * \brief DMA Channel Naming
 *
 * These defines keep Nut/OS internal channel numbering
 * aligned with the STM32 documentation RM0008.
 */
#include <cfg/devices.h>

#define DMA1_CH1   0
#define DMA1_CH2   1
#define DMA1_CH3   2
#define DMA1_CH4   3
#define DMA1_CH5   4
#define DMA1_CH6   5
#define DMA1_CH7   6
#define STM_HAS_DMA1 7

#define DMA2_CH1   7
#define DMA2_CH2   8
#define DMA2_CH3   9
#define DMA2_CH4  10
/* FIXME: Some F1 devices have C4 and C5 coupled*/
#define DMA2_CH5  11
#if defined(HW_DMA2_STM32F1)
#define STM_HAS_DMA2 5
#endif

#define ADC1_DMA_DMA                   DMA1_CH1
#define ADC1_DMA_DMA_SIG               sig_DMA1_CH1
#define TIM2_CH3_DMA                   DMA1_CH1
#define TIM2_CH3_DMA_IRQ               sig_DMA1_CH1
#define TIM4_CH1_DMA                   DMA1_CH1
#define TIM4_CH1_DMA_IRQ               sig_DMA1_CH1
#define TIM17_CH1_UP_DMA               DMA1_CH1
#define TIM17_CH1_UP_DMA_IRQ           sig_DMA1_CH1
#define TIM19_CH3_CH4_DMA              DMA1_CH1
#define TIM19_CH3_CH4_DMA_IRQ          sig_DMA1_CH1

#define SPI1_RX_DMA                    DMA1_CH2
#define SPI1_RX_DMA_IRQ                sig_DMA1_CH2
#define USART3_TX_DMA                  DMA1_CH2
#define USART3_TX_DMA_IRQ              sig_DMA1_CH2
#define TIM2_UP_DMA                    DMA1_CH2
#define TIM2_UP_DMA_IRQ                sig_DMA1_CH2
#define TIM3_CH3_DMA                   DMA1_CH2
#define TIM3_CH3_DMA_IRQ               sig_DMA1_CH2
#define TIM19_CH1_DMA                  DMA1_CH2
#define TIM19_CH1_DMA_IRQ              sig_DMA1_CH2

#define SPI1_TX_DMA                    DMA1_CH3
#define SPI1_TX_DMA_IRQ                sig_DMA1_CH3
#define USART3_RX_DMA                  DMA1_CH3
#define USART3_RX_DMA_IRQ              sig_DMA1_CH3
#define TIM3_CH4_UP_DMA                DMA1_CH3
#define TIM3_CH4_UP_DMA_IRQ            sig_DMA1_CH3
#define TIM6_UP_DAC1_CH1_DMA           DMA1_CH3
#define TIM6_UP_DAC1_CH1_DMA_IRQ       sig_DMA1_CH3
#define TIM15_CH1_UP_DMA               DMA1_CH3
#define TIM15_CH1_UP_DMA_IRQ           sig_DMA1_CH3
#define TIM19_CH2_DMA                  DMA1_CH3
#define TIM19_CH2_DMA_IRQ              sig_DMA1_CH3

#define SPI2_RX_DMA                    DMA1_CH4
#define SPI2_RX_DMA_IRQ                sig_DMA1_CH4
#define USART1_TX_DMA                  DMA1_CH4
#define USART1_TX_DMA_IRQ              sig_DMA1_CH4
#define TIM4_CH2_DMA                   DMA1_CH4
#define TIM4_CH2_DMA_IRQ               sig_DMA1_CH4
#define TIM7_UP_DAC1_CH2_DMA           DMA1_CH4
#define TIM7_UP_DAC1_CH2_DMA_IRQ       sig_DMA1_CH4
#define TIM19_UP_DMA                   DMA1_CH4
#define TIM19_UP_DMA_IRQ               sig_DMA1_CH4

#define SPI2_TX_DMA                    DMA1_CH5
#define SPI2_TX_DMA_IRQ                sig_DMA1_CH5
#define USART1_RX_DMA                  DMA1_CH5
#define USART1_RX_DMA_IRQ              sig_DMA1_CH5
#define TIM1_CH1_DMA                   DMA1_CH5
#define TIM1_CH1_DMA_IRQ               sig_DMA1_CH5
#define TIM4_CH3_DMA                   DMA1_CH5
#define TIM4_CH3_DMA_IRQ               sig_DMA1_CH5
#define TIM18_UP_DAC2_CH1_DMA          DMA1_CH5
#define TIM18_UP_DAC2_CH1_DMA_IRQ      sig_DMA1_CH5
#define TIM15_CH1_UP_TRIG_COM_DMA      DMA1_CH5
#define TIM15_CH1_UP_TRIG_COM_DMA_IRQ  sig_DMA1_CH5

#define USART2_RX_DMA                  DMA1_CH6
#define USART2_RX_DMA_IRQ              sig_DMA1_CH6
#define I2C1_TX_DMA                    DMA1_CH6
#define I2C1_TX_DMA_IRQ                sig_DMA1_CH6
#define TIM3_CH1_TRIG_DMA              DMA1_CH6
#define TIM3_CH1_TRIG_DMA_IRQ          sig_DMA1_CH6
#define TIM16_CH1_UP_DMA               DMA1_CH6
#define TIM16_CH1_UP_DMA_IRQ           sig_DMA1_CH6

#define USART2_TX_DMA                  DMA1_CH7
#define USART2_TX_DMA_IRQ              sig_DMA1_CH7
#define I2C1_RX_DMA                    DMA1_CH7
#define I2C1_RX_DMA_IRQ                sig_DMA1_CH7
#define TIM2_CH2_CH4_DMA               DMA1_CH7
#define TIM2_CH2_CH4_DMA_IRQ           sig_DMA1_CH7
#define TIM4_UP_DMA                    DMA1_CH7
#define TIM4_UP_DMA_IRQ                sig_DMA1_CH7
#define TIM17_CH1_UP_ALT_DMA           DMA1_CH7
#define TIM17_CH1_UP_ALT_DMA_IRQ       sig_DMA1_CH7

#define SPI3_RX_DMA                    DMA2_CH1
#define SPI3_RX_DMA_IRQ                sig_DMA2_CH1
#define TIM5_CH4_TRIG_DMA              DMA2_CH1
#define TIM5_CH4_TRIG_DMA_IRQ          sig_DMA2_CH1

#define SPI3_TX_DMA                    DMA2_CH2
#define SPI3_TX_DMA_IRQ                sig_DMA2_CH2
#define TIM5_CH3_UP_DMA                DMA2_CH2
#define TIM5_CH3_UP_DMA_IRQ            sig_DMA2_CH2

#define SDADC1_DMA                     DMA2_CH3
#define SDADC1_DMA_IRQ                 sig_DMA2_CH3
#define TIM6_UP_DAC1_CH1_ALT_DMA       DMA2_CH3
#define TIM6_UP_DAC1_CH1_ALT_DMA_IRQ   sig_DMA2_CH3

#define SDADC2_DMA                     DMA2_CH4
#define SDADC2_DMA_IRQ                 sig_DMA2_CH4
#define TIM5_CH2_DMA                   DMA2_CH4
#define TIM5_CH2_DMA_IRQ               sig_DMA2_CH4
#define TIM7_UP_DAC1_CH2_ALT_DMA       DMA2_CH4
#define TIM7_UP_DAC1_CH2_ALT_DMA_IRQ   sig_DMA2_CH4

#define SDADC3_DMA                     DMA2_CH5
#define SDADC3_DMA_IRQ                 sig_DMA2_CH5
#define TIM5_CH1                       DMA2_CH5
#define TIM5_CH1_IRQ                   sig_DMA2_CH5
#define TIM18_UP_DAC2_CH1_ALT_DMA      DMA2_CH5
#define TIM18_UP_DAC2_CH1_ALT_DMA_IRQ  sig_DMA2_CH5

/* RM0008 defines the bits for CCR1, CCR2 etc. only.
 * Equalize the names by using the general names from STM32F3*/
#if !defined(DMA_CCR_EN) && defined(DMA_CCR1_EN)
#define DMA_CCR_EN DMA_CCR1_EN
#endif
#if !defined(DMA_CCR_TCIE) && defined(DMA_CCR1_TCIE)
#define DMA_CCR_TCIE DMA_CCR1_TCIE
#endif
#if !defined(DMA_CCR_HTIE) && defined(DMA_CCR1_HTIE)
#define DMA_CCR_HTIE DMA_CCR1_HTIE
#endif
#if !defined(DMA_CCR_TEIE) && defined(DMA_CCR1_TEIE)
#define DMA_CCR_TEIE DMA_CCR1_TEIE
#endif
#if !defined(DMA_CCR_DIR) && defined(DMA_CCR1_DIR)
#define DMA_CCR_DIR DMA_CCR1_DIR
#endif
#if !defined(DMA_CCR_CIRC) && defined(DMA_CCR1_CIRC)
#define DMA_CCR_CIRC DMA_CCR1_CIRC
#endif
#if !defined(DMA_CCR_PINC) && defined(DMA_CCR1_PINC)
#define DMA_CCR_PINC DMA_CCR1_PINC
#endif
#if !defined(DMA_CCR_MINC) && defined(DMA_CCR1_MINC)
#define DMA_CCR_MINC DMA_CCR1_MINC
#endif
#if !defined(DMA_CCR_PSIZE) && defined(DMA_CCR1_PSIZE)
#define DMA_CCR_PSIZE DMA_CCR1_PSIZE
#endif
#if !defined(DMA_CCR_PSIZE_0) && defined(DMA_CCR1_PSIZE_0)
#define DMA_CCR_PSIZE_0 DMA_CCR1_PSIZE_0
#endif
#if !defined(DMA_CCR_PSIZE_1) && defined(DMA_CCR1_PSIZE_1)
#define DMA_CCR_PSIZE_1 DMA_CCR1_PSIZE_1
#endif
#if !defined(DMA_CCR_MSIZE) && defined(DMA_CCR1_MSIZE)
#define DMA_CCR_MSIZE DMA_CCR1_MSIZE
#endif
#if !defined(DMA_CCR_MSIZE_0) && defined(DMA_CCR1_MSIZE_0)
#define DMA_CCR_MSIZE_0 DMA_CCR1_MSIZE_0
#endif
#if !defined(DMA_CCR_MSIZE_1) && defined(DMA_CCR1_MSIZE_1)
#define DMA_CCR_MSIZE_1 DMA_CCR1_MSIZE_1
#endif
#if !defined(DMA_CCR_PL) && defined(DMA_CCR1_PL)
#define DMA_CCR_PL DMA_CCR1_PL
#endif
#if !defined(DMA_CCR_PL_0) && defined(DMA_CCR1_PL_0)
#define DMA_CCR_PL_0 DMA_CCR1_PL_0
#endif
#if !defined(DMA_CCR_PL_1) && defined(DMA_CCR1_PL_1)
#define DMA_CCR_PL_1 DMA_CCR1_PL_1
#endif
#if !defined(DMA_CCR_MEM2MEM) && defined(DMA_CCR1_MEM2MEM)
#define DMA_CCR_MEM2MEM DMA_CCR1_MEM2MEM
#endif

#define  DMA_MINC DMA_CCR_MINC
#define  DMA_CIRC DMA_CCR_CIRC

/* Internally used struct and table to align
 * DMA channels and interrupts. */
typedef struct {
    uint32_t dma;       /*< DMA Controller Register Base Address */
    uint32_t fofs;      /*< DMA Channel Flags Offset */
    DMA_Channel_TypeDef* dma_ch;    /*< DMA Channel Register Base Address */
} DMATAB;

extern const DMATAB DmaTab[];

#endif /* _STM32F1_DMA_H_ */
