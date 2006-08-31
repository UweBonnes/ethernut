#ifndef _ARCH_ARM_AT91_H_
#define _ARCH_ARM_AT91_H_

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
 */

/*!
 * \file arch/arm/at91.h
 * \brief AT91 peripherals.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.10  2006/08/31 19:04:08  haraldkipp
 * Added support for the AT91SAM9260 and Atmel's AT91SAM9260 Evaluation Kit.
 *
 * Revision 1.9  2006/08/01 07:35:59  haraldkipp
 * Exclude function prototypes when included by assembler.
 *
 * Revision 1.8  2006/07/05 07:45:25  haraldkipp
 * Split on-chip interface definitions.
 *
 * Revision 1.7  2006/06/28 17:22:34  haraldkipp
 * Make it compile for AT91SAM7X256.
 *
 * Revision 1.6  2006/05/25 09:09:57  haraldkipp
 * API documentation updated and corrected.
 *
 * Revision 1.5  2006/04/07 12:57:00  haraldkipp
 * Fast interrupt doesn't require to store R8-R12.
 *
 * Revision 1.4  2006/03/02 20:02:56  haraldkipp
 * Added ICCARM interrupt entry code. Probably not working, because I
 * excluded an immediate load.
 *
 * Revision 1.3  2006/01/05 16:52:49  haraldkipp
 * Baudrate calculation is now based on NutGetCpuClock().
 * The AT91_US_BAUD macro had been marked deprecated.
 *
 * Revision 1.2  2005/11/20 14:44:14  haraldkipp
 * Register offsets added.
 *
 * Revision 1.1  2005/10/24 10:31:13  haraldkipp
 * Moved from parent directory.
 *
 *
 * \endverbatim
 */

#if defined (MCU_AT91R40008)
#include <arch/arm/at91x40.h>
#elif defined (MCU_AT91SAM7X256)
#include <arch/arm/at91sam7x.h>
#elif defined (MCU_AT91SAM9260)
#include <arch/arm/at91sam9260.h>
#endif

/*! \addtogroup xgNutArchArmAt91 */
/*@{*/

#ifdef __GNUC__

/*!
 * \brief Interrupt entry.
 */
#define IRQ_ENTRY() \
    asm volatile("sub   lr, lr,#4"          "\n\t"  /* Adjust LR */ \
                 "stmfd sp!,{r0-r12,lr}"    "\n\t"  /* Save registers on IRQ stack. */ \
                 "mrs   r1, spsr"           "\n\t"  /* Save SPSR */ \
                 "stmfd sp!,{r1}"           "\n\t")     /* */

/*!
 * \brief Interrupt exit.
 */
#define IRQ_EXIT() \
    asm volatile("ldmfd sp!, {r1}"          "\n\t"  /* Restore SPSR */ \
                 "msr   spsr_c, r1"         "\n\t"  /* */ \
                 "ldr   r0, =0xFFFFF000"    "\n\t"  /* End of interrupt. */ \
                 "str   r0, [r0, #0x130]"   "\n\t"  /* */ \
                 "ldmfd sp!, {r0-r12, pc}^" "\n\t")     /* Restore registers and return. */

/*!
 * \brief Fast interrupt entry.
 */
#define FIQ_ENTRY() \
    asm volatile("sub   lr, lr,#4"          "\n\t"  /* Adjust LR */ \
                 "stmfd sp!,{r0-r7,lr}"    "\n\t"  /* Save registers on IRQ stack. */ \
                 "mrs   r1, spsr"           "\n\t"  /* Save SPSR */ \
                 "stmfd sp!,{r1}"           "\n\t")     /* */

/*!
 * \brief Fast interrupt exit.
 */
#define FIQ_EXIT() \
    asm volatile("ldmfd sp!, {r1}"          "\n\t"  /* Restore SPSR */ \
                 "msr   spsr_c, r1"         "\n\t"  /* */ \
                 "ldr   r0, =0xFFFFF000"    "\n\t"  /* End of interrupt. */ \
                 "str   r0, [r0, #0x130]"   "\n\t"  /* */ \
                 "ldmfd sp!, {r0-r7, pc}^" "\n\t")     /* Restore registers and return. */

#else /* __IMAGECRAFT__ */

#define IRQ_ENTRY() \
    asm("sub   lr, lr,#4\n" \
        "stmfd sp!,{r0-r12,lr}\n" \
        "mrs   r1, spsr\n" \
        "stmfd sp!,{r1}\n")

#define IRQ_EXIT() \
    asm("ldmfd sp!, {r1}\n" \
        "msr   spsr_c, r1\n" \
        ";ldr   r0, =0xFFFFF000\n" /* ICCARM: FIXME! */ \
        "str   r0, [r0, #0x130]\n" \
        "ldmfd sp!, {r0-r12, pc}^")

#define FIQ_ENTRY() \
    asm("sub   lr, lr,#4\n" \
        "stmfd sp!,{r0-r7,lr}\n" \
        "mrs   r1, spsr\n" \
        "stmfd sp!,{r1}\n")

#define FIQ_EXIT() \
    asm("ldmfd sp!, {r1}\n" \
        "msr   spsr_c, r1\n" \
        ";ldr   r0, =0xFFFFF000\n" /* ICCARM: FIXME! */ \
        "str   r0, [r0, #0x130]\n" \
        "ldmfd sp!, {r0-r7, pc}^")

#endif

/*@} xgNutArchArmAt91 */

#ifndef __ASSEMBLER__
extern void McuInit(void);
#endif

#endif                          /* _ARCH_ARM_AT91_H_ */
