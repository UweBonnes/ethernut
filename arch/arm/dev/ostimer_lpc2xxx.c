/****************************************************************************
*  This file is part of the Ethernut port for the LPC2XXX
*
*  Copyright (c) 2005 by Michael Fischer. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*  
*  1. Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
*  3. Neither the name of the author nor the names of its contributors may 
*     be used to endorse or promote products derived from this software 
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
*  SUCH DAMAGE.
*
****************************************************************************
*
*  History:
*
*  24.09.05  mifi   First Version
*                   The CrossWorks for ARM toolchain will be used. Some of 
*                   this functions looks similar like my ARM uC/OS port for 
*                   CrossWorks because I have done it before (AN-1711B).
*                   For some bit definition and information, take a look
*                   in the LPC2119/2129/2194/2292/2294 user manual 2004 May 03.
****************************************************************************/

#include <cfg/arch.h>
#include <arch/arm/lpc2xxx.h>
#include <dev/irqreg.h>

#ifndef NUT_TICK_FREQ
#define NUT_TICK_FREQ 1000UL
#endif

/*
 * This is the input frequency of 
 * the OLIMEX LPC-E2294 eval board.
 */
#define CLK_IN        14745600UL

/*
 * Some stuff for the interrupt handler
 */
typedef  void (*PFNCT)(void);
static PFNCT NutOSTimerTickHandler = NULL;

/*
 * Return the frequency of the peripheral clk
 */
static u_long GetPeripheralClk (void)
{
    u_long          msel;
    volatile u_long vpbdiv;
    u_long          CPUClkFreq;
    u_long          PeripheralClkFreq;

    msel       = (u_long)(PLLCFG & 0x1F);
    CPUClkFreq = CLK_IN * (msel + 1);

    /*
     * See errata:
     * August 5, 2005
     * Version 1.8
     * LPC2294
     *
     * VPBDIV.1 Incorrect read of VPBDIV
     */
    vpbdiv = (u_long)(VPBDIV & 0x03);
    vpbdiv = (u_long)(VPBDIV & 0x03);
    switch (vpbdiv) {
        case 0:  PeripheralClkFreq = CPUClkFreq / 4;  break;
        case 1:  PeripheralClkFreq = CPUClkFreq;      break;
        case 2:  PeripheralClkFreq = CPUClkFreq / 2;  break;
        default: PeripheralClkFreq = CPUClkFreq / 4;  break;
    }
    
    return (PeripheralClkFreq);
}

/*
 * This is our timer #0 ISR Handler
 */
static void TimerTickISRHandler (void)
{
    /*
     * Clear interrupt
     */ 
    T0IR = 0xFF;
    
    if (NutOSTimerTickHandler != NULL) {    
        (*NutOSTimerTickHandler)();    
    }    
    
    /*
     * Update the priority hardware.
     */                                             
    VICVectAddr = 0;
}


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
        for (i = 893; i--; ) {
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
    /* 
     * Save the new handler.
     */
    NutOSTimerTickHandler = (PFNCT)handler;

    /* 
     * Disable timer.
     */
    T0TCR = 0;
    
    /* 
     * TC is incrementet on every pclk.
     */
    T0PC  = 0;
    
    /* 
     * Set match register.
     */
    T0MR0 = (GetPeripheralClk() / NUT_TICK_FREQ);
    
    /* 
     * An interrupt is generated when MR0 matches the value in TC.
     * The TC will be reset if MR0 matches it.
     */
    T0MCR = 3;
    
    /*
     * Capture feature is disabled.
     */
    T0CCR = 0;
    
    /*
     * No external match.
     */
    T0EMR = 0;
    
    /*
     * Enable timer.
     */
    T0TCR = 1;

    /* 
     * VIC TIMER #0 Initialization
     */
    VICVectAddr2  = (u_long)TimerTickISRHandler;
    VICVectCntl2  = 0x20 | VIC_TIMER0;
}


/*!
 * \brief Return the CPU clock in Hertz.
 *
 * \return CPU clock frequency in Hertz.
 */
u_long NutGetCpuClock(void)
{
    u_long msel;
    u_long CPUClkFreq;

    msel       = (u_long)(PLLCFG & 0x1F);
    CPUClkFreq = CLK_IN * (msel + 1);

    return CPUClkFreq;
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
    u_long x;

    x = ms * NutGetTickClock() / 1000UL;
    if (x == 0) {
        x = 1;
    }
    
    return (x);
}

/*
 * Enable timer interrupt.
 */
void NutEnableTimerIrq (void)
{
    VICIntEnable  = (1 << VIC_TIMER0);
}

/*
 * Disable timer interrupt.
 */
void NutDisableTimerIrq (void)
{
    VICIntEnClr = (1 << VIC_TIMER0);
}

