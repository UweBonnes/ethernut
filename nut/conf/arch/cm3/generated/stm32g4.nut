-- Do not edit!
-- Generated from nut/include/arch/cm3/stm/vendor/stm32*.h

nutarch_cm3_stm32g4 =
{
   {
   name = "nutarch_cm3_stm32g4_family",
   options =
      {
         {
            macro = "STM32G431xx",
            requires = {"HW_MCU_STM32G431xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_468",
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
               "HW_PVD_PVM_STM32",
               "HW_RTC_TAMP_LSECSS_STM32",
               "HW_RTC_WKUP_STM32",
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
               "HW_ADC1_2_STM32",
               "HW_USB_HP_STM32",
               "HW_USB_LP_STM32",
               "HW_FDCAN1_IT0_STM32",
               "HW_FDCAN1_IT1_STM32",
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
               "HW_USBWAKEUP_STM32",
               "HW_TIM8_STM32",
               "HW_TIM8_BRK_STM32",
               "HW_TIM8_UP_STM32",
               "HW_TIM8_TRG_COM_STM32",
               "HW_TIM8_CC_STM32",
               "HW_LPTIM1_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_STM32",
               "HW_DMA2_CHANNEL5_STM32",
               "HW_UCPD1_STM32",
               "HW_COMP1_2_3_STM32",
               "HW_CRS_STM32",
               "HW_SAI1_STM32",
               "HW_FPU_STM32",
               "HW_RNG_STM32",
               "HW_LPUART1_STM32",
               "HW_I2C3_EV_STM32",
               "HW_I2C3_STM32",
               "HW_I2C3_ER_STM32",
               "HW_DMAMUX_OVR_STM32",
               "HW_DMA2_CHANNEL6_STM32",
               "HW_CORDIC_STM32",
               "HW_FMAC_STM32",
               "HW_UART_FIFO_STM32",
               "HW_TIM2_32BIT_STM32",
               "HW_USART1_AUTOBAUD",
               "HW_UART1_AUTOBAUD",
               "HW_USART2_AUTOBAUD",
               "HW_UART2_AUTOBAUD",
               "HW_USART3_AUTOBAUD",
               "HW_UART3_AUTOBAUD",
               "HW_UART4_AUTOBAUD",
               "HW_UART_AUTOBAUD",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=102",
               "HWDEF += -DSTM32DIE=468",
               "HWDEF += -DVENDOR=\"<arch/cm3/stm/vendor/stm32g431xx.h>\"",
               "HWDEF += -DPINMUXHEADER=\"<arch/cm3/stm/generated/stm32g43x_pinmux.h>\"",
               "HWDEF += -DDMACHANNELHEADER=\"<arch/cm3/stm/generated/stm32g484_dma.h>\"",
            },
         },
         {
            macro = "STM32G441xx",
            requires = {"HW_MCU_STM32G441xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_468",
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
               "HW_PVD_PVM_STM32",
               "HW_RTC_TAMP_LSECSS_STM32",
               "HW_RTC_WKUP_STM32",
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
               "HW_ADC1_2_STM32",
               "HW_USB_HP_STM32",
               "HW_USB_LP_STM32",
               "HW_FDCAN1_IT0_STM32",
               "HW_FDCAN1_IT1_STM32",
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
               "HW_USBWAKEUP_STM32",
               "HW_TIM8_STM32",
               "HW_TIM8_BRK_STM32",
               "HW_TIM8_UP_STM32",
               "HW_TIM8_TRG_COM_STM32",
               "HW_TIM8_CC_STM32",
               "HW_LPTIM1_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_STM32",
               "HW_DMA2_CHANNEL5_STM32",
               "HW_UCPD1_STM32",
               "HW_COMP1_2_3_STM32",
               "HW_CRS_STM32",
               "HW_SAI1_STM32",
               "HW_FPU_STM32",
               "HW_AES_STM32",
               "HW_RNG_STM32",
               "HW_LPUART1_STM32",
               "HW_I2C3_EV_STM32",
               "HW_I2C3_STM32",
               "HW_I2C3_ER_STM32",
               "HW_DMAMUX_OVR_STM32",
               "HW_DMA2_CHANNEL6_STM32",
               "HW_CORDIC_STM32",
               "HW_FMAC_STM32",
               "HW_UART_FIFO_STM32",
               "HW_TIM2_32BIT_STM32",
               "HW_USART1_AUTOBAUD",
               "HW_UART1_AUTOBAUD",
               "HW_USART2_AUTOBAUD",
               "HW_UART2_AUTOBAUD",
               "HW_USART3_AUTOBAUD",
               "HW_UART3_AUTOBAUD",
               "HW_UART4_AUTOBAUD",
               "HW_UART_AUTOBAUD",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=102",
               "HWDEF += -DSTM32DIE=468",
               "HWDEF += -DVENDOR=\"<arch/cm3/stm/vendor/stm32g441xx.h>\"",
               "HWDEF += -DPINMUXHEADER=\"<arch/cm3/stm/generated/stm32g43x_pinmux.h>\"",
               "HWDEF += -DDMACHANNELHEADER=\"<arch/cm3/stm/generated/stm32g484_dma.h>\"",
            },
         },
         {
            macro = "STM32G471xx",
            requires = {"HW_MCU_STM32G471xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_469",
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
               "HW_PVD_PVM_STM32",
               "HW_RTC_TAMP_LSECSS_STM32",
               "HW_RTC_WKUP_STM32",
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
               "HW_USB_HP_STM32",
               "HW_USB_LP_STM32",
               "HW_FDCAN1_IT0_STM32",
               "HW_FDCAN1_IT1_STM32",
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
               "HW_USBWAKEUP_STM32",
               "HW_TIM8_STM32",
               "HW_TIM8_BRK_STM32",
               "HW_TIM8_UP_STM32",
               "HW_TIM8_TRG_COM_STM32",
               "HW_TIM8_CC_STM32",
               "HW_ADC3_STM32",
               "HW_LPTIM1_STM32",
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
               "HW_DMA2_CHANNEL4_STM32",
               "HW_DMA2_CHANNEL5_STM32",
               "HW_UCPD1_STM32",
               "HW_COMP1_2_3_STM32",
               "HW_CRS_STM32",
               "HW_SAI1_STM32",
               "HW_FPU_STM32",
               "HW_I2C4_EV_STM32",
               "HW_I2C4_STM32",
               "HW_I2C4_ER_STM32",
               "HW_SPI4_STM32",
               "HW_FDCAN2_IT0_STM32",
               "HW_FDCAN2_IT1_STM32",
               "HW_RNG_STM32",
               "HW_LPUART1_STM32",
               "HW_I2C3_EV_STM32",
               "HW_I2C3_STM32",
               "HW_I2C3_ER_STM32",
               "HW_DMAMUX_OVR_STM32",
               "HW_DMA1_CHANNEL8_STM32",
               "HW_DMA2_CHANNEL6_STM32",
               "HW_DMA2_CHANNEL7_STM32",
               "HW_DMA2_CHANNEL8_STM32",
               "HW_CORDIC_STM32",
               "HW_FMAC_STM32",
               "HW_UART_FIFO_STM32",
               "HW_TIM2_32BIT_STM32",
               "HW_TIM5_32BIT_STM32",
               "HW_USART1_AUTOBAUD",
               "HW_UART1_AUTOBAUD",
               "HW_USART2_AUTOBAUD",
               "HW_UART2_AUTOBAUD",
               "HW_USART3_AUTOBAUD",
               "HW_UART3_AUTOBAUD",
               "HW_UART4_AUTOBAUD",
               "HW_UART5_AUTOBAUD",
               "HW_UART_AUTOBAUD",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=102",
               "HWDEF += -DSTM32DIE=469",
               "HWDEF += -DVENDOR=\"<arch/cm3/stm/vendor/stm32g471xx.h>\"",
               "HWDEF += -DPINMUXHEADER=\"<arch/cm3/stm/generated/stm32g47x_pinmux.h>\"",
               "HWDEF += -DDMACHANNELHEADER=\"<arch/cm3/stm/generated/stm32g484_dma.h>\"",
            },
         },
         {
            macro = "STM32G473xx",
            requires = {"HW_MCU_STM32G473xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_469",
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
               "HW_PVD_PVM_STM32",
               "HW_RTC_TAMP_LSECSS_STM32",
               "HW_RTC_WKUP_STM32",
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
               "HW_USB_HP_STM32",
               "HW_USB_LP_STM32",
               "HW_FDCAN1_IT0_STM32",
               "HW_FDCAN1_IT1_STM32",
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
               "HW_USBWAKEUP_STM32",
               "HW_TIM8_STM32",
               "HW_TIM8_BRK_STM32",
               "HW_TIM8_UP_STM32",
               "HW_TIM8_TRG_COM_STM32",
               "HW_TIM8_CC_STM32",
               "HW_ADC3_STM32",
               "HW_FMC_STM32",
               "HW_LPTIM1_STM32",
               "HW_TIM5_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_UART5_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_TIM7_DAC_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_STM32",
               "HW_DMA2_CHANNEL5_STM32",
               "HW_ADC4_STM32",
               "HW_ADC5_STM32",
               "HW_UCPD1_STM32",
               "HW_COMP1_2_3_STM32",
               "HW_COMP7_STM32",
               "HW_CRS_STM32",
               "HW_SAI1_STM32",
               "HW_TIM20_STM32",
               "HW_TIM20_BRK_STM32",
               "HW_TIM20_UP_STM32",
               "HW_TIM20_TRG_COM_STM32",
               "HW_TIM20_CC_STM32",
               "HW_FPU_STM32",
               "HW_I2C4_EV_STM32",
               "HW_I2C4_STM32",
               "HW_I2C4_ER_STM32",
               "HW_SPI4_STM32",
               "HW_FDCAN2_IT0_STM32",
               "HW_FDCAN2_IT1_STM32",
               "HW_FDCAN3_IT0_STM32",
               "HW_FDCAN3_IT1_STM32",
               "HW_RNG_STM32",
               "HW_LPUART1_STM32",
               "HW_I2C3_EV_STM32",
               "HW_I2C3_STM32",
               "HW_I2C3_ER_STM32",
               "HW_DMAMUX_OVR_STM32",
               "HW_QUADSPI_STM32",
               "HW_DMA1_CHANNEL8_STM32",
               "HW_DMA2_CHANNEL6_STM32",
               "HW_DMA2_CHANNEL7_STM32",
               "HW_DMA2_CHANNEL8_STM32",
               "HW_CORDIC_STM32",
               "HW_FMAC_STM32",
               "HW_UART_FIFO_STM32",
               "HW_TIM2_32BIT_STM32",
               "HW_TIM5_32BIT_STM32",
               "HW_USART1_AUTOBAUD",
               "HW_UART1_AUTOBAUD",
               "HW_USART2_AUTOBAUD",
               "HW_UART2_AUTOBAUD",
               "HW_USART3_AUTOBAUD",
               "HW_UART3_AUTOBAUD",
               "HW_UART4_AUTOBAUD",
               "HW_UART5_AUTOBAUD",
               "HW_UART_AUTOBAUD",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=102",
               "HWDEF += -DSTM32DIE=469",
               "HWDEF += -DVENDOR=\"<arch/cm3/stm/vendor/stm32g473xx.h>\"",
               "HWDEF += -DPINMUXHEADER=\"<arch/cm3/stm/generated/stm32g47x_pinmux.h>\"",
               "HWDEF += -DDMACHANNELHEADER=\"<arch/cm3/stm/generated/stm32g484_dma.h>\"",
            },
         },
         {
            macro = "STM32G474xx",
            requires = {"HW_MCU_STM32G474xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_469",
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
               "HW_PVD_PVM_STM32",
               "HW_RTC_TAMP_LSECSS_STM32",
               "HW_RTC_WKUP_STM32",
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
               "HW_USB_HP_STM32",
               "HW_USB_LP_STM32",
               "HW_FDCAN1_IT0_STM32",
               "HW_FDCAN1_IT1_STM32",
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
               "HW_USBWAKEUP_STM32",
               "HW_TIM8_STM32",
               "HW_TIM8_BRK_STM32",
               "HW_TIM8_UP_STM32",
               "HW_TIM8_TRG_COM_STM32",
               "HW_TIM8_CC_STM32",
               "HW_ADC3_STM32",
               "HW_FMC_STM32",
               "HW_LPTIM1_STM32",
               "HW_TIM5_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_UART5_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_TIM7_DAC_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_STM32",
               "HW_DMA2_CHANNEL5_STM32",
               "HW_ADC4_STM32",
               "HW_ADC5_STM32",
               "HW_UCPD1_STM32",
               "HW_COMP1_2_3_STM32",
               "HW_COMP7_STM32",
               "HW_HRTIM1_MASTER_STM32",
               "HW_HRTIM1_TIMA_STM32",
               "HW_HRTIM1_TIMB_STM32",
               "HW_HRTIM1_TIMC_STM32",
               "HW_HRTIM1_TIMD_STM32",
               "HW_HRTIM1_TIME_STM32",
               "HW_HRTIM1_FLT_STM32",
               "HW_HRTIM1_TIMF_STM32",
               "HW_CRS_STM32",
               "HW_SAI1_STM32",
               "HW_TIM20_STM32",
               "HW_TIM20_BRK_STM32",
               "HW_TIM20_UP_STM32",
               "HW_TIM20_TRG_COM_STM32",
               "HW_TIM20_CC_STM32",
               "HW_FPU_STM32",
               "HW_I2C4_EV_STM32",
               "HW_I2C4_STM32",
               "HW_I2C4_ER_STM32",
               "HW_SPI4_STM32",
               "HW_FDCAN2_IT0_STM32",
               "HW_FDCAN2_IT1_STM32",
               "HW_FDCAN3_IT0_STM32",
               "HW_FDCAN3_IT1_STM32",
               "HW_RNG_STM32",
               "HW_LPUART1_STM32",
               "HW_I2C3_EV_STM32",
               "HW_I2C3_STM32",
               "HW_I2C3_ER_STM32",
               "HW_DMAMUX_OVR_STM32",
               "HW_QUADSPI_STM32",
               "HW_DMA1_CHANNEL8_STM32",
               "HW_DMA2_CHANNEL6_STM32",
               "HW_DMA2_CHANNEL7_STM32",
               "HW_DMA2_CHANNEL8_STM32",
               "HW_CORDIC_STM32",
               "HW_FMAC_STM32",
               "HW_UART_FIFO_STM32",
               "HW_TIM2_32BIT_STM32",
               "HW_TIM5_32BIT_STM32",
               "HW_USART1_AUTOBAUD",
               "HW_UART1_AUTOBAUD",
               "HW_USART2_AUTOBAUD",
               "HW_UART2_AUTOBAUD",
               "HW_USART3_AUTOBAUD",
               "HW_UART3_AUTOBAUD",
               "HW_UART4_AUTOBAUD",
               "HW_UART5_AUTOBAUD",
               "HW_UART_AUTOBAUD",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=102",
               "HWDEF += -DSTM32DIE=469",
               "HWDEF += -DVENDOR=\"<arch/cm3/stm/vendor/stm32g474xx.h>\"",
               "HWDEF += -DPINMUXHEADER=\"<arch/cm3/stm/generated/stm32g47x_pinmux.h>\"",
               "HWDEF += -DDMACHANNELHEADER=\"<arch/cm3/stm/generated/stm32g484_dma.h>\"",
            },
         },
         {
            macro = "STM32G483xx",
            requires = {"HW_MCU_STM32G483xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_469",
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
               "HW_PVD_PVM_STM32",
               "HW_RTC_TAMP_LSECSS_STM32",
               "HW_RTC_WKUP_STM32",
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
               "HW_USB_HP_STM32",
               "HW_USB_LP_STM32",
               "HW_FDCAN1_IT0_STM32",
               "HW_FDCAN1_IT1_STM32",
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
               "HW_USBWAKEUP_STM32",
               "HW_TIM8_STM32",
               "HW_TIM8_BRK_STM32",
               "HW_TIM8_UP_STM32",
               "HW_TIM8_TRG_COM_STM32",
               "HW_TIM8_CC_STM32",
               "HW_ADC3_STM32",
               "HW_FMC_STM32",
               "HW_LPTIM1_STM32",
               "HW_TIM5_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_UART5_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_TIM7_DAC_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_STM32",
               "HW_DMA2_CHANNEL5_STM32",
               "HW_ADC4_STM32",
               "HW_ADC5_STM32",
               "HW_UCPD1_STM32",
               "HW_COMP1_2_3_STM32",
               "HW_COMP7_STM32",
               "HW_CRS_STM32",
               "HW_SAI1_STM32",
               "HW_TIM20_STM32",
               "HW_TIM20_BRK_STM32",
               "HW_TIM20_UP_STM32",
               "HW_TIM20_TRG_COM_STM32",
               "HW_TIM20_CC_STM32",
               "HW_FPU_STM32",
               "HW_I2C4_EV_STM32",
               "HW_I2C4_STM32",
               "HW_I2C4_ER_STM32",
               "HW_SPI4_STM32",
               "HW_AES_STM32",
               "HW_FDCAN2_IT0_STM32",
               "HW_FDCAN2_IT1_STM32",
               "HW_FDCAN3_IT0_STM32",
               "HW_FDCAN3_IT1_STM32",
               "HW_RNG_STM32",
               "HW_LPUART1_STM32",
               "HW_I2C3_EV_STM32",
               "HW_I2C3_STM32",
               "HW_I2C3_ER_STM32",
               "HW_DMAMUX_OVR_STM32",
               "HW_QUADSPI_STM32",
               "HW_DMA1_CHANNEL8_STM32",
               "HW_DMA2_CHANNEL6_STM32",
               "HW_DMA2_CHANNEL7_STM32",
               "HW_DMA2_CHANNEL8_STM32",
               "HW_CORDIC_STM32",
               "HW_FMAC_STM32",
               "HW_UART_FIFO_STM32",
               "HW_TIM2_32BIT_STM32",
               "HW_TIM5_32BIT_STM32",
               "HW_USART1_AUTOBAUD",
               "HW_UART1_AUTOBAUD",
               "HW_USART2_AUTOBAUD",
               "HW_UART2_AUTOBAUD",
               "HW_USART3_AUTOBAUD",
               "HW_UART3_AUTOBAUD",
               "HW_UART4_AUTOBAUD",
               "HW_UART5_AUTOBAUD",
               "HW_UART_AUTOBAUD",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=102",
               "HWDEF += -DSTM32DIE=469",
               "HWDEF += -DVENDOR=\"<arch/cm3/stm/vendor/stm32g483xx.h>\"",
               "HWDEF += -DPINMUXHEADER=\"<arch/cm3/stm/generated/stm32g47x_pinmux.h>\"",
               "HWDEF += -DDMACHANNELHEADER=\"<arch/cm3/stm/generated/stm32g484_dma.h>\"",
            },
         },
         {
            macro = "STM32G484xx",
            requires = {"HW_MCU_STM32G484xx"},
            default = 1,
            provides =
            {
               "HW_MCU_STM32_DIE_469",
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
               "HW_PVD_PVM_STM32",
               "HW_RTC_TAMP_LSECSS_STM32",
               "HW_RTC_WKUP_STM32",
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
               "HW_USB_HP_STM32",
               "HW_USB_LP_STM32",
               "HW_FDCAN1_IT0_STM32",
               "HW_FDCAN1_IT1_STM32",
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
               "HW_USBWAKEUP_STM32",
               "HW_TIM8_STM32",
               "HW_TIM8_BRK_STM32",
               "HW_TIM8_UP_STM32",
               "HW_TIM8_TRG_COM_STM32",
               "HW_TIM8_CC_STM32",
               "HW_ADC3_STM32",
               "HW_FMC_STM32",
               "HW_LPTIM1_STM32",
               "HW_TIM5_STM32",
               "HW_SPI3_STM32",
               "HW_UART4_STM32",
               "HW_UART5_STM32",
               "HW_TIM6_STM32",
               "HW_TIM6_DAC_STM32",
               "HW_TIM7_STM32",
               "HW_TIM7_DAC_STM32",
               "HW_DMA2_CHANNEL1_STM32",
               "HW_DMA2_CHANNEL2_STM32",
               "HW_DMA2_CHANNEL3_STM32",
               "HW_DMA2_CHANNEL4_STM32",
               "HW_DMA2_CHANNEL5_STM32",
               "HW_ADC4_STM32",
               "HW_ADC5_STM32",
               "HW_UCPD1_STM32",
               "HW_COMP1_2_3_STM32",
               "HW_COMP7_STM32",
               "HW_HRTIM1_MASTER_STM32",
               "HW_HRTIM1_TIMA_STM32",
               "HW_HRTIM1_TIMB_STM32",
               "HW_HRTIM1_TIMC_STM32",
               "HW_HRTIM1_TIMD_STM32",
               "HW_HRTIM1_TIME_STM32",
               "HW_HRTIM1_FLT_STM32",
               "HW_HRTIM1_TIMF_STM32",
               "HW_CRS_STM32",
               "HW_SAI1_STM32",
               "HW_TIM20_STM32",
               "HW_TIM20_BRK_STM32",
               "HW_TIM20_UP_STM32",
               "HW_TIM20_TRG_COM_STM32",
               "HW_TIM20_CC_STM32",
               "HW_FPU_STM32",
               "HW_I2C4_EV_STM32",
               "HW_I2C4_STM32",
               "HW_I2C4_ER_STM32",
               "HW_SPI4_STM32",
               "HW_AES_STM32",
               "HW_FDCAN2_IT0_STM32",
               "HW_FDCAN2_IT1_STM32",
               "HW_FDCAN3_IT0_STM32",
               "HW_FDCAN3_IT1_STM32",
               "HW_RNG_STM32",
               "HW_LPUART1_STM32",
               "HW_I2C3_EV_STM32",
               "HW_I2C3_STM32",
               "HW_I2C3_ER_STM32",
               "HW_DMAMUX_OVR_STM32",
               "HW_QUADSPI_STM32",
               "HW_DMA1_CHANNEL8_STM32",
               "HW_DMA2_CHANNEL6_STM32",
               "HW_DMA2_CHANNEL7_STM32",
               "HW_DMA2_CHANNEL8_STM32",
               "HW_CORDIC_STM32",
               "HW_FMAC_STM32",
               "HW_UART_FIFO_STM32",
               "HW_TIM2_32BIT_STM32",
               "HW_TIM5_32BIT_STM32",
               "HW_USART1_AUTOBAUD",
               "HW_UART1_AUTOBAUD",
               "HW_USART2_AUTOBAUD",
               "HW_UART2_AUTOBAUD",
               "HW_USART3_AUTOBAUD",
               "HW_UART3_AUTOBAUD",
               "HW_UART4_AUTOBAUD",
               "HW_UART5_AUTOBAUD",
               "HW_UART_AUTOBAUD",
            },
            file = "include/cfg/arch.h",
            makedefs =
            {
               "UCPFLAGS+=-DIRQn_MAX=102",
               "HWDEF += -DSTM32DIE=469",
               "HWDEF += -DVENDOR=\"<arch/cm3/stm/vendor/stm32g484xx.h>\"",
               "HWDEF += -DPINMUXHEADER=\"<arch/cm3/stm/generated/stm32g47x_pinmux.h>\"",
               "HWDEF += -DDMACHANNELHEADER=\"<arch/cm3/stm/generated/stm32g484_dma.h>\"",
            },
         },
      }
   }
}
