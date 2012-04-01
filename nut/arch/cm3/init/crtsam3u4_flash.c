//*****************************************************************************
//
// startup.c - Boot code for atmel sam3u-ek.
//
//*****************************************************************************

#include <arch/arm.h>
void ResetISR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);


//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int NutInit(void);
extern void main(void);


//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
#ifndef STACK_SIZE
#define STACK_SIZE		1024
#endif
static unsigned long pulStack[STACK_SIZE];

extern unsigned long __exp_stack;
//*****************************************************************************
//
// The minimal vector table for a Cortex M3.  Note that the proper constructs
// must be placed on this to ensure that it ends up at physical address
// 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((unsigned long)pulStack + sizeof(pulStack)),
                                            // The initial stack pointer
    ResetISR,                               // The reset handler
    0,		                            // Reserved
    FaultISR,                               // The hard fault handler
    IntDefaultHandler,                      // The MPU fault handler
    IntDefaultHandler,                      // The bus fault handler
    IntDefaultHandler,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,                      // SVCall handler
    IntDefaultHandler,                      // Debug monitor handler
    0,                                      // Reserved
    IntDefaultHandler,                      // The PendSV handler
    IntDefaultHandler,                      // The SysTick handler
    IntDefaultHandler,                      // Supply Controller
    IntDefaultHandler,                      // Reset Controller
    IntDefaultHandler,                      // Real Time Clock
    IntDefaultHandler,                      // Real Time Timer
    IntDefaultHandler,                      // Watchdog Timer
    IntDefaultHandler,                      // Power Management Controller
    IntDefaultHandler,                      // Enhanced Embedded Flash Controller 0
    IntDefaultHandler,                      // Enhanced Embedded Flash Controller 1
    IntDefaultHandler,                      // UART
    IntDefaultHandler,                      // Static Memory Controller
    IntDefaultHandler,                      // PIOA
    IntDefaultHandler,                      // PIOB
    IntDefaultHandler,                      // PIOC
    IntDefaultHandler,                      // USART0
    IntDefaultHandler,                      // USART1
    IntDefaultHandler,                      // USART2
    IntDefaultHandler,                      // USART3
    IntDefaultHandler,                      // HS/MCI3
    IntDefaultHandler,                      // TWI 0
    IntDefaultHandler,                      // TWI 1
    IntDefaultHandler,                      // SPI
    IntDefaultHandler,                      // Syncronous Serial Controller
    IntDefaultHandler,                      // Timer Counter 0
    IntDefaultHandler,                      // Timer Counter 1
    IntDefaultHandler,                      // Timer Counter 2
    IntDefaultHandler,                      // PWM Controller
    IntDefaultHandler,                      // 12-bit ADC Controller
    IntDefaultHandler,                      // 10-bit ADC Controller
    IntDefaultHandler,                      // DMA Controller
    IntDefaultHandler                      // USB Device High Speed Device
};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied main() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void
ResetISR(void)
{
    unsigned long *pulSrc, *pulDest;
    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.  This is done with inline assembly since this
    // will clear the value of pulDest if it is not kept in a register.
    //

    __asm("    ldr     r0, =_bss\n"
          "    ldr     r1, =_ebss\n"
          "    mov     r2, #0\n"
          "    .thumb_func\n"
          "zero_loop:\n"
          "        cmp     r0, r1\n"
          "        it      lt\n"
          "        strlt   r2, [r0], #4\n"
          "        blt     zero_loop");


    /*
         * Jump to Nut/OS initialization.
         */
    __asm("		ldr     r0, =NutInit\n"
         "		bx      r0");
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
FaultISR(void)
{
	int ulLoop;
	LEDS_INIT();
	LED_RED_ON();
    //
    // Enter an infinite loop.
    //
    for (;;)
	{
        for(ulLoop = 0; ulLoop < 900000; ulLoop++)
		LED_RED_OFF();
        for(ulLoop = 0; ulLoop < 900000; ulLoop++)
		LED_RED_ON();
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
IntDefaultHandler(void)
{
	int ulLoop;
	LEDS_INIT();
	LED_RED_ON();
    //
    // Go into an infinite loop.
    //
    for (;;)
	{
        for(ulLoop = 0; ulLoop < 90000; ulLoop++)
		LED_RED_ON();
        for(ulLoop = 0; ulLoop < 20000; ulLoop++)
		LED_RED_OFF();
    }
}
