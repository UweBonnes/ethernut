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
#include <dev/nicrtl.h>
#include <dev/urom.h>

#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/confnet.h>
#include <sys/print.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <net/route.h>

#include <pro/httpd.h>
#include <pro/dhcp.h>

#ifdef __GNUC__
void NutInit(void) __attribute__ ((naked)) __attribute__ ((section(".init8")));
extern int NutAppMain(void) __attribute__ ((noreturn));
#endif


static int uart_bs;
static char *version = "3.1.1";
static u_short sram;
static char inbuff[32];
static char my_ip[32];
static char my_mask[32];
static char my_gate[32];
static u_char my_mac[32];

static char *states[] = { "TRM",
    "<FONT COLOR=#CC0000>RUN</FONT>",
    "<FONT COLOR=#339966>RDY</FONT>",
    "SLP"
};

static prog_char presskey_P[] = "Press any key to stop\r\n";
/*
 * Thread list CGI.
 */
static int ShowThreads(FILE * stream, REQUEST * req)
{
    NUTTHREADINFO *tdp = nutThreadList;
    static prog_char head_P[] = "<HTML><HEAD><TITLE>Nut/OS Threads</TITLE>" "</HEAD><BODY><h1>Nut/OS Threads</h1>\r\n";
    static prog_char ttop_P[] = "<TABLE BORDER><TR><TH>Handle</TH>"
        "<TH>Name</TH><TH>Priority</TH>"
        "<TH>Status</TH><TH>Event<BR>Queue</TH>" "<TH>Timer</TH><TH>Stack-<BR>pointer</TH>" "<TH>Free<BR>Stack</TH></TR>\r\n";
    static prog_char tbot_P[] = "</TABLE></BODY></HTML>";
    static prog_char tfmt_P[] = "<TR><TD>%04X</TD><TD>%s</TD><TD>%u</TD>"
        "<TD>%s</TD><TD>%04X</TD><TD>%04X</TD>" "<TD>%04X</TD><TD>%u</TD><TD>%s</TD></TR>\r\n";

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, "text/html", -1);

    fputs_P(head_P, stream);

    fputs_P(ttop_P, stream);
    while (tdp) {
        fprintf_P(stream, tfmt_P, tdp, tdp->td_name, tdp->td_priority,
                  states[tdp->td_state], tdp->td_queue, tdp->td_timer,
                  tdp->td_sp,
                  (u_short) tdp->td_sp - (u_short) tdp->td_memory, *((u_long *) tdp->td_memory) != DEADBEEF ? "Corr" : "OK");
        tdp = tdp->td_next;
    }
    fputs_P(tbot_P, stream);
    fflush(stream);

    return 0;
}

/*
 * Timer list CGI.
 */
static int ShowTimer(FILE * stream, REQUEST * req)
{
    NUTTIMERINFO *tnp;
    u_long ticks_left;
    u_long crystal;
    static prog_char head_P[] = "<HTML><HEAD><TITLE>Nut/OS Timers</TITLE>" "</HEAD><BODY>";
    static prog_char cfmt_P[] = "\r\nCPU running at %u.%04u MHz<br>\r\n";
    static prog_char ttop_P[] = "<TABLE BORDER><TR><TH>Handle</TH>"
        "<TH>Countdown</TH><TH>Tick Reload</TH>" "<TH>Callback<BR>Address</TH>" "<TH>Callback<BR>Argument</TH></TR>\r\n";
    static prog_char tfmt_P[] = "<TR><TD>%04X</TD><TD>%lu</TD><TD>%lu</TD>" "<TD>%04X</TD><TD>%04X</TD></TR>\r\n";
    static prog_char tbot_P[] = "</TABLE></BODY></HTML>";

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, "text/html", -1);

    fputs_P(head_P, stream);

    crystal = NutGetCpuClock();
    fprintf_P(stream, cfmt_P, (int) (crystal / 1000000UL), (int) ((crystal - (crystal / 1000000UL) * 1000000UL) / 100));

    if ((tnp = nutTimerList) != 0) {
        fputs_P(ttop_P, stream);
        ticks_left = 0;
        while (tnp) {
            ticks_left += tnp->tn_ticks_left;
            fprintf_P(stream, tfmt_P, tnp, ticks_left, tnp->tn_ticks, tnp->tn_callback, tnp->tn_arg);
            tnp = tnp->tn_next;
        }
    }

    fputs_P(tbot_P, stream);
    fflush(stream);

    return 0;
}

/*
 * Socket list CGI.
 */
