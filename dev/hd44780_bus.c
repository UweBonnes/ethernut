/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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
 *
 */

/*!
 * \file dev/hd44780_bus.c
 * \brief Terminal device definitions for memory mapped lcd.
 *
 *
 * (c) 2004 Ole Reinhardt, Kernel concepts (http://www.kernelconcepts.de)
 * This is a terminal device driver for a memory mapped hd44780 compatible
 * lcd. It is connected to the databus / adressbus. A Chipselect is generated
 * from /rd, /wr, and the address decoder. It is connected to the lcds enable
 * signal. A0 is connected to the register select pin and A1 to the read/write
 * signal. Therefore you'll read from an address with an offset of two
 *
 * Have a look to our m-can board if you have questions.
 */
 
 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/nutconfig.h>
#include <dev/hd44780_bus.h>
#include <dev/term.h>
#include <sys/timer.h>

static u_short lcd_base = 0x0000;

#define LCD_DELAY		asm volatile ("nop"); asm volatile ("nop")


/*!
 * \addtogroup xgDisplay
 */
/*@{*/

/*!
 * \brief Waits until Busy bit is reset or timeout
 */


static inline void LcdBusyWait(void)
{
//	volatile u_char count;
	// wait until LCD busy bit goes to zero
	// do a read from control register
	while(*(volatile u_char *) (LCD_CTRL_ADDR+LCD_READ_OFFSET) & 1<<LCD_BUSY) LCD_DELAY;
	LCD_DELAY;
	LCD_DELAY;
	LCD_DELAY;
	LCD_DELAY;
	LCD_DELAY;
	LCD_DELAY;
	LCD_DELAY;
	LCD_DELAY;
	LCD_DELAY;
	LCD_DELAY;
	LCD_DELAY;
	LCD_DELAY;
}

/*!
 * \brief Write databyte to LCD controller over data bus
 *
 * \param ch Byte to send.
 */

static void LcdWriteData(u_char data)
{
	LcdBusyWait();						// wait until LCD not busy
	*(volatile u_char *) (LCD_DATA_ADDR) = data;
}

/*!
 * \brief Write command byte to LCD controller.
 */
static void LcdWriteCmd(u_char cmd, u_char delay)
{
	LcdBusyWait();						// wait until LCD not busy
	*(volatile u_char *) (LCD_CTRL_ADDR) = cmd;
}

static void LcdSetCursor(u_char pos)
{
	LcdWriteCmd(1<<LCD_DDRAM | pos, 0);
}

static void LcdCursorHome(void)
{
	LcdWriteCmd(1<<LCD_HOME, 0);
}

static void LcdCursorLeft(void)
{
	LcdWriteCmd(1<<LCD_MOVE, 0);
}

static void LcdCursorRight(void)
{
	LcdWriteCmd(1<<LCD_MOVE | 1 << LCD_MOVE_RIGHT, 0);
}

static void LcdClear(void)
{
	LcdWriteCmd(1<<LCD_CLR, 0);
}

static void LcdCursorMode(u_char on)
{
	LcdWriteCmd(1<<LCD_ON_CTRL | on ? 1<< LCD_ON_CURSOR : 0x00, 0);
}

/*!
 * \brief Initialization of the LCD controller
 */

