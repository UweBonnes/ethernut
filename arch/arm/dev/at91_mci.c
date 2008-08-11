/*
 * Copyright (C) 2006 by egnite Software GmbH. All rights reserved.
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
 * \brief Multimedia Card Interface.
 *
 * This simple implementation supports reading a single 
 * 3.3V MultiMedia Card in slot B only.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.2  2008/08/11 06:59:04  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.1  2006/09/05 12:34:21  haraldkipp
 * Support for hardware MultiMedia Card interface added.
 * SD Cards are currently not supported.
 *
 *
 * \endverbatim
 */

#include <cfg/arch.h>
#include <cfg/arch/gpio.h>

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
#include <dev/at91_mci.h>


#if 0
/* Use for local debugging. */
#define NUTDEBUG
#include <stdio.h>
#endif

/*!
 * \addtogroup xgAt91Mci
 */
/*@{*/

#ifndef MMC_BLOCK_SIZE
#define MMC_BLOCK_SIZE  512
#endif

#ifndef MMC_PINS_A
#define MMC_PINS_A  _BV(PA8_MCCK_A)
#endif

#ifndef MMC_PINS_B
#define MMC_PINS_B  _BV(PA1_MCCDB_B) | _BV(PA0_MCDB0_B) | _BV(PA5_MCDB1_B) | _BV(PA4_MCDB2_B) | _BV(PA3_MCDB3_B)
#endif

/*!
 * \brief Local card interface information.
 */
typedef struct _MCIFC {
    /*! \brief Operating conditions. */
    uint32_t ifc_opcond;
    /*! \brief Relative card address. */
    u_int ifc_reladdr;
    /*! \brief Pointer to sector buffer. */
    uint8_t *ifc_buff;
    /*! \brief MMC response. */
    u_int ifc_resp[4];
} MCIFC;

/*!
 * \brief Local multimedia card mount information.
 */
typedef struct _MCIFCB {
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
    uint32_t fcb_blknum;

    /*! \brief Internal block buffer.
     *
     * A file system driver may use this one or optionally provide it's 
     * own buffers.
     *
     * Minimal systems may share their external bus interface with
     * device I/O lines, in which case the buffer must be located
     * in internal memory.
     */
    uint8_t fcb_blkbuf[MMC_BLOCK_SIZE];
} MCIFCB;

/*!
 * \brief Initialize MMC hardware interface.
 *
 * This function is automatically executed during during device
 * registration via NutRegisterDevice().
 *
 * \param dev Identifies the device to initialize.
 */
static int At91MciInit(NUTDEVICE * dev)
{
#ifdef NUTDEBUG
    printf("[DevInit '%s']", dev->dev_name);
#endif
    /* Disable PIO lines used for MCI. */
    outr(PIOA_PDR, MMC_PINS_A | MMC_PINS_B);
    /* Enable peripherals. */
    outr(PIOA_ASR, MMC_PINS_A);
    outr(PIOA_BSR, MMC_PINS_B);

    /* Disable and software reset. */
    outr(MCI_CR, MCI_MCIDIS | MCI_SWRST);
    NutSleep(10);

    outr(MCI_CR, MCI_MCIEN);
    outr(MCI_IDR, 0xFFFFFFFF);
    outr(MCI_DTOR, MCI_DTOMUL_1M | MCI_DTOCYC);
    /*
     * MMC clock is MCK / (2 * (CLKDIV + 1))
     */
    //outr(MCI_MR, MCI_PDCMODE | (3 << MCI_PWSDIV_LSB) | (47 << MCI_CLKDIV_LSB));
    //outr(MCI_MR, MCI_PDCMODE | (3 << MCI_PWSDIV_LSB) | (15 << MCI_CLKDIV_LSB)); // 180MHz
    outr(MCI_MR, MCI_PDCMODE | (3 << MCI_PWSDIV_LSB) | (11 << MCI_CLKDIV_LSB)); // Test
    //outr(MCI_MR, MCI_PDCMODE | (3 << MCI_PWSDIV_LSB) | (11 << MCI_CLKDIV_LSB));

    /* Select 1-bit mode and slot B. */
    outr(MCI_SDCR, MCI_SDCSEL_SLOTB);

    /* Enable MCI clock. */
    outr(PMC_PCER, _BV(MCI_ID));

    return 0;
}

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
static u_int At91MciTxCmd(MCIFC * ifc, u_int cmd, u_int param)
{
    u_int sr;
    u_int rl;
    u_int i;
    u_int wfs = MCI_CMDRDY;

#ifdef NUTDEBUG
    printf("[MmcCmd %X", cmd);
#endif
    outr(MCI_PTCR, PDC_TXTDIS | PDC_RXTDIS);
    if ((cmd & MCI_TRCMD_START) != 0 && ifc->ifc_buff) {
        outr(MCI_MR, (inr(MCI_MR) & ~MCI_BLKLEN) | (MMC_BLOCK_SIZE << MCI_BLKLEN_LSB) | MCI_PDCMODE);
        outr(MCI_RPR, (u_int) ifc->ifc_buff);
        outr(MCI_RCR, MMC_BLOCK_SIZE / 4);
        outr(MCI_PTCR, PDC_RXTEN);
        wfs = MCI_ENDRX;
        ifc->ifc_buff = NULL;
    } else {
        outr(MCI_RCR, 0);
        outr(MCI_RNCR, 0);
        outr(MCI_TCR, 0);
        outr(MCI_TNCR, 0);
    }
    outr(MCI_ARGR, param);
    outr(MCI_CMDR, cmd);
    while (((sr = inr(MCI_SR)) & wfs) == 0);

    if ((cmd & MCI_RSPTYP) == MCI_RSPTYP_48) {
        rl = 2;
    } else if ((cmd & MCI_RSPTYP) == MCI_RSPTYP_136) {
        rl = 4;
    } else {
        rl = 0;
    }
    for (i = 0; i < rl; i++) {
        ifc->ifc_resp[i] = inr(MCI_RSPR);
    }
#ifdef NUTDEBUG
    printf("=%X]", sr);
#endif
    return sr;
}


/*!
 * \brief Discover available cards.
 *
 * \param ifc Specifies the hardware interface.
 *
 * \return 0 on success, -1 otherwise.
 */
