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
 * Revision 1.5  2004/01/14 19:34:08  drsung
 * New TCP output buffer handling
 *
 * Revision 1.4  2003/11/04 17:58:18  haraldkipp
 * Removed volatile attibute from socket list
 *
 * Revision 1.3  2003/08/14 15:08:36  haraldkipp
 * Bugfix, TCP may reuse socket ports because of wrong byte order during compare.
 *
 * Revision 1.2  2003/07/13 19:23:59  haraldkipp
 * TCP transfer speed increased by changing the character receive buffer
 * in TCPSOCKET to a NETBUF queue.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:44  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.18  2003/02/04 18:14:57  harald
 * Version 3 released
 *
 * Revision 1.17  2002/09/15 17:07:05  harald
 * Allow application to read buffered data during close wait
 *
 * Revision 1.16  2002/09/03 17:43:15  harald
 * Configurable receive buffer size
 *
 * Revision 1.15  2002/08/16 17:54:57  harald
 * Count out of sequence drops
 *
 * Revision 1.14  2002/08/11 12:26:21  harald
 * ICC mods
 *
 * Revision 1.13  2002/06/26 17:29:36  harald
 * First pre-release with 2.4 stack
 *
 */

#include <string.h>

#include <sys/atom.h>
#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <net/errno.h>
#include <net/route.h>
#include <netinet/ip.h>
#include <netinet/icmp.h>
#include <netinet/ip_icmp.h>
#include <netinet/ipcsum.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#ifdef NUTDEBUG
#include <net/netdebug.h>
#endif

/*!
 * \addtogroup xgTcpSocket
 */
/*@{*/


#define TICK_RATE   1

TCPSOCKET *tcpSocketList = 0;   /*!< Linked list of all TCP sockets. */

static volatile u_short last_local_port = 4096; /* Unassigned local port. */

static u_char tcpStateRunning = 0;


/*!
 * \brief Destroy a previously allocated socket.
 *
 * Remove socket from the socket list and release occupied memory.
 *
 * Applications typically do not need to call this function. It
 * is automatically called by a timer after the socket has been
 * closed by NutTcpCloseSocket().
 *
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 */
void NutTcpDestroySocket(TCPSOCKET * sock)
{
    TCPSOCKET *sp;
    TCPSOCKET *volatile *spp;
    NETBUF *nb;

    /*
     * Remove socket from the list.
     */
    sp = tcpSocketList;
    spp = &tcpSocketList;
    while (sp) {
        if (sp == sock) {
            *spp = sp->so_next;
            break;
        }
        spp = &sp->so_next;
        sp = sp->so_next;
    }

    /*
     * Free all memory occupied by the socket.
     */
    if (sp) {
        while ((nb = sock->so_rx_buf) != 0) {
            sock->so_rx_buf = nb->nb_next;
            NutNetBufFree(nb);
        }
        while ((nb = sock->so_tx_nbq) != 0) {
            sock->so_tx_nbq = nb->nb_next;
            NutNetBufFree(nb);
        }
        while ((nb = sock->so_rx_nbq) != 0) {
            sock->so_rx_nbq = nb->nb_next;
            NutNetBufFree(nb);
        }
        memset(sock, 0, sizeof(TCPSOCKET));
        NutHeapFree(sock);
    }
}

/*!
 * \brief Find a matching socket.
 *
 * Loop through all sockets and find a matching connection (prefered) 
 * or a listening socket.
 *
 * Applications typically do not need to call this function.
 *
 * \param lport Local port number.
 * \param rport Remote port number.
 * \param raddr Remote IP address in network byte order.
 *
 * \return Socket descriptor.
 */
TCPSOCKET *NutTcpFindSocket(u_short lport, u_short rport, u_long raddr)
{
    TCPSOCKET *sp;
    TCPSOCKET *sock = 0;

    /*
     * Try to find an exact match for the remote
     * address and port first.
     */
    for (sp = tcpSocketList; sp; sp = sp->so_next) {
        if (sp->so_local_port == lport) {
            if (sp->so_remote_addr == raddr && sp->so_remote_port == rport && sp->so_state != TCPS_CLOSED) {
                sock = sp;
                break;
            }
        }
    }

    /*
     * If no exact match exists, try a listening socket.
     * This part had been totally wrong, because it
     * didn't check the local port number and accepted
     * incoming requests on any port. Thanks to
     * Alejandro Lopez, who pointed this out.
     */
    if (sock == 0)
        for (sp = tcpSocketList; sp; sp = sp->so_next)
            if (sp->so_state == TCPS_LISTEN && sp->so_local_port == lport)
                sock = sp;

    return sock;
}


