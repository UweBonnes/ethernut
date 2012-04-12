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
 * Revision 1.1  2003/05/09 14:40:26  haraldkipp
 * Initial revision
 *
 * Revision 1.2  2003/04/21 16:20:54  harald
 * Added comment for debug device
 *
 * Revision 1.1  2003/02/04 17:49:05  harald
 * *** empty log message ***
 *
 */

#include "nut_io.h"

/*!
 * \addtogroup xgCrtStdio
 */
/*@{*/

/*!
 * \brief Print formatted data to a stream.
 *
 * Similar to fprintf() except that the format string is located in
 * program memory.
 *
 * \param stream Pointer to a previously opened stream.
 * \param fmt    Format string in program space containing conversion
 *               specifications.
 *
 * \return The number of bytes written or a negative value to indicate
 *         an error.
 *
 * \warning The function will not check, if the stream pointer points
 *          to a valid stream.
 *
 * \warning Do not use this function with a debug device while running
 *          in interrupt context.
 */
int fprintf_P(FILE * stream, PGM_P fmt, ...)
{
    int rc;
    va_list ap;

    va_start(ap, fmt);
    rc = vfprintf_P(stream, fmt, ap);
    va_end(ap);

    return rc;
}

/*@}*/
