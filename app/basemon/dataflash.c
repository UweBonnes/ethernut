/*
 * Copyright (C) 2001-2004 by egnite Software GmbH. All rights reserved.
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

/*
 * $Log$
 * Revision 1.2  2005/02/07 19:05:19  haraldkipp
 * ATmega 103 compile errors fixed
 *
 */

#include "dataflash.h"

/*!
 * \brief Exchange SPI byte.
 *
 * Inlined?
 */
static u_char SpiByte(u_char c)
{
    outb(SPDR, c);
    while ((inb(SPSR) & 0x80) == 0);

    return inb(SPDR);
}

/*!
 * \brief Init SPI interface.
 *
 * Should be more general (Dataflash, VS1001 etc.)
 * Dataflash requires SPI mode 3 (enable flash when SCK is high)
 *
 * - PB1(SCK) -> Mem-SCK
 * - PB2(MOSI) -> Mem-SI
 * - PB3(MISO) <- Mem-SO
 * - PB4 -> Mem-CS (through inverter)
 *
 */
static void SpiInit(void)
{
    /*
     * Init SS pin. When configured as input, we will lose master
     * mode, if this pin goes low. Thus we switch on the pull-up.
     */
    if(bit_is_clear(DDRB, 0)) {
        sbi(PORTB, 0);
    }

    /* Set SCK output. */
    sbi(DDRB, 1);
    /* Set MOSI output. */
    sbi(DDRB, 2);
    /* Enable MISO pullup. */
    sbi(PORTB, 3);

#ifdef __AVR_ATmega128__
    /* Set double speed. */
    outb(SPSR, _BV(SPI2X));
#endif

    /* Enable SPI master mode 3, fosc/2. */
    outb(SPCR, _BV(SPE) | _BV(MSTR) | _BV(CPHA) | _BV(CPOL));

    /* Clean-up the status. */
    outb(SPSR, inb(SPSR));
    inb(SPDR);
}

/*!
 * \brief Read memory chip status.
 */
static u_char SpiMemStatus(void)
{
    u_char rc;

    sbi(SPIMEM_CS_PORT, SPIMEM_CS_BIT);
    SpiByte(0x57);
    rc = SpiByte(0);
    cbi(SPIMEM_CS_PORT, SPIMEM_CS_BIT);

    return rc;
}

static int SpiMemInit(u_short *pages, u_short *pagesize)
{
    u_char fs;
    
    /* Init SPI memory chip select. */
    cbi(SPIMEM_CS_PORT, SPIMEM_CS_BIT);
    sbi(SPIMEM_CS_DDR, SPIMEM_CS_BIT);
    
    /* Initialize the SPI interface. */
    SpiInit();
    
    /* Read the status register for a rudimentary check. */
    fs = SpiMemStatus();
    if(fs & 0x80) {
        fs = (fs >> 2) & 0x0F;
        *pagesize = 264;
        if(fs == 3) {
            *pages = 512;
            return 0;
        }
        else if(fs == 5) {
            *pages = 1024;
            return 0;
        }
        else if(fs == 7) {
            *pages = 2048;
            return 0;
        }
    }
    return -1;
}

/*
 * \return Number of bytes.
 */
long SpiMemTest(void)
{
    u_short pages = 0;
    u_short pagesize = 0;

    SpiMemInit(&pages, &pagesize);

    /* Disable SPI */
    outb(SPCR, 0);

    return (long)pages * (long)pagesize;
}
