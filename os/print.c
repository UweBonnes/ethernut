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
 */

/*
 * $Log$
 * Revision 1.1  2003/05/09 14:41:53  haraldkipp
 * Initial revision
 *
 * Revision 1.19  2003/02/04 18:15:57  harald
 * Version 3 released
 *
 * Revision 1.18  2002/11/02 15:19:28  harald
 * Compiler warnings avoided
 *
 * Revision 1.17  2002/10/31 16:34:11  harald
 * Mods by troth for Linux
 *
 * Revision 1.16  2002/06/26 17:29:44  harald
 * First pre-release with 2.4 stack
 *
 */

/*!
 * \addtogroup xgPrint
 */

/*@{*/

#include <stdarg.h>
#include <string.h>
#include <sys/print.h>

/*!
 * \brief Write a buffer to a specified device.
 *
 * Data may not be immediately transmitted to the physical, 
 * but stored in the output buffer. Call NutPrintFlush() to 
 * force immediate transmission.
 *
 * If the buffer is in program space, use NutPrintBinary_P()
 * instead.
 *
 * \deprecated Use _write() or fwrite() in new programs.
 *
 * \param dev  Identifies the device to write to. A null pointer may be used 
 *             for unbuffered output to the first on-chip UART. If this 
 *             pointer is not null, it must have been retrieved by directly 
 *             or indirectly calling NutDeviceOpen() for real devices. For 
 *             virtual devices this pointer is returned by the function that 
 *             creates the device.
 * \param data Buffer to be written.
 * \param len  Number of characters to be printed.
 *
 * \return The number of characters printed or -1 in case
 *         of an error.
 */
int NutPrintBinary(NUTDEVICE * dev, CONST char *data, int len)
{
    int rc = 0;
    int bite;

    while (len) {
        if ((bite = NutDeviceWrite(dev, data, len)) < 0) {
            rc = -1;
            break;
        }
        data += bite;
        rc += bite;
        len -= bite;
    }
    return rc;
}

/*!
 * \brief Write a buffer in program space to a specified device.
 *
 * Data may not be immediately transmitted to the physical, 
 * but stored in the output buffer. Call NutPrintFlush() to 
 * force immediate transmission.
 *
 * To print a buffer in RAM, use NutPrintBinary().
 *
 * \deprecated Use _write_P() or fwrite_P() in new programs.
 *
 * \param dev  Identifies the device to write to. A null pointer may be used 
 *             for unbuffered output to the first on-chip UART. If this 
 *             pointer is not null, it must have been retrieved by directly 
 *             or indirectly calling NutDeviceOpen() for real devices. For 
 *             virtual devices this pointer is returned by the function that 
 *             creates the device.
 * \param data Buffer in program space to be printed.
 * \param len  Number of characters to be printed.
 *
 * \return The number of characters printed or -1 in case
 *         of an error.
 */
int NutPrintBinary_P(NUTDEVICE * dev, PGM_P data, int len)
{
    int rc = 0;
    int bite;

    while (len) {
        if ((bite = NutDeviceWrite_P(dev, data, len)) < 0) {
            rc = -1;
            break;
        }
        data += bite;
        rc += bite;
        len -= bite;
    }
    return rc;
}

/*!
 * \brief Print a string on a specified device.
 *
 * The string may not be immediately transmitted to the physical
 * device, but stored in the output buffer. Call NutPrintFlush() 
 * to force immediate transmission.
 *
 * If the string is defined in program space, use
 * NutPrintString_P() instead.
 *
 * \deprecated Use fputs() in new programs.
 *
 * \param dev  Identifies the device to write to. A null pointer may be used 
 *             for unbuffered output to the first on-chip UART. If this 
 *             pointer is not null, it must have been retrieved by directly 
 *             or indirectly calling NutDeviceOpen() for real devices. For 
 *             virtual devices this pointer is returned by the function that 
 *             creates the device.
 * \param str String to be printed.
 *
 * \return The number of characters printed or -1 in case
 *         of an error.
 */
int NutPrintString(NUTDEVICE * dev, CONST char *str)
{
    return NutPrintBinary(dev, str, strlen(str));
}

/*!
 * \brief Print a string in program space on a specified device.
 *
 * The string may not be immediately transmitted to the physical
 * device, but stored in the output buffer. Call NutPrintFlush() 
 * to force immediate transmission.
 *
 * If the string is defined in RAM, use NutPrintString() instead.
 *
 * \deprecated Use fputs_P() in new programs.
 *
 * \param dev  Identifies the device to write to. A null pointer may be used 
 *             for unbuffered output to the first on-chip UART. If this 
 *             pointer is not null, it must have been retrieved by directly 
 *             or indirectly calling NutDeviceOpen() for real devices. For 
 *             virtual devices this pointer is returned by the function that 
 *             creates the device.
 * \param str String in program space to be printed.
 *
 * \return The number of characters printed or -1 in case
 *         of an error.
 */
