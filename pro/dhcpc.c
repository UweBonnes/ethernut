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
 * Revision 1.9  2004/02/02 18:54:43  drsung
 * gateway ip address was not set, if static network configuration from EEPROM is used.
 *
 * Revision 1.8  2004/01/14 17:50:41  drsung
 * Fix for Win2k DHCP server applied. Thanks to Damian Slee
 *
 * Revision 1.7  2003/11/03 16:22:59  haraldkipp
 * Renewal disabled if lease time is set to zero
 *
 * Revision 1.6  2003/10/13 10:18:08  haraldkipp
 * Using new seconds counter
 *
 * Revision 1.5  2003/08/07 09:09:08  haraldkipp
 * Redesign to follow RFC 2131 more closely.
 *
 * Revision 1.4  2003/07/20 18:25:40  haraldkipp
 * Support secondary DNS.
 *
 * Revision 1.3  2003/07/17 09:44:14  haraldkipp
 * Removed default MAC address and let the caller have a second chance.
 *
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

#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/confnet.h>

#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/route.h>
#include <sys/socket.h>
#include <pro/dhcp.h>

#ifdef NUTDEBUG
#include <net/netdebug.h>
#endif

/*!
 * \addtogroup xgDHCPC
 */
/*@{*/

#define MIN_DHCP_MSGSIZE    300
#define MAX_DHCP_WAIT       5000
#define MAX_OFFERS          5
#define MAX_DCHP_RETRIES    3

#define DHCPST_SELECTING    0
#define DHCPST_REQUESTING   1
#define DHCPST_REBOOTING    2
#define DHCPST_BOUND        3
#define DHCPST_RENEWING     4
#define DHCPST_REBINDING    5
#define DHCPST_ERROR        6

static u_char dhcpState;

typedef struct dyn_cfg DYNCFG;

struct dyn_cfg {
    u_char dyn_msgtyp;          /*!< \brief DHCP message type */
    u_long dyn_yiaddr;          /*!< \brief Offered IP address. */
    u_long dyn_netmask;         /*!< \brief Local IP netmask. */
    u_long dyn_broadcast;       /*!< \brief Local IP broadcast address. */
    u_long dyn_gateway;         /*!< \brief Default gate IP address. */
    u_long dyn_pdns;            /*!< \brief Primary DNS IP address. */
    u_long dyn_sdns;            /*!< \brief Secondary DNS IP address. */
    u_long dyn_sid;             /*!< \brief Server identifier. */
    u_long dyn_leaseTime;       /*!< \brief Offered lease time in seconds. */
    u_char *dyn_hostname;       /*!< \brief Local hostname. */
    u_char *dyn_domain;         /*!< \brief Name of local domain. */
};

static HANDLE dhcpDone;

/* Magic cookie according to RFC 1497. */
static u_char cookie[4] = { 0x63, 0x82, 0x53, 0x63 };

static void copy_str(u_char ** dst, void *src, int len)
{
    if (*dst) {
        free(*dst);
    }
    *dst = malloc(len + 1);
    if (len)
        memcpy(*dst, src, len);
    *(*dst + len) = 0;
}

/*!
 * \brief Parse a DHCP reply telegram.
 *
 * \param cfgp Pointer to config structure.
 * \param ip   DHCP server IP address.
 * \param bp   Pointer to the reply telegram.
 * \param len  Number of valid bytes in the reply telegram.
 *
 * \return 0 on success, -1 otherwise.
 */
