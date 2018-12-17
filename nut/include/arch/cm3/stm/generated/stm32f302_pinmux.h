/* Do not edit!
 * Generated from /devel/cubemx/db/mcu/IPGPIO-STM32F302_gpio_v1_0_Modes.xml*/

#if !defined(_STM32F302_PINMUX_H_)
# define _STM32F302_PINMUX_H_

#define CAN_RX_FUNC              0
#define CAN1_RX_FUNC             0
#define CAN_TX_FUNC              1
#define CAN1_TX_FUNC             1
#define COMP2_OUT_FUNC           2
#define COMP4_OUT_FUNC           3
#define COMP6_OUT_FUNC           4
#define EVENTOUT_FUNC            5
#define I2C1_SCL_FUNC            6
#define I2C1_SDA_FUNC            7
#define I2C1_SMBA_FUNC           8
#define I2C2_SCL_FUNC            9
#define I2C2_SDA_FUNC            10
#define I2C2_SMBA_FUNC           11
#define I2C3_SCL_FUNC            12
#define I2C3_SDA_FUNC            13
#define I2C3_SMBA_FUNC           14
#define I2S2_CK_FUNC             15
#define I2S2_MCK_FUNC            16
#define I2S2_SD_FUNC             17
#define I2S2_WS_FUNC             18
#define I2S2_ext_SD_FUNC         19
#define I2S3_CK_FUNC             20
#define I2S3_MCK_FUNC            21
#define I2S3_SD_FUNC             22
#define I2S3_WS_FUNC             23
#define I2S3_ext_SD_FUNC         24
#define I2S_CKIN_FUNC            25
#define IR_OUT_FUNC              26
#define RCC_MCO_FUNC             27
#define RTC_REFIN_FUNC           28
#define SPI2_MISO_FUNC           29
#define SPI2_MOSI_FUNC           30
#define SPI2_NSS_FUNC            31
#define SPI2_SCK_FUNC            32
#define SPI3_MISO_FUNC           33
#define SPI3_MOSI_FUNC           34
#define SPI3_NSS_FUNC            35
#define SPI3_SCK_FUNC            36
#define SYS_JTCK_SWCLK_FUNC      37
#define SYS_JTDI_FUNC            38
#define SYS_JTDO_TRACESWO_FUNC   39
#define SYS_JTMS_SWDIO_FUNC      40
#define SYS_NJTRST_FUNC          41
#define TIM15_BKIN_FUNC          42
#define TIM15_CH1_FUNC           43
#define TIM15_CH1N_FUNC          44
#define TIM15_CH2_FUNC           45
#define TIM16_BKIN_FUNC          46
#define TIM16_CH1_FUNC           47
#define TIM16_CH1N_FUNC          48
#define TIM17_BKIN_FUNC          49
#define TIM17_CH1_FUNC           50
#define TIM17_CH1N_FUNC          51
#define TIM1_BKIN_FUNC           52
#define TIM1_BKIN2_FUNC          53
#define TIM1_CH1_FUNC            54
#define TIM1_CH1N_FUNC           55
#define TIM1_CH2_FUNC            56
#define TIM1_CH2N_FUNC           57
#define TIM1_CH3_FUNC            58
#define TIM1_CH3N_FUNC           59
#define TIM1_CH4_FUNC            60
#define TIM1_ETR_FUNC            61
#define TIM2_CH1_FUNC            62
#define TIM2_CH2_FUNC            63
#define TIM2_CH3_FUNC            64
#define TIM2_CH4_FUNC            65
#define TIM2_ETR_FUNC            66
#define TSC_G1_IO1_FUNC          67
#define TSC_G1_IO2_FUNC          68
#define TSC_G1_IO3_FUNC          69
#define TSC_G1_IO4_FUNC          70
#define TSC_G2_IO1_FUNC          71
#define TSC_G2_IO2_FUNC          72
#define TSC_G2_IO3_FUNC          73
#define TSC_G2_IO4_FUNC          74
#define TSC_G3_IO1_FUNC          75
#define TSC_G3_IO2_FUNC          76
#define TSC_G3_IO3_FUNC          77
#define TSC_G3_IO4_FUNC          78
#define TSC_G4_IO1_FUNC          79
#define TSC_G4_IO2_FUNC          80
#define TSC_G4_IO3_FUNC          81
#define TSC_G4_IO4_FUNC          82
#define TSC_G5_IO1_FUNC          83
#define TSC_G5_IO2_FUNC          84
#define TSC_G5_IO3_FUNC          85
#define TSC_G5_IO4_FUNC          86
#define TSC_G6_IO1_FUNC          87
#define TSC_G6_IO2_FUNC          88
#define TSC_G6_IO3_FUNC          89
#define TSC_G6_IO4_FUNC          90
#define TSC_SYNC_FUNC            91
#define USART1_CK_FUNC           92
#define UART1_CK_FUNC            92
#define USART1_CTS_FUNC          93
#define UART1_CTS_FUNC           93
#define USART1_DE_FUNC           94
#define UART1_DE_FUNC            94
#define USART1_RTS_FUNC          95
#define UART1_RTS_FUNC           95
#define USART1_RX_FUNC           96
#define UART1_RX_FUNC            96
#define USART1_TX_FUNC           97
#define UART1_TX_FUNC            97
#define USART2_CK_FUNC           98
#define UART2_CK_FUNC            98
#define USART2_CTS_FUNC          99
#define UART2_CTS_FUNC           99
#define USART2_DE_FUNC           100
#define UART2_DE_FUNC            100
#define USART2_RTS_FUNC          101
#define UART2_RTS_FUNC           101
#define USART2_RX_FUNC           102
#define UART2_RX_FUNC            102
#define USART2_TX_FUNC           103
#define UART2_TX_FUNC            103
#define USART3_CK_FUNC           104
#define UART3_CK_FUNC            104
#define USART3_CTS_FUNC          105
#define UART3_CTS_FUNC           105
#define USART3_DE_FUNC           106
#define UART3_DE_FUNC            106
#define USART3_RTS_FUNC          107
#define UART3_RTS_FUNC           107
#define USART3_RX_FUNC           108
#define UART3_RX_FUNC            108
#define USART3_TX_FUNC           109
#define UART3_TX_FUNC            109

