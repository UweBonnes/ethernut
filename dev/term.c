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
 * Revision 1.1  2003/05/09 14:40:52  haraldkipp
 * Initial revision
 *
 * Revision 1.3  2003/05/06 18:34:22  harald
 * Cleanup
 *
 * Revision 1.2  2003/04/21 16:25:24  harald
 * Release prep
 *
 * Revision 1.1  2003/03/31 14:53:08  harald
 * Prepare release 3.1
 *
 */

#include <dev/term.h>

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*!
 * \addtogroup xgTerminal
 */

/*@{*/

static prog_char termid[] = "Term 1.0";

static void TermRefreshLineEnd(CONST TERMDCB * dcb, u_char row, u_char col)
{
    u_char i = col;
    u_char *cp = dcb->dcb_smem + row * dcb->dcb_vcols + col;

    /* Disable cursor to avoid flickering. */
    if (dcb->dcb_modeflags & LCD_MF_CURSORON)
        (*dcb->dss_cursor_mode) (0);

    /* Position cursor to the rwo and column to refresh. */
    (*dcb->dss_set_cursor) (row * dcb->dcb_ncols + col);

    /*
     * This loop looks weird. But it was the only way I found to get 
     * around a GCC bug in reload1.c:1920.
     */
    for (;;) {
        if (i++ >= dcb->dcb_vcols)
            break;
        (*dcb->dss_write) (*cp++);
    }

    /* Re-enable cursor. */
    if (dcb->dcb_modeflags & LCD_MF_CURSORON)
        (*dcb->dss_cursor_mode) (1);
}

void TermRefresh(TERMDCB * dcb)
{
    u_char ir;

    for (ir = 0; ir < dcb->dcb_nrows; ir++)
        TermRefreshLineEnd(dcb, ir, 0);
    (*dcb->dss_set_cursor) (dcb->dcb_row * dcb->dcb_ncols + dcb->dcb_col);
}

static void TermClear(TERMDCB * dcb)
{
    memset(dcb->dcb_smem, ' ', dcb->dcb_vcols * dcb->dcb_nrows);
    dcb->dcb_col = 0;
    dcb->dcb_row = 0;
    (*dcb->dss_clear) ();
}

static void TermDeleteLine(TERMDCB * dcb, u_char row)
{
    u_char i;
    u_char *dcp;

    for (i = row; i < dcb->dcb_nrows - 1; i++) {
        dcp = dcb->dcb_smem + i * dcb->dcb_vcols;
        memcpy(dcp, dcp + dcb->dcb_vcols, dcb->dcb_vcols);
    }
    memset(dcb->dcb_smem + (dcb->dcb_nrows - 1) * dcb->dcb_vcols, ' ', dcb->dcb_vcols);
    TermRefresh(dcb);
}

static void TermInsertLine(TERMDCB * dcb, u_char row)
{
    u_char i;
    u_char *dcp;

    for (i = dcb->dcb_nrows - 1; i > row; i--) {
        dcp = dcb->dcb_smem + i * dcb->dcb_vcols;
        memcpy(dcp, dcp - dcb->dcb_vcols, dcb->dcb_vcols);
    }
    memset(dcb->dcb_smem + row * dcb->dcb_vcols, ' ', dcb->dcb_vcols);
    TermRefresh(dcb);
}

static void TermCursorLeft(TERMDCB * dcb)
{
    if (dcb->dcb_col) {
        (*dcb->dss_cursor_left) ();
        dcb->dcb_col--;
    }
}

static void TermCursorRight(TERMDCB * dcb)
{
    if (++dcb->dcb_col < dcb->dcb_vcols)
        (*dcb->dss_cursor_right) ();
    else
        dcb->dcb_col = dcb->dcb_vcols - 1;
}

static void TermCursorUp(TERMDCB * dcb)
{
    if (dcb->dcb_row) {
        dcb->dcb_row--;
        (*dcb->dss_set_cursor) (dcb->dcb_row * dcb->dcb_ncols + dcb->dcb_col);
    }
}

static void TermLinefeed(TERMDCB * dcb)
{
    if (++dcb->dcb_row >= dcb->dcb_nrows) {
        dcb->dcb_row = dcb->dcb_nrows - 1;
        TermDeleteLine(dcb, 0);
    } else
        (*dcb->dss_set_cursor) (dcb->dcb_row * dcb->dcb_ncols + dcb->dcb_col);
}

static void TermReverseLinefeed(TERMDCB * dcb)
{
    if (dcb->dcb_row--)
        (*dcb->dss_set_cursor) (dcb->dcb_row * dcb->dcb_ncols + dcb->dcb_col);
    else {
        dcb->dcb_row = 0;
        TermInsertLine(dcb, 0);
    }
}

