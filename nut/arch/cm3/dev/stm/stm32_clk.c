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

/*
 * \file arch/cm3/stm/stm32_clk.c
 * \brief Common routines for clock setup
 *
 * Include this file in the device specific setup to allow better optimization!
 */

static uint32_t sys_clock;
static uint8_t clk_shift[NUT_HWCLK_MAX];

static const uint8_t AHBPrescTable[16] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
static const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

#if (HSE_BYPASS != ENABLE) &&  (HSE_BYPASS != DISABLE)
# warning HSE_BYPASS needs explicit value
#endif

#if !defined(LSE_VALUE)
# define LSE_VALUE 0
#endif

#if (APB1_DIV == AUTO)
# undef APB1_DIV
# define APB1_DIV 1
#endif
#if (APB2_DIV == AUTO)
# undef APB2_DIV
# define APB2_DIV 1
#endif

#if defined(CLOCK_DEBUG)
#define XSTR(x) STR(x)
#define STR(x) #x

#pragma message "SYSCLK_SOURCE = " XSTR(SYSCLK_SOURCE)
#pragma message "SYSCLK_FREQ   = " XSTR(SYSCLK_FREQ)
#pragma message "PLLCLK_IN     = " XSTR(PLLCLK_IN)
# if defined(MCU_STM32F1_CL)
#pragma message "PLL2CLK_PREDIV= " XSTR(PLL2CLK_PREDIV)
#pragma message "PLL2CLK_MULT  = " XSTR(PLL2CLK_MULT)
# endif
#pragma message "PLLCLK_PREDIV = " XSTR(PLLCLK_PREDIV)
#pragma message "PLLCLK_MULT   = " XSTR(PLLCLK_MULT)
#pragma message "PLLCLK_DIV    = " XSTR(PLLCLK_DIV)
#pragma message "SYSCLK_MAX    = " XSTR(SYSCLK_MAX)
#pragma message "SYSCLK_RES    = " XSTR(SYSCLK_RES)
#pragma message "AHB_DIV       = " XSTR(AHB_DIV)
#pragma message "APB1_DIV      = " XSTR(APB1_DIV)
#if !defined(MCU_STM32F0)
# pragma message "APB2_DIV      = " XSTR(APB2_DIV)
#endif
#endif

#if defined(MCU_STM32F2) || defined(MCU_STM32F4) || defined(MCU_STM32F7)
/*!
  * \brief  Get divisor for APB clock
  *
  * Return needed divisor so that resulting frequency
  * is smaller or equal the selected frequency or maximum
  * PCLK divisor is reached.
  *
  * @param  target_frequency Selected Frequency
  * @retval RCC_CFGR_PPRE base value
  */
static uint32_t GetPclkDiv(uint32_t target_frequency) __attribute__((unused));
static uint32_t GetPclkDiv(uint32_t target_frequency)
{
    uint32_t div;
    uint32_t res_freq;

    div = 3;
    res_freq = SYSCLK_FREQ;
    if (res_freq > target_frequency) {
        div = div + 1;
        res_freq = res_freq /2;
    }
    if (res_freq > target_frequency) {
        div = div + 1;
        res_freq = res_freq /2;
    }
    if (res_freq > target_frequency) {
        div = div + 1;
        res_freq = res_freq /2;
    }
    if (res_freq > target_frequency) {
        div = div + 1;
        res_freq = res_freq /2;
    }
    return div;
}
#endif

/**
  * \brief  Get timer clock shift
  *
  * \param  div  Connected PCLK APB prescaler
  * \retval Corrected prescaler
  */
static uint8_t GetTimerShift(uint8_t shift)
{
    uint8_t res;
#if defined(RCC_DCKCFGR_TIMPRE)
    if ((RCC->DCKCFGR & RCC_DCKCFGR_TIMPRE) ==  RCC_DCKCFGR_TIMPRE) {
        if (shift < 2) {
            res = 0;
        } else {
            res = shift - 2;
        }
        return res;
    }
#endif
    if (shift < 1) {
        res =  0;
    } else {
        res = shift - 1;
    }
    return res;
}

 /*!
 * \brief Set bus dividers
 *
 * \param  ahb_div    Division factor for AHB BUS
 * \param  apb1_div   Division factor for APB1 BUS
 * \param  apb2_div   Division factor for APB2 BUS
 */
static void SetBusDividers(uint32_t ahb_div, uint32_t apb1_div,
                           uint32_t apb2_div) __attribute__((unused));
