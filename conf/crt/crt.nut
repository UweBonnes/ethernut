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

nutcrt =
{
    --
    -- Low level I/O
    --
    {
        name = "nutcrt_file",
        brief = "Low level file I/O",
        requires = { "DEV_FILE" },
        provides = { "CRT_FILE" },
        sources = 
        { 
            "close.c",
            "clrerr.c",
            "ioctl.c",
            "open.c",
        }
    },
    {
        name = "nutcrt_read",
        brief = "Low level file read",
        requires = { "DEV_READ", "CRT_FILE" },
        provides = { "CRT_READ" },
        sources = 
        { 
            "getf.c",
            "getff.c",
            "read.c",
        }
    },
    {
        name = "nutcrt_write",
        brief = "Low level file write",
        requires = { "DEV_WRITE", "CRT_FILE" },
        provides = { "CRT_WRITE" },
        sources = 
        { 
            "putf.c",
            "putff.c",
            "write.c",
            "write_p.c"
        }
    },

    --
    -- Stream I/O
    --
    {
        name = "nutcrt_stream",
        brief = "File streams",
        provides = { "CRT_STREAM" },
        sources = 
        { 
            "fclose.c",
            "fcloseall.c",
            "fdopen.c",
            "feof.c",
            "ferror.c",
            "fflush.c",
            "filelength.c",
            "fileno.c",
            "flushall.c",
            "fmode.c",
            "fopen.c",
            "fpurge.c",
            "freopen.c",
            "fseek.c",
            "ftell.c"
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
        name = "nutcrt_istream",
        brief = "File stream input",
        provides = { "CRT_STREAM_READ" },
        sources = 
        { 
            "fgetc.c",
            "fgets.c",
            "fread.c",
            "fscanf.c",
            "fscanf_p.c",
            "getc.c",
            "getchar.c",
            "gets.c",
            "scanf.c",
            "scanf_p.c",
            "ungetc.c",
            "vfscanf.c",
            "vfscanf_p.c"
        }
    },
    {
        name = "nutcrt_ostream",
        brief = "File stream output",
        provides = { "CRT_STREAM_WRITE" },
        sources = 
        { 
            "fprintf.c",
            "fprintf_p.c",
            "fputc.c",
            "fputs.c",
            "fputs_p.c",
            "fwrite.c",
            "fwrite_p.c",
            "printf.c",
            "printf_p.c",
            "putc.c",
            "putchar.c",
            "puts.c",
            "puts_p.c",
            "vfprintf.c",
            "vfprintf_p.c",
        }
    },

    --
    -- String I/O
    --
    {
        name = "nutcrt_fstrio",
        brief = "Formatted string I/O",
        sources = 
        { 
            "sprintf.c",
            "sprintf_p.c",
            "sscanf.c",
            "sscanf_p.c",
            "vsprintf.c",
            "vsprintf_p.c",
            "vsscanf.c",
            "vsscanf_p.c"
        }
    },

    --
    -- Date and time
    --
    {
        name = "nutcrt_time",
        brief = "Time and date",
        provides = { "CRT_DATETIME" },
        sources = 
        { 
            "gmtime.c",
            "localtim.c",
            "mktime.c",
            "time.c",
            "tzset.c"
        }
    },

    --
    -- Date and time
    --
    {
        name = "nutcrt_errno",
        brief = "Global errno",
        sources = { "errno.c" }
    },

    --
    -- Heap memory
    --
    {
        name = "nutcrt_malloc",
        brief = "Heap memory",
        requires = { "NUT_HEAPMEM" },
        provides = { "CRT_HEAPMEM" },
        sources = { "malloc.c" }
    },

    --
    -- Misc
    --
    {
        name = "nutcrt_strtok_r",
        brief = "Recursive strtok",
        requires = { "TOOL_ICC" },
        sources = { "strtok_r.c" }
    }
}
