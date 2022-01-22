/*
 * Copyright (C) 2012 by Rob van Lieshout (info@pragmalab.nl)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 *
 **************************************************************************
 *
 * Parts taken from lpc177x_8x_timer.c       2013-09-02
 *
 * file     lpc177x_8x_timer.c
 * brief    Contains all functions support for Timer firmware library
 *          on LPC177x_8x
 * version  2.0
 * date     02. September. 2013
 * author   NXP MCU SW Application Team
 *
 * Copyright(C) 2011, NXP Semiconductor
 * All rights reserved.
 *
 ***********************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors'
 * relevant copyright in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 **********************************************************************/

#include <cfg/arch.h>
#include <sys/timer.h>
#include <dev/gpio.h>
#include <dev/irqreg.h>

#include <arch/cm3/nxp/lpc177x_8x_clk.h>
#include <arch/cm3/nxp/lpc177x_8x_timer.h>

//#define NUTDEBUG

#ifdef NUTDEBUG
    #include <stdio.h>
#endif

/* Private Functions ---------------------------------------------------------- */

static uint32_t getPClock (void);
static uint32_t converUSecToVal (uint32_t usec);
//static uint32_t converPtrToTimeNum (LPC_TIM_TypeDef *TIMx);

/*********************************************************************//**
 * @brief 		Get peripheral clock of each timer controller
 * @param[in]	timernum Timer number
 * @return 		Peripheral clock of timer
 **********************************************************************/
static uint32_t getPClock ()
{
    uint32_t clkdlycnt;
//  clkdlycnt = CLKPWR_GetCLK(CLKPWR_CLKTYPE_PER);
    clkdlycnt = Lpc17xx_ClockGet(NUT_HWCLK_PCLK);

    return(clkdlycnt);
}

/*********************************************************************//**
 * @brief 		Convert a time to a timer count value
 * @param[in]	timernum Timer number
 * @param[in]	usec Time in microseconds
 * @return 		The number of required clock ticks to give the time delay
 **********************************************************************/
uint32_t converUSecToVal (uint32_t usec)
{
//    uint64_t clkdlycnt;
    uint32_t clkdlycnt;

    // Get Pclock of timer
    clkdlycnt = getPClock();
    clkdlycnt = (clkdlycnt * usec) / 1000000;

    return (clkdlycnt);
}

#if 0
/*********************************************************************//**
 * @brief 		Convert a timer register pointer to a timer number
 * @param[in]	TIMx Pointer to LPC_TIM_TypeDef, should be:
 * 				- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @return 		The timer number (0 to 3) or -1 if register pointer is bad
 **********************************************************************/
uint32_t converPtrToTimeNum (LPC_TIM_TypeDef *TIMx)
{
    uint32_t tnum = -1;

    if (TIMx == LPC_TIM0)
    {
        tnum = 0;
    }
    else if (TIMx == LPC_TIM1)
    {
        tnum = 1;
    }
    else if (TIMx == LPC_TIM2)
    {
        tnum = 2;
    }
    else if (TIMx == LPC_TIM3)
    {
        tnum = 3;
    }

    return(tnum);
}
#endif

/* End of Private Functions ---------------------------------------------------- */

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup TIM_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief 		Get Interrupt Status
 * @param[in]	TIMx Timer selection, should be:
 *   			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @param[in]	IntFlag: interrupt type, should be:
 * 				- TIM_MR0_INT: Interrupt for Match channel 0
 * 				- TIM_MR1_INT: Interrupt for Match channel 1
 * 				- TIM_MR2_INT: Interrupt for Match channel 2
 * 				- TIM_MR3_INT: Interrupt for Match channel 3
 * 				- TIM_CR0_INT: Interrupt for Capture channel 0
 * 				- TIM_CR1_INT: Interrupt for Capture channel 1
 * @return 		FlagStatus
 * 				- SET : interrupt
 * 				- RESET : no interrupt
 **********************************************************************/
