/*
 * Copyright (C) 2013,2014 by Uwe Bonnes(bon@elektron.ikp,physik.tu-darmmstadt.de).
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
#include <cfg/rtc.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <dev/rtc.h>

#include <cfg/arch/gpio.h>
#include <arch/cm3/stm/stm32xxxx.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RTCCLK_LSE 1
#define RTCCLK_HSE 2
#define RTCCLK_LSI 3

/*Equalize L1 anormalies */
#if defined (RCC_CSR_RTCEN) && !defined(RCC_BDCR_RTCEN)
# define RCC_BDCR_RTCEN RCC_CSR_RTCEN
#endif

#if defined (RCC_CSR_LSEBYP) && !defined(RCC_BDCR_LSEBYP)
# define RCC_BDCR_LSEBYP RCC_CSR_LSEBYP
#endif

#if defined (RCC_CSR_LSEON) && !defined(RCC_BDCR_LSEON)
# define RCC_BDCR_LSEON RCC_CSR_LSEON
#endif

#if defined (RCC_CSR_LSERDY) && !defined(RCC_BDCR_LSERDY)
# define RCC_BDCR_LSERDY RCC_CSR_LSERDY
#endif

#if defined (RCC_CSR_RTCRST) && !defined(RCC_BDCR_BDRST)
# define RCC_BDCR_BDRST RCC_CSR_RTCRST
#endif

#if defined (RCC_CSR_RTCSEL_0) && !defined(RCC_BDCR_RTCSEL_0)
# define RCC_BDCR_RTCSEL_0 RCC_CSR_RTCSEL_0
#endif

#if defined (RCC_CSR_RTCSEL_1) && !defined(RCC_BDCR_RTCSEL_1)
# define RCC_BDCR_RTCSEL_1 RCC_CSR_RTCSEL_1
#endif

#if defined (RCC_CSR_RTCSEL) && !defined(RCC_BDCR_RTCSEL)
# define RCC_BDCR_RTCSEL RCC_CSR_RTCSEL
# define RCC_BDCR RCC->CSR
#else
# define RCC_BDCR RCC->BDCR
#endif

#if defined(RCC_CFGR_RTCPRE)
# define HSE_RTCPRE 0
#endif

#if !defined(RCC_CFGR_RTCPRE_0)
# define RCC_CFGR_RTCPRE_0 0
#endif

/* L0 has only one Alarm channel*/
#if defined (RTC_ISR_ALRBF)
# define NUM_ALARMS 2
#else
# define NUM_ALARMS 1
# if !defined(RTC_CR_ALRBIE)
#  define  RTC_CR_ALRBIE 0
# endif
#endif

/* F411 has some power level setting for the LSE */
#if !defined(RCC_BDCR_LSEMOD)
# define RCC_BDCR_LSEMOD 0
#endif

#if defined(RTC_LSE_MOD)
# define RTC_LSEMODF RCC_BDCR_LSEMOD
#else
# define  RTC_LSEMODF 0
#endif

#if defined(RTC_LSE_BYPASS)
# define RTC_BYPASSF RCC_BDCR_LSEBYP
#else
# define RTC_BYPASSF 0
#endif

static int RtcRccHsePrepare(void) __attribute__ ((unused));
static int RtcLseSetup(void) __attribute__ ((unused));
static int RtcRccLsiPrepare(void) __attribute__ ((unused));

/* STM CMSIS definition has (uint32_t) marker and so can not be used
 * for math in preprocessor */
#if defined(HSE_RTCPRE) && !defined(RCC_CFGR_RTCPRE_2)
/* 15 bit on L1/L0 */
# define SYNC_MAX 0x7fff
#else
# define SYNC_MAX 0x1fff
#endif

