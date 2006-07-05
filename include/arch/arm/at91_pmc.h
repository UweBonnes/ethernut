#ifndef _ARCH_ARM_AT91_PMC_H_
#define _ARCH_ARM_AT91_PMC_H_

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
 * \file arch/arm/at91_pmc.h
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
 * \addtogroup xgNutArchArmAt91Pmc
 */
/*@{*/
#define PMC_SCER    (PMC_BASE + 0x00)   /*!< \brief System Clock Enable Register. */
#define PMC_PCK         ((unsigned int) 0x1 <<  0) /*!< \brief Processor Clock. */
#define PMC_UDP         ((unsigned int) 0x1 <<  7) /*!< \brief USB Device Port Clock. */
#define PMC_PCK0        ((unsigned int) 0x1 <<  8) /*!< \brief Programmable Clock Output. */
#define PMC_PCK1        ((unsigned int) 0x1 <<  9) /*!< \brief Programmable Clock Output. */
#define PMC_PCK2        ((unsigned int) 0x1 << 10) /*!< \brief Programmable Clock Output. */

#define PMC_SCDR    (PMC_BASE + 0x04)   /*!< \brief System Clock Disable Register. */
#define PMC_SCSR    (PMC_BASE + 0x08)   /*!< \brief System Clock Status Register. */
#define PMC_PCER    (PMC_BASE + 0x10)   /*!< \brief Peripheral Clock Enable Register. */
#define PMC_PCDR    (PMC_BASE + 0x14)   /*!< \brief Peripheral Clock Disable Register. */
#define PMC_PCSR    (PMC_BASE + 0x18)   /*!< \brief Peripheral Clock Status Register. */

#define CKGR_MOR        (PMC_BASE + 0x20)   /*!< \brief Main Oscillator Register. */
#define CKGR_MOSCEN     ((unsigned int) 0x1 <<  0) /*!< \brief Main Oscillator Enable. */
#define CKGR_OSCBYPASS  ((unsigned int) 0x1 <<  1) /*!< \brief Main Oscillator Bypass. */
#define CKGR_OSCOUNT    ((unsigned int) 0xFF <<  8) /*!< \brief Main Oscillator Start-up Time. */

#define CKGR_MCFR       (PMC_BASE + 0x24)   /*!< \brief Main Clock  Frequency Register. */
#define CKGR_MAINF      ((unsigned int) 0xFFFF <<  0) /*!< \brief Main Clock Frequency. */
#define CKGR_MAINRDY    ((unsigned int) 0x1 << 16) /*!< \brief Main Clock Ready. */

#define CKGR_PLLR       (PMC_BASE + 0x2C)   /*!< \brief PLL Register. */
#define CKGR_DIV        ((unsigned int) 0xFF <<  0) /*!< \brief Divider Selected. */
#define CKGR_DIV_0      ((unsigned int) 0x0) /*!< \brief Divider output is 0. */
#define CKGR_DIV_BYPASS ((unsigned int) 0x1) /*!< \brief Divider is bypassed. */
#define CKGR_PLLCOUNT   ((unsigned int) 0x3F <<  8) /*!< \brief PLL Counter. */
#define CKGR_OUT        ((unsigned int) 0x3 << 14) /*!< \brief PLL Output Frequency Range. */
#define CKGR_OUT_0      ((unsigned int) 0x0 << 14) /*!< \brief Please refer to the PLL datasheet. */
#define CKGR_OUT_1      ((unsigned int) 0x1 << 14) /*!< \brief Please refer to the PLL datasheet. */
#define CKGR_OUT_2      ((unsigned int) 0x2 << 14) /*!< \brief Please refer to the PLL datasheet. */
#define CKGR_OUT_3      ((unsigned int) 0x3 << 14) /*!< \brief Please refer to the PLL datasheet. */
#define CKGR_MUL        ((unsigned int) 0x7FF << 16) /*!< \brief PLL Multiplier. */
#define CKGR_USBDIV     ((unsigned int) 0x3 << 28) /*!< \brief Divider for USB Clocks. */
#define CKGR_USBDIV_0   ((unsigned int) 0x0 << 28) /*!< \brief Divider output is PLL clock output. */
#define CKGR_USBDIV_1   ((unsigned int) 0x1 << 28) /*!< \brief Divider output is PLL clock output divided by 2. */
#define CKGR_USBDIV_2   ((unsigned int) 0x2 << 28) /*!< \brief Divider output is PLL clock output divided by 4. */

