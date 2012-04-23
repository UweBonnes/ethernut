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

#include <arch/arm.h>
#include <sys/nutdebug.h>
#include <arch/arm/cortex_sysctl.h>

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
#if defined(SAM3U_EK)
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
#endif
#ifdef STM32F105
void SysCtlClockSet( unsigned long ulConfig ){
	uint32_t temp;
	//Включить RC генератор и дождаться его стабильности
	RCC->CR |= HSION;
	while( (RCC->CR & HSIRDY)==0)
		continue;
	//Выбрать RC генератор в качестве системного
	temp= RCC->CFGR;
	temp &= ~((1<<1)|(1<<0));
	temp |= RCC_SYSCLKSource_HSI;
	RCC->CFGR=temp;
	//Включить внешний кварцевый генератор
	//Дождаться его стабильности и переключится на него
	RCC->CR |= HSEON;
	while( (RCC->CR & HSERDY)==0)
		continue;
	temp= RCC->CFGR;
	temp &= ~((1<<1)|(1<<0));
	temp |= RCC_SYSCLKSource_HSE;
	RCC->CFGR=temp;
	//Установить делители для шин и периферии
	//AHB
	temp= RCC->CFGR;
	temp &= ~((1<<4)|(1<<5)|(1<<6)|(1<<7));
	temp |= RCC_SYSCLK_Div1;//Max 72Mhz
	RCC->CFGR=temp;
	//ADC
	temp= RCC->CFGR;
	temp &= ~((1<<14)|(1<<15));
	temp |= RCC_PCLK2_Div8;//max 14Mhz TODO: add clock calculation
	RCC->CFGR=temp;
	//ABP1
	temp= RCC->CFGR;
	temp &= ~((1<<8)|(1<<9)|(1<<10));
	if(ulConfig<=36000000){
		temp |= RCC_HCLK_Div1;//Max 36Mhz
	}else{
		temp |= RCC_HCLK_Div2;//Max 36Mhz
	};
	RCC->CFGR=temp;
	//ABP2
	temp = RCC->CFGR;
	temp &= ~((1<<11)|(1<<12)|(1<<13));
	temp |= RCC_HCLK_Div1 << 3;//Max 72Mhz
	RCC->CFGR=temp;
	//Настроить состояния ожидания flash-а
	temp=FLASH->ACR;
	temp &= ~((1<<0)|(1<<1)|(1<<2));
	if(ulConfig < 24000000){
		temp |= FLASH_Latency_0;
	}else if(ulConfig < 48000000){
		temp |= FLASH_Latency_1;
	}else if(ulConfig <= 72000000){
		temp |= FLASH_Latency_2;
	}else {
		temp |= FLASH_Latency_2;
	};
	FLASH->ACR=temp|FLASH_PrefetchBuffer_Enable;
	//Настроить множитель pll-а
	temp=RCC->CFGR;
	temp &= ~((1<<21)|(1<<20)|(1<<19)|(1<<18));
	if(ulConfig<36000000){
		temp |= RCC_PLLMul_4;
	}else if(ulConfig<44000000){
		temp |= RCC_PLLMul_5;
	}else if(ulConfig<50000000){
		temp |= RCC_PLLMul_6;
	}else if(ulConfig<54000000){
		temp |= RCC_PLLMul_6_5;
	}else if(ulConfig<58000000){
		temp |= RCC_PLLMul_7;
	}else if(ulConfig<66000000){
		temp |= RCC_PLLMul_8;
	}else if(ulConfig<=72000000){
		temp |= RCC_PLLMul_9;
	}else{
		temp |= RCC_PLLMul_9;//TODO: сделать управление pll-ем
	};
	RCC->CFGR=temp;
	//Подключить кварц как источник для pll и выставить делитель в 1
	temp=RCC->CFGR;
	temp &= ~((1<<16)|(1<<17));
	temp |= RCC_PLLSource_PREDIV1;
	RCC->CFGR=temp;
	//Включить pll
	RCC->CR |= PLLON;
	//Дождаться стабилизации pll
	while ((RCC->CR & PLLRDY)==0)
		continue;
	//Переключить clock на pll
	temp=RCC->CFGR;
	temp &= ~((1<<1)|(1<<0));
	temp |= RCC_SYSCLKSource_PLLCLK;
	RCC->CFGR=temp;
};
#endif
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

#if defined(SAM3U_EK)
  /* this is a dirty hack and should be rewritten */
  ulClk = inr( AT91C_CKGR_MCFR ) & AT91C_CKGR_MAINF;    /* reading frequency counter */
  if( inr( AT91C_CKGR_PLLAR ) & AT91C_CKGR_MULA ) {     //PLL enabled
    ulClk = ulClk * ( ( inr( AT91C_CKGR_PLLAR ) & AT91C_CKGR_MULA ) >> 16 ) / ( inr( AT91C_CKGR_PLLAR ) & AT91C_CKGR_DIVA );    /* is this correct? */
  };

  /* end of dirty hack */
#endif
#ifdef STM32F105
  switch(RCC->CFGR & ((1<<21)|(1<<20)|(1<<19)|(1<<18))){
	  case RCC_PLLMul_4:
		  ulClk = 8000000*4;
		  break;
	  case RCC_PLLMul_5:
		  ulClk = 8000000*5;
		  break;
	  case RCC_PLLMul_6:
		  ulClk = 8000000*6;
		  break;
	  case RCC_PLLMul_7:
		  ulClk = 8000000*7;
		  break;
	  case RCC_PLLMul_8:
		  ulClk = 8000000*8;
		  break;
	  case RCC_PLLMul_9:
		  ulClk = 8000000*9;
		  break;
	  case RCC_PLLMul_6_5:
  		  ulClk = 8000000*6.5;
		  break;
	  default:
		  ulClk = 8000000;
		  break;
  };
#endif

  //
  // Return the computed clock rate.
  //
  return ( ulClk );

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
