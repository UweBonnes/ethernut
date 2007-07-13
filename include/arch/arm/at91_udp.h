/*
 * Copyright (C) 2007 by Ole Reinhardt, EmbeddedIT. ole.reinhardt@embedded-it.de
 * All rights reserved.
 *
 * This file is based on at91_udp.h 
 * (c) Uwe Bonnes <bon@elektron.ikp.physik.tu-darmstadt.de>
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
 * THIS SOFTWARE IS PROVIDED BY EMBEDDED-IT AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EMBEDDED-IT
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

#ifndef _ARCH_ARM_AT91_UDP_H_
#define _ARCH_ARM_AT91_UDP_H_

/*!
 * \file arch/arm/at91_udp.h
 * \brief AT91 peripherals (USB device port).
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2007/07/13 15:21:36  olereinhardt
 * 2007-07-13  Ole Reinhardt <ole.reinhardt@embedded-it.de>
 *         * include/arch/arm/at91sam7x.h: Added UDP support
 *         * include/arch/arm/at91_udp.h: Defines for arm7 UDP controller
 * *
 *
 * \endverbatim
 */

/*!
 * \addtogroup xgNutArchArmAt91Udp
 */
/*@{*/

/*! \name Frame Number Register */
/*@{*/
#define UDP_NUM_OFF      0x00000000
#define UDP_NUM          (UDP_BASE + UDP_NUM_OFF)
#define UDP_FRM_NUM      (0x07FF <<  0) /*!< \brief Mask: Frame Number as Defined in the Packet Field Formats */
#define UDP_FRM_ERR      _BV(16)        /*!< \brief Frame Error */
#define UDP_FRM_OK       _BV(17)        /*!< \brief Frame OK */
/*@}*/

/*! \name Global State Register */
/*@{*/
#define UDP_GLBSTATE_OFF 0x00000004
#define UDP_GLBSTATE     (UDP_BASE + UDP_GLBSTATE_OFF)
#define UDP_FADDEN       _BV(0)         /*!< \brief Function Address Enable */
#define UDP_CONFG        _BV(1)         /*!< \brief Configured */
/* TODO: Not defined in datasheet for AT91SAM7X... */
//#define UDP_ESR          _BV(2)         /*!< \brief Enable Send Resume */
//#define UDP_RSMINPR      _BV(3)         /*!< \brief A Resume Has Been Sent to the Host */
//#define UDP_RMWUPE       _BV(4)         /*!< \brief Remote Wake Up Enable */
/*@}*/

/*! \name Function Address Register */
/*@{*/
#define UDP_FADDR_OFF    0x00000008
#define UDP_FADDR        (UDP_BASE + UDP_FADDR_OFF)
#define UDP_FADD         (0x7F << 0)    /*!< \brief Mask: Function Address Value */
#define UDP_FEN          _BV(8)         /*!< \brief Function Enable */
/*@}*/

/*! \name Interrup Enable Register */
/*@{*/
#define UDP_IER_OFF      0x00000010 
#define UDP_IER          (UDP_BASE + UDP_IER_OFF)

#define UDP_EPINT0       _BV( 0)        /*!< \brief Endpoint 0 Interrupt */
#define UDP_EPINT1       _BV( 1)        /*!< \brief Endpoint 0 Interrupt */
#define UDP_EPINT2       _BV( 2)        /*!< \brief Endpoint 2 Interrupt */
#define UDP_EPINT3       _BV( 3)        /*!< \brief Endpoint 3 Interrupt */
#define UDP_EPINT4       _BV( 4)        /*!< \brief Endpoint 4 Interrupt */
#define UDP_EPINT5       _BV( 5)        /*!< \brief Endpoint 5 Interrupt */
#define UDP_RXSUSP       _BV( 8)        /*!< \brief USB Suspend Interrupt */
#define UDP_RXRSM        _BV( 9)        /*!< \brief USB Resume Interrupt */
#define UDP_EXTRSM       _BV(10)        /*!< \brief USB External Resume Interrupt */
#define UDP_SOFINT       _BV(11)        /*!< \brief USB Start Of frame Interrupt */
#define UDP_ENDBUSRES    _BV(12)        /*!< \brief USB End Of Bus Reset Interrupt */
#define UDP_WAKEUP       _BV(13)        /*!< \brief USB Resume Interrupt */
/*@}*/

/*! \name Interrup Disable Register */
/*@{*/
#define UDP_IDR_OFF      0x00000014
#define UDP_IDR          (UDP_BASE + UDP_IDR_OFF)
/*@}*/

/*! \name Interrup Mask Register */
/*@{*/
#define UDP_IMR_OFF      0x00000018
#define UDP_IMR          (UDP_BASE + UDP_IMR_OFF)
/*@}*/

/*! \name Interrup Status Register */
/*@{*/
#define UDP_ISR_OFF      0x0000001C
#define UDP_ISR          (UDP_BASE + UDP_ISR_OFF)
/*@}*/

/*! \name Interrup Clear Register */
/*@{*/
#define UDP_ICR_OFF      0x00000020
#define UDP_ICR          (UDP_BASE + UDP_ICR_OFF)
/*@}*/

/*! \name Reset Endpoint Register */
/*@{*/
#define UDP_RST_EP_OFF   0x00000028
#define UDP_RST_EP       (UDP_BASE + UDP_RST_EP_OFF)

