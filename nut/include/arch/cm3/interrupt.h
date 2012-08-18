#ifndef _ARCH_CM3_INTERRUPT_H_
#define _ARCH_CM3_INTERRUPT_H_

/*
 * Copyright (c) 2005-2012 Texas Instruments Incorporated
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
 * This is part of revision 9107 of the Stellaris Peripheral Driver Library.
 */

/*!
 * \file arch/cm3/systick.h
 * \brief Prototypes for the NVIC Interrupt Controller Driver.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#if defined(SAM3U)
#define NUM_INTERRUPTS       48
#endif
#if defined(MCU_STM32)
#define NUM_INTERRUPTS      121
#endif
#if defined(MCU_LPC176x)
#define NUM_INTERRUPTS       35
#endif
#if defined(MCU_LPC177x_8x)
#define NUM_INTERRUPTS       41
#endif

/*!
 * \brief Macro to generate an interrupt priority mask.
 *
 * Based on the number of bits of priority supported by the hardware.
 */
#define INT_PRIORITY_MASK   ((0xFF << (8 - NUM_PRIORITY_BITS)) & 0xFF)

extern int IntMasterEnable(void);
extern int IntMasterDisable(void);

extern void IntRegister(unsigned long ulInterrupt, void (*pfnHandler)(void));
extern void IntUnregister(unsigned long ulInterrupt);

extern void IntPriorityGroupingSet(unsigned long ulBits);
extern unsigned long IntPriorityGroupingGet(void);

extern void IntPrioritySet(unsigned long ulInterrupt, unsigned char ucPriority);
extern long IntPriorityGet(unsigned long ulInterrupt);

extern void IntEnable(unsigned long ulInterrupt);
extern void IntDisable(unsigned long ulInterrupt);
extern unsigned long IntIsEnabled(unsigned long ulInterrupt);

extern void IntPendSet(unsigned long ulInterrupt);
extern void IntPendClear(unsigned long ulInterrupt);

extern void IntPriorityMaskSet(unsigned long ulPriorityMask);
extern unsigned long IntPriorityMaskGet(void);

#endif
