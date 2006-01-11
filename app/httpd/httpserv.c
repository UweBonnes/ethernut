/*
 * Copyright (C) 2001-2004 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.13  2006/01/11 08:32:57  hwmaier
 * Added explicit type casts to silence a few avr-gcc 3.4.3 warning messages
 *
 * Revision 1.12  2005/11/22 09:14:13  haraldkipp
 * Replaced specific device names by generalized macros.
 *
 * Revision 1.11  2005/10/16 23:22:20  hwmaier
 * Removed unreferenced nutconfig.h include statement
 *
 * Revision 1.10  2005/08/05 11:32:50  olereinhardt
 * Added SSI and ASP sample
 *
 * Revision 1.9  2005/04/05 18:04:17  haraldkipp
 * Support for ARM7 Wolf Board added.
 *
 * Revision 1.8  2005/02/23 04:39:26  hwmaier
 * no message
 *
 * Revision 1.7  2005/02/22 02:44:34  hwmaier
 * Changes to compile as well for AT90CAN128 device.
 *
 * Revision 1.6  2004/12/16 10:17:18  haraldkipp
 * Added Mikael Adolfsson's excellent parameter parsing routines.
 *
 * Revision 1.5  2004/03/16 16:48:26  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.4  2003/11/04 17:46:52  haraldkipp
 * Adapted to Ethernut 2
 *
 * Revision 1.3  2003/09/29 16:33:12  haraldkipp
 * Using portable strtok and strtok_r
 *
 * Revision 1.2  2003/08/07 08:27:58  haraldkipp
 * Bugfix, remote not displayed in socket list
 *
 * Revision 1.1  2003/07/20 15:56:14  haraldkipp
 * *** empty log message ***
 *
 */

/*!
 * \example httpd/httpserv.c
 *
 * Simple multithreaded HTTP daemon.
 */

/* These values are used if there is no valid configuration in EEPROM. */
#define MYMAC   0x00, 0x06, 0x98, 0x00, 0x00, 0x00
#define MYIP    "192.168.192.100"
#define MYMASK  "255.255.255.0"

#include <cfg/os.h>

#include <string.h>
#include <io.h>

#include <dev/board.h>
#include <dev/urom.h>

#include <sys/version.h>
#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/heap.h>
#include <sys/confnet.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <pro/httpd.h>
#include <pro/dhcp.h>
#include <pro/ssi.h>
#include <pro/asp.h>

#ifdef NUTDEBUG
#include <sys/osdebug.h>
#include <net/netdebug.h>
#endif

static char *html_mt = "text/html";


/**************************************************************/
/*  ASPCallback                                               */
/*                                                            */
/* This routine must have been registered by                  */
/* NutRegisterAspCallback() and is automatically called by    */
/* NutHttpProcessFileRequest() when the server process a page */
/* with an asp function.                                      */
/*                                                            */
/* Return 0 on success, -1 otherwise.                         */
/**************************************************************/


static int ASPCallback (char *pASPFunction, FILE *stream)
{
    if (strcmp(pASPFunction, "usr_date") == 0) {
        fprintf(stream, "Dummy example: 01.01.2005");
        return(0);
    }

    if (strcmp(pASPFunction, "usr_time") == 0) {
        fprintf(stream, "Dummy example: 12:15:02");
        return(0);
    }

    return (-1);
}

/*
 * CGI Sample: Show request parameters.
 *
 * See httpd.h for REQUEST structure.
 *
 * This routine must have been registered by NutRegisterCgi() and is
 * automatically called by NutHttpProcessRequest() when the client
 * request the URL 'cgi-bin/test.cgi'.
 */
