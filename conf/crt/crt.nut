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
-- Revision 1.1  2004/06/07 16:35:53  haraldkipp
-- First release
--
--

nutcrt =
{
    {
        name = "nutcrt_io",
        sources = 
        { 
            "close.c",
            "clrerr.c",
            "getf.c",
            "getff.c",
            "ioctl.c",
            "open.c",
            "putf.c",
            "putff.c",
            "read.c",
            "write.c",
            "write_p.c"
        }
    },
    {
        name = "nutcrt_stdio",
        sources = 
        { 
            "fclose.c",
            "fcloseall.c",
            "fdopen.c",
            "feof.c",
            "ferror.c",
            "fflush.c",
            "fgetc.c",
            "fgets.c",
            "filelength.c",
            "fileno.c",
            "flushall.c",
            "fmode.c",
            "fopen.c",
            "fprintf.c",
            "fprintf_p.c",
            "fpurge.c",
            "fputc.c",
            "fputs.c",
            "fputs_p.c",
            "fread.c",
            "freopen.c",
            "fscanf.c",
            "fscanf_p.c",
            "fseek.c",
            "ftell.c",
            "fwrite.c",
            "fwrite_p.c",
            "getc.c",
            "getchar.c",
            "gets.c",
            "printf.c",
            "printf_p.c",
            "putc.c",
            "putchar.c",
            "puts.c",
            "puts_p.c",
            "scanf.c",
            "scanf_p.c",
            "sprintf.c",
            "sprintf_p.c",
            "sscanf.c",
            "sscanf_p.c",
            "ungetc.c",
            "vfprintf.c",
            "vfprintf_p.c",
            "vfscanf.c",
            "vfscanf_p.c",
            "vsprintf.c",
            "vsprintf_p.c",
            "vsscanf.c",
            "vsscanf_p.c"
        },
        options = 
        {
            {
                macro = "STDIO_FLOATING_POINT",
                brief = "Floating point",
                description = "Enables floating point support for standard input/output.", 
                flavor = "boolean",
                file = "cfg/crt.h"
            }
        }

    },
    {
        name = "nutcrt_time",
        sources = 
        { 
            "gmtime.c",
            "localtim.c",
            "mktime.c",
            "time.c",
            "tzset.c"
        }
    },
    {
        name = "nutcrt_errno",
        sources = 
        { 
            "errno.c"
        }
    },
    {
        name = "nutcrt_h8_irqreq",
        sources = 
        { 
            "h8_irqreg.c"
        }
    },
    {
        name = "nutcrt_malloc",
        sources = 
        { 
            "malloc.c"
        }
    },
    {
        name = "nutcrt_strtok_r",
        sources = 
        { 
            "strtok_r.c"
        }
    }
}
