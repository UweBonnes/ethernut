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
 * $Log: ostimer_at91.c,v $
 * Revision 1.0  2008/03/26 19:34:10 psilva
 * Firts revision to rum on Stellaris LM3S6965
 *
 */

#include <cfg/os.h>
#include <cfg/clock.h>
#include <arch/arm.h>
#include <dev/irqreg.h>
#include <sys/timer.h>
#include <arch/arm/cortex_sysctl.h>
#include <arch/arm/cortex_systick.h>


#ifndef NUT_CPU_FREQ
#define NUT_CPU_FREQ    48000000UL
#endif //!NUT_CPU_FREQ

/*!
 * \addtogroup xgNutArchArmOsTimerAt91
 */
/*@{*/

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
 * \bug This function is a miserable hack.
 *
 * \param ms Delay time in milliseconds, maximum is 255.
 */
/*
void NutDelay(u_char ms)
{
    int i;

    while (ms--) {
        for (i = 14600; i--; ) {
            _NOP();
        }
    }
}
*/
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

    // Set up and enable the SysTick timer.  It will be used as a reference
    // for delay loops.  The SysTick timer period will be set up for one
    // second.
    SysTickPeriodSet(NutGetCpuClock()/NUT_TICK_FREQ);//48000
    SysTickEnable();

    SysTickIntRegister(handler);
}

/*!
 * \brief Determine the PLL output clock frequency.
 *
 * \param plla Specifies the PLL, 0 for default, 1 for alternate.
 *
 * \return Frequency of the selected PLL in Hertz.
 */
 /*
static u_int CortexGetPllClock(int plla)
{
    u_int rc;

    return rc;
}
*/
/*!
 * \brief Determine the processor clock frequency.
 *
 * \return CPU clock frequency in Hertz.
 */
static u_long CortexGetProcessorClock(void)
{
    u_int rc = SysCtlClockGet();
    return rc;
}

/*!
 * \brief Determine the master clock frequency.
 *
 * \return Master clock frequency in Hertz.
 */
u_long CortexGetMasterClock(void)
{
    u_long rc = CortexGetProcessorClock();

    return rc;
}
/*!
 * \brief Return the CPU clock in Hertz. Or peripheral clock in hertx
 * On cortex this the same.
 *
 * \return CPU clock frequency in Hertz.
 */
uint32_t NutArchClockGet(int idx)
{
    uint32_t clock;
    if ( idx == NUT_HWCLK_CPU )
        return SysCtlClockGet();
    else if ( idx == NUT_HWCLK_PERIPHERAL )
        return SysCtlClockGet();
    else if ( idx == NUT_HWCLK_PCLK1 ){//clock frequency for PCLK1
        clock=SysCtlClockGet(); //Get sysclock
        switch(RCC->CFGR & ((1<<8)|(1<<9)|(1<<10))){ //Check divider
            case RCC_HCLK_Div1:
                break;
            case RCC_HCLK_Div2:
                clock>>=1;
                break;
        };
        return clock;
    }
    else if ( idx == NUT_HWCLK_PCLK2 ){ //clock frequency for PCLK2
        clock=SysCtlClockGet(); //Get sysclock
        switch(RCC->CFGR & ((1<<11)|(1<<12)|(1<<13))){ //Check divider
            case (RCC_HCLK_Div1<<3):
                break;
            case (RCC_HCLK_Div2<<3):
                clock>>=1;
                break;
        };
        return clock;
    }
    return 0;
}

/*!
 * \brief Return the CPU clock in Hertz.
 *
 * On several AT91 CPUs the processor clock may differ from the clock
 * driving the peripherals. In this case At91GetMasterClock() will
 * provide the correct master clock.
 *
 * \return CPU clock frequency in Hertz.
 */

/*u_long NutGetCpuClock(void)
{
#if defined(NUT_CPU_FREQ)
    return NUT_CPU_FREQ;
#else
    return SysCtlClockGet();
#endif
}*/

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
    return (ms * NutGetTickClock()) / 1000;
}

/*@}*/

