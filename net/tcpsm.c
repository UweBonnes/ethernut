/*
 * Copyright (C) 2001-2007 by egnite Software GmbH. All rights reserved.
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
 * -
 * Portions Copyright (C) 2000 David J. Hudson <dave@humbug.demon.co.uk>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "copying-gpl.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "copying-liquorice.txt" for details.
 * -
 * Portions Copyright (c) 1983, 1993 by
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * -
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/*
 * $Log$
 * Revision 1.23  2007/02/15 15:59:59  haraldkipp
 * Serious bug in the TCP state machine froze socket connection on 32-bit
 * platforms.
 *
 * Revision 1.22  2006/10/05 17:25:41  haraldkipp
 * Avoid possible alignment errors. Fixes bug #1567748.
 *
 * Revision 1.21  2006/05/15 12:49:12  haraldkipp
 * ICCAVR doesn't accept void pointer calculation.
 *
 * Revision 1.20  2006/03/21 21:22:19  drsung
 * Enhancement made to TCP state machine. Now TCP options
 * are read from peer and at least the maximum segment size is stored.
 *
 * Revision 1.19  2005/04/30 16:42:42  chaac
 * Fixed bug in handling of NUTDEBUG. Added include for cfg/os.h. If NUTDEBUG
 * is defined in NutConf, it will make effect where it is used.
 *
 * Revision 1.18  2005/04/05 17:44:57  haraldkipp
 * Made stack space configurable.
 *
 * Revision 1.17  2005/03/30 15:17:58  mrjones4u
 * Defussed race condition in NutTcpStateActiveOpenEvent where the NutEventWait would be called after sock->so_ac_tq had been signaled and therefore the calling thread will hang due to usage of NutEventBroadcast which will not ‘store’ the signaled state. This condition can e.g. occur when attempting connection an unconnected target port on an active host. The returning RST would be processed and signaled before the NutEventWait is called.
 *
 * Revision 1.16  2005/02/04 17:17:49  haraldkipp
 * Unused include files removed.
 *
 * Revision 1.15  2005/01/21 16:49:46  freckle
 * Seperated calls to NutEventPostAsync between Threads and IRQs
 *
 * Revision 1.14  2005/01/03 08:43:29  haraldkipp
 * Replaced unprotected calls to NutEventPostAsync() by late calls to NutEventPost().
 * This should fix the infrequent system halts/resets. The event to the transmitter
 * waiting queue will be broadcasted on relevant state changes.
 *
 * Revision 1.13  2004/07/30 19:54:46  drsung
 * Some code of TCP stack redesigned. Round trip time calculation is now
 * supported. Fixed several bugs in TCP state machine. Now TCP connections
 * should be more reliable under heavy traffic or poor physical connections.
 *
 * Revision 1.12  2004/04/15 11:08:21  haraldkipp
 * Bugfix: Sequence number had not been incremented on FIN segments.
 * Added Realtek hack to reduce concurrent connection problems.
 * Rely on TCP output to set the retransmission timer.
 *
 * Revision 1.11  2004/02/28 20:14:38  drsung
 * Merge from nut-3_4-release b/c of bugfixes.
 *
 * Revision 1.9.2.1  2004/02/28 20:02:23  drsung
 * Bugfix in several tcp state functions. Swapped around the check
 * for ACK and SYN flag. Because initial SYN packets don't have
 * an ACK flag, recevied SYN packets were never rejected.
 * Thanks to Damian Slee, who discovered that.
 *
 * Revision 1.9  2004/01/25 11:50:03  drsung
 * setting correct error code on timeout while NutTcpConnect.
 *
 * Revision 1.8  2004/01/25 11:29:48  drsung
 * bugfix for connection establishing.
 *
 * Revision 1.7  2004/01/14 19:35:19  drsung
 * New TCP output buffer handling and fixed not starting retransmission timer for NutTcpConnect.
 *
 * Revision 1.6  2003/11/28 19:49:58  haraldkipp
 * TCP connections suddenly drop during transmission.
 * Bug in retransmission timer fixed.
 *
 * Revision 1.5  2003/11/04 17:57:35  haraldkipp
 * Bugfix: Race condition left socket in close-wait state
 *
 * Revision 1.4  2003/11/03 16:48:02  haraldkipp
 * Use the system timer for retransmission timouts
 *
 * Revision 1.3  2003/08/14 15:10:31  haraldkipp
 * Two bugfixes: 1. NutTcpAccept fails if caller got higher priority.
 * 2. Incoming TCP NETBUFs will never be released if TCP is not used by
 * the application.
 *
 * Revision 1.2  2003/07/13 19:22:23  haraldkipp
 * TCP transfer speed increased by changing the character receive buffer
 * in TCPSOCKET to a NETBUF queue. (More confusing diff lines by using
 * indent, sorry.)
 *
 * Revision 1.1.1.1  2003/05/09 14:41:42  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.20  2003/05/06 18:20:02  harald
 * Stack size reduced
 *
 * Revision 1.19  2003/04/01 18:36:11  harald
 * Added forced ACK response on same sequence
 *
 * Revision 1.18  2003/03/31 12:29:45  harald
 * Check NEBUF allocation
 *
 * Revision 1.17  2003/02/04 18:14:57  harald
 * Version 3 released
 *
 * Revision 1.16  2003/01/14 16:51:29  harald
 * Handle possible deadlock in the TCP state machine in low memory situations.
 * Fixed: TCP might fail to process incoming packets on slow connections.
 *
 * Revision 1.15  2002/09/15 17:05:41  harald
 * Silently ignore late SYNs.
 * Detect host down in local networks during connect.
 * Avoid re-sending packets too soon.
 *
 * Revision 1.14  2002/09/03 17:42:20  harald
 * Buffer sequences received in advance
 *
 * Revision 1.13  2002/08/16 17:54:56  harald
 * Count out of sequence drops
 *
 * Revision 1.12  2002/06/26 17:29:36  harald
 * First pre-release with 2.4 stack
 *
 */

#include <cfg/os.h>
#include <cfg/tcp.h>

#include <sys/thread.h>
#include <sys/heap.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <net/errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/route.h>
#include <sys/socket.h>
#include <netinet/tcputil.h>
#include <netinet/tcp.h>

#ifdef NUTDEBUG
#include <net/netdebug.h>
#endif

#ifndef NUT_THREAD_TCPSMSTACK
#define NUT_THREAD_TCPSMSTACK   512
#endif

extern TCPSOCKET *tcpSocketList;

/*!
 * \addtogroup xgTCP
 */
/*@{*/

