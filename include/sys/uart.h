#ifndef _SYS_UART_H
#define _SYS_UART_H

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
 */

/*
 * $Log$
 * Revision 1.1  2003/05/09 14:41:23  haraldkipp
 * Initial revision
 *
 * Revision 1.11  2003/05/06 17:58:28  harald
 * Handshakes added
 *
 * Revision 1.10  2003/03/31 14:53:25  harald
 * Prepare release 3.1
 *
 * Revision 1.9  2003/02/04 18:00:54  harald
 * Version 3 released
 *
 * Revision 1.8  2003/01/14 16:35:24  harald
 * Definitions moved
 *
 * Revision 1.7  2002/11/02 15:17:01  harald
 * Library dependencies moved to compiler.h
 *
 * Revision 1.6  2002/06/26 17:29:30  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/device.h>

/*!
 * \file sys/uart.h
 * \brief UART I/O function prototypes.
 */

__BEGIN_DECLS

/*!
 * \addtogroup xgUARTIOCTL
 */
/*@{*/

#define UART_SETSPEED       0x0101  /*!< \brief Set baudrate. */
#define UART_GETSPEED       0x0102  /*!< \brief Query baudrate. */
#define UART_SETDATABITS    0x0103  /*!< \brief Set number of data bits. */
#define UART_GETDATABITS    0x0104  /*!< \brief Set number of data bits. */
#define UART_SETPARITY      0x0105  /*!< \brief Set parity. */
#define UART_GETPARITY      0x0106  /*!< \brief Query parity. */
#define UART_SETSTOPBITS    0x0107  /*!< \brief Set number of stop bits. */
#define UART_GETSTOPBITS    0x0108  /*!< \brief Query number of stop bits. */
#define UART_SETSTATUS      0x0109  /*!< \brief Set status. */
#define UART_GETSTATUS      0x010a  /*!< \brief Query status. */
#define UART_SETREADTIMEOUT 0x010b  /*!< \brief Set read timeout. */
#define UART_GETREADTIMEOUT 0x010c  /*!< \brief Query read timeout. */
#define UART_SETWRITETIMEOUT 0x010d /*!< \brief Set write timeout. */
#define UART_GETWRITETIMEOUT 0x010e /*!< \brief Query write timeout. */
#define UART_SETLOCALECHO   0x010f  /*!< \brief Set local echo mode. */
#define UART_GETLOCALECHO   0x0110  /*!< \brief Query local echo mode. */
#define UART_SETFLOWCONTROL 0x0111  /*!< \brief Set flow control flags. */ 
#define UART_GETFLOWCONTROL 0x0112  /*!< \brief Query flow control flags. */ 
#define UART_SETCOOKEDMODE  0x0113  /*!< \brief Set raw mode. */ 
#define UART_GETCOOKEDMODE  0x0114  /*!< \brief Query raw mode. */ 
#define UART_SETBUFFERMODE  0x0115  /*!< \brief Set buffering mode. */ 
#define UART_GETBUFFERMODE  0x0116  /*!< \brief Query buffering mode. */ 
#define HDLC_SETIFNET       0x0117  /*!< \brief Set network interface mode. */ 
#define HDLC_GETIFNET       0x0118  /*!< \brief Query network interface mode. */ 

#define DIO_SETSTATUS       0x0209  /*!< \brief Set digital output status. */
#define DIO_GETSTATUS       0x020a  /*!< \brief Query digital input status. */
#define DIO_WAITSTATUS      0x020b  /*!< \brief Wait for digital input status change. */

/*@}*/

/*!
 * \addtogroup xgUARTStatus
 */
/*@{*/
#define UART_FRAMINGERROR   0x00000001  /*!< \brief Framing error. */
#define UART_OVERRUNERROR   0x00000002  /*!< \brief Overrun error. */
#define UART_RXBUFFEREMPTY  0x00000004  /*!< \brief Receiver buffer empty. */
#define UART_TXBUFFEREMPTY  0x00000008  /*!< \brief Transmitter buffer empty. */

#define UART_RTSENABLED     0x00000100  /*!< \brief RTS handshake output enabled. */
#define UART_RTSDISABLED    0x00000200  /*!< \brief RTS handshake output disabled. */
#define UART_CTSENABLED     0x00000400  /*!< \brief CTS handshake input enabled. */
#define UART_CTSDISABLED    0x00000800  /*!< \brief CTS handshake input disabled. */
#define UART_DTRENABLED     0x00001000  /*!< \brief DTR handshake output enabled. */
#define UART_DTRDISABLED    0x00002000  /*!< \brief DTR handshake output disabled. */
/*@}*/


extern int UartAvrInit(NUTDEVICE *dev);
extern int UartAvrIOCtl(NUTDEVICE *dev, int req, void *conf);
extern int UartAvrInput(NUTDEVICE *dev);
extern int UartAvrOutput(NUTDEVICE *dev);
extern int UartAvrFlush(NUTDEVICE *dev);

extern int UartAvrGetRaw(u_char *cp);
extern int UartAvrPutRaw(u_char ch);

extern int UartAvrRead(NUTFILE *fp, void *buffer, int size);
extern int UartAvrWrite(NUTFILE *fp, CONST void *buffer, int len);
extern int UartAvrWrite_P(NUTFILE *fp, PGM_P buffer, int len);
extern NUTFILE *UartAvrOpen(NUTDEVICE *dev, CONST char *name, int mode, int acc);
extern int UartAvrClose(NUTFILE *fp);

__END_DECLS

#endif
