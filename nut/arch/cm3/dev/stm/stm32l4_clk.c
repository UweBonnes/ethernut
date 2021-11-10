/*
 * Copyright (C) 2015-2019 by Uwe Bonnes
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

#if !defined(RCC_PLLCFGR_PLLP)
# define RCC_PLLCFGR_PLLP 0
#endif

/* Prepare some defaults if configuration is incomplete */

#define MSI_DEFAULT MSI_4M

#define HSI_VALUE (16*1000*1000)

/* Prepare system limits*/
#if defined(MCU_STM32G4)
# define PLLMULT_MAX           127
# define PLLIN_MIN         (2660*1000)
# if   (STM32_POWERSCALE == 1)
/* Give some tolerance to that MSI_32M use 1 WS and MSI_48M 2 WS*/
#  define FLASH_BASE_FREQ (20*1000*1000)
#  if STM32_OVERDRIVE == ENABLE
#   define SYSCLK_MAX    (170*1000*1000)
#  else
#   define SYSCLK_MAX    (150*1000*1000)
#  endif
#  define PLLVCO_MAX     (344*1000*1000)
# elif (STM32_POWERSCALE == 2)
#  define FLASH_BASE_FREQ  (8*1000*1000)
#  define SYSCLK_MAX      (26*1000*1000)
#  define PLLVCO_MAX     (128*1000*1000)
# else
#  warning Invalid STM32_POWERSCALE
# endif
#elif defined(MCU_STM32L4R)
# define PLLMULT_MAX           127
# define PLLIN_MIN         (2660*1000)
# if   (STM32_POWERSCALE == 1)
/* Give some tolerance to that MSI_32M use 1 WS and MSI_48M 2 WS*/
#  define FLASH_BASE_FREQ (20*1000*1000)
#  if STM32_OVERDRIVE == ENABLE
#   define SYSCLK_MAX    (120*1000*1000)
#  else
#   define SYSCLK_MAX     (80*1000*1000)
#  endif
#  define PLLVCO_MAX     (344*1000*1000)
# elif (STM32_POWERSCALE == 2)
#  define FLASH_BASE_FREQ  (8*1000*1000)
#  define SYSCLK_MAX      (26*1000*1000)
#  define PLLVCO_MAX     (128*1000*1000)
# else
#  warning Invalid STM32_POWERSCALE
# endif
#else
# define PLLMULT_MAX            86
# define PLLIN_MIN         (4*1000*1000)
# if   (STM32_POWERSCALE == 1)
/* Give some tolerance to that MSI_32M use 1 WS and MSI_48M 2 WS*/
#  define FLASH_BASE_FREQ (  16007*1000)
#  define SYSCLK_MAX      (80*1000*1000)
#  define PLLVCO_MAX     (344*1000*1000)
# elif (STM32_POWERSCALE == 2)
#  define FLASH_BASE_FREQ  (6*1000*1000)
#  define SYSCLK_MAX      (26*1000*1000)
#  define PLLVCO_MAX     (128*1000*1000)
# else
#   warning Invalid STM32_POWERSCALE
# endif
#endif

#define PCLK1_MAX   SYSCLK_MAX
#define PCLK2_MAX   SYSCLK_MAX

/* Define PLL Input Clock */
#if (PLLCLK_SOURCE == PLLCLK_HSE)
# if (HSE_VALUE < 1000*1000)
#  warning HSE_VALUE not given or too low!
# endif
# define PLLCLK_IN HSE_VALUE
#elif (PLLCLK_SOURCE == PLLCLK_HSI)
# define PLLCLK_IN HSI_VALUE
#elif (PLLCLK_SOURCE == PLLCLK_MSI) && (SYSCLK_SOURCE == SYSCLK_PLL)
/* Use rounded MSI value here to keep calculation below easy */
# define PLLCLK_IN (48*1000*1000)
# undef MSI_RANGE
# define MSI_RANGE MSI_48M
#else
# define PLLCLK_IN 0
#endif

#if (SYSCLK_SOURCE == SYSCLK_PLL)
/* 7 < PLL_MULT < PLLMULT_MAX
 * 0 < PLL_PREDIV < 9
 * 0 < (PLLCLK_DIV / 2) < 5
 */