/*!
 * \brief Create a TCP socket.
 *
 * Allocates a TCPSOCKET structure from heap memory, initializes 
 * it and returns a pointer to that structure.
 *
 * The first call will also start the TCP timer, which is
 * required by various timeout checks.
 *
 * \return Socket descriptor of the newly created TCP socket or
 *         0 if there is not enough memory left.
 *
 * \todo Avoid fixed initial sequence number.
 * \todo Configurable buffer size.
 * \todo Allow larger maximum segment size.
 */
TCPSOCKET *NutTcpCreateSocket(void)
{
    TCPSOCKET *sock = 0;

    if (tcpStateRunning || (tcpStateRunning = (NutTcpInitStateMachine() == 0))) {

        if ((sock = NutHeapAllocClear(sizeof(TCPSOCKET))) != 0) {
            sock->so_state = TCPS_CLOSED;

            /*
             * Initialize the virtual device interface.
             */
            sock->so_devtype = IFTYP_TCPSOCK;
            sock->so_devread = NutTcpDeviceRead;
            sock->so_devwrite = NutTcpDeviceWrite;
            sock->so_devwrite_P = NutTcpDeviceWrite_P;
            sock->so_devioctl = NutTcpDeviceIOCtl;

            sock->so_tx_isn = 1000000;  /* 0x00a8393a; */
            sock->so_tx_una = sock->so_tx_isn;
            sock->so_tx_nxt = sock->so_tx_isn;
            sock->so_rx_bsz = sock->so_rx_win = TCP_WINSIZE;

            sock->so_mss = TCP_MSS;
            sock->so_rto_next = 3 * TICK_RATE;
            sock->so_srtt_var = 3 * TICK_RATE;

            sock->so_next = tcpSocketList;

            sock->so_devobsz = TCP_MSS; /* Default output buffer size is TCP_MSS bytes */

            tcpSocketList = sock;
        }
    }
    return sock;
}

/*!
 * \brief Set value of a TCP socket option.
 *
 * The following values can be set:
 *
 * - TCP_MAXSEG Maximum segment size (#u_short).
 * - SO_SNDTIMEO Socket send timeout (#u_long).
 * - SO_RCVTIMEO Socket receive timeout (#u_long).
 * - SO_SNDBUF   Socket output buffer size (#u_short).
 *
 * \param sock    Socket descriptor. This pointer must have been 
 *                retrieved by calling NutTcpCreateSocket().
 * \param optname Option to set.
 * \param optval  Pointer to the value.
 * \param optlen  Length of the value.
 * \return 0 on success, -1 otherwise. The specific error code
 *         can be retrieved by calling NutTcpError().
 */
int NutTcpSetSockOpt(TCPSOCKET * sock, int optname, CONST void *optval, int optlen)
{
    int rc = -1;

    if (sock == 0)
        return -1;
    switch (optname) {

    case TCP_MAXSEG:
        if (optval == 0 || optlen != sizeof(u_short))
            sock->so_last_error = EINVAL;
        else {
            sock->so_mss = *((u_short *) optval);
            rc = 0;
        }
        break;

    case SO_RCVBUF:
        if (optval == 0 || optlen != sizeof(u_short))
            sock->so_last_error = EINVAL;
        else {
            sock->so_rx_bsz = *((u_short *) optval);
            sock->so_rx_win = sock->so_rx_bsz;
            rc = 0;
        }
        break;

    case SO_SNDTIMEO:
        if (optval == 0 || optlen != sizeof(u_long))
            sock->so_last_error = EINVAL;
        else {
            sock->so_write_to = *((u_long *) optval);
            rc = 0;
        }
        break;

    case SO_RCVTIMEO:
        if (optval == 0 || optlen != sizeof(u_long))
            sock->so_last_error = EINVAL;
        else {
            sock->so_read_to = *((u_long *) optval);
            rc = 0;
        }
        break;

    case SO_SNDBUF:
        if (optval == 0 || optlen != sizeof(u_short))
            sock->so_last_error = EINVAL;
        else {
            NutTcpDeviceWrite(sock, 0, 0);
            sock->so_devobsz = *((u_short *) optval);
            rc = 0;
        }
        break;

    default:
        sock->so_last_error = ENOPROTOOPT;
        break;
    }
    return rc;
}

