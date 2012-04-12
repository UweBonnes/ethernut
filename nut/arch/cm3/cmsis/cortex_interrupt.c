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
#include <arch/cm3/cortex_interrupt.h>
#include <sys/nutdebug.h>
#include <sys/timer.h>
#include <sys/atom.h>

//#define OWN_EXCEPTION_HANDLER

#ifdef OWN_EXCEPTION_HANDLER
#include <stdio.h>
#include <string.h>

const char s_fault[] = "\n*** HardFault ***\n";
const char s_fbus[] = "[BusFault]\n";
const char s_fmem[] = "[MemManageFault]\n";
const char s_fuse[] = "[UsageFault]\n";
const char s_sirq[] = "[UnregIRQ]\n";

static void IntDefaultHandler(void *arg);

static void mprintf( char *s)
{
    // Disable transmit interrupts.
    USART1->CR1  &= ~(USART_CR1_TXEIE|USART_CR1_TCIE);

    while(*s) {
        while( (USART1->SR & USART_SR_TXE) == 0);
        if (*s=='\n') {
            USART1->DR = '\r';
            while( (USART1->SR & USART_SR_TXE) == 0);
        }
        USART1->DR = *s++;
    }
}

/* Hardfault Handler.
 * hard fault handler in C, with stack frame location as input parameter
 */

static void IntFaultEntry(void *arg)
{
    asm volatile (
        "tst lr, #4 \n\t"
        "ite eq \n\t"
        "mrseq r0, MSP \n\t"
        "mrsne r0, PSP \n\t"
        "/* check that r0 is a valid stack */\n\t"
        "/* pointer to avoid a second fault */\n\t"
        "tst r0, #3 \n\t"
        "bne skip \n\t"
        "ldr r1, =_stack_start \n\t"
        "cmp r0, r1 \n\t"
        "bmi skip \n\t"
        "ldr r1, =_stack_end - 32 \n\t"
        "cmp r0, r1 \n\t"
        "bpl skip \n\t"
        "ldr r1, [r0, #24] /* r1 <= stacked PC */\n\t"
        "skip: \n\t"
        "b IntDefaultHandler \n\t"
    );
}
#endif
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

#ifdef OWN_EXCEPTION_HANDLER
    int i;
    unsigned int *hardfault_args = arg;
    unsigned int stacked_r0;
    unsigned int stacked_r1;
    unsigned int stacked_r2;
    unsigned int stacked_r3;
    unsigned int stacked_r12;
    unsigned int stacked_lr;
    unsigned int stacked_pc;
    unsigned int stacked_psr;
    unsigned int cfsr = (*((volatile unsigned long *)(0xE000ED28)));
    char s[40];

    mprintf( (char*)s_fault);
    if (cfsr & SCB_CFSR_MEMFAULTSR_Msk) {
        mprintf( (char*)s_fmem);
    }
    else if (cfsr & SCB_CFSR_BUSFAULTSR_Msk) {
        mprintf( (char*)s_fbus);
    }
    else if (cfsr & SCB_CFSR_USGFAULTSR_Msk) {
        mprintf( (char*)s_fuse);
    }
    else {
        mprintf( (char*)s_sirq);
        sprintf ( s, "VTOR = %08lx\n", (*((volatile unsigned long *)(SCB->VTOR))));
        mprintf ( s);
        sprintf ( s, "ICSR = %08lx\n", (*((volatile unsigned long *)(SCB->ICSR))));
        mprintf ( s);
    }

    stacked_r0 = ((unsigned long) hardfault_args[0]);
    stacked_r1 = ((unsigned long) hardfault_args[1]);
    stacked_r2 = ((unsigned long) hardfault_args[2]);
    stacked_r3 = ((unsigned long) hardfault_args[3]);

    stacked_r12 = ((unsigned long) hardfault_args[4]);
    stacked_lr = ((unsigned long) hardfault_args[5]);
    stacked_pc = ((unsigned long) hardfault_args[6]);
    stacked_psr = ((unsigned long) hardfault_args[7]);

    sprintf ( s, "R0  = %08x\n", stacked_r0);
    mprintf ( s);
    sprintf ( s, "R1  = %08x\n", stacked_r1);
    mprintf ( s);
    sprintf ( s, "R2  = %08x\n", stacked_r2);
    mprintf ( s);
    sprintf ( s, "R3  = %08x\n", stacked_r3);
    mprintf ( s);
    sprintf ( s, "R12 = %08x\n", stacked_r12);
    mprintf ( s);
    sprintf ( s, "LR  = %08x\n", stacked_lr);
    mprintf ( s);
    sprintf ( s, "PC  = %08x\n", stacked_pc);
    mprintf ( s);
    sprintf ( s, "PSR = %08x\n\n", stacked_psr);
    mprintf ( s);
    sprintf ( s, "BFAR = %08lx\n", (*((volatile unsigned long *)(0xE000ED38))));
    mprintf ( s);
    sprintf ( s, "CFSR = %08x\n", cfsr);
    mprintf ( s);
    sprintf ( s, "HFSR = %08lx\n", (*((volatile unsigned long *)(0xE000ED2C))));
    mprintf ( s);
    sprintf ( s, "DFSR = %08lx\n", (*((volatile unsigned long *)(0xE000ED30))));
    mprintf ( s);
    sprintf ( s, "AFSR = %08lx\n", (*((volatile unsigned long *)(0xE000ED3C))));
    mprintf ( s);

    while(1) {
        for(i=0; i<100000; i++)
            GPIOB->BSRR = 0x0001;
        for(i=0; i<100000; i++)
            GPIOB->BRR = 0x0001;
    };
