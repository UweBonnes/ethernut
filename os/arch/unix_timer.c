/*
 * Copyright (C) 2000-2004 by ETH Zurich
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
 * THIS SOFTWARE IS PROVIDED BY ETH ZURICH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH ZURICH
 *  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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
 * unix_timer.c - unix emulation of a real-time clock that is used as a timer
 *
 * 2004.04.01 Matthias Ringwald <matthias.ringwald@inf.ethz.ch>
 *
 */

#include <unistd.h>

/* timer thread, generating ms ticks */
static pthread_t timer_thread;

/* counter for ms */
static u_short ms1;


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
    usleep(1000L * ms);
}


/*
 * Timer 0 interrupt handler. Should be called every ms
 */
void NutTimer0Intr(void *);
void NutTimer0Intr(void *arg)
{

    NUTTIMERINFO *tnp;

    /*
     * Increment the tick counter used by Michael Fischer's
     * NutGetTickCount() routine.
     */
    milli_ticks++;

    millis++;
    if (ms1++ >= 999) {
        ms1 = 0;
        seconds++;
    }

    if (nutTimerList) {
        if (nutTimerList->tn_ticks_left) {
            nutTimerList->tn_ticks_left--;
        }

        /*
         * Process all elapsed timers.
         * Bugfix by KU: Avoid crash on empty timer list.
         */
        while ((nutTimerList != 0) && (nutTimerList->tn_ticks_left == 0)) {

            /*
             * Execute the callback.
             */
            if (nutTimerList->tn_callback) {
                (*nutTimerList->tn_callback) (nutTimerList, (void *) nutTimerList->tn_arg);
            }

            /*
             * Remove the timer from the list.
             */
            tnp = nutTimerList;
#ifdef NUTDEBUG
            if (__os_trf)
                fprintf(__os_trs, "RemTmr<%04X>\r\n", tnp);
#endif
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

/*
 * Unix fake timer interrupt handle
 * arg: irq nummer to trigger
 */

void *NutTimerSimulation(void *);
void *NutTimerSimulation(void *arg)
{
    u_char irq_nr = (u_char) (uptr_t) arg;

    // printf("NutTimerSimulation started\n");

    // non-nut thread => block IRQ signals
    pthread_sigmask(SIG_BLOCK, &irq_signal, 0);

    while (1) {
        usleep(1000);           // 100 ms

        // printf("NutTimerSimulation tick\n");
        NutIRQTrigger(irq_nr);
    }
}

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
void NutTimerInit(void);
void NutTimerInit()
{
    u_char timerIrqNr = 0;

    // register irq handler
    NutRegisterIrqHandler(timerIrqNr, NutTimer0Intr, (void *) 0);

    // create rtc timer simulation
    pthread_create(&timer_thread, NULL, NutTimerSimulation, (void *) (uptr_t) timerIrqNr);
}
