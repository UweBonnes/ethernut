--
-- Copyright (C) 2005-2007 by egnite Software GmbH. All rights reserved.
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


-- The repository contains an enumerated list
-- of all top-level components.
--
-- $Log$
-- Revision 1.32  2008/08/11 11:51:20  thiagocorrea
-- Preliminary Atmega2560 compile options, but not yet supported.
-- It builds, but doesn't seam to run properly at this time.
--
-- Revision 1.31  2008/08/06 12:51:07  haraldkipp
-- Added support for Ethernut 5 (AT91SAM9XE reference design).
--
-- Revision 1.30  2008/07/24 17:05:12  haraldkipp
-- Demonstrates some Configurator capabilities.
--
-- Revision 1.29  2008/07/14 13:04:28  haraldkipp
-- New Ethernut 3 link option with code in RAM and constant data in ROM.
--
-- Revision 1.28  2008/07/09 11:14:46  haraldkipp
-- Changelog corrected, again.
--
-- Revision 1.27  2008/04/01 10:12:08  haraldkipp
-- Added missing at91sam7s entries.
--
-- Revision 1.26  2008/02/15 17:05:52  haraldkipp
-- AT91SAM7SE512 support added. Global MCU list simplifies exclusivity
-- attribute. HW_EBI_AT91 added for MCUs with external bus interface.
--
-- Revision 1.25  2008/01/31 09:11:19  haraldkipp
-- Added ability to upload AT91 flash image via HTTP. Many thanks to
-- Matthias Wilde.
-- Added AVR ports H up to L.
--
-- Revision 1.24  2007/09/11 13:39:45  haraldkipp
-- Configurable startup file for ICCAVR.
--
-- Revision 1.23  2007/09/06 19:54:31  olereinhardt
-- Added ld script selection for eb40a_redboot_ram (EB40A Eval board with
-- redboot bootloader running, changed load address)
--
-- Revision 1.22  2007/04/12 09:09:27  haraldkipp
-- Allow to select empty entry for AT91 ports.
--
-- Revision 1.21  2007/02/15 16:19:44  haraldkipp
-- AT91 PIO IDs added.
--
-- Revision 1.20  2006/10/08 16:40:17  haraldkipp
-- Many thanks to Thiago Correa for adding LCD port configuration.
--
-- Revision 1.19  2006/10/05 17:14:03  haraldkipp
-- Added script, which extracts version information from C source file.
-- This demonstrates the new scripting capabilities of the Configurator.
--
-- Revision 1.18  2006/09/05 12:31:34  haraldkipp
-- Added missing linker script for SAM9260 applications running in external
-- RAM.
--
-- Revision 1.17  2006/08/01 07:34:16  haraldkipp
-- New linker script and new startup file support applications running in
-- flash memory.
--
-- Revision 1.16  2006/02/23 15:40:09  haraldkipp
-- Added choice for 32 bit ports.
--
-- Revision 1.15  2006/01/23 17:31:36  haraldkipp
-- Enhanced description of dev library.
--
-- Revision 1.14  2006/01/22 17:35:22  haraldkipp
-- Baudrate calculation for Ethernut 3 failed if MCU Clock was not specified.
--
-- Revision 1.13  2006/01/05 16:44:58  haraldkipp
-- PLL clock choices added.
--
-- Revision 1.12  2005/10/24 10:01:19  haraldkipp
-- New linker scripts added for AT91 apps running in RAM and ROM.
--
-- Revision 1.11  2005/10/07 22:10:29  hwmaier
-- Added support for PLATFORM macro
--
-- Revision 1.10  2005/07/26 16:14:21  haraldkipp
-- Better title for architecture.
--
-- Revision 1.9  2005/04/05 18:04:17  haraldkipp
-- Support for ARM7 Wolf Board added.
--
-- Revision 1.8  2005/01/22 19:18:37  haraldkipp
-- Changed AVR port configuration names from PORTx to AVRPORTx.
-- Added C++ support contributed by Oliver Schulz (MPI).
--
-- Revision 1.7  2004/11/24 14:47:24  haraldkipp
-- Empty items allow to disable choices
--
-- Revision 1.6  2004/10/03 18:37:39  haraldkipp
-- GBA support
--
-- Revision 1.5  2004/09/07 19:12:57  haraldkipp
-- Linker script support added
--
-- Revision 1.4  2004/09/01 14:09:49  haraldkipp
-- AVR interrupt selection simplified
--
-- Revision 1.3  2004/08/18 13:46:08  haraldkipp
-- Fine with avr-gcc
--
-- Revision 1.2  2004/08/03 15:09:30  haraldkipp
-- Another change of everything
--
-- Revision 1.1  2004/06/07 16:38:43  haraldkipp
-- First release
--
--

mcu_names = { "MCU_ATMEGA103", "MCU_ATMEGA128", "MCU_AT90CAN128", "MCU_ATMEGA2560", "MCU_ATMEGA2561", "MCU_AT91SAM9260", "MCU_AT91SAM9XE512",
              "MCU_AT91SAM7X256", "MCU_AT91SAM7S256", "MCU_AT91SAM7SE512", "MCU_AT91R40008", "MCU_GBA", 
              "MCU_LINUX_EMU", "MCU_S3C4510B" }

avr_bit_choice = { " ", "0", "1", "2", "3", "4", "5", "6", "7" }
avr_port_choice = { " ", "AVRPORTA", "AVRPORTB", "AVRPORTC", "AVRPORTD", "AVRPORTE", "AVRPORTF", "AVRPORTG", "AVRPORTH", "AVRPORTI", "AVRPORTJ", "AVRPORTK", "AVRPORTL" }
avr_irq_choice = { " ", "INT0", "INT1", "INT2", "INT3", "INT4", "INT5", "INT6", "INT7" }

