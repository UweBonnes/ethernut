/* Do not edit!
 * Generated from /evo860/opt/stm/STM32CubeMX/db/mcu/IPGPIO-STM32G07x_gpio_v1_0_Modes.xml*/

#if !defined(_STM32G07x_PINMUX_H_)
# define _STM32G07x_PINMUX_H_

#define CEC_FUNC                 0
#define COMP1_OUT_FUNC           1
#define COMP2_OUT_FUNC           2
#define EVENTOUT_FUNC            3
#define I2C1_SCL_FUNC            4
#define I2C1_SDA_FUNC            5
#define I2C1_SMBA_FUNC           6
#define I2C2_SCL_FUNC            7
#define I2C2_SDA_FUNC            8
#define I2S1_CK_FUNC             9
#define I2S1_MCK_FUNC            10
#define I2S1_SD_FUNC             11
#define I2S1_WS_FUNC             12
#define I2S_CKIN_FUNC            13
#define IR_OUT_FUNC              14
#define LPTIM1_ETR_FUNC          15
#define LPTIM1_IN1_FUNC          16
#define LPTIM1_IN2_FUNC          17
#define LPTIM1_OUT_FUNC          18
#define LPTIM2_ETR_FUNC          19
#define LPTIM2_IN1_FUNC          20
#define LPTIM2_OUT_FUNC          21
#define LPUART1_CTS_FUNC         22
#define LPUART1_DE_FUNC          23
#define LPUART1_RTS_FUNC         24
#define LPUART1_RX_FUNC          25
#define LPUART1_TX_FUNC          26
#define RCC_MCO_FUNC             27
#define RCC_OSC32_EN_FUNC        28
#define RCC_OSC_EN_FUNC          29
#define SPI1_MISO_FUNC           30
#define SPI1_MOSI_FUNC           31
#define SPI1_NSS_FUNC            32
#define SPI1_SCK_FUNC            33
#define SPI2_MISO_FUNC           34
#define SPI2_MOSI_FUNC           35
#define SPI2_NSS_FUNC            36
#define SPI2_SCK_FUNC            37
#define SYS_SWCLK_FUNC           38
#define SYS_SWDIO_FUNC           39
#define TIM14_CH1_FUNC           40
#define TIM15_BK_FUNC            41
#define TIM15_CH1_FUNC           42
#define TIM15_CH1N_FUNC          43
#define TIM15_CH2_FUNC           44
#define TIM16_BK_FUNC            45
#define TIM16_CH1_FUNC           46
#define TIM16_CH1N_FUNC          47
#define TIM17_BK_FUNC            48
#define TIM17_CH1_FUNC           49
#define TIM17_CH1N_FUNC          50
#define TIM1_BK_FUNC             51
#define TIM1_BK2_FUNC            52
#define TIM1_CH1_FUNC            53
#define TIM1_CH1N_FUNC           54
#define TIM1_CH2_FUNC            55
#define TIM1_CH2N_FUNC           56
#define TIM1_CH3_FUNC            57
#define TIM1_CH3N_FUNC           58
#define TIM1_CH4_FUNC            59
#define TIM1_ETR_FUNC            60
#define TIM2_CH1_FUNC            61
#define TIM2_CH2_FUNC            62
#define TIM2_CH3_FUNC            63
#define TIM2_CH4_FUNC            64
#define TIM2_ETR_FUNC            65
#define TIM3_CH1_FUNC            66
#define TIM3_CH2_FUNC            67
#define TIM3_CH3_FUNC            68
#define TIM3_CH4_FUNC            69
#define TIM3_ETR_FUNC            70
#define UCPD1_FRSTX_FUNC         71
#define UCPD1_TXDATA_FUNC        72
#define UCPD1_TXGND_FUNC         73
#define UCPD2_FRSTX_FUNC         74
#define UCPD2_TXDATA_FUNC        75
#define UCPD2_TXGND_FUNC         76
#define USART1_CK_FUNC           77
#define UART1_CK_FUNC            77
#define USART1_CTS_FUNC          78
#define UART1_CTS_FUNC           78
#define USART1_DE_FUNC           79
#define UART1_DE_FUNC            79
#define USART1_NSS_FUNC          80
#define UART1_NSS_FUNC           80
#define USART1_RTS_FUNC          81
#define UART1_RTS_FUNC           81
#define USART1_RX_FUNC           82
#define UART1_RX_FUNC            82
#define USART1_TX_FUNC           83
#define UART1_TX_FUNC            83
#define USART2_CK_FUNC           84
#define UART2_CK_FUNC            84
#define USART2_CTS_FUNC          85
#define UART2_CTS_FUNC           85
#define USART2_DE_FUNC           86
#define UART2_DE_FUNC            86
#define USART2_NSS_FUNC          87
#define UART2_NSS_FUNC           87
#define USART2_RTS_FUNC          88
#define UART2_RTS_FUNC           88
#define USART2_RX_FUNC           89
#define UART2_RX_FUNC            89
#define USART2_TX_FUNC           90
#define UART2_TX_FUNC            90
#define USART3_CK_FUNC           91
#define UART3_CK_FUNC            91
#define USART3_CTS_FUNC          92
#define UART3_CTS_FUNC           92
#define USART3_DE_FUNC           93
#define UART3_DE_FUNC            93
#define USART3_NSS_FUNC          94
#define UART3_NSS_FUNC           94
#define USART3_RTS_FUNC          95
#define UART3_RTS_FUNC           95
#define USART3_RX_FUNC           96
#define UART3_RX_FUNC            96
#define USART3_TX_FUNC           97
#define UART3_TX_FUNC            97
#define USART4_CK_FUNC           98
#define UART4_CK_FUNC            98
#define USART4_CTS_FUNC          99
#define UART4_CTS_FUNC           99
#define USART4_DE_FUNC           100
#define UART4_DE_FUNC            100
#define USART4_NSS_FUNC          101
#define UART4_NSS_FUNC           101
#define USART4_RTS_FUNC          102
#define UART4_RTS_FUNC           102
#define USART4_RX_FUNC           103
#define UART4_RX_FUNC            103
#define USART4_TX_FUNC           104
#define UART4_TX_FUNC            104

