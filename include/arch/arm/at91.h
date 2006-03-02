#ifndef _ARCH_ARM_AT91_H_
#define _ARCH_ARM_AT91_H_

/*
 * Copyright (C) 2005 by egnite Software GmbH. All rights reserved.
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
 * \file arch/arm/at91.h
 * \brief AT91 peripherals.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.4  2006/03/02 20:02:56  haraldkipp
 * Added ICCARM interrupt entry code. Probably not working, because I
 * excluded an immediate load.
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

/*!
 * \addtogroup xgNutArchArmAt91Ebi
 */
/*@{*/

#define EBI_BASE                0xFFE00000      /*!< \brief EBI base address. */

/*! \name Chip Select Register */
/*@{*/
#define EBI_CSR(i)      (EBI_BASE + i * 4)      /*!< \brief Chip select register address. */

#define EBI_DBW                 0x00000003      /*!< \brief Masks data bus width. */
#define EBI_DBW_16              0x00000001      /*!< \brief 16-bit data bus width. */
#define EBI_DBW_8               0x00000002      /*!< \brief 8-bit data bus width. */

#define EBI_NWS                 0x0000001C      /*!< \brief Masks number of wait states. */
#define EBI_NWS_1               0x00000000      /*!< \brief 1 wait state. */
#define EBI_NWS_2               0x00000004      /*!< \brief 2 wait states. */
#define EBI_NWS_3               0x00000008      /*!< \brief 3 wait states. */
#define EBI_NWS_4               0x0000000C      /*!< \brief 4 wait states. */
#define EBI_NWS_5               0x00000010      /*!< \brief 5 wait states. */
#define EBI_NWS_6               0x00000014      /*!< \brief 6 wait states. */
#define EBI_NWS_7               0x00000018      /*!< \brief 7 wait states. */
#define EBI_NWS_8               0x0000001C      /*!< \brief 8 wait states. */

#define EBI_WSE                 0x00000020      /*!< \brief Wait state enable. */

#define EBI_PAGES               0x00000180      /*!< \brief Page size mask. */
#define EBI_PAGES_1M            0x00000000      /*!< \brief 1 MByte page size. */
#define EBI_PAGES_4M            0x00000080      /*!< \brief 4 MBytes page size. */
#define EBI_PAGES_16M           0x00000100      /*!< \brief 16 MBytes page size. */
#define EBI_PAGES_64M           0x00000180      /*!< \brief 64 MBytes page size. */

#define EBI_TDF                 0x00000E00      /*!< \brief Masks data float output time clock cycles. */
#define EBI_TDF_0               0x00000000      /*!< \brief No added cycles. */
#define EBI_TDF_1               0x00000200      /*!< \brief 1 cycle. */
#define EBI_TDF_2               0x00000400      /*!< \brief 2 cycles. */
#define EBI_TDF_3               0x00000600      /*!< \brief 3 cycles. */
#define EBI_TDF_4               0x00000800      /*!< \brief 4 cycles. */
#define EBI_TDF_5               0x00000A00      /*!< \brief 5 cycles. */
#define EBI_TDF_6               0x00000C00      /*!< \brief 6 cycles. */
#define EBI_TDF_7               0x00000E00      /*!< \brief 7 cycles. */

#define EBI_BAT                 0x00001000      /*!< \brief Byte access type */
#define EBI_BAT_BYTE_WRITE      0x00000000      /*!< \brief Byte write access type */
#define EBI_BAT_BYTE_SELECT     0x00001000      /*!< \brief Byte select access type */

#define EBI_CSEN                0x00002000      /*!< \brief Chip select enable */

#define EBI_BA                  0xFFF00000      /*!< \brief Page base address mask. */
/*@}*/

/*! \name Remap Control Register */
/*@{*/
#define EBI_RCR         (EBI_BASE + 0x20)       /*!< \brief Remap control register address. */
#define EBI_RCB                 0x00000001      /*!< \brief Remap command. */
/*@}*/

