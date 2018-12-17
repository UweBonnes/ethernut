/* Do not edit! Generated automatically from /devel/cubemx/db/mcu/IP/(B)DMA_STM32H7**/

#if !defined(_STM32H753_DMA_H_)
# define _STM32H753_DMA_H_

#define STM32_DMA_VERSION 13
#define BDMA_REQUEST_MEM2MEM        0
#define BDMA_REQUEST_GENERATOR0     1
#define BDMA_REQUEST_GENERATOR1     2
#define BDMA_REQUEST_GENERATOR2     3
#define BDMA_REQUEST_GENERATOR3     4
#define BDMA_REQUEST_GENERATOR4     5
#define BDMA_REQUEST_GENERATOR5     6
#define BDMA_REQUEST_GENERATOR6     7
#define BDMA_REQUEST_GENERATOR7     8
#define DMA_REQUEST_LPUART1_RX      9
#define DMA_REQUEST_LPUART1_TX     10
#define DMA_REQUEST_SPI6_RX        11
#define DMA_REQUEST_SPI6_TX        12
#define DMA_REQUEST_I2C4_RX        13
#define DMA_REQUEST_I2C4_TX        14
#define DMA_REQUEST_SAI4_A         15
#define DMA_REQUEST_SAI4_B         16
#define BDMA_REQUEST_ADC3          17
#define DMA_REQUEST_MEM2MEM         0
#define DMA_REQUEST_GENERATOR0      1
#define DMA_REQUEST_GENERATOR1      2
#define DMA_REQUEST_GENERATOR2      3
#define DMA_REQUEST_GENERATOR3      4
#define DMA_REQUEST_GENERATOR4      5
#define DMA_REQUEST_GENERATOR5      6
#define DMA_REQUEST_GENERATOR6      7
#define DMA_REQUEST_GENERATOR7      8
#define DMA_REQUEST_ADC1            9
#define DMA_REQUEST_ADC2           10
#define DMA_REQUEST_TIM1_CH1       11
#define DMA_REQUEST_TIM1_CH2       12
#define DMA_REQUEST_TIM1_CH3       13
#define DMA_REQUEST_TIM1_CH4       14
#define DMA_REQUEST_TIM1_UP        15
#define DMA_REQUEST_TIM1_TRIG      16
#define DMA_REQUEST_TIM1_COM       17
#define DMA_REQUEST_TIM2_CH1       18
#define DMA_REQUEST_TIM2_CH2       19
#define DMA_REQUEST_TIM2_CH3       20
#define DMA_REQUEST_TIM2_CH4       21
#define DMA_REQUEST_TIM2_UP        22
#define DMA_REQUEST_TIM3_CH1       23
#define DMA_REQUEST_TIM3_CH2       24
#define DMA_REQUEST_TIM3_CH3       25
#define DMA_REQUEST_TIM3_CH4       26
#define DMA_REQUEST_TIM3_UP        27
#define DMA_REQUEST_TIM3_TRIG      28
#define DMA_REQUEST_TIM4_CH1       29
#define DMA_REQUEST_TIM4_CH2       30
#define DMA_REQUEST_TIM4_CH3       31
#define DMA_REQUEST_TIM4_UP        32
#define DMA_REQUEST_I2C1_RX        33
#define DMA_REQUEST_I2C1_TX        34
#define DMA_REQUEST_I2C2_RX        35
#define DMA_REQUEST_I2C2_TX        36
#define DMA_REQUEST_SPI1_RX        37
#define DMA_REQUEST_SPI1_TX        38
#define DMA_REQUEST_SPI2_RX        39
#define DMA_REQUEST_SPI2_TX        40
#define DMA_REQUEST_USART1_RX      41
#define DMA_REQUEST_USART1_TX      42
#define DMA_REQUEST_USART2_RX      43
#define DMA_REQUEST_USART2_TX      44
#define DMA_REQUEST_USART3_RX      45
#define DMA_REQUEST_USART3_TX      46
#define DMA_REQUEST_TIM8_CH1       47
#define DMA_REQUEST_TIM8_CH2       48
#define DMA_REQUEST_TIM8_CH3       49
#define DMA_REQUEST_TIM8_CH4       50
#define DMA_REQUEST_TIM8_UP        51
#define DMA_REQUEST_TIM8_TRIG      52
#define DMA_REQUEST_TIM8_COM       53
#define DMA_REQUEST_TIM5_CH1       54
#define DMA_REQUEST_TIM5_CH2       55
#define DMA_REQUEST_TIM5_CH3       56
#define DMA_REQUEST_TIM5_CH4       57
#define DMA_REQUEST_TIM5_UP        58
#define DMA_REQUEST_TIM5_TRIG      59
#define DMA_REQUEST_SPI3_RX        60
#define DMA_REQUEST_SPI3_TX        61
#define DMA_REQUEST_UART4_RX       62
#define DMA_REQUEST_UART4_TX       63
#define DMA_REQUEST_UART5_RX       64
#define DMA_REQUEST_UART5_TX       65
#define DMA_REQUEST_DAC1           66
#define DMA_REQUEST_DAC2           67
#define DMA_REQUEST_TIM6_UP        68
#define DMA_REQUEST_TIM7_UP        69
#define DMA_REQUEST_USART6_RX      70
#define DMA_REQUEST_USART6_TX      71
#define DMA_REQUEST_I2C3_RX        72
#define DMA_REQUEST_I2C3_TX        73
#define DMA_REQUEST_DCMI           74
#define DMA_REQUEST_CRYP_IN        75
#define DMA_REQUEST_CRYP_OUT       76
#define DMA_REQUEST_HASH_IN        77
#define DMA_REQUEST_UART7_RX       78
#define DMA_REQUEST_UART7_TX       79
#define DMA_REQUEST_UART8_RX       80
#define DMA_REQUEST_UART8_TX       81
#define DMA_REQUEST_SPI4_RX        82
#define DMA_REQUEST_SPI4_TX        83
#define DMA_REQUEST_SPI5_RX        84
#define DMA_REQUEST_SPI5_TX        85
#define DMA_REQUEST_SAI1_A         86
#define DMA_REQUEST_SAI1_B         87
#define DMA_REQUEST_SAI2_A         88
#define DMA_REQUEST_SAI2_B         89
#define DMA_REQUEST_SWPMI_RX       90
#define DMA_REQUEST_SWPMI_TX       91
#define DMA_REQUEST_SPDIF_RX_DT    92
#define DMA_REQUEST_SPDIF_RX_CS    93
#define DMA_REQUEST_HRTIM_MASTER   94
#define DMA_REQUEST_HRTIM_TIMER_A  95
#define DMA_REQUEST_HRTIM_TIMER_B  96
#define DMA_REQUEST_HRTIM_TIMER_C  97
#define DMA_REQUEST_HRTIM_TIMER_D  98
#define DMA_REQUEST_HRTIM_TIMER_E  99
#define DMA_REQUEST_DFSDM1_FLT0   100
#define DMA_REQUEST_DFSDM1_FLT1   101
#define DMA_REQUEST_DFSDM1_FLT2   102
#define DMA_REQUEST_DFSDM1_FLT3   103
#define DMA_REQUEST_TIM15_CH1     104
#define DMA_REQUEST_TIM15_UP      105
#define DMA_REQUEST_TIM15_TRIG    106
#define DMA_REQUEST_TIM15_COM     107
#define DMA_REQUEST_TIM16_CH1     108
#define DMA_REQUEST_TIM16_UP      109
#define DMA_REQUEST_TIM17_CH1     110
#define DMA_REQUEST_TIM17_UP      111
#define DMA_REQUEST_SAI3_A        112
#define DMA_REQUEST_SAI3_B        113
#define DMA_REQUEST_ADC3          114

