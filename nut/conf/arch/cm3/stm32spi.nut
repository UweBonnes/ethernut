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

--
--
-- Retrieve SPI1 pins available on the device.
--
function GetSpi1Nss()
    if c_is_provided("HW_MCU_STM32L1") then
        return { "PIN_NONE", "PA04", "PA15", "PE12"}
    end
    if c_is_provided("HW_MCU_STM32F0") then
        return { "PIN_NONE", "PA04", "PA15", "PE12"}
    end
    if c_is_provided("HW_MCU_STM32F37") then
        return { "PIN_NONE", "PA04", "PA15", "PC06" }
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return { "PIN_NONE", "PA04", "PA15", "PE12", "PG05" }
    end
    return { "PIN_NONE", "PA04", "PA15" }
end
--
function GetSpi1Sck()
    if c_is_provided("HW_MCU_STM32L1") then
        return { "PA05", "PB03", "PE13"}
    end
    if c_is_provided("HW_MCU_STM32F37") then
        return { "PA05", "PB03", "PA12" , "PC07" }
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return { "PA05", "PB03", "PE13", "PG02" }
    end
    return { "PA05", "PB03" }
end
function GetSpi1Miso()
    if c_is_provided("HW_MCU_STM32L0") then
        return { "PA06", "PB04", "PA11"}
    end
    if c_is_provided("HW_MCU_STM32L1") then
        return { "PA06", "PB04", "PA11", "PE14"}
    end
    if c_is_provided("HW_MCU_STM32F0") then
        return { "PA06", "PB04", "PE14"}
    end
    if c_is_provided("HW_MCU_STM32F37") then
        return { "PA06", "PB04", "PA13" , "PC08" }
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return {  "PA06", "PB04", "PE14", "PG03" }
    end
    return { "PA06", "PB04" }
end
function GetSpi1Mosi()
    if c_is_provided("HW_MCU_STM32L0") then
        return { "PA07", "PB05", "PA12"}
    end
    if c_is_provided("HW_MCU_STM32L1") then
        return { "PA07", "PB05", "PA12", "PE15"}
    end
    if c_is_provided("HW_MCU_STM32F0") then
        return { "PA07", "PB05", "PE15"}
    end
    if c_is_provided("HW_MCU_STM32F37") then
        return { "PA07", "PB05", "PB00", "PC09" , "PF06" }
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return { "PA07", "PB05", "PE15", "PG04" }
    end
    return { "PA07", "PB05" }
end

--
--
-- Retrieve SPI2 pins available on the device.
--
function GetSpi2Nss()
    if c_is_provided("HW_MCU_STM32F0") then
        return { "PIN_NONE", "PB12", "PB09", "PD00"}
    end
    if c_is_provided("HW_MCU_STM32F30") then
        return { "PIN_NONE", "PB12", "PF00", "PD15"}
    end
    if c_is_provided("HW_MCU_STM32F37") then
        return { "PIN_NONE", "PB09", "PA11", "PD06"}
    end
    if c_is_provided("HW_MCU_STM32F40") then
        return { "PIN_NONE", "PB12", "PB09", "PI00"}
    end
    if c_is_provided("HW_MCU_STM32F42") then
        return { "PIN_NONE", "PB12", "PB09", "PI00"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "PIN_NONE", "PB12", "PB09", "PB04", "PD01"}
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return { "PIN_NONE", "PB12", "PB09", "PB04", "PI00"}
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return { "PIN_NONE", "PB12", "PB09", "PD00" }
    end
    return { "PIN_NONE", "PB12", "PB09" }
end
function GetSpi2NssDefault()
    if c_is_provided("HW_MCU_STM32F37") then
        return "PB09"
    end
    return "PB12"
