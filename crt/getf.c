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
 * Revision 1.1  2003/05/09 14:40:29  haraldkipp
 * Initial revision
 *
 * Revision 1.1  2003/02/04 17:49:07  harald
 * *** empty log message ***
 *
 */

#include "nut_io.h"

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/*!
 * \addtogroup xgCrtStdio
 */
/*@{*/

#define	CF_LONG		0x01    /* 1: long or double */
#define	CF_SUPPRESS	0x02    /* suppress assignment */
#define	CF_SIGNOK	0x04    /* +/- is (still) legal */
#define	CF_NDIGITS	0x08    /* no digits detected */
#define	CF_PFXOK	0x10    /* 0x prefix is (still) legal */
#define	CF_NZDIGITS	0x20    /* no zero digits detected */
#define	CF_DPTOK	0x10    /* (float) decimal point is still legal */
#define	CF_EXPOK	0x20    /* (float) exponent (e+3, etc) still legal */

/*
 * Conversion types.
 */
#define	CT_CHAR		0       /* %c conversion */
#define	CT_STRING	2       /* %s conversion */
#define	CT_INT		3       /* integer, i.e., strtoq or strtouq */
#define	CT_FLOAT	4       /* floating, i.e., strtod */

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
int _getf(int _getb(int, void *, size_t), int fd, CONST char *fmt,
          va_list ap)
{
    u_char cf;                  /* Character from format. */
    u_char ch;                  /* Character from input. */
    size_t width;               /* Field width. */
    u_char flags;               /* CF_ flags. */
    u_char ct;                  /* CT_ conversion type. */
    u_char base;                /* Conversion base. */
    u_char ccnt = 0;            /* Number of conversions. */
    u_char acnt = 0;            /* Number of fields assigned. */
    char buf[16];               /* Temporary buffer. */
    char *cp;                   /* Temporary pointer. */

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
                if (!isspace(ch))
                    break;
            }
            continue;
        }

        /*
         * Match literals.
         */
        if (cf != '%') {
            if (_getb(fd, &ch, 1) != 1)
                return ccnt ? acnt : EOF;
            if (ch != cf)
                return acnt;
            continue;
        }

        /*
         * Collect modifiers.
         */
        width = 0;
        flags = 0;
        for (;;) {
            cf = *fmt++;
            if (cf == '%') {
                if (_getb(fd, &ch, 1) != 1)
                    return ccnt ? acnt : EOF;
                if (ch != cf)
                    return acnt;
                continue;
            }
            if (cf == '*')
                flags |= CF_SUPPRESS;
            else if (cf == 'l')
                flags |= CF_LONG;
            else if (cf >= '0' && cf <= '9')
                width = width * 10 + cf - '0';
            else
                break;
        }

        /*
         * Determine the types.
         */
        base = 10;
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
        case 'i':
            base = 0;
            break;
        case 'o':
            base = 8;
            break;
        case 'x':
            flags |= CF_PFXOK;
            base = 16;
            break;
#ifdef STDIO_FLOATING_POINT
        case 'e':
        case 'f':
        case 'g':
            ct = CT_FLOAT;
            break;
#endif
        }

        /*
         * Process characters.
         */
        if (ct == CT_CHAR) {
            if (width == 0)
                width = 1;
            if (flags & CF_SUPPRESS) {
                while (width > sizeof(buf)) {
                    if (_getb(fd, buf, sizeof(buf)) <= 0)
                        return ccnt ? acnt : EOF;
                    width -= sizeof(buf);
                }
                if (width)
                    if (_getb(fd, &buf, width) <= 0)
                        return ccnt ? acnt : EOF;
            } else {
                if (_getb(fd, (void *) va_arg(ap, char *), width) <= 0)
                     return ccnt ? acnt : EOF;
                acnt++;
            }
            ccnt++;
            continue;
        }

        /*
         * Skip whitespaces.
         */
        if (_getb(fd, &ch, 1) != 1)
            return ccnt ? acnt : EOF;
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
                } else
                    break;
                *cp++ = ch;
                if (_getb(fd, &ch, 1) != 1)
                    break;
            }

            if (flags & CF_NDIGITS)
                return acnt;

            if ((flags & CF_SUPPRESS) == 0) {
                u_long res;

                *cp = 0;
                res = strtol(buf, 0, base);
                if (flags & CF_LONG)
                    *va_arg(ap, long *) = res;
                else
                    *va_arg(ap, int *) = res;
                acnt++;
            }
            ccnt++;
        }
#ifdef STDIO_FLOATING_POINT
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
                    flags =
                        (flags & ~(CF_EXPOK | CF_DPTOK)) | CF_SIGNOK |
                        CF_NDIGITS;
                } else
                    break;
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
                res = strtod(buf, 0);
                *va_arg(ap, double *) = res;
                acnt++;
            }
            ccnt++;
        }
#endif                          /* STDIO_FLOATING_POINT */
    }
}

/*@}*/
