/*
 * Copyright (C) 2005-2007 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.1  2007/08/17 13:16:31  haraldkipp
 * Checked in.
 *
 */

#include "utils.h"
#include "uart.h"
#include "dialog.h"
#include "ether.h"
#include "dhcp.h"
#include "bootmon.h"

BOOTFRAME sframe;
BOOTFRAME rframe;

static unsigned char my_mac[32];
static char my_ip[32];
static char my_mask[32];
static char my_gate[32];
static char my_tftpd[32];
static char my_image[64];

static int UserEntry(void)
{
    int n;
    unsigned char yn[3];

    PutString("\nBootMon 1.0.4\n");

    memcpy_(my_mac, confnet.cdn_mac, 6);
    strcpy_(my_ip, inet_ntoa(confnet.cdn_cip_addr));
    strcpy_(my_mask, inet_ntoa(confnet.cdn_ip_mask));
    strcpy_(my_gate, inet_ntoa(confnet.cdn_gateway));
    strcpy_(my_tftpd, inet_ntoa(confboot.cb_tftp_ip));
    strcpy_(my_image, confboot.cb_image);
    for (;;) {
        GetMac(my_mac);
        GetIP("IP address", my_ip);

        if (inet_addr(my_ip)) {
            GetIP("Net mask", my_mask);
            GetIP("Default route", my_gate);
        }
        GetIP("TFTP IP", my_tftpd);
        if (inet_addr(my_ip)) {
            PutString("TFTP Image (");
            PutString(my_image);
            PutString("): ");
            if (GetLine(my_image, 31) == 0) {
                strcpy_(my_image, confboot.cb_image);
            }
        } else {
            my_image[0] = 0;
        }
        if (inet_addr(my_ip) == 0) {
            PutString("Using DHCP (Y): ");
            n = GetLine(yn, 2);
            if (n == 0 || yn[0] == 'Y' || yn[0] == 'y') {
                break;
            }
        } else if (inet_addr(my_gate) == 0 || (inet_addr(my_ip) & inet_addr(my_mask)) == (inet_addr(my_gate) & inet_addr(my_mask))) {
            break;
        }
    }
    PutString("\n");

    memcpy_(confnet.cdn_mac, my_mac, sizeof(confnet.cdn_mac));
    confnet.cdn_cip_addr = inet_addr(my_ip);
    confnet.cdn_ip_mask = inet_addr(my_mask);
    confnet.cdn_gateway = inet_addr(my_gate);

    confboot.cb_tftp_ip = inet_addr(my_tftpd);
    DEBUG("Store '");
    DEBUG(my_image);
    DEBUG("'\n");
    strcpy_(confboot.cb_image, my_image);

    BootConfigWrite();

    return 0;
}

/*
 * Bootloader entry point.
 */
int main(void)
{
    int i;
    unsigned long *flash_app = 0x10020000;

    /* Initialize RS232 port. */
    UartInit();

    /* Read configuration from EEPROM. */
    BootConfigRead();

    /*
     * Loop until a valid image is loaded.
     */
    for (;;) {
        /*
         * Give user a chance to enter a space and a new configuration.
         */
        for (i = 0; i < 20; i++) {
            if (UartRxWait(6000) == 0) {
                if (UartRx() == ' ') {
                    UserEntry();
                    break;
                }
            }
        }

        if (confboot.cb_tftp_ip == 0 && *flash_app == 0xE59FF018) {
            DEBUG("\nStarting Flashed Application\n");
            asm volatile ("@ Start Application" "\n\t"  /* */
                          "ldr r0, =0x10020000" "\n\t"   /* */
                          "bx  r0" "\n\t"       /* */
                          :::"r0"       /* */
            );
        }

        /*
         * Initialize the network interface controller hardware.
         */
        DEBUG("\nETHERNET ");
        if (EtherInit() == 0) {
            DEBUG("OK\n");

            /*
             * DHCP query and TFTP download.
             */
            if (DhcpQuery() == 0) {
                if (TftpRecv() == 0) {
                    break;
                }
            }
        } else {
            DEBUG("No\n");
        }
    }
#ifdef NUTDEBUG
    /* Avoids UART garbage. */
    Delay(5);
#endif
    asm volatile ("@ Start Application" "\n\t"  /* */
                  "ldr r0, =0" "\n\t"   /* */
                  "bx  r0" "\n\t"       /* */
                  :::"r0"       /* */
        );
    return 0;
}
