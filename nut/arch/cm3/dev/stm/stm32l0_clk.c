/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * (C) 2015 Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de
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
 * $Id: stm32_gpio.c 3182 2010-10-17 21:46:04Z Astralix $
 * \endverbatim
 */

#include <cfg/arch.h>
#include <arch/cm3.h>

#include <arch/cm3/timer.h>
#include <arch/cm3/stm/stm32_clk.h>
#include <cfg/clock.h>

#include <arch/cm3/stm/stm32xxxx.h>

#if !defined(HSI_VALUE)
#define HSI_VALUE  16000000
#endif
#if !defined  (HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT   0x5000
#endif

/* Prepare some defaults if configuration is incomplete */
#if !defined(SYSCLK_SOURCE)
#define SYSCLK_SOURCE SYSCLK_HSI
#endif

const uint32_t MSIFreqTable[8] = {
    65536,
    131072,
    262144,
    524288,
    1048000,
    2097000,
    4194000,
    0
};

/*----------------  Clock Setup Procedure ------------------------------
 *
 * Clock system ist arranged like this:
 *
 *                            ,--------------------------- USB
 *                            |           ,--------------- CPU
 *                            |           +--------------- SDIO
 * 1-32MHz HSE ----+---PLLMUL-+-AHBPRES---+-- APB1PRESC--- APB1
 *                 |          |           +-- ABP2PRESC--- ABP2
 * 16MHz HSI ------+----------'-------------- ADCPRESC---- ADC
 *                            |
 *       MSI -----------------'
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

/*Provide defaults */
#if !defined(PLLCLK_SOURCE)
# if (SYSCLK_SOURCE == SYSCLK_HSE)
#  define PLLCLK_SOURCE PLLCLK_HSE
# else
#  define PLLCLK_SOURCE PLLCLK_HSI
# endif
#endif

/* Set fVCO to 96 MHz to allow USB operatiosn */
#if (PLLCLK_SOURCE==PLLCLK_HSE)
# define PLLCLK_IN HSE_VALUE
#else
# define PLLCLK_IN (HSI_VALUE)
#endif

#if (defined(PLLCLK_MULT) && defined(PLLCLK_DIV))
#elif (defined(PLLCLK_MULT) && !defined(PLLCLK_DIV)) || (!defined(PLLCLK_MULT) && defined(PLLCLK_DIV))
# warning Please either provide both PLLCLK_MULT and PLLCLK_DIV or none
#else
# if  (PLLCLK_SOURCE == PLLCLK_HSI)
#  define PLLCLK_MULT 6
#  define PLLCLK_DIV 3
# else
#  define PLLCLK_DIV 3
#  if   (HSE_VALUE ==  2000000)
#   define PLLCLK_MULT 48
#  elif (HSE_VALUE ==  3000000)
#   define PLLCLK_MULT 32
#  elif (HSE_VALUE ==  4000000)
#   define PLLCLK_MULT 24
#  elif (HSE_VALUE ==  6000000)
#   define PLLCLK_MULT 16
#  elif (HSE_VALUE ==  8000000)
#   define PLLCLK_MULT 12
#  elif (HSE_VALUE == 12000000)
#   define PLLCLK_MULT  8
#  elif (HSE_VALUE == 16000000)
#   define PLLCLK_MULT  6
#  elif (HSE_VALUE == 24000000)
#   define PLLCLK_MULT  4
#  else
#   warning Please provide PLLCLK_MUL and PLLCLK_DIV
#  endif
# endif
#endif

#if   (PLLCLK_MULT ==  3)
# define PLLM RCC_CFGR_PLLMUL3
#elif (PLLCLK_MULT ==  4)
# define PLLM RCC_CFGR_PLLMUL4
#elif (PLLCLK_MULT ==  6)
# define PLLM RCC_CFGR_PLLMUL6
#elif (PLLCLK_MULT ==  8)
# define PLLM RCC_CFGR_PLLMUL8
#elif (PLLCLK_MULT == 12)
# define PLLM RCC_CFGR_PLLMUL12
#elif (PLLCLK_MULT == 16)
# define PLLM RCC_CFGR_PLLMUL16
#elif (PLLCLK_MULT == 24)
# define PLLM RCC_CFGR_PLLMUL24
#elif (PLLCLK_MULT == 32)
# define PLLM RCC_CFGR_PLLMUL32
#elif (PLLCLK_MULT ==  48)
# define PLLM RCC_CFGR_PLLMUL48
#else
# warning Illegal PLLCLK_MULT
#endif

