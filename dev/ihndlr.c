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
 *
 */

/*
 * $Log$
 * Revision 1.3  2005/08/02 17:46:47  haraldkipp
 * Major API documentation update.
 *
 * Revision 1.2  2005/07/26 16:30:58  haraldkipp
 * Copyright added.
 * Temporarily exclude NutRegisterIrqHandler() from Linux builds.
 *
 */

#include <sys/atom.h>
#include <dev/irqreg.h>

/*!
 * \addtogroup xgInterrupt
 */
/*@{*/

/*!
 * \brief Call a registered interrupt handler.
 */
void CallHandler(IRQ_HANDLER * irh)
{
    irh->ir_count++;
    if (irh->ir_handler)
        (irh->ir_handler) (irh->ir_arg);
}

/*!
 * \brief Register an interrupt handler.
 *
 * This function is typically called by device drivers, but
 * applications may also implement their local interrupt
 * handlers.
 *
 * \param irq     Interrupt number to be associated with this handler.
 * \param handler This routine will be called by Nut/OS, when the
 *                specified interrupt occurs.
 * \param arg     Argument to be passed to the interrupt handler.
 *
 * \return 0 on success, -1 otherwise.
 */
#if !defined (__linux__) && !defined(__APPLE__) && !defined(__CYGWIN__)
int NutRegisterIrqHandler(IRQ_HANDLER * irq, void (*handler) (void *),
                          void *arg)
{
    NutEnterCritical();

    irq->ir_arg = arg;
    irq->ir_handler = handler;

    NutExitCritical();

    return 0;
}
#endif

/*@}*/
