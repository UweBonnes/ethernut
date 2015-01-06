/*
 * Copyright (C) 2013 by Uwe Bonnes <bon@elektron.ikp.physik.tu-darmstadt.de>
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

#include <stdint.h>
#include <cfg/arch.h>
#include <arch/cm3/stm/stm32xxxx.h>

#define GPIO_DEFAULT_MODE          0
#define GPIO_DEFAULT_MODE_FLOAT    1
#define GPIO_DEFAULT_MODE_ANALOG   2
#define GPIO_DEFAULT_MODE_PULLDOWN 3
#define GPIO_DEFAULT_MODE_PULLUP   4

#if !defined(RCC_AHBENR_GPIOFEN)
#define RCC_AHBENR_GPIOFEN 0
#endif
#if !defined(RCC_AHBENR_GPIOGEN)
#define RCC_AHBENR_GPIOGEN 0
#endif
#if !defined(RCC_AHBENR_GPIOHEN)
#define RCC_AHBENR_GPIOHEN 0
#endif

#if !defined(RCC_AHB1ENR_GPIOFEN)
#define RCC_AHB1ENR_GPIOFEN 0
#endif
#if !defined(RCC_AHB1ENR_GPIOGEN)
#define RCC_AHB1ENR_GPIOGEN 0
#endif
#if !defined(RCC_AHB1ENR_GPIOHEN)
#define RCC_AHB1ENR_GPIOHEN 0
#endif
#if !defined(RCC_AHB1ENR_GPIOIEN)
#define RCC_AHB1ENR_GPIOIEN 0
#endif
#if !defined(RCC_AHB1ENR_GPIOJEN)
#define RCC_AHB1ENR_GPIOJEN 0
#endif
#if !defined(RCC_AHB1ENR_GPIOKEN)
#define RCC_AHB1ENR_GPIOKEN 0
#endif

#if !defined(STM32_GPIO_DEFAULT_MODE) || STM32_GPIO_DEFAULT_MODE == GPIO_DEFAULT_MODE || defined( GPIO_CRL_CNF)
#define GpioSetDefault()
#elif STM32_GPIO_DEFAULT_MODE == GPIO_DEFAULT_MODE_ANALOG
static void GpioSetDefault(void)
{
    uint32_t ahbxenr_value;
    volatile uint32_t *ahbxenr;
#if defined(RCC_AHBENR_GPIOAEN)
    ahbxenr = &RCC->AHBENR;
    ahbxenr_value =  *ahbxenr;
    *ahbxenr = ahbxenr_value |
        RCC_AHBENR_GPIOAEN |  RCC_AHBENR_GPIOBEN |  RCC_AHBENR_GPIOCEN |
        RCC_AHBENR_GPIODEN |  RCC_AHBENR_GPIOEEN |  RCC_AHBENR_GPIOFEN |
        RCC_AHBENR_GPIOGEN |  RCC_AHBENR_GPIOHEN;
#else
    ahbxenr = &RCC->AHB1ENR;
    ahbxenr_value =  *ahbxenr;
    *ahbxenr = ahbxenr_value |
        RCC_AHB1ENR_GPIOAEN |  RCC_AHB1ENR_GPIOBEN |  RCC_AHB1ENR_GPIOCEN |
        RCC_AHB1ENR_GPIODEN |  RCC_AHB1ENR_GPIOEEN |  RCC_AHB1ENR_GPIOFEN |
        RCC_AHB1ENR_GPIOGEN |  RCC_AHB1ENR_GPIOHEN |  RCC_AHB1ENR_GPIOIEN |
        RCC_AHB1ENR_GPIOJEN |  RCC_AHB1ENR_GPIOKEN;
#endif

/* Only SWD on F0/L0 */
#if defined(__STM32F0XX_H)
    GPIOA->MODER |= 0xc3ffffff;
    GPIOB->MODER |= 0xffffffff;
#else
    GPIOA->MODER |= 0x03ffffff;
    GPIOB->MODER |= 0xfffffc3f;
#endif
    GPIOC->MODER |= 0xffffffff;
    GPIOD->MODER |= 0xffffffff;
    GPIOE->MODER |= 0xffffffff;
#if defined(GPIOF_BASE)
    GPIOF->MODER |= 0xffffffff;
#endif
#if defined(GPIOG_BASE)
    GPIOG->MODER |= 0xffffffff;
#endif
#if defined(GPIOH_BASE)
    GPIOH->MODER |= 0xffffffff;
#endif
#if defined(GPIOI_BASE)
    GPIOI->MODER |= 0xffffffff;
#endif
#if defined(GPIOJ_BASE)
    GPIOJ->MODER |= 0xffffffff;
#endif
#if defined(GPIOK_BASE)
    GPIOK->MODER |= 0xffffffff;
#endif

    *ahbxenr = ahbxenr_value;
}
#else
#warning Unknown GPIO default mode
#endif

#if defined(RCC_CR_MSION)
#define RCC_CR_RESET_VALUE RCC_CR_MSION 
#else
#if !defined(RCC_CR_HSITRIM_4)
#define RCC_CR_RESET_VALUE (RCC_CR_HSION | 0x80)
#else
#define RCC_CR_RESET_VALUE (RCC_CR_HSION | RCC_CR_HSITRIM_4)
#endif
#endif

/** @addtogroup STM32_System_Defines
  * @{
  */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Switch back to default clock state.
 *
 *         We write the default reset values to the appropriated registers.
 *         Some bits can only be written in certain constellation, so we write
 *         multiple time, as previous writes prepare these constellations.
 *         We keep eventual CIR IRQ pendings flags hanging...
 */
void SystemInit (void)
{
    RCC_TypeDef *rcc = (RCC_TypeDef *) RCC_BASE;

    GpioSetDefault();
    /* FPU settings ---------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    /* set CP10 and CP11 Full Access */
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2)); 
#endif
    /* Disable clock related interrupts*/
    rcc->CIR = 0;
    /* Switch on HSI Clock */
    rcc->CR = RCC_CR_RESET_VALUE;
    rcc->CFGR = 0;
    /* Now HSEON, CSSON and PLLON should reset*/
    rcc->CR   = RCC_CR_RESET_VALUE;
#if defined(RCC_PLLCFGR_PLLM)
    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE bits */
    rcc->PLLCFGR = 0;
#endif
    /* Now HSEBYP should reset*/
    rcc->CR = RCC_CR_RESET_VALUE;
#if defined(RCC_CFGR_PLLSRC)
    /* Reset PLL bits*/
    rcc->CFGR = 0;
#endif
#if defined(RCC_CFGR2_PREDIV1)
    rcc->CFGR2 = 0;
#endif
#if defined(RCC_CFGR3_USART1SW1)
    rcc->CFGR3 = 0;
#endif
/* Seperate USB and CAN interrupts on F30 */
#if defined(STM32F30X)
    rcc->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG_TypeDef *syscfg = (SYSCFG_TypeDef *) SYSCFG_BASE;
    syscfg->CFGR1 |= SYSCFG_CFGR1_USB_IT_RMP;
#endif

}

/**
 * @}
 */
