/*
 * Copyright (C) 2003 by egnite Software GmbH. All rights reserved.
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
 *
 */

/*
 * $Log$
 * Revision 1.5  2004/01/14 19:31:43  drsung
 * Speed improvement to NicWrite applied. Thanks to Kolja Waschk
 *
 * Revision 1.4  2003/11/06 09:26:50  haraldkipp
 * Removed silly line with hardcoded MAC, left over from testing
 *
 * Revision 1.3  2003/11/04 17:54:47  haraldkipp
 * PHY configuration timing changed again for reliable linking
 *
 * Revision 1.2  2003/11/03 17:12:53  haraldkipp
 * Allow linking with RTL8019 driver.
 * Links more reliable to 10 MBit networks now.
 * Reset MMU on allocation failures.
 * Some optimizations.
 *
 * Revision 1.1  2003/10/13 10:13:49  haraldkipp
 * First release
 *
 */

#include <string.h>

#include <cfg/ethernut.h>

#include <sys/atom.h>
#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/confnet.h>

#include <netinet/if_ether.h>
#include <net/ether.h>
#include <net/if_var.h>

#include <dev/irqreg.h>
#include <dev/lanc111.h>

#ifdef NUTDEBUG
#include <stdio.h>
#endif

/*!
 * \addtogroup xgSmscRegs
 */
/*@{*/


/*! 
 * \brief Bank select register. 
 */
#define NIC_BSR         NIC_BASE + 0x0E

/*! 
 * \brief Bank 0 - Transmit control register. 
 */
#define NIC_TCR         NIC_BASE + 0x00

#define TCR_SWFDUP      0x8000  /*!< \ref NIC_TCR bit mask, enables full duplex.  */
#define TCR_EPH_LOOP    0x2000  /*!< \ref NIC_TCR bit mask, enables internal loopback. */
#define TCR_STP_SQET    0x1000  /*!< \ref NIC_TCR bit mask, enables transmission stop on SQET error. */
#define TCR_FDUPLX      0x0800  /*!< \ref NIC_TCR bit mask, enables receiving own frames. */
#define TCR_MON_CSN     0x0400  /*!< \ref NIC_TCR bit mask, enables carrier monitoring. */
#define TCR_NOCRC       0x0100  /*!< \ref NIC_TCR bit mask, disables CRC transmission. */
#define TCR_PAD_EN      0x0080  /*!< \ref NIC_TCR bit mask, enables automatic padding. */
#define TCR_FORCOL      0x0004  /*!< \ref NIC_TCR bit mask, forces collision. */
#define TCR_LOOP        0x0002  /*!< \ref NIC_TCR bit mask, enables PHY loopback. */
#define TCR_TXENA       0x0001  /*!< \ref NIC_TCR bit mask, enables transmitter. */


/*! 
 * \brief Bank 0 - EPH status register. 
 */
#define NIC_EPHSR       NIC_BASE + 0x02

/*! 
 * \brief Bank 0 - Receive control register. 
 */
#define NIC_RCR         NIC_BASE + 0x04

#define RCR_SOFT_RST    0x8000  /*!< \ref NIC_RCR bit mask, activates software reset. */
#define RCR_FILT_CAR    0x4000  /*!< \ref NIC_RCR bit mask, enables carrier filter. */
#define RCR_ABORT_ENB   0x2000  /*!< \ref NIC_RCR bit mask, enables receive abort on collision. */
#define RCR_STRIP_CRC   0x0200  /*!< \ref NIC_RCR bit mask, strips CRC. */
#define RCR_RXEN        0x0100  /*!< \ref NIC_RCR bit mask, enables receiver. */
#define RCR_ALMUL       0x0004  /*!< \ref NIC_RCR bit mask, multicast frames accepted when set. */
#define RCR_PRMS        0x0002  /*!< \ref NIC_RCR bit mask, enables promiscuous mode. */
#define RCR_RX_ABORT    0x0001  /*!< \ref NIC_RCR bit mask, set when receive was aborted. */

/*! 
 * \brief Bank 0 - Counter register.
 */
#define NIC_ECR         NIC_BASE + 0x06

/*! 
 * \brief Bank 0 - Memory information register.
 */
#define NIC_MIR         NIC_BASE + 0x08

/*! 
 * \brief Bank 0 - Receive / PHY control register.
 */
#define NIC_RPCR        NIC_BASE + 0x0A

#define RPCR_SPEED      0x2000  /*!< \ref NIC_RPCR bit mask, PHY operates at 100 Mbps. */
#define RPCR_DPLX       0x1000  /*!< \ref NIC_RPCR bit mask, PHY operates at full duplex mode. */
#define RPCR_ANEG       0x0800  /*!< \ref NIC_RPCR bit mask, sets PHY in auto-negotiation mode. */
#define RPCR_LEDA_PAT   0x0000  /*!< \ref NIC_RPCR bit mask for LEDA mode. */
#define RPCR_LEDB_PAT   0x0010  /*!< \ref NIC_RPCR bit mask for LEDB mode. */

/*! 
 * \brief Bank 1 - Configuration register.
 */
#define NIC_CR          NIC_BASE + 0x00

#define CR_EPH_EN       0x8000  /*!< \ref NIC_CR bit mask, . */

/*! 
 * \brief Bank 1 - Base address register.
 */
#define NIC_BAR         NIC_BASE + 0x02

/*! 
 * \brief Bank 1 - Individual address register.
 */
#define NIC_IAR         NIC_BASE + 0x04

/*! 
 * \brief Bank 1 - General purpose register.
 */
