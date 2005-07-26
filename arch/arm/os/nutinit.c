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
 * Revision 1.2  2005/07/26 16:17:03  haraldkipp
 * Use default stack sizes for main and idle, if none had been defined.
 *
 * Revision 1.1  2005/05/27 17:16:40  drsung
 * Moved the file.
 *
 * Revision 1.4  2005/04/05 17:52:41  haraldkipp
 * Much better implementation of GBA interrupt registration.
 *
 * Revision 1.3  2004/11/08 18:58:59  haraldkipp
 * Configurable stack sizes
 *
 * Revision 1.2  2004/09/08 10:19:23  haraldkipp
 * Made it look more general
 *
 * Revision 1.1  2004/03/16 16:48:46  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 *
 */

#include <cfg/arch.h>
#include <cfg/memory.h>
#include <cfg/os.h>
#ifdef MCU_GBA
#include <dev/irqreg.h>
#else
#include <arch/at91.h>
#include <arch/at91eb40a.h>
#endif

#ifndef NUT_THREAD_MAINSTACK
#define NUT_THREAD_MAINSTACK    768
#endif

#ifndef NUT_THREAD_IDLESTACK
#define NUT_THREAD_IDLESTACK    384
#endif

/*!
 * \brief Last memory address.
 */
#define NUTMEM_END (uptr_t)(NUTMEM_START + NUTMEM_SIZE - 1U)

extern void NutAppMain(void *arg) __attribute__ ((noreturn));
extern void *__heap_start;

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
#ifdef MCU_GBA
    InitIrqHandler();
#endif
    /* Initialize system timers. */
    NutTimerInit();

    /* Create the main application thread. */
    NutThreadCreate("main", main, 0, NUT_THREAD_MAINSTACK);

    /*
     * Run in an idle loop at the lowest priority. We can still
     * do something useful here, like killing terminated threads
     * or putting the CPU into sleep mode.
     */
    NutThreadSetPriority(254);
    for (;;) {
        NutThreadYield();
        NutThreadDestroy();
    }
}

/*!
 * \brief Nut/OS Initialization.
 *
 * Initializes the memory management and the thread system and starts 
 * an idle thread, which in turn initializes the timer management. 
 * Finally the application's main() function is called.
 */
void NutInit(void)
{
    NutHeapAdd(&__heap_start, (uptr_t)(NUTMEM_END - 256 - (uptr_t)(&__heap_start)));

    /*
     * No EEPROM configuration.
     */
    strcpy(confos.hostname, "ethernut");

    /*
     * Create idle thread
     */
    NutThreadCreate("idle", NutIdle, 0, NUT_THREAD_IDLESTACK);
}

/*@}*/
