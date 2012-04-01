/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 *
 * All rights reserved.
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
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <cfg/os.h>
#include <cfg/clock.h>
#include <cfg/arch.h>
#include <cfg/memory.h>

#include <arch/cm3.h>
#include <dev/rtc.h>
#include <sys/heap.h>
#include <arch/cm3/cortexM3.h>
#include <arch/cm3/cortex_interrupt.h>

#ifndef MSP_STACK_SIZE
#define MSP_STACK_SIZE 128
#endif

#ifndef PSP_STACK_SIZE
#define PSP_STACK_SIZE 32
#endif

#ifndef NUT_BOOT_FUNCTION
#define NUT_BOOT_FUNCTION NutInit
#endif

extern void NUT_BOOT_FUNCTION(void *);

/*!
 * \brief CortexM3 MSP processor stack.
 */
__attribute__ ((section(".stack")))
volatile static uint32_t mspStack[MSP_STACK_SIZE];

/*!
 * \brief CortexM3 PSP processor stack.
 */
__attribute__ ((section(".psp_stack")))
volatile static uint32_t pspStack[PSP_STACK_SIZE];


static void IntDefaultHandler(void *arg) __attribute__ ((naked));

/*!
 * \brief CortexM3 memory pointers
 *
 * These pointers are provided by the linker, controlled by the
 * linker script. The list is shrinked to the two basic 
 * entries of the initial stack pointer address and the 
 * reset vector. All other interrupt vectors are set up dynamically
 * later after system start.
 */
__attribute__ ((section(".isr_vector")))
#if defined(NUTDEBUG_RAM)
void (* g_pfnVectors[NUM_INTERRUPTS])(void*) =
#else
void (* const g_pfnVectors[])(void *) =
#endif
{
    (void (*)(void *))((uint32_t)mspStack + sizeof(mspStack)), /* Initial Stack Pointer */
    NUT_BOOT_FUNCTION, /* Reset_Handler */
    IntDefaultHandler, /* NMI_Handler */
    IntDefaultHandler, /* HardFault_Handler */
    IntDefaultHandler, /* MemManage_Handler */
    IntDefaultHandler, /* BusFault_Handler */
    IntDefaultHandler, /* UsageFault_Handler */
    0,                 /* Reserved */
    0,                 /* Reserved */
    0,                 /* Reserved */
    0,                 /* Reserved */
};

/*!
 * \brief CortexM3 memory pointers
 *
 * These pointers are provided by the linker, controlled by the
 * linker script.
 */
extern void * _etext;           /* Start of constants in FLASH */
extern void * _sidata;          /* Start of variables in FLASH */
extern void * _sdata;           /* Start of variables in RAM */
extern void * _edata;           /* End of variables in RAM */
extern void * _sbss;            /* Start of unset variables in RAM */
extern void * _ebss;            /* End of unset variables in RAM */

/* Remember: following stack grow from end address to start */
extern void * _pspstack_start;  /* Process stack start address */
extern void * _pspstack_end;    /* Process stack end address */
extern void * _stack_start;     /* Main stack start address */
extern void * _stack_end;       /* Main stack end address */

static void IntDefaultHandler(void *arg) 
{
    while (1);
}
    
/*!
 * \brief CortexM3 Initialization.
 *
 * This function copies over the data segment from flash to ram
 * and fills the bss segment with 0.
 */
void Cortex_MemInit(void)
{
    register uint32_t *src, *dst, *end;
    register uint32_t fill = 0;

    /*
     * Plain C-Code Cortex Init
     */

    /* Copy the data segment initializers from flash to SRAM. */
    src = (uint32_t*)&_etext;
    end = (uint32_t*)&_edata;
    for( dst = (uint32_t*)&_sdata; dst < end;)
    {
        *dst++ = *src++;
    }

    /* Fill the bss segment with 0x00 */
    end = (uint32_t*)&_ebss;
    for( dst = (uint32_t*)&_sbss; dst < end; )
    {
        *dst++ = fill;
    }
    
    __set_PSP((uint32_t)&_pspstack_end);
}

/*!
 * \brief CortexM3 Startup.
 *
 * This function runs the basic cpu initialization.
 */
void Cortex_Start(void)
{
    Cortex_MemInit();
    Cortex_IntInit();
}
