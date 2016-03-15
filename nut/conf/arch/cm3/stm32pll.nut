    --
    -- STM32 PLL Configuration
    --
--
-- Retrieve SYSCLK available on the device.
--
function GetSysClkSrc()
    if c_is_provided("HW_RCC_STM32L") then
        return {"SYSCLK_MSI", "SYSCLK_HSI", "SYSCLK_HSE", "SYSCLK_PLL"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return {"SYSCLK_HSI", "SYSCLK_HSE", "SYSCLK_PLL", "SYSCLK_PLLR"}
    end
    return {"SYSCLK_HSI", "SYSCLK_HSE", "SYSCLK_PLL"}
end

--
-- Retrieve SYSCLK description
--
function GetSysClockSourceDesc()
    if c_is_provided("HW_HSI8_STM32F") then
-- F0/F1/F3
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 8 MHz oscillator.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n\n"..
              "Default is SYSCLK_PLL."
    end
    if c_is_provided("HW_MCU_STM32F446") then
-- F44x: Choice from two outputs of the PLL
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 16 MHz oscillator.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "SYSCLK_PLLR is internal PLL R output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n"..
              "\tPLLR is also used for I2S, SA1 and SPDIF.\n"..
              "\tPLL is dedicated to core.\n\n"..
              "Default is SYSCLK_PLL."
    end
    if c_is_provided("HW_HSI16_STM32F") then
-- F2/F4/F7
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 16 MHz oscillator.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n\n"..
              "Default is SYSCLK_PLL."
    end
    if c_is_provided("HW_MCU_STM32L0") then
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 16 MHz oscillator, "..
                          "eventually divided by 4.\n"..
              "\tSelect with HSICLK_DIV4.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n"..
              "PLLCLK_MSI is internal multi-speed oscillator.\n"..
              "\tSelect MSI Frequency with MSI_RANGE.\n\n"..
              "Default is SYSCLK_PLL."
    end
    if c_is_provided("HW_MCU_STM32L1") then
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 16 MHz oscillator.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n"..
              "PLLCLK_MSI is internal multi-speed oscillator.\n"..
              "\tSelect MSI Frequency with MSI_RANGE.\n\n"..
              "Default is SYSCLK_PLL."
    end
    if c_is_provided("HW_MCU_STM32L4") then
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 16 MHz oscillator.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n"..
              "PLLCLK_MSI is internal multi-speed oscillator.\n"..
              "\tSelect MSI Frequency with MSI_RANGE.\n\n"..
              "Default is SYSCLK_PLL."
    end
    return "Unhandled case"
end

--
-- HSE Value Description
--
-- With value given, SYSCLK_SOURCE will be SYSCLK_PLL with PLLCLK_HSE
--
function GetHseValueDesc()
    if c_is_provided("HW_MCU_STM32L4") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN (Range 1)\t: 0 .. 46 Mhz\n"..
               "External input at OSCIN (Range 2)\t: 0 .. 26 Mhz\n"..
               "External resonator\t\t: 4 .. 48 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_RCC_STM32L") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 32 Mhz\n"..
               "With CSS off and PLL off\t: 0 .. 32 MHz\n"..
               "External resonator\t: 4 .. 25 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F0") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 32 Mhz\n"..
               "External resonator\t: 4 .. 32 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F100") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 24 Mhz\n"..
               "External resonator\t: 4 .. 24 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F101") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 25 Mhz\n"..
               "External resonator\t: 4 .. 16 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F102") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 25 Mhz\n"..
               "External resonator\t: 4 .. 16 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F1_CL") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 50 Mhz\n"..
               "External resonator\t: 3 .. 25 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Builtin USB bootloader starts much faster with 25 MHz.\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F2") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 26 Mhz\n"..
               "External resonator\t: 4 .. 26 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F3") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 32 Mhz\n"..
               "External resonator\t: 4 .. 32 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F4") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 50 Mhz\n"..
               "External resonator\t: 4 .. 26 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 50 Mhz\n"..
               "External resonator\t: 4 .. 26 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    return Unhandled
end

--
-- Retrieve PLL Input CLK description for the device.
--
function GetPllClockSourceDesc()
    if c_is_provided("HW_MCU_STM32L0") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 16 MHz oscillator, "..
                        "eventually divided by 4.\n"..
            "Input clock must be in the range 2 .. 24 MHz.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
    if c_is_provided("HW_MCU_STM32L1") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 16 MHz oscillator.\n"..
            "Input clock must be in range 2 .. 24 MHz.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_NONE is PLL off.\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 16 MHz oscillator.\n"..
            "PLLCLK_MSI is internal multispeed oscillator. "..
                        "See MSI_ related values.\n"..
            "Pll input clock range is 4 .. 16 MHz.\n"..
            "Clock division by 1 .. 8 is available.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, with\n"..
            "LSE_VALUE provided PLLCLK_MSI, PLLCLK_HSI else."
   end