#if   (PLLCLK_DIV == 2)
# define PLLN RCC_CFGR_PLLDIV2
#elif (PLLCLK_DIV == 3)
# define PLLN RCC_CFGR_PLLMUL3
#elif (PLLCLK_DIV == 4)
# define PLLN RCC_CFGR_PLLMUL4
#else
# warning Illegal PLLCLK_DIV
#endif

/* Allow to configure ABP1/2 division */
#if (APB1_DIV == AUTO)
# undef APB1_DIV
# define APB1_DIV 1
#endif
#if (APB2_DIV == AUTO)
# undef APB2_DIV
# define APB2_DIV 1
#endif

#if   (APB1_DIV ==  1)
# define PPRE1 RCC_CFGR_PPRE1_DIV1
#elif (APB1_DIV ==  2)
# define PPRE1 RCC_CFGR_PPRE1_DIV2
#elif (APB1_DIV ==  4)
# define PPRE1 RCC_CFGR_PPRE1_DIV4
#elif (APB1_DIV ==  8)
# define PPRE1 RCC_CFGR_PPRE1_DIV8
#elif (APB1_DIV == 16)
# define PPRE1 RCC_CFGR_PPRE1_DIV16
#else
# warning Illegal APB1_DIV
#endif

#if   (APB2_DIV ==  1)
# define PPRE2 RCC_CFGR_PPRE2_DIV1
#elif (APB2_DIV ==  2)
# define PPRE2 RCC_CFGR_PPRE2_DIV2
#elif (APB2_DIV ==  4)
# define PPRE2 RCC_CFGR_PPRE2_DIV4
#elif (APB2_DIV ==  8)
# define PPRE2 RCC_CFGR_PPRE2_DIV8
#elif (APB2_DIV == 16)
# define PPRE2 RCC_CFGR_PPRE2_DIV16
#else
# warning Illegal APB2_DIV
#endif

/* Include common routines*/
#include "stm32_clk.c"

/*!
 * \brief  Update SystemCoreClock according to Clock Register Values
 *
 * This function reads out the CPUs clock and PLL registers and assembles
 * the actual clock speed values into the SystemCoreClock local variable.
 */
