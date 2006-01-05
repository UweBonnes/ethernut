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
 * \file fs/phatvol.c
 * \brief Volume related routines of the PHAT file system.
 *
 * When mounting a partition, we expect the block device driver to call
 * the related mount routine of the file system driver.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/01/05 16:31:56  haraldkipp
 * First check-in.
 *
 *
 * \endverbatim
 */

#include <dev/blockdev.h>

#include <fs/dospart.h>
#include <fs/phatio.h>
#include <fs/phatutil.h>
#include <fs/phatvol.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>


#if 0
/* Use for local debugging. */
#define NUTDEBUG
#include <stdio.h>
#include <fs/phatdbg.h>
#endif

/*!
 * \addtogroup xgPhatVol
 */
/*@{*/

/*!
 * \brief Counts the number of free clusters in a volume.
 *
 * \param dev Specifies the file system device.
 *
 * \return The number of free clusters.
 */
static u_long PhatCountFreeClusters(NUTDEVICE * dev)
{
    u_long rc = 0;
    u_long i = 2;
    u_long link;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    if (vol->vol_type == 32) {
        while (i < vol->vol_last_clust) {
            if (Phat32GetClusterLink(dev, i, &link)) {
                break;
            }
            if (link == 0) {
                rc++;
            }
            i++;
        }
    } else if (vol->vol_type == 16) {
        while (i < vol->vol_last_clust) {
            if (Phat16GetClusterLink(dev, i, &link)) {
                break;
            }
            if (link == 0) {
                rc++;
            }
            i++;
        }
    } else {
        while (i < vol->vol_last_clust) {
            if (Phat12GetClusterLink(dev, i, &link)) {
                break;
            }
            if (link == 0) {
                rc++;
            }
            i++;
        }
    }
    return rc;
}

/*!
 * \brief Mount a PHAT volume.
 *
 * This routine is called by the block device driver while mounting a
 * partition. It reads and verifies the volume boot record, which is
 * located in the first sector of a volume.
 *
 * The routine may also initializes any caching mechanism. Thus, it must
 * be called before any other read or write access.
 *
 * \param dev    Specifies the file system device.
 * \param blkmnt Handle of the block device's partition mount.
 *
 * \return 0 on success or -1 in case of an error.
 */