static void TermEraseLineEnd(TERMDCB * dcb, u_char col)
{
    if (col < dcb->dcb_vcols) {
        memset(dcb->dcb_smem + dcb->dcb_row * dcb->dcb_vcols + col, ' ', dcb->dcb_vcols - col);
        TermRefresh(dcb);
    }
}

static void TermEraseEnd(TERMDCB * dcb)
{
    u_char i;

    if (dcb->dcb_col < dcb->dcb_vcols)
        memset(dcb->dcb_smem + dcb->dcb_row * dcb->dcb_vcols + dcb->dcb_col, ' ', dcb->dcb_vcols - dcb->dcb_col);
    for (i = dcb->dcb_row + 1; i < dcb->dcb_nrows; i++)
        memset(dcb->dcb_smem + i * dcb->dcb_vcols, ' ', dcb->dcb_vcols);
    TermRefresh(dcb);
}

static void TermEraseLineStart(TERMDCB * dcb)
{
    if (dcb->dcb_col) {
        memset(dcb->dcb_smem + dcb->dcb_row * dcb->dcb_vcols, ' ', dcb->dcb_col);
        TermRefresh(dcb);
    }
}

static void TermEraseStart(TERMDCB * dcb)
{
    u_char i;

    if (dcb->dcb_col)
        memset(dcb->dcb_smem + dcb->dcb_row * dcb->dcb_vcols, ' ', dcb->dcb_col);
    for (i = 0; i < dcb->dcb_row; i++)
        memset(dcb->dcb_smem + i * dcb->dcb_vcols, ' ', dcb->dcb_vcols);
    TermRefresh(dcb);
}

static void TermDeleteChar(TERMDCB * dcb, u_char col)
{
    u_char i;
    u_char *cp = dcb->dcb_smem + dcb->dcb_row * dcb->dcb_vcols + col;

    for (i = col; i < dcb->dcb_vcols - 1; i++, cp++)
        *cp = *(cp + 1);
    *cp = ' ';
    TermRefresh(dcb);
}

/*
 * Insert a space at the cursor position.
 */
static void TermInsertSpace(TERMDCB * dcb)
{
    u_char i;
    u_char *cp = dcb->dcb_smem + (dcb->dcb_row + 1) * dcb->dcb_vcols - 1;

    for (i = dcb->dcb_col; i < dcb->dcb_vcols - 1; i++, cp--)
        *cp = *(cp - 1);
    *cp = ' ';
    TermRefreshLineEnd(dcb, dcb->dcb_row, dcb->dcb_col);
    (*dcb->dss_set_cursor) (dcb->dcb_row * dcb->dcb_ncols + dcb->dcb_col);
}

/*
 * Clear display and print identification.
 */
static void TermIdentify(TERMDCB * dcb)
{
    PGM_P pcp = termid;

    TermClear(dcb);
    while (PRG_RDB(pcp)) {
        (*dcb->dss_write) (PRG_RDB(pcp));
        pcp++;
    }
}

/*!
 * \brief Perform special LCD control functions.
 *
 * \param dev  Identifies the device that receives the device-control
 *             function. This pointer must have been retrieved previously
 *             by using the NutDeviceOpen() function.
 * \param req  Requested control function. May be set to one of the
 *             following constants:
 * - LCD_CMDBYTE     Send command byte to display. Parameter conf points 
 *                   to  an u_char value containing the command byte.
 * - LCD_CMDWORD16   Send 16 bit command word to display. Parameter conf 
 *                   points to an u_short value containing the command 
 *                   bytes. The most significant byte is send first.
 * - LCD_CMDWORD32   Send 32 bit command byte to display. Parameter conf 
 *                   points to an u_long value containing the command 
 *                   bytes. The most significant bytes are send first.
 * - LCD_DATABYTE    Send raw data byte to display. Parameter conf points 
 *                   to an u_char value containing the data byte.
 * - LCD_DATAWORD16  Send 16 bit raw data word to display. Parameter conf 
 *                   points to an u_short value containing the data 
 *                   bytes. The most significant byte is send first.
 * - LCD_DATAWORD32  Send 32 bit raw data word to display. Parameter conf 
 *                   points to an u_long value containing the data 
 *                   bytes. The most significant bytes are send first.
 * - LCD_SETCOOKEDMODE Set terminal control character mode. Parameter conf 
 *                     points to an u_long value containing 0 (off) or 1 
 *                     (on).
 * - LCD_GETCOOKEDMODE Query terminal control character mode. Parameter 
 *                     conf points to an u_long value receiving 0 (off) 
 *                     or 1 (on).
 *
 * \param conf Points to a buffer that contains any data required for
 *             the given control function or receives data from that
 *             function.
 *
 * \return 0 on success, -1 otherwise.
 *
 */
int TermIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    TERMDCB *dcb = dev->dev_dcb;
    u_short usv;
    u_long ulv;

    switch (req) {
    case LCD_CMDBYTE:
        (*dcb->dss_command) (*(u_char *)conf, 10);
        break;
    case LCD_CMDWORD16:
        usv = *(u_short *)conf;
        (*dcb->dss_command) ((u_char)(usv >> 8), 10);
        (*dcb->dss_command) ((u_char)usv, 10);
        break;
    case LCD_CMDWORD32:
        ulv = *(u_long *)conf;
        (*dcb->dss_command) ((u_char)(ulv >> 24), 10);
        (*dcb->dss_command) ((u_char)(ulv >> 16), 10);
        (*dcb->dss_command) ((u_char)(ulv >> 8), 10);
        (*dcb->dss_command) ((u_char)ulv, 10);
        break;
    case LCD_DATABYTE:
        (*dcb->dss_write) (*(u_char *)conf);
        break;
    case LCD_DATAWORD16:
        usv = *(u_short *)conf;
        (*dcb->dss_write) ((u_char)(usv >> 8));
        (*dcb->dss_write) ((u_char)usv);
        break;
    case LCD_DATAWORD32:
        ulv = *(u_long *)conf;
        (*dcb->dss_write) ((u_char)(ulv >> 24));
        (*dcb->dss_write) ((u_char)(ulv >> 16));
        (*dcb->dss_write) ((u_char)(ulv >> 8));
        (*dcb->dss_write) ((u_char)ulv);
        break;
    case LCD_SETCOOKEDMODE:
        if (*(u_long *)conf)
            dcb->dcb_modeflags |= LCD_MF_COOKEDMODE;
        else
            dcb->dcb_modeflags &= ~LCD_MF_COOKEDMODE;
        break;
    case LCD_GETCOOKEDMODE:
        if (dcb->dcb_modeflags & LCD_MF_COOKEDMODE)
            *(u_long *)conf = 1;
        else
            *(u_long *)conf = 0;
        break;
    }
    return 0;
}


/*!
 * \brief Initialize the terminal device.
 *
 * Prepares the device for subsequent writing.
 *
 * Application should not call this function directly, but use the
 * stdio interface.
 *
 * \param dev  Identifies the device to initialize.
 *
 * \return 0 on success, -1 otherwise.
 */
int TermInit(NUTDEVICE * dev)
{
    TERMDCB *dcb = dev->dev_dcb;

    /*
     * Initialize the display hardware.
     */
    (*dcb->dss_init) ();

    /*
     * Initialize driver control block.
     */
    dcb->dcb_smem = malloc(dcb->dcb_nrows * dcb->dcb_vcols);
    TermClear(dcb);

    return 0;
}

/*! 
 * \brief Write to the terminal device. 
 *
 * \param fp     File pointer to a previously opened device.
 * \param buffer Pointer to the data bytes to be written.
 * \param len    Number of bytes to write.
 * \param pflg   Set if the buffer is located in program space.
 *
 * \return The number of bytes written.
 *
 */
