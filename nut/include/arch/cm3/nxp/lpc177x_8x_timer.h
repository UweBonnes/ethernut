/**********************************************************************
* $Id$		lpc177x_8x_timer.h			2011-06-02
*//**
* @file		lpc177x_8x_timer.h
* @brief	Contains all macro definitions and function prototypes
*			support for Timer firmware library on LPC177x_8x
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
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
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup TIMER	Timer
 * @ingroup LPC177x_8xCMSIS_FwLib_Drivers
 * @{
 */

#ifndef __LPC177X_8X_TIMER_H_
#define __LPC177X_8X_TIMER_H_

/* Includes ------------------------------------------------------------------- */
//#include "LPC177x_8x.h"
//#include "lpc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Private Macros ------------------------------------------------------------- */

 /** _BIT(n) sets the bit at position "n"
   * _BIT(n) is intended to be used in "OR" and "AND" expressions:
   * e.g., "(_BIT(3) | _BIT(7))".
   */
#undef _BIT
/** Set bit macro */
#define _BIT(n)	(1<<n)

/** _SBF(f,v) sets the bit field starting at position "f" to value "v".
 * _SBF(f,v) is intended to be used in "OR" and "AND" expressions:
 * e.g., "((_SBF(5,7) | _SBF(12,0xF)) & 0xFFFF)"
 */
#undef _SBF
/* Set bit field macro */
#define _SBF(f,v) (v<<f)

/* _BITMASK constructs a symbol with 'field_width' least significant
 * bits set.
 * e.g., _BITMASK(5) constructs '0x1F', _BITMASK(16) == 0xFFFF
 * The symbol is intended to be used to limit the bit field width
 * thusly:
 * <a_register> = (any_expression) & _BITMASK(x), where 0 < x <= 32.
 * If "any_expression" results in a value that is larger than can be
 * contained in 'x' bits, the bits above 'x - 1' are masked off.  When
 * used with the _SBF example above, the example would be written:
 * a_reg = ((_SBF(5,7) | _SBF(12,0xF)) & _BITMASK(16))
 * This ensures that the value written to a_reg is no wider than
 * 16 bits, and makes the code easier to read and understand.
 */
#undef _BITMASK
/* Bitmask creation macro */
#define _BITMASK(field_width) ( _BIT(field_width) - 1)

#ifndef ENABLE
#define ENABLE 1
#endif

#ifndef DISABLE
#define DISABLE 0
#endif

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/**********************************************************************
** Interrupt information
**********************************************************************/
/** Macro to clean interrupt pending */
#define TIM_IR_CLR(n) _BIT(n)

/**********************************************************************
** Timer interrupt register definitions
**********************************************************************/
/** Macro for getting a timer match interrupt bit */
#define TIM_MATCH_INT(n)		(_BIT(n & 0x0F))
/** Macro for getting a capture event interrupt bit */
#define TIM_CAP_INT(n)     (_BIT(((n & 0x0F) + 4)))

/**********************************************************************
* Timer control register definitions
**********************************************************************/
/** Timer/counter enable bit */
#define TIM_ENABLE			((uint32_t)(1<<0))
/** Timer/counter reset bit */
#define TIM_RESET			((uint32_t)(1<<1))
/** Timer control bit mask */
#define TIM_TCR_MASKBIT		((uint32_t)(3))

/**********************************************************************
* Timer match control register definitions
**********************************************************************/
/** Bit location for interrupt on MRx match, n = 0 to 3 */
#define TIM_INT_ON_MATCH(n)      	(_BIT((n * 3)))
/** Bit location for reset on MRx match, n = 0 to 3 */
#define TIM_RESET_ON_MATCH(n)    	(_BIT(((n * 3) + 1)))
/** Bit location for stop on MRx match, n = 0 to 3 */
#define TIM_STOP_ON_MATCH(n)     	(_BIT(((n * 3) + 2)))
/** Timer Match control bit mask */
#define TIM_MCR_MASKBIT			   ((uint32_t)(0x0FFF))
/** Timer Match control bit mask for specific channel*/
#define	TIM_MCR_CHANNEL_MASKBIT(n)		((uint32_t)(7<<(n*3)))

