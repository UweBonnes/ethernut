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
 * Revision 1.1  2005/05/27 17:16:40  drsung
 * Moved the file.
 *
 * Revision 1.4  2005/04/05 17:54:36  haraldkipp
 * Moved from user mode to system mode. Probably breaks the GBA port.
 *
 * Revision 1.3  2004/11/08 19:15:33  haraldkipp
 * Made assembly includes look nicer.
 * Changed mode from supervisory to user supervisory, which seems to work
 * with the GBA.
 * Skipped entry frame, because it simply confuses me. :-)
 *
 * Revision 1.2  2004/09/08 10:19:31  haraldkipp
 * Tyou's support for the ARM7
 *
 * Revision 1.1  2004/03/16 16:48:46  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.2  2004/02/18 16:32:48  drsung
 * Bugfix in NutThreadCreate. Thanks to Mike Cornelius.
 *
 * Revision 1.1  2004/02/01 18:49:48  haraldkipp
 * Added CPU family support
 *
 */

/*!
 * \brief ARM7TDMI GCC context switch frame layout.
 *
 * This is the layout of the stack after a thread's context has been
 * switched-out.
 */
typedef struct {
	u_long csf_spsr;
	u_long csf_cpsr;
    u_long csf_r0;
	u_long csf_r1;
	u_long csf_r2;
	u_long csf_r3;
	u_long csf_r4;
	u_long csf_r5;
	u_long csf_r6;
	u_long csf_r7;
	u_long csf_r8;
	u_long csf_r9;
	u_long csf_r10;
	u_long csf_r11;
	u_long csf_r12;
	u_long csf_lr;
    u_long csf_pc;
} SWITCHFRAME;

/*!
 * \brief Thread entry frame layout.
 *
 * This is the stack layout being build to enter a new thread.
 */
typedef struct {
	u_long cef_cpsr;
    u_long cef_r0;
    u_long cef_pc;
} ENTERFRAME;

/*
 * This code is executed when entering a thread.
 */
void NutThreadEntry(void) __attribute__ ((naked));
void NutThreadEntry(void)
{
    asm volatile(
	    "ldmfd   sp!, {r0}\n\t"     /* */
	    "msr     cpsr_all, r0\n\t"  /* */
	    "ldmfd   sp!, {r0, pc}"     /* */
	);
}

/*!
 * \brief Switch to another thread.
 *
 * Stop the current thread, saving its context. Then start the
 * one with the highest priority, which is ready to run.
 *
 * Application programs typically do not call this function.
 *
 * \note CPU interrupts must be disabled before calling this function.
 *
 */
void NutThreadSwitch(void) __attribute__ ((naked));
void NutThreadSwitch(void)
{
/*
 * arm-elf-gcc can not recognize attribute noinline.
 * It won't be inlined because there are global label thread_start in func?
 * static void NutThreadSwitch(void) __attribute__ ((noinline));
 */
    /*
     * Save all CPU registers.
     */
    asm volatile(                       /* */
        "@ Save context\n\t"            /* */
		"stmfd   sp!, {lr}\n\t"         /* */
		"stmfd   sp!, {r0-r12,lr}\n\t"  /* */
		"mrs     r4, cpsr\n\t"          /* */
		"stmfd   sp!, {r4}\n\t"         /* */
		"mrs     r4, spsr\n\t"          /* */
		"stmfd   sp!, {r4}\n\t"         /* */
		"str     sp, %0"                /* */
		::"m"(runningThread->td_sp)     /* */
		:"cc"
	);

    /*
     * This defines a global label, which may be called
     * as an entry point into this function.
     */
    asm volatile (
    	".global thread_start\n"
    	"thread_start:"
    );

    /*
     * Reload CPU registers from the thread on top of the run queue.
     */
    runningThread = runQueue;
    runningThread->td_state = TDS_RUNNING;

	asm volatile(                       /* */
        "@ Load context\n\t"            /* */
        "ldr     sp, %0\n\t"            /* */
		"ldmfd 	 sp!, {r4}\n\t"         /* */
		"msr     spsr_all, r4\n\t"      /* */
		"ldmfd   sp!, {r4}\n\t"         /* */
                "bic     r4, r4, #0x80" "\n\t"  /* */
		"msr     cpsr_all, r4\n\t"      /* */
		"ldmfd   sp!, {r0-r12, lr, pc}" /* */
        ::"m"(runningThread->td_sp)     /* */
    );
}

