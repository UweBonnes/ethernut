/*
 * Copyright (C) 2012 by Simon Budig (simon@budig.de)
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
 *
 */

/*
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <sys/timer.h>
#include <sys/nutdebug.h>
#include <dev/spibus.h>
#include <dev/gpio.h>

#include <arch/cm3.h>
#include <arch/cm3/nxp/mach/lpc_sc.h>
#include <arch/cm3/nxp/mach/lpc_ssp.h>
#include <arch/cm3/nxp/lpc176x_clk.h>

#include <errno.h>
#include <stdlib.h>

#define SSP0BUS_SCK_PORT  NUTGPIO_PORT0
#define SSP0BUS_MISO_PORT NUTGPIO_PORT0
#define SSP0BUS_MOSI_PORT NUTGPIO_PORT0

#define SSP0BUS_SCK_PIN  15
#define SSP0BUS_SSEL_PIN 16
#define SSP0BUS_MISO_PIN 17
#define SSP0BUS_MOSI_PIN 18

#define SSP0BUS_SCK_PIN_CFG  (GPIO_CFG_OUTPUT | GPIO_CFG_PERIPHERAL2)
#define SSP0BUS_MISO_PIN_CFG (GPIO_CFG_PERIPHERAL2)
#define SSP0BUS_MOSI_PIN_CFG (GPIO_CFG_OUTPUT | GPIO_CFG_PERIPHERAL2)

#define SSP1BUS_SCK_PORT  NUTGPIO_PORT0
#define SSP1BUS_MISO_PORT NUTGPIO_PORT0
#define SSP1BUS_MOSI_PORT NUTGPIO_PORT0

#define SSP1BUS_SCK_PIN   7
#define SSP1BUS_SSEL_PIN  6
#define SSP1BUS_MISO_PIN  8
#define SSP1BUS_MOSI_PIN  9

#define SSP1BUS_SCK_PIN_CFG  (GPIO_CFG_OUTPUT | GPIO_CFG_PERIPHERAL2)
#define SSP1BUS_MISO_PIN_CFG (GPIO_CFG_PERIPHERAL2)
#define SSP1BUS_MOSI_PIN_CFG (GPIO_CFG_OUTPUT | GPIO_CFG_PERIPHERAL2)

static int  Lpc17xxSspSetup       (NUTSPINODE * node);
static int  Lpc17xxSspBusNodeInit (NUTSPINODE * node);
static int  Lpc17xxSspBusSelect   (NUTSPINODE * node, uint32_t tmo);
static int  Lpc17xxSspBusDeselect (NUTSPINODE * node);
static int  Lpc17xxSspBusTransfer (NUTSPINODE * node, const void *txbuf, void *rxbuf, int xlen);

// TODO: Add a mutex for the bus access

#if defined(LPC_SSP0_BASE)
NUTSPIBUS spiBus0Lpc17xxSsp = {
    NULL,                       /*!< Bus mutex semaphore (bus_mutex). */
    NULL,                       /*!< Bus ready signal (bus_ready). */
    LPC_SSP0_BASE,              /*!< Bus base address (bus_base). */
    NULL,                       /*!< Bus interrupt handler (bus_sig). */
    Lpc17xxSspBusNodeInit,      /*!< Initialize the bus (bus_initnode). */
    Lpc17xxSspBusSelect,        /*!< Select the specified device (bus_alloc). */
    Lpc17xxSspBusDeselect,      /*!< Deselect the specified device (bus_release). */
    Lpc17xxSspBusTransfer,
    NutSpiBusWait,
    NutSpiBusSetMode,           /*!< Set SPI mode of a specified device (bus_set_mode). */
    NutSpiBusSetRate,           /*!< Set clock rate of a specified device (bus_set_rate). */
    NutSpiBusSetBits            /*!< Set number of data bits of a specified device (bus_set_bits). */
};
#endif