HANDLE tcp_in_rdy;
NETBUF *volatile tcp_in_nbq;
static u_short tcp_in_cnt;
static HANDLE tcpThread = 0;

/* ================================================================
 * Helper functions
 * ================================================================
 */

/*!
 * \brief Reads TCP option fields if any, and writes the data to
 *        the socket descriptor if important for us.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 * \param nb   Network buffer structure containing a TCP segment.
 */
static void NutTcpInputOptions(TCPSOCKET * sock, NETBUF * nb)
{
    u_char *cp;
    u_short s;
    
    /* any options there? */
    if (nb->nb_tp.sz <= sizeof (TCPHDR))
        return;
    
    /* loop through available options */
    for (cp = ((u_char*) nb->nb_tp.vp) + sizeof(TCPHDR); (*cp != TCPOPT_EOL) 
       && (cp - (u_char *)nb->nb_tp.vp < (int)nb->nb_tp.sz); )
    {
        switch (*cp)
        {
            /* On NOP just proceed to next option */
            case TCPOPT_NOP:
                cp++;
                continue;
                
            /* Read MAXSEG option */
            case TCPOPT_MAXSEG:
                s = ntohs(((u_short)cp[2] << 8) | cp[3]);
                if (s < sock->so_mss)
                    sock->so_mss = s;
                cp += TCPOLEN_MAXSEG;
                break;
            /* Ignore any other options */
            default:
                cp += *(u_char*) (cp + 1);
                break;
        }
    }
}

/*!
 * \brief Move application data in sync from the network buffer 
 *        structure to the socket's receive buffer.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 * \param nb   Network buffer structure containing a TCP segment.
 */
static void NutTcpProcessAppData(TCPSOCKET * sock, NETBUF * nb)
{
    /*
     * Add the NETBUF to the socket's input buffer.
     */
    if (sock->so_rx_buf) {
        NETBUF *nbp = sock->so_rx_buf;

        while (nbp->nb_next)
            nbp = nbp->nb_next;
        nbp->nb_next = nb;
    } else
        sock->so_rx_buf = nb;

    /*
     * Update the number of bytes available in the socket's input buffer
     * and the sequence number we expect next.
     */
    sock->so_rx_cnt += nb->nb_ap.sz;
    sock->so_rx_nxt += nb->nb_ap.sz;

    /*
     * Reduce our TCP window size.
     */
    if (nb->nb_ap.sz >= sock->so_rx_win)
        sock->so_rx_win = 0;
    else
        sock->so_rx_win -= nb->nb_ap.sz;

    /*
     * Set the socket's ACK flag. This will enable ACK transmission in
     * the next outgoing segment. If no more NETBUFs are queued, we
     * force immediate transmission of the ACK.
     */
    sock->so_tx_flags |= SO_ACK;
    if (nb->nb_next)
        nb->nb_next = 0;
    else
        sock->so_tx_flags |= SO_FORCE;

    NutTcpOutput(sock, 0, 0);
}

/*
 * \param sock Socket descriptor.
 */
static void NutTcpProcessSyn(TCPSOCKET * sock, IPHDR * ih, TCPHDR * th)
{
    u_short mss;
    NUTDEVICE *dev;
    IFNET *nif;

    sock->so_local_addr = ih->ip_dst;
    sock->so_remote_port = th->th_sport;
    sock->so_remote_addr = ih->ip_src;

    sock->so_rx_nxt = sock->so_tx_wl1 = sock->so_rx_isn = ntohl(th->th_seq);
    sock->so_rx_nxt++;
    sock->so_tx_win = ntohs(th->th_win);

    /*
     * To avoid unnecessary fragmentation, limit the
     * maximum segment size to the maximum transfer
     * unit of our interface.
     */
    if ((dev = NutIpRouteQuery(ih->ip_src, 0)) != 0) {
        nif = dev->dev_icb;
        mss = nif->if_mtu - sizeof(IPHDR) - sizeof(TCPHDR);
        if (sock->so_mss == 0 || sock->so_mss > mss)
            sock->so_mss = mss;

        /* Limit output buffer size to mms */
        if (sock->so_devobsz > sock->so_mss)
            sock->so_devobsz = sock->so_mss;
    }
}

/*!
 * \brief ACK processing.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 *
 */
static int NutTcpProcessAck(TCPSOCKET * sock, TCPHDR * th, u_short length)
{
    NETBUF *nb;
    u_long h_seq;
    u_long h_ack;

    /*
     * If remote acked something not yet send, reply immediately.
     */
    h_ack = ntohl(th->th_ack);
    if (h_ack > sock->so_tx_nxt) {
        sock->so_tx_flags |= SO_ACK | SO_FORCE;
        return 0;
    }

    /*
     * If the new sequence number or acknowledged sequence number
     * is above our last update, we adjust our transmit window.
     * Avoid dupe ACK processing on window updates.
     */
    if (h_ack == sock->so_tx_una) {
        h_seq = ntohl(th->th_seq);
        if (h_seq > sock->so_tx_wl1 || (h_seq == sock->so_tx_wl1 && h_ack >= sock->so_tx_wl2)) {
            sock->so_tx_win = ntohs(th->th_win);
            sock->so_tx_wl1 = h_seq;
            sock->so_tx_wl2 = h_ack;
        }
    }

    /*
     * Ignore old ACKs but wake up sleeping transmitter threads, because
     * the window size may have changed.
     */
    if (h_ack < sock->so_tx_una) {
        return 0;
    }

    /*
     * Process duplicate ACKs.
     */
    if (h_ack == sock->so_tx_una) {
        /*
         * Don't count, if nothing is waiting for ACK,
         * segment contains data or on SYN/FIN segments.
         */
        if (sock->so_tx_nbq && length == 0 && (th->th_flags & (TH_SYN | TH_FIN)) == 0) {
            /*
             * If dupe counter reaches it's limit, resend
             * the oldest unacknowledged netbuf.
             */
            if (++sock->so_tx_dup >= 3) {
                sock->so_tx_dup = 0;
#ifdef NUTDEBUG
                if (__tcp_trf)
                    NutDumpTcpHeader(__tcp_trs, "RET", sock, sock->so_tx_nbq);
#endif
                /*
                 * Retransmit first unacked packet from queue.
                 * Actually we got much more trouble if this fails.
                 */
                if (NutTcpStateRetranTimeout(sock))
                    return -1;
            }
        }
        return 0;
    }

    /*
     * We're here, so the ACK must have actually acked something
     */
    sock->so_tx_dup = 0;
    sock->so_tx_una = h_ack;

    /*
     * Bugfix contributed by Liu Limin: If the remote is slow and this
     * line is missing, then Ethernut will send a full data packet even
     * if the remote closed the window.
     */
    sock->so_tx_win = ntohs(th->th_win);

    /* 
     * Do round trip time calculation.
     */
    if (sock->so_rtt_seq && sock->so_rtt_seq < h_ack)
        NutTcpCalcRtt (sock);
    sock->so_rtt_seq = 0;
    /*
     * Remove all acknowledged netbufs.
     */
    while ((nb = sock->so_tx_nbq) != 0) {
        /* Calculate the sequence beyond this netbuf. */
        h_seq = ntohl(((TCPHDR *) (nb->nb_tp.vp))->th_seq) + nb->nb_ap.sz;
        if (((TCPHDR *) (nb->nb_tp.vp))->th_flags & (TH_SYN | TH_FIN)) {
            h_seq++;
        }
        //@@@printf ("[%04X]*: processack, check seq#: %lu\n", (u_short) sock, h_seq);
        if (h_seq > h_ack) {
            break;
        }
        sock->so_tx_nbq = nb->nb_next;
        NutNetBufFree(nb);
    }

    /*
     * Reset retransmit timer and wake up waiting transmissions.
     */
    if (sock->so_tx_nbq) {
        sock->so_retran_time = (u_short) NutGetMillis();
    } else {
        sock->so_retran_time = 0;
    }
    sock->so_retransmits = 0;

    return 0;
}



