--
-- Copyright (C) 2004-2007 by egnite Software GmbH. All rights reserved.
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
-- THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
-- ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
-- LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
-- FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
-- SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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

-- Operating system functions
--
-- $Log$
-- Revision 1.54  2009/02/13 14:46:19  haraldkipp
-- Using the VS10xx driver as a template for redesign.
--
-- Revision 1.53  2009/02/06 15:48:08  haraldkipp
-- New audio driver for VLSI chips uses the SPI bus.
--
-- Revision 1.52  2009/01/30 08:57:16  haraldkipp
-- Added auto detected VS10xx.
-- Default audio decoder reset recover times changed to 0.
--
-- Revision 1.51  2009/01/19 10:38:54  haraldkipp
-- Configuration of the AT45D non-volatile memory support was not useable.
--
-- Revision 1.50  2009/01/18 16:44:07  haraldkipp
-- Added GPIO based bit banging SPI bus driver.
--
-- Revision 1.49  2009/01/09 17:59:05  haraldkipp
-- Added target independent AT45D block device drivers and non-volatile
-- memory support based on the new bus controllers.
--
-- Revision 1.48  2008/12/24 14:52:10  thiagocorrea
-- Improve Serial Bit Bang drivers dependency check in nutconf.
--
-- Revision 1.47  2008/10/23 08:52:20  haraldkipp
-- New software SPI MMC routines added.
--
-- Revision 1.46  2008/10/05 16:56:15  haraldkipp
-- Added Helix audio device.
--
-- Revision 1.45  2008/10/03 11:31:27  haraldkipp
-- Added TWI support for the AT91SAM9260.
--
-- Revision 1.44  2008/09/23 07:26:51  haraldkipp
-- Made early watchdog setting configurable.
--
-- Revision 1.43  2008/09/02 14:27:15  haraldkipp
-- Added platform independent reset functions.
-- Disabled the old vs1001k driver.
--
-- Revision 1.42  2008/08/28 16:11:31  haraldkipp
-- Fixed bitbanging SPI for ARM targets.
--
-- Revision 1.41  2008/08/28 11:07:06  haraldkipp
-- Added platform independant driver for LAN91 chips, currently LAN91C111
-- only. This is also the first Ethernet driver with ioctl support.
--
-- Revision 1.40  2008/07/29 07:30:57  haraldkipp
-- Added VS1053B.
--
-- Revision 1.39  2008/07/09 14:25:06  haraldkipp
-- Made EEPROM_PAGE_SIZE configurable. Does it really make sense?
--
-- Revision 1.38  2008/02/15 16:55:43  haraldkipp
-- Added support for VS1033/53 codec.
--
-- Revision 1.37  2007/08/30 12:15:06  haraldkipp
-- Configurable MMC timings.
--
-- Revision 1.36  2007/04/12 08:57:19  haraldkipp
-- New VS10XX decoder support. It will replace the old VS1001K driver, but
-- hasn't been tested for this chip yet. For the time being, please add the
-- vs10xx.c to your application code when using the VS1011E.
-- New API added, which allows to program external AVR devices via SPI.
-- Configurable SPI support added. Polled hardware SPI only, which is
-- currently limited to AVR. Header files are available for up to 4 software
-- SPI devices, but the API routines had been implemented for device 0 only.
--
-- Revision 1.35  2007/03/22 08:23:41  haraldkipp
-- Added the user (green) LED settings for Ethernut 3.0.
--
-- Revision 1.34  2007/02/15 16:17:27  haraldkipp
-- Configurable port bits for bit-banging I2C. Should work now on all
-- AT91 MCUs.
--
-- Revision 1.33  2006/10/05 17:16:50  haraldkipp
-- Hardware independant RTC layer added.
--
-- Revision 1.32  2006/09/29 12:41:55  haraldkipp
-- Added support for AT45 serial DataFlash memory chips. Currently limited
-- to AT91 builds.
--
-- Revision 1.31  2006/08/05 12:00:39  haraldkipp
-- Added clock settings for Ethernut 3.0 Rev-E.
--
-- Revision 1.30  2006/06/30 22:08:23  christianwelzel
-- DS1307 RTC Driver added.
--
-- Revision 1.29  2006/06/28 17:22:34  haraldkipp
-- Make it compile for AT91SAM7X256.
--
-- Revision 1.28  2006/06/28 14:31:55  haraldkipp
-- Null device added to Configurator build.
--
-- Revision 1.27  2006/05/25 09:13:23  haraldkipp
-- Platform independent watchdog API added.
--
-- Revision 1.26  2006/04/07 12:26:59  haraldkipp
-- Removing requirement for non-volatile hardware solves link problem
-- if no such hardware is available.
--
-- Revision 1.25  2006/02/23 15:42:26  haraldkipp
-- MMC low level bit banging SPI added.
--
-- Revision 1.24  2006/01/23 17:29:14  haraldkipp
-- X1226/X1286 EEPROM now available for system configuration storage.
--
-- Revision 1.23  2006/01/22 17:35:22  haraldkipp
-- Baudrate calculation for Ethernut 3 failed if MCU Clock was not specified.
--
-- Revision 1.22  2006/01/05 16:49:06  haraldkipp
-- Mulimedia Card Block Device Driver added.
-- Programmable Logic Device added, which supports the CPLD
-- implementation of the Ethernut 3 design.
-- New options added for CY2239x support.
--
-- Revision 1.21  2005/10/24 09:54:55  haraldkipp
-- New i2C bit banging driver.
-- New Xicor RTC driver.
-- New Cypress programmable clock driver.
--
-- Revision 1.20  2005/10/04 05:48:11  hwmaier
-- Added CAN driver for AT90CAN128
--
-- Revision 1.19  2005/07/26 15:41:06  haraldkipp
-- All target dependent code is has been moved to a new library named
-- libnutarch. Each platform got its own script.
--
-- Revision 1.18  2005/05/27 13:51:07  olereinhardt
-- Added new Display sizes for hd44780 and hd44780_bus
-- Ports used for display connection still need's to be added.
--
-- Revision 1.17  2005/04/05 17:44:56  haraldkipp
-- Made stack space configurable.
--
-- Revision 1.16  2005/02/21 00:56:59  hwmaier
-- New CAN int vectors ivect35.c and ivect36 added, removed "makedefs" entry for RTL_IRQ_RISING_EDGE.
--
-- Revision 1.15  2005/02/19 22:47:54  hwmaier
-- no message
--
-- Revision 1.14  2005/02/07 19:05:25  haraldkipp
-- ATmega 103 compile errors fixed
--
-- Revision 1.13  2005/02/02 19:46:53  haraldkipp
-- Port configuration was completely broken, because no AVRPORT values
-- had been defined for the preprocessor. To fix this without modifying
-- too many sourcefiles we change the name of AVR port config file and
-- include this new file in the old avr.h.
--
-- Revision 1.12  2005/01/22 19:22:42  haraldkipp
-- Changed AVR port configuration names from PORTx to AVRPORTx.
-- Removed uartspi devices.
--
-- Revision 1.11  2004/12/17 15:31:28  haraldkipp
-- Support of rising edge interrupts for hardware w/o inverter gate.
-- Fixed compilation issue for hardware with RTL reset port.
-- Thanks to FOCUS Software Engineering Pty Ltd.
--
-- Revision 1.10  2004/11/24 14:48:34  haraldkipp
-- crt/crt.nut
--
-- Revision 1.9  2004/10/03 18:39:12  haraldkipp
-- GBA debug output on screen
--
-- Revision 1.8  2004/09/25 15:43:54  drsung
-- Light change for separate interrupt stack. Now it depends also
-- on macro HW_MCU_AVR.
--
-- Revision 1.7  2004/09/22 08:21:43  haraldkipp
-- No ATmega103 support for LAN91C111. Is there any hardware?
-- List of devices sorted by platform.
-- Separate IRQ stack for AVR is configurable.
-- Configurable ports for digital I/O shift register.
-- Configurable handshake ports for AVR USART.
--
-- Revision 1.6  2004/09/07 19:11:15  haraldkipp
-- Simplified IRQ handling to get it done for EB40A
--
-- Revision 1.5  2004/09/01 14:04:57  haraldkipp
-- Added UART handshake and EEPROM emulation port bits
--
-- Revision 1.4  2004/08/18 16:05:38  haraldkipp
-- Use consistent directory structure
--
-- Revision 1.3  2004/08/18 13:46:09  haraldkipp
-- Fine with avr-gcc
--
-- Revision 1.2  2004/08/03 15:09:31  haraldkipp
-- Another change of everything
--
-- Revision 1.1  2004/06/07 16:35:53  haraldkipp
-- First release
--
--

