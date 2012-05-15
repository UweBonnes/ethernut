//*****************************************************************************
//
// systick.h - Prototypes for the SysTick driver.
//
//*****************************************************************************

#ifndef __SYSTICK_H__
#define __SYSTICK_H__

extern void SysTick_Handler(void);
//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void SysTickEnable(void);
extern void SysTickDisable(void);
extern void SysTickIntRegister(void (*pfnHandler)(void*));
extern void SysTickIntUnregister(void);
extern void SysTickIntEnable(void);
extern void SysTickIntDisable(void);
extern void SysTickPeriodSet(unsigned long ulPeriod);
extern unsigned long SysTickPeriodGet(void);
extern unsigned long SysTickValueGet(void);

#endif // __SYSTICK_H__