/*! \name Memory Control Register */
/*@{*/
#define EBI_MCR         (EBI_BASE + 0x24)       /*!< \brief Memory control register address. */
#define EBI_ALE                 0x00000007      /*!< \brief Address line enable */
#define EBI_ALE_16M             0x00000000      /*!< \brief 16 Mbytes total address space. */
#define EBI_ALE_8M              0x00000004      /*!< \brief 8 Mbytes total address space. */
#define EBI_ALE_4M              0x00000005      /*!< \brief 4 Mbytes total address space. */
#define EBI_ALE_2M              0x00000006      /*!< \brief 2 Mbytes total address space. */
#define EBI_ALE_1M              0x00000007      /*!< \brief 1 Mbyte total address space. */

#define EBI_DRP                 0x00000010      /*!< \brief Data read protocol mask. */
#define EBI_DRP_STANDARD        0x00000000      /*!< \brief Standard read protocol. */
#define EBI_DRP_EARLY           0x00000010      /*!< \brief Early read protocol. */
/*@}*/

/*@} xgNutArchArmAt91Ebi */

/*!
 * \addtogroup xgNutArchArmAt91Sf
 */
/*@{*/

#define SF_BASE                 0xFFF00000      /*!< \brief Special function register base address. */

/*! \name Chip Identification Registers */
/*@{*/
#define SF_CIDR         (SF_BASE + 0x00)        /*!< \brief Chip ID register address. */
#define SF_EXID         (SF_BASE + 0x04)        /*!< \brief Chip ID extension register address. */
#define SF_VERSION              0x0000001F      /*!< \brief Version number mask. */

#define SF_NVPSIZ               0x00000F00      /*!< \brief Masks non volatile program memory size. */
#define SF_NVPSIZ_NONE          0x00000000      /*!< \brief No NV program memory. */
#define SF_NVPSIZ_32K           0x00000300      /*!< \brief 32 kBytes NV program memory. */
#define SF_NVPSIZ_64K           0x00000500      /*!< \brief 64 kBytes NV program memory. */
#define SF_NVPSIZ_128K          0x00000700      /*!< \brief 128 kBytes NV program memory. */
#define SF_NVPSIZ_256K          0x00000900      /*!< \brief 256 kBytes NV program memory. */

#define SF_NVDSIZ               0x0000F000      /*!< \brief Masks non volatile data memory size. */
#define SF_NVDSIZ_NONE          0x00000000      /*!< \brief No NV data memory. */

#define SF_VDSIZ                0x000F0000      /*!< \brief Masks volatile data memory size. */
#define SF_VDSIZ_NONE           0x00000000      /*!< \brief No volatile data memory. */
#define SF_VDSIZ_1K             0x00010000      /*!< \brief 1 kBytes volatile data memory. */
#define SF_VDSIZ_2K             0x00020000      /*!< \brief 2 kBytes volatile data memory. */
#define SF_VDSIZ_4K             0x00040000      /*!< \brief 4 kBytes volatile data memory. */
#define SF_VDSIZ_8K             0x00080000      /*!< \brief 8 kBytes volatile data memory. */

#define SF_ARCH                 0x0FF00000      /*!< \brief Architecture code mask. */
#define SF_ARCH_AT91x40         0x04000000      /*!< \brief AT91x40 architecture. */
#define SF_ARCH_AT91x55         0x05500000      /*!< \brief AT91x55 architecture. */
#define SF_ARCH_AT91x63         0x06300000      /*!< \brief AT91x63 architecture. */

#define SF_NVPTYP               0x70000000      /*!< \brief Masks non volatile program memory type. */
#define SF_NVPTYP_M             0x01000000      /*!< \brief M or F series. */
#define SF_NVPTYP_C             0x02000000      /*!< \brief C series. */
#define SF_NVPTYP_S             0x03000000      /*!< \brief S series. */
#define SF_NVPTYP_R             0x04000000      /*!< \brief R series. */

#define SF_EXT                  0x80000000      /*!< \brief Extension flag. */

