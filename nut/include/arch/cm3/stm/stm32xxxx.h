/*
 * Copyright (C) 2012-2015 Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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
#if defined(MCU_STM32F0)
# if   defined(STM32F030x6)
#  include <arch/cm3/stm/vendor/stm32f030x6.h>
# elif defined(STM32F030x8)
#  include <arch/cm3/stm/vendor/stm32f030x8.h>
# elif defined(STM32F030xC)
#  include <arch/cm3/stm/vendor/stm32f030xC.h>
# elif defined(STM32F031x6)
#  include <arch/cm3/stm/vendor/stm32f031x6.h>
# elif defined(STM32F038xx)
#  include <arch/cm3/stm/vendor/stm32f031x6.h>
# elif defined(STM32F042x6)
#  include <arch/cm3/stm/vendor/stm32f042x6.h>
# elif defined(STM32F048xx)
#  include <arch/cm3/stm/vendor/stm32f048xx.h>
# elif defined(STM32F051x8)
#  include <arch/cm3/stm/vendor/stm32f051x8.h>
# elif defined(STM32F058xx)
#  include <arch/cm3/stm/vendor/stm32f058xx.h>
# elif defined(STM32F070x6)
#  include <arch/cm3/stm/vendor/stm32f070x6.h>
# elif defined(STM32F070x8)
#  include <arch/cm3/stm/vendor/stm32f070x8.h>
# elif defined(STM32F070xB)
#  include <arch/cm3/stm/vendor/stm32f070xb.h>
# elif defined(STM32F071xB)
#  include <arch/cm3/stm/vendor/stm32f071xb.h>
# elif defined(STM32F072xB)
#  include <arch/cm3/stm/vendor/stm32f072xb.h>
# elif defined(STM32F078xx)
#  include <arch/cm3/stm/vendor/stm32f078xx.h>
# elif defined(STM32F091xC)
#  include <arch/cm3/stm/vendor/stm32f091xc.h>
# elif defined(STM32F098xx)
#  include <arch/cm3/stm/vendor/stm32f098xx.h>
# else
#  warning Unhandled STM32F0 device
# endif
#elif defined(MCU_STM32F1)
#include <arch/cm3/stm/vendor/stm32f10x.h>
#elif defined(MCU_STM32L1)
#include <arch/cm3/stm/vendor/stm32l1xx.h>
#elif defined(MCU_STM32F2)
/**
  * @brief CMSIS Device version number V2.0.1
  */
#define __STM32F2xx_CMSIS_DEVICE_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __STM32F2xx_CMSIS_DEVICE_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __STM32F2xx_CMSIS_DEVICE_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __STM32F2xx_CMSIS_DEVICE_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32F2xx_CMSIS_DEVICE_VERSION        ((__CMSIS_DEVICE_VERSION_MAIN     << 24)\
                                      |(__CMSIS_DEVICE_HAL_VERSION_SUB1 << 16)\
                                      |(__CMSIS_DEVICE_HAL_VERSION_SUB2 << 8 )\
                                      |(__CMSIS_DEVICE_HAL_VERSION_RC))
#if defined(STM32F205xx)
#include <arch/cm3/stm/vendor/stm32f205xx.h>
#elif defined(STM32F215xx)
#include  <arch/cm3/stm/vendor/stm32f215xx.h>
#elif defined(STM32F207xx)
#include  <arch/cm3/stm/vendor/stm32f207xx.h>
#elif defined(STM32F217xx)
#include  <arch/cm3/stm/vendor/stm32f217xx.h>
#else
#warning "Unknown STM32F2 family"
#endif
#elif defined(MCU_STM32F3)
# if   defined(STM32F301x8)
#  include <arch/cm3/stm/vendor/stm32f301x8.h>
# elif defined(STM32F302x8)
#  include <arch/cm3/stm/vendor/stm32f302x8.h>
# elif defined(STM32F302xC)
#  include <arch/cm3/stm/vendor/stm32f302xc.h>
# elif defined(STM32F302xE)
#  include <arch/cm3/stm/vendor/stm32f302xe.h>
# elif defined(STM32F303x8)
#  include <arch/cm3/stm/vendor/stm32f303x8.h>
# elif defined(STM32F303xC)
#  include <arch/cm3/stm/vendor/stm32f303xc.h>
# elif defined(STM32F303xE)
#  include <arch/cm3/stm/vendor/stm32f303xe.h>
# elif defined(STM32F318xx)
#  include <arch/cm3/stm/vendor/stm32f318xx.h>
# elif defined(STM32F328xx)
#  include <arch/cm3/stm/vendor/stm32f328xx.h>
# elif defined(STM32F334x8)
#  include <arch/cm3/stm/vendor/stm32f334x8.h>
# elif defined(STM32F358xx)
#  include <arch/cm3/stm/vendor/stm32f358xx.h>
# elif defined(STM32F373xC)
#  include <arch/cm3/stm/vendor/stm32f373xc.h>
# elif defined(STM32F378xx)
#  include <arch/cm3/stm/vendor/stm32f378xx.h>
# elif defined(STM32F379xx)
#  include <arch/cm3/stm/vendor/stm32f398xx.h>
# else
#  warning "Unhandled STM32F3 device"
# end
# endif
#elif defined(MCU_STM32F4)
# if defined(STM32F401xC)
#  include <arch/cm3/stm/vendor/stm32f401xc.h>
# elif defined(STM32F401xE)
#  include <arch/cm3/stm/vendor/stm32f401xe.h>
# elif defined(STM32F405xx)
#  include <arch/cm3/stm/vendor/stm32f405xx.h>
# elif defined(STM32F407xx)
#  include <arch/cm3/stm/vendor/stm32f407xx.h>
# elif defined(STM32F411xE)
#  include <arch/cm3/stm/vendor/stm32f411xe.h>
# elif defined(STM32F415xx)
#  include <arch/cm3/stm/vendor/stm32f415xx.h>
# elif defined(STM32F417xx)
#  include <arch/cm3/stm/vendor/stm32f417xx.h>
# elif defined(STM32F427xx)
#  include <arch/cm3/stm/vendor/stm32f427xx.h>
# elif defined(STM32F429xx)
#  include <arch/cm3/stm/vendor/stm32f429xx.h>
# elif defined(STM32F437xx)
#  include <arch/cm3/stm/vendor/stm32f437xx.h>
# elif defined(STM32F439xx)
#  include <arch/cm3/stm/vendor/stm32f439xx.h>
# else
#  warning "Unknown STM32F4 family"
# endif
#else
#warning "Unknown STM32 family"
#endif

/* Equalize names in a common place. Even recent CUBE uses "random" names.*/
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