end
--
function GetSpi2Sck()
    if c_is_provided("HW_MCU_STM32L1") then
        return { "PB13", "PD01"}
    end
    if c_is_provided("HW_MCU_STM32F0") then
        return { "PB13", "PB10", "PD01"}
    end
    if c_is_provided("HW_MCU_STM32F2") then
        return { "PB13", "PB10", "PI01"}
    end
    if c_is_provided("HW_MCU_STM32F30") then
        return { "PB13", "PF01", "PF09", "PF10"}
    end
    if c_is_provided("HW_MCU_STM32F37") then
        return { "PB08", "PA08", "PB10", "PD07", "PD08"}
    end
    if c_is_provided("HW_MCU_STM32F401") then
        return { "PB13", "PB10", "PD03"}
    end
    if c_is_provided("HW_MCU_STM32F411") then
        return { "PB13", "PB10", "PC07", "PD03"}
    end
    if c_is_provided("HW_MCU_STM32F40") then
        return { "PB13", "PB10", "PI01"}
    end
    if c_is_provided("HW_MCU_STM32F42") then
        return { "PB13", "PB10", "PD03", "PI01"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "PB13", "PB10", "PA09", "PC07", "PD03"}
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return { "PB13", "PB10", "PA09", "PD03", "PI10"}
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return { "PB13", "PB10", "PD01" }
    end
    return { "PB13", "PB10" }
end
function GetSpi2SckDefault()
    if c_is_provided("HW_MCU_STM32F37") then
        return "PB08"
    end
    return "PB13"
end
function GetSpi2Miso()
    if c_is_provided("HW_MCU_STM32L0") then
        return { "PB14", "PC02"}
    end
    if c_is_provided("HW_MCU_STM32L1") then
        return { "PB14", "PD03"}
    end
    if c_is_provided("HW_MCU_STM32F0") then
        return { "PB14", "PC02", "PD03"}
    end
    if c_is_provided("HW_MCU_STM32F2") then
        return { "PB14", "PC02", "PI02"}
    end
    if c_is_provided("HW_MCU_STM32F30") then
        return { "PB14", "PA10"}
    end
    if c_is_provided("HW_MCU_STM32F37") then
        return { "PB14", "PA09", "PC02", "PD03"}
    end
    if c_is_provided("HW_MCU_STM32F401") then
        return { "PB14", "PB15", "PC02"}
    end
    if c_is_provided("HW_MCU_STM32F411") then
        return { "PB14", "PA06", "PC02"}
    end
    if c_is_provided("HW_MCU_STM32F40") then
        return { "PB14", "PC02", "PI02"}
    end
    if c_is_provided("HW_MCU_STM32F42") then
        return { "PB14", "PC02", "PI02" }
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "PB14", "PB04", "PA06", "PC02" }
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return { "PB14", "PC02", "PI02" }
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return { "PB14", "PC02", "PD03" }
    end
    return { "PB14", "PA10" }
end
function GetSpi2Mosi()
    if c_is_provided("HW_MCU_STM32L0") then
        return { "PB15", "PC03"}
    end
    if c_is_provided("HW_MCU_STM32L1") then
        return { "PB15", "PD04"}
    end
    if c_is_provided("HW_MCU_STM32F0") then
        return { "PB15", "PC03", "PD04"}
    end
    if c_is_provided("HW_MCU_STM32F2") then
        return { "PB15", "PC03", "PI03"}
    end
    if c_is_provided("HW_MCU_STM32F30") then
        return { "PB15", "PA11"}
    end
    if c_is_provided("HW_MCU_STM32F37") then
        return { "PB15", "PA10", "PC03", "PD04"}
    end
    if c_is_provided("HW_MCU_STM32F401") then
        return { "PB15", "PC03"}
    end
    if c_is_provided("HW_MCU_STM32F411") then
        return { "PB15", "PC03"}
    end
    if c_is_provided("HW_MCU_STM32F40") then
        return { "PB15", "PC03", "PI03"}
    end
    if c_is_provided("HW_MCU_STM32F42") then
        return { "PB15", "PC03", "PI03" }
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "PB15", "PC03", "PC01" }
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return { "PB15", "PC03", "PI03" }
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return { "PB15", "PC03", "PD04" }
    end
    return { "PB15", "PC03" }
