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
 * Parts taken from lpc177x_8x_pwm.c       2013-09-02
 *
 * file     lpc177x_8x_pwm.c
 * brief    Contains all functions support for PWM firmware library
 *          on LPC177x_8x
 * version  2.0
 * date     08. September. 2013
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
#include <arch/cm3/nxp/lpc177x_8x_pwm.h>

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup PWM
 * @{
     */

/* Includes ------------------------------------------------------------------- */
#include <arch/cm3/nxp/lpc177x_8x_clk.h>
#include <arch/cm3/nxp/lpc177x_8x_pwm.h>
//#include "lpc177x_8x_pwm.h"
//#include "lpc177x_8x_clkpwr.h"

//#define NUTDEBUG

#ifdef NUTDEBUG
    #include <stdio.h>
#endif

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup PWM_Public_Functions
 * @{
 */

static LPC_PWM_TypeDef* PWM_GetPointer (uint8_t pwmId);

/*********************************************************************//**
 * @brief 		Setting CAN baud rate (bps)
 * @param[in] 	canId point to LPC_CAN_TypeDef object, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @return 		The pointer to CAN peripheral that's expected to use
 ***********************************************************************/
static LPC_PWM_TypeDef* PWM_GetPointer (uint8_t pwmId)
{
	LPC_PWM_TypeDef* pPwm;

	switch (pwmId)
	{
		case PWM_0:
			pPwm = LPC_PWM0;
			break;

		case PWM_1:
			pPwm = LPC_PWM1;
			break;

		default:
			pPwm = NULL;
			break;
	}

	return pPwm;
}


/*********************************************************************//**
 * @brief 		Check whether specified interrupt flag in PWM is set or not
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @param[in]	IntFlag: PWM interrupt flag, should be:
 * 				- PWM_INTSTAT_MR0: Interrupt flag for PWM match channel 0
 * 				- PWM_INTSTAT_MR1: Interrupt flag for PWM match channel 1
 * 				- PWM_INTSTAT_MR2: Interrupt flag for PWM match channel 2
 * 				- PWM_INTSTAT_MR3: Interrupt flag for PWM match channel 3
 * 				- PWM_INTSTAT_MR4: Interrupt flag for PWM match channel 4
 * 				- PWM_INTSTAT_MR5: Interrupt flag for PWM match channel 5
 * 				- PWM_INTSTAT_MR6: Interrupt flag for PWM match channel 6
 * 				- PWM_INTSTAT_CAP0: Interrupt flag for capture input 0
 * 				- PWM_INTSTAT_CAP1: Interrupt flag for capture input 1
 * @return 		New State of PWM interrupt flag (SET or RESET)
 **********************************************************************/
FlagStatus Lpc177x_8x_PWM_GetIntStatus(uint8_t pwmId, uint32_t IntFlag)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	return ((pPwm->IR & IntFlag) ? SET : RESET);
}



/*********************************************************************//**
 * @brief 		Clear specified PWM Interrupt pending
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @param[in]	IntFlag: PWM interrupt flag, should be:
 * 				- PWM_INTSTAT_MR0: Interrupt flag for PWM match channel 0
 * 				- PWM_INTSTAT_MR1: Interrupt flag for PWM match channel 1
 * 				- PWM_INTSTAT_MR2: Interrupt flag for PWM match channel 2
 * 				- PWM_INTSTAT_MR3: Interrupt flag for PWM match channel 3
 * 				- PWM_INTSTAT_MR4: Interrupt flag for PWM match channel 4
 * 				- PWM_INTSTAT_MR5: Interrupt flag for PWM match channel 5
 * 				- PWM_INTSTAT_MR6: Interrupt flag for PWM match channel 6
 * 				- PWM_INTSTAT_CAP0: Interrupt flag for capture input 0
 * 				- PWM_INTSTAT_CAP1: Interrupt flag for capture input 1
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_PWM_ClearIntPending(uint8_t pwmId, uint32_t IntFlag)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	pPwm->IR = IntFlag;
}



/*****************************************************************************//**
* @brief		Fills each PWM_InitStruct member with its default value:
* 				- If PWMCounterMode = PWM_MODE_TIMER:
* 					+ PrescaleOption = PWM_TIMER_PRESCALE_USVAL
* 					+ PrescaleValue = 1
* 				- If PWMCounterMode = PWM_MODE_COUNTER:
* 					+ CountInputSelect = PWM_COUNTER_PCAP1_0
* 					+ CounterOption = PWM_COUNTER_RISING
* @param[in]	PWMTimerCounterMode Timer or Counter mode, should be:
* 				- PWM_MODE_TIMER: Counter of PWM peripheral is in Timer mode
* 				- PWM_MODE_COUNTER: Counter of PWM peripheral is in Counter mode
* @param[in]	PWM_InitStruct Pointer to structure (PWM_TIMERCFG_Type or
* 				 PWM_COUNTERCFG_Type) which will be initialized.
* @return		None
* Note: PWM_InitStruct pointer will be assigned to corresponding structure
* 		(PWM_TIMERCFG_Type or PWM_COUNTERCFG_Type) due to PWMTimerCounterMode.
*******************************************************************************/
void Lpc177x_8x_PWM_ConfigStructInit(uint8_t PWMTimerCounterMode, void *PWM_InitStruct)
{
	PWM_TIMERCFG_Type *pTimeCfg;
	PWM_COUNTERCFG_Type *pCounterCfg;

	pTimeCfg = (PWM_TIMERCFG_Type *) PWM_InitStruct;
	pCounterCfg = (PWM_COUNTERCFG_Type *) PWM_InitStruct;

	if (PWMTimerCounterMode == PWM_MODE_TIMER )
	{
		pTimeCfg->PrescaleOption = PWM_TIMER_PRESCALE_USVAL;
		pTimeCfg->PrescaleValue = 1;
	}
	else if (PWMTimerCounterMode == PWM_MODE_COUNTER)
	{
		pCounterCfg->CountInputSelect = PWM_COUNTER_PCAP1_0;
		pCounterCfg->CounterOption = PWM_COUNTER_RISING;
	}
}


/*********************************************************************//**
 * @brief 		Initializes the PWM peripheral corresponding to the specified
 *            parameters in the PWM_ConfigStruct.
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 *
 * @param[in]	PWMTimerCounterMode Timer or Counter mode, should be:
 * 				    - PWM_MODE_TIMER: Counter of PWM peripheral is in Timer mode
 * 				    - PWM_MODE_COUNTER: Counter of PWM peripheral is in Counter mode
 * @param[in]	PWM_ConfigStruct Pointer to structure (PWM_TIMERCFG_Type or
 * 				    PWM_COUNTERCFG_Type) which will be initialized.
 * @return 		None
 *
 * Note: PWM_ConfigStruct pointer will be assigned to corresponding structure
 * 		   (PWM_TIMERCFG_Type or PWM_COUNTERCFG_Type) due to PWMTimerCounterMode.
 **********************************************************************/
void Lpc177x_8x_PWM_Init(uint8_t pwmId, uint32_t PWMTimerCounterMode, void *PWM_ConfigStruct)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	PWM_TIMERCFG_Type *pTimeCfg;
	PWM_COUNTERCFG_Type *pCounterCfg;
//	uint64_t clkdlycnt;
  uint32_t clkdlycnt;

	pTimeCfg = (PWM_TIMERCFG_Type *)PWM_ConfigStruct;
	pCounterCfg = (PWM_COUNTERCFG_Type *)PWM_ConfigStruct;

	if(pwmId == PWM_0)
	{
//	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCPWM0, ENABLE);
    LPC_SC->PCONP |= CLKPWR_PCONP_PCPWM0;
	}
	else if(pwmId == PWM_1)
	{
//	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCPWM1, ENABLE);
    LPC_SC->PCONP |= CLKPWR_PCONP_PCPWM1;
	}
	else
	{
		//Trap the error
		while(1);
		return;
	}

	// Get peripheral clock of PWM1
	clkdlycnt = Lpc17xx_ClockGet(NUT_HWCLK_PCLK);

	// Clear all interrupts pending
	pPwm->IR = 0xFF & PWM_IR_BITMASK;
	pPwm->TCR = 0x00;
	pPwm->CTCR = 0x00;
	pPwm->MCR = 0x00;
	pPwm->CCR = 0x00;
	pPwm->PCR = 0x00;
	pPwm->LER = 0x00;

	if (PWMTimerCounterMode == PWM_MODE_TIMER)
	{
		/* Absolute prescale value */
		if (pTimeCfg->PrescaleOption == PWM_TIMER_PRESCALE_TICKVAL)
		{
			pPwm->PR   = pTimeCfg->PrescaleValue - 1;
		}
		/* uSecond prescale value */
		else
		{
			clkdlycnt = (clkdlycnt * pTimeCfg->PrescaleValue) / 1000000;
			pPwm->PR = ((uint32_t) clkdlycnt) - 1;
		}

	}
	else if (PWMTimerCounterMode == PWM_MODE_COUNTER)
	{

		pPwm->CTCR |= (PWM_CTCR_MODE((uint32_t)pCounterCfg->CounterOption)) \
						| (PWM_CTCR_SELECT_INPUT((uint32_t)pCounterCfg->CountInputSelect));
	}
}

