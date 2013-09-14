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
static char *pattern =  "0123456789abcdef0123456789ABCDEF";
static char *pattern1 = "FEDCBA9876543210fedcba987654321";
static char *pattern2 = "0123456789abcdef0123456789ABCD";

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
    uint32_t iap_flash_end = IapFlashEnd();
    void *dword_aligned_end;

    NutRegisterDevice(&DEV_CONSOLE, 0, 0);

    uart = fopen(DEV_CONSOLE.dev_name, "r+");
    _ioctl(_fileno(uart), UART_SETSPEED, &baud);
    _ioctl(_fileno(uart), UART_SETREADTIMEOUT, &read_timeout);
    freopen(DEV_CONSOLE.dev_name, "r", stdin);
    freopen(DEV_CONSOLE.dev_name, "w", stdout);
    printf(banner);
    NutSleep(10);
    res = NutNvMemSave(0x0, "Save0", 6);
    if(res)
        printf("NutNvMemSave failed: %d\n", res);
    NutNvMemLoad(0x0, buffer, 6);
    if (memcmp(buffer, "Save0", 5))
        printf("NutNvMemSave compare failed: %s vs %s\n", buffer, "Save0");

    res = NutNvMemSave(0x0, "Save1", 6);
    if(res)
        printf("NutNvMemSave failed: %d\n", res);
    NutNvMemLoad(0x0, buffer, 6);
    if (memcmp(buffer, "Save1", 5))
        printf("NutNvMemSave compare failed: %s vs %s\n", buffer, "Save1");

    res = NutNvMemSave(0x0, "Save2", 6);
    if(res)
        printf("NutNvMemSave failed: %d\n", res);
    NutNvMemLoad(0x0, buffer, 6);
    if (memcmp(buffer, "Save2", 5))
        printf("NutNvMemSave compare failed: %s vs %s\n", buffer, "Save2");

    res = NutNvMemSave(0x0, "u", 1);
    if(res)
        printf("NutNvMemSave failed: %d\n", res);
    NutNvMemLoad(0x0, buffer, 6);
    if (memcmp(buffer, "uave2", 5))
        printf("NutNvMemSave compare failed: %s vs %s\n", buffer, "uave2");

    printf("Application Flash ends at 0x%08lx\n", iap_flash_end);
    if (*(uint32_t*)(iap_flash_end -0xff) != 0xffffffff)
        printf("Not");
    printf("Empty\n");
    NutSleep(10);
    res = IapFlashWrite((void*)(iap_flash_end -0xff), pattern,
                        strlen(pattern) + 1, FLASH_ERASE_ALWAYS);
    printf("%55s %3d:", "After write Res", res);
    printf((char*)(iap_flash_end -0xff));
    printf("\n");
    NutSleep(10);
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;
    dword_aligned_end = (void*)((iap_flash_end - 0xff + strlen(pattern))
                                & 0xfffffff0);

    res = IapFlashWrite(dword_aligned_end - 1, buffer, 1,
                        FLASH_ERASE_NEVER);
    printf("%55s %3d:", "After unaligned overwrite byte Res", res);
    if ((res == 0) || (res == FLASH_COMPARE))
        printf((char*)(iap_flash_end -0xff));
    NutSleep(10);
    printf("\n");

    res = IapFlashWrite(dword_aligned_end - 3, buffer, 2,
                        FLASH_ERASE_NEVER);
    printf("%55s %3d:", "After unaligned overwrite word Res", res);
    if ((res == 0) || (res == FLASH_COMPARE))
        printf((char*)(iap_flash_end -0xff));
    printf("\n");
    NutSleep(10);

    res = IapFlashWrite(dword_aligned_end - 4, buffer, 2,
                        FLASH_ERASE_NEVER);
    printf("%55s %3d:", "After aligned overwrite word Res", res);
    if ((res == 0) || (res == FLASH_COMPARE))
        printf((char*)(iap_flash_end -0xff));
    printf("\n");
    NutSleep(10);

    res = IapFlashWrite(dword_aligned_end - 5, buffer, 4,
                        FLASH_ERASE_NEVER);
    printf("%55s %3d:", "After unaligned overwrite dword on odd address Res",
           res);
    if ((res == 0) || (res == FLASH_COMPARE))
        printf((char*)(iap_flash_end -0xff));
    printf("\n");
    NutSleep(10);

    res = IapFlashWrite(dword_aligned_end - 6, buffer, 4,
                        FLASH_ERASE_NEVER);
    printf("%55s %3d:", "After unaligned overwrite dword on word address Res",
           res);
    if ((res == 0) || (res == FLASH_COMPARE))
        printf((char*)(iap_flash_end -0xff));
    printf("\n");
    NutSleep(10);

    res = IapFlashWrite(dword_aligned_end - 8, buffer, 4,
                        FLASH_ERASE_NEVER);
    printf("%55s %3d:", "After aligned overwrite dword Res", res);
    if ((res == 0) || (res == FLASH_COMPARE))
        printf((char*)(iap_flash_end -0xff));
    printf("\n");
    NutSleep(10);

    res = IapFlashWrite((void*)(iap_flash_end -0x13f), pattern1,
                        strlen(pattern1) + 1, FLASH_ERASE_NEVER);
    printf("%55s %3d:", "After write with 0x0 on odd address Res", res);
    printf((char*)(iap_flash_end -0x13f));
    printf("\n");
    NutSleep(10);

    res = IapFlashWrite((void*)(iap_flash_end -0x17f), pattern2,
                        strlen(pattern2) + 1, FLASH_ERASE_NEVER);
    printf("%55s %3d:", "After write with 0x0 on odd halfword Res", res);
    printf((char*)(iap_flash_end -0x17f));
    printf("\n");
    NutSleep(10);
    for (;;) {
        NutSleep(100);
    }
    return 0;
}
