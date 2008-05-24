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
 * Revision 1.5  2008/05/24 22:22:26  olereinhardt
 * 2008-05-25  Ole Reinhardt <ole.reinhardt@embedded-it.de>
 *         * net/icmpin.c: Fixed size comparision in NutIcmpUnreach
 *
 * Revision 1.4  2004/03/18 10:28:36  haraldkipp
 * Comments updated
 *
 * Revision 1.3  2004/02/06 19:23:59  drsung
 * Bugfix. After last changes, ping didn't work any more. Thanks to Pavel Celeda, who discovered this bug.
 *
 * Revision 1.2  2004/02/02 18:59:25  drsung
 * Some more ICMP support added.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:28  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.11  2003/02/04 18:14:57  harald
 * Version 3 released
 *
 * Revision 1.10  2002/06/26 17:29:35  harald
 * First pre-release with 2.4 stack
 *
 */

#include <netinet/ip_icmp.h>
#include <netinet/ipcsum.h>
#include <netinet/icmp.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <net/errno.h>
#include <netinet/in.h>

/*!
 * \addtogroup xgICMP
 */
/*@{*/

/*
 * Send out ICMP echo response.
 */
static int NutIcmpReflect(NUTDEVICE * dev, u_char type, NETBUF * nb)
{
    IPHDR *ip;
    ICMPHDR *icmp;
    u_long dest;
    IFNET *nif;

    ip = nb->nb_nw.vp;
    icmp = nb->nb_tp.vp;
    dest = ip->ip_src;
    nif = dev->dev_icb;
    ip->ip_src = nif->if_local_ip;
    ip->ip_ttl = MAXTTL;

    return NutIcmpOutput(type, dest, nb);
}

/*
 * Process incoming ICMP messages for destination unreachable.
 */
static int NutIcmpUnreach(NETBUF * nb)
{
    IPHDR *ih;
    TCPHDR *th;
    TCPSOCKET *sock;

    if (nb->nb_ap.sz < sizeof(IPHDR) + 8)
        return -1;

    ih = nb->nb_ap.vp;
    if (ih->ip_p != IPPROTO_TCP)
        return -1;

    th = (TCPHDR *) ((u_char *) ih) + sizeof(IPHDR);
    sock = NutTcpFindSocket(th->th_dport, th->th_sport, ih->ip_src);
    if (sock == 0)
        return -1;

    if (sock->so_state != TCPS_SYN_SENT && sock->so_state != TCPS_ESTABLISHED)
        return -1;

    NutTcpAbortSocket(sock, EHOSTUNREACH);
    NutNetBufFree(nb);
    return 0;
}

/*!
 * \brief Handle incoming ICMP packets.
 *
 * Incoming ICMP packets are processed in the background.
 * NutNet currently handles echo request and destination
 * unreachable packets. Any other packet type is silently 
 * discarded.
 *
 * \note This routine is called by the IP layer on incoming 
 *       ICMP datagrams. Applications typically do not call 
 *       this function.
 *
 * \param dev Identifies the device that received the packet.
 * \param nb  Pointer to a network buffer structure containing 
 *            the ICMP datagram.
 */
void NutIcmpInput(NUTDEVICE * dev, NETBUF * nb)
{
    /*
     * Silently discard packets, which are too small.
     */
    if (nb->nb_tp.sz > ICMP_MINLEN) {
        ICMPHDR *icp = (ICMPHDR *) nb->nb_tp.vp;

        if (nb->nb_tp.sz > sizeof(ICMPHDR)) {
            nb->nb_ap.sz = nb->nb_tp.sz - sizeof(ICMPHDR);
            nb->nb_ap.vp = ((u_char *) icp) + sizeof(ICMPHDR);
            nb->nb_tp.sz = sizeof(ICMPHDR);
        }

        switch (icp->icmp_type) {
        case ICMP_ECHO:
            if (NutIcmpReflect(dev, ICMP_ECHOREPLY, nb) == 0)
                NutNetBufFree(nb);
            break;
        case ICMP_UNREACH:
            if (NutIcmpUnreach(nb) != 0)
                NutNetBufFree(nb);
            break;
        default:
            NutNetBufFree(nb);
        }
    } else
        NutNetBufFree(nb);
}

/*@}*/
