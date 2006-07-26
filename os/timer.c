/*
 * Copyright (C) 2001-2006 by egnite Software GmbH. All rights reserved.
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
 * \file os/timer.c
 * \brief System timer support.
 *
 * This kernel module contains the hardware independent system timer routines.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.31  2006/07/26 11:16:12  haraldkipp
 * NutSleep() didn't take the difference between timer creation time and
 * last timer elapsed processing into account. This may resulted in shorter
 * sleep time. Thanks to Andras Albert for this fix.
 *
 * Revision 1.30  2006/06/28 14:42:28  haraldkipp
 * Event and timer handling re-design, again.
 * Time-out timers are now released early.
 * The timer list is now double linked to reduce removal time.
 * Internally timeout condition is now flagged by setting the timer handle
 * to SIGNALED.
 * The documentation had been updated.
 * NUTDEBUG code requires some updates. Temporarly removed.
 * LGPL copyright removed after all the initial sources had been finally
 * replaced by BSDL'ed code.
 * Variable nut_ticks_resume is used in NutTimerProcessElapsed() only. Moved
 * there.
 *
 * Revision 1.29  2006/01/26 15:34:49  going_nuts
 * adapted to new interrupt handling scheme for unix emulation
 * now uses Unix timer and runs without interrupts unless you emulate other hardware
 *
 * Revision 1.28  2005/08/03 14:43:54  freckle
 * made nut_tick public
 *
 * Revision 1.27  2005/07/26 16:01:19  haraldkipp
 * Cygwin added.
 * Platform dependent code is now located in arch/(target)/ostimer.c.
 *
 * Revision 1.26  2005/07/21 15:31:04  freckle
 * rewrote timer handling. timer interrupt handler only updates nut_ticks now
 *
 * Revision 1.25  2005/07/20 09:26:34  haraldkipp
 * Use native heap calls to avoid dependencies
 *
 * Revision 1.24  2005/07/15 14:44:46  freckle
 * corrected NutGetMillis, update comments
 *
 * Revision 1.23  2005/07/12 16:37:02  freckle
 * made NutTimerInsert public
 *
 * Revision 1.22  2005/07/12 16:26:04  freckle
 * extracted timer creation from NutTimerStartTicks into NutTimerCreate
 *
 * Revision 1.21  2005/07/12 15:23:40  olereinhardt
 * Added NULL pointer checks in NutTimerProcessElapsed(void)
 *
 * Revision 1.20  2005/07/12 15:10:43  freckle
 * removed critical section in NutSleep
 *
 * Revision 1.19  2005/07/07 12:43:49  freckle
 * removed stopped timers in NutTimerProcessElapsed
 *
 * Revision 1.18  2005/06/14 19:04:44  freckle
 * Fixed NutTimerStopAsync(): it now removes callback ptr => timer stop
 *
 * Revision 1.17  2005/06/12 16:53:01  haraldkipp
 * Major redesing to reduce interrupt latency.
 * Much better seperation of hardware dependent and independent parts.
 *
 * Revision 1.16  2005/05/27 17:30:27  drsung
 * Platform dependant files were moved to /arch directory.
 *
 * Revision 1.15  2005/02/21 12:37:57  phblum
 * Removed tabs and added semicolons after NUTTRACER macros
 *
 * Revision 1.14  2005/02/16 19:53:27  haraldkipp
 * Ready-to-run queue handling removed from interrupt context.
 *
 * Revision 1.13  2005/02/10 07:06:51  hwmaier
 * Changes to incorporate support for AT90CAN128 CPU
 *
 * Revision 1.12  2005/01/24 22:34:40  freckle
 * Added new tracer by Phlipp Blum <blum@tik.ee.ethz.ch>
 *
 * Revision 1.11  2005/01/19 17:59:43  freckle
 * Improved interrupt performance by reducing some critical section
 *
 * Revision 1.10  2005/01/13 18:51:23  haraldkipp
 * Moved ms62_5 counter to nutinit.c to make sure this is located in internal
 * RAM (AVR platforms). This fixes the wrong baudrate bug for applications
 * occupying all internal RAM.
 *
 * Optimized some timer handling.
 *
 * Revision 1.9  2005/01/02 10:07:10  haraldkipp
 * Replaced platform dependant formats in debug outputs.
 *
 * Revision 1.8  2004/09/08 10:53:25  haraldkipp
 * os/timer.c
 *
 * Revision 1.7  2004/04/07 12:13:58  haraldkipp
 * Matthias Ringwald's *nix emulation added
 *
 * Revision 1.6  2004/03/19 09:05:12  jdubiec
 * Fixed format strings declarations for AVR.
 *
 * Revision 1.5  2004/03/16 16:48:45  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.4  2004/02/01 18:49:48  haraldkipp
 * Added CPU family support
 *
 * Revision 1.3  2003/11/03 16:34:21  haraldkipp
 * New API returns system uptime in miliseconds
 *
 * Revision 1.2  2003/10/13 10:17:11  haraldkipp
 * Seconds counter added
 *
 * Revision 1.1.1.1  2003/05/09 14:41:55  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.24  2003/05/06 18:56:58  harald
 * Allow compilation without clock crystal
 *
 * Revision 1.23  2003/04/21 17:09:19  harald
 * *** empty log message ***
 *
 * Revision 1.22  2003/03/31 14:39:54  harald
 * *** empty log message ***
 *
 * Revision 1.21  2003/02/04 18:15:57  harald
 * Version 3 released
 *
 * Revision 1.20  2003/01/14 16:58:41  harald
 * Tick counter added.
 *
 * Revision 1.19  2002/10/29 15:37:55  harald
 * Some compilers refuse fixed crystal source
 *
 * Revision 1.18  2002/09/15 17:08:07  harald
 * *** empty log message ***
 *
 * Revision 1.17  2002/08/08 17:32:44  harald
 * Imagecraft support by Klaus Ummenhofer
 *
 * Revision 1.16  2002/06/26 17:29:45  harald
 * First pre-release with 2.4 stack
 *
 * \endverbatim
 */

