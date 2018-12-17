/* Do not edit! Generated automatically from /devel/cubemx/db/mcu/IP/DMA-STM32L021_dma_v1_1_Modes.xml.*/

#if !defined(_STM32L021_DMA_H_)
# define _STM32L021_DMA_H_

#define STM32_DMA_VERSION 11

#define DMA_ADC            {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_0 },\
                            {DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_0 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_0 }, {}}
#define DMA_AES_IN         {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_11},\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_11}, {}}
#define DMA_AES_OUT        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_11},\
                            {DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_11}, {}}
#define DMA_I2C1_RX        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_6 },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_6 }, {}}
#define DMA_I2C1_TX        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_6 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_6 }, {}}
#define DMA_LPUART1_RX     {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_5 },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_5 }, {}}
#define DMA_LPUART1_TX     {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_5 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_5 }, {}}
#define DMA_SPI1_RX        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_1 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_1 }, {}}
#define DMA_SPI1_TX        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_1 },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_1 }, {}}
#define DMA_TIM2_CH1       {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_8 }, {}}
#define DMA_TIM2_CH2       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_8 }, {}}
#define DMA_TIM2_CH3       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_8 }, {}}
#define DMA_TIM2_CH4       {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_8 }, {}}
#define DMA_TIM2_UP        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_8 }, {}}
#define DMA_USART2_RX      {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_4 },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_4 }, {}}
#define DMA_UART2_RX    DMA_USART2_RX
#define DMA_USART2_TX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_4 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_4 }}
#define DMA_UART2_TX    DMA_USART2_TX
#define STM32_DMA_COMBINED_IRQ

#endif
