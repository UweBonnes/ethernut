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
 * Revision 1.9  2005/04/05 17:52:40  haraldkipp
 * Much better implementation of GBA interrupt registration.
 *
 * Revision 1.8  2005/02/10 07:06:51  hwmaier
 * Changes to incorporate support for AT90CAN128 CPU
 *
 * Revision 1.7  2004/08/05 12:13:56  freckle
 * Added unix emulation hook in NutThreadYield to safely process
 * NutPostEventAsync calls occuring in non Nut/OS threads.
 * Rewrote the unix read function again using the new unix NutThreadYield hook
 * to call the NutPostEventAsync function safely (fast & correct).
 * _write(nf, 0, 0) aka fflush is ignored on unix emulation.
 *
 * Revision 1.6  2004/04/07 12:13:57  haraldkipp
 * Matthias Ringwald's *nix emulation added
 *
 * Revision 1.5  2004/03/18 15:48:13  haraldkipp
 * ICCAVR failed to compile
 *
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

#include <cfg/arch.h>
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
    void (*ir_handler) (void *);
} IRQ_HANDLER;

#if defined(__AVR__)
#include <dev/irqreg_avr.h>
#elif defined(__arm__)
#include <dev/irqreg_arm.h>
#elif defined(__H8300H__) || defined(__H8300S__)
#include <dev/irqreg_h8.h>
#elif defined(__m68k__)
#include <dev/irqreg_m68k.h>
#else
#warning "MCU not defined"
#endif

/*@}*/

__BEGIN_DECLS
/* NutRegisterInterrupt is obsolete, use NutRegisterIrqHandler instead */
//extern int NutRegisterInterrupt(int irq, void (*handler)(void *), void *arg) __attribute__ ((obsolete)) ;
extern void CallHandler(IRQ_HANDLER * irh);

#if defined (__linux__) || defined (__APPLE__)

#include <signal.h>
extern u_char irq_processed;
extern pthread_mutex_t irq_mutex;
extern pthread_cond_t irq_cv;
extern sigset_t irq_signal;

extern void NutIRQTrigger(u_char);
extern int  NutRegisterIrqHandler(u_char irq_nr, void (*handler) (void *), void *arg);
extern void NutUnixIrqEventPostAsync(u_char irq_nr, HANDLE * queue );

#else

extern int NutRegisterIrqHandler(IRQ_HANDLER * irh, void (*handler) (void *), void *arg);

#endif

__END_DECLS
#endif