#if RTC_CLK_SRC == RTCCLK_HSE
/* Handle HSE as RTC clock */
# define RTC_CLKSEL RCC_BDCR_RTCSEL
# define RTC_STATUS_START (RTC_STATUS_HAS_QUEUE | RTC_STATUS_INACCURATE)
# define RTC_STATUS_FLAG  RTC_STATUS_HAS_QUEUE
# define BDCR_MASK RCC_BDCR_RTCSEL
# define BDCR_FLAG RCC_BDCR_RTCSEL
# define RTC_CLK_SETUP() 0

# if !defined(RTC_PRE)
/* Calculate RTC_PRE divisor as it is not given*/
#  if defined(HCE_RTCPRE)
#   if !defined(RCC_CFGR_RTCPRE_2)
/* L1 has HSE prescaler 2/4/8/16 */
#    if HSE_VALUE >   8000000
#     define RTC_PRE     16
#    elif HSE_VALUE > 4000000
#     define RTC_PRE      8
#    elif HSE_VALUE > 2000000
#     define RTC_PRE      4
#    else
#     define RTC_PRE      2
#    endif
#   else
/* F2/F4 has HSE prescaler 2.. 32*/
#    if HSE_VALUE <= 2000000
#     define RTC_PRE 2
#    elif HSE_VALUE > 32000000
#     warning HSE_VALUE to high to reach 1 MHz RTC clock
#    else
#     define RTC_PRE (HSE_VALUE / 1000000)
#    endif
#   endif
/* F3/F0/L0 has fixed HSE prescaler of 32*/
#  else
#   define RTC_PRE 32
#  endif
# endif

/* Check value of calculated or given RTC_PRE and find RTC_PRE_VAL*/
# if defined(HSE_RTCPRE)
#  define RTC_RCC_PPREPARE() RtcRccHsePrepare()
#  if !defined(RCC_CFGR_RTCPRE_2)
#   if  RTC_PRE == 16
#    define  RTC_PRE_VAL  3
#   elif  RTC_PRE == 8
#    define  RTC_PRE_VAL  2
#   elif  RTC_PRE == 4
#    define  RTC_PRE_VAL  1
#   elif  RTC_PRE == 2
#    define  RTC_PRE_VAL  0
#   elif
#    warning Illegal RTC_PRE for L1/L0 given
#   endif
#  else
#   if RTC_PRE  < 2 || RTC_PRE > 32
#    warning Illegal RTC_PRE for F2/F4 given
#   else
#    define RTC_PRE_VAL (RTC_PRE -1 )
#   endif
#  endif
# else
#   define RTC_RCC_PPREPARE() 0
#  if RTC_PRE != 32
#    warning Illegal RTC_PRE for F0/F3 given, only 32 allowed
#  else
#   define RTC_PRE_VAL 0
#  endif
# endif

/* Guess RTC_ASYNC and RTC_SYNC if not given*/
# if !defined(RTC_ASYNC) && !defined(RTC_SYNC)
#  define RTC_ASYNC (125 -1)
#  define RTC_SYNC (HSE_VALUE/(RTC_PRE * 125) -1)
# elif !defined(RTC_ASYNC) && defined(RTC_SYNC)
#  define RTC_ASYNC (HSE_VALUE/(RTC_PRE * (RTC_SYNC + 1)) -1)
# elif defined(RTC_ASYNC) && !defined(RTC_SYNC)
#  define RTC_SYNC (HSE_VALUE/(RTC_PRE * (RTC_ASYNC + 1)) -1)
# endif

/* Warn if values do not give 1 Hz */
# if (HSE_VALUE /RTC_PRE) % ((RTC_ASYNC + 1 ) * (RTC_SYNC + 1 )) != 0 || \
    (HSE_VALUE /RTC_PRE) / ((RTC_ASYNC + 1 ) * (RTC_SYNC + 1 )) != 1
#  warning Given RTC_SYNC/RTC_ASYNC do not give 1 Hz. Please set manual!
# endif

#elif RTC_CLK_SRC == RTCCLK_LSE
/* LSE as Clock source*/
# define RTC_RCC_PPREPARE() 0
# define RTC_CLK_SETUP() RtcLseSetup()
# define RTC_CLKSEL RCC_BDCR_RTCSEL_0
# if RTC_CLK_LSE != 32768
 /* Handle LSE with external clock or clock with other frequency as 32768*/
