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
 * \file fs/phatdir.c
 * \brief PHAT File System.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.2  2006/01/22 17:40:51  haraldkipp
 * Now mkdir() fails, if the directory exists already.
 * Now rmdir() returns an error when trying to delete subdirectories, which
 * are not empty.
 * Now PhatDirEntryStatus() sets correct errno value, if out of memory.
 *
 * Revision 1.1  2006/01/05 16:31:32  haraldkipp
 * First check-in.
 *
 *
 * \endverbatim
 */

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include <fs/fs.h>
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
 * \addtogroup xgPhatDir
 */
/*@{*/

/*! \brief Maximum number of directory entries. */
#define PHAT_MAXDIRENT   65536

/*!
 * \brief Allocate a directory entry.
 *
 * \param ndp   Parent directory of the new entry.
 * \param entry Information structure of the new entry is store here.
 * \param fname Name of the new entry.
 *
 * \return 0 on success, -1 otherwise.
 */
static int PhatDirEntryAlloc(NUTFILE * ndp, CONST char *fname, PHATDIRENT * entry)
{
    int rc = -1;
    int pos;
    int got;
    u_char sect;
    u_char *temp;
    NUTDEVICE *dev = ndp->nf_dev;
    PHATFILE *dfcb = ndp->nf_fcb;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;
    PHATDIRENT *dent;

    /* Convert the dotted name to a space filled one. */
    if (MakePhatName(fname, entry->dent_name)) {
        /* Malformed name or name contains wildcards. */
        errno = EINVAL;
        return -1;
    }

    /* Allocate a temporary entry buffer. */
    if ((dent = malloc(sizeof(PHATDIRENT))) == NULL) {
        errno = ENOMEM;
        return -1;
    }

    /*
     * Find a free entry, starting from the beginning.
     */
    PhatFilePosSet(ndp, 0);
    for (;;) {
        /* Memorize the current position and try to read the next entry. */
        pos = dfcb->f_pos;
        if ((got = (*dev->dev_read) (ndp, dent, sizeof(PHATDIRENT))) < 0) {
            /* Read failed. */
            break;
        }
        if (got != sizeof(PHATDIRENT)) {
            /*
             * End of directory reached. Try to expand it. 
             */
            if (IsFixedRootDir(ndp) ||  /* */
                dfcb->f_pos >= PHAT_MAXDIRENT * sizeof(PHATDIRENT)) {
                /* Failed. Either PHAT12/16 root dir or max. size reached. */
                errno = EFBIG;
                break;
            }

            /* Fill a new cluster with zeros. */
            if ((temp = malloc(vol->vol_sectsz)) == NULL) {
                errno = ENOMEM;
                break;
            }
            memset(temp, 0, vol->vol_sectsz);
            for (sect = vol->vol_clustsz; sect; sect--) {
                if ((*dev->dev_write) (ndp, temp, vol->vol_sectsz) < 0) {
                    /* Write failed. */
                    break;
                }
            }
            free(temp);
            /* End of directory reached and expanded by a new cluster. */
            if (sect == 0) {
                rc = 0;
            }
            break;
        }
        if (dent->dent_name[0] == PHAT_REM_DIRENT || dent->dent_name[0] == 0) {
            /* Empty entry found. */
            rc = 0;
            break;
        }
    }
    free(dent);

    if (rc == 0) {
        /* Return to the memorized position and write the new entry. */
        PhatFilePosSet(ndp, pos);
        if ((*dev->dev_write) (ndp, entry, sizeof(PHATDIRENT)) < 0) {
            /* Write error. */
            rc = -1;
        }
    }
    return rc;
}

/*!
 * \brief Release a directory entry.
 *
 * In the PHAT file system released entries will be marked with the
 * special character 0xE5 as the first character in the file name.
 *
 * \param ndp The parant directory of the entry that should be removed.
 * \param pos Byte position of that entry.
 *
 * \return 0 on success, -1 otherwsie.
 */
static int PhatDirEntryRelease(NUTFILE * ndp, u_long pos)
{
    NUTDEVICE *dev = ndp->nf_dev;
    u_char ch = PHAT_REM_DIRENT;

    PhatFilePosSet(ndp, pos);
    if ((*dev->dev_write) (ndp, &ch, 1) < 0) {
        return -1;
    }
    return 0;
}