/*********************************************************************//**
 * @brief		De-initializes the PWM peripheral registers to their
*                  default reset values.
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_PWM_DeInit (uint8_t pwmId)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	// Disable PWM control (timer, counter and PWM)
	pPwm->TCR = 0x00;

  if(pwmId == PWM_0)
  {
      LPC_SC->PCONP &= ~CLKPWR_PCONP_PCPWM0;
  }
  else if(pwmId == PWM_1)
  {
      LPC_SC->PCONP &= ~CLKPWR_PCONP_PCPWM1;
  }
  else
  {
      //Trap the error
      while(1);
      return;
  }
}


/*********************************************************************//**
 * @brief	 	Enable/Disable PWM peripheral
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @param[in]	NewState	New State of this function, should be:
 * 							- ENABLE: Enable PWM peripheral
 * 							- DISABLE: Disable PWM peripheral
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_PWM_Cmd(uint8_t pwmId, uint8_t NewState)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	if (NewState == ENABLE)
	{
		pPwm->TCR	|=  PWM_TCR_PWM_ENABLE;
	}
	else
	{
		pPwm->TCR &= (~PWM_TCR_PWM_ENABLE) & PWM_TCR_BITMASK;
	}
}


/*********************************************************************//**
 * @brief 		Enable/Disable Counter in PWM peripheral
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @param[in]	NewState New State of this function, should be:
 * 							- ENABLE: Enable Counter in PWM peripheral
 * 							- DISABLE: Disable Counter in PWM peripheral
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_PWM_CounterCmd(uint8_t pwmId, uint8_t NewState)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	if (NewState == ENABLE)
	{
		pPwm->TCR	|=  PWM_TCR_COUNTER_ENABLE;
	}
	else
	{
		pPwm->TCR &= (~PWM_TCR_COUNTER_ENABLE) & PWM_TCR_BITMASK;
	}
}


/*********************************************************************//**
 * @brief 		Reset Counter in PWM peripheral
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_PWM_ResetCounter(uint8_t pwmId)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	pPwm->TCR |= PWM_TCR_COUNTER_RESET;

	pPwm->TCR &= (~PWM_TCR_COUNTER_RESET) & PWM_TCR_BITMASK;
}


/*********************************************************************//**
 * @brief 		Configures match for PWM peripheral
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @param[in]   PWM_MatchConfigStruct	Pointer to a PWM_MATCHCFG_Type structure
*                    that contains the configuration information for the
*                    specified PWM match function.
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_PWM_ConfigMatch(uint8_t pwmId, PWM_MATCHCFG_Type *PWM_MatchConfigStruct)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	//interrupt on MRn
	if (PWM_MatchConfigStruct->IntOnMatch == ENABLE)
	{
		pPwm->MCR |= PWM_MCR_INT_ON_MATCH(PWM_MatchConfigStruct->MatchChannel);
	}
	else
	{
		pPwm->MCR &= (~ PWM_MCR_INT_ON_MATCH(PWM_MatchConfigStruct->MatchChannel)) \
																		& PWM_MCR_BITMASK;
	}

	//reset on MRn
	if (PWM_MatchConfigStruct->ResetOnMatch == ENABLE)
	{
		pPwm->MCR |= PWM_MCR_RESET_ON_MATCH(PWM_MatchConfigStruct->MatchChannel);
	}
	else
	{
		pPwm->MCR &= (~ PWM_MCR_RESET_ON_MATCH(PWM_MatchConfigStruct->MatchChannel)) \
																		& PWM_MCR_BITMASK;
	}

	//stop on MRn
	if (PWM_MatchConfigStruct->StopOnMatch == ENABLE)
	{
		pPwm->MCR |= PWM_MCR_STOP_ON_MATCH(PWM_MatchConfigStruct->MatchChannel);
	}
	else
	{
		pPwm->MCR &= (~ PWM_MCR_STOP_ON_MATCH(PWM_MatchConfigStruct->MatchChannel)) \
																		& PWM_MCR_BITMASK;
	}
}


/*********************************************************************//**
 * @brief 		Configures capture input for PWM peripheral
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @param[in]   PWM_CaptureConfigStruct	Pointer to a PWM_CAPTURECFG_Type structure
 *                    that contains the configuration information for the
 *                    specified PWM capture input function.
 * @return 		None
 **********************************************************************/
