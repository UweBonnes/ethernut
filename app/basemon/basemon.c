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
 */

/*
 * $Log$
 * Revision 1.2  2003/11/03 16:07:40  haraldkipp
 * Completely rewritten to support Ethernut 2
 *
 * Revision 1.1  2003/08/07 09:37:59  haraldkipp
 * First public check in
 *
 * Revision 1.15  2003/05/08 11:52:25  harald
 * *** empty log message ***
 *
 * Revision 1.14  2003/02/04 18:19:36  harald
 * Version 3 released
 *
 * Revision 1.13  2003/02/04 16:24:24  harald
 * Adapted to version 3
 *
 * Revision 1.12  2002/10/31 16:23:20  harald
 * Mods by troth for Linux
 *
 * Revision 1.11  2002/08/02 13:59:15  harald
 * *** empty log message ***
 *
 * Revision 1.10  2002/07/03 16:31:42  harald
 * Several new tests
 *
 * Revision 1.9  2002/06/26 17:28:59  harald
 * First pre-release with 2.4 stack
 *
 * Revision 1.8  2002/06/04 18:46:29  harald
 * SRAM test extended
 *
 * Revision 1.7  2002/05/11 20:58:35  harald
 * Using new standard output
 *
 * Revision 1.6  2002/05/08 16:02:27  harald
 * First Imagecraft compilation
 *
 * Revision 1.5  2002/04/21 17:12:01  harald
 * New html layout
 *
 * Revision 1.4  2001/09/02 10:36:45  harald
 * Configuration updated
 *
 * Revision 1.3  2001/09/01 12:56:21  harald
 * *** empty log message ***
 *
 * Revision 1.2  2001/08/10 18:20:08  harald
 * GCC version 3 update
 *
 * Revision 1.1  2001/06/28 18:36:42  harald
 * Preview release
 *
 */

/*!
 * \example basemon/basemon.c
 *
 * Basic hardware test monitor. Do not take this as a
 * typical application sample, because it contains
 * parts of the OS. It also uses a bunch of depricated
 * API calls. But it is still valid to do basic hardware
 * checks.
 */

#include <stdio.h>
#include <string.h>

#include "rtlregs.h"
#include <dev/debug.h>

#include <sys/version.h>
#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/confnet.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <net/if_var.h>

#include "realtek.h"
#include "smsc.h"
#include "webdemo.h"
#include "xmemtest.h"
#include "uart.h"
#include "utils.h"

#ifdef __GNUC__
void NutInit(void) __attribute__ ((naked)) __attribute__ ((section(".init8")));
extern int NutAppMain(void) __attribute__ ((noreturn));
#endif


int uart_bs;
u_char nic;

static char *version = "4.0.1";
static size_t sram;
static u_char banks;
static size_t banksize;

char my_ip[32];
char my_mask[32];
char my_gate[32];
u_char my_mac[32];

THREAD(idle, arg)
{
    NutTimerInit();
    NutThreadCreate("main", WebDemo, 0, 1384);
    NutThreadSetPriority(255);
    for (;;)
        NutThreadYield();
}

/*
 * Test external SRAM.
 */
