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
 * F401/F42
 *        NSS:  PE4/PE11
 *        SCK:  PE2/PE12
 *        MISO: PE5/PE13
 *        MOSI: PE6/PE14
 * F411
 *        NSS:  PE4/PB12(AF6)
 *        SCK:  PE2/PB13(AF6)
 *        MISO: PE5/PA11(AF6)
 *        MOSI: PE6/PA1
 * For Chip select, we use NSS pin as default or any other pin as pure GPIO
 *
 * Use PE4 as default chip select
  */

#if !defined( SPIBUS4_NO_CS)
 #if !defined(SPIBUS4_CS0_PORT) && !defined(SPIBUS4_CS0_PIN)
  #define SPIBUS_CS0_PORT NUTGPIO_PORTE
  #define SPIBUS_CS0_PIN  4
 #elif !defined(SPIBUS4_CS0_PORT) || !defined(SPIBUS4_CS0_PIN)
  #warnig "SPIBUS4 uncomplete chip select"
 #else
  #define SPIBUS_CS0_PORT SPIBUS4_CS0_PORT
  #define SPIBUS_CS0_PIN  SPIBUS4_CS0_PIN
 #endif

 #if defined(SPIBUS4_CS1_PORT) && defined(SPIBUS4_CS1_PIN)
  #define SPIBUS_CS1_PORT SPIBUS4_CS1_PORT
  #define SPIBUS_CS1_PIN  SPIBUS4_CS1_PIN
 #endif
 #if defined(SPIBUS4_CS2_PORT) && defined(SPIBUS4_CS2_PIN)
  #define SPIBUS_CS2_PORT SPIBUS4_CS2_PORT
  #define SPIBUS_CS2_PIN  SPIBUS4_CS2_PIN
 #endif
 #if defined(SPIBUS4_CS3_PORT) && defined(SPIBUS4_CS3_PIN)
  #define SPIBUS_CS3_PORT SPIBUS4_CS3_PORT
  #define SPIBUS_CS3_PIN  SPIBUS4_CS3_PIN
 #endif
#endif

/* Assume SPI4 only on F4 */
#if  SPIBUS4_SCK_PIN == 2 || !defined(SPIBUS4_SCK_PIN)
#define SPIBUS_SCK_PIN 2
#define SPIBUS_SCK_PORT  NUTGPIO_PORTE
#elif defined(STM32F411) && SPIBUS4_SCK_PIN == 13
#define SPIBUS_SCK_PIN   13
#define SPIBUS_SCK_PORT  NUTGPIO_PORTB
#define SPIBUS_SCK_AF    6
#elif (defined(STM32F401) || defined(STM32F42X)) && SPIBUS4_SCK_PIN == 12
#define SPIBUS_SCK_PIN   12
#define SPIBUS_SCK_PORT  NUTGPIO_PORTE
#else
#warning  Unknown SPIBUS4_SCK_PIN
#endif

#if  SPIBUS4_MISO_PIN == 5 || !defined(SPIBUS4_MISO_PIN)
#define SPIBUS_MISO_PIN 5
#define SPIBUS_MISO_PORT  NUTGPIO_PORTE
#elif defined(STM32F411) && SPIBUS4_MISO_PIN == 11
#define SPIBUS_MISO_PIN   11
#define SPIBUS_MISO_PORT  NUTGPIO_PORTA
#define SPIBUS_MISO_AF    6
#elif (defined(STM32F401) || defined(STM32F42X)) && SPIBUS4_MISO_PIN == 13
#define SPIBUS_MISO_PIN   13
#define SPIBUS_MISO_PORT  NUTGPIO_PORTE
#else
#warning  Unknown SPIBUS4_MISO_PIN
#endif

#if  SPIBUS4_MOSI_PIN == 6 || !defined(SPIBUS4_MOSI_PIN)
#define SPIBUS_MOSI_PIN 6
#define SPIBUS_MOSI_PORT  NUTGPIO_PORTE
#elif defined(STM32F411) && SPIBUS4_MOSI_PIN == 1
#define SPIBUS_MOSI_PIN   1
#define SPIBUS_MOSI_PORT  NUTGPIO_PORTA
#define SPIBUS_MOSI_AF    6
#elif (defined(STM32F401) || defined(STM32F42X)) && SPIBUS4_MOSI_PIN == 14
#define SPIBUS_MOSI_PIN   14
#define SPIBUS_MOSI_PORT  NUTGPIO_PORTE
#else
#warning  Unknown SPIBUS4_MOSI_PIN
#endif

#define SPI_GPIO_AF GPIO_AF_SPI4
#define SPI_ENABLE_CLK_SET() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR, _BI32(RCC_APB2ENR_SPI4EN))
#define SPI_ENABLE_CLK_GET() CM3BBGET(RCC_BASE, RCC_TypeDef, APB2ENR, _BI32(RCC_APB2ENR_SPI4EN))
#define sig_SPI     sig_SPI4
#define SPI_BASE    SPI4_BASE

#if !defined(SPIBUS4_MODE)
#define SPIBUS_MODE IRQ_MODE
#else
#define SPIBUS_MODE SPIBUS4_MODE
#endif

#if SPIBUS_MODE == DMA_MODE
 #if defined(SPIBUS1_DMA_TX_ALTERNATE_STREAM)
  #define SPI_DMA_TX_CHANNEL SPI4_TX_ALT_DMA
  #define sig_SPI_DMA_TX     SPI4_TX_ALT_DMA_IRQ
 #else
  #define SPI_DMA_TX_CHANNEL SPI4_TX_DMA
  #define sig_SPI_DMA_TX     SPI4_TX_DMA_IRQ
 #endif
 #if defined(SPIBUS1_DMA_RX_ALTERNATE_STREAM)
  #define SPI_DMA_RX_CHANNEL SPI4_RX_ALT_DMA
  #define sig_SPI_DMA_RX     SPI4_RX_ALT_DMA_IRQ
 #else
  #define SPI_DMA_RX_CHANNEL SPI4_RX_DMA
  #define sig_SPI_DMA_RX     SPI4_RX_DMA_IRQ
 #endif
#endif

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
