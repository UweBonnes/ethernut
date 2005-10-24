#ifndef _ARCH_AVR_GCC_H_
#define _ARCH_AVR_GCC_H_

/*
 * Copyright (C) 2001-2005 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.1  2005/10/24 10:46:05  haraldkipp
 * First check in.
 * Contents taken from avr.h in the parent directory.
 *
 */


/* ================================================================ */
/* To be sorted out.                                                */
/* ================================================================ */


#define CONST   const
#define INLINE  inline


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <stdlib.h>


/*
 * test for a macro added in avr-libc 1.2.0, if yes use different path for twi.h
 * note: has to be after #include <eeprom.h>
 */
#ifdef eeprom_rb
#include <avr/twi.h>
#else
#include <compat/twi.h>
#endif

#ifndef __SFR_OFFSET
#define __SFR_OFFSET    0
#endif

#define COMPRESS_DISABLE
#define COMPRESS_REENABLE

#ifndef _NOP
#define _NOP() __asm__ __volatile__ ("nop")
#endif

#ifndef atof
#define atof(s)	    strtod(s, 0)
#endif

#define EEPROMReadBytes(addr, ptr, size)    eeprom_read_block((char *)(addr), ptr, size)
/*!
 * \brief Read multibyte types from the EEPROM.
 */
#define EEPROM_READ(addr, dst)		    eeprom_read_block((char *)(addr), &dst, sizeof(dst))
#define EEPROMread(addr)	 	    eeprom_read_byte((char *)(addr))

/*!
 * \brief Write multibyte types to the EEPROM.
 */
#define EEPROM_WRITE(addr, src)							\
{										\
    unsigned short __i;								\
    for(__i = 0; __i < sizeof(src); __i++)					\
	eeprom_write_byte(((char *)(addr)) + __i, *(((char *)(&(src))) + __i)); \
}

#define EEPROMWriteBytes(addr, ptr, size)					\
{										\
    unsigned short __i;								\
    for(__i = 0; __i < size; __i++)						\
	eeprom_write_byte(((char *)(addr)) + __i, *(((char *)(ptr)) + __i));	\
}

#define main    NutAppMain

#endif /* _ARCH_AVR_GCC_H_ */

