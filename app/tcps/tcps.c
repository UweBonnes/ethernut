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
 *
 */

/*!
 * $Log$
 * Revision 1.1  2003/08/05 18:59:05  haraldkipp
 * Release 3.3 update
 *
 * Revision 1.8  2003/02/04 16:24:35  harald
 * Adapted to version 3
 *
 * Revision 1.7  2002/06/26 17:29:06  harald
 * First pre-release with 2.4 stack
 *
 * Revision 1.6  2002/06/12 10:59:05  harald
 * *** empty log message ***
 *
 * Revision 1.5  2002/06/04 19:12:55  harald
 * *** empty log message ***
 *
 * Revision 1.4  2002/05/08 16:02:32  harald
 * First Imagecraft compilation
 *
 */

/*!
 * \example tcps/tcps.c
 *
 * Simple TCP server.
 *
 * Program Ethernut with tcps.rom and enter
 *
 * \code telnet x.x.x.x \endcode
 *
 * on a command prompt, replacing x.x.x.x with the
 * IP address of your ethernut board. Enter help
 * for a list of available commands.
 */

#include <string.h>
#include <stdio.h>
#include <io.h>

#include <dev/nicrtl.h>
//#include <dev/lanc111.h>
#include <dev/debug.h>

#include <sys/version.h>
#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <pro/dhcp.h>

static char buff[128];

/*
 * To save RAM, we store large strings in program space. With AVRGCC we
 * would be able to use the PSTR() macro and put the text directly in
 * the statement that uses it. But ICCAVR doesn't support anything like 
 * this. Sigh.
 */
#if defined(__IMAGECRAFT__)
#define CC_STRING   "ICCAVR"
#elif defined(__GNUC__)
#define CC_STRING   "AVRGCC"
#else
#define CC_STRING   "Compiler unknown"
#endif

prog_char vbanner_P[] = "\n\nTCP Server Sample - Nut/OS %s - " CC_STRING "\n";
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

    /*
     * Send a welcome banner.
     */
    fputs_P(banner_P, stream);
    for (;;) {

        /*
         * Flush output and read a line.
         */
        fflush(stream);
        if (fgets(buff, sizeof(buff), stream) == 0)
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
 * Main application routine. 
 *
 * Nut/OS automatically calls this entry after initialization.
 */
int main(void)
{
    TCPSOCKET *sock;
    FILE *stream;
    u_long baud = 115200;
    u_char mac[6] = { 0x00, 0x06, 0x98, 0x00, 0x00, 0x00 };

    /*
     * Register all devices used in our application.
     */
    NutRegisterDevice(&devDebug0, 0, 0);
    NutRegisterDevice(&devEth0, 0x8300, 5);

    /*
     * Assign stdout to the UART device.
     */
    freopen("uart0", "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);
    printf_P(vbanner_P, NutVersionString());

    //NutRegisterDevice(&devSmsc111, 0, 0);

    /*
     * Setup the ethernet device. Try DHCP first. If this is
     * the first time boot with empty EEPROM and no DHCP server
     * was found, use hardcoded values.
     */
    printf("Configure eth0...");
    if (NutDhcpIfConfig("eth0", 0, 60000)) {
        printf("initial boot...");
        if (NutDhcpIfConfig("eth0", mac, 60000)) {
            u_long ip_addr = inet_addr("192.168.192.100");
            u_long ip_mask = inet_addr("255.255.255.0");

            printf("no DHCP...");
            NutNetIfConfig("eth0", mac, ip_addr, ip_mask);
            /* If not in a local network, we must also call 
               NutIpRouteAdd() to configure the routing. */
        }
    }
    puts("OK");

    /*
     * Now loop endless for connections.
     */
    for (;;) {
        /*
         * Create a socket.
         */
        if ((sock = NutTcpCreateSocket()) != 0) {
            /*
             * Listen on port 23. If we return, we got a client.
             */
            printf("Waiting for a telnet client...");
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
                    puts("Disconnected");

                    /*
                     * Close the stream.
                     */
                    fclose(stream);
                } else
                    puts("Assigning a stream failed");
            } else
                puts("failed");

            /*
             * Close our socket.
             */
            NutTcpCloseSocket(sock);
        }
    }
}
