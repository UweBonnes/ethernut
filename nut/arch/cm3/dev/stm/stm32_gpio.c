/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * Copyright (C) 2011-2015 Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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

/*
 * \verbatim
 * $Id$
 * \endverbatim
 */

/* GPIO Configuration for the GPIO of L0/L1/F2/F3/F4/F7 */

#include <cfg/os.h>
#include <cfg/arch.h>
#include <cfg/arch/gpio.h>

#include <sys/nutdebug.h>

#include <arch/cm3.h>
#include <dev/gpio.h>
#include <arch/cm3/stm/stm32_gpio.h>
#if defined (MCU_STM32L0)
const uint16_t ospeed_values[4] =
{2000000 >> 16,  2000000 >> 16, 10000000 >> 16,  50000000 >> 16};
#elif defined (MCU_STM32L1)
const uint16_t ospeed_values[4] =
{ 400000 >> 16,  2000000 >> 16, 10000000 >> 16,  50000000 >> 16};
#elif defined (MCU_STM32F0)
const uint16_t ospeed_values[4] =
{2000000 >> 16,  2000000 >> 16, 10000000 >> 16,  50000000 >> 16};
#elif defined (MCU_STM32F2)
const uint16_t ospeed_values[4] =
{2000000 >> 16, 25000000 >> 16, 50000000 >> 16, 100000000 >> 16};
#elif defined (MCU_STM32F3)
const uint16_t ospeed_values[4] =
{2000000 >> 16,  2000000 >> 16, 10000000 >> 16,  50000000 >> 16};
#elif defined (MCU_STM32F4)
const uint16_t ospeed_values[4] =
{2000000 >> 16, 25000000 >> 16, 50000000 >> 16, 100000000 >> 16};
#elif defined (MCU_STM32F7)
const uint16_t ospeed_values[4] =
{2000000 >> 16, 25000000 >> 16, 50000000 >> 16, 100000000 >> 16};
#else
#warning "Unknown STM32 family"
#endif

static void Stm32GpioCompensationEnable(void)
{
#if defined(SYSCFG_CMPCR_CMP_PD)
    /* On F4, if even one pin needs fastest (high) speed, we need to
     * enable the SYSCFG clock and the IO compensation cell
     */
    CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR, _BI32(RCC_APB2ENR_SYSCFGEN));
    CM3BBSET(SYSCFG_BASE, SYSCFG_TypeDef, CMPCR, _BI32(SYSCFG_CMPCR_CMP_PD));
    /* FIXME: Do we need to check SYSCFG_CMPCR_READY ? */
#endif
}

/*!
 * \brief Get pin configuration.
 *
 * Trying to set undefined ports must be avoided.
 * If NUTDEBUG is enabled an assertion will be rised.
 *
 * \param bank GPIO bank/port number.
 * \param bit  Bit number of the specified bank/port.
 *
 * \return Attribute flags of the pin.
 */
uint32_t GpioPinConfigGet(int bank, int bit)
{
    uint32_t rc = 0;
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)bank;
    uint8_t mode = ((gpio->MODER) >> (bit *2)) & 0x3;
    uint8_t pull = ((gpio->PUPDR) >> (bit *2)) & 0x3;
    uint8_t dr_oc = ((gpio->OTYPER) >> bit ) & 0x1;
    uint8_t speed = ((gpio->OSPEEDR) >> (bit *2)) & 0x3;
    if (mode == GPIO_Mode_OUT)
    {
        rc = GPIO_CFG_OUTPUT;
        rc |= (dr_oc)? GPIO_CFG_MULTIDRIVE: 0;
        rc |= (pull == GPIO_PuPd_UP)?GPIO_CFG_PULLUP : 0;
    }
    else if (mode == GPIO_Mode_AF)
    {
        rc  = GPIO_CFG_PERIPHAL;
        rc |= (dr_oc)? GPIO_CFG_MULTIDRIVE: 0;
        rc |= (pull == GPIO_PuPd_UP)?GPIO_CFG_PULLUP : 0;
    }
    else if (mode == GPIO_Mode_AN)
    {
        rc  = GPIO_CFG_OUTPUT| GPIO_CFG_DISABLED;
    }
    else if(mode == GPIO_Mode_IN)
    {
        rc |= (pull == GPIO_PuPd_UP)?GPIO_CFG_PULLUP : 0;
    }
    switch (speed)
    {
    case 0:  rc |= GPIO_CFG_SPEED_SLOW; break;
    case 1:  rc |= GPIO_CFG_SPEED_MED;  break ;
    case 2:  rc |= GPIO_CFG_SPEED_HIGH; break;
    case 3:  rc |= GPIO_CFG_SPEED_FAST; break;
     }
    return rc;
}