/**********************************************************************
* Timer capture control register definitions
**********************************************************************/
/** Bit location for CAP.n on CRx rising edge, n = 0 to 3 */
#define TIM_CAP_RISING(n)   	(_BIT((n * 3)))
/** Bit location for CAP.n on CRx falling edge, n = 0 to 3 */
#define TIM_CAP_FALLING(n)   	(_BIT(((n * 3) + 1)))
/** Bit location for CAP.n on CRx interrupt enable, n = 0 to 3 */
#define TIM_INT_ON_CAP(n)    	(_BIT(((n * 3) + 2)))
/** Mask bit for rising and falling edge bit */
#define TIM_EDGE_MASK(n)		(_SBF((n * 3), 0x03))
/** Timer capture control bit mask */
#define TIM_CCR_MASKBIT			((uint32_t)(0x3F))
/** Timer Capture control bit mask for specific channel*/
#define	TIM_CCR_CHANNEL_MASKBIT(n)		((uint32_t)(7<<(n*3)))

/**********************************************************************
* Timer external match register definitions
**********************************************************************/
/** Bit location for output state change of MAT.n when external match
   happens, n = 0 to 3 */
#define TIM_EM(n)    			_BIT(n)
/** Output state change of MAT.n when external match happens: no change */
#define TIM_EM_NOTHING    	((uint8_t)(0x0))
/** Output state change of MAT.n when external match happens: low */
#define TIM_EM_LOW         	((uint8_t)(0x1))
/** Output state change of MAT.n when external match happens: high */
#define TIM_EM_HIGH        	((uint8_t)(0x2))
/** Output state change of MAT.n when external match happens: toggle */
#define TIM_EM_TOGGLE      	((uint8_t)(0x3))
/** Macro for setting for the MAT.n change state bits */
#define TIM_EM_SET(n,s) 	(_SBF(((n << 1) + 4), (s & 0x03)))
/** Mask for the MAT.n change state bits */
#define TIM_EM_MASK(n) 		(_SBF(((n << 1) + 4), 0x03))
/** Timer external match bit mask */
#define TIM_EMR_MASKBIT	0x0FFF

/**********************************************************************
* Timer Count Control Register definitions
**********************************************************************/
/** Mask to get the Counter/timer mode bits */
#define TIM_CTCR_MODE_MASK  0x3
/** Mask to get the count input select bits */
#define TIM_CTCR_INPUT_MASK 0xC
/** Timer Count control bit mask */
#define TIM_CTCR_MASKBIT	0xF
#define TIM_COUNTER_MODE ((uint8_t)(1))

#ifndef FLAG_STATUS
#define FLAG_STATUS
typedef enum 
{
    RESET = 0, 
    SET = !RESET
} FlagStatus, ITStatus;
#endif

/**
 * @}
 */


/* Public Types --------------------------------------------------------------- */
/** @defgroup TIM_Public_Types Timer Public Types
 * @{
 */

/***********************************************************************
 * Timer device enumeration
**********************************************************************/
/** @brief interrupt type */
typedef enum
{
	TIM_MR0_INT =0, /*!< interrupt for Match channel 0*/
	TIM_MR1_INT =1, /*!< interrupt for Match channel 1*/
	TIM_MR2_INT =2, /*!< interrupt for Match channel 2*/
	TIM_MR3_INT =3, /*!< interrupt for Match channel 3*/
	TIM_CR0_INT =4, /*!< interrupt for Capture channel 0*/
	TIM_CR1_INT =5, /*!< interrupt for Capture channel 1*/
}TIM_INT_TYPE;

