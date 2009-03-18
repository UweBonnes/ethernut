#ifndef _ARCH_AVR32_H_
#define _ARCH_AVR32_H_

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
* $Log: avr32.h,v $
*
*/

#include <cfg/arch.h>

/* AVR32 is big endian core */
#if !defined(__BIG_ENDIAN__)
#define __BIG_ENDIAN__
#endif

#ifdef __GNUC__
# define CONST      const
# define INLINE     inline
#else
# ifndef CONST
#  define CONST      const
# endif
# ifndef INLINE
#  define INLINE
# endif
#endif

#define PSTR(p)    (p)
#define PRG_RDB(p) (*((const char *)(p)))

#define prog_char  const char
#define PGM_P      prog_char *

#define SIGNAL(x)  __attribute__((__interrupt__)) void x(void)
#define RAMFUNC __attribute__ ((long_call, section (".ramfunc")))

/* -------------------------------------------------------------------------
* redefine main
* ------------------------------------------------------------------------- */
#if !defined(__cplusplus)
# define main       NutAppMain
#endif

#define strlen_P(x)             strlen((char *)(x))
#define strcpy_P(x,y)           strcpy(x,(char *)(y))

#define strcmp_P(x, y)          strcmp((char *)(x), (char *)(y))
#define memcpy_P(x, y, z)       memcpy(x, y, z)

#ifndef __ASSEMBLER__
/*!
* \brief End of uninitialized data segment. Defined in the linker script.
*/
extern void *__bss_end;

#endif

#ifndef _NOP
# ifdef __GNUC__
#  define _NOP() __asm__ __volatile__ ("nop")
# else
#  define _NOP() asm("nop")
# endif
#endif

#define outb(_reg, _val)  (*((volatile unsigned char *)(_reg)) = (_val))
#define outw(_reg, _val)  (*((volatile unsigned short *)(_reg)) = (_val))
#define outr(_reg, _val)  (*((volatile unsigned long *)(_reg)) = (_val))

#define inb(_reg)   (*((volatile unsigned char *)(_reg)))
#define inw(_reg)   (*((volatile unsigned short *)(_reg)))
#define inr(_reg)   (*((volatile unsigned long *)(_reg)))

#define _BV(bit)    (1 << (bit))

#ifdef __IMAGECRAFT__
# define __attribute__(x)
#endif

#define _SFR_MEM8(addr)     (addr)
#define _SFR_MEM16(addr)    (addr)

/*! 
 * \brief Gets the value of the \a sysreg system register.
 *
 * \param sysreg  Address of the system register of which to get the value.
 *
 * \return Value of the \a sysreg system register.
 */
#if __GNUC__
# define Get_system_register(sysreg)         __builtin_mfsr(sysreg)
#elif __ICCAVR32__
# define Get_system_register(sysreg)         __get_system_register(sysreg)
#endif

/*! 
 * \brief Sets the value of the \a sysreg system register to \a value.
 *
 * \param sysreg  Address of the system register of which to set the value.
 * \param value   Value to set the \a sysreg system register to.
 */
#if __GNUC__
# define Set_system_register(sysreg, value)  __builtin_mtsr(sysreg, value)
#elif __ICCAVR32__
# define Set_system_register(sysreg, value)  __set_system_register(sysreg, value)
#endif


#endif // _ARCH_AVR32_H_