int NutPrintString_P(NUTDEVICE * dev, PGM_P str)
{
    return NutPrintBinary_P(dev, str, strlen_P(str));
}

/*!
 * \brief Print a numeric value on a specified device.
 *
 * The numeric value will be converted to an ASCII string and 
 * printed to the specified stream device. The string may not
 * be transmitted immediately, but stored in the output buffer.
 * Call NutPrintFlush() to force immediate transmission.
 *
 * \deprecated Use fprintf() in new programs.
 *
 * \param dev  Identifies the device to write to. A null pointer may be used 
 *             for unbuffered output to the first on-chip UART. If this 
 *             pointer is not null, it must have been retrieved by directly 
 *             or indirectly calling NutDeviceOpen() for real devices. For 
 *             virtual devices this pointer is returned by the function that 
 *             creates the device.
 * \param val   Value to be printed.
 * \param radix Number base, may be any value between 2 and 16.
 * \param width Minimum width or 0 if no justification required.
 * \param flags Format flags. Any of the following may be ored:
 * - FMTFLG_ZERO for output padding with zeros.
 * - FMTFLG_PLUS forces printing a sign character.
 * - FMTFLG_MINUS for left justification.
 * - FMTFLG_SIGNED for signed values.
 * - FMTFLG_CAPITAL for using capital letters with base above 10.
 *
 * \return The number of characters printed or -1 in case of an error.
 *
 * @see Format flags
 *
 */
int NutPrintInteger(NUTDEVICE * dev, u_long val, u_char radix,
                    u_char width, u_char flags)
{
    u_char *digits;
    u_char raw[16];
    int result;
    u_char rem;
    u_char sign;
    u_char cnt;

    if (radix < 2)
        radix = 10;
    result = 0;

    sign = 0;
    if (flags & FMTFLG_PLUS)
        sign = '+';
    if (flags & FMTFLG_SIGNED)
        if ((long) val < 0) {
            sign = '-';
            val = -(long) val;
        }

    /*
     * Fill scratch buffer with raw digits.
     */
    digits = (flags & FMTFLG_CAPITAL) ? "0123456789ABCDEF"
        : "0123456789abcdef";
    cnt = 0;
    do {
        rem = (u_char) (val % (u_long) radix);
        val = val / (u_long) radix;
        raw[cnt++] = digits[rem];
    } while (val);

    /*
     * Calculate the remaining width for padding
     */
    if (width > cnt) {
        if (sign)
            width--;
        width -= cnt;
    } else
        width = 0;

    /*
     * If not zero padded and not left justified,
     * we put enough spaces in front.
     */
    if ((flags & (FMTFLG_ZERO | FMTFLG_MINUS)) == 0)
        while (width) {
            NutPrintBinary(dev, " ", 1);
            result++;
            width--;
        }
    if (sign) {
        NutPrintBinary(dev, &sign, 1);
        result++;
    }
    if ((flags & (FMTFLG_ZERO | FMTFLG_MINUS)) == FMTFLG_ZERO) {
        result += width;
        while (width) {
            NutPrintBinary(dev, "0", 1);
            width--;
        }
    }
    while (cnt-- > 0) {
        NutPrintBinary(dev, &raw[cnt], 1);
        result++;
    }
    while (width) {
        NutPrintBinary(dev, " ", 1);
        result++;
        width--;
    }
    return result;
}

/*!
 * \brief Print parameters using a format string.
 *
 * This function formats and prints characters and values
 * to a specified stream device.
 *
 * The format string consists of ordinary characters, escape
 * sequences, and format specifications. Ordinary characters
 * and escape sequences are copied to the output routine in
 * the order of their appearance. The format is scanned from
 * left to right. If a format specification is encountered,
 * the value of the corresponding argument is converted and
 * output according to that format specification. If there
 * are more arguments than format specifications, the extra
 * arguments are ignored. If there are more format specifications
 * than arguments, the results are undefined.
 *
 * Recognized format specifiers are:
 * \arg %%c prints a character.
 * \arg %%s prints a string.
 * \arg %%o prints a numerical value in octal digits.
 * \arg %%d prints a signed numerical value in deciaml digits.
 * \arg %%u prints an unsigned numerical value in decimal digits.
 * \arg %%x prints a numerical value in hexadecimal digits using lowercase letters.
 * \arg %%X prints a numerical value in hexadecimal digits using uppercase letters.
 *
 * The resulting string may not be transmitted immediately, but 
 * stored in the output buffer. Call NutPrintFlush() to force 
 * immediate transmission.
 *
 * \deprecated Use fprintf() in new programs.
 *
 * \param dev  Identifies the device to write to. A null pointer may be used 
 *             for unbuffered output to the first on-chip UART. If this 
 *             pointer is not null, it must have been retrieved by directly 
 *             or indirectly calling NutDeviceOpen() for real devices. For 
 *             virtual devices this pointer is returned by the function that 
 *             creates the device.
 * \param fmt Format string.
 *
 * \return The number of characters printed, or a negative value
 *         in case of an output error.
 */
