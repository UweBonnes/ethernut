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

#define TC_BASE         0xFFFA0000      /*!< \brief TC base address. */
#define USART1_BASE     0xFFFC4000      /*!< \brief USART 1 base address. */
#define USART0_BASE     0xFFFC0000      /*!< \brief USART 0 base address. */
#define DBGU_BASE       0xFFFFF200      /*!< \brief DBGU base address. */
#define EMAC_BASE       0xFFFDC000      /*!< \brief EMAC base address. */
#define AIC_BASE        0xFFFFF000      /*!< \brief AIC base address. */
#define PIOA_BASE       0xFFFFF400      /*!< \brief PIO A base address. */
#define PIOB_BASE       0xFFFFF600      /*!< \brief PIO B base address. */
#define PMC_BASE        0xFFFFFC00      /*!< \brief PMC base address. */
#define RSTC_BASE       0xFFFFFD00      /*!< \brief Resect controller register base address. */
#define WDT_BASE        0xFFFFFD40      /*!< \brief Watch Dog register base address. */
#define MC_BASE         0xFFFFFF00      /*!< \brief Memory controller base. */

#include <arch/arm/at91_tc.h>
#include <arch/arm/at91_us.h>
#include <arch/arm/at91_dbgu.h>
#include <arch/arm/at91_emac.h>
#include <arch/arm/at91_aic.h>
#include <arch/arm/at91_pio.h>
#include <arch/arm/at91_pmc.h>
#include <arch/arm/at91_rstc.h>
#include <arch/arm/at91_wdt.h>
#include <arch/arm/at91_mc.h>

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
#define EMAC_ID     16      /*!< \brief Ethernet MAC ID. */
#define ADC_ID      17      /*!< \brief Analog to digital converter ID. */
#define IRQ0_ID     30      /*!< \brief External interrupt 0 ID. */
#define IRQ1_ID     31      /*!< \brief External interrupt 1 ID. */
/*@}*/
/*@} xgNutArchArmAt91 */

#endif                          /* _ARCH_ARM_AT91SAM7X_H_ */
