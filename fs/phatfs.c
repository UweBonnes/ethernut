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
 * \file fs/phatfs.c
 * \brief PHAT File System.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/01/05 16:31:39  haraldkipp
 * First check-in.
 *
 *
 * \endverbatim
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <fs/fs.h>

#include <dirent.h>

#include <dev/blockdev.h>

#include <fs/phatfs.h>
#include <fs/phatvol.h>
#include <fs/phatio.h>
#include <fs/phatutil.h>
#include <fs/phatdir.h>

#if 0
/* Use for local debugging. */
#define NUTDEBUG
#include <stdio.h>
#include <fs/phatdbg.h>
#endif

/*!
 * \addtogroup xgPhatFs
 */
/*@{*/

/*!
 * \brief Search free cluster.
 *
 * \param dev   Specifies the file system device.
 * \param first First cluster to check. Must not be lower than 2.
 * \param last  Last cluster to check.
 *
 * \return Number of the free cluster found. If an error occured,  the
 *         returned value will be lower than the first cluster searched.
 */
static u_long SearchFreeCluster(NUTDEVICE * dev, u_long first, u_long last)
{
    int rc = -1;
    u_long clust;
    u_long link = 1;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    if (vol->vol_type == 32) {
        for (clust = first; clust < last; clust++) {
            if ((rc = Phat32GetClusterLink(dev, clust, &link)) < 0 || link == 0) {
                break;
            }
        }
    } else if (vol->vol_type == 16) {
        for (clust = first; clust < last; clust++) {
            if ((rc = Phat16GetClusterLink(dev, clust, &link)) < 0 || link == 0) {
                break;
            }
        }
    } else {
        for (clust = first; clust < last; clust++) {
            if ((rc = Phat12GetClusterLink(dev, clust, &link)) < 0 || link == 0) {
                break;
            }
        }
    }

    if (rc || link) {
        return 0;
    }
    return clust;
}

/*!
 * \brief Allocate a new cluster.
 *
 * \param dev Specifies the file system device.
 *
 * \return Number of the allocated cluster, which is not lower than 2.
 *         Any value lower than 2 indicates an error.
 */
static u_long AllocCluster(NUTDEVICE * dev)
{
    u_long clust;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    /*
     * If the hint for the next free cluster is invalid, start from the beginning,
     */
    if (vol->vol_nxtfree < 2 || vol->vol_nxtfree >= vol->vol_last_clust) {
        vol->vol_nxtfree = 2;
    }
    if ((clust = SearchFreeCluster(dev, vol->vol_nxtfree, vol->vol_last_clust)) < 2) {
        if ((clust = SearchFreeCluster(dev, 2, vol->vol_nxtfree)) < 2) {
            vol->vol_nxtfree = 0;
            errno = ENOSPC;
            return 0;
        }
    }
    vol->vol_nxtfree = clust;

    return clust;
}

/*!
 * \brief Allocate the first cluster of a file.
 *
 * \param  The file for which a cluster is allocated.
 *
 * \return Number of the allocated cluster, which is not lower than 2.
 *         Any value lower than 2 indicates an error.
 */
u_long AllocFirstCluster(NUTFILE * nfp)
{
    u_long clust;
    NUTDEVICE *dev = nfp->nf_dev;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;
    PHATFILE *fcb = nfp->nf_fcb;

    if ((clust = AllocCluster(dev)) < 2) {
        return 0;
    }

    fcb->f_dirent.dent_clusthi = (u_short) (clust >> 16);
    fcb->f_dirent.dent_clust = (u_short) clust;
    fcb->f_de_dirty = 1;

    /* The first cluster entry will be set to EOC. */
    if (vol->vol_type == 32) {
        if (Phat32SetClusterLink(dev, clust, PHAT32CMASK)) {
            return 0;
        }
    } else if (vol->vol_type == 16) {
        if (Phat16SetClusterLink(dev, clust, PHAT16CMASK)) {
            return 0;
        }
    } else if (Phat12SetClusterLink(dev, clust, PHAT12CMASK)) {
        return 0;
    }
    vol->vol_numfree--;

    return clust;
}

/*!
 * \brief Allocate the next cluster of a file.
 *
 * \param  The file for which a cluster is allocated.
 *
 * \return Number of the allocated cluster, which is not lower than 2.
 *         Any value lower than 2 indicates an error.
 */
