/* Do not edit! Generated automatically from /devel/cubemx/db/mcu/IP/DMA-STM32L4S9_dma1_v1_2_Modes.xml.*/

#if !defined(_STM32L4S9_DMA_H_)
# define _STM32L4S9_DMA_H_

#define STM32_DMA_VERSION 12
#define  DMA_REQUEST_MEM2MEM          0
#define  DMA_REQUEST_GENERATOR0       1
#define  DMA_REQUEST_GENERATOR1       2
#define  DMA_REQUEST_GENERATOR2       3
#define  DMA_REQUEST_GENERATOR3       4
#define  DMA_REQUEST_ADC1             5
#define  DMA_REQUEST_DAC1_CH1         6
#define  DMA_REQUEST_DAC1_CH2         7
#define  DMA_REQUEST_TIM6_UP          8
#define  DMA_REQUEST_TIM7_UP          9
#define  DMA_REQUEST_SPI1_RX         10
#define  DMA_REQUEST_SPI1_TX         11
#define  DMA_REQUEST_SPI2_RX         12
#define  DMA_REQUEST_SPI2_TX         13
#define  DMA_REQUEST_SPI3_RX         14
#define  DMA_REQUEST_SPI3_TX         15
#define  DMA_REQUEST_I2C1_RX         16
#define  DMA_REQUEST_I2C1_TX         17
#define  DMA_REQUEST_I2C2_RX         18
#define  DMA_REQUEST_I2C2_TX         19
#define  DMA_REQUEST_I2C3_RX         20
#define  DMA_REQUEST_I2C3_TX         21
#define  DMA_REQUEST_I2C4_RX         22
#define  DMA_REQUEST_I2C4_TX         23
#define  DMA_REQUEST_USART1_RX       24
#define  DMA_REQUEST_USART1_TX       25
#define  DMA_REQUEST_USART2_RX       26
#define  DMA_REQUEST_USART2_TX       27
#define  DMA_REQUEST_USART3_RX       28
#define  DMA_REQUEST_USART3_TX       29
#define  DMA_REQUEST_UART4_RX        30
#define  DMA_REQUEST_UART4_TX        31
#define  DMA_REQUEST_UART5_RX        32
#define  DMA_REQUEST_UART5_TX        33
#define  DMA_REQUEST_LPUART1_RX      34
#define  DMA_REQUEST_LPUART1_TX      35
#define  DMA_REQUEST_SAI1_A          36
#define  DMA_REQUEST_SAI1_B          37
#define  DMA_REQUEST_SAI2_A          38
#define  DMA_REQUEST_SAI2_B          39
#define  DMA_REQUEST_OCTOSPI1        40
#define  DMA_REQUEST_OCTOSPI2        41
#define  DMA_REQUEST_TIM1_CH1        42
#define  DMA_REQUEST_TIM1_CH2        43
#define  DMA_REQUEST_TIM1_CH3        44
#define  DMA_REQUEST_TIM1_CH4        45
#define  DMA_REQUEST_TIM1_UP         46
#define  DMA_REQUEST_TIM1_TRIG       47
#define  DMA_REQUEST_TIM1_COM        48
#define  DMA_REQUEST_TIM8_CH1        49
#define  DMA_REQUEST_TIM8_CH2        50
#define  DMA_REQUEST_TIM8_CH3        51
#define  DMA_REQUEST_TIM8_CH4        52
#define  DMA_REQUEST_TIM8_UP         53
#define  DMA_REQUEST_TIM8_TRIG       54
#define  DMA_REQUEST_TIM8_COM        55
#define  DMA_REQUEST_TIM2_CH1        56
#define  DMA_REQUEST_TIM2_CH2        57
#define  DMA_REQUEST_TIM2_CH3        58
#define  DMA_REQUEST_TIM2_CH4        59
#define  DMA_REQUEST_TIM2_UP         60
#define  DMA_REQUEST_TIM3_CH1        61
#define  DMA_REQUEST_TIM3_CH2        62
#define  DMA_REQUEST_TIM3_CH3        63
#define  DMA_REQUEST_TIM3_CH4        64
#define  DMA_REQUEST_TIM3_UP         65
#define  DMA_REQUEST_TIM3_TRIG       66
#define  DMA_REQUEST_TIM4_CH1        67
#define  DMA_REQUEST_TIM4_CH2        68
#define  DMA_REQUEST_TIM4_CH3        69
#define  DMA_REQUEST_TIM4_CH4        70
#define  DMA_REQUEST_TIM4_UP         71
#define  DMA_REQUEST_TIM5_CH1        72
#define  DMA_REQUEST_TIM5_CH2        73
#define  DMA_REQUEST_TIM5_CH3        74
#define  DMA_REQUEST_TIM5_CH4        75
#define  DMA_REQUEST_TIM5_UP         76
#define  DMA_REQUEST_TIM5_TRIG       77
#define  DMA_REQUEST_TIM15_CH1       78
#define  DMA_REQUEST_TIM15_UP        79
#define  DMA_REQUEST_TIM15_TRIG      80
#define  DMA_REQUEST_TIM15_COM       81
#define  DMA_REQUEST_TIM16_CH1       82
#define  DMA_REQUEST_TIM16_UP        83
#define  DMA_REQUEST_TIM17_CH1       84
#define  DMA_REQUEST_TIM17_UP        85
#define  DMA_REQUEST_DFSDM1_FLT0     86
#define  DMA_REQUEST_DFSDM1_FLT1     87
#define  DMA_REQUEST_DFSDM1_FLT2     88
#define  DMA_REQUEST_DFSDM1_FLT3     89
#define  DMA_REQUEST_DCMI            90
#define  DMA_REQUEST_AES_IN          91
#define  DMA_REQUEST_AES_OUT         92
#define  DMA_REQUEST_HASH_IN         93

