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
-- Revision 1.3  2004/08/18 13:46:08  haraldkipp
-- Fine with avr-gcc
--
-- Revision 1.2  2004/08/03 15:09:31  haraldkipp
-- Another change of everything
--
-- Revision 1.1  2004/06/07 16:32:45  haraldkipp
-- First release
--
--

nutc =
{
    {
        name = "nutc_stdlib",
        brief = "C stdlib routines",
        description = "This component will provide all stdlib functions "..
                      "required by Nut/OS. Typically these are included "..
                      "in the compiler's runtime library, but may not be "..
                      "available in all environments.",
        requires = { "TOOL_NOLIBC" },
        provides = { "C_STDLIB" },
        sources = 
        { 
            "stdlib/abs.c",
            "stdlib/atoi.c",
            "stdlib/atol.c",
            "stdlib/labs.c",
            "stdlib/strtol.c",
            "stdlib/strtoul.c"
        }
    },
    {
        name = "nutc_heap",
        brief = "C heap routines",
        description = "Although some runtime libraries provide "..
                      "identical routines, we better use our own to avoid "..
                      "memory management conflicts.\nFor historical reasons "..
                      "malloc and free are part of another component.",
        requires = { "NUT_HEAPMEM" },
        provides = { "C_STDLIB" },
        sources = 
        { 
            "stdlib/calloc.c"
        }
    },
    {
        name = "nutc_string",
        brief = "C string routines",
        description = "This component will provide all string functions "..
                      "required by Nut/OS. Typically these are included "..
                      "in the compiler's runtime library, but may not be "..
                      "available in all environments.",
        requires = { "TOOL_NOLIBC" },
        provides = { "C_STRING" },
        sources = 
        { 
            "string/memchr.c",
            "string/memcmp.c",
            "string/memcpy.c",
            "string/memset.c",
            "string/strcat.c",
            "string/strchr.c",
            "string/strcmp.c",
            "string/strcpy.c",
            "string/strdup.c",
            "string/stricmp.c",
            "string/strlen.c",
            "string/strncat.c",
            "string/strncmp.c",
            "string/strncpy.c",
            "string/strrchr.c",
            "string/strstr.c",
            "string/strtok.c"
        }
    }
}
