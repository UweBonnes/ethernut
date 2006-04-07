--
-- Copyright (C) 2004-2006 by egnite Software GmbH. All rights reserved.
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
        description = "General read/write access to non volatile memory.",
        provides = { "DEV_NVMEM"},
        sources = { "nvmem.c" },
    },

    --
    -- Simple Interface Drivers.
    --
    {
        name = "nutdev_twbbif",
        brief = "Bit Banging Two Wire",
        description = "Tested on AT91 only.",
        requires = { "HW_MCU_AT91" },
        provides = { "DEV_TWI" },
        sources = { "twbbif.c" }
    },

    --
    -- Block Device Drivers.
    --
    {
        name = "nutdev_mmcard",
        brief = "Basic MMC Driver",
        description = "Basic Multimedia card driver. To run this driver, a few low "..
                      "level routines are required for direct hardware access. "..
                      "Tested on AT91 only.",
        requires = { "DEV_MMCLL" },
        provides = { "DEV_BLOCK" },
        sources = { "mmcard.c" }
    },

    --
    -- Special Chip Drivers.
    --
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
            {
                macro = "FLASH_CONF_SECTOR",
                brief = "Configuration Sector Address",
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
                provides = { "HW_FLASH_PARAM_SECTOR" },
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "NUT_CONFIG_AT49BV",
                brief = "System Configuration",
                description = "If enabled, Nut/OS and Nut/Net configurations will "..
                              "be stored in this chip.",
                provides = { "HW_FLASH_PARAM_SECTOR" },
                provides = { "HW_NVMEM" },
                flavor = "boolean",
                file = "include/cfg/eeprom.h"
            },
        },
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
                macro = "NUT_CONFIG_X12RTC",
                brief = "System Configuration",
                description = "If enabled, Nut/OS and Nut/Net configurations will "..
                              "be stored in this chip.",
                provides = { "HW_NVMEM" },
                flavor = "boolean",
                file = "include/cfg/eeprom.h"
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
                              "1 for for the CLKB Pin\n"..
                              "2 for for the CLKC Pin (Ethernut 3 default)\n"..
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
                              "0 for for the CLKA Pin (Ethernut 3 default)\n"..
                              "1 for for the CLKB Pin\n"..
                              "2 for for the CLKC Pin\n"..
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
                              "0 for for the CLKA Pin\n"..
                              "1 for for the CLKB Pin (Ethernut 3 default)\n"..
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
                              "3 for for the CLKD Pin (Ethernut 3 default)\n"..
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
        name = "nutdev_sbimmc",
        brief = "Bit Banging Multimedia Card Access",
        description = "Bit banging implementation of a low level MMC interface. "..
                      "Tested on AT91 only.",
        requires = { "HW_GPIO" },                                    
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
