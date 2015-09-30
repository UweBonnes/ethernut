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
#include <arch/cm3/stm/stm32_spi_pinmux.h>
#include <dev/irqreg.h>
#include <sys/event.h>
#include <sys/nutdebug.h>

#include <stdlib.h>
#include <errno.h>

#if !defined( SPIBUS6_NO_CS)
 #if !defined(SPIBUS6_CS0_PORT) && !defined(SPIBUS6_CS0_PIN)
  #define SPIBUS_CS0_PORT NUTGPIO_PORTG
  #define SPIBUS_CS0_PIN  8
 #elif !defined(SPIBUS6_CS0_PORT) || !defined(SPIBUS6_CS0_PIN)
  #warning "SPIBUS6 uncomplete chip select"
 #else
  #define SPIBUS_CS0_PORT SPIBUS6_CS0_PORT
  #define SPIBUS_CS0_PIN  SPIBUS6_CS0_PIN
 #endif

 #if defined(SPIBUS6_CS1_PORT) && defined(SPIBUS6_CS1_PIN)
  #define SPIBUS_CS1_PORT SPIBUS6_CS1_PORT
  #define SPIBUS_CS1_PIN  SPIBUS6_CS1_PIN
 #endif
 #if defined(SPIBUS6_CS2_PORT) && defined(SPIBUS6_CS2_PIN)
  #define SPIBUS_CS2_PORT SPIBUS6_CS2_PORT
  #define SPIBUS_CS2_PIN  SPIBUS6_CS2_PIN
 #endif
 #if defined(SPIBUS6_CS3_PORT) && defined(SPIBUS6_CS3_PIN)
  #define SPIBUS_CS3_PORT SPIBUS6_CS3_PORT
  #define SPIBUS_CS3_PIN  SPIBUS6_CS3_PIN
 #endif

#endif

#define Stm32F1SpiRemap()

#define SPI_SCK     SPI6_SCK
#define SPI_MISO    SPI6_MISO
#define SPI_MOSI    SPI6_MOSI

#define SPI_SCK_AF  SPI6_SCK_AF
#define SPI_MISO_AF SPI6_MISO_AF
#define SPI_MOSI_AF SPI6_MOSI_AF

#define SPI_ENABLE_CLK_SET() CM3BBSET(RCC_BASE, RCC_TypeDef, APB2ENR, _BI32(RCC_APB2ENR_SPI6EN))
#define SPI_ENABLE_CLK_GET() CM3BBGET(RCC_BASE, RCC_TypeDef, APB2ENR, _BI32(RCC_APB2ENR_SPI6EN))
#define sig_SPI             sig_SPI6
#define SPI_BASE            SPI6_BASE

#if !defined(SPIBUS6_MODE)
#define SPIBUS_MODE IRQ_MODE
#else
#define SPIBUS_MODE SPIBUS6_MODE
#endif

#if SPIBUS_MODE == DMA_MODE
 #define SPI_DMA_TX_CHANNEL SPI6_TX_DMA
 #define SPI_DMA_RX_CHANNEL SPI6_RX_DMA
#endif

NUTSPIBUS spiBus6Stm32 = {
    NULL,                       /*!< Bus mutex semaphore (bus_mutex). */
    NULL,                       /*!< Bus ready signal (bus_ready). */
    SPI6_BASE,                  /*!< Bus base address (bus_base). */
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
