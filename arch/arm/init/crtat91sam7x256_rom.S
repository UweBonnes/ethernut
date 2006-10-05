/*
 * Copyright (C) 2005-2006 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.4  2006/10/05 17:12:36  haraldkipp
 * Bug #1497065 fixed. Stack sizes increased.
 *
 * Revision 1.3  2006/07/18 14:02:55  haraldkipp
 * Low level hardware initialization moved from nutinit.c to
 * crtat91sam7x256_rom.S. This avoids the ugly jump from C code back into
 * the runtime initialization.
 * All segment entry labels moved into the specific segment. Still not
 * sure how the linker assembles segments. Possibly we need additional
 * empty segments, which may be filled from C code.
 *
 * Revision 1.2  2006/07/15 11:12:51  haraldkipp
 * CPU ran into the data pool of Sam7xLowLevelInit(). Temporarily
 * fixed by Andras Albert with an additional global label.
 *
 * Revision 1.1  2006/07/05 07:51:05  haraldkipp
 * AT91SAM7X256 startup code and linker script, contributed by Daidai.
 *
 */

IRQ_STACK_SIZE = (128*4)        @ 128 words for interrupt nesting
FIQ_STACK_SIZE = (64*4)         @ 64 words
ABT_STACK_SIZE = (32*4)         @ 16 word
UND_STACK_SIZE = (32*4)         @ 16 word

ARM_MODE_USER = 0x10
ARM_MODE_FIQ = 0x11
ARM_MODE_IRQ = 0x12
ARM_MODE_SVC = 0x13
ARM_MODE_ABORT = 0x17
ARM_MODE_UNDEF = 0x1B
ARM_MODE_SYS = 0x1F

AIC_BASE = 0xFFFFF000
AIC_EOICR_OFF = 0x130
AIC_IDCR_OFF = 0x124

PMC_BASE = 0xFFFFFC00

CKGR_MOR_OFF = 0x00000020
CKGR_MOSCEN = 0x00000001

CKGR_PLLR_OFF = 0x0000002C

PMC_MCKR_OFF = 0x00000030
PMC_CSS_MAIN_CLK = 0x00000001
PMC_CSS_PLL_CLK = 0x00000003
PMC_PRES_CLK_2 = 0x00000004

PMC_SR_OFF = 0x00000068
PMC_MOSCS = 0x00000001
PMC_MCKRDY = 0x00000008
PMC_LOCK = 0x00000004

RSTC_MR = 0xFFFFFD08
RSTC_KEY = 0xA5000000
RSTC_URSTEN = 0x00000001

WDT_BASE = 0xFFFFFD40
WDT_MR_OFF = 0x00000004
WDT_WDDIS = 0x00008000

MC_BASE = 0xFFFFFF00
MC_FMR_OFF = 0x00000060
MC_FWS_1FWS = 0x00000100

/*
 * Section 0: Vector table and reset entry.
 */
        .section .init0,"ax",%progbits

        .global __vectors
