/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
 *
 * Copyright (c) 1990, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * This code is partly derived from software contributed to Berkeley by
 * Chris Torek, but heavily rewritten for Nut/OS.
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
 * Revision 1.7  2008/08/11 06:59:40  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.6  2008/07/09 14:20:25  haraldkipp
 * Added support for length modifier h. According to C99, F, G and X type
 * specifiers should work like f, g, and x.
 *
 * Revision 1.5  2006/05/15 15:31:11  freckle
 * Take care of first character after integer
 *
 * Revision 1.4  2006/05/05 15:43:07  freckle
 * Fixes for bugs #1477658 and #1477676
 *
 * Revision 1.3  2004/11/24 15:24:07  haraldkipp
 * Floating point configuration works again.
 *
 * Revision 1.2  2004/02/28 20:14:38  drsung
 * Merge from nut-3_4-release b/c of bugfixes.
 *
 * Revision 1.1.1.1.2.1  2004/02/28 18:47:34  drsung
 * Several bugfixes provided by Francois Rademeyer.
 * - "%%" didnt work
 * - integer parsing corrected
 * - support for "%u"
 *
 * Revision 1.1.1.1  2003/05/09 14:40:29  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.1  2003/02/04 17:49:07  harald
 * *** empty log message ***
 *
 */

#include <cfg/crt.h>

#include "nut_io.h"

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#if defined(NUTCRT_TINYPRINT) && defined(__arm__)
/*
 * Newlib needs _sbrk for floating point conversion. Because newlib
 * libraries are linked after Nut/OS libraries, this function is referenced
 * too late. So we include a reference here to force _sbrk inclusion.
 * This reference should depend on newlib usage, not generally on ARM CPUs,
 * but how to find out if we have newlib or not?
 */
extern char *_sbrk(size_t nbytes);
char *(*sbrk_force)(size_t) = _sbrk;
extern void __assert_func(const char *file, int line, const char *func,
                         const char *failedexpr);
# if !defined(__assert_func_force)
void *(__assert_func_force) = __assert_func;
# endif
#endif

/*!
 * \addtogroup xgCrtStdio
 */
/*@{*/

#define CF_LONG     0x01    /* 1: long or double */
#define CF_SUPPRESS 0x02    /* suppress assignment */
#define CF_SIGNOK   0x04    /* +/- is (still) legal */
#define CF_NDIGITS  0x08    /* no digits detected */
#define CF_PFXOK    0x10    /* 0x prefix is (still) legal */
#define CF_NZDIGITS 0x20    /* no zero digits detected */
#define CF_DPTOK    0x10    /* (float) decimal point is still legal */
#define CF_EXPOK    0x20    /* (float) exponent (e+3, etc) still legal */
#define CF_LONGLONG 0x80    /* 1: long long or long double */

/*
 * Conversion types.
 */
#define CT_CHAR     0       /* %c conversion */
#define CT_STRING   2       /* %s conversion */
#define CT_INT      3       /* integer, i.e., strtoq or strtouq */
#define CT_FLOAT    4       /* floating, i.e., strtod */

double strtod_simple(const char* str, char** endptr)
{
    double result = 0.0;
    char signedResult = '\0';
    char signedExponent = '\0';
    int decimals = 0;
    bool isExponent = false;
    bool hasExponent = false;
    bool hasResult = false;
    // exponent is logically int but is coded as double so that its eventual
    // overflow detection can be the same as for double result
    double exponent = 0;
    char c;

    for (; '\0' != (c = *str); ++str) {
        if ((c >= '0') && (c <= '9')) {
            int digit = c - '0';
            if (isExponent) {
                exponent = (10 * exponent) + digit;
                hasExponent = true;
            } else if (decimals == 0) {
                result = (10 * result) + digit;
                hasResult = true;
            } else {
                result += (double)digit / decimals;
                decimals *= 10;
            }
            continue;
        }

        if (c == '.') {
            if (!hasResult) break; // don't allow leading '.'
            if (isExponent) break; // don't allow decimal places in exponent
            if (decimals != 0) break; // this is the 2nd time we've found a '.'

            decimals = 10;
            continue;
        }

        if ((c == '-') || (c == '+')) {
            if (isExponent) {
                if (signedExponent || (exponent != 0)) break;
                else signedExponent = c;
            } else {
                if (signedResult || (result != 0)) break;
                else signedResult = c;
            }
            continue;
        }

        if (c == 'E') {
            if (!hasResult) break; // don't allow leading 'E'
            if (isExponent) break;
            else isExponent = true;
            continue;
        }

        break; // unexpected character
    }

    if (isExponent && !hasExponent) {
        while (*str != 'E')
            --str;
    }

    if (!hasResult && signedResult)
	--str;

    if (endptr)
	*endptr = (char*)str;

    for (; exponent != 0; --exponent) {
        if (signedExponent == '-') result /= 10;
        else result *= 10;
    }

    if (signedResult == '-' && result != 0)
	result = -result;
    return result;
}

