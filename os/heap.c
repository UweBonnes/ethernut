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
 * Revision 1.12  2008/06/09 16:50:39  thiagocorrea
 * NutHeapRealloc code contributed by Moritz Struebe. Thanks!
 * Add realloc to override libc's realloc using NutHeapRealloc.
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
#include <stddef.h>
#include <stdio.h>



#ifdef NUTMEM_NAMED_MEM
#include <util/crc16.h>
#endif

#ifdef NUTDEBUG
#include <sys/osdebug.h>
#endif

#ifdef NUTMEM_THREAD
#include <sys/thread.h>
#endif


#if defined(__arm__) || defined(__m68k__) || defined(__H8300H__) || defined(__H8300S__) || defined(__linux__) || defined(__APPLE__) || defined(__CYGWIN__)
#define ARCH_32BIT
#endif




#ifdef NUTMEM_NAMED_MEM
struct nmem_node_t{
	union{
		size_t size;
		UHEAPNODE hn;
	};
	struct nmem_node_t * next;
	char name[9];
	uint8_t crc8;
	
};

struct nmem_node_t * NutNmemRoot;
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
#define MEMOVHD (sizeof(UHEAPNODE) + sizeof(0xDEADBEEF) + 2)

static inline void setBeef(HEAPNODE * node){
	*((u_long *) ((uptr_t) node + node->hn_size - sizeof(0xDEADBEEF))) = 0xDEADBEEF;
}

static inline char checkBeef(HEAPNODE * node){
	return (*((u_long *) ((uptr_t) node + node->hn_size - sizeof(0xDEADBEEF))) == 0xDEADBEEF);
}



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
void *NutHeapAlloc(size_t size){
	return NutHeapAlloc_type(size, 0);
}