# define PINMUX(gpio, func) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA00) && (func == TIM2_ETR_FUNC            )) ?  1 : \
	((gpio == PA00) && (func == TSC_G1_IO1_FUNC          )) ?  3 : \
	((gpio == PA00) && (func == USART2_CTS_FUNC          )) ?  7 : \
	((gpio == PA00) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA01) && (func == RTC_REFIN_FUNC           )) ?  0 : \
	((gpio == PA01) && (func == TIM2_CH2_FUNC            )) ?  1 : \
	((gpio == PA01) && (func == TSC_G1_IO2_FUNC          )) ?  3 : \
	((gpio == PA01) && (func == USART2_DE_FUNC           )) ?  7 : \
	((gpio == PA01) && (func == USART2_RTS_FUNC          )) ?  7 : \
	((gpio == PA01) && (func == TIM15_CH1N_FUNC          )) ?  9 : \
	((gpio == PA01) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA02) && (func == TIM2_CH3_FUNC            )) ?  1 : \
	((gpio == PA02) && (func == TSC_G1_IO3_FUNC          )) ?  3 : \
	((gpio == PA02) && (func == USART2_TX_FUNC           )) ?  7 : \
	((gpio == PA02) && (func == COMP2_OUT_FUNC           )) ?  8 : \
	((gpio == PA02) && (func == TIM15_CH1_FUNC           )) ?  9 : \
	((gpio == PA02) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA03) && (func == TIM2_CH4_FUNC            )) ?  1 : \
	((gpio == PA03) && (func == TSC_G1_IO4_FUNC          )) ?  3 : \
	((gpio == PA03) && (func == USART2_RX_FUNC           )) ?  7 : \
	((gpio == PA03) && (func == TIM15_CH2_FUNC           )) ?  9 : \
	((gpio == PA03) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA04) && (func == TSC_G2_IO1_FUNC          )) ?  3 : \
	((gpio == PA04) && (func == I2S3_WS_FUNC             )) ?  6 : \
	((gpio == PA04) && (func == SPI3_NSS_FUNC            )) ?  6 : \
	((gpio == PA04) && (func == USART2_CK_FUNC           )) ?  7 : \
	((gpio == PA04) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA05) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA05) && (func == TIM2_ETR_FUNC            )) ?  1 : \
	((gpio == PA05) && (func == TSC_G2_IO2_FUNC          )) ?  3 : \
	((gpio == PA05) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA06) && (func == TIM16_CH1_FUNC           )) ?  1 : \
	((gpio == PA06) && (func == TSC_G2_IO3_FUNC          )) ?  3 : \
	((gpio == PA06) && (func == TIM1_BKIN_FUNC           )) ?  6 : \
	((gpio == PA06) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA07) && (func == TIM17_CH1_FUNC           )) ?  1 : \
	((gpio == PA07) && (func == TSC_G2_IO4_FUNC          )) ?  3 : \
	((gpio == PA07) && (func == TIM1_CH1N_FUNC           )) ?  6 : \
	((gpio == PA07) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA08) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PA08) && (func == I2C3_SCL_FUNC            )) ?  3 : \
	((gpio == PA08) && (func == I2C2_SMBA_FUNC           )) ?  4 : \
	((gpio == PA08) && (func == I2S2_MCK_FUNC            )) ?  5 : \
	((gpio == PA08) && (func == TIM1_CH1_FUNC            )) ?  6 : \
	((gpio == PA08) && (func == USART1_CK_FUNC           )) ?  7 : \
	((gpio == PA08) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA09) && (func == I2C3_SMBA_FUNC           )) ?  2 : \
	((gpio == PA09) && (func == TSC_G4_IO1_FUNC          )) ?  3 : \
	((gpio == PA09) && (func == I2C2_SCL_FUNC            )) ?  4 : \
	((gpio == PA09) && (func == I2S3_MCK_FUNC            )) ?  5 : \
	((gpio == PA09) && (func == TIM1_CH2_FUNC            )) ?  6 : \
	((gpio == PA09) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PA09) && (func == TIM15_BKIN_FUNC          )) ?  9 : \
	((gpio == PA09) && (func == TIM2_CH3_FUNC            )) ? 10 : \
	((gpio == PA09) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA10) && (func == TIM17_BKIN_FUNC          )) ?  1 : \
	((gpio == PA10) && (func == TSC_G4_IO2_FUNC          )) ?  3 : \
	((gpio == PA10) && (func == I2C2_SDA_FUNC            )) ?  4 : \
	((gpio == PA10) && (func == I2S2_ext_SD_FUNC         )) ?  5 : \
	((gpio == PA10) && (func == SPI2_MISO_FUNC           )) ?  5 : \
	((gpio == PA10) && (func == TIM1_CH3_FUNC            )) ?  6 : \
	((gpio == PA10) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PA10) && (func == COMP6_OUT_FUNC           )) ?  8 : \
	((gpio == PA10) && (func == TIM2_CH4_FUNC            )) ? 10 : \
	((gpio == PA10) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA11) && (func == I2S2_SD_FUNC             )) ?  5 : \
	((gpio == PA11) && (func == SPI2_MOSI_FUNC           )) ?  5 : \
	((gpio == PA11) && (func == TIM1_CH1N_FUNC           )) ?  6 : \
	((gpio == PA11) && (func == USART1_CTS_FUNC          )) ?  7 : \
	((gpio == PA11) && (func == CAN_RX_FUNC              )) ?  9 : \
	((gpio == PA11) && (func == TIM1_CH4_FUNC            )) ? 11 : \
	((gpio == PA11) && (func == TIM1_BKIN2_FUNC          )) ? 12 : \
	((gpio == PA11) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA12) && (func == TIM16_CH1_FUNC           )) ?  1 : \
	((gpio == PA12) && (func == I2S_CKIN_FUNC            )) ?  5 : \
	((gpio == PA12) && (func == TIM1_CH2N_FUNC           )) ?  6 : \
	((gpio == PA12) && (func == USART1_DE_FUNC           )) ?  7 : \
	((gpio == PA12) && (func == USART1_RTS_FUNC          )) ?  7 : \
	((gpio == PA12) && (func == COMP2_OUT_FUNC           )) ?  8 : \
	((gpio == PA12) && (func == CAN_TX_FUNC              )) ?  9 : \
	((gpio == PA12) && (func == TIM1_ETR_FUNC            )) ? 11 : \
	((gpio == PA12) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA13) && (func == SYS_JTMS_SWDIO_FUNC      )) ?  0 : \
	((gpio == PA13) && (func == TIM16_CH1N_FUNC          )) ?  1 : \
	((gpio == PA13) && (func == TSC_G4_IO3_FUNC          )) ?  3 : \
	((gpio == PA13) && (func == IR_OUT_FUNC              )) ?  5 : \
	((gpio == PA13) && (func == USART3_CTS_FUNC          )) ?  7 : \
	((gpio == PA13) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA14) && (func == SYS_JTCK_SWCLK_FUNC      )) ?  0 : \
	((gpio == PA14) && (func == TSC_G4_IO4_FUNC          )) ?  3 : \
	((gpio == PA14) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PA14) && (func == TIM1_BKIN_FUNC           )) ?  6 : \
	((gpio == PA14) && (func == USART2_TX_FUNC           )) ?  7 : \
	((gpio == PA14) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA15) && (func == SYS_JTDI_FUNC            )) ?  0 : \
	((gpio == PA15) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA15) && (func == TIM2_ETR_FUNC            )) ?  1 : \
	((gpio == PA15) && (func == TSC_SYNC_FUNC            )) ?  3 : \
	((gpio == PA15) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PA15) && (func == I2S3_WS_FUNC             )) ?  6 : \
	((gpio == PA15) && (func == SPI3_NSS_FUNC            )) ?  6 : \
	((gpio == PA15) && (func == USART2_RX_FUNC           )) ?  7 : \
	((gpio == PA15) && (func == TIM1_BKIN_FUNC           )) ?  9 : \
	((gpio == PA15) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB00) && (func == TSC_G3_IO2_FUNC          )) ?  3 : \
	((gpio == PB00) && (func == TIM1_CH2N_FUNC           )) ?  6 : \
	((gpio == PB00) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB01) && (func == TSC_G3_IO3_FUNC          )) ?  3 : \
	((gpio == PB01) && (func == TIM1_CH3N_FUNC           )) ?  6 : \
	((gpio == PB01) && (func == COMP4_OUT_FUNC           )) ?  8 : \
	((gpio == PB01) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB02) && (func == TSC_G3_IO4_FUNC          )) ?  3 : \
	((gpio == PB02) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB03) && (func == SYS_JTDO_TRACESWO_FUNC   )) ?  0 : \
	((gpio == PB03) && (func == TIM2_CH2_FUNC            )) ?  1 : \
	((gpio == PB03) && (func == TSC_G5_IO1_FUNC          )) ?  3 : \
	((gpio == PB03) && (func == I2S3_CK_FUNC             )) ?  6 : \
	((gpio == PB03) && (func == SPI3_SCK_FUNC            )) ?  6 : \
	((gpio == PB03) && (func == USART2_TX_FUNC           )) ?  7 : \
	((gpio == PB03) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB04) && (func == SYS_NJTRST_FUNC          )) ?  0 : \
	((gpio == PB04) && (func == TIM16_CH1_FUNC           )) ?  1 : \
	((gpio == PB04) && (func == TSC_G5_IO2_FUNC          )) ?  3 : \
	((gpio == PB04) && (func == I2S3_ext_SD_FUNC         )) ?  6 : \
	((gpio == PB04) && (func == SPI3_MISO_FUNC           )) ?  6 : \
	((gpio == PB04) && (func == USART2_RX_FUNC           )) ?  7 : \
	((gpio == PB04) && (func == TIM17_BKIN_FUNC          )) ? 10 : \
	((gpio == PB04) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB05) && (func == TIM16_BKIN_FUNC          )) ?  1 : \
	((gpio == PB05) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PB05) && (func == I2S3_SD_FUNC             )) ?  6 : \
	((gpio == PB05) && (func == SPI3_MOSI_FUNC           )) ?  6 : \
	((gpio == PB05) && (func == USART2_CK_FUNC           )) ?  7 : \
	((gpio == PB05) && (func == I2C3_SDA_FUNC            )) ?  8 : \
	((gpio == PB05) && (func == TIM17_CH1_FUNC           )) ? 10 : \
	((gpio == PB05) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB06) && (func == TIM16_CH1N_FUNC          )) ?  1 : \
	((gpio == PB06) && (func == TSC_G5_IO3_FUNC          )) ?  3 : \
	((gpio == PB06) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB06) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PB06) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB07) && (func == TIM17_CH1N_FUNC          )) ?  1 : \
	((gpio == PB07) && (func == TSC_G5_IO4_FUNC          )) ?  3 : \
	((gpio == PB07) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB07) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PB07) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB08) && (func == TIM16_CH1_FUNC           )) ?  1 : \
	((gpio == PB08) && (func == TSC_SYNC_FUNC            )) ?  3 : \
	((gpio == PB08) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB08) && (func == USART3_RX_FUNC           )) ?  7 : \
	((gpio == PB08) && (func == CAN_RX_FUNC              )) ?  9 : \
	((gpio == PB08) && (func == TIM1_BKIN_FUNC           )) ? 12 : \
	((gpio == PB08) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB09) && (func == TIM17_CH1_FUNC           )) ?  1 : \
	((gpio == PB09) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB09) && (func == IR_OUT_FUNC              )) ?  6 : \
	((gpio == PB09) && (func == USART3_TX_FUNC           )) ?  7 : \
	((gpio == PB09) && (func == COMP2_OUT_FUNC           )) ?  8 : \
	((gpio == PB09) && (func == CAN_TX_FUNC              )) ?  9 : \
	((gpio == PB09) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB10) && (func == TIM2_CH3_FUNC            )) ?  1 : \
	((gpio == PB10) && (func == TSC_SYNC_FUNC            )) ?  3 : \
	((gpio == PB10) && (func == USART3_TX_FUNC           )) ?  7 : \
	((gpio == PB10) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB11) && (func == TIM2_CH4_FUNC            )) ?  1 : \
	((gpio == PB11) && (func == TSC_G6_IO1_FUNC          )) ?  3 : \
	((gpio == PB11) && (func == USART3_RX_FUNC           )) ?  7 : \
	((gpio == PB11) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB12) && (func == TSC_G6_IO2_FUNC          )) ?  3 : \
	((gpio == PB12) && (func == I2C2_SMBA_FUNC           )) ?  4 : \
	((gpio == PB12) && (func == I2S2_WS_FUNC             )) ?  5 : \
	((gpio == PB12) && (func == SPI2_NSS_FUNC            )) ?  5 : \
	((gpio == PB12) && (func == TIM1_BKIN_FUNC           )) ?  6 : \
	((gpio == PB12) && (func == USART3_CK_FUNC           )) ?  7 : \
	((gpio == PB12) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB13) && (func == TSC_G6_IO3_FUNC          )) ?  3 : \
	((gpio == PB13) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PB13) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PB13) && (func == TIM1_CH1N_FUNC           )) ?  6 : \
	((gpio == PB13) && (func == USART3_CTS_FUNC          )) ?  7 : \
	((gpio == PB13) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB14) && (func == TIM15_CH1_FUNC           )) ?  1 : \
	((gpio == PB14) && (func == TSC_G6_IO4_FUNC          )) ?  3 : \
	((gpio == PB14) && (func == I2S2_ext_SD_FUNC         )) ?  5 : \
	((gpio == PB14) && (func == SPI2_MISO_FUNC           )) ?  5 : \
	((gpio == PB14) && (func == TIM1_CH2N_FUNC           )) ?  6 : \
	((gpio == PB14) && (func == USART3_DE_FUNC           )) ?  7 : \
	((gpio == PB14) && (func == USART3_RTS_FUNC          )) ?  7 : \
	((gpio == PB14) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB15) && (func == RTC_REFIN_FUNC           )) ?  0 : \
	((gpio == PB15) && (func == TIM15_CH2_FUNC           )) ?  1 : \
	((gpio == PB15) && (func == TIM15_CH1N_FUNC          )) ?  2 : \
	((gpio == PB15) && (func == TIM1_CH3N_FUNC           )) ?  4 : \
	((gpio == PB15) && (func == I2S2_SD_FUNC             )) ?  5 : \
	((gpio == PB15) && (func == SPI2_MOSI_FUNC           )) ?  5 : \
	((gpio == PB15) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC00) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC00) && (func == TIM1_CH1_FUNC            )) ?  2 : \
	((gpio == PC01) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC01) && (func == TIM1_CH2_FUNC            )) ?  2 : \
	((gpio == PC02) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC02) && (func == TIM1_CH3_FUNC            )) ?  2 : \
	((gpio == PC03) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC03) && (func == TIM1_CH4_FUNC            )) ?  2 : \
	((gpio == PC03) && (func == TIM1_BKIN2_FUNC          )) ?  6 : \
	((gpio == PC04) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC04) && (func == TIM1_ETR_FUNC            )) ?  2 : \
	((gpio == PC04) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PC05) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC05) && (func == TIM15_BKIN_FUNC          )) ?  2 : \
	((gpio == PC05) && (func == TSC_G3_IO1_FUNC          )) ?  3 : \
	((gpio == PC05) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PC06) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC06) && (func == I2S2_MCK_FUNC            )) ?  6 : \
	((gpio == PC06) && (func == COMP6_OUT_FUNC           )) ?  7 : \
	((gpio == PC07) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC07) && (func == I2S3_MCK_FUNC            )) ?  6 : \
	((gpio == PC08) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC09) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC09) && (func == I2C3_SDA_FUNC            )) ?  3 : \
	((gpio == PC09) && (func == I2S_CKIN_FUNC            )) ?  5 : \
	((gpio == PC10) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC10) && (func == I2S3_CK_FUNC             )) ?  6 : \
	((gpio == PC10) && (func == SPI3_SCK_FUNC            )) ?  6 : \
	((gpio == PC10) && (func == USART3_TX_FUNC           )) ?  7 : \
	((gpio == PC11) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC11) && (func == I2S3_ext_SD_FUNC         )) ?  6 : \
	((gpio == PC11) && (func == SPI3_MISO_FUNC           )) ?  6 : \
	((gpio == PC11) && (func == USART3_RX_FUNC           )) ?  7 : \
	((gpio == PC12) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PC12) && (func == I2S3_SD_FUNC             )) ?  6 : \
	((gpio == PC12) && (func == SPI3_MOSI_FUNC           )) ?  6 : \
	((gpio == PC12) && (func == USART3_CK_FUNC           )) ?  7 : \
	((gpio == PC13) && (func == TIM1_CH1N_FUNC           )) ?  4 : \
	((gpio == PD02) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PF00) && (func == I2C2_SDA_FUNC            )) ?  4 : \
	((gpio == PF00) && (func == I2S2_WS_FUNC             )) ?  5 : \
	((gpio == PF00) && (func == SPI2_NSS_FUNC            )) ?  5 : \
	((gpio == PF00) && (func == TIM1_CH3N_FUNC           )) ?  6 : \
	((gpio == PF01) && (func == I2C2_SCL_FUNC            )) ?  4 : \
	((gpio == PF01) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PF01) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	AF_NO_SUCH_PINFUNC)

