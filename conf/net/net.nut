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
-- Revision 1.1  2004/06/07 16:38:43  haraldkipp
-- First release
--
--

nutnet =
{
    {
        name = "nutnet_arp",
        sources = 
        { 
            "arpcache.c",
            "arpin.c",
            "arpout.c"
        }
    },
    {
        name = "nutnet_conf",
        sources = 
        { 
            "confnet.c"
        }
    },
    {
        name = "nutnet_ethernet",
        sources = 
        { 
            "ethin.c",
            "ethout.c"
        }
    },
    {
        name = "nutnet_icmp",
        sources = 
        { 
            "icmpin.c",
            "icmpout.c"
        }
    },
    {
        name = "nutnet_ifconfig",
        sources = 
        { 
            "ifconfig.c"
        }
    },
    {
        name = "nutnet_inet",
        sources = 
        { 
            "inet.c"
        }
    },
    {
        name = "nutnet_ipcp",
        sources = 
        { 
            "ipcpin.c",
            "ipcpout.c"
        }
    },
    {
        name = "nutnet_ip",
        sources = 
        { 
            "ipcsum.c",
            "ipin.c",
            "ipout.c",
            "route.c"
        }
    },
    {
        name = "nutnet_lcp",
        sources = 
        { 
            "lcpin.c",
            "lcpout.c"
        }
    },
    {
        name = "nutnet_pap",
        sources = 
        { 
            "papin.c",
            "papout.c"
        }
    },
    {
        name = "nutnet_debug",
        sources = 
        { 
            "netdebug.c"
        }
    },
    {
        name = "nutnet_debug_ppp",
        sources = 
        { 
            "pppdebug.c"
        }
    },
    {
        name = "nutnet_ppp",
        sources = 
        { 
            "pppin.c",
            "pppout.c",
            "pppsm.c"
        }
    },
    {
        name = "nutnet_tcp",
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
        sources = 
        { 
            "udpin.c",
            "udpout.c",
            "udpsock.c"
        }
    }
}