void *NutHeapAlloc_type(size_t size, uint8_t algo)
{
    HEAPNODE *node;
    HEAPNODE **npp;
    HEAPNODE *fit = 0;
    HEAPNODE **fpp = 0;

#if defined(__arm__) || defined(__m68k__) || defined(__H8300H__) || defined(__H8300S__) || defined(__linux__) || defined(__APPLE__) || defined(__CYGWIN__)
    /*
     * Align to the word boundary
     */
    while ((size & 0x03) != 0)
        size++;
#endif

    if (size >= available) {
#ifdef NUTDEBUG
        if (__heap_trf)
            fputs("MEMOVR\n", __heap_trs);
#endif
        return NULL;
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
		 * Found a node that fits?
         */
        if (node->hn_size >= size) {
        	if(algo == 0){ //best fit
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
        	} else { //Last fit
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
        	if(algo == 0){
	            node = (HEAPNODE *) ((uptr_t) fit + size);
	            node->hn_size = fit->hn_size - size;
	            node->hn_next = fit->hn_next;
	            fit->hn_size = size;
	            *fpp = node;
	            
        	} else {
        		node = fit;
        		fit = (HEAPNODE *)((uptr_t)fit + node->hn_size - size);
            	node->hn_size -= size;
	            //next can be cept.
	            fit->hn_size = size;
	            //*ffp does not have to modified either.
        	}
	         
        } else {
            *fpp = fit->hn_next;
        }

        available -= fit->hn_size;
        setBeef(fit);
        
        
        
           
#ifdef NUTMEM_THREAD
        //Assign to thread;
    	if(runningThread != NULL){
        	fit->hn_next = runningThread->td_heap;
        	runningThread->td_heap = fit;
    	}
#endif
        fit =(HEAPNODE *) (((UHEAPNODE *) fit) + 1);

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
int_fast8_t NutHeapFree(void *block)
{
    HEAPNODE *node;
    HEAPNODE **npp;
    HEAPNODE *fnode;
    size_t size;

    /*
     * Convert our block into a node.
     */
    
    if(block == NULL) return -1;
    
    fnode = (HEAPNODE *) (((UHEAPNODE *) block) - 1);    
    
    
#ifdef NUTDEBUG
    if (__heap_trf) {
        if (block) {
            if (!checkBeef(fnode)){
            	fputs("\nMEMCORR-", __heap_trs);  	
            }
        } else {
            fputs("\nMEMNULL", __heap_trs);
        }
    }
#endif

    if(!checkBeef(fnode)) return -2;
    

#ifdef NUTMEM_THREAD
	//Remove from thread;
	if(runningThread != NULL){
		HEAPNODE ** tnode;
		tnode = &(runningThread->td_heap);
		while(*tnode != NULL){
			if(*tnode == fnode){
				*tnode = fnode->hn_next;
				break;
			}
			tnode = &((*tnode)->hn_next);
		}
		if(tnode == NULL){
			/// \todo search other threads
		}
	}
#endif

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
            return -3;
        }



        npp = &node->hn_next;
        node = node->hn_next;
    }

    /*
     * If no link was found, put us at the end of the list
     */
    if (!node) {
        fnode->hn_next = NULL;
        *npp = fnode;
    }
    available += size;

    return 0;
}


/**
 * \brief Change the size of a memory block. If more memory is 
 * 				requested than available at that block the data
 * 				is copied to a new, bigger block.
 * 
 * \param block Points to a memory block previously allocated
 *				through a call to NutHeapAlloc(). If block is 
 * 				NULL this call is equivalent to malloc.
 * \param size The size of the new allocated block.
 * 
 * \return > NULL A pointer to the memory block
 * \return NULL Reallocation has failed. The data is still 
 * 				available at the old address.
 * \todo see if we can do some code sharing with malloc
 * \todo add some checks like reallocating free mem, etc
 */
void *NutHeapRealloc( void * block, u_short size)
{
	HEAPNODE *node;
	HEAPNODE **npp;
	HEAPNODE *fnode;
	HEAPNODE *newNode;
	size_t size_miss;
	void * newmem;

	
	//basic forwarding
	if(size == 0){
		if( NutHeapFree(block) == 0){
			return block;
		} else {
			return NULL;
		} 
	} else if(block == NULL){
		return 	NutHeapAlloc(size);
	}

	/*
	 * Convert our block into a node.
	 */
	fnode = (HEAPNODE *) (((UHEAPNODE *) block) - 1);
	

#ifdef NUTDEBUG
    if (__heap_trf) {
        
        if (!checkBeef(fnode)){
        	fputs("\nMEMCORR-", __heap_trs);  	
        }
        
    }
#endif

	// Calculate minimum size
	if ((size += MEMOVHD) < sizeof(HEAPNODE))
		size = sizeof(HEAPNODE);




#ifdef NUTDEBUG
	if (__heap_trf)
		fprintf(__heap_trs, "\n[H%x,R%d] ", (u_int)(uptr_t) block, (int)fnode->hn_size);
#endif
	
	if (size > fnode->hn_size){	 //More ram is needed


		//Check whether there is a free node behind.
		node = heapFreeList;
		npp = (HEAPNODE **) & heapFreeList;
		size_miss = size - fnode->hn_size;
		//Find the first node behind the node to realloc
		while(node != NULL && fnode < node){			 
			npp = &node->hn_next;
			node = node->hn_next;
		} 
		

		if(node != NULL){ // There is a node behind the node
			/*
			 * If a free node is following us _and_ is big enaugh: use it!
			 */
			if (((uptr_t) fnode + fnode->hn_size) == (uptr_t) node &&  node->hn_size >= size_miss) {
				if(node->hn_size + ALLOC_THRESHOLD >= size_miss){ //split next block
		
					
					newNode = (HEAPNODE *) ((uptr_t) node + size_miss); //create new node;
					//Memove seves difficulties when allocating less then HEAPNODE bytes
					memmove(newNode, node, sizeof(HEAPNODE)); 
					newNode->hn_size -= size_miss;
					//newNode->hn_next is already ok.
					*npp = newNode;	//link previous node to new node.
					fnode->hn_size = size; //Adjust size of current node
					available -= size_miss;
				} else { //Fits nicely
					*npp = node->hn_next;	//Link previus node 
					fnode->hn_size += node->hn_size;
					available -= node->hn_size;
				}
				setBeef(fnode);
#ifdef NUTDEBUG
    if (__heap_trf) {
        fprintf(__heap_trs, "\n[H%x,R%d/%d] ", (u_int)(uptr_t) fit, (int)(((HEAPNODE *) ((UHEAPNODE *) fit - 1))->hn_size), (int)size);
    }
#endif
				return block; 
			}
		}

		
		//Failed to resize -> move
		
		newmem = NutHeapAlloc(size);
		if(newmem == NULL) return NULL; //Failed to allocate a big enough block.
		memcpy(newmem, block, fnode->hn_size - MEMOVHD); //MWMOVHD must not to be moved!!!
		NutHeapFree(block);
		return newmem;
		
	} 
	
	
	//The new size is smaller. 
	if(size + REALLOC_THRESHOLD + MEMOVHD < fnode->hn_size){	
		newNode = (HEAPNODE *) ((uptr_t) fnode + size); //behind realloc node
		newNode->hn_size = fnode->hn_size - size; //set size of freed mem
		fnode->hn_size = size; //Adjust the size of the realloc node
		setBeef(fnode); //Add new beef to current node
		NutHeapFree((void *)((UHEAPNODE *) newNode + 	1)); //Free the block				
	}
#ifdef NUTDEBUG
    if (__heap_trf) {
        fprintf(__heap_trs, "\n[H%x,R%d/%d] ", (u_int)(uptr_t) fit, (int)(((HEAPNODE *) ((UHEAPNODE *) fit - 1))->hn_size), (int)size);
    }
#endif
	return block;
}

#ifdef NUTMEM_NAMED_MEM

/**
 * Checks whether a shem node is valid.
 * \param addr Pointer to the node to check
 * \return 1 Node ok.
 * \return 0 Node not ok.
 */
static inline int8_t NutNmem_node_chk(struct nmem_node_t * addr){
	uint8_t * p;
	uint8_t crc;
	uint8_t i;
	p = (uint8_t *) addr;
	crc = 0xFF;
	for(i = 0; i < offsetof(struct nmem_node_t, crc8); i++){
		crc = _crc_ibutton_update(crc, *(p++)); 
	} 
	crc = ~crc;

	if(*p == crc) return 1;
	return 0;
}

/**
 * Calculates and sets the CRC of a node.
 * \param addr Pointer to the node to check
 */
static inline void NutNmem_node_crcset(struct nmem_node_t * addr){
	uint8_t * p;
	uint8_t crc;
	uint8_t i;
	p = (uint8_t *) addr;
	crc = 0xFF;
	for(i = 0; i < offsetof(struct nmem_node_t, crc8); i++){
		crc = _crc_ibutton_update(crc, *(p++)); 
	} 
	*p = ~crc;
}



#endif




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
#ifdef NUTMEM_NAMED_MEM
	
#if defined (NUTMEM_STACKHEAP)
	if(heapFreeList != stackHeapFreeList){
#else
	{
#endif
		uptr_t start, end;
		struct nmem_node_t *p;
		uptr_t first;
		if(NutNmemRoot == NULL){
			size -= sizeof(struct nmem_node_t); //Make space for the Poiner
			NutNmemRoot = (struct nmem_node_t *)((uptr_t)addr + size); //Place the root.
		}
		
		//This gets optimized by the compiler. But it more easy to read this way.
		start = (uptr_t)addr;	
		end = (uptr_t)addr + size;
		
		//Let's see whether we get reasonable data
		p = NutNmemRoot;
		if(NutNmem_node_chk(p)== 0){
			 /* This is ok, because it is not a normal node. Otherwise the size information needed
			  * by the "normal" mem management would be overwritten */
			memset((void*)p,0, sizeof(*p));
			NutNmem_node_crcset(p);
		} else {
			while(p != NULL){
				struct nmem_node_t * next;
				next = p->next;
				
				//If it's the last segment there's noting to check;
				if(next != NULL){
					//Check wheter the node is valid; It's not possible to fix anything.
					if(!NutNmem_node_chk(next)){
						p->next = NULL;
						NutNmem_node_crcset(p);
					}
				}
				p = next;
			}
		}
		/*The list seems ok. Now let's fee the mem in between*/
		do{
			struct nmem_node_t *p;
			//Search the first node in unfreed mem, and free mem in front.
			p = NutNmemRoot;
			first = (uptr_t) NutNmemRoot;				
			while(p != NULL){
				if((uptr_t)p > start && (uptr_t)p < first) first = (uptr_t)p;
				p = p->next;				
			}
			//Set free mem up to the first Nmem;
			if(first > start){
				*((uptr_t *) start) = first - start;
				setBeef((HEAPNODE *)start);
				NutHeapFree(((UHEAPNODE *) start) + 1); //Free the mem
			}
			start = first + ((struct nmem_node_t *)first)->size; //Now let's start behind the start-block
			
		} while(first != (uptr_t)NutNmemRoot);	
		
		return;
	}
#endif	//ifdef NUTMEM_NAMED_MEM
    *((uptr_t *) addr) = size;
    setBeef((HEAPNODE *)addr);
    NutHeapFree((UHEAPNODE *) addr + 1);
    

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

#ifdef NUTMEM_NAMED_MEM
/*!
 * \breif Get an already crated shared memory
 * 
 * \param[out] size The size of the shared memory
 * \param[in] name Name of the shared memory
 * \return NULL No memory with that name found.
 * \return > NULL memory found. The size of the memory is returend in the size pointer.
 */

void * NutNmemGet(char * name, size_t * size){
	struct nmem_node_t * p;
	p = NutNmemRoot;
		
	while(p != NULL){
		if(strncmp(name,p->name,8) == 0){ //Is this the NutNmem we a looking for?
			return (void *)(p + 1);
		} 
		p = p->next;
	}
	return NULL;
}

void * NutNmemCreate(char * name, size_t size){
	struct nmem_node_t *p;
	p = NutNmemRoot;
	UHEAPNODE * temp;
		
	while(1){
		
		if(strcmp(name,p->name) == 0){ //Is this the NutNmem we a looking for?
			return NULL; //It already exists.
		}
		if(p->next == NULL) break;
		p = p->next;
	}
	//We need to allocate mem. The next two lines can be put into one, but why?
	temp = (UHEAPNODE *) NutHeapAlloc_type(size + sizeof(struct nmem_node_t) - sizeof(UHEAPNODE), 1);
	//Update the last node
	p->next = (struct nmem_node_t *)(temp - 1);
	NutNmem_node_crcset(p);
	
	//Update the new node
	p = p->next;
	p->next = 0;
	memset(p->name, 0, 9);
	strncpy(p->name, name, 8);
	NutNmem_node_crcset(p);
	p++; //Return a pointer to the memory behind the header
	return (void *)p; 
}


/**
 * \brief Free a shared memory entry
 * \param name The name of the entry
 * \return 0 Free Successful
 * \return -1 No entry with this name found.
 * \return -2 Error freeing shared memory
 */  
int_fast8_t NutNmemFree(void * _NutNmem){
	struct nmem_node_t *p, *NutNmem_free;
	
	//Move address to the header.
	NutNmem_free = (struct nmem_node_t *) _NutNmem - 1; 
	
	//Find previous list element.
	p = NutNmemRoot;	
	while(p->next != NutNmem_free){
		p = p->next;
		if(p == NULL) return -3;
	}
	
	//Update previous element.
	p->next = NutNmem_free->next;
	NutNmem_node_crcset(p);
	///\todo What shall be done if NutHeapFree fails? Unlikely but possible.
	//Free memory
	return NutHeapFree((UHEAPNODE *)NutNmem_free + 1 );
	
}


	
char * NutNmemRead(char * name){
	struct nmem_node_t *p;
	if(name == NULL) name = NutNmemRoot->name;
	p = NutNmemRoot;	
	while((uptr_t)p->name != (uptr_t)name){
		p = p->next;
		if(p == NULL) return NULL;
	}
	p = p->next;
	if(p == NULL) return NULL;
	return p->name;	
}


#endif //NUTMEM_NAMED_MEM

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

int_fast8_t NutStackFree(void *block)
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

size_t NutStackAvailable(void){
	return stackHeapAvailable;
}

#endif /* defined(NUTMEM_STACKHEAP) */

/*@}*/
