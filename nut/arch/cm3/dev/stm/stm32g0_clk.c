/*
 * Copyright (C) 2020 by Uwe Bonnes
 *                              (bon@elektron.ikp.physik.tu-darmstadt.de)
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

#include <sys/nutdebug.h>

#include <arch/cm3.h>

#include <arch/cm3/timer.h>
#include <arch/cm3/stm/stm32_clk.h>
#include <cfg/clock.h>
#include <dev/rtc.h>
#include <dev/irqreg.h>

#include <arch/cm3/stm/stm32xxxx.h>

/* Prepare some defaults if configuration is incomplete */

#define HSI_VALUE 16000000

/* Prepare system limits*/
#define PLLMULT_MIN             8
#define PLLMULT_MAX            86
#define PLLIN_MIN         2660000
#define PLLIN_MAX        16000000
#if   (STM32_POWERSCALE == 1)
# define SYSCLK_MAX      64000000
# define FLASH_BASE_FREQ 24000000
#  define PLLVCO_MAX    344000000
#elif (STM32_POWERSCALE == 2)
# define SYSCLK_MAX      16000000
# define FLASH_BASE_FREQ  8000000
# define PLLVCO_MAX    128000000
#else
# warning Invalid STM32_POWERSCALE
#endif

#if (SYSCLK_SOURCE == SYSCLK_HSE) || ((SYSCLK_SOURCE == SYSCLK_PLL) && (PLLCLK_SOURCE == PLLCLK_HSE))
# if (HSE_VALUE > (48*1000*1000))
#  warning HSE_VALUE too high!
# elif !defined(HSE_BYPASS) && (HSE_VALUE < (4*1000*1000))
#  warning HSE_VALUE too low for crystal/ceramic resonator!
# elif HSE_VALUE < 1
#  warning HSE_VALUE too low!
# endif
#endif

/* Define PLL Input Clock */
#if (PLLCLK_SOURCE == PLLCLK_HSE)
# if (HSE_VALUE < PLLIN_MIN)
#  warning HSE_VALUE too low for PLL!
# endif
# define PLLCLK_IN HSE_VALUE
#elif (PLLCLK_SOURCE == PLLCLK_HSI)
# define PLLCLK_IN HSI_VALUE
#else
# define PLLCLK_IN 0
#endif

#if (SYSCLK_SOURCE == SYSCLK_PLL)
/* 7 < PLL_MULT < PLLMULT_MAX
 * 0 < PLL_PREDIV < 9
 * 0 < (PLLCLK_DIV / 2) < 5
 */
# if !defined(SYSCLK_FREQ) && !defined(PLLCLK_PREDIV) && !defined(PLLCLK_MULT) && !defined(PLLCLK_DIV)
#  define SYSCLK_FREQ SYSCLK_MAX
#  if (SYSCLK_FREQ == 64000000)
#   if ((PLLCLK_IN % 8000000) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / 8000000)
#    define PLLCLK_MULT   16
#    define PLLCLK_DIV     2
#   elif ((PLLCLK_IN % 6000000) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / 6000000)
#    define PLLCLK_MULT   32
#    define PLLCLK_DIV     3
#   elif ((PLLCLK_IN % 5000000) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / 5000000)
#    define PLLCLK_MULT   64
#    define PLLCLK_DIV     5
#   elif ((PLLCLK_IN % 4000000) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / 4000000)
#    define PLLCLK_MULT   32
#    define PLLCLK_DIV     2
#   else
#    warning Provide PLLCLK_PREDIV, PLLCLK_MULT and PLLCLK_DIV to reach 64 MHz
#   endif
#  elif (SYSCLK_FREQ == 26000000)
/*  8 MHz PLL Input, 104 MHz PLL VCO, 26 MHz PLL R output */
#   if   ((PLLCLK_IN % 8000000) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / 8000000)
#    define PLLCLK_MULT   13
#    define PLLCLK_DIV     4
/*  7 MHz PLL Input, 98 MHz PLL VCO, 24,5 MHz PLL R output */
#   elif ((PLLCLK_IN % 7000000) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / 7000000)
#    define PLLCLK_MULT   14
#    define PLLCLK_DIV     4
/*  6 MHz PLL Input, 96 MHz PLL VCO, 24 MHz PLL R output */
#   elif ((PLLCLK_IN % 6000000) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / 6000000)
#    define PLLCLK_MULT   16
#    define PLLCLK_DIV     4
/*  5 MHz PLL Input, 100 MHz PLL VCO, 25 MHz PLL R output */
#   elif ((PLLCLK_IN % 5000000) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / 5000000)
#    define PLLCLK_MULT   20
#    define PLLCLK_DIV     4
/*  4 MHz PLL Input, 104 MHz PLL VCO, 26 MHz PLL R output */
#   elif ((PLLCLK_IN % 4000000) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / 4000000)
#    define PLLCLK_MULT   26
#    define PLLCLK_DIV     4
#   else
#    warning Please Provide PLLCLK_PREDIV, PLLCLK_MULT and PLLCLK_DIV
#   endif
#  endif
# endif

