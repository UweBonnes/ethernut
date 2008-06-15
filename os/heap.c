/*
 * Copyright (C) 2001-2005 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.13  2008/06/15 17:07:15  haraldkipp
 * Rolled back to version 1.11.
 *
 * Revision 1.11  2006/10/05 17:26:15  haraldkipp
 * Fixes bug #1567729. Thanks to Ashley Duncan.
 *
 * Revision 1.10  2006/09/29 12:26:14  haraldkipp
 * All code should use dedicated stack allocation routines. For targets
 * allocating stack from the normal heap the API calls are remapped by
 * preprocessor macros.
 * Stack allocation code moved from thread module to heap module.
 * Adding static attribute to variable 'available' will avoid interference
 * with application code. The ugly format macros had been replaced by
 * converting all platform specific sizes to unsigned integers.
 *
 * Revision 1.9  2005/08/02 17:47:04  haraldkipp
 * Major API documentation update.
 *
 * Revision 1.8  2005/07/26 15:49:59  haraldkipp
 * Cygwin support added.
 *
 * Revision 1.7  2005/04/30 16:42:42  chaac
 * Fixed bug in handling of NUTDEBUG. Added include for cfg/os.h. If NUTDEBUG
 * is defined in NutConf, it will make effect where it is used.
 *
 * Revision 1.6  2004/11/08 18:15:02  haraldkipp
 * Very bad hack to support 32-bit boundaries.
 *
 * Revision 1.5  2004/04/07 12:13:58  haraldkipp
 * Matthias Ringwald's *nix emulation added
 *
 * Revision 1.4  2004/03/19 09:05:12  jdubiec
 * Fixed format strings declarations for AVR.
 *
 * Revision 1.3  2004/03/16 16:48:45  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.2  2003/07/20 16:04:36  haraldkipp
 * Nicer debug output
 *
 * Revision 1.1.1.1  2003/05/09 14:41:49  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.18  2003/05/06 18:53:10  harald
 * Use trace flag
 *
 * Revision 1.17  2003/03/31 14:53:30  harald
 * Prepare release 3.1
 *
 * Revision 1.16  2003/02/04 18:15:56  harald
 * Version 3 released
 *
 * Revision 1.15  2002/06/26 17:29:44  harald
 * First pre-release with 2.4 stack
 *
 */

/*!
 * \addtogroup xgHeap
 */

/*@{*/

#include <cfg/os.h>
#include <compiler.h>
#include <string.h>

#include <sys/atom.h>
#include <sys/heap.h>

#ifdef NUTDEBUG
#include <sys/osdebug.h>
#endif

#if defined(__arm__) || defined(__m68k__) || defined(__H8300H__) || defined(__H8300S__) || defined(__linux__) || defined(__APPLE__) || defined(__CYGWIN__)
#define ARCH_32BIT
#endif

/*!
 * \brief List of free nodes.
 */
HEAPNODE *volatile heapFreeList;

/*!
 * \brief Number of bytes available.
 */
static size_t available;

/*!
 * \brief Overhead for each allocated memory clock.
 * \showinitializer
 */
/* MEMOVHD = sizeof(HEAPNODE:hn_size) + sizeof(0xDEADBEEF) */
#define MEMOVHD (sizeof(size_t) + sizeof(0xDEADBEEF))

/*!
 * \brief
 * Allocate a block from heap memory.
 *
 * This functions allocates a memory block of the specified
 * size and returns a pointer to that block.
 *
 * The actual size of the allocated block is larger than the
 * requested size because of space required for maintenance
 * information. This additional information is invisible to
 * the application.
 *
 * The routine looks for the smallest block that will meet
 * the required size and releases it to the caller. If the
 * block being requested is usefully smaller than the smallest
 * free block then the block from which the request is being
 * met is split in two. The unused portion is put back into
 * the free-list.
 *
 * The contents of the allocated block is unspecified.
 * To allocate a block with all bytes set to zero use
 * NutHeapAllocClear().
 *
 * \note Do not use this function in interrupt routines.
 *
 * \param size Size of the requested memory block.
 *
 * \return Pointer to the allocated memory block if the
 *         function is successful or NULL if the requested
 *         amount of memory is not available.
 */
