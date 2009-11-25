/*
 * Copyright (C) 2009 by Ulrich Prinz, <uprinz2@netscape.net>
 * All rights reserved.
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
 * Revision 0.3  2009/09/12 ulrichprinz
 * First checkin, new push button driver example
 * (currently SAM7X256 is tested only)
 *
 */

/*!
 * \example buttons/pbtest.c
 *
 * This sample demonstrates Nut/OS LEd and key handling.
 * It is written for the ATMEL SAM7X-EK but can be adapted to any other
 * hardware within minutes.
 *
 */

#include <stdio.h>
#include <io.h>

#include <cfg/arch.h>
#include <cfg/dev.h>
#include <dev/board.h>
#include <sys/timer.h>
#include <sys/event.h>
#include <sys/thread.h>

#include <dev/gpio.h>

#include <dev/keys.h>
#include <dev/led.h>

/* Handles for the LEDs */
HANDLE led1, led2, led3, led4;
/* Handles for the keys / joystick of the SAM7X-EK */
HANDLE keyUp, keyDn, keyLt, keyRt, keyMi;
/* Handles for two threads controlled by the keys */
HANDLE keyT1w, keyT2w;

/*!
 * \brief First thread responding to pressed keys.
 *
 * This thread handles some of the keys of this example.
 * In response to a key it gives some text on the debug consle
 * and alters the behaviour of the assigned LEDs.
 *
 */
THREAD( Key1Thread, arg)
{
    NutThreadSetPriority(60);
    for(;;) {
        NutEventWait( &keyT1w, NUT_WAIT_INFINITE);
        if( NutGetKeyState( keyMi) & KEY_PENDING) {
            printf("KEY ENTER pressed\n");
            NutSetLed( led2, LED_OFF, 0, 0);
            NutSetLed( led3, LED_OFF, 0, 0);
            NutSetLed( led4, LED_OFF, 0, 0);
        }
        if( NutGetKeyState( keyDn) & KEY_PENDING) {
            printf("\nKEY DOWN pressed\n");
            NutSetLed( led2, LED_BLINK, 100, 900);
            NutSetLed( led3, LED_BLINK, 500, 500);
            NutSetLed( led4, LED_BLINK, 900, 100);
        }
    }
}

/*!
 * \brief Second0 thread responding to pressed keys.
 *
 * This thread handles some of the keys of this example.
 * In response to a key it gives some text on the debug consle
 * and alters the behaviour of the assigned LEDs.
 *
 */
THREAD( Key2Thread, arg)
{
    NutThreadSetPriority(60);
    for(;;) {
        NutEventWait( &keyT2w, NUT_WAIT_INFINITE);
        if( NutGetKeyState( keyLt) & KEY_PENDING) {
            printf("\nKEY LEFT pressed\n");
            NutSetLed( led2, LED_ON, 200, 0);
            NutSetLed( led4, LED_OFF, 0, 0);
        }
        if( NutGetKeyState( keyRt) & KEY_PENDING) {
            printf("\nKEY RIGHT pressed\n");
            NutSetLed( led2, LED_OFF, 0, 0);
            NutSetLed( led4, LED_ON, 200, 0);
        }
        if( NutGetKeyState( keyUp) & KEY_PENDING) {
            printf("\nKEY UP pressed\n");
            NutSetLed( led3, LED_FLIP, 0, 0);
        }
    }
}


/*
 * Main application thread.
 */
int main(void)
{
    u_long baud = 115200;
    /*
     * Register the UART device, open it, assign stdout to it and set
     * the baudrate.
     */
    NutRegisterDevice(&DEV_DEBUG, 0, 0);
    freopen(DEV_DEBUG_NAME, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);

    puts("\n*** LED and key Test ***\n");

    /* Register LED1 as blinking led */
	NutRegisterLed( &led1, NUTGPIO_PORTB, 19);
	NutSetLed( led1, LED_BLINK, 100, 900);

    /* Register LED2 and let it flash once for 200ms */
    NutRegisterLed( &led2, NUTGPIO_PORTB, 20);
    NutSetLed( led2, LED_ON, 200, 0);

    /* Register LED3 and let it flash once for 200ms */
    NutRegisterLed( &led3, NUTGPIO_PORTB, 21);
    NutSetLed( led3, LED_ON, 200, 0);

    /* Register LED4 and let it flash once for 200ms */
    NutRegisterLed( &led4, NUTGPIO_PORTB, 22);
    NutSetLed( led4, LED_ON, 200, 0);

    /* Register keys for thread 1 mutex
     *   Register keyMi to release mutex simply on pressing key1
     *   what will clear three right LEDs.
     *   Register keyDn to release mutex if key2 is hold down for 2 seconds
     *   what will bring right three LEDs to different blinking.
     */
	NutRegisterKey( &keyMi, &keyT1w, NUTGPIO_PORTA, 25, KEY_ACTION_SHORT, 1000);
	NutRegisterKey( &keyDn, &keyT1w, NUTGPIO_PORTA, 22, KEY_ACTION_HOLD, 2000);

    /* Register keys for thread 2 mutex
     *   Register key3 to release mutex if it is relased
     *   Register key4 to release mutex if it is pressed very short
     */
	NutRegisterKey( &keyLt, &keyT2w, NUTGPIO_PORTA, 23, KEY_ACTION_UP, 0);
	NutRegisterKey( &keyRt, &keyT2w, NUTGPIO_PORTA, 24, KEY_ACTION_DOWN, 0);
	NutRegisterKey( &keyUp, &keyT2w, NUTGPIO_PORTA, 21, KEY_ACTION_DOWN, 0);

    /* Register threads that wait on keys */
    NutThreadCreate("k1", Key1Thread, NULL, 256);
    NutThreadCreate("k2", Key2Thread, NULL, 256);

    /*
     * Endless loop in main thread.
     */
    for (;;) {
        putchar('.');
        NutSleep(2000);
    }
    return 0;
}
