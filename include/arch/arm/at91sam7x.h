#ifndef _ARCH_ARM_SAM7X_H_
#define _ARCH_ARM_SAM7X_H_
/*
 * Copyright (C) 2006 by egnite Software GmbH. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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

/*!
 * Change to sam7x.h
 *     By HU Daoxu, 2006/05/26
 *
 * \file arch/arm/at91.h
 * \brief AT91 peripherals.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.4  2006/08/05 11:57:38  haraldkipp
 * PDC register configuration added.
 * Added register definitions for synchronous serial controller.
 *
 * Revision 1.3  2006/07/21 09:03:56  haraldkipp
 * Added SPI support, kindly contributed by Andras Albert.
 *
 * Revision 1.2  2006/07/15 11:14:45  haraldkipp
 * Missing base addresses and peripheral IDs added.
 *
 * Revision 1.1  2006/07/05 07:45:28  haraldkipp
 * Split on-chip interface definitions.
 *
 * Revision 1.3  2006/01/05 16:52:49  haraldkipp
 * Baudrate calculation is now based on NutGetCpuClock().
 * The AT91_US_BAUD macro had been marked deprecated.
 *
 * Revision 1.2  2005/11/20 14:44:14  haraldkipp
 * Register offsets added.
 *
 * Revision 1.1  2005/10/24 10:31:13  haraldkipp
 * Moved from parent directory.
 *
 *
 * \endverbatim
 */

#define FLASH_BASE      0x100000UL
#define RAM_BASE        0x200000UL

#define TC_BASE         0xFFFA0000      /*!< \brief Timer/counter base address. */
#define UDP_BASE        0xFFFB0000      /*!< \brief USB device port base address. */
#define TWI_BASE        0xFFFB8000      /*!< \brief Two-wire interface base address. */
#define USART0_BASE     0xFFFC0000      /*!< \brief USART 0 base address. */
#define USART1_BASE     0xFFFC4000      /*!< \brief USART 1 base address. */
#define PWMC_BASE       0xFFFCC000      /*!< \brief PWM controller base address. */
#define CAN_BASE        0xFFFD0000      /*!< \brief CAN controller base address. */
#define SSC_BASE        0xFFFD4000      /*!< \brief Serial synchronous controller base address. */
#define ADC_BASE        0xFFFD8000      /*!< \brief ADC base address. */
#define EMAC_BASE       0xFFFDC000      /*!< \brief EMAC base address. */
#define SPI0_BASE       0xFFFE0000      /*!< \brief SPI0 base address. */
#define SPI1_BASE       0xFFFE4000      /*!< \brief SPI0 base address. */

#define AIC_BASE        0xFFFFF000      /*!< \brief AIC base address. */
#define DBGU_BASE       0xFFFFF200      /*!< \brief DBGU base address. */
#define PIOA_BASE       0xFFFFF400      /*!< \brief PIO A base address. */
#define PIOB_BASE       0xFFFFF600      /*!< \brief PIO B base address. */
#define PMC_BASE        0xFFFFFC00      /*!< \brief PMC base address. */
#define RSTC_BASE       0xFFFFFD00      /*!< \brief Resect controller register base address. */
#define RTT_BASE        0xFFFFFD20      /*!< \brief Realtime timer base address. */
#define PIT_BASE        0xFFFFFD30      /*!< \brief Periodic interval timer base address. */
#define WDT_BASE        0xFFFFFD40      /*!< \brief Watch Dog register base address. */
#define VREG_BASE       0xFFFFFD60      /*!< \brief Voltage regulator mode controller base address. */
#define MC_BASE         0xFFFFFF00      /*!< \brief Memory controller base. */

#define PERIPH_RPR_OFF  0x00000100      /*!< \brief Receive pointer register offset. */
#define PERIPH_RCR_OFF  0x00000104      /*!< \brief Receive counter register offset. */
#define PERIPH_TPR_OFF  0x00000108      /*!< \brief Transmit pointer register offset. */
#define PERIPH_TCR_OFF  0x0000010C      /*!< \brief Transmit counter register offset. */
#define PERIPH_RNPR_OFF 0x00000110      /*!< \brief Receive next pointer register offset. */
#define PERIPH_RNCR_OFF 0x00000114      /*!< \brief Receive next counter register offset. */
#define PERIPH_TNPR_OFF 0x00000118      /*!< \brief Transmit next pointer register offset. */
#define PERIPH_TNCR_OFF 0x0000011C      /*!< \brief Transmit next counter register offset. */
#define PERIPH_PTCR_OFF 0x00000120      /*!< \brief PDC transfer control register offset. */
#define PERIPH_PTSR_OFF 0x00000124      /*!< \brief PDC transfer status register offset. */