static void SetBusDividers(uint32_t ahb_div, uint32_t apb1_div,
                           uint32_t apb2_div)
{
    uint32_t cfgr;
    int i;

    cfgr = RCC->CFGR;
    cfgr  &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE2);
    /* Skip zeros at start of table*/
    i = 7;
    while ((1 << AHBPrescTable[i]) < AHB_DIV) {
        i++;
    }
    cfgr |= i * RCC_CFGR_HPRE_0;
#if defined(RCC_CFGR_PPRE1)
    cfgr  &= ~RCC_CFGR_PPRE1;
    i = 3;
    while ((1 << APBPrescTable[i]) < APB1_DIV) {
        i++;
    }
    cfgr |= i * RCC_CFGR_PPRE1_0;
#endif
    i = 3;
    while ((1 << APBPrescTable[i]) < APB2_DIV) {
        i++;
    }
    cfgr |= i * RCC_CFGR_PPRE2_0;
    RCC->CFGR = cfgr;
}

/*!
 * \brief Re/Set RCC register bit and wait for same state of connected RDY bit or timeout
 *
 * \param  reg       Register to check
 * \param  setmask   Bit to (re)set
 * \param  checkmask Bit to check
 * \param  value     Value to set and check
 * \param  tout      timeout in delay units.
 * \return 0 on success, -1 on HSE start failed.
 */
static int rcc_set_and_wait_rdy(__IO uint32_t *reg, uint32_t setmask,
                                uint32_t checkmask, int value, uint32_t tout)
{
    if (value) {
        *reg |= setmask;
    } else {
        *reg &= ~setmask;
    }
    for (; tout; tout--) {
        if (((value) && (*reg & checkmask)) ||
            (!value && (!(*reg & checkmask)))) {
            return 0;
        }
    }
    return -1;
}

/*!
 * \brief Set RCC register bits and wait for status bits
 *
 * \param  reg        Register to check
 * \param  setmask    Bit to set/reset
 * \param  setvalue   Value to set
 * \param  checkmask  Bit to check
 * \param  checkvalue Value to set and check
 * \param  tout       timeout in delay units.
 * \return 0 on success, -1 on HSE start failed.
 */
static int rcc_set_and_wait_rdy_value(
    __IO uint32_t *reg, uint32_t setmask, uint32_t setvalue,
    uint32_t checkmask, int checkvalue, uint32_t tout) __attribute__((unused));
static int rcc_set_and_wait_rdy_value(
    __IO uint32_t *reg, uint32_t setmask, uint32_t setvalue,
    uint32_t checkmask, int checkvalue, uint32_t tout)
{
    uint32_t reg_value;

    reg_value = *reg;
    reg_value &= ~setmask;
    reg_value |= setvalue;
    *reg = reg_value;
    for (; tout; tout--) {
        if ((*reg & checkmask) ==  checkvalue) {
            return 0;
        }
    }
    return -1;
}

/*!
 * \brief Control HSE clock.
 *
 * \param  ena 0 disable clock, any other value enable it.
 * \return 0 on success, -1 on HSE start failed.
 */
int CtlHseClock(int ena)
{
    int rc, byp;
    uint32_t cr;

    /* Is HSE set already? */
    cr = RCC->CR;
    rc = (cr & RCC_CR_HSERDY);
    byp = (cr & RCC_CR_HSEBYP);
    if (rc && (ena) && (byp == (HSE_BYPASS))) {
        return 0;
    } else {
        /* switch HSE off to allow to set HSE_BYPASS */
        rc = rcc_set_and_wait_rdy(&RCC->CR, RCC_CR_HSEON, RCC_CR_HSERDY,
                              0, HSE_STARTUP_TIMEOUT);
    }
    if (ena) {
        if (HSE_BYPASS) {
            RCC->CR |= RCC_CR_HSEBYP;
        } else {
            RCC->CR &= ~RCC_CR_HSEBYP;
        }

        /* Enable HSE */
        rc = rcc_set_and_wait_rdy(&RCC->CR, RCC_CR_HSEON, RCC_CR_HSERDY,
                                  1, HSE_STARTUP_TIMEOUT);
    }
    return rc;
}

/*!
 * \brief Control HSI clock.
 *
 * \param  ena 0 disable clock, any other value enable it.
 * \return 0 on success, -1 on HSI start failed.
 */
int CtlHsiClock(int ena)
{
    int rc = 0;

#if defined(RCC_CR_HSI16DIVF)
    while ((RCC->CR & RCC_CR_HSI16DIVF) != HSI_DIVIDE_BY_FOUR) {
        if (HSI_DIVIDE_BY_FOUR) {
            RCC->CR |= RCC_CR_HSI16DIVEN;
        } else {
            RCC->CR &= ~RCC_CR_HSI16DIVEN;
        }
    }
#endif
    rc = rcc_set_and_wait_rdy(&RCC->CR, RCC_CR_HSION, RCC_CR_HSIRDY,
            ena, HSE_STARTUP_TIMEOUT);

    return rc;
}

