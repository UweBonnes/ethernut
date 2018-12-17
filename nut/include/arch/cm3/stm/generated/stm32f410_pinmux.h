/* Do not edit!
 * Generated from /opt/stm/STM32CubeMX/db/mcu/IPGPIO-STM32F410_gpio_v1_0_Modes.xml*/

#if !defined(_STM32F410_PINMUX_H_)
# define _STM32F410_PINMUX_H_

#define EVENTOUT_FUNC            0
#define FMPI2C1_SCL_FUNC         1
#define FMPI2C1_SDA_FUNC         2
#define FMPI2C1_SMBA_FUNC        3
#define I2C1_SCL_FUNC            4
#define I2C1_SDA_FUNC            5
#define I2C1_SMBA_FUNC           6
#define I2C2_SCL_FUNC            7
#define I2C2_SDA_FUNC            8
#define I2C2_SMBA_FUNC           9
#define I2S1_CK_FUNC             10
#define I2S1_MCK_FUNC            11
#define I2S1_SD_FUNC             12
#define I2S1_WS_FUNC             13
#define I2S2_CK_FUNC             14
#define I2S2_MCK_FUNC            15
#define I2S2_SD_FUNC             16
#define I2S2_WS_FUNC             17
#define I2S5_CK_FUNC             18
#define I2S5_SD_FUNC             19
#define I2S5_WS_FUNC             20
#define I2S_CKIN_FUNC            21
#define LPTIM1_ETR_FUNC          22
#define LPTIM1_IN1_FUNC          23
#define LPTIM1_IN2_FUNC          24
#define LPTIM1_OUT_FUNC          25
#define RCC_MCO_1_FUNC           26
#define RCC_MCO_2_FUNC           27
#define RTC_REFIN_FUNC           28
#define SPI1_MISO_FUNC           29
#define SPI1_MOSI_FUNC           30
#define SPI1_NSS_FUNC            31
#define SPI1_SCK_FUNC            32
#define SPI2_MISO_FUNC           33
#define SPI2_MOSI_FUNC           34
#define SPI2_NSS_FUNC            35
#define SPI2_SCK_FUNC            36
#define SPI5_MISO_FUNC           37
#define SPI5_MOSI_FUNC           38
#define SPI5_NSS_FUNC            39
#define SPI5_SCK_FUNC            40
#define SYS_JTCK_SWCLK_FUNC      41
#define SYS_JTDI_FUNC            42
#define SYS_JTDO_SWO_FUNC        43
#define SYS_JTMS_SWDIO_FUNC      44
#define SYS_JTRST_FUNC           45
#define SYS_TRACECLK_FUNC        46
#define SYS_TRACED0_FUNC         47
#define SYS_TRACED1_FUNC         48
#define SYS_TRACED2_FUNC         49
#define SYS_TRACED3_FUNC         50
#define TIM11_CH1_FUNC           51
#define TIM1_BKIN_FUNC           52
#define TIM1_CH1_FUNC            53
#define TIM1_CH1N_FUNC           54
#define TIM1_CH2_FUNC            55
#define TIM1_CH2N_FUNC           56
#define TIM1_CH3_FUNC            57
#define TIM1_CH3N_FUNC           58
#define TIM1_CH4_FUNC            59
#define TIM1_ETR_FUNC            60
#define TIM5_CH1_FUNC            61
#define TIM5_CH2_FUNC            62
#define TIM5_CH3_FUNC            63
#define TIM5_CH4_FUNC            64
#define TIM9_CH1_FUNC            65
#define TIM9_CH2_FUNC            66
#define USART1_CK_FUNC           67
#define UART1_CK_FUNC            67
#define USART1_CTS_FUNC          68
#define UART1_CTS_FUNC           68
#define USART1_RTS_FUNC          69
#define UART1_RTS_FUNC           69
#define USART1_RX_FUNC           70
#define UART1_RX_FUNC            70
#define USART1_TX_FUNC           71
#define UART1_TX_FUNC            71
#define USART2_CK_FUNC           72
#define UART2_CK_FUNC            72
#define USART2_CTS_FUNC          73
#define UART2_CTS_FUNC           73
#define USART2_RTS_FUNC          74
#define UART2_RTS_FUNC           74
#define USART2_RX_FUNC           75
#define UART2_RX_FUNC            75
#define USART2_TX_FUNC           76
#define UART2_TX_FUNC            76
#define USART6_CK_FUNC           77
#define UART6_CK_FUNC            77
#define USART6_RX_FUNC           78
#define UART6_RX_FUNC            78
#define USART6_TX_FUNC           79
#define UART6_TX_FUNC            79

