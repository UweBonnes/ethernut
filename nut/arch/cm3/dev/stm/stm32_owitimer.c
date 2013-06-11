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

#include <cfg/arch.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <dev/owibus.h>
#include <dev/gpio.h>
#include <arch/cm3/stm/stm32_owitimer.h>
#include <arch/cm3/stm/stm32_gpio.h>
#include <dev/hwtimer_stm32.h>

static void STM32TIM_OwiInterrupt(void *arg)
{
    NUTOWIINFO_STM32TIM *owcb = arg;
    TIM_TypeDef *owi_timer =  (TIM_TypeDef *) (owcb->timer);

    if (owi_timer->SR & TIM_SR_UIF) {
        owi_timer->SR &= ~TIM_SR_UIF; /* Reset Flag*/
        NutEventPostFromIrq(&owcb->owi_irq_done);
    }
}

/*!
 * \brief Perform One-Wire transaction.
 *
 * \param bus     Specifies the One-Wire bus.
 * \param command Either OWI_CMD_RESET or OWI_CMD_RWBIT.
 * \param value   The value to send.
 *
 * \return The value read on success, a negative value otherwise.
 */
static int STM32TIM_OwiTransaction(NUTOWIBUS *bus, int_fast8_t command, int_fast8_t value)
{
    NUTOWIINFO_STM32TIM *owcb = (NUTOWIINFO_STM32TIM *) (bus->owibus_info);
    TIM_TypeDef *owi_timer =  (TIM_TypeDef*)owcb->timer;
    uint16_t ccmr = *owcb->ccmr;

    if (value)
        *owcb->compare = -(owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RELEASE]
                           - owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_SYNC_PULSE]);
    else
        *owcb->compare = -(owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RELEASE]
                           - owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_SYNC_PULSE_LOW]);

    owi_timer->CNT = -(owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RELEASE]
                       - owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_SETUP]);
    TIM_IRQEnable(owcb->timer);
    NutEnterCritical();
    /* Force active high */
    ccmr &= ~( 7 <<(owcb->ccmr_shift));
    ccmr |=  ( 5 <<(owcb->ccmr_shift));
    *owcb->ccmr = ccmr;
    TIM_StartTimer(owcb->timer);
    /* Toggle on compare match */
    ccmr &= ~( 7 <<(owcb->ccmr_shift));
    ccmr |=  ( 3 <<(owcb->ccmr_shift));
    *owcb->ccmr = ccmr;
    NutExitCritical();
    if(NutEventWait(&owcb->owi_irq_done, 10)) {
        if (command == OWI_CMD_RESET)
            return OWI_PRESENCE_ERR;
        else
            return OWI_DATA_ERROR;
    }
    owi_timer->SR = 0;
    TIM_IRQDisable(owcb->timer);
    /* We need to handle 32 and 16 bit counters. Store the value to compare
       in owcb->capture, so the same ting happens to the top word*/
    *owcb->compare = -( owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RELEASE]
                       - owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RW]);
    if (*owcb->capture > *owcb->compare)
        return 0;
    else
        return 1;
}

/*!
 * \brief Reset the One-Wire bus and check if device(s) present.
 *
 * \param bus Specifies the One-Wire bus.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
static int STM32TIM_OwiTouchReset(NUTOWIBUS *bus)
{
    return STM32TIM_OwiTransaction(bus, OWI_CMD_RESET, 1);
}

/*!
 * \brief Exchange one bit on the One-Wire bus.
 *
 * \param bus Specifies the One-Wire bus.
 * \param bit Value for the bit to send.
 *
 * \return The bus state at the read slot on success, a negative value
 *         otherwise.
 */
static int OwiRWBit(NUTOWIBUS *bus, uint_fast8_t bit)
{
    return STM32TIM_OwiTransaction(bus, OWI_CMD_RWBIT, bit);
}

/*!
 * \brief Write a block of data bits to the One-Wire bus.
 *
 * \param bus  Specifies the One-Wire bus.
 * \param data Data bits to send.
 * \param len  Number of bits to send.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
static int STM32TIM_OwiWriteBlock(
    NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len)
{
    int res;
    int i;

    for (i = 0; i < len; i++) {
        res = OwiRWBit(bus, data[i >> 3] & (1 << (i & 0x7)));
        if (res < 0)
            return OWI_HW_ERROR;
    }
    return OWI_SUCCESS;
}

/*!
 * \brief Read a block of data bits from the One-Wire bus.
 *
 * \param bus  Specifies the One-Wire bus.
 * \param data Data bits received.
 * \param len  Number of bits to read.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
static int STM32TIM_OwiReadBlock(
    NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len)
{
    int res;
    int i;

    memset(data, 0, (len >> 3) + 1);
    for (i = 0; i < len; i++) {
        res = OwiRWBit(bus, 1);
        if (res < 0)
            return OWI_HW_ERROR;
        data[i >> 3] |= (res << (i & 0x7));
    }
    return OWI_SUCCESS;
}

/*!
 * \brief Register the One-Wire bus.
 *
 * \param bus         The returned NUTOWIBUS.
 * \param txrx_port   The port to use for the One_Wire bus.
 * \param txrx_pin    The pin to use for the One_Wire bus.
 * \param timer       The timer to use. Needs to have at least two CC channels.
 * \param channel     The timer CC channel to use for output. Other connected
 *                    channel is used for capture.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
int NutRegisterOwiBus_STM32TIM(
    NUTOWIBUS *bus, int txrx_port, uint_fast8_t txrx_pin,
    uint32_t timer, uint32_t channel)
{
    NUTOWIINFO_STM32TIM *owcb;
    uint8_t gpio_af;
    TIM_TypeDef *owi_timer = (TIM_TypeDef*)timer;

    if (!timer || !(channel) || (channel >4))
        return OWI_INVALID_HW;
    switch (timer) {
/* Availability and function for the timers from AN4013 Rev2 */
#if defined(NUTTIMER1)
    case NUTTIMER1:
        gpio_af = GPIO_AF_TIM1;
        break;
