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
 * Revision 1.2  2003/10/13 10:17:36  haraldkipp
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

#include <dev/irqreg.h>

#include <sys/types.h>
#include <sys/atom.h>
#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/timer.h>

#ifdef NUTDEBUG
#include <sys/osdebug.h>
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

static volatile u_char ms62_5;
static u_long cpu_clock;
static u_short delay_count;
static u_long NutComputeCpuClock(void);

#endif                          /* !NUT_CPU_FREQ */

static volatile u_long milli_ticks;
static volatile u_long seconds;

/*!
 * \brief Insert a new timer in the global timer list.
 */
static void NutTimerInsert(NUTTIMERINFO * tn)
{
    NUTTIMERINFO *volatile *tnpp;
    NUTTIMERINFO *tnp;

#ifdef NUTDEBUG
    if (__os_trf)
        fprintf(__os_trs, "InsTmr<%04X>\n", (u_int) tn);
#endif
    tnpp = &nutTimerList;
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

/*
 * Timer 0 interrupt handler.
 */
static void NutTimer0Intr(void *arg)
{
    NUTTIMERINFO *tnp;

    /*
     * Increment the tick counter used by Michael Fischer's
     * NutGetTickCount() routine.
     */
    milli_ticks++;

#ifdef NUT_CPU_FREQ

    if (ms1++ >= 999) {
        ms1 = 0;
        seconds++;
    }

#else   /* NUT_CPU_FREQ */

    /*
     * Update RTC. We do a post increment here, because
     * of an ImageCraft bug with volatiles. Thanks to
     * Michael Fischer.
     */
    if (ms62_5++ >= 15) {
        ms62_5 = 0;
        seconds++;
    }

#endif  /* NUT_CPU_FREQ */

    if (nutTimerList) {
        if (nutTimerList->tn_ticks_left)
            nutTimerList->tn_ticks_left--;

        /*
         * Process all elapsed timers.
         * Bugfix by KU: Avoid crash on empty timer list.
         */
        while ((nutTimerList != 0) && (nutTimerList->tn_ticks_left == 0)) {
            /*
             * Execute the callback.
             */
            if (nutTimerList->tn_callback)
                (*nutTimerList->tn_callback) (nutTimerList,
                                              (void *) nutTimerList->
                                              tn_arg);

            /*
             * Remove the timer from the list.
             */
            tnp = nutTimerList;
            //fprintf(__os_trs, "RemTmr<%04X>\r\n", tnp);
            nutTimerList = nutTimerList->tn_next;

            /*
             * Periodic timers are re-inserted.
             */
            if ((tnp->tn_ticks_left = tnp->tn_ticks) != 0)
                NutTimerInsert(tnp);

            /*
             * We can't touch the heap while running in
             * interrupt context. Oneshot timers are added 
             * to a pool of available timers.
             */
            else {
                tnp->tn_next = nutTimerPool;
                nutTimerPool = tnp;
                //NutKDumpTimerList();
            }
        }
    }
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
HANDLE NutTimerStart(u_long ms, void (*callback) (HANDLE, void *),
                     void *arg, u_char flags)
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
        /*
         * Calculate the number of system ticks.
         */
#ifdef NUT_CPU_FREQ
        tn->tn_ticks_left = ms;
#else                           /* !NUT_CPU_FREQ */
        tn->tn_ticks_left = (ms / 125) * 2 + ((ms % 125) > 62);
#endif                          /* !NUT_CPU_FREQ */

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
    if (ms) {
        NutEnterCritical();
        if ((runningThread->td_timer =
             NutTimerStart(ms, NutThreadWake, runningThread,
                           TM_ONESHOT)) != 0) {
#ifdef NUTDEBUG
            if (__os_trf)
                fprintf(__os_trs, "Rem<%04x>", (u_int) runningThread);
#endif
            NutThreadRemoveQueue(runningThread, &runQueue);
            runningThread->td_state = TDS_SLEEP;
#ifdef NUTDEBUG
            if (__os_trf) {
                NutDumpThreadList(__os_trs);
                //NutDumpThreadQueue(__os_trs, runQueue);
                fprintf(__os_trs, "SWS<%04x %04x>", (u_int) runningThread,
                        (u_int) runQueue);
            }
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
    if(__os_trf)
        fprintf(__os_trs, "StpTmr<%04X>\r\n", (int)handle);
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

    NutEnterCritical();
    NutTimerStopAsync(handle);

    /*
     * Release the timer pool.
     */
    tnp = nutTimerPool;
    while ((tnp = nutTimerPool) != 0) {
        nutTimerPool = nutTimerPool->tn_next;
        NutHeapFree(tnp);
    }
    NutExitCritical();
}

/*!
 * \brief Loop for a specified number of milliseconds.
 *
 * This call will not release the CPU and will
 * not switch to another thread. However, because
 * of absent thread switching, this delay time is
 * very exact.
 *
 * Use NutSleep() to avoid blocking the CPU, if no
 * exact timing is needed.
 *
 * \param ms Delay time in milliseconds, maximum is 255.
 */
void NutDelay(u_char ms)
{
#ifdef __GNUC__
    u_short cnt;
    asm volatile ("\n"
                  "L_dl1%=:\n\t"
                  "mov %A0, %A2\n\t"
                  "mov %B0, %B2\n"
                  "L_dl2%=:\n\t"
                  "sbiw %A0, 1\n\t"
                  "brne L_dl2%=\n\t"
                  "dec %1\n\t" "brne L_dl1%=\n\t":"=&w" (cnt)
                  :"r"(ms), "r"((u_short) (delay_count))
        );
#else
    u_short delay_cnt = 2400;   //*KU* for 14.745600 MHz Clock
    u_short delay_cnt_buffer;

    while (ms--) {
        delay_cnt_buffer = delay_cnt;
        while (delay_cnt_buffer--);
    }
#endif
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

#ifndef NUT_CPU_FREQ

/*!
 * \brief Compute CPU clock in Hertz.
 *
 * This function determines the CPU clock by running
 * a counter loop between two timer interrupts.
 *
 * \return CPU clock in Hertz.
 *
 */
static u_long NutComputeCpuClock(void)
{
    /*
     * Disable timer 0 interrupts.
     */
    cbi(TIMSK, TOIE0);
    cbi(TIMSK, OCIE0);

    /*
     * Select oscillator 1 (32.768 kHz).
     */
    sbi(ASSR, AS0);

    /*
     * Reset counter register and set
     * prescaler to 8.
     */
    outp(0, TCNT0);
    outp(2, TCCR0);

    /*
     * Wait for update busy clear and
     * enable timer overflow interrupt.
     */
    NutRegisterIrqHandler(&sig_OVERFLOW0, NutTimer0Intr, 0);
    while (inp(ASSR) & (TCN0UB | TCR0UB))
        continue;
    sbi(TIMSK, TOIE0);

    /*
     * Atmel's instruction set manual was wrong.
     * LDS takes two cycles only, not three.
     * Thanks to Les Grant.
     */
#ifdef __IMAGECRAFT__
    cpu_clock = 14745600UL / 160UL;
#else
    asm volatile ("ldi r24,lo8(16)" "\n\t"
                  "eor r25,r25" "\n\t"
                  "eor r26,r26" "\n"
                  "L_w2%=:" "\n\t"
                  "lds r27, ms62_5" "\n\t"
                  "cpi r27, lo8(2)" "\n\t"
                  "brlo L_w2%=" "\n"
                  "L_w4%=:" "\n\t"
                  "nop" "\n\t"
                  "nop" "\n\t"
                  "adiw r24,1" "\n\t"
                  "adc r26,__zero_reg__" "\n\t"
                  "lds r27, ms62_5" "\n\t"
                  "cpi r27, lo8(3)" "\n\t"
                  "brlo L_w4%=" "\n\t"
                  "sts cpu_clock,r24" "\n\t"
                  "sts (cpu_clock)+1,r25" "\n\t"
                  "sts (cpu_clock)+2,r26" "\n\t":::"r24", "r25", "r26",
                  "r27", "memory");
#endif
    cbi(TIMSK, TOIE0);

    cpu_clock *= 160UL;
    delay_count = cpu_clock / 4000;

    return cpu_clock;
}

#endif

/*!
 * \brief Initialize system timer.
 *
 * This function is automatically called by Nut/OS
 * during system initialization.
 *
 * Nut/OS uses on-chip timer 0 for its timer services.
 * Applications should not modify any registers of this
 * timer, but make use of the Nut/OS timer API. Timer 1
 * and timer 2 are available to applications.
 */
void NutTimerInit(void)
{
#ifdef NUT_CPU_FREQ
    /*
     * - Program prescaler to output PCK0/32 and clear Timer0 on compare match.
     * - Clear timer counter register to get the first tick right.
     * - Write (CPU frequency / (prescaler * 1KHz)) in the output compare.
     *   register, so we'll get a compare match interrupt every millisecond.
     */
#ifdef __AVR_ATmega128__
    outp(BV(CS0) | BV(CS2) | BV(WGM1), TCCR0);
#else
    outp(BV(CS00) | BV(CS02) | BV(CTC0), TCCR0);
#endif
    outp(0, TCNT0);
    outp(NUT_CPU_FREQ / (128L * 1000), OCR0);
    NutRegisterIrqHandler(&sig_OUTPUT_COMPARE0, NutTimer0Intr, 0);
    sbi(TIMSK, OCIE0);
#else
    NutComputeCpuClock();
    sbi(TIMSK, TOIE0);
#endif
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

/*@}*/
