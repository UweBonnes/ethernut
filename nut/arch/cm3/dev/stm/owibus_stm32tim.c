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
/*!
 * \file dev/owibus-stm32tim.c
 * \brief Common implementation of One-Wire primitives with dual channel
 *        STM32 timer, configured at library compile time. This file is
 *        to be included from  bus specific drivers.
 *
 * The OWI Bus needs to be connected of CH1|2 or CH3|4 of some timer. The
 * base pin is set to opendrain, and CNT and CCR1|2 or CCR3|4 are use to
 * generate the pulse and capture the rising edge.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

/* Force level to inactive at startup */
#if defined(STM32TIM_OWI_CHANNEL)
#if (STM32TIM_OWI_CHANNEL == 1)
#define  STM32TIM_OWI_CCMR CCMR1
#define  STM32TIM_OWI_COMPARE CCR1
#define  STM32TIM_OWI_CCMODE TIM_CCMR1_CC2S_1 | TIM_CCMR1_OC1M_2
#define  STM32TIM_OWI_CAPTURE CCR2
#define  STM32TIM_OWI_CCMR_SHIFT 4
#define  STM32TIM_OWI_CCER (TIM_CCER_CC2E | TIM_CCER_CC1E | TIM_CCER_CC1P)
#elif (STM32TIM_OWI_CHANNEL == 2)
#define  STM32TIM_OWI_CCMR CCMR1
#define  STM32TIM_OWI_CCMODE TIM_CCMR1_CC1S_1 | TIM_CCMR1_OC2M_2
#define  STM32TIM_OWI_COMPARE CCR2
#define  STM32TIM_OWI_CAPTURE CCR1
#define  STM32TIM_OWI_CCMR_SHIFT 12
#define  STM32TIM_OWI_CCER (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC2P)
#elif (STM32TIM_OWI_CHANNEL == 3)
#define  STM32TIM_OWI_CCMR CCMR2
#define  STM32TIM_OWI_CCMODE TIM_CCMR2_CC4S_1 | TIM_CCMR2_OC3M_2
#define  STM32TIM_OWI_COMPARE CCR3
#define  STM32TIM_OWI_CAPTURE CCR4
#define  STM32TIM_OWI_CCMR_SHIFT 4
#define  STM32TIM_OWI_CCER (TIM_CCER_CC4E | TIM_CCER_CC3E | TIM_CCER_CC3P)
#elif (STM32TIM_OWI_CHANNEL == 4)
#define  STM32TIM_OWI_CCMR CCMR2
#define  STM32TIM_OWI_CCMODE TIM_CCMR2_CC3S_1 | TIM_CCMR2_OC4M_2
#define  STM32TIM_OWI_COMPARE CCR4
#define  STM32TIM_OWI_CAPTURE CCR3
#define  STM32TIM_OWI_CCMR_SHIFT 12
#define  STM32TIM_OWI_CCER (TIM_CCER_CC3E | TIM_CCER_CC4E |TIM_CCER_CC4P)
#else
#warning Wrong STM32TIM_OWI_CHANNEL
#endif
#else
#warning STM32TIM_OWI_CHANNEL undefined
#endif

/* There is no timer yet with 3 channels only (20130703) */
#if (STM32TIM_OWI_CHANNEL > STM32_OWITIMER_NCH ) || (STM32_OWITIMER_NCH < 2)
#warning No compagnion timer channel available
#endif