/*!
 * \brief Create a new directory entry.
 *
 * \param ndp    Handle to the parent directory.
 * \param name   Name of the new entry.
 * \param acc    Attributes of the new entry.
 * \param dirent Information structure of the new entry.
 *
 * \return 0 on success. Otherwise -1 is returned.
 */
int PhatDirEntryCreate(NUTFILE * ndp, CONST char *name, int acc, PHATDIRENT * dirent)
{
    dirent->dent_attr = (u_char) acc;
    GetDosTimeStamp(&dirent->dent_ctime, &dirent->dent_cdate);
    dirent->dent_adate = dirent->dent_cdate;
    dirent->dent_mtime = dirent->dent_ctime;
    dirent->dent_mdate = dirent->dent_cdate;

    if (PhatDirEntryAlloc(ndp, name, dirent)) {
        return -1;
    }
#ifdef NUTDEBUG
    PhatDbgFileInfo(stdout, "New Dir Entry", ndp->nf_fcb);
#endif
    return 0;
}

/*!
 * \brief Update directory entry of an opened file or directory.
 *
 * \param nfp File of which the directory entry will be updated.
 *
 * \return 0 on success. Otherwise -1 is returned.
 */
int PhatDirEntryUpdate(NUTFILE * nfp)
{
    NUTDEVICE *dev = nfp->nf_dev;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;
    PHATFILE *fcb = nfp->nf_fcb;

    /*
     * The file may specify the root directory, in which case
     * the updated is skipped.
     */
    if (fcb->f_de_sect) {
        if (fcb->f_de_dirty) {
#ifdef NUTDEBUG
            PhatDbgDirEntry(stdout, "PhatDirEntryUpdate", &fcb->f_dirent);
#endif
            if (PhatSectorLoad(dev, fcb->f_de_sect)) {
                return -1;
            }
            memcpy(vol->vol_buf + fcb->f_de_offs, &fcb->f_dirent, sizeof(PHATDIRENT));
            vol->vol_bufdirty = 1;
            fcb->f_de_dirty = 0;
        }
    }
    return 0;
}

/*!
 * \brief Read the next directory entry.
 *
 * \param ndp  Specifies the directory to read.
 * \param srch Pointer to a structure which receives the result.
 *
 * \return 0 on success. Otherwise -1 is returned.
 */
static int PhatDirEntryRead(NUTFILE * ndp, PHATFIND * srch)
{
    PHATDIRENT *entry = &srch->phfind_ent;
    NUTDEVICE *dev = ndp->nf_dev;
    PHATFILE *fcb = ndp->nf_fcb;

    for (;;) {
        /* Read next entry. */
        if ((*dev->dev_read) (ndp, entry, sizeof(PHATDIRENT)) != sizeof(PHATDIRENT)) {
            return -1;
        }
        /* Skip volume IDs. */
        if ((entry->dent_attr & PHAT_FATTR_VOLID) == 0) {
            /* Skip removed entries. */
            if (entry->dent_name[0] != PHAT_REM_DIRENT) {
                if (entry->dent_name[0]) {
                    srch->phfind_pos = fcb->f_pos - sizeof(PHATDIRENT);
                    MakeVisibleName(entry->dent_name, srch->phfind_name);
                } else {
                    /* End marker found. */
                    srch->phfind_name[0] = 0;
                }
                break;
            }
        }
    }
    return 0;
}

/*!
 * \brief Find a directory entry with a specified name.
 *
 * \param ndp    Pointer to a previously opened directory.
 * \param spec   Name of the entry to retrieve.
 * \param attmsk Attribute mask. Entries with attributes not specified
 *               in this mask will be ignored.
 * \param srch   Optional pointer to a structure which receives the
 *               search result.
 *
 * \return 0 if an entry was found, otherwise -1 is returned.
 */
int PhatDirEntryFind(NUTFILE * ndp, CONST char *spec, u_long attmsk, PHATFIND * srch)
{
    int rc = -1;
    PHATFIND *temps;

    /* Allocate a temporary structure to store the search result. */
    if ((temps = malloc(sizeof(PHATFIND))) == NULL) {
        errno = ENOMEM;
        return -1;
    }

    /*
     * Loop until the specified entry was found or until we reach the
     * end of the directory.
     */
    PhatFilePosSet(ndp, 0);
    while ((rc = PhatDirEntryRead(ndp, temps)) == 0) {
        if (temps->phfind_name[0] == 0) {
            /* Reached the end of the directory. */
            rc = -1;
            break;
        }
        if ((temps->phfind_ent.dent_attr | attmsk) == attmsk) {
            if (stricmp(temps->phfind_name, spec) == 0) {
                /* Specified entry found. */
                if (srch) {
                    *srch = *temps;
                }
                break;
            }
        }
    }
    free(temps);

    return rc;
}