# if   (PLLCLK_IN / PLLCLK_PREDIV) < PLLIN_MIN
#  warning PLL Input frequency too low
# elif (PLLCLK_IN / PLLCLK_PREDIV) > 16000000
#  warning PLL Input frequency too high
# elif ((PLLCLK_IN / PLLCLK_PREDIV) * PLLCLK_MULT) < 64000000
#  warning PLL VCO frequency too low
# elif ((PLLCLK_IN / PLLCLK_PREDIV) * PLLCLK_MULT) > PLLVCO_MAX
#  warning PLL VCO frequency too high
# elif (((PLLCLK_IN / PLLCLK_PREDIV) * PLLCLK_MULT) / PLLCLK_DIV) > SYSCLK_MAX
#  warning "SYSCLK_FREQ overclocked"
# endif
#endif

/* Provide fallback values for PLLCLK PREDIV, MULT, DIV */
#if !defined(PLLCLK_PREDIV)
# define PLLCLK_PREDIV 1
#endif
#if !defined(PLLCLK_MULT)
# define PLLCLK_MULT   8
#endif
#if !defined(PLLCLK_DIV)
# define PLLCLK_DIV    2
#endif

/* Check PLL factors */
#if  (PLLCLK_PREDIV < 1) || (PLLCLK_PREDIV > 8)
# warning Wrong PLLCLK_PREDIV
#elif (PLLCLK_MULT < 8) || (PLLCLK_MULT > PLLMULT_MAX)
# warning Wrong PLLCLK_MULT
#elif (PLLCLK_DIV < 2) || (PLLCLK_DIV % 2) || (PLLCLK_DIV > 8)
# warning Wrong PLLCLK_DIV
#endif

/*----------------  Clock Setup Procedure ------------------------------
 *
 * Clock system ist arranged like this:
 *
 *                        /Q------------------------------ USB
 *                        |               ,--------------- CPU
 *                        |               +--------------- SDIO
 * 4-26   MHz HSE -+--/M*N/R--+-AHBPRES---+-- APB1PRESC--- APB1
 *                 |          |           +-- ABP2PRESC--- ABP2
 * 16MHz HSI ------+----------'           '-- ADCPRESC---- ADC
 *
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
    RCC_TypeDef *rcc =  (RCC_TypeDef *)RCC_BASE;
    uint32_t tmp = 0;
    uint32_t cfgr;

    /* Get SYSCLK source ---------------------------------------------------*/
    cfgr = RCC->CFGR & RCC_CFGR_SWS;
    switch (cfgr) {
    case RCC_CFGR_SWS_HSE:
        tmp = HSE_VALUE;
        break;
    case RCC_CFGR_SWS_PLL: {
        uint32_t pllcfgr = rcc->PLLCFGR;
        uint32_t n = (pllcfgr & RCC_PLLCFGR_PLLN) >> _BI32(RCC_PLLCFGR_PLLN_0);
        uint32_t m = (pllcfgr & RCC_PLLCFGR_PLLM) >> _BI32(RCC_PLLCFGR_PLLM_0);
        uint32_t r = (pllcfgr & RCC_PLLCFGR_PLLR) >> _BI32(RCC_PLLCFGR_PLLR_0);

        m += 1;
        r = 2 * (r + 1);
        if ((pllcfgr & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSE) {
            tmp = HSE_VALUE;
        } else {
            tmp = HSI_VALUE;
        }
        tmp = ((tmp * n) / m) / r;
        break;
    }
    default:
        tmp = HSI_VALUE;
    }
    sys_clock = tmp;
    SetClockShift();
}

