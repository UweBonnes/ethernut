#ifndef _NET_IF_VAR_H_
#define _NET_IF_VAR_H_

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
 * Revision 1.2  2003/08/05 20:19:13  haraldkipp
 * DNS removed from interface
 *
 * Revision 1.1.1.1  2003/05/09 14:41:12  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.10  2003/03/31 14:33:26  harald
 * Moved some declarations to ppp device
 *
 * Revision 1.9  2003/02/04 18:00:42  harald
 * Version 3 released
 *
 * Revision 1.8  2002/10/29 15:40:49  harald
 * *** empty log message ***
 *
 * Revision 1.7  2002/06/26 17:29:18  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/types.h>
#include <sys/device.h>
#include <dev/netbuf.h>
#include <net/if_types.h>

/*!
 * \file net/if_var.h
 * \brief Network interface structure.
 *
 * Each network device in Nut/Net has an associated
 * network interface structure.
 */

#ifdef __cplusplus
//extern "C" {
#endif

/*!
 * \addtogroup xgIP
 */
/*@{*/

#define ATF_COM     0x02        /*!< \brief Completed entry. */
#define ATF_PERM    0x04        /*!< \brief Permanent entry. */

/*!
 * \brief ARP entry type.
 */
typedef struct _ARPENTRY ARPENTRY;

/*!
 * \struct _ARPENTRY if_var.h net/if_var.h
 * \brief ARP entry structure.
 *
 * Each network interface maintains its own ARP table.
 */
struct _ARPENTRY {
    ARPENTRY *ae_next;          /*!< \brief Linked list chain. */
    u_long ae_ip;               /*!< \brief IP address. */
    u_char ae_ha[6];            /*!< \brief Hardware address. */
    u_char ae_flags;            /*!< \brief Status flags, permanent and completed. */
    u_char ae_outdated;         /*!< \brief Minutes since last use. */
    HANDLE ae_tq;               /*!< \brief Threads waiting for entry to be completed. */
};

/*!
 * \brief Network interface type.
 */
typedef struct ifnet IFNET;

/*!
 * \struct ifnet if_var.h net/if_var.h
 * \brief Network interface structure.
 *
 * Contains information about the network interface.
 */
struct ifnet {
    u_char if_type;             /*!< \brief Interface type.
                                 *  Either IFT_ETHER or IFT_PPP.
                                 */
    u_char if_mac[6];           /*!< \brief Hardware net address. */
    u_long if_local_ip;         /*!< \brief IP address. */
    u_long if_remote_ip;        /*!< \brief Remote IP address for point to point. */
    u_long if_mask;             /*!< \brief IP network mask. */
    u_short if_mtu;             /*!< \brief Maximum size of a transmission unit. */
    u_short if_pkt_id;          /*!< \brief Packet identifier. */
    ARPENTRY *arpTable;         /*!< \brief Linked list of arp entries. */
    void (*if_recv) (NUTDEVICE *, NETBUF *);			    /*!< \brief Receive routine. */
    int (*if_send) (NUTDEVICE *, NETBUF *);			    /*!< \brief Send routine. */
    int (*if_output) (NUTDEVICE *, u_short, u_char *, NETBUF *);    /*!< \brief Media output routine. */
};

typedef struct _PPP_PARAMS {
    int ppp_host_dev;
    u_char *ppp_user;
    u_char *ppp_pass;

} PPP_PARAMS;


extern int NutNetIfConfig(CONST char *name, void *mac_dev, u_long ip_addr,
                          u_long ip_mask);
extern int NutNetIfSetup(NUTDEVICE * dev, u_long ip_addr, u_long ip_mask,
                         u_long gateway);

extern int NutNetLoadConfig(CONST char *name);
extern int NutNetSaveConfig(void);
extern int NutNetAutoConfig(CONST char *name);

/*@}*/

#ifdef __cplusplus
//}
#endif

#endif
