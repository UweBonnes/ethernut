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
 * Revision 1.1  2003/05/09 14:41:26  haraldkipp
 * Initial revision
 *
 * Revision 1.16  2003/02/04 18:14:56  harald
 * Version 3 released
 *
 * Revision 1.15  2002/06/26 17:29:35  harald
 * First pre-release with 2.4 stack
 *
 */

#include <string.h>

#include <sys/atom.h>
#include <sys/heap.h>
#include <sys/event.h>
#include <sys/thread.h>
#include <sys/timer.h>

#include <net/if_var.h>
#include <netinet/if_ether.h>
#include <net/ether.h>

static HANDLE arpExThread = 0;

/*!
 * \addtogroup xgARP
 */
/*@{*/

/*! \fn NutArpExpire(void *arg)
 * \brief ARP expiration thread.
 *
 * Running once a minute to handle expired 
 * ARP cache entries.
 */
THREAD(NutArpExpire, arg)
{
    ARPENTRY *volatile ae;
    ARPENTRY *volatile aer = 0;
    ARPENTRY **volatile aep;
    NUTDEVICE *dev;
    IFNET *ifn;

    for (;;) {
        NutSleep(60000);

        /*
         * Loop through the list of registered devices
         * and process all net devices.
         */
        for (dev = nutDeviceList; dev; dev = dev->dev_next) {
            if (dev->dev_type == IFTYP_NET) {
                ifn = dev->dev_icb;
                ae = ifn->arpTable;
                aep = &ifn->arpTable;

                /*
                 * Loop through the list of ARP entries and
                 * remove outdated ones.
                 */
                while (ae) {
                    if (ae->ae_outdated++ >= 9) {
                        *aep = ae->ae_next;
                        aer = ae;
                    }
                    if (aer == 0)
                        aep = &ae->ae_next;
                    ae = ae->ae_next;
                    if (aer) {
                        NutHeapFree(aer);
                        aer = 0;
                    }
                }
            }
        }
    }
}


/*!
 * \brief Locate ARP entry of a specified interface by a specified IP address.
 *
 * \note A read lock must have been set before calling this function.
 *
 * \param ifn Pointer to the interface.
 * \param ip  IP address.
 *
 * \return Pointer to the ARP table entry, if found, or NULL
 *         if no entry exists.
 */
static ARPENTRY *volatile NutArpCacheLookup(IFNET * ifn, u_long ip)
{
    ARPENTRY *volatile entry;

    for (entry = ifn->arpTable; entry; entry = entry->ae_next) {
        if (entry->ae_ip == ip)
            break;
    }
    return entry;
}


/*!
 * \brief Create a new ARP cache entry.
 *
 * \note A read lock must have been set before calling this function.
 *
 * If the cache is locked by another reader, the entry is not added.
 * Later we may add a backlog to queue completed entries.
 */
static ARPENTRY *NutArpCacheNew(IFNET * ifn, u_long ip, u_char * ha,
                                u_char flags)
{
    u_char rc = 0;
    ARPENTRY *entry = 0;

    if ((entry = NutHeapAllocClear(sizeof(ARPENTRY))) != 0) {
        entry->ae_ip = ip;
        if (ha)
            memcpy(entry->ae_ha, ha, 6);
        entry->ae_flags = flags;

        NutEnterCritical();
        entry->ae_next = ifn->arpTable;
        ifn->arpTable = entry;
        rc = 1;
        NutExitCritical();

        if (rc == 0) {
            NutHeapFree(entry);
            entry = 0;
        }
    }
    return entry;
}

/*!
 * \brief Update an ARP entry.
 *
 * If an entry with the same IP address exists, then this entry is 
 * updated. If no entry exists, a new one is created. All threads 
 * waiting for address resolution are woken up.
 *
 * \note This function is automatically called on each incoming
 *       ARP telegram. Applications typically do not call this 
 *       function.
 *
 * \param dev Identifies the device, which cache is updated.
 *            This pointer must have been retrieved previously
 *            by using the NutDeviceOpen() function.
 * \param ip  Requested IP address in network byte order.
 * \param ha  Pointer to a buffer which receives the MAC address.
 */
void NutArpCacheUpdate(NUTDEVICE * dev, u_long ip, u_char * ha)
{
    ARPENTRY *entry;

    if (arpExThread == 0)
        arpExThread = NutThreadCreate("arpex", NutArpExpire, NULL, 384);

    entry = NutArpCacheLookup(dev->dev_icb, ip);

    /*
     * Entry found, update it.
     */
    if (entry) {

        /*
         * Do not update permanent entries.
         */
        if ((entry->ae_flags & ATF_PERM) == 0) {
            entry->ae_outdated = 0;
            memcpy(entry->ae_ha, ha, 6);
            entry->ae_flags |= ATF_COM;
        }

        /*
         * Wake all waiting threads.
         */
        NutEventBroadcast(&entry->ae_tq);
        return;
    }

    /*
     * Entry not found. Try to add a new one.
     */
    NutArpCacheNew(dev->dev_icb, ip, ha, ATF_COM);
}

/*!
 * \brief Query MAC address for a specified IP address.
 *
 * If no entry is available in the ARP cache, an ARP request
 * is generated, sent out to the network and the calling
 * thread will be suspended.
 *
 * \param dev  Identifies the device to be used for sending.
 *             This pointer must have been retrieved previously
 *             by using the NutDeviceOpen() function.
 * \param ip   IP address of which the caller asked the MAC address.
 * \param mac  Buffer for the retrieved MAC address.
 *
 * \return 0 if address resolved, -1 otherwise.
 */
int NutArpCacheQuery(NUTDEVICE * dev, CONST u_long ip, u_char * mac)
{
    volatile ARPENTRY *entry;
    NETBUF *nb = 0;
    int rc = -1;
    u_char retries = 4;

    /*
     * Search a matching entry. If we found a completed
     * one, return the MAC address to the caller.
     */
    if ((entry = NutArpCacheLookup(dev->dev_icb, ip)) != 0) {
        if (entry->ae_flags & ATF_COM) {
            memcpy(mac, ((ARPENTRY *) entry)->ae_ha, 6);
            return 0;
        }
    }

    /*
     * No entry exists. Create a new incomplete entry.
     * This may fail if we ran out of memeory.
     */
    else if ((entry = NutArpCacheNew(dev->dev_icb, ip, 0, 0)) == 0) {
        return -1;
    }

    /*
     * We have created a new entry or found an incomplete one.
     * Allocate a request packet.
     */
    if ((nb = NutArpAllocNetBuf(ARPOP_REQUEST, ip, 0)) == 0) {
        return -1;
    }

    /*
     * We enter a loop, which will send ARP requests on fixed
     * time intervals until our ARP entry gets completed.
     */
    while (retries--) {
        if (NutArpOutput(dev, nb))
            break;
        if (entry->ae_flags & ATF_COM) {
            memcpy(mac, ((ARPENTRY *) entry)->ae_ha, 6);
            rc = 0;
            break;
        }

        /*
         * Sleep until woken up by a new ARP
         * response from the net or on timeout.
         */
        NutEventWait(&((ARPENTRY *) entry)->ae_tq, 500);
    }
    NutNetBufFree(nb);
    return rc;
}

/*@}*/
