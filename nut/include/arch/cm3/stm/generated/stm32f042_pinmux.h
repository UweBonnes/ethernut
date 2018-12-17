/* Do not edit!
 * Generated from /opt/stm/STM32CubeMX/db/mcu/IPGPIO-STM32F042_gpio_v1_0_Modes.xml*/

#if !defined(_STM32F042_PINMUX_H_)
# define _STM32F042_PINMUX_H_

#define CAN_RX_FUNC              0
#define CAN1_RX_FUNC             0
#define CAN_TX_FUNC              1
#define CAN1_TX_FUNC             1
#define CEC_FUNC                 2
#define CRS_SYNC_FUNC            3
#define EVENTOUT_FUNC            4
#define I2C1_SCL_FUNC            5
#define I2C1_SDA_FUNC            6
#define I2C1_SMBA_FUNC           7
#define I2C2_SCL_FUNC            8
#define I2C2_SDA_FUNC            9
#define I2S1_CK_FUNC             10
#define I2S1_MCK_FUNC            11
#define I2S1_SD_FUNC             12
#define I2S1_WS_FUNC             13
#define IR_OUT_FUNC              14
#define RCC_MCO_FUNC             15
#define SPI1_MISO_FUNC           16
#define SPI1_MOSI_FUNC           17
#define SPI1_NSS_FUNC            18
#define SPI1_SCK_FUNC            19
#define SPI2_MISO_FUNC           20
#define SPI2_MOSI_FUNC           21
#define SPI2_NSS_FUNC            22
#define SPI2_SCK_FUNC            23
#define SYS_SWCLK_FUNC           24
#define SYS_SWDIO_FUNC           25
#define TIM14_CH1_FUNC           26
#define TIM16_BKIN_FUNC          27
#define TIM16_CH1_FUNC           28
#define TIM16_CH1N_FUNC          29
#define TIM17_BKIN_FUNC          30
#define TIM17_CH1_FUNC           31
#define TIM17_CH1N_FUNC          32
#define TIM1_BKIN_FUNC           33
#define TIM1_CH1_FUNC            34
#define TIM1_CH1N_FUNC           35
#define TIM1_CH2_FUNC            36
#define TIM1_CH2N_FUNC           37
#define TIM1_CH3_FUNC            38
#define TIM1_CH3N_FUNC           39
#define TIM1_CH4_FUNC            40
#define TIM1_ETR_FUNC            41
#define TIM2_CH1_FUNC            42
#define TIM2_CH2_FUNC            43
#define TIM2_CH3_FUNC            44
#define TIM2_CH4_FUNC            45
#define TIM2_ETR_FUNC            46
#define TIM3_CH1_FUNC            47
#define TIM3_CH2_FUNC            48
#define TIM3_CH3_FUNC            49
#define TIM3_CH4_FUNC            50
#define TSC_G1_IO1_FUNC          51
#define TSC_G1_IO2_FUNC          52
#define TSC_G1_IO3_FUNC          53
#define TSC_G1_IO4_FUNC          54
#define TSC_G2_IO1_FUNC          55
#define TSC_G2_IO2_FUNC          56
#define TSC_G2_IO3_FUNC          57
#define TSC_G2_IO4_FUNC          58
#define TSC_G3_IO2_FUNC          59
#define TSC_G3_IO3_FUNC          60
#define TSC_G3_IO4_FUNC          61
#define TSC_G4_IO1_FUNC          62
#define TSC_G4_IO2_FUNC          63
#define TSC_G4_IO3_FUNC          64
#define TSC_G4_IO4_FUNC          65
#define TSC_G5_IO1_FUNC          66
#define TSC_G5_IO2_FUNC          67
#define TSC_G5_IO3_FUNC          68
#define TSC_G5_IO4_FUNC          69
#define TSC_SYNC_FUNC            70
#define USART1_CK_FUNC           71
#define UART1_CK_FUNC            71
#define USART1_CTS_FUNC          72
#define UART1_CTS_FUNC           72
#define USART1_DE_FUNC           73
#define UART1_DE_FUNC            73
#define USART1_RTS_FUNC          74
#define UART1_RTS_FUNC           74
#define USART1_RX_FUNC           75
#define UART1_RX_FUNC            75
#define USART1_TX_FUNC           76
#define UART1_TX_FUNC            76
#define USART2_CK_FUNC           77
#define UART2_CK_FUNC            77
#define USART2_CTS_FUNC          78
#define UART2_CTS_FUNC           78
#define USART2_DE_FUNC           79
#define UART2_DE_FUNC            79
#define USART2_RTS_FUNC          80
#define UART2_RTS_FUNC           80
#define USART2_RX_FUNC           81
#define UART2_RX_FUNC            81
#define USART2_TX_FUNC           82
#define UART2_TX_FUNC            82
#define USB_NOE_FUNC             83