#endif
#if defined(NUTTIMER2)
    case NUTTIMER2:
        gpio_af = GPIO_AF_TIM2;
        break;
#endif
    case NUTTIMER3:
        gpio_af = GPIO_AF_TIM3;
        break;
    case NUTTIMER4:
        gpio_af = GPIO_AF_TIM4;
        break;
#if defined(NUTTIMER5)
    case NUTTIMER5:
        gpio_af = GPIO_AF_TIM5;
        break;
#endif
#if defined(NUTTIMER9)
    case NUTTIMER9:
        if (channel > 2)
            return OWI_INVALID_HW;
        gpio_af = GPIO_AF_TIM9;
        break;
#endif
#if defined(NUTTIMER12)
    case NUTTIMER12:
        if (channel > 2)
            return OWI_INVALID_HW;
        gpio_af = GPIO_AF_TIM12;
        break;
#endif
    default:
        return OWI_INVALID_HW;
    }
    owcb = calloc(1, sizeof(*owcb));
    if (owcb == NULL) {
        return OWI_OUT_OF_MEM;
    }
    GpioPinConfigSet(txrx_port, txrx_pin,
                     GPIO_CFG_PERIPHAL|GPIO_CFG_OUTPUT|GPIO_CFG_MULTIDRIVE|
                     GPIO_CFG_PULLUP|GPIO_CFG_INIT_HIGH );
    TIM_Init(timer);
    TIM_Clear(timer);
    owcb->owi_ev = &sig_TIM3;
    owcb->timer = timer;
    owcb->ccmr_shift = (channel-1)*4;
    switch (channel) {
    case 1:
        owcb->compare    = &(owi_timer->CCR1);
        owcb->capture    = &(owi_timer->CCR2);
        owcb->ccmr       = &(owi_timer->CCMR1);
        /* CC1 as output, T1 mapped to CC2 to capture rising edge*/
        *owcb->ccmr      = TIM_CCMR1_CC2S_1 ;
        owi_timer->CCER = TIM_CCER_CC2E | TIM_CCER_CC1E |TIM_CCER_CC1P;
        break;
    case 2:
        owcb->compare    = &(owi_timer->CCR2);
        owcb->capture    = &(owi_timer->CCR1);
        owcb->ccmr       = &(owi_timer->CCMR1);
        /* CC2 as output, T2 mapped to CC1 to capture rising edge*/
        *owcb->ccmr      = TIM_CCMR1_CC1S_1;
        owi_timer->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E |TIM_CCER_CC2P;
        break;
    case 3:
        owcb->compare    = &(owi_timer->CCR3);
        owcb->capture    = &(owi_timer->CCR4);
        owcb->ccmr       = &(owi_timer->CCMR2);
        /* CC3 as output, T3 mapped to CC4 to capture rising edge*/
        *owcb->ccmr      = TIM_CCMR2_CC4S_1;
        owi_timer->CCER = TIM_CCER_CC4E | TIM_CCER_CC3E |TIM_CCER_CC3P;
       break;
    case 4:
        owcb->compare    = &(owi_timer->CCR4);
        owcb->capture    = &(owi_timer->CCR3);
        owcb->ccmr       = &(owi_timer->CCMR2);
        /* CC4 as output, T4 mapped to CC3 to capture rising edge*/
        *owcb->ccmr      = TIM_CCMR2_CC3S_1;
        owi_timer->CCER = TIM_CCER_CC3E | TIM_CCER_CC4E |TIM_CCER_CC4P;
        break;
    }
    /* ccmr and ccer need to be set before connecting the output*/
    GPIO_PinAFConfig((GPIO_TypeDef*) txrx_port, txrx_pin, gpio_af);
    NutRegisterIrqHandler( owcb->owi_ev, &STM32TIM_OwiInterrupt, owcb);
    TIM_Prescaler(timer) = ((TIM_ClockVal(timer))/2000000L)-1;
    TIM_OnePulse( timer);
    TIM_Update(timer);
    owi_timer->SR = 0;
    NutIrqEnable(owcb->owi_ev);
    bus->owibus_info = (uintptr_t)owcb;
    bus->OwiTouchReset = STM32TIM_OwiTouchReset;
    bus->OwiReadBlock = STM32TIM_OwiReadBlock;
    bus->OwiWriteBlock = STM32TIM_OwiWriteBlock;
    bus->mode = 0;
    return OWI_SUCCESS;
}