static int ShowSockets(FILE * stream, REQUEST * req)
{
    extern TCPSOCKET *tcpSocketList;
    TCPSOCKET *ts;
    static prog_char head_P[] = "<HTML><HEAD><TITLE>Show Threads</TITLE>"
        "</HEAD><BODY><TABLE BORDER><TR>" "<TH>Handle</TH><TH>Type</TH><TH>Local</TH>" "<TH>Remote</TH><TH>Status</TH></TR>\r\n";
    static prog_char fmt1_P[] = "<TR><TD>%04X</TD><TD>TCP</TD><TD>%s:%u</TD>";
    static prog_char fmt2_P[] = "<TD>%s:%u</TD><TD>";
    static prog_char estb_P[] = "<FONT COLOR=#CC0000>ESTABL</FONT>";
    static prog_char tbot_P[] = "</TABLE></BODY></HTML>\r\n";

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, "text/html", -1);

    fputs_P(head_P, stream);

    for (ts = tcpSocketList; ts; ts = ts->so_next) {
        fprintf_P(stream, fmt1_P, ts, inet_ntoa(ts->so_local_addr), ntohs(ts->so_local_port));
        fprintf_P(stream, fmt2_P, inet_ntoa(ts->so_remote_addr), ntohs(ts->so_remote_port));
        switch (ts->so_state) {
        case TCPS_LISTEN:
            fputs("LISTEN", stream);
            break;
        case TCPS_SYN_SENT:
            fputs("SYNSENT", stream);
            break;
        case TCPS_SYN_RECEIVED:
            fputs("SYNRCVD", stream);
            break;
        case TCPS_ESTABLISHED:
            fputs_P(estb_P, stream);
            break;
        case TCPS_FIN_WAIT_1:
            fputs("FINWAIT1", stream);
            break;
        case TCPS_FIN_WAIT_2:
            fputs("FINWAIT2", stream);
            break;
        case TCPS_CLOSE_WAIT:
            fputs("CLOSEWAIT", stream);
            break;
        case TCPS_CLOSING:
            fputs("CLOSING", stream);
            break;
        case TCPS_LAST_ACK:
            fputs("LASTACK", stream);
            break;
        case TCPS_TIME_WAIT:
            fputs("TIMEWAIT", stream);
            break;
        case TCPS_CLOSED:
            fputs("CLOSED", stream);
            break;
        default:
            fputs("?UNK?", stream);
            break;
        }
        fputs("</TD></TR>\r\n", stream);
    }

    fputs_P(tbot_P, stream);
    fflush(stream);

    return 0;
}

void DoCheckboxes(FILE * stream, u_char * name, u_char val)
{
    u_char i;
    static prog_char ttop_P[] = "<tr><td>%s</td>";
    static prog_char tfmt_P[] = "<td><input type=\"checkbox\"" " name=\"%s\" value=\"%u\" ";
    static prog_char tchk_P[] = " checked=\"checked\"";

    fprintf_P(stream, ttop_P, name);
    for (i = 8; i-- > 0;) {
        fprintf_P(stream, tfmt_P, name, i);
        if (val & BV(i))
            fputs_P(tchk_P, stream);
        fputs("></td>\r\n", stream);
    }
    fputs("</tr>\r\n", stream);
}

/*
 * Socket list CGI.
 */
static int ShowPorts(FILE * stream, REQUEST * req)
{
    static prog_char ttop_P[] = "<HTML><HEAD><TITLE>Show Ports</TITLE>"
        "</HEAD><BODY>"
        "<form action=\"cgi-bin/setports.cgi\" "
        "enctype=\"text/plain\"> <TABLE BORDER>"
        "<tr><td>Bit</td><td>7</td><td>6</td>" "<td>5</td><td>4</td><td>3</td><td>2</td>" "<td>1</td><td>0</td></tr>\r\n";
    static prog_char trow_P[] = "<tr></tr>";
    static prog_char tbot_P[] = "</table></form>\r\n</body>\r\n</html>";

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, "text/html", -1);

    fputs_P(ttop_P, stream);
    DoCheckboxes(stream, "DDRA", inp(DDRA));
    DoCheckboxes(stream, "PINA", inp(PINA));
    DoCheckboxes(stream, "PORTA", inp(PORTA));

    fputs_P(trow_P, stream);
    DoCheckboxes(stream, "DDRB", inp(DDRB));
    DoCheckboxes(stream, "PINB", inp(PINB));
    DoCheckboxes(stream, "PORTB", inp(PORTB));

    fputs_P(trow_P, stream);
    DoCheckboxes(stream, "PORTC", inp(PORTC));

    fputs_P(trow_P, stream);
    DoCheckboxes(stream, "DDRD", inp(DDRD));
    DoCheckboxes(stream, "PIND", inp(PIND));
    DoCheckboxes(stream, "PORTD", inp(PORTD));

    fputs_P(trow_P, stream);
    DoCheckboxes(stream, "DDRE", inp(DDRE));
    DoCheckboxes(stream, "PINE", inp(PINE));
    DoCheckboxes(stream, "PORTE", inp(PORTE));

    fputs_P(trow_P, stream);
    DoCheckboxes(stream, "PINF", inp(PINF));

    fputs_P(tbot_P, stream);
    fflush(stream);

    return 0;
}

/*
 * Delay by executing a given number of NOPs.
 */
void Delay(long nops)
{
    volatile long i;

    for (i = 0; i < nops; i++)
#ifdef __IMAGECRAFT__
        asm("nop\n");
#else
        asm volatile ("nop\n\t"::);
#endif
}

/*
 * Wait for a character from the serial port.
 */
u_char GetChar(void)
{
    u_char ch;

    if ((inp(USR) & BV(RXC)) == 0)
        return 0;
    ch = inp(UDR);
    return ch;
}

/*
 * Get a line of input.
 */