FlagStatus Lpc177x_8x_TIM_GetIntStatus(LPC_TIM_TypeDef *TIMx, TIM_INT_TYPE IntFlag)
{
    uint8_t temp;
    temp = (TIMx->IR)& TIM_IR_CLR(IntFlag);
    if (temp)
    {    
        return(SET);
    }

    return(RESET);

}
/*********************************************************************//**
 * @brief 		Get Capture Interrupt Status
 * @param[in]	TIMx Timer selection, should be:
 *  	   		- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @param[in]	IntFlag: interrupt type, should be:
 * 				- TIM_MR0_INT: Interrupt for Match channel 0
 * 				- TIM_MR1_INT: Interrupt for Match channel 1
 * 				- TIM_MR2_INT: Interrupt for Match channel 2
 * 				- TIM_MR3_INT: Interrupt for Match channel 3
 * 				- TIM_CR0_INT: Interrupt for Capture channel 0
 * 				- TIM_CR1_INT: Interrupt for Capture channel 1
 * @return 		FlagStatus
 * 				- SET : interrupt
 * 				- RESET : no interrupt
 **********************************************************************/
FlagStatus Lpc177x_8x_TIM_GetIntCaptureStatus(LPC_TIM_TypeDef *TIMx, TIM_INT_TYPE IntFlag)
{
    uint8_t temp;
    temp = (TIMx->IR) & (1<<(4+IntFlag));
    if (temp)
        return(SET);
    return(RESET);
}
/*********************************************************************//**
 * @brief 		Clear Interrupt pending
 * @param[in]	TIMx Timer selection, should be:
 *    			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @param[in]	IntFlag: interrupt type, should be:
 * 				- TIM_MR0_INT: Interrupt for Match channel 0
 * 				- TIM_MR1_INT: Interrupt for Match channel 1
 * 				- TIM_MR2_INT: Interrupt for Match channel 2
 * 				- TIM_MR3_INT: Interrupt for Match channel 3
 * 				- TIM_CR0_INT: Interrupt for Capture channel 0
 * 				- TIM_CR1_INT: Interrupt for Capture channel 1
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_ClearIntPending(LPC_TIM_TypeDef *TIMx, TIM_INT_TYPE IntFlag)
{
    TIMx->IR = TIM_IR_CLR(IntFlag);
}

/*********************************************************************//**
 * @brief 		Clear Capture Interrupt pending
 * @param[in]	TIMx Timer selection, should be
 *    			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @param[in]	IntFlag interrupt type, should be:
 *				- TIM_MR0_INT: Interrupt for Match channel 0
 * 				- TIM_MR1_INT: Interrupt for Match channel 1
 * 				- TIM_MR2_INT: Interrupt for Match channel 2
 * 				- TIM_MR3_INT: Interrupt for Match channel 3
 * 				- TIM_CR0_INT: Interrupt for Capture channel 0
 * 				- TIM_CR1_INT: Interrupt for Capture channel 1
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_ClearIntCapturePending(LPC_TIM_TypeDef *TIMx, TIM_INT_TYPE IntFlag)
{
    TIMx->IR = (1<<(4+IntFlag));
}

/*********************************************************************//**
 * @brief 		Configuration for Timer at initial time
 * @param[in] 	TimerCounterMode timer counter mode, should be:
 * 				- TIM_TIMER_MODE: Timer mode
 * 				- TIM_COUNTER_RISING_MODE: Counter rising mode
 * 				- TIM_COUNTER_FALLING_MODE: Counter falling mode
 * 				- TIM_COUNTER_ANY_MODE:Counter on both edges
 * @param[in] 	TIM_ConfigStruct pointer to TIM_TIMERCFG_Type or
 * 				TIM_COUNTERCFG_Type
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_ConfigStructInit(TIM_MODE_OPT TimerCounterMode, void *TIM_ConfigStruct)
{
    if (TimerCounterMode == TIM_TIMER_MODE)
    {
        TIM_TIMERCFG_Type * pTimeCfg = (TIM_TIMERCFG_Type *)TIM_ConfigStruct;
        pTimeCfg->PrescaleOption = TIM_PRESCALE_USVAL;
        pTimeCfg->PrescaleValue = 1;
    }
    else
    {
        TIM_COUNTERCFG_Type * pCounterCfg = (TIM_COUNTERCFG_Type *)TIM_ConfigStruct;
        pCounterCfg->CountInputSelect = TIM_COUNTER_INCAP0;
    }
}

/*********************************************************************//**
 * @brief 		Initial Timer/Counter device
 * 				 	Set Clock frequency for Timer
 * 					Set initial configuration for Timer
 * @param[in]	TIMx  Timer selection, should be:
 * 				- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @param[in]	TimerCounterMode Timer counter mode, should be:
 * 				- TIM_TIMER_MODE: Timer mode
 * 				- TIM_COUNTER_RISING_MODE: Counter rising mode
 * 				- TIM_COUNTER_FALLING_MODE: Counter falling mode
 * 				- TIM_COUNTER_ANY_MODE:Counter on both edges
 * @param[in]	TIM_ConfigStruct pointer to TIM_TIMERCFG_Type
 * 				that contains the configuration information for the
 *                    specified Timer peripheral.
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_Init(LPC_TIM_TypeDef *TIMx, TIM_MODE_OPT TimerCounterMode, void *TIM_ConfigStruct)
{
    TIM_TIMERCFG_Type *pTimeCfg;
    TIM_COUNTERCFG_Type *pCounterCfg;

    // set power
    if (TIMx == LPC_TIM0)
    {
        LPC_SC->PCONP |= CLKPWR_PCONP_PCTIM0;
    }
    else if (TIMx == LPC_TIM1)
    {
        LPC_SC->PCONP |= CLKPWR_PCONP_PCTIM1;
    }

    else if (TIMx == LPC_TIM2)
    {
        LPC_SC->PCONP |= CLKPWR_PCONP_PCTIM2;
    }
    else if (TIMx == LPC_TIM3)
    {
        LPC_SC->PCONP |= CLKPWR_PCONP_PCTIM3;
    }

    TIMx->CCR &= ~TIM_CTCR_MODE_MASK;
    TIMx->CCR |= TIM_TIMER_MODE;

    TIMx->TC =0;
    TIMx->PC =0;
    TIMx->PR =0;
    TIMx->TCR |= (1<<1);  // Reset counter
    TIMx->TCR &= ~(1<<1); // Release reset

    if (TimerCounterMode == TIM_TIMER_MODE)
    {
        pTimeCfg = (TIM_TIMERCFG_Type *)TIM_ConfigStruct;
        if (pTimeCfg->PrescaleOption == TIM_PRESCALE_TICKVAL)
        {
            TIMx->PR   = pTimeCfg->PrescaleValue -1  ;
        }
        else
        {
            TIMx->PR   = converUSecToVal (pTimeCfg->PrescaleValue)-1;
        }
    }
    else
    {

        pCounterCfg = (TIM_COUNTERCFG_Type *)TIM_ConfigStruct;
        TIMx->CCR  &= ~TIM_CTCR_INPUT_MASK;

        if (pCounterCfg->CountInputSelect == TIM_COUNTER_INCAP1)
        {    
            TIMx->CCR |= _BIT(2);
        }
    }

    // Clear interrupt pending
    TIMx->IR = 0xFFFFFFFF;
}

//if (NewState == ENABLE)
//{
//	LPC_SC->PCONP |= PPType;
//}
//else if (NewState == DISABLE)
//{
//	LPC_SC->PCONP &= ~PPType;
//}


/*********************************************************************//**
 * @brief 		Close Timer/Counter device
 * @param[in]	TIMx  Pointer to timer device, should be:
 * 				- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_DeInit (LPC_TIM_TypeDef *TIMx)
{
    // Disable timer/counter
    TIMx->TCR = 0x00;

    // Disable power
    if (TIMx== LPC_TIM0)
    {    
        LPC_SC->PCONP &= ~CLKPWR_PCONP_PCTIM0;
    }
    else if (TIMx== LPC_TIM1)
    {    
        LPC_SC->PCONP &= ~CLKPWR_PCONP_PCTIM1;
    }
     else if (TIMx== LPC_TIM2)
    {     
         LPC_SC->PCONP &= ~CLKPWR_PCONP_PCTIM2;
    }
     else if (TIMx== LPC_TIM3)
    {     
         LPC_SC->PCONP &= ~CLKPWR_PCONP_PCTIM3;
    }
}

/*********************************************************************//**
 * @brief	 	Start/Stop Timer/Counter device
 * @param[in]	TIMx Pointer to timer device, should be:
 *  			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @param[in]	NewState
 * 				-	ENABLE  : set timer enable
 * 				-	DISABLE : disable timer
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_Cmd(LPC_TIM_TypeDef *TIMx, uint8_t NewState)
{
    if (NewState == ENABLE)
    {
        TIMx->TCR   |=  TIM_ENABLE;
    }
    else
    {
        TIMx->TCR &= ~TIM_ENABLE;
    }
}

/*********************************************************************//**
 * @brief 		Reset Timer/Counter device,
 * 					Make TC and PC are synchronously reset on the next
 * 					positive edge of PCLK
 * @param[in]	TIMx Pointer to timer device, should be:
 *   			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_ResetCounter(LPC_TIM_TypeDef *TIMx)
{
    TIMx->TCR |= TIM_RESET;
    TIMx->TCR &= ~TIM_RESET;
}

/*********************************************************************//**
 * @brief 		Configuration for Match register
 * @param[in]	TIMx Pointer to timer device, should be:
 *   			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @param[in]   TIM_MatchConfigStruct Pointer to TIM_MATCHCFG_Type
 * 					- MatchChannel : choose channel 0 or 1
 * 					- IntOnMatch	 : if SET, interrupt will be generated when MRxx match
 * 									the value in TC
 * 					- StopOnMatch	 : if SET, TC and PC will be stopped whenM Rxx match
 * 									the value in TC
 * 					- ResetOnMatch : if SET, Reset on MR0 when MRxx match
 * 									the value in TC
 * 					-ExtMatchOutputType: Select output for external match
 * 						 +	 0:	Do nothing for external output pin if match
 *						 +   1:	Force external output pin to low if match
 *						 + 	 2: Force external output pin to high if match
 *						 + 	 3: Toggle external output pin if match
 *					MatchValue: Set the value to be compared with TC value
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_ConfigMatch(LPC_TIM_TypeDef *TIMx, TIM_MATCHCFG_Type *TIM_MatchConfigStruct)
{
    switch (TIM_MatchConfigStruct->MatchChannel)
    {
        case 0:
            TIMx->MR0 = TIM_MatchConfigStruct->MatchValue;
            break;
        case 1:
            TIMx->MR1 = TIM_MatchConfigStruct->MatchValue;
            break;
        case 2:
            TIMx->MR2 = TIM_MatchConfigStruct->MatchValue;
            break;
        case 3:
            TIMx->MR3 = TIM_MatchConfigStruct->MatchValue;
            break;
        default:
            //Error match value
            //Error loop
            while (1);
    }
    //interrupt on MRn
    TIMx->MCR &= ~ TIM_MCR_CHANNEL_MASKBIT(TIM_MatchConfigStruct->MatchChannel);

    if (TIM_MatchConfigStruct->IntOnMatch)
    {    
        TIMx->MCR |= TIM_INT_ON_MATCH(TIM_MatchConfigStruct->MatchChannel);
    }

    //reset on MRn
    if (TIM_MatchConfigStruct->ResetOnMatch)
    {    
        TIMx->MCR |= TIM_RESET_ON_MATCH(TIM_MatchConfigStruct->MatchChannel);
    }

    //stop on MRn
    if (TIM_MatchConfigStruct->StopOnMatch)
    {    
        TIMx->MCR |= TIM_STOP_ON_MATCH(TIM_MatchConfigStruct->MatchChannel);
    }

    // match output type

    TIMx->EMR   &= ~ TIM_EM_MASK(TIM_MatchConfigStruct->MatchChannel);
    TIMx->EMR   |= TIM_EM_SET(TIM_MatchConfigStruct->MatchChannel,TIM_MatchConfigStruct->ExtMatchOutputType);
}
/*********************************************************************//**
 * @brief 		Update Match value
 * @param[in]	TIMx Pointer to timer device, should be:
 *   			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @param[in]	MatchChannel	Match channel, should be: 0..3
 * @param[in]	MatchValue		updated match value
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_UpdateMatchValue(LPC_TIM_TypeDef *TIMx,uint8_t MatchChannel, uint32_t MatchValue)
{
    switch (MatchChannel)
    {
        case 0:
            TIMx->MR0 = MatchValue;
            break;
        case 1:
            TIMx->MR1 = MatchValue;
            break;
        case 2:
            TIMx->MR2 = MatchValue;
            break;
        case 3:
            TIMx->MR3 = MatchValue;
            break;
        default:
            //Error Loop
            while (1);
    }
}

/*********************************************************************//**
 * @brief 		Configuration for Capture register
 * @param[in]	TIMx Pointer to timer device, should be:
 *   			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * 					- CaptureChannel: set the channel to capture data
 * 					- RisingEdge    : if SET, Capture at rising edge
 * 					- FallingEdge	: if SET, Capture at falling edge
 * 					- IntOnCaption  : if SET, Capture generate interrupt
 * @param[in]   TIM_CaptureConfigStruct	Pointer to TIM_CAPTURECFG_Type
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_ConfigCapture(LPC_TIM_TypeDef *TIMx, TIM_CAPTURECFG_Type *TIM_CaptureConfigStruct)
{
    TIMx->CCR &= ~TIM_CCR_CHANNEL_MASKBIT(TIM_CaptureConfigStruct->CaptureChannel);

    if (TIM_CaptureConfigStruct->RisingEdge)
    {
        TIMx->CCR |= TIM_CAP_RISING(TIM_CaptureConfigStruct->CaptureChannel);
    }

    if (TIM_CaptureConfigStruct->FallingEdge)
    {
        TIMx->CCR |= TIM_CAP_FALLING(TIM_CaptureConfigStruct->CaptureChannel);
    }

    if (TIM_CaptureConfigStruct->IntOnCaption)
    {
        TIMx->CCR |= TIM_INT_ON_CAP(TIM_CaptureConfigStruct->CaptureChannel);
    }
}

/*********************************************************************//**
 * @brief 		Read value of capture register in timer/counter device
 * @param[in]	TIMx Pointer to timer/counter device, should be:
 *  			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 * @param[in]	CaptureChannel: capture channel number, should be:
 * 				- TIM_COUNTER_INCAP0: CAPn.0 input pin for TIMERn
 * 				- TIM_COUNTER_INCAP1: CAPn.1 input pin for TIMERn
 * @return 		Value of capture register
 **********************************************************************/
