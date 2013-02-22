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

/*!
 * \file arch/avr/dev/hd44780_bus.c
 * \brief Terminal device definitions for port mapped LCD.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <stdlib.h>
#include <string.h>

#include <cfg/arch/avr.h>
#include <dev/hd44780.h>
#include <dev/term.h>
#include <sys/timer.h>

/* Backward compatibility with old macros */
#ifndef LCD_ROWS
#if defined(LCD_4x20) || defined(LCD_4x16) || defined(KS0073_CONTROLLER)
#define LCD_ROWS    4
#elif defined(LCD_1x20) || defined(LCD_1x16) || defined(LCD_1x8)
#define LCD_ROWS    1
#else
#define LCD_ROWS    2
#endif
#endif                          /* LCD_ROWS */

/* Backward compatibility with old macros */
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

/*
 * Many thanks to Thiago Correa for adding LCD port configuration.
 */

/* LCD_DATA_PORT and LCD_DATA_DDR switches */
#if ( LCD_DATA_AVRPORT == AVRPORTA )
#define LCD_DATA_PORT PORTA
#define LCD_DATA_PIN PINA
#define LCD_DATA_DDR DDRA

#elif ( LCD_DATA_AVRPORT == AVRPORTB )
#define LCD_DATA_PORT PORTB
#define LCD_DATA_PIN PINB
#define LCD_DATA_DDR DDRB

#elif ( LCD_DATA_AVRPORT == AVRPORTC )
#define LCD_DATA_PORT PORTC
#define LCD_DATA_PIN PINC
#define LCD_DATA_DDR DDRC

#elif ( LCD_DATA_AVRPORT == AVRPORTE )
#define LCD_DATA_PORT PORTE
#define LCD_DATA_PIN PINE
#define LCD_DATA_DDR DDRE

#elif ( LCD_DATA_AVRPORT == AVRPORTF )
#define LCD_DATA_PORT PORTF
#define LCD_DATA_PIN PINF
#define LCD_DATA_DDR DDRF

#elif ( LCD_DATA_AVRPORT == AVRPORTG )
#define LCD_DATA_PORT PORTG
#define LCD_DATA_PIN PING
#define LCD_DATA_DDR DDRG

#else
#define LCD_DATA_PORT PORTD
#define LCD_DATA_PIN PIND
#define LCD_DATA_DDR DDRD

#endif

#ifndef LCD_DATA_BITS
#define LCD_DATA_BITS   0xF0
#endif

/* LCD_ENABLE_PORT switches */
#if ( LCD_ENABLE_AVRPORT == AVRPORTA )
#define LCD_ENABLE_PORT PORTA
#define LCD_ENABLE_DDR DDRA

#elif ( LCD_ENABLE_AVRPORT == AVRPORTB )
#define LCD_ENABLE_PORT PORTB
#define LCD_ENABLE_DDR DDRB

#elif ( LCD_ENABLE_AVRPORT == AVRPORTC )
#define LCD_ENABLE_PORT PORTC
#define LCD_ENABLE_DDR DDRC

#elif ( LCD_ENABLE_AVRPORT == AVRPORTD )
#define LCD_ENABLE_PORT PORTD
#define LCD_ENABLE_DDR DDRD

#elif ( LCD_ENABLE_AVRPORT == AVRPORTF )
#define LCD_ENABLE_PORT PORTF
#define LCD_ENABLE_DDR DDRF

#elif ( LCD_ENABLE_AVRPORT == AVRPORTG )
#define LCD_ENABLE_PORT PORTG
#define LCD_ENABLE_DDR DDRG

#else
#define LCD_ENABLE_PORT PORTE
#define LCD_ENABLE_DDR DDRE

#endif

#ifndef LCD_ENABLE_BIT
#define LCD_ENABLE_BIT  3       /*!< \brief LCD enable output. */
#endif

/* LCD_REGSEL_PORT switches */
#if ( LCD_REGSEL_AVRPORT == AVRPORTA )
#define LCD_REGSEL_PORT PORTA
#define LCD_REGSEL_DDR DDRA

