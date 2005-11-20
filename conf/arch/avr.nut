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

-- AVR Architecture
--
-- $Log$
-- Revision 1.5  2005/11/20 14:43:11  haraldkipp
-- AHDLC driver and deprecated AVR UART driver temporarely removed from ICCAVR
-- build. Will look into this later.
--
-- Revision 1.4  2005/10/24 11:14:07  haraldkipp
-- NUT_CPU_FREQ option removed. This is handled in os.nut.
--
-- Revision 1.3  2005/10/04 05:51:49  hwmaier
-- Added CAN driver for AT90CAN128
--
-- Revision 1.2  2005/09/17 09:28:26  drsung
-- Added irqstack.c to sources for avr target.
--
-- Revision 1.1  2005/07/26 15:41:06  haraldkipp
-- All target dependent code is has been moved to a new library named
-- libnutarch. Each platform got its own script.
--
--

nutarch_avr =
{
    --
    -- Runtime Initialization
    --
    {
        name = "nutarch_avr_cstartup_icc",
        brief = "ICCAVR Startup",
        description = "There are four different ICCAVR startup files available.\n\n"..
                      "crtnut.s, if globals and static variables use less than 4kB.\n"..
                      "crtenut.s, same as above but including EEPROM emulation.\n"..
                      "crtnutram.s, if globals and static variables use more than 4kB.\n"..
                      "crtenutram.s, same as above but including EEPROM emulation.\n\n"..
                      "Ethernut 1.3 Rev-G boards require EEPROM emulation.",
        sources = {
            "avr/init/crtnut.s",
            "avr/init/crtnutram.s",
            "avr/init/crtenut.s",
            "avr/init/crtenutram.s"
        },
        targets = {
            "avr/init/crtnut.o",
            "avr/init/crtnutram.o",
            "avr/init/crtenut.o",
            "avr/init/crtenutram.o"
        },
        requires = { "TOOL_CC_AVR", "TOOL_ICC" },
    },

    --
    -- Context Switching
    --
    {
        name = "nutarch_avr_context_gcc",
        brief = "Context Switching (AVRGCC)",
        provides = { "NUT_CONTEXT_SWITCH" },
        requires = { "HW_MCU_AVR", "TOOL_GCC" },
        sources = { "avr/os/context_gcc.c" },
    },
    {
        name = "nutarch_avr_context_icc",
        brief = "Context Switching (ICCAVR)",
        provides = { "NUT_CONTEXT_SWITCH" },
        requires = { "HW_MCU_AVR", "TOOL_ICC" },
        sources = { "avr/os/context_icc.c" },
    },

    --
    -- System Timer Hardware
    --
    {
        name = "nutarch_avr_ostimer",
        brief = "System Timer",
        requires = { "HW_MCU_AVR" },
        provides = { "NUT_OSTIMER_DEV" },
        sources = { "avr/dev/ostimer.c" },
    },

    --
    -- AVR interrupt handling.
    --
    {
        name = "nutarch_avr_irq",
        brief = "Interrupt Handler",
        requires = { "HW_MCU_AVR" },
        provides = { "DEV_IRQ_AVR" },
        sources =
        {
            "avr/dev/ivect01.c",
            "avr/dev/ivect02.c",
            "avr/dev/ivect03.c",
            "avr/dev/ivect04.c",
            "avr/dev/ivect05.c",
            "avr/dev/ivect06.c",
            "avr/dev/ivect07.c",
            "avr/dev/ivect08.c",
            "avr/dev/ivect09.c",
            "avr/dev/ivect10.c",
            "avr/dev/ivect11.c",
            "avr/dev/ivect12.c",
            "avr/dev/ivect13.c",
            "avr/dev/ivect14.c",
            "avr/dev/ivect15.c",
            "avr/dev/ivect16.c",
            "avr/dev/ivect17.c",
            "avr/dev/ivect18.c",
            "avr/dev/ivect19.c",
            "avr/dev/ivect20.c",
            "avr/dev/ivect21.c",
            "avr/dev/ivect22.c",
            "avr/dev/ivect23.c",
            "avr/dev/ivect24.c",
            "avr/dev/ivect25.c",
            "avr/dev/ivect26.c",
            "avr/dev/ivect27.c",
            "avr/dev/ivect28.c",
            "avr/dev/ivect29.c",
            "avr/dev/ivect30.c",
            "avr/dev/ivect31.c",
            "avr/dev/ivect32.c",
            "avr/dev/ivect33.c",
            "avr/dev/ivect34.c",
            "avr/dev/ivect35.c",
            "avr/dev/ivect36.c",
            "avr/dev/irqstack.c"
        },
        options =
        {
            {
                macro = "IRQSTACK_SIZE",
                brief = "Separate Stack",
                description = "If this option is enabled, Nut/OS will use a separate "..
                              "stack of the specified size for interrupts.\n"..
                              "If this option is disabled, make sure to reserve "..
                              "additional stack space for each thread.\n\n"..
                              "Available for AVR-GCC only.",
                requires = { "HW_MCU_AVR", "TOOL_GCC" },
                flavor = "booldata",
                file = "include/cfg/dev.h"
            },
        }
    },

    --
    -- Device Drivers
    --
    {
        name = "nutarch_avr_ahdlc",
        brief = "AHDLC Protocol",
        description = "HDLC driver, required for PPP.",
        requires = { "HW_UART_AVR", "NUT_EVENT", "TOOL_GCC" },
        provides = { "PROTO_HDLC" },
        sources = { "avr/dev/ahdlc0.c", "avr/dev/ahdlc1.c", "avr/dev/ahdlcavr.c" },
        options =
        {
            {
                macro = "NUT_THREAD_AHDLCRXSTACK",
                brief = "Receiver Thread Stack",
                description = "Number of bytes to be allocated for the stack of the AHDLC receive thread.",
                default = "512",
                type = "integer",
                file = "include/cfg/ahdlc.h"
            }
        }
    },
    {
        name = "nutarch_avr_debug",
        brief = "UART Debug Output",
        description = "This simple UART output driver uses polling instead of "..
                      "interrupts and can be used within interrupt routines. It "..
                      "is mainly used for debugging and tracing.\n\n"..
                      "Call NutRegisterDevice(&devDebug0, 0, 0) for U(S)ART0 and "..
                      "NutRegisterDevice(&devDebug1, 0, 0) for U(S)ART1. "..
                      "Then you can use any of the stdio functions to open "..
                      "device uart0 or uart1 resp.",
        requires = { "HW_UART_AVR" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "avr/dev/debug0.c", "avr/dev/debug1.c" }
    },
    {
        name = "nutarch_avr_usart0",
        brief = "USART0 Driver",
        description = "Hardware specific USART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = {
                        "HW_MCU_AVR", "DEV_IRQ_AVR", "DEV_UART_GENERIC",
                        "NUT_EVENT", "CRT_HEAPMEM"
        },
        provides = { "DEV_UART_SPECIFIC" },
        sources = { "avr/dev/usart0avr.c" },
        options =
        {
            {
                macro = "UART0_RTS_BIT",
                brief = "RTS Bit",
                description = "Bit number of UART0 RTS handshake output. If enabled, "..
                              "the driver provides RS 232 input hardware handshake.\n\n"..
                              "Ethernut 2.x:\n"..
                              "Short JP1 pins 4 and 6 to select bit 2 on PORTE.",
                provides = { "UART0_RTS_BIT" },
                flavor = "booldata",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "UART0_RTS_AVRPORT",
                brief = "RTS Port",
                description = "Port register name of UART0 RTS handshake output.",
                requires = { "UART0_RTS_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "UART0_CTS_IRQ",
                brief = "CTS Interrupt",
                description = "Interrupt number of UART0 CTS handshake input. If enabled, "..
                              "the driver provides RS 232 output hardware handshake.\n\n"..
                              "Ethernut 2.x:\n"..
                              "Short JP1 pins 7 and 8 to select INT7.",
                flavor = "booldata",
                type = "enumerated",
                choices = avr_irq_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "UART0_HDX_BIT",
                brief = "Half Duplex Bit",
                description = "Bit number of UART0 half duplex control output. If enabled, "..
                              "the driver provides RS 485 half duplex mode.",
                provides = { "UART0_HDX_BIT" },
                flavor = "booldata",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "UART0_HDX_AVRPORT",
                brief = "Half Duplex Port",
                description = "Port register name of UART0 half duplex control output.",
                requires = { "UART0_HDX_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
        }
    },
    {
        name = "nutarch_avr_usart1",
        brief = "USART1 Driver",
        description = "Hardware specific USART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = {
                        "HW_MCU_AVR", "DEV_IRQ_AVR", "DEV_UART_GENERIC",
                        "NUT_EVENT", "CRT_HEAPMEM"
        },
        provides = { "DEV_UART_SPECIFIC" },
        sources = { "avr/dev/usart1avr.c" },
        options =
        {
            {
                macro = "UART1_RTS_BIT",
                brief = "RTS Bit",
                description = "Bit number of UART1 RTS handshake output. If enabled, "..
                              "the driver provides RS 232 input hardware handshake.\n\n"..
                              "Ethernut 2.x:\n"..
                              "Short JP1 pins 4 and 6 to select bit 2 on PORTE.",
                provides = { "UART1_RTS_BIT" },
                flavor = "booldata",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "UART1_RTS_AVRPORT",
                brief = "RTS Port",
                description = "Port register name of UART1 RTS handshake output.",
                requires = { "UART1_RTS_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "UART1_CTS_IRQ",
                brief = "CTS Interrupt",
                description = "Interrupt number of UART1 CTS handshake input. If enabled, "..
                              "the driver provides RS 232 output hardware handshake.\n\n"..
                              "Ethernut 2.x:\n"..
                              "Short JP1 pins 7 and 8 to select INT7.",
                flavor = "booldata",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "UART1_HDX_BIT",
                brief = "Half Duplex Bit",
                description = "Bit number of UART1 half duplex control output. If enabled, "..
                              "the driver provides RS 485 half duplex mode.",
                provides = { "UART1_HDX_BIT" },
                flavor = "booldata",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "UART1_HDX_AVRPORT",
                brief = "Half Duplex Port",
                description = "Port register name of UART1 half duplex control output.",
                requires = { "UART1_HDX_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
        }
    },
    {
        name = "nutarch_avr_uart",
        brief = "UART driver",
        description = "Interrupt driven, buffered UART driver.\n"..
                      "Deprecated, use the USART driver.",
        sources = { "avr/dev/uart0.c", "avr/dev/uart1.c", "avr/dev/uartavr.c" },
        requires = { "HW_MCU_AVR", "DEV_IRQ_AVR", "NUT_EVENT", "CRT_HEAPMEM", "TOOL_GCC" },
        provides = { "DEV_FILE", "DEV_READ", "DEV_WRITE" },
    },
    {
        name = "nutarch_avr_lanc111",
        brief = "LAN91C111 Driver",
        description = "LAN driver for SMSC LAN91C111. ATmega128 only.",
        requires = { "HW_MCU_AVR", "NUT_EVENT", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "avr/dev/lanc111.c" },
        options =
        {
            {
                macro = "LANC111_BASE_ADDR",
                brief = "Controller Base Address",
                description = "The driver supports memory mapped controllers only, using "..
                              "the specified based address.\n\n"..
                              "The Ethernut 2 reference design uses 0xC000.",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LANC111_SIGNAL_IRQ",
                brief = "Ethernet Interrupt",
                description = "Ethernet controller interrupt, typically INT5.",
                default = "INT5",
                type = "enumerated",
                choices = avr_irq_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LANC111_RESET_BIT",
                brief = "Ethernet Reset Bit",
                description = "Bit number of the Ethernet controller reset output.\n\n"..
                              "Should be disabled when the LAN91C111 hardware reset "..
                              "is not connected to a port pin.\n\n"..
                              "On Ethernut 2 boards the controller's reset pin "..
                              "is connected to the system reset. However, on early "..
                              "board revisions of Ethernut 2.0 no supervisory chip "..
                              "had been mounted, which causes problems with some "..
                              "power supplies. Mounting R30 or R31 connects the reset "..
                              "to bit 3 or bit 7 of PORTE.",
                provides = { "LANC111_RESET_BIT" },
                flavor = "booldata",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LANC111_RESET_AVRPORT",
                brief = "Ethernet Reset Port",
                description = "Port register name of the Ethernet controller reset output.",
                requires = { "LANC111_RESET_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
        }
    },
    {
        name = "nutarch_avr_rtl8019as",
        brief = "RTL8019AS Driver",
        requires = { "HW_MCU_AVR", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "avr/dev/nicrtl.c", "avr/dev/eth0rtl.c" },
        options =
        {
            {
                macro = "RTL_BASE_ADDR",
                brief = "Controller Base Address",
                description = "The driver supports memory mapped controllers only, using "..
                              "the specified based address.\n\n"..
                              "The Ethernut 1 reference design uses 0x8300.",
                default = "0x8300",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "RTL_SIGNAL_IRQ",
                brief = "Ethernet Interrupt",
                description = "Ethernet controller interrupt, typically INT5.",
                default = "INT5",
                type = "enumerated",
                choices = avr_irq_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "RTL_IRQ_RISING_EDGE",
                brief = "IRQ Rising Edge Trigger",
                description = "Configures rising edge trigger for RTL interrupt.\n\n"..
                              "Don't enable this for Ethernut boards! "..
                              "Used only on custom hardware designs without "..
                              "inverter gate for the interrupt.\n\n"..
                              "Valid only for ATMega 128 or AT90CAN128 parts, "..
                              "not supported by ATMega 103.",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "RTL_RESET_BIT",
                brief = "Ethernet Reset Bit",
                description = "Bit number of the Ethernet controller reset output.\n\n"..
                              "Early Ethernut revisions use bit 4 on PORTE to hard "..
                              "reset the RTL8019AS. Later the controller's reset pin "..
                              "had been connected to the system reset.\n\n"..
                              "Should be disabled when the RTL8019AS hardware reset "..
                              "is not connected to a port pin.",
                provides = { "RTL_RESET_BIT" },
                flavor = "booldata",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "RTL_RESET_AVRPORT",
                brief = "Ethernet Reset Port",
                description = "Port register name of the Ethernet controller reset output.",
                requires = { "RTL_RESET_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "RTL_EESK_BIT",
                brief = "Ethernet EESK Bit",
                description = "Bit number of the Ethernet controller EEPROM clock input.\n\n"..
                              "This function, when enabled, uses three port pins to emulate "..
                              "a serial EEPROM for the RTL8019AS. The clock pin is driven "..
                              "by the Ethernet controller and will be monitored by the "..
                              "driver to shift the correct values out of the EEDO pin.\n\n"..
                              "Ethernut 1.3 Rev-G uses Bit 5 on Port C. On previous board "..
                              "revisions the EEDO pin is connected to high. As a result, "..
                              "the full duplex bit is permanently enabled. But the RTL8019AS "..
                              "does not provide link level handshake, which makes the "..
                              "peer believe, that it's half duplex only and any full duplex "..
                              "traffic from the RTL8019AS is considered a collision. This "..
                              "is not fatal, but noticeably reduces throughput.\n\n"..
                              "Should be disabled when the hardware doesn't support "..
                              "EEPROM Emulation or provides a real EEPROM.",
                provides = { "RTL_EESK_BIT" },
                flavor = "booldata",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "RTL_EESK_AVRPORT",
                brief = "Ethernet EESK Port",
                description = "Port register name of the Ethernet controller EEPROM clock input.\n\n"..
                              "Ethernut 1.3 Rev-G uses Bit 5 on Port C.",
                requires = { "RTL_EESK_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "RTL_EEDO_BIT",
                brief = "Ethernet EEDO Bit",
                description = "Bit number of the Ethernet controller EEPROM data output.\n\n"..
                              "Ethernut 1.3 Rev-G uses Bit 6 on Port C.",
                requires = { "RTL_EESK_BIT" },
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "RTL_EEDO_AVRPORT",
                brief = "Ethernet EEDO Port",
                description = "Port register name of the Ethernet controller EEPROM data output.\n\n"..
                              "Ethernut 1.3 Rev-G uses Bit 6 on Port C.",
                requires = { "RTL_EESK_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "RTL_EEMU_BIT",
                brief = "Ethernet EEMU Bit",
                description = "Bit number of the Ethernet controller EEPROM emulator output.\n\n"..
                              "Ethernut 1.3 Rev-G uses Bit 7 on Port C to keep A15 high.\n\n"..
                              "Disable this option if the hardware doesn't use this signal.",
                requires = { "RTL_EESK_BIT" },
                provides = { "RTL_EEMU_BIT" },
                flavor = "booldata",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "RTL_EEMU_AVRPORT",
                brief = "Ethernet EEMU Port",
                description = "Port register name of the Ethernet controller reset output.\n\n"..
                              "Ethernut 1.3 Rev-G uses Bit 7 on Port C.",
                requires = { "RTL_EEMU_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            }

        }
    },
    {
        name = "nutarch_avr_ide",
        brief = "ATA/IDE driver",
        description = "Mass storage driver.",
        requires = { "HW_MCU_AVR" },
        sources = { "avr/dev/ide.c" }
    },
    {
        name = "nutarch_avr_cs8900",
        brief = "CS8900 Driver",
        requires = { "HW_MCU_AVR", "NUT_TIMER" },
        sources = { "avr/dev/cs8900.c", "avr/dev/eth0cs.c" }
    },
    {
        name = "nutarch_avr_wlan",
        brief = "WLAN Driver",
        requires = { "NUT_EVENT", "HW_MCU_AVR" },
        provides = { "NET_PHY" },
        sources = { "avr/dev/wlan.c", "avr/dev/wlandrv.c" }
    },

    --
    -- Additional Hardware Support
    --
    {
        name = "nutarch_avr_adc",
        brief = "ADC Driver",
        description = "Driver for the ATmega128 analog to digital converter.\n\n"..
                      "Only available for AVR-GCC.\n\n"..
                      "Contributed by Ole Reinhardt from www.kernelconcepts.de",
        requires = { "HW_MCU_AVR", "TOOL_GCC" },
        sources = { "avr/dev/adc.c" },
        options =
        {
            {
                macro = "ADC_BUF_SIZE",
                brief = "Buffer Size",
                description = "Defaults to 16",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "ADC_INITIAL_CHANNEL",
                brief = "Initial Channel",
                description = "Set to ADC0..ADC7",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "ADC_INITIAL_REF",
                brief = "Initial Reference",
                description = "Set to AVCC (supply voltage), "..
                              "AREF (external reference) "..
                              "or INTERNAL_256 (2.56V)",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "ADC_INITIAL_MODE",
                brief = "Initial Mode",
                description = "Set to\n\nFREE_RUNNING:\n"..
                              "Free-running mode. Samples continuously taken "..
                              "every 13 cycles of ADC clock after "..
                              "ADC_start_conversion() is called.\n\n"..
                              "SINGLE_CONVERSION:\n"..
                              "Single-conversion mode. One sample taken every "..
                              "time ADC_start_conversion() is called.",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "ADC_INITIAL_PRESCALE",
                brief = "Initial Prescale",
                description = "Set to\n"..
                              "ADC_PRESCALE_DIV2 for CPU clk/2\n"..
                              "ADC_PRESCALE_DIV4 for CPU clk/4\n"..
                              "ADC_PRESCALE_DIV8 for CPU clk/8\n"..
                              "ADC_PRESCALE_DIV16 for CPU clk/16\n"..
                              "ADC_PRESCALE_DIV32 for CPU clk/32\n"..
                              "ADC_PRESCALE_DIV64 for CPU clk/64\n"..
                              "ADC_PRESCALE_DIV128 for CPU clk/128",
                file = "include/cfg/arch/avrpio.h"
            },
        }
    },
    {
        name = "nutarch_avr_ir",
        brief = "IR Driver",
        requires = { "HW_MCU_AVR", "NUT_EVENT" },
        description = "Infrared remote control for Sony protocol.",
        sources = { "avr/dev/ir.c", "avr/dev/irsony.c" }
    },
    {
        name = "nutarch_avr_hd44780",
        brief = "HD44780 Driver",
        requires = { "HW_MCU_AVR" },
        provides = { "DEV_FILE", "DEV_WRITE" },
        sources = { "avr/dev/hd44780.c" },
        options =
        {
            {
                macro = "LCD_4x20",
                brief = "LCD 4x20",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_4x16",
                brief = "LCD 4x16",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },            {
                macro = "LCD_2x40",
                brief = "LCD 2x40",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_2x20",
                brief = "LCD 2x20",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_2x16",
                brief = "LCD 2x16",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_2x8",
                brief = "LCD 2x8",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },            {
                macro = "LCD_1x20",
                brief = "LCD 1x20",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_1x16",
                brief = "LCD 1x16",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_1x8",
                brief = "LCD 1x8",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "KS0073_CONTROLLER",
                brief = "KS0073 Controller",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_SHORT_DELAY",
                brief = "Short delay",
                description = "Number of milliseconds",
                flavor = "booldata",
                type = "integer",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_LONG_DELAY",
                brief = "Long delay",
                description = "Number of milliseconds",
                flavor = "booldata",
                type = "integer",
                file = "include/cfg/arch/avrpio.h"
            }

        }
    },
    {
        name = "nutarch_arv_hd44780_bus",
        brief = "HD44780 Bus Driver",
        description = "Alternate memory mapped LCD Driver.\n\n"..
                      "This is a terminal device driver for a memory "..
                      "mapped HD44780 compatible LCD. It is connected "..
                      "to the databus / adressbus. A chip select is "..
                      "generated from /RD, /WR, and the address decoder. "..
                      "It is connected to the LCD's enable signal. "..
                      "A0 is connected to the register select pin and A1 "..
                      "to the read/write signal. Therefore you'll read from "..
                      "an address with an offset of two.\n\n"..
                      "Only available for AVR-GCC.\n\n"..
                      "Contributed by Ole Reinhardt from www.kernelconcepts.de",
        requires = { "HW_MCU_AVR", "TOOL_GCC" },
        provides = { "DEV_FILE", "DEV_WRITE" },
        sources = { "avr/dev/hd44780_bus.c" },
        options =
        {
            {
                macro = "LCD_4x20",
                brief = "LCD 4x20",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_4x16",
                brief = "LCD 4x16",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },            {
                macro = "LCD_2x40",
                brief = "LCD 2x40",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_2x20",
                brief = "LCD 2x20",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_2x16",
                brief = "LCD 2x16",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_2x8",
                brief = "LCD 2x8",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },            {
                macro = "LCD_1x20",
                brief = "LCD 1x20",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_1x16",
                brief = "LCD 1x16",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "LCD_1x8",
                brief = "LCD 1x8",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "KS0073_CONTROLLER",
                brief = "KS0073 Controller",
                flavor = "boolean",
                file = "include/cfg/arch/avrpio.h"
            }
        }
    },
    {
        name = "nutarch_avr_sja1000",
        brief = "SJA1000 CAN Driver",
        description = "Driver for SJA1000 CAN-Bus controller.\n\n"..
                      "The SJA1000 controller is connected to the memory "..
                      "bus. It's base address and interrupt is set by "..
                      "NutRegisterDevice().\n\n"..
                      "Only available for AVR-GCC.\n\n"..
                      "Contributed by Ole Reinhardt from www.kernelconcepts.de",
        provides = { "DEV_CAN_SPECIFIC"},
        requires = { "HW_MCU_AVR", "TOOL_GCC" },
        sources = { "avr/dev/sja1000.c" },
        options =
        {
            {
                macro = "SJA_SIGNAL_BIT",
                brief = "Interrupt Number",
                description = "Default is 7. Must correspond "..
                              "to SJA_SIGNAL.",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SJA_SIGNAL",
                brief = "Interrupt Signal",
                description = "Default is sig_INTERRUPT7. Must correspond "..
                              "to SJA_SIGNAL_BIT.",
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SJA_EICR",
                brief = "Interrupt Control",
                description = "Default is EICRB. Must correspond "..
                              "to SJA_SIGNAL_BIT.",
                file = "include/cfg/arch/avrpio.h"
            }
        }
    },
    {
        name = "nutarch_avr_atcan",
        brief = "AT CAN Driver",
        description = "Driver for Atmel's internal Full CAN controller "..
                      "found in AT90 parts like the AT90CAN128 MCU.\n\n"..
                      "Requires a fixed MCU clock setting of either 12 or 16 MHz. "..
                      "Check setting in System Timer category.\n"..
                      "Currently only available for AVR-GCC.\n\n"..
                      "Contributed by Henrik Maier from www.proconx.com",
        requires = { "HW_CAN_AVR", "TOOL_GCC"},
        provides = { "DEV_CAN_SPECIFIC"},
        sources = { "avr/dev/atcan.c" },
        options =
        {
        }
    },
    {
        name = "nutarch_avr_spidigio",
        brief = "Shift Register I/O",
        description = "Digital I/O using shift registers.\n"..
                      "This driver supports upto 32 digital inputs or outputs "..
                      "and occupies 5 port bits only:\n"..
                      "- Shift register data output\n"..
                      "- Shift register data input\n"..
                      "- Shift register clock output\n"..
                      "- Shift register input latch\n"..
                      "- Shift register output latch\n"..
                      "The hardware is very simple, using standard TTL 74165 "..
                      "shift registers for inputs and Allegro's UCN5841A for "..
                      "outputs. All shift are connected in series, with the "..
                      "first output register connected to the MCU data output "..
                      "and the last input register connected to the data input.",
        sources = { "avr/dev/spidigio.c" },
        requires = { "HW_MCU_AVR" },
        options =
        {
            {
                macro = "SPIDIGIO_SOUT_BIT",
                brief = "Shift Output Bit",
                description = "Bit number of the serial data output.\n"..
                              "Coconut uses bit 5 of PORTD.",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPIDIGIO_SOUT_AVRPORT",
                brief = "Shift Output Port",
                description = "Port register name of serial data output.",
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPIDIGIO_SIN_BIT",
                brief = "Shift Input Bit",
                description = "Bit number of the serial data input.\n"..
                              "Coconut uses bit 6 of PIND.",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPIDIGIO_SIN_AVRPORT",
                brief = "Shift Input Port",
                description = "Port register name of serial data input.",
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPIDIGIO_SCLK_BIT",
                brief = "Clock Output Bit",
                description = "Bit number of the serial clock output.\n"..
                              "Coconut uses bit 7 of PORTD.",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPIDIGIO_SCLK_AVRPORT",
                brief = "Clock Output Port",
                description = "Port register name of serial clock output.",
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPIDIGIO_LDI_BIT",
                brief = "Input Latch Bit",
                description = "Bit number of the input latch output.\n"..
                              "Coconut uses bit 7 of PORTB.",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPIDIGIO_LDI_AVRPORT",
                brief = "Input Latch Port",
                description = "Port register name of the input latch output.",
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPIDIGIO_LDO_BIT",
                brief = "Output Latch Bit",
                description = "Bit number of the output latch signal, which is "..
                              "connected to the strobe input of the UCN5841A\n"..
                              "Coconut uses bit 5 of PORTB.",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avrpio.h"
            },
            {
                macro = "SPIDIGIO_LDO_AVRPORT",
                brief = "Output Latch Port",
                description = "Port register name of the output latch output.",
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avrpio.h"
            },
        }
    },
    {
        name = "nutarch_avr_twif",
        brief = "TWI Driver",
        requires = { "HW_MCU_AVR", "NUT_EVENT" },
        provides = { "DEV_TWI" },
        sources = { "avr/dev/twif.c" }
    },
    {
        name = "nutarch_avr_vs1001k",
        brief = "VS1001 Driver",
        requires = { "NUT_SEGBUF", "HW_MCU_AVR" },
        sources = { "avr/dev/vs1001k.c" }
    },
    {
        name = "nutarch_avr_pcmcia",
        brief = "PCMCIA Driver",
        requires = { "HW_MCU_AVR" },
        sources = { "avr/dev/pcmcia.c" },
    },
}
