/*
 * Copyright (C) 2009 by Rittal GmbH & Co. KG,
 * Ulrich Prinz <prinz.u@rittal.de> All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY EMBEDDED IT AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EMBEDDED IT
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 */

/*
 * $Log$
 *
 * Revision 0.2  2009/04/13 ulrichprinz
 * First checkin, led driver with extra functionality and variable io-access
 * (currently SAM7X256 is tested only)
 *
 */

#include <compiler.h>
#include <stdlib.h>
#include <string.h>
#include <memdebug.h>
#include <sys/heap.h>

#include <io.h>

#include <cfg/arch.h>
#include <dev/board.h>
#include <dev/gpio.h>

#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/event.h>
#include <dev/pca9555.h>
#include <dev/led.h>

typedef struct
{
    void       *next;       /**< pointer to next LED descriptor */
	uint8_t	    interval;	/**< current LED blink interval ( 0, 1, >1 ) */
	uint8_t	    duration;	/**< time of  */
    int         bank;       /**< port where led is connected */
    int         pin;        /**< pin on port, where led is connected */
	uint8_t		state;		/**< current state */
    uint8_t     fx;         /**< selected function of led */
} LEDEventT;

LEDEventT *first_led = NULL;

HANDLE led_evt = NULL;
HANDLE led_tmr = NULL;

/**
 * @brief Configures LED connection port.
 * @note This is an internal function called by NutRegisterLED().
*/
/****************************************************************************/
void InitLED(LEDEventT *led)
/****************************************************************************/
{
    if( led->bank < IOXP_PORT0) {
        GpioPinConfigSet( led->bank, led->pin, GPIO_CFG_OUTPUT);
        GpioPinSetHigh( led->bank, led->pin);
    }
    else {
        IOExpPinConfigSet(led->bank, led->pin, GPIO_CFG_OUTPUT);
        IOExpSetBitHigh( led->bank, led->pin);
    }
}

/**
 * @brief Callback function for LED blink and flash timer.
 * @note This is an internal function of the led driver.
*/
/****************************************************************************/
void TimerCallback(HANDLE timer, void *arg)
/****************************************************************************/
{
    NutEventPostAsync(arg);
}

/**
 * @brief Thread to control blinking and flashing of all registered LEDs.
 * @note This is an internal function of the led driver.
 *
*/
/****************************************************************************/
THREAD( sys_led, arg)
/****************************************************************************/
{
    LEDEventT *led;
    NutThreadSetPriority(16);
    for(;;) {
        if (NutEventWait(arg, NUT_WAIT_INFINITE)==0) {
            led = first_led;
            while( led) 
            {
                if( led->fx >= LED_ONESHOT) 
                {
                    if( led->duration) 
                        led->duration--;
                    else {
                        led->duration = led->interval;
                        if( led->fx==LED_ONESHOT) led->fx = LED_OFF;
                        NutSetLed( (HANDLE*)led, led->interval, led->fx);
                    }
                }
                led = led->next;
            }
        }
    }
}

/**
 * @brief sets state of a LED
 * @param ledh the handle to the LED that should be controlled.
 * @param tim: interval for blinking or time for oneshot.
 * @param fxin: effect to set
 * <table>
 * <tr><td>LED_OFF</td><td>switch LED off</td></tr>
 * <tr><td>LED_ON</td><td>switch LED on</td></tr>
 * <tr><td>>LED_FLIP</td><td>toggle LED</td></tr>
 * <tr><td>>LED_BLINK</td><td>let LED blink</td></tr>
 * <tr><td>>LED_ONESHOT</td><td>let LED flash one time</td></tr>
 * </table>
 * @return None.
 */
/****************************************************************************/
void NutSetLed( HANDLE ledh, uint8_t tim, uint8_t fxin)
/****************************************************************************/
{
    LEDEventT *led = (LEDEventT *)ledh;

    if( ledh == NULL) return;

    led->duration = tim;
    led->fx = fxin;
    
    switch( fxin) {
        case LED_ONESHOT:
            led->interval = 0;
        case LED_ON:
            led->state = 0;
            break;
        case LED_BLINK:
            led->interval = tim;
        case LED_FLIP:
            led->state ^= 1;
            break;
        case LED_OFF:
        default:
            led->state = 1;
            break;
    }

    if( led->bank < IOXP_PORT0)
        GpioPinSet( led->bank, led->pin, led->state);
    else
        IOExpSetBit( led->bank, led->pin, led->state);

    if( led_tmr == NULL) {
        NutThreadCreate("sys_led", sys_led, &led_evt, 512);
        led_tmr = NutTimerStart(10, TimerCallback, &led_evt, 0);
    }
}

/**
 * @brief register an LED for handling.
 * @param ledh the led-handle for accessing the LED through NutSetLed().
 * @param bank: Port of CPU or IO-Expander the LED is connected to.
 * @param pin: Pin at the given port.
 * @return -1 if registering failed, else 0.
 */
/****************************************************************************/
int NutRegisterLed( HANDLE * ledh, int bank, int pin)
/****************************************************************************/
{
    LEDEventT *led;

    /* Check memory constraints and assign memory to new led struct */
    led = malloc(sizeof( LEDEventT));
    *ledh = (void*)led;
    
    if( led == NULL) {
        return -1;
    }

    /* Preset new led struct */
    memset( led, 0, sizeof( LEDEventT));
    led->bank = bank;
    led->pin = pin;

    /* Assign the led to the led chain */
    if( first_led == NULL) {
        /* it is the first led */
        first_led = led;
    }
    else {
        led->next = first_led;
        first_led = led;
    }

    InitLED( led);
    return 0;
}