int PhatVolMount(NUTDEVICE * dev, NUTFILE * blkmnt, u_char part_type)
{
    PHATVOL *vol;
    PHATVBR *vbr;
    BLKPAR_INFO pari;
    NUTDEVICE *blkdev = blkmnt->nf_dev;

    /*
     * Allocate the volume information structure 
     */
    if ((vol = malloc(sizeof(PHATVOL))) == 0) {
        errno = ENOMEM;
        return -1;
    }
    memset(vol, 0, sizeof(PHATVOL));

    /*
     * Determine the PHAT type.
     */
    switch (part_type) {
    case PTYPE_FAT32:
    case PTYPE_FAT32_LBA:
        vol->vol_type = 32;
        break;
    case PTYPE_FAT16:
    case PTYPE_FAT16_BIG:
    case PTYPE_FAT16_LBA:
        vol->vol_type = 16;
        break;
    case PTYPE_FAT12:
        vol->vol_type = 12;
        break;
    }
    if (vol->vol_type == 0) {
        /* Unknown partition type. */
        PhatVolUnmount(dev);
        errno = ENODEV;
        return -1;
    }

    /*
     * Query information from the block device driver.
     */
    pari.par_nfp = blkmnt;
    if ((*blkdev->dev_ioctl) (blkdev, NUTBLKDEV_INFO, &pari)) {
        PhatVolUnmount(dev);
        errno = ENODEV;
        return -1;
    }
    vol->vol_buf = pari.par_blkbp;

    /*
     * We use PhatSectorRead() instead of PhatSectorLoad() for our 
     * very first read to properly initialize the caching status.
     */
    if (PhatSectorRead(blkmnt, 0, vol->vol_buf)) {
        PhatVolUnmount(dev);
        return -1;
    }
    vol->vol_bufsect = 0;
    vbr = (PHATVBR *) vol->vol_buf;

    /* Convert to the PHAT32 layout. */
    if (vol->vol_type != 32) {
        memcpy(&vbr->boot_drive, &vbr->bios_tabsz_big, 26);
        memset(&vbr->bios_tabsz_big, 0, 28);
    }
#ifdef NUTDEBUG
    PhatDbgVbr(stdout, "Volume Boot Record", vbr);
#endif

    /*
     * Verify the VBR signature.
     */
    if (vol->vol_buf[510] != 0x55 || vol->vol_buf[511] != 0xAA) {
        errno = ENODEV;
        PhatVolUnmount(dev);
        return -1;
    }

    /*
     * Make sure we got a valid media type.
     */
    if (vbr->bios_media != 0xF0 && vbr->bios_media < 0xF8) {
        errno = ENODEV;
        PhatVolUnmount(dev);
        return -1;
    }

    /*
     * Examine the informations found in the boot record.
     */
    /* Bytes per sector. */
    vol->vol_sectsz = vbr->bios_sectsz;
    if (vol->vol_sectsz < 512 || vol->vol_sectsz & 0xFF) {
        errno = ENODEV;
        PhatVolUnmount(dev);
        return -1;
    }
    /* Sectors per cluster. */
    if ((vol->vol_clustsz = vbr->bios_clustsz) == 0) {
        errno = ENODEV;
        PhatVolUnmount(dev);
        return -1;
    }
    /* Allocation table size and position. */
    if (vbr->bios_tabsz) {
        vol->vol_tabsz = vbr->bios_tabsz;
    } else {
        vol->vol_tabsz = vbr->bios_tabsz_big;
    }
    vol->vol_tab_sect[0] = vbr->bios_rsvd_sects;
    if (vbr->bios_ntabs > 1) {
        vol->vol_tab_sect[1] = vol->vol_tab_sect[0] + vol->vol_tabsz;
    }
    /* Root directory size and position. */
    vol->vol_rootsz = (vbr->bios_rootsz * sizeof(PHATDIRENT) +  /* */
                       vol->vol_sectsz - 1) / vol->vol_sectsz;
    vol->vol_root_sect = vbr->bios_rsvd_sects + /* */
        vbr->bios_ntabs * vol->vol_tabsz;
    if (vol->vol_type == 32) {
        vol->vol_root_clust = vbr->bios_root_clust;
    }
    /* First data sector and number of data clusters. */
    vol->vol_data_sect = vol->vol_root_sect + vol->vol_rootsz;
    if (vbr->bios_volsz) {
        vol->vol_last_clust = vbr->bios_volsz - vol->vol_data_sect;
    } else {
        vol->vol_last_clust = vbr->bios_volsz_big - vol->vol_data_sect;
    }
    vol->vol_last_clust /= vol->vol_clustsz;
    /* First cluster number is 2. */
    vol->vol_last_clust += 2;

    dev->dev_dcb = vol;
    dev->dev_icb = blkmnt;

    vol->vol_numfree = PhatCountFreeClusters(dev);

    return 0;
}

/*!
 * \brief Unmount a PHAT volume.
 *
 * This routine is called by the block device driver while unmounting a
 * partition.
 *
 * \param dev Specifies the file system device.
 *
 * \return 0 on success or -1 in case of an error.
 */
int PhatVolUnmount(NUTDEVICE * dev)
{
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    if (vol) {
        free(vol);
    }
    return 0;
}

/*
 * \brief Get first sector of a specified cluster.
 *
 * \param nfp
 * \param clust
 */
u_long PhatClusterSector(NUTFILE * nfp, u_long clust)
{
    NUTDEVICE *dev = nfp->nf_dev;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    /*
     * If the file descriptor specifies the root directory, then the first
     * sector is located after the reserved sectors and the allocation table.
     */
    if (IsFixedRootDir(nfp)) {
        return vol->vol_root_sect;
    }

    /*
     * For all other files/directories the sector is located in the data
     * area of the volume.
     */
    if (clust >= 2) {
        clust -= 2;
    }
    return vol->vol_data_sect + clust * vol->vol_clustsz;
}

/*@}*/