/*@}*/

/*! \name Reset Status Flag Register */
/*@{*/
#define SF_RSR          (SF_BASE + 0x08)        /*!< \brief Reset status register address. */
#define SF_EXT_RESET            0x0000006C      /*!< \brief Reset caused by external pin. */
#define SF_WD_RESET             0x00000053      /*!< \brief Reset caused by internal watch dog. */
/*@}*/

/*! \name Memory Mode Register */
/*@{*/
#define SF_MMR          (SF_BASE + 0x0C)        /*!< \brief Memory mode register address. */
#define SF_RAMWU                0x00000001      /*!< \brief Internal extended RAM write allowed. */
/*@}*/

/*! \name Protect Mode Register */
/*@{*/
#define SF_PMR          (SF_BASE + 0x18)        /*!< \brief Protect mode register address. */
#define SF_AIC                  0x00000020      /*!< \brief AIC runs in protect mode. */
/*@}*/

/*@} xgNutArchArmAt91Sf */

/*!
 * \addtogroup xgNutArchArmAt91Us
 */
/*@{*/

#define USART1_BASE             0xFFFCC000      /*!< \brief USART 1 base address. */
#define USART0_BASE             0xFFFD0000      /*!< \brief USART 0 base address. */

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

/*!
 * \addtogroup xgNutArchArmAt91Tc
 */
/*@{*/
#define TC_BASE                 0xFFFE0000      /*!< \brief TC base address. */

/*! \name Timer Counter Control Register */
/*@{*/
#define TC0_CCR         (TC_BASE + 0x00)        /*!< \brief Channel 0 control register address. */
#define TC1_CCR         (TC_BASE + 0x40)        /*!< \brief Channel 1 control register address. */
#define TC2_CCR         (TC_BASE + 0x80)        /*!< \brief Channel 2 control register address. */
#define TC_CLKEN                0x00000001      /*!< \brief Clock enable command. */
#define TC_CLKDIS               0x00000002      /*!< \brief Clock disable command. */
#define TC_SWTRG                0x00000004      /*!< \brief Software trigger command. */
/*@}*/

/*! \name Timer Counter Channel Mode Register */
/*@{*/
#define TC0_CMR         (TC_BASE + 0x04)        /*!< \brief Channel 0 mode register address. */
#define TC1_CMR         (TC_BASE + 0x44)        /*!< \brief Channel 1 mode register address. */
#define TC2_CMR         (TC_BASE + 0x84)        /*!< \brief Channel 2 mode register address. */

#define TC_CLKS                 0x00000007      /*!< \brief Clock selection mask. */
#define TC_CLKS_MCK2            0x00000000      /*!< \brief Selects MCK / 2. */
#define TC_CLKS_MCK8            0x00000001      /*!< \brief Selects MCK / 8. */
#define TC_CLKS_MCK32           0x00000002      /*!< \brief Selects MCK / 32. */
#define TC_CLKS_MCK128          0x00000003      /*!< \brief Selects MCK / 128. */
#define TC_CLKS_MCK1024         0x00000004      /*!< \brief Selects MCK / 1024. */
#define TC_CLKS_XC0             0x00000005      /*!< \brief Selects external clock 0. */
#define TC_CLKS_XC1             0x00000006      /*!< \brief Selects external clock 1. */
#define TC_CLKS_XC2             0x00000007      /*!< \brief Selects external clock 2. */

#define TC_CLKI                 0x00000008      /*!< \brief Increments on falling edge. */

#define TC_BURST                0x00000030      /*!< \brief Burst signal selection mask. */
#define TC_BURST_NONE           0x00000000      /*!< \brief Clock is not gated by an external signal. */
#define TC_BUSRT_XC0            0x00000010      /*!< \brief ANDed with external clock 0. */
#define TC_BURST_XC1            0x00000020      /*!< \brief ANDed with external clock 1. */
#define TC_BURST_XC2            0x00000030      /*!< \brief ANDed with external clock 2. */

