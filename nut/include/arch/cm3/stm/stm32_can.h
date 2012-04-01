#ifndef _STM32_CAN_H_
#define _STM32_CAN_H_
/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * Copyright (C) 2010 by Rittal GmbH & Co. KG. All rights reserved.
 *
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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

/*
 * \verbatim
 * $Id$
 * \endverbatim
 */

/*!
 * CAN Interrupts
 */

#define CAN_IT_RQCP0    ((uint32_t)0x00000005) /*!< Request completed mailbox 0 */
#define CAN_IT_RQCP1    ((uint32_t)0x00000006) /*!< Request completed mailbox 1 */
#define CAN_IT_RQCP2    ((uint32_t)0x00000007) /*!< Request completed mailbox 2 */
#define CAN_IT_TME      ((uint32_t)0x00000001) /*!< Transmit mailbox empty */
#define CAN_IT_FMP0     ((uint32_t)0x00000002) /*!< FIFO 0 message pending */
#define CAN_IT_FF0      ((uint32_t)0x00000004) /*!< FIFO 0 full */
#define CAN_IT_FOV0     ((uint32_t)0x00000008) /*!< FIFO 0 overrun */
#define CAN_IT_FMP1     ((uint32_t)0x00000010) /*!< FIFO 1 message pending */
#define CAN_IT_FF1      ((uint32_t)0x00000020) /*!< FIFO 1 full */
#define CAN_IT_FOV1     ((uint32_t)0x00000040) /*!< FIFO 1 overrun */
#define CAN_IT_EWG      ((uint32_t)0x00000100) /*!< Error warning */
#define CAN_IT_EPV      ((uint32_t)0x00000200) /*!< Error passive */
#define CAN_IT_BOF      ((uint32_t)0x00000400) /*!< Bus-off */
#define CAN_IT_LEC      ((uint32_t)0x00000800) /*!< Last error code */
#define CAN_IT_ERR      ((uint32_t)0x00008000) /*!< Error */
#define CAN_IT_WKU      ((uint32_t)0x00010000) /*!< Wake-up */
#define CAN_IT_SLK      ((uint32_t)0x00020000) /*!< Sleep */

#define CAN_IT_MSK      ((uint32_t)0x00038F77) /*!< Interrupt Mask Value */

/* Enable / disable time triggered communication mode */
#define CAN_TTCM    0x0001

/* Enable / disable automatic bus-off management */
#define CAN_ABOM    0x0002

/* Set the automatic wake-up mode */
#define CAN_AWUM    0x0004

/* Enable / disable no automatic retransmission */
#define CAN_NART    0x0008

/* Enable / disable receive FIFO locked mode */
#define CAN_RFLM    0x0010

/* Enable / disable transmit FIFO priority */
#define CAN_TXFP    0x0020

#if 0
extern NUTCANBUS Stm32CanBus_1;
extern NUTCANBUS Stm32CanBus_2;

#ifdef CANBUS2_AS_DEFAULT
#define DEFCANBUS_ Stm32CanBus_2
#else
#define DEFCANBUS_ Stm32CanBus_1
#endif
#endif

typedef struct _CANBUS CANBUS;

struct _CANBUS {
    uptr_t      bus_base;       /*< Periphery Base Register Address */
    __IO uint32_t* bb_base;        /*< Periphery BIT BAND Base Register Address */
    IRQ_HANDLER *sig_rx0_irq;   /*< IRQ Handler RX0 Interrupt */
    IRQ_HANDLER *sig_rx1_irq;   /*< IRQ Handler RX1 Interrupt */
    IRQ_HANDLER *sig_tx_irq;    /*< IRQ Handler TX Interrupt */
    IRQ_HANDLER *sig_sce_irq;   /*< IRQ Handler SCE Interrupt */
    uint32_t    bus_irqmsk;     /*< Backup Register for Interrupt Flags */

    int (*bus_inithw)(void);    /*< Function for low level hardware initialization */
};

extern CANBUS Stm32CanBus1;
extern CANBUS Stm32CanBus1;

extern int Stm32CanBus1Init( void);
extern int Stm32CanBus2Init( void);

extern int CanSetFeatures( CANBUS *bus, uint32_t flags, uint8_t ena);
extern int CanGetFeatures( CANBUS *bus, uint32_t flags);
extern int Stm32CanBusInit( CANBUS *bus, uint8_t baud);

#endif /* _STM32_CAN_H_ */
