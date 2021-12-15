/* Do not edit!
 * Generated from /devel/cubemx/db/mcu/IPGPIO-STM32WB35x_gpio_v1_0_Modes.xml*/

#if !defined(_STM32WB35x_PINMUX_H_)
# define _STM32WB35x_PINMUX_H_

#define CM4_EVENTOUT_FUNC        0
#define COMP1_OUT_FUNC           1
#define I2C1_SCL_FUNC            2
#define I2C1_SDA_FUNC            3
#define I2C1_SMBA_FUNC           4
#define LPTIM1_ETR_FUNC          5
#define LPTIM1_IN1_FUNC          6
#define LPTIM1_IN2_FUNC          7
#define LPTIM1_OUT_FUNC          8
#define LPTIM2_ETR_FUNC          9
#define LPTIM2_IN1_FUNC          10
#define LPTIM2_OUT_FUNC          11
#define LPUART1_CTS_FUNC         12
#define LPUART1_DE_FUNC          13
#define LPUART1_RTS_FUNC         14
#define LPUART1_RX_FUNC          15
#define LPUART1_TX_FUNC          16
#define RCC_LSCO_FUNC            17
#define RCC_MCO_FUNC             18
#define RF_TX_MOD_EXT_PA_FUNC    19
#define RTC_OUT_FUNC             20
#define SPI1_MISO_FUNC           21
#define SPI1_MOSI_FUNC           22
#define SPI1_NSS_FUNC            23
#define SPI1_SCK_FUNC            24
#define SYS_JTCK_SWCLK_FUNC      25
#define SYS_JTDI_FUNC            26
#define SYS_JTDO_SWO_FUNC        27
#define SYS_JTMS_SWDIO_FUNC      28
#define SYS_JTRST_FUNC           29
#define TIM1_BKIN_FUNC           30
#define TIM1_BKIN2_FUNC          31
#define TIM1_CH1_FUNC            32
#define TIM1_CH1N_FUNC           33
#define TIM1_CH2_FUNC            34
#define TIM1_CH2N_FUNC           35
#define TIM1_CH3_FUNC            36
#define TIM1_CH3N_FUNC           37
#define TIM1_CH4_FUNC            38
#define TIM1_ETR_FUNC            39
#define TIM2_CH1_FUNC            40
#define TIM2_CH2_FUNC            41
#define TIM2_CH3_FUNC            42
#define TIM2_CH4_FUNC            43
#define TIM2_ETR_FUNC            44
#define TSC_G1_IO1_FUNC          45
#define TSC_G1_IO2_FUNC          46
#define TSC_G2_IO1_FUNC          47
#define TSC_G2_IO2_FUNC          48
#define TSC_G2_IO3_FUNC          49
#define TSC_G2_IO4_FUNC          50
#define TSC_G3_IO1_FUNC          51
#define TSC_G3_IO2_FUNC          52
#define TSC_G3_IO3_FUNC          53
#define TSC_G7_IO1_FUNC          54
#define TSC_G7_IO2_FUNC          55
#define TSC_G7_IO3_FUNC          56
#define TSC_G7_IO4_FUNC          57
#define USART1_CK_FUNC           58
#define UART1_CK_FUNC            58
#define USART1_CTS_FUNC          59
#define UART1_CTS_FUNC           59
#define USART1_DE_FUNC           60
#define UART1_DE_FUNC            60
#define USART1_NSS_FUNC          61
#define UART1_NSS_FUNC           61
#define USART1_RTS_FUNC          62
#define UART1_RTS_FUNC           62
#define USART1_RX_FUNC           63
#define UART1_RX_FUNC            63
#define USART1_TX_FUNC           64
#define UART1_TX_FUNC            64

