/*
 * Copyright (C) 2002-2004 by egnite Software GmbH. All rights reserved.
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
 *
 */

/*
 * $Log$
 * Revision 1.1  2004/04/15 09:34:45  haraldkipp
 * Checked in
 *
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "ether.h"
#include "dhcp.h"
#include "tftp.h"
#include "util.h"
#include "eboot.h"

BOOTFRAME sframe;
BOOTFRAME rframe;
u_long netmask;
u_long broadcast;
u_long gateway;
u_long dns;
u_long sid;
u_long local_ip;
u_long server_ip;
u_char bootfile[128];

u_char mac[6] = { 0x00, 0x06, 0x98, 0x00, 0x00, 0x00 };


/*!
 * \addtogroup xgEBoot
 */
/*@{*/

/*!
 * \brief Boot loader entry.
 *
 * This boot loader is very special. It is completely self
 * contained, which means that it runs without any library.
 * This entry point must be linked first and will be located
 * at byte address 0x1F000 in the program flash ROM.
 *
 * \return Never, but jumps at absolute address 0 when done.
 */
int main(void)
{
    u_char *bp;
    u_long pp;

    UBRR0L = 7;
    UCSR0B = (1<<RXEN) | (1<<TXEN);

    /*
     * We are without runtime library, so we have
     * to initialize everything.
     */
    asm volatile ("clr r1");
    asm volatile ("cli");

    /*
     * Initialize the data segment.
     */
    pp = (u_long) & __data_load_start;
    for (bp = &__data_start; bp < &__data_end; bp++) {
        *bp = __ELPM(pp);
        pp++;
    }

    /*
     * Clear bss.
     */
    for (bp = &__bss_start; bp < &__bss_end; bp++)
        *bp = 0;

    /*
     * Initialize the network interface controller hardware.
     */
    NicInit();

    /*
     * DHCP query and TFTP download.
     */
    if (DhcpQuery() == 0 && bootfile[0])
        TftpRecv();

    /*
     * Will jump to the application.
     */
    asm volatile ("jmp 0");
    for (;;);
    return 0;
}

/*@}*/
