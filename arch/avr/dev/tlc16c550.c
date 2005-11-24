/*
 * Copyright (C) 2001-2003 by Cyber Integration, LLC. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY CYBER INTEGRATION, LLC AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL CYBER
 * INTEGRATION, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 */

/*
 * $Log$
 * Revision 1.1  2005/11/24 11:24:06  haraldkipp
 * Initial check-in.
 * Many thanks to William Basser for this code and also to Przemyslaw Rudy
 * for several enhancements.
 *
 */

// system include files
#include <string.h>
#include <sys/atom.h>
#include <sys/heap.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/device.h>
#include <dev/irqreg.h>
#include <dev/tlc16c550.h>
#include <fcntl.h>
#include <stdio.h>
/*
 * Not nice because stdio already defined them. But in order to save memory,
 * we do the whole buffering and including stdio here would be more weird.
 */
#ifndef _IOFBF
#define _IOFBF	    0x00
#define _IOLBF	    0x01
#define _IONBF	    0x02
#endif

/*!
 * \
 */
/*@{*/


// define the register offset
// define the UART Register offsets
#define	ACE_DLSB_OFS	0
#define	ACE_DATA_OFS	0
#define	ACE_DMSB_OFS	1
#define	ACE_INTE_OFS	1
#define	ACE_FCTL_OFS	2
#define	ACE_INTI_OFS	2
#define	ACE_LCTL_OFS	3
#define	ACE_MCTL_OFS	4
#define	ACE_LSTS_OFS	5
#define	ACE_MSTS_OFS	6
#define	ACE_SCRG_OFS	7

// define the interrupt enable masks
#define	INTE_RDA_MSK	0x01    // receiver data avaialbe
#define	INTE_THE_MSK	0x02    // transmit holding empty
#define	INTE_LST_MSK	0x04    // line status
#define	INTE_MST_MSK	0x08    // modem status

// define the fifo control mask
#define FCTL_ENABLE     0x01    //fifo enable
#define FCTL_LEVEL_1    0x00    //receive trigger level 1
#define FCTL_LEVEL_4    0x40    //receive trigger level 4
#define FCTL_LEVEL_8    0x80    //receive trigger level 8
#define FCTL_LEVEL_14   0xc0    //receive trigger level 14

// define the interrupt id masks
#define	INTI_MST_MSK	0x00    // modem status interrupt
#define	INTI_TXE_MSK	0x02    // transmit buffer empty
#define	INTI_RDA_MSK	0x04    // receive data available
#define	INTI_TDA_MSK	0x0c    // timeout receive data available
#define	INTI_LST_MSK	0x06    // line status interrupt
#define	INTI_NON_MSK	0x01    // no interrupt
#define INTI_FIFO_MSK   0xc0    // mask to eliminate the fifo status

// define the line control masks
#define	LCTL_WS0_MSK	0x01
#define	LCTL_WS1_MSK	0x02
#define	LCTL_STB_MSK	0x04
#define	LCTL_PEN_MSK	0x08
#define	LCTL_PRE_MSK	0x10
#define	LCTL_PRS_MSK	0x20
#define	LCTL_BRK_MSK	0x40
#define	LCTL_ENB_MSK	0x80

// define the modem control masks
#define	MCTL_DTR_MSK	0x01
#define	MCTL_RTS_MSK	0x02
#define	MCTL_GP1_MSK	0x04
#define	MCTL_GP2_MSK	0x08
#define	MCTL_LOP_MSK	0x10

// define the line status masks
#define	LSTS_RDR_MSK	0x01
#define	LSTS_OVR_MSK	0x02
#define	LSTS_PER_MSK	0x04
#define	LSTS_FER_MSK	0x08
#define	LSTS_BDT_MSK	0x10
#define	LSTS_THE_MSK	0x20
#define	LSTS_TXE_MSK	0x40

