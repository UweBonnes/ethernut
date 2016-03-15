/*
 * Copyright (C) 2012-2015 by Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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

#include <arch/cm3/stm/stm32xxxx.h>

#if !defined(HSI_VALUE)
#define HSI_VALUE 16000000
#endif /* HSI_VALUE */

#if !defined(HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT ((uint32_t)5000) /*!< Time out for HSE start up, in ms */
#endif

/* Prepare some defaults if configuration is incomplete */
#if !defined(SYSCLK_SOURCE)
#define SYSCLK_SOURCE SYSCLK_HSI
#endif

/*----------------  Clock Setup Procedure ------------------------------
 *
 * Clock system ist arranged like this:
 *
 *                        /Q------------------------------ USB
 *                        |               ,--------------- CPU
 *                        |               +--------------- SDIO
 * 4-26   MHz HSE -+--/M*N/P--+-AHBPRES---+-- APB1PRESC--- APB1
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
        uint32_t p = (pllcfgr & RCC_PLLCFGR_PLLP) >> _BI32(RCC_PLLCFGR_PLLP_0);

        /* Pll Divisor is (p + 1) * 2. Move * 2  into the base clock sonstant*/
        if ((pllcfgr & RCC_PLLCFGR_PLLSRC_HSE) == RCC_PLLCFGR_PLLSRC_HSE)
            tmp = HSE_VALUE / 2;
        else
            tmp = HSI_VALUE / 2;
        tmp = ((tmp / m) * n)/(p + 1);
        break;
    }
    default:
        tmp = HSI_VALUE;
    }
    sys_clock = tmp;
    SetClockShift();
}

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

        if ((RCC->CR & RCC_CR_PLLRDY) == 0) {
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
    if (src == PLLCLK_HSE) {
        rc = CtlHseClock(1);
        if (rc==0) {
            CM3BBSET(RCC_BASE, RCC_TypeDef, PLLCFGR, _BI32(RCC_PLLCFGR_PLLSRC));
        }
    }
    else if (src == PLLCLK_HSI) {
        rc = CtlHsiClock(1);
        /* Select HSI/2 as PLL clock source */
        if (rc==0) {
            CM3BBCLR(RCC_BASE, RCC_TypeDef, PLLCFGR, _BI32(RCC_PLLCFGR_PLLSRC));
        }
    }

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

    if (src == SYSCLK_HSE) {
        rc = CtlHseClock(1);
        if (rc == 0) {
            /* Select HSE as system clock source */
            RCC->CFGR &= ~RCC_CFGR_SW;
            RCC->CFGR |= RCC_CFGR_SW_HSE;

            /* Wait till HSE is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);
        }
    }
    else if (src == SYSCLK_HSI) {
        rc = CtlHsiClock(1);
        if (rc == 0) {
            /* Select HSI as system clock source */
            RCC->CFGR &= ~RCC_CFGR_SW;
            RCC->CFGR |= RCC_CFGR_SW_HSI;

            /* Wait till HSI is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
        }
    }
    else if (src == SYSCLK_PLL) {
        rc = CtlPllClock(1);
        if (rc == 0) {
            /* Select HSI as system clock source */
            RCC->CFGR &= ~RCC_CFGR_SW;
            RCC->CFGR |= RCC_CFGR_SW_PLL;

            /* Wait till PLL is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
        }
    }

    /* Update core clock information */
    SystemCoreClockUpdate();

    return rc;
}

#if (SYSCLK_SOURCE == SYSCLK_HSI) || (SYSCLK_SOURCE == SYSCLK_HSE)
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
#else /* (SYSCLK_SOURCE == SYSCLK_HSI) || (SYSCLK_SOURCE == SYSCLK_HSE) */
#if (PLLCLK_SOURCE==PLLCLK_HSE)
#define PLLCLK_IN HSE_VALUE
#else
#define PLLCLK_IN HSI_VALUE
#endif

#if !defined(STM32_VRANGE) || STM32_VRANGE == 0
#define FLASH_BASE_FREQ 30000000
#elif STM32_VRANGE == 1
#define FLASH_BASE_FREQ 24000000
#elif STM32_VRANGE == 2
#define FLASH_BASE_FREQ 22000000

#elif STM32_VRANGE == 3
#define FLASH_BASE_FREQ 20000000
#endif

#if PLLCLK_IN > 26000000
#warning PLL Input frequency too high
#endif

#if defined(STM32F411xE)
#define SYSCLK_MAX 100000000
#define PCLK1_MAX   50000000
#define PCLK2_MAX  100000000
#elif defined(STM32F401)
#define SYSCLK_MAX  84000000
#define PCLK1_MAX   42000000
#define PCLK2_MAX   84000000
#else
/* FIXME: Differentiate F40x/F41x and F42x/F43x */
#define SYSCLK_MAX  168000000
#define PCLK1_MAX   42000000
#define PCLK2_MAX   84000000
#endif

#if SYSCLK_FREQ > SYSCLK_MAX
#warning "SYSCLK_FREQ overclocked"
#endif

/* Multiple of 2 MHZ*/
#if (PLLCLK_IN > 3999999) && ((PLLCLK_IN % 2000000L) == 0)
 #define  PLLM (PLLCLK_IN/2000000)
 #define  PLLN ((SYSCLK_FREQ/1000000) << _BI32(RCC_PLLCFGR_PLLN_0))
/*Multiple of 1 MHZ */
#elif (PLLCLK_IN > 1999999) && ((PLLCLK_IN % 1000000L) == 0)
 #define  PLLM (PLLCLK_IN/1000000)
 #define  PLLN ((SYSCLK_FREQ/500000 ) << _BI32(RCC_PLLCFGR_PLLN_0))
#else
 #warning "PLL Source frequency isn't a multiple of 1 or 2 MHz"
#endif
#define  PLLP ((2/2-1) << _BI32(RCC_PLLCFGR_PLLP_0))
#define  PLLQ (5 << _BI32(RCC_PLLCFGR_PLLQ_0))
#define  NUT_FLASH_LATENCY  (SYSCLK_FREQ/FLASH_BASE_FREQ)

/* Voltage Scaling in PWR->CR register */
#if defined(STM32F401)
#if SYSCLK_FREQ > 60000000
#define VOS 2
#else
#define VOS 1
#endif
#elif defined(STM32F411xE)
#if SYSCLK_FREQ > 84000000
#define VOS 3
#elif SYSCLK_FREQ > 64000000
#define VOS 2
#else
#define VOS 1
#endif
#elif defined(MCU_STM32F40)
#if SYSCLK_FREQ > 144000000
#define VOS 1
#else
#define VOS 0
#endif
#else
#if SYSCLK_FREQ > 168000000
#define VOS 3
#elif SYSCLK_FREQ > 144000000
#define VOS 2
#else
#define VOS 1
#endif
#endif
#if !defined(PWR_CR_VOS_0) && defined(PWR_CR_VOS)
#define PWR_CR_VOS_0 PWR_CR_VOS
#endif

#if !defined(PCLK1_TARGET) || (PCLK1_TARGET > PCLK1_MAX)
#define PCLK1_TARGET PCLK1_MAX
#endif

#if !defined(PCLK2_TARGET) || (PCLK2_TARGET > PCLK2_MAX)
#define PCLK2_TARGET PCLK2_MAX
#endif

/**
  * @brief  Sets System clock frequency to 120/168MHz and configure HCLK, PCLK2
  *          and PCLK1 prescalers.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */

/*
   Ranges :
   M: 2..63
   N: 64.. 432
   P: 2, 4, 6, 8
   Q: 2..15

   0.95 MHz < PLLCLK_IN/M < 2 MHz, Prefer 2 MHz for low jitter
   192 MHz < PLLCLK_IN/M*N < 432 MHz
   PLLCLK_IN/M*N/P < 168 MHz
   PLLCLK_IN/M*N/Q < 48 MHz, Use 48 MHz for USB

   Easy Approach:
   Try to reach fvco = 336 Mhz with M/N.
   Require a clock >= 4 MHz in 2 MHz Steps
*/
int SetSysClock(void)
{
    int rc = 0;
    uint32_t rcc_reg;
#if defined(PWR_CR_VOS_0)
    uint32_t cr;

    /* Select System frequency up to 168 MHz */
    cr = PWR->CR;
    cr &= ~(PWR_CR_VOS);
    cr |= VOS * PWR_CR_VOS_0;
    PWR->CR = cr;
#endif

    /* Set up RTC clock source and eventually LSE and LSI */
    SetRtcClockSource(RTCCLK_SOURCE);

    rcc_reg =  RCC->PLLCFGR;
    rcc_reg &= ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ);
#if (PLLCLK_SOURCE==PLLCLK_HSE)
    if (CtlHseClock(1) != 0)
        return -1;
    rcc_reg = PLLM | PLLN | PLLP | PLLQ | RCC_PLLCFGR_PLLSRC_HSE;
#else
    if (CtlHsiClock(1) != 0)
        return -1;
    rcc_reg = PLLM| PLLN | PLLP | PLLQ | RCC_PLLCFGR_PLLSRC_HSI;
#endif
    RCC->PLLCFGR = rcc_reg;

    rcc_reg = FLASH->ACR;
    rcc_reg &= ~FLASH_ACR_LATENCY;
#if STM32_VRANGE == 3
    /* Prefetch must be off*/
    rcc_reg |= NUT_FLASH_LATENCY;
#else
    rcc_reg |= NUT_FLASH_LATENCY | FLASH_ACR_PRFTEN ;
#endif
    /* Enable Instruction and Data cache */
    rcc_reg |= FLASH_ACR_ICEN | FLASH_ACR_DCEN;
    FLASH->ACR = rcc_reg;

    rcc_reg = RCC->CFGR;
    rcc_reg  &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE2| RCC_CFGR_PPRE1);
    rcc_reg |= RCC_CFGR_HPRE_DIV1;
    rcc_reg |= GetPclkDiv(PCLK1_TARGET) * RCC_CFGR_PPRE1_0;
    rcc_reg |= GetPclkDiv(PCLK2_TARGET) * RCC_CFGR_PPRE2_0;
    RCC->CFGR = rcc_reg;

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