/* ================================================================
 * State changes.
 * ================================================================
 */
/*!
 * State change, possibly inform application.
 *
 * \param sock  Socket descriptor.
 * \param state New state to switch to.
 *
 * \return 0 on success, -1 on illegal state change attempt.
 */
static int NutTcpStateChange(TCPSOCKET * sock, u_char state)
{
    int rc = 0;
    ureg_t txf = 0;

    switch (sock->so_state) {
        /* Handle the most common case first. */
    case TCPS_ESTABLISHED:
        switch (state) {
        case TCPS_FIN_WAIT_1:
            /*
             * Closed by application.
             */
            sock->so_tx_flags |= SO_FIN | SO_ACK;
            txf = 1;

#ifdef RTLCONNECTHACK
            /*
             * Hack alert!
             * On the RTL8019AS we got a problem. Because of not handling
             * the CHRDY line, the controller drops outgoing packets when
             * a browser opens multiple connections concurrently, producing
             * several short incoming packets. Empirical test showed, that
             * a slight delay during connects and disconnects helped to
             * remarkably reduce this problem.
             */
            NutDelay(5);
#endif
            break;
        case TCPS_CLOSE_WAIT:
            /*
             * FIN received.
             */
            sock->so_tx_flags |= SO_ACK | SO_FORCE;
            txf = 1;
            break;
        default:
            rc = -1;
            break;
        }
        break;

    case TCPS_LISTEN:
        /*
         * SYN received.
         */
        if (state == TCPS_SYN_RECEIVED) {
            sock->so_tx_flags |= SO_SYN | SO_ACK;
            txf = 1;
#ifdef RTLCONNECTHACK
            /*
             * Hack alert!
             * On the RTL8019AS we got a problem. Because of not handling
             * the CHRDY line, the controller drops outgoing packets when
             * a browser opens multiple connections concurrently, producing
             * several short incoming packets. Empirical test showed, that
             * a slight delay during connects and disconnects helped to
             * remarkably reduce this problem.
             */
            NutDelay(5);
#endif
        } else
            rc = -1;
        break;

    case TCPS_SYN_SENT:
        switch (state) {
        case TCPS_LISTEN:
            /*
             * RST received on passive socket.
             */
            break;
        case TCPS_SYN_RECEIVED:
            /*
             * SYN received.
             */
            sock->so_tx_flags |= SO_SYN | SO_ACK;
            txf = 1;
            break;
        case TCPS_ESTABLISHED:
            /*
             * SYNACK received.
             */
            sock->so_tx_flags |= SO_ACK | SO_FORCE;
            txf = 1;
            break;
        default:
            rc = -1;
            break;
        }
        break;

    case TCPS_SYN_RECEIVED:
        switch (state) {
        case TCPS_LISTEN:
            /*
             * RST received on passive socket.
             */
            break;
        case TCPS_ESTABLISHED:
            /*
             * ACK of SYN received.
             */
            break;
        case TCPS_FIN_WAIT_1:
            /*
             * Closed by application.
             */
            sock->so_tx_flags |= SO_FIN;
            txf = 1;
            break;
        case TCPS_CLOSE_WAIT:
            /*
             * FIN received.
             */
            sock->so_tx_flags |= SO_FIN | SO_ACK;
            txf = 1;
            break;
        default:
            rc = -1;
            break;
        }
        break;

    case TCPS_FIN_WAIT_1:
        switch (state) {
        case TCPS_FIN_WAIT_1:
        case TCPS_FIN_WAIT_2:
            /*
             * ACK of FIN received.
             */
            break;
        case TCPS_CLOSING:
            /*
             * FIN received.
             */
            sock->so_tx_flags |= SO_ACK | SO_FORCE;
            txf = 1;
            break;
        case TCPS_TIME_WAIT:
            /*
             * FIN and ACK of FIN received.
             */
            break;
        default:
            rc = -1;
            break;
        }
        break;

    case TCPS_FIN_WAIT_2:
        /*
         * FIN received.
         */
        if (state != TCPS_TIME_WAIT)
            rc = -1;
        sock->so_tx_flags |= SO_ACK | SO_FORCE;
        txf = 1;
        break;

    case TCPS_CLOSE_WAIT:
        /*
         * Closed by application.
         */
        if (state == TCPS_LAST_ACK) {
            sock->so_tx_flags |= SO_FIN | SO_ACK;
            txf = 1;
        } else
            rc = -1;
        break;

    case TCPS_CLOSING:
        /*
         * ACK of FIN received.
         */
        if (state != TCPS_TIME_WAIT)
            rc = -1;
        break;

    case TCPS_LAST_ACK:
        rc = -1;
        break;

    case TCPS_TIME_WAIT:
        rc = -1;
        break;

    case TCPS_CLOSED:
        switch (state) {
        case TCPS_LISTEN:
            /*
             * Passive open by application.
             */
            break;
        case TCPS_SYN_SENT:
            /*
             * Active open by application.
             */
            sock->so_tx_flags |= SO_SYN;
            txf = 1;
            break;
        default:
            rc = -1;
            break;
        }
        break;
    }
#ifdef NUTDEBUG
    if (__tcp_trf) {
        fprintf(__tcp_trs, " %04x-", (u_int) sock);
        if (rc)
            NutDumpSockState(__tcp_trs, sock->so_state, "**ERR ", "**>");
        NutDumpSockState(__tcp_trs, state, "[>", "]");
    }
#endif

    if (rc == 0) {
        sock->so_state = state;
        if (txf && NutTcpOutput(sock, 0, 0)) {
            if (state == TCPS_SYN_SENT) {
                rc = -1;
                sock->so_last_error = EHOSTDOWN;
                NutEventPostAsync(&sock->so_ac_tq);
            }
        }
        if (state == TCPS_CLOSE_WAIT) {
            /*
             * Inform application.
             */
            NutEventBroadcast(&sock->so_rx_tq);
            NutEventBroadcast(&sock->so_pc_tq);
            NutEventBroadcast(&sock->so_ac_tq);
        }
    }
    return rc;
}

