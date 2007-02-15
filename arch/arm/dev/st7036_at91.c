/*
 * Copyright (C) 2007 by egnite Software GmbH. All rights reserved.
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

/*
 * $Log$
 * Revision 1.1  2007/02/15 16:09:07  haraldkipp
 * Tested with EA DOG-M LCDs.
 *
 */

#include <cfg/arch.h>
#include <cfg/arch/gpio.h>
#include <cfg/lcd.h>

#include <stdlib.h>
#include <string.h>

#include <sys/nutconfig.h>
#include <dev/st7036.h>
#include <dev/term.h>
#include <sys/timer.h>

#ifndef LCD_ROWS
#define LCD_ROWS    2
#endif

#ifndef LCD_COLS
#define LCD_COLS    16
#endif

#ifndef LCD_SHORT_DELAY
#define LCD_SHORT_DELAY 100
#endif

#ifndef LCD_LONG_DELAY
#define LCD_LONG_DELAY  1000
#endif

/*!
 * \brief GPIO controller ID.
 */
#if defined(AT91SAM7X_EK)

#ifndef LCD_CS_PIO_ID
#define LCD_CS_PIO_ID   PIOB_ID
#endif
#ifndef LCD_CS_BIT
#define LCD_CS_BIT	    24
#endif

#ifndef LCD_RS_PIO_ID
#define LCD_RS_PIO_ID   PIOB_ID
#endif
#ifndef LCD_RS_BIT
#define LCD_RS_BIT	    23
#endif

#ifndef LCD_CLK_PIO_ID
#define LCD_CLK_PIO_ID  PIOA_ID
#endif
#ifndef LCD_CLK_BIT
#define LCD_CLK_BIT	    18
#endif

#ifndef LCD_MOSI_PIO_ID
#define LCD_MOSI_PIO_ID PIOA_ID
#endif
#ifndef LCD_MOSI_BIT
#define LCD_MOSI_BIT	17
#endif

#elif defined(AT91SAM9260_EK)

#ifndef LCD_CS_PIO_ID
#define LCD_CS_PIO_ID   PIOB_ID
#endif
#ifndef LCD_CS_BIT
#define LCD_CS_BIT	    11
#endif

#ifndef LCD_RS_PIO_ID
#define LCD_RS_PIO_ID   PIOB_ID
#endif
#ifndef LCD_RS_BIT
#define LCD_RS_BIT	    20
#endif

#ifndef LCD_CLK_PIO_ID
#define LCD_CLK_PIO_ID  PIOB_ID
#endif
#ifndef LCD_CLK_BIT
#define LCD_CLK_BIT	    2
#endif

#ifndef LCD_MOSI_PIO_ID
#define LCD_MOSI_PIO_ID PIOB_ID
#endif
#ifndef LCD_MOSI_BIT
#define LCD_MOSI_BIT	1
#endif

#else

#ifndef LCD_CS_PIO_ID
#define LCD_CS_PIO_ID   PIO_ID
#endif
#ifndef LCD_CS_BIT
#define LCD_CS_BIT	    0
#endif

#ifndef LCD_RS_PIO_ID
#define LCD_RS_PIO_ID   PIO_ID
#endif
#ifndef LCD_RS_BIT
#define LCD_RS_BIT	    3
#endif

#ifndef LCD_CLK_PIO_ID
#define LCD_CLK_PIO_ID  PIO_ID
#endif
#ifndef LCD_CLK_BIT
#define LCD_CLK_BIT	    1
#endif

#ifndef LCD_MOSI_PIO_ID
#define LCD_MOSI_PIO_ID PIO_ID
#endif
#ifndef LCD_MOSI_BIT
#define LCD_MOSI_BIT	2
#endif

#endif

#define LCD_CS      _BV(LCD_CS_BIT)
#define LCD_RS      _BV(LCD_RS_BIT)
#define LCD_CLK     _BV(LCD_CLK_BIT)
#define LCD_MOSI    _BV(LCD_MOSI_BIT)


