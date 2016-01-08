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
#if !defined(HSE_BYPASS)
    byp = !byp;
#endif
    if (rc && (ena) && byp) {
        return 0;
    } else {
        /* switch HSE off to allow to set HSE_BYPASS */
        rc = rcc_set_and_wait_rdy(&RCC->CR, RCC_CR_HSEON, RCC_CR_HSERDY,
                              0, HSE_STARTUP_TIMEOUT);
    }
    if (ena) {
#if defined(HSE_BYPASS)
        RCC->CR |= RCC_CR_HSEBYP;
#else
        RCC->CR &= ~RCC_CR_HSEBYP;
#endif

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

    rc = rcc_set_and_wait_rdy(&RCC->CR, RCC_CR_HSION, RCC_CR_HSIRDY,
            ena, HSE_STARTUP_TIMEOUT);

    return rc;
}

/*!
 * \brief Control LSI clock.
 *
 * When there are useres of LSI, LSI might be on even if LSION == 0.
 *
 * \param  ena 0 disable clock, any other value enable it.
 * \return none
 */

static void CtlLsiClock(int ena)
{
    if (ena) {
        RCC->CSR |= RCC_CSR_LSION;
    } else {
        RCC->CSR &= ~RCC_CSR_LSION;
    }
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

/*!
 * \brief Control LSE clock.
 *
 * This function is called early during startup. LSE startup takes
 * typical 2 seconds. If PLL-stabilized MSI is needed, code must
 * wait for LSEON before enabling MSIPLL in another place.
 * NutRegisterIrqHandler is not available yet!
 *
 * \param  ena 0 disable clock, any other value enable it.
 * \return none
 */

static void CtlLseClock(int ena)
{
    uint32_t bdcr;
    int lse_state;
    bdcr = RCC_BDCR;
    /* Check state of LSE */
    if (bdcr & RCC_BDCR_LSERDY) {
        lse_state = 1;
    } else {
        lse_state = 0;
    }
    if ((ena && !lse_state) || (!ena && lse_state)) {
        /* Allow read access to the Backup Registers */
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        PWR_CR |= PWR_CR_DBP;
        if (ena) {
            bdcr &= ~(RCC_BDCR_LSEDRV_MASK | RCC_BDCR_LSEBYP);
# if defined(LSE_BYPASS)
            bdcr |= RCC_BDCR_LSEBYP;
# endif
            bdcr |= RTC_LSEMODF;
            bdcr |= RCC_BDCR_LSEON;
            RCC_BDCR = bdcr;
        } else {
            bdcr &= ~RCC_BDCR_LSEON;
            RCC_BDCR = bdcr;
            while(RCC_BDCR & RCC_BDCR_LSERDY);
        PWR_CR &= ~PWR_CR_DBP;
        }
    }
}

/* HSE_VALUE / RTCPRE must be <= 1 MHz*/
# if !defined(RTCPRE)
/* L0/L1 may divide HSE by 2/4/8/16*/
#  if defined(RCC_CR_RTCPRE_1)
#   if HSE_VALUE > 8000000
#    define RTCPRE 3
#   elif HSE_VALUE > 4000000
#    define RTCPRE 2
#   elif HSE_VALUE > 2000000
#    define RTCPRE 1
#   else
#    define RTCPRE 0
#   endif
#  elif defined(RCC_CFGR_RTCPRE_3)
/* F2/F4/F7 may devide HSE by 2..32*/
#   if HSE_VALUE < 2000000
#    define RTCPRE 2
#   else
#    define RTCPRE (((HSE_VALUE - 1) / 1000000) + 1)
#   endif
#  endif
# endif
/* F0/F3/L4 uses HSE/32, F1 uses HSE/128 */

/**
  * @brief  Set RTC clock to selected source.
  *
  * Assume related clocks already activated. LSE might still not be on.
  *
  * @param  source Clock source LSI/LSE/HSE
  * @retval -1 on error, 0 on success
  */
int SetRtcClock(int source)
{
    int rc = 0;

    if (RCC_BDCR & RCC_BDCR_RTCEN) {
        /* RTC is already running. Reset Backup domain if source differs*/
        switch (RCC_BDCR & RCC_BDCR_RTCSEL) {
        case 0: /* No source selected */
            break;
        case RCC_BDCR_RTCSEL_0: /* LSE */
            if (source == RTCCLK_LSE) {
                return 0;
            }
            break;
        case RCC_BDCR_RTCSEL_1: /* LSI */
            if (source == RTCCLK_LSI) {
                return 0;
            }
            break;
        case RCC_BDCR_RTCSEL: /* LSE */
            if (source != RTCCLK_HSE) {
                break;
            }
#if defined(RCC_CR_RTCPRE)
            if ((RCC_CR & RCC_CR_RTCPRE) != (RTCPRE * RCC_CR_RTCPRE_0)) {
                break;
            }
#endif
            return 0;
        }
    }
    /* Enable PWR Controller and access to the RTC backup domain*/
    PWR_CR |= PWR_CR_DBP;
    rc = -1;
    RCC_BDCR &= ~(RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL);
    switch (source)
    {
    case RTCCLK_LSI:
        if (!(RCC->CSR & RCC_CSR_LSIRDY)) {
            goto done;
        }
        RCC_BDCR |= RCC_BDCR_RTCSEL_1;
        break;
    case RTCCLK_LSE:
        if (!(RCC_BDCR & RCC_BDCR_LSERDY)) {
            int i;

            for (i = 0; i < 4000; i++) {
                if (RCC_BDCR & RCC_BDCR_LSERDY) {
                    break;
                }
            }
        }
        if (!(RCC_BDCR & RCC_BDCR_LSERDY)) {
            goto done;
        }
        /* Enable MSI PLL on L4 now? */
        RCC_BDCR |= RCC_BDCR_RTCSEL_0;
        break;
    case RTCCLK_HSE:
#if defined(RCC_CR_RTCPRE)
   {
        uint32_t rcc_cr;
        rcc_cr = RCC_CR;
        rcc_cr &= RCC_CR_RTCPRE;
        rcc_cr |= RTCPRE * RCC_CR_RTCPRE_0;
        RCC_CR = rcc_cr;
    }
#endif
        RCC_BDCR |= RCC_BDCR_RTCSEL;
        break;
    default:
        /* No source selected*/
        goto done;
    }
    rc = 0;
    RCC_BDCR |= RCC_BDCR_RTCEN;
done:
    PWR_CR &= ~PWR_CR_DBP;
    return rc;
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
