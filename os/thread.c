/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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
 * -
 * Portions Copyright (C) 2000 David J. Hudson <dave@humbug.demon.co.uk>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "copying-gpl.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "copying-liquorice.txt" for details.
 */

/*
 * $Log$
 * Revision 1.2  2003/11/18 22:18:40  drsung
 * thread name with 8 characters fixed.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:54  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.23  2003/05/06 18:56:24  harald
 * Avoid inlining with GCC optimizations
 *
 * Revision 1.22  2003/03/31 14:39:43  harald
 * *** empty log message ***
 *
 * Revision 1.21  2003/02/04 18:15:57  harald
 * Version 3 released
 *
 * Revision 1.20  2002/10/29 15:36:59  harald
 * New function retrieves thread by name
 *
 * Revision 1.19  2002/08/08 17:31:47  harald
 * Imagecraft support by Klaus Ummenhofer
 *
 * Revision 1.18  2002/07/03 16:45:42  harald
 * Using GCC 3.2
 *
 * Revision 1.17  2002/06/26 17:29:44  harald
 * First pre-release with 2.4 stack
 *
 */

#include <string.h>

#ifdef __IMAGECRAFT__
#include <iom128v.h>
#endif
#include <sys/types.h>
#include <sys/heap.h>
#include <sys/atom.h>
#include <sys/timer.h>
#include <sys/thread.h>

#ifdef NUTDEBUG
#include <sys/osdebug.h>
#endif

/*!
 * \addtogroup xgThread
 */
/*@{*/

/*
 * Context switch frame layout.
 *
 * This is the layout of the stack after a thread's
 * context has been switched-out.
 */
typedef struct {
    u_char csf_r31;
    u_char csf_r30;
    u_char csf_r29;
    u_char csf_r28;
    u_char csf_r27;
    u_char csf_r26;
    u_char csf_r25;
    u_char csf_r24;
    u_char csf_r23;
    u_char csf_r22;
    u_char csf_r21;
    u_char csf_r20;
    u_char csf_r19;
    u_char csf_r18;
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
    u_char csf_r1;
    u_char csf_r0;
    u_char csf_pchi;
    u_char csf_pclo;
} SWITCHFRAME;

/*
 * Thread entry frame layout.
 *
 * This is the stack layout being build to enter a new thread.
 */
typedef struct {
    u_char cef_arghi;           // former r25; for ICC r17
    u_char cef_arglo;           // former r24; for ICC r16
#ifdef __IMAGECRAFT__
    u_char cef_yhi;             // SW-Stack
    u_char cef_ylo;
#endif
    u_char cef_rampz;
    u_char cef_sreg;
    u_char cef_r1;
    u_char cef_pchi;
    u_char cef_pclo;
} ENTERFRAME;


/*!
 * \brief Currently running thread.
 *
 * Pointer to the NUTTHREADINFO structure of the currently
 * running thread.
 */
NUTTHREADINFO *volatile runningThread = 0;

/*!
 * \brief List of all created threads.
 *
 * Linked list of NUTTHREADINFO structures of all threads.
 * New threads are put in front. This list contains at
 * least two threads, the main application thread followed
 * by the idle thread.
 */
NUTTHREADINFO *volatile nutThreadList = 0;

/*!
 * \brief List of ready-to-run threads.
 *
 * Priority ordered linked list of NUTTHREADINFO structures
 * of all threads which are ready to run.
 */
NUTTHREADINFO *volatile runQueue = 0;

#ifdef __GNUC__
/*
 * This code is executed when entering a thread.
 */
static void NutThreadEntry(void) __attribute__ ((naked));
static void NutThreadEntry(void)
{
    asm volatile ("pop r25" "\n\t"
                  "pop r24" "\n\t"
                  "pop __tmp_reg__" "\n\t"
                  "out %0, __tmp_reg__" "\n\t"
                  "pop __tmp_reg__" "\n\t"
                  "out %1, __tmp_reg__" "\n\t"
                  "pop __zero_reg__" "\n\t" "reti" "\n\t"::"I" _SFR_IO_ADDR(RAMPZ), "I" _SFR_IO_ADDR(SREG)
        );
}
#else