/*!
 * \brief Read formatted data using a given input function.
 *
 * \param _getb Input function for reading data.
 * \param fd    Descriptor of a previously opened file, device or
 *              connected socket.
 * \param fmt   Format string containing coversion specifications.
 * \param ap    List of pointer arguments.
 *
 * \return The number of fields successfully converted and assigned.
 *         The return value is EOF, if an error occurs or if the end
 *         of the stream is reached before the first conversion.
 */
int _getf(int _getb(int, void *, size_t), int fd, const char *fmt, va_list ap)
{

    uint8_t cf;                 /* Character from format. */
    uint8_t ch;                 /* Character from input. */
    uint8_t sign;               /* Character from input. */
    size_t width;               /* Field width. */
    uint8_t flags;              /* CF_ flags. */
    uint8_t ct;                 /* CT_ conversion type. */
    uint8_t base;               /* Conversion base. */
    uint8_t ccnt = 0;           /* Number of conversions. */
    uint8_t acnt = 0;           /* Number of fields assigned. */
    uint8_t hcnt;               /* Number of 'half' specifiers. */
    char buf[25];               /* Temporary buffer, up to 0xffffffffffffffff = 01777777777777777777777\0/. */
    char *cp;                   /* Temporary pointer. */
    uint8_t ch_ready = 0;       /* Character available from previous peek
                                   This is necessary as a hack to get around a missing ungetc */

    for (;;) {
        cf = *fmt++;
        if (cf == 0)
            return acnt;

        /*
         * Match whitespace.
         */
        if (isspace(cf)) {
            for (;;) {
                if (_getb(fd, &ch, 1) != 1)
                    break;
                if (!isspace(ch)) {
                    ch_ready = 1; /* character avail without read */
                    break;
                }
            }
            continue;
        }

        /*
         * Match literals.
         */
        if (cf != '%') {
            if (!ch_ready && _getb(fd, &ch, 1) != 1)
                return ccnt ? acnt : EOF;
            if (ch != cf)
                return acnt;
            ch_ready = 0; /* character used now */
            continue;
        }

        cf = *fmt++;
        /*
         * Check for a '%' literal.
         */
        if (cf == '%') {
            if (!ch_ready && _getb(fd, &ch, 1) != 1)
                return ccnt ? acnt : EOF;
            if (ch != cf)
                return acnt;
            ch_ready = 0; /* character used now */
            continue;
        }

        /*
         * Collect modifiers.
         */
        width = 0;
        flags = 0;
        hcnt = 0;
        for (;;) {
            if (cf == '*')
                flags |= CF_SUPPRESS;
            else if (cf == 'l') {
                if (flags & CF_LONG) {
                    flags |= CF_LONGLONG;
                } else {
                    flags |= CF_LONG;
                }
            } else if (cf == 'L')
                flags |= CF_LONGLONG;
            else if (cf == 'h')
                hcnt++;
            else if (cf >= '0' && cf <= '9')
                width = width * 10 + cf - '0';
            else
                break;

            cf = *fmt++;
        }

        /*
         * Determine the types.
         */
        base = 10;
        sign = 0;
        ct = CT_INT;
        switch (cf) {
        case '\0':
            return EOF;
        case 's':
            ct = CT_STRING;
            break;
        case 'c':
            ct = CT_CHAR;
            break;
        case 'd':
            sign = 1;
            break;
        case 'i':
            sign = 1;
            base = 0;
            break;
        case 'u':
            base = 10;
            break;
        case 'o':
            base = 8;
            break;
        case 'x':
        case 'X':
            flags |= CF_PFXOK;
            base = 16;
            break;
        case 'e':
        case 'f':
        case 'F':
        case 'g':
        case 'G':
            ct = CT_FLOAT;
            break;
       }

        /*
         * Process characters.
         */
        if (ct == CT_CHAR) {
            if (width == 0)
                width = 1;
            if (flags & CF_SUPPRESS) {
                while (width > sizeof(buf)) {
                    if (!ch_ready && _getb(fd, buf, sizeof(buf)) <= 0)
                        return ccnt ? acnt : EOF;
                    width -= sizeof(buf);
                }
                if (width)
                    if (!ch_ready && _getb(fd, &buf, width) <= 0)
                        return ccnt ? acnt : EOF;
            } else {
                if (!ch_ready && _getb(fd, (void *) va_arg(ap, char *), width) <= 0)
                     return ccnt ? acnt : EOF;
                acnt++;
            }
            ch_ready = 0; /* character used now */
            ccnt++;
            continue;
        }

        /*
         * Skip whitespaces.
         */
        if (!ch_ready && _getb(fd, &ch, 1) != 1)
            return ccnt ? acnt : EOF;
        ch_ready = 0; /* no character ready anymore */
        while (isspace(ch)) {
            if (_getb(fd, &ch, 1) != 1)
                return ccnt ? acnt : EOF;
        }

        /*
         * Process string.
         */
        if (ct == CT_STRING) {
            if (width == 0)
                width = 0xFFFF;
            if (flags & CF_SUPPRESS) {
                while (!isspace(ch)) {
                    if (--width == 0)
                        break;
                    if (_getb(fd, &ch, 1) != 1)
                        break;
                }
            } else {
                cp = va_arg(ap, char *);
                while (!isspace(ch)) {
                    *cp++ = ch;
                    if (--width == 0)
                        break;
                    if (_getb(fd, &ch, 1) != 1)
                        break;
                }
                *cp = 0;
                acnt++;
            }
            ccnt++;
        }

        /*
         * Process integer.
         */
        else if (ct == CT_INT) {
            if (width == 0 || width > sizeof(buf) - 1)
                width = sizeof(buf) - 1;

            flags |= CF_SIGNOK | CF_NDIGITS | CF_NZDIGITS;

            for (cp = buf; width; width--) {
                if (ch == '0') {
                    if (base == 0) {
                        base = 8;
                        flags |= CF_PFXOK;
                    }
                    if (flags & CF_NZDIGITS)
                        flags &= ~(CF_SIGNOK | CF_NZDIGITS | CF_NDIGITS);
                    else
                        flags &= ~(CF_SIGNOK | CF_PFXOK | CF_NDIGITS);
                } else if (ch >= '1' && ch <= '7') {
                    if (base == 0)
                        base = 10;
                    flags &= ~(CF_SIGNOK | CF_PFXOK | CF_NDIGITS);
                } else if (ch == '8' || ch == '9') {
                    if (base == 0)
                        base = 10;
                    else if (base <= 8)
                        break;
                    flags &= ~(CF_SIGNOK | CF_PFXOK | CF_NDIGITS);
                } else if ((ch >= 'A' && ch <= 'F')
                           || (ch >= 'a' && ch <= 'f')) {
                    if (base <= 10)
                        break;
                    flags &= ~(CF_SIGNOK | CF_PFXOK | CF_NDIGITS);
                } else if (ch == '-' || ch == '+') {
                    if ((flags & CF_SIGNOK) == 0)
                        break;
                    flags &= ~CF_SIGNOK;
                } else if (ch == 'x' || ch == 'X') {
                    if ((flags & CF_PFXOK) == 0)
                        break;
                    base = 16;
                    flags &= ~CF_PFXOK;
                } else {
                    ch_ready = 1; /* character avail without read */
                    break;
                }
                *cp++ = ch;
                if (width > 1) {
                    if (_getb(fd, &ch, 1) != 1)
                        break;
                }
            }

            if (flags & CF_NDIGITS)
                return acnt;

            if ((flags & CF_SUPPRESS) == 0) {
                if (sign) {
                    *cp = 0;
#if defined( __AVR_ARCH__)
                    long res;
                    res = strtol(buf, 0, base);
                    if (flags & CF_LONGLONG)
                        *va_arg(ap, long long *) = res;
                    else if (flags & CF_LONG)
                        *va_arg(ap, long *) = res;
                    else if (hcnt == 1)
                        *va_arg(ap, short *) = res;
                    else if (hcnt)
                        *va_arg(ap, char *) = res;
                    else
                        *va_arg(ap, int *) = res;
#else
                    long long res;
                    res = strtoll(buf, 0, base);
                    if (flags & CF_LONGLONG)
                        *va_arg(ap, long long *) = res;
                    else if (flags & CF_LONG)
                        *va_arg(ap, long *) = res;
                    else if (hcnt == 1)
                        *va_arg(ap, short *) = res;
                    else if (hcnt)
                        *va_arg(ap, char *) = res;
                    else
                        *va_arg(ap, int *) = res;
#endif
                    acnt++;
                } else {
                    *cp = 0;
#if defined( __AVR_ARCH__)
                    unsigned long res;
                    res = strtoul(buf, 0, base);
                    if (flags & CF_LONGLONG)
                        *va_arg(ap, unsigned long long *) = res;
                    else if (flags & CF_LONG)
                        *va_arg(ap, unsigned long *) = res;
                    else if (hcnt == 1)
                        *va_arg(ap, unsigned short *) = res;
                    else if (hcnt)
                        *va_arg(ap, char *) = res;
                    else
                        *va_arg(ap, unsigned int *) = res;
#else
                    unsigned long long res;
                    res = strtoull(buf, 0, base);
                    if (flags & CF_LONGLONG)
                        *va_arg(ap, unsigned long long *) = res;
                    else if (flags & CF_LONG)
                        *va_arg(ap, unsigned long *) = res;
                    else if (hcnt == 1)
                        *va_arg(ap, unsigned short *) = res;
                    else if (hcnt)
                        *va_arg(ap, char *) = res;
                    else
                        *va_arg(ap, unsigned int *) = res;
#endif
                    acnt++;
                }
            }
            ccnt++;
        }
        else if (ct == CT_FLOAT) {
            if (width == 0 || width > sizeof(buf) - 1)
                width = sizeof(buf) - 1;
            flags |= CF_SIGNOK | CF_NDIGITS | CF_DPTOK | CF_EXPOK;
            for (cp = buf; width; width--) {
                if (ch >= '0' && ch <= '9')
                    flags &= ~(CF_SIGNOK | CF_NDIGITS);
                else if (ch == '+' || ch == '-') {
                    if ((flags & CF_SIGNOK) == 0)
                        break;
                    flags &= ~CF_SIGNOK;
                } else if (ch == '.') {
                    if ((flags & CF_DPTOK) == 0)
                        break;
                    flags &= ~(CF_SIGNOK | CF_DPTOK);
                } else if (ch == 'e' || ch == 'E') {
                    if ((flags & (CF_NDIGITS | CF_EXPOK)) != CF_EXPOK)
                        break;
                    flags = (flags & ~(CF_EXPOK | CF_DPTOK)) | CF_SIGNOK | CF_NDIGITS;
                } else {
                    ch_ready = 1; /* character avail without read */
                    break;
                }
                *cp++ = ch;
                if (_getb(fd, &ch, 1) != 1)
                    break;
            }
            if (flags & CF_NDIGITS) {
                if (flags & CF_EXPOK)
                    return acnt;
            }
            if ((flags & CF_SUPPRESS) == 0) {
                double res;

                *cp = 0;
#ifdef STDIO_FLOATING_POINT
                res = strtod(buf, 0);
#else
		res = strtod_simple(buf, 0);
#endif                          /* STDIO_FLOATING_POINT */
                if (flags & CF_LONGLONG)
                    *va_arg(ap, long double *) = res;
                else if (flags & CF_LONG)
                    *va_arg(ap, double *) = res;
                else
                    *va_arg(ap, float *) = res;
                acnt++;
            }
            ccnt++;
        }
    }
}

/*@}*/
