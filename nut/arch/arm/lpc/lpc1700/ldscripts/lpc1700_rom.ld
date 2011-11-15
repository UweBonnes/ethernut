/*
 * Copyright (C) 2005-2007 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

/*
 * $Id$
 */

/* OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm") */
/* ENTRY(_start) */
/* STARTUP(start_rom.o) */

STARTUP(crtlpc1700_rom.o)
SEARCH_DIR(.)

MEMORY
{
  rom (rx) : org = 0x00000000, len = 512k
  ram (rw) : org = 0x10000000, len = 32k
}

SECTIONS
{
  .text :
  {
    KEEP(*(.vectors));
    *(.init0);
    *(.init1);
    *(.init2);
    *(.init3);
    *(.init4);
    *(.init5);
    . = 0x000002FC;
    KEEP(*(.crp))
    *(.text);
    . = ALIGN(4);
    *(.rodata*);
    . = ALIGN(4);
    *(.glue_7t);
    . = ALIGN(4);
    *(.glue_7);
    . = ALIGN(4);
    _mdata = .;
    . = ALIGN(4);
    etext = .;
    _text_end = .;
    . = ALIGN(4);
  } > rom

  .ARM.exidx : {
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
  } > rom

  . = ALIGN(4);
  .ARM.extab : {
    __extab_start = .;
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    __extab_end = .;
    __data_load_start = .;
  } > rom

  .bss :
  {
    . += 0x400;
    PROVIDE (__stack = .);
    PROVIDE (__bss_start = .);
    *(.bss)
    *(COMMON)
    . = ALIGN(4);
    PROVIDE (__bss_end = .);
  } > ram

  /* .data : AT (__data_load_start) */
  .data : AT ( ADDR(.ARM.extab) + SIZEOF(.ARM.extab) )
  {
    PROVIDE (__data_start = .);
    *(.data)
    . = ALIGN(4);
    edata = .;
    _edata = .;
    PROVIDE (__data_end = .);
    PROVIDE (__heap_start = .);
  } > ram

  .stab 0 (NOLOAD) :
  {
    [ .stab ]
  }
  .stabstr 0 (NOLOAD) :
  {
    [ .stabstr ]
  }
}