#else
    while(1);
#endif
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
//! Enables the processor interrupt.
//!
//! Allows the processor to respond to interrupts.  This does not affect the
//! set of interrupts enabled in the interrupt controller; it just gates the
//! single interrupt from the controller to the processor.
//!
//! \return Returns \b true if interrupts were disabled when the function was
//! called or \b false if they were initially enabled.
//
//*****************************************************************************
unsigned char IntMasterEnable(void)
{
    return(CPUcpsie());
}

//*****************************************************************************
//
//! Disables the processor interrupt.
//!
//! Prevents the processor from receiving interrupts.  This does not affect the
//! set of interrupts enabled in the interrupt controller; it just gates the
//! single interrupt from the controller to the processor.
//!
//! \return Returns \b true if interrupts were already disabled when the
//! function was called or \b false if they were initially enabled.
//
//*****************************************************************************
unsigned char IntMasterDisable(void)
{
    return(CPUcpsid());
}

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
//! Sets the priority grouping of the interrupt controller.
//!
//! \param ulBits specifies the number of bits of preemptable priority.
//!
//! This function specifies the split between preemptable priority levels and
//! subpriority levels in the interrupt priority specification.  The range of
//! the grouping values are dependent upon the hardware implementation; on
//! the Stellaris family, three bits are available for hardware interrupt
//! prioritization and therefore priority grouping values of three through
//! seven have the same effect.
//!
//! \return None.
//
//*****************************************************************************
/*void
IntPriorityGroupingSet(unsigned long ulBits)
{
    //
    // Check the arguments.
    //
    NUTASSERT(ulBits < NUM_PRIORITY);

    //
    // Set the priority grouping.
    //
#if defined(SAM3U)
    outr(AT91C_NVIC_AIRCR, AT91C_NVIC_VECTKEY | g_pulPriority[ulBits]);
#endif
#if defined(MCU_STM32F10X)
    outr(SCB->AIRCR, NVIC_AIRCR_VECTKEY | g_pulPriority[ulBits]);
#endif
}*/

//*****************************************************************************
//
//! Gets the priority grouping of the interrupt controller.
//!
//! This function returns the split between preemptable priority levels and
//! subpriority levels in the interrupt priority specification.
//!
//! \return The number of bits of preemptable priority.
//
//*****************************************************************************
/*unsigned long
IntPriorityGroupingGet(void)
{
    unsigned long ulLoop, ulValue;

    //
    // Read the priority grouping.
    //
#if defined(SAM3U)
    ulValue = inr(AT91C_NVIC_AIRCR) & AT91C_NVIC_PRIGROUP;
#endif
#if defined(MCU_STM32F10X)
    ulValue = inr(SCB->AIRCR) & 0x0700;
#endif

    //
    // Loop through the priority grouping values.
    //
    for(ulLoop = 0; ulLoop < NUM_PRIORITY; ulLoop++)
    {
        //
        // Stop looping if this value matches.
        //
        if(ulValue == g_pulPriority[ulLoop])
        {
            break;
        }
    }

    //
    // Return the number of priority bits.
    //
    return(ulLoop);
}*/

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


void CortexExceptHandler(int state)
{
#ifdef OWN_EXCEPTION_HANDLER
    uint32_t control;

    /* Set privileged mode */
    control = __get_CONTROL();
    __set_CONTROL(0x0);
    if (state) {
        SCB->SHCSR |= (SCB_SHCSR_MEMFAULTENA|SCB_SHCSR_BUSFAULTENA|SCB_SHCSR_USGFAULTENA);
    }
    else {
        SCB->SHCSR &= ~(SCB_SHCSR_MEMFAULTENA|SCB_SHCSR_BUSFAULTENA|SCB_SHCSR_USGFAULTENA);
    }

    /* Set back Thread mode */
    __set_CONTROL(control);
#endif
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
