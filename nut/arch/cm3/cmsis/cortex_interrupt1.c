//*****************************************************************************
//
// cpu.c - Instruction wrappers for special CPU instructions needed by the
//         drivers.
//
// Copyright (c) 2006-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 5727 of the Stellaris Peripheral Driver Library.
//
//*****************************************************************************

unsigned long __attribute__ ( ( naked ) ) CPUcpsie( void )
{
  unsigned long ulRet;

  //
  // Read PRIMASK and enable interrupts.
  //
__asm( "    mrs     %0, PRIMASK\n" "    cpsie   i\n" "    bx      lr\n":"=r"( ulRet ) );

  return ( ulRet );
}

unsigned long __attribute__ ( ( naked ) ) CPUcpsid( void )
{
  unsigned long ulRet;

  //
  // Read PRIMASK and disable interrupts.
  //
__asm( "    mrs     %0, PRIMASK\n" "    cpsid   i\n" "    bx      lr\n":"=r"( ulRet ) );

  return ( ulRet );
}

