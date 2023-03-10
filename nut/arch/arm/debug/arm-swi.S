/*
 * Copyright (C) 2008 by Duane Ellis
 *
 * All rights reserved.
 *
 * The original code had been released as part of the LoastARM Project 
 * under GPL Version 2 and is published here under the following license
 * with kind permission from the author:
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
 * For additional information see http://lostarm.sourceforge.net/
 */

/*!
 * \file arch/arm/debug/arm-swi.S
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <arch/arm.h>
#include <arch/arm/ptrace.h>
#include <arch/arm/exception_asm.h>

	
	.section	.text,"ax","progbits"
	nop /* put this here so we can see the symbol below */
	.global	__swi
	.type	__swi, function
__swi:
	exception_enter ARM_SVC_MODE, PTRACE_FRAMETYPE_swi

	/* We need the SVC sp, not something else! */
	/* pass frame in R0 */
	mov	r0,sp
	
	/* Switch to supervisor mode */
	/* We use r5 here - cause it is saved!*/
	
	/* fetch cpsr - need T bit for thumb test */
	ldr	r2,[r0,#(PTRACE_CPSR_idx*4)]

	/* fetch PC */
	ldr	r1,[r0,#(PTRACE_R15_idx*4)]

	/* TBit Test */
	tst	r2,#0x20

	/* Thumb condition */
	ldrneh	r1,[r1,#-2]
	bicne   r1,r1,#0xff00

	/* ARM condition */
	ldreq	r1,[r1,#-4]
	biceq	r1,r1,#0xff000000

	/* Save SWI code */
	str	r1,[r0,#(PTRACE_SWICODE_idx*4)]
	mov	r1,#PTRACE_FRAMETYPE_swi
	str	r1,[r0,#(PTRACE_FRAMETYPE_idx*4)]
	
	ldr	r1,=ARM_SWI_Handler
	mov	lr,pc
	bx	r1
	
	exception_exit ARM_SVC_MODE
	nop
	.size	__swi, (. - __swi)
