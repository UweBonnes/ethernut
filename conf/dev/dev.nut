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
-- Revision 1.9  2004/10/03 18:39:12  haraldkipp
-- GBA debug output on screen
--
-- Revision 1.8  2004/09/25 15:43:54  drsung
-- Light change for separate interrupt stack. Now it depends also
-- on macro HW_MCU_AVR.
--
-- Revision 1.7  2004/09/22 08:21:43  haraldkipp
-- No ATmega103 support for LAN91C111. Is there any hardware?
-- List of devices sorted by platform.
-- Separate IRQ stack for AVR is configurable.
-- Configurable ports for digital I/O shift register.
-- Configurable handshake ports for AVR USART.
--
-- Revision 1.6  2004/09/07 19:11:15  haraldkipp
-- Simplified IRQ handling to get it done for EB40A
--
-- Revision 1.5  2004/09/01 14:04:57  haraldkipp
-- Added UART handshake and EEPROM emulation port bits
--
-- Revision 1.4  2004/08/18 16:05:38  haraldkipp
-- Use consistent directory structure
--
-- Revision 1.3  2004/08/18 13:46:09  haraldkipp
-- Fine with avr-gcc
--
-- Revision 1.2  2004/08/03 15:09:31  haraldkipp
-- Another change of everything
--
-- Revision 1.1  2004/06/07 16:35:53  haraldkipp
-- First release
--
--