uint32_t Lpc177x_8x_TIM_GetCaptureValue(LPC_TIM_TypeDef *TIMx, TIM_COUNTER_INPUT_OPT CaptureChannel)
{
    if (CaptureChannel==0)
    {
        return(TIMx->CR0);
    }
    else
    {
        return(TIMx->CR1);
    }
}

/*---------------Advanced TIMER functions -----------------------------------------*/
/*********************************************************************//**
 * @brief 		Timer wait (microseconds)
 * @param[in]	time	number of microseconds waiting
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_Waitus(uint32_t time)
{
    TIM_MATCHCFG_Type MatchConfigStruct;
    LPC_TIM0->IR = 0xFFFFFFFF;

    MatchConfigStruct.MatchChannel = 0;
    MatchConfigStruct.IntOnMatch = ENABLE;
    MatchConfigStruct.ResetOnMatch = ENABLE;
    MatchConfigStruct.StopOnMatch = ENABLE;
    MatchConfigStruct.ExtMatchOutputType = 0;
    MatchConfigStruct.MatchValue = time;

    Lpc177x_8x_TIM_ConfigMatch(LPC_TIM0, &MatchConfigStruct);
    Lpc177x_8x_TIM_Cmd(LPC_TIM0,ENABLE);
    //wait until interrupt flag occur
    while (!(LPC_TIM0->IR & 0x01));
    Lpc177x_8x_TIM_ResetCounter(LPC_TIM0);
}
/*********************************************************************//**
 * @brief 		Timer wait (milliseconds)
 * @param[in]	time	number of millisecond waiting
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_TIM_Waitms(uint32_t time)
{
    Lpc177x_8x_TIM_Waitus(time * 1000);
}

/*********************************************************************//**
 * \brief       Enable Timer interrupt
 *
 * @param[in]	TIMx Pointer to timer/counter device, should be:
 *  			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 *
 * \return      None
 *************************************************************************/