end

--
--
-- Retrieve SPI3 pins available on the device.
--
function GetSpi3Nss()
    if c_is_provided("HW_MCU_STM32L4") then
        return { "PIN_NONE", "PA04", "PA15", "PG12" }
    end
    return { "PIN_NONE", "PA04", "PA15" }
end
--
function GetSpi3Sck()
    if c_is_provided("HW_MCU_STM32F37") then
        return { "PB03", "PC10", "PA01"}
    end
    if c_is_provided("HW_MCU_STM32F411") then
        return { "PB03", "PC10", "PB12" }
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return { "PB03", "PC10", "PG09"}
    end
    return { "PB03", "PC10" }
end
function GetSpi3Miso()
    if c_is_provided("HW_MCU_STM32F37") then
        return { "PB04", "PC11", "PA02"}
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return { "PB04", "PC11", "PG10"}
    end
    return { "PB04", "PC11" }
end
function GetSpi3Mosi()
    if c_is_provided("HW_MCU_STM32F37") then
        return { "PB05", "PC12", "PA03"}
    end
    if c_is_provided("HW_MCU_STM32F401") then
        return { "PB05", "PC12", "PD06"}
    end
    if c_is_provided("HW_MCU_STM32F411") then
        return { "PB05", "PC12", "PD06"}
    end
    if c_is_provided("HW_MCU_STM32F42") then
        return { "PB05", "PC12", "PD06"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "PB05", "PC12", "PB00", "PB02", "PC01", "PC12", "PD00", "PD06"}
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return { "PB05", "PC12", "PB02", "PD06"}
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return { "PB05", "PC12", "PG11"}
    end
    return { "PB05", "PC12" }
end

--
--
-- Retrieve SPI4 pins available on the device.
--
function GetSpi4Nss()
    if c_is_provided("HW_MCU_STM32F30") then
        return { "PIN_NONE", "PE04", "PE11", "PE03"}
    end
    if c_is_provided("HW_MCU_STM32F411") then
        return { "PIN_NONE", "PE04", "PE11", "PB12"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "PIN_NONE", "PE04", "PE11", "PG14"}
    end
    return { "PIN_NONE", "PE04", "PE11"}
end

function GetSpi4Sck()
    if c_is_provided("HW_MCU_STM32F411") then
        return { "PE02", "PE12", "PB13"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "PE04", "PE11", "PG11"}
    end
    return { "PE02", "PE12"}
end

function GetSpi4Miso()
    if c_is_provided("HW_MCU_STM32F411") then
        return { "PE05", "PE13", "PA11"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "PE05", "PE13", "PD00", "PG12"}
    end
    return { "PE05", "PE13" }
end

function GetSpi4Mosi()
    if c_is_provided("HW_MCU_STM32F411") then
        return { "PE06", "PE14", "PA01"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "PE06", "PE14", "PG13"}
    end
    return { "PE06", "PE14"}
end

--
--
-- Retrieve SPI5 pins available on the device.
--
function GetSpi5Nss()
    if c_is_provided("W_MCU_STM32F411") then
        return {"PIN_NONE", "PB01", "PE04", "PE11"}
    end
    return {"PIN_NONE", "PF06", "PH05"}
end
function GetSpi5NssDefault()
    if c_is_provided("W_MCU_STM32F411") then
        return "PB01"
    end
    return "PF06"
end
function GetSpi5Sck()
    if c_is_provided("W_MCU_STM32F411") then
        return {"PB00", "PE02", "PE12"}
    end
    return {"PF07", "PH06"}
end
function GetSpi5SckDefault()
    if c_is_provided("W_MCU_STM32F411") then
        return "PB00"
    end
    return "PF07"
end
function GetSpi5Miso()
    if c_is_provided("W_MCU_STM32F411") then
        return {"PA12", "PE05", "PE13"}
    end
    return {"PF08", "PH07"}
