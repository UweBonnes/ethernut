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

-- ARM Architecture
--
-- $Log$
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
        requires = { "TOOL_CC_ARM" },
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
    },
    {
        name = "nutarch_ostimer_gba",
        brief = "System Timer (GBA)",
        requires = { "HW_TIMER_GBA" },
        provides = { "NUT_OSTIMER_DEV" },
        sources = { "arm/dev/ostimer_gba.c" },
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
        name = "nutarch_arm_irqat91",
        brief = "Interrupt Handler (AT91)",
        requires = { "HW_MCU_AT91" },
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
        name = "nutarch_arm_debug",
        brief = "LCD Debug Output (GBA)",
        requires = { "HW_LCD_GBA" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "arm/dev/debug_gba.c" }
    },   
    {
        name = "nutarch_arm_ax88796",
        brief = "AX88796 Driver (AT91)",
        description = "LAN driver for Asix 88796. AT91 only.",
        requires = { "HW_MCU_AT91", "NUT_EVENT", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "arm/dev/ax88796.c" },
    },     
    {
        name = "nutarch_arm_dm9000e",
        brief = "DM9000E Driver (AT91)",
        description = "LAN driver for Davicom DM9000E. AT91 only.",
        requires = { "HW_MCU_AT91", "NUT_EVENT", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "arm/dev/dm9000e.c" },
    },     
}

