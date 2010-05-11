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
 * $Id$
 * \endverbatim
 */

/*!
 * \addtogroup xgNutArchArmAt91Udp
 */
/*@{*/

/*! \name Frame Number Register */
/*@{*/
#define UDP_NUM_OFF      0x00000000     /*!< \brief Frame number register offset. */
#define UDP_NUM          (UDP_BASE + UDP_NUM_OFF) /*!< \brief Frame number register address. */
#define UDP_FRM_NUM      0x000007FF     /*!< \brief Current frame number mask. */
#define UDP_FRM_NUM_LSB  0              /*!< \brief Current frame number LSB. */
#define UDP_FRM_ERR      _BV(16)        /*!< \brief Frame error. */
#define UDP_FRM_OK       _BV(17)        /*!< \brief Frame OK. */
/*@}*/

/*! \name Global State Register */
/*@{*/
#define UDP_GLBSTATE_OFF 0x00000004     /*!< \brief Global state register offset. */
#define UDP_GLBSTATE     (UDP_BASE + UDP_GLBSTATE_OFF) /*!< \brief Global state register address. */
#define UDP_FADDEN       _BV(0)         /*!< \brief Device is in address state. */
#define UDP_CONFG        _BV(1)         /*!< \brief Device is in configured state. */
/* TODO: Not defined in datasheet for AT91SAM7X... */
//#define UDP_ESR          _BV(2)         /*!< \brief Enable send resume. */
//#define UDP_RSMINPR      _BV(3)         /*!< \brief A resume has been sent to the host. */
//#define UDP_RMWUPE       _BV(4)         /*!< \brief Remote wake up enable. */
/*@}*/

/*! \name Function Address Register */
/*@{*/
#define UDP_FADDR_OFF    0x00000008     /*!< \brief Function address register offset. */
#define UDP_FADDR        (UDP_BASE + UDP_FADDR_OFF) /*!< \brief Function address register address. */
#define UDP_FADD         0x0000007F     /*!< \brief Mask: Function address value. */
#define UDP_FEN          _BV(8)         /*!< \brief Function endpoint enabled. */
/*@}*/

/*! \name Interrupt Registers */
/*@{*/
#define UDP_IER_OFF      0x00000010     /*!< \brief Interrupt enable register offset. */
#define UDP_IER          (UDP_BASE + UDP_IER_OFF) /*!< \brief Interrupt enable register address. */
#define UDP_IDR_OFF      0x00000014     /*!< \brief Interrupt disable register offset. */
#define UDP_IDR          (UDP_BASE + UDP_IDR_OFF) /*!< \brief Interrupt disable register address. */
#define UDP_IMR_OFF      0x00000018     /*!< \brief Interrupt mask register offset. */
#define UDP_IMR          (UDP_BASE + UDP_IMR_OFF) /*!< \brief Interrupt mask register address. */
#define UDP_ISR_OFF      0x0000001C     /*!< \brief Interrupt status register offset. */
#define UDP_ISR          (UDP_BASE + UDP_ISR_OFF) /*!< \brief Interrupt status register address. */
#define UDP_ICR_OFF      0x00000020     /*!< \brief Interrupt clear register offset. */
#define UDP_ICR          (UDP_BASE + UDP_ICR_OFF) /*!< \brief Interrupt clear register address. */
#define UDP_EPINT0       _BV( 0)        /*!< \brief Endpoint 0 interrupt. */
#define UDP_EPINT1       _BV( 1)        /*!< \brief Endpoint 0 interrupt. */
#define UDP_EPINT2       _BV( 2)        /*!< \brief Endpoint 2 interrupt. */
#define UDP_EPINT3       _BV( 3)        /*!< \brief Endpoint 3 interrupt. */
#define UDP_EPINT4       _BV( 4)        /*!< \brief Endpoint 4 interrupt. */
#define UDP_EPINT5       _BV( 5)        /*!< \brief Endpoint 5 interrupt. */
#define UDP_RXSUSP       _BV( 8)        /*!< \brief USB suspend interrupt. */
#define UDP_RXRSM        _BV( 9)        /*!< \brief USB resume interrupt. */
#define UDP_EXTRSM       _BV(10)        /*!< \brief USB external resume interrupt. */
#define UDP_SOFINT       _BV(11)        /*!< \brief USB start of frame interrupt. */
#define UDP_ENDBUSRES    _BV(12)        /*!< \brief USB end of bus reset interrupt. */
#define UDP_WAKEUP       _BV(13)        /*!< \brief USB resume interrupt. */
/*@}*/

/*! \name Reset Endpoint Register */
/*@{*/
#define UDP_RST_EP_OFF   0x00000028     /*!< \brief Reset endpoint register offset. */
#define UDP_RST_EP       (UDP_BASE + UDP_RST_EP_OFF) /*!< \brief Reset endpoint register address. */

#define UDP_EP0          _BV(0)          /*!< \brief Reset endpoint 0. */
#define UDP_EP1          _BV(1)          /*!< \brief Reset endpoint 1. */
#define UDP_EP2          _BV(2)          /*!< \brief Reset endpoint 2. */
#define UDP_EP3          _BV(3)          /*!< \brief Reset endpoint 3. */
#define UDP_EP4          _BV(4)          /*!< \brief Reset endpoint 4. */
#define UDP_EP5          _BV(5)          /*!< \brief Reset endpoint 5. */
/*@}*/

