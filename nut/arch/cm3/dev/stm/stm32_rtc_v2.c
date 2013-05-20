/*
 * Copyright (C) 20130 by Uwe Bonnes(bon@elektron.ikp,physik.tu-darmmstadt.de).
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
 *
 */

/*
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <cfg/os.h>
#include <cfg/clock.h>
#include <cfg/arch.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <dev/rtc.h>

#include <cfg/arch/gpio.h>
#include <arch/cm3/stm/stm32xxxx.h>
#include <arch/cm3/stm/stm32xxxx.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

/*!
 * \brief Get status of the STM32 V2 hardware clock.
 *
 * \param sflags Points to an unsigned long that receives the status flags.
 *               - Bit 0: Backup Domain Reset happened.
 *               - Bit 5: Alarm occured.
 * \return 0 on success or -1 in case of an error.
 */
static int Stm32RtcGetStatus(NUTRTC *rtc, uint32_t *sflags)
{
    uint32_t res = 0;

    /* Check for power failure */
    if (!(RTC->ISR & RTC_ISR_INITS)) {
        res = 1;
    }

    if (sflags) {
        *sflags = res;
        return 0;
    } else {
        return -1;
    }
}

/*!
 * \brief Get date and time from an STM32 V2 hardware clock.
 *
 * \param tm Points to a structure that receives the date and time
 *           information.
 *
 * \return 0 on success or -1 in case of an error.
 */
int Stm32RtcGetClock(NUTRTC *rtc, struct _tm *tm)
{
    if (tm)
    {
        tm->tm_mday = ((RTC->DR >>  0) & 0xf) + (((RTC->DR >>  2) & 0x3) * 10);
        tm->tm_mon  = ((RTC->DR >>  8) & 0xf) + (((RTC->DR >> 12) & 0x1) * 10);
        tm->tm_mon  -= 1;
        tm->tm_year = ((RTC->DR >> 16) & 0xf) + (((RTC->DR >> 20) & 0xf) * 10)
            + 100;
        tm->tm_hour = ((RTC->TR >> 16) & 0xf) + (((RTC->TR >> 20) & 0x7) * 10);
        if (RTC->TR & RTC_TR_PM)
            tm->tm_hour += 12;
        tm->tm_min  = ((RTC->TR >>  8) & 0xf) + (((RTC->TR >> 12) & 0x7) * 10);
        tm->tm_sec  = ((RTC->TR >>  0) & 0xf) + (((RTC->TR >>  4) & 0x7) * 10);
        tm->tm_wday = ((RTC->DR >> 13) & 0x7);
        tm->tm_yday = 0/*FIXME*/;
        return 0;
    }
    else
        return -1;
}

/*!
 * \brief Set the STM32 V2 hardware clock.
 *
 * \param tm Points to a structure which contains the date and time
 *           information.
 *
 * \return 0 on success or -1 in case of an error.
 */
int Stm32RtcSetClock(NUTRTC *rtc, const struct _tm *tm)
{
    uint32_t bcd_date, bcd_time, year, month;

    /* Allow RTC Write Access */
    RTC->WPR = 0xca;
    RTC->WPR = 0x53;

    /* Stop Clock*/
    RTC->ISR |= RTC_ISR_INIT;
    while (!(RTC->ISR & RTC_ISR_INITF));

    month = tm->tm_mon +1 ; /* Range 1..12*/
    year = tm->tm_year - 100; /* only two digits available*/
    bcd_date  = (tm->tm_mday % 10);
    bcd_date |= (tm->tm_mday / 10) <<  4;
    bcd_date |= (     month  % 10) <<  8;
    bcd_date |= (     month  / 10) << 12;
    bcd_date |= (tm-> tm_wday    ) << 13;
    bcd_date |= (       year % 10) << 16;
    bcd_date |= (       year / 10) << 20;
    RTC->DR = bcd_date;

    bcd_time  = (tm->tm_sec  % 10);
    bcd_time |= (tm->tm_sec  / 10) <<  4;
    bcd_time |= (tm->tm_min  % 10) <<  8;
    bcd_time |= (tm->tm_min  / 10) << 12;
    bcd_time |= (tm->tm_hour % 10) << 16;
    bcd_time |= (tm->tm_hour / 10) << 20;
    RTC->TR = bcd_time;

    /*enable clock and inhibit RTC write access */
    RTC->ISR &= ~RTC_ISR_INIT;
    RTC->WPR = 0;
    return 0;
}


/*!
 * \brief Initialize the RTC in stm32f10x controller
 *
 * \return 0 on success or -1 in case of an error.
 *
 */
int Stm32RtcInit(NUTRTC *rtc)
{

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_DBP;
    RCC->BDCR|= RCC_BDCR_RTCEN;

    if(RTC->ISR & RTC_ISR_INITS)
        /* The RTC has been set before. Do not set it*/
        /* Fixme: May give problems when reflashing to a different CLK source*/
        return 0;

    /* Backup Domain software reset*/
    RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;
    /* Reenable RTC clk */
    RCC->BDCR|= RCC_BDCR_RTCEN;

    /* Select HSE/32 Clock for now. FIXME! */
    RCC->BDCR &= ~RCC_BDCR_RTCSEL;
    RCC->BDCR |= RCC_BDCR_RTCSEL_0 | RCC_BDCR_RTCSEL_1;
#if (HSE_VALUE % (32 * 250) != 0)
#warning FIXME: RTC clock setup for given HSE_VALUE
    return -1;
#endif
    /* Allow RTC Write Access */
    RTC->WPR = 0xca;
    RTC->WPR = 0x53;

    RTC->ISR |= RTC_ISR_INIT;
    while (!(RTC->ISR & RTC_ISR_INITF));

    /* 1 Mhz/32 = 31.250 kHz. So use 125 for the asyn prescaler */
    RTC->PRER = ((125 - 1) <<16) | ((HSE_VALUE/32/125) - 1);
    RTC->ISR &= ~RTC_ISR_INIT;
    RTC->WPR = 0; /* Disable  RTC Write Access */

    return 0;
}

NUTRTC rtcStm32 = {
  /*.dcb           = */ NULL,               /*!< Driver control block */
  /*.rtc_init      = */ Stm32RtcInit,       /*!< Hardware initializatiuon, rtc_init */
  /*.rtc_gettime   = */ Stm32RtcGetClock,   /*!< Read date and time, rtc_gettime */
  /*.rtc_settime   = */ Stm32RtcSetClock,   /*!< Set date and time, rtc_settime */
  /*.rtc_getalarm  = */ NULL,               /*!< Read alarm date and time, rtc_getalarm */
  /*.rtc_setalarm  = */ NULL,               /*!< Set alarm date and time, rtc_setalarm */
  /*.rtc_getstatus = */ Stm32RtcGetStatus,  /*!< Read status flags, rtc_getstatus */
  /*.rtc_clrstatus = */ NULL,               /*!< Clear status flags, rtc_clrstatus */
  /*.alarm         = */ NULL,               /*!< Handle for alarm event queue, not supported right now */
};
