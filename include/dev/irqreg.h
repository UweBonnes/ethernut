#ifndef _DEV_IRQREG_H_
#define _DEV_IRQREG_H_

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
 * Revision 1.4  2004/03/16 16:48:28  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.3  2004/01/30 17:00:46  drsung
 * Separate interrupt stack for avr-gcc only added.
 *
 * Revision 1.2  2003/11/23 16:42:13  drsung
 * NutRegisterInterrupt removed, because it's incompatible to the new interrupt handling
 *
 * Revision 1.1.1.1  2003/05/09 14:41:06  haraldkipp
 * Initial using 3.2.1
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
#include <dev/irqstack.h>

/*!
 * \file dev/irqreg.h
 * \brief Interrupt management definitions.
 */

/*!
 * \addtogroup xgIrqReg
 */
/*@{*/

/*
 * Registered interrupt handler information structure.
 */
typedef struct {
    u_long ir_count;
    void *ir_arg;
    void (*ir_handler)(void *);
} IRQ_HANDLER;

#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega103__)
#include "irqreg_avr.h"
#elif defined(__arm__)
#include "irqreg_arm.h"
#elif defined(__H8300H__) || defined(__H8300S__)
#include "irqreg_h8.h"
#elif defined(__m68k__)
#include "irqreg_m68k.h"
#endif

/*@}*/

__BEGIN_DECLS

/* NutRegisterInterrupt is obsolete, use NutRegisterIrqHandler instead */
//extern int NutRegisterInterrupt(int irq, void (*handler)(void *), void *arg) __attribute__ ((obsolete)) ;
	
extern void CallHandler(IRQ_HANDLER *irh);

extern int NutRegisterIrqHandler(IRQ_HANDLER *irh, void (*handler)(void *), void *arg);

__END_DECLS

#endif
