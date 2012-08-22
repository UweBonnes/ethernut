//*****************************************************************************
//
// interrupt.c - Driver for the NVIC Interrupt Controller.
//
// Copyright (c) 2005-2008 Luminary Micro, Inc.  All rights reserved.
//
// Software License Agreement
//
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
//
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
//
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2752 of the Stellaris Peripheral Driver Library.
//
//*****************************************************************************
//*****************************************************************************
//
// interrupt.c - Driver for the NVIC Interrupt Controller.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup interrupt_api
//! @{
//
//*****************************************************************************

#include <cfg/arch.h>
#include <arch/cm3.h>
#include <arch/cm3/interrupt.h>
#include <sys/nutdebug.h>
#include <sys/timer.h>
#include <sys/atom.h>

//*****************************************************************************
//
//! \internal
//! The default interrupt handler.
//!
//! This is the default interrupt handler for all interrupts.  It simply loops
//! forever so that the system state is preserved for observation by a
//! debugger.  Since interrupts should be disabled before unregistering the
//! corresponding handler, this should never be called.
//!
//! \return None.
//
//*****************************************************************************
// static void IntDefaultHandler(void *arg) __attribute__ ((naked));
static void IntDefaultHandler(void *arg)
{

    while(1);
}

static void IntNmiHandler(void *arg)
{
    while(1);
}

static void IntHardfaultHandler(void *arg)
{
    while(1);
}

static void IntMemfaultHandler(void *arg)
{
    while(1);
}

static void IntBusfaultHandler(void *arg)
{
    while(1);
}

static void IntUsagefaultHandler(void *arg)
{
    while(1);
}


//*****************************************************************************
//
// The processor vector table.
//
// This contains a list of the handlers for the various interrupt sources in
// the system.  The layout of this list is defined by the hardware; assertion
// of an interrupt causes the processor to start executing directly at the
// address given in the corresponding location in this list.
//
//*****************************************************************************
#if defined(ewarm)
static __no_init void (*g_pfnRAMVectors[NUM_INTERRUPTS])(void) @ "VTABLE";
#elif defined(sourcerygxx)
static __attribute__((section(".cs3.region-head.ram")))
void (*g_pfnRAMVectors[NUM_INTERRUPTS])(void*);
#else
extern void *_evect;
#if defined(NUTDEBUG_RAM)
#define g_pfnRAMVectors g_pfnVectors
#else
static __attribute__((section(".vtable")))
void (*g_pfnRAMVectors[NUM_INTERRUPTS])(void*);
#endif
#endif

//*****************************************************************************
//
//! Registers a function to be called when an interrupt occurs.
//!
//! \param ulInterrupt specifies the interrupt in question.
//! \param pfnHandler is a pointer to the function to be called.
//!
//! This function is used to specify the handler function to be called when the
//! given interrupt is asserted to the processor.  When the interrupt occurs,
//! if it is enabled (via IntEnable()), the handler function will be called in
//! interrupt context.  Since the handler function can preempt other code, care
//! must be taken to protect memory or peripherals that are accessed by the
//! handler and other non-handler code.
//!
//! \note The use of this function (directly or indirectly via a peripheral
//! driver interrupt register function) moves the interrupt vector table from
//! flash to SRAM.  Therefore, care must be taken when linking the application
//! to ensure that the SRAM vector table is located at the beginning of SRAM;
//! otherwise NVIC will not look in the correct portion of memory for the
//! vector table (it requires the vector table be on a 1 kB memory alignment).
//! Normally, the SRAM vector table is so placed via the use of linker scripts;
//! some tool chains, such as the evaluation version of RV-MDK, do not support
//! linker scripts and therefore will not produce a valid executable.  See the
//! discussion of compile-time versus run-time interrupt handler registration
//! in the introduction to this chapter.
//!
//! \return None.
//
//*****************************************************************************
void IntRegister(IRQn_Type ulInterrupt, void (*pfnHandler)(void*))
{
    uint16_t ulIdx = ulInterrupt+16;

    /* Check for valid interrupt number */
    NUTASSERT(ulIdx < NUM_INTERRUPTS);

    /* Make sure that the RAM vector table is correctly aligned. */
    NUTASSERT(((uint32_t)g_pfnRAMVectors & 0x000003ff) == 0);

    /* Save the interrupt handler. */
    g_pfnRAMVectors[ulIdx] = pfnHandler;
}