#define TC_CPCTRG               0x00004000      /*!< \brief RC Compare Enable Trigger Enable. */

#define TC_WAVE                 0x00008000      /*!< \brief Selects waveform mode. */
#define TC_CAPT                 0x00000000      /*!< \brief Selects capture mode. */
/*@}*/

/*! \name Capture Mode */
/*@{*/
#define TC_LDBSTOP              0x00000040      /*!< \brief Counter clock stopped on RB loading. */
#define TC_LDBDIS               0x00000080      /*!< \brief Counter clock disabled on RB loading. */

#define TC_ETRGEDG              0x00000300      /*!< \brief External trigger edge selection mask. */
#define TC_ETRGEDG_RISING_EDGE  0x00000100      /*!< \brief Trigger on external rising edge. */
#define TC_ETRGEDG_FALLING_EDGE 0x00000200      /*!< \brief Trigger on external falling edge. */
#define TC_ETRGEDG_BOTH_EDGE    0x00000300      /*!< \brief Trigger on both external edges. */

#define TC_ABETRG               0x00000400      /*!< \brief TIOA or TIOB external trigger selection mask. */
#define TC_ABETRG_TIOB          0x00000000      /*!< \brief TIOB used as an external trigger. */
#define TC_ABETRG_TIOA          0x00000400      /*!< \brief TIOA used as an external trigger. */

#define TC_LDRA                 0x00030000      /*!< \brief RA loading selection mask. */
#define TC_LDRA_RISING_EDGE     0x00010000      /*!< \brief Load RA on rising edge of TIOA. */
#define TC_LDRA_FALLING_EDGE    0x00020000      /*!< \brief Load RA on falling edge of TIOA. */
#define TC_LDRA_BOTH_EDGE       0x00030000      /*!< \brief Load RA on any edge of TIOA. */

#define TC_LDRB                 0x000C0000      /*!< \brief RB loading selection mask. */
#define TC_LDRB_RISING_EDGE     0x00040000      /*!< \brief Load RB on rising edge of TIOA. */
#define TC_LDRB_FALLING_EDGE    0x00080000      /*!< \brief Load RB on falling edge of TIOA. */
#define TC_LDRB_BOTH_EDGE       0x000C0000      /*!< \brief Load RB on any edge of TIOA. */

/*@}*/

/*! \name Waveform Mode */
/*@{*/
#define TC_CPCSTOP              0x00000040      /*!< \brief Counter clock stopped on RC compare. */
#define TC_CPCDIS               0x00000080      /*!< \brief Counter clock disabled on RC compare. */

#define TC_EEVTEDG              0x00000300      /*!< \brief External event edge selection mask. */
#define TC_EEVTEDG_RISING_EDGE  0x00000100      /*!< \brief External event on rising edge.. */
#define TC_EEVTEDG_FALLING_EDGE 0x00000200      /*!< \brief External event on falling edge.. */
#define TC_EEVTEDG_BOTH_EDGE    0x00000300      /*!< \brief External event on any edge.. */

#define TC_EEVT                 0x00000C00      /*!< \brief External event selection mask. */
#define TC_EEVT_TIOB            0x00000000      /*!< \brief TIOB selected as external event. */
#define TC_EEVT_XC0             0x00000400      /*!< \brief XC0 selected as external event. */
#define TC_EEVT_XC1             0x00000800      /*!< \brief XC1 selected as external event. */
#define TC_EEVT_XC2             0x00000C00      /*!< \brief XC2 selected as external event. */

#define TC_ENETRG               0x00001000      /*!< \brief External event trigger enable. */

#define TC_ACPA                 0x00030000      /*!< \brief Masks RA compare effect on TIOA. */
#define TC_ACPA_SET_OUTPUT      0x00010000      /*!< \brief RA compare sets TIOA. */
#define TC_ACPA_CLEAR_OUTPUT    0x00020000      /*!< \brief RA compare clears TIOA. */
#define TC_ACPA_TOGGLE_OUTPUT   0x00030000      /*!< \brief RA compare toggles TIOA. */

