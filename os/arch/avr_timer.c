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
 * Revision 1.3  2005/01/24 22:34:50  freckle
 * Added new tracer by Phlipp Blum <blum@tik.ee.ethz.ch>
 *
 * Revision 1.2  2004/12/16 08:40:35  haraldkipp
 * Late increment fixes ICCAVR bug.
 *
 * Revision 1.1  2004/03/16 16:48:46  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.3  2004/03/05 20:38:18  drsung
 * Bugfix from bugfix. sorry!
 *
 * Revision 1.2  2004/03/05 20:19:45  drsung
 * Bugfix in NutTimerInit. ICCAVR failed to compile, if NUT_CPU_FREQ is defined.
 *
 * Revision 1.1  2004/02/01 18:49:48  haraldkipp
 * Added CPU family support
 *
 */

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
                  "mov %B0, %B2\n" "L_dl2%=:\n\t" "sbiw %A0, 1\n\t" "brne L_dl2%=\n\t" "dec %1\n\t" "brne L_dl1%=\n\t":"=&w" (cnt)
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

/*
 * Timer 0 interrupt handler.
 */
static void NutTimer0Intr(void *arg)
{
    NUTTIMERINFO *tnp;

#ifdef NUTTRACER
    TRACE_ADD_ITEM(TRACE_TAG_INTERRUPT_ENTER,TRACE_INT_TIMER0_OVERFL)
#endif

    /*
     * Increment the tick counter used by Michael Fischer's
     * NutGetTickCount() routine.
     */
    milli_ticks++;

#ifdef NUT_CPU_FREQ

    millis++;
    ms1++; /* See comment on volatiles below. */
    if (ms1 >= 1000) {
        ms1 = 0;
        seconds++;
    }
#else                           /* NUT_CPU_FREQ */

    if (ms62_5 & 1) {
        millis += 62;
    } else {
        millis += 63;
    }
    /*
     * Update RTC. We do a post increment here, because
     * of an ImageCraft bug with volatiles. Thanks to
     * Michael Fischer.
     *
     * Later comment: Now I'm confused. The ICCAVR bug was reported on
     * pre-increments only. But later Michael Fischer sent a fix for
     * twi, where all increments of volatile variables had been moved
     * out of the comparision. Checking all of the code again for these
     * increments I stumbled over this one, which had been fixed
     * previously by changing the pre-increment to a post-increment.
     *
     * To play save, I'll follow Michael's advice and isolate all
     * increments of volatile variables from expressions.
     */
    ms62_5++;
    if (ms62_5 >= 16) {
        ms62_5 = 0;
        seconds++;
    }
#endif                          /* NUT_CPU_FREQ */

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
#ifdef NUTTRACER
    TRACE_ADD_ITEM(TRACE_TAG_INTERRUPT_EXIT,TRACE_INT_TIMER0_OVERFL)
#endif    
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
                  "sts (cpu_clock)+1,r25" "\n\t" "sts (cpu_clock)+2,r26" "\n\t":::"r24", "r25", "r26", "r27", "memory");
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
    outp(BV(CS00) | BV(CS02) | BV(WGM01), TCCR0);
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