//*****************************************************************************
//
//! Unregisters the function to be called when an interrupt occurs.
//!
//! \param ulInterrupt specifies the interrupt in question.
//!
//! This function is used to indicate that no handler should be called when the
//! given interrupt is asserted to the processor.  The interrupt source will be
//! automatically disabled (via IntDisable()) if necessary.
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void IntUnregister(IRQn_Type ulInterrupt)
{
    /* Check for valid interrupt number */
    NUTASSERT(ulInterrupt+16 < NUM_INTERRUPTS);

    /* Reset the interrupt handler to IntDefaultHandler function. */
    g_pfnRAMVectors[ulInterrupt+16] = (void *)IntDefaultHandler;
}


//*****************************************************************************
//
//! Sets the priority of an interrupt.
//!
//! \param ulInterrupt specifies the interrupt in question.
//! \param ucPriority specifies the priority of the interrupt.
//!
//! This function is used to set the priority of an interrupt.  When multiple
//! interrupts are asserted simultaneously, the ones with the highest priority
//! are processed before the lower priority interrupts.  Smaller numbers
//! correspond to higher interrupt priorities; priority 0 is the highest
//! interrupt priority.
//!
//! The hardware priority mechanism will only look at the upper N bits of the
//! priority level (where N is 3 for the Stellaris family), so any
//! prioritization must be performed in those bits.  The remaining bits can be
//! used to sub-prioritize the interrupt sources, and may be used by the
//! hardware priority mechanism on a future part.  This arrangement allows
//! priorities to migrate to different NVIC implementations without changing
//! the gross prioritization of the interrupts.
//!
//! \return None.
//
//*****************************************************************************
void IntPrioritySet(IRQn_Type Interrupt, uint32_t Priority)
{
//  NUTASSERT((ulInterrupt >= 4) && (ulInterrupt < NUM_INTERRUPTS));

    NVIC_SetPriority(Interrupt,Priority);
}

//*****************************************************************************
//
//! Gets the priority of an interrupt.
//!
//! \param ulInterrupt specifies the interrupt in question.
//!
//! This function gets the priority of an interrupt.  See IntPrioritySet() for
//! a definition of the priority value.
//!
//! \return Returns the interrupt priority, or -1 if an invalid interrupt was
//! specified.
//
//*****************************************************************************
uint32_t IntPriorityGet(IRQn_Type Interrupt)
{
//  NUTASSERT((ulInterrupt >= 4) && (ulInterrupt < NUM_INTERRUPTS));

    return NVIC_GetPriority(Interrupt);
}

//*****************************************************************************
//
//! Enables an interrupt.
//!
//! \param ulInterrupt specifies the interrupt to be enabled.
//!
//! The specified interrupt is enabled in the interrupt controller.  Other
//! enables for the interrupt (such as at the peripheral level) are unaffected
//! by this function.
//!
//! \return None.
//
//*****************************************************************************
void IntEnable(IRQn_Type ulInterrupt)
{
    if(ulInterrupt<0)
    {
        /* Core specific interrupt numbers are below 0 */
        switch(ulInterrupt)
        {
            case MemoryManagement_IRQn:
                /* Enable the MemManage interrupt. */
                SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
                break;
            case BusFault_IRQn:
                /* Enable the bus fault interrupt. */
                SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
                break;
            case UsageFault_IRQn:
                /* Enable the usage fault interrupt. */
                SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
                break;
            case SysTick_IRQn:
                /* Enable the System Tick interrupt. */
                SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
                break;
            default:
                break;
        }
    }
    else {
        /* Enable the general interrupt. */
        NVIC_EnableIRQ(ulInterrupt);
    }
}

//*****************************************************************************
//
//! Disables an interrupt.
//!
//! \param ulInterrupt specifies the interrupt to be disabled.
//!
//! The specified interrupt is disabled in the interrupt controller.  Other
//! enables for the interrupt (such as at the peripheral level) are unaffected
//! by this function.
//!
//! \return None.
//
//*****************************************************************************
void IntDisable(IRQn_Type ulInterrupt)
{
    if(ulInterrupt<0)
    {
        /* Core specific interrupt numbers are below 0 */
        switch(ulInterrupt)
        {
            case MemoryManagement_IRQn:
                    /* Disable the MemManage interrupt. */
                SCB->SHCSR &= ~SCB_SHCSR_MEMFAULTENA_Msk;
                break;
            case BusFault_IRQn:
                    /* Disable the bus fault interrupt. */
                SCB->SHCSR &= ~SCB_SHCSR_BUSFAULTENA_Msk;
                break;
            case UsageFault_IRQn:
                    /* Disable the usage fault interrupt. */
                SCB->SHCSR &= ~SCB_SHCSR_USGFAULTENA_Msk;
                break;
            case SysTick_IRQn:
                    /* Disable the System Tick interrupt. */
                SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
                break;
            default:
                break;
        }
    }
    else {
        /* Disable the general interrupt. */
        NVIC_DisableIRQ(ulInterrupt);
    }
}