#define TC_ACPC                 0x000C0000      /*!< \brief Masks RC compare effect on TIOA. */
#define TC_ACPC_SET_OUTPUT      0x00040000      /*!< \brief RC compare sets TIOA. */
#define TC_ACPC_CLEAR_OUTPUT    0x00080000      /*!< \brief RC compare clears TIOA. */
#define TC_ACPC_TOGGLE_OUTPUT   0x000C0000      /*!< \brief RC compare toggles TIOA. */

#define TC_AEEVT                0x00300000      /*!< \brief Masks external event effect on TIOA. */
#define TC_AEEVT_SET_OUTPUT     0x00100000      /*!< \brief External event sets TIOA. */
#define TC_AEEVT_CLEAR_OUTPUT   0x00200000      /*!< \brief External event clears TIOA. */
#define TC_AEEVT_TOGGLE_OUTPUT  0x00300000      /*!< \brief External event toggles TIOA. */

#define TC_ASWTRG               0x00C00000      /*!< \brief Masks software trigger effect on TIOA. */
#define TC_ASWTRG_SET_OUTPUT    0x00400000      /*!< \brief Software trigger sets TIOA. */
#define TC_ASWTRG_CLEAR_OUTPUT  0x00800000      /*!< \brief Software trigger clears TIOA. */
#define TC_ASWTRG_TOGGLE_OUTPUT 0x00C00000      /*!< \brief Software trigger toggles TIOA. */

#define TC_BCPB                 0x03000000      /*!< \brief Masks RB compare effect on TIOB. */
#define TC_BCPB_SET_OUTPUT      0x01000000      /*!< \brief RB compare sets TIOB. */
#define TC_BCPB_CLEAR_OUTPUT    0x02000000      /*!< \brief RB compare clears TIOB. */
#define TC_BCPB_TOGGLE_OUTPUT   0x03000000      /*!< \brief RB compare toggles TIOB. */

#define TC_BCPC                 0x0C000000      /*!< \brief Masks RC compare effect on TIOB. */
#define TC_BCPC_SET_OUTPUT      0x04000000      /*!< \brief RC compare sets TIOB. */
#define TC_BCPC_CLEAR_OUTPUT    0x08000000      /*!< \brief RC compare clears TIOB. */
#define TC_BCPC_TOGGLE_OUTPUT   0x0C000000      /*!< \brief RC compare toggles TIOB. */

#define TC_BEEVT                0x30000000      /*!< \brief Masks external event effect on TIOB. */
#define TC_BEEVT_SET_OUTPUT     0x10000000      /*!< \brief External event sets TIOB. */
#define TC_BEEVT_CLEAR_OUTPUT   0x20000000      /*!< \brief External event clears TIOB. */
#define TC_BEEVT_TOGGLE_OUTPUT  0x30000000      /*!< \brief External event toggles TIOB. */

#define TC_BSWTRG               0xC0000000      /*!< \brief Masks software trigger effect on TIOB. */
#define TC_BSWTRG_SET_OUTPUT    0x40000000      /*!< \brief Software trigger sets TIOB. */
#define TC_BSWTRG_CLEAR_OUTPUT  0x80000000      /*!< \brief Software trigger clears TIOB. */
#define TC_BSWTRG_TOGGLE_OUTPUT 0xC0000000      /*!< \brief Software trigger toggles TIOB. */
/*@}*/

/*! \name Counter Value Register */
/*@{*/
#define TC0_CV          (TC_BASE + 0x10)        /*!< \brief Counter 0 value. */
#define TC1_CV          (TC_BASE + 0x50)        /*!< \brief Counter 1 value. */
#define TC2_CV          (TC_BASE + 0x90)        /*!< \brief Counter 2 value. */
/*@}*/

