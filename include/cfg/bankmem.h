#ifndef _CFG_BANKMEM_H_
#define _CFG_BANKMEM_H_

/*
 * Copyright (C) 2003-2004 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.1  2004/08/18 18:51:41  haraldkipp
 * Made banked memory configurable.
 *
 */

/*!
 * \file cfg/bankmem.h
 * \brief Banked memory default configuration.
 *
 * Values can be changed by the configurator.
 */

#ifndef NUTBANK_COUNT
/*!
 * \brief Number of memory banks.
 *
 * For systems without banked memory this is set to zero.
 * Ethernut 2 has 30 memory banks.
 */
#define NUTBANK_COUNT   0
#endif

#ifndef NUTBANK_START
/*!
 * \brief Start address of memory banks.
 *
 * For systems without banked memory this is ignored.
 */
#define NUTBANK_START   0x8000
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
#define NUTBANK_SR      0xFF00
#endif

