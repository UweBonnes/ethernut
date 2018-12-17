/* Do not edit! Generated automatically from /devel/cubemx/db/mcu/IP/DMA-STM32L083_dma_v1_1_Modes.xml.*/

#if !defined(_STM32L083_DMA_H_)
# define _STM32L083_DMA_H_

#define STM32_DMA_VERSION 11

#define DMA_ADC            {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_0 },\
                            {DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_0 }, {}}
#define DMA_AES_IN         {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_11},\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_11}, {}}
#define DMA_AES_OUT        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_11},\
                            {DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_11}, {}}
#define DMA_DAC_CH1        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_9 }, {}}
#define DMA_DAC_CH2        {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_15}, {}}
#define DMA_I2C1_RX        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_6 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_6 }, {}}
#define DMA_I2C1_TX        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_6 },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_6 }, {}}
#define DMA_I2C2_RX        {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_7 }, {}}
#define DMA_I2C2_TX        {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_7 }, {}}
#define DMA_I2C3_RX        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_14},\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_14}, {}}
#define DMA_I2C3_TX        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_14},\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_14}, {}}
#define DMA_LPUART1_RX     {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_5 },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_5 }, {}}
#define DMA_LPUART1_TX     {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_5 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_5 }, {}}
#define DMA_SPI1_RX        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_1 }, {}}
#define DMA_SPI1_TX        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_1 }, {}}
#define DMA_SPI2_RX        {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_2 },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_2 }, {}}
#define DMA_SPI2_TX        {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_2 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_2 }, {}}
#define DMA_TIM2_CH1       {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_8 }, {}}
#define DMA_TIM2_CH2       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_8 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_8 }, {}}
#define DMA_TIM2_CH3       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_REQUEST_8 }, {}}
#define DMA_TIM2_CH4       {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_8 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_8 }, {}}
#define DMA_TIM2_UP        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_8 }, {}}
#define DMA_TIM3_CH1       {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_10}, {}}
#define DMA_TIM3_CH3       {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_10}, {}}
#define DMA_TIM3_CH4       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_10}, {}}
#define DMA_TIM3_TRIG      {{DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_10}, {}}
#define DMA_TIM3_UP        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_10}, {}}
#define DMA_TIM6_UP        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_9 }, {}}
#define DMA_TIM7_UP        {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_15}, {}}
#define DMA_USART1_RX      {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_3 },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_3 }, {}}
#define DMA_UART1_RX    DMA_USART1_RX
#define DMA_USART1_TX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_3 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_3 }, {}}
#define DMA_UART1_TX    DMA_USART1_TX
#define DMA_USART2_RX      {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_4 },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_4 }, {}}
#define DMA_UART2_RX    DMA_USART2_RX
#define DMA_USART2_TX      {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_4 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_4 }, {}}
#define DMA_UART2_TX    DMA_USART2_TX
#define DMA_USART4_RX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_12},\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_12}, {}}
#define DMA_UART4_RX    DMA_USART4_RX
#define DMA_USART4_TX      {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_12},\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_12}, {}}
#define DMA_UART4_TX    DMA_USART4_TX
#define DMA_USART5_RX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_13},\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_13}, {}}
#define DMA_UART5_RX    DMA_USART5_RX
#define DMA_USART5_TX      {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_REQUEST_13},\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_REQUEST_13}}
#define DMA_UART5_TX    DMA_USART5_TX
#define STM32_DMA_COMBINED_IRQ

#endif