# define PINMUX(gpio, func) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (func == USART2_CTS_FUNC          )) ?  1 : \
	((gpio == PA00) && (func == TIM2_CH1_FUNC            )) ?  2 : \
	((gpio == PA00) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA00) && (func == TSC_G1_IO1_FUNC          )) ?  3 : \
	((gpio == PA01) && (func == EVENTOUT_FUNC            )) ?  0 : \
	((gpio == PA01) && (func == USART2_DE_FUNC           )) ?  1 : \
	((gpio == PA01) && (func == USART2_RTS_FUNC          )) ?  1 : \
	((gpio == PA01) && (func == TIM2_CH2_FUNC            )) ?  2 : \
	((gpio == PA01) && (func == TSC_G1_IO2_FUNC          )) ?  3 : \
	((gpio == PA02) && (func == USART2_TX_FUNC           )) ?  1 : \
	((gpio == PA02) && (func == TIM2_CH3_FUNC            )) ?  2 : \
	((gpio == PA02) && (func == TSC_G1_IO3_FUNC          )) ?  3 : \
	((gpio == PA03) && (func == USART2_RX_FUNC           )) ?  1 : \
	((gpio == PA03) && (func == TIM2_CH4_FUNC            )) ?  2 : \
	((gpio == PA03) && (func == TSC_G1_IO4_FUNC          )) ?  3 : \
	((gpio == PA04) && (func == I2S1_WS_FUNC             )) ?  0 : \
	((gpio == PA04) && (func == SPI1_NSS_FUNC            )) ?  0 : \
	((gpio == PA04) && (func == USART2_CK_FUNC           )) ?  1 : \
	((gpio == PA04) && (func == USB_NOE_FUNC             )) ?  2 : \
	((gpio == PA04) && (func == TSC_G2_IO1_FUNC          )) ?  3 : \
	((gpio == PA04) && (func == TIM14_CH1_FUNC           )) ?  4 : \
	((gpio == PA05) && (func == I2S1_CK_FUNC             )) ?  0 : \
	((gpio == PA05) && (func == SPI1_SCK_FUNC            )) ?  0 : \
	((gpio == PA05) && (func == CEC_FUNC                 )) ?  1 : \
	((gpio == PA05) && (func == TIM2_CH1_FUNC            )) ?  2 : \
	((gpio == PA05) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA05) && (func == TSC_G2_IO2_FUNC          )) ?  3 : \
	((gpio == PA06) && (func == I2S1_MCK_FUNC            )) ?  0 : \
	((gpio == PA06) && (func == SPI1_MISO_FUNC           )) ?  0 : \
	((gpio == PA06) && (func == TIM3_CH1_FUNC            )) ?  1 : \
	((gpio == PA06) && (func == TIM1_BKIN_FUNC           )) ?  2 : \
	((gpio == PA06) && (func == TSC_G2_IO3_FUNC          )) ?  3 : \
	((gpio == PA06) && (func == TIM16_CH1_FUNC           )) ?  5 : \
	((gpio == PA06) && (func == EVENTOUT_FUNC            )) ?  6 : \
	((gpio == PA07) && (func == I2S1_SD_FUNC             )) ?  0 : \
	((gpio == PA07) && (func == SPI1_MOSI_FUNC           )) ?  0 : \
	((gpio == PA07) && (func == TIM3_CH2_FUNC            )) ?  1 : \
	((gpio == PA07) && (func == TIM1_CH1N_FUNC           )) ?  2 : \
	((gpio == PA07) && (func == TSC_G2_IO4_FUNC          )) ?  3 : \
	((gpio == PA07) && (func == TIM14_CH1_FUNC           )) ?  4 : \
	((gpio == PA07) && (func == TIM17_CH1_FUNC           )) ?  5 : \
	((gpio == PA07) && (func == EVENTOUT_FUNC            )) ?  6 : \
	((gpio == PA08) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PA08) && (func == USART1_CK_FUNC           )) ?  1 : \
	((gpio == PA08) && (func == TIM1_CH1_FUNC            )) ?  2 : \
	((gpio == PA08) && (func == EVENTOUT_FUNC            )) ?  3 : \
	((gpio == PA08) && (func == CRS_SYNC_FUNC            )) ?  4 : \
	((gpio == PA09) && (func == USART1_TX_FUNC           )) ?  1 : \
	((gpio == PA09) && (func == TIM1_CH2_FUNC            )) ?  2 : \
	((gpio == PA09) && (func == TSC_G4_IO1_FUNC          )) ?  3 : \
	((gpio == PA09) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PA09) && (func == RCC_MCO_FUNC             )) ?  5 : \
	((gpio == PA10) && (func == TIM17_BKIN_FUNC          )) ?  0 : \
	((gpio == PA10) && (func == USART1_RX_FUNC           )) ?  1 : \
	((gpio == PA10) && (func == TIM1_CH3_FUNC            )) ?  2 : \
	((gpio == PA10) && (func == TSC_G4_IO2_FUNC          )) ?  3 : \
	((gpio == PA10) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PA11) && (func == EVENTOUT_FUNC            )) ?  0 : \
	((gpio == PA11) && (func == USART1_CTS_FUNC          )) ?  1 : \
	((gpio == PA11) && (func == TIM1_CH4_FUNC            )) ?  2 : \
	((gpio == PA11) && (func == TSC_G4_IO3_FUNC          )) ?  3 : \
	((gpio == PA11) && (func == CAN_RX_FUNC              )) ?  4 : \
	((gpio == PA11) && (func == I2C1_SCL_FUNC            )) ?  5 : \
	((gpio == PA12) && (func == EVENTOUT_FUNC            )) ?  0 : \
	((gpio == PA12) && (func == USART1_DE_FUNC           )) ?  1 : \
	((gpio == PA12) && (func == USART1_RTS_FUNC          )) ?  1 : \
	((gpio == PA12) && (func == TIM1_ETR_FUNC            )) ?  2 : \
	((gpio == PA12) && (func == TSC_G4_IO4_FUNC          )) ?  3 : \
	((gpio == PA12) && (func == CAN_TX_FUNC              )) ?  4 : \
	((gpio == PA12) && (func == I2C1_SDA_FUNC            )) ?  5 : \
	((gpio == PA13) && (func == SYS_SWDIO_FUNC           )) ?  0 : \
	((gpio == PA13) && (func == IR_OUT_FUNC              )) ?  1 : \
	((gpio == PA13) && (func == USB_NOE_FUNC             )) ?  2 : \
	((gpio == PA14) && (func == SYS_SWCLK_FUNC           )) ?  0 : \
	((gpio == PA14) && (func == USART2_TX_FUNC           )) ?  1 : \
	((gpio == PA15) && (func == I2S1_WS_FUNC             )) ?  0 : \
	((gpio == PA15) && (func == SPI1_NSS_FUNC            )) ?  0 : \
	((gpio == PA15) && (func == USART2_RX_FUNC           )) ?  1 : \
	((gpio == PA15) && (func == TIM2_CH1_FUNC            )) ?  2 : \
	((gpio == PA15) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA15) && (func == EVENTOUT_FUNC            )) ?  3 : \
	((gpio == PA15) && (func == USB_NOE_FUNC             )) ?  5 : \
	((gpio == PB00) && (func == EVENTOUT_FUNC            )) ?  0 : \
	((gpio == PB00) && (func == TIM3_CH3_FUNC            )) ?  1 : \
	((gpio == PB00) && (func == TIM1_CH2N_FUNC           )) ?  2 : \
	((gpio == PB00) && (func == TSC_G3_IO2_FUNC          )) ?  3 : \
	((gpio == PB01) && (func == TIM14_CH1_FUNC           )) ?  0 : \
	((gpio == PB01) && (func == TIM3_CH4_FUNC            )) ?  1 : \
	((gpio == PB01) && (func == TIM1_CH3N_FUNC           )) ?  2 : \
	((gpio == PB01) && (func == TSC_G3_IO3_FUNC          )) ?  3 : \
	((gpio == PB02) && (func == TSC_G3_IO4_FUNC          )) ?  3 : \
	((gpio == PB03) && (func == I2S1_CK_FUNC             )) ?  0 : \
	((gpio == PB03) && (func == SPI1_SCK_FUNC            )) ?  0 : \
	((gpio == PB03) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PB03) && (func == TIM2_CH2_FUNC            )) ?  2 : \
	((gpio == PB03) && (func == TSC_G5_IO1_FUNC          )) ?  3 : \
	((gpio == PB04) && (func == I2S1_MCK_FUNC            )) ?  0 : \
	((gpio == PB04) && (func == SPI1_MISO_FUNC           )) ?  0 : \
	((gpio == PB04) && (func == TIM3_CH1_FUNC            )) ?  1 : \
	((gpio == PB04) && (func == EVENTOUT_FUNC            )) ?  2 : \
	((gpio == PB04) && (func == TSC_G5_IO2_FUNC          )) ?  3 : \
	((gpio == PB04) && (func == TIM17_BKIN_FUNC          )) ?  5 : \
	((gpio == PB05) && (func == I2S1_SD_FUNC             )) ?  0 : \
	((gpio == PB05) && (func == SPI1_MOSI_FUNC           )) ?  0 : \
	((gpio == PB05) && (func == TIM3_CH2_FUNC            )) ?  1 : \
	((gpio == PB05) && (func == TIM16_BKIN_FUNC          )) ?  2 : \
	((gpio == PB05) && (func == I2C1_SMBA_FUNC           )) ?  3 : \
	((gpio == PB06) && (func == USART1_TX_FUNC           )) ?  0 : \
	((gpio == PB06) && (func == I2C1_SCL_FUNC            )) ?  1 : \
	((gpio == PB06) && (func == TIM16_CH1N_FUNC          )) ?  2 : \
	((gpio == PB06) && (func == TSC_G5_IO3_FUNC          )) ?  3 : \
	((gpio == PB07) && (func == USART1_RX_FUNC           )) ?  0 : \
	((gpio == PB07) && (func == I2C1_SDA_FUNC            )) ?  1 : \
	((gpio == PB07) && (func == TIM17_CH1N_FUNC          )) ?  2 : \
	((gpio == PB07) && (func == TSC_G5_IO4_FUNC          )) ?  3 : \
	((gpio == PB08) && (func == CEC_FUNC                 )) ?  0 : \
	((gpio == PB08) && (func == I2C1_SCL_FUNC            )) ?  1 : \
	((gpio == PB08) && (func == TIM16_CH1_FUNC           )) ?  2 : \
	((gpio == PB08) && (func == TSC_SYNC_FUNC            )) ?  3 : \
	((gpio == PB08) && (func == CAN_RX_FUNC              )) ?  4 : \
	((gpio == PB09) && (func == IR_OUT_FUNC              )) ?  0 : \
	((gpio == PB09) && (func == I2C1_SDA_FUNC            )) ?  1 : \
	((gpio == PB09) && (func == TIM17_CH1_FUNC           )) ?  2 : \
	((gpio == PB09) && (func == EVENTOUT_FUNC            )) ?  3 : \
	((gpio == PB09) && (func == CAN_TX_FUNC              )) ?  4 : \
	((gpio == PB09) && (func == SPI2_NSS_FUNC            )) ?  5 : \
	((gpio == PB10) && (func == CEC_FUNC                 )) ?  0 : \
	((gpio == PB10) && (func == I2C1_SCL_FUNC            )) ?  1 : \
	((gpio == PB10) && (func == TIM2_CH3_FUNC            )) ?  2 : \
	((gpio == PB10) && (func == TSC_SYNC_FUNC            )) ?  3 : \
	((gpio == PB10) && (func == SPI2_SCK_FUNC            )) ?  5 : \
	((gpio == PB11) && (func == EVENTOUT_FUNC            )) ?  0 : \
	((gpio == PB11) && (func == I2C1_SDA_FUNC            )) ?  1 : \
	((gpio == PB11) && (func == TIM2_CH4_FUNC            )) ?  2 : \
	((gpio == PB12) && (func == SPI2_NSS_FUNC            )) ?  0 : \
	((gpio == PB12) && (func == EVENTOUT_FUNC            )) ?  1 : \
	((gpio == PB12) && (func == TIM1_BKIN_FUNC           )) ?  2 : \
	((gpio == PB13) && (func == SPI2_SCK_FUNC            )) ?  0 : \
	((gpio == PB13) && (func == TIM1_CH1N_FUNC           )) ?  2 : \
	((gpio == PB13) && (func == I2C2_SCL_FUNC            )) ?  5 : \
	((gpio == PB14) && (func == SPI2_MISO_FUNC           )) ?  0 : \
	((gpio == PB14) && (func == TIM1_CH2N_FUNC           )) ?  2 : \
	((gpio == PB14) && (func == I2C2_SDA_FUNC            )) ?  5 : \
	((gpio == PB15) && (func == SPI2_MOSI_FUNC           )) ?  0 : \
	((gpio == PB15) && (func == TIM1_CH3N_FUNC           )) ?  2 : \
	((gpio == PF00) && (func == CRS_SYNC_FUNC            )) ?  0 : \
	((gpio == PF00) && (func == I2C1_SDA_FUNC            )) ?  1 : \
	((gpio == PF01) && (func == I2C1_SCL_FUNC            )) ?  1 : \
	AF_NO_SUCH_PINFUNC)