#   if (!defined(RTC_ASYNC) || !defined(RTC_SYNC))
#    warning Please define RTC_ASYNC and RTC_SYNC for 1 Hz RTC clock from LSE
#   endif
# else
#  undef  RTC_ASYNC
#  define RTC_ASYNC 0x7f
#  undef  RTC_SYNC
#  define RTC_SYNC  0xff
# endif
# define BDCR_MASK (RCC_BDCR_RTCEN  | RCC_BDCR_RTCSEL | RCC_BDCR_LSERDY | \
                    RCC_BDCR_LSEBYP | RCC_BDCR_LSEMOD)
# define BDCR_FLAG (RCC_BDCR_RTCEN  | RTC_CLKSEL      | RTC_BYPASSF |\
                    RTC_LSEMODF     | RCC_BDCR_LSERDY)
# define RTC_STATUS_START RTC_STATUS_HAS_QUEUE
# define RTC_STATUS_FLAG  RTC_STATUS_HAS_QUEUE
# define RTC_PRE_VAL 0
#elif RTC_CLK_SRC == RTCCLK_LSI
# define RTC_CLKSEL RCC_BDCR_RTCSEL_1
# define RTC_RCC_PPREPARE() RtcRccLsiPrepare()
# define RTC_CLK_SETUP() 0
# undef  RTC_ASYNC
# undef  RTC_SYNC
/* 32 kHz on F2/F4, 40(37?) kHz else.
 * Values used from AN3371/Table 3 Rev. 5
 */
# if defined(STM32F2) || defined(STM32F4)
#  define RTC_ASYNC 124
#  define RTC_SYNC  295
# else
#  define RTC_ASYNC 127
#  define RTC_SYNC  249
# endif
# define BDCR_MASK (RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL)
# define BDCR_FLAG (RCC_BDCR_RTCEN | RTC_CLKSEL)
# define RTC_STATUS_START (RTC_STATUS_HAS_QUEUE | RTC_STATUS_INACCURATE)
# define RTC_STATUS_FLAG  (RTC_STATUS_HAS_QUEUE | RTC_STATUS_INACCURATE)
# define RTC_PRE_VAL 0
#else
# warning No RTC Clock source defined!
#endif

/* Check if RTC_SYNC and RTC_ASYNC for overflow */
#if RTC_SYNC > SYNC_MAX
# warning RTC_SYNC is too large
#endif
#define RTC_ASYNC_VAL ( RTC_ASYNC * 0x10000)
#if RTC_ASYNC_VAL > 0x007F0000
# warning RTC_ASYNC is too large
#endif

typedef struct _stm32_rtc_dcb stm32_rtc_dcb;

struct _stm32_rtc_dcb {
    uint32_t flags;
};

/* We only have one RTC, so static allocation should be okay */
static stm32_rtc_dcb rtc_dcb = {RTC_STATUS_START};


/*!
 * \brief Setup HSE as RTC Clock
 *
 * \return 0 on success or -1 in case of an error.
 */
