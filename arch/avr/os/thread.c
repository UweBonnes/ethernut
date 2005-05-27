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
 * Revision 1.1  2005/05/27 17:17:31  drsung
 * Moved the file
 *
 * Revision 1.6  2005/04/30 16:42:42  chaac
 * Fixed bug in handling of NUTDEBUG. Added include for cfg/os.h. If NUTDEBUG
 * is defined in NutConf, it will make effect where it is used.
 *
 * Revision 1.5  2005/02/16 19:55:18  haraldkipp
 * Ready-to-run queue handling removed from interrupt context.
 * Avoid AVRGCC prologue and epilogue code. Thanks to Pete Allinson.
 *
 * Revision 1.4  2005/02/10 07:06:48  hwmaier
 * Changes to incorporate support for AT90CAN128 CPU
 *
 * Revision 1.3  2004/09/22 08:15:56  haraldkipp
 * Speparate IRQ stack configurable
 *
 * Revision 1.2  2004/04/25 17:06:17  drsung
 * Separate IRQ stack now compatible with nested interrupts.
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

#include <cfg/os.h>

/* Support for separate irq stack only for avr-gcc */
#ifdef __GNUC__
#include <dev/irqstack.h>
#ifdef IRQSTACK_SIZE
/*!  * \brief Decrement value for thread's stack size, if separate irq stack is used.
 *
 * If separate irq stack is enabled (avr-gcc only), the initial parameter
 * 'stacksize' in function 'NutThreadCreate' is decremented by this value, if the
 * remaining stack size is 128 bytes or greater.
 *
 */
u_short _irqstackdec = 128;
#endif /* #ifdef IRQSTACK_SIZE */
#endif /* #ifdef __GNU__ */

#ifdef __GNUC__

/*!
 * \brief AVR GCC context switch frame layout.
 *
 * This is the layout of the stack after a thread's context has been
 * switched-out.
 */
typedef struct {
    u_char csf_r29;
    u_char csf_r28;
    u_char csf_r17;
    u_char csf_r16;
    u_char csf_r15;
    u_char csf_r14;
    u_char csf_r13;
    u_char csf_r12;
    u_char csf_r11;
    u_char csf_r10;
    u_char csf_r9;
    u_char csf_r8;
    u_char csf_r7;
    u_char csf_r6;
    u_char csf_r5;
    u_char csf_r4;
    u_char csf_r3;
    u_char csf_r2;
    u_char csf_pchi;
    u_char csf_pclo;
} SWITCHFRAME;

#else                           /* ImageCraft */

/*!
 * \brief ICC AVR context switch frame layout.
 *
 * This is the layout of the stack after a thread's context has been
 * switched-out.
 */
typedef struct {
    u_char csf_r29;
    u_char csf_r28;
    u_char csf_r23;
    u_char csf_r22;
    u_char csf_r21;
    u_char csf_r20;
    u_char csf_r15;
    u_char csf_r14;
    u_char csf_r13;
    u_char csf_r12;
    u_char csf_r11;
    u_char csf_r10;
    u_char csf_pchi;
    u_char csf_pclo;
} SWITCHFRAME;

#endif

/*!
 * \brief Thread entry frame layout.
 *
 * This is the stack layout being build to enter a new thread.
 */
typedef struct {
    u_char cef_arghi;
    u_char cef_arglo;
#ifdef __IMAGECRAFT__
    /*!
     * \brief ImageCraft software stack.
     */
    u_char cef_yhi;
    u_char cef_ylo;
#endif
    u_char cef_rampz;
    u_char cef_sreg;
    u_char cef_r1;
    u_char cef_pchi;
    u_char cef_pclo;
} ENTERFRAME;


#ifdef __GNUC__


/*
 * This code is executed when entering a thread.
 */
static void NutThreadEntry(void) __attribute__ ((naked));
static void NutThreadEntry(void)
{
    __asm__ __volatile__("pop r25" "\n\t"       /* first parameter hi-byte */
                         "pop r24" "\n\t"       /* first parameter lo-byte */
                         "pop __tmp_reg__" "\n\t"       /* Get RAMPZ */
                         "out %0, __tmp_reg__" "\n\t"   /* Restore RAMPZ */
                         "pop __tmp_reg__" "\n\t"       /* Get SREG */
                         "out %1, __tmp_reg__" "\n\t"   /* Restore SREG */
                         "pop __zero_reg__" "\n\t"      /* Zero register */
                         "reti" "\n\t"  /* enables interrupts */
                         ::"I" _SFR_IO_ADDR(RAMPZ), "I" _SFR_IO_ADDR(SREG)
        );
}