# define PINMUX(gpio, func) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (func == TIM5_CH1_FUNC            )) ?  2 : \
	((gpio == PA00) && (func == USART2_CTS_FUNC          )) ?  7 : \
	((gpio == PA00) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA01) && (func == TIM5_CH2_FUNC            )) ?  2 : \
	((gpio == PA01) && (func == USART2_RTS_FUNC          )) ?  7 : \
	((gpio == PA01) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA02) && (func == TIM5_CH3_FUNC            )) ?  2 : \
	((gpio == PA02) && (func == TIM9_CH1_FUNC            )) ?  3 : \
	((gpio == PA02) && (func == I2S_CKIN_FUNC            )) ?  5 : \
	((gpio == PA02) && (func == USART2_TX_FUNC           )) ?  7 : \
	((gpio == PA02) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA03) && (func == TIM5_CH4_FUNC            )) ?  2 : \
	((gpio == PA03) && (func == TIM9_CH2_FUNC            )) ?  3 : \
	((gpio == PA03) && (func == I2S2_MCK_FUNC            )) ?  5 : \
	((gpio == PA03) && (func == USART2_RX_FUNC           )) ?  7 : \
	((gpio == PA03) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA04) && (func == I2S1_WS_FUNC             )) ?  5 : \
	((gpio == PA04) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PA04) && (func == USART2_CK_FUNC           )) ?  7 : \
	((gpio == PA04) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA05) && (func == I2S1_CK_FUNC             )) ?  5 : \
	((gpio == PA05) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PA05) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA06) && (func == TIM1_BKIN_FUNC           )) ?  1 : \
	((gpio == PA06) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PA06) && (func == I2S2_MCK_FUNC            )) ?  6 : \
	((gpio == PA06) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA07) && (func == TIM1_CH1N_FUNC           )) ?  1 : \
	((gpio == PA07) && (func == I2S1_SD_FUNC             )) ?  5 : \
	((gpio == PA07) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PA07) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA08) && (func == RCC_MCO_1_FUNC           )) ?  0 : \
	((gpio == PA08) && (func == TIM1_CH1_FUNC            )) ?  1 : \
	((gpio == PA08) && (func == FMPI2C1_SCL_FUNC         )) ?  4 : \
	((gpio == PA08) && (func == USART1_CK_FUNC           )) ?  7 : \
	((gpio == PA08) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA09) && (func == TIM1_CH2_FUNC            )) ?  1 : \
	((gpio == PA09) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PA09) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA10) && (func == TIM1_CH3_FUNC            )) ?  1 : \
	((gpio == PA10) && (func == I2S5_SD_FUNC             )) ?  6 : \
	((gpio == PA10) && (func == SPI5_MOSI_FUNC           )) ?  6 : \
	((gpio == PA10) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PA10) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA11) && (func == TIM1_CH4_FUNC            )) ?  1 : \
	((gpio == PA11) && (func == USART1_CTS_FUNC          )) ?  7 : \
	((gpio == PA11) && (func == USART6_TX_FUNC           )) ?  8 : \
	((gpio == PA11) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA12) && (func == TIM1_ETR_FUNC            )) ?  1 : \
	((gpio == PA12) && (func == SPI5_MISO_FUNC           )) ?  6 : \
	((gpio == PA12) && (func == USART1_RTS_FUNC          )) ?  7 : \
	((gpio == PA12) && (func == USART6_RX_FUNC           )) ?  8 : \
	((gpio == PA12) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA13) && (func == SYS_JTMS_SWDIO_FUNC      )) ?  0 : \
	((gpio == PA13) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA14) && (func == SYS_JTCK_SWCLK_FUNC      )) ?  0 : \
	((gpio == PA14) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PA15) && (func == SYS_JTDI_FUNC            )) ?  0 : \
	((gpio == PA15) && (func == I2S1_WS_FUNC             )) ?  5 : \
	((gpio == PA15) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PA15) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PA15) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB00) && (func == TIM1_CH2N_FUNC           )) ?  1 : \
	((gpio == PB00) && (func == I2S5_CK_FUNC             )) ?  6 : \
	((gpio == PB00) && (func == SPI5_SCK_FUNC            )) ?  6 : \
	((gpio == PB00) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB01) && (func == TIM1_CH3N_FUNC           )) ?  1 : \
	((gpio == PB01) && (func == I2S5_WS_FUNC             )) ?  6 : \
	((gpio == PB01) && (func == SPI5_NSS_FUNC            )) ?  6 : \
	((gpio == PB01) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB02) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PB02) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB03) && (func == SYS_JTDO_SWO_FUNC        )) ?  0 : \
	((gpio == PB03) && (func == FMPI2C1_SDA_FUNC         )) ?  4 : \
	((gpio == PB03) && (func == I2S1_CK_FUNC             )) ?  5 : \
	((gpio == PB03) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PB03) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PB03) && (func == I2C2_SDA_FUNC            )) ?  9 : \
	((gpio == PB03) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB04) && (func == SYS_JTRST_FUNC           )) ?  0 : \
	((gpio == PB04) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PB04) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB05) && (func == LPTIM1_IN1_FUNC          )) ?  1 : \
	((gpio == PB05) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PB05) && (func == I2S1_SD_FUNC             )) ?  5 : \
	((gpio == PB05) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PB05) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB06) && (func == LPTIM1_ETR_FUNC          )) ?  1 : \
	((gpio == PB06) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB06) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PB06) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB07) && (func == LPTIM1_IN2_FUNC          )) ?  1 : \
	((gpio == PB07) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB07) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PB07) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB08) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PB08) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB08) && (func == I2S5_SD_FUNC             )) ?  6 : \
	((gpio == PB08) && (func == SPI5_MOSI_FUNC           )) ?  6 : \
	((gpio == PB08) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB09) && (func == TIM11_CH1_FUNC           )) ?  3 : \
	((gpio == PB09) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB09) && (func == I2S2_WS_FUNC             )) ?  5 : \
	((gpio == PB09) && (func == SPI2_NSS_FUNC            )) ?  5 : \
	((gpio == PB09) && (func == I2C2_SDA_FUNC            )) ?  9 : \
	((gpio == PB09) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB10) && (func == I2C2_SCL_FUNC            )) ?  4 : \
	((gpio == PB10) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PB10) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PB10) && (func == I2S1_MCK_FUNC            )) ?  6 : \
	((gpio == PB10) && (func == FMPI2C1_SCL_FUNC         )) ?  9 : \
	((gpio == PB10) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB11) && (func == SYS_TRACED3_FUNC         )) ?  0 : \
	((gpio == PB11) && (func == TIM5_CH4_FUNC            )) ?  2 : \
	((gpio == PB11) && (func == I2C2_SDA_FUNC            )) ?  4 : \
	((gpio == PB11) && (func == I2S_CKIN_FUNC            )) ?  5 : \
	((gpio == PB11) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB12) && (func == TIM1_BKIN_FUNC           )) ?  1 : \
	((gpio == PB12) && (func == TIM5_CH1_FUNC            )) ?  2 : \
	((gpio == PB12) && (func == I2C2_SMBA_FUNC           )) ?  4 : \
	((gpio == PB12) && (func == I2S2_WS_FUNC             )) ?  5 : \
	((gpio == PB12) && (func == SPI2_NSS_FUNC            )) ?  5 : \
	((gpio == PB12) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB13) && (func == TIM1_CH1N_FUNC           )) ?  1 : \
	((gpio == PB13) && (func == FMPI2C1_SMBA_FUNC        )) ?  4 : \
	((gpio == PB13) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PB13) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PB13) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB14) && (func == TIM1_CH2N_FUNC           )) ?  1 : \
	((gpio == PB14) && (func == FMPI2C1_SDA_FUNC         )) ?  4 : \
	((gpio == PB14) && (func == SPI2_MISO_FUNC           )) ?  5 : \
	((gpio == PB14) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PB15) && (func == RTC_REFIN_FUNC           )) ?  0 : \
	((gpio == PB15) && (func == TIM1_CH3N_FUNC           )) ?  1 : \
	((gpio == PB15) && (func == FMPI2C1_SCL_FUNC         )) ?  4 : \
	((gpio == PB15) && (func == I2S2_SD_FUNC             )) ?  5 : \
	((gpio == PB15) && (func == SPI2_MOSI_FUNC           )) ?  5 : \
	((gpio == PB15) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC00) && (func == LPTIM1_IN1_FUNC          )) ?  1 : \
	((gpio == PC00) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC01) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PC01) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC02) && (func == LPTIM1_IN2_FUNC          )) ?  1 : \
	((gpio == PC02) && (func == SPI2_MISO_FUNC           )) ?  5 : \
	((gpio == PC02) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC03) && (func == LPTIM1_ETR_FUNC          )) ?  1 : \
	((gpio == PC03) && (func == I2S2_SD_FUNC             )) ?  5 : \
	((gpio == PC03) && (func == SPI2_MOSI_FUNC           )) ?  5 : \
	((gpio == PC03) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC04) && (func == TIM9_CH1_FUNC            )) ?  3 : \
	((gpio == PC04) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC05) && (func == TIM9_CH2_FUNC            )) ?  3 : \
	((gpio == PC05) && (func == FMPI2C1_SMBA_FUNC        )) ?  4 : \
	((gpio == PC05) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC06) && (func == SYS_TRACECLK_FUNC        )) ?  0 : \
	((gpio == PC06) && (func == FMPI2C1_SCL_FUNC         )) ?  4 : \
	((gpio == PC06) && (func == I2S2_MCK_FUNC            )) ?  5 : \
	((gpio == PC06) && (func == USART6_TX_FUNC           )) ?  8 : \
	((gpio == PC06) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC07) && (func == FMPI2C1_SDA_FUNC         )) ?  4 : \
	((gpio == PC07) && (func == I2S2_CK_FUNC             )) ?  5 : \
	((gpio == PC07) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PC07) && (func == I2S1_MCK_FUNC            )) ?  6 : \
	((gpio == PC07) && (func == USART6_RX_FUNC           )) ?  8 : \
	((gpio == PC07) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC08) && (func == USART6_CK_FUNC           )) ?  8 : \
	((gpio == PC08) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC09) && (func == RCC_MCO_2_FUNC           )) ?  0 : \
	((gpio == PC09) && (func == FMPI2C1_SDA_FUNC         )) ?  4 : \
	((gpio == PC09) && (func == I2S_CKIN_FUNC            )) ?  5 : \
	((gpio == PC09) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC10) && (func == SYS_TRACED0_FUNC         )) ?  0 : \
	((gpio == PC10) && (func == TIM5_CH2_FUNC            )) ?  2 : \
	((gpio == PC10) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC11) && (func == SYS_TRACED1_FUNC         )) ?  0 : \
	((gpio == PC11) && (func == TIM5_CH3_FUNC            )) ?  2 : \
	((gpio == PC11) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC12) && (func == SYS_TRACED2_FUNC         )) ?  0 : \
	((gpio == PC12) && (func == TIM11_CH1_FUNC           )) ?  3 : \
	((gpio == PC12) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC13) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC14) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PC15) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PH00) && (func == EVENTOUT_FUNC            )) ? 15 : \
	((gpio == PH01) && (func == EVENTOUT_FUNC            )) ? 15 : \
	AF_NO_SUCH_PINFUNC)