int GetLine(u_char * line, int size)
{
    int cnt = 0;
    u_char ch;

    GetChar();
    for (;;) {
        if (cnt > size - 1)
            break;
        while ((ch = GetChar()) == 0);
        if (ch < 32)
            break;
        NutPrintBinary(0, &ch, 1);
        *line++ = ch;
        cnt++;
    }
    *line = 0;
    return cnt;
}

/*
 * Get a line of input.
 */
u_char *GetIP(u_char * prompt, u_char * value)
{
    static prog_char badip_P[] = "\r\nBad IP address";
    for (;;) {
        NutPrintFormat(0, "\r\n%s(%s): ", prompt, value);
        if (GetLine(inbuff, sizeof(inbuff)) == 0)
            break;
        if (inet_addr(inbuff) != (u_long) (-1L)) {
            strcpy(value, inbuff);
            break;
        }
        NutPrintString_P(0, badip_P);
    }
    return value;
}

/*
 * Automatic baudrate detection.
 *
 * Switch to different baud rates and wait 
 * for a space character.
 */
int DetectSpeed(void)
{
    u_char bstab[] = { 1, 2, 3, 5, 7, 11, 15, 23, 31, 47, 63, 95, 191,
        0, 8, 12, 17, 25, 35, 51, 71, 103, 143, 207
    };
    u_char bsx;
    int bs;
    u_short i;
    u_char t;
    u_char rec;
    u_char ict;                 /* Imagecraft dummy */

    /*
     * Enable UART transmitter and receiver.
     */
    outp(BV(RXEN) | BV(TXEN), UCR);

    /*
     * Endless retries.
     */
    for (t = 1; t < 16; t++) {
        for (bsx = 0; bsx < sizeof(bstab); bsx++) {
            bs = bstab[bsx];

            /*
             * Set baudrate selector.
             */
            Delay(1000);
            if ((inp(USR) & BV(RXC)) != 0)
                ict = inp(UDR);
            outp((u_char) bs, UBRR);
            Delay(1000);
            if ((inp(USR) & BV(RXC)) != 0)
                ict = inp(UDR);

            /*
             * Now wait for some time for a character received.
             */
            for (i = 0; i < 40; i++) {
                if ((inp(USR) & BV(RXC)) != 0)
                    break;
                Delay(1000);
            }
            rec = 0;
            for (i = 1; i; i++) {
                if ((inp(USR) & BV(RXC)) != 0) {
                    if (inp(UDR) == 32) {
                        if (rec++)
                            return bs;
                        i = 1;
                    } else {
                        rec = 0;
                        break;
                    }
                } else if (rec)
                    Delay(100);
            }
        }
    }
    outp(23, UBRR);
    return -1;
}

