#ifndef _ARCH_ARM_AT91_AIC_H_
#define _ARCH_ARM_AT91_AIC_H_

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
 * \file arch/arm/at91_aic.h
 * \brief AT91 peripherals.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/07/05 07:45:25  haraldkipp
 * Split on-chip interface definitions.
 *
 *
 * \endverbatim
 */

/*!
 * \addtogroup xgNutArchArmAt91Aic
 */
/*@{*/

/*! \name Interrupt Source Mode Registers */
/*@{*/
/*! \brief Source mode register array.
 */
#define AIC_SMR(i)  (AIC_BASE + i * 4)

/*! \brief Priority mask. 
 *
 * Priority levels can be between 0 (lowest) and 7 (highest).
 */
#define AIC_PRIOR                       0x00000007

/*! \brief Interrupt source type mask. 
 *
 * Internal interrupts can level sensitive or edge triggered.
 *
 * External interrupts can triggered on positive or negative levels or 
 * on rising or falling edges.
 */
#define AIC_SRCTYPE                     0x00000060

#define AIC_SRCTYPE_INT_LEVEL_SENSITIVE 0x00000000      /*!< \brief Internal level sensitive. */
#define AIC_SRCTYPE_INT_EDGE_TRIGGERED  0x00000020      /*!< \brief Internal edge triggered. */
#define AIC_SRCTYPE_EXT_LOW_LEVEL       0x00000000      /*!< \brief External low level. */
#define AIC_SRCTYPE_EXT_NEGATIVE_EDGE   0x00000020      /*!< \brief External falling edge. */
#define AIC_SRCTYPE_EXT_HIGH_LEVEL      0x00000040      /*!< \brief External high level. */
#define AIC_SRCTYPE_EXT_POSITIVE_EDGE   0x00000060      /*!< \brief External rising edge. */
/*@}*/

/*! \name Interrupt Source Vector Registers */
/*@{*/
/*! \brief Source vector register array. 
 *
 * Stores the addresses of the corresponding interrupt handlers.
 */
#define AIC_SVR(i)  (AIC_BASE + 0x80 + i * 4)
/*@}*/

/*! \name Interrupt Vector Register */
/*@{*/
#define AIC_IVR     (AIC_BASE + 0x100)  /*!< \brief IRQ vector register address. */
/*@}*/

/*! \name Fast Interrupt Vector Register */
/*@{*/
#define AIC_FVR     (AIC_BASE + 0x104)  /*!< \brief FIQ vector register address. */
/*@}*/

/*! \name Interrupt Status Register */
/*@{*/
#define AIC_ISR     (AIC_BASE + 0x108)  /*!< \brief Interrupt status register address. */
#define AIC_IRQID               0x0000001F      /*!< \brief Current interrupt identifier mask. */
/*@}*/

/*! \name Interrupt Pending Register */
/*@{*/
#define AIC_IPR     (AIC_BASE + 0x10C)  /*!< \brief Interrupt pending register address. */
/*@}*/

/*! \name Interrupt Mask Register */
/*@{*/
#define AIC_IMR     (AIC_BASE + 0x110)  /*!< \brief Interrupt mask register address. */
/*@}*/

/*! \name Interrupt Core Status Register */
/*@{*/
#define AIC_CISR    (AIC_BASE + 0x114)  /*!< \brief Core interrupt status register address. */
#define AIC_NFIQ                0x00000001      /*!< \brief Core FIQ Status */
#define AIC_NIRQ                0x00000002      /*!< \brief Core IRQ Status */
/*@}*/

/*! \name Interrupt Enable Command Register */
/*@{*/
#define AIC_IECR    (AIC_BASE + 0x120)  /*!< \brief Interrupt enable command register address. */
/*@}*/

/*! \name Interrupt Disable Command Register */
/*@{*/
#define AIC_IDCR    (AIC_BASE + 0x124)  /*!< \brief Interrupt disable command register address. */
/*@}*/

/*! \name Interrupt Clear Command Register */
/*@{*/
#define AIC_ICCR    (AIC_BASE + 0x128)  /*!< \brief Interrupt clear command register address. */
/*@}*/

/*! \name Interrupt Set Command Register */
/*@{*/
#define AIC_ISCR    (AIC_BASE + 0x12C)  /*!< \brief Interrupt set command register address. */
/*@}*/

/*! \name End Of Interrupt Command Register */
/*@{*/
#define AIC_EOICR   (AIC_BASE + 0x130)  /*!< \brief End of interrupt command register address. */
/*@}*/

/*! \name Spurious Interrupt Vector Register */
/*@{*/
#define AIC_SPU     (AIC_BASE + 0x134)  /*!< \brief Spurious vector register address. */
/*@}*/

/*@} xgNutArchArmAt91Aic */

#endif                          /* _ARCH_ARM_AT91_AIC_H_ */

