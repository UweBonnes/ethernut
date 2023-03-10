/*
 * Copyright (C) 2007 by egnite Software GmbH
 * Copyright (c) 1992, 1993 The Regents of the University of California
 * Copyright (c) 1988 Stephen Deering
 *
 * This code is derived from software contributed to Berkeley by
 * Stephen Deering of Stanford University.
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
 */

/*!
 * \file net/igmpin.c
 * \brief IGMP input functions.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <string.h>
#include <net/if_var.h>
#include <netinet/ipcsum.h>
#include <netinet/igmp.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>

/*!
 * \addtogroup xgIGMP
 */
/*@{*/

/*!
 * \brief Process incoming IGMP packets.
 *
 * \param dev Identifies the device that received the packet.
 * \param nb  Pointer to a network buffer structure containing the IGMP packet.
 */
void NutIgmpInput(NUTDEVICE * dev, NETBUF * nb)
{
    IGMP *igmp = (IGMP *) nb->nb_tp.vp;
    IFNET *nif;
    MCASTENTRY *mca;

    nif = dev->dev_icb;

    /*
     * Silently discard packets, which are too small.
     */
    if (igmp == NULL || nb->nb_tp.sz < IGMP_MINLEN) {
        NutNetBufFree(nb);
        return;
    }

    switch (igmp->igmp_type) {
    case IGMP_MEMBERSHIP_QUERY:
        /* Clear the received buffer first. */
        NutNetBufFree(nb);

        /* Go through the list and send reports */
        for (mca = nif->if_mcast; mca; mca = mca->mca_next) {
            /* Do not send for 224.0.0.1 */
            if (mca->mca_ip != INADDR_ALLHOSTS_GROUP) {
                NutIgmpJoinGroup(dev, mca->mca_ip);
            }
        }
        break;
    case IGMP_V1_MEMBERSHIP_REPORT:
    case IGMP_V2_MEMBERSHIP_REPORT:
        /* Not supported in the moment */
        NutNetBufFree(nb);
        break;
    default:
        /* Unrecognized message types are silently ignored. */
        NutNetBufFree(nb);
    }
}

/*@}*/
