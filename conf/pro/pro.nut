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
        brief = "DHCP/BOOTP client",
        requires = { "NET_UDP", "NUT_EVENT" },
        sources = 
        { 
            "dhcpc.c"
        }
    },
    {
        name = "nutpro_resolv",
        brief = "DNS client API",
        requires = { "NET_UDP" },
        sources = 
        { 
            "resolv.c"
        }
    },
    {
        name = "nutpro_ftpd",
        brief = "FTP server API",
        description = "File transfer protocol. Not implemented.",
        requires = 
        {
            "NOT_AVAILABLE",
            "NET_TCP",
            "NET_UDP",
            "CRT_STREAM_READ", 
            "CRT_STREAM_WRITE", 
            "NUT_FS", 
            "NUT_FS_READ", 
            "NUT_FS_WRITE" 
        }
    },
    {
        name = "nutpro_httpd",
        brief = "HTTP server API",
        description = "Webserver helper routines. Provides simple authorization "..
                      "and registration of C functions as CGI routines",
        requires = { "NET_TCP", "CRT_STREAM_READ", "NUT_FS", "NUT_FS_READ" },
        sources = 
        { 
            "auth.c",
            "cgi.c",
            "dencode.c",
            "httpd.c"
        }
    },
    {
        name = "nutpro_snmp",
        brief = "SNMP agent",
        description = "Simple network management protocol. Not implemented.",
        requires = { "NOT_AVAILABLE", "NET_UDP" },
        sources =  { "snmp.c" }
    },
    {
        name = "nutpro_sntp",
        brief = "SNTP client API",
        description = "Simple network time protocol.",
        requires = { "NET_UDP" },
        provides = { "PRO_SNTP" },
        sources =  { "sntp.c" }
    },
    {
        name = "nutpro_smtpc",
        brief = "SMTP client API",
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
    }
}