#define NIC_GPR         NIC_BASE + 0x0A

/*! 
 * \brief Bank 1 - Control register.
 */
#define NIC_CTR         NIC_BASE + 0x0C

#define CTR_RCV_BAD     0x4000  /*!< \ref NIC_CTR bit mask. */
#define CTR_AUTO_RELEASE 0x0800 /*!< \ref NIC_CTR bit mask, transmit packets automatically released. */

/*!
 * \brief Bank 2 - MMU command register.
 */
#define NIC_MMUCR       NIC_BASE + 0x00

#define MMUCR_BUSY      0x0001

#define MMU_NOP         0
#define MMU_ALO         (1<<5)
#define MMU_RST         (2<<5)
#define MMU_REM         (3<<5)
#define MMU_TOP         (4<<5)
#define MMU_PKT         (5<<5)
#define MMU_ENQ         (6<<5)
#define MMU_RTX         (7<<5)

/*!
 * \brief Bank 2 - Packet number register.
 *
 * This byte register specifies the accessible transmit packet number.
 */
#define NIC_PNR         NIC_BASE + 0x02

/*!
 * \brief Bank 2 - Allocation result register.
 *
 * This byte register is updated upon a \ref MMU_ALO command.
 */
#define NIC_ARR         NIC_BASE + 0x03

#define ARR_FAILED      0x80

/*!
 * \brief Bank 2 - FIFO ports register.
 */
#define NIC_FIFO        NIC_BASE + 0x04

/*!
 * \brief Bank 2 - Pointer register.
 */
#define NIC_PTR         NIC_BASE + 0x06

#define PTR_RCV         0x8000  /*! \ref NIC_PTR bit mask, specifies receive or transmit buffer. */
#define PTR_AUTO_INCR   0x4000  /*! \ref NIC_PTR bit mask, enables automatic pointer increment. */
#define PTR_READ        0x2000  /*! \ref NIC_PTR bit mask, indicates type of access. */
#define PTR_ETEN        0x1000  /*! \ref NIC_PTR bit mask, enables early transmit underrun detection. */
#define PTR_NOT_EMPTY   0x0800  /*! \ref NIC_PTR bit mask, set when write data fifo is not empty. */

/*!
 * \brief Bank 2 - Data register.
 */
#define NIC_DATA        NIC_BASE + 0x08

/*!
 * \brief Bank 2 - Interrupt status register.
 */
#define NIC_IST         NIC_BASE + 0x0C

/*!
 * \brief Bank 2 - Interrupt acknowledge register.
 */
#define NIC_ACK         NIC_BASE + 0x0C

/*!
 * \brief Bank 2 - Interrupt mask register.
 */
#define NIC_MSK         NIC_BASE + 0x0D

#define INT_MD          0x80    /*!< \ref PHY state change interrupt bit mask. */
#define INT_ERCV        0x40    /*!< \ref Early receive interrupt bit mask. */
#define INT_EPH         0x20    /*!< \ref Ethernet protocol interrupt bit mask. */
#define INT_RX_OVRN     0x10    /*!< \ref Receive overrun interrupt bit mask. */
#define INT_ALLOC       0x08    /*!< \ref Transmit allocation interrupt bit mask. */
#define INT_TX_EMPTY    0x04    /*!< \ref Transmitter empty interrupt bit mask. */
#define INT_TX          0x02    /*!< \ref Transmit complete interrupt bit mask. */
#define INT_RCV         0x01    /*!< \ref Receive interrupt bit mask. */

/*!
 * \brief Bank 3 - Multicast table register.
 */
#define NIC_MT          NIC_BASE + 0x00

/*!
 * \brief Bank 3 - Management interface register.
 */
#define NIC_MGMT        NIC_BASE + 0x08

#define MGMT_MDOE       0x08    /*!< \ref NIC_MGMT bit mask, enables MDO pin. */
#define MGMT_MCLK       0x04    /*!< \ref NIC_MGMT bit mask, drives MDCLK pin. */
#define MGMT_MDI        0x02    /*!< \ref NIC_MGMT bit mask, reflects MDI pin status. */
#define MGMT_MDO        0x01    /*!< \ref NIC_MGMT bit mask, drives MDO pin. */

/*!
 * \brief Bank 3 - Revision register.
 */
#define NIC_REV         NIC_BASE + 0x0A

/*!
 * \brief Bank 3 - Early RCV register.
 */
#define NIC_ERCV        NIC_BASE + 0x0C

/*!
 * \brief PHY control register.
 */
#define NIC_PHYCR       0

#define PHYCR_RST       0x8000  /*!< \ref NIC_PHYCR bit mask, resets PHY. */
#define PHYCR_LPBK      0x4000  /*!< \ref NIC_PHYCR bit mask, . */
#define PHYCR_SPEED     0x2000  /*!< \ref NIC_PHYCR bit mask, . */
#define PHYCR_ANEG_EN   0x1000  /*!< \ref NIC_PHYCR bit mask, . */
#define PHYCR_PDN       0x0800  /*!< \ref NIC_PHYCR bit mask, . */
#define PHYCR_MII_DIS   0x0400  /*!< \ref NIC_PHYCR bit mask, . */
#define PHYCR_ANEG_RST  0x0200  /*!< \ref NIC_PHYCR bit mask, . */
#define PHYCR_DPLX      0x0100  /*!< \ref NIC_PHYCR bit mask, . */
#define PHYCR_COLST     0x0080  /*!< \ref NIC_PHYCR bit mask, . */


/*!
 * \brief PHY status register.
 */
#define NIC_PHYSR       1

