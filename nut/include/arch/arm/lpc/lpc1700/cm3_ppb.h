#ifndef _ARCH_CM3_PPB_H_
#define _ARCH_CM3_PPB_H_

/*
 * Copyright 2011 by egnite GmbH
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
 */

/*!
 * \file arch/cm3/ppb.h
 * \brief Cortex-M3 private peripheral bus.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

/*!
 * \addtogroup xgNutArchCm3Ppb
 */
/*@{*/

/*! System control space base address. */
#define CM3_SCS_BASE        0xE000E000
/*! SysTick base address. */
#define CM3_SYSTICK_BASE    (CM3_SCS_BASE + 0x0010)
/*! NVIC base address. */
#define CM3_NVIC_BASE       (CM3_SCS_BASE + 0x0100)
/*! System control block base address. */
#define CM3_SCB_BASE        (CM3_SCS_BASE + 0x0D00)
/*! Memory protection unit base address. */
#define CM3_MPU_BASE        (CM3_SCS_BASE + 0x0D90)
/*! ITM base address. */
#define CM3_ITM_BASE        0xE0000000
/*! Core debug base address. */
#define CM3_COREDEBUG_BASE  (CM3_SCS_BASE + 0x0DF0)



/*! \name System Tick Control and Status Register */
/*@{*/
#define CM3_SYSTICK_CTRL_OFF    0x0000
#define CM3_SYSTICK_CTRL        (CM3_SYSTICK_BASE + CM3_SYSTICK_CTRL_OFF)

#define CM3_SYSTICK_CTRL_ENABLE_LSB     0
#define CM3_SYSTICK_CTRL_ENABLE         (1 << CM3_SYSTICK_CTRL_ENABLE_LSB)
#define CM3_SYSTICK_CTRL_TICKINT_LSB    1
#define CM3_SYSTICK_CTRL_TICKINT        (1 << CM3_SYSTICK_CTRL_TICKINT_LSB)
#define CM3_SYSTICK_CTRL_CLKSOURCE_LSB  2
#define CM3_SYSTICK_CTRL_CLKSOURCE      (1 << CM3_SYSTICK_CTRL_CLKSOURCE_LSB)
#define CM3_SYSTICK_CTRL_COUNTFLAG_LSB  16
#define CM3_SYSTICK_CTRL_COUNTFLAG      (1 << CM3_SYSTICK_CTRL_COUNTFLAG_LSB)
/*@}*/

/*! \name System Tick Reload Value Register */
/*@{*/
#define CM3_SYSTICK_LOAD_OFF    0x0004
#define CM3_SYSTICK_LOAD        (CM3_SYSTICK_BASE + CM3_SYSTICK_LOAD_OFF)
/*@}*/

/*! \name System Tick Current Value Register */
/*@{*/
#define CM3_SYSTICK_VAL_OFF     0x0008
#define CM3_SYSTICK_VAL         (CM3_SYSTICK_BASE + CM3_SYSTICK_VAL_OFF)
#define CM3_SYSTICK_VAL_MSK     0x00FFFFFF
/*@}*/

/*! \name System Tick Calibration Value Register */
/*@{*/
#define CM3_SYSTICK_CALIB_OFF   0x000C
#define CM3_SYSTICK_CALIB       (CM3_SYSTICK_BASE + CM3_SYSTICK_CALIB_OFF)
#define CM3_SYSTICK_CALIB_SKEW_LSB  30
#define CM3_SYSTICK_CALIB_SKEW      (1 << CM3_SYSTICK_CALIB_SKEW_LSB)
#define CM3_SYSTICK_CALIB_NOREF_LSB 31
#define CM3_SYSTICK_CALIB_NOREF     (1 << CM3_SYSTICK_CALIB_NOREF_LSB)
/*@}*/



/*! \name Interrupt Set-Enable Registers */
/*@{*/
#define CM3_NVIC_ISER_OFF(x)    (0x0000 + (x) * 4)
#define CM3_NVIC_ISER(x)        (CM3_NVIC_BASE + CM3_NVIC_ISER_OFF(x))
/*@}*/

/*! \name Interrupt Clear-Enable Registers */
/*@{*/
#define CM3_NVIC_ICER_OFF(x)    (0x0080 + (x) * 4)
#define CM3_NVIC_ICER(x)        (CM3_NVIC_BASE + CM3_NVIC_ICER_OFF(x))
/*@}*/

