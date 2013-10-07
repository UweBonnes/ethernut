/*
 * Copyright (C) 2001-2007 by egnite Software GmbH. 
 * Copyright (C) 2013 by Comm5 Tecnologia Ltda.
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
 */

/*
 * $Log$
 * Port done by Walter Orosco
 */

#include <arch/avr32.h>
#include <avr32/io.h>
 
#include <cfg/arch.h>
#include <cfg/lcd.h>

#include <dev/hd44780.h>
#include <dev/term.h>
#include <dev/gpio.h>
#include <dev/board.h>
#include <dev/watchdog.h>

#include <sys/thread.h>
#include <sys/nutconfig.h>
#include <sys/timer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LCD_ROWS
#define LCD_ROWS    2
#endif

#ifndef LCD_COLS
#define LCD_COLS    16
#endif

#ifndef LCD_E2E_DLY
#define LCD_E2E_DLY 80
#endif

#ifndef LCD_LONG_DELAY
#define LCD_LONG_DELAY  1000
#endif

#ifndef LCD_DATA_BIT0
#define LCD_DATA_BIT0 1
#define LCD_DATA_BIT1 1
#define LCD_DATA_BIT2 1
#define LCD_DATA_BIT3 1
#endif

#ifndef LCD_PW_EH
#define LCD_PW_EH 500
#define LcdNanoDelay(x) NutMicroDelay(x/10);
#else
#define LcdNanoDelay(x)
#endif


#if !defined(LCD_IF_8BIT) && !defined(LCD_IF_4BIT)
#define LCD_IF_4BIT
#endif

#ifdef LCD_IF_4BIT
#ifdef LCD_DATA_LSB
#define LCD_DATA    (0xF << LCD_DATA_LSB)
#else   /* LCD_DATA_LSB */
#define LCD_D0      _BV(LCD_DATA_BIT0)
#define LCD_D1      _BV(LCD_DATA_BIT1)
#define LCD_D2      _BV(LCD_DATA_BIT2)
#define LCD_D3      _BV(LCD_DATA_BIT3)
#define LCD_DATA    (LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3)
#endif  /* LCD_DATA_LSB */
#else   /* LCD_IF_4BIT */
#define LCD_DATA     (0xFF << LCD_DATA_LSB)
#endif  /* LCD_IF_4BIT */

#ifndef LCD_SHORT_DELAY
#define LCD_SHORT_DELAY 100
#endif

#ifndef LCD_LONG_DELAY
#define LCD_LONG_DELAY  200
#endif

#define LCD_E2E_DLY 80

#ifdef LCD_EN_BIT
#define LCD_EN      _BV(LCD_EN_BIT)
#if LCD_EN_PIO_ID == PIOA_ID
#define LCD_EN_SET() { outr(PIO_PER, LCD_EN ); outr(PIO_SODR, LCD_EN ); outr(PIO_OER, LCD_EN ); }
#define LCD_EN_CLR() { outr(PIO_PER, LCD_EN ); outr(PIO_CODR, LCD_EN ); outr(PIO_OER, LCD_EN ); }
#elif LCD_EN_PIO_ID == PIOB_ID
#define LCD_EN_SET() { outr(PIO_PER, LCD_EN ); outr(PIO_SODR, LCD_EN ); outr(PIO_OER, LCD_EN ); }
#define LCD_EN_CLR() { outr(PIO_PER, LCD_EN ); outr(PIO_CODR, LCD_EN ); outr(PIO_OER, LCD_EN ); }
#elif LCD_EN_PIO_ID == PIOC_ID
#define LCD_EN_SET() { outr(PIO_PER, LCD_EN ); outr(PIO_SODR, LCD_EN ); outr(PIO_OER, LCD_EN ); }
#define LCD_EN_CLR() { outr(PIO_PER, LCD_EN ); outr(PIO_CODR, LCD_EN ); outr(PIO_OER, LCD_EN ); }
#else
#define LCD_EN_SET() { outr(PIO_PER, LCD_EN); outr(PIO_SODR, LCD_EN); outr(PIO_OER, LCD_EN); }
#define LCD_EN_CLR() { outr(PIO_PER, LCD_EN); outr(PIO_CODR, LCD_EN); outr(PIO_OER, LCD_EN); }
#endif /* LCD_EN_PIO_ID */
#else /* LCD_EN_BIT */
#define LCD_EN_SET()
#define LCD_EN_CLR()
#endif /* LCD_EN_BIT */