static u_long AllocNextCluster(NUTFILE * nfp)
{
    u_long clust;
    NUTDEVICE *dev = nfp->nf_dev;
    PHATFILE *fcb = nfp->nf_fcb;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    /* Allocate a free cluster. */
    if ((clust = AllocCluster(dev)) < 2) {
        return 0;
    }

    /* Link the previous cluster to the new one and set
     * the entry of the new one to EOC.
     */
    if (vol->vol_type == 32) {
        if (Phat32SetClusterLink(dev, fcb->f_clust, clust)) {
            return 0;
        }
        if (Phat32SetClusterLink(dev, clust, PHAT32CMASK)) {
            return 0;
        }
    } else if (vol->vol_type == 16) {
        if (Phat16SetClusterLink(dev, fcb->f_clust, clust)) {
            return 0;
        }
        if (Phat16SetClusterLink(dev, clust, PHAT16CMASK)) {
            return 0;
        }
    } else if (Phat12SetClusterLink(dev, fcb->f_clust, clust)) {
        return 0;
    } else if (Phat12SetClusterLink(dev, clust, PHAT12CMASK)) {
        return 0;
    }
    vol->vol_numfree--;

    return clust;
}

/*!
 * \brief Flush file buffers.
 *
 * \param nfp Specifies the file.
 *
 * \return 0 on success, -1 otherwise.
 */
static int PhatFileFlush(NUTFILE * nfp)
{
    /* Update the file's directory entry. */
    if (PhatDirEntryUpdate(nfp) < 0) {
        return -1;
    }
    /* Flush sector buffers. */
    return PhatSectorFlush(nfp->nf_dev);
}

/*!
 * \brief Close a file.
 *
 * \param nfp File descriptor.
 *
 * \return 0 on success, -1 otherwise.
 */
static int PhatFileClose(NUTFILE * nfp)
{
    int rc;

    if (nfp == NULL) {
        errno = EBADF;
        return -1;
    }
#ifdef NUTDEBUG
    PhatDbgFileInfo(stdout, "Close file", (PHATFILE *) nfp->nf_fcb);
#endif
    rc = PhatFileFlush(nfp);
    if (nfp->nf_fcb) {
        free(nfp->nf_fcb);
    }
    free(nfp);

    return rc;
}

/*!
 * \brief Open a file.
 *
 * This function is called by the low level open routine of the C runtime 
 * library, using the _NUTDEVICE::dev_open entry.
 *
 * \param dev  Specifies the file system device.
 * \param path Pathname of the file to open. If the last character is a
 *             slash, then a directory will be opened.
 * \param mode Operation mode.
 * \param acc  File attribute.
 *
 * \return Pointer to a NUTFILE structure if successful or NUTFILE_EOF otherwise.
 */
