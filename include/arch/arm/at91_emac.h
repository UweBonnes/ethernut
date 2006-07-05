#ifndef _ARCH_ARM_AT91_EMAC_H_
#define _ARCH_ARM_AT91_EMAC_H_

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
 * \file arch/arm/at91_emac.h
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
 * \addtogroup xgNutArchArmAt91Emac
 */
/*@{*/

#define EMAC_NCR    (EMAC_BASE + 0x00)    /*!< \brief Network Control Register. */
#define EMAC_LB         ((unsigned int) 0x1 <<  0) /*!< \brief  Loopback. Optional. When set, loopback signal is at high level. */
#define EMAC_LLB        ((unsigned int) 0x1 <<  1) /*!< \brief  Loopback local. */
#define EMAC_RE         ((unsigned int) 0x1 <<  2) /*!< \brief  Receive enable. */
#define EMAC_TE         ((unsigned int) 0x1 <<  3) /*!< \brief  Transmit enable. */
#define EMAC_MPE        ((unsigned int) 0x1 <<  4) /*!< \brief  Management port enable. */
#define EMAC_CLRSTAT    ((unsigned int) 0x1 <<  5) /*!< \brief  Clear statistics registers. */
#define EMAC_INCSTAT    ((unsigned int) 0x1 <<  6) /*!< \brief  Increment statistics registers. */
#define EMAC_WESTAT     ((unsigned int) 0x1 <<  7) /*!< \brief  Write enable for statistics registers. */
#define EMAC_BP         ((unsigned int) 0x1 <<  8) /*!< \brief  Back pressure. */
#define EMAC_TSTART     ((unsigned int) 0x1 <<  9) /*!< \brief  Start Transmission. */
#define EMAC_THALT      ((unsigned int) 0x1 << 10) /*!< \brief  Transmission Halt. */
#define EMAC_TPFR       ((unsigned int) 0x1 << 11) /*!< \brief  Transmit pause frame. */
#define EMAC_TZQ        ((unsigned int) 0x1 << 12) /*!< \brief  Transmit zero quantum pause frame. */

#define EMAC_NCFGR  (EMAC_BASE + 0x04)    /*!< \brief Network Configuration Register. */
#define EMAC_SPD        ((unsigned int) 0x1 <<  0) /*!< \brief  Speed. */
#define EMAC_FD         ((unsigned int) 0x1 <<  1) /*!< \brief  Full duplex. */
#define EMAC_JFRAME     ((unsigned int) 0x1 <<  3) /*!< \brief  Jumbo Frames. */
#define EMAC_CAF        ((unsigned int) 0x1 <<  4) /*!< \brief  Copy all frames. */
#define EMAC_NBC        ((unsigned int) 0x1 <<  5) /*!< \brief  No broadcast. */
#define EMAC_MTI        ((unsigned int) 0x1 <<  6) /*!< \brief  Multicast hash event enable. */
#define EMAC_UNI        ((unsigned int) 0x1 <<  7) /*!< \brief  Unicast hash enable. */
#define EMAC_BIG        ((unsigned int) 0x1 <<  8) /*!< \brief  Receive 1522 bytes. */
#define EMAC_EAE        ((unsigned int) 0x1 <<  9) /*!< \brief  External address match enable. */
#define EMAC_CLK        ((unsigned int) 0x3 << 10) /*!< \brief . */
#define EMAC_CLK_HCLK_8     ((unsigned int) 0x0 << 10) /*!< \brief  HCLK divided by 8. */
#define EMAC_CLK_HCLK_16    ((unsigned int) 0x1 << 10) /*!< \brief  HCLK divided by 16. */
#define EMAC_CLK_HCLK_32    ((unsigned int) 0x2 << 10) /*!< \brief  HCLK divided by 32. */
#define EMAC_CLK_HCLK_64    ((unsigned int) 0x3 << 10) /*!< \brief  HCLK divided by 64. */
#define EMAC_RTY        ((unsigned int) 0x1 << 12) /*!< \brief  . */
#define EMAC_PAE        ((unsigned int) 0x1 << 13) /*!< \brief  . */
#define EMAC_RBOF       ((unsigned int) 0x3 << 14) /*!< \brief  . */
#define EMAC_RBOF_OFFSET_0  ((unsigned int) 0x0 << 14) /*!< \brief  no offset from start of receive buffer. */
#define EMAC_RBOF_OFFSET_1  ((unsigned int) 0x1 << 14) /*!< \brief  one byte offset from start of receive buffer. */
#define EMAC_RBOF_OFFSET_2  ((unsigned int) 0x2 << 14) /*!< \brief  two bytes offset from start of receive buffer. */
#define EMAC_RBOF_OFFSET_3  ((unsigned int) 0x3 << 14) /*!< \brief  three bytes offset from start of receive buffer. */
#define EMAC_RLCE       ((unsigned int) 0x1 << 16) /*!< \brief  Receive Length field Checking Enable. */
#define EMAC_DRFCS      ((unsigned int) 0x1 << 17) /*!< \brief  Discard Receive FCS. */
#define EMAC_EFRHD      ((unsigned int) 0x1 << 18) /*!< \brief  . */
#define EMAC_IRXFCS     ((unsigned int) 0x1 << 19) /*!< \brief  Ignore RX FCS. */

