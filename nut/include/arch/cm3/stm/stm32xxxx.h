#ifndef _STM32XXXX_H_
#define _STM32XXXX_H_
/*
 * Copyright (C) 2012-2018 Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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
 *
 */

/*!
 * \file arch/cm3/stm/atm32xxxx.h
 * \brief Wrapper for the device dependant stm32XZxx.h files.
 * $Id: stm32_flash.h 3220 2010-11-12 13:04:17Z astralix $
 * \verbatim
 */

#include <cfg/arch.h>
#if defined(VENDOR)
# include VENDOR
#else
# warning "Unknown STM32 family"
#endif

/* Amount of flash available over all banks.*/
#if !defined(FLASH_END) && defined(FLASH_BANK2_END)
# define FLASH_END FLASH_BANK2_END
#endif
#if !defined(FLASH_END) && defined(FLASH_BANK1_END)
# define FLASH_END FLASH_BANK1_END
#endif
#if defined(FLASH_END) && !defined(FLASH_SIZE)
# define FLASH_SIZE (FLASH_END + 1 - FLASH_BASE)
#endif
#define FLASH_PAGE_SIZE (1 << FLASH_PAGE_SHIFT)
#define FLASH_PAGE_MASK (~(FLASH_PAGE_SIZE - 1))
#define FLASH_PAGES_BITS (FLASH_SIZE / FLASH_PAGE_SIZE)
#define FLASH_PAGES_WORDS (FLASH_PAGES_BITS / 32)

/* Equalize name changes in newer header versions.*/
#if !defined(FLASH_PECR_FIX) && defined(FLASH_PECR_FTDW)
# define FLASH_PECR_FIX FLASH_PECR_FTDW
#endif

/* Equalize names in a common place. Even recent CUBE uses "random" names.*/
#if defined(GPIO_BRR_BR0) && !defined(GPIO_BRR_BR_0) && !defined(MCU_STM32F4)
# define GPIO_BRR_BR_0 GPIO_BRR_BR0
#endif

#if defined(SRAM1_BASE) && !defined(SRAM_BASE)
#define SRAM_BASE SRAM1_BASE
#endif

#if defined(RCC_APB1ENR_CANEN) && !defined(RCC_APB1ENR_CAN1EN)
#define RCC_APB1ENR_CAN1EN RCC_APB1ENR_CANEN
#endif
#if defined(RCC_APB1RSTR_CANRST) && !defined(RCC_APB1RSTR_CAN1RST)
#define RCC_APB1RSTR_CAN1RST RCC_APB1RSTR_CANRST
#endif
#if defined(CAN) && !defined(CAN1)
#define CAN1 CAN
#endif
#if defined(CAN_BASE) && !defined(CAN1_BASE)
#define CAN1_BASE CAN_BASE
#endif

#if defined(RCC_APB1ENR_DACEN) && !defined(RCC_APB1ENR_DAC1EN)
#define RCC_APB1ENR_DAC1EN RCC_APB1ENR_DACEN
#endif

# if !defined(PWR_CR_DBP) && defined (PWR_CR1_DBP)
#  define PWR_CR_DBP   PWR_CR1_DBP
#  define PWR_CR_VOS   PWR_CR1_VOS
#  define PWR_CR_VOS_0 PWR_CR1_VOS_0
#  define PWR_CR (PWR->CR1)
# else
#  define PWR_CR (PWR->CR)
# endif

# if !defined(RCC_CIFR_LSERDYF) && defined(RCC_CIR_LSERDYF)
#  define RCC_CIFR (RCC->CIR)
#  define RCC_CIER (RCC->CIR)
#  define RCC_CICR (RCC->CIR)
#  define RCC_CIFR_LSERDYF   RCC_CIR_LSERDYF
#  define RCC_CIER_LSERDYIE  RCC_CIR_LSERDYIE
#  define RCC_CICR_LSERDYC   RCC_CIR_LSERDYC
# else
#  define RCC_CIFR (RCC->CIFR)
#  define RCC_CIER (RCC->CIER)
#  define RCC_CICR (RCC->CICR)
#endif

