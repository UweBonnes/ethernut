#ifndef _DEV_PCF8563_H_
#define _DEV_PCF8563_H_

/*
 * Copyright (C) 2006 by egnite Software GmbH. All rights reserved.
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
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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
 * Revision 1.3  2008/08/11 06:59:59  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.2  2006/10/05 17:18:49  haraldkipp
 * Hardware independant RTC layer added.
 *
 * Revision 1.1  2006/04/07 13:54:17  haraldkipp
 * PCF8563 RTC driver added.
 *
 */

#include <dev/rtc.h>

extern NUTRTC rtcPcf8563;

extern int PcfRtcInit(NUTRTC *rtc);

extern int PcfRtcGetClock(NUTRTC *rtc, struct _tm *tm);
extern int PcfRtcSetClock(NUTRTC *rtc, const struct _tm *tm);
extern int PcfRtcGetAlarm(NUTRTC *rtc, int idx, struct _tm *tm, int *aflgs);
extern int PcfRtcSetAlarm(NUTRTC *rtc, int idx, const struct _tm *tm, int aflgs);
extern int PcfRtcGetStatus(NUTRTC *rtc, uint32_t *sflgs);
extern int PcfRtcClearStatus(NUTRTC *rtc, const uint32_t sflgs);
extern int PcfRtcReadRegs(uint8_t addr, uint8_t *buff, size_t len);
extern int PcfRtcWrite(int nv, const uint8_t *buff, size_t len);

#endif
