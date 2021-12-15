/* Do not edit!
 * Generated from /devel/cubemx/db/mcu/IPGPIO-STM32WL_gpio_v1_0_Modes.xml*/

#if !defined(_STM32WL_PINMUX_H_)
# define _STM32WL_PINMUX_H_

#define CM4_EVENTOUT_FUNC        0
#define COMP1_OUT_FUNC           1
#define COMP2_OUT_FUNC           2
#define DEBUG_JTCK_SWCLK_FUNC    3
#define DEBUG_JTDI_FUNC          4
#define DEBUG_JTDO_SWO_FUNC      5
#define DEBUG_JTMS_SWDIO_FUNC    6
#define DEBUG_PWR_LDORDY_FUNC    7
#define DEBUG_PWR_REGLP1S_FUNC   8
#define DEBUG_PWR_REGLP2S_FUNC   9
#define DEBUG_RF_BUSY_FUNC       10
#define DEBUG_RF_DTB1_FUNC       11
#define DEBUG_RF_HSE32RDY_FUNC   12
#define DEBUG_RF_LDORDY_FUNC     13
#define DEBUG_RF_NRESET_FUNC     14
#define DEBUG_RF_SMPSRDY_FUNC    15
#define DEBUG_SUBGHZSPI_MISOOUT_FUNC16
#define DEBUG_SUBGHZSPI_MOSIOUT_FUNC17
#define DEBUG_SUBGHZSPI_NSSOUT_FUNC18
#define DEBUG_SUBGHZSPI_SCKOUT_FUNC19
#define I2C1_SCL_FUNC            20
#define I2C1_SDA_FUNC            21
#define I2C1_SMBA_FUNC           22
#define I2C2_SCL_FUNC            23
#define I2C2_SDA_FUNC            24
#define I2C2_SMBA_FUNC           25
#define I2C3_SCL_FUNC            26
#define I2C3_SDA_FUNC            27
#define I2C3_SMBA_FUNC           28
#define I2S2_CK_FUNC             29
#define I2S2_MCK_FUNC            30
#define I2S2_SD_FUNC             31
#define I2S2_WS_FUNC             32
#define I2S_CKIN_FUNC            33
#define IR_OUT_FUNC              34
#define LPTIM1_ETR_FUNC          35
#define LPTIM1_IN1_FUNC          36
#define LPTIM1_IN2_FUNC          37
#define LPTIM1_OUT_FUNC          38
#define LPTIM2_ETR_FUNC          39
#define LPTIM2_IN1_FUNC          40
#define LPTIM2_OUT_FUNC          41
#define LPTIM3_ETR_FUNC          42
#define LPTIM3_IN1_FUNC          43
#define LPTIM3_OUT_FUNC          44
#define LPUART1_CTS_FUNC         45
#define LPUART1_DE_FUNC          46
#define LPUART1_RTS_FUNC         47
#define LPUART1_RX_FUNC          48
#define LPUART1_TX_FUNC          49
#define RCC_LSCO_FUNC            50
#define RCC_MCO_FUNC             51
#define RF_IRQ0_FUNC             52
#define RF_IRQ1_FUNC             53
#define RF_IRQ2_FUNC             54
#define RTC_REFIN_FUNC           55
#define SPI1_MISO_FUNC           56
#define SPI1_MOSI_FUNC           57
#define SPI1_NSS_FUNC            58
#define SPI1_SCK_FUNC            59
#define SPI2_MISO_FUNC           60
#define SPI2_MOSI_FUNC           61
#define SPI2_NSS_FUNC            62
#define SPI2_SCK_FUNC            63
#define SYS_JTRST_FUNC           64
#define TIM16_BKIN_FUNC          65
#define TIM16_CH1_FUNC           66
#define TIM16_CH1N_FUNC          67
#define TIM17_BKIN_FUNC          68
#define TIM17_CH1_FUNC           69
#define TIM17_CH1N_FUNC          70
#define TIM1_BKIN_FUNC           71
#define TIM1_BKIN2_FUNC          72
#define TIM1_CH1_FUNC            73
#define TIM1_CH1N_FUNC           74
#define TIM1_CH2_FUNC            75
#define TIM1_CH2N_FUNC           76
#define TIM1_CH3_FUNC            77
#define TIM1_CH3N_FUNC           78
#define TIM1_CH4_FUNC            79
#define TIM1_ETR_FUNC            80
#define TIM2_CH1_FUNC            81
#define TIM2_CH2_FUNC            82
#define TIM2_CH3_FUNC            83
#define TIM2_CH4_FUNC            84
#define TIM2_ETR_FUNC            85
#define USART1_CK_FUNC           86
#define UART1_CK_FUNC            86
#define USART1_CTS_FUNC          87
#define UART1_CTS_FUNC           87
#define USART1_DE_FUNC           88
#define UART1_DE_FUNC            88
#define USART1_NSS_FUNC          89
#define UART1_NSS_FUNC           89
#define USART1_RTS_FUNC          90
#define UART1_RTS_FUNC           90
#define USART1_RX_FUNC           91
#define UART1_RX_FUNC            91
#define USART1_TX_FUNC           92
#define UART1_TX_FUNC            92
#define USART2_CK_FUNC           93
#define UART2_CK_FUNC            93
#define USART2_CTS_FUNC          94
#define UART2_CTS_FUNC           94
#define USART2_DE_FUNC           95
#define UART2_DE_FUNC            95
#define USART2_NSS_FUNC          96
#define UART2_NSS_FUNC           96
#define USART2_RTS_FUNC          97
#define UART2_RTS_FUNC           97
#define USART2_RX_FUNC           98
#define UART2_RX_FUNC            98
#define USART2_TX_FUNC           99
#define UART2_TX_FUNC            99