static int RtcRccHsePrepare(void)
{
    uint32_t cfgr;

    if((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY)
    /* If Hse was not already set, don't try to
     * fiddle with the HSE clock but return error.
     */
        return -1;
    cfgr = RCC->CFGR;
    cfgr &= ~RCC_CFGR_RTCPRE;
    cfgr |= RTC_PRE_VAL * RCC_CFGR_RTCPRE_0;
    RCC->CFGR = cfgr;
    return 0;
}

/*!
 * \brief Setup LSE as RTC Clock
 *
 * \return 0 on success or -1 in case of an error.
 */
static int RtcLseSetup(void)
{
    int i;
    uint32_t bdcr;
    uint32_t mask = (RCC_BDCR_RTCEN | RCC_BDCR_LSERDY |
                     RCC_BDCR_LSEBYP | RCC_BDCR_LSEMOD);
    uint32_t flag = (RTC_BYPASSF | RTC_LSEMODF);
    bdcr = RCC_BDCR;
    bdcr &= ~mask;
    bdcr |= flag;
    RCC_BDCR = bdcr;
    RCC_BDCR |= RCC_BDCR_LSEON;
    /* LSE startup time is 2 s typical */
    for(i = 0; i < 4000; i++) {
        NutSleep(1);
        if (RCC_BDCR & RCC_BDCR_LSERDY)
            return 0;
    }
    return -1;
}
static int RtcRccLsiPrepare(void)
{
    int i;
    if(RCC->CSR & RCC_CSR_LSIRDY)
            return 0;
    /* LSI startup is 40 us typical*/
    RCC->CSR |= RCC_CSR_LSION;
    for(i = 0; i < 10; i++) {
        NutSleep(1);
        if(RCC->CSR & RCC_CSR_LSIRDY)
            return 0;
    }
    return -1;
}
/*!
 * \brief Interrupt handler for RTC Alarm
 *
 */
static void Stm32RtcInterrupt(void *arg)
{
    NUTRTC *rtc = (NUTRTC *)arg;
    stm32_rtc_dcb *dcb = (stm32_rtc_dcb *)rtc->dcb;

    int do_alert = 0;
    if (RTC->ISR & RTC_ISR_ALRAF) {
        dcb->flags |= RTC_STATUS_AL0;
        /* Clear pending interrupt */
        RTC->ISR &= ~RTC_ISR_ALRAF;
        do_alert ++;
    }
#if NUM_ALARMS == 2
    if (RTC->ISR & RTC_ISR_ALRBF) {
        dcb->flags |= RTC_STATUS_AL1;
        /* Clear pending interrupt */
        RTC->ISR &= ~RTC_ISR_ALRBF;
        do_alert ++;
    }
#endif
    if(do_alert) {
        /* Clear Pending EXTI17 Interrupt*/
        EXTI->PR   =  (1 << 17);
        /* Signal alarm event queue */
        NutEventPostFromIrq(&rtc->alarm);
    }
}
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
    int res;

    stm32_rtc_dcb *dcb = (stm32_rtc_dcb *)rtc->dcb;
    /* Check for power failure */
    if (!(RTC->ISR & RTC_ISR_INITS)) {
        dcb->flags |= RTC_STATUS_PF;
    }
    if (sflags) {
        *sflags = dcb->flags;
        res = 0;
    } else {
        res = -1;
    }
    dcb->flags &= ~RTC_STATUS_START;
    dcb->flags |=  RTC_STATUS_FLAG;
    return res;
}

/*!
 * \brief Clear status of the Stm32 hardware clock.
 *
 * \param tm Points to a structure which contains the date and time
 *           information.
 *
 * \return 0 on success or -1 in case of an error.
 */
static int Stm32RtcClearStatus(NUTRTC *rtc, uint32_t sflags)
{
    /* Don't reset persistant flags*/
    sflags &= ~RTC_STATUS_FLAG;
    ((stm32_rtc_dcb *)rtc->dcb)->flags &= ~sflags;

    return 0;
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
        uint32_t tr, dr;
        /* Accessing RTC->TR locks shadow register until RTC->DR is accessed */
        tr = RTC->TR;
        dr = RTC->DR;
        tm->tm_mday = ((dr >>  0) & 0xf) + (((dr >>  4) & 0x3) * 10);
        tm->tm_mon  = ((dr >>  8) & 0xf) + (((dr >> 12) & 0x1) * 10);
        tm->tm_mon  -= 1;
        tm->tm_year = ((dr >> 16) & 0xf) + (((dr >> 20) & 0xf) * 10) + 100;
        tm->tm_hour = ((tr >> 16) & 0xf) + (((tr >> 20) & 0x7) * 10);
        if (tr & RTC_TR_PM)
            tm->tm_hour += 12;
        tm->tm_min  = ((tr >>  8) & 0xf) + (((tr >> 12) & 0x7) * 10);
        tm->tm_sec  = ((tr >>  0) & 0xf) + (((tr >>  4) & 0x7) * 10);
        tm->tm_wday = ((dr >> 13) & 0x7);
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
    PWR->CR |= PWR_CR_DBP;
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
    PWR->CR &= ~PWR_CR_DBP;
    /* Wait until shadow registers are updated with new value*/
    while ((RTC->ISR & RTC_ISR_RSF) != RTC_ISR_RSF);
    return 0;
}


