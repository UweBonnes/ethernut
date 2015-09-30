/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * Copyright (C) 2010 by Nikolaj Zamotaev. All rights reserved.
 * Copyright (C) 2014-15 Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de).
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
 * \file arch/cm3/stm/stm32_spi.c
 * \brief STM32 SPI handling. Only include from devices specific file
 *        stm32_spiX.c.
 *
 * The SPI IP has no interrupt for SPI getting idle, so it takes some effort
 * to find the right moment to switch SPI and CS off. To do so, we always
 * count the received byte even if we transmit only.
 * DMA_Mode handles this case per se.
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <arch/cm3/stm/stm32xxxx.h>
#include <dev/spibus_stm32.h>
#include <sys/nutdebug.h>

#include <stdlib.h>
#include <errno.h>

/* Keep SPIBUS_CSx_INIT()  undefined when PORT/PIN is not available*/

#if defined(SPIBUS_CS0_PORT) && defined(SPIBUS_CS0_PIN)
#define SPIBUS_CS0_INIT(x)  GpioPinConfigSet(SPIBUS_CS0_PORT,  SPIBUS_CS0_PIN, GPIO_CFG_OUTPUT | x)
#define SPIBUS_CS0_SET()    GpioPinSetHigh(SPIBUS_CS0_PORT,  SPIBUS_CS0_PIN)
#define SPIBUS_CS0_CLR()    GpioPinSetLow (SPIBUS_CS0_PORT,  SPIBUS_CS0_PIN)
#else
#define SPIBUS_CS0_SET()
#define SPIBUS_CS0_CLR()
#endif

#if defined(SPIBUS_CS1_PORT) && defined(SPIBUS_CS1_PIN)
#define SPIBUS_CS1_INIT(x)  GpioPinConfigSet(SPIBUS_CS1_PORT,  SPIBUS_CS1_PIN, GPIO_CFG_OUTPUT | x)
#define SPIBUS_CS1_SET()    GpioPinSetHigh(SPIBUS_CS1_PORT,  SPIBUS_CS1_PIN)
#define SPIBUS_CS1_CLR()    GpioPinSetLow (SPIBUS_CS1_PORT,  SPIBUS_CS1_PIN)
#endif

#if defined(SPIBUS_CS2_PORT) && defined(SPIBUS_CS2_PIN)
#define SPIBUS_CS2_INIT(x)  GpioPinConfigSet(SPIBUS_CS2_PORT,  SPIBUS_CS2_PIN, GPIO_CFG_OUTPUT | x)
#define SPIBUS_CS2_SET()    GpioPinSetHigh(SPIBUS_CS2_PORT,  SPIBUS_CS2_PIN)
#define SPIBUS_CS2_CLR()    GpioPinSetLow (SPIBUS_CS2_PORT,  SPIBUS_CS2_PIN)
#endif

#if defined(SPIBUS_CS3_PORT) && defined(SPIBUS_CS3_PIN)
#define SPIBUS_CS3_INIT(x)  GpioPinConfigSet(SPIBUS_CS3_PORT,  SPIBUS_CS3_PIN, GPIO_CFG_OUTPUT | x)
#define SPIBUS_CS3_SET()    GpioPinSetHigh(SPIBUS_CS3_PORT,  SPIBUS_CS3_PIN)
#define SPIBUS_CS3_CLR()    GpioPinSetLow (SPIBUS_CS3_PORT,  SPIBUS_CS3_PIN)
#endif

#define SPIBUS_SCK_PORT  (uint32_t)(stm32_port_nr2gpio[SPI_SCK  >> 8])
#define SPIBUS_SCK_PIN   (SPI_SCK & 0xf)
#define SPIBUS_MISO_PORT (uint32_t)(stm32_port_nr2gpio[SPI_MISO >> 8])
#define SPIBUS_MISO_PIN  (SPI_MISO & 0xf)
#define SPIBUS_MOSI_PORT (uint32_t)(stm32_port_nr2gpio[SPI_MOSI >> 8])
#define SPIBUS_MOSI_PIN  (SPI_MOSI & 0xf)

#if defined (MCU_STM32F1)
/* For F1, all SPI pins are on the same port */
#define GPIO_CRL_CM3BB   CM3BBADDR(SPIBUS_MOSI_PORT, GPIO_TypeDef, CRL, 0)
#endif

