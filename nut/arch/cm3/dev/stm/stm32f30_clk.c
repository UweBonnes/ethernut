/*
 * Copyright (C) 2013-2015 by Uwe Bonnes
 *                                (bon@elektron.ikp.physik.tu-darmstadt.de)
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

#include <cfg/arch.h>
#include <arch/cm3.h>

#include <arch/cm3/timer.h>
#include <arch/cm3/stm/stm32_clk.h>
#include <cfg/clock.h>
#include <cfg/rtc.h>

#if defined(MCU_STM32F0) || defined(MCU_STM32F3)
#include <arch/cm3/stm/stm32xxxx.h>
#else
#warning "Unknown STM32 family"
#endif

/* Prepare some defaults if configuration is incomplete */
#if !defined(SYSCLK_SOURCE)
#define SYSCLK_SOURCE SYSCLK_HSI
#endif

#undef HSI_VALUE
#define HSI_VALUE 8000000
#if defined(RCC_CFGR_SWS_HSI48)
#undef HSI48_VALUE
#define HSI48_VALUE 48000000
#endif
#undef HSE_STARTUP_TIMEOUT
#define HSE_STARTUP_TIMEOUT 5000


/* Equalize missing STM32F0 register bits */
#if !defined(RCC_CFGR_PPRE2)
#define RCC_CFGR_PPRE2 0
#endif
#if !defined(RCC_CFGR_PPRE2_0)
#define RCC_CFGR_PPRE2_0 0
#endif
#if defined( RCC_CFGR_PPRE) && !defined( RCC_CFGR_PPRE1)
#define RCC_CFGR_PPRE1 RCC_CFGR_PPRE
#endif
#if defined (RCC_CFGR_PPRE_DIV1) && !defined(RCC_CFGR_PPRE1_DIV1)
#define RCC_CFGR_PPRE1_DIV1 RCC_CFGR_PPRE_DIV1
#endif
#if defined (RCC_CFGR_PPRE_DIV2) && !defined(RCC_CFGR_PPRE1_DIV2)
#define RCC_CFGR_PPRE1_DIV2 RCC_CFGR_PPRE_DIV2
#endif
#if !defined(RCC_CFGR_PPRE2_DIV1)
#define RCC_CFGR_PPRE2_DIV1 0
#endif
#if !defined(RCC_CFGR_PPRE2_DIV2)
#define RCC_CFGR_PPRE2_DIV2 0
#endif
#if defined(FLASH_ACR_LATENCY) && !defined(FLASH_ACR_LATENCY_0)
#define FLASH_ACR_LATENCY_0 FLASH_ACR_LATENCY
#endif
#if defined(RCC_CFGR_PPRE_0) && !defined(RCC_CFGR_PPRE1_0)
#define RCC_CFGR_PPRE1_0 RCC_CFGR_PPRE_0
#endif
#if defined(RCC_CFGR2_PREDIV) && !defined(RCC_CFGR2_PREDIV1)
#define RCC_CFGR2_PREDIV1 RCC_CFGR2_PREDIV
#endif
#if defined(RCC_CFGR2_PREDIV_0) && !defined(RCC_CFGR2_PREDIV1_0)
#define RCC_CFGR2_PREDIV1_0 RCC_CFGR2_PREDIV_0
#endif

