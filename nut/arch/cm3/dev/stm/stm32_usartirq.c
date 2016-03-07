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

/* ToDo: Allow to deregister DMA Signalhandlers.
 */

#include <stdlib.h>

#include <cfg/arch.h>
#include <cfg/uart.h>
#include <cfg/devices.h>
#include <arch/cm3.h>
#include <dev/irqreg.h>

#include <arch/cm3/stm/stm32xxxx.h>
#include <arch/cm3/stm/stm32_usartirq.h>

#if defined(MCU_STM32F0) || defined(MCU_STM32L0)
# if defined(HW_USART8_STM32)
#  define NUM_USART 8
# elif defined(HW_USART6_STM32)
#  define NUM_USART 6
# elif defined(HW_USART4_STM32)
#  define NUM_USART 4
# elif defined(HW_USART2_STM32)
#  define NUM_USART 2
# else
#  define NUM_USART 1
# endif

static uint32_t usart_nr2base[NUM_USART + 1] = {
    USART1_BASE,
# if defined(HW_USART2_STM32)
    USART2_BASE,
# endif
# if defined(HW_USART4_STM32)
    USART3_BASE,
    USART4_BASE,
# endif
# if defined(HW_USART6_STM32)
    USART5_BASE,
    USART6_BASE,
# endif
# if defined(HW_USART8_STM32)
    USART7_BASE,
    USART8_BASE,
# endif
    0
};
static IRQ_HANDLER *usart_nr2irq[] = {
    &sig_USART1,
# if defined(HW_USART2_STM32)
    &sig_USART2,
# endif
};
#else
/* F1-F4 */
static uint32_t usart_nr2base[] = {
    USART1_BASE,
# if defined(HW_USART2_STM32)
    USART2_BASE,
# endif
# if defined(HW_USART3_STM32)
    USART3_BASE,
# endif
# if defined(HW_UART4_STM32)
    UART4_BASE,
# endif
# if defined(HW_UART5_STM32)
    UART5_BASE,
# endif
# if defined(HW_USART6_STM32)
    USART6_BASE,
# endif
# if defined(HW_UART7_STM32)
    UART7_BASE,
# endif
# if defined(HW_UART8_STM32)
    UART8_BASE,
# endif
};
static IRQ_HANDLER *usart_nr2irq[] = {
    &sig_USART1,
# if defined(HW_USART2_STM32)
    &sig_USART2,
# endif
# if defined(HW_USART3_STM32)
    &sig_USART3,
# endif
# if defined(HW_UART4_STM32)
    &sig_UART4,
# endif
# if defined(HW_UART5_STM32)
    &sig_UART5,
# endif
# if defined(HW_USART6_STM32)
    &sig_USART6,
# endif
# if defined(HW_UART7_STM32)
    &sig_UART7,
# endif
# if defined(HW_UART8_STM32)
    &sig_UART8,
# endif
};
# define NUM_USART sizeof(usart_nr2irq)/sizeof(usart_nr2irq[0])
#endif

# if defined(HW_USART_COMBINED_IRQ_STM32)

static void Stm32UsartGroupHandler(void *arg) {
    USART_SIGNAL *signal;
    for (signal = (USART_SIGNAL *) arg; signal; signal = signal->next) {
        int usart_nr = signal->usart_nr;;
        uint32_t flags;
# if defined(SYSCFG_ITLINE29_SR_USART3_GLB)
/* Only F09x devices have 8 Usarts and have the IT_LINE */
        flags = SYSCFG->IT_LINE_SR[29] & (1 << (usart_nr - 3));
#else
        USART_TypeDef * usart;
        usart = (USART_TypeDef*)usart_nr2base[usart_nr];
        flags = usart->ISR;
        flags &= (USART_ISR_WUF  | USART_ISR_CMF   | USART_ISR_EOBF |
                  USART_ISR_RTOF | USART_ISR_CTSIF | USART_ISR_LBDF |
                  USART_ISR_TXE  | USART_ISR_TC    | USART_ISR_RXNE |
                  USART_ISR_IDLE | USART_ISR_ORE   | USART_ISR_FE   |
                  USART_ISR_PE);
        /* Fixme: Should we check here if interrupt is enabled? */
#endif
        if (flags) {
            if (signal->usart_handler) {
                (signal->usart_handler)(signal->usart_arg);
                /* IRQ flag handling must be done by user */
            } else {
                USART_TypeDef* usart;
                usart = (USART_TypeDef*)usart_nr2base[usart_nr];
                /* ToDo: Clear dangling Interrupts */
                (void)usart;
                /* Dangling interrupt, reset flags */
            }
        }
    }
}

