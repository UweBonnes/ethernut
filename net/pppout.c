/*!
 * Copyright (C) 2002 by Call Direct Cellular Solutions Pty. Ltd. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY CALL DIRECT CELLULAR SOLUTIONS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL CALL DIRECT
 * CELLULAR SOLUTIONS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.calldirect.com.au/
 * -
 *
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
 */

/*
 * $Log$
 * Revision 1.1  2003/05/09 14:41:37  haraldkipp
 * Initial revision
 *
 * Revision 1.2  2003/05/06 18:17:58  harald
 * PPP hack for simple UART support
 *
 * Revision 1.1  2003/03/31 14:53:28  harald
 * Prepare release 3.1
 *
 */

#include <string.h>

#include <dev/ppp.h>
#include <dev/ahdlc.h>
#include <netinet/if_ppp.h>
#include <net/ppp.h>

#include <net/netdebug.h>

/*!
 * \addtogroup xgPPP
 */
/*@{*/


/*!
 * \brief Send PPP frame.
 *
 * Send a PPP frame of a given type using the specified device.
 *
 * \param dev   Identifies the network device to use.
 * \param type  Type of this frame.
 * \param ha    Hardware address of the destination, ignored with PPP.
 * \param nb    Network buffer structure containing the packet to be sent.
 *              The structure must have been allocated by a previous
 *              call NutNetBufAlloc() and will be freed if this function
 *              returns with an error.
 *
 * \return 0 on success, -1 in case of any errors.
 */
int NutPppOutput(NUTDEVICE * dev, u_short type, u_char * ha, NETBUF * nb)
{
    PPPHDR *ph;
    IFNET *nif = dev->dev_icb;
    PPPDCB *dcb = dev->dev_dcb;

    /*
     * Allocate and set the HDLC header.
     */
    if (NutNetBufAlloc(nb, NBAF_DATALINK, sizeof(PPPHDR)) == 0)
        return -1;

    ph = (PPPHDR *) nb->nb_dl.vp;
    ph->address = AHDLC_ALLSTATIONS;
    ph->control = AHDLC_UI;
    ph->prot_type = htons(type);

#ifdef NUTDEBUG
    if (__ppp_trf) {
        fprintf(__ppp_trs, "\nPPP<");
        NutDumpPpp(__ppp_trs, nb);
    }
#endif

    /*
     * Call the network device output routine.
     */
    if(nif->if_send) {
        if((*nif->if_send)(((NUTFILE *)(dcb->dcb_fd))->nf_dev, nb)) {
            NutNetBufFree(nb);
            return -1;
        }
    }

    /*
     * Using a simple UART.
     */
    else 
        PPPPutPacket(dev, nb);

    return 0;
}


/*@}*/

