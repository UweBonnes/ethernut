#ifndef _COMPILER_H_
#define _COMPILER_H_

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
 * Revision 1.5  2004/02/01 18:49:47  haraldkipp
 * Added CPU family support
 *
 */

#ifdef  __cplusplus
# define __BEGIN_DECLS  extern "C" {
# define __END_DECLS    }
#else
# define __BEGIN_DECLS
# define __END_DECLS
#endif

/*!
 * \addtogroup xgCrtMisc
 */
/*@{*/

/*! 
 * \brief Unsigned register type. 
 *
 * The size of this type is equal to the size of a register, 
 * the hardware datapath or whatever might fit to give optimum 
 * performance for values from 0 to 255.
 *
 * Typically 8 bit CPUs will use unsigned characters, 16 bit
 * CPUs will use unsigned shorts etc.
 */
typedef unsigned char ureg_t;

/*! 
 * \brief Unsigned register type. 
 *
 * Similar to ureg_t, but for signed values from -128 to +127.
 */
typedef signed char reg_t;

/*! 
 * \brief Unsigned pointer value type. 
 *
 * The size of this type is at least the size of a memory pointer.
 * For CPUs with 16 address bits this will be an unsigned short.
 */
typedef unsigned short uptr_t;

#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega103__)
#include <cpu/avr.h>
#elif defined(__arm__)
#include <cpu/arm.h>
#elif defined(__H8300__)
#include <cpu/h8.h>
#elif defined(__m68k__)
#include <cpu/m68k.h>
#endif


/*@}*/

#endif
