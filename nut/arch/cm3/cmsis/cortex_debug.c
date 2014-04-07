/*
 * Copyright (C) 2012 by Ole Reinhardt (ole.reinhardt@embedded-it.de)
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
 * \file arch/cm3/cmsis/cortex_debug.c
 *
 * \verbatim
 * $Id: cortex_debug.c $
 * \endverbatim
 */


#include <cfg/arch.h>
#include <sys/types.h>
#include <inttypes.h>
#include <cfg/cortex_debug.h>
#include <arch/cm3/cortex_debug.h>

#if defined (MCU_LPC17xx)
#include <arch/cm3/nxp/lpc17xx_debug_macro.h>
#elif defined (MCU_STM32)
#include <arch/cm3/stm/stm32_debug_macro.h>
#endif


static char *exception_names[6] = {"Default", "NMI", "Hard Fault", "Mem Fault", "Bus Fault", "Usage Fault"};

static void DebugPrint(const char *str)
{
#ifdef DEBUG_MACRO
    while (*str != 0) {
        if (*str == '\n') DebugPut('\r');
        DebugPut(*str);
        str++;
    }
#endif
}

static void DebugPrintU32Hex(uint32_t val) {
#ifdef DEBUG_MACRO
    int i;
    for (i = 7; i >= 0; i--) {
        register int nibble = (val >> (4 * i)) & 0x0F;
        DebugPut(nibble < 10 ? '0' + nibble : 'a' + nibble - 10);
    }
#endif
}

void CortexRegDump(uint32_t *arg, int exception_type)
{
    unsigned int stacked_r0;
    unsigned int stacked_r1;
    unsigned int stacked_r2;
    unsigned int stacked_r3;
    unsigned int stacked_r12;
    unsigned int stacked_lr;
    unsigned int stacked_pc;
    unsigned int stacked_psr;

    DebugPrint("\n---------------------------------------------------\n");
    stacked_r0 = ((unsigned long) arg[0]);
    stacked_r1 = ((unsigned long) arg[1]);
    stacked_r2 = ((unsigned long) arg[2]);
    stacked_r3 = ((unsigned long) arg[3]);

    stacked_r12 = ((unsigned long) arg[4]);
    stacked_lr  = ((unsigned long) arg[5]);
    stacked_pc  = ((unsigned long) arg[6]);
    stacked_psr = ((unsigned long) arg[7]);

    DebugPrint("["); DebugPrint(exception_names[exception_type]); DebugPrint(" handler - all numbers in hex]\n");

#if defined(SCB_HFSR_FORCED_Msk)
    if (exception_type == 2) {
        /* This is a hard fault handler */
        if ((SCB->HFSR & SCB_HFSR_FORCED_Msk) != 0) {
            DebugPrint("Forced Hard Fault!\n");
        }
    }
#endif

    DebugPrint("\nR0        = 0x"); DebugPrintU32Hex(stacked_r0);
    DebugPrint("\nR1        = 0x"); DebugPrintU32Hex(stacked_r1);
    DebugPrint("\nR2        = 0x"); DebugPrintU32Hex(stacked_r2);
    DebugPrint("\nR3        = 0x"); DebugPrintU32Hex(stacked_r3);
    DebugPrint("\nR12       = 0x"); DebugPrintU32Hex(stacked_r12);
    DebugPrint("\nLR [R14]  = 0x"); DebugPrintU32Hex(stacked_lr);
    DebugPrint("\nPC [R15]  = 0x"); DebugPrintU32Hex(stacked_pc);
    DebugPrint("\nPSR       = 0x"); DebugPrintU32Hex(stacked_psr);
    DebugPrint("\nBFAR      = 0x"); DebugPrintU32Hex(*((volatile unsigned long *)(0xE000ED38)));
    DebugPrint("\nCFSR      = 0x"); DebugPrintU32Hex(*((volatile unsigned long *)(0xE000ED28)));
    DebugPrint("\nHFSR      = 0x"); DebugPrintU32Hex(*((volatile unsigned long *)(0xE000ED2C)));
    DebugPrint("\nDFSR      = 0x"); DebugPrintU32Hex(*((volatile unsigned long *)(0xE000ED30)));
    DebugPrint("\nAFSR      = 0x"); DebugPrintU32Hex(*((volatile unsigned long *)(0xE000ED3C)));
    DebugPrint("\nSCB_SHCSR = 0x"); DebugPrintU32Hex(SCB->SHCSR);
    DebugPrint("\n---------------------------------------------------\n");

    __asm("BKPT #0\n") ; // Break into the debugger
    while(1);
}