/*!
 * \brief Rename file.
 *
 * \param dev      Specifies the file system device.
 * \param old_path Old name and path of the file.
 * \param new_path New name and path of the file.
 *
 * \return 0 on success, -1 otherwise.
 */
int PhatDirRenameEntry(NUTDEVICE * dev, CONST char *old_path, CONST char *new_path)
{
    int rc;
    char *parent;
    CONST char *fname;
    NUTFILE *old_ndp;
    NUTFILE *new_ndp;
    PHATFIND *srch;

    /*
     * Open directory of the old file.
     */
    if ((parent = GetParentPath(old_path, &fname)) == NULL) {
        return -1;
    }
    old_ndp = (*dev->dev_open) (dev, parent, _O_RDWR, 0);
    free(parent);
    if (old_ndp == NUTFILE_EOF) {
        return -1;
    }

    if ((srch = malloc(sizeof(PHATFIND))) == NULL) {
        errno = ENOMEM;
    } else {
        if ((rc = PhatDirEntryFind(old_ndp, fname, PHAT_FATTR_FILEMASK, srch)) == 0) {
            rc = -1;

            if ((parent = GetParentPath(new_path, &fname)) == NULL) {
                errno = ENOMEM;
            } else {
                new_ndp = (*dev->dev_open) (dev, parent, _O_RDWR, 0);
                if (new_ndp != NUTFILE_EOF) {
                    if (PhatDirEntryFind(new_ndp, fname, PHAT_FATTR_FILEMASK, NULL) == 0) {
                        errno = EEXIST;
                    } else {
                        if ((rc = PhatDirEntryAlloc(new_ndp, fname, &srch->phfind_ent)) == 0) {
                            rc = PhatDirEntryRelease(old_ndp, srch->phfind_pos);
                        }
                    }
                    (*dev->dev_close) (new_ndp);
                }
                free(parent);
            }
        }
        free(srch);
    }
    (*dev->dev_close) (old_ndp);

    return rc;
}

/*!
 * \brief Remove a directory entry.
 *
 * Allocated clusters are released.
 *
 * \param dev   Specifies the file system device.
 * \param path  Name of the entry to remove.
 * \param flags Attributes of the entry to remove.
 *
 * \return 0 if successful. Otherwise returns an error code.
 */
int PhatDirDelEntry(NUTDEVICE * dev, CONST char *path, u_long flags)
{
    PHATFIND *srch;
    NUTFILE *ndp;
    u_long clust;
    char *parent;
    CONST char *fname;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    /*
     * Open the parent directory.
     */
    if ((parent = GetParentPath(path, &fname)) == NULL) {
        return -1;
    }
    ndp = (*dev->dev_open) (dev, parent, _O_RDWR, 0);
    free(parent);
    if (ndp == NUTFILE_EOF) {
        return -1;
    }

    /*
     * Find the specified file name.
     */
    if ((srch = malloc(sizeof(PHATFIND))) == NULL) {
        (*dev->dev_close) (ndp);
        errno = ENOMEM;
        return -1;
    }
    if (PhatDirEntryFind(ndp, fname, flags, srch)) {
        free(srch);
        (*dev->dev_close) (ndp);
        errno = ENOENT;
        return -1;
    }

    /*
     * Do not remove files with RDONLY attribute.
     */
    if (srch->phfind_ent.dent_attr & PHAT_FATTR_RDONLY) {
        free(srch);
        (*dev->dev_close) (ndp);
        errno = EACCES;
        return -1;
    }

    /*
     * Relase all clusters allocated by this entry.
     */
    clust = srch->phfind_ent.dent_clusthi;
    clust <<= 16;
    clust += srch->phfind_ent.dent_clust;
    if (clust) {
        if (vol->vol_type == 32) {
            if (Phat32ReleaseChain(dev, clust)) {
                free(srch);
                (*dev->dev_close) (ndp);
                return -1;
            }
        } else if (vol->vol_type == 16) {
            if (Phat16ReleaseChain(dev, clust)) {
                free(srch);
                (*dev->dev_close) (ndp);
                return -1;
            }
        } else if (Phat12ReleaseChain(dev, clust)) {
            free(srch);
            (*dev->dev_close) (ndp);
            return -1;
        }
    }

    /*
     * Release the directory entry itself.
     */
    if (PhatDirEntryRelease(ndp, srch->phfind_pos)) {
        return -1;
    }
    free(srch);
    return (*dev->dev_close) (ndp);
}

