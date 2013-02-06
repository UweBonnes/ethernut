/*
 * Copyright (C) 2012 Uwe Bonnes. All rights reserved.
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

#if !defined(__GNUC__)

int main(void)
{
    uint32_t baud = 115200;

    NutRegisterDevice(&DEV_CONSOLE, 0, 0);
    freopen(DEV_CONSOLE.dev_name, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);
    puts("This program requires a compiler that supports 64-bit integers.");
    for (;;);
    return 0;
}

#else

#include <dev/owibus.h>

#if defined(OWI_UART)
#if !defined(USE_BB)
#define USE_UART
#endif
#else
#if defined(OWI_PORT) && defined(OWI_PIN)
#define USE_BB
#endif
#endif

static char *banner = "\nNut/OS OWI Bus "__DATE__ " " __TIME__"\n";
/*
 * UART sample.
 *
 */
int main(void)
{

    uint32_t baud = 115200;
    FILE *uart;
    int res, i = 0;
    uint64_t hid  = 0;
    int32_t xcelsius;
    int run =0;
    uint8_t raw[2];
    NUTOWIBUS bus_container, *bus=&bus_container;
    uint8_t diff;

    NutRegisterDevice(&DEV_CONSOLE, 0, 0);

    uart = fopen(DEV_CONSOLE.dev_name, "r+");

    _ioctl(_fileno(uart), UART_SETSPEED, &baud);

    freopen(DEV_CONSOLE.dev_name, "w", stdout);
    fprintf(stdout, banner);

#if  !defined(USE_BB) && !defined(USE_UART)
    fprintf(stdout,
            "Please defined the access method to the OWI BUS(Uart/Bitbang)\n");
    while(1) NutSleep(10);

#elif defined(USE_BB)
 #if !defined(OWI_PORT) || !defined (OWI_PIN)
    fprintf(stdout, "Please defined the Port/Pin to use for One-Wire "
            "Bus on your board\n");
    while(1) NutSleep(10);
 #else
    fprintf(stdout, "Using Bitbang PORT %lx PIN %x\n", OWI_PORT, OWI_PIN);
    fprintf(stdout,
            "Make sure no other (floating) pin results in the OWI "
            "line pulled low\n");
    /* E.g. with UART RX line used as bitbang OWI driver, the USART TX
     * line normally driving the OWI bus via the enable of a 3-state driver
     * needs to be pulled up. E.g for AVR Uart0:
     */
    GpioPinConfigSet(OWI_PORT, OWI_PIN +1 , GPIO_CFG_PULLUP);
    res= NutRegisterOwiBus_BB(bus, OWI_PORT, OWI_PIN, 0, 0);
 #endif
#elif defined(USE_UART)
 #if !defined(OWI_UART)
    fprintf(stdout, "Please defined UART to use for One-Wire Bus\n");
    while(1) NutSleep(10);
 #else
    res= NutRegisterOwiBus_Uart(bus, &OWI_UART, 0, 0);
    fprintf(stdout, "Using UART");

    /* Switch to Open Drain */
 #if defined(MCU_STM32) && defined(OWI_PORT) && defined(OWI_PIN)
    /* Switch to Open Drain */
  #if defined(MCU_STM32F1)
    CM3BBREG(OWI_PORT, GPIO_TypeDef, CRL, 4*(OWI_PIN*4)) = 1;
  #else
    CM3BBREG(OWI_PORT, GPIO_TypeDef, OTYPER, OWI_PIN) = 1;
  #endif
/*   Switch to Half Duplex */
    CM3BBREG((devUsartStm32_1.dev_base), USART_TypeDef, CR3,
             _BI32(USART_CR3_HDSEL))=1;
    fprintf(stdout, " with RX connected internal to TX and TX "
            "configured as Open Drain\n");
 #else
    fprintf(stdout, "Make sure TX drives the OWI device as Open Drain "
            "and RX is connected to OWI\n");
 #endif
    if (res)
    {
        fprintf(stdout, "NutRegisterOwiBus_Timer failed %d\n", res);
        while(1)
            NutSleep(100);
    }
 #endif
#endif
    diff = OWI_SEARCH_FIRST;
    res = OwiRomSearch(bus, &diff, &hid);
    if(res)
    {
        printf("OwiRomSearch failed\n");
        while(1)
            NutSleep(10);
    }
    fprintf(stdout, "Hardware ID of first device %08lx%08lx\n",
            (uint32_t)(hid>>32),
            (uint32_t)(hid &0xffffffff));
    if ((hid & 0xff) != W1_THERM_DS18B20)
    {
        fprintf(stdout, "One-wire device found, but family not handled"
                "in this example\n");
        while(1) NutSleep(10);
    }
    while(1)
    {
        res = OwiCommand( bus, OWI_CONVERT_T, NULL);
        if (res)
            printf("OwiCommand convert_t error %d\n", res);
        NutSleep(750);
        while (!(res = OwiReadBlock(bus, &diff, 1)) && !diff && i < 100)
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
        res = OwiCommand( bus, OWI_READ, NULL);
        if (res)
            printf("OwiCommand read error %d\n", res);
        res = OwiReadBlock(bus, raw, 16);
        if (res)
            printf("OwiReadBlock error %d\n", res);
        xcelsius = (raw[1]<<8 | raw[0]) * (int32_t)625;
        fprintf(stdout, "Run %3d: Temp %ld.%04ld\r",
                run++, xcelsius/10000, xcelsius%10000);
    }
    return 0;
}

#endif