#ifdef LCD_RS_BIT
#define LCD_RS      _BV(LCD_RS_BIT)
#if LCD_RS_PIO_ID == PIOA_ID
#define LCD_RS_SET() { outr(PIO_PER, LCD_RS); outr(PIO_SODR, LCD_RS); outr(PIO_OER, LCD_RS); }
#define LCD_RS_CLR() { outr(PIO_PER, LCD_RS); outr(PIO_CODR, LCD_RS); outr(PIO_OER, LCD_RS); }
#elif LCD_RS_PIO_ID == PIOB_ID
#define LCD_RS_SET() { outr(PIO_PER, LCD_RS); outr(PIO_SODR, LCD_RS); outr(PIO_OER, LCD_RS); }
#define LCD_RS_CLR() { outr(PIO_PER, LCD_RS); outr(PIO_CODR, LCD_RS); outr(PIO_OER, LCD_RS); }
#elif LCD_RS_PIO_ID == PIOC_ID
#define LCD_RS_SET() { outr(PIO_PER, LCD_RS); outr(PIO_SODR, LCD_RS); outr(PIO_OER, LCD_RS); }
#define LCD_RS_CLR() { outr(PIO_PER, LCD_RS); outr(PIO_CODR, LCD_RS); outr(PIO_OER, LCD_RS); }
#else
#define LCD_RS_SET() { outr(PIO_PER, LCD_RS); outr(PIO_SODR, LCD_RS); outr(PIO_OER, LCD_RS); }
#define LCD_RS_CLR() { outr(PIO_PER, LCD_RS); outr(PIO_CODR, LCD_RS); outr(PIO_OER, LCD_RS); }
#endif /* LCD_RS_PIO_ID */
#else /* LCD_RS_BIT */
#define LCD_RS_SET()
#define LCD_RS_CLR()
#endif /* LCD_RS_BIT */

#ifdef LCD_RW_BIT
#define LCD_RW      _BV(LCD_RW_BIT)
#if LCD_RS_PIO_ID == PIOA_ID
#define LCD_RW_SET() { outr(PIO_PER, LCD_RW); outr(PIO_SODR, LCD_RW); outr(PIO_OER, LCD_RW); }
#define LCD_RW_CLR() { outr(PIO_PER, LCD_RW); outr(PIO_CODR, LCD_RW); outr(PIO_OER, LCD_RW); }
#elif LCD_RS_PIO_ID == PIOB_ID
#define LCD_RW_SET() { outr(PIO_PER, LCD_RW); outr(PIO_SODR, LCD_RW); outr(PIO_OER, LCD_RW); }
#define LCD_RW_CLR() { outr(PIO_PER, LCD_RW); outr(PIO_CODR, LCD_RW); outr(PIO_OER, LCD_RW); }
#elif LCD_RS_PIO_ID == PIOC_ID
#define LCD_RW_SET() { outr(PIO_PER, LCD_RW); outr(PIO_SODR, LCD_RW); outr(PIO_OER, LCD_RW); }
#define LCD_RW_CLR() { outr(PIO_PER, LCD_RW); outr(PIO_CODR, LCD_RW); outr(PIO_OER, LCD_RW); }
#else
#define LCD_RW_SET() { outr(PIO_PER, LCD_RW); outr(PIO_SODR, LCD_RW); outr(PIO_OER, LCD_RW); }
#define LCD_RW_CLR() { outr(PIO_PER, LCD_RW); outr(PIO_CODR, LCD_RW); outr(PIO_OER, LCD_RW); }
#endif /* LCD_RW_PIO_ID */
#else /* LCD_RW_BIT */
#define LCD_RW_SET()
#define LCD_RW_CLR()
#endif /* LCD_RW_BIT */