static NUTFILE *PhatFileOpen(NUTDEVICE * dev, CONST char *path, int mode, int acc)
{
    NUTFILE *nfp = NUTFILE_EOF;
    NUTFILE *ndp;
    PHATFILE *ffcb;
    PHATFILE *dfcb;
    PHATFIND *srch;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;
    char *parent;
    CONST char *fname;
    u_long clust;

    /* Chop off the file name, if one is specified. */
    if ((parent = GetParentPath(path, &fname)) == NULL) {
        return NUTFILE_EOF;
    }

    /* Open the target directory. */
    ndp = PhatDirOpen(dev, parent);
    free(parent);
    if (ndp == NUTFILE_EOF) {
        return NUTFILE_EOF;
    }

    /*
     * We successfully opened the directory. If no file name had been specified,
     * then the caller wants to open the directory itself. In this case, simply
     * return the NUTFILE for it.
     */
    dfcb = ndp->nf_fcb;
    if (*fname == 0) {
        dfcb->f_mode = mode;
        return ndp;
    }

    /*
     * Allocate a file information and a temporary search structure.
     */
    nfp = malloc(sizeof(NUTFILE));
    ffcb = malloc(sizeof(PHATFILE));
    srch = malloc(sizeof(PHATFIND));
    if (nfp == NULL || ffcb == NULL || srch == NULL) {
        PhatFileClose(ndp);
        if (nfp) {
            free(nfp);
        }
        if (ffcb) {
            free(ffcb);
        }
        if (srch) {
            free(srch);
        }
        errno = ENOMEM;
        return NUTFILE_EOF;
    }

    memset(ffcb, 0, sizeof(PHATFILE));
    nfp->nf_next = 0;
    nfp->nf_dev = dev;
    nfp->nf_fcb = ffcb;

    /*
     * Check if the specified file already exists.
     *
     * Note, that directories are handled differently in PhatDirCreate(),
     * where the first cluster is initialized with zeros.
     */
    if (PhatDirEntryFind(ndp, fname, PHAT_FATTR_FILEMASK, srch)) {
        /* 
         * File doesn't exist. Does the opening mode allow to create 
         * a new file? 
         */
        if ((mode & _O_CREAT) == 0) {
            free(srch);
            PhatFileClose(ndp);
            PhatFileClose(nfp);
            errno = ENOENT;
            return NUTFILE_EOF;
        }
        /* Create a new directory entry. */
        if (PhatDirEntryCreate(ndp, fname, acc, &ffcb->f_dirent)) {
            free(srch);
            PhatFileClose(ndp);
            PhatFileClose(nfp);
            return NUTFILE_EOF;
        }
        ffcb->f_de_dirty = 1;
#ifdef NUTDEBUG
        PhatDbgFileInfo(stdout, "New entry", ffcb);
#endif
    } else {
        /*
         * We return an error, if the file exists and _O_EXCL has been
         * set with _O_CREAT.
         */
        if ((mode & (_O_CREAT | _O_EXCL)) == (_O_CREAT | _O_EXCL)) {
            free(srch);
            PhatFileClose(ndp);
            PhatFileClose(nfp);
            errno = EEXIST;
            return NUTFILE_EOF;
        }
#ifdef NUTDEBUG
        PhatDbgFileInfo(stdout, "Existing entry", ffcb);
#endif
        /*
         * Truncate an existing file.
         */
        if (mode & _O_TRUNC) {
            /*
             * Relase all clusters allocated by this entry.
             */
            clust = srch->phfind_ent.dent_clusthi;
            clust <<= 16;
            clust += srch->phfind_ent.dent_clust;
            if (clust) {
                if (vol->vol_type == 32) {
                    if (Phat32ReleaseChain(dev, clust)) {
                        PhatFileClose(ndp);
                        PhatFileClose(nfp);
                        free(srch);
                        return NUTFILE_EOF;
                    }
                } else if (vol->vol_type == 16) {
                    if (Phat16ReleaseChain(dev, clust)) {
                        PhatFileClose(ndp);
                        PhatFileClose(nfp);
                        free(srch);
                        return NUTFILE_EOF;
                    }
                } else if (Phat12ReleaseChain(dev, clust)) {
                    PhatFileClose(ndp);
                    PhatFileClose(nfp);
                    free(srch);
                    return NUTFILE_EOF;
                }
            }
            memset(ffcb, 0, sizeof(PHATFILE));
            memcpy(ffcb->f_dirent.dent_name, srch->phfind_ent.dent_name, sizeof(ffcb->f_dirent.dent_name));
            ffcb->f_dirent.dent_attr = srch->phfind_ent.dent_attr;
            ffcb->f_dirent.dent_rsvdnt = srch->phfind_ent.dent_rsvdnt;
            ffcb->f_dirent.dent_ctsecs = srch->phfind_ent.dent_ctsecs;
            ffcb->f_dirent.dent_ctime = srch->phfind_ent.dent_ctime;
            ffcb->f_dirent.dent_cdate = srch->phfind_ent.dent_cdate;
            ffcb->f_de_dirty = 1;
        }
        /*
         * Append to an existing file.
         */
        else {
            ffcb->f_dirent = srch->phfind_ent;
            PhatFilePosSet(nfp, ffcb->f_dirent.dent_fsize + 1);
        }
    }
    free(srch);

    /* Store position of our directory entry. */
    ffcb->f_de_sect = PhatClusterSector(ndp, dfcb->f_clust) + dfcb->f_clust_pos;
    ffcb->f_de_offs = dfcb->f_sect_pos - 32;
    /* Store first cluster of parent. */
    ffcb->f_pde_clusthi = dfcb->f_dirent.dent_clusthi;
    ffcb->f_pde_clust = dfcb->f_dirent.dent_clust;
    /* Set the current cluster. */
    ffcb->f_clust = ffcb->f_dirent.dent_clusthi;
    ffcb->f_clust <<= 16;
    ffcb->f_clust += ffcb->f_dirent.dent_clust;
    /* Store the opening mode. */
    ffcb->f_mode = mode;

    /* Close the directory. */
    PhatFileClose(ndp);

#ifdef NUTDEBUG
    PhatDbgFileInfo(stdout, "File opened", ffcb);
#endif

    return nfp;
}