#define PHYSR_CAP_T4    0x8000  /*!< \ref NIC_PHYSR bit mask, indicates 100BASE-T4 capability. */
#define PHYSR_CAP_TXF   0x4000  /*!< \ref NIC_PHYSR bit mask, indicates 100BASE-TX full duplex capability. */
#define PHYSR_CAP_TXH   0x2000  /*!< \ref NIC_PHYSR bit mask, indicates 100BASE-TX half duplex capability. */
#define PHYSR_CAP_TF    0x1000  /*!< \ref NIC_PHYSR bit mask, indicates 10BASE-T full duplex capability. */
#define PHYSR_CAP_TH    0x0800  /*!< \ref NIC_PHYSR bit mask, indicates 10BASE-T half duplex capability. */
#define PHYSR_CAP_SUPR  0x0040  /*!< \ref NIC_PHYSR bit mask, indicates preamble suppression capability. */
#define PHYSR_ANEG_ACK  0x0020  /*!< \ref NIC_PHYSR bit mask, auto-negotiation completed. */
#define PHYSR_REM_FLT   0x0010  /*!< \ref NIC_PHYSR bit mask, remote fault detected. */
#define PHYSR_CAP_ANEG  0x0008  /*!< \ref NIC_PHYSR bit mask, indicates auto-negotiation capability. */
#define PHYSR_LINK      0x0004  /*!< \ref NIC_PHYSR bit mask, valid link status. */
#define PHYSR_JAB       0x0002  /*!< \ref NIC_PHYSR bit mask, jabber collision detected. */
#define PHYSR_EXREG     0x0001  /*!< \ref NIC_PHYSR bit mask, extended capabilities available. */


/*!
 * \brief PHY identifier register 1.
 */
#define NIC_PHYID1      2

/*!
 * \brief PHY identifier register 1.
 */
#define NIC_PHYID2      3

/*!
 * \brief PHY auto-negotiation advertisement register.
 */
#define NIC_PHYANAD     4

#define PHYANAD_NP      0x8000  /*!< \ref NIC_PHYANAD bit mask, exchanging next page information. */
#define PHYANAD_ACK     0x4000  /*!< \ref NIC_PHYANAD bit mask, acknowledged. */
#define PHYANAD_RF      0x2000  /*!< \ref NIC_PHYANAD bit mask, remote fault. */
#define PHYANAD_T4      0x0200  /*!< \ref NIC_PHYANAD bit mask, indicates 100BASE-T4 capability. */
#define PHYANAD_TX_FDX  0x0100  /*!< \ref NIC_PHYANAD bit mask, indicates 100BASE-TX full duplex capability. */
#define PHYANAD_TX_HDX  0x0080  /*!< \ref NIC_PHYANAD bit mask, indicates 100BASE-TX half duplex capability. */
#define PHYANAD_10FDX   0x0040  /*!< \ref NIC_PHYANAD bit mask, indicates 10BASE-T full duplex capability. */
#define PHYANAD_10_HDX  0x0020  /*!< \ref NIC_PHYANAD bit mask, indicates 10BASE-T half duplex capability. */
#define PHYANAD_CSMA    0x0001  /*!< \ref NIC_PHYANAD bit mask, indicates 802.3 CSMA capability. */

/*!
 * \brief PHY auto-negotiation remote end capability register.
 */
#define NIC_PHYANRC     5

/*!
 * \brief PHY configuration register 1.
 */
#define NIC_PHYCFR1     16

/*!
 * \brief PHY configuration register 2.
 */
#define NIC_PHYCFR2     17

/*!
 * \brief PHY status output register.
 */
#define NIC_PHYSOR      18

#define PHYSOR_INT      0x8000  /*!< \ref NIC_PHYSOR bit mask, interrupt bits changed. */
#define PHYSOR_LNKFAIL  0x4000  /*!< \ref NIC_PHYSOR bit mask, link failure detected. */
#define PHYSOR_LOSSSYNC 0x2000  /*!< \ref NIC_PHYSOR bit mask, descrambler sync lost detected. */
#define PHYSOR_CWRD     0x1000  /*!< \ref NIC_PHYSOR bit mask, code word error detected. */
#define PHYSOR_SSD      0x0800  /*!< \ref NIC_PHYSOR bit mask, start of stream error detected. */
#define PHYSOR_ESD      0x0400  /*!< \ref NIC_PHYSOR bit mask, end of stream error detected. */
#define PHYSOR_RPOL     0x0200  /*!< \ref NIC_PHYSOR bit mask, reverse polarity detected. */
#define PHYSOR_JAB      0x0100  /*!< \ref NIC_PHYSOR bit mask, jabber detected. */
#define PHYSOR_SPDDET   0x0080  /*!< \ref NIC_PHYSOR bit mask, 100/10 speed detected. */
#define PHYSOR_DPLXDET  0x0040  /*!< \ref NIC_PHYSOR bit mask, duplex detected. */

/*!
 * \brief PHY mask register.
 */
#define NIC_PHYMSK      19