static int ShowQuery(FILE * stream, REQUEST * req)
{
    char *cp;
    /*
     * This may look a little bit weird if you are not used to C programming
     * for flash microcontrollers. The special type 'prog_char' forces the
     * string literals to be placed in flash ROM. This saves us a lot of
     * precious RAM.
     */
    static prog_char head[] = "<HTML><HEAD><TITLE>Parameters</TITLE></HEAD><BODY><H1>Parameters</H1>";
    static prog_char foot[] = "</BODY></HTML>";
    static prog_char req_fmt[] = "Method: %s<BR>\r\nVersion: HTTP/%d.%d<BR>\r\nContent length: %d<BR>\r\n";
    static prog_char url_fmt[] = "URL: %s<BR>\r\n";
    static prog_char query_fmt[] = "Argument: %s<BR>\r\n";
    static prog_char type_fmt[] = "Content type: %s<BR>\r\n";
    static prog_char cookie_fmt[] = "Cookie: %s<BR>\r\n";
    static prog_char auth_fmt[] = "Auth info: %s<BR>\r\n";
    static prog_char agent_fmt[] = "User agent: %s<BR>\r\n";

    /* These useful API calls create a HTTP response for us. */
    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, html_mt, -1);

    /* Send HTML header. */
    fputs_P(head, stream);

    /*
     * Send request parameters.
     */
    switch (req->req_method) {
    case METHOD_GET:
        cp = "GET";
        break;
    case METHOD_POST:
        cp = "POST";
        break;
    case METHOD_HEAD:
        cp = "HEAD";
        break;
    default:
        cp = "UNKNOWN";
        break;
    }
    fprintf_P(stream, req_fmt, cp, req->req_version / 10, req->req_version % 10, req->req_length);
    if (req->req_url)
        fprintf_P(stream, url_fmt, req->req_url);
    if (req->req_query)
        fprintf_P(stream, query_fmt, req->req_query);
    if (req->req_type)
        fprintf_P(stream, type_fmt, req->req_type);
    if (req->req_cookie)
        fprintf_P(stream, cookie_fmt, req->req_cookie);
    if (req->req_auth)
        fprintf_P(stream, auth_fmt, req->req_auth);
    if (req->req_agent)
        fprintf_P(stream, agent_fmt, req->req_agent);

    /* Send HTML footer and flush output buffer. */
    fputs_P(foot, stream);
    fflush(stream);

    return 0;
}

/*
 * CGI Sample: Show list of threads.
 *
 * This routine must have been registered by NutRegisterCgi() and is
 * automatically called by NutHttpProcessRequest() when the client
 * request the URL 'cgi-bin/threads.cgi'.
 */
static int ShowThreads(FILE * stream, REQUEST * req)
{
    static prog_char head[] = "<HTML><HEAD><TITLE>Threads</TITLE></HEAD><BODY><H1>Threads</H1>\r\n"
        "<TABLE BORDER><TR><TH>Handle</TH><TH>Name</TH><TH>Priority</TH><TH>Status</TH><TH>Event<BR>Queue</TH><TH>Timer</TH><TH>Stack-<BR>pointer</TH><TH>Free<BR>Stack</TH></TR>\r\n";
#if defined(__AVR__)
    static prog_char tfmt[] =
        "<TR><TD>%04X</TD><TD>%s</TD><TD>%u</TD><TD>%s</TD><TD>%04X</TD><TD>%04X</TD><TD>%04X</TD><TD>%u</TD><TD>%s</TD></TR>\r\n";
#else
    static prog_char tfmt[] =
        "<TR><TD>%08lX</TD><TD>%s</TD><TD>%u</TD><TD>%s</TD><TD>%08lX</TD><TD>%08lX</TD><TD>%08lX</TD><TD>%lu</TD><TD>%s</TD></TR>\r\n";
#endif
    static prog_char foot[] = "</TABLE></BODY></HTML>";
    static char *thread_states[] = { "TRM", "<FONT COLOR=#CC0000>RUN</FONT>", "<FONT COLOR=#339966>RDY</FONT>", "SLP" };
    NUTTHREADINFO *tdp = nutThreadList;

    /* Send HTTP response. */
    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, html_mt, -1);

    /* Send HTML header. */
    fputs_P(head, stream);
    for (tdp = nutThreadList; tdp; tdp = tdp->td_next) {
        fprintf_P(stream, tfmt, (uptr_t) tdp, tdp->td_name, tdp->td_priority,
                  thread_states[tdp->td_state], (uptr_t) tdp->td_queue, (uptr_t) tdp->td_timer,
                  (uptr_t) tdp->td_sp, (uptr_t) tdp->td_sp - (uptr_t) tdp->td_memory,
                  *((u_long *) tdp->td_memory) != DEADBEEF ? "Corr" : "OK");
    }
    fputs_P(foot, stream);
    fflush(stream);

    return 0;
}

