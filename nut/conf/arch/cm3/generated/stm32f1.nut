-- Do not edit!
-- Generated from nut/include/arch/cm3/stm/vendor/stm32*.h

nutarch_cm3_stm32f1 =
{
   {
   name = "nutarch_cm3_stm32f1_family",
   options =
      {
         {
            macro = "STM32F100xB",
            requires = {"HW_MCU_STM32F100xB"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM1_STM32",
               "HW_TIM15_STM32",
               "HW_TIM1_BRK_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM1_UP_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_TIM1_TRG_COM_TIM17_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM4_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_I2C2_EV_STM32",
               "HW_I2C2_STM32",
               "HW_I2C2_ER_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_STM32",
               "HW_UART3_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_CEC_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=56"},
         },
         {
            macro = "STM32F100xE",
            requires = {"HW_MCU_STM32F100xE"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM1_STM32",
               "HW_TIM15_STM32",
               "HW_TIM1_BRK_TIM15_STM32",
               "HW_TIM16_STM32",
               "HW_TIM1_UP_TIM16_STM32",
               "HW_TIM17_STM32",
               "HW_TIM1_TRG_COM_TIM17_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM4_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_I2C2_EV_STM32",
               "HW_I2C2_STM32",
               "HW_I2C2_ER_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_STM32",
               "HW_UART3_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_CEC_STM32",
               "HW_TIM12_STM32",
               "HW_TIM13_STM32",
               "HW_TIM14_STM32",
               "HW_TIM5_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_UART5_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_5_STM32",
               "HW_DMA2_CHANNEL4_STM32",
               "HW_DMA2_CHANNEL5_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=61"},
         },
         {
            macro = "STM32F101x6",
            requires = {"HW_MCU_STM32F101x6"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_SPI1_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=42"},
         },
         {
            macro = "STM32F101xB",
            requires = {"HW_MCU_STM32F101xB"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM4_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_I2C2_EV_STM32",
               "HW_I2C2_STM32",
               "HW_I2C2_ER_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_STM32",
               "HW_UART3_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=42"},
         },
         {
            macro = "STM32F101xE",
            requires = {"HW_MCU_STM32F101xE"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM4_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_I2C2_EV_STM32",
               "HW_I2C2_STM32",
               "HW_I2C2_ER_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_STM32",
               "HW_UART3_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_FSMC_STM32",
               "HW_TIM5_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_UART5_STM32",
               "HW_TIM6_STM32",
               "HW_TIM7_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_5_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=60"},
         },
         {
            macro = "STM32F101xG",
            requires = {"HW_MCU_STM32F101xG"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM9_STM32",
               "HW_TIM10_STM32",
               "HW_TIM11_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM4_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_I2C2_EV_STM32",
               "HW_I2C2_STM32",
               "HW_I2C2_ER_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_STM32",
               "HW_UART3_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_TIM12_STM32",
               "HW_TIM13_STM32",
               "HW_TIM14_STM32",
               "HW_FSMC_STM32",
               "HW_TIM5_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_UART5_STM32",
               "HW_TIM6_STM32",
               "HW_TIM7_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_5_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=60"},
         },
         {
            macro = "STM32F102x6",
            requires = {"HW_MCU_STM32F102x6"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_SPI1_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_USBWAKEUP_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=43"},
         },
         {
            macro = "STM32F102xB",
            requires = {"HW_MCU_STM32F102xB"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM4_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_I2C2_EV_STM32",
               "HW_I2C2_STM32",
               "HW_I2C2_ER_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_STM32",
               "HW_UART3_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_USBWAKEUP_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=43"},
         },
         {
            macro = "STM32F103x6",
            requires = {"HW_MCU_STM32F103x6"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_2_STM32",
               "HW_USB_HP_CAN1_TX_STM32",
               "HW_USB_LP_CAN1_RX0_STM32",
               "HW_CAN1_RX1_STM32",
               "HW_CAN1_SCE_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_STM32",
               "HW_TIM1_UP_STM32",
               "HW_TIM1_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_SPI1_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_USBWAKEUP_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=43"},
         },
         {
            macro = "STM32F103xB",
            requires = {"HW_MCU_STM32F103xB"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_2_STM32",
               "HW_USB_HP_CAN1_TX_STM32",
               "HW_USB_LP_CAN1_RX0_STM32",
               "HW_CAN1_RX1_STM32",
               "HW_CAN1_SCE_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_STM32",
               "HW_TIM1_UP_STM32",
               "HW_TIM1_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM4_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_I2C2_EV_STM32",
               "HW_I2C2_STM32",
               "HW_I2C2_ER_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_STM32",
               "HW_UART3_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_USBWAKEUP_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=43"},
         },
         {
            macro = "STM32F103xE",
            requires = {"HW_MCU_STM32F103xE"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_2_STM32",
               "HW_USB_HP_CAN1_TX_STM32",
               "HW_USB_LP_CAN1_RX0_STM32",
               "HW_CAN1_RX1_STM32",
               "HW_CAN1_SCE_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_STM32",
               "HW_TIM1_UP_STM32",
               "HW_TIM1_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM4_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_I2C2_EV_STM32",
               "HW_I2C2_STM32",
               "HW_I2C2_ER_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_STM32",
               "HW_UART3_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_USBWAKEUP_STM32",
               "HW_TIM8_STM32",
               "HW_TIM8_BRK_STM32",
               "HW_TIM8_UP_STM32",
               "HW_TIM8_TRG_COM_STM32",
               "HW_TIM8_CC_STM32",
               "HW_ADC3_STM32",
               "HW_FSMC_STM32",
               "HW_SDIO_STM32",
               "HW_TIM5_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_UART5_STM32",
               "HW_TIM6_STM32",
               "HW_TIM7_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_5_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=60"},
         },
         {
            macro = "STM32F103xG",
            requires = {"HW_MCU_STM32F103xG"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_2_STM32",
               "HW_USB_HP_CAN1_TX_STM32",
               "HW_USB_LP_CAN1_RX0_STM32",
               "HW_CAN1_RX1_STM32",
               "HW_CAN1_SCE_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM1_STM32",
               "HW_TIM9_STM32",
               "HW_TIM1_BRK_TIM9_STM32",
               "HW_TIM10_STM32",
               "HW_TIM1_UP_TIM10_STM32",
               "HW_TIM11_STM32",
               "HW_TIM1_TRG_COM_TIM11_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM4_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_I2C2_EV_STM32",
               "HW_I2C2_STM32",
               "HW_I2C2_ER_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_STM32",
               "HW_UART3_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_USBWAKEUP_STM32",
               "HW_TIM8_STM32",
               "HW_TIM12_STM32",
               "HW_TIM8_BRK_TIM12_STM32",
               "HW_TIM13_STM32",
               "HW_TIM8_UP_TIM13_STM32",
               "HW_TIM14_STM32",
               "HW_TIM8_TRG_COM_TIM14_STM32",
               "HW_TIM8_CC_STM32",
               "HW_ADC3_STM32",
               "HW_FSMC_STM32",
               "HW_SDIO_STM32",
               "HW_TIM5_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_UART5_STM32",
               "HW_TIM6_STM32",
               "HW_TIM7_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_5_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=60"},
         },
         {
            macro = "STM32F105xC",
            requires = {"HW_MCU_STM32F105xC"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_2_STM32",
               "HW_CAN1_TX_STM32",
               "HW_CAN1_RX0_STM32",
               "HW_CAN1_RX1_STM32",
               "HW_CAN1_SCE_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_STM32",
               "HW_TIM1_UP_STM32",
               "HW_TIM1_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM4_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_I2C2_EV_STM32",
               "HW_I2C2_STM32",
               "HW_I2C2_ER_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_STM32",
               "HW_UART3_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_OTG_FS_WKUP_STM32",
               "HW_TIM5_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_UART5_STM32",
               "HW_TIM6_STM32",
               "HW_TIM7_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_STM32",
               "HW_DMA2_CHANNEL5_STM32",
               "HW_CAN2_TX_STM32",
               "HW_CAN2_RX0_STM32",
               "HW_CAN2_RX1_STM32",
               "HW_CAN2_SCE_STM32",
               "HW_OTG_FS_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=68"},
         },
         {
            macro = "STM32F107xC",
            requires = {"HW_MCU_STM32F107xC"},
            default = 1,
            provides =
            {
               "HW_NONMASKABLEINT_STM32",
               "HW_HARDFAULT_STM32",
               "HW_MEMORYMANAGEMENT_STM32",
               "HW_BUSFAULT_STM32",
               "HW_USAGEFAULT_STM32",
               "HW_SVCALL_STM32",
               "HW_DEBUGMONITOR_STM32",
               "HW_PENDSV_STM32",
               "HW_SYSTICK_STM32",
               "HW_WWDG_STM32",
               "HW_PVD_STM32",
               "HW_TAMPER_STM32",
               "HW_RTC_STM32",
               "HW_FLASH_STM32",
               "HW_RCC_STM32",
               "HW_EXTI0_STM32",
               "HW_EXTI1_STM32",
               "HW_EXTI2_STM32",
               "HW_EXTI3_STM32",
               "HW_EXTI4_STM32",
               "HW_DMA1_CHANNEL1_STM32",
               "HW_DMA1_CHANNEL2_STM32",
               "HW_DMA1_CHANNEL3_STM32",
               "HW_DMA1_CHANNEL4_STM32",
               "HW_DMA1_CHANNEL5_STM32",
               "HW_DMA1_CHANNEL6_STM32",
               "HW_DMA1_CHANNEL7_STM32",
               "HW_ADC1_2_STM32",
               "HW_CAN1_TX_STM32",
               "HW_CAN1_RX0_STM32",
               "HW_CAN1_RX1_STM32",
               "HW_CAN1_SCE_STM32",
               "HW_EXTI9_5_STM32",
               "HW_TIM1_STM32",
               "HW_TIM1_BRK_STM32",
               "HW_TIM1_UP_STM32",
               "HW_TIM1_TRG_COM_STM32",
               "HW_TIM1_CC_STM32",
               "HW_TIM2_STM32",
               "HW_TIM3_STM32",
               "HW_TIM4_STM32",
               "HW_I2C1_EV_STM32",
               "HW_I2C1_STM32",
               "HW_I2C1_ER_STM32",
               "HW_I2C2_EV_STM32",
               "HW_I2C2_STM32",
               "HW_I2C2_ER_STM32",
               "HW_SPI1_STM32",
               "HW_SPI2_STM32",
               "HW_USART1_STM32",
               "HW_UART1_STM32",
               "HW_USART2_STM32",
               "HW_UART2_STM32",
               "HW_USART3_STM32",
               "HW_UART3_STM32",
               "HW_EXTI15_10_STM32",
               "HW_RTC_ALARM_STM32",
               "HW_OTG_FS_WKUP_STM32",
               "HW_TIM5_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_UART5_STM32",
               "HW_TIM6_STM32",
               "HW_TIM7_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_STM32",
               "HW_DMA2_CHANNEL5_STM32",
               "HW_ETH_STM32",
               "HW_ETH_WKUP_STM32",
               "HW_CAN2_TX_STM32",
               "HW_CAN2_RX0_STM32",
               "HW_CAN2_RX1_STM32",
               "HW_CAN2_SCE_STM32",
               "HW_OTG_FS_STM32",
            },
            file = "include/cfg/arch.h",
            makedefs = {"UCPFLAGS+=-DIRQn_MAX=68"},
         },
      }
   }
}