// define the modem status masks
#define	MSTS_DCTS_MSK	0x01
#define	MSTS_DDSR_MSK	0x02
#define	MSTS_DRI_MSK	0x04
#define	MSTS_DDCD_MSK	0x08
#define	MSTS_CTS_MSK	0x10
#define	MSTS_DSR_MSK	0x20
#define	MSTS_RI_MSK		0x40
#define	MSTS_DCD_MSK	0x80
u_char hackup[50] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// define the irq structure
typedef struct tagIRQDEFS {
    IRQ_HANDLER *pvIrq;
    volatile u_char *pnIrqMskPort;
    u_char nMask;
} IRQDEFS;

// define the interrupt handlers
static CONST PROGMEM IRQDEFS atIrqDefs[] = {
    {&sig_INTERRUPT0, &EICRA, 0x03},
    {&sig_INTERRUPT1, &EICRA, 0x0C},
    {&sig_INTERRUPT2, &EICRA, 0x30},
    {&sig_INTERRUPT3, &EICRA, 0xc0},
    {&sig_INTERRUPT4, &EICRB, 0x03},
    {&sig_INTERRUPT5, &EICRB, 0x0C},
    {&sig_INTERRUPT6, &EICRB, 0x30},
    {&sig_INTERRUPT7, &EICRB, 0xC0},
};

// define the dcb's asigned to the interrupt to have more than one device on the same interrupt
// NUT intrnal irq structure could be used instead but that would be a hack
static ACEDCB *atIrqDcb[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };


/*
 * Handle AVR ACE interrupts
 */
static void AceIrqHandler(void *arg)
{
    NUTDEVICE *dev = (NUTDEVICE *) arg;
    IFSTREAM *ifs;
    ACEDCB *dcb;
    volatile u_char event;
    u_char maxData;

    do {
        ifs = dev->dev_icb;
        dcb = dev->dev_dcb;
        // get the interrupt source
        while (((event = *(u_char *) (dev->dev_base + ACE_INTI_OFS)) & ~INTI_FIFO_MSK) != INTI_NON_MSK) {
            switch (event & ~INTI_FIFO_MSK) {
            case INTI_RDA_MSK: // receive data available
            case INTI_TDA_MSK: // timeout receive data available
                /* maxData can be avoided but it ensures that for slow system and fast uart we will not get stuck
                 * reading incomming data all the time.
                 */
                maxData = (dcb->dcb_rfifo == 0) ? 1 : dcb->dcb_rfifo;
                for (; (*(u_char *) (dev->dev_base + ACE_LSTS_OFS) & LSTS_RDR_MSK) && (maxData > 0); --maxData) {
                    // get the character and store it//increment count/adjust pointers
                    ifs->if_rx_buf[ifs->if_rx_idx] = *(u_char *) (dev->dev_base + ACE_DATA_OFS);
                    /* if we have just received a first byte into the empty buffer */
                    if (ifs->if_rd_idx == ifs->if_rx_idx) {
                        NutEventPostAsync(&(dcb->dcb_rx_rdy));
                    }
                    /* Late increment fixes ICCAVR bug on volatile variables. */
                    ifs->if_rx_idx++;
                }
                break;

            case INTI_TXE_MSK: // transmit buffer empty
                dcb->dcb_wfifo = (event & INTI_FIFO_MSK) ? 16 : 1;
                if (ifs->if_tx_idx != ifs->if_wr_idx) {
                    for (; (ifs->if_tx_idx != ifs->if_wr_idx) && dcb->dcb_wfifo; --dcb->dcb_wfifo) {
                        // send a character
                        *(u_char *) (dev->dev_base + ACE_DATA_OFS) = ifs->if_tx_buf[ifs->if_tx_idx++];
                    }
                } else {
                    ifs->if_tx_act = 0;
                    NutEventPostAsync(&(dcb->dcb_tx_rdy));
                }
                break;

            case INTI_MST_MSK: // modem status interrupt
                break;

            case INTI_LST_MSK: // line status interrupt
                break;
            }
        }
        /* get the next device assigned to this interrupt */
        dev = dcb->dev_next;
    } while (dev != NULL);
}