/*!
 * \brief Get an alarm using the STM32 hardware clock.
 *
 * \param Idx Zero based index. Two alarms are supported>
 * \param tm Points to a structure to receive date and time information.
 *
 * \param aflags Points to an unsigned long that receives the enable flags.
 *
 * \return 0 on success or -1 in case of an error.
 */
static int Stm32RtcGetAlarm(NUTRTC *rtc, int idx, struct _tm *tm, int *aflags)
{
    uint32_t bcd_alarm;
    switch (idx) {
    case 0:  bcd_alarm = RTC->ALRMAR; break;
#if NUM_ALARMS == 2
    case 1:  bcd_alarm = RTC->ALRMBR; break;
#endif
    default: return -1;
    }
    if (aflags) {
        uint32_t flags = 0;
        if ((bcd_alarm & RTC_ALRMAR_MSK1) != RTC_ALRMAR_MSK1)
            flags |= RTC_ALARM_SECOND;
        if ((bcd_alarm & RTC_ALRMAR_MSK2) != RTC_ALRMAR_MSK2)
            flags |= RTC_ALARM_MINUTE;
        if ((bcd_alarm & RTC_ALRMAR_MSK3) != RTC_ALRMAR_MSK3)
            flags |= RTC_ALARM_HOUR;
        if ((bcd_alarm & RTC_ALRMAR_MSK4) != RTC_ALRMAR_MSK4) {
            if(bcd_alarm & RTC_ALRMAR_WDSEL)
                 flags |= RTC_ALARM_WDAY;
            else
                 flags |= RTC_ALARM_MDAY;
        }
        *aflags = flags;
    }
    if (tm) {
        tm->tm_sec   =  (bcd_alarm & RTC_ALRMAR_SU ) *  1 /RTC_ALRMAR_SU_0;
        tm->tm_sec  +=  (bcd_alarm & RTC_ALRMAR_ST ) * 10 /RTC_ALRMAR_ST_0;
        tm->tm_min   =  (bcd_alarm & RTC_ALRMAR_MNU) *  1 /RTC_ALRMAR_MNU_0;
        tm->tm_min  +=  (bcd_alarm & RTC_ALRMAR_MNT) * 10 /RTC_ALRMAR_MNT_0;
        tm->tm_hour  =  (bcd_alarm & RTC_ALRMAR_HU ) *  1 /RTC_ALRMAR_HU_0;
        tm->tm_hour +=  (bcd_alarm & RTC_ALRMAR_HT ) * 10 /RTC_ALRMAR_HT_0;
        if(bcd_alarm & RTC_ALRMAR_WDSEL)
            tm->tm_wday =(bcd_alarm & RTC_ALRMAR_DU) *  1 /RTC_ALRMAR_DU_0;
        else {
            tm->tm_mday =(bcd_alarm & RTC_ALRMAR_DU) *  1 /RTC_ALRMAR_DU_0;
            tm->tm_mday+=(bcd_alarm & RTC_ALRMAR_DT) * 10 /RTC_ALRMAR_DT_0;
        }
    }
    return 0;
}

