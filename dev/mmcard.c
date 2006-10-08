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
 * \brief Basic block device driver for multimedia cards.
 *
 * The driver uses SPI mode, but doesn't include any low level hardware
 * access. This must be provided by some additional routines.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.9  2006/10/08 16:48:09  haraldkipp
 * Documentation fixed
 *
 * Revision 1.8  2006/07/05 08:03:12  haraldkipp
 * Bugfix. Trailing slash in mount path not properly handled.
 * Thanks to Michael Fischer.
 *
 * Revision 1.7  2006/06/18 16:34:46  haraldkipp
 * Mutex deadlock fixed.
 *
 * Revision 1.6  2006/05/25 09:34:21  haraldkipp
 * Added mutual exclusion lock for multithreaded access.
 *
 * Revision 1.5  2006/04/07 12:29:03  haraldkipp
 * Number of read retries increased. Memory hole fixed.
 * Added ioctl(NUTBLKDEV_MEDIAAVAIL).
 * Card change ioctl() will also return 1 if no card is available.
 *
 * Revision 1.4  2006/02/23 15:43:56  haraldkipp
 * Timeout value increased. Some cards have long write latencies.
 *
 * Revision 1.3  2006/01/22 17:36:31  haraldkipp
 * Some cards need more time to enter idle state.
 * Card access now returns an error after card change detection.
 *
 * Revision 1.2  2006/01/19 18:40:08  haraldkipp
 * Timeouts increased and long time sleeps decreased for better performance.
 *
 * Revision 1.1  2006/01/05 16:30:49  haraldkipp
 * First check-in.
 *
 *
 * \endverbatim
 */

#if 0
/* Use for local debugging. */
#define NUTDEBUG
#include <stdio.h>
#endif

#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <sys/heap.h>
#include <sys/timer.h>
#include <sys/event.h>
#include <fs/dospart.h>
#include <fs/fs.h>

#include <dev/blockdev.h>
#include <dev/mmcard.h>

/*!
 * \addtogroup xgMmCard
 */
/*@{*/

#ifndef MMC_BLOCK_SIZE
#define MMC_BLOCK_SIZE  512
#endif

/*!
 * \brief Local multimedia card mount information.
 */
typedef struct _MMCFCB {
    /*! \brief Attached file system device. 
     */
    NUTDEVICE *fcb_fsdev;

    /*! \brief Partition table entry of the currently mounted partition.
     */
    DOSPART fcb_part;

    /*! \brief Next block number to read.
     *
     * The file system driver will send a NUTBLKDEV_SEEK control command
     * to set this value before calling the read or the write routine.
     *
     * The number is partition relative.
     */
    u_long fcb_blknum;

    /*! \brief Internal block buffer.
     *
     * A file system driver may use this one or optionally provide it's 
     * own buffers.
     *
     * Minimal systems may share their external bus interface with
     * device I/O lines, in which case the buffer must be located
     * in internal memory.
     */
    u_char fcb_blkbuf[MMC_BLOCK_SIZE];
} MMCFCB;

/*
 * Several routines call NutSleep, which results in a context switch.
 * This mutual exclusion semaphore takes care, that multiple threads
 * do not interfere with each other.
 */
static HANDLE mutex;

/*!
 * \brief Send command to multimedia card.
 *
 * \param ifc   Specifies the hardware interface.
 * \param cmd   Command code. See MMCMD_ macros.
 * \param param Optional command parameter.
 */