#define UDP_EP0          _BV(0)          /*!< \brief Reset Endpoint 0 */
#define UDP_EP1          _BV(1)          /*!< \brief Reset Endpoint 1 */
#define UDP_EP2          _BV(2)          /*!< \brief Reset Endpoint 2 */
#define UDP_EP3          _BV(3)          /*!< \brief Reset Endpoint 3 */
#define UDP_EP4          _BV(4)          /*!< \brief Reset Endpoint 3 */
#define UDP_EP5          _BV(5)          /*!< \brief Reset Endpoint 3 */
/*@}*/

/*! \name Endpoint Control and Status Register */
/*@{*/
#define UDP_CSR_OFF      0x00000030
#define UDP_CSR          (UDP_BASE + UDP_CSR_OFF)
#define UDP_EP0_CSR      (UDP_CSR + 0x00000000)  /*!< \brief Endpoint 0 CSR Register */
#define UDP_EP1_CSR      (UDP_CSR + 0x00000004)  /*!< \brief Endpoint 1 CSR Register */
#define UDP_EP2_CSR      (UDP_CSR + 0x00000008)  /*!< \brief Endpoint 2 CSR Register */
#define UDP_EP3_CSR      (UDP_CSR + 0x0000000C)  /*!< \brief Endpoint 3 CSR Register */
#define UDP_EP4_CSR      (UDP_CSR + 0x00000010)  /*!< \brief Endpoint 4 CSR Register */
#define UDP_EP5_CSR      (UDP_CSR + 0x00000014)  /*!< \brief Endpoint 5 CSR Register */

#define UDP_TXCOMP       _BV(0)         /*!< \brief Generates an IN packet with data previously written in the DPR */
#define UDP_RX_DATA_BK0  _BV(1)         /*!< \brief Receive Data Bank 0 */
#define UDP_RXSETUP      _BV(2)         /*!< \brief Sends STALL to the Host (Control endpoints) */
#define UDP_STALL_SEND_ISOERROR _BV(3)  /*!< \brief STALL send / Isochronous error (Isochronous endpoints) */
#define UDP_TXPKTRDY     _BV(4)         /*!< \brief Transmit Packet Ready */
#define UDP_FORCESTALL   _BV(5)         /*!< \brief Force Stall (used by Control, Bulk and Isochronous endpoints). */
#define UDP_RX_DATA_BK1  _BV(6)         /*!< \brief Receive Data Bank 1 (only used by endpoints with ping-pong attributes) */
#define UDP_DIR          _BV(7)         /*!< \brief Transfer Direction */
#define UDP_EPTYPE       (0x7 << 8)     /*!< \brief Mask: Endpoint type, 3 BIT */
#define     UDP_EPTYPE_CTRL      (0x0 << 8) /*!< \brief Endpoint typeControl */
#define     UDP_EPTYPE_ISO_OUT   (0x1 << 8) /*!< \brief Endpoint type Isochronous OUT */
#define     UDP_EPTYPE_BULK_OUT  (0x2 << 8) /*!< \brief Endpoint type Bulk OUT */
#define     UDP_EPTYPE_INT_OUT   (0x3 << 8) /*!< \brief Endpoint type Interrupt OUT */
#define     UDP_EPTYPE_ISO_IN    (0x5 << 8) /*!< \brief Endpoint type Isochronous IN */
#define     UDP_EPTYPE_BULK_IN   (0x6 << 8) /*!< \brief Endpoint type Bulk IN */
#define     UDP_EPTYPE_INT_IN    (0x7 << 8) /*!< \brief Endpoint type Interrupt IN */
#define UDP_DTGLE       _BV(11)         /*!< \brief Data Toggle */
#define UDP_EPEDS       _BV(15)         /*!< \brief Endpoint Enable Disable */
#define UDP_RXBYTECNT   (0x7FF << 16)   /*!< \brief Mask: Number Of Bytes Available in the FIFO */
/*@}*/

/*! \name FIFO Data Register */
/*@{*/
#define UDP_FDR_OFF      0x00000050
#define UDP_FDR          (UDP_BASE + UDP_FDR_OFF)
#define UDP_EP0_FDR      (UDP_FDR + 0x00000000)  /*!< \brief Endpoint 0 FIFO Data Register */
#define UDP_EP1_FDR      (UDP_FDR + 0x00000004)  /*!< \brief Endpoint 1 FIFO Data Register */
#define UDP_EP2_FDR      (UDP_FDR + 0x00000008)  /*!< \brief Endpoint 2 FIFO Data Register */
#define UDP_EP3_FDR      (UDP_FDR + 0x0000000C)  /*!< \brief Endpoint 3 FIFO Data Register */
#define UDP_EP4_FDR      (UDP_FDR + 0x00000010)  /*!< \brief Endpoint 4 FIFO Data Register */
#define UDP_EP5_FDR      (UDP_FDR + 0x00000014)  /*!< \brief Endpoint 5 FIFO Data Register */
/*@}*/

/*! \name Tranceiver Control Register */
/*@{*/
#define UDP_TXVC_OFF     0x00000074
#define UDP_TXVC         (UDP_BASE + UDP_TXVDIS_OFF)
#define UDB_TXVDIS       _BV(8)         /*!< \brief Tranceiver disable). */
/*@}*/

/*@} xgNutArchArmAt91Udp */

#endif                          /* _ARCH_ARM_AT91_UDP_H_ */
