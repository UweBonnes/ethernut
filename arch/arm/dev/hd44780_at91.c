/*
 * Copyright (C) 2001-2007 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.11  2008/08/11 06:59:09  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.10  2008/08/06 12:51:00  haraldkipp
 * Added support for Ethernut 5 (AT91SAM9XE reference design).
 *
 * Revision 1.9  2008/02/15 16:58:41  haraldkipp
 * Spport for AT91SAM7SE512 added.
 *
 * Revision 1.8  2007/10/04 19:57:54  olereinhardt
 * Support for SAM7S256 added
 *
 * Revision 1.7  2007/02/15 16:05:29  haraldkipp
 * Port usage is now configurable. Data bits no longer need four consecutive
 * port bits. Added delays in read for better reliability with some slow
 * displays.
 *
 * Revision 1.6  2006/10/05 17:11:16  haraldkipp
 * Fixes bug #1567813. Should now work after power on and after reset without
 * power loss. Many thanks to Klaus-Dieter Sohn.
 *
 * Revision 1.5  2006/08/31 19:02:25  haraldkipp
 * Added support for AT91SAM9260.
 * Some displays fail after reset. An additional nibble sent
 * during 4-bit initialization seems to fix this. However,
 * a user reported that his 3.3V driven LCD now fails during
 * power on.
 *
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

/*!
 * \brief GPIO controller ID.
 */
#if !defined(LCD_PIO_ID)
#if defined(MCU_AT91SAM7X256) || defined (MCU_AT91SAM7S256) || defined (MCU_AT91SAM7SE512)
#define LCD_PIO_ID  PIOA_ID
#elif defined(MCU_AT91SAM9260) || defined(MCU_AT91SAM9XE512)
#define LCD_PIO_ID  PIOB_ID
#else
#define LCD_PIO_ID  PIO_ID
#endif
#endif

/*!
 * \brief LCD GPIO enable register.
 */
#if !defined(LCD_PIO_PE_REG)
#if LCD_PIO_ID == PIOA_ID
#define LCD_PIO_PE_REG  PIOA_PER
#elif LCD_PIO_ID == PIOB_ID
#define LCD_PIO_PE_REG  PIOB_PER
#elif LCD_PIO_ID == PIOC_ID
#define LCD_PIO_PE_REG  PIOC_PER
#else
#define LCD_PIO_PE_REG  PIO_PER
#endif
#endif

/*!
 * \brief LCD GPIO output enable register.
 */
#if !defined(LCD_PIO_OE_REG)
#if LCD_PIO_ID == PIOA_ID
#define LCD_PIO_OE_REG  PIOA_OER
#elif LCD_PIO_ID == PIOB_ID
#define LCD_PIO_OE_REG  PIOB_OER
#elif LCD_PIO_ID == PIOC_ID
#define LCD_PIO_OE_REG  PIOC_OER
#else
#define LCD_PIO_OE_REG  PIO_OER
#endif
#endif

/*!
 * \brief LCD GPIO output disable register.
 */
#ifdef LCD_RW_BIT
#if !defined(LCD_PIO_OD_REG)
#if LCD_PIO_ID == PIOA_ID
#define LCD_PIO_OD_REG  PIOA_ODR
#elif LCD_PIO_ID == PIOB_ID
#define LCD_PIO_OD_REG  PIOB_ODR
#elif LCD_PIO_ID == PIOC_ID
#define LCD_PIO_OD_REG  PIOC_ODR
#else
#define LCD_PIO_OD_REG  PIO_ODR
#endif
#endif
#endif /* LCD_RW_BIT */

/*!
 * \brief LCD GPIO set output data register.
 */
#if !defined(LCD_PIO_SOD_REG)
#if LCD_PIO_ID == PIOA_ID
#define LCD_PIO_SOD_REG PIOA_SODR
#elif LCD_PIO_ID == PIOB_ID
#define LCD_PIO_SOD_REG PIOB_SODR
#elif LCD_PIO_ID == PIOC_ID
#define LCD_PIO_SOD_REG PIOC_SODR
#else
#define LCD_PIO_SOD_REG PIO_SODR
#endif
#endif

/*!
 * \brief LCD GPIO clear output data register.
 */
#if !defined(LCD_PIO_COD_REG)
#if LCD_PIO_ID == PIOA_ID
#define LCD_PIO_COD_REG PIOA_CODR
#elif LCD_PIO_ID == PIOB_ID
#define LCD_PIO_COD_REG PIOB_CODR
#elif LCD_PIO_ID == PIOC_ID
#define LCD_PIO_COD_REG PIOC_CODR
#else
#define LCD_PIO_COD_REG PIO_CODR
#endif
#endif

/*!
 * \brief LCD GPIO pin data status register.
 */
#ifdef LCD_RW_BIT
#if !defined(LCD_PIO_PDS_REG)
#if LCD_PIO_ID == PIOA_ID
#define LCD_PIO_PDS_REG PIOA_PDSR
#elif LCD_PIO_ID == PIOB_ID
#define LCD_PIO_PDS_REG PIOB_PDSR
#elif LCD_PIO_ID == PIOC_ID
#define LCD_PIO_PDS_REG PIOC_PDSR
#else
#define LCD_PIO_PDS_REG PIO_PDSR
#endif
#endif
#endif /* LCD_RW_BIT */

#if !defined(LCD_DATA_LSB) && !defined(LCD_DATA_BIT0)
#define LCD_DATA_LSB    0
#endif

