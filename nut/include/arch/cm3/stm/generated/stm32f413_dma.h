/* Do not edit! Generated automatically from /devel/cubemx/db/mcu/IP/DMA-STM32F413_dma_v2_0_Modes.xml.*/

#if !defined(_STM32F413_DMA_H_)
# define _STM32F413_DMA_H_

#define STM32_DMA_VERSION 20

#define DMA_ADC1           {{DMA2_Stream0_BASE, &sig_DMA2_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_0 },\
                            {DMA2_Stream4_BASE, &sig_DMA2_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_0 }, {}}
#define DMA_AES_IN         {{DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_2 }, {}}
#define DMA_AES_OUT        {{DMA2_Stream5_BASE, &sig_DMA2_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_2 }, {}}
#define DMA_DAC1           {{DMA1_Stream5_BASE, &sig_DMA1_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_DAC2           {{DMA1_Stream6_BASE, &sig_DMA1_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_DFSDM1_FLT0    {{DMA2_Stream0_BASE, &sig_DMA2_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_7 },\
                            {DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_DFSDM1_FLT1    {{DMA2_Stream1_BASE, &sig_DMA2_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_3 },\
                            {DMA2_Stream4_BASE, &sig_DMA2_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_DFSDM2_FLT0    {{DMA2_Stream0_BASE, &sig_DMA2_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_8 },\
                            {DMA2_Stream4_BASE, &sig_DMA2_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_8 }, {}}
#define DMA_DFSDM2_FLT1    {{DMA2_Stream1_BASE, &sig_DMA2_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_8 },\
                            {DMA2_Stream5_BASE, &sig_DMA2_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_8 }, {}}
#define DMA_DFSDM2_FLT2    {{DMA2_Stream2_BASE, &sig_DMA2_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_8 },\
                            {DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_8 }, {}}
#define DMA_DFSDM2_FLT3    {{DMA2_Stream3_BASE, &sig_DMA2_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_8 },\
                            {DMA2_Stream7_BASE, &sig_DMA2_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_8 }, {}}
#define DMA_FMPI2C1_RX     {{DMA1_Stream0_BASE, &sig_DMA1_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_7 },\
                            {DMA1_Stream3_BASE, &sig_DMA1_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_1 }, {}}
#define DMA_FMPI2C1_TX     {{DMA1_Stream1_BASE, &sig_DMA1_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_2 },\
                            {DMA1_Stream7_BASE, &sig_DMA1_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_I2C1_RX        {{DMA1_Stream0_BASE, &sig_DMA1_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_1 },\
                            {DMA1_Stream5_BASE, &sig_DMA1_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_1 }, {}}
#define DMA_I2C1_TX        {{DMA1_Stream1_BASE, &sig_DMA1_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_0 },\
                            {DMA1_Stream6_BASE, &sig_DMA1_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_1 },\
                            {DMA1_Stream7_BASE, &sig_DMA1_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_1 }, {}}
