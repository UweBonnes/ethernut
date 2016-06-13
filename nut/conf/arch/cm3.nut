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

-- ARM CortexM3 Architecture
--
-- $Log$
-- Revision 1.0  2010/09/10 13:24:58  ulrichprinz
-- STM32F Initial Port
--

nutarch_cm3 =
{
    --
    -- MCU Family CortexM3
    --
    {
        name = "nutarch_cm3_family",
        brief = "MCU Family",
        options =
        {
            {
                macro = "MCU_CM3",
                brief = "Cortex-M Family",
                default = 1,
                requires = { "HW_MCU_CM3" },
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_CM7",
                brief = "Cortex-M7 Core",
                default = 1,
                requires = { "HW_MCU_CM7" },
                file = "include/cfg/arch.h"
            }
        }
    },

    --
    -- Cortex-M Core Functions
    --
    {
        name = "nutarch_cm3_init",
        brief = "Cortex-M initialization.",
        description = "Selection of :\n"..
                      "- Boot function\n"..
                      "- Stack sizes.",
        requires = { "HW_MCU_CM3" },
        provides = {
           "DEV_IRQ_CM3",
           "NUT_OSTIMER_DEV",
           "NUT_CONTEXT_SWITCH",
           "DEV_IRQ_CM3",
        },
        sources = {
            "cm3/cmsis/cortex_init.c",
            "cm3/cmsis/ostimer_cortex.c",
            "cm3/os/context.c",
            "cm3/os/cortex_irqctl.c",
            "cm3/cmsis/cortex_reset.c",
        },
        options =
        {
            {
                macro = "NUT_BOOT_FUNCTION",
                brief = "Boot Function",
                description = "This function is given for the reset entry vecor.\n"..
                              "Default is NutInit\n"..
                              "The function must be declarated as int function(void).",
                flavor = "booldata",
                file = "include/cfg/arch.h"
            },
            {
                macro = "MSP_STACK_SIZE",
                brief = "Main Stack Size",
                description = "Number of bytes reserved for interrupt stack\n"..
                              "Default is 128 words.\n\n"..
                              "This stack is used in flat mode, i.e. the thread mode the CPU uses with Nut/OS.\n"..
                              "The stack is needed for any operation and function call while Nut/OS startup.\n",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "PSP_STACK_SIZE",
                brief = "Process Stack Size",
                description = "Number of bytes reserved for process stack\n"..
                              "Default is 32 words.\n\n"..
                              "The process stack is actually not used in Nut/OS but may be used for faster task switching "..
                              "in a later release. So beware of modifications.",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            }
        }
    },


     --
    -- CortexM4 FPU
    --
    {
        name = "nutarch_cm4_fpu",
        brief = "FPU support (CortexM4)",
        options =
        {
            {
                macro = "MCU_USE_CORTEX_FPU",
                brief = "Enable single precission FPU support",
                requires = {"HW_MCU_FPU"},
                flavor = "boolean",
                file = "include/cfg/arch.h",
                makedefs = { "FPUFLAGS=-mfloat-abi=hard -mfpu=fpv4-sp-d16" }
            },
            {
                macro = "MCU_USE_CORTEX_DFPU",
                brief = "Enable double precession FPU support",
                requires = {"HW_MCU_DFPU"},
                flavor = "boolean",
                file = "include/cfg/arch.h",
                makedefs = { "FPUFLAGS=-mfloat-abi=hard -mfpu=fpv5-d16" }
            },
        }
    },
    --
    -- Cortex-M7 I/D Cache
    --
    {
        name = "nutarch_cm7_cache",
        brief = "Cache Support (Cortex M7)",
        description = "Cortex M7 I/D cache setting";
        requires = { "HW_MCU_CM7" },
        options =
        {
            {
                macro = "CORTEX_USE_ICACHE",
                brief = "Enable ICACHE",
                description = "Enable Instruction cache for AXIM Bus access.\n\n"..
                         "For Instruction access above 0x00300000.\n"..
                         "Default is enabled\n",
                type = "enumerated",
                choices =  {"ENABLE", "DISABLE"},
                default = "ENABLE",
                file = "include/cfg/arch.h",
            },
            {
                macro = "CORTEX_USE_DCACHE",
                brief = "Enable DCACHE",
                description = "Enable Data cache for AXIM Bus access.\n\n"..
                         "For Data access outside DTCM range.\n"..
                         "Default is enabled\n",
                type = "enumerated",
                choices =  {"ENABLE", "DISABLE"},
                default = "ENABLE",
                file = "include/cfg/arch.h",
            },
        }
    },

    --
    -- Board Initialization
    --
    {
        name = "nutarch_cm3_bs",
        brief = "Board Support",
        sources =
            function()
                return { "cm3/board/"..string.lower(c_macro_edit("PLATFORM"))..".c" };
            end,
        requires = { "HW_BOARD_SUPPORT" },
    },

    --
    -- Cortex Based Cpu Directory
    --
    {
        name = "nutarch_cm3_stm32_family",
        brief = "STM32 Family",
        requires = { "HW_MCU_STM32" },
        description = "ST Microelectronics STM32 Series",
        script = "arch/cm3/stm32fam.nut",
        sources = {
            "cm3/dev/stm/system_stm32.c",
        },
        options =
        {
            {
                macro = "MCU_STM32",
                brief = "STM32 Family",
                descrition ="STM32 Family.",
                default = 1,
                requires = { "HW_MCU_STM32" },
                file = "include/cfg/arch.h"
            },
        }
    },
    {
        name = "nutarch_cm3_sam3",
        brief = "SAM3",
        requires = { "HW_MCU_SAM3" },
        description = "ATMEL AT91SAM3 Series",
        script = "arch/cm3/sam3u.nut"
    },
    {
        name = "nutarch_cm3_lm3",
        brief = "LM3",
        requires = { "HW_MCU_LM3" },
        description = "Luminary Micro LM3 Series",
        script = "arch/cm3/lm3.nut"
    },
    {
        name = "nutarch_cm3_lpc17xx_family",
        brief = "LPC17xx",
        requires = { "HW_MCU_LPC17xx" },
        description = "NXP LPC17xx Series",
        script = "arch/cm3/lpc17xxfam.nut"
    }
}

