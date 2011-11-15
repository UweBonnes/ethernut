/*
 * Copyright (C) 2001-2007 by egnite Software GmbH. All rights reserved.
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
 * $Id$
 */

#define NUT_DEPRECATED

#include <cfg/os.h>
#include <cfg/clock.h>
#include <arch/arm.h>
//#include <arch/cm3.h>
//#include <arch/cm3/nxp/ppb.h>
#include <dev/irqreg.h>
#include <sys/timer.h>


/*!
 * \addtogroup xgNutArchArmOsTimerAt91
 */
/*@{*/

#ifndef NUT_TICK_FREQ
#define NUT_TICK_FREQ   1000UL
#endif

static void (*systick_handler) (void *);
extern volatile uint32_t nut_ticks;
extern int dputchar(int c);

void __sys_tick(void)
{
    nut_ticks++;
#if 0
    if (systick_handler) {
        (systick_handler)(NULL);
    }
#endif
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
 */
void NutRegisterTimer(void (*handler) (void *))
{
    uint32_t t;

    systick_handler = handler;
    /* Set system tick priority. */
    t = inr(CM3_SCB_SHPR(CM3_SCB_SHPR_PRI_15_IDX));
    t &= ~(15 << CM3_SCB_SHPR_PRI_15_LSB);
    t |= 8 << CM3_SCB_SHPR_PRI_15_LSB;
    outr(CM3_SCB_SHPR(CM3_SCB_SHPR_PRI_15_IDX), t);
    /* Set reload value to get 1 ms ticks. */
    outr(CM3_SYSTICK_LOAD, NutArchClockGet(NUT_HWCLK_CPU) / 1000);
    /* Enable tick counter. */
    //outr(CM3_SYSTICK_CTRL, CM3_SYSTICK_CTRL_ENABLE | CM3_SYSTICK_CTRL_TICKINT | CM3_SYSTICK_CTRL_CLKSOURCE);
}

#define OSC_CLK 12000000
#define RTC_CLK 32000
#define IRC_OSC 4000000

/*!
 * \brief Return the CPU clock in Hertz.
 *
 * On several AT91 CPUs the processor clock may differ from the clock 
 * driving the peripherals. In this case At91GetMasterClock() will 
 * provide the correct master clock.
 *
 * \return CPU clock frequency in Hertz.
 */
uint32_t NutArchClockGet(int idx)
{
    uint32_t freq;
    uint32_t pll0 = inr(SC_PLL0STAT);

    /* Determine the system clock frequency. */
    switch (inr(SC_CLKSRCSEL) & SC_CLKSRC) {
    case SC_CLKSRC_MCLK:
        /* Main oscillator. */
        freq = OSC_CLK;
        break;
    case SC_CLKSRC_RTCCLK:
        /* RTC oscillator */
        freq = RTC_CLK;
        break;
    default:
        /* Internal RC oscillator. */
        freq = IRC_OSC;
        break;
    }

    /* Determine PLL clock. */
    if ((pll0 & (SC_PLLE_STAT | SC_PLLC_STAT)) == (SC_PLLE_STAT | SC_PLLC_STAT)) {
        /* PLL0 is enabled and connected. */
        freq *= 2;
        freq *= ((pll0 & SC_MSEL) >> SC_MSEL_LSB) + 1;
        freq /= ((pll0 & SC_NSEL) >> SC_NSEL_LSB) + 1;
    }

    /* Incorporate CPU clock divider. */
    freq /= ((inr(SC_CCLKCFG) & SC_CCLKSEL) >> SC_CCLKSEL_LSB) + 1;

    return freq;
}

/*!
 * \brief Return the number of system ticks per second.
 *
 * \return System tick frequency in Hertz.
 */
uint32_t NutGetTickClock(void)
{
    return NUT_TICK_FREQ;
}

/*!
 * \brief Calculate system ticks for a given number of milliseconds.
 */
uint32_t NutTimerMillisToTicks(uint32_t ms)
{
    return (ms * NutGetTickClock()) / 1000;
}

/*@}*/

