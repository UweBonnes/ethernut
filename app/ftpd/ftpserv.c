/*
 * Copyright (C) 2005 by egnite Software GmbH. All rights reserved.
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

/*!
 * $Log$
 * Revision 1.2  2005/02/07 19:05:23  haraldkipp
 * ATmega 103 compile errors fixed
 *
 * Revision 1.1  2005/02/05 20:32:57  haraldkipp
 * First release
 *
 */

#include <stdio.h>
#include <io.h>

#include <dev/lanc111.h>
#include <dev/debug.h>
#include <dev/pnut.h>

#include <sys/confnet.h>
#include <sys/version.h>
#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <net/route.h>
#include <pro/dhcp.h>
#include <pro/ftpd.h>
#include <pro/wins.h>

/* Determine the compiler. */
#if defined(__IMAGECRAFT__)
#define CC_STRING   "ICCAVR"
#elif defined(__GNUC__)
#define CC_STRING   "AVRGCC"
#else
#define CC_STRING   "Compiler unknown"
#endif

/*!
 * \example ftpd/ftpserv.c
 *
 * FTP server sample.
 *
 * This application requires the PNUT file system, which is
 * not available on Ethernut 1.x or Charon II.
 */

/* 
 * Device for debug output. 
 */
#define DBG_DEVICE devDebug0

/* 
 * Device name for debug output. 
 */
#define DBG_DEVNAME "uart0"

/* 
 * Baudrate for debug output. 
 */
#define DBG_BAUDRATE 115200

/* 
 * Unique MAC address of the Ethernut Board. 
 *
 * Ignored if EEPROM contains a valid configuration.
 */
#define MY_MAC { 0x00, 0x06, 0x98, 0x10, 0x01, 0x10 }

/* 
 * Unique IP address of the Ethernut Board. 
 *
 * Ignored if DHCP is used. 
 */
#define MY_IPADDR "192.168.192.100"

/* 
 * IP network mask of the Ethernut Board.
 *
 * Ignored if DHCP is used. 
 */
#define MY_IPMASK "255.255.255.0"

/* 
 * Gateway IP address for the Ethernut Board.
 *
 * Ignored if DHCP is used. 
 */
#define MY_IPGATE "192.168.192.1"

/* 
 * NetBIOS name.
 *
 * Use a symbolic name with Win32 Explorer.
 */
//#define MY_WINSNAME "ETHERNUT"

/*
 * FTP port number.
 */
#define FTP_PORTNUM 21

/*
 * FTP timeout.
 *
 * The server will terminate the session, if no new command is received
 * within the specified number of milliseconds.
 */
#define FTPD_TIMEOUT 600000

/*
 * TCP buffer size.
 */
#define TCPIP_BUFSIZ 5840

/*
 * Maximum segment size. 
 *
 * Choose 536 up to 1460. Note, that segment sizes above 536 may result 
 * in fragmented packets. Remember, that Ethernut doesn't support TCP 
 * fragmentation.
 */
#define TCPIP_MSS 1460

/*
 * FTP service.
 *
 * This function waits for client connect, processes the FTP request 
 * and disconnects. Nut/Net doesn't support a server backlog. If one 
 * client has established a connection, further connect attempts will 
 * be rejected. 
 *
 * Some FTP clients, like the Win32 Explorer, open more than one 
 * connection for background processing. So we run this routine by
 * several threads.
 */
void FtpService(void)
{
    TCPSOCKET *sock;

    /*
     * Create a socket.
     */
    if ((sock = NutTcpCreateSocket()) != 0) {

        /* 
         * Set specified socket options. 
         */
#ifdef TCPIP_MSS
        {
            u_short mss = TCPIP_MSS;
            NutTcpSetSockOpt(sock, TCP_MAXSEG, &mss, sizeof(mss));
        }
#endif
#ifdef FTPD_TIMEOUT
        {
            u_long tmo = FTPD_TIMEOUT;
            NutTcpSetSockOpt(sock, SO_RCVTIMEO, &tmo, sizeof(tmo));
        }
#endif
#ifdef TCPIP_BUFSIZ
        {
            u_short siz = TCPIP_BUFSIZ;
            NutTcpSetSockOpt(sock, SO_RCVBUF, &siz, sizeof(siz));
        }
#endif

        /*
         * Listen on our port. If we return, we got a client.
         */
        printf("\nWaiting for an FTP client...");
        if (NutTcpAccept(sock, FTP_PORTNUM) == 0) {
            printf("Connected, %u bytes free\n", NutHeapAvailable());
            NutFtpServerSession(sock);
            printf("Disconnected, %u bytes free\n", NutHeapAvailable());
        } else {
            puts("Accept failed");
        }

        /*
         * Close our socket.
         */
        NutTcpCloseSocket(sock);
    }
}

/*
 * FTP service thread.
 */
THREAD(FtpThread, arg)
{
    /* Loop endless for connections. */
    for (;;) {
        FtpService();
    }
}

/*
 * Assign stdout to the UART device.
 */
void InitDebugDevice(void)
{
    u_long baud = DBG_BAUDRATE;

    NutRegisterDevice(&DBG_DEVICE, 0, 0);
    freopen(DBG_DEVNAME, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);
}

/*
 * Setup the ethernet device. Try DHCP first. If this is
 * the first time boot with empty EEPROM and no DHCP server
 * was found, use hardcoded values.
 */
void InitEthernetDevice(void)
{
#ifdef __AVR_ATmega128__
    NutRegisterDevice(&DEV_ETHER, 0x8300, 5);
    printf("Configure eth0...");
    if (NutDhcpIfConfig("eth0", 0, 60000)) {
        u_char mac[6] = MY_MAC;

        printf("initial boot...");
        if (NutDhcpIfConfig("eth0", mac, 60000)) {
            u_long ip_addr = inet_addr(MY_IPADDR);
            u_long ip_mask = inet_addr(MY_IPMASK);
            u_long ip_gate = inet_addr(MY_IPGATE);

            printf("no DHCP...");
            NutNetIfConfig("eth0", mac, ip_addr, ip_mask);
            /* Without DHCP we had to set the default gateway manually.*/
            if(ip_gate) {
                printf("hard coded gate...");
                NutIpRouteAdd(0, 0, ip_gate, &DEV_ETHER);
            }
        }
    }
    puts("OK");
#endif
}

/*
 * Main application routine. 
 *
 * Nut/OS automatically calls this entry after initialization.
 */
int main(void)
{
    /* Initialize a debug output device and print a banner. */
    InitDebugDevice();
    printf("\n\nFTP Server Sample - Nut/OS %s - " CC_STRING "\n", NutVersionString());

    /* Initialize the Peanut file system. */
    printf("Register Peanut...");
    if (NutRegisterDevice(&devPnut, 0, 0)) {
        puts("failed");
        for (;;);
    }
    puts("OK");

    /* Initialize the Ethernet device and print our IP address. */
    InitEthernetDevice();
    printf("IP: %s\n", inet_ntoa(confnet.cdn_ip_addr));

    /* Start two additional server threads. */
    NutThreadCreate("ftpd0", FtpThread, 0, 640);
    NutThreadCreate("ftpd1", FtpThread, 0, 640);

    /* Main server thread. */
    for (;;) {
#ifdef MY_WINSNAME
        NutWinsNameQuery(MY_WINSNAME, confnet.cdn_ip_addr);
#endif
        FtpService();
    }
}
