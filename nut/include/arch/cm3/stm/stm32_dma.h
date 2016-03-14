#ifndef _DEV_STM32_DMA_H
#define _DEV_STM32_DMA_H
/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * Copyright (C) 2010 by Nikolaj Zamotaev. All rights reserved.
 * Copyright (C) 2012-2013 Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
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

/*
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <cfg/arch.h>
#include <arch/cm3/stm/stm32_irqreg.h>

#if defined(DMA1_Channel1)
#include <arch/cm3/stm/stm32f1_dma.h>
#elif  defined( DMA1_Stream0)
#include <arch/cm3/stm/stm32f2_dma.h>
#else
#warning "STM32 family has no implemented DMA"
#endif

/*
 * DMA Handles and Interrupt Entry Points
 */
extern void Dma1IrqEntry(void *arg);
#ifdef STM_HAS_DMA2
extern void Dma2IrqEntry(void *arg);
#endif

/*
 * DMA Control Functions
 */
void DMA_Setup( uint8_t ch, void* dst, void* src, uint16_t length, uint32_t flags);
void DMA_Enable(uint8_t ch);
void DMA_Disable(uint8_t ch);
extern int  DmaIsEnabled(uint8_t ch);
void DMA_Init(void);
void DMA_IrqMask( uint8_t ch, uint32_t mask, uint8_t ena);
void DMA_ClearFlag( uint8_t ch, uint32_t flags);
uint32_t DMA_GetFlag( uint8_t ch);
uint16_t DMA_GetRemainingTransfers( uint8_t ch);
extern void *DmaGetMemoryBase( uint8_t ch);
extern DMA_SIGNAL *DmaCreateHandler(uint8_t ch);
extern int DmaRegisterHandler(
    DMA_SIGNAL* signal, uint8_t ch, void (*handler) (void *), void *arg);
extern int DmaEnableHandler(DMA_SIGNAL* signal, uint8_t ch);
extern int DmaDisableHandler(DMA_SIGNAL* signal, uint8_t ch);
#endif