# define TIMERMUX(gpio, timer_nr, channel_nr) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (timer_nr ==  5) && (channel_nr ==  1)) ?  2 : \
	((gpio == PA01) && (timer_nr ==  5) && (channel_nr ==  2)) ?  2 : \
	((gpio == PA02) && (timer_nr ==  5) && (channel_nr ==  3)) ?  2 : \
	((gpio == PA02) && (timer_nr ==  9) && (channel_nr ==  1)) ?  3 : \
	((gpio == PA03) && (timer_nr ==  5) && (channel_nr ==  4)) ?  2 : \
	((gpio == PA03) && (timer_nr ==  9) && (channel_nr ==  2)) ?  3 : \
	((gpio == PA07) && (timer_nr ==  1) && (channel_nr == -1)) ?  1 : \
	((gpio == PA08) && (timer_nr ==  1) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA09) && (timer_nr ==  1) && (channel_nr ==  2)) ?  1 : \
	((gpio == PA10) && (timer_nr ==  1) && (channel_nr ==  3)) ?  1 : \
	((gpio == PA11) && (timer_nr ==  1) && (channel_nr ==  4)) ?  1 : \
	((gpio == PB00) && (timer_nr ==  1) && (channel_nr == -2)) ?  1 : \
	((gpio == PB01) && (timer_nr ==  1) && (channel_nr == -3)) ?  1 : \
	((gpio == PB09) && (timer_nr == 11) && (channel_nr ==  1)) ?  3 : \
	((gpio == PB11) && (timer_nr ==  5) && (channel_nr ==  4)) ?  2 : \
	((gpio == PB12) && (timer_nr ==  5) && (channel_nr ==  1)) ?  2 : \
	((gpio == PB13) && (timer_nr ==  1) && (channel_nr == -1)) ?  1 : \
	((gpio == PB14) && (timer_nr ==  1) && (channel_nr == -2)) ?  1 : \
	((gpio == PB15) && (timer_nr ==  1) && (channel_nr == -3)) ?  1 : \
	((gpio == PC04) && (timer_nr ==  9) && (channel_nr ==  1)) ?  3 : \
	((gpio == PC05) && (timer_nr ==  9) && (channel_nr ==  2)) ?  3 : \
	((gpio == PC10) && (timer_nr ==  5) && (channel_nr ==  2)) ?  2 : \
	((gpio == PC11) && (timer_nr ==  5) && (channel_nr ==  3)) ?  2 : \
	((gpio == PC12) && (timer_nr == 11) && (channel_nr ==  1)) ?  3 : \
	AF_NO_SUCH_PINFUNC)
#endif
