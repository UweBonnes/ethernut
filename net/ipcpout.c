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
 * Portions are 
 * Copyright (c) 1989 by Carnegie Mellon University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by Carnegie Mellon University.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * $Log$
 * Revision 1.3  2003/08/14 15:17:50  haraldkipp
 * Caller controls ID increment
 *
 * Revision 1.2  2003/07/24 16:13:38  haraldkipp
 * Never request a rejected DNS
 *
 * Revision 1.1.1.1  2003/05/09 14:41:31  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.1  2003/03/31 14:53:27  harald
 * Prepare release 3.1
 *
 */
#include <string.h>

#include <dev/ppp.h>

#include <sys/heap.h>
#include <sys/thread.h>

#include <net/if_var.h>
#include <netinet/if_ppp.h>
#include <netinet/ppp_fsm.h>
#include <net/ppp.h>

/*!
 * \addtogroup xgIPCP
 */
/*@{*/

/*!
 * \brief Send a IPCP packet.
 *
 * \note Applications typically do not call this function.
 *
 * \param dev   Identifies the device to use.
 * \param nb    Network buffer structure containing the packet to send
 *              or null if the packet contains no information.
 *              The structure must have been allocated by a previous
 *              call NutNetBufAlloc() and will be freed when this function
 *              returns.
 *
 * \return 0 on success, -1 in case of any errors.
 */
int NutIpcpOutput(NUTDEVICE * dev, u_char code, u_char id, NETBUF * nb)
{
    XCPHDR *xcp;

    if ((nb = NutNetBufAlloc(nb, NBAF_NETWORK, sizeof(XCPHDR))) == 0)
        return -1;

    xcp = nb->nb_nw.vp;
    xcp->xch_code = code;
    xcp->xch_id = id;
    xcp->xch_len = htons(nb->nb_nw.sz + nb->nb_tp.sz + nb->nb_ap.sz);

    if (NutPppOutput(dev, PPP_IPCP, 0, nb) == 0) {
        NutNetBufFree(nb);
        return 0;
    }
    return -1;
}

/*
 * Send a Configure-Request.
 * TODO: May use preconfigured addresses.
 */
void IpcpTxConfReq(NUTDEVICE *dev, u_char id)
{
    PPPDCB *dcb = dev->dev_dcb;
    XCPOPT *xcpo;
    NETBUF *nb;
    u_int len;

    /*
     * Not currently negotiating.
     */
    if(dcb->dcb_ipcp_state != PPPS_REQSENT &&
       dcb->dcb_ipcp_state != PPPS_ACKRCVD &&
       dcb->dcb_ipcp_state != PPPS_ACKSENT) {
        dcb->dcb_ipcp_naks = 0;
    }
    dcb->dcb_acked = 0;

    /*
     * Create the request.
     */
    len = 6;
    if((dcb->dcb_rejects & REJ_IPCP_DNS1) == 0)
        len += 6;
    if((dcb->dcb_rejects & REJ_IPCP_DNS2) == 0)
        len += 6;
    if ((nb = NutNetBufAlloc(0, NBAF_APPLICATION, len)) != 0) {
        xcpo = nb->nb_ap.vp;
        xcpo->xcpo_type = IPCP_ADDR;
        xcpo->xcpo_len = 6;
        xcpo->xcpo_.ul = dcb->dcb_local_ip;

        if((dcb->dcb_rejects & REJ_IPCP_DNS1) == 0) {
            xcpo = (XCPOPT *)((char *)xcpo + xcpo->xcpo_len);
            xcpo->xcpo_type = IPCP_MS_DNS1;
            xcpo->xcpo_len = 6;
            xcpo->xcpo_.ul = dcb->dcb_ip_dns1;
        }

        if((dcb->dcb_rejects & REJ_IPCP_DNS2) == 0) {
            xcpo = (XCPOPT *)((char *)xcpo + xcpo->xcpo_len);
            xcpo->xcpo_type = IPCP_MS_DNS2;
            xcpo->xcpo_len = 6;
            xcpo->xcpo_.ul = dcb->dcb_ip_dns2;
        }
        NutIpcpOutput(dev, XCP_CONFREQ, id, nb);
    }
}

/*@}*/