# define PINMUX(gpio, func) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA00) && (func == COMP1_OUT_FUNC           )) ? 12 : \
	((gpio == PA00) && (func == TIM2_ETR_FUNC            )) ? 14 : \
	((gpio == PA00) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA01) && (func == TIM2_CH2_FUNC            )) ?  1 : \
	((gpio == PA01) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PA01) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PA01) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA02) && (func == RCC_LSCO_FUNC            )) ?  0 : \
	((gpio == PA02) && (func == TIM2_CH3_FUNC            )) ?  1 : \
	((gpio == PA02) && (func == LPUART1_TX_FUNC          )) ?  8 : \
	((gpio == PA02) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA03) && (func == TIM2_CH4_FUNC            )) ?  1 : \
	((gpio == PA03) && (func == LPUART1_RX_FUNC          )) ?  8 : \
	((gpio == PA03) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA04) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PA04) && (func == LPTIM2_OUT_FUNC          )) ? 14 : \
	((gpio == PA04) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA05) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA05) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA05) && (func == SPI1_MOSI_FUNC           )) ?  4 : \
	((gpio == PA05) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PA05) && (func == LPTIM2_ETR_FUNC          )) ? 14 : \
	((gpio == PA05) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA06) && (func == TIM1_BKIN_FUNC           )) ?  1 : \
	((gpio == PA06) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PA06) && (func == LPUART1_CTS_FUNC         )) ?  8 : \
	((gpio == PA06) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA07) && (func == TIM1_CH1N_FUNC           )) ?  1 : \
	((gpio == PA07) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PA07) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA08) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PA08) && (func == TIM1_CH1_FUNC            )) ?  1 : \
	((gpio == PA08) && (func == USART1_CK_FUNC           )) ?  7 : \
	((gpio == PA08) && (func == LPTIM2_OUT_FUNC          )) ? 14 : \
	((gpio == PA08) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA09) && (func == TIM1_CH2_FUNC            )) ?  1 : \
	((gpio == PA09) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PA09) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PA09) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA10) && (func == TIM1_CH3_FUNC            )) ?  1 : \
	((gpio == PA10) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PA10) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PA10) && (func == TSC_G7_IO2_FUNC          )) ?  9 : \
	((gpio == PA10) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA11) && (func == TIM1_CH4_FUNC            )) ?  1 : \
	((gpio == PA11) && (func == TIM1_BKIN2_FUNC          )) ?  2 : \
	((gpio == PA11) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PA11) && (func == USART1_CTS_FUNC          )) ?  7 : \
	((gpio == PA11) && (func == USART1_NSS_FUNC          )) ?  7 : \
	((gpio == PA11) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA12) && (func == TIM1_ETR_FUNC            )) ?  1 : \
	((gpio == PA12) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PA12) && (func == USART1_DE_FUNC           )) ?  7 : \
	((gpio == PA12) && (func == USART1_RTS_FUNC          )) ?  7 : \
	((gpio == PA12) && (func == LPUART1_RX_FUNC          )) ?  8 : \
	((gpio == PA12) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA13) && (func == SYS_JTMS_SWDIO_FUNC      )) ?  0 : \
	((gpio == PA13) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PA13) && (func == TSC_G7_IO1_FUNC          )) ?  9 : \
	((gpio == PA13) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA14) && (func == SYS_JTCK_SWCLK_FUNC      )) ?  0 : \
	((gpio == PA14) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PA14) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PA14) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PA14) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA15) && (func == SYS_JTDI_FUNC            )) ?  0 : \
	((gpio == PA15) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA15) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA15) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PA15) && (func == RCC_MCO_FUNC             )) ?  6 : \
	((gpio == PA15) && (func == TSC_G3_IO1_FUNC          )) ?  9 : \
	((gpio == PA15) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB00) && (func == RF_TX_MOD_EXT_PA_FUNC    )) ?  6 : \
	((gpio == PB00) && (func == COMP1_OUT_FUNC           )) ? 12 : \
	((gpio == PB00) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB01) && (func == LPUART1_DE_FUNC          )) ?  8 : \
	((gpio == PB01) && (func == LPUART1_RTS_FUNC         )) ?  8 : \
	((gpio == PB01) && (func == LPTIM2_IN1_FUNC          )) ? 14 : \
	((gpio == PB01) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB02) && (func == RTC_OUT_FUNC             )) ?  0 : \
	((gpio == PB02) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PB02) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PB02) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB03) && (func == SYS_JTDO_SWO_FUNC        )) ?  0 : \
	((gpio == PB03) && (func == TIM2_CH2_FUNC            )) ?  1 : \
	((gpio == PB03) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PB03) && (func == USART1_DE_FUNC           )) ?  7 : \
	((gpio == PB03) && (func == USART1_RTS_FUNC          )) ?  7 : \
	((gpio == PB03) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB04) && (func == SYS_JTRST_FUNC           )) ?  0 : \
	((gpio == PB04) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PB04) && (func == USART1_CTS_FUNC          )) ?  7 : \
	((gpio == PB04) && (func == USART1_NSS_FUNC          )) ?  7 : \
	((gpio == PB04) && (func == TSC_G2_IO1_FUNC          )) ?  9 : \
	((gpio == PB04) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB05) && (func == LPTIM1_IN1_FUNC          )) ?  1 : \
	((gpio == PB05) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PB05) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PB05) && (func == USART1_CK_FUNC           )) ?  7 : \
	((gpio == PB05) && (func == LPUART1_TX_FUNC          )) ?  8 : \
	((gpio == PB05) && (func == TSC_G2_IO2_FUNC          )) ?  9 : \
	((gpio == PB05) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB06) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PB06) && (func == LPTIM1_ETR_FUNC          )) ?  1 : \
	((gpio == PB06) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB06) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PB06) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PB06) && (func == TSC_G2_IO3_FUNC          )) ?  9 : \
	((gpio == PB06) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB07) && (func == LPTIM1_IN2_FUNC          )) ?  1 : \
	((gpio == PB07) && (func == TIM1_BKIN_FUNC           )) ?  3 : \
	((gpio == PB07) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB07) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PB07) && (func == TSC_G2_IO4_FUNC          )) ?  9 : \
	((gpio == PB07) && (func == TIM1_CH3_FUNC            )) ? 12 : \
	((gpio == PB07) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB08) && (func == TIM1_CH2N_FUNC           )) ?  1 : \
	((gpio == PB08) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB08) && (func == TSC_G7_IO3_FUNC          )) ?  9 : \
	((gpio == PB08) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB09) && (func == TIM1_CH3N_FUNC           )) ?  1 : \
	((gpio == PB09) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB09) && (func == TSC_G7_IO4_FUNC          )) ?  9 : \
	((gpio == PB09) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB10) && (func == TSC_G3_IO2_FUNC          )) ?  9 : \
	((gpio == PB10) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB12) && (func == TIM2_CH2_FUNC            )) ?  1 : \
	((gpio == PB12) && (func == TSC_G1_IO1_FUNC          )) ?  9 : \
	((gpio == PB12) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB13) && (func == TIM2_CH3_FUNC            )) ?  1 : \
	((gpio == PB13) && (func == TSC_G1_IO2_FUNC          )) ?  9 : \
	((gpio == PB13) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB14) && (func == TIM1_CH1_FUNC            )) ?  1 : \
	((gpio == PB14) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB15) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PB15) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC01) && (func == TSC_G3_IO3_FUNC          )) ?  9 : \
	((gpio == PC01) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC14) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PC15) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PE04) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PH03) && (func == RCC_LSCO_FUNC            )) ?  0 : \
	((gpio == PH03) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	AF_NO_SUCH_PINFUNC)

