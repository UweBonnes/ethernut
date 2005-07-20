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
 * Revision 1.21  2005/07/20 09:19:45  haraldkipp
 * Use native heap calls to avoid dependencies
 *
 * Revision 1.20  2005/07/13 15:25:50  freckle
 * Rewrote NutEventWait to get short critical sections
 *
 * Revision 1.19  2005/07/12 18:44:05  freckle
 * Removed unnecessary critical sections in NutEventPost, NutEventWait
 *
 * Revision 1.18  2005/07/12 18:04:12  freckle
 * Reverted NutEventWait back to 1.15 but kept critical section in
 * NutEventTimeout + changed CS in NutEventWaitNext
 *
 * Revision 1.15  2005/06/10 12:59:26  freckle
 * corrected NuEventBroadcastAsync documentation.
 * NutEventBroadcast uses NutEventPostAsync which disables IRQsCVS:
 * ----------------------------------------------------------------------
 *
 * Revision 1.14  2005/02/21 12:37:57  phblum
 * Removed tabs and added semicolons after NUTTRACER macros
 *
 * Revision 1.13  2005/02/17 14:45:10  phblum
 * In NutEventWait(), replaced NutThreadRemoveQueue by direct removal of first thread in queue.
 *
 * Revision 1.12  2005/02/16 19:53:17  haraldkipp
 * Ready-to-run queue handling removed from interrupt context.
 *
 * Revision 1.11  2005/01/24 22:34:35  freckle
 * Added new tracer by Phlipp Blum <blum@tik.ee.ethz.ch>
 *
 * Revision 1.10  2005/01/24 21:11:21  freckle
 * renamed NutEventPostFromIRQ into NutEventPostFromIrq
 *
 * Revision 1.9  2005/01/21 16:49:44  freckle
 * Seperated calls to NutEventPostAsync between Threads and IRQs
 *
 * Revision 1.8  2005/01/19 17:59:45  freckle
 * Improved interrupt performance by reducing some critical section
 *
 * Revision 1.7  2005/01/02 10:07:10  haraldkipp
 * Replaced platform dependant formats in debug outputs.
 *
 * Revision 1.6  2004/07/20 08:33:28  freckle
 * Fixed NutPostEvent to give up CPU if there is another thread ready with
 * same priority to match the documentation
 * Also removed 'SWP..' NBUTDEBUG output, as switching is done calling NutThreadYield()
 *
 * Revision 1.5  2004/04/07 12:13:58  haraldkipp
 * Matthias Ringwald's *nix emulation added
 *
 * Revision 1.4  2004/03/19 09:05:12  jdubiec
 * Fixed format strings declarations for AVR.
 *
 * Revision 1.3  2004/03/16 16:48:45  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.2  2003/07/20 18:27:44  haraldkipp
 * Explain how to disable timeout.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:49  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.20  2003/04/21 17:07:41  harald
 * Comments added
 *
 * Revision 1.19  2003/03/31 14:46:41  harald
 * Broadcasts clear signaled queues
 *
 * Revision 1.18  2003/02/04 18:15:56  harald
 * Version 3 released
 *
 * Revision 1.17  2003/01/14 16:56:39  harald
 * Racing condition on signaled events fixed.
 * All posting routines return the number of woken up threads.
 *
 * Revision 1.16  2002/06/26 17:29:44  harald
 * First pre-release with 2.4 stack
 *
 */

#include <cfg/os.h>

#include <compiler.h>
#include <sys/atom.h>
#include <sys/heap.h>
#include <sys/timer.h>
#include <sys/thread.h>
#include <sys/event.h>

#ifdef NUTDEBUG
#include <sys/osdebug.h>
#include <stdio.h>
#endif

#ifdef NUTTRACER
#include <sys/tracer.h>
#endif

/*!
 * \addtogroup xgEvent
 */
/*@{*/


/*!
 * \brief Timer callback in case of event timeout.
 *
 * \param timer Handle of the elapsed timeout timer.
 * \param arg   Handle of an event queue.
 *
 * \note This routine is running in interrupt context.
 */
