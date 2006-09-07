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
-- Revision 1.8  2006/09/07 09:06:17  haraldkipp
-- Discovery service added.
--
-- Revision 1.7  2005/08/05 11:26:01  olereinhardt
-- Added asp.c, ssi.c
--
-- Revision 1.6  2005/04/05 17:44:56  haraldkipp
-- Made stack space configurable.
--
-- Revision 1.5  2005/02/05 20:41:13  haraldkipp
-- Wins and FTP added.
--
-- Revision 1.4  2005/02/02 15:53:15  haraldkipp
-- DHCP configuration added
--
-- Revision 1.3  2004/09/19 11:18:44  haraldkipp
-- Syslog client added
--
-- Revision 1.2  2004/08/18 13:46:10  haraldkipp
-- Fine with avr-gcc
--
-- Revision 1.1  2004/06/07 16:38:43  haraldkipp
-- First release
--
--

nutpro =
{
    {
        name = "nutpro_dhcpc",
        brief = "DHCP/BOOTP Client",
        requires = { "NET_UDP", "NUT_EVENT" },
        sources = 
        { 
            "dhcpc.c"
        },
        options = 
        {
            {
                macro = "DHCP_SERVERPORT",
                brief = "Server Port",
                description = "UDP port of the DHCP server. Default is 67.", 
                flavor = "booldata",
                file = "include/cfg/dhcp.h"
            },
            {
                macro = "DHCP_CLIENTPORT",
                brief = "Client Port",
                description = "UDP port of the DHCP client. Default is 68.", 
                flavor = "booldata",
                file = "include/cfg/dhcp.h"
            },
            {
                macro = "DHCP_BROADCAST_FLAG",
                brief = "Broadcast Flag",
                description = "If enabled, the client will set the broadcast flag in all "..
                              "outgoing messages. This is not required, because Nut/Net is "..
                              "able to receive UPD datagrams without configuring the "..
                              "interface.",
                flavor = "boolean",
                file = "include/cfg/dhcp.h"
            },
            {
                macro = "MIN_DHCP_MSGSIZE",
                brief = "Min. Message Size",
                description = "Used to maintain BOOTP compatibility of outgoing messages. "..
                              "Default is 300 octets.",
                flavor = "booldata",
                file = "include/cfg/dhcp.h"
            },
            {
                macro = "MAX_DHCP_MSGSIZE",
                brief = "Max. Message Size",
                description = "RFC 2131 demands, that a DHCP client must be prepared to receive DHCP "..
                              "messages with an options field length of at least 312 octets. This "..
                              "implies that we must be able to accept messages of up to 576 octets (default)",
                flavor = "booldata",
                file = "include/cfg/dhcp.h"
            },
            {
                macro = "MAX_DHCP_BUFSIZE",
                brief = "UDP Buffer Size",
                description = "By default this value is set to 1728 and allows the client to concurrently "..
                              "receive offers from up to 3 DHCP servers. Setting this value to zero will "..
                              "save some heap space by disabling UDP buffering. This is fine for networks "..
                              "with a single DHCP server.",
                flavor = "booldata",
                file = "include/cfg/dhcp.h"
            },
            {
                macro = "MIN_DHCP_WAIT",
                brief = "Min. Wait Time",
                description = "The client will wait this number of milliseconds (default is 4000) before "..
                              "resending a request. The timeout is doubled on each retry up to the "..
                              "number of milliseconds specified by MAX_DHCP_WAIT.",
                flavor = "booldata",
                file = "include/cfg/dhcp.h"
            },
            {
                macro = "MAX_DHCP_WAIT",
                brief = "Max. Wait Time",
                description = "With each retry the client will double the number of milliseconds to wait "..
                              "for a response from the server. However, the maximum time can be limited "..
                              "by this item, which defaults to 64000.",
                flavor = "booldata",
                file = "include/cfg/dhcp.h"
            },
            {
                macro = "MAX_DCHP_RETRIES",
                brief = "Max. Request Retries",
                description = "The client will give up after resending this number of requests without "..
                              "receiving a response from the server. Default is 3.",
                flavor = "booldata",
                file = "include/cfg/dhcp.h"
            },
            {
                macro = "MAX_DCHP_RELEASE_RETRIES",
                brief = "Max. Release Retries",
                description = "RFC 2131 doesn't specify a server response to release messages from "..
                              "the client. If the message gets lost, then the lease isn't released. "..
                              "Setting a value greater than zero (default) will cause the client "..
                              "to resend a release message for the given number of times or until "..
                              "the server sends a response.",
                flavor = "booldata",
                file = "include/cfg/dhcp.h"
            },
            {
                macro = "DHCP_DEFAULT_LEASE",
                brief = "Default Lease",
                description = "If the server doesn't provide a maximum lease time, the client "..
                              "uses the number of seconds given by this value. Default is 43200.",
                flavor = "booldata",
                file = "include/cfg/dhcp.h"
            },
            {
                macro = "NUT_THREAD_DHCPSTACK",
                brief = "Client Thread Stack",
                description = "Number of bytes to be allocated for the stack of the DHCP client thread.",
                flavor = "booldata",
                file = "include/cfg/dhcp.h"
            },
        }

    },
    {
        name = "nutpro_resolv",
        brief = "DNS Client API",
        requires = { "NET_UDP" },
        sources = 
        { 
            "resolv.c"
        }
    },
    {
        name = "nutpro_ftpd",
        brief = "FTP Server API",
        description = "File transfer protocol server.",
        requires = 
        {
            "NET_TCP",
            "NET_UDP",
            "CRT_STREAM_READ", 
            "CRT_STREAM_WRITE", 
            "NUT_FS_DIR", 
            "NUT_FS_READ", 
            "NUT_FS_WRITE" 
        },
        sources = { "ftpd.c" }
    },
    {
        name = "nutpro_httpd",
        brief = "HTTP Server API",
        description = "Webserver helper routines. Provides simple authorization "..
                      "and registration of C functions as CGI routines",
        requires = { "NET_TCP", "CRT_STREAM_READ", "NUT_FS", "NUT_FS_READ" },
        sources = 
        { 
            "auth.c",
            "cgi.c",
            "dencode.c",
            "discover.c",
            "httpd.c",
            "asp.c",
            "ssi.c"
        }
    },
    {
        name = "nutpro_snmp",
        brief = "SNMP Agent",
        description = "Simple network management protocol. Not implemented.",
        requires = { "NOT_AVAILABLE", "NET_UDP" },
        sources =  { "snmp.c" }
    },
    {
        name = "nutpro_sntp",
        brief = "SNTP Client API",
        description = "Simple network time protocol.",
        requires = { "NET_UDP" },
        provides = { "PRO_SNTP" },
        sources =  { "sntp.c" },
        options = 
        {
            {
                macro = "NUT_THREAD_SNTPSTACK",
                brief = "Client Thread Stack",
                description = "Number of bytes to be allocated for the stack of the SNTP client thread.",
                flavor = "booldata",
                file = "include/cfg/sntp.h"
            }
        }
    },
    {
        name = "nutpro_smtpc",
        brief = "SMTP Client API",
        description = "Simple mail transfer protocol. Not implemented.",
        requires = 
        {
            "NOT_AVAILABLE",
            "NET_TCP",
            "CRT_STREAM_READ", 
            "CRT_STREAM_WRITE"
        }
    },
    {
        name = "nutpro_syslog",
        brief = "Syslog Client API",
        description = "Logs system and debug information to a remote server.",
        requires = { "PRO_SNTP", "NET_UDP" },
        sources =  { "syslog.c" },
        options = 
        {
            {
                macro = "SYSLOG_PERROR_ONLY",
                brief = "Disable Network",
                description = "UDP references are excluded.", 
                flavor = "boolean",
                file = "include/cfg/syslog.h"
            },
            {
                macro = "SYSLOG_MAXBUF",
                brief = "Output buffer size",
                description = "This is a critical value. If set too low, then "..
                              "syslog may crash with long messages. Default is 256.",
                flavor = "booldata",
                file = "include/cfg/syslog.h"
            }
        }
    },
    {
        name = "nutpro_wins",
        brief = "NetBIOS WINS Responder",
        description = "Netbios WINS (RFC 1002) Name Query Response.\n\n"..
                      "Only query request client routine sending/positive name query response "..
                      "receiving is implemented.\n\n"..
                      "When the NetBIOS name 1uery request UDP datagram is on the ethernet "..
                      "network, asking 'Who is name?', NutWinsNameQuery answers with the "..
                      "specified 'ipaddr' Ethernut IP address.",
        requires =  { "NET_UDP" },
        sources =  { "wins.c" }
    }    
}
