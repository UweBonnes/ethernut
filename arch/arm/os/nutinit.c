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
 * Revision 1.16  2008/08/06 12:51:05  haraldkipp
 * Added support for Ethernut 5 (AT91SAM9XE reference design).
 *
 * Revision 1.15  2008/02/15 16:59:27  haraldkipp
 * Spport for AT91SAM7SE512 added.
 *
 * Revision 1.14  2007/10/04 20:08:00  olereinhardt
 * Support for SAM7S256 added
 *
 * Revision 1.13  2006/09/29 12:39:51  haraldkipp
 * Spurious interrupt handling on all supported AT91 devices.
 *
 * Revision 1.12  2006/07/26 11:17:16  haraldkipp
 * Defining AT91_PLL_MAINCK will automatically determine SAM7X clock by
 * reading PLL settings.
 *
 * Revision 1.11  2006/07/18 14:04:10  haraldkipp
 * Low level hardware initialization moved to crtat91sam7x256_rom.S. This
 * avoids the ugly jump from C code back into the runtime initialization.
 * Watchdog reset (tiger bell) removed from idle thread.
 *
 * Revision 1.10  2006/07/15 11:13:30  haraldkipp
 * CPU ran into the data pool of Sam7xLowLevelInit(). Temporarily
 * fixed by Andras Albert with an additional global label in the
 * startup code. Furthermore Andras changed the clock initialization.
 * The CPU is now running at 47.9232 MHz and the MAC starts working.
 * Great, TCP/IP is now running on the SAM7X.
 *
 * Revision 1.9  2006/07/10 14:27:03  haraldkipp
 * C++ will use main instead of NutAppMain. Contributed by Matthias Wilde.
 *
 * Revision 1.8  2006/07/05 07:57:52  haraldkipp
 * Daidai's support for AT91SAM7X added. Possibly taken from Atmel.
 * May require new coding from ground up in order to not conflict with
 * original copyright.
 * Nevertheless, many thanks to Daidai for providing his adaption.
 *
 * Revision 1.7  2006/06/28 17:22:34  haraldkipp
 * Make it compile for AT91SAM7X256.
 *
 * Revision 1.6  2006/03/02 19:43:11  haraldkipp
 * Added MCU specific hardware initialization routine. This should be done
 * later for all MCUs to avoid contaminating NutInit() with MCU specific
 * stuff. For the AT91 the spurious interrupt handler has been added,
 * which fixes SF 1440948.
 *
 * Revision 1.5  2006/02/23 15:34:00  haraldkipp
 * Support for Philips LPC2xxx Family and LPC-E2294 Board from Olimex added.
 * Many thanks to Michael Fischer for this port.
 *
 * Revision 1.4  2005/10/24 09:22:29  haraldkipp
 * Default idle and main thread stack sizes increased.
 * AT91 header file moved.
 *
 * Revision 1.3  2005/08/02 17:46:45  haraldkipp
 * Major API documentation update.
 *
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
#elif defined(MCU_LPC2XXX)
#include <arch/arm/lpc2xxx.h>
#else
#include <arch/arm/at91.h>
#endif

/*!
 * \addtogroup xgNutArchArmInit
 */
/*@{*/

#ifndef NUT_THREAD_MAINSTACK
#define NUT_THREAD_MAINSTACK    1024
#endif

#ifndef NUT_THREAD_IDLESTACK
#define NUT_THREAD_IDLESTACK    512
#endif

#ifdef __CROSSWORKS4ARM__
extern void *__unused_start__;
/*
 * Michael, Why does Crossworks needs this one. Is memory configurable
 * with the Configurator?
 */
extern void *__External_SRAM_segment_end__;

#define HEAP_START  &__unused_start__
#define HEAP_SIZE  ((uptr_t)(&__External_SRAM_segment_end__ - 1) - (uptr_t)(HEAP_START) - 256)
#else   /* GCC */
/*!
 * \brief Last memory address.
 */
#define NUTMEM_END (uptr_t)(NUTMEM_START + NUTMEM_SIZE - 1U)
extern void *__heap_start;

#define HEAP_START  &__heap_start
#define HEAP_SIZE  ((uptr_t) (NUTMEM_END - 256 - (uptr_t) (&__heap_start)))
#endif

#if !defined(__arm__) && !defined(__cplusplus)
extern void NutAppMain(void *arg) __attribute__ ((noreturn));
#else
extern void main(void *);
#endif


#if defined(OLIMEX_LPCE2294)
/*
 * InitHW for OLIMEX LPC-E2294
 */
static void InitHW (void)
{
  PINSEL0  = 0;
  PINSEL1  = 0;

  BCFG2    = 0x03501;
  PINSEL2 |= 0x00804000;
} /* InitHW */

#endif /* OLIMEX_LPCE2294 */



/*!
 * \brief Idle thread. 
 *
 * \param arg Ignored by the idle thread.
 *
 * This function runs in an endless loop as a lowest priority thread.
 */
THREAD(NutIdle, arg)
{
#if defined(MCU_GBA) || defined(MCU_LPC2XXX)
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
#if defined(OLIMEX_LPCE2294)
    InitHW();
#elif defined(MCU_AT91R40008) || defined (MCU_AT91SAM7X256) || defined (MCU_AT91SAM7S256) || defined (MCU_AT91SAM9260) || defined(MCU_AT91SAM7SE512) || defined(MCU_AT91SAM9XE512)
    McuInit();
#endif
#if defined(MCU_AT91SAM7X256) || defined (MCU_AT91SAM7S256) || defined(MCU_AT91SAM7SE512)
    {
        u_long freq = NutGetCpuClock();
        /* Set Flash Waite state. */
        outr(MC_FMR, ((((freq + freq / 2) / 1000000UL) & 0xFF) << 16) | MC_FWS_2R3W);
    }
#endif

    NutHeapAdd(HEAP_START, HEAP_SIZE);

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
