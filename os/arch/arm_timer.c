/*
 * Copyright (C) 2001-2004 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.3  2004/10/03 18:42:21  haraldkipp
 * No GBA support yet, but let the compiler run through
 *
 * Revision 1.2  2004/09/08 10:19:39  haraldkipp
 * Running on AT91 and S3C, thanks to James Tyou
 *
 */

#include <cfg/arch.h>
#if defined(MCU_AT91R40008)
#include <arch/at91eb40a.h> /* LED debug */
#include <arch/at91.h>
#elif defined(MCU_S3C4510B)
#include <arch/arm.h>
#include <dev/s3c4510b_hw.h>
#include <dev/s3c4510b_irqs.h>
#endif

#if defined(MCU_AT91R40008)
static int dummy;
#endif

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
    u_short delay_cnt = 2400;   //*KU* for 14.745600 MHz Clock
    u_short delay_cnt_buffer;

    while (ms--) {
        delay_cnt_buffer = delay_cnt;
        while (delay_cnt_buffer--);
    }
}

#if defined(MCU_AT91R40008)
/*
 * Timer interrupt handler.
 */
static void NutTimerIntr(void *arg)
{
    NUTTIMERINFO *tnp;

    dummy = inr(TC0_SR);

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
                (*nutTimerList->tn_callback) (nutTimerList, (void *) nutTimerList->tn_arg);

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
#endif

/*!
 * \brief Timer 0 interrupt entry.
 */
#if defined(MCU_AT91R40008)
static void Timer0Entry(void) __attribute__ ((naked));
void Timer0Entry(void)
{
    IRQ_ENTRY;
    NutTimerIntr(0);
    IRQ_EXIT;
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
 *
 * \todo Hardware stuff to be put in nutlibdev.
 */
void NutTimerInit(void)
{
#if defined(MCU_AT91R40008)

    /* Disable the Clock Counter */
    outr(TC0_CCR, TC_CLKDIS);
    /* Disable all interrupts */
    outr(TC0_IDR, 0xFFFFFFFF);
    /* Clear the status register. */
    dummy = inr(TC0_SR);
    /* Select divider and compare trigger */
    outr(TC0_CMR, TC_CLKS_MCK32 | TC_CPCTRG);
    /* Enable the Clock counter */
    outr(TC0_CCR, TC_CLKEN);
    /* Validate the RC compare interrupt */
    outr(TC0_IER, TC_CPCS);

    /* Disable timer 0 interrupts. */
    outr(AIC_IDCR, (1<<TC0_ID));
    /* Set the TC0 IRQ handler address */
    outr(AIC_SVR(4), (unsigned int)Timer0Entry);
    /* Set the trigg and priority for timer 0 interrupt */
    outr(AIC_SMR(4), (AIC_SRCTYPE_INT_LEVEL_SENSITIVE | 0x4));
    /* Clear timer 0 interrupt */
    outr(AIC_ICCR, (1<<TC0_ID));
    /* Enable timer 0 interrupts */
    outr(AIC_IECR, (1<<TC0_ID));

    /* Set compare value for 1 ms. */
    outr(TC0_RC, 0x80F);
    /* Software trigger starts the clock. */
    outr(TC0_CCR, TC_SWTRG);

#elif defined(MCU_S3C4510B)

    INT_DISABLE(IRQ_TIMER);
    CSR_WRITE(TCNT0, 0);
    CSR_WRITE(TDATA0, CLOCK_TICK_RATE);
    CSR_WRITE(TMOD, TMOD_TIMER0_VAL);

    CLEAR_PEND_INT(IRQ_TIMER);

    NutRegisterIrqHandler(
        &InterruptHandlers[IRQ_TIMER], NutTimerIntr, 0);

    INT_ENABLE(IRQ_TIMER);

#elif defined(MCU_GBA)
    ms1++; /* Avoids compiler warning */
#else
#warning "MCU not defined"
#endif
}
