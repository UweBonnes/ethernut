#ifndef _SYS_FILE_H_
#define _SYS_FILE_H_

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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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
 * Revision 1.2  2006/10/08 16:48:22  haraldkipp
 * Documentation fixed
 *
 * Revision 1.1.1.1  2003/05/09 14:41:20  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.2  2003/04/21 16:59:51  harald
 * Global eof definition added
 *
 * Revision 1.1  2003/02/04 18:13:09  harald
 * Version 3 released
 *
 */

#include <sys/types.h>

struct _NUTDEVICE;

/*!
 * \addtogroup xgDevice
 */
/*@{*/

/*!
 * \brief File structure type.
 */
typedef struct _NUTFILE NUTFILE;

/*!
 * \struct _NUTFILE file.h sys/file.h
 * \brief File structure.
 */
struct _NUTFILE {

    /*!
     * \brief Device containing this file.
     */
    struct _NUTDEVICE *nf_dev;

    /*!
     * \brief Device specific file control block.
     */
    void *nf_fcb;
};

#define NUTFILE_EOF ((NUTFILE *)(-1))

/*@}*/

#endif
