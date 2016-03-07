/*
 * Copyright (C) 2015, Uwe Bonnes bon@elektron.ikp.physik.tu-darmstadt.de
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

/*
 * XL STM32F1 and all STM32F0 and STM32L0 devices have some
 * combined DMA interrupts. By using DmaCreateHandler() and
 * DmaRegisterHandler() application must not care what interrupt
 * to use and how to separate from the connected signals
 *
 * If several DMA channels assert an IRQ, the handlers are called in
 * th order they where registered by DmaRegisterHandler(). So handlers
 * registered ealier have some higher priority.
 *
 * ToDo: Allow to deregister DMA Signalhandlers.
 */

#include <stdlib.h>

#include <cfg/arch.h>
#include <cfg/devices.h>
#include <arch/cm3.h>
#include <dev/irqreg.h>

#include <arch/cm3/stm/stm32xxxx.h>
#include <arch/cm3/stm/stm32_dma.h>

#if defined(MCU_STM32F1) || defined(MCU_STM32F3) ||  defined(MCU_STM32L1) ||defined(MCU_STM32L4)
# define FIRST_COMBINED_CHANNEL DMA2_CH5
static IRQ_HANDLER *ch2irq[DMA_COUNT] = {
    &sig_DMA1_CH1,
    &sig_DMA1_CH2,
    &sig_DMA1_CH3,
    &sig_DMA1_CH4,
    &sig_DMA1_CH5,
    &sig_DMA1_CH6,
    &sig_DMA1_CH7,
# if defined(HW_DMA2_STM32F1)
    &sig_DMA2_CH1,
    &sig_DMA2_CH2,
    &sig_DMA2_CH3,
    &sig_DMA2_CH4,
#  if defined(HW_DMA_COMBINED_IRQ_STM32)
    &sig_DMA2_CH4,
#  else
    &sig_DMA2_CH5,
#  endif
#  if defined(HW_DMA2_7CH_STM32)
    &sig_DMA2_CH6,
    &sig_DMA2_CH7,
#  endif
# endif
};
#elif defined(MCU_STM32F0) || defined(MCU_STM32L0)
# define FIRST_COMBINED_CHANNEL DMA1_CH2
# if   defined(HW_DMA1_5CH_STM32)
#  define DMA_COUNT  5
# elif defined(HW_DMA2_STM32F1)
#  define DMA_COUNT 12
# else
#  define DMA_COUNT  7
# endif
static IRQ_HANDLER *ch2irq[DMA_COUNT] = {
    &sig_DMA1_CH1,
    &sig_DMA_GROUP1,
    &sig_DMA_GROUP1,
    &sig_DMA_GROUP2,
    &sig_DMA_GROUP2,
# if !defined(HW_DMA1_5CH_STM32)
    &sig_DMA_GROUP2,
    &sig_DMA_GROUP2,
# endif
# if defined(HW_DMA2_STM32F1)
    &sig_DMA_GROUP1,
    &sig_DMA_GROUP1,
    &sig_DMA_GROUP2,
    &sig_DMA_GROUP2,
    &sig_DMA_GROUP2,
# endif
};
# if defined(SYSCFG_ITLINE10_SR_DMA1_CH2)
static __IO uint32_t *ch2it_line_sr[DMA_COUNT] = {
    &SYSCFG->IT_LINE_SR[ 9],
    &SYSCFG->IT_LINE_SR[10],
    &SYSCFG->IT_LINE_SR[10],
    &SYSCFG->IT_LINE_SR[11],
    &SYSCFG->IT_LINE_SR[11],
    &SYSCFG->IT_LINE_SR[11],
    &SYSCFG->IT_LINE_SR[11],
    &SYSCFG->IT_LINE_SR[10],
    &SYSCFG->IT_LINE_SR[10],
    &SYSCFG->IT_LINE_SR[11],
    &SYSCFG->IT_LINE_SR[11],
    &SYSCFG->IT_LINE_SR[11],
};
static uint32_t ch2it_line_mask[DMA_COUNT] = {
    SYSCFG_ITLINE9_SR_DMA1_CH1,
    SYSCFG_ITLINE10_SR_DMA1_CH2,
    SYSCFG_ITLINE10_SR_DMA1_CH3,
    SYSCFG_ITLINE11_SR_DMA1_CH4,
    SYSCFG_ITLINE11_SR_DMA1_CH5,
    SYSCFG_ITLINE11_SR_DMA1_CH6,
    SYSCFG_ITLINE11_SR_DMA1_CH7,
    SYSCFG_ITLINE10_SR_DMA2_CH1,
    SYSCFG_ITLINE10_SR_DMA2_CH2,
    SYSCFG_ITLINE11_SR_DMA2_CH3,
    SYSCFG_ITLINE11_SR_DMA2_CH4,
    SYSCFG_ITLINE11_SR_DMA2_CH5,
};
# endif
#else
static IRQ_HANDLER *dma2irq[2][8] = {
    {
        &sig_DMA1_STREAM0,
        &sig_DMA1_STREAM1,
        &sig_DMA1_STREAM2,
        &sig_DMA1_STREAM3,
        &sig_DMA1_STREAM4,
        &sig_DMA1_STREAM5,
        &sig_DMA1_STREAM6,
        &sig_DMA1_STREAM7
    },
    {
        &sig_DMA2_STREAM0,
        &sig_DMA2_STREAM1,
        &sig_DMA2_STREAM2,
        &sig_DMA2_STREAM3,
        &sig_DMA2_STREAM4,
        &sig_DMA2_STREAM5,
        &sig_DMA2_STREAM6,
        &sig_DMA2_STREAM7
    }
};
#endif

