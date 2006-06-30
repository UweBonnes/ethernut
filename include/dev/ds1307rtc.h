#ifndef _DEV_DS1307RTC_H_
#define	_DEV_DS1307RTC_H_

/*
 * Copyright (C) 2006 by egnite Software GmbH and Christian Welzel.
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
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
 * Revision 1.1  2006/06/30 22:07:43  christianwelzel
 * Initial check in.
 *
 *
 */

#include <sys/types.h>
#include <time.h>

#define BCD2BIN(x) ((((u_char)x) >> 4) * 10 + ((x) & 0x0F))
#define BIN2BCD(x) (((((u_char)x) / 10) << 4) + (x) % 10)

__BEGIN_DECLS
/* Prototypes */
extern int DS1307Init(void);

extern int DS1307RtcGetClock(struct _tm *tm);
extern int DS1307RtcSetClock(CONST struct _tm *tm);
extern int DS1307RtcReadRegs(u_char addr, u_char *buff, size_t len);
extern int DS1307RtcWrite(CONST u_char *buff, size_t len);

extern int DS1307RamRead(u_char addr, u_char *buff, size_t cnt);
extern int DS1307RamWrite(u_char addr, CONST void *buff, size_t len);

__END_DECLS
/* End of prototypes */
#endif