/*!
 * \brief Create a new thread.
 *
 * If the current thread's priority is lower or equal than the default
 * priority (64), then the current thread is stopped and the new one
 * is started.
 *
 * \param name      String containing the symbolic name of the new thread,
 *                  up to 8 characters long.
 * \param fn        The thread's entry point, typically created by the
 *                  THREAD macro.
 * \param arg       Argument pointer passed to the new thread.
 * \param stackSize Number of bytes of the stack space allocated for
 *                  the new thread.
 *
 * \return Pointer to the NUTTHREADINFO structure or 0 to indicate an
 *         error.
 */
HANDLE NutThreadCreate(u_char * name, void (*fn) (void *), void *arg, size_t stackSize)
{
    u_char *threadMem;
    SWITCHFRAME *sf;
    ENTERFRAME *ef;
    NUTTHREADINFO *td;

    NutEnterCritical();

    /*
     * Allocate stack and thread info structure in one block.
     * Confirm the mem size be 4 bytes aligned.
     * sizeof(NUTTHREADINFO) be 4 bytes too.
     */
    if ((threadMem = NutHeapAlloc(stackSize + sizeof(NUTTHREADINFO))) == 0) {
        NutJumpOutCritical();
        return 0;
    }

    /*
     * threadMem --> bottom
     * sf
     * ef
     * td
     */
    td = (NUTTHREADINFO *) (threadMem + stackSize);
    ef = (ENTERFRAME *) ((uptr_t) td - sizeof(ENTERFRAME));
    sf = (SWITCHFRAME *) ((uptr_t) ef - sizeof(SWITCHFRAME));

    memcpy(td->td_name, name, sizeof(td->td_name) - 1);
    td->td_name[sizeof(td->td_name) - 1] = 0;
    td->td_sp = (uptr_t) sf; /* FIXME */
    td->td_memory = threadMem;
    *((u_long *) threadMem) = DEADBEEF;
    *((u_long *) (threadMem + 4)) = DEADBEEF;
    *((u_long *) (threadMem + 8)) = DEADBEEF;
    *((u_long *) (threadMem + 12)) = DEADBEEF;
    td->td_priority = 64;
    /*
     * Setup entry frame to simulate C function entry.
     */

    /*
     * 0101003 SVC mode, irq enabled.
     * the first thread , idle thread, will jump
     * to start without NutExitCritical.
     */
    ef->cef_cpsr = 0xD3;
    ef->cef_pc   = (uptr_t) fn;
    ef->cef_r0 = (uptr_t) arg;

    //sf->csf_pc = (uptr_t) NutThreadEntry;
    sf->csf_pc = (uptr_t) fn;

    /*
     * SVC mode, irq need to be disabled when context switching.
     */
    sf->csf_cpsr = 0xD3;
    sf->csf_spsr = 0xD3;

    /*
     * Insert into the thread list and the run queue.
     */
    td->td_next = nutThreadList;
    nutThreadList = td;
    td->td_state = TDS_READY;
    td->td_timer = 0;
    td->td_queue = 0;

    NutThreadAddPriQueue(td, (NUTTHREADINFO **) & runQueue);

    /*
     * If no thread is active, switch to new thread.
     */
    if (runningThread == 0) {
        asm volatile("\n\tb thread_start\n\t");
    }

    /*
     * If current context is not in front of
     * the run queue (highest priority), then
     * switch to the thread in front.
     */
    if (runningThread != runQueue) {
        runningThread->td_state = TDS_READY;
        NutThreadSwitch();
    }
    NutExitCritical();

    return td;
}
