/*!
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
 */

/*!
 * $Log$
 * Revision 1.6  2008/01/31 09:38:15  haraldkipp
 * Added return statement in main to avoid warnings with latest GCC.
 *
 * Revision 1.5  2005/11/22 09:17:31  haraldkipp
 * Replaced specific device names by generalized macros.
 * Thread stack size increased to get it running on ARM CPUs.
 *
 * Revision 1.4  2005/04/05 18:00:29  haraldkipp
 * Make it work on the Gameboy Advance.
 *
 * Revision 1.3  2004/09/10 10:31:06  haraldkipp
 * Use newer AVR UART device driver
 *
 * Revision 1.2  2004/09/08 10:18:23  haraldkipp
 * For EB40A
 *
 * Revision 1.1  2003/08/05 18:59:05  haraldkipp
 * Release 3.3 update
 *
 * Revision 1.7  2003/02/04 18:19:40  harald
 * Version 3 released
 *
 * Revision 1.6  2003/02/04 16:24:36  harald
 * Adapted to version 3
 *
 * Revision 1.5  2002/06/26 17:29:06  harald
 * First pre-release with 2.4 stack
 *
 * Revision 1.4  2002/06/04 19:13:06  harald
 * *** empty log message ***
 *
 * Revision 1.3  2002/05/08 16:02:33  harald
 * First Imagecraft compilation
 *
 * Revision 1.2  2001/12/04 16:45:40  harald
 * Stack space increased
 *
 * Revision 1.1  2001/06/28 18:43:13  harald
 * Preview release
 *
 */

/*!
 * \example threads/threads.c
 *
 * This sample demonstrates Nut/OS multithreading.
 *
 * Each thread is started with 192 bytes of stack. This is very
 * low and doesn't provide much space for local variables.
 */

#include <stdio.h>
#include <io.h>

#include <cfg/arch.h>
#include <dev/board.h>

#include <sys/thread.h>
#include <sys/timer.h>

/*
 * High priority thread.
 */
THREAD(Thread1, arg)
{
    /*
     * Endless loop in high priority thread.
     */
    NutThreadSetPriority(16);
    for (;;) {
        putchar('H');
        NutSleep(125);
    }
}

/*
 * Low priority thread.
 */
THREAD(Thread2, arg)
{
    /*
     * Endless loop in low priority thread.
     */
    NutThreadSetPriority(128);
    for (;;) {
        putchar('L');
        NutSleep(125);
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
    NutRegisterDevice(&DEV_UART, 0, 0);
    freopen(DEV_UART_NAME, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);

    puts("\nThread Test");

    /*
     * Start two additional threads. All threads are started with 
     * priority 64.
     */
    NutThreadCreate("t1", Thread1, 0, 512);
    NutThreadCreate("t2", Thread2, 0, 512);

    /*
     * Endless loop in main thread.
     */
    for (;;) {
        putchar('M');
        NutSleep(125);
    }
    return 0;
}