static void NutThreadEntry(void) __attribute__ ((naked));
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
void NutThreadSwitch(void) __attribute__ ((noinline));
void NutThreadSwitch(void)
{
    /*
     * Save all CPU registers.
     */
    asm volatile ("push r0" "\n\t"
                  "push r1" "\n\t"
                  "push r2" "\n\t"
                  "push r3" "\n\t"
                  "push r4" "\n\t"
                  "push r5" "\n\t"
                  "push r6" "\n\t"
                  "push r7" "\n\t"
                  "push r8" "\n\t"
                  "push r9" "\n\t"
                  "push r10" "\n\t"
                  "push r11" "\n\t"
                  "push r12" "\n\t"
                  "push r13" "\n\t"
                  "push r14" "\n\t"
                  "push r15" "\n\t"
                  "push r16" "\n\t"
                  "push r17" "\n\t"
                  "push r18" "\n\t"
                  "push r19" "\n\t"
                  "push r20" "\n\t"
                  "push r21" "\n\t"
                  "push r22" "\n\t"
                  "push r23" "\n\t"
                  "push r24" "\n\t"
                  "push r25" "\n\t"
                  "push r26" "\n\t"
                  "push r27" "\n\t"
                  "push r28" "\n\t"
                  "push r29" "\n\t"
                  "push r30" "\n\t" "push r31" "\n\t" "in %A0, %1" "\n\t" "in %B0, %2" "\n\t":"=r" (runningThread->td_sp)
                  :"I" _SFR_IO_ADDR(SPL), "I" _SFR_IO_ADDR(SPH)
        );

    /*
     * This defines a global label, which may be called
     * as an entry point into this function.
     */
    asm volatile (".global thread_start\n" "thread_start:\n\t"::);

    /*
     * Reload CPU registers from the thread in front
     * of the run queue.
     */
    runningThread = runQueue;
    runningThread->td_state = TDS_RUNNING;
    asm volatile ("out %1, %A0" "\n\t"
                  "out %2, %B0" "\n\t"
                  "pop r31" "\n\t"
                  "pop r30" "\n\t"
                  "pop r29" "\n\t"
                  "pop r28" "\n\t"
                  "pop r27" "\n\t"
                  "pop r26" "\n\t"
                  "pop r25" "\n\t"
                  "pop r24" "\n\t"
                  "pop r23" "\n\t"
                  "pop r22" "\n\t"
                  "pop r21" "\n\t"
                  "pop r20" "\n\t"
                  "pop r19" "\n\t"
                  "pop r18" "\n\t"
                  "pop r17" "\n\t"
                  "pop r16" "\n\t"
                  "pop r15" "\n\t"
                  "pop r14" "\n\t"
                  "pop r13" "\n\t"
                  "pop r12" "\n\t"
                  "pop r11" "\n\t"
                  "pop r10" "\n\t"
                  "pop r9" "\n\t"
                  "pop r8" "\n\t"
                  "pop r7" "\n\t"
                  "pop r6" "\n\t"
                  "pop r5" "\n\t"
                  "pop r4" "\n\t"
                  "pop r3" "\n\t"
                  "pop r2" "\n\t"
                  "pop r1" "\n\t" "pop r0" "\n\t"::"r" (runningThread->td_sp), "I" _SFR_IO_ADDR(SPL), "I" _SFR_IO_ADDR(SPH)
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

    asm("push r0");
    asm("push r1");
    asm("push r2");
    asm("push r3");
    asm("push r4");
    asm("push r5");
    asm("push r6");
    asm("push r7");
    asm("push r8");
    asm("push r9");
    asm("push r10");
    asm("push r11");
    asm("push r12");
    asm("push r13");
    asm("push r14");
    asm("push r15");
    asm("push r16");
    asm("push r17");
    asm("push r18");
    asm("push r19");
    asm("push r20");
    asm("push r21");
    asm("push r22");
    asm("push r23");
    asm("push r24");
    asm("push r25");
    asm("push r26");
    asm("push r27");
    asm("push r28");
    asm("push r29");
    asm("push r30");
    asm("push r31");
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
    asm("pop r31");
    asm("pop r30");
    asm("pop r29");
    asm("pop r28");
    asm("pop r27");
    asm("pop r26");
    asm("pop r25");
    asm("pop r24");
    asm("pop r23");
    asm("pop r22");
    asm("pop r21");
    asm("pop r20");
    asm("pop r19");
    asm("pop r18");
    asm("pop r17");
    asm("pop r16");
    asm("pop r15");
    asm("pop r14");
    asm("pop r13");
    asm("pop r12");
    asm("pop r11");
    asm("pop r10");
    asm("pop r9");
    asm("pop r8");
    asm("pop r7");
    asm("pop r6");
    asm("pop r5");
    asm("pop r4");
    asm("pop r3");
    asm("pop r2");
    asm("pop r1");
    asm("pop r0");
}
#endif


/*!
 * \brief Add a thread to a prioritiy ordered queue.
 *
 * Insert the thread into a specified queue behind
 * the last thread with lower or equal priority.
 *
 * \note CPU interrupts must have been disabled before 
 *       calling this function.
 *
 * \param td   Pointer to NUTTHREADINFO of the thread to be
 *             inserted in the queue.
 * \param tqpp Pointer to the root of the queue.
 */
void NutThreadAddPriQueue(NUTTHREADINFO * td, NUTTHREADINFO ** tqpp)
{
    NUTTHREADINFO *tqp;
#ifdef NUTDEBUG
    NUTTHREADINFO **dump = tqpp;
#endif

    td->td_queue = tqpp;
    tqp = *tqpp;
    while (tqp && (tqp->td_priority <= td->td_priority)) {
        tqpp = &tqp->td_qnxt;
        tqp = tqp->td_qnxt;
    }
    td->td_qnxt = tqp;
    *tqpp = td;
#ifdef NUTDEBUG
    if (__os_trf)
        NutDumpThreadQueue(__os_trs, *dump);
#endif
}

/*!
 * \brief Remove a thread from a specified queue.
 *
 * CPU interrupts must have been disabled before calling
 * this function.
 *
 * \param td   Pointer to NUTTHREADINFO of the thread to be
 *             removed from the queue.
 * \param tqpp Pointer to the root of the queue.
 */
void NutThreadRemoveQueue(NUTTHREADINFO * td, NUTTHREADINFO * volatile *tqpp)
{
    NUTTHREADINFO *tqp;
#ifdef NUTDEBUG
    NUTTHREADINFO *volatile *dump = tqpp;
#endif

    tqp = *tqpp;
    while (tqp) {
        if (tqp == td) {
            *tqpp = td->td_qnxt;
            td->td_qnxt = 0;
            td->td_queue = 0;
            break;
        }
        tqpp = &tqp->td_qnxt;
        tqp = tqp->td_qnxt;
    }
#ifdef NUTDEBUG
    if (__os_trf)
        NutDumpThreadQueue(__os_trs, *dump);
#endif
}

/*!
 * \brief Make a previously suspended thread ready to run.
 *
 * \param th Handle of the thread to resume.
 *
 * \note CPU interrupts must have been disabled before calling
 *       this function. It is save to call this function from
 *       within an interrupt handler.
 */
void NutThreadResumeAsync(HANDLE th)
{
    if (th && ((NUTTHREADINFO *) th)->td_state == TDS_SLEEP) {
#ifdef NUTDEBUG
        if (__os_trf)
            fprintf(__os_trs, "Add<%04x>", (u_int) th);
#endif
        NutThreadAddPriQueue(th, (NUTTHREADINFO **) & runQueue);
        ((NUTTHREADINFO *) th)->td_state = TDS_READY;
#ifdef NUTDEBUG
        if (__os_trf)
            NutDumpThreadList(__os_trs);
#endif
    }
#ifdef NUTDEBUG
    else if (__os_trf)
        fprintf(__os_trs, "<#A%04X>", (u_int) th);
#endif
}

/*!
 * \brief Resume a previously suspended thread.
 *
 * This routine is called by the system when a
 * sleep timer elapses.
 *
 * \note This routine is running in interrupt context.
 *       Applications typically do not call this 
 *       function. In any case interrupts must have 
 *       been disabled.
 *
 * \param timer Handle of the elapsed timer.
 * \param th    Handle of the thread to wake up.
 */
void NutThreadWake(HANDLE timer, HANDLE th)
{
    ((NUTTHREADINFO *) th)->td_timer = 0;
    NutThreadResumeAsync(th);
}

/*!
 * \brief Give up the CPU.
 *
 * If another thread within the same or higher priority
 * is ready to run, then the current thread is stopped
 * and the other one is started.
 *
 */
void NutThreadYield(void)
{
    NutEnterCritical();

    /*
     * If the current thread is still in front of the
     * run queue, remove it and reinsert it. The idle
     * thread is the last one in the queue and will
     * never be removed.
     */
    if (runningThread == runQueue && runningThread->td_qnxt) {
#ifdef NUTDEBUG
        if (__os_trf)
            fprintf(__os_trs, "Yld<%04x>", (u_int) runningThread);
#endif
        runQueue = runningThread->td_qnxt;
        runningThread->td_qnxt = 0;
        NutThreadAddPriQueue(runningThread, (NUTTHREADINFO **) & runQueue);
#ifdef NUTDEBUG
        if (__os_trf)
            NutDumpThreadList(__os_trs);
        //NutDumpThreadQueue(__os_trs, runQueue);
#endif
    }

    /*
     * If another thread moved in front,
     * switch to it.
     */
    if (runningThread != runQueue) {
        runningThread->td_state = TDS_READY;
#ifdef NUTDEBUG
        if (__os_trf)
            fprintf(__os_trs, "SWY<%04x %04x>", (u_int) runningThread, (u_int) runQueue);
#endif
        NutThreadSwitch();
    }
    NutExitCritical();
}

/*!
 * \brief Set the current thread's priority.
 *
 * The priority of newly created threads is set to 64,
 * but may be changed when the thread starts running.
 *
 * When another thread with a higher or equal priority
 * is ready to run, the current thread will be stopped
 * and control of the CPU is passed to the other thread.
 *
 * The function returns the old priority, which makes it
 * easy to temporarily switch to another priority and
 * later set back the old one.
 *
 * \param level New priority level.
 *
 * \return The old priority of this thread.
 */
u_char NutThreadSetPriority(u_char level)
{
    u_char last = runningThread->td_priority;

    NutEnterCritical();
#ifdef NUTDEBUG
    if (__os_trf)
        fprintf(__os_trs, "Pri%u<%04x>", level, (u_int) runningThread);
#endif
    NutThreadRemoveQueue(runningThread, &runQueue);
    runningThread->td_priority = level;
    NutThreadAddPriQueue(runningThread, (NUTTHREADINFO **) & runQueue);
#ifdef NUTDEBUG
    if (__os_trf)
        NutDumpThreadList(__os_trs);
    //NutDumpThreadQueue(__os_trs, runQueue);
#endif
    if (runningThread == runQueue)
        runningThread->td_state = TDS_RUNNING;
    else {
        runningThread->td_state = TDS_READY;
#ifdef NUTDEBUG
        if (__os_trf)
            fprintf(__os_trs, "SWC<%04x %04x>", (u_int) runningThread, (u_int) runQueue);
#endif
        NutThreadSwitch();
    }
    NutExitCritical();

    return last;
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
HANDLE NutThreadCreate(u_char * name, void (*fn) (void *), void *arg, u_short stackSize)
{
    u_char *threadMem;
    SWITCHFRAME *sf;
    ENTERFRAME *ef;
    NUTTHREADINFO *td;
#ifdef __IMAGECRAFT__
    u_short yreg;
#endif

    const u_short *paddr;
    NutEnterCritical();
    paddr = (const u_short *) fn;       // *KU* fn doesn't contain the entry address
    // of the thread!
    /*
     * Allocate stack and thread info structure in one block.
     */
    if ((threadMem = NutHeapAlloc(stackSize + sizeof(NUTTHREADINFO))) == 0)
        return 0;

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
        fprintf(__os_trs, "Cre<%04x>", (u_int) td);
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
            fprintf(__os_trs, "New<%04x %04x>", (u_int) runningThread, (u_int) runQueue);
#endif
        NutThreadSwitch();
    }
    NutExitCritical();

    return td;
}

HANDLE GetThreadByName(u_char * name)
{
    NUTTHREADINFO *tdp;

    for (tdp = nutThreadList; tdp; tdp = tdp->td_next) {
        if (strcmp(tdp->td_name, name) == 0)
            return tdp;
    }
    return NULL;
}

/*@}*/
