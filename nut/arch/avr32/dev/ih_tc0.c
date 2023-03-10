/*!
 * Copyright (C) 2001-2010 by egnite Software GmbH
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


/*
* $Log: ih_tc0.c,v $
*
*/

#include <arch/avr32.h>
#include <dev/irqreg.h>
#include <avr32/io.h>

#include <arch/avr32/ihndlr.h>

#ifndef NUT_IRQPRI_TC0
#define NUT_IRQPRI_TC0  0
#endif

#define CHANNEL 0

#ifndef NUT_IRQCHANNEL_TC0 
#define NUT_IRQCHANNEL_TC0 AVR32_TC0_IRQ0
#endif

// Some devices with only 1 TC like A0512 will define it as TC instead of TC0
#if !defined(AVR32_TC0) && defined( AVR32_TC )
# define AVR32_TC0 AVR32_TC
# define AVR32_TC0_IRQ0 AVR32_TC_IRQ0
#endif

static int TimerCounter0IrqCtl(int cmd, void *param);

IRQ_HANDLER sig_TC0 = {
#ifdef NUT_PERFMON
    0,                          /* Interrupt counter, ir_count. */
#endif
    NULL,                       /* Passed argument, ir_arg. */
    NULL,                       /* Handler subroutine, ir_handler. */
    TimerCounter0IrqCtl         /* Interrupt control, ir_ctl. */
};

/*!
 * \brief UART 0 interrupt entry.
 */
SIGNAL(TC0IrqEntry)
{
    IRQ_ENTRY();
#ifdef NUT_PERFMON
    sig_TC0.ir_count++;
#endif
    if (sig_TC0.ir_handler) {
        (sig_TC0.ir_handler) (sig_TC0.ir_arg);
    }
    IRQ_EXIT();
}

/*!
* \brief Timer/Counter 0 interrupt control.
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
static int TimerCounter0IrqCtl(int cmd, void *param)
{
    int rc = 0;
    unsigned int *ival = (unsigned int *) param;
	ureg_t imr = AVR32_TC0.channel[CHANNEL].imr;
	static ureg_t enabledIMR = 0;
	int_fast8_t enabled = imr;

	/* Disable interrupt. */
	if (enabled) {
		AVR32_TC0.channel[CHANNEL].idr = 0xFFFFFFFF;
		AVR32_TC0.channel[CHANNEL].sr;
		enabledIMR = imr;
	}

    switch (cmd) {
    case NUT_IRQCTL_INIT:
		register_interrupt(TC0IrqEntry, NUT_IRQCHANNEL_TC0, NUT_IRQPRI_TC0);

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
#ifdef NUT_PERFMON
    case NUT_IRQCTL_GETCOUNT:
        *ival = (unsigned int) sig_TC0.ir_count;
        sig_TC0.ir_count = 0;
        break;
#endif
    default:
        rc = -1;
        break;
    }

	/* Enable interrupt. */
	if (enabled) {
		AVR32_TC0.channel[CHANNEL].ier = enabledIMR;
		AVR32_TC0.channel[CHANNEL].sr;
	}
    return rc;
}