void NutEventTimeout(HANDLE timer, void *arg)
{
    NUTTHREADINFO *tqp;
    NUTTHREADINFO *volatile *tqpp = arg;

    /*
     * A signaled queue is an empty queue. So our
     * thread already left this queue.
     */
    if ((tqp = *tqpp) != SIGNALED) {

        /*
         * Walk down the linked list and identify
         * the thread by the timer id it is waiting
         * for.
         */
        while (tqp) {
            if (tqp->td_timer == timer) {
                /* Found the thread. Remove it from the event queue and
                   add it to the queue of threads, which are waiting to
                   become ready at the next context switch. */
                *tqpp = tqp->td_qnxt;
                /* protected access to readyList */
                NutEnterCritical();
                tqp->td_qnxt = readyQueue;
                readyQueue = tqp;
                NutExitCritical();
                
                /* Clear the timer entry in the thread's info structure.
                   This will tell the waiting thread, that it has been
                   woken up by a timeout. */
                tqp->td_timer = 0;
                break;
            }
            tqpp = &tqp->td_qnxt;
            tqp = tqp->td_qnxt;
        }
    }
}

/*!
 * \brief Wait for an event in a specified queue.
 *
 * Give up the CPU until another thread posts an event to this queue 
 * or until a time-out occurs, whichever comes first.
 *
 * If previously an event had been posted to this queue without any 
 * thread waiting, then the thread will not wait for a new event,
 * but may still pass CPU control, if another thread with equal or
 * higher priority is ready to run.
 *
 * \param qhp Identifies the queue to wait on.
 * \param ms  Maximum wait time in milliseconds. To disable timeout,
 *            set this parameter to NUT_WAIT_INFINITE.
 *
 * \return 0 if event received, -1 on timeout.
 *
 * \note Timeout is limited to the granularity of the system timer.
 */
