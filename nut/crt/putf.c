/*
 * Copyright (C) 2001-2005 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.18  2009/03/05 22:58:05  freckle
 * don't follow __arm__ if __NUT_EMULATION__
 *
 * Revision 1.17  2009/02/13 14:52:05  haraldkipp
 * Include memdebug.h for heap management debugging support.
 *
 * Revision 1.16  2009/01/17 11:26:38  haraldkipp
 * Getting rid of two remaining BSD types in favor of stdint.
 * Replaced 'u_int' by 'unsinged int' and 'uptr_t' by 'uintptr_t'.
 *
 * Revision 1.15  2008/08/11 06:59:40  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.14  2008/07/08 13:26:58  haraldkipp
 * Floating point bug with avr-libc 1.16 fixed (bug #1871390).
 *
 * Revision 1.13  2008/06/28 07:49:33  haraldkipp
 * Added floating point support for stdio running on ARM.
 *
 * Revision 1.12  2008/04/18 13:22:26  haraldkipp
 * Added type casts to fix ICCAVR V7.16 compile errors.
 *
 * Revision 1.11  2005/04/19 10:21:30  haraldkipp
 * Support for size_t modifier added. Thanks to Tom Lynn
 *
 * Revision 1.10  2004/11/24 15:24:07  haraldkipp
 * Floating point configuration works again.
 *
 * Revision 1.9  2004/08/18 16:30:05  haraldkipp
 * Compile error on non-Harvard architecture fixed
 *
 * Revision 1.8  2004/03/16 16:48:27  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.7  2003/12/17 14:33:24  drsung
 * Another bug fix for putf. Thanks to Dusan Ferbas.
 *
 * Revision 1.6  2003/12/12 23:14:11  drsung
 * Rewritten %P handling for program space strings
 *
 * Revision 1.5  2003/12/12 20:23:17  drsung
 * Fixed %P handling
 *
 * Revision 1.4  2003/11/26 12:45:20  drsung
 * Portability issues ... again
 *
 * Revision 1.3  2003/11/24 18:21:50  drsung
 * Added support for program space strings (%P)
 *
 * Revision 1.2  2003/08/14 15:21:51  haraldkipp
 * Formatted output of unsigned int fixed
 *
 * Revision 1.1.1.1  2003/05/09 14:40:32  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.1  2003/02/04 17:49:08  harald
 * *** empty log message ***
 *
 */

#include <cfg/crt.h>
#include <toolchain.h>
#include <string.h>
#include "nut_io.h"
#include <stdlib.h>
#include <memdebug.h>

/*!
 * \addtogroup xgCrtStdio
 */
/*@{*/

#ifdef STDIO_FLOATING_POINT

#include <math.h>

#if defined(__arm__)
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
#endif                          /* STDIO_FLOATING_POINT */

#define BUF 24
#define DEFPREC 6
#define PADSIZE 16

static const char blanks[PADSIZE] PROGMEM = {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};
static const char zeroes[PADSIZE] PROGMEM = {
    '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0'
};

static const char HEXDIGIT[32] PROGMEM = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};


/*
 *
 */
#ifdef __HARVARD_ARCH__
static void _putpad(int _putb(int fd, PGM_P, size_t), int fd, PGM_P padch, int count)
#else
static void _putpad(int _putb(int fd, const void *, size_t), int fd, const char *padch, int count)
#endif
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
#define ALT     0x01    /* alternate form */
#define LADJUST     0x04    /* left adjustment */
#define LONGINT     0x08    /* long integer */
#define ZEROPAD     0x10    /* zero (as opposed to blank) pad */
#define LONGLONG    0x20    /* long long integer*/
#define UNSIGNED    0x40    /* unsigned integer*/

/* va_arg(...) fetches at least an integer argument from the stack.
 * int may be 2 byte on AVR8, 4 Bytes or more.*/