#else

static void NutThreadEntry(void)
{
    asm("pop r17");             // first parameter for ICC
    asm("pop r16");
    asm("pop r29");             // SW-Stack; Y-Register
    asm("pop r28");
    asm("pop r0");              // r0 = _tmp_reg_
    asm("out 0x3B, r0");        // RAMPZ; replace with define later
    asm("pop r0");
    asm("pop r1");              // r1 = _zero_reg_
    asm("out 0x3F, r0");        // SREG; replace with define later
    asm("reti");
}

#endif

#ifdef __GNUC__
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
void NutThreadSwitch(void) __attribute__ ((noinline)) __attribute__ ((naked));
void NutThreadSwitch(void)
{
    /*
     * Save all CPU registers.
     */
    asm volatile ("push r2" "\n\t"              /* */
              "push r3" "\n\t"              /* */
              "push r4" "\n\t"              /* */
              "push r5" "\n\t"              /* */
              "push r6" "\n\t"              /* */
              "push r7" "\n\t"              /* */
              "push r8" "\n\t"              /* */
              "push r9" "\n\t"              /* */
              "push r10" "\n\t"             /* */
              "push r11" "\n\t"             /* */
              "push r12" "\n\t"             /* */
              "push r13" "\n\t"             /* */
              "push r14" "\n\t"             /* */
              "push r15" "\n\t"             /* */
              "push r16" "\n\t"             /* */
              "push r17" "\n\t"             /* */
              "push r28" "\n\t"             /* */
              "push r29" "\n\t"             /* */
              "in %A0, %1" "\n\t"           /* */
              "in %B0, %2" "\n\t"           /* */
              :"=r" (runningThread->td_sp)  /* */
              :"I" _SFR_IO_ADDR(SPL),       /* */
               "I" _SFR_IO_ADDR(SPH)        /* */
    );

    /*
     * This defines a global label, which may be called
     * as an entry point into this function.
     */
    asm volatile (".global thread_start\n"  /* */
                  "thread_start:\n\t"::);

    /*
     * Reload CPU registers from the thread on top of the run queue.
     */
    runningThread = runQueue;
    runningThread->td_state = TDS_RUNNING;
    asm volatile ("out %1, %A0" "\n\t"          /* */
              "out %2, %B0" "\n\t"          /* */
              "pop r29" "\n\t"              /* */
              "pop r28" "\n\t"              /* */
              "pop r17" "\n\t"              /* */
              "pop r16" "\n\t"              /* */
              "pop r15" "\n\t"              /* */
              "pop r14" "\n\t"              /* */
              "pop r13" "\n\t"              /* */
              "pop r12" "\n\t"              /* */
              "pop r11" "\n\t"              /* */
              "pop r10" "\n\t"              /* */
              "pop r9" "\n\t"               /* */
              "pop r8" "\n\t"               /* */
              "pop r7" "\n\t"               /* */
              "pop r6" "\n\t"               /* */
              "pop r5" "\n\t"               /* */
              "pop r4" "\n\t"               /* */
              "pop r3" "\n\t"               /* */
              "pop r2" "\n\t"               /* */
              "ret" "\n\t"                  /* */
              ::"r" (runningThread->td_sp), /* */
                "I" _SFR_IO_ADDR(SPL),      /* */
                "I" _SFR_IO_ADDR(SPH)       /* */
    );
}
#else

void NutThreadSwitch(void)
{
    /*
     * Save all CPU registers.
     */
    register u_char i = 0;
    register u_char j = 0;

    asm("push r10");
    asm("push r11");
    asm("push r12");
    asm("push r13");
    asm("push r14");
    asm("push r15");
    asm("push r20");
    asm("push r21");
    asm("push r22");
    asm("push r23");
    asm("push r28");
    asm("push r29");
    asm("in %i, $3D");          // SPL
    asm("in %j, $3E");          // SPH

    runningThread->td_sp = (((u_short) j) << 8) & 0xFF00 | (i & 0xFF);

    /*
     * This defines a global label, which may be called
     * as an entry point into this function.
     */
    asm(".globl thread_start");
    asm("thread_start:");

    /*
     * Reload CPU registers from the thread in front
     * of the run queue.
     */
    runningThread = runQueue;
    runningThread->td_state = TDS_RUNNING;

    i = (u_char) (runningThread->td_sp & 0xFF);
    j = (u_char) ((runningThread->td_sp >> 8) & 0xFF);

    asm("out $3D, %i");         // SPL
    asm("out $3E, %j");         // SPH
    asm("pop r29");
    asm("pop r28");
    asm("pop r23");
    asm("pop r22");
    asm("pop r21");
    asm("pop r20");
    asm("pop r15");
    asm("pop r14");
    asm("pop r13");
    asm("pop r12");
    asm("pop r11");
    asm("pop r10");
}
#endif


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
#ifdef __IMAGECRAFT__
    u_short yreg;