/* ================================================================
 * Application events.
 * ================================================================
 */
/*!
 * \brief Initiated by the application.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 *
 * \return 0 if granted, error code otherwise.
 */
int NutTcpStatePassiveOpenEvent(TCPSOCKET * sock)
{
    if (sock->so_state != TCPS_CLOSED)
        return (sock->so_last_error = EISCONN);

    NutTcpStateChange(sock, TCPS_LISTEN);

    /*
     * Block application.
     */
    NutEventWait(&sock->so_pc_tq, 0);

    return 0;
}

/*!
 * \brief Initiated by the application.
 *
 * The caller must make sure, that the socket is in closed state.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 *
 * \return 0 if granted, -1 otherwise.
 */
int NutTcpStateActiveOpenEvent(TCPSOCKET * sock)
{
    /*
     * Switch state to SYN_SENT. This will
     * transmit a SYN packet.
     */
    NutTcpStateChange(sock, TCPS_SYN_SENT);

    /*
     * Block application.
     */
	 if(sock->so_state == TCPS_SYN_SENT)
		NutEventWait(&sock->so_ac_tq, 0);

    if (sock->so_state != TCPS_ESTABLISHED)
        return -1;

    return 0;
}

/*!
 * \brief Socket close by application.
 *
 * If socket is in state SYN_RECEIVED or ESTABLISHED,
 * it is changed to FINWAIT1.
 *
 * No further data sending is accepted.
 * Receiving is still allowed.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 */
int NutTcpStateCloseEvent(TCPSOCKET * sock)
{
    if (sock == 0)
        return -1;

    NutThreadYield();

    switch (sock->so_state) {
    case TCPS_LISTEN:
    case TCPS_SYN_SENT:
    case TCPS_CLOSED:
        /*
         * No connection yet, immediately destroy the socket.
         */
        NutTcpDestroySocket(sock);
        break;

    case TCPS_SYN_RECEIVED:
    case TCPS_ESTABLISHED:
        /*
         * Send a FIN and wait for ACK or FIN.
         */
        //@@@printf ("[%04X]ESTABLISHED: going to FIN_WAIT_1\n", (u_short) sock);
        NutTcpStateChange(sock, TCPS_FIN_WAIT_1);
        break;

    case TCPS_CLOSE_WAIT:
        /* 
         * RFC 793 is wrong. 
         */
        //@@@printf("[%04X]CLOSE_WAIT: going to LAST_ACK\n", (u_short) sock);
        NutTcpStateChange(sock, TCPS_LAST_ACK);
        break;

    case TCPS_FIN_WAIT_1:
    case TCPS_FIN_WAIT_2:
    case TCPS_CLOSING:
    case TCPS_LAST_ACK:
    case TCPS_TIME_WAIT:
        sock->so_last_error = EALREADY;
        return -1;

    default:
        sock->so_last_error = ENOTCONN;
        return -1;
    }
    return 0;
}

/*!
 * \brief Initiated by the application.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 */
int NutTcpStateWindowEvent(TCPSOCKET * sock)
{
    if (sock == 0)
        return -1;
    sock->so_tx_flags |= SO_ACK | SO_FORCE;
    NutTcpOutput(sock, 0, 0);

    return 0;
}

/* ================================================================
 * Timeout events.
 * ================================================================
 */
/*!
 * \brief Retransmit a segment after ACK timeout.
 *
 * This function is called by the TCP timer.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 * \returns Nonzero if socket was aborted due to reach of retransmit 
 *          limit or network error.
 *
 */
int NutTcpStateRetranTimeout(TCPSOCKET * sock)
{
    NETBUF *so_tx_next;
    if (sock->so_retransmits++ > 7)
    {
        /* Abort the socket */
        NutTcpAbortSocket(sock, ETIMEDOUT);
        return -1;
    } else {
#ifdef NUTDEBUG
        if (__tcp_trf)
            NutDumpTcpHeader(__tcp_trs, "RET", sock, sock->so_tx_nbq);
#endif
        /* We must save sock->so_tx_nbq->nb_next before calling NutIpOutput,
         * because in case of error the NETBUF is release by NutIpOutput and
         * not longer available.
         */
        so_tx_next = sock->so_tx_nbq->nb_next;
        if (NutIpOutput(IPPROTO_TCP, sock->so_remote_addr, sock->so_tx_nbq)) {
            /* Adjust packet queue */
            sock->so_tx_nbq = so_tx_next;
            /* Abort the socket */
            NutTcpAbortSocket(sock, ENETDOWN);
            return -1;
        } else {
            /* Restart the retransmission timer. */
            sock->so_retran_time = (u_short) NutGetMillis();
            return 0;
        }
    }
}

/* ================================================================
 * Segment arrival events.
 * ================================================================
 */

/*!
 * \brief
 * Process incoming segments in listening state.
 *
 * Wait for a connection request from a remote socket.
 *
 * \param sock Socket descriptor.
 * \param nb   Network buffer structure containing a TCP segment.
 */
static void NutTcpStateListen(TCPSOCKET * sock, u_char flags, TCPHDR * th, NETBUF * nb)
{
    /*
     * Got a SYN segment. Store relevant data in our socket
     * structure and switch to TCPS_SYN_RECEIVED.
     */
    if ((flags & (TH_SYN | TH_ACK | TH_RST)) == TH_SYN) {
        NutTcpProcessSyn(sock, nb->nb_nw.vp, th);
        NutTcpStateChange(sock, TCPS_SYN_RECEIVED);
        NutNetBufFree(nb);
    } else
        NutTcpReject(nb);
}


/*!
 * \brief Process incoming segments in SYN-SENT state.
 *
 * Wait for a matching connection request after having sent ours.
 *
 * \param sock Socket descriptor.
 * \param nb   Network buffer structure containing a TCP segment.
 */
