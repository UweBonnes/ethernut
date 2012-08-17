/*****************************************************************************
//
// systick.c - Driver for the SysTick timer in NVIC.
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
// This is part of revision 3223 of the Stellaris Peripheral Driver Library.
//
//*****************************************************************************

//*****************************************************************************
//
// systick.c - Driver for the SysTick timer in NVIC.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup systick_api
//! @{
//
//*****************************************************************************

#include <sys/nutdebug.h>
#include <cfg/arch.h>
#include <arch/cm3.h>
#if defined(MCU_STM32F4)
#include <arch/cm3/core_cm4.h>
#else
#include <arch/cm3/core_cm3.h>
#endif
#include <arch/cm3/cortex_interrupt.h>
#include <arch/cm3/cortex_systick.h>

//*****************************************************************************
//
//! Enables the SysTick counter.
//!
//! This will start the SysTick counter.  If an interrupt handler has been
//! registered, it will be called when the SysTick counter rolls over.
//!
//! \note Calling this function will cause the SysTick counter to (re)commence
//! counting from its current value. The counter is not automatically reloaded
//! with the period as specified in a previous call to SysTickPeriodSet().  If
//! an immediate reload is required, the \b NVIC_ST_CURRENT register must be
//! written to force this.  Any write to this register clears the SysTick
//! counter to 0 and will cause a reload with the supplied period on the next
//! clock.
//!
//! \return None.
//
//*****************************************************************************
void SysTickEnable(void)
{
    //
    // Enable SysTick.
    //
    // TODO: These are part of cmsis -> they are the same address and bit
#if defined(SAM3U)
    orr(AT91C_NVIC_STICKCSR, AT91C_NVIC_STICKCLKSOURCE|AT91C_NVIC_STICKENABLE);
#else
    SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);
#endif

}

//*****************************************************************************
//
//! Disables the SysTick counter.
//!
//! This will stop the SysTick counter.  If an interrupt handler has been
//! registered, it will no longer be called until SysTick is restarted.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickDisable(void)
{
    //
    // Disable SysTick.
    //
    // TODO: These are part of cmsis -> they are the same address and bit
#if defined(SAM3U)
    andr(AT91C_NVIC_STICKCSR, ~(AT91C_NVIC_STICKENABLE));
#else
    SysTick->CTRL&= ~(SysTick_CTRL_ENABLE_Msk);
#endif
}

//*****************************************************************************
//
//! Registers an interrupt handler for the SysTick interrupt.
//!
//! \param pfnHandler is a pointer to the function to be called when the
//! SysTick interrupt occurs.
//!
//! This sets the handler to be called when a SysTick interrupt occurs.
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void SysTickIntRegister(void (*pfnHandler)(void*))
{
    /* Register the interrupt handler, returning an error if an error occurs. */
    IntRegister(SysTick_IRQn, pfnHandler);

    /* Enable the SysTick interrupt. */
    // TODO: These are part of cmsis -> they are the same address and bit
#if defined(SAM3U)
    orr(AT91C_NVIC_STICKCSR, AT91C_NVIC_STICKINT);
#else
    SysTick->CTRL |= (SysTick_CTRL_TICKINT_Msk);
#endif
}

//*****************************************************************************
//
//! Unregisters the interrupt handler for the SysTick interrupt.
//!
//! This function will clear the handler to be called when a SysTick interrupt
//! occurs.
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickIntUnregister(void)
{
    //
    // Disable the SysTick interrupt.
    //
    // TODO: These are part of cmsis -> they are the same address and bit
#if defined(SAM3U)
    andr(AT91C_NVIC_STICKCSR, ~(AT91C_NVIC_STICKINT));
#else
    SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);
#endif

    //
    // Unregister the interrupt handler.
    //
    IntUnregister(SysTick_IRQn);
}

//*****************************************************************************
//
//! Enables the SysTick interrupt.
//!
//! This function will enable the SysTick interrupt, allowing it to be
//! reflected to the processor.
//!
//! \note The SysTick interrupt handler does not need to clear the SysTick
//! interrupt source as this is done automatically by NVIC when the interrupt
//! handler is called.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickIntEnable(void)
{
    //
    // Enable the SysTick interrupt.
    //
    // TODO: These are part of cmsis -> they are the same address and bit
#if defined(SAM3U)
    orr(AT91C_NVIC_STICKCSR, AT91C_NVIC_STICKINT);
#else
    SysTick->CTRL |= (SysTick_CTRL_TICKINT_Msk);
#endif
}

//*****************************************************************************
//
//! Disables the SysTick interrupt.
//!
//! This function will disable the SysTick interrupt, preventing it from being
//! reflected to the processor.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickIntDisable(void)
{
    //
    // Disable the SysTick interrupt.
    //
    // TODO: These are part of cmsis -> they are the same address and bit
#if defined(SAM3U)
    andr(AT91C_NVIC_STICKCSR, ~(AT91C_NVIC_STICKINT));
#else
    SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);
#endif
}

//*****************************************************************************
//
//! Sets the period of the SysTick counter.
//!
//! \param ulPeriod is the number of clock ticks in each period of the SysTick
//! counter; must be between 1 and 16,777,216, inclusive.
//!
//! This function sets the rate at which the SysTick counter wraps; this
//! equates to the number of processor clocks between interrupts.
//!
//! \note Calling this function does not cause the SysTick counter to reload
//! immediately.  If an immediate reload is required, the \b NVIC_ST_CURRENT
//! register must be written.  Any write to this register clears the SysTick
//! counter to 0 and will cause a reload with the \e ulPeriod supplied here on
//! the next clock after the SysTick is enabled.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickPeriodSet(unsigned long ulPeriod)
{
    //
    // Check the arguments.
    //
    NUTASSERT((ulPeriod > 0) && (ulPeriod <= 16777216));

    //
    // Set the period of the SysTick counter.
    //
    // TODO: These are part of cmsis -> they are the same address and bit
#if defined(SAM3U)
    outr(AT91C_NVIC_STICKRVR, ulPeriod - 1);
#else
    SysTick->LOAD = ulPeriod - 1;
#endif
}

//*****************************************************************************
//
//! Gets the period of the SysTick counter.
//!
//! This function returns the rate at which the SysTick counter wraps; this
//! equates to the number of processor clocks between interrupts.
//!
//! \return Returns the period of the SysTick counter.
//
//*****************************************************************************
unsigned long
SysTickPeriodGet(void)
{
    //
    // Return the period of the SysTick counter.
    //
    // TODO: These are part of cmsis -> they are the same address and bit
#if defined(SAM3U)
    return(inr(AT91C_NVIC_STICKRVR) + 1);
#else
    return(SysTick->LOAD+1);
#endif
}

//*****************************************************************************
//
//! Gets the current value of the SysTick counter.
//!
//! This function returns the current value of the SysTick counter; this will
//! be a value between the period - 1 and zero, inclusive.
//!
//! \return Returns the current value of the SysTick counter.
//
//*****************************************************************************
uint32_t SysTickValueGet(void)
{
    //
    // Return the current value of the SysTick counter.
    //
    // TODO: These are part of cmsis -> they are the same address and bit
#if defined(SAM3U)
    return(inr(AT91C_NVIC_STICKCVR));
#else
    return SysTick->VAL;
#endif
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