#include <cfg/os.h>
#include <dev/irqreg.h>

#include <sys/types.h>
#include <sys/atom.h>
#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/timer.h>

#ifdef NUTDEBUG
#include <sys/osdebug.h>
#endif

#ifdef NUTTRACER
#include <sys/tracer.h>
#endif

#if defined (__linux__) || defined(__APPLE__) || defined(__CYGWIN__)
#include <sys/time.h>

static struct timeval   timeStart;
#endif

/*!
 * \addtogroup xgTimer
 */
/*@{*/

/*!
 * \brief Double linked list of all system timers.
 */
NUTTIMERINFO *nutTimerList;

/*
 * Last processing time of elapsed timers. 
 */
static u_long nut_ticks_resume; 

/*!
*  \brief System tick counter
 */
volatile u_long nut_ticks;

/*!
 * \brief System timer interrupt handler.
 */
#if !(defined (__linux__) || defined(__APPLE__) || defined(__CYGWIN__))
#ifdef USE_TIMER
SIGNAL( SIG_TIMER ) 
#else
static void NutTimerIntr(void *arg)
#endif
{
    nut_ticks++;
}
#endif


/*!
 * \brief Initialize system timer.
 *
 * This function is automatically called by Nut/OS during system 
 * initialization. It calls the hardware dependent layer to initialze
 * the timer hardware and register a timer interrupt handler.
 */
void NutTimerInit(void)
{
#if defined (__linux__) || defined(__APPLE__) || defined(__CYGWIN__)
    gettimeofday( &timeStart, NULL );
#else
    NutRegisterTimer(NutTimerIntr);
    NutEnableTimerIrq();
#endif
}


/*!
 * \brief Insert a new timer in the global timer list.
 *
 * Applications should not call this function.
 *
 * \param tn Pointer to the timer structure to insert.
 *
 * \todo Make this local function static.
 */
void NutTimerInsert(NUTTIMERINFO * tn)
{
    NUTTIMERINFO *tnp;

    tn->tn_prev = NULL;
    for (tnp = nutTimerList; tnp; tnp = tnp->tn_next) {
        if (tn->tn_ticks_left < tnp->tn_ticks_left) {
            tnp->tn_ticks_left -= tn->tn_ticks_left;
            break;
        }
        tn->tn_ticks_left -= tnp->tn_ticks_left;
        tn->tn_prev = tnp;
    }
    tn->tn_next = tnp;
    if (tn->tn_next) {
        tn->tn_next->tn_prev = tn;
    }
    if (tn->tn_prev) {
        tn->tn_prev->tn_next = tn;
    }
    else {
        nutTimerList = tn;
    }
}

/*!
 * \brief Process elapsed timers.
 *
 * This routine is called during context switch processing.
 * Applications should not use this function.
 */