#if SPIBUS_MODE != POLLING_MODE
static uint32_t clk_ratio;
#if SPIBUS_MODE == IRQ_MODE
static const uint8_t * spi_txp;
static uint8_t *       spi_rxp;
static volatile size_t spi_rx_len;
static volatile size_t spi_tx_len;
static volatile size_t spi_len;

static void Stm32SpiBusInterrupt(void *arg)
{
    SPI_TypeDef *spi = (SPI_TypeDef *)SPI_BASE;

    if(CM3BBGET(SPI_BASE, SPI_TypeDef, SR, _BI32(SPI_SR_RXNE))) {
        uint8_t b;
        b = spi->DR;
        spi_len --;
        if (spi_rx_len) {
            if (spi_rxp) {
                *spi_rxp = b;
                spi_rxp++;
                spi_rx_len--;
                if (spi_rx_len == 1) {
                    if (spi->CR1 & (SPI_CR1_BIDIMODE| SPI_CR1_RXONLY)) {
                        /* Follow "Disabling the SPI" */
                        while( GpioPinGet(SPIBUS_SCK_PORT,  SPIBUS_SCK_PIN));
                        while(!GpioPinGet(SPIBUS_SCK_PORT,  SPIBUS_SCK_PIN));
                        CM3BBCLR(SPI_BASE, SPI_TypeDef, CR1, _BI32(SPI_CR1_SPE));
                    }
                }
            }
        }
        /* Terminate when the requested number of bytes have been received
         * even so perhaps we didn't need to store them.
         * That way we can make sure we don't deassert CS while SCK is still running.
         */
        if (spi_len == 0){
            spi->CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE);
            NutEventPostFromIrq((void **)arg);
        }
    }
    if (CM3BBGET(SPI_BASE, SPI_TypeDef, SR, _BI32(SPI_SR_TXE))) {
        if (spi_tx_len) {
            *(uint8_t*)&spi->DR = *spi_txp;
            /* After sending the last byte we need to wait for the last
             * receive interrupt, but we are not interested in the transmitter
             * empty interrupt
             */
            if (spi_tx_len == 1) {
                CM3BBCLR(SPI_BASE, SPI_TypeDef, CR2, _BI32(SPI_CR2_TXEIE));
            }
            /* Half word access via "spi->DR = b" shifts out two frames
             * on the F373! */
            spi_txp++;
            spi_tx_len --;
        }
    }
}
#elif SPIBUS_MODE == DMA_MODE
#include <arch/cm3/stm/stm32_dma.h>
static void Stm32SpiBusDMAInterrupt(void *arg)
{
    DMA_ClearFlag(SPI_DMA_RX_CHANNEL, DMA_TCIF);
    NutEventPostFromIrq((void **)arg);
}
#endif
#endif

/*!
 * \brief Set the specified chip select to a given level.
 */
static int Stm32SpiChipSelect(NUTSPINODE *node, int assert)
{
    int res;
    int hi;
    if (node->node_mode & SPI_MODE_CSHIGH)
        hi = assert;
    else
        hi = !assert;
    switch (node->node_cs) {
    case 0:
        if(hi)
            SPIBUS_CS0_SET();
        else
            SPIBUS_CS0_CLR();
        res = 0;
        break;
#if defined(SPIBUS_CS1_SET)
    case 1:
        if(hi)
            SPIBUS_CS1_SET();
        else
            SPIBUS_CS1_CLR();
        res = 0;
        break;
#endif
#if defined(SPIBUS_CS2_SET)
    case 2:
        if(hi)
            SPIBUS_CS2_SET();
        else
            SPIBUS_CS2_CLR();
        res = 0;
        break;
#endif
#if defined(SPIBUS_CS3_SET)
    case 3:
        if(hi)
            SPIBUS_CS3_SET();
        else
            SPIBUS_CS3_CLR();
        res = 0;
        break;
#endif
     default:
         res = -1;
    }
    return res;
}
/*! \brief Deselect a device on the SPI bus.
 *
 * Deactivates the chip select and unlocks the bus.
 *
 * \param node Specifies the SPI bus node.
 *
 * \return Always 0.
 */
