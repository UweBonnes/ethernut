#ifndef _DEV_IRQREG_H_
#define _DEV_IRQREG_H_

/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.1  2003/05/09 14:41:06  haraldkipp
 * Initial revision
 *
 * Revision 1.10  2003/05/06 18:42:55  harald
 * Cleanup
 *
 * Revision 1.9  2003/02/04 18:00:37  harald
 * Version 3 released
 *
 * Revision 1.8  2002/07/03 16:45:40  harald
 * Using GCC 3.2
 *
 * Revision 1.7  2002/06/26 17:29:15  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/device.h>

/*!
 * \file dev/irqreg.h
 * \brief Interrupt management definitions.
 */

/*!
 * \addtogroup xgIrqReg
 */
/*@{*/

enum {
    IRQ_INT0,
    IRQ_INT1,
    IRQ_INT2,
    IRQ_INT3,
    IRQ_INT4,
    IRQ_INT5,
    IRQ_INT6,
    IRQ_INT7,
    IRQ_TIMER2_COMP,
    IRQ_TIMER2_OVF,
    IRQ_TIMER1_CAPT,
    IRQ_TIMER1_COMPA,
    IRQ_TIMER1_COMPB,
    IRQ_TIMER1_OVF,
    IRQ_TIMER0_COMP,
    IRQ_TIMER0_OVF,
    IRQ_SPI_STC,
    IRQ_UART_RX,
    IRQ_UART_UDRE,
    IRQ_UART_TX,
    IRQ_ADC,
    IRQ_EE_RDY,
    IRQ_ANA_COMP,
#ifdef __AVR_ATmega128__
    IRQ_TIMER1_COMPC,
    IRQ_TIMER3_CAP,
    IRQ_TIMER3_COMPA,
    IRQ_TIMER3_COMPB,
    IRQ_TIMER3_COMPC,
    IRQ_TIMER3_OVF,
    IRQ_UART1_RX,
    IRQ_UART1_UDRE,
    IRQ_UART1_TX,
    IRQ_I2C,
    IRQ_SPM_RDY,
#endif
    IRQ_MAX
};

/*
 * Registered interrupt handler information structure.
 */
typedef struct {
    u_long ir_count;
    void *ir_arg;
    void (*ir_handler)(void *);
} IRQ_HANDLER;

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
#ifdef __AVR_ATmega128__
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

/*@}*/

__BEGIN_DECLS

extern int NutRegisterInterrupt(int irq, void (*handler)(void *), void *arg);

extern void CallHandler(IRQ_HANDLER *irh);

extern int NutRegisterIrqHandler(IRQ_HANDLER *irh, void (*handler)(void *), void *arg);

__END_DECLS

#endif
