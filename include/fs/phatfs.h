#ifndef FS_PHATFS_H_
#define FS_PHATFS_H_

/*
 * Copyright (C) 2005-2006 by egnite Software GmbH. All rights reserved.
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
 * \file fs/phat.h
 * \brief PHAT file system.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.4  2006/06/18 16:42:50  haraldkipp
 * Support for long filenames (VFAT) added.
 *
 * Revision 1.3  2006/05/15 11:48:38  haraldkipp
 * Added a second PHAT file system instance.
 *
 * Revision 1.2  2006/01/22 17:44:20  haraldkipp
 * ICCAVR failed to find local header files.
 *
 * Revision 1.1  2006/01/05 16:32:57  haraldkipp
 * First check-in.
 *
 *
 * \endverbatim
 */

#include <sys/types.h>
#include <sys/file.h>
#include <sys/device.h>

#include <fs/phatdir.h>

/*!
 * \addtogroup xgPhatFs
 */
/*@{*/

/*!
 * \brief Maximum length of a full path name.
 */
#define PHAT_MAX_PATHLEN    255

/*!
 * \brief Maximum length of a base file name.
 */
#define PHAT_MAX_NAMELEN    12

/*!
 * \brief PHAT file descriptor structure.
 */
typedef struct _PHATFILE {
    /*! \brief Current position into the file. */
    u_long f_pos;
    /*! \brief Current cluster. */
    u_long f_clust;
    /*! \brief Sector within the current cluster. */
    u_long f_clust_pos;
    /*! \brief Position within the sector. */
    u_long f_sect_pos;
    /*! \brief Previous cluster used, */
    u_long f_clust_prv;
    /*! \brief File open mode flags. */
    u_long f_mode;
    /*! \brief Directory entry of this file. */
    PHATDIRENT f_dirent;
    /*! \brief Sector of the directory entry.
     *
     * For the root directory this value is zero, because the root
     * doesn't have any entry in another directory.
     */
    u_long f_de_sect;
    /*! \brief Offset into the sector containing the directory entry. */
    u_long f_de_offs;
    /*! \brief Directory entry change marker. */
    u_int f_de_dirty;
    /*! \brief First cluster of the parent directory, low word. 
     *
     * Our directory entry is located in this cluster.
     */
    u_short f_pde_clust;
    /*! \brief First cluster of the parent directory, high word. */
    u_short f_pde_clusthi;
} PHATFILE;

/*! \brief Marks end of cluster chain. */
#define PHATEOC     0x0FFFFFF8

/*!
 * \name File attributes.
 */
/*@{*/

/*! Read only file. */
#define PHAT_FATTR_RDONLY    0x01
/*! Hidden file. */
#define PHAT_FATTR_HIDDEN    0x02
/*! System file. */
#define PHAT_FATTR_SYSTEM    0x04
/*! No file, but a volume label. */
#define PHAT_FATTR_VOLID     0x08
/*! File contains a subdirectory. */
#define PHAT_FATTR_DIR       0x10
/*! File is not archived. This flag will be set when the file is created
 * or modified.
 */
#define PHAT_FATTR_ARCHIV    0x20

/*! Long filename entry. */
#define PHAT_FATTR_LFN      (PHAT_FATTR_RDONLY | PHAT_FATTR_HIDDEN | PHAT_FATTR_SYSTEM | PHAT_FATTR_VOLID)

/*! Only these flags are allowed for files and directories. */
#define PHAT_FATTR_FILEMASK  0x37

/*@}*/


#include <fs/phat12.h>
#include <fs/phat16.h>
#include <fs/phat32.h>

extern NUTDEVICE devPhat0;
extern NUTDEVICE devPhat1;

extern u_long AllocFirstCluster(NUTFILE * nfp);

#endif
