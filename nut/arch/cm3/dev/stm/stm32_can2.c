/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * Copyright (C) 2010 by Rittal GmbH & Co. KG. All rights reserved.
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

#include <cfg/os.h>
#include <cfg/clock.h>
#include <cfg/arch.h>
#include <cfg/can_dev.h>
#include <cfg/arch/gpio.h>

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <dev/irqreg.h>
#include <dev/gpio.h>
#include <dev/can_dev.h>

#if defined(MCU_STM32F1)
#include <arch/cm3/stm/stm32f10x.h>
#include <arch/cm3/stm/stm32f10x_gpio.h>
#include <arch/cm3/stm/stm32f10x_rcc.h>
#else
#warning "Unknown STM32 family"
#endif
#include <arch/cm3/stm/stm32_can.h>

#ifndef CANBUS2_REMAP_CAN
#define CANBUS2_REMAP_CAN 0
#endif

/*!
 * \brief CANBUS2 GPIO configuartion and assignment.
 */

#if (CANBUS2_REMAP_CAN==1)
  #define CANBUS_REMAP   GPIO_Remap_CAN2
  #define CANBUS_DOREMAP ENABLE

  #define CAN2_GPIO_PORT  NUTGPIO_PORTB
  #define CAN2RX_GPIO_PIN 5
  #define CAN2TX_GPIO_PIN 6

#else
  #define CANBUS_DOREMAP DISABLE

  #define CAN2_GPIO_PORT  NUTGPIO_PORTB
  #define CAN2RX_GPIO_PIN 12
  #define CAN2TX_GPIO_PIN 13
#endif


/*!
 * \brief Processor specific Hardware Initiliaization
 *
 */
int Stm32CanHw2Init(void)
{
    /* Enable CAN Bus 2 peripheral clock. */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    /* Reset CAN Bus 2 IP */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, DISABLE);

    /* Setup Related GPIOs. */
    GpioPinConfigSet(CAN2_GPIO_PORT, CAN2RX_GPIO_PIN, GPIO_CFG_PULLUP|GPIO_CFG_PERIPHAL);
    GpioPinConfigSet(CAN2_GPIO_PORT, CAN2TX_GPIO_PIN, GPIO_CFG_OUTPUT|GPIO_CFG_PERIPHAL);

#ifdef CANBUS_REMAP
    /* Configure alternate configuration. */
    GPIO_PinRemapConfig(CANBUS_REMAP, CANBUS_DOREMAP);
#endif

    return 0;
}

CANBUS Stm32CanBus2 = {
    CAN2_BASE,
    CM3BB_BASE(CAN2_BASE),
    &sig_CAN2_RX0,
    &sig_CAN2_RX1,
    &sig_CAN2_TX,
    &sig_CAN2_SCE,
    0,
    Stm32CanHw2Init
};