#if defined(LPC_SSP1_BASE)
NUTSPIBUS spiBus1Lpc17xxSsp = {
    NULL,                       /*!< Bus mutex semaphore (bus_mutex). */
    NULL,                       /*!< Bus ready signal (bus_ready). */
    LPC_SSP1_BASE,              /*!< Bus base address (bus_base). */
    NULL,                       /*!< Bus interrupt handler (bus_sig). */
    Lpc17xxSspBusNodeInit,      /*!< Initialize the bus (bus_initnode). */
    Lpc17xxSspBusSelect,        /*!< Select the specified device (bus_alloc). */
    Lpc17xxSspBusDeselect,      /*!< Deselect the specified device (bus_release). */
    Lpc17xxSspBusTransfer,
    NutSpiBusWait,
    NutSpiBusSetMode,           /*!< Set SPI mode of a specified device (bus_set_mode). */
    NutSpiBusSetRate,           /*!< Set clock rate of a specified device (bus_set_rate). */
    NutSpiBusSetBits            /*!< Set number of data bits of a specified device (bus_set_bits). */
};
#endif


#if defined(LPC_SSP2_BASE)
NUTSPIBUS spiBus2Lpc17xxSsp = {
    NULL,                       /*!< Bus mutex semaphore (bus_mutex). */
    NULL,                       /*!< Bus ready signal (bus_ready). */
    LPC_SSP2_BASE,              /*!< Bus base address (bus_base). */
    NULL,                       /*!< Bus interrupt handler (bus_sig). */
    Lpc17xxSspBusNodeInit,      /*!< Initialize the bus (bus_initnode). */
    Lpc17xxSspBusSelect,        /*!< Select the specified device (bus_alloc). */
    Lpc17xxSspBusDeselect,      /*!< Deselect the specified device (bus_release). */
    Lpc17xxSspBusTransfer,
    NutSpiBusWait,
    NutSpiBusSetMode,           /*!< Set SPI mode of a specified device (bus_set_mode). */
    NutSpiBusSetRate,           /*!< Set clock rate of a specified device (bus_set_rate). */
    NutSpiBusSetBits            /*!< Set number of data bits of a specified device (bus_set_bits). */
};
#endif


/*!
 * \brief Set the specified chip select to a given level.
 */
static int Lpc17xxSspChipSelect(uint_fast8_t cs, uint_fast8_t hi)
{
    GpioPinConfigSet(cs / 32, cs % 32, GPIO_CFG_OUTPUT);
    GpioPinSet(cs / 32, cs % 32, hi);
    return 0;
}

/*! \brief Deselect a device on the SSP bus.
 *
 * Deactivates the chip select and unlocks the bus.
 *
 * \param node Specifies the SSP bus node.
 *
 * \return Always 0.
 */
static int Lpc17xxSspBusDeselect(NUTSPINODE * node)
{
    /* Sanity check. */
    NUTASSERT(node != NULL);
    NUTASSERT(node->node_bus != NULL);

    NutSpiBusWait(node, NUT_WAIT_INFINITE);
    /* Deactivate the node's chip select. */
    Lpc17xxSspChipSelect(node->node_cs, (node->node_mode & SPI_MODE_CSHIGH) == 0);

    /* Release the bus. */
    NutEventPostAsync(&node->node_bus->bus_mutex);

    return 0;
}

/*! \brief Select a device on the SSP bus.
 *
 * Locks and activates the bus for the specified node.
 *
 * \param node Specifies the SSP bus node.
 * \param tmo  Timeout in milliseconds. To disable timeout, set this
 *             parameter to NUT_WAIT_INFINITE.
 *
 * \return 0 on success. In case of an error, -1 is returned and the bus
 *         is not locked.
 */
