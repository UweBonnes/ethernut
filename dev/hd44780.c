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

/*
 * $Log$
 * Revision 1.3  2004/03/16 16:48:27  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.2  2003/07/17 09:41:35  haraldkipp
 * Setting the data direction during init only may fail on some hardware.
 * We are now doing this immediately before using the port.
 *
 * Revision 1.1.1.1  2003/05/09 14:40:37  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.3  2003/05/06 18:30:10  harald
 * ICCAVR port
 *
 * Revision 1.2  2003/04/21 16:22:46  harald
 * Moved back to outp/inp for portability
 *
 * Revision 1.1  2003/03/31 14:53:06  harald
 * Prepare release 3.1
 *
 */

#include <stdlib.h>
#include <string.h>

#include <sys/nutconfig.h>
#include <dev/hd44780.h>
#include <dev/term.h>
#include <sys/timer.h>

static u_char long_delay = LCD_LONG_DELAY;
static u_char short_delay = LCD_SHORT_DELAY;

/*!
 * \addtogroup xgDisplay
 */
/*@{*/

static INLINE void LcdSendNibble(u_char nib)
{
    outp(inp(LCD_DATA_DDR) | LCD_DATA_BITS, LCD_DATA_DDR);
    outp((inp(LCD_DATA_PORT) & ~LCD_DATA_BITS) | (nib & LCD_DATA_BITS), LCD_DATA_PORT);
    sbi(LCD_ENABLE_PORT, LCD_ENABLE_BIT);
    _NOP(); _NOP();
    cbi(LCD_ENABLE_PORT, LCD_ENABLE_BIT); 
}

/*!
 * \brief Send byte to LCD controller.
 *
 * The byte is sent to a 4-bit interface in two nibbles.
 *
 * \param ch Byte to send.
 * \param xt Delay time in milliseconds.
 */
static INLINE void LcdSendByte(u_char ch, u_char xt)
{
#if LCD_DATA_BITS == 0x0F
    LcdSendNibble(ch >> 4);
    if(xt)
        NutDelay(xt);
    LcdSendNibble(ch);
#elif LCD_DATA_BITS == 0xF0
    LcdSendNibble(ch);
    if(xt)
        NutDelay(xt);
    LcdSendNibble(ch << 4);
#else
#error "Bad definition of LCD_DATA_BITS"
#endif
    if(xt)
        NutDelay(xt);
}

static void LcdWriteData(u_char ch)
{
    sbi(LCD_REGSEL_DDR, LCD_REGSEL_BIT);
    sbi(LCD_REGSEL_PORT, LCD_REGSEL_BIT);
    LcdSendByte(ch, short_delay);
}

/*!
 * \brief Write command byte to LCD controller.
 */
static void LcdWriteCmd(u_char cmd, u_char xt)
{
    sbi(LCD_REGSEL_DDR, LCD_REGSEL_BIT);
    cbi(LCD_REGSEL_PORT, LCD_REGSEL_BIT);
    LcdSendByte(cmd, xt);
}


static void LcdSetCursor(u_char pos)
{
    LcdWriteCmd(0x80 + pos, long_delay);
}

static void LcdCursorHome(void)
{
    LcdWriteCmd(0x02, long_delay);
}

static void LcdCursorLeft(void)
{
    LcdWriteCmd(0x10, short_delay);
}

static void LcdCursorRight(void)
{
    LcdWriteCmd(0x14, short_delay);
}

static void LcdClear(void)
{
    LcdWriteCmd(0x01, long_delay);
}

static void LcdCursorMode(u_char on)
{
    LcdWriteCmd(on ? 0x0F : 0x0C, long_delay);
}

static void LcdInit(void)
{
    /*
     * Set LCD register select and enable outputs.
     */
    sbi(LCD_REGSEL_DDR, LCD_REGSEL_BIT);
    sbi(LCD_ENABLE_DDR, LCD_ENABLE_BIT);

    /*
     * Send a dummy data byte.
     */
    LcdWriteData(0);

    /*
     * Initialize for 4-bit operation.
     */
    cbi(LCD_REGSEL_PORT, LCD_REGSEL_BIT);
    LcdSendNibble(0x33);
    NutDelay(50);
    LcdSendNibble(0x33);
    NutDelay(50);
    LcdSendNibble(0x33);
    NutDelay(50);
    LcdSendNibble(0x22);
    NutDelay(50);
    LcdWriteData(0);
    NutDelay(50);

    /*
     * Clear display.
     */
    LcdWriteCmd(0x07, 2 * long_delay);

    /*
     * Disable display shift and set incremental mode.
     */
    LcdWriteCmd(0x06,  2 * long_delay);

    /*
     * Enable the display and a blinking cursor.
     */
    LcdWriteCmd(0x0F,  2 * long_delay);
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
    2,                  /*!< \brief Number of rows. */
    64,                 /*!< \brief Number of columns per row. */
    16,                 /*!< \brief Number of visible columns. */
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