/*----------------  Clock Setup Procedure ------------------------------
 *
 * Clock system ist arranged like this:
 *
 *                         /Q------------------------------ USB
 *                         |               ,--------------- CPU
 *                         |               +--------------- SDIO
 *  (1)4-32MHz HSE-/M+-+-*N+---+-AHBPRES---+-- APB1PRESC--- APB1
 *                     /2          |           +-- ABP2PRESC--- ABP2
 *  8MHz HSI -----------+----------'       '-- ADCPRESC---- ADC
 *                      +-- Flash
 *                      '-- Option byte Loader
 *
 * M = 1..16
 * N = 2..16
 * Q = 1.1.5 (On F04/07/09 only 1)
 *
 * On F04/07/09, HSI and HSI48 enter PLL undivded and PLLDIV M is
 * active on HSE/HSI/HSI48
 * Same behaviour on F3XXxD\E with no HSI48;
 *
 *        ***** Setup of system clock configuration *****
 *
 * 1) Select system clock sources
 *
 * To setup system to use HSI call: SetSysClockSource( SYSCLK_HSI);
 * To setup system to use HSE call: SetSysClockSource( SYSCLK_HSE);
 *
 * To setup system to use the PLL output, first setup the PLL source:
 * SetPllClockSource( PLLCLK_HSI);
 * or
 * SetPllClockSource( PLLCLK_HSE);
 * Then call SetSysClockSource( SYSCLK_PLL);
 *
 * 2) Configure prescalers
 * After selecting the right clock sources, the prescalers need to
 * be configured:
 * Call SetSysClock(); to do this automatically.
 *
 */

/* Include common routines*/
#include "stm32_clk.c"

/*!
 * \brief  Update SystemCoreClock according to Clock Register Values
 *
 * This function reads out the CPUs clock and PLL registers and assembles
 * the actual clock speed values into the SystemCoreClock local variable.
 */
static void SystemCoreClockUpdate(void)
{
    RCC_TypeDef *rcc = (RCC_TypeDef*) RCC_BASE;
    uint32_t cfgr;
    uint32_t tmp = 0;
    uint32_t hpre;

    /* Get SYSCLK source ---------------------------------------------------*/
    cfgr = RCC->CFGR;
    switch(cfgr & RCC_CFGR_SWS) {
    case RCC_CFGR_SWS_HSE:
        tmp = HSE_VALUE;
        break;
#if defined(RCC_CFGR_SWS_HSI48)
    case RCC_CFGR_SWS_HSI48:
        tmp = HSI48_VALUE;
        break;
#endif
    case RCC_CFGR_SWS_PLL: {
        uint32_t cfgr2 = rcc->CFGR2;
        uint32_t prediv;
        uint32_t pllmull;

        prediv = (cfgr2 & RCC_CFGR2_PREDIV1) >> _BI32(RCC_CFGR2_PREDIV1_0);
        prediv += 1;
        pllmull = (cfgr & RCC_CFGR_PLLMUL) >> _BI32(RCC_CFGR_PLLMUL_0);
        pllmull += 2;
        if (pllmull > 16)
            pllmull = 16;
        if ((cfgr & RCC_CFGR_PLLSRC ) == RCC_CFGR_PLLSRC_HSI_DIV2 ) {
            tmp = HSI_VALUE;
            prediv = 2;
        }
        else if ((cfgr & RCC_CFGR_PLLSRC ) == RCC_CFGR_PLLSRC_HSE_PREDIV ) {
            tmp = HSE_VALUE;
        }
#if defined(RCC_CFGR_PLLSRC_HSI_PREDIV)
        else if ((cfgr & RCC_CFGR_PLLSRC ) == RCC_CFGR_PLLSRC_HSI_PREDIV ) {
            tmp = HSI_VALUE;
        }
#endif
#if defined(RCC_CFGR_PLLSRC_HSI48_PREDIV)
        else if ((cfgr & RCC_CFGR_PLLSRC ) == RCC_CFGR_PLLSRC_HSI48_PREDIV ) {
            tmp = HSI48_VALUE;
        }
#endif
        tmp = (tmp / prediv) * pllmull;
        break;
    }
    default:
        tmp = HSI_VALUE;
    }
    sys_clock = tmp;
    hpre = (cfgr & RCC_CFGR_HPRE) >> _BI32(RCC_CFGR_HPRE_0);
    clk_shift[NUT_HWCLK_CPU] = AHBPrescTable[hpre];
    tmp = (RCC->CFGR & RCC_CFGR_PPRE1) >> _BI32( RCC_CFGR_PPRE1_0);
    clk_shift[NUT_HWCLK_PCLK1] = APBPrescTable[tmp];
    clk_shift[NUT_HWCLK_TCLK1] = GetTimerShift(clk_shift[NUT_HWCLK_PCLK1]);
    tmp = (RCC->CFGR & RCC_CFGR_PPRE2) >> _BI32( RCC_CFGR_PPRE2_0);
    clk_shift[NUT_HWCLK_PCLK2] = APBPrescTable[tmp];
    clk_shift[NUT_HWCLK_TCLK2] = GetTimerShift(clk_shift[NUT_HWCLK_PCLK2]);
}