#if LCD_DATA_PIO_ID == PIOA_ID
#define LCD_DATA_BASE PIOA_BASE
#elif LCD_DATA_PIO_ID == PIOB_ID
#define LCD_DATA_BASE PIOB_BASE
#else /* LCD_DATA_PIO_ID */
#define LCD_DATA_BASE PIO_BASE
#endif

#define PIO_PER     (LCD_DATA_BASE + AVR32_GPIO_GPERS)    /*!< \brief enable register address. */
#define PIO_PDR     (LCD_DATA_BASE + AVR32_GPIO_GPERC)    /*!< \brief disable register address. */
#define PIO_PSR     (LCD_DATA_BASE + AVR32_GPIO_GPER)    /*!< \brief status register address. */

#define PIO_OER     (LCD_DATA_BASE + AVR32_GPIO_ODERS)    /*!< \brief Output enable register address. */
#define PIO_ODR     (LCD_DATA_BASE + AVR32_GPIO_ODERC)    /*!< \brief Output disable register address. */
#define PIO_OSR     (LCD_DATA_BASE + AVR32_GPIO_ODER)    /*!< \brief Output status register address. */

#define PIO_SODR    (LCD_DATA_BASE + AVR32_GPIO_OVRS)   /*!< \brief Set output data register address. */
#define PIO_CODR    (LCD_DATA_BASE + AVR32_GPIO_OVRC)   /*!< \brief Clear output data register address. */
#define PIO_ODSR    (LCD_DATA_BASE + AVR32_GPIO_OVR)   /*!< \brief Output data status register address. */

#define PIO_PDSR    (LCD_DATA_BASE + AVR32_GPIO_PVR)   /*!< \brief Pin data status register address. */

#define PIO_IFER    (LCD_DATA_BASE + AVR32_GPIO_GFERS)   /*!< \brief Input filter enable register address. */
#define PIO_IFDR    (LCD_DATA_BASE + AVR32_GPIO_GFERC)   /*!< \brief Input filter disable register address. */
#define PIO_IFSR    (LCD_DATA_BASE + AVR32_GPIO_GFER)   /*!< \brief Input filter status register address. */

#define PIO_IER     (LCD_DATA_BASE + AVR32_GPIO_IERS)    /*!< \brief Interrupt enable register address. */
#define PIO_IDR     (LCD_DATA_BASE + AVR32_GPIO_IERC)    /*!< \brief Interrupt disable register address. */
#define PIO_ISR     (LCD_DATA_BASE + AVR32_GPIO_IER)    /*!< \brief Interrupt status register address. */

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

static void INLINE LcdSetBits(unsigned int mask)
{
	outr(PIO_PER, mask);
    outr(PIO_SODR , mask);
    outr(PIO_OER, mask);
}

static void INLINE LcdClrBits(unsigned int mask)
{
	outr(PIO_PER, mask);
    outr(PIO_CODR, mask);
    outr(PIO_OER, mask);
}
  
