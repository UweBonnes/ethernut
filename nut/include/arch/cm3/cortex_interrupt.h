//*****************************************************************************
//
// cortex_interrupt.h - Prototypes for the NVIC Interrupt Controller Driver.
//
//*****************************************************************************

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#if defined(SAM3U)
#define NUM_INTERRUPTS       48
#endif
#if defined(MCU_STM32)
#define NUM_INTERRUPTS      121
#endif
#if defined(MCU_LPC176x)
#define NUM_INTERRUPTS       35
#endif
#if defined(MCU_LPC177x_8x)
#define NUM_INTERRUPTS       41
#endif

//*****************************************************************************
//
// The following are defines for the total number of priority levels.
//
//*****************************************************************************
#define NUM_PRIORITY            8
#define NUM_PRIORITY_BITS       3
#ifdef __cplusplus
extern "C"
{
#endif


//*****************************************************************************
//
// Macro to generate an interrupt priority mask based on the number of bits
// of priority supported by the hardware.
//
//*****************************************************************************
#define INT_PRIORITY_MASK       ((0xFF << (8 - NUM_PRIORITY_BITS)) & 0xFF)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern unsigned long CPUcpsid(void);
extern unsigned long CPUcpsie(void);

extern unsigned char IntMasterEnable(void);
extern unsigned char IntMasterDisable(void);

extern void IntRegister(IRQn_Type ulInterrupt, void (*pfnHandler)(void*));
extern void IntUnregister(IRQn_Type ulInterrupt);

/*extern void IntPriorityGroupingSet(unsigned long ulBits);
extern unsigned long IntPriorityGroupingGet(void);*/
extern void IntPrioritySet(IRQn_Type Interrupt, uint32_t Priority);
extern uint32_t IntPriorityGet(IRQn_Type ulInterrupt);

extern void IntEnable(IRQn_Type ulInterrupt);
extern void IntDisable(IRQn_Type ulInterrupt);
extern int IntIsEnabled(IRQn_Type ulInterrupt);

#ifdef __cplusplus
}
#endif

#endif // __INTERRUPT_H__
