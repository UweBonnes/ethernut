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

#include <arch/cm3.h>
#include <sys/timer.h>
#include <cfg/spi.h>
#include <cfg/arch/gpio.h>
#include <dev/spibus.h>
#include <dev/gpio.h>

#include <arch/cm3/stm/stm32_gpio.h>
#if defined(MCU_STM32F1)
#include <arch/cm3/stm/stm32f1_dma.h>
#endif
#include <arch/cm3/stm/stm32_spi.h>
#include <dev/irqreg.h>
#include <sys/event.h>
#include <sys/nutdebug.h>

#include <stdlib.h>
#include <errno.h>

/* Handle the PIN remap possibilities
 * F1_CL/F2/F3/F4/L1 >= 256 k flash
 *        NSS:  PA15/PA4
 *        SCK:  PB3/PC10
 *        MISO: PB4/PC11
 *        MOSI: PB5/PC12
 * F1: Only STM32F10X_CL can remap
 * F373:  SCK:  PA1
 *        MISO: PA2
 *        MOSI: PA3
 * F411:  SCK:  PB12/AF7
 * F401/F411/F42/F43 : MOSI/PD6/AF5
 * Use PA15 as default chip select
 */

#if !defined( SPIBUS3_NO_CS)
 #if !defined(SPIBUS3_CS0_PORT) && !defined(SPIBUS3_CS0_PIN)
  #define SPIBUS_CS0_PORT NUTGPIO_PORTA
  #define SPIBUS_CS0_PIN  15
 #elif !defined(SPIBUS3_CS0_PORT) || !defined(SPIBUS3_CS0_PIN)
  #warnig "SPIBUS3 uncomplete chip select"
 #else
  #define SPIBUS_CS0_PORT SPIBUS3_CS0_PORT
  #define SPIBUS_CS0_PIN  SPIBUS3_CS0_PIN
 #endif
 #if defined(SPIBUS3_CS1_PORT) && defined(SPIBUS3_CS1_PIN)
  #define SPIBUS_CS1_PORT SPIBUS3_CS1_PORT
  #define SPIBUS_CS1_PIN  SPIBUS3_CS1_PIN
 #endif
 #if defined(SPIBUS3_CS2_PORT) && defined(SPIBUS3_CS2_PIN)
  #define SPIBUS_CS2_PORT SPIBUS3_CS2_PORT
  #define SPIBUS_CS2_PIN  SPIBUS3_CS2_PIN
 #endif
 #if defined(SPIBUS3_CS3_PORT) && defined(SPIBUS3_CS3_PIN)
  #define SPIBUS_CS3_PORT SPIBUS3_CS3_PORT
  #define SPIBUS_CS3_PIN  SPIBUS3_CS3_PIN
 #endif
#endif

#if defined(MCU_STM32F1)
 #if defined(STM32F10X_CL) && defined(SPIBUS3_REMAP_SPI)
  #define SPIBUS_REMAP_BB() CM3BBSET(AFIO_BASE, AFIO_TypeDef, MAPR, _BI32(AFIO_MAPR_SPI3_REMAP))
  #define SPIBUS_SCK_PIN 10
  #define SPIBUS_SCK_PORT NUTGPIO_PORTC
  #define SPIBUS_MISO_PIN 11
  #define SPIBUS_MISO_PORT NUTGPIO_PORTC
  #define SPIBUS_MOSI_PIN 12
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTC
 #else
  #if defined(STM32F10X_CL)
   #define SPIBUS_REMAP_BB() CM3BBCLR(AFIO_BASE, AFIO_TypeDef, MAPR, _BI32(AFIO_MAPR_SPI3_REMAP))
  #else
   #define SPIBUS_REMAP_BB()
  #endif
  #define SPIBUS_SCK_PIN 3
  #define SPIBUS_SCK_PORT NUTGPIO_PORTB
  #define SPIBUS_MISO_PIN 4
  #define SPIBUS_MISO_PORT NUTGPIO_PORTB
  #define SPIBUS_MOSI_PIN 5
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
 #endif