#elif ( LCD_REGSEL_AVRPORT == AVRPORTB )
#define LCD_REGSEL_PORT PORTB
#define LCD_REGSEL_DDR DDRB

#elif ( LCD_REGSEL_AVRPORT == AVRPORTC )
#define LCD_REGSEL_PORT PORTC
#define LCD_REGSEL_DDR DDRC

#elif ( LCD_REGSEL_AVRPORT == AVRPORTD )
#define LCD_REGSEL_PORT PORTD
#define LCD_REGSEL_DDR DDRD

#elif ( LCD_REGSEL_AVRPORT == AVRPORTF )
#define LCD_REGSEL_PORT PORTF
#define LCD_REGSEL_DDR DDRF

#elif ( LCD_REGSEL_AVRPORT == AVRPORTG )
#define LCD_REGSEL_PORT PORTG
#define LCD_REGSEL_DDR DDRG

#else
#define LCD_REGSEL_PORT PORTE
#define LCD_REGSEL_DDR DDRE

#endif

#ifndef LCD_REGSEL_BIT
#define LCD_REGSEL_BIT  2       /*!< \brief LCD register select output. */
#endif

/* LCD_RW_PORT switches */
#if ( LCD_RW_AVRPORT == AVRPORTA )
#define LCD_RW_PORT PORTA
#define LCD_RW_DDR DDRA

#elif ( LCD_RW_AVRPORT == AVRPORTB )
#define LCD_RW_PORT PORTB
#define LCD_RW_DDR DDRB

#elif ( LCD_RW_AVRPORT == AVRPORTC )
#define LCD_RW_PORT PORTC
#define LCD_RW_DDR DDRC

#elif ( LCD_RW_AVRPORT == AVRPORTD )
#define LCD_RW_PORT PORTD
#define LCD_RW_DDR DDRD

#elif ( LCD_RW_AVRPORT == AVRPORTE )
#define LCD_RW_PORT PORTE
#define LCD_RW_DDR DDRE

#elif ( LCD_RW_AVRPORT == AVRPORTF )
#define LCD_RW_PORT PORTF
#define LCD_RW_DDR DDRF

#elif ( LCD_RW_AVRPORT == AVRPORTG )
#define LCD_RW_PORT PORTG
#define LCD_RW_DDR DDRG
#endif



/*!
 * \addtogroup xgDisplay
 */
/*@{*/

#ifndef LCD_SHORT_DELAY
#define LCD_SHORT_DELAY 1
#endif

#ifndef LCD_LONG_DELAY
#define LCD_LONG_DELAY  2
#endif

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

static uint8_t during_init = 1;
#define LCD_DELAY _NOP(); _NOP(); _NOP(); _NOP() /*Three Nops would fit too */

#ifdef LCD_RW_BIT

static INLINE uint8_t LcdReadNibble(void)
{

    uint8_t ret;
    sbi(LCD_RW_PORT, LCD_RW_BIT);
    outb(LCD_DATA_DDR, inb(LCD_DATA_DDR) & ~LCD_DATA_BITS);   // enable data input
    sbi(LCD_ENABLE_PORT, LCD_ENABLE_BIT);
    LCD_DELAY;
    ret = inb(LCD_DATA_PIN) & LCD_DATA_BITS;
    cbi(LCD_ENABLE_PORT, LCD_ENABLE_BIT);
    LCD_DELAY;
    return ret;
}

static INLINE uint8_t LcdReadByte(void)
{
    uint8_t data;
#if LCD_DATA_BITS == 0x0F
    data = LcdReadNibble();
    data = data | (LcdReadNibble() << 4);
#elif LCD_DATA_BITS == 0xF0
    data = LcdReadNibble() >> 4;
    data |= LcdReadNibble();
#elif LCD_DATA_BITS == 0xFF
    data = LcdReadNibble();
#else
#error "Bad definition of LCD_DATA_BITS"
#endif
    return data;
}

/*!
 * \brief Read command byte from LCD controller.
 */

static uint8_t LcdReadCmd(void)
{
    sbi(LCD_REGSEL_DDR, LCD_REGSEL_BIT);
    cbi(LCD_REGSEL_PORT, LCD_REGSEL_BIT);
    return LcdReadByte();
}