end
function GetSpi5MisoDefault()
    if c_is_provided("W_MCU_STM32F411") then
        return "PA12"
    end
    return "PF08"
end
function GetSpi5Mosi()
    if c_is_provided("W_MCU_STM32F411") then
        return {"PA10", "PB08", "PE06", "PE14"}
    end
    return {"PF09", "PF11"}
end
function GetSpi5MosiDefault()
    if c_is_provided("W_MCU_STM32F411") then
        return "PA10"
    end
    return "PF09"
end


nutarch_cm3_stm32_spi_devices =
{
    -- ******************************************
    --
    -- STM32 SPI Device Configuration and drivers
    --
    -- ******************************************
    --
    -- STM32F SPI1 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spi1",
        brief = "STM32 SPI1 Bus Controller",
        description = "First SPI Controller with up to four Chip select line.",
        requires = { "HW_SPI1_STM32" },
        provides = { "SPIBUS_CONTROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi1.c" },
        options =
        {
            {
                macro = "SPI1_REMAP",
                brief = "Use Alternate Pins",
                description = "Choose SPI1 Pin remap. Default = 0 means no remap.\n\n"..
                              "Default 0 Pin Mapping is:\n NSS PA04\n SCK PA05\n MISO PA06\n MOSI PA07\n"..
                              "Remap   1 Pin Mapping is:\n NSS PA15\n SCK PB03\n MISO PB04\n MOSI PB05\n",
                requires = { "HW_GPIO_STM32V1" },
                type = "enumerated",
                choices = {"0", "1"},
                default = "0",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI1_USE_NSS",
                brief = "Use SPI1 NSS on F1",
                description = "Use SPI1  Hardware NSS on F1. Default off.",
                requires = { "HW_GPIO_STM32V1" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = "SPI1_CS0",
                brief = "SPI1 CS0 Gpio",
                description = "Choice of SPI1 CS0 Gpio.\n"..
                              "Default is default NSS pin.\n",
                              "Use PIN_NONE if no CS is required.\n",
                default = "PA04",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI1_CS1",
                brief = "SPI1 CS1 Gpio",
                description = "Choice of SPI1 CS1 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI1_CS2",
                brief = "SPI1 CS2 Gpio",
                description = "Choice of SPI1 CS2 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI1_CS3",
                brief = "SPI1 CS3 Gpio",
                description = "Choice of SPI1 CS3 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI1_NSS",
                brief = "SPI1 NSS Pins",
                description = "Choice of SPI1 NSS Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi1Nss() end,
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI1_SCK",
                brief = "SPI1 SCK Pins",
                description = "Choice of SPI1 SCK Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi1Sck() end,
                default = "PA05",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI1_MISO",
                brief = "SPI1 MISO Pins",
                description = "Choice of SPI1 MISO Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi1Miso() end,
                default = "PA06",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI1_MOSI",
                brief = "SPI1 MOSI Pins",
                description = "Choice of SPI1 MOSI Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi1Mosi() end,
                default = "PA07",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI1_I2S_MODE",
                brief = "I2S Mode (First Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS1_MODE",
                brief = "Select SPIBUS 1 MODE",
                description = "Choices are IRQ(0, default), DMA(1) or polling (2)",
                choices = { "0", "1", "2"},
                default = "0",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI1_DMA_TX_ALTERNATE_STREAM",
                brief = "Select SPI1 Alternate TX DMA stream ",
                description = "Select to use stream 5 versus stream 3(default) as SPI1 TX",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI1_DMA_RX_ALTERNATE_STREAM",
                brief = "Select SPI1 Alternate RX DMA stream ",
                description = "Select to use stream 2 versus stream 0(default) as SPI1 RX",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
        },
    },

    --
    -- STM32F SPI2 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spi2",
        brief = "STM32F SPI2 Bus Controller",
        description = "Second SPI Controller with up to four Chip select line.",
        requires = { "HW_SPI2_STM32" },
        provides = { "SPIBUS_CONTRROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi2.c" },
        options =
        {
-- No Remap of SPI2 on F1
            {
                macro = "SPI2_CS0",
                brief = "SPI2 CS0 Gpio",
                description = "Choice of SPI2 CS0 Gpio.\n"..
                              "Default is default NSS pin.\n",
                              "Use PIN_NONE if no CS is required.\n",
                default = function() return GetSpi2NssDefault() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI2_CS1",
                brief = "SPI2 CS1 Gpio",
                description = "Choice of SPI2 CS1 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI2_CS2",
                brief = "SPI2 CS2 Gpio",
                description = "Choice of SPI2 CS2 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI2_CS3",
                brief = "SPI2 CS3 Gpio",
                description = "Choice of SPI2 CS3 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI2_NSS",
                brief = "SPI2 NSS Pins",
                description = "Choice of SPI2 NSS Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi2Nss() end,
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI2_SCK",
                brief = "SPI2 SCK Pins",
                description = "Choice of SPI2 SCK Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi2Sck() end,
                default = function() return GetSpi2SckDefault() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI2_MISO",
                brief = "SPI2 MISO Pins",
                description = "Choice of SPI2 MISO Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi2Miso() end,
                default = "PB14",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI2_MOSI",
                brief = "SPI2 MOSI Pins",
                description = "Choice of SPI2 MOSI Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi2Mosi() end,
                default = "PB15",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI2_I2S_MODE",
                brief = "I2S Mode (Second Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS2_MODE",
                brief = "Select SPIBUS 2 MODE",
                description = "Choices are IRQ(0, default), DMA(1) or polling (2)",
                choices = { "0", "1", "2"},
                default = "0",
                file = "include/cfg/spi.h"
            },
        }
    },

    --
    -- STM32F SPI3 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spi3",
        brief = "STM32F SPI3 Bus Controller",
        description = "3. SPI Controller with up to four Chip select line.",
        requires = { "HW_SPI3_STM32" },
        provides = { "SPIBUS_CONTRROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi3.c" },
        options =
        {
            {
                macro = "SPI3_REMAP",
                brief = "Use SPI3 Alternate Pins",
                description = "Choose SPI3 Pin remap. Default = 0 means no remap.\n\n"..
                              "Default 0 Pin Mapping is:\n NSS PA15\n SCK PB03\n MISO PB04\n MOSI PB05\n"..
                              "Remap   1 Pin Mapping is:\n NSS PA04\n SCK PC10\n MISO PC11\n MOSI PC12\n",
                requires = { "HW_GPIO_STM32V1" },
                type = "enumerated",
                choices = {"0", "1"},
                default = "0",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI3_CS0",
                brief = "SPI3 CS0 Gpio",
                description = "Choice of SPI3 CS0 Gpio.\n"..
                              "Default is default NSS pin.\n",
                              "Use PIN_NONE if no CS is required.\n",
                default = "PA04",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI3_CS1",
                brief = "SPI3 CS1 Gpio",
                description = "Choice of SPI3 CS1 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI3_CS2",
                brief = "SPI3 CS2 Gpio",
                description = "Choice of SPI3 CS2 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI3_CS3",
                brief = "SPI3 CS3 Gpio",
                description = "Choice of SPI3 CS3 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI3_NSS",
                brief = "SPI3 NSS Pins",
                description = "Choice of SPI3 NSS Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi3Nss() end,
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI3_SCK",
                brief = "SPI3 SCK Pins",
                description = "Choice of SPI3 SCK Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi3Sck() end,
                default = "PB03",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI3_MISO",
                brief = "SPI3 MISO Pins",
                description = "Choice of SPI3 MISO Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi3Miso() end,
                default = "PB04",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI3_MOSI",
                brief = "SPI3 MOSI Pins",
                description = "Choice of SPI3 MOSI Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi3Mosi() end,
                default = "PB05",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI3_I2S_MODE",
                brief = "I2S Mode (Third Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS3_MODE",
                brief = "Select SPIBUS 3 MODE",
                description = "Choices are IRQ(0, default), DMA(1) or polling (2)",
                choices = { "0", "1", "2"},
                default = "0",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI3_DMA_TX_ALTERNATE_STREAM",
                brief = "Select SPI3 Alternate TX DMA stream ",
                description = "Select to use stream 7 versus stream 5(default) as SPI3 TX",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI3_DMA_RX_ALTERNATE_STREAM",
                brief = "Select SPI3 Alternate RX DMA stream ",
                description = "Select to use stream 2 versus stream 0(default) as SPI3 RX",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
        },
    },

    --
    -- STM32F SPI4 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spi4",
        brief = "STM32F SPI4 Bus Controller",
        description = "STM32 SPI4 Bus Controller",
        requires = { "HW_SPI4_STM32" },
        provides = { "SPIBUS_CONTRROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi4.c" },
        options =
        {
-- Not available on F1, so no global remap
            {
                macro = "SPI4_CS0",
                brief = "SPI4 CS0 Gpio",
                description = "Choice of SPI4 CS0 Gpio.\n"..
                              "Default is default NSS pin.\n",
                              "Use PIN_NONE if no CS is required.\n",
                default = "PE04",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI4_CS1",
                brief = "SPI4 CS1 Gpio",
                description = "Choice of SPI4 CS1 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI4_CS2",
                brief = "SPI4 CS2 Gpio",
                description = "Choice of SPI4 CS2 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI4_CS3",
                brief = "SPI4 CS3 Gpio",
                description = "Choice of SPI4 CS3 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI4_NSS",
                brief = "SPI4 NSS Pins",
                description = "Choice of SPI4 NSS Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi4Nss() end,
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI4_SCK",
                brief = "SPI4 SCK Pins",
                description = "Choice of SPI4 SCK Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi4Sck() end,
                default = "PE03",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI4_MISO",
                brief = "SPI4 MISO Pins",
                description = "Choice of SPI4 MISO Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi4Miso() end,
                default = "PE05",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI4_MOSI",
                brief = "SPI4 MOSI Pins",
                description = "Choice of SPI4 MOSI Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi4Mosi() end,
                default = "PE06",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI4_I2S_MODE",
                brief = "I2S Mode (Third Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS4_MODE",
                brief = "Select SPIBUS 4 MODE",
                description = "Choices are IRQ(0, default), DMA(1) or polling (2)",
                choices = { "0", "1", "2"},
                default = "0",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI4_DMA_TX_ALTERNATE_STREAM",
                brief = "Select SPI4 Alternate TX DMA stream ",
                description = "Select to use stream 4 versus stream 1(default) as SPI4 TX",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI4_DMA_RX_ALTERNATE_STREAM",
                brief = "Select SPI4 Alternate RX DMA stream ",
                description = "Select to use stream 3 versus stream 0(default) as SPI4 TX",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
        },
    },

    --
    -- STM32F SPI5 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spi5",
        brief = "STM32F SPI5 Bus Controller",
        description = "STM32F SPI5 Bus Controller",
        requires = { "HW_SPI5_STM32" },
        provides = { "SPIBUS_CONTRROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi5.c" },
        options =
        {
-- Not available on F1, so no global remap
            {
                macro = "SPI5_CS0",
                brief = "SPI5 CS0 Gpio",
                description = "Choice of SPI5 CS0 Gpio.\n"..
                              "Default is default NSS pin.\n",
                              "Use PIN_NONE if no CS is required.\n",
                default = function() return GetSpi5NssDefault() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI5_CS1",
                brief = "SPI5 CS1 Gpio",
                description = "Choice of SPI5 CS1 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI5_CS2",
                brief = "SPI5 CS2 Gpio",
                description = "Choice of SPI5 CS2 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI5_CS3",
                brief = "SPI5 CS3 Gpio",
                description = "Choice of SPI5 CS3 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI5_NSS",
                brief = "SPI5 NSS Pins",
                description = "Choice of SPI5 NSS Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi5Nss() end,
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI5_SCK",
                brief = "SPI5 SCK Pins",
                description = "Choice of SPI5 SCK Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi5Sck() end,
                default = function() return GetSpi5SckDefault() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI5_MISO",
                brief = "SPI5 MISO Pins",
                description = "Choice of SPI5 MISO Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi5Miso() end,
                default = function() return GetSpi5MisoDefault() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI5_MOSI",
                brief = "SPI5 MOSI Pins",
                description = "Choice of SPI5 MOSI Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                choices = function() return GetSpi5Mosi() end,
                default = function() return GetSpi5MosiDefault() end,
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI5_I2S_MODE",
                brief = "I2S Mode (Third Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS5_MODE",
                brief = "Select SPIBUS 5 MODE",
                description = "Choices are IRQ(0, default), DMA(1) or polling (2)",
                choices = { "0", "1", "2"},
                default = "0",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI5_DMA_TX_ALTERNATE_STREAM",
                brief = "Select SPI5 Alternate TX DMA stream ",
                description = "Select to use stream 6 versus stream 4(default) as SPI5 TX",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI5_DMA_RX_ALTERNATE_STREAM",
                brief = "Select SPI5 Alternate RX DMA stream ",
                description = "Select to use stream 5 versus stream 2(default) as SPI5 RX",
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
        },
    },

    --
    -- STM32F SPI6 Interface
    --
    {
        name = "nutarch_cm3_stm32f_spi6",
        brief = "STM32F SPI6 Bus Controller",
        description = "STM32F SPI6 Bus Controller",
        requires = { "HW_SPI6_STM32" },
        provides = { "SPIBUS_CONTRROLLER", "HW_SPI_STM32" },
        sources =  { "cm3/dev/stm/stm32_spi6.c" },
        options =
        {
-- Not available on F1, so no global remap
            {
                macro = "SPI6_CS0",
                brief = "SPI6 CS0 Gpio",
                description = "Choice of SPI6 CS0 Gpio.\n"..
                              "Default is default NSS pin.\n",
                              "Use PIN_NONE if no CS is required.\n",
                default = "PG08",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI6_CS1",
                brief = "SPI6 CS1 Gpio",
                description = "Choice of SPI6 CS1 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI6_CS2",
                brief = "SPI6 CS2 Gpio",
                description = "Choice of SPI6 CS2 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI6_CS3",
                brief = "SPI6 CS3 Gpio",
                description = "Choice of SPI6 CS3 Gpio.",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI6_NSS",
                brief = "SPI6 NSS Pins",
                description = "Choice of SPI6 NSS Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                default = "PIN_NONE",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI6_SCK",
                brief = "SPI6 SCK Pins",
                description = "Choice of SPI6 SCK Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                default = "PG13",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI6_MISO",
                brief = "SPI6 MISO Pins",
                description = "Choice of SPI6 MISO Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                default = "PG12",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI6_MOSI",
                brief = "SPI6 MOSI Pins",
                description = "Choice of SPI6 MOSI Pins",
                requires = { "HW_GPIO_STM32V2" },
                type = "enumerated",
                default = "PG14",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPI6_I2S_MODE",
                brief = "I2S Mode (Third Controller)",
                description = "If enabled, the controller will transfer I2S data.\n\n"..
                              "Under development.",
                requires = { "NOT_AVAILABLE" },
                flavor = "boolean",
                file = "include/cfg/spi.h"
            },
            {
                macro = "SPIBUS6_MODE",
                brief = "Select SPIBUS 6 MODE",
                description = "Choices are IRQ(0, default), DMA(1) or polling (2)",
                choices = { "0", "1", "2"},
                default = "0",
                file = "include/cfg/spi.h"
            },
        },
    },
}