void Lpc177x_8x_TIMIrqEnable(LPC_TIM_TypeDef *TIMx)
{
    if (TIMx== LPC_TIM0)
    {
        NutIrqEnable (&sig_TIM0);
    }
    if (TIMx== LPC_TIM1)
    {
        NutIrqEnable (&sig_TIM1);
    }
    if (TIMx== LPC_TIM2)
    {
        NutIrqEnable (&sig_TIM2);
    }
    if (TIMx== LPC_TIM3)
    {
        NutIrqEnable (&sig_TIM3);
    }
}


/*********************************************************************//**
 * \brief       Disable Timer interrupt
 *
 * @param[in]	TIMx Pointer to timer/counter device, should be:
 *  			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 *
 * \return      None
 *************************************************************************/
void Lpc177x_8x_TIMIrqTXDisable(LPC_TIM_TypeDef *TIMx)
{
    if (TIMx== LPC_TIM0)
    {
        NutIrqDisable (&sig_TIM0);
    }
    if (TIMx== LPC_TIM1)
    {
        NutIrqDisable (&sig_TIM1);
    }
    if (TIMx== LPC_TIM2)
    {
        NutIrqDisable (&sig_TIM2);
    }
    if (TIMx== LPC_TIM3)
    {
        NutIrqDisable (&sig_TIM3);
    }
}

