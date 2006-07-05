#ifndef _ARCH_ARM_AT91_US_H_
#define _ARCH_ARM_AT91_US_H_

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
 * \file arch/arm/at91_us.h
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
 * \addtogroup xgNutArchArmAt91Us
 */
/*@{*/

/*! \name USART Control Register */
/*@{*/
#define US_CR_OFF               0x00000000      /*!< \brief USART control register offset. */
#define US0_CR  (USART0_BASE + US_CR_OFF)       /*!< \brief Channel 0 control register address. */
#define US1_CR  (USART1_BASE + US_CR_OFF)       /*!< \brief Channel 1 control register address. */
#define US_RSTRX                0x00000004      /*!< \brief Reset receiver */
#define US_RSTTX                0x00000008      /*!< \brief Reset transmitter */
#define US_RXEN                 0x00000010      /*!< \brief Receiver enable */
#define US_RXDIS                0x00000020      /*!< \brief Receiver disable */
#define US_TXEN                 0x00000040      /*!< \brief Transmitter enable */
#define US_TXDIS                0x00000080      /*!< \brief Transmitter disable */
#define US_RSTSTA               0x00000100      /*!< \brief Reset status bits */
#define US_STTBRK               0x00000200      /*!< \brief Start break */
#define US_STPBRK               0x00000400      /*!< \brief Stop break */
#define US_STTTO                0x00000800      /*!< \brief Start timeout */
#define US_SENDA                0x00001000      /*!< \brief Send next byte with address bit set. */
/*@}*/

/*! \name Mode Register */
/*@{*/
#define US_MR_OFF               0x00000004      /*!< \brief USART mode register offset. */
#define US0_MR  (USART0_BASE + US_MR_OFF)       /*!< \brief Channel 0 mode register address. */
#define US1_MR  (USART1_BASE + US_MR_OFF)       /*!< \brief Channel 1 mode register address. */

#define US_CLKS                 0x00000030      /*!< \brief Clock selection mask. */
#define US_CLKS_MCK             0x00000000      /*!< \brief Master clock. */
#define US_CLKS_MCK8            0x00000010      /*!< \brief Master clock divided by 8. */
#define US_CLKS_SCK             0x00000020      /*!< \brief External clock. */
#define US_CLKS_SLCK            0x00000030      /*!< \brief Slow clock. */

#define US_CHRL                 0x000000C0      /*!< \brief Masks data length. */
#define US_CHRL_5               0x00000000      /*!< \brief 5 data bits. */
#define US_CHRL_6               0x00000040      /*!< \brief 6 data bits. */
#define US_CHRL_7               0x00000080      /*!< \brief 7 data bits. */
#define US_CHRL_8               0x000000C0      /*!< \brief 8 data bits. */

#define US_SYNC                 0x00000100      /*!< \brief Synchronous mode enable. */

#define US_PAR                  0x00000E00      /*!< \brief Parity mode mask. */
#define US_PAR_EVEN             0x00000000      /*!< \brief Even parity */
#define US_PAR_ODD              0x00000200      /*!< \brief Odd parity */
#define US_PAR_SPACE            0x00000400      /*!< \brief Space parity. */
#define US_PAR_MARK             0x00000600      /*!< \brief Marked parity. */
#define US_PAR_NO               0x00000800      /*!< \brief No parity. */
#define US_PAR_MULTIDROP        0x00000C00      /*!< \brief Multi-drop mode. */

#define US_NBSTOP               0x00003000      /*!< \brief Masks stop bit length. */
#define US_NBSTOP_1             0x00000000      /*!< \brief 1 stop bit. */
#define US_NBSTOP_1_5           0x00001000      /*!< \brief 1.5 stop bits. */
#define US_NBSTOP_2             0x00002000      /*!< \brief 2 stop bits. */

#define US_CHMODE                   0x0000C000  /*!< \brief Channel mode mask. */
#define US_CHMODE_NORMAL            0x00000000  /*!< \brief Normal mode. */
#define US_CHMODE_AUTOMATIC_ECHO    0x00004000  /*!< \brief Automatic echo. */
#define US_CHMODE_LOCAL_LOOPBACK    0x00008000  /*!< \brief Local loopback. */
#define US_CHMODE_REMOTE_LOOPBACK   0x0000C000  /*!< \brief Remote loopback. */

#define US_MODE9                0x00020000      /*!< \brief 9 bit mode. */

#define US_CLKO                 0x00040000      /*!< \brief Baud rate output enable */
/*@}*/

/*! \name Status and Interrupt Register */
/*@{*/
#define US_CSR_OFF              0x00000014      /*!< \brief USART status register offset. */
#define US0_CSR (USART0_BASE + US_CSR_OFF)      /*!< \brief Channel 0 status register address. */
#define US1_CSR (USART1_BASE + US_CSR_OFF)      /*!< \brief Channel 1 status register address. */

#define US_IER_OFF              0x00000008      /*!< \brief USART interrupt enable register offset. */
#define US0_IER (USART0_BASE + US_IER_OFF)      /*!< \brief Channel 0 interrupt enable register address. */
#define US1_IER (USART1_BASE + US_IER_OFF)      /*!< \brief Channel 1 interrupt enable register address. */

#define US_IDR_OFF              0x0000000C      /*!< \brief USART interrupt disable register offset. */
#define US0_IDR (USART0_BASE + US_IDR_OFF)      /*!< \brief Channel 0 interrupt disable register address. */
#define US1_IDR (USART1_BASE + US_IDR_OFF)      /*!< \brief Channel 1 interrupt disable register address. */

