--
-- Copyright (C) 2004-2005 by egnite Software GmbH. All rights reserved.
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
-- Revision 1.15  2006/01/23 17:28:30  haraldkipp
-- HW_NVMEM is now provided by specific modules.
--
-- Revision 1.14  2006/01/05 16:45:04  haraldkipp
-- ATmega2561 CPU added, but not yet supported.
--
-- Revision 1.13  2005/10/07 22:09:23  hwmaier
-- Changed HWDEF= to HWDEF+= so more defs can be added to this macros from other LUA scripts.
--
-- Revision 1.12  2005/10/04 05:47:26  hwmaier
-- Added CAN driver for AT90CAN128
--
-- Revision 1.11  2005/07/26 15:41:05  haraldkipp
-- All target dependent code is has been moved to a new library named
-- libnutarch. Each platform got its own script.
--
-- Revision 1.10  2005/02/19 22:48:12  hwmaier
-- no message
--
-- Revision 1.9  2005/01/22 19:20:21  haraldkipp
-- Fixes Ethernut 1.3G memory bug
--
-- Revision 1.8  2004/10/03 18:37:39  haraldkipp
-- GBA support
--
-- Revision 1.7  2004/09/22 08:19:42  haraldkipp
-- No ATmega103 with LAN91C111
--
-- Revision 1.6  2004/09/19 15:17:37  haraldkipp
-- ICCAVR initialization added to build
--
-- Revision 1.5  2004/09/08 10:18:54  haraldkipp
-- C startup on ARM platforms only
--
-- Revision 1.4  2004/09/07 19:07:45  haraldkipp
-- MCU type in NutConf.mk and cstartup added.
--
-- Revision 1.3  2004/08/18 16:05:26  haraldkipp
-- Use consistent directory structure
--
-- Revision 1.2  2004/08/18 13:46:08  haraldkipp
-- Fine with avr-gcc
--
-- Revision 1.1  2004/08/03 15:09:30  haraldkipp
-- Another change of everything
--
-- Revision 1.1  2004/06/07 16:32:45  haraldkipp
-- First release
--
--

