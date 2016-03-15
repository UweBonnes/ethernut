
/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
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

/*
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <cfg/arch.h>
#include <arch/cm3.h>

#include <arch/cm3/timer.h>
#include <arch/cm3/stm/stm32_clk.h>
#include <cfg/clock.h>

#if !defined(MCU_STM32F1)
#warning "Unknown STM32 family"
#endif

#if !defined(HSI_VALUE)
#define HSI_VALUE 8000000
#endif

#include <arch/cm3/stm/stm32xxxx.h>

/* Prepare some defaults if configuration is incomplete */
#if !defined(SYSCLK_SOURCE)
#define SYSCLK_SOURCE SYSCLK_HSI
#endif

#if defined(MCU_STM32F1_CL)
# define HSE_MAX  25000000
#elif defined(MCU_STM32F100)
# define HSE_MAX  24000000
#else
# define HSE_MAX  16000000
#endif

#if   defined(MCU_STM32F100)
# define SYSCLK_MAX 24000000
#elif defined(MCU_STM32F101)
# define SYSCLK_MAX 36000000
#elif defined(MCU_STM32F102)
# define SYSCLK_MAX 48000000
#else
# define SYSCLK_MAX 72000000
#endif

#if defined(HSE_VALUE)
# if HSE_VALUE < 4000000
#  warning HSE_VALUE too low
# elif HSE_VALUE > HSE_MAX
#  warning HSE_VALUE too high
# endif
#endif

/* TODO: Provide more heuristics*/
#if (SYSCLK_SOURCE == SYSCLK_PLL)
# undef PLLCLK_SOURCE
# undef SYSCLK_SOURCE
# undef PLLCLK_DIV
# undef PLLCLK_MULT
# undef PLL2CLK_DIV
# undef PLL2CLK_MULT
# if   defined(MCU_STM32F100)
#  if defined(HSE_VALUE) && HSE_VALUE == 24000000
#   define SYSCLK_SOURCE     SYSCLK_HSE
#  endif
# elif defined(MCU_STM32F101)
#  define SYSCLK_FREQ         36000000
# elif defined(MCU_STM32F102)
#   define SYSCLK_FREQ        48000000
# elif defined(MCU_STM32F103)
#  if defined(HSE_VALUE)
#   define SYSCLK_FREQ        72000000
#  else
#   define SYSCLK_FREQ        64000000
#  endif
# elif defined(MCU_STM32F1_CL)
#  define SYSCLK_SOURCE     SYSCLK_PLL
#  if defined(HSE_VALUE)
#   if (HSE_VALUE == 25000000)
/* Values From STM32F105xx datasheet*/
#    define PLLCLK_SOURCE     PLLCLK_PLL2
#    define PLL2CLK_PREDIV    5
#    define PLL2CLK_MULT      8
#    define PLLCLK_DIV        5
#    define PLLCLK_MULT       9
#   elif (HSE_VALUE == 14745600)
#    define PLLCLK_SOURCE     PLLCLK_PLL2
#    define PLL2CLK_PREDIV    4
#    define PLL2CLK_MULT      12
#    define PLLCLK_DIV        4
#    define PLLCLK_MULT       (RCC_CFGR_PLLMULL6_5 / RCC_CFGR_PLLMULL_0)
#   elif (HSE_VALUE == 8000000)
#    define PLLCLK_SOURCE     PLLCLK_HSE
#    define PLLCLK_PREDIV     1
#    define PLLCLK_MULT       9
#   else
#    warning Please supply fitting values
#   endif
#  else
#   define PLLCLK_SOURCE      PLLCLK_HSI
#   define PLLCLK_MULT        9
#  endif
# else
#  warning Unhandled device
# endif
/* Now handle Values for all devices but STM32F1_CL */
# if !defined(SYSCLK_SOURCE)
#  if defined(HSE_VALUE)
#   define SYSCLK_SOURCE    SYSCLK_PLL
#   if (HSE_VALUE ==  8000000)
#    define PLLCLK_SOURCE  PLLCLK_HSE
#    define PLLCLK_DIV     1
#    define PLLCLK_MULT    (SYSCLK_FREQ / HSE_VALUE)
#   else
#    warning Please supply fitting values
#   endif
#  else
#   define PLLCLK_SOURCE  PLLCLK_HSI
#   define PLL_MULT       (SYSCLK_FREQ / HSI_VALUE)
#  endif
# endif
#endif