#endif


static void LcdDelay(uint8_t xt)
{
    if (during_init) {
        NutDelay(xt);
    } else {
#if defined(LCD_RW_BIT)
    while (LcdReadCmd() & (1 << LCD_BUSY))
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
#elif defined(NUT_CPU_FREQ)
    NutSleep(xt);
#else
    NutDelay(xt);
#endif
    }
}

static INLINE void LcdSendNibble(uint8_t nib)
{
#ifdef LCD_RW_BIT
    cbi(LCD_RW_PORT, LCD_RW_BIT);
#endif
    outb(LCD_DATA_DDR, inb(LCD_DATA_DDR) | LCD_DATA_BITS);
    outb(LCD_DATA_PORT, (inb(LCD_DATA_PORT) & ~LCD_DATA_BITS) | (nib & LCD_DATA_BITS));
    sbi(LCD_ENABLE_PORT, LCD_ENABLE_BIT);
    LCD_DELAY;
    cbi(LCD_ENABLE_PORT, LCD_ENABLE_BIT);
    LCD_DELAY;
}

/*!
 * \brief Send byte to LCD controller.
 *
 * The byte is sent to a 4-bit interface in two nibbles. If one has configured
 * LCD_DATA_BITS to 0xFF this will send a whole byte at once
 *
 * \param ch Byte to send.
 * \param xt Delay time in milliseconds.
 */
static INLINE void LcdSendByte(uint8_t ch, uint8_t xt)
{
#if LCD_DATA_BITS == 0x0F
    LcdSendNibble(ch >> 4);
    if(xt)
        LcdDelay(xt);
    LcdSendNibble(ch);
#elif LCD_DATA_BITS == 0xF0
    LcdSendNibble(ch);
    if(xt)
        LcdDelay(xt);
    LcdSendNibble(ch << 4);
#elif LCD_DATA_BITS == 0xFF
    LcdSendNibble(ch);
#else
#error "Bad definition of LCD_DATA_BITS"
#endif
    if(xt)
        LcdDelay(xt);
}

static void LcdWriteData(uint8_t ch)
{
    sbi(LCD_REGSEL_DDR, LCD_REGSEL_BIT);
    sbi(LCD_REGSEL_PORT, LCD_REGSEL_BIT);
    LcdSendByte(ch, LCD_SHORT_DELAY);
}

/*!
 * \brief Write command byte to LCD controller.
 */
static void LcdWriteCmd(uint8_t cmd, uint8_t xt)
{
    sbi(LCD_REGSEL_DDR, LCD_REGSEL_BIT);
    cbi(LCD_REGSEL_PORT, LCD_REGSEL_BIT);
    LcdSendByte(cmd, xt);
}

static void LcdSetCursor(uint8_t pos)
{
    uint8_t offset[] = {
#ifdef KS0073_CONTROLLER
        0x00, 0x20, 0x40, 0x60
#elif LCD_COLS == 20
        0x00, 0x40, 0x14, 0x54
#else
        0x00, 0x40, 0x10, 0x50
#endif
    };

    pos = offset[(pos / LCD_COLS) % LCD_ROWS] + pos % LCD_COLS;
    LcdWriteCmd(1 << LCD_DDRAM | pos, LCD_SHORT_DELAY);
}

static void LcdCursorHome(void)
{
    LcdWriteCmd(1 << LCD_HOME, LCD_LONG_DELAY);
}

static void LcdCursorLeft(void)
{
    LcdWriteCmd(1 << LCD_MOVE, LCD_SHORT_DELAY);
}

static void LcdCursorRight(void)
{
    LcdWriteCmd(1 << LCD_MOVE | 1 << LCD_MOVE_RIGHT, LCD_SHORT_DELAY);
}

static void LcdClear(void)
{
    LcdWriteCmd(1 << LCD_CLR, LCD_LONG_DELAY);
}

static void LcdCursorMode(uint8_t on)
{
    LcdWriteCmd(1 << LCD_ON_CTRL | on ? 1 << LCD_ON_CURSOR : 0x00, LCD_LONG_DELAY);
}

