--
-- Copyright (C) 2004 by egnite Software GmbH. All rights reserved.
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

-- Tools
--
-- $Log$
-- Revision 1.1  2004/08/18 14:02:00  haraldkipp
-- First check-in
--
--
--

nuttools =
{
    options = 
    {
        {
            brief = "GCC for ARM",
            description = "GNU Compiler Collection for ARM including libc."..
                          "Make sure you have deselected all other compilers.",
            provides = { "TOOL_CC_ARM", "TOOL_GCC" },
            macro = "ARM_GCC",
            flavor = "boolean",
            file = "cfg/arch.h"
        },
        {
            brief = "GCC for ARM (no libc)",
            description = "GNU Compiler Collection for ARM excluding libc."..
                          "Nut/OS provides all required C standard functions."..
                          "Make sure you have deselected all other compilers.",
            provides = { "TOOL_CC_ARM", "TOOL_GCC", "TOOL_NOLIBC" },
            macro = "ARM_GCC",
            flavor = "boolean",
            file = "cfg/arch.h"
        },
        {
            brief = "GCC for AVR",
            description = "Make sure you have deselected all other compilers.",
            provides = { "TOOL_CC_AVR", "TOOL_GCC" },
            macro = "AVR_GCC",
            flavor = "boolean",
            file = "cfg/arch.h",
            makedefs = { "MCU_ATMEGA128=atmega128", "MCU_ATMEGA103=atmega103" }
        },
        {
            brief = "GCC for Linux",
            description = "Make sure you have deselected all other compilers.",
            provides = { "TOOL_CC_LINUX", "TOOL_GCC" },
            macro = "LINUX_GCC",
            flavor = "boolean",
            file = "cfg/arch.h"
        },
        {
            brief = "ImageCraft for AVR",
            description = "Make sure you have deselected all othe compilers.",
            provides = { "TOOL_CC_AVR", "TOOL_ICC" },
            macro = "ICCAVR",
            flavor = "boolean",
            file = "cfg/arch.h",
            makedefs = { "MCU_ATMEGA128=enhanced", "MCU_ATMEGA103=atmega" }
        }
    },
    {
        name = "nuttools_gccopt",
        brief = "GCC Settings",
        requires = { "TOOL_GCC" },
        options = 
        {
            {
                brief = "Include debug info",
                flavor = "booldata",
                macro = "GCC_DEBUG"
            },
        }
    },
    {
        name = "nuttools_iccopt",
        brief = "ImageCraft AVR Settings",
        requires = { "TOOL_ICC" }
    }

    --
    -- Intentionally no programmer or urom creator specified. 
    -- This will be part of the application wizard.
    --
}