void SystemCoreClockUpdate(void)
{
    uint32_t pllmull = 0, plldiv = 0, msirange;
    uint32_t rcc, tmp;

    rcc = RCC->CFGR;

    /* Get SYSCLK source -------------------------------------------------------*/
    switch (rcc & RCC_CFGR_SWS)
    {
        case RCC_CFGR_SWS_MSI:  /* MSI used as system clock , value depends on RCC_ICSCR/MSIRANGE[2:0]: */
            msirange = RCC->ICSCR & RCC_ICSCR_MSIRANGE ;
            msirange = msirange>>_BI16(RCC_ICSCR_MSIRANGE_1);
            sys_clock = MSIFreqTable[msirange];
            break;
        case RCC_CFGR_SWS_HSI:  /* HSI used as system clock */
            sys_clock = HSI_VALUE;
            break;
        case RCC_CFGR_SWS_HSE:  /* HSE used as system clock */
            sys_clock = HSE_VALUE;
            if (RCC->CR & RCC_CR_HSIDIVF) {
                sys_clock = sys_clock /4;
            }
            break;
        case RCC_CFGR_SWS_PLL:
            /* Assume that values not allowed don't occure*/
            if      (rcc & RCC_CFGR_PLLMUL4)  pllmull =  4;
            else if (rcc & RCC_CFGR_PLLMUL6)  pllmull =  6;
            else if (rcc & RCC_CFGR_PLLMUL8)  pllmull =  8;
            else if (rcc & RCC_CFGR_PLLMUL12) pllmull = 12;
            else if (rcc & RCC_CFGR_PLLMUL16) pllmull = 16;
            else if (rcc & RCC_CFGR_PLLMUL24) pllmull = 24;
            else if (rcc & RCC_CFGR_PLLMUL32) pllmull = 32;
            else if (rcc & RCC_CFGR_PLLMUL48) pllmull = 48;
            else                              pllmull =  3;

            if ((rcc & RCC_CFGR_PLLDIV4) == RCC_CFGR_PLLDIV4) plldiv = 4;
            else if (rcc & RCC_CFGR_PLLDIV3)                  plldiv = 3;
            else                                              plldiv = 2;

            if (rcc & RCC_CFGR_PLLSRC_HSE) {
                sys_clock = HSE_VALUE * pllmull / plldiv;
            } else {
                sys_clock = HSI_VALUE * pllmull / plldiv;
                if (RCC->CR & RCC_CR_HSIDIVF)
                    sys_clock = sys_clock /4;
            }
    }

    /* Compute HCLK clock frequency ----------------*/
    tmp = rcc & (RCC_CFGR_HPRE_0 | RCC_CFGR_HPRE_1 |RCC_CFGR_HPRE_2);
    if (rcc & RCC_CFGR_HPRE_3) {
        clk_shift[NUT_HWCLK_CPU] = tmp +1;
    } else {
        clk_shift[NUT_HWCLK_CPU] = 0;
    }
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
 * \return 0 on success, -1 on HSI start failed.
 */
int CtlMsiClock( uint8_t ena)
{
    int rc = 0;

    rc = rcc_set_and_wait_rdy(&RCC->CR, RCC_CR_MSION, RCC_CR_MSIRDY,
            ena, HSE_STARTUP_TIMEOUT);

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

    rc = rcc_set_and_wait_rdy(&RCC->CR, RCC_CR_PLLON,  RCC_CR_PLLRDY,
                                   ena, HSE_STARTUP_TIMEOUT);

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
            RCC->CFGR |= RCC_CFGR_PLLSRC;
        }
    }
    else if (src == PLLCLK_HSI) {
        rc = CtlHsiClock(1);
        /* Select HSI16/HSI16/4 as PLL clock source */
        if (rc==0) {
            RCC->CFGR &= ~RCC_CFGR_PLLSRC;
        }
    }
    return rc;
}

static void WaitClockSwitch(uint32_t value)
{
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= value;
    value = value << 2;
    /* Wait till HSE is used as system clock source */
    while ((RCC->CFGR & RCC_CFGR_SWS) != value);
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

    /* Fixme: Set MSI source with MSI frequency parameter */
    if (src == SYSCLK_MSI) {
        rc = CtlMsiClock(1);
        if (rc == 0) {
            WaitClockSwitch(RCC_CFGR_SW_MSI);
        }
    }
    else if (src == SYSCLK_HSE) {
        rc = CtlHseClock(1);
        if (rc == 0) {
            WaitClockSwitch(RCC_CFGR_SW_HSE);
        }
    }
    else if ((src == SYSCLK_HSI) || (src == SYSCLK_HSIDIV)) {
        rc = CtlHsiClock(1);
        if (rc == 0) {
            if (src == SYSCLK_HSIDIV) {
                RCC->CR |= RCC_CR_HSIDIVEN;
            } else{
                RCC->CR &= ~RCC_CR_HSIDIVEN;
            }
           /* Select HSI as system clock source */
            WaitClockSwitch(RCC_CFGR_SW_HSI);
        }
    } else if (src == SYSCLK_PLL) {
        rc  = SetPllClockSource(PLLCLK_SOURCE);
        if (rc == 0) {
            WaitClockSwitch(RCC_CFGR_SW_PLL);
        }
        rc = CtlPllClock(1);
    }

    /* Update core clock information */
    SystemCoreClockUpdate();

    return rc;
}

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

    /* Switch on HSE to have intermediate clock */
    rc = CtlMsiClock(1);
    if (rc == 0)
        WaitClockSwitch(RCC_CFGR_SW_MSI);
    /* Switch off PLL */
    rc = CtlPllClock(0);
    cfgr = RCC->CFGR;

    cfgr &= ~(RCC_CFGR_HPRE|RCC_CFGR_PPRE1|RCC_CFGR_PPRE2);

    /* HCLK = SYSCLK */
    cfgr |= RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK */
    cfgr |= PPRE2;

    /* PCLK1 = HCLK */
    cfgr |= PPRE1;

    /* PCLK1 = HCLK */
    cfgr |= PLLM | PLLN;

    RCC->CFGR = cfgr;

    rc = SetSysClockSource(SYSCLK_SOURCE);

    return rc;
}