#if defined(RCC_CFGR_SWS_HSI48)
/*!
 * \brief Control HSI48 clock.
 *
 * \param  ena 0 disable clock, any other value enable it.
 * \return 0 on success, -1 on HSI start failed.
 */
int CtlHsi48Clock( uint8_t ena)
{
    int rc = 0;

    uint32_t tout = HSE_STARTUP_TIMEOUT;
    volatile uint32_t HSIStatus = 0;

    if( ena) {
        /* Enable HSI */
        RCC->CR2 |= RCC_CR2_HSI48ON;

        /* Wait till HSI is ready or time out is reached */
        do {
            tout--;
            HSIStatus = RCC->CR2 & RCC_CR2_HSI48RDY;
        } while((HSIStatus == 0) && (tout > 0));

        if ((RCC->CR2 & RCC_CR2_HSI48RDY) == ENABLE) {
            /* HSI failed to start */
            rc = -1;
        }
    }
    else {
        /* Disable HSE clock */
        RCC->CR2 &= ~RCC_CR2_HSI48ON;
    }

    return rc;
}
#endif

/* Functional same as F1 */
/*!
 * \brief Control PLL clock.
 *
 * \param  ena 0 disable clock, any other value enable it.
 * \return 0 on success, -1 on PLL start failed.
 */
int CtlPllClock( uint8_t ena)
{
    int rc = 0;

    uint32_t tout = HSE_STARTUP_TIMEOUT;
    volatile uint32_t PLLStatus = 0;

    if( ena) {
        /* Enable PLL */
        RCC->CR |= RCC_CR_PLLON;

        /* Wait till PLL is ready or time out is reached */
        do {
            tout--;
            PLLStatus = RCC->CR & RCC_CR_PLLRDY;
        } while((PLLStatus == 0) && (tout > 0));

        if ((RCC->CR & RCC_CR_PLLRDY) == ENABLE) {
            /* PLL failed to start */
            rc = -1;
        }
    }
    else {
        /* Disable HSE clock */
        RCC->CR &= ~RCC_CR_PLLON;
    }

    return rc;
}


/*!
 * \brief  Configures the System clock source: HSE or HSI.
 * \note   This function should be used with PLL disables
 * \param  src is one of PLLCLK_HSE, PLLCLK_HSI.
 * \return 0 if clock is running ale -1.
 */
int SetPllClockSource( int src)
{
    int rc = -1;
    uint32_t cfgr = RCC->CFGR;
    cfgr &= ~RCC_CFGR_PLLSRC;
    if (src == PLLCLK_HSI_DIV2) {
        rc = CtlHsiClock(ENABLE);
    }
    else if ((src == PLLCLK_HSE_PREDIV) || (src == PLLCLK_HSE)){
        rc = CtlHseClock(ENABLE);
        if (rc==0) {
            cfgr |=  RCC_CFGR_PLLSRC_HSE_PREDIV;
        }
    }
#if defined(RCC_CFGR_PLLSRC_HSI_PREDIV)
    else if (src == PLLCLK_HSI_PREDIV) {
        rc = CtlHsiClock(ENABLE);
        if (rc==0) {
            cfgr |= RCC_CFGR_PLLSRC_HSI_PREDIV;
        }
    }
#endif
#if defined(RCC_CFGR_PLLSRC_HSI48_PREDIV)
    else if (src == PLLCLK_HSI48_PREDIV) {
        rc = CtlHsi48Clock(ENABLE);
        if (rc==0) {
            cfgr |= RCC_CFGR_PLLSRC_HSI48_PREDIV;
         }
    }
#endif
    RCC->CFGR = cfgr;
    return rc;
}