THREAD(NutMain, arg)
{
    TCPSOCKET *sock;
    FILE *stream;
    u_long ip_addr;             /* ICCAVR bugfix */
    static prog_char netfail_P[] = "\r\nFailed to configure network " "interface: Ethernut stopped!\r\n\x07";
    /*
     * Register Realtek controller at address 8300 hex
     * and interrupt 5.
     */
    NutRegisterDevice(&devEth0, 0x8300, 5);

    /*
     * Configure lan interface.
     */
    ip_addr = inet_addr(my_ip);
    if (ip_addr) {
        if (NutNetIfConfig("eth0", my_mac, ip_addr, inet_addr(my_mask))) {
            if (uart_bs >= 0) {
                NutPrintString_P(0, netfail_P);
                for (;;)
                    NutSleep(1000);
            } else {
#ifdef __IMAGECRAFT__
                asm("cli\n");
                asm("call 0\n");
#else
                asm volatile ("cli\n\tcall 0\n\t"::);
#endif
            }
        }
    } else if (NutDhcpIfConfig("eth0", my_mac, 60000)) {
        if (uart_bs >= 0) {
            NutPrintString_P(0, netfail_P);
            for (;;)
                NutSleep(1000);
        } else {
#ifdef __IMAGECRAFT__
            asm("cli\n");
            asm("call 0\n");
#else
            asm volatile ("cli\n\tcall 0\n\t"::);
#endif
        }
    }


    NutNetLoadConfig("eth0");
    if ((confnet.cdn_mac[0] & confnet.cdn_mac[1] & confnet.cdn_mac[2]) == 0xFF) {
        u_char mac[] = {
            0x00, 0x06, 0x98, 0x00, 0x00, 0x00
        };
        memcpy(confnet.cdn_mac, mac, sizeof(confnet.cdn_mac));
    }

    if (uart_bs >= 0) {
        NutPrintFormat(0, "MAC %02X-%02X-%02X-%02X-%02X-%02X\r\nIP %s",
                       confnet.cdn_mac[0], confnet.cdn_mac[1],
                       confnet.cdn_mac[2], confnet.cdn_mac[3], confnet.cdn_mac[4], confnet.cdn_mac[5],
                       inet_ntoa(confnet.cdn_ip_addr));
        NutPrintFormat(0, "\r\nMask %s", inet_ntoa(confnet.cdn_ip_mask));
    }

    /*
     * Add optional default route.
     */
    if (inet_addr(my_ip) && inet_addr(my_gate)) {
        if (uart_bs >= 0)
            NutPrintFormat(0, "\r\nAdd gateway %s", my_gate);
        NutIpRouteAdd(0, 0, inet_addr(my_gate), &devEth0);
    }

    else if (confnet.cdn_gateway && uart_bs >= 0)
        NutPrintFormat(0, "\r\nGate %s", inet_ntoa(confnet.cdn_gateway));
    if (uart_bs >= 0)
        NutPrintFormat(0, "\r\nHTTP server running. URL http://%s/index.html\r\n",
                       inet_ntoa(((IFNET *) (devEth0.dev_icb))->if_local_ip));
    /*
     * Register our device for the file system.
     */
    NutRegisterDevice(&devUrom, 0, 0);
    /*
     * Register CGI routines.
     */
    NutRegisterCgi("threads.cgi", ShowThreads);
    NutRegisterCgi("timer.cgi", ShowTimer);
    NutRegisterCgi("sockets.cgi", ShowSockets);
    NutRegisterCgi("ports.cgi", ShowPorts);
    /*
     * Now loop endless for connections.
     */
    for (;;) {
        /*
         * Create a socket.
         */
        sock = NutTcpCreateSocket();
        /*
         * Listen on port 80. If we return,
         * we got a client.
         */
        NutTcpAccept(sock, 80);
        /*
         * Create a stream from the socket, so we can use stdio.
         */
        stream = _fdopen((int) sock, "r+b");
        /*
         * Process http request.
         */
        NutHttpProcessRequest(stream);
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

THREAD(idle, arg)
{
    NutTimerInit();
    NutThreadCreate("main", NutMain, 0, 1384);
    NutThreadSetPriority(255);
    for (;;)
        NutThreadYield();
}

static int NicReset(void)
{
    volatile u_char *base = (u_char *) 0x8300;
    u_char i;
    u_char j;
    for (j = 0; j < 20; j++) {
        NutPrintString(0, "SW-Reset...");
        i = nic_read(NIC_RESET);
        Delay(500);
        nic_write(NIC_RESET, i);
        for (i = 0; i < 20; i++) {
            Delay(5000);
            /*
             * ID detection added for version 1.1 boards.
             */
            if ((nic_read(NIC_PG0_ISR) & NIC_ISR_RST) != 0 && nic_read(NIC_PG0_RBCR0) == 0x50 && nic_read(NIC_PG0_RBCR1) == 0x70) {
                NutPrintString(0, "OK\r\n");
                return 0;
            }
        }
        NutPrintString(0, "failed\r\n\x07");
        /*
         * Toggle the hardware reset line. Since Ethernut version 1.3 the 
         * hardware reset pin of the nic is no longer connected to bit 4 
         * on port E, but wired to the board reset line.
         */
        if (j == 10) {
            NutPrintString(0, "HW-Reset\r\n");
            sbi(DDRE, 4);
            sbi(PORTE, 4);
            Delay(100000);
            cbi(PORTE, 4);
            Delay(250000);
        }
    }
    return -1;
}

#define NIC_PAGE_SIZE       0x100
#define NIC_START_PAGE      0x40
#define NIC_STOP_PAGE       0x60
#define NIC_TX_PAGES        6
#define NIC_TX_BUFFERS      2
#define NIC_FIRST_TX_PAGE   NIC_START_PAGE
#define NIC_FIRST_RX_PAGE   (NIC_FIRST_TX_PAGE + NIC_TX_PAGES * NIC_TX_BUFFERS)
#define TX_PAGES            12

void SendBroadcasts(void)
{
    u_char mac[] = {
        0x00, 0x06, 0x98, 0x00, 0x00, 0x00
    };
    u_short sz;
    u_short i;
    volatile u_char *base = (u_char *) 0x8300;
    u_char rb;
    u_long cnt = 0;
    if (NicReset())
        return;
    NutPrintString(0, "Init controller...");
    nic_write(NIC_PG0_IMR, 0);
    nic_write(NIC_PG0_ISR, 0xff);
    nic_write(NIC_CR, NIC_CR_STP | NIC_CR_RD2 | NIC_CR_PS0 | NIC_CR_PS1);
    nic_write(NIC_PG3_EECR, NIC_EECR_EEM0 | NIC_EECR_EEM1);
    nic_write(NIC_PG3_CONFIG3, 0);
    nic_write(NIC_PG3_CONFIG2, NIC_CONFIG2_BSELB);
    nic_write(NIC_PG3_EECR, 0);
    Delay(50000);
    nic_write(NIC_CR, NIC_CR_STP | NIC_CR_RD2);
    nic_write(NIC_PG0_DCR, NIC_DCR_LS | NIC_DCR_FT1);
    nic_write(NIC_PG0_RBCR0, 0);
    nic_write(NIC_PG0_RBCR1, 0);
    nic_write(NIC_PG0_RCR, NIC_RCR_MON);
    nic_write(NIC_PG0_TCR, NIC_TCR_LB0);
    nic_write(NIC_PG0_TPSR, NIC_FIRST_TX_PAGE);
    nic_write(NIC_PG0_BNRY, NIC_STOP_PAGE - 1);
    nic_write(NIC_PG0_PSTART, NIC_FIRST_RX_PAGE);
    nic_write(NIC_PG0_PSTOP, NIC_STOP_PAGE);
    nic_write(NIC_PG0_ISR, 0xff);
    nic_write(NIC_CR, NIC_CR_STP | NIC_CR_RD2 | NIC_CR_PS0);
    for (i = 0; i < 6; i++)
        nic_write(NIC_PG1_PAR0 + i, mac[i]);
    for (i = 0; i < 8; i++)
        nic_write(NIC_PG1_MAR0 + i, 0);
    nic_write(NIC_PG1_CURR, NIC_START_PAGE + TX_PAGES);
    nic_write(NIC_CR, NIC_CR_STP | NIC_CR_RD2);
    nic_write(NIC_PG0_RCR, NIC_RCR_AB);
    nic_write(NIC_PG0_ISR, 0xff);
    nic_write(NIC_PG0_IMR, 0);
    nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2);
    nic_write(NIC_PG0_TCR, 0);
    Delay(1000000);
    NutPrintString(0, "done\r\n");
    nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2 | NIC_CR_PS0 | NIC_CR_PS1);
    rb = nic_read(NIC_PG3_CONFIG0);
    switch (rb & 0xC0) {
    case 0x00:
        NutPrintString(0, "RTL8019AS ");
        if (rb & 0x08)
            NutPrintString(0, "jumper mode: ");
        if (rb & 0x20)
            NutPrintString(0, "AUI ");
        if (rb & 0x10)
            NutPrintString(0, "PNP ");
        break;
    case 0xC0:
        NutPrintString(0, "RTL8019 ");
        if (rb & 0x08)
            NutPrintString(0, "jumper mode: ");
        break;
    default:
        NutPrintString(0, "Unknown chip ");
        break;
    }
    if (rb & 0x04)
        NutPrintString(0, "BNC\x07 ");
    if (rb & 0x03)
        NutPrintString(0, "Failed\x07 ");
    rb = nic_read(NIC_PG3_CONFIG1);
    NutPrintFormat(0, "IRQ%u ", (rb >> 4) & 7);
    rb = nic_read(NIC_PG3_CONFIG2);
    switch (rb & 0xC0) {
    case 0x00:
        NutPrintString(0, "Auto ");
        break;
    case 0x40:
        NutPrintString(0, "10BaseT ");
        break;
    case 0x80:
        NutPrintString(0, "10Base5 ");
        break;
    case 0xC0:
        NutPrintString(0, "10Base2 ");
        break;
    }


    NutPrintString(0, "\r\n");
    nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2);
    for (;;) {
        NutPrintFormat(0, "\r%lu", cnt++);
        sz = 1500;
        nic_write(NIC_PG0_RBCR0, sz);
        nic_write(NIC_PG0_RBCR1, sz >> 8);
        nic_write(NIC_PG0_RSAR0, 0);
        nic_write(NIC_PG0_RSAR1, NIC_FIRST_TX_PAGE);
        nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD1);
        /*
         * Transfer ethernet physical header.
         */
        for (i = 0; i < 6; i++)
            nic_write(NIC_IOPORT, 0xFF);
        for (i = 0; i < 6; i++)
            nic_write(NIC_IOPORT, mac[i]);
        nic_write(NIC_IOPORT, 0x08);
        nic_write(NIC_IOPORT, 0x00);
        /*
         * Add pad bytes.
         */
        for (i = 0; i < sz; i++)
            nic_write(NIC_IOPORT, 0);
        /*
         * Complete remote dma.
         */
        nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2);
        for (i = 0; i <= 20; i++)
            if (nic_read(NIC_PG0_ISR) & NIC_ISR_RDC)
                break;
        nic_write(NIC_PG0_ISR, NIC_ISR_RDC);
        /*
         * Number of bytes to be transmitted.
         */
        nic_write(NIC_PG0_TBCR0, (sz & 0xff));
        nic_write(NIC_PG0_TBCR1, ((sz >> 8) & 0xff));
        /*
         * First page of packet to be transmitted.
         */
        nic_write(NIC_PG0_TPSR, NIC_FIRST_TX_PAGE);
        /*
         * Start transmission.
         */
        nic_write(NIC_CR, NIC_CR_STA | NIC_CR_TXP | NIC_CR_RD2);
        if (GetChar())
            break;
        Delay(10000);
    }
}

