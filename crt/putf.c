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
 * Revision 1.1  2003/05/09 14:40:32  haraldkipp
 * Initial revision
 *
 * Revision 1.1  2003/02/04 17:49:08  harald
 * *** empty log message ***
 *
 */

#include <string.h>
#include "nut_io.h"

/*!
 * \addtogroup xgCrtStdio
 */
/*@{*/

#ifdef STDIO_FLOATING_POINT

#include <math.h>
#define	BUF	16
#define	DEFPREC	6

#else

#define	BUF	16

#endif                          /* STDIO_FLOATING_POINT */

#define	PADSIZE	16
static char blanks[PADSIZE] =
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
' ', ' ' };
static char zeroes[PADSIZE] =
    { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
'0', '0' };

/*
 *
 */
static void _putpad(int _putb(int fd, CONST void *, size_t), int fd,
                    char *padch, int count)
{
    while (count > PADSIZE) {
        _putb(fd, padch, PADSIZE);
        count -= PADSIZE;
    }
    if (count > 0)
        _putb(fd, padch, count);
}

/*
 * Flags used during conversion.
 */
#define	ALT		0x01    /* alternate form */
#define	LADJUST		0x04    /* left adjustment */
#define	LONGINT		0x08    /* long integer */
#define	ZEROPAD		0x10    /* zero (as opposed to blank) pad */

/*!
 * \brief Write formatted data using a given output function.
 *
 * \param _putb Output function for writing data.
 * \param fd    Descriptor of a previously opened file, device or
 *              connected socket.
 * \param fmt   Format string containing conversion specifications.
 * \param ap    List of arguments.
 *
 * 
 */
