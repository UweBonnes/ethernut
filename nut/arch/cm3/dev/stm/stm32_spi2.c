/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * Copyright (C) 2010 by Nikolaj Zamotaev. All rights reserved.
 * Copyright (C) 2012-2014 by Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de
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
 * F1:    NSS:  PB12
 *        SCK:  PB13
 *        MISO: PB14
 *        MOSI: PB15
 *  L1:   NSS:  PB12/PD0
 *        SCK:  PB13/PD1
 *        MISO: PB14/PD3
 *        MOSI: PB15/PD4
 * F2/F4: NSS:  PB12/PB9/PI0
 *        SCK:  PB13/PB10/PI1
 *        MISO: PB14/PC2/PI2
 *        MOSI: PB15/PC3/PI3
 * F30x:  NSS:  PB12/PD12
 *        SCK:  PB13/PF9/PF10
 *        MISO: PB14
 *        MOSI: PB15
 *
 * F373:  NSS:  PA11/PB9/PD6
 *        SCK:  PA8/PB8/PB10/PD7/PD8
 *        MISO: PA9/PB14/PC2/PD3
 *        MOSI: PA10/PB15/PC3/PD4/
 *
 * Use PB12 or on F373 PA11 as default chip select
 */

#if !defined( SPIBUS2_NO_CS)
#if !defined(SPIBUS2_CS0_PORT) && !defined(SPIBUS2_CS0_PIN)
#if defined(MCU_STM32F37X)
#define SPIBUS_CS0_PORT NUTGPIO_PORTA
#define SPIBUS_CS0_PIN  11
#else
#define SPIBUS_CS0_PORT NUTGPIO_PORTB
#define SPIBUS_CS0_PIN  12
#endif
#elif !defined(SPIBUS2_CS0_PORT) || !defined(SPIBUS2_CS0_PIN)
#warning "SPIBUS2 uncomplete chip select"
#else
#define SPIBUS_CS0_PORT SPIBUS2_CS0_PORT
#define SPIBUS_CS0_PIN  SPIBUS2_CS0_PIN
#endif

#if defined(SPIBUS2_CS1_PORT)
#define SPIBUS_CS1_PORT SPIBUS2_CS1_PORT
#endif
#if defined(SPIBUS2_CS2_PORT)
#define SPIBUS_CS2_PORT SPIBUS2_CS2_PORT
#endif
#if defined(SPIBUS2_CS3_PORT)
#define SPIBUS_CS3_PORT SPIBUS2_CS3_PORT
#endif
#if defined(SPIBUS2_CS1_PIN)
#define SPIBUS_CS1_PIN  SPIBUS2_CS1_PIN
#endif
#if defined(SPIBUS2_CS2_PIN)
#define SPIBUS_CS2_PIN  SPIBUS2_CS2_PIN
#endif
#if defined(PIBUS2_CS2_PIN)
#define SPIBUS_CS3_PIN  SPIBUS2_CS3_PIN
#endif

#endif

#if defined(MCU_STM32F1)
 #undef SPIBUS_REMAP_BB
 #define SPIBUS_SCK_PIN 13
 #define SPIBUS_SCK_PORT NUTGPIO_PORTB
 #define SPIBUS_MISO_PIN 14
 #define SPIBUS_MISO_PORT NUTGPIO_PORTB
 #define SPIBUS_MOSI_PIN 15
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
#elif defined(MCU_STM32F37X)
 #if SPIBUS2_SCK_PIN == 8 || !defined(SPIBUS2_SCK_PIN)
 #define SPIBUS_SCK_PORT NUTGPIO_PORTA
 #define SPIBUS_SCK_PIN  8
 #elif SPIBUS2_SCK_PIN == 208
 #define SPIBUS_SCK_PORT NUTGPIO_PORTB
 #define SPIBUS_SCK_PIN  8
 #elif SPIBUS2_SCK_PIN == 408
 #define SPIBUS_SCK_PORT NUTGPIO_PORTD
 #define SPIBUS_SCK_PIN  8
 #elif SPIBUS2_SCK_PIN == 7
 #define SPIBUS_SCK_PORT NUTGPIO_PORTD
 #define SPIBUS_SCK_PIN  7
 #elif SPIBUS2_SCK_PIN == 10
 #define SPIBUS_SCK_PORT NUTGPIO_PORTB
 #define SPIBUS_SCK_PIN  10
 #else
 #warning "Illegal STM32F373 SPI2 SCK assignment"
 #endif
 #if SPIBUS2_MOSI_PIN == 10 || !defined(SPIBUS2_MOSI_PIN)
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTA
 #define SPIBUS_MOSI_PIN  10
 #elif SPIBUS2_MOSI_PIN == 15
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
 #define SPIBUS_MOSI_PIN  15
 #elif SPIBUS2_MOSI_PIN == 3
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTC
 #define SPIBUS_MOSI_PIN  3
 #elif SPIBUS2_MOSI_PIN == 4
 #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
 #define SPIBUS_MOSI_PIN  4
 #else
 #warning "Illegal STM32F373 SPI2 MOSI assignment"
 #endif
 #if SPIBUS2_MISO_PIN == 9 || !defined(SPIBUS2_MISO_PIN)
 #define SPIBUS_MISO_PORT NUTGPIO_PORTA
 #define SPIBUS_MISO_PIN  9
 #elif SPIBUS2_MISO_PIN == 14
 #define SPIBUS_MISO_PORT NUTGPIO_PORTB
 #define SPIBUS_MISO_PIN  14
 #elif SPIBUS2_MISO_PIN == 2
 #define SPIBUS_MISO_PORT NUTGPIO_PORTC
 #define SPIBUS_MISO_PIN  2
 #elif SPIBUS2_MISO_PIN == 3
 #define SPIBUS_MISO_PORT NUTGPIO_PORTD
 #define SPIBUS_MISO_PIN  3
 #else
 #warning "Illegal STM32F373 SPI2 MISO assignment"
 #endif
