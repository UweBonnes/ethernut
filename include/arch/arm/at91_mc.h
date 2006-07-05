#ifndef _ARCH_ARM_AT91_MC_H_
#define _ARCH_ARM_AT91_MC_H_

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

/*!
 * \file arch/arm/at91_mc.h
 * \brief AT91 peripherals.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/07/05 07:45:28  haraldkipp
 * Split on-chip interface definitions.
 *
 *
 * \endverbatim
 */

#define MC_BASE     0xFFFFFF00          /* Memory controller base. */

#define MC_RCR      (MC_BASE + 0x00)    /* MC Remap Control Register. */
#define MC_RCB          ((unsigned int) 0x1 <<  0) /* Remap Command Bit. */

#define MC_ASR      (MC_BASE + 0x04)    /* MC Abort Status Register. */
#define MC_UNDADD       ((unsigned int) 0x1 <<  0) /* Undefined Addess Abort Status. */
#define MC_MISADD       ((unsigned int) 0x1 <<  1) /* Misaligned Addess Abort Status. */
#define MC_ABTSZ        ((unsigned int) 0x3 <<  8) /* Abort Size Status. */
#define MC_ABTSZ_BYTE   ((unsigned int) 0x0 <<  8) /* Byte. */
#define MC_ABTSZ_HWORD  ((unsigned int) 0x1 <<  8) /* Half-word. */
#define MC_ABTSZ_WORD   ((unsigned int) 0x2 <<  8) /* Word. */
#define MC_ABTTYP       ((unsigned int) 0x3 << 10) /* Abort Type Status. */
#define MC_ABTTYP_DATAR ((unsigned int) 0x0 << 10) /* Data Read. */
#define MC_ABTTYP_DATAW ((unsigned int) 0x1 << 10) /* Data Write. */
#define MC_ABTTYP_FETCH ((unsigned int) 0x2 << 10) /* Code Fetch. */
#define MC_MST0         ((unsigned int) 0x1 << 16) /* Master 0 Abort Source. */
#define MC_MST1         ((unsigned int) 0x1 << 17) /* Master 1 Abort Source. */
#define MC_SVMST0       ((unsigned int) 0x1 << 24) /* Saved Master 0 Abort Source. */
#define MC_SVMST1       ((unsigned int) 0x1 << 25) /* Saved Master 1 Abort Source. */

#define MC_AASR     (MC_BASE + 0x08)    /* MC Abort Address Status Register. */

#define MC_FMR      (MC_BASE + 0x60)    /* MC Flash Mode Register. */
#define MC_FRDY         ((unsigned int) 0x1 <<  0) /* Flash Ready. */
#define MC_LOCKE        ((unsigned int) 0x1 <<  2) /* Lock Error. */
#define MC_PROGE        ((unsigned int) 0x1 <<  3) /* Programming Error. */
#define MC_NEBP         ((unsigned int) 0x1 <<  7) /* No Erase Before Programming. */
#define MC_FWS          ((unsigned int) 0x3 <<  8) /* Flash Wait State. */
#define MC_FWS_0FWS     ((unsigned int) 0x0 <<  8) /* 1 cycle for Read, 2 for Write operations. */
#define MC_FWS_1FWS     ((unsigned int) 0x1 <<  8) /* 2 cycles for Read, 3 for Write operations. */
#define MC_FWS_2FWS     ((unsigned int) 0x2 <<  8) /* 3 cycles for Read, 4 for Write operations. */
#define MC_FWS_3FWS     ((unsigned int) 0x3 <<  8) /* 4 cycles for Read, 4 for Write operations. */
#define MC_FMCN         ((unsigned int) 0xFF << 16)/* Flash Microsecond Cycle Number. */