# if (PLLCLK_SOURCE == PLLCLK_HSI)
#  define PLLCLK_IN (HSI_VALUE / 2)
# else
# define PLLCLK_IN HSE_VALUE
# endif

/* Provide Fallback values*/
#if !defined(SYSCLK_FREQ)
# define SYSCLK_FREQ SYSCLK_MAX
#endif
#if !defined(PLL2CLK_PREDIV)
#define PLL2CLK_PREDIV 1
#endif
#if !defined(PLLCLK_PREDIV)
#define PLLCLK_PREDIV 1
#endif

/* Some checks*/
#if SYSCLK_FREQ > SYSCLK_MAX
# warning Requested SYSCLK_FREQ too high
#endif

/*----------------  Clock Setup Procedure ------------------------------
 *
 * Clock system ist arranged like this:
 *
 * PLLCLK2--/1..16- (CL)
 *                 |
 * HSE -- /1..16 (CL  VL)     ,--------------------------- USB
 *                 |          |           ,--------------- CPU
 * HSE -- /1..2 ---+          |           +--------------- SDIO
 *                 +---PLLMUL-+-AHBPRES---+-- APB1PRESC--- APB1
 *                /2          |           +-- ABP2PRESC--- ABP2
 * 8MHz HSI -------+----------'           '-- ADCPRESC---- ADC
 *
 *
 *        ***** Setup of system clock configuration *****
 *
 * HSE 4 .. 25 MHz (CL)
 * HSE 4 .. 24 MHz (VL)
 * HSE 4 .. 16 MHz (all other)
 *
 * Maximum Clock
 * STM32F100     : 24 MHz
 * STM32F101     : 36 MHz
 * STM32F102     : 48 MHz
 * STM32F103/5/7 : 72 MHz

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
    uint32_t tmp = 0, pllmult = 0, pllsource = 0, pllinput;

    /* Get SYSCLK source -------------------------------------------------------*/
    tmp = RCC->CFGR & RCC_CFGR_SWS;

    switch (tmp)
    {
        case 0x00:  /* HSI used as system clock */
            sys_clock = HSI_VALUE;
            break;
        case 0x04:  /* HSE used as system clock */
            sys_clock = HSE_VALUE;
            break;
        case 0x08:  /* PLL used as system clock */
            /* Get PLL clock source and multiplication factor ----------------------*/
            pllmult = RCC->CFGR & RCC_CFGR_PLLMULL;
            pllmult = pllmult / RCC_CFGR_PLLMULL_0;
            if (pllmult > 14)
                pllmult = 14;
            pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;
            if ( RCC_CFGR_PLLSRC != pllsource) {
                pllinput = HSI_VALUE / 2;
#if defined(RCC_CFGR2_PREDIV1)
            } else {
                uint32_t pllmuxinput;
                uint32_t prediv1;
                pllmuxinput = HSE_VALUE;
# if defined(RCC_CFGR2_PREDIV1SRC)
                if (RCC_CFGR2_PREDIV1SRC_PLL2 == (RCC->CFGR2 & RCC_CFGR2_PREDIV1SRC)) {
                    uint32_t prediv2, pll2fac;
                    const uint8_t pll2mult[16] ={0,0,0,0,0,0,8,9,10,11,12,13,14,0,16,20};
                    prediv2 = RCC->CFGR2 & RCC_CFGR2_PREDIV2;
                    prediv2 = prediv2 / RCC_CFGR2_PREDIV2_0;
                    prediv2 = prediv2 + 1;
                    pll2fac = RCC->CFGR2 & RCC_CFGR2_PLL2MUL;
                    pll2fac = pll2fac / RCC_CFGR2_PLL2MUL_0;
                    pllmuxinput = (HSE_VALUE * pll2mult[pll2fac]) / prediv2;
                }
                if ((RCC_CFGR_PLLMULL6_5 / RCC_CFGR_PLLMULL_0) == pllmult) {
                    pllinput = pllinput / 2;
                    pllmult = 11; /* FIX offset added later*/
                }
# endif
                prediv1 =  RCC->CFGR2 &  RCC_CFGR2_PREDIV1;
                prediv1 =  prediv1 / RCC_CFGR2_PREDIV1_0;
                pllinput = pllmuxinput / (prediv1 + 1);
#else
            } else if (RCC_CFGR_PLLXTPRE_HSE_DIV2 == (RCC->CFGR & RCC_CFGR_PLLXTPRE)) {
                pllinput = HSE_VALUE / 2;
            } else {
                pllinput = HSE_VALUE;
#endif
            }

# if defined (RCC_CFGR2_PREDIV1)
            if (RCC_CFGR_PLLSRC != pllsource) {
                uint32_t prediv1factor;
                prediv1factor = RCC->CFGR2 & RCC_CFGR2_PREDIV1;
                /* HSE oscillator clock selected as PREDIV1 clock entry */
                pllinput = HSE_VALUE / (prediv1factor + 1);
            }
#endif
            sys_clock = pllinput * (pllmult + 2);
            break;

        default:
            sys_clock = HSI_VALUE;
            break;
    }
    SetClockShift();
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

    uint32_t tout = HSE_STARTUP_TIMEOUT;
    volatile uint32_t PLLStatus = 0;

    if( ena) {
        /* Enable PLL */
        RCC->CR |= RCC_CR_PLLON;

        /* Wait till PLL is ready or time out is reached */
        do {
            PLLStatus = RCC->CR & RCC_CR_PLLRDY;
            tout--;
        } while((PLLStatus == 0) && (tout > 0));

        if ((RCC->CR & RCC_CR_PLLRDY) == 0) {
            /* HSE failed to start */
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
 * \note   This function should be used only after reset.
 * \param  src is one of PLLCLK_HSE, PLLCLK_HSI.
 * \return 0 if clock is running ale -1.
 */
int SetPllClockSource( int src)
{
    int rc = -1;

    if (src == PLLCLK_HSE) {
        rc = CtlHseClock(1);
        if (rc==0) {
            /* Select HSE/2 as PLL clock source */
            RCC->CFGR |= RCC_CFGR_PLLSRC|RCC_CFGR_PLLXTPRE;
        }
    }
    else if (src == PLLCLK_HSI) {
        rc = CtlHsiClock(1);
        /* Select HSI/2 as PLL clock source */
            RCC->CFGR &= ~(RCC_CFGR_PLLSRC|RCC_CFGR_PLLXTPRE);
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

#if defined(FLASH_ACR_PRFTBE)
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 0 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);

#if (SYSCLK_SOURCE == SYSCLK_HSE)
    if (HSE_VALUE > 24000000) {
        FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;
    }
#endif
#endif

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
/**
  * @brief  Sets System clock frequency to 72MHz and configure HCLK, PCLK2
  *          and PCLK1 prescalers.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
int SetSysClock(void)
{
    uint32_t cfgr;
    int rc;

    /* Set up RTC clock source and eventually LSE and LSI */
    SetRtcClockSource(RTCCLK_SOURCE);

    cfgr = RCC->CFGR;
    cfgr &= ~RCC_CFGR_PLLMULL;
#if defined(RCC_CFGR2_PREDIV1)
    uint32_t cfgr2;
    cfgr2 = RCC->CFGR2;
    cfgr2 &= ~RCC_CFGR2_PREDIV1;
    cfgr2 |= (PLL2CLK_PREDIV - 1) * RCC_CFGR2_PREDIV1_0;
# if defined(RCC_CFGR2_PREDIV2)
    cfgr2 &= ~(RCC_CFGR2_PREDIV2 | RCC_CFGR2_PLL2MUL |
                 RCC_CFGR2_PREDIV1SRC);
#  if PLLCLK_SOURCE == PLLCLK_PLL2
    cfgr2 |= RCC_CFGR2_PREDIV1SRC_PLL2;
#   if   PLL2CLK_MULT == 20
    cfgr2 |= RCC_CFGR2_PLL2MUL20;
#   elif PLL2CLK_MULT == 16
    cfgr2 |= RCC_CFGR2_PLL2MUL16;
#   elif PLL2CLK_MULT == 14
    cfgr2 |= RCC_CFGR2_PLL2MUL14;
#   elif PLL2CLK_MULT == 13
    cfgr2 |= RCC_CFGR2_PLL2MUL13;
#   elif PLL2CLK_MULT == 12
    cfgr2 |= RCC_CFGR2_PLL2MUL12;
#   elif PLL2CLK_MULT == 12
    cfgr2 |= RCC_CFGR2_PLL2MUL11;
#   elif PLL2CLK_MULT == 10
    cfgr2 |= RCC_CFGR2_PLL2MUL10;
#   elif PLL2CLK_MULT == 9
    cfgr2 |= RCC_CFGR2_PLL2MUL9;
#   elif PLL2CLK_MULT == 8
    cfgr2 |= RCC_CFGR2_PLL2MUL8;
#   else
#    warning Wrong PLL2CLK_MULT value:
#   endif
#  endif
# else
# endif
    cfgr2 |= (PLLCLK_PREDIV - 1) * RCC_CFGR2_PREDIV1_0;
    RCC->CFGR2 = cfgr2;

# if PLLCLK_SOURCE == PLLCLK_PLL2
    /* Only enable PLL2 if needed. FIXME: Handle PLL2 for MCO */
    RCC->CR |= RCC_CR_PLL2ON;
    /* Wait till PLL2 is ready */
    while ((RCC->CR & RCC_CR_PLL2RDY) == 0);
# endif
#else
    if (PLLCLK_PREDIV > 1) {
        cfgr |= RCC_CFGR_PLLXTPRE_HSE_DIV2;
    } else {
        cfgr &= ~RCC_CFGR_PLLXTPRE_HSE_DIV2;
    }
#endif
    if (PLLCLK_SOURCE == PLLCLK_HSI) {
        cfgr &= ~RCC_CFGR_PLLSRC;
    } else {
        cfgr |=  RCC_CFGR_PLLSRC;
    }

    /* HCLK = SYSCLK */
    cfgr |= RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK */
    cfgr |= RCC_CFGR_PPRE2_DIV1;

#if  SYSCLK_FREQ > 36000000
        /* PCLK1 = HCLK/2 */
    cfgr|= RCC_CFGR_PPRE1_DIV2;
#endif
    RCC->CFGR = cfgr;
    /* Enable PLL clock source */
    rc = SetPllClockSource(PLLCLK_SOURCE);
    if (rc) {
        /* Something went wrong with the PLL startup! */
        SetSysClockSource(SYSCLK_HSI);
        return rc;
    }

#if defined(FLASH_ACR_PRFTBE)
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;
#endif

#if defined(FLASH_ACR_LATENCY)
    uint32_t tmp;
    tmp = FLASH->ACR;
    tmp &= ~FLASH_ACR_LATENCY;
    /* Flash wait state calculation and setup */
# if    (SYSCLK_FREQ > 48000000)
    tmp |= 2 * FLASH_ACR_LATENCY_0;
# elif ( SYSCLK_FREQ > 24000000)
    tmp |= 1 * FLASH_ACR_LATENCY_0;
# endif
    FLASH->ACR = tmp;
#endif

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
