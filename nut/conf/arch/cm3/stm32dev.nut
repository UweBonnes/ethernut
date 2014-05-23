--
-- Retrieve U(S)ARTs available on the device.
--
function GetStmUsarts()
    if c_is_provided("STM32F030") then
        return { "", "USART1", "USART2" }
    end
    if c_is_provided("STM32F031") then
        return { "", "USART1" }
    end
    if c_is_provided("STM32F040") then
        return { "", "USART1", "USART2" }
    end
    if c_is_provided("STM32F052") then
        return { "", "USART1", "USART2" }
    end
    if c_is_provided("STM32F072") then
        return { "", "USART1", "USART2", "USART3", "USART4", }
    end
    if c_is_provided("STM32F10X_LD_VL") then
        return { "", "USART1", "USART2" }
    end
    if c_is_provided("STM32F10X_MD") then
        return { "", "USART1", "USART2", "USART3" }
    end
    if c_is_provided("STM32F10X_MD_VL") then
        return { "", "USART1", "USART2", "USART3" }
    end
    if c_is_provided("STM32F10X_HD") then
        return { "", "USART1", "USART2", "USART3", "USART4", "USART5" }
    end
    if c_is_provided("STM32F10X_HD_VL") then
        return { "", "USART1", "USART2", "USART3", "USART4", "USART5" }
    end
    if c_is_provided("STM32F10X_XL") then
        return { "", "USART1", "USART2", "USART3", "USART4", "USART5" }
    end
    if c_is_provided("STM32F10X_CL") then
        return { "", "USART1", "USART2", "USART3", "USART4", "USART5" }
    end
    if c_is_provided("STM32F2x") then
        return { "", "USART1", "USART2", "USART3", "USART4", "USART5", "USART6" }
    end
    if c_is_provided("STM32F30X") then
        return { "", "USART1", "USART2", "USART3", "USART4", "USART5" }
    end
    if c_is_provided("STM32F37X") then
        return { "", "USART1", "USART2", "USART3" }
    end
    if c_is_provided("STM32F40_41xxx") then
        return { "", "USART1", "USART2", "USART3", "USART4", "USART5", "USART6" }
    end
    if c_is_provided("STM32F401xx") then
        return { "", "USART1", "USART2",                               "USART6" }
    end
    if c_is_provided("STM32F427_437xx") then
        return { "", "USART1", "USART2", "USART3", "USART4", "USART5", "USART6", "USART7", "USART8" }
    end
    if c_is_provided("STM32F429_439xx") then
        return { "", "USART1", "USART2", "USART3", "USART4", "USART5", "USART6", "USART7", "USART8" }
    end
    if c_is_provided("STM32L1XX_MD") then
        return { "", "USART1", "USART2", "USART3" }
    end
    if c_is_provided("STM32L1XX_MDP") then
        return { "", "USART1", "USART2", "USART3" }
    end
    if c_is_provided("STM32L1XX_HD") then
        return { "", "USART1", "USART2", "USART3", "USART4", "USART5" }
    end
end

--
-- Retrieve USART1TX pins available on the device.
--
function GetTxUsart1()
    if c_is_provided("STM32F3XX") then
        return { "9", "4", "6", "0", "-1" }
    end
    return { "9", "6", "-1" }
end

--
-- Retrieve USART1RX pins available on the device.
--
function GetRxUsart1()
    if c_is_provided("STM32F3XX") then
        return { "10", "5", "7", "1", "-1" }
    end
    return { "10", "7", "-1" }
end

--
-- Retrieve USART2TX pins available on the device.
--
function GetTxUsart2()
    if c_is_provided("STM32F3XX") then
        return { "2", "5", "14", "3", "-1" }
    end
    if c_is_provided("MCU_STM32F0") then
        return { "2", "5", "14", "-1" }
    end
    return { "1", "5", "-1" }
end

--
-- Retrieve USART2RX pins available on the device.
--
function GetRxUsart2()
    if c_is_provided("STM32F3XX") then
        return { "3", "6", "15", "4", "-1" }
    end
    return { "3", "6", "-1" }
end

--
-- Retrieve USART3RX pins available on the device.
--
function GetTxUsart3()
    if c_is_provided("STM32F07X") then
        return { "4", "10", "310", "8", "-1"}
    end
    if c_is_provided("STM32F3XX") then
        return { "11", "311", "9", "15", "-1" }
    end
    return { "11", "311", "9", "-1" }
end

--
-- Retrieve USART3RX pins available on the device.
--
function GetRxUsart3()
    if c_is_provided("STM32F3XX") then
        return { "11", "311", "9", "15", "-1" }
    end
    return { "11", "311", "9", "-1" }
end

--
-- Retrieve USART4TX pins available on the device.
--
function GetTxUsart4()
    if c_is_provided("STM32F10X") then
        return { "0", "-1" }
    end
    if c_is_provided("STM32F3XX") then
        return { "0", "-1" }
    end
    return { "0", "10", "-1" }
end

--
-- Retrieve USART4RX pins available on the device.
--
function GetRxUsart4()
    if c_is_provided("STM32F10X") then
        return { "1", "-1" }
    end
    if c_is_provided("STM32F3XX") then
        return { "1", "-1" }
    end
    return { "1", "11", "-1" }
end

--
-- Retrieve 32 bit Timers.
--
function GetStmTimers32bit()
    if c_is_provided("STM32F2XX") then
        return { "2", "5"}
    end
    if c_is_provided("STM32F30X") then
        return { "2"}
    end
    if c_is_provided("STM32F37X") then
        return { "2", "5"}
    end
    if c_is_provided("STM32F4X") then
        return { "2", "5"}
    end
end

--
-- Retrieve Timer with at least two channels.
--
function GetStmTimers2Ch()
-- List timers with at least two channels
-- List of all known timers with two channels min:
-- {"", "1", "2", "3", "4", "5", "8", "9", "12", "16", "17", "19"}
    if c_is_provided("STM32F10X_LD") then
        return { "", "1", "2", "3"}
    end
    if c_is_provided("STM32F10X_LD_VL") then
        return { "", "1", "2", "3"}
    end
    if c_is_provided("STM32F10X_MD") then
        return { "", "1", "2", "3", "4"}
    end
    if c_is_provided("STM32F10X_MD_VL") then
        return { "", "1", "2", "3", "4"}
    end
    if c_is_provided("STM32F10X_HD") then
        return { "", "1", "2", "3", "4", "5"}
    end
    if c_is_provided("STM32F10X_HD_VL") then
        return { "", "1", "2", "3", "4", "5", "12"}
    end
    if c_is_provided("STM32F10X_XL") then
        return { "", "1", "2", "3", "4", "5"}
    end
    if c_is_provided("STM32F10X_CL") then
        return { "", "1", "2", "3", "4", "5"}
    end
    if c_is_provided("STM32F2XX") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "12"}
    end
    if c_is_provided("STM32F30X") then
        return { "", "1", "2", "3", "4", "8", "16", "17"}
    end
    if c_is_provided("STM32F37X") then
        return { "", "2", "3", "4", "5", "9", "12", "16", "17", "19" }
    end
    if c_is_provided("STM32F401xx") then
        return { "", "1", "2", "3", "4", "5", "9"}
    end
    if c_is_provided("STM32F40_41xxx") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "12"}
    end
    if c_is_provided("STM32F427_437x") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "12"}
    end
    if c_is_provided("STM32F429_439x") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "12"}
    end
    if c_is_provided("STM32L1XX_MD") then
        return { "", "2", "3", "4", "9"}
    end
    if c_is_provided("STM32L1XX_MDP") then
        return { "", "2", "3", "4", "5", "9"}
    end
    if c_is_provided("STM32L1XX_HD") then
        return { "", "2", "3", "4", "5", "9"}
    end
end

--
-- Retrieve Timer with at least two channels and available interrupt.
--
function GetStmTimers2ChIrq()
-- List timers with at least two channels and a seperated interrupt
-- List of timers with two channels min:
-- {"", "1", "2", "3", "4", "5", "8", "9", "12", "16", "17", "19"}
-- TIM1/TIM8/TIM9/TIM12/TIM16/TIM17 with seperated/coupled interrupts on most devices
-- and so not usable yet
    if c_is_provided("STM32F10X_LD") then
        return { "", "2", "3"}
    end
    if c_is_provided("STM32F10X_LD_VL") then
        return { "", "2", "3"}
    end
    if c_is_provided("STM32F10X_MD") then
        return { "", "2", "3", "4"}
    end
    if c_is_provided("STM32F10X_MD_VL") then
        return { "", "2", "3", "4"}
    end
    if c_is_provided("STM32F10X_HD") then
        return { "", "2", "3", "4", "5"}
    end
    if c_is_provided("STM32F10X_HD_VL") then
        return { "", "2", "3", "4", "5", "12"}
    end
    if c_is_provided("STM32F10X_XL") then
        return { "", "2", "3", "4", "5"}
    end
    if c_is_provided("STM32F10X_CL") then
        return { "", "2", "3", "4", "5"}
    end
    if c_is_provided("STM32F2XX") then
        return { "", "2", "3", "4", "5"}
    end
    if c_is_provided("STM32F30X") then
        return { "", "2", "3", "4"}
    end
    if c_is_provided("STM32F37X") then
        return { "", "2", "3", "4", "5", "9", "12", "16", "17", "19" }
    end
    if c_is_provided("STM32F4XX") then
        return { "", "2", "3", "4", "5"}
    end
    if c_is_provided("STM32L1XX_MD") then
        return { "", "2", "3", "4", "9"}
    end
    if c_is_provided("STM32L1XX_MDP") then
        return { "", "2", "3", "4", "5", "9"}
    end
    if c_is_provided("STM32L1XX_HD") then
        return { "", "2", "3", "4", "5", "9"}
    end
end