/*
 * CGI Sample: Show list of timers.
 *
 * This routine must have been registered by NutRegisterCgi() and is
 * automatically called by NutHttpProcessRequest() when the client
 * request the URL 'cgi-bin/timers.cgi'.
 */
static int ShowTimers(FILE * stream, REQUEST * req)
{
    static prog_char head[] = "<HTML><HEAD><TITLE>Timers</TITLE></HEAD><BODY><H1>Timers</H1>\r\n";
    static prog_char thead[] =
        "<TABLE BORDER><TR><TH>Handle</TH><TH>Countdown</TH><TH>Tick Reload</TH><TH>Callback<BR>Address</TH><TH>Callback<BR>Argument</TH></TR>\r\n";
#if defined(__AVR__)
    static prog_char tfmt[] = "<TR><TD>%04X</TD><TD>%lu</TD><TD>%lu</TD><TD>%04X</TD><TD>%04X</TD></TR>\r\n";
#else
    static prog_char tfmt[] = "<TR><TD>%08lX</TD><TD>%lu</TD><TD>%lu</TD><TD>%08lX</TD><TD>%08lX</TD></TR>\r\n";
#endif
    static prog_char foot[] = "</TABLE></BODY></HTML>";
    NUTTIMERINFO *tnp;
    u_long ticks_left;

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, html_mt, -1);

    /* Send HTML header. */
    fputs_P(head, stream);
    if ((tnp = nutTimerList) != 0) {
        fputs_P(thead, stream);
        ticks_left = 0;
        while (tnp) {
            ticks_left += tnp->tn_ticks_left;
            fprintf_P(stream, tfmt, (uptr_t) tnp, ticks_left, tnp->tn_ticks, (uptr_t) tnp->tn_callback, (uptr_t) tnp->tn_arg);
            tnp = tnp->tn_next;
        }
    }

    fputs_P(foot, stream);
    fflush(stream);

    return 0;
}

/*
 * CGI Sample: Show list of sockets.
 *
 * This routine must have been registered by NutRegisterCgi() and is
 * automatically called by NutHttpProcessRequest() when the client
 * request the URL 'cgi-bin/sockets.cgi'.
 */
static int ShowSockets(FILE * stream, REQUEST * req)
{
    /* String literals are kept in flash ROM. */
    static prog_char head[] = "<HTML><HEAD><TITLE>Sockets</TITLE></HEAD>"
        "<BODY><H1>Sockets</H1>\r\n"
        "<TABLE BORDER><TR><TH>Handle</TH><TH>Type</TH><TH>Local</TH><TH>Remote</TH><TH>Status</TH></TR>\r\n";
#if defined(__AVR__)
    static prog_char tfmt1[] = "<TR><TD>%04X</TD><TD>TCP</TD><TD>%s:%u</TD>";
#else
    static prog_char tfmt1[] = "<TR><TD>%08lX</TD><TD>TCP</TD><TD>%s:%u</TD>";
#endif
    static prog_char tfmt2[] = "<TD>%s:%u</TD><TD>";
    static prog_char foot[] = "</TABLE></BODY></HTML>";
    static prog_char st_listen[] = "LISTEN";
    static prog_char st_synsent[] = "SYNSENT";
    static prog_char st_synrcvd[] = "SYNRCVD";
    static prog_char st_estab[] = "<FONT COLOR=#CC0000>ESTABL</FONT>";
    static prog_char st_finwait1[] = "FINWAIT1";
    static prog_char st_finwait2[] = "FINWAIT2";
    static prog_char st_closewait[] = "CLOSEWAIT";
    static prog_char st_closing[] = "CLOSING";
    static prog_char st_lastack[] = "LASTACK";
    static prog_char st_timewait[] = "TIMEWAIT";
    static prog_char st_closed[] = "CLOSED";
    static prog_char st_unknown[] = "UNKNOWN";
    prog_char *st_P;
    extern TCPSOCKET *tcpSocketList;
    TCPSOCKET *ts;

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, html_mt, -1);

    /* Send HTML header. */
    fputs_P(head, stream);
    for (ts = tcpSocketList; ts; ts = ts->so_next) {
        switch (ts->so_state) {
        case TCPS_LISTEN:
            st_P = (prog_char *) st_listen;
            break;
        case TCPS_SYN_SENT:
            st_P = (prog_char *) st_synsent;
            break;
        case TCPS_SYN_RECEIVED:
            st_P = (prog_char *) st_synrcvd;
            break;
        case TCPS_ESTABLISHED:
            st_P = (prog_char *) st_estab;
            break;
        case TCPS_FIN_WAIT_1:
            st_P = (prog_char *) st_finwait1;
            break;
        case TCPS_FIN_WAIT_2:
            st_P = (prog_char *) st_finwait2;
            break;
        case TCPS_CLOSE_WAIT:
            st_P = (prog_char *) st_closewait;
            break;
        case TCPS_CLOSING:
            st_P = (prog_char *) st_closing;
            break;
        case TCPS_LAST_ACK:
            st_P = (prog_char *) st_lastack;
            break;
        case TCPS_TIME_WAIT:
            st_P = (prog_char *) st_timewait;
            break;
        case TCPS_CLOSED:
            st_P = (prog_char *) st_closed;
            break;
        default:
            st_P = (prog_char *) st_unknown;
            break;
        }
        /*
         * Fixed a bug reported by Zhao Weigang.
         */
        fprintf_P(stream, tfmt1, (uptr_t) ts, inet_ntoa(ts->so_local_addr), ntohs(ts->so_local_port));
        fprintf_P(stream, tfmt2, inet_ntoa(ts->so_remote_addr), ntohs(ts->so_remote_port));
        fputs_P(st_P, stream);
        fputs("</TD></TR>\r\n", stream);
        fflush(stream);
    }

    fputs_P(foot, stream);
    fflush(stream);

    return 0;
}