# define PINMUX(gpio, func) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA00) && (func == I2C3_SMBA_FUNC           )) ?  4 : \
	((gpio == PA00) && (func == I2S_CKIN_FUNC            )) ?  5 : \
	((gpio == PA00) && (func == USART2_CTS_FUNC          )) ?  7 : \
	((gpio == PA00) && (func == USART2_NSS_FUNC          )) ?  7 : \
	((gpio == PA00) && (func == COMP1_OUT_FUNC           )) ? 12 : \
	((gpio == PA00) && (func == DEBUG_PWR_REGLP1S_FUNC   )) ? 13 : \
	((gpio == PA00) && (func == TIM2_ETR_FUNC            )) ? 14 : \
	((gpio == PA00) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA01) && (func == TIM2_CH2_FUNC            )) ?  1 : \
	((gpio == PA01) && (func == LPTIM3_OUT_FUNC          )) ?  3 : \
	((gpio == PA01) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PA01) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PA01) && (func == USART2_DE_FUNC           )) ?  7 : \
	((gpio == PA01) && (func == USART2_RTS_FUNC          )) ?  7 : \
	((gpio == PA01) && (func == LPUART1_RTS_FUNC         )) ?  8 : \
	((gpio == PA01) && (func == DEBUG_PWR_REGLP2S_FUNC   )) ? 13 : \
	((gpio == PA01) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA02) && (func == RCC_LSCO_FUNC            )) ?  0 : \
	((gpio == PA02) && (func == TIM2_CH3_FUNC            )) ?  1 : \
	((gpio == PA02) && (func == USART2_TX_FUNC           )) ?  7 : \
	((gpio == PA02) && (func == LPUART1_TX_FUNC          )) ?  8 : \
	((gpio == PA02) && (func == COMP2_OUT_FUNC           )) ? 12 : \
	((gpio == PA02) && (func == DEBUG_PWR_LDORDY_FUNC    )) ? 13 : \
	((gpio == PA02) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA03) && (func == TIM2_CH4_FUNC            )) ?  1 : \
	((gpio == PA03) && (func == I2S2_MCK_FUNC            )) ?  5 : \
	((gpio == PA03) && (func == USART2_RX_FUNC           )) ?  7 : \
	((gpio == PA03) && (func == LPUART1_RX_FUNC          )) ?  8 : \
	((gpio == PA03) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA04) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PA04) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PA04) && (func == USART2_CK_FUNC           )) ?  7 : \
	((gpio == PA04) && (func == DEBUG_SUBGHZSPI_NSSOUT_FUNC)) ? 13 : \
	((gpio == PA04) && (func == LPTIM2_OUT_FUNC          )) ? 14 : \
	((gpio == PA04) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA05) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA05) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA05) && (func == SPI2_MISO_FUNC           )) ?  3 : \
	((gpio == PA05) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PA05) && (func == DEBUG_SUBGHZSPI_SCKOUT_FUNC)) ? 13 : \
	((gpio == PA05) && (func == LPTIM2_ETR_FUNC          )) ? 14 : \
	((gpio == PA05) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA06) && (func == I2C2_SMBA_FUNC           )) ?  4 : \
	((gpio == PA06) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PA06) && (func == LPUART1_CTS_FUNC         )) ?  8 : \
	((gpio == PA06) && (func == TIM1_BKIN_FUNC           )) ? 12 : \
	((gpio == PA06) && (func == DEBUG_SUBGHZSPI_MISOOUT_FUNC)) ? 13 : \
	((gpio == PA06) && (func == TIM16_CH1_FUNC           )) ? 14 : \
	((gpio == PA06) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA07) && (func == TIM1_CH1N_FUNC           )) ?  1 : \
	((gpio == PA07) && (func == I2C3_SCL_FUNC            )) ?  4 : \
	((gpio == PA07) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PA07) && (func == COMP2_OUT_FUNC           )) ? 12 : \
	((gpio == PA07) && (func == DEBUG_SUBGHZSPI_MOSIOUT_FUNC)) ? 13 : \
	((gpio == PA07) && (func == TIM17_CH1_FUNC           )) ? 14 : \
	((gpio == PA07) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA08) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PA08) && (func == TIM1_CH1_FUNC            )) ?  1 : \
	((gpio == PA08) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PA08) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PA08) && (func == USART1_CK_FUNC           )) ?  7 : \
	((gpio == PA08) && (func == LPTIM2_OUT_FUNC          )) ? 14 : \
	((gpio == PA08) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA09) && (func == TIM1_CH2_FUNC            )) ?  1 : \
	((gpio == PA09) && (func == I2S2_WS_FUNC             )) ?  3 : \
	((gpio == PA09) && (func == SPI2_NSS_FUNC            )) ?  3 : \
	((gpio == PA09) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PA09) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PA09) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PA09) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PA09) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA10) && (func == RTC_REFIN_FUNC           )) ?  0 : \
	((gpio == PA10) && (func == TIM1_CH3_FUNC            )) ?  1 : \
	((gpio == PA10) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PA10) && (func == I2S2_SD_FUNC             )) ?  5 : \
	((gpio == PA10) && (func == SPI2_MOSI_FUNC           )) ?  5 : \
	((gpio == PA10) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PA10) && (func == DEBUG_RF_HSE32RDY_FUNC   )) ? 13 : \
	((gpio == PA10) && (func == TIM17_BKIN_FUNC          )) ? 14 : \
	((gpio == PA10) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA11) && (func == TIM1_CH4_FUNC            )) ?  1 : \
	((gpio == PA11) && (func == LPTIM3_ETR_FUNC          )) ?  3 : \
	((gpio == PA11) && (func == I2C2_SDA_FUNC            )) ?  4 : \
	((gpio == PA11) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PA11) && (func == USART1_CTS_FUNC          )) ?  7 : \
	((gpio == PA11) && (func == USART1_NSS_FUNC          )) ?  7 : \
	((gpio == PA11) && (func == TIM1_BKIN2_FUNC          )) ? 12 : \
	((gpio == PA11) && (func == DEBUG_RF_NRESET_FUNC     )) ? 13 : \
	((gpio == PA11) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA12) && (func == TIM1_ETR_FUNC            )) ?  1 : \
	((gpio == PA12) && (func == LPTIM3_IN1_FUNC          )) ?  3 : \
	((gpio == PA12) && (func == I2C2_SCL_FUNC            )) ?  4 : \
	((gpio == PA12) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PA12) && (func == DEBUG_RF_BUSY_FUNC       )) ?  6 : \
	((gpio == PA12) && (func == USART1_DE_FUNC           )) ?  7 : \
	((gpio == PA12) && (func == USART1_RTS_FUNC          )) ?  7 : \
	((gpio == PA12) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA13) && (func == DEBUG_JTMS_SWDIO_FUNC    )) ?  0 : \
	((gpio == PA13) && (func == I2C2_SMBA_FUNC           )) ?  4 : \
	((gpio == PA13) && (func == IR_OUT_FUNC              )) ?  8 : \
	((gpio == PA13) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA14) && (func == DEBUG_JTCK_SWCLK_FUNC    )) ?  0 : \
	((gpio == PA14) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PA14) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PA14) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA15) && (func == DEBUG_JTDI_FUNC          )) ?  0 : \
	((gpio == PA15) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA15) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA15) && (func == I2C2_SDA_FUNC            )) ?  4 : \
	((gpio == PA15) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PA15) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB00) && (func == COMP1_OUT_FUNC           )) ? 12 : \
	((gpio == PB00) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB01) && (func == LPUART1_DE_FUNC          )) ?  8 : \
	((gpio == PB01) && (func == LPUART1_RTS_FUNC         )) ?  8 : \
	((gpio == PB01) && (func == LPTIM2_IN1_FUNC          )) ? 14 : \
	((gpio == PB01) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB02) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PB02) && (func == I2C3_SMBA_FUNC           )) ?  4 : \
	((gpio == PB02) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PB02) && (func == DEBUG_RF_SMPSRDY_FUNC    )) ? 13 : \
	((gpio == PB02) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB03) && (func == DEBUG_JTDO_SWO_FUNC      )) ?  0 : \
	((gpio == PB03) && (func == TIM2_CH2_FUNC            )) ?  1 : \
	((gpio == PB03) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PB03) && (func == RF_IRQ0_FUNC             )) ?  6 : \
	((gpio == PB03) && (func == USART1_DE_FUNC           )) ?  7 : \
	((gpio == PB03) && (func == USART1_RTS_FUNC          )) ?  7 : \
	((gpio == PB03) && (func == DEBUG_RF_DTB1_FUNC       )) ? 13 : \
	((gpio == PB03) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB04) && (func == SYS_JTRST_FUNC           )) ?  0 : \
	((gpio == PB04) && (func == I2C3_SDA_FUNC            )) ?  4 : \
	((gpio == PB04) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PB04) && (func == USART1_CTS_FUNC          )) ?  7 : \
	((gpio == PB04) && (func == USART1_NSS_FUNC          )) ?  7 : \
	((gpio == PB04) && (func == DEBUG_RF_LDORDY_FUNC     )) ? 13 : \
	((gpio == PB04) && (func == TIM17_BKIN_FUNC          )) ? 14 : \
	((gpio == PB04) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB05) && (func == LPTIM1_IN1_FUNC          )) ?  1 : \
	((gpio == PB05) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PB05) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PB05) && (func == RF_IRQ1_FUNC             )) ?  6 : \
	((gpio == PB05) && (func == USART1_CK_FUNC           )) ?  7 : \
	((gpio == PB05) && (func == COMP2_OUT_FUNC           )) ? 12 : \
	((gpio == PB05) && (func == TIM16_BKIN_FUNC          )) ? 14 : \
	((gpio == PB05) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB06) && (func == LPTIM1_ETR_FUNC          )) ?  1 : \
	((gpio == PB06) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB06) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PB06) && (func == TIM16_CH1N_FUNC          )) ? 14 : \
	((gpio == PB06) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB07) && (func == LPTIM1_IN2_FUNC          )) ?  1 : \
	((gpio == PB07) && (func == TIM1_BKIN_FUNC           )) ?  3 : \
	((gpio == PB07) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB07) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PB07) && (func == TIM17_CH1N_FUNC          )) ? 14 : \
	((gpio == PB07) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB08) && (func == TIM1_CH2N_FUNC           )) ?  1 : \
	((gpio == PB08) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB08) && (func == RF_IRQ2_FUNC             )) ?  6 : \
	((gpio == PB08) && (func == TIM16_CH1_FUNC           )) ? 14 : \
	((gpio == PB08) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB09) && (func == TIM1_CH3N_FUNC           )) ?  1 : \
	((gpio == PB09) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB09) && (func == I2S2_WS_FUNC             )) ?  5 : \
	((gpio == PB09) && (func == SPI2_NSS_FUNC            )) ?  5 : \
	((gpio == PB09) && (func == IR_OUT_FUNC              )) ?  8 : \
	((gpio == PB09) && (func == TIM17_CH1_FUNC           )) ? 14 : \
	((gpio == PB09) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB10) && (func == TIM2_CH3_FUNC            )) ?  1 : \
	((gpio == PB10) && (func == I2C3_SCL_FUNC            )) ?  4 : \
	((gpio == PB10) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PB10) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PB10) && (func == LPUART1_RX_FUNC          )) ?  8 : \
	((gpio == PB10) && (func == COMP1_OUT_FUNC           )) ? 12 : \
	((gpio == PB10) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB11) && (func == TIM2_CH4_FUNC            )) ?  1 : \
	((gpio == PB11) && (func == I2C3_SDA_FUNC            )) ?  4 : \
	((gpio == PB11) && (func == LPUART1_TX_FUNC          )) ?  8 : \
	((gpio == PB11) && (func == COMP2_OUT_FUNC           )) ? 12 : \
	((gpio == PB11) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB12) && (func == TIM1_BKIN_FUNC           )) ?  3 : \
	((gpio == PB12) && (func == I2C3_SMBA_FUNC           )) ?  4 : \
	((gpio == PB12) && (func == I2S2_WS_FUNC             )) ?  5 : \
	((gpio == PB12) && (func == SPI2_NSS_FUNC            )) ?  5 : \
	((gpio == PB12) && (func == LPUART1_DE_FUNC          )) ?  8 : \
	((gpio == PB12) && (func == LPUART1_RTS_FUNC         )) ?  8 : \
	((gpio == PB12) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB13) && (func == TIM1_CH1N_FUNC           )) ?  1 : \
	((gpio == PB13) && (func == I2C3_SCL_FUNC            )) ?  4 : \
	((gpio == PB13) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PB13) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PB13) && (func == LPUART1_CTS_FUNC         )) ?  8 : \
	((gpio == PB13) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB14) && (func == TIM1_CH2N_FUNC           )) ?  1 : \
	((gpio == PB14) && (func == I2S2_MCK_FUNC            )) ?  3 : \
	((gpio == PB14) && (func == I2C3_SDA_FUNC            )) ?  4 : \
	((gpio == PB14) && (func == SPI2_MISO_FUNC           )) ?  5 : \
	((gpio == PB14) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB15) && (func == TIM1_CH3N_FUNC           )) ?  1 : \
	((gpio == PB15) && (func == I2C2_SCL_FUNC            )) ?  4 : \
	((gpio == PB15) && (func == I2S2_SD_FUNC             )) ?  5 : \
	((gpio == PB15) && (func == SPI2_MOSI_FUNC           )) ?  5 : \
	((gpio == PB15) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC00) && (func == LPTIM1_IN1_FUNC          )) ?  1 : \
	((gpio == PC00) && (func == I2C3_SCL_FUNC            )) ?  4 : \
	((gpio == PC00) && (func == LPUART1_RX_FUNC          )) ?  8 : \
	((gpio == PC00) && (func == LPTIM2_IN1_FUNC          )) ? 14 : \
	((gpio == PC00) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC01) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PC01) && (func == I2S2_SD_FUNC             )) ?  3 : \
	((gpio == PC01) && (func == SPI2_MOSI_FUNC           )) ?  3 : \
	((gpio == PC01) && (func == I2C3_SDA_FUNC            )) ?  4 : \
	((gpio == PC01) && (func == LPUART1_TX_FUNC          )) ?  8 : \
	((gpio == PC01) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC02) && (func == LPTIM1_IN2_FUNC          )) ?  1 : \
	((gpio == PC02) && (func == SPI2_MISO_FUNC           )) ?  5 : \
	((gpio == PC02) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC03) && (func == LPTIM1_ETR_FUNC          )) ?  1 : \
	((gpio == PC03) && (func == I2S2_SD_FUNC             )) ?  5 : \
	((gpio == PC03) && (func == SPI2_MOSI_FUNC           )) ?  5 : \
	((gpio == PC03) && (func == LPTIM2_ETR_FUNC          )) ? 14 : \
	((gpio == PC03) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC04) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC05) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC06) && (func == I2S2_MCK_FUNC            )) ?  5 : \
	((gpio == PC06) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC13) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC14) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC15) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PH03) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	AF_NO_SUCH_PINFUNC)

