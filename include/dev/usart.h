#ifndef _DEV_USART_H_
#define _DEV_USART_H_

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
 * Revision 1.2  2004/03/16 16:48:28  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1  2003/12/18 09:33:58  haraldkipp
 * First check in
 *
 */

#include <dev/uart.h>

/*!
 * \file dev/usart.h
 * \brief Synchronous/asynchronous serial device definitions.
 */

/*!
 * \addtogroup xgUsart
 */
/*@{*/

/*!
 * \name Ring Buffer
 */
/*@{*/

/*! \brief Initial receive buffer size.
 * \showinitializer
 */
#ifndef USART_RXBUFSIZ
#define USART_RXBUFSIZ    256
#endif

/*! \brief Receiver's initial high water mark.
 *
 * Disables receiver handshake.
 * \showinitializer
 */
#ifndef USART_RXHIWMARK
#define USART_RXHIWMARK   240
#endif

/*! \brief Receiver's initial low water mark.
 *
 * Enables receiver handshake.
 * \showinitializer
 */
#ifndef USART_RXLOWMARK
#define USART_RXLOWMARK   208
#endif

/*! \brief Initial transmit buffer size.
 * \showinitializer
 */
#ifndef USART_TXBUFSIZ
#define USART_TXBUFSIZ    64
#endif

/*! \brief Transmitter's initial high water mark.
 *
 * Starts the transmitter.
 * \showinitializer
 */
#ifndef USART_TXHIWMARK
#define USART_TXHIWMARK   56
#endif

/*! \brief Transmitter's initial low water mark.
 *
 * Wakes up transmitting threads.
 * \showinitializer
 */
#ifndef USART_TXLOWMARK
#define USART_TXLOWMARK   40
#endif

/*!
 * \typedef RINGBUF
 * \brief Character device ring buffer type.
 */
typedef struct _RINGBUF RINGBUF;

/*!
 * \struct _RINGBUF usart.h dev/usart.h
 * \brief Character device ring buffer structure.
 */
struct _RINGBUF {

    /*! \brief Buffer head pointer.
     */
    u_char *rbf_head;

    /*! \brief Buffer tail pointer.
     */
    u_char *rbf_tail;

    /*! \brief First buffer address.
     */
    u_char *rbf_start;

    /*! \brief Last buffer address.
     */
    u_char *rbf_last;

    /*! \brief Total buffer size.
     *
     * Zero, if no buffer available.
     */
    size_t rbf_siz;

    /*! \brief Number of bytes in the buffer.
     */
    size_t rbf_cnt;

    /*! \brief Buffer low watermark.
     *
     * If the number of bytes in the buffer reaches this value, then
     * the previously disabled buffer input is enabled again.
     */
    size_t rbf_lwm;

    /*! \brief Buffer high watermark.
     *
     * If the number of bytes in the buffer reaches this value, then
     * buffer input is disabled.
     */
    size_t rbf_hwm;

    /*! \brief Queue of waiting threads.
     *
     * Consuming threads are added to this queue when the buffer is empty.
     * Producing threads are added to this queue when the buffer is full.
     */
    HANDLE rbf_que;
};

/*@}*/


/*!
 * \name Initial UART Configuration
 */
/*@{*/

/*! \brief Initial bit rate.
 *
 * \showinitializer
 */
#define USART_INITSPEED   115200

/*@}*/


#define USART_MF_RTSCONTROL     0x0001  /*!< DTE output. */
#define USART_MF_CTSSENSE       0x0002  /*!< DTE input. */
#define USART_MF_DTRCONTROL     0x0004  /*!< DTE output. */
#define USART_MF_DSRSENSE       0x0008  /*!< DTE input. */
#define USART_MF_DCDSENSE       0x0010  /*!< DTE input. */

#define USART_MF_SENSEMASK      0x001A  /*!< Handshake sense mask. */
#define USART_MF_CONTROLMASK    0x0005  /*!< Handshake control mask. */

/*! \brief Software handshake.
 *
 * It is recommended to set a proper read timeout with software handshake.
 * In this case a timeout may occur, if the communication peer lost our 
 * last XON character. The application may then use ioctl() to disable the 
 * receiver and do the read again. This will send out another XON.
 */
#define USART_MF_XONXOFF        0x0020

#define USART_MF_LOCALECHO      0x0040  /*!< Should be used in stream, not device. */
#define USART_MF_COOKEDMODE     0x0080  /*!< Should be used in stream, not device. */

