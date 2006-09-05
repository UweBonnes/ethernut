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

-- Operating system functions
--
-- $Log$
-- Revision 1.8  2006/09/05 12:35:39  haraldkipp
-- DHCP servers may probe an IP/MAC relationship by sending an
-- ICMP request. This triggered the Nut/Net ARP method and
-- terminated the DHCP client, leaving the system with default
-- configurations of the network mask (255.255.255.0) and
-- default gateway (none). The rarely used ARP method is now
-- disabled by default.
--
-- Revision 1.7  2006/08/01 07:41:01  haraldkipp
-- New functions ether_aton() and ether_ntoa() added. They convert the ASCII
-- representation of an Ethernet MAC address to its binary form and vice versa.
--
-- Revision 1.6  2006/01/23 17:30:25  haraldkipp
-- Configurable offset into non-volatile memory for network parameters.
--
-- Revision 1.5  2005/04/05 17:44:56  haraldkipp
-- Made stack space configurable.
--
-- Revision 1.4  2005/02/04 15:05:59  haraldkipp
-- ARP configuration added.
--
-- Revision 1.3  2005/01/13 18:47:31  haraldkipp
-- Network dependencies added.
--
-- Revision 1.2  2004/08/18 13:46:09  haraldkipp
-- Fine with avr-gcc
--
-- Revision 1.1  2004/06/07 16:38:43  haraldkipp
-- First release
--
--

nutnet =
{
    {
        name = "nutnet_tcp",
        brief = "TCP",
        requires = { "NET_IP", "NET_ICMP", "NUT_EVENT" },
        provides = { "NET_TCP", "DEV_READ", "DEV_WRITE" },
        sources = 
        { 
            "tcpin.c",
            "tcpout.c",
            "tcpsm.c",
            "tcpsock.c",
            "tcputil.c"
        },
        options = 
        {
            {
                macro = "NUT_THREAD_TCPSMSTACK",
                brief = "State Machine Stack",
                description = "Number of bytes to be allocated for the stack of the TCP state machine thread.",
                flavor = "booldata",
                file = "include/cfg/tcp.h"
            }
        }
    },
    {
        name = "nutnet_udp",
        brief = "UDP",
        requires = { "NET_IP", "NUT_EVENT" },
        provides = { "NET_UDP" },
        sources = 
        { 
            "udpin.c",
            "udpout.c",
            "udpsock.c"
        }
    },
    {
        name = "nutnet_ip",
        brief = "IP",
        requires = { "NET_LINK" },
        provides = { "NET_IP" },
        sources = 
        { 
            "ipcsum.c",
            "ipin.c",
            "ipout.c",
            "route.c"
        },
        options = 
        {
            {
                macro = "NUTIPCONF_ICMP_ARPMETHOD",
                brief = "Enable ARM Method",
                description = "Allow IP configuration using ARP method. ",
                flavor = "booldata",
                file = "include/cfg/ip.h"
            },
        }        
    },
    {
        name = "nutnet_icmp",
        brief = "ICMP",
        requires = { "NET_LINK" },
        provides = { "NET_ICMP" },
        sources = 
        { 
            "icmpin.c",
            "icmpout.c"
        }
    },
    {
        name = "nutnet_inet",
        brief = "INET",
        requires = { "NET_LINK" },
        provides = { "NET_INET" },
        sources = 
        { 
            "inet.c"
        }
    },
    {
        name = "nutnet_arp",
        brief = "ARP",
        description = "Address Resolution Protocol, translates a "..
                      "32-bit IP address into a 48-bit Ethernet address.",
        requires = { "NUT_EVENT", "NET_PHY" },
        provides = { "NET_ARP" },
        sources = 
        { 
            "arpcache.c",
            "arpin.c",
            "arpout.c"
        },
        options = 
        {
            {
                macro = "MAX_ARPAGE",
                brief = "Max. ARP Age",
                description = "Maximum age of an entry in the ARP cache in minutes. "..
                              "Default is 9 minutes.",
                flavor = "booldata",
                file = "include/cfg/arp.h"
            },
            {
                macro = "MAX_ARPREQUESTS",
                brief = "Max. ARP Requests",
                description = "Maximum number of ARP requests generated per query. "..
                              "Default is 4.",
                flavor = "booldata",
                file = "include/cfg/arp.h"
            },
            {
                macro = "MIN_ARPWAIT",
                brief = "Min. ARP Wait",
                description = "Minimum number of milliseconds to wait before sending "..
                              "out a new ARP request. Default is 500 milliseconds.",
                flavor = "booldata",
                file = "include/cfg/arp.h"
            },
        }
        
    },
    {
        name = "nutnet_ethernet",
        brief = "Ethernet",
        requires = { "NET_ARP" },
        provides = { "NET_LINK" },
        sources = 
        { 
            "ethin.c",
            "ethout.c",
            "ether_addr.c"
        }
    },
    {
        name = "nutnet_ppp",
        brief = "PPP",
        requires = { 
            "NET_PPPAUTH", "NUT_EVENT", "PROTO_HDLC", "DEV_FILE", "DEV_READ", "DEV_WRITE" 
        },
        provides = { "NET_PPP", "NET_LINK" },
        sources = { 
            "pppin.c",
            "pppout.c",
            "pppsm.c",
            "ipcpin.c",
            "ipcpout.c",
            "lcpin.c", 
            "lcpout.c"
        },
        options = 
        {
            {
                macro = "NUT_THREAD_PPPSMSTACK",
                brief = "State Machine Stack",
                description = "Number of bytes to be allocated for the stack of the PPP state machine thread.",
                flavor = "booldata",
                file = "include/cfg/ppp.h"
            }
        }
    },
    {
        name = "nutnet_pap",
        brief = "PPP PAP",
        requires = { "NET_PPP" },
        provides = { "NET_PPPAUTH" },
        sources = { "papin.c", "papout.c" }
    },
    {
        name = "nutnet_ifconfig",
        brief = "Network interface",
        requires = { "NUT_EVENT", "NET_LINK" },
        sources = { "ifconfig.c" }
    },
    {
        name = "nutnet_conf",
        brief = "Network Configuration",
        description = "Initial configuration settings are stored in non-volatile memory.",
        requires = { "DEV_NVMEM" },
        provides = { "NET_PARMS" },
        sources = { "confnet.c" },
        options =
        {
            {
                macro = "CONFNET_EE_OFFSET",
                brief = "Location",
                description = "This is the first non-volatile memory address, where Nut/OS "..
                              "expects its configuration",
                default = "64",
                type = "integer",
                file = "include/cfg/eeprom.h"
            }
        }
    },
    {
        name = "nutnet_debug",
        brief = "Network debug",
        requires = { "DEV_WRITE", "NET_LINK" },
        sources = { "netdebug.c" }
    },
    {
        name = "nutnet_debug_ppp",
        brief = "PPP debug",
        requires = { "DEV_WRITE", "NET_PPP" },
        sources = { "pppdebug.c" }
    }
}
