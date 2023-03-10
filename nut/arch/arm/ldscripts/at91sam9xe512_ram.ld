/*
 * Copyright (C) 2005-2006 by egnite Software GmbH. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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

OUTPUT_ARCH(arm)
ENTRY(_start)
STARTUP(crtat91sam9xe512_ram.o)
SEARCH_DIR(.)

MEMORY
{
  iram(rw) : org = 0x00000000, len = 32K
  xram(rw) : org = 0x20000000, len = 128M
}

SECTIONS
{
  .text :
  {
    *(.vectors);
    /* Initialization code. */
    . = ALIGN(16384); KEEP(*(.init0));
    . = ALIGN(4); *(.init1.user);
    . = ALIGN(4); *(.init1);
    . = ALIGN(4); *(.init2.user);
    . = ALIGN(4); *(.init2);
    . = ALIGN(4); *(.init3.user);
    . = ALIGN(4); *(.init3);
    . = ALIGN(4); *(.init4.user);
    . = ALIGN(4); *(.init4);
    /* Exit handling. */
    . = ALIGN(4); *(.exit0.user);
    . = ALIGN(4); *(.exit0);
    /* System and application code. */
    . = ALIGN(4); *(.text .text.*);
    *(.glue_7t);
    . = ALIGN(4);
    *(.glue_7);
  } > xram

  .rodata :
  {
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    . = ALIGN(4);
  } > xram

  .eh_frame :
  {
    KEEP (*(.eh_frame))
    . = ALIGN(4);
  } > xram

  .ctors :
  {
    PROVIDE(__ctors_start__ = .);
    KEEP(*(SORT(.ctors.*)))
    KEEP(*(.ctors))
    PROVIDE(__ctors_end__ = .);
    . = ALIGN(4);
  } > xram

  .dtors :
  {
    PROVIDE(__dtors_start__ = .);
    KEEP(*(SORT(.dtors.*)))
    KEEP(*(.dtors))
    PROVIDE(__dtors_end__ = .);
    . = ALIGN(4);
  } > xram

  .data :
  {
    . = ALIGN(4); *(.data)
    . = ALIGN(4); *(.data.*)
    . = ALIGN(4); *(.ramfunc);
    *(.gnu.linkonce.d*)
    SORT(CONSTRUCTORS) /* this entry is in the Anglia example */
    /* CONSTRUCTORS */ /* this entry is in the devkitarm script but not in the Anglia example */
    . = ALIGN(4);
  } > xram

  .bss :
  {
    PROVIDE (__bss_start = .);
    *(.bss)
    *(.gnu.linkonce.b*)
    *(COMMON)
    . = ALIGN(4);
    PROVIDE (__bss_end = .);
  } > xram

  .stacks :
  {
    /* Stacks for exception handlers and low level initialization. */
    . = ALIGN(8); *(.stack);
    PROVIDE (__stack = .);
    /* All RAM beyond this point is occupied by the heap. */
    PROVIDE (__heap_start = .);
  } > xram

  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
}