#else
/* Handle SCK */
 #if   defined(MCU_STM32F37) && SPIBUS3_SCK_PIN == 1
  #define SPIBUS_SCK_PIN 1
  #define SPIBUS_SCK_PORT NUTGPIO_PORTA
 #elif SPIBUS3_SCK_PIN == 3 || !defined(SPIBUS3_SCK_PIN )
  #define SPIBUS_SCK_PIN 3
  #define SPIBUS_SCK_PORT NUTGPIO_PORTB
 #elif SPIBUS3_SCK_PIN == 10
  #define SPIBUS_SCK_PIN 10
  #define SPIBUS_SCK_PORT NUTGPIO_PORTC
 #elif defined(STM32F411xE) && SPIBUS3_SCK_PIN == 12
  #define SPIBUS_SCK_PIN 12
  #define SPIBUS_SCK_PORT NUTGPIO_PORTB
  #define SPIBUS_SCK_AF   7
 #else
  #warning Unknown SPIBUS3_SCK_PIN
 #endif

 #if   defined(MCU_STM32F37) && SPIBUS3_MISO_PIN == 2
  #define SPIBUS_MISO_PIN 2
  #define SPIBUS_MISO_PORT NUTGPIO_PORTA
 #elif SPIBUS3_MISO_PIN == 4 || !defined(SPIBUS3_MISO_PIN )
  #define SPIBUS_MISO_PIN 4
  #define SPIBUS_MISO_PORT NUTGPIO_PORTB
 #elif SPIBUS3_MISO_PIN == 11
  #define SPIBUS_MISO_PIN 11
  #define SPIBUS_MISO_PORT NUTGPIO_PORTC
 #else
  #warning Unknown SPIBUS3_MISO_PIN
 #endif

 #if   defined(MCU_STM32F37) && SPIBUS3_MOSI_PIN == 3
  #define SPIBUS_MOSI_PIN 3
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTA
 #elif  SPIBUS3_MOSI_PIN == 5 || !defined(SPIBUS3_MOSI_PIN )
  #define SPIBUS_MOSI_PIN 5
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
 #elif SPIBUS3_MOSI_PIN == 12
  #define SPIBUS_MOSI_PIN 12
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTC
 #elif defined(MCU_STM32F4) && !defined(MCU_STM32F40) || SPIBUS3_MISO_PIN == 6
  #define SPIBUS_MOSI_PIN  6
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTD
  #define SPIBUS_MOSI_AF   5
 #else
  #warning Unknown SPIBUS3_MOSI_PIN
 #endif
#endif

#define SPI_DEV 3
#define SPI_GPIO_AF GPIO_AF_SPI3
#define SPI_ENABLE_CLK_SET() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR, _BI32(RCC_APB1ENR_SPI3EN))
#define SPI_ENABLE_CLK_GET() CM3BBGET(RCC_BASE, RCC_TypeDef, APB1ENR, _BI32(RCC_APB1ENR_SPI3EN))
#define sig_SPI             sig_SPI3
#define SPI_BASE            SPI3_BASE

#if !defined(SPIBUS3_MODE)
#define SPIBUS_MODE IRQ_MODE
#else
#define SPIBUS_MODE SPIBUS3_MODE
#endif

#if SPIBUS_MODE == DMA_MODE
 #if defined(SPIBUS3_DMA_TX_ALTERNATE_STREAM)
  #define SPI_DMA_TX_CHANNEL SPI3_TX_ALT_DMA
 #else
  #define SPI_DMA_TX_CHANNEL SPI3_TX_DMA
 #endif
 #if defined(SPIBUS1_DMA_RX_ALTERNATE_STREAM)
  #define SPI_DMA_RX_CHANNEL SPI3_RX_ALT_DMA
 #else
  #define SPI_DMA_RX_CHANNEL SPI3_RX_DMA
 #endif
#endif

NUTSPIBUS spiBus3Stm32 = {
    NULL,                       /*!< Bus mutex semaphore (bus_mutex). */
    NULL,                       /*!< Bus ready signal (bus_ready). */
    SPI3_BASE,                  /*!< Bus base address (bus_base). */
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
