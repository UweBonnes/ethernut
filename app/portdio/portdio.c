/*!
 * Copyright (C) 2001-2002 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.1  2003/08/05 18:59:05  haraldkipp
 * Release 3.3 update
 *
 * Revision 1.7  2003/02/04 18:19:38  harald
 * Version 3 released
 *
 * Revision 1.6  2003/02/04 16:24:33  harald
 * Adapted to version 3
 *
 * Revision 1.5  2002/06/26 17:29:03  harald
 * First pre-release with 2.4 stack
 *
 * Revision 1.4  2002/06/12 10:56:16  harald
 * *** empty log message ***
 *
 * Revision 1.3  2002/06/04 19:12:37  harald
 * *** empty log message ***
 *
 * Revision 1.2  2002/05/08 16:02:31  harald
 * First Imagecraft compilation
 *
 */

/*!
 * \example portdio/portdio.c
 *
 * TCP server for Port D I/O.
 *
 * Program Ethernut with portdio.hex and enter
 *
 * telnet x.x.x.x 12345
 *
 * two times on two command prompts, replacing x.x.x.x with the IP address 
 * of your ethernut board. This will start two telnet session. 
 *
 * Enter help for a list of available commands.
 *
 * Enter query on the first will show the current port status.
 *
 * Then enter wait on this session, which will hang until the port status 
 * changes. 
 *
 * On the second telnet session enter set1 to set the first output bit.
 */

#define MY_MAC          {0x00,0x06,0x98,0x20,0x00,0x00}
#define MY_IP           "192.168.192.100"
#define MY_MASK         "255.255.255.0"

#include <string.h>
#include <stdio.h>

#include <dev/nicrtl.h>

#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <net/route.h>
#include <netdb.h>

#include <pro/dhcp.h>


/*
 * Process client requests.
 */
void ProcessRequests(FILE * stream)
{
    u_char buff[128];
    u_char *cp;
    int stat = -1;

    fputs("200 Welcome to portdio. Type help to get help.\r\n", stream);
    for (;;) {
        fflush(stream);

        /*
         * Read a line from the client. Ignore
         * blank lines.
         */
        if (fgets(buff, sizeof(buff), stream) == 0)
            break;
        if ((cp = strchr(buff, '\r')) != 0)
            *cp = 0;
        if ((cp = strchr(buff, '\n')) != 0)
            *cp = 0;
        if (buff[0] == 0)
            continue;

        /*
         * Memory info.
         */
        if (strncmp(buff, "memory", strlen(buff)) == 0) {
            fprintf(stream, "210 %u bytes RAM free\r\n", NutHeapAvailable());
            continue;
        }

        /*
         * List threads.
         */
        if (strncmp(buff, "threads", strlen(buff)) == 0) {
            NUTTHREADINFO *tdp;
            NUTTIMERINFO *tnp;

            fputs("220 List of threads with name,state,prio,stack,mem,timeout follows\r\n", stream);
            for (tdp = nutThreadList; tdp; tdp = tdp->td_next) {
                fputs(tdp->td_name, stream);
                fputs("\t", stream);
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
                    fprintf(stream, "%lu", tnp->tn_ticks_left);
                else
                    fputs("None", stream);
                fputs("\r\n", stream);
            }
            fputs(".\r\n", stream);
            continue;
        }

        /*
         * List timer.
         */
        if (strncmp(buff, "timers", strlen(buff)) == 0) {
            NUTTIMERINFO *tnp;

            fputs("221 List of timers with ticks left and interval follows\r\n", stream);
            for (tnp = nutTimerList; tnp; tnp = tnp->tn_next) {
                fprintf(stream, "%lu", tnp->tn_ticks_left);
                if (tnp->tn_ticks)
                    fprintf(stream, "\t%lu\r\n", tnp->tn_ticks);
                else
                    fputs("\tOneshot\r\n", stream);
            }
            fputs(".\r\n", stream);
            continue;
        }

        /*
         * Port status.
         */
        if (strncmp(buff, "query", strlen(buff)) == 0) {
            stat = inp(PIND);
            fprintf(stream, "210 %02X\r\n", stat);
            continue;
        }

        /*
         * Reset output bit.
         */
        if (strlen(buff) > 1 && strncmp(buff, "reset", strlen(buff) - 1) == 0) {
            int mask = 0;
            switch (buff[strlen(buff) - 1]) {
            case '1':
                mask = 0x10;
                break;
            case '2':
                mask = 0x20;
                break;
            case '3':
                mask = 0x40;
                break;
            case '4':
                mask = 0x80;
                break;
            }
            if (mask) {
                outp(inp(PORTD) & ~mask, PORTD);
                fputs("210 OK\r\n", stream);
            } else
                fputs("410 Bad pin\r\n", stream);
            continue;
        }

        /*
         * Set output bit.
         */
        if (strlen(buff) > 1 && strncmp(buff, "set", strlen(buff) - 1) == 0) {
            int mask = 0;
            switch (buff[strlen(buff) - 1]) {
            case '1':
                mask = 0x10;
                break;
            case '2':
                mask = 0x20;
                break;
            case '3':
                mask = 0x40;
                break;
            case '4':
                mask = 0x80;
                break;
            }
            if (mask) {
                outp(inp(PORTD) | mask, PORTD);
                fputs("210 OK\r\n", stream);
            } else
                fputs("410 Bad pin\r\n", stream);
            continue;
        }

        /*
         * wait for status change.
         */
        if (strncmp(buff, "wait", strlen(buff)) == 0) {
            while (stat == inp(PIND))
                NutThreadYield();
            stat = inp(PIND);
            fprintf(stream, "210 %02X\r\n", stat);
            continue;
        }

        /*
         * Help.
         */
        fputs("400 List of commands follows\r\n", stream);
        fputs("memory\tQueries number of RAM bytes free\r\n", stream);
        fputs("query\tQuery digital i/o status\r\n", stream);
        fputs("reset#\tSet output bit 1..4 low\r\n", stream);
        fputs("set#\tSet output bit 1..4 high\r\n", stream);
        fputs("threads\tLists all created threads\r\n", stream);
        fputs("timers\tLists all running timers\r\n", stream);
        fputs("wait\tWaits for digital i/o change\r\n", stream);
        fputs(".\r\n", stream);
    }
}

