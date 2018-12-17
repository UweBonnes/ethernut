/* Do not edit! Generated automatically from /devel/cubemx/db/mcu/IP/DMA-STM32L4A6_dma_v1_1_Modes.xml.*/

#if !defined(_STM32L4A6_DMA_H_)
# define _STM32L4A6_DMA_H_

#define STM32_DMA_VERSION 11

#define DMA_ADC1           {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_0 },\
                            {DMA2_Channel3_BASE, &sig_DMA2_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_0 }, {}}
#define DMA_ADC2           {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_0 },\
                            {DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_0 }, {}}
#define DMA_ADC3           {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_0 },\
                            {DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_0 }, {}}
#define DMA_AES_IN         {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_6 },\
                            {DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_6 }, {}}
#define DMA_AES_OUT        {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_6 },\
                            {DMA2_Channel3_BASE, &sig_DMA2_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_6 }, {}}
#define DMA_DAC_CH1        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, 0             },\
                            {DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_3 }, {}}
#define DMA_DAC_CH2        {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, 0             },\
                            {DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_3 }, {}}
#define DMA_DCMI           {{DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_4 },\
                            {DMA2_Channel6_BASE, &sig_DMA2_CH6  , DMA_IRQ_SINGLE, 0             }, {}}
#define DMA_DFSDM1_FLT0    {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_0 }, {}}
#define DMA_DFSDM1_FLT1    {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_0 }, {}}
#define DMA_DFSDM1_FLT2    {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_0 }, {}}
#define DMA_DFSDM1_FLT3    {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_0 }, {}}
#define DMA_HASH_IN        {{DMA2_Channel7_BASE, &sig_DMA2_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_6 }, {}}
#define DMA_I2C1_RX        {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_3 },\
                            {DMA2_Channel6_BASE, &sig_DMA2_CH6  , DMA_IRQ_SINGLE, 0             }, {}}
#define DMA_I2C1_TX        {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_3 },\
                            {DMA2_Channel7_BASE, &sig_DMA2_CH7  , DMA_IRQ_SINGLE, 0             }, {}}
#define DMA_I2C2_RX        {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_3 }, {}}
#define DMA_I2C2_TX        {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_3 }, {}}
#define DMA_I2C3_RX        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_3 }, {}}
#define DMA_I2C3_TX        {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_3 }, {}}
#define DMA_I2C4_RX        {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_0 }, {}}
#define DMA_I2C4_TX        {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_0 }, {}}
#define DMA_LPUART_RX      {{DMA2_Channel7_BASE, &sig_DMA2_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_4 }, {}}
#define DMA_LPUART1_RX    DMA_LPUART_RX
#define DMA_LPUART_TX      {{DMA2_Channel6_BASE, &sig_DMA2_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_4 }, {}}
#define DMA_LPUART1_TX    DMA_LPUART_TX
#define DMA_QUADSPI        {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_5 },\
                            {DMA2_Channel7_BASE, &sig_DMA2_CH7  , DMA_IRQ_SINGLE, 0             }, {}}
#define DMA_SAI1_A         {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_1 },\
                            {DMA2_Channel6_BASE, &sig_DMA2_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_1 }, {}}
#define DMA_SAI1_B         {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_1 },\
                            {DMA2_Channel7_BASE, &sig_DMA2_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_1 }, {}}
#define DMA_SAI2_A         {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_1 },\
                            {DMA2_Channel3_BASE, &sig_DMA2_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_1 }, {}}
#define DMA_SAI2_B         {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_1 },\
                            {DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_1 }, {}}
