/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * Copyright (C) 2010 by Nikolaj Zamotaev. All rights reserved.
 * Copyright (C) 2014 by Uwe Bonnes.
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
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <arch/cm3.h>
#include <sys/timer.h>
#include <cfg/spi.h>
#include <cfg/arch/gpio.h>
#include <dev/spibus.h>
#include <dev/gpio.h>

#include <arch/cm3/stm/stm32_gpio.h>
#include <arch/cm3/stm/stm32_spi.h>
#include <dev/irqreg.h>
#include <sys/event.h>
#include <sys/nutdebug.h>

#include <stdlib.h>
#include <errno.h>

/* Handle the PIN remap possibilities
 * F4
 *        NSS:  PE4/PE11
 *        SCK:  PE2/PE12
 *        MISO: PE5/PE13
 *        MOSI: PE6/PE14
 *
 * For Chip select, we use NSS pin as default or any other pin as pure GPIO
 *
 * For function pins, we use PE2/4/5/6 as default
 */

#if !defined(SPIBUS4_CS_PORT)
 #define SPIBUS_CS_PORT NUTGPIO_PORTE
#else
 #define SPIBUS_CS_PORT SPIBUS4_CS_PORT
#endif
#if !defined(SPIBUS4_CS_PIN)
 #define SPIBUS_CS_PIN 4
#else
 #define SPIBUS_CS_PIN SPIBUS4_CS_PIN
#endif
 #if  SPIBUS4_SCK_PIN == 12
  #define SPIBUS_SCK_PIN 12
 #else
  #define SPIBUS_SCK_PIN 2
 #endif
 #if  SPIBUS4_MISO_PIN == 13
  #define SPIBUS_MISO_PIN 13
 #else
  #define SPIBUS_MISO_PIN 5
 #endif
 #if  SPIBUS4_MOSI_PIN == 14
  #define SPIBUS_MOSI_PIN 14
 #else
  #define SPIBUS_MOSI_PIN 6
 #endif
#define SPIBUS_SCK_PORT  NUTGPIO_PORTE
#define SPIBUS_MISO_PORT NUTGPIO_PORTE
#define SPIBUS_MOSI_PORT NUTGPIO_PORTE

#define SPI_GPIO_AF GPIO_AF_SPI5
#define SPI_ENABLE_CLK (RCC->APB2ENR |= RCC_APB2ENR_SPI4EN)

NUTSPIBUS spiBus4Stm32 = {
    NULL,                       /*!< Bus mutex semaphore (bus_mutex). */
    NULL,                       /*!< Bus ready signal (bus_ready). */
    SPI4_BASE,                  /*!< Bus base address (bus_base). */
    NULL,                       /*!< Bus interrupt handler (bus_sig). */
    Stm32SpiBusNodeInit,         /*!< Initialize the bus (bus_initnode). */
    Stm32SpiBusSelect,          /*!< Select the specified device (bus_alloc). */
    Stm32SpiBusDeselect,        /*!< Deselect the specified device (bus_release). */
    Stm32SpiBusTransfer,
    NutSpiBusWait,
    NutSpiBusSetMode,           /*!< Set SPI mode of a specified device (bus_set_mode). */
    NutSpiBusSetRate,           /*!< Set clock rate of a specified device (bus_set_rate). */
    NutSpiBusSetBits            /*!< Set number of data bits of a specified device (bus_set_bits). */
};

#include "stm32_spi.c"