/*!
 * \brief Set an alarm using the STM32 Ver 2 hardware clock.
 *
 * \param Idx   Zero based index. Two alarms are supported.
 *
 * \param tm    Points to a structure which contains the date and time
 *              information. May be NULL to clear the alarm.
 * \param aflgs Each bit enables a specific comparision.
 *              - Bit 0: Seconds
 *              - Bit 1: Minutes
 *              - Bit 2: Hours
 *              - Bit 3: Day of month
 *              - Bit 4: Month
 *              - Bit 7: Day of week (Sunday is zero)
 *              - Bit 8: Year
 *              - Bit 9: Day of year
 *
 * \return 0 on success or -1 in case of an error.
 */
static int Stm32RtcSetAlarm(NUTRTC *rtc, int idx, const struct _tm *tm, int aflags)
{
    uint32_t bcd_alarm;
    if (idx >= NUM_ALARMS)
        return -1;
    /* Either use Weekday or day of month*/
    if ((aflags & RTC_ALARM_WDAY) && (aflags & RTC_ALARM_MDAY))
        return -1;
    /* No Month or day of year */
    if ((aflags & RTC_ALARM_MONTH) || (aflags & RTC_ALARM_YDAY))
        return -1;
    /* Check minimum RTC_PRER if seconds are active */
    if (aflags & RTC_ALARM_SECOND) {
        if ((RTC->PRER & 0x7ffff) < 3)
            return -1;
    }

    bcd_alarm  = (tm->tm_sec  % 10) * RTC_ALRMAR_SU_0;
    bcd_alarm |= (tm->tm_sec  / 10) * RTC_ALRMAR_ST_0;
    bcd_alarm |= (tm->tm_min  % 10) * RTC_ALRMAR_MNU_0;
    bcd_alarm |= (tm->tm_min  / 10) * RTC_ALRMAR_MNT_0;
    bcd_alarm |= (tm->tm_hour % 10) * RTC_ALRMAR_HU_0;
    bcd_alarm |= (tm->tm_hour / 10) * RTC_ALRMAR_HT_0;
    if (aflags & RTC_ALARM_WDAY) {
        bcd_alarm |= RTC_ALRMAR_WDSEL;
        bcd_alarm |= (tm->tm_wday ) * RTC_ALRMAR_DU_0;
    }
    else {
        bcd_alarm  &= ~RTC_ALRMAR_WDSEL;
        bcd_alarm |= (tm->tm_mday % 10 ) * RTC_ALRMAR_DU_0;
        bcd_alarm |= (tm->tm_mday / 10 ) * RTC_ALRMAR_DT_0;
    }
    if (aflags & RTC_ALARM_SECOND)
        bcd_alarm &= ~RTC_ALRMAR_MSK1;
    else
        bcd_alarm |=  RTC_ALRMAR_MSK1;
    if (aflags & RTC_ALARM_MINUTE)
        bcd_alarm &= ~RTC_ALRMAR_MSK2;
    else
        bcd_alarm |=  RTC_ALRMAR_MSK2;
    if (aflags & RTC_ALARM_HOUR)
        bcd_alarm &= ~RTC_ALRMAR_MSK3;
    else
        bcd_alarm |=  RTC_ALRMAR_MSK3;
    if (aflags & (RTC_ALARM_WDAY | RTC_ALARM_MDAY))
        bcd_alarm &= ~RTC_ALRMAR_MSK4;
    else
        bcd_alarm |=  RTC_ALRMAR_MSK4;

    PWR->CR |= PWR_CR_DBP;
    /* Allow RTC Write Access */
    RTC->WPR = 0xca;
    RTC->WPR = 0x53;
    switch(idx) {
    case 0:
        RTC->CR &= ~RTC_CR_ALRAE;
        if (aflags != 0) {
            while ((RTC->ISR & RTC_ISR_ALRAWF) != RTC_ISR_ALRAWF);
            RTC->ALRMAR = bcd_alarm;
            RTC->CR |=  RTC_CR_ALRAE;
        }
        break;
#if NUM_ALARMS == 2
    case 1:
        RTC->CR &= ~RTC_CR_ALRBE;
        if (aflags != 0) {
            while ((RTC->ISR & RTC_ISR_ALRBWF) != RTC_ISR_ALRBWF);
            RTC->ALRMBR = bcd_alarm;
            RTC->CR |=  RTC_CR_ALRBE;
        }
        break;
#endif
    }
    RTC->WPR = 0;
    PWR->CR &= ~PWR_CR_DBP;
    return 0;
}