#define USART_MF_NOBUFFER       0x0100  /*!< No buffering. */
#define USART_MF_LINEBUFFER     0x0200  /*!< Line buffered. */
#define USART_MF_BUFFERMASK     0x0300  /*!< Masks buffering mode flags. */

#define USART_MF_HALFDUPLEX     0x0400  /*!< Half duplex control. */

#define USART_SF_RTSOFF         0x0001  /*!< Set if RTS line is off. */
#define USART_SF_CTSOFF         0x0002  /*!< Set if CTS line is off. */
#define USART_SF_DTROFF         0x0004  /*!< Set if DTR line is off. */
#define USART_SF_DSROFF         0x0008  /*!< Set if DSR line is off. */
#define USART_SF_DCDOFF         0x0010  /*!< Set if DCD line is off. */

#define USART_SF_TXDISABLED     0x0040  /*!< Transmitter disabled. */
#define USART_SF_RXDISABLED     0x0080  /*!< Receiver disabled. */


/*!
 * \struct _USARTDCB usart.h dev/usart.h
 * \brief USART device low level information structure.
 *
 */
struct _USARTDCB {

    /*! \brief Mode flags.
     */
    u_long dcb_modeflags;

    /*! \brief Status flags.
     */
    u_long dcb_statusflags;

    /*! \brief Read timeout.
     */
    u_long dcb_rtimeout;

    /*! \brief Write timeout.
     */
    u_long dcb_wtimeout;

    /*! \brief Output ring buffer.
     */
    RINGBUF dcb_tx_rbf;

    /*! \brief Input ring buffer.
     */
    RINGBUF dcb_rx_rbf;

    /*! \brief Last EOL character.
     */
    u_char dcb_last_eol;

    /*!
     * \fn dcb_init
     * \brief Driver control initialization.
     */
    int (*dcb_init) (void);

    /*!
     * \brief Driver control de-initialization.
     */
    int (*dcb_deinit) (void);

    /*!
     * \brief Driver control write notification.
     */
    void (*dcb_tx_start) (void);

    /*!
     * \brief Driver control read notification.
     */
    void (*dcb_rx_start) (void);

    /*!
     * \brief Set handshake mode.
     */
    int (*dcb_set_flow_control) (u_long flags);

    /*!
     * \brief Get handshake mode.
     */
     u_long(*dcb_get_flow_control) (void);

    /*!
     * \brief Set hardware speed.
     */
    int (*dcb_set_speed) (u_long rate);

    /*!
     * \brief Get hardware speed.
     */
     u_long(*dcb_get_speed) (void);

    /*!
     * \brief Set hardware data bits.
     */
    int (*dcb_set_data_bits) (u_char bits);

    /*!
     * \brief Get hardware data bits.
     */
     u_char(*dcb_get_data_bits) (void);

    /*!
     * \brief Set hardware parity mode.
     */
    int (*dcb_set_parity) (u_char bits);

    /*!
     * \brief Get hardware parity mode.
     */
     u_char(*dcb_get_parity) (void);

    /*!
     * \brief Set hardware stop bits.
     */
    int (*dcb_set_stop_bits) (u_char bits);

    /*!
     * \brief Get hardware stop bits.
     */
     u_char(*dcb_get_stop_bits) (void);

    /*!
     * \brief Set hardware status.
     */
    int (*dcb_set_status) (u_long flags);

    /*!
     * \brief Get hardware status.
     */
     u_long(*dcb_get_status) (void);

    /*!
     * \brief Set clock mode.
     */
    int (*dcb_set_clock_mode) (u_char mode);

    /*!
     * \brief Get clock mode.
     */
     u_char(*dcb_get_clock_mode) (void);
};

/*!
 * USART device low level information type.
 */
typedef struct _USARTDCB USARTDCB;

/*@}*/


__BEGIN_DECLS

/*! \name Function Prototypes */
/*@{*/
extern int UsartInit(NUTDEVICE * dev);
extern int UsartIOCtl(NUTDEVICE * dev, int req, void *conf);
extern int UsartRead(NUTFILE * fp, void *buffer, int size);
extern int UsartWrite(NUTFILE * fp, CONST void *buffer, int len);
#ifdef __HARVARD_ARCH__
extern int UsartWrite_P(NUTFILE * fp, PGM_P buffer, int len);
#endif
extern NUTFILE *UsartOpen(NUTDEVICE * dev, CONST char *name, int mode, int acc);
extern int UsartClose(NUTFILE * fp);
/*@}*/

__END_DECLS

#endif
