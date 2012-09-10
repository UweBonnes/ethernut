

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
#include <arch/cm3.h>
#include <dev/irqreg.h>
#include <sys/timer.h>
#if defined(MCU_STM32)
#include <arch/cm3/stm/stm32xxxx.h>
#include <arch/cm3/stm/system_stm32xxxx.h>
#include <arch/cm3/stm/stm32xxxx_rcc.h>
#include <arch/cm3/stm/stm32_clk.h>
#elif defined(MCU_LPC176x)
#include <arch/cm3/nxp/lpc176x.h>
#include <arch/cm3/nxp/lpc176x_clk.h>
#elif defined(MCU_LPC177x_8x)
#include <arch/cm3/nxp/lpc177x_8x.h>
#include <arch/cm3/nxp/lpc177x_8x_clk.h>
#else
#warning "Unknown CM3 family"
#endif

/*!
 * \addtogroup xgNutArchArmOsTimerCm3
 */
/*@{*/

#ifndef NUT_TICK_FREQ
#define NUT_TICK_FREQ   1000UL
#endif

/*!
 * \brief Loop for a specified number of microseconds.
 *
 * This call will release the CPU for delays longer than 2 ms
 * at the expense of waiting 1 ms longer in very rare cases
 *
 *
 * \param us Delay time in microseconds.
 */

extern uint32_t nut_ticks;
void NutArchMicroDelay(uint32_t us)
{
    uint32_t start_ticks;
    uint32_t end_ms, start_ms;
    int32_t end_ticks;

    SysTick->CTRL&= ~(SysTick_CTRL_ENABLE_Msk);
    start_ticks = SysTick->VAL;
    start_ms = nut_ticks;
    SysTick->CTRL |= (SysTick_CTRL_TICKINT_Msk);

    if (us > 2000)
    {
    int n = us/1000 -1;
    NutSleep(n);
    us -= n*1000;
    }
    end_ticks = start_ticks - (us%1000 * (SysTick->LOAD +1))/NUT_TICK_FREQ;
    end_ms = start_ms + us/1000;
    /* Wraparounf of Systick*/
    if (end_ticks <= 0)
    {
    end_ms++;
    end_ticks += SysTick->LOAD +1;
    }
    /* Wraparound of nut_ticks*/
    if( end_ms < start_ms)
    while (nut_ticks) {};
    for(;;)
    {
    /* Paranoid check for wraparound of nut_ticks at end of delay*/
        if ((end_ms == (uint32_t)-1) && (nut_ticks == 0))
            break;
    /* Check for wraparound of SysTickValue at end of delay*/
        if(nut_ticks > end_ms)
           break;
        if (nut_ticks < end_ms)
            continue;
        if (SysTick->VAL > end_ticks)
            continue;
        break;
    }
}

/*!
 * \brief Initialize system timer.
 *
 * This function is automatically called by Nut/OS
 * during system initialization.
 *
 * Nut/OS uses CortexM SysTick timer via CMSIS for its
 * timer services.
 * Applications should not modify any registers of this
 * timer, but make use of the Nut/OS timer API.
 */
void NutRegisterTimer(void (*handler)(void*))
{
    /* With CortexM SysTick timer is a core timer.
     * It is installed by providing a function void SysTick_Handler(void).
     * This function is called because it moves the interrupt
     * vectors from flash to ram.
     * Then it programs the timer and starts it.
     */
    Cortex_RegisterInt(SysTick_IRQn, handler);
    /* Program frequency and enable is done by CMSIS function */
    SysTick_Config(SysCtlClockGet()/NUT_TICK_FREQ);

//    SysTickCortex_RegisterInt(handler);
}

/*!
 * \brief Return the CPU clock in Hertz. Or peripheral clock in hertz
 * On cortex this the same.
 *
 * \return CPU clock frequency in Hertz.
 */
uint32_t NutArchClockGet(int idx)
{
    uint32_t clock = 0;
#if defined(MCU_STM32)
#if defined(MCU_STM32F1) || defined(MCU_STM32L1) || defined(MCU_STM32F2) || defined(MCU_STM32F4)
    RCC_ClocksTypeDef RCC_ClocksStatus;

    RCC_GetClocksFreq(&RCC_ClocksStatus);

    if ( idx == NUT_HWCLK_CPU ) {
        clock = RCC_ClocksStatus.SYSCLK_Frequency;
    }
    else if ( idx == NUT_HWCLOCK_HCLK){
        clock = RCC_ClocksStatus.HCLK_Frequency;
    }
    else if ( idx == NUT_HWCLK_PCLK1 ) {
        clock = RCC_ClocksStatus.PCLK1_Frequency;
    }
    else if ( idx == NUT_HWCLK_PCLK2 ) {
        clock = RCC_ClocksStatus.PCLK2_Frequency;
    }
#if defined(MCU_STM32F1)
    else if ( idx == NUT_HWCLK_ADC) {
        clock = RCC_ClocksStatus.ADCCLK_Frequency;
    }
#endif
#else
#warning "Unknown STM32 Family"
#endif

#elif defined(MCU_LPC17xx)
    clock = Lpc17xx_ClockGet(idx);
#else
#warning "Unknown CM3 family"
#endif
    return clock;
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