#ifdef STDIO_64_BIT
#define ULTYPE uint64_t
#define LLTYPE long long
uint64_t va_args_ulval(int flags, va_list *ap)
{
    uint64_t result;
    if (flags & LONGLONG)
        result = va_arg(*ap, uint64_t);
    else if (flags & UNSIGNED)
        result = (uint64_t)va_arg(*ap, unsigned int);
    else if (flags & LONGINT)
        result = (uint64_t)va_arg(*ap, int32_t);
    else
        result = (uint64_t)va_arg(*ap, int);
    return result;
}
#else
#define ULTYPE uint32_t
#define LLTYPE long
uint32_t va_args_ulval(int flags, va_list *ap)
{
    uint32_t result;
    if (flags & LONGINT)
        result = va_arg(*ap, uint32_t);
    else if (flags & UNSIGNED)
        result = (uint32_t)va_arg(*ap, unsigned int);
    else
        result = (uint32_t)va_arg(*ap, int);
    return result;
}
#endif

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
int _putf(int _putb(int, const void *, size_t),
#ifdef __HARVARD_ARCH__
          int _putb_P (int, PGM_P, size_t),
#endif
          int fd, const char *fmt, va_list ap)
{
    uint8_t ch;                 /* character from fmt */
    int n;                      /* handy integer (short term usage) */
    char *cp;                   /* handy char pointer (short term usage) */
    uint8_t flags;              /* flags as above */
    int rc;                     /* return value accumulator */
    int width;                  /* width from format (%8d), or 0 */
    int prec;                   /* precision from format (%.3d), or -1 */
    int dprec;                  /* a copy of prec if [diouxX], 0 otherwise */
    int realsz;                 /* field size expanded by dprec, sign, etc */
    uint8_t sign;               /* sign prefix (' ', '+', '-', or \0) */
    ULTYPE ulval;             /* long integer arguments %[diouxX] */
    int size;                   /* size of converted field or string */
#ifdef __HARVARD_ARCH__
    char *xdigs;                /* for %P malloc'd string*/
#endif
    PGM_P hexdigs;              /* digits for [xX] conversion */
    char buf[BUF];              /* space for %c, %[diouxX], %[eEfgG] */

#ifdef STDIO_FLOATING_POINT
    double _double;             /* double precision arguments %[eEfgG] */

#ifdef __IMAGECRAFT__
    int iccfmt;
    int fps;
    extern char *FormatFP_1(int format, float f, unsigned flag, int field_width, int prec);
    extern char *ftoa(float f, int *status);
#else /* __IMAGECRAFT__ */
    char *dtostre(double f, char *str, uint8_t prec, uint8_t flags);
#if __AVR_LIBC_VERSION__  >= 10600
    char *dtostrf(double f, signed char width, unsigned char prec, char *str);
#else /* __AVR_LIBC_VERSION__ */
    char *dtostrf(double f, char width, char prec, char *str);
#endif /* __AVR_LIBC_VERSION__ */
#endif /* __IMAGECRAFT__ */

#endif /* STDIO_FLOATING_POINT */

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
            else if (ch == 'l') {
                if(*fmt == 'l') {
                    flags |= LONGLONG;
                    ch = *fmt++;
                }
                else
                    flags |= LONGINT;
            }
            else if (ch == 'h') { /* Ignore */
                if(*fmt == 'h') {
                    ch = *fmt++;
                }
            }
            else if (ch == 'z') {
                if (sizeof(size_t) > sizeof(int)) {
                    flags |= LONGINT;
                }
            }
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

        case 'P':
#ifdef __HARVARD_ARCH__
            /*
             * Thanks to Ralph Mason and Damian Slee, who provided some ideas of
             * handling prog_char strings
             */
            cp = va_arg(ap, char *);    /* retrieve pointer */
            if (cp == 0) {      /* if NULL pointer jump to std %s handling */
                ch = 's';       /* manipulate ch, so 'free' is later not called */
                goto putf_s;
            }
            size = strlen_P((PGM_P)cp);        /* get length of string */
            xdigs = malloc(size + 1);   /* allocate buffer to store string */
            strcpy_P(xdigs, (PGM_P)cp);        /* copy the string to RAM */
            cp = xdigs;         /* use cp for further processing */
            goto putf_s;        /* jump to std %s handling */
#endif /* __HARVARD_ARCH__ */

        case 's':
            cp = va_arg(ap, char *);

#ifdef __HARVARD_ARCH__
          putf_s:
#endif /* __HARVARD_ARCH__ */

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
            flags |= UNSIGNED;
        case 'd':
        case 'i':
            /* Thanks to Ralph Mason for fixing the u_int bug. */
#ifndef STDIO_64_BIT
            if (flags & LONGLONG) {
                (void)va_arg(ap, uint64_t);
                strcpy_P(buf, PSTR("NA"));
                cp = buf;
                size = strlen("NA");
                break;
            }
#endif
            ulval = va_args_ulval(flags, &ap);
            if (ch != 'u' && ( LLTYPE ) ulval < 0) {
                ulval = ( ULTYPE ) (-(( LLTYPE) ulval));
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
#ifndef STDIO_64_BIT
            if (flags & LONGLONG) {
                (void)va_arg(ap, uint64_t);
                strcpy_P(buf, PSTR("NA"));
                cp = buf;
                size = strlen("NA");
                break;
            }
#endif
            ulval = va_args_ulval(flags, &ap);
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
            flags |= UNSIGNED;
#ifndef STDIO_64_BIT
            if (flags & LONGLONG) {
                (void)va_arg(ap, uint64_t);
                strcpy_P(buf, PSTR("NA"));
                cp = buf;
                size = strlen("NA");
                break;
            }
#endif
            if (ch == 'p') {
                ulval = (uintptr_t) va_arg(ap, void *);
                flags |= ALT;
                ch = 'x';
            } else
                ulval = va_args_ulval(flags, &ap);

            sign = 0;
            if ((dprec = prec) >= 0)
                flags &= ~ZEROPAD;

            hexdigs = HEXDIGIT;
            if (ch == 'x')
                hexdigs += 16;

            cp = buf + BUF;
            do {
                *--cp = PRG_RDB(hexdigs + (ulval & 0x0f));
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
#elif defined(__arm__) && !defined(__NUT_EMULATION__)
        case 'g':
        case 'G':
        case 'e':
        case 'E':
        case 'f':
            {
                int decpt;
                int neg;
                char *rve = buf;
                char *bp = buf;

                if (prec == -1)
                    prec = DEFPREC;
                _double = va_arg(ap, double);
                cp = _dtoa_r(_REENT, _double, 3, prec, &decpt, &neg, &rve);
                if (neg)
                    sign = '-';
                if (decpt == 9999) {
                    /* Infinite or invalid. */
                    strcpy(bp, cp);
                } else {
                    /* Left of decimal dot. */
                    if (decpt > 0) {
                        while (*cp && decpt > 0) {
                            *bp++ = *cp++;
                            decpt--;
                        }
                        while (decpt > 0) {
                            *bp++ = '0';
                            decpt--;
                        }
                    } else {
                        *bp++ = '0';
                    }
                    *bp++ = '.';
                    /* Right of decimal dot. */
                    while (decpt < 0 && prec > 0) {
                        *bp++ = '0';
                        decpt++;
                        prec--;
                    }
                    while (*cp && prec > 0) {
                        *bp++ = *cp++;
                        prec--;
                    }
                    while (prec > 0) {
                        *bp++ = '0';
                        prec--;
                    }
                    *bp = 0;
                }
                cp = buf;
            }
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
#endif
#else
        case 'g':
        case 'G':
        case 'e':
        case 'E':
        case 'f':
            (void) va_arg(ap, double);
            strcpy_P(buf, PSTR("NA"));
            cp = buf;
#endif                          /* STDIO_FLOATING_POINT */
            size = strlen(buf);
            break;
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

#ifdef __HARVARD_ARCH__
        if ((flags & (LADJUST | ZEROPAD)) == 0)
            _putpad(_putb_P, fd, blanks, width - realsz);

        if (sign)
            _putb(fd, &sign, 1);

        if ((flags & (LADJUST | ZEROPAD)) == ZEROPAD)
            _putpad(_putb_P, fd, zeroes, width - realsz);

        _putpad(_putb_P, fd, zeroes, dprec - size);

        if (size)       /* DF 12/16/03 - zero length is "flush" in NutTcpDeviceWrite() */
            _putb(fd, cp, size);

        if (ch == 'P')
            free(cp);

        if (flags & LADJUST)
            _putpad(_putb_P, fd, blanks, width - realsz);
#else
        if ((flags & (LADJUST | ZEROPAD)) == 0)
            _putpad(_putb, fd, blanks, width - realsz);

        if (sign)
            _putb(fd, &sign, 1);

        if ((flags & (LADJUST | ZEROPAD)) == ZEROPAD)
            _putpad(_putb, fd, zeroes, width - realsz);

        _putpad(_putb, fd, zeroes, dprec - size);

        if (size)       /* DF 12/16/03 - zero length is "flush" in NutTcpDeviceWrite() */
            _putb(fd, cp, size);

        if (flags & LADJUST)
            _putpad(_putb, fd, blanks, width - realsz);
#endif

        if (width >= realsz)
            rc += width;
        else
            rc += realsz;
    }
    return rc;
}

/*@}*/