static void MmCardTxCmd(MMCIFC * ifc, u_char cmd, u_long param)
{
    u_int tmo = 1024;
    u_char ch;

    /* Enable card select. */
    (*ifc->mmcifc_cs) (1);
    /*
     * Repeat sending nothing until we receive nothing. Actually
     * it should be sufficient to send a single 0xFF value, but
     * running a loop seems to fix certain kind of sync problems.
     */
    while ((ch = (*ifc->mmcifc_io) (0xFF)) != 0xFF) {
        if (--tmo == 0) {
#ifdef NUTDEBUG
            printf("[MMCmd%u Timeout %02X]\n", cmd, ch);
#endif
            break;
        }
        if (tmo < 256) {
            NutSleep(10);
        }
    }
    /* Send command and parameter. */
    (*ifc->mmcifc_io) (MMCMD_HOST | cmd);
    (*ifc->mmcifc_io) ((u_char) (param >> 24));
    (*ifc->mmcifc_io) ((u_char) (param >> 16));
    (*ifc->mmcifc_io) ((u_char) (param >> 8));
    (*ifc->mmcifc_io) ((u_char) param);
    /*
     * We are running with CRC disabled. However, the reset command must
     * be send with a valid CRC. Fortunately this command is sent with a
     * fixed parameter value of zero, which results in a fixed CRC value
     */
    (*ifc->mmcifc_io) (MMCMD_RESET_CRC);
}

/*!
 * \brief Receive an R1 response token from the card.
 *
 * In SPI mode, the card sends an R1 response after every command except
 * after the SEND_STATUS and the READ_OCR commands.
 *
 * \param ifc Specifies the hardware interface.
 *
 * \return R1 response token or 0xFF on timeout.
 */
static u_char MmCardRxR1(MMCIFC * ifc)
{
    u_char rc;
    int i;

    for (i = 0; i < 255; i++) {
        if ((rc = (*ifc->mmcifc_io) (0xFF)) != 0xFF) {
            break;
        }
    }
    return rc;
}

/*!
 * \brief Receive an R2 response token from the card.
 *
 * In SPI mode the card sends this token in response to the SEND_STATUS
 * command.
 *
 * \param ifc Specifies the hardware interface.
 *
 * \return R2 response token or 0xFFFF on timeout.
 */
static u_short MmCardRxR2(MMCIFC * ifc)
{
    u_short rc;

    rc = MmCardRxR1(ifc);
    rc <<= 8;
    rc += (*ifc->mmcifc_io) (0xFF);

    return rc;
}

/*!
 * \brief Receive an R3 response token from the card.
 *
 * In SPI mode the card sends this token in response to the READ_OCR
 * command.
 *
 * \param ifc Specifies the hardware interface.
 * \param ocr Points to a buffer which receives the OCR register contents.
 *
 * \return R1 response token or 0xFF on timeout.
 */
static u_char MmCardRxR3(MMCIFC * ifc, u_long * ocr)
{
    u_char rc;
    int i;

    /* The first byte is equal to the R1 response. */
    rc = MmCardRxR1(ifc);
    /* Receive the operating condition. */
    for (i = 0; i < 4; i++) {
        *ocr <<= 8;
        *ocr |= (*ifc->mmcifc_io) (0xFF);
    }
    return rc;
}

/*!
 * \brief Configure card for SPI mode.
 *
 * \param ifc Specifies the hardware interface.
 *
 * \return 0 on success, -1 otherwise.
 */
static int MmCardReset(MMCIFC * ifc)
{
    int i;
    u_char rsp;

    /*
     * Initialize the low level card interface.
     */
    if ((*ifc->mmcifc_in) ()) {
        return -1;
    }

    /*
     * 80 bits of ones with deactivated chip select will put the card 
     * in SPI mode.
     */
    (*ifc->mmcifc_cs) (0);
    for (i = 0; i < 10; i++) {
        (*ifc->mmcifc_io) (0xFF);
    }

    /*
     * Switch to idle state and wait until initialization is running
     * or idle state is reached.
     */
    for (i = 0; i < 255; i++) {
        MmCardTxCmd(ifc, MMCMD_GO_IDLE_STATE, 0);
        rsp = MmCardRxR1(ifc);
        (*ifc->mmcifc_cs) (0);
        if (rsp == MMR1_IDLE_STATE || rsp == MMR1_NOT_IDLE) {
            return 0;
        }
    }
    return -1;
}

