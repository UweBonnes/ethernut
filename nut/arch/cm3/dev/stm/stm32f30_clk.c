/*
 * Copyright (C) 2013,2014 by Uwe Bonnes
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

#if defined(MCU_STM32F0) || defined(MCU_STM32F3)
#include <arch/cm3/stm/stm32xxxx.h>
#else
#warning "Unknown STM32 family"
#endif

/* Prepare some defaults if configuration is incomplete */
#if !defined(SYSCLK_SOURCE)
#define SYSCLK_SOURCE SYSCLK_HSI
#endif

static uint32_t SystemCoreClock = 0;
static uint8_t clk_div[NUT_HWCLK_MAX] = {1};

static const uint8_t AHBPrescTable[16] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
static const uint8_t APBPrescTable[8]  = {1, 1, 1, 1, 2, 4, 8, 16};

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

/*!
 * \brief  Update SystemCoreClock according to Clock Register Values
 *
 * This function reads out the CPUs clock and PLL registers and assembles
 * the actual clock speed values into the SystemCoreClock local variable.
 */
void SystemCoreClockUpdate(void)
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
        if ((cfgr & RCC_CFGR_PLLSRC ) == RCC_CFGR_PLLSRC )
            tmp = HSE_VALUE;
        else
            tmp = HSI_VALUE / 2;
        tmp = (tmp / prediv) * pllmull;
        break;
    }
    default:
        tmp = HSI_VALUE;
    }
    hpre = (cfgr & RCC_CFGR_HPRE) >> _BI32(RCC_CFGR_HPRE_0);
    SystemCoreClock = tmp >> AHBPrescTable[hpre];
    tmp = (RCC->CFGR & RCC_CFGR_PPRE1) >> _BI32( RCC_CFGR_PPRE1_0);
    clk_div[NUT_HWCLK_PCLK1] = APBPrescTable[tmp];
    if (clk_div[NUT_HWCLK_PCLK1] < 2)
        clk_div[NUT_HWCLK_TCLK1] = 1;
    else
        clk_div[NUT_HWCLK_TCLK1] = clk_div[NUT_HWCLK_PCLK1] / 2;
    tmp = (RCC->CFGR & RCC_CFGR_PPRE2) >> _BI32( RCC_CFGR_PPRE2_0);
    clk_div[NUT_HWCLK_PCLK2] = APBPrescTable[tmp];
    if (clk_div[NUT_HWCLK_PCLK2] < 2)
        clk_div[NUT_HWCLK_TCLK2] = 1;
    else
        clk_div[NUT_HWCLK_TCLK2] = clk_div[NUT_HWCLK_PCLK2] / 2;
}

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
#if defined(HSE_BYPASS)
        /* Assume HSE if off */
        RCC->CR |= RCC_CR_HSEBYP;
#else
        RCC->CR &= ~RCC_CR_HSEBYP;