#define PHYMSK_MINT     0x8000  /*!< \ref NIC_PHYMSK bit mask, enables \ref PHYSOR_INT interrupt. */
#define PHYMSK_MLNKFAIL 0x4000  /*!< \ref NIC_PHYMSK bit mask, enables \ref PHYSOR_LNKFAIL interrupt. */
#define PHYMSK_MLOSSSYN 0x2000  /*!< \ref NIC_PHYMSK bit mask, enables \ref PHYSOR_LOSSSYNC interrupt. */
#define PHYMSK_MCWRD    0x1000  /*!< \ref NIC_PHYMSK bit mask, enables \ref PHYSOR_CWRD interrupt. */
#define PHYMSK_MSSD     0x0800  /*!< \ref NIC_PHYMSK bit mask, enables \ref PHYSOR_SSD interrupt. */
#define PHYMSK_MESD     0x0400  /*!< \ref NIC_PHYMSK bit mask, enables \ref PHYSOR_ESD interrupt. */
#define PHYMSK_MRPOL    0x0200  /*!< \ref NIC_PHYMSK bit mask, enables \ref PHYSOR_RPOL interrupt. */
#define PHYMSK_MJAB     0x0100  /*!< \ref NIC_PHYMSK bit mask, enables \ref PHYSOR_JAB interrupt. */
#define PHYMSK_MSPDDT   0x0080  /*!< \ref NIC_PHYMSK bit mask, enables \ref PHYSOR_SPDDET interrupt. */
#define PHYMSK_MDPLDT   0x0040  /*!< \ref NIC_PHYMSK bit mask, enables \ref PHYSOR_DPLXDET interrupt. */



#define MSBV(bit)       (1 << ((bit) - 8))

#define nic_outlb(addr, val) (*(volatile u_char *)(addr) = (val))
#define nic_outhb(addr, val) (*(volatile u_char *)((addr) + 1) = (val))
#define nic_outwx(addr, val) (*(volatile u_short *)(addr) = (val))
#define nic_outw(addr, val) { \
    *(volatile u_char *)(addr) = (u_char)(val); \
    *((volatile u_char *)(addr) + 1) = (u_char)((val) >> 8); \
}

#define nic_inlb(addr) (*(volatile u_char *)(addr))
#define nic_inhb(addr) (*(volatile u_char *)((addr) + 1))
#define nic_inw(addr) (*(volatile u_short *)(addr))

#define nic_bs(bank)    nic_outlb(NIC_BSR, bank)

/*!
 * \struct _NICINFO lanc111.h dev/lanc111.h
 * \brief Network interface controller information structure.
 */
struct _NICINFO {
    HANDLE volatile ni_rx_rdy;  /*!< Receiver event queue. */
    HANDLE volatile ni_tx_rdy;  /*!< Transmitter event queue. */
    u_short ni_tx_cnt;          /*!< Number of bytes in transmission queue. */
    u_long ni_rx_packets;       /*!< Number of packets received. */
    u_long ni_tx_packets;       /*!< Number of packets sent. */
    u_long ni_interrupts;       /*!< Number of interrupts. */
    u_long ni_overruns;         /*!< Number of packet overruns. */
    u_long ni_rx_frame_errors;  /*!< Number of frame errors. */
    u_long ni_rx_crc_errors;    /*!< Number of CRC errors. */
    u_long ni_rx_missed_errors; /*!< Number of missed packets. */
};

/*!
 * \brief Network interface controller information type.
 */
typedef struct _NICINFO NICINFO;

/*@}*/

/*!
 * \addtogroup xgNicLanc111
 */
/*@{*/


static HANDLE mutex;
static HANDLE maq;

/*!
 * \brief Select specified PHY register for reading or writing.
 *
 * \note NIC interrupts must have been disabled before calling this routine.
 *
 * \param reg PHY register number.
 * \param we  Indicates type of access, 1 for write and 0 for read.
 *
 * \return Contents of the PHY interface rgister.
 */
static u_char NicPhyRegSelect(u_char reg, u_char we)
{
    u_char rs;
    u_char msk;
    u_char i;

    nic_bs(3);
    rs = (nic_inlb(NIC_MGMT) & ~(MGMT_MCLK | MGMT_MDO)) | MGMT_MDOE;

    /* Send idle pattern. */
    for (i = 0; i < 33; i++) {
        nic_outlb(NIC_MGMT, rs | MGMT_MDO);
        nic_outlb(NIC_MGMT, rs | MGMT_MDO | MGMT_MCLK);
    }

    /* Send start sequence. */
    nic_outlb(NIC_MGMT, rs);
    nic_outlb(NIC_MGMT, rs | MGMT_MCLK);
    nic_outlb(NIC_MGMT, rs | MGMT_MDO);
    nic_outlb(NIC_MGMT, rs | MGMT_MDO | MGMT_MCLK);

    /* Write or read mode. */
    if (we) {
        nic_outlb(NIC_MGMT, rs);
        nic_outlb(NIC_MGMT, rs | MGMT_MCLK);
        nic_outlb(NIC_MGMT, rs | MGMT_MDO);
        nic_outlb(NIC_MGMT, rs | MGMT_MDO | MGMT_MCLK);
    } else {
        nic_outlb(NIC_MGMT, rs | MGMT_MDO);
        nic_outlb(NIC_MGMT, rs | MGMT_MDO | MGMT_MCLK);
        nic_outlb(NIC_MGMT, rs);
        nic_outlb(NIC_MGMT, rs | MGMT_MCLK);
    }

    /* Send PHY address. Zero is used for the internal PHY. */
    for (i = 0; i < 5; i++) {
        nic_outlb(NIC_MGMT, rs);
        nic_outlb(NIC_MGMT, rs | MGMT_MCLK);
    }

    /* Send PHY register number. */
    for (msk = 0x10; msk; msk >>= 1) {
        if (reg & msk) {
            nic_outlb(NIC_MGMT, rs | MGMT_MDO);
            nic_outlb(NIC_MGMT, rs | MGMT_MDO | MGMT_MCLK);
        } else {
            nic_outlb(NIC_MGMT, rs);
            nic_outlb(NIC_MGMT, rs | MGMT_MCLK);
        }
    }
    nic_outlb(NIC_MGMT, rs);

    return rs;
}

