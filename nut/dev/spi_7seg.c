/*
 * Copyright (C) 2008-2009 by egnite GmbH
 * Copyright (C) 2006 by egnite Software GmbH
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
 * \file dev/spi_7seg.c
 * \brief Routines for 7 segment display.
 *
 * \verbatim
 * $Id: spi_7seg.c,$
 * \endverbatim
 */

#include <cfg/os.h>

#include <sys/nutdebug.h>
#include <sys/timer.h>

#include <string.h>
#include <stdio.h>

#include <cfg/memory.h>

#include <dev/blockdev.h>
#include <dev/spi_7seg.h>

#include <cfg/arch/gpio.h>
#include <dev/spibus_at91.h>


#define SEG_NOOP 0x00
#define SEG_DIG0 0x01
#define SEG_DIG1 0x02
#define SEG_DIG2 0x03
#define SEG_DIG3 0x04
#define SEG_DIG4 0x05
#define SEG_DIG5 0x06
#define SEG_DIG6 0x07
#define SEG_DIG7 0x08
#define SEG_DEC_MODE 	0x09
#define SEG_INTENSITY 	0x0a
#define SEG_SCAN_LIM	0x0b
#define SEG_SHUTDOWN	0x0c
#define SEG_FEATURE		0x0e
#define SEG_DSP_TEST	0x0f

/* shutdown register (0x0c) */
#define SHUTDOWN_RESET 	0x00
#define SHUTDOWN_SOFT	0x80
#define NORM_OP_RESET	0x01
#define NORM_OP_SOFT	0x81

/* decode en register (0x09) */
#define NO_DIG_DECODE	0x00
#define DIG_0_DECODE	0x01
#define DIG_0_3_DECODE	0x0f
#define DIG_0_7_DECODE	0xff

#define TEST_MODE_OFF	0x00
#define TEST_MODE_ON	0x01

/* scan limit register (0x0b) */
#define DISPLAY_LIMIT	2

#define MAX_7SEG_DIGITS 3

#ifndef DISP_7SEG_WRITE_POLLS
#define DISP_7SEG_WRITE_POLLS        1000
#endif


#ifndef SPI_RATE_DISP_7SEG
#define SPI_RATE_DISP_7SEG  1000000
#endif

#ifndef SPI_MODE_DISP_7SEG
#define SPI_MODE_DISP_7SEG SPI_MODE_3
#endif


/*!
 * \brief 7seg SPI node implementation structure.
 */
NUTSPINODE nodeSpi7SEG = {
    NULL,                   /*!< \brief Pointer to the bus controller driver, node_bus. */
    NULL,                   /*!< \brief Pointer to device driver specific settings, node_stat. */
    SPI_RATE_DISP_7SEG,     /*!< \brief Initial clock rate, node_rate. */
    SPI_MODE_DISP_7SEG,     /*!< \brief Initial mode, node_mode. */
    8,                      /*!< \brief Initial data bits, node_bits. */
    0                       /*!< \brief Chip select, node_cs. */
};

/*!
 * \brief 7seg device implementation structure.
 */
NUTDEVICE devSpi7SEG = {
    NULL,                       		/*!< \brief Pointer to next device, dev_next. */
    {'7', 'S', 'E', 'G', 0, 0, 0},    	/*!< \brief Unique device name, dev_name. */
    IFTYP_BLKIO,                		/*!< \brief Type of device, dev_type. */
    0,                          		/*!< \brief Base address, dev_base (not used). */
    0,                         			/*!< \brief First interrupt number, dev_irq (not used). */
    &nodeSpi7SEG,             			/*!< \brief Interface control block, dev_icb. */
    0,              		 			/*!< \brief Driver control block, dev_dcb. */
    Spi7segInit,               			/*!< \brief Driver initialization routine, dev_init. */
    0,        							/*!< \brief Driver specific control function, dev_ioctl. */
	0,									/*!< \brief Read from device, dev_read. */
    0,									/*!< \brief Write to device, dev_write. */
#ifdef __HARVARD_ARCH__
    0,									/*!< \brief Write data from program space to device, dev_write_P. */
#endif
    0,									/*!< \brief Mount volume, dev_open. */
    0,									/*!< \brief Unmount volume, dev_close. */
    0 									/*!< \brief Request file size, dev_size. */
};