void Lpc177x_8x_PWM_ConfigCapture(uint8_t pwmId, PWM_CAPTURECFG_Type *PWM_CaptureConfigStruct)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	if (PWM_CaptureConfigStruct->RisingEdge == ENABLE)
	{
		pPwm->CCR |= PWM_CCR_CAP_RISING(PWM_CaptureConfigStruct->CaptureChannel);
	}
	else
	{
		pPwm->CCR &= (~ PWM_CCR_CAP_RISING(PWM_CaptureConfigStruct->CaptureChannel)) \
																		& PWM_CCR_BITMASK;
	}

	if (PWM_CaptureConfigStruct->FallingEdge == ENABLE)
	{
		pPwm->CCR |= PWM_CCR_CAP_FALLING(PWM_CaptureConfigStruct->CaptureChannel);
	}
	else
	{
		pPwm->CCR &= (~ PWM_CCR_CAP_FALLING(PWM_CaptureConfigStruct->CaptureChannel)) \
																		& PWM_CCR_BITMASK;
	}

	if (PWM_CaptureConfigStruct->IntOnCaption == ENABLE)
	{
		pPwm->CCR |= PWM_CCR_INT_ON_CAP(PWM_CaptureConfigStruct->CaptureChannel);
	}
	else
	{
		pPwm->CCR &= (~ PWM_CCR_INT_ON_CAP(PWM_CaptureConfigStruct->CaptureChannel)) \
																		& PWM_CCR_BITMASK;
	}
}