/*!
 * \brief Write data to a file.
 *
 * \param nfp    Pointer to a \ref NUTFILE structure, obtained by a previous 
 *               call to PnutFileOpen().
 * \param buffer Pointer to the data to be written. If zero, then the
 *               output buffer will be flushed.
 * \param len    Number of bytes to write.
 *
 * \return The number of bytes written. A return value of -1 indicates an 
 *         error.
 */
static int PhatFileWrite(NUTFILE * nfp, CONST void *buffer, int len)
{
    int rc;
    int step;
    u_long clust;
    u_char *buf = (u_char *) buffer;
    NUTDEVICE *dev = nfp->nf_dev;
    PHATFILE *fcb = nfp->nf_fcb;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    /*
     * Refuse to write to files with RDONLY attribute set.
     */
    if (fcb->f_dirent.dent_attr & PHAT_FATTR_RDONLY) {
        errno = EACCES;
        return -1;
    }

    /*
     * In case of normal files, check for sufficient space.
     */
    if ((fcb->f_dirent.dent_attr & PHAT_FATTR_DIR) == 0) {
        /* Bytes per cluster. */
        u_long num = vol->vol_sectsz * vol->vol_clustsz;
        /* Number of clusters already used. */
        u_long cur = (fcb->f_dirent.dent_fsize + num - 1) / num;

        /* Number of clusters used after writing. */
        num = (fcb->f_pos + len + num - 1) / num;
        /* If additional clusters are required, are they available? */
        if (num > cur && num - cur > vol->vol_numfree) {
            errno = ENOSPC;
            return -1;
        }

        /* If the file is empty, allocate the first cluster. */
        if (fcb->f_dirent.dent_fsize == 0) {
            if ((clust = AllocFirstCluster(nfp)) < 2) {
                return -1;
            }
            fcb->f_clust_prv = clust;
            fcb->f_clust = clust;
        }
    }

    /*
     * Write the data.
     */
    for (rc = 0, step = 0; rc < len; rc += step) {
        /* Did we reach the end of a sector? */
        if (fcb->f_sect_pos >= vol->vol_sectsz) {
            /* Move to the next sector within the cluster. */
            fcb->f_sect_pos = 0;
            fcb->f_clust_pos++;
            if (!IsFixedRootDir(nfp)) {
                if (fcb->f_clust_pos >= vol->vol_clustsz) {
                    /* Move to the next cluster. */
                    if (vol->vol_type == 32) {
                        if (Phat32GetClusterLink(dev, fcb->f_clust, &clust)) {
                            rc = -1;
                            break;
                        }
                        if (clust >= (PHATEOC & PHAT32CMASK)) {
                            if ((clust = AllocNextCluster(nfp)) < 2) {
                                rc = -1;
                                break;
                            }
                        }
                    } else if (vol->vol_type == 16) {
                        if (Phat16GetClusterLink(dev, fcb->f_clust, &clust)) {
                            rc = -1;
                            break;
                        }
                        if (clust >= (PHATEOC & PHAT16CMASK)) {
                            if ((clust = AllocNextCluster(nfp)) < 2) {
                                rc = -1;
                                break;
                            }
                        }
                    } else if (Phat12GetClusterLink(dev, fcb->f_clust, &clust)) {
                        rc = -1;
                        break;
                    } else if (clust >= (PHATEOC & PHAT12CMASK)) {
                        if ((clust = AllocNextCluster(nfp)) < 2) {
                            rc = -1;
                            break;
                        }
                    }
                    fcb->f_clust_pos = 0;
                    fcb->f_clust_prv = fcb->f_clust;
                    fcb->f_clust = clust;
                }
            }
        }
        /* Load the sector we want to write to. */
        if (PhatSectorLoad(nfp->nf_dev, PhatClusterSector(nfp, fcb->f_clust) + fcb->f_clust_pos)) {
            rc = -1;
            break;
        }
        /* The number of bytes we write to this sector. */
        step = (int) (vol->vol_sectsz - fcb->f_sect_pos);
        if (step > len - rc) {
            step = len - rc;
        }
        /* Copy data to this sector. */
        memcpy(&vol->vol_buf[fcb->f_sect_pos], &buf[rc], step);
        vol->vol_bufdirty = 1;
        /* Advance file pointers. */
        fcb->f_pos += step;
        fcb->f_sect_pos += step;
    }

    if (rc > 0) {
        /*
         * Update directory entry. Note that directory entries of directories
         * are never updated in a PHAT file system.
         */
        if ((fcb->f_dirent.dent_attr & PHAT_FATTR_DIR) == 0) {
            GetDosTimeStamp(&fcb->f_dirent.dent_mtime, &fcb->f_dirent.dent_mdate);
            fcb->f_dirent.dent_adate = fcb->f_dirent.dent_mdate;
            fcb->f_dirent.dent_attr |= PHAT_FATTR_ARCHIV;
            fcb->f_dirent.dent_fsize += rc;
            fcb->f_de_dirty = 1;
        }
    }
    return rc;
}