static int TermPut(NUTDEVICE * dev, CONST void *buffer, int len, int pflg)
{
    int rc;
    CONST u_char *cp;
    u_char ch;
    TERMDCB *dcb = dev->dev_dcb;

    /*
     * Call without data pointer is accepted.
     */
    if (buffer == 0)
        return 0;

    /*
     * Put characters in transmit buffer.
     */
    cp = buffer;
    for (rc = 0; rc < len; cp++, rc++) {
        ch = pflg ? PRG_RDB(cp) : *cp;

        if (dcb->dcb_modeflags & LCD_MF_COOKEDMODE) {
            /* Process special characters. */
            if (dcb->dcb_ctlseq == 0) {
                /* Linefeed. */
                if (ch == 10) {
                    dcb->dcb_col = 0;
                    TermLinefeed(dcb);
                    continue;
                }

                /* Carriage return. */
                if (ch == 13) {
                    dcb->dcb_col = 0;
                    (*dcb->dss_set_cursor) (dcb->dcb_row * dcb->dcb_ncols);
                    continue;
                }

                /* Escape. */
                if (ch == 27) {
                    dcb->dcb_ctlseq = 1;
                    continue;
                }

                /* Backspace. */
                if (ch == 8) {
                    if (dcb->dcb_col) {
                        dcb->dcb_col--;
                        TermDeleteChar(dcb, dcb->dcb_col);
                    }
                    continue;
                }

                /* Formfeed. */
                if (ch == 12) {
                    TermClear(dcb);
                    continue;
                }
            }

            /* Last character was ESC. */
            if (dcb->dcb_ctlseq == 1) {
                dcb->dcb_ctlseq = 0;

                switch (ch) {
                    /* Insert space. */
                case '@':
                    TermInsertSpace(dcb);
                    break;

                    /* Cursor up. */
                case 'A':
                    TermCursorUp(dcb);
                    break;

                    /* Cursor down. */
                case 'B':
                    if (++dcb->dcb_row >= dcb->dcb_nrows)
                        dcb->dcb_row = dcb->dcb_nrows - 1;
                    else
                        (*dcb->dss_set_cursor) (dcb->dcb_row * dcb->dcb_ncols + dcb->dcb_col);
                    break;

                    /* Cursor right. */
                case 'C':
                    TermCursorRight(dcb);
                    break;

                    /* Cursor left. */
                case 'D':
                    TermCursorLeft(dcb);
                    break;

                    /* Clear screen and cursor home. */
                case 'E':
                    TermClear(dcb);
                    break;

                    /* Cursor home. */
                case 'H':
                    dcb->dcb_col = 0;
                    dcb->dcb_row = 0;
                    (*dcb->dss_cursor_home) ();
                    break;

                    /* Reverse linefeed. */
                case 'I':
                    TermReverseLinefeed(dcb);
                    break;

                    /* Erase to end of screen. */
                case 'J':
                    TermEraseEnd(dcb);
                    break;

                    /* Erase to end of line. */
                case 'K':
                    TermEraseLineEnd(dcb, dcb->dcb_col);
                    break;

                    /* Insert line. */
                case 'L':
                    TermInsertLine(dcb, dcb->dcb_row);
                    break;

                    /* Delete line. */
                case 'M':
                    TermDeleteLine(dcb, dcb->dcb_row);
                    break;

                    /* Delete character. */
                case 'P':
                    TermDeleteChar(dcb, dcb->dcb_col);
                    break;

                    /* Cursor position. */
                case 'Y':
                    dcb->dcb_ctlseq = 2;
                    break;

                    /* Identify. */
                case 'Z':
                    TermIdentify(dcb);
                    break;

                    /* Cursor on. */
                case 'e':
                    dcb->dcb_modeflags |= LCD_MF_CURSORON;
                    (*dcb->dss_cursor_mode) (1);
                    break;

                    /* Cursor off. */
                case 'f':
                    dcb->dcb_modeflags &= ~LCD_MF_CURSORON;
                    (*dcb->dss_cursor_mode) (0);
                    break;

                    /* Erase to start of screen. */
                case 'd':
                    TermEraseStart(dcb);
                    break;

                    /* Erase to start of line. */
                case 'o':
                    TermEraseLineStart(dcb);
                    break;
                }
                continue;
            }

            /* Receive cursor row position. */
            if (dcb->dcb_ctlseq == 2) {
                dcb->dcb_ctlseq = 3;
                if (ch < 32)
                    dcb->dcb_row = 0;
                else if (ch - 32 >= dcb->dcb_nrows)
                    dcb->dcb_row = dcb->dcb_nrows - 1;
                else
                    dcb->dcb_row = ch - 32;
                continue;
            }

            /* Receive cursor column position. */
            if (dcb->dcb_ctlseq == 3) {
                dcb->dcb_ctlseq = 0;
                if (ch < 32)
                    dcb->dcb_col = 0;
                else if (ch - 32 >= dcb->dcb_vcols)
                    dcb->dcb_col = dcb->dcb_vcols - 1;
                else
                    dcb->dcb_col = ch - 32;
                (*dcb->dss_set_cursor) (dcb->dcb_row * dcb->dcb_ncols + dcb->dcb_col);
                continue;
            }
        }

        /* 
         * Send any character to the LCD driver, which had been left 
         * unprocessed upto this point.
         */
        (*dcb->dss_write) (ch);

        /* Update shadow memory. */
        if (dcb->dcb_modeflags & LCD_MF_COOKEDMODE) {
            *(dcb->dcb_smem + dcb->dcb_row * dcb->dcb_vcols + dcb->dcb_col) = ch;
            if (++dcb->dcb_col >= dcb->dcb_vcols) {
                dcb->dcb_col = dcb->dcb_vcols - 1;
                (*dcb->dss_set_cursor) (dcb->dcb_row * dcb->dcb_ncols + dcb->dcb_col);
            }
        }
    }
    return rc;
}

