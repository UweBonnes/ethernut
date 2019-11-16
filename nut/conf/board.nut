nutboard =
{
    brief = "Board",
    description = "Provides board items for experimental setups.",
    options =
    {
         {
              macro = "SW1_PORT",
              brief = "SW1_PORT",
              description = "Provides SW1 port for examples.",
              type = "enumerated",
              choices = function() return GetGpioBanks() end,
              flavor = "integer",
              file = function() return GetGpioHeaderPath() end
         },
         {
              macro = "SW1_PIN",
              brief = "SW1_PIN",
              description = "Provides SW1 pin for examples.",
              type = "enumerated",
              choices = function() return GetGpioBits() end,
              flavor = "integer",
              file = function() return GetGpioHeaderPath() end
         },
         {
              macro = "LED1_PORT",
              brief = "LED1_PORT",
              description = "Provides LED1 port for examples.",
              type = "enumerated",
              choices = function() return GetGpioBanks() end,
              flavor = "integer",
              file = function() return GetGpioHeaderPath() end
         },
         {
              macro = "LED1_PIN",
              brief = "LED1_PIN",
              description = "Provides LED1 pin for examples.",
              type = "enumerated",
              choices = function() return GetGpioBits() end,
              flavor = "integer",
              file = function() return GetGpioHeaderPath() end
         },
         {
              macro = "LED2_PORT",
              brief = "LED2_PORT",
              description = "Provides LED2 port for examples.",
              type = "enumerated",
              choices = function() return GetGpioBanks() end,
              flavor = "integer",
              file = function() return GetGpioHeaderPath() end
         },
         {
              macro = "LED2_PIN",
              brief = "LED2_PIN",
              description = "Provides LED2 pin for examples.",
              type = "enumerated",
              choices = function() return GetGpioBits() end,
              flavor = "integer",
              file = function() return GetGpioHeaderPath() end
         },
         {
              macro = "LED3_PORT",
              brief = "LED3_PORT",
              description = "Provides LED3 port for examples.",
              type = "enumerated",
              choices = function() return GetGpioBanks() end,
              flavor = "integer",
              file = function() return GetGpioHeaderPath() end
         },
         {
              macro = "LED3_PIN",
              brief = "LED3_PIN",
              description = "Provides LED3 pin for examples.",
              type = "enumerated",
              choices = function() return GetGpioBits() end,
              flavor = "integer",
              file = function() return GetGpioHeaderPath() end
         },
         {
              macro = "DEV_UART",
              brief = "DEV_UART",
              description = "Provides UART for examples.",
              flavor = "integer",
              file = "include/cfg/uart.h"
         },
    },
}