static void NutTcpStateSynSent(TCPSOCKET * sock, u_char flags, TCPHDR * th, NETBUF * nb)
{
    /*
     * Validate ACK, if set.
     */
    if (flags & TH_ACK) {
        if (!IsInLimits(ntohl(th->th_ack), sock->so_tx_isn + 1, sock->so_tx_nxt)) {
            NutTcpReject(nb);
            return;
        }
    }

    /*
     * Handle RST flag. If we were in the LISTEN state,
     * then we return to the LISTEN state, otherwise we
     * abort the connection and go to the CLOSED state.
     */
    if (flags & TH_RST) {
        if (flags & TH_ACK) {
            /*if (sock->so_pc_tq)
                NutTcpStateChange(sock, TCPS_LISTEN);
            else */
                NutTcpAbortSocket(sock, ECONNREFUSED);
        }
        NutNetBufFree(nb);
        return;
    }

    /*
     * Handle SYN flag. If we got a valid ACK too, then
     * our connection is established. Otherwise enter
     * SYNRCVD state.
     */
    if (flags & TH_SYN) {
        NutTcpProcessSyn(sock, nb->nb_nw.vp, th);
        if (flags & TH_ACK) {
            NutTcpProcessAck(sock, th, nb->nb_ap.sz);
            NutTcpStateChange(sock, TCPS_ESTABLISHED);
            /* Wake up the actively connecting thread. */
            NutEventPost(&sock->so_ac_tq);
        } else {
            NutTcpStateChange(sock, TCPS_SYN_RECEIVED);
        }
    }
    NutNetBufFree(nb);
}

/*
 * \brief
 * Process incoming segments in SYN-RECEIVED state.
 *
 * Waiting for a confirming connection request
 * acknowledgment after having both received
 * and sent a connection request.
 *
 * \param sock Socket descriptor.
 * \param nb   Network buffer structure containing a TCP segment.
 */
static void NutTcpStateSynReceived(TCPSOCKET * sock, u_char flags, TCPHDR * th, NETBUF * nb)
{
    /*
     * If our previous ack receives a reset response,
     * then we fall back to the listening state.
     */
    if (flags & TH_RST) {
        if (sock->so_pc_tq)
            NutTcpStateChange(sock, TCPS_LISTEN);
        else 
            NutTcpAbortSocket(sock, ECONNREFUSED);
        NutNetBufFree(nb);
    	sock->so_retran_time = 0;
        NutTcpDiscardBuffers(sock);
        return;
    }

    /*
     * Reject SYNs.
     */
    if (flags & TH_SYN) {
        NutTcpReject(nb);
        return;
    }

    /*
     * Silently discard segments without ACK.
     */
    if ((flags & TH_ACK) == 0) {
        NutNetBufFree(nb);
        return;
    }

    /*
     * Reject out of window sequence.
     */
    if (!IsInLimits(ntohl(th->th_ack), sock->so_tx_una + 1, sock->so_tx_nxt)) {
        NutTcpReject(nb);
        return;
    }

    /* Acknowledge processing. */
    NutTcpProcessAck(sock, th, nb->nb_ap.sz);

    /*
     * Even SYN segments may contain application data, which will be stored
     * in the socket's input buffer. However, there is no need to post an
     * event to any thread waiting for data, because our connection is not 
     * yet established.
     */
    if (nb->nb_ap.sz)
        NutTcpProcessAppData(sock, nb);
    else
        NutNetBufFree(nb);

    /*
     * Process state change.
     */
    if (flags & TH_FIN) {
        sock->so_rx_nxt++;
        NutTcpStateChange(sock, TCPS_CLOSE_WAIT);
    } else {
        NutTcpStateChange(sock, TCPS_ESTABLISHED);
        NutEventPost(&sock->so_pc_tq);
        NutEventPost(&sock->so_ac_tq);
    }
}

/*
 * \brief Process incoming segments from established connections.
 *
 * Received application data will be delivered to the application
 * until we receive a FIN segment.
 *
 * \param sock  Socket descriptor.
 * \param flags TCP flags.
 * \param th    Pointer to the TCP header within the NETBUF.
 * \param nb    Network buffer structure containing a TCP segment.
 *
 * \todo We may remove the unused counter of dropped segments, which
 *       were out of sequence.
 */
static void NutTcpStateEstablished(TCPSOCKET * sock, u_char flags, TCPHDR * th, NETBUF * nb)
{
    if (flags & TH_RST) {
        NutNetBufFree(nb);
        NutTcpAbortSocket(sock, ECONNRESET);
        return;
    }

    /*
     * Reject SYNs. Silently discard late SYNs
     * (Thanks to Mike Cornelius).
     */
    if (flags & TH_SYN) {
        if (htonl(th->th_seq) != sock->so_rx_isn)
            NutTcpReject(nb);
        else
            NutNetBufFree(nb);
        return;
    }

    /*
     * Silently discard segments without ACK.
     */
    if ((flags & TH_ACK) == 0) {
        NutNetBufFree(nb);
        return;
    }

    NutTcpProcessAck(sock, th, nb->nb_ap.sz);

    /*
     * If the sequence number of the incoming segment is larger than 
     * expected, we probably missed one or more previous segments. Let's 
     * add this one to a linked list of segments received in advance and 
     * hope that the missing data will arrive later.
     */
    if (htonl(th->th_seq) > sock->so_rx_nxt) {
        NETBUF *nbq;
        NETBUF **nbqp;
        TCPHDR *thq;
        u_long th_seq;
        u_long thq_seq;

        if (nb->nb_ap.sz) {
            nbq = sock->so_rx_nbq;
            nbqp = &sock->so_rx_nbq;
            while (nbq) {
                thq = (TCPHDR *) (nbq->nb_tp.vp);
                th_seq = htonl(th->th_seq);
                thq_seq = htonl(thq->th_seq);
                if (th_seq < thq_seq) {
                    *nbqp = nb;
                    nb->nb_next = nbq;
                    break;
                }
                if (th_seq == thq_seq) {
                    NutNetBufFree(nb);
                    sock->so_tx_flags |= SO_ACK | SO_FORCE;
                    NutTcpOutput(sock, 0, 0);
                    return;
                }
                nbqp = &nbq->nb_next;
                nbq = nbq->nb_next;
            }
            if (nbq == 0) {
                *nbqp = nb;
                nb->nb_next = 0;
            }
        } else
            NutNetBufFree(nb);

        sock->so_tx_flags |= SO_ACK | SO_FORCE;
        NutTcpOutput(sock, 0, 0);
        return;
    }

    /*
     * Acknowledge any sequence numbers not expected, 
     * even if they do not contain any data. Keepalive
     * packets contain a sequence number one less
     * than the next data expected and they do not 
     * contain any data.
     */
    if (htonl(th->th_seq) != sock->so_rx_nxt) {
        sock->so_tx_flags |= SO_ACK | SO_FORCE;
        /* This seems to be unused. */
        sock->so_oos_drop++;
        NutNetBufFree(nb);
        NutTcpOutput(sock, 0, 0);
    } 

    /*
     * The sequence number is exactly what we expected.
     */
    else if (nb->nb_ap.sz) {
        NutTcpProcessAppData(sock, nb);
        /*
         * Process segments we may have received in advance.
         */
        while ((nb = sock->so_rx_nbq) != 0) {
            th = (TCPHDR *) (nb->nb_tp.vp);
            if (htonl(th->th_seq) > sock->so_rx_nxt)
                break;
            sock->so_rx_nbq = nb->nb_next;
            if (htonl(th->th_seq) == sock->so_rx_nxt) {
                NutTcpProcessAppData(sock, nb);
                flags |= th->th_flags;
            } else
                NutNetBufFree(nb);
        }
        /* Wake up a thread waiting for data. */
        NutEventPost(&sock->so_rx_tq);
    } else {
        NutNetBufFree(nb);
        //sock->so_tx_flags |= SO_ACK | SO_FORCE;
        //NutTcpOutput(sock, 0, 0);
    }
    if (flags & TH_FIN) {
        //@@@printf ("[%04X]ESTABLISHED: going to CLOSE_WAIT\n", (u_short) sock);
        sock->so_rx_nxt++;
        NutTcpStateChange(sock, TCPS_CLOSE_WAIT);
    }
}

