#ifndef _ARCH_CM3_H_
#define _ARCH_CM3_H_

/*
 * Copyright (C) 2001-2006 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.00  2010/08/06 09:34:34  ulrichprinz
 * Initial version.
 */

#include <cfg/arch.h>
#include <arch/cm3/cortexM3.h>

#ifndef __ASSEMBLER__
#include <dev/mweeprom.h>
#endif

#define ARM_MODE_USER       0x10
#define ARM_MODE_FIQ        0x11
#define ARM_MODE_IRQ        0x12
#define ARM_MODE_SVC        0x13
#define ARM_MODE_ABORT      0x17
#define ARM_MODE_UNDEF      0x1B
#define ARM_MODE_SYS        0x1F
#define ARM_MODE_MASK       0x1F

#define I_BIT               0x80
#define ARM_CPSR_I_BIT      0x80
#define F_BIT               0x40
#define ARM_CPSR_F_BIT      0x40
#define T_BIT               0x20
#define ARM_CPSR_T_BIT      0x20

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
#define RAMFUNC __attribute__ ((long_call, section (".ramfunc")))

#if !defined(__arm__) && !defined(__cplusplus)
#define main       NutAppMain
#endif

#define strlen_P(x)             strlen((char *)(x))
#define strcpy_P(x,y)           strcpy(x,(char *)(y))

#define strcmp_P(x, y)          strcmp((char *)(x), (char *)(y))
#define memcpy_P(x, y, z)       memcpy(x, y, z)

#ifndef __ASSEMBLER__
/*!
 * \brief End of uninitialised data segment. Defined in the linker script.
 */
extern void *__bss_end;

/*!
 * \brief Begin of the stack segment. Defined in the linker script.
 */
extern void *__stack;
#endif

#ifndef _NOP
#ifdef __GNUC__
#define _NOP() __asm__ __volatile__ ("mov r0, r0  @ _NOP")
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

#define _BV(bit)    (1 << (bit))

#define sbi(_reg, _bit)         outr(_reg, inr(_reg) | _BV(_bit))
#define cbi(_reg, _bit)         outr(_reg, inr(_reg) & ~_BV(_bit))
#define bit_is_set(_reg, _bit)  ((inr(_reg) & _BV(_bit)) != 0)

#ifdef __IMAGECRAFT__
#define __attribute__(x)
#endif

#define _SFR_MEM8(addr)     (addr)
#define _SFR_MEM16(addr)    (addr)

#ifndef __ASSEMBLER__
#ifdef __GNUC__
#define ARM_SET_CP15_CR(val) __asm__ __volatile__("mcr p15, 0, %0, c1, c0, 0" :: "r"(val) : "cc")
#define ARM_GET_CP15_CR() ( \
    { \
        unsigned int val; \
        __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 0" : "=r"(val) :: "cc"); \
        val; \
    } \
)
#endif /* __GNUC__ */
#endif /* __ASSEMBLER__ */

#if !defined (__ASSEMBLER__) && defined(__CROSSWORKS_ARM)
/*!
 * \brief Case insensitive string comparisions.
 *
 * Not supported by CrossWorks and temporarly redirected 
 * to the stricmp and strnicmp routines.
 *
 */
#define strcasecmp(s1, s2)      stricmp(s1, s2)
#define strncasecmp(s1, s2, n)  strnicmp(s1, s2, n)

/*
 * Not supported by CrossWorks, added prototypes here.
 */
int   stricmp(CONST char *s1, CONST char *s2);
int   strnicmp(CONST char *s1, CONST char *s2, size_t n);
char *strdup(CONST char *str);

/*
 * If "Enforce ANSI Checking" is enabled, which is
 * the default from the v2.x version of CrossWorks
 * the keyword asm will not be recognize. Therefore
 * the next define is needed to solve the problem.
 */
#define asm __asm__
#endif

#endif /* _CM3_H_ */
