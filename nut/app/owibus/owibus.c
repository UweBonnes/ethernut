/*
 * Copyright (C) 2012, 2013, 2016 Uwe Bonnes.
 *                               (bon@elektron.ikp.physik.tu-darmstadt.de)
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
 * OR TORT (INCLUDING NEGLIGENce OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

/*!
 * \file app/owibus/owibus.c
 * \brief Example access to a DS18B20.
 *
 * The Onewire device needs to be connected to a port pin with pull-up
 * or the uart with TX low driving RX low and TX high not touching RX
 *
 * STM32 UART can do that by itself, with RX connected internal to TX
 * and TX driven Tristate
 *
 * \verbatim
 */

#include <string.h>
#include <stdio.h>
#include <io.h>

#include <cfg/arch.h>
#include <stdint.h>
#include <sys/timer.h>
#include <dev/board.h>
#include <dev/gpio.h>

#if !defined(__GNUC__) || !defined(DEF_OWIBUS)

int main(void)
{
    uint32_t baud = 115200;

    NutRegisterDevice(&DEV_CONSOLE, 0, 0);
    freopen(DEV_CONSOLE.dev_name, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);
#if !defined(__GNUC__)
    puts("This program requires a compiler that supports 64-bit integers.");
#else
    puts("This program requires a configured OWIBUS.");
#endif
    for (;;);
    return 0;
}

#else

#include <dev/owibus.h>

static char *banner = "\nNut/OS OWI Bus on " BOARDNAME
    " "__DATE__ " " __TIME__"\n";
/*
 * UART sample.
 *
 */
int main(void)
{

    uint32_t baud = 115200;
    FILE *uart;
    int res, i = 0;
    uint8_t hid[8]  = { 0 };
    int32_t xcelsius;
    int run =0;
    uint8_t raw[2];
    uint8_t diff;
    uint8_t family;

    NutRegisterDevice(&DEV_CONSOLE, 0, 0);

    uart = fopen(DEV_CONSOLE.dev_name, "r+");

    _ioctl(_fileno(uart), UART_SETSPEED, &baud);

    freopen(DEV_CONSOLE.dev_name, "w", stdout);
    fprintf(stdout, banner);


    res = OwiInit(&DEF_OWIBUS);
    if(res)
    {
        printf("Owi Error %d\n", res);
        while(1)
            NutSleep(10);
    }
    diff = OWI_SEARCH_FIRST;
    res = OwiRomSearch(&DEF_OWIBUS, &diff, hid);
    if(res)
    {
        printf("OwiRomSearch failed\n");
        while(1)
            NutSleep(10);
    }
    fprintf(stdout,
            "Hardware ID of first device %02x%02x%02x%02x%02x%02x%02x%02x\n",
            hid[7], hid[6], hid[5], hid[4], hid[3], hid[2], hid[1], hid[0]);
    family = hid[0];
    if ((family != W1_THERM_DS18B20) && (family != W1_THERM_DS18S20))
    {
        fprintf(stdout, "One-wire device found, but family not handled"
                "in this example\n");
        while(1) NutSleep(10);
    }
    while(1)
    {
        res = OwiCommand(&DEF_OWIBUS, OWI_CONVERT_T, NULL);
        if (res)
            printf("OwiCommand convert_t error %d\n", res);
        NutSleep(750);
        while (!(res = OwiReadBlock(&DEF_OWIBUS, &diff, 1)) && !diff && i < 100)
        {
            NutSleep(10);
            i++;
        }
        if (i)
        {
            printf(
                "Conversion took additional %d poll cycles\n",
                i);
            i = 0;
        }
        res = OwiCommand(&DEF_OWIBUS, OWI_READ, NULL);
        if (res)
            printf("OwiCommand read error %d\n", res);
        res = OwiReadBlock(&DEF_OWIBUS, raw, 16);
        if (res)
            printf("OwiReadBlock error %d\n", res);
        if (family == W1_THERM_DS18B20 )
            xcelsius = ((raw[1]<<8) | raw[0]) * (int32_t)625;
        else
            xcelsius = ((raw[1]<< 8) | raw[0]) * (int32_t)5000;
        fprintf(stdout, "Run %3d: Temp %ld.%04ld\r",
                run++, xcelsius/10000, xcelsius%10000);
    }
    return 0;
}

#endif