/*
 * \brief Process incoming segments in FIN-WAIT1 state.
 *
 * Waiting for a connection termination request
 * from the remote, or an acknowledgment of the
 * connection termination request previously sent.
 *
 * The application already closed the socket.
 *
 * \param sock Socket descriptor.
 * \param nb   Network buffer structure containing a TCP segment.
 *
 * \todo The out of sync case seems to be ignored. Anyway, do we
 *       really need to process application data in this state?
 */
static void NutTcpStateFinWait1(TCPSOCKET * sock, u_char flags, TCPHDR * th, NETBUF * nb)
{
    //@@@printf ("[%04X]FIN_WAIT_1: incomming segment, IP %04X\n", (u_short) sock, ntohs(((IPHDR*)nb->nb_nw.vp)->ip_id));
    if (flags & TH_RST) {
        NutNetBufFree(nb);
        NutTcpDestroySocket(sock);
        return;
    }

    /*
     * Reject SYNs.
     */
    if (flags & TH_SYN) {
        NutTcpReject(nb);
        return;
    }

    /*
     * Silently discard segments without ACK.
     */
    if ((flags & TH_ACK) == 0) {
        NutNetBufFree(nb);
        return;
    }

    //@@@if (flags & TH_FIN) printf ("[%04X]FIN_WAIT_1: received FIN\n", (u_short) sock);
    //@@@printf ("[%04X]FIN_WAIT_1: received ACK: %lu, unack: %lu, next: %lu\n", (u_short) sock, ntohl(th->th_ack), sock->so_tx_una, sock->so_tx_nxt);
    
    //@@@printf ("[%04X]FIN_WAIT_1:  pre processack, nbq: %04X\n", (u_short) sock, (u_short) sock->so_tx_nbq);
    NutTcpProcessAck(sock, th, nb->nb_ap.sz);
    //@@@printf ("[%04X]FIN_WAIT_1: post processack, nbq: %04X\n", (u_short) sock, (u_short) sock->so_tx_nbq);

    /*
     * All segments had been acknowledged, including our FIN.
     */
    if (sock->so_tx_nxt == sock->so_tx_una) {
        //@@@printf ("[%04X]FIN_WAIT_1: going to FIN_WAIT_2\n", (u_short) sock);
        NutTcpStateChange(sock, TCPS_FIN_WAIT_2);
    }

    /*
     * Process application data and release the network buffer.
     * Is this really required?
     */
    if (nb->nb_ap.sz) {
        NutTcpProcessAppData(sock, nb);
        /* Wake up a thread waiting for data. */
        NutEventPost(&sock->so_rx_tq);
    }
    else
        NutNetBufFree(nb);

    if (flags & TH_FIN) {
        sock->so_rx_nxt++;
        /* 
         * Our FIN has been acked.
         */
        sock->so_time_wait = 0;
        //@@@printf ("[%04X]FIN_WAIT_1: going to CLOSING\n", (u_short) sock);
        if (sock->so_state == TCPS_FIN_WAIT_2)
            NutTcpStateChange(sock, TCPS_TIME_WAIT);
        else
            NutTcpStateChange(sock, TCPS_CLOSING);
    }
}

/*
 * \brief Process incoming segments in FIN-WAIT2 state.
 *
 * Waiting for a connection termination request
 * from the remote.
 *
 * The application already closed the socket.
 *
 * \param sock Socket descriptor.
 * \param nb   Network buffer structure containing a TCP segment.
 *
 * \todo There's probably no need to process application data.
 */
static void NutTcpStateFinWait2(TCPSOCKET * sock, u_char flags, TCPHDR * th, NETBUF * nb)
{
    //@@@printf ("[%04X]FIN_WAIT_2: incomming segment, IP %04X\n", (u_short) sock, ntohs(((IPHDR*)nb->nb_nw.vp)->ip_id));
    if (flags & TH_RST) {
        NutNetBufFree(nb);
        NutTcpDestroySocket(sock);
        return;
    }

    /*
     * Reject SYNs.
     */
    if (flags & TH_SYN) {
        NutTcpReject(nb);
        return;
    }

    /*
     * Silently discard segments without ACK.
     */
    if ((flags & TH_ACK) == 0) {
        NutNetBufFree(nb);
        return;
    }

    //@@@printf ("[%04X]FIN_WAIT_2: received ACK: %lu, unack: %lu, next: %lu\n", (u_short) sock, ntohl(th->th_ack), sock->so_tx_una, sock->so_tx_nxt);
    /*
     * Process acknowledge and application data and release the 
     * network buffer.
     */
    NutTcpProcessAck(sock, th, nb->nb_ap.sz);

    //@@@if (sock->so_tx_nbq) printf ("[%04X]FIN_WAIT_2: xmit buffer not empty!", (u_short) sock);
    /* Do we really need this? */
    if (nb->nb_ap.sz) {
        NutTcpProcessAppData(sock, nb);
        /* Wake up a thread waiting for data. */
        NutEventPost(&sock->so_rx_tq);
    }
    else
        NutNetBufFree(nb);

    if (flags & TH_FIN) {
        sock->so_rx_nxt++;
        sock->so_time_wait = 0;
        //@@@printf ("[%04X]FIN_WAIT_2: going to TIME_WAIT\n", (u_short) sock);
        NutTcpStateChange(sock, TCPS_TIME_WAIT);
    }
}