# if !defined(SYSCLK_FREQ)
#  define SYSCLK_FREQ SYSCLK_MAX
# endif
# if !defined(PLLCLK_PREDIV) && !defined(PLLCLK_MULT) && !defined(PLLCLK_DIV)
#  if (SYSCLK_FREQ == (170*1000*1000))
     /* 340 MHz PLL output. 8 and 6 MHz PLL input would require fractional PLL.*/
#   if ((PLLCLK_IN % (5*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (5*1000*1000))
#    define PLLCLK_MULT   68
#    define PLLCLK_DIV     2
#   elif ((PLLCLK_IN % (4*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (4*1000*1000))
#    define PLLCLK_MULT   85
#    define PLLCLK_DIV     2
#   else
#    warning Provide PLLCLK_PREDIV, PLLCLK_MULT and PLLCLK_DIV to reach 170 MHz
#   endif
#  elif (SYSCLK_FREQ == (150*1000*1000))
     /* 300 MHz PLL output.  8 MHz PLL input would require fractional PLL.*/
#   if   ((PLLCLK_IN % (12*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (12*1000*1000))
#    define PLLCLK_MULT   25
#    define PLLCLK_DIV     2
#   elif ((PLLCLK_IN % (6*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (6*1000*1000))
#    define PLLCLK_MULT   50
#    define PLLCLK_DIV     2
#   elif ((PLLCLK_IN % (5*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (5*1000*1000))
#    define PLLCLK_MULT   60
#    define PLLCLK_DIV     2
#   elif ((PLLCLK_IN % (4*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (4*1000*1000))
#    define PLLCLK_MULT   75
#    define PLLCLK_DIV     2
#   else
#    warning Provide PLLCLK_PREDIV, PLLCLK_MULT and PLLCLK_DIV to reach 80 MHz
#   endif
#  elif (SYSCLK_FREQ == (120*1000*1000))
     /* 240 MHz PLL output */
#   if   ((PLLCLK_IN % (8*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (8*1000*1000))
#    define PLLCLK_MULT   30
#    define PLLCLK_DIV     2
#   elif ((PLLCLK_IN % (6*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (6*1000*1000))
#    define PLLCLK_MULT   40
#    define PLLCLK_DIV     2
#   elif ((PLLCLK_IN % (5*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (5*1000*1000))
#    define PLLCLK_MULT   48
#    define PLLCLK_DIV     2
#   elif ((PLLCLK_IN % (4*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (4*1000*1000))
#    define PLLCLK_MULT   60
#    define PLLCLK_DIV     2
#   else
#    warning Provide PLLCLK_PREDIV, PLLCLK_MULT and PLLCLK_DIV to reach 80 MHz
#   endif
#  elif (SYSCLK_FREQ == (80*1000*1000))
/* 160 MHz PLL output */
#   if   ((PLLCLK_IN % (8*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (8*1000*1000))
#    define PLLCLK_MULT   20
#    define PLLCLK_DIV     2
#   elif ((PLLCLK_IN % (4*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (4*1000*1000))
#    define PLLCLK_MULT   40
#    define PLLCLK_DIV     2
/*   6 MHz PLL Input, 144 MHz PLL VCO, 72 MHz PLL R output */
#   elif ((PLLCLK_IN % (6*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (6*1000*1000))
#    define PLLCLK_MULT   24
#    define PLLCLK_DIV     2
#   elif ((PLLCLK_IN % (5*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (5*1000*1000))
#    define PLLCLK_MULT   32
#    define PLLCLK_DIV     2
#   else
#    warning Provide PLLCLK_PREDIV, PLLCLK_MULT and PLLCLK_DIV to reach 80 MHz
#   endif
#  elif (SYSCLK_FREQ == (26*1000*1000))
/*  8 MHz PLL Input, 104 MHz PLL VCO, 26 MHz PLL R output */
#   if   ((PLLCLK_IN % (8*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (8*1000*1000))
#    define PLLCLK_MULT   13
#    define PLLCLK_DIV     4
/*  7 MHz PLL Input, 98 MHz PLL VCO, 24,5 MHz PLL R output */
#   elif ((PLLCLK_IN % (7*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (7*1000*1000))
#    define PLLCLK_MULT   14
#    define PLLCLK_DIV     4
/*  6 MHz PLL Input, 96 MHz PLL VCO, 24 MHz PLL R output */
#   elif ((PLLCLK_IN % (6*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (6*1000*1000))
#    define PLLCLK_MULT   16
#    define PLLCLK_DIV     4
/*  5 MHz PLL Input, 100 MHz PLL VCO, 25 MHz PLL R output */
#   elif ((PLLCLK_IN % (5*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (5*1000*1000))
#    define PLLCLK_MULT   20
#    define PLLCLK_DIV     4
/*  4 MHz PLL Input, 104 MHz PLL VCO, 26 MHz PLL R output */
#   elif ((PLLCLK_IN % (4*1000*1000)) == 0)
#    define PLLCLK_PREDIV (PLLCLK_IN / (4*1000*1000))
#    define PLLCLK_MULT   26
#    define PLLCLK_DIV     4
#   else
#    warning Please Provide PLLCLK_PREDIV, PLLCLK_MULT and PLLCLK_DIV
#   endif
#  endif
# endif
# if   (PLLCLK_IN / PLLCLK_PREDIV) < PLLIN_MIN
#  warning PLL Input frequency too low
# elif (PLLCLK_IN / PLLCLK_PREDIV) > (16*1000*1000)
#  warning PLL Input frequency too high
# elif ((PLLCLK_IN / PLLCLK_PREDIV) * PLLCLK_MULT) < (64*1000*1000)
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