nutarch =
{
    {
        name = "nutarch_mcu",
        brief = "Target CPU",
        description = "Select one only.",
        options =
        {
            {
                macro = "MCU_ATMEGA103",
                brief = "Atmel ATmega 103",
                description = "8-bit RISC microcontroller with 128K bytes flash, 4K bytes RAM, "..
                              "4K bytes EEPROM, 64K bytes data memory space, UART, 3 timers, "..
                              "8-channel ADC and SPI.",
                requires = { "TOOL_CC_AVR" },
                provides = {
                    "HW_TARGET",
                    "HW_MCU_AVR",
                    "HW_TIMER_AVR",
                    "HW_UART_AVR"
                },
                flavor = "boolean",
                file = "include/cfg/arch.h",
                makedefs = { "MCU=$(MCU_ATMEGA103)", "HWDEF+=-D__HARVARD_ARCH__" }
            },
            {
                macro = "MCU_ATMEGA128",
                brief = "Atmel ATmega 128",
                description = "8-bit RISC microcontroller with 128K bytes flash, 4K bytes RAM, "..
                              "4K bytes EEPROM, 64K bytes data memory space, 2 USARTs, 4 timers, "..
                              "8-channel ADC, SPI and TWI.",
                requires = { "TOOL_CC_AVR" },
                provides = {
                    "HW_TARGET",
                    "HW_MCU_AVR",
                    "HW_MCU_AVR_ENHANCED",
                    "HW_MCU_ATMEGA128",
                    "HW_TIMER_AVR",
                    "HW_UART_AVR"
                },
                flavor = "boolean",
                file = "include/cfg/arch.h",
                makedefs = { "MCU=$(MCU_ATMEGA128)", "HWDEF+=-D__HARVARD_ARCH__" }
            },
            {
                macro = "MCU_AT90CAN128",
                brief = "Atmel AT90CAN128",
                description = "8-bit RISC microcontroller with 128K bytes flash, 4K bytes RAM, "..
                              "4K bytes EEPROM, 64K bytes data memory space, 2 USARTs, 4 timers, "..
                              "8-channel ADC, SPI, TWI and CAN controller.\n"..
                              "\n"..
                              "Please configure appropriate stack settings in category "..
                              "Memory Management. "..
                              "Due to a bug for silicon revisions C of the AT90CAN128 MCU, "..
                              "the code stack must reside in internal RAM. Refer to "..
                              "AT90CAN128 Datasheet Rev. 4250F–CAN–04/05 - Errata Rev C ",
                flavor = "boolean",
                file = "include/cfg/arch.h",
                requires = { "TOOL_CC_AVR" },
                provides = {
                    "HW_TARGET",
                    "HW_MCU_AVR",
                    "HW_MCU_AVR_ENHANCED",
                    "HW_TIMER_AVR",
                    "HW_UART_AVR",
                    "HW_CAN_AVR"
                },
                makedefs = { "MCU=at90can128", "HWDEF+=-D__HARVARD_ARCH__" },
                options =
                {
                }
            },
            {
                macro = "MCU_ATMEGA2561",
                brief = "Atmel ATmega 2561",
                description = "8-bit RISC microcontroller with 256K bytes flash, 8K bytes RAM, "..
                              "4K bytes EEPROM, 64K bytes data memory space, 2 USARTs, 6 timers, "..
                              "8-channel ADC, SPI and TWI.",
                requires = { "TOOL_CC_AVR" },
                provides = {
                    "HW_TARGET",
                    "HW_MCU_AVR",
                    "HW_MCU_AVR_ENHANCED",
                    "HW_MCU_ATMEGA2561",
                    "HW_TIMER_AVR",
                    "HW_UART_AVR"
                },
                flavor = "boolean",
                file = "include/cfg/arch.h",
                makedefs = { "MCU=$(MCU_ATMEGA128)", "HWDEF+=-D__HARVARD_ARCH__" }
            },
            {
                macro = "MCU_AT91R40008",
                brief = "Atmel AT91R40008",
                description = "ARM7TDMI 16/32-bit RISC microcontroller with 256K bytes RAM, "..
                              "64M bytes address space, 2 USARTs and 3 timers. ",
                flavor = "boolean",
                file = "include/cfg/arch.h",
                requires = { "TOOL_CC_ARM" },
                provides = {
                    "HW_TARGET",
                    "HW_MCU_ARM",
                    "HW_MCU_AT91",
                    "HW_TIMER_AT91",
                    "HW_UART_AT91"
                },
                makedefs = { "MCU=arm7tdmi" }
            },
            {
                macro = "MCU_GBA",
                brief = "Nintendo GBA",
                description = "ARM7TDMI 16/32-bit RISC microcontroller",
                flavor = "boolean",
                file = "include/cfg/arch.h",
                requires = { "TOOL_CC_ARM" },
                provides = {
                    "HW_TARGET",
                    "HW_MCU_ARM",
                    "HW_MCU_GBA",
                    "HW_TIMER_GBA",
                    "HW_LCD_GBA"
                },
                makedefs = { "MCU=arm7tdmi" }
            },
            {
                macro = "MCU_LINUX_EMU",
                brief = "Linux Emulator",
                flavor = "boolean",
                file = "include/cfg/arch.h",
                requires = { "TOOL_CC_LINUX" },
                provides = { "HW_TARGET", "HW_NVMEM", "HW_EMU_LINUX" }
            },
            {
                macro = "MCU_H8_3068",
                brief = "Renesas H8/3068",
                flavor = "boolean",
                file = "include/cfg/arch.h",
                requires = { "TOOL_CC_H8300" },
                provides = {
                    "HW_TARGET",
                    "HW_MCU_H8300",
                    "H83068_TIMER",
                    "H83068_UART"
                }
            },
            {
                macro = "MCU_S3C4510B",
                brief = "Samsung S3C4510B",
                description = "ARM7TDMI 16/32-bit RISC microcontroller with Ethernet MAC,"..
                              "HDLC protocol, 64M bytes address space, I2C, 2 UARTs and "..
                              "2 timers.",
                flavor = "boolean",
                file = "include/cfg/arch.h",
                requires = { "TOOL_CC_ARM" },
                provides = {
                    "HW_TARGET",
                    "HW_MCU_ARM",
                    "HW_MCU_S3C45",
                    "HW_TIMER_S3C45",
                    "HW_UART_S3C45"
                }
            }
        }
    },

    --
    -- Architecture Dependent Implementations
    --
    {
        name = "nutarch_arm",
        brief = "ARM",
        requires = { "HW_MCU_ARM" },
        script = "arch/arm.nut"
    },
    {
        name = "nutarch_avr",
        brief = "AVR",
        requires = { "HW_MCU_AVR" },
        script = "arch/avr.nut"
    },
    {
        name = "nutarch_h8300h",
        brief = "H8/300H",
        requires = { "HW_MCU_H8300" },
        script = "arch/h8300h.nut"
    },
    {
        name = "nutarch_m68k",
        brief = "M68K",
        requires = { "HW_MCU_M68K" },
        script = "arch/m68k.nut"
    },
    {
        name = "nutarch_unix",
        brief = "Linux Emulator",
        requires = { "HW_EMU_LINUX" },
        script = "arch/unix.nut"
    },


}
