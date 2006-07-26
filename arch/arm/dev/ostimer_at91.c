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
 * Revision 1.8  2006/07/26 11:17:16  haraldkipp
 * Defining AT91_PLL_MAINCK will automatically determine SAM7X clock by
 * reading PLL settings.
 *
 * Revision 1.7  2006/07/05 07:59:41  haraldkipp
 * Daidai's support for AT91SAM7X added.
 *
 * Revision 1.6  2006/06/28 17:10:35  haraldkipp
 * Include more general header file for ARM.
 *
 * Revision 1.5  2006/03/02 19:53:01  haraldkipp
 * Bugfix. The system timer configuration was based on a fixed MCU clock
 * of 66.6 MHz. Now it uses the actual frequency.
 *
 * Revision 1.4  2006/01/05 16:46:25  haraldkipp
 * Added support for CY22393 programmable clock chip.
 *
 * Revision 1.3  2005/10/24 08:34:13  haraldkipp
 * Moved AT91 family specific header files to sbudir arm.
 * Use new IRQ API.
 *
 * Revision 1.2  2005/08/02 17:46:45  haraldkipp
 * Major API documentation update.
 *
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

#include <cfg/os.h>
#include <cfg/clock.h>
#include <arch/arm.h>
#include <dev/irqreg.h>
#include <sys/timer.h>

#ifndef NUT_CPU_FREQ
#ifdef NUT_PLL_CPUCLK
#include <dev/cy2239x.h>
#elif !defined(AT91_PLL_MAINCK)
#define NUT_CPU_FREQ    73728000UL
#endif /* !AT91_PLL_MAINCK */
#endif /* !NUT_CPU_FREQ */


/*!
 * \addtogroup xgNutArchArmOsTimerAt91
 */
/*@{*/

#define NutEnableTimerIrq()     NutEnterCritical()
#define NutDisableTimerIrq()    NutExitCritical()

#ifndef NUT_TICK_FREQ
#define NUT_TICK_FREQ   1000UL
#endif

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
    int i;

    while (ms--) {
        for (i = 3200; i--; ) {
            _NOP();
        }
    }
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
    int dummy;

#ifdef MCU_AT91SAM7X256
    /* Enable TC0 clock. */
    outr(PMC_PCER, _BV(TC0_ID));
#endif

    /* Disable the Clock Counter */
    outr(TC0_CCR, TC_CLKDIS);
    /* Disable all interrupts */
    outr(TC0_IDR, 0xFFFFFFFF);
    /* Clear the status register. */
    dummy = inr(TC0_SR);
    /* Select divider and compare trigger */
    outr(TC0_CMR, TC_CLKS_MCK32 | TC_CPCTRG);
    /* Enable the Clock counter */
    outr(TC0_CCR, TC_CLKEN);
    /* Validate the RC compare interrupt */
    outr(TC0_IER, TC_CPCS);

    /* Register timer interrupt handler. */
    NutRegisterIrqHandler(&sig_TC0, handler, 0);
    /* Set to lowest priority. */
    NutIrqSetPriority(&sig_TC0, 0);

    /* Enable timer 0 interrupts */
    NutIrqEnable(&sig_TC0);
    //outr(AIC_IECR, _BV(TC0_ID));

    /* Set compare value for 1 ms. */
    outr(TC0_RC, NutGetCpuClock() / (32 * NUT_TICK_FREQ));

    /* Software trigger starts the clock. */
    outr(TC0_CCR, TC_SWTRG);
}

#if defined(AT91_PLL_MAINCK)
#if !defined(AT91_SLOW_CLOCK)
/* This is just a guess and my be completely wrong. */
#define AT91_SLOW_CLOCK 32000
#endif

static u_int At91GetPllClock(void)
{
    u_int rc = AT91_PLL_MAINCK;
    u_int pllr = inr(CKGR_PLLR);
    u_int divider = (pllr & CKGR_DIV) >> CKGR_DIV_LSB;

    if (divider) {
        rc *= ((pllr & CKGR_MUL) >> CKGR_MUL_LSB) + 1;
        rc /= divider;
    }
    return rc;
}

static u_long At91GetMasterClock(void)
{
    u_int rc = 0;
    u_int mckr = inr(PMC_MCKR);

    /* Determine the clock source. */
    switch((mckr & PMC_CSS) >> PMC_CSS_LSB) {
    case 0:
        /* Slow clock selected. */
        rc = AT91_SLOW_CLOCK;
        break;
    case 1:
        /* Main clock selected. */
        rc = AT91_PLL_MAINCK;
        break;
    case 3:
        /* PLL clock selected. */
        rc = At91GetPllClock();
        break;
    }

    /* Handle pre-scaling. */
    mckr &= PMC_PRES;
    mckr >>= PMC_PRES_LSB;
    if (mckr < 7) {
        rc /= _BV(mckr);
    }
    else {
        rc = 0;
    }
    return rc;
}
#endif

/*!
 * \brief Return the CPU clock in Hertz.
 *
 * \return CPU clock frequency in Hertz.
 */
u_long NutGetCpuClock(void)
{
#if defined(AT91_PLL_MAINCK)
    return At91GetMasterClock();
#elif defined(NUT_PLL_CPUCLK)
    return Cy2239xGetFreq(NUT_PLL_CPUCLK, 7);
#else
    return NUT_CPU_FREQ;
#endif
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

/*@}*/

