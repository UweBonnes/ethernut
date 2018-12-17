/* Do not edit! Generated automatically from /devel/cubemx/db/mcu/IP/DMA-STM32F051_dma_v1_0_Modes.xml.*/

#if !defined(_STM32F051_DMA_H_)
# define _STM32F051_DMA_H_

#define STM32_DMA_VERSION 10
#define STM32_DMA_SYSCFG_REMAP

#define DMA_ADC            {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_ADC_DMA_RMP_Msk, 0},\
                            {DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , SYSCFG_CFGR1_ADC_DMA_RMP_Msk, SYSCFG_CFGR1_ADC_DMA_RMP}, {}}
#define DMA_DAC1_CH1       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_I2C1_RX        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_I2C1_TX        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_I2C2_RX        {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_I2C2_TX        {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_SPI1_RX        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_SPI1_TX        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_SPI2_RX        {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_SPI2_TX        {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM15_CH1      {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM15_COM      {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM15_TRIG     {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM15_UP       {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM16_CH1      {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , SYSCFG_CFGR1_TIM16_DMA_RMP_Msk, 0},\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , SYSCFG_CFGR1_TIM16_DMA_RMP_Msk, SYSCFG_CFGR1_TIM16_DMA_RMP}, {}}
#define DMA_TIM16_UP       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , SYSCFG_CFGR1_TIM16_DMA_RMP_Msk, 0},\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , SYSCFG_CFGR1_TIM16_DMA_RMP_Msk, SYSCFG_CFGR1_TIM16_DMA_RMP}, {}}
#define DMA_TIM17_CH1      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM17_DMA_RMP_Msk, 0},\
                            {DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , SYSCFG_CFGR1_TIM17_DMA_RMP_Msk, SYSCFG_CFGR1_TIM17_DMA_RMP}, {}}
#define DMA_TIM17_UP       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM17_DMA_RMP_Msk, 0},\
                            {DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , SYSCFG_CFGR1_TIM17_DMA_RMP_Msk, SYSCFG_CFGR1_TIM17_DMA_RMP}, {}}
#define DMA_TIM1_CH1       {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM1_CH2       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM1_CH3       {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM1_CH4       {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM1_COM       {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM1_TRIG      {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM1_UP        {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM2_CH1       {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM2_CH2       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM2_CH3       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM2_CH4       {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM2_UP        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM3_CH1       {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM3_CH3       {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM3_CH4       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM3_TRIG      {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM3_UP        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_TIM6_UP        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_USART1_RX      {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , SYSCFG_CFGR1_USART1RX_DMA_RMP_Msk, 0},\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , SYSCFG_CFGR1_USART1RX_DMA_RMP_Msk, SYSCFG_CFGR1_USART1RX_DMA_RMP}, {}}
#define DMA_UART1_RX    DMA_USART1_RX
#define DMA_USART1_TX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , SYSCFG_CFGR1_USART1TX_DMA_RMP_Msk, 0},\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , SYSCFG_CFGR1_USART1TX_DMA_RMP_Msk, SYSCFG_CFGR1_USART1TX_DMA_RMP}, {}}
#define DMA_UART1_TX    DMA_USART1_TX
#define DMA_USART2_RX      {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}, {}}
#define DMA_UART2_RX    DMA_USART2_RX
#define DMA_USART2_TX      {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , 0, 0}}
#define DMA_UART2_TX    DMA_USART2_TX
#define STM32_DMA_COMBINED_IRQ

#endif
