/*
 * Copyright (C) 2013 by Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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

#include <arch/cm3/stm/stm32_clk.h>
#include <cfg/clock.h>

#if defined(MCU_STM32F30X)
#include <arch/cm3/stm/vendor/stm32f30x.h>
#include <arch/cm3/stm/vendor/stm32f30x_rcc.h>
#else
#warning "Unknown STM32 family"
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
 *  (1)4-32MHz HSE-+--/M*N+---+-AHBPRES---+-- APB1PRESC--- APB1
 *                 |          |           +-- ABP2PRESC--- ABP2
 *  8MHz HSI ------+----------'           '-- ADCPRESC---- ADC
 *                 +-- Flash
 *                 '-- Option byte Loader
 *
 * M = 1..16
 * N = 2..16
 * Q = 1/1.5
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

/* Functional same as F1 */
/*!
 * \brief Control HSE clock.
 *
 * \param  ena 0 disable clock, any other value enable it.
 * \return 0 on success, -1 on HSE start failed.
 */
int CtlHseClock( uint8_t ena)
{
    int rc = 0;

    uint32_t tout = HSE_STARTUP_TIMEOUT;
    volatile uint32_t HSEStatus = 0;

    if( ena) {
        /* Enable HSE */
        RCC->CR |= RCC_CR_HSEON;

        /* Wait till HSE is ready or time out is reached */
        do {
            tout--;
            HSEStatus = RCC->CR & RCC_CR_HSERDY;
        } while((HSEStatus == 0) && (tout > 0));

        if ((RCC->CR & RCC_CR_HSERDY) == RESET) {
            /* HSE failed to start */
            rc = -1;
        }
    }
    else {
        /* Disable HSE clock */
        RCC->CR &= ~RCC_CR_HSEON;
    }

    return rc;
}

/* Functional same as F1 */
/*!
 * \brief Control HSI clock.
 *
 * \param  ena 0 disable clock, any other value enable it.
 * \return 0 on success, -1 on HSI start failed.
 */