void *NutHeapAlloc(size_t size)
{
    HEAPNODE *node;
    HEAPNODE **npp;
    HEAPNODE *fit = 0;
    HEAPNODE **fpp = 0;

#if defined(__arm__) || defined(__m68k__) || defined(__H8300H__) || defined(__H8300S__) || defined(__linux__) || defined(__APPLE__) || defined(__CYGWIN__)
    /*
     * Allign to the word boundary
     */
    while ((size & 0x03) != 0)
        size++;
#endif

    if (size >= available) {
#ifdef NUTDEBUG
        if (__heap_trf)
            fputs("MEMOVR\n", __heap_trs);
#endif
        return 0;
    }

    /*
     * We need additional space in front of the allocated memory
     * block to store its size. If this is still less than the
     * space required by a free node, increase it.
     */
    if ((size += MEMOVHD) < sizeof(HEAPNODE))
        size = sizeof(HEAPNODE);

    /*
     * Walk through the linked list of free nodes and find the best fit.
     */
    node = heapFreeList;
    npp = (HEAPNODE **) & heapFreeList;
    while (node) {

        /*
         * Found a note that fits?
         */
        if (node->hn_size >= size) {
            /*
             * If it's an exact match, we don't
             * search any further.
             */
            if (node->hn_size == size) {
                fit = node;
                fpp = npp;
                break;
            }

            /*
             * Is it the first one we found
             * or was the previous one larger?
             */
            if (fit == 0 || (fit->hn_size > node->hn_size)) {
                fit = node;
                fpp = npp;
            }
        }
        npp = &node->hn_next;
        node = node->hn_next;
    }

    if (fit) {
        /*
         * If the node we found is larger than the
         * required space plus the space needed for
         * a new node plus a defined threshold, then
         * we split it.
         */
        if (fit->hn_size > size + sizeof(HEAPNODE) + ALLOC_THRESHOLD) {
            node = (HEAPNODE *) ((uptr_t) fit + size);
            node->hn_size = fit->hn_size - size;
            node->hn_next = fit->hn_next;
            fit->hn_size = size;
            *fpp = node;
        } else
            *fpp = fit->hn_next;

        available -= fit->hn_size;
        *((u_long *) (((char *) fit) + (fit->hn_size - 4))) = 0xDEADBEEF;
        fit = (HEAPNODE *) & fit->hn_next;
    }
#ifdef NUTDEBUG
    if (__heap_trf) {
        fprintf(__heap_trs, "\n[H%x,A%d/%d] ", (u_int)(uptr_t) fit, (int)(((HEAPNODE *) (((uptr_t *) fit) - 1))->hn_size), (int)size);
    }
#endif
    return fit;
}

/*!
 * \brief Allocate an initialized block from heap memory.
 *
 * This functions allocates a memory block of the specified
 * size with all bytes initialized to zero and returns a
 * pointer to that block.
 *
 * \param size Size of the requested memory block.
 *
 * \return Pointer to the allocated memory block if the
 *         function is successful or NULL if the requested
 *         amount of memory is not available.
 */
void *NutHeapAllocClear(size_t size)
{
    void *ptr;

    if ((ptr = NutHeapAlloc(size)) != 0)
        memset(ptr, 0, size);

    return ptr;
}

/*!
 * \brief Return a block to heap memory.
 *
 * An application calls this function, when a previously
 * allocated memory block is no longer needed.
 *
 * The heap manager checks, if the released block adjoins any
 * other free regions. If it does, then the adjacent free regions
 * are joined together to form one larger region.
 *
 * \note Do not use this function in interrupt routines.
 *
 * \param block Points to a memory block previously allocated
 *              through a call to NutHeapAlloc().
 *
 * \return 0 on success, -1 if the caller tried to free
 *         a block which had been previously released.
 */