# if !defined(RCC_BDCR_LSEBYP) && defined(RCC_CSR_LSEBYP)
/* L0 and  L1 */
#  define RCC_BDCR (RCC->CSR)
#  define RCC_BDCR_LSEBYP RCC_CSR_LSEBYP
#  define RCC_BDCR_LSERDY RCC_CSR_LSERDY
#  define RCC_BDCR_RTCEN  RCC_CSR_RTCEN
#  define RCC_BDCR_RTCSEL_0  RCC_CSR_RTCSEL_0
#  define RCC_BDCR_RTCSEL_1  RCC_CSR_RTCSEL_1
#  define RCC_BDCR_RTCSEL    RCC_CSR_RTCSEL
#  define RCC_BDCR_BDRST     RCC_CSR_RTCRST
#  define RCC_BDCR_LSIRDY    RCC_CSR_LSIRDY
#  define RCC_BDCR_LSION     RCC_CSR_LSION
#  if defined(RCC_CSR_LSEDRV)
/* L0 */
#   define RCC_BDCR_LSEDRV_0 RCC_CSR_LSEDRV_0
#   define RCC_BDCR_LSEDRV   RCC_CSR_LSEDRV
#  else
/* L1 */
#   define RCC_BDCR_LSEDRV_0 1
#   define RCC_BDCR_LSEDRV   0
#   define NO_LSEDRV
#  endif
# elif defined(RCC_BDCR_LSEBYP)
#  if   defined(RCC_BCDR_LSEMOD)
/* F411, F446, F469 and F479*/
#   define RCC_BDCR_LSEDRV_0 RCC_BDCR_LSEMOD
#   define RCC_BDCR_LSEDRV   RCC_BDCR_LSEMOD
#  elif !defined(RCC_BDCR_LSEDRV)
/* F1, F2 and older F4 */
#   define RCC_BDCR_LSEDRV_0 1
#   define RCC_BDCR_LSEDRV   0
#   define NO_LSEDRV
#  else
/* F0, F3, F7 and F4 */
#  endif
# endif
#if !defined(RCC_BDCR)
# define RCC_BDCR (RCC->BDCR)
#endif

# if !defined(RCC_BDCR_LSEON) && defined(RCC_CSR_LSEON)
#  define RCC_BDCR_LSEON RCC_CSR_LSEON
# endif

# if !defined(RCC_CR_RTCPRE_0) && defined(RCC_CFGR_RTCPRE_0)
#  define RCC_CR (RCC->CFGR)
# else
#  define RCC_CR (RCC->CR)
# endif

# if !defined(FLASH_SR_PGSERR) && defined(FLASH_SR_ERSERR)
#  define FLASH_SR_PGSERR FLASH_SR_ERSERR
# endif

#if defined(RCC_APB1ENR1_TIM2EN_Pos)
# define APB1ENR APB1ENR1
# define RCC_APB1ENR_TIM2EN   RCC_APB1ENR1_TIM2EN
# define RCC_APB1ENR_TIM3EN   RCC_APB1ENR1_TIM3EN
# define RCC_APB1ENR_TIM4EN   RCC_APB1ENR1_TIM4EN
# define RCC_APB1ENR_TIM5EN   RCC_APB1ENR1_TIM5EN
# define RCC_APB1ENR_TIM6EN   RCC_APB1ENR1_TIM6EN
# define RCC_APB1ENR_TIM7EN   RCC_APB1ENR1_TIM7EN
# define RCC_APB1ENR_SPI2EN   RCC_APB1ENR1_SPI2EN
# define RCC_APB1ENR_SPI3EN   RCC_APB1ENR1_SPI3EN
# define RCC_APB1ENR_USART2EN RCC_APB1ENR1_USART2EN
# define RCC_APB1ENR_USART3EN RCC_APB1ENR1_USART3EN
# define RCC_APB1ENR_UART4EN  RCC_APB1ENR1_UART4EN
# define RCC_APB1ENR_UART5EN  RCC_APB1ENR1_UART5EN
# define RCC_APB1ENR_I2C1EN   RCC_APB1ENR1_I2C1EN
# define RCC_APB1ENR_I2C2EN   RCC_APB1ENR1_I2C2EN
# define RCC_APB1ENR_I2C3EN   RCC_APB1ENR1_I2C3EN
# define RCC_APB1ENR_CAN1EN   RCC_APB1ENR1_CAN1EN
# define RCC_APB1ENR_PWREN    RCC_APB1ENR1_PWREN
# define RCC_APB1ENR_DAC1EN   RCC_APB1ENR1_DAC1EN
# define RCC_APB1ENR_LCDEN    RCC_APB1ENR1_LCDEN