/*!
 * \brief Read contents of PHY register.
 *
 * \note NIC interrupts must have been disabled before calling this routine.
 *
 * \param reg PHY register number.
 *
 * \return Contents of the specified register.
 */
static u_short NicPhyRead(u_char reg)
{
    u_short rc = 0;
    u_char rs;
    u_char i;

    /* Select register for reading. */
    rs = NicPhyRegSelect(reg, 0);

    /* Switch data direction. */
    rs &= ~MGMT_MDOE;
    nic_outlb(NIC_MGMT, rs);
    nic_outlb(NIC_MGMT, rs | MGMT_MCLK);

    /* Clock data in. */
    for (i = 0; i < 16; i++) {
        nic_outlb(NIC_MGMT, rs);
        nic_outlb(NIC_MGMT, rs | MGMT_MCLK);
        rc <<= 1;
        rc |= (nic_inlb(NIC_MGMT) & MGMT_MDI) != 0;
    }

    /* This will set the clock line to low. */
    nic_outlb(NIC_MGMT, rs);

    return rc;
}

/*!
 * \brief Write value to PHY register.
 *
 * \note NIC interrupts must have been disabled before calling this routine.
 *
 * \param reg PHY register number.
 * \param val Value to write.
 */
static void NicPhyWrite(u_char reg, u_short val)
{
    u_short msk;
    u_char rs;

    /* Select register for writing. */
    rs = NicPhyRegSelect(reg, 1);

    /* Switch data direction dummy. */
    nic_outlb(NIC_MGMT, rs | MGMT_MDO);
    nic_outlb(NIC_MGMT, rs | MGMT_MDO | MGMT_MCLK);
    nic_outlb(NIC_MGMT, rs);
    nic_outlb(NIC_MGMT, rs | MGMT_MCLK);

    /* Clock data out. */
    for (msk = 0x8000; msk; msk >>= 1) {
        if (val & msk) {
            nic_outlb(NIC_MGMT, rs | MGMT_MDO);
            nic_outlb(NIC_MGMT, rs | MGMT_MDO | MGMT_MCLK);
        } else {
            nic_outlb(NIC_MGMT, rs);
            nic_outlb(NIC_MGMT, rs | MGMT_MCLK);
        }
    }

    /* Set clock line low and output line int z-state. */
    nic_outlb(NIC_MGMT, rs & ~MGMT_MDOE);
}

/*!
 * \brief Configure the internal PHY.
 *
 * Reset the PHY and initiate auto-negotiation.
 */
static int NicPhyConfig(void)
{
    u_short phy_sor;
    u_short phy_sr;
    u_short phy_to;
    u_short mode;

    /* 
     * Reset the PHY and wait until this self clearing bit
     * becomes zero. We sleep 63 ms before each poll and
     * give up after 3 retries. 
     */
    //printf("Reset PHY..");
    NicPhyWrite(NIC_PHYCR, PHYCR_RST);
    for (phy_to = 0;; phy_to++) {
        NutSleep(63);
        if ((NicPhyRead(NIC_PHYCR) & PHYCR_RST) == 0)
            break;
        if (phy_to > 3)
            return -1;
    }
    //printf("OK\n");

    /* Store PHY status output. */
    phy_sor = NicPhyRead(NIC_PHYSOR);

    /* Enable PHY interrupts. */
    NicPhyWrite(NIC_PHYMSK, PHYMSK_MLOSSSYN | PHYMSK_MCWRD | PHYMSK_MSSD |
                PHYMSK_MESD | PHYMSK_MRPOL | PHYMSK_MJAB | PHYMSK_MSPDDT | PHYMSK_MDPLDT);

    /* Set RPC register. */
    mode = RPCR_ANEG | RPCR_LEDA_PAT | RPCR_LEDB_PAT;
    nic_bs(0);
    nic_outw(NIC_RPCR, mode);

    /*
     * Advertise our capabilities, initiate auto negotiation
     * and wait until this has been completed.
     */
    //printf("Negotiate..");
    NicPhyWrite(NIC_PHYANAD, PHYANAD_TX_FDX | PHYANAD_TX_HDX | PHYANAD_10FDX | PHYANAD_10_HDX | PHYANAD_CSMA);
    NutSleep(63);
    for (phy_to = 0, phy_sr = 0;; phy_to++) {
        /* Give up after 10 seconds. */
        if (phy_to >= 1024)
            return -1;
        /* Restart auto negotiation every 4 seconds or on failures. */
        if ((phy_to & 127) == 0 /* || (phy_sr & PHYSR_REM_FLT) != 0 */ ) {
            NicPhyWrite(NIC_PHYCR, PHYCR_ANEG_EN | PHYCR_ANEG_RST);
            //printf("Restart..");
            NutSleep(63);
        }
        /* Check if we are done. */
        phy_sr = NicPhyRead(NIC_PHYSR);
        //printf("[SR %04X]", phy_sr);
        if (phy_sr & PHYSR_ANEG_ACK)
            break;
        NutSleep(63);
    }
    //printf("OK\n");

    return 0;
}

/*!
 * \brief Wait until MMU is ready.
 *
 * Poll the MMU command register until \ref MMUCR_BUSY
 * is cleared.
 *
 * \param tmo Timeout in milliseconds.
 *
 * \return 0 on success or -1 on timeout.
 */
