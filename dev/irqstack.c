/*
 * Copyright (C) 2004 by egnite Software GmbH. All rights reserved.
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
 *
 */

/*
 * $Log$
 * Revision 1.7  2004/09/22 08:15:57  haraldkipp
 * Speparate IRQ stack configurable
 *
 * Revision 1.6  2004/04/25 17:05:56  drsung
 * Separate IRQ stack now compatible with nested interrupts.
 *
 * Revision 1.5  2004/02/03 11:28:40  drsung
 * Modified to support different target platforms.
 *
 * Revision 1.4  2004/02/02 15:35:31  drsung
 * Bloody 'no new line at end of file' error fixed... Grrrr..
 *
 * Revision 1.3  2004/02/01 10:47:51  drsung
 * #ifdef .. #endif inserted, to make iccavr compile.
 *
 * Revision 1.2  2004/01/31 13:59:14  drsung
 * Stupid attributes removed.
 *
 * Revision 1.1  2004/01/30 17:02:20  drsung
 * Separate interrupt stack for avr-gcc only added.
 *
 */

#include <dev/irqstack.h>

#ifdef IRQSTACK_SIZE

u_char _irq_stack[IRQSTACK_SIZE]; //__attribute__((section(".data.irqstack")));
u_char _irq_SPL; 	//__attribute__((section(".data.irqstack")));
u_char _irq_SPH; 	//__attribute__((section(".data.irqstack")));
u_char _irq_nesting;	// __attribute__((section(".data.irqstack"))) = 0;

#endif /* #ifdef IRQSTACK_SIZE */
