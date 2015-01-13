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
 * F42
 *        NSS:  PF6/PH5
 *        SCK:  PF7/PH6
 *        MISO: PF8/PH7
 *        MOSI: PF9/PF11
 * For function pins, we use PF6/7/8/9 as default
 *
 * F411
 *        NSS:  PB1/PE4/PE11
 *        SCK:  PB0/PE2/PE12
 *        MISO: PA12/PE5/PE13
 *        MOSI: PA10/PB8/PE6/PE14
 * For function pins, we use PB1/PB0/PA12/PA10 as default
 */

#if !defined( SPIBUS5_NO_CS)
 #if !defined(SPIBUS5_CS0_PORT) && !defined(SPIBUS5_CS0_PIN)
  #if defined(STM32F411)
   #define SPIBUS_CS0_PORT NUTGPIO_PORTB
   #define SPIBUS_CS0_PIN  1
  #else
   #define SPIBUS_CS0_PORT NUTGPIO_PORTF
   #define SPIBUS_CS0_PIN  6
  #endif
 #elif !defined(SPIBUS5_CS0_PORT) || !defined(SPIBUS5_CS0_PIN)
  #warning "SPIBUS5 uncomplete chip select"
 #else
  #define SPIBUS_CS0_PORT SPIBUS5_CS0_PORT
  #define SPIBUS_CS0_PIN  SPIBUS5_CS0_PIN
 #endif

 #if defined(SPIBUS5_CS1_PORT) && defined(SPIBUS5_CS1_PIN)
  #define SPIBUS_CS1_PORT SPIBUS5_CS1_PORT
  #define SPIBUS_CS1_PIN  SPIBUS5_CS1_PIN
 #endif
 #if defined(SPIBUS5_CS2_PORT) && defined(SPIBUS5_CS2_PIN)
  #define SPIBUS_CS2_PORT SPIBUS5_CS2_PORT
  #define SPIBUS_CS2_PIN  SPIBUS5_CS2_PIN
 #endif
 #if defined(SPIBUS5_CS3_PORT) && defined(SPIBUS5_CS3_PIN)
  #define SPIBUS_CS3_PORT SPIBUS5_CS3_PORT
  #define SPIBUS_CS3_PIN  SPIBUS5_CS1_PIN
 #endif
#endif

#if defined(STM32F411)
 #define SPI_GPIO_AF GPIO_AF_5
 #if  !defined(SPIBUS5_SCK_PIN) || SPIBUS5_SCK_PIN == 0
  #define SPIBUS_SCK_PORT  NUTGPIO_PORTB
  #define SPIBUS_SCK_PIN 10
 #elif  SPIBUS5_SCK_PIN == 2
  #define SPIBUS_SCK_PORT  NUTGPIO_PORTE
  #define SPIBUS_SCK_PIN 2
 #elif  SPIBUS5_SCK_PIN == 12
  #define SPIBUS_SCK_PORT  NUTGPIO_PORTE
  #define SPIBUS_SCK_PIN 12
 #else
  #warning Unknown STM32F411 SPIBUS5_SCK_PIN
 #endif

 #if !defined(SPIBUS5_MISO_PIN) || SPIBUS5_MISO_PIN == 13
  #define SPIBUS_MISO_PORT NUTGPIO_PORTA
  #define SPIBUS_MISO_PIN 12
 #elif  SPIBUS5_MISO_PIN == 13
  #define SPIBUS_MISO_PORT NUTGPIO_PORTE
  #define SPIBUS_MISO_PIN 13
 #elif  SPIBUS5_MISO_PIN == 5
  #define SPIBUS_MISO_PORT NUTGPIO_PORTE
  #define SPIBUS_MISO_PIN 5
 #else
  #warning Unknown STM32F411 SPIBUS5_MISO_PIN
 #endif

 #if !defined(SPIBUS5_MOSI_PIN) || SPIBUS5_MOSI_PIN == 10
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTA
  #define SPIBUS_MOSI_PIN 10
 #elif  SPIBUS5_MOSI_PIN == 8
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
  #define SPIBUS_MOSI_PIN 8
 #elif  SPIBUS5_MOSI_PIN == 6
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTE
  #define SPIBUS_MOSI_PIN 6
 #elif  SPIBUS5_MOSI_PIN == 14
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTE
  #define SPIBUS_MOSI_PIN 14
 #else
  #warning Unknown STM32F411 SPIBUS5_MOSI_PIN
 #endif