/*!
 * \brief Wait for input.
 *
 * This function checks the input buffer for any data. If
 * the buffer is empty, the calling \ref xrThread "thread" 
 * will be blocked until at least one new character is 
 * received or a timeout occurs.
 *
 * \param dev Indicates the ACE device.
 *
 * \return 0 on success, -1 on timeout.
 */
int AceInput(NUTDEVICE * dev)
{
    int rc = 0;
    IFSTREAM *ifs = dev->dev_icb;
    ACEDCB *dcb = dev->dev_dcb;

    if (ifs->if_rd_idx == ifs->if_rx_idx) {
        /*
         * Changing if into a while loop fixes a serious bug: 
         * Previous receiver events without any waiting thread 
         * set the event handle to the signaled state. So the
         * wait returns immediately. Unfortunately the calling
         * routines rely on a filled buffer when we return 0.
         * Thanks to Mike Cornelius, who found this bug.
         */
        do {
            rc = NutEventWait(&(dcb->dcb_rx_rdy), dcb->dcb_rtimeout);
        } while ((rc == 0) && (ifs->if_rd_idx == ifs->if_rx_idx));
    }

    return rc;
}

/*!
 * \brief Initiate output.
 *
 * This function checks the output buffer for any data. If
 * the buffer contains at least one character, the transmitter
 * is started, if not already running. The function returns
 * immediately, without waiting for the character being
 * completely transmitted. Any remaining characters in the
 * output buffer are transmitted in the background.
 *
 * \param dev Indicates the ACE device.
 *
 * \return 0 on success, -1 otherwise.
 */
int AceOutput(NUTDEVICE * dev)
{
    IFSTREAM *ifs = dev->dev_icb;
    ACEDCB *dcb = dev->dev_dcb;

    if ((ifs->if_tx_act == 0) && (ifs->if_tx_idx != ifs->if_wr_idx)) {
        ifs->if_tx_act = 1;
        for (; (ifs->if_tx_idx != ifs->if_wr_idx) && (dcb->dcb_wfifo > 0); --dcb->dcb_wfifo) {
            // send a character
            *(u_char *) (dev->dev_base + ACE_DATA_OFS) = ifs->if_tx_buf[ifs->if_tx_idx++];
        }
        // no need to enable an interrupt here as it should be enabled all the time
    }
    return 0;
}

/*!
 * \brief Wait for output buffer empty.
 *
 * If the output buffer contains any data, the calling
 * thread is suspended until all data has been transmitted.
 *
 * \param dev Indicates the ACE device.
 *
 * \return 0 on success, -1 otherwise.
 */
int AceFlush(NUTDEVICE * dev)
{
    IFSTREAM *ifs = dev->dev_icb;
    ACEDCB *dcb = dev->dev_dcb;

    /*
     * Start any pending output.
     */
    if (AceOutput(dev))
        return -1;

    /*
     * Wait until output buffer empty.
     */
    while (ifs->if_tx_idx != ifs->if_wr_idx)
        NutEventWait(&dcb->dcb_tx_rdy, 100);

    return 0;
}

/*
 *
 * \param dev Indicates the ACE device.
 *
 * \return 0 on success, -1 otherwise.
 */
static int AceGetStatus(NUTDEVICE * dev, u_long * status)
{
    IFSTREAM *ifs = dev->dev_icb;
    u_char us;

    *status = 0;

    us = *(u_char *) (dev->dev_base + ACE_LSTS_OFS);
    if (us & LSTS_FER_MSK)
        *status |= ACE_FRAMINGERROR;
    if (us & LSTS_OVR_MSK)
        *status |= ACE_OVERRUNERROR;
    if (ifs->if_tx_idx == ifs->if_wr_idx)
        *status |= ACE_TXBUFFEREMPTY;
    if (ifs->if_rd_idx == ifs->if_rx_idx)
        *status |= ACE_RXBUFFEREMPTY;
    return 0;
}

