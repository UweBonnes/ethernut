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
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.btnode.ethz.ch
 *
 */
 /*
 * sys/tracer.h
 *
 * 22.12.2004 Philipp Blum <blum@tik.ee.ethz.ch>
 */
#ifndef _SYS_TRACER_H_
#define _SYS_TRACER_H_

/**

\file sys/tracer.h

\author Philipp Blum <blum@tik.ee.ethz.ch>

\date 22.12.2004

\brief Trace functions


*/

#include <sys/types.h>
#include <sys/atom.h>

/******************************************************************
 * defines 
 ******************************************************************/
#define TRACE_MODE_OFF              0
#define TRACE_MODE_CIRCULAR         1 
#define TRACE_MODE_DEFAULT        TRACE_MODE_CIRCULAR
#define TRACE_SIZE_DEFAULT        500
#define TRACE_TAG_CRITICAL_ENTER    0
#define TRACE_TAG_CRITICAL_EXIT     1
#define TRACE_TAG_THREAD_YIELD      2
#define TRACE_TAG_THREAD_SETPRIO    3
#define TRACE_TAG_THREAD_WAIT       4
#define TRACE_TAG_THREAD_SLEEP      5
#define TRACE_TAG_INTERRUPT_ENTER   6
#define TRACE_TAG_INTERRUPT_EXIT    7
#define TRACE_TAG_STOP              8
#define TRACE_TAG_END               9

#define TRACE_INT_UART0_CTS         0
#define TRACE_INT_UART0_RXCOMPL     1
#define TRACE_INT_UART0_TXEMPTY     2
#define TRACE_INT_UART1_CTS         3
#define TRACE_INT_UART1_RXCOMPL     4
#define TRACE_INT_UART1_TXEMPTY     5
#define TRACE_INT_TIMER0_OVERFL     6
#define TRACE_INT_TIMER1_OVERFL     7
#define TRACE_INT_SUART_TIMER       8
#define TRACE_INT_SUART_RX          9
#define TRACE_INT_END              10

/******************************************************************
 * typedefs
 ******************************************************************/
typedef struct _t_traceitem {
	u_char tag;
	u_int  pc;
	u_int  time_h;
	u_int  time_l;
} t_traceitem;

/******************************************************************
 * global variables 
 ******************************************************************/
extern u_int micros_high;
extern t_traceitem *trace_items;
extern t_traceitem *trace_current;
extern int trace_head;
extern int trace_size;
extern char trace_isfull;
extern char trace_mode;
extern char trace_mask[TRACE_TAG_END];

/******************************************************************
 * function prototypes API
 ******************************************************************/
extern int  NutInitTrace(int size); 
extern int  NutInitTraceCmds(void);
extern void NutStopTrace(void);
extern void NutStartTrace(void);
extern void NutPrintTrace(u_char* arg);
extern int  NutGetPC(void);
extern void NutClearTrace(void);
extern void NutPrintTraceMask(u_char* arg);
extern void NutPrintTrace(u_char* arg);


#define TRACE_ADD_ITEM(TAG,PC)                      \
 	asm volatile(                                   \
        "in  __tmp_reg__, __SREG__" "\n\t"          \
                "push __tmp_reg__"  "\n\t"          \
        "cli"                       "\n\t"          \
    );                                              \
    if ((trace_mode != TRACE_MODE_OFF) &&           \
		(trace_mask[TAG] == 1))                     \
	{                                               \
		trace_current = &trace_items[trace_head++]; \
		trace_current->tag = TAG;                   \
		trace_current->pc = PC;                     \
		trace_current->time_h = micros_high;        \
		trace_current->time_l = TCNT1;              \
		if (trace_head >= trace_size) {             \
			trace_head = 0;                         \
			trace_isfull = 1;                       \
		}                                           \
	};                                              \
	asm volatile(                                   \
        "pop __tmp_reg__"           "\n\t"          \
        "out __SREG__, __tmp_reg__" "\n\t"          \
    );
#define TRACE_ADD_ITEM_PC(TAG) TRACE_ADD_ITEM(TAG,NutGetPC())

#endif