/*!
 * \brief Send 7seg command.
 *
 * \param node  Specifies the SPI node.
 * \param addr  Optional command parameter.
 * \param txbuf Pointer to the transmit data buffer, may be set to NULL.
 * \param rxbuf Pointer to the receive data buffer, may be set to NULL.
 * \param xlen  Number of byte to receive and/or transmit.
 */
static int disp7segCommand(NUTSPINODE * node, uint8_t addr, CONST void *txbuf, void *rxbuf, int xlen)
{
    int rc = -1;
    NUTSPIBUS *bus;
    uint8_t *tmp;
	uint8_t cmd[2];

    NUTASSERT(node != NULL);
    bus = (NUTSPIBUS *) node->node_bus;
    NUTASSERT(bus != NULL);
    NUTASSERT(bus->bus_alloc != NULL);
    NUTASSERT(bus->bus_transfer != NULL);
    NUTASSERT(bus->bus_release != NULL);

	/* write address */
	 cmd[0] = addr;
  	
	tmp = (uint8_t *)txbuf;
	/* write data */
	 cmd[1] = tmp[0];
	//cmd[1] = (uint8_t )txbuf[0];
	/* write data */
    rc = (*bus->bus_alloc) (node, 1000);
    if (rc == 0) {
        rc = (*bus->bus_transfer) (node, cmd, NULL, 2);

        (*bus->bus_release) (node);
    }
    return rc;
}


/*!
 * \brief Initialize the 7seg device.
 *
 * This routine determines the 7seg type. It is internally called 
 * by Nut/OS during device registration.
 *
 * The driver framework may call this function more than once.
 *
 * \param dev Specifies the 7seg device.
 *
 * \return 0 on success or -1 if no valid 7seg was found.
 */
int Spi7segInit(NUTDEVICE * dev)
{
	uint8_t data;
//	uint8_t cmd;
//	int rc;
 //   NUTBLOCKIO *blkio;
    NUTSPINODE *node;
   // uint8_t sr;
    //int_fast8_t i;

    NUTASSERT(dev != NULL);
    NUTASSERT(dev->dev_dcb != NULL);
    NUTASSERT(dev->dev_icb != NULL);
    //blkio = dev->dev_dcb;
   node = dev->dev_icb;
	data = TEST_MODE_OFF;
	disp7segCommand(node, SEG_DSP_TEST, &data, NULL, 1);
	
	disp7segCommand(node, SEG_DSP_TEST, &data, NULL, 1);
	
	disp7segCommand(node, SEG_DSP_TEST, &data, NULL, 1);
	
	data = NORM_OP_RESET;
	disp7segCommand(node, SEG_SHUTDOWN, &data, NULL, 1);
	
	data = 2;
	disp7segCommand(node, SEG_SCAN_LIM, &data, NULL, 1);
	
	data = NO_DIG_DECODE;
	disp7segCommand(node, SEG_DEC_MODE, &data, NULL, 1);
	
	data = 0x0F;
	disp7segCommand(node, SEG_INTENSITY, &data, NULL, 1);
	
	data = 0;
	disp7segCommand(node, SEG_DIG0, &data, NULL, 1);
	
	disp7segCommand(node, SEG_DIG1, &data, NULL, 1);
	
	disp7segCommand(node, SEG_DIG2, &data, NULL, 1);
	
	return 0;

}