int TestPorts(void)
{
    u_char pat;
    u_char ipat;
    u_char val;
    u_char bpat = 0;
    u_char dpat = 0;
    u_char epat = 0;
    u_char fpat = 0;

    /*
     * Port B.
     */
    for (pat = 1; pat; pat <<= 1) {
        ipat = ~pat;
        outb(DDRB, pat);
        outb(PORTB, ipat);
        Delay(100);
        if ((val = inb(PINB)) != ipat)
            bpat |= ~val;
    }
    outb(DDRB, 0);
    if (bpat)
        printf("PORTB bits %02X\n\x07", bpat);

    /*
     * Port D.
     */
    for (pat = 1; pat; pat <<= 1) {
        ipat = ~pat;
        outb(DDRD, pat);
        outb(PORTD, ipat);
        Delay(1000);
        if ((val = inb(PIND)) != ipat)
            dpat |= ~val;
    }
    outb(DDRD, 0);
    if (dpat)
        printf("PORTD bits %02X\n\x07", dpat);

    /*
     * Port E. Exclude PE0, PE1 and PE5.
     */
    for (pat = 4; pat; pat <<= 1) {
        if (pat == 0x20)
            continue;
        ipat = ~pat;
        outb(DDRE, pat);
        outb(PORTE, ipat);
        Delay(1000);
        if ((val = inb(PINE) | 0x23) != ipat) {
            epat |= ~val;
        }
    }
    outb(DDRE, 0);
    if (epat)
        printf("PORTE bits %02X\n\x07", epat);

#ifdef __AVR_ATmega128__
    /*
     * Port F. Exclude PF4, PF5, PF6 and PF7.
     */
    for (pat = 1; pat & 0x0F; pat <<= 1) {
        ipat = ~pat;
        outb(DDRF, pat);
        outb(PORTF, ipat);
        Delay(1000);
        if ((val = inb(PINF) | 0xF0) != ipat) {
            fpat |= ~val;
        }
    }
    outb(DDRF, 0);
    if (fpat)
        printf("PORTF bits %02X\n\x07", fpat);
#endif

    if (bpat || dpat || epat || fpat)
        return -1;
    return 0;
}

/*
 * Basic monitor function.
 */
void BaseMon(void)
{
    char ch;
    int i;
    int n;
    static prog_char menu1_P[] = 
        "\nPress any of the following keys:";
    static prog_char menu2_P[] = 
        "    B - Send broadcasts";
    static prog_char menu3_P[] =
        "    E - Ethernet controller read/write\n"
        "    J - Jump to bootloader\n"
        "    S - SRAM read/write";
    static prog_char menu4_P[] = 
        "    X - Exit BaseMon, configure network and start WebServer";

    /*
     * Use the debug UART driver for output. In opposite
     * to the standard UART driver, it uses non-buffered
     * polling mode, which is better suited to run on
     * untested hardware.
     */
    NutRegisterDevice(&devDebug0, 0, 0);
    uart_bs = DetectSpeed();
    freopen("uart0", "w", stdout);

    /*
     * Print banner.
     */
    printf("\n\nBaseMon %s\nNut/OS %s\n", version, NutVersionString());
    printf("Compiled by ");
#ifdef __IMAGECRAFT__
    printf("ICCAVR");
#else
    printf("AVRGCC");
#endif
    printf(" for ATmega");
#ifdef __AVR_ATmega128__
    puts("128");
#else
    puts("103");
#endif

    if (uart_bs >= 0)
        printf("Baudrate select = %d\n", uart_bs);

    /*
     * Test external SRAM.
     */
    printf("External RAM Test... ");
    sram = XMemTest();
    printf("%u bytes\n", sram);

    printf("Banked RAM Test...   ");
    banksize = sram;
    banks = XMemBankTest(&banksize);
    if(banks)
        printf("%u banks, %u bytes ea.\n", banks, banksize);
    else
        puts("none");

    /*
     * Test Ethernet controller hardware.
     */
    printf("Detecting NIC...     ");
    if(SmscDetect()) {
        if(RealtekDetect()) {
            nic = 0;
            puts("none\x07");
        }
        else {
            nic = 1;
            puts("RTL8019AS");
        }
    }
    else {
        nic = 2;
        puts("LAN91C111");
    }

    if(nic) {
        printf("Testing NIC...       ");
        if(nic == 1) {
            RealtekTest();
        }
        else {
            SmscTest();
        }
    }

    /*
     * Test Ethernet controller hardware.
     */
    printf("I/O Port Test...     ");
    if (TestPorts() == 0)
        puts("OK");
    /*
     * Return if running without serial port.
     */
    if (uart_bs < 0)
        return;
    for (;;) {
        for (;;) {
            while (GetChar());
            puts_P(menu1_P);
            if (sram)
                puts_P(menu2_P);
            puts_P(menu3_P);
            if (sram > 8191 && nic)
                puts_P(menu4_P);
            while ((ch = GetChar()) == 0 || ch == ' ');
            if (sram > 8191 && (ch == 'x' || ch == 'X'))
                break;
            if (sram && (ch == 'b' || ch == 'B')) {
                if(nic == 1)
                    RealtekSend();
                else
                    SmscSend();
            }
            else if (ch == 'e' || ch == 'E') {
                if(nic == 1) {
                    RealtekLoop();
                }
                else {
                    SmscLoop();
                }
            }
            else if (ch == 'j' || ch == 'J') {
                puts("Booting...");
                asm("jmp 0x1F000");
            }
            else if (ch == 's' || ch == 'S')
                LoopSRAM();
        }

        /*
         * Input MAC address.
         */
        for (;;) {
            printf("\nMAC address  (%02X%02X%02X%02X%02X%02X): ",
                   my_mac[0], my_mac[1], my_mac[2], my_mac[3], my_mac[4], my_mac[5]);
            GetLine(inbuff, sizeof(inbuff));
            if ((n = strlen(inbuff)) == 0)
                break;
            for (i = 0; i < n; i++)
                if (inbuff[i] < '0' ||
                    (inbuff[i] > '9' && inbuff[i] < 'A') || (inbuff[i] > 'F' && inbuff[i] < 'a') || inbuff[i] > 'f')
                    n = 13;
            if (n <= 12 && (n & 1) == 0) {
                n >>= 1;
                for (i = 0; i < n; i++)
                    my_mac[6 - n + i] = hex2bin(inbuff[i * 2]) * 16 + hex2bin(inbuff[i * 2 + 1]);
                break;
            }
            printf("Bad MAC address");
        }

        /*
         * Input IP address.
         */
        GetIP("IP address   ", my_ip);
        /*
         * Input netmask and gateway, if non-zero IP address.
         */
        if (inet_addr(my_ip)) {
            GetIP("Net mask     ", my_mask);
            GetIP("Default route", my_gate);
            if (inet_addr(my_gate) == 0 || (inet_addr(my_ip) & inet_addr(my_mask)) == (inet_addr(my_gate) & inet_addr(my_mask)))
                break;
        } else {
            printf("Using DHCP or ARP method (Y): ");
            GetLine(inbuff, sizeof(inbuff));
            if (strlen(inbuff) == 0 || inbuff[0] == 'Y' || inbuff[0] == 'y')
                break;
        }
    }
    puts("\n");
    confnet.cdn_cip_addr = inet_addr(my_ip);
    confnet.cdn_ip_mask = inet_addr(my_mask);
    confnet.cdn_gateway = inet_addr(my_gate);
	memcpy(confnet.cdn_mac, my_mac, sizeof(confnet.cdn_mac));
    NutNetSaveConfig();
}