#define EMAC_NSR    (EMAC_BASE + 0x08)    /*!< \brief Network Status Register. */
#define EMAC_LINKR      ((unsigned int) 0x1 <<  0) /*!< \brief  . */
#define EMAC_MDIO       ((unsigned int) 0x1 <<  1) /*!< \brief  . */
#define EMAC_IDLE       ((unsigned int) 0x1 <<  2) /*!< \brief  . */

#define EMAC_TSR    (EMAC_BASE + 0x14)    /*!< \brief Transmit Status Register. */
#define EMAC_UBR        ((unsigned int) 0x1 <<  0) /*!< \brief  . */
#define EMAC_COL        ((unsigned int) 0x1 <<  1) /*!< \brief  . */
#define EMAC_RLES       ((unsigned int) 0x1 <<  2) /*!< \brief  . */
#define EMAC_TGO        ((unsigned int) 0x1 <<  3) /*!< \brief  Transmit Go. */
#define EMAC_BEX        ((unsigned int) 0x1 <<  4) /*!< \brief  Buffers exhausted mid frame. */
#define EMAC_COMP       ((unsigned int) 0x1 <<  5) /*!< \brief  . */
#define EMAC_UND        ((unsigned int) 0x1 <<  6) /*!< \brief  . */

#define EMAC_RBQP   (EMAC_BASE + 0x18)    /*!< \brief Receive Buffer Queue Pointer. */

#define EMAC_TBQP   (EMAC_BASE + 0x1C)    /*!< \brief Transmit Buffer Queue Pointer. */

#define EMAC_RSR    (EMAC_BASE + 0x20)    /*!< \brief Receive Status Register. */
#define EMAC_BNA        ((unsigned int) 0x1 <<  0) /*!< \brief  . */
#define EMAC_REC        ((unsigned int) 0x1 <<  1) /*!< \brief  . */
#define EMAC_OVR        ((unsigned int) 0x1 <<  2) /*!< \brief  . */

#define EMAC_ISR    (EMAC_BASE + 0x24)    /*!< \brief Interrupt Status Register. */
#define EMAC_MFD        ((unsigned int) 0x1 <<  0) /*!< \brief  . */
#define EMAC_RCOMP      ((unsigned int) 0x1 <<  1) /*!< \brief  . */
#define EMAC_RXUBR      ((unsigned int) 0x1 <<  2) /*!< \brief  . */
#define EMAC_TXUBR      ((unsigned int) 0x1 <<  3) /*!< \brief  . */
#define EMAC_TUNDR      ((unsigned int) 0x1 <<  4) /*!< \brief  . */
#define EMAC_RLEX       ((unsigned int) 0x1 <<  5) /*!< \brief  . */
#define EMAC_TXERR      ((unsigned int) 0x1 <<  6) /*!< \brief  . */
#define EMAC_TCOMP      ((unsigned int) 0x1 <<  7) /*!< \brief  . */
#define EMAC_LINK       ((unsigned int) 0x1 <<  9) /*!< \brief  . */
#define EMAC_ROVR       ((unsigned int) 0x1 << 10) /*!< \brief  . */
#define EMAC_HRESP      ((unsigned int) 0x1 << 11) /*!< \brief  . */
#define EMAC_PFRE       ((unsigned int) 0x1 << 12) /*!< \brief  . */
#define EMAC_PTZ        ((unsigned int) 0x1 << 13) /*!< \brief  . */

#define EMAC_IER    (EMAC_BASE + 0x28)    /*!< \brief Interrupt Enable Register. */
#define EMAC_IDR    (EMAC_BASE + 0x2C)    /*!< \brief Interrupt Disable Register. */
#define EMAC_IMR    (EMAC_BASE + 0x30)    /*!< \brief Interrupt Mask Register. */
#define EMAC_MAN    (EMAC_BASE + 0x34)    /*!< \brief PHY Maintenance Register. */
#define EMAC_DATA       ((unsigned int) 0xFFFF <<  0) /*!< \brief  . */
#define EMAC_CODE       ((unsigned int) 0x3 << 16) /*!< \brief  . */
#define EMAC_REGA       ((unsigned int) 0x1F << 18) /*!< \brief  . */
#define EMAC_PHYA       ((unsigned int) 0x1F << 23) /*!< \brief  . */
#define EMAC_RW         ((unsigned int) 0x3 << 28) /*!< \brief  . */
#define EMAC_SOF        ((unsigned int) 0x3 << 30) /*!< \brief  . */

