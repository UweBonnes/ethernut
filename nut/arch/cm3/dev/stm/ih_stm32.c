/*
 * Copyright (C) 2013 Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadty.de).
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
 */

/*
 * $Log$
 * Revision 1.0  2011/12/09 17:30:10  ulrichprinz
 * Initial version.
 *
 */

#include <cfg/arch.h>
#include <cfg/devices.h>
#include <arch/cm3.h>
#include <dev/irqreg.h>

#ifndef NUT_IRQPRI_DEF
#define NUT_IRQPRI_DEF  4
#endif
/* sig_INTERRUPTX needs special handling for level sensivity and multiple
   occupancy*/

/* We can't use XXX_IRQn to check for availability, as XXX_IRQn is an enum
   not usable for the preprocessor.
   - If the Device is available on all devices in a family but not for all
     families, for the device specific structure (e.g. USART6).
   - If the device is only available on some F1 devices, create a configurator
     item with the HW_XXX_STM32 value.
*/
Create_Handler(WWDG,          WWDG,     NUT_IRQPRI_DEF);    /* Window Watchdog */
Create_Handler(PVD,           PVD,      NUT_IRQPRI_DEF);    /* PVD through EXTI */
Create_Handler(FLASH,         FLASH,    NUT_IRQPRI_DEF);    /* Flash global */
Create_Handler(RCC,           RCC,      NUT_IRQPRI_DEF);    /* RCC global */
#if defined(HW_RTC_STM32F1)
Create_Handler(RTC,           RTC,      NUT_IRQPRI_DEF);    // Real Time Clock
#endif
#if defined(HW_RTC_STM32_V2)
Create_Handler(RTC,           RTC_Alarm,NUT_IRQPRI_DEF);    // Real Time Clock
#endif
Create_Handler(SPI1,          SPI1,     NUT_IRQPRI_DEF);     // SPI 1 Controller
Create_Handler(SPI2,          SPI2,     NUT_IRQPRI_DEF);     // SPI 2 Controller
#if defined(HW_SPI3_STM32)
Create_Handler(SPI3,          SPI3,     NUT_IRQPRI_DEF);     // SPI 3 Controller
#endif
#if defined(HW_SPI4_STM32)
Create_Handler(SPI4,          SPI4,     NUT_IRQPRI_DEF);     // SPI 4 Controller
#endif
#if defined(HW_SPI5_STM32)
Create_Handler(SPI5,          SPI5,     NUT_IRQPRI_DEF);     // SPI 5 Controller
#endif
#if defined(HW_SPI6_STM32)
Create_Handler(SPI6,          SPI6,     NUT_IRQPRI_DEF);     // SPI 6 Controller
#endif

#if defined(HW_SDIO_STM32)
Create_Handler(SDIO,          SDIO,     NUT_IRQPRI_DEF);     // SDIO Controller
#endif

#if defined(HW_SAI1_STM32)
Create_Handler(SAI1,          SAI1,     NUT_IRQPRI_DEF);     // SPI 2 Controller
#endif