/*
 * Carefully enable ACE functions.
 */
static void AceEnable(u_short base)
{
    /*volatile u_char* pnBase = *(volatile u_char* )base; */

    /*
     * Enable ACE interrupts.
     */
    NutEnterCritical();
    *(u_char *) (base + ACE_INTE_OFS) = INTE_RDA_MSK | INTE_THE_MSK;
    NutExitCritical();
}

/*
 * Carefully disable ACE functions.
 */
static void AceDisable(u_short base)
{
    /*volatile u_char* pnBase = *(volatile u_char* )base; */

    /*
     * Disable ACE interrupts.
     */
    NutEnterCritical();
    *(u_char *) (base + ACE_INTE_OFS) &= (u_char) ~ (INTE_RDA_MSK);
    NutExitCritical();

    /*
     * Allow incoming or outgoing character to finish.
     */
    NutDelay(10);
}

/*!
 * \brief Perform ACE control functions.
 *
 * \param dev  Identifies the device that receives the device-control
 *             function.
 * \param req  Requested control function. May be set to one of the
 *             following constants:
 *             - ACE_SETSPEED, conf points to an u_long value containing the baudrate.
 *             - ACE_GETSPEED, conf points to an u_long value receiving the current baudrate.
 *             - ACE_SETDATABITS, conf points to an u_long value containing the number of data bits, 5, 6, 7 or 8.
 *             - ACE_GETDATABITS, conf points to an u_long value receiving the number of data bits, 5, 6, 7 or 8.
 *             - ACE_SETPARITY, conf points to an u_long value containing the parity, 0 (no), 1 (odd) or 2 (even).
 *             - ACE_GETPARITY, conf points to an u_long value receiving the parity, 0 (no), 1 (odd) or 2 (even).
 *             - ACE_SETSTOPBITS, conf points to an u_long value containing the number of stop bits 1 or 2.
 *             - ACE_GETSTOPBITS, conf points to an u_long value receiving the number of stop bits 1 or 2.
 *             - ACE_SETSTATUS
 *             - ACE_GETSTATUS
 *             - ACE_SETREADTIMEOUT, conf points to an u_long value containing the read timeout.
 *             - ACE_GETREADTIMEOUT, conf points to an u_long value receiving the read timeout.
 *             - ACE_SETWRITETIMEOUT, conf points to an u_long value containing the write timeout.
 *             - ACE_GETWRITETIMEOUT, conf points to an u_long value receiving the write timeout.
 *             - ACE_SETLOCALECHO, conf points to an u_long value containing 0 (off) or 1 (on).
 *             - ACE_GETLOCALECHO, conf points to an u_long value receiving 0 (off) or 1 (on).
 *             - ACE_SETFLOWCONTROL, conf points to an u_long value containing combined ACE_FCTL_ values.
 *             - ACE_GETFLOWCONTROL, conf points to an u_long value containing receiving ACE_FCTL_ values.
 *             - ACE_SETCOOKEDMODE, conf points to an u_long value containing 0 (off) or 1 (on).
 *             - ACE_GETCOOKEDMODE, conf points to an u_long value receiving 0 (off) or 1 (on).
 *
 * \param conf Points to a buffer that contains any data required for
 *             the given control function or receives data from that
 *             function.
 * \return 0 on success, -1 otherwise.
 *
 * \warning Timeout values are given in milliseconds and are limited to 
 *          the granularity of the system timer. To disable timeout,
 *          set the parameter to NUT_WAIT_INFINITE.
 *
 *
 */
int AceIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    int rc = 0;
    ACEDCB *dcb;
    IFSTREAM *ifs;
    u_long *lvp = (u_long *) conf;
    u_long lv = *lvp;
    u_char bv = (u_char) lv;
    u_short sv;
    u_short devnum;
    u_char tv;

    if (dev == 0) {
        rc = -1;
        return rc;
    }

    devnum = dev->dev_base;
    dcb = dev->dev_dcb;

    switch (req) {
    case ACE_SETSPEED:
        AceDisable(devnum);
        sv = (u_short) (ACE_CLOCK / (lv * 16UL));

        *(u_char *) (dev->dev_base + ACE_LCTL_OFS) |= LCTL_ENB_MSK;
        *(u_char *) (dev->dev_base + ACE_DLSB_OFS) = sv & 0xFF;
        *(u_char *) (dev->dev_base + ACE_DMSB_OFS) = (u_char) sv >> 8;
        *(u_char *) (dev->dev_base + ACE_LCTL_OFS) &= (u_char) ~ LCTL_ENB_MSK;
        AceEnable(devnum);
        break;

    case ACE_GETSPEED:
        *(u_char *) (dev->dev_base + ACE_LCTL_OFS) |= LCTL_ENB_MSK;
        sv = *(u_char *) (dev->dev_base + ACE_DLSB_OFS);
        sv |= *(u_char *) (dev->dev_base + ACE_DMSB_OFS) >> 8;
        *(u_char *) (dev->dev_base + ACE_LCTL_OFS) &= (u_char) ~ LCTL_ENB_MSK;
        *lvp = ACE_CLOCK / (16UL * (u_long) (sv));
        break;

    case ACE_SETDATABITS:
        AceDisable(devnum);
        if ((bv >= 5) && (bv <= 8)) {
            bv -= 5;
            tv = *(u_char *) (dev->dev_base + ACE_LCTL_OFS);
            tv &= (u_char) ~ (LCTL_WS0_MSK | LCTL_WS1_MSK);
            tv |= (bv & (LCTL_WS0_MSK | LCTL_WS1_MSK));
            *(u_char *) (dev->dev_base + ACE_LCTL_OFS) = tv;
        }
        AceEnable(devnum);
        break;

    case ACE_GETDATABITS:
        *lvp = *(u_char *) (dev->dev_base + ACE_LCTL_OFS) & (LCTL_WS0_MSK | LCTL_WS1_MSK);
        break;

    case ACE_SETPARITY:
        AceDisable(devnum);
        if (bv <= 4) {
            switch (bv) {
            case 1:            // odd parity
                bv = LCTL_PEN_MSK;
                break;

            case 2:            // event parity
                bv = LCTL_PEN_MSK | LCTL_PRE_MSK;
                break;

            case 3:            // space
                bv = LCTL_PEN_MSK;
                break;

            case 4:            // mark
                bv = LCTL_PEN_MSK | LCTL_PRS_MSK;

            default:           // no parity
                bv = 0;
                break;
            }

            tv = *(u_char *) (dev->dev_base + ACE_LCTL_OFS);
            tv &= (u_char) ~ (LCTL_PEN_MSK | LCTL_PRE_MSK | LCTL_PRS_MSK);
            tv |= bv;
            *(u_char *) (dev->dev_base + ACE_LCTL_OFS) = tv;
        }
        AceEnable(devnum);
        break;

    case ACE_GETPARITY:
        tv = *(u_char *) (dev->dev_base + ACE_LCTL_OFS) & (LCTL_PEN_MSK | LCTL_PRE_MSK | LCTL_PRS_MSK);
        switch (tv) {
        case 0:
            *lvp = 0;           // no parity
            break;

        case LCTL_PEN_MSK:
            *lvp = 1;           // odd parity
            break;

        case LCTL_PEN_MSK | LCTL_PRE_MSK:
            *lvp = 2;           // event parity
            break;

        case LCTL_PEN_MSK | LCTL_PRS_MSK:
            *lvp = 4;           // mark parity
            break;
        }
        break;

    case ACE_SETSTOPBITS:
        AceDisable(devnum);
        if (bv == 1 || bv == 2) {
            tv = *(u_char *) (dev->dev_base + ACE_LCTL_OFS);
            tv &= (u_char) ~ (LCTL_STB_MSK);
            tv |= (bv == 2) ? LCTL_STB_MSK : 0;
            *(u_char *) (dev->dev_base + ACE_LCTL_OFS) = tv;
        }
        AceEnable(devnum);
        break;

    case ACE_GETSTOPBITS:
        tv = *(u_char *) (dev->dev_base + ACE_LCTL_OFS);
        *lvp = (tv & LCTL_STB_MSK) ? 2 : 1;
        break;

    case ACE_SETFIFO:
        AceDisable(devnum);
        dcb->dcb_wfifo = 16;
        switch (bv) {
        case 1:
            tv = FCTL_ENABLE | FCTL_LEVEL_1;
            break;

        case 4:
            tv = FCTL_ENABLE | FCTL_LEVEL_4;
            break;

        case 8:
            tv = FCTL_ENABLE | FCTL_LEVEL_8;
            break;

        case 14:
            tv = FCTL_ENABLE | FCTL_LEVEL_14;
            break;

        default:
            bv = 0;
            tv = 0;
            dcb->dcb_wfifo = 1;
            break;
        }

        *(u_char *) (dev->dev_base + ACE_FCTL_OFS) = tv;
        /* if enabling then must write the level after */
        *(u_char *) (dev->dev_base + ACE_FCTL_OFS) = tv;
        dcb->dcb_rfifo = bv;

        /* must signal any active and waiting writer, discard pending data */
        ifs = dev->dev_icb;

        ifs->if_tx_act = 0;
        ifs->if_tx_idx = ifs->if_wr_idx;
        NutEventPostAsync(&(dcb->dcb_tx_rdy));

        AceEnable(devnum);
        break;

    case ACE_GETFIFO:
        *lvp = (u_long) (dcb->dcb_rfifo);
        break;

    case ACE_GETSTATUS:
        AceGetStatus(dev, lvp);
        break;

    case ACE_SETSTATUS:
        rc = -1;
        break;

    case ACE_SETREADTIMEOUT:
        dcb->dcb_rtimeout = lv;
        break;
    case ACE_GETREADTIMEOUT:
        *lvp = dcb->dcb_rtimeout;
        break;

    case ACE_SETWRITETIMEOUT:
        dcb->dcb_wtimeout = lv;
        break;
    case ACE_GETWRITETIMEOUT:
        *lvp = dcb->dcb_wtimeout;
        break;

    case ACE_SETLOCALECHO:
        if (bv)
            dcb->dcb_modeflags |= ACE_MF_LOCALECHO;
        else
            dcb->dcb_modeflags &= ~ACE_MF_LOCALECHO;
        break;
    case ACE_GETLOCALECHO:
        *lvp = (dcb->dcb_modeflags & ACE_MF_LOCALECHO) ? 1 : 0;
        break;

    case ACE_SETFLOWCONTROL:
        if (bv)
            dcb->dcb_modeflags |= ACE_MF_LOCALECHO;
        else
            dcb->dcb_modeflags &= ~ACE_MF_LOCALECHO;
        break;
    case ACE_GETFLOWCONTROL:
        break;

    case ACE_SETCOOKEDMODE:
        if (bv)
            dcb->dcb_modeflags |= ACE_MF_COOKEDMODE;
        else
            dcb->dcb_modeflags &= ~ACE_MF_COOKEDMODE;
        break;
    case ACE_GETCOOKEDMODE:
        *lvp = (dcb->dcb_modeflags & ACE_MF_COOKEDMODE) ? 1 : 0;
        break;

    default:
        rc = -1;
        break;
    }
    return rc;
}

