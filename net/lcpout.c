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
 * Revision 1.1  2003/05/09 14:41:34  haraldkipp
 * Initial revision
 *
 * Revision 1.2  2003/05/06 18:15:30  harald
 * Use async map define
 *
 * Revision 1.1  2003/03/31 14:53:28  harald
 * Prepare release 3.1
 *
 */

#include <string.h>

#include <dev/ppp.h>

#include <sys/heap.h>

#include <net/if_var.h>
#include <netinet/if_ppp.h>
#include <netinet/ppp_fsm.h>
#include <net/ppp.h>

/*!
 * \addtogroup xgLCP
 */
/*@{*/

/*!
 * \brief Send a LCP packet.
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
int NutLcpOutput(NUTDEVICE * dev, u_char code, u_char id, NETBUF * nb)
{
    XCPHDR *lcp;

    if ((nb = NutNetBufAlloc(nb, NBAF_NETWORK, sizeof(XCPHDR))) == 0)
        return -1;

    lcp = nb->nb_nw.vp;
    lcp->xch_code = code;
    lcp->xch_id = id;
    lcp->xch_len = htons(nb->nb_nw.sz + nb->nb_tp.sz + nb->nb_ap.sz);

    if (NutPppOutput(dev, PPP_LCP, 0, nb) == 0)
        NutNetBufFree(nb);

    return 0;
}

void LcpResetOptions(NUTDEVICE * dev)
{
    PPPDCB *dcb = dev->dev_dcb;

    dcb->dcb_compr = 0;
    dcb->dcb_auth = PPP_PAP;
    dcb->dcb_loc_magic = 0;
    dcb->dcb_rem_magic = 0;
    dcb->dcb_accm = LCP_DEFOPT_ASYNCMAP;
    dcb->dcb_loc_mru = 1500;
}

/*
 * Send a Configure-Request.
 */
void LcpTxConfReq(NUTDEVICE *dev)
{
    PPPDCB *dcb = dev->dev_dcb;
    XCPOPT *xcpo;
    NETBUF *nb;

    /*
     * Not currently negotiating, reset options.
     */
    if(dcb->dcb_lcp_state != PPPS_REQSENT &&
       dcb->dcb_lcp_state != PPPS_ACKRCVD &&
       dcb->dcb_lcp_state != PPPS_ACKSENT) {
        LcpResetOptions(dev);
        dcb->dcb_lcp_naks = 0;
    }
    dcb->dcb_acked = 0;
    dcb->dcb_retries = 0;

    /*
     * Create the request.
     */
    if ((nb = NutNetBufAlloc(0, NBAF_APPLICATION, 6)) != 0) {
        xcpo = nb->nb_ap.vp;
        xcpo->xcpo_type = LCP_ASYNCMAP;
        xcpo->xcpo_len = 6;
        xcpo->xcpo_.ul = htonl(LCP_DEFOPT_ASYNCMAP);
        NutLcpOutput(dev, XCP_CONFREQ, ++dcb->dcb_reqid, nb);
    }
}

/*
 * Send a Protocol-Reject for some protocol.
 */
void LcpTxProtRej(NUTDEVICE *dev, u_short protocol, NETBUF *nb)
{
    PPPDCB *dcb = dev->dev_dcb;
    NETBUF *nbr;
    u_short *sp;

    if ((nbr = NutNetBufAlloc(0, NBAF_APPLICATION, nb->nb_nw.sz)) != 0) {
        sp = nbr->nb_ap.vp;
        *sp++ = protocol;
        memcpy(sp, nb->nb_nw.vp, nb->nb_nw.sz - 2);
        NutNetBufFree(nb);
        NutLcpOutput(dev, LCP_PROTREJ, ++dcb->dcb_rejid, nbr);
    }
    else {
        NutNetBufFree(nb);
    }
}

/*@}*/
