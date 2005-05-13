#ifndef _SYS_ATOM_H_
#define _SYS_ATOM_H_

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
 * Revision 1.10  2005/05/13 21:47:07  chaac
 * Entering a critical section should now be faster on AVR's when
 * calling NutEnterCritical().
 *
 * Revision 1.9  2005/04/05 17:42:45  haraldkipp
 * ARM7 implementation of critical sections added.
 *
 * Revision 1.8  2005/02/21 12:37:59  phblum
 * Removed tabs and added semicolons after NUTTRACER macros
 *
 * Revision 1.7  2005/02/16 19:51:15  haraldkipp
 * Enable tracer configuration.
 *
 * Revision 1.6  2005/02/10 07:06:48  hwmaier
 * Changes to incorporate support for AT90CAN128 CPU
 *
 * Revision 1.5  2005/01/24 22:34:46  freckle
 * Added new tracer by Phlipp Blum <blum@tik.ee.ethz.ch>
 *
 * Revision 1.4  2004/04/07 12:13:57  haraldkipp
 * Matthias Ringwald's *nix emulation added
 *
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

#include <cfg/os.h>
#include <sys/types.h>

#ifdef NUTTRACER_CRITICAL
#include <sys/tracer.h>
#endif


__BEGIN_DECLS
#if defined(__AVR__)
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

#define NutEnterCritical() \
        asm volatile (             \
                "@ NutEnterCritical"      "\n\t"      \
                "mrs r0, cpsr"      "\n\t"      \
                "orr r0, r0, #0xC0" "\n\t"  \
                "msr cpsr, r0"      "\n\t"  \
                ::: "r0" )

#define NutExitCritical() \
        asm volatile (             \
                "@ NutEnterCritical"      "\n\t"      \
                "mrs r0, cpsr"      "\n\t"      \
                "bic r0, r0, #0x80" "\n\t"  \
                "msr cpsr, r0"      "\n\t"  \
                ::: "r0" )

#define NutJumpOutCritical()    NutExitCritical()

#elif defined(__linux__) || defined (__APPLE__)
#include <pthread.h>
#include <signal.h>
#include <sys/thread.h>
#include <stdio.h>
#include <stdlib.h>
extern u_short main_cs_level;
extern sigset_t irq_signal;

extern FILE *__os_trs;
extern u_char __os_trf;

#define AtomicInc(p)     (++(*p))
#define AtomicDec(p)     (--(*p))



#define NutEnterCritical()					     \
		pthread_sigmask(SIG_BLOCK, &irq_signal, 0);  \
		if (runningThread){						 \
			/* if (runningThread->td_cs_level==0)												    \
				printf("Entered a: %s.%d - %s\n", __FILE__, __LINE__, runningThread->td_name); */	\
			runningThread->td_cs_level++;        \
		} else {								 \
			/* if (main_cs_level==0)												\
				printf("Entered b: %s.%d - %s\n", __FILE__, __LINE__, "ROOT") */;	\
			main_cs_level++;					 \
		}										 \

#define NutExitCritical()									\
		if (runningThread){									\
			if (--runningThread->td_cs_level == 0) {		\
				/* printf("Left a: %s.%d - %s\n", __FILE__, __LINE__, runningThread->td_name);	*/ \
				pthread_sigmask(SIG_UNBLOCK, &irq_signal, 0);   \
			}												\
		} else {											\
			if (--main_cs_level == 0) {						\
				/* printf("Left a: %s.%d - %s\n", __FILE__, __LINE__, "ROOT");	*/ \
				pthread_sigmask(SIG_UNBLOCK, &irq_signal, 0);	\
			}												\
		}

#if 0

#define NutEnterCritical()					     \
		if (runningThread){						 \
			if (runningThread->td_cs_level==0)												\
				printf("Entered a: %s.%d - %s\n", __FILE__, __LINE__, runningThread->td_name);	\
			runningThread->td_cs_level++;        \
		} else {								 \
			if (main_cs_level==0)												\
				printf("Entered b: %s.%d - %s\n", __FILE__, __LINE__, "ROOT");	\
			main_cs_level++;					 \
		}										 \

#define NutExitCritical()									\
		if (runningThread){									\
			if (--runningThread->td_cs_level == 0) {		\
				printf("Left a: %s.%d - %s\n", __FILE__, __LINE__, runningThread->td_name);	\
			}												\
		} else {											\
			if (--main_cs_level == 0) {						\
				printf("Left a: %s.%d - %s\n", __FILE__, __LINE__, "ROOT");	\
			}												\
		}
#endif

#if 0
/* TODO */
#define NutEnterCritical()
/* TODO */
#define NutExitCritical()
#endif


#define NutJumpOutCritical() NutExitCritical()


#elif defined(__m68k__)
#define AtomicInc(p)     (++(*p))
#define AtomicDec(p)     (--(*p))
/* TODO */
#define NutEnterCritical()
/* TODO */
#define NutExitCritical()
/* TODO */
#define NutJumpOutCritical()
#endif                          /* !defined(__m68k__) */
    __END_DECLS
#endif
