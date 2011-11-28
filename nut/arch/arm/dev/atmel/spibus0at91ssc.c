/*
 * Copyright (C) 2009, 2011 by egnite GmbH
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

/*!
 * \file arch/arm/dev/atmel/spibus0at91ssc.c
 * \brief SSC SPI bus 0 driver.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <cfg/spi.h>
#include <cfg/arch/gpio.h>

#include <dev/gpio.h>
#include <dev/spibus_ssc.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/nutdebug.h>

#include <errno.h>
#include <stdlib.h>

#ifndef SBBI0_CS0_BIT
#define SBBI0_CS0_BIT   15
#endif

static GSPIREG gspi_reg0;

/*!
 * \brief Set the specified chip select to a given level.
 */
static GSPIREG *SscSpi0ChipSelect(uint_fast8_t cs, uint_fast8_t hi)
{
    if (hi) {
        outr(PIOA_SODR, _BV(SBBI0_CS0_BIT));
    } else {
        outr(PIOA_CODR, _BV(SBBI0_CS0_BIT));
    }
    return &gspi_reg0;
}

/*!
 * \brief Transfer data on the SPI bus.
 *
 * A device must have been selected by calling SscSpi0Select().
 *
 * \param node  Specifies the SPI bus node.
 * \param txbuf Pointer to the transmit buffer. If NULL, undetermined
 *              byte values are transmitted.
 * \param rxbuf Pointer to the receive buffer. If NULL, then incoming
 *              data is discarded.
 * \param xlen  Number of bytes to transfer.
 *
 * \return Always 0.
 */
static int SscSpiBus0Transfer(NUTSPINODE * node, CONST void *txbuf, void *rxbuf, int xlen)
{
    CONST uint8_t *txp = txbuf;
    uint8_t *rxp = rxbuf;

    if (txp) {
        if (rxp) {
            while (xlen--) {
                outr(SSC_THR, *txp);
                txp++;
                while ((inr(SSC_SR) & SSC_RXRDY) == 0);
                *rxp++ = inr(SSC_RHR);
            }
        } else {
            while (xlen--) {
                outr(SSC_THR, *txp);
                txp++;
                while ((inr(SSC_SR) & SSC_RXRDY) == 0);
                inr(SSC_RHR);
            }
        }
    }
    else if (rxp) {
        while (xlen--) {
            outr(SSC_THR, 0xFF);
            while ((inr(SSC_SR) & SSC_RXRDY) == 0);
            *rxp++ = inr(SSC_RHR);
        }
    }
    else {
        while (xlen--) {
            outr(SSC_THR, 0xFF);
            while ((inr(SSC_SR) & SSC_RXRDY) == 0);
            inr(SSC_RHR);
        }
    }
    return 0;
}

/*!
 * \brief Initialize an SPI bus node.
 *
 * This routine is called for each SPI node, which is registered via
 * NutRegisterSpiDevice().
 *
 * \param node Specifies the SPI bus node.
 *
 * \return Always 0.
 */
static int SscSpiBus0NodeInit(NUTSPINODE * node)
{
    outr(PIOA_PER, _BV(SBBI0_CS0_BIT));
    outr(PIOA_OER, _BV(SBBI0_CS0_BIT));
    SscSpi0ChipSelect(node->node_cs, 1);
    /* Perform software reset. */
    outr(SSC_CR, SSC_SWRST | SSC_TXDIS | SSC_RXDIS);
    /* Enable SSC clock. */
    outr(PMC_PCER, _BV(SSC_ID));
    /* Select SSC peripheral functions. */
    outr(PIOA_ASR, _BV(PA16_TK_A) | _BV(PA17_TD_A) | _BV(PA18_RD_A) | _BV(PA19_RK_A));
    /* Enable SSC peripheral pins. */
    outr(PIOA_PDR, _BV(PA16_TK_A) | _BV(PA17_TD_A) | _BV(PA18_RD_A) | _BV(PA19_RK_A));
    /* Set clock divider, clk=mck/(n*2), 0: off, 1: 24MHz, 24: 1MHz, 60: 400kHz  */
    outr(SSC_CMR, 1);
    /* Set receive clock mode. */
    outr(SSC_RCMR, SSC_CKS_PIN | SSC_CKI | SSC_START_TX);
    /* Set transmit clock mode. */
    outr(SSC_TCMR, SSC_CKS_DIV | SSC_CKO_TRAN);
    /* Set receive frame mode. */
    outr(SSC_RFMR, ((8 - 1) << SSC_DATLEN_LSB) | SSC_MSBF);
    /* Set transmit frame mode. */
    outr(SSC_TFMR, ((8 - 1) << SSC_DATLEN_LSB) | SSC_MSBF);
    /* Enable transmitter. */
    outr(SSC_CR, SSC_TXEN | SSC_RXEN);

    SscSpiSetup(node);

    return 0;
}

/*! \brief Select a device on the SPI bus.
 *
 * Locks and activates the bus for the specified node.
 *
 * \param node Specifies the SPI bus node.
 * \param tmo Timeout in milliseconds. To disable timeout, set this
 *            parameter to NUT_WAIT_INFINITE.
 *
 * \return 0 on success. In case of an error, -1 is returned and the bus
 *         is not locked.
 */
static int SscSpiBus0Select(NUTSPINODE * node, uint32_t tmo)
{
    int rc;

    /* Sanity check. */
    NUTASSERT(node != NULL);
    NUTASSERT(node->node_stat != NULL);

    /* Allocate the bus. */
    rc = NutEventWait(&node->node_bus->bus_mutex, tmo);
    if (rc) {
        errno = EIO;
    } else {
        /* Do the update, if the mode update bit is set. */
        if (node->node_mode & SPI_MODE_UPDATE) {
            SscSpiSetup(node);
        }
        /* Activate the node's chip select. */
        SscSpi0ChipSelect(node->node_cs, 0);
    }
    return rc;
}

/*! \brief Deselect a device on the SPI bus.
 *
 * Deactivates the chip select and unlocks the bus.
 *
 * \param node Specifies the SPI bus node.
 *
 * \return Always 0.
 */
static int SscSpiBus0Deselect(NUTSPINODE * node)
{
    /* Sanity check. */
    NUTASSERT(node != NULL);
    NUTASSERT(node->node_bus != NULL);

    /* Deactivate the node's chip select. */
    SscSpi0ChipSelect(node->node_cs, 1);

    /* Release the bus. */
    NutEventPost(&node->node_bus->bus_mutex);

    return 0;
}

/*!
 * \brief AT91 SSC SPI bus driver implementation structure.
 */
NUTSPIBUS spiBus0Ssc = {
    NULL,               /*!< Bus mutex semaphore (bus_mutex). */
    NULL,               /*!< Bus ready signal (bus_ready). */
    0,                  /*!< Unused bus base address (bus_base). */
    NULL,               /*!< Bus interrupt handler (bus_sig). */
    SscSpiBus0NodeInit, /*!< Initialize the bus (bus_initnode). */
    SscSpiBus0Select,   /*!< Select the specified device (bus_alloc). */
    SscSpiBus0Deselect, /*!< Deselect the specified device (bus_release). */
    SscSpiBus0Transfer, /*!< Transfer data to and from a specified device (bus_transfer). */
    NutSpiBusWait,      /*!< Wait for bus transfer ready (bus_wait). */
    NutSpiBusSetMode,   /*!< Set SPI mode of a specified device (bus_set_mode). */
    NutSpiBusSetRate,   /*!< Set clock rate of a specified device (bus_set_rate). */
    NutSpiBusSetBits    /*!< Set number of data bits of a specified device (bus_set_bits). */
};
