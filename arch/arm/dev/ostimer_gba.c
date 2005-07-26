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
 * Revision 1.1  2005/07/26 18:02:26  haraldkipp
 * Moved from dev.
 *
 * Revision 1.2  2005/07/20 09:17:26  haraldkipp
 * Default NUT_CPU_FREQ and NUT_TICK_FREQ added.
 * NutTimerIntr() removed, because we can use the hardware independent code.
 *
 * Revision 1.1  2005/05/27 17:16:40  drsung
 * Moved the file.
 *
 * Revision 1.5  2005/04/05 17:50:46  haraldkipp
 * Use register names in gba.h.
 *
 * Revision 1.4  2004/11/08 19:16:37  haraldkipp
 * Hacked in Gameboy timer support
 *
 * Revision 1.3  2004/10/03 18:42:21  haraldkipp
 * No GBA support yet, but let the compiler run through
 *
 * Revision 1.2  2004/09/08 10:19:39  haraldkipp
 * Running on AT91 and S3C, thanks to James Tyou
 *
 */

#include <cfg/arch.h>
#include <arch/gba.h>
#include <dev/irqreg.h>

#ifndef NUT_CPU_FREQ
#define NUT_CPU_FREQ    1000000UL
#endif

#ifndef NUT_TICK_FREQ
#define NUT_TICK_FREQ   1000UL
#endif

static void (*os_handler) (void *);

/*!
 * \brief Loop for a specified number of milliseconds.
 *
 * This call will not release the CPU and will
 * not switch to another thread. However, because
 * of absent thread switching, this delay time is
 * very exact.
 *
 * Use NutSleep() to avoid blocking the CPU, if no
 * exact timing is needed.
 *
 * \param ms Delay time in milliseconds, maximum is 255.
 */
void NutDelay(u_char ms)
{
    u_short delay_cnt = 2400;   //*KU* for 14.745600 MHz Clock
    u_short delay_cnt_buffer;

    while (ms--) {
        delay_cnt_buffer = delay_cnt;
        while (delay_cnt_buffer--);
    }
}


/*!
 * \brief Timer 0 interrupt entry.
 */
void Timer3Entry(void *arg)
{
    outw(REG_IF, INT_TMR3);
    os_handler(0);
}

/*!
 * \brief Initialize system timer.
 *
 * This function is automatically called by Nut/OS
 * during system initialization.
 *
 * Nut/OS uses on-chip timer 0 for its timer services.
 * Applications should not modify any registers of this
 * timer, but make use of the Nut/OS timer API. Timer 1
 * and timer 2 are available to applications.
 *
 * \todo Hardware stuff to be put in nutlibdev.
 */
void NutRegisterTimer(void (*handler) (void *))
{
    os_handler = handler;


    /* Disable master interrupt. */
    outw(REG_IME, 0);

    /* Set global interrupt vector. */
    NutRegisterIrqHandler(&sig_TMR3, Timer3Entry, 0);

    /* Enable timer and timer interrupts. */
    outdw(REG_TMR3CNT, TMR_IRQ_ENA | TMR_ENA | 48756);

    /* Enable timer 3 interrupts. */
    outw(REG_IE, inw(REG_IE) | INT_TMR3);

    /* Enable master interrupt. */
    outw(REG_IME, 1);
}

/*!
 * \brief Return the CPU clock in Hertz.
 *
 * \return CPU clock frequency in Hertz.
 */
u_long NutGetCpuClock(void)
{
    return NUT_CPU_FREQ;
}

/*!
 * \brief Return the number of system ticks per second.
 *
 * \return System tick frequency in Hertz.
 */
u_long NutGetTickClock(void)
{
    return NUT_TICK_FREQ;
}

/*!
 * \brief Calculate system ticks for a given number of milliseconds.
 */
u_long NutTimerMillisToTicks(u_long ms)
{
    return ms * 1000L / NutGetTickClock();
}


