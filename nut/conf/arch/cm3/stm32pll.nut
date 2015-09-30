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

nutarch_cm3_stm32_pll =
{
      {
          name = "nutarch_cm3_stm32L1_pll",
          brief = "STM32L1 PLL Setup",
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
                        macro = "HSE_BYPASS",
                        brief = "HSE from external source",
                        description = "Use the clock signal applied to OSC_IN.",
                        flavor = "booldata",
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
                        choices = { "8", "9", "10", "11", "12", "13", "14", "16", "20"};
                        file = "include/cfg/clock.h"
                  },
            },
      },
}