#define MC_FCR      (MC_BASE + 0x64)    /* MC Flash Command Register. */
#define MC_FCMD         ((unsigned int) 0xF <<  0) /* Flash Command. */
#define MC_FCMD_START_PROG ((unsigned int) 0x1) /* Starts the programming of th epage specified by PAGEN.. */
#define MC_FCMD_LOCK       ((unsigned int) 0x2) /* Starts a lock sequence of the sector defined by the bits 4 to 7 of the field PAGEN.. */
#define MC_FCMD_PROG_AND_LOCK ((unsigned int) 0x3) /* The lock sequence automatically happens after the programming sequence is completed.. */
#define MC_FCMD_UNLOCK     ((unsigned int) 0x4) /* Starts an unlock sequence of the sector defined by the bits 4 to 7 of the field PAGEN.. */
#define MC_FCMD_ERASE_ALL  ((unsigned int) 0x8) /* Starts the erase of the entire flash.If at least a page is locked, the command is cancelled.. */
#define MC_FCMD_SET_GP_NVM ((unsigned int) 0xB) /* Set General Purpose NVM bits.. */
#define MC_FCMD_CLR_GP_NVM ((unsigned int) 0xD) /* Clear General Purpose NVM bits.. */
#define MC_FCMD_SET_SECURITY ((unsigned int) 0xF) /* Set Security Bit.. */
#define MC_PAGEN        ((unsigned int) 0x3FF <<  8) /* Page Number. */
#define MC_KEY          ((unsigned int) 0xFF << 24) /* Writing Protect Key. */

#define MC_FSR      (MC_BASE + 0x68)    /* MC Flash Status Register. */
#define MC_SECURITY     ((unsigned int) 0x1 <<  4) /* Security Bit Status. */
#define MC_GPNVM0       ((unsigned int) 0x1 <<  8) /* Sector 0 Lock Status. */
#define MC_GPNVM1       ((unsigned int) 0x1 <<  9) /* Sector 1 Lock Status. */
#define MC_GPNVM2       ((unsigned int) 0x1 << 10) /* Sector 2 Lock Status. */
#define MC_GPNVM3       ((unsigned int) 0x1 << 11) /* Sector 3 Lock Status. */
#define MC_GPNVM4       ((unsigned int) 0x1 << 12) /* Sector 4 Lock Status. */
#define MC_GPNVM5       ((unsigned int) 0x1 << 13) /* Sector 5 Lock Status. */
#define MC_GPNVM6       ((unsigned int) 0x1 << 14) /* Sector 6 Lock Status. */
#define MC_GPNVM7       ((unsigned int) 0x1 << 15) /* Sector 7 Lock Status. */
#define MC_LOCKS0       ((unsigned int) 0x1 << 16) /* Sector 0 Lock Status. */
#define MC_LOCKS1       ((unsigned int) 0x1 << 17) /* Sector 1 Lock Status. */
#define MC_LOCKS2       ((unsigned int) 0x1 << 18) /* Sector 2 Lock Status. */
#define MC_LOCKS3       ((unsigned int) 0x1 << 19) /* Sector 3 Lock Status. */
#define MC_LOCKS4       ((unsigned int) 0x1 << 20) /* Sector 4 Lock Status. */
#define MC_LOCKS5       ((unsigned int) 0x1 << 21) /* Sector 5 Lock Status. */
#define MC_LOCKS6       ((unsigned int) 0x1 << 22) /* Sector 6 Lock Status. */
#define MC_LOCKS7       ((unsigned int) 0x1 << 23) /* Sector 7 Lock Status. */
#define MC_LOCKS8       ((unsigned int) 0x1 << 24) /* Sector 8 Lock Status. */
#define MC_LOCKS9       ((unsigned int) 0x1 << 25) /* Sector 9 Lock Status. */
#define MC_LOCKS10      ((unsigned int) 0x1 << 26) /* Sector 10 Lock Status. */
#define MC_LOCKS11      ((unsigned int) 0x1 << 27) /* Sector 11 Lock Status. */
#define MC_LOCKS12      ((unsigned int) 0x1 << 28) /* Sector 12 Lock Status. */
#define MC_LOCKS13      ((unsigned int) 0x1 << 29) /* Sector 13 Lock Status. */
#define MC_LOCKS14      ((unsigned int) 0x1 << 30) /* Sector 14 Lock Status. */
#define MC_LOCKS15      ((unsigned int) 0x1 << 31) /* Sector 15 Lock Status. */

#endif                          /* _ARCH_ARM_AT91_MC_H_ */