nutarch_cm3_stm32_devices =
{
    -- ***********************************
    --
    -- STM32 Device Drivers
    --
    -- ***********************************

    -- STM32F10x RTC
    --
    {
        name = "nutarch_cm3_stm32f10x_rtc",
        brief = "STM32F1 RTC Driver",
        description = "STM32F1 RTC driver.",
        requires = { "HW_RTC_STM32F1" },
        provides = { "DEV_RTC" },
        sources = { "cm3/dev/stm/stm32f1_rtc.c" },
    },
    -- STM32 RTC V2
    --
    {
        name = "nutarch_cm3_stm32v2_rtc",
        brief = "STM32 RTC Driver V2",
        description = "STM32 RTC driver V2.",
        requires = { "HW_RTC_STM32_V2" },
        provides = { "DEV_RTC" },
        sources = { "cm3/dev/stm/stm32_rtc_v2.c" },
    },



    --
    -- STM32 DEBUG Macro (Low-Level Debug UART definition)
    --
    {
        name = "nutarch_cm3_stm32_debugmacro",
        brief = "STM32 Low-Level Debug UART macros for use in exception handlers",
        description = "Polling UART function (macro) to use in exception handlers\n",
        requires = { "DEV_UART_STM32" },
        provides = { "DEBUG_MACRO"},
		sources = { "cm3/cmsis/cortex_debug.c" },
        options =
        {
            {
                macro = "DEBUG_MACRO",
                brief = "Enabled",
                description = "Check to enable debug output of exception handlers",
                flavor = "boolean",
                file = "include/cfg/cortex_debug.h"
            },
            {
                macro = "DEBUG_BACKTRACE",
                brief = "Enable call backtrace (experimental)",
                description = "Check to enable call stack back trace in case of an exception.\n\n"..
                              "In case of an exception the last 128 words on the stack will\n"..
                              "be printed out and a call trace will be generated and printed out\n"..
                              "(Address of the function entry and offset in the function).\n"..
                              "Together with the .map file and / or the assembler listing\n"..
                              "the call trace allows you to trace back your code flow in\n"..
                              "which led to the exception. The assembler listing can be\n"..
                              "created by 'arm-none-eabi-objdump -d myfile.elf'\n\n"..
                              "This function is highly experimental and relies on a very\n"..
                              "defined function entry code and existance of a stack\n"..
                              "frame pointer\n"..
                              "To ensure the correct function layout this option disables\n"..
                              "compiler optimisation and enables a stack frame pointer\n"..
                              "generation. The following compiler options will be set:\n\n"..
                              "-O0 -mtpcs-frame -mtpcs-leaf-frame -fno-omit-frame-pointer\n\n"..
                              "This will enlarge the resulkting binary size significantly\n"..
                              "any may influence execution performance.\n\n"..
                              "The backtrace will only work for functions compiled with\n"..
                              "above compiler parameters. E.g. it will very likely not work"..
                              "for any newlib functions.\n",
                flavor = "boolean",
                makedefs = { "MDEBUG+=-O0 -mtpcs-frame -mtpcs-leaf-frame -fno-omit-frame-pointer" },
                file = "include/cfg/cortex_debug.h"
            },
            {
                macro = "DEBUG_UART_NR",
                brief = "Debug UART",
                description = "Select the UART to use for low level debugging",
                type = "enumerated",
                choices = function() return GetStmUsarts() end,
                file = "include/cfg/cortex_debug.h"
            }
        }
    },

    --
    -- STM32 DEBUG UART configuration
    --
    {
        name = "nutarch_cm3_stm32_devices_debug",
        brief = "UART Debug Output (STM32)",
        requires = { "DEV_UART_STM32" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        options =
        {
            {
                macro = "DEBUG_USE_UART",
                brief = "Select debug UART channel",
                description = "Select the UART to use as debug output\n"..
                              "This is only to adapt the application examples and to tell them\n"..
                              "which USART to use as stdout.",
                type = "enumerated",
                default = "DEV_UART1",
                choices = function() return GetStmUsarts() end,
                file = "include/cfg/uart.h",
--                sources = { "cm3/dev/stm/stm32_usart.c" }
            }
        }
    },
    --
    -- STM32 Flash Interface F1 and F3
    --
    {
        name = "nutarch_cm3_stm32f1_3_flash",
        brief = "Flash and parameter access",
        description = "Generic flash and parameter storage interface.",
        requires = { "HW_FLASH_STM32F1_3" },
        provides = { "HW_FLASH", "HW_FLASH_STM32" };
        sources = { "cm3/dev/stm/stm32f1_3_flash.c"}
    },
    --
    -- STM32 Flash Interface L1
    --
    {
        name = "nutarch_cm3_stm32l1_flash",
        brief = "Flash access",
        description = "Generic flash interface.",
        requires = { "HW_FLASH_STM32L1" },
        provides = { "HW_FLASH", "HW_FLASH_STM32" },
        sources = { "cm3/dev/stm/stm32l1_flash.c"},
        options =
        {
            {
                macro = "FLASH_ERASED_PATTERN32",
                brief = "Value of erase flash",
                description = "Value of erase flash. Mostly 0xffffffff, but e.g 0 for STM32L1",
                type = "integer",
                default = "0",
                file = "include/cfg/memory.h",
             },
        }
    },
    --
    -- STM32 EEPROM Interface L1
    --
    {
        name = "nutarch_cm3_stm32l1_eeprom",
        brief = "EEPROM access",
        description = "Generic EEPROM interface.",
        requires = { "HW_EEPROM_STM32L1" },
        provides = { "DEV_NVMEM" };
        sources = { "cm3/dev/stm/stm32l1_eeprom.c"}
    },
     --
    -- STM32 Flash Interface F2 and F4
    --
    {
        name = "nutarch_cm3_stm32f2_4_flash",
        brief = "Flash and parameter access",
        description = "Generic flash and parameter storage interface.",
        requires = { "HW_FLASH_STM32F2_4" },
        provides = { "HW_FLASH", "HW_FLASH_STM32" };
        sources = { "cm3/dev/stm/stm32f2_4_flash.c"},
        options =
        {
            {
                macro = "FLASH_PE_PARALLELISM",
                brief = "Program/erase parallelism",
                description = "Depending on CPU voltage and eventual external programming voltage,\n"..
                "flash erase and programming can happen 8/16/32/64 Bit wide. Default is 32.",
                type = "integer",
                choices = { "8", "16", "32", "64" },
                default = "32",
                file = "include/cfg/memory.h",
             },
             {
                macro = "STM32_VRANGE",
                brief = "Supply Voltage Range",
                description = "E.g. flash access depends on voltage range. Choices are 0 (2.7-3.6V),\n"..
                " 1 (2.4-2.7V), 2 (2.1 - 2.4V) and 3 (1.8-21.1)",
                type = "integer",
                choices = { "0", "1", "2", "3" },
                default = "0",
                file = "include/cfg/memory.h",
             }
         }
    },
    --
     --
    -- STM32 GPIO Interface Version 2 on L1/F2/F4
    --
    {
        name = "nutarch_cm3_stm32_gpio",
        brief = "STM32F2 GPIO",
        description = "Generic port I/O API.",
        requires = { "HW_GPIO_STM32V2" },
        provides = { "HW_GPIO_V2", "HW_GPIO" },
        sources = { "cm3/dev/stm/stm32_gpio.c"}
    },
    --
    -- STM32F EXTI Interrupt handler
    --
    {
        name = "nutarch_cm3_stm32_devices_exti04",
        brief = "STM32F External Interrupt 4..0",
        description = "Interrupt Configuration for external interrups through GPIO.",
        requires = { "HW_MCU_STM32", "HW_EXTI04_STM32" },
        sources = { "cm3/dev/stm/ih_stm32_exti0.c",
                    "cm3/dev/stm/ih_stm32_exti1.c",
                    "cm3/dev/stm/ih_stm32_exti2.c",
                    "cm3/dev/stm/ih_stm32_exti3.c",
                    "cm3/dev/stm/ih_stm32_exti4.c",
                    }
    },
    {
        name = "nutarch_cm3_stm32_devices_exti95",
        brief = "STM32F External Interrupts 9..5",
        description = "Interrupt Configuration for external interrups through GPIO.",
        requires = { "HW_MCU_STM32", "HW_EXTI95_STM32" },
        sources = { "cm3/dev/stm/ih_stm32_exti9_5.c" }
    },
    {
        name = "nutarch_cm3_stm32_devices_exti1510",
        brief = "STM32F External Interrupts 15..10",
        description = "Interrupt Configuration for external interrups through GPIO.",
        requires = { "HW_MCU_STM32", "HW_EXTI1510_STM32" },
        sources = { "cm3/dev/stm/ih_stm32_exti15_10.c" }
    },

    --
    -- STM32 CEC V2 on STM32F373
    --
    {
        name = "nutarch_cm3_stm32_cec_v2",
        brief = "STM32 CEC V2",
        description = "STM32 CEC V2 on STM32F37X.",
        requires = { "HW_CEC_STM32V3" },
        provides = { "HW_CEC_STM32" },
    },
     --
    -- STM32 32 bit Timers
    --
    {
        name = "nutarch_cm3_stm32_timer2_32bit",
        brief = "STM32 Timer2 32Bit",
        description = "STM32 Timer infrastructure.",
        requires = { "HW_TIM2_32BIT_STM32" },
        provides = { "HW_TIM2_STM32" , "HW_STM32_TIMER_32BIT" },
    },
    {
        name = "nutarch_cm3_stm32_qenc32_0",
        brief = "STM32 32Bit Quadrature Encoder0",
        description = "STM32 32Bit Quadrature Encoder 0.",
        requires = { "HW_STM32_TIMER_32BIT" },
        sources = { "cm3/dev/stm/stm32_qenc32_0.c" },
        options =
        {
            {
                macro = "STM32_QENC32_0_TIMER_ID",
                brief = "STM32 32Bit Quadrature Encoder Timer ID",
                description = "Select Timer for 32 bit Quadrature Enoder",
                type = "enumerated",
                choices = function() return GetStmTimers32bit() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC32_0_I_PORT",
                brief = "STM32 qenc32 I input port",
                description = "STM32 32Bit Quadrature Encoder I input port. Can by TI1 or TI2",
                requires = { "HW_STM32_TIMER_32BIT" },
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC32_0_I_PIN",
                brief = "STM32 qenc32 I input pin",
                description = "STM32 32Bit Quadrature Encoder I input pin. Can by TI1 or TI2",
                requires = { "HW_STM32_TIMER_32BIT" },
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC32_0_Q_PORT",
                brief = "STM32 qenc32 Q input port",
                description = "STM32 32Bit Quadrature Encoder I input port. Can by TI1 or TI2",
                requires = { "HW_STM32_TIMER_32BIT" },
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC32_0_Q_PIN",
                brief = "STM32 qenc32 Q input pin",
                description = "STM32 32Bit Quadrature Encoder I input pin. Can by TI1 or TI2",
                requires = { "HW_STM32_TIMER_32BIT" },
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC32_0_INVERT",
                brief = "STM32 qenc32 reverse count direction",
                description = "STM32 32Bit Quadrature Encoder reverse count direction. Effective exchanges I and Q.",
                flavor = "booldata",
                file = "include/cfg/qenc.h",
            },
        },
    },
     --
    -- STM32 16bit Timers for Encode
    --
    {
        name = "nutarch_cm3_stm32_qenc16_0",
        brief = "STM32 32 Bit Quadrature Encoder0 using 16 bit timer",
        description = "STM32 32Bit Quadrature Encoder 0.",
        sources = { "cm3/dev/stm/stm32_qenc16_0.c" },
        options =
        {
            {
                macro = "STM32_QENC16_0_TIMER_ID",
                brief = "STM32 32Bit Quadrature Encoder using 16 bit timer Timer ID",
                description = "Select Timer for 16 bit Quadrature Enoder",
                type = "enumerated",
                choices = function() return GetStmTimers2Ch() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC16_0_I_PORT",
                brief = "STM32 qenc16 I input port",
                description = "STM32 32Bit Quadrature Encoder using 16 bit timer I input port. Can by TI1 or TI2",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC16_0_I_PIN",
                brief = "STM32 qenc16 I input pin",
                description = "STM32 32Bit Quadrature Encoder using 16 bit timer I input pin. Can by TI1 or TI2",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC16_0_Q_PORT",
                brief = "STM32 qenc16 Q input port",
                description = "STM32 32Bit Quadrature Encoder using 16 bit timer I input port. Can by TI1 or TI2",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC16_0_Q_PIN",
                brief = "STM32 qenc16 Q input pin",
                description = "STM32 32Bit Quadrature Encoder using 16 bit timer I input pin. Can by TI1 or TI2",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC16_0_INVERT",
                brief = "STM32 qenc16 reverse count direction",
                description = "STM32 32Bit Quadrature Encoder using 16 bit timer reverse count direction. Effective exchanges I and Q.",
                flavor = "booldata",
                file = "include/cfg/qenc.h",
            },
        },
    },
    --
    -- STM32F USART1 Interface
    --
    {
        name = "nutarch_cm3_stm32_devices_usart1",
        brief = "STM32 USART1 Driver",
        description = "Hardware specific USART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = { "HW_USART1_STM32", "DEV_IRQ_STM32", "NUT_EVENT", "CRT_HEAPMEM" },
        provides =
        {
            "DEV_UART",
            "DEV_UART_STM32",
            "DEV_UART_SPECIFIC",
            "DEV_UART1_GPIO_RTS",
            "DEV_UART1_GPIO_CTS",
            "DEV_UART1_GPIO_HDX"
        },
        sources =
        {
            "cm3/dev/stm/stm32_usart1.c",
        },
        options =
        {
            {
                macro = "USART1_REMAP_USART",
                brief = "Use Alternate Pins",
                description = "Leaving this option unchecked, the driver will initialize the standard Pins.\n\n"..
                              "Pin Mapping is:\n TXD PA9\n RXD PA10 \n CTS PB13\n RTS PB14"..
                              "Enabling the option the driver remaps RXD and TXD to its alternate port pins:\n\n"..
                              "Pin Mapping is:\n TXD PB6\n RXD PB7\n CTS PB13\n RTS PB14"..
                                      "For L1/F2/F4 Pins may also get assigned individually",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_TX_PIN",
                brief = "USART1 TX Pin selection",
                description = "Choose USART1 TX Pin, Default: PA9. Use -1 for not used.",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetTxUsart1() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_RX_PIN",
                brief = "USART1 RX Pin selection",
                description = "Choose USART1 RX Pin, Default: PA10. Use -1 for not used.",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetRxUsart1() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_TX_PIN_INV",
                brief = "USART1 TX Pin inversion",
                description = "Invert  USART1 TX",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_RX_PIN_INV",
                brief = "USART1 RX Pin inversion",
                description = "USART1 RX Pin inversion",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_SWAP",
                brief = "Swap USART1 TX/RX pins",
                description = "Swap USART1 TX/RX pins",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_HARDWARE_HANDSHAKE",
                brief = "Hardware Handshake",
                description = "When selected, the driver will support RTS/CTS hardware handshake.\n"..
                              "The related GPIO pins are initialized automatically.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_HARDWARE_HDX",
                brief = "Half Duplex Mode",
                description = "When selected, the driver will be configured for half-duplex mode.\n",
                flavor = "booldata",
                provides = { "USART1_HARDWARE_HDX" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_1WIRE",
                brief = "1Wire Mode",
                description = "In this mode the UARTs RX pin is not connected but the TX pin is used "..
                              "in bidirectional mode.",
                flavor = "booldata",
                requires = { "USART1_HARDWARE_HDX" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_XONXOFF_CONTROL",
                brief = "XON/XOFF Protocol",
                description = "When selected, the driver will use software XON/XOFF protocol.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_INIT_BAUDRATE",
                brief = "Initial Baudrate",
                description = "Initial baudrate the Port is set to.",
                type = "integer",
                default = 115200,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_SUPPORT_IRQ",
                brief = "Use Interrupt Transfer",
                description = "When selected, the driver will use interrupt for transfers. ",
                flavor = "booldata",
                provides = { "USART1_SUPPORT_IRQ" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_SUPPORT_DMA",
                brief = "Support DMA Blocktransfer",
                description = "When selected, the driver can use DMA for block transfers.\n"..
                              "Block transfers can be enabled or disabled by calling _ioctl "..
                              "with the following parameters:\n"..
                              "UART_SETBLOCKREAD\nUART_SETBLOCKWRITE",
                flavor = "booldata",
                requires = { "USART1_SUPPORT_IRQ" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_MODE_IRDA",
                brief = "Set To IrDA Mode",
                description = "When selected, the USART will run in IrDA mode.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_RS485_CTRL",
                brief = "Support RS485 control",
                description = "When selected a GPIO pin must be configured for RS485 direction control.",
                flavor = "booldata",
                provides = { "USART1_RS485_CTRL" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_485DE_INV",
                brief = "RS485 DE Invert",
                description = "Sets the DE signal as active low.\n",
                requires = { "USART1_RS485_CTRL" },
            flavor = "booldata",
               file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_485DE_PORT",
                brief = "RS485 DE Control Port",
                description = "Select the port of the DE signal.\n",
                requires = { "USART1_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_485DE_PIN",
                brief = "RS485 DE Control Pin",
                description = "Select the pin for the DE signal.\n",
                requires = { "USART1_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_485RE_INV",
                brief = "RS485 RE Invert",
                description = "Sets the RE signal as active high.\n",
                requires = { "USART1_RS485_CTRL" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_485RE_PORT",
                brief = "RS485 /RE Control Port",
                description = "Select the port of the /RE signal.\n",
                requires = { "USART1_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART1_485RE_PIN",
                brief = "RS485 /RE Control Pin",
                description = "Select the pin for the /RE signal.\n",
                requires = { "USART1_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },

--            {
--                macro = "USART1_MODE_LIN",
--                brief = "Set To LIN Mode",
--                description = "When selected, the USART will be configured for LIN bus.",
--                flavor = "booldata",
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "USART1_RXTX_ONLY",
--                brief = "Receive/Transmit Only",
--                description = "When selected, the driver will not support any handshake signals.",
--                flavor = "boolean",
--                exclusivity = { "UART0_RXTX_ONLY", "UART0_HARDWARE_HANDSHAKE", "UART0_MODEM_CONTROL" },
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "USART1_MODEM_CONTROL",
--                brief = "Full Modem Control",
--                description = "When selected, the driver will support full modem control. "..
--                              "Make sure, that all related peripheral pins are available.",
--                flavor = "boolean",
--                exclusivity = { "UART0_RXTX_ONLY", "UART0_HARDWARE_HANDSHAKE", "UART0_MODEM_CONTROL" },
--                requires = { "HW_UART0_MODEM" },
--                file = "include/cfg/uart.h"
--            },
        },
    },

    --
    -- STM32F USART2 Interface
    --
    {
        name = "nutarch_cm3_stm32f_usart2",
        brief = "STM32 USART2 Driver",
        description = "Hardware specific USART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = { "HW_USART2_STM32", "NUT_EVENT", "CRT_HEAPMEM" },
        provides =
        {
            "DEV_UART",
            "DEV_UART_STM32",
            "DEV_UART_SPECIFIC",
            "DEV_UART2_GPIO_RTS",
            "DEV_UART2_GPIO_CTS",
            "DEV_UART2_GPIO_HDX"
        },
        sources =
        {
            "cm3/dev/stm/stm32_usart2.c",
        },
        options =
        {
            {
                macro = "USART2_REMAP_USART",
                brief = "Use Alternate Pins",
                description = "Leaving this option unchecked, the driver will initialize the standard Pins.\n\n"..
                              "Pin Mapping is:\n TXD PA2\n RXD PA3 \n CTS PA0\n RTS PA1"..
                              "Enabling the option the driver remaps RXD and TXD to its alternate port pins:\n\n"..
                              "Pin Mapping is:\n TXD PD5\n RXD PD6\n CTS PD3\n RTS PD4"..
                              "For L1/F2/F4 Pins may also get assigned individually",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_TX_PIN",
                brief = "USART2 TX Pin selection",
                description = "Choose USART2 TX Pin, default: PA2. Use -1 for not used.",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetTxUsart2() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_RX_PIN",
                brief = "USART2 RX Pin selection",
                description = "Choose USART2 RX Pin, default: PA3. Use -1 for not used.",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetTxUsart2() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_TX_PIN_INV",
                brief = "USART2 TX Pin inversion",
                description = "Invert  USART2 TX",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_RX_PIN_INV",
                brief = "USART2 RX Pin inversion",
                description = "USART2 RX Pin inversion",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_SWAP",
                brief = "Swap USART2 TX/RX pins",
                description = "Swap USART2 TX/RX pins",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_CK_PIN",
                brief = "USART2 CK Pin selection",
                description = "Choose USART2 CK Pin from PA4(default)and PD7",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "4", "7" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_CTS_PIN",
                brief = "USART2 CTS Pin selection",
                description = "Choose USART2 CTS Pin from PA0(default) and PD3",
                requires = { "HW_GPIO_STM32V2", "USART2_HARDWARE_HANDSHAKE" },
                type = "enumerated",
                choices = { "0", "3" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_RTS_PIN",
                brief = "USART2 RTS Pin selection",
                description = "Choose USART2 RTS Pin from PA1(default) and PD2",
                requires = { "HW_GPIO_STM32V2", "USART2_HARDWARE_HANDSHAKE" },
                type = "enumerated",
                choices = { "1", "2" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_HARDWARE_HANDSHAKE",
                brief = "Hardware Handshake",
                description = "When selected, the driver will support RTS/CTS hardware handshake. "..
                              "The related GPIO pins are initialized automatically.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_HARDWARE_HDX",
                brief = "Half Duplex Mode",
                description = "When selected, the driver will be configured for half-duplex mode.\n",
                provides = { "USART2_HARDWARE_HDX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_1WIRE",
                brief = "1Wire Mode",
                description = "In this mode the UARTs RX pin is not connected but the TX pin is used "..
                              "in bidirectional mode.",
                flavor = "booldata",
                requires = { "USART2_HARDWARE_HDX" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_XONXOFF_CONTROL",
                brief = "XON/XOFF Protocol",
                description = "When selected, the driver will use software XON/XOFF protocol.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_INIT_BAUDRATE",
                brief = "Initial Baudrate",
                description = "Initial baudrate the Port is set to.",
                type = "integer",
                default = 115200,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_SUPPORT_IRQ",
                brief = "Use Interrupt Transfer",
                description = "When selected, the driver will use interrupt for transfers. ",
                flavor = "booldata",
                provides = { "USART2_SUPPORT_IRQ" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_SUPPORT_DMA",
                brief = "Support DMA Blocktransfer",
                description = "When selected, the driver can use DMA for block transfers.\n"..
                              "Block transfers can be enabled or disabled by calling _ioctl "..
                              "with the following parameters:\n"..
                              "UART_SETBLOCKREAD\nUART_SETBLOCKWRITE",
                flavor = "booldata",
                requires = { "USART2_SUPPORT_IRQ" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_MODE_IRDA",
                brief = "Set To IrDA Mode",
                description = "When selected, the USART will run in IrDA mode.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_RS485_CTRL",
                brief = "Support RS485 control",
                description = "When selected a GPIO pin must be configured for RS485 direction control.",
                flavor = "booldata",
                provides = { "USART2_RS485_CTRL" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_485DE_INV",
                brief = "RS485 DE Invert",
                description = "Sets the DE signal as active low.\n",
                requires = { "USART2_RS485_CTRL" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_485DE_PORT",
                brief = "RS485 DE Control Port",
                description = "Select the port of the DE signal.\n",
                requires = { "USART2_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_485DE_PIN",
                brief = "RS485 DE Control Pin",
                    description = "Select the pin for the DE signal.\n",
                requires = { "USART2_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_485RE_INV",
                brief = "RS485 RE Invert",
                description = "Sets the RE signal as active high.\n",
                requires = { "USART2_RS485_CTRL" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_485RE_PORT",
                brief = "RS485 /RE Control Port",
                description = "Select the port of the /RE signal.\n",
                requires = { "USART2_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART2_485RE_PIN",
                brief = "RS485 /RE Control Pin",
                description = "Select the pin for the /RE signal.\n",
                requires = { "USART2_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },
--            {
--                macro = "USART2_MODE_LIN",
--                brief = "Set To LIN Mode",
--                description = "When selected, the USART will be configured for LIN bus.",
--                flavor = "booldata",
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "UART2_RXTX_ONLY",
--                brief = "Receive/Transmit Only",
--                description = "When selected, the driver will not support any handshake signals.",
--                flavor = "boolean",
--                exclusivity = { "UART1_RXTX_ONLY", "UART1_HARDWARE_HANDSHAKE", "UART1_MODEM_CONTROL" },
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "UART2_MODEM_CONTROL",
--                brief = "Full Modem Control",
--                description = "When selected, the driver will support full modem control. "..
--                              "Make sure, that all related peripheral pins are available.",
--                flavor = "boolean",
--                exclusivity = { "UART1_RXTX_ONLY", "UART1_HARDWARE_HANDSHAKE", "UART1_MODEM_CONTROL" },
--                requires = { "HW_UART1_MODEM" },
--                file = "include/cfg/uart.h"
--            },
        },
    },

    --
    -- STM32F USART3 Interface
    --
    {
        name = "nutarch_cm3_stm32_devices_usart3",
        brief = "STM32 USART3 Driver",
        description = "Hardware specific USART driver. Implements hardware "..
                  "functions for the generic driver framework.",
        requires = { "HW_USART3_STM32", "DEV_IRQ_STM32", "NUT_EVENT", "CRT_HEAPMEM" },
        provides =
        {
            "DEV_UART",
            "DEV_UART_STM32",
            "DEV_UART_SPECIFIC",
            "DEV_UART3_GPIO_RTS",
            "DEV_UART3_GPIO_CTS",
            "DEV_UART3_GPIO_HDX"
        },
        sources =
        {
            "cm3/dev/stm/stm32_usart3.c",
        },
        options =
        {
            {
                macro = "USART3_NOREMAP_USART",
                brief = "Use Default Pins",
                description = "When selected, the driver will initialize the standard Pins.\n\n"..
                              "Pin Mapping is:\n TXD B10\n RXD PB11 \n CTS PB13\n RTS PB14"..
                              "For L1/F2/F4 Pins may also get assigned individually",
                flavor = "booldata",
                exclusivity = { "USART3_NOREMAP_USART", "USART3_PARTREMAP_USART", "USART3_FULLREMAP_USART" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_PARTREMAP_USART",
                brief = "Partial Remap Pins",
                description = "When selected, the driver will initialize the alternate pins in\n"..
                              "a partially remapped configuration:\n\n"..
                              "Pin Mapping is:\n TXD PC10\n RXD PC11\n CTS PB13\n RTS PB14"..
                              "For L1/F2/F4 Pins may also get assigned individually",
                flavor = "booldata",
                exclusivity = { "USART3_NOREMAP_USART", "USART3_PARTREMAP_USART", "USART3_FULLREMAP_USART" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_FULLREMAP_USART",
                brief = "Full Remap Pins",
                description = "When selected, the driver will initialize the alternate pins in\n"..
                              "a fully remapped configuration:\n\n"..
                              "Pin Mapping is:\n TXD PD8\n RXD PD9\n CTS PD11\n RTS PD12"..
                              "For L1/F2/F4 Pins may also get assigned individually",
                flavor = "booldata",
                exclusivity = { "USART3_NOREMAP_USART", "USART3_PARTREMAP_USART", "USART3_FULLREMAP_USART" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_TX_PIN",
                brief = "USART3 TX Pin selection",
                description = "Choose USART3 TX Pin, default: PB10. Use -1 for not used.",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "10", "310", "8" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_RX_PIN",
                brief = "USART3 RX Pin selection",
                description = "Choose USART3 RX Pin, default: PB11. Use -1 for not used.",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetTxUsart3() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_TX_PIN_INV",
                brief = "USART3 TX Pin inversion",
                description = "Invert  USART3 TX",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_RX_PIN_INV",
                brief = "USART3 RX Pin inversion",
                description = "USART3 RX Pin inversion",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_SWAP",
                brief = "Swap USART3 TX/RX pins",
                description = "Swap USART3 TX/RX pins",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_CK_PIN",
                brief = "USART3 CK Pin selection",
                description = "Choose USART3 CK Pin from PB12(default), PC12 and PD10",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "12", "312", "10" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_CTS_PIN",
                brief = "USART3 CTS Pin selection",
                description = "Choose USART3 CTS Pin from PB13(default) and PD11",
                requires = { "HW_GPIO_STM32V2", "USART3_HARDWARE_HANDSHAKE" },
                type = "enumerated",
                default = "13",
                choices = { "13", "11" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_RTS_PIN",
                brief = "USART3 RTS Pin selection",
                description = "Choose USART3 RTS Pin from PB14(default) and PD12",
                requires = { "HW_GPIO_STM32V2", "USART3_HARDWARE_HANDSHAKE" },
                type = "enumerated",
                choices = { "14", "12" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_HARDWARE_HANDSHAKE",
                brief = "Hardware Handshake",
                description = "When selected, the driver will support RTS/CTS hardware handshake. "..
                              "The related GPIO pins are initialized automatically.",
                provides = { "USART3_HARDWARE_HANDSHAKE" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_HARDWARE_HDX",
                brief = "Half Duplex Mode",
                description = "When selected, the driver will be configured for half-duplex mode.\n",
                provides = { "USART3_HARDWARE_HDX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_1WIRE",
                brief = "1Wire Mode",
                description = "In this mode the UARTs RX pin is not connected but the TX pin is used "..
                              "in bidirectional mode.",
                flavor = "booldata",
                requires = { "USART3_HARDWARE_HDX" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_XONXOFF_CONTROL",
                brief = "XON/XOFF Protocol",
                description = "When selected, the driver will use software XON/XOFF protocol.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_INIT_BAUDRATE",
                brief = "Initial Baudrate",
                description = "Initial baudrate the Port is set to.",
                type = "integer",
                default = 115200,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_SUPPORT_IRQ",
                brief = "Use Interrupt Transfer",
                description = "When selected, the driver will use interrupt for transfers. ",
                flavor = "booldata",
                provides = { "USART2_SUPPORT_IRQ" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_SUPPORT_DMA",
                brief = "Support DMA Blocktransfer",
                description = "When selected, the driver can use DMA for block transfers.\n"..
                              "Block transfers can be enabled or disabled by calling _ioctl "..
                              "with the following parameters:\n"..
                              "UART_SETBLOCKREAD\nUART_SETBLOCKWRITE",
                flavor = "booldata",
                requires = { "USART2_SUPPORT_IRQ" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_MODE_IRDA",
                brief = "Set To IrDA Mode",
                description = "When selected, the USART will run in IrDA mode.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_RS485_CTRL",
                brief = "Support RS485 control",
                description = "When selected a GPIO pin must be configured for RS485 direction control.",
                flavor = "booldata",
                provides = { "USART3_RS485_CTRL" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_485DE_INV",
                brief = "RS485 DE Invert",
                description = "Sets the DE signal as active low.\n",
                requires = { "USART3_RS485_CTRL" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_485DE_PORT",
                brief = "RS485 DE Control Port",
                description = "Select the port of the DE signal.\n",
                requires = { "USART3_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_485DE_PIN",
                brief = "RS485 DE Control Pin",
                description = "Select the pin for the DE signal.\n",
                requires = { "USART3_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_485RE_INV",
                brief = "RS485 RE Invert",
                    description = "Sets the RE signal as active high.\n",
                requires = { "USART3_RS485_CTRL" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_485RE_PORT",
                brief = "RS485 /RE Control Port",
                description = "Select the port of the /RE signal.\n",
                requires = { "USART3_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART3_485RE_PIN",
                brief = "RS485 /RE Control Pin",
                description = "Select the pin for the /RE signal.\n",
                requires = { "USART3_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },
--            {
--                macro = "USART3_MODE_LIN",
--                brief = "Set To LIN Mode",
--                description = "When selected, the USART will be configured for LIN bus.",
--                flavor = "booldata",
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "USART3_RXTX_ONLY",
--                brief = "Receive/Transmit Only",
--                description = "When selected, the driver will not support any handshake signals.",
--                flavor = "boolean",
--                exclusivity = { "UART3_RXTX_ONLY", "UART1_HARDWARE_HANDSHAKE", "UART1_MODEM_CONTROL" },
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "USART3_MODEM_CONTROL",
--                brief = "Full Modem Control",
--                description = "When selected, the driver will support full modem control. "..
--                              "Make sure, that all related peripheral pins are available.",
--                flavor = "boolean",
--                exclusivity = { "UART3_RXTX_ONLY", "UART1_HARDWARE_HANDSHAKE", "UART1_MODEM_CONTROL" },
--                requires = { "HW_UART1_MODEM" },
--                file = "include/cfg/uart.h"
--            },
        },
    },

    --
    -- STM32F UART4 Interface
    --
    {
        name = "nutarch_cm3_stm32f_uart4",
        brief = "STM32 UART4 Driver",
        description = "Hardware specific UART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = { "HW_UART4_STM32", "DEV_IRQ_STM32", "NUT_EVENT", "CRT_HEAPMEM" },
        provides =
        {
            "DEV_UART",
            "DEV_UART_STM32",
            "DEV_UART_SPECIFIC",
            "DEV_UART4_GPIO_HDX"
        },
        sources =
        {
            "cm3/dev/stm/stm32_uart4.c",
        },
        options =
        {
            {
                macro = "UART4_REMAP_USART",
                brief = "Use Alternate Pins",
                description = "Leaving this option unchecked, the driver will initialize the standard Pins.\n\n"..
                              "Pin Mapping is:\n TXD PA0\n RXD PA1"..
                              "Enabling the option the driver remaps RXD and TXD to its alternate port pins:\n\n"..
                              "Pin Mapping is:\n TXD PC10\n RXD PC11"..
                              "For L1/F2/F4 Pins may also get assigned individually",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_TX_PIN",
                brief = "UART4 TX Pin selection",
                description = "Choose UART4 TX Pin, default: PA0. Use -1 for not used.",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetTxUsart4() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_RX_PIN",
                brief = "UART4 RX Pin selection",
                description = "Choose UART4 RX Pin, default: PA1. Use -1 for not used.",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetRxUsart4() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART4_TX_PIN_INV",
                brief = "USART4 TX Pin inversion",
                description = "Invert  USART4 TX",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART4_RX_PIN_INV",
                brief = "USART4 RX Pin inversion",
                description = "USART4 RX Pin inversion",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART4_SWAP",
                brief = "Swap USART4 TX/RX pins",
                description = "Swap USART4 TX/RX pins",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_HARDWARE_HDX",
                brief = "Half Duplex Mode",
                description = "When selected, the driver will be configured for half-duplex mode.\n",
                provides = { "UART4_HARDWARE_HDX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_1WIRE",
                brief = "1Wire Mode",
                description = "In this mode the UARTs RX pin is not connected but the TX pin is used "..
                              "in bidirectional mode.",
                flavor = "booldata",
                requires = { "UART4_HARDWARE_HDX" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_XONXOFF_CONTROL",
                brief = "XON/XOFF Protocol",
                description = "When selected, the driver will use software XON/XOFF protocol.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_INIT_BAUDRATE",
                brief = "Initial Baudrate",
                description = "Initial baudrate the Port is set to.",
                type = "integer",
                default = 115200,
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_SUPPORT_IRQ",
                brief = "Use Interrupt Transfer",
                description = "When selected, the driver will use interrupt for transfers. ",
                flavor = "booldata",
                provides = { "UART4_SUPPORT_IRQ" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_SUPPORT_DMA",
                brief = "Support DMA Blocktransfer",
                description = "When selected, the driver can use DMA for block transfers.\n"..
                              "Block transfers can be enabled or disabled by calling _ioctl "..
                              "with the following parameters:\n"..
                              "UART_SETBLOCKREAD\nUART_SETBLOCKWRITE",
                flavor = "booldata",
                requires = { "UART4_SUPPORT_IRQ" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_MODE_IRDA",
                brief = "Set To IrDA Mode",
                description = "When selected, the UART will run in IrDA mode.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_RS485_CTRL",
                brief = "Support RS485 control",
                description = "When selected a GPIO pin must be configured for RS485 direction control.",
                flavor = "booldata",
                provides = { "UART4_RS485_CTRL" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_485DE_INV",
                brief = "RS485 DE Invert",
                description = "Sets the DE signal as active low.\n",
                requires = { "UART4_RS485_CTRL" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_485DE_PORT",
                brief = "RS485 DE Control Port",
                description = "Select the port of the DE signal.\n",
                requires = { "UART4_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_485DE_PIN",
                brief = "RS485 DE Control Pin",
                description = "Select the pin for the DE signal.\n",
                requires = { "UART4_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_485RE_INV",
                brief = "RS485 RE Invert",
                description = "Sets the RE signal as active high.\n",
                requires = { "UART4_RS485_CTRL" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_485RE_PORT",
                brief = "RS485 /RE Control Port",
                description = "Select the port of the /RE signal.\n",
                requires = { "UART4_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART4_485RE_PIN",
                brief = "RS485 /RE Control Pin",
                description = "Select the pin for the /RE signal.\n",
                requires = { "UART4_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },
--            {
--                macro = "UART4_MODE_LIN",
--                brief = "Set To LIN Mode",
--                description = "When selected, the UART will be configured for LIN bus.",
--                flavor = "booldata",
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "UART4_RXTX_ONLY",
--                brief = "Receive/Transmit Only",
--                description = "When selected, the driver will not support any handshake signals.",
--                flavor = "boolean",
--                exclusivity = { "UART4_RXTX_ONLY", "UART4_HARDWARE_HANDSHAKE", "UART4_MODEM_CONTROL" },
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "UART4_MODEM_CONTROL",
--                brief = "Full Modem Control",
--                description = "When selected, the driver will support full modem control. "..
--                              "Make sure, that all related peripheral pins are available.",
--                flavor = "boolean",
--                exclusivity = { "UART4_RXTX_ONLY", "UART4_HARDWARE_HANDSHAKE", "UART4_MODEM_CONTROL" },
--                requires = { "HW_UART1_MODEM" },
--                file = "include/cfg/uart.h"
--            },
        },
    },

    --
    -- STM32F UART5 Interface
    --
    {
        name = "nutarch_cm3_stm32f_uart5",
        brief = "STM32 UART5 Driver",
        description = "Hardware specific UART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = { "HW_UART5_STM32", "DEV_IRQ_STM32", "NUT_EVENT", "CRT_HEAPMEM" },
        provides =
        {
            "DEV_UART",
            "DEV_UART_SPECIFIC",
            "DEV_UART5_GPIO_HDX"
        },
        sources =
        {
            "cm3/dev/stm/stm32_uart5.c",
        },
        options =
        {
            {
                macro = "USART5_TX_PIN_INV",
                brief = "USART5 TX Pin inversion",
                description = "Invert  USART5 TX",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART5_RX_PIN_INV",
                brief = "USART5 RX Pin inversion",
                description = "USART5 RX Pin inversion",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART5_SWAP",
                brief = "Swap USART5 TX/RX pins",
                description = "Swap USART5 TX/RX pins",
                requires = { "HW_GPIO_STM32V2", "STM32F3XX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_HARDWARE_HDX",
                brief = "Half Duplex Mode",
                description = "When selected, the driver will be configured for half-duplex mode.\n",
                provides = { "UART5_HARDWARE_HDX" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_1WIRE",
                brief = "1Wire Mode",
                description = "In this mode the UARTs RX pin is not connected but the TX pin is used "..
                              "in bidirectional mode.",
                flavor = "booldata",
                requires = { "UART5_HARDWARE_HDX" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_XONXOFF_CONTROL",
                brief = "XON/XOFF Protocol",
                description = "When selected, the driver will use software XON/XOFF protocol.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_INIT_BAUDRATE",
                brief = "Initial Baudrate",
                description = "Initial baudrate the Port is set to.",
                type = "integer",
                default = 115200,
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_SUPPORT_IRQ",
                brief = "Use Interrupt Transfer",
                description = "When selected, the driver will use interrupt for transfers. ",
                flavor = "booldata",
                provides = { "UART5_SUPPORT_IRQ" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_MODE_IRDA",
                brief = "Set To IrDA Mode",
                description = "When selected, the UART will run in IrDA mode.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_RS485_CTRL",
                brief = "Support RS485 control",
                description = "When selected a GPIO pin must be configured for RS485 direction control.",
                flavor = "booldata",
                provides = { "UART5_RS485_CTRL" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_485DE_INV",
                brief = "RS485 DE Invert",
                description = "Sets the DE signal as active low.\n",
                requires = { "UART5_RS485_CTRL" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_485DE_PORT",
                brief = "RS485 DE Control Port",
                description = "Select the port of the DE signal.\n",
                requires = { "UART5_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_485DE_PIN",
                brief = "RS485 DE Control Pin",
                description = "Select the pin for the DE signal.\n",
                requires = { "UART5_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_485RE_INV",
                brief = "RS485 RE Invert",
                description = "Sets the RE signal as active high.\n",
                requires = { "UART5_RS485_CTRL" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART35_485RE_PORT",
                brief = "RS485 /RE Control Port",
                description = "Select the port of the /RE signal.\n",
                requires = { "UART5_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "UART5_485RE_PIN",
                brief = "RS485 /RE Control Pin",
                description = "Select the pin for the /RE signal.\n",
                requires = { "UART5_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },
--            {
--                macro = "UART5_MODE_LIN",
--                brief = "Set To LIN Mode",
--                description = "When selected, the UART will be configured for LIN bus.",
--                flavor = "booldata",
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "UART5_RXTX_ONLY",
--                brief = "Receive/Transmit Only",
--                description = "When selected, the driver will not support any handshake signals.",
--                flavor = "boolean",
--                exclusivity = { "UART5_RXTX_ONLY", "UART5_HARDWARE_HANDSHAKE", "UART5_MODEM_CONTROL" },
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "UART5_MODEM_CONTROL",
--                brief = "Full Modem Control",
--                description = "When selected, the driver will support full modem control. "..
--                              "Make sure, that all related peripheral pins are available.",
--                flavor = "boolean",
--                exclusivity = { "UART5_RXTX_ONLY", "UART5_HARDWARE_HANDSHAKE", "UART5_MODEM_CONTROL" },
--                requires = { "HW_UART5_MODEM" },
--                file = "include/cfg/uart.h"
--            },
        },
    },
    --
    -- STM32F USART6 Interface
    --
    {
        name = "nutarch_cm3_stm32_devices_usart6",
        brief = "STM32 USART6 Driver",
        description = "Hardware specific USART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = { "HW_USART6_STM32", "DEV_IRQ_STM32", "NUT_EVENT", "CRT_HEAPMEM" },
        provides =
        {
            "DEV_UART",
            "DEV_UART_STM32",
            "DEV_UART_SPECIFIC",
            "DEV_UART6_GPIO_RTS",
            "DEV_UART6_GPIO_CTS",
            "DEV_UART6_GPIO_HDX"
        },
        sources =
        {
            "cm3/dev/stm/stm32_usart6.c",
        },
        options =
        {
            {
                macro = "USART6_REMAP_USART",
                brief = "Use Alternate Pins",
                description = "Leaving this option unchecked, the driver will initialize the standard Pins.\n\n"..
                              "Pin Mapping is:\n TXD PC6\n RXD PC7 \n CTS PG13\n RTS PG8\n CK PC8"..
                              "Enabling the option the driver remaps RXD and TXD to its alternate port pins:\n\n"..
                              "Pin Mapping is:\n TXD PG14\n RXD PG9\n CTS PG15\n RTS PG12\n CK PG7"..
                                      "For L1/F2/F4 Pins may also get assigned individually",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_TX_PIN",
                brief = "USART6 TX Pin selection",
                description = "Choose USART6 TX Pin from PC6(default) and PG14 or not used(-1)",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "6", "14", "-1" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_RX_PIN",
                brief = "USART6 RX Pin selection",
                description = "Choose USART6 RX Pin from PC7(default) and PG9 or not used(-1)",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "7", "9", "-1" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_CK_PIN",
                brief = "USART6 CK Pin selection",
                description = "Choose USART3 CK Pin from PC8(default) and PG7",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                default = "8",
                choices = { "8", "7" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_CTS_PIN",
                brief = "USART6 CTS Pin selection",
                description = "Choose USART6 CTS Pin from PG13(default) and PG15",
                requires = { "HW_GPIO_STM32V2", "USART3_HARDWARE_HANDSHAKE" },
                type = "enumerated",
                choices = { "13", "15" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_RTS_PIN",
                brief = "USART6 RTS Pin selection",
                description = "Choose USART6 RTS Pin from PG8(default) and PG12",
                requires = { "HW_GPIO_STM32V2", "USART3_HARDWARE_HANDSHAKE" },
                type = "enumerated",
                choices = { "8", "12" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_HARDWARE_HANDSHAKE",
                brief = "Hardware Handshake",
                description = "When selected, the driver will support RTS/CTS hardware handshake.\n"..
                              "The related GPIO pins are initialized automatically.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_HARDWARE_HDX",
                brief = "Half Duplex Mode",
                description = "When selected, the driver will be configured for half-duplex mode.\n",
                flavor = "booldata",
                provides = { "USART6_HARDWARE_HDX" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_1WIRE",
                brief = "1Wire Mode",
                description = "In this mode the UARTs RX pin is not connected but the TX pin is used "..
                              "in bidirectional mode.",
                flavor = "booldata",
                requires = { "USART6_HARDWARE_HDX" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_XONXOFF_CONTROL",
                brief = "XON/XOFF Protocol",
                description = "When selected, the driver will use software XON/XOFF protocol.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_INIT_BAUDRATE",
                brief = "Initial Baudrate",
                description = "Initial baudrate the Port is set to.",
                type = "integer",
                default = 115200,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_SUPPORT_IRQ",
                brief = "Use Interrupt Transfer",
                description = "When selected, the driver will use interrupt for transfers. ",
                flavor = "booldata",
                provides = { "USART6_SUPPORT_IRQ" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_SUPPORT_DMA",
                brief = "Support DMA Blocktransfer",
                description = "When selected, the driver can use DMA for block transfers.\n"..
                              "Block transfers can be enabled or disabled by calling _ioctl "..
                              "with the following parameters:\n"..
                              "UART_SETBLOCKREAD\nUART_SETBLOCKWRITE",
                flavor = "booldata",
                requires = { "USART6_SUPPORT_IRQ" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_MODE_IRDA",
                brief = "Set To IrDA Mode",
                description = "When selected, the USART will run in IrDA mode.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_RS485_CTRL",
                brief = "Support RS485 control",
                description = "When selected a GPIO pin must be configured for RS485 direction control.",
                flavor = "booldata",
                provides = { "USART6_RS485_CTRL" },
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_485DE_INV",
                brief = "RS485 DE Invert",
                    description = "Sets the DE signal as active low.\n",
                requires = { "USART6_RS485_CTRL" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_485DE_PORT",
                brief = "RS485 DE Control Port",
                description = "Select the port of the DE signal.\n",
                requires = { "USART6_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_485DE_PIN",
                brief = "RS485 DE Control Pin",
                description = "Select the pin for the DE signal.\n",
                requires = { "USART6_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_485RE_INV",
                brief = "RS485 RE Invert",
                description = "Sets the RE signal as active high.\n",
                requires = { "USART6_RS485_CTRL" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_485RE_PORT",
                brief = "RS485 /RE Control Port",
                description = "Select the port of the /RE signal.\n",
                requires = { "USART6_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/uart.h"
            },
            {
                macro = "USART6_485RE_PIN",
                brief = "RS485 /RE Control Pin",
                description = "Select the pin for the /RE signal.\n",
                requires = { "USART6_RS485_CTRL" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/uart.h"
            },

--            {
--                macro = "USART6_MODE_LIN",
--                brief = "Set To LIN Mode",
--                description = "When selected, the USART will be configured for LIN bus.",
--                flavor = "booldata",
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "USART6_RXTX_ONLY",
--                brief = "Receive/Transmit Only",
--                description = "When selected, the driver will not support any handshake signals.",
--                flavor = "boolean",
--                exclusivity = { "UART0_RXTX_ONLY", "UART0_HARDWARE_HANDSHAKE", "UART0_MODEM_CONTROL" },
--                file = "include/cfg/uart.h"
--            },
--            {
--                macro = "USART6_MODEM_CONTROL",
--                brief = "Full Modem Control",
--                description = "When selected, the driver will support full modem control. "..
--                              "Make sure, that all related peripheral pins are available.",
--                flavor = "boolean",
--                exclusivity = { "UART0_RXTX_ONLY", "UART0_HARDWARE_HANDSHAKE", "UART0_MODEM_CONTROL" },
--                requires = { "HW_UART0_MODEM" },
--                file = "include/cfg/uart.h"
--            },
        },
    },

    --
    -- STM32F EMAC Configuration
    --
    {
        name = "nutarch_cm3_stm32f_emac",
        brief = "STM32F EMAC Driver",
        description = "LAN driver for STM32Fxx7xx.",
        requires = { "HW_EMAC_STM32", "NUT_EVENT", "NUT_TIMER" },
        provides = { "NET_MAC" },
        sources = { "cm3/dev/stm/stm32_emac.c" },
        options =
        {
            {
                macro = "EMAC_REMAP",
                brief = "Use Alternate Pins",
                description = "Leaving this option unchecked, the driver will initialize the standard Pins.\n\n"..
                              "Pin Mapping is:\n RX_DV-CRS PA7\n RXD0 PC4 \n RXD1 PC5\n RXD2 PB0\n RXD3 PB1\n"..
                              "Enabling the option the driver remaps to its alternate port pins:\n\n"..
                              "Pin Mapping is:\n RX_DV-CRS PD8\n RXD0 PD9 \n RXD1 PD10\n RXD2 PD11\n RXD3 PD12\n",
                flavor = "booldata",
                file = "include/cfg/dev.h"
            },
            {
                macro = "PHY_PWRDN_BIT",
                brief = "PHY Power Down Bit",
                description = "For the SAM7X default is 18.\n\n",
                provides = { "PHY_PWRDN_CONTROL" },
                flavor = "booldata",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "PHY_PWRDN_NEGPOL",
                brief = "PHY Power Down Polarity",
                description = "Select this, if the PHY is powered down by a low signal.",
                requires = { "PHY_PWRDN_CONTROL" },
                flavor = "boolean",
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "NUT_THREAD_NICRXSTACK",
                brief = "Receiver Thread Stack",
                description = "Number of bytes to be allocated for the stack of the NIC receive thread.",
                flavor = "booldata",
                type = "integer",
                file = "include/cfg/dev.h"
            },
            {
                macro = "EMAC_RX_BUFFERS",
                brief = "Receive Buffers",
                description = "Number of 128 byte receive buffers.\n"..
                              "Increase to handle high traffic situations.\n"..
                              "Decrease to handle low memory situations.\n"..
                              "Default is 32.\n",
                flavor = "booldata",
                type = "integer",
                file = "include/cfg/dev.h"
            },
            {
                macro = "EMAC_TX_BUFSIZ",
                brief = "Transmit Buffer Size",
                description = "The driver will allocate two transmit buffers.\n"..
                              "Can be decreased in low memory situations. Be aware, "..
                              "that this may break your network application. Do not "..
                              "change this without exactly knowing the consequences.\n"..
                              "Default is 1536.\n",
                flavor = "booldata",
                type = "integer",
                file = "include/cfg/dev.h"
            },
            {
                macro = "EMAC_LINK_LOOPS",
                brief = "Link Polling Loops",
                description = "This simple implementation runs a dumb polling loop "..
                              "while waiting for the Ethernet link status.\n"..
                              "If you experience link problems, increasing this value "..
                              "may help.\n"..
                              "Default is 10000.\n",
                flavor = "booldata",
                type = "integer",
                file = "include/cfg/dev.h"
            },
        }
    },

    --
    -- STM32F I2C Bus 1 Configuration
    --
    {
        name = "nutarch_cm3_stm32f_i2c1",
        brief = "STM32F I2C Bus 1 Controller",
        description = "STM32F hardware I2C support.",
        requires = { "HW_I2C1_STM32" },
        provides = { "DEV_TWI" },
        sources =
        {
            "cm3/dev/stm/stm32_twi1.c",
            "cm3/dev/stm/stm32_twi.c"
        },
        options =
        {
            {
                macro = "I2CBUS1_AS_DEFAULT",
                brief = "Use as default I2C",
                description = "Enabling this options reconfigures TwiXxx() Macros to use this bus.\n"..
                              "This is for compatibility with Nut/OS.",
                exclusivity = { "I2CBUS1_AS_DEFAULT", "I2CBUS2_AS_DEFAULT" },
                flavor = "booldata",
                file = "include/cfg/twi.h"

            },
            {
                macro = "I2CBUS1_REMAP_I2C",
                brief = "Use Alternate Pins",
                description = "Leaving this option unchecked, the driver will initialize the standard Pins.\n\n"..
                              "Pin Mapping is:\n SCL PB6\n SDA PB7\n"..
                              "By enabling the option alternate port pins are used:\n\n"..
                              "Pin Mapping is:\n SCL PB8\n SDA PB9\n"..
                                      "For L1/F2/F4 Pins may also get assigned individually",
                flavor = "booldata",
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS1_SCL_PIN",
                brief = "I2C1 SCL Pins",
                description = "Pin for I2C1 SCL, Default PB6 or PB8",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "6", "8" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS1_SDA_PIN",
                brief = "I2C1 SDA Pins",
                description = "Pin for I2C1 SDA, Default PB7 or PB11",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "7", "9" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS1_DEFAULT_SPEED",
                brief = "Default Speed",
                description = "Default speed for this bus. Different speeds can be set by software.\n",
                default = "100",
                type = "boolean",
                choices = { "75", "100", "400" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS1_USE_DMA",
                brief = "Use DMA with I2C1",
                description = "Enabling this options configures I2C bus to use DMA.\n\n"..
                              "Errata Sheet 14574 Rev.9:\n"..
                              "This should be default for STM32F10xxB/8 devices according this Errata "..
                              "to overcome the mentioned limitations.",
                flavor = "booldata",
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS1_MODE_SMBUS",
                brief = "Enable SMBUS",
                description = "Enabling this options adds SM-Bus functions and initializes the Alert line GPIO.\n"..
                              "For this bus SMBA is PB5.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "booldata",
                file = "include/cfg/twi.h"
            },
        },
    },

    --
    -- STM32F I2C Bus 2 Configuration
    --
    {
        name = "nutarch_cm3_stm32f_i2c2",
        brief = "STM32F I2C Bus 2 Controller",
        description = "STM32F hardware I2C support.\n\n This bus cannot be remapped on F1/L1. Pins used are:\n"..
                      "SCL PB10\nSDA PB11\n",
        requires = { "HW_I2C2_STM32" },
        provides = { "DEV_TWI" },
        sources =
        {
            "cm3/dev/stm/stm32_twi2.c",
            "cm3/dev/stm/stm32_twi.c"
        },
        options =
        {
            {
                macro = "I2CBUS2_AS_DEFAULT",
                brief = "Use as default I2C",
                description = "Enabling this options reconfigures TwiXxx() Macros to use this bus.\n"..
                              "This is for compatibility with Nut/OS.",
                exclusivity = { "I2CBUS1_AS_DEFAULT", "I2CBUS2_AS_DEFAULT" },
                flavor = "booldata",
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS2_SCL_PIN",
                brief = "I2C2 SCL Pins",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C2 SCL Pin PB10(default) and on F2/F4 PF1 or PH4",
                type = "enumerated",
                choices = function() if c_is_provided("HW_MCU_STM32L1") then return { "10" } else return { "10", "1", "4" } end; end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS2_SDA_PIN",
                brief = "I2C2 SDA Pins",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C2 SDA Pin PB11(dafault) and on F2/F4 PF0 or PH5",
                type = "enumerated",
                choices = function() if c_is_provided("HW_MCU_STM32L1") then return { "11" } else return { "11", "0", "5" } end; end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS2_SMBA_PIN",
                brief = "I2C2 SMBA Pins",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C2 SMBA Pin PB12(default) and on F2/F4 PF2 or PH6",
                type = "enumerated",
                choices = function() if c_is_provided("HW_MCU_STM32L1") then return { "12" } else return { "12", "2", "6" } end; end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS2_DEFAULT_SPEED",
                brief = "Default Speed",
                description = "Default speed for this bus. Different speeds can be set by software.\n",
                default = "100",
                type = "boolean",
                choices = { "75", "100", "400" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS2_USE_DMA",
                brief = "Use DMA with I2C2",
                description = "Enabling this options configures I2C bus to use DMA.\n"..
                              "This should be default for STM32F10xxB/8 devices according Errata Sheet 14574 Rev.9.\n"..
                              "to overcome the mentioned limitations.",
                flavor = "booldata",
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS2_MODE_SMBUS",
                brief = "Enable SMBUS",
                description = "Enabling this options adds SM-Bus functions and initializes the Alert line GPIO.\n"..
                              "For this bus SMBA is PB12.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "booldata",
                file = "include/cfg/twi.h"
            }
        }
    },

    --
    -- STM32F I2C V1 Bus 1 Configuration
    --
    {
        name = "nutarch_cm3_stm32_i2c1_v1",
        brief = "STM32 I2C Bus 1 on F1/2/4 and L1",
        description = "STM32F hardware I2C V1 support.",
        requires = { "HW_I2C1_STM32" },
        provides = { "I2CBUS_CONTROLLER" },
        sources =
        {
            "cm3/dev/stm/stm32_i2cbus_v1.c",
        },
        options =
        {
            {
                macro = "I2C1_REMAP",
                brief = "Use Alternate Pins for I2C1",
                description = "Leaving this option unchecked, the driver will initialize the standard Pins.\n\n"..
                              "Pin Mapping is:\n  SDA PB7\n SCL PB6 \n SMBA PB5"..
                              "Pin Mapping remapped is:\n SDA PB9\n SCL PB8\n SMBA PB5",
                requires = { "HW_GPIO_STM32V1" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "I2C1_SDA_PIN",
                brief = "I2C1 SDA Pins",
                description = "Choose I2C1 SDA Pin PB7 (Default) or PB9",
                requires = { "HW_GPIO_STM32V2" },
                default = "7",
                type = "enumerated",
                choices = { "7", "9" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C1_SCL_PIN",
                brief = "I2C1 SCL Pins",
                description = "Choose I2C1 SCL Pin PB6 (Default) or PB8",
                requires = { "HW_GPIO_STM32V2" },
                default = "6",
                type = "enumerated",
                choices = { "6", "8" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C1_SMBA_PIN",
                brief = "I2C1 SCL Pins",
                description = "Choose I2C1 SMBA -1 (default) to not use SMBA or Pin PB5",
                default = "-1",
                type = "enumerated",
                choices = { "-1", "5" },
                file = "include/cfg/twi.h"
            }
         }
    },
    --
    -- STM32F I2C Bus 2 Configuration
    --
    {
        name = "nutarch_cm3_stm32_i2c2_v1",
        brief = "STM32 I2C Bus 1 on F3",
        description = "STM32F hardware I2C V1 support.",
        requires = { "HW_I2C2_STM32" },
        provides = { "I2CBUS_CONTROLLER" },
        sources =
        {
            "cm3/dev/stm/stm32_i2cbus_v1.c",
        },
        options =
        {
            {
                macro = "I2C2_SDA_PIN",
                brief = "I2C2 SDA Pins",
                description = "Choose I2C2 SDA Pin PB11(Default) or PF0 or PH5",
                requires = { "HW_GPIO_STM32V2" },
                default = "11",
                type = "enumerated",
                choices = { "15", "0" , "5" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C2_SCL_PIN",
                brief = "I2C2 SCL Pins",
                description = "Choose I2C2 SCL Pin PB10 (Default) or PF1 or PH4",
                requires = { "HW_GPIO_STM32V2" },
                default = "10",
                type = "enumerated",
                choices = { "11", "1", "4" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C2_SMBA_PIN",
                brief = "I2C2 SCL Pins",
                description = "Choose I2C2 SMBA Pin -1 (default) to not use SMBA or PB12 or PF2 or PH6",
                requires = { "HW_GPIO_STM32V2" },
                default = "-1",
                type = "enumerated",
                choices = { "-1", "12" , "2" , "6" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C2_SMBA_F1PIN",
                brief = "I2C2 SMBA Pins on F1",
                description = "F1: Choose I2C2 SMBA Pin -1 (default) to not use SMBA or PB12",
                requires = { "HW_GPIO_STM32V1" },
                default = "-1",
                type = "enumerated",
                choices = { "-1", "11" },
                file = "include/cfg/twi.h"
            }
         }
    },
    --
    -- STM32F I2C Bus 3 Configuration
    --
    {
        name = "nutarch_cm3_stm32_i2c3_v1",
        brief = "STM32 I2C Bus 3",
        description = "STM32F hardware I2C V1 support.",
        requires = { "HW_I2C3_STM32" },
        provides = { "I2CBUS_CONTROLLER" },
        sources =
        {
            "cm3/dev/stm/stm32_i2cbus_v1.c",
        },
        options =
        {
            {
                macro = "I2C3_SDA_PIN",
                brief = "I2C3 SDA Pins",
                description = "Choose I2C3 SDA Pin PC9(Default) or PH8",
                requires = { "HW_GPIO_STM32V2" },
                default = "9",
                type = "enumerated",
                choices = { "9", "8" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C3_SCL_PIN",
                brief = "I2C3 SCL Pins",
                description = "Choose I2C3 SCL Pin PA8 (Default) or PH7",
                requires = { "HW_GPIO_STM32V2" },
                default = "8",
                type = "enumerated",
                choices = { "8", "7" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C3_SMBA_PIN",
                brief = "I2C3 SCL Pins",
                description = "Choose I2C3 SMBA Pin -1 (default) to not use SMBA or PA9 or PH9(809)",
                requires = { "HW_GPIO_STM32V2" },
                default = "-1",
                type = "enumerated",
                choices = { "-1", "9" , "809" },
                file = "include/cfg/twi.h"
            }
         }
    },
    --
    --
    -- STM32F I2C V2 Bus 1 Configuration
    --
    {
        name = "nutarch_cm3_stm32_i2c1_v2",
        brief = "STM32 I2C Bus 1 on F3",
        description = "STM32F hardware I2C V2 support.",
        requires = { "HW_I2C1_STM32V2" },
        provides = { "I2CBUS_CONTROLLER" },
        sources =
        {
            "cm3/dev/stm/stm32_i2cbus_v2.c",
        },
        options =
        {
            {
                macro = "I2C1_SDA_PIN",
                brief = "I2C1 SDA Pins",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C1 SDA Pin PB7 (Default) or PB9 or PA14",
                default = "7",
                type = "enumerated",
                choices = { "7", "9", "14" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C1_SCL_PIN",
                brief = "I2C1 SCL Pins",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C1 SCL Pin PB6 (Default) or PB8",
                default = "6",
                type = "enumerated",
                choices = { "6", "8" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C1_SMBA_PIN",
                brief = "I2C1 SCL Pins",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C1 SMBA -1 (default) to not use SMBA or Pin PB5",
                default = "-1",
                type = "enumerated",
                choices = { "-1", "5" },
                file = "include/cfg/twi.h"
            }
         }
    },
     --
    -- STM32F I2C V2 Bus 2 Configuration
    --
    {
        name = "nutarch_cm3_stm32_i2c2_v2",
        brief = "STM32 I2C Bus 1 on F3",
        description = "STM32F hardware I2C V2 support.",
        requires = { "HW_I2C2_STM32V2" },
        provides = { "I2CBUS_CONTROLLER" },
        sources =
        {
            "cm3/dev/stm/stm32_i2cbus_v2.c",
        },
        options =
        {
            {
                macro = "I2C2_SDA_PIN",
                brief = "I2C2 SDA Pins",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C2 SDA Pin PA10(Default) or PF0",
                default = "10",
                type = "enumerated",
                choices = { "10", "0" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C2_SCL_PIN",
                brief = "I2C2 SCL Pins",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C2 SCL Pin PA9 (Default) or PF1 or PF6",
                default = "11",
                type = "enumerated",
                choices = { "11", "1", "6" },
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C2_SMBA_PIN",
                brief = "I2C2 SCL Pins",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C2 SMBA Pin -1 (default) to not use SMBA or PA8 or PB12",
                default = "-1",
                type = "enumerated",
                choices = { "-1", "8" , "12"},
                file = "include/cfg/twi.h"
            }
         }
    },
    --
    -- STM32F CAN Configuration
    --
    {
        name = "nutarch_cm3_stm32f_can1",
        brief = "STM32F CAN 1",
        description = "CAN support for STM32F.\n\n"..
                      "On some devices this bus shares pins and interrupts with USB bus!\n"..
                      "You cannot use them together!",
        requires = { "HW_CAN1_STM32" },
        provides = { "DEV_CANBUS" },
        sources =
        {
            "cm3/dev/stm/stm32_can.c",
            "cm3/dev/stm/stm32_can1.c",
        },
        options =
        {
            {
                macro = "CANBUS1_REMAP_CAN",
                brief = "Use Alternate Pins",
                description = "This CAN bus can be internally connected to several sets of pins.\n\n"..
                              "Choice 0: CAN_RX PA11 CAN_TX PA12\n"..
                              "Choice 1: CAN_RX PB8  CAN_TX PB9\n"..
                              "Choice 2: CAN_RX PD0  CAN_TX PD1\n",
                requires = { "HW_GPIO_STM32V1" },
                default = "0",
                type = "enumerated",
                choices = { "0", "1", "2" },
                file = "include/cfg/can_dev.h"
            },
            {
                macro = "CANBUS1_TX_PIN",
                brief = "CAN1 TX Pin selection",
                description = "Choose CAN1 TX Pin, Default: PA12",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "12", "9", "1" },
                file = "include/cfg/can_dev.h"
            },
            {
                macro = "CANBUS1_RX_PIN",
                brief = "CAN1 RX Pin selection",
                description = "Choose CAN1 RX Pin, Default: PA11",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "11", "8", "0" },
                file = "include/cfg/can_dev.h"
            }
        }
    },
    {
        name = "nutarch_cm3_stm32f_can2",
        brief = "STM32F CAN 2",
        description = "CAN Bus 2 support for STM32F.",
        requires = { "HW_CAN2_STM32" },
        provides = { "DEV_CANBUS" },
        sources =
        {
            "cm3/dev/stm/stm32_can.c",
            "cm3/dev/stm/stm32_can2.c",
        },
        options =
        {
            {
                macro = "CANBUS2_REMAP_CAN",
                brief = "Use Alternate Pins",
                description = "This CAN bus can be internally connected to two sets of pins.\n\n"..
                              "Choice 0: CAN_RX PB12 CAN_TX PB13\n"..
                              "Choice 1: CAN_RX PB5  CAN_TX PB6\n",
                requires = { "HW_GPIO_STM32V1" },
                default = "0",
                type = "enumerated",
                choices = { "0", "1" },
                file = "include/cfg/can_dev.h"
            },
            {
                macro = "CANBUS2_TX_PIN",
                brief = "CAN2 TX Pin selection",
                description = "Choose CAN2 TX Pin, Default: PB13",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "13", "6" },
                file = "include/cfg/can_dev.h"
            },
            {
                macro = "CANBUS2_RX_PIN",
                brief = "CAN2 RX Pin selection",
                description = "Choose CAN2 RX Pin, Default: PB12",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "12", "5" },
                file = "include/cfg/can_dev.h"
            }
        }
    },

    --
    -- STM32F CRC Configuration
    --
    {
        name = "nutarch_cm3_stm32f_crc",
        brief = "STM32F CRC32 Generator",
        description = "hardware CRC32 Generator support for STM32F.",
        requires = { "HW_CRC32_STM32" },
        provides = { "DEV_CRC" },
        sources = { "cm3/dev/stm/stm32_crc.c" },
    },

    --
    -- STM32F ADC Configuration
    --
    {
        name = "nutarch_cm3_stm32f_adc",
        brief = "STM32F ADC Channel 1,2",
        description = "ADC channel 1 & 2 support for STM32F.",
        requires = { "HW_ADCF1_12_STM32" },
        provides = { "DEV_ADC" },
--        sources = { "cm3/dev/stm/stm32f1_adc.c" },
    },

    --
    -- STM32F MMC Interface
    --
    {
        name = "nutarch_cm3_stm32f_sdio",
        brief = "STM32F SD-Card Access",
        description = "Low level SD-Card interface for STM32F.\n\n"..
                      "Under development.",
        requires = { "HW_SPI_STM32" },
        provides = { "DEV_MMCLL" },
--        sources = { "cm3/dev/stm/stm32_sdio.c" },
    },

    --
    -- STM32F SPI1 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spibus1",
        brief = "STM32F SPI1 Bus Controller",
        description = "Supports up to two bus controllers.\n\n"..
                      "This early release had been tested on the STM32F103 and STM32F107 only.",
        requires = { "HW_SPI1_STM32" },
        provides = { "SPIBUS_CONTROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi1.c" },
        options =
        {
            {
                macro = "SPIBUS1_REMAP_SPI",
                brief = "Use Alternate Pins",
                description = "Leaving this option unchecked, the driver will initialize the standard Pins.\n\n"..
                              "Pin Mapping is:\n SPI1_NSS PA4\n SPI1_SCK PA5\n SPI1_MISO PA6\n SPI1_MOSI PA7\n"..
                              "Enabling the option alternate port pins are used:\n\n"..
                              "Pin Mapping is:\n SPI1_NSS PA15\n SPI1_SCK PB3\n SPI1_MISO PB4\n SPI1_MOSI PB5\n" ..
                                      "For L1/F2/F4 Pins may also get assigned individually",
                flavor = "booldata",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_CS0_PORT",
                brief = "SPI1 CS0 Port",
                description = "Choice of SPI1 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_CS0_PIN",
                brief = "SPI1 CS0 Pins",
                description = "Choice of SPI1 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_CS1_PORT",
                brief = "SPI1 CS1 Port",
                description = "Choice of SPI1 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_CS1_PIN",
                brief = "SPI1 CS1 Pins",
                description = "Choice of SPI1 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_CS2_PORT",
                brief = "SPI1 CS2 Port",
                description = "Choice of SPI1 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_CS2_PIN",
                brief = "SPI1 CS2 Pins",
                description = "Choice of SPI1 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_CS3_PORT",
                brief = "SPI1 CS3 Port",
                description = "Choice of SPI1 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_CS3_PIN",
                brief = "SPI1 CS3 Pins",
                description = "Choice of SPI1 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_SCK_PIN",
                brief = "SPI1 SCK Pins",
                description = "Choice of SPI1 SCK Pins: PA5(default) and PB3 and on L1 PE13",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() if c_is_provided("HW_MCU_STM32L1") then return { "5", "3", "13" } else return { "5", "3" } end; end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_MISO_PIN",
                brief = "SPI1 MISO Pins",
                description = "Choice of SPI1 MISO Pins: PA6(default) and PB4 and on L1 PE14",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() if c_is_provided("HW_MCU_STM32L1") then return { "6", "4", "14" } else return { "6", "4" } end; end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_MOSI_PIN",
                brief = "SPI1 MOSI Pins",
                description = "Choice of SPI1 MOSI Pins: PA7(default) and PB5 and on L1 PE15",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() if c_is_provided("HW_MCU_STM32L1") then return { "7", "5", "15" } else return { "7", "7" } end;end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_I2S_MODE",
                brief = "I2S Mode (First Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_POLLING_MODE",
                brief = "Polling Mode (First Controller)",
                description = "If enabled, the controller will use polling mode. By default "..
                              "interrupt mode is used.",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_DOUBLE_BUFFER",
                brief = "DMA Mode (First Controller)",
                description = "If enabled, the controller will use DMA mode.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
        },
    },

    --
    -- STM32F SPI2 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spibus2",
        brief = "STM32F SPI2 Bus Controller",
        description = "Supports up to two bus controllers.\n\n"..
                      "This early release had been tested on the STM32F103 and STM32F107 only.",
        requires = { "HW_SPI2_STM32" },
        provides = { "SPIBUS_CONTROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi2.c" },
        options =
        {
            {
                macro = "SPIBUS2_REMAP_SPI",
                brief = "Use Alternate Pins",
                description = "Leaving this option unchecked, the driver will initialize the standard Pins.\n\n"..
                              "Pin Mapping is:\n SPI2_NSS PA4\n SPI2_SCK PA5\n SPI2_MISO PA6\n SPI2_MOSI PA7\n"..
                              "Enabling the option alternate port pins are used:\n\n"..
                              "Pin Mapping is:\n SPI2_NSS PA15\n SPI2_SCK PB3\n SPI2_MISO PB4\n SPI2_MOSI PB5\n" ..
                                      "For L1/F2/F4 Pins may also get assigned individually",
                flavor = "booldata",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_CS0_PORT",
                brief = "SPI2 CS0 Port",
                description = "Choice of SPI2 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_CS0_PIN",
                brief = "SPI2 CS0 Pins",
                description = "Choice of SPI2 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_CS1_PORT",
                brief = "SPI2 CS1 Port",
                description = "Choice of SPI2 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_CS1_PIN",
                brief = "SPI2 CS1 Pins",
                description = "Choice of SPI2 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_CS2_PORT",
                brief = "SPI2 CS2 Port",
                description = "Choice of SPI2 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_CS2_PIN",
                brief = "SPI2 CS2 Pins",
                description = "Choice of SPI2 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_CS3_PORT",
                brief = "SPI2 CS3 Port",
                description = "Choice of SPI2 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_CS3_PIN",
                brief = "SPI2 CS3 Pins",
                description = "Choice of SPI2 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_SCK_PIN",
                brief = "SPI2 SCK Pins",
                description = "Choice of SPI2 SCK Pins: PB13(default) and on F2/F4 PB10 and PI1 and on L1 PD1",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() if c_is_provided("HW_MCU_STM32L1") then return { "13", "1" } else return { "13", "1", "901" } end; end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_MISO_PIN",
                brief = "SPI2 MISO Pins",
                description = "Choice of SPI2 MISO Pins: PB14(default) and on F2/F4  PC2 and PI2 and on L1 PD2",
                requires = { "HW_GPIO_STM32V2"  },
                type = "enumerated",
                choices = function() if c_is_provided("HW_MCU_STM32L1") then return { "14", "2" } else return { "14", "2", "902" } end; end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_MOSI_PIN",
                brief = "SPI2 MOSI Pins",
                description = "Choice of SPI2 MOSI Pins: PB15(default) and on F2/F4  PC3 and PI3 and on L1 PD3",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() if c_is_provided("HW_MCU_STM32L1") then return { "15", "3" } else return { "15", "3", "903" } end; end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_I2S_MODE",
                brief = "I2S Mode (Second Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_POLLING_MODE",
                brief = "Polling Mode (Second Controller)",
                description = "If enabled, the controller will use polling mode. By default "..
                              "interrupt mode is used.",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_DOUBLE_BUFFER",
                brief = "PDC Mode (Second Controller)",
                description = "If enabled, the controller will use PDC mode.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
        }
    },

    --
    -- STM32F SPI3 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spibus3",
        brief = "STM32F SPI3 Bus Controller",
        description = "Supports up to two bus controllers.\n\n"..
                      "This early release had been tested on the STM32F103 and STM32F107 only.",
        requires = { "HW_SPI3_STM32" },
        provides = { "SPIBUS_CONTROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi3.c" },
        options =
        {
            {
                macro = "SPIBUS3_REMAP_SPI",
                brief = "Use Alternate Pins",
                description = "Leaving this option unchecked, the driver will initialize the standard Pins.\n\n"..
                              "Pin Mapping is:\n SPI3_NSS PA4\n SPI3_SCK PA5\n SPI3_MISO PA6\n SPI3_MOSI PA7\n"..
                              "Enabling the option alternate port pins are used:\n\n"..
                              "Pin Mapping is:\n SPI3_NSS PA15\n SPI3_SCK PB3\n SPI3_MISO PB4\n SPI3_MOSI PB5\n" ..
                                      "For L1/F2/F4 Pins may also get assigned individually",
                flavor = "booldata",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_CS0_PORT",
                brief = "SPI3 CS0 Port",
                description = "Choice of SPI3 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_CS0_PIN",
                brief = "SPI3 CS0 Pins",
                description = "Choice of SPI3 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_CS1_PORT",
                brief = "SPI3 CS1 Port",
                description = "Choice of SPI3 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_CS1_PIN",
                brief = "SPI3 CS1 Pins",
                description = "Choice of SPI3 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_CS2_PORT",
                brief = "SPI3 CS2 Port",
                description = "Choice of SPI3 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_CS2_PIN",
                brief = "SPI3 CS2 Pins",
                description = "Choice of SPI3 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_CS3_PORT",
                brief = "SPI3 CS3 Port",
                description = "Choice of SPI3 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_CS3_PIN",
                brief = "SPI3 CS3 Pins",
                description = "Choice of SPI3 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_SCK_PIN",
                brief = "SPI3 SCK Pins",
                description = "Choice of SPI3 SCK Pins: PB3(default) and PC10",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "3", "10" },
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_MISO_PIN",
                brief = "SPI3 MISO Pins",
                description = "Choice of SPI3 MISO Pins: PB4(default) and PC11",
                requires = { "HW_GPIO_STM32V2"  },
                type = "enumerated",
                choices = { "4", "11" },
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_MOSI_PIN",
                brief = "SPI3 MOSI Pins",
                description = "Choice of SPI3 MOSI Pins: PB5(default) PC12",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "5", "12" },
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_CS_PORT",
                brief = "SPI3 CS Port",
                description = "Choice of SPI3 CS Pins on GPIO BANK",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_CS_PIN",
                brief = "SPI3 CS Pins",
                description = "Choice of SPI3 CS Pins on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_I2S_MODE",
                brief = "I2S Mode (Third Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_POLLING_MODE",
                brief = "Polling Mode (Second Controller)",
                description = "If enabled, the controller will use polling mode. By default "..
                              "interrupt mode is used.",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_DOUBLE_BUFFER",
                brief = "PDC Mode (Second Controller)",
                description = "If enabled, the controller will use PDC mode.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
        },
    },

    --
    -- STM32F SPI4 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spibus4",
        brief = "STM32F SPI4 Bus Controller",
        description = "STM32F SPI4 Bus Controller",
        requires = { "HW_SPI4_STM32" },
        provides = { "SPIBUS_CONTROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi4.c" },
        options =
        {
-- Not available on F1, so no global remap
            {
                macro = "SPIBUS4_CS0_PORT",
                brief = "SPI4 CS0 Port",
                description = "Choice of SPI4 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_CS0_PIN",
                brief = "SPI4 CS0 Pins",
                description = "Choice of SPI4 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_CS1_PORT",
                brief = "SPI4 CS1 Port",
                description = "Choice of SPI4 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_CS1_PIN",
                brief = "SPI4 CS1 Pins",
                description = "Choice of SPI4 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_CS2_PORT",
                brief = "SPI4 CS2 Port",
                description = "Choice of SPI4 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_CS2_PIN",
                brief = "SPI4 CS2 Pins",
                description = "Choice of SPI4 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_CS3_PORT",
                brief = "SPI4 CS3 Port",
                description = "Choice of SPI4 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_CS3_PIN",
                brief = "SPI4 CS3 Pins",
                description = "Choice of SPI4 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_SCK_PIN",
                brief = "SPI4 SCK Pins",
                description = "Choice of SPI4 SCK Pins: PE2(default) and PE11",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "2", "11" },
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_MISO_PIN",
                brief = "SPI4 MISO Pins",
                description = "Choice of SPI4 MISO Pins: PE5(default) and PE13",
                requires = { "HW_GPIO_STM32V2"  },
                type = "enumerated",
                choices = { "5", "13" },
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_MOSI_PIN",
                brief = "SPI4 MOSI Pins",
                description = "Choice of SPI4 MOSI Pins: PE6(default) and PE14",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "6", "14" },
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_CS_PORT",
                brief = "SPI4 CS Port",
                description = "Choice of SPI4 CS Pins on GPIO BANK",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_CS_PIN",
                brief = "SPI4 CS Pins",
                description = "Choice of SPI4 CS Pins on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_I2S_MODE",
                brief = "I2S Mode (Third Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_POLLING_MODE",
                brief = "Polling Mode (Second Controller)",
                description = "If enabled, the controller will use polling mode. By default "..
                              "interrupt mode is used.",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_DOUBLE_BUFFER",
                brief = "PDC Mode (Second Controller)",
                description = "If enabled, the controller will use PDC mode.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
        },
    },

    --
    -- STM32F SPI5 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spibus5",
        brief = "STM32F SPI5 Bus Controller",
        description = "STM32F SPI5 Bus Controller",
        requires = { "HW_SPI5_STM32" },
        provides = { "SPIBUS_CONTROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi5.c" },
        options =
        {
-- Not available on F1, so no global remap
            {
                macro = "SPIBUS5_CS0_PORT",
                brief = "SPI5 CS0 Port",
                description = "Choice of SPI5 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_CS0_PIN",
                brief = "SPI5 CS0 Pins",
                description = "Choice of SPI5 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_CS1_PORT",
                brief = "SPI5 CS1 Port",
                description = "Choice of SPI5 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_CS1_PIN",
                brief = "SPI5 CS1 Pins",
                description = "Choice of SPI5 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_CS2_PORT",
                brief = "SPI5 CS2 Port",
                description = "Choice of SPI5 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_CS2_PIN",
                brief = "SPI5 CS2 Pins",
                description = "Choice of SPI5 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_CS3_PORT",
                brief = "SPI5 CS3 Port",
                description = "Choice of SPI5 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_CS3_PIN",
                brief = "SPI5 CS3 Pins",
                description = "Choice of SPI5 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_SCK_PIN",
                brief = "SPI5 SCK Pins",
                description = "Choice of SPI5 SCK Pins: PF7(default) and PH6",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "7", "6" },
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_MISO_PIN",
                brief = "SPI5 MISO Pins",
                description = "Choice of SPI5 MISO Pins: PF8(default) and PH7",
                requires = { "HW_GPIO_STM32V2"  },
                type = "enumerated",
                choices = { "8", "7" },
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_MOSI_PIN",
                brief = "SPI5 MOSI Pins",
                description = "Choice of SPI5 MOSI Pins: PF9(default) and PF11",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = { "9", "11" },
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_CS_PORT",
                brief = "SPI5 CS Port",
                description = "Choice of SPI5 CS Pins on GPIO BANK",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_CS_PIN",
                brief = "SPI5 CS Pins",
                description = "Choice of SPI5 CS Pins on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_I2S_MODE",
                brief = "I2S Mode (Third Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_POLLING_MODE",
                brief = "Polling Mode (Second Controller)",
                description = "If enabled, the controller will use polling mode. By default "..
                              "interrupt mode is used.",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_DOUBLE_BUFFER",
                brief = "PDC Mode (Second Controller)",
                description = "If enabled, the controller will use PDC mode.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
        },
    },

    --
    -- STM32F SPI6 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spibus6",
        brief = "STM32F SPI6 Bus Controller",
        description = "STM32F SPI6 Bus Controller",
        requires = { "HW_SPI6_STM32" },
        provides = { "SPIBUS_CONTROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi6.c" },
        options =
        {
-- Not available on F1, so no global remap
            {
                macro = "SPIBUS6_CS0_PORT",
                brief = "SPI6 CS0 Port",
                description = "Choice of SPI6 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS6_CS0_PIN",
                brief = "SPI6 CS0 Pins",
                description = "Choice of SPI6 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS6_CS1_PORT",
                brief = "SPI6 CS1 Port",
                description = "Choice of SPI6 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS6_CS1_PIN",
                brief = "SPI6 CS1 Pins",
                description = "Choice of SPI6 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS6_CS2_PORT",
                brief = "SPI6 CS2 Port",
                description = "Choice of SPI6 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS6_CS2_PIN",
                brief = "SPI6 CS2 Pins",
                description = "Choice of SPI6 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS6_CS3_PORT",
                brief = "SPI6 CS3 Port",
                description = "Choice of SPI6 Node0 CS on GPIO BANK",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS6_CS3_PIN",
                brief = "SPI6 CS3 Pins",
                description = "Choice of SPI6 Node 0 CS on GPIO PIN",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS6_I2S_MODE",
                brief = "I2S Mode (Third Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS6_POLLING_MODE",
                brief = "Polling Mode (Second Controller)",
                description = "If enabled, the controller will use polling mode. By default "..
                              "interrupt mode is used.",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_DOUBLE_BUFFER",
                brief = "PDC Mode (Second Controller)",
                description = "If enabled, the controller will use PDC mode.\n\n"..
                              "Under development.",
--                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
        },
    },

    --
    -- STM32F LCD Interface Drivers
    --
    {
        name = "nutarch_cm3_stm32f_chlcd",
        brief = "Character LCD Driver (STM32F)",
        description = "Parallel or serial connected displays like\n"..
                      "HD44780, KS0066, KS0073 and others.\n",
        requires = { "HW_MCU_STM32" },
        provides = { "DEV_FILE", "DEV_WRITE" },
--        sources = { "arm/dev/stm32_charlcd.c" },
        options =
        {
            --
            --  Define selection of supported driver chips
            --
            {
                macro = "LCD_HD44780",
                brief = "HD44780 Driver",
                description = "Tested on the EIR 1.0 with 2x16 and 4x20 character LCD.",
                exclusivity = { "LCD_HD44780", "LCD_KS0066", "LCD_KS0073", "LCD_ST7036" },
                flavor = "booldata",
                provides = { "LCD_GPIO" },
                file = "include/cfg/lcd.h",
            },
            {
                macro = "LCD_KS0066",
                brief = "KS0066 Driver",
                description = "Currently not tested.",
                exclusivity = { "LCD_HD44780", "LCD_KS0066", "LCD_KS0073", "LCD_ST7036" },
                flavor = "booldata",
                provides = { "LCD_GPIO" },
                file = "include/cfg/lcd.h",
            },
            {
                macro = "LCD_KS0073",
                brief = "KS0073 Driver",
                description = "Currently not tested.",
                exclusivity = { "LCD_HD44780", "LCD_KS0066", "LCD_KS0073", "LCD_ST7036" },
                flavor = "booldata",
                provides = { "LCD_GPIO" },
                file = "include/cfg/lcd.h",
            },
            {
                macro = "LCD_ST7036",
                brief = "ST7036 Driver",
                description = "Serial connected display via SPI.\nCurrently not tested.",
                exclusivity = { "LCD_HD44780", "LCD_KS0066", "LCD_KS0073", "LCD_ST7036" },
                flavor = "booldata",
                provides = { "LCD_SPI" },
                file = "include/cfg/lcd.h",
            },
            --
            --  Support for timing related parameters
            --
            {
                macro = "LCD_ROWS",
                brief = "Rows",
                description = "The number of available display rows, either 1, 2 or 4.",
                default = "2",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_COLS",
                brief = "Columns",
                description = "The number of available display colums, either 8, 16, 20 or 40.",
                default = "16",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_PW_EH",
                brief = "Enable Pulse Stretch",
                description = "Set a value here if the system is to fast to guarantee a minimum enable high time:\n"..
                              "For HD44780 with Vcc=5.0V this is 230ns\n"..
                              "For HD44780 with Vcc=3.3V this is 500ns\n\n"..
                              "This value uses a NOP-Loop to stretch the enable Pulse and is directly dependand on the "..
                              "systems CPU speed. It also may differ for other chips.\n\n"..
                              "For a 5V driven display connected to EIR 1.0, this value can be left empty while it must "..
                              "be set to about 5 for a 3.3V driven HD44780 LCD.",
                requires = { "LCD_GPIO" },
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_E2E_DLY",
                brief = "Enable to Enable Delay",
                description = "Time for Enable to Enable delay in µs. This is the timespan between two\n"..
                              "consecutive accesses of the 4-bit or 8-bit bus.\n For SPI-bus driven chips, this "..
                              "is the /CS to /CS delay. This function uses NutMicroDelay()",
                default = "80",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_SLEEP_DLY",
                brief = "Sleep after Data",
                description = "Enable this option on a timing critical system.\n"..
                              "Instead of cosecutive accessing the display this option will enable a NutSleep(1)\n"..
                              "After each data or command written out to the LCD.",
                flavor = "boolean",
                file = "include/cfg/lcd.h"
            },
            --
            --  Selection of parallel interface parameters
            --
            {
                macro = "LCD_IF_8BIT",
                brief = "8-Bit Mode",
                description = "Select parallel bus width is 8 bit.\n"..
                              "Splitting single bus lines accross ports is not "..
                              "supported for data bit lines.\n"..
                              "In 8 bit mode all data lines have to be aligned "..
                              "in one row.\n"..
                              "This option is actually not supported in this driver.",
                requires = { "LCD_GPIO" },
                flavor = "booldata",
                exclusivity = { "LCD_IF_8BIT", "LCD_IF_4BIT" },
                provides = { "LCD_IF_8BIT" },
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_IF_4BIT",
                brief = "Use 4-Bit Mode",
                description = "Select parallel bus width is 4 bit."..
                              "Splitting single bus lines accross ports is not"..
                              "supported for data bit lines.",
                requires = { "LCD_GPIO" },
                flavor = "booldata",
                exclusivity = { "LCD_IF_8BIT", "LCD_IF_4BIT" },
                provides = { "LCD_IF_4BIT" },
                file = "include/cfg/lcd.h"
            },
            --
            --  Selection of parallel interface parameters
            --
            {
                macro = "LCD_DATA_PIO_ID",
                brief = "Port of LCD data pins",
                requires = { "LCD_GPIO" },
                description = "Port of the below defined data pins. Valid for "..
                              "both, single bit definitions or LSB.",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_DATA_LSB",
                brief = "Least Significant Data Bit",
                requires = { "LCD_GPIO" },
                description = "Bit number of the least significant data bit. The remaining "..
                              "data bits must be connected to the following port bits.",
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_DATA_BIT0",
                brief = "Data Bit 0",
                requires = { "LCD_GPIO", "LCD_IF_4BIT" },
                description = "Port bit connected to LCD data bit 0.\n"..
                              "Not used if LCD_DATA_LSB is defined.\n",
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_DATA_BIT1",
                brief = "Data Bit 1",
                requires = { "LCD_GPIO", "LCD_IF_4BIT" },
                type = "enumerated",
                requires = { "LCD_IF_4BIT" },
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_DATA_BIT2",
                brief = "Data Bit 2",
                requires = { "LCD_GPIO", "LCD_IF_4BIT" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_DATA_BIT3",
                brief = "Data Bit 3",
                requires = { "LCD_GPIO", "LCD_IF_4BIT" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },
            --
            --  Selection of serial interface parameters
            --
            {
                macro = "LCD_CLK_PIO_ID",
                brief = "SPI Clock Port",
                description = "Port ID of the SPI clock line.\n",
                requires = { "LCD_SPI" },
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_CLK_BIT",
                brief = "SPI Clock Bit",
                description = "Port bit of the SPI clock line. ",
                requires = { "LCD_SPI" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_MOSI_PIO_ID",
                brief = "SPI Clock Port",
                description = "Port ID of the SPI MOSI line.\n",
                requires = { "LCD_SPI" },
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_MOSI_BIT",
                brief = "SPI MOSI Bit",
                description = "Port bit of the SPI clock line. ",
                requires = { "LCD_SPI" },
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },

            --
            --  Selection of display control lines
            --
            {
                macro = "LCD_EN_PIO_ID",
                brief = "LCD Enable Port",
                description = "Port ID of the Enable line.\n"..
                              "For parallel chips this is the active high enable signal.\n"..
                              "For serial chips this is the active low chip select line.",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_EN_BIT",
                brief = "Enable Bit",
                description = "Port bit of the LCD enable line. "..
                              "This line must be exclusively reserved."..
                              "For parallel chips this is the active high enable signal.\n"..
                              "For serial chips this is the active low chip select line.",
                default = "4",
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_RS_PIO_ID",
                brief = "LCD Register Select Port",
                description = "Port ID of the Register Select line.",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_RS_BIT",
                brief = "Register Select Bit",
                description = "Port bit of the LCD register select line. "..
                              "May be shared.",
                default = "7",
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },
            --
            --  Selection of optional display control lines
            --
            {
                macro = "LCD_RW_PIO_ID",
                brief = "LCD Read/Write Select Port",
                description = "Optional port base of the Read/Write Select line."..
                              "Driver supports display confiurations with R/W tied to 0.\n"..
                              "For this, leave this entry blank.",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_RW_BIT",
                brief = "Read/Write Bit",
                description = "Optional port ID of the Read/Write Select line."..
                              "Driver supports display confiurations with R/W tied to 0.\n"..
                              "For this, leave this entry blank.",
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_EN2_PIO_ID",
                brief = "LCD Enable Port 2",
                requires = { "LCD_GPIO" },
                description = "Optional port base of the 2nd Enable line. "..
                              "This line must be exclusively reserved.\n"..
                              "This is only used on large 4x40 character displays.",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_EN2_BIT",
                brief = "LCD Enable-2 Bit",
                requires = { "LCD_GPIO" },
                description = "Optional port bit of the 2nd LCD enable line. "..
                              "This is only used on large 4x40 character displays.\n"..
                              "This line must be exclusively reserved.",
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_RST_PIO_ID",
                brief = "LCD Reset Port",
                description = "Optional port base of the LCD Reset line. "..
                              "This line must be exclusively reserved.\n"..
                              "Some LCD Drivers support an optional reset line.\n"..
                              "This is currently not supported by this driver!",
                type = "enumerated",
                choices = function() GetGpioBanks() end,
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_RST_BIT",
                brief = "LCD Reset Bit",
                description = "Optional port bit of the 2nd LCD enable line. "..
                              "This line must be exclusively reserved.\n"..
                              "Some LCD Drivers support an optional reset line.\n"..
                              "This is currently not supported by this driver!",
                type = "enumerated",
                choices = function() GetGpioBits() end,
                file = "include/cfg/lcd.h"
            },
        },
    },

    --
    -- STM32F I2S Audio Device Interface
    --
    {
        name = "nutarch_cm3_stm32f_tlv320",
        brief = "TLV320 DAC",
        requires = { "HW_EXT_CALYPSO" },
        provides = { "HW_AUDIO_DAC" },
        sources = { "arm/dev/tlv320dac.c" },
    },

    --
    -- STM32 PWMs using hardware timer
    --
    {
        name = "nutarch_cm3_stm32_pwm0",
        brief = "STM32 PWM Output 0",
        description = "STM32 PWM_Output 0.",
        sources = { "cm3/dev/stm/stm32_pwm_0.c" },
        options =
        {
            {
                macro = "STM32_PWM0_TIMER_ID",
                brief = "STM32 32Bit PWM0 Timer ID",
                description = "Select Timer for PWM0 output. Check for availability on selected device.",
                type = "enumerated",
                choices = { "1", "2", "3", "4", "5", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19" },
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM0_TIMER_CHANNEL",
                brief = "STM32 32Bit PWM0 Timer Channel",
                description = "Select Timer Channel for PWM0 output. Check for availability on selected device.",
                type = "enumerated",
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM0_PIN",
                brief = "PIN for STM32 PWM0 output",
                description = "Port bit used for PWM0. Only some pins are valid!",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM0_PORT",
                brief = "PORT for STM32 PWM0 output",
                description = "Port used for PWM0. Only some ports are valid!",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                file = "include/cfg/pwm.h",
            },
        }
    },
    {
        name = "nutarch_cm3_stm32_pwm1",
        brief = "STM32 PWM Output 1",
        description = "STM32 PWM_Output 1.",
        sources = { "cm3/dev/stm/stm32_pwm_1.c" },
        options =
        {
            {
                macro = "STM32_PWM1_TIMER_ID",
                brief = "STM32 32Bit PWM1 Timer ID",
                description = "Select Timer for PWM1 output. Check for availability on selected device.",
                type = "enumerated",
                choices = { "1", "2", "3", "4", "5", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19" },
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM1_TIMER_CHANNEL",
                brief = "STM32 32Bit PWM1 Timer Channel",
                description = "Select Timer Channel for PWM1 output. Check for availability on selected device.",
                type = "enumerated",
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM1_PIN",
                brief = "PIN for STM32 PWM1 output",
                description = "Port bit used for PWM1. Only some pins are valid!",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM1_PORT",
                brief = "PORT for STM32 PWM1 output",
                description = "Port used for PWM1. Only some ports are valid!",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                file = "include/cfg/pwm.h",
            },
        }
    },
    --
    -- STM32 OWI using hardware timer
    --
    {
        name = "nutarch_cm3_stm32_owi",
        brief = "STM32 OWI with hardware timer",
        description = "STM32 OWI with hardware timer, library compile time configured",
        provides = { "OWIBUS" },
        sources = { "cm3/dev/stm/owibus0stm32tim.c" },
        options =
        {
            {
                macro = "STM32TIM_OWI0_PIN",
                brief = "PIN for STM32 hardware timer OWI0 bus",
                description = "Port bit used for OWI Bus 0. Only some pins are valid!",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_PORT",
                brief = "PORT for STM32 hardware timer OWI0 bus",
                description = "Port used for OWI Bus 0. Only some ports are valid!",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_TIMER_ID",
                brief = "Timer for STM32 hardware timer OWI0 bus",
                description = "Timer used for OWI Bus 0. Requires at least dual channel!",
                type = "enumerated",
                choices = function() return GetStmTimers2ChIrq() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_CHANNEL",
                brief = "Channel for STM32 hardware timer OWI0 bus",
                description = "Channel used for STM32 hardware timer OWI0 bus. Check for Channel 3/4 availability!",
                type = "enumerated",
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_REMAP",
                brief = "REMAP for STM32 hardware timer OWI0 bus",
                requires = { "HW_MCU_STM32F10X" },
                description = "Give Remap used for STM32F1 hardware timer OWI0 bus if needed!",
                type = "interger",
                default = "0",
                file = "include/cfg/owi.h",
            },
        }
     },

    --
    -- STM32F1/L1 DMA Interface
    --
    {
        name = "nutarch_cm3_stm32f1_dma1",
        brief = "STM32F1/L1 DMA1 Controller",
        description = "Generic DMA Support for F1/L1",
        requires = { "HW_MCU_STM32", "HW_DMA1_STM32F1" },
        provides = { "HW_DMA1_F1" },
        sources = { "cm3/dev/stm/stm32f1_dma.c" }
    },
    {
        name = "nutarch_cm3_stm32f1_dma2",
        brief = "STM32F1/L1 DMA2 Controller",
        description = "Generic DMA Support for F1/L1",
        requires = { "HW_MCU_STM32", "HW_DMA2_STM32F1" },
        provides = { "HW_DMA2_F1" },
        sources = { "cm3/dev/stm/stm32f1_dma.c" }
    },

    --
    -- STM32F2/F4 DMA Interface
    --
    {
        name = "nutarch_cm3_stm32f2_dma1",
        brief = "STM32F2/F4 DMA1 Controller",
        description = "Generic DMA1 Support for F2/F4",
        requires = { "HW_MCU_STM32", "HW_DMA1_STM32F2" },
        provides = { "HW_DMA1_F2" },
        sources = { "cm3/dev/stm/stm32f2_dma.c"}
    },
    {
        name = "nutarch_cm3_stm32f2_dma2",
        brief = "STM32F2/F4 DMA2 Controller",
        description = "Generic DMA2 Support for F2/F4",
        requires = { "HW_MCU_STM32", "HW_DMA2_STM32F2" },
        provides = { "HW_DMA2_F2" },
        sources = { "cm3/dev/stm/stm32f2_dma.c" }
    },
    --
    -- STM32 Signal handler
    {
        name = "nutarch_cm3_stm32_sig",
        brief = "STM32 Signals ",
        description = "Helper to create the sig_DEVXX entries",
        sources = { "cm3/dev/stm/ih_stm32.c"},
        options =
        {
            {
                macro = "HW_RTC_STM32F1",
                brief = "RTC V1 Availability",
                description = "RTC V1 Availability",
                requires = { "HW_RTC_STM32F1" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_RTC_STM32_V2",
                brief = "RTC V2 Availability",
                description = "RTC V2 Availability",
                requires = { "HW_RTC_STM32_V2" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_DMA1_STM32F1",
                brief = "DMA1 Availability",
                description = "7 Channel DMA1 on F1/L1/F3",
                requires = { "HW_DMA1_STM32F1" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_DMA2_STM32F1",
                brief = "DMA2 Availability",
                description = "5 Channel DMA2 on F1/L1/F3",
                requires = { "HW_DMA2_STM32F1" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_USART2_STM32",
                brief = "USART2 Availability",
                description = "USART2 Availability",
                requires = { "HW_USART2_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_USART3_STM32",
                brief = "USART3 Availability",
                description = "USART3 Availability",
                requires = { "HW_USART3_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_UART4_STM32",
                brief = "UART4 Availability",
                description = "USART4 Availability",
                requires = { "HW_UART4_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_UART5_STM32",
                brief = "UART5 Availability",
                description = "USART5 Availability",
                requires = { "HW_UART5_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_USART6_STM32",
                brief = "USART6 Availability",
                description = "USART6 Availability",
                requires = { "HW_USART6_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_UART7_STM32",
                brief = "UART7 Availability",
                description = "UART7 Availability",
                requires = { "HW_UART7_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_UART8_STM32",
                brief = "UART8 Availability",
                description = "UART8 Availability",
                requires = { "HW_UART8_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_CAN1_STM32",
                brief = "CAN1 Availability",
                description = "CAN1 Availability",
                requires = { "HW_CAN1_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_USB_STM32",
                brief = "USB FS Availability",
                description = "USB Availability",
                requires = { "HW_USB_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM1_STM32",
                brief = "TIM1 Availability",
                description = "TIM1 Availability",
                requires = { "HW_TIM1_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM2_STM32",
                brief = "TIM2 Availability",
                description = "TIM2 Availability",
                requires = { "HW_TIM2_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM3_STM32",
                brief = "TIM3 Availability",
                description = "TIM3 Availability",
                requires = { "HW_TIM3_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM4_STM32",
                brief = "TIM4 Availability",
                description = "TIM4 Availability",
                requires = { "HW_TIM4_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM5_STM32",
                brief = "TIM5 Availability",
                description = "TIM5 Availability",
                requires = { "HW_TIM5_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM6_STM32",
                brief = "TIM6 Availability",
                description = "TIM6 Availability",
                requires = { "HW_TIM6_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM7_STM32",
                brief = "TIM7 Availability",
                description = "TIM7 Availability",
                requires = { "HW_TIM7_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM8_STM32",
                brief = "TIM8 Availability",
                description = "TIM8 Availability",
                requires = { "HW_TIM8_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM9_STM32",
                brief = "TIM9 Availability",
                description = "TIM9 Availability",
                requires = { "HW_TIM9_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM10_STM32",
                brief = "TIM10 Availability",
                description = "TIM10 Availability",
                requires = { "HW_TIM10_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM11_STM32",
                brief = "TIM11 Availability",
                description = "TIM11 Availability",
                requires = { "HW_TIM11_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM12_STM32",
                brief = "TIM12 Availability",
                description = "TIM12 Availability",
                requires = { "HW_TIM12_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM13_STM32",
                brief = "TIM13 Availability",
                description = "TIM13 Availability",
                requires = { "HW_TIM13_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM14_STM32",
                brief = "TIM14 Availability",
                description = "TIM14 Availability",
                requires = { "HW_TIM14_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM15_STM32",
                brief = "TIM15 Availability",
                description = "TIM15 Availability",
                requires = { "HW_TIM15_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM16_STM32",
                brief = "TIM16 Availability",
                description = "TIM16 Availability",
                requires = { "HW_TIM16_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM17_STM32",
                brief = "TIM17 Availability",
                description = "TIM17 Availability",
                requires = { "HW_TIM17_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_TIM19_STM32",
                brief = "TIM19 Availability",
                description = "TIM19 Availability",
                requires = { "HW_TIM19_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_I2C2_STM32",
                brief = "I2C2 Availability",
                description = "I2C2 Availability",
                requires = { "HW_I2C2_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
             {
                macro = "HW_I2C3_STM32",
                brief = "I2C3 Availability",
                description = "I2C3 Availability",
                requires = { "HW_I2C3_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
             {
                macro = "HW_SPI2_STM32",
                brief = "SPI2 Availability",
                description = "SPI2 Availability",
                requires = { "HW_SPI2_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_SPI3_STM32",
                brief = "SPI3 Availability",
                description = "SPI2 Availability",
                requires = { "HW_SPI3_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_SPI4_STM32",
                brief = "SPI4 Availability",
                description = "SPI4 Availability",
                requires = { "HW_SPI4_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_SPI5_STM32",
                brief = "SPI5 Availability",
                description = "SPI5 Availability",
                requires = { "HW_SPI5_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_SPI6_STM32",
                brief = "SPI6 Availability",
                description = "SPI6 Availability",
                requires = { "HW_SPI6_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_SDIO_STM32",
                brief = "SDIO Availability",
                description = "SDIO Availability",
                requires = { "HW_SDIO_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_SAI1_STM32",
                brief = "SAI1 Availability",
                description = "SAI1 Availability",
                requires = { "HW_SAI1_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_ADC_STM32",
                brief = "ADC Global interrupt on F2/4",
                description = "ADC Global interrupt on F2/4",
                requires = { "HW_ADC_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_ADC1_STM32F1",
                brief = "ADC1 Single Interrupt on F1",
                description = "ADC1 Single Interrupt on F1",
                requires = { "HW_ADC1_STM32F1" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_ADC1_STM32",
                brief = "ADC1 Single Interrupt",
                description = "ADC1 Single Interrupt",
                requires = { "HW_ADC1_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_ADC1_2_STM32F1",
                brief = "ADC1_2 Interrupt on F1",
                description = "ADC1_2 Interrupt on F1",
                requires = { "HW_ADC1_2_STM32F1" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_ADC1_2_STM32",
                brief = "ADC1_2 Interrupt",
                description = "",
                requires = { "HW_ADC1_2_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_ADC3_STM32F1",
                brief = "ADC3 Availability on F1",
                description = "ADC3 Availability on F1",
                requires = { "HW_ADC3_STM32F1" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_ADC3_STM32",
                brief = "ADC3 Availability",
                description = "ADC3 Availability",
                requires = { "HW_ADC3_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_ADC4_STM32",
                brief = "ADC4 Availability",
                description = "ADC4 Availability",
                requires = { "HW_ADC4_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_SDADC1_STM32",
                brief = "SDADC1 Availability",
                description = "SDADC1 Availability",
                requires = { "HW_SDADC1_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_SDADC2_STM32",
                brief = "SDADC2 Availability",
                description = "SDADC2 Availability",
                requires = { "HW_SDADC2_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_SDADC3_STM32",
                brief = "SDADC3 Availability",
                description = "SDADC3 Availability",
                requires = { "HW_SDADC3_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_HASH_STM32",
                brief = "HASH Availability",
                description = "HASH Availability",
                requires = { "HW_HASH_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_HASH_RNG_STM32",
                brief = "HASH and RNG Availability",
                description = "HASH and RNG Availability",
                requires = { "HW_HASH_RNG_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_DCMI_STM32",
                brief = "CAMERA Availability",
                description = "CAMERA interface Availability",
                requires = { "HW_DCMI_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_LTDC_STM32",
                brief = "LTDC Availability",
                description = "LTDC Availability",
                requires = { "HW_LTDC_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_DMA2D_STM32",
                brief = "DMA2D Availability",
                description = "DMA2D Availability",
                requires = { "HW_DMA2D_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_EMAC_STM32",
                brief = "Ethernet Availability",
                description = "Ethernet Availability",
                requires = { "HW_EMAC_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_FSMC_STM32",
                brief = "FSMC Availability",
                description = "FSMC Availability",
                requires = { "HW_FSMC_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_FMC_STM32",
                brief = "FMC Availability",
                description = "FMC Availability",
                requires = { "HW_FMC_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_COMP1_2_3_STM32",
                brief = "COMP1_2_3 Availability",
                description = "COMP1_2_3 Availability",
                requires = { "HW_COMP1_2_3_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_COMP4_5_6_STM32",
                brief = "COMP4_5_6 Availability",
                description = "COMP4_5_6 Availability",
                requires = { "HW_COMP4_5_6_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_COMP7_STM32",
                brief = "COMP7 Availability",
                description = "COMP7 Availability",
                requires = { "HW_COMP7_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_COMP_STM32",
                brief = "COMP Availability",
                description = "COMP Availability",
                requires = { "HW_COMP_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_COMP_ACQ_STM32",
                brief = "COMP_ACQ Availability",
                description = "COMP_ACQ Availability",
                requires = { "HW_COMP_ACQ_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
            {
                macro = "HW_CEC_STM32",
                brief = "CEC Availability",
                description = "Availability",
                requires = { "HW_CEC_STM32" },
                default = "1",
                type = "integer",
                file = "include/cfg/devices.h"
            },
        }
    }
}
