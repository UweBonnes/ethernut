#ifndef _SYS_TIMER_H
#define _SYS_TIMER_H

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
 * Revision 1.3  2003/11/03 16:35:18  haraldkipp
 * New API returns system uptime in miliseconds
 *
 * Revision 1.2  2003/10/13 10:17:11  haraldkipp
 * Seconds counter added
 * 
 * Revision 1.1.1.1  2003/05/09 14:41:22  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.12  2003/02/04 18:00:54  harald
 * Version 3 released
 *
 * Revision 1.11  2003/01/14 16:38:18  harald
 * New tick counter query
 *
 * Revision 1.10  2002/06/26 17:29:29  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/device.h>

/*!
 * \file sys/timer.h
 * \brief Timer management definitions.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Timer type.
 */
typedef struct _NUTTIMERINFO NUTTIMERINFO;

/*!
 * \struct _NUTTIMERINFO timer.h sys/timer.h
 * \brief Timer information structure.
 */
struct _NUTTIMERINFO {
    /*! \brief Link to next timer. 
     */
    NUTTIMERINFO *tn_next;          
    /*! \brief Number of system ticks. 
     *  Set to zero on one-shot timers.
     */
    u_long tn_ticks;        
    /*! \brief Decremented by one on each system tick intervall. 
     */
    u_long tn_ticks_left;   
    /*! \brief Callback function. 
     */
    void (*tn_callback)(HANDLE, void *);    
    /*! \brief Argument pointer passed to callback function. 
     */
    volatile void *tn_arg;          
};

extern NUTTIMERINFO* volatile nutTimerList;
extern NUTTIMERINFO* volatile nutTimerPool;

extern void NutTimerInit(void);


/*
 * API declarations.
 */

#define TM_ONESHOT  0x01

extern HANDLE NutTimerStart(u_long ms, void (*callback)(HANDLE, void *), void *arg, u_char flags);
extern void NutTimerStop(HANDLE handle);
extern void NutTimerStopAsync(HANDLE handle);
extern void NutSleep(u_long ms);
extern void NutDelay(u_char ms);

extern u_long NutGetCpuClock(void);

extern u_long NutGetTickCount(void); //@@MF

extern u_long NutGetSeconds(void);
extern u_long NutGetMillis(void);

#ifdef __cplusplus
}
#endif

#endif
