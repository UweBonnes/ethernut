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
 * Revision 1.1  2003/05/09 14:41:33  haraldkipp
 * Initial revision
 *
 * Revision 1.17  2003/04/01 18:31:47  harald
 * *** empty log message ***
 *
 * Revision 1.16  2003/03/31 12:28:10  harald
 * Check NEBUF allocation
 *
 * Revision 1.15  2003/02/04 18:14:57  harald
 * Version 3 released
 *
 * Revision 1.14  2003/01/14 16:44:08  harald
 * *** empty log message ***
 *
 * Revision 1.13  2002/10/29 15:32:14  harald
 * PPP support
 *
 * Revision 1.12  2002/09/15 16:49:42  harald
 * *** empty log message ***
 *
 * Revision 1.11  2002/06/26 17:29:36  harald
 * First pre-release with 2.4 stack
 *
 */

#include <string.h>

#include <net/ether.h>
#include <net/route.h>
#include <netinet/if_ether.h>
#include <netinet/if_ppp.h>
#include <netinet/ipcsum.h>
#include <netinet/ip.h>

/*!
 * \addtogroup xgIP
 */
/*@{*/

/*!
 * \brief Send IP datagram.
 *
 * Route an IP datagram to the proper interface.
 *
 * The function will not return until the data has been stored 
 * in the network device hardware for transmission. If the 
 * device is not ready for transmitting a new packet, the 
 * calling thread will be suspended until the device becomes 
 * ready again. If the hardware address of the target host needs 
 * to be resolved the function will be suspended too.
 *
 * \param proto Protocol type.
 * \param dest  Destination IP address. The function will determine
 *              the proper network interface by checking the routing
 *              table. It will also perform any neccessary hardware
 *              address resolution.
 * \param nb    Network buffer structure containing the datagram. 
 *              This buffer will be released if the function returns
 *              an error.
 *
 * \return 0 on success, -1 otherwise.
 *
 * \bug Broadcasts to multiple network devices will fail after the
 *      first device returns an error.
 */
int NutIpOutput(u_char proto, u_long dest, NETBUF * nb)
{
    u_char ha[6];
    IPHDR *ip;
    NUTDEVICE *dev;
    IFNET *nif;
    u_long gate;
    int rc;

    if ((nb = NutNetBufAlloc(nb, NBAF_NETWORK, sizeof(IPHDR))) == 0)
        return -1;

    /*
     * Set those items in the IP header, which are common for
     * all interfaces.
     */
    ip = nb->nb_nw.vp;
    ip->ip_v = 4;
    ip->ip_hl = sizeof(IPHDR) / 4;
    ip->ip_tos = 0;
    ip->ip_len = htons(nb->nb_nw.sz + nb->nb_tp.sz + nb->nb_ap.sz);
    ip->ip_off = 0;
    ip->ip_ttl = 0x40;
    ip->ip_p = proto;
    ip->ip_dst = dest;

    /*
     * Broadcasts are sent on all network interfaces.
     */
    if (dest == 0xffffffff) {

        memset(ha, 0xff, sizeof(ha));

        for (dev = nutDeviceList, rc = 0; dev && rc == 0; dev = dev->dev_next) {
            if (dev->dev_type == IFTYP_NET) {

                /*
                 * Set remaining IP header items and calculate the checksum.
                 */
                nif = dev->dev_icb;
                ip->ip_id = htons(nif->if_pkt_id++);
                ip->ip_src = nif->if_local_ip;
                ip->ip_sum = 0;
                ip->ip_sum = NutIpChkSum(0, nb->nb_nw.vp, nb->nb_nw.sz);
                /*
                 * TODO: We must clone the NETBUF!!!
                 */
                if (nif->if_type == IFT_ETHER)
                    rc = (*nif->if_output)(dev, ETHERTYPE_IP, ha, nb);
                else 
                    rc = (*nif->if_output)(dev, PPP_IP, 0, nb);
            }
        }
        return rc;
    }

    /*
     * Get destination's route. This will also return the proper 
     * interface.
     */
    if ((dev = NutIpRouteQuery(dest, &gate)) == 0) {
        NutNetBufFree(nb);
        return -1;
    }

    /*
     * Set remaining IP header items and calculate the checksum.
     */
    nif = dev->dev_icb;
    ip->ip_id = htons(nif->if_pkt_id++);
    ip->ip_src = nif->if_local_ip;
    ip->ip_sum = 0;
    ip->ip_sum = NutIpChkSum(0, nb->nb_nw.vp, nb->nb_nw.sz);

    /*
     * On Ethernet we query the MAC address of our next hop,
     * which might be the destination or the gateway to this
     * destination.
     */
    if (nif->if_type == IFT_ETHER) {
        if (NutArpCacheQuery(dev, gate ? gate : dest, ha))
            return -1;
        return (*nif->if_output)(dev, ETHERTYPE_IP, ha, nb);
    }
    else if (nif->if_type == IFT_PPP) 
        return (*nif->if_output)(dev, PPP_IP, 0, nb);

    NutNetBufFree(nb);
    return -1;
}

/*@}*/