#if LCD_CS_PIO_ID == PIOA_ID
#define LCD_CS_SET() { outr(PIOA_PER, LCD_CS); outr(PIOA_SODR, LCD_CS); outr(PIOA_OER, LCD_CS); }
#define LCD_CS_CLR() { outr(PIOA_PER, LCD_CS); outr(PIOA_CODR, LCD_CS); outr(PIOA_OER, LCD_CS); }
#elif LCD_CS_PIO_ID == PIOB_ID
#define LCD_CS_SET() { outr(PIOB_PER, LCD_CS); outr(PIOB_SODR, LCD_CS); outr(PIOB_OER, LCD_CS); }
#define LCD_CS_CLR() { outr(PIOB_PER, LCD_CS); outr(PIOB_CODR, LCD_CS); outr(PIOB_OER, LCD_CS); }
#elif LCD_CS_PIO_ID == PIOC_ID
#define LCD_CS_SET() { outr(PIOC_PER, LCD_CS); outr(PIOC_SODR, LCD_CS); outr(PIOC_OER, LCD_CS); }
#define LCD_CS_CLR() { outr(PIOC_PER, LCD_CS); outr(PIOC_CODR, LCD_CS); outr(PIOC_OER, LCD_CS); }
#else
#define LCD_CS_SET() { outr(PIO_PER, LCD_CS); outr(PIO_SODR, LCD_CS); outr(PIO_OER, LCD_CS); }
#define LCD_CS_CLR() { outr(PIO_PER, LCD_CS); outr(PIO_CODR, LCD_CS); outr(PIO_OER, LCD_CS); }
#endif

#if LCD_RS_PIO_ID == PIOA_ID
#define LCD_RS_SET() { outr(PIOA_PER, LCD_RS); outr(PIOA_SODR, LCD_RS); outr(PIOA_OER, LCD_RS); }
#define LCD_RS_CLR() { outr(PIOA_PER, LCD_RS); outr(PIOA_CODR, LCD_RS); outr(PIOA_OER, LCD_RS); }
#elif LCD_RS_PIO_ID == PIOB_ID
#define LCD_RS_SET() { outr(PIOB_PER, LCD_RS); outr(PIOB_SODR, LCD_RS); outr(PIOB_OER, LCD_RS); }
#define LCD_RS_CLR() { outr(PIOB_PER, LCD_RS); outr(PIOB_CODR, LCD_RS); outr(PIOB_OER, LCD_RS); }
#elif LCD_RS_PIO_ID == PIOC_ID
#define LCD_RS_SET() { outr(PIOC_PER, LCD_RS); outr(PIOC_SODR, LCD_RS); outr(PIOC_OER, LCD_RS); }
#define LCD_RS_CLR() { outr(PIOC_PER, LCD_RS); outr(PIOC_CODR, LCD_RS); outr(PIOC_OER, LCD_RS); }
#else
#define LCD_RS_SET() { outr(PIO_PER, LCD_RS); outr(PIO_SODR, LCD_RS); outr(PIO_OER, LCD_RS); }
#define LCD_RS_CLR() { outr(PIO_PER, LCD_RS); outr(PIO_CODR, LCD_RS); outr(PIO_OER, LCD_RS); }
#endif

#if LCD_CLK_PIO_ID == PIOA_ID
#define LCD_CLK_SET() { outr(PIOA_PER, LCD_CLK); outr(PIOA_SODR, LCD_CLK); outr(PIOA_OER, LCD_CLK); }
#define LCD_CLK_CLR() { outr(PIOA_PER, LCD_CLK); outr(PIOA_CODR, LCD_CLK); outr(PIOA_OER, LCD_CLK); }
#elif LCD_CLK_PIO_ID == PIOB_ID
#define LCD_CLK_SET() { outr(PIOB_PER, LCD_CLK); outr(PIOB_SODR, LCD_CLK); outr(PIOB_OER, LCD_CLK); }
#define LCD_CLK_CLR() { outr(PIOB_PER, LCD_CLK); outr(PIOB_CODR, LCD_CLK); outr(PIOB_OER, LCD_CLK); }
#elif LCD_CLK_PIO_ID == PIOC_ID
#define LCD_CLK_SET() { outr(PIOC_PER, LCD_CLK); outr(PIOC_SODR, LCD_CLK); outr(PIOC_OER, LCD_CLK); }
#define LCD_CLK_CLR() { outr(PIOC_PER, LCD_CLK); outr(PIOC_CODR, LCD_CLK); outr(PIOC_OER, LCD_CLK); }
#else
#define LCD_CLK_SET() { outr(PIO_PER, LCD_CLK); outr(PIO_SODR, LCD_CLK); outr(PIO_OER, LCD_CLK); }
#define LCD_CLK_CLR() { outr(PIO_PER, LCD_CLK); outr(PIO_CODR, LCD_CLK); outr(PIO_OER, LCD_CLK); }
#endif