/*!
 * \brief Initialize the multimedia card.
 *
 * This routine will put a newly powered up card into SPI mode.
 * It is called by MmCardMount().
 *
 * \param ifc Specifies the hardware interface.
 *
 * \return 0 on success, -1 otherwise.
 */
static int MmCardInit(MMCIFC * ifc)
{
    int i;
    u_char rsp;

    /*
     * Try to switch to SPI mode. Looks like a retry helps to fix
     * certain synchronization problems.
     */
    if (MmCardReset(ifc)) {
        if (MmCardReset(ifc)) {
#ifdef NUTDEBUG
            printf("[CardReset failed]");
#endif
            return -1;
        }
    }

    /*
     * Wait for a really long time until card is initialized
     * and enters idle state.
     */
    for (i = 0; i < 512; i++) {
        /*
         * In SPI mode SEND_OP_COND is a dummy, used to poll the card
         * for initialization finished. Thus, there are no parameters
         * and no operation condition data is sent back.
         */
        MmCardTxCmd(ifc, MMCMD_SEND_OP_COND, 0);
        rsp = MmCardRxR1(ifc);
        (*ifc->mmcifc_cs) (0);
        if (rsp == MMR1_IDLE_STATE) {
#ifdef NUTDEBUG
            printf("[CardIdle]");
#endif
            /* Initialize MMC access mutex semaphore. */
            NutEventPost(&mutex);
            return 0;
        }
        if (i > 128) {
            NutSleep(1);
        }
    }
#ifdef NUTDEBUG
    printf("[CardInit failed]");
#endif
    return -1;
}

/*!
 * \brief Read or verify a single block.
 *
 * \param ifc Specifies the hardware interface.
 * \param blk Block number to read or verify.
 * \param buf Data buffer. Receives the data or is verified against the
 *            data being read from the card.
 *
 * \return 0 on success, -1 otherwise.
 */
static int MmCardReadOrVerify(MMCIFC * ifc, u_long blk, u_char * buf, int vflg)
{
    int rc = -1;
    int retries = 64;
    int i;
    u_char rsp;

    /* Gain mutex access. */
    NutEventWait(&mutex, 0);

    while (retries--) {
        MmCardTxCmd(ifc, MMCMD_READ_SINGLE_BLOCK, blk << 9);
        if ((rsp = MmCardRxR1(ifc)) == 0x00) {
            if ((rsp = MmCardRxR1(ifc)) == 0xFE) {
                rc = 0;
                if (vflg) {
                    for (i = 0; i < 512; i++) {
                        if (*buf != (*ifc->mmcifc_io) (0xFF)) {
                            rc = -1;
                        }
                        buf++;
                    }
                } else {
                    for (i = 0; i < 512; i++) {
                        *buf = (*ifc->mmcifc_io) (0xFF);
                        buf++;
                    }
                }
                (*ifc->mmcifc_io) (0xff);
                (*ifc->mmcifc_io) (0xff);
                (*ifc->mmcifc_cs) (0);
                break;
            }
        }
        (*ifc->mmcifc_cs) (0);
    }

    /* Release mutex access. */
    NutEventPost(&mutex);

    return rc;
}

/*!
 * \brief Write a single block.
 *
 * \param ifc Specifies the hardware interface.
 * \param blk Block number to read or verify.
 * \param buf Pointer to a buffer which holds the data to write.
 *
 * \return 0 on success, -1 otherwise.
 */
