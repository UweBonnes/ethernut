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
 * unix_nutinit.c - init for unix emulation
 *
 * 2004.04.01 Matthias Ringwald <matthias.ringwald@inf.ethz.ch>
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <cfg/os.h>

#include <sys/types.h>
#include <sys/event.h>
#include <sys/device.h>
#include <sys/osdebug.h>
#include <sys/atom.h>
#include <dev/irqreg.h>
#include <dev/unix_devs.h>

extern void NutAppMain(void *arg) __attribute__ ((noreturn));


/* our IRQ signal */
sigset_t irq_signal;

/* IRQ dispatcher thread */
pthread_t irq_thread;

/* our simulated interrupt flag. to trigger an interrupt this needs to be owned */
pthread_mutex_t irq_mutex;

/* used to signal NutIRQSimulation */
pthread_cond_t irq_cv;

/* interrupt event happend */
u_char irq_vect[IRQ_MAX];

/* interrupt handler routines */
IRQ_HANDLER irq_handlers[IRQ_MAX];

/* irq number to process */
u_char irq_nr;

/* used to ack interrupt processing  */
u_char irq_processed;

/* used to check (not-)allowed IRQ/signaled recursion */
static int irq_inside;

/* queue to signal at next NutThreadYield() */
HANDLE * irq_eventqueues[IRQ_MAX];

/*!
 * \brief Register an interrupt handler.
 *
 * This function is typically called by device drivers.
 *
 * \param irq     Interrupt number to be associated with this handler.
 * \param handler This routine will be called by Nut/OS, when the
 *                specified interrupt occurs.
 * \param arg     Argument to be passed to the interrupt handler.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutRegisterIrqHandler(u_char irq_nr, void (*handler) (void *), void *arg)
{
    if (irq_nr >= IRQ_MAX)
        return -1;

    NutEnterCritical();

    irq_handlers[irq_nr].ir_arg = arg;
    irq_handlers[irq_nr].ir_handler = handler;

    NutExitCritical();

    return 0;
}


/*
 * IRQ simulation, simulate IRQ caused 
 */
void NutIRQTrigger(u_char);
void NutIRQTrigger(u_char irq_nr)
{
    if (irq_nr >= IRQ_MAX)
        return;

    // signal this interrupt
    pthread_mutex_lock(&irq_mutex);
    irq_vect[irq_nr] = 1;
    pthread_cond_broadcast(&irq_cv);
    pthread_mutex_unlock(&irq_mutex);
}

/*
 * Signal handler calling IRQ handler
 */

void NutUnixIntr(int);
void NutUnixIntr(int signal)
{
    // check: recursively entered ?
    if (irq_inside == 1) {
        printf("ERROR: NutTimer0Intr recursively entered\n");
        exit(1);
    }
    // we're inside a IRQ handler
    irq_inside = 1;

    // check: entered although thread in critical section
    if (runningThread) {
        if (runningThread->td_cs_level) {
            printf("ERROR: NutTimer0Intr: CS = %d running %s.>\n", runningThread->td_cs_level, runningThread->td_name);
            exit(1);
        }
    }
    // We're in a critical section. prevent NutExitCritical() to unblock signals
    if (runningThread)
        runningThread->td_cs_level++;
    else
        main_cs_level++;

    // printf("NutUnixIntr: calling IRQ %d\n", irq_nr);
    

    // call IRQ_handler (if set)
    if (irq_nr < IRQ_MAX) {
        if (irq_handlers[irq_nr].ir_handler) {
            irq_handlers[irq_nr].ir_handler(irq_handlers[irq_nr].ir_arg);
        }
    }
    // End of this special critical section.
    if (runningThread)
        runningThread->td_cs_level--;
    else
        main_cs_level--;


    // IRQ handler finished
    irq_inside = 0;

    irq_processed = 1;
}


/*!
 * Interrupt Dispatcher Thread
 *
 * we use an extra thread to
 *  a) be able to seperate irq cause and processing
 *  b) allow a correct dispatching
 *  c) allow to use busy wait until IRQ is finished
 */

void *NutIRQSimulation(void *arg)
{

    // non-nut thread => block IRQ signals
    pthread_sigmask(SIG_BLOCK, &irq_signal, 0);

    while (1) {

        // lock the irq_mutex and wait for interrupts
        pthread_mutex_lock(&irq_mutex);

        // printf("NutIRQSimulation: mutex locked\n");

        // check if irq already set
        for (irq_nr = 0; irq_nr < IRQ_MAX; irq_nr++) {

            if (irq_vect[irq_nr]) {
                break;
            }
        }

        // if none set, wait for next one to occure
        if (irq_nr >= IRQ_MAX)
            pthread_cond_wait(&irq_cv, &irq_mutex);

        // printf("NutIRQSimulation: IRQ received\n");

        // get first pending IRQ
        for (irq_nr = 0; irq_nr < IRQ_MAX; irq_nr++) {

            if (irq_vect[irq_nr]) {

                // clear its flag
                irq_vect[irq_nr] = 0;

                // handshake
                irq_processed = 0;

                // printf("NutIRQSimulation: IRQ signaling nr %d \n", irq_nr);
    
                // signal to running thread
                kill(-getpgrp(), SIGUSR1);
                break;
            }
        }

        // unlock the irq_mutex
        pthread_mutex_unlock(&irq_mutex);

        // printf("NutIRQSimulation: mutex unlocked\n");

        // if irq found and signaled, wait until irq processed
        if (irq_nr < IRQ_MAX) {

            while (1) {

                if (irq_processed)
                    break;

                usleep(1000);

            }
        }
        // printf("NutIRQSimulation: IRQ processed\n");
    }
}

