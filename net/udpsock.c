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
 * Revision 1.3  2003/08/14 15:07:18  haraldkipp
 * Optimization
 *
 * Revision 1.2  2003/07/20 16:01:01  haraldkipp
 * Bugfix: Send crashed on routing problems.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:46  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.13  2003/05/06 18:21:52  harald
 * Memory hole fixed
 *
 * Revision 1.12  2003/04/21 17:04:57  harald
 * Bugfix: Return port number in host byte order
 *
 * Revision 1.11  2003/02/04 18:14:57  harald
 * Version 3 released
 *
 * Revision 1.10  2002/06/26 17:29:36  harald
 * First pre-release with 2.4 stack
 *
 */

#include <string.h>

#include <sys/heap.h>
#include <sys/event.h>

#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>

/*!
 * \addtogroup xgUdpSocket
 */
/*@{*/


UDPSOCKET *udpSocketList;   /*!< Linked list of all UDP sockets. */
static u_short last_local_port = 4096;  /* Unassigned local port. */

/*!
 * \brief Create an UDP socket.
 *
 * \param  port Server applications provide the local port number
 *              with this parameter. Client applications should
 *              pass zero.
 *
 * \return Socket descriptor of the newly created UDP socket or
 *         0 if there is not enough memory left.
 *
 */
UDPSOCKET *NutUdpCreateSocket(u_short port)
{
    UDPSOCKET *sock;

    if (port == 0) {
        do {
            if (++last_local_port == 0)
                last_local_port = 4096;
            port = htons(last_local_port);
            sock = udpSocketList;
            while (sock) {
                if (sock->so_local_port == port)
                    break;
                sock = sock->so_next;
            }
        } while (sock);
        port = last_local_port;
    }
    if ((sock = NutHeapAllocClear(sizeof(UDPSOCKET))) != 0) {
        sock->so_local_port = htons(port);
        sock->so_next = udpSocketList;
        udpSocketList = sock;
    }
    return sock;
}

/*!
 * \brief Send an UDP datagram.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutUdpCreateSocket().
 * \param addr IP address of the remote host in network byte order.
 * \param port Remote port number in host byte order.
 * \param data Pointer to a buffer containing the data to send.
 * \param len  Number of bytes to be sent.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutUdpSendTo(UDPSOCKET * sock, u_long addr, u_short port, void *data,
                 u_short len)
{
    int rc;
    NETBUF *nb;

    if ((nb = NutNetBufAlloc(0, NBAF_APPLICATION, len)) == 0) {
        return -1;
    }
    memcpy(nb->nb_ap.vp, data, len);

    /* Bugfix by Ralph Mason. We should not free the NETBUF in case of an error. */
    if((rc = NutUdpOutput(sock, addr, port, nb)) == 0)
        NutNetBufFree(nb);

    return rc;
}

/*!
 * \brief Receive an UDP datagram.
 *
 * \param sock    Socket descriptor. This pointer must have been 
 *                retrieved by calling NutUdpCreateSocket().
 * \param addr    IP address of the remote host in network byte order.
 * \param port    Remote port number in host byte order.
 * \param data    Pointer to the buffer that receives the data.
 * \param size    Size of the buffer that receives the data.
 * \param timeout Maximum number of milliseconds to wait.
 *
 * \return The number of bytes received, if successful. The return
 *         value -1 indicates an error. On timeout 0 is returned.
 *
 * \note Timeout is limited to the granularity of the system timer.
 */
int NutUdpReceiveFrom(UDPSOCKET * sock, u_long * addr, u_short * port,
                      void *data, u_short size, u_long timeout)
{
    IPHDR *ip;
    UDPHDR *uh;
    NETBUF *nb;

    if (sock->so_rx_nb == 0)
        NutEventWait(&sock->so_rx_rdy, timeout);

    if ((nb = sock->so_rx_nb) == 0)
        return 0;
    sock->so_rx_nb = 0;

    ip = nb->nb_nw.vp;
    *addr = ip->ip_src;

    uh = nb->nb_tp.vp;
    *port = htons(uh->uh_sport);

    if (size > nb->nb_ap.sz)
        size = nb->nb_ap.sz;

    memcpy(data, nb->nb_ap.vp, size);
    NutNetBufFree(nb);

    return size;
}

/*!
 * \brief Close UDP socket.
 *
 * The memory occupied by the socket is immediately released
 * after calling this function. The application  must not use 
 * the socket after this call.
 *
 * \param sock Socket descriptor. This pointer must have been 
 *             retrieved by calling NutUdpCreateSocket().
 *
 * \return 0 on success, -1 otherwise.
 */
int NutUdpDestroySocket(UDPSOCKET * sock)
{
    UDPSOCKET *sp;
    UDPSOCKET **spp;
    int rc = -1;

    spp = &udpSocketList;
    sp = udpSocketList;

    while (sp) {
        if (sp == sock) {
            *spp = sock->so_next;
            /* A packet may have arrived that the application 
               did not read before closing the socket. */
            if ( sock->so_rx_nb != 0 ) {
			NutNetBufFree(sock->so_rx_nb);
            }
            NutHeapFree(sock);
            rc = 0;
            break;
        }
        spp = &sp->so_next;
        sp = sp->so_next;
    }
    return rc;
}

/*!
 * \brief Find a matching socket.
 *
 * Loop through all sockets and find a matching one.
 *
 * \note Applications typically do not need to call this function.
 *
 * \param port  Local port number.
 *
 * \return Socket descriptor.
 */
UDPSOCKET *NutUdpFindSocket(u_short port)
{
    UDPSOCKET *sp;
    UDPSOCKET *sock = 0;

    for (sp = udpSocketList; sp; sp = sp->so_next) {
        if (sp->so_local_port == port) {
            sock = sp;
            break;
        }
    }
    return sock;
}

/*@}*/