static int MmCardWrite(MMCIFC * ifc, u_long blk, CONST u_char * buf)
{
    int rc = -1;
    int retries = 32;
    int tmo;
    int i;
    u_char rsp;

    /* Gain mutex access. */
    NutEventWait(&mutex, 0);

    while (retries--) {
        MmCardTxCmd(ifc, MMCMD_WRITE_BLOCK, blk << 9);
        if ((rsp = MmCardRxR1(ifc)) == 0x00) {
            (*ifc->mmcifc_io) (0xFF);
            (*ifc->mmcifc_io) (0xFE);
            for (i = 0; i < 512; i++) {
                (*ifc->mmcifc_io) (*buf);
                buf++;
            }
            // (*ifc->mmcifc_io)(0xFF);
            // (*ifc->mmcifc_io)(0xFF);
            if ((rsp = MmCardRxR1(ifc)) == 0xE5) {
                for (tmo = 0; tmo < 1024; tmo++) {
                    if ((*ifc->mmcifc_io) (0xFF) == 0xFF) {
                        break;
                    }
                    if (tmo > 1000) {
                        NutSleep(1);
                    }
                }
                if (tmo) {
                    rc = 0;
                    break;
                }
#ifdef NUTDEBUG
                printf("[MMCWR Timeout]\n");
#endif
            }
        }
        (*ifc->mmcifc_cs) (0);
    }
    (*ifc->mmcifc_cs) (0);

    /* Release mutex access. */
    NutEventPost(&mutex);

    return rc;
}

/*!
 * \brief Read data blocks from a mounted partition.
 *
 * Applications should not call this function directly, but use the
 * stdio interface.
 *
 * \param nfp    Pointer to a ::NUTFILE structure, obtained by a previous 
 *               call to MmCardMount().
 * \param buffer Pointer to the data buffer to fill.
 * \param num    Maximum number of blocks to read. However, reading 
 *               multiple blocks is not yet supported by this driver.
 *
 * \return The number of blocks actually read. A return value of -1 
 *         indicates an error.
 */
int MmCardBlockRead(NUTFILE * nfp, void *buffer, int num)
{
    MMCFCB *fcb = (MMCFCB *) nfp->nf_fcb;
    u_long blk = fcb->fcb_blknum;
    NUTDEVICE *dev = (NUTDEVICE *) nfp->nf_dev;
    MMCIFC *ifc = (MMCIFC *) dev->dev_icb;

    if ((*ifc->mmcifc_cd) () != 1) {
        return -1;
    }
    if (buffer == 0) {
        buffer = fcb->fcb_blkbuf;
    }
    blk += fcb->fcb_part.part_sect_offs;

#ifdef MMC_VERIFY_AFTER
    {
        int i;
        /*
         * It would be much better to verify the checksum than to re-read
         * and verify the data block.
         */
        for (i = 0; i < 8; i++) {
            if (MmCardReadOrVerify(ifc, blk, buffer, 0) == 0) {
                if (MmCardReadOrVerify(ifc, blk, buffer, 1) == 0) {
                    return 1;
                }
            }
        }
    }
#else
    if (MmCardReadOrVerify(ifc, blk, buffer, 0) == 0) {
        return 1;
    }
#endif
    return -1;
}

/*!
 * \brief Write data blocks to a mounted partition.
 *
 * Applications should not call this function directly, but use the
 * stdio interface.
 *
 * \param nfp    Pointer to a \ref NUTFILE structure, obtained by a previous 
 *               call to MmCardMount().
 * \param buffer Pointer to the data to be written.
 * \param num    Maximum number of blocks to write. However, writing
 *               multiple blocks is not yet supported by this driver.
 *
 * \return The number of blocks written. A return value of -1 indicates an 
 *         error.
 */
int MmCardBlockWrite(NUTFILE * nfp, CONST void *buffer, int num)
{
    MMCFCB *fcb = (MMCFCB *) nfp->nf_fcb;
    u_long blk = fcb->fcb_blknum;
    NUTDEVICE *dev = (NUTDEVICE *) nfp->nf_dev;
    MMCIFC *ifc = (MMCIFC *) dev->dev_icb;

    if ((*ifc->mmcifc_cd) () != 1) {
        return -1;
    }
    if (buffer == 0) {
        buffer = fcb->fcb_blkbuf;
    }
    blk += fcb->fcb_part.part_sect_offs;

#ifdef MMC_VERIFY_AFTER
    {
        int i;

        for (i = 0; i < 8; i++) {
            if (MmCardWrite(ifc, blk, buffer) == 0) {
                if (MmCardReadOrVerify(ifc, blk, (void *) buffer, 1) == 0) {
                    return 1;
                }
                if (MmCardReadOrVerify(ifc, blk, (void *) buffer, 1) == 0) {
                    return 1;
                }
            }
        }
    }
#else
    if (MmCardWrite(ifc, blk, buffer) == 0) {
        return 1;
    }
#endif
    return -1;
}