/*! \name Interrupt Set-Pending Registers */
/*@{*/
#define CM3_NVIC_ISPR_OFF(x)    (0x0100 + (x) * 4)
#define CM3_NVIC_ISPR(x)        (CM3_NVIC_BASE + CM3_NVIC_ISPR_OFF(x))
/*@}*/

/*! \name Interrupt Clear-Pending Registers */
/*@{*/
#define CM3_NVIC_ICPR_OFF(x)    (0x0180 + (x) * 4)
#define CM3_NVIC_ICPR(x)        (CM3_NVIC_BASE + CM3_NVIC_ICPR_OFF(x))
/*@}*/

/*! \name Interrupt Active Bit Registers */
/*@{*/
#define CM3_NVIC_IABR_OFF(x)    (0x0200 + (x) * 4)
#define CM3_NVIC_IABR(x)        (CM3_NVIC_BASE + CM3_NVIC_IABR_OFF(x))
/*@}*/

/*! \name Interrupt Priority Registers */
/*@{*/
#define CM3_NVIC_IPR_OFF(x)     (0x0300 + (x) * 4)
#define CM3_NVIC_IPR(x)         (CM3_NVIC_BASE + CM3_NVIC_IPR_OFF(x))
/*@}*/

/*! \name Software Trigger Interrupt Register */
/*@{*/
#define CM3_NVIC_STIR_OFF       0x0E00
#define CM3_NVIC_STIR           (CM3_NVIC_BASE + CM3_NVIC_STIR_OFF)
/*@}*/



/*! \name Interrupt Type Register */
/*@{*/
#define CM3_SCB_ICTR            (CM3_SCS_BASE + 0x0004)
/*@}*/

/*! \name Auxiliary Control Register */
/*@{*/
#define CM3_SCB_ACTLR           (CM3_SCS_BASE + 0x0008)
/*@}*/




/*! \name CPU ID Base Register */
/*@{*/
#define CM3_SCB_CPUID_OFF       0x0000
#define CM3_SCB_CPUID           (CM3_SCB_BASE + CM3_SCB_CPUID_OFF)
/*@}*/

/*! \name Interrupt Control and State Register */
/*@{*/
#define CM3_SCB_ICSR_OFF        0x0004
#define CM3_SCB_ICSR            (CM3_SCB_BASE + CM3_SCB_ICSR_OFF)
/*@}*/

/*! \name Vector Table Offset Register */
/*@{*/
#define CM3_SCB_VTOR_OFF        0x0008
#define CM3_SCB_VTOR            (CM3_SCB_BASE + CM3_SCB_VTOR_OFF)
/*@}*/

/*! \name Application Interrupt and Reset Control Register */
/*@{*/
#define CM3_SCB_AIRCR_OFF       0x000C
#define CM3_SCB_AIRCR           (CM3_SCB_BASE + CM3_SCB_AIRCR_OFF)
/*@}*/

/*! \name System Control Register */
/*@{*/
#define CM3_SCB_SCR_OFF         0x010
#define CM3_SCB_SCR             (CM3_SCB_BASE + CM3_SCB_SCR_OFF)
/*@}*/

/*! \name Configuration and Control Register */
/*@{*/
#define CM3_SCB_CCR_OFF         0x0014
#define CM3_SCB_CCR             (CM3_SCB_BASE + CM3_SCB_CCR_OFF)
/*@}*/

/*! \name System Handler Priority Registers 1..3 */
/*@{*/
#define CM3_SCB_SHPR_OFF(x)     (0x0014 + (x) * 4)
#define CM3_SCB_SHPR(x)         (CM3_SCB_BASE + CM3_SCB_SHPR_OFF(x))
#define CM3_SCB_SHPR_PRI_4_IDX  1
#define CM3_SCB_SHPR_PRI_4_LSB  24
#define CM3_SCB_SHPR_PRI_5_IDX  1
#define CM3_SCB_SHPR_PRI_5_LSB  24
#define CM3_SCB_SHPR_PRI_6_IDX  1
#define CM3_SCB_SHPR_PRI_6_LSB  24
#define CM3_SCB_SHPR_PRI_7_IDX  1
#define CM3_SCB_SHPR_PRI_7_LSB  24
#define CM3_SCB_SHPR_PRI_11_IDX 2
#define CM3_SCB_SHPR_PRI_11_LSB 24
#define CM3_SCB_SHPR_PRI_14_IDX 3
#define CM3_SCB_SHPR_PRI_14_LSB 24
#define CM3_SCB_SHPR_PRI_15_IDX 3
#define CM3_SCB_SHPR_PRI_15_LSB 24
/*@}*/

