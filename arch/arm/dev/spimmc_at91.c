/*
 * Copyright (C) 2006 by egnite Software GmbH. All rights reserved.
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
 */

/*!
 * \brief Low Level Multimedia Card Access.
 *
 * Low level MMC hardware routines, tested on the AT91SAM7X.
 *
 * These routines support SPI mode only and are required by the
 * basic MMC driver.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/07/26 11:20:08  haraldkipp
 * Added MMC/SD-Card support for AT91SAM7X Evaluation Kit.
 *
 *
 * \endverbatim
 */

#include <cfg/arch.h>
#include <cfg/arch/gpio.h>

#include <dev/mmcard.h>
#include <dev/spimmc_at91.h>

#if 0
/* Use for local debugging. */
#define NUTDEBUG
#include <stdio.h>
#endif

/*!
 * \addtogroup xgAt91SpiMmc
 */
/*@{*/

/*!
 * \brief Initialize the card in slot 0.
 *
 * Called by the MMC driver during card reset. The card change
 * detection flag will be cleared.
 *
 * \return 0 on success, -1 if no card is detected.
 */
static int At91SpiMmCard0Init(void)
{
    return 0;
}

/*!
 * \brief Activate or deactivate chip select on card slot 0.
 *
 * \param on The card will be selected if 1, deselected if 0.
 *           Any other value can be used to query the status.
 *
 * \return Previous select status. 1 if selected, 0 otherwise.
 */
static int At91SpiMmCard0Select(int on)
{
    int rc = (inr(PIOA_ODSR) & _BV(SPI0_NPCS1_PA13A)) == 0;

    /* MMC select is low active. */
    if (on == 1) {
        outr(PIOA_CODR, _BV(SPI0_NPCS1_PA13A));
    } else if (on == 0) {
        outr(PIOA_SODR, _BV(SPI0_NPCS1_PA13A));
    }
    return rc;
}

/*!
 * \brief Read the previously received byte and transmit a new one.
 *
 * \param val Byte to transmit.
 *
 * \return Last byte received.
 */
static u_char At91SpiMmCard0Io(u_char val)
{
#ifdef NUTDEBUG
    putchar('[');
    if (val != 0xFF) {
        printf("s%02X", val);
    }
#endif

    /* Transmission is started by writing the transmit data. */
    outr(SPI0_TDR, val);
    /* Wait for receiver data register full. */
    while((inr(SPI0_SR) & SPI_RDRF) == 0);
    /* Read data. */
    val = (u_char)inr(SPI0_RDR);

#ifdef NUTDEBUG
    if (val != 0xFF) {
        printf("r%02X", val);
    }
    putchar(']');
#endif
    return val;
}

/*!
 * \brief Check if card is available in slot 0.
 *
 * \todo Card change should verify the card identifier. Right now
 *       any detection of removing and re-inserting a card counts
 *       as a card change.
 *
 * \return 0 if no card is detected, 1 if a card is available or 2 if
 *         a card change had been detected after the last mount.
 */
int At91SpiMmCard0Avail(void)
{
    return 1;
}

/*!
 * \brief Check if card in slot 0 is write protected.
 *
 * \todo Not implemented.
 *
 * \return Always 0.
 */
int At91SpiMmCard0WrProt(void)
{
    return 0;
}

/*!
 * \brief Initialize MMC hardware interface.
 *
 * This function is automatically executed during during device
 * registration via NutRegisterDevice().
 *
 * \param dev Identifies the device to initialize.
 */
static int At91SpiMmcIfcInit(NUTDEVICE * dev)
{
    /* Disable PIO lines used for SPI. */
    outr(PIOA_PDR, _BV(SPI0_MISO_PA16A) | _BV(SPI0_MOSI_PA17A) | _BV(SPI0_SPCK_PA18A));
    /* SPI is in peripheral group A. Enable it. */
    outr(PIOA_ASR, _BV(SPI0_MISO_PA16A) | _BV(SPI0_MOSI_PA17A) | _BV(SPI0_SPCK_PA18A));

    /* MMC chip select is manually controlled. */
    outr(PIOA_PER, _BV(SPI0_NPCS1_PA13A));
	outr(PIOA_SODR, _BV(SPI0_NPCS1_PA13A));
	outr(PIOA_OER, _BV(SPI0_NPCS1_PA13A));

    /* Enable SPI clock. */
    outr(PMC_PCER, _BV(SPI0_ID));

    /* SPI enable and reset. */
    outr(SPI0_CR, SPI_SPIEN | SPI_SWRST);
    outr(SPI0_CR, SPI_SPIEN);

    /* Set SPI to master mode, fixed peripheral at CS1, fault detection disabled. */
    outr(SPI0_MR, (1 << SPI_PCS_LSB) | SPI_MODFDIS | SPI_MSTR);

    /* Data changes during clock low and will be sampled on rising edges. */
    outr(SPI0_CSR1, (3 << SPI_SCBR_LSB) | SPI_CPOL);

    return MmCardDevInit(dev);
}

static MMCIFC mmc0_ifc = {
    At91SpiMmCard0Init,             /*!< mmcifc_in */
    At91SpiMmCard0Io,               /*!< mmcifc_io */
    At91SpiMmCard0Select,           /*!< mmcifc_cs */
    At91SpiMmCard0Avail,            /*!< mmcifc_cd */
    At91SpiMmCard0WrProt            /*!< mmcifc_wp */
};

/*!
 * \brief Multimedia card device information structure.
 *
 * A pointer to this structure must be passed to NutRegisterDevice()
 * to bind this driver to the Nut/OS kernel. An application may then
 * call
 * /verbatim
 * _open("MMC0:", _O_RDWR | _O_BINARY);
 * /endverbatim
 * to mount the first active primary partition with any previously
 * registered file system driver (typically devPhat0).
 */
NUTDEVICE devAt91SpiMmc0 = {
    0,                          /*!< Pointer to next device, dev_next. */
    {'M', 'M', 'C', '0', 0, 0, 0, 0, 0}
    ,                           /*!< Unique device name, dev_name. */
    0,                          /*!< Type of device, dev_type. Obsolete. */
    0,                          /*!< Base address, dev_base. Unused. */
    0,                          /*!< First interrupt number, dev_irq. Unused. */
    &mmc0_ifc,                  /*!< Interface control block, dev_icb. */
    0,                          /*!< Driver control block used by the low level part, dev_dcb. */
    At91SpiMmcIfcInit,          /*!< Driver initialization routine, dev_init. */
    MmCardIOCtl,                /*!< Driver specific control function, dev_ioctl. */
    MmCardBlockRead,            /*!< Read data from a file, dev_read. */
    MmCardBlockWrite,           /*!< Write data to a file, dev_write. */
#ifdef __HARVARD_ARCH__
    MmCardBlockWrite_P,         /*!< Write data from program space to a file, dev_write_P. */
#endif
    MmCardMount,                /*!< Mount a file system, dev_open. */
    MmCardUnmount,              /*!< Unmount a file system, dev_close. */
    0                           /*!< Return file size, dev_size. */
};

/*@}*/
