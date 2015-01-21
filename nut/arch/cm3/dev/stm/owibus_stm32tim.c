/*
 * Copyright (C) 2013-15 by Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
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
 * \file dev/owibus-stm32tim.c
 * \brief Common implementation of One-Wire primitives with dual channel
 *        STM32 timer, configured at library compile time. This file is
 *        to be included from bus specific drivers.
 *
 * The OWI Bus needs to be connected of CH1|2 or CH3|4 of some timer. The
 * base pin is set to opendrain, and CNT and CCR1|2 or CCR3|4 are use to
 * generate the pulse and capture the rising edge.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

typedef enum {
    RESET_CYCLE = 0,
    RESET_ACTIVE,
    RESET_SAMPLE,
    CYCLE,
    WRITE_1,
    WRITE_0,
    SAMPLE,
    OWI_TIM_SIZE
}OWI_TIM_TIMES;

/* Total duration, period low, sample point  straight from AN126*/
static const uint16_t owi_tim_times_us[OWI_TIM_SIZE]  = {
    960, /* H+I+J     */
    480, /* H         */
    550, /* H+I       */
    70,  /* A+E+F/C+D */
    6,   /* A         */
    60,  /* C         */
    15   /* A+E       */
};

static uint16_t owi_tim_values[OWI_TIM_SIZE];

static volatile uint32_t *ccr_pulse;
static volatile uint32_t *ccr_capture;
static volatile uint16_t sample_value;
static volatile uint8_t owi_rx_len;
static volatile uint8_t owi_tx_len;
static volatile int      owi_index;
static uint8_t  *owi_txp;
static uint8_t  *owi_rxp;
static uint16_t owi_compare;

/* Each timer start has the one-pulse mode (OPM) set
 * and the the cc_pulse value for the next cycle set to 0.
 * Interruption may be harmfull between:
 * - Update shadow registers (TIM_EGR_UG) as this sets OWI low
 * - Start Timer.
 * Any interrupt in between here will prolong OWI long.
 * An between
 * - Start Timer.
 * - Write 0 as pulse value for next timer update
 * Any interruption as long as the cycle will generate a ghost cycle.
 *
 * Eventually a critical section is needed.
 */