# define PINMUX(gpio, func) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (func == SPI2_SCK_FUNC            )) ?  0 : \
	((gpio == PA00) && (func == USART2_CTS_FUNC          )) ?  1 : \
	((gpio == PA00) && (func == USART2_NSS_FUNC          )) ?  1 : \
	((gpio == PA00) && (func == TIM2_CH1_FUNC            )) ?  2 : \
	((gpio == PA00) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA00) && (func == USART4_TX_FUNC           )) ?  4 : \
	((gpio == PA00) && (func == LPTIM1_OUT_FUNC          )) ?  5 : \
	((gpio == PA00) && (func == UCPD2_FRSTX_FUNC         )) ?  6 : \
	((gpio == PA00) && (func == COMP1_OUT_FUNC           )) ?  7 : \
	((gpio == PA01) && (func == I2S1_CK_FUNC             )) ?  0 : \
	((gpio == PA01) && (func == SPI1_SCK_FUNC            )) ?  0 : \
	((gpio == PA01) && (func == USART2_CK_FUNC           )) ?  1 : \
	((gpio == PA01) && (func == USART2_DE_FUNC           )) ?  1 : \
	((gpio == PA01) && (func == USART2_RTS_FUNC          )) ?  1 : \
	((gpio == PA01) && (func == TIM2_CH2_FUNC            )) ?  2 : \
	((gpio == PA01) && (func == UCPD1_TXGND_FUNC         )) ?  3 : \
	((gpio == PA01) && (func == USART4_RX_FUNC           )) ?  4 : \
	((gpio == PA01) && (func == TIM15_CH1N_FUNC          )) ?  5 : \
	((gpio == PA01) && (func == I2C1_SMBA_FUNC           )) ?  6 : \
	((gpio == PA01) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PA02) && (func == I2S1_SD_FUNC             )) ?  0 : \
	((gpio == PA02) && (func == SPI1_MOSI_FUNC           )) ?  0 : \
	((gpio == PA02) && (func == USART2_TX_FUNC           )) ?  1 : \
	((gpio == PA02) && (func == TIM2_CH3_FUNC            )) ?  2 : \
	((gpio == PA02) && (func == UCPD1_FRSTX_FUNC         )) ?  4 : \
	((gpio == PA02) && (func == TIM15_CH1_FUNC           )) ?  5 : \
	((gpio == PA02) && (func == LPUART1_TX_FUNC          )) ?  6 : \
	((gpio == PA02) && (func == COMP2_OUT_FUNC           )) ?  7 : \
	((gpio == PA03) && (func == SPI2_MISO_FUNC           )) ?  0 : \
	((gpio == PA03) && (func == USART2_RX_FUNC           )) ?  1 : \
	((gpio == PA03) && (func == TIM2_CH4_FUNC            )) ?  2 : \
	((gpio == PA03) && (func == UCPD2_TXGND_FUNC         )) ?  3 : \
	((gpio == PA03) && (func == UCPD2_FRSTX_FUNC         )) ?  4 : \
	((gpio == PA03) && (func == TIM15_CH2_FUNC           )) ?  5 : \
	((gpio == PA03) && (func == LPUART1_RX_FUNC          )) ?  6 : \
	((gpio == PA03) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PA04) && (func == I2S1_WS_FUNC             )) ?  0 : \
	((gpio == PA04) && (func == SPI1_NSS_FUNC            )) ?  0 : \
	((gpio == PA04) && (func == SPI2_MOSI_FUNC           )) ?  1 : \
	((gpio == PA04) && (func == TIM14_CH1_FUNC           )) ?  4 : \
	((gpio == PA04) && (func == LPTIM2_OUT_FUNC          )) ?  5 : \
	((gpio == PA04) && (func == UCPD2_FRSTX_FUNC         )) ?  6 : \
	((gpio == PA04) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PA05) && (func == I2S1_CK_FUNC             )) ?  0 : \
	((gpio == PA05) && (func == SPI1_SCK_FUNC            )) ?  0 : \
	((gpio == PA05) && (func == CEC_FUNC                 )) ?  1 : \
	((gpio == PA05) && (func == TIM2_CH1_FUNC            )) ?  2 : \
	((gpio == PA05) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA05) && (func == UCPD1_TXDATA_FUNC        )) ?  3 : \
	((gpio == PA05) && (func == USART3_TX_FUNC           )) ?  4 : \
	((gpio == PA05) && (func == LPTIM2_ETR_FUNC          )) ?  5 : \
	((gpio == PA05) && (func == UCPD1_FRSTX_FUNC         )) ?  6 : \
	((gpio == PA05) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PA06) && (func == I2S1_MCK_FUNC            )) ?  0 : \
	((gpio == PA06) && (func == SPI1_MISO_FUNC           )) ?  0 : \
	((gpio == PA06) && (func == TIM3_CH1_FUNC            )) ?  1 : \
	((gpio == PA06) && (func == TIM1_BK_FUNC             )) ?  2 : \
	((gpio == PA06) && (func == UCPD1_TXDATA_FUNC        )) ?  3 : \
	((gpio == PA06) && (func == USART3_CTS_FUNC          )) ?  4 : \
	((gpio == PA06) && (func == USART3_NSS_FUNC          )) ?  4 : \
	((gpio == PA06) && (func == TIM16_CH1_FUNC           )) ?  5 : \
	((gpio == PA06) && (func == LPUART1_CTS_FUNC         )) ?  6 : \
	((gpio == PA06) && (func == COMP1_OUT_FUNC           )) ?  7 : \
	((gpio == PA07) && (func == I2S1_SD_FUNC             )) ?  0 : \
	((gpio == PA07) && (func == SPI1_MOSI_FUNC           )) ?  0 : \
	((gpio == PA07) && (func == TIM3_CH2_FUNC            )) ?  1 : \
	((gpio == PA07) && (func == TIM1_CH1N_FUNC           )) ?  2 : \
	((gpio == PA07) && (func == UCPD2_TXDATA_FUNC        )) ?  3 : \
	((gpio == PA07) && (func == TIM14_CH1_FUNC           )) ?  4 : \
	((gpio == PA07) && (func == TIM17_CH1_FUNC           )) ?  5 : \
	((gpio == PA07) && (func == UCPD1_FRSTX_FUNC         )) ?  6 : \
	((gpio == PA07) && (func == COMP2_OUT_FUNC           )) ?  7 : \
	((gpio == PA08) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PA08) && (func == SPI2_NSS_FUNC            )) ?  1 : \
	((gpio == PA08) && (func == TIM1_CH1_FUNC            )) ?  2 : \
	((gpio == PA08) && (func == UCPD2_TXDATA_FUNC        )) ?  3 : \
	((gpio == PA08) && (func == LPTIM2_OUT_FUNC          )) ?  5 : \
	((gpio == PA08) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PA09) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PA09) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PA09) && (func == USART1_TX_FUNC           )) ?  1 : \
	((gpio == PA09) && (func == USART1_TX_FUNC           )) ?  1 : \
	((gpio == PA09) && (func == TIM1_CH2_FUNC            )) ?  2 : \
	((gpio == PA09) && (func == TIM1_CH2_FUNC            )) ?  2 : \
	((gpio == PA09) && (func == UCPD1_TXGND_FUNC         )) ?  3 : \
	((gpio == PA09) && (func == UCPD1_TXGND_FUNC         )) ?  3 : \
	((gpio == PA09) && (func == SPI2_MISO_FUNC           )) ?  4 : \
	((gpio == PA09) && (func == SPI2_MISO_FUNC           )) ?  4 : \
	((gpio == PA09) && (func == TIM15_BK_FUNC            )) ?  5 : \
	((gpio == PA09) && (func == TIM15_BK_FUNC            )) ?  5 : \
	((gpio == PA09) && (func == I2C1_SCL_FUNC            )) ?  6 : \
	((gpio == PA09) && (func == I2C1_SCL_FUNC            )) ?  6 : \
	((gpio == PA09) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PA09) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PA10) && (func == SPI2_MOSI_FUNC           )) ?  0 : \
	((gpio == PA10) && (func == SPI2_MOSI_FUNC           )) ?  0 : \
	((gpio == PA10) && (func == USART1_RX_FUNC           )) ?  1 : \
	((gpio == PA10) && (func == USART1_RX_FUNC           )) ?  1 : \
	((gpio == PA10) && (func == TIM1_CH3_FUNC            )) ?  2 : \
	((gpio == PA10) && (func == TIM1_CH3_FUNC            )) ?  2 : \
	((gpio == PA10) && (func == UCPD2_TXGND_FUNC         )) ?  3 : \
	((gpio == PA10) && (func == UCPD2_TXGND_FUNC         )) ?  3 : \
	((gpio == PA10) && (func == TIM17_BK_FUNC            )) ?  5 : \
	((gpio == PA10) && (func == TIM17_BK_FUNC            )) ?  5 : \
	((gpio == PA10) && (func == I2C1_SDA_FUNC            )) ?  6 : \
	((gpio == PA10) && (func == I2C1_SDA_FUNC            )) ?  6 : \
	((gpio == PA10) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PA10) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PA11) && (func == I2S1_MCK_FUNC            )) ?  0 : \
	((gpio == PA11) && (func == SPI1_MISO_FUNC           )) ?  0 : \
	((gpio == PA11) && (func == USART1_CTS_FUNC          )) ?  1 : \
	((gpio == PA11) && (func == USART1_NSS_FUNC          )) ?  1 : \
	((gpio == PA11) && (func == TIM1_CH4_FUNC            )) ?  2 : \
	((gpio == PA11) && (func == TIM1_BK2_FUNC            )) ?  5 : \
	((gpio == PA11) && (func == I2C2_SCL_FUNC            )) ?  6 : \
	((gpio == PA11) && (func == COMP1_OUT_FUNC           )) ?  7 : \
	((gpio == PA12) && (func == I2S1_SD_FUNC             )) ?  0 : \
	((gpio == PA12) && (func == SPI1_MOSI_FUNC           )) ?  0 : \
	((gpio == PA12) && (func == USART1_CK_FUNC           )) ?  1 : \
	((gpio == PA12) && (func == USART1_DE_FUNC           )) ?  1 : \
	((gpio == PA12) && (func == USART1_RTS_FUNC          )) ?  1 : \
	((gpio == PA12) && (func == TIM1_ETR_FUNC            )) ?  2 : \
	((gpio == PA12) && (func == I2S_CKIN_FUNC            )) ?  5 : \
	((gpio == PA12) && (func == I2C2_SDA_FUNC            )) ?  6 : \
	((gpio == PA12) && (func == COMP2_OUT_FUNC           )) ?  7 : \
	((gpio == PA13) && (func == SYS_SWDIO_FUNC           )) ?  0 : \
	((gpio == PA13) && (func == IR_OUT_FUNC              )) ?  1 : \
	((gpio == PA13) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PA14) && (func == SYS_SWCLK_FUNC           )) ?  0 : \
	((gpio == PA14) && (func == USART2_TX_FUNC           )) ?  1 : \
	((gpio == PA14) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PA15) && (func == I2S1_WS_FUNC             )) ?  0 : \
	((gpio == PA15) && (func == SPI1_NSS_FUNC            )) ?  0 : \
	((gpio == PA15) && (func == USART2_RX_FUNC           )) ?  1 : \
	((gpio == PA15) && (func == TIM2_CH1_FUNC            )) ?  2 : \
	((gpio == PA15) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA15) && (func == USART4_CK_FUNC           )) ?  4 : \
	((gpio == PA15) && (func == USART4_DE_FUNC           )) ?  4 : \
	((gpio == PA15) && (func == USART4_RTS_FUNC          )) ?  4 : \
	((gpio == PA15) && (func == USART3_CK_FUNC           )) ?  5 : \
	((gpio == PA15) && (func == USART3_DE_FUNC           )) ?  5 : \
	((gpio == PA15) && (func == USART3_RTS_FUNC          )) ?  5 : \
	((gpio == PA15) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB00) && (func == I2S1_WS_FUNC             )) ?  0 : \
	((gpio == PB00) && (func == SPI1_NSS_FUNC            )) ?  0 : \
	((gpio == PB00) && (func == TIM3_CH3_FUNC            )) ?  1 : \
	((gpio == PB00) && (func == TIM1_CH2N_FUNC           )) ?  2 : \
	((gpio == PB00) && (func == USART3_RX_FUNC           )) ?  4 : \
	((gpio == PB00) && (func == LPTIM1_OUT_FUNC          )) ?  5 : \
	((gpio == PB00) && (func == UCPD1_FRSTX_FUNC         )) ?  6 : \
	((gpio == PB00) && (func == COMP1_OUT_FUNC           )) ?  7 : \
	((gpio == PB01) && (func == TIM14_CH1_FUNC           )) ?  0 : \
	((gpio == PB01) && (func == TIM3_CH4_FUNC            )) ?  1 : \
	((gpio == PB01) && (func == TIM1_CH3N_FUNC           )) ?  2 : \
	((gpio == PB01) && (func == USART3_CK_FUNC           )) ?  4 : \
	((gpio == PB01) && (func == USART3_DE_FUNC           )) ?  4 : \
	((gpio == PB01) && (func == USART3_RTS_FUNC          )) ?  4 : \
	((gpio == PB01) && (func == LPTIM2_IN1_FUNC          )) ?  5 : \
	((gpio == PB01) && (func == LPUART1_DE_FUNC          )) ?  6 : \
	((gpio == PB01) && (func == LPUART1_RTS_FUNC         )) ?  6 : \
	((gpio == PB01) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB02) && (func == SPI2_MISO_FUNC           )) ?  1 : \
	((gpio == PB02) && (func == UCPD1_TXGND_FUNC         )) ?  3 : \
	((gpio == PB02) && (func == USART3_TX_FUNC           )) ?  4 : \
	((gpio == PB02) && (func == LPTIM1_OUT_FUNC          )) ?  5 : \
	((gpio == PB02) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB03) && (func == I2S1_CK_FUNC             )) ?  0 : \
	((gpio == PB03) && (func == SPI1_SCK_FUNC            )) ?  0 : \
	((gpio == PB03) && (func == TIM1_CH2_FUNC            )) ?  1 : \
	((gpio == PB03) && (func == TIM2_CH2_FUNC            )) ?  2 : \
	((gpio == PB03) && (func == USART1_CK_FUNC           )) ?  4 : \
	((gpio == PB03) && (func == USART1_DE_FUNC           )) ?  4 : \
	((gpio == PB03) && (func == USART1_RTS_FUNC          )) ?  4 : \
	((gpio == PB03) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB04) && (func == I2S1_MCK_FUNC            )) ?  0 : \
	((gpio == PB04) && (func == SPI1_MISO_FUNC           )) ?  0 : \
	((gpio == PB04) && (func == TIM3_CH1_FUNC            )) ?  1 : \
	((gpio == PB04) && (func == UCPD2_TXGND_FUNC         )) ?  3 : \
	((gpio == PB04) && (func == USART1_CTS_FUNC          )) ?  4 : \
	((gpio == PB04) && (func == USART1_NSS_FUNC          )) ?  4 : \
	((gpio == PB04) && (func == TIM17_BK_FUNC            )) ?  5 : \
	((gpio == PB04) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB05) && (func == I2S1_SD_FUNC             )) ?  0 : \
	((gpio == PB05) && (func == SPI1_MOSI_FUNC           )) ?  0 : \
	((gpio == PB05) && (func == TIM3_CH2_FUNC            )) ?  1 : \
	((gpio == PB05) && (func == TIM16_BK_FUNC            )) ?  2 : \
	((gpio == PB05) && (func == LPTIM1_IN1_FUNC          )) ?  5 : \
	((gpio == PB05) && (func == I2C1_SMBA_FUNC           )) ?  6 : \
	((gpio == PB05) && (func == COMP2_OUT_FUNC           )) ?  7 : \
	((gpio == PB06) && (func == USART1_TX_FUNC           )) ?  0 : \
	((gpio == PB06) && (func == TIM1_CH3_FUNC            )) ?  1 : \
	((gpio == PB06) && (func == TIM16_CH1N_FUNC          )) ?  2 : \
	((gpio == PB06) && (func == UCPD1_TXGND_FUNC         )) ?  3 : \
	((gpio == PB06) && (func == SPI2_MISO_FUNC           )) ?  4 : \
	((gpio == PB06) && (func == LPTIM1_ETR_FUNC          )) ?  5 : \
	((gpio == PB06) && (func == I2C1_SCL_FUNC            )) ?  6 : \
	((gpio == PB06) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB07) && (func == USART1_RX_FUNC           )) ?  0 : \
	((gpio == PB07) && (func == SPI2_MOSI_FUNC           )) ?  1 : \
	((gpio == PB07) && (func == TIM17_CH1N_FUNC          )) ?  2 : \
	((gpio == PB07) && (func == USART4_CTS_FUNC          )) ?  4 : \
	((gpio == PB07) && (func == USART4_NSS_FUNC          )) ?  4 : \
	((gpio == PB07) && (func == LPTIM1_IN2_FUNC          )) ?  5 : \
	((gpio == PB07) && (func == I2C1_SDA_FUNC            )) ?  6 : \
	((gpio == PB07) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB08) && (func == CEC_FUNC                 )) ?  0 : \
	((gpio == PB08) && (func == SPI2_SCK_FUNC            )) ?  1 : \
	((gpio == PB08) && (func == TIM16_CH1_FUNC           )) ?  2 : \
	((gpio == PB08) && (func == UCPD1_TXGND_FUNC         )) ?  3 : \
	((gpio == PB08) && (func == USART3_TX_FUNC           )) ?  4 : \
	((gpio == PB08) && (func == TIM15_BK_FUNC            )) ?  5 : \
	((gpio == PB08) && (func == I2C1_SCL_FUNC            )) ?  6 : \
	((gpio == PB08) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB09) && (func == IR_OUT_FUNC              )) ?  0 : \
	((gpio == PB09) && (func == UCPD2_FRSTX_FUNC         )) ?  1 : \
	((gpio == PB09) && (func == TIM17_CH1_FUNC           )) ?  2 : \
	((gpio == PB09) && (func == UCPD2_TXGND_FUNC         )) ?  3 : \
	((gpio == PB09) && (func == USART3_RX_FUNC           )) ?  4 : \
	((gpio == PB09) && (func == SPI2_NSS_FUNC            )) ?  5 : \
	((gpio == PB09) && (func == I2C1_SDA_FUNC            )) ?  6 : \
	((gpio == PB09) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB10) && (func == CEC_FUNC                 )) ?  0 : \
	((gpio == PB10) && (func == LPUART1_RX_FUNC          )) ?  1 : \
	((gpio == PB10) && (func == TIM2_CH3_FUNC            )) ?  2 : \
	((gpio == PB10) && (func == UCPD1_TXGND_FUNC         )) ?  3 : \
	((gpio == PB10) && (func == USART3_TX_FUNC           )) ?  4 : \
	((gpio == PB10) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PB10) && (func == I2C2_SCL_FUNC            )) ?  6 : \
	((gpio == PB10) && (func == COMP1_OUT_FUNC           )) ?  7 : \
	((gpio == PB11) && (func == SPI2_MOSI_FUNC           )) ?  0 : \
	((gpio == PB11) && (func == LPUART1_TX_FUNC          )) ?  1 : \
	((gpio == PB11) && (func == TIM2_CH4_FUNC            )) ?  2 : \
	((gpio == PB11) && (func == UCPD1_TXGND_FUNC         )) ?  3 : \
	((gpio == PB11) && (func == USART3_RX_FUNC           )) ?  4 : \
	((gpio == PB11) && (func == I2C2_SDA_FUNC            )) ?  6 : \
	((gpio == PB11) && (func == COMP2_OUT_FUNC           )) ?  7 : \
	((gpio == PB12) && (func == SPI2_NSS_FUNC            )) ?  0 : \
	((gpio == PB12) && (func == LPUART1_DE_FUNC          )) ?  1 : \
	((gpio == PB12) && (func == LPUART1_RTS_FUNC         )) ?  1 : \
	((gpio == PB12) && (func == TIM1_BK_FUNC             )) ?  2 : \
	((gpio == PB12) && (func == TIM15_BK_FUNC            )) ?  5 : \
	((gpio == PB12) && (func == UCPD2_FRSTX_FUNC         )) ?  6 : \
	((gpio == PB12) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB13) && (func == SPI2_SCK_FUNC            )) ?  0 : \
	((gpio == PB13) && (func == LPUART1_CTS_FUNC         )) ?  1 : \
	((gpio == PB13) && (func == TIM1_CH1N_FUNC           )) ?  2 : \
	((gpio == PB13) && (func == UCPD2_TXGND_FUNC         )) ?  3 : \
	((gpio == PB13) && (func == USART3_CTS_FUNC          )) ?  4 : \
	((gpio == PB13) && (func == USART3_NSS_FUNC          )) ?  4 : \
	((gpio == PB13) && (func == TIM15_CH1N_FUNC          )) ?  5 : \
	((gpio == PB13) && (func == I2C2_SCL_FUNC            )) ?  6 : \
	((gpio == PB13) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB14) && (func == SPI2_MISO_FUNC           )) ?  0 : \
	((gpio == PB14) && (func == UCPD1_FRSTX_FUNC         )) ?  1 : \
	((gpio == PB14) && (func == TIM1_CH2N_FUNC           )) ?  2 : \
	((gpio == PB14) && (func == UCPD2_TXGND_FUNC         )) ?  3 : \
	((gpio == PB14) && (func == USART3_CK_FUNC           )) ?  4 : \
	((gpio == PB14) && (func == USART3_DE_FUNC           )) ?  4 : \
	((gpio == PB14) && (func == USART3_RTS_FUNC          )) ?  4 : \
	((gpio == PB14) && (func == TIM15_CH1_FUNC           )) ?  5 : \
	((gpio == PB14) && (func == I2C2_SDA_FUNC            )) ?  6 : \
	((gpio == PB14) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PB15) && (func == SPI2_MOSI_FUNC           )) ?  0 : \
	((gpio == PB15) && (func == TIM1_CH3N_FUNC           )) ?  2 : \
	((gpio == PB15) && (func == TIM15_CH1N_FUNC          )) ?  4 : \
	((gpio == PB15) && (func == TIM15_CH2_FUNC           )) ?  5 : \
	((gpio == PB15) && (func == EVENTOUT_FUNC            )) ?  7 : \
	((gpio == PC00) && (func == LPTIM1_IN1_FUNC          )) ?  0 : \
	((gpio == PC00) && (func == LPUART1_RX_FUNC          )) ?  1 : \
	((gpio == PC00) && (func == LPTIM2_IN1_FUNC          )) ?  2 : \
	((gpio == PC00) && (func == UCPD1_TXGND_FUNC         )) ?  3 : \
	((gpio == PC01) && (func == LPTIM1_OUT_FUNC          )) ?  0 : \
	((gpio == PC01) && (func == LPUART1_TX_FUNC          )) ?  1 : \
	((gpio == PC01) && (func == TIM15_CH1_FUNC           )) ?  2 : \
	((gpio == PC01) && (func == UCPD1_TXGND_FUNC         )) ?  3 : \
	((gpio == PC02) && (func == LPTIM1_IN2_FUNC          )) ?  0 : \
	((gpio == PC02) && (func == SPI2_MISO_FUNC           )) ?  1 : \
	((gpio == PC02) && (func == TIM15_CH2_FUNC           )) ?  2 : \
	((gpio == PC02) && (func == UCPD2_TXGND_FUNC         )) ?  3 : \
	((gpio == PC03) && (func == LPTIM1_ETR_FUNC          )) ?  0 : \
	((gpio == PC03) && (func == SPI2_MOSI_FUNC           )) ?  1 : \
	((gpio == PC03) && (func == LPTIM2_ETR_FUNC          )) ?  2 : \
	((gpio == PC03) && (func == UCPD2_TXGND_FUNC         )) ?  3 : \
	((gpio == PC04) && (func == USART3_TX_FUNC           )) ?  0 : \
	((gpio == PC04) && (func == USART1_TX_FUNC           )) ?  1 : \
	((gpio == PC04) && (func == TIM2_CH1_FUNC            )) ?  2 : \
	((gpio == PC04) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PC05) && (func == USART3_RX_FUNC           )) ?  0 : \
	((gpio == PC05) && (func == USART1_RX_FUNC           )) ?  1 : \
	((gpio == PC05) && (func == TIM2_CH2_FUNC            )) ?  2 : \
	((gpio == PC05) && (func == UCPD2_TXGND_FUNC         )) ?  3 : \
	((gpio == PC06) && (func == UCPD1_FRSTX_FUNC         )) ?  0 : \
	((gpio == PC06) && (func == TIM3_CH1_FUNC            )) ?  1 : \
	((gpio == PC06) && (func == TIM2_CH3_FUNC            )) ?  2 : \
	((gpio == PC06) && (func == UCPD1_TXDATA_FUNC        )) ?  3 : \
	((gpio == PC07) && (func == UCPD2_FRSTX_FUNC         )) ?  0 : \
	((gpio == PC07) && (func == TIM3_CH2_FUNC            )) ?  1 : \
	((gpio == PC07) && (func == TIM2_CH4_FUNC            )) ?  2 : \
	((gpio == PC07) && (func == UCPD2_TXDATA_FUNC        )) ?  3 : \
	((gpio == PC08) && (func == UCPD2_FRSTX_FUNC         )) ?  0 : \
	((gpio == PC08) && (func == TIM3_CH3_FUNC            )) ?  1 : \
	((gpio == PC08) && (func == TIM1_CH1_FUNC            )) ?  2 : \
	((gpio == PC08) && (func == UCPD1_TXDATA_FUNC        )) ?  3 : \
	((gpio == PC09) && (func == I2S_CKIN_FUNC            )) ?  0 : \
	((gpio == PC09) && (func == TIM3_CH4_FUNC            )) ?  1 : \
	((gpio == PC09) && (func == TIM1_CH2_FUNC            )) ?  2 : \
	((gpio == PC09) && (func == UCPD1_TXDATA_FUNC        )) ?  3 : \
	((gpio == PC10) && (func == USART3_TX_FUNC           )) ?  0 : \
	((gpio == PC10) && (func == USART4_TX_FUNC           )) ?  1 : \
	((gpio == PC10) && (func == TIM1_CH3_FUNC            )) ?  2 : \
	((gpio == PC10) && (func == UCPD2_TXDATA_FUNC        )) ?  3 : \
	((gpio == PC11) && (func == USART3_RX_FUNC           )) ?  0 : \
	((gpio == PC11) && (func == USART4_RX_FUNC           )) ?  1 : \
	((gpio == PC11) && (func == TIM1_CH4_FUNC            )) ?  2 : \
	((gpio == PC11) && (func == UCPD2_TXDATA_FUNC        )) ?  3 : \
	((gpio == PC12) && (func == LPTIM1_IN1_FUNC          )) ?  0 : \
	((gpio == PC12) && (func == UCPD1_FRSTX_FUNC         )) ?  1 : \
	((gpio == PC12) && (func == TIM14_CH1_FUNC           )) ?  2 : \
	((gpio == PC13) && (func == TIM1_BK_FUNC             )) ?  2 : \
	((gpio == PC14) && (func == TIM1_BK2_FUNC            )) ?  2 : \
	((gpio == PC15) && (func == RCC_OSC32_EN_FUNC        )) ?  0 : \
	((gpio == PC15) && (func == RCC_OSC_EN_FUNC          )) ?  1 : \
	((gpio == PC15) && (func == TIM15_BK_FUNC            )) ?  2 : \
	((gpio == PD00) && (func == EVENTOUT_FUNC            )) ?  0 : \
	((gpio == PD00) && (func == SPI2_NSS_FUNC            )) ?  1 : \
	((gpio == PD00) && (func == TIM16_CH1_FUNC           )) ?  2 : \
	((gpio == PD00) && (func == UCPD1_TXDATA_FUNC        )) ?  3 : \
	((gpio == PD01) && (func == EVENTOUT_FUNC            )) ?  0 : \
	((gpio == PD01) && (func == SPI2_SCK_FUNC            )) ?  1 : \
	((gpio == PD01) && (func == TIM17_CH1_FUNC           )) ?  2 : \
	((gpio == PD01) && (func == UCPD1_TXDATA_FUNC        )) ?  3 : \
	((gpio == PD02) && (func == USART3_CK_FUNC           )) ?  0 : \
	((gpio == PD02) && (func == USART3_DE_FUNC           )) ?  0 : \
	((gpio == PD02) && (func == USART3_RTS_FUNC          )) ?  0 : \
	((gpio == PD02) && (func == TIM3_ETR_FUNC            )) ?  1 : \
	((gpio == PD02) && (func == TIM1_CH1N_FUNC           )) ?  2 : \
	((gpio == PD02) && (func == UCPD2_TXDATA_FUNC        )) ?  3 : \
	((gpio == PD03) && (func == USART2_CTS_FUNC          )) ?  0 : \
	((gpio == PD03) && (func == USART2_NSS_FUNC          )) ?  0 : \
	((gpio == PD03) && (func == SPI2_MISO_FUNC           )) ?  1 : \
	((gpio == PD03) && (func == TIM1_CH2N_FUNC           )) ?  2 : \
	((gpio == PD03) && (func == UCPD2_TXDATA_FUNC        )) ?  3 : \
	((gpio == PD04) && (func == USART2_CK_FUNC           )) ?  0 : \
	((gpio == PD04) && (func == USART2_DE_FUNC           )) ?  0 : \
	((gpio == PD04) && (func == USART2_RTS_FUNC          )) ?  0 : \
	((gpio == PD04) && (func == SPI2_MOSI_FUNC           )) ?  1 : \
	((gpio == PD04) && (func == TIM1_CH3N_FUNC           )) ?  2 : \
	((gpio == PD05) && (func == USART2_TX_FUNC           )) ?  0 : \
	((gpio == PD05) && (func == I2S1_MCK_FUNC            )) ?  1 : \
	((gpio == PD05) && (func == SPI1_MISO_FUNC           )) ?  1 : \
	((gpio == PD05) && (func == TIM1_BK_FUNC             )) ?  2 : \
	((gpio == PD06) && (func == USART2_RX_FUNC           )) ?  0 : \
	((gpio == PD06) && (func == I2S1_SD_FUNC             )) ?  1 : \
	((gpio == PD06) && (func == SPI1_MOSI_FUNC           )) ?  1 : \
	((gpio == PD06) && (func == LPTIM2_OUT_FUNC          )) ?  2 : \
	((gpio == PD08) && (func == USART3_TX_FUNC           )) ?  0 : \
	((gpio == PD08) && (func == I2S1_CK_FUNC             )) ?  1 : \
	((gpio == PD08) && (func == SPI1_SCK_FUNC            )) ?  1 : \
	((gpio == PD08) && (func == LPTIM1_OUT_FUNC          )) ?  2 : \
	((gpio == PD08) && (func == UCPD1_TXDATA_FUNC        )) ?  3 : \
	((gpio == PD09) && (func == USART3_RX_FUNC           )) ?  0 : \
	((gpio == PD09) && (func == I2S1_WS_FUNC             )) ?  1 : \
	((gpio == PD09) && (func == SPI1_NSS_FUNC            )) ?  1 : \
	((gpio == PD09) && (func == TIM1_BK2_FUNC            )) ?  2 : \
	((gpio == PD09) && (func == UCPD2_TXDATA_FUNC        )) ?  3 : \
	((gpio == PF00) && (func == TIM14_CH1_FUNC           )) ?  2 : \
	((gpio == PF01) && (func == RCC_OSC_EN_FUNC          )) ?  0 : \
	((gpio == PF01) && (func == TIM15_CH1N_FUNC          )) ?  2 : \
	((gpio == PF02) && (func == RCC_MCO_FUNC             )) ?  0 : \
	AF_NO_SUCH_PINFUNC)