int ParseReply(DYNCFG * cfgp, u_long ip, struct bootp *bp, int len)
{
    u_char *op;
    u_long lval;
    int left;
    int ol;
    int rc = -1;

    memset(cfgp, 0, sizeof(*cfgp));

    /* Reject anything which is not a reply. */
    if (bp->bp_op != 2)
        return -1;

    memcpy(&cfgp->dyn_yiaddr, &bp->bp_yiaddr, 4);

    op = bp->bp_options + 4;
    left = len - (sizeof(*bp) - sizeof(bp->bp_options)) - 4;
    while (left > 0) {
        /* End marker. */
        if (*op == DHCPOPT_END)
            break;

        /* Pad option is used for boundary alignment. */
        if (*op == DHCPOPT_PAD) {
            op++;
            left--;
            continue;
        }

        /* Reject if option length exceeds total length. */
        if ((ol = *(op + 1)) > left) {
            rc = -1;
            break;
        }

        /* Preset most often used long value. */
        if (ol == 4)
            memcpy(&lval, op + 2, 4);

        switch (*op) {
        case DHCPOPT_MSGTYPE:
            if (ol == 1) {
                cfgp->dyn_msgtyp = *(op + 2);
                rc = 0;
            } else
                rc = -1;
            break;
        case DHCPOPT_NETMASK:
            if (ol == 4)
                cfgp->dyn_netmask = lval;
            else
                rc = -1;
            break;
        case DHCPOPT_BROADCAST:
            if (ol >= 4)
                cfgp->dyn_broadcast = lval;
            else
                rc = -1;
            break;
        case DHCPOPT_GATEWAY:
            /* More than one gateway may be specified. We take the
               fist one only and ignore the rest. */
            if (ol >= 4)
                cfgp->dyn_gateway = lval;
            else
                rc = -1;
            break;
        case DHCPOPT_HOSTNAME:
            /* May or may not include the domain. */
            copy_str(&cfgp->dyn_hostname, op + 2, ol);
            break;
        case DHCPOPT_DOMAIN:
            copy_str(&cfgp->dyn_domain, op + 2, ol);
            break;
        case DHCPOPT_DNS:
            /* Update by Jelle Martijn Kok to support a secondary DNS. */
            if (ol >= 4)
                cfgp->dyn_pdns = lval;
            if (ol >= 8)
                cfgp->dyn_sdns = *((u_long *) (op + 6));
            if (ol < 4)
                rc = -1;
            break;
        case DHCPOPT_SID:
            if (ol >= 4)
                cfgp->dyn_sid = lval;
            else
                rc = -1;
            break;
        case DHCPOPT_LEASETIME:
            /* Only lease time is recognized. We ignore rebinding or 
               renewal time options. */
            if (ol == 4)
                cfgp->dyn_leaseTime = ntohl(lval);
            else
                rc = -1;
            break;
        }
        op += ol + 2;
        left -= ol + 2;
        if (rc)
            break;
    }
    return rc;
}

/*!
 * \brief Add single octet option.
 *
 * \param op Pointer into the option buffer.
 * \param ot Option type.
 * \param ov Option value in network byte order.
 *
 * \return The number of option bytes added.
 */
static INLINE size_t DhcpAddByteOption(u_char * op, u_char ot, u_char ov)
{
    *op++ = ot;
    *op++ = 1;
    *op++ = ov;

    return 3;
}

/*!
 * \brief Add double octet option.
 *
 * \param op Pointer into the option buffer.
 * \param ot Option type.
 * \param ov Option value in network byte order.
 *
 * \return The number of option bytes added.
 */
static INLINE size_t DhcpAddShortOption(u_char * op, u_char ot, u_short ov)
{
    *op++ = ot;
    *op++ = 2;
    memcpy(op, &ov, 2);

    return 4;
}

/*!
 * \brief Add quad octet option.
 *
 * \param op Pointer into the option buffer.
 * \param ot Option type.
 * \param ov Option value in network byte order.
 *
 * \return The number of option bytes added.
 */
static INLINE size_t DhcpAddLongOption(u_char * op, u_char ot, u_long ov)
{
    *op++ = ot;
    *op++ = 4;
    memcpy(op, &ov, 4);

    return 6;
}

/*!
 * \brief Add variable octet option.
 *
 * \param op Pointer into the option buffer.
 * \param ot Option type.
 * \param ov Pointer to buffer to copy from.
 * \param len Length of octets to copy across
 *
 * \return The number of option bytes added.
 */
static INLINE size_t DhcpAddOptionFromRAM(u_char * op, u_char ot, u_char * ov, u_char len)
{
    *op++ = ot;
    *op++ = len;
    memcpy(op, ov, len);

    return 2 + len;
}

/*!
 * \brief Prepare bootp telegram header.
 *
 * \param xid Random transaction identifier.
 *
 * \return The number of option bytes added.
 */