#ifdef __HARVARD_ARCH__
/*! 
 * \brief Write data blocks from program space to a mounted partition.
 *
 * This function is not yet implemented and will always return -1.
 *
 * Similar to MmCardBlockWrite() except that the data is located in 
 * program memory.
 *
 * Applications should not call this function directly, but use the
 * stdio interface.
 *
 * \param nfp    File pointer to a previously opened device.
 * \param buffer Pointer to the data bytes in program space to be written.
 * \param num    Maximum number of blocks to write. However, writing
 *               multiple blocks is not yet supported by this driver.
 *
 * \return The number of blocks written. A return value of -1 indicates an 
 *         error.
 */
int MmCardBlockWrite_P(NUTFILE * nfp, PGM_P buffer, int num)
{
    return -1;
}
#endif

/*!
 * \brief Mount a partition.
 *
 * Nut/OS doesn't provide specific routines for mounting. Instead routines
 * for opening files are used.
 *
 * Applications should not directly call this function, but use the high
 * level stdio routines for opening a file.
 *
 * \param dev  Pointer to the MMC device.
 * \param name Partition number followed by a slash followed by a name
 *             of the file system device. Both items are optional. If no 
 *             file system driver name is given, the first file system
 *             driver found in the list of registered devices will be 
 *             used. If no partition number is specified or if partition
 *             zero is given, the first active primary partition will be 
 *             used.
 * \param mode Opening mode. Currently ignored, but 
 *             \code _O_RDWR | _O_BINARY \endcode should be used for
 *             compatibility with future enhancements.
 * \param acc  File attributes, ignored.
 *
 * \return Pointer to a newly created file pointer to the mounted
 *         partition or NUTFILE_EOF in case of any error.
 */
