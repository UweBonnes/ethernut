#ifndef _PRO_DHCP_H_
#define _PRO_DHCP_H_

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
 */

/*
 * $Log$
 * Revision 1.4  2004/02/25 16:24:13  haraldkipp
 * DHCP release added
 *
 * Revision 1.3  2004/01/14 17:51:05  drsung
 * Fix for Win2k DHCP server applied. Thanks to Damian Slee
 *
 * Revision 1.2  2003/08/05 20:20:36  haraldkipp
 * Comments added
 *
 * Revision 1.1.1.1  2003/05/09 14:41:18  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.8  2003/05/06 18:47:25  harald
 * New function
 *
 * Revision 1.7  2003/04/21 16:59:18  harald
 * Auto configuration moved
 *
 * Revision 1.6  2003/02/04 18:00:50  harald
 * Version 3 released
 *
 * Revision 1.5  2002/06/26 17:29:27  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/types.h>

/*!
 * \file pro/dhcp.h
 * \brief DHCP protocol definitions.
 */

#define DHCP_SERVERPORT      67
#define DHCP_CLIENTPORT      68

/* See RFC 2131. */
#define DHCPOPT_PAD          0
#define DHCPOPT_NETMASK      1
#define DHCPOPT_GATEWAY      3
#define DHCPOPT_DNS          6
#define DHCPOPT_HOSTNAME     12
#define DHCPOPT_DOMAIN       15
#define DHCPOPT_BROADCAST    28
#define DHCPOPT_REQESTIP     50
#define DHCPOPT_LEASETIME    51
#define DHCPOPT_MSGTYPE      53
#define DHCPOPT_SID          54
#define DHCPOPT_PARAMREQUEST 55
#define DHCPOPT_MAXMSGSIZE   57
#define DHCPOPT_RENEWALTIME  58
#define DHCPOPT_REBINDTIME   59
#define DHCPOPT_END          255

/*! \brief Client broadcast to locate available servers.
 */
#define DHCP_DISCOVER   1

/*! \brief Server to client in response to DHCP_DISCOVER.
 * Contains an offer of configuration parameters.
 */
#define DHCP_OFFER      2

/*! \brief Client message to servers.
 * Used for 
 * - requesting offered parameters from one server and implicitly declining offers from all others.
 * - confirming correctness of previously allocated address after, e.g., system reboot.
 * - extending the lease on a particular network address.
 */
#define DHCP_REQUEST    3

/*! \brief Client to server indicating network address is already in use.
 */
#define DHCP_DECLINE    4

/*! \brief Server to client with configuration parameters.
 * Contains committed network address.
 */
#define DHCP_ACK        5

/*! \brief Server to client indicating client's notion of network address is incorrect.
 * May be caused by the client's move to new subnet or by expiration of the client's lease.
 */
#define DHCP_NAK        6

/*! \brief Client to server relinquishing network address and cancelling remaining lease.
 */
#define DHCP_RELEASE    7

/*! \brief Client to server, asking only for local configuration parameters.
 * Used, if the client already has externally configured network address.
 */
#define DHCP_INFORM     8

/*!
 * \struct bootp dhcp.h pro/dhcp.h
 * \brief DHCP telegram structure.
 */
struct bootp {
    u_char  bp_op;              /*!< \brief Packet opcode type: 1=request, 2=reply */
    u_char  bp_htype;           /*!< \brief Hardware address type: 1=Ethernet */
    u_char  bp_hlen;            /*!< \brief Hardware address length: 6 for Ethernet */
    u_char  bp_hops;            /*!< \brief Gateway hops */
    u_long  bp_xid;             /*!< \brief Transaction ID */
    u_short bp_secs;            /*!< \brief Seconds since boot began */
    u_short bp_flags;           /*!< \brief RFC1532 broadcast, etc. */
    u_long  bp_ciaddr;          /*!< \brief Client IP address */
    u_long  bp_yiaddr;          /*!< \brief 'Your' IP address */
    u_long  bp_siaddr;          /*!< \brief Server IP address */
    u_long  bp_giaddr;          /*!< \brief Gateway IP address */
    u_char  bp_chaddr[16];      /*!< \brief Client hardware address */
    char    bp_sname[64];       /*!< \brief Server host name */
    char    bp_file[128];       /*!< \brief Boot file name */
    u_char  bp_options[312];    /*!< \brief Vendor-specific area */
};

__BEGIN_DECLS
/* Function prototypes. */
extern int NutDhcpIfConfig(CONST char *name, u_char *mac, u_long timeout);
extern int NutDhcpRelease(CONST char *name, u_long timeout);
extern int NutNetAutoConfig(CONST char *name);

__END_DECLS

#endif