/*!
 * \brief Get a TCP socket option value.
 *
 * The following values can be set:
 *
 * - TCP_MAXSEG Maximum segment size (#u_short).
 * - SO_SNDTIMEO Socket send timeout (#u_long).
 * - SO_RCVTIMEO Socket receive timeout (#u_long).
 * - SO_SNDBUF   Socket output buffer size (#u_short).
 *
 * \param sock    Socket descriptor. This pointer must have been 
 *                retrieved by calling NutTcpCreateSocket().
 * \param optname Option to get.
 * \param optval  Points to a buffer receiving the value.
 * \param optlen  Length of the value buffer.
 *
 * \return 0 on success, -1 otherwise. The specific error code
 *         can be retrieved by calling NutTcpError().
 */
int NutTcpGetSockOpt(TCPSOCKET * sock, int optname, void *optval, int optlen)
{
    int rc = -1;

    if (sock == 0)
        return -1;
    switch (optname) {

    case TCP_MAXSEG:
        if (optval == 0 || optlen != sizeof(u_short))
            sock->so_last_error = EINVAL;
        else {
            *((u_short *) optval) = sock->so_mss;
            rc = 0;
        }
        break;

    case SO_RCVBUF:
        if (optval == 0 || optlen != sizeof(u_short))
            sock->so_last_error = EINVAL;
        else {
            *((u_short *) optval) = sock->so_rx_bsz;
            rc = 0;
        }
        break;

    case SO_SNDTIMEO:
        if (optval == 0 || optlen != sizeof(u_long))
            sock->so_last_error = EINVAL;
        else {
            *((u_long *) optval) = sock->so_write_to;
            rc = 0;
        }
        break;

    case SO_RCVTIMEO:
        if (optval == 0 || optlen != sizeof(u_long))
            sock->so_last_error = EINVAL;
        else {
            *((u_long *) optval) = sock->so_read_to;
            rc = 0;
        }
        break;

    case SO_SNDBUF:
        if (optval == 0 || optlen != sizeof(u_short))
            sock->so_last_error = EINVAL;
        else {
            *((u_short *) optval) = sock->so_devobsz;
            rc = 0;
        }
        break;
    default:
        sock->so_last_error = ENOPROTOOPT;
        break;
    }
    return rc;
}

/*!
 * \brief Connect to a remote socket.
 *
 * This function tries to establish a connection to the specified
 * remote port of the specified remote server. The calling thread
 * will be suspended until a connection is successfully established 
 * or an error occurs.
 *
 * This function is typically used by TCP client applications.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 * \param addr IP address of the host to connect (network byte order).
 * \param port Port number to connect (host byte order).
 *
 * \return 0 on success, -1 otherwise. The specific error code
 *         can be retrieved by calling NutTcpError().
 */
int NutTcpConnect(TCPSOCKET * sock, u_long addr, u_short port)
{
    TCPSOCKET *sp;
    NUTDEVICE *dev;

    if (sock == 0)
        return -1;
    /*
     * Despite RFC793 we do not allow a passive
     * open to become active.
     */
    if (sock->so_state == TCPS_LISTEN) {
        sock->so_last_error = EOPNOTSUPP;
        return -1;
    } else if (sock->so_state != TCPS_CLOSED) {
        sock->so_last_error = EISCONN;
        return -1;
    }

    /*
     * Find an unused local port.
     */
    do {
        if (++last_local_port == 0)
            last_local_port = 4096;

        sp = tcpSocketList;
        while (sp) {
            /* Thanks to Ralph Mason for fixing the byte order bug. */
            if (sp->so_local_port == htons(last_local_port))
                break;
            sp = sp->so_next;
        }
    } while (sp);

    /*
     * OK - we've got a new port. Now fill
     * remaining parts of the socket structure.
     */
    sock->so_local_port = htons(last_local_port);
    sock->so_remote_port = htons(port);
    sock->so_remote_addr = addr;

    /*
     * Get local address for this destination.
     */
    if ((dev = NutIpRouteQuery(addr, 0)) != 0) {
        IFNET *nif = dev->dev_icb;
        sock->so_local_addr = nif->if_local_ip;
    } else {
        sock->so_last_error = EHOSTUNREACH;
        return -1;
    }

    /*
     * Trigger active open event for the state machine.
     * This will suspend the thread until synchronized.
     */
    return NutTcpStateActiveOpenEvent(sock);
}

