--
--  * Copyright (C) 2015
--               Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions
-- are met:
--
-- 1. Redistributions of source code must retain the above copyright
--    notice, this list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright
--    notice, this list of conditions and the following disclaimer in the
--    documentation and/or other materials provided with the distribution.
-- 3. Neither the name of the copyright holders nor the names of
--    contributors may be used to endorse or promote products derived
--    from this software without specific prior written permission.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
-- ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
-- LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
-- FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
-- COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
-- INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
-- BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
-- OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
-- AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
-- OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
-- THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
-- SUCH DAMAGE.
--
-- For additional information see http://www.ethernut.de/
--

-- ARM CortexM3 Architecture
-- STMicroelectronics STM32L0 Family Devices
--
--

-- *****************************************************************************
-- STM32L0 Family
-- *****************************************************************************
--

nutarch_cm3_stm32l0 =
{

    --
    -- MCU Family
    --
    {
        name = "nutarch_cm3_stm32L0_family",
        brief = "MCU L0 Family",
        requires = { "HW_MCU_STM32L0" },
        options =
        {
            {
                macro = "MCU_STM32L0",
                brief = "STM32L0 common devices",
                type = "integer",
                default = 1,
                provides =
                {
-- Todo: FLASH_WRPROT
--                    "HW_FLASH_STM32L1",
                    "HW_RTC_STM32_V2",
                    "HW_FLASH_STM32L0",
                    "HW_EEPROM_STM32L0",
                    "HW_GPIO_STM32V2",
                    "HW_CRC32_STM32",
                    "DEV_IRQ_STM32",
                    "HW_DMA1_STM32L0",
                    "HW_EXTI01_STM32",
                    "HW_EXTI23_STM32",
                    "HW_EXTI415_STM32",
                    "HW_ADC1_COMP_STM32",
                    "HW_I2C1_STM32V2",
                    "HW_I2C2_STM32V2",
                    "HW_PVD_STM32",
                    "HW_SPI1_STM32",
                    "HW_SPI2_STM32",
                    "HW_TIM2_32BIT_STM32",
                    "HW_TIM21_STM32",
                    "HW_TIM21_STM32",
                    "HW_USART1_STM32",
                    "HW_USART2_STM32",
                },
                file = "include/cfg/arch.h",
            },
            {
                macro = "MCU_CM_NO_BITBAND",
                brief = "No Bitbanding",
                description = "Selected family has no bitband support",
                type = "integer",
                default = 1,
                file = "include/cfg/arch.h"
            },
-- Use macro names so that ST vendor defines are produced
            {
                macro = "STM32L051xx",
                brief = "STM32L051 32/64 kB.",
                description = "STM32L051 32/64 kB.",
                type = "integer",
                default = 1,
                requires = { "HW_MCU_STM32L051" },
                provides =
                {
                    "HW_RCC_STM32",
                    "HW_TIM6_STM32",
                    "HW_LPUART1_STM32",
                },
                file = "include/cfg/arch.h",
            },
            {
                macro = "STM32L052xx",
                brief = "STM32L052 32/64 kB.",
                description = "STM32L052 32/64 kB.",
                type = "integer",
                default = 1,
                requires = { "HW_MCU_STM32L052" },
                provides =
                {
                    "HW_RCC_CRS_STM32",
                    "HW_TSC_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_RNG_LPUART1_STM32",
                    "HW_USB_STM32",
                },
                file = "include/cfg/arch.h",
            },
            {
                macro = "STM32L053xx",
                brief = "STM32L053 32/64 kb",
                description = "STM32L053 32/64 kb.",
                type = "integer",
                default = 1,
                requires = { "HW_MCU_STM32L053" },
                provides = {
                   "HW_RCC_CRS_STM32",
                    "HW_TSC_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_RNG_LPUART1_STM32",
                    "HW_LCD_STM32",
                    "HW_USB_STM32",
                 },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32L061xx",
                brief = "STM32L061 32/64 kB.",
                description = "STM32L061 32/64 kB.",
                type = "integer",
                default = 1,
                requires = { "HW_MCU_STM32L061" },
                provides =
                {
                    "HW_RCC_STM32",
                    "HW_TIM6_STM32",
                    "HW_AES_LPUART1_STM32",
                },
                file = "include/cfg/arch.h",
            },
            {
                macro = "STM32L062xx",
                brief = "STM32L062 32/64 kB.",
                description = "STM32L062 32/64 kB.",
                type = "integer",
                default = 1,
                requires = { "HW_MCU_STM32L062" },
                provides =
                {
                    "HW_RCC_CRS_STM32",
                    "HW_TSC_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_AES_RNG_LPUART1_STM32",
                    "HW_USB_STM32",
                },
                file = "include/cfg/arch.h",
            },
            {
                macro = "STM32L063xx",
                brief = "STM32L063 32/64 kb",
                description = "STM32L063 32/64 kb.",
                type = "integer",
                default = 1,
                requires = { "HW_MCU_STM32L063" },
                provides = {
                   "HW_RCC_CRS_STM32",
                    "HW_TSC_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_AES_RNG_LPUART1_STM32",
                    "HW_LCD_STM32",
                    "HW_USB_STM32",
                 },
                file = "include/cfg/arch.h"
            },
        }
    },
}
