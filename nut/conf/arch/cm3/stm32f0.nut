--
--  * Copyright (C) 2014 by Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
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
-- STMicroelectronics STM32F0 Family Devices
--
-- $Id$
--
--


--
-- ********************************************************************************
-- STM32F1 Family
-- ********************************************************************************
--

nutarch_cm3_stm32f0 =
{

    --
    -- MCU Family
    --
    {
        name = "nutarch_cm3_stm32f0_family",
        brief = "MCU F0 Family",
        sources = { "cm3/dev/stm/stm32f30_clk.c"},
        options =
        {
            {
                macro = "MCU_STM32F0",
                brief = "STM32F0",
                type = "integer",
                default = 1,
                provides = {
                    "HW_RCC_STM32",
                    "HW_RTC_STM32_V2",
                    "HW_FLASH_STM32F1_3",
                    "HW_GPIO_STM32V2",
                    "HW_CRC32_STM32V2",
                    "DEV_IRQ_STM32",
                    "HW_EXTI01_STM32",
                    "HW_EXTI23_STM32",
                    "HW_EXTI415_STM32",
                    "HW_WWDG_STM32",
--                    "HW_DMA1_STM32F1",
--                    "HW_I2C1_STM32",
                    "HW_SPI1_STM32",
                    "HW_USART1_STM32",
                    "HW_TIM1_STM32",
                    "HW_TIM3_STM32",
                    "HW_TIM14_STM32",
                    "HW_TIM16_STM32",
                    "HW_TIM17_STM32",
                },
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_CM_NO_BITBAND",
                brief = "No Bitbanding",
                description = "Selected family has no bitband support",
                type = "integer",
                default = 1,
                file = "include/cfg/arch.h"
             },
         }
    },

    --
    -- STM32F0 MCU Classes
    --
    {
        name = "nutarch_cm3_stm32f0_class",
        brief = "STM32F0 Device Classes",
        options =
        {
            {
                macro = "STM32F030",
                brief = "STM32F030",
                description = "STM32F030",
                flavor = "integer",
                default = 1,
                requires = { "HW_MCU_STM32F030" };
                provides = {
                    "HW_USART2_STM32",
--                    "HW_I2C2_STM32",
                    "HW_SPI2_STM32",
                    "HW_TIM15_STM32",
                   },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32F031",
                brief = "STM32F031",
                description = "STM32F031.",
                flavor = "integer",
                default = 1,
                requires = { "HW_MCU_STM32F031" };
                provides = {
                    "HW_PVD_STM32",
                    "HW_TIM2_32BIT_STM32",
                     },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32F042",
                brief = "STM32F042",
                description = "STM32F042.",
                flavor = "integer",
                default = 1,
                requires = { "HW_MCU_STM32F042" };
                provides = {
                    "HW_CEC_CAN_STM32",
                    "HW_PVD_VDDIO2_STM32",
                    "HW_SPI2_STM32",
                    "HW_TIM2_32BIT_STM32",
                    "HW_TSC_STM32",
                    "HW_USART2_STM32",
                    "HW_USB_STM32",
                    },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32F051",
                brief = "STM32F051",
                description = "STM32F051.",
                flavor = "integer",
                default = 1,
                requires = { "HW_MCU_STM32F051" };
                provides = {
                    "HW_ADC1_COMP_STM32",
                    "HW_CEC_STM32",
--                    "HW_I2C2_STM32",
                    "HW_PVD_STM32",
                    "HW_TSC_STM32",
                    "HW_SPI2_STM32",
                    "HW_TIM2_32BIT_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM15_STM32",
                    "HW_USART2_STM32",
                     },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32F072",
                brief = "STM32F072",
                description = "STM32F072.",
                flavor = "integer",
                default = 1,
                requires = { "HW_MCU_STM32F072" };
                provides = {
                    "HW_ADC1_COMP_STM32",
                    "HW_CEC_CAN_STM32",
--                    "HW_I2C2_STM32",
                    "HW_PVD_VDDIO2_STM32",
                    "HW_SPI2_STM32",
                    "HW_TIM2_32BIT_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM15_STM32",
                    "HW_TSC_STM32",
                    "HW_USART2_STM32",
                    "HW_USART3_4_STM32",
                    },
                file = "include/cfg/arch.h"
            },
        },
    },
}