nutdev =
{
    --
    -- AVR interrupt handling.
    --
    {
        name = "nutdev_irq_avr",
        brief = "Interrupt handler (AVR)",
        requires = { "HW_MCU_AVR" },
        provides = { "DEV_IRQ_AVR" },
        sources = 
        { 
            "ihndlr.c",
            "irqstack.c",
            "ivect01.c",
            "ivect02.c",
            "ivect03.c",
            "ivect04.c",
            "ivect05.c",
            "ivect06.c",
            "ivect07.c",
            "ivect08.c",
            "ivect09.c",
            "ivect10.c",
            "ivect11.c",
            "ivect12.c",
            "ivect13.c",
            "ivect14.c",
            "ivect15.c",
            "ivect16.c",
            "ivect17.c",
            "ivect18.c",
            "ivect19.c",
            "ivect20.c",
            "ivect21.c",
            "ivect22.c",
            "ivect23.c",
            "ivect24.c",
            "ivect25.c",
            "ivect26.c",
            "ivect27.c",
            "ivect28.c",
            "ivect29.c",
            "ivect30.c",
            "ivect31.c",
            "ivect32.c",
            "ivect33.c",
            "ivect34.c"
        },
        options = 
        {
            {
                macro = "IRQSTACK_SIZE",
                brief = "Separate Stack",
                description = "If this option is enabled, Nut/OS will use a separate "..
                              "stack of the specified size for interrupts.\n"..
                              "If this option is disabled, make sure to reserve "..
                              "additional stack space for each thread.",
                requires = { "HW_MCU_AVR", "TOOL_GCC" },
                flavor = "booldata",
                file = "include/cfg/dev.h"
            },
        }
    },

    --
    -- AVR UART drivers.
    --
    {
        name = "nutdev_debug_avr",
        brief = "UART Debug Output (AVR)",
        description = "Simple UART output",
        requires = { "HW_UART_AVR" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "debug0.c", "debug1.c" }
    },
    {
        name = "nutdev_usart0_avr",
        brief = "USART0 Driver (AVR)",
        description = "Hardware specific USART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = { 
                        "HW_MCU_AVR", "DEV_IRQ_AVR", "DEV_UART_GENERIC", 
                        "NUT_EVENT", "CRT_HEAPMEM" 
        },
        provides = { "DEV_UART_SPECIFIC" },
        sources = { "usart0avr.c" },
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
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "UART0_RTS_PORT",
                brief = "RTS Port",
                description = "Port register name of UART0 RTS handshake output.",
                requires = { "UART0_RTS_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
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
                file = "include/cfg/arch/avr.h"
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
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "UART0_HDX_PORT",
                brief = "Half Duplex Port",
                description = "Port register name of UART0 half duplex control output.",
                requires = { "UART0_HDX_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
            },
        }
    },
    {
        name = "nutdev_usart1_avr",
        brief = "USART1 Driver (AVR)",
        description = "Hardware specific USART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = { 
                        "HW_MCU_AVR", "DEV_IRQ_AVR", "DEV_UART_GENERIC", 
                        "NUT_EVENT", "CRT_HEAPMEM" 
        },
        provides = { "DEV_UART_SPECIFIC" },
        sources = { "usart1avr.c" },
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
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "UART1_RTS_PORT",
                brief = "RTS Port",
                description = "Port register name of UART1 RTS handshake output.",
                requires = { "UART1_RTS_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
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
                file = "include/cfg/arch/avr.h"
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
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "UART1_HDX_PORT",
                brief = "Half Duplex Port",
                description = "Port register name of UART1 half duplex control output.",
                requires = { "UART1_HDX_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
            },
        }
    },
    {
        name = "nutdev_uart_avr",
        brief = "UART driver (AVR)",
        description = "Interrupt driven, buffered UART driver.\n"..
                      "Deprecated, use the USART driver.",
        sources = { "uart0.c", "uart1.c", "uartavr.c" },
        requires = { "HW_MCU_AVR", "DEV_IRQ_AVR", "NUT_EVENT", "CRT_HEAPMEM" },
        provides = { "DEV_FILE", "DEV_READ", "DEV_WRITE" },
    },
    {
        name = "nutdev_uart_spi",
        brief = "SPI UART Driver (AVR)",
        description = "Deprecated driver for UARTs connected via SPI.",
        requires = { "CRT_HEAPMEM", "HW_MCU_AVR" },
        provides = { "DEV_FILE", "DEV_READ", "DEV_WRITE" },
        sources = { "uarts.c", "uartspi.c" },
    },

    --
    -- AVR LAN drivers.
    --
    {
        name = "nutdev_cs8900_avr",
        brief = "CS8900 Driver (AVR)",
        requires = { "HW_MCU_AVR", "NUT_TIMER" },
        sources = { "cs8900.c", "eth0cs.c" }
    },
    {
        name = "nutdev_lanc111_avr",
        brief = "LAN91C111 Driver (AVR)",
        description = "LAN driver for SMSC LAN91C111. ATmega128 only.",
        requires = { "HW_MCU_ATMEGA128", "NUT_EVENT", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "lanc111.c" },
        options = 
        {
            {
                macro = "LANC111_BASE_ADDR",
                brief = "Controller Base Address",
                description = "The driver supports memory mapped controllers only, using "..
                              "the specified based address.\n\n"..
                              "The Ethernut 2 reference design uses 0xC000.",
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "LANC111_SIGNAL_IRQ",
                brief = "Ethernet Interrupt",
                description = "Ethernet controller interrupt, typically INT5.",
                type = "enumerated",
                choices = avr_irq_choice,
                file = "include/cfg/arch/avr.h"
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
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "LANC111_RESET_PORT",
                brief = "Ethernet Reset Port",
                description = "Port register name of the Ethernet controller reset output.",
                requires = { "LANC111_RESET_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
            },
        }
    },
    {
        name = "nutdev_rtl8019as_avr",
        brief = "RTL8019AS Driver (AVR)",
        requires = { "HW_MCU_AVR", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "nicrtl.c", "eth0rtl.c" },
        options = 
        {
            {
                macro = "RTL_BASE_ADDR",
                brief = "Controller Base Address",
                description = "The driver supports memory mapped controllers only, using "..
                              "the specified based address.\n\n"..
                              "The Ethernut 1 reference design uses 0x8300.",
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "RTL_SIGNAL_IRQ",
                brief = "Ethernet Interrupt",
                description = "Ethernet controller interrupt, typically INT5.",
                type = "enumerated",
                choices = avr_irq_choice,
                file = "include/cfg/arch/avr.h"
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
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "RTL_RESET_PORT",
                brief = "Ethernet Reset Port",
                description = "Port register name of the Ethernet controller reset output.",
                requires = { "RTL_RESET_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
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
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "RTL_EESK_PORT",
                brief = "Ethernet EESK Port",
                description = "Port register name of the Ethernet controller EEPROM clock input.\n\n"..
                              "Ethernut 1.3 Rev-G uses Bit 5 on Port C.",
                requires = { "RTL_EESK_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "RTL_EEDO_BIT",
                brief = "Ethernet EEDO Bit",
                description = "Bit number of the Ethernet controller EEPROM data output.\n\n"..
                              "Ethernut 1.3 Rev-G uses Bit 6 on Port C.",
                requires = { "RTL_EESK_BIT" },
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "RTL_EEDO_PORT",
                brief = "Ethernet EEDO Port",
                description = "Port register name of the Ethernet controller EEPROM data output.\n\n"..
                              "Ethernut 1.3 Rev-G uses Bit 6 on Port C.",
                requires = { "RTL_EESK_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
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
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "RTL_EEMU_PORT",
                brief = "Ethernet EEMU Port",
                description = "Port register name of the Ethernet controller reset output.\n\n"..
                              "Ethernut 1.3 Rev-G uses Bit 7 on Port C.",
                requires = { "RTL_EEMU_BIT" },
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
            }

        }
    },
    {
        name = "nutdev_wlan_avr",
        brief = "WLAN Driver (AVR)",
        requires = { "NUT_EVENT", "HW_MCU_AVR" },
        provides = { "NET_PHY" },
        sources = { "wlan.c", "wlandrv.c" }
    },

    --
    -- AVR WAN drivers.
    --
    {
        name = "nutdev_ppp",
        brief = "PPP Driver (AVR)",
        requires = { "HW_MCU_AVR", "NUT_TIMER", "PROTO_HDLC" },
        provides = { "NET_PHY" },
        sources = { "ppp.c" }
    },
    {
        name = "nutdev_ahdlc_avr",
        brief = "AHDLC Protocol (AVR)",
        requires = { "HW_UART_AVR", "NUT_EVENT" },
        provides = { "PROTO_HDLC" },
        sources = 
        { 
            "ahdlc0.c",
            "ahdlc1.c",
            "ahdlcavr.c"
        }
    },

    --
    -- Miscellaneous AVR drivers.
    --
    {
        name = "nutdev_hd44780_avr",
        brief = "HD44780 Driver (AVR)",
        requires = { "HW_MCU_AVR" },
        provides = { "DEV_FILE", "DEV_WRITE" },
        sources = { "hd44780.c" },
        options = 
        {
            {
                macro = "LCD_SHORT_DELAY",
                brief = "Short delay",
                description = "Number of milliseconds",
                type = "integer",
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "LCD_LONG_DELAY",
                brief = "Long delay",
                description = "Number of milliseconds",
                type = "integer",
                file = "include/cfg/arch/avr.h"
            }
        }
    },
    {
        name = "nutdev_ide_avr",
        brief = "ATA/IDE driver (AVR)",
        description = "Mass storage driver.",
        requires = { "HW_MCU_AVR" },
        sources = { "ide.c" }
    },
    {
        name = "nutdev_ir_avr",
        brief = "IR Driver (AVR)",
        requires = { "HW_MCU_AVR", "NUT_EVENT" },
        description = "Infrared remote control for Sony protocol.",
        sources = { "ir.c", "irsony.c" }
    },
    {
        name = "nutdev_pcmcia",
        brief = "PCMCIA Driver (AVR)",
        sources = { "pcmcia.c" },
        requires = { "HW_MCU_AVR" }
    },
    {
        name = "nutdev_spidigio_avr",
        brief = "Shift Register I/O (AVR)",
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
        sources = { "spidigio.c" },
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
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "SPIDIGIO_SOUT_PORT",
                brief = "Shift Output Port",
                description = "Port register name of serial data output.",
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "SPIDIGIO_SIN_BIT",
                brief = "Shift Input Bit",
                description = "Bit number of the serial data input.\n"..
                              "Coconut uses bit 6 of PIND.",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "SPIDIGIO_SIN_PIN",
                brief = "Shift Input Port",
                description = "Port register name of serial data input.",
                type = "enumerated",
                choices = avr_pin_choice,
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "SPIDIGIO_SCLK_BIT",
                brief = "Clock Output Bit",
                description = "Bit number of the serial clock output.\n"..
                              "Coconut uses bit 7 of PORTD.",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "SPIDIGIO_SCLK_PORT",
                brief = "Clock Output Port",
                description = "Port register name of serial clock output.",
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "SPIDIGIO_LDI_BIT",
                brief = "Input Latch Bit",
                description = "Bit number of the input latch output.\n"..
                              "Coconut uses bit 7 of PORTB.",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "SPIDIGIO_LDI_PORT",
                brief = "Input Latch Port",
                description = "Port register name of the input latch output.",
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "SPIDIGIO_LDO_BIT",
                brief = "Output Latch Bit",
                description = "Bit number of the output latch signal, which is "..
                              "connected to the strobe input of the UCN5841A\n"..
                              "Coconut uses bit 5 of PORTB.",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "include/cfg/arch/avr.h"
            },
            {
                macro = "SPIDIGIO_LDO_PORT",
                brief = "Output Latch Port",
                description = "Port register name of the output latch output.",
                type = "enumerated",
                choices = avr_port_choice,
                file = "include/cfg/arch/avr.h"
            },
        }
    },
    {
        name = "nutdev_twif_avr",
        brief = "TWI Driver (AVR)",
        requires = { "HW_MCU_AVR", "NUT_EVENT" },
        sources = { "twif.c" }
    },
    {
        name = "nutdev_vs1001k_avr",
        brief = "VS1001 Driver (AVR)",
        requires = { "NUT_SEGBUF", "HW_MCU_AVR" },
        sources = { "vs1001k.c" }
    },

    --
    -- AT91 drivers
    --
    {
        name = "nutdev_debug_at91",
        brief = "UART Debug Output (AT91)",
        requires = { "HW_UART_AT91" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "debug_at91.c" }
    },
    {
        name = "nutdev_debug_gba",
        brief = "LCD Debug Output (GBA)",
        requires = { "HW_LCD_GBA" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "debug_gba.c" }
    },

    --
    -- H8/300 drivers
    --
    {
        name = "nutdev_scih8dbg",
        brief = "SCI Debug Output (H8300)",
        requires = { "HW_MCU_H8300" },
        sources = { "scih8dbg.c" }
    },
    {
        name = "nutdev_scih8",
        brief = "UART driver (H8300)",
        requires = { "HW_UART_H8300" },
        provides = { "DEV_FILE", "DEV_READ", "DEV_WRITE" },
        sources = { "scih8.c", "scih8devs.c" }
    },
    {
        name = "nutdev_rtl8019as_h8",
        brief = "RTL8019AS Driver (H8300)",
        requires = { "HW_MCU_H8300", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "nicrtl_h8.c" }
    },
    {
        name = "nutdev_mweeprom",
        brief = "EEPROM Support (H8300)",
        requires = { "HW_MCU_H8300" },
        sources = { "mweeprom.c" }
    },

    --
    -- S3C45 drivers
    --
    {
        name = "nutdev_uart_s3c4510b_dbg",
        brief = "S3C4510B Debug Output (S3C45)",
        requires = { "HW_UART_S3C45" },
        sources = { "uart_s3c4510b_dbg.c" }
    },
    {
        name = "nutdev_s3c4510b_console",
        brief = "UART driver (S3C45)",
        requires = { "HW_UART_S3C45" },
        provides = { "DEV_FILE", "DEV_READ", "DEV_WRITE" },
        sources = { "s3c4510b_console.c" }
    },

    --
    -- LINUX emulation drivers.
    --
    {
        name = "nutdev_unix_devs",
        brief = "Device Emulation (Linux)",
        requires = { "HW_EMU_LINUX", "NUT_EVENT" },
        sources = { "unix_devs.c" }
    },
    {
        name = "nutdev_ueeprom",
        brief = "EEPROM Support (Linux)",
        provides = { "HW_NVMEM" },
        requires = { "HW_EMU_LINUX" },
        sources = { "ueeprom.c" }
    },

    --
    -- General device helper routines.
    --
    {
        name = "nutdev_ihndlr_avr",
        brief = "Interrupt Handler",
        description = "Currently used for all MCUs except AVR.",
        sources = { "irqreg.c" },
    },
    {
        name = "nutdev_usart",
        brief = "USART Driver Framework",
        description = "Generic USART driver framework.",
        requires = { "CRT_HEAPMEM", "DEV_UART_SPECIFIC" },
        provides = { "DEV_UART_GENERIC", "DEV_FILE", "DEV_READ", "DEV_WRITE" },
        sources = { "usart.c" },
    },
    {
        name = "nutdev_chat",
        brief = "UART Chat",
        description = "Executes a conversational exchange with a serial device.",
        requires = { "CRT_HEAPMEM", "DEV_UART", "NUT_TIMER" },
        provides = { "UART_CHAT" },
        sources =  { "chat.c" }
    },
    {
        name = "nutdev_term",
        brief = "Terminal Emulation",
        requires = { "CRT_HEAPMEM" },
        sources = { "term.c" }
    },
    {
        name = "nutdev_netbuf",
        brief = "Network Buffers",
        provides = { "DEV_NETBUF" },
        sources = { "netbuf.c" }
    },

    --
    -- Disabled components.
    --
    {
        name = "nutdev_spiflash_avr",
        brief = "SPI Flashing (AVR)",
        sources = { "spiflash.c" },
        requires = { "NOT_AVAILABLE" }
    },
}
