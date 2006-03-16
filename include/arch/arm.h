#ifndef _ARCH_ARM_H_
#define _ARCH_ARM_H_

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
 * Revision 1.8  2006/03/16 15:25:26  haraldkipp
 * Changed human readable strings from u_char to char to stop GCC 4 from
 * nagging about signedness.
 *
 * Revision 1.7  2006/03/02 20:02:05  haraldkipp
 * Added a few macros to allow compilation with ICCARM.
 *
 * Revision 1.6  2006/02/23 15:34:00  haraldkipp
 * Support for Philips LPC2xxx Family and LPC-E2294 Board from Olimex added.
 * Many thanks to Michael Fischer for this port.
 *
 * Revision 1.5  2005/11/20 14:45:15  haraldkipp
 * Define printf_P for non Harvard architectures.
 *
 * Revision 1.4  2005/10/24 18:03:02  haraldkipp
 * GameBoy header file added.
 *
 * Revision 1.3  2005/10/24 10:35:05  haraldkipp
 * Port I/O macros added.
 *
 * Revision 1.2  2004/09/08 10:24:26  haraldkipp
 * RAMSTART is too platform dependant
 *
 * Revision 1.1  2004/03/16 16:48:28  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1  2004/02/01 18:49:47  haraldkipp
 * Added CPU family support
 *
 */

#include <cfg/arch.h>
#if defined (MCU_AT91R40008)
#include <arch/arm/at91.h>
#elif defined (MCU_GBA)
#include <arch/arm/gba.h>
#elif defined (MCU_LPC2XXX)
#include <arch/arm/lpc2xxx.h>
#endif

#include <dev/mweeprom.h>

#define ARM_MODE_USER       0x10
#define ARM_MODE_FIQ        0x11
#define ARM_MODE_IRQ        0x12
#define ARM_MODE_SVC        0x13
#define ARM_MODE_ABORT      0x17
#define ARM_MODE_UNDEF      0x1B
#define ARM_MODE_SYS        0x1F

#define I_BIT               0x80
#define F_BIT               0x40
#define T_BIT               0x20

#ifdef __GNUC__
#define CONST      const
#define INLINE     inline
#else
#ifndef CONST
#define CONST      const
#endif
#ifndef INLINE
#define INLINE
#endif
#endif

#define PSTR(p)    (p)
#define PRG_RDB(p) (*((const char *)(p)))

#define prog_char  const char
#define PGM_P      prog_char *

#define SIGNAL(x)  __attribute__((interrupt_handler)) void x(void)

#define main       NutAppMain

#define strlen_P(x)             strlen((char *)(x))
#define strcpy_P(x,y)           strcpy(x,(char *)(y))

#define strcmp_P(x, y)          strcmp((char *)(x), (char *)(y))
#define memcpy_P(x, y, z)       memcpy(x, y, z)

#define fputs_P(x, y)           fputs((char *)(x), y)
#define fprintf_P               fprintf
#define printf_P                printf

/*!
 * \brief End of uninitialised data segment. Defined in the linker script.
 */
extern void *__bss_end;

/*!
 * \brief Begin of the stack segment. Defined in the linker script.
 */
extern void *__stack;

#ifndef _NOP
#ifdef __GNUC__
#define _NOP() __asm__ __volatile__ ("mov r0, r0")
#else
#define _NOP() asm("mov r0, r0")
#endif
#endif

#define outb(_reg, _val)  (*((volatile unsigned char *)(_reg)) = (_val))
#define outw(_reg, _val)  (*((volatile unsigned short *)(_reg)) = (_val))
#define outr(_reg, _val)  (*((volatile unsigned int *)(_reg)) = (_val))

#define inb(_reg)   (*((volatile unsigned char *)(_reg)))
#define inw(_reg)   (*((volatile unsigned short *)(_reg)))
#define inr(_reg)   (*((volatile unsigned int *)(_reg)))

#define _BV(bit)    (1 << bit)

#ifdef __IMAGECRAFT__
#define __attribute__(x)
#endif

#endif
