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
    if c_is_provided("HW_MCU_STM32F3") then
       return { "PLLCLK_HSI_PREDIV", "PLLCLK_HSE_PREDIV" }
    end
    return { "PLLCLK_HSI", "PLLCLK_HSE" }
end
--
-- Retrieve PLL Input CLK default.
--
function GetPllClkSrcDefault()
    if c_is_provided("HW_MCU_STM32F0") then
       return { "PLLCLK_HSI_DIV2"}
    end
    if c_is_provided("HW_MCU_STM32F3") then
       return { "PLLCLK_HSI_PREDIV" }
    end
    return { "PLLCLK_HSI" }
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
                        type = "enumerated",
                        choices = function() return GetSysClkSrc() end,
                        default = "SYSCLK_HSI",
                        file = "include/cfg/clock.h"
                 },
                 {
                        macro = "PLLCLK_SOURCE",
                        brief = "PLL Clock Source",
                        description = "Select where the PLL should get its clock from.\n\n"..
                              "SYSCLK_HSI_DIV2 is the internal 8 MHz clock divided by 2.\n"..
                              "SYSCLK_HSI_PREDIV is the internal 8 MHz clock divided by 1...16.\n"..
                              "SYSCLK_HSE_PREDIV is external oscillator or crystal input divided by 1...16.\n"..
                              "SYSCLK_HSI48_PREDIV is internal 48 MHz clock divided by 1...16.\n"..
                              "If not explicit given as PLLCLK_DIV and PLLCLK_MULT, is it tried to calculate\n"..
                              "both values from SYSCLK_FREQ and the choosen PLL input clock.",
                              type = "enumerated",
                        choices = function() return GetPllClkSrc() end;
                        default = function() return GetPllClkSrcDefault() end;
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
                              "   STM32F102/3: 1.. 25 Mhz\n"..
                              "   STM32F105/7: 3.. 25 Mhz\n"..
                              "   STM32F20X  : 1.. 26 Mhz\n"..
                              "   STM32F3XX  : 1.. 32 Mhz\n"..
                              "   STM32F4XX  : 1.. 50 Mhz\n"..
                              "Typical Values is 8MHz. On F105/7 DFU bootloader works\n"..
                              "better with 25 MHz\n",
                        flavor = "integer",
                        type = "long",
                        default = "8000000",
                        file = "include/cfg/clock.h"
                  },
                  {
                        macro = "SYSCLK_FREQ",
                        brief = "CM3 System Clock",
                        description = "System clock (SYSCLK) target frequency.\n"..
                              "Allowed values:\n"..
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
                        flavor = "integer",
                        type = "long",
                        file = "include/cfg/clock.h"
                   },
                   {
                        macro = "PLLCLK_MULT",
                        brief = "PLL Clock Multiplier",
                        description = "In many cases HSE_VALUE and SYSCLK_FREQ is enough for the code\n"..
                                      "to calculate PLLCLK_MULT and PLLCLK_DIV. If code is unable,\n"..
                                      "enter non-zero integer PLL Clock Multiplier value here\n",
                        flavor = "integer",
                        file = "include/cfg/clock.h"
                  },
                   {
                        macro = "PLLCLK_DIV",
                        brief = "PLL Clock Divider",
                        description = "In many cases HSE_VALUE and SYSCLK_FREQ is enough for the code\n"..
                                      "to calculate PLLCLK_MULT and PLLCLK_DIV. If code is unable,\n"..
                                      "enter non-zero integer PLL Clock Divider value here\n",
                        flavor = "integer",
                        file = "include/cfg/clock.h"
                  },
            },
      },
}