/*!
 * \brief Set port wide pin configuration.
 *
 * \note This function does not check for undefined ports and pins or
 *       invalid attributes. If this is required, use GpioPinConfigSet().
 *       If NUTDEBUG is enabled accessing an undefined port will rise
 *       an assertion.
 *
 * \param bank  GPIO bank/port number.
 * \param mask  The given attributes are set for a specific pin, if the
 *              corresponding bit in this mask is 1.
 * \param flags Attribute flags to set.
 *
 * \return Always 0.
 */
int GpioPortConfigSet(int bank, uint32_t mask, uint32_t flags)
{
    int i;

    for( i=0; i<16; i++)
    {
        if (mask & 1)
            GpioPinConfigSet(bank, i, flags);
        mask >>= 1;
    }
    return 0;
}

int GpioPinConfigSet(int bank, int bit, uint32_t flags)
{
    NUTASSERT(IS_GPIO_ALL_PERIPH(bank));
    GPIO_TypeDef *gpio = (GPIO_TypeDef *) bank;

    GpioClkEnable(bank);
    if (flags == GPIO_CFG_DISABLED) {
        /* Set Analog Mode and reset all other bits */
        gpio->MODER   |=  (3 << (bit << 1));
        gpio->OTYPER  &= ~(1 << bit);
        gpio->OSPEEDR &= ~(3 << (1 << bit));
        gpio->PUPDR   &= ~(3 << (1 << bit));
        if( GpioPinConfigGet( bank, bit ) != flags ) {
            return -1;
        }
        return 0;
    }
    /* Set the inital value, if given
     *
     * Otherwise we may introduce unwanted transistions on the port
     */
    if (flags & GPIO_CFG_INIT_HIGH)
    {
        if (flags & GPIO_CFG_INIT_LOW)
            return -1;
        else
            GpioPinSetHigh(bank, bit);
    }
    if (flags & GPIO_CFG_INIT_LOW)
        GpioPinSetLow(bank, bit);

    /* we can't check for these flags, so clear them */
    flags &= ~(GPIO_CFG_INIT_LOW |GPIO_CFG_INIT_HIGH);

    /* Reset all two bit entries */
    gpio->MODER   &= ~(0x3 <<(bit << 1));
    gpio->OSPEEDR &= ~(0x3 <<(bit << 1));
    gpio->PUPDR   &= ~(0x3 <<(bit << 1));
    /* For non-output, multidrive is don't care*/
    if (flags & GPIO_CFG_MULTIDRIVE )
        gpio->OTYPER |= 1<<bit;
    else
        gpio->OTYPER &= ~(1<<bit);
    /* For non-output, ospeedr is don't care*/
    switch(flags & GPIO_CFG_SPEED_FAST)
    {
    case GPIO_CFG_SPEED_HIGH:
        gpio->OSPEEDR |=  (3 <<(bit << 1));
        Stm32GpioCompensationEnable();
        break;
    case GPIO_CFG_SPEED_FAST:
        gpio->OSPEEDR |=  (2 <<(bit << 1));
        break;
    case GPIO_CFG_SPEED_MED:
        gpio->OSPEEDR |=  (0x1 <<(bit << 1));
        break;
    }
    /* Pull Up/Pull Down applies to all configurations*/
        if (flags & GPIO_CFG_PULLUP )
            gpio->PUPDR |=  (1<<((bit << 1)));
        if (flags & GPIO_CFG_PULLDOWN )
            gpio->PUPDR |=  (2<<((bit << 1)));

    if (flags & GPIO_CFG_PERIPHAL)
    {
        gpio->MODER |=  2<<((bit << 1));
    }
    else if (flags & GPIO_CFG_OUTPUT)
    {
        gpio->MODER |=  1<<((bit << 1));
    }

    /* Check the result. */
    if( GpioPinConfigGet( bank, bit ) != flags ) {
        return -1;
    }
    return 0;
}

/* Copied from STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c*/
/**
  * @}
  */

