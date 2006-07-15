/*
 * Copyright (C) 2001-2006 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.4  2006/07/15 11:15:31  haraldkipp
 * Initialization flag removed. It is not required because the driver doesn't
 * poll the busy flag during initialization.
 * Bug fixed, which let the driver fail to properly initialize displays with
 * two lines.
 *
 * Revision 1.3  2006/06/28 17:23:19  haraldkipp
 * Significantly extend delay time to allow running slow 3.3V LCDs with fast
 * CPUs. Not a nice fix, but it works.
 *
 * Revision 1.2  2006/05/15 11:44:06  haraldkipp
 * Added delays for more reliable initialization.
 *
 * Revision 1.1  2006/04/07 13:50:15  haraldkipp
 * ARM driver for HD44780 LCD controller added.
 *
 */

#include <cfg/arch.h>
#include <cfg/arch/gpio.h>
#include <cfg/lcd.h>

#if 0
/* Configuration items. */
#define LCD_DATA_LSB    0
#define LCD_ENABLE_BIT  4
#define LCD_RW_BIT      5
#define LCD_REGSEL_BIT  7

#endif

#include <stdlib.h>
#include <string.h>

#include <sys/nutconfig.h>
#include <dev/hd44780.h>
#include <dev/term.h>
#include <sys/timer.h>

#if !defined(LCD_4x20) && !defined(LCD_4x16)
#if !defined(LCD_2x40) && !defined(LCD_2x20) && !defined(LCD_2x16) && !defined(LCD_2x8)
#if !defined(LCD_1x20) && !defined(LCD_1x16) && !defined(LCD_1x8)
#if !defined(KS0073_CONTROLLER)
#define LCD_2x16
#endif                          /* !KS0073_CONTROLLER */
#endif                          /* !1 line */
#endif                          /* !2 lines */
#endif                          /* !4 lines */

#ifndef LCD_ROWS
#if defined(LCD_4x20) || defined(LCD_4x16) || defined(KS0073_CONTROLLER)
#define LCD_ROWS    4
#elif defined(LCD_1x20) || defined(LCD_1x16) || defined(LCD_1x8)
#define LCD_ROWS    1
#else
#define LCD_ROWS    2
#endif
#endif                          /* LCD_ROWS */

#ifndef LCD_COLS
#if defined(LCD_2x40)
#define LCD_COLS    40
#elif defined(LCD_4x20) || defined(LCD_2x20) || defined(LCD_1x20) || defined(KS0073_CONTROLLER)
#define LCD_COLS    20
#elif defined(LCD_2x8) || defined(LCD_1x8)
#define LCD_COLS    8
#else
#define LCD_COLS    16
#endif
#endif                          /* LCD_COLS */


#ifndef LCD_DATA_LSB
#define LCD_DATA_LSB    0
#endif
#define LCD_DATA    (0xF << LCD_DATA_LSB)

#ifndef LCD_ENABLE_BIT
#define LCD_ENABLE_BIT  4
#endif
#define LCD_EN      _BV(LCD_ENABLE_BIT)

#ifndef LCD_REGSEL_BIT
#define LCD_REGSEL_BIT  7
#endif
#define LCD_RS      _BV(LCD_REGSEL_BIT)

#ifdef LCD_RW_BIT
#define LCD_RW      _BV(LCD_RW_BIT)
#endif

#ifndef LCD_SHORT_DELAY
#define LCD_SHORT_DELAY 10
#endif

#ifndef LCD_LONG_DELAY
#define LCD_LONG_DELAY  1000
#endif

/*!
 * \addtogroup xgDisplay
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

static void INLINE LcdSetBits(u_int mask)
{
    outr(PIO_SODR, mask);
    outr(PIO_OER, mask);
}

static void INLINE LcdClrBits(u_int mask)
{
    outr(PIO_CODR, mask);
    outr(PIO_OER, mask);
}

#ifdef LCD_RW_BIT

static u_int LcdReadNibble(void)
{
    u_int rc;

    LcdSetBits(LCD_EN);
    LcdDelay(LCD_SHORT_DELAY);
    rc = inr(PIO_PDSR) & LCD_DATA;
    LcdClrBits(LCD_EN);

    return rc >> LCD_DATA_LSB;
}

/*!
 * \brief Read byte from LCD controller.
 */
static u_int LcdReadByte(void)
{
    outr(PIO_ODR, LCD_DATA);
    LcdSetBits(LCD_RW);
    return (LcdReadNibble() << 4) | LcdReadNibble();
}

