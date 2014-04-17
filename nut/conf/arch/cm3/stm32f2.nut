--
--  * Copyright (C) 2011 by Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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
-- STMicroelectronics STM32L Family Devices
--
--

-- *****************************************************************************
-- STM32F2 Family
-- *****************************************************************************
--

nutarch_cm3_stm32f2 =
{

    --
    -- MCU Family
    --
    {
        name = "nutarch_cm3_stm32f2_family",
        brief = "MCU F2 Family",
        requires = { "HW_MCU_STM32F2XX" },
        sources = { "cm3/dev/stm/stm32f4_clk.c" },
        options =
        {
            {
                macro = "MCU_STM32F2",
                brief = "STM32F2",
                type = "integer",
                default = 1,
                provides =
                {
                    "HW_RCC_STM32",
                    "HW_FLASH_STM32F2_4",
                    "HW_GPIO_STM32V2",
                    "HW_CRC32_STM32",
                    "DEV_IRQ_STM32",
                    "HW_EXTI04_STM32",
                    "HW_EXTI95_STM32",
                    "HW_EXTI1510_STM32",
                    "HW_DMA1_STM32F2",
                    "HW_DMA2_STM32F2",
                    "HW_OTG_FS_STM32",
                    "HW_OTG_HS_STM32",
                    "HW_I2C1_STM32",
                    "HW_I2C2_STM32",
                    "HW_I2C3_STM32",
                    "HW_RNG_STM32",
                    "HW_SDIO_STM32",
                    "HW_SPI1_STM32",
                    "HW_SPI2_STM32",
                    "HW_SPI3_STM32",
                    "HW_USART1_STM32",
                    "HW_USART2_STM32",
                    "HW_USART3_STM32",
                    "HW_UART4_STM32",
                    "HW_UART5_STM32",
                    "HW_USART6_STM32",
                    "HW_CAN1_STM32",
                    "HW_CAN2_STM32",
                    "HW_RTC_STM32_V2",
                    "HW_TIM1_TIM9_TIM10_TIM11_STM32",
                    "HW_TIM2_32BIT_STM32",
                    "HW_TIM3_STM32",
                    "HW_TIM4_STM32",
                    "HW_TIM5_32BITT_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM8_TIM12_TIM13_TIM14_STM32",
                },
                file = "include/cfg/arch.h"
            }
        }
    },
    --
    -- STM32F2 MCU Classes
    --
    {
        name = "nutarch_cm3_stm32F2_class",
        brief = "STM32F2 Device Classes",
        requires = { "HW_MCU_STM32F2XX" },
        options =
        {
            {
                macro = "STM32F205xx",
                brief = "STM32F205",
                description = "STM32F207 w/o ETH/DCMI and Crypto.",
                requires = { "HW_MCU_STM32F205" },
                type = "integer",
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32F207xx",
                brief = "STM32F207",
                description = "STM32F207 with ETH/DCMI w/o Crypto.",
                requires = { "HW_MCU_STM32F207" },
                type = "integer",
                default = 1,
                provides =
                {
                    "HW_EMAC_STM32",
                    "HW_DCMI_STM32",
                },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32F215xx",
                brief = "STM32F215",
                description = "STM32F215 w/o Ethernet, with Crypto.",
                requires = { "HW_MCU_STM32F215" },
                type = "integer",
                default = 1,
                provides =
                {
                    "HW_HASH_RNG_STM32",
                    "HW_CRYP_STM32",
                },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32F217xx",
                brief = "STM32F217",
                description = "STM32F217 with Ethernet/DCMI and Crypto.",
                requires = { "HW_MCU_STM32F217" },
                type = "integer",
                default = 1,
                provides =
                {
                    "HW_EMAC_STM32",
                    "HW_DCMI_STM32",
                    "HW_HASH_RNG_STM32",
                    "HW_CRYP_STM32",
                },
                file = "include/cfg/arch.h"
            },
        }
    },
    {
        name = "nutarch_cm3_stm32F2_memory",
        brief = "STM32F2 Device Memory",
        requires = { "HW_MCU_STM32" },
        options =
        {
            {
                macro = "MCU_STM32F2XX",
                brief = "STM32F2xx memory",
                description = "Select your devices memory by the marked alphanumeric code on the chip:\n"..
                              "STM32F2xx>Y<zz where Y is one of the list below.\n\n"..
                              "B =  128 kbytes Flash\n"..
                              "C =  256 kbytes Flash\n"..
                              "E =  512 kbytes Flash\n"..
                              "F =  768 kbytes Flash\n"..
                              "G =  1024 kbytes Flash\n",

                requires = { "HW_MCU_STM32F2XX" },
                type = "enumerated",
--                choices = stm32_memory_f2xx,
                file = "include/cfg/arch.h"
            },
        }
    },
}