/*
 * Toggle bit 4 on port E until key pressed.
 */
void LoopReset(void)
{
    NutPrintString_P(0, presskey_P);
    /*
     * Port test makes PORTE all inputs, so set
     * bit 4 of PORTE back to output.
     */
    sbi(DDRE, 4);
    for (;;) {
        sbi(PORTE, 4);
        Delay(200000);
        cbi(PORTE, 4);
        Delay(200000);
        if (GetChar())
            break;
    }
}

void LoopRealtek(void)
{
    volatile u_char *base = (u_char *) 0x8300;
    u_short nic_id;
    NutPrintString_P(0, presskey_P);
    for (;;) {
        nic_write(NIC_CR, NIC_CR_STP | NIC_CR_RD2);
        nic_id = *(base + 10) << 8;
        nic_id |= *(base + 11);
        NutPrintFormat(0, "\rid=0x%04X", nic_id);
        if (GetChar()) {
            NutPrintString(0, "\r\n");
            return;
        }
    }
}

void FindRealtek(void)
{
    volatile u_char *base = (u_char *) 0x8300;
    int i;
    u_short nic_id;
    NutPrintString_P(0, presskey_P);
    for (;;) {
        for (base = (u_char *) 0x8000; base < (u_char *) 0xffff; base += 0x20) {
            NutPrintFormat(0, "NIC %04X reset...", (u_short) base);
            *(base + 0x1f) = *(base + 0x1f);
            Delay(200000);
            for (i = 0; i < 255; i++) {
                if (*(base + 7) & 0x80)
                    break;
            }
            if (i < 255) {
                nic_id = *(base + 10) << 8;
                nic_id |= *(base + 11);
                NutPrintFormat(0, "OK id=0x%04X", nic_id);
                if (nic_id != 0x5070)
                    NutPrintString(0, " failed\r");
                else
                    NutPrintString(0, " OK    \r\n");
            } else
                NutPrintString(0, "failed\r");
            if (GetChar()) {
                NutPrintString(0, "\r\n");
                return;
            }
        }
    }
}

