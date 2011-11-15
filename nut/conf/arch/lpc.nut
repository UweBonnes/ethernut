--
-- Copyright (C) 2011 by egnite GmbH
-- Copyright (C) 2004-2007 by egnite Software GmbH
--
-- All rights reserved.
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

-- LPC Architecture
--
-- $Id$
--

nutarch_lpc =
{
    --
    -- MCU Family
    --
    {
        name = "nutarch_lpc_family",
        brief = "LPC Architecture Macros",
        description = "These macros are used by the pre-processor to conditionally "..
                      "include or exclude CPU specific parts of the source code.",
        options =
        {
            {
                macro = "MCU_LPC",
                brief = "NXP LPC Family",
                type = "integer",
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_LPC1700",
                brief = "LPC1700 Series",
                description = "Cortex-M3 microcontrollers with advanced peripherals "..
                              "such as Ethernet, USB, CAN or LCD controller. They "..
                              "provide up to 512kB flash and 96kB RAM while operating "..
                              "at up to 120MHz.\n\n"..
                              "Support for LPC1758 is under development, only.",
                type = "integer",
                default = 1,
                requires = { "HW_MCU_LPC1700" },
                file = "include/cfg/arch.h"
            }
        }
    },

    --
    -- Runtime Initialization
    --
    {
        name = "nutarch_lpc_cstartup",
        brief = "ARM-GCC Startup",
        sources = { "arm/lpc/lpc1700/init/crt$(LDNAME).S" },
        targets = { "arm/lpc/lpc1700/init/crt$(LDNAME).o" },
        requires = { "TOOL_CC_ARM", "TOOL_GCC" },
    },

    --
    -- Board Initialization
    --
    {
        name = "nutarch_lpc_bs",
        brief = "Board Support",
        sources =
            function()
                return { "arm/lpc/lpc1700/board/"..string.lower(c_macro_edit("PLATFORM"))..".c" };
            end,
        requires = { "HW_BOARD_SUPPORT" },
    },

    --
    -- Context Switching
    --
    {
        name = "nutarch_lpc1700_context",
        brief = "Context Switching",
        provides = { "NUT_CONTEXT_SWITCH" },
        requires = { "HW_MCU_LPC1700", "TOOL_GCC" },
        sources = { "arm/lpc/lpc1700/os/context.c" },
    },

    --
    -- System Timer Hardware
    --
    {
        name = "nutarch_lpc1700_ostimer",
        brief = "System Timer (LPC1700)",
        requires = { "HW_TIMER_LPC1700" },
        provides = { "NUT_OSTIMER_DEV" },
        sources = { "arm/lpc/lpc1700/dev/ostimer_lpc1700.c" },
    },

    {
        name = "nutarch_irq_lpc1758",
        brief = "Interrupt Handler (LPC1758)",
        requires = { "HW_MCU_LPC1758" },
        provides = { "DEV_IRQ_LPC1700" },
        sources =
        {
            "arm/lpc/lpc1700/dev/ih_lpc1700ssp0.c",
            "arm/lpc/lpc1700/dev/ih_lpc1700tc2.c"
        },
    },

    --
    -- Device Drivers
    --
    {
        name = "nutarch_lpc1700_debug",
        brief = "UART Debug Output",
        requires = { "HW_UART_LPC" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "arm/lpc/lpc1700/dev/debug_lpc1700.c" }
    },
}