Create_Handler(TWI1_EV,       I2C1_EV,  NUT_IRQPRI_DEF);     // I2C 1 Data/Event
Create_Handler(TWI1_ER,       I2C1_ER,  NUT_IRQPRI_DEF);     // I2C 2 Data/Event
#if defined(HW_I2C2_STM32)
Create_Handler(TWI2_EV,       I2C2_EV,  NUT_IRQPRI_DEF);     // I2C 2 Data/Event
Create_Handler(TWI2_ER,       I2C2_ER,  NUT_IRQPRI_DEF);     // I2C 1 Error
#endif
#if defined(I2C3)
Create_Handler(TWI3_EV,       I2C3_EV,  NUT_IRQPRI_DEF);     // I2C 2 Error
Create_Handler(TWI3_ER,       I2C3_ER,  NUT_IRQPRI_DEF);     // I2C 2 Error
#endif
#if defined(HW_USB_CAN1_STM32) || defined(STM32F30X)
Create_Handler(CAN1_TX,       USB_HP_CAN1_TX,  NUT_IRQPRI_DEF);  // CAN 1 TX
Create_Handler(CAN1_RX0,      USB_LP_CAN1_RX0, NUT_IRQPRI_DEF);  // CAN 1 RX0
Create_Handler(CAN1_RX1,      CAN1_RX1,        NUT_IRQPRI_DEF);  // CAN 1 RX1
Create_Handler(CAN1_SCE,      CAN1_SCE,        NUT_IRQPRI_DEF);  // CAN 1 SCE
#elif defined(HW_CAN1_STM32)
Create_Handler(CAN1_TX,       CAN1_TX,         NUT_IRQPRI_DEF);  // CAN 1 TX
Create_Handler(CAN1_RX0,      CAN1_RX0,        NUT_IRQPRI_DEF);  // CAN 1 RX0
Create_Handler(CAN1_RX1,      CAN1_RX1,        NUT_IRQPRI_DEF);  // CAN 1 RX1
Create_Handler(CAN1_SCE,      CAN1_SCE,        NUT_IRQPRI_DEF);  // CAN 1 SCE
#endif
#if defined(HW_CAN2_STM32)
Create_Handler(CAN2_TX,       CAN2_TX,         NUT_IRQPRI_DEF);  // CAN 2 TX
Create_Handler(CAN2_RX0,      CAN2_RX0,        NUT_IRQPRI_DEF);  // CAN 2 RX0
Create_Handler(CAN2_RX1,      CAN2_RX1,        NUT_IRQPRI_DEF);  // CAN 2 RX1
Create_Handler(CAN2_SCE,      CAN2_SCE,        NUT_IRQPRI_DEF);  // CAN 2 SCE
#endif
#if defined(HW_USB_STM32)
Create_Handler(USB_HP,        USB_HP,          NUT_IRQPRI_DEF);  // USB High Priority, separted
Create_Handler(USB_LP,        USB_LP,          NUT_IRQPRI_DEF);  // USB Low Priority, separted
#if defined(STM32F30X)
Create_Handler(USB_WAKE,      USBWakeUp_RMP,   NUT_IRQPRI_DEF);  // USB Wake Priority, separted
#else
Create_Handler(USB_WAKE,      USBWakeUp,   NUT_IRQPRI_DEF);  // USB Wake Priority, separted
#endif
#endif

Create_Handler(USART1,        USART1,          NUT_IRQPRI_DEF);  // USART 1
#if defined(HW_USART2_STM32)
Create_Handler(USART2,        USART2,          NUT_IRQPRI_DEF);  // USART 2
#endif
#if defined(HW_USART3_STM32)
Create_Handler(USART3,        USART3,          NUT_IRQPRI_DEF);  // USART 3
#endif
#if defined(HW_UART4_STM32)
Create_Handler(UART4,         UART4,           NUT_IRQPRI_DEF);  // UART 4
#endif
#if defined(HW_UART5_STM32)
Create_Handler(UART5,         UART5,           NUT_IRQPRI_DEF);  // UART 5
#endif
#if defined(HW_USART6_STM32)
Create_Handler(USART6,        USART6,          NUT_IRQPRI_DEF);  // USART 6
#endif
#if defined(HW_UART7_STM32)
Create_Handler(UART7,         UART7,          NUT_IRQPRI_DEF);  // UART 7
#endif
#if defined(HW_UART8_STM32)
Create_Handler(UART8,         UART8,          NUT_IRQPRI_DEF);  // UART 8
#endif

