#ifndef _SYS_BANKMEM_H_
#define _SYS_BANKMEM_H_

/*
 * Copyright (C) 2003 by egnite Software GmbH. All rights reserved.
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
 *
 */

/*!
 * $Log$
 * Revision 1.2  2003/12/15 19:27:53  haraldkipp
 * Ethernut 2 support added
 *
 * Revision 1.1  2003/07/21 18:21:34  haraldkipp
 * First check in
 *
 */

#include <sys/types.h>

/*!
 * \file sys/bankmem.h
 * \brief Banked memory management definitions.
 */

#ifndef NUTBANK_COUNT
#ifdef ETHERNUT2
/*!
 * \brief Number of memory banks.
 *
 * For systems without banked memory this is set to zero.
 * Ethernut 2 has 30 memory banks.
 */
#define NUTBANK_COUNT   30
#else
#define NUTBANK_COUNT   0
#endif
#endif

#ifndef NUTBANK_START
/*!
 * \brief Start address of memory banks.
 *
 * For systems without banked memory this is ignored.
 */
#define NUTBANK_START   ((char *)0x8000)
#endif

#ifndef NUTBANK_SIZE
/*!
 * \brief Size of a single memory bank.
 *
 * For systems without banked memory this is ignored.
 */
#define NUTBANK_SIZE    0x4000
#endif

#ifndef NUTBANK_SR
/*!
 * \brief Address of the bank select register.
 *
 * For systems without banked memory this is ignored.
 */
#define NUTBANK_SR      ((u_char *)0xFF00)
#endif

#ifndef NutSegBufEnable
#define NutSegBufEnable(bank) *(NUTBANK_SR + (bank)) = (bank);
#endif

__BEGIN_DECLS
/* Prototypes */
extern char *NutSegBufReset(void);
extern char *NutSegBufInit(size_t size);
extern char *NutSegBufWriteRequest(size_t * bcp);
extern char *NutSegBufReadRequest(size_t * bcp);
extern char *NutSegBufWriteCommit(u_short bc);
extern char *NutSegBufReadCommit(size_t bc);
extern void NutSegBufWriteLast(u_short bc);
extern void NutSegBufReadLast(u_short bc);
extern u_long NutSegBufAvailable(void);
extern u_long NutSegBufUsed(void);

__END_DECLS
#endif
