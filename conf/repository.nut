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


-- The repository contains an enumerated list
-- of all top-level components.
--
-- $Log$
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
avr_port_choice = { " ", "PORTA", "PORTB", "PORTC", "PORTD", "PORTE", "PORTF", "PORTG" }
avr_pin_choice = { " ", "PINA", "PINB", "PINC", "PIND", "PINE", "PINF", "PING" }
avr_ddr_choice = { " ", "DDRA", "DDRB", "DDRC", "DDRD", "DDRE", "DDRF", "DDRG" }
avr_irq_choice = { " ", "INT0", "INT1", "INT2", "INT3", "INT4", "INT5", "INT6", "INT7" }

arm_ld_choice = { " ", "s3c4510b-ram", "eb40a_ram", "gbaxport2" }

repository =
{
    {
        name = "nuttools",
        brief = "Tools",
        description = "Select one only.",
        script = "tools.nut"
    },
    {
        name = "nutarch",
        brief = "Target",
        description = "Select one only.",
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
        brief = "Device drivers",
        description = "Hardware device drivers",
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
    }
}