#endif
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
            CM3BBSET(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PLLSRC_PREDIV1));
        }
    }
    else if (src == PLLCLK_HSI) {
        rc = CtlHsiClock(ENABLE);
        /* Select HSI/2 as PLL clock source */
        if (rc==0) {
            CM3BBCLR(RCC_BASE, RCC_TypeDef, CFGR, _BI32(RCC_CFGR_PLLSRC_PREDIV1));
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

/**
  * @brief  Sets System clock frequency to 72MHz and configure HCLK, PCLK2
  *          and PCLK1 prescalers.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
#if (PLLCLK_SOURCE==PLLCLK_HSE)
 #define PLLCLK_IN HSE_VALUE
 #if (PLLCLK_MULT == 0 ) && (PLLCLK_DIV == 0)
  #undef PLLCLK_DIV
  #if   (((PLLCLK_IN % 24000000) == 0) && ((SYSCLK_FREQ  % 24000000) == 0))
   #define PLLCLK_DIV  (PLLCLK_IN / 24000000)
  #elif (((PLLCLK_IN % 16000000) == 0) && ((SYSCLK_FREQ % 16000000) == 0))
   #define PLLCLK_DIV  (PLLCLK_IN / 16000000)
  #elif (((PLLCLK_IN % 14400000) == 0) && ((SYSCLK_FREQ % 14400000) == 0))
   #define PLLCLK_DIV  (PLLCLK_IN / 14400000)
  #elif (((PLLCLK_IN % 12000000) == 0) && ((SYSCLK_FREQ % 12000000) == 0))
   #define PLLCLK_DIV  (PLLCLK_IN / 12000000)
  #elif (((PLLCLK_IN %  9600000) == 0) && ((SYSCLK_FREQ %  9600000) == 0))
   #define PLLCLK_DIV  (PLLCLK_IN / 9600000)
  #elif (((PLLCLK_IN %  8000000) == 0) && ((SYSCLK_FREQ %  8000000) == 0))
   #define PLLCLK_DIV  (PLLCLK_IN / 8000000)
  #elif (((PLLCLK_IN %  6000000) == 0) && ((SYSCLK_FREQ %  6000000) == 0))
   #define PLLCLK_DIV  (PLLCLK_IN / 6000000)
  #elif (((PLLCLK_IN %  4800000) == 0) && ((SYSCLK_FREQ %  4800000) == 0))
   #define PLLCLK_DIV  (PLLCLK_IN / 4800000)
  #elif (((PLLCLK_IN %  4000000) == 0) && ((SYSCLK_FREQ %  4000000) == 0))
   #define PLLCLK_DIV  (PLLCLK_IN / 4000000)
  #elif (((PLLCLK_IN %  3000000) == 0) && ((SYSCLK_FREQ %  3000000) == 0))
   #define PLLCLK_DIV  (PLLCLK_IN / 3000000)
  #endif
 #endif
#else /* HSI Used*/
/* HSI_VALUE value from vendor provided file as illegal type */
 #define PLLCLK_IN 4000000
 #if (PLLCLK_MULT == 0 ) && (PLLCLK_DIV == 0)
  #undef PLLCLK_DIV
  #define PLLCLK_DIV 1
 #elif (PLLCLK_DIV != 0)
  #warning "HSI/2 Input has no clock divider"
 #endif
#endif

#if (PLLCLK_MULT == 0 ) && (PLLCLK_DIV > 0)
 #undef PLLCLK_MULT
 #define PLLCLK_MULT (SYSCLK_FREQ/(PLLCLK_IN / PLLCLK_DIV))
#endif

#if (PLLCLK_DIV < 1)  || (PLLCLK_DIV > 16)
 # warning "Illegal PLLCLK_DIV value"
#endif

#if (PLLCLK_MULT < 2)  || (PLLCLK_MULT > 16)
 # warning "Illegal PLLCLK_MULT value"
#endif

#if (PLLCLK_IN /PLLCLK_DIV * PLLCLK_MULT > 72000000)
  #warning "Clock frequency too high"
#elif (PLLCLK_IN /PLLCLK_DIV * PLLCLK_MULT >48000000)
 #define NUT_FLASH_LATENCY  FLASH_ACR_LATENCY_1
 #if  defined(MCU_STM32F0)
  #warning "Clock frequency too high"
 #endif
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

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    rcc_reg =  RCC->CFGR;
    rcc_reg &= ~(RCC_CFGR_PLLMUL |RCC_CFGR_PLLSRC |RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1 |RCC_CFGR_HPRE);
#if defined(MCU_STM32F0)
/* APB Bus can run with full SYSCLK speed (48 MHz) */
    rcc_reg |= ((PLLCLK_MULT -2) * RCC_CFGR_PLLMUL_0);
#else
/* APB1 Bus (Slow APB) bus can only run with half SYSCLK speed 36 MHz) */
    rcc_reg |= ((PLLCLK_MULT -2) * RCC_CFGR_PLLMUL_0) | RCC_CFGR_PPRE1_DIV2;
#endif
#if (PLLCLK_SOURCE == PLLCLK_HSE)
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

/**
  * @brief  requests frequency of the given clock
  *
  * @param  idx NUT_HWCLK Index
  * @retval clock or 0 if idx points to an invalid clock
  */
uint32_t STM_ClockGet(int idx)
{
    SystemCoreClockUpdate();
    if (idx < NUT_HWCLK_MAX)
        return SystemCoreClock/clk_div[idx];
    return 0;
}
