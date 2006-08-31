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

-- ARM Architecture
--
-- $Log$
-- Revision 1.11  2006/08/31 19:04:08  haraldkipp
-- Added support for the AT91SAM9260 and Atmel's AT91SAM9260 Evaluation Kit.
--
-- Revision 1.10  2006/07/26 11:19:06  haraldkipp
-- Defining AT91_PLL_MAINCK will automatically determine SAM7X clock by
-- reading PLL settings.
-- Added MMC/SD-Card support for AT91SAM7X Evaluation Kit.
--
-- Revision 1.9  2006/07/05 08:02:17  haraldkipp
-- SAM7X interrupt and EMAC support added.
--
-- Revision 1.8  2006/06/28 17:22:34  haraldkipp
-- Make it compile for AT91SAM7X256.
--
-- Revision 1.7  2006/05/25 09:13:22  haraldkipp
-- Platform independent watchdog API added.
--
-- Revision 1.6  2006/04/07 12:24:12  haraldkipp
-- ARM driver for HD44780 LCD controller added.
--
-- Revision 1.5  2006/03/02 19:56:10  haraldkipp
-- First attempt to compile with ICCARM. All compile errors fixed, but not
-- a finished port yet. Many things are missing.
-- Added MCU specific hardware initialization routine. For the AT91 the
-- spurious interrupt handler has been added, which fixes SF 1440948.
--
-- Revision 1.4  2006/02/23 15:41:40  haraldkipp
-- Added support for AT91 watchdog timer.
--
-- Revision 1.3  2005/11/20 14:40:28  haraldkipp
-- Added interrupt driven UART driver for AT91.
--
-- Revision 1.2  2005/10/24 09:52:32  haraldkipp
-- New AT91 interrupt handler routines.
-- New DM9000E Ethernet driver for Ethernut 3 board.
--
-- Revision 1.1  2005/07/26 15:41:06  haraldkipp
-- All target dependent code is has been moved to a new library named
-- libnutarch. Each platform got its own script.
--
--