#define PDC_RXTEN       0x00000001      /*!< \brief Receiver transfer enable. */
#define PDC_RXTDIS      0x00000002      /*!< \brief Receiver transfer disable. */
#define PDC_TXTEN       0x00000100      /*!< \brief Transmitter transfer enable. */
#define PDC_TXTDIS      0x00000200      /*!< \brief Transmitter transfer disable. */

#define DBGU_HAS_PDC
#define SPI_HAS_PDC
#define SSC_HAS_PDC
#define USART_HAS_PDC

#include <arch/arm/at91_tc.h>
#include <arch/arm/at91_us.h>
#include <arch/arm/at91_dbgu.h>
#include <arch/arm/at91_emac.h>
#include <arch/arm/at91_spi.h>
#include <arch/arm/at91_aic.h>
#include <arch/arm/at91_pio.h>
#include <arch/arm/at91_pmc.h>
#include <arch/arm/at91_rstc.h>
#include <arch/arm/at91_wdt.h>
#include <arch/arm/at91_mc.h>
#include <arch/arm/at91_ssc.h>

/*! \addtogroup xgNutArchArmAt91 */
/*@{*/

/*! \name Peripheral Identifiers and Interrupts */
/*@{*/
#define FIQ_ID      0       /*!< \brief Fast interrupt ID. */
#define SYSIRQ_ID   1       /*!< \brief System interrupt ID. */
#define PIOA_ID     2       /*!< \brief Parallel I/O controller ID. */
#define PIOB_ID     3       /*!< \brief Parallel I/O controller ID. */
#define SPI0_ID     4       /*!< \brief Serial peripheral interface 0 ID. */
#define SPI1_ID     5       /*!< \brief Serial peripheral interface 1 ID. */
#define US0_ID      6       /*!< \brief USART 0 ID. */
#define US1_ID      7       /*!< \brief USART 1 ID. */
#define SSC_ID      8       /*!< \brief Synchronous serial controller ID. */
#define TWI_ID      9       /*!< \brief Two-wire interface ID. */
#define PWMC_ID     10      /*!< \brief PWM controller ID. */
#define UDP_ID      11      /*!< \brief USB device port ID. */
#define TC0_ID      12      /*!< \brief Timer 0 ID. */
#define TC1_ID      13      /*!< \brief Timer 1 ID. */
#define TC2_ID      14      /*!< \brief Timer 2 ID. */
#define CAN_ID      15      /*!< \brief CAN controller ID. */
#define EMAC_ID     16      /*!< \brief Ethernet MAC ID. */
#define ADC_ID      17      /*!< \brief Analog to digital converter ID. */
#define IRQ0_ID     30      /*!< \brief External interrupt 0 ID. */
#define IRQ1_ID     31      /*!< \brief External interrupt 1 ID. */
/*@}*/

/*! \name SPI0 peripheral multiplexing */
/*@{*/
#define SPI0_NPCS0_PA12A	12	/*!< \brief Port bit number on PIO-A Perpheral A. */
#define SPI0_NPCS1_PA13A	13	/*!< \brief Port bit number on PIO-A Perpheral A. */
#define SPI0_NPCS1_PA07B	7	/*!< \brief Port bit number on PIO-A Perpheral B. */
#define SPI0_NPCS1_PB13B	13	/*!< \brief Port bit number on PIO-B Perpheral B. */
#define SPI0_NPCS2_PA14A	14	/*!< \brief Port bit number on PIO-A Perpheral A. */
#define SPI0_NPCS2_PA08B	8	/*!< \brief Port bit number on PIO-A Perpheral B. */
#define SPI0_NPCS2_PB14B	14	/*!< \brief Port bit number on PIO-B Perpheral B. */
#define SPI0_NPCS3_PA15A	15	/*!< \brief Port bit number on PIO-A Perpheral A. */
#define SPI0_NPCS3_PA09B	9	/*!< \brief Port bit number on PIO-A Perpheral B. */
#define SPI0_NPCS3_PB17B	17	/*!< \brief Port bit number on PIO-B Perpheral B. */
#define SPI0_MISO_PA16A		16	/*!< \brief Port bit number on PIO-A Perpheral A. */
#define SPI0_MOSI_PA17A		17	/*!< \brief Port bit number on PIO-A Perpheral A. */
#define SPI0_SPCK_PA18A		18	/*!< \brief Port bit number on PIO-A Perpheral A. */
/*@}*/

/*@} xgNutArchArmAt91 */

#endif                          /* _ARCH_ARM_AT91SAM7X_H_ */
