#ifndef _SYS_THREAD_H_
#define _SYS_THREAD_H_

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
 * -
 * Portions Copyright (C) 2000 David J. Hudson <dave@humbug.demon.co.uk>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "copying-gpl.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "copying-liquorice.txt" for details.
 */

/*
 * $Log$
 * Revision 1.2  2003/12/15 19:28:26  haraldkipp
 * Kill function prototypes
 *
 * Revision 1.1.1.1  2003/05/09 14:41:22  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.14  2003/02/04 18:00:54  harald
 * Version 3 released
 *
 * Revision 1.13  2002/10/29 15:40:50  harald
 * *** empty log message ***
 *
 * Revision 1.12  2002/06/26 17:29:29  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/device.h>

/*!
 * \file sys/thread.h
 * \brief Thread management definitions.
 */

#ifdef __cplusplus
extern "C" {
#endif

#define DEADBEEF    0xDEADBEEF


/*!
 * \addtogroup xgThread
 */
/*@{*/

/*!
 * Thread information structure type.
 */
typedef struct _NUTTHREADINFO NUTTHREADINFO;

/*!
 * \struct _NUTTHREADINFO thread.h sys/thread.h
 * \brief Thread information structure.
 */
struct _NUTTHREADINFO {
    NUTTHREADINFO *volatile td_next;    /*!< \brief Linked list of all threads. */
    NUTTHREADINFO *td_qnxt;    /*!< \brief Linked list of all queued thread. */
    u_char  td_name[9];     /*!< \brief Name of this thread. */
    u_char  td_state;       /*!< \brief Operating state. One of TDS_ */
    u_short td_sp;          /*!< \brief Stack pointer. */
    u_char  td_priority;    /*!< \brief Priority level. 0 is highest priority. */
    u_char *td_memory;     /*!< \brief Pointer to heap memory used for stack. */
    HANDLE  td_timer;       /*!< \brief Event timer. */
    HANDLE  td_queue;       /*!< \brief Root entry of the waiting queue. */
};
/*@}*/

/*!
 * \addtogroup xgThreadState
 */
/*@{*/
#define TDS_TERM        0   /*!< Thread has exited. */
#define TDS_RUNNING     1   /*!< Thread is running. */
#define TDS_READY       2   /*!< Thread is ready to run. */
#define TDS_SLEEP       3   /*!< Thread is sleeping. */
/*@}*/


extern NUTTHREADINFO* volatile runningThread;
extern NUTTHREADINFO* volatile nutThreadList;
extern NUTTHREADINFO * volatile runQueue;



extern HANDLE NutThreadCreate(u_char *name, void (*fn)(void *), void *arg, u_short stackSize);
extern u_char NutThreadSetPriority(u_char level);

extern void NutThreadKill(void);
extern void NutThreadDestroy(void);
extern void NutThreadExit(void);

extern void NutThreadResumeAsync(HANDLE th);
extern void NutThreadWake(HANDLE timer, HANDLE th);
extern void NutThreadYield(void);

extern void NutThreadAddPriQueue(NUTTHREADINFO *td, NUTTHREADINFO **tqpp);
extern void NutThreadRemoveQueue(NUTTHREADINFO *td, NUTTHREADINFO *volatile *tqpp);

extern void NutThreadSwitch(void);
extern HANDLE GetThreadByName(u_char *name);

/*!
 * \brief Macro for thread entry definitions.
 */
#define THREAD(threadfn, arg) \
void threadfn(void *arg) __attribute__ ((noreturn)); \
void threadfn(void *arg)

extern void DumpThreads(HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif
