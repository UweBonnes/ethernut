/*!
 * Copyright (C) 2002 by Call Direct Cellular Solutions Pty. Ltd. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY CALL DIRECT CELLULAR SOLUTIONS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL CALL DIRECT
 * CELLULAR SOLUTIONS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.calldirect.com.au/
 * -
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgement:
 *
 *    This product includes software developed by egnite Software GmbH
 *    and its contributors.
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

/*
 * $Log$
 * Revision 1.2  2003/08/05 20:05:11  haraldkipp
 * DNS removed from interface
 *
 * Revision 1.1.1.1  2003/05/09 14:40:48  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.2  2003/05/06 18:33:50  harald
 * PPP hack for simple UART support, functions reordered.
 *
 * Revision 1.1  2003/03/31 14:53:08  harald
 * Prepare release 3.1
 *
 */

#include <string.h>
#include <io.h>

#include <net/if_var.h>
#include <net/ppp.h>

#include <sys/heap.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/thread.h>

#include <dev/ppp.h>
#include <dev/ahdlc.h>

#ifdef NUTDEBUG
#include <net/netdebug.h>
#endif

/*!
 * \addtogroup xgPPP
 */
/*@{*/

/* Ugly hack to get simple UART drivers working. */
extern u_char ppp_hackup;

static PPPDCB dcb_ppp;
static char ppp_tx_bsy;
static HANDLE ppp_tx_rdy;

/*
 * Deprecated. Will be included in the AHDLC driver.
 */
static unsigned int PPP_Calc_CRC(unsigned int c)
{
    char i;                     // Just a loop index
    c &= 0xFF;                  // Only calculate CRC on low byte
    for (i = 0; i < 8; i++)     // Loop eight times, once for each bit
    {
        if (c & 1)              // Is bit high?
        {
            c /= 2;             // Position for next bit
            c ^= 0x8408;        // Toggle the feedback bits
        } else
            c /= 2;             // Just position for next bit
    }                           // This routine would be best optimized in assembly
    return c;                   // Return the 16 bit checksum
}

/*
 * Deprecated. Will be included in the AHDLC driver.
 */
static void PPP_AsyncMapOut(int fd, u_char * p, u_short len)
{

    u_char obuff[130];
    u_char i;
    u_short n;

    n = 0;

    while (n < len) {
        // Send 128 byte chunks
        for (i = 0; i < 128;) {
            if ((*p) < 0x20) {
                obuff[i++] = 0x7D;
                obuff[i] = (*p) ^ 0x20;
            } else if ((*p) == 0x7E) {
                obuff[i++] = 0x7D;
                obuff[i] = (*p) ^ 0x20;
            } else if ((*p) == 0x7D) {
                obuff[i++] = 0x7D;
                obuff[i] = (*p) ^ 0x20;
            } else {
                obuff[i] = (*p);
            }
            p++;
            n++;
            i++;
            if (n == len)
                break;
        }
        _write(fd, obuff, i);
    }


    return;
}

/*!
 * \brief Actually send a PPP packet
 *
 * Deprecated. Will be included in the AHDLC driver.
 *
 * \param nb Network buffer structure containing the packet to be sent.
 *           The structure must have been allocated by a previous
 *           call NutNetBufAlloc(). This routine will automatically
 *           release the buffer in case of an error.
 *
 * \return 0 on success, -1 in case of any errors. Errors
 *         will automatically release the network buffer 
 *         structure.
 *
 * \bug Never returns an error.
 */