#define EMAC_PTR    (EMAC_BASE + 0x38)    /*!< \brief Pause Time Register. */
#define EMAC_PFR    (EMAC_BASE + 0x3C)    /*!< \brief Pause Frames received Register. */
#define EMAC_FTO    (EMAC_BASE + 0x40)    /*!< \brief Frames Transmitted OK Register. */
#define EMAC_SCF    (EMAC_BASE + 0x44)    /*!< \brief Single Collision Frame Register. */
#define EMAC_MCF    (EMAC_BASE + 0x48)    /*!< \brief Multiple Collision Frame Register. */
#define EMAC_FRO    (EMAC_BASE + 0x4C)    /*!< \brief Frames Received OK Register. */
#define EMAC_FCSE   (EMAC_BASE + 0x50)    /*!< \brief Frame Check Sequence Error Register. */
#define EMAC_ALE    (EMAC_BASE + 0x54)    /*!< \brief Alignment Error Register. */
#define EMAC_DTF    (EMAC_BASE + 0x58)    /*!< \brief Deferred Transmission Frame Register. */
#define EMAC_LCOL   (EMAC_BASE + 0x5C)    /*!< \brief Late Collision Register. */
#define EMAC_ECOL   (EMAC_BASE + 0x60)    /*!< \brief Excessive Collision Register. */
#define EMAC_TUND   (EMAC_BASE + 0x64)    /*!< \brief Transmit Underrun Error Register. */
#define EMAC_CSE    (EMAC_BASE + 0x68)    /*!< \brief Carrier Sense Error Register. */
#define EMAC_RRE    (EMAC_BASE + 0x6C)    /*!< \brief Receive Ressource Error Register. */
#define EMAC_ROV    (EMAC_BASE + 0x70)    /*!< \brief Receive Overrun Errors Register. */
#define EMAC_RSE    (EMAC_BASE + 0x74)    /*!< \brief Receive Symbol Errors Register. */
#define EMAC_ELE    (EMAC_BASE + 0x78)    /*!< \brief Excessive Length Errors Register. */
#define EMAC_RJA    (EMAC_BASE + 0x7C)    /*!< \brief Receive Jabbers Register. */
#define EMAC_USF    (EMAC_BASE + 0x80)    /*!< \brief Undersize Frames Register. */
#define EMAC_STE    (EMAC_BASE + 0x84)    /*!< \brief SQE Test Error Register. */
#define EMAC_RLE    (EMAC_BASE + 0x88)    /*!< \brief Receive Length Field Mismatch Register. */
#define EMAC_TPF    (EMAC_BASE + 0x8C)    /*!< \brief Transmitted Pause Frames Register. */
#define EMAC_HRB    (EMAC_BASE + 0x90)    /*!< \brief Hash Address Bottom[31:0]. */
#define EMAC_HRT    (EMAC_BASE + 0x94)    /*!< \brief Hash Address Top[63:32]. */
#define EMAC_SA1L   (EMAC_BASE + 0x98)    /*!< \brief Specific Address 1 Bottom, First 4 bytes. */
#define EMAC_SA1H   (EMAC_BASE + 0x9C)    /*!< \brief Specific Address 1 Top, Last 2 bytes. */
#define EMAC_SA2L   (EMAC_BASE + 0xA0)    /*!< \brief Specific Address 2 Bottom, First 4 bytes. */
#define EMAC_SA2H   (EMAC_BASE + 0xA4)    /*!< \brief Specific Address 2 Top, Last 2 bytes. */
#define EMAC_SA3L   (EMAC_BASE + 0xA8)    /*!< \brief Specific Address 3 Bottom, First 4 bytes. */
#define EMAC_SA3H   (EMAC_BASE + 0xAC)    /*!< \brief Specific Address 3 Top, Last 2 bytes. */
#define EMAC_SA4L   (EMAC_BASE + 0xB0)    /*!< \brief Specific Address 4 Bottom, First 4 bytes. */
#define EMAC_SA4H   (EMAC_BASE + 0xB4)    /*!< \brief Specific Address 4 Top, Last 2 bytes. */
#define EMAC_TID    (EMAC_BASE + 0xB8)    /*!< \brief Type ID Checking Register. */
#define EMAC_TPQ    (EMAC_BASE + 0xBC)    /*!< \brief Transmit Pause Quantum Register. */

#define EMAC_USRIO  (EMAC_BASE + 0xC0)    /*!< \brief USER Input/Output Register. */
#define EMAC_RMII           0x00000001    /*!< \brief Enable reduced MII. */
#define EMAC_CLKEN          0x00000002    /*!< \brief Enable tranceiver input clock. */

#define EMAC_WOL    (EMAC_BASE + 0xC4)    /*!< \brief Wake On LAN Register. */
#define EMAC_IP         ((unsigned int) 0xFFFF <<  0) /*!< \brief  ARP request IP address. */
#define EMAC_MAG        ((unsigned int) 0x1 << 16) /*!< \brief  Magic packet event enable. */
#define EMAC_ARP        ((unsigned int) 0x1 << 17) /*!< \brief  ARP request event enable. */
#define EMAC_SA1        ((unsigned int) 0x1 << 18) /*!< \brief  Specific address register 1 event enable. */

#define EMAC_REV    (EMAC_BASE + 0xFC)    /*!< \brief Revision Register. */
#define EMAC_REVREF     ((unsigned int) 0xFFFF <<  0) /*!< \brief  . */
#define EMAC_PARTREF    ((unsigned int) 0xFFFF << 16) /*!< \brief  . */

/*@} xgNutArchArmAt91Emac */

#endif                          /* _ARCH_ARM_AT91_EMAC_H_ */