void NutTimerProcessElapsed(void)
{
    NUTTIMERINFO *tn;
    u_long ticks;
    u_long ticks_new;

    // calculate ticks since last call
    ticks = NutGetTickCount();
    ticks_new = ticks - nut_ticks_resume;
    nut_ticks_resume = ticks;
    
    // process timers
    while (nutTimerList && ticks_new){
        
        tn = nutTimerList;

        // subtract time
        if (ticks_new < tn->tn_ticks_left) {
            tn->tn_ticks_left -= ticks_new;
            ticks_new = 0;
        } else {
            ticks_new -= tn->tn_ticks_left;
            tn->tn_ticks_left = 0;
        }
        
        // elapsed
        if (tn->tn_ticks_left == 0){

            // callback
            if (tn->tn_callback) {
                (*tn->tn_callback) (tn, (void *) tn->tn_arg);
            }
            if ((tn->tn_ticks_left = tn->tn_ticks) == 0) {
                NutTimerStop(tn);
            }
            else {
                // remove from list
                nutTimerList = nutTimerList->tn_next;
                if (nutTimerList) {
                    nutTimerList->tn_prev = 0;
                }
                // re-insert
                NutTimerInsert(tn);
            }
        }
    }
}

/*!
 * \brief Create a new system timer.
 *
 * Applications should not call this function.
 *
 * \param ticks    Specifies the timer interval in system ticks.
 * \param callback Identifies the function to be called on each
 *                 timer interval.
 * \param arg      The argument passed to the callback function.
 * \param flags    If set to TM_ONESHOT, the timer will be stopped
 *                 after the first interval. Set to 0 for periodic
 *                 timers.
 *
 * \return Timer handle if successfull, 0 otherwise. The handle
 *         may be used to release the timer by calling TimerStop.
 *
 * \todo Make this local function static or directly integrate it into
 *       NutTimerStartTicks().
 */
NUTTIMERINFO * NutTimerCreate(u_long ticks, void (*callback) (HANDLE, void *), void *arg, u_char flags)
{
    NUTTIMERINFO *tn;
    
    tn = NutHeapAlloc(sizeof(NUTTIMERINFO));
    if (tn) {
        tn->tn_ticks_left = ticks + NutGetTickCount() - nut_ticks_resume;
        
        /*
         * Periodic timers will reload the tick counter on each timer 
         * intervall.
         */
        if (flags & TM_ONESHOT) {
            tn->tn_ticks = 0;
        } else {
            tn->tn_ticks = ticks;
        }
        
        /* Set callback and callback argument. */
        tn->tn_callback = callback;
        tn->tn_arg = arg;
    }
    return tn;    
}

/*!
 * \brief Start a system timer.
 *
 * The function returns immediately, while the timer runs asynchronously in 
 * the background.
 *
 * The timer counts for a specified number of ticks, then calls the callback 
 * routine with a given argument.
 *
 * Even after the timer elapsed, the callback function is not executed
 * before the currently running thread is ready to give up the CPU. Thus,
 * system timers may not fulfill the required accuracy. For precise or
 * high resolution timing, native timer interrupt routines are a better
 * choice.
 *
 * \param ticks    Specifies the timer interval in system ticks.
 * \param callback Identifies the function to be called on each
 *                 timer interval.
 * \param arg      The argument passed to the callback function.
 * \param flags    If set to TM_ONESHOT, the timer will be stopped
 *                 after the first interval. Set to 0 for periodic
 *                 timers.
 *
 * \return Timer handle if successfull, 0 otherwise. The handle
 *         may be used to stop the timer by calling TimerStop.
 */
HANDLE NutTimerStartTicks(u_long ticks, void (*callback) (HANDLE, void *), void *arg, u_char flags)
{
    NUTTIMERINFO *tn;

    tn = NutTimerCreate( ticks, callback, arg, flags);
    if (tn) {
        /* Add the timer to the list. */
        NutTimerInsert(tn);
    }
    return tn;
}

/*!
 * \brief Start a system timer.
 *
 * The function returns immediately, while the timer runs
 * asynchronously in the background.
 *
 * The timer counts for a specified number of milliseconds,
 * then calls the callback routine with a given argument.
 *
 * Even after the timer elapsed, the callback function is not executed
 * before the currently running thread is ready to give up the CPU. Thus,
 * system timers may not fulfill the required accuracy. For precise or
 * high resolution timing, native timer interrupt routines are a better
 * choice.
 *
 * \param ms       Specifies the timer interval in milliseconds. The
 *                 resolution is limited to the granularity of the system 
 *                 timer.
 * \param callback Identifies the function to be called on each
 *                 timer interval.
 * \param arg      The argument passed to the callback function.
 * \param flags    If set to TM_ONESHOT, the timer will be stopped
 *                 after the first interval. Set to 0 for periodic
 *                 timers.
 *
 * \return Timer handle if successfull, 0 otherwise. The handle
 *         may be used to stop the timer by calling TimerStop.
 */
HANDLE NutTimerStart(u_long ms, void (*callback) (HANDLE, void *), void *arg, u_char flags)
{
        return NutTimerStartTicks(NutTimerMillisToTicks(ms), callback, arg, flags);
}

