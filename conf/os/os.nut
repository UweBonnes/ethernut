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
-- Revision 1.4  2004/08/18 16:05:38  haraldkipp
-- Use consistent directory structure
--
-- Revision 1.3  2004/08/18 13:46:10  haraldkipp
-- Fine with avr-gcc
--
-- Revision 1.2  2004/08/03 15:09:31  haraldkipp
-- Another change of everything
--
-- Revision 1.1  2004/06/07 16:38:43  haraldkipp
-- First release
--
--

nutos =
{
    --
    -- Initialization
    --
    {
        name = "nutos_init",
        brief = "Initialization",
        sources = { "nutinit.c" },
        targets = { "nutinit.o" }
    },

    --
    -- Thread management
    --
    {
        name = "nutos_thread",
        brief = "Multithreading",
        provides = { "NUT_THREAD" },
        sources = { "thread.c" },
        options = 
        {
            {
                macro = "USE_IRQ_STACK",
                brief = "Seperate IRQ stack",
                description = "If separate IRQ stack is enabled", 
                flavor = "boolean",
                file = "include/cfg/os.h"
            }
        }
    },

    --
    -- Timer management
    --
    {
        name = "nutos_timer",
        brief = "Timer management",
        requires = { "NUT_EVENT" },
        provides = { "NUT_TIMER" },
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
                file = "include/cfg/os.h"
            }
        }
    },

    --
    -- Event management
    --
    {
        name = "nutos_event",
        brief = "Event handling",
        description = "Events provide the core thread synchronization.",
        provides = { "NUT_EVENT" },
        sources = { "event.c" }
    },


    --
    -- Memory management
    --
    {
        name = "nutos_heap",
        brief = "Dynamic memory management",
        provides = { "NUT_HEAPMEM" },
        sources = { "heap.c" }
    },
    {
        name = "nutos_bankmem",
        brief = "Segmented Buffer (AVR)",
        description = "The segmented buffer API can either handle a continuos "..
                      "memory space automatically allocated from heap or it can "..
                      "use banked memory hardware.",
        requires = { "HW_MCU_AVR" },
        provides = { "NUT_SEGMEM" },
        sources = { "bankmem.c" },
        provides = { "NUT_SEGBUF" },
        options = 
        {
            {
                macro = "NUTBANK_COUNT",
                brief = "Number of banks",
                description = "For systems without banked memory this is set to zero.",
                file = "include/cfg/bankmem.h"
            },
            {
                macro = "NUTBANK_START",
                brief = "Bank start address",
                description = "First address of the banked memory area.",
                ctype = "(char *)",
                file = "include/cfg/bankmem.h"
            },
            {
                macro = "NUTBANK_SIZE",
                brief = "Bank size",
                description = "Size of the banked memory area.",
                file = "include/cfg/bankmem.h"
            },
            {
                macro = "NUTBANK_SR",
                brief = "Bank select register",
                description = "Address of the bank select register.",
                ctype = "(char *)",
                file = "include/cfg/bankmem.h"
            }
        }
    },
    {
        name = "nutos_devreg",
        brief = "Device registration",
        description = "Applications need to register devices before using "..
                      "them. This serves two purposes. First it will create "..
                      "a link to the device driver code. Second it will "..
                      "initialize the device hardware.",
        provides = { "NUT_DEVREG" },
        sources = { "devreg.c" }
    },
    {
        name = "nutos_confos",
        brief = "Configuration",
        description = "Initial configuration settings are stored in the AVR EEPROM",
        provides = { "NUT_OSCONFIG" },
        sources = { "confos.c" },
        options = 
        {
            {
                macro = "CONFOS_EE_OFFSET",
                brief = "Location",
                description = "This is the first EEPROM address, where Nut/OS "..
                              "expects its configuration",
                type = "integer",
                file = "include/cfg/eeprom.h"
            }
        }
    },
    {
        name = "nutos_version",
        brief = "Version identifier",
        sources = { "version.c" },
        options = 
        {
            {
                macro = "NUT_VERSION_EXT",
                brief = "Extended",
                description = "User provided extension to the hard coded version information.",
                requires = { "NOT_AVAILABLE" },
                file = "include/cfg/os.h"
            }
        }
    },

    --
    -- Additional functions, not required by the kernel.
    --
    {
        name = "nutos_semaphore",
        brief = "Semaphores",
        description = "Semaphores are not required by the kernel, but "..
                      "may be useful for applications.",
        requires = { "NUT_EVENT" },
        provides = { "NUT_SEMAPHORE" },
        sources = { "semaphore.c" }
    },
    {
        name = "nutos_mutex",
        brief = "Mutual exclusion semaphores",
        description = "Mutex semaphores are not required by the kernel, but "..
                      "may be useful for applications.",
        requires = { "NUT_EVENT" },
        provides = { "NUT_MUTEX" },
        sources = { "mutex.c" }
    },
    {
        name = "nutos_msg",
        brief = "Message queues",
        description = "Message queues are not required by the kernel, but "..
                      "may be useful for applications.",
        requires = { "NUT_EVENT" },
        provides = { "NUT_MQUEUE" },
        sources = { "msg.c" }
    },

    --
    -- Debugging
    --
    {
        name = "nutos_osdebug",
        brief = "OS debug tracing",
        requires = { "NUT_EVENT", "CRT_STREAM_WRITE" },
        provides = { "NUT_OSDEBUG" },
        sources = { "osdebug.c" },
        options = 
        {
            {
                macro = "NUT_DEBUG",
                brief = "OS tracing",
                description = "Used for kernel debugging.\n"..
                              "Enabling this functions will add a lot of "..
                              "extra code and require more RAM. In addition "..
                              "the application must register an output device "..
                              "early and redirect stdout to it.",
                flavor = "boolean",
                file = "include/cfg/os.h"
            }
        }
    }
}