iccavr_startup_choice = { "crtenutram", "crtenut", "crtnutram", "crtnut", "crtnutm256" }

mcu_32bit_choice = { " ", 
                     "0", "1", "2", "3", "4", "5", "6", "7",
                     "8", "9", "10", "11", "12", "13", "14", "15",
                     "16", "17", "18", "19", "20", "21", "22", "23",
                     "24", "25", "26", "27", "28", "29", "30", "31"
                   }

at91_pio_id_choice = { " ", "PIO_ID", "PIOA_ID", "PIOB_ID", "PIOC_ID" }


arm_ld_choice = { 
                    " ", 
                    "at91_boot", 
                    "at91_bootcrom", 
                    "at91_ram", 
                    "at91_rom", 
                    "at91_httprom", 
                    "at91sam7s256_rom",
                    "at91sam7se512_rom",
                    "at91sam7se512_ram",
                    "at91sam7x256_rom", 
                    "at91sam9260_ram", 
                    "at91sam9xe512_ram", 
                    "s3c4510b-ram", 
                    "eb40a_ram",
                    "eb40a_redboot_ram", 
                    "gbaxport2", 
                    "wolf_ram" 
                }

pll_clk_choice = { " ", "0", "1", "2", "3", "4" }

hd44780_databits_choice = { " ", "0xFF", "0xF0", "0x0F" }

repository =
{
    {
        name = "nutinfo",
-- First version of a dynamic item:
-- The string contains a Lua script, which is compiled and executed by the Configurator.
-- This version is deprecated.
--        brief = "--\n".. -- Strings starting with this sequence are executed.
--                "return 'Nut/OS ' .. GetNutOsVersion()\n", -- This is the executed script.

-- Second version of a dynamic item:
-- A function result is combined with a static string. Note, that the function
-- is executed when this script file is loaded and must have been defined 
-- previously.
--        brief = "Nut/OS " .. GetNutOsVersion(),

-- Third version of a dynamic item:
-- The value is specified as a function returning a string. In this case any function
-- used in our function body may be defined later.
        brief = function() return "Nut/OS " .. GetNutOsVersion(); end,


        description = "The version info was read from os/version.c in the current source "..
                      "tree by a Lua script defined in the configuration file.\n\n"..
                      "Also added here to demonstrate the capabilities of Lua as a "..
                      "configuration language.\n\n"..
                      "See http://www.lua.org.\n\n",
        options =
        {
            {
                macro = "PLATFORM",
                brief = "Platform Macro",
                description = "String constant identifying the target hardware used.\n\n"..
                      "Examples are ETHERNUT1, ETHERNUT2 etc. "..
                      "This string constant is passed as a macro definition to "..
                      "the Makefiles and allows conditional compilation "..
                      "depending on the hardware used.",
                flavor = "booldata",
                file = "include/cfg/arch.h",
                makedefs = { "PLATFORM", "HWDEF+=-D$(PLATFORM)" }
            }
        }
    },
    {
        name = "nuttools",
        brief = "Tools",
        description = "Tool selection.",
        script = "tools.nut"
    },
    {
        name = "nutarch",
        brief = "Architecture",
        description = "Target selection.",
        subdir = "arch",
        script = "arch/arch.nut"
    },
    {
        name = "nutos",
        brief = "RTOS Kernel",
        description = "Operating system core functions",
        requires = { "HW_TARGET" },
        subdir = "os",
        script = "os/os.nut"
    },
    {
        name = "nutdev",
        brief = "Device Drivers",
        description = "This library contains architecture independant...\n"..
                      "... hardware device drivers, typically supporting external chips.\n"..
                      "... device driver frameworks, which provide the hardware independant part of a driver\n"..
                      "... helper routines, which are of general use for device drivers.",
        requires = { "HW_TARGET" },
        subdir = "dev",
        script = "dev/dev.nut"
    },
    {
        name = "nutc",
        brief = "C runtime (tool specific)",
        description = "Hardware independent C runtime",
        requires = { "HW_TARGET" },
        subdir = "c",
        script = "c/c.nut"
    },
    {
        name = "nutcrt",
        brief = "C runtime (target specific)",
        description = "Hardware dependent C runtime",
        requires = { "HW_TARGET" },
        subdir = "crt",
        script = "crt/crt.nut"
    },
    {
        name = "nutnet",
        brief = "Network (general)",
        description = "Network functions",
        requires = { "HW_TARGET" },
        subdir = "net",
        script = "net/net.nut"
    },
    {
        name = "nutpro",
        brief = "Network (application layer)",
        description = "High level network protocols",
        requires = { "HW_TARGET" },
        subdir = "pro",
        script = "pro/pro.nut"
    },
    {
        name = "nutfs",
        brief = "File system",
        description = "File systems",
        requires = { "HW_TARGET" },
        subdir = "fs",
        script = "fs/fs.nut"
    },
    {
        name = "nutcpp",
        brief = "C++ runtime extensions",
        description = "C++ runtime extensions",
        requires = { "HW_TARGET", "TOOL_CXX" },
        subdir = "cpp",
        script = "cpp/cpp.nut"
    }
}

--
-- Read OS Version from C source file.
--
function GetNutOsVersion()
    local fp, buf, p1, p2, vers

    -- Retrieve the repository path by calling a C function provided
    -- by the Configurator and read the Nut/OS version file.
    fp = io.open(c_repo_path() .. "/../os/version.c", "r")
    if fp == nil then
        return nil
    end
    buf = fp:read(8192)
    fp:close()

    -- Extract the version string.
    p1, p2, vers = string.find(buf, "os_version_string.+\"(.+)\"")
    
    return vers or "unknown"
end