/*
 * CGI Sample: Proccessing a form.
 *
 * This routine must have been registered by NutRegisterCgi() and is
 * automatically called by NutHttpProcessRequest() when the client
 * request the URL 'cgi-bin/form.cgi'.
 *
 * Thanks to Tom Boettger, who provided this sample for ICCAVR.
 */
int ShowForm(FILE * stream, REQUEST * req)
{
    static prog_char html_head[] = "<HTML><BODY><BR><H1>Form Result</H1><BR><BR>";
    static prog_char html_body[] = "<BR><BR><p><a href=\"../index.html\">return to main</a></BODY></HTML></p>";

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, html_mt, -1);

    /* Send HTML header. */
    fputs_P(html_head, stream);

    if (req->req_query) {
        char *name;
        char *value;
        int i;
        int count;

        count = NutHttpGetParameterCount(req);
        /* Extract count parameters. */
        for (i = 0; i < count; i++) {
            name = NutHttpGetParameterName(req, i);
            value = NutHttpGetParameterValue(req, i);

            /* Send the parameters back to the client. */

#ifdef __IMAGECRAFT__
            fprintf(stream, "%s: %s<BR>\r\n", name, value);
#else
            fprintf_P(stream, PSTR("%s: %s<BR>\r\n"), name, value);
#endif
        }
    }

    fputs_P(html_body, stream);
    fflush(stream);

    return 0;
}

/*! \fn Service(void *arg)
 * \brief HTTP service thread.
 *
 * The endless loop in this thread waits for a client connect,
 * processes the HTTP request and disconnects. Nut/Net doesn't
 * support a server backlog. If one client has established a
 * connection, further connect attempts will be rejected.
 * Typically browsers open more than one connection in order
 * to load images concurrently. So we run this routine by
 * several threads.
 *
 */