/** @brief Timer/counter operating mode */
typedef enum
{
	TIM_TIMER_MODE = 0,				/*!< Timer mode */
	TIM_COUNTER_RISING_MODE,		/*!< Counter rising mode */
	TIM_COUNTER_FALLING_MODE,		/*!< Counter falling mode */
	TIM_COUNTER_ANY_MODE			/*!< Counter on both edges */
} TIM_MODE_OPT;

/** @brief Timer/Counter prescale option */
typedef enum
{
	TIM_PRESCALE_TICKVAL = 0,		/*!< Prescale in absolute value */
	TIM_PRESCALE_USVAL				/*!< Prescale in microsecond value */
} TIM_PRESCALE_OPT;

/** @brief Counter input option */
typedef enum
{
	TIM_COUNTER_INCAP0 = 0,			/*!< CAPn.0 input pin for TIMERn */
	TIM_COUNTER_INCAP1,				/*!< CAPn.1 input pin for TIMERn */
} TIM_COUNTER_INPUT_OPT;

/** @brief Timer/Counter external match option */
typedef enum
{
	TIM_EXTMATCH_NOTHING = 0,		/*!< Do nothing for external output pin if match */
	TIM_EXTMATCH_LOW,				/*!< Force external output pin to low if match */
	TIM_EXTMATCH_HIGH,				/*!< Force external output pin to high if match */
	TIM_EXTMATCH_TOGGLE				/*!< Toggle external output pin if match */
}TIM_EXTMATCH_OPT;

/** @brief Timer/counter capture mode options */
typedef enum {
	TIM_CAPTURE_NONE = 0,	/*!< No Capture */
	TIM_CAPTURE_RISING,		/*!< Rising capture mode */
	TIM_CAPTURE_FALLING,	/*!< Falling capture mode */
	TIM_CAPTURE_ANY			/*!< On both edges */
} TIM_CAP_MODE_OPT;

/** @brief Configuration structure in TIMER mode */
typedef struct
{

	uint8_t PrescaleOption;		/**< Timer Prescale option, should be:
									- TIM_PRESCALE_TICKVAL: Prescale in absolute value
									- TIM_PRESCALE_USVAL: Prescale in microsecond value
									*/
	uint8_t Reserved[3];		/**< Reserved */
	uint32_t PrescaleValue;		/**< Prescale value */
} TIM_TIMERCFG_Type;

/** @brief Configuration structure in COUNTER mode */
typedef struct {

	uint8_t CounterOption;		/**< Counter Option, should be:
								- TIM_COUNTER_INCAP0: CAPn.0 input pin for TIMERn
								- TIM_COUNTER_INCAP1: CAPn.1 input pin for TIMERn
								*/
	uint8_t CountInputSelect;
	uint8_t Reserved[2];
} TIM_COUNTERCFG_Type;

/** @brief Match channel configuration structure */
typedef struct {
	uint8_t MatchChannel;	/**< Match channel, should be in range
							from 0..3 */
	uint8_t IntOnMatch;		/**< Interrupt On match, should be:
							- ENABLE: Enable this function.
							- DISABLE: Disable this function.
							*/
	uint8_t StopOnMatch;	/**< Stop On match, should be:
							- ENABLE: Enable this function.
							- DISABLE: Disable this function.
							*/
	uint8_t ResetOnMatch;	/**< Reset On match, should be:
							- ENABLE: Enable this function.
							- DISABLE: Disable this function.
							*/

	uint8_t ExtMatchOutputType;	/**< External Match Output type, should be:
							 -	 TIM_EXTMATCH_NOTHING:	Do nothing for external output pin if match
							 -   TIM_EXTMATCH_LOW:	Force external output pin to low if match
							 - 	 TIM_EXTMATCH_HIGH: Force external output pin to high if match
							 -   TIM_EXTMATCH_TOGGLE: Toggle external output pin if match.
							*/
	uint8_t Reserved[3];	/** Reserved */
	uint32_t MatchValue;	/** Match value */
} TIM_MATCHCFG_Type;