static int At91MciDiscover(MCIFC * ifc)
{
    u_int sr;
    int tmo;

    /* Put all cards in idle state. */
#ifdef NUTDEBUG
    printf("\n[MciIdle]");
#endif
    outr(MCI_MR, MCI_PDCMODE | (3 << MCI_PWSDIV_LSB) | (47 << MCI_CLKDIV_LSB)); // 400kHz during identification
    At91MciTxCmd(ifc, MMCMD_GO_IDLE_STATE, 0);

    /* Poll operating conditions. */
#ifdef NUTDEBUG
    printf("\n[MciOpCond]");
#endif
    for (tmo = 1000; --tmo;) {
        sr = At91MciTxCmd(ifc, MCI_OPCMD | MCI_MAXLAT | MCI_RSPTYP_48 | MMCMD_SEND_OP_COND, ifc->ifc_opcond);
        if (sr & 0xC06B0000) {
            return -1;
        }
        if ((ifc->ifc_resp[0] & MMCOP_NBUSY) != 0) {
            break;
        }
        ifc->ifc_opcond = ifc->ifc_resp[0];
        NutSleep(1);
    }
    if (tmo == 0) {
#ifdef NUTDEBUG
        printf("[Failed]");
#endif
        return -1;
    }

    /* Discover cards. */
#ifdef NUTDEBUG
    printf("\n[MciDiscover]");
#endif
    ifc->ifc_reladdr = 0;
    for (tmo = 500; --tmo;) {
        sr = At91MciTxCmd(ifc, MCI_OPCMD | MCI_MAXLAT | MCI_RSPTYP_136 | MMCMD_ALL_SEND_CID, 0);
        if (sr & MCI_RTOE) {
            /* No more cards. */
            break;
        }
        ifc->ifc_reladdr++;
        At91MciTxCmd(ifc, MCI_MAXLAT | MCI_RSPTYP_48 | MMCMD_SEND_RELATIVE_ADDR, ifc->ifc_reladdr << 16);
    }
#ifdef NUTDEBUG
    printf("[%u Cards]", ifc->ifc_reladdr);
#endif
    outr(MCI_MR, MCI_PDCMODE | (3 << MCI_PWSDIV_LSB) | (1 << MCI_CLKDIV_LSB)); // 10MHz @ 180/4 MHz
    return ifc->ifc_reladdr ? 0 : -1;
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
static int At91MciReadSingle(MCIFC * ifc, uint32_t blk, uint8_t * buf)
{
    int rc = -1;
    u_int sr;

    /* Gain mutex access. */
    NutEventWait(&mutex, 0);

#ifdef NUTDEBUG
    printf("[RB%lu]", blk);
#endif

    sr = At91MciTxCmd(ifc, MCI_MAXLAT | MCI_RSPTYP_48 | MMCMD_SELECT_CARD, ifc->ifc_reladdr << 16);
    if ((sr & 0xC07F0000) == 0) {
        sr = At91MciTxCmd(ifc, MCI_MAXLAT | MCI_RSPTYP_48 | MMCMD_SET_BLOCKLEN, MMC_BLOCK_SIZE);
        if ((sr & 0xC07F0000) == 0) {
            ifc->ifc_buff = buf;
            sr = At91MciTxCmd(ifc, MCI_TRDIR | MCI_TRCMD_START | MCI_MAXLAT | MCI_RSPTYP_48 |
                              MMCMD_READ_SINGLE_BLOCK, blk * MMC_BLOCK_SIZE);
            if ((sr & 0xC07F0000) == 0) {
                rc = 0;
            }
        }
        At91MciTxCmd(ifc, MMCMD_SELECT_CARD, 0);
    }

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
 *               call to At91MciMount().
 * \param buffer Pointer to the data buffer to fill.
 * \param num    Maximum number of blocks to read. However, reading 
 *               multiple blocks is not yet supported by this driver.
 *
 * \return The number of blocks actually read. A return value of -1 
 *         indicates an error.
 */
static int At91MciBlockRead(NUTFILE * nfp, void *buffer, int num)
{
    MCIFCB *fcb = (MCIFCB *) nfp->nf_fcb;
    uint32_t blk = fcb->fcb_blknum;
    NUTDEVICE *dev = (NUTDEVICE *) nfp->nf_dev;
    MCIFC *ifc = (MCIFC *) dev->dev_icb;

    if (buffer == 0) {
        buffer = fcb->fcb_blkbuf;
    }
    blk += fcb->fcb_part.part_sect_offs;

    if (At91MciReadSingle(ifc, blk, buffer) == 0) {
        return 1;
    }
    return -1;
}

/*!
 * \brief Write data blocks to a mounted partition.
 *
 * \note Not implemented. Always returns -1.
 *
 * Applications should not call this function directly, but use the
 * stdio interface.
 *
 * \param nfp    Pointer to a \ref NUTFILE structure, obtained by a previous 
 *               call to At91MciMount().
 * \param buffer Pointer to the data to be written.
 * \param num    Maximum number of blocks to write. However, writing
 *               multiple blocks is not yet supported by this driver.
 *
 * \return The number of blocks written. A return value of -1 indicates an 
 *         error.
 */
static int At91MciBlockWrite(NUTFILE * nfp, CONST void *buffer, int num)
{
    return -1;
}

/*!
 * \brief Unmount a previously mounted partition.
 *
 * Applications should not directly call this function, but use the high
 * level stdio routines for closing a previously opened file.
 *
 * \return 0 on success, -1 otherwise.
 */
static int At91MciUnmount(NUTFILE * nfp)
{
    int rc = -1;

    if (nfp) {
        MCIFCB *fcb = (MCIFCB *) nfp->nf_fcb;

        if (fcb) {
            rc = fcb->fcb_fsdev->dev_ioctl(fcb->fcb_fsdev, FS_VOL_UNMOUNT, NULL);
            NutHeapFree(fcb);
        }
        NutHeapFree(nfp);
    }
    return rc;
}

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
static NUTFILE *At91MciMount(NUTDEVICE * dev, CONST char *name, int mode, int acc)
{
    int partno = 0;
    int i;
    NUTDEVICE *fsdev;
    NUTFILE *nfp;
    MCIFCB *fcb;
    DOSPART *part;
    MCIFC *ifc = (MCIFC *) dev->dev_icb;
    FSCP_VOL_MOUNT mparm;

    if (At91MciDiscover(ifc)) {
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
#ifdef NUTDEBUG
    printf("['%s'-PART%d]", name, partno);
#endif

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

    if ((fcb = NutHeapAllocClear(sizeof(MCIFCB))) == 0) {
        errno = ENOMEM;
        return NUTFILE_EOF;
    }
    fcb->fcb_fsdev = fsdev;

    /* Initialize MMC access mutex semaphore. */
    NutEventPost(&mutex);

    /* Read MBR. */
    if (At91MciReadSingle(ifc, 0, fcb->fcb_blkbuf)) {
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
        At91MciUnmount(nfp);
        return NUTFILE_EOF;
    }
    return nfp;
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
 *             - \ref NUTBLKDEV_MEDIAAVAIL
 *             - \ref NUTBLKDEV_MEDIACHANGE
 *             - \ref NUTBLKDEV_INFO
 *             - \ref NUTBLKDEV_SEEK
 *             - \ref MMCARD_GETOCR
 *
 * \param conf Points to a buffer that contains any data required for
 *             the given control function or receives data from that
 *             function.
 * \return 0 on success, -1 otherwise.
 */
static int At91MciIOCtrl(NUTDEVICE * dev, int req, void *conf)
{
    int rc = 0;
    MCIFC *ifc = (MCIFC *) dev->dev_icb;

    switch (req) {
    case NUTBLKDEV_MEDIAAVAIL:
        *((int *) conf) = 1;
        break;
    case NUTBLKDEV_MEDIACHANGE:
        *((int *) conf) = 0;
        break;
    case NUTBLKDEV_INFO:
        {
            BLKPAR_INFO *par = (BLKPAR_INFO *) conf;
            MCIFCB *fcb = (MCIFCB *) par->par_nfp->nf_fcb;

            par->par_nblks = fcb->fcb_part.part_sects;
            par->par_blksz = MMC_BLOCK_SIZE;
            par->par_blkbp = fcb->fcb_blkbuf;
        }
        break;
    case NUTBLKDEV_SEEK:
        {
            BLKPAR_SEEK *par = (BLKPAR_SEEK *) conf;
            MCIFCB *fcb = (MCIFCB *) par->par_nfp->nf_fcb;

            fcb->fcb_blknum = par->par_blknum;
        }
        break;
    case MMCARD_GETOCR:
        *((uint32_t *) conf) = ifc->ifc_opcond;
        break;
    default:
        rc = -1;
        break;
    }
    return rc;
}

static MCIFC mci0_info;

/*!
 * \brief Multimedia card device information structure.
 *
 * A pointer to this structure must be passed to NutRegisterDevice()
 * to bind this driver to the Nut/OS kernel. An application may then
 * call
 * /verbatim
 * _open("MCI0:", _O_RDWR | _O_BINARY);
 * /endverbatim
 * to mount the first active primary partition with any previously
 * registered file system driver (typically devPhat0).
 */
NUTDEVICE devAt91Mci0 = {
    0,                          /*!< Pointer to next device, dev_next. */
    {'M', 'C', 'I', '0', 0, 0, 0, 0, 0}
    ,                           /*!< Unique device name, dev_name. */
    0,                          /*!< Type of device, dev_type. Obsolete. */
    0,                          /*!< Base address, dev_base. Unused. */
    0,                          /*!< First interrupt number, dev_irq. Unused. */
    &mci0_info,                 /*!< Interface control block, dev_icb. */
    0,                          /*!< Driver control block used by the low level part, dev_dcb. */
    At91MciInit,                /*!< Driver initialization routine, dev_init. */
    At91MciIOCtrl,              /*!< Driver specific control function, dev_ioctl. */
    At91MciBlockRead,           /*!< Read data from a file, dev_read. */
    At91MciBlockWrite,          /*!< Write data to a file, dev_write. */
    At91MciMount,               /*!< Mount a file system, dev_open. */
    At91MciUnmount,             /*!< Unmount a file system, dev_close. */
    0                           /*!< Return file size, dev_size. */
};

/*@}*/