#if defined(RCC_CR_MSION)
#define NUM_MSI_FREQ 12
static const uint32_t MSI_FREQUENCY[NUM_MSI_FREQ] = {
    /* Values from STM32L476 Rev. 2 datasheet, table 47
     * Exact values only for MSI PLL on.
     */
       98304, /* (   3 * (1 << 15))*/
      196608, /* (   6 * (1 << 15))*/
      393216, /* (  12 * (1 << 15))*/
      786432, /* (  24 * (1 << 15))*/
     1016808, /* (  31 * (1 << 15))*/
     1999848, /* (  61 * (1 << 15))*/
     3997696, /* ( 122 * (1 << 15))*/
     7995392, /* ( 244 * (1 << 15))*/
    15990784, /* ( 488 * (1 << 15))*/
    23986176, /* ( 732 * (1 << 15))*/
    32014336, /* ( 977 * (1 << 15))*/
    48005120, /* (1465 * (1 << 15))*/
};

static uint32_t msi_clock;
/* Forward declarations */

static int CtlMsiClock(msi_range_t range);
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
 * \brief Control MSI clock.
 *
 * \param  Range  MSI_OFF to switch off or MSI_RANGEx
 * \return 0 on success, -1 on MSI start failed.
 */
static int CtlMsiClock(msi_range_t range)
{
    int res = 0;
    uint32_t cr;

    cr = RCC->CR;
    if (range != MSI_OFF) {
        /* MSIRANGE must NOT be modified when MSI is ON and NOT
         *  ready (MSION=1 and MSIRDY=0)
         */
        while ((RCC_CR & RCC_CR_MSION) && !(RCC_CR & RCC_CR_MSIRDY)) {_NOP();}
        cr = RCC->CR;
        cr &= ~RCC_CR_MSIRANGE;
        cr |= range * RCC_CR_MSIRANGE_1;
        RCC->CR = cr;
        cr |= RCC_CR_MSIRGSEL;
        RCC->CR = cr;
        cr |= RCC_CR_MSION;
        RCC->CR = cr;
    } else {
        /* Disable MSI clock */
        RCC->CR &= ~RCC_CR_MSION;
        res = -1;
    }

    return res;
}
#else
# define MsiFrequencyUpdate()
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

    MsiFrequencyUpdate();
    /* Get SYSCLK source ---------------------------------------------------*/
    cfgr = RCC->CFGR & RCC_CFGR_SWS;
    switch (cfgr) {
#if defined(RCC_CR_MSION)
    case RCC_CFGR_SWS_MSI:
        MsiFrequencyUpdate();
        tmp = msi_clock;
        break;
#endif
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
#if defined(RCC_CR_MSION)
        } else if ((pllcfgr & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_MSI) {
            tmp = msi_clock;
#endif
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
#if defined(RCC_CR_MSION)
    case (PLLCLK_MSI):
        rc = CtlMsiClock(MSI_RANGE);
        if (rc) {
            return rc;
        }
        rc = rcc_set_and_wait_rdy_value(
            &RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_MSI,
            RCC_CFGR_SWS, RCC_CFGR_SWS_MSI, HSE_STARTUP_TIMEOUT);
        if (rc) {
            return rc;
        }
        break;
#endif
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
#if defined(RCC_CR_MSION)
    case (PLLCLK_MSI):
        pllcfgr |= RCC_PLLCFGR_PLLSRC_MSI;
        break;
#endif
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
    if (frequency < (26*1000*1000)) {
        cr |= 2 * PWR_CR1_VOS_0;
    } else {
        cr |= 1 * PWR_CR1_VOS_0;
    }
    PWR->CR1 = cr;
    /* Wait until VOSF is ready. */
    while (PWR->SR2 & PWR_SR2_VOSF) {_NOP();}
}

/* Set or stop Low power run mode */
static void SetLpMode(uint32_t frequency)
{
    if (frequency > (2*1000*1000)) {
	PWR->CR1 &= ~(PWR_CR1_LPR);
	/* Wait until normal regulator is ready. */
	while (PWR->SR2 & PWR_SR2_REGLPF) {_NOP();}
    } else {
        PWR->CR1 |= PWR_CR1_LPR;
    }
}

static void SetLatency(uint32_t value)
{
    uint32_t flash_acr;
    flash_acr = FLASH->ACR;
    flash_acr &= ~FLASH_ACR_LATENCY;
    flash_acr |= value;
    FLASH->ACR = flash_acr;
}

static void SetR1Mode(uint32_t frequency)
{
#if defined(MCU_STM32L4R)
    if (frequency > (80*1000*1000)) {
        PWR->CR5 &= ~PWR_CR5_R1MODE;
    } else {
        PWR->CR5 |=  PWR_CR5_R1MODE;
    }
#endif
}

/*!
 * \brief  Configures the System clock source: MSI, HSI, HS or PLL.
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
#if defined(RCC_CR_MSION)
    case (SYSCLK_MSI):
        new_sysclk = MSI_FREQUENCY[MSI_RANGE];
        break;
#endif
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

#if defined(MCU_STM32L4R)
    uint32_t cfgr = RCC->CFGR;
    cfgr &= ~RCC_CFGR_HPRE_Msk;
    cfgr |= RCC_CFGR_HPRE_3;
    RCC->CFGR = cfgr;
    int i;
    for (i = 0; i < sys_clock >> 21; i++) {
        /* Wait at least 1 us. NutDelay() is not yet set up!*/
        RCC->CFGR = cfgr;
    }