static int LcdInit(NUTDEVICE *dev)
{
    /*
     * Set LCD register select and enable outputs.
     */
    sbi(LCD_REGSEL_DDR, LCD_REGSEL_BIT);
    sbi(LCD_ENABLE_DDR, LCD_ENABLE_BIT);
#ifdef LCD_RW_BIT
    sbi(LCD_RW_DDR, LCD_RW_BIT);
    cbi(LCD_RW_PORT, LCD_RW_BIT);
#endif


    /*
     * Send a dummy data byte.
     */
    //LcdWriteData(0);

    /*
     * Initialize for 4-bit operation.
     */
    cbi(LCD_REGSEL_PORT, LCD_REGSEL_BIT);

#if (LCD_DATA_BITS == 0xFF)     // 8 Bit initialisation
    LcdWriteCmd((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT), 50);
    LcdWriteCmd((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT), 50);
    LcdWriteCmd((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT), 50);
    #ifdef  KS0073_CONTROLLER
        LcdWriteCmd((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT) | (1 << LCD_FUNCTION_RE), LCD_SHORT_DELAY);
        LcdWriteCmd((1 << LCD_EXT) | ((((TERMDCB *) dev->dev_dcb)->dcb_nrows > 2) ? (1 << LCD_EXT_4LINES) : 0), LCD_SHORT_DELAY);
        LcdWriteCmd((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT), LCD_SHORT_DELAY);
    #endif
    LcdWriteCmd((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT) | ((((TERMDCB *) dev->dev_dcb)->dcb_nrows > 1) ?(1 << LCD_FUNCTION_2LINES):0), LCD_SHORT_DELAY);


#else                           // 4 Bit initialisation
    LcdSendNibble((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT) | (((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT)) >> 4));
    LcdDelay(50);
    LcdSendNibble((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT) | (((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT)) >> 4));
    LcdDelay(50);
    LcdSendNibble((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT) | (((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT)) >> 4));
    LcdDelay(50);
    LcdSendNibble((1 << LCD_FUNCTION) | ((1 << LCD_FUNCTION) >> 4));    // Enter 4 Bit mode
    LcdDelay(50);
    #ifdef  KS0073_CONTROLLER
        LcdWriteCmd((1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_RE), LCD_SHORT_DELAY);
        LcdWriteCmd((1 << LCD_EXT) | ((((TERMDCB *) dev->dev_dcb)->dcb_nrows > 2) ? (1 << LCD_EXT_4LINES) : 0), LCD_LONG_DELAY);
        LcdWriteCmd((1 << LCD_FUNCTION), LCD_LONG_DELAY);
    #endif
    LcdWriteCmd((1 << LCD_FUNCTION) | ((((TERMDCB *) dev->dev_dcb)->dcb_nrows > 1) ? (1 << LCD_FUNCTION_2LINES):0), LCD_SHORT_DELAY);
#endif

    // clear LCD
    LcdWriteCmd(1 << LCD_CLR, LCD_LONG_DELAY);
    // set entry mode
    LcdWriteCmd(1 << LCD_ENTRY_MODE | 1 << LCD_ENTRY_INC, LCD_LONG_DELAY);
    // set display to on
    LcdWriteCmd(1 << LCD_ON_CTRL | 1 << LCD_ON_DISPLAY, LCD_LONG_DELAY);
    // move cursor to home
    LcdWriteCmd(1 << LCD_HOME, LCD_LONG_DELAY);
    // set data address to 0
    LcdWriteCmd(1 << LCD_DDRAM | 0x00, LCD_LONG_DELAY);
    during_init = 0;

    return 0;
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
    LCD_ROWS,           /*!< \brief Number of rows. */
    LCD_COLS,           /*!< \brief Number of columns per row. */
    LCD_COLS,           /*!< \brief Number of visible columns. */
    0,                  /*!< \brief Cursor row. */
    0,                  /*!< \brief Cursor column. */
    0                   /*!< \brief Display shadow memory. */
};

/*!
 * \brief LCD device information structure.
 */
NUTDEVICE devLcd = {
    0,              /*!< Pointer to next device. */
    {'l', 'c', 'd', 0, 0, 0, 0, 0, 0},      /*!< Unique device name. */
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
