/* Do not edit! Generated automatically from /devel/cubemx/db/mcu/IP/DMA-STM32F373_dma_v1_0_Modes.xml.*/

#if !defined(_STM32F373_DMA_H_)
# define _STM32F373_DMA_H_

#define STM32_DMA_VERSION 10
#define STM32_DMA_SYSCFG_REMAP

#define DMA_ADC1           {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_DAC1_CH1       {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM6DAC1Ch1_DMA_RMP_Msk, SYSCFG_CFGR1_TIM6DAC1Ch1_DMA_RMP},\
                            {DMA2_Channel3_BASE, &sig_DMA2_CH3  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM6DAC1Ch1_DMA_RMP_Msk, 0}, {}}
#define DMA_DAC1_CH2       {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM7DAC1Ch2_DMA_RMP_Msk, SYSCFG_CFGR1_TIM7DAC1Ch2_DMA_RMP},\
                            {DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM7DAC1Ch2_DMA_RMP_Msk, 0}, {}}
#define DMA_DAC2_CH1       {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_DAC2_DMA_RMP_Msk, SYSCFG_CFGR1_DAC2_DMA_RMP},\
                            {DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_DAC2_DMA_RMP_Msk, 0}, {}}
#define DMA_I2C1_RX        {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_I2C1_TX        {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_I2C2_RX        {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_I2C2_TX        {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_SDADC1         {{DMA2_Channel3_BASE, &sig_DMA2_CH3  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_SDADC2         {{DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_SDADC3         {{DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_SPI1_RX        {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_SPI1_TX        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_SPI2_RX        {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_SPI2_TX        {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_SPI3_RX        {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_SPI3_TX        {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM15_CH1      {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM15_COM      {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM15_TRIG     {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM15_UP       {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM16_CH1      {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM16_DMA_RMP_Msk, SYSCFG_CFGR1_TIM16_DMA_RMP},\
                            {DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM16_DMA_RMP_Msk, SYSCFG_CFGR1_TIM16_DMA_RMP}, {}}
#define DMA_TIM16_UP       {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM16_DMA_RMP_Msk, SYSCFG_CFGR1_TIM16_DMA_RMP},\
                            {DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM16_DMA_RMP_Msk, SYSCFG_CFGR1_TIM16_DMA_RMP}, {}}
#define DMA_TIM17_CH1      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM17_DMA_RMP_Msk, SYSCFG_CFGR1_TIM17_DMA_RMP},\
                            {DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM17_DMA_RMP_Msk, SYSCFG_CFGR1_TIM17_DMA_RMP},\
                            {DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM17_DMA_RMP_Msk, SYSCFG_CFGR1_TIM17_DMA_RMP}, {}}
#define DMA_TIM17_UP       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM17_DMA_RMP_Msk, SYSCFG_CFGR1_TIM17_DMA_RMP},\
                            {DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM17_DMA_RMP_Msk, SYSCFG_CFGR1_TIM17_DMA_RMP},\
                            {DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM17_DMA_RMP_Msk, SYSCFG_CFGR1_TIM17_DMA_RMP}, {}}
#define DMA_TIM18_UP       {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM18_DMA_RMP_Msk, SYSCFG_CFGR1_TIM18_DMA_RMP},\
                            {DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM18_DMA_RMP_Msk, 0}, {}}
#define DMA_TIM19_CH1      {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM19_CH2      {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM19_CH3      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM19_CH4      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM19_UP       {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM2_CH1       {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM2_CH2       {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM2_CH3       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM2_CH4       {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM2_UP        {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM3_CH1       {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM3_CH3       {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM3_CH4       {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM3_TRIG      {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM3_UP        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM4_CH1       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM4_CH2       {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM4_CH3       {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM4_UP        {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM5_CH1       {{DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM5_CH2       {{DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM5_CH3       {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM5_CH4       {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM5_TRIG      {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM5_UP        {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_TIM6_UP        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM6DAC1Ch1_DMA_RMP_Msk, SYSCFG_CFGR1_TIM6DAC1Ch1_DMA_RMP},\
                            {DMA2_Channel3_BASE, &sig_DMA2_CH3  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM6DAC1Ch1_DMA_RMP_Msk, 0}, {}}
#define DMA_TIM7_UP        {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM7DAC1Ch2_DMA_RMP_Msk, SYSCFG_CFGR1_TIM7DAC1Ch2_DMA_RMP},\
                            {DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, SYSCFG_CFGR1_TIM7DAC1Ch2_DMA_RMP_Msk, 0}, {}}
#define DMA_USART1_RX      {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_UART1_RX    DMA_USART1_RX
#define DMA_USART1_TX      {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_UART1_TX    DMA_USART1_TX
#define DMA_USART2_RX      {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_UART2_RX    DMA_USART2_RX
#define DMA_USART2_TX      {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_UART2_TX    DMA_USART2_TX
#define DMA_USART3_RX      {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, 0, 0}, {}}
#define DMA_UART3_RX    DMA_USART3_RX
#define DMA_USART3_TX      {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, 0, 0}}
#define DMA_UART3_TX    DMA_USART3_TX

#endif