int PPPPutPacket(NUTDEVICE * dev, NETBUF * nb)
{
    u_short sz;
    u_short CSum;
    u_short i;
    u_char flag = 0x7E;
    u_char *p;
    PPPDCB *dcb = dev->dev_dcb;

    /*
     * Lock duplicate entries. Michael Fischer presented a better
     * example on how to use events for mutex. But this one is
     * OK for now.
     */
    if (ppp_tx_bsy == 1)
        NutEventWait(&ppp_tx_rdy, 0);
    ppp_tx_bsy = 1;

    //
    // Calculate the number of bytes to be send. Do not
    // send packets larger than 1536 bytes.
    //
    sz = nb->nb_dl.sz + nb->nb_nw.sz + nb->nb_tp.sz + nb->nb_ap.sz;
    if (sz >= dcb->dcb_loc_mru) {
#ifdef NUTDEBUG
        if (__ppp_trf) {
            static prog_char dbgfmt[] = "[PPP] sz = %d mru = %d\n";
            fprintf_P(__ppp_trs, dbgfmt, sz, dcb->dcb_loc_mru);
        }
#endif
        NutNetBufFree(nb);
        return -1;
    }

    // Calculate checksum
    CSum = 0xFFFF;

    p = nb->nb_dl.vp;
    for (i = 0; i < nb->nb_dl.sz; i++)
        CSum = PPP_Calc_CRC((*p++) ^ CSum) ^ (CSum / 256);

    p = nb->nb_nw.vp;
    for (i = 0; i < nb->nb_nw.sz; i++)
        CSum = PPP_Calc_CRC((*p++) ^ CSum) ^ (CSum / 256);

    p = nb->nb_tp.vp;
    for (i = 0; i < nb->nb_tp.sz; i++)
        CSum = PPP_Calc_CRC((*p++) ^ CSum) ^ (CSum / 256);

    p = nb->nb_ap.vp;
    for (i = 0; i < nb->nb_ap.sz; i++)
        CSum = PPP_Calc_CRC((*p++) ^ CSum) ^ (CSum / 256);

    _write(dcb->dcb_fd, &flag, 1);

    PPP_AsyncMapOut(dcb->dcb_fd, nb->nb_dl.vp, nb->nb_dl.sz);
    PPP_AsyncMapOut(dcb->dcb_fd, nb->nb_nw.vp, nb->nb_nw.sz);
    PPP_AsyncMapOut(dcb->dcb_fd, nb->nb_tp.vp, nb->nb_tp.sz);
    PPP_AsyncMapOut(dcb->dcb_fd, nb->nb_ap.vp, nb->nb_ap.sz);


    CSum = ~CSum;
    PPP_AsyncMapOut(dcb->dcb_fd, (u_char *) & CSum, 2);
    _write(dcb->dcb_fd, &flag, 1);

    // Let any other threads that are waiting to send PPP packets know it's OK to do so
    ppp_tx_bsy = 0;
    NutEventBroadcastAsync(&ppp_tx_rdy);

    return 0;
}

/*! \fn PPPRx(void *arg)
 * \brief PPP receiver thread.
 *
 * Deprecated. Will be included in the AHDLC driver.
 * 
 * It runs with high priority.
 */

THREAD(PPPRx, arg)
{
    NUTDEVICE *dev;
    IFNET *ifn;
    NETBUF *nb;
    u_char *PPPRxBuff;
    u_char inbuf[1];
    int i;
    char esc;
    unsigned int CSum;
    PPPDCB *dcb;

    dev = arg;
    dcb = dev->dev_dcb;
    ifn = (IFNET *) dev->dev_icb;

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "Enter PPPReceive \n";
        fputs_P(dbgstr, __ppp_trs);
    }
#endif

    while((PPPRxBuff = NutHeapAlloc(PPP_MRU)) == NULL) {
#ifdef NUTDEBUG
        if (__ppp_trf) {
            static prog_char dbgstr[] = "\nFatal Error: Cannot Allocate PPP receive buffer\n";
            fputs_P(dbgstr, __ppp_trs);
        }
#endif
        NutSleep(1000);
    }

    NutThreadSetPriority(18);
    for (;;) {
        /* Wait for network mode. */
        while(ppp_hackup == 0)
            NutSleep(250);

        /* Signal the link driver that we are up. */
        dev->dev_ioctl(dev, LCP_LOWERUP, 0);
        
        // Start off by waiting for a flag byte 
        if ((_read(dcb->dcb_fd, inbuf, 1) == 0) || (inbuf[0] != AHDLC_FLAG))
            continue;

        // Absorb any extra flags that may appear (particularly if we miss a packet)
        // And place the first byte of the packet in inbuf [0]
        while (inbuf[0] == AHDLC_FLAG)
            _read(dcb->dcb_fd, inbuf, 1);

        esc = 0;
        CSum = 0xFFFF;

        // Ok we've got a flag now lets get the rest of the packet
        for (i = 0; i < PPP_MRU; i++) {
            if (inbuf[0] == AHDLC_FLAG) {
                // Complete packet received
                if (CSum == AHDLC_GOODFCS) {
                    i -= 2;     // Don't copy FCS       
                    nb = NutNetBufAlloc(0, NBAF_DATALINK, i);
                    if (nb) {
                        memcpy(nb->nb_dl.vp, PPPRxBuff, i);
                        (*ifn->if_recv) (dev, nb);
                    }
                }
                break;
            } else if (inbuf[0] == AHDLC_ESCAPE) {
                esc = 1;
                i--;
            } else {
                PPPRxBuff[i] = inbuf[0];
                if (esc) {
                    PPPRxBuff[i] ^= 0x20;
                    esc = 0;
                }
                CSum = PPP_Calc_CRC(PPPRxBuff[i] ^ CSum) ^ (CSum / 256);        // Calculate CRC
            }
            // Get the next byte
            while (ppp_hackup && _read(dcb->dcb_fd, inbuf, 1) == 0);
        }
        /* Signal the link driver that we are down. */
        if(ppp_hackup == 0)
            dev->dev_ioctl(dev, LCP_LOWERDOWN, 0);
    }
}

