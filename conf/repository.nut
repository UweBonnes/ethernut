--
-- Copyright (C) 2005 by egnite Software GmbH. All rights reserved.
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

avr_bit_choice = { " ", "0", "1", "2", "3", "4", "5", "6", "7" }
avr_port_choice = { " ", "AVRPORTA", "AVRPORTB", "AVRPORTC", "AVRPORTD", "AVRPORTE", "AVRPORTF", "AVRPORTG" }
avr_irq_choice = { " ", "INT0", "INT1", "INT2", "INT3", "INT4", "INT5", "INT6", "INT7" }

mcu_32bit_choice = { " ", 
                     "0", "1", "2", "3", "4", "5", "6", "7",
                     "8", "9", "10", "11", "12", "13", "14", "15",
                     "16", "17", "18", "19", "20", "21", "22", "23",
                     "24", "25", "26", "27", "28", "29", "30", "31"
                   }
                   
arm_ld_choice = { 
                    " ", 
                    "at91_boot", 
                    "at91_ram", 
                    "at91_rom", 
                    "at91sam7x256_rom", 
                    "at91sam9260_ram", 
                    "s3c4510b-ram", 
                    "eb40a_ram", 
                    "gbaxport2", 
                    "wolf_ram" 
                }

pll_clk_choice = { " ", "0", "1", "2", "3", "4" }

repository =
{
    {
        name = "nutinfo",
        brief = "--\n".. -- Strings starting with this sequence are executed.
                "return 'Nut/OS ' .. GetNutOsVersion()\n", -- This is the executed script.
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
