#ifndef _DEV_AX88796_H_
#define _DEV_AX88796_H_

#include <netinet/if_ether.h>
#include <net/ether.h>
#include <net/if_var.h>


/*!
 * brief Network interface controller information structure and type.
 */
typedef struct {
    HANDLE volatile ni_rx_rdy;      /*!< Receiver event queue. */
    HANDLE volatile ni_tx_rdy;      /*!< Transmitter event queue. */
    u_short ni_tx_cnt;              /*!< Number of bytes in transmission queue. */
    u_long ni_rx_packets;           /*!< Number of packets received. */
    u_long ni_tx_packets;           /*!< Number of packets sent. */
    u_long ni_interrupts;           /*!< Number of interrupts. */
    u_long ni_overruns;             /*!< Number of packet overruns. */
    u_long ni_rx_frame_errors;      /*!< Number of frame errors. */
    u_long ni_rx_crc_errors;        /*!< Number of CRC errors. */
    u_long ni_rx_missed_errors;     /*!< Number of missed packets. */
    u_char volatile ni_tx_bsy;      /*!< NIC transmitter busy flags. */
    u_long ni_rx_pending;           /*!< Number of pending receiver interrupts. */
    u_char ni_curr_page;            /*!< Current receive page. */
}NICINFO;

/*
 * Available drivers.
 */
extern NUTDEVICE devAx88796;

#ifndef DEV_ETHER
#define DEV_ETHER   devAx88796
#endif

#ifndef devEth0
#define devEth0   devAx88796
#endif


/*! 
 * \brief Charon III Ethernet controller parameters.
 */
#define ASIX_BASE        0x20000000

/* ! Read and write byte from controller */

#define Asix_Read(reg) *(unsigned char *) (reg + ASIX_BASE)
#define Asix_ReadWord(reg) *(unsigned short *) (reg + ASIX_BASE)
#define Asix_Write(reg, data) *(unsigned char *) (reg + ASIX_BASE) = data
#define Asix_WriteWord(reg, data) *(unsigned short *) (reg + ASIX_BASE) = data

// buffer boundaries - transmit has 6 256-byte pages = 1536
//   receive has 52 256-byte pages = 14848
//   entire available packet buffer space is allocated
#define TXSTART_INIT   	0x40
#define RXSTART_INIT   	0x46
#define RXSTOP_INIT    	0x80

extern int AsixInit(NUTDEVICE *dev);
extern int AsixOutput(NUTDEVICE *dev, NETBUF *nb);
void NicPhyWrite(u_char reg, u_short val);
u_short NicPhyRead(u_char reg);

#endif