/*! 
 * \brief Write data to a terminal device.
 *
 * Application should not call this function directly, but use the
 * stdio interface.
 *
 * The data may contain special character sequences, which are interpreted
 * by the terminal device to control specific display functions:
 * - Ctrl-H (ASCII 8) Backspace
 * - Ctrl-J (ASCII 10) Linefeed
 * - Ctrl-L (ASCII 12) Formfeed
 * - Ctrl-M (ASCII 13) Carriage return
 *
 * In addition a superset of VT52 control sequences are interpreted. Each
 * sequence starts with an ESC character (ASCII 27):
 * - ESC @ Insert space(*).
 * - ESC A Move cursor up, ignored if on first line.
 * - ESC B Move cursor down, ignored if on last line.
 * - ESC C Move cursor right, ignored if on last column.
 * - ESC D Move cursor left, ignored if on first column.
 * - ESC E Clear display and move cursor home (*).
 * - ESC H Move cursor home.
 * - ESC I Reverse linefeed.
 * - ESC J Erase to end of display.
 * - ESC K Erase to end of line.
 * - ESC L Insert line (*).
 * - ESC M Delete line (*).
 * - ESC P Delete character (*).
 * - ESC R Reactivate display (*).
 * - ESC S Put display in sleep mode (*).
 * - ESC Y x+32 y+32 Move cursor to position.
 * - ESC e Cursor on (*).
 * - ESC f Cursor off (*).
 * - ESC d Erase to start of display (*).
 * - ESC o Erase to start of line (*).
 *
 * (*) Not a VT52 command.
 *
 * \param fp     File pointer to a previously opened device.
 * \param buffer Pointer to the data bytes to be written.
 * \param len    Number of bytes to write.
 *
 * \return The number of bytes written.
 *
 * \todo TAB should be interpreted.
 *
 * \bug Switching from graphic mode back to text mode will not work,
 *      because all escape sequences are ignored as soon as the graphic
 *      mode has been enabled. Applications should use _ioctl() 
 *      functions to switch modes.
 */
int TermWrite(NUTFILE * fp, CONST void *buffer, int len)
{
    return TermPut(fp->nf_dev, buffer, len, 0);
}

/*! 
 * \brief Write data from program space to a terminal device.
 *
 * Similar to TermWrite() except that the data is located in program memory.
 *
 * Application should not call this function directly, but use the
 * stdio interface.
 *
 * \param fp     File pointer to a previously opened device.
 * \param buffer Pointer to the data bytes in program space to be written.
 * \param len    Number of bytes to write.
 *
 * \return The number of bytes written.
 */
int TermWrite_P(NUTFILE * fp, PGM_P buffer, int len)
{
    return TermPut(fp->nf_dev, (CONST char *) buffer, len, 1);
}

/*! 
 * \brief Open a terminal device.
 *
 * Application should not call this function directly, but use the
 * stdio interface.
 *
 * \param dev  Pointer to device information structure.
 * \param name Filename. Not used with terminal devices.
 * \param mode Operation mode. May be any of the following:
 *             - _O_BINARY Raw mode, no control character
 *               interpretation.
 *             - _O_TEXT Text mode. Control characters are
 *               interpreted and tranlated to LCD commands.
 * \param acc  Access mode. Not used with terminal devices.
 *
 * \return Pointer to a NUTFILE structure or –1 to indicate an error.
 */
NUTFILE *TermOpen(NUTDEVICE * dev, CONST char *name, int mode, int acc)
{
    TERMDCB *dcb = dev->dev_dcb;
    NUTFILE *fp = malloc(sizeof(NUTFILE));

    if (fp == 0)
        return NUTFILE_EOF;

    if (mode & _O_BINARY)
        dcb->dcb_modeflags &= ~LCD_MF_COOKEDMODE;
    else
        dcb->dcb_modeflags |= LCD_MF_COOKEDMODE;
    fp->nf_next = 0;
    fp->nf_dev = dev;
    fp->nf_fcb = 0;

    return fp;
}

/*! 
 * \brief Close a device or file.
 *
 * Application should not call this function directly, but use the
 * stdio interface.
 *
 * \param fp   Pointer to a previously opened NUTFILE structure.
 *
 * \return 0 if the device was successfully closed or –1 to indicate an error.
 */
int TermClose(NUTFILE * fp)
{
    if (fp && fp != NUTFILE_EOF) {
        free(fp);
        return 0;
    }
    return -1;
}