/*!
 * \brief Open a directory.
 *
 * \param dev   Specifies the file system device.
 * \param dpath Full absolute pathname of the directory to open.
 *
 * \return Pointer to a NUTFILE structure if successful or NUTFILE_EOF otherwise.
 */
NUTFILE *PhatDirOpen(NUTDEVICE * dev, char *dpath)
{
    NUTFILE *ndp;
    PHATFILE *dfcb;
    PHATFIND *srch;
    char *comp;
    char *cp;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    /* Make sure the volume is mounted. */
    if (vol == NULL) {
        errno = ENODEV;
        return NUTFILE_EOF;
    }

    /* Allocate the structure to return. */
    if ((ndp = malloc(sizeof(NUTFILE))) == NULL) {
        errno = ENOMEM;
        return NUTFILE_EOF;
    }
    if ((ndp->nf_fcb = malloc(sizeof(PHATFILE))) == NULL) {
        free(ndp);
        errno = ENOMEM;
        return NUTFILE_EOF;
    }
    memset(ndp->nf_fcb, 0, sizeof(PHATFILE));
    ndp->nf_next = NULL;
    ndp->nf_dev = dev;

    dfcb = ndp->nf_fcb;
    dfcb->f_dirent.dent_attr = PHAT_FATTR_DIR;
    dfcb->f_mode = _O_RDONLY;

    /* We start at the root directory. */
    dfcb->f_clust = vol->vol_root_clust;
    dfcb->f_dirent.dent_clusthi = (u_short) (vol->vol_root_clust >> 16);
    dfcb->f_dirent.dent_clust = (u_short) vol->vol_root_clust;

    if (*dpath == '/') {
        dpath++;
    }
    if (*dpath) {
        /*
         * We are looking for a subdirectory.
         */
        if ((comp = malloc(PHAT_MAX_NAMELEN + 1)) == NULL) {
            errno = ENOMEM;
            free(dfcb);
            free(ndp);
            return NUTFILE_EOF;
        }
        if ((srch = malloc(sizeof(PHATFIND))) == NULL) {
            errno = ENOMEM;
            free(comp);
            free(dfcb);
            free(ndp);
            return NUTFILE_EOF;
        }

        /*
         * Walk down the path.
         */
        while (*dpath) {
            /* Chop off the next component. */
            cp = comp;
            while (*dpath) {
                if (*dpath == '/') {
                    dpath++;
                    break;
                }
                *cp++ = *dpath++;
            }
            *cp = 0;

            /* Search component's entry in the current directory. */
            if (PhatDirEntryFind(ndp, comp, PHAT_FATTR_FILEMASK, srch)) {
                errno = ENOENT;
                free(dfcb);
                free(ndp);
                ndp = NUTFILE_EOF;
                break;
            }

            /*
             * Next component found. Mimic the open by updating the existing
             * file control block structure.
             */
            dfcb->f_de_sect = PhatClusterSector(ndp, dfcb->f_clust) + dfcb->f_clust_pos;
            dfcb->f_de_offs = dfcb->f_sect_pos - 32;

            /* Set the cluster of our directory entry. */
            dfcb->f_pde_clusthi = dfcb->f_dirent.dent_clusthi;
            dfcb->f_pde_clust = dfcb->f_dirent.dent_clust;

            dfcb->f_dirent = srch->phfind_ent;
#ifdef NUTDEBUG
            PhatDbgFileInfo(stdout, "Component", dfcb);
#endif

            /*
             * Handle root directory.
             */
            if (dfcb->f_dirent.dent_attr & PHAT_FATTR_DIR) {
                if (dfcb->f_dirent.dent_clust == 0 && dfcb->f_dirent.dent_clusthi == 0) {
                    if (vol->vol_type != 32) {
                        dfcb->f_de_sect = 0;
                    }
                    dfcb->f_dirent.dent_clusthi = (u_short) (vol->vol_root_clust >> 16);
                    dfcb->f_dirent.dent_clust = (u_short) vol->vol_root_clust;
                }
            }

            /*
             * Reset position.
             */
            PhatFilePosRewind(dfcb);
            dfcb->f_clust_prv = 0;
            dfcb->f_mode = _O_RDONLY;
        }
        free(comp);
    }
    return ndp;
}

