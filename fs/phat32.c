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
 * \file fs/phat32.c
 * \brief PHAT32 specific routines.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.4  2006/01/23 19:52:10  haraldkipp
 * Added required typecasts before left shift.
 *
 * Revision 1.3  2006/01/23 17:33:47  haraldkipp
 * Avoid memory alignment errors.
 *
 * Revision 1.2  2006/01/22 17:43:46  haraldkipp
 * Bugfix. Deleting files sometimes corrupted a volume.
 *
 * Revision 1.1  2006/01/05 16:31:22  haraldkipp
 * First check-in.
 *
 *
 * \endverbatim
 */

#include <errno.h>

#include <fs/phatfs.h>
#include <fs/phatvol.h>
#include <fs/phatio.h>

/*!
 * \addtogroup xgPhat32
 */
/*@{*/

/*!
 * \brief Calculate table location of a specified cluster.
 *
 * \param vol    Mounted volume.
 * \param clust  Cluster number of the entry to locate.
 * \param tabnum Number of the table.
 * \param sect   Pointer to the variable that receives the sector of the 
 *               table entry.
 * \param pos    Pointer to the variable that receives position within 
 *               the sector.
 */
static void PhatTableLoc(PHATVOL * vol, u_long clust, int tabnum, u_long * sect, u_long * pos)
{
    u_long tabpos = clust * 4;

    *sect = vol->vol_tab_sect[tabnum] + tabpos / vol->vol_sectsz;
    *pos = tabpos % vol->vol_sectsz;
}

/*!
 * \brief Get link value of a specified cluster.
 *
 * \param dev   Specifies the file system device.
 * \param clust Get the link of this cluster.
 * \param link  Pointer to a variable which will receive the link.
 *
 * \return 0 on success or -1 on failure.
 */
int Phat32GetClusterLink(NUTDEVICE * dev, u_long clust, u_long * link)
{
    u_long sect, pos;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    /* Do not seek beyond the end of the chain. */
    if (clust >= (PHATEOC & PHAT32CMASK)) {
        return -1;
    }

    /* Load the sector that contains the table entry. */
    PhatTableLoc(vol, clust, 0, &sect, &pos);
    if (PhatSectorLoad(dev, sect)) {
        return -1;
    }

    /* Get the 32 bit link value. */
    *link = vol->vol_buf[pos];
    *link += (u_long)(vol->vol_buf[pos + 1]) << 8;
    *link += (u_long)(vol->vol_buf[pos + 2]) << 16;
    *link += (u_long)(vol->vol_buf[pos + 3]) << 24;

    return 0;
}

/*!
 * \brief Set link value of a specified cluster.
 *
 * \param dev   Specifies the file system device.
 * \param clust This cluster will be linked.
 * \param link  Link to this cluster.
 *
 * \return 0 on success or -1 on failure.
 */
int Phat32SetClusterLink(NUTDEVICE * dev, u_long clust, u_long link)
{
    int tabnum;
    u_long sect;
    u_long pos;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    for (tabnum = 0; tabnum < 2 && vol->vol_tab_sect[tabnum]; tabnum++) {
        link &= PHAT32CMASK;

        PhatTableLoc(vol, clust, tabnum, &sect, &pos);
        if (PhatSectorLoad(dev, sect)) {
            return -1;
        }
        vol->vol_buf[pos] = (u_char) link;
        vol->vol_buf[pos + 1] = (u_char) (link >> 8);
        vol->vol_buf[pos + 2] = (u_char) (link >> 16);
        vol->vol_buf[pos + 3] = (u_char) (link >> 24);
        vol->vol_bufdirty = 1;
    }
    return 0;
}

/*!
 * \brief Release a cluster chain.
 *
 * \param dev   Specifies the file system device.
 * \param first First cluster of the chain to release.
 *
 * \return 0 on success or -1 on failure.
 */
int Phat32ReleaseChain(NUTDEVICE * dev, u_long first)
{
    u_long next;
    PHATVOL *vol = (PHATVOL *) dev->dev_dcb;

    while (first < (PHATEOC & PHAT32CMASK)) {
        if (Phat32GetClusterLink(dev, first, &next)) {
            /* Read error. */
            return -1;
        }
        if (next < 2) {
            /* Incomplete chain, should not happen. */
            break;
        }
        if (Phat32SetClusterLink(dev, first, 0)) {
            /* Write error. */
            return -1;
        }
        vol->vol_numfree++;
        first = next;
    }
    return 0;
}

/*@}*/