static int Lpc17xxSspBusSelect(NUTSPINODE * node, uint32_t tmo)
{
    int rc;
    LPC_SSP_TypeDef* base;

    /* Sanity check. */
    NUTASSERT(node != NULL);
    NUTASSERT(node->node_bus != NULL);
    NUTASSERT(node->node_stat != NULL);

    base = (LPC_SSP_TypeDef*) (node->node_bus->bus_base);

    /* Allocate the bus. */
    rc = NutEventWait(&node->node_bus->bus_mutex, tmo);
    if (rc) {
        errno = EIO;
    } else {
        LPC_SSP_TypeDef *sspreg = node->node_stat;

        /* Dectivate the IO Pins to avoid glitches*/
        if (node->node_bus->bus_base == LPC_SSP0_BASE) {
            SysCtlPeripheralClkEnable (CLKPWR_PCONP_PCSSP0);
            GpioPinConfigSet (SSP0BUS_SCK_PORT,  SSP0BUS_SCK_PIN,  GPIO_CFG_DISABLED);  // SCK
            GpioPinConfigSet (SSP0BUS_MISO_PORT, SSP0BUS_MISO_PIN, GPIO_CFG_DISABLED);  // MISO
            GpioPinConfigSet (SSP0BUS_MOSI_PORT, SSP0BUS_MOSI_PIN, GPIO_CFG_DISABLED);  // MOSI
        } else if (node->node_bus->bus_base == LPC_SSP1_BASE) {
            SysCtlPeripheralClkEnable (CLKPWR_PCONP_PCSSP1);
            GpioPinConfigSet (SSP1BUS_SCK_PORT,  SSP1BUS_SCK_PIN,  GPIO_CFG_DISABLED);  // SCK
            GpioPinConfigSet (SSP1BUS_MISO_PORT, SSP1BUS_MISO_PIN, GPIO_CFG_DISABLED);  // MISO
            GpioPinConfigSet (SSP1BUS_MOSI_PORT, SSP1BUS_MOSI_PIN, GPIO_CFG_DISABLED);  // MOSI
        }

        /* If the mode update bit is set, then update our shadow registers. */
        if (node->node_mode & SPI_MODE_UPDATE) {
            Lpc17xxSspSetup(node);
        }

        /* Set SPI mode. */
        base->CR0 = sspreg->CR0;
        base->CR1 = sspreg->CR1;
        base->CPSR = sspreg->CPSR;

        if (node->node_bus->bus_base == LPC_SSP0_BASE) {
            GpioPinConfigSet(SSP0BUS_SCK_PORT,  SSP0BUS_SCK_PIN,  SSP0BUS_SCK_PIN_CFG);   // SCK
            GpioPinConfigSet(SSP0BUS_MISO_PORT, SSP0BUS_MISO_PIN, SSP0BUS_MISO_PIN_CFG);  // MISO
            GpioPinConfigSet(SSP0BUS_MOSI_PORT, SSP0BUS_MOSI_PIN, SSP0BUS_MOSI_PIN_CFG);  // MOSI
        } else if (node->node_bus->bus_base == LPC_SSP1_BASE) {
            GpioPinConfigSet(SSP1BUS_SCK_PORT,  SSP1BUS_SCK_PIN,  SSP1BUS_SCK_PIN_CFG);   // SCK
            GpioPinConfigSet(SSP1BUS_MISO_PORT, SSP1BUS_MISO_PIN, SSP1BUS_MISO_PIN_CFG);  // MISO
            GpioPinConfigSet(SSP1BUS_MOSI_PORT, SSP1BUS_MOSI_PIN, SSP1BUS_MOSI_PIN_CFG);  // MOSI
        }

        /* Finally activate the node's chip select. */
        rc = Lpc17xxSspChipSelect(node->node_cs, (node->node_mode & SPI_MODE_CSHIGH) != 0);
        if (rc) {
            /* Release the bus in case of an error. */
            NutEventPost(&node->node_bus->bus_mutex);
        }
    }
    return rc;
}


/*!
 * \brief Update SSP shadow registers.
 *
 * \param node Specifies the SPI bus node.
 *
 * \return Always 0.
 */