#if LCD_MOSI_PIO_ID == PIOA_ID
#define LCD_MOSI_SET() { outr(PIOA_PER, LCD_MOSI); outr(PIOA_SODR, LCD_MOSI); outr(PIOA_OER, LCD_MOSI); }
#define LCD_MOSI_CLR() { outr(PIOA_PER, LCD_MOSI); outr(PIOA_CODR, LCD_MOSI); outr(PIOA_OER, LCD_MOSI); }
#elif LCD_MOSI_PIO_ID == PIOB_ID
#define LCD_MOSI_SET() { outr(PIOB_PER, LCD_MOSI); outr(PIOB_SODR, LCD_MOSI); outr(PIOB_OER, LCD_MOSI); }
#define LCD_MOSI_CLR() { outr(PIOB_PER, LCD_MOSI); outr(PIOB_CODR, LCD_MOSI); outr(PIOB_OER, LCD_MOSI); }
#elif LCD_MOSI_PIO_ID == PIOC_ID
#define LCD_MOSI_SET() { outr(PIOC_PER, LCD_MOSI); outr(PIOC_SODR, LCD_MOSI); outr(PIOC_OER, LCD_MOSI); }
#define LCD_MOSI_CLR() { outr(PIOC_PER, LCD_MOSI); outr(PIOC_CODR, LCD_MOSI); outr(PIOC_OER, LCD_MOSI); }
#else
#define LCD_MOSI_SET() { outr(PIO_PER, LCD_MOSI); outr(PIO_SODR, LCD_MOSI); outr(PIO_OER, LCD_MOSI); }
#define LCD_MOSI_CLR() { outr(PIO_PER, LCD_MOSI); outr(PIO_CODR, LCD_MOSI); outr(PIO_OER, LCD_MOSI); }
#endif

/*!
 * \addtogroup xgST7036
 */
/*@{*/

/*!
 * \brief Wait until controller will be ready again
 *
 * If LCD_WR_BIT is defined we will wait until the ready bit is set, otherwise 
 * We will either busy loop with NutDelay or sleep with NutSleep. The second 
 * option will be used if we have defined NUT_CPU_FREQ. In this case we have a higher 
 * timer resolution.
 *
 * \param xt Delay time in milliseconds
 */
static void LcdDelay(u_int cycles)
{
    while (cycles--) {
        _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP();
    }
}

#if 0
static void INLINE LcdSetBits(u_int mask)
{
    outr(LCD_PIO_SOD_REG, mask);
    outr(LCD_PIO_OE_REG, mask);
}

static void INLINE LcdClrBits(u_int mask)
{
    outr(LCD_PIO_COD_REG, mask);
    outr(LCD_PIO_OE_REG, mask);
}
#endif

static void LcdWaitReady(u_int delay)
{
    while (delay--) {
        _NOP();
    }
}

/*!
 * \brief Send byte to LCD controller.
 *
 * \param data Byte to send.
 */
static void LcdWriteByte(u_int data)
{
    u_char msk = 0x80;

    while (msk) {
        LCD_CLK_CLR();
        if (data & msk) {
            LCD_MOSI_SET();
        } else {
            LCD_MOSI_CLR();
        }
        LCD_CLK_SET();
        msk >>= 1;
        LcdDelay(1);
    }
    LcdWaitReady(LCD_LONG_DELAY);
}

/*!
 * \brief Send command byte to LCD controller.
 *
 * \param cmd Byte to send.
 */
static void LcdWriteCmd(u_char cmd)
{
    /* RS low selects instruction register. */
    LCD_CS_CLR();
    LcdDelay(LCD_SHORT_DELAY);
    LCD_RS_CLR();
    LcdDelay(LCD_SHORT_DELAY);
    LcdWriteByte(cmd);
    LcdDelay(LCD_SHORT_DELAY);
    LCD_RS_SET();
    LcdDelay(LCD_SHORT_DELAY);
    LCD_CS_SET();
    LcdDelay(LCD_SHORT_DELAY);
}

static void LcdWriteInstruction(u_char cmd, u_char xt)
{
    LcdWriteCmd(cmd);
}

/*!
 * \brief Send data byte to LCD controller.
 *
 * \param data Byte to send.
 */
static void LcdWriteData(u_char data)
{
    /* RS high selects data register. */
	LCD_CS_CLR();
    LcdDelay(LCD_SHORT_DELAY);
    LCD_RS_SET();
    LcdDelay(LCD_SHORT_DELAY);
    LcdWriteByte(data);
    LcdDelay(LCD_SHORT_DELAY);
	LCD_CS_SET();
    LcdDelay(LCD_SHORT_DELAY);
}

static void LcdSetCursor(u_char pos)
{
    u_char offset[] = {
        0x00, 0x40, 0x10, 0x50
    };

    pos = offset[(pos / LCD_COLS) % LCD_ROWS] + pos % LCD_COLS;
    LcdWriteCmd(0x80 | pos);
}

