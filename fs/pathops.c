/*
 * Copyright (C) 2004-2005 by egnite Software GmbH. All rights reserved.
 *
 * Copyright (c) 1991
 *      The Regents of the University of California.  All rights reserved.
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

/*!
 * \file fs/pathops.c
 * \brief File system path operations.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.2  2006/01/05 16:52:23  haraldkipp
 * The argument for the FS_STATUS ioctl now uses an individual structure.
 *
 * Revision 1.1  2005/02/05 20:35:21  haraldkipp
 * Peanut added
 *
 *
 * \endverbatim
 */

#include <errno.h>
#include <sys/device.h>

#include <fs/fs.h>
#include <unistd.h>
#include <sys/stat.h>

/*!
 * \addtogroup xgFS
 */
/*@{*/

static int PathOperation(CONST char *path, int opcode)
{
    NUTDEVICE *dev;
    u_char dev_name[9];
    u_char nidx;
    CONST char *nptr = path;

    /*
     * Extract device name.
     */
    for (nidx = 0; *nptr && *nptr != ':' && nidx < 8; nidx++) {
        dev_name[nidx] = *nptr++;
    }
    dev_name[nidx] = 0;
    nptr++;

    /*
     * Get device structure of registered device. In later releases we
     * try to open a file on a root device.
     */
    if ((dev = NutDeviceLookup(dev_name)) == 0) {
        errno = ENOENT;
        return -1;
    }
    return (*dev->dev_ioctl) (dev, opcode, (void *) nptr);
}

/*!
 * \brief Check the accessibility of a file.
 *
 * \param path Pathname of the file to check.
 * \param what Access permission to check.
 *
 * \return Always -1 due to missing implementation.
 */
int access(CONST char *path, int what)
{
    return -1;
}

/*!
 * \brief Reposition a file pointer.
 *
 * \param fh     Handle of an open file.
 * \param pos    Positioning value.
 * \param whence Positioning directive.
 *
 * \return Always -1 due to missing implementation.
 */
long lseek(int fh, long pos, int whence)
{
    //IOCTL_ARG3 args;

    //args.arg1 = (void *)fh;
    //args.arg2 = (void *)(u_int)pos;
    //args.arg3 = (void *)whence;
    //return (*dev->dev_ioctl) (dev, opcode, (void *)&args);
    return -1;
}

/*!
 * \brief Remove a directory.
 *
 * \param name Pathname of the directory. Must be the full pathname
 *             including the device, because Nut/OS doesn't support 
 *             relative paths.
 *
 * \return 0 if the remove succeeds, otherwise -1 is returned.
 */
int rmdir(CONST char *path)
{
    return PathOperation(path, FS_DIR_REMOVE);
}

/*!
 * \brief Remove a file entry.
 *
 * \return 0 if the remove succeeds, otherwise -1 is returned.
 */
int unlink(CONST char *path)
{
    return PathOperation(path, FS_FILE_DELETE);
}


/*!
 * \brief Remove a directory.
 *
 * \return 0 if the query succeeds, otherwise -1 is returned.
 */
int stat(CONST char *path, struct stat *s)
{
    NUTDEVICE *dev;
    u_char dev_name[9];
    u_char nidx;
    CONST char *nptr = path;
    FSCP_STATUS parms;

    /* Extract the device name. */
    for (nidx = 0; *nptr && *nptr != ':' && nidx < 8; nidx++) {
        dev_name[nidx] = *nptr++;
    }
    dev_name[nidx] = 0;

    /* Get device structure of registered device. */
    if ((dev = NutDeviceLookup(dev_name)) != 0) {
        if (*nptr == ':') {
            nptr++;
        }
        parms.par_path = nptr;
        parms.par_stp = s;
        return (*dev->dev_ioctl) (dev, FS_STATUS, (void *) &parms);
    }
    return -1;
}

/*!
 * \brief Remove a directory.
 *
 * \return Always -1 due to missing implementation.
 */
int fstat(int fh, struct stat *s)
{
    return -1;
}

/*!
 * \brief Create a directory entry.
 *
 * \return 0 on success, otherwise -1 is returned.
 */
int mkdir(CONST char *path, int mode)
{
    return PathOperation(path, FS_DIR_CREATE);
}

/*@}*/
