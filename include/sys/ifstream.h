#ifndef _SYS_IFSTREAM_H_
#define _SYS_IFSTREAM_H_

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
 * Revision 1.2  2004/03/16 16:48:44  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:20  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.7  2003/02/04 18:00:53  harald
 * Version 3 released
 *
 * Revision 1.6  2002/06/26 17:29:28  harald
 * First pre-release with 2.4 stack
 *
 */

/*!
 * \file sys/ifstream.h
 * \brief Stream device interface definitions.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Stream interface type.
 */
typedef struct _IFSTREAM IFSTREAM;

#include <sys/device.h>

/*!
 * \struct _IFSTREAM ifstream.h sys/ifstream.h
 * \brief Stream interface information structure.
 */
struct _IFSTREAM {
    int  (*if_input)(NUTDEVICE *);  /*!< \brief Wait for input. */
    int  (*if_output)(NUTDEVICE *); /*!< \brief Initiate output. */
    int  (*if_flush)(NUTDEVICE *);  /*!< \brief Wait until output buffer empty. */
    volatile u_char if_rx_idx;      /*!< \brief Next input index. */
    u_char if_rd_idx;               /*!< \brief Next read index. */
    volatile u_char if_tx_idx;      /*!< \brief Next output index. */
    u_char if_wr_idx;               /*!< \brief Next write index. */
    volatile u_char if_tx_act;      /*!< \brief Set if transmitter running. */
    u_char if_last_eol;             /*!< \brief Last end of line character read. */
    u_char if_rx_buf[256];          /*!< \brief Input buffer. */
    u_char if_tx_buf[256];          /*!< \brief Output buffer. */
};

extern int NutIfStreamRead(NUTDEVICE *dev, char *data, int size);
extern int NutIfStreamReadTran(NUTDEVICE *dev, char *data, int size);
extern int NutIfStreamGetLine(NUTDEVICE *dev, char *data, int size);
extern int NutIfStreamWrite(NUTDEVICE *dev, CONST char *data, int len);
extern int NutIfStreamWriteTran(NUTDEVICE *dev, CONST char *data);
#ifdef __HARVARD_ARCH__
extern int NutIfStreamWrite_P(NUTDEVICE *dev, PGM_P data, int len);
#endif
extern int NutIfStreamFlush(NUTDEVICE *dev);

#ifdef __cplusplus
}
#endif

#endif
