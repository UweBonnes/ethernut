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
        provides = { "NET_TCP" },
        sources = 
        { 
            "tcpin.c",
            "tcpout.c",
            "tcpsm.c",
            "tcpsock.c",
            "tcputil.c"
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
        requires = { "NUT_EVENT" },
        provides = { "NET_ARP" },
        sources = 
        { 
            "arpcache.c",
            "arpin.c",
            "arpout.c"
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
            "ethout.c"
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
        requires = { "NUT_EVENT" },
        sources = { "ifconfig.c" }
    },
    {
        name = "nutnet_conf",
        brief = "Network parameters",
        requires = { "HW_NVMEM" },
        provides = { "NET_PARMS" },
        sources = { "confnet.c" }
    },
    {
        name = "nutnet_debug",
        brief = "Network debug",
        requires = { "DEV_WRITE" },
        sources = { "netdebug.c" }
    },
    {
        name = "nutnet_debug_ppp",
        brief = "PPP debug",
        requires = { "DEV_WRITE" },
        sources = { "pppdebug.c" }
    }
}
