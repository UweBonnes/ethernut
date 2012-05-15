//*****************************************************************************
//
// sysctl.h - Prototypes for the system control driver.
//
//*****************************************************************************

#ifndef __SYSCTL_H__
#define __SYSCTL_H__

#ifdef STM32F105
#include <arch/arm/stm32f10x_rcc.h>
#include <arch/arm/stm32f10x_flash.h>
#endif

extern void SysCtlClockSet(unsigned long ulConfig);
extern unsigned long SysCtlClockGet(void);

#endif // __SYSCTL_H__
