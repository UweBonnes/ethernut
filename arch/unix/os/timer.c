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

#include <cfg/os.h>
#include <unistd.h>

#ifndef NUT_CPU_FREQ
#define NUT_CPU_FREQ    1000000UL
#endif


/* timer thread, generating ms ticks */
static pthread_t timer_thread;

/*
 * No idea how to block specific interrupts in the
 * UNIX emulation. For now this should help.
 */
#define NutEnableTimerIrq()     NutEnterCritical()
#define NutDisableTimerIrq()    NutExitCritical()


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
 */
void NutRegisterTimer(void (*handler) (void *))
{
    u_char timerIrqNr = IRQ_TIMER0;

    // register irq handler
    NutRegisterIrqHandler(timerIrqNr, handler, (void *) 0);

    // create rtc timer simulation
    pthread_create(&timer_thread, NULL, NutTimerSimulation, (void *) (uptr_t) timerIrqNr);
}

/*!
 * \brief Return the CPU clock in Hertz.
 *
 * \return CPU clock frequency in Hertz.
 */
u_long NutGetCpuClock(void)
{
    return NUT_CPU_FREQ;
}

/*!
 * \brief Return the number of system ticks per second.
 *
 * \return System tick frequency in Hertz.
 */
u_long NutGetTickClock(void)
{
    return 1000UL;
}

/*!
 * \brief Calculate system ticks for a given number of milliseconds.
 */
u_long NutTimerMillisToTicks(u_long ms)
{
    return ms;
}