/*! \name System Handler Control and State Register */
/*@{*/
#define CM3_SCB_SHCRS_OFF       0x0024
#define CM3_SCB_SHCRS           (CM3_SCB_BASE + CM3_SCB_SHCRS_OFF)
/*@}*/

/*! \name Configurable Fault Status Register */
/*@{*/
#define CM3_SCB_CFSR_OFF        0x0028
#define CM3_SCB_CFSR            (CM3_SCB_BASE + CM3_SCB_CFSR_OFF)
/*@}*/

/*! \name Memory Management Fault Status Register */
/*@{*/
#define CM3_SCB_MMSR_OFF        0x0028
#define CM3_SCB_MMSR            (CM3_SCB_BASE + CM3_SCB_MMSR_OFF)
/*@}*/

/*! \name Bus Fault Status Register */
/*@{*/
#define CM3_SCB_BFSR_OFF        0x0029
#define CM3_SCB_BFSR            (CM3_SCB_BASE + CM3_SCB_BFSR_OFF)
/*@}*/

/*! \name Usage Fault Status Register */
/*@{*/
#define CM3_SCB_UFSR_OFF        0x002A
#define CM3_SCB_UFSR            (CM3_SCB_BASE + CM3_SCB_UFSR_OFF)
/*@}*/

/*! \name Hard Fault Status Register */
/*@{*/
#define CM3_SCB_HFSR_OFF        0x002C
#define CM3_SCB_HFSR            (CM3_SCB_BASE + CM3_SCB_HFSR_OFF)
/*@}*/

/*! \name Memory Management Fault Address Register */
/*@{*/
#define CM3_SCB_MMFAR_OFF       0x0034
#define CM3_SCB_MMFAR           (CM3_SCB_BASE + CM3_SCB_MMFAR_OFF)
/*@}*/

/*! \name Bus Fault Address Register */
/*@{*/
#define CM3_SCB_BFAR_OFF        0x0038
#define CM3_SCB_BFAR            (CM3_SCB_BASE + CM3_SCB_BFAR_OFF)
/*@}*/



/*! \name MPU Type Register */
/*@{*/
#define CM3_MPU_TYPE_OFF        0x0000
#define CM3_MPU_TYPE            (CM3_SCB_BASE + CM3_MPU_TYPE_OFF)
/*@}*/

/*! \name MPU Control Register */
/*@{*/
#define CM3_MPU_CTRL_OFF        0x0004
#define CM3_MPU_CTRL            (CM3_SCB_BASE + CM3_MPU_CTRL_OFF)
/*@}*/

/*! \name MPU Region Number Register */
/*@{*/
#define CM3_MPU_RNR_OFF         0x0008
#define CM3_MPU_RNR             (CM3_SCB_BASE + CM3_MPU_RNR_OFF)
/*@}*/

/*! \name MPU Region Base Address Register */
/*@{*/
#define CM3_MPU_RBAR_OFF        0x000C
#define CM3_MPU_RBAR            (CM3_SCB_BASE + CM3_MPU_RBAR_OFF)
#define CM3_MPU_RBAR_A_OFF(x)   (CM3_MPU_RBAR_OFF + (x) * 8)
#define CM3_MPU_RBAR_A(x)       (CM3_SCB_BASE + CM3_MPU_RBAR_A_OFF(x))
/*@}*/

/*! \name MPU Region Attribute and Size Register */
/*@{*/
#define CM3_MPU_RASR_OFF        0x0010
#define CM3_MPU_RASR            (CM3_SCB_BASE + CM3_MPU_RASR_OFF)
#define CM3_MPU_RASR_A_OFF(x)   (CM3_MPU_RASR_OFF + (x) * 8)
#define CM3_MPU_RASR_A(x)       (CM3_SCB_BASE + CM3_MPU_RASR_A_OFF(x))
/*@}*/





/*@}*/

#endif

