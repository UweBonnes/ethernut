#ifndef _SYS_PRINT_H_
#define _SYS_PRINT_H_

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
 * Revision 1.1  2003/05/09 14:41:20  haraldkipp
 * Initial revision
 *
 * Revision 1.8  2003/02/04 18:00:53  harald
 * Version 3 released
 *
 * Revision 1.7  2002/11/02 15:17:01  harald
 * Library dependencies moved to compiler.h
 *
 * Revision 1.6  2002/06/26 17:29:29  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/device.h>

/*!
 * \file sys/print.h
 * \brief Formatted printing definitions.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name Format flags
 *
 * \brief Number printing formats.
 *
 * These flags are used as number printing formats. Several
 * flags may be combined using the binary OR operator.
 *
 * \see NutPrintInteger
 *
 */
/*@{*/

#define FMTFLG_ZERO     0x01    /*!< \brief Set, if zero padding required */
#define FMTFLG_SIGNED   0x02    /*!< \brief Set, if signed value */
#define FMTFLG_PLUS     0x04    /*!< \brief Set to force sign */
#define FMTFLG_MINUS    0x08    /*!< \brief Set to force left justification */
#define FMTFLG_CAPITAL  0x10    /*!< \brief Set for capital letter digits */

/*@}*/

extern int NutPrintBinary(NUTDEVICE *dev, CONST char *data, int len);
extern int NutPrintBinary_P(NUTDEVICE *dev, PGM_P data, int len);
extern int NutPrintString(NUTDEVICE *dev, CONST char *str);
extern int NutPrintString_P(NUTDEVICE *dev, PGM_P str);
extern int NutPrintInteger(NUTDEVICE *dev, u_long val, u_char radix, u_char width, u_char flags);
extern int NutPrintFormat(NUTDEVICE *dev, CONST char *fmt, ...);
extern int NutPrintFlush(NUTDEVICE *dev);

extern HANDLE app1, app2, app3, app4;

#ifdef __cplusplus
}
#endif

#endif
