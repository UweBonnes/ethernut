#include <stdio.h>
#include <string.h>

#include <dev/nicrtl.h>
#include <dev/lanc111.h>
#include <dev/urom.h>

#include <sys/timer.h>
#include <sys/thread.h>
#include <sys/confnet.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <net/route.h>
#include <pro/httpd.h>
#include <pro/dhcp.h>
#include <sys/atom.h>

#include "basemon.h"
#include "webdemo.h"

static char *states[] = { "TRM",
    "<FONT COLOR=#CC0000>RUN</FONT>",
    "<FONT COLOR=#339966>RDY</FONT>",
    "SLP"
};


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
                  (uptr_t) tdp->td_sp - (uptr_t) tdp->td_memory, *((u_long *) tdp->td_memory) != DEADBEEF ? "Corr" : "OK");
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

    NutEnterCritical();
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
    NutExitCritical();
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
    DoCheckboxes(stream, "DDRA", inb(DDRA));
    DoCheckboxes(stream, "PINA", inb(PINA));
    DoCheckboxes(stream, "PORTA", inb(PORTA));

    fputs_P(trow_P, stream);
    DoCheckboxes(stream, "DDRB", inb(DDRB));
    DoCheckboxes(stream, "PINB", inb(PINB));
    DoCheckboxes(stream, "PORTB", inb(PORTB));

    fputs_P(trow_P, stream);
    DoCheckboxes(stream, "PORTC", inb(PORTC));

    fputs_P(trow_P, stream);
    DoCheckboxes(stream, "DDRD", inb(DDRD));
    DoCheckboxes(stream, "PIND", inb(PIND));
    DoCheckboxes(stream, "PORTD", inb(PORTD));

    fputs_P(trow_P, stream);
    DoCheckboxes(stream, "DDRE", inb(DDRE));
    DoCheckboxes(stream, "PINE", inb(PINE));
    DoCheckboxes(stream, "PORTE", inb(PORTE));

    fputs_P(trow_P, stream);
    DoCheckboxes(stream, "PINF", inb(PINF));

    fputs_P(tbot_P, stream);
    fflush(stream);

    return 0;
}

THREAD(WebDemo, arg)
{
    TCPSOCKET *sock;
    FILE *stream;
    u_long ip_addr;             /* ICCAVR bugfix */
    static prog_char netfail_P[] = "\nFailed to configure network " "interface: Ethernut stopped!\n\x07";
    /*
     * Register Realtek controller at address 8300 hex
     * and interrupt 5.
     */
    if(nic == 1)
        NutRegisterDevice(&devEth0, 0x8300, 5);
    else
        NutRegisterDevice(&devSmsc111, 0, 0);

    /*
     * Configure lan interface.
     */
    ip_addr = inet_addr(my_ip);
    if (ip_addr) {
        if (NutNetIfConfig("eth0", my_mac, ip_addr, inet_addr(my_mask))) {
            if (uart_bs >= 0) {
                printf_P(netfail_P);
                for (;;)
                    NutSleep(1000);
            } else {
                asm("cli");
                asm("call 0");
            }
        }
    } else if (NutDhcpIfConfig("eth0", my_mac, 60000)) {
        if (uart_bs >= 0) {
            printf_P(netfail_P);
            for (;;)
                NutSleep(1000);
        } else {
            asm("cli");
            asm("call 0");
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
        printf("MAC  %02X-%02X-%02X-%02X-%02X-%02X\nIP   %s",
                       confnet.cdn_mac[0], confnet.cdn_mac[1],
                       confnet.cdn_mac[2], confnet.cdn_mac[3], confnet.cdn_mac[4], confnet.cdn_mac[5],
                       inet_ntoa(confnet.cdn_ip_addr));
        printf("\nMask %s", inet_ntoa(confnet.cdn_ip_mask));
    }

    /*
     * Add optional default route.
     */
    if (inet_addr(my_ip) && inet_addr(my_gate)) {
        if (uart_bs >= 0)
            printf("\nAdd gateway %s", my_gate);
        NutIpRouteAdd(0, 0, inet_addr(my_gate), &devEth0);
    }

    else if (confnet.cdn_gateway && uart_bs >= 0)
        printf("\nGate %s", inet_ntoa(confnet.cdn_gateway));
    if (uart_bs >= 0) {
        IFNET *ifn;

        if(nic == 1)
            ifn = (IFNET *) (devEth0.dev_icb);
        else
            ifn = (IFNET *) (devSmsc111.dev_icb);

        printf("\nHTTP server running. URL http://%s/\n", inet_ntoa(ifn->if_local_ip));
    }

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

