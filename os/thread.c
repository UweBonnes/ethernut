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
 * Revision 1.13  2005/02/17 14:44:39  phblum
 * Removed volatile declarations of runQueue since it is not touched from interrupt context. Same for nutThreadList, runningThread and killedThreads.
 * As runQueue is not modified by interrupts, runningThread == runQueue always. Therefore removed obsolete comparison in NutThreadYield().
 * Removed unnecessary critical section in NutThreadYield.
 * Put NutThreadSwitch call of NutThreadResume into critical section.
 *
 * Revision 1.12  2005/02/16 19:53:17  haraldkipp
 * Ready-to-run queue handling removed from interrupt context.
 *
 * Revision 1.11  2005/01/24 22:34:36  freckle
 * Added new tracer by Phlipp Blum <blum@tik.ee.ethz.ch>
 *
 * Revision 1.10  2005/01/02 10:07:10  haraldkipp
 * Replaced platform dependant formats in debug outputs.
 *
 * Revision 1.9  2004/08/05 12:13:57  freckle
 * Added unix emulation hook in NutThreadYield to safely process
 * NutPostEventAsync calls occuring in non Nut/OS threads.
 * Rewrote the unix read function again using the new unix NutThreadYield hook
 * to call the NutPostEventAsync function safely (fast & correct).
 * _write(nf, 0, 0) aka fflush is ignored on unix emulation.
 *
 * Revision 1.8  2004/04/07 12:13:58  haraldkipp
 * Matthias Ringwald's *nix emulation added
 *
 * Revision 1.7  2004/03/19 09:05:12  jdubiec
 * Fixed format strings declarations for AVR.
 *
 * Revision 1.6  2004/03/16 16:48:45  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.5  2004/02/01 18:49:48  haraldkipp
 * Added CPU family support
 *
 * Revision 1.4  2003/12/15 19:31:46  haraldkipp
 * Ralph Mason's thread termination and reduced stack frame.
 *
 * Revision 1.3  2003/11/19 12:06:23  drsung
 * CR/LF issue corrected
 *
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

#include <cfg/os.h>

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

#ifdef NUTTRACER
#include <sys/tracer.h>
#endif

/*!
 * \addtogroup xgThread
 */
/*@{*/

#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega103__)
#include "arch/avr_thread.c"
#elif defined(__arm__)
#include "arch/arm_thread.c"
#elif defined(__H8300H__) || defined(__H8300S__)
#include "arch/h8_thread.c"
#elif defined(__m68k__)
#include "arch/m68k_thread.c"
#elif defined(__linux__) || defined(__APPLE__)
#include "arch/unix_thread.c"
#endif

/*!
 * \brief Currently running thread.
 *
 * Pointer to the NUTTHREADINFO structure of the currently
 * running thread.
 */
NUTTHREADINFO * runningThread;

/*!
 * \brief Thread to be killed.
 *
 * Pointer to the NUTTHREADINFO structure of the latest 
 * killed thread.
 */
NUTTHREADINFO * killedThread;

/*!
 * \brief List of all created threads.
 *
 * Linked list of NUTTHREADINFO structures of all threads.
 * New threads are put in front. This list contains at
 * least two threads, the main application thread followed
 * by the idle thread.
 */
NUTTHREADINFO * nutThreadList;

/*!
 * \brief List of threads to resume.
 *
 * Unordered linked list of NUTTHREADINFO structures of all threads which
 * are ready to run but haven't been added to the runQueue yet.
 */
NUTTHREADINFO *volatile readyQueue;

/*!
 * \brief List of ready-to-run threads.
 *
 * Priority ordered linked list of NUTTHREADINFO structures
 * of all threads which are ready to run.
 */
NUTTHREADINFO * runQueue;



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
 * \brief Continue with the highest priority thread, which is ready to run.
 *
 * All threads, which had been woken up in interrupt context, will be
 * inserted into the priority ordered queue of ready-to-run threads.
 * If the currently running thread lost its top position in the queue
 * of ready-to-run threads, then the context will be switched.
 */