/*!
 * \brief  Configures the System clock source: HSE or HSI.
 * \note   This function should be used with PLL disables
 * \param  src is one of PLLCLK_HSE, PLLCLK_HSI.
 * \return 0 if clock is running else -1.
 */
static int SetPllClockSource(int src)
{
    int rc = -1;
    uint32_t pllcfgr;

    /* Enable PLLCLK source and switch to that source */
    switch (src) {
    case (PLLCLK_HSE):
        rc = CtlHseClock(ENABLE);
        if (rc) {
            return rc;
        }
        rc = rcc_set_and_wait_rdy_value(
            &RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSE,
            RCC_CFGR_SWS, RCC_CFGR_SWS_HSE, HSE_STARTUP_TIMEOUT);
        if (rc) {
            return rc;
        }
        break;
    case (PLLCLK_HSI) :
        CtlHsiClock(ENABLE);
        rc = rcc_set_and_wait_rdy_value(
            &RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI,
            RCC_CFGR_SWS, RCC_CFGR_SWS_HSI, HSE_STARTUP_TIMEOUT);
        if (rc) {
            return rc;
        }
        break;
    }
    pllcfgr =  RCC->PLLCFGR;
    pllcfgr &= ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP);
    pllcfgr |=  ( PLLCLK_PREDIV - 1)   * RCC_PLLCFGR_PLLM_0;
    pllcfgr |=    PLLCLK_MULT          * RCC_PLLCFGR_PLLN_0;
    pllcfgr |=  ((PLLCLK_DIV / 2) - 1) * RCC_PLLCFGR_PLLR_0;
    pllcfgr |=  RCC_PLLCFGR_PLLREN;

    switch (src) {
    case (PLLCLK_HSE):
        pllcfgr |= RCC_PLLCFGR_PLLSRC_HSE;
        break;
    case (PLLCLK_HSI):
        pllcfgr |= RCC_PLLCFGR_PLLSRC_HSI;
        break;
    }
    RCC->PLLCFGR = pllcfgr;
    return rc;
}

static void SetVos(uint32_t frequency)
{
    uint32_t cr = PWR->CR1;
    cr &= ~(PWR_CR1_VOS);
    if (frequency < 26000000) {
        cr |= 2 * PWR_CR1_VOS_0;
    } else {
        cr |= 1 * PWR_CR1_VOS_0;
    }
    PWR->CR1 = cr;
    /* Wait until VOSF is ready. */
    while (PWR->SR2 & PWR_SR2_VOSF) {_NOP();}
}

static void SetLatency(uint32_t value)
{
    uint32_t flash_acr;
    flash_acr = FLASH->ACR;
    flash_acr &= ~FLASH_ACR_LATENCY;
    flash_acr |= value;
    FLASH->ACR = flash_acr;
}

/*!
 * \brief  Configures the System clock source: HSI, HSE or PLL.
 * \note   This function should be used only after reset.
 * \param  src is one of SYSCLK_HSE, SYSCLK_HSI or SYSCLK_PLL.
 * \return 0 if selected clock is running else -1.
 */
static int SetSysClockSource(int src)
{
    int rc = -1;
    uint32_t old_latency, new_latency, new_sysclk;

    /* Set up RTC clock source and eventually LSE and LSI */
    SetRtcClockSource(RTCCLK_SOURCE);
    SystemCoreClockUpdate();
    old_latency = FLASH->ACR & FLASH_ACR_LATENCY;
    /* Get new sys_clockk*/
    switch (src) {
    case (SYSCLK_HSE):
        new_sysclk = HSE_VALUE;
        break;
    case (SYSCLK_PLL):
        new_sysclk = ((PLLCLK_IN / PLLCLK_PREDIV) * PLLCLK_MULT) / PLLCLK_DIV;
        break;
    default:
        new_sysclk = HSI_VALUE;
        break;
    }
    new_latency = (new_sysclk - 1)/ FLASH_BASE_FREQ;

    if (sys_clock < new_sysclk) {
        SetVos(new_sysclk);
    }
    if (new_latency > old_latency) {
        SetLatency(new_latency);
    }
    rc = SwitchSystemClock(src);
    if (sys_clock > new_sysclk) {
        SetVos(new_sysclk);
    }
    if (new_latency < old_latency) {
        SetLatency(new_latency);
    }
    SetBusDividers(AHB_DIV, APB1_DIV, APB2_DIV);
    /* Update core clock information */
    SystemCoreClockUpdate();
    return rc;
}