static u_int DhcpPrepHeader(struct bootp *bp, u_char msgtyp, u_long xid, u_long caddr, u_short secs)
{
    u_char *op;

    memset(bp, 0, sizeof(*bp));
    /* Clients send bootp requests only. */
    bp->bp_op = 1;
    /* Ethernet address. */
    bp->bp_htype = 1;
    bp->bp_hlen = 6;
    memcpy(bp->bp_chaddr, confnet.cdn_mac, 6);
    /* Transaction identifier. */
    bp->bp_xid = xid;
    /* Seconds elapsed since address acquisition. */
    bp->bp_secs = htons(secs);

#if 0
    /*
     * If no local address has been assigned, our stack accepts IP telegrams 
     * to any destination. We do not need the broadcast flag anymore.
     */
    /*
     * According to RFC 2131 we need to set the broadcast flag.
     * Many thanks to Andre Albsmeier, who found this problem
     * while using ISC-DHCP 3.
     */
    bp->bp_flags = htons(0x8000);
#endif

    bp->bp_ciaddr = caddr;

    op = bp->bp_options;
    memcpy(op, cookie, 4);

    return DhcpAddByteOption(op + 4, DHCPOPT_MSGTYPE, msgtyp) + 4;
}

/*!
 * \brief Send a DHCP telegram to the server.
 *
 * This routine will add an end of option identifier and take care,
 * that the message length will not fall below the minimum expected
 * by BOOTP.
 *
 * \param sock  Socket descriptor. This pointer must have been 
 *              retrieved by calling NutUdpCreateSocket().
 * \param bp    Pointer to a buffer to be used for transmission.
 *              No specific initialization required.
 * \param len   Total length of DHCP options.
 *
 * \return 0 on success, -1 otherwise.
 */
static int DhcpSendMessage(UDPSOCKET * sock, u_long addr, struct bootp *bp, size_t len)
{
    /* End of options. */
    bp->bp_options[len++] = DHCPOPT_END;

    /* Maintain a BOOTP compatible minimum packet size of 300 octets. 
       Thanks to Tomohiro Haraikawa. */
    if ((len += sizeof(struct bootp) - sizeof(bp->bp_options)) < MIN_DHCP_MSGSIZE)
        len = 300;

    if (NutUdpSendTo(sock, addr, DHCP_SERVERPORT, bp, len) < 0) {
        NutSleep(MAX_DHCP_WAIT);
        return -1;
    }
    return 0;
}

/*!
 * \brief Receive a DHCP telegram from the server.
 *
 * \param sock Socket descriptor. 
 * \param xid  Expected transaction identifier.
 * \param addr IP address of the remote host in network byte order.
 * \param bp   Pointer to receive buffer.
 *
 * \return The number of bytes received, if successful. The return
 *         value -1 indicates an error. On timeout 0 is returned.
 */
static int DhcpRecvMessage(UDPSOCKET * sock, u_long xid, u_long * addr, struct bootp *bp)
{
    int rc;
    u_short port;

    for (;;) {
        rc = NutUdpReceiveFrom(sock, addr, &port, bp, sizeof(struct bootp), MAX_DHCP_WAIT);
        if (rc > 0) {
            /* Ignore messages with different ID. */
            if (bp->bp_xid == xid) {
                break;
            }
        } else {
            if (rc < 0) {
                /* Receive error. */
                NutSleep(MAX_DHCP_WAIT);
            }
            break;
        }
    }
    return rc;
}

/*!
 * \brief Broadcast a DHCP discover telegram.
 *
 * \param sock Socket descriptor. This pointer must have been retrieved 
 *             by calling NutUdpCreateSocket().
 * \param bp   Pointer to a buffer to be used for transmission. No specific 
 *             initialization required.
 * \param xid  Random transaction identifier.
 * \param secs Seconds elapsed since start of address acquisition. Related
 *             requests must use the same value.
 *
 * \return 0 on success, -1 if send failed.
 */
