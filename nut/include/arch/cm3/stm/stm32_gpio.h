/*
 * Copyright (C) 2013 Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
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
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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

/*
 * \verbatim
 * $Id$
 * \endverbatim
 */

/* STM32 Remapping defines for L1/F2/3/4*/
#define GPIO_Mode_IN 0
#define GPIO_Mode_OUT 1
#define GPIO_Mode_AF  2
#define GPIO_Mode_AN  3

#define GPIO_PuPd_NOPULL 0
#define GPIO_PuPd_UP     1
#define GPIO_PuPd_DOWN   2

#if !defined(MCU_STM32F1)
#define GPIO_AF_0 0
#define GPIO_AF_1 1
#define GPIO_AF_2 2
#define GPIO_AF_3 3
#define GPIO_AF_4 4
#define GPIO_AF_5 5
#define GPIO_AF_6 6
#define GPIO_AF_7 7
#define GPIO_AF_8 8
#define GPIO_AF_9 9
#define GPIO_AF_A 10
#define GPIO_AF_B 11
#define GPIO_AF_C 12
#define GPIO_AF_D 13
#define GPIO_AF_E 14
#define GPIO_AF_F 15

#define GPIO_AF_RTC_50Hz GPIO_AF_0
#define GPIO_AF_MCO      GPIO_AF_0
#define GPIO_AF_TAMPER   GPIO_AF_0
#define GPIO_AF_SWJ      GPIO_AF_0
#define GPIO_AF_TRACE    GPIO_AF_0

#define GPIO_AF_TIM1     GPIO_AF_1
#define GPIO_AF_TIM2     GPIO_AF_1

#define GPIO_AF_TIM3     GPIO_AF_2
#define GPIO_AF_TIM4     GPIO_AF_2
#define GPIO_AF_TIM5     GPIO_AF_2

#define GPIO_AF_TIM8     GPIO_AF_3
#define GPIO_AF_TIM9     GPIO_AF_3
#define GPIO_AF_TIM10    GPIO_AF_3
#define GPIO_AF_TIM11    GPIO_AF_3

#define GPIO_AF_I2C1     GPIO_AF_4
#define GPIO_AF_I2C2     GPIO_AF_4
#define GPIO_AF_I2C3     GPIO_AF_4

#define GPIO_AF_SPI1     GPIO_AF_5
#define GPIO_AF_SPI2     GPIO_AF_5
#define GPIO_AF_SPI4     GPIO_AF_5
#define GPIO_AF_SPI5     GPIO_AF_5
#define GPIO_AF_SPI6     GPIO_AF_5

#define GPIO_AF_SPI3     GPIO_AF_6

#define GPIO_AF_USART1   GPIO_AF_7
#define GPIO_AF_USART2   GPIO_AF_7
#define GPIO_AF_USART3   GPIO_AF_7
#define GPIO_AF_I2S3ext  GPIO_AF_7

#define GPIO_AF_UART4    GPIO_AF_8
#define GPIO_AF_UART5    GPIO_AF_8
#define GPIO_AF_USART6   GPIO_AF_8
#define GPIO_AF_UART7    GPIO_AF_8
#define GPIO_AF_UART8    GPIO_AF_8

#define GPIO_AF_CAN1     GPIO_AF_9
#define GPIO_AF_CAN2     GPIO_AF_9
#define GPIO_AF_TIM12    GPIO_AF_9
#define GPIO_AF_TIM13    GPIO_AF_9
#define GPIO_AF_TIM14    GPIO_AF_9

#define GPIO_AF_OTG_FS   GPIO_AF_A
#define GPIO_AF_OTG_HS   GPIO_AF_A

#define GPIO_AF_ETH      GPIO_AF_B

#define GPIO_AF_FSMC     GPIO_AF_C
#define GPIO_AF_OTG_HS_FS GPIO_AF_C
#define GPIO_AF_SDIO     GPIO_AF_C

#define GPIO_AF_DCMI     GPIO_AF_D

#define GPIO_AF_EVENTOUT GPIO_AD_F

void GPIO_PinAFConfig(nutgpio_port_t GPIOx, nutgpio_pin_t GPIO_PinSource, uint8_t GPIO_AF);
#endif