#else
/* F42x */
 #define SPI_GPIO_AF GPIO_AF_SPI5
 #if !defined(SPIBUS5_SCK_PIN) || SPIBUS5_SCK_PIN == 7
  #define SPIBUS_SCK_PORT NUTGPIO_PORTF
  #define SPIBUS_SCK_PIN 7
 #elif SPIBUS5_SCK_PIN == 6
  #define SPIBUS_SCK_PORT  NUTGPIO_PORTH
  #define SPIBUS_SCK_PIN 6
 #else
  #warning Unknown STM32F42/3 SPIBUS5_SCK_PIN
 #endif

 #if !defined(SPIBUS5_MISO_PIN) || SPIBUS5_MISO_PIN ==8
  #define SPIBUS_MISO_PORT NUTGPIO_PORTF
  #define SPIBUS_MISO_PIN 8
 #elif SPIBUS5_MISO_PIN == 7
  #define SPIBUS_MISO_PORT NUTGPIO_PORTH
  #define SPIBUS_MISO_PIN 7
 #else
  #warning Unknown STM32F42/3 SPIBUS5_MISO_PIN
 #endif

 #if !defined(SPIBUS5_MOSI_PIN) || SPIBUS5_MOSI_PIN ==9
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTF
  #define SPIBUS_MOSI_PIN 9
 #elif  SPIBUS5_MOSI_PIN == 11
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTF
  #define SPIBUS_MOSI_PIN 11
 #else
  #warning Unknown STM32F42/3 SPIBUS5_MOSI_PIN
 #endif

#endif

#define SPI_ENABLE_CLK_SET() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR, _BI32(RCC_APB2ENR_SPI5EN))
#define SPI_ENABLE_CLK_GET() CM3BBGET(RCC_BASE, RCC_TypeDef, APB2ENR, _BI32(RCC_APB2ENR_SPI5EN))
#define sig_SPI             sig_SPI5
#define SPI_BASE            SPI5_BASE

#if !defined(SPIBUS5_MODE)
#define SPIBUS_MODE IRQ_MODE
#else
#define SPIBUS_MODE SPIBUS5_MODE
#endif

#if SPIBUS_MODE == DMA_MODE
 #if defined(SPIBUS1_DMA_TX_ALTERNATE_STREAM)
  #define SPI_DMA_TX_CHANNEL SPI5_TX_ALT_DMA
  #define sig_SPI_DMA_TX     SPI5_TX_ALT_DMA_IRQ
 #else
  #define SPI_DMA_TX_CHANNEL SPI5_TX_DMA
  #define sig_SPI_DMA_TX     SPI5_TX_DMA_IRQ
 #endif
 #if defined(SPIBUS1_DMA_RX_ALTERNATE_STREAM)
  #define SPI_DMA_RX_CHANNEL SPI5_RX_ALT_DMA
  #define sig_SPI_DMA_RX     SPI5_RX_ALT_DMA_IRQ
 #else
  #define SPI_DMA_RX_CHANNEL SPI5_RX_DMA
  #define sig_SPI_DMA_RX     SPI5_RX_DMA_IRQ
 #endif
#endif

NUTSPIBUS spiBus5Stm32 = {
    NULL,                       /*!< Bus mutex semaphore (bus_mutex). */
    NULL,                       /*!< Bus ready signal (bus_ready). */
    SPI5_BASE,                  /*!< Bus base address (bus_base). */
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
