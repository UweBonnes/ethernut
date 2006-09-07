#ifndef _PRO_DISCOVER_H
#define _PRO_DISCOVER_H

/*
 * Copyright (C) 2001-2006 by egnite Software GmbH. All rights reserved.
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
 */

/*!
 * \file pro/discover.h
 * \brief Responds to UDP discovery broadcasts.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/09/07 09:06:17  haraldkipp
 * Discovery service added.
 *
 *
 * \endverbatim
 */


#include <sys/types.h>

#define DISCOVERY_VERSION   0x10

#define DIST_REQUEST    0
#define DIST_ANNOUNCE   1
#define DIST_APPLY      2

#define DISF_INITAL_ANN     0x0001

typedef struct __attribute__ ((packed)) _DISCOVERY_TELE {
    u_long dist_xid;            /*!< \brief Exchange identifier. */
    u_char dist_type;           /*!< \brief Message type, 0=request, 1=announce, 2=apply. */
    u_char dist_ver;            /*!< \brief Telegram version. */
    u_char dist_mac[6];         /*!< \brief Ethernet MAC address. */
    u_long dist_ip_addr;        /*!< \brief Last used IP address. */
    u_long dist_ip_mask;        /*!< \brief IP netmask. */
    u_long dist_gateway;        /*!< \brief Default route. */
    u_long dist_cip_addr;       /*!< \brief Configured IP address. */
    u_char dist_hostname[8];    /*!< \brief Host name of the system. */
    u_char dist_custom[92];     /*!< \brief Bootfile to request. */
} DISCOVERY_TELE;

typedef int (*NutDiscoveryCallback) (u_long, u_short, DISCOVERY_TELE *, int);

extern int NutRegisterDiscovery(u_long ipmask, u_short port, u_int flags);
extern NutDiscoveryCallback NutRegisterDiscoveryCallback(NutDiscoveryCallback func);

extern int NutDiscoveryAnnTele(DISCOVERY_TELE *dist);
extern int NutDiscoveryAppConf(DISCOVERY_TELE *dist);

#endif