/*! \name Endpoint Control and Status Registers */
/*@{*/
#define UDP_CSR_OFF      0x00000030     /*!< \brief Endpoint control and status register offset. */
#define UDP_CSR          (UDP_BASE + UDP_CSR_OFF) /*!< \brief Endpoint control and status register address. */
#define UDP_EP0_CSR      (UDP_CSR + 0x00000000)  /*!< \brief Endpoint 0 control and status register address. */
#define UDP_EP1_CSR      (UDP_CSR + 0x00000004)  /*!< \brief Endpoint 1 control and status register address. */
#define UDP_EP2_CSR      (UDP_CSR + 0x00000008)  /*!< \brief Endpoint 2 control and status register address. */
#define UDP_EP3_CSR      (UDP_CSR + 0x0000000C)  /*!< \brief Endpoint 3 control and status register address. */
#define UDP_EP4_CSR      (UDP_CSR + 0x00000010)  /*!< \brief Endpoint 4 control and status register address. */
#define UDP_EP5_CSR      (UDP_CSR + 0x00000014)  /*!< \brief Endpoint 5 control and status register address. */

#define UDP_TXCOMP       _BV(0)         /*!< \brief Generates an IN packet with data previously written in the DPR. */
#define UDP_RX_DATA_BK0  _BV(1)         /*!< \brief Receive data bank 0. */
#define UDP_RXSETUP      _BV(2)         /*!< \brief Sends Stall to the host (control endpoints). */
#define UDP_STALL_SEND_ISOERROR _BV(3)  /*!< \brief Stall send / isochronous error (isochronous endpoints). */
#define UDP_TXPKTRDY     _BV(4)         /*!< \brief Transmit packet ready. */
#define UDP_FORCESTALL   _BV(5)         /*!< \brief Force Stall (used by control, bulk and isochronous endpoints). */
#define UDP_RX_DATA_BK1  _BV(6)         /*!< \brief Receive data bank 1 (only used by endpoints with ping-pong attributes). */
#define UDP_DIR          _BV(7)         /*!< \brief Transfer direction. */
#define UDP_EPTYPE       (0x7 << 8)     /*!< \brief Mask: Endpoint type, 3 BIT. */
#define     UDP_EPTYPE_CTRL      (0x0 << 8) /*!< \brief Endpoint type control. */
#define     UDP_EPTYPE_ISO_OUT   (0x1 << 8) /*!< \brief Endpoint type isochronous OUT. */
#define     UDP_EPTYPE_BULK_OUT  (0x2 << 8) /*!< \brief Endpoint type bulk OUT. */
#define     UDP_EPTYPE_INT_OUT   (0x3 << 8) /*!< \brief Endpoint type interrupt OUT. */
#define     UDP_EPTYPE_ISO_IN    (0x5 << 8) /*!< \brief Endpoint type isochronous IN. */
#define     UDP_EPTYPE_BULK_IN   (0x6 << 8) /*!< \brief Endpoint type bulk IN. */
#define     UDP_EPTYPE_INT_IN    (0x7 << 8) /*!< \brief Endpoint type interrupt IN. */
#define UDP_DTGLE       _BV(11)         /*!< \brief Data toggle. */
#define UDP_EPEDS       _BV(15)         /*!< \brief Endpoint enable disable. */
#define UDP_RXBYTECNT   0x07FF0000   /*!< \brief Mask: Number of bytes available in the FIFO. */
#define UDP_RXBYTECNT_LSB   16   /*!< \brief LSB: Number of bytes available in the FIFO. */
/*@}*/

/*! \name Endpoint FIFO Data Registers */
/*@{*/
#define UDP_FDR_OFF      0x00000050     /*!< \brief Endpoint FIFO data register offset. */
#define UDP_FDR          (UDP_BASE + UDP_FDR_OFF) /*!< \brief Endpoint FIFO data register address. */
#define UDP_EP0_FDR      (UDP_FDR + 0x00000000)  /*!< \brief Endpoint 0 FIFO data register address. */
#define UDP_EP1_FDR      (UDP_FDR + 0x00000004)  /*!< \brief Endpoint 1 FIFO data register address. */
#define UDP_EP2_FDR      (UDP_FDR + 0x00000008)  /*!< \brief Endpoint 2 FIFO data register address. */
#define UDP_EP3_FDR      (UDP_FDR + 0x0000000C)  /*!< \brief Endpoint 3 FIFO data register address. */
#define UDP_EP4_FDR      (UDP_FDR + 0x00000010)  /*!< \brief Endpoint 4 FIFO data register address. */
#define UDP_EP5_FDR      (UDP_FDR + 0x00000014)  /*!< \brief Endpoint 5 FIFO data register address. */
/*@}*/

/*! \name Tranceiver Control Register */
/*@{*/
#define UDP_TXVC_OFF     0x00000074     /*!< \brief Transceiver control register offset. */
#define UDP_TXVC         (UDP_BASE + UDP_TXVC_OFF) /*!< \brief Transceiver control register address. */
#define UDP_TXVDIS       _BV(8)         /*!< \brief Tranceiver disable. */
/*@}*/

/*@} xgNutArchArmAt91Udp */

#endif                          /* _ARCH_ARM_AT91_UDP_H_ */
