#ifndef _DEV_NICRTL_H_
#define _DEV_NICRTL_H_

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
 */

/*
 * $Log$
 * Revision 1.2  2003/07/20 16:40:52  haraldkipp
 * CrystalTek 8900A driver added.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:07  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.9  2003/02/04 18:00:37  harald
 * Version 3 released
 *
 * Revision 1.8  2002/10/29 15:40:49  harald
 * *** empty log message ***
 *
 * Revision 1.7  2002/08/08 17:23:20  harald
 * *** empty log message ***
 *
 * Revision 1.6  2002/06/26 17:29:15  harald
 * First pre-release with 2.4 stack
 *
 */

#include <dev/netbuf.h>
#include <net/if_var.h>


/*!
 * \file dev/nicrtl.h
 * \brief Network interface controller definitions.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \struct _NICINFO nicrtl.h dev/nicrtl.h
 * \brief Network interface controller information structure.
 */
struct _NICINFO {
    u_char volatile ni_tx_bsy;      /*!< NIC transmitter busy flags. */
    HANDLE volatile ni_rx_rdy;      /*!< Receiver event queue. */
    HANDLE volatile ni_tx_rdy;      /*!< Transmitter event queue. */
    NETBUF * volatile ni_tx_nbq;    /*!< Queued packets waiting for transmission. */
    u_short ni_tx_cnt;              /*!< Number of bytes in transmission queue. */
    u_char ni_curr_page;            /*!< Current receive page. */
    u_long ni_rx_pending;           /*!< Number of pending receiver interrupts. */
    u_long ni_rx_packets;           /*!< Number of packets received. */
    u_long ni_tx_packets;           /*!< Number of packets sent. */
    u_long ni_interrupts;           /*!< Number of interrupts. */
    u_long ni_overruns;             /*!< Number of packet overruns. */
    u_long ni_rx_frame_errors;      /*!< Number of frame errors. */
    u_long ni_rx_crc_errors;        /*!< Number of CRC errors. */
    u_long ni_rx_missed_errors;     /*!< Number of missed packets. */
};

/*!
 * \brief Network interface controller information type.
 */
typedef struct _NICINFO NICINFO;



/*
 * Available drivers.
 */
extern NUTDEVICE devEth0;
extern NUTDEVICE devEth0cs;

/*
 * Driver routines.
 */
extern int NicInit(NUTDEVICE *dev);
extern int NicOutput(NUTDEVICE *dev, NETBUF *nb);

#ifdef __cplusplus
}
#endif

#endif