/*********************************************************************//**
 * @brief 		Read value of capture register PWM peripheral
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @param[in]	CaptureChannel: capture channel number, should be in
 * 				range 0 to 1
 * @return 		Value of capture register
 **********************************************************************/
uint32_t Lpc177x_8x_PWM_GetCaptureValue(uint8_t pwmId, uint8_t CaptureChannel)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	switch (CaptureChannel)
	{
		case 0:
			return pPwm->CR0;

		case 1:
			return pPwm->CR1;

		default:
			return (0);
	}
}


/********************************************************************//**
 * @brief 		Update value for each PWM channel with update type option
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @param[in]	MatchChannel Match channel
 * @param[in]	MatchValue Match value
 * @param[in]	UpdateType Type of Update, should be:
 * 				- PWM_MATCH_UPDATE_NOW: The update value will be updated for
 * 					this channel immediately
 * 				- PWM_MATCH_UPDATE_NEXT_RST: The update value will be updated for
 * 					this channel on next reset by a PWM Match event.
 * @return		None
 *********************************************************************/
void Lpc177x_8x_PWM_MatchUpdate(uint8_t pwmId, uint8_t MatchChannel,
										uint32_t MatchValue, uint8_t UpdateType)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	switch (MatchChannel)
	{
		case 0:
			pPwm->MR0 = MatchValue;
			break;

		case 1:
			pPwm->MR1 = MatchValue;
			break;

		case 2:
			pPwm->MR2 = MatchValue;
			break;

		case 3:
			pPwm->MR3 = MatchValue;
			break;

		case 4:
			pPwm->MR4 = MatchValue;
			break;

		case 5:
			pPwm->MR5 = MatchValue;
			break;

		case 6:
			pPwm->MR6 = MatchValue;
			break;
	}

	// Write Latch register
	pPwm->LER |= PWM_LER_EN_MATCHn_LATCH(MatchChannel);

	// In case of update now
	if (UpdateType == PWM_MATCH_UPDATE_NOW)
	{
		pPwm->TCR |= PWM_TCR_COUNTER_RESET;
		pPwm->TCR &= (~PWM_TCR_COUNTER_RESET) & PWM_TCR_BITMASK;
	}
}