//*****************************************************************************
//
//! Check if an interrupt is enabled.
//!
//! \param ulInterrupt specifies the interrupt to check.
//!
//! Check if the specified interrupt is enabled in the interrupt controller.
//!
//! \return -1 for invalid interrupt, 0 if disabled, 1 if enabled.
//
//*****************************************************************************
int IntIsEnabled(IRQn_Type ulInterrupt)
{
    int rc = 0;
    if(ulInterrupt<0)
    {
        /* Core specific interrupt numbers are below 0 */
        switch(ulInterrupt)
        {
            case NonMaskableInt_IRQn:
                rc = 1;
                break;
#if defined(MCU_STM32F1)
            case HardFault_IRQn:
                rc = 1;
#endif
                break;
            case MemoryManagement_IRQn:
                rc = (SCB->SHCSR & SCB_SHCSR_MEMFAULTENA_Msk)?1:0;
                break;
            case BusFault_IRQn:
                rc = (SCB->SHCSR & SCB_SHCSR_BUSFAULTENA_Msk)?1:0;
                break;
            case UsageFault_IRQn:
                rc = (SCB->SHCSR & SCB_SHCSR_USGFAULTENA_Msk)?1:0;
                break;
            case SysTick_IRQn:
                rc = (SysTick->CTRL & SysTick_CTRL_TICKINT_Msk)?1:0;
                break;
            default:
                rc = -1;
                break;
        }
    }
    else {
        /* Enable the general interrupt. */
        rc = NVIC_GetEnableIRQ(ulInterrupt)?1:0;
    }
    return rc;
}

void Cortex_IntInit(void)
{
    int i;

    for( i=0; i<NUM_INTERRUPTS; i++)
    {

        /* Disable current interrupt */
        IntDisable(i-16);

        switch(i) {
            case 0:
            case 1:
#ifndef NUTDEBUG_RAM
                /* copy stackpointer and reset vector */
                g_pfnRAMVectors[i] = (void(*)(void*))g_pfnVectors[i];
#endif
                break;
            case 2:
                g_pfnRAMVectors[i] = (void(*)(void*))IntNmiHandler;
                break;
            case 3:
                g_pfnRAMVectors[i] = &IntHardfaultHandler;
                break;
            case 4:
                g_pfnRAMVectors[i] = (void(*)(void*))IntMemfaultHandler;
                IntEnable(i-16);
                break;
            case 5:
                g_pfnRAMVectors[i] = (void(*)(void*))IntBusfaultHandler;
                IntEnable(i-16);
                break;
            case 6:
                g_pfnRAMVectors[i] = (void(*)(void*))IntUsagefaultHandler;
                IntEnable(i-16);
                break;
            case 7:
            case 8:
            case 9:
            case 10:
            case 13:
                /* Not supported interrupts are cleared */
                g_pfnRAMVectors[i] = NULL;
                break;
#if 0
            case 11:
                g_pfnRAMVectors[i] = &IntDefaultHandler;
                break;
            case 12:
                g_pfnRAMVectors[i] = &IntDefaultHandler;
                break;
            case 14:
                g_pfnRAMVectors[i] = &IntDefaultHandler;
                break;
            case 15:
                g_pfnRAMVectors[i] = &IntDefaultHandler;
                break;
#endif
            default:
                g_pfnRAMVectors[i] = &IntDefaultHandler;
                break;

        }
    }

    /* Basic interrupt system setup */
//    SCB->AIRCR = (0x05fa0000|SCB_AIRCR_PRIGROUP0);
    SCB->ICSR |= (SCB_ICSR_PENDSTCLR_Msk | SCB_ICSR_PENDSVCLR_Msk | SCB_ICSR_ISRPENDING_Msk);

    /* Point NVIC at the RAM vector table. */
    SCB->VTOR = (uint32_t)g_pfnRAMVectors;
    __enable_fault_irq();
    __enable_irq();
}


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