#define US_IMR_OFF              0x00000010      /*!< \brief USART interrupt mask register offset. */
#define US0_IMR (USART0_BASE + US_IMR_OFF)      /*!< \brief Channel 0 interrupt mask register address. */
#define US1_IMR (USART1_BASE + US_IMR_OFF)      /*!< \brief Channel 1 interrupt mask register address. */

#define US_RXRDY                0x00000001      /*!< \brief Receiver ready */
#define US_TXRDY                0x00000002      /*!< \brief Transmitter ready */
#define US_RXBRK                0x00000004      /*!< \brief Receiver break */
#define US_ENDRX                0x00000008      /*!< \brief End of receiver PDC transfer */
#define US_ENDTX                0x00000010      /*!< \brief End of transmitter PDC transfer */
#define US_OVRE                 0x00000020      /*!< \brief Overrun error */
#define US_FRAME                0x00000040      /*!< \brief Framing error */
#define US_PARE                 0x00000080      /*!< \brief Parity error */
#define US_TIMEOUT              0x00000100      /*!< \brief Receiver timeout */
#define US_TXEMPTY              0x00000200      /*!< \brief Transmitter empty */

/*! \brief Baud rate calculation helper macro. 
 *
 * \deprecated Use NutGetCpuClock() and calculate the divider value locally.
 */
#define AT91_US_BAUD(baud) ((NUT_CPU_FREQ / (8 * (baud)) + 1) / 2)
/*@}*/

/*! \name Receiver Holding Register */
/*@{*/
#define US_RHR_OFF              0x00000018      /*!< \brief USART receiver holding register offset. */
#define US0_RHR (USART0_BASE + US_RHR_OFF)      /*!< \brief Channel 0 receiver holding register address. */
#define US1_RHR (USART1_BASE + US_RHR_OFF)      /*!< \brief Channel 1 receiver holding register address. */
/*@}*/

/*! \name Transmitter Holding Register */
/*@{*/
#define US_THR_OFF              0x0000001C      /*!< \brief USART transmitter holding register offset. */
#define US0_THR (USART0_BASE + US_THR_OFF)      /*!< \brief Channel 0 transmitter holding register address. */
#define US1_THR (USART1_BASE + US_THR_OFF)      /*!< \brief Channel 1 transmitter holding register address. */
/*@}*/

/*! \name Baud Rate Generator Register */
/*@{*/
#define US_BRGR_OFF             0x00000020      /*!< \brief USART baud rate register offset. */
#define US0_BRGR (USART0_BASE + US_BRGR_OFF)    /*!< \brief Channel 0 baud rate register address. */
#define US1_BRGR (USART1_BASE + US_BRGR_OFF)    /*!< \brief Channel 1 baud rate register address. */
/*@}*/

/*! \name Receiver Timeout Register */
/*@{*/
#define US_RTOR_OFF             0x00000024      /*!< \brief USART receiver timeout register offset. */
#define US0_RTOR (USART0_BASE + US_RTOR_OFF)    /*!< \brief Channel 0 receiver timeout register address. */
#define US1_RTOR (USART1_BASE + US_RTOR_OFF)    /*!< \brief Channel 1 receiver timeout register address. */
/*@}*/

/*! \name Transmitter Time Guard Register */
/*@{*/
#define US_TTGR_OFF             0x00000028      /*!< \brief USART transmitter time guard register offset. */
#define US0_TTGR (USART0_BASE + US_TTGR_OFF)    /*!< \brief Channel 0 transmitter time guard register address. */
#define US1_TTGR (USART1_BASE + US_TTGR_OFF)    /*!< \brief Channel 1 transmitter time guard register address. */
/*@}*/

/*! \name Receive Pointer Register */
/*@{*/
#define US_RPR_OFF              0x00000030      /*!< \brief USART receive pointer register offset. */
#define US0_RPR (USART0_BASE + US_RPR_OFF)      /*!< \brief Channel 0 receive pointer register address. */
#define US1_RPR (USART1_BASE + US_RPR_OFF)      /*!< \brief Channel 1 receive pointer register address. */
/*@}*/

/*! \name Receive Counter Register */
/*@{*/
#define US_RCR_OFF              0x00000034      /*!< \brief USART receive counter register offset. */
#define US0_RCR (USART0_BASE + US_RCR_OFF)      /*!< \brief Channel 0 receive counter register address. */
#define US1_RCR (USART1_BASE + US_RCR_OFF)      /*!< \brief Channel 1 receive counter register address. */
/*@}*/

/*! \name Transmit Pointer Register */
/*@{*/
#define US_TPR_OFF              0x00000038      /*!< \brief USART transmit pointer register offset. */
#define US0_TPR (USART0_BASE + US_TPR_OFF)      /*!< \brief Channel 0 transmit pointer register address. */
#define US1_TPR (USART1_BASE + US_TPR_OFF)      /*!< \brief Channel 1 transmit pointer register address. */
/*@}*/

/*! \name Transmit Counter Register */
/*@{*/
#define US_TCR_OFF              0x0000003C      /*!< \brief USART transmit counter register offset. */
#define US0_TCR (USART0_BASE + US_TCR_OFF)      /*!< \brief Channel 0 transmit counter register address. */
#define US1_TCR (USART1_BASE + US_TCR_OFF)      /*!< \brief Channel 1 transmit counter register address. */
/*@}*/

/*@} xgNutArchArmAt91Us */


#endif                          /* _ARCH_ARM_AT91_US_H_ */