void NutThreadResume(void)
{
    NUTTHREADINFO *td;

    /*
     * Process all entries of the readyQueue.
     */
    do {
        /* Protect access to the readyQueue. */
        NutEnterCritical();

        /* Remove the top entry from the woken up threads. */
        if ((td = readyQueue) != 0) {
            readyQueue = td->td_qnxt;
        }

        NutExitCritical();

        /* Add the thread to the runQueue. */
        if (td) {
            td->td_state = TDS_READY;
            NutThreadAddPriQueue(td, (NUTTHREADINFO **) & runQueue);
        }
    } while (td);

    /* Check for context switch. */
    if (runningThread != runQueue) {
#ifdef NUTDEBUG
        if (__os_trf) {
            static prog_char fmt2[] = "SW<%p %p>";
            fprintf_P(__os_trs, fmt2, runningThread, runQueue);
        }
#endif
        if (runningThread->td_state == TDS_RUNNING) {
            runningThread->td_state = TDS_READY;
        }
        NutEnterCritical();
        NutThreadSwitch();
        NutExitCritical();
    }
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
    ((NUTTHREADINFO *) th)->td_qnxt = readyQueue;
    readyQueue = (NUTTHREADINFO *)th;
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

#if defined(__linux__) || defined(__APPLE__)
    NutEnterCritical();
    NutUnixThreadYieldHook();
    NutExitCritical();
#endif

    /*
     * Remove current thread from runQueue and reinsert it.
     * The idle thread is the last one in the queue and will
     * never be removed.
     */
    if (runningThread->td_qnxt) {
#ifdef NUTDEBUG
        if (__os_trf) {
            static prog_char fmt1[] = "Yld<%p>";
            fprintf_P(__os_trs, fmt1, runningThread);
        }
#endif
        runQueue = runningThread->td_qnxt;
        runningThread->td_qnxt = 0;
        NutThreadAddPriQueue(runningThread, (NUTTHREADINFO **) & runQueue);
#ifdef NUTDEBUG
        if (__os_trf)
            NutDumpThreadList(__os_trs);
#endif
    }

    /* Continue with the highest priority thread, which is ready to run. */
#ifdef NUTTRACER
		TRACE_ADD_ITEM(TRACE_TAG_THREAD_YIELD,(int)runningThread)
#endif
    NutThreadResume();
}

/*!
 * \brief Set the current thread's priority.
 *
 * The priority of newly created threads is set to 64,
 * but may be changed when the thread starts running.
 *
 * Changing the priority level to 255 will kill the
 * calling thread.
 *
 * When another thread with a higher or equal priority
 * is ready to run, the current thread will be stopped
 * and control of the CPU is passed to the other thread.
 *
 * The function returns the old priority, which makes it
 * easy to temporarily switch to another priority and
 * later set back the old one.
 *
 * \param level New priority level or 255 to kill the thread.
 *
 * \return The old priority of this thread.
 */
u_char NutThreadSetPriority(u_char level)
{
    u_char last = runningThread->td_priority;

    /* Block interrupts. */
    NutEnterCritical();

#ifdef NUTDEBUG
    if (__os_trf) {
        static prog_char fmt1[] = "Pri%u<%p>";
        fprintf_P(__os_trs, fmt1, level, runningThread);
    }
#endif

    /*
     * Remove the thread from the run queue and re-insert it with a new
     * priority, if this new priority level is below 255. A priotity of
     * 255 will kill the thread.
     */
    NutThreadRemoveQueue(runningThread, &runQueue);
    runningThread->td_priority = level;
    if (level < 255) {
        NutThreadAddPriQueue(runningThread, (NUTTHREADINFO **) & runQueue);
    } else {
        NutThreadKill();
    }

#ifdef NUTDEBUG
    if (__os_trf) {
        NutDumpThreadList(__os_trs);
        NutDumpThreadQueue(__os_trs, runQueue);
    }
#endif

    /*
     * Are we still on top of the queue? If yes, then change our status
     * back to running, otherwise do a context switch.
     */
    if (runningThread == runQueue) {
        runningThread->td_state = TDS_RUNNING;
    } else {
        runningThread->td_state = TDS_READY;
#ifdef NUTDEBUG
        if (__os_trf) {
            static prog_char fmt2[] = "SWC<%p %p>";
            fprintf_P(__os_trs, fmt2, runningThread, runQueue);
        }
#endif
#ifdef NUTTRACER
		TRACE_ADD_ITEM(TRACE_TAG_THREAD_SETPRIO,(int)runningThread)
#endif		

        NutThreadSwitch();
    }
    /* Release interrupt blocking. */
    NutExitCritical();

    return last;
}

/*!
 * \brief End the current thread
 *
 * Terminates the current thread, in due course the memory associated 
 * with the thread will be released back to the OS this is done by the 
 * idle thread.
 */
void NutThreadExit(void)
{
    NutThreadSetPriority(255);
}

/*!
 * \brief Free a thread that was previously killed and release memory 
 *        back to the OS.
 *
 * Called when another thread is killed and by the idle thread.
 *
 * Applications generally do not call this function, however you could 
 * call it to try to reclaim memory.
 */
void NutThreadDestroy(void)
{
    if (killedThread) {
        NutHeapFree(killedThread->td_memory);
        killedThread = 0;
    }
}

/*!
 * \brief Kill the running thread.
 *
 * The thread is moved from the schedule que and
 *
 * Applications generally do not call this function
 */
void NutThreadKill(void)
{

    NUTTHREADINFO *pCur = nutThreadList;
    NUTTHREADINFO **pp = (NUTTHREADINFO **) & nutThreadList;

    /* Free up any unfinished already killed threads. */
    NutThreadDestroy();

    /* Remove from the thread list. */
    while (pCur) {
        if (pCur == runningThread) {
            *pp = pCur->td_next;
            break;
        }

        pp = (NUTTHREADINFO **) & pCur->td_next;
        pCur = pCur->td_next;
    }

    /* Schedule for cleanup. */
    killedThread = runningThread;
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