#if defined(RCC_BDCR_LSEMOD)
/* F411/446 have one power level setting for the LSE */
# define RCC_BDCR_LSEDRV_MASK RCC_BDCR_LSEMOD
# define RTC_LSEMODF ((LSE_DRIVE_LEVEL * RCC_BDCR_LSEMOD) & RCC_BDCR_LSEMOD)
#elif defined(RCC_BDCR_LSEDRV)
/* F0, L0, F3, F7 and L4 have four levels*/
# define RCC_BDCR_LSEDRV_MASK RCC_BDCR_LSEDRV
# define RTC_LSEMODF ((LSE_DRIVE_LEVEL * RCC_BDCR_LSEDRV_0) & RCC_BDCR_LSEDRV)
#else
# define RCC_BDCR_LSEDRV_MASK 0
# define RTC_LSEMODF 0
#endif

/**
  * \brief  Set RTC clock to selected source.
  *
  * Call early during startup, as LSE Startup may take two seconds!
  *
  * Check, if LSEBYP or RTCEN need change
  * Change can only be done by resetting Backup Domain.
  * With reset, rebuild LSEON, LSEBYP or RTCEN.
  * Check success later when using RTC clock.
  *
  * \param  source Clock source NONE/LSI/LSE/HSE
  * \retval NONE
  */
void SetRtcClockSource(int source)
{
    int lsebyp_state;

    if (RCC_BDCR & RCC_BDCR_RTCEN) {
        /* RTC is already running. Reset Backup domain if source differs*/
        switch (RCC_BDCR & RCC_BDCR_RTCSEL) {
        case 0: /* No source selected */
            if (source == RTCCLK_NONE) {
                goto check_lse;
            }
            break;
        case RCC_BDCR_RTCSEL_0: /* LSE */
            lsebyp_state = (RCC_BDCR & RCC_BDCR_LSEBYP);
            if ((source == RTCCLK_LSE) && (lsebyp_state == LSE_BYPASS)) {
                return;
            }
            break;
        case RCC_BDCR_RTCSEL_1: /* LSI */
            if (source == RTCCLK_LSI) {
                goto check_lse;
            }
            break;
        case RCC_BDCR_RTCSEL: /* HSE */
            if (source == RTCCLK_HSE) {
                goto check_lse;
            }
            break;
        }
    }
    /* Enable backup domain access*/
    PWR_CR |= PWR_CR_DBP;
    /* Reset LSEON, RTCSEL and backupregisters, as RTC clock source/LSEBYP needs change*/
    RCC_BDCR |= RCC_BDCR_BDRST;
    RCC_BDCR &= ~RCC_BDCR_BDRST;
    /* If LSE_VALUE is > 0, we assume we need LSE anyway */
    switch (source) {
    case RTCCLK_LSI:
        RCC_BDCR |= RCC_BDCR_RTCSEL_1;
        break;
    case RTCCLK_LSE:
        RCC_BDCR |= RCC_BDCR_RTCSEL_0 | ((LSE_BYPASS * RCC_BDCR_LSEBYP));
        break;
    case RTCCLK_HSE:
        RCC_BDCR |= RCC_BDCR_RTCSEL;
        break;
    }
check_lse:
    /* LSE might be needed even if no RTC is needed */
    if (LSE_VALUE) {
        RCC_BDCR |= RCC_BDCR_LSEON;
    }
    if (LSI_ON) {
        RCC->CSR |= RCC_CSR_LSION;
    } else {
        RCC->CSR &= ~RCC_CSR_LSION;
    }
    PWR_CR &= ~PWR_CR_DBP;
}

/**
  * \brief  requests System clock frequency
  *
  * \param  None
  * \retval None
  */
uint32_t SysCtlClockGet(void)
{
    return STM_ClockGet(NUT_HWCLK_CPU);
}

static void SystemCoreClockUpdate(void);
/**
  * \brief  requests frequency of the given clock
  *
  * \param  idx NUT_HWCLK Index
  * \retval clock or 0 if idx points to an invalid clock
  */
uint32_t STM_ClockGet(int idx)
{
    if (!sys_clock) {
        SystemCoreClockUpdate();
    }
    if (idx < NUT_HWCLK_MAX) {
        return sys_clock >> clk_shift[idx];
    }
    return 0;
}