static int Stm32SpiBusDeselect(NUTSPINODE * node)
{
    /* Sanity check. */
    NUTASSERT(node != NULL);
    NUTASSERT(node->node_bus != NULL);

    NutSpiBusWait(node, NUT_WAIT_INFINITE);
    /* Deactivate the node's chip select. */
    Stm32SpiChipSelect(node, 0);

    /* Release the bus. */
    NutEventPostAsync(&node->node_bus->bus_mutex);

    return 0;
}

#define OSPEED_MULT 5
    /* Choose pin speed according to rate. Try to choose pin speed ~ OSPEED_MULT * rate */

/*! \brief Choose pin speed according to spi rate
 *
 * Try to choose pin speed ~ OSPEED_MULT * rate.
 * Watch out if with low supply voltag
 *
 * \param node Specifies the SPI bus node.
 * \param rate Effective SPI clock.
 *
 */

static void SetPinSpeed( NUTSPINODE * node, uint32_t rate)
{
    uint16_t rate_div64k = rate >> 16;
    uint8_t ospeed_set;
    rate_div64k = node->node_rate >> 16;

#if defined (MCU_STM32F1)
    if      (rate_div64k * OSPEED_MULT < ospeed_values[2])
        ospeed_set = 2;
    else if (rate_div64k * OSPEED_MULT < ospeed_values[1])
        ospeed_set = 1;
    else
        ospeed_set = 3;
    GPIO_CRL_CM3BB[SPIBUS_SCK_PIN  * 4 + 0] = (ospeed_set & 1);
    GPIO_CRL_CM3BB[SPIBUS_MOSI_PIN * 4 + 0] = (ospeed_set & 1);
    ospeed_set = ospeed_set >> 1;
    GPIO_CRL_CM3BB[SPIBUS_SCK_PIN  * 4 + 1] = (ospeed_set & 2);
    GPIO_CRL_CM3BB[SPIBUS_MOSI_PIN * 4 + 1] = (ospeed_set & 2);
#else
    uint32_t speed_reg;
    if      (rate_div64k * OSPEED_MULT < ospeed_values[0])
        ospeed_set = 0;
    else if (rate_div64k * OSPEED_MULT < ospeed_values[1])
        ospeed_set = 1;
    else if (rate_div64k * OSPEED_MULT < ospeed_values[2])
        ospeed_set = 2;
    else
        ospeed_set = 3;
    speed_reg = ((GPIO_TypeDef*)SPIBUS_MOSI_PORT)->OSPEEDR;
    speed_reg &= ~(0x3 << (SPIBUS_MOSI_PIN << 1));
    speed_reg |= (ospeed_set << (SPIBUS_MOSI_PIN << 1));
    ((GPIO_TypeDef*)SPIBUS_SCK_PORT)->OSPEEDR = speed_reg;
    speed_reg = ((GPIO_TypeDef*)SPIBUS_SCK_PORT)->OSPEEDR;
    speed_reg &= ~(0x3 << (SPIBUS_SCK_PIN << 1));
    speed_reg |= (ospeed_set << (SPIBUS_SCK_PIN << 1));
    ((GPIO_TypeDef*)SPIBUS_SCK_PORT)->OSPEEDR = speed_reg;
#endif
}

/*! \brief Pull SCK to the node-dependant idle level.
 *
 * \param node Specifies the SPI bus node.
 */
static void SetNodeSckIdleLevel(NUTSPINODE * node)
{
#if defined (MCU_STM32F1)
    if (node->node_mode & SPI_MODE_CPOL)
        GpioPinSetHigh(SPIBUS_SCK_PORT, SPIBUS_SCK_PIN);
    else
        GpioPinSetLow(SPIBUS_SCK_PORT, SPIBUS_SCK_PIN);
#else
    GPIO_TypeDef* sck_port;
    uint32_t pudr;

    sck_port = (GPIO_TypeDef*)SPIBUS_SCK_PORT;
    pudr = sck_port->PUPDR;
    pudr &= ~(3 <<(SPIBUS_SCK_PIN<<1));
     if (node->node_mode & SPI_MODE_CPOL)
         pudr |= (1 <<(SPIBUS_SCK_PIN << 1));
     else
         pudr |= (2 <<(SPIBUS_SCK_PIN << 1));
     sck_port->PUPDR = pudr;
#endif
}

