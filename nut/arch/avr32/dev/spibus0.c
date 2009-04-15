/*
* Copyright (C) 2008-2009 by egnite GmbH
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
* \file arch/avr32/dev/spibus0.c
* \brief Primary AVR32 SPI bus controller.
*
* May be configured as an interrupt driven or polling driver. The
* interrupt driven version may use single or double buffering.
*
* \verbatim
* $Id: spibus0.c,v 1.3 2009/01/30 08:56:39 haraldkipp Exp $
* \endverbatim
*/

#include <cfg/spi.h>

#include <arch/avr32.h>
#include <arch/avr32/gpio.h>

#include <dev/spibus_avr32.h>
#include <dev/irqreg.h>
#include <dev/gpio.h>
#include <sys/event.h>
#include <sys/nutdebug.h>

#include <stdlib.h>
#include <errno.h>

#include <avr32/io.h>

#if defined(SPI0_CS0_PIO_BIT)
#if defined(SPI0_CS0_PIO_ID)
#undef GPIO_ID
#define GPIO_ID SPI0_CS0_PIO_ID
#include <cfg/arch/porttran.h>
static INLINE void SPI0_CS0_LO(void) { GPIO_SET_LO(SPI0_CS0_PIO_BIT); }
static INLINE void SPI0_CS0_HI(void) { GPIO_SET_HI(SPI0_CS0_PIO_BIT); }
static INLINE void SPI0_CS0_SO(void) { GPIO_ENABLE(SPI0_CS0_PIO_BIT); GPIO_OUTPUT(SPI0_CS0_PIO_BIT); }
#else
#define SPI0_CS0_LO()
#define SPI0_CS0_HI()
#define SPI0_CS0_SO()
#endif
#endif

#if defined(SPI0_CS1_PIO_BIT)
#if defined(SPI0_CS1_PIO_ID)
#undef GPIO_ID
#define GPIO_ID SPI0_CS1_PIO_ID
#include <cfg/arch/porttran.h>
static INLINE void SPI0_CS1_LO(void) { GPIO_SET_LO(SPI0_CS1_PIO_BIT); }
static INLINE void SPI0_CS1_HI(void) { GPIO_SET_HI(SPI0_CS1_PIO_BIT); }
static INLINE void SPI0_CS1_SO(void) { GPIO_ENABLE(SPI0_CS1_PIO_BIT); GPIO_OUTPUT(SPI0_CS1_PIO_BIT); }
#else
#define SPI0_CS1_LO()
#define SPI0_CS1_HI()
#define SPI0_CS1_SO()
#endif
#endif

#if defined(SPI0_CS2_PIO_BIT)
#if defined(SPI0_CS2_PIO_ID)
#undef GPIO_ID
#define GPIO_ID SPI0_CS2_PIO_ID
#include <cfg/arch/porttran.h>
static INLINE void SPI0_CS2_LO(void) { GPIO_SET_LO(SPI0_CS2_PIO_BIT); }
static INLINE void SPI0_CS2_HI(void) { GPIO_SET_HI(SPI0_CS2_PIO_BIT); }
static INLINE void SPI0_CS2_SO(void) { GPIO_ENABLE(SPI0_CS2_PIO_BIT); GPIO_OUTPUT(SPI0_CS2_PIO_BIT); }
#else
#define SPI0_CS2_LO()
#define SPI0_CS2_HI()
#define SPI0_CS2_SO()
#endif
#endif

#if defined(SPI0_CS3_PIO_BIT)
#if defined(SPI0_CS3_PIO_ID)
#undef GPIO_ID
#define GPIO_ID SPI0_CS3_PIO_ID
#include <cfg/arch/porttran.h>
static INLINE void SPI0_CS3_LO(void) { GPIO_SET_LO(SPI0_CS3_PIO_BIT); }
static INLINE void SPI0_CS3_HI(void) { GPIO_SET_HI(SPI0_CS3_PIO_BIT); }
static INLINE void SPI0_CS3_SO(void) { GPIO_ENABLE(SPI0_CS3_PIO_BIT); GPIO_OUTPUT(SPI0_CS3_PIO_BIT); }
#else
#define SPI0_CS3_LO()
#define SPI0_CS3_HI()
#define SPI0_CS3_SO()
#endif
#endif