/** @brief Capture Input configuration structure */
typedef struct {
	uint8_t CaptureChannel;	/**< Capture channel, should be in range
							from 0..1 */
	uint8_t RisingEdge;		/**< caption rising edge, should be:
							- ENABLE: Enable rising edge.
							- DISABLE: Disable this function.
							*/
	uint8_t FallingEdge;		/**< caption falling edge, should be:
							- ENABLE: Enable falling edge.
							- DISABLE: Disable this function.
								*/
	uint8_t IntOnCaption;	/**< Interrupt On caption, should be:
							- ENABLE: Enable interrupt function.
							- DISABLE: Disable this function.
							*/

} TIM_CAPTURECFG_Type;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup TIM_Public_Functions Timer Public Functions
 * @{
 */
/* Init/DeInit TIM functions -----------*/
void Lpc177x_8x_TIM_Init(LPC_TIM_TypeDef *TIMx, TIM_MODE_OPT TimerCounterMode, void *TIM_ConfigStruct);
void Lpc177x_8x_TIM_DeInit(LPC_TIM_TypeDef *TIMx);

/* TIM interrupt functions -------------*/
void Lpc177x_8x_TIM_ClearIntPending(LPC_TIM_TypeDef *TIMx, TIM_INT_TYPE IntFlag);
void Lpc177x_8x_TIM_ClearIntCapturePending(LPC_TIM_TypeDef *TIMx, TIM_INT_TYPE IntFlag);
FlagStatus Lpc177x_8x_TIM_GetIntStatus(LPC_TIM_TypeDef *TIMx, TIM_INT_TYPE IntFlag);
FlagStatus Lpc177x_8x_TIM_GetIntCaptureStatus(LPC_TIM_TypeDef *TIMx, TIM_INT_TYPE IntFlag);

/* TIM configuration functions --------*/
void Lpc177x_8x_TIM_ConfigStructInit(TIM_MODE_OPT TimerCounterMode, void *TIM_ConfigStruct);
void Lpc177x_8x_TIM_ConfigMatch(LPC_TIM_TypeDef *TIMx, TIM_MATCHCFG_Type *TIM_MatchConfigStruct);
void Lpc177x_8x_TIM_UpdateMatchValue(LPC_TIM_TypeDef *TIMx,uint8_t MatchChannel, uint32_t MatchValue);
void Lpc177x_8x_TIM_SetMatchExt(LPC_TIM_TypeDef *TIMx,TIM_EXTMATCH_OPT ext_match );
void Lpc177x_8x_TIM_ConfigCapture(LPC_TIM_TypeDef *TIMx, TIM_CAPTURECFG_Type *TIM_CaptureConfigStruct);
void Lpc177x_8x_TIM_Cmd(LPC_TIM_TypeDef *TIMx, uint8_t NewState);

uint32_t Lpc177x_8x_TIM_GetCaptureValue(LPC_TIM_TypeDef *TIMx, TIM_COUNTER_INPUT_OPT CaptureChannel);
void Lpc177x_8x_TIM_ResetCounter(LPC_TIM_TypeDef *TIMx);

void Lpc177x_8x_TIM_Waitus(uint32_t time);
void Lpc177x_8x_TIM_Waitms(uint32_t time);

void Lpc177x_8x_TIMIrqEnable(LPC_TIM_TypeDef *TIMx);
void Lpc177x_8x_TIMIrqTXDisable(LPC_TIM_TypeDef *TIMx);
int  Lpc177x_8x_TIMIrqGetPriority(LPC_TIM_TypeDef *TIMx);
void Lpc177x_8x_TIMIrqSetPriority(LPC_TIM_TypeDef *TIMx, int priority);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif /* __LPC177X_8X_TIMER_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
