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
-- Revision 1.6  2006/02/23 15:43:08  haraldkipp
-- PHAT file system now supports configurable number of sector buffers.
--
-- Revision 1.5  2006/01/05 16:49:48  haraldkipp
-- PHAT file system driver added.
--
-- Revision 1.4  2005/04/05 17:56:16  haraldkipp
-- UROM file system is not platform dependant.
--
-- Revision 1.3  2005/02/05 20:40:44  haraldkipp
-- Peanut added.
--
-- Revision 1.2  2004/08/18 13:46:09  haraldkipp
-- Fine with avr-gcc
--
-- Revision 1.1  2004/06/07 16:35:53  haraldkipp
-- First release
--
--

nutfs =
{
    {
        name = "nutfs_pathops",
        brief = "Path Operations",
        requires = { "NUT_FS_WRITE", "NUT_FS_DIR" },
        provides = { "NUT_FS_PATHOPS" },
        description = "Standard file system functions:\n"..
                      "mkdir()\nstat()\nfstat()\nunlink()\nrmdir()\nlseek()\n",
        sources = { "pathops.c" },
    },
    {
        name = "nutfs_dirent",
        brief = "Directory Read",
        requires = { "NUT_FS_DIR" },
        provides = { "NUT_FS_DIRSTREAM" },
        description = "Standard functions for reading directories:\n"..
                      "opendir()\nclosedir()\nreaddir()\n",
        sources = { "dirent.c" },
    },
    {
        name = "nutfs_fat",
        brief = "FAT32",
        description = "Read only.",
        requires = { "NUT_EVENT", "HW_MCU_AVR" },
        provides = { "NUT_FS", "NUT_FS_READ" },
        sources = { "fat.c" }
    },
    {
        name = "nutfs_uromfs",
        brief = "UROM",
        provides = { "NUT_FS", "NUT_FS_READ" },
        description = "Read only.",
        sources = { "uromfs.c" },
        makedefs = { "CRUROM=crurom" }
    },
    {
        name = "nutfs_phatfs",
        brief = "PHAT",
        requires = { "DEV_BLOCK" },
        provides = { "NUT_FS", "NUT_FS_READ", "NUT_FS_WRITE", "NUT_FS_DIR" },
        description = "FAT compatible file system.",
        sources = { 
                "phatfs.c", 
                "phatvol.c", 
                "phatdir.c", 
                "phatio.c", 
                "phat12.c", 
                "phat16.c", 
                "phat32.c", 
                "phatutil.c",
                "phatdbg.c"
        },
        options =
        {
            {
                macro = "PHAT_SECTOR_BUFFERS",
                brief = "Sector Buffers",
                description = "Number of sector buffers.\n\n"..
                              "These buffers will be allocated from heap memory. Thus, you "..
                              "need to make sure that enough heap memory is available.\n"..
                              "If this option is disabled, the file system will use "..
                              "a single sector buffer, which is typically provided by the "..
                              "block device itself.",
                type = "integer",
                flavor = "booldata",
                file = "include/cfg/fs.h"
            },
        },
    },
    {
        name = "nutfs_pnutfs",
        brief = "PNUT",
        requires = { "HW_MCU_AVR", "NUT_SEGBUF" },
        provides = { "NUT_FS", "NUT_FS_READ", "NUT_FS_WRITE", "NUT_FS_DIR" },
        description = "RAM file system for banked memory (Ethernut 2).",
        sources = { "pnutfs.c" },
    }
}
