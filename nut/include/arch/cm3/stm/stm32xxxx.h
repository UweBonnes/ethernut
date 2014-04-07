/*
 * Copyright (C) 2012-2013 Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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
#include <arch/cm3/stm/vendor/stm32f0xx.h>
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
#elif defined(STM32F30X)
#include <arch/cm3/stm/vendor/stm32f30x.h>
#elif defined(STM32F37X)
#include <arch/cm3/stm/vendor/stm32f37x.h>
#elif defined(MCU_STM32F4)
#define __STM32F4xx_CMSIS_DEVICE_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __STM32F4xx_CMSIS_DEVICE_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __STM32F4xx_CMSIS_DEVICE_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __STM32F4xx_CMSIS_DEVICE_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32F4xx_CMSIS_DEVICE_VERSION        ((__CMSIS_DEVICE_VERSION_MAIN     << 24)\
                                      |(__CMSIS_DEVICE_HAL_VERSION_SUB1 << 16)\
                                      |(__CMSIS_DEVICE_HAL_VERSION_SUB2 << 8 )\
                                      |(__CMSIS_DEVICE_HAL_VERSION_RC))

#if defined(MCU_STM32F401)
#include <arch/cm3/stm/vendor/stm32f401xe.h>
#elif defined(MCU_STM32F405)
#include <arch/cm3/stm/vendor/stm32f405xx.h>
#elif defined(MCU_STM32F407)
#include <arch/cm3/stm/vendor/stm32f407xx.h>
#elif defined(MCU_STM32F415)
#include <arch/cm3/stm/vendor/stm32f415xx.h>
#elif defined(MCU_STM32F417)
#include <arch/cm3/stm/vendor/stm32f417xx.h>
#elif defined(MCU_STM32F427)
#include <arch/cm3/stm/vendor/stm32f427xx.h>
#elif defined(MCU_STM32F429)
#include <arch/cm3/stm/vendor/stm32f429xx.h>
#elif defined(MCU_STM32F437)
#include <arch/cm3/stm/vendor/stm32f437xx.h>
#elif defined(MCU_STM32F439)
#include <arch/cm3/stm/vendor/stm32f439xx.h>
#else
#warning "Unknown STM32F4 family"
#endif
#else
#warning "Unknown STM32 family"
#endif