#ifdef LCD_DATA_LSB
#define LCD_DATA    (0xF << LCD_DATA_LSB)
#else
#define LCD_D0      _BV(LCD_DATA_BIT0)
#define LCD_D1      _BV(LCD_DATA_BIT1)
#define LCD_D2      _BV(LCD_DATA_BIT2)
#define LCD_D3      _BV(LCD_DATA_BIT3)
#define LCD_DATA    (LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3)
#endif

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
    outr(LCD_PIO_SOD_REG, mask);
    outr(LCD_PIO_OE_REG, mask);
}

static void INLINE LcdClrBits(u_int mask)
{
    outr(LCD_PIO_COD_REG, mask);
    outr(LCD_PIO_OE_REG, mask);
}

#ifdef LCD_RW_BIT

static u_int LcdReadNibble(void)
{
    u_int rc;

    LcdSetBits(LCD_EN);
    LcdDelay(LCD_SHORT_DELAY);
    rc = inr(LCD_PIO_PDS_REG) & LCD_DATA;
    LcdClrBits(LCD_EN);
    LcdDelay(LCD_SHORT_DELAY);

#ifdef LCD_DATA_LSB
    rc >>= LCD_DATA_LSB
#else
    {
        u_int val = 0;

        if (rc & LCD_D0) {
            val |= 0x01;
        }
        if (rc & LCD_D1) {
            val |= 0x02;
        }
        if (rc & LCD_D2) {
            val |= 0x04;
        }
        if (rc & LCD_D3) {
            val |= 0x08;
        }
        rc = val;
    }
#endif
    return rc;
}

/*!
 * \brief Read byte from LCD controller.
 */
static u_int LcdReadByte(void)
{
    outr(LCD_PIO_OD_REG, LCD_DATA);
    LcdDelay(LCD_SHORT_DELAY);
    LcdSetBits(LCD_RW);
    LcdDelay(LCD_SHORT_DELAY);
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
#ifdef LCD_DATA_LSB
    nib <<= LCD_DATA_LSB;
#else
    {
        u_int val = 0;
        if (nib & 0x01) {
            val |= LCD_D0;
        }
        if (nib & 0x02) {
            val |= LCD_D1;
        }
        if (nib & 0x04) {
            val |= LCD_D2;
        }
        if (nib & 0x08) {
            val |= LCD_D3;
        }
        nib = val;
    }
#endif
    LcdSetBits(nib & LCD_DATA);
    LcdClrBits(~nib & LCD_DATA);

    LcdDelay(LCD_SHORT_DELAY);
    LcdSetBits(LCD_EN);
    LcdDelay(LCD_SHORT_DELAY);
    LcdClrBits(LCD_EN);
    LcdDelay(LCD_SHORT_DELAY);
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
static void LcdWriteCmd(uint8_t cmd)
{
    /* RS low selects instruction register. */
    LcdClrBits(LCD_RS);
    LcdWriteByte(cmd);
}

static void LcdWriteInstruction(uint8_t cmd, uint8_t xt)
{
    LcdWriteCmd(cmd);
}

/*!
 * \brief Send data byte to LCD controller.
 *
 * \param data Byte to send.
 */
static void LcdWriteData(uint8_t data)
{
    /* RS high selects data register. */
    LcdSetBits(LCD_RS);
    LcdWriteByte(data);
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

static void LcdCursorMode(uint8_t on)
{
    LcdWriteCmd(1 << LCD_ON_CTRL | on ? 1 << LCD_ON_CURSOR : 0x00);
    LcdDelay(10 * LCD_LONG_DELAY);
}

static void LcdInit(NUTDEVICE * dev)
{
#if defined(PMC_PCER)
    outr(PMC_PCER, _BV(LCD_PIO_ID));
#endif

    /* Initialize GPIO lines. */
#ifdef LCD_RW_BIT
    outr(LCD_PIO_PE_REG, LCD_RW);
    LcdClrBits(LCD_RW);
#endif
    outr(LCD_PIO_PE_REG, LCD_EN | LCD_RS | LCD_DATA);
    LcdClrBits(LCD_DATA | LCD_RS);
    LcdDelay(LCD_LONG_DELAY);
    LcdClrBits(LCD_EN);
    LcdDelay(LCD_LONG_DELAY);

    /* Initial delay. Actually only required after power on. */
    NutSleep(16);

    /* This initialization will make sure, that the LCD is switched
       to 4-bit mode, no matter which mode we start from. */
    LcdWriteNibble((_BV(LCD_FUNCTION) | _BV(LCD_FUNCTION_8BIT)) >> 4);
    NutSleep(5);
    LcdWriteNibble((_BV(LCD_FUNCTION) | _BV(LCD_FUNCTION_8BIT)) >> 4);
    NutSleep(2);
    LcdWriteNibble((_BV(LCD_FUNCTION) | _BV(LCD_FUNCTION_8BIT)) >> 4);
    NutSleep(2);
    LcdWriteNibble(_BV(LCD_FUNCTION) >> 4);
    NutSleep(2);

    /* Set number of lines and font. Can't be changed later. */
    LcdWriteNibble((_BV(LCD_FUNCTION) | _BV(LCD_FUNCTION_2LINES)) >> 4);
    LcdWriteNibble(_BV(LCD_FUNCTION) | _BV(LCD_FUNCTION_2LINES));
    NutSleep(2);

    /* Switch display and cursor off. */
    LcdWriteNibble(_BV(LCD_ON_CTRL) >> 4);
    LcdWriteNibble(_BV(LCD_ON_CTRL));
    NutSleep(2);

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
