    --
    -- STM32 PLL Configuration
    --
--
-- Retrieve SYSCLK available on the device.
--
function GetSysClkSrc()
    if c_is_provided("HW_CLK48_STM32") then
        return { "SYSCLK_HSI", "SYSCLK_PLL", "SYSCLK_HSE", "SYSCLK_HSI48" }
    end
    return { "SYSCLK_HSI", "SYSCLK_PLL", "SYSCLK_HSE" }
end

--
-- Retrieve PLL Input CLK available on the device.
--
function GetPllClkSrc()
    if c_is_provided("HW_MCU_STM32F0") then
       if c_is_provided("HW_CLK48_STM32") then
          return { "PLLCLK_HSI_DIV2", "PLLCLK_HSI_PREDIV", "PLLCLK_HSE_PREDIV", "SYSCLK_HSI48_PREDIV" }
       end
       return { "PLLCLK_HSI_DIV2", "PLLCLK_HSI_PREDIV" }
    end
    if c_is_provided("HW_MCU_STM32F1") then
       if c_is_provided("HW_MCU_STM32F1_CL") then
          return { "PLLCLK_HSI_DIV2", "PLLCLK_HSE_PREDIV", "PLLCLK_PLL2" }
       end
       if c_is_provided("HW_MCU_STM32F100") then
          return { "PLLCLK_HSI_DIV2", "PLLCLK_HSE_PREDIV"}
       end
       return { "PLLCLK_HSI_DIV2", "PLLCLK_HSE", "PLLCLK_HSE_DIV2"}
    end
    if c_is_provided("HW_MCU_STM32F3") then
       return { "PLLCLK_HSI_PREDIV", "PLLCLK_HSE_PREDIV" }
    end
    return { "PLLCLK_HSI", "PLLCLK_HSE" }
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

nutarch_cm3_stm32_pll =
{
    {
        name = "nutarch_cm3_stm32_rcc_common",
        brief = "STM32 RCC common",
        description = "STM Common RCC parameters.",
        options =
        {
            {
                macro = "HSE_BYPASS",
                brief = "HSE from external source",
                description = "Use the clock signal applied to OSC_IN.",
                flavor = "booldata",
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
                macro = "RTC_LSE_BYPASS",
                brief = "LSE_BYPASS",
                description = "External LSE input provided on OSC32_IN.",
                flavor = "booldata",
                file = "include/cfg/clock.h"
            },
            {
                macro = "LSE_VALUE",
                brief = "Frequency of LSE Clock ",
                description = "Frequency of LSE quarz/external LSE input.\n"..
                              "Use 0 to disable LSE clock. Default is 32768 Hz.",
                type = "integer",
                default = "32768",
                file = "include/cfg/clock.h"
            },
            {
                macro = "LSI_ON",
                brief = "Turn LSI on",
                description = "Turn LSI on(1) or off(0).\n"..
                              "Default is on.",
                type = "integer",
                default = "1",
                file = "include/cfg/clock.h"
            },
            {
                macro = "RTCCLK_SOURCE",
                brief = "RTC(/LCD) clock source",
                description = "Clock used for RTC and LCD.",
                type = "enumerated",
                choices = {"RTCCLK_LSE", "RTCCLK_HSE", "RTCCLK_LSI"},
                default = "RTCCLK_LSE",
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
                macro = "SYSCLK_SOURCE",
                brief = "SYSCLK Source",
                description = "Select where SYSCLK should get its clock from.\n\n"..
                    "SYSCLK_HSI is the internal 8MHz clock.\n"..
                    "SYSCLK_PLL is the internal PLL output. Select the source for the PLL in the next option.\n"..
                    "SYSCLK_HSE is the external oscillator or crystal input.\n"..
                    "SYSCLK_HSI48 is the internal 48MHz clock on F042/072/091.\n",
                    "SYSCLK_PLLR is the R PLL output on STM32F7.\n",
                type = "enumerated",
                choices = function() return GetSysClkSrc() end,
                file = "include/cfg/clock.h"
            },
            {
                        macro = "PLLCLK_SOURCE",
                        brief = "PLL Clock Source",
                        description = "Select where the PLL should get its clock from.\n\n"..
                              "SYSCLK_HSI_DIV2 is the internal 8 MHz clock divided by 2.\n"..
                              "SYSCLK_HSI_PREDIV is the internal 8 MHz clock divided by 1...16.\n"..
                              "SYSCLK_HSI48_PREDIV is internal 48 MHz clock divided by 1...16.\n"..
                              "SYSCLK_HSE is external oscillator or crystal undivided.\n"..
                              "SYSCLK_HSE_PREDIV is external oscillator or crystal input divided by 1...16.\n"..
                              "With PLLCLK_AUTO NutOS tries to configure for highest speed.",
                              type = "enumerated",
                        default = "PLLCLK_AUTO";
                        file = "include/cfg/clock.h"
                  },
                  {
                        macro = "HSE_VALUE",
                        brief = "External Oszillator Frequency",
                        description = "Value of the external oscillator in Herz.\n"..
                              "Allowed values:\n"..
                              "   STM32F0xx  : 1.. 32 Mhz\n"..
                              "   STM32F100  : 4.. 24 Mhz\n"..
                              "   STM32F101  : 4.. 16 Mhz\n"..
                              "   STM32F102/3: 4.. 25 Mhz\n"..
                              "   STM32F105/7: 4.. 25 Mhz\n"..
                              "   STM32F20X  : 1.. 26 Mhz\n"..
                              "   STM32F3XX  : 1.. 32 Mhz\n"..
                              "   STM32F4XX  : 1.. 50 Mhz\n"..
                              "Typical Values is 8MHz. On F105/7 DFU bootloader works\n"..
                              "better with 25 MHz\n",
                        flavor = "integer",
                        type = "long",
                        file = "include/cfg/clock.h"
                  },
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
        requires = "HW_RCC_STM32l4",
        options =
        {
            {
                macro = "SYSCLK_SOURCE",
                brief = "SYSCLK Source",
                description = "Select where SYSCLK should get its clock from.\n\n"..
                      "SYSCLK_HSE is the external oscillator or crystal input.\n"..
                      "SYSCLK_MSI is the internal multi speed oscillator. Select speed in option MSI_SPEED.\n"..
                      "SYSCLK_HSI is the internal 16 MHz oscillator.\n"..
                      "SYSCLK_PLL is the internal PLL output. Select the source for the PLL in the next option.\n",
                type = "enumerated",
                choices = {"SYSCLK_HSE", "SYSCLK_MSI", "SYSCLK_HSI", "SYSCLK_PLL"},
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLLCLK_SOURCE",
                brief = "PLL Clock Source",
                description = "Select PLL input clock. Available sources:\n\n"..
                      "SYSCLK_MSI is internal multi-speed oscillator.\n"..
                      "SYSCLK_HSI is internal 16 MHz oscillator..\n"..
                      "SYSCLK_HSE is external oscillator.",
                type = "long",
                file = "include/cfg/clock.h"
            },
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
                macro = "HSE_VALUE",
                brief = "External Oszillator Frequency",
                description = "Value of the external oscillator in Herz.\n"..
                      "Allowed values:\n"..
                      "   HSE_BYPASS/Range 1:  0.. 48 MHZ\n"..
                      "   HSE_BYPASS/Range 2:  0.. 26 MHZ\n"..
                      "   External crystal  :  4.. 48 MHZ\n"..
                      "Typical Values is 8MHz.\n",
                flavor = "integer",
                type = "long",
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
