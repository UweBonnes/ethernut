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
 */

/*
 * $Log$
 * Revision 1.1  2003/05/09 14:40:50  haraldkipp
 * Initial revision
 *
 * Revision 1.4  2003/02/04 17:50:54  harald
 * Version 3 released
 *
 * Revision 1.3  2003/01/14 13:34:53  harald
 * *** empty log message ***
 *
 * Revision 1.2  2002/08/11 12:25:38  harald
 * ICC mods
 *
 * Revision 1.1  2002/08/02 14:25:09  harald
 * First check in
 *
 */

#include <sys/timer.h>
#include <dev/spidigio.h>

/*!
 * \addtogroup xgSpiDigIo
 */
/*@{*/

static u_char us_loops = 1;

/*!
 * \brief Loop for a number of microseconds.
 *
 * This call will not release the CPU and will not switch 
 * to another thread.
 *
 * \param us Approximate delay in microseconds.
 *
 */
static INLINE void delay_us(u_char us)
{
    u_char _cnt = us * us_loops;

    while (_cnt--)
        _NOP();
}

/*!
 * \brief Toggle SPI clock to perform a shift.
 *
 * Shifting is done on the falling edge of the clock line.
 *
 * The required puls width is 150 ns for the output and only 25 ns for 
 * input shift register. This is far below the rising edge of our RC 
 * filters, which is about 3 us.
 */
static INLINE void ShiftDigital(void)
{
    cbi(PORTD, 7);
    delay_us(4);
    sbi(PORTD, 7);
    delay_us(4);
}

/*!
 * \brief Return the number of shifts required to get an expected bit 
 * value at the input.
 *
 * When calling this function, it is assumed, that the shift 
 * register is already filled with the complement of the input bit.
 *
 * If the search mode is zero, then the function will return
 * the number of shifts until the bit value at the input appears 
 * at the output. This can be used to detect the total size of the
 * shift register.
 *
 * If the search mode is set to one, then the input strobe will be 
 * toggled before shifting and the function returns the number of 
 * shifts until the first unmodified bit appears.  This can be used 
 * to detect the number of inputs.
 *
 * \param num   Total number of shifts, should be 8, 16, 24 or 32.
 * \param bit   Input bit, either 0 or 1.
 * \param smode Search mode.
 *
 * \return The number of shifts.
 */
static u_char CountDigitalShifts(u_char num, u_char bit, u_char smode)
{
    u_char i;
    u_char rc = 0;

    /*
     * Toggle input strobe if we are searching the last modified bit.
     * Input lines are latched on the falling edge.
     */
    if (smode) {
        sbi(PORTB, 7);
        delay_us(4);
        cbi(PORTB, 7);
    }

    /*
     * Set the shift register input.
     */
    if (bit)
        sbi(PORTD, 5);
    else
        cbi(PORTD, 5);
    delay_us(4);

    /*
     * Do the requested number of shifts and watch
     * the requested bit position.
     */
    for (i = 0; i < num; i++) {
        if (bit_is_set(PIND, 6)) {
            if (bit) {
                if (smode) {
                    rc = i + 1;
                } else if (rc == 0) {
                    rc = i + 1;
                }
            }
        } else {
            if (bit == 0) {
                if (smode) {
                    rc = i + 1;
                } else if (rc == 0) {
                    rc = i + 1;
                }
            }
        }
        ShiftDigital();
    }
    return rc;
}

/*!
 * \brief Initialize the digital I/O interface.
 *
 * Automatically detects the number of digital inputs and outputs.
 *
 * \param inputs  Pointer to an 8-bit value, where the number of 
 *                detected inputs will be stored.
 * \param outputs Pointer to an 8-bit value, where the number of 
 *                detected outputs will be stored.
 */
void SpiDigitalInit(u_char * inputs, u_char * outputs)
{
    u_char total = 0;
    u_char i;
    u_char cnt;


    /*
     * Determine the delay loop count based on the CPU clock.
     */
    if ((us_loops = (NutGetCpuClock() + 500000UL) / 4000000UL) < 1)
        us_loops = 1;

    /*
     * Set serial data output 
     * line direction.
     */
    sbi(DDRD, 5);

    /*
     * Set serial data input 
     * line direction and 
     * enable pullup.
     */
    sbi(PORTD, 6);
    cbi(DDRD, 6);

    /*
     * Clock. Input data is shifted on the falling, output
     * data on the rising edge.
     */
    sbi(PORTD, 7);
    sbi(DDRD, 7);

    /*
     * UCN5841 output strobe. Shift register data appears on the 
     * output pins as long as this line is held high.
     */
    sbi(PORTB, 5);
    sbi(DDRB, 5);

    /*
     * SN74HC165 input strobe. Data at input pins is latched in
     * the shift register on the falling edge.
     */
    cbi(PORTB, 7);
    sbi(DDRB, 7);

    /*
     * Fill the shift register with zeros.
     * Then shift in ones until the first
     * appears on the output. This gives 
     * us the total size plus one of the 
     * shift register.
     */
    CountDigitalShifts(32, 0, 0);
    total = CountDigitalShifts(32, 1, 0) - 1;

    /*
     * Determine the number of inputs. We do 
     * this five times for zeros and ones and 
     * take the maximum count. This way we 
     * compensate changing inputs while 
     * counting.
     */
    *inputs = 0;
    for (i = 0; i < 5; i++) {
        if ((cnt = CountDigitalShifts(total, 0, 1)) > *inputs)
            *inputs = cnt;
        if ((cnt = CountDigitalShifts(total, 1, 1)) > *inputs)
            *inputs = cnt;
    }
    *outputs = total - *inputs;
}

/*!
 * \brief Query digital inputs and outputs.
 *
 * If both inputs and outputs are installed, typically the
 * inputs are represented by the most signigficant bits.
 *
 * SpiDigitalInit() must have been called by the application
 * before calling this function.
 *
 * \param num Number of bits to query, either 8, 16, 24 or 32.
 *
 * \return Binary value of the requested inputs.
 */
u_long SpiDigitalGet(u_char num)
{
    u_char i;
    u_long bits = 0;

    cbi(PORTD, 5);
    /*
     * Toggle input strobe.
     */
    sbi(PORTB, 7);
    delay_us(4);
    cbi(PORTB, 7);
    delay_us(4);

    for (i = 0; i < num; i++) {
        bits <<= 1;
        if (bit_is_set(PIND, 6)) {
            bits |= 1;
        }
        ShiftDigital();
    }
    return bits;
}

/*!
 * \brief Set digital outputs.
 *
 * Although possible, it makes no sense to set digital inputs.
 *
 * SpiDigitalInit() must have been called by the application
 * before calling this function.
 *
 * \param num Number of bits to set, either 8, 16, 24 or 32.
 * \param bits The bit value to be set
 */
void SpiDigitalSet(u_char num, u_long bits)
{
    u_char i;
    u_long mask;

    if (num) {
        mask = 1UL << (num - 1);
        for (i = 0; i < num; i++) {

            /*
             * Set output bit.
             */
            if (bits & mask)
                sbi(PORTD, 5);
            else
                cbi(PORTD, 5);
            delay_us(4);

            ShiftDigital();

            mask >>= 1;
        }

        /*
         * Toggle output strobe.
         */
        cbi(PORTB, 5);
        delay_us(4);
        sbi(PORTB, 5);
    }
}

/*@}*/