static void Stm32Tim_OwiInterrupt(void *arg)
{
    if (TIM_Status(STM32_OWITIMER_BASE) & TIM_SR_UIF) {
        TIM_Status(STM32_OWITIMER_BASE) &= ~TIM_SR_UIF; /* Reset Flag*/
        NutEventPostFromIrq(&STM32TIM_OWI_MUTEX);
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
static int Stm32Tim_OwiTransaction(NUTOWIBUS *bus, int_fast8_t command, int_fast8_t value)
{
    TIM_TypeDef *owi_timer =   (TIM_TypeDef *)STM32_OWITIMER_BASE;
    uint16_t ccmr = owi_timer->STM32TIM_OWI_CCMR;

    if (value)
        owi_timer->STM32TIM_OWI_COMPARE = -(owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RELEASE]
                           - owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_SYNC_PULSE]);
    else
        owi_timer->STM32TIM_OWI_COMPARE = -(owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RELEASE]
                           - owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_SYNC_PULSE_LOW]);

    owi_timer->CNT = -(owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RELEASE]
                       - owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_SETUP]);
    NutEnterCritical();
    TIM_IRQEnable(STM32_OWITIMER_BASE);
    /* Force active level */
    ccmr &= ~( 7 <<(STM32TIM_OWI_CCMR_SHIFT));
    ccmr |=  ( 5 <<(STM32TIM_OWI_CCMR_SHIFT));
    owi_timer->STM32TIM_OWI_CCMR = ccmr;
    TIM_StartTimer(STM32_OWITIMER_BASE);
    /* Toggle on compare match */
    ccmr &= ~( 7 <<(STM32TIM_OWI_CCMR_SHIFT));
    ccmr |=  ( 3 <<(STM32TIM_OWI_CCMR_SHIFT));
    owi_timer->STM32TIM_OWI_CCMR = ccmr;
    NutExitCritical();
    if(NutEventWait(&STM32TIM_OWI_MUTEX, 10)) {
        if (command == OWI_CMD_RESET)
            return OWI_PRESENCE_ERR;
        else
            return OWI_DATA_ERROR;
    }
    owi_timer->SR = 0;
    TIM_IRQDisable(STM32_OWITIMER_BASE);
    /* We need to handle 32 and 16 bit counters. Store the value to compare
       in timer->capture, so the same thing happens to the top word*/
    owi_timer->STM32TIM_OWI_COMPARE =
        -( owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RELEASE]
           - owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RW]);
    if (owi_timer->STM32TIM_OWI_CAPTURE > owi_timer->STM32TIM_OWI_COMPARE)
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
static int Stm32Tim_OwiTouchReset(NUTOWIBUS *bus)
{
    return Stm32Tim_OwiTransaction(bus, OWI_CMD_RESET, 1);
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
static int Stm32Tim_OwiRWBit(NUTOWIBUS *bus, uint_fast8_t bit)
{
    return Stm32Tim_OwiTransaction(bus, OWI_CMD_RWBIT, bit);
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
static int Stm32Tim_OwiWriteBlock(
    NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len)
{
    int res;
    int i;

    for (i = 0; i < len; i++) {
        res = Stm32Tim_OwiRWBit(bus, data[i >> 3] & (1 << (i & 0x7)));
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
static int Stm32Tim_OwiReadBlock(
    NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len)
{
    int res;
    int i;

    memset(data, 0, (len >> 3) + 1);
    for (i = 0; i < len; i++) {
        res = Stm32Tim_OwiRWBit(bus, 1);
        if (res < 0)
            return OWI_HW_ERROR;
        data[i >> 3] |= (res << (i & 0x7));
    }
    return OWI_SUCCESS;
}

/*!
 * \brief Initailize the driver.
 *
 * \param bus  Specifies the One-Wire bus.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
static int Stm32Tim_OwiSetup(NUTOWIBUS *bus)
{
    (void) bus;
    TIM_TypeDef *owi_timer =   (TIM_TypeDef *)STM32_OWITIMER_BASE;

#if !defined(STM32TIM_OWI_PORT) || !defined(STM32TIM_OWI_PIN) ||\
    !defined(STM32_OWITIMER_BASE) || !defined(STM32TIM_OWI_CHANNEL)
    return OWI_INVALID_HW;
#else
    STM32_OWITIMER_CLK();
    STM32_OWITIMER_RST();
    NutRegisterIrqHandler( &STM32_OWITIMER_SIG, &Stm32Tim_OwiInterrupt, NULL);
    owi_timer->PSC = (STM32_OWITIMER_PCLK()/2000000L)-1;
    TIM_OnePulse(STM32_OWITIMER_BASE);
    owi_timer->STM32TIM_OWI_CCMR = STM32TIM_OWI_CCMODE;
    owi_timer->CCER = STM32TIM_OWI_CCER;
    TIM_Update(STM32_OWITIMER_BASE);
    owi_timer->SR   = 0;
    NutIrqEnable(&STM32_OWITIMER_SIG);
    GpioPinConfigSet(STM32TIM_OWI_PORT, STM32TIM_OWI_PIN,
                     GPIO_CFG_PERIPHAL|GPIO_CFG_OUTPUT|GPIO_CFG_MULTIDRIVE|
                     GPIO_CFG_PULLUP|GPIO_CFG_INIT_HIGH );

#if defined(MCU_STM32F1)
    CM3REG(AFIO_BASE, AFIO_TypeDef, STM32_OWITIMER_REMAP_REG) &= ~(STM32_OWITIMER_REMAP_MASK);
    CM3REG(AFIO_BASE, AFIO_TypeDef, STM32_OWITIMER_REMAP_REG) |=  (STM32TIM_OWI_REMAP << STM32_OWITIMER_REMAP_SHIFT);
#else
    GPIO_PinAFConfig(STM32TIM_OWI_PORT, STM32TIM_OWI_PIN, STM32_OWITIMER_AF );
#endif

    return OWI_SUCCESS;
#endif
