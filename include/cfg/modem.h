#ifndef _CFG_MODEM_H_
#define _CFG_MODEM_H_

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
 * Revision 1.2  2003/08/05 20:17:46  haraldkipp
 * Typing errors corrected
 *
 * Revision 1.1.1.1  2003/05/09 14:41:04  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.1  2003/05/06 18:37:33  harald
 * *** empty log message ***
 *
 */

/*!
 * \file cfg/modem.h
 * \brief RS232 handshake hardware specification.
 */

/*!
 * \addtogroup xgModemCfg
 */
/*@{*/

#if 0

/*! Port output register of \ref UART1_RTS_BIT. */
#define UART1_RTS_PORT   PORTD
/*! Data direction register of \ref UART1_RTS_BIT. */
#define UART1_RTS_DDR    DDRD
/*! \brief RTS handshake output bit. 
           If undefined, RTS handshake is not supported. */
#define UART1_RTS_BIT    5

/*! Port output register of \ref UART1_CTS_BIT. */
#define UART1_CTS_PORT   PORTE
/*! Port input register of \ref UART1_CTS_BIT. */
#define UART1_CTS_PIN    PINE
/*! Data direction register of \ref UART1_CTS_BIT. */
#define UART1_CTS_DDR    DDRE
/*! Interrupt signal of \ref UART1_CTS_BIT. */
#define UART1_CTS_SIGNAL sig_INTERRUPT7
/*! \brief CTS handshake input bit. 
           If undefined, CTS handshake is not supported.
           Bits 0 to 3 are not supported. */
#define UART1_CTS_BIT    7

/*! Port output register of \ref UART1_DTR_BIT. */
#define UART1_DTR_PORT   PORTD
/*! Data direction register of \ref UART1_DTR_BIT. */
#define UART1_DTR_DDR    DDRD
/*! \brief DTR handshake output bit.
           If undefined, DTR handshake is not supported. */
#define UART1_DTR_BIT    7

#endif

/*@}*/

#endif