NUTFILE *MmCardMount(NUTDEVICE * dev, CONST char *name, int mode, int acc)
{
    int partno = 0;
    int i;
    NUTDEVICE *fsdev;
    NUTFILE *nfp;
    MMCFCB *fcb;
    DOSPART *part;
    MMCIFC *ifc = (MMCIFC *) dev->dev_icb;
    FSCP_VOL_MOUNT mparm;

    /* Return an error if no card is detected. */
    if ((*ifc->mmcifc_cd) () == 0) {
        errno = ENODEV;
        return NUTFILE_EOF;
    }

    /* Set the card in SPI mode. */
    if (MmCardInit(ifc)) {
        errno = ENODEV;
        return NUTFILE_EOF;
    }

    /* Parse the name for a partition number and a file system driver. */
    if (*name) {
        partno = atoi(name);
        do {
            name++;
        } while (*name && *name != '/');
        if (*name == '/') {
            name++;
        }
    }

    /*
     * Check the list of registered devices for the given name of the
     * files system driver. If none has been specified, get the first
     * file system driver in the list. Hopefully the application
     * registered one only.
     */
    for (fsdev = nutDeviceList; fsdev; fsdev = fsdev->dev_next) {
        if (*name == 0) {
            if (fsdev->dev_type == IFTYP_FS) {
                break;
            }
        } else if (strcmp(fsdev->dev_name, name) == 0) {
            break;
        }
    }

    if (fsdev == 0) {
#ifdef NUTDEBUG
        printf("[No FSDriver]");
#endif
        errno = ENODEV;
        return NUTFILE_EOF;
    }

    if ((fcb = NutHeapAllocClear(sizeof(MMCFCB))) == 0) {
        errno = ENOMEM;
        return NUTFILE_EOF;
    }
    fcb->fcb_fsdev = fsdev;

    /* Read MBR. */
    if (MmCardReadOrVerify(ifc, 0, fcb->fcb_blkbuf, 0)) {
        NutHeapFree(fcb);
        return NUTFILE_EOF;
    }

    /* Read partition table. */
    part = (DOSPART *) & fcb->fcb_blkbuf[DOSPART_SECTORPOS];
    for (i = 1; i <= 4; i++) {
        if (partno) {
            if (i == partno) {
                /* Found specified partition number. */
                fcb->fcb_part = *part;
                break;
            }
        } else if (part->part_state & 0x80) {
            /* Located first active partition. */
            fcb->fcb_part = *part;
            break;
        }
        part++;
    }

    if (fcb->fcb_part.part_type == PTYPE_EMPTY) {
        NutHeapFree(fcb);
        return NUTFILE_EOF;
    }

    if ((nfp = NutHeapAlloc(sizeof(NUTFILE))) == 0) {
        NutHeapFree(fcb);
        errno = ENOMEM;
        return NUTFILE_EOF;
    }
    nfp->nf_next = 0;
    nfp->nf_dev = dev;
    nfp->nf_fcb = fcb;

    /*
     * Mount the file system volume.
     */
    mparm.fscp_bmnt = nfp;
    mparm.fscp_part_type = fcb->fcb_part.part_type;
    if (fsdev->dev_ioctl(fsdev, FS_VOL_MOUNT, &mparm)) {
        MmCardUnmount(nfp);
        return NUTFILE_EOF;
    }
    return nfp;
}

/*!
 * \brief Unmount a previously mounted partition.
 *
 * Applications should not directly call this function, but use the high
 * level stdio routines for closing a previously opened file.
 *
 * \return 0 on success, -1 otherwise.
 */
int MmCardUnmount(NUTFILE * nfp)
{
    int rc = -1;

    if (nfp) {
        MMCFCB *fcb = (MMCFCB *) nfp->nf_fcb;

        if (fcb) {
            NUTDEVICE *dev = (NUTDEVICE *) nfp->nf_dev;
            MMCIFC *ifc = (MMCIFC *) dev->dev_icb;

            if ((*ifc->mmcifc_cd) () == 1) {
                rc = fcb->fcb_fsdev->dev_ioctl(fcb->fcb_fsdev, FS_VOL_UNMOUNT, NULL);
            }
            NutHeapFree(fcb);
        }
        NutHeapFree(nfp);
    }
    return rc;
}

/*!
 * \brief Retrieve contents of a card register.
 *
 * \param ifc Specifies the hardware interface.
 * \param cmd This command is sent to the card to retrieve the contents
 *            of a specific register.
 * \param rbp Pointer to the buffer that receives the register contents.
 * \param siz Size of the specified register.
 *
 * \return 0 on success, -1 otherwise.
 */
static int MmCardGetReg(MMCIFC * ifc, u_char cmd, u_char * rbp, int siz)
{
    int rc = -1;
    int retries = 512;
    int i;

    /* Gain mutex access. */
    NutEventWait(&mutex, 0);

    while (retries--) {
        /* Send the command to the card. This will select the card. */
        MmCardTxCmd(ifc, cmd, 0);
        /* Wait for OK response from the card. */
        if (MmCardRxR1(ifc) == 0x00) {
            /* Wait for data from the card. */
            if (MmCardRxR1(ifc) == 0xFE) {
                for (i = 0; i < siz; i++) {
                    *rbp++ = (*ifc->mmcifc_io) (0xFF);
                }
                /* Ignore the CRC. */
                (*ifc->mmcifc_io) (0xFF);
                (*ifc->mmcifc_io) (0xFF);
                /* De-activate card selection. */
                (*ifc->mmcifc_cs) (0);
                /* Return a positive result. */
                rc = 0;
                break;
            }
        }
        /* De-activate card selection. */
        (*ifc->mmcifc_cs) (0);
    }

    /* Release mutex access. */
    NutEventPost(&mutex);

    return rc;
}