/*!
 * \brief Read the next directory entry.
 *
 * \param dir Pointer to the internal directory information structure.
 *
 * \return 0 on success, -1 otherwise.
 */
int PhatDirRead(DIR * dir)
{
    PHATFIND *srch;
    struct dirent *ent;

    if ((srch = malloc(sizeof(PHATFIND))) == NULL) {
        errno = ENOMEM;
        return -1;
    }
    if (PhatDirEntryRead(dir->dd_fd, srch)) {
        return -1;
    }
#ifdef NUTDEBUG
    PhatDbgDirEntry(stdout, "Read entry", &srch->phfind_ent);
#endif

    ent = (struct dirent *) dir->dd_buf;
    ent->d_namlen = (u_char) strlen(srch->phfind_name);
    strcpy(ent->d_name, srch->phfind_name);

    return 0;
}

/*!
 * \brief Create a new subdirectory.
 *
 * One cluster is allocated, initialized to zero and two directory
 * entries are created, '.' and '..'.
 *
 * \param dev  Specifies the file system device.
 * \param path Full path to the directory.
 *
 * \return 0 on success. Otherwise -1 is returned.
 */
int PhatDirCreate(NUTDEVICE * dev, char *path)
{
    NUTFILE *ndp;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;
    PHATFILE *dfcb;
    PHATDIRENT *entry;
    u_char *buf;
    u_long sect;
    u_long clust;

    if ((ndp = (*dev->dev_open) (dev, path, _O_CREAT | _O_RDWR | _O_EXCL, PHAT_FATTR_DIR)) == NUTFILE_EOF) {
        return -1;
    }
    dfcb = ndp->nf_fcb;

    /*
     * Allocate a first cluster and initialize it with zeros.
     */
    if ((clust = AllocFirstCluster(ndp)) < 2) {
        (*dev->dev_close) (ndp);
        return -1;
    }
    dfcb->f_clust_prv = clust;
    dfcb->f_clust = clust;
    if ((buf = malloc(vol->vol_sectsz)) == NULL) {
        (*dev->dev_close) (ndp);
        errno = ENOMEM;
        return -1;
    }
    memset(buf, 0, vol->vol_sectsz);
    for (sect = vol->vol_clustsz; sect; sect--) {
        if ((*dev->dev_write) (ndp, buf, vol->vol_sectsz) < 0) {
            /* Write failed. */
            free(buf);
            (*dev->dev_close) (ndp);
            return -1;
        }
    }
    free(buf);

    /*
     * Write the dot entry.
     */
    entry = malloc(sizeof(PHATDIRENT));
    *entry = dfcb->f_dirent;
    memset(entry->dent_name, ' ', sizeof(entry->dent_name));
    entry->dent_name[0] = '.';
    PhatFilePosSet(ndp, 0);
    if ((*dev->dev_write) (ndp, entry, sizeof(PHATDIRENT)) != sizeof(PHATDIRENT)) {
        (*dev->dev_close) (ndp);
        free(entry);
        return -1;
    }

    /*
     * Write the double dot entry. If it points to the root cluster,
     * then the cluster number in the directory entry must be zero.
     */
    if ((u_short) vol->vol_root_clust == dfcb->f_pde_clust &&   /* */
        vol->vol_root_clust >> 16 == dfcb->f_pde_clusthi) {
        entry->dent_clust = 0;
        entry->dent_clusthi = 0;
    } else {
        entry->dent_clust = dfcb->f_pde_clust;
        entry->dent_clusthi = dfcb->f_pde_clusthi;
    }
    entry->dent_name[1] = '.';
    if ((*dev->dev_write) (ndp, entry, sizeof(PHATDIRENT)) != sizeof(PHATDIRENT)) {
        (*dev->dev_close) (ndp);
        free(entry);
        return -1;
    }
    free(entry);

    return (*dev->dev_close) (ndp);
}