/*!
 * \brief Initialize on chip ACE device.
 *
 * Prepares the device for subsequent reading or writing.
 * Enables ACE transmitter and receiver interrupts.
 *
 * \param dev  Identifies the device to initialize.
 *
 * \return 0 on success, -1 otherwise.
 */
int AceInit(NUTDEVICE * dev)
{
    IFSTREAM *ifs;
    ACEDCB *dcb;
    u_long baudrate = 9600;
    u_long databits = 8;
    u_long parity = 0;
    u_long stopbits = 1;

    IRQ_HANDLER *irq;
    u_char *pnPort;
    u_char nMask;

    /*
     * We only support character devices for on-chip ACEs.
     */
    if (dev->dev_type != IFTYP_STREAM) {
        return -1;
    }

    /*
     * Initialize interface control block.
     */
    ifs = dev->dev_icb;
    memset(ifs, 0, sizeof(IFSTREAM));
    ifs->if_input = AceInput;
    ifs->if_output = AceOutput;
    ifs->if_flush = AceFlush;

    /*
     * Initialize driver control block.
     */
    dcb = dev->dev_dcb;
    memset(dcb, 0, sizeof(ACEDCB));
    dcb->dcb_modeflags = ACE_MF_NOBUFFER;
    dcb->dcb_rfifo = 0;
    dcb->dcb_wfifo = 1;
    dcb->dev_next = NULL;
    /*
     * Register interrupt handler.
     */
    if (dev->dev_base) {
        /* if any ACE is already assigned to this interrupt */
        if (atIrqDcb[dev->dev_irq] != NULL) {
            /* put ourself on the end of the list */
            (atIrqDcb[dev->dev_irq])->dev_next = dev;
        } else {
            // get the appropriate irq handler
            irq = (IRQ_HANDLER *) pgm_read_word(&(atIrqDefs[dev->dev_irq].pvIrq));
            if (NutRegisterIrqHandler(irq, AceIrqHandler, dev)) {
                return -1;
            }
            // enable the interrupts
            pnPort = (u_char *) pgm_read_word(&(atIrqDefs[dev->dev_irq].pnIrqMskPort));
            nMask = pgm_read_byte(&(atIrqDefs[dev->dev_irq].nMask));
            *pnPort |= nMask;
        }
        /* remember dcb of the recently initialized device */
        atIrqDcb[dev->dev_irq] = dcb;
    }

    /*
     * Set baudrate and handshake default. This will also
     * enable the ACE functions.
     */
    AceIOCtl(dev, ACE_SETSPEED, (void *) &baudrate);
    AceIOCtl(dev, ACE_SETDATABITS, (void *) &databits);
    AceIOCtl(dev, ACE_SETPARITY, (void *) &parity);
    AceIOCtl(dev, ACE_SETSTOPBITS, (void *) &stopbits);

    sbi(EIMSK, dev->dev_irq);   /* dev->dev_irq to IRQ_INTx map should be used for a clean implementation but it looks like an overhead */

    AceEnable(dev->dev_base);

    return 0;
}

