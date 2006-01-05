#ifndef DEV_BLOCKDEV_H_
#define DEV_BLOCKDEV_H_

/*
 * Copyright (C) 2005 by egnite Software GmbH. All rights reserved.
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

/*!
 * \file dev/blockdev.h
 * \brief Block device driver definitions.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/01/05 16:32:05  haraldkipp
 * First check-in.
 *
 *
 * \endverbatim
 */

#include <sys/types.h>
#include <sys/file.h>

/*!
 * \addtogroup xgBlockDev
 */
/*@{*/

/*!
 * \name Control Codes
 */
/*@{*/

/*! \brief Query for media change. */
#define NUTBLKDEV_MEDIACHANGE  0x1201
/*! \brief Retrieve device information. */
#define NUTBLKDEV_INFO         0x1202
/*! \brief Block seek request. */
#define NUTBLKDEV_SEEK         0x1203

/*@}*/


typedef struct _BLKPAR_SEEK {
    NUTFILE *par_nfp;
    u_long par_blknum;
} BLKPAR_SEEK;

typedef struct _BLKPAR_INFO {
    NUTFILE *par_nfp;
    u_long par_nblks;
    u_long par_blksz;
    u_char *par_blkbp;
} BLKPAR_INFO;

/*@}*/

#endif