#define BDMA_MEM2MEM           {{BDMA_ANY, BDMA_REQUEST_MEM2MEM     },{}}
#define BDMA_GENERATOR0        {{BDMA_ANY, BDMA_REQUEST_GENERATOR0  },{}}
#define BDMA_GENERATOR1        {{BDMA_ANY, BDMA_REQUEST_GENERATOR1  },{}}
#define BDMA_GENERATOR2        {{BDMA_ANY, BDMA_REQUEST_GENERATOR2  },{}}
#define BDMA_GENERATOR3        {{BDMA_ANY, BDMA_REQUEST_GENERATOR3  },{}}
#define BDMA_GENERATOR4        {{BDMA_ANY, BDMA_REQUEST_GENERATOR4  },{}}
#define BDMA_GENERATOR5        {{BDMA_ANY, BDMA_REQUEST_GENERATOR5  },{}}
#define BDMA_GENERATOR6        {{BDMA_ANY, BDMA_REQUEST_GENERATOR6  },{}}
#define BDMA_GENERATOR7        {{BDMA_ANY, BDMA_REQUEST_GENERATOR7  },{}}
#define DMA_LPUART1_RX         {{ DMA_ANY, DMA_REQUEST_LPUART1_RX   },{}}
#define DMA_LPUART1_TX         {{ DMA_ANY, DMA_REQUEST_LPUART1_TX   },{}}
#define DMA_SPI6_RX            {{ DMA_ANY, DMA_REQUEST_SPI6_RX      },{}}
#define DMA_SPI6_TX            {{ DMA_ANY, DMA_REQUEST_SPI6_TX      },{}}
#define DMA_I2C4_RX            {{ DMA_ANY, DMA_REQUEST_I2C4_RX      },{}}
#define DMA_I2C4_TX            {{ DMA_ANY, DMA_REQUEST_I2C4_TX      },{}}
#define DMA_SAI4_A             {{ DMA_ANY, DMA_REQUEST_SAI4_A       },{}}
#define DMA_SAI4_B             {{ DMA_ANY, DMA_REQUEST_SAI4_B       },{}}
#define BDMA_ADC3              {{BDMA_ANY, BDMA_REQUEST_ADC3        },{}}
#define DMA_MEM2MEM            {{ DMA_ANY, DMA_REQUEST_MEM2MEM      },{}}
#define DMA_GENERATOR0         {{ DMA_ANY, DMA_REQUEST_GENERATOR0   },{}}
#define DMA_GENERATOR1         {{ DMA_ANY, DMA_REQUEST_GENERATOR1   },{}}
#define DMA_GENERATOR2         {{ DMA_ANY, DMA_REQUEST_GENERATOR2   },{}}
#define DMA_GENERATOR3         {{ DMA_ANY, DMA_REQUEST_GENERATOR3   },{}}
#define DMA_GENERATOR4         {{ DMA_ANY, DMA_REQUEST_GENERATOR4   },{}}
#define DMA_GENERATOR5         {{ DMA_ANY, DMA_REQUEST_GENERATOR5   },{}}
#define DMA_GENERATOR6         {{ DMA_ANY, DMA_REQUEST_GENERATOR6   },{}}
#define DMA_GENERATOR7         {{ DMA_ANY, DMA_REQUEST_GENERATOR7   },{}}
#define DMA_ADC1               {{ DMA_ANY, DMA_REQUEST_ADC1         },{}}
#define DMA_ADC2               {{ DMA_ANY, DMA_REQUEST_ADC2         },{}}
#define DMA_TIM1_CH1           {{ DMA_ANY, DMA_REQUEST_TIM1_CH1     },{}}
#define DMA_TIM1_CH2           {{ DMA_ANY, DMA_REQUEST_TIM1_CH2     },{}}
#define DMA_TIM1_CH3           {{ DMA_ANY, DMA_REQUEST_TIM1_CH3     },{}}
#define DMA_TIM1_CH4           {{ DMA_ANY, DMA_REQUEST_TIM1_CH4     },{}}
#define DMA_TIM1_UP            {{ DMA_ANY, DMA_REQUEST_TIM1_UP      },{}}
#define DMA_TIM1_TRIG          {{ DMA_ANY, DMA_REQUEST_TIM1_TRIG    },{}}
#define DMA_TIM1_COM           {{ DMA_ANY, DMA_REQUEST_TIM1_COM     },{}}
#define DMA_TIM2_CH1           {{ DMA_ANY, DMA_REQUEST_TIM2_CH1     },{}}
#define DMA_TIM2_CH2           {{ DMA_ANY, DMA_REQUEST_TIM2_CH2     },{}}
#define DMA_TIM2_CH3           {{ DMA_ANY, DMA_REQUEST_TIM2_CH3     },{}}
#define DMA_TIM2_CH4           {{ DMA_ANY, DMA_REQUEST_TIM2_CH4     },{}}
#define DMA_TIM2_UP            {{ DMA_ANY, DMA_REQUEST_TIM2_UP      },{}}
#define DMA_TIM3_CH1           {{ DMA_ANY, DMA_REQUEST_TIM3_CH1     },{}}
#define DMA_TIM3_CH2           {{ DMA_ANY, DMA_REQUEST_TIM3_CH2     },{}}
#define DMA_TIM3_CH3           {{ DMA_ANY, DMA_REQUEST_TIM3_CH3     },{}}
#define DMA_TIM3_CH4           {{ DMA_ANY, DMA_REQUEST_TIM3_CH4     },{}}
#define DMA_TIM3_UP            {{ DMA_ANY, DMA_REQUEST_TIM3_UP      },{}}
#define DMA_TIM3_TRIG          {{ DMA_ANY, DMA_REQUEST_TIM3_TRIG    },{}}
#define DMA_TIM4_CH1           {{ DMA_ANY, DMA_REQUEST_TIM4_CH1     },{}}
#define DMA_TIM4_CH2           {{ DMA_ANY, DMA_REQUEST_TIM4_CH2     },{}}
#define DMA_TIM4_CH3           {{ DMA_ANY, DMA_REQUEST_TIM4_CH3     },{}}
#define DMA_TIM4_UP            {{ DMA_ANY, DMA_REQUEST_TIM4_UP      },{}}
#define DMA_I2C1_RX            {{ DMA_ANY, DMA_REQUEST_I2C1_RX      },{}}
#define DMA_I2C1_TX            {{ DMA_ANY, DMA_REQUEST_I2C1_TX      },{}}
#define DMA_I2C2_RX            {{ DMA_ANY, DMA_REQUEST_I2C2_RX      },{}}
#define DMA_I2C2_TX            {{ DMA_ANY, DMA_REQUEST_I2C2_TX      },{}}
#define DMA_SPI1_RX            {{ DMA_ANY, DMA_REQUEST_SPI1_RX      },{}}
#define DMA_SPI1_TX            {{ DMA_ANY, DMA_REQUEST_SPI1_TX      },{}}
#define DMA_SPI2_RX            {{ DMA_ANY, DMA_REQUEST_SPI2_RX      },{}}
#define DMA_SPI2_TX            {{ DMA_ANY, DMA_REQUEST_SPI2_TX      },{}}
#define DMA_USART1_RX          {{ DMA_ANY, DMA_REQUEST_USART1_RX    },{}}
#define DMA_UART1_RX DMA_USART1_RX
#define DMA_USART1_TX          {{ DMA_ANY, DMA_REQUEST_USART1_TX    },{}}
#define DMA_UART1_TX DMA_USART1_TX
#define DMA_USART2_RX          {{ DMA_ANY, DMA_REQUEST_USART2_RX    },{}}
#define DMA_UART2_RX DMA_USART2_RX
#define DMA_USART2_TX          {{ DMA_ANY, DMA_REQUEST_USART2_TX    },{}}
#define DMA_UART2_TX DMA_USART2_TX
#define DMA_USART3_RX          {{ DMA_ANY, DMA_REQUEST_USART3_RX    },{}}
#define DMA_UART3_RX DMA_USART3_RX
#define DMA_USART3_TX          {{ DMA_ANY, DMA_REQUEST_USART3_TX    },{}}
#define DMA_UART3_TX DMA_USART3_TX
#define DMA_TIM8_CH1           {{ DMA_ANY, DMA_REQUEST_TIM8_CH1     },{}}
#define DMA_TIM8_CH2           {{ DMA_ANY, DMA_REQUEST_TIM8_CH2     },{}}
#define DMA_TIM8_CH3           {{ DMA_ANY, DMA_REQUEST_TIM8_CH3     },{}}
#define DMA_TIM8_CH4           {{ DMA_ANY, DMA_REQUEST_TIM8_CH4     },{}}
#define DMA_TIM8_UP            {{ DMA_ANY, DMA_REQUEST_TIM8_UP      },{}}
#define DMA_TIM8_TRIG          {{ DMA_ANY, DMA_REQUEST_TIM8_TRIG    },{}}
#define DMA_TIM8_COM           {{ DMA_ANY, DMA_REQUEST_TIM8_COM     },{}}
#define DMA_TIM5_CH1           {{ DMA_ANY, DMA_REQUEST_TIM5_CH1     },{}}
#define DMA_TIM5_CH2           {{ DMA_ANY, DMA_REQUEST_TIM5_CH2     },{}}
#define DMA_TIM5_CH3           {{ DMA_ANY, DMA_REQUEST_TIM5_CH3     },{}}
#define DMA_TIM5_CH4           {{ DMA_ANY, DMA_REQUEST_TIM5_CH4     },{}}
#define DMA_TIM5_UP            {{ DMA_ANY, DMA_REQUEST_TIM5_UP      },{}}
#define DMA_TIM5_TRIG          {{ DMA_ANY, DMA_REQUEST_TIM5_TRIG    },{}}
#define DMA_SPI3_RX            {{ DMA_ANY, DMA_REQUEST_SPI3_RX      },{}}
#define DMA_SPI3_TX            {{ DMA_ANY, DMA_REQUEST_SPI3_TX      },{}}
#define DMA_UART4_RX           {{ DMA_ANY, DMA_REQUEST_UART4_RX     },{}}
#define DMA_UART4_TX           {{ DMA_ANY, DMA_REQUEST_UART4_TX     },{}}
#define DMA_UART5_RX           {{ DMA_ANY, DMA_REQUEST_UART5_RX     },{}}
#define DMA_UART5_TX           {{ DMA_ANY, DMA_REQUEST_UART5_TX     },{}}
#define DMA_DAC1               {{ DMA_ANY, DMA_REQUEST_DAC1         },{}}
#define DMA_DAC2               {{ DMA_ANY, DMA_REQUEST_DAC2         },{}}
#define DMA_TIM6_UP            {{ DMA_ANY, DMA_REQUEST_TIM6_UP      },{}}
#define DMA_TIM7_UP            {{ DMA_ANY, DMA_REQUEST_TIM7_UP      },{}}
#define DMA_USART6_RX          {{ DMA_ANY, DMA_REQUEST_USART6_RX    },{}}
#define DMA_UART6_RX DMA_USART6_RX
#define DMA_USART6_TX          {{ DMA_ANY, DMA_REQUEST_USART6_TX    },{}}
#define DMA_UART6_TX DMA_USART6_TX
#define DMA_I2C3_RX            {{ DMA_ANY, DMA_REQUEST_I2C3_RX      },{}}
#define DMA_I2C3_TX            {{ DMA_ANY, DMA_REQUEST_I2C3_TX      },{}}
#define DMA_DCMI               {{ DMA_ANY, DMA_REQUEST_DCMI         },{}}
#define DMA_CRYP_IN            {{ DMA_ANY, DMA_REQUEST_CRYP_IN      },{}}
#define DMA_CRYP_OUT           {{ DMA_ANY, DMA_REQUEST_CRYP_OUT     },{}}
#define DMA_HASH_IN            {{ DMA_ANY, DMA_REQUEST_HASH_IN      },{}}
#define DMA_UART7_RX           {{ DMA_ANY, DMA_REQUEST_UART7_RX     },{}}
#define DMA_UART7_TX           {{ DMA_ANY, DMA_REQUEST_UART7_TX     },{}}
#define DMA_UART8_RX           {{ DMA_ANY, DMA_REQUEST_UART8_RX     },{}}
#define DMA_UART8_TX           {{ DMA_ANY, DMA_REQUEST_UART8_TX     },{}}
#define DMA_SPI4_RX            {{ DMA_ANY, DMA_REQUEST_SPI4_RX      },{}}
#define DMA_SPI4_TX            {{ DMA_ANY, DMA_REQUEST_SPI4_TX      },{}}
#define DMA_SPI5_RX            {{ DMA_ANY, DMA_REQUEST_SPI5_RX      },{}}
#define DMA_SPI5_TX            {{ DMA_ANY, DMA_REQUEST_SPI5_TX      },{}}
#define DMA_SAI1_A             {{ DMA_ANY, DMA_REQUEST_SAI1_A       },{}}
#define DMA_SAI1_B             {{ DMA_ANY, DMA_REQUEST_SAI1_B       },{}}
#define DMA_SAI2_A             {{ DMA_ANY, DMA_REQUEST_SAI2_A       },{}}
#define DMA_SAI2_B             {{ DMA_ANY, DMA_REQUEST_SAI2_B       },{}}
#define DMA_SWPMI_RX           {{ DMA_ANY, DMA_REQUEST_SWPMI_RX     },{}}
#define DMA_SWPMI_TX           {{ DMA_ANY, DMA_REQUEST_SWPMI_TX     },{}}
#define DMA_SPDIF_RX_DT        {{ DMA_ANY, DMA_REQUEST_SPDIF_RX_DT  },{}}
#define DMA_SPDIF_RX_CS        {{ DMA_ANY, DMA_REQUEST_SPDIF_RX_CS  },{}}
#define DMA_HRTIM_MASTER       {{ DMA_ANY, DMA_REQUEST_HRTIM_MASTER },{}}
#define DMA_HRTIM_TIMER_A      {{ DMA_ANY, DMA_REQUEST_HRTIM_TIMER_A},{}}
#define DMA_HRTIM_TIMER_B      {{ DMA_ANY, DMA_REQUEST_HRTIM_TIMER_B},{}}
#define DMA_HRTIM_TIMER_C      {{ DMA_ANY, DMA_REQUEST_HRTIM_TIMER_C},{}}
#define DMA_HRTIM_TIMER_D      {{ DMA_ANY, DMA_REQUEST_HRTIM_TIMER_D},{}}
#define DMA_HRTIM_TIMER_E      {{ DMA_ANY, DMA_REQUEST_HRTIM_TIMER_E},{}}
#define DMA_DFSDM1_FLT0        {{ DMA_ANY, DMA_REQUEST_DFSDM1_FLT0  },{}}
#define DMA_DFSDM1_FLT1        {{ DMA_ANY, DMA_REQUEST_DFSDM1_FLT1  },{}}
#define DMA_DFSDM1_FLT2        {{ DMA_ANY, DMA_REQUEST_DFSDM1_FLT2  },{}}
#define DMA_DFSDM1_FLT3        {{ DMA_ANY, DMA_REQUEST_DFSDM1_FLT3  },{}}
#define DMA_TIM15_CH1          {{ DMA_ANY, DMA_REQUEST_TIM15_CH1    },{}}
#define DMA_TIM15_UP           {{ DMA_ANY, DMA_REQUEST_TIM15_UP     },{}}
#define DMA_TIM15_TRIG         {{ DMA_ANY, DMA_REQUEST_TIM15_TRIG   },{}}
#define DMA_TIM15_COM          {{ DMA_ANY, DMA_REQUEST_TIM15_COM    },{}}
#define DMA_TIM16_CH1          {{ DMA_ANY, DMA_REQUEST_TIM16_CH1    },{}}
#define DMA_TIM16_UP           {{ DMA_ANY, DMA_REQUEST_TIM16_UP     },{}}
#define DMA_TIM17_CH1          {{ DMA_ANY, DMA_REQUEST_TIM17_CH1    },{}}
#define DMA_TIM17_UP           {{ DMA_ANY, DMA_REQUEST_TIM17_UP     },{}}
#define DMA_SAI3_A             {{ DMA_ANY, DMA_REQUEST_SAI3_A       },{}}
#define DMA_SAI3_B             {{ DMA_ANY, DMA_REQUEST_SAI3_B       },{}}
#define DMA_ADC3               {{ DMA_ANY, DMA_REQUEST_ADC3         },{}}

#endif