__vectors:
        ldr     pc, [pc, #24]   /* Reset */
        ldr     pc, [pc, #24]   /* Undefined instruction */
        ldr     pc, [pc, #24]   /* Software interrupt */
        ldr     pc, [pc, #24]   /* Prefetch abort */
        ldr     pc, [pc, #24]   /* Data abort */
        ldr     pc, [pc, #24]   /* Reserved */

        /*
         * On IRQ the PC will be loaded from AIC_IVR, which
         * provides the address previously set in AIC_SVR.
         * The interrupt routine will be called in ARM_MODE_IRQ
         * with IRQ disabled and FIQ unchanged.
         */
        ldr     pc, [pc, #-0xF20]   /* Interrupt request, auto vectoring. */
        ldr     pc, [pc, #-0xF20]   /* Fast interrupt request, auto vectoring. */

        .word   _start
        .word   __undef
        .word   __swi
        .word   __prefetch_abort
        .word   __data_abort

        .weak   __undef
        .set    __undef, __xcpt_dummy
        .weak   __swi
        .set    __swi, __xcpt_dummy
        .weak   __prefetch_abort
        .set    __prefetch_abort, __xcpt_dummy
        .weak   __data_abort
        .set    __data_abort, __xcpt_dummy

        .global __xcpt_dummy
__xcpt_dummy:
        b       __xcpt_dummy

        .ltorg
/*
 * Section 1: Hardware initialization.
 */
        .section .init1, "ax", %progbits
        .globl	_start
_start:

        /*
         * Use 2 cycles for flash access.
         */
        ldr     r1, =MC_BASE
        mov     r0, #MC_FWS_1FWS
        str     r0, [r1, #MC_FMR_OFF]

        /*
         * Disable all interrupts. Useful for debugging w/o target reset.
         */
        ldr     r1, =AIC_BASE
        mvn     r0, #0
        str     r0, [r1, #AIC_EOICR_OFF]
        str     r0, [r1, #AIC_IDCR_OFF]

        /*
         * The watchdog is enabled after processor reset. Disable it.
         */
        ldr     r1, =WDT_BASE
        ldr     r0, =WDT_WDDIS
        str     r0, [r1, #WDT_MR_OFF]

        /*
         * Enable the main oscillator. Set startup time of 6 * 8 slow 
         * clock cycles and wait until oscillator is stabilized.
         */
        ldr     r1, =PMC_BASE
        mov     r0, #(6 << 8)
        orr     r0, r0, #CKGR_MOSCEN
        str     r0, [r1, #CKGR_MOR_OFF]
wait_moscs:
        ldr     r0, [r1, #PMC_SR_OFF]
        tst     r0, #PMC_MOSCS
        beq     wait_moscs

        /*
         * Set PLL:
         * PLLfreq = crystal / divider * (multiplier + 1)
         * Crystal is 18.432 MHz, divider is 5 and multiplier is 25.
         * Thus, the resulting frequency is 95.8464 MHz.
         * Wait 28 clock cycles until PLL is locked.
         */
@        ldr     r0, =((25 << 16) | (28 << 8) | 5)
        ldr     r0, =((48 << 16) | (28 << 8) | 10)
        str     r0, [r1, #CKGR_PLLR_OFF]
wait_lock:
        ldr     r0, [r1, #PMC_SR_OFF]
        tst     r0, #PMC_LOCK
        beq     wait_lock

        /*
         * Set prescaler to 2 and switch to PLL clock, resulting in a 
         * final master clock of 47.9232 MHz.
         */
        mov     r0, #PMC_PRES_CLK_2
        str     r0, [r1, #PMC_MCKR_OFF]
wait_presrdy:
        ldr     r0, [r1, #PMC_SR_OFF]
        tst     r0, #PMC_MCKRDY
        beq     wait_presrdy

        /*
         * Switch to PLL clock. Trying to set this together with the
         * prescaler fails for unknown reason.
         */
        ldr     r0, [r1, #PMC_MCKR_OFF]
        orr     r0, r0, #PMC_CSS_PLL_CLK
        str     r0, [r1, #PMC_MCKR_OFF]
wait_pllsel:
        ldr     r0, [r1, #PMC_SR_OFF]
        tst     r0, #PMC_MCKRDY
        beq     wait_pllsel

        /*
         * Enable external reset key.
         */
        ldr     r0, =(RSTC_KEY | RSTC_URSTEN)
        ldr     r1, =RSTC_MR
        str     r0, [r1, #0]
        
        b       __set_stacks

        .ltorg


/*
 * Section 2: Set stack pointers.
 */
        .section .init2,"ax",%progbits        
        .global __set_stacks
__set_stacks:

        /*
         * Set exception stack pointers and enable interrupts.
         */
        ldr     r0, =__exp_stack
        msr     CPSR_c, #ARM_MODE_FIQ | 0xC0
        mov     r13, r0
        sub     r0, r0, #FIQ_STACK_SIZE
        msr     CPSR_c, #ARM_MODE_IRQ | 0xC0
        mov     r13, r0
        sub     r0, r0, #IRQ_STACK_SIZE
        msr     CPSR_c, #ARM_MODE_ABORT | 0xC0
        mov     r13, r0
        sub     r0, r0, #ABT_STACK_SIZE
        msr     CPSR_c, #ARM_MODE_UNDEF | 0xC0
        mov     r13, r0
        sub     r0, r0, #UND_STACK_SIZE
        msr     CPSR_c, #ARM_MODE_SVC | 0xC0
        mov     r13, r0
        b       __enter_mode
        
        .ltorg

/*
 * Section 3: Enter user mode.
 */
        .section .init3,"ax",%progbits

        .global __enter_mode
__enter_mode:

@        msr     CPSR_c, #ARM_MODE_USER

        b       __clear_bss

        .ltorg

/*
 * Section 4: Clear bss and copy data.
 */
        .section .init4,"ax",%progbits
        .global __clear_bss
__clear_bss:

        ldr     r1, =__bss_start
        ldr     r2, =__bss_end
        ldr     r3, =0

_40:
        cmp     r1, r2
        strne   r3, [r1], #+4
        bne     _40

        /*
         * Relocate .data section (Copy from ROM to RAM).
         */
        ldr     r1, =__etext
        ldr     r2, =__data_start
        ldr     r3, =__data_end

_41:
        cmp     r2, r3
        ldrlo   r0, [r1], #4
        strlo   r0, [r2], #4
        blo     _41

        /*
         * Initialize user stack pointer.
         */
        ldr     r13, =__stack
        b       __call_rtos

        .ltorg

/*
 * Section 5: Call RTOS
 */
        .section .init5,"ax",%progbits
        .global __call_rtos
__call_rtos:

        /*
         * Jump to Nut/OS initialization.
         */
        ldr     r0, =NutInit
        bx      r0

End:
        b       End

        .ltorg