/*! 
 * \brief Read from device. 
 */
int AceRead(NUTFILE * fp, void *buffer, int size)
{
    int rc;
    NUTDEVICE *dev;
    IFSTREAM *ifs;
    ACEDCB *dcb;
    u_char elmode;
    u_char ch;
    u_char *cp = buffer;

    dev = fp->nf_dev;
    ifs = (IFSTREAM *) dev->dev_icb;
    dcb = dev->dev_dcb;

    if (dcb->dcb_modeflags & ACE_MF_COOKEDMODE)
        elmode = 1;
    else
        elmode = 0;

    /*
     * Call without data pointer discards receive buffer.
     */
    if (buffer == 0) {
        ifs->if_rd_idx = ifs->if_rx_idx;
        return 0;
    }

    /*
     * Get characters from receive buffer.
     */
    for (rc = 0; rc < size;) {
        /* if nothing has been received yet */
        if (ifs->if_rd_idx == ifs->if_rx_idx) {
            /* while incomming buffer is empty */
            while (ifs->if_rd_idx == ifs->if_rx_idx) {
                /* wait (timeout) for incomming data */
                if (AceInput(dev)) {
                    /* if a timeout */
                    return 0;
                }
            }
        }

        ch = ifs->if_rx_buf[ifs->if_rd_idx++];
        if (elmode && (ch == '\r' || ch == '\n')) {
            if ((ifs->if_last_eol == 0) || (ifs->if_last_eol == ch)) {
                ifs->if_last_eol = ch;
                *cp++ = '\n';
                rc++;
            }
        } else {
            ifs->if_last_eol = 0;
            *cp++ = ch;
            rc++;
        }
    }
    return rc;
}

