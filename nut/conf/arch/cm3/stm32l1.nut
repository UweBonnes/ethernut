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

stm32_memory_l1xx = { "32" , "64" , "128" , "256" , "384" , "512"}

stm32l1_device_class = {
                     "STM32L1XX_MD" ,"STM32L1XX_MDP",
                     "STM32L1XX_HD", "STM32L1XX_XL" }

-- *****************************************************************************
-- STM32L1 Family
-- *****************************************************************************
--

nutarch_cm3_stm32l1 =
{

    --
    -- MCU Family
    --
    {
        name = "nutarch_cm3_stm32L1_family",
        brief = "MCU L1 Family",
        requires = { "HW_MCU_STM32L1XX" },
        sources = { "cm3/dev/stm/stm32l1_clk.c" },
        options =
        {
            {
                macro = "MCU_STM32L1",
                brief = "STM32L1",
                type = "integer",
                default = 1,
                requires = { "HW_MCU_CM3" },
                provides =
                {
                    "HW_RCC_STM32",
                    "HW_FLASH_STM32L1",
                    "HW_EEPROM_STM32L1",
                    "HW_GPIO_STM32V2",
                    "HW_CRC32_STM32",
                    "DEV_IRQ_STM32",
                    "HW_DMA1_STM32F1",
                    "HW_EXTI04_STM32",
                    "HW_EXTI95_STM32",
                    "HW_EXTI1510_STM32",
                    "HW_I2C1_STM32",
                    "HW_I2C2_STM32",
                    "HW_PVD_STM32",
                    "HW_SPI1_STM32",
                    "HW_SPI2_STM32",
                    "HW_USART1_STM32",
                    "HW_USART2_STM32",
                    "HW_USART3_STM32",
                    "HW_RTC_STM32_V2",
                    "HW_TIM2_STM32",
                    "HW_TIM3_STM32",
                    "HW_TIM4_STM32",
                    "HW_TIM6_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM9_STM32",
                    "HW_STM32_TIM10",
                    "HW_STM32_TIM11",
--                    "HW_USB_STM32",
                },
                file = "include/cfg/arch.h"
            }
        }
    },
    --
    -- STM32L1 MCU Classes
    --
    {
        name = "nutarch_cm3_stm32l1_class",
        brief = "STM32L1 Device Classes",
        requires = { "HW_MCU_STM32L1XX" },
        options =
        {
            {
                macro = "STM32L1XX_MD",
                brief = "STM32L1 MD Series",
                description = "STM32L1 Medium Density devices.",
                type = "integer",
                default = 1,
                requires = { "STM32L1XX_MD" },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32L1XX_MDP",
                brief = "STM32L1 MDP Series",
                description = "STM32L1 Medium Density devices.",
                type = "integer",
                default = 1,
                requires = { "STM32L1XX_MDP" },
                provides =
                {
                         "HW_TIM5_STM32",
                         "HW_SPI3_STM32",
                         "HW_OPAMP1_STM32",
                         "HW_OPAMP2_STM32",
                },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32L1XX_HD",
                brief = "STM32L1 HD Series",
                description = "STM32L1 Medium Density devices.",
                type = "integer",
                default = 1,
                requires = { "STM32L1XX_HD" },
                provides =
                {
                         "HW_UART4_STM32",
                         "HW_UART5_STM32",
                         "HW_SPI3_STM32",
                         "HW_SDIO_STM32",
                         "HW_TIM5_STM32",
                         "HW_TIM7_STM32",
                         "HW_OPAMP1_STM32",
                         "HW_OPAMP2_STM32",
                         "HW_OPAMP3_STM32",
                         "HW_FSMC_STM32",
                },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32L1XX_XL",
                brief = "STM32L1 XL Series",
                description = "STM32L1 XL-Density devices.",
                type = "integer",
                default = 1,
                requires = { "STM32L1XX_XL" },
                provides =
                {
                         "HW_UART4_STM32",
                         "HW_UART5_STM32",
                         "HW_SPI3_STM32",
                         "HW_TIM5_STM32",
                         "HW_TIM7_STM32",
                         "HW_OPAMP1_STM32",
                         "HW_OPAMP2_STM32",
                         "HW_OPAMP3_STM32",
                },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32L152",
                brief = "STM32L152 Series",
                description = "STM32L152 LCD devices.",
                requires = { "HW_MCU_STM32L152" },
                flavor = "booldata",
                provides =
                {
                         "HW_LCD_STM32",
                },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32L162",
                brief = "STM32L152 Series",
                description = "STM32L162 LCD with Crypto devices.",
                requires = { "HW_MCU_STM32L152" },
                flavor = "booldata",
                provides =
                {
                         "HW_LCD_STM32",
                },
                file = "include/cfg/arch.h"
            },
        }
    },
    {
        name = "nutarch_cm3_stm32l1_memory",
        brief = "STM32L1 Device Memory",
        requires = { "HW_MCU_STM32" },
        options =
        {
            {
                macro = "MCU_STM32L15X",
                brief = "STM32L1xx memory",
                description = "Select your devices memory by the marked alphanumeric code on the chip:\n"..
                              "STM32L1xx>Y<zz where Y is one of the list below.\n\n"..
                              "6 =   32 kbytes Flash\n"..
                              "8 =   64 kbytes Flash\n"..
                              "B =  128 kbytes Flash\n",
                              "C =  256 kbytes Flash\n",
                              "D =  384 kbytes Flash\n",

                requires = { "HW_MCU_STM32L1XX" },
                type = "enumerated",
                choices = stm32_memory_l1xx,
                file = "include/cfg/arch.h"
            },
        }
    },
}
