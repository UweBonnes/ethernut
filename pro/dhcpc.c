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
 * Revision 1.2  2003/05/15 14:25:38  haraldkipp
 * Some DHCP servers discard discover telegrams.
 * Thanks to Tomohiro Haraikawa.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:57  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.14  2003/05/08 12:02:50  harald
 * More consistent handling if DHCP not available
 *
 * Revision 1.13  2003/05/06 18:59:40  harald
 * Avoid running with all zero MAC
 *
 * Revision 1.12  2003/04/21 17:12:02  harald
 * Added new configuration routine, which allows timeout.
 * Auto configuration moved to DHCP module.
 *
 * Revision 1.11  2003/03/31 14:53:31  harald
 * Prepare release 3.1
 *
 * Revision 1.10  2003/02/04 18:17:32  harald
 * Version 3 released
 *
 * Revision 1.9  2002/06/26 17:29:49  harald
 * First pre-release with 2.4 stack
 *
 */

#include <string.h>

#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/confnet.h>

#include <netinet/in.h>
#include <net/route.h>
#include <sys/socket.h>
#include <pro/dhcp.h>

/*!
 * \addtogroup xgDHCPC
 */
/*@{*/

typedef struct {
    u_char op;                  /* 1=request, 2=reply */
    u_long ip;
    u_long netmask;
    u_long broadcast;
    u_long gateway;
    u_long dns;
    u_long sid;
    u_long xid;
    u_long leaseTime;
    u_long rebindTime;
    u_long renewalTime;
    u_long ackTime;
    u_char *hostname;
    u_char *domain;
} DYNCFG;

static u_char egnite_null_mac[] = { 0x00, 0x06, 0x98, 0x00, 0x00, 0x00 };
static u_char cookie[4] = { 0x63, 0x82, 0x53, 0x63 };
static u_char macaddr[6];

static volatile HANDLE dhcpDone;

static void copy_str(u_char ** dst, void *src, int len)
{
    if (*dst) {
        NutHeapFree(*dst);
    }
    *dst = NutHeapAlloc(len + 1);
    if (len)
        memcpy(*dst, src, len);
    *(*dst + len) = 0;
}

/*!
 * \brief Parse a DHCP reply telegram.
 *
 * \param cfgp Pointer to config structure.
 * \param bp Pointer to the reply telegram.
 * \param len Number of valid bytes in the reply telegram.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutDhcpParse(DYNCFG * cfgp, struct bootp *bp, int len)
{
    u_char *cp;
    u_long lval;
    int left;
    int ol;
    int result;

    memset(cfgp, 0, sizeof(*cfgp));

    result = 0;
    memcpy(&cfgp->ip, &bp->bp_yiaddr, 4);
    cfgp->xid = bp->bp_xid;

    cp = bp->bp_options + 4;
    left = len - (sizeof(*bp) - sizeof(bp->bp_options)) - 4;
    while (result == 0 && left > 0) {
        if (*cp == DHCPOPT_END)
            break;
        if (*cp == DHCPOPT_PAD) {
            cp++;
            left--;
            continue;
        }
        if ((ol = *(cp + 1)) > left) {
            result = -1;
            break;
        }

        lval = *((u_long *) (cp + 2));
        switch (*cp) {
        case DHCPOPT_MSGTYPE:
            if (ol == 1)
                cfgp->op = *(cp + 2);
            else
                result = -1;
            break;
        case DHCPOPT_NETMASK:
            if (ol >= 4)
                cfgp->netmask = lval;
            else
                result = -1;
            break;
        case DHCPOPT_BROADCAST:
            if (ol >= 4)
                cfgp->broadcast = lval;
            else
                result = -1;
            break;
        case DHCPOPT_GATEWAY:
            if (ol >= 4)
                cfgp->gateway = lval;
            else
                result = -1;
            break;
        case DHCPOPT_HOSTNAME:
            copy_str(&cfgp->hostname, cp + 2, ol);
            break;
        case DHCPOPT_DOMAIN:
            copy_str(&cfgp->domain, cp + 2, ol);
            break;
        case DHCPOPT_DNS:
            /*
             * Fix me. 
             * There may be several entries, but
             * we take the first only.
             */
            if (ol >= 4)
                cfgp->dns = lval;
            else
                result = -1;
            break;
        case DHCPOPT_SID:
            if (ol >= 4)
                cfgp->sid = lval;
            else
                result = -1;
            break;
        case DHCPOPT_LEASETIME:
            if (ol >= 4)
                cfgp->leaseTime = ntohl(lval);
            else
                result = -1;
            break;
        case DHCPOPT_RENEWALTIME:
            if (ol >= 4)
                cfgp->renewalTime = ntohl(lval);
            else
                result = -1;
            break;
        case DHCPOPT_REBINDTIME:
            if (ol >= 4)
                cfgp->rebindTime = ntohl(lval);
            else
                result = -1;
            break;
        }
        cp += ol + 2;
        left -= ol + 2;
    }
    return result;
}

