#ifndef _DEV_X12RTC_H_
#define	_DEV_X12RTC_H_

/*
 * Copyright (C) 2005 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.1  2005/10/24 11:41:39  haraldkipp
 * Initial check-in.
 *
 */

#include <sys/types.h>

#define RTC_STATUS_PF       0x01
#define RTC_STATUS_AL0      0x20
#define RTC_STATUS_AL1      0x40

#define RTC_ALARM_SECOND    0x01
#define RTC_ALARM_MINUTE    0x02
#define RTC_ALARM_HOUR      0x04
#define RTC_ALARM_MDAY      0x08
#define RTC_ALARM_MONTH     0x10
#define RTC_ALARM_WDAY      0x80

#define BCD2BIN(x) ((((u_char)x) >> 4) * 10 + ((x) & 0x0F))
#define BIN2BCD(x) (((((u_char)x) / 10) << 4) + (x) % 10)

__BEGIN_DECLS
/* Prototypes */
extern int X12Init(void);

extern int X12RtcGetClock(struct _tm *tm);
extern int X12RtcSetClock(CONST struct _tm *tm);
extern int X12RtcGetAlarm(int idx, struct _tm *tm, int *aflgs);
extern int X12RtcSetAlarm(int idx, struct _tm *tm, int aflgs);
extern int X12RtcGetStatus(u_long *sflgs);
extern int X12RtcClearStatus(u_long sflgs);
extern int X12RtcReadRegs(u_char addr, u_char *buff, size_t len);
extern int X12RtcWrite(int nv, CONST u_char *buff, size_t len);

extern int X12EepromRead(u_int addr, void *buff, size_t len);
extern int X12EepromWrite(u_int addr, CONST void *buff, size_t len);

__END_DECLS
/* End of prototypes */
#endif