void TestRealtek(void)
{
    int i;
    u_char force_swreset = 0;
    volatile u_char *base = (u_char *) 0x8300;
    u_short nic_id;
    /*
     * NIC ID detection loop.
     */
    for (;;) {
        /*
         * Reset loop.
         */
        for (;;) {
            /*
             * The hardware reset pin of the nic is connected
             * to bit 4 on port E. Make this pin an output pin
             * and toggle it.
             */
            NutPrintString(0, "NIC hardware reset...");
            sbi(DDRE, 4);
            sbi(PORTE, 4);
            Delay(2000);
            cbi(PORTE, 4);
            Delay(20000);
            /*
             * If the hardware reset didn't set the nic in reset
             * state, perform an additional software reset and
             * wait until the controller enters the reset state.
             */
            if (force_swreset || (*(base + 7) & 0x80) == 0) {
                NutPrintString(0, "failed\r\nTrying NIC software reset...");
                *(base + 0x1f) = *(base + 0x1f);
                Delay(200000);
                for (i = 0; i < 255; i++) {
                    if (*(base + 7) & 0x80) {
                        break;
                    }
                }
                if (i < 255) {
                    NutPrintString(0, "OK\r\n");
                    break;
                }
                NutPrintString(0, "failed\r\n\x07");
            } else {
                NutPrintString(0, "OK\r\n");
                break;
            }
            if (uart_bs >= 0)
                break;
        }
        NutPrintString(0, "NIC id detection...  ");
        nic_id = *(base + 10) << 8;
        nic_id |= *(base + 11);
        if (nic_id == 0x5070) {
            NutPrintString(0, "OK\r\n");
            break;
        }
        NutPrintFormat(0, "failed, got 0x%04X, expected 0x5070\r\n\x07", nic_id);
        if (force_swreset && uart_bs >= 0)
            break;
        force_swreset = 1;
    }
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
    for (pat = 1; pat; pat <<= 1) {
        ipat = ~pat;
        outp(pat, DDRB);
        outp(ipat, PORTB);
        Delay(100);
        if ((val = inp(PINB)) != ipat)
            bpat |= ~val;
    }
    outp(0, DDRB);
    if (bpat)
        NutPrintFormat(0, "PORTB bits %02X\r\n\x07", bpat);
    for (pat = 1; pat; pat <<= 1) {
        ipat = ~pat;
        outp(pat, DDRD);
        outp(ipat, PORTD);
        Delay(1000);
        if ((val = inp(PIND)) != ipat)
            dpat |= ~val;
    }
    outp(0, DDRD);
    if (dpat)
        NutPrintFormat(0, "PORTD bits %02X\r\n\x07", dpat);
    for (pat = 4; pat; pat <<= 1) {
        if (pat == 0x20)
            continue;
        ipat = ~pat;
        outp(pat, DDRE);
        outp(ipat, PORTE);
        Delay(1000);
        if ((val = inp(PINE)) != ipat)
            epat |= ~val;
    }
    outp(0, DDRE);
    if (epat)
        NutPrintFormat(0, "PORTE bits %02X\r\n\x07", epat);
    if (bpat || dpat || epat)
        return -1;
    return 0;
}

/*
 * Test external SRAM.
 */
u_short TestExternalRam(u_short first, u_short last)
{
    volatile u_char *mem;
    u_short all_low;
    u_short all_high;
    u_short short_high;
    u_short short_low;
    u_char pat8;
    u_short pat16;
    u_short ipat16;
    u_short msk16;
    /*
     * Enable external RAM.
     */
    outp(BV(SRE) | BV(SRW), MCUCR);
    //outp(BV(SRW11), XMCRA);
    /*
     * Check data bus integrity.
     * -------------------------
     */
    all_low = 0;
    for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
        *mem = 0xFF;
        if (*mem != 0xFF) {
            if (all_low)
                all_low &= ~*mem;
            else
                all_low = ~*mem;
        }
        if (all_low == 0)
            break;
    }
    if (all_low)
        NutPrintFormat(0, "Data bus bits %02X always low\r\n\x07", all_low);
    all_high = 0;
    for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
        *mem = 0;
        if (*mem) {
            if (all_high)
                all_high &= *mem;
            else
                all_high = *mem;
        }
        if (all_high == 0)
            break;
    }
    if (all_high)
        NutPrintFormat(0, "Data bus bits %02X always high\r\n\x07", all_high);
    short_high = 0;
    for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
        if (short_high && (short_high & (u_short) mem) != 0 && (short_high & (u_short) mem) != short_high)
            continue;
        for (pat8 = 1; pat8; pat8 <<= 1) {
            *mem = pat8;
            if ((*mem & ~pat8) != 0) {
                if (short_high)
                    short_high &= *mem;
                else
                    short_high = *mem;
            }
        }
        if (short_high == 0)
            break;
    }

    short_low = short_high;
    for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
        if (short_low && (short_low & (u_short) mem) != 0 && (short_low & (u_short) mem) != short_low)
            continue;
        for (pat8 = 1; pat8; pat8 <<= 1) {
            *mem = ~pat8;
            if ((*mem | pat8) != 0xFF) {
                if (short_low)
                    short_low &= ~*mem;
                else
                    short_low = ~*mem & 0xFF;
            }
        }
        if (short_low == 0)
            break;
    }

    if (short_high | short_low)
        NutPrintFormat(0, "Data bus bits 0x%02X may be shortened\r\n\x07", short_high | short_low);
    if (short_high || short_low || all_high || all_low)
        return 0;
    /*
     * Check address bus integrity.
     * ----------------------------
     */
    for (mem = (u_char *) first; (u_short) mem <= last; mem++)
        *mem = 0;
    all_low = 0;
    for (msk16 = 0xFFFF; msk16 > last; msk16 >>= 1);
    *((u_char *) msk16) = 0xAA;
    for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
        if (*mem == 0xAA && (u_short) mem != msk16) {
            if (all_low)
                all_low &= ~(u_short) mem;
            else
                all_low = ~(u_short) mem & msk16;
        }
    }
    *((u_char *) msk16) = 0x55;
    for (mem = (u_char *) first; (u_short) mem < last; mem++) {
        if (*mem == 0x55 && (u_short) mem != msk16) {
            if (all_low)
                all_low &= ~(u_short) mem;
            else
                all_low = ~(u_short) mem & msk16;
        }
    }
    *((u_char *) msk16) = 0;
    if (all_low)
        NutPrintFormat(0, "Address bus bits %04X always low\r\n\x07", all_low);
    all_high = 0;
    for (pat16 = 0x8000; pat16 > last; pat16 >>= 1);
    *((u_char *) pat16) = 0xAA;
    for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
        if (*mem == 0xAA && (u_short) mem != pat16) {
            if (all_high)
                all_high &= (u_short) mem;
            else
                all_high = (u_short) mem;
        }
    }
    *((u_char *) pat16) = 0x55;
    for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
        if (*mem == 0x55 && (u_short) mem != pat16) {
            if (all_high)
                all_high &= (u_short) mem;
            else
                all_high = (u_short) mem;
        }
    }
    *((u_char *) pat16) = 0;
    for (pat16 = 1; pat16 <= first; pat16 <<= 1);
    *((u_char *) pat16) = 0xAA;
    for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
        if (*mem == 0xAA && (u_short) mem != pat16) {
            if (all_high)
                all_high &= (u_short) mem;
            else
                all_high = (u_short) mem;
        }
    }
    *((u_char *) pat16) = 0x55;
    for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
        if (*mem == 0x55 && (u_short) mem != pat16) {
            if (all_high)
                all_high &= (u_short) mem;
            else
                all_high = (u_short) mem;
        }
    }
    *((u_char *) pat16) = 0;
    if (all_high)
        NutPrintFormat(0, "Address bus bits %04X always high\r\n\x07", all_high);
    short_high = 0;
    for (pat16 = 0x0100; pat16 <= last; pat16 <<= 1) {
        if (pat16 < first)
            continue;
        *((u_char *) pat16) = 0x55;
        for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
            if (*mem == 0x55 && ((u_short) mem & ~pat16) != 0) {
                *((u_char *) pat16) = 0xAA;
                if (*mem == 0xAA) {
                    if (short_high)
                        short_high &= (u_short) mem;
                    else
                        short_high = (u_short) mem;
                }
                *((u_char *) pat16) = 0x55;
            }
        }
        *((u_char *) pat16) = 0;
    }

    short_low = 0;
    for (pat16 = 0x0100; pat16 <= last; pat16 <<= 1) {
        ipat16 = ~pat16 & msk16;
        if (ipat16 < first)
            continue;
        *((u_char *) ipat16) = 0x55;
        for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
            if (*mem == 0x55 && ((u_short) mem | pat16) != msk16) {
                *mem = 0xAA;
                if (*((u_char *) ipat16) == 0xAA) {
                    if (short_low)
                        short_low &= ~((u_short) mem & ipat16);
                    else
                        short_low = ~((u_short) mem & ipat16) & msk16;
                }
                *mem = 0x55;
            }
        }
        *((u_char *) ipat16) = 0;
    }

    if (short_high | short_low)
        NutPrintFormat(0, "Address bus bits %04X may be shortened\r\n\x07", short_low | short_high);
    if (short_high || short_low || all_high || all_low)
        return 0;
    for (mem = (u_char *) first; (u_short) mem <= last; mem++) {
        for (pat8 = 1; pat8; pat8 <<= 1) {
            *mem = pat8;
            if (*mem != pat8) {
                last = (u_short) mem;
                NutPrintFormat(0, "Memory chip error at %04X\r\n\x07", last);
                break;
            }
        }
    }

    return last - first + 1;
}

