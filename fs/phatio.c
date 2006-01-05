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
 * \file fs/phatio.c
 * \brief PHAT File System.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/01/05 16:31:45  haraldkipp
 * First check-in.
 *
 *
 * \endverbatim
 */

#include <errno.h>

#include <fs/fs.h>

#include <fs/phatfs.h>
#include <fs/phatvol.h>
#include <dev/blockdev.h>
#include <fs/phatio.h>

#if 0
/* Use for local debugging. */
#define NUTDEBUG
#include <stdio.h>
#endif

/*!
 * \addtogroup xgPhatIo
 */
/*@{*/

/*!
 * \brief Flush sector buffers.
 *
 * \param dev Specifies the file system device.
 *
 * \return 0 on success, -1 on failures.
 */
int PhatSectorFlush(NUTDEVICE * dev)
{
    BLKPAR_SEEK pars;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;
    NUTFILE *blkmnt = dev->dev_icb;
    NUTDEVICE *blkdev = blkmnt->nf_dev;

    if (vol->vol_bufdirty) {
        pars.par_nfp = blkmnt;
        pars.par_blknum = vol->vol_bufsect;
        if ((*blkdev->dev_ioctl) (blkdev, NUTBLKDEV_SEEK, &pars)) {
            errno = EIO;
            return -1;
        }
        if ((*blkdev->dev_write) (blkmnt, vol->vol_buf, 1) != 1) {
            errno = EIO;
            return -1;
        }
        vol->vol_bufdirty = 0;
    }
    return 0;
}

/*!
 * \brief Read sector.
 *
 * \param blkmnt Specifies the mounted block device partition.
 * \param sect   Sector to load.
 * \param buf    Points to a buffer which will receive the sector data.
 *
 * \return 0 on success, -1 on failures.
 */
int PhatSectorRead(NUTFILE * blkmnt, u_long sect, u_char * buf)
{
    BLKPAR_SEEK pars;
    NUTDEVICE *blkdev = blkmnt->nf_dev;

    pars.par_nfp = blkmnt;
    pars.par_blknum = sect;
    if ((*blkdev->dev_ioctl) (blkdev, NUTBLKDEV_SEEK, &pars)) {
        errno = EIO;
        return -1;
    }
    if ((*blkdev->dev_read) (blkmnt, buf, 1) != 1) {
        errno = EIO;
        return -1;
    }
    return 0;
}

/*
 * \param dev Specifies the file system device.
 * \return 0 on success, -1 on failures.
 */
int PhatSectorLoad(NUTDEVICE * dev, u_long sect)
{
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    if (vol->vol_bufsect != sect) {
        if (PhatSectorFlush(dev)) {
            return -1;
        }
        if (PhatSectorRead(dev->dev_icb, sect, vol->vol_buf)) {
            return -1;
        }
        vol->vol_bufsect = sect;
    }
    return 0;
}

/*@}*/