/*!
 * \brief Temporarily suspends the current thread.
 *
 * Causes the current thread to wait for a specified interval or,
 * if the specified interval is zero, to give up the CPU for
 * another thread with higher or same priority.
 *
 * This function may switch to another application thread, that
 * got the same or a higher priority and is ready to run.
 *
 * \note Threads may sleep longer than the specified number of
 *       milliseconds, depending on the number of threads
 *       with higher or equal priority, which are ready to run.
 *
 * \param ms Milliseconds to sleep. If 0, the current thread will not 
 *           sleep, but may give up the CPU. The resolution is limited 
 *           to the granularity of the system timer.
 *
 * \todo Code size can be reduced by trying to create the timer before
 *       removing the thread from the run queue.
 */
void NutSleep(u_long ms)
{
    if (ms) {

        /* remove running thread from runQueue */
        NutThreadRemoveQueue(runningThread, &runQueue);
        runningThread->td_state = TDS_SLEEP;

        if ((runningThread->td_timer = NutTimerStart(ms, NutThreadWake, runningThread, TM_ONESHOT)) != 0) {
#ifdef NUTTRACER
            TRACE_ADD_ITEM(TRACE_TAG_THREAD_SLEEP,(int)runningThread);
#endif
            NutThreadResume();
        } else
        {
            /* timer creation failed, restore queues */
            runningThread->td_queue = &runQueue;
            runningThread->td_qnxt  = runQueue;
            runningThread->td_state = TDS_RUNNING;
            runQueue = runningThread;
        }
    } else
        NutThreadYield();
}

/*!
 * \brief Stop a specified timer.
 *
 * Only periodic timers need to be stopped. One-shot timers
 * are automatically stopped by the timer management after
 * ther first timer interval. Anyway, long running one-shot
 * timers may be stopped to release the occupied memory.
 *
 * \param handle Identifies the timer to be stopped. This handle must 
 *               have been created by calling NutTimerStart() or
 *               NutTimerStartTicks().
 */
void NutTimerStop(HANDLE handle)
{
    NUTTIMERINFO *tn = (NUTTIMERINFO *)handle;

    if (tn->tn_prev) {
        tn->tn_prev->tn_next = tn->tn_next;
    }
    else {
        nutTimerList = tn->tn_next;
    }
    if (tn->tn_next) {
        tn->tn_next->tn_prev = tn->tn_prev;
        tn->tn_next->tn_ticks_left += tn->tn_ticks_left;
    }
    NutHeapFree(tn);
}

/*!
 * \brief Return the number of system timer ticks.
 *
 * This function returns the number of system ticks since the system was 
 * started.
 *
 * \return Number of ticks.
 */
u_long NutGetTickCount(void)
{
    u_long rc;

#if defined (__linux__) || defined(__APPLE__) || defined(__CYGWIN__)
    struct timeval   timeNow;

    gettimeofday( &timeNow, NULL );
    rc = (timeNow.tv_sec - timeStart.tv_sec) * 1000;
    rc += (timeNow.tv_usec - timeStart.tv_usec) / 1000;
#else
    NutEnterCritical();
    rc = nut_ticks;
    NutExitCritical();
#endif

    return rc;
}

/*!
 * \brief Return the seconds counter value.
 *
 * This function returns the value of a counter, which is incremented
 * every second. During system start, the counter is cleared to zero.
 *
 * \note There is intentionally no provision to modify the seconds counter.
 *       Callers can rely on a continuous update and use this value for
 *       system tick independend timeout calculations. Applications,
 *       which want to use this counter for date and time functions,
 *       should use an offset value.
 *
 * \return Value of the seconds counter.
 */
u_long NutGetSeconds(void)
{
    return NutGetTickCount() / NutGetTickClock();
}

/*!
 * \brief Return the milliseconds counter value.
 *
 * This function returns the value of a counter, which is incremented
 * every system timer tick. During system start, the counter is cleared
 * to zero and will overflow with the 64 bit tick counter (4294967296).
 * With the default 1024 ticks/s this will happen after 7.9 years.
 * The resolution is also given by the system ticks.
 *
 * \note There is intentionally no provision to modify the seconds counter.
 *       Callers can rely on a continuous update and use this value for
 *       system tick independend timeout calculations.
 *       Depending on 
 *
 * \return Value of the seconds counter.
 */
u_long NutGetMillis(void)
{
    // carefully stay within 64 bit values
    u_long ticks   = NutGetTickCount();
    u_long seconds = ticks / NutGetTickClock();
    ticks         -= seconds * NutGetTickClock();
    return seconds * 1000 + (ticks * 1000 ) / NutGetTickClock();
}

/*@}*/