static CONST unsigned char CharLookup[] = {
	/* '0' */(0x7E),
	/* '1' */(0x30),
	/* '2' */(0x6D),
	/* '3' */(0x79),
	/* '4' */(0x33),
	/* '5' */(0x5B),
	/* '6' */(0x5F),
	/* '7' */(0x70),
	/* '8' */(0x7F),
	/* '9' */(0x7B),
	/* 'A' */(0x77),
	/* 'b' */(0x1F),
	/* 'c' */(0x0D),
	/* 'd' */(0x3D),
	/* 'E' */(0x4F),
	/* 'F' */(0x47),
	/* 'G' */(0x5F),
	/* 'H' */(0x37),
	/* 'i' */(0x10),
	/* 'J' */(0x3C),
	/* 'K' */(0x0F),
	/* 'L' */(0x0E),
	/* 'M' */(0x76),
	/* 'N' */(0x15),
	/* 'O' */(0x1D),
	/* 'P' */(0x67),
	/* 'Q' */(0x73),
	/* 'R' */(0x05),
	/* 'S' */(0x5B),
	/* 'T' */(0x0F),
	/* 'U' */(0x3E),
	/* 'V' */(0x1C),
	/* 'W' */(0x3F),
	/* 'X' */(0x37),
	/* 'Y' */(0x3B),
	/* 'Z' */(0x6D),
	/* '-' */(0x01),
	/*set point on lower right side*/
//	{'.', (unsigned char)DISPL_DP},
	/*unset point on lower right side*/
//	{'^', (unsigned char)~(DISPL_DP)},
	/*
	switch all segments off
	NOTE do not remove the line below!
	*/
//	{' ', (unsigned char)DISPL_OFF},
};
#define HUNDRED 2
#define TENNER 1
#define ANY 0
void display_7seg(NUTDEVICE * dev, uint16_t number)
{
	uint8_t display[3], next=0;
	NUTSPINODE *node;
	NUTASSERT(dev->dev_icb != NULL);
    node = dev->dev_icb;
	
        if (number< 1000) 
        {/*distance between 0 and 99,9 meters*/
           display[HUNDRED]        = number / 100;
	       display[TENNER]  	        = (number % 100) / 10;
	       display[ANY]  	= (number % 100) % 10;
        }
	else 
        {
		 /*TODO FEHLER anzeigen */
        }
       

	for (next=0;next < 3;next++)
	{
		disp7segCommand(node,  2-next+1, &CharLookup[display[next]], NULL, 1);
		// printf("addr: %d data:0x%2X\n",2-next+1,CharLookup[display[next]] );
	}
}

uint8_t display_buffer[MAX_7SEG_DIGITS] = { 0, 0, 0 };

void print_7seg(NUTDEVICE *dev, char* chars)
{
	uint8_t count=0;
	NUTSPINODE *node;
	NUTASSERT(dev->dev_icb != NULL);
    node = dev->dev_icb;
	for(count=0; count<MAX_7SEG_DIGITS; count++)
	{
		uint8_t display = 0;
		if(chars[count] == 0)
		{
			break;
		}
		else if(chars[count] >= '0' && chars[count] <= '9')
		{
			display = CharLookup[chars[count] - '0'];
		}
		else if(chars[count] >= 'A' && chars[count] <= 'Z')
		{
			display = CharLookup[chars[count] - 'A' + 10];
		}
		else if(chars[count] >= 'a' && chars[count] <= 'z')
		{
			display = CharLookup[chars[count] - 'a' + 10];
		}
		else if(chars[count] == '-')
		{
			display = CharLookup[36];
		}
		//save char independent of decimal point
		display_buffer[count] &= 0x80;
		display_buffer[count] |= (display & 0x7F);
		disp7segCommand(node, MAX_7SEG_DIGITS-count, &(display_buffer[count]), NULL, 1);
	}
}

void dot_7seg(NUTDEVICE *dev, uint8_t dotNumber, uint8_t dotCommand)
{
	uint8_t count=0;
	NUTSPINODE *node;
	NUTASSERT(dev->dev_icb != NULL);
    node = dev->dev_icb;
	if(dotNumber<MAX_7SEG_DIGITS)
	{
		switch(dotCommand)
		{
			case DOT_7SEG_SET:
				display_buffer[MAX_7SEG_DIGITS-dotNumber-1] |= 0x80;
				break;
			case DOT_7SEG_CLEAR:
				display_buffer[MAX_7SEG_DIGITS-dotNumber-1] &= 0x7F;
				break;
			case DOT_7SEG_FLIP:
				display_buffer[MAX_7SEG_DIGITS-dotNumber-1] ^= 0x80;
				break;
			default:
				break;
		}
	}
	for(count=0; count<MAX_7SEG_DIGITS; count++)
	{
		disp7segCommand(node, MAX_7SEG_DIGITS-count, &(display_buffer[count]), NULL, 1);
	}
}