/*! \name Timer Counter Register A */
/*@{*/
#define TC0_RA          (TC_BASE + 0x14)        /*!< \brief Channel 0 register A. */
#define TC1_RA          (TC_BASE + 0x54)        /*!< \brief Channel 1 register A. */
#define TC2_RA          (TC_BASE + 0x94)        /*!< \brief Channel 2 register A. */
/*@}*/

/*! \name Timer Counter Register B */
/*@{*/
#define TC0_RB          (TC_BASE + 0x18)        /*!< \brief Channel 0 register B. */
#define TC1_RB          (TC_BASE + 0x58)        /*!< \brief Channel 1 register B. */
#define TC2_RB          (TC_BASE + 0x98)        /*!< \brief Channel 2 register B. */
/*@}*/

/*! \name Timer Counter Register C */
/*@{*/
#define TC0_RC          (TC_BASE + 0x1C)        /*!< \brief Channel 0 register C. */
#define TC1_RC          (TC_BASE + 0x5C)        /*!< \brief Channel 1 register C. */
#define TC2_RC          (TC_BASE + 0x9C)        /*!< \brief Channel 2 register C. */
/*@}*/


/*! \name Timer Counter Status and Interrupt Registers */
/*@{*/
#define TC0_SR          (TC_BASE + 0x20)        /*!< \brief Status register address. */
#define TC1_SR          (TC_BASE + 0x60)        /*!< \brief Status register address. */
#define TC2_SR          (TC_BASE + 0xA0)        /*!< \brief Status register address. */

#define TC0_IER         (TC_BASE + 0x24)        /*!< \brief Channel 0 interrupt enable register address. */
#define TC1_IER         (TC_BASE + 0x64)        /*!< \brief Channel 1 interrupt enable register address. */
#define TC2_IER         (TC_BASE + 0xA4)        /*!< \brief Channel 2 interrupt enable register address. */

#define TC0_IDR         (TC_BASE + 0x28)        /*!< \brief Channel 0 interrupt disable register address. */
#define TC1_IDR         (TC_BASE + 0x68)        /*!< \brief Channel 1 interrupt disable register address. */
#define TC2_IDR         (TC_BASE + 0xA8)        /*!< \brief Channel 2 interrupt disable register address. */

#define TC0_IMR         (TC_BASE + 0x2C)        /*!< \brief Channel 0 interrupt mask register address. */
#define TC1_IMR         (TC_BASE + 0x6C)        /*!< \brief Channel 1 interrupt mask register address. */
#define TC2_IMR         (TC_BASE + 0xAC)        /*!< \brief Channel 2 interrupt mask register address. */

#define TC_COVFS                0x00000001      /*!< \brief Counter overflow flag. */
#define TC_LOVRS                0x00000002      /*!< \brief Load overrun flag. */
#define TC_CPAS                 0x00000004      /*!< \brief RA compare flag. */
#define TC_CPBS                 0x00000008      /*!< \brief RB compare flag. */
#define TC_CPCS                 0x00000010      /*!< \brief RC compare flag. */
#define TC_LDRAS                0x00000020      /*!< \brief RA loading flag. */
#define TC_LDRBS                0x00000040      /*!< \brief RB loading flag. */
#define TC_ETRGS                0x00000080      /*!< \brief External trigger flag. */
#define TC_CLKSTA               0x00010000      /*!< \brief Clock enable flag. */
#define TC_MTIOA                0x00020000      /*!< \brief TIOA flag. */
#define TC_MTIOB                0x00040000      /*!< \brief TIOB flag. */
/*@}*/

/*! \name Timer Counter Block Control Register */
/*@{*/
#define TC_BCR          (TC_BASE + 0xC0)        /*!< \brief Block control register address. */
#define TC_SYNC                 0x00000001      /*!< \brief Synchronisation trigger */
/*@}*/

