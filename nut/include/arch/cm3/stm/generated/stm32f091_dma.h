/* Do not edit! Generated automatically from /devel/cubemx/db/mcu/IP/DMA-STM32F091_dma_v1_1_Modes.xml.*/

#if !defined(_STM32F091_DMA_H_)
# define _STM32F091_DMA_H_

#define STM32_DMA_VERSION 11

#define DMA_ADC            {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request1  },\
                            {DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request1  },\
                            {DMA2_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request1  }, {}}
#define DMA_DAC_CH1        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request1  },\
                            {DMA2_Channel3_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request1  }, {}}
#define DMA_DAC_CH2        {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request1  },\
                            {DMA2_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request1  }, {}}
#define DMA_I2C1_RX        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request2  },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request2  }, {}}
#define DMA_I2C1_TX        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request2  },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request2  }, {}}
#define DMA_I2C2_RX        {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request2  },\
                            {DMA2_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request2  }, {}}
#define DMA_I2C2_TX        {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request2  },\
                            {DMA2_Channel1_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request2  }, {}}
#define DMA_SPI1_RX        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request3  },\
                            {DMA2_Channel3_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request3  }, {}}
#define DMA_SPI1_TX        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request3  },\
                            {DMA2_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request3  }, {}}
#define DMA_SPI2_RX        {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request3  },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request3  }, {}}
#define DMA_SPI2_TX        {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request3  },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request3  }, {}}
#define DMA_TIM15_CH1      {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM15_COM      {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM15_TRIG     {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM15_UP       {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM16_CH1      {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request7  },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request7  },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request7  }, {}}
#define DMA_TIM16_UP       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request7  },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request7  },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request7  }, {}}
#define DMA_TIM17_CH1      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request7  },\
                            {DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request7  },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request7  }, {}}
#define DMA_TIM17_UP       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request7  },\
                            {DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request7  },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request7  }, {}}
#define DMA_TIM1_CH1       {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request4  },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request4  }, {}}
#define DMA_TIM1_CH2       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request4  },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request4  }, {}}
#define DMA_TIM1_CH3       {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request0  },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request4  },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request4  }, {}}
#define DMA_TIM1_CH4       {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM1_COM       {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM1_TRIG      {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM1_UP        {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM2_CH1       {{DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM2_CH2       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request5  },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request5  }, {}}
#define DMA_TIM2_CH3       {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request0  }, {}}
#define DMA_TIM2_CH4       {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request5  },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request5  }, {}}
#define DMA_TIM2_UP        {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM3_CH1       {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request6  },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request6  }, {}}
#define DMA_TIM3_CH3       {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM3_CH4       {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM3_TRIG      {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request6  },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request6  }, {}}
#define DMA_TIM3_UP        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request0  }, {}}
#define DMA_TIM6_UP        {{DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request1  },\
                            {DMA2_Channel3_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request1  }, {}}
#define DMA_TIM7_UP        {{DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request1  },\
                            {DMA2_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request1  }, {}}
#define DMA_USART1_RX      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request8  },\
                            {DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request8  },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request8  },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request8  },\
                            {DMA2_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request8  },\
                            {DMA2_Channel3_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request8  }, {}}
#define DMA_UART1_RX    DMA_USART1_RX
#define DMA_USART1_TX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request8  },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request8  },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request8  },\
                            {DMA2_Channel1_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request8  },\
                            {DMA2_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request8  },\
                            {DMA2_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request8  }, {}}
#define DMA_UART1_TX    DMA_USART1_TX
#define DMA_USART2_RX      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request9  },\
                            {DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request9  },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request9  },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request9  },\
                            {DMA2_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request9  },\
                            {DMA2_Channel3_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request9  }, {}}
#define DMA_UART2_RX    DMA_USART2_RX
#define DMA_USART2_TX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request9  },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request9  },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request9  },\
                            {DMA2_Channel1_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request9  },\
                            {DMA2_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request9  },\
                            {DMA2_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request9  }, {}}
#define DMA_UART2_TX    DMA_USART2_TX
#define DMA_USART3_RX      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request10 },\
                            {DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request10 },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request10 },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request10 },\
                            {DMA2_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request10 },\
                            {DMA2_Channel3_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request10 }, {}}
#define DMA_UART3_RX    DMA_USART3_RX
#define DMA_USART3_TX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request10 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request10 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request10 },\
                            {DMA2_Channel1_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request10 },\
                            {DMA2_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request10 },\
                            {DMA2_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request10 }, {}}
#define DMA_UART3_TX    DMA_USART3_TX
#define DMA_USART4_RX      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request11 },\
                            {DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request11 },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request11 },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request11 },\
                            {DMA2_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request11 },\
                            {DMA2_Channel3_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request11 }, {}}
#define DMA_UART4_RX    DMA_USART4_RX
#define DMA_USART4_TX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request11 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request11 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request11 },\
                            {DMA2_Channel1_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request11 },\
                            {DMA2_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request11 },\
                            {DMA2_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request11 }, {}}
#define DMA_UART4_TX    DMA_USART4_TX
#define DMA_USART5_RX      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request12 },\
                            {DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request12 },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request12 },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request12 },\
                            {DMA2_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request12 },\
                            {DMA2_Channel3_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request12 }, {}}
#define DMA_UART5_RX    DMA_USART5_RX
#define DMA_USART5_TX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request12 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request12 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request12 },\
                            {DMA2_Channel1_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request12 },\
                            {DMA2_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request12 },\
                            {DMA2_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request12 }, {}}
#define DMA_UART5_TX    DMA_USART5_TX
#define DMA_USART6_RX      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request13 },\
                            {DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request13 },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request13 },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request13 },\
                            {DMA2_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request13 },\
                            {DMA2_Channel3_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request13 }, {}}
#define DMA_UART6_RX    DMA_USART6_RX
#define DMA_USART6_TX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request13 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request13 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request13 },\
                            {DMA2_Channel1_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request13 },\
                            {DMA2_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request13 },\
                            {DMA2_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request13 }, {}}
#define DMA_UART6_TX    DMA_USART6_TX
#define DMA_USART7_RX      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request14 },\
                            {DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request14 },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request14 },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request14 },\
                            {DMA2_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request14 },\
                            {DMA2_Channel3_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request14 }, {}}
#define DMA_UART7_RX    DMA_USART7_RX
#define DMA_USART7_TX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request14 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request14 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request14 },\
                            {DMA2_Channel1_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request14 },\
                            {DMA2_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request14 },\
                            {DMA2_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request14 }, {}}
#define DMA_UART7_TX    DMA_USART7_TX
#define DMA_USART8_RX      {{DMA1_Channel1_BASE, &sig_DMA1_CH1  , DMA_IRQ_SINGLE, DMA_Request15 },\
                            {DMA1_Channel3_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request15 },\
                            {DMA1_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request15 },\
                            {DMA1_Channel6_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request15 },\
                            {DMA2_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request15 },\
                            {DMA2_Channel3_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request15 }, {}}
#define DMA_UART8_RX    DMA_USART8_RX
#define DMA_USART8_TX      {{DMA1_Channel2_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request15 },\
                            {DMA1_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request15 },\
                            {DMA1_Channel7_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request15 },\
                            {DMA2_Channel1_BASE, &sig_DMA_GROUP1, DMA_IRQ_GROUP , DMA_Request15 },\
                            {DMA2_Channel4_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request15 },\
                            {DMA2_Channel5_BASE, &sig_DMA_GROUP2, DMA_IRQ_GROUP , DMA_Request15 }}
#define DMA_UART8_TX    DMA_USART8_TX
#define STM32_DMA_COMBINED_IRQ

#endif