#endif
    if (sys_clock < new_sysclk) {
        SetR1Mode(new_sysclk);
        SetVos(new_sysclk);
	SetLpMode(new_sysclk);
    }
    if (new_latency > old_latency) {
        SetLatency(new_latency);
    }
    rc = SwitchSystemClock(src);
    if (sys_clock > new_sysclk) {
        SetR1Mode(new_sysclk);
        SetVos(new_sysclk);
	SetLpMode(new_sysclk);
    }
    if (new_latency < old_latency) {
        SetLatency(new_latency);
    }
    CM3BBSETVAL(FLASH_R_BASE, FLASH_TypeDef, ACR, _BI32(FLASH_ACR_PRFTEN), FLASH_PREFETCH);
    CM3BBSETVAL(FLASH_R_BASE, FLASH_TypeDef, ACR, _BI32(FLASH_ACR_ICEN),   FLASH_ICACHE);
    CM3BBSETVAL(FLASH_R_BASE, FLASH_TypeDef, ACR, _BI32(FLASH_ACR_DCEN),   FLASH_DCACHE);
#if defined(MCU_STM32L4R)
    for (i = 0; i < new_sysclk >> 21; i++) {
        /* Wait at least 1 us. NutDelay() is not yet set up!*/
        cfgr = RCC->CFGR;
    }
#endif
    SetBusDividers(AHB_DIV, APB1_DIV, APB2_DIV);
    /* Update core clock information */
    SystemCoreClockUpdate();
#if defined(RCC_CR_MSION)
    if (LSE_VALUE) {
        /* Enable MSI/LSE PLL.
         * This may take up to 2 seconds on a cold start!
         * But neither interrupts nor timers are set up yet,
         * so busy waiting is a resonable choice!*/
        while (!(RCC_BDCR & RCC_BDCR_LSERDY)) {_NOP();}
        RCC->CR |= RCC_CR_MSIPLLEN;
    }
#endif
    return rc;
}
