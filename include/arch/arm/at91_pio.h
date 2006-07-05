#ifndef _ARCH_ARM_AT91_PIO_H_
#define _ARCH_ARM_AT91_PIO_H_

/*
 * Copyright (C) 2005-2006 by egnite Software GmbH. All rights reserved.
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
 * \file arch/arm/at91_pio.h
 * \brief AT91 peripherals.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/07/05 07:45:28  haraldkipp
 * Split on-chip interface definitions.
 *
 *
 * \endverbatim
 */

/*!
 * \addtogroup xgNutArchArmAt91Pio
 */
/*@{*/

#if defined(PIO_BASE)
#define PIO_PER     (PIO_BASE + 0x00)   /*!< \brief PIO enable register. */
#define PIO_PDR     (PIO_BASE + 0x04)   /*!< \brief PIO disable register. */
#define PIO_PSR     (PIO_BASE + 0x08)   /*!< \brief PIO status register. */
#define PIO_OER     (PIO_BASE + 0x10)   /*!< \brief Output enable register. */
#define PIO_ODR     (PIO_BASE + 0x14)   /*!< \brief Output disable register. */
#define PIO_OSR     (PIO_BASE + 0x18)   /*!< \brief Output status register. */
#define PIO_IFER    (PIO_BASE + 0x20)   /*!< \brief Input filter enable register. */
#define PIO_IFDR    (PIO_BASE + 0x24)   /*!< \brief Input filter disable register. */
#define PIO_IFSR    (PIO_BASE + 0x28)   /*!< \brief Input filter status register. */
#define PIO_SODR    (PIO_BASE + 0x30)   /*!< \brief Set output data register. */
#define PIO_CODR    (PIO_BASE + 0x34)   /*!< \brief Clear output data register. */
#define PIO_ODSR    (PIO_BASE + 0x38)   /*!< \brief Output data status register. */
#define PIO_PDSR    (PIO_BASE + 0x3C)   /*!< \brief Pin data status register. */
#define PIO_IER     (PIO_BASE + 0x40)   /*!< \brief Interrupt enable register. */
#define PIO_IDR     (PIO_BASE + 0x44)   /*!< \brief Interrupt disable register. */
#define PIO_IMR     (PIO_BASE + 0x48)   /*!< \brief Interrupt mask register. */
#define PIO_ISR     (PIO_BASE + 0x4C)   /*!< \brief Interrupt status register. */
#endif

#if defined(PIOA_BASE)
#define PIOA_PER    (PIOA_BASE + 0x00)  /*!< \brief PIO enable register. */
#define PIOA_PDR    (PIOA_BASE + 0x04)  /*!< \brief PIO disable register. */
#define PIOA_PSR    (PIOA_BASE + 0x08)  /*!< \brief PIO status register. */
#define PIOA_OER    (PIOA_BASE + 0x10)  /*!< \brief Output enable register. */
#define PIOA_ODR    (PIOA_BASE + 0x14)  /*!< \brief Output disable register. */
#define PIOA_OSR    (PIOA_BASE + 0x18)  /*!< \brief Output status register. */
#define PIOA_IFER   (PIOA_BASE + 0x20)  /*!< \brief Input filter enable register. */
#define PIOA_IFDR   (PIOA_BASE + 0x24)  /*!< \brief Input filter disable register. */
#define PIOA_IFSR   (PIOA_BASE + 0x28)  /*!< \brief Input filter status register. */
#define PIOA_SODR   (PIOA_BASE + 0x30)  /*!< \brief Set output data register. */
#define PIOA_CODR   (PIOA_BASE + 0x34)  /*!< \brief Clear output data register. */
#define PIOA_ODSR   (PIOA_BASE + 0x38)  /*!< \brief Output data status register. */
#define PIOA_PDSR   (PIOA_BASE + 0x3C)  /*!< \brief Pin data status register. */
#define PIOA_IER    (PIOA_BASE + 0x40)  /*!< \brief Interrupt enable register. */
#define PIOA_IDR    (PIOA_BASE + 0x44)  /*!< \brief Interrupt disable register. */
#define PIOA_IMR    (PIOA_BASE + 0x48)  /*!< \brief Interrupt mask register. */
#define PIOA_ISR    (PIOA_BASE + 0x4C)  /*!< \brief Interrupt status register. */
#define PIOA_MDER   (PIOA_BASE + 0x50)  /*!< \brief Multi-driver enable register. */
#define PIOA_MDDR   (PIOA_BASE + 0x54)  /*!< \brief Multi-driver disable register. */
#define PIOA_MDSR   (PIOA_BASE + 0x58)  /*!< \brief Multi-driver status register. */
#define PIOA_PUDR   (PIOA_BASE + 0x60)  /*!< \brief Pull-up disable register. */
#define PIOA_PUER   (PIOA_BASE + 0x64)  /*!< \brief Pull-up enable register. */
#define PIOA_PUSR   (PIOA_BASE + 0x68)  /*!< \brief Pull-up status register. */
#define PIOA_ASR    (PIOA_BASE + 0x70)  /*!< \brief PIO peripheral A select register. */
#define PIOA_BSR    (PIOA_BASE + 0x74)  /*!< \brief PIO peripheral B select register. */
#define PIOA_ABSR   (PIOA_BASE + 0x78)  /*!< \brief PIO peripheral AB status register. */
#define PIOA_OWER   (PIOA_BASE + 0xA0)  /*!< \brief PIO output write enable register. */
#define PIOA_OWDR   (PIOA_BASE + 0xA4)  /*!< \brief PIO output write disable register. */
#define PIOA_OWSR   (PIOA_BASE + 0xA8)  /*!< \brief PIO output write status register. */
#endif