/*! \name Timer Counter Block Mode Register */
/*@{*/
#define TC_BMR          (TC_BASE + 0xC4)        /*!< \brief Block mode register address. */
#define TC_TC0XC0S              0x00000003      /*!< \brief External clock signal 0 selection mask. */
#define TC_TCLK0XC0             0x00000000      /*!< \brief Selects TCLK0. */
#define TC_NONEXC0              0x00000001      /*!< \brief None selected. */
#define TC_TIOA1XC0             0x00000002      /*!< \brief Selects TIOA1. */
#define TC_TIOA2XC0             0x00000003      /*!< \brief Selects TIOA2. */

#define TC_TC1XC1S              0x0000000C      /*!< \brief External clock signal 1 selection mask. */
#define TC_TCLK1XC1             0x00000000      /*!< \brief Selects TCLK1. */
#define TC_NONEXC1              0x00000004      /*!< \brief None selected. */
#define TC_TIOA0XC1             0x00000008      /*!< \brief Selects TIOA0. */
#define TC_TIOA2XC1             0x0000000C      /*!< \brief Selects TIOA2. */

#define TC_TC2XC2S              0x00000030      /*!< \brief External clock signal 2 selection mask. */
#define TC_TCLK2XC2             0x00000000      /*!< \brief Selects TCLK2. */
#define TC_NONEXC2              0x00000010      /*!< \brief None selected. */
#define TC_TIOA0XC2             0x00000020      /*!< \brief Selects TIOA0. */
#define TC_TIOA1XC2             0x00000030      /*!< \brief Selects TIOA1. */
/*@}*/

/*@} xgNutArchArmAt91Tc */

/*!
 * \addtogroup xgNutArchArmAt91Pio
 */
/*@{*/
#define PIO_BASE    0xFFFF0000  /*!< \brief PIO base address. */
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

/*@} xgNutArchArmAt91Pio */

/*!
 * \addtogroup xgNutArchArmAt91Ps
 */
/*@{*/
#define PS_BASE     0xFFFF4000  /*!< \brief PS base address. */

/*!
 * \name PS Control Register
 */
/*@{*/

/*! \brief Register address.
 *
 * This register allows to stop the CPU clock. The clock is automatically
 * enabled after reset and by any interrupt.
 */
#define PS_CR       (PS_BASE + 0x00)
/*@}*/

/*!
 * \name Peripheral Clock Control Registers
 */
/*@{*/
#define PS_PCER     (PS_BASE + 0x04)    /*!< \brief Peripheral clock enable register address. */
#define PS_PCDR     (PS_BASE + 0x08)    /*!< \brief Peripheral clock disable register address. */
#define PS_PCSR     (PS_BASE + 0x0C)    /*!< \brief Peripheral clock status register address. */
/*@}*/

/*@} xgNutArchArmAt91Ps */

/*!
 * \addtogroup xgNutArchArmAt91Wd
 */
/*@{*/
#define WD_BASE     0xFFFF8000  /*!< \brief Watch Dog register base address. */

/*! \name Watch Dog Overflow Mode Register */
/*@{*/
#define WD_OMR          (WD_BASE + 0x00)        /*!< \brief Overflow mode register address. */
#define WD_WDEN                 0x00000001      /*!< \brief Watch Dog enable. */
#define WD_RSTEN                0x00000002      /*!< \brief Internal reset enable. */
#define WD_IRQEN                0x00000004      /*!< \brief Interrupt enable. */
#define WD_EXTEN                0x00000008      /*!< \brief External signal enable. */
#define WD_OKEY                 0x00002340      /*!< \brief Overflow mode register access key. */
/*@}*/

/*! \name Watch Dog Clock Register */
/*@{*/
#define WD_CMR          (WD_BASE + 0x04)        /*!< \brief Clock mode register address. */
#define WD_WDCLKS               0x00000003      /*!< \brief Clock selection mask. */
#define WD_WDCLKS_MCK8          0x00000000      /*!< \brief Selects MCK/8. */
#define WD_WDCLKS_MCK32         0x00000001      /*!< \brief Selects MCK/32. */
#define WD_WDCLKS_MCK128        0x00000002      /*!< \brief Selects MCK/128. */
#define WD_WDCLKS_MCK1024       0x00000003      /*!< \brief Selects MCK/1024. */
#define WD_HPCV                 0x0000003C      /*!< \brief High preload counter value. */
#define WD_CKEY                 (0x06E<<7)      /*!< \brief Clock register access key. */
/*@}*/