static void Stm32TimOwiInterrupt(void *arg)
{
    TIM_TypeDef *owi_timer = (TIM_TypeDef *)STM32_OWITIMER_BASE;
    uint16_t flags;

    flags = owi_timer->SR;
    /* Reset all flags we have catched */
    owi_timer->SR = ~flags;
    if (flags & TIM_SR_UIF) {
        if (owi_rxp) {
            int res;
            unsigned int value;

            value = *ccr_capture;
            if (owi_rx_len > 1) {
                *ccr_pulse = sample_value;
                NutEnterCritical();
                CM3BBSET(STM32_OWITIMER_BASE, TIM_TypeDef, EGR, _BI32(TIM_EGR_UG));
                CM3BBSET(STM32_OWITIMER_BASE, TIM_TypeDef, CR1, _BI32(TIM_CR1_CEN));
                *ccr_pulse = 0;
                NutExitCritical();
            }
            if (owi_rx_len) {
                res = 1 << owi_index;
                if (value < owi_compare)
                    *owi_rxp |= res;
                else
                    *owi_rxp &= ~res;
                owi_rx_len --;
                if ( owi_index >= 7) {
                    owi_index = 0;
                    owi_rxp++;
                }
                else
                    owi_index++;
            }
            if (!owi_rx_len)
               NutEventPostFromIrq(&STM32TIM_OWI_MUTEX);
        }
        if (owi_txp) {
            if (owi_tx_len) {
                int data;

                data = *owi_txp & (1 << owi_index);
                if (data)
                    *ccr_pulse     = owi_tim_values[WRITE_1];
                else
                    *ccr_pulse     = owi_tim_values[WRITE_0];
                NutEnterCritical();
                CM3BBSET(STM32_OWITIMER_BASE, TIM_TypeDef, EGR, _BI32(TIM_EGR_UG));
                CM3BBSET(STM32_OWITIMER_BASE, TIM_TypeDef, CR1, _BI32(TIM_CR1_CEN));
                *ccr_pulse = 0;
                NutExitCritical();
                 if (owi_index >= 7) {
                    owi_index = 0;
                    owi_txp++;
                }
                else
                    owi_index++;
                owi_tx_len --;
            }
            else
                NutEventPostFromIrq(&STM32TIM_OWI_MUTEX);
        }
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
static int Stm32TimOwiTransaction(NUTOWIBUS *bus)
{
    int res;
    TIM_TypeDef *owi_timer =   (TIM_TypeDef *)STM32_OWITIMER_BASE;

    owi_timer->CNT = 0;
    owi_timer->SR  = 0;
    *ccr_pulse = sample_value;
    /* Transfer Values to shadow register*/
    NutEnterCritical();
    CM3BBSET(STM32_OWITIMER_BASE, TIM_TypeDef, EGR, _BI32(TIM_EGR_UG));
    CM3BBSET(STM32_OWITIMER_BASE, TIM_TypeDef, CR1, _BI32(TIM_CR1_CEN));
    *ccr_pulse = 0;
    NutExitCritical();
     if(NutEventWait(&STM32TIM_OWI_MUTEX, 10)) {
        res =  OWI_DATA_ERROR;
    }
    else
        res = OWI_SUCCESS;
    owi_txp = 0;
    owi_rxp = 0;
    return res;
}

/*!
 * \brief Reset the One-Wire bus and check if device(s) present.
 *
 *  With a connected device, we seldom see a single rising edge after
 *  the sample point. Mostly there is a first rising edge short after
 *  OWI line has been released by the master. The capture register
 *  only contains the later rising edge. So evaluating CCR after the
 *  update event will always give us the right edge. No need to evaluate
 *  the overcapture flag.
 *
 * \param bus Specifies the One-Wire bus.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
static int Stm32TimOwiTouchReset(NUTOWIBUS *bus)
{
    TIM_TypeDef *owi_timer =   (TIM_TypeDef *)STM32_OWITIMER_BASE;
    uint8_t data;
    int res;

    owi_index      = 0;
    owi_rx_len     = 1;
    owi_rxp        = &data;
    owi_timer->ARR = owi_tim_values[RESET_CYCLE];
    sample_value   = owi_tim_values[RESET_ACTIVE];
    owi_compare    = owi_tim_values[RESET_SAMPLE];
    res = Stm32TimOwiTransaction(bus);
    if ((res) || ((data & 1) == 1))
        return OWI_PRESENCE_ERR;
    return OWI_SUCCESS;
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
static int Stm32TimOwiWriteBlock(
    NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len)
{
    TIM_TypeDef *owi_timer =   (TIM_TypeDef *)STM32_OWITIMER_BASE;
    int res;

    if (len == 0)
        return OWI_SUCCESS;
    owi_tx_len     = len -1;
    owi_txp        = data;
    owi_timer->ARR = owi_tim_values[CYCLE];
    if (*data & 1)
        sample_value = owi_tim_values[WRITE_1];
    else
        sample_value = owi_tim_values[WRITE_0];
    owi_index      = 1;
    res = Stm32TimOwiTransaction(bus);
    if (res)
        return OWI_HW_ERROR;
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
static int Stm32TimOwiReadBlock(
    NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len)
{
    TIM_TypeDef *owi_timer =   (TIM_TypeDef *)STM32_OWITIMER_BASE;
    int res;

    owi_rx_len     = len;
    owi_rxp        = data;
    *owi_rxp       = 0;               /* Reset whole byte*/
    owi_timer->ARR = owi_tim_values[CYCLE];
    sample_value   = owi_tim_values[WRITE_1];
    owi_index      = 0;
    data           = 0;
    owi_compare    = owi_tim_values[SAMPLE];
    res = Stm32TimOwiTransaction(bus);
    if (res)
        return OWI_HW_ERROR;
    return OWI_SUCCESS;
}

/*!
 * \brief Initialize the driver.
 *
 * \param bus  Specifies the One-Wire bus.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
static int Stm32TimOwiSetup(NUTOWIBUS *bus)
{
#if !defined(STM32TIM_OWI_PORT) || !defined(STM32TIM_OWI_PIN) ||\
    !defined(STM32_OWITIMER_BASE) || !defined(STM32TIM_OWI_CHANNEL)
    return OWI_INVALID_HW;
#else
    (void) bus;
    uint32_t freq;
    int res;
    int prescaler;
    int capture_channel;
    TIM_TypeDef *owi_timer =   (TIM_TypeDef *)STM32_OWITIMER_BASE;
    uint32_t reg32u;
    uint16_t *p;
    const uint16_t *q;

    STM32_OWITIMER_INIT();
    Stm32TimerConfig(owi_timer, TIM_CLK_MODE_CKINT, TIM_TRG_SELECTION_NONE,
                       TIM_SLAVE_MODE_NONE, TIM_MASTER_MODE1_NONE);
    capture_channel = ((STM32TIM_OWI_CHANNEL -1) ^ 1) +1;
    /* Capture Rising edge an companion channel*/
    res = Stm32TimerChannelConfig(
        owi_timer, capture_channel, 0,
        TIM_CC_EXCHANGE, TIM_CC_FROZEN, TIM_CC_POL_TRUE);
    /* Active low while CNT < CCR */
    res |= Stm32TimerChannelConfig(
        owi_timer, STM32TIM_OWI_CHANNEL, 0,
        TIM_CC_OUTPUT, TIM_CC_ACTIVE_IF_LESS, TIM_CC_POL_INVERT);
    if (res)
        return -1;

    owi_txp = 0;
    owi_rxp = 0;
    owi_timer->CR1 |= TIM_CR1_ARPE | TIM_CR1_URS | TIM_CR1_OPM;
    /* Store registers for capture/compare access
     * And set Output compare 1 preload enable
     */
    switch(STM32TIM_OWI_CHANNEL) {
    case 1:
        ccr_pulse   = &owi_timer->CCR1;
        ccr_capture = &owi_timer->CCR2;
        owi_timer->CCMR1 |= TIM_CCMR1_OC1PE;
        break;
    case 2:
        ccr_pulse   = &owi_timer->CCR2;
        ccr_capture = &owi_timer->CCR1;
        owi_timer->CCMR1 |= TIM_CCMR1_OC2PE;
        break;
    case 3:
        ccr_pulse   = &owi_timer->CCR3;
        ccr_capture = &owi_timer->CCR4;
        owi_timer->CCMR2 |= TIM_CCMR2_OC3PE;
        break;
    case 4:
        ccr_pulse   = &owi_timer->CCR4;
        ccr_capture = &owi_timer->CCR3;
        owi_timer->CCMR2 |= TIM_CCMR2_OC4PE;
        break;
    }
    freq = Stm32TimerGetClock(owi_timer);
    /* We need as longest a 1 ms periode.
       We have 16 bit timers available.
       So at 2^16 * 1kHz ~= 64 MHz can be handled without prescaler */
    prescaler = 1;
    while(freq / prescaler > 64000000)
        prescaler ++;
    freq = freq / prescaler;
    if (prescaler > 1) {
        prescaler --;
        owi_timer->PSC = prescaler;
    }
    /* Calculate values.  Care not to overflow*/
    freq = freq / 32;
    p = owi_tim_values;
    q = owi_tim_times_us;
    do {
        *p++ = ((*q++  * freq)/(1000000 /32)) -1;
    }
    while (q < &owi_tim_times_us[OWI_TIM_SIZE]);
    /* Set the counter to one before overflow*/
    *ccr_pulse = 0;
    owi_timer->CNT = -1;
    owi_timer->DIER = TIM_DIER_UIE;
    NutRegisterIrqHandler( &STM32_OWITIMER_SIG, &Stm32TimOwiInterrupt, NULL);
    NutIrqEnable(&STM32_OWITIMER_SIG);
    reg32u  = GPIO_CFG_PERIPHAL | GPIO_CFG_MULTIDRIVE;
    reg32u |= GPIO_CFG_PULLUP   | GPIO_CFG_INIT_HIGH;
    GpioPinConfigSet(STM32TIM_OWI_PORT, STM32TIM_OWI_PIN,reg32u);
#if defined(MCU_STM32F1)
    CM3REG(AFIO_BASE, AFIO_TypeDef, STM32_OWITIMER_REMAP_REG) &= ~(STM32_OWITIMER_REMAP_MASK);
    CM3REG(AFIO_BASE, AFIO_TypeDef, STM32_OWITIMER_REMAP_REG) |=  (STM32TIM_OWI_REMAP << STM32_OWITIMER_REMAP_SHIFT);
#else
    GPIO_PinAFConfig(STM32TIM_OWI_PORT, STM32TIM_OWI_PIN, STM32_OWITIMER_AF);
#endif
    return OWI_SUCCESS;
#endif
}