static void LcdCursorHome(void)
{
    LcdWriteCmd(0x02);
    LcdDelay(10 * LCD_LONG_DELAY);
}

static void LcdCursorLeft(void)
{
    LcdWriteCmd(0x10);
}

static void LcdCursorRight(void)
{
    LcdWriteCmd(0x14);
}

static void LcdClear(void)
{
    LcdWriteCmd(0x01);
    LcdDelay(10 * LCD_LONG_DELAY);
}

static void LcdCursorMode(u_char on)
{
    if (on) {
        LcdWriteCmd(0x0D);
    } else {
        LcdWriteCmd(0x0C);
    }
    LcdDelay(10 * LCD_LONG_DELAY);
}

static void LcdInit(NUTDEVICE * dev)
{
#if defined(PMC_PCER)
    outr(PMC_PCER, _BV(LCD_CS_PIO_ID) | _BV(LCD_RS_PIO_ID) | _BV(LCD_CLK_PIO_ID) | _BV(LCD_MOSI_PIO_ID));
#endif

    /* Initialize GPIO lines. */
    LCD_CS_SET();
    LCD_RS_SET();
    LCD_CLK_SET();
    LCD_MOSI_SET();

    /* Initial delay. Actually only required after power on. */
    NutSleep(50);

	LCD_RS_CLR();
	LCD_CS_CLR();

	LcdWriteCmd(0x38);    /* Function set. */
	NutSleep(2);
	LcdWriteCmd(0x39);    /* Function set. */
	NutSleep(2);
	LcdWriteCmd(0x14);    /* Bias 1/5, 2 lines. */
	NutSleep(1);
	LcdWriteCmd(0x55);    /* Power/ICON/Contrast control. */
	NutSleep(1);
	LcdWriteCmd(0x6D);    /* Follower control. */
	NutSleep(1);
	LcdWriteCmd(0x78);    /* Booster on, Contrast set. */
	NutSleep(1);
	LcdWriteCmd(0x0F);    /* Display on. */
	NutSleep(1);
	LcdWriteCmd(0x01);    /* Clear display. */
	NutSleep(1);
	LcdWriteCmd(0x06);    /* Entry mode set. */
	NutSleep(1);

	LCD_CS_SET();
	LCD_RS_SET();

    /* Clear display. */
    LcdClear();
    /* Move cursor home. */
    LcdCursorHome();
}

/*!
 * \brief Terminal device control block structure.
 */
static TERMDCB dcb_term = {
    LcdInit,                    /*!< \brief Initialize display subsystem, dss_init. */
    LcdWriteData,               /*!< \brief Write display character, dss_write. */
    LcdWriteInstruction,        /*!< \brief Write display command, dss_command. */
    LcdClear,                   /*!< \brief Clear display, dss_clear. */
    LcdSetCursor,               /*!< \brief Set display cursor, dss_set_cursor. */
    LcdCursorHome,              /*!< \brief Set display cursor home, dss_cursor_home. */
    LcdCursorLeft,              /*!< \brief Move display cursor left, dss_cursor_left. */
    LcdCursorRight,             /*!< \brief Move display cursor right, dss_cursor_right. */
    LcdCursorMode,              /*!< \brief Switch cursor on/off, dss_cursor_mode. */
    0,                          /*!< \brief Mode flags. */
    0,                          /*!< \brief Status flags. */
    LCD_ROWS,                   /*!< \brief Number of rows. */
    LCD_COLS,                   /*!< \brief Number of columns per row. */
    LCD_COLS,                   /*!< \brief Number of visible columns. */
    0,                          /*!< \brief Cursor row. */
    0,                          /*!< \brief Cursor column. */
    0                           /*!< \brief Display shadow memory. */
};

/*!
 * \brief LCD device information structure.
 */
NUTDEVICE devSbiLcd = {
    0,                          /*!< Pointer to next device. */
    {'s', 'b', 'i', 'l', 'c', 'd', 0, 0, 0},  /*!< Unique device name. */
    IFTYP_STREAM,               /*!< Type of device. */
    0,                          /*!< Base address. */
    0,                          /*!< First interrupt number. */
    0,                          /*!< Interface control block. */
    &dcb_term,                  /*!< Driver control block. */
    TermInit,                   /*!< Driver initialization routine. */
    TermIOCtl,                  /*!< Driver specific control function. */
    0,
    TermWrite,
    TermOpen,
    TermClose,
    0
};

/*@}*/
