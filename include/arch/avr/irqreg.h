#ifndef _DEV_IRQREG_AVR_H_
#define _DEV_IRQREG_AVR_H_

/*
 * Copyright (C) 2001-2005 by egnite Software GmbH. All rights reserved.
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

/*
 * $Log$
 * Revision 1.3  2005/10/07 21:52:31  hwmaier
 * Removed test for __ATmega64__
 *
 * Revision 1.2  2005/10/04 05:24:24  hwmaier
 * Updated interrupt hooks to reflect new CAN interrupt names of avr-lib 1.2.3
 *
 * Revision 1.1  2005/07/26 18:35:09  haraldkipp
 * First check in
 *
 * Revision 1.2  2005/02/10 07:06:51  hwmaier
 * Changes to incorporate support for AT90CAN128 CPU
 *
 * Revision 1.1  2004/03/16 16:48:28  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 *
 */

extern IRQ_HANDLER sig_INTERRUPT0;
extern IRQ_HANDLER sig_INTERRUPT1;
extern IRQ_HANDLER sig_INTERRUPT2;
extern IRQ_HANDLER sig_INTERRUPT3;
extern IRQ_HANDLER sig_INTERRUPT4;
extern IRQ_HANDLER sig_INTERRUPT5;
extern IRQ_HANDLER sig_INTERRUPT6;
extern IRQ_HANDLER sig_INTERRUPT7;
extern IRQ_HANDLER sig_OUTPUT_COMPARE2;
extern IRQ_HANDLER sig_OVERFLOW2;
extern IRQ_HANDLER sig_INPUT_CAPTURE1;
extern IRQ_HANDLER sig_OUTPUT_COMPARE1A;
extern IRQ_HANDLER sig_OUTPUT_COMPARE1B;
extern IRQ_HANDLER sig_OVERFLOW1;
extern IRQ_HANDLER sig_OUTPUT_COMPARE0;
extern IRQ_HANDLER sig_OVERFLOW0;
extern IRQ_HANDLER sig_SPI;
extern IRQ_HANDLER sig_UART0_TRANS;
extern IRQ_HANDLER sig_UART0_DATA;
extern IRQ_HANDLER sig_UART0_RECV;
extern IRQ_HANDLER sig_ADC;
extern IRQ_HANDLER sig_EEPROM_READY;
extern IRQ_HANDLER sig_COMPARATOR;
#if defined(__AVR_AT90CAN128__) || defined(__AVR_ATmega128__)
extern IRQ_HANDLER sig_OUTPUT_COMPARE1C;
extern IRQ_HANDLER sig_INPUT_CAPTURE3;
extern IRQ_HANDLER sig_OUTPUT_COMPARE3A;
extern IRQ_HANDLER sig_OUTPUT_COMPARE3B;
extern IRQ_HANDLER sig_OUTPUT_COMPARE3C;
extern IRQ_HANDLER sig_OVERFLOW3;
extern IRQ_HANDLER sig_UART1_RECV;
extern IRQ_HANDLER sig_UART1_DATA;
extern IRQ_HANDLER sig_UART1_TRANS;
extern IRQ_HANDLER sig_2WIRE_SERIAL;
extern IRQ_HANDLER sig_SPM_READY;
#endif
#if defined(__AVR_AT90CAN128__)
extern IRQ_HANDLER sig_CAN_TRANSFER;
extern IRQ_HANDLER sig_CAN_OVERRUN;
#endif

#endif
