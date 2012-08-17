//*****************************************************************************
//
// sysctl.c - Driver for the system controller.
//
// Copyright (c) 2005-2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 8555 of the Stellaris Peripheral Driver Library.
//
//*****************************************************************************

//*****************************************************************************
//
// cortex_sysctl.c - Driver for the system controller on cortex-m3 sam3u
//
//
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup sysctl_api
//! @{
//
//*****************************************************************************
#include <cfg/arch.h>
#include <arch/cm3.h>
#include <sys/nutdebug.h>

#if defined(MCU_STM32)
#include <arch/cm3/system_stm32f10x.h>
#else
#include <arch/cm3/cortex_sysctl.h>
#endif

//*****************************************************************************
//
//! Sets the clocking of the device.
//!
//! \param ulConfig is the required configuration of the device clocking.
//!
//! This function configures the clocking of the device.  The input crystal
//! frequency, oscillator to be used, use of the PLL, and the system clock
//! divider are all configured with this function.
//!
//! \bug ulConfig ignored
//!
//! \return None.
//
//*****************************************************************************
#if defined(SAM3U)
void SysCtlClockSet( unsigned long ulConfig )
{
  unsigned int timeout;
  CONST unsigned int CLOCK_TIMEOUT = 0xFFFFFFFF;
  CONST unsigned int AT91C_CKGR_MUL_SHIFT = 16;
  CONST unsigned int AT91C_CKGR_OUT_SHIFT = 14;
  CONST unsigned int AT91C_CKGR_PLLCOUNT_SHIFT = 8;
  CONST unsigned int AT91C_CKGR_DIV_SHIFT = 0;
  CONST unsigned int BOARD_OSCOUNT =
    ( AT91C_CKGR_MOSCXTST & ( 0x3F << 8 ) );
  CONST unsigned int BOARD_PLLR =
    ( ( 1 << 29 ) | ( 0xf << AT91C_CKGR_MUL_SHIFT )
      | ( 0x0 << AT91C_CKGR_OUT_SHIFT ) | ( 0x3f <<
                                            AT91C_CKGR_PLLCOUNT_SHIFT )
      | ( 0x1 << AT91C_CKGR_DIV_SHIFT ) );
  CONST unsigned int BOARD_MCKR =
    ( AT91C_PMC_PRES_CLK_2 | AT91C_PMC_CSS_PLLA_CLK );

  /* Select external slow clock
   ****************************/
  if( ( inr( AT91C_SUPC_SR ) & AT91C_SUPC_SR_OSCSEL_CRYST ) !=
      AT91C_SUPC_SR_OSCSEL_CRYST ) {
    outr( AT91C_SUPC_CR,
          AT91C_SUPC_CR_XTALSEL_CRYSTAL_SEL | ( 0xA5UL << 24UL ) );
    timeout = 0;
    while( !( inr( AT91C_SUPC_SR ) & AT91C_SUPC_SR_OSCSEL_CRYST )
           && ( timeout++ < CLOCK_TIMEOUT ) );
  }
  /* Initialize main oscillator
   ****************************/

  if( !( inr( AT91C_PMC_MOR ) & AT91C_CKGR_MOSCSEL ) ) {
    outr( AT91C_PMC_MOR,
          ( 0x37 << 16 ) | BOARD_OSCOUNT | AT91C_CKGR_MOSCRCEN |
          AT91C_CKGR_MOSCXTEN );
    timeout = 0;
    while( !( inr( AT91C_PMC_SR ) & AT91C_PMC_MOSCXTS )
           && ( timeout++ < CLOCK_TIMEOUT ) );
  }
  else {
    outr( AT91C_PMC_MOR,
          ( 0x37 << 16 ) | BOARD_OSCOUNT | AT91C_CKGR_MOSCRCEN |
          AT91C_CKGR_MOSCXTEN | AT91C_CKGR_MOSCSEL );
    timeout = 0;
    while( !( inr( AT91C_PMC_SR ) & AT91C_PMC_MOSCRCS )
           && ( timeout++ < CLOCK_TIMEOUT ) );
    outr( AT91C_PMC_MOR,
          ( 0x37 << 16 ) | BOARD_OSCOUNT | AT91C_CKGR_MOSCRCEN |
          AT91C_CKGR_MOSCXTEN );
    timeout = 0;
    while( !( inr( AT91C_PMC_SR ) & AT91C_PMC_MOSCSELS )
           && ( timeout++ < CLOCK_TIMEOUT ) );
  }

  /* Switch to moscsel */
  outr( AT91C_PMC_MOR,
        ( 0x37 << 16 ) | BOARD_OSCOUNT | AT91C_CKGR_MOSCRCEN |
        AT91C_CKGR_MOSCXTEN | AT91C_CKGR_MOSCSEL );
  timeout = 0;
  while( !( inr( AT91C_PMC_SR ) & AT91C_PMC_MOSCSELS )
         && ( timeout++ < CLOCK_TIMEOUT ) );
  outr( AT91C_PMC_MCKR,
        ( inr( AT91C_PMC_MCKR ) & ~AT91C_PMC_CSS ) |
        AT91C_PMC_CSS_MAIN_CLK );
  timeout = 0;
  while( !( inr( AT91C_PMC_SR ) & AT91C_PMC_MCKRDY )
         && ( timeout++ < CLOCK_TIMEOUT ) );

  /* Initialize PLLA */
  outr( AT91C_PMC_PLLAR, BOARD_PLLR );
  timeout = 0;
  while( !( inr( AT91C_PMC_SR ) & AT91C_PMC_LOCKA )
         && ( timeout++ < CLOCK_TIMEOUT ) );

  /* Initialize UTMI for USB usage */
  orr( AT91C_CKGR_UCKR,
       ( AT91C_CKGR_UPLLCOUNT & ( 3 << 20 ) ) | AT91C_CKGR_UPLLEN );
  timeout = 0;
  while( !( inr( AT91C_PMC_SR ) & AT91C_PMC_LOCKU )
         && ( timeout++ < CLOCK_TIMEOUT ) );

  /* Switch to fast clock
   **********************/
  outr( AT91C_PMC_MCKR,
        ( BOARD_MCKR & ~AT91C_PMC_CSS ) | AT91C_PMC_CSS_MAIN_CLK );
  timeout = 0;
  while( !( inr( AT91C_PMC_SR ) & AT91C_PMC_MCKRDY )
         && ( timeout++ < CLOCK_TIMEOUT ) );

  outr( AT91C_PMC_MCKR, BOARD_MCKR );
  timeout = 0;
  while( !( inr( AT91C_PMC_SR ) & AT91C_PMC_MCKRDY )
         && ( timeout++ < CLOCK_TIMEOUT ) );
}

//*****************************************************************************
//
//! Gets the processor clock rate.
//!
//! \return The processor clock rate.
//
//*****************************************************************************
unsigned long SysCtlClockGet( void )
{
  unsigned long ulClk;

  /* this is a dirty hack and should be rewritten */
  ulClk = inr( AT91C_CKGR_MCFR ) & AT91C_CKGR_MAINF;    /* reading frequency counter */
  if( inr( AT91C_CKGR_PLLAR ) & AT91C_CKGR_MULA ) {     //PLL enabled
    ulClk = ulClk * ( ( inr( AT91C_CKGR_PLLAR ) & AT91C_CKGR_MULA ) >> 16 ) / ( inr( AT91C_CKGR_PLLAR ) & AT91C_CKGR_DIVA );    /* is this correct? */
  };

  return ( ulClk );

  /* end of dirty hack */
};

#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
