#ifndef _SYS_HEAP_H_
#define _SYS_HEAP_H_

/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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
 * -
 * Portions Copyright (C) 2000 David J. Hudson <dave@humbug.demon.co.uk>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "copying-gpl.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "copying-liquorice.txt" for details.
 */

/*
 * $Log$
 * Revision 1.1  2003/05/09 14:41:20  haraldkipp
 * Initial revision
 *
 * Revision 1.7  2003/02/04 18:00:52  harald
 * Version 3 released
 *
 * Revision 1.6  2002/06/26 17:29:28  harald
 * First pre-release with 2.4 stack
 *
 */

#include <stddef.h>
#include <sys/types.h>

/*!
 * \file sys/heap.h
 * \brief Heap management definitions.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \struct _HEAPNODE heap.h sys/heap.h
 * \brief Heap memory node information structure.
 */
/*!
 * \typedef HEAPNODE
 * \brief Heap memory node type.
 */
typedef struct _HEAPNODE {
    u_short hn_size;            /*!< \brief Size of this node. */
    struct _HEAPNODE *hn_next;  /*!< \brief Link to next free node. */
} HEAPNODE;

extern HEAPNODE* volatile heapFreeList;


/*!
 * \brief Allocation threshold.
 *
 * Might be increased to avoid creating
 * too many small nodes.
 */
#define ALLOC_THRESHOLD 6

extern void *NutHeapAlloc(u_short size);
extern void *NutHeapAllocClear(u_short size);
extern int NutHeapFree(void *block);
extern void NutHeapAdd(void *addr, u_short size);
extern u_short NutHeapAvailable(void);

#ifdef __cplusplus
}
#endif

#endif
