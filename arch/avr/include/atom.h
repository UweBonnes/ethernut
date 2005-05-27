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
 *
 */

/*
 * $Log$
 * Revision 1.1  2005/05/27 17:41:52  drsung
 * Moved the file.
 *
 * Revision 1.1  2005/05/26 10:08:42  drsung
 * Moved the platform dependend code from include/sys/atom.h to this file.
 *
 *
 */
 
#ifndef _SYS_ATOM_H_
#error "Do not include this file directly. Use sys/atom.h instead!"
#endif

__BEGIN_DECLS

#ifdef __IMAGECRAFT__
#define AtomicInc(p)     (++(*p))
#define AtomicDec(p)     (--(*p))
#else
static inline void AtomicInc(volatile u_char * p)
{
    asm volatile ("in  __tmp_reg__, __SREG__" "\n\t"
                  "cli" "\n\t"
                  "ld r24, %a0" "\n\t" "subi r24, lo8(-1)" "\n\t" "st %a0, r24" "\n\t" "out __SREG__, __tmp_reg__" "\n\t"::"z" (p)
                  :"r24");
}

static inline void AtomicDec(volatile u_char * p)
{
    asm volatile ("in  __tmp_reg__, __SREG__" "\n\t"
                  "cli" "\n\t"
                  "ld r24, %a0" "\n\t" "subi r24, lo8(1)" "\n\t" "st %a0, r24" "\n\t" "out __SREG__, __tmp_reg__" "\n\t"::"z" (p)
                  :"r24");
}

#endif


#ifdef __IMAGECRAFT__

#define NutEnterCritical()  \
{                           \
    asm("in R0, 0x3F\n"     \
        "cli\n"             \
        "push R0\n");       \
}

#define NutExitCritical()   \
{                           \
    asm("pop R0\n"          \
        "out 0x3F, R0\n");  \
}

#else

#define NutEnterCritical_nt()               \
    asm volatile(                           \
        "in  __tmp_reg__, __SREG__" "\n\t"  \
        "cli"                       "\n\t"  \
        "push __tmp_reg__"          "\n\t"  \
    )

#define NutExitCritical_nt()                \
    asm volatile(                           \
        "pop __tmp_reg__"           "\n\t"  \
        "out __SREG__, __tmp_reg__" "\n\t"  \
    )

#ifdef NUTTRACER_CRITICAL
#define NutEnterCritical()                  \
    NutEnterCritical_nt();                  \
    TRACE_ADD_ITEM_PC(TRACE_TAG_CRITICAL_ENTER);

#define NutExitCritical()                   \
    TRACE_ADD_ITEM_PC(TRACE_TAG_CRITICAL_EXIT); \
    NutExitCritical_nt()
#else
#define NutEnterCritical()                  \
    NutEnterCritical_nt();

#define NutExitCritical()                   \
    NutExitCritical_nt()
#endif
#endif

#define NutJumpOutCritical() NutExitCritical()

__END_DECLS
