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
-- Revision 1.1  2004/06/07 16:35:53  haraldkipp
-- First release
--
--

nutdev =
{
    {
        name = "nutdev_ahdlc_avr",
        sources = 
        { 
            "ahdlc0.c",
            "ahdlc1.c",
            "ahdlcavr.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_chat",
        sources = 
        { 
            "chat.c"
        }
    },
    {
        name = "nutdev_cs8900_avr",
        sources = 
        { 
            "cs8900.c",
            "eth0cs.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_debug_avr",
        sources = 
        { 
            "debug0.c",
            "debug1.c"
        },
        requires =
        {
            "AVR_UART"
        }
    },
    {
        name = "nutdev_debug_at91",
        sources = 
        { 
            "debug_at91.c"
        },
        requires =
        {
            "AT91_UART"
        }
    },
    {
        name = "nutdev_rtl8019as_avr",
        sources = 
        { 
            "nicrtl.c",
            "eth0rtl.c"
        },
        requires =
        {
            "AVR_MCU"
        },
        provides =
        {
            "PHYNET_LAYER"
        },
        options = 
        {
            {
                macro = "RTL_RESET_BIT",
                brief = "Ethernet reset bit",
                description = "Bit number of the Ethernet controller reset output. "..
                              "Should be disabled when the hardware doesn't support "..
                              "this function.",
                flavor = "booldata",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "cfg/portdefs.h"
            },
            {
                macro = "RTL_RESET_PORT",
                brief = "Ethernet reset port",
                description = "Port register name of the Ethernet controller reset output.",
                active_if = "RTL_RESET_BIT",
                type = "enumerated",
                choices = avr_port_choice,
                file = "cfg/portdefs.h"
            },
            {
                macro = "RTL_RESET_DDR",
                brief = "Ethernet reset DDR",
                description = "Data direction register name of the Ethernet controller reset output.",
                active_if = "RTL_RESET_BIT",
                type = "enumerated",
                choices = avr_ddr_choice,
                file = "cfg/portdefs.h"
            },
            {
                macro = "RTL_SIGNAL_BIT",
                brief = "Ethernet IRQ bit",
                description = "Bit number of the Ethernet controller interrupt input.",
                type = "enumerated",
                choices = avr_bit_choice,
                file = "cfg/portdefs.h"
            },
            {
                macro = "RTL_SIGNAL_PORT",
                brief = "Ethernet IRQ port",
                description = "Port register name of the Ethernet controller interrupt input.",
                type = "enumerated",
                choices = avr_port_choice,
                file = "cfg/portdefs.h"
            },
            {
                macro = "RTL_SIGNAL_DDR",
                brief = "Ethernet IRQ DDR",
                description = "Data direction register name of the Ethernet controller interrupt input.",
                type = "enumerated",
                choices = avr_ddr_choice,
                file = "cfg/portdefs.h"
            }
        }
    },
    {
        name = "nutdev_hd44780_avr",
        sources = 
        { 
            "hd44780.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_ide_avr",
        sources = 
        { 
            "ide.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_ir_avr",
        sources = 
        { 
            "ir.c",
            "irsony.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_irq_avr",
        sources = 
        { 
            "irqreg.c",
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
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_irq_arm",
        requires = { "ARM7_MCU" },
        sources = 
        { 
            "arm_irqreg.c"
        }
    },
    {
        name = "nutdev_irq_h8",
        requires = { "H8300_MCU" },
        sources = 
        { 
            "h8_irqreg.c"
        }
    },
    {
        name = "nutdev_lanc111_avr",
        sources = 
        { 
            "lanc111.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_irq_m68k",
        sources = 
        { 
            "m68k_irqreg.c"
        }
    },
    {
        name = "nutdev_rtl8019as_h8",
        requires = { "H8300_MCU" },
        sources = 
        { 
            "nicrtl_h8.c"
        }
    },
    {
        name = "nutdev_netbuf",
        sources = 
        { 
            "netbuf.c"
        }
    },
    {
        name = "nutdev_pcmcia",
        sources = 
        { 
            "pcmcia.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_ppp",
        sources = 
        { 
            "ppp.c"
        }
    },
    {
        name = "nutdev_s3c4510b_console",
        sources = 
        { 
            "s3c4510b_console.c"
        }
    },
    {
        name = "nutdev_s3c4510b_irqreg",
        sources = 
        { 
            "s3c4510b_irqreg.c"
        }
    },
    {
        name = "nutdev_scih8",
        requires = { "H8300_MCU" },
        sources = 
        { 
            "scih8.c",
            "scih8devs.c"
        }
    },
    {
        name = "nutdev_scih8dbg",
        requires = { "H8300_MCU" },
        sources = 
        { 
            "scih8dbg.c"
        }
    },
    {
        name = "nutdev_ihndlr_avr",
        sources = 
        { 
            "ihndlr.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_mweeprom",
        sources = 
        { 
            "mweeprom.c"
        }
    },
    {
        name = "nutdev_spidigio_avr",
        sources = 
        { 
            "spidigio.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_spiflash_avr",
        sources = 
        { 
            "spiflash.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_term",
        sources = 
        { 
            "term.c"
        }
    },
    {
        name = "nutdev_twif_avr",
        sources = 
        { 
            "twif.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_uart_avr",
        sources = 
        { 
            "uart0.c",
            "uart1.c",
            "uartavr.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_uarts",
        sources = 
        { 
            "uarts.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_uartspi",
        sources = 
        { 
            "uartspi.c"
        },
        requires = { "AVR_MCU" }
    },
    {
        name = "nutdev_uart_s3c4510b_dbg",
        sources = 
        { 
            "uart_s3c4510b_dbg.c"
        }
    },
    {
        name = "nutdev_ueeprom",
        sources = 
        { 
            "ueeprom.c"
        }
    },
    {
        name = "nutdev_unix_devs",
        sources = 
        { 
            "unix_devs.c"
        }
    },
    {
        name = "nutdev_usart_avr",
        brief = "AVR USART device",
        description = "Interrupt driven buffered UART driver.",
        provides = { "UART_READ", "UART_WRITE" },
        requires = { "AVR_MCU" },
        sources = 
        { 
            "usart.c",
            "usart0avr.c",
            "usart1avr.c",
            "usartavr.c"
        },
        options = 
        {
            {
                macro = "UART0_RTS_BIT",
                brief = "UART0 RTS bit",
                description = "Bit number of UART0 RTS handshake output.",
                flavor = "booldata",
                file = "cfg/portdefs.h"
            },
            {
                macro = "UART0_RTS_PORT",
                brief = "UART0 RTS port",
                description = "Port register name of UART0 RTS handshake output.",
                file = "cfg/portdefs.h"
            },
            {
                macro = "UART0_RTS_DDR",
                brief = "UART0 RTS DDR",
                description = "Data direction register name of UART0 RTS handshake output.",
                file = "cfg/portdefs.h"
            }
        }
    },
    {
        name = "nutdev_vs1001k_avr",
        requires = { "AVR_MCU" },
        sources = 
        { 
            "vs1001k.c"
        }
    },
    {
        name = "nutdev_wlan_avr",
        requires = { "AVR_MCU" },
        sources = 
        { 
            "wlan.c",
            "wlandrv.c"
        }
    }
}
