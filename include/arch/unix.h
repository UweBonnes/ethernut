#ifndef _ARCH_UNIX_H_
#define _ARCH_UNIX_H_

/*
 * Copyright (C) 2000-2004 by ETH Zurich
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
 * THIS SOFTWARE IS PROVIDED BY ETH ZURICH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH ZURICH
 *  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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
 * unix.h.c - types and defines for unix emulation
 *
 * 2004.04.01 Matthias Ringwald <matthias.ringwald@inf.ethz.ch>
 *
 */


#if !defined(__linux__) && !defined(__APPLE__)
#error "Nut/OS emulation currently runs only on Linux and MAC OS X systems."
#endif

/* -------------------------------------------------------------------------
 * map some AVR types/defines to something else
 * ------------------------------------------------------------------------- */

#define CONST      const
#define INLINE     inline

#define PSTR(p)    (p)
#define PRG_RDB(p) (*((const char *)(p)))

#define prog_char  const char
#define PGM_P      prog_char *

/* -------------------------------------------------------------------------
 * now we can include types (and time.h)
 * ------------------------------------------------------------------------- */

#include <sys/types.h>
#include <dev/mweeprom.h>


/* -------------------------------------------------------------------------
 * redefine main
 * ------------------------------------------------------------------------- */

#define main(...)       NutAppMain(__VA_ARGS__)

/* -------------------------------------------------------------------------
 * map harvard specific calls to normal ones
 * ------------------------------------------------------------------------- */


#define strlen_P(x)             strlen(x)
#define strcpy_P(x,y)           strcpy(x,y)
#define strcmp_P(x, y)          strcmp(x, y)
#define memcpy_P(x, y, z)       memcpy(x, y, z)

/* -------------------------------------------------------------------------
 * emulated heap
 * ------------------------------------------------------------------------- */

/* RAMSTART could be zero, but RAMSTART < 3 leads to a crash in freopen (stdout) */
#define RAMSTART    ((void *)0x00100)

/* on linux our malloc function makes the init section crash, so we better rename it */
#define malloc(...)		NUT_malloc(__VA_ARGS__)
#define free(...)		NUT_free(__VA_ARGS__)

/* -------------------------------------------------------------------------
 * parsing of command line options
 * ------------------------------------------------------------------------- */

/*
 * options of one uart 
 * usbnum: a negative usbnum is used to indicate that device name is used
 */

typedef struct {

    char *device;
    u_long bautrate;
    u_char flowcontrol;
    signed char usbnum;
} uart_options_t;

/* 
 * all command line options
 */
typedef struct {

    // debug output
    int verbose;

    uart_options_t uart_options[3];

} emulation_options_t;

/* the command line options are stored here */
extern emulation_options_t emulation_options;

/* -------------------------------------------------------------------------
 * function declarations
 * ------------------------------------------------------------------------- */
void emulation_options_parse(int argc, char *argv[]);


#endif                          /* #ifndef _CPU_UNIX_H_ */
