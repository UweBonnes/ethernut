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
 * $Id: stm32_usart.c 3139 2010-09-27 13:33:34Z astralix $
 * \endverbatim
 */

#include <cfg/clock.h>

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <dev/irqreg.h>
#include <arch/cm3/stm/stm32f10x_rcc.h>
#include <arch/cm3/stm/stm32f10x_usart.h>
#include <arch/cm3/stm/stm32_can.h>

#if defined(CAN_USE_DMA)
#include <arch/cm3/stm/stm32f1_dma.h>
#endif

/* Time out for INAK bit */
#define INAK_TimeOut        ((uint32_t)0x0000FFFF)

/* Time out for SLAK bit */
#define SLAK_TimeOut        ((uint32_t)0x0000FFFF)

int CanWaitCheckAck( CANBUS *bus)
{
    int rc = 0;
    uint32_t wait_ack = 0;
    __IO uint32_t *CANBBx = bus->bb_base;

    /* Wait the acknowledge */
    while ((CANBBx[CM3BB_OFFSET(CAN_TypeDef, MSR, _BI32(CAN_MSR_INAK))] == 0) && (wait_ack < INAK_TimeOut))
    {
        wait_ack++;
    }

    /* ...and check acknowledged */
    if (CANBBx[CM3BB_OFFSET(CAN_TypeDef, MSR, _BI32(CAN_MSR_INAK))] == 0) {
        rc = -1;
    }

    return rc;
}

/*!
 * \brief CAN Bus Interrupt Control.
 *
 * This function sets the interrupt bits for the
 * CAN controller at the given bus.
 */
void CanIrqSetup( CANBUS *bus, uint32_t flags)
{
    CAN_TypeDef *CANx = (CAN_TypeDef*)bus->bus_base;
    bus->bus_irqmsk = flags;
    CANx->IER = flags;
}

/*!
 * \brief CAN Bus Interrupt Restore.
 *
 * This function restores all interrupts for the
 * CAN controller at the given bus.
 */
void CanIrqRestore( CANBUS  *bus)
{
    CAN_TypeDef *CANx = (CAN_TypeDef*)bus->bus_base;
    CANx->IER = bus->bus_irqmsk;
}

/*!
 * \brief CAN Bus Interrupt Disable.
 *
 * This function disables all interrupts for the
 * CAN controller at the given bus.
 */
void CanIrqDisable( CANBUS *bus)
{
    CAN_TypeDef *CANx = (CAN_TypeDef*)bus->bus_base;
    __IO uint32_t *CANBBx = bus->bb_base;
    bus->bus_irqmsk = CANx->IER;
    CANBBx[CM3BB_OFFSET(CAN_TypeDef, IER, _BI32(CAN_IT_MSK))] = 0;
}


int CanSetFeatures( CANBUS *bus, uint32_t flags, uint8_t ena)
{
    int rc = 0;
    CAN_TypeDef *CANx = (CAN_TypeDef*)bus->bus_base;
    uint32_t mcr = CANx->MCR;

    /* Set the time triggered communication mode */
    if (flags & CAN_TTCM) {
        if( ena)
            mcr |= CAN_MCR_TTCM;
        else
            mcr &= ~CAN_MCR_TTCM;
    }

    /* Set the automatic bus-off management */
    if (flags&CAN_ABOM) {
        if( ena)
            mcr |= CAN_MCR_ABOM;
        else
            mcr &= ~CAN_MCR_ABOM;
    }

    /* Set the automatic wake-up mode */
    if (flags&CAN_AWUM) {
        if (ena)
            mcr |= CAN_MCR_AWUM;
        else
            mcr &= ~CAN_MCR_AWUM;
    }

    /* Set the no automatic retransmission */
    if (flags&CAN_NART) {
        if (ena)
            mcr |= CAN_MCR_NART;
        else
            mcr &= ~CAN_MCR_NART;
    }

    /* Set the receive FIFO locked mode */
    if (flags&CAN_RFLM) {
        if (ena)
            mcr |= CAN_MCR_RFLM;
        else
            mcr &= ~CAN_MCR_RFLM;
    }

    /* Set the transmit FIFO priority */
    if (flags&CAN_TXFP) {
        if (ena)
            mcr |= CAN_MCR_TXFP;
        else
            mcr &= ~CAN_MCR_TXFP;
    }
    CANx->MCR = mcr;

    return rc;
}

int CanGetFeatures( CANBUS *bus, uint32_t flags)
{
    int rc = -1;
    CAN_TypeDef *CANx = (CAN_TypeDef*)bus->bus_base;
    uint32_t mcr = CANx->MCR;

    if (flags&CAN_TTCM) {
        /* Get time triggered communication mode */
        rc = (mcr&CAN_MCR_TTCM)?1:0;
    }
    else if (flags&CAN_ABOM) {
        /* Get automatic bus-off management */
        rc = (mcr&CAN_MCR_ABOM)?1:0;
    }
    else if (flags&CAN_AWUM) {
        /* Get automatic wake-up mode */
        rc = (mcr&CAN_MCR_AWUM)?1:0;
    }
    else if (flags&CAN_NART) {
        /* Get the no automatic retransmission config */
        rc = (mcr&CAN_MCR_NART)?1:0;
    }
    else if (flags&CAN_RFLM) {
        /* Get receive FIFO locked mode */
        rc = (mcr&CAN_MCR_RFLM)?1:0;
    }
    else if (flags&CAN_TXFP) {
        /* Set the transmit FIFO priority */
        rc = (mcr&CAN_MCR_TXFP)?1:0;
    }

    return rc;
}

int Stm32CanBusInit( CANBUS *bus, uint8_t baud)
{
    int rc = 0;
#if 0
    CAN_TypeDef *CANx = (CAN_TypeDef*)bus->bus_base;
#endif
    __IO uint32_t *CANBBx = bus->bb_base;

    /* If bus has hardware init function, call it. */
    if( bus->bus_inithw) {
        rc = (bus->bus_inithw)();
    }

    /* If baud == 0 caller only needs hardware setup */
    if( baud)
    {
        /* exit from sleep mode */
        CANBBx[CM3BB_OFFSET(CAN_TypeDef, MCR, _BI32(CAN_MCR_SLEEP))] = 0;

        /* Request initialisation */
        CANBBx[CM3BB_OFFSET(CAN_TypeDef, MCR, _BI32(CAN_MCR_INRQ))] = 1;

        /* Wait for Acknowledge */
        rc = CanWaitCheckAck( bus);
        if( rc)
            return rc;

#if 0
        CanSetFeatures( bus, uint32_t flags, ENABLE);

        /* Set the bit timing register */
        CANx->BTR = (uint32_t)((uint32_t)CAN_InitStruct->CAN_Mode << 30) | ((uint32_t)CAN_InitStruct->CAN_SJW << 24) |
           ((uint32_t)CAN_InitStruct->CAN_BS1 << 16) | ((uint32_t)CAN_InitStruct->CAN_BS2 << 20) |
           ((uint32_t)CAN_InitStruct->CAN_Prescaler - 1);
#endif

        /* Request leave initialisation */
        CANBBx[CM3BB_OFFSET(CAN_TypeDef, MCR, _BI32(CAN_MCR_INRQ))] = 0;

        rc = CanWaitCheckAck( bus);
    }

    /* At this step, return the status of initialization */
    return rc;
}
