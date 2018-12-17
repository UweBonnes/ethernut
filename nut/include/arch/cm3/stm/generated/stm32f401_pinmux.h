/* Do not edit!
 * Generated from /devel/cubemx/db/mcu/IPGPIO-STM32F401_gpio_v1_0_Modes.xml*/

#if !defined(_STM32F401_PINMUX_H_)
# define _STM32F401_PINMUX_H_

#define I2C1_SCL_FUNC            0
#define I2C1_SDA_FUNC            1
#define I2C1_SMBA_FUNC           2
#define I2C2_SCL_FUNC            3
#define I2C2_SDA_FUNC            4
#define I2C2_SMBA_FUNC           5
#define I2C3_SCL_FUNC            6
#define I2C3_SDA_FUNC            7
#define I2C3_SMBA_FUNC           8
#define I2S2_CK_FUNC             9
#define I2S2_MCK_FUNC            10
#define I2S2_SD_FUNC             11
#define I2S2_WS_FUNC             12
#define I2S2_ext_SD_FUNC         13
#define I2S2ext_CK_FUNC          14
#define I2S2ext_MISO_FUNC        15
#define I2S2ext_WS_FUNC          16
#define I2S3_CK_FUNC             17
#define I2S3_MCK_FUNC            18
#define I2S3_SD_FUNC             19
#define I2S3_WS_FUNC             20
#define I2S3_ext_SD_FUNC         21
#define I2S3ext_CK_FUNC          22
#define I2S3ext_MISO_FUNC        23
#define I2S3ext_WS_FUNC          24
#define I2S_CKIN_FUNC            25
#define RCC_MCO_1_FUNC           26
#define RCC_MCO_2_FUNC           27
#define RTC_REFIN_FUNC           28
#define SDIO_CK_FUNC             29
#define SDIO_CMD_FUNC            30
#define SDIO_D0_FUNC             31
#define SDIO_D1_FUNC             32
#define SDIO_D2_FUNC             33
#define SDIO_D3_FUNC             34
#define SDIO_D4_FUNC             35
#define SDIO_D5_FUNC             36
#define SDIO_D6_FUNC             37
#define SDIO_D7_FUNC             38
#define SPI1_MISO_FUNC           39
#define SPI1_MOSI_FUNC           40
#define SPI1_NSS_FUNC            41
#define SPI1_SCK_FUNC            42
#define SPI2_MISO_FUNC           43
#define SPI2_MOSI_FUNC           44
#define SPI2_NSS_FUNC            45
#define SPI2_SCK_FUNC            46
#define SPI3_MISO_FUNC           47
#define SPI3_MOSI_FUNC           48
#define SPI3_NSS_FUNC            49
#define SPI3_SCK_FUNC            50
#define SPI4_MISO_FUNC           51
#define SPI4_MOSI_FUNC           52
#define SPI4_NSS_FUNC            53
#define SPI4_SCK_FUNC            54
#define SYS_JTCK_SWCLK_FUNC      55
#define SYS_JTDI_FUNC            56
#define SYS_JTDO_SWO_FUNC        57
#define SYS_JTMS_SWDIO_FUNC      58
#define SYS_JTRST_FUNC           59
#define SYS_TRACECLK_FUNC        60
#define SYS_TRACED0_FUNC         61
#define SYS_TRACED1_FUNC         62
#define SYS_TRACED2_FUNC         63
#define SYS_TRACED3_FUNC         64
#define TIM10_CH1_FUNC           65
#define TIM11_CH1_FUNC           66
#define TIM1_BKIN_FUNC           67
#define TIM1_CH1_FUNC            68
#define TIM1_CH1N_FUNC           69
#define TIM1_CH2_FUNC            70
#define TIM1_CH2N_FUNC           71
#define TIM1_CH3_FUNC            72
#define TIM1_CH3N_FUNC           73
#define TIM1_CH4_FUNC            74
#define TIM1_ETR_FUNC            75
#define TIM2_CH1_FUNC            76
#define TIM2_CH2_FUNC            77
#define TIM2_CH3_FUNC            78
#define TIM2_CH4_FUNC            79
#define TIM2_ETR_FUNC            80
#define TIM3_CH1_FUNC            81
#define TIM3_CH2_FUNC            82
#define TIM3_CH3_FUNC            83
#define TIM3_CH4_FUNC            84
#define TIM3_ETR_FUNC            85
#define TIM4_CH1_FUNC            86
#define TIM4_CH2_FUNC            87
#define TIM4_CH3_FUNC            88
#define TIM4_CH4_FUNC            89
#define TIM4_ETR_FUNC            90
#define TIM5_CH1_FUNC            91
#define TIM5_CH2_FUNC            92
#define TIM5_CH3_FUNC            93
#define TIM5_CH4_FUNC            94
#define TIM9_CH1_FUNC            95
#define TIM9_CH2_FUNC            96
#define USART1_CK_FUNC           97
#define UART1_CK_FUNC            97
#define USART1_CTS_FUNC          98
#define UART1_CTS_FUNC           98
#define USART1_RTS_FUNC          99
#define UART1_RTS_FUNC           99
#define USART1_RX_FUNC           100
#define UART1_RX_FUNC            100
#define USART1_TX_FUNC           101
#define UART1_TX_FUNC            101
#define USART2_CK_FUNC           102
#define UART2_CK_FUNC            102
#define USART2_CTS_FUNC          103
#define UART2_CTS_FUNC           103
#define USART2_RTS_FUNC          104
#define UART2_RTS_FUNC           104
#define USART2_RX_FUNC           105
#define UART2_RX_FUNC            105
#define USART2_TX_FUNC           106
#define UART2_TX_FUNC            106
#define USART6_CK_FUNC           107
#define UART6_CK_FUNC            107
#define USART6_RX_FUNC           108
#define UART6_RX_FUNC            108
#define USART6_TX_FUNC           109
#define UART6_TX_FUNC            109
#define USB_OTG_FS_DM_FUNC       110
#define USB_OTG_FS_DP_FUNC       111
#define USB_OTG_FS_ID_FUNC       112
#define USB_OTG_FS_SOF_FUNC      113

