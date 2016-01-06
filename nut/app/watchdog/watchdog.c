/*!
 * Copyright (C) 2016 by Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
 *
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

/*!
 * $Id$
 */

/*!
 * \example threads/watchdo.c
 *
 * This sample demonstrates watchdog behaviour.
 *
 * The application requested a timeout of 200 ms. It starts with
 * sleeping 150 ms and increased the sleeping time until reset happens.
 */

#include <stdio.h>
#include <io.h>

#include <sys/thread.h>
#include <sys/timer.h>

#include <dev/board.h>
#include <dev/watchdog.h>

/*
 * Main application thread.
 */
int main(void)
{
    uint32_t baud = 115200;
    uint32_t ms;
    uint32_t ms_requested;

    /*
     * Register the UART device, open it, assign stdout to it and set
     * the baudrate.
     */
    NutRegisterDevice(&DEV_CONSOLE, 0, 0);
    freopen(DEV_CONSOLE.dev_name, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);

    puts("\nWatchdog Test. Compiled " __DATE__ " " __TIME__);

    /*
     * Endless loop in main thread.
     */
    ms_requested = 200;
    ms = NutWatchDogStart(ms_requested, 0);
    printf("Requested timeout %ld ms, actual value %ld ms\n",
           ms_requested, ms);
    ms = ms  - (ms >> 2);
    for (;;) {
        NutWatchDogRestart();
        NutSleep(ms);
        printf("\rSleeping: %4ld", ms);
        fflush(stdout);
        ms ++;
    }
    return 0;
}