/*
 * Init Port D
 */
void init_dio(void)
{
    /*
     * Upper four pins are output pins.
     */
    outp(0xf0, DDRD);

    /*
     * Outputs to low and inputs pulled up.
     */
    outp(0x0f, PORTD);
}

void service(void)
{
    TCPSOCKET *sock;
    FILE *stream;

    /*
     * Loop endless for connections.
     */
    for (;;) {
        /*
         * Create a socket.
         */
        sock = NutTcpCreateSocket();

        /*
         * Listen on port 12345. If we return,
         * we got a client.
         */
        NutTcpAccept(sock, 12345);

        /*
         * Create a stream from the socket.
         */
        stream = _fdopen((int) sock, "r+b");

        /*
         * Process client requests.
         */
        ProcessRequests(stream);

        /*
         * Destroy our device.
         */
        fclose(stream);

        /*
         * Close our socket.
         */
        NutTcpCloseSocket(sock);
    }
}

THREAD(service_thread, arg)
{
    for (;;)
        service();
}

/*
 * Main application routine. 
 *
 * Nut/OS automatically calls this entry after initialization.
 */
int main(void)
{
    u_char my_mac[] = MY_MAC;

    /*
     * Initialize digital I/O.
     */
    init_dio();

    /*
     * Register Realtek controller at address 8300 hex
     * and interrupt 5.
     */
    NutRegisterDevice(&devEth0, 0x8300, 5);

    /*
     * Configure lan interface. 
     */
    if (NutDhcpIfConfig("eth0", 0, 60000) && NutDhcpIfConfig("eth0", my_mac, 60000)) {
        /*
         * No DHCP server available. Use hard coded values.
         */
        u_long ip_addr = inet_addr(MY_IP);      /* ICCAVR fix. */
        NutNetIfConfig("eth0", my_mac, ip_addr, inet_addr(MY_MASK));
    }

    /*
     * Start another service thread to allow
     * two concurrent connections.
     */
    NutThreadCreate("sback", service_thread, 0, 1384);

    for (;;)
        service();
}