#ifdef __HARVARD_ARCH__
/*! 
 * \brief Write data from program space to a file.
 *
 * This function is not yet implemented and will always return -1.
 *
 * Similar to PhatFileWrite() except that the data is located in 
 * program memory.
 *
 * \param nfp    Pointer to a \ref NUTFILE structure, obtained by a previous 
 *               call to PnutFileOpen().
 * \param buffer Pointer to the data in program space. If zero, then the
 *               output buffer will be flushed.
 * \param len    Number of bytes to write.
 *
 * \return The number of bytes written. A return value of -1 indicates an 
 *         error.
 */
static int PhatFileWrite_P(NUTFILE * nfp, PGM_P buffer, int len) return -1;
}
#endif

/*!
 * \brief Read data from a file.
 *
 * \param nfp    Pointer to a ::NUTFILE structure, obtained by a previous 
 *               call to PnutFileOpen().
 * \param buffer Pointer to the data buffer to fill.
 * \param len    Maximum number of bytes to read.
 *
 * \return The number of bytes actually read. A return value of -1 indicates 
 *         an error.
 */
static int PhatFileRead(NUTFILE * nfp, void *buffer, int size)
{
    int rc;
    int step;
    u_char *buf = (u_char *) buffer;
    NUTDEVICE *dev = nfp->nf_dev;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;
    PHATFILE *fcb = nfp->nf_fcb;

    /* Respect the end of normal files. */
    if ((fcb->f_dirent.dent_attr & PHAT_FATTR_DIR) == 0) {
        if (fcb->f_pos + size >= fcb->f_dirent.dent_fsize) {
            size = fcb->f_dirent.dent_fsize - fcb->f_pos;
        }
    }
    for (rc = 0, step = 0; rc < size; rc += step) {

        /* Did we reach the end of a sector? */
        if (fcb->f_sect_pos >= vol->vol_sectsz) {
            /* Move to the next sector within the cluster. */
            fcb->f_sect_pos = 0;
            fcb->f_clust_pos++;

            if (!IsFixedRootDir(nfp)) {
                /* Did we reach the last sector of this cluster? */
                if (fcb->f_clust_pos >= vol->vol_clustsz) {
                    /* Move to the next cluster. */
                    fcb->f_clust_pos = 0;
                    fcb->f_clust_prv = fcb->f_clust;

                    if (vol->vol_type == 32) {
                        if (Phat32GetClusterLink(dev, fcb->f_clust, &fcb->f_clust)) {
                            break;
                        }
                    } else if (vol->vol_type == 16) {
                        if (Phat16GetClusterLink(dev, fcb->f_clust, &fcb->f_clust)) {
                            break;
                        }
                    } else if (Phat12GetClusterLink(dev, fcb->f_clust, &fcb->f_clust)) {
                        break;
                    }
                }
            }
        }

        /* Make sure that the required sector is loaded. */
        if (PhatSectorLoad(nfp->nf_dev, PhatClusterSector(nfp, fcb->f_clust) + fcb->f_clust_pos)) {
            rc = -1;
            break;
        }
        step = (int) (vol->vol_sectsz - fcb->f_sect_pos);
        if (step > size - rc) {
            step = size - rc;
        }
        memcpy(&buf[rc], &vol->vol_buf[fcb->f_sect_pos], step);
        fcb->f_pos += step;
        fcb->f_sect_pos += step;
    }
    return rc;
}