/*!
 * \brief  Configures the System clock source: HSI, HS or PLL.
 * \note   This function should be used only after reset.
 * \param  src is one of SYSCLK_HSE, SYSCLK_HSI or SYSCLK_PLL.
 * \return 0 if selected clock is running else -1.
 */
int SetSysClockSource( int src)
{
    int rc = -1;
    uint32_t cfgr;

    cfgr = RCC->CFGR;
    cfgr &= ~RCC_CFGR_SW;
    if (src == SYSCLK_HSE) {
        rc = CtlHseClock(ENABLE);
        if (rc == 0) {
            /* Select HSE as system clock source */
            cfgr |= RCC_CFGR_SW_HSE;
            RCC->CFGR = cfgr;

            /* Wait till HSE is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);
        }
    }
    else if (src == SYSCLK_HSI) {
        rc = CtlHsiClock(ENABLE);
        if (rc == 0) {
            /* Select HSI as system clock source */
            cfgr |= RCC_CFGR_SW_HSI;
            RCC->CFGR = cfgr;

            /* Wait till HSI is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
        }
    }
#if defined(RCC_CFGR_SWS_HSI48)
    else if (src == SYSCLK_HSI48) {
        rc = CtlHsi48Clock(ENABLE);
        if (rc == 0) {
            /* 1 Flash Waitstate needed */
            FLASH->ACR |= FLASH_ACR_LATENCY| FLASH_ACR_PRFTBE;

            /* Select HSI48 as system clock source */
            cfgr |= RCC_CFGR_SW_HSI48;
            RCC->CFGR = cfgr;
            /* Wait till HSI48 is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI48);
        }
    }
#endif
    else if (src == SYSCLK_PLL) {
        rc = CtlPllClock(ENABLE);
        if (rc == 0) {
            /* Select HSI as system clock source */
            cfgr |= RCC_CFGR_SW_PLL;
            RCC->CFGR = cfgr;

            /* Wait till PLL is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
        }
    }

    /* Update core clock information */
    SystemCoreClockUpdate();

    return rc;
}

#if (SYSCLK_SOURCE == SYSCLK_HSI) || (SYSCLK_SOURCE == SYSCLK_HSE) || (SYSCLK_SOURCE == SYSCLK_HSI48)
/*!
 * \brief  Configures the System clock coming from HSE or HSI oscillator.
 *
 * Enable HSI/HSE clock and setup HCLK, PCLK2 and PCLK1 prescalers.
 *
 * \param  None.
 * \return 0 on success, -1 on fault of HSE.
 */
int SetSysClock(void)
{
    int rc = 0;
    register uint32_t cfgr;

    /* Set up RTC clock source and eventually LSE and LSI */
    SetRtcClockSource(RTCCLK_SOURCE);

/* Todo: Check Voltage range! Here 2.7-3.6 Volt is assumed */
/* For 2.7-3.6 Volt up to 30 MHz no Wait state required */
    cfgr = RCC->CFGR;

    cfgr &= ~(RCC_CFGR_HPRE|RCC_CFGR_PPRE1|RCC_CFGR_PPRE2);

    /* HCLK = SYSCLK */
    cfgr |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK */
    cfgr |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    /* PCLK1 = HCLK */
    cfgr |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

    RCC->CFGR = cfgr;

    rc = SetSysClockSource(SYSCLK_SOURCE);

    return rc;
}
#elif (SYSCLK_SOURCE == SYSCLK_PLL)

/**
  * @brief  Sets System clock frequency to 48 MHz (F0)/ 72MHz (F3) and
  *         configure HCLK, PCLK2 and PCLK1 prescalers.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
#if defined(MCU_STM32F0)
# define PLLCLK_MAX 48000000
#else
# if (HSE_VALUE > 0) || defined(RCC_CFGR_PLLSRC_HSI_PREDIV)
#  define PLLCLK_MAX  72000000
# else
/*  HSI is divided by 2. Maximum PLLMULT is 16*/
#  define PLLCLK_MAX 64000000
# endif
#endif

#if !defined(SYSCLK_FREQ)
#define SYSCLK_FREQ PLLCLK_MAX
#endif

#if (defined(PLLCLK_MULT) && !defined(PLLCLK_DIV)) || (!defined(PLLCLK_MULT) && defined(PLLCLK_DIV))
#warning Please either provide both PLLCLK_MULT and PLLCLK_DIV or none
#endif

#if !defined(PLLCLK_SOURCE)
#warning No PLLCLK_SOURCE defined
#endif

#if defined(RCC_CFGR_PLLSRC_HSI_PREDIV)
# if (PLLCLK_SOURCE==PLLCLK_HSE) || (PLLCLK_SOURCE==PLLCLK_HSI)
#  warning PLLCLK_SOURCE PLLCLK_HSE or PLLCLK_HSI illegal for this family
# elif !(PLLCLK_SOURCE) || (PLLCLK_SOURCE == PLLCLK_AUTO)
#  undef PLLCLK_SOURCE
#  if HSE_VALUE > 0
#   define PLLCLK_SOURCE PLLCLK_HSE_PREDIV
#  else
#   define PLLCLK_SOURCE PLLCLK_HSI_PREDIV
#  endif
# endif
#else
# if (PLLCLK_SOURCE==PLLCLK_HSI_PREDIV)
#  warning PLLCLK_SOURCE PLLCLK_HSI_PREDIV illegal for this device
# elif !defined (PLLCLK_SOURCE) || (PLLCLK_SOURCE == PLLCLK_AUTO)
#  undef PLLCLK_SOURCE
#  if HSE_VALUE > 0
#   define PLLCLK_SOURCE PLLCLK_HSE
#  else
#   define PLLCLK_SOURCE PLLCLK_HSI_DIV2
#  endif
# endif
#endif

#if (PLLCLK_SOURCE==PLLCLK_HSE_PREDIV) || (PLLCLK_SOURCE==PLLCLK_HSE)
# define PLLCLK_IN  HSE_VALUE
 #if !defined(PLLCLK_MULT)
 /* No user provided values, try to calculate for some discrete values */
  #if (PLLCLK_IN == 8000000)
   #if (SYSCLK_FREQ == 72000000)
    #define PLLCLK_MULT 9
    #define PLLCLK_DIV  1
   #elif (SYSCLK_FREQ == 48000000)
    #define PLLCLK_MULT 6
    #define PLLCLK_DIV  1
   #endif
  #else
   #warning Please provide PLLCLK_MULT and PLLCLK_DIV for your settings!
  #endif
 #endif
#elif (PLLCLK_SOURCE==PLLCLK_HSI_DIV2) || (PLLCLK_SOURCE==PLLCLK_HSI_PREDIV)
 #define PLLCLK_IN HSI_VALUE
 #if !defined(PLLCLK_MULT)
 /* No user provided values, try to calculate for some discrete values */
  #define PLLCLK_DIV 2
  #define PLLCLK_MULT ((SYSCLK_FREQ / PLLCLK_IN) * PLLCLK_DIV)
  #if ((PLLCLK_IN * PLLCLK_MULT )/ PLLCLK_DIV) != SYSCLK_FREQ
   #warning Target frequency not reach exactly
  #endif
 #endif
#elif (PLLCLK_SOURCE==PLLCLK_HSI48_PREDIV)
 #if !defined(PLLCLK_MULT == 0 )
  #warning Use PLLCLK_MULT and PLLCLK_DIV with HSI48 as PLLCLK!
 #endif
#endif

#if (PLLCLK_DIV < 1)  || (PLLCLK_DIV > 16)
# warning "Illegal PLLCLK_DIV value"
#endif

#if (PLLCLK_MULT < 2)  || (PLLCLK_MULT > 16)
# warning "Illegal PLLCLK_MULT value"
#endif

#if (PLLCLK_IN /PLLCLK_DIV * PLLCLK_MULT > PLLCLK_MAX)
#warning "Clock frequency too high"
#endif
#if (PLLCLK_IN /PLLCLK_DIV * PLLCLK_MULT >48000000)
#define NUT_FLASH_LATENCY  FLASH_ACR_LATENCY_1
#elif (PLLCLK_IN /PLLCLK_DIV * PLLCLK_MULT >24000000)
#define NUT_FLASH_LATENCY  FLASH_ACR_LATENCY_0
#else
#define NUT_FLASH_LATENCY 0
#endif

/*
   Easy Approach:
   Try to reach fpll = 72 Mhz with M/N.
*/
int SetSysClock(void)
{
    int rc = 0;
    uint32_t rcc_reg;

    /* Set up RTC clock source and eventually LSE and LSI */
    SetRtcClockSource(RTCCLK_SOURCE);

    rcc_reg =  RCC->CFGR;
    rcc_reg &= ~(RCC_CFGR_PLLMUL |RCC_CFGR_PLLSRC |RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1 |RCC_CFGR_HPRE);
    rcc_reg |= ((PLLCLK_MULT -2) * RCC_CFGR_PLLMUL_0);
#if defined(MCU_STM32F0)
/* APB Bus can run with full SYSCLK speed (48 MHz) */
#else
    if (SYSCLK_FREQ > 36000000)
/* APB1 Bus (Slow APB) bus can only run with max 36 MHz) */
        rcc_reg |= RCC_CFGR_PPRE1_DIV2;
#endif
    RCC->CFGR = rcc_reg;
    if (SetPllClockSource(PLLCLK_SOURCE) != 0)
        return -1;

    rcc_reg = FLASH->ACR;
    rcc_reg &= ~FLASH_ACR_LATENCY;
    rcc_reg |= NUT_FLASH_LATENCY | FLASH_ACR_PRFTBE ;
    FLASH->ACR = rcc_reg;

    rcc_reg = RCC->CFGR2;
    rcc_reg  &= ~(RCC_CFGR2_PREDIV1);
    /* HCLK = SYSCLK, PCLK2 = HCLK , PCLK1 = HCLK/2 */
    rcc_reg |= (PLLCLK_DIV - 1);
    RCC->CFGR2 = rcc_reg;

    /* Start PLL, wait ready and switch to it as clock source */
    rc = SetSysClockSource(SYSCLK_SOURCE);
    if (rc) {
        /* Something went wrong with the PLL startup! */
        SetSysClockSource(SYSCLK_HSI);
        return rc;
    }

    return rc;
}
#endif /* (SYSCLK_SOURCE == SYSCLK_HSI) || (SYSCLK_SOURCE == SYSCLK_HSE) */

/**
  * @brief  Some devices may use HSI/LSE and SYSCLK additional to PCLKx
  *
  * @param  bi  Bitindex of RCC_CFGR3_xxxSW_1
  * @retval Clock frequency of choosen clk or 0 if PCLKx choosen
  */
uint32_t Stm32ClockSwitchGet(int bi)
{
    if (bi) {
        int clksrc;
        bi--;
        clksrc = RCC->CFGR3 >> bi;
        clksrc &= 3;
        if (1 == clksrc)
            return  STM_ClockGet(NUT_HWCLK_CPU);
        else if (2 == clksrc)
            return HSI_VALUE;
        else if (3 == clksrc)
            return LSE_VALUE;
    }
    return 0;
}