/*! \name Watch Dog Control Register */
/*@{*/
#define WD_CR           (WD_BASE + 0x08)        /*!< \brief Control register address. */
#define WD_RSTKEY               0x0000C071      /*!< \brief Watch Dog restart key. */
/*@}*/

/*! \name Watch Dog Status Register */
/*@{*/
#define WD_SR           (WD_BASE + 0x0C)        /*!< \brief Status register address. */
#define WD_WDOVF                0x00000001      /*!< \brief Watch Dog overflow status. */
/*@}*/

/*@} xgNutArchArmAt91Wd */

/*!
 * \addtogroup xgNutArchArmAt91Aic
 */
/*@{*/
#define AIC_BASE    0xFFFFF000  /*!< AIC base address. */

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

#ifdef __GNUC__

/*!
 * \brief Interrupt entry.
 */
#define IRQ_ENTRY() \
    asm volatile("sub   lr, lr,#4"          "\n\t"  /* Adjust LR */ \
                 "stmfd sp!,{r0-r12,lr}"    "\n\t"  /* Save registers on IRQ stack. */ \
                 "mrs   r1, spsr"           "\n\t"  /* Save SPSR */ \
                 "stmfd sp!,{r1}"           "\n\t")     /* */

/*!
 * \brief Interrupt exit.
 */
#define IRQ_EXIT() \
    asm volatile("ldmfd sp!, {r1}"          "\n\t"  /* Restore SPSR */ \
                 "msr   spsr_c, r1"         "\n\t"  /* */ \
                 "ldr   r0, =0xFFFFF000"    "\n\t"  /* End of interrupt. */ \
                 "str   r0, [r0, #0x130]"   "\n\t"  /* */ \
                 "ldmfd sp!, {r0-r12, pc}^" "\n\t")     /* Restore registers and return. */


#else /* __IMAGECRAFT__ */

#define IRQ_ENTRY() \
    asm("sub   lr, lr,#4\n" \
        "stmfd sp!,{r0-r12,lr}\n" \
        "mrs   r1, spsr\n" \
        "stmfd sp!,{r1}\n")

#define IRQ_EXIT() \
    asm("ldmfd sp!, {r1}\n" \
        "msr   spsr_c, r1\n" \
        ";ldr   r0, =0xFFFFF000\n" /* ICCARM: FIXME! */ \
        "str   r0, [r0, #0x130]\n" \
        "ldmfd sp!, {r0-r12, pc}^")

#endif

/*@} xgNutArchArmAt91Aic */

/*! \addtogroup xgNutArchArmAt91 */
/*@{*/

/*! \name Peripheral Identifiers and Interrupts */
/*@{*/
#define FIQ_ID      0           /*!< \brief Fast interrupt ID. */
#define SWIRQ_ID    1           /*!< \brief Software interrupt ID. */
#define US0_ID      2           /*!< \brief USART 0 ID. */
#define US1_ID      3           /*!< \brief USART 1 ID. */
#define TC0_ID      4           /*!< \brief Timer 0 ID. */
#define TC1_ID      5           /*!< \brief Timer 1 ID. */
#define TC2_ID      6           /*!< \brief Timer 2 ID. */
#define WDI_ID      7           /*!< \brief Watchdog interrupt ID. */
#define PIO_ID      8           /*!< \brief Parallel I/O controller ID. */
#define IRQ0_ID     16          /*!< \brief External interrupt 0 ID. */
#define IRQ1_ID     17          /*!< \brief External interrupt 1 ID. */
#define IRQ2_ID     18          /*!< \brief External interrupt 2 ID. */
/*@}*/
/*@} xgNutArchArmAt91 */

extern void McuInit(void);

#endif                          /* _ARCH_ARM_AT91_H_ */