static INLINE int NicMmuWait(u_short tmo)
{
    while (tmo--) {
        if ((nic_inlb(NIC_MMUCR) & MMUCR_BUSY) == 0)
            break;
        NutDelay(1);
    }
    return tmo ? 0 : -1;
}

/*!
 * \brief Reset the Ethernet controller.
 *
 * \return 0 on success, -1 otherwise.
 */
static int NicReset(void)
{
    /* Disable all interrupts. */
    nic_outlb(NIC_MSK, 0);

    /* MAC and PHY software reset. */
    nic_bs(0);
    nic_outw(NIC_RCR, RCR_SOFT_RST);

    /* Enable Ethernet protocol handler. */
    nic_bs(1);
    nic_outw(NIC_CR, CR_EPH_EN);

    NutDelay(10);

    /* Disable transmit and receive. */
    nic_bs(0);
    nic_outw(NIC_RCR, 0);
    nic_outw(NIC_TCR, 0);

    /* Enable auto release. */
    nic_bs(1);
    nic_outw(NIC_CTR, CTR_AUTO_RELEASE);

    /* Reset MMU. */
    nic_bs(2);
    nic_outlb(NIC_MMUCR, MMU_RST);
    if (NicMmuWait(1000))
        return -1;

    return 0;
}

/*
 * Fires up the network interface. NIC interrupts
 * should have been disabled when calling this
 * function.
 *
 * \param mac Six byte unique MAC address.
 */
static int NicStart(CONST u_char * mac)
{
    u_char i;

    if (NicReset())
        return -1;

    /* Enable receiver. */
    nic_bs(3);
    nic_outlb(NIC_ERCV, 7);
    nic_bs(0);
    nic_outw(NIC_RCR, RCR_RXEN);

    /* Enable transmitter and padding. */
    nic_outw(NIC_TCR, TCR_PAD_EN | TCR_TXENA);

    /* Configure the PHY. */
    if (NicPhyConfig())
        return -1;

    /* Set MAC address. */
    //printf("Set MAC %02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    nic_bs(1);
    for (i = 0; i < 6; i++)
        nic_outlb(NIC_IAR + i, mac[i]);
    //printf("OK\n");

    /* Enable interrupts. */
    nic_bs(2);
    nic_outlb(NIC_MSK, INT_ERCV | INT_RCV | INT_RX_OVRN);

    return 0;
}

/*
 * NIC interrupt entry.
 */
static void NicInterrupt(void *arg)
{
    u_char isr;
    u_char imr;
    NICINFO *ni = (NICINFO *) ((NUTDEVICE *) arg)->dev_dcb;

    ni->ni_interrupts++;

    /* Read the interrupt mask and disable all interrupts. */
    nic_bs(2);
    imr = nic_inlb(NIC_MSK);
    nic_outlb(NIC_MSK, 0);

    /* Read the interrupt status and acknowledge all interrupts. */
    isr = nic_inlb(NIC_IST);
    //printf("\n!%02X-%02X ", isr, imr);
    isr &= imr;

    /*
     * If this is a transmit interrupt, then a packet has been sent. 
     * So we can clear the transmitter busy flag and wake up the 
     * transmitter thread.
     */
    if (isr & INT_TX_EMPTY) {
        nic_outlb(NIC_ACK, INT_TX_EMPTY);
        imr &= ~INT_TX_EMPTY;
        NutEventPostAsync(&ni->ni_tx_rdy);
    }
    /* Transmit error. */
    else if (isr & INT_TX) {
        /* re-enable transmit */
        nic_bs(0);
        nic_outw(NIC_TCR, nic_inlb(NIC_TCR) | TCR_TXENA);
        nic_bs(2);
        nic_outlb(NIC_ACK, INT_TX);
        /* kill the packet */
        nic_outlb(NIC_MMUCR, MMU_PKT);

        NutEventPostAsync(&ni->ni_tx_rdy);
    }


    /*
     * If this is a receive interrupt, then wake up the receiver 
     * thread.
     */
    if (isr & INT_RX_OVRN) {
        nic_outlb(NIC_ACK, INT_RX_OVRN);
        //nic_outlb(NIC_MMUCR, MMU_TOP);
    }
    if (isr & INT_ERCV) {
        nic_outlb(NIC_ACK, INT_ERCV);
        NutEventPostAsync(&ni->ni_rx_rdy);
    }
    if (isr & INT_RCV) {
        nic_outlb(NIC_ACK, INT_RCV);
        imr &= ~INT_RCV;
        NutEventPostAsync(&ni->ni_rx_rdy);
    }

    if (isr & INT_ALLOC) {
        imr &= ~INT_ALLOC;
        NutEventPostAsync(&maq);
    }
    //printf(" -%02X-%02X- ", nic_inlb(NIC_IST), inb(PINE) & 0x20);
    nic_outlb(NIC_MSK, imr);
}

/*
 * Write data block to the NIC.
 */
static void NicWrite(u_char * buf, u_short len)
{
    register u_short l = len - 1;
    register u_char ih = (u_short) l >> 8;
    register u_char il = (u_char) l;

    if (!len)
        return;

    do {
        do {
            nic_outlb(NIC_DATA, *buf++);
        } while (il-- != 0);
    } while (ih-- != 0);
}

/*
 * Read data block from the NIC.
 */
static void NicRead(u_char * buf, u_short len)
{
    register u_short l = len - 1;
    register u_char ih = (u_short) l >> 8;
    register u_char il = (u_char) l;

    if (!len)
        return;

    do {
        do {
            *buf++ = nic_inlb(NIC_DATA);
        } while (il-- != 0);
    } while (ih-- != 0);
}

