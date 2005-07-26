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
-- Revision 1.19  2005/07/26 15:41:06  haraldkipp
-- All target dependent code is has been moved to a new library named
-- libnutarch. Each platform got its own script.
--
-- Revision 1.18  2005/05/27 13:51:07  olereinhardt
-- Added new Display sizes for hd44780 and hd44780_bus
-- Ports used for display connection still need's to be added.
--
-- Revision 1.17  2005/04/05 17:44:56  haraldkipp
-- Made stack space configurable.
--
-- Revision 1.16  2005/02/21 00:56:59  hwmaier
-- New CAN int vectors ivect35.c and ivect36 added, removed "makedefs" entry for RTL_IRQ_RISING_EDGE.
--
-- Revision 1.15  2005/02/19 22:47:54  hwmaier
-- no message
--
-- Revision 1.14  2005/02/07 19:05:25  haraldkipp
-- ATmega 103 compile errors fixed
--
-- Revision 1.13  2005/02/02 19:46:53  haraldkipp
-- Port configuration was completely broken, because no AVRPORT values
-- had been defined for the preprocessor. To fix this without modifying
-- too many sourcefiles we change the name of AVR port config file and
-- include this new file in the old avr.h.
--
-- Revision 1.12  2005/01/22 19:22:42  haraldkipp
-- Changed AVR port configuration names from PORTx to AVRPORTx.
-- Removed uartspi devices.
--
-- Revision 1.11  2004/12/17 15:31:28  haraldkipp
-- Support of rising edge interrupts for hardware w/o inverter gate.
-- Fixed compilation issue for hardware with RTL reset port.
-- Thanks to FOCUS Software Engineering Pty Ltd.
--
-- Revision 1.10  2004/11/24 14:48:34  haraldkipp
-- crt/crt.nut
--
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
    -- General device helper routines.
    --
    {
        name = "nutdev_ihndlr",
        brief = "Interrupt Handler",
        description = "Interrupt registration and distribution.",
        sources = { "ihndlr.c" },
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
        description = "Executes a conversational exchange with a serial device."..
                      "Typically used for modem dial commands and login scripts.",
        requires = { "CRT_HEAPMEM", "DEV_UART", "NUT_TIMER" },
        provides = { "UART_CHAT" },
        sources =  { "chat.c" },
        options =
        {
            {
                macro = "CHAT_MAX_ABORTS",
                brief = "Max. Number of Aborts",
                description = "Default is 10.",
                file = "include/cfg/chat.h"
            },
            {
                macro = "CHAT_MAX_REPORT_SIZE",
                brief = "Max. Size of Reports",
                description = "Default is 32",
                file = "include/cfg/chat.h"
            },
            {
                macro = "CHAT_DEFAULT_TIMEOUT",
                brief = "Default Timeout",
                description = "Specify the number of seconds. "..
                              "Default is 45",
                file = "include/cfg/chat.h"
            }
        }
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
    -- WAN Drivers.
    --
    {
        name = "nutdev_ppp",
        brief = "PPP Driver",
        requires = { "PROTO_HDLC", "NUT_TIMER", "PROTO_HDLC" },
        provides = { "NET_PHY" },
        sources = { "ppp.c" }
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
