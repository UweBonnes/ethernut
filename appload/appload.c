/*
 * Copyright (C) 2003 by egnite Software GmbH. All rights reserved.
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
 */

/*
 * $Log$
 * Revision 1.2  2004/02/20 12:31:36  haraldkipp
 * Ignore target IP if local IP is not configured
 *
 * Revision 1.1  2003/11/03 16:19:38  haraldkipp
 * First release
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>

#include "debug.h"
#include "ether.h"
#include "dhcp.h"
#include "appload.h"

BOOTFRAME sframe;
BOOTFRAME rframe;

u_long my_netmask;
u_long my_ip;
u_long server_ip;
u_char bootfile[128];

/*
 * Internal compiler address values.
 */
extern unsigned char __data_start;
extern unsigned char __data_end;
extern unsigned char __data_load_start;

extern unsigned char __bss_start;
extern unsigned char __bss_end;

CONFNET confnet;

void eeprom_read_block(void *buf, const void *addr, size_t n) __attribute__ ((naked));

/*
 * Application loader entry point.
 *
 * Remember that we are running without any initialization or library 
 * code. All CPU registers and global variables are uninitialized.
 *
 * No code is allowed above this point.
 *
 */
int main(void)
{
    u_char *bp;

    /*
     * GCC depends on register r1 set to 0.
     */
    asm volatile ("clr r1");

    /*
     * No interrupts used.
     */
    cli();

    /*
     * Enable external data and address
     * bus.
     */
    outp(BV(SRE) | BV(SRW), MCUCR);

    /*
     * There's no data to initialize. However, if debugging
     * is enabled, we want to print out string literals and
     * need data initialization.
     */
#ifdef NUTDEBUG
    {
        /*
         * Initialize the data segment.
         */
        u_long pp = (u_long) & __data_load_start;
        for (bp = &__data_start; bp < &__data_end; bp++) {
            *bp = __ELPM(pp);
            pp++;
        }
    }
#endif

    /*
     * Clear bss.
     */
    for (bp = &__bss_start; bp < &__bss_end; bp++) {
        *bp = 0;
    }

#ifdef NUTDEBUG
    DebugInit();
    Debug("\nAppLoad 1.0.3\n");
#endif

    /*
     * Read network configuration from EEPROM.
     * Kindly contributed by Oliver Schulz.
     */
    eeprom_read_block(&confnet, (void *) CONFNET_EE_OFFSET, sizeof(CONFNET));
    if (confnet.cd_size != sizeof(CONFNET))
        asm volatile(
        "st z+, __zero_reg__"           "\n\t"
        "ldi r24, 0x06"                 "\n\t"
        "st z+, r24"                    "\n\t"
        "ldi r24, 0x98"                 "\n\t"
        "st z+, r24"                    "\n\t"
        "st z+, __zero_reg__"           "\n\t"
        "st z+, __zero_reg__"           "\n\t"
        "st z+, __zero_reg__"           "\n\t"
        ::"z" (confnet.cdn_mac):"r24");

    /*
     * Initialize the network interface controller hardware.
     */
    if(EtherInit() == 0) {

        /*
         * DHCP query and TFTP download.
         */
        if (DhcpQuery() == 0 && bootfile[0]) {
            TftpRecv();
        }
    }
#ifdef NUTDEBUG
    else {
        Debug("EtherInit failed\n");
    }
#endif

    /*
     * Will jump into the loaded code.
     */
    asm volatile ("jmp 0");

    /*
     * Never return to stop GCC to include a reference to the exit code.
     * Actually we will never reach this point, but the compiler doesn't 
     * understand the assembly statement above.
     */
    for (;;);
}

/*!
 * \brief Delay by executing a given number of nops.
 */
void MicroDelay(u_short nops)
{
    u_short i;

    for (i = 0; i < nops; i++) {
        asm volatile ("nop");
    }
}

/*!
 * \brief Delay by executing a given number of nops, multiplied by 100000.
 */
void Delay(u_char val)
{
    u_char i;

    for(i = 0; i < val; i++) {
        MicroDelay(0xFFFF);
        MicroDelay(0xFFFF);
        MicroDelay(0xFFFF);
    }
}

/*
 * \brief Read a block from EEPROM.
 *
 * I assume, that Oliver took it from avrlibc.
 */
void eeprom_read_block( void *buf __attribute__ ((unused)),
                        const void *addr __attribute__ ((unused)),
                        size_t n  __attribute__ ((unused)))
{
        asm volatile(
        "movw   r26, r24"               "\n\t"
        ".Lloop:"                       "\n\t"
        "out    %2, r23"                "\n\t"
        "out    %3, r22"                "\n\t"
        "sbi    %0, %4"                 "\n\t"
        "subi   r22, lo8(-1)"           "\n\t"
        "sbci   r23, hi8(-1)"           "\n\t"
        "in     __tmp_reg__, %5"        "\n\t"
        "st     X+, __tmp_reg__"        "\n\t"
        "subi   r20, lo8(1)"            "\n\t"
        "sbci   r21, hi8(1)"            "\n\t"
        "brne   .Lloop"                 "\n\t"
        "ret"
        ::"I" (_SFR_IO_ADDR(EECR)), "I" (EEWE),
          "I" (_SFR_IO_ADDR(EEARH)), "I" (_SFR_IO_ADDR(EEARL)),
          "I" (EERE), "I" (_SFR_IO_ADDR(EEDR))  );
}
