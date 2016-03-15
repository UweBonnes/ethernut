/*
 * Copyright (C) 2015 by Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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

#include <cfg/arch.h>
#include <arch/cm3.h>

#include <arch/cm3/timer.h>
#include <arch/cm3/stm/stm32_clk.h>
#include <cfg/clock.h>
#include <dev/rtc.h>
#include <dev/irqreg.h>

#include <arch/cm3/stm/stm32xxxx.h>

/* Prepare some defaults if configuration is incomplete */

#if !defined(HSE_VALUE)
# define HSE_VALUE 0
#endif

#if !defined(HSI_VALUE)
# define HSI_VALUE 16000000
#endif

#if !defined(HSE_STARTUP_TIMEOUT)
/*!< Time out for HSE start up, in loops*/
# define HSE_STARTUP_TIMEOUT ((uint32_t)5000)
#endif

#if !defined(AHB_PRE)
#define AHB_PRE 1
#endif

#if !defined(APB1_PRE)
#define APB1_PRE 1
#endif

#if !defined(APB2_PRE)
#define APB2_PRE 1
#endif
#if !defined(SYSCLK_SOURCE)
# define SYSCLK_SOURCE SYSCLK_MSI
#endif

#if !defined(MSI_RANGE)
# define MSI_RANGE MSI_4M
#endif

#define NUM_MSI_FREQ 12
static const uint32_t MSI_FREQUENCY[NUM_MSI_FREQ] = {
    /* Values from STM32L476 Rev. 2 datasheet, table 47
     *  Exact values only for MSI PLL on.
     */
    98304,
    196608,
    393216,
    786432,
    1016000,
    1999000,
    3998000,
    7995000,
    15991000,
    23986000,
    32014000,
    48005000
};

#if (PLLCLK_SOURCE == PLLCLK_HSE)
# define PLLCLK_IN HSE_VALUE
#elif (PLLCLK_SOURCE == PLLCLK_HSI)
# define PLLCLK_IN HSI_VALUE
#else
# define PLLCLK_IN 3998000
# undef MSI_RANGE
# define MSI_RANGE MSI_4M
#endif

#if !defined(STM32_VRANGE) || STM32_VRANGE == 1
/* Give some tolerance to that MSI_32M use 1 WS and MSI_48M 2 WS*/
# define FLASH_BASE_FREQ 16007001
# define SYSCLK_MAX      80000000
# define PLLVCO_MAX     344000000
# if !defined(SYSCLK_FREQ)
/* 8 MHz PLL Input, 160 MHz PLL VCO, 80 MHz PLL R output */
#  if (PLLCLK_IN == 16000000)
#   define PLLM 2
#   define PLLN 20
#   define PLLR 2
#  elif (PLLCLK_IN == 8000000)
#   define PLLM 1
#   define PLLN 20
#   define PLLR 2
#  elif (PLLCLK_IN == 3998000)
#   define PLLM 1
#   define PLLN 40
#   define PLLR 2
#  endif
# endif
#elif STM32_VRANGE == 2
# define FLASH_BASE_FREQ  6000000
# define SYSCLK_MAX      26000000
# define PLLVCO_MAX     128000000
# if !defined(SYSCLK_FREQ)
/* 8 MHz PLL Input, 104 MHz PLL VCO, 26 MHz PLL R output */
#  if (PLLCLK_IN == 16000000)
#   define PLLM 2
#   define PLLN 13
#   define PLLR 4
#  elif (PLLCLK_IN == 8000000)
#   define PLLM 1
#   define PLLN 13
#   define PLLR 4
#  elif (PLLCLK_IN == 3998000)
#   define PLLM 1
#   define PLLN 26
#   define PLLR 2
#  endif
# endif
#else
# warning Unhandled STM32_VRANGE
#endif

#if (PLLCLK_IN / PLLM) < 3998000
# warning PLL Input frequency too high
#endif

#if (PLLCLK_IN / PLLM) > 16000000
# warning PLL Input frequency too high
#endif