/*!
 * \brief Read status byte from LCD controller.
 */
static u_int LcdReadStatus(void)
{
    /* RS low selects status register. */
    LcdClrBits(LCD_RS);
    return LcdReadByte();
}

#endif                          /* LCD_RW_BIT */

static void LcdWaitReady(u_int delay)
{
    while (delay--) {
#if defined(LCD_RW_BIT)
        if ((LcdReadStatus() & _BV(LCD_BUSY)) == 0) {
            break;
        }
#endif
        _NOP();
    }
}

/*!
 * \brief Send half byte to LCD controller.
 *
 * \param nib The four least significant bits are sent.
 */
static void LcdWriteNibble(u_int nib)
{
    nib <<= LCD_DATA_LSB;
    LcdSetBits(nib & LCD_DATA);
    LcdClrBits(~nib & LCD_DATA);

    LcdSetBits(LCD_EN);
    LcdDelay(LCD_SHORT_DELAY);
    LcdClrBits(LCD_EN);
}

/*!
 * \brief Send byte to LCD controller.
 *
 * \param data Byte to send.
 */
static void LcdWriteByte(u_int data)
{
#ifdef LCD_RW_BIT
    LcdClrBits(LCD_RW);
#endif
    LcdWriteNibble(data >> 4);
    LcdWriteNibble(data);
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
    LcdClrBits(LCD_RS);
    LcdWriteByte(cmd);
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
    LcdSetBits(LCD_RS);
    LcdWriteByte(data);
}

static void LcdSetCursor(u_char pos)
{
    u_char offset[] = {
#ifdef KS0073_CONTROLLER
        0x00, 0x20, 0x40, 0x60
#elif LCD_COLS == 20
        0x00, 0x40, 0x14, 0x54
#else
        0x00, 0x40, 0x10, 0x50
#endif
    };

    pos = offset[(pos / LCD_COLS) % LCD_ROWS] + pos % LCD_COLS;
    LcdWriteCmd(1 << LCD_DDRAM | pos);
}

static void LcdCursorHome(void)
{
    LcdWriteCmd(1 << LCD_HOME);
    LcdDelay(10 * LCD_LONG_DELAY);
}

static void LcdCursorLeft(void)
{
    LcdWriteCmd(1 << LCD_MOVE);
}

static void LcdCursorRight(void)
{
    LcdWriteCmd(1 << LCD_MOVE | 1 << LCD_MOVE_RIGHT);
}

static void LcdClear(void)
{
    LcdWriteCmd(_BV(LCD_CLR));
    LcdDelay(10 * LCD_LONG_DELAY);
}

static void LcdCursorMode(u_char on)
{
    LcdWriteCmd(1 << LCD_ON_CTRL | on ? 1 << LCD_ON_CURSOR : 0x00);
    LcdDelay(10 * LCD_LONG_DELAY);
}

static void LcdInit(NUTDEVICE * dev)
{
    /* Initialize GPIO lines. */
#ifdef LCD_RW_BIT
    outr(PIO_PER, LCD_RW);
    LcdClrBits(LCD_RW);
#endif
    outr(PIO_PER, LCD_EN | LCD_RS | LCD_DATA);
    LcdClrBits(LCD_DATA | LCD_RS);
    LcdDelay(LCD_LONG_DELAY);
    LcdClrBits(LCD_EN);
    LcdDelay(LCD_LONG_DELAY);

    /* Initialize for 4-bit operation. */
    LcdWriteNibble(_BV(LCD_FUNCTION) >> 4);
    NutSleep(1);
    LcdWriteNibble(_BV(LCD_FUNCTION) >> 4);
    NutSleep(1);
    LcdWriteNibble(_BV(LCD_FUNCTION_2LINES));
    NutSleep(1);

    /* Clear display. */
    LcdClear();
    /* Set entry mode. */
    LcdWriteCmd(_BV(LCD_ENTRY_MODE) | _BV(LCD_ENTRY_INC));
    /* Switch display on. */
    LcdWriteCmd(_BV(LCD_ON_CTRL) | _BV(LCD_ON_DISPLAY));
    /* Move cursor home. */
    LcdCursorHome();
    /* Set data address to zero. */
    LcdWriteCmd(_BV(LCD_DDRAM));
}

/*!
 * \brief Terminal device control block structure.
 */
TERMDCB dcb_term = {
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
NUTDEVICE devLcd = {
    0,                          /*!< Pointer to next device. */
    {'l', 'c', 'd', 0, 0, 0, 0, 0, 0},  /*!< Unique device name. */
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
