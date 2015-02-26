/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * Copyright (C) 2010 by Nikolaj Zamotaev. All rights reserved.
 * Copyright (C) 2014 by Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de
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

#include <cfg/arch.h>
#include <sys/timer.h>
#include <cfg/spi.h>
#include <cfg/arch/gpio.h>
#include <dev/spibus.h>
#include <dev/gpio.h>

#include <arch/cm3/stm/stm32_gpio.h>
#include <arch/cm3/stm/stm32_dma.h>
#include <arch/cm3/stm/stm32_spi.h>
#include <dev/irqreg.h>
#include <sys/event.h>
#include <sys/nutdebug.h>

#include <stdlib.h>
#include <errno.h>

/* Handle the PIN remap possibilities
 * F1:      NSS:  PA4/PA15
 *          SCK:  PA5/PB3
 *          MISO: PA6/PB4
 *          MOSI: PA7/PB5
 * All:
 *          NSS:  PA4/PA15
 *          SCK:  PA5/PB3
 *          MISO: PA6/PB4
 *          MOSI: PA7/PB5
 * Remap:
 * F03
 *          NSS:  PB12
 *          SCK:  PB13
 *          MISO: PB14
 *          MOSI: PB15
 * F07/L1XX
 *          NSS:  PE12
 *          SCK:  PE13
 *          MISO: PE14
 *          MOSI: PE15
 * F05/L1:  MOSI: PA12
 * F37X:
 *          NSS:  PA11/PC6
 *          SCK:  PA12/PC7
 *          MISO: PA13/PC8
 *          MOSI: PB0/ PC9/PF6

 * Use PA4 as default chip select
 */

#if !defined( SPIBUS1_NO_CS)
 #if !defined(SPIBUS1_CS0_PORT) && !defined(SPIBUS1_CS0_PIN)
  #define SPIBUS_CS0_PORT NUTGPIO_PORTA
  #define SPIBUS_CS0_PIN  4
 #elif !defined(SPIBUS1_CS0_PORT) || !defined(SPIBUS1_CS0_PIN)
  #warning "SPIBUS1 uncomplete chip select"
 #else
  #define SPIBUS_CS0_PORT SPIBUS1_CS0_PORT
  #define SPIBUS_CS0_PIN  SPIBUS1_CS0_PIN
 #endif

 #if defined(SPIBUS1_CS1_PORT) && defined(SPIBUS1_CS1_PIN)
  #define SPIBUS_CS1_PORT SPIBUS1_CS1_PORT
  #define SPIBUS_CS1_PIN  SPIBUS1_CS1_PIN
 #endif
 #if defined(SPIBUS1_CS2_PORT) && defined(SPIBUS1_CS2_PIN)
  #define SPIBUS_CS2_PORT SPIBUS1_CS2_PORT
  #define SPIBUS_CS2_PIN  SPIBUS1_CS2_PIN
 #endif
 #if defined(SPIBUS1_CS3_PORT) && defined(SPIBUS1_CS3_PIN)
  #define SPIBUS_CS3_PORT SPIBUS1_CS3_PORT
  #define SPIBUS_CS3_PIN  SPIBUS1_CS3_PIN
 #endif

#endif

#if defined(MCU_STM32F1)
 #if defined(SPIBUS3_REMAP_SPI)
  #define SPIBUS_REMAP_BB() CM3BBSET(AFIO_BASE, AFIO_TypeDef, MAPR, _BI32(AFIO_MAPR_SPI1_REMAP))
  #define SPIBUS_SCK_PIN 3
  #define SPIBUS_SCK_PORT NUTGPIO_PORTB
  #define SPIBUS_MISO_PIN 4
  #define SPIBUS_MISO_PORT NUTGPIO_PORTB
  #define SPIBUS_MOSI_PIN 5
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
 #else
  #define SPIBUS_REMAP_BB() CM3BBCLR(AFIO_BASE, AFIO_TypeDef, MAPR, _BI32(AFIO_MAPR_SPI1_REMAP))
  #define SPIBUS_SCK_PIN 5
  #define SPIBUS_SCK_PORT NUTGPIO_PORTA
  #define SPIBUS_MISO_PIN 6
  #define SPIBUS_MISO_PORT NUTGPIO_PORTA
  #define SPIBUS_MOSI_PIN 7
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTA
 #endif
#else

#if SPIBUS1_SCK_PIN == 5 || !defined(SPIBUS1_SCK_PIN)
 #define SPIBUS_SCK_PORT NUTGPIO_PORTA
 #define  SPIBUS_SCK_PIN  5
#elif SPIBUS1_SCK_PIN == 3
 #define SPIBUS_SCK_PORT NUTGPIO_PORTB
 #define SPIBUS_SCK_PIN  3
#elif defined(STM32F37X) && SPIBUS1_SCK_PIN == 12
 #define SPIBUS_SCK_PORT NUTGPIO_PORTA
 #define SPIBUS_SCK_PIN  12
