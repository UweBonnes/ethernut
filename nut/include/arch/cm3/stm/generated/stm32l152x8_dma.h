/* Do not edit! Generated automatically from /devel/cubemx/db/mcu/IP/DMA-STM32L152x8_dma_v1_0_Modes.xml.*/

#if !defined(_STM32L152x8_DMA_H_)
# define _STM32L152x8_DMA_H_

#define STM32_DMA_VERSION 10

#define DMA_ADC            {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE}, {}}
#define DMA_DAC_CH1        {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE}, {}}
#define DMA_DAC_CH2        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE}, {}}
#define DMA_I2C1_RX        {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE}, {}}
#define DMA_I2C1_TX        {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE}, {}}
#define DMA_I2C2_RX        {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE}, {}}
#define DMA_I2C2_TX        {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE}, {}}
#define DMA_SPI1_RX        {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE}, {}}
#define DMA_SPI1_TX        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE}, {}}
#define DMA_SPI2_RX        {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE}, {}}
#define DMA_SPI2_TX        {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM2_CH1       {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM2_CH2       {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM2_CH3       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM2_CH4       {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM2_UP        {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM3_CH1       {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM3_CH3       {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM3_CH4       {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM3_TRIG      {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM3_UP        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM4_CH1       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM4_CH2       {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM4_CH3       {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM4_UP        {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM6_UP        {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE}, {}}
#define DMA_TIM7_UP        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE}, {}}
#define DMA_USART1_RX      {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE}, {}}
#define DMA_UART1_RX    DMA_USART1_RX
#define DMA_USART1_TX      {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE}, {}}
#define DMA_UART1_TX    DMA_USART1_TX
#define DMA_USART2_RX      {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE}, {}}
#define DMA_UART2_RX    DMA_USART2_RX
#define DMA_USART2_TX      {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE}, {}}
#define DMA_UART2_TX    DMA_USART2_TX
#define DMA_USART3_RX      {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE}, {}}
#define DMA_UART3_RX    DMA_USART3_RX
#define DMA_USART3_TX      {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE}}
#define DMA_UART3_TX    DMA_USART3_TX

#endif