#define DMA_ADC1           {{DMA_ANY, DMA_REQUEST_ADC1}, {}}
#define DMA_AES_IN         {{DMA_ANY, DMA_REQUEST_AES_IN}, {}}
#define DMA_AES_OUT        {{DMA_ANY, DMA_REQUEST_AES_OUT}, {}}
#define DMA_DAC1_CH1       {{DMA_ANY, DMA_REQUEST_DAC1_CH1}, {}}
#define DMA_DAC1_CH2       {{DMA_ANY, DMA_REQUEST_DAC1_CH2}, {}}
#define DMA_DCMI           {{DMA_ANY, DMA_REQUEST_DCMI}, {}}
#define DMA_DFSDM1_FLT0    {{DMA_ANY, DMA_REQUEST_DFSDM1_FLT0}, {}}
#define DMA_DFSDM1_FLT1    {{DMA_ANY, DMA_REQUEST_DFSDM1_FLT1}, {}}
#define DMA_DFSDM1_FLT2    {{DMA_ANY, DMA_REQUEST_DFSDM1_FLT2}, {}}
#define DMA_DFSDM1_FLT3    {{DMA_ANY, DMA_REQUEST_DFSDM1_FLT3}, {}}
#define DMA_DMA_GENERATOR0 {{DMA_ANY, DMA_REQUEST_DMA_GENERATOR0}, {}}
#define DMA_DMA_GENERATOR1 {{DMA_ANY, DMA_REQUEST_DMA_GENERATOR1}, {}}
#define DMA_DMA_GENERATOR2 {{DMA_ANY, DMA_REQUEST_DMA_GENERATOR2}, {}}
#define DMA_DMA_GENERATOR3 {{DMA_ANY, DMA_REQUEST_DMA_GENERATOR3}, {}}
#define DMA_HASH_IN        {{DMA_ANY, DMA_REQUEST_HASH_IN}, {}}
#define DMA_I2C1_RX        {{DMA_ANY, DMA_REQUEST_I2C1_RX}, {}}
#define DMA_I2C1_TX        {{DMA_ANY, DMA_REQUEST_I2C1_TX}, {}}
#define DMA_I2C2_RX        {{DMA_ANY, DMA_REQUEST_I2C2_RX}, {}}
#define DMA_I2C2_TX        {{DMA_ANY, DMA_REQUEST_I2C2_TX}, {}}
#define DMA_I2C3_RX        {{DMA_ANY, DMA_REQUEST_I2C3_RX}, {}}
#define DMA_I2C3_TX        {{DMA_ANY, DMA_REQUEST_I2C3_TX}, {}}
#define DMA_I2C4_RX        {{DMA_ANY, DMA_REQUEST_I2C4_RX}, {}}
#define DMA_I2C4_TX        {{DMA_ANY, DMA_REQUEST_I2C4_TX}, {}}
#define DMA_LPUART1_RX     {{DMA_ANY, DMA_REQUEST_LPUART1_RX}, {}}
#define DMA_LPUART1_TX     {{DMA_ANY, DMA_REQUEST_LPUART1_TX}, {}}
#define DMA_OCTOSPI1       {{DMA_ANY, DMA_REQUEST_OCTOSPI1}, {}}
#define DMA_OCTOSPI2       {{DMA_ANY, DMA_REQUEST_OCTOSPI2}, {}}
#define DMA_SAI1_A         {{DMA_ANY, DMA_REQUEST_SAI1_A}, {}}
#define DMA_SAI1_B         {{DMA_ANY, DMA_REQUEST_SAI1_B}, {}}
#define DMA_SAI2_A         {{DMA_ANY, DMA_REQUEST_SAI2_A}, {}}
#define DMA_SAI2_B         {{DMA_ANY, DMA_REQUEST_SAI2_B}, {}}
#define DMA_SPI1_RX        {{DMA_ANY, DMA_REQUEST_SPI1_RX}, {}}
#define DMA_SPI1_TX        {{DMA_ANY, DMA_REQUEST_SPI1_TX}, {}}
#define DMA_SPI2_RX        {{DMA_ANY, DMA_REQUEST_SPI2_RX}, {}}
#define DMA_SPI2_TX        {{DMA_ANY, DMA_REQUEST_SPI2_TX}, {}}
#define DMA_SPI3_RX        {{DMA_ANY, DMA_REQUEST_SPI3_RX}, {}}
#define DMA_SPI3_TX        {{DMA_ANY, DMA_REQUEST_SPI3_TX}, {}}
#define DMA_TIM15_CH1      {{DMA_ANY, DMA_REQUEST_TIM15_CH1}, {}}
#define DMA_TIM15_COM      {{DMA_ANY, DMA_REQUEST_TIM15_COM}, {}}
#define DMA_TIM15_TRIG     {{DMA_ANY, DMA_REQUEST_TIM15_TRIG}, {}}
#define DMA_TIM15_UP       {{DMA_ANY, DMA_REQUEST_TIM15_UP}, {}}
#define DMA_TIM16_CH1      {{DMA_ANY, DMA_REQUEST_TIM16_CH1}, {}}
#define DMA_TIM16_UP       {{DMA_ANY, DMA_REQUEST_TIM16_UP}, {}}
#define DMA_TIM17_CH1      {{DMA_ANY, DMA_REQUEST_TIM17_CH1}, {}}
#define DMA_TIM17_UP       {{DMA_ANY, DMA_REQUEST_TIM17_UP}, {}}
#define DMA_TIM1_CH1       {{DMA_ANY, DMA_REQUEST_TIM1_CH1}, {}}
#define DMA_TIM1_CH2       {{DMA_ANY, DMA_REQUEST_TIM1_CH2}, {}}
#define DMA_TIM1_CH3       {{DMA_ANY, DMA_REQUEST_TIM1_CH3}, {}}
#define DMA_TIM1_CH4       {{DMA_ANY, DMA_REQUEST_TIM1_CH4}, {}}
#define DMA_TIM1_COM       {{DMA_ANY, DMA_REQUEST_TIM1_COM}, {}}
#define DMA_TIM1_TRIG      {{DMA_ANY, DMA_REQUEST_TIM1_TRIG}, {}}
#define DMA_TIM1_UP        {{DMA_ANY, DMA_REQUEST_TIM1_UP}, {}}
#define DMA_TIM2_CH1       {{DMA_ANY, DMA_REQUEST_TIM2_CH1}, {}}
#define DMA_TIM2_CH2       {{DMA_ANY, DMA_REQUEST_TIM2_CH2}, {}}
#define DMA_TIM2_CH3       {{DMA_ANY, DMA_REQUEST_TIM2_CH3}, {}}
#define DMA_TIM2_CH4       {{DMA_ANY, DMA_REQUEST_TIM2_CH4}, {}}
#define DMA_TIM2_UP        {{DMA_ANY, DMA_REQUEST_TIM2_UP}, {}}
#define DMA_TIM3_CH1       {{DMA_ANY, DMA_REQUEST_TIM3_CH1}, {}}
#define DMA_TIM3_CH2       {{DMA_ANY, DMA_REQUEST_TIM3_CH2}, {}}
#define DMA_TIM3_CH3       {{DMA_ANY, DMA_REQUEST_TIM3_CH3}, {}}
#define DMA_TIM3_CH4       {{DMA_ANY, DMA_REQUEST_TIM3_CH4}, {}}
#define DMA_TIM3_TRIG      {{DMA_ANY, DMA_REQUEST_TIM3_TRIG}, {}}
#define DMA_TIM3_UP        {{DMA_ANY, DMA_REQUEST_TIM3_UP}, {}}
#define DMA_TIM4_CH1       {{DMA_ANY, DMA_REQUEST_TIM4_CH1}, {}}
#define DMA_TIM4_CH2       {{DMA_ANY, DMA_REQUEST_TIM4_CH2}, {}}
#define DMA_TIM4_CH3       {{DMA_ANY, DMA_REQUEST_TIM4_CH3}, {}}
#define DMA_TIM4_CH4       {{DMA_ANY, DMA_REQUEST_TIM4_CH4}, {}}
#define DMA_TIM4_UP        {{DMA_ANY, DMA_REQUEST_TIM4_UP}, {}}
#define DMA_TIM5_CH1       {{DMA_ANY, DMA_REQUEST_TIM5_CH1}, {}}
#define DMA_TIM5_CH2       {{DMA_ANY, DMA_REQUEST_TIM5_CH2}, {}}
#define DMA_TIM5_CH3       {{DMA_ANY, DMA_REQUEST_TIM5_CH3}, {}}
#define DMA_TIM5_CH4       {{DMA_ANY, DMA_REQUEST_TIM5_CH4}, {}}
#define DMA_TIM5_TRIG      {{DMA_ANY, DMA_REQUEST_TIM5_TRIG}, {}}
#define DMA_TIM5_UP        {{DMA_ANY, DMA_REQUEST_TIM5_UP}, {}}
#define DMA_TIM6_UP        {{DMA_ANY, DMA_REQUEST_TIM6_UP}, {}}
#define DMA_TIM7_UP        {{DMA_ANY, DMA_REQUEST_TIM7_UP}, {}}
#define DMA_TIM8_CH1       {{DMA_ANY, DMA_REQUEST_TIM8_CH1}, {}}
#define DMA_TIM8_CH2       {{DMA_ANY, DMA_REQUEST_TIM8_CH2}, {}}
#define DMA_TIM8_CH3       {{DMA_ANY, DMA_REQUEST_TIM8_CH3}, {}}
#define DMA_TIM8_CH4       {{DMA_ANY, DMA_REQUEST_TIM8_CH4}, {}}
#define DMA_TIM8_COM       {{DMA_ANY, DMA_REQUEST_TIM8_COM}, {}}
#define DMA_TIM8_TRIG      {{DMA_ANY, DMA_REQUEST_TIM8_TRIG}, {}}
#define DMA_TIM8_UP        {{DMA_ANY, DMA_REQUEST_TIM8_UP}, {}}
#define DMA_UART4_RX       {{DMA_ANY, DMA_REQUEST_UART4_RX}, {}}
#define DMA_UART4_TX       {{DMA_ANY, DMA_REQUEST_UART4_TX}, {}}
#define DMA_UART5_RX       {{DMA_ANY, DMA_REQUEST_UART5_RX}, {}}
#define DMA_UART5_TX       {{DMA_ANY, DMA_REQUEST_UART5_TX}, {}}
#define DMA_USART1_RX      {{DMA_ANY, DMA_REQUEST_USART1_RX}, {}}
#define DMA_UART1_RX    DMA_USART1_RX
#define DMA_USART1_TX      {{DMA_ANY, DMA_REQUEST_USART1_TX}, {}}
#define DMA_UART1_TX    DMA_USART1_TX
#define DMA_USART2_RX      {{DMA_ANY, DMA_REQUEST_USART2_RX}, {}}
#define DMA_UART2_RX    DMA_USART2_RX
#define DMA_USART2_TX      {{DMA_ANY, DMA_REQUEST_USART2_TX}, {}}
#define DMA_UART2_TX    DMA_USART2_TX
#define DMA_USART3_RX      {{DMA_ANY, DMA_REQUEST_USART3_RX}, {}}
#define DMA_UART3_RX    DMA_USART3_RX
#define DMA_USART3_TX      {{DMA_ANY, DMA_REQUEST_USART3_TX}}
#define DMA_UART3_TX    DMA_USART3_TX

#endif