nutarch_arm =
{
    --
    -- Runtime Initialization
    --
    {
        name = "nutarch_arm_cstartup",
        brief = "ARM-GCC Startup",
        sources = { "arm/init/crt$(LDNAME).S" },
        targets = { "arm/init/crt$(LDNAME).o" },
        -- ICCARM: FIXME!
        requires = { "TOOL_CC_ARM", "TOOL_GCC" },
    },
    
    --
    -- Context Switching
    --
    {
        name = "nutarch_arm_context",
        brief = "Context Switching",
        provides = { "NUT_CONTEXT_SWITCH" },
        requires = { "HW_MCU_ARM", "TOOL_GCC" },
        sources = { "arm/os/context.c" },
    },
    
    --
    -- System Timer Hardware
    --
    {
        name = "nutarch_ostimer_at91",
        brief = "System Timer (AT91)",
        requires = { "HW_TIMER_AT91" },
        provides = { "NUT_OSTIMER_DEV" },
        sources = { "arm/dev/ostimer_at91.c" },
        options =
        {
            {
                macro = "AT91_PLL_MAINCK",
                brief = "AT91 Main Clock",
                description = "Frequency of the external crystal. If this option is "..
                              "enabled and NUT_CPU_FREQ is not enabled, then the "..
                              "specified value will be used together with "..
                              "the PLL register settings to determine the CPU master clock. "..
                              "Otherwise you must specify NUT_CPU_FREQ.",
                requires = { "HW_PLL_AT91" },
                flavor = "booldata",
                file = "include/cfg/clock.h"
            },
        },
    },
    {
        name = "nutarch_ostimer_gba",
        brief = "System Timer (GBA)",
        requires = { "HW_TIMER_GBA" },
        provides = { "NUT_OSTIMER_DEV" },
        sources = { "arm/dev/ostimer_gba.c" },
    },
    {
        name = "nutarch_wdt_at91",
        brief = "Watchdog Timer (AT91)",
        requires = { "HW_WDOG_AT91" },
        sources = { "arm/dev/wdt_at91.c" },
    },

    --
    -- Interrupt handling.
    --
    {
        name = "nutarch_arm_irqgba",
        brief = "Interrupt Handler (GBA)",
        requires = { "HW_MCU_GBA" },
        provides = { "DEV_IRQ_GBA" },
        sources = { "arm/dev/ihndlr_gba.c" },
    },
    {
        name = "nutarch_arm_irqat91x40",
        brief = "Interrupt Handler (AT91R40XX)",
        requires = { "HW_MCU_AT91R40008" },
        provides = { "DEV_IRQ_AT91" },
        sources =
        {
            "arm/dev/ih_at91fiq.c",
            "arm/dev/ih_at91irq0.c",
            "arm/dev/ih_at91irq1.c",
            "arm/dev/ih_at91irq2.c",
            "arm/dev/ih_at91pio.c",
            "arm/dev/ih_at91swirq.c",
            "arm/dev/ih_at91tc0.c",
            "arm/dev/ih_at91tc1.c",
            "arm/dev/ih_at91tc2.c",
            "arm/dev/ih_at91uart0.c",
            "arm/dev/ih_at91uart1.c",
            "arm/dev/ih_at91wdi.c"
        },
    },
    {
        name = "nutarch_arm_irqat91sam7x",
        brief = "Interrupt Handler (SAM7X)",
        requires = { "HW_MCU_AT91SAM7X" },
        provides = { "DEV_IRQ_AT91" },
        sources =
        {
            "arm/dev/ih_at91fiq.c",
            "arm/dev/ih_at91irq0.c",
            "arm/dev/ih_at91irq1.c",
            "arm/dev/ih_at91irq2.c",
            "arm/dev/ih_at91pio.c",
            "arm/dev/ih_at91swirq.c",
            "arm/dev/ih_at91tc0.c",
            "arm/dev/ih_at91tc1.c",
            "arm/dev/ih_at91tc2.c",
            "arm/dev/ih_at91uart0.c",
            "arm/dev/ih_at91uart1.c",
            "arm/dev/ih_at91emac.c",
            "arm/dev/ih_at91wdi.c"
        },
    },
    {
        name = "nutarch_arm_irqat91sam7x",
        brief = "Interrupt Handler (SAM926X)",
        requires = { "HW_MCU_AT91SAM9260" },
        provides = { "DEV_IRQ_AT91" },
        sources =
        {
            "arm/dev/ih_at91fiq.c",
            "arm/dev/ih_at91irq0.c",
            "arm/dev/ih_at91irq1.c",
            "arm/dev/ih_at91irq2.c",
            "arm/dev/ih_at91pio.c",
            "arm/dev/ih_at91swirq.c",
            "arm/dev/ih_at91tc0.c",
            "arm/dev/ih_at91tc1.c",
            "arm/dev/ih_at91tc2.c",
            "arm/dev/ih_at91uart0.c",
            "arm/dev/ih_at91uart1.c",
            "arm/dev/ih_at91emac.c"
        },
    },

    --
    -- Device Drivers
    --
    {
        name = "nutarch_arm_debug",
        brief = "UART Debug Output (AT91)",
        requires = { "HW_UART_AT91" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "arm/dev/debug_at91.c" }
    },
    {
        name = "nutarch_arm_usart0",
        brief = "USART0 Driver",
        description = "Hardware specific USART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = { "HW_UART_AT91", "DEV_IRQ_AT91", "NUT_EVENT", "CRT_HEAPMEM" },
        provides = { "DEV_UART_SPECIFIC" },
        sources = { "arm/dev/usart0at91.c" },
    },
    {
        name = "nutarch_arm_usart1",
        brief = "USART1 Driver",
        description = "Hardware specific USART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = { "HW_UART_AT91", "DEV_IRQ_AT91", "NUT_EVENT", "CRT_HEAPMEM" },
        provides = { "DEV_UART_SPECIFIC" },
        sources = { "arm/dev/usart1at91.c" },
    },
    {
        name = "nutarch_at91_hd44780",
        brief = "HD44780 LCD Driver (AT91)",
        description = "Only 4 bit interfaces are currently supported.\n"..
                      "Tested on the AT91R40008 with 2x16 character LCD.",
        requires = { "HW_MCU_AT91R40008" },
        provides = { "DEV_FILE", "DEV_WRITE" },
        sources = { "arm/dev/hd44780_at91.c" },
        options =
        {
            {
                macro = "LCD_ROWS",
                brief = "Rows",
                description = "The number of available display rows, either 1, 2 or 4.",
                default = "2",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_COLS",
                brief = "Columns",
                description = "The number of available display colums, either 8, 16, 20 or 40.",
                default = "16",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_SHORT_DELAY",
                brief = "Short Delay",
                description = "The number of dummy loops executed after LCD enable goes up.",
                default = "10",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_LONG_DELAY",
                brief = "Long Delay",
                description = "The number of loops executed after sending a command to the LCD "..
                              "controller. If a R/W line is speicifed, then the driver will queries "..
                              "the LCD status and terminates the loop as soon as the LCD busy "..
                              "flag has been cleared.",
                default = "1000",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_DATA_LSB",
                brief = "Least Significant Data Bit",
                description = "Bit number of the least significant data bit. The remaining "..
                              "data bits must be connected to the following port bits.",
                default = "0",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_ENABLE_BIT",
                brief = "Enable Bit",
                description = "Port bit of the LCD enable line. "..
                              "This line must be exclusively reserved.",
                default = "4",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_REGSEL_BIT",
                brief = "Register Select Bit",
                description = "Port bit of the LCD register select line. "..
                              "May be shared.",
                default = "7",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_RW_BIT",
                brief = "Read/Write Bit",
                description = "Optional port bit of the LCD register select line. "..
                              "If not specified, the driver will use the maximum delay. "..
                              "May be shared.",
                type = "enumerated",
                flavor = "booldata",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
        },
    },
    {
        name = "nutarch_gba_debug",
        brief = "LCD Debug Output (GBA)",
        requires = { "HW_LCD_GBA" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "arm/dev/debug_gba.c" }
    },   
    {
        name = "nutarch_arm_ax88796",
        brief = "AX88796 Driver (AT91)",
        description = "LAN driver for Asix 88796. AT91 only.",
        requires = { "HW_MCU_AT91R40008", "NUT_EVENT", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "arm/dev/ax88796.c" },
    },     
    {
        name = "nutarch_arm_dm9000e",
        brief = "DM9000E Driver (AT91)",
        description = "LAN driver for Davicom DM9000E. AT91 only.",
        requires = { "HW_MCU_AT91R40008", "NUT_EVENT", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "arm/dev/dm9000e.c" },
    },     
    {
        name = "nutarch_arm_at91sam7x_emac",
        brief = "AT91SAM7X EMAC Driver",
        description = "LAN driver for AT91SAM7X.",
        requires = { "HW_EMAC_AT91", "NUT_EVENT", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "arm/dev/at91_emac.c" },
    },     
    {
        name = "nutarch_arm_spimmc_at91",
        brief = "AT91 SPI MMC Access",
        description = "Low level MMC interface for AT91SAM7X.",
        requires = { "HW_SPI_AT91" },
        provides = { "DEV_MMCLL" },
        sources = { "arm/dev/spimmc_at91.c" },
    },     
    
    --
    -- Special Functions
    --
    {
        name = "nutarch__arm_init",
        brief = "AT91 Initialization",
        description = "Contains spurious interrupt handler.",
        requires = { "HW_MCU_AT91" },
        sources = { "arm/dev/at91init.c" },
    },
    {
        name = "nutarch__arm_at91efc",
        brief = "AT91 Embedded Flash",
        description = "Routines for reading and writing embedded flash memory.",
        requires = { "HW_MCU_AT91SAM7X" },
        sources = { "arm/dev/at91_efc.c" },
        options =
        {
            {
                macro = "NUT_CONFIG_AT91EFC",
                brief = "System Configuration",
                description = "If enabled, Nut/OS and Nut/Net configurations will "..
                              "be stored in on-chip flash memory.",
                provides = { "HW_FLASH_PARAM_SECTOR" },
                flavor = "boolean",
                file = "include/cfg/eeprom.h"
            },
        },
    },
}

