#ifndef _CAN_DEV_H_
#define _CAN_DEV_H_

/*
 * Copyright (C) 2004 by Ole Reinhardt<ole.reinhardt@kernelconcepts.de>
 * Kernel concepts (http://www.kernelconcepts.de) All rights reserved.
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

/*!
 * \file dev/can_dev.h
 * \brief Headers for can driver interface
 */

/*
 * $Log$
 * Revision 1.1  2004/06/07 15:13:48  olereinhardt
 * Initial checkin
 *
 */

#include <sys/types.h>
#include <sys/device.h>

/*!
 * \addtogroup xgCanDev
 */
/*@{*/

#define CAN_IF_2A    0x01
#define CAN_IF_2B    0x02

/*!
 * \struct _CANFRAME can_dev.h
 * \brief CAN frame structure
 */
 
struct _CANFRAME {              // todo: Implement flags
    u_long id;                  // Identifier
    u_char len;                 // Length of frame, max = 8
    u_char byte[8];
    u_char ext;                 // Boolean, extendet frame      
    u_char rtr;                 // Boolean, remote transmition bit
}; 

/*!
 * \brief CAN frame type
 */

typedef struct _CANFRAME CANFRAME;

/*!
 * \struct _CANINFO can_dev.h
 * \brief CAN controller information structure.
 */
struct _CANINFO {
    HANDLE volatile can_rx_rdy;     /*!< Receiver event queue. */
    HANDLE volatile can_tx_rdy;     /*!< Transmitter event queue. */
    u_long can_rx_frames;           /*!< Number of packets received. */
    u_long can_tx_frames;           /*!< Number of packets sent. */
    u_long can_interrupts;          /*!< Number of interrupts. */
    u_long can_overruns;            /*!< Number of packet overruns. */
    u_long can_errors;              /*!< Number of frame errors. */
};

/*!
 * \brief CAN controller information type.
 */
typedef struct _CANINFO CANINFO;

/*!
 * \struct ifcan can_dev.h can/can_dev.h
 * \brief CAN interface structure.
 *
 * Contains information about the CAN interface.
 */
struct ifcan {
    u_char can_type;            /*!< \brief Interface type. Either CAN_IF_2A or CAN_IF_2B. */
    u_long can_baudrate;        /*!< \brief Baudrate of device */
    u_char can_acc_mask[4];     /*!< \brief Acceptance mask */
    u_char can_acc_code[4];     /*!< \brief Acceptance code */
    
    u_char (*can_rxavail) (NUTDEVICE *);          /*!< \brief Receive buffer data available? */
    u_char (*can_txfree) (NUTDEVICE *);           /*!< \brief Transmit buffer free? */
    void   (*can_recv) (NUTDEVICE *, CANFRAME *); /*!< \brief Receive routine. */
    void   (*can_send) (NUTDEVICE *, CANFRAME *); /*!< \brief Send routine. */
    void   (*can_set_ac) (NUTDEVICE *, u_char* code); /*!< \brief Set accaptance code */
    void   (*can_set_am) (NUTDEVICE *, u_char* mode); /*!< \brief Set accaptance mode */
    u_char (*can_set_baud) (NUTDEVICE *, u_long baudrate); /*!< \brief Set accaptance code */
};

/*!
 * \brief Canbus interface type.
 */
typedef struct ifcan IFCAN;

void   CAN_TxFrame(NUTDEVICE *dev, CANFRAME *frame);
u_char CAN_TryTxFrame(NUTDEVICE *dev, CANFRAME *frame);

void   CAN_RxFrame(NUTDEVICE *dev, CANFRAME *frame);
u_char CAN_TryRXFrame(NUTDEVICE *dev, CANFRAME *frame);

/*@}*/

#endif
