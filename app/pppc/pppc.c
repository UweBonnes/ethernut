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
 * For additional information see http://www.ethernut.de/
 *
 */

/*!
 * $Log$
 * Revision 1.1  2003/08/14 14:57:07  haraldkipp
 * First release
 *
 */

/*!
 * \example pppc/pppc.c
 *
 * PPP client. Similar to the TCP server sample, but uses PPP over RS232.
 *
 * The default settings in this sample may be used to connect to a RAS
 * server of a Windows PC. When adding a similar modem script, it will
 * also work with a Linux PC nearly out of the box. At least you need
 * to change the PPPUSER and PPPPASS.
 *
 * \bug This sample works with ICCAVR (6.28 tested) only with debugging
 *      enabled.
 */

/*
 * PPP user and password.
 */
#define PPPUSER     "me"
#define PPPPASS     "secret"

/*
 * The Nut/OS modem chat utility works similar to the the UNIX
 * chat script. This one is used to connect to a Windows PC
 * using a direct cable.
 */
#define PPPCHAT     "TIMEOUT 2 '' CLIENT\\c CLIENTSERVER"

/*
 * A very simple modem script.
 */
//#define PPPCHAT     "'' AT OK ATD555 CONNECT"


/*
 * PPP device settings.
 */
//#define PPPDEV      devAhdlc0   /* Use HDLC driver. */
#define PPPDEV      devUart0    /* Use standard UART driver. */
#define PPPCOM      "uart0"     /* Physical device name. */
#define PPPSPEED    115200      /* Baudrate. */
#define PPPRXTO     1000        /* Character receive timeout. */


/*
 * Debug output device settings.
 */
#define DBGDEV      devDebug1   /* Use debug driver. */
//#define DBGDEV      devUart1    /* Use standard UART driver. */
#define DBGCOM      "uart1"     /* Device name. */
#define DBGSPEED    115200      /* Baudrate. */

/*
 * Server input buffer size.
 */
#define RXBUFFSIZE  256

#include <dev/debug.h>
#include <dev/hd44780.h>
//#include <dev/ahdlcavr.h>
#include <dev/uartavr.h>
#include <dev/ppp.h>
#include <dev/chat.h>

#include <sys/version.h>
#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/socket.h>

#include <sys/timer.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <net/if_var.h>
#include <net/route.h>

#ifdef NUTDEBUG
#include <net/netdebug.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#if defined(__IMAGECRAFT__)
#define CC_STRING   "ICCAVR"
#elif defined(__GNUC__)
#define CC_STRING   "AVRGCC"
#else
#define CC_STRING   "Compiler unknown"
#endif

prog_char vbanner_P[] = "\n\nPPP Client Sample - Nut/OS %s - " CC_STRING "\n";
prog_char banner_P[] = "200 Welcome to tcps. Type help to get help.\r\n";
prog_char help_P[] = "400 List of commands follows\r\n"
    "m[emory]\tQueries number of RAM bytes free.\r\n"
    "t[hreads]\tLists all created threads.\r\n"
    "ti[mers]\tLists all running timers.\r\n" "q[uit]\t\tTerminates connection.\r\n" ".\r\n";
prog_char thread_intro_P[] = "220 List of threads with name,state,prio,stack,mem,timeout follows\r\n";
prog_char timer_intro_P[] = "221 List of timers with ticks left and interval follows\r\n";
prog_char mem_fmt_P[] = "210 %u bytes RAM free\r\n";


/*
 * Process client requests.
 */
void ProcessRequests(FILE * stream)
{
    int got;
    char *cp;
    char *buff;

    /*
     * We allocate the input buffer from heap memory.
     */
    buff = malloc(RXBUFFSIZE);

    /*
     * Send a welcome banner.
     */
    fputs_P(banner_P, stream);
    for (;;) {

        /*
         * Flush output and read a line.
         */
        fflush(stream);
        if (fgets(buff, RXBUFFSIZE, stream) == 0)
            break;

        /*
         * Chop off EOL.
         */
        if ((cp = strchr(buff, '\r')) != 0)
            *cp = 0;
        if ((cp = strchr(buff, '\n')) != 0)
            *cp = 0;

        /*
         * Ignore blank lines.
         */
        got = strlen(buff);
        if (got == 0)
            continue;

        /*
         * Memory info.
         */
        if (strncmp(buff, "memory", got) == 0) {
            fprintf_P(stream, mem_fmt_P, NutHeapAvailable());
            continue;
        }

        /*
         * List threads.
         */
        if (strncmp(buff, "threads", got) == 0) {
            NUTTHREADINFO *tdp;
            NUTTIMERINFO *tnp;

            fputs_P(thread_intro_P, stream);
            for (tdp = nutThreadList; tdp; tdp = tdp->td_next) {
                fputs(tdp->td_name, stream);
                switch (tdp->td_state) {
                case TDS_TERM:
                    fputs("\tTerm\t", stream);
                    break;
                case TDS_RUNNING:
                    fputs("\tRun\t", stream);
                    break;
                case TDS_READY:
                    fputs("\tReady\t", stream);
                    break;
                case TDS_SLEEP:
                    fputs("\tSleep\t", stream);
                    break;
                }
                fprintf(stream, "%u\t%u", tdp->td_priority, (u_short) tdp->td_sp - (u_short) tdp->td_memory);
                if (*((u_long *) tdp->td_memory) != DEADBEEF)
                    fputs("\tCorrupted\t", stream);
                else
                    fputs("\tOK\t", stream);

                if ((tnp = (NUTTIMERINFO *) tdp->td_timer) != 0)
                    fprintf(stream, "%lu\r\n", tnp->tn_ticks_left);
                else
                    fputs("None\r\n", stream);
            }
            fputs(".\r\n", stream);
            continue;
        }

        /*
         * List timers.
         */
        if (strncmp("timers", buff, got) == 0) {
            NUTTIMERINFO *tnp;

            fputs_P(timer_intro_P, stream);
            for (tnp = nutTimerList; tnp; tnp = tnp->tn_next) {
                fprintf(stream, "%lu\t", tnp->tn_ticks_left);
                if (tnp->tn_ticks)
                    fprintf(stream, "%lu\r\n", tnp->tn_ticks);
                else
                    fputs("Oneshot\r\n", stream);
            }
            fputs(".\r\n", stream);
            continue;
        }

        /*
         * Quit connection.
         */
        if (strncmp("quit", buff, got) == 0) {
            break;
        }

        /*
         * Display help text on any unknown command.
         */
        fputs_P(help_P, stream);
    }
}