/*!
 * \brief Remove a specified subdirectory.
 *
 * \param dev  Specifies the file system device.
 * \param path Full path to the directory.
 *
 * \return 0 on success. Otherwise -1 is returned.
 */
int PhatDirRemove(NUTDEVICE * dev, char *path)
{
    int rc = -1;
    PHATDIRENT *entry;
    NUTFILE *ndp;

    /* Never remove the root directory */
    if (path[0] == '/' && path[1] == 0) {
        errno = EBUSY;
        return -1;
    }

    if ((entry = malloc(sizeof(PHATDIRENT))) == NULL) {
        errno = ENOMEM;
        return -1;
    }

    /*
     * Make sure, that the directory we want to remove is empty. The dot 
     * and double dot entries are ignored.
     */
    if ((ndp = (*dev->dev_open) (dev, path, _O_RDONLY, 0)) != NUTFILE_EOF) {
        rc = 0;
        for (;;) {
            rc = (*dev->dev_read) (ndp, entry, sizeof(PHATDIRENT));
            if (rc < 0) {
                break;
            }
            /* Check for end of directory. */
            if (rc < sizeof(PHATDIRENT) || entry->dent_name[0] == 0) {
                rc = 0;
                break;
            }
            /* Skip removed entries. */
            if (entry->dent_name[0] == PHAT_REM_DIRENT) {
                continue;
            }
            /* Ignore entries which are not files. */
            if ((entry->dent_attr | PHAT_FATTR_FILEMASK) != PHAT_FATTR_FILEMASK) {
                continue;
            }
            /* Ignore dot and double dot entries. */
            if (entry->dent_name[0] == '.' &&   /* */
                (entry->dent_name[1] == '.' || entry->dent_name[1] == ' ')) {
                if (memcmp("         ", &entry->dent_name[2], 9) == 0) {
                    continue;
                }
            }
            errno = ENOTEMPTY;
            rc = -1;
            break;
        }
        (*dev->dev_close) (ndp);
    }
    free(entry);

    /* If the empty check was successful, then remove the entry. */
    if (rc == 0) {
        rc = PhatDirDelEntry(dev, path, PHAT_FATTR_DIR);
    }
    return rc;
}

/*!
 * \brief Retrieve status of a specified file.
 *
 * \param dev File system device.
 * \param path Path name to the file to query.
 * \param stp  Pointer to a structure which receives the result.
 *
 * return 0 on success, -1 otherwise.
 */
int PhatDirEntryStatus(NUTDEVICE * dev, CONST char *path, struct stat *stp)
{
    int rc;
    char *parent;
    CONST char *fname;
    NUTFILE *ndp;
    PHATFIND *srch;
    u_int val;

    /*
     * Open directory of the old file.
     */
    if ((parent = GetParentPath(path, &fname)) == NULL) {
        errno = EINVAL;
        return -1;
    }
    ndp = (*dev->dev_open) (dev, parent, _O_RDWR, 0);
    free(parent);
    if (ndp == NUTFILE_EOF) {
        return -1;
    }

    if ((srch = malloc(sizeof(PHATFIND))) == NULL) {
        (*dev->dev_close) (ndp);
        errno = ENOMEM;
        return -1;
    }
    if ((rc = PhatDirEntryFind(ndp, fname, PHAT_FATTR_FILEMASK, srch)) == 0) {
        struct _tm t;

        memset(&t, 0, sizeof(struct _tm));
        val = srch->phfind_ent.dent_mtime;
        t.tm_sec = (val & 0x1F) << 1;
        t.tm_min = (val >> 5) & 0x3F;
        t.tm_hour = (val >> 11) & 0x1F;
        val = srch->phfind_ent.dent_mdate;
        t.tm_mday = val & 0x1F;
        t.tm_mon = ((val >> 5) & 0x0F);
        if (t.tm_mon) {
            t.tm_mon--;
        }
        t.tm_year = ((val >> 9) & 0x7F) + 80;
        stp->st_mtime = mktime(&t);

        stp->st_ino = 0;
        stp->st_mode = (srch->phfind_ent.dent_attr & PHAT_FATTR_DIR) != 0;
        stp->st_nlink = 0;
        stp->st_size = srch->phfind_ent.dent_fsize;
    }
    free(srch);
    (*dev->dev_close) (ndp);

    return rc;
}

/*@}*/
