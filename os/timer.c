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

/*!
 * \addtogroup xgTimer
 */
/*@{*/

/*!
 * \brief Linked list of all system timers.
 */
NUTTIMERINFO *volatile nutTimerList = 0;

/*!
 * This pool is used to collect released memory
 * from elapsed timers. It's required because
 * we can't free memory in interrupt context.
 */
NUTTIMERINFO *volatile nutTimerPool = 0;

/*
 * Bernardo Innocenti and Marc Wetzel both asked
 * for a high resolution timer. I can't really
 * follow that request, because timer 1 and 2
 * are still avaible for the application. Beside
 * that the 32 kHz timer offers some advantages
 * with CPU wakeup and automatic detection of
 * crystal frequency. Anyway I added Bernardo's
 * diff. It doesn't differ much from Marc's code,
 * but was based on a later version of Nut/OS.
 * I modified it a bit: To enable 1 millisecond
 * resolution you have to define the CPU frequency
 * in Makedefs:
 * DEFS = -DNUT_CPU_FREQ=14745600
*/

#ifdef NUT_CPU_FREQ

#define cpu_clock   NUT_CPU_FREQ
#define delay_count (NUT_CPU_FREQ/4000)

static volatile u_short ms1;

#else                           /* !NUT_CPU_FREQ */

static u_long cpu_clock;

#if !defined(__linux__) && !defined(__APPLE__)

extern volatile u_char ms62_5;
static u_short delay_count;
static u_long NutComputeCpuClock(void);

#endif

#endif                          /* !NUT_CPU_FREQ */

static volatile u_long milli_ticks;
static volatile u_long millis;
static volatile u_long seconds;

static void NutTimerInsert(NUTTIMERINFO * tn);

#if defined(__AVR__)
#include "arch/avr_timer.c"
#elif defined(__arm__)
#include "arch/arm_timer.c"
#elif defined(__H8300H__) || defined(__H8300S__)
#include "arch/h8_timer.c"
#elif defined(__m68k__)
#include "arch/m68k_timer.c"
#elif defined(__linux__) || defined(__APPLE__)
#include "arch/unix_timer.c"
#endif

/*!
 * \brief Insert a new timer in the global timer list.
 */
static void NutTimerInsert(NUTTIMERINFO * tn)
{
    NUTTIMERINFO **tnpp;
    NUTTIMERINFO *tnp;

#ifdef NUTDEBUG
    if (__os_trf) {
        static prog_char fmt[] = "InsTmr<%p>\n";
        fprintf_P(__os_trs, fmt, tn);
    }
#endif
    tnpp = (NUTTIMERINFO **)&nutTimerList;
    tnp = nutTimerList;
    while (tnp) {
        if (tn->tn_ticks_left < tnp->tn_ticks_left) {
            tnp->tn_ticks_left -= tn->tn_ticks_left;
            break;
        }
        tn->tn_ticks_left -= tnp->tn_ticks_left;
        tnpp = &tnp->tn_next;
        tnp = tnp->tn_next;
    }
    tn->tn_next = tnp;
    *tnpp = tn;
#ifdef NUTDEBUG
    if (__os_trf)
        NutDumpTimerList(__os_trs);
#endif
}


/*!
 * \brief Create an asynchronous timer.
 *
 * The function returns immediately, while the timer runs
 * asynchronously in the background.
 *
 * The timer counts for a specified number of milliseconds,
 * then calls the callback routine with a given argument.
 *
 * The callback function is executed in interrupt context
 * at a very high priority. It can call only a limited set
 * of functions and must return as soon as possible.
 *
 * \param ms       Specifies the timer interval in milliseconds.
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

u_long NutTimerMillisToTicks(u_long ms)
{
    /*
     * Calculate the number of system ticks.
     */

#if defined(NUT_CPU_FREQ) || defined(__linux__) || defined(__APPLE__)
    return ms;
#else
	return (ms / 125) * 2 + ((ms % 125) > 62);
#endif
}

HANDLE NutTimerStart(u_long ms, void (*callback) (HANDLE, void *), void *arg, u_char flags)
{
	return NutTimerStartTicks(NutTimerMillisToTicks(ms), callback, arg, flags);
}