/*!
 * \brief File system specific functions.
 * \param dev Specifies the file system device.
 */
static int PhatIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    int rc = -1;

    switch (req) {
    case FS_STATUS:
        {
            FSCP_STATUS *par = (FSCP_STATUS *) conf;

            rc = PhatDirEntryStatus(dev, par->par_path, par->par_stp);
        }
        break;
    case FS_DIR_CREATE:
        rc = PhatDirCreate(dev, (char *) conf);
        break;
    case FS_DIR_REMOVE:
        rc = PhatDirRemove(dev, (char *) conf);
        break;
    case FS_DIR_OPEN:
        /* Open a directory for reading entries. */
        {
            DIR *dir = (DIR *) conf;

            if ((dir->dd_fd = PhatDirOpen(dev, dir->dd_buf)) != NUTFILE_EOF) {
                rc = 0;
            }
        }
        break;
    case FS_DIR_CLOSE:
        rc = PhatFileClose(((DIR *) conf)->dd_fd);
        break;
    case FS_DIR_READ:
        rc = PhatDirRead((DIR *) conf);
        break;
    case FS_FILE_STATUS:
        /* TODO */
        break;
    case FS_FILE_DELETE:
        rc = PhatDirDelEntry(dev, (char *) conf, PHAT_FATTR_FILEMASK);
        break;
    case FS_FILE_SEEK:
        /* TODO */
        break;
    case FS_RENAME:
        /* Rename an existing file or directory. */
        {
            FSCP_RENAME *par = (FSCP_RENAME *) conf;

            rc = PhatDirRenameEntry(dev, par->par_old, par->par_new);
        }
        break;

    case FS_VOL_MOUNT:
        {
            /* Mount a volume. */
            FSCP_VOL_MOUNT *par = (FSCP_VOL_MOUNT *) conf;

            rc = PhatVolMount(dev, par->fscp_bmnt, par->fscp_part_type);
        }
        break;
    case FS_VOL_UNMOUNT:
        /* Unmount a volume. */
        rc = PhatVolUnmount(dev);
        break;
    }
    return rc;
}

/*!
 * \brief Initialize the PHAT file system driver.
 *
 * This routine is called during device registration.
 *
 * \param dev Specifies the file system device.
 *
 * \return Zero on success. Otherwise an error code is returned.
 */
static int PhatInit(NUTDEVICE * dev)
{
    /* Nothing to do. */
    return 0;
}

/*!
 * \brief PHAT file system driver information structure.
 *
 * A pointer to this structure must be passed to NutRegisterDevice() 
 * to bind this file system driver to the Nut/OS kernel.
 * An application may then call
 * /verbatim
 * _open("MMC0:1/PHAT0", _O_RDWR | _O_BINARY);
 * /endverbatim
 * to mount partition 1 on the previously registered block
 * device (devMmc0 in this example).
 */
NUTDEVICE devPhat0 = {
    0,                          /*!< Pointer to next device, dev_next. */
    {'P', 'H', 'A', 'T', '0', 0, 0, 0, 0}
    ,                           /*!< Unique device name, dev_name. */
    IFTYP_FS,                   /*!< Type of device, dev_type. Obsolete. */
    0,                          /*!< Base address, dev_base. Unused. */
    0,                          /*!< First interrupt number, dev_irq. Unused. */
    0,                          /*!< Mounted block device partition, dev_icb. */
    0,                          /*!< Volume information, dev_dcb. */
    PhatInit,                   /*!< Driver initialization routine, dev_init. */
    PhatIOCtl,                  /*!< Driver specific control function, dev_ioctl. */
    PhatFileRead,               /*!< Read data from a file, dev_read. */
    PhatFileWrite,              /*!< Write data to a file, dev_write. */
#ifdef __HARVARD_ARCH__
    PhatFileWrite_P,            /*!< Write data from program space to a file, dev_write_P. */
#endif
    PhatFileOpen,               /*!< Open a file, dev_open. */
    PhatFileClose,              /*!< Close a file, dev_close. */
    0                           /*!< Return file size, dev_size. */
};

/*@}*/