# define TIMERMUX(gpio, timer_nr, channel_nr) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (timer_nr ==  2) && (channel_nr ==  1)) ?  2 : \
	((gpio == PA01) && (timer_nr ==  2) && (channel_nr ==  2)) ?  2 : \
	((gpio == PA02) && (timer_nr ==  2) && (channel_nr ==  3)) ?  2 : \
	((gpio == PA03) && (timer_nr ==  2) && (channel_nr ==  4)) ?  2 : \
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
	((gpio == PA10) && (timer_nr ==  1) && (channel_nr ==  3)) ?  2 : \
	((gpio == PA11) && (timer_nr ==  1) && (channel_nr ==  4)) ?  2 : \
	((gpio == PA15) && (timer_nr ==  2) && (channel_nr ==  1)) ?  2 : \
	((gpio == PB00) && (timer_nr ==  3) && (channel_nr ==  3)) ?  1 : \
	((gpio == PB00) && (timer_nr ==  1) && (channel_nr == -2)) ?  2 : \
	((gpio == PB01) && (timer_nr == 14) && (channel_nr ==  1)) ?  0 : \
	((gpio == PB01) && (timer_nr ==  3) && (channel_nr ==  4)) ?  1 : \
	((gpio == PB01) && (timer_nr ==  1) && (channel_nr == -3)) ?  2 : \
	((gpio == PB03) && (timer_nr ==  2) && (channel_nr ==  2)) ?  2 : \
	((gpio == PB04) && (timer_nr ==  3) && (channel_nr ==  1)) ?  1 : \
	((gpio == PB05) && (timer_nr ==  3) && (channel_nr ==  2)) ?  1 : \
	((gpio == PB06) && (timer_nr == 16) && (channel_nr == -1)) ?  2 : \
	((gpio == PB07) && (timer_nr == 17) && (channel_nr == -1)) ?  2 : \
	((gpio == PB08) && (timer_nr == 16) && (channel_nr ==  1)) ?  2 : \
	((gpio == PB09) && (timer_nr == 17) && (channel_nr ==  1)) ?  2 : \
	((gpio == PB10) && (timer_nr ==  2) && (channel_nr ==  3)) ?  2 : \
	((gpio == PB11) && (timer_nr ==  2) && (channel_nr ==  4)) ?  2 : \
	((gpio == PB13) && (timer_nr ==  1) && (channel_nr == -1)) ?  2 : \
	((gpio == PB14) && (timer_nr ==  1) && (channel_nr == -2)) ?  2 : \
	((gpio == PB15) && (timer_nr ==  1) && (channel_nr == -3)) ?  2 : \
	AF_NO_SUCH_PINFUNC)
#endif