/*!
 * \brief Fetch the next packet out of the receive ring buffer.
 *
 * Nic interrupts must be disabled when calling this funtion.
 *
 * \return Pointer to an allocated ::NETBUF. If there is no
 *         no data available, then the function returns a
 *         null pointer. If the NIC's buffer seems to be
 *         corrupted, a pointer to 0xFFFF is returned.
 */
static NETBUF *NicGetPacket(void)
{
    NETBUF *nb = 0;
    //u_char *buf;
    u_short fsw;
    u_short fbc;

    /* Check the fifo empty bit. If it is set, then there is 
       nothing in the receiver fifo. */
    nic_bs(2);
    if (nic_inw(NIC_FIFO) & 0x8000) {
        return 0;
    }

    /* Inialize pointer register. */
    nic_outw(NIC_PTR, PTR_READ | PTR_RCV | PTR_AUTO_INCR);
    _NOP();
    _NOP();
    _NOP();
    _NOP();

    /* Read status word and byte count. */
    fsw = nic_inw(NIC_DATA);
    fbc = nic_inw(NIC_DATA);
    //printf("[SW=%04X,BC=%04X]", fsw, fbc);

    /* Check for frame errors. */
    if (fsw & 0xAC00) {
        nb = (NETBUF *) 0xFFFF;
    }
    /* Check the byte count. */
    else if (fbc < 66 || fbc > 1524) {
        nb = (NETBUF *) 0xFFFF;
    }

    else {
        /* 
         * Allocate a NETBUF. 
         * Hack alert: Rev A chips never set the odd frame indicator.
         */
        fbc -= 3;
        nb = NutNetBufAlloc(0, NBAF_DATALINK, fbc);

        /* Perform the read. */
        if (nb)
            NicRead(nb->nb_dl.vp, fbc);
    }

    /* Release the packet. */
    nic_outlb(NIC_MMUCR, MMU_TOP);

    return nb;
}

/*!
 * \brief Load a packet into the nic's transmit ring buffer.
 *
 * Interupts must have been disabled when calling this function.
 *
 * \param nb Network buffer structure containing the packet to be sent.
 *           The structure must have been allocated by a previous
 *           call NutNetBufAlloc(). This routine will automatically
 *           release the buffer in case of an error.
 *
 * \return 0 on success, -1 in case of any errors. Errors
 *         will automatically release the network buffer 
 *         structure.
 */
static int NicPutPacket(NETBUF * nb)
{
    u_short sz;
    u_char odd = 0;
    u_char imsk;

    //printf("[P]");
    /*
     * Calculate the number of bytes to be send. Do not send packets 
     * larger than the Ethernet maximum transfer unit. The MTU
     * consist of 1500 data bytes plus the 14 byte Ethernet header
     * plus 4 bytes CRC. We check the data bytes only.
     */
    if ((sz = nb->nb_nw.sz + nb->nb_tp.sz + nb->nb_ap.sz) > ETHERMTU)
        return -1;

    /* Disable all interrupts. */
    imsk = nic_inlb(NIC_MSK);
    nic_outlb(NIC_MSK, 0);

    /* Allocate packet buffer space. */
    nic_bs(2);
    nic_outlb(NIC_MMUCR, MMU_ALO);
    if (NicMmuWait(100))
        return -1;

    /* Enable interrupts including allocation success. */
    nic_outlb(NIC_MSK, imsk | INT_ALLOC);

    /* The MMU needs some time. Use it to calculate the byte count. */
    sz += nb->nb_dl.sz;
    sz += 6;
    if (sz & 1) {
        sz++;
        odd++;
    }

    /* Wait for allocation success. */
    while ((nic_inlb(NIC_IST) & INT_ALLOC) == 0) {
        if (NutEventWait(&maq, 125)) {
            nic_outlb(NIC_MMUCR, MMU_RST);
            NicMmuWait(1000);
            nic_outlb(NIC_MMUCR, MMU_ALO);
            if (NicMmuWait(100) || (nic_inlb(NIC_IST) & INT_ALLOC) == 0) {
                if (NutEventWait(&maq, 125)) {
                    return -1;
                }
            }
        }
    }

    /* Disable interrupts. */
    imsk = nic_inlb(NIC_MSK);
    nic_outlb(NIC_MSK, 0);


    nic_outlb(NIC_PNR, nic_inhb(NIC_PNR));

    nic_outw(NIC_PTR, 0x4000);

    /* Transfer control word. */
    nic_outlb(NIC_DATA, 0);
    nic_outlb(NIC_DATA, 0);

    /* Transfer the byte count. */
    nic_outw(NIC_DATA, sz);

    /* Transfer the Ethernet frame. */
    NicWrite(nb->nb_dl.vp, nb->nb_dl.sz);
    NicWrite(nb->nb_nw.vp, nb->nb_nw.sz);
    NicWrite(nb->nb_tp.vp, nb->nb_tp.sz);
    NicWrite(nb->nb_ap.vp, nb->nb_ap.sz);

    if (odd)
        nic_outlb(NIC_DATA, 0);

    /* Transfer the control word. */
    nic_outw(NIC_DATA, 0);

    /* Enqueue packet. */
    if (NicMmuWait(100))
        return -1;
    nic_outlb(NIC_MMUCR, MMU_ENQ);

    /* Enable interrupts. */
    imsk |= INT_TX | INT_TX_EMPTY;
    nic_outlb(NIC_MSK, imsk);

    return 0;
}


/*! \fn NicRxLanc(void *arg)
 * \brief NIC receiver thread.
 *
 */