/*! \brief Select a device on the SPI bus.
 *
 * Locks and activates the bus for the specified node.
 *
 * \param node Specifies the SPI bus node.
 * \param tmo  Timeout in milliseconds. To disable timeout, set this
 *             parameter to NUT_WAIT_INFINITE.
 *
 * \return 0 on success. In case of an error, -1 is returned and the bus
 *         is not locked.
 */
static int Stm32SpiBusSelect(NUTSPINODE * node, uint32_t tmo)
{
    int rc;
    SPI_TypeDef* base = (SPI_TypeDef *)SPI_BASE;

    /* Sanity check. */
    NUTASSERT(node != NULL);
    NUTASSERT(node->node_bus != NULL);
    NUTASSERT(node->node_stat != NULL);

    /* Allocate the bus. */
    rc = NutEventWait(&node->node_bus->bus_mutex, tmo);
    if (rc) {
        errno = EIO;
    } else {
        STM32SPIREG *spireg = node->node_stat;

        /* If the mode update bit is set, then update our shadow registers. */
        if (node->node_mode & SPI_MODE_UPDATE) {
            Stm32SpiSetup(node);
        }

        /* Idle level be set before SPI is enabled
         * See note e.g.  in RM0316 28.5.5 Communication formats
         */
        SetNodeSckIdleLevel(node);

        /* Set SPI mode. */
        base->CR1 = spireg->CR1;
        base->CR1 |= SPI_CR1_SSI|SPI_CR1_MSTR;
        base->CR2 = spireg->CR2;

        /* Finally activate the node's chip select. */
        rc = Stm32SpiChipSelect(node, 1);
        if (rc) {
            /* Release the bus in case of an error. */
            NutEventPost(&node->node_bus->bus_mutex);
        }
    }
    return rc;
}

/*!
 * \brief Update SPI shadow registers.
 *
 * \param node Specifies the SPI bus node.
 *
 * \return Always 0.
 */