#endif
#ifdef IRQSTACK_SIZE
    if (stackSize > _irqstackdec + 128) stackSize -= _irqstackdec;
#endif

    const u_short *paddr;
    NutEnterCritical();
    paddr = (const u_short *) fn;       // *KU* fn doesn't contain the entry address
    // of the thread!
    /*
     * Allocate stack and thread info structure in one block.
     */
    if ((threadMem = NutHeapAlloc(stackSize + sizeof(NUTTHREADINFO))) == 0) {
        NutExitCritical();
        return 0;
    }

    td = (NUTTHREADINFO *) (threadMem + stackSize);
    ef = (ENTERFRAME *) ((u_short) td - sizeof(ENTERFRAME));
    sf = (SWITCHFRAME *) ((u_short) ef - sizeof(SWITCHFRAME));


    memcpy(td->td_name, name, sizeof(td->td_name) - 1);
    td->td_name[sizeof(td->td_name) - 1] = 0;
    td->td_sp = (u_short) sf - 1;
    td->td_memory = threadMem;
    *((u_long *) threadMem) = DEADBEEF;
    *((u_long *) (threadMem + 4)) = DEADBEEF;
    *((u_long *) (threadMem + 8)) = DEADBEEF;
    *((u_long *) (threadMem + 12)) = DEADBEEF;
    td->td_priority = 64;

    /*
     * Setup entry frame to simulate C function entry.
     */
#ifdef __IMAGECRAFT__
    ef->cef_pchi = (u_char) (*paddr >> 8);
    ef->cef_pclo = (u_char) (*paddr & 0xff);
#else
    ef->cef_pchi = (u_char) (((u_short) fn) >> 8);
    ef->cef_pclo = (u_char) (((u_short) fn) & 0xff);
#endif
    ef->cef_sreg = 0x80;
    ef->cef_rampz = 0;
    ef->cef_r1 = 0;

    ef->cef_arglo = (u_char) (((u_short) arg) & 0xff);
    ef->cef_arghi = (u_char) (((u_short) arg) >> 8);

#ifdef __IMAGECRAFT__
    yreg = td->td_sp - 40;
    ef->cef_yhi = (u_char) (yreg >> 8);
    ef->cef_ylo = (u_char) (yreg & 0xFF);
#endif


#ifdef __IMAGECRAFT__
    paddr = (const u_short *) NutThreadEntry;
    sf->csf_pchi = (u_char) (*paddr >> 8);
    sf->csf_pclo = (u_char) (*paddr & 0xff);
#else
    sf->csf_pchi = (u_char) (((u_short) NutThreadEntry) >> 8);
    sf->csf_pclo = (u_char) (((u_short) NutThreadEntry) & 0xff);
#endif


    /*
     * Insert into the thread list and the run queue.
     */
//    NutEnterCritical();
    td->td_next = nutThreadList;
    nutThreadList = td;
    td->td_state = TDS_READY;
    td->td_timer = 0;
    td->td_queue = 0;
#ifdef NUTDEBUG
    if (__os_trf)
        fprintf(__os_trs, "Cre<%04x>", (uptr_t) td);
#endif
    NutThreadAddPriQueue(td, (NUTTHREADINFO **) & runQueue);
#ifdef NUTDEBUG
    if (__os_trf)
        NutDumpThreadList(__os_trs);
    //NutDumpThreadQueue(__os_trs, runQueue);
#endif

    /*
     * If no thread is active, switch to new thread.
     */
#ifndef __IMAGECRAFT__
    if (runningThread == 0)
        asm volatile ("rjmp thread_start\n\t"::);
#else
    if (runningThread == 0)
        asm("rjmp thread_start");
#endif

    /*
     * If current context is not in front of
     * the run queue (highest priority), then
     * switch to the thread in front.
     */
    if (runningThread != runQueue) {
        runningThread->td_state = TDS_READY;
#ifdef NUTDEBUG
        if (__os_trf)
            fprintf(__os_trs, "New<%04x %04x>", (uptr_t) runningThread, (uptr_t) runQueue);
#endif
        NutThreadSwitch();
    }
    NutExitCritical();

    return td;
}