static int DhcpDiscover(UDPSOCKET * sock, struct bootp *bp, u_long xid, u_short secs)
{
    size_t optlen;
    u_char *op = bp->bp_options;
    u_char reqOpts[3];

    optlen = DhcpPrepHeader(bp, DHCP_DISCOVER, xid, 0, secs);

    /* Request a specific IP if one had been assigned previously. */
    if ((confnet.cdn_ip_addr & confnet.cdn_ip_mask) != 0) {
        optlen += DhcpAddLongOption(op + optlen, DHCPOPT_REQESTIP, confnet.cdn_ip_addr);
    }

    /* request gateway and DNS be returned */
    reqOpts[0] = DHCPOPT_NETMASK;       /* Win2k DHCP Server responds with this anyway. But Ethereal, showed it in a Win2k DHCP client request */
    reqOpts[1] = DHCPOPT_GATEWAY;       /* Win2k DHCP won't respond with gateway, unless we ask */
    reqOpts[2] = DHCPOPT_DNS;   /* Win2k DHCP won't respond with DNS, unless we ask */
    optlen += DhcpAddOptionFromRAM(op + optlen, DHCPOPT_PARAMREQUEST, reqOpts, 3);

    /* Request a maximum message size. */
    optlen += DhcpAddShortOption(op + optlen, DHCPOPT_MAXMSGSIZE, htons(576));

    return DhcpSendMessage(sock, INADDR_BROADCAST, bp, optlen);
}


/*!
 * \brief Request a configuration.
 *
 * \param sock  Socket descriptor. This pointer must have been retrieved by 
 *              calling NutUdpCreateSocket().
 * \param daddr IP address of the DHCP server.
 * \param bp    Pointer to a buffer to be used for transmission.
 * \param xid   Random transaction identifier.
 * \param caddr Our IP address. Should be set only if we are able to respond 
 *              to ARP requests. Otherwise must be set to 0.
 * \param raddr Requested IP address. Should be used only if we are able to 
 *              respond to ARP requests. Otherwise must be set to 0.
 * \param sid   Server identifier. If this request is not an offer response, 
 *              then set it to zero.
 * \param secs  Seconds elapsed since start of address acquisition. If this
 *              request is sent in reponse to an offer, the same value must
 *              be used.
 *
 * \return 0 on success, -1 if send failed.
 */
static int DhcpRequest(UDPSOCKET * sock, u_long daddr, struct bootp *bp, u_long xid, u_long caddr, u_long raddr, u_long sid,
                       u_short secs)
{
    size_t optlen;
    u_char *op = bp->bp_options;
    u_char reqOpts[3];

    optlen = DhcpPrepHeader(bp, DHCP_REQUEST, xid, caddr, secs);

    /* Add specified options. */
    if (raddr) {
        optlen += DhcpAddLongOption(op + optlen, DHCPOPT_REQESTIP, raddr);
    }
    if (sid) {
        optlen += DhcpAddLongOption(op + optlen, DHCPOPT_SID, sid);
    }

    /* request gateway and DNS be returned */
    reqOpts[0] = DHCPOPT_NETMASK;       /* Win2k DHCP Server responds with this anyway. But Ethereal, showed it in a Win2k DHCP client request */
    reqOpts[1] = DHCPOPT_GATEWAY;       /* Win2k DHCP won't respond with gateway, unless we ask */
    reqOpts[2] = DHCPOPT_DNS;   /* Win2k DHCP won't respond with DNS, unless we ask */
    optlen += DhcpAddOptionFromRAM(op + optlen, DHCPOPT_PARAMREQUEST, reqOpts, 3);

    return DhcpSendMessage(sock, daddr, bp, optlen);
}

/*!
 * \brief Release DYNCFG structure.
 */
static void ReleaseDynCfg(DYNCFG * dyncfg)
{
    if (dyncfg) {
        if (dyncfg->dyn_hostname)
            free(dyncfg->dyn_hostname);
        if (dyncfg->dyn_domain)
            free(dyncfg->dyn_domain);
        free(dyncfg);
    }
}

/*!
 * \brief Add a valid offer to our list.
 *
 * \param offer   Array of offered configurations.
 * \param noffers Number of offers.
 * \param ip      DHCP server IP address.
 * \param bp      DHCP message.
 * \param len     DHCP message length.
 *
 * \return Updated number of offers.
 */
