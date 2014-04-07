    --
    -- STM32 PLL Configuration
    --
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
                              "SYSCLK_HSE is the external oscillator or crystal input.\n",
                        type = "enumerated",
                        choices = { "SYSCLK_HSI", "SYSCLK_PLL", "SYSCLK_HSE" },
                        file = "include/cfg/clock.h"
                 },
                 {
                        macro = "PLLCLK_SOURCE",
                        brief = "PLL Clock Source",
                        description = "Select where the PLL should get its clock from.\n\n"..
                              "SYSCLK_HSI is the internal 8MHz clock. PLL is fed with SYSCLK_HSI/2.\n"..
                              "SYSCLK_HSE is the external oscillator or crystal input.\n",
                              type = "enumerated",
                              choices = { "PLLCLK_HSI", "PLLCLK_HSE" },
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
            },
      },
}
