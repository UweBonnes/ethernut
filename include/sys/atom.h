#ifndef _SYS_ATOM_H_
#define _SYS_ATOM_H_

/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.3  2004/03/17 11:30:22  haraldkipp
 * Bugfix for ICCAVR
 *
 * Revision 1.2  2004/03/16 16:48:28  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:18  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.6  2003/05/06 17:53:56  harald
 * Force critical inline
 *
 * Revision 1.5  2003/02/04 18:00:51  harald
 * Version 3 released
 *
 * Revision 1.4  2002/06/26 17:29:28  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/types.h>

__BEGIN_DECLS

#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega103__)

#ifdef __IMAGECRAFT__

#define AtomicInc(p)     (++(*p))
#define AtomicDec(p)     (--(*p))

#else

static inline void AtomicInc(volatile u_char *p)
{
    asm volatile(
        "in  __tmp_reg__, __SREG__" "\n\t"
        "cli"                       "\n\t"
        "ld r24, %a0"               "\n\t"
        "subi r24, lo8(-1)"         "\n\t"
        "st %a0, r24"               "\n\t"
        "out __SREG__, __tmp_reg__" "\n\t"
        :
        : "z" (p)
        : "r24"
    );
}

static inline void AtomicDec(volatile u_char *p)
{
    asm volatile(
        "in  __tmp_reg__, __SREG__" "\n\t"
        "cli"                       "\n\t"
        "ld r24, %a0"               "\n\t"
        "subi r24, lo8(1)"          "\n\t"
        "st %a0, r24"               "\n\t"
        "out __SREG__, __tmp_reg__" "\n\t"
        :
        : "z" (p)
        : "r24"
    );
}

#endif


#ifdef __IMAGECRAFT__

#define NutEnterCritical()  \
{                           \
    asm("in R0, 0x3F\n"     \
        "push R0\n"         \
        "cli\n");           \
}

#define NutExitCritical()   \
{                           \
    asm("pop R0\n"          \
        "out 0x3F, R0\n");  \
}

#else

#define NutEnterCritical()                  \
    asm volatile(                           \
        "in  __tmp_reg__, __SREG__" "\n\t"  \
        "push __tmp_reg__"          "\n\t"  \
        "cli"                       "\n\t"  \
    )

#define NutExitCritical()                   \
    asm volatile(                           \
        "pop __tmp_reg__"           "\n\t"  \
        "out __SREG__, __tmp_reg__" "\n\t"  \
    )

#endif

#define NutJumpOutCritical() NutExitCritical()

#elif defined(__H8300H__) || defined(__H8300S__)

#define AtomicInc(p)     (++(*p))
#define AtomicDec(p)     (--(*p))

#define NutEnterCritical()                     \
    {                                          \
        u_char __ccr__;                        \
        asm volatile(                          \
            "stc.b ccr, %0l"            "\n\t" \
            "orc.b #0xc0, ccr":"=r"(__ccr__):  \
        );

#define NutExitCritical()                      \
       asm volatile(                           \
        "ldc.b %0l, ccr"::"r"(__ccr__)         \
       );                                      \
    }

#define NutJumpOutCritical()                   \
       asm volatile(                           \
        "ldc.b %0l, ccr"::"r"(__ccr__)         \
       );

#define NutEnableInt(ccr)                      \
    {                                          \
        u_char __ccr__;                        \
        asm volatile(                          \
            "stc.b ccr, %0l"            "\n\t" \
            "andc.b #0x3f, ccr":"=r"(__ccr__): \
        );

#define NutDisableInt() NutExitCritical()

/* #define NutEnterCritical(ccr) NutDisableInt(ccr) */

/* #define NutExitCritical(ccr) NutRestoreInt(ccr) */

#elif defined(__arm__)

#define AtomicInc(p)     (++(*p))
#define AtomicDec(p)     (--(*p))

/* TODO */
#define NutEnterCritical()
/* TODO */
#define NutExitCritical()
/* TODO */
#define NutJumpOutCritical()

#elif defined(__m68k__)

#define AtomicInc(p)     (++(*p))
#define AtomicDec(p)     (--(*p))

/* TODO */
#define NutEnterCritical()
/* TODO */
#define NutExitCritical()
/* TODO */
#define NutJumpOutCritical()

#endif /* !defined(__m68k__) */

__END_DECLS

#endif
