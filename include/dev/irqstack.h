#ifndef _DEV_IRQSTACK_H_
#define _DEV_IRQSTACK_H_

/*
 * Copyright (C) 2001-2004 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.2  2004/02/03 11:28:41  drsung
 * Modified to support different target platforms.
 *
 * Revision 1.1  2004/01/30 17:00:46  drsung
 * Separate interrupt stack for avr-gcc only added.
 *
 */

#include <sys/types.h>
#include <stdio.h>

#if defined(__GNUC__) && (defined(__AVR_ATmega128__) || defined(__AVR_ATmega103__))

#define IRQSTACK_SIZE 256

extern u_char _irq_stack[];
extern u_char _irq_SPL;
extern u_char _irq_SPH;

#define _SwitchToIrqStack	\
    asm volatile ("push r24" "\n\t"			\
                  "in r24,__SREG__" "\n\t"		\
                  "push r24" "\n\t"			\
                  "in r24, __SP_L__" "\n\t"		\
                  "sts _irq_SPL, r24" "\n\t"		\
                  "in r24, __SP_H__" "\n\t"		\
                  "sts _irq_SPH, r24" "\n\t");		\
    asm volatile ("ldi r24, lo8(%0)" "\n\t"		\
                  "out __SP_L__, r24" "\n\t"		\
                  "ldi r24, hi8(%0)" "\n\t"		\
                  "out __SP_H__, r24" "\n\t"::		\
                  "i" (_irq_stack+IRQSTACK_SIZE-1)  );

#define _SwitchToAppStack	\
    asm volatile ("lds r24, _irq_SPL" "\n\t"		\
                  "out __SP_L__, r24" "\n\t"		\
                  "lds r24, _irq_SPH" "\n\t"		\
                  "out __SP_H__, r24" "\n\t");		\
    asm volatile ("pop r24" "\n\t"			\
                  "out __SREG__, r24" "\n\t"		\
                  "pop r24" "\n\t");

#define _SaveRegs	\
    asm volatile ("push __tmp_reg__" "\n\t"	\
                  "push __zero_reg__" "\n\t"	\
                  "clr __zero_reg__" "\n\t"	\
                  "push r18" "\n\t"		\
                  "push r19" "\n\t"		\
                  "push r20" "\n\t"		\
                  "push r21" "\n\t"		\
                  "push r22" "\n\t"		\
                  "push r23" "\n\t"		\
                  "push r24" "\n\t"		\
                  "push r25" "\n\t"		\
                  "push r26" "\n\t"		\
                  "push r27" "\n\t"		\
                  "push r30" "\n\t"		\
                  "push r31" "\n\t");


#define _RestoreRegs	\
    asm volatile ("pop r31" "\n\t"		\
                  "pop r30" "\n\t"		\
                  "pop r27" "\n\t"		\
                  "pop r26" "\n\t"		\
                  "pop r25" "\n\t"		\
                  "pop r24" "\n\t"		\
                  "pop r23" "\n\t"		\
                  "pop r22" "\n\t"		\
                  "pop r21" "\n\t"		\
                  "pop r20" "\n\t"		\
                  "pop r19" "\n\t"		\
                  "pop r18" "\n\t"		\
                  "pop __zero_reg__" "\n\t"	\
                  "pop __tmp_reg__" "\n\t");

#define NUTSIGNAL(signame,handler)		\
void signame (void) __attribute__ ((naked));	\
void signame (void)	\
{				\
   _SwitchToIrqStack		\
   _SaveRegs			\
   CallHandler (&handler);	\
   _RestoreRegs			\
   _SwitchToAppStack		\
   asm ("reti");		\
}

#else                           /* #if defined(__GNUC__) && (defined(__AVR_ATmega128__) || defined(__AVR_ATmega103__)) */


#define NUTSIGNAL(signame,handler)	\
SIGNAL(signame)		\
{ CallHandler (&handler);  }

#endif                          /* #if defined(__GNUC__) && (defined(__AVR_ATmega128__) || defined(__AVR_ATmega103__)) */

#endif