# define PINMUX(gpio, func) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA00) && (func == TIM2_ETR_FUNC            )) ?  1 : \
	((gpio == PA00) && (func == TIM5_CH1_FUNC            )) ?  2 : \
	((gpio == PA00) && (func == USART2_CTS_FUNC          )) ?  7 : \
	((gpio == PA01) && (func == TIM2_CH2_FUNC            )) ?  1 : \
	((gpio == PA01) && (func == TIM5_CH2_FUNC            )) ?  2 : \
	((gpio == PA01) && (func == USART2_RTS_FUNC          )) ?  7 : \
	((gpio == PA02) && (func == TIM2_CH3_FUNC            )) ?  1 : \
	((gpio == PA02) && (func == TIM5_CH3_FUNC            )) ?  2 : \
	((gpio == PA02) && (func == TIM9_CH1_FUNC            )) ?  3 : \
	((gpio == PA02) && (func == USART2_TX_FUNC           )) ?  7 : \
	((gpio == PA03) && (func == TIM2_CH4_FUNC            )) ?  1 : \
	((gpio == PA03) && (func == TIM5_CH4_FUNC            )) ?  2 : \
	((gpio == PA03) && (func == TIM9_CH2_FUNC            )) ?  3 : \
	((gpio == PA03) && (func == USART2_RX_FUNC           )) ?  7 : \
	((gpio == PA04) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PA04) && (func == I2S3_WS_FUNC             )) ?  6 : \
	((gpio == PA04) && (func == SPI3_NSS_FUNC            )) ?  6 : \
	((gpio == PA04) && (func == USART2_CK_FUNC           )) ?  7 : \
	((gpio == PA05) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA05) && (func == TIM2_ETR_FUNC            )) ?  1 : \
	((gpio == PA05) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PA06) && (func == TIM1_BKIN_FUNC           )) ?  1 : \
	((gpio == PA06) && (func == TIM3_CH1_FUNC            )) ?  2 : \
	((gpio == PA06) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PA07) && (func == TIM1_CH1N_FUNC           )) ?  1 : \
	((gpio == PA07) && (func == TIM3_CH2_FUNC            )) ?  2 : \
	((gpio == PA07) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PA08) && (func == RCC_MCO_1_FUNC           )) ?  0 : \
	((gpio == PA08) && (func == TIM1_CH1_FUNC            )) ?  1 : \
	((gpio == PA08) && (func == I2C3_SCL_FUNC            )) ?  4 : \
	((gpio == PA08) && (func == USART1_CK_FUNC           )) ?  7 : \
	((gpio == PA08) && (func == USB_OTG_FS_SOF_FUNC      )) ? 10 : \
	((gpio == PA09) && (func == TIM1_CH2_FUNC            )) ?  1 : \
	((gpio == PA09) && (func == I2C3_SMBA_FUNC           )) ?  4 : \
	((gpio == PA09) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PA10) && (func == TIM1_CH3_FUNC            )) ?  1 : \
	((gpio == PA10) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PA10) && (func == USB_OTG_FS_ID_FUNC       )) ? 10 : \
	((gpio == PA11) && (func == TIM1_CH4_FUNC            )) ?  1 : \
	((gpio == PA11) && (func == USART1_CTS_FUNC          )) ?  7 : \
	((gpio == PA11) && (func == USART6_TX_FUNC           )) ?  8 : \
	((gpio == PA11) && (func == USB_OTG_FS_DM_FUNC       )) ? 10 : \
	((gpio == PA12) && (func == TIM1_ETR_FUNC            )) ?  1 : \
	((gpio == PA12) && (func == USART1_RTS_FUNC          )) ?  7 : \
	((gpio == PA12) && (func == USART6_RX_FUNC           )) ?  8 : \
	((gpio == PA12) && (func == USB_OTG_FS_DP_FUNC       )) ? 10 : \
	((gpio == PA13) && (func == SYS_JTMS_SWDIO_FUNC      )) ?  0 : \
	((gpio == PA14) && (func == SYS_JTCK_SWCLK_FUNC      )) ?  0 : \
	((gpio == PA14) && (func == I2S3ext_WS_FUNC          )) ?  5 : \
	((gpio == PA15) && (func == SYS_JTDI_FUNC            )) ?  0 : \
	((gpio == PA15) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA15) && (func == TIM2_ETR_FUNC            )) ?  1 : \
	((gpio == PA15) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PA15) && (func == I2S3_WS_FUNC             )) ?  6 : \
	((gpio == PA15) && (func == SPI3_NSS_FUNC            )) ?  6 : \
	((gpio == PB00) && (func == TIM1_CH2N_FUNC           )) ?  1 : \
	((gpio == PB00) && (func == TIM3_CH3_FUNC            )) ?  2 : \
	((gpio == PB01) && (func == TIM1_CH3N_FUNC           )) ?  1 : \
	((gpio == PB01) && (func == TIM3_CH4_FUNC            )) ?  2 : \
	((gpio == PB03) && (func == SYS_JTDO_SWO_FUNC        )) ?  0 : \
	((gpio == PB03) && (func == TIM2_CH2_FUNC            )) ?  1 : \
	((gpio == PB03) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PB03) && (func == I2S3_CK_FUNC             )) ?  6 : \
	((gpio == PB03) && (func == SPI3_SCK_FUNC            )) ?  6 : \
	((gpio == PB03) && (func == I2C2_SDA_FUNC            )) ?  9 : \
	((gpio == PB04) && (func == SYS_JTRST_FUNC           )) ?  0 : \
	((gpio == PB04) && (func == TIM3_CH1_FUNC            )) ?  2 : \
	((gpio == PB04) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PB04) && (func == SPI3_MISO_FUNC           )) ?  6 : \
	((gpio == PB04) && (func == I2S3_ext_SD_FUNC         )) ?  7 : \
	((gpio == PB04) && (func == I2C3_SDA_FUNC            )) ?  9 : \
	((gpio == PB05) && (func == TIM3_CH2_FUNC            )) ?  2 : \
	((gpio == PB05) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PB05) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PB05) && (func == I2S3_SD_FUNC             )) ?  6 : \
	((gpio == PB05) && (func == SPI3_MOSI_FUNC           )) ?  6 : \
	((gpio == PB06) && (func == TIM4_CH1_FUNC            )) ?  2 : \
	((gpio == PB06) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB06) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PB07) && (func == TIM4_CH2_FUNC            )) ?  2 : \
	((gpio == PB07) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB07) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PB08) && (func == TIM4_CH3_FUNC            )) ?  2 : \
	((gpio == PB08) && (func == TIM10_CH1_FUNC           )) ?  3 : \
	((gpio == PB08) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB08) && (func == SDIO_D4_FUNC             )) ? 12 : \
	((gpio == PB09) && (func == TIM4_CH4_FUNC            )) ?  2 : \
	((gpio == PB09) && (func == TIM11_CH1_FUNC           )) ?  3 : \
	((gpio == PB09) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB09) && (func == I2S2_WS_FUNC             )) ?  5 : \
	((gpio == PB09) && (func == SPI2_NSS_FUNC            )) ?  5 : \
	((gpio == PB09) && (func == SDIO_D5_FUNC             )) ? 12 : \
	((gpio == PB10) && (func == TIM2_CH3_FUNC            )) ?  1 : \
	((gpio == PB10) && (func == I2C2_SCL_FUNC            )) ?  4 : \
	((gpio == PB10) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PB10) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PB11) && (func == TIM2_CH4_FUNC            )) ?  1 : \
	((gpio == PB11) && (func == I2C2_SDA_FUNC            )) ?  4 : \
	((gpio == PB12) && (func == TIM1_BKIN_FUNC           )) ?  1 : \
	((gpio == PB12) && (func == I2C2_SMBA_FUNC           )) ?  4 : \
	((gpio == PB12) && (func == I2S2_WS_FUNC             )) ?  5 : \
	((gpio == PB12) && (func == SPI2_NSS_FUNC            )) ?  5 : \
	((gpio == PB12) && (func == I2S2ext_WS_FUNC          )) ?  6 : \
	((gpio == PB13) && (func == TIM1_CH1N_FUNC           )) ?  1 : \
	((gpio == PB13) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PB13) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PB13) && (func == I2S2ext_CK_FUNC          )) ?  6 : \
	((gpio == PB14) && (func == TIM1_CH2N_FUNC           )) ?  1 : \
	((gpio == PB14) && (func == SPI2_MISO_FUNC           )) ?  5 : \
	((gpio == PB14) && (func == I2S2_ext_SD_FUNC         )) ?  6 : \
	((gpio == PB15) && (func == RTC_REFIN_FUNC           )) ?  0 : \
	((gpio == PB15) && (func == TIM1_CH3N_FUNC           )) ?  1 : \
	((gpio == PB15) && (func == I2S2_SD_FUNC             )) ?  5 : \
	((gpio == PB15) && (func == SPI2_MOSI_FUNC           )) ?  5 : \
	((gpio == PB15) && (func == I2S2ext_MISO_FUNC        )) ?  6 : \
	((gpio == PC02) && (func == SPI2_MISO_FUNC           )) ?  5 : \
	((gpio == PC02) && (func == I2S2_ext_SD_FUNC         )) ?  6 : \
	((gpio == PC03) && (func == I2S2_SD_FUNC             )) ?  5 : \
	((gpio == PC03) && (func == SPI2_MOSI_FUNC           )) ?  5 : \
	((gpio == PC06) && (func == TIM3_CH1_FUNC            )) ?  2 : \
	((gpio == PC06) && (func == I2S2_MCK_FUNC            )) ?  5 : \
	((gpio == PC06) && (func == USART6_TX_FUNC           )) ?  8 : \
	((gpio == PC06) && (func == SDIO_D6_FUNC             )) ? 12 : \
	((gpio == PC07) && (func == TIM3_CH2_FUNC            )) ?  2 : \
	((gpio == PC07) && (func == I2S3_MCK_FUNC            )) ?  6 : \
	((gpio == PC07) && (func == USART6_RX_FUNC           )) ?  8 : \
	((gpio == PC07) && (func == SDIO_D7_FUNC             )) ? 12 : \
	((gpio == PC08) && (func == TIM3_CH3_FUNC            )) ?  2 : \
	((gpio == PC08) && (func == USART6_CK_FUNC           )) ?  8 : \
	((gpio == PC08) && (func == SDIO_D0_FUNC             )) ? 12 : \
	((gpio == PC09) && (func == RCC_MCO_2_FUNC           )) ?  0 : \
	((gpio == PC09) && (func == TIM3_CH4_FUNC            )) ?  2 : \
	((gpio == PC09) && (func == I2C3_SDA_FUNC            )) ?  4 : \
	((gpio == PC09) && (func == I2S_CKIN_FUNC            )) ?  5 : \
	((gpio == PC09) && (func == SDIO_D1_FUNC             )) ? 12 : \
	((gpio == PC10) && (func == I2S3ext_CK_FUNC          )) ?  5 : \
	((gpio == PC10) && (func == I2S3_CK_FUNC             )) ?  6 : \
	((gpio == PC10) && (func == SPI3_SCK_FUNC            )) ?  6 : \
	((gpio == PC10) && (func == SDIO_D2_FUNC             )) ? 12 : \
	((gpio == PC11) && (func == I2S3_ext_SD_FUNC         )) ?  5 : \
	((gpio == PC11) && (func == SPI3_MISO_FUNC           )) ?  6 : \
	((gpio == PC11) && (func == SDIO_D3_FUNC             )) ? 12 : \
	((gpio == PC12) && (func == I2S3ext_MISO_FUNC        )) ?  5 : \
	((gpio == PC12) && (func == I2S3_SD_FUNC             )) ?  6 : \
	((gpio == PC12) && (func == SPI3_MOSI_FUNC           )) ?  6 : \
	((gpio == PC12) && (func == SDIO_CK_FUNC             )) ? 12 : \
	((gpio == PD02) && (func == TIM3_ETR_FUNC            )) ?  2 : \
	((gpio == PD02) && (func == SDIO_CMD_FUNC            )) ? 12 : \
	((gpio == PD03) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PD03) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PD03) && (func == USART2_CTS_FUNC          )) ?  7 : \
	((gpio == PD04) && (func == USART2_RTS_FUNC          )) ?  7 : \
	((gpio == PD05) && (func == USART2_TX_FUNC           )) ?  7 : \
	((gpio == PD06) && (func == I2S3_SD_FUNC             )) ?  5 : \
	((gpio == PD06) && (func == SPI3_MOSI_FUNC           )) ?  5 : \
	((gpio == PD06) && (func == USART2_RX_FUNC           )) ?  7 : \
	((gpio == PD07) && (func == USART2_CK_FUNC           )) ?  7 : \
	((gpio == PD12) && (func == TIM4_CH1_FUNC            )) ?  2 : \
	((gpio == PD13) && (func == TIM4_CH2_FUNC            )) ?  2 : \
	((gpio == PD14) && (func == TIM4_CH3_FUNC            )) ?  2 : \
	((gpio == PD15) && (func == TIM4_CH4_FUNC            )) ?  2 : \
	((gpio == PE00) && (func == TIM4_ETR_FUNC            )) ?  2 : \
	((gpio == PE02) && (func == SYS_TRACECLK_FUNC        )) ?  0 : \
	((gpio == PE02) && (func == SPI4_SCK_FUNC            )) ?  5 : \
	((gpio == PE03) && (func == SYS_TRACED0_FUNC         )) ?  0 : \
	((gpio == PE04) && (func == SYS_TRACED1_FUNC         )) ?  0 : \
	((gpio == PE04) && (func == SPI4_NSS_FUNC            )) ?  5 : \
	((gpio == PE05) && (func == SYS_TRACED2_FUNC         )) ?  0 : \
	((gpio == PE05) && (func == TIM9_CH1_FUNC            )) ?  3 : \
	((gpio == PE05) && (func == SPI4_MISO_FUNC           )) ?  5 : \
	((gpio == PE06) && (func == SYS_TRACED3_FUNC         )) ?  0 : \
	((gpio == PE06) && (func == TIM9_CH2_FUNC            )) ?  3 : \
	((gpio == PE06) && (func == SPI4_MOSI_FUNC           )) ?  5 : \
	((gpio == PE07) && (func == TIM1_ETR_FUNC            )) ?  1 : \
	((gpio == PE08) && (func == TIM1_CH1N_FUNC           )) ?  1 : \
	((gpio == PE09) && (func == TIM1_CH1_FUNC            )) ?  1 : \
	((gpio == PE10) && (func == TIM1_CH2N_FUNC           )) ?  1 : \
	((gpio == PE11) && (func == TIM1_CH2_FUNC            )) ?  1 : \
	((gpio == PE11) && (func == SPI4_NSS_FUNC            )) ?  5 : \
	((gpio == PE12) && (func == TIM1_CH3N_FUNC           )) ?  1 : \
	((gpio == PE12) && (func == SPI4_SCK_FUNC            )) ?  5 : \
	((gpio == PE13) && (func == TIM1_CH3_FUNC            )) ?  1 : \
	((gpio == PE13) && (func == SPI4_MISO_FUNC           )) ?  5 : \
	((gpio == PE14) && (func == TIM1_CH4_FUNC            )) ?  1 : \
	((gpio == PE14) && (func == SPI4_MOSI_FUNC           )) ?  5 : \
	((gpio == PE15) && (func == TIM1_BKIN_FUNC           )) ?  1 : \
	AF_NO_SUCH_PINFUNC)

