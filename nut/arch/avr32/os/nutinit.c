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
 * $Log: nutinit.c,v $
 *
 *
 */

#include <cfg/clock.h>
#include <cfg/arch.h>
#include <cfg/memory.h>
#include <cfg/os.h>
#include <arch/avr32.h>

#include <arch/avr32/pm.h>


/*!
 * \addtogroup xgNutArchAvr32Init
 */
/*@{*/

#ifndef NUT_THREAD_MAINSTACK
#define NUT_THREAD_MAINSTACK    1024
#endif

/* Unknown optimized value */
#ifndef NUT_THREAD_IDLESTACK
#define NUT_THREAD_IDLESTACK    512
#endif


extern void *__heap_start;

/*!
* \brief Last memory address.
*/
#define NUTMEM_END (uptr_t)(NUTMEM_START + NUTMEM_SIZE - 1U)
extern void *__heap_start;

#define HEAP_START  &__heap_start
#define HEAP_SIZE  ((uptr_t) (NUTMEM_END - 256 - (uptr_t) (&__heap_start)))


#if !defined(__cplusplus)
extern void NutAppMain(void *arg) __attribute__ ((noreturn));
#else
extern void main(void *);
#endif


// Defined in ihndlr.c
extern void IrqInit(void);

/*!
 * \brief Idle thread. 
 *
 * \param arg Ignored by the idle thread.
 *
 * This function runs in an endless loop as a lowest priority thread.
 */
THREAD(NutIdle, arg)
{

	// Init INTC
	IrqInit();

    /* Initialize system timers. */
    NutTimerInit();

	/* Read OS configuration from non-volatile memory. We can't do this
	** earlier, because the low level driver may be interrupt driven. */
	NutLoadConfig();

	/* Create the main application thread. Watch this carefully when
	** changing compilers and compiler versions. Some handle main()
	** in a special way, like setting the stack pointer and other
	** weird stuff that may break this code. */
	NutThreadCreate("main", main, 0, 
		(NUT_THREAD_MAINSTACK * NUT_THREAD_STACK_MULT) + NUT_THREAD_STACK_ADD);

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
	pm_switch_to_osc0(&AVR32_PM, OSC0_VAL, AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);

	// Configure Clocks & Flash wait States 
#if defined(PLL_MUL_VAL) || defined(PLL_DIV_VAL)
	pm_pll_setup(&AVR32_PM,
		0,   // unsigned int pll
		PLL_MUL_VAL,   // unsigned int mul
		PLL_DIV_VAL / 2,   // unsigned int div, Sel Osc0/PLL0 or Osc1/Pll1
		0,   // unsigned int osc
		16); // unsigned int lockcount

# if PLL_DIV_VAL > 1
	// set PLL options to run half speed.
	pm_pll_set_option(&AVR32_PM, 
		0,  // unsigned int pll
		0,  // unsigned int  pll_freq
		1,  // unsigned int  pll_div2
		0); // unsigned int  pll_wbwdisable
# endif

	// Enable PLL0
	pm_pll_enable(&AVR32_PM,0);
	// Wait for PLL0 locked
	pm_wait_for_pll0_locked(&AVR32_PM);

# ifndef PLL_MUL_VAL
#  define PLL_MUL_VAL 0
# endif
# ifndef PLL_DIV_VAL
#  define PLL_DIV_VAL 1
# endif
# define F_CPU ((PLL_MUL_VAL+1)/PLL_DIV_VAL*OSC0_VAL)

	// Scale down peripheral clocks not to exceed their maximum ratings
	pm_cksel(&AVR32_PM,
#if F_CPU > AVR32_PM_PBA_MAX_FREQ
	1, // pbadiv
	F_CPU / 2 / AVR32_PM_PBA_MAX_FREQ, // pbasel
#else
	0, 0,
#endif
#if F_CPU > AVR32_PM_PBB_MAX_FREQ
	1, // pbbdiv
	F_CPU / 2 / AVR32_PM_PBA_MAX_FREQ, // pbbsel
#else
	0, 0,
#endif
#if F_CPU > AVR32_PM_HSB_MAX_FREQ
	1, // hsbdiv
	F_CPU / 2 / AVR32_PM_HSB_MAX_FREQ // hsbsel
#else
	0, 0
#endif
	);


# if F_CPU > AVR32_FLASHC_FWS_0_MAX_FREQ
	AVR32_FLASHC.FCR.fws = 1;
# endif

	pm_switch_to_clock(&AVR32_PM, AVR32_PM_MCCTRL_MCSEL_PLL0);

#else
# define F_CPU OSC0_VAL
# if F_CPU > AVR32_FLASHC_FWS_0_MAX_FREQ
	AVR32_FLASHC.FCR.fws = 1;
# endif
#endif

#ifdef EARLY_STDIO_DEV
	/* We may optionally initialize stdout as early as possible.
	** Be aware, that no heap is available and no threads are 
	** running. We need a very basic driver here, which won't
	** use interrupts or call malloc, NutEventXxx, NutSleep etc. */
	{
		extern NUTDEVICE EARLY_STDIO_DEV;
		static struct __iobuf early_stdout;
		/* Initialize the output device. */
		EARLY_STDIO_DEV.dev_init(&EARLY_STDIO_DEV);
		/* Assign a static iobuf. */
		stdout = &early_stdout;
		/* Open the device. */
		stdout->iob_fd = (int)EARLY_STDIO_DEV.dev_open(&EARLY_STDIO_DEV, "", 0, 0);
		/* Set the mode. No idea if this is required. */
		stdout->iob_mode = _O_WRONLY | _O_CREAT | _O_TRUNC;
		/* A first trial. */
		puts("\nStarting Nut/OS");
	}
#endif
	/* Initialize our heap memory. */
    NutHeapAdd(HEAP_START, HEAP_SIZE);

	/* Create idle thread. Note, that the first call to NutThreadCreate 
	** will never return. */
	NutThreadCreate("idle", NutIdle, 0, 
		(NUT_THREAD_IDLESTACK * NUT_THREAD_STACK_MULT) + NUT_THREAD_STACK_ADD);
}

/*@}*/