/*
 * Read/write external SRAM until key pressed.
 */
void LoopSRAM(void)
{
    register u_short pattern;
    volatile u_char *mem;
    u_short faddr = 0xFFFF;
    NutPrintString(0, "Check address and data bus. ");
    NutPrintString_P(0, presskey_P);
    for (;;) {
        for (pattern = 1; pattern; pattern <<= 1) {
            if (pattern <= RAMEND)
                mem = (u_char *) (pattern | (RAMEND + 1));
            else
                mem = (u_char *) pattern;
            *mem = (u_char) ((pattern >> 8) | pattern);
        }
        for (pattern = 1; pattern; pattern <<= 1) {
            if (pattern <= RAMEND)
                mem = (u_char *) (pattern | (RAMEND + 1));
            else
                mem = (u_char *) pattern;
            if (*mem != (u_char) ((pattern >> 8) | pattern))
                faddr = (u_short) mem;
        }
        Delay(5000);
        if (GetChar()) {
            NutPrintFormat(0, "No RAM at 0x%04X\r\n", faddr);
            return;
        }
    }
}

int hex2bin(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
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
    static prog_char menu1_P[] = "Press any of the following keys:\r\n";
    static prog_char menu2_P[] = "    B - Send broadcasts\r\n";
    static prog_char menu3_P[] =
        "    E - Ethernet controller read/write\r\n"
        "    F - Find Ethernet controller\r\n"
        "    R - Ethernet controller hardware reset (Ethernut 1.0/1.1)\r\n" "    S - SRAM read/write\r\n";
    static prog_char menu4_P[] = "    X - Exit BaseMon, configure network and start WebServer\r\n";
    uart_bs = DetectSpeed();
    UDR = 'A';
    /*
     * Print banner.
     */
    NutPrintFormat(0, "\r\nEthernut BaseMon Version %s\r\n", version);
    if (uart_bs >= 0)
        NutPrintFormat(0, "Baudrate select = %d\r\n", uart_bs);
    /*
     * Test external SRAM.
     */
#ifdef __IMAGECRAFT__
    sram = 28416;
    NutPrintFormat(0, "Skipped\r\n", sram);
    outp(BV(SRE) | BV(SRW), MCUCR);
#else
    NutPrintString(0, "External SRAM Test...");
    if ((sram = TestExternalRam(RAMEND + 1, 0x7FFF)) == 0x7FFF - RAMEND)
        NutPrintFormat(0, "OK\r\n", sram);
#endif
    /*
     * Test Ethernet controller hardware.
     */
    TestRealtek();
    /*
     * Test Ethernet controller hardware.
     */
    NutPrintString(0, "I/O Port Test...     ");
    if (TestPorts() == 0)
        NutPrintString(0, "OK\r\n");
    /*
     * Return if running without serial port.
     */
    if (uart_bs < 0)
        return;
    for (;;) {
        for (;;) {
            while (GetChar());
            NutPrintString_P(0, menu1_P);
            if (sram)
                NutPrintString_P(0, menu2_P);
            NutPrintString_P(0, menu3_P);
            if (sram > 8191)
                NutPrintString_P(0, menu4_P);
            while ((ch = GetChar()) == 0);
            if (sram > 8191 && (ch == 'x' || ch == 'X'))
                break;
            if (sram && (ch == 'b' || ch == 'B'))
                SendBroadcasts();
            if (ch == 'e' || ch == 'E')
                LoopRealtek();
            if (ch == 'f' || ch == 'F')
                FindRealtek();
            if (ch == 'r' || ch == 'R')
                LoopReset();
            if (ch == 's' || ch == 'S')
                LoopSRAM();
        }

        /*
         * Input MAC address.
         */
        for (;;) {
            NutPrintFormat(0,
                           "\r\nMAC address  (%02X%02X%02X%02X%02X%02X): ",
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
            NutPrintString(0, "\r\nBad MAC address");
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
            NutPrintString(0, "\r\nUsing DHCP or ARP method (Y): ");
            GetLine(inbuff, sizeof(inbuff));
            if (strlen(inbuff) == 0 || inbuff[0] == 'Y' || inbuff[0] == 'y')
                break;
        }
    }
    NutPrintString(0, "\r\n\r\n");
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
     * Initialize heap.
     */
    if ((short) RAMEND - (short) (&__bss_end) > 384)
        NutHeapAdd(&__bss_end, (u_short) RAMEND - 256 - (u_short) (&__bss_end));
    if (sram)
        NutHeapAdd((void *) (RAMEND + 1), sram);
    /*
     * Create idle thread. This will in turn create
     * our NutMain thread.
     */
    NutThreadCreate("idle", idle, 0, 384);
    NutPrintString(0, "Never!");
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