int NutPrintFormat(NUTDEVICE * dev, CONST char *fmt, ...)
{
    va_list ap;
    u_char *s;
    u_char *cp;
    int width;
    int result;
    u_short len;
    u_long val;
    u_char radix;
    u_char flags;
    u_char isize;

    va_start(ap, fmt);
    for (result = 0; *fmt; ++fmt) {
        if (*fmt != '%') {
            if (NutPrintBinary(dev, fmt, 1) < 0)
                return -1;
            result++;
            continue;
        }

        flags = 0;
        while (1) {
            if (*++fmt == '-')
                flags |= FMTFLG_MINUS;
            else if (*fmt == '+')
                flags |= FMTFLG_PLUS;
            else if (*fmt == '0')
                flags |= FMTFLG_ZERO;
            else
                break;
        }

        if (*fmt == '*') {
            ++fmt;
            width = va_arg(ap, int);
            if (width < 0) {
                width = -width;
                flags |= FMTFLG_MINUS;
            }
        } else {
            width = 0;
            while (*fmt >= '0' && *fmt <= '9')
                width = width * 10 + (*fmt++ - '0');
        }

        isize = 's';
        if (*fmt == 'l') {
            isize = *fmt;
            ++fmt;
        }

        if (*fmt == 'c' || *fmt == 's') {
            s = 0;
            if (*fmt == 's') {
                if ((s = va_arg(ap, char *)) == 0) {
                    s = "(NULL)";
                    len = 6;
                } else {
                    len = 0;
                    cp = s;
                    while (*cp++)
                        len++;
                }
            } else
                len = 1;
            if ((flags & FMTFLG_MINUS) == 0)
                while (width > (int) len) {
                    NutPrintBinary(dev, " ", 1);
                    result++;
                    width--;
                }
            if (s) {
                NutPrintBinary(dev, s, len);
                result += len;
            } else {
                u_char ch = (u_char) va_arg(ap, int);
                NutPrintBinary(dev, &ch, 1);
                result++;
            }
            while (width > (int) len) {
                NutPrintBinary(dev, " ", 1);
                result++;
                width--;
            }
            continue;
        }

        radix = 10;
        if (*fmt == 'o')
            radix = 8;
        else if (*fmt == 'X' || *fmt == 'x') {
            if (*fmt == 'X')
                flags |= FMTFLG_CAPITAL;
            radix = 16;
        } else if (*fmt == 'd')
            flags |= FMTFLG_SIGNED;
        else if (*fmt != 'u') {
            if (*fmt != '%') {
                NutPrintBinary(dev, "%", 1);
                result++;
            }
            if (*fmt) {
                NutPrintBinary(dev, fmt, 1);
                result++;
            } else
                fmt--;
            continue;
        }
        if (isize == 'l') {
            if (flags & FMTFLG_SIGNED)
                val = va_arg(ap, long);
            else
                val = va_arg(ap, unsigned long);
        } else {
            if (flags & FMTFLG_SIGNED)
                val = va_arg(ap, int);
            else
                val = va_arg(ap, unsigned int);
        }
        result += NutPrintInteger(dev, val, radix, width, flags);
    }
    va_end(ap);

    return result;
}

/*!
 * Send buffered characters to the specified device.
 *
 * If the device's output buffer contains any, the transmitter
 * is started, if not already running. The function returns
 * immediately, without waiting for data being completely 
 * transmitted.
 *
 * \deprecated Use fflush() in new programs.
 *
 * \param dev  Identifies the device to flush. This must have been retrieved 
 *             by directly or indirectly calling NutDeviceOpen() for real 
 *             devices. For virtual devices this pointer is returned by the 
 *             function that creates the device.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutPrintFlush(NUTDEVICE * dev)
{
    if ((void *) dev >= RAMSTART)
        return NutDeviceWrite(dev, 0, 0);
    return -1;
}

/*@}*/
