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
-- Revision 1.1  2004/06/07 16:38:43  haraldkipp
-- First release
--
--

avr_bit_choice = { "0", "1", "2", "3", "4", "5", "6", "7" }
avr_port_choice = { "PORTA", "PORTB", "PORTC", "PORTD", "PORTE", "PORTF", "PORTG" }
avr_pin_choice = { "PINA", "PINB", "PINC", "PIND", "PINE", "PINF", "PING" }
avr_ddr_choice = { "DDRA", "DDRB", "DDRC", "DDRD", "DDRE", "DDRF", "DDRG" }

test =
    {
        name = "nutbld",
        display = "General build options",
        xsubdir = "bld",
        script = "bld/bld.nut"
    }
repository =
{
    {
        name = "nutos",
        display = "Operating system functions",
        subdir = "os",
        script = "os/os.nut"
    },
    {
        name = "nutfs",
        display = "File systems",
        subdir = "fs",
        script = "fs/fs.nut"
    },
    {
        name = "nutdev",
        display = "Hardware device drivers",
        subdir = "dev",
        script = "dev/dev.nut"
    },
    {
        name = "nutnet",
        display = "Network functions",
        subdir = "net",
        script = "net/net.nut"
    },
    {
        name = "nutpro",
        display = "High level network protocols",
        subdir = "pro",
        script = "pro/pro.nut"
    },
    {
        name = "nutcrt",
        display = "Hardware dependent C runtime",
        subdir = "crt",
        script = "crt/crt.nut"
    },
    {
        name = "nutc",
        display = "Hardware independent C runtime",
        subdir = "c",
        script = "c/c.nut"
    }
}
