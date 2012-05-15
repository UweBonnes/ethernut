//*****************************************************************************
//
// systick.h - Prototypes for the SysTick driver.
//
//*****************************************************************************

#ifndef __CORTEX_CLK_H__
#define __CORTEX_CLK_H__

extern void SysTick_Handler(void);
//*****************************************************************************
//
// Prototypes for the common APIs.
//
//*****************************************************************************

extern uint32_t SystemCoreClock;

/* Auto-configuration of the clocks and prescalers */
extern int SetSysClock(void);

/* Functions to retrieve clock information */
extern void SystemCoreClockUpdate(void);
extern uint32_t SysCtlClockGet(void);


#endif // __CORTEX_CLK_H__
