#ifndef _NETINET_IF_ETHER_H_
#define _NETINET_IF_ETHER_H_

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
 */

/*
 * $Log$
 * Revision 1.5  2008/07/14 13:10:30  haraldkipp
 * Added macros to determine Ethernet address types.
 *
 * Revision 1.4  2006/08/01 07:41:01  haraldkipp
 * New functions ether_aton() and ether_ntoa() added. They convert the ASCII
 * representation of an Ethernet MAC address to its binary form and vice versa.
 *
 * Revision 1.3  2005/04/05 17:38:45  haraldkipp
 * ARM7 memory alignment bugs fixed.
 *
 * Revision 1.2  2004/03/16 16:48:28  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:13  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.6  2003/02/04 18:00:45  harald
 * Version 3 released
 *
 * Revision 1.5  2002/06/26 17:29:21  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/types.h>
#include <net/if_var.h>
#include <net/if_arp.h>

/*!
 * \file netinet/if_ether.h
 * \brief Ethernet interface definitions.
 *
 * A more elaborated file description.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \struct ether_header if_ether.h netinet/if_ether.h
 * \brief Ethernet protocol header.
 */
/*!
 * \typedef ETHERHDR
 * \brief Ethernet protocol header type.
 */
typedef struct __attribute__((packed)) ether_header {
    u_char  ether_dhost[6];     /*!< \brief Destination MAC address. */
    u_char  ether_shost[6];     /*!< \brief Source MAC address. */
    u_short ether_type;         /*!< \brief Protocol type. */
} ETHERHDR;

#define ETHERTYPE_IP    0x0800  /*!< \brief IP protocol */
#define ETHERTYPE_ARP   0x0806  /*!< \brief Address resolution protocol */

#define ETHERMTU    1500        /*!< \brief Ethernet maximum transfer unit. */
//#define ETHERMTU    576           /*!< \brief Ethernet maximum transfer unit. */
#define ETHERMIN    (60-14)     /*!< \brief Ethernet minimum transfer unit. */

/*! 
 * \brief Determine if a given Ethernet address is zero.
 *
 * \param ea Pointer to a character array containing the address.
 *
 * Return 1 if the address is zero. Otherwise 0 is returned.
 */
#define	ETHER_IS_ZERO(ea) (((ea)[0] | (ea)[1] | (ea)[2] | (ea)[3] | (ea)[4] | (ea)[5]) == 0)

/*! 
 * \brief Determine if a given Ethernet address is a broadcast address.
 *
 * \param ea Pointer to a character array containing the address.
 *
 * Return 1 if the address is a broadcast address. Otherwise 0 is returned.
 */
#define	ETHER_IS_BROADCAST(ea) (((ea)[0] & (ea)[1] & (ea)[2] & (ea)[3] & (ea)[4] & (ea)[5]) == 0xFF)

/*! 
 * \brief Determine if a given Ethernet address is a multicast address.
 *
 * The broadcast address is defined as a special multicast address.
 *
 * \param ea Pointer to a character array containing the address.
 *
 * Return 1 if the address is a multicast address. Otherwise 0 is returned.
 */
#define	ETHER_IS_MULTICAST(ea) ((ea)[0] & 1) 

/*! 
 * \brief Determine if a given Ethernet address is a unicast address.
 *
 * By definition, an address with all zeros is not a valid unicast address.
 *
 * \param ea Pointer to a character array containing the address.
 *
 * Return 1 if the address is a unicast address. Otherwise 0 is returned.
 */
#define	ETHER_IS_UNICAST(ea) (!ETHER_IS_ZERO(ea) && !ETHER_IS_MULTICAST(ea)) 

extern u_char *ether_aton(CONST char *str);
extern char *ether_ntoa(CONST u_char *mac);


/*!
 * \struct ether_arp if_ether.h netinet/if_ether.h
 * \brief Ethernet ARP protocol structure.
 *
 * See RFC 826 for protocol description.
 */
/*!
 * \typedef ETHERARP
 * \brief Ethernet ARP protocol type.
 */
typedef struct __attribute__((packed)) ether_arp {
    ARPHDR ea_hdr;     /*!< \brief Fixed-size header. */
    u_char arp_sha[6]; /*!< \brief Source hardware address. */
    u_long arp_spa;    /*!< \brief Source protocol address. */
    u_char arp_tha[6]; /*!< \brief Target hardware address. */
    u_long arp_tpa;    /*!< \brief Target protocol address. */
} ETHERARP;

extern void NutArpInput(NUTDEVICE *dev, NETBUF *nb);

extern NETBUF *NutArpAllocNetBuf(u_short type, u_long ip, u_char *mac);
extern int NutArpOutput(NUTDEVICE *dev, NETBUF *nb);

extern void NutArpCacheUpdate(NUTDEVICE *dev, u_long ip, u_char *ha);

extern int NutArpCacheQuery(NUTDEVICE *dev, u_long ip, u_char *mac);

#ifdef __cplusplus
}
#endif

#endif