#if ((PLLCLK_IN / PLLM) * PLLn) > PLLVCO_MAX
# warning PLL VCO frequency too high
#endif

#define PCLK1_MAX   SYSCLK_MAX
#define PCLK2_MAX   SYSCLK_MAX

#if defined(SYSCLK_FREQ) && SYSCLK_FREQ > SYSCLK_MAX
# warning "SYSCLK_FREQ overclocked"
#endif

static uint32_t msi_clock;

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
 *                 |
 * MSI ------------
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

/**
  * @brief  Get Msi Frequency
  *
  * @retval MsiFrequency
  */
static void MsiFrequencyUpdate(void)
{
    if (RCC->CR & RCC_CR_MSIRDY) {
        uint8_t range;
        if (RCC->CR & RCC_CR_MSIRGSEL) {
            range = (RCC->CR & RCC_CR_MSIRANGE) / RCC_CR_MSIRANGE_1;
        } else {
            range = (RCC->CSR & RCC_CSR_MSISRANGE) / RCC_CSR_MSISRANGE_1 + 5;
        }
        NUTASSERT(range <= NUM_MSI_FREQ);
        msi_clock = MSI_FREQUENCY[range];
    } else {
        msi_clock = 0;
    }
}

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
    uint32_t hpre;

    MsiFrequencyUpdate();
    /* Get SYSCLK source ---------------------------------------------------*/
    cfgr = RCC->CFGR & RCC_CFGR_SWS;
    switch (cfgr) {
    case RCC_CFGR_SWS_MSI:
        tmp = msi_clock;
        break;
    case RCC_CFGR_SWS_HSE:
        tmp = HSE_VALUE;
        break;
    case RCC_CFGR_SWS_PLL: {
        uint32_t pllcfgr = rcc->PLLCFGR;
        uint32_t n = (pllcfgr & RCC_PLLCFGR_PLLN) >> _BI32(RCC_PLLCFGR_PLLN_0);
        uint32_t m = (pllcfgr & RCC_PLLCFGR_PLLM) >> _BI32(RCC_PLLCFGR_PLLM_0);
        uint32_t r = (pllcfgr & RCC_PLLCFGR_PLLR) >> _BI32(RCC_PLLCFGR_PLLR_0);

        n += 1;
        m += 1;
        r = 2 * (r + 1);
        if ((pllcfgr & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSE) {
            tmp = HSE_VALUE;
        } else if ((pllcfgr & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSI) {
            tmp = HSI_VALUE;
        } else {
            tmp = msi_clock;
        }
        tmp = ((tmp * n) / m) / r;
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

/*!
 * \brief Control MSI clock.
 *
 * \param  ena 0 disable clock, any other value enable it.
 * \return 0 on success, -1 on MSI start failed.
 */
static int CtlMsiClock( uint8_t ena)
{
    int rc = 0;

    uint32_t tout = HSE_STARTUP_TIMEOUT;
    volatile uint32_t HSIStatus = 0;

    if (ena) {
        uint32_t cr;
        msi_range_t msi_range;
        /* Enable MSI and MSI range selection in CR */
        RCC->CR |= RCC_CR_MSION ;

        /* Wait till HSI is ready or time out is reached */
        do {
            tout--;
            HSIStatus = RCC->CR & RCC_CR_MSIRDY;
        } while((HSIStatus == 0) && (tout > 0));

        if ((RCC->CR & RCC_CR_MSIRDY) == 0) {
            /* HSI failed to start */
            rc = -1;
        }

        msi_range = MSI_RANGE;

        /* Now switch MSI to selected value.*/
        cr = RCC->CR;
        cr &= ~RCC_CR_MSIRANGE;
        cr |= (msi_range * RCC_CR_MSIRANGE_1) | RCC_CR_MSIRGSEL;
        RCC->CR = cr;
    } else {
        /* Disable HSE clock */
        RCC->CR &= ~RCC_CR_MSION;
    }

    return rc;
}

/*!
 * \brief Control PLL clock.
 *
 * \param  ena 0 disable clock, any other value enable it.
 * \return 0 on success, -1 on PLL start failed.
 */
int CtlPllClock( uint8_t ena)
{
    int rc = 0;
    uint32_t pllcfgr;

    uint32_t tout = HSE_STARTUP_TIMEOUT;
    volatile uint32_t PLLStatus = 0;

    pllcfgr =  RCC->PLLCFGR;
    pllcfgr &= ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLR);
    pllcfgr |=  (PLLM - 1) * RCC_PLLCFGR_PLLM_0;
    pllcfgr |=  (PLLN - 1) * RCC_PLLCFGR_PLLN_0;
    pllcfgr |=  ((PLLR / 2) - 1) * RCC_PLLCFGR_PLLR_0;
    pllcfgr |=  RCC_PLLCFGR_PLLREN;

#if (PLLCLK_SOURCE == PLLCLK_HSE)
    if (CtlHseClock(1) != 0)
        return -1;
    pllcfgr |= RCC_PLLCFGR_PLLSRC_HSE;
#elif (PLLCLK_SOURCE == PLLCLK_HSI)
    if (CtlHsiClock(1) != 0)
        return -1;
    pllcfgr |= RCC_PLLCFGR_PLLSRC_HSI;
#else
    if (CtlMsiClock(1) != 0)
        return -1;
    pllcfgr |= RCC_PLLCFGR_PLLSRC_MSI;
#endif
    RCC->PLLCFGR = pllcfgr;

    if( ena) {
        /* Enable PLL */
        RCC->CR |= RCC_CR_PLLON;

        /* Wait till PLL is ready or time out is reached */
        do {
            tout--;
            PLLStatus = RCC->CR & RCC_CR_PLLRDY;
        } while((PLLStatus == 0) && (tout > 0));

        if ((RCC->CR & RCC_CR_PLLRDY) == 0) {
            /* PLL failed to start */
            rc = -1;
        }
    } else {
        /* Disable HSE clock */
        RCC->CR &= ~RCC_CR_PLLON;
    }

    return rc;
}


/*!
 * \brief  Configures the System clock source: HSE or HSI.
 * \note   This function should be used with PLL disables
 * \param  src is one of PLLCLK_HSE, PLLCLK_HSI.
 * \return 0 if clock is running else -1.
 */
int SetPllClockSource( int src)
{
    int rc = -1;
    uint32_t pllcfgr;

    pllcfgr = RCC->PLLCFGR;
    pllcfgr &= ~RCC_PLLCFGR_PLLSRC;
    if (src == PLLCLK_HSE) {
        rc = CtlHseClock(1);
        if (rc == 0) {
            pllcfgr |= RCC_PLLCFGR_PLLSRC_HSE;
        }
    } else if (src == PLLCLK_HSI) {
        rc = CtlHsiClock(1);
        /* Select HSI as PLL clock source */
        if (rc == 0) {
            pllcfgr |= RCC_PLLCFGR_PLLSRC_HSI;
        }
    } else if (src == PLLCLK_MSI) {
        rc = CtlMsiClock(1);
        /* Select MSI as PLL clock source */
        if (rc == 0) {
            pllcfgr |= RCC_PLLCFGR_PLLSRC_MSI;
        }
    }
    RCC->PLLCFGR = pllcfgr;
    return rc;
}

/*!
 * \brief  Configures the System clock source: MSI, HSI, HS or PLL.
 * \note   This function should be used only after reset.
 * \param  src is one of SYSCLK_HSE, SYSCLK_HSI or SYSCLK_PLL.
 * \return 0 if selected clock is running else -1.
 */
static int SetSysClockSource( int src)
{
    int rc = -1, i;
    uint32_t cr, cfgr;
    uint32_t old_latency, new_latency, new_sysclk;

    /* Set up RTC clock source and eventually LSE and LSI */
    SetRtcClockSource(RTCCLK_SOURCE);

    /* Set voltage scaling*/
    cr = PWR->CR1;
    cr &= ~(PWR_CR1_VOS);
    cr |= STM32_VRANGE * PWR_CR1_VOS_0;
    PWR->CR1 = cr;

    /* Set Latency*/
    old_latency = FLASH->ACR & FLASH_ACR_LATENCY;
    if (src == SYSCLK_MSI) {
        new_sysclk = MSI_FREQUENCY[MSI_RANGE];
    } else if (src == SYSCLK_HSE) {
        new_sysclk = HSE_VALUE;
    } else if (src == SYSCLK_HSI) {
        new_sysclk = HSI_VALUE;
    } else {
        new_sysclk = ((PLLCLK_IN / PLLM) * PLLN) / PLLR;
    }
    new_latency = new_sysclk / FLASH_BASE_FREQ;

    if (new_latency > old_latency) {
        uint32_t flash_acr;
        flash_acr = FLASH->ACR;
        flash_acr &= ~FLASH_ACR_LATENCY;
        flash_acr |= new_latency;
        flash_acr |= FLASH_ACR_PRFTEN;
        FLASH->ACR = flash_acr;
    }
    /* Set AHB/APB1 and APB2 dividers. */
    cfgr = RCC->CFGR;
    cfgr  &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE2| RCC_CFGR_PPRE1);
    i = 0;
    while((1 << AHBPrescTable[i]) < AHB_PRE)
        i++;
    cfgr |= i *RCC_CFGR_HPRE_0;
    i = 0;
    while((1 << APBPrescTable[i]) < APB1_PRE)
        i++;
    cfgr |= i *RCC_CFGR_PPRE1_0;
    i = 0;
    while((1 << APBPrescTable[i]) < APB2_PRE)
        i++;
    cfgr |= i *RCC_CFGR_PPRE2_0;

    RCC->CFGR = cfgr;

    cfgr = RCC->CFGR;
    cfgr &= ~RCC_CFGR_SW;
    if (src == SYSCLK_MSI) {
        rc = CtlMsiClock(1);
        if (rc == 0) {
            /* Select Msi as system clock source */
            cfgr |= RCC_CFGR_SW_MSI;
            RCC->CFGR = cfgr;

            /* Wait till HSE is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_MSI);
        }
    } else if (src == SYSCLK_HSE) {
        rc = CtlHseClock(1);
        if (rc == 0) {
            /* Select HSE as system clock source */
            cfgr |= RCC_CFGR_SW_HSE;
            RCC->CFGR = cfgr;

            /* Wait till HSE is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);
        }
    } else if (src == SYSCLK_HSI) {
        rc = CtlHsiClock(1);
        if (rc == 0) {
            /* Select HSI as system clock source */
            cfgr |= RCC_CFGR_SW_HSI;
            RCC->CFGR = cfgr;

            /* Wait till HSI is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
        }
    } else if (src == SYSCLK_PLL) {
        rc = CtlPllClock(1);
        if (rc == 0) {
            /* Select PLL as system clock source */
            cfgr |= RCC_CFGR_SW_PLL;
            RCC->CFGR = cfgr;

            /* Wait till PLL is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
        } else {
            rc = CtlMsiClock(1);
            cfgr &= ~RCC_CFGR_SW;
            cfgr |= RCC_CFGR_SW_MSI;
            RCC->CFGR = cfgr;
        }
    }

    if (new_latency < old_latency) {
        uint32_t flash_acr;
        flash_acr = FLASH->ACR;
        flash_acr &= ~FLASH_ACR_LATENCY;
        flash_acr |= new_latency;
        flash_acr |= FLASH_ACR_PRFTEN;
        FLASH->ACR = flash_acr;
    }
    /* Update core clock information */
    SystemCoreClockUpdate();

    return rc;
}

int SetSysClock(void)
{
    int rc = 0;
    rc = SetSysClockSource(SYSCLK_SOURCE);

    return rc;
}