/*!
* \brief Set the specified chip select to a given level.
*/
int Avr32Spi0ChipSelect(uint_fast8_t cs, uint_fast8_t hi)
{
	int rc = 0;

	switch (cs) {
#if defined(SPI0_CS0_PIO_BIT)
	case 0:
		if (hi) {
			SPI0_CS0_HI();
		} else {
			SPI0_CS0_LO();
		}
		SPI0_CS0_SO();
		break;
#endif
#if defined(SPI0_CS1_PIO_BIT)
	case 1:
		if (hi) {
			SPI0_CS1_HI();
		} else {
			SPI0_CS1_LO();
		}
		SPI0_CS1_SO();
		break;
#endif
#if defined(SPI0_CS2_PIO_BIT)
	case 2:
		if (hi) {
			SPI0_CS2_HI();
		} else {
			SPI0_CS2_LO();
		}
		SPI0_CS2_SO();
		break;
#endif
#if defined(SPI0_CS3_PIO_BIT)
	case 3:
		if (hi) {
			SPI0_CS3_HI();
		} else {
			SPI0_CS3_LO();
		}
		SPI0_CS3_SO();
		break;
#endif
	default:
		errno = EIO;
		rc = -1;
		break;
	}
	return rc;
}

/*! \brief Select a device on the first SPI bus.
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
int Avr32SpiBus0Select(NUTSPINODE * node, uint32_t tmo)
{
	int rc;

	/* Sanity check. */
	NUTASSERT(node != NULL);
	NUTASSERT(node->node_bus != NULL);
	NUTASSERT(node->node_stat != NULL);

	/* Allocate the bus. */
	rc = NutEventWait(&node->node_bus->bus_mutex, tmo);
	if (rc) {
		errno = EIO;
	} else {
		AVR32SPIREG *spireg = node->node_stat;

		/* Enable SPI peripherals and clock. */
		GpioPinConfigSet( AVR32_GPIO_BANK(AVR32_SPI0_SCK_0_PIN), AVR32_GPIO_PIN(AVR32_SPI0_SCK_0_PIN), AVR32_GPIO_FUNCTION(AVR32_SPI0_SCK_0_FUNCTION) );
		GpioPinConfigSet( AVR32_GPIO_BANK(AVR32_SPI0_MISO_0_PIN), AVR32_GPIO_PIN(AVR32_SPI0_MISO_0_PIN), AVR32_GPIO_FUNCTION(AVR32_SPI0_MISO_0_FUNCTION) );
		GpioPinConfigSet( AVR32_GPIO_BANK(AVR32_SPI0_MOSI_0_PIN), AVR32_GPIO_PIN(AVR32_SPI0_MOSI_0_PIN), AVR32_GPIO_FUNCTION(AVR32_SPI0_MOSI_0_FUNCTION) );
		GpioPinConfigSet( AVR32_GPIO_BANK(AVR32_SPI0_NPCS_0_PIN), AVR32_GPIO_PIN(AVR32_SPI0_NPCS_0_PIN), AVR32_GPIO_FUNCTION(AVR32_SPI0_NPCS_0_FUNCTION) );

		/* If the mode update bit is set, then update our registers. */
		if (node->node_mode & SPI_MODE_UPDATE) {
			Avr32SpiSetup(node);
		}

		/* Set SPI mode. */
		outr(AVR32_SPI0_ADDRESS + AVR32_SPI_MR, spireg->mr);
		//outr(AVR32_SPI0_ADDRESS + AVR32_SPI_CSR0 + (node->node_cs * (AVR32_SPI_CSR0 - AVR32_SPI_CSR0)), spireg->csr);
		outr(AVR32_SPI0_ADDRESS + AVR32_SPI_CSR0, spireg->csr);

		/* Enable SPI. */
		AVR32_SPI0.cr |= AVR32_SPI_CR_SPIEN_MASK;

		/* Finally activate the node's chip select. */
		rc = Avr32Spi0ChipSelect(node->node_cs, (node->node_mode & SPI_MODE_CSHIGH) != 0);
		if (rc) {
			/* Release the bus in case of an error. */
			NutEventPost(&node->node_bus->bus_mutex);
		}
	}
	return rc;
}