THREAD(NicRxLanc, arg)
{
    NUTDEVICE *dev;
    IFNET *ifn;
    NICINFO *ni;
    NETBUF *nb;
    u_char imsk;

    dev = arg;
    ifn = (IFNET *) dev->dev_icb;
    ni = (NICINFO *) dev->dev_dcb;

    /*
     * This is a temporary hack. Due to a change in initialization,
     * we may not have got a MAC address yet. Wait until one has been
     * set.
     */
    while ((ifn->if_mac[0] & ifn->if_mac[1] & ifn->if_mac[2]) == 0xFF) {
        //printf("[WM]");
        NutSleep(63);
    }
    NicStart(ifn->if_mac);

    sbi(EICR, 2);
    sbi(EICR, 3);
    sbi(EIMSK, LAN_SIGNAL_BIT);

    NutEventPost(&mutex);

    /* Run at high priority. */
    NutThreadSetPriority(9);

    for (;;) {

        /*
         * Wait for the arrival of new packets or
         * check the receiver every two second.
         */
        NutEventWait(&ni->ni_rx_rdy, 2000);
        //printf("[P]");

        /*
         * Fetch all packets from the NIC's internal
         * buffer and pass them to the registered handler.
         */
        imsk = nic_inlb(NIC_MSK);
        nic_outlb(NIC_MSK, 0);
        while ((nb = NicGetPacket()) != 0) {
            if (nb != (NETBUF *) 0xFFFF) {
                ni->ni_rx_packets++;
                (*ifn->if_recv) (dev, nb);
            }
        }
        nic_outlb(NIC_MSK, imsk | INT_RCV | INT_ERCV);
    }
}

/*!
 * \brief Send Ethernet packet.
 *
 * \param dev   Identifies the device to use.
 * \param nb    Network buffer structure containing the packet to be sent.
 *              The structure must have been allocated by a previous
 *              call NutNetBufAlloc().
 *
 * \return 0 on success, -1 in case of any errors.
 */
int LancOutput(NUTDEVICE * dev, NETBUF * nb)
{
    int rc = -1;
    NICINFO *ni;

    if (NutEventWait(&mutex, 15000)) {
        //printf("[MTX]");
    }

    ni = (NICINFO *) dev->dev_dcb;

    if (NicPutPacket(nb) == 0) {
        ni->ni_tx_packets++;
        rc = 0;
    }
    NutEventPost(&mutex);

    return rc;
}

/*!
 * \brief Initialize Ethernet hardware.
 *
 * Resets the LAN91C111 Ethernet controller, initializes all required 
 * hardware registers and starts a background thread for incoming 
 * Ethernet traffic.
 *
 * Applications should do not directly call this function. It is 
 * automatically executed during during device registration by 
 * NutRegisterDevice().
 *
 * If the network configuration hasn't been set by the application
 * before registering the specified device, this function will
 * call NutNetLoadConfig() to get the MAC address.
 *
 * \param dev Identifies the device to initialize.
 */
int LancInit(NUTDEVICE * dev)
{
    /* Disable NIC interrupt and clear NICINFO structure. */
    cbi(EIMSK, LAN_SIGNAL_BIT);
    memset(dev->dev_dcb, 0, sizeof(NICINFO));

    /* Register interrupt handler and enable interrupts. */
    if (NutRegisterIrqHandler(&LAN_SIGNAL, NicInterrupt, dev))
        return -1;

    /*
     * Start the receiver thread.
     */
    NutThreadCreate("rxi5", NicRxLanc, dev, 640);

    //NutSleep(500);

    return 0;
}

/*@}*/

/*!
 * \addtogroup xgSmscDev
 */
/*@{*/

static NICINFO dcb_eth0;

/*!
 * \brief Network interface information structure.
 *
 * Used to call.
 */
static IFNET ifn_eth0 = {
    IFT_ETHER,                  /*!< \brief Interface type. */
    {0, 0, 0, 0, 0, 0},         /*!< \brief Hardware net address. */
    0,                          /*!< \brief IP address. */
    0,                          /*!< \brief Remote IP address for point to point. */
    0,                          /*!< \brief IP network mask. */
    ETHERMTU,                   /*!< \brief Maximum size of a transmission unit. */
    0,                          /*!< \brief Packet identifier. */
    0,                          /*!< \brief Linked list of arp entries. */
    NutEtherInput,              /*!< \brief Routine to pass received data to, if_recv(). */
    LancOutput,                 /*!< \brief Driver output routine, if_send(). */
    NutEtherOutput              /*!< \brief Media output routine, if_output(). */
};

/*!
 * \brief Device information structure.
 *
 * A pointer to this structure must be passed to NutRegisterDevice() 
 * to bind this Ethernet device driver to the Nut/OS kernel.
 * An application may then call NutNetIfConfig() with the name \em eth0 
 * of this driver to initialize the network interface.
 * 
 */
NUTDEVICE devSmsc111 = {
    0,                          /* Pointer to next device. */
    {'e', 't', 'h', '0', 0, 0, 0, 0, 0},        /* Unique device name. */
    IFTYP_NET,                  /* Type of device. */
    0,                          /* Base address. */
    0,                          /* First interrupt number. */
    &ifn_eth0,                  /* Interface control block. */
    &dcb_eth0,                  /* Driver control block. */
    LancInit,                   /* Driver initialization routine. */
    0,                          /* Driver specific control function. */
    0,                          /* Read from device. */
    0,                          /* Write to device. */
    0,                          /* Write from program space data to device. */
    0,                          /* Open a device or file. */
    0,                          /* Close a device or file. */
    0                           /* Request file size. */
};

/*@}*/