/*!
 * \brief Register NutEventPostAsync for next NutThreadYield
 * 
 * Store responsible IRQ and queue to signal in list
 *
 * \param irq_nr responsible IRQ
 * \param queue to signal
 *
 * this is added to allow an non-nut thread to post events without
 * introducing a race-condition
 *
 */
void NutUnixIrqEventPostAsync(u_char irq_nr, HANDLE * queue )
{
    if (irq_nr < IRQ_MAX)
        irq_eventqueues[irq_nr] = queue;
}

/*!
 * \brief Handle interrupt triggered NutEventPostAsync
 * Check list of events to post and call NutEventPostAsync on them
 *
 * this is added to allow an non-nut thread to post events without
 * introducing a race-condition
 *
 */
void NutUnixThreadYieldHook(void);
void NutUnixThreadYieldHook()
{   
    u_char irq_nr;
    for (irq_nr = 0; irq_nr < IRQ_MAX; irq_nr++) {
        if ( irq_eventqueues[irq_nr] != 0) {
            // printf("NutUnixThreadYield posting event nr %d\n\r", irq_nr);
            NutEventPostFromIrq( irq_eventqueues[irq_nr] );
            irq_eventqueues[irq_nr] = 0; 
        }
    }
}


/*
 * Init IRQ handling
 *
 */
void NutUnixControlC(int);
void NutUnixControlC(int signal)
{
	printf("CTRL-C! Abort application.\n\r");
    exit( 0 );
}

/*
 * Init IRQ handling
 *
 */

void NutIRQInit(void);
void NutIRQInit()
{
    /* Init interrupts */
    irq_inside = 0;

    for (irq_nr = 0; irq_nr < IRQ_MAX; irq_nr++)
        irq_eventqueues[irq_nr] = 0;

    // no irq pending
    irq_nr = IRQ_MAX;

    // define our IRQ signal
    sigemptyset(&irq_signal);
    sigaddset(&irq_signal, SIGUSR1);

    // so far IRQ are not allowed
    // sigprocmask(SIG_BLOCK, &irq_signal, 0);

    // the signal/IRQ handler
    signal(SIGUSR1, NutUnixIntr);
    signal(SIGINT,  NutUnixControlC);

    // synchronization tools
    pthread_mutex_init(&irq_mutex, NULL);
    pthread_cond_init(&irq_cv, NULL);
    pthread_create(&irq_thread, NULL, NutIRQSimulation, (void *) NULL);
}

/*!
 * \addtogroup xgNutInit
 */
/*@{*/

/*! \fn NutIdle(void *arg)
 * \brief Idle thread. 
 *
 * After initializing the timers, the idle thread switches to priority 254
 * and enters an endless loop.
 */
THREAD(NutIdle, arg)
{

    /* Initialize system timers. */
    NutTimerInit();

    /* Create the main application thread. */
    NutThreadCreate("main", NutAppMain, 0, NUT_THREAD_MAINSTACK);

    // printf("main task created, idling now..\n");
    /*
     * Run in an idle loop at the lowest priority. We can still
     * do something useful here, like killing terminated threads
     * or putting the CPU into sleep mode.
     */
    NutThreadSetPriority(254);
    for (;;) {
        NutThreadYield();
        NutThreadDestroy();
        
        // sleep(); ... sleeping would be fine. try to simulate this

        // lock the irq_mutex and wait for interrupts
        pthread_mutex_lock(&irq_mutex);
        pthread_cond_wait(&irq_cv, &irq_mutex);

        // unlock the irq_mutex
        pthread_mutex_unlock(&irq_mutex);
    }
}

/*!
 * \brief Nut/OS Initialization.
 *
 * Initializes the memory management and the thread system and starts 
 * an idle thread, which in turn initializes the timer management. 
 * Finally the application's main() function is called.
 */
#undef main

#define PSEUDO_RAM_SIZE 999999
u_char PSEUDO_RAM[PSEUDO_RAM_SIZE];

extern void NutThreadInit(void);

extern NUTFILE *NUT_freopen(CONST char *name, CONST char *mode, NUTFILE * stream);
extern NUTFILE *__iob[];

int main(int argc, char *argv[])
{

    /* get command line options */
    emulation_options_parse(argc, argv);

    /*
     * Register our Pseudo RAM
     */
    NutHeapAdd(PSEUDO_RAM, PSEUDO_RAM_SIZE);

    /*
     * Read eeprom configuration.
     */
    if (NutLoadConfig()) {
        strcpy(confos.hostname, "ethernut");
        NutSaveConfig();
    }

    /*
     * set stdio
     */

    /*
       NutRegisterDevice(&devUart0, 0, 0);
       NUT_freopen("uart0", "w", __iob[1]);
       printf("OS Debug Mode, stdout opened in unix_nutinit.c\n");
       // NutTraceOs( stdout, 1);
     */

    /*
     * Init interrupt handling
     */
    NutIRQInit();

    /*
     * Init threading
     */
    NutThreadInit();

    /*
     * Create idle thread
     */
    NutThreadCreate("idle", NutIdle, 0, NUT_THREAD_IDLESTACK);
    
    return 0;
}

/*@}*/
