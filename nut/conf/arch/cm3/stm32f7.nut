--
--  * Copyright (C) 2015
--    by Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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

-- ARM CortexM7 Architecture
-- STMicroelectronics STM32F7 Family Devices
--
--
-- *****************************************************************************
-- STM32F7 Family
-- *****************************************************************************
--

nutarch_cm3_stm32f7 =
{

    --
    -- MCU Family
    --
    {
        name = "nutarch_cm3_stm32f7_family",
        brief = "MCU F7 Family",
        requires = { "HW_MCU_STM32F7" },
        options =
        {
            {
                macro = "MCU_CM_NO_BITBAND",
                brief = "No Bitbanding",
                description = "Selected family has no bitband support",
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_STM32F7",
                brief = "STM32F7 Devices",
                description = "Provide devices available in all STM32F7xx devices",
                default = 1,
                provides =
                {
                    "HW_RCC_STM32",
                    "HW_VOS_STM32",
                    "HW_OVERDRIVE_STM32",
                    "HW_HSI16_STM32F",
                    "HW_MCU_FPU",
                    "HW_FLASH_STM32F2_4",
                    "HW_GPIO_STM32V2",
                    "HW_BKPSRAM_STM32",
                    "HW_CRC32_STM32",
                    "HW_CAN1_STM32",
                    "HW_CAN2_STM32",
                    "DEV_IRQ_STM32",
                    "HW_DCMI_STM32",
                    "HW_DMA_STM32F2",
                    "HW_EXTI04_STM32",
                    "HW_EXTI95_STM32",
                    "HW_EXTI1510_STM32",
                    "HW_OTG_FS_STM32",
                    "HW_I2C1_STM32V2",
                    "HW_I2C2_STM32V2",
                    "HW_I2C3_STM32V2",
                    "HW_I2C4_STM32V2",
                    "HW_PVD_STM32",
                    "HW_SPI1_STM32",
                    "HW_SPI2_STM32",
                    "HW_SPI3_STM32",
                    "HW_SPI4_STM32",
                    "HW_SPI5_STM32",
                    "HW_SPI6_STM32",
                    "HW_USART1_STM32",
                    "HW_USART2_STM32",
                    "HW_USART3_STM32",
                    "HW_UART4_STM32",
                    "HW_UART5_STM32",
                    "HW_USART6_STM32",
                    "HW_UART7_STM32",
                    "HW_UART8_STM32",
                    "HW_RTC_STM32_V2",
                    "HW_TIM1_TIM9_TIM10_TIM11_STM32",
                    "HW_TIM2_32BIT_STM32",
                    "HW_TIM3_STM32",
                    "HW_TIM4_STM32",
                    "HW_TIM5_32BIT_STM32",
                    "HW_TIM4_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM8_TIM12_TIM13_COM_TIM14_STM32",
                    "HW_ADC_STM32",
                    "HW_ADC1_STM32F2_4",
                    "HW_WWDG_STM32",
                    "HW_EMAC_STM32",
                 },
                file = "include/cfg/arch.h"
             },
             {
                macro = "STM32F745xx",
                brief = "STM32F745 512/1024 kB",
                description = "Provides vendor header for STM32F745 512/1024 kB device.",
                requires = { "HW_MCU_STM32F745" },
                default = 1,
                provides =
                {
                },
                file = "include/cfg/arch.h",
             },
             {
                macro = "STM32F746xx",
                brief = "STM32F746 512/1024 kB",
                description = "Provides vendor header for STM32F745 512/1024 kB device.",
                requires = { "HW_MCU_STM32F746" },
                default = 1,
                provides =
                {
                    "HW_LTDc_STM32",
                },
                file = "include/cfg/arch.h",
             },
        }
    },
}