/** @defgroup GPIO_Group3 GPIO Alternate functions configuration function
 *  @brief   GPIO Alternate functions configuration function
 *
 ===============================================================================
               GPIO Alternate functions configuration function
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Changes the mapping of the specified pin.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @param  GPIO_PinSource: specifies the pin for the Alternate function.
  *         This parameter can be GPIO_PinSourcex where x can be (0..15).
  * @param  GPIO_AFSelection: selects the pin to used as Alternate function.
  *          This parameter can be one of the following values:
  *            @arg GPIO_AF_RTC_50Hz: Connect RTC_50Hz pin to AF0 (default after reset)
  *            @arg GPIO_AF_MCO: Connect MCO pin (MCO1 and MCO2) to AF0 (default after reset)
  *            @arg GPIO_AF_TAMPER: Connect TAMPER pins (TAMPER_1 and TAMPER_2) to AF0 (default after reset)
  *            @arg GPIO_AF_SWJ: Connect SWJ pins (SWD and JTAG)to AF0 (default after reset)
  *            @arg GPIO_AF_TRACE: Connect TRACE pins to AF0 (default after reset)
  *            @arg GPIO_AF_TIM1: Connect TIM1 pins to AF1
  *            @arg GPIO_AF_TIM2: Connect TIM2 pins to AF1
  *            @arg GPIO_AF_TIM3: Connect TIM3 pins to AF2
  *            @arg GPIO_AF_TIM4: Connect TIM4 pins to AF2
  *            @arg GPIO_AF_TIM5: Connect TIM5 pins to AF2
  *            @arg GPIO_AF_TIM8: Connect TIM8 pins to AF3
  *            @arg GPIO_AF_TIM9: Connect TIM9 pins to AF3
  *            @arg GPIO_AF_TIM10: Connect TIM10 pins to AF3
  *            @arg GPIO_AF_TIM11: Connect TIM11 pins to AF3
  *            @arg GPIO_AF_I2C1: Connect I2C1 pins to AF4
  *            @arg GPIO_AF_I2C2: Connect I2C2 pins to AF4
  *            @arg GPIO_AF_I2C3: Connect I2C3 pins to AF4
  *            @arg GPIO_AF_SPI1: Connect SPI1 pins to AF5
  *            @arg GPIO_AF_SPI2: Connect SPI2/I2S2 pins to AF5
  *            @arg GPIO_AF_SPI3: Connect SPI3/I2S3 pins to AF6
  *            @arg GPIO_AF_I2S3ext: Connect I2S3ext pins to AF7
  *            @arg GPIO_AF_USART1: Connect USART1 pins to AF7
  *            @arg GPIO_AF_USART2: Connect USART2 pins to AF7
  *            @arg GPIO_AF_USART3: Connect USART3 pins to AF7
  *            @arg GPIO_AF_UART4: Connect UART4 pins to AF8
  *            @arg GPIO_AF_UART5: Connect UART5 pins to AF8
  *            @arg GPIO_AF_USART6: Connect USART6 pins to AF8
  *            @arg GPIO_AF_CAN1: Connect CAN1 pins to AF9
  *            @arg GPIO_AF_CAN2: Connect CAN2 pins to AF9
  *            @arg GPIO_AF_TIM12: Connect TIM12 pins to AF9
  *            @arg GPIO_AF_TIM13: Connect TIM13 pins to AF9
  *            @arg GPIO_AF_TIM14: Connect TIM14 pins to AF9
  *            @arg GPIO_AF_OTG_FS: Connect OTG_FS pins to AF10
  *            @arg GPIO_AF_OTG_HS: Connect OTG_HS pins to AF10
  *            @arg GPIO_AF_ETH: Connect ETHERNET pins to AF11
  *            @arg GPIO_AF_FSMC: Connect FSMC pins to AF12
  *            @arg GPIO_AF_OTG_HS_FS: Connect OTG HS (configured in FS) pins to AF12
  *            @arg GPIO_AF_SDIO: Connect SDIO pins to AF12
  *            @arg GPIO_AF_DCMI: Connect DCMI pins to AF13
  *            @arg GPIO_AF_EVENTOUT: Connect EVENTOUT pins to AF15
  * @retval None
  */
void GPIO_PinAFConfig(nutgpio_port_t GPIOx, nutgpio_pin_t GPIO_PinSource, uint8_t GPIO_AF)
{
  uint32_t temp = 0x00;
  uint32_t temp_2 = 0x00;
  GPIO_TypeDef * gpio = (GPIO_TypeDef *)GPIOx;
  volatile uint32_t *afr;

  /* Check the parameters */
  NUTASSERT(IS_GPIO_ALL_PERIPH(GPIOx));
  NUTASSERT(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  NUTASSERT(IS_GPIO_AF(GPIO_AF));

  afr = &gpio->AFR[GPIO_PinSource / 8 ];
  temp = 0xf << ((GPIO_PinSource % 8) * 4);
  temp_2 = *afr;
  temp_2 &= ~temp;
  temp_2 |= GPIO_AF << ((GPIO_PinSource % 8) * 4);
  *afr = temp_2;
}