nutdev =
{
    --
    -- General device helper routines.
    --
    {
        name = "nutdev_ihndlr",
        brief = "Interrupt Handler",
        description = "Interrupt registration and distribution.",
        sources = { "ihndlr.c" },
    },
    {
        name = "nutdev_usart",
        brief = "USART Driver Framework",
        description = "Generic USART driver framework.",
        requires = { "CRT_HEAPMEM", "DEV_UART_SPECIFIC" },
        provides = { "DEV_UART_GENERIC", "DEV_FILE", "DEV_READ", "DEV_WRITE" },
        sources = { "usart.c" },
    },
    {
        name = "nutdev_chat",
        brief = "UART Chat",
        description = "Executes a conversational exchange with a serial device."..
                      "Typically used for modem dial commands and login scripts.",
        requires = { "CRT_HEAPMEM", "DEV_UART", "NUT_TIMER" },
        provides = { "UART_CHAT" },
        sources =  { "chat.c" },
        options =
        {
            {
                macro = "CHAT_MAX_ABORTS",
                brief = "Max. Number of Aborts",
                description = "Default is 10.",
                file = "include/cfg/chat.h"
            },
            {
                macro = "CHAT_MAX_REPORT_SIZE",
                brief = "Max. Size of Reports",
                description = "Default is 32",
                file = "include/cfg/chat.h"
            },
            {
                macro = "CHAT_DEFAULT_TIMEOUT",
                brief = "Default Timeout",
                description = "Specify the number of seconds. "..
                              "Default is 45",
                file = "include/cfg/chat.h"
            }
        }
    },
    {
        name = "nutdev_term",
        brief = "Terminal Emulation",
        requires = { "CRT_HEAPMEM" },
        sources = { "term.c" }
    },
    {
        name = "nutdev_netbuf",
        brief = "Network Buffers",
        provides = { "DEV_NETBUF" },
        sources = { "netbuf.c" }
    },
    {
        name = "nutdev_can",
        brief = "CAN Driver Framework",
        description = "Generic CAN driver framework.",
        requires = { "DEV_CAN_SPECIFIC" },
        provides = { "DEV_CAN_GENERIC"},
        sources = { "can_dev.c" },
    },
    {
        name = "nutdev_nvmem",
        brief = "Non Volatile Memory",
        description = "The Nut/OS API provides routines for platform independent access "..
                      "to non-volatile memory. A part of this memory is used by the system "..
                      "to store global settings like the host name or network parameters, "..
                      "while the remaining part is available for application settings.",
        sources = { "nvmem.c" },
        options =
        {
            {
                macro = "NUT_CONFIG_AT45D",
                brief = "Atmel AT45D DataFlash",
                description = "If enabled, the Atmel AT45D DataFlash is used for non-volatile memory."..
                              "Additionally specify the chip index as 0 for the first, 1 for the second etc.\n\n"..
                              "For the AVR family the on-chip EEPROM is used by default.",
                provides = { "DEV_NVMEM", "DEV_NVMEM_AT45D" },
                flavor = "booldata",
                exclusivity = { "NUT_CONFIG_X12RTC", "NUT_CONFIG_AT45D", "NUT_CONFIG_AT49BV", "NUT_CONFIG_AT91EFC" },
                default = "0",
                file = "include/cfg/eeprom.h"
            },
            {
                macro = "NUT_CONFIG_AT45DB",
                brief = "Atmel AT45DB DataFlash",
                description = "Deprecated, uses old SPI routines.",
                provides = { "DEV_NVMEM", "DEV_NVMEM_AT45DB" },
                flavor = "boolean",
                exclusivity = { "NUT_CONFIG_X12RTC", "NUT_CONFIG_AT45D", "NUT_CONFIG_AT45DB", "NUT_CONFIG_AT49BV", "NUT_CONFIG_AT91EFC" },
                file = "include/cfg/eeprom.h"
            },
            {
                macro = "NUT_CONFIG_AT49BV",
                brief = "Atmel AT49BV NOR Flash",
                description = "If enabled, Nut/OS and Nut/Net configurations will "..
                              "be stored in this chip.",
                provides = { "DEV_NVMEM", "DEV_NVMEM_NORFLASH" },
                flavor = "boolean",
                exclusivity = { "NUT_CONFIG_X12RTC", "NUT_CONFIG_AT45D", "NUT_CONFIG_AT45DB", "NUT_CONFIG_AT49BV", "NUT_CONFIG_AT91EFC" },
                file = "include/cfg/eeprom.h"
            },
            {
                macro = "NUT_CONFIG_AT91EFC",
                brief = "AT91 On-Chip Flash",
                description = "If enabled, Nut/OS and Nut/Net configurations will "..
                              "be stored in on-chip flash memory.",
                requires = { "HW_EFC_AT91" },
                provides = { "DEV_NVMEM", "DEV_NVMEM_NORFLASH" },
                flavor = "boolean",
                exclusivity = { "NUT_CONFIG_X12RTC", "NUT_CONFIG_AT45D", "NUT_CONFIG_AT45DB", "NUT_CONFIG_AT49BV", "NUT_CONFIG_AT91EFC" },
                file = "include/cfg/eeprom.h"
            },
            {
                macro = "NUT_CONFIG_X12RTC",
                brief = "Intersil X12x6 EEPROM",
                description = "If enabled, the EEPROM on the Intersil X1226/X1286 chip is used for non-volatile memory.",
                provides = { "DEV_NVMEM" },
                flavor = "boolean",
                exclusivity = { "NUT_CONFIG_X12RTC", "NUT_CONFIG_AT45D", "NUT_CONFIG_AT45DB", "NUT_CONFIG_AT49BV", "NUT_CONFIG_AT91EFC" },
                file = "include/cfg/eeprom.h"
            },
            {
                macro = "NUT_CONFIG_AT45D_CS",
                brief = "AT45D Chip Select",
                description = "Chip select number.",
                requires = { "DEV_NVMEM_AT45D" },
                default = "0",
                type = "enumerated",
                choices = { "0", "1", "2", "3" },
                file = "include/cfg/eeprom.h"
            },
            {
                macro = "NUT_CONFIG_AT45D_PAGE",
                brief = "AT45D Start Page",
                description = "First page used by the configuration data area. "..
                              "By default the last page will be used.",
                requires = { "DEV_NVMEM_AT45D" },
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "NUT_CONFIG_AT45D_SIZE",
                brief = "AT45D Area Size",
                description = "Size of the configuration data area."..
                              "By default one full page will be used.",
                requires = { "DEV_NVMEM_AT45D" },
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "AT45_CONF_SIZE",
                brief = "Configuration Area Size",
                description = "During write operations a buffer with this size is allocated "..
                              "from heap and may cause memory problems with large sectors. "..
                              "Thus, this value may be less than the size of the configuration "..
                              "sector, in which case the rest of the sector is unused.",
                requires = { "DEV_NVMEM_AT45DB" },
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "FLASH_CONF_SECTOR",
                brief = "Flash Sector Address",
                description = "Address of the sector used for configuration data.\n\n"..
                              "Specify the relative memory address, e.g. 0x0000 for the first sector.",
                requires = { "DEV_NVMEM_NORFLASH" },
                default = "0x6000",
                file = "include/cfg/memory.h"
            },
            {
                macro = "FLASH_CONF_SIZE",
                brief = "Configuration Area Size",
                description = "During write operations a buffer with this size is allocated "..
                              "from heap and may cause memory problems with large sectors. "..
                              "Thus, this value may be less than the size of the configuration "..
                              "sector, in which case the rest of the sector is unused.",
                requires = { "DEV_NVMEM_NORFLASH" },
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
        }
    },
    {
        name = "nutdev_reset",
        brief = "System Reset",
        sources = { "reset.c" },
    },
    {
        name = "nutdev_watchdog",
        brief = "Watchdog Timer",
        description = "Platform independent watchdog API.\n\n"..
                      "The watchdog timer prevents system lock-up.",
        sources = { "watchdog.c" },
        options =
        {
            {
                macro = "NUT_WDT_START",
                brief = "Start Value",
                description = "If enabled, the watchdog timer will be started during system "..
                              "initialization with the specified register value.\n\n"..
                              "On some systems, e.g. the AT91SAM7, the watchdog timer is "..
                              "enabled by default. If the start value is zero, the watchdog "..
                              "register is not touched. If this option is disabled, the "..
                              "watchdog timer will be disabled during system initialization. "..
                              "It is typically not possible to re-enable the watchdog timer.\n\n"..
                              "Note, that this setting is optional and may not be available "..
                              "for all targets.",
                requires = { "HW_GPIO" },
                flavor = "booldata",
                file = "include/cfg/clock.h"
            }
        }
    },

    --
    -- Simple Interface Drivers.    
    --
    {
        name = "nutdev_sbbif0",
        brief = "Serial Bit Banged Interface 0",
        description = "Software SPI0, master mode only.",
        provides = { "DEV_SPI" },
        requires = { "HW_GPIO" },
        sources = { 
            "sbbif0.c",
            "spibus_gpio.c",
            "spibus0gpio.c"
        },
        options =
        {
            {
                macro = "SBBI0_SCK_PORT",
                brief = "SCK Port",
                description = "ID of the port used for SPI clock.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_SCK_BIT",
                brief = "SCK Port Bit",
                description = "Port bit used for SPI clock.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_MOSI_PORT",
                brief = "MOSI Port",
                description = "ID of the port used for SPI data output.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_MOSI_BIT",
                brief = "MOSI Port Bit",
                description = "Port bit used for SPI data output.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_MISO_PORT",
                brief = "MISO Port",
                description = "ID of the port used for SPI data input.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_MISO_BIT",
                brief = "MISO Port Bit",
                description = "Port bit used for SPI data input.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_CS0_PORT",
                brief = "CS0 Port",
                description = "ID of the port used for SPI chip select 0.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_CS0_BIT",
                brief = "CS0 Port Bit",
                description = "Port bit used for SPI chip select 0.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_CS1_PORT",
                brief = "CS1 Port",
                description = "ID of the port used for SPI chip select 1.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_CS1_BIT",
                brief = "CS1 Port Bit",
                description = "Port bit used for SPI chip select 1.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_CS2_PORT",
                brief = "CS2 Port",
                description = "ID of the port used for SPI chip select 2.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_CS2_BIT",
                brief = "CS2 Port Bit",
                description = "Port bit used for SPI chip select 2.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_CS3_PORT",
                brief = "CS3 Port",
                description = "ID of the port used for SPI chip select 3.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_CS3_BIT",
                brief = "CS3 Port Bit",
                description = "Port bit used for SPI chip select 3.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_RST0_PORT",
                brief = "RESET0 Port",
                description = "ID of the port used for SPI chip reset 0.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_RST0_BIT",
                brief = "RESET0 Port Bit",
                description = "Port bit used for SPI chip reset 0.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_RST1_PORT",
                brief = "RESET1 Port",
                description = "ID of the port used for SPI chip reset 1.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_RST1_BIT",
                brief = "RESET1 Port Bit",
                description = "Port bit used for SPI chip reset 1.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_RST2_PORT",
                brief = "RESET2 Port",
                description = "ID of the port used for SPI chip reset 2.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_RST2_BIT",
                brief = "RESET2 Port Bit",
                description = "Port bit used for SPI chip reset 2.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_RST3_PORT",
                brief = "RESET3 Port",
                description = "ID of the port used for SPI chip reset 3.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI0_RST3_BIT",
                brief = "RESET3 Port Bit",
                description = "Port bit used for SPI chip reset 3.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },

        },
    },
    {
        name = "nutdev_sbbif1",
        brief = "Serial Bit Banged Interface 1",
        description = "Software SPI1, master mode only.",
        provides = { "DEV_SPI" },
        requires = { "HW_GPIO" },
        options =
        {
            {
                macro = "SBBI1_SCK_PORT",
                brief = "SCK Port",
                description = "ID of the port used for SPI clock.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_SCK_BIT",
                brief = "SCK Port Bit",
                description = "Port bit used for SPI clock.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_MOSI_PORT",
                brief = "MOSI Port",
                description = "ID of the port used for SPI data output.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_MOSI_BIT",
                brief = "MOSI Port Bit",
                description = "Port bit used for SPI data output.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_MISO_PORT",
                brief = "MISO Port",
                description = "ID of the port used for SPI data input.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_MISO_BIT",
                brief = "MISO Port Bit",
                description = "Port bit used for SPI data input.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_CS0_PORT",
                brief = "CS0 Port",
                description = "ID of the port used for SPI chip select 0.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_CS0_BIT",
                brief = "CS0 Port Bit",
                description = "Port bit used for SPI chip select 0.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_CS1_PORT",
                brief = "CS1 Port",
                description = "ID of the port used for SPI chip select 1.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_CS1_BIT",
                brief = "CS1 Port Bit",
                description = "Port bit used for SPI chip select 1.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_CS2_PORT",
                brief = "CS2 Port",
                description = "ID of the port used for SPI chip select 2.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_CS2_BIT",
                brief = "CS2 Port Bit",
                description = "Port bit used for SPI chip select 2.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_CS3_PORT",
                brief = "CS3 Port",
                description = "ID of the port used for SPI chip select 3.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_CS3_BIT",
                brief = "CS3 Port Bit",
                description = "Port bit used for SPI chip select 3.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_RST0_PORT",
                brief = "RESET0 Port",
                description = "ID of the port used for SPI chip reset 0.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_RST0_BIT",
                brief = "RESET0 Port Bit",
                description = "Port bit used for SPI chip reset 0.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_RST1_PORT",
                brief = "RESET1 Port",
                description = "ID of the port used for SPI chip reset 1.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_RST1_BIT",
                brief = "RESET1 Port Bit",
                description = "Port bit used for SPI chip reset 1.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_RST2_PORT",
                brief = "RESET2 Port",
                description = "ID of the port used for SPI chip reset 2.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_RST2_BIT",
                brief = "RESET2 Port Bit",
                description = "Port bit used for SPI chip reset 2.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_RST3_PORT",
                brief = "RESET3 Port",
                description = "ID of the port used for SPI chip reset 3.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI1_RST3_BIT",
                brief = "RESET3 Port Bit",
                description = "Port bit used for SPI chip reset 3.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
        },
    },
    {
        name = "nutdev_sbbif2",
        brief = "Serial Bit Banged Interface 2",
        description = "Software SPI2, master mode only.",
        provides = { "DEV_SPI" },
        requires = { "HW_GPIO" },
        options =
        {
            {
                macro = "SBBI2_SCK_PORT",
                brief = "SCK Port",
                description = "ID of the port used for SPI clock.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_SCK_BIT",
                brief = "SCK Port Bit",
                description = "Port bit used for SPI clock.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_MOSI_PORT",
                brief = "MOSI Port",
                description = "ID of the port used for SPI data output.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_MOSI_BIT",
                brief = "MOSI Port Bit",
                description = "Port bit used for SPI data output.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_MISO_PORT",
                brief = "MISO Port",
                description = "ID of the port used for SPI data input.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_MISO_BIT",
                brief = "MISO Port Bit",
                description = "Port bit used for SPI data input.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_CS0_PORT",
                brief = "CS0 Port",
                description = "ID of the port used for SPI chip select 0.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_CS0_BIT",
                brief = "CS0 Port Bit",
                description = "Port bit used for SPI chip select 0.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_CS1_PORT",
                brief = "CS1 Port",
                description = "ID of the port used for SPI chip select 1.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_CS1_BIT",
                brief = "CS1 Port Bit",
                description = "Port bit used for SPI chip select 1.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_CS2_PORT",
                brief = "CS2 Port",
                description = "ID of the port used for SPI chip select 2.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_CS2_BIT",
                brief = "CS2 Port Bit",
                description = "Port bit used for SPI chip select 2.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_CS3_PORT",
                brief = "CS3 Port",
                description = "ID of the port used for SPI chip select 3.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_CS3_BIT",
                brief = "CS3 Port Bit",
                description = "Port bit used for SPI chip select 3.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_RST0_PORT",
                brief = "RESET0 Port",
                description = "ID of the port used for SPI chip reset 0.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_RST0_BIT",
                brief = "RESET0 Port Bit",
                description = "Port bit used for SPI chip reset 0.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_RST1_PORT",
                brief = "RESET1 Port",
                description = "ID of the port used for SPI chip reset 1.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_RST1_BIT",
                brief = "RESET1 Port Bit",
                description = "Port bit used for SPI chip reset 1.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_RST2_PORT",
                brief = "RESET2 Port",
                description = "ID of the port used for SPI chip reset 2.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_RST2_BIT",
                brief = "RESET2 Port Bit",
                description = "Port bit used for SPI chip reset 2.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_RST3_PORT",
                brief = "RESET3 Port",
                description = "ID of the port used for SPI chip reset 3.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI2_RST3_BIT",
                brief = "RESET3 Port Bit",
                description = "Port bit used for SPI chip reset 3.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
        },
    },
    {
        name = "nutdev_sbbif3",
        brief = "Serial Bit Banged Interface 3",
        description = "Software SPI3, master mode only.",
        provides = { "DEV_SPI" },
        requires = { "HW_GPIO" },
        options =
        {
            {
                macro = "SBBI3_SCK_PORT",
                brief = "SCK Port",
                description = "ID of the port used for SPI clock.",
                type = "enumerated",
                choices = avr_port_choice,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_SCK_BIT",
                brief = "SCK Port Bit",
                description = "Port bit used for SPI clock.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_MOSI_PORT",
                brief = "MOSI Port",
                description = "ID of the port used for SPI data output.",
                type = "enumerated",
                choices = avr_port_choice,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_MOSI_BIT",
                brief = "MOSI Port Bit",
                description = "Port bit used for SPI data output.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_MISO_PORT",
                brief = "MISO Port",
                description = "ID of the port used for SPI data input.",
                type = "enumerated",
                choices = avr_port_choice,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_MISO_BIT",
                brief = "MISO Port Bit",
                description = "Port bit used for SPI data input.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_CS0_PORT",
                brief = "CS0 Port",
                description = "ID of the port used for SPI chip select 0.",
                type = "enumerated",
                choices = avr_port_choice,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_CS0_BIT",
                brief = "CS0 Port Bit",
                description = "Port bit used for SPI chip select 0.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_CS1_PORT",
                brief = "CS1 Port",
                description = "ID of the port used for SPI chip select 1.",
                type = "enumerated",
                choices = avr_port_choice,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_CS1_BIT",
                brief = "CS1 Port Bit",
                description = "Port bit used for SPI chip select 1.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_CS2_PORT",
                brief = "CS2 Port",
                description = "ID of the port used for SPI chip select 2.",
                type = "enumerated",
                choices = avr_port_choice,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_CS2_BIT",
                brief = "CS2 Port Bit",
                description = "Port bit used for SPI chip select 2.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_CS3_PORT",
                brief = "CS3 Port",
                description = "ID of the port used for SPI chip select 3.",
                type = "enumerated",
                choices = avr_port_choice,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_CS3_BIT",
                brief = "CS3 Port Bit",
                description = "Port bit used for SPI chip select 3.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_RST0_PORT",
                brief = "RESET0 Port",
                description = "ID of the port used for SPI chip reset 0.",
                type = "enumerated",
                choices = avr_port_choice,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_RST0_BIT",
                brief = "RESET0 Port Bit",
                description = "Port bit used for SPI chip reset 0.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_RST1_PORT",
                brief = "RESET1 Port",
                description = "ID of the port used for SPI chip reset 1.",
                type = "enumerated",
                choices = avr_port_choice,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_RST1_BIT",
                brief = "RESET1 Port Bit",
                description = "Port bit used for SPI chip reset 1.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_RST2_PORT",
                brief = "RESET2 Port",
                description = "ID of the port used for SPI chip reset 2.",
                type = "enumerated",
                choices = avr_port_choice,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_RST2_BIT",
                brief = "RESET2 Port Bit",
                description = "Port bit used for SPI chip reset 2.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_RST3_PORT",
                brief = "RESET3 Port",
                description = "ID of the port used for SPI chip reset 3.",
                type = "enumerated",
                choices = avr_port_choice,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "SBBI3_RST3_BIT",
                brief = "RESET3 Port Bit",
                description = "Port bit used for SPI chip reset 3.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
        },
    },
    {
        name = "nutdev_spibus_npl",
        brief = "NPL SPI Bus Controller",
        description = "Programmable logic SPI bus master controller.",
        requires = { "DEV_PLL" },
        provides = { "SPIBUS_CONTROLLER" },
        sources = { "spibus_npl.c" }
    },
    {
        name = "nutdev_twbbif",
        brief = "Bit Banging Two Wire",
        description = "Tested on AT91 only.",
        requires = { "HW_MCU_SWTWI" },
        provides = { "DEV_TWI" },
        sources = { "twbbif.c" },
        options =
        {
            {
                macro = "TWI_PIO_ID",
                brief = "GPIO ID (AT91)",
                description = "Data and clock line must be connected to the same GPIO port.",
                type = "enumerated",
                choices = at91_pio_id_choice,
                flavor = "integer",
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "TWI_SDA_BIT",
                brief = "GPIO Data Bit (AT91)",
                description = "Port bit number of the TWI data line.",
                requires = { "HW_MCU_AT91" },
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "TWI_SCL_BIT",
                brief = "GPIO Clock Bit (AT91)",
                description = "Port bit number of the TWI clock line.\n",
                requires = { "HW_MCU_AT91" },
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "TWI_DELAY",
                brief = "Delay Loops",
                description = "The number of dummy loops executed after falling and raising clock.",
                default = "16",
                flavor = "integer",
                file = "include/cfg/twi.h"
            },
        },
    },
    
    --
    -- Character Device Drivers.
    --
    {
        name = "nutdev_null",
        brief = "Null",
        description = "This can be useful if your application might write unwanted "..
                      "output to stdout. With this device you can redirect stdout "..
                      "to the nullDev which discards any output.",
        sources = { "null.c" }
    },

    {
        name = "nutdev_hxcodec",
        brief = "Helix Audio Device",
        sources = { "hxcodec.c" },
        requires = { "AUDIO_DECODER_HELIX", "HW_AUDIO_DAC" },
        options =
        {
            {
                macro = "HXCODEC0_OUTPUT_BUFSIZ",
                brief = "Decoder Buffer Size",
                description = "Number of bytes for the decoder buffer.\n\n"..
                              "If not specified, half of available data RAM is used, "..
                              "up to a maximum of 16k.",
                flavor = "booldata",
                file = "include/cfg/audio.h"
            },
            {
                macro = "NUT_THREAD_HXCODEC0STACK",
                brief = "Thread Stack Size",
                description = "Number of bytes for the decoder thread.",
                flavor = "booldata",
                file = "include/cfg/audio.h"
            },
            {
                macro = "HXCODEC0_RESAMPLER",
                brief = "Resampler",
                description = "Enables the Hermite resampler.",
                flavor = "boolean",
                file = "include/cfg/audio.h"
            }
            
        }
    },

    {
        name = "nutdev_spi_mlcd",
        brief = "Mega LCD",
        description = "Currently a dummy without any function.",
        sources = { "spi_mlcd.c" },
        requires = { "SPIBUS_CONTROLLER", "HW_GPIO" },
    },

    {
        name = "nutdev_spi_vscodec0",
        brief = "VLSI Audio Codec",
        description = "Early release tested with VS1053B on SAM7SE.",
        sources = { "spi_vscodec.c", "spi_vscodec0.c" },
        requires = { "SPIBUS_CONTROLLER", "HW_GPIO" },
        options =
        {
            {
                macro = "AUDIO0_VSAUTO",
                brief = "Auto Detect",
                description = "Generates significantly more code. Untested.",
                flavor = "boolean",
                exclusivity = {
                    "AUDIO0_VSAUTO",
                    "AUDIO0_VS1001K",
                    "AUDIO0_VS1011E",
                    "AUDIO0_VS1002D",
                    "AUDIO0_VS1003B",
                    "AUDIO0_VS1033C",
                    "AUDIO0_VS1053B"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "AUDIO0_VS1001K",
                brief = "VS1001K",
                description = "Untested.",
                flavor = "boolean",
                exclusivity = {
                    "AUDIO0_VSAUTO",
                    "AUDIO0_VS1001K",
                    "AUDIO0_VS1011E",
                    "AUDIO0_VS1002D",
                    "AUDIO0_VS1003B",
                    "AUDIO0_VS1033C",
                    "AUDIO0_VS1053B"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "AUDIO0_VS1011E",
                brief = "VS1011E",
                description = "Untested.",
                flavor = "boolean",
                exclusivity = {
                    "AUDIO0_VSAUTO",
                    "AUDIO0_VS1001K",
                    "AUDIO0_VS1011E",
                    "AUDIO0_VS1002D",
                    "AUDIO0_VS1003B",
                    "AUDIO0_VS1033C",
                    "AUDIO0_VS1053B"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "AUDIO0_VS1002D",
                brief = "VS1002D",
                description = "Untested.",
                flavor = "boolean",
                exclusivity = {
                    "AUDIO0_VSAUTO",
                    "AUDIO0_VS1001K",
                    "AUDIO0_VS1011E",
                    "AUDIO0_VS1002D",
                    "AUDIO0_VS1003B",
                    "AUDIO0_VS1033C",
                    "AUDIO0_VS1053B"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "AUDIO0_VS1003B",
                brief = "VS1003B",
                description = "Untested.",
                flavor = "boolean",
                exclusivity = {
                    "AUDIO0_VSAUTO",
                    "AUDIO0_VS1001K",
                    "AUDIO0_VS1011E",
                    "AUDIO0_VS1002D",
                    "AUDIO0_VS1003B",
                    "AUDIO0_VS1033C",
                    "AUDIO0_VS1053B"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "AUDIO0_VS1033C",
                brief = "VS1033C",
                description = "Untested.",
                flavor = "boolean",
                exclusivity = {
                    "AUDIO0_VSAUTO",
                    "AUDIO0_VS1001K",
                    "AUDIO0_VS1011E",
                    "AUDIO0_VS1002D",
                    "AUDIO0_VS1003B",
                    "AUDIO0_VS1033C",
                    "AUDIO0_VS1053B"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "AUDIO0_VS1053B",
                brief = "VS1053B",
                description = "Tested with SAM7SE.",
                flavor = "boolean",
                exclusivity = {
                    "AUDIO0_VSAUTO",
                    "AUDIO0_VS1001K",
                    "AUDIO0_VS1011E",
                    "AUDIO0_VS1002D",
                    "AUDIO0_VS1003B",
                    "AUDIO0_VS1033C",
                    "AUDIO0_VS1053B"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "VSCODEC0_FREQ",
                brief = "Crystal Clock Frequency",
                description = "Frequency of the crystal clock in Hz.\n\n"..
                              "Tested with default of 12,288 MHz only",
                default = "12288000",
                flavor = "booldata",                             
                file = "include/cfg/audio.h"
            },
            {
                macro = "VSCODEC0_SPI_MODE",
                brief = "SPI Mode",
                description = "SPI mode of command channel, 0 is default.",
                type = "integer",
                default = "0",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VSCODEC0_SPI_RATE",
                brief = "SPI Bitrate",
                description = "Interface speed in bits per second, default is VSCODEC_FREQ/6.",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VSCODEC0_XRESET_PORT",
                brief = "XRESET GPIO Port",
                description = "ID of the port used for VS10XX hardware reset.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_XRESET_BIT",
                brief = "XRESET GPIO Bit",
                description = "Port bit used for VS10XX hardware.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_SIGNAL_IRQ",
                brief = "Decoder Interrupt",
                description = "External interrupt line used for DREQ interrupt. "..
                              "Must match DREQ port and bit settings.",
                type = "enumerated",
                choices = gpio_irq_choice,
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_DREQ_PORT",
                brief = "DREQ GPIO Port",
                description = "ID of the port used for VS10XX DREQ.\n\n"..
                              "Must correspond with the selected interrupt input.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_DREQ_BIT",
                brief = "DREQ GPIO Bit",
                description = "Port bit used for VS10XX DREQ.\n\n"..
                              "Must correspond with the selected interrupt input.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_XCS_PORT",
                brief = "XCS GPIO Port",
                description = "ID of the port used for VS10XX XCS.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_XCS_BIT",
                brief = "XCS GPIO Bit",
                description = "Port bit used for VS10XX XCS.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_XDCS_PORT",
                brief = "XDCS GPIO Port",
                description = "ID of the port used for VS10XX XDCS.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_XDCS_BIT",
                brief = "XDCS GPIO Bit",
                description = "Port bit used for VS10XX XDCS.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_VSCS_PORT",
                brief = "VSCS GPIO Port",
                description = "ID of the port used for VS10XX general select.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_VSCS_BIT",
                brief = "VSCS GPIO Bit",
                description = "Port bit used for VS10XX general select.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_BSYNC_PORT",
                brief = "BSYNC GPIO Port",
                description = "Unsupported. ID of the port used for optional VS10XX BSYNC.",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_BSYNC_BIT",
                brief = "BSYNC GPIO Bit",
                description = "Unsupported. Port bit used for optional VS10XX BSYNC.\n\n"..
                              "Required for the VS1001. Other decoders are driven "..
                              "in VS1001 mode, if this bit is defined. However, "..
                              "it is recommended to use this option for the VS1001 "..
                              "only and run newer chips in so called VS1002 native mode.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VSCODEC0_HWRST_DURATION",
                brief = "Hardware Reset Duration",
                description = "Minimum time in milliseconds to held hardware reset low.",
                default = "1",
                flavor = "integer",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VSCODEC0_HWRST_RECOVER",
                brief = "Hardware Reset Recover",
                description = "Milliseconds to wait after hardware reset.",
                default = "0",
                flavor = "integer",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VSCODEC_SWRST_RECOVER",
                brief = "Software Reset Recover",
                description = "Milliseconds to wait after software reset.",
                default = "0",
                flavor = "integer",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VSCODEC0_OUTPUT_BUFSIZ",
                brief = "Decoder Buffer Size",
                description = "Number of bytes for the decoder buffer.\n\n"..
                              "If not specified, half of available data RAM is used, "..
                              "up to a maximum of 16k.",
                flavor = "booldata",
                file = "include/cfg/audio.h"
            },
            {
                macro = "NUT_THREAD_VSCODEC0STACK",
                brief = "Thread Stack Size",
                description = "Number of bytes for the decoder thread.",
                flavor = "booldata",
                file = "include/cfg/audio.h"
            },
        },
    },

    {
        name = "nutdev_vscodec",
        brief = "VS10XX Audio Device",
        description = "Tested with VS1033 and VS1053 on a SAM7SE system. "..
                      "Most options are currently hard coded.",
        sources = { "vscodec.c" },
        requires = { "NOT_AVAILABLE" },
    },

    --
    -- Ethernet Device Drivers.
    --
    {
        name = "nutdev_lan91",
        brief = "SMSC LAN91x Driver",
        description = "LAN driver for SMSC LAN91. Currently supports LAN91C111 only.",
        requires = { "NUT_EVENT", "NUT_TIMER", "HW_GPIO" },
        provides = { "NET_PHY" },
        sources = { "lan91.c" },
        options =
        {
            {
                macro = "LAN91_BASE_ADDR",
                brief = "Controller Base Address",
                description = "The driver supports memory mapped controllers only, using "..
                              "the specified based address.\n\n"..
                              "The Ethernut 2 reference design uses 0xC000.",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "LAN91_SIGNAL_IRQ",
                brief = "Interrupt",
                description = "Ethernet controller interrupt.",
                default = "INT5",
                type = "enumerated",
                choices = avr_irq_choice,
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "LAN91_RESET_GPIO_BIT",
                brief = "Reset Bit",
                description = "Bit number of the Ethernet controller reset output.\n\n"..
                              "Should be disabled when the LAN91 hardware reset "..
                              "is not connected to a port pin.\n",
                provides = { "LAN91_RESET_BIT" },
                flavor = "booldata",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "LAN91_RESET_GPIO_BANK",
                brief = "Reset Port",
                description = "Port register name of the Ethernet controller reset output.",
                requires = { "LAN91_RESET_BIT" },
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = function() return GetGpioHeaderPath() end
            },
        }
    },

    --
    -- Block Device Drivers.
    --
    {
        name = "nutdev_blockdev",
        brief = "Block I/O",
        description = "Generic block I/O driver support routines.",
        sources = { "blockdev.c" }
    },
    {
        name = "nutdev_spi_at45d",
        brief = "AT45D DataFlash Block I/O",
        description = "Block I/O driver for up to four AT45D chips or cards.\n\n"..
                      "This new driver is based on the device independent "..
                      "bus interface and may replace older ones.\n\n"..
                      "Tested with non-volatile memory routines and "..
                      "the raw file system only. This early release will "..
                      "definitely not work with the PHAT file system.",
        requires = { "SPIBUS_CONTROLLER" },
        provides = { "DEV_BLOCKIO" },
        sources = 
        { 
            "spi_at45d.c", 
            "spi_at45d0.c", 
            "spi_at45d1.c", 
            "spi_at45d2.c", 
            "spi_at45d3.c"
        },
        options =
        {
            {
                macro = "MOUNT_OFFSET_AT45D0",
                brief = "Reserved Bottom Pages (First Device)",
                description = "Number of pages reserved at the bottom.\n\n"..
                              "When a file system driver mounts this device, the specified number "..
                              "of pages will be excluded from the volume and may be used for "..
                              "other purposes like storing configuration data or boot loader images.",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "MOUNT_TOP_RESERVE_AT45D0",
                brief = "Reserved Top Pages (First Device)",
                description = "Number of pages reserved at the top.\n\n"..
                              "When a file system driver mounts this device, the specified number "..
                              "of pages will be excluded from the volume. On some targets the "..
                              "top page is used to store internal Nut/OS settings.",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "SPI_RATE_AT45D0",
                brief = "Transfer Rate (First Device)",
                description = "Interface speed in bits per second, default is 33000000 (33Mbps).\n\n"..
                              "If the exact value can't be set, the driver will choose the "..
                              "next lower one. Bit banging interfaces always run at maximum speed.",
                default = "33000000",
                file = "include/cfg/memory.h"
            },
            {
                macro = "SPI_MODE_AT45D0",
                brief = "Transfer Mode (First Device)",
                description = "Either mode 3 (default) or mode 0 is supported.",
                default = "SPI_MODE_3",
                type = "enumerated",
                choices = { "SPI_MODE_3", "SPI_MODE_0" },
                file = "include/cfg/memory.h"
            },
            {
                macro = "SPI_CSHIGH_AT45D0",
                brief = "Pos. Chip Select (First Device)",
                description = "When set, the chip select is driven high to activate the device. "..
                              "By default chip selects are low active.",
                flavor = "boolean",
                file = "include/cfg/memory.h"
            },
            {
                macro = "MOUNT_OFFSET_AT45D1",
                brief = "Reserved Bottom Pages (Second Device)",
                description = "Number of pages reserved at the bottom.\n\n"..
                              "When a file system driver mounts this device, the specified number "..
                              "of pages will be excluded from the volume and may be used for "..
                              "other purposes like storing configuration data or boot loader images.",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "MOUNT_TOP_RESERVE_AT45D1",
                brief = "Reserved Top Pages (Second Device)",
                description = "Number of pages reserved at the top.\n\n"..
                              "When a file system driver mounts this device, the specified number "..
                              "of pages will be excluded from the volume. On some targets the "..
                              "top page is used to store internal Nut/OS settings.",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "SPI_RATE_AT45D1",
                brief = "Transfer Rate (Second Device)",
                description = "Interface speed in bits per second, default is 33000000 (33Mbps).\n\n"..
                              "If the exact value can't be set, the driver will choose the "..
                              "next lower one. Bit banging interfaces always run at maximum speed.",
                description = ".",
                default = "33000000",
                file = "include/cfg/memory.h"
            },
            {
                macro = "SPI_CSHIGH_AT45D1",
                brief = "Pos. Chip Select (Second Device)",
                description = "When set, the chip select is driven high to activate the device. "..
                              "By default chip selects are low active.",
                flavor = "boolean",
                file = "include/cfg/memory.h"
            },
            {
                macro = "MOUNT_OFFSET_AT45D2",
                brief = "Reserved Bottom Pages (Third Device)",
                description = "Number of pages reserved at the bottom.\n\n"..
                              "When a file system driver mounts this device, the specified number "..
                              "of pages will be excluded from the volume and may be used for "..
                              "other purposes like storing configuration data or boot loader images.",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "MOUNT_TOP_RESERVE_AT45D2",
                brief = "Reserved Top Pages (Third Device)",
                description = "Number of pages reserved at the top.\n\n"..
                              "When a file system driver mounts this device, the specified number "..
                              "of pages will be excluded from the volume. On some targets the "..
                              "top page is used to store internal Nut/OS settings.",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "SPI_RATE_AT45D2",
                brief = "Transfer Rate (Third Device)",
                description = "Interface speed in bits per second, default is 33000000 (33Mbps).\n\n"..
                              "If the exact value can't be set, the driver will choose the "..
                              "next lower one. Bit banging interfaces always run at maximum speed.",
                description = ".",
                default = "33000000",
                file = "include/cfg/memory.h"
            },
            {
                macro = "SPI_CSHIGH_AT45D2",
                brief = "Pos. Chip Select (Third Device)",
                description = "When set, the chip select is driven high to activate the device. "..
                              "By default chip selects are low active.",
                flavor = "boolean",
                file = "include/cfg/memory.h"
            },
            {
                macro = "MOUNT_OFFSET_AT45D3",
                brief = "Reserved Bottom Pages (Fourth Device)",
                description = "Number of pages reserved at the bottom.\n\n"..
                              "When a file system driver mounts this device, the specified number "..
                              "of pages will be excluded from the volume and may be used for "..
                              "other purposes like storing configuration data or boot loader images.",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "MOUNT_TOP_RESERVE_AT45D3",
                brief = "Reserved Top Pages (Fourth Device)",
                description = "Number of pages reserved at the top.\n\n"..
                              "When a file system driver mounts this device, the specified number "..
                              "of pages will be excluded from the volume. On some targets the "..
                              "top page is used to store internal Nut/OS settings.",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "SPI_RATE_AT45D3",
                brief = "Transfer Rate (Fourth Device)",
                description = "Interface speed in bits per second, default is 33000000 (33Mbps).\n\n"..
                              "If the exact value can't be set, the driver will choose the "..
                              "next lower one. Bit banging interfaces always run at maximum speed.",
                description = ".",
                default = "33000000",
                file = "include/cfg/memory.h"
            },
            {
                macro = "SPI_CSHIGH_AT45D3",
                brief = "Pos. Chip Select (Fourth Device)",
                description = "When set, the chip select is driven high to activate the device. "..
                              "By default chip selects are low active.",
                flavor = "boolean",
                file = "include/cfg/memory.h"
            },
            {
                macro = "AT45_WRITE_POLLS",
                brief = "Max. Write Poll Number",
                description = "Maximum number of polling loops for page write.",
                default = "1000",
                file = "include/cfg/memory.h"
            }
        }
    },
    {
        name = "nutdev_mmcard",
        brief = "Basic MMC Driver",
        description = "Basic Multimedia card driver. To run this driver, a few low "..
                      "level routines are required for direct hardware access. "..
                      "Tested on AT91 only.",
        requires = { "DEV_MMCLL" },
        provides = { "DEV_BLOCK" },
        sources = { "mmcard.c" },
        options =
        {
            {
                macro = "MMC_BLOCK_SIZE",
                brief = "Block Size",
                description = "Block size in bytes. Do not change unless you are "..
                              "sure that both, the file system and the hardware support it.",
                default = "512",                              
                flavor = "integer",
                file = "include/cfg/mmci.h"
            },
            {
                macro = "MMC_VERIFY_AFTER",
                brief = "Verify Read/Write",
                description = "If enabled, an additional read will be done to verify "..
                              "each read or written sector. This is useful for testing.",
                flavor = "boolean",
                file = "include/cfg/mmci.h"
            },
            {
                macro = "MMC_MAX_INIT_POLLS",
                brief = "Card Init Timeout",
                description = "Max. number of loops waiting for card's idle mode "..
                              "after initialization. An additional delay of 1 ms is "..
                              "added to each loop after one quarter of this value "..
                              "elapsed.",
                default = "512",
                flavor = "integer",
                file = "include/cfg/mmci.h"
            },
            {
                macro = "MMC_MAX_RESET_POLLS",
                brief = "Card Reset Timeout",
                description = "Max. number of loops waiting for card's idle mode "..
                              "after resetting it.",
                default = "255",
                flavor = "integer",
                file = "include/cfg/mmci.h"
            },
            {
                macro = "MMC_MAX_WRITE_POLLS",
                brief = "Card Write Timeout",
                description = "Max. number of loops waiting for card's idle mode "..
                              "after resetting it. An additional delay of 1 ms is "..
                              "added to each loop after 31/32 of this value "..
                              "elapsed.",
                default = "1024",
                flavor = "integer",
                file = "include/cfg/mmci.h"
            },
            {
                macro = "MMC_MAX_WRITE_RETRIES",
                brief = "Card Write Retries",
                description = "Max. number of retries while writing.",
                default = "32",
                flavor = "integer",
                file = "include/cfg/mmci.h"
            },
            {
                macro = "MMC_MAX_READ_RETRIES",
                brief = "Card Read Retries",
                description = "Max. number of retries while reading.",
                default = "8",
                flavor = "integer",
                file = "include/cfg/mmci.h"
            },
            {
                macro = "MMC_MAX_REG_POLLS",
                brief = "Register Read Timeout",
                description = "Max. number of loops while reading a card's register.",
                default = "512",
                flavor = "integer",
                file = "include/cfg/mmci.h"
            },
            {
                macro = "MMC_MAX_CMDACK_POLLS",
                brief = "Command Acknowledge Timeout",
                description = "Max. number of loops waiting for card's acknowledge "..
                              "of a command. An additional delay of 1 ms is "..
                              "added to each loop after three quarter of this value "..
                              "elapsed.",
                default = "1024",
                flavor = "integer",
                file = "include/cfg/mmci.h"
            },
            {
                macro = "MMC_MAX_R1_POLLS",
                brief = "R1 Response Timeout",
                description = "Max. number of loops waiting for card's R1 response. "..
                              "Increase on fast systems and old cards, decrease for "..
                              "faster timeout. ",
                default = "1024",
                flavor = "integer",
                file = "include/cfg/mmci.h"
            },
        },
    },

    --
    -- RTC Support.
    --

    {
        name = "nutdev_rtc",
        brief = "RTC API",
        description = "Hardware independant RTC routines.",
        provides = { "DEV_RTC" },
        sources = { "rtc.c" },
    },
    {
        name = "nutdev_ds1307",
        brief = "DS1307 Driver",
        description = "Dallas DS1307 RTC driver. Tested on AVR (MMnet02) only.",
        requires = { "HW_MCU_AVR" },
        provides = { "DEV_RTC" },
        sources = { "ds1307rtc.c" },
    },
    {
        name = "nutdev_pcf8563",
        brief = "PCF8563 Driver",
        description = "Philips PCF8563 RTC driver. Tested on AT91 only.",
        requires = { "HW_MCU_AT91" },
        provides = { "DEV_RTC" },
        sources = { "pcf8563.c" },
    },
    {
        name = "nutdev_x12rtc",
        brief = "X12xx Driver",
        description = "Intersil X12xx RTC and EEPROM driver. Tested on AT91 only.",
        requires = { "HW_MCU_AT91" },
        provides = { "DEV_RTC" },
        sources = { "x12rtc.c" },
        options =
        {
            {
                macro = "EEPROM_PAGE_SIZE",
                brief = "EEPROM Page Size",
                description = "If enabled, a different page size may be specified.\n\n"..
                              "Do we really need this?",
                flavor = "booldata",
                file = "include/cfg/eeprom.h"
            },
        },
    },

    --
    -- Bus Controller Drivers.
    --

    {
        name = "nutdev_spibus",
        brief = "SPI Bus",
        requires = { "SPIBUS_CONTROLLER" },
        description = "Generic SPI bus routines, which may be used "..
                      "by bus controller implementations.",
        sources = { "spibus.c" }
    },
        
    --
    -- Special Chip Drivers.
    --

    {
        name = "nutdev_vs10xx",
        brief = "VS10XX Audio Decoder",
        description = "Tested with Medianut 2 attached to Ethernut 3.\n\n"..
                      "In the current implementation this file may conflict with "..
                      "the previous VS1001K driver. For now add this to your "..
                      "application's Makefile. ",
        sources = { "vs10xx.c" },
        requires = { "DEV_SPI", "HW_GPIO" },
        options =
        {
            {
                macro = "VS10XX_FREQ",
                brief = "Crystal Clock Frequency",
                description = "Frequency of the crystal clock in Hz.\n\n"..
                              "The clock doubler will be enabled if this value is "..
                              "lower than 20,000,000 Hz.",
                default = "12288000",                              
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SCI_SPI0_DEVICE",
                brief = "Command Hardware SPI (AVR)",
                description = "Hardware SPI 0 device index (chip select) used for command channel.\n\n"..
                              "Specify device index 0, 1, 2 or 3.\n\n"..
                              "Currently supported on the AVR platform only.",
                requires = { "HW_MCU_AVR" },
                flavor = "booldata",
                exclusivity = { 
                    "VS10XX_SCI_SPI0_DEVICE",
                    "VS10XX_SCI_SBBI0_DEVICE",
                    "VS10XX_SCI_SBBI1_DEVICE",
                    "VS10XX_SCI_SBBI2_DEVICE",
                    "VS10XX_SCI_SBBI3_DEVICE"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SCI_SBBI0_DEVICE",
                brief = "Command Software SPI0",
                description = "Software SPI 0 device index (chip select) used for command channel.\n\n"..
                              "Specify device index 0, 1, 2 or 3.\n\n"..
                              "Currently supported on the AVR platform only.",
                flavor = "booldata",
                exclusivity = { 
                    "VS10XX_SCI_SPI0_DEVICE",
                    "VS10XX_SCI_SBBI0_DEVICE",
                    "VS10XX_SCI_SBBI1_DEVICE",
                    "VS10XX_SCI_SBBI2_DEVICE",
                    "VS10XX_SCI_SBBI3_DEVICE"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SCI_SBBI1_DEVICE",
                brief = "Command Software SPI1",
                description = "Software SPI 1 device index (chip select) used for command channel.\n\n"..
                              "Specify device index 0, 1, 2 or 3.\n\n"..
                              "Currently supported on the AVR platform only.",
                flavor = "booldata",
                exclusivity = { 
                    "VS10XX_SCI_SPI0_DEVICE",
                    "VS10XX_SCI_SBBI0_DEVICE",
                    "VS10XX_SCI_SBBI1_DEVICE",
                    "VS10XX_SCI_SBBI2_DEVICE",
                    "VS10XX_SCI_SBBI3_DEVICE"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SCI_SBBI2_DEVICE",
                brief = "Command Software SPI2",
                description = "Software SPI 2 device index (chip select) used for command channel.\n\n"..
                              "Specify device index 0, 1, 2 or 3.\n\n"..
                              "Currently supported on the AVR platform only.",
                flavor = "booldata",
                exclusivity = { 
                    "VS10XX_SCI_SPI0_DEVICE",
                    "VS10XX_SCI_SBBI0_DEVICE",
                    "VS10XX_SCI_SBBI1_DEVICE",
                    "VS10XX_SCI_SBBI2_DEVICE",
                    "VS10XX_SCI_SBBI3_DEVICE"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SCI_SBBI3_DEVICE",
                brief = "Command Software SPI3",
                description = "Software SPI 3 device index (chip select) used for command channel.\n\n"..
                              "Specify device index 0, 1, 2 or 3.\n\n"..
                              "Currently supported on the AVR platform only.",
                flavor = "booldata",
                exclusivity = { 
                    "VS10XX_SCI_SPI0_DEVICE",
                    "VS10XX_SCI_SBBI0_DEVICE",
                    "VS10XX_SCI_SBBI1_DEVICE",
                    "VS10XX_SCI_SBBI2_DEVICE",
                    "VS10XX_SCI_SBBI3_DEVICE"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SCI_MODE",
                brief = "Command SPI Mode",
                description = "SPI mode of command channel, 0 is default.\n\n"..
                              "Mode 0: Leading edge is rising, data sampled on rising edge.\n"..
                              "Mode 1: Leading edge is rising, data sampled on falling edge.\n"..
                              "Mode 2: Leading edge is falling, data sampled on falling edge.\n"..
                              "Mode 3: Leading edge is falling, data sampled on rising edge.\n",
                type = "integer",
                default = "0",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SCI_RATE",
                brief = "Command SPI Bitrate",
                description = "Interface speed in bits per second, default is VS10XX_FREQ/4.\n\n"..
                              "If the exact value can't be set, the driver will choose the "..
                              "next lower one. Bit banging interfaces always run at maximum speed.",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SDI_SPI0_DEVICE",
                brief = "Data Hardware SPI (AVR)",
                description = "Use hardware SPI for data channel.\n\n"..
                              "Currently supported on the AVR platform only.",
                requires = { "HW_MCU_AVR" },
                flavor = "booldata",
                exclusivity = { 
                    "VS10XX_SDI_SPI0_DEVICE",
                    "VS10XX_SDI_SBBI0_DEVICE",
                    "VS10XX_SDI_SBBI1_DEVICE",
                    "VS10XX_SDI_SBBI2_DEVICE",
                    "VS10XX_SDI_SBBI3_DEVICE"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SDI_SBBI0_DEVICE",
                brief = "Data Software SPI0",
                description = "Use software SPI 0 for data channel.",
                flavor = "booldata",
                exclusivity = { 
                    "VS10XX_SDI_SPI0_DEVICE",
                    "VS10XX_SDI_SBBI0_DEVICE",
                    "VS10XX_SDI_SBBI1_DEVICE",
                    "VS10XX_SDI_SBBI2_DEVICE",
                    "VS10XX_SDI_SBBI3_DEVICE"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SDI_SBBI1_DEVICE",
                brief = "Data Software SPI1",
                description = "Use software SPI 1 for data channel.",
                flavor = "booldata",
                exclusivity = { 
                    "VS10XX_SDI_SPI0_DEVICE",
                    "VS10XX_SDI_SBBI0_DEVICE",
                    "VS10XX_SDI_SBBI1_DEVICE",
                    "VS10XX_SDI_SBBI2_DEVICE",
                    "VS10XX_SDI_SBBI3_DEVICE"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SDI_SBBI2_DEVICE",
                brief = "Data Software SPI2",
                description = "Use software SPI 2 for data channel.",
                flavor = "booldata",
                exclusivity = { 
                    "VS10XX_SDI_SPI0_DEVICE",
                    "VS10XX_SDI_SBBI0_DEVICE",
                    "VS10XX_SDI_SBBI1_DEVICE",
                    "VS10XX_SDI_SBBI2_DEVICE",
                    "VS10XX_SDI_SBBI3_DEVICE"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SDI_SBBI3_DEVICE",
                brief = "Data Software SPI3",
                description = "Use software SPI 3 for data channel.",
                flavor = "booldata",
                exclusivity = { 
                    "VS10XX_SDI_SPI0_DEVICE",
                    "VS10XX_SDI_SBBI0_DEVICE",
                    "VS10XX_SDI_SBBI1_DEVICE",
                    "VS10XX_SDI_SBBI2_DEVICE",
                    "VS10XX_SDI_SBBI3_DEVICE"
                },
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SDI_MODE",
                brief = "Data SPI Mode",
                description = "SPI mode of data channel, 0 is default.\n\n"..
                              "Mode 0: Leading edge is rising, data sampled on rising edge.\n"..
                              "Mode 1: Leading edge is rising, data sampled on falling edge.\n"..
                              "Mode 2: Leading edge is falling, data sampled on falling edge.\n"..
                              "Mode 3: Leading edge is falling, data sampled on rising edge.\n",
                type = "integer",
                default = "0",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SDI_RATE",
                brief = "Data SPI Bitrate",
                description = "Interface speed in bits per second, default is VS10XX_FREQ/4.\n\n"..
                              "If the exact value can't be set, the driver will choose the "..
                              "next lower one. Bit banging interfaces always run at maximum speed.",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SELECT_ACTIVE_HIGH",
                brief = "Active High Chip Select",
                description = "Select this option if the chip select is active high.", 
                flavor = "boolean",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_RESET_ACTIVE_HIGH",
                brief = "Active High Reset",
                description = "Select this option if the reset is active high.", 
                flavor = "boolean",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SIGNAL_IRQ",
                brief = "Decoder Interrupt",
                description = "Audio decoder interrupt, default is INT0.",
                default = "INT0",
                type = "enumerated",
                choices = avr_irq_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "VS10XX_DREQ_PIO_ID",
                brief = "DREQ GPIO Port ID (AT91)",
                description = "ID of the port used for VS10XX DREQ.\n\n"..
                              "Must correspond with the selected interrupt input.",
                requires = { "HW_MCU_AT91" },
                type = "enumerated",
                choices = at91_pio_id_choice,
                flavor = "integer",
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "VS10XX_DREQ_BIT",
                brief = "DREQ GPIO Bit (AT91)",
                description = "Port bit used for VS10XX DREQ.\n\n"..
                              "Must correspond with the selected interrupt input.",
                requires = { "HW_MCU_AT91" },
                type = "enumerated",
                choices = mcu_32bit_choice,
                flavor = "integer",
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "VS10XX_XCS_PORT",
                brief = "XCS Port",
                description = "ID of the port used for VS10XX XCS.",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VS10XX_XCS_BIT",
                brief = "XCS Port Bit",
                description = "Port bit used for VS10XX XCS.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VS10XX_XDCS_PORT",
                brief = "XDCS Port",
                description = "ID of the port used for VS10XX XDCS.",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VS10XX_XDCS_BIT",
                brief = "XDCS Port Bit",
                description = "Port bit used for VS10XX XDCS.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VS10XX_BSYNC_PORT",
                brief = "BSYNC Port",
                description = "ID of the port used for optional VS10XX BSYNC.",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VS10XX_BSYNC_BIT",
                brief = "BSYNC Port Bit",
                description = "Port bit used for optional VS10XX BSYNC.\n\n"..
                              "Required for the VS1001. Other decoders are driven "..
                              "in VS1001 mode, if this bit is defined. However, "..
                              "it is recommended to use this option for the VS1001 "..
                              "only and run newer chips in so called VS1002 native mode.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "VS10XX_HWRST_DURATION",
                brief = "Hardware Reset Duration",
                description = "Minimum time in milliseconds to held hardware reset low.",
                default = "1",
                flavor = "integer",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_HWRST_RECOVER",
                brief = "Hardware Reset Recover",
                description = "Milliseconds to wait after hardware reset.",
                default = "0",
                flavor = "integer",
                file = "include/cfg/audio.h"
            },
            {
                macro = "VS10XX_SWRST_RECOVER",
                brief = "Software Reset Recover",
                description = "Milliseconds to wait after software reset.",
                default = "0",
                flavor = "integer",
                file = "include/cfg/audio.h"
            },
            
        },
    },
    
    {
        name = "nutdev_avrtarget",
        brief = "AVR Serial Programming",
        description = "Routines for programming AVR targets via SPI.\n",
        provides = { "DEV_SPI" },
        sources = { "avrtarget.c" },
        options =
        {
            {
                macro = "AVRTARGET_PAGESIZE",
                brief = "Program Flash Page Size",
                description = "Specify the number of bytes.\n\n"..
                              "Later we will determine this automatically.",
                type = "integer",
                default = "128",
                file = "include/cfg/progif.h"
            },
            {
                macro = "AVRTARGET_CHIPERASE_TIMEOUT",
                brief = "Chip Erase Timeout",
                description = "Specify the number of milliseconds.\n\n"..
                              "Later we will determine this automatically.",
                type = "integer",
                default = "200",
                file = "include/cfg/progif.h"
            },
            {
                macro = "AVRTARGET_PAGEWRITE_TIMEOUT",
                brief = "Page Write Timeout",
                description = "Specify the number of milliseconds.\n\n"..
                              "Later we will determine this automatically.",
                type = "integer",
                default = "10",
                file = "include/cfg/progif.h"
            },
            {
                macro = "AVRTARGET_SPI0_DEVICE",
                brief = "Polled Hardware SPI0 Device (AVR)",
                description = "Use hardware SPI0 for programming.\n\n"..
                              "Specify device index 0, 1, 2 or 3.",
                requires = { "HW_MCU_AVR" },
                flavor = "booldata",
                exclusivity = { 
                    "AVRTARGET_SPI",
                    "AVRTARGET_SBBI0",
                    "AVRTARGET_SBBI1",
                    "AVRTARGET_SBBI2",
                    "AVRTARGET_SBBI3"
                },
                file = "include/cfg/progif.h"
            },
            {
                macro = "AVRTARGET_SBBI0_DEVICE",
                brief = "Software SPI0 Device",
                description = "Use software SPI 0 for programming.\n\n"..
                              "Specify device index 0, 1, 2 or 3.",
                flavor = "booldata",
                exclusivity = { 
                    "AVRTARGET_SPI",
                    "AVRTARGET_SBBI0",
                    "AVRTARGET_SBBI1",
                    "AVRTARGET_SBBI2",
                    "AVRTARGET_SBBI3"
                },
                file = "include/cfg/progif.h"
            },
            {
                macro = "AVRTARGET_SBBI1_DEVICE",
                brief = "Software SPI1 Device",
                description = "Use software SPI 1 for programming.\n\n"..
                              "Specify device index 0, 1, 2 or 3.",
                flavor = "booldata",
                exclusivity = { 
                    "AVRTARGET_SPI",
                    "AVRTARGET_SBBI0",
                    "AVRTARGET_SBBI1",
                    "AVRTARGET_SBBI2",
                    "AVRTARGET_SBBI3"
                },
                file = "include/cfg/progif.h"
            },
            {
                macro = "AVRTARGET_SBBI2_DEVICE",
                brief = "Software SPI2 Device",
                description = "Use software SPI 2 for programming.\n\n"..
                              "Specify device index 0, 1, 2 or 3.",
                flavor = "booldata",
                exclusivity = { 
                    "AVRTARGET_SPI",
                    "AVRTARGET_SBBI0",
                    "AVRTARGET_SBBI1",
                    "AVRTARGET_SBBI2",
                    "AVRTARGET_SBBI3"
                },
                file = "include/cfg/progif.h"
            },
            {
                macro = "AVRTARGET_SBBI3_DEVICE",
                brief = "Software SPI3 Device",
                description = "Use software SPI 3 for programming.\n\n"..
                              "Specify device index 0, 1, 2 or 3.",
                flavor = "booldata",
                exclusivity = { 
                    "AVRTARGET_SPI",
                    "AVRTARGET_SBBI0",
                    "AVRTARGET_SBBI1",
                    "AVRTARGET_SBBI2",
                    "AVRTARGET_SBBI3"
                },
                file = "include/cfg/progif.h"
            },
            {
                macro = "AVRTARGET_SELECT_ACTIVE_HIGH",
                brief = "Active High Chip Select",
                description = "Select this option if the chip select is active high.", 
                flavor = "boolean",
                file = "include/cfg/progif.h"
            },
            {
                macro = "AVRTARGET_RESET_ACTIVE_HIGH",
                brief = "Active High Reset",
                description = "Select this option if the reset is active high.", 
                flavor = "boolean",
                file = "include/cfg/progif.h"
            },
            {
                macro = "AVRTARGET_SPI_MODE",
                brief = "SPI Mode",
                description = "SPI mode, 0 is default.\n\n"..
                              "Mode 0: Leading edge is rising, data sampled on rising edge.\n"..
                              "Mode 1: Leading edge is rising, data sampled on falling edge.\n"..
                              "Mode 2: Leading edge is falling, data sampled on falling edge.\n"..
                              "Mode 3: Leading edge is falling, data sampled on rising edge.\n",
                type = "integer",
                default = "0",
                file = "include/cfg/progif.h"
            },
            {
                macro = "AVRTARGET_SPI_RATE",
                brief = "SPI Bitrate",
                description = "Interface speed in bits per second, default is 500000.\n\n"..
                              "If the exact value can't be set, the driver will choose the "..
                              "next lower one. Bit banging interfaces always run at maximum speed.",
                default = "500000",                              
                file = "include/cfg/progif.h"
            },
        },
    },
    {
        name = "nutdev_nvmem_at45d",
        brief = "AT45D Non-Volatile Memory",
        description = "DataFlash chips and cards may be used to store any kind of configuration "..
                      "data, including internal Nut/OS and Nut/Net settings.\n\n"..
                      "This new module will replace existing ones, which either depend "..
                      "on specific target CPUs or which may interfere with other SPI devices.",
        sources = { "nvmem_at45d.c" }
    },
    {
        name = "nutdev_at45db",
        brief = "AT45DB Serial DataFlash Memory",
        description = "Supports multiple chips.\n\n"..
                      "Tested with AT91SAM9260-EK.",
        sources = { "at45db.c" },
        requires = { "HW_MCU_AT91", "HW_SPI_AT91" },
        options =
        {
            {
                macro = "MAX_AT45_DEVICES",
                brief = "Number of Chips",
                description = "Maximum number of chips, which can be used concurrently.",
                default = "1",
                file = "include/cfg/memory.h"
            },
            {
                macro = "AT45_ERASE_WAIT",
                brief = "Max. Page Erase Wait Time",
                description = "Maximum number of milliseconds to wait until the chip "..
                              "becomes ready again after a sector erase command.",
                default = "3000",
                file = "include/cfg/memory.h"
            },
            {
                macro = "AT45_CHIP_ERASE_WAIT",
                brief = "Max. Chip Erase Wait Time",
                description = "Maximum number of milliseconds to wait until the chip "..
                              "becomes ready again after a chip erase command.",
                default = "50000",
                file = "include/cfg/memory.h"
            },
            {
                macro = "AT45_WRITE_POLLS",
                brief = "Max. Write Poll Number",
                description = "Maximum number of polling loops for page write.",
                default = "1000",
                file = "include/cfg/memory.h"
            },
        },
    },
    
    {
        name = "nutdev_at49bv",
        brief = "AT49BV Flash Memory",
        description = "Currently supports a single chip only.\n\n"..
                      "Tested with AT49BV322A and AT49BV002A.",
        sources = { "at49bv.c" },
        options =
        {
            {
                macro = "FLASH_CHIP_BASE",
                brief = "Base Address",
                description = "First memory address of the chip.",
                default = "0x10000000",
                file = "include/cfg/memory.h"
            },
            {
                macro = "FLASH_8BIT",
                brief = "8-Bit Flash",
                description = "If enabled, flash is driven in 8-bit mode. "..
                              "By default Nut/OS supports 16-bit flash.",
                flavor = "boolean",
                file = "include/cfg/memory.h"
            },
            {
                macro = "FLASH_ERASE_WAIT",
                brief = "Max. Sector Erase Wait Time",
                description = "Maximum number of milliseconds to wait until the chip "..
                              "becomes ready again after a sector erase command.",
                default = "3000",
                file = "include/cfg/memory.h"
            },
            {
                macro = "FLASH_CHIP_ERASE_WAIT",
                brief = "Max. Chip Erase Wait Time",
                description = "Maximum number of milliseconds to wait until the chip "..
                              "becomes ready again after a chip erase command.",
                default = "50000",
                file = "include/cfg/memory.h"
            },
            {
                macro = "FLASH_WRITE_POLLS",
                brief = "Max. Write Poll Number",
                description = "Maximum number of polling loops for a byte/word write.",
                default = "1000",
                file = "include/cfg/memory.h"
            },
        },
    },
    {
        name = "nutdev_cy2239x",
        brief = "CY2239x Driver",
        description = "Cypress CY22393/4/5 PLL clock. Tested on AT91 only.",
        requires = { "HW_MCU_AT91" },
        provides = { "DEV_PLL" },
        sources = { "cy2239x.c" },
        options =
        {
            {
                macro = "NUT_PLL_FREF",
                brief = "Reference Clock",
                description = "Frequency of the PLL reference clock in Hz. If enabled and "..
                              "if NUT_CPU_FREQ is not defined, this value will be used to "..
                              "determine the CPU clock as well.",
                flavor = "booldata",
                file = "include/cfg/os.h"
            },
            {
                macro = "NUT_PLL_CPUCLK",
                brief = "CPU Clock Output",
                description = "PLL output used for the CPU Clock\n\n"..
                              "If an output is selected and if NUT_CPU_FREQ is not defined, "..
                              "then the CPU Clock Value will be queried from the PLL Clock "..
                              "Chip.\n\n"..
                              "Select\n"..
                              "0 for for the CLKA Pin\n"..
                              "1 for for the CLKB Pin (Ethernut 3.0 Rev-E)\n"..
                              "2 for for the CLKC Pin (Ethernut 3.0 Rev-D)\n"..
                              "3 for for the CLKD Pin\n"..
                              "4 for for the CLKE Pin",
                type = "enumerated",
                choices = pll_clk_choice,
                file = "include/cfg/clock.h"
            },
            {
                macro = "NUT_PLL_ETHCLK",
                brief = "Ethernet Clock Output",
                description = "PLL output used to drive the Ethernet Controller\n\n"..
                              "Select\n"..
                              "0 for for the CLKA Pin (Ethernut 3.0 Rev-D)\n"..
                              "1 for for the CLKB Pin\n"..
                              "2 for for the CLKC Pin (Ethernut 3.0 Rev-E)\n"..
                              "3 for for the CLKD Pin\n"..
                              "4 for for the CLKE Pin",
                type = "enumerated",
                choices = pll_clk_choice,
                file = "include/cfg/clock.h"
            },
            {
                macro = "NUT_PLL_NPLCLK1",
                brief = "NPL Clock 1 Output",
                description = "PLL output connected to the CPLD GCK1 Pin\n\n"..
                              "Select\n"..
                              "0 for for the CLKA Pin (Ethernut 3.0 Rev-E)\n"..
                              "1 for for the CLKB Pin (Ethernut 3.0 Rev-D)\n"..
                              "2 for for the CLKC Pin\n"..
                              "3 for for the CLKD Pin\n"..
                              "4 for for the CLKE Pin",
                type = "enumerated",
                choices = pll_clk_choice,
                file = "include/cfg/clock.h"
            },
            {
                macro = "NUT_PLL_NPLCLK3",
                brief = "NPL Clock 3 Output",
                description = "PLL output connected to the CPLD GCK3 Pin\n\n"..
                              "Select\n"..
                              "0 for for the CLKA Pin\n"..
                              "1 for for the CLKB Pin\n"..
                              "2 for for the CLKC Pin\n"..
                              "3 for for the CLKD Pin (Ethernut 3.0)\n"..
                              "4 for for the CLKE Pin",
                type = "enumerated",
                choices = pll_clk_choice,
                file = "include/cfg/clock.h"
            }
        }
    },
    {
        name = "nutdev_npl",
        brief = "Nut Programmable Logic",
        description = "CPLD interrupt handling.\n"..
                      "The CPLD on the Ethernut 3 reference design monitors "..
                      "a number of status lines and activates IRQ0 on certain "..
                      "status changes. This includes RS232 handshake inputs, "..
                      "multimedia card detection, RTC alarms and LAN wakeup. "..
                      "Tested on AT91 only.",
        requires = { "DEV_IRQ_AT91" },
        provides = { "DEV_NPL" },
        sources = { "npl.c" },
    },
    {
        name = "nutdev_nplmmc",
        brief = "NPL Multimedia Card Access",
        description = "CPLD implementation of a low level MMC interface. "..
                      "Tested on AT91 only.",
        requires = { "DEV_NPL" },
        provides = { "DEV_MMCLL" },
        sources = { "nplmmc.c" },
    },
    {
        name = "nutdev_npluled",
        brief = "NPL User (green) LED Access",
        description = "CPLD implementation of a user (green) LED interface. "..
                      "Tested on AT91 only.",
        requires = { "DEV_NPL" },
        provides = { "DEV_NPLULED" },
        sources = { "npluled.c" },
    },
    {
        name = "nutdev_sbi0mmc",
        brief = "MMC Software SPI 0 Driver",
        description = "Bit banging implementation of a low level MMC interface. "..
                      "Tested on AT91 only, but should work on most targets.",
        requires = { "HW_GPIO" },
        provides = { "DEV_MMCLL" },
        sources = { "sbi0mmc0.c", "sbi0mmc1.c" },
        options =
        {
            {
                macro = "MMC0_CLK_PIO_ID",
                brief = "Clock Port",
                description = "Port ID of the clock line.\n"..
                              "SD-Card Pin 5, CLK",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_CLK_PIO_BIT",
                brief = "Clock Bit",
                description = "Port bit number of the clock line.\n"..
                              "SD-Card Pin 5, CLK",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_MOSI_PIO_ID",
                brief = "MOSI Port",
                description = "Port ID of the MOSI line.\n"..
                              "SD-Card Pin 2, CMD",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_MOSI_PIO_BIT",
                brief = "MOSI Bit",
                description = "Port bit number of the MOSI line.\n"..
                              "SD-Card Pin 2, CMD",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_MISO_PIO_ID",
                brief = "MISO Port",
                description = "Port ID of the MISO line.\n"..
                              "SD-Card Pin 7, DAT0",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_MISO_PIO_BIT",
                brief = "MISO Bit",
                description = "Port bit number of the MISO line.\n"..
                              "SD-Card Pin 7, DAT0",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_CS0_PIO_ID",
                brief = "Card 0 Select Port",
                description = "Port ID of the first card select line.\n"..
                              "SD-Card Pin 1, DAT3",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_CS0_PIO_BIT",
                brief = "Card 0 Select Bit",
                description = "Port bit number of the first card select line.\n"..
                              "SD-Card Pin 1, DAT3",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_CD0_PIO_ID",
                brief = "Card 0 Detect Port",
                description = "Port ID of the first card's detect line.\n"..
                              "Must use an external interrupt pin. If left "..
                              "empty, then card change detection is disabled.",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_CD0_PIO_BIT",
                brief = "Card 0 Detect Bit",
                description = "Port bit number of the first card's detect line.\n"..
                              "Must use an external interrupt pin. If left "..
                              "empty, then card change detection is disabled.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_WP0_PIO_ID",
                brief = "Card 0 Write Protect Port",
                description = "Port ID of the first card's write protect line.\n"..
                              "Currently ignored.",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_WP0_PIO_BIT",
                brief = "Card 0 Write Protect Bit",
                description = "Port bit number of the first card's write protect line.\n"..
                              "Currently ignored.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_CS1_PIO_ID",
                brief = "Card 1 Select Port",
                description = "Port ID of the second card select line.\n"..
                              "SD-Card Pin 1, DAT3",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_CS1_PIO_BIT",
                brief = "Card 1 Select Bit",
                description = "Port bit number of the second card select line.\n"..
                              "SD-Card Pin 1, DAT3",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_CD1_PIO_ID",
                brief = "Card 1 Detect Port",
                description = "Port ID of the second card's detect line.\n"..
                              "Must use an external interrupt pin. If left "..
                              "empty, then card change detection is disabled.",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_CD1_PIO_BIT",
                brief = "Card 1 Detect Bit",
                description = "Port bit number of the second card's detect line.\n"..
                              "Must use an external interrupt pin. If left "..
                              "empty, then card change detection is disabled.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_WP1_PIO_ID",
                brief = "Card 1 Write Protect Port",
                description = "Port ID of the second card's write protect line.\n"..
                              "Currently ignored.",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC0_WP1_PIO_BIT",
                brief = "Card 1 Write Protect Bit",
                description = "Port bit number of the second card's write protect line.\n"..
                              "Currently ignored.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
        },
    },
    {
        name = "nutdev_sbi1mmc",
        brief = "MMC Software SPI 1 Driver",
        description = "Bit banging implementation of a low level MMC interface. "..
                      "Tested on AT91 only, but should work on most targets.",
        requires = { "HW_GPIO" },
        provides = { "DEV_MMCLL" },
        sources = { "sbi1mmc0.c", "sbi1mmc1.c" },
        options =
        {
            {
                macro = "MMC1_CLK_PIO_ID",
                brief = "Clock Port",
                description = "Port ID of the clock line.\n"..
                              "SD-Card Pin 5, CLK",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_CLK_PIO_BIT",
                brief = "Clock Bit",
                description = "Port bit number of the clock line.\n"..
                              "SD-Card Pin 5, CLK",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_MOSI_PIO_ID",
                brief = "MOSI Port",
                description = "Port ID of the MOSI line.\n"..
                              "SD-Card Pin 2, CMD",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_MOSI_PIO_BIT",
                brief = "MOSI Bit",
                description = "Port bit number of the MOSI line.\n"..
                              "SD-Card Pin 2, CMD",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_MISO_PIO_ID",
                brief = "MISO Port",
                description = "Port ID of the MISO line.\n"..
                              "SD-Card Pin 7, DAT0",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_MISO_PIO_BIT",
                brief = "MISO Bit",
                description = "Port bit number of the MISO line.\n"..
                              "SD-Card Pin 7, DAT0",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_CS0_PIO_ID",
                brief = "Card 0 Select Port",
                description = "Port ID of the first card select line.\n"..
                              "SD-Card Pin 1, DAT3",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_CS0_PIO_BIT",
                brief = "Card 0 Select Bit",
                description = "Port bit number of the first card select line.\n"..
                              "SD-Card Pin 1, DAT3",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_CD0_PIO_ID",
                brief = "Card 0 Detect Port",
                description = "Port ID of the first card's detect line.\n"..
                              "Must use an external interrupt pin. If left "..
                              "empty, then card change detection is disabled.",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_CD0_PIO_BIT",
                brief = "Card 0 Detect Bit",
                description = "Port bit number of the first card's detect line.\n"..
                              "Must use an external interrupt pin. If left "..
                              "empty, then card change detection is disabled.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_WP0_PIO_ID",
                brief = "Card 0 Write Protect Port",
                description = "Port ID of the first card's write protect line.\n"..
                              "Currently ignored.",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_WP0_PIO_BIT",
                brief = "Card 0 Write Protect Bit",
                description = "Port bit number of the first card's write protect line.\n"..
                              "Currently ignored.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_CS1_PIO_ID",
                brief = "Card 1 Select Port",
                description = "Port ID of the second card select line.\n"..
                              "SD-Card Pin 1, DAT3",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_CS1_PIO_BIT",
                brief = "Card 1 Select Bit",
                description = "Port bit number of the second card select line.\n"..
                              "SD-Card Pin 1, DAT3",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_CD1_PIO_ID",
                brief = "Card 1 Detect Port",
                description = "Port ID of the second card's detect line.\n"..
                              "Must use an external interrupt pin. If left "..
                              "empty, then card change detection is disabled.",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_CD1_PIO_BIT",
                brief = "Card 1 Detect Bit",
                description = "Port bit number of the second card's detect line.\n"..
                              "Must use an external interrupt pin. If left "..
                              "empty, then card change detection is disabled.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_WP1_PIO_ID",
                brief = "Card 1 Write Protect Port",
                description = "Port ID of the second card's write protect line.\n"..
                              "Currently ignored.",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
            {
                macro = "MMC1_WP1_PIO_BIT",
                brief = "Card 1 Write Protect Bit",
                description = "Port bit number of the second card's write protect line.\n"..
                              "Currently ignored.",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                flavor = "integer",
                file = function() return GetGpioHeaderPath() end
            },
        },
    },
    {
        name = "nutdev_sbimmc",
        brief = "Bit Banging Multimedia Card Access",
        description = "Old driver, use the one above!\n\n"..
                      "Bit banging implementation of a low level MMC interface. "..
                      "Tested on AT91 only.",
        requires = { "HW_GPIO", "HW_MCU_AT91R40008" },
        provides = { "DEV_MMCLL" },
        sources = { "sbimmc.c" },
        options =
        {
            {
                macro = "SPI0_CS_BIT",
                brief = "SPI0 Chip Select (AT91)",
                description = "Bit number of the chip select line.\n"..
                              "SD-Card Pin 1, DAT3",
                requires = { "HW_MCU_AT91" },
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "SPI0_CLK_BIT",
                brief = "SPI0 Clock (AT91)",
                description = "Bit number of the clock line.\n"..
                              "SD-Card Pin 5, CLK",
                requires = { "HW_MCU_AT91" },
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "SPI0_MOSI_BIT",
                brief = "SPI0 MOSI (AT91)",
                description = "Bit number of the MOSI line.\n"..
                              "SD-Card Pin 2, CMD",
                requires = { "HW_MCU_AT91" },
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "SPI0_MISO_BIT",
                brief = "SPI0 MISO (AT91)",
                description = "Bit number of the MISO line.\n"..
                              "SD-Card Pin 7, DAT0",
                requires = { "HW_MCU_AT91" },
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "MMC0_CD_BIT",
                brief = "MMC0 Card Detect (AT91)",
                description = "Bit number of the card detect line.\n"..
                              "Must use an external interrupt pin. If left "..
                              "empty, then card change detection is disabled.",
                requires = { "HW_MCU_AT91" },
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "MMC0_WP_BIT",
                brief = "MMC0 Write Protect (AT91)",
                description = "Bit number of the write protect line.\n"..
                              "Currently ignored.",
                requires = { "HW_MCU_AT91" },
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },

            {
                macro = "SPI0_PORT",
                brief = "SPI0 Port (AVR)",
                description = "Port register name, AVRPORTB by default.",
                requires = { "HW_MCU_AVR" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPI0_CS_BIT",
                brief = "SPI0 Chip Select (AVR)",
                description = "Bit number of the chip select line.",
                requires = { "HW_MCU_AVR" },
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPI0_CLK_BIT",
                brief = "SPI0 Clock (AVR)",
                description = "Bit number of the clock line.",
                requires = { "HW_MCU_AVR" },
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPI0_MOSI_BIT",
                brief = "SPI0 MOSI (AVR)",
                description = "Bit number of the MOSI line.",
                requires = { "HW_MCU_AVR" },
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPI0_MISO_BIT",
                brief = "SPI0 MISO (AVR)",
                description = "Bit number of the MISO line.",
                requires = { "HW_MCU_AVR" },
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
        },
    },

    --
    -- WAN Drivers.
    --
    {
        name = "nutdev_ppp",
        brief = "PPP Driver",
        requires = { "PROTO_HDLC", "NUT_TIMER", "PROTO_HDLC" },
        provides = { "NET_PHY" },
        sources = { "ppp.c" }
    },

    --
    -- Disabled components.
    --
    {
        name = "nutdev_spiflash_avr",
        brief = "SPI Flashing (AVR)",
        sources = { "spiflash.c" },
        requires = { "NOT_AVAILABLE" }
    },
}