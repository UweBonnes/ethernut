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
 * Revision 1.1  2003/05/09 14:41:45  haraldkipp
 * Initial revision
 *
 * Revision 1.10  2003/05/06 18:23:15  harald
 * Still not done
 *
 * Revision 1.9  2003/02/04 18:14:57  harald
 * Version 3 released
 *
 * Revision 1.8  2002/06/26 17:29:36  harald
 * First pre-release with 2.4 stack
 *
 */

#include <netinet/tcputil.h>

/*!
 * \addtogroup xgTCP
 */
/*@{*/


/*!
 * \brief Sequence number comparisons.
 *
 * Return true if x is between low and high inclusive,
 * false otherwise
 */
int IsInLimits(u_long x, u_long low, u_long high)
{
    if (low <= high) {
        if (low <= x && x <= high)
            return 1;
    } else if (low >= x && x >= high)
        return 1;

    return 0;
}


#if 0
/*
 * This part had been stolen from NetBSD and not yet ported.
 * The main porting problem is our timer resolution.
 */
/*
 * Calculate round trip time.
 */
void NutTcpCalcRtt(TCPSOCKET * sock)
{
    long ticks;

    /* if(sock->so_tx_flags & TCP_SND_ACKED)
       return;
     */

    /*
     * Don't use retransmitted segments to calculate new RTTs,
     * instead keep the last backed-off RTO value instead.
     */
#if 0
    if (sock->so_retransmits)
        sock->so_rto_next <<= sock->so_retransmits;
    else {
        /*
         * Ticks elapsed since last send.
         */
        ticks = 0;              /*(long)(timer_get_jiffies() - sock->so_send_jiffies); */
        if (ticks == 0)
            ticks = 1;

        if (sock->so_srtt_avg == 0) {
            sock->so_srtt_var = ticks << 2;
            sock->so_srtt_avg = sock->so_srtt_var << 1;
        } else {
            /*
             * deviation += abs(error) - deviation
             */
            ticks -= (sock->so_srtt_avg >> 3);
            sock->so_srtt_avg += ticks;
            if (ticks < 0)
                ticks = -ticks;
            ticks -= (sock->so_srtt_var >> 2);
            sock->so_srtt_var += ticks;
        }
        sock->so_rto_next = (sock->so_srtt_avg >> 3) + sock->so_srtt_var;
    }
#endif

    /*
     * Don't allow a timeout that's greater than 2xMSL!
     */
    if (sock->so_rto_next > (240L * TICK_RATE))
        sock->so_rto_next = 240L * TICK_RATE;

    /* sock->so_tx_flags |= TCP_SND_ACKED;
     */
}

void NutTcpCalcRtt(TCPSOCKET * sock)
{
    int old = tpcb->tp_rtt;
    int s, elapsed, delta = 0;

    s = splclock();
    elapsed = (int) (hardclock_ticks - tpcb->tp_rttemit);
    splx(s);

    if (tpcb->tp_rtt != 0) {
        /*
         * rtt is the smoothed round trip time in machine clock
         * ticks (hz). It is stored as a fixed point number,
         * unscaled (unlike the tcp srtt).  The rationale here
         * is that it is only significant to the nearest unit of
         * slowtimo, which is at least 8 machine clock ticks
         * so there is no need to scale.  The smoothing is done
         * according to the same formula as TCP (rtt = rtt*7/8
         * + measured_rtt/8).
         */
        delta = elapsed - tpcb->tp_rtt;
        if ((tpcb->tp_rtt += (delta >> TP_RTT_ALPHA)) <= 0)
            tpcb->tp_rtt = 1;
        /*
         * rtv is a smoothed accumulated mean difference, unscaled
         * for reasons expressed above.
         * It is smoothed with an alpha of .75, and the round trip timer
         * will be set to rtt + 4*rtv, also as TCP does.
         */
        if (delta < 0)
            delta = -delta;
        if ((tpcb->tp_rtv += ((delta - tpcb->tp_rtv) >> TP_RTV_ALPHA)) <= 0)
            tpcb->tp_rtv = 1;
    } else {
        /*
         * No rtt measurement yet - use the unsmoothed rtt. Set the
         * variance to half the rtt (so our first retransmit happens
         * at 3*rtt)
         */
        tpcb->tp_rtt = elapsed;
        tpcb->tp_rtv = elapsed >> 1;
    }
    tpcb->tp_rttemit = 0;
    tpcb->tp_rxtshift = 0;
    /*
     * Quoting TCP: "the retransmit should happen at rtt + 4 * rttvar.
     * Because of the way we do the smoothing, srtt and rttvar
     * will each average +1/2 tick of bias.  When we compute
     * the retransmit timer, we want 1/2 tick of rounding and
     * 1 extra tick because of +-1/2 tick uncertainty in the
     * firing of the timer.  The bias will give us exactly the
     * 1.5 tick we need.  But, because the bias is
     * statistical, we have to test that we don't drop below
     * the minimum feasible timer (which is 2 ticks)."
     */
    TP_RANGESET(tpcb->tp_dt_ticks, TP_REXMTVAL(tpcb), tpcb->tp_peer_acktime, 128 /* XXX */ );
    tpcb->tp_rxtcur = tpcb->tp_dt_ticks;

#endif

/*@}*/