HANDLE NutTimerStartTicks(u_long ticks, void (*callback) (HANDLE, void *), void *arg, u_char flags)
{
    NUTTIMERINFO *tn;

    NutEnterCritical();

    /*
     * If any timer info structure is available in
     * the pool of elapsed timers, take it. Otherwise
     * allocate memory from heap.
     */
    if ((tn = nutTimerPool) != 0)
        nutTimerPool = nutTimerPool->tn_next;
    else {
        tn = NutHeapAlloc(sizeof(NUTTIMERINFO));
    }

    if (tn) {
        tn->tn_ticks_left = ticks;

        /*
         * Periodic timers will reload the tick counter
         * on each timer intervall.
         */
        if (flags & TM_ONESHOT)
            tn->tn_ticks = 0;
        else
            tn->tn_ticks = tn->tn_ticks_left;

        /*
         * Set callback and callback argument.
         */
        tn->tn_callback = callback;
        tn->tn_arg = arg;

        /*
         * Add the timer to the list.
         */
        NutTimerInsert(tn);
    }
    NutExitCritical();
    return tn;
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
 *       If you need exact timing, use NutDelay().
 *
 * \bug The system may freeze if a sleep time greater than 0 ms
 *      but lower than 63 ms is specified.
 *
 * \param ms Milliseconds to sleep. Granularity is 62.5 ms.
 *           If 0, the current thread will not sleep, but
 *           may give up the CPU.
 */
void NutSleep(u_long ms)
{
    u_long ticks;
    if (ms) {
        ticks = NutTimerMillisToTicks(ms);
        NutEnterCritical();
        if ((runningThread->td_timer = NutTimerStartTicks(ticks, NutThreadWake, runningThread, TM_ONESHOT)) != 0) {
#ifdef NUTDEBUG
            if (__os_trf) {
                static prog_char fmt1[] = "Rem<%p>\n";
                fprintf_P(__os_trs, fmt1, runningThread);
            }
#endif
            NutThreadRemoveQueue(runningThread, &runQueue);
            runningThread->td_state = TDS_SLEEP;
#ifdef NUTDEBUG
            if (__os_trf) {
                static prog_char fmt2[] = "SWS<%p %p>\n";
                NutDumpThreadList(__os_trs);
                //NutDumpThreadQueue(__os_trs, runQueue);
                fprintf_P(__os_trs, fmt2, runningThread, runQueue);
            }
#endif
#ifdef NUTTRACER
			TRACE_ADD_ITEM(TRACE_TAG_THREAD_SLEEP,(int)runningThread)
#endif
            NutThreadSwitch();
        }
        NutExitCritical();
    } else
        NutThreadYield();
}

/*!
 * \brief Asynchronously stop a specified timer.
 *
 * Stops one-shot and periodic timers.
 *
 * \note It is save to call this function from within an interrupt
 *       handler. The memory occupied by the timer is not released,
 *       but added to a pool and will be re-used by the next timer
 *       being created. In any case interrupts should be disabled
 *       when calling this function.
 *
 * \param handle Identifies the timer to be stopped. This handle
 *               must have been created by calling NutTimerStart().
 *
 */
void NutTimerStopAsync(HANDLE handle)
{
    NUTTIMERINFO *tnp;
    NUTTIMERINFO *volatile *tnpp;

#ifdef NUTDEBUG
    if (__os_trf) {
        static prog_char fmt[] = "StpTmr<%p>\r\n";
        fprintf_P(__os_trs, fmt, (uptr_t) handle);
    }
#endif
    tnpp = &nutTimerList;
    tnp = nutTimerList;
    while (tnp) {
        if (tnp == handle) {
            /*
             * We found the timer to stop. Remove it from
             * the list and add the number of ticks left
             * to the next timer in the list.
             *
             * Bugfix: Thanks to Bernardo, who discovered,
             * that we need an extra statement if just one
             * timer is left.
             */
            if ((*tnpp = tnp->tn_next) != 0)
                (*tnpp)->tn_ticks_left += tnp->tn_ticks_left;

            /*
             * This function may run in interrupt context,
             * which doesn't allow us to use the heap. We
             * put add the timer to a pool.
             */
            tnp->tn_next = nutTimerPool;
            nutTimerPool = tnp;
            break;
        }
        tnpp = &tnp->tn_next;
        tnp = tnp->tn_next;
    }
#ifdef NUTDEBUG
    if (__os_trf)
        NutDumpTimerList(__os_trs);
#endif
}

/*!
 * \brief Stop a specified timer.
 *
 * Only periodic timers need to be stopped. One-shot timers
 * are automatically stopped by the timer management after
 * ther first timer interval. Anyway, long running one-shot
 * timers may be stopped to release the occupied memory.
 *
 * \param handle Identifies the timer to be stopped. This handle
 *               must have been created by calling NutTimerStart().
 */
void NutTimerStop(HANDLE handle)
{
    NUTTIMERINFO *tnp;
    NUTTIMERINFO *tnp_nxt;

    NutEnterCritical();
    NutTimerStopAsync(handle);
    tnp_nxt = nutTimerPool;
    nutTimerPool = 0;
    NutExitCritical();

    /* Release the timer pool. */
    while ((tnp = tnp_nxt) != 0) {
        tnp_nxt = tnp->tn_next;
        NutHeapFree(tnp);
    }
}

/*!
 * \brief Return the CPU clock in Hertz.
 *
 * \return CPU clock frequency in Hertz.
 */
u_long NutGetCpuClock(void)
{
    return cpu_clock;
}

/*!
 * \brief Return the number of timer ticks.
 *
 * This function returns the TickCount since the system was started. It
 * is limited to the resolution of the system timer.
 *
 * \return Number of ticks.
 */
u_long NutGetTickCount(void)
{
    u_long rc;

    NutEnterCritical();
    rc = milli_ticks;
    NutExitCritical();

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
    u_long rc;

    NutEnterCritical();
    rc = seconds;
    NutExitCritical();

    return rc;
}

/*!
 * \brief Return the milliseconds counter value.
 *
 * This function returns the value of a counter, which is incremented
 * every system timer tick. During system start, the counter is cleared
 * to zero and will overflow roughly after 49.7 days. The resolution
 * is depends on system ticks.
 *
 * \note There is intentionally no provision to modify the seconds counter.
 *       Callers can rely on a continuous update and use this value for
 *       system tick independend timeout calculations.
 *
 * \return Value of the seconds counter.
 */
u_long NutGetMillis(void)
{
    u_long rc;

    NutEnterCritical();
    rc = millis;
    NutExitCritical();

    return rc;
}

/*@}*/