#if defined (DMA1_Channel1_BASE)
Create_Handler(DMA1_CH1, DMA1_Channel1,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA1_CH2, DMA1_Channel2,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA1_CH3, DMA1_Channel3,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA1_CH4, DMA1_Channel4,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA1_CH5, DMA1_Channel5,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA1_CH6, DMA1_Channel6,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA1_CH7, DMA1_Channel7,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
#endif
#if defined(HW_DMA2_STM32F1)
Create_Handler(DMA2_CH1, DMA2_Channel1,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA2_CH2, DMA2_Channel2,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA2_CH3, DMA2_Channel3,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA2_CH4, DMA2_Channel4,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA2_CH5, DMA2_Channel5,   NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
#endif
#if defined(DMA1_Stream0)
Create_Handler(DMA1_CH1, DMA1_Stream0,    NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA1_CH2, DMA1_Stream1,    NUT_IRQPRI_DEF); // DMA Controller 1 Channel 1
Create_Handler(DMA1_CH3, DMA1_Stream2,    NUT_IRQPRI_DEF); // DMA Controller 1 Channel 2
Create_Handler(DMA1_CH4, DMA1_Stream3,    NUT_IRQPRI_DEF); // DMA Controller 1 Channel 3
Create_Handler(DMA1_CH5, DMA1_Stream4,    NUT_IRQPRI_DEF); // DMA Controller 1 Channel 4
Create_Handler(DMA1_CH6, DMA1_Stream5,    NUT_IRQPRI_DEF); // DMA Controller 1 Channel 5
Create_Handler(DMA1_CH7, DMA1_Stream6,    NUT_IRQPRI_DEF); // DMA Controller 1 Channel 6
Create_Handler(DMA1_CH8, DMA1_Stream7,    NUT_IRQPRI_DEF); // DMA Controller 1 Channel 7
Create_Handler(DMA2_CH1, DMA2_Stream0,    NUT_IRQPRI_DEF); // DMA Controller 2 Channel 1
Create_Handler(DMA2_CH2, DMA2_Stream1,    NUT_IRQPRI_DEF); // DMA Controller 2 Channel 2
Create_Handler(DMA2_CH3, DMA2_Stream2,    NUT_IRQPRI_DEF); // DMA Controller 2 Channel 3
Create_Handler(DMA2_CH4, DMA2_Stream3,    NUT_IRQPRI_DEF); // DMA Controller 2 Channel 4
Create_Handler(DMA2_CH5, DMA2_Stream4,    NUT_IRQPRI_DEF); // DMA Controller 2 Channel 5
Create_Handler(DMA2_CH6, DMA2_Stream5,    NUT_IRQPRI_DEF); // DMA Controller 2 Channel 6
Create_Handler(DMA2_CH7, DMA2_Stream6,    NUT_IRQPRI_DEF); // DMA Controller 2 Channel 7
Create_Handler(DMA2_CH8, DMA2_Stream7,    NUT_IRQPRI_DEF); // DMA Controller 2 Channel 7
#endif

#if defined(HW_EMAC_STM32)
Create_Handler(EMAC,          ETH,          NUT_IRQPRI_DEF);     // Ethernet global interrupt
Create_Handler(EMAC_WAKE,     ETH_WKUP,     NUT_IRQPRI_DEF);     // Ethernet global interrupt
#endif
#if defined(HW_TIM1_STM32)
Create_Handler(TIM1_BRK,      TIM1_BRK,     NUT_IRQPRI_DEF);
Create_Handler(TIM1_UP,       TIM1_UP,      NUT_IRQPRI_DEF);
Create_Handler(TIM1_TRG_COM,  TIM1_TRG_COM, NUT_IRQPRI_DEF);
Create_Handler(TIM1_CC,       TIM1_CC,      NUT_IRQPRI_DEF);
#endif
/* Only Tim2 up to TIM5 have always a single TIMx_IRQn*/
Create_Handler(TIM2,          TIM2,         NUT_IRQPRI_DEF);
Create_Handler(TIM3,          TIM3,         NUT_IRQPRI_DEF);
Create_Handler(TIM4,          TIM4,         NUT_IRQPRI_DEF);
#if defined(HW_TIM5_STM32)
Create_Handler(TIM5,          TIM5,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM6_STM32)
Create_Handler(TIM6,          TIM6,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM7_STM32)
Create_Handler(TIM7,          TIM7,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM8_STM32)
Create_Handler(TIM8_BRK,      TIM8_BRK,     NUT_IRQPRI_DEF);
Create_Handler(TIM8_UP,       TIM8_UP,      NUT_IRQPRI_DEF);
Create_Handler(TIM8_TRG_COM,  TIM8_TRG_COM, NUT_IRQPRI_DEF);
Create_Handler(TIM8_CC,       TIM8_CC,     NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM10_STM32)
Create_Handler(TIM10,          TIM10,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM11_STM32)
Create_Handler(TIM11,          TIM11,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM12_STM32)
Create_Handler(TIM12,          TIM12,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM13_STM32)
Create_Handler(TIM13,          TIM13,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM14_STM32)
Create_Handler(TIM14,          TIM14,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM15_STM32)
Create_Handler(TIM15,          TIM15,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM16_STM32)
Create_Handler(TIM16,          TIM16,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM17_STM32)
Create_Handler(TIM17,          TIM17,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_TIM19_STM32)
Create_Handler(TIM19,          TIM19,         NUT_IRQPRI_DEF);
#endif

#if defined(HW_ADC_STM32)
Create_Handler(ADC,            ADC,           NUT_IRQPRI_DEF);
#endif
#if defined(HW_ADC1_STM32) || defined(HW_ADC1_STM32F1)
Create_Handler(ADC1,           ADC1,           NUT_IRQPRI_DEF);
#endif
#if defined(HW_ADC1_2_STM32) || defined(HW_ADC1_2_STM32F1)
Create_Handler(ADC1_2,         ADC1_2,           NUT_IRQPRI_DEF);
#endif
#if defined(HW_ADC3_STM32) || defined(HW_ADC3_STM32F1)
Create_Handler(ADC3,           ADC3,           NUT_IRQPRI_DEF);
#endif
#if defined(HW_ADC4_STM32)
Create_Handler(ADC4,           ADC4,           NUT_IRQPRI_DEF);
#endif

#if defined(HW_SDADC1_STM32)
Create_Handler(SDADC1,         SDADC1,       NUT_IRQPRI_DEF);
#endif
#if defined(HW_SDADC2_STM32)
Create_Handler(SDADC2,         SDADC2,       NUT_IRQPRI_DEF);
#endif
#if defined(HW_SDADC3_STM32)
Create_Handler(SDADC3,         SDADC3,       NUT_IRQPRI_DEF);
#endif

#if defined(HW_HASH_STM32)
Create_Handler(HASH,            HASH,         NUT_IRQPRI_DEF);
#endif

#if defined(HW_RNG_STM32)
Create_Handler(RNG,             HASH_RNG,     NUT_IRQPRI_DEF);
#endif
#if defined(HW_HASH_RNG_STM32)
Create_Handler(HASH_RNG,        HASH_RNG,     NUT_IRQPRI_DEF);
#endif
#if defined(HW_CRYP_STM32)
Create_Handler(CRYP,            CRYP,         NUT_IRQPRI_DEF);
#endif

#if defined(HW_DCMI_STM32)
Create_Handler(DCMI,            DCMI,         NUT_IRQPRI_DEF);
#endif

#if defined(HW_DMA2D_STM32)
Create_Handler(DMA2D,           DMA2D,        NUT_IRQPRI_DEF);
#endif

#if defined(HW_LTDC_STM32)
Create_Handler(LTDC,            LTDC,         NUT_IRQPRI_DEF);
#endif

#if defined(HW_FSMC_STM32)
Create_Handler(FSMC,            FSMC,         NUT_IRQPRI_DEF);
#endif

#if defined(HW_FMC_STM32)
Create_Handler(FMC,             FMC,          NUT_IRQPRI_DEF);
#endif

#if defined(HW_COMP1_2_3_STM32)
Create_Handler(COMP1_2_3,       COMP1_2_3,    NUT_IRQPRI_DEF);
#endif
#if defined(HW_COMP4_5_6_STM32)
Create_Handler(COMP4_5_6,       COMP4_5_6,    NUT_IRQPRI_DEF);
#endif
#if defined(HW_COMP_STM32)
Create_Handler(COMP,            COMP,         NUT_IRQPRI_DEF);
#endif
#if defined(HW_COMP_ACQ_STM32)
Create_Handler(COMP_ACQ,        COMP_ACQ,     NUT_IRQPRI_DEF);
#endif

#if defined(HW_CEC_STM32)
Create_Handler(CEC,             CEC,          NUT_IRQPRI_DEF);
#endif

#if defined( __FPU_PRESENT)
Create_Handler(FPU,             FPU,          NUT_IRQPRI_DEF);
#endif