/********************************************************************//**
 * @brief 		Configure Edge mode for each PWM channel
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @param[in]	PWMChannel PWM channel, should be in range from 2 to 6
 * @param[in]	ModeOption PWM mode option, should be:
 * 				    - PWM_CHANNEL_SINGLE_EDGE: Single Edge mode
 * 				    - PWM_CHANNEL_DUAL_EDGE: Dual Edge mode
 * @return 		None
 * Note: PWM Channel 1 can not be selected for mode option
 *********************************************************************/
void Lpc177x_8x_PWM_ChannelConfig(uint8_t pwmId, uint8_t PWMChannel, uint8_t ModeOption)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	// Single edge mode
	if (ModeOption == PWM_CHANNEL_SINGLE_EDGE)
	{
		pPwm->PCR &= (~ PWM_PCR_PWMSELn(PWMChannel)) & PWM_PCR_BITMASK;
	}
	// Double edge mode
	else if (PWM_CHANNEL_DUAL_EDGE)
	{
		pPwm->PCR |= PWM_PCR_PWMSELn(PWMChannel);
	}
}



/********************************************************************//**
 * @brief 		Enable/Disable PWM channel output
 * @param[in]	 pwmId			 The Id of the expected PWM component
 *
 * @param[in]	PWMChannel PWM channel, should be in range from 1 to 6
 * @param[in]	NewState New State of this function, should be:
 * 				- ENABLE: Enable this PWM channel output
 * 				- DISABLE: Disable this PWM channel output
 * @return		None
 *********************************************************************/
void Lpc177x_8x_PWM_ChannelCmd(uint8_t pwmId, uint8_t PWMChannel, uint8_t NewState)
{
	LPC_PWM_TypeDef* pPwm = PWM_GetPointer(pwmId);

	if (NewState == ENABLE)
	{
		pPwm->PCR |= PWM_PCR_PWMENAn(PWMChannel);
	}
	else
	{
		pPwm->PCR &= (~ PWM_PCR_PWMENAn(PWMChannel)) & PWM_PCR_BITMASK;
	}
}

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