/*
 * Deprecated. The AHDLC driver will be enabled by ioctl.
 */
void NutPppEnable(NUTDEVICE *dev) 
{
    /* If an AHDLC driver has been registered, it must have started 
       a receiver thread with the same name. */
    if (GetThreadByName("ahdlcrx") == NULL)
        NutThreadCreate("ahdlcrx", PPPRx, dev, 512);
}

/*
 * Pass reads to the physical driver for now.
 */
int NutPppRead(NUTFILE * fp, void *buffer, int size)
{
    PPPDCB *pppdcb = fp->nf_dev->dev_dcb;

    return _read(pppdcb->dcb_fd, buffer, size);
}

/*
 * Pass writes to the physical driver for now.
 */
static int NutPppWrite(NUTFILE * fp, CONST void *buffer, int len)
{
    PPPDCB *pppdcb = fp->nf_dev->dev_dcb;

    return _write(pppdcb->dcb_fd, buffer, len);
}

/*
 * Pass writes to the physical driver for now.
 */
static int NutPppWrite_P(NUTFILE * fp, PGM_P buffer, int len)
{
    PPPDCB *pppdcb = fp->nf_dev->dev_dcb;

    return _write_P(pppdcb->dcb_fd, buffer, len);
}

/*!
 * \brief Network interface information structure.
 */
IFNET ifn_ppp = {
    IFT_PPP,                    /*!< \brief Interface type. */
    {0, 0, 0, 0, 0, 0},         /*!< \brief Hardware net address. */
    0,                          /*!< \brief IP address. */
    0,                          /*!< \brief Remote IP address for point to point. */
    0,                          /*!< \brief IP network mask. */
    PPP_MRU,                    /*!< \brief Maximum size of a transmission unit. */
    0,                          /*!< \brief Packet identifier. */
    0,                          /*!< \brief Linked list of arp entries. */
    NutPppInput,                /*!< \brief Routine to pass received data to, if_recv(). */
    0,                          /*!< \brief Dynamically attached driver output routine, if_send(). */
    NutPppOutput                /*!< \brief Media output routine, if_output(). */
};

/*!
 * \brief Device information structure.
 *
 * This is a virtual device driver has no underlying hardware
 * and must not be registered. It will be initialized when the
 * application calls NutNetIfConfig().
 */
NUTDEVICE devPpp = {
    0,              /* Pointer to next device. */
    {'p', 'p', 'p', 0, 0, 0, 0, 0, 0},        /* Unique device name. */
    IFTYP_NET,	    /* Type of device. */
    0,		    /* Base address. */
    0,		    /* First interrupt number. */
    &ifn_ppp,	    /* Interface control block. */
    &dcb_ppp,	    /* Driver control block. */
    NutPppInit,	    /* Driver initialization routine, dev_init(). */
    NutPppIOCtl,    /* Driver specific control function, dev_ioctl(). */
    NutPppRead,     /* Read from device, dev_read. */
    NutPppWrite,    /* Write to device, dev_write. */ 
    NutPppWrite_P,  /* Write data from program space to device, dev_write_P. */
    NutPppOpen,     /* Open a device or file, dev_open. */
    NutPppClose,    /* Close a device or file, dev_close. */
    0		    /* Request file size. */
};

/*@}*/
