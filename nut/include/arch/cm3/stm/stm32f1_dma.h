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
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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
#define DMA1_C1   0
#define DMA1_C2   1
#define DMA1_C3   2
#define DMA1_C4   3
#define DMA1_C5   4
#define DMA1_C6   5
#define DMA1_C7   6
#define STM_HAS_DMA1 7

#if defined(STM32F10X_HD) || defined(STM32F10X_XL)
#define DMA2_C1   7
#define DMA2_C2   8
#define DMA2_C3   9
#define DMA2_C45 10
#define STM_HAS_DMA2 4
#elif defined(STM32F10X_CL)
#define DMA2_C1   7
#define DMA2_C2   8
#define DMA2_C3   9
#define DMA2_C4  10
#define DMA2_C5  11
#define STM_HAS_DMA2 5
#endif

/*!
 * \brief STM32 DMA Status and Interrupt Flags.
 */
#define DMA_TEIF 0x8  /*< Channel x Transfer Error Flag */
#define DMA_HTIF 0x4  /*< Channel x Half Transfer Complete Flag */
#define DMA_TCIF 0x2  /*< Channel x Transfer Complete Flag */
#define DMA_GIF  0x1  /*< Channel x Global Interrupt Flag */

#define DMA_FLAGMASK 0xF
#define DMA_IRQMASK  0x1

/* Internally used struct and table to align
 * DMA channels and interrupts. */
typedef struct {
    uint32_t dma;       /*< DMA Controller Register Base Address */
    uint32_t fofs;      /*< DMA Channel Flags Offset */
    DMA_Channel_TypeDef* dma_ch;    /*< DMA Channel Register Base Address */
} DMATAB;

extern const DMATAB DmaTab[];

/*
 * DMA Handles and Interrupt Entry Points
 */
extern HANDLE* dma1_args[];
extern void Dma1IrqEntry(void *arg);
#ifdef STM_HAS_DMA2
extern HANDLE* dma2_args[];
extern void Dma2IrqEntry(void *arg);
#endif

/*
 * DMA Control Functions
 */
void DMA_Setup( uint8_t ch, void* dst, void* src, uint16_t length, uint32_t flags);
void DMA_Enable(uint8_t ch);
void DMA_Disable(uint8_t ch);
void DMA_Init(void);
void DMA_IrqMask( uint8_t ch, uint32_t mask, uint8_t ena);
void DMA_ClearFlag( uint8_t ch, uint32_t flags);
uint32_t DMA_GetFlag( uint8_t ch);

#endif /* _STM32F1_DMA_H_ */