/************************************************************************//**
 * \brief       Get the priority level of the SD interrupt
 *              please note that only 1 level is present for _ALL_ different
 *              types of SD-interrupts
 *
 * @param[in]	TIMx Pointer to timer/counter device, should be:
 *  			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 *
 * \return      the interrupt level [0..31]
 ****************************************************************************/
int Lpc177x_8x_TIMIrqGetPriority(LPC_TIM_TypeDef *TIMx)
{
    if (TIMx== LPC_TIM0)
    {
        return(NutIrqGetPriority(&sig_TIM0));
    }
    if (TIMx== LPC_TIM1)
    {
        return(NutIrqGetPriority(&sig_TIM1));
    }
    if (TIMx== LPC_TIM2)
    {
        return(NutIrqGetPriority(&sig_TIM2));
    }
    if (TIMx== LPC_TIM3)
    {
        return(NutIrqGetPriority(&sig_TIM3));
    }

    return(-1);
}


/************************************************************************//**
 * \brief       Set the priority level of the SD interrupt
 *              please note that only 1 level is present for _ALL_ different
 *              types of SD-interrupts
 *
 * @param[in]	TIMx Pointer to timer/counter device, should be:
 *  			- LPC_TIM0: TIMER0 peripheral
 * 				- LPC_TIM1: TIMER1 peripheral
 * 				- LPC_TIM2: TIMER2 peripheral
 * 				- LPC_TIM3: TIMER3 peripheral
 *
 * \return      None
 ****************************************************************************/
void Lpc177x_8x_TIMIrqSetPriority(LPC_TIM_TypeDef *TIMx, int priority)
{
    if ((priority >= 0) && (priority < 32))
    {
        if (TIMx== LPC_TIM0)
        {
            NutIrqSetPriority(&sig_TIM0, priority);
        }
        if (TIMx== LPC_TIM1)
        {
            NutIrqSetPriority(&sig_TIM1, priority);
        }
        if (TIMx== LPC_TIM2)
        {
            NutIrqSetPriority(&sig_TIM2, priority);
        }
        if (TIMx== LPC_TIM3)
        {
            NutIrqSetPriority(&sig_TIM3, priority);
        }
    }
}