/*
 * PPP client application entry.
 */
int main(void)
{
    int pppcom;
    PPPDCB *dcb;
    u_long lctl;
    int rc;

    /*
     * Register our devices.
     */
    NutRegisterDevice(&DBGDEV, 0, 0);
    NutRegisterDevice(&PPPDEV, 0, 0);
    NutRegisterDevice(&devPpp, 0, 0);

    /*
     * Open debug device for standard output.
     */
    freopen("uart1", "w", stdout);

    /*
     * Set debug output speed.
     */
    lctl = DBGSPEED;
    _ioctl(_fileno(stdout), UART_SETSPEED, &lctl);

    /*
     * Display banner including compiler info and Nut/OS version.
     */
    printf_P(vbanner_P, NutVersionString());

    /*
     * Open PPP device. Specify physical device, user and password.
     */
    printf("Open uart...");
    if ((pppcom = _open("ppp:" PPPCOM "/" PPPUSER "/" PPPPASS, _O_RDWR | _O_BINARY)) == -1) {
        printf("Failed to open " PPPCOM "\n");
        for (;;);
    }
    puts("done");

    /*
     * Set PPP line speed.
     */
    lctl = PPPSPEED;
    _ioctl(pppcom, UART_SETSPEED, &lctl);

    /*
     * The PPP driver doesn't set any receive timeout, but
     * may require it.
     */
    lctl = PPPRXTO;
    _ioctl(pppcom, UART_SETREADTIMEOUT, &lctl);

#ifdef NUTDEBUG
    /*
     * Optionally enable PPP trace.
     */
    NutTracePPP(stdout, 1);
#endif

    /*
     * This delay may be removed. It is quite helpful during development.
     */
    NutSleep(5000);

    /*
     * PPP connection loop.
     */
    for (;;) {
        /*
         * Connect using a chat script. We may also set any
         * required hardware handshake line at this stage.
         */
        printf("Connecting...");
        if ((rc = NutChat(pppcom, PPPCHAT)) != 0) {
            printf("no connect, reason = %d\n", rc);
            continue;
        }
        puts("done");

        /*
         * We are connected, configure our PPP network interface.
         * This will initiate the PPP configuration negotiation
         * and authentication with the server.
         */
        printf("Configure PPP...");
        rc = NutNetIfConfig("ppp", 0, 0, 0);
        if (rc != 0) {
            puts("failed");
            /*
             * Optionally toggle DTR to hang up the modem.
             */
            continue;
        }
        puts("done");

        /*
         * Set name server and default route. Actually the PPP interface 
         * should do this, but the current release doesn't.
         */
        dcb = devPpp.dev_dcb;
        NutDnsConfig2(0, 0, dcb->dcb_ip_dns1, dcb->dcb_ip_dns2);
        NutIpRouteAdd(0, 0, dcb->dcb_remote_ip, &devPpp);

        /*
         * Display our IP settings.
         */
        printf("     Local IP: %s\n", inet_ntoa(dcb->dcb_local_ip));
        printf("    Remote IP: %s\n", inet_ntoa(dcb->dcb_remote_ip));
        printf("  Primary DNS: %s\n", inet_ntoa(dcb->dcb_ip_dns1));
        printf("Secondary DNS: %s\n", inet_ntoa(dcb->dcb_ip_dns2));

        /*
         * Client connection loop.
         */
        for (;;) {
            TCPSOCKET *sock;
            FILE *stream;

            /*
             * Create a socket.
             */
            if ((sock = NutTcpCreateSocket()) != 0) {

                /*
                 * Listen on port 23. If we return, we got a client.
                 */
                printf("Waiting for a client...");
                if (NutTcpAccept(sock, 23) == 0) {
                    puts("connected");

                    /*
                     * Open a stream and associate it with the socket, so 
                     * we can use standard I/O. Note, that socket streams
                     * currently do support text mode.
                     */
                    if ((stream = _fdopen((int) sock, "r+b")) != 0) {
                        /*
                         * Process client requests.
                         */
                        ProcessRequests(stream);
                        puts("\nDisconnected");

                        /*
                         * Close the stream.
                         */
                        fclose(stream);
                    } else {
                        puts("Assigning a stream failed");
                    }
                } else {
                    puts("failed");
                }

                /*
                 * Close our socket.
                 */
                NutTcpCloseSocket(sock);
            }
            NutSleep(1000);
            printf("%u bytes free\n", NutHeapAvailable());
        }
    }
}