#ifdef LCD_RW_BIT
static unsigned int LcdReadNibble(void)
{
    unsigned int rc;
	LCD_EN_SET();
    NutMicroDelay(LCD_SHORT_DELAY);
	rc = inr(PIO_PDSR) & LCD_DATA;
	LCD_EN_CLR();
    NutMicroDelay(LCD_SHORT_DELAY);

#ifdef LCD_DATA_LSB
    rc >>= LCD_DATA_LSB;
#else
    {
        unsigned int val = 0;

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
static unsigned int LcdReadByte(void)
{
    NutMicroDelay(LCD_SHORT_DELAY);
    LCD_RW_SET();
	outr(PIO_PDR, LCD_DATA);
    outr(PIO_ODR, LCD_DATA);
	outr(LCD_DATA_BASE,inr(LCD_DATA_BASE)&LCD_DATA); 	
    NutMicroDelay(LCD_SHORT_DELAY);
    return LcdReadNibble();
}

/*!
 * \brief Read status byte from LCD controller.
 */
static unsigned int LcdReadStatus(void)
{
    /* RS low selects status register. */
    LCD_RS_CLR();
    return LcdReadByte();
}

#endif                          

#if 0
/* This function is a bit critical as some chipsets are known to
 * release the redy bit some time early. So after rady goes low,
 * another fixed delay has to be added before soing the next access.
 */
static void LcdWaitReady(unsigned int delay)
{
    while (delay--) {
        NutMicroDelay(1);
    }
}
#endif

/*!
 * \brief Send half byte to LCD controller.
 *
 * \param nib The four least significant bits are sent.
 */
static void LcdWriteNibble(unsigned int nib)
{
#ifdef LCD_DATA_LSB
    nib <<= LCD_DATA_LSB;
#else
    {
        unsigned int val = 0;
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

    /* Create Enable Pulse:
     * For HD44780 Displays we need:
     * Vcc = 5.0V -> PWeh >= 230ns
     * Vcc = 3.3V -> PWeh >= 500ns
     */
    LCD_EN_SET();
    NutMicroDelay(LCD_SHORT_DELAY);
    LCD_EN_CLR();
}

/*!
 * \brief Send byte to LCD controller.
 *
 * \param data Byte to send.
 */
static void LcdWriteByte(unsigned int data)
{
    /* If configured set RW low */
#ifdef LCD_RW_BIT
    LCD_RW_CLR();
#endif

    /* If using 4-bit access, write two nibbles now */
#ifdef LCD_IF_4BIT
    LcdWriteNibble(data >> 4);
    LcdNanoDelay(LCD_PW_EH);
    LcdWriteNibble(data);
#else
    /* else write one byte */
	LcdWriteNibble(data);
#endif

    /* If configured, let the task sleep before next character */
#if defined(LCD_SLEEP_DLY)
    NutSleep(1);
#else
    /* or add a fixed delay and immediately process next char */
    NutMicroDelay(LCD_E2E_DLY);
#endif
}

/*!
 * \brief Send command byte to LCD controller.
 *
 * \param cmd Byte to send.
 */
static void LcdWriteCmd(uint8_t cmd)
{
    /* RS low selects instruction register. */
    LCD_RS_CLR();
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
	LCD_RS_SET();
    LcdWriteByte(data);
}

static void LcdSetCursor(uint8_t pos)
{
    uint8_t offset[] = {
#ifdef LCD_KS0073
        0x00, 0x20, 0x40, 0x60
#elif (LCD_COLS >= 20)
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
    NutSleep(2);
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
    NutSleep(2);
}

static void LcdCursorMode(uint8_t on)
{
    LcdWriteCmd(1 << LCD_ON_CTRL | on ? 1 << LCD_ON_CURSOR : 0x00);
}

static int LcdInit(NUTDEVICE * dev)
{ 
	LCD_RS_CLR();			
	LCD_RW_CLR();			
	LcdClrBits(LCD_DATA);	
	NutMicroDelay(30);
	LCD_EN_CLR();
	NutMicroDelay(30);
	NutSleep(18);

    /* This initialization will make sure, that the LCD is switched
     * to 8-bit mode, no matter which mode we start from or we finally
     * need.
     */
	LcdWriteCmd(_BV(LCD_FUNCTION) | _BV(LCD_FUNCTION_8BIT));
    NutSleep(16);
	LcdWriteCmd(_BV(LCD_FUNCTION) | _BV(LCD_FUNCTION_8BIT));
    NutSleep(4);
	LcdWriteCmd(_BV(LCD_FUNCTION) | _BV(LCD_FUNCTION_8BIT));
    NutSleep(2);
    LcdWriteInstruction((_BV(LCD_FUNCTION)) |  (_BV(LCD_FUNCTION_8BIT)) |  (_BV(LCD_EXT)) , LCD_SHORT_DELAY);
  
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

	return 0;
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
    {'L', 'c', 'd', 0, 0, 0, 0, 0, 0},  /*!< Unique device name. */
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
