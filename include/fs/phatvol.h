#ifndef FS_PHATVOL_H_
#define FS_PHATVOL_H_

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
 * \file fs/phatvol.h
 * \brief PHAT file system.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.2  2006/02/23 15:47:18  haraldkipp
 * PHAT file system now supports configurable number of sector buffers.
 *
 * Revision 1.1  2006/01/05 16:33:10  haraldkipp
 * First check-in.
 *
 *
 * \endverbatim
 */

#include <cfg/fs.h>

#include <sys/types.h>
#include <sys/file.h>
#include <sys/device.h>

/*!
 * \addtogroup xgPhatVol
 */
/*@{*/

/*!
 * \brief Volume boot record.
 */
typedef struct __attribute__ ((packed)) _PHATVBR {
    /*! \brief Jump instruction, contains E9xxxx or EBxx90. */
    u_char boot_jump[3];
    /*! \brief OEM name and version. */
    u_char boot_oem[8];
    /*! \brief Bytes per sector. 
     *
     * The BIOS parameter block starts here.
     */
    u_short bios_sectsz;
    /*! \brief Sectors per cluster. */
    u_char bios_clustsz;
    /*! \brief Number of reserved sectors. 
     *
     * This includes the boot sector, which is typically the only
     * reserved sector with PHAT12/16.
     */
    u_short bios_rsvd_sects;
    /*! \brief Number of allocation tables. */
    u_char bios_ntabs;
    /*! \brief Number of root directory entries. */
    u_short bios_rootsz;
    /*! \brief Total number of sectors. */
    u_short bios_volsz;
    /*! \brief Media descriptor. */
    u_char bios_media;
    /*! \brief Number of sectors per allocation table.
     *
     * If zero, then the value is stored in bios_tabsz_big.
     */
    u_short bios_tabsz;
    /*! \brief Number of sectors per track. */
    u_short bios_sects_p_trk;
    /*! \brief Number of heads. */
    u_short bios_heads;
    /*! \brief Number of hidden sectors. 
     *
     * With PHAT12 this field is two bytes only. We ignore it anyway.
     */
    u_long bios_sects_hidd;
    /*! \brief Total number of sectors for huge drives. 
     *
     * Valid only if the value in bios_volsz is zero.
     */
    u_long bios_volsz_big;

    /*! \brief Number of sectors per allocation table for huge drives.
     *
     * This and the following 6 fields are available with PHAT32 only.
     */
    u_long bios_tabsz_big;
    /*! \brief Extended flags. 
     *
     * If bit 7 is set, then bits 0-3 specify the active allocation table.
     * This feature is not yet supported. PHAT32 always updates the first
     * two tables.
     */
    u_short bios_xflags;
    /*! \brief File system version. */
    u_short bios_fsver;
    /*! \brief First cluster of root directory. */
    u_long bios_root_clust;
    /*! \brief File system info sector. */
    u_short bios_fsinfo;
    /*! \brief Boot backup sector. */
    u_short bios_boot_bak;
    /*! \brief Reserved for future expansion. 
     *
     * End of the BIOS parameter block.
     */
    u_char bios_rsvd32[12];

    /*! \brief Logical drive number.
     *
     * With PHAT12 and PHAT16 this and the following fields
     * are at offset 36.
     */
    u_char boot_drive;
    /*! \brief Reserved field, used by Windows NT. */
    u_char boot_rsvd_nt;
    /*! \brief Extended signature.
     *
     * If 0x28 or 0x29, than the following fields are valid.
     */
    u_char boot_xsig;
    /*! \brief Volume serial number. */
    u_long boot_vol_id;
    /*! \brief Volume label. */
    u_char boot_vol_lbl[11];
    /*! \brief File system. */
    u_char boot_vol_fs[8];
} PHATVBR;

/*!
 * \brief Sector buffer structure.
 */
typedef struct _PHATSECTBUF {
    /*! \brief Sector data buffer. */
    u_char *sect_data;
    /*! \brief Sector currently stored in the buffer. */
    u_long sect_num;
    /*! \brief If not zero, buffer needs to be written. */
    int sect_dirty;
} PHATSECTBUF;

/*!
 * \brief Volume info structure.
 */
typedef struct _PHATVOL {
    /*! \brief Type of volume.
     *
     * Either 12, 16 or 32.
     */
    int vol_type;
    /*! \brief Number of free clusters. */
    u_long vol_numfree;
    /*! \brief Possibly next free cluster. */
    u_long vol_nxtfree;
    /*! \brief Sector buffer of this volume. */
#if PHAT_SECTOR_BUFFERS
    PHATSECTBUF vol_buf[PHAT_SECTOR_BUFFERS];
    /*! \brief Next buffer to use. Simple round robin scheme. */
    int vol_usenext;
#else
    PHATSECTBUF vol_buf[1];
#endif
    /*! \brief Bytes per sector. */
    u_int vol_sectsz;
    /*! \brief Sectors per cluster. */
    u_int vol_clustsz;

    /*! \brief Number of sectors per allocation table. */
    u_long vol_tabsz;
    /*! \brief First sector of each allocation table. 
     *
     * We maintain upto two allocation tables.
     */
    u_long vol_tab_sect[2];

    /*! \brief Number of sectors used by the root directory. */
    u_int vol_rootsz;
    /*! \brief First sector of the root directory. */
    u_long vol_root_sect;
    /*! \brief First cluster of the root directory. */
    u_long vol_root_clust;

    /*! \brief Last data cluster number.
     *
     * The first cluster number is 2, so this value is equal to the total
     * number of data clusters plus 2.
     */
    u_long vol_last_clust;
    /*! \brief First data sector. 
     *
     * The first sector following the root directory.
     */
    u_long vol_data_sect;
} PHATVOL;

/*@}*/

__BEGIN_DECLS
/* Prototypes */
extern int PhatVolMount(NUTDEVICE * dev, NUTFILE * blkmnt, u_char part_type);
extern int PhatVolUnmount(NUTDEVICE * dev);
extern u_long PhatClusterSector(NUTFILE * nfp, u_long clust);

__END_DECLS
/* End of prototypes */
#endif