static int Lpc17xxSspSetup(NUTSPINODE * node)
{
    uint32_t clk;
    uint32_t clkdiv;
    LPC_SSP_TypeDef *sspreg;

    NUTASSERT(node != NULL);
    NUTASSERT(node->node_stat != NULL);
    NUTASSERT(node->node_bus != NULL);
    NUTASSERT(node->node_bus->bus_base != 0);
    sspreg = node->node_stat;

    sspreg->CR0 &= ~(SSP_CR0_DSS_MSK | SSP_CR0_FRF_MSK | SSP_CR0_CPOL | SSP_CR0_CPHA | SSP_CR0_SCR_MSK);
    sspreg->CR0 |= SSP_CR0_DSS (node->node_bits);
    if (node->node_mode & SPI_MODE_CPOL) {
        sspreg->CR0 |= SSP_CR0_CPOL;
    }
    if (node->node_mode & SPI_MODE_CPHA) {
        sspreg->CR0 |= SSP_CR0_CPHA;
    }
    sspreg->CR0 |= SSP_CR0_FRF_SPI;

    /* master only for now */
    sspreg->CR1 &= ~(SSP_CR1_LBM | SSP_CR1_SSE | SSP_CR1_MS | SSP_CR1_SOD);

    clk = NutClockGet(NUT_HWCLK_PCLK);

    if (node->node_bus->bus_base == LPC_SSP0_BASE) {
        clk /= Lpc176x_PclkDivGet(CLKPWR_PCLKSEL_SSP0);
    } else if (node->node_bus->bus_base == LPC_SSP1_BASE) {
        clk /= Lpc176x_PclkDivGet(CLKPWR_PCLKSEL_SSP1);
    }

    /* Calculate the SSP clock divider. Avoid rounding errors. */
    clkdiv = (clk + node->node_rate - 1) / node->node_rate;

    /* The divider value minimum is 2. */
    if (clkdiv < 2) {
        clkdiv = 2;
    }
    /* The divider value maximum is 255. */
    else if (clkdiv > 255) {
        clkdiv = 255;
    }

    /* must be even */
    clkdiv &= 0xfe;

    sspreg->CPSR = clkdiv;

    /* Update interface parameters. */
    node->node_rate = clk / clkdiv;
    node->node_mode &= ~SPI_MODE_UPDATE;

    return 0;
}


/*!
 * \brief Initialize an SSP bus node.
 *
 * This routine is called for each SSP node, which is registered via
 * NutRegisterSpiDevice().
 *
 * \param node Specifies the SSP bus node.
 *
 * \return 0 on success or -1 if there is no valid chip select.
 */
static int Lpc17xxSspBusNodeInit(NUTSPINODE * node)
{
    int rc = -1;

    /* Sanity check. */
    NUTASSERT(node != NULL);
    NUTASSERT(node->node_bus != NULL);

    /* Try to deactivate the node's chip select. */
    rc = Lpc17xxSspChipSelect(node->node_cs, (node->node_mode & SPI_MODE_CSHIGH) == 0);
    /* It should not hurt us being called more than once. Thus, we
       ** check wether any initialization had been taken place already. */
    if (rc == 0 && node->node_stat == NULL)
    {
        /* Allocate and set our shadow registers. */
        LPC_SSP_TypeDef *sspreg = malloc(sizeof(LPC_SSP_TypeDef));

        if (sspreg) {
            /* Update with node's defaults. */
            node->node_stat = (void *)sspreg;
            Lpc17xxSspSetup(node);
        }
        else {
            /* Out of memory? */
            rc = -1;
        }
    }

    return rc;
}

/*!
 * \brief Transfer data on the SSP bus using single buffered interrupt mode.
 *
 * A device must have been selected by calling At91SpiSelect().
 *
 * \param node  Specifies the SSP bus node.
 * \param txbuf Pointer to the transmit buffer. If NULL, undetermined
 *              byte values are transmitted.
 * \param rxbuf Pointer to the receive buffer. If NULL, then incoming
 *              data is discarded.
 * \param xlen  Number of bytes to transfer.
 *
 * \return Always 0.
 */
static int Lpc17xxSspBusTransfer(NUTSPINODE * node, const void *txbuf, void *rxbuf, int xlen)
{
    LPC_SSP_TypeDef* base;

    /* Sanity check. */
    NUTASSERT(node != NULL);
    NUTASSERT(node->node_bus != NULL);
    NUTASSERT(node->node_bus->bus_base != 0);
    base = (LPC_SSP_TypeDef*) node->node_bus->bus_base;

    unsigned char *tx = (unsigned char*) txbuf;
    unsigned char *rx = (unsigned char*) rxbuf;

    base->CR1 |= SSP_CR1_SSE;

    while (xlen-- > 0) {
        unsigned char b = tx ? (*tx++) : 0xff;

        base->DR = b;

        /* wait until receive buffer no longer empty */
        while ((base->SR & SSP_SR_RNE) == 0)
          ;

        b = base->DR;

        if (rx) {
          *rx++ = b;
        }
    }

    base->CR1 &= ~SSP_CR1_SSE;

    return 0;
}