#else
 #if !defined(SPIBUS2_SCK_PIN)
  #define SPIBUS_SCK_PORT NUTGPIO_PORTB
  #define SPIBUS_SCK_PIN 13
 #elif SPIBUS2_SCK_PIN == 13
  #define SPIBUS_SCK_PORT NUTGPIO_PORTB
  #define SPIBUS_SCK_PIN 13
 #elif (defined(MCU_STM32F2) || defined(MCU_STM32F4)) && SPIBUS2_SCK_PIN == 10
  #define SPIBUS_SCK_PORT NUTGPIO_PORTB
  #define SPIBUS_SCK_PIN 10
 #elif (defined(MCU_STM32F2) || defined(MCU_STM32F4)) && SPIBUS2_SCK_PIN == 1
  #define SPIBUS_SCK_PORT NUTGPIO_PORTD
  #define SPIBUS_SCK_PIN 1
 #elif defined(MCU_STM32L1) && SPIBUS2_SCK_PIN == 1
  #define SPIBUS_SCK_PORT NUTGPIO_PORTD
  #define SPIBUS_SCK_PIN 1
 #elif (defined(MCU_STM32F2) || defined(MCU_STM32F4)) && SPIBUS2_SCK_PIN == 901
  #undef SPIBUS2_SCK_PIN
  #define SPIBUS_SCK_PIN 1
  #define SPIBUS_SCK_PORT NUTGPIO_PORTI
 #else
  #warning "Illegal I2C2 SCK pin assignement"
 #endif
 #if !defined(SPIBUS2_MISO_PIN)
  #define SPIBUS_MISO_PIN 14
  #define SPIBUS_MISO_PORT NUTGPIO_PORTB
 #elif  SPIBUS2_MISO_PIN == 14
  #define SPIBUS_MISO_PIN 14
  #define SPIBUS_MISO_PORT NUTGPIO_PORTB
 #elif defined(MCU_STM32L1) && SPIBUS2_MISO_PIN == 3
  #define SPIBUS_MISO_PIN 3
  #define SPIBUS_MISO_PORT NUTGPIO_PORTD
 #elif (defined(MCU_STM32F2) || defined(MCU_STM32F4)) && SPIBUS2_MISO_PIN == 2
  #define SPIBUS_MISO_PIN 2
  #define SPIBUS_MISO_PORT NUTGPIO_PORTC
 #elif (defined(MCU_STM32F2) || defined(MCU_STM32F4)) && SPIBUS2_MISO_PIN == 902
  #undef SPIBUS2_MISO_PIN
  #define SPIBUS_MISO_PIN 2
  #define SPIBUS_MISO_PORT NUTGPIO_PORTI
 #else
  #warning "Illegal I2C2 MISO pin assignement"
 #endif
 #if !defined(SPIBUS2_MOSI_PIN)
  #define SPIBUS_MOSI_PIN 15
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
 #elif SPIBUS2_MOSI_PIN == 15
  #define SPIBUS_MOSI_PIN 15
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTB
 #elif defined(MCU_STM32L1) && SPIBUS2_MOSI_PIN == 4
  #define SPIBUS_MOSI_PIN 4
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTD
 #elif (defined(MCU_STM32F2) || defined(MCU_STM32F4)) && SPIBUS2_MOSI_PIN == 3
  #define SPIBUS_MOSI_PIN 3
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTC
 #elif (defined(MCU_STM32F2) || defined(MCU_STM32F4)) && SPIBUS2_MOSI_PIN == 903
  #undef SPIBUS2_MOSI_PIN
  #define SPIBUS_MOSI_PIN 3
  #define SPIBUS_MOSI_PORT NUTGPIO_PORTI
 #else
  #warning "Illegal I2C2 MOSI pin assignement"
 #endif
#endif
#define SPI_DEV 2
#define SPI_GPIO_AF GPIO_AF_SPI2
#define SPI_ENABLE_CLK_SET() CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR, _BI32(RCC_APB1ENR_SPI2EN))
#define SPI_ENABLE_CLK_GET() CM3BBGET(RCC_BASE, RCC_TypeDef, APB1ENR, _BI32(RCC_APB1ENR_SPI2EN))
#define sig_SPI             sig_SPI2
#define SPI_BASE            SPI2_BASE

#if !defined(SPIBUS2_MODE)
#define SPIBUS_MODE IRQ_MODE
#else
#define SPIBUS_MODE SPIBUS2_MODE
#endif

#if SPIBUS_MODE == DMA_MODE
 #define SPI_DMA_TX_CHANNEL SPI2_TX_DMA
 #define sig_SPI_DMA_TX     SPI2_TX_DMA_IRQ
 #define SPI_DMA_RX_CHANNEL SPI2_RX_DMA
 #define sig_SPI_DMA_RX     SPI2_RX_DMA_IRQ
#endif

NUTSPIBUS spiBus2Stm32 = {
    NULL,                       /*!< Bus mutex semaphore (bus_mutex). */
    NULL,                       /*!< Bus ready signal (bus_ready). */
    SPI2_BASE,                  /*!< Bus base address (bus_base). */
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
