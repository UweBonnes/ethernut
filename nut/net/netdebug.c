/*
 * Copyright (C) 2001-2003 by egnite Software GmbH
 *
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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
 * \file net/netdebug.c
 * \brief Network debug helper functions.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/icmp.h>
#include <netinet/ip_icmp.h>
#include <netinet/ipcsum.h>
#include <net/netdebug.h>
#include <sys/socket.h>

extern TCPSOCKET *tcpSocketList;
extern UDPSOCKET *udpSocketList;

FILE *__tcp_trs;                /*!< \brief TCP trace output stream. */
uint_fast8_t __tcp_trf;               /*!< \brief TCP trace flags. */

void NutDumpTcpHeader(FILE * stream, char * ds, TCPSOCKET * sock, NETBUF * nb)
{
    static const char fmt[] PROGMEM = "%s%p[%u]-SEQ(%lx)";
    TCPHDR *th = (TCPHDR *) nb->nb_tp.vp;

    fprintf_P(stream, fmt, ds, sock, (unsigned int)nb->nb_ap.sz, ntohl(th->th_seq));
    if (th->th_flags & TH_ACK)
        fprintf(stream, "-ACK(%lx)", ntohl(th->th_ack));
    if (th->th_flags & TH_FIN)
        fputs("-FIN", stream);
    if (th->th_flags & TH_SYN)
        fputs("-SYN", stream);
    if (th->th_flags & TH_RST)
        fputs("-RST", stream);
    if (th->th_flags & TH_PUSH)
        fputs("-PSH", stream);
    if (th->th_flags & TH_URG)
        fputs("-URG", stream);
    fputs("\n", stream);
}

void NutDumpSockState(FILE * stream, uint8_t state, char * lead, char * trail)
{
    if (lead)
        fputs(lead, stream);
    switch (state) {
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
        fputs("ESTABL", stream);
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
    case TCPS_DESTROY:
        fputs("DESTROY", stream);
    default:
        fputs("?UNK?", stream);
        break;
    }
    if (trail)
        fputs(trail, stream);
}


void NutDumpSocketList(FILE * stream)
{
    TCPSOCKET *ts;
    UDPSOCKET *us;

    static const char fmt1[] PROGMEM = "%10p TCP %15s:%-6u ";
    static const char fmt2[] PROGMEM = "%10p UDP %6u\r\n";

    fputs("\r\nSocket     Typ Local                  Remote                 State\n", stream);
    /*         1234567890 123 123456789012345:123456 123456789012345:123456 */

    for (ts = tcpSocketList; ts; ts = ts->so_next) {
        fprintf_P(stream, fmt1, ts, inet_ntoa(ts->so_local_addr), ntohs(ts->so_local_port));
        fprintf(stream, "%15s:%-6u ", inet_ntoa(ts->so_remote_addr), ntohs(ts->so_remote_port));
        NutDumpSockState(stream, ts->so_state, 0, "\r\n");
    }
    for (us = udpSocketList; us; us = us->so_next) {
        fprintf_P(stream, fmt2, us, ntohs(us->so_local_port));
    }
}


/*!
 * \brief Control TCP tracing.
 *
 * \param stream Pointer to a previously opened stream or null to leave
 *               it unchanged.
 * \param flags  Flags to enable specific traces.
 */
void NutTraceTcp(FILE * stream, uint8_t flags)
{
    if (stream)
        __tcp_trs = stream;
    if (__tcp_trs)
        __tcp_trf = flags;
    else
        __tcp_trf = 0;
}