-- F03/F05/F100/F37
    if c_is_provided("HW_HSI8_DIV2_STM32") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 8 MHz oscillator divided by 2.\n"..
            "Input clock must be in range 1 .. 24 MHz.\n"..
            "HSE Clock division by 1 .. 16 is available.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
-- F04/F07/F09/F3_HD
    if c_is_provided("HW_HSI8_STM32") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 8 MHz oscillator.\n"..
            "Input clock must be in range 1 .. 24 MHz.\n"..
            "Clock division by 1 .. 16 is available.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
-- F105/F107
    if c_is_provided("HW_MCU_STM32F1_CL") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 8 MHz oscillator divided by 2.\n"..
            "Input clock must be in range 1 .. 12 MHz.\n"..
            "HSE Clock division by 1 .. 16 is available.\n"..
            "HSE may also be provided by PLL2.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
    if c_is_provided("HW_MCU_STM32F1") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 8 MHz oscillator divided by 2.\n"..
            "Input clock must be in range 1 .. 25 MHz.\n"..
            "HSE Clock division by 2 is available.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
    if c_is_provided("HW_MCU_STM32F3") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 8 MHz oscillator divided by 2.\n"..
            "Input clock must be in range 1 .. 24 MHz.\n"..
            "HSE Clock division by 2 .. 16 is available.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
-- F2/F4/F7
    return "Select source for PLL input.\n\n"..
        "PLLCLK_HSE is external resonator or input. "..
                    "See HSE_ related values.\n"..
        "PLLCLK_HSI is internal 16 MHz oscillator.\n"..
        "Input clock must be in range 0.95 .. 2.1 MHz.\n"..
        "HSE Clock division by 2 .. 63 is available.\n"..
        "PLL setup is done automatic for some default value.\n"..
        "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
        "Default is PLLCLK_HSI."
end

--
-- Retrieve PLL Input CLK available on the device.
--
function GetPllClkSrc()
    if c_is_provided("HW_MCU_STM32L4") then
        return {"PLLCLK_NONE", "PLLCLK_HSI", "PLLCLK_HSE"}
    end
    return {"PLLCLK_HSI", "PLLCLK_HSE"}
end

--
-- Retrieve PLL Input CLK default.
--
function GetPllClkSrcDefault()
    if c_is_provided("HW_MCU_STM32L4") then
        if c_is_provided("LSE_VALUE") then
            return "PLLCLK_MSI"
        end
    end
    if c_is_provided("HSE_VALUE") then
        return "PLLCLK_HSE"
    end
    return "PLLCLK_HSI"
end