/*
 * \brief
 * Process incoming segments in CLOSE-WAIT state.
 *
 * Waiting for a connection termination request
 * from the local application.
 *
 * \param sock Socket descriptor.
 * \param nb   Network buffer structure containing a TCP segment.
 */
static void NutTcpStateCloseWait(TCPSOCKET * sock, u_char flags, TCPHDR * th, NETBUF * nb)
{
    //@@@printf ("[%04X]CLOSE_WAIT: incomming segment, IP %04X\n", (u_short) sock, ((IPHDR*)nb->nb_nw.vp)->ip_id);
    if (flags & TH_RST) {
        NutNetBufFree(nb);
        NutTcpAbortSocket(sock, ECONNRESET);
        return;
    }

    /*
     * Reject SYNs.
     */
    if (flags & TH_SYN) {
        NutTcpReject(nb);
        return;
    }

    /*
     * Silently discard segments without ACK.
     */
    if ((flags & TH_ACK) == 0) {
        NutNetBufFree(nb);
        return;
    }

    NutTcpProcessAck(sock, th, nb->nb_ap.sz);

    NutNetBufFree(nb);
}

/*
 * \brief
 * Process incoming segments in CLOSING state.
 *
 * Waiting for a connection termination request
 * acknowledgment from the remote.
 *
 * The application already closed the socket.
 *
 * \param sock Socket descriptor.
 * \param nb   Network buffer structure containing a TCP segment.
 */
static void NutTcpStateClosing(TCPSOCKET * sock, u_char flags, TCPHDR * th, NETBUF * nb)
{
    //@@@printf ("[%04X]CLOSING: Incomming segment\n", (u_short) sock);
    if (flags & TH_RST) {
        NutNetBufFree(nb);
        NutTcpDestroySocket(sock);
        return;
    }

    /*
     * Reject SYNs.
     */
    if (flags & TH_SYN) {
        NutTcpReject(nb);
        return;
    }

    /*
     * Silently discard segments without ACK.
     */
    if ((flags & TH_ACK) == 0) {
        NutNetBufFree(nb);
        return;
    }

    NutTcpProcessAck(sock, th, nb->nb_ap.sz);

    /*
     * All segments had been acknowledged, including our FIN.
     */
    if (sock->so_tx_nxt == sock->so_tx_una) {
        sock->so_time_wait = 0;
        NutTcpStateChange(sock, TCPS_TIME_WAIT);
        //@@@printf ("[%04X]CLOSING: Going to TIME_WAIT\n", (u_short) sock);
    }
    //@@@else printf ("[%04X]CLOSING: NOT changing state\n", (u_short) sock);

    NutNetBufFree(nb);
}

/*!
 * \brief
 * Process incoming segment in LAST_ACK state.
 *
 * Waiting for an acknowledgment of the connection termination
 * request previously sent.
 *
 * The application already closed the socket.
 *
 * \param sock  Socket descriptor.
 * \param flags TCP flags of incoming segment.
 * \param th    TCP header of incoming segment.
 * \param nb    Network buffer structure containing a TCP segment.
 */
static void NutTcpStateLastAck(TCPSOCKET * sock, u_char flags, TCPHDR * th, NETBUF * nb)
{
    //@@@printf ("[%04X]LAST_ACK: incomming segment, IP %04X\n", (u_short) sock, ((IPHDR*)nb->nb_nw.vp)->ip_id);
    if (flags & TH_RST) {
        NutNetBufFree(nb);
        NutTcpDestroySocket(sock);
        return;
    }

    /*
     * Reject SYNs.
     */
    if (flags & TH_SYN) {
        NutTcpReject(nb);
        return;
    }

    /*
     * Silently discard segments without ACK.
     */
    if ((flags & TH_ACK) == 0) {
        NutNetBufFree(nb);
        return;
    }

    NutTcpProcessAck(sock, th, nb->nb_ap.sz);
    NutNetBufFree(nb);

    if (sock->so_tx_nxt == sock->so_tx_una) 
        NutTcpDestroySocket(sock);
    //@@@else printf ("[%04X]LAST_ACK: no destroy sock\n", (u_short) sock);
}

/*!
 * \brief Process incoming TCP segments.
 *
 * Processing is based on the current state of the socket connection.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 * \param nb   Network buffer structure containing a TCP segment.
 *             Will be released within this routine.
 */
static void NutTcpStateProcess(TCPSOCKET * sock, NETBUF * nb)
{
    u_long tx_win;
    u_long tx_una;
    TCPHDR *th = (TCPHDR *) nb->nb_tp.vp;
    u_char flags = th->th_flags;

#ifdef NUTDEBUG
    if (__tcp_trf) {
        fprintf(__tcp_trs, " %04x-", (u_int) sock);
        NutDumpSockState(__tcp_trs, sock->so_state, "[", ">]");
    }
#endif
    switch (sock->so_state) {
        /* Handle the most common case first. */
    case TCPS_ESTABLISHED:
        tx_win = sock->so_tx_win;
        tx_una = sock->so_tx_una;
        NutTcpStateEstablished(sock, flags, th, nb);
        /* Wake up all threads waiting for transmit, if something interesting happened. */
        if(sock->so_state != TCPS_ESTABLISHED || /* Status changed. */
           sock->so_tx_win > tx_win ||           /* Windows changed. */
           sock->so_tx_una > tx_una) {           /* Unacknowledged data changed. */
            NutEventBroadcast(&sock->so_tx_tq);
        }
        break;
    case TCPS_LISTEN:
        NutTcpStateListen(sock, flags, th, nb);
        break;
    case TCPS_SYN_SENT:
        NutTcpStateSynSent(sock, flags, th, nb);
        break;
    case TCPS_SYN_RECEIVED:
        NutTcpStateSynReceived(sock, flags, th, nb);
        break;
    case TCPS_FIN_WAIT_1:
        NutTcpStateFinWait1(sock, flags, th, nb);
        break;
    case TCPS_FIN_WAIT_2:
        NutTcpStateFinWait2(sock, flags, th, nb);
        break;
    case TCPS_CLOSE_WAIT:
        NutTcpStateCloseWait(sock, flags, th, nb);
        break;
    case TCPS_CLOSING:
        NutTcpStateClosing(sock, flags, th, nb);
        break;
    case TCPS_LAST_ACK:
        NutTcpStateLastAck(sock, flags, th, nb);
        break;
    case TCPS_TIME_WAIT:
        /*
         * Ignore everything while in TIME_WAIT state.
         */
        NutNetBufFree(nb);
        break;
    case TCPS_CLOSED:
        /*
         * Reject everything while in CLOSED state.
         */
        NutTcpReject(nb);
        break;
    default:
        NutNetBufFree(nb);
        break;
    }
}

