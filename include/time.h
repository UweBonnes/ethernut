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
 * Revision 1.1  2003/11/24 18:11:03  drsung
 * first release
 *
 *
 */

#ifndef __TIME_H
#define __TIME_H

#include <sys/types.h>
#include <compiler.h>

__BEGIN_DECLS 
typedef struct _tm tm;
struct _tm {
    int tm_sec;                 /* seconds after the minute - [0,59] */
    int tm_min;                 /* minutes after the hour - [0,59] */
    int tm_hour;                /* hours since midnight - [0,23] */
    int tm_mday;                /* day of the month - [1,31] */
    int tm_mon;                 /* months since January - [0,11] */
    int tm_year;                /* years since 1900 */
    int tm_wday;                /* days since Sunday - [0,6] */
    int tm_yday;                /* days since January 1 - [0,365] */
    int tm_isdst;               /* daylight savings time flag */
};

typedef u_long time_t;

time_t time(time_t *);
int gmtime_r(const time_t *, tm *);
tm *gmtime(const time_t *);
int localtime_r(const time_t *, tm *);
tm *localtime(const time_t *);

int stime(time_t * t);
time_t mktime(tm * timeptr);
time_t _mkgmtime(tm * timeptr);

extern u_char _daylight;
extern long _timezone;
extern long _dstbias;

__END_DECLS

#endif
