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

-- Operating system functions
--
-- $Log$
-- Revision 1.2  2004/08/03 15:09:31  haraldkipp
-- Another change of everything
--
-- Revision 1.1  2004/06/07 16:38:43  haraldkipp
-- First release
--
--

nutos =
{
    {
        name = "nutos_bankmem",
        brief = "AVR memory banking",
        description = "Part of the AVR data space can be used for banked memory.",
        requires = { "AVR_MCU" },
        sources = { "bankmem.c" },
        provides = { "MEMORY_BANKING" },
        options = 
        {
            {
                macro = "NUTBANK_COUNT",
                brief = "Number of banks",
                description = "For systems without banked memory this is set to zero.",
                file = "cfg/bankmem.h"
            },
            {
                macro = "NUTBANK_START",
                brief = "Bank start address",
                description = "First address of the banked memory area.",
                ctype = "(char *)",
                file = "cfg/bankmem.h"
            },
            {
                macro = "NUTBANK_SIZE",
                brief = "Bank size",
                description = "Size of the banked memory area.",
                file = "cfg/bankmem.h"
            },
            {
                macro = "NUTBANK_SR",
                brief = "Bank select register",
                description = "Address of the bank select register.",
                ctype = "(char *)",
                file = "cfg/bankmem.h"
            }
        }
    },
    {
        name = "nutos_confos",
        brief = "Configuration",
        description = "Initial configuration settings are stored in the AVR EEPROM",
        sources = { "confos.c" },
        options = 
        {
            {
                macro = "CONFOS_EE_OFFSET",
                brief = "Location",
                description = "This is the first EEPROM address, where Nut/OS "..
                              "expects its configuration",
                type = "integer",
                file = "cfg/eeprom.h"
            }
        }
    },
    {
        name = "nutos_devreg",
        brief = "Device registration",
        sources = { "devreg.c" }
    },
    {
        name = "nutos_event",
        brief = "Event handling",
        sources = { "event.c" }
    },
    {
        name = "nutos_heap",
        brief = "Dynamic memory management",
        sources = { "heap.c" }
    },
    {
        name = "nutos_msg",
        brief = "Message queues",
        sources = { "msg.c" }
    },
    {
        name = "nutos_mutex",
        brief = "Mutual exclusion semaphores",
        sources = { "mutex.c" }
    },
    {
        name = "nutos_osdebug",
        brief = "OS debug tracing",
        sources = { "osdebug.c" }
    },
    {
        name = "nutos_semaphore",
        brief = "Semaphores",
        sources = { "semaphore.c" }
    },
    {
        name = "nutos_thread",
        brief = "Multithreading",
        sources = { "thread.c" },
        options = 
        {
            {
                macro = "USE_IRQ_STACK",
                brief = "Seperate IRQ stack",
                description = "If separate IRQ stack is enabled", 
                flavor = "boolean",
                file = "cfg/os.h"
            }
        }
    },
    {
        name = "nutos_timer",
        brief = "Timer management",
        sources = { "timer.c" },
        options = 
        {
            {
                macro = "NUT_CPU_FREQ",
                brief = "Fixed MCU clock",
                description = "Frequency of the MCU clock. If disabled, the system "..
                              "will automatically determine this value during initialization "..
                              "by using an additional 32 kHz crystal as a reference clock.",
                flavor = "booldata",
                file = "cfg/os.h"
            }
        }
    },
    {
        name = "nutos_version",
        brief = "Version identifier",
        sources = { "version.c" }
    }
}