#define PMC_MCKR    (PMC_BASE + 0x30)   /*!< \brief Master Clock Register. */
#define PMC_CSS         ((unsigned int) 0x3 <<  0) /*!< \brief Programmable Clock Selection. */
#define PMC_CSS_SLOW_CLK ((unsigned int) 0x0) /*!< \brief Slow Clock is selected. */
#define PMC_CSS_MAIN_CLK ((unsigned int) 0x1) /*!< \brief Main Clock is selected. */
#define PMC_CSS_PLL_CLK ((unsigned int) 0x3) /*!< \brief Clock from PLL is selected. */
#define PMC_PRES        ((unsigned int) 0x7 <<  2) /*!< \brief Programmable Clock Prescaler. */
#define PMC_PRES_CLK    ((unsigned int) 0x0 <<  2) /*!< \brief Selected clock. */
#define PMC_PRES_CLK_2  ((unsigned int) 0x1 <<  2) /*!< \brief Selected clock divided by 2. */
#define PMC_PRES_CLK_4  ((unsigned int) 0x2 <<  2) /*!< \brief Selected clock divided by 4. */
#define PMC_PRES_CLK_8  ((unsigned int) 0x3 <<  2) /*!< \brief Selected clock divided by 8. */
#define PMC_PRES_CLK_16 ((unsigned int) 0x4 <<  2) /*!< \brief Selected clock divided by 16. */
#define PMC_PRES_CLK_32 ((unsigned int) 0x5 <<  2) /*!< \brief Selected clock divided by 32. */
#define PMC_PRES_CLK_64 ((unsigned int) 0x6 <<  2) /*!< \brief Selected clock divided by 64. */

#define PMC_PCKR0   (PMC_BASE + 0x38)   /*!< \brief Programmable Clock Register. */
#define PMC_PCKR1   (PMC_BASE + 0x3C)   /*!< \brief Programmable Clock Register. */
#define PMC_PCKR2   (PMC_BASE + 0x40)   /*!< \brief Programmable Clock Register. */
#define PMC_PCKR3   (PMC_BASE + 0x44)   /*!< \brief Programmable Clock Register. */

#define PMC_IER     (PMC_BASE + 0x60)   /*!< \brief Interrupt Enable Register. */
#define PMC_IDR     (PMC_BASE + 0x64)   /*!< \brief Interrupt Disable Register. */
#define PMC_SR      (PMC_BASE + 0x68)   /*!< \brief Status Register. */
#define PMC_IMR     (PMC_BASE + 0x6C)   /*!< \brief Interrupt Mask Register. */
#define PMC_MOSCS       ((unsigned int) 0x1 <<  0) /*!< \brief MOSC Status/Enable/Disable/Mask. */
#define PMC_LOCK        ((unsigned int) 0x1 <<  2) /*!< \brief PLL Status/Enable/Disable/Mask. */
#define PMC_MCKRDY      ((unsigned int) 0x1 <<  3) /*!< \brief MCK_RDY Status/Enable/Disable/Mask. */
#define PMC_PCK0RDY     ((unsigned int) 0x1 <<  8) /*!< \brief PCK0_RDY Status/Enable/Disable/Mask. */
#define PMC_PCK1RDY     ((unsigned int) 0x1 <<  9) /*!< \brief PCK1_RDY Status/Enable/Disable/Mask. */
#define PMC_PCK2RDY     ((unsigned int) 0x1 << 10) /*!< \brief PCK2_RDY Status/Enable/Disable/Mask. */

/*@} xgNutArchArmAt91Pmc */


#endif                          /* _ARCH_ARM_AT91_PMC_H_ */