int NutEventWait(volatile HANDLE * qhp, u_long ms)
{
    u_long ticks;
    NUTTIMERINFO *tn;
    
    /*
     * If a timeout value had been given, create
     * a oneshot timer.
     */
    if (ms) {
        ticks = NutTimerMillisToTicks(ms);
        tn = NutTimerCreate(ticks, NutEventTimeout, (void *) qhp, TM_ONESHOT);
    } else
        tn = 0;

    NutEnterCritical();

    /*
     * Check for posts on a previously empty queue. 
     */
    if (*qhp == SIGNALED) {
        /*
         * Even if already signaled, switch to any other thread, which 
         * is ready to run and has the same or higher priority.
         */
        *qhp = 0;
        NutJumpOutCritical();
        
        /* but free previously allocated timer first */
        if (tn)
            NutHeapFree(tn);

        NutThreadYield();
        return 0;
    }

    /*
     * Remove the current thread from the list
     * of running threads and add it to the
     * specified queue.
     */

    // NutThreadRemoveQueue(runningThread, &runQueue); basically would do the following:
    runQueue = runQueue->td_qnxt;
    runningThread->td_qnxt = 0;
    runningThread->td_queue = 0;

    NutThreadAddPriQueue(runningThread, (NUTTHREADINFO **) qhp);

    NutExitCritical();

    /* update our thread's state (sleeping + timer) */
    runningThread->td_state = TDS_SLEEP;
    runningThread->td_timer = tn;
    if (tn)
        NutTimerInsert(tn);

#ifdef NUTDEBUG
    if (__os_trf) {
        static prog_char fmt1[] = "Rem<%p>";
        fprintf_P(__os_trs, fmt1, runningThread);
        NutDumpThreadList(__os_trs);
#endif
    
    /*
     * Switch to the next thread, which is ready
     * to run.
     */
#ifdef NUTDEBUG
    if (__os_trf) {
        static prog_char fmt2[] = "SWW<%p %p>";
        fprintf_P(__os_trs, fmt2, runningThread, runQueue);
    }
#endif
#ifdef NUTTRACER
    TRACE_ADD_ITEM(TRACE_TAG_THREAD_WAIT,(int)runningThread);
#endif

    NutThreadResume();

    /*
     * If our timer handle is still set, we were
     * woken up by an event. If the handle is
     * cleared and a timeout value had been
     * specified, then we know that a timeout
     * occured.
     */
    if (runningThread->td_timer)
        runningThread->td_timer = 0;
    else if (ms) {
        return -1;
    }
    return 0;
}

/*!
 * \brief Wait for a new event in a specified queue.
 *
 * Give up the CPU until another thread posts an event to this
 * queue or until a time-out occurs, whichever comes first.
 *
 * This call is similar to NutEventWait(), but will ignore the
 * signaled state of the queue. This way, previously posted events to 
 * an empty queue are not considered.
 *
 * \param qhp Identifies the queue to wait on.
 * \param ms  Maximum wait time in milliseconds.
 *
 * \return 0 if event received, -1 on timeout.
 *
 * \note Timeout is limited to the granularity of the system timer.
 */
int NutEventWaitNext(volatile HANDLE * qhp, u_long ms)
{
    NutEnterCritical();

    /*
     * Check for posts on a previously empty queue. 
     */
    if (*qhp == SIGNALED)
        *qhp = 0;

    NutExitCritical();

    return NutEventWait(qhp, ms);
}

/*!
 * \brief Asynchronously post an event to a specified queue.
 *
 * Wake up the thread with the highest priority waiting on the 
 * specified queue. But even if the priority of the woken thread is 
 * higher than the current thread's priority, the current one 
 * continues running.
 *
 * If no thread is waiting, then the queue will be set to the signaled
 * state. 
 *
 * \param qhp Identifies the queue an event is posted to.
 *
 * \return The number of threads woken up, either 0 or 1.
 *
 */
int NutEventPostAsync(HANDLE volatile *qhp)
{
    int rc = 0;

    NutEnterCritical();
    
    /*
     * so far, the code is identical to NutEventPostIrq
     */ 

    rc = NutEventPostFromIrq(qhp);

    NutExitCritical();

    return rc;
}

/*!
* \brief Asynchronously post an event to a specified queue from IRQ.
 *
 * Wake up the thread with the highest priority waiting on the 
 * specified queue. This function is explicitly provided for IRQ
 * handlers to wakeup waiting user threads
 *
 * \note It is save to call this function from within an interrupt
 *       handler.
 *
 * \param qhp Identifies the queue an event is posted to.
 *
 * \return The number of threads woken up, either 0 or 1.
 *
 */
int NutEventPostFromIrq(HANDLE volatile *qhp)
{
    NUTTHREADINFO *td;

    /* Ignore signaled queues. */
    if (*qhp != SIGNALED) {

        /* A thread is waiting. */
        if ((td = *qhp) != 0) {
            /* Remove the thread from the wait queue. */
            *qhp = td->td_qnxt;
            /* Stop any running timeout timer. */
            if (td->td_timer) {
                NutTimerStopAsync(td->td_timer);
            }
            /* Add the thread to the ready queue. */
            td->td_qnxt = readyQueue;
            readyQueue = td;

            return 1;
        }

        /* No thread is waiting. Mark the queue signaled. */
        else {
            *qhp = SIGNALED;
        }
    }
    return 0;
}

/*!
 * \brief Post an event to a specified queue.
 *
 * Wake up the thread with the highest priority waiting on this queue. 
 * If the priority of the waiting thread is higher or equal than the 
 * current thread's priority, then the current thread is stopped and 
 * CPU control is passed to the waiting thread.
 *
 * If no thread is waiting, the queue will be set to the signaled
 * state. 
 *
 * \param qhp Identifies the queue an event is posted to.
 *
 * \return The number of threads woken up, either 0 or 1.
 *
 */
int NutEventPost(HANDLE * qhp)
{
    int rc;

    rc = NutEventPostAsync(qhp);

    /*
     * If any thread with higher or equal priority is
     * ready to run, switch the context.
     */
    NutThreadYield();

    return rc;
}

/*!
 * \brief Asynchronously broadcast an event to a specified queue.
 *
 * Wake up all threads waiting on this queue. But even if the
 * priority of any woken thread is higher than the current thread's
 * priority, the current one continues running.
 *
 * In opposite to NutEventPostAsync(), the queue will be cleared in 
 * any case, even if it is in signaled state. Applications may use this
 * call to make sure, that a queue is cleared before initiating some 
 * event triggering action.
 *
 * \note It is save to call this function from within an interrupt
 *       handler. 
 *
 * \param qhp Identifies the queue an event is broadcasted to.
 *
 * \return The number of threads woken up.
 *
 */
int NutEventBroadcastAsync(HANDLE * qhp)
{
    int rc = 0;

    if (*qhp == SIGNALED)
        *qhp = 0;
    else
        while (*qhp)
            rc += NutEventPostAsync(qhp);

    return rc;
}

/*!
 * \brief Broadcast an event to a specified queue.
 *
 * Wake up all threads waiting on this queue. If the priority of any 
 * waiting thread is higher or equal than the current thread's 
 * priority, then the current thread is stopped and CPU control is 
 * passed to the woken up thread with the highest priority.
 *
 * In opposite to NutEventPost(), the queue will be cleared in any
 * case, even if it is in signaled state. Applications may use this
 * call to make sure, that a queue is cleared before initiating
 * some event triggering action.
 *
 * \param qhp Identifies the queue an event is broadcasted to.
 *
 * \return The number of threads woken up.
 *
 */
int NutEventBroadcast(HANDLE * qhp)
{
    int rc = 0;

    if (*qhp == SIGNALED)
        *qhp = 0;
    else
        while (*qhp)
            rc += NutEventPost(qhp);

    return rc;
}

/*@}*/