#elif defined(STM32F37X) && SPIBUS1_SCK_PIN == 7
 #define SPIBUS_SCK_PORT NUTGPIO_PORTC
 #define SPIBUS_SCK_PIN  7
#elif defined(STM32L1XX) && SPIBUS1_SCK_PIN == 13
 #define SPIBUS_SCK_PORT NUTGPIO_PORTE
 #define SPIBUS_SCK_PIN  13
#elif defined(STM32F03X) && SPIBUS1_SCK_PIN == 13
 #define SPIBUS_SCK_PORT NUTGPIO_PORTB
 #define SPIBUS_SCK_PIN  13
#else
 #warning Unknown SPIBUS1_SCK_PIN
#endif

#if SPIBUS1_MISO_PIN == 6 || !defined(SPIBUS1_MISO_PIN)
 #define SPIBUS_MISO_PORT NUTGPIO_PORTA
 #define  SPIBUS_MISO_PIN  6
#elif SPIBUS1_MISO_PIN == 4
 #define SPIBUS_MISO_PORT NUTGPIO_PORTB
 #define SPIBUS_MISO_PIN  4
#elif defined(STM32F37X) && SPIBUS1_MISO_PIN == 13
 #define SPIBUS_MISO_PORT NUTGPIO_PORTA
 #define SPIBUS_MISO_PIN  13
#elif defined(STM32F37X) && SPIBUS1_MISO_PIN == 8
 #define SPIBUS_MISO_PORT NUTGPIO_PORTC
 #define SPIBUS_MISO_PIN  8
#elif defined(STM32L1XX) && SPIBUS1_MISO_PIN == 14
 #define SPIBUS_MISO_PORT NUTGPIO_PORTE
 #define SPIBUS_MISO_PIN  14
#elif defined(STM32F03X) && SPIBUS1_MISO_PIN == 14
 #define SPIBUS_MISO_PORT NUTGPIO_PORTB
 #define SPIBUS_MISO_PIN  14
#else
 #warning Unknown SPIBUS1_MISO_PIN
#endif

#if SPIBUS1_MOSI_PIN == 7 || !defined(SPIBUS1_MOSI_PIN)
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTA
 #define  SPIBUS_MOSI_PIN  7
#elif SPIBUS1_MOSI_PIN == 7
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
 #define SPIBUS_MOSI_PIN  7
#elif defined(STM32F37X) && SPIBUS1_MOSI_PIN == 0
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
 #define SPIBUS_MOSI_PIN  0
#elif defined(STM32F37X) && SPIBUS1_MOSI_PIN == 9
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTC
 #define SPIBUS_MOSI_PIN  9
#elif defined(STM32F37X) && SPIBUS1_MOSI_PIN == 6
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTF
 #define SPIBUS_MOSI_PIN  6
#elif defined(STM32L1XX) && SPIBUS1_MOSI_PIN == 15
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTE
 #define SPIBUS_MOSI_PIN  15
#elif defined(STM32F03X) && SPIBUS1_MOSI_PIN == 15
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
 #define SPIBUS_MOSI_PIN  15
#else
 #warning Unknown SPIBUS1_MOSI_PIN
#endif
#endif

#define SPI_REMAP GPIO_Remap_SPI1
#define SPI_GPIO_AF GPIO_AF_SPI1
#define SPI_ENABLE_CLK_SET() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR, _BI32(RCC_APB2ENR_SPI1EN))
#define SPI_ENABLE_CLK_GET() CM3BBGET(RCC_BASE, RCC_TypeDef, APB2ENR, _BI32(RCC_APB2ENR_SPI1EN))
#define sig_SPI     sig_SPI1
#define SPI_BASE    SPI1_BASE

#if !defined(SPIBUS1_MODE)
#define SPIBUS_MODE IRQ_MODE
#else
#define SPIBUS_MODE SPIBUS1_MODE
#endif

#if SPIBUS_MODE == DMA_MODE
 #if defined(SPIBUS1_DMA_TX_ALTERNATE_STREAM)
  #define SPI_DMA_TX_CHANNEL SPI1_TX_ALT_DMA
  #define sig_SPI_DMA_TX     SPI1_TX_ALT_DMA_IRQ
 #else
  #define SPI_DMA_TX_CHANNEL SPI1_TX_DMA
  #define sig_SPI_DMA_TX     SPI1_TX_DMA_IRQ
 #endif
 #if defined(SPIBUS1_DMA_RX_ALTERNATE_STREAM)
  #define SPI_DMA_RX_CHANNEL SPI1_RX_ALT_DMA
  #define sig_SPI_DMA_RX     SPI1_RX_ALT_DMA_IRQ
 #else
  #define SPI_DMA_RX_CHANNEL SPI1_RX_DMA
  #define sig_SPI_DMA_RX     SPI1_RX_DMA_IRQ
 #endif
#endif

NUTSPIBUS spiBus1Stm32 = {
    NULL,                       /*!< Bus mutex semaphore (bus_mutex). */
    NULL,                       /*!< Bus ready signal (bus_ready). */
    SPI1_BASE,                  /*!< Bus base address (bus_base). */
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