# define TIMERMUX(gpio, timer_nr, channel_nr) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA01) && (timer_nr ==  2) && (channel_nr ==  2)) ?  1 : \
	((gpio == PA02) && (timer_nr ==  2) && (channel_nr ==  3)) ?  1 : \
	((gpio == PA03) && (timer_nr ==  2) && (channel_nr ==  4)) ?  1 : \
	((gpio == PA05) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA07) && (timer_nr ==  1) && (channel_nr == -1)) ?  1 : \
	((gpio == PA08) && (timer_nr ==  1) && (channel_nr ==  1)) ?  1 : \
	((gpio == PA09) && (timer_nr ==  1) && (channel_nr ==  2)) ?  1 : \
	((gpio == PA10) && (timer_nr ==  1) && (channel_nr ==  3)) ?  1 : \
	((gpio == PA11) && (timer_nr ==  1) && (channel_nr ==  4)) ?  1 : \
	((gpio == PA15) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	((gpio == PB03) && (timer_nr ==  2) && (channel_nr ==  2)) ?  1 : \
	((gpio == PB07) && (timer_nr ==  1) && (channel_nr ==  3)) ? 12 : \
	((gpio == PB08) && (timer_nr ==  1) && (channel_nr == -2)) ?  1 : \
	((gpio == PB09) && (timer_nr ==  1) && (channel_nr == -3)) ?  1 : \
	((gpio == PB12) && (timer_nr ==  2) && (channel_nr ==  2)) ?  1 : \
	((gpio == PB13) && (timer_nr ==  2) && (channel_nr ==  3)) ?  1 : \
	((gpio == PB14) && (timer_nr ==  1) && (channel_nr ==  1)) ?  1 : \
	((gpio == PB15) && (timer_nr ==  2) && (channel_nr ==  1)) ?  1 : \
	AF_NO_SUCH_PINFUNC)
#endif