int CtlHsiClock( uint8_t ena)
{
    int rc = 0;

    uint32_t tout = HSE_STARTUP_TIMEOUT;
    volatile uint32_t HSIStatus = 0;

    if( ena) {
        /* Enable HSI */
        RCC->CR |= RCC_CR_HSION;

        /* Wait till HSI is ready or time out is reached */
        do {
            tout--;
            HSIStatus = RCC->CR & RCC_CR_HSIRDY;
        } while((HSIStatus == 0) && (tout > 0));

        if ((RCC->CR & RCC_CR_HSIRDY) == RESET) {
            /* HSI failed to start */
            rc = -1;
        }
    }
    else {
        /* Disable HSE clock */
        RCC->CR &= ~RCC_CR_HSION;
    }

    return rc;
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

        if ((RCC->CR & RCC_CR_PLLRDY) == RESET) {
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
        rc = CtlHseClock(ENABLE);
        if (rc==0) {
            CM3BBREG(RCC_BASE, RCC_TypeDef, CFGR2, _BI32(RCC_CFGR_PLLSRC_PREDIV1)) = 1;
        }
    }
    else if (src == PLLCLK_HSI) {
        rc = CtlHsiClock(ENABLE);
        /* Select HSI/2 as PLL clock source */
        if (rc==0) {
            CM3BBREG(RCC_BASE, RCC_TypeDef, CFGR2, _BI32(RCC_CFGR_PLLSRC_PREDIV1)) = 0;
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
        rc = CtlHseClock(ENABLE);
        if (rc == 0) {
            /* Select HSE as system clock source */
            RCC->CFGR &= ~RCC_CFGR_SW;
            RCC->CFGR |= RCC_CFGR_SW_HSE;

            /* Wait till HSE is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);
        }
    }
    else if (src == SYSCLK_HSI) {
        rc = CtlHsiClock(ENABLE);
        if (rc == 0) {
            /* Select HSI as system clock source */
            RCC->CFGR &= ~RCC_CFGR_SW;
            RCC->CFGR |= RCC_CFGR_SW_HSI;

            /* Wait till HSI is used as system clock source */
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
        }
    }
    else if (src == SYSCLK_PLL) {
        rc = CtlPllClock(ENABLE);
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
#if (PLLCLK_SOURCE==PLLCLK_HSE)
#define PLLCLK_IN HSE_VALUE
#else
#define PLLCLK_IN HSI_VALUE
#endif


/**
  * @brief  Sets System clock frequency to 72MHz and configure HCLK, PCLK2
  *          and PCLK1 prescalers.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */

/*
   Easy Approach:
   Try to reach fpll = 72 Mhz with M/N.
*/
int SetSysClock(void)
{
    int rc = 0;
    uint32_t rcc_reg;

#if (PLLCLK_IN == 32000000)
  #define  PLLM 4
  #define  PLLN 9
#elif (PLLCLK_IN == 24000000)
  #define  PLLM 1
  #define  PLLN 3
#elif (PLLCLK_IN == 18000000)
  #define  PLLM 1
  #define  PLLN 4
#elif (PLLCLK_IN == 16000000)
  #define  PLLM 2
  #define  PLLN 8
#elif (PLLCLK_IN == 12000000)
  #define  PLLM 1
  #define  PLLN 6
#elif (PLLCLK_IN ==  9000000)
  #define  PLLM 1
  #define  PLLN 8
#elif (PLLCLK_IN == 8000000)
  #define  PLLM 1
  #define  PLLN 9
#elif (PLLCLK_IN ==  6000000)
  #define  PLLM 1
  #define  PLLN 12
/* 72 MHz not reachable, use 48 MHz */
#elif (PLLCLK_IN == 20000000)
  #define  PLLM 5
  #define  PLLN 12
#elif (PLLCLK_IN ==  4000000)
  #define  PLLM 1
  #define  PLLN 12
#else
 #if defined(PLL_CLOCK_DIV)
  #define  PLLM PLL_CLOCK_DIV
 #endif
 #if defined(PLL_CLOCK_MUL)
  #define  PLLN PLL_CLOCK_MUL
 #endif
#endif

#if defined(PLLM) && defined(PLLN)
 #if (PLLM > 16)
  # warning "Illegal PLL_CLOCK_DIV value"
 #endif
 #if (PLLN < 2) || (PLLN > 16)
  # warning "Illegal PLL_CLOCK_MUL value"
 #endif
 #if (PLLCLK_IN /PLLM * PLLN >48000000)
  #define NUT_FLASH_LATENCY  FLASH_ACR_LATENCY_1
 #elif (PLLCLK_IN /PLLM * PLLN >24000000)
  #define NUT_FLASH_LATENCY  FLASH_ACR_LATENCY_0
 #else
  #define NUT_FLASH_LATENCY 0
 #endif
#else
 # warning "Neither 72 nor 48 MHz reachable, define PLL_CLOCK_DIV and PLL_CLOCK_MUL for custom frequency"
#endif


    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    rcc_reg =  RCC->CFGR;
    rcc_reg &= ~(RCC_CFGR_PLLMULL |RCC_CFGR_PLLSRC |RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1 |RCC_CFGR_HPRE);
    rcc_reg |= ((PLLN -2) * RCC_CFGR_PLLMULL_0) | RCC_CFGR_PPRE1_DIV2;
#if (PLLCLK_SOURCE==PLLCLK_HSE)
    if (CtlHseClock(ENABLE) != 0)
        return -1;
    rcc_reg |= RCC_CFGR_PLLSRC;
#else
    if (CtlHsiClock(ENABLE) != 0)
        return -1;
#endif
    RCC->CFGR = rcc_reg;

    rcc_reg = FLASH->ACR;
    rcc_reg &= ~FLASH_ACR_LATENCY;
    rcc_reg |= NUT_FLASH_LATENCY | FLASH_ACR_PRFTBE ;
    FLASH->ACR = rcc_reg;

    rcc_reg = RCC->CFGR2;
    rcc_reg  &= ~(RCC_CFGR2_PREDIV1);
    /* HCLK = SYSCLK, PCLK2 = HCLK , PCLK1 = HCLK/2 */
    rcc_reg |= (PLLM-1);
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
  * @brief  requests System clock frequency
  *
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
uint32_t SysCtlClockGet(void)
{
    SystemCoreClockUpdate();
    return SystemCoreClock;
}
