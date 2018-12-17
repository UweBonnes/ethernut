--
-- Copyright (C) 2015 Uwe Bonnes bon@elektron.ikp.physik.tu-darmstadt.de.
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

nutarch_cm3_stm32_i2c_devices =
{
    -- ******************************************
    --
    -- STM32 I2C Device Configuration and drivers
    --
    -- ******************************************
    --
    -- STM32 I2C Bus 1 Configuration
    --
    {
        name = "nutarch_cm3_stm32f_i2c1",
        brief = "STM32 I2C Bus 1 Controller",
        description = "STM32 hardware I2C1 support.",
        requires = { "HW_I2C1_STM32" },
        provides = { "DEV_TWI" },
        options =
        {
            {
                macro = "I2C1_AS_DEFAULT",
                brief = "Use as default I2C",
                description = "Enabling this options reconfigures TwiXxx() Macros to use this bus.\n"..
                              "This is for compatibility with Nut/OS.",
                exclusivity = { "I2C1_AS_DEFAULT", "I2C2_AS_DEFAULT" },
                flavor = "booldata",
                file = "include/cfg/twi.h"

            },
            {
                macro = "I2C1_USE_DMA",
                brief = "Use DMA with I2C1",
                description = "Enabling this options configures I2C bus to use DMA.\n\n"..
                              "Errata Sheet 14574 Rev.9:\n"..
                              "This should be default for STM32F10xxB/8 devices according this Errata "..
                              "to overcome the mentioned limitations.",
                flavor = "booldata",
                file = "include/cfg/twi.h"
            },
       },
    },

    --
    -- STM32 I2C Bus 2 Configuration
    --
    {
        name = "nutarch_cm3_stm32f_i2c2",
        brief = "STM32 I2C Bus 2 Controller",
        description = "STM32 hardware I2C support.\n\n This bus cannot be remapped on F1/L1. Pins used are:\n"..
                      "SCL PB10\nSDA PB11\n",
        requires = { "HW_I2C2_STM32" },
        provides = { "DEV_TWI" },
        options =
        {
            {
                macro = "I2C2_AS_DEFAULT",
                brief = "Use as default I2C",
                description = "Enabling this options reconfigures TwiXxx() Macros to use this bus.\n"..
                              "This is for compatibility with Nut/OS.",
                exclusivity = { "I2C1_AS_DEFAULT", "I2C2_AS_DEFAULT" },
                flavor = "booldata",
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C2_USE_DMA",
                brief = "Use DMA with I2C2",
                description = "Enabling this options configures I2C bus to use DMA.\n"..
                              "This should be default for STM32F10xxB/8 devices according Errata Sheet 14574 Rev.9.\n"..
                              "to overcome the mentioned limitations.",
                flavor = "booldata",
                file = "include/cfg/twi.h"
            },
        }
    },

     -- I2C Pin Configuration
     {
        name = "nutarch_cm3_stm32_i2c1_pin",
        brief = "STM32 I2C1 Pin mappings",
        description = "STM32 I2C1 Pin mappings",
        options =
        {
            {
                macro = "I2C1_REMAP_I2C",
                brief = "Use Alternate Pins",
                description = "Choose Pin remap. Default = 0 means no remap.\n\n"..
                              "Default 0 Pin Mapping is:\n SCL PB06\n SDA PB07\n"..
                              "Remap   1 Pin Mapping is:\n SCL PB08\n SDA PB09\n",
                requires = { "HW_GPIO_STM32V1" },
                type = "enumerated",
                choices = {"0", "1"},
                default = "0",
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C1_USE_SMBA",
                brief = "Use I2C1 SMBA on F1",
                description = "Use I2C1 SMBA on F1",
                requires = { "HW_GPIO_STM32V1" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "I2C1_SDA",
                brief = "I2C1 SDA GPIO",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C1 SDA according to availability.",
                type = "enumerated",
                default = function() return GetI2c1Sda()[1]; end,
                choices = function() return GetI2c1Sda(); end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C1_SCL",
                brief = "I2C1 SCL GPIO",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C1 SCL Pin according to availability.",
                type = "enumerated",
                default = function() return GetI2c1Scl()[1]; end,
                choices = function() return GetI2c1Scl(); end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C1_SMBA",
                brief = "I2C1 SMBA GPIO",
                requires = { "HW_GPIO_STM32V2" },
                description = "Default is PIN_NONE to not use SMBA.",
                type = "enumerated",
                default = "PIN_NONE",
                choices = function() return GetI2c1Smba(); end,
                file = "include/cfg/twi.h"
            }
         }
     },
     {
        name = "nutarch_cm3_stm32_i2c2_pin",
        brief = "STM32 I2C2 Pin mappings",
        description = "STM32 I2C2 Pin mappings",
        options =
        {
            {
                macro = "I2C2_SDA",
                brief = "I2C2 SDA",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C2 SDA according to availability.",
                type = "enumerated",
                default = function() return GetI2c2Sda()[1]; end,
                choices = function() return GetI2c2Sda(); end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C2_SCL",
                brief = "I2C2 SCL",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C2 SCL Pin according to availability.",
                type = "enumerated",
                choices = function() return GetI2c2Scl(); end,
                default = function() return GetI2c2Scl()[1]; end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C2_SMBA",
                brief = "I2C2 SMBA",
                requires = { "HW_GPIO_STM32V2" },
                description = "Default is PIN_NONE to not use SMBA.",
                type = "enumerated",
                default = "PIN_NONE",
                choices = function() return GetI2c2Smba(); end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C2_USE_SMBA",
                brief = "Use I2C2 SMBA on F1",
                description = "Use I2C2 SMBA on F1",
                requires = { "HW_GPIO_STM32V1" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
         }
    },
     {
        name = "nutarch_cm3_stm32_i2c3_pin",
        brief = "STM32 I2C3 Pin mappings",
        description = "STM32 I2C3 Pin mappings",
        requires = { "HW_I2C3_STM32" },
        options =
        {
            {
                macro = "I2C3_SDA",
                brief = "I2C3 SDA",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C3 SDA according to availability.",
                type = "enumerated",
                default = function() return GetI2c3Sda()[1] end,
                choices = function() return GetI2c3Sda() end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C3_SCL",
                brief = "I2C3 SCL",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C3 SCL Pin according to availability.",
                type = "enumerated",
                default = function() return GetI2c3Scl()[1] end,
                choices = function() return GetI2c3Scl() end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C3_SMBA",
                brief = "I2C3 SMBA",
                requires = { "HW_GPIO_STM32V2" },
                description = "Default is PIN_NONE to not use SMBA.",
                type = "enumerated",
                default = "PIN_NONE",
                choices = function() return GetI2c2Smba(); end,
                file = "include/cfg/twi.h"
            },
         }
    },
   {
        name = "nutarch_cm3_stm32_i2c4_pin",
        brief = "STM32 I2C4 Pin mappings",
        description = "STM32 I2C4 Pin mappings",
        requires = { "HW_I2C4_STM32" },
        options =
        {
            {
                macro = "I2C4_SDA",
                brief = "I2C4 SDA",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C4 SDA according to availability.",
                type = "enumerated",
                default = function() return GetI2c4Sda()[1] end,
                choices = function() return GetI2c4Sda() end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C4_SCL",
                brief = "I2C4 SCL",
                requires = { "HW_GPIO_STM32V2" },
                description = "Choose I2C4 SCL Pin according to availability.",
                type = "enumerated",
                default = function() return GetI2c4Scl()[1] end,
                choices = function() return GetI2c4Scl() end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2C4_SMBA",
                brief = "I2C4 SMBA",
                requires = { "HW_GPIO_STM32V2" },
                description = "Default is PIN_NONE to not use SMBA.",
                type = "enumerated",
                default = "PIN_NONE",
                choices = function() return GetI2c4Smba() end,
                file = "include/cfg/twi.h"
            }
        }
    }
}
