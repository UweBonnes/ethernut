#ifndef _ARCH_AVR_H_
#define _ARCH_AVR_H_

/*
 * Copyright (C) 2001-2004 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.3  2004/03/18 09:57:01  haraldkipp
 * Architecture required in UserConf.mk
 *
 * Revision 1.2  2004/03/17 14:54:00  haraldkipp
 * Compiling for AVR works again
 *
 * Revision 1.1  2004/03/16 16:48:28  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1  2004/02/01 18:49:47  haraldkipp
 * Added CPU family support
 *
 */

#ifdef __IMAGECRAFT__

#include <macros.h>
#include <eeprom.h>

/*
 * HACK ALERT. Forces ICCAVR to use our routines.
 */
#define printf		printf_M
#define puts		puts_M
#define cprintf		printf_P
#define csprintf	sprintf_P
#define sprintf		sprintf_M
#define vprintf		vprintf_M
#define scanf		scanf_M
#define gets		gets_M
#define malloc		malloc_M
#define free		free_M

#ifndef CONST
#define CONST
#endif

#ifndef INLINE
#define INLINE
#endif

#define __heap_start   _bss_end

#define __attribute__(x)

#define PSTR(p)     (p)
#define PRG_RDB(p)  (*((const char *)(p)))

#define prog_char const char
#define PGM_P prog_char *

#define prog_int const int

#define strlen_P(x)         cstrlen(x)
#define strcpy_P(x,y)       cstrcpy(x,y)
#define strncpy_P(x,y,z)    cstrncpy(x,y,z)

/* This will only work for equal/not equal comparisions. */
#define strcmp_P(x, y)      cstrcmp(y, x)

#define memcpy_P(dst, src, size)    \
{                                   \
    u_short _i;                     \
    u_char *d = dst;                \
    const u_char *s = src;          \
    for(_i = 0; _i < size; _i++)    \
        *d++ = *s++ ;               \
}

/* FIXME */
#define strcasecmp(s1, s2)  strcmp(s1, s2)
/* FIXME */
#define strncasecmp(s1, s2, n)  strncmp(s1, s2, n)

#define __SFR_OFFSET 0
#define SFR_IO_ADDR(sfr) ((sfr) - __SFR_OFFSET)
#define SFR_MEM_ADDR(sfr) (sfr)
#define SFR_IO_REG_P(sfr) ((sfr) < 0x40 + __SFR_OFFSET)

#define BV(x)       BIT(x)
#define _BV(x)      BIT(x)

#define cli()           CLI()
#define sei()           SEI()
#define cbi(reg, bit)   (reg &= ~BIT(bit))
#define sbi(reg, bit)   (reg |= BIT(bit))


#define loop_until_bit_is_set(reg, bit) while((reg & BIT(bit)) == 0)

#define bit_is_clear(reg, bit)  ((reg & BIT(bit)) == 0)
#define bit_is_set(reg, bit)    ((reg & BIT(bit)) != 0)

/* FIXME */
#define parity_even_bit(x)  (0)

/* FIXME */
#define SIGNAL(x)   void x(void)

#define outp(val, reg)  (reg = val)
#define outb(reg, val)  (reg = val)

#define inp(reg)        (reg)
#define inb(reg)        (reg)

#include <eeprom.h>

#ifdef _MCU_enhanced

#include <iom128v.h>
#define __AVR_ATmega128__

#ifndef RAMEND
#define RAMEND  0x10FF
#endif

#ifndef SRW
#define SRW  6
#endif

/* Master */
#define TW_START		0x08
#define TW_REP_START		0x10
/* Master Transmitter */
#define TW_MT_SLA_ACK		0x18
#define TW_MT_SLA_NACK		0x20
#define TW_MT_DATA_ACK		0x28
#define TW_MT_DATA_NACK		0x30
#define TW_MT_ARB_LOST		0x38
/* Master Receiver */
#define TW_MR_ARB_LOST		0x38
#define TW_MR_SLA_ACK		0x40
#define TW_MR_SLA_NACK		0x48
#define TW_MR_DATA_ACK		0x50
#define TW_MR_DATA_NACK		0x58
/* Slave Transmitter */
#define TW_ST_SLA_ACK		0xA8
#define TW_ST_ARB_LOST_SLA_ACK	0xB0
#define TW_ST_DATA_ACK		0xB8
#define TW_ST_DATA_NACK		0xC0
#define TW_ST_LAST_DATA		0xC8
/* Slave Receiver */
#define TW_SR_SLA_ACK		0x60
#define TW_SR_ARB_LOST_SLA_ACK	0x68
#define TW_SR_GCALL_ACK		0x70
#define TW_SR_ARB_LOST_GCALL_ACK 0x78
#define TW_SR_DATA_ACK		0x80
#define TW_SR_DATA_NACK		0x88
#define TW_SR_GCALL_DATA_ACK	0x90
#define TW_SR_GCALL_DATA_NACK	0x98
#define TW_SR_STOP		0xA0
/* Misc */
#define TW_NO_INFO		0xF8
#define TW_BUS_ERROR		0x00


#else                           /* ATmega103 */

#include <iom103v.h>

#ifndef DOR
#define DOR  OVR
#endif

#ifndef RAMEND
#define RAMEND  0x0FFF
#endif

#endif

#define eeprom_read_block(dst, addr, size)  EEPROMReadBytes((int)addr, dst, size)
#define eeprom_write_byte(addr, src)        EEPROMwrite((int)addr, src)
#define eeprom_read_byte(addr)              EEPROMread((int)addr)

#else                           /* GCC -------------------------------------------------------------- */

#define CONST   const
#define INLINE  inline

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/twi.h>

#ifndef __SFR_OFFSET
#define __SFR_OFFSET    0
#endif

#define COMPRESS_DISABLE
#define COMPRESS_REENABLE

#ifndef _NOP
#define _NOP() __asm__ __volatile__ ("nop")
#endif

#define SEI()	    sei()
#define CLI()	    cli()
#define atof(s)	    strtod(s, 0)

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

#endif                          /* GCC */

#define __bss_end	__heap_start
extern void *__heap_start;

#ifdef __AVR_ATmega128__
#define RAMSTART    ((void *)0x100)

/* Nut/OS is still using the original ATmega103 register names for
   backward compatibility. */
#ifndef UDR
#define UDR     UDR0
#endif
#ifndef UBRR
#define UBRR    UBRR0L
#endif
#ifndef USR
#define USR     UCSR0A
#endif
#ifndef UCR
#define UCR     UCSR0B
#endif
#ifndef EICR
#define EICR    EICRB
#endif
#ifndef RXC
#define RXC     RXC0
#endif
#ifndef UDRE
#define UDRE    UDRE0
#endif
#ifndef FE
#define FE      FE0
#endif
#ifndef DOR
#define DOR     DOR0
#endif
#ifndef RXCIE
#define RXCIE   RXCIE0
#endif
#ifndef TXCIE
#define TXCIE   TXCIE0
#endif
#ifndef UDRIE
#define UDRIE   UDRIE0
#endif
#ifndef RXEN
#define RXEN    RXEN0
#endif
#ifndef TXEN
#define TXEN    TXEN0
#endif

#else
#define RAMSTART    ((void *)0x60)

#endif

#endif
