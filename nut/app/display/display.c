/*!
 * Copyright (C) 2013-2015 Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
 *
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

/*!
 * $Id$
 */

#include <dev/board.h>
#include <string.h>
#include <stdio.h>
#include <sys/timer.h>

#include <pro/rfctime.h>
#include <cfg/crt.h>

#include <arch/cm3/stm/stm32xxxx.h>

static const char banner[] = "\nLCD on "
    BOARDNAME " " __DATE__ " " __TIME__"     \n";

static int SetRtc(time_t *now)
{
    int res;
    struct _tm gmt;
    gmtime_r(now, &gmt);
    res = NutRtcSetTime(&gmt);
    /* Set system time from RTC*/
    if (NutRtcGetTime(&gmt) == 0) {
        time_t now = _mkgmtime(&gmt);
        if (now != -1) {
            stime(&now);
        }
    }
    return res;
}

NUTRTC *RTC_Init(void)
{
    time_t now;
    uint32_t rtc_stat;

    now = RfcTimeParse("Unk, " __DATE__ " " __TIME__);
    now -= (CRT_TIMEZONE - CRT_DAYLIGHT) * 60;
    if (NutRegisterRtc(&RTC_CHIP)) {
        return 0;
    }
    NutRtcGetStatus(&rtc_stat);
    if (rtc_stat & RTC_STATUS_PF) {
        SetRtc(&now);
    }
    else {
        struct _tm rtc_tm;
        time_t rtc_time, rtc_gm_time;

        NutRtcGetTime(&rtc_tm);
        rtc_time = mktime(&rtc_tm);
        rtc_gm_time = rtc_time + (CRT_TIMEZONE - CRT_DAYLIGHT) * 60;
        if (now > rtc_gm_time) {
            /* Set Rtc as Rtc has older time than executable*/
            SetRtc(&now);
        }
    }
    return &RTC_CHIP;
}

int main(void)
{
    RCC_TypeDef *rcc= (RCC_TypeDef *) RCC_BASE;
    FILE *lcd;
    int i = 0;

    rcc->APB1ENR |= RCC_APB1ENR_PWREN;
    NutRegisterDevice(&DEV_DISPLAY, 0, 0);
    lcd = fopen(DEV_DISPLAY_NAME, "r+");
    fprintf(lcd, banner);
    RTC_Init();
    while (1) {
        struct _tm rtctime;
        if (NutRtcGetTime(&rtctime) == 0) {
            fprintf(lcd, "%2d:%02d:%02d\n", rtctime.tm_hour,
                    rtctime.tm_min, rtctime.tm_sec);
        }
        else fprintf(lcd, "Failed");
        i++;
        NutSleep(250);
    }
}