/*!
 * \brief Wait for incoming connect from a remote socket.
 *
 * The calling thread will be suspended until until an
 * incoming connection request is received.
 *
 * This function is typically used by TCP server applications.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 * \param port Port number to listen to (host byte order).
 *
 * \return 0 on success, -1 otherwise. The specific error code
 *         can be retrieved by calling NutTcpError().
 */
int NutTcpAccept(TCPSOCKET * sock, u_short port)
{
    sock->so_local_port = htons(port);

    return NutTcpStatePassiveOpenEvent(sock);
}

/*!
 * \brief Send data on a connected TCP socket.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket(). In
 *             addition a connection must have been established
 *             by calling NutTcpConnect or NutTcpAccept.
 * \param data Pointer to a buffer containing the data to send.
 * \param len  Number of bytes to be sent.
 *
 * \return If successful, the number of bytes added to the
 *         socket transmit buffer. This may be less than
 *         the specified number of bytes to send. The return
 *         value -1 indicates a fatal error.
 */
int NutTcpSend(TCPSOCKET * sock, CONST void *data, u_short len)
{
    u_short unacked;

    /*
     * Check parameters.
     */
    if (sock == 0)
        return -1;
    if (sock->so_state != TCPS_ESTABLISHED) {
        sock->so_last_error = ENOTCONN;
        return -1;
    }
    if (data == 0 || len == 0)
        return 0;

    /*
     * Limit the transmission size to our maximum segment size.
     */
    if (len > sock->so_mss)
        len = sock->so_mss;

    for (;;) {
        /*
         * Limit the size of unacknowledged data to four full segments.
         */
        unacked = sock->so_tx_nxt - sock->so_tx_una;
        if ((unacked >> 2) > sock->so_mss) {
            if (NutEventWait(&sock->so_tx_tq, sock->so_write_to)) {
                return 0;
            }
            if (sock->so_state != TCPS_ESTABLISHED) {
                sock->so_last_error = ENOTCONN;
                return -1;
            }
            continue;
        }
        /*
         * Wait for peer's window open wide enough to take all our 
         * data. This also avoids silly window syndrome on our side.
         */
        if (len > sock->so_tx_win - unacked) {
            if (NutEventWait(&sock->so_tx_tq, sock->so_write_to)) {
                return 0;
            }
            if (sock->so_state != TCPS_ESTABLISHED) {
                sock->so_last_error = ENOTCONN;
                return -1;
            }
            continue;
        }
        break;
    }
    /*
     * The segment will be automatically retransmitted if not 
     * acknowledged in time. If this returns an error, it's a 
     * fatal one.
     */
    sock->so_tx_flags |= SO_ACK;
    if (NutTcpOutput(sock, data, len))
        return -1;
    return len;
}

/*!
 * \brief Receive data on a connected TCP socket.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket(). In
 *             addition a connection must have been established
 *             by calling NutTcpConnect or NutTcpAccept.
 * \param data Pointer to the buffer that receives the data.
 * \param size Size of the buffer.
 *
 * \return If successful, the number of received data bytes
 *         is returned. This may be less than the specified
 *         size of the buffer. The return value 0 indicates
 *         a timeout, while -1 is returned in case of an error 
 *         or broken connection. Call NutTcpError() to determine
 *         the specific error code.
 */