void NutInit(void)
{
    extern void *__bss_end;

    /*
     * Switch off analog comparator to reduce power 
     * consumption. The comparator is switched on
     * after CPU reset.
     */
    sbi(ACSR, ACD);

    /*
     * Use the rest of our internal RAM for our heap. Re-opening
     * standard output will use malloc. We do not use any external
     * RAM before passing the memory test.
     */
    NutHeapAdd(&__bss_end, (uptr_t) RAMEND - 256 - (uptr_t) (&__bss_end));

    /*
     * Load network configuration from EEPROM.
     */
    NutNetLoadConfig("eth0");
    if ((confnet.cdn_mac[0] & confnet.cdn_mac[1] & confnet.cdn_mac[2]) == 0xFF) {
        u_char mac[] = {
            0x00, 0x06, 0x98, 0x00, 0x00, 0x00
        };
        memcpy(confnet.cdn_mac, mac, sizeof(confnet.cdn_mac));
    }
    strcpy(my_ip, inet_ntoa(confnet.cdn_cip_addr));
    strcpy(my_mask, inet_ntoa(confnet.cdn_ip_mask));
    strcpy(my_gate, inet_ntoa(confnet.cdn_gateway));
    memcpy(my_mac, confnet.cdn_mac, 6);

    /*
     * Perform basic monitor functions.
     */
    BaseMon();

    /*
     * Initialize heap and create the idle thread. This will in turn 
     * start the WebDemo thread.
     */
    if (sram) {
        NutHeapAdd((void *) (RAMEND + 1), sram);
        NutThreadCreate("idle", idle, 0, 384);
    }

    NutThreadSetPriority(128);
    for (;;)
        NutSleep(1000);
}

#ifdef __IMAGECRAFT__

void main(void)
{
    NutInit();
}

#endif
