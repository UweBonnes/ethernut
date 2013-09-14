/*!
 * Copyright (C) 2001-2003 by egnite Software GmbH
 * Copyright (C) 2013 Uwe Bonnes
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
 * \example uart/uart.c
 *
 * This sample demonstrates the usage of the parameter memory.
 */

#include <string.h>
#include <stdio.h>
#include <io.h>

#include <dev/board.h>
#include <dev/iap_flash.h>
#include <dev/nvmem.h>
#include <sys/timer.h>

static char *banner = "\nNut/OS Flash Sample " __DATE__ " " __TIME__"\n";

/*
 * UART sample.
 *
 * Some functions do not work with ICCAVR.
 */
int main(void)
{
    int res;
    uint32_t baud = 115200, read_timeout = 10;
    FILE *uart;
    char buffer[7];

    NutRegisterDevice(&DEV_CONSOLE, 0, 0);

    uart = fopen(DEV_CONSOLE.dev_name, "r+");
    _ioctl(_fileno(uart), UART_SETSPEED, &baud);
    _ioctl(_fileno(uart), UART_SETREADTIMEOUT, &read_timeout);
    freopen(DEV_CONSOLE.dev_name, "r", stdin);
    freopen(DEV_CONSOLE.dev_name, "w", stdout);
    printf(banner);
    NutSleep(10);
    NutNvMemSave(0x0, "Save0", 6);
    NutNvMemSave(0x0, "Save1", 6);
    NutNvMemSave(0x0, "Save2", 6);
    NutNvMemLoad(0x0, buffer, 6);
    if (*(uint32_t*)0x080c0000 != 0xffffffff)
        printf("Not");
    printf("Empty\n");
    NutSleep(10);
    res = IapFlashWrite((void*)0x080c0000, banner + 1, strlen(banner),
                        FLASH_ERASE_ALWAYS);
    printf("After write Res %d:", res);
    NutSleep(10);
    printf((char*)0x080c0000);
    NutSleep(10);
    buffer[0] = 0;
    res = IapFlashWrite((void*)0x080c0008, buffer, 1,
                        FLASH_ERASE_NEVER);
    printf("After overwrite Res %d:", res);
    printf((char*)0x080c0000);
    printf("\n");
    NutSleep(10);

    for (;;) {
        NutSleep(100);
    }
    return 0;
}