/*! \brief Deselect a device on the first SPI bus.
*
* Deactivates the chip select and unlocks the bus.
*
* \param node Specifies the SPI bus node.
*
* \return Always 0.
*/
int Avr32SpiBus0Deselect(NUTSPINODE * node)
{
	/* Sanity check. */
	NUTASSERT(node != NULL);
	NUTASSERT(node->node_bus != NULL);

#ifdef SPIBUS0_DOUBLE_BUFFER
	Avr32SpiBusWait(node, NUT_WAIT_INFINITE);
#endif
	/* Deactivate the node's chip select. */
	Avr32Spi0ChipSelect(node->node_cs, (node->node_mode & SPI_MODE_CSHIGH) == 0);

	/* Release the bus. */
	NutEventPost(&node->node_bus->bus_mutex);

	return 0;
}

#if !defined(SPIBUS0_POLLING_MODE) || !defined(SPIBUS0_DOUBLE_BUFFER)

static uint8_t * volatile spi0_txp;
static uint8_t * volatile spi0_rxp;
static volatile size_t spi0_xc;

void Avr32SpiBus0Interrupt(void *arg)
{
	uint8_t b;

	/* Get the received byte. */
	b = (uint8_t)inr(AVR32_SPI0_ADDRESS + AVR32_SPI_RDR) >> AVR32_SPI_RDR_RD_OFFSET;
	if (spi0_xc) {
		if (spi0_rxp) {
			*spi0_rxp++ = b;
		}
		spi0_xc--;
	}
	if (spi0_xc) {
		if (spi0_txp) {
			b = *spi0_txp++;
		}
		outr(AVR32_SPI0_ADDRESS + AVR32_SPI_TDR, (b << AVR32_SPI_TDR_TD_OFFSET));
	} else {
		NutEventPostFromIrq((void **)arg);
	}
}

/*! 
* \brief Transfer data on the SPI bus using single buffered interrupt mode.
*
* A device must have been selected by calling Avr32SpiSelect().
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
int Avr32SpiBus0Transfer(NUTSPINODE * node, CONST void *txbuf, void *rxbuf, int xlen)
{
	uint8_t b = 0xff;
	uintptr_t base;

	/* Sanity check. */
	NUTASSERT(node != NULL);
	NUTASSERT(node->node_bus != NULL);
	NUTASSERT(node->node_bus->bus_base != 0);
	base = node->node_bus->bus_base;

	if (xlen) {
		spi0_txp = (uint8_t *) txbuf;
		spi0_rxp = (uint8_t *) rxbuf;
		spi0_xc = (size_t) xlen;
		if (spi0_txp) {
			b = *spi0_txp++;
		}
		/* Enable and kick interrupts. */
		outr(base + AVR32_SPI_IER, AVR32_SPI_IER_RDRF_MASK);
		outr(base + AVR32_SPI_TDR, (b << AVR32_SPI_TDR_TD_OFFSET));
		/* Wait until transfer has finished. */
		NutEventWait(&node->node_bus->bus_ready, NUT_WAIT_INFINITE);
		outr(base + AVR32_SPI_IDR, (unsigned int) - 1);
	}
	return 0;
}
#endif

/*!
* \brief Avr32 SPI bus driver implementation structure.
*/
NUTSPIBUS spiBus0Avr32 = {
	NULL,                       /*!< Bus mutex semaphore (bus_mutex). */
	NULL,                       /*!< Bus ready signal (bus_ready). */
	AVR32_SPI0_ADDRESS,          /*!< Bus base address (bus_base). */
	&sig_SPI0,                  /*!< Bus interrupt handler (bus_sig). */
	Avr32SpiBusNodeInit,         /*!< Initialize the bus (bus_initnode). */
	Avr32SpiBus0Select,          /*!< Select the specified device (bus_alloc). */
	Avr32SpiBus0Deselect,        /*!< Deselect the specified device (bus_release). */
#if defined(SPIBUS0_POLLING_MODE)
	Avr32SpiBusPollTransfer,     /*!< Transfer data to and from a specified node (bus_transfer). */
#elif defined(SPIBUS0_DOUBLE_BUFFER)
	Avr32SpiBusDblBufTransfer,
#else
	Avr32SpiBus0Transfer,
#endif
#ifdef SPIBUS0_DOUBLE_BUFFER
	Avr32SpiBusWait,
#else
	NutSpiBusWait,              /*!< Wait for bus transfer ready (bus_wait). */
#endif
	NutSpiBusSetMode,           /*!< Set SPI mode of a specified device (bus_set_mode). */
	NutSpiBusSetRate,           /*!< Set clock rate of a specified device (bus_set_rate). */
	NutSpiBusSetBits            /*!< Set number of data bits of a specified device (bus_set_bits). */
};