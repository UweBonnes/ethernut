/*
 * Copyright (C) 2005 by egnite Software GmbH. All rights reserved.
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
 * $Log: ih_irq7.c,v $
 *
 */

#include <arch/avr32.h>
#include <dev/irqreg.h>
#include <dev/gpio.h>

#include <arch/avr32/gpio.h>
#include <arch/avr32/ihndlr.h>

#include <avr32/io.h>

#ifndef NUT_IRQPRI_IRQ7
#define NUT_IRQPRI_IRQ7  AVR32_INTC_INT3
#endif

static int Interrupt7Ctl(int cmd, void *param);

IRQ_HANDLER sig_INTERRUPT7 = {
#ifdef NUT_PERFMON
    0,                  /* Interrupt counter, ir_count. */
#endif
    NULL,               /* Passed argument, ir_arg. */
    NULL,               /* Handler subroutine, ir_handler. */
    Interrupt7Ctl       /* Interrupt control, ir_ctl. */
};

/*!
 * \brief External interrupt 7 entry.
 */
static SIGNAL(Interrupt7Entry)
{
    IRQ_ENTRY();
#ifdef NUT_PERFMON
    sig_INTERRUPT7.ir_count++;
#endif
    if (sig_INTERRUPT7.ir_handler) {
        (sig_INTERRUPT7.ir_handler) (sig_INTERRUPT7.ir_arg);
    }
	/* Clear interrupt */
	AVR32_EIC.icr = AVR32_EIC_ICR_INT7_MASK;
	AVR32_EIC.isr;

	IRQ_EXIT();
}

/*!
 * \brief External interrupt 7 control.
 *
 * \param cmd   Control command.
 *              - NUT_IRQCTL_INIT Initialize and disable interrupt.
 *              - NUT_IRQCTL_STATUS Query interrupt status.
 *              - NUT_IRQCTL_ENABLE Enable interrupt.
 *              - NUT_IRQCTL_DISABLE Disable interrupt.
 *              - NUT_IRQCTL_GETPRIO Query interrupt priority.
 *              - NUT_IRQCTL_GETCOUNT Query and clear interrupt counter.
 * \param param Pointer to optional parameter.
 *
 * \return 0 on success, -1 otherwise.
 */
static int Interrupt7Ctl(int cmd, void *param)
{
    int rc = 0;
    unsigned int *ival = (unsigned int *)param;
    int_fast8_t enabled = AVR32_EIC.imr & AVR32_EIC_IMR_INT7_MASK;

    /* Disable interrupt. */
    if (enabled) {
		AVR32_EIC.idr = AVR32_EIC_IDR_INT7_MASK;
		AVR32_EIC.imr;
    }

    switch(cmd) {
    case NUT_IRQCTL_INIT:
		/* Setup Peripheral mux for interrupt line */
		GpioPinConfigSet( AVR32_GPIO_BANK(AVR32_EIC_EXTINT_7_PIN), AVR32_GPIO_PIN(AVR32_EIC_EXTINT_7_PIN), AVR32_GPIO_FUNCTION(AVR32_EIC_EXTINT_7_FUNCTION) );
		/* Set the vector. */
		register_interrupt(Interrupt7Entry, AVR32_EIC_IRQ_7, NUT_IRQPRI_IRQ7);
        /* Initialize to edge triggered with defined priority. */
		AVR32_EIC.mode &= ~AVR32_EIC_MODE_INT7_MASK;
        /* Clear interrupt */
        AVR32_EIC.icr = AVR32_EIC_ICR_INT7_MASK;
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
        {
            if (AVR32_EIC.mode & AVR32_EIC_MODE_INT7_MASK) {
				if ( AVR32_EIC.level & AVR32_EIC_LEVEL_INT7_MASK)
					*ival = NUT_IRQMODE_HIGHLEVEL;
				else
					*ival = NUT_IRQMODE_LOWLEVEL;
			} else {
				if (AVR32_EIC.edge & AVR32_EIC_EDGE_INT7_MASK)
					*ival = NUT_IRQMODE_RISINGEDGE;
				else
					*ival = NUT_IRQMODE_FALLINGEDGE;
            }
        }
        break;
    case NUT_IRQCTL_SETMODE:
        if (*ival == NUT_IRQMODE_LOWLEVEL) {
            AVR32_EIC.mode |= AVR32_EIC_MODE_INT7_MASK;
			AVR32_EIC.level &= ~AVR32_EIC_LEVEL_INT7_MASK;
        } else if (*ival == NUT_IRQMODE_HIGHLEVEL) {
			AVR32_EIC.mode |= AVR32_EIC_MODE_INT7_MASK;
			AVR32_EIC.level |= ~AVR32_EIC_LEVEL_INT7_MASK;
        } else if (*ival == NUT_IRQMODE_FALLINGEDGE) {
			AVR32_EIC.mode &= ~AVR32_EIC_MODE_INT7_MASK;
			AVR32_EIC.edge &= ~AVR32_EIC_EDGE_INT7_MASK;
        } else  if (*ival == NUT_IRQMODE_RISINGEDGE) {
			AVR32_EIC.mode &= ~AVR32_EIC_MODE_INT7_MASK;
			AVR32_EIC.edge |= ~AVR32_EIC_EDGE_INT7_MASK;
        } else  {
            rc = -1;
        }
        break;
    case NUT_IRQCTL_GETPRIO:
	    *ival = NUT_IRQPRI_IRQ7;
        break;
#ifdef NUT_PERFMON
    case NUT_IRQCTL_GETCOUNT:
        *ival = (unsigned int)sig_INTERRUPT7.ir_count;
        sig_INTERRUPT7.ir_count = 0;
        break;
#endif
    default:
        rc = -1;
        break;
    }

    /* Enable interrupt. */
    if (enabled) {
		AVR32_EIC.ier = AVR32_EIC_IER_INT7_MASK;
		AVR32_EIC.imr;
		AVR32_EIC.en |= AVR32_EIC_EN_INT7_MASK;
    }
    return rc;
}
