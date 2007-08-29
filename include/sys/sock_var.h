#ifndef _SYS_SOCK_VAR_H_
#define _SYS_SOCK_VAR_H_

/*
 * Copyright (C) 2001-2005 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.10  2007/08/29 07:43:54  haraldkipp
 * Documentation updated and corrected.
 *
 * Revision 1.9  2006/03/21 21:22:19  drsung
 * Enhancement made to TCP state machine. Now TCP options
 * are read from peer and at least the maximum segment size is stored.
 *
 * Revision 1.8  2005/08/02 17:46:49  haraldkipp
 * Major API documentation update.
 *
 * Revision 1.7  2005/06/05 16:48:25  haraldkipp
 * Additional parameter enables NutUdpInput() to avoid responding to UDP
 * broadcasts with ICMP unreachable messages. Fixes bug #1215192.
 *
 * Revision 1.6  2004/07/30 19:54:46  drsung
 * Some code of TCP stack redesigned. Round trip time calculation is now
 * supported. Fixed several bugs in TCP state machine. Now TCP connections
 * should be more reliable under heavy traffic or poor physical connections.
 *
 * Revision 1.5  2004/03/16 16:48:44  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.4  2004/01/14 19:33:13  drsung
 * New TCP output buffer handling
 *
 * Revision 1.3  2003/11/24 21:00:21  drsung
 * Packet queue added for UDP sockets.
 *
 * Revision 1.2  2003/07/13 19:32:12  haraldkipp
 * Faster TCP transfers by changing receive buffer
 *
 * Revision 1.1.1.1  2003/05/09 14:41:22  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.10  2003/02/04 18:00:53  harald
 * Version 3 released
 *
 * Revision 1.9  2002/09/03 17:50:18  harald
 * Configurable receive buffer size
 *
 * Revision 1.8  2002/08/16 17:54:19  harald
 * Count out of sequence drops
 *
 * Revision 1.7  2002/06/26 17:29:29  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/types.h>
#include <dev/netbuf.h>

/*!
 * \file sys/sock_var.h
 * \brief UDP and TCP socket interface definitions.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************\
 * UDP
\*********************************************************************/

/*!
 * \addtogroup xgUdpSocket
 */
/*@{*/

/*!
 * \brief UDP socket type.
 */
typedef struct udp_socket UDPSOCKET;

/*!
 * \brief UDP socket information structure.
 *
 * Applications should not rely on the content of this structure.
 * It may change without notice.
 */
struct udp_socket {
    UDPSOCKET *so_next;     /*!< \brief Link to next tcp socket structure. */
    u_short so_local_port;  /*!< \brief Local port number in net byte order. */
    NETBUF  *so_rx_nb;      /*!< \brief Received, but not read by application. */
    HANDLE  so_rx_rdy;      /*!< \brief Receiver event queue. */
    u_short so_rx_cnt;      /*!< \brief Number of data bytes in the receive buffer. */
    u_short so_rx_bsz;      /*!< \brief Receive buffer size. */
};

/*@}*/

extern void NutUdpInput(NETBUF *nb, ureg_t bcast);
extern int NutUdpOutput(UDPSOCKET *sock, u_long dest, u_short port, NETBUF *nb);


/*********************************************************************\
 * TCP
\*********************************************************************/

/*!
 * \addtogroup xgTcpSocket
 */
/*@{*/

/*!
 * \brief TCP socket type.
 */
typedef struct tcp_socket TCPSOCKET;

/*!
 * \brief TCP socket information structure.
 *
 * Applications should not rely on the content of this structure.
 * It may change without notice.
 */
struct tcp_socket {
    TCPSOCKET *so_next;     /*!< \brief Link to next tcp socket structure. */
    void *so_device;	    /*!< \brief Always zero. */
    u_char so_devtype;	    /*!< \brief Device type, always IFTYP_TCPSOCK. */
    int (*so_devread) (TCPSOCKET *, void *, int); /*!< \brief Read from device. */
    int (*so_devwrite) (TCPSOCKET *, CONST void *, int); /*!< \brief Write to device. */
#ifdef __HARVARD_ARCH__
    int (*so_devwrite_P) (TCPSOCKET *, PGM_P, int); /*!< \brief Write to device. */
#endif
    int (*so_devioctl) (TCPSOCKET *, int, void *); /*!< \brief Driver control function. */
    