# define TIMERMUX(gpio, timer_nr, channel_nr) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA01) && (timer_nr ==  2) && (channel_nr ==  2)) ?  1 : \
	((gpio == PA01) && (timer_nr == 15) && (channel_nr == -1)) ?  9 : \
	((gpio == PA02) && (timer_nr ==  2) && (channel_nr ==  3)) ?  1 : \
	((gpio == PA02) && (timer_nr == 15) && (channel_nr ==  1)) ?  9 : \
	((gpio == PA03) && (timer_nr ==  2) && (channel_nr ==  4)) ?  1 : \
	((gpio == PA03) && (timer_nr == 15) && (channel_nr ==  2)) ?  9 : \
	((gpio == PA05) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA06) && (timer_nr == 16) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA07) && (timer_nr == 17) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA07) && (timer_nr ==  1) && (channel_nr == -1)) ?  6 : \
	((gpio == PA08) && (timer_nr ==  1) && (channel_nr ==  1)) ?  6 : \
	((gpio == PA09) && (timer_nr ==  1) && (channel_nr ==  2)) ?  6 : \
	((gpio == PA09) && (timer_nr ==  2) && (channel_nr ==  3)) ? 10 : \
	((gpio == PA10) && (timer_nr ==  1) && (channel_nr ==  3)) ?  6 : \
	((gpio == PA10) && (timer_nr ==  2) && (channel_nr ==  4)) ? 10 : \
	((gpio == PA11) && (timer_nr ==  1) && (channel_nr == -1)) ?  6 : \
	((gpio == PA11) && (timer_nr ==  1) && (channel_nr ==  4)) ? 11 : \
	((gpio == PA12) && (timer_nr == 16) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA12) && (timer_nr ==  1) && (channel_nr == -2)) ?  6 : \
	((gpio == PA13) && (timer_nr == 16) && (channel_nr == -1)) ?  1 : \
	((gpio == PA15) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PB00) && (timer_nr ==  1) && (channel_nr == -2)) ?  6 : \
	((gpio == PB01) && (timer_nr ==  1) && (channel_nr == -3)) ?  6 : \
	((gpio == PB03) && (timer_nr ==  2) && (channel_nr ==  2)) ?  1 : \
	((gpio == PB04) && (timer_nr == 16) && (channel_nr ==  1)) ?  1 : \
	((gpio == PB05) && (timer_nr == 17) && (channel_nr ==  1)) ? 10 : \
	((gpio == PB06) && (timer_nr == 16) && (channel_nr == -1)) ?  1 : \
	((gpio == PB07) && (timer_nr == 17) && (channel_nr == -1)) ?  1 : \
	((gpio == PB08) && (timer_nr == 16) && (channel_nr ==  1)) ?  1 : \
	((gpio == PB09) && (timer_nr == 17) && (channel_nr ==  1)) ?  1 : \
	((gpio == PB10) && (timer_nr ==  2) && (channel_nr ==  3)) ?  1 : \
	((gpio == PB11) && (timer_nr ==  2) && (channel_nr ==  4)) ?  1 : \
	((gpio == PB13) && (timer_nr ==  1) && (channel_nr == -1)) ?  6 : \
	((gpio == PB14) && (timer_nr == 15) && (channel_nr ==  1)) ?  1 : \
	((gpio == PB14) && (timer_nr ==  1) && (channel_nr == -2)) ?  6 : \
	((gpio == PB15) && (timer_nr == 15) && (channel_nr ==  2)) ?  1 : \
	((gpio == PB15) && (timer_nr == 15) && (channel_nr == -1)) ?  2 : \
	((gpio == PB15) && (timer_nr ==  1) && (channel_nr == -3)) ?  4 : \
	((gpio == PC00) && (timer_nr ==  1) && (channel_nr ==  1)) ?  2 : \
	((gpio == PC01) && (timer_nr ==  1) && (channel_nr ==  2)) ?  2 : \
	((gpio == PC02) && (timer_nr ==  1) && (channel_nr ==  3)) ?  2 : \
	((gpio == PC03) && (timer_nr ==  1) && (channel_nr ==  4)) ?  2 : \
	((gpio == PC13) && (timer_nr ==  1) && (channel_nr == -1)) ?  4 : \
	((gpio == PF00) && (timer_nr ==  1) && (channel_nr == -3)) ?  6 : \
	AF_NO_SUCH_PINFUNC)
#endif