#if defined(PIOB_BASE)
#define PIOB_PER    (PIOB_BASE + 0x00)  /*!< \brief PIO enable register. */
#define PIOB_PDR    (PIOB_BASE + 0x04)  /*!< \brief PIO disable register. */
#define PIOB_PSR    (PIOB_BASE + 0x08)  /*!< \brief PIO status register. */
#define PIOB_OER    (PIOB_BASE + 0x10)  /*!< \brief Output enable register. */
#define PIOB_ODR    (PIOB_BASE + 0x14)  /*!< \brief Output disable register. */
#define PIOB_OSR    (PIOB_BASE + 0x18)  /*!< \brief Output status register. */
#define PIOB_IFER   (PIOB_BASE + 0x20)  /*!< \brief Input filter enable register. */
#define PIOB_IFDR   (PIOB_BASE + 0x24)  /*!< \brief Input filter disable register. */
#define PIOB_IFSR   (PIOB_BASE + 0x28)  /*!< \brief Input filter status register. */
#define PIOB_SODR   (PIOB_BASE + 0x30)  /*!< \brief Set output data register. */
#define PIOB_CODR   (PIOB_BASE + 0x34)  /*!< \brief Clear output data register. */
#define PIOB_ODSR   (PIOB_BASE + 0x38)  /*!< \brief Output data status register. */
#define PIOB_PDSR   (PIOB_BASE + 0x3C)  /*!< \brief Pin data status register. */
#define PIOB_IER    (PIOB_BASE + 0x40)  /*!< \brief Interrupt enable register. */
#define PIOB_IDR    (PIOB_BASE + 0x44)  /*!< \brief Interrupt disable register. */
#define PIOB_IMR    (PIOB_BASE + 0x48)  /*!< \brief Interrupt mask register. */
#define PIOB_ISR    (PIOB_BASE + 0x4C)  /*!< \brief Interrupt status register. */
#define PIOB_MDER   (PIOB_BASE + 0x50)  /*!< \brief Multi-driver enable register. */
#define PIOB_MDDR   (PIOB_BASE + 0x54)  /*!< \brief Multi-driver disable register. */
#define PIOB_MDSR   (PIOB_BASE + 0x58)  /*!< \brief Multi-driver status register. */
#define PIOB_PUDR   (PIOB_BASE + 0x60)  /*!< \brief Pull-up disable register. */
#define PIOB_PUER   (PIOB_BASE + 0x64)  /*!< \brief Pull-up enable register. */
#define PIOB_PUSR   (PIOB_BASE + 0x68)  /*!< \brief Pull-up status register. */
#define PIOB_ASR    (PIOB_BASE + 0x70)  /*!< \brief PIO peripheral A select register. */
#define PIOB_BSR    (PIOB_BASE + 0x74)  /*!< \brief PIO peripheral B select register. */
#define PIOB_ABSR   (PIOB_BASE + 0x78)  /*!< \brief PIO peripheral AB status register. */
#define PIOB_OWER   (PIOB_BASE + 0xA0)  /*!< \brief PIO output write enable register. */
#define PIOB_OWDR   (PIOB_BASE + 0xA4)  /*!< \brief PIO output write disable register. */
#define PIOB_OWSR   (PIOB_BASE + 0xA8)  /*!< \brief PIO output write status register. */
#endif

/*@} xgNutArchArmAt91Pio */

#endif                          /* _ARCH_ARM_AT91_PIO_H_ */