int _putf(int _putb(int, CONST void *, size_t), int fd, CONST char *fmt,
          va_list ap)
{
    u_char ch;                  /* character from fmt */
    int n;                      /* handy integer (short term usage) */
    char *cp;                   /* handy char pointer (short term usage) */
    u_char flags;               /* flags as above */
    int rc;                     /* return value accumulator */
    int width;                  /* width from format (%8d), or 0 */
    int prec;                   /* precision from format (%.3d), or -1 */
    int dprec;                  /* a copy of prec if [diouxX], 0 otherwise */
    int realsz;                 /* field size expanded by dprec, sign, etc */
    u_char sign;                /* sign prefix (' ', '+', '-', or \0) */
    u_long ulval;               /* integer arguments %[diouxX] */
    int size;                   /* size of converted field or string */
    char *xdigs;                /* digits for [xX] conversion */
    char buf[BUF];              /* space for %c, %[diouxX], %[eEfgG] */
#ifdef STDIO_FLOATING_POINT
    double _double;             /* double precision arguments %[eEfgG] */
#ifdef __IMAGECRAFT__
    int iccfmt;
    int fps;
    extern char *FormatFP_1(int format, float f, unsigned flag,
                            int field_width, int prec);
    extern char *ftoa(float f, int *status);
#else
    char *dtostre(double f, char *str, u_char prec, u_char flags);
    char *dtostrf(double f, char width, char prec, char *str);
#endif
#endif

    rc = 0;

    for (;;) {

        /*
         * Print format string until next percent sign.
         */
        for (cp = (char *) fmt; (ch = *fmt) != 0 && ch != '%'; fmt++);
        if ((n = fmt - cp) != 0) {
            _putb(fd, cp, n);
            rc += n;
        }
        if (ch == 0)
            break;
        fmt++;

        /*
         * Process modifiers.
         */
        flags = 0;
        sign = 0;
        width = 0;
        dprec = 0;
        prec = -1;
#if defined(STDIO_FLOATING_POINT) && defined(__IMAGECRAFT__)
        iccfmt = 0;
#endif
        for (;;) {
            ch = *fmt++;
            if (ch == ' ') {
                if (!sign)
                    sign = ' ';
            } else if (ch == '+')
                sign = '+';
            else if (ch == '-')
                flags |= LADJUST;
            else if (ch == '#')
                flags |= ALT;
            else if (ch == '0')
                flags |= ZEROPAD;
            else if (ch == 'l')
                flags |= LONGINT;
            else if (ch == '*') {
                width = va_arg(ap, int);
                if (width < 0) {
                    flags |= LADJUST;
                    width = -width;
                }
            } else if (ch == '.') {
                if (*fmt == '*') {
                    fmt++;
                    prec = va_arg(ap, int);
                } else {
                    prec = 0;
                    while (*fmt >= '0' && *fmt <= '9')
                        prec = 10 * prec + (*fmt++ - '0');
                }
                if (prec < 0)
                    prec = -1;
            } else if (ch >= '1' && ch <= '9') {
                width = ch - '0';
                while (*fmt >= '0' && *fmt <= '9')
                    width = 10 * width + (*fmt++ - '0');
            } else
                break;
        }

        /*
         * Process type field.
         */
        switch (ch) {
        case 'c':
            *(cp = buf) = va_arg(ap, int);
            size = 1;
            sign = 0;
            break;

        case 's':
            cp = va_arg(ap, char *);
            if (cp == 0)
                cp = "(null)";
            if (prec >= 0) {
                char *p = memchr(cp, 0, (size_t) prec);

                if (p) {
                    size = p - cp;
                    if (size > prec)
                        size = prec;
                } else
                    size = prec;
            } else
                size = strlen(cp);
            sign = 0;
            break;

        case 'u':
            sign = 0;
        case 'd':
        case 'i':
            ulval = (flags & LONGINT) ? va_arg(ap, long) : va_arg(ap, int);
            if (ch != 'u' && (long) ulval < 0) {
                ulval = (u_long) (-((long) ulval));
                sign = '-';
            }
            if ((dprec = prec) >= 0)
                flags &= ~ZEROPAD;
            cp = buf + BUF;
            if (ulval || prec) {
                if (ulval < 10)
                    *--cp = (char) ulval + '0';
                else
                    do {
                        *--cp = (char) (ulval % 10) + '0';
                        ulval /= 10;
                    } while (ulval);
            }
            size = buf + BUF - cp;
            break;

        case 'o':
            ulval =
                (flags & LONGINT) ? va_arg(ap, u_long) : va_arg(ap, u_int);
            sign = 0;
            if ((dprec = prec) >= 0)
                flags &= ~ZEROPAD;
            cp = buf + BUF;
            if (ulval || prec) {
                do {
                    *--cp = (char) (ulval & 7) + '0';
                    ulval >>= 3;
                } while (ulval);
                if ((flags & ALT) != 0 && *cp != '0')
                    *--cp = '0';
            }
            size = buf + BUF - cp;
            break;

        case 'p':
        case 'X':
        case 'x':
            if (ch == 'p') {
                ulval = (u_short) va_arg(ap, void *);
                flags |= ALT;
                ch = 'x';
            } else
                ulval =
                    (flags & LONGINT) ? va_arg(ap,
                                               u_long) : (u_long)
                    va_arg(ap, u_int);

            sign = 0;
            if ((dprec = prec) >= 0)
                flags &= ~ZEROPAD;

            if (ch == 'X')
                xdigs = "0123456789ABCDEF";
            else
                xdigs = "0123456789abcdef";

            cp = buf + BUF;
            do {
                *--cp = xdigs[ulval & 0x0f];
                ulval >>= 4;
            } while (ulval);
            if (flags & ALT) {
                *--cp = ch;
                *--cp = '0';
            }
            size = buf + BUF - cp;
            break;

#ifdef STDIO_FLOATING_POINT
#ifdef __IMAGECRAFT__
        case 'G':
            iccfmt++;
        case 'g':
            iccfmt++;
        case 'E':
            iccfmt++;
        case 'e':
            iccfmt++;
        case 'f':
            if (prec == -1)
                prec = DEFPREC;
            _double = va_arg(ap, double);
            /* ICCAVR bug, we use a hack */
            /* cp = FormatFP_1(iccfmt, _double, 0, 1, prec); */
            cp = ftoa(_double, &fps);
            size = strlen(cp);
            break;
#else
        case 'g':
        case 'G':
        case 'e':
        case 'E':
        case 'f':
            if (prec == -1)
                prec = DEFPREC;
            _double = va_arg(ap, double);
            if (ch == 'f')
                dtostrf(_double, 1, prec, buf);
            else
                dtostre(_double, buf, prec, 1);
            cp = buf;
            size = strlen(buf);
            break;
#endif
#else
        case 'g':
        case 'G':
        case 'e':
        case 'E':
        case 'f':
            (void) va_arg(ap, long);
#endif                          /* STDIO_FLOATING_POINT */

        default:
            if (ch == 0)
                return rc;
            cp = buf;
            *cp = ch;
            size = 1;
            sign = '\0';
            break;
        }                       /* switch */

        /*
         * Output.
         */
        realsz = dprec > size ? dprec : size;
        if (sign)
            realsz++;

        if ((flags & (LADJUST | ZEROPAD)) == 0)
            _putpad(_putb, fd, blanks, width - realsz);

        if (sign)
            _putb(fd, &sign, 1);

        if ((flags & (LADJUST | ZEROPAD)) == ZEROPAD)
            _putpad(_putb, fd, zeroes, width - realsz);

        _putpad(_putb, fd, zeroes, dprec - size);

        _putb(fd, cp, size);

        if (flags & LADJUST)
            _putpad(_putb, fd, blanks, width - realsz);

        if (width >= realsz)
            rc += width;
        else
            rc += realsz;
    }
    return rc;
}

/*@}*/