/*!
 * Collect offer responses.
 *
 * \param sock Handle of the listening socket.
 * \param wsecs Maximum seconds to wait for a telegram.
 *
 * \return Number of bytes received, zero on timeout or -1 on error.
 */
static int NutDhcpReceive(UDPSOCKET * sock, struct bootp *rxbuf,
                          u_long wsecs)
{
    u_long addr;
    u_short port;
    int rc;

    rc = NutUdpReceiveFrom(sock, &addr, &port, rxbuf, sizeof(struct bootp),
                           wsecs * 1000);
    return rc;
}

/*
 * Prepare bootp telegram header.
 */
static u_char *prep_header(struct bootp *bp, u_char msgtyp, u_long xid)
{
    u_char *op;

    memset(bp, 0, sizeof(*bp));
    bp->bp_op = 1;
    bp->bp_xid = xid;

    bp->bp_htype = 1;
    bp->bp_hlen = 6;
    /*
     * According to RFC 2131 we need to set the broadcast flag.
     * Many thanks to Andre Albsmeier, who found this problem
     * while using ISC-DHCP 3.
     */
    bp->bp_flags = htons(0x8000);
    memcpy(bp->bp_chaddr, macaddr, bp->bp_hlen);

    op = bp->bp_options;
    memcpy(op, cookie, 4);
    op += 4;
    *op++ = DHCPOPT_MSGTYPE;
    *op++ = 1;
    *op++ = msgtyp;

    return op;
}

/*!
 * \brief Broadcast a DHCP discover telegram.
 *
 * \param sock  Socket descriptor. This pointer must have been 
 *              retrieved by calling NutUdpCreateSocket().
 * \param daddr Destination IP address, typically INADDR_BROADCAST.
 * \param dport Destination port, typically 67.
 * \param bp    Pointer to a buffer to be used for transmission.
 *              No specific initialization required.
 * \param cfgp  Pointer to an initialized DYNCFG structure.
 *
 * \return 0 on success, -1 if send failed.
 */
int NutDhcpDiscover(UDPSOCKET * sock, u_long daddr, u_short dport,
                    struct bootp *bp, DYNCFG * cfgp)
{
    char *cp;
    u_short len;

    cp = prep_header(bp, cfgp->op = DHCP_DISCOVER, cfgp->xid);
    bp->bp_secs = htons(cfgp->ackTime);

    *cp++ = DHCPOPT_END;

    /* DHCP discover telegram should be upward compatible with BOOTP */
    len = (u_short) cp - (u_short) bp;
    if (len < (char *)&bp->bp_options - (char *)bp + 64)
        len = (char *)&bp->bp_options - (char *)bp + 64;

    return NutUdpSendTo(sock, daddr, dport, bp, len);
}


/*!
 * Broadcast the offered settings.
 *
 * \param sock  Socket descriptor. This pointer must have been 
 *              retrieved by calling NutUdpCreateSocket().
 * \param daddr Destination IP address, typically INADDR_BROADCAST.
 * \param dport Destination port, typically 67.
 * \param bp    Pointer to a buffer to be used for transmission.
 * \param cfgp  Pointer to an initialized DYNCFG structure.
 *
 * \return 0 on success, -1 if send failed.
 */
int NutDhcpRequest(UDPSOCKET * sock, u_long daddr, u_short dport,
                   struct bootp *bp, DYNCFG * cfgp)
{
    char *cp;