static void LcdInit(NUTDEVICE *dev)
{
	lcd_base = dev->dev_base;
#ifdef  KS0073_CONTROLLER	
	// LCD function set with extended Register Set.
	LcdWriteCmd((1 << LCD_FUNCTION) | 1<<LCD_FUNCTION_8BIT | (1<<LCD_FUNCTION_RE), 0);
	NutDelay(50);
	LcdWriteCmd((1 << LCD_FUNCTION) | 1<<LCD_FUNCTION_8BIT | (1<<LCD_FUNCTION_RE), 0);
	NutDelay(50);
	LcdWriteCmd((1 << LCD_FUNCTION) | 1<<LCD_FUNCTION_8BIT | (1<<LCD_FUNCTION_RE), 0);
	NutDelay(50);
	
	LcdWriteCmd((1 << LCD_EXT) | ((((TERMDCB*)dev->dev_dcb)->dcb_nrows > 2) ?(1<<LCD_EXT_4LINES):0), 0);
	LcdWriteCmd((1 << LCD_FUNCTION) | 1<<LCD_FUNCTION_8BIT, 0);	
	
#else
	
	// LCD function set
	
	LcdWriteCmd((1 << LCD_FUNCTION) | 1<<LCD_FUNCTION_8BIT | (1<<LCD_FUNCTION_2LINES), 0);	
	NutDelay(50);
	LcdWriteCmd((1 << LCD_FUNCTION) | 1<<LCD_FUNCTION_8BIT | (1<<LCD_FUNCTION_2LINES), 0);	
	NutDelay(50);
	LcdWriteCmd((1 << LCD_FUNCTION) | 1<<LCD_FUNCTION_8BIT | (1<<LCD_FUNCTION_2LINES), 0);	
	NutDelay(50);
#endif		
	// clear LCD
	LcdWriteCmd(1<<LCD_CLR, 0);
	// set entry mode
	LcdWriteCmd(1<<LCD_ENTRY_MODE | 1<<LCD_ENTRY_INC, 0);
	// set display to on
	LcdWriteCmd(1<<LCD_ON_CTRL | 1<<LCD_ON_DISPLAY , 0);
	// move cursor to home
	LcdWriteCmd(1<<LCD_HOME, 0);
	// set data address to 0
	LcdWriteCmd(1<<LCD_DDRAM | 0x00, 0);
}

/*!
 * \brief Terminal device control block structure.
 */
TERMDCB dcb_term = {
    LcdInit,            /*!< \brief Initialize display subsystem, dss_init. */
    LcdWriteData,       /*!< \brief Write display character, dss_write. */
    LcdWriteCmd,        /*!< \brief Write display command, dss_command. */
    LcdClear,           /*!< \brief Clear display, dss_clear. */
    LcdSetCursor,       /*!< \brief Set display cursor, dss_set_cursor. */
    LcdCursorHome,      /*!< \brief Set display cursor home, dss_cursor_home. */
    LcdCursorLeft,      /*!< \brief Move display cursor left, dss_cursor_left. */
    LcdCursorRight,     /*!< \brief Move display cursor right, dss_cursor_right. */
    LcdCursorMode,      /*!< \brief Switch cursor on/off, dss_cursor_mode. */
    0,                  /*!< \brief Mode flags. */
    0,                  /*!< \brief Status flags. */
#ifdef  KS0073_CONTROLLER	
    4,                  /*!< \brief Number of rows. */
    32,                 /*!< \brief Number of columns per row. */
    20,                 /*!< \brief Number of visible columns. */
#else
    2,                  /*!< \brief Number of rows. */
    80,                 /*!< \brief Number of columns per row. */
    20,                 /*!< \brief Number of visible columns. */	
#endif
    0,                  /*!< \brief Cursor row. */
    0,                  /*!< \brief Cursor column. */
    0                   /*!< \brief Display shadow memory. */
};

/*!
 * \brief LCD device information structure.
 */
NUTDEVICE devLcdBus = {
    0,              /*!< Pointer to next device. */
    {'l', 'c', 'd', 'b', 'u', 's', 0, 0, 0},      /*!< Unique device name. */
    IFTYP_STREAM,   /*!< Type of device. */
    0,              /*!< Base address. */
    0,              /*!< First interrupt number. */
    0,              /*!< Interface control block. */
    &dcb_term,      /*!< Driver control block. */
    TermInit,       /*!< Driver initialization routine. */
    TermIOCtl,      /*!< Driver specific control function. */
    0,
    TermWrite,
    TermWrite_P,
    TermOpen,
    TermClose,
    0
};

/*@}*/