static u_char AddOffer(DYNCFG ** offer, u_char noffers, u_long ip, struct bootp *bp, size_t len)
{
    u_char i;

    /* Avoid table overrun. */
    if (noffers >= MAX_OFFERS) {
        return noffers;
    }

    /* Avoid duplicate entries. */
    for (i = 0; i < noffers; i++) {
        if (offer[i]->dyn_sid == ip) {
            return noffers;
        }
    }

    /* Create a new entry. */
    offer[noffers] = malloc(sizeof(DYNCFG));
    if (ParseReply(offer[noffers], ip, bp, len) || offer[noffers]->dyn_msgtyp != DHCP_OFFER) {
        /* Silently discard malformed messages and anything which isn't an offer. */
        ReleaseDynCfg(offer[noffers]);
    } else {
        noffers++;
    }
    return noffers;
}

/*!
 * \brief Select an offer.
 *
 * Moves the selected offer to the request configuration and releases
 * memory of all unused offers.
 *
 * \param offer   Array of offered configurations.
 * \param noffers Number of offers.
 *
 */
static DYNCFG *SelectOffer(DYNCFG ** offer, u_char noffers)
{
    DYNCFG *rc = 0;
    u_char i = 0;

    /* 
     * Check if any offer matches our previous configuration. 
     */
    if (confnet.cdn_ip_addr & confnet.cdn_ip_mask) {
        for (i = noffers; --i;) {
            if (offer[i]->dyn_yiaddr == confnet.cdn_ip_addr) {
                break;
            }
        }
    }

    /* Release unused offers. */
    while (noffers--) {
        if (noffers == i)
            rc = offer[noffers];
        else {
            ReleaseDynCfg(offer[noffers]);
        }
    }
    return rc;
}

/*! \fn NutDhcpClient(void *arg)
 * \brief DHCP client thread.
 *
 * This thread implements a DHCP state machine.
 *
 * \bug We are not able to shutdown our interface, which may cause problems
 * if out original DHCP server dies.
 */
