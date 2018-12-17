-- Do not edit!
-- Generated from nut/include/arch/cm3/stm/vendor/stm32*.h

nutarch_cm3_stm32f0 =
{
   {
   name = "nutarch_cm3_stm32f0_family",
   options =
      {
         {
            macro = "STM32F030x6",
            requires = {"HW_MCU_STM32F030x6"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_444",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_STM32",
               "HW_ADC1_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM3_STM32",
               "HW_TIM14_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_SPI1_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=28",
               "HWDEF += -DSTM32DIE=444",
            },
         },
         {
            macro = "STM32F030x8",
            requires = {"HW_MCU_STM32F030x8"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_440",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_STM32",
               "HW_ADC1_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM3_STM32",
               "HW_TIM6_STM32",
               "HW_TIM14_STM32",
               "HW_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_I2C2_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=29",
               "HWDEF += -DSTM32DIE=440",
            },
         },
         {
            macro = "STM32F030xC",
            requires = {"HW_MCU_STM32F030xC"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_444",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_STM32",
               "HW_ADC1_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM3_STM32",
               "HW_TIM6_STM32",
               "HW_TIM7_STM32",
               "HW_TIM14_STM32",
               "HW_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_I2C2_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_6_STM32",
               "HW_UART3_6_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=30",
               "HWDEF += -DSTM32DIE=444",
            },
         },
         {
            macro = "STM32F031x6",
            requires = {"HW_MCU_STM32F031x6"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_444",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_STM32",
               "HW_ADC1_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM14_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_SPI1_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_TIM2_32BIT_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=28",
               "HWDEF += -DSTM32DIE=444",
            },
         },
         {
            macro = "STM32F038xx",
            requires = {"HW_MCU_STM32F038xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_444",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_STM32",
               "HW_ADC1_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM14_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_SPI1_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_TIM2_32BIT_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=28",
               "HWDEF += -DSTM32DIE=444",
            },
         },
         {
            macro = "STM32F042x6",
            requires = {"HW_MCU_STM32F042x6"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_445",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_VDDIO2_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_CRS_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_TSC_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_STM32",
               "HW_ADC1_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM14_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_CEC_CAN_STM32",
               "HW_USB_STM32",
               "HW_TIM2_32BIT_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=32",
               "HWDEF += -DSTM32DIE=445",
            },
         },
         {
            macro = "STM32F048xx",
            requires = {"HW_MCU_STM32F048xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_445",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_VDDIO2_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_CRS_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_TSC_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_STM32",
               "HW_ADC1_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM14_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_CEC_CAN_STM32",
               "HW_USB_STM32",
               "HW_TIM2_32BIT_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=32",
               "HWDEF += -DSTM32DIE=445",
            },
         },
         {
            macro = "STM32F051x8",
            requires = {"HW_MCU_STM32F051x8"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_440",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_TSC_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_STM32",
               "HW_ADC1_COMP_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM14_STM32",
               "HW_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_I2C2_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_CEC_CAN_STM32",
               "HW_TIM2_32BIT_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=31",
               "HWDEF += -DSTM32DIE=440",
            },
         },
         {
            macro = "STM32F058xx",
            requires = {"HW_MCU_STM32F058xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_440",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_TSC_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_STM32",
               "HW_ADC1_COMP_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM14_STM32",
               "HW_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_I2C2_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_CEC_CAN_STM32",
               "HW_TIM2_32BIT_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=31",
               "HWDEF += -DSTM32DIE=440",
            },
         },
         {
            macro = "STM32F070x6",
            requires = {"HW_MCU_STM32F070x6"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_445",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_STM32",
               "HW_ADC1_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM3_STM32",
               "HW_TIM14_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_SPI1_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USB_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=32",
               "HWDEF += -DSTM32DIE=445",
            },
         },
         {
            macro = "STM32F070xB",
            requires = {"HW_MCU_STM32F070xB"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_448",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_STM32",
               "HW_ADC1_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM3_STM32",
               "HW_TIM6_STM32",
               "HW_TIM7_STM32",
               "HW_TIM14_STM32",
               "HW_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_I2C2_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_4_STM32",
               "HW_UART3_4_STM32",
               "HW_USB_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=32",
               "HWDEF += -DSTM32DIE=448",
            },
         },
         {
            macro = "STM32F071xB",
            requires = {"HW_MCU_STM32F071xB"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_448",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_VDDIO2_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_CRS_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_TSC_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_6_7_STM32",
               "HW_ADC1_COMP_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_TIM14_STM32",
               "HW_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_I2C2_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_4_STM32",
               "HW_UART3_4_STM32",
               "HW_CEC_CAN_STM32",
               "HW_TIM2_32BIT_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=31",
               "HWDEF += -DSTM32DIE=448",
            },
         },
         {
            macro = "STM32F072xB",
            requires = {"HW_MCU_STM32F072xB"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_448",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_VDDIO2_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_CRS_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_TSC_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_6_7_STM32",
               "HW_ADC1_COMP_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_TIM14_STM32",
               "HW_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_I2C2_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_4_STM32",
               "HW_UART3_4_STM32",
               "HW_CEC_CAN_STM32",
               "HW_USB_STM32",
               "HW_TIM2_32BIT_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=32",
               "HWDEF += -DSTM32DIE=448",
            },
         },
         {
            macro = "STM32F078xx",
            requires = {"HW_MCU_STM32F078xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_448",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_VDDIO2_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_CRS_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_TSC_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_3_STM32",
               "HW_DMA1_CHANNEL4_5_6_7_STM32",
               "HW_ADC1_COMP_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_TIM14_STM32",
               "HW_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_I2C2_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_4_STM32",
               "HW_UART3_4_STM32",
               "HW_CEC_CAN_STM32",
               "HW_USB_STM32",
               "HW_TIM2_32BIT_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=32",
               "HWDEF += -DSTM32DIE=448",
            },
         },
         {
            macro = "STM32F091xC",
            requires = {"HW_MCU_STM32F091xC"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_442",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_VDDIO2_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_CRS_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_TSC_STM32",
               "HW_DMA1_CH1_STM32",
               "HW_DMA1_CH2_3_DMA2_CH1_2_STM32",
               "HW_DMA1_CH4_7_DMA2_CH3_5_STM32",
               "HW_ADC1_COMP_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_TIM14_STM32",
               "HW_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_I2C2_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_8_STM32",
               "HW_UART3_8_STM32",
               "HW_CEC_CAN_STM32",
               "HW_TIM2_32BIT_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=31",
               "HWDEF += -DSTM32DIE=442",
            },
         },
         {
            macro = "STM32F098xx",
            requires = {"HW_MCU_STM32F098xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_442",
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_SVC_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_VDDIO2_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_CRS_STM32",
               "HW_EXTI0_1_STM32",
               "HW_EXTI2_3_STM32",
               "HW_EXTI4_15_STM32",
               "HW_TSC_STM32",
               "HW_DMA1_CH1_STM32",
               "HW_DMA1_CH2_3_DMA2_CH1_2_STM32",
               "HW_DMA1_CH4_7_DMA2_CH3_5_STM32",
               "HW_ADC1_COMP_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_UP_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_TIM14_STM32",
               "HW_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_I2C1_STM32",
               "HW_I2C2_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_8_STM32",
               "HW_UART3_8_STM32",
               "HW_CEC_CAN_STM32",
               "HW_TIM2_32BIT_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=31",
               "HWDEF += -DSTM32DIE=442",
            },
         },
      }
   }
}
