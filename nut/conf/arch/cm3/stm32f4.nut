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
        requires = { "LICENSE_MCD_ST_LIBERTY", "LICENSE_ST_GUIDANCE_ONLY", "HW_MCU_STM32F4XX" },
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
                    "HW_PLL_STM32F4",
                    "HW_RCC_STM32",
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
                macro = "MCU_STM32F40X_41X",
                brief = "STM32F40x",
                brief = "Devices available in all F40xand F41x additional to F4XX common devices",
                type = "integer",
                default = 1,
                provides =
                {
                    "HW_USART3_STM32",
                    "HW_UART4_STM32",
                    "HW_UART5_STM32",
                    "HW_CAN1_STM32",
                    "HW_CAN2_STM32",
                    "HW_TIM6_DAC_STM32",
                    "HW_TIM7_STM32",
                    "HW_TIM8_TIM12_TIM13_TIM14_STM32",
                    "HW_ADC2_STM32F2_4",
                    "HW_ADC3_STM32F2_4",
                    "HW_DAC1_STM32",
                    "HW_DAC2_STM32",
                    "HW_OTG_HS_STM32",
                },
            },
            {
                macro = "STM32F401xx",
                brief = "STM32F401",
                brief = "Include needed for STM32F401",
                type = "integer",
                requires = { "STM32F401xx" },
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32F40_41xxx",
                brief = "STM32F40x/41x",
                brief = "Include needed for STM32F405/407/415/417",
                type = "integer",
                requires = { "STM32F40_41xxx" },
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32F427_437xx",
                brief = "STM32F427/437",
                brief = "Include needed for STM32F427/438",
                type = "integer",
                requires = { "STM32F427_437xx" },
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "STM32F429_439xx",
                brief = "STM32F429/439",
                brief = "Include needed for STM32F429/439",
                type = "integer",
                requires = { "STM32F429_439xx" },
                default = 1,
                file = "include/cfg/arch.h"
            }
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
    --
    -- STM32F4 PLL Configuration
    --
    {
        name = "nutarch_cm3_stm32f4_pll",
        brief = "STM32F4 PLL Setup",
        sources =
        {
            "cm3/dev/stm/system_stm32.c",
            "cm3/dev/stm/stm32f4_clk.c"
        },
        requires = { "HW_MCU_STM32", "TOOL_CC_CM3", "TOOL_GCC" },
        options =
        {
            {
                macro = "SYSCLK_SOURCE",
                brief = "SYSCLK Source",
                description = "Select where SYSCLK should get its clock from.\n\n"..
                              "SYSCLK_HSI is the internal 8MHz clock.\n"..
                              "SYSCLK_PLL is the internal PLL output. Select the source for the PLL in the next option.\n"..
                              "SYSCLK_HSE is the external oscillator or crystal input.\n",
                requires = { "HW_PLL_STM32F4" },
                type = "enumerated",
                choices = { "SYSCLK_HSI", "SYSCLK_PLL", "SYSCLK_HSE" },
                file = "include/cfg/clock.h"
            },
            {
                macro = "HSE_BYPASS",
                brief = "HSE from external source",
                description = "Use the clock signal applied to OSC_IN.",
                requires = { "HW_PLL_STM32F1" },
                flavor = "boolean",
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLLCLK_SOURCE",
                brief = "PLL Clock Source",
                description = "Select where the PLL should get its clock from.\n\n"..
                              "SYSCLK_HSI is the internal 8MHz clock. PLL is fed with SYSCLK_HSI/2.\n"..
                              "SYSCLK_HSE is the external oscillator or crystal input.\n",
                requires = { "HW_PLL_STM32F4" },
                type = "enumerated",
                choices = { "PLLCLK_HSI", "PLLCLK_HSE" },
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLL_CLOCK_DIV",
                brief = "PLL Clock Prescaler",
                description = "Select this to force the HSE clock beeing divided by 2.",
                requires = { "HW_PLL_STM32F4", "DISABLED" },
                flavor = "boolean",
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLL_CLOCK_MUL",
                brief = "PLL Multiplier",
                description = "Set this value to override automatic setup of the PLL.\n"..
                              "Respect that the PLL output has to be at maximum 72MHz.\n"..
                              "If USB is used the ouput must provide a frequency of 72MHz or 48MHz.\n"..
                              "This clock is used for audio I2S interface directly.\n",
                requires = { "HW_PLL_STM32F4", "DISABLED" },
                flavor = "boolean",
                file = "include/cfg/clock.h"
            },
            {
                macro = "HSE_VALUE",
                brief = "External Oszillator Frequency",
                description = "Value of the external oscillator in Herz.\n"..
                              "Typical Values are:\n"..
                              "STM32F Conectivity Line Devices: 25MHz\n"..
                              "STM32F Value Line devices asr limited to 24MHz and do not have a PLL."..
                              "Other devices: 8MHz.",
                requires = { "HW_PLL_STM32F4" },
                flavor = "boolean",
                type = "long",
                default = "8000000",
                file = "include/cfg/clock.h"
            },
            {
                macro = "SYSCLK_FREQ",
                brief = "CM3 System Clock",
                description = "System clock (SYSCLK) target frequency after PLL setup.\n\n"..
                              "24MHz USB not supported\n"..
                              "36MHz USB not supported\n"..
                              "48MHz USB supported\n"..
                              "56MHz USB not supported\n"..
                              "72MHz USB supported\n\n",
                requires = { "HW_PLL_STM32F4" },
                type = "enumerated",
                choices = stm32_syclk_frequencies,
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLL_DIV_HCLK",
                brief = "AHB Prescaler",
                description = "This is the divider for the AHB bus. It is supplied by the SYSCLK and must not exceed 72MHz.\n"..
                              "The AHB clocks the core, memory, DMA and all other busses.\n"..
                              "To override auto calculation enter a value n here where the division is 2^n\n"..
                              "where a value of 0 disables the prescaler and the auto-calculation.\n"..
                              "Respect that usage of ethernet or USB requires at least 25MHz on this bus.\n",
                requires = { "HW_PLL_STM32F4", "DISABLED" },
                flavor = "boolean",
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLL_DIV_APB1",
                brief = "APB1 Prescaler",
                description = "This is the divider for the slow peripheral bus (APB1). It is upplied by the AHB clock and "..
                              "it must not exceed 36MHz.\n"..
                              "To override auto calculation enter a value n here where the division is 2^n\n"..
                              "where a value of 0 disables the prescaler and the auto-calculation.\n"..
                              "specific BoardInit() function.\n\n",
                requires = { "HW_PLL_STM32F4", "DISABLED" },
                flavor = "boolean",
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLL_DIV_APB2",
                brief = "APB2 Prescaler",
                description = "This is the divider for the fast peripheral bus (APB2). It is upplied by the AHB clock and "..
                              "it must not exceed 36MHz.\n"..
                              "To override auto calculation enter a value n here where the division is 2^n\n"..
                              "where a value of 0 disables the prescaler and the auto-calculation.\n"..
                              "specific BoardInit() function.\n\n",
                requires = { "HW_PLL_STM32F4", "DISABLED" },
                flavor = "boolean",
                file = "include/cfg/clock.h"
            },
        }
    },

    -- ***********************************
    --
    -- STM32F4 Device Drivers
    --
    -- ***********************************

}
