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
 * Revision 1.2  2004/02/02 18:59:25  drsung
 * Some more ICMP support added.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:32  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.16  2003/05/06 18:14:18  harald
 * Allow incoming DHCP telegrams, even if not broadcasted
 *
 * Revision 1.15  2003/03/31 12:26:05  harald
 * Accept masks with all bits set
 *
 * Revision 1.14  2003/02/04 18:14:57  harald
 * Version 3 released
 *
 * Revision 1.13  2002/06/26 17:29:36  harald
 * First pre-release with 2.4 stack
 *
 */

#include <net/route.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/icmp.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/*!
 * \addtogroup xgIP
 */
/*@{*/

/*!
 * \brief Process incoming IP datagrams.
 *
 * Datagrams addressed to other destinations and datagrams 
 * whose version number is not 4 are silently discarded.
 *
 * \note This routine is called by the Ethernet layer on
 *       incoming IP datagrams. Applications typically do
 *       not call this function.
 *
 * \param dev Identifies the device that received this datagram.
 * \param nb  The network buffer received.
 */
void NutIpInput(NUTDEVICE * dev, NETBUF * nb)
{
    IPHDR *ip;
    u_short ip_hdrlen;
    u_long dst;
    u_char bcast;
    IFNET *nif;

    ip = nb->nb_nw.vp;

    /*
     * Silently discard datagrams of different IP version
     * and fragmented datagrams.
     */
    if (ip->ip_v != IPVERSION || (ntohs(ip->ip_off) & (IP_MF | IP_OFFMASK)) != 0) {
        NutNetBufFree(nb);
        return;
    }

    ip_hdrlen = ip->ip_hl * 4;

    /*
     * No checksum calculation on incoming datagrams!
     */

    /*
     * Check for broadcast.
     */
    dst = ip->ip_dst;
    nif = dev->dev_icb;

    if (dst == 0xffffffff || (nif->if_local_ip && nif->if_mask != 0xffffffff && (dst | nif->if_mask) == 0xffffffff)) {
        bcast = 1;
    } else {
        bcast = 0;

        if (nif->if_local_ip == 0 && ip->ip_p == IPPROTO_ICMP && (dst & 0xff000000) != 0xff000000 && (dst & 0xff000000) != 0) {
            NutNetIfSetup(dev, dst, 0, 0);
        }

        /*
         * Silently discard datagrams for other destinations.
         * However, if we haven't got an IP address yet, we
         * allow ICMP datagrams to support dynamic IP ARP method.
         */
        if (nif->if_local_ip && (dst == 0 || dst != nif->if_local_ip)) {
            NutNetBufFree(nb);
            return;
        }
    }

    nb->nb_nw.sz = ip_hdrlen;
    nb->nb_tp.vp = ((u_char *) ip) + (ip_hdrlen);
    nb->nb_tp.sz = htons(ip->ip_len) - (ip_hdrlen);

    switch (ip->ip_p) {
    case IPPROTO_ICMP:
        NutIcmpInput(dev, nb);
        break;
    case IPPROTO_UDP:
        NutUdpInput(nb);
        break;
    case IPPROTO_TCP:
        /*
         * Silently discard TCP broadcasts.
         */
        if (bcast)
            NutNetBufFree(nb);
        else
            NutTcpInput(nb);
        break;
    default:
        /* Unkown protocol, send ICMP destination (protocol)
         * unreachable message.
         */
        if (!NutIcmpResponse(ICMP_UNREACH, ICMP_UNREACH_PROTOCOL, 0, nb))
            NutNetBufFree(nb);
        break;
    }
}

/*@}*/