# define TIMERMUX(gpio, timer_nr, channel_nr) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA00) && (timer_nr ==  5) && (channel_nr ==  1)) ?  2 : \
	((gpio == PA01) && (timer_nr ==  2) && (channel_nr ==  2)) ?  1 : \
	((gpio == PA01) && (timer_nr ==  5) && (channel_nr ==  2)) ?  2 : \
	((gpio == PA02) && (timer_nr ==  2) && (channel_nr ==  3)) ?  1 : \
	((gpio == PA02) && (timer_nr ==  5) && (channel_nr ==  3)) ?  2 : \
	((gpio == PA02) && (timer_nr ==  9) && (channel_nr ==  1)) ?  3 : \
	((gpio == PA03) && (timer_nr ==  2) && (channel_nr ==  4)) ?  1 : \
	((gpio == PA03) && (timer_nr ==  5) && (channel_nr ==  4)) ?  2 : \
	((gpio == PA03) && (timer_nr ==  9) && (channel_nr ==  2)) ?  3 : \
	((gpio == PA05) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA06) && (timer_nr ==  3) && (channel_nr ==  1)) ?  2 : \
	((gpio == PA07) && (timer_nr ==  1) && (channel_nr == -1)) ?  1 : \
	((gpio == PA07) && (timer_nr ==  3) && (channel_nr ==  2)) ?  2 : \
	((gpio == PA08) && (timer_nr ==  1) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA09) && (timer_nr ==  1) && (channel_nr ==  2)) ?  1 : \
	((gpio == PA10) && (timer_nr ==  1) && (channel_nr ==  3)) ?  1 : \
	((gpio == PA11) && (timer_nr ==  1) && (channel_nr ==  4)) ?  1 : \
	((gpio == PA15) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PB00) && (timer_nr ==  1) && (channel_nr == -2)) ?  1 : \
	((gpio == PB00) && (timer_nr ==  3) && (channel_nr ==  3)) ?  2 : \
	((gpio == PB01) && (timer_nr ==  1) && (channel_nr == -3)) ?  1 : \
	((gpio == PB01) && (timer_nr ==  3) && (channel_nr ==  4)) ?  2 : \
	((gpio == PB03) && (timer_nr ==  2) && (channel_nr ==  2)) ?  1 : \
	((gpio == PB04) && (timer_nr ==  3) && (channel_nr ==  1)) ?  2 : \
	((gpio == PB05) && (timer_nr ==  3) && (channel_nr ==  2)) ?  2 : \
	((gpio == PB06) && (timer_nr ==  4) && (channel_nr ==  1)) ?  2 : \
	((gpio == PB07) && (timer_nr ==  4) && (channel_nr ==  2)) ?  2 : \
	((gpio == PB08) && (timer_nr ==  4) && (channel_nr ==  3)) ?  2 : \
	((gpio == PB08) && (timer_nr == 10) && (channel_nr ==  1)) ?  3 : \
	((gpio == PB09) && (timer_nr ==  4) && (channel_nr ==  4)) ?  2 : \
	((gpio == PB09) && (timer_nr == 11) && (channel_nr ==  1)) ?  3 : \
	((gpio == PB10) && (timer_nr ==  2) && (channel_nr ==  3)) ?  1 : \
	((gpio == PB11) && (timer_nr ==  2) && (channel_nr ==  4)) ?  1 : \
	((gpio == PB13) && (timer_nr ==  1) && (channel_nr == -1)) ?  1 : \
	((gpio == PB14) && (timer_nr ==  1) && (channel_nr == -2)) ?  1 : \
	((gpio == PB15) && (timer_nr ==  1) && (channel_nr == -3)) ?  1 : \
	((gpio == PC06) && (timer_nr ==  3) && (channel_nr ==  1)) ?  2 : \
	((gpio == PC07) && (timer_nr ==  3) && (channel_nr ==  2)) ?  2 : \
	((gpio == PC08) && (timer_nr ==  3) && (channel_nr ==  3)) ?  2 : \
	((gpio == PC09) && (timer_nr ==  3) && (channel_nr ==  4)) ?  2 : \
	((gpio == PD12) && (timer_nr ==  4) && (channel_nr ==  1)) ?  2 : \
	((gpio == PD13) && (timer_nr ==  4) && (channel_nr ==  2)) ?  2 : \
	((gpio == PD14) && (timer_nr ==  4) && (channel_nr ==  3)) ?  2 : \
	((gpio == PD15) && (timer_nr ==  4) && (channel_nr ==  4)) ?  2 : \
	((gpio == PE05) && (timer_nr ==  9) && (channel_nr ==  1)) ?  3 : \
	((gpio == PE06) && (timer_nr ==  9) && (channel_nr ==  2)) ?  3 : \
	((gpio == PE08) && (timer_nr ==  1) && (channel_nr == -1)) ?  1 : \
	((gpio == PE09) && (timer_nr ==  1) && (channel_nr ==  1)) ?  1 : \
	((gpio == PE10) && (timer_nr ==  1) && (channel_nr == -2)) ?  1 : \
	((gpio == PE11) && (timer_nr ==  1) && (channel_nr ==  2)) ?  1 : \
	((gpio == PE12) && (timer_nr ==  1) && (channel_nr == -3)) ?  1 : \
	((gpio == PE13) && (timer_nr ==  1) && (channel_nr ==  3)) ?  1 : \
	((gpio == PE14) && (timer_nr ==  1) && (channel_nr ==  4)) ?  1 : \
	AF_NO_SUCH_PINFUNC)
#endif