    cp = prep_header(bp, cfgp->op = DHCP_REQUEST, cfgp->xid);

    *cp++ = DHCPOPT_REQESTIP;
    *cp++ = 4;
    memcpy(cp, &cfgp->ip, 4);
    cp += 4;

    *cp++ = DHCPOPT_SID;
    *cp++ = 4;
    memcpy(cp, &cfgp->sid, 4);
    cp += 4;

    *cp++ = DHCPOPT_END;

    return NutUdpSendTo(sock, daddr, dport, bp,
                        (u_short) cp - (u_short) bp);
}

/*!
 * Release our ip address.
 */
int NutDhcpRelease(UDPSOCKET * sock, u_long daddr, u_short dport,
                   struct bootp *bp, DYNCFG * cfgp)
{
    char *cp;

    cp = prep_header(bp, cfgp->op = DHCP_RELEASE, cfgp->xid);
    memcpy(&bp->bp_yiaddr, &cfgp->ip, 4);

    *cp++ = DHCPOPT_SID;
    *cp++ = 4;
    memcpy(cp, &cfgp->sid, 4);
    cp += 4;

    *cp++ = DHCPOPT_END;

    return NutUdpSendTo(sock, daddr, dport, bp,
                        (u_short) cp - (u_short) bp);
}


/*! \fn NutDhcpClient(void *arg)
 * \brief DHCP client thread. 
 */
THREAD(NutDhcpClient, arg)
{
    DYNCFG *rxcfg;
    DYNCFG *txcfg;
    UDPSOCKET *sock;
    int n;
    int try;
    int wait_secs;
    u_short boot_secs;
    int got_offer;
    struct bootp *rxbuf;
    struct bootp *txbuf;
    u_long xid;
    NUTDEVICE *dev;
    IFNET *nif;

    dev = arg;
    nif = dev->dev_icb;
    xid = NutGetTickCount() + htonl(*((u_long *)&nif->if_mac[2]));
    boot_secs = 0;
    wait_secs = 3;
    got_offer = 0;

    sock = NutUdpCreateSocket(DHCP_CLIENTPORT);

    rxcfg = NutHeapAlloc(sizeof(DYNCFG));
    txcfg = NutHeapAlloc(sizeof(DYNCFG));
    rxbuf = NutHeapAlloc(sizeof(struct bootp));
    txbuf = NutHeapAlloc(sizeof(struct bootp));
    while (sock) {
        got_offer = 0;

        /*
         * This loop will broadcast discover telegrams and
         * then listen upto 'wait_secs' seconds for offers.
         */
        for (try = 0; try < 10; try++) {
            memset(txcfg, 0, sizeof(DYNCFG));
            txcfg->xid = xid;
            txcfg->ackTime = boot_secs;
            if (NutDhcpDiscover
                (sock, INADDR_BROADCAST, DHCP_SERVERPORT, txbuf,
                 txcfg) < 0)
                break;

            /*
             * Wait for offers. Only the first valid offer
             * is processed. Quit this loop after 'wait_secs'
             * seconds without any response.
             */
            for (;;) {
                if ((n = NutDhcpReceive(sock, rxbuf, wait_secs)) == 0) {
                    boot_secs += wait_secs;
                    break;
                }
                if (rxbuf->bp_xid == xid) {
                    if (NutDhcpParse(rxcfg, rxbuf, n) == 0) {
                        got_offer = 1;
                        break;
                    }
                }
            }
            if (got_offer)
                break;
        }
        if (!got_offer)
            break;

        got_offer = 0;
        memcpy(txcfg, rxcfg, sizeof(DYNCFG));
        if (NutDhcpRequest(sock, INADDR_BROADCAST, DHCP_SERVERPORT, txbuf, txcfg) >= 0) {
            if ((n = NutDhcpReceive(sock, rxbuf, 5)) >= 0
                && rxbuf->bp_xid == xid) {
                if (NutDhcpParse(rxcfg, rxbuf, n) == 0) {
                    got_offer = 1;
                    break;
                }
            }
            break;
        }
    }

    /*
     * If we got an offer, which is accepted by the interface setup
     * routine, we wake up the application thread and luckily fall 
     * asleep.
     */
    if(got_offer && NutNetIfSetup(dev, rxcfg->ip, rxcfg->netmask, rxcfg->gateway) == 0) {
        NutEventPost((HANDLE *) & dhcpDone);
        /*
         * Fix me: We should renew our ip.
         */
        NutThreadSetPriority(254);
        for (;;)
            NutSleep(60000);
    }

    /*
     * No success with DHCP. Check if any previously assigned IP 
     * configuration is available. If this fails, then wait
     * until someone else configures the inferface. This may be 
     * through ARP method or by the application.
     */
    if(NutNetIfSetup(dev, 0, 0, 0)) {
        NutThreadSetPriority(254);
        while (nif->if_local_ip == 0)
            NutThreadYield();
    }
    NutEventPost((HANDLE *) & dhcpDone);

    /*
     * Too bad that Nut/OS doesn't support thread exit.
     */
    NutThreadSetPriority(254);
    for (;;)
        NutSleep(60000);
}