# define TIMERMUX(gpio, timer_nr, channel_nr) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA01) && (timer_nr ==  2) && (channel_nr ==  2)) ?  1 : \
	((gpio == PA02) && (timer_nr ==  2) && (channel_nr ==  3)) ?  1 : \
	((gpio == PA03) && (timer_nr ==  2) && (channel_nr ==  4)) ?  1 : \
	((gpio == PA05) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA06) && (timer_nr == 16) && (channel_nr ==  1)) ? 14 : \
	((gpio == PA07) && (timer_nr ==  1) && (channel_nr == -1)) ?  1 : \
	((gpio == PA07) && (timer_nr == 17) && (channel_nr ==  1)) ? 14 : \
	((gpio == PA08) && (timer_nr ==  1) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA09) && (timer_nr ==  1) && (channel_nr ==  2)) ?  1 : \
	((gpio == PA10) && (timer_nr ==  1) && (channel_nr ==  3)) ?  1 : \
	((gpio == PA11) && (timer_nr ==  1) && (channel_nr ==  4)) ?  1 : \
	((gpio == PA15) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PB03) && (timer_nr ==  2) && (channel_nr ==  2)) ?  1 : \
	((gpio == PB06) && (timer_nr == 16) && (channel_nr == -1)) ? 14 : \
	((gpio == PB07) && (timer_nr == 17) && (channel_nr == -1)) ? 14 : \
	((gpio == PB08) && (timer_nr ==  1) && (channel_nr == -2)) ?  1 : \
	((gpio == PB08) && (timer_nr == 16) && (channel_nr ==  1)) ? 14 : \
	((gpio == PB09) && (timer_nr ==  1) && (channel_nr == -3)) ?  1 : \
	((gpio == PB09) && (timer_nr == 17) && (channel_nr ==  1)) ? 14 : \
	((gpio == PB10) && (timer_nr ==  2) && (channel_nr ==  3)) ?  1 : \
	((gpio == PB11) && (timer_nr ==  2) && (channel_nr ==  4)) ?  1 : \
	((gpio == PB13) && (timer_nr ==  1) && (channel_nr == -1)) ?  1 : \
	((gpio == PB14) && (timer_nr ==  1) && (channel_nr == -2)) ?  1 : \
	((gpio == PB15) && (timer_nr ==  1) && (channel_nr == -3)) ?  1 : \
	AF_NO_SUCH_PINFUNC)
#endif