/*!
 * \brief Initialize the RTC in stm32f30x controller
 *
 * \return 0 on success or -1 in case of an error.
 *
 */
int Stm32RtcInit(NUTRTC *rtc)
{
    int res;

    if (NutRegisterIrqHandler(&sig_RTC, Stm32RtcInterrupt, rtc) != 0)
        return -1;
    /* Alarm Interrupt is on EXTI 17 Line, Rising Edge */
    EXTI->PR   =  (1 << 17);
    EXTI->IMR  |= (1 << 17);
    EXTI->RTSR |=  (1 << 17);
    EXTI->FTSR &= ~(1 << 17);
    NutIrqEnable(&sig_RTC);
    rtc->dcb   = &rtc_dcb;
    rtc->alarm = NULL;
    res  = RTC_RCC_PPREPARE();
    if (res) {
        /* Some failure happend */
        goto rtc_done;
    }
    /* Allow read access to the Backup Registers */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_DBP;
    /* Check for valid or changed settings */
    if ((RTC->PRER & 0x007f7fff) == (RTC_SYNC + RTC_ASYNC_VAL)) {
        if ((RTC->ISR & RTC_ISR_INITS) ==  RTC_ISR_INITS) {
            if ((RCC_BDCR  & BDCR_MASK) ==  BDCR_FLAG) {
                /* The RTC has been set before. Do not set it*/
                goto rtc_done;
            }
        }
    }
    if ((RCC_BDCR & BDCR_MASK) != BDCR_FLAG) {
        /* We need a backup domain reset to set the new source */
        RCC_BDCR |= RCC_BDCR_BDRST;
        RCC_BDCR &= ~RCC_BDCR_BDRST;
        /* Select clock source */
        RCC_BDCR |= RTC_CLKSEL;
    }
    rtc_dcb.flags &=  ~RTC_STATUS_START;
    /* Enable RTC CLK*/
    res = RTC_CLK_SETUP();
    if (res) {
        /* Some failure happend */
        goto rtc_done;
    }
    /* Allow write access to the Backup Registers */
    RTC->WPR = 0xca;
    RTC->WPR = 0x53;
    RCC_BDCR|= RCC_BDCR_RTCEN;

    RTC->ISR |= RTC_ISR_INIT;
    while (!(RTC->ISR & RTC_ISR_INITF));

    RTC->PRER = RTC_SYNC + RTC_ASYNC_VAL;
    /* Enable RTC ALARM A/B Interrupt*/
    RTC->CR |= (RTC_CR_ALRAIE | RTC_CR_ALRBIE);
    RTC->ISR &= ~RTC_ISR_INIT;
    RTC->WPR = 0; /* Disable  RTC Write Access */
rtc_done:
    /* Disable Backup domain write access*/
    PWR->CR &= ~PWR_CR_DBP;

    return res;
}

NUTRTC rtcStm32 = {
  /*.dcb           = */ NULL,               /*!< Driver control block */
  /*.rtc_init      = */ Stm32RtcInit,       /*!< Hardware initialization */
  /*.rtc_gettime   = */ Stm32RtcGetClock,   /*!< Read date and time */
  /*.rtc_settime   = */ Stm32RtcSetClock,   /*!< Set date and time */
  /*.rtc_getalarm  = */ Stm32RtcGetAlarm,   /*!< Read alarm date and time */
  /*.rtc_setalarm  = */ Stm32RtcSetAlarm,   /*!< Set alarm date and time */
  /*.rtc_getstatus = */ Stm32RtcGetStatus,  /*!< Read status flags */
  /*.rtc_clrstatus = */ Stm32RtcClearStatus,/*!< Clear status flags */
  /*.alarm         = */ NULL,               /*!< Handle for alarm event queue */
};