#define DMA_I2C2_RX        {{DMA1_Stream2_BASE, &sig_DMA1_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_7 },\
                            {DMA1_Stream3_BASE, &sig_DMA1_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_I2C2_TX        {{DMA1_Stream7_BASE, &sig_DMA1_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_I2C3_RX        {{DMA1_Stream1_BASE, &sig_DMA1_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_1 },\
                            {DMA1_Stream2_BASE, &sig_DMA1_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_I2C3_TX        {{DMA1_Stream4_BASE, &sig_DMA1_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_3 },\
                            {DMA1_Stream5_BASE, &sig_DMA1_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_I2S2_EXT_RX    {{DMA1_Stream3_BASE, &sig_DMA1_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_I2S2_EXT_TX    {{DMA1_Stream4_BASE, &sig_DMA1_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_2 }, {}}
#define DMA_I2S3_EXT_RX    {{DMA1_Stream0_BASE, &sig_DMA1_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_3 },\
                            {DMA1_Stream2_BASE, &sig_DMA1_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_2 }, {}}
#define DMA_I2S3_EXT_TX    {{DMA1_Stream5_BASE, &sig_DMA1_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_2 }, {}}
#define DMA_QUADSPI        {{DMA2_Stream7_BASE, &sig_DMA2_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_SAI1_A         {{DMA2_Stream1_BASE, &sig_DMA2_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_0 },\
                            {DMA2_Stream3_BASE, &sig_DMA2_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_0 }, {}}
#define DMA_SAI1_B         {{DMA2_Stream4_BASE, &sig_DMA2_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_1 },\
                            {DMA2_Stream5_BASE, &sig_DMA2_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_0 }, {}}
#define DMA_SDIO           {{DMA2_Stream3_BASE, &sig_DMA2_STREAM3, DMA_IRQ_SINGLE, 0             },\
                            {DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_SDIO_RX        {{DMA2_Stream3_BASE, &sig_DMA2_STREAM3, DMA_IRQ_SINGLE, 0             },\
                            {DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_SDIO_TX        {{DMA2_Stream3_BASE, &sig_DMA2_STREAM3, DMA_IRQ_SINGLE, 0             },\
                            {DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_SPI1_RX        {{DMA2_Stream0_BASE, &sig_DMA2_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_3 },\
                            {DMA2_Stream2_BASE, &sig_DMA2_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_SPI1_TX        {{DMA2_Stream2_BASE, &sig_DMA2_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_2 },\
                            {DMA2_Stream3_BASE, &sig_DMA2_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_3 },\
                            {DMA2_Stream5_BASE, &sig_DMA2_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_SPI2_RX        {{DMA1_Stream3_BASE, &sig_DMA1_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_0 }, {}}
#define DMA_SPI2_TX        {{DMA1_Stream4_BASE, &sig_DMA1_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_0 }, {}}
#define DMA_SPI3_RX        {{DMA1_Stream0_BASE, &sig_DMA1_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_0 },\
                            {DMA1_Stream2_BASE, &sig_DMA1_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_0 }, {}}
#define DMA_SPI3_TX        {{DMA1_Stream5_BASE, &sig_DMA1_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_0 },\
                            {DMA1_Stream7_BASE, &sig_DMA1_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_0 }, {}}
#define DMA_SPI4_RX        {{DMA2_Stream0_BASE, &sig_DMA2_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_4 },\
                            {DMA2_Stream3_BASE, &sig_DMA2_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_5 },\
                            {DMA2_Stream4_BASE, &sig_DMA2_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_SPI4_TX        {{DMA2_Stream1_BASE, &sig_DMA2_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_4 },\
                            {DMA2_Stream4_BASE, &sig_DMA2_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_SPI5_RX        {{DMA2_Stream3_BASE, &sig_DMA2_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_2 },\
                            {DMA2_Stream5_BASE, &sig_DMA2_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_SPI5_TX        {{DMA2_Stream4_BASE, &sig_DMA2_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_2 },\
                            {DMA2_Stream5_BASE, &sig_DMA2_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_5 },\
                            {DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_TIM1_CH1       {{DMA2_Stream1_BASE, &sig_DMA2_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_6 },\
                            {DMA2_Stream3_BASE, &sig_DMA2_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_6 },\
                            {DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_0 }, {}}
#define DMA_TIM1_CH2       {{DMA2_Stream2_BASE, &sig_DMA2_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_6 },\
                            {DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_0 }, {}}
#define DMA_TIM1_CH3       {{DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_0 },\
                            {DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_TIM1_CH4       {{DMA2_Stream4_BASE, &sig_DMA2_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_TIM1_COM       {{DMA2_Stream4_BASE, &sig_DMA2_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_TIM1_TRIG      {{DMA2_Stream0_BASE, &sig_DMA2_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_6 },\
                            {DMA2_Stream4_BASE, &sig_DMA2_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_TIM1_UP        {{DMA2_Stream5_BASE, &sig_DMA2_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_TIM2_CH1       {{DMA1_Stream5_BASE, &sig_DMA1_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_TIM2_CH2       {{DMA1_Stream6_BASE, &sig_DMA1_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_TIM2_CH3       {{DMA1_Stream1_BASE, &sig_DMA1_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_TIM2_CH4       {{DMA1_Stream6_BASE, &sig_DMA1_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_3 },\
                            {DMA1_Stream7_BASE, &sig_DMA1_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_TIM2_UP        {{DMA1_Stream1_BASE, &sig_DMA1_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_3 },\
                            {DMA1_Stream7_BASE, &sig_DMA1_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_3 }, {}}
#define DMA_TIM3_CH1       {{DMA1_Stream4_BASE, &sig_DMA1_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_TIM3_CH2       {{DMA1_Stream5_BASE, &sig_DMA1_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_TIM3_CH3       {{DMA1_Stream7_BASE, &sig_DMA1_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_TIM3_CH4       {{DMA1_Stream2_BASE, &sig_DMA1_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_TIM3_TRIG      {{DMA1_Stream4_BASE, &sig_DMA1_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_TIM3_UP        {{DMA1_Stream2_BASE, &sig_DMA1_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_TIM4_CH1       {{DMA1_Stream0_BASE, &sig_DMA1_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_2 }, {}}
#define DMA_TIM4_CH2       {{DMA1_Stream3_BASE, &sig_DMA1_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_2 }, {}}
#define DMA_TIM4_CH3       {{DMA1_Stream7_BASE, &sig_DMA1_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_2 }, {}}
#define DMA_TIM4_UP        {{DMA1_Stream6_BASE, &sig_DMA1_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_2 }, {}}
#define DMA_TIM5_CH1       {{DMA1_Stream2_BASE, &sig_DMA1_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_TIM5_CH2       {{DMA1_Stream4_BASE, &sig_DMA1_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_TIM5_CH3       {{DMA1_Stream0_BASE, &sig_DMA1_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_TIM5_CH4       {{DMA1_Stream1_BASE, &sig_DMA1_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_6 },\
                            {DMA1_Stream3_BASE, &sig_DMA1_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_TIM5_TRIG      {{DMA1_Stream1_BASE, &sig_DMA1_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_6 },\
                            {DMA1_Stream3_BASE, &sig_DMA1_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_TIM5_UP        {{DMA1_Stream0_BASE, &sig_DMA1_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_6 },\
                            {DMA1_Stream6_BASE, &sig_DMA1_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_TIM6_UP        {{DMA1_Stream1_BASE, &sig_DMA1_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_TIM7_UP        {{DMA1_Stream2_BASE, &sig_DMA1_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_1 },\
                            {DMA1_Stream4_BASE, &sig_DMA1_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_1 }, {}}
#define DMA_TIM8_CH1       {{DMA2_Stream2_BASE, &sig_DMA2_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_0 },\
                            {DMA2_Stream2_BASE, &sig_DMA2_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_TIM8_CH2       {{DMA2_Stream2_BASE, &sig_DMA2_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_0 },\
                            {DMA2_Stream3_BASE, &sig_DMA2_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_TIM8_CH3       {{DMA2_Stream2_BASE, &sig_DMA2_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_0 },\
                            {DMA2_Stream4_BASE, &sig_DMA2_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_TIM8_CH4       {{DMA2_Stream7_BASE, &sig_DMA2_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_TIM8_COM       {{DMA2_Stream7_BASE, &sig_DMA2_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_TIM8_TRIG      {{DMA2_Stream7_BASE, &sig_DMA2_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_TIM8_UP        {{DMA2_Stream1_BASE, &sig_DMA2_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_UART10_RX      {{DMA2_Stream0_BASE, &sig_DMA2_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_5 },\
                            {DMA2_Stream3_BASE, &sig_DMA2_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_9 }, {}}
#define DMA_UART10_TX      {{DMA2_Stream5_BASE, &sig_DMA2_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_9 },\
                            {DMA2_Stream7_BASE, &sig_DMA2_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_UART4_RX       {{DMA1_Stream2_BASE, &sig_DMA1_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_UART4_TX       {{DMA1_Stream4_BASE, &sig_DMA1_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_UART5_RX       {{DMA1_Stream0_BASE, &sig_DMA1_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_UART5_TX       {{DMA1_Stream7_BASE, &sig_DMA1_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_8 }, {}}
#define DMA_UART7_RX       {{DMA1_Stream3_BASE, &sig_DMA1_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_UART7_TX       {{DMA1_Stream1_BASE, &sig_DMA1_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_UART8_RX       {{DMA1_Stream6_BASE, &sig_DMA1_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_UART8_TX       {{DMA1_Stream0_BASE, &sig_DMA1_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_UART9_RX       {{DMA2_Stream7_BASE, &sig_DMA2_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_0 }, {}}
#define DMA_UART9_TX       {{DMA2_Stream0_BASE, &sig_DMA2_STREAM0, DMA_IRQ_SINGLE, DMA_CHANNEL_1 }, {}}
#define DMA_USART1_RX      {{DMA2_Stream2_BASE, &sig_DMA2_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_4 },\
                            {DMA2_Stream5_BASE, &sig_DMA2_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_UART1_RX    DMA_USART1_RX
#define DMA_USART1_TX      {{DMA2_Stream7_BASE, &sig_DMA2_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_UART1_TX    DMA_USART1_TX
#define DMA_USART2_RX      {{DMA1_Stream5_BASE, &sig_DMA1_STREAM5, DMA_IRQ_SINGLE, DMA_CHANNEL_4 },\
                            {DMA1_Stream7_BASE, &sig_DMA1_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_6 }, {}}
#define DMA_UART2_RX    DMA_USART2_RX
#define DMA_USART2_TX      {{DMA1_Stream6_BASE, &sig_DMA1_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_UART2_TX    DMA_USART2_TX
#define DMA_USART3_RX      {{DMA1_Stream1_BASE, &sig_DMA1_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_4 }, {}}
#define DMA_UART3_RX    DMA_USART3_RX
#define DMA_USART3_TX      {{DMA1_Stream3_BASE, &sig_DMA1_STREAM3, DMA_IRQ_SINGLE, DMA_CHANNEL_4 },\
                            {DMA1_Stream4_BASE, &sig_DMA1_STREAM4, DMA_IRQ_SINGLE, DMA_CHANNEL_7 }, {}}
#define DMA_UART3_TX    DMA_USART3_TX
#define DMA_USART6_RX      {{DMA2_Stream1_BASE, &sig_DMA2_STREAM1, DMA_IRQ_SINGLE, DMA_CHANNEL_5 },\
                            {DMA2_Stream2_BASE, &sig_DMA2_STREAM2, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }, {}}
#define DMA_UART6_RX    DMA_USART6_RX
#define DMA_USART6_TX      {{DMA2_Stream6_BASE, &sig_DMA2_STREAM6, DMA_IRQ_SINGLE, DMA_CHANNEL_5 },\
                            {DMA2_Stream7_BASE, &sig_DMA2_STREAM7, DMA_IRQ_SINGLE, DMA_CHANNEL_5 }}
#define DMA_UART6_TX    DMA_USART6_TX

#endif