# define TIMERMUX(gpio, timer_nr, channel_nr) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (timer_nr ==  2) && (channel_nr ==  1)) ?  2 : \
	((gpio == PA01) && (timer_nr ==  2) && (channel_nr ==  2)) ?  2 : \
	((gpio == PA01) && (timer_nr == 15) && (channel_nr == -1)) ?  5 : \
	((gpio == PA02) && (timer_nr ==  2) && (channel_nr ==  3)) ?  2 : \
	((gpio == PA02) && (timer_nr == 15) && (channel_nr ==  1)) ?  5 : \
	((gpio == PA03) && (timer_nr ==  2) && (channel_nr ==  4)) ?  2 : \
	((gpio == PA03) && (timer_nr == 15) && (channel_nr ==  2)) ?  5 : \
	((gpio == PA04) && (timer_nr == 14) && (channel_nr ==  1)) ?  4 : \
	((gpio == PA05) && (timer_nr ==  2) && (channel_nr ==  1)) ?  2 : \
	((gpio == PA06) && (timer_nr ==  3) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA06) && (timer_nr == 16) && (channel_nr ==  1)) ?  5 : \
	((gpio == PA07) && (timer_nr ==  3) && (channel_nr ==  2)) ?  1 : \
	((gpio == PA07) && (timer_nr ==  1) && (channel_nr == -1)) ?  2 : \
	((gpio == PA07) && (timer_nr == 14) && (channel_nr ==  1)) ?  4 : \
	((gpio == PA07) && (timer_nr == 17) && (channel_nr ==  1)) ?  5 : \
	((gpio == PA08) && (timer_nr ==  1) && (channel_nr ==  1)) ?  2 : \
	((gpio == PA09) && (timer_nr ==  1) && (channel_nr ==  2)) ?  2 : \
	((gpio == PA09) && (timer_nr ==  1) && (channel_nr ==  2)) ?  2 : \
	((gpio == PA10) && (timer_nr ==  1) && (channel_nr ==  3)) ?  2 : \
	((gpio == PA10) && (timer_nr ==  1) && (channel_nr ==  3)) ?  2 : \
	((gpio == PA11) && (timer_nr ==  1) && (channel_nr ==  4)) ?  2 : \
	((gpio == PA15) && (timer_nr ==  2) && (channel_nr ==  1)) ?  2 : \
	((gpio == PB00) && (timer_nr ==  3) && (channel_nr ==  3)) ?  1 : \
	((gpio == PB00) && (timer_nr ==  1) && (channel_nr == -2)) ?  2 : \
	((gpio == PB01) && (timer_nr == 14) && (channel_nr ==  1)) ?  0 : \
	((gpio == PB01) && (timer_nr ==  3) && (channel_nr ==  4)) ?  1 : \
	((gpio == PB01) && (timer_nr ==  1) && (channel_nr == -3)) ?  2 : \
	((gpio == PB03) && (timer_nr ==  1) && (channel_nr ==  2)) ?  1 : \
	((gpio == PB03) && (timer_nr ==  2) && (channel_nr ==  2)) ?  2 : \
	((gpio == PB04) && (timer_nr ==  3) && (channel_nr ==  1)) ?  1 : \
	((gpio == PB05) && (timer_nr ==  3) && (channel_nr ==  2)) ?  1 : \
	((gpio == PB06) && (timer_nr ==  1) && (channel_nr ==  3)) ?  1 : \
	((gpio == PB06) && (timer_nr == 16) && (channel_nr == -1)) ?  2 : \
	((gpio == PB07) && (timer_nr == 17) && (channel_nr == -1)) ?  2 : \
	((gpio == PB08) && (timer_nr == 16) && (channel_nr ==  1)) ?  2 : \
	((gpio == PB09) && (timer_nr == 17) && (channel_nr ==  1)) ?  2 : \
	((gpio == PB10) && (timer_nr ==  2) && (channel_nr ==  3)) ?  2 : \
	((gpio == PB11) && (timer_nr ==  2) && (channel_nr ==  4)) ?  2 : \
	((gpio == PB13) && (timer_nr ==  1) && (channel_nr == -1)) ?  2 : \
	((gpio == PB13) && (timer_nr == 15) && (channel_nr == -1)) ?  5 : \
	((gpio == PB14) && (timer_nr ==  1) && (channel_nr == -2)) ?  2 : \
	((gpio == PB14) && (timer_nr == 15) && (channel_nr ==  1)) ?  5 : \
	((gpio == PB15) && (timer_nr ==  1) && (channel_nr == -3)) ?  2 : \
	((gpio == PB15) && (timer_nr == 15) && (channel_nr == -1)) ?  4 : \
	((gpio == PB15) && (timer_nr == 15) && (channel_nr ==  2)) ?  5 : \
	((gpio == PC01) && (timer_nr == 15) && (channel_nr ==  1)) ?  2 : \
	((gpio == PC02) && (timer_nr == 15) && (channel_nr ==  2)) ?  2 : \
	((gpio == PC04) && (timer_nr ==  2) && (channel_nr ==  1)) ?  2 : \
	((gpio == PC05) && (timer_nr ==  2) && (channel_nr ==  2)) ?  2 : \
	((gpio == PC06) && (timer_nr ==  3) && (channel_nr ==  1)) ?  1 : \
	((gpio == PC06) && (timer_nr ==  2) && (channel_nr ==  3)) ?  2 : \
	((gpio == PC07) && (timer_nr ==  3) && (channel_nr ==  2)) ?  1 : \
	((gpio == PC07) && (timer_nr ==  2) && (channel_nr ==  4)) ?  2 : \
	((gpio == PC08) && (timer_nr ==  3) && (channel_nr ==  3)) ?  1 : \
	((gpio == PC08) && (timer_nr ==  1) && (channel_nr ==  1)) ?  2 : \
	((gpio == PC09) && (timer_nr ==  3) && (channel_nr ==  4)) ?  1 : \
	((gpio == PC09) && (timer_nr ==  1) && (channel_nr ==  2)) ?  2 : \
	((gpio == PC10) && (timer_nr ==  1) && (channel_nr ==  3)) ?  2 : \
	((gpio == PC11) && (timer_nr ==  1) && (channel_nr ==  4)) ?  2 : \
	((gpio == PC12) && (timer_nr == 14) && (channel_nr ==  1)) ?  2 : \
	((gpio == PD00) && (timer_nr == 16) && (channel_nr ==  1)) ?  2 : \
	((gpio == PD01) && (timer_nr == 17) && (channel_nr ==  1)) ?  2 : \
	((gpio == PD02) && (timer_nr ==  1) && (channel_nr == -1)) ?  2 : \
	((gpio == PD03) && (timer_nr ==  1) && (channel_nr == -2)) ?  2 : \
	((gpio == PD04) && (timer_nr ==  1) && (channel_nr == -3)) ?  2 : \
	((gpio == PF00) && (timer_nr == 14) && (channel_nr ==  1)) ?  2 : \
	((gpio == PF01) && (timer_nr == 15) && (channel_nr == -1)) ?  2 : \
	AF_NO_SUCH_PINFUNC)
#endif