/*!
 * \brief Automatically configure an Ethernet network interface.
 *
 * If no MAC address is specified, this routine will try to read
 * a previously stored configuration from the EEPROM. If this
 * retrieves a valid IP configuration, then the network interface
 * will be immediately configured with these values by calling
 * NutNetIfConfig(). If no valid IP configuration has been read,
 * then this routine will start the DHCP client thread and wait
 * upto a given number of milliseconds for a response from a
 * DHCP server.
 *
 * If a MAC address has been specified, this routine will not
 * read the EEPROM configuration. If the application has set
 * the global ::CONFNET structure to a valid IP configuration
 * before calling this function, then the network interface
 * will be immediately configured with these values by calling
 * NutNetIfConfig(). Otherwise the DHCP client thread will be
 * started and this routine wiill wait upto a given number of 
 * milliseconds for a response from a DHCP server.
 *
 * \param name    Name of the registered Ethernet device.
 * \param mac     MAC address of the destination. Set null to use 
 *                the configuration stored in the EEPROM.
 * \param timeout Maximum number of milliseconds to wait.
 *
 * \return 0 on success or -1, if no IP configuration could have
 *         been received within the specified timeout.
 *
 * \bug The client doesn't send update information to the server. To 
 *      avoid loosing the assignment while Ethernut is down, addresses 
 *      can be fixed in the DHCP server tables.
 */
int NutDhcpIfConfig(CONST char *name, u_char *mac, u_long timeout)
{
    NUTDEVICE *dev;
    IFNET *nif;

    /*
     * Open the device and make sure we have a network.
     */
    if ((dev = NutDeviceLookup(name)) == 0 || dev->dev_type != IFTYP_NET)
        return -1;

    /*
     * We support Ethernet devices only.
     */
    nif = dev->dev_icb;
    if (nif->if_type != IFT_ETHER)
        return -1;

    /*
     * If no MAC address has been specified, read the configuration 
     * from EEPROM. If this fails, use egnite's null MAC.
     */
    if(mac == 0) {
        if(NutNetLoadConfig(name))
            memcpy(confnet.cdn_mac, egnite_null_mac, 6);
        mac = confnet.cdn_mac;
    }
    memcpy(nif->if_mac, mac, 6);

    /*
     * If we got an invalid configuration, then start the
     * DHCP client and wait the given number of milliseconds
     * for a response from the DHCP server.
     */
    if((confnet.cdn_cip_addr & confnet.cdn_ip_mask) == 0) {
        memcpy(macaddr, nif->if_mac, 6);
        NutThreadCreate("dhcpc", NutDhcpClient, dev, 512);
        return NutEventWait((HANDLE *) & dhcpDone, timeout);
    }

    /*
     * We got a valid IP configuration. Configure the interface.
     */
    return NutNetIfConfig(name, confnet.cdn_mac, confnet.cdn_cip_addr, confnet.cdn_ip_mask);
}

/*!
 * \brief Automatically configure a network interface.
 *
 * This function is deprecated. New applications should call
 * NutDhcpIfConfig().
 *
 * 
 *
 * \param name Name of the device.
 *
 * \return 0 on success, -1 otherwise.
 *
 */
int NutNetAutoConfig(CONST char *name)
{
    return NutDhcpIfConfig(name, 0, 0);
}

/*@}*/