#if defined(HW_DMA_COMBINED_IRQ_STM32)
static void DmaGroupHandler(void *arg) {
    DMA_SIGNAL *signal;
    for (signal = (DMA_SIGNAL *) arg; signal; signal = signal->next) {
        DMA_TypeDef *dma;
        int ch;
        uint32_t flag;

        ch = signal->ch;
# if defined(MCU_STM32F1)
        dma = DMA2;
# elif defined(MCU_STM32F0) || defined(MCU_STM32L0)
#  if defined(DMA2)
        if (ch < 7) {
            dma = DMA1;
        } else {
            dma = DMA2;
            ch = ch - 7;
        }
#  else
        dma = DMA1;
#  endif
# endif
# if defined(SYSCFG_ITLINE10_SR_DMA1_CH2)
        flag = *signal->it_line_sr & signal->it_line_mask;
# else
        flag = dma->ISR & (1 << (ch << 2));
# endif
        if (flag) {
            if (signal->dma_channel_handler) {
                (signal->dma_channel_handler)(signal->dma_channel_arg);
                /* IRQ flag handling must be done by user */
            } else {
                /* Dangling interrupt, reset flags */
                dma->IFCR = 0xf << (ch << 2);
            }
        }
    }
}

DMA_SIGNAL *DmaCreateHandler(uint8_t ch)
{
    IRQ_HANDLER *irq;
    DMA_SIGNAL *sig, *chain;

    if (ch >= DMA_COUNT) { /* No such DMA*/
        return NULL;
    }
    if (ch < FIRST_COMBINED_CHANNEL) {
        irq = ch2irq[ch];
        NutIrqEnable(irq);
        return (DMA_SIGNAL*)irq;
    }
    irq = ch2irq[ch];
    if (irq->ir_arg) {
        /* Interrupt already installed, check chain*/
        chain = irq->ir_arg;
        while(1) {
            if (chain->ch == ch) {
                /* Slot allocated */
                return chain;
            }
            if (!chain->next) {
                /* slot empty */
                break;
            }
            chain = chain->next;
        }
    } else {
        chain = NULL;
    }
    sig = malloc(sizeof(DMA_SIGNAL));
    if (sig) {
# if defined(SYSCFG_ITLINE10_SR_DMA1_CH2)
/* Get SYSCFG IL_LINE_SR going */
        sig->it_line_sr = ch2it_line_sr[ch];
        sig->it_line_mask = ch2it_line_mask[ch];
# endif
        sig->ch = ch;
        sig->dma_channel_handler = NULL;
        sig->next = NULL;
        if (chain) {
            chain->next = sig;
        } else {
            int res;
            res = NutRegisterIrqHandler(irq, DmaGroupHandler, sig);
            if (res) {
                /* We failed to install the chain handler */
                free(sig);
                sig = NULL;
            }
            NutIrqEnable(irq);
        }
    }
    return sig;
}

int DmaRegisterHandler(
    DMA_SIGNAL *sig, uint8_t ch, void (*handler) (void *), void *arg)
{
    IRQ_HANDLER *irq = NULL;
    DMA_SIGNAL *chain;

    if (ch >= DMA_COUNT) { /* No such DMA*/
        return 0;
    }
    irq = ch2irq[ch];
    if (ch < FIRST_COMBINED_CHANNEL) {
        int res;
        res = NutRegisterIrqHandler(irq, handler, arg);
        return res;
    }
    for (chain = irq->ir_arg; chain->next; chain = chain->next) {
        if (chain->ch == ch) {
            break;
        }
    }
    if (!chain) {
        return -1;
    }
    /* We found our slot */
    if (handler) { /* Install new handler */
        if (chain->dma_channel_handler) {
            /* But it is already occupied */
            return -1;
        } else {
            /* save the new handler and args*/
            chain->dma_channel_handler = handler;
            chain->dma_channel_arg = arg;
        }
    } else {
        chain->dma_channel_handler = NULL;
        chain->dma_channel_arg = NULL;
    }
    return 0;
}

#else
# if defined(MCU_STM32F2) || defined(MCU_STM32F4) ||defined(MCU_STM32F7)
extern DMA_SIGNAL *DmaCreateHandler(uint8_t ch)
{
    uint8_t dma, stream;
    IRQ_HANDLER *irq;

    dma = ch >> 7;
    stream = ch & 0x7f;

    if (dma > 1) {
        /* No such DMA*/
        return NULL;
    }
    if (stream > 7) {
        /* No such stream*/
        return NULL;
    }
    irq = dma2irq[dma][stream];
    NutIrqEnable(irq);
    return irq;
}

# else
extern DMA_SIGNAL *DmaCreateHandler(uint8_t ch)
{
    IRQ_HANDLER *irq;

    if (ch >= DMA_COUNT) {
        /* No such DMA*/
        return NULL;
    }
    irq = ch2irq[ch];
    NutIrqEnable(irq);
    return irq;
}
# endif

int DmaRegisterHandler(
    DMA_SIGNAL *sig, uint8_t ch, void (*handler) (void *), void *arg)
{
    int res;
    res = NutRegisterIrqHandler(sig, handler, arg);
    return res;
}
#endif
