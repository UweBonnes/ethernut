/*!
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

/*!
 * $Log$
 * Revision 1.2  2003/11/04 17:46:52  haraldkipp
 * Adapted to Ethernut 2
 *
 * Revision 1.1  2003/08/05 18:59:05  haraldkipp
 * Release 3.3 update
 *
 * Revision 1.7  2003/02/04 18:19:39  harald
 * Version 3 released
 *
 * Revision 1.6  2003/02/04 16:24:34  harald
 * Adapted to version 3
 *
 * Revision 1.5  2002/10/31 16:25:48  harald
 * Mods by troth for Linux
 *
 * Revision 1.3  2002/06/26 17:29:03  harald
 * First pre-release with 2.4 stack
 *
 * Revision 1.2  2002/06/12 11:22:55  harald
 * *** empty log message ***
 *
 * Revision 1.1  2002/06/04 19:00:49  harald
 * First check in
 *
 */

/*!
 * \example rs232d/rs232d.c
 *
 * Simple RS232 server. Use a serial cable to connect the RS232 port
 * of the Ethernut Board with a COM port of a PC. Start a terminal
 * program and a telnet client on the PC. Telnet should connect to
 * the Ethernut Board.
 *
 * Characters typed in the telnet window will appear in the terminal
 * program window and vice versa. Baudrate is 9600.
 *
 */

#ifdef ETHERNUT2
#include <dev/lanc111.h>
#else
#include <dev/nicrtl.h>
#endif
#include <dev/uartavr.h>

#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/print.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include <netinet/sostream.h>
#include <arpa/inet.h>

#include <pro/dhcp.h>

#define BUFFERSIZE  128
#define TCPPORT     23

typedef struct {
    FILE *cd_rs232;
    FILE *cd_tcpip;
    char cd_connected;
} CHANNEL;

/*
 * Transfer data from input stream to output stream.
 */
void StreamCopy(FILE * istream, FILE * ostream, char *cop)
{
    int cnt;
    char *buff;

    buff = malloc(BUFFERSIZE);
    while (*cop) {
        if ((cnt = fread(buff, 1, BUFFERSIZE, istream)) <= 0)
            break;
        if (*cop && (cnt = fwrite(buff, 1, cnt, ostream)) <= 0)
            break;
        if (*cop && fflush(ostream))
            break;
    }
    *cop = 0;
    free(buff);
}

/*
 * From RS232 to socket.
 */
THREAD(Receiver, arg)
{
    CHANNEL *cdp = arg;

    for (;;) {
        if (cdp->cd_connected) {
            NutThreadSetPriority(64);
            /*
             * We are reading from the UART without any timeout. So we
             * won't return immediately when disconnected.
             */
            StreamCopy(cdp->cd_rs232, cdp->cd_tcpip, &cdp->cd_connected);
            NutThreadSetPriority(128);
        }
        NutThreadYield();
    }
}

/*
 * Main application routine. 
 *
 * Nut/OS automatically calls this entry after initialization.
 */
int main(void)
{
    TCPSOCKET *sock;
    CHANNEL cd;
    u_long baud = 9600;

    /*
     * Register our devices.
     */
    NutRegisterDevice(&devUart0, 0, 0);
    NutRegisterDevice(&DEV_ETHER, 0x8300, 5);

    /*
     * Setup the uart device.
     */
    cd.cd_rs232 = fopen("uart0", "r+b");
    _ioctl(_fileno(cd.cd_rs232), UART_SETSPEED, &baud);

    /*
     * Setup the ethernet device. Try DHCP first. If this is
     * the first time boot with empty EEPROM and no DHCP server
     * was found, use hardcoded values.
     */
    if (NutDhcpIfConfig("eth0", 0, 60000)) {
        /* No valid EEPROM contents, use hard coded MAC. */
        u_char my_mac[] = { 0x00, 0x06, 0x98, 0x20, 0x00, 0x00 };

        if (NutDhcpIfConfig("eth0", my_mac, 60000)) {
            /* No DHCP server found, use hard coded IP address. */
            u_long ip_addr = inet_addr("192.168.192.100");
            u_long ip_mask = inet_addr("255.255.255.0");

            NutNetIfConfig("eth0", my_mac, ip_addr, ip_mask);
            /* If not in a local network, we must also call 
               NutIpRouteAdd() to configure the routing. */
        }
    }

    /*
     * Start a RS232 receiver thread.
     */
    NutThreadCreate("xmit", Receiver, &cd, 512);

    /*
     * Now loop endless for connections.
     */
    cd.cd_connected = 0;
    for (;;) {
        /*
         * Create a socket and listen for a client.
         */
        sock = NutTcpCreateSocket();
        NutTcpAccept(sock, TCPPORT);

        /*
         * Open a stdio stream assigned to the connected socket.
         */
        cd.cd_tcpip = _fdopen((int) sock, "r+b");
        cd.cd_connected = 1;

        /*
         * Call RS232 transmit routine. On return we will be
         * disconnected again.
         */
        StreamCopy(cd.cd_tcpip, cd.cd_rs232, &cd.cd_connected);

        /*
         * Close the stream.
         */
        fclose(cd.cd_tcpip);

        /*
         * Close our socket.
         */
        NutTcpCloseSocket(sock);
    }
}
