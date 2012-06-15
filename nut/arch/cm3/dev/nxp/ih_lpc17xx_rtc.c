/*
 * Copyright (C) 2012 by Rob van Lieshout (info@pragmalab.nl)
 *
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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
 * \verbatim
 * $Id: $
 * \endverbatim
 */

#include <cfg/arch.h>
#include <arch/cm3.h>
#include <dev/irqreg.h>
#include <sys/device.h>
#include <arch/cm3/cortex_interrupt.h>

#ifndef NUT_IRQPRI_RTC
#define NUT_IRQPRI_RTC  4
#endif


static int RtcIrqCtl(int cmd, void *param);

/*!
 * \brief IRQ Handler for RTC.
 */
IRQ_HANDLER sig_RTC = {
#ifdef NUT_PERFMON
    0,                  /* Interrupt counter, ir_count. */
#endif
    0,                  /* Passed argument, ir_arg. Normally the NUTDEV structure, but an RTC doesn not have one*/
    NULL,               /* Handler subroutine, ir_handler. */
    RtcIrqCtl           /* Interrupt control, ir_ctl. */
};

/*!
 * \brief RTC interrupt entry.
 */
void RtcIrqEntry(void *arg)
{
#ifdef NUT_PERFMON
    sig_RTC.ir_count++;
#endif
    if (sig_RTC.ir_handler) {
        (sig_RTC.ir_handler) (sig_RTC.ir_arg);
    }
}

/*!
 * \brief RTC interrupt control.
 *
 * \param cmd   Control command.
 *              - NUT_IRQCTL_INIT Initialize and disable interrupt.
 *              - NUT_IRQCTL_STATUS Query interrupt status.
 *              - NUT_IRQCTL_ENABLE Enable interrupt.
 *              - NUT_IRQCTL_DISABLE Disable interrupt.
 *              - NUT_IRQCTL_GETMODE Query interrupt mode.
 *              - NUT_IRQCTL_SETMODE Set interrupt mode (NUT_IRQMODE_LEVEL or NUT_IRQMODE_EDGE).
 *              - NUT_IRQCTL_GETPRIO Query interrupt priority.
 *              - NUT_IRQCTL_SETPRIO Set interrupt priority.
 *              - NUT_IRQCTL_GETCOUNT Query and clear interrupt counter.
 * \param param Pointer to optional parameter.
 *
 * \return 0 on success, -1 otherwise.
 */
static int RtcIrqCtl(int cmd, void *param)
{
    int rc = 0;
    uint32_t *ival = (uint32_t *)param;
    int enabled = IntIsEnabled(RTC_IRQn);

    /* Disable interrupt. */
    if (enabled) {
        IntDisable(RTC_IRQn);
    }

    switch(cmd) {
    case NUT_IRQCTL_INIT:
        /* Set the vector. */
        IntRegister(RTC_IRQn, RtcIrqEntry);
        /* Initialize with defined priority. */
        IntPrioritySet(RTC_IRQn, NUT_IRQPRI_RTC);
        /* Clear interrupt */
        NVIC_ClearPendingIRQ(RTC_IRQn);
        break;
    case NUT_IRQCTL_STATUS:
        if (enabled) {
            *ival |= 1;
        }
        else {
            *ival &= ~1;
        }
        break;
    case NUT_IRQCTL_ENABLE:
        enabled = 1;
        break;
    case NUT_IRQCTL_DISABLE:
        enabled = 0;
        break;
    case NUT_IRQCTL_GETMODE:
        *ival = NUT_IRQMODE_EDGE;
        break;
    case NUT_IRQCTL_SETMODE:
        rc = -1;
        break;
    case NUT_IRQCTL_GETPRIO:
        *ival = IntPriorityGet(RTC_IRQn);
        break;
    case NUT_IRQCTL_SETPRIO:
        IntPrioritySet(RTC_IRQn, *ival);
        break;
#ifdef NUT_PERFMON
    case NUT_IRQCTL_GETCOUNT:
        *ival = (uint32_t)sig_RTC.ir_count;
        sig_RTC.ir_count = 0;
        break;
#endif
    default:
        rc = -1;
        break;
    }

    /* Enable interrupt. */
    if (enabled) {
        IntEnable(RTC_IRQn);
    }
    return rc;
}