/*
 * Copyright (C) 2006 by egnite Software GmbH. All rights reserved.
 * Copyright (C) 2011 by Ulrich Prinz (uprinz2@googlemail.com)
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
 * Revision 1.0  2011/12/09 17:30:10  ulrichprinz
 * Initial version.
 *
 */

#ifdef STM32_SPI3

#include <cfg/arch.h>
#include <arch/cm3.h>
#include <dev/irqreg.h>
#include <sys/device.h>
#include <arch/cm3/cortex_interrupt.h>

#ifndef NUT_IRQPRI_SPI3
#define NUT_IRQPRI_SPI3  4
#endif

static int SerialPeripheral3IrqCtl(int cmd, void *param);

IRQ_HANDLER sig_SPI3 = {
#ifdef NUT_PERFMON
    0,                          /* Interrupt counter, ir_count. */
#endif
    NULL,                       /* Passed argument, ir_arg. */
    NULL,                       /* Handler subroutine, ir_handler. */
    SerialPeripheral3IrqCtl     /* Interrupt control, ir_ctl. */
};

/*!
 * \brief Serial peripheral interface 1 interrupt entry.
 */
static void SerialPeripheral3IrqEntry(void *arg) __attribute__ ((naked));
void SerialPeripheral3IrqEntry(void *arg)
{
#ifdef NUT_PERFMON
    sig_SPI3.ir_count++;
#endif
    if (sig_SPI3.ir_handler) {
        (sig_SPI3.ir_handler) (sig_SPI3.ir_arg);
    }
}

/*!
 * \brief Serial peripheral interface 1 interrupt control.
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
static int SerialPeripheral3IrqCtl(int cmd, void *param)
{
    int rc = 0;
    unsigned int *ival = (unsigned int *) param;
    int enabled = IntIsEnabled(SPI3_IRQn);

    /* Disable interrupt. */
    if (enabled) {
        IntDisable(SPI3_IRQn);
    }

    switch (cmd) {
    case NUT_IRQCTL_INIT:
        /* Set the vector. */
        IntRegister(SPI3_IRQn, SerialPeripheral3IrqEntry);
        /* Initialize with defined priority. */
        IntPrioritySet(SPI3_IRQn, NUT_IRQPRI_SPI3);
        /* Clear interrupt */
        NVIC_ClearPendingIRQ(SPI3_IRQn);
        break;
    case NUT_IRQCTL_STATUS:
        if (enabled) {
            *ival |= 1;
        } else {
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
        *ival = IntPriorityGet(SPI3_IRQn);
        break;
    case NUT_IRQCTL_SETPRIO:
        IntPrioritySet(SPI3_IRQn,*ival);
        break;
#ifdef NUT_PERFMON
    case NUT_IRQCTL_GETCOUNT:
        *ival = (unsigned int) sig_SPI3.ir_count;
        sig_SPI3.ir_count = 0;
        break;
#endif
    default:
        rc = -1;
        break;
    }

    /* Enable interrupt. */
    if (enabled) {
        IntEnable(SPI3_IRQn);
    }
    return rc;
}

#endif /* STM32_SPI3 */