int NutTcpReceive(TCPSOCKET * sock, void *data, u_short size)
{
    u_short i;

    /*
     * Check parameters.
     */
    if (sock == 0)
        return -1;
    if (sock->so_state != TCPS_ESTABLISHED && sock->so_state != TCPS_CLOSE_WAIT) {
        sock->so_last_error = ENOTCONN;
        return -1;
    }
    if (data == 0 || size == 0)
        return 0;

    /*
     * Wait until any data arrived, a timeout occurs
     * or the connection terminates.
     */
    while (sock->so_rx_cnt - sock->so_rd_cnt == 0) {
        if (sock->so_state != TCPS_ESTABLISHED) {
            sock->so_last_error = ENOTCONN;
            return -1;
        }
        if (NutEventWait(&sock->so_rx_tq, sock->so_read_to))
            return 0;
    }

    if (size > sock->so_rx_cnt - sock->so_rd_cnt)
        size = sock->so_rx_cnt - sock->so_rd_cnt;
    if (size) {
        NETBUF *nb;
        u_short rd_cnt;         /* Bytes read from NETBUF. */
        u_short nb_cnt;         /* Bytes left in NETBUF. */
        u_short ab_cnt;         /* Total bytes in app buffer. */
        u_short mv_cnt;         /* Bytes to move to app buffer. */

        rd_cnt = sock->so_rd_cnt;

        ab_cnt = 0;
        while (ab_cnt < size) {
            nb = sock->so_rx_buf;
            nb_cnt = nb->nb_ap.sz - rd_cnt;
            mv_cnt = size - ab_cnt;
            if (mv_cnt > nb_cnt)
                mv_cnt = nb_cnt;
            memcpy((char *) data + ab_cnt, (char *) (nb->nb_ap.vp) + rd_cnt, mv_cnt);
            ab_cnt += mv_cnt;
            rd_cnt += mv_cnt;
            if (mv_cnt >= nb_cnt) {
                sock->so_rx_buf = nb->nb_next;
                sock->so_rx_cnt -= rd_cnt;
                NutNetBufFree(nb);
                nb = sock->so_rx_buf;
                rd_cnt = 0;
            }
        }
        sock->so_rd_cnt = rd_cnt;

        /*
         * Update our receive window.
         */
        if (sock->so_state == TCPS_ESTABLISHED) {
            i = sock->so_rx_win;
            if ((i += size) > sock->so_rx_bsz)
                i = sock->so_rx_bsz;

            if (sock->so_rx_win <= sock->so_mss && i > sock->so_mss) {
                sock->so_rx_win = i;
                NutTcpStateWindowEvent(sock);
            } else {
                sock->so_rx_win = i;
            }
        }
    }
    return size;
}

/*!
 * \brief Close TCP socket.
 *
 * Note, that the socket may not be immediately destroyed
 * after calling this function. However, the application 
 * must not use the socket after this call.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 *
 * \return 0 on success, -1 otherwise.
 */
int NutTcpCloseSocket(TCPSOCKET * sock)
{
    /* Flush buffer first */
    NutTcpDeviceWrite(sock, 0, 0);
    return NutTcpStateCloseEvent(sock);
}

/*!
 * \brief Return specific code of the last error.
 *
 * Possible error codes (net/errno.h) are:
 *
 * - EWOULDBLOCK: Operation would block
 * - EINPROGRESS: Operation now in progress
 * - EALREADY: Operation already in progress
 * - ENOTSOCK: Socket operation on non-socket
 * - EDESTADDRREQ: Destination address required
 * - EMSGSIZE: Message too long
 * - EPROTOTYPE: Protocol wrong type for socket
 * - ENOPROTOOPT: Protocol not available
 * - EPROTONOSUPPORT: Protocol not supported
 * - ESOCKTNOSUPPORT: Socket type not supported
 * - EOPNOTSUPP: Operation not supported on socket
 * - EPFNOSUPPORT: Protocol family not supported
 * - EAFNOSUPPORT: Address family not supported by protocol family
 * - EADDRINUSE: Address already in use
 * - EADDRNOTAVAIL: Can't assign requested address
 * - ENETDOWN: Network is down
 * - ENETUNREACH: Network is unreachable
 * - ENETRESET: Network dropped connection on reset
 * - ECONNABORTED: Software caused connection abort
 * - ECONNRESET: Connection reset by peer
 * - ENOBUFS: No buffer space available
 * - EISCONN: Socket is already connected
 * - ENOTCONN: Socket is not connected
 * - ESHUTDOWN: Can't send after socket shutdown
 * - ETOOMANYREFS: Too many references: can't splice
 * - ETIMEDOUT: Connection timed out
 * - ECONNREFUSED: Connection refused
 * - ELOOP: Too many levels of symbolic links
 * - ENAMETOOLONG: File name too long
 * - EHOSTDOWN: Host is down
 * - EHOSTUNREACH: No route to host
 * - ENOTEMPTY: Directory not empty
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 *
 * \note Applications must not call this function to retrieve the
 *       error code if NutTcpCloseSocket() or NutTcpDestroySocket()
 *       failed.
 *
 * \bug  Not all error codes are properly set right now. Some socket
 *       functions return an error without setting an error code.
 */
int NutTcpError(TCPSOCKET * sock)
{
    if (sock == 0)
        return ENOTSOCK;
    return sock->so_last_error;
}

/*! 
 * \brief Read from device. 
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 */
int NutTcpDeviceRead(TCPSOCKET * sock, void *buffer, int size)
{
    return NutTcpReceive(sock, buffer, size);
}

static int SendBuffer(TCPSOCKET * sock, CONST void *buffer, int size)
{
    int rc;
    int bite;

    for (rc = 0; rc < size; rc += bite) {
        if ((bite = NutTcpSend(sock, (u_char *) buffer + rc, size - rc)) <= 0) {
            return -1;
        }
    }
    return rc;
}

/*! 
 * \brief Write to a socket.
 *
 * In contrast to NutTcpSend() this routine provides some buffering.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 *
 */
int NutTcpDeviceWrite(TCPSOCKET * sock, CONST void *buffer, int size)
{
    int rc, sz;

    /* Flush buffer? */
    if (size == 0) {
        if (sock->so_devocnt) {
            if (SendBuffer(sock, sock->so_devobuf, sock->so_devocnt) < 0) {
                NutHeapFree(sock->so_devobuf);
                sock->so_devocnt = 0;
                return -1;
            }
            NutHeapFree(sock->so_devobuf);
            sock->so_devocnt = 0;
        }
        return 0;
    }

    /* If we don't have a buffer so far... */
    if (sock->so_devocnt == 0) {
        /* If new data block is bigger or equal than buffer size
         * send first part of data to nic and store remaining
         * bytes in buffer
         */
        if (size >= sock->so_devobsz) {
            rc = size % sock->so_devobsz;
            SendBuffer(sock, buffer, size - rc);
            buffer += size - rc;
        } else
            rc = size;

        /* If there are some remainings bytes, allocate buffer
         * and store them
         */
        if (rc) {
            if (!(sock->so_devobuf = NutHeapAlloc(sock->so_devobsz)))
                return -1;
            memcpy(sock->so_devobuf, buffer, rc);
            sock->so_devocnt = rc;
        }
        return size;
    }

    /* Check if new data fully fits in output buffer */
    if (sock->so_devocnt + size < sock->so_devobsz) {
        memcpy(sock->so_devobuf + sock->so_devocnt, buffer, size);
        sock->so_devocnt += size;
        return size;
    }

    /* Otherwise store first bytes of new data in buffer and flush
     * the buffer
     */
    sz = sock->so_devobsz - sock->so_devocnt;
    memcpy(sock->so_devobuf + sock->so_devocnt, buffer, sz);
    buffer += sz;
    if (SendBuffer(sock, sock->so_devobuf, sock->so_devobsz) < 0) {
        NutHeapFree(sock->so_devobuf);
        sock->so_devocnt = 0;
        return -1;
    }

    /* If remaining data is bigger or equal than buffer size
     * send first part of data to nic and later store remaining
     * bytes in buffer
     */
    sz = size - sz;
    if (sz >= sock->so_devobsz) {
        rc = sz % sock->so_devobsz;
        SendBuffer(sock, buffer, sz - rc);
        buffer += sz - rc;
    } else
        rc = sz;

    /* If there are some remainings bytes, store them in buffer
     */
    if (rc)
        memcpy(sock->so_devobuf, buffer, rc);
    else                        /* Otherwise free buffer */
        NutHeapFree(sock->so_devobuf);
    sock->so_devocnt = rc;

    return size;
}

/*! 
 * \brief Write to device.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 *
 * \warning Inefficient implementation.
 */
int NutTcpDeviceWrite_P(TCPSOCKET * sock, PGM_P buffer, int size)
{
    int rc;
    char *rp = 0;

    /*
     * Hack alert. Neither do we handle out of memory correctly
     * nor do we pass the PGM pointer to lower levels.
     */
    if (size && (rp = NutHeapAlloc(size)) != 0)
        memcpy_P(rp, buffer, size);
    rc = NutTcpDeviceWrite(sock, rp, size);
    if (rp)
        NutHeapFree(rp);

    return rc;
}

/*! 
 * \brief Driver control function.
 *
 * Used to modify or query device specific settings.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutTcpCreateSocket().
 */
int NutTcpDeviceIOCtl(TCPSOCKET * sock, int cmd, void *param)
{
    return 0;
}

/*@}*/