THREAD(NutDhcpClient, arg)
{
    DYNCFG *dyncfg = 0;
    UDPSOCKET *sock;
    int n;
    struct bootp *bp;
    u_long server_ip;
    u_long xid;
    IFNET *nif = ((NUTDEVICE *) arg)->dev_icb;
    u_long secs = 0;
    u_char retries = 0;
    u_char noffers = 0;
    DYNCFG *offer[MAX_OFFERS];
    u_char configured = 0;

    while ((sock = NutUdpCreateSocket(DHCP_CLIENTPORT)) == 0)
        NutSleep(10000);

    bp = malloc(sizeof(struct bootp));

    /* Determine wether this is an initial boot or a reboot. */
    if ((confnet.cdn_ip_addr & confnet.cdn_ip_mask) != 0) {
        dhcpState = DHCPST_REBOOTING;
    } else {
        dhcpState = DHCPST_SELECTING;
    }

    /* Generate a random transaction identifier. */
    memcpy(&xid, &nif->if_mac[2], sizeof(u_long));
    xid += NutGetTickCount();

    for (;;) {

        /*
         * Maintain lease time.
         */
        if (dhcpState == DHCPST_BOUND) {
#ifdef NUTDEBUG
            if (__tcp_trf)
                fprintf(__tcp_trs, "[BOUND %lu]", NutGetSeconds() - secs);
#endif
            if (dyncfg->dyn_leaseTime && NutGetSeconds() - secs > dyncfg->dyn_leaseTime / 2UL) {
                /* Lease time elapsed. */
                while ((sock = NutUdpCreateSocket(DHCP_CLIENTPORT)) == 0)
                    NutSleep(1000);
                dhcpState = DHCPST_RENEWING;
                retries = 0;
                xid += NutGetTickCount();
            } else {
                /* We do not need the socket until our lease time expires.
                   Release its allocated memory. */
                if (sock) {
                    NutUdpDestroySocket(sock);
                    sock = 0;
                }
                NutSleep(10000);
            }
        }

        /*
         * Waiting for an acknowledge of our renewal request.
         */
        else if (dhcpState == DHCPST_RENEWING) {
#ifdef NUTDEBUG
            if (__tcp_trf)
                fprintf(__tcp_trs, "[RENEWING]");
#endif
            if (retries++ >= MAX_DCHP_RETRIES) {
                /* This is not following RFC 2131, because we do not maintain a second 
                   timer but a fixed number of retries. And we will never disable our
                   interface. */
                retries = 0;
                xid += NutGetTickCount();
                dhcpState = DHCPST_REBINDING;
            }
            /* Send a request to our leasing server. */
            else if (DhcpRequest(sock, dyncfg->dyn_sid, bp, xid, confnet.cdn_ip_addr, confnet.cdn_ip_addr, 0, (u_short) secs) <
                     0) {
                /* Fatal transmit error. */
                dhcpState = DHCPST_ERROR;
            } else if ((n = DhcpRecvMessage(sock, xid, &server_ip, bp)) < 0) {
                /* Fatal receive error. */
                dhcpState = DHCPST_ERROR;
            } else if (n > 0 && ParseReply(dyncfg, server_ip, bp, n) == 0) {
                if (dyncfg->dyn_msgtyp == DHCP_ACK) {
                    /* Got an acknowledge, return to bound state. */
                    secs = NutGetSeconds();
                    dhcpState = DHCPST_BOUND;
                } else if (dyncfg->dyn_msgtyp == DHCP_NAK) {
                    /* Unexpected NAK. */
                    retries = 0;
                    xid += NutGetTickCount();
                    dhcpState = DHCPST_REBINDING;
                }
            }
        }

        /*
         * Waiting for an acknowledge of our rebind request.
         */
        else if (dhcpState == DHCPST_REBINDING) {
#ifdef NUTDEBUG
            if (__tcp_trf)
                fprintf(__tcp_trs, "[REBINDING]");
#endif
            if (retries++ >= MAX_DCHP_RETRIES) {
                /* This is not following RFC 2131, because we do not maintain a second 
                   timer but a fixed number of retries. And we will never disable our
                   interface. */
                xid += NutGetTickCount();
                dhcpState = DHCPST_SELECTING;
            }
            /* Broadcast a request for our previous configuration. */
            else if (DhcpRequest(sock, INADDR_BROADCAST, bp, xid, confnet.cdn_ip_addr, confnet.cdn_ip_addr, 0, (u_short) secs) <
                     0) {
                /* Fatal transmit error. */
                dhcpState = DHCPST_ERROR;
            } else if ((n = DhcpRecvMessage(sock, xid, &server_ip, bp)) < 0) {
                /* Fatal receive error. */
                dhcpState = DHCPST_ERROR;
            } else if (n > 0 && ParseReply(dyncfg, server_ip, bp, n) == 0) {
                if (dyncfg->dyn_msgtyp == DHCP_ACK) {
                    /* Got an acknowledge, return to bound state. */
                    secs = NutGetSeconds();
                    dhcpState = DHCPST_BOUND;
                } else if (dyncfg->dyn_msgtyp == DHCP_NAK) {
                    /*
                     * We have a problem here if the last DHCP server died. 
                     * If a backup server exists, it may probe our IP address
                     * using ARP or ICMP. Our interface is up and responding,
                     * so the backup server may think that the IP address
                     * is in use and respond with NAK. Without shutting
                     * down our interface (not yet implemented) we are stuck.
                     * We switch to discovery state, but the problem remains.
                     */
                    secs = NutGetSeconds();
                    xid += NutGetTickCount();
                    dhcpState = DHCPST_SELECTING;
                }
            }
        }

        /*
         * Check if something else configured our interface.
         */
        else if (!configured && nif->if_local_ip) {
            /* If we need additional configuration, we can sent
               a DHCP Inform message here. */
            NutSleep(10000);
        }

        /*
         * Broadcast discover and collect incoming offers.
         */
        else if (dhcpState == DHCPST_SELECTING) {
#ifdef NUTDEBUG
            if (__tcp_trf)
                fprintf(__tcp_trs, "[SELECTING]");
#endif
            ReleaseDynCfg(dyncfg);
            dyncfg = 0;
            /* Broadcast a discover telegram. No retry count, continue until success. */
            if (DhcpDiscover(sock, bp, xid, (u_short) secs) < 0) {
                /* Fatal transmit error. */
                dhcpState = DHCPST_ERROR;
            } else {
                /* Response collection loop. */
                for (;;) {
                    if ((n = DhcpRecvMessage(sock, xid, &server_ip, bp)) < 0) {
                        /* Fatal receive error. */
                        dhcpState = DHCPST_ERROR;
                        break;
                    } else if (n > 0) {
                        /* Add the offer received, if it's unique and valid. */
                        noffers = AddOffer(offer, noffers, server_ip, bp, n);
                    } else {
                        /* Receive timeout. If we got offers, select one. */
                        if (noffers) {
                            dyncfg = SelectOffer(offer, noffers);
                            noffers = 0;
                            /* Start requesting the selected offer. */
                            retries = 0;
                            dhcpState = DHCPST_REQUESTING;
                        }
                        break;
                    }
                }
            }
        }

        /*
         * Send request and wait for an acknowledge.
         */
        else if (dhcpState == DHCPST_REQUESTING) {
#ifdef NUTDEBUG
            if (__tcp_trf)
                fprintf(__tcp_trs, "[REQUESTING]");
#endif
            if (retries++ >= MAX_DCHP_RETRIES) {
                /* Too many retries with this server, fall back to discovery. */
                xid += NutGetTickCount();
                dhcpState = DHCPST_SELECTING;
            }
            /* Request an offered configuration. */
            else if (DhcpRequest(sock, INADDR_BROADCAST, bp, xid, 0, dyncfg->dyn_yiaddr, dyncfg->dyn_sid, (u_short) secs) < 0) {
                /* Fatal transmit error. */
                dhcpState = DHCPST_ERROR;
            } else if ((n = DhcpRecvMessage(sock, xid, &server_ip, bp)) < 0) {
                /* Fatal receive error. */
                dhcpState = DHCPST_ERROR;
            } else if (n > 0) {
                dyncfg = malloc(sizeof(DYNCFG));
                if (ParseReply(dyncfg, server_ip, bp, n) == 0) {
                    if (dyncfg->dyn_msgtyp == DHCP_ACK) {
                        /* Hack. Shutdown of the interface isn't available,
                           so we can't reconfigure it. */
                        if (configured) {
                            dhcpState = DHCPST_BOUND;
                        }
                        /* Got a final acknowledge, configure the interface. */
                        else if (NutNetIfSetup(arg, dyncfg->dyn_yiaddr, dyncfg->dyn_netmask, dyncfg->dyn_gateway) == 0) {
                            configured = 1;
                            NutDnsConfig2(0, 0, dyncfg->dyn_pdns, dyncfg->dyn_sdns);
                            dhcpState = DHCPST_BOUND;
                            NutEventBroadcast((HANDLE *) & dhcpDone);
                        }
                    } else if (dyncfg->dyn_msgtyp == DHCP_NAK) {
                        /* Unexpected NAK, fall back to discovery. */
                        xid += NutGetTickCount();
                        dhcpState = DHCPST_SELECTING;
                    }
                }
            }
        }

        /*
         * Waiting for a response after reboot.
         */
        else if (dhcpState == DHCPST_REBOOTING) {
#ifdef NUTDEBUG
            if (__tcp_trf)
                fprintf(__tcp_trs, "[REBOOTING]");
#endif
            if (retries++ >= MAX_DCHP_RETRIES) {
                /* Too many retries, fall back to discovery. */
                xid += NutGetTickCount();
                dhcpState = DHCPST_SELECTING;
            }
            /* Broadcast a request for our previous configuration. */
            else if (DhcpRequest(sock, INADDR_BROADCAST, bp, xid, 0, confnet.cdn_ip_addr, 0, (u_short) secs) < 0) {
                /* Fatal transmit error. */
                dhcpState = DHCPST_ERROR;
            } else if ((n = DhcpRecvMessage(sock, xid, &server_ip, bp)) < 0) {
                /* Fatal receive error. */
                dhcpState = DHCPST_ERROR;
            } else if (n > 0) {
                dyncfg = malloc(sizeof(DYNCFG));
                if (ParseReply(dyncfg, server_ip, bp, n) == 0) {
                    if (dyncfg->dyn_msgtyp == DHCP_ACK) {
                        if (configured) {
                            dhcpState = DHCPST_BOUND;
                        }
                        /* Got an acknowledge, configure the interface. */
                        else if (NutNetIfSetup(arg, dyncfg->dyn_yiaddr, dyncfg->dyn_netmask, dyncfg->dyn_gateway) == 0) {
                            configured = 1;
                            NutDnsConfig2(0, 0, dyncfg->dyn_pdns, dyncfg->dyn_sdns);
                            dhcpState = DHCPST_BOUND;
                            NutEventBroadcast((HANDLE *) & dhcpDone);
                        }
                    } else if (dyncfg->dyn_msgtyp == DHCP_NAK) {
                        /* The previous server doesn't like us any more. Try discover. */
                        xid += NutGetTickCount();
                        dhcpState = DHCPST_SELECTING;
                    }
                }
            }
        }


        /*
         * Something miserable happened to us. Release all resources
         * and keep as quiet as possible.
         */
        else if (dhcpState == DHCPST_ERROR) {
#ifdef NUTDEBUG
            if (__tcp_trf)
                fprintf(__tcp_trs, "[DHCPERROR]");
#endif
            if (sock) {
                NutUdpDestroySocket(sock);
                sock = 0;
            }
            while (noffers) {
                ReleaseDynCfg(offer[noffers]);
                noffers--;
            }
            ReleaseDynCfg(dyncfg);
            dyncfg = 0;
            NutSleep(1000);
        }
    }
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
 * started and this routine will wait upto a given number of 
 * milliseconds for a response from a DHCP server.
 *
 * \param name    Name of the registered Ethernet device.
 * \param mac     MAC address of the destination. Set null to use 
 *                the configuration stored in the EEPROM.
 * \param timeout Maximum number of milliseconds to wait. To disable 
 *                timeout, set this parameter to NUT_WAIT_INFINITE.
 *
 * \return 0 on success or -1, if no IP configuration could have
 *         been received within the specified timeout.
 *
 * \bug The client doesn't send update information to the server. To 
 *      avoid loosing the assignment while Ethernut is down, addresses 
 *      can be fixed in the DHCP server tables.
 */
int NutDhcpIfConfig(CONST char *name, u_char * mac, u_long timeout)
{
    static HANDLE thread;
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
     * If the caller specified a MAC address, we use it and
     * overwrite the configuration.
     */
    if (mac)
        memcpy(confnet.cdn_mac, mac, sizeof(confnet.cdn_mac));

    /*
     * If no MAC address has been specified, read the configuration 
     * from EEPROM. If this fails, we do not continue any further,
     * but let the caller know that something is wrong. He may call
     * us again with a valid MAC address.
     */
    else if (NutNetLoadConfig(name))
        return -1;

    /*
     * Copy the MAC address to the interface structure. This will
     * magically enable the interface. Well, actually this is a
     * bad hack.
     */
    memcpy(nif->if_mac, confnet.cdn_mac, 6);
    NutSleep(500);

    /*
     * If we got an invalid IP configuration, then start the
     * DHCP client and wait the given number of milliseconds
     * for a response from the DHCP server.
     */
    if ((confnet.cdn_cip_addr & confnet.cdn_ip_mask) == 0) {
        if (thread == 0)
            thread = NutThreadCreate("dhcpc", NutDhcpClient, dev, 512);
        NutEventWait((HANDLE *) & dhcpDone, timeout);
        if ((confnet.cdn_ip_addr & confnet.cdn_ip_mask) == 0)
            return -1;
    } else
        confnet.cdn_ip_addr = confnet.cdn_cip_addr;

    /*
     * We got a valid IP configuration. Configure the interface.
     */
    return NutNetIfConfig2(name, confnet.cdn_mac, confnet.cdn_ip_addr, confnet.cdn_ip_mask, confnet.cdn_gateway);
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