# define APB1RSTR             APB1RSTR1
# define RCC_APB1RSTR_TIM2RST   RCC_APB1RSTR1_TIM2RST
# define RCC_APB1RSTR_TIM3RST   RCC_APB1RSTR1_TIM3RST
# define RCC_APB1RSTR_TIM4RST   RCC_APB1RSTR1_TIM4RST
# define RCC_APB1RSTR_TIM5RST   RCC_APB1RSTR1_TIM5RST
# define RCC_APB1RSTR_TIM6RST   RCC_APB1RSTR1_TIM6RST
# define RCC_APB1RSTR_TIM7RST   RCC_APB1RSTR1_TIM7RST
# define RCC_APB1RSTR_SPI2RST   RCC_APB1RSTR1_SPI2RST
# define RCC_APB1RSTR_SPI3RST   RCC_APB1RSTR1_SPI3RST
# define RCC_APB1RSTR_USART2RST RCC_APB1RSTR1_USART2RST
# define RCC_APB1RSTR_USART3RST RCC_APB1RSTR1_USART3RST
# define RCC_APB1RSTR_UART4RST  RCC_APB1RSTR1_UART4RST
# define RCC_APB1RSTR_UART5RST  RCC_APB1RSTR1_UART5RST
# define RCC_APB1RSTR_I2C1RST   RCC_APB1RSTR1_I2C1RST
# define RCC_APB1RSTR_I2C2RST   RCC_APB1RSTR1_I2C2RST
# define RCC_APB1RSTR_I2C3RST   RCC_APB1RSTR1_I2C3RST
# define RCC_APB1RSTR_CAN1RST   RCC_APB1RSTR1_CAN1RST
# define RCC_APB1RSTR_PWRRST    RCC_APB1RSTR1_PWRRST
# define RCC_APB1RSTR_DAC1RST   RCC_APB1RSTR1_DAC1RST

# define AHBENR AHB1ENR
# define RCC_AHBENR_DMA1EN RCC_AHB1ENR_DMA1EN
# define RCC_AHBENR_DMA2EN RCC_AHB1ENR_DMA2EN

# if defined(USART_CR1_TXEIE_TXFNFIE) && !defined(USART_CR1_TXEIE)
#  define USART_CR1_TXEIE  USART_CR1_TXEIE_TXFNFIE
#  define USART_CR1_RXNEIE USART_CR1_RXNEIE_RXFNEIE
#  define USART_ISR_TXE    USART_ISR_TXE_TXFNF
#  define USART_ISR_RXNE   USART_ISR_RXNE_RXFNE
# endif
#endif

#if defined(EXTI_PR1_PIF0) && !defined(EXTI_PR_PR0)
# define EXTI_PR   (EXTI->PR1)
# define EXTI_IMR  (EXTI->IMR1)
# define EXTI_FTSR (EXTI->FTSR1)
# define EXTI_RTSR (EXTI->RTSR1)
#elif defined(EXTI_D1_BASE) /* STM32H7*/
# define EXTI_PR   (EXTI_D1->PR1)
# define EXTI_IMR  (EXTI_D1->IMR1)
# define EXTI_FTSR (EXTI->FTSR1)
# define EXTI_RTSR (EXTI->RTSR1)
#else
# define EXTI_PR   (EXTI->PR)
# define EXTI_IMR  (EXTI->IMR)
# define EXTI_FTSR (EXTI->FTSR)
# define EXTI_RTSR (EXTI->RTSR)
#endif

#if defined(IWDG_KR_KEY) && !defined(IWDG_KR)
# define IWDG_KR IWDG_KR_KEY
#endif

extern int Stm32ResetCause(void);
extern void SystemInit (void);

/* Allow readable values in the configurator, but avoid
 * name clash when STM libraries are used too.
 */
#ifndef USE_STD_PERIPHERAL_DRIVERS
# define ENABLE  1
# define DISABLE 0
#endif

#endif
