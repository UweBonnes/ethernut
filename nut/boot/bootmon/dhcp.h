#ifndef _DHCP_H
#define _DHCP_H

/*
 * Copyright (c) 1983, 1993 by The Regents of the University of California
 * Copyright (C) 2001-2007 by egnite Software GmbH
 * Copyright (C) 2010 by egnite GmbH
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
 * $Id$
 */

/*!
 * \addtogroup xgDhcp
 */
/*@{*/

#define DHCP_SERVERPORT     0x4300
#define DHCP_CLIENTPORT     0x4400

#define DHCPOPT_PAD         0
#define DHCPOPT_NETMASK     1
#define DHCPOPT_GATEWAY     3
#define DHCPOPT_DNS         6
#define DHCPOPT_HOSTNAME    12
#define DHCPOPT_DOMAIN      15
#define DHCPOPT_BROADCAST   28
#define DHCPOPT_REQUESTIP   50
#define DHCPOPT_LEASETIME   51
#define DHCPOPT_MSGTYPE     53
#define DHCPOPT_SID         54
#define DHCPOPT_RENEWALTIME 58
#define DHCPOPT_REBINDTIME  59
#define DHCPOPT_END         255

#define DHCP_DISCOVER   1
#define DHCP_OFFER      2
#define DHCP_REQUEST    3
#define DHCP_DECLINE    4
#define DHCP_ACK        5
#define DHCP_NAK        6
#define DHCP_RELEASE    7
#define DHCP_INFORM     8

/*
 * DHCP message structure according to RFC 2131.
 */
typedef struct __attribute__ ((packed)) bootp {
    unsigned char bp_op;        /*!< \brief Packet opcode type: 1=request, 2=reply */
    unsigned char bp_htype;     /*!< \brief Hardware address type: 1=Ethernet */
    unsigned char bp_hlen;      /*!< \brief Hardware address length: 6 for Ethernet */
    unsigned char bp_hops;      /*!< \brief Gateway hops */
    unsigned long bp_xid;       /*!< \brief Transaction ID */
    unsigned short bp_secs;     /*!< \brief Seconds since boot began */
    unsigned short bp_flags;    /*!< \brief RFC1532 broadcast, etc. */
    unsigned long bp_ciaddr;    /*!< \brief Client IP address */
    unsigned long bp_yiaddr;    /*!< \brief 'Your' IP address */
    unsigned long bp_siaddr;    /*!< \brief Server IP address */
    unsigned long bp_giaddr;    /*!< \brief Gateway IP address */
    unsigned char bp_chaddr[16];        /*!< \brief Client hardware address */
    char bp_sname[64];          /*!< \brief Server host name */
    char bp_file[128];          /*!< \brief Boot file name */
    unsigned long bp_cookie;    /*!< \brief Cookie */
    unsigned char bp_options[312];      /*!< \brief Vendor-specific area */
} BOOTPHDR;

/*@}*/

extern int DhcpQuery(void);

#endif