/*!
 * \brief Perform MMC control functions.
 *
 * This function is called by the ioctl() function of the C runtime
 * library. Applications should not directly call this function.
 *
 * \todo Card change detection should verify the serial card number.
 *
 * \param dev  Identifies the device that receives the device-control
 *             function.
 * \param req  Requested control function. May be set to one of the
 *             following constants:
 *             - \ref NUTBLKDEV_MEDIACHANGE
 *             - \ref NUTBLKDEV_INFO
 *             - \ref NUTBLKDEV_SEEK
 *             - \ref MMCARD_GETCID
 *             - \ref MMCARD_GETCSD
 *
 * \param conf Points to a buffer that contains any data required for
 *             the given control function or receives data from that
 *             function.
 * \return 0 on success, -1 otherwise.
 */
int MmCardIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    int rc = 0;
    MMCIFC *ifc = (MMCIFC *) dev->dev_icb;

    switch (req) {
    case NUTBLKDEV_MEDIAAVAIL:
        {
            int *flg = (int *) conf;
            *flg = (*ifc->mmcifc_cd) ();
        }
        break;
    case NUTBLKDEV_MEDIACHANGE:
        {
            int *flg = (int *) conf;
            if ((*ifc->mmcifc_cd) () != 1) {
                *flg = 1;
            } else {
                *flg = 0;
            }
        }
        break;
    case NUTBLKDEV_INFO:
        {
            BLKPAR_INFO *par = (BLKPAR_INFO *) conf;
            MMCFCB *fcb = (MMCFCB *) par->par_nfp->nf_fcb;

            par->par_nblks = fcb->fcb_part.part_sects;
            par->par_blksz = MMC_BLOCK_SIZE;
            par->par_blkbp = fcb->fcb_blkbuf;
        }
        break;
    case NUTBLKDEV_SEEK:
        {
            BLKPAR_SEEK *par = (BLKPAR_SEEK *) conf;
            MMCFCB *fcb = (MMCFCB *) par->par_nfp->nf_fcb;

            fcb->fcb_blknum = par->par_blknum;
        }
        break;
    case MMCARD_GETSTATUS:
        {
            u_short *s = (u_short *) conf;

            /* Gain mutex access. */
            NutEventWait(&mutex, 0);

            MmCardTxCmd(ifc, MMCMD_SEND_STATUS, 0);
            *s = MmCardRxR2(ifc);

            /* Release mutex access. */
            NutEventPost(&mutex);
        }
        break;
    case MMCARD_GETOCR:
        /* Gain mutex access. */
        NutEventWait(&mutex, 0);

        MmCardTxCmd(ifc, MMCMD_READ_OCR, 0);
        if (MmCardRxR3(ifc, (u_long *) conf) != MMR1_IDLE_STATE) {
            rc = -1;
        }

        /* Release mutex access. */
        NutEventPost(&mutex);
        break;
    case MMCARD_GETCID:
        rc = MmCardGetReg(ifc, MMCMD_SEND_CID, (u_char *) conf, sizeof(MMC_CID));
        break;
    case MMCARD_GETCSD:
        rc = MmCardGetReg(ifc, MMCMD_SEND_CSD, (u_char *) conf, sizeof(MMC_CSD));
        break;
    default:
        rc = -1;
        break;
    }
    return rc;
}

/*!
 * \brief Initialize high level MMC driver.
 *
 * Applications should not directly call this function. It is 
 * automatically executed during during device registration by 
 * NutRegisterDevice().
 *
 * \param dev  Identifies the device to initialize.
 *
 * \return Always zero.
 */
int MmCardDevInit(NUTDEVICE * dev)
{
    return 0;
}

/*@}*/