/*! 
 * \brief Write to device. 
 */
int AcePut(NUTDEVICE * dev, CONST void *buffer, int len, int pflg)
{
    int rc;
    IFSTREAM *ifs;
    ACEDCB *dcb;
    CONST u_char *cp;
    u_char lbmode;
    u_char elmode;
    u_char ch;

    ifs = dev->dev_icb;
    dcb = dev->dev_dcb;

    if (dcb->dcb_modeflags & ACE_MF_LINEBUFFER)
        lbmode = 1;
    else
        lbmode = 0;

    if (dcb->dcb_modeflags & ACE_MF_COOKEDMODE)
        elmode = 1;
    else
        elmode = 0;

    /*
     * Call without data pointer starts transmission.
     */
    if (buffer == 0) {
        rc = AceFlush(dev);
        return rc;
    }

    /*
     * Put characters in transmit buffer.
     */
    cp = buffer;
    for (rc = 0; rc < len;) {
        if ((u_char) (ifs->if_wr_idx + 1) == ifs->if_tx_idx) {
            if (AceFlush(dev)) {
                return -1;
            }
        }
        ch = pflg ? PRG_RDB(cp) : *cp;
        if (elmode == 1 && ch == '\n') {
            elmode = 2;
            if (lbmode == 1)
                lbmode = 2;
            ch = '\r';
        } else {
            if (elmode == 2)
                elmode = 1;
            cp++;
            rc++;
        }
        ifs->if_tx_buf[ifs->if_wr_idx++] = ch;
    }

    if (lbmode > 1 || (dcb->dcb_modeflags & ACE_MF_NOBUFFER) != 0) {
        if (AceFlush(dev))
            rc = -1;
    }
    return rc;
}

int AceWrite(NUTFILE * fp, CONST void *buffer, int len)
{
    return AcePut(fp->nf_dev, buffer, len, 0);
}

int AceWrite_P(NUTFILE * fp, PGM_P buffer, int len)
{
    return AcePut(fp->nf_dev, (CONST char *) buffer, len, 1);
}


/*! 
 * \brief Open a device or file.
 */
NUTFILE *AceOpen(NUTDEVICE * dev, CONST char *name, int mode, int acc)
{
    NUTFILE *fp = NutHeapAlloc(sizeof(NUTFILE));
    ACEDCB *dcb;

    if (fp == 0)
        return NUTFILE_EOF;

    dcb = dev->dev_dcb;
    if (mode & _O_BINARY)
        dcb->dcb_modeflags &= ~ACE_MF_COOKEDMODE;
    else
        dcb->dcb_modeflags |= ACE_MF_COOKEDMODE;

    fp->nf_next = 0;
    fp->nf_dev = dev;
    fp->nf_fcb = 0;

    return fp;
}

/*! 
 * \brief Close a device or file. 
 */
int AceClose(NUTFILE * fp)
{
    NutHeapFree(fp);

    return 0;
}

/*! 
 * \brief Request file size.
 */
long AceSize(NUTFILE * fp)
{
    NUTDEVICE *dev;
    IFSTREAM *ifs;

    dev = fp->nf_dev;
    ifs = (IFSTREAM *) dev->dev_icb;
    return ((u_char) (ifs->if_rx_idx - ifs->if_rd_idx));
}


/*@}*/
