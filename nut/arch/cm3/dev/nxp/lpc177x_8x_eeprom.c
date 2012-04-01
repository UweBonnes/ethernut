/*
 * Copyright (C) 2012 by Ole Reinhardt (ole.reinhardt@embedded-it.de)
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
 *
 *
 * Parts taken from lpc177x_8x_eeprom.c			2011-06-02
 * file		lpc177x_8x_eeprom.c
 * brief	Contains all functions support for EEPROM firmware library on
 *			on LPC177x_8x
 * version	1.0
 * date		02. June. 2011
 * author	NXP MCU SW Application Team
 * 
 * Copyright(C) 2011, NXP Semiconductor
 * All rights reserved.
 *
 ***********************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/

#include <inttypes.h>
#include <sys/timer.h>
#include <arch/cm3.h>
#include <arch/cm3/nxp/lpc177x_8x.h>
#include <arch/cm3/nxp/lpc177x_8x_clk.h>
#include <arch/cm3/nxp/lpc177x_8x_eeprom.h>

static uint8_t eeprom_init = 0;

/*!
 * \brief       Initialize EEPROM
 *
 * Initialize the eeprom memeory. Therefore enable power and setup clocks
 *
 * \param       none
 * \return      none
 */

void Lpc177x_8x_EepromInit(void)
{
    uint32_t cpu_clock;
    uint32_t clk_div;
    uint32_t waitstates;

    if (eeprom_init) {
        return;
    }
    
    /* EEPROM is automate turn on after reset */
    LPC_EEPROM->PWRDWN = 0x0;

    /* Clocks have to be set:
     * EEPROM requires a 375kHz. This clock is generated by dividing the
     * system bus clock.
     */
    cpu_clock = NutArchClockGet(NUT_HWCLK_CPU);
    clk_div = (cpu_clock / 375000) - 1;
    LPC_EEPROM->CLKDIV = clk_div;

    /* Setting wait state */
    waitstates  = ((((cpu_clock  / 1000000) * 15) / 1000) + 1);
    waitstates |= (((((cpu_clock / 1000000) * 55) / 1000) + 1) << 8);
    waitstates |= (((((cpu_clock / 1000000) * 35) / 1000) + 1) << 16);
    LPC_EEPROM->WSTATE = waitstates;

    eeprom_init = 1;
}


/*!
 * \brief       Read data from EEPROM at specific address
 *
 * \param       address EEPROM address that start to write data, it must be
 * 				in range 0..0x1000
 * \param       buff	buffer to place the read data in
 * \param       size    number of bytes to be read
 *
 * \return      none
 */
#include <stdio.h>
int Lpc177x_8x_EepromRead(uint16_t addr, void* buff, size_t size)
{
    uint16_t  page_nr;
    uint16_t  page_offs;

    uint32_t  i;
    uint8_t  *pt = (uint8_t*) buff;

    if (addr + size > EEPROM_SIZE) {
        return -1;
    }
    
    if (!eeprom_init) {
        Lpc177x_8x_EepromInit();
    }
    
    page_offs = addr & EEPROM_PAGE_OFFSET_MASK;
    page_nr   = (addr & EEPROM_PAGE_NUM_MASK) >> 6;

    LPC_EEPROM->INT_CLR_STATUS = (_BV(EEPROM_ENDOF_RW) | _BV(EEPROM_ENDOF_PROG));
    LPC_EEPROM->ADDR = EEPROM_PAGE_ADRESS(page_nr) | EEPROM_PAGE_OFFSET(page_offs);
    LPC_EEPROM->CMD  = EEPROM_CMD_8_BIT_READ | EEPROM_CMD_RDPREFETCH;

	/* Read and store data in buffer */
	for(i = 0; i < size; i++){
        /* Check if we read beyond the eeprom size */
        if (page_nr >= EEPROM_PAGE_NUM) {
            return -1;
        }
        
        LPC_EEPROM->INT_CLR_STATUS = _BV(EEPROM_ENDOF_RW);

        *(pt++) = (uint8_t)(LPC_EEPROM->RDATA);
        page_offs ++;

        while (!(LPC_EEPROM->INT_STATUS & _BV(EEPROM_ENDOF_RW)));
           
        if(page_offs >= EEPROM_PAGE_SIZE) {
            page_offs = 0;
            page_nr++;
            LPC_EEPROM->ADDR = EEPROM_PAGE_ADRESS(page_nr) | EEPROM_PAGE_OFFSET(page_offs);
            LPC_EEPROM->CMD = EEPROM_CMD_8_BIT_READ | EEPROM_CMD_RDPREFETCH;
        }
    }
    return 0;
}


/*!
 * \brief       Write data to EEPROM at specific address
 *
 * \param       address EEPROM address that start to write data, it must be
 * 				in range 0..0x1000
 * \param       buff	buffer that contain data that will be written to buffer
 * \param       size    number of bytes to be written
 *
 * \return      none
 */

int Lpc177x_8x_EepromWrite(uint16_t addr, CONST void* buff, size_t size)
{
    uint16_t  page_nr;
    uint16_t  page_offs;

    uint32_t  i;
    uint8_t   *pt = (uint8_t*) buff;

    if (addr + size > EEPROM_SIZE) {
        return -1;
    }
    
    if (!eeprom_init) {
        Lpc177x_8x_EepromInit();
    }
    
    page_offs = addr & EEPROM_PAGE_OFFSET_MASK;
    page_nr = (addr & EEPROM_PAGE_NUM_MASK) >> 6;

    LPC_EEPROM->INT_CLR_STATUS = (_BV(EEPROM_ENDOF_RW) | _BV(EEPROM_ENDOF_PROG));
    LPC_EEPROM->ADDR = EEPROM_PAGE_ADRESS(page_nr) | EEPROM_PAGE_OFFSET(page_offs);
    LPC_EEPROM->INT_CLR_STATUS = _BV(EEPROM_ENDOF_RW);

    for(i = 0; i < size; i++) {
        /* Update data to page register */
        LPC_EEPROM->CMD = EEPROM_CMD_8_BIT_WRITE;
        LPC_EEPROM->WDATA = *(pt++);
        page_offs ++;
            
        if ((page_offs >= EEPROM_PAGE_SIZE) | (i == size - 1)) {
            /* Update to EEPROM memory */
            
            LPC_EEPROM->INT_CLR_STATUS = _BV(EEPROM_ENDOF_PROG);
            LPC_EEPROM->ADDR = EEPROM_PAGE_ADRESS(page_nr);
            LPC_EEPROM->CMD = EEPROM_CMD_ERASE_PRG_PAGE;

            while (!(LPC_EEPROM->INT_STATUS & _BV(EEPROM_ENDOF_PROG)));
        }
        
        if (page_offs >= EEPROM_PAGE_SIZE) {
            page_offs = 0;
            page_nr ++;
            LPC_EEPROM->ADDR =0;
            if(page_nr >= EEPROM_PAGE_NUM) {
                page_nr = 0;
            }
        }
    }
    return 0;
}