int NutHeapFree(void *block)
{
    HEAPNODE *node;
    HEAPNODE **npp;
    HEAPNODE *fnode;
    size_t size;

#ifdef NUTDEBUG
    if (__heap_trf) {
        if (block) {
            size_t size;
            size = *(((uptr_t *) block) - 1);
            if (*((u_long *) (((char *) block) + (size - MEMOVHD))) != 0xDEADBEEF)
                fputs("\nMEMCORR-", __heap_trs);

        } else
            fputs("\nMEMNULL", __heap_trs);
    }
#endif

    /*
     * Convert our block into a node.
     */
    fnode = (HEAPNODE *) (((uptr_t *) block) - 1);

#ifdef NUTDEBUG
    if (__heap_trf)
        fprintf(__heap_trs, "\n[H%x,F%d] ", (u_int)(uptr_t) block, (int)fnode->hn_size);
#endif
    size = fnode->hn_size;

    /*
     * Walk through the linked list of free nodes and try
     * to link us in.
     */
    node = heapFreeList;
    npp = (HEAPNODE **) & heapFreeList;
    while (node) {
        /*
         * If there' s a free node in front of us, merge it.
         */
        if (((uptr_t) node + node->hn_size) == (uptr_t) fnode) {
            node->hn_size += fnode->hn_size;

            /*
             * If a free node is following us, merge it.
             */
            if (((uptr_t) node + node->hn_size) == (uptr_t) node->hn_next) {
                node->hn_size += node->hn_next->hn_size;
                node->hn_next = node->hn_next->hn_next;
            }
            break;
        }

        /*
         * If we walked past our address, link us to the list.
         */
        if ((uptr_t) node > (uptr_t) fnode) {
            *npp = fnode;

            /*
             * If a free node is following us, merge it.
             */
            if (((uptr_t) fnode + fnode->hn_size) == (uptr_t) node) {
                fnode->hn_size += node->hn_size;
                fnode->hn_next = node->hn_next;
            } else
                fnode->hn_next = node;
            break;
        }

        /*
         * If we are within a free node, somebody tried
         * to free a block twice.
         */
        if (((uptr_t) node + node->hn_size) > (uptr_t) fnode) {
#ifdef NUTDEBUG
            if (__heap_trf)
                fputs("\nTWICE\n", __heap_trs);
#endif
            //NutExitCritical();
            return -1;
        }

        npp = &node->hn_next;
        node = node->hn_next;
    }

    /*
     * If no link was found, put us at the end of the list
     */
    if (!node) {
        fnode->hn_next = node;
        *npp = fnode;
    }
    available += size;

    return 0;
}

/*!
 * \brief
 * Add a new memory region to the free heap.
 *
 * This function is automatically called by Nut/OS during
 * initialization.
 *
 * Applications typically do not call this function.
 *
 * \param addr Start address of the memory region.
 * \param size Number of bytes of the memory region.
 */
void NutHeapAdd(void *addr, size_t size)
{
    *((uptr_t *) addr) = size;
    NutHeapFree(((uptr_t *) addr) + 1);
}

/*!
 * \brief Return the number of bytes available.
 *
 * \return Number of bytes.
 */
size_t NutHeapAvailable(void)
{
    return available;
}


#if defined (NUTMEM_STACKHEAP) /* Stack resides in internal memory */
/*
 * The following routines are wrappers around the standard heap
 * allocation routines.  These wrappers tweak the free heap pointer to point
 * to a second heap which is kept in internal memory and used only for a
 * thread's stack.
 */

static HEAPNODE* volatile stackHeapFreeList; /* for special stack heap */
static u_short stackHeapAvailable;

void *NutStackAlloc(size_t size)
{
    void * result;
    HEAPNODE* savedHeapNode;
    u_short savedAvailable;

    // Save current real heap context
    savedHeapNode = heapFreeList;
    savedAvailable = available;
    // Restore stack-heap context
    heapFreeList = stackHeapFreeList;
    available = stackHeapAvailable;

    result = NutHeapAlloc(size);

    // Save stack-heap context
    stackHeapFreeList = heapFreeList;
    stackHeapAvailable = available;
    // Restore real heap context
    heapFreeList = savedHeapNode;
    available = savedAvailable;

    return result;
}

int NutStackFree(void *block)
{
    int result;
    HEAPNODE* savedHeapNode;
    u_short savedAvailable;

    // Save current real heap context
    savedHeapNode = heapFreeList;
    savedAvailable = available;
    // Restore stack-heap context
    heapFreeList = stackHeapFreeList;
    available = stackHeapAvailable;

    result = NutHeapFree(block);

    // Save stack-heap context
    stackHeapFreeList = heapFreeList;
    stackHeapAvailable = available;
    // Restore real heap context
    heapFreeList = savedHeapNode;
    available = savedAvailable;

    return result;
}

void NutStackAdd(void *addr, size_t size)
{
   HEAPNODE* savedHeapNode;
   u_short savedAvailable;

   // Save current real heap context
   savedHeapNode = heapFreeList;
   savedAvailable = available;
   // Restore stack-heap context
   heapFreeList = stackHeapFreeList;
   available = stackHeapAvailable;

   NutHeapAdd(addr, size);

   // Save stack-heap context
   stackHeapFreeList = heapFreeList;
   stackHeapAvailable = available;
   // Restore real heap context
   heapFreeList = savedHeapNode;
   available = savedAvailable;
}

#endif /* defined(NUTMEM_STACKHEAP) */

/*@}*/
