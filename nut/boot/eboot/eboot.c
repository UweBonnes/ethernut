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
 * Revision 1.5  2007/11/15 01:35:34  hwmaier
 * Include wdt.h was missing to compile for ATmega2561
 *
 * Revision 1.4  2007/07/30 15:13:08  olereinhardt
 * Disable watchdog after reset for ATMega2561
 *
 * Revision 1.3  2007/07/30 09:47:55  olereinhardt
 * ATMega2561 port. Makedefs need to be modifies by hand (uncomment LDFLAGS
 * line and comment out LDFLAGS for mega128
 *
 * Revision 1.2  2004/09/10 10:10:17  haraldkipp
 * Removed unused bootp parameters
 *
 * Revision 1.1  2004/04/15 09:34:45  haraldkipp
 * Checked in
 *
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "config.h"
#include "debug.h"
#include "ether.h"
#include "dhcp.h"
#include "tftp.h"
#include "util.h"
#include "eboot.h"

BOOTFRAME sframe;
BOOTFRAME rframe;
u_long sid;

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
#if defined(__AVR_ATmega2561__)
    /* unlike ATMega128 the ATMega2561 does not disbale the watchdog */
    /* after a reset, so we need to do this here                     */
 
    MCUSR = 0;
    wdt_disable();
#endif

    /*
     * Enable external data and address bus.
     */
    MCUCR = _BV(SRE) | _BV(SRW);
    DEBUGINIT();
    DEBUG("\neboot 2.0.0\n");

    BootConfigRead();

    /*
     * Initialize the network interface controller hardware.
     */
    DEBUG("ETHERNET ");
    if (NicInit() == 0) {
        DEBUG("OK\n");

        /*
        * DHCP query and TFTP download.
        */
        if (DhcpQuery() == 0 && confboot.cb_image[0])
            TftpRecv();
    } else {
        DEBUG("No\n");
    }

    /*
     * Will jump to the application.
     */
    return 0;
}

/*@}*/