    u_short so_devocnt;     /*!< \brief Number of data bytes in output buffer. */
    u_char *so_devobuf;     /*!< \brief Pointer to output buffer. */
    u_short so_devobsz;     /*!< \brief Output buffer size. */

    volatile u_char  so_state;       /*!< \brief Connection state, see tcp_fsm.h */
    u_long  so_local_addr;  /*!< \brief Local IP address in net byte order. */
    u_short so_local_port;  /*!< \brief Local port number in net byte order. */
    u_long  so_remote_addr; /*!< \brief Remote IP address in net byte order. */
    u_short so_remote_port; /*!< \brief Remote port number in net byte order. */

    u_char  so_tx_flags;    /*!< \brief Flags used during transmissions - see below */
    u_long  so_tx_isn;      /*!< \brief Initial sequence number. */
    u_long  so_tx_una;      /*!< \brief Unacknowledged sequence number. */
    u_long  so_tx_nxt;      /*!< \brief Next sequence number to send. */
    u_long  so_tx_wl1;      /*!< \brief Sequence number of last window update. */
    u_long  so_tx_wl2;      /*!< \brief Acknowledged sequence of last window update. */
    u_short so_tx_win;      /*!< \brief Peer's receive window. */
    u_char  so_tx_dup;      /*!< \brief Duplicate ACK counter. */
    NETBUF  *so_tx_nbq;     /*!< \brief Network buffers waiting to be acknowledged. */
    HANDLE  so_tx_tq;       /*!< \brief Threads waiting for transmit buffer space. */

    u_long  so_rx_isn;      /*!< \brief Initial sequence number of remote. */
    u_long  so_rx_nxt;      /*!< \brief Next sequence number to receive. */
    u_short so_rx_win;      /*!< \brief Local receive window. */
    u_short so_rx_cnt;      /*!< \brief Number of data bytes in the receive buffer. */
    u_short so_rx_bsz;      /*!< \brief Receive buffer size. */
    u_short so_rd_cnt;      /*!< \brief Number of bytes read from buffer top. */
    NETBUF  *so_rx_buf;     /*!< \brief Data waiting to be read by application. */
    HANDLE  so_rx_tq;       /*!< \brief Threads waiting for received data. */
    NETBUF  *so_rx_nbq;     /*!< \brief Network buffers received in advance. */

    u_short so_mss;         /*!< \brief MSS, limited by remote option or MTU. */

    u_long  so_rtt_seq;     /*!< \brief Sequence number for RTT calculation. */
    u_short so_rtto;        /*!< \brief Current retransmission timeout. */
    u_short so_retransmits; /*!< \brief Number of retransmits. */
    u_short so_time_wait;   /*!< \brief Time wait counter. */
    u_short so_retran_time; /*!< \brief Retransmit time counter. */
    u_short so_last_error;  /*!< \brief Last error code. */
    HANDLE  so_pc_tq;       /*!< \brief Listening thread. */
    HANDLE  so_ac_tq;       /*!< \brief Connecting thread. */

    u_long  so_read_to;     /*!< \brief Read timeout. */
    u_long  so_write_to;    /*!< \brief Write timeout. */
    u_long  so_oos_drop;    /*!< \brief Out of sequence dropped. */
};

/*
 * TCP send flags.
 */
#define SO_FIN      0x01    /*!< \brief Socket transmit flag. Send FIN after all data has been transmitted. */
#define SO_SYN      0x02    /*!< \brief Socket transmit flag. Send SYN first. */
#define SO_FORCE    0x08    /*!< \brief Socket transmit flag. Force sending ACK. */
#define SO_ACK      0x10    /*!< \brief Socket transmit flag. Send ACK. */

/*@}*/

#include <netinet/tcp_fsm.h>

extern int NutTcpOutput(TCPSOCKET *sock, CONST u_char *data, u_short size);
extern int NutTcpReject(NETBUF *nb);

#ifdef __cplusplus
}
#endif

#endif