#define DMA_SDMMC1         {{DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, 0             },\
                            {DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_SDMMC1_RX      {{DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, 0             },\
                            {DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_SDMMC1_TX      {{DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, 0             },\
                            {DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_SPI1_RX        {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_1 },\
                            {DMA2_Channel3_BASE, &sig_DMA2_CH3  , DMA_IRQ_SINGLE, 0             }, {}}
#define DMA_SPI1_TX        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_1 },\
                            {DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, 0             }, {}}
#define DMA_SPI2_RX        {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_1 }, {}}
#define DMA_SPI2_TX        {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_1 }, {}}
#define DMA_SPI3_RX        {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_3 }, {}}
#define DMA_SPI3_TX        {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_3 }, {}}
#define DMA_SWPMI_RX       {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_4 }, {}}
#define DMA_SWPMI_TX       {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_4 }, {}}
#define DMA_TIM15_CH1      {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM15_COM      {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM15_TRIG     {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM15_UP       {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM16_CH1      {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_4 },\
                            {DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_4 }, {}}
#define DMA_TIM16_UP       {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_4 },\
                            {DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_4 }, {}}
#define DMA_TIM17_CH1      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_5 },\
                            {DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM17_UP       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_5 },\
                            {DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM1_CH1       {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM1_CH2       {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM1_CH3       {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM1_CH4       {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM1_COM       {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM1_TRIG      {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM1_UP        {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM2_CH1       {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_4 }, {}}
#define DMA_TIM2_CH2       {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_4 }, {}}
#define DMA_TIM2_CH3       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_4 }, {}}
#define DMA_TIM2_CH4       {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_4 }, {}}
#define DMA_TIM2_UP        {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_4 }, {}}
#define DMA_TIM3_CH1       {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM3_CH3       {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM3_CH4       {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM3_TRIG      {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM3_UP        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM4_CH1       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_6 }, {}}
#define DMA_TIM4_CH2       {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_6 }, {}}
#define DMA_TIM4_CH3       {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_6 }, {}}
#define DMA_TIM4_UP        {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_6 }, {}}
#define DMA_TIM5_CH1       {{DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM5_CH2       {{DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM5_CH3       {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM5_CH4       {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM5_COM       {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM5_TRIG      {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM5_UP        {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_5 }, {}}
#define DMA_TIM6_UP        {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, 0             },\
                            {DMA2_Channel4_BASE, &sig_DMA2_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_3 }, {}}
#define DMA_TIM7_UP        {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, 0             },\
                            {DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_3 }, {}}
#define DMA_TIM8_CH1       {{DMA2_Channel6_BASE, &sig_DMA2_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM8_CH2       {{DMA2_Channel7_BASE, &sig_DMA2_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM8_CH3       {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM8_CH4       {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM8_COM       {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM8_TRIG      {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_TIM8_UP        {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_7 }, {}}
#define DMA_UART4_RX       {{DMA2_Channel5_BASE, &sig_DMA2_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_2 }, {}}
#define DMA_UART4_TX       {{DMA2_Channel3_BASE, &sig_DMA2_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_2 }, {}}
#define DMA_UART5_RX       {{DMA2_Channel2_BASE, &sig_DMA2_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_2 }, {}}
#define DMA_UART5_TX       {{DMA2_Channel1_BASE, &sig_DMA2_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_2 }, {}}
#define DMA_USART1_RX      {{DMA1_Channel5_BASE, &sig_DMA1_CH5  , DMA_IRQ_SINGLE, DMA_REQUEST_2 },\
                            {DMA2_Channel7_BASE, &sig_DMA2_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_2 }, {}}
#define DMA_UART1_RX    DMA_USART1_RX
#define DMA_USART1_TX      {{DMA1_Channel4_BASE, &sig_DMA1_CH4  , DMA_IRQ_SINGLE, DMA_REQUEST_2 },\
                            {DMA2_Channel6_BASE, &sig_DMA2_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_2 }, {}}
#define DMA_UART1_TX    DMA_USART1_TX
#define DMA_USART2_RX      {{DMA1_Channel6_BASE, &sig_DMA1_CH6  , DMA_IRQ_SINGLE, DMA_REQUEST_2 }, {}}
#define DMA_UART2_RX    DMA_USART2_RX
#define DMA_USART2_TX      {{DMA1_Channel7_BASE, &sig_DMA1_CH7  , DMA_IRQ_SINGLE, DMA_REQUEST_2 }, {}}
#define DMA_UART2_TX    DMA_USART2_TX
#define DMA_USART3_RX      {{DMA1_Channel3_BASE, &sig_DMA1_CH3  , DMA_IRQ_SINGLE, DMA_REQUEST_2 }, {}}
#define DMA_UART3_RX    DMA_USART3_RX
#define DMA_USART3_TX      {{DMA1_Channel2_BASE, &sig_DMA1_CH2  , DMA_IRQ_SINGLE, DMA_REQUEST_2 }}
#define DMA_UART3_TX    DMA_USART3_TX

#endif
