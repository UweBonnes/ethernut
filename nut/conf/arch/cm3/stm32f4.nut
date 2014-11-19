--
--  * Copyright (C) 2011-13 by Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
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
stm32_memory_f401 = { "128", "256" }
stm32_memory_f40x = { "512", "1024" }
stm32_memory_f42x = { "1024", "2048" }

-- *****************************************************************************
-- STM32F4 Family
-- *****************************************************************************
--

nutarch_cm3_stm32f4 =
{

    --
    -- MCU Family
    --
    {
        name = "nutarch_cm3_stm32f4_family",
        brief = "MCU F4 Family",
        requires = { "HW_MCU_STM32F4XX" },
        sources = { "cm3/dev/stm/stm32f4_clk.c" },
        options =
        {
            {
                macro = "MCU_STM32F4",
                brief = "STM32F4",
                brief = "Devices available in all F4xx devices",
                type = "integer",
                default = 1,
                provides =
                {
                    "HW_RCC_STM32",
                    "HW_MCU_FPU",
                    "HW_FLASH_STM32F2_4",
                    "HW_GPIO_STM32V2",
                    "HW_CRC32_STM32",
                    "DEV_IRQ_STM32",
                    "HW_DMA1_STM32F2",
                    "HW_DMA2_STM32F2",
                    "HW_EXTI04_STM32",
                    "HW_EXTI95_STM32",
                    "HW_EXTI1510_STM32",
                    "HW_OTG_FS_STM32",
                    "HW_I2C1_STM32",
                    "HW_I2C2_STM32",
                    "HW_I2C3_STM32",
                    "HW_SPI1_STM32",
                    "HW_SPI2_STM32",
                    "HW_SPI3_STM32",
                    "HW_USART1_STM32",
                    "HW_USART2_STM32",
                    "HW_USART6_STM32",
                    "HW_RTC_STM32_V2",
                    "HW_TIM1_TIM9_TIM10_TIM11_STM32",
                    "HW_TIM2_32BIT_STM32",
                    "HW_TIM3_STM32",
                    "HW_TIM4_STM32",
                    "HW_TIM5_32BIT_STM32",
                    "HW_ADC_STM32",
                    "HW_ADC1_STM32F2_4",
                    "HW_WWDG_STM32",
                    "HW_SDIO_STM32",
                 },
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32F401xe",
                brief = "STM32F401",
                brief = "STM32F4 w/o CAN etc.",
                type = "integer",
                requires = { "HW_MCU_STM32F401" },
                default = 1,
                provides =
                {
                    "HW_SPI4_STM32",
                },
                file = "include/cfg/arch.h"
             },
             {
                macro = "STM32F405xx",
                brief = "STM32F405",
                brief = "STM32F4 basic device",
                type = "integer",
                requires = { "HW_MCU_STM32F405" },
                default = 1,
                provides =
                {
                    "HW_CAN1_STM32",
                    "HW_CAN2_STM32",
                    "HW_FSMC_STM32",
                    "HW_USART3_STM32",
                    "HW_UART4_STM32",
                    "HW_UART5_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM8_TIM12_TIM13_TIM14_STM32",
                    "HW_ADC2_STM32F2_4",
                    "HW_ADC3_STM32F2_4",
                    "HW_DAC1_STM32",
                    "HW_DAC2_STM32",
                    "HW_OTG_HS_STM32",
                },
                file = "include/cfg/arch.h"
             },
             {
                macro = "STM32F407xx",
                brief = "STM32F407",
                brief = "STM32F4 with ETH/DCMI, w/o Crypto/DMA2D",
                type = "integer",
                requires = { "HW_MCU_STM32F407" },
                default = 1,
                provides =
                {
                    "HW_CAN1_STM32",
                    "HW_CAN2_STM32",
                    "HW_ETH_STM32",
                    "HW_DCMI_STM32",
                    "HW_FSMC_STM32",
                    "HW_USART3_STM32",
                    "HW_UART4_STM32",
                    "HW_UART5_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM8_TIM12_TIM13_TIM14_STM32",
                    "HW_ADC2_STM32F2_4",
                    "HW_ADC3_STM32F2_4",
                    "HW_DAC1_STM32",
                    "HW_DAC2_STM32",
                    "HW_OTG_HS_STM32",
                },
                file = "include/cfg/arch.h"
             },
             {
                macro = "STM32F415xx",
                brief = "STM32F415",
                brief = "STM32F4 basic device with Crypto, w/o ETH/DCMI/DMA2D",
                type = "integer",
                requires = { "HW_MCU_STM32F415" },
                default = 1,
                provides =
                {
                    "HW_CAN1_STM32",
                    "HW_CAN2_STM32",
                    "HW_CRYP_STM32",
                    "HW_FSMC_STM32",
                    "HW_USART3_STM32",
                    "HW_UART4_STM32",
                    "HW_UART5_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM8_TIM12_TIM13_TIM14_STM32",
                    "HW_ADC2_STM32F2_4",
                    "HW_ADC3_STM32F2_4",
                    "HW_DAC1_STM32",
                    "HW_DAC2_STM32",
                    "HW_OTG_HS_STM32",
                },
                file = "include/cfg/arch.h"
             },
             {
                macro = "STM32F417xx",
                brief = "STM32F417",
                brief = "STM32F4 basic device with Crypto/ETH/DCMI, w/o DMA2D",
                type = "integer",
                requires = { "HW_MCU_STM32F417" },
                default = 1,
                provides =
                {
                    "HW_CAN1_STM32",
                    "HW_CAN2_STM32",
                    "HW_CRYP_STM32",
                    "HW_ETH_STM32",
                    "HW_DCMI_STM32",
                    "HW_FSMC_STM32",
                    "HW_USART3_STM32",
                    "HW_UART4_STM32",
                    "HW_UART5_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM8_TIM12_TIM13_TIM14_STM32",
                    "HW_ADC2_STM32F2_4",
                    "HW_ADC3_STM32F2_4",
                    "HW_DAC1_STM32",
                    "HW_DAC2_STM32",
                    "HW_OTG_HS_STM32",
                },
                file = "include/cfg/arch.h"
             },
             {
                macro = "STM32F427xx",
                brief = "STM32F427",
                brief = "STM32F4 extended device with ETH/DCMI/DMA2D w/o Crypto/LTDC",
                type = "integer",
                requires = { "HW_MCU_STM32F427" },
                default = 1,
                provides =
                {
                    "HW_CAN1_STM32",
                    "HW_CAN2_STM32",
                    "HW_ETH_STM32",
                    "HW_DCMI_STM32",
                    "HW_DMA2D_STM32",
                    "HW_FMC_STM32",
                    "HW_USART3_STM32",
                    "HW_UART4_STM32",
                    "HW_UART5_STM32",
                    "HW_UART7_STM32",
                    "HW_UART8_STM32",
                    "HW_SPI4_STM32",
                    "HW_SPI5_STM32",
                    "HW_SPI6_STM32",
                    "HW_SAI1_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM8_TIM12_TIM13_TIM14_STM32",
                    "HW_ADC2_STM32F2_4",
                    "HW_ADC3_STM32F2_4",
                    "HW_DAC1_STM32",
                    "HW_DAC2_STM32",
                    "HW_OTG_HS_STM32",
                },
                file = "include/cfg/arch.h"
             },
             {
                macro = "STM32F429xx",
                brief = "STM32F429",
                brief = "STM32F4 extended device with ETH/DCMI/DMA2D/LTDC w/o Crypto",
                type = "integer",
                requires = { "HW_MCU_STM32F429" },
                default = 1,
                provides =
                {
                    "HW_CAN1_STM32",
                    "HW_CAN2_STM32",
                    "HW_ETH_STM32",
                    "HW_DCMI_STM32",
                    "HW_DMA2D_STM32",
                    "HW_FMC_STM32",
                    "HW_LTDC_STM32",
                    "HW_USART3_STM32",
                    "HW_UART4_STM32",
                    "HW_UART5_STM32",
                    "HW_UART7_STM32",
                    "HW_UART8_STM32",
                    "HW_SPI4_STM32",
                    "HW_SPI5_STM32",
                    "HW_SPI6_STM32",
                    "HW_SAI1_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM8_TIM12_TIM13_TIM14_STM32",
                    "HW_ADC2_STM32F2_4",
                    "HW_ADC3_STM32F2_4",
                    "HW_DAC1_STM32",
                    "HW_DAC2_STM32",
                    "HW_OTG_HS_STM32",
                },
                file = "include/cfg/arch.h"
             },
             {
                macro = "STM32F437xx",
                brief = "STM32F437",
                brief = "STM32F4 extended device with Crypto/ETH/DCMI/DMA2D w/o LTDC",
                type = "integer",
                requires = { "HW_MCU_STM32F437" },
                default = 1,
                provides =
                {
                    "HW_CAN1_STM32",
                    "HW_CAN2_STM32",
                    "HW_CRYP_STM32",
                    "HW_ETH_STM32",
                    "HW_DCMI_STM32",
                    "HW_DMA2D_STM32",
                    "HW_FMC_STM32",
                    "HW_USART3_STM32",
                    "HW_UART4_STM32",
                    "HW_UART5_STM32",
                    "HW_UART7_STM32",
                    "HW_UART8_STM32",
                    "HW_SPI4_STM32",
                    "HW_SPI5_STM32",
                    "HW_SPI6_STM32",
                    "HW_SAI1_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM8_TIM12_TIM13_TIM14_STM32",
                    "HW_ADC2_STM32F2_4",
                    "HW_ADC3_STM32F2_4",
                    "HW_DAC1_STM32",
                    "HW_DAC2_STM32",
                    "HW_OTG_HS_STM32",
                    "HW_EMAC_STM32",
                },
                file = "include/cfg/arch.h"
             },
             {
                macro = "STM32F439xx",
                brief = "STM32F439",
                brief = "STM32F4 extended device with Crypto/ETH/DCMI/DMA2D/LTDC.",
                type = "integer",
                requires = { "HW_MCU_STM32F439" },
                default = 1,
                provides =
                {
                    "HW_CAN1_STM32",
                    "HW_CAN2_STM32",
                    "HW_CRYP_STM32",
                    "HW_ETH_STM32",
                    "HW_DCMI_STM32",
                    "HW_DMA2D_STM32",
                    "HW_FMC_STM32",
                    "HW_LTDC_STM32",
                    "HW_USART3_STM32",
                    "HW_UART4_STM32",
                    "HW_UART5_STM32",
                    "HW_UART7_STM32",
                    "HW_UART8_STM32",
                    "HW_SPI4_STM32",
                    "HW_SPI5_STM32",
                    "HW_SPI6_STM32",
                    "HW_SAI1_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM8_TIM12_TIM13_TIM14_STM32",
                    "HW_ADC2_STM32F2_4",
                    "HW_ADC3_STM32F2_4",
                    "HW_DAC1_STM32",
                    "HW_DAC2_STM32",
                    "HW_OTG_HS_STM32",
                },
                file = "include/cfg/arch.h"
             },
        }
    },
    --
    -- STM32F4 MCU Memory
    --
    {
        name = "nutarch_cm3_stm32F4_memory",
        brief = "STM32F4 Device Memory",
        requires = { "HW_MCU_STM32" },
        options =
        {
            {
                macro = "MCU_STM32F401",
                brief = "STM32F4xx memory",
                description = "Select your devices memory by the marked alphanumeric code on the chip:\n"..
                              "STM32F401>Y<zz where Y is one of the list below.\n\n"..
                              "B =  128 kbytes Flash\n"..
                              "C =  256 kbytes Flash\n",
                requires = { "HW_MCU_STM32F401" },
                type = "enumerated",
                choices = stm32_memory_f401,
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_STM32F40X",
                brief = "STM32F4xx memory",
                description = "Select your devices memory by the marked alphanumeric code on the chip:\n"..
                              "STM32F4xx>Y<zz where Y is one of the list below.\n\n"..
                              "E =  512 kbytes Flash\n"..
                              "G =  1024 kbytes Flash\n",
                requires = { "HW_MCU_STM32F40X" },
                type = "enumerated",
                choices = stm32_memory_f40x,
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_STM32F42X",
                brief = "STM32F4xx memory",
                description = "Select your devices memory by the marked alphanumeric code on the chip:\n"..
                              "STM32F4xx>Y<zz where Y is one of the list below.\n\n"..
                              "G =  1024 kbytes Flash\n"..
                              "I =  2048 kbytes Flash\n",
                requires = { "HW_MCU_STM32F42X" },
                type = "enumerated",
                choices = stm32_memory_f42x,
                file = "include/cfg/arch.h"
            },
        }
    },
}