USART_SIGNAL *Stm32UsartCreateHandler(uint32_t usart_base)
{
    IRQ_HANDLER *irq;
    USART_SIGNAL *sig, *chain;
    int usart_nr;

    for (usart_nr = 0; usart_nr2base[usart_nr];usart_nr++)
        if (usart_base == usart_nr2base[usart_nr]) {
            break;
        }
    if (usart_nr >= NUM_USART) { /* No such USART*/
        return NULL;
    }
    if (usart_nr < 2) {
        irq = usart_nr2irq[usart_nr];
        NutIrqEnable(irq);
        return (USART_SIGNAL*)irq;
    }
    irq = &sig_USART_GROUP;
    if (irq->ir_arg) {
        /* Interrupt already installed, check chain*/
        chain = irq->ir_arg;
        while (1) {
            if (chain->usart_nr == usart_nr) {
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
    sig = malloc(sizeof(USART_SIGNAL));
    if (sig) {
        sig->usart_nr = usart_nr;
        sig->usart_handler = NULL;
        sig->next = NULL;
        if (chain) {
            chain->next = sig;
        } else {
            int res;
            res = NutRegisterIrqHandler(irq, Stm32UsartGroupHandler, sig);
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

int Stm32UsartRegisterHandler(
    USART_SIGNAL *sig, uint32_t usart_base,
    void (*handler) (void *), void *arg)
{
    IRQ_HANDLER *irq = NULL;
    USART_SIGNAL *chain;
    int usart_nr;

    for (usart_nr = 0; usart_nr2base[usart_nr];usart_nr++) {
        if (usart_base == usart_nr2base[usart_nr]) {
            break;
        }
    }
    if (usart_nr >= NUM_USART) { /* No such USART*/
        return 1;
    }
    irq = usart_nr2irq[usart_nr];
    if (usart_nr < 2) {
        int res;
        res = NutRegisterIrqHandler(irq, handler, arg);
        return res;
    }
    for (chain = irq->ir_arg; chain->next; chain = chain->next) {
        if (chain->usart_nr == usart_nr) {
            break;
        }
    }
    if (!chain) {
        return -1;
    }
    /* We found our slot */
    if (handler) { /* Install new handler */
        if (chain->usart_handler) {
            /* But it is already occupied */
            return 0;
        } else {
            /* save the new handler and args*/
            chain->usart_handler = handler;
            chain->usart_arg = arg;
        }
    } else {
        chain->usart_handler = NULL;
        chain->usart_arg = NULL;
    }
    return 0;
}

#else


USART_SIGNAL *Stm32UsartCreateHandler(uint32_t usart_base)
{
    IRQ_HANDLER *irq;
    int usart_nr;

    for (usart_nr = 0; usart_nr2base[usart_nr];usart_nr++) {
        if (usart_base == usart_nr2base[usart_nr]) {
            break;
        }
    }
    if (usart_nr >= NUM_USART) { /* No such USART*/
        return NULL;
    }
    irq = usart_nr2irq[usart_nr];
    NutIrqEnable(irq);
    return (USART_SIGNAL*)irq;
}

int Stm32UsartRegisterHandler(
    USART_SIGNAL *sig, uint32_t usart_base,
    void (*handler) (void *), void *arg)
{
    return NutRegisterIrqHandler(sig, handler, arg);
}

#endif