/*! \fn NutTcpSm(void *arg)
 * \brief TCP state machine thread.
 *
 * The TCP state machine serves two purposes: It processes incoming TCP
 * segments and handles TCP timers.
 */
THREAD(NutTcpSm, arg)
{
    NETBUF *nb;
    NETBUF *nbx;
    TCPHDR *th;
    IPHDR *ih;
    TCPSOCKET *sock;
    u_char tac = 0;

    /*
     * It won't help giving us a higher priority than the application
     * code. We depend on the speed of the reading application.
     */
    NutThreadSetPriority (32);
    
    for (;;) {
        if (++tac > 3 || NutEventWait(&tcp_in_rdy, 200)) {
            tac = 0;
            for (sock = tcpSocketList; sock; sock = sock->so_next) {

                /*
                 * Send late acks.
                 */
                if (sock->so_tx_flags & SO_ACK) {
                    sock->so_tx_flags |= SO_FORCE;
                    NutTcpOutput(sock, 0, 0);
                }

                /*
                 * Process retransmit timer.
                 */
                if (sock->so_tx_nbq && sock->so_retran_time) {
                    if ((u_short)((u_short)NutGetMillis() - sock->so_retran_time) > sock->so_rtto) {
                        NutTcpStateRetranTimeout(sock);
                    }
                }

                /*
                 * Destroy sockets after timeout in TIMEWAIT state.
                 */
                if (sock->so_state == TCPS_TIME_WAIT || sock->so_state == TCPS_FIN_WAIT_2) {
                    if (sock->so_time_wait++ >= 9) {
                        NutTcpDestroySocket(sock);
                        break;
                    }
                }

                /*
                 * Recover from SYN flood attacks.
                 */
                else if (sock->so_state == TCPS_SYN_RECEIVED) {
                    if (sock->so_time_wait++ >= 45) {
                        sock->so_state = TCPS_LISTEN;
                        sock->so_time_wait = 0;
                    }
                }
            }
        } else {
            nb = tcp_in_nbq;
            tcp_in_nbq = 0;
            tcp_in_cnt = 0;
            while (nb) {
                ih = (IPHDR *) nb->nb_nw.vp;
                th = (TCPHDR *) nb->nb_tp.vp;
                sock = NutTcpFindSocket(th->th_dport, th->th_sport, ih->ip_src);
#ifdef NUTDEBUG
                if (__tcp_trf)
                    NutDumpTcpHeader(__tcp_trs, " IN", sock, nb);
#endif
                nbx = nb->nb_next;
                if (sock) {
                    NutTcpInputOptions(sock, nb);
                    NutTcpStateProcess(sock, nb);
                }

                /*
                 * Reject the segment, if no matching socket was found.
                 */
                else
                    NutTcpReject(nb);
                nb = nbx;
            }
        }
    }
}

/*!
 * \brief Process incoming TCP segments.
 *
 * All incoming TCP packets are passed to this routine. They will
 * be added to a global queue and processed by the TCP state
 * machine, which is running on a separate thread.
 *
 * \note This routine is called by the IP layer on incoming 
 *       TCP segments. Applications typically do not call 
 *       this function.
 */
void NutTcpStateMachine(NETBUF * nb)
{
    NETBUF *nbp;
    u_short size;

    nb->nb_next = 0;

    /*
     * Incoming TCP segments are rejected and released if no TCP
     * sockets have been opened. Not doing so would add them
     * to the queue and never release the NETBUF. Thanks to
     * Ralph Mason for this fix.
     */
    if (tcpThread == 0) {
        NutTcpReject(nb);
        return;
    }

    if ((nbp = tcp_in_nbq) == 0) {
        tcp_in_nbq = nb;
        NutEventPost(&tcp_in_rdy);
    } else {
        size = nb->nb_nw.sz + nb->nb_tp.sz + nb->nb_ap.sz;
        if (tcp_in_cnt + size + 2048 < NutHeapAvailable()) {
            tcp_in_cnt += size;
            while (nbp->nb_next)
                nbp = nbp->nb_next;
            nbp->nb_next = nb;
            NutEventPost(&tcp_in_rdy);
        } else
            NutNetBufFree(nb);
    }
}

/*!
 * \brief Start TCP state machine.
 *
 * The socket interface will automatically call this routine as
 * soon as the first socket is created.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutTcpInitStateMachine(void)
{
    if (tcpThread == 0 && (tcpThread = NutThreadCreate("tcpsm", NutTcpSm, NULL, NUT_THREAD_TCPSMSTACK)) == 0)
        return -1;
    return 0;
}

/*!
 * \brief Closes socket with error.
 *
 * Aborts any socket activity and sets last error.
 *
 * \param sock  Socket descriptor.
 * \param last_error Error number to report
 *
 * \note This routine is called internally by Nut/Net.
 *       Applications typically do not call this function.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutTcpAbortSocket(TCPSOCKET * sock, u_short last_error)
{
    sock->so_last_error = last_error;
    sock->so_retran_time = 0;
    sock->so_time_wait = 0;
    /*
     * If NutTcpCloseSocket was already called, we have to change
     * to TCPS_TIME_WAIT state, otherwise the socket will not be destroyed.
     * For the other cases just go to TCPS_CLOSED.
     */
    if (sock->so_state >= TCPS_FIN_WAIT_1)      /* FIN_WAIT_1, FIN_WAIT_2, CLOSING, LAST_ACK, TIME_WAIT */
        sock->so_state = TCPS_TIME_WAIT;
    else
        sock->so_state = TCPS_CLOSED;
    NutTcpDiscardBuffers(sock);
    NutEventBroadcast(&sock->so_rx_tq);
    NutEventBroadcast(&sock->so_tx_tq);
    NutEventBroadcast(&sock->so_pc_tq);
    NutEventBroadcast(&sock->so_ac_tq);
    return 0;
}

/*@}*/