static int Stm32SpiSetup(NUTSPINODE * node)
{
    uint32_t clk;
    uint32_t clkdiv;
    SPI_TypeDef *spireg;

    NUTASSERT(node != NULL);
    NUTASSERT(node->node_stat != NULL);
    NUTASSERT(node->node_bus != NULL);
    spireg = node->node_stat;

#if defined(SPI_CR2_DS)
    spireg->CR1 &= ~( SPI_CR1_LSBFIRST | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_BR);
    spireg->CR2 &= ~SPI_CR2_DS;
    if((node->node_bits > 6) && (node->node_bits <= 16))
        spireg->CR2 |= (node->node_bits -1) <<8;
#else
    spireg->CR1 &= ~(SPI_CR1_LSBFIRST | SPI_CR1_DFF | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_BR);
    switch(node->node_bits){
        case 8:
            spireg->CR1 &= ~(SPI_CR1_DFF);
            break;
        case 16:
            spireg->CR1 |= SPI_CR1_DFF;
            break;
        default:
            break;
    };
#endif
    if (node->node_mode & SPI_MODE_CPOL) {
        spireg->CR1 |= SPI_CR1_CPOL;
    }
    if (node->node_mode & SPI_MODE_CPHA) {
        spireg->CR1 |= SPI_CR1_CPHA;
    }
    if (node->node_mode & SPI_MODE_LSB) {
        spireg->CR1 |= SPI_CR1_LSBFIRST;
    }

    /* Query peripheral clock. */
#if defined (APB2PERIPH_BASE)
    if (SPI_BASE < APB2PERIPH_BASE)
        clk = NutClockGet(NUT_HWCLK_PCLK1);
    else
        clk = NutClockGet(NUT_HWCLK_PCLK2);
#else
    clk = NutClockGet(NUT_HWCLK_PCLK1);
#endif
    /* Calculate the SPI clock divider. Avoid rounding errors. */
    clkdiv = (clk + node->node_rate - 1) / node->node_rate;
    /* Choose the actual clock rate not greater than the requested rate */
    if (clkdiv > 128)
        clkdiv = 7;
    else if (clkdiv > 64)
        clkdiv = 6;
    else if (clkdiv > 32)
        clkdiv = 5;
    else if (clkdiv > 16)
        clkdiv = 4;
    else if (clkdiv >  8)
        clkdiv = 3;
    else if (clkdiv >  4)
        clkdiv = 2;
    else if (clkdiv >  2)
        clkdiv = 1;
    else
        clkdiv = 0;
    spireg->CR1 |= (clkdiv * SPI_CR1_BR_0);
#if SPIBUS_MODE != POLLING_MODE
    /* Calculate ratio between Processor and SPI clock */
    clk_ratio =  NutClockGet(NUT_HWCLK_CPU)/clk;
    clk_ratio = clk_ratio << clkdiv;
#endif
    /* Update interface parameters. */
    node->node_rate = clk / (1 << (clkdiv + 1)) ;
    SetPinSpeed(node, node->node_rate);
    node->node_mode &= ~SPI_MODE_UPDATE;

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
 * \return 0 on success or -1 if there is no valid chip select.
 */
static int Stm32SpiBusNodeInit(NUTSPINODE * node)
{
    int rc = 0;
    uint32_t init_flag;

    /* Sanity check. */
    NUTASSERT(node != NULL);
    NUTASSERT(node->node_bus != NULL);
    if ((node->node_mode & SPI_MODE_CSHIGH) == 0)
        init_flag = GPIO_CFG_INIT_HIGH;
    else
        init_flag = GPIO_CFG_INIT_LOW;
    (void) init_flag;
    switch (node->node_cs) {
    case 0:
        /* If CS0 is undefined, we assume permanent selection. */
#if defined(SPIBUS_CS0_INIT)
        SPIBUS_CS0_INIT(init_flag);
#endif
        break;
#if defined(SPIBUS_CS1_INIT)
    case 1:
        SPIBUS_CS1_INIT(init_flag);
        break;
#endif
#if defined(SPIBUS_CS2_INIT)
    case 2:
        SPIBUS_CS2_INIT(init_flag);
        break;
#endif
#if defined(SPIBUS_CS3_INIT)
    case 3:
        SPIBUS_CS3_INIT(init_flag);
        break;
#endif
    default:
        return -1;
    }
    /* Deactivate the node's chip select when initializing the port */
    rc = Stm32SpiChipSelect(node, 0);
    /* Test if  the SPI Bus is already initialized*/
    if (SPI_ENABLE_CLK_GET() == 0) {
        /* Initialize Hardware */
        Stm32GpioConfigSet
            (SPI_SCK,  GPIO_CFG_PERIPHAL | GPIO_CFG_OUTPUT | GPIO_CFG_INIT_LOW, SPI_SCK_AF );
        Stm32GpioConfigSet
            (SPI_MISO, GPIO_CFG_PERIPHAL                                      , SPI_MISO_AF);
        Stm32GpioConfigSet
            (SPI_MOSI, GPIO_CFG_PERIPHAL | GPIO_CFG_OUTPUT | GPIO_CFG_INIT_LOW, SPI_MOSI_AF);
        Stm32F1SpiRemap();
        SPI_ENABLE_CLK_SET();
    }
    /* It should not hurt us being called more than once. Thus, we
       ** check wether any initialization had been taken place already. */
    if (rc == 0 && node->node_stat == NULL)
    {
        /* Allocate and set our shadow registers. */
        STM32SPIREG *spireg = malloc(sizeof(STM32SPIREG));

        if (spireg) {
            /* Set interface defaults. */
            spireg->CR1 = SPI_CR1_SSM | SPI_CR1_MSTR;
            /* FIXME: Check values needed*/
#if defined(SPI_CR2_FRXTH)
            spireg->CR2 = SPI_CR2_FRXTH | 0x700;
#else
            spireg->CR2 =  0;
#endif
            /* Update with node's defaults. */
            node->node_stat = (void *)spireg;
            Stm32SpiSetup(node);
#if SPIBUS_MODE == IRQ_MODE
            NUTSPIBUS *bus;
            bus = node->node_bus;
            NutRegisterIrqHandler(&sig_SPI, Stm32SpiBusInterrupt, &bus->bus_ready);
            NutIrqEnable(&sig_SPI);
#elif SPIBUS_MODE == DMA_MODE
            DMA_Init();
#endif
        }
        else {
            /* Out of memory? */
            rc = -1;
        }
    }
    return rc;
}

/*!
 * \brief Transfer data on the SPI bus using single buffered interrupt mode.
 *
 * A device must have been selected by calling At91SpiSelect().
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
static int Stm32SpiBusTransfer
    (NUTSPINODE * node, const void *txbuf, void *rxbuf, int xlen)
{
    SPI_TypeDef* base = (SPI_TypeDef *)SPI_BASE;
    int tx_only;
    int rx_only;

    /* Sanity check. */
    if (xlen == 0)
        return 0;
    NUTASSERT(node != NULL);
    NUTASSERT(node->node_bus != NULL);

    tx_only = txbuf && !rxbuf;
    rx_only = (!txbuf || node->node_mode & SPI_MODE_HALFDUPLEX);
    /* Remove any remainders in DR */
    while (CM3BBGET(SPI_BASE, SPI_TypeDef, SR, _BI32(SPI_SR_RXNE)))
        (void) base->DR; /* Empty DR */

/* Dump guess for the numbers of cycles where a schedule make sense*/
#define SCHEDULE_CYCLES (1<<10)

#if SPIBUS_MODE == DMA_MODE
    /* Let's assume about 1000 CPU clocks is break even where a DMA
       transfer is faster than pure polling */
    if (xlen * clk_ratio > SCHEDULE_CYCLES/8) {
        DMA_SIGNAL *sig;
       if (rx_only) {
            if (node->node_mode & SPI_MODE_HALFDUPLEX)
                CM3BBSET(SPI_BASE, SPI_TypeDef, CR1, _BI32(SPI_CR1_BIDIMODE));
            else
                CM3BBSET(SPI_BASE, SPI_TypeDef, CR1, _BI32(SPI_CR1_RXONLY));
            DMA_Setup( SPI_DMA_RX_CHANNEL, rxbuf, (void*)(&base->DR), xlen , DMA_MINC);
            DMA_Enable( SPI_DMA_RX_CHANNEL);
        }
        else {
            DMA_Setup( SPI_DMA_TX_CHANNEL, (void*)(&base->DR), (void*) txbuf, xlen, DMA_MINC);
            DMA_Enable( SPI_DMA_TX_CHANNEL);
            if (tx_only) {
                uint32_t dummy;
                DMA_Setup( SPI_DMA_RX_CHANNEL, &dummy, (void*)(&base->DR), xlen, 0);
            }
            else
                DMA_Setup( SPI_DMA_RX_CHANNEL, rxbuf, (void*)(&base->DR), xlen, DMA_MINC);
            CM3BBSET(SPI_BASE, SPI_TypeDef, CR2, _BI32(SPI_CR2_TXDMAEN));
        }
        /* Register the DMA interrupt after we have acquired the DMA channel.
         * Otherwise we may disrupt an on-going transaction to an other device on the same
         * channel.
         */
       sig = DmaCreateHandler(SPI_DMA_RX_CHANNEL);
       DmaRegisterHandler(sig, SPI_DMA_RX_CHANNEL, Stm32SpiBusDMAInterrupt, &node->node_bus->bus_ready);
        DMA_Enable( SPI_DMA_RX_CHANNEL);
        DMA_IrqMask(SPI_DMA_RX_CHANNEL, DMA_TCIF, 1);
        CM3BBSET(SPI_BASE, SPI_TypeDef, CR2, _BI32(SPI_CR2_RXDMAEN));
        CM3BBSET(SPI_BASE, SPI_TypeDef, CR1, _BI32(SPI_CR1_SPE));
        NutEventWait(&node->node_bus->bus_ready, NUT_WAIT_INFINITE);
    }
#elif SPIBUS_MODE == IRQ_MODE
    /* Let's assume about 1000 CPU clocks is break even where a single Byte IRQ
       transfer is faster than pure polling */
    if (clk_ratio > SCHEDULE_CYCLES/8) {
        spi_txp = txbuf;
        spi_rxp = rxbuf;
        spi_len = xlen;
        spi_rx_len = xlen;
        if (rx_only) {
            spi_tx_len = 0;
            if (node->node_mode & SPI_MODE_HALFDUPLEX)
                CM3BBSET(SPI_BASE, SPI_TypeDef, CR1, _BI32(SPI_CR1_BIDIMODE));
            else
                CM3BBSET(SPI_BASE, SPI_TypeDef, CR1, _BI32(SPI_CR1_RXONLY));
            CM3BBSET(SPI_BASE, SPI_TypeDef, CR2, _BI32(SPI_CR2_RXNEIE));
            CM3BBSET(SPI_BASE, SPI_TypeDef, CR1, _BI32(SPI_CR1_SPE));
        }
        else {
            spi_tx_len = xlen;
            if (tx_only)
                spi_rx_len = 0;
            CM3BBSET(SPI_BASE, SPI_TypeDef, CR2, _BI32(SPI_CR2_RXNEIE));
            CM3BBSET(SPI_BASE, SPI_TypeDef, CR1, _BI32(SPI_CR1_SPE));
            CM3BBSET(SPI_BASE, SPI_TypeDef, CR2, _BI32(SPI_CR2_TXEIE));
        }
        NutEventWait(&node->node_bus->bus_ready, NUT_WAIT_INFINITE);
    }
#else
    if (0) {}
#endif
    else {
        if (tx_only) {
            base->CR1 |= SPI_CR1_SPE;
            while( xlen > 0) {
                /* Half word access via "spi->DR = b" shifts out two frames
                 * on the F373! */
                *(uint8_t *)&base->DR = *(const uint8_t *)txbuf;
                xlen--;
                txbuf++;
                while ( (base->SR & SPI_SR_TXE ) == 0 ); /* Wait till TXE = 1*/
            }
            while (base->SR & SPI_SR_BSY);     /* Wait till BSY = 0 */
            /* Wait for SCK idle */
            if (node->node_mode & SPI_MODE_CPOL)
                while (!(GpioPinGet(SPIBUS_SCK_PORT,SPIBUS_SCK_PIN)));
            else
                while ((GpioPinGet(SPIBUS_SCK_PORT,SPIBUS_SCK_PIN)));
        }
        else if (rx_only) {
            (void) base->DR; /* Empty DR */
            if (node->node_mode & SPI_MODE_HALFDUPLEX)
                base->CR1 |= SPI_CR1_BIDIMODE;
            else
                base->CR1 |= SPI_CR1_RXONLY;
            base->CR1 |= SPI_CR1_SPE;
            while( xlen > 0) {
                if(xlen < 2) {
                    /* Follow procedure "Disabling the SPI" */
                    while(!(GpioPinGet(SPIBUS_SCK_PORT,SPIBUS_SCK_PIN)));
                    while(GpioPinGet(SPIBUS_SCK_PORT,SPIBUS_SCK_PIN));
                    base->CR1 &= ~SPI_CR1_SPE;
                }
                xlen--;
                while ((base->SR & SPI_SR_RXNE) == 0 ); /* Wait till RXNE = 1*/
                if (rxbuf) {
                    *(uint8_t *)rxbuf = base->DR;
                    rxbuf++;
                }
            }
        }
        else {
            base->CR1 |= SPI_CR1_SPE;
            *(uint8_t *)&base->DR = *(const uint8_t *)txbuf; /* Write first item */
            while( xlen > 0){
                txbuf++;
                xlen --;
                while ((base->SR & SPI_SR_TXE) == 0 ); /* Wait till TXE = 1*/
                if (xlen > 0)
                    *(uint8_t *)&base->DR = *(const uint8_t *)txbuf;
                while ((base->SR & SPI_SR_RXNE) == 0 );/* Wait till RXNE = 1*/
                *(uint8_t *)rxbuf = base->DR;
                rxbuf++;
            }
            if (node->node_mode & SPI_MODE_CPOL)
                while (!(GpioPinGet(SPIBUS_SCK_PORT,SPIBUS_SCK_PIN)));
            else
                while ((GpioPinGet(SPIBUS_SCK_PORT,SPIBUS_SCK_PIN)));
        }
    }
    base->CR1 &= ~(SPI_CR1_SPE | SPI_CR1_RXONLY | SPI_CR1_BIDIMODE |
                   SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
    return 0;
}

