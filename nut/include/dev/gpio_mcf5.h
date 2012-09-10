/*
 * Copyright 2012 by Embedded Technologies s.r.o
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

#ifndef _DEV_GPIO_H_
#error "Do not include this file directly. Use dev/gpio.h instead!"
#endif

#include <cfg/arch.h>
#include <dev/irqreg.h>

#if defined (MCU_MCF5225X)
#include <arch/m68k/coldfire/mcf5225x/mcf5225x_gpio.h>
#else
#warning "Unknown Coldfire MCU Family defined"
#endif


#define GPIO_CFG_PERIPHERAL_MASK    0x00000003

/*!
 * \brief Pin assumes the GPIO input function
 *
 * If set, the pin is configured as GPIO input.
 * This is the default state, when no other config option is given.
 */

#define GPIO_CFG_INPUT              0x00000000

/*!
 * \brief Pin assumes the GPIO output function
 */
#define GPIO_CFG_OUTPUT             0x00000004

/*!
 * \brief Pin assumes the primary function
 */
#define GPIO_CFG_PERIPHERAL0        0x00000001

/*!
 * \brief Pin assumes the secondary function
 */
#define GPIO_CFG_PERIPHERAL1        0x00000002

/*!
 * \brief Pin assumes the tertiary function
 */
#define GPIO_CFG_PERIPHERAL2        0x00000003

/*!
 * \brief GPIO configuration not supported by Coldfire MPUs
 */
#define GPIO_CFG_PULLUP     0x00000000
#define GPIO_CFG_MULTIDRIVE 0x00000000
#define GPIO_CFG_DEBOUNCE   0x00000000

extern uint32_t GpioPinConfigGet(int bank, int bit);
extern int GpioPinConfigSet(int bank, int bit, uint32_t flags);
extern int GpioPortConfigSet(int bank, uint32_t mask, uint32_t flags);

#define GpioPinGet(bank, bit)           ((MCF_GPIO_PIN(bank) >> bit) & 0x1)
#define GpioPinSet(bank, bit, value)    (value) ? (GpioPinSetHigh(bank, bit)) : (GpioPinSetLow(bank, bit))
#define GpioPinSetHigh(bank, bit)       MCF_GPIO_SET(bank) = _BV(bit)
#define GpioPinSetLow(bank, bit)        MCF_GPIO_CLR(bank) = _BV(bit)

#define GpioPortGet(bank)               MCF_GPIO_PIN(bank)
#define GpioPortSet(bank, value)        MCF_GPIO_PORT(bank) = (value)
#define GpioPortSetHigh(bank, mask)     MCF_GPIO_SET(bank) = (mask)
#define GpioPortSetLow(bank, mask)      MCF_GPIO_CLR(bank) = (mask)