THREAD(Service, arg)
{
    TCPSOCKET *sock;
    FILE *stream;
    u_char id = (u_char) ((uptr_t) arg);

    /*
     * Now loop endless for connections.
     */
    for (;;) {

        /*
         * Create a socket.
         */
        if ((sock = NutTcpCreateSocket()) == 0) {
            printf("[%u] Creating socket failed\n", id);
            NutSleep(5000);
            continue;
        }

        /*
         * Listen on port 80. This call will block until we get a connection
         * from a client.
         */
        NutTcpAccept(sock, 80);
#if defined(__AVR__)
        printf("[%u] Connected, %u bytes free\n", id, NutHeapAvailable());
#else
        printf("[%u] Connected, %lu bytes free\n", id, NutHeapAvailable());
#endif

        /*
         * Wait until at least 8 kByte of free RAM is available. This will
         * keep the client connected in low memory situations.
         */
#if defined(__AVR__)
        while (NutHeapAvailable() < 8192) {
#else
        while (NutHeapAvailable() < 4096) {
#endif
            printf("[%u] Low mem\n", id);
            NutSleep(1000);
        }

        /*
         * Associate a stream with the socket so we can use standard I/O calls.
         */
        if ((stream = _fdopen((int) ((uptr_t) sock), "r+b")) == 0) {
            printf("[%u] Creating stream device failed\n", id);
        } else {
            /*
             * This API call saves us a lot of work. It will parse the
             * client's HTTP request, send any requested file from the
             * registered file system or handle CGI requests by calling
             * our registered CGI routine.
             */
            NutHttpProcessRequest(stream);

            /*
             * Destroy the virtual stream device.
             */
            fclose(stream);
        }

        /*
         * Close our socket.
         */
        NutTcpCloseSocket(sock);
        printf("[%u] Disconnected\n", id);
    }
}

/*!
 * \brief Main application routine.
 *
 * Nut/OS automatically calls this entry after initialization.
 */
int main(void)
{
    u_long baud = 115200;
    u_char i;

    /*
     * Initialize the uart device.
     */
    NutRegisterDevice(&DEV_DEBUG, 0, 0);
    freopen(DEV_DEBUG_NAME, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);
    NutSleep(200);
    printf("\n\nNut/OS %s HTTP Daemon...", NutVersionString());

#ifdef NUTDEBUG
    NutTraceTcp(stdout, 0);
    NutTraceOs(stdout, 0);
    NutTraceHeap(stdout, 0);
    NutTracePPP(stdout, 0);
#endif

    /*
     * Register Ethernet controller.
     */
    if (NutRegisterDevice(&DEV_ETHER, 0, 0)) {
        puts("Registering device failed");
    }

    /*
     * LAN configuration using EEPROM values or DHCP/ARP method.
     * If it fails, use fixed values.
     */
    if (NutDhcpIfConfig("eth0", 0, 60000)) {
        u_char mac[] = { MYMAC };
        u_long ip_addr = inet_addr(MYIP);
        u_long ip_mask = inet_addr(MYMASK);

        puts("EEPROM/DHCP/ARP config failed");
        NutNetIfConfig("eth0", mac, ip_addr, ip_mask);
    }
    printf("%s ready\n", inet_ntoa(confnet.cdn_ip_addr));

    /*
     * Register our device for the file system.
     */
    NutRegisterDevice(&devUrom, 0, 0);

    /*
     * Register our CGI sample. This will be called
     * by http://host/cgi-bin/test.cgi?anyparams
     */
    NutRegisterCgi("test.cgi", ShowQuery);

    /*
     * Register some CGI samples, which display interesting
     * system informations.
     */
    NutRegisterCgi("threads.cgi", ShowThreads);
    NutRegisterCgi("timers.cgi", ShowTimers);
    NutRegisterCgi("sockets.cgi", ShowSockets);

    /*
     * Finally a CGI example to process a form.
     */
    NutRegisterCgi("form.cgi", ShowForm);

    /*
     * Protect the cgi-bin directory with
     * user and password.
     */
    NutRegisterAuth("cgi-bin", "root:root");

    /*
     * Register SSI and ASP handler
     */
    NutRegisterSsi();
    NutRegisterAsp();
    NutRegisterAspCallback(ASPCallback);
    /*
     * Start four server threads.
     */
    for (i = 1; i <= 4; i++) {
        char *thname = "httpd0";

        thname[5] = '0' + i;
        NutThreadCreate(thname, Service, (void *) (uptr_t) i, NUT_THREAD_MAINSTACK);
    }

    /*
     * We could do something useful here, like serving a watchdog.
     */
    NutThreadSetPriority(254);
    for (;;)
        NutSleep(60000);
}