function GetPllMult()
    if c_is_provided("HW_MCU_STM32F1_CL") then
       return { "4", "5", "6", "7", "8", "9", "6.5"}
    end
    return { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"}
end

function GetPllPrediv()
    if c_is_provided("HW_MCU_STM32F1") then
       if c_is_provided("HW_MCU_STM32F1_CL") then
          return { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"}
       end
       if c_is_provided("HW_MCU_STM32F100") then
          return { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"}
       end
       return { "1", "2"}
    end
    return { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"}
end

function GetLseDriveLevel()
    if c_is_provided("HW_MCU_STM32F411") then
        return { "0" , "1"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "0" , "1"}
    end
    if c_is_provided("HW_MCU_STM32L1") then
        return "0"
    end
    if c_is_provided("HW_MCU_STM32F1") then
        return "0"
    end
    if c_is_provided("HW_MCU_STM32F2") then
        return "0"
    end
    if c_is_provided("HW_MCU_STM32F4") then
        return "0"
    end
    return { "0", "1", "2", "3"}
end

function GetRtcClkSrcDefault()
    if c_is_provided("LSE_VALUE") then
       return "RTCCLK_LSE"
    end
    if c_is_provided("HSE_VALUE") then
       return "RTCCLK_HSE"
    end
    return "RTCCLK_LSI"
end

nutarch_cm3_stm32_pll =
{
    {
        name = "nutarch_cm3_stm32_rcc_common",
        brief = "STM32 RCC common",
        description = "STM Common RCC parameters.",
        options =
        {
            {
                macro = "SYSCLK_SOURCE",
                brief = "System clock source",
                description =function() return GetSysClockSourceDesc() end,
                type = "enumerated",
                choices = function() return GetSysClkSrc() end,
                default = "SYSCLK_PLL",
                file = "include/cfg/clock.h"
            },
            {
               macro = "PLLCLK_SOURCE",
                brief = "PLL Clock Source",
                description = function() return GetPllClockSourceDesc() end,
                type = "enumerated",
                choices = function() return GetPllClkSrc() end,
                default = function() return GetPllClkSrcDefault() end,
                file = "include/cfg/clock.h"
            },
            {
                macro = "HSE_VALUE",
                brief = "External Oszillator Frequency",
                description = function() return GetHseValueDesc() end,
                type = "integer",
                provides = {"HSE_VALUE"},
                file = "include/cfg/clock.h"
            },
            {
                macro = "HSE_BYPASS",
                brief = "HSE from external source",
                description = "Use the clock signal applied to OSC_IN.",
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/clock.h",
            },
            {
                macro = "LSE_DRIVE_LEVEL",
                brief = "LSE osc power",
                description = "Power level of LSE oscillator\n"..
                              "0 = Low drive\n"..
                              "1 = Medium high drive\n"..
                              "2 = Medium low drive\n"..
                              "3 = High drive.",
                type = "enumerated",
                choices = function() return GetLseDriveLevel() end,
                default = "0",
                file = "include/cfg/clock.h"
            },
            {
                macro = "LSE_BYPASS",
                brief = "LSE from external source",
                description = "Use clock signal applied to OSC32_IN.",
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/clock.h"
            },
            {
                macro = "LSE_VALUE",
                brief = "Frequency of LSE Clock ",
                description = "Frequency of LSE quarz/external LSE input.\n"..
                              "Standard is 32768 Hz.\n"..
                              "Default is undefined.",
                type = "integer",
                provides = {"LSE_VALUE"},
                file = "include/cfg/clock.h"
            },
            {
                macro = "LSI_ON",
                brief = "Turn LSI on",
                description = "Turn LSI on(1) or off(0).\n"..
                              "Default is on.",
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/clock.h"
            },
            {
                macro = "RTCCLK_SOURCE",
                brief = "RTC(/LCD) clock source",
                description = "Clock used for RTC and LCD.",
                type = "enumerated",
                choices = {"RTCCLK_LSE", "RTCCLK_HSE", "RTCCLK_LSI"},
                default = function() return GetRtcClkSrcDefault() end,
                file = "include/cfg/clock.h"
            },
        }
    },
    {
        name = "nutarch_cm3_stm32_rcc",
        brief = "STM32 PLL Setup",
        requires = {"HW_RCC_STM32"},
        options =
        {
                  {
                        macro = "SYSCLK_FREQ",
                        brief = "CM3 System Clock",
                        description = "System clock (SYSCLK) target frequency.\n"..
                              "Do not define so that NutOS may choose highest possible:\n"..
                              "Allowed values:\n"..
                              "   STM32F0XX   :  48 Mhz\n"..
                              "   STM32F100   :  24 Mhz\n"..
                              "   STM32F101   :  36 Mhz\n"..
                              "   STM32F102/3 :  48 Mhz\n"..
                              "   STM32F105/7 :  72 Mhz\n"..
                              "   STM32F20X   : 120 Mhz\n"..
                              "   STM32F3XX   :  72 Mhz\n"..
                              "   STM32F401   :  72 Mhz\n"..
                              "   STM32F40_1X : 168 Mhz\n"..
                              "   STM32F42_3X : 184 Mhz\n",
                              "   STM32F7     : 216 Mhz\n",
                        provides = {"SYSCLK_FREQ"},
                        file = "include/cfg/clock.h"
                   },
                   {
                        macro = "PLLCLK_MULT",
                        brief = "PLL Clock Multiplier",
                        type = "enumerated",
                        choices = function() return GetPllMult() end,
                        requires = {"SYSCLK_FREQ"},
                        file = "include/cfg/clock.h"
                  },
                  {
                        macro = "PLLCLK_PREDIV",
                        brief = "PLL Clock Divider",
                        description = "In many cases HSE_VALUE and SYSCLK_FREQ is enough for the code\n"..
                                      "to calculate PLLCLK_MULT and PLLCLK_DIV. If code is unable,\n"..
                                      "enter non-zero integer PLL Clock Divider value here\n",
                        requires = {"SYSCLK_FREQ"},
                        type = "enumerated",
                        choices =  function() return GetPllPrediv() end,
                        file = "include/cfg/clock.h"
                  },
--STM32F1_CL specific values
                  {
                        macro = "PLL2CLK_PREDIV",
                        brief = "PLL2 Clock Predivider",
                        description = "Divider between HSE and PLL2 input.\n",
                        type = "enumerated",
                        choices = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
                        requires = {"HW_MCU_STM32F1_CL", "SYSCLK_FREQ"},
                        file = "include/cfg/clock.h"
                  },
                  {
                        macro = "PLL2CLK_MULT",
                        brief = "PLL2 Clock Multiplier",
                        description = "Multiplier for PLL2 clock.\n",
                        requires = {"HW_MCU_STM32F1_CL", "SYSCLK_FREQ"},
                        type = "enumerated",
                        choices = { "8", "9", "10", "11", "12", "13", "14", "16", "20"},
                        file = "include/cfg/clock.h"
                  },
            },
      },
    {
        name = "nutarch_cm3_stm32l4_rccl",
        brief = "STM32L4 PLL Setup",
        requires = {"HW_RCC_STM32L4"},
        options =
        {
            {
                macro = "MSI_RANGE",
                brief = "MSI value",
                description = "Select MSI frequency.\n"..
                              "Possible values are 100/200/400/800 kHz "..
                              "and 1/2/4/8/16/24/32/48 MHz.\n"..
                              "Default value after reset is 4 MHz.\n",
                              "In Range 2, maximum allowed frequency is 24 MHz.\n"..
                              "Use MSI_OFF to disable MSI.",
                type = "enumerated",
                choices = {"MSI_OFF", "MSI_100k", "MSI_200k", "MSI_400k", "MSI_800k", "MSI_1M", "MSI_2M", "MSI_4M", "MSI_8M", "MSI_16M", "MSI_24M", "MSI_32M", "MSI_48M"},
                default = "MSI_4M",
                file = "include/cfg/clock.h"
            },
            {
                macro = "STM32_VRANGE",
                brief = "Voltage Scaling",
                description = "Use 2 for extra low power. Restrictions apply.\n"..
                              "E.g. max SYSCLK is  26 MHz in range 2.",
                type = "enumerated",
                choices = {"1", "2"},
                default = "1",
                file = "include/cfg/clock.h"
            },
            {
                macro = "SYSCLK_FREQ",
                brief = "System Clock",
                description = "System clock (SYSCLK) target frequency.\n"..
                      "When not defined, setup tries to choose highest possible value.\n"..
                      "With value given, PLLM/PLLN and PLLR need to be given!\n"..
                      "Maximum allowed frequency is 80 Mhz in Range 1 and 26 MHz in Range 2.\n",
                type = "long",
                provides = {"SYSCLK_FREQ"},
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLLM",
                brief = "PLL input clock divisor",
                description = "PLL input clock M divisor.\n"..
                              "Must be 1..8. Input Clock must be 4..16 MHz.\n"..
                              "Only required if SYSCLK_FREQ is given.",
                requires = {"SYSCLK_FREQ"},
                type = "enumerated",
                choices =  {"1", "2", "3", "4", "5", "6", "7", "8"},
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLLN",
                brief = "PLL clock multiplicator.",
                description = "PLL input clock Multiplicator N.\n"..
                              "Must be 8 .. 86.\n"..
                              "Resulting VCO output must be 64 .. 344(Range 1)/128 (Range2) Mhz.\n"..
                              "Only required if SYSCLK_FREQ is given.",
                requires = {"SYSCLK_FREQ"},
                type = "long",
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLLR",
                brief = "PLL VCO Clock divisor.",
                description = "PLL input clock N Multiplicator.\n"..
                              "Must be 8 .. 86.\n"..
                              "Resulting PLL cloxk must be <= 80 (Range 1)/ 26 (Range2) Mhz.\n"..
                              "Only required if SYSCLK_FREQ is given.",
                requires = {"SYSCLK_FREQ"},
                type = "enumerated",
                choices =  {"2", "4", "6", "8"},
                file = "include/cfg/clock.h"
            },
            {
                macro = "AHB_PRE",
                brief = "AHB clock divisor.",
                description = "Divisor between SYSCLK and HCLK.\n"..
                              "Allowed values are 1, 2, 4, 8, 16, 32, 64. 128, 256 and 512.",
                type = "enumerated",
                choices =  {"1", "2", "4", "8", "16", "32", "64", "128", "256", "512"},
                default = "1",
                file = "include/cfg/clock.h"
            },
            {
                macro = "APB1_PRE",
                brief = "APB1 clock divisor.",
                description = "Divisor between HCLK and PCLK1.\n"..
                              "Allowed values are 1, 2, 4, 8, and 16.",
                type = "enumerated",
                choices =  {"1", "2", "4", "8", "16"},
                default = "1",
                file = "include/cfg/clock.h"
            },
            {
                macro = "APB2_PRE",
                brief = "APB2 clock divisor.",
                description = "Divisor between HCLK and PCLK2.\n"..
                              "Allowed values are 1, 2, 4, 8, and 16.",
                type = "enumerated",
                choices =  {"1", "2", "4", "8", "16"},
                default = "1",
                file = "include/cfg/clock.h"
            },
        },
    }
}