/*!
 * \brief Switch to PLL with selected source
 *
 * \param  ahb_div    Division factor for AHB BUS
 * \param  apb1_div   Division factor for APB1 BUS
 * \param  apb2_div   Division factor for APB2 BUS
 */
static int SwitchSystemClock(int source)  __attribute__((unused));
static int SwitchSystemClock(int source)
{
    int rc = -1;

    switch (source) {
    case SYSCLK_HSI:
        rc = CtlHsiClock(1);
        if (!rc) {
            rc = rcc_set_and_wait_rdy_value(
                &RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI,
                RCC_CFGR_SWS, RCC_CFGR_SWS_HSI, HSE_STARTUP_TIMEOUT);
        }
        break;
#if defined(RCC_CFGR_SWS_HSI48)
    case SYSCLK_HSI48:
        rc = rcc_set_and_wait_rdy(&RCC->CR2, RCC_CR2_HSI48ON, RCC_CR2_HSI48RDY,
                                  1, HSE_STARTUP_TIMEOUT);
        if (!rc) {
            rc = rcc_set_and_wait_rdy_value(
                &RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI48,
                RCC_CFGR_SWS, RCC_CFGR_SWS_HSI48, HSE_STARTUP_TIMEOUT);
        }
        break;
#endif
#if defined(RCC_CFGR_SWS_MSI)
    case SYSCLK_MSI:
        rc = CtlMsiClock(MSI_DEFAULT);
        if (!rc) {
            rc = rcc_set_and_wait_rdy_value(
                &RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_MSI,
                RCC_CFGR_SWS, RCC_CFGR_SWS_MSI, HSE_STARTUP_TIMEOUT);
        }
        break;
#endif
    case SYSCLK_HSE:
        rc = CtlHseClock(1);
        if (!rc) {
            rc = rcc_set_and_wait_rdy_value(
                &RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSE,
                RCC_CFGR_SWS, RCC_CFGR_SWS_HSE, HSE_STARTUP_TIMEOUT);
        }
        break;
    case SYSCLK_PLL:
        rc = SetPllClockSource(PLLCLK_SOURCE);
        if (!rc) {
            /* Switch On PLL */
            rc =  rcc_set_and_wait_rdy(&RCC->CR, RCC_CR_PLLON, RCC_CR_PLLRDY,
                                       1, HSE_STARTUP_TIMEOUT);
            if (!rc) {
                rc = rcc_set_and_wait_rdy_value(
                    &RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL,
                    RCC_CFGR_SWS, RCC_CFGR_SWS_PLL, HSE_STARTUP_TIMEOUT);
            }
        }
        break;
    }
    if (rc) {
#if defined(RCC_CFGR_SWS_MSI)
        /* something went wrong, switch to MSI */
        CtlMsiClock(MSI_DEFAULT);
        rcc_set_and_wait_rdy_value(
            &RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_MSI,
            RCC_CFGR_SWS, RCC_CFGR_SWS_MSI, HSE_STARTUP_TIMEOUT);
#else
        CtlHsiClock(1);
        rcc_set_and_wait_rdy_value(
            &RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI,
            RCC_CFGR_SWS, RCC_CFGR_SWS_HSI, HSE_STARTUP_TIMEOUT);
#endif
    }
    return rc;
}

/*!
 * \brief Set Clock Shift array
 *
 */
static void SetClockShift(void)  __attribute__((unused));
static void SetClockShift(void)
{
    uint32_t hpre;
    uint32_t cfgr;
    uint32_t tmp;

    cfgr = RCC->CFGR;
    hpre = (cfgr & RCC_CFGR_HPRE) / RCC_CFGR_HPRE_0;
    clk_shift[NUT_HWCLK_CPU] = AHBPrescTable[hpre];
    tmp = (RCC->CFGR & RCC_CFGR_PPRE1) / RCC_CFGR_PPRE1_0;
    clk_shift[NUT_HWCLK_PCLK1] = APBPrescTable[tmp];
    clk_shift[NUT_HWCLK_TCLK1] = GetTimerShift(clk_shift[NUT_HWCLK_PCLK1]);
#if !defined(MCU_STM32F0)
    tmp = (RCC->CFGR & RCC_CFGR_PPRE2) / RCC_CFGR_PPRE2_0;
    clk_shift[NUT_HWCLK_PCLK2] = APBPrescTable[tmp];
    clk_shift[NUT_HWCLK_TCLK2] = GetTimerShift(clk_shift[NUT_HWCLK_PCLK2]);
#endif
}
