/* Do not edit!
 * Generated from /opt/stm/STM32CubeMX/db/mcu/IPGPIO-STM32L021_gpio_v1_0_Modes.xml*/

#if !defined(_STM32L021_PINMUX_H_)
# define _STM32L021_PINMUX_H_

#define COMP1_OUT_FUNC           0
#define COMP2_OUT_FUNC           1
#define EVENTOUT_FUNC            2
#define I2C1_SCL_FUNC            3
#define I2C1_SDA_FUNC            4
#define I2C1_SMBA_FUNC           5
#define LPTIM1_ETR_FUNC          6
#define LPTIM1_IN1_FUNC          7
#define LPTIM1_IN2_FUNC          8
#define LPTIM1_OUT_FUNC          9
#define LPUART1_CTS_FUNC         10
#define LPUART1_DE_FUNC          11
#define LPUART1_RTS_FUNC         12
#define LPUART1_RX_FUNC          13
#define LPUART1_TX_FUNC          14
#define RCC_MCO_FUNC             15
#define RTC_REFIN_FUNC           16
#define SPI1_MISO_FUNC           17
#define SPI1_MOSI_FUNC           18
#define SPI1_NSS_FUNC            19
#define SPI1_SCK_FUNC            20
#define SYS_SWCLK_FUNC           21
#define SYS_SWDIO_FUNC           22
#define TIM21_CH1_FUNC           23
#define TIM21_CH2_FUNC           24
#define TIM21_ETR_FUNC           25
#define TIM2_CH1_FUNC            26
#define TIM2_CH2_FUNC            27
#define TIM2_CH3_FUNC            28
#define TIM2_CH4_FUNC            29
#define TIM2_ETR_FUNC            30
#define USART2_CK_FUNC           31
#define UART2_CK_FUNC            31
#define USART2_CTS_FUNC          32
#define UART2_CTS_FUNC           32
#define USART2_DE_FUNC           33
#define UART2_DE_FUNC            33
#define USART2_RTS_FUNC          34
#define UART2_RTS_FUNC           34
#define USART2_RX_FUNC           35
#define UART2_RX_FUNC            35
#define USART2_TX_FUNC           36
#define UART2_TX_FUNC            36

# define PINMUX(gpio, func) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (func == USART2_RX_FUNC           )) ?  0 : \
	((gpio == PA00) && (func == LPTIM1_IN1_FUNC          )) ?  1 : \
	((gpio == PA00) && (func == TIM2_CH1_FUNC            )) ?  2 : \
	((gpio == PA00) && (func == USART2_CTS_FUNC          )) ?  4 : \
	((gpio == PA00) && (func == TIM2_ETR_FUNC            )) ?  5 : \
	((gpio == PA00) && (func == LPUART1_RX_FUNC          )) ?  6 : \
	((gpio == PA00) && (func == COMP1_OUT_FUNC           )) ?  7 : \
	((gpio == PA01) && (func == EVENTOUT_FUNC            )) ?  0 : \
	((gpio == PA01) && (func == LPTIM1_IN2_FUNC          )) ?  1 : \
	((gpio == PA01) && (func == TIM2_CH2_FUNC            )) ?  2 : \
	((gpio == PA01) && (func == I2C1_SMBA_FUNC           )) ?  3 : \
	((gpio == PA01) && (func == USART2_DE_FUNC           )) ?  4 : \
	((gpio == PA01) && (func == USART2_RTS_FUNC          )) ?  4 : \
	((gpio == PA01) && (func == TIM21_ETR_FUNC           )) ?  5 : \
	((gpio == PA01) && (func == LPUART1_TX_FUNC          )) ?  6 : \
	((gpio == PA02) && (func == TIM21_CH1_FUNC           )) ?  0 : \
	((gpio == PA02) && (func == TIM2_CH3_FUNC            )) ?  2 : \
	((gpio == PA02) && (func == USART2_TX_FUNC           )) ?  4 : \
	((gpio == PA02) && (func == LPUART1_TX_FUNC          )) ?  6 : \
	((gpio == PA02) && (func == COMP2_OUT_FUNC           )) ?  7 : \
	((gpio == PA03) && (func == TIM21_CH2_FUNC           )) ?  0 : \
	((gpio == PA03) && (func == TIM2_CH4_FUNC            )) ?  2 : \
	((gpio == PA03) && (func == USART2_RX_FUNC           )) ?  4 : \
	((gpio == PA03) && (func == LPUART1_RX_FUNC          )) ?  6 : \
	((gpio == PA04) && (func == SPI1_NSS_FUNC            )) ?  0 : \
	((gpio == PA04) && (func == LPTIM1_IN1_FUNC          )) ?  1 : \
	((gpio == PA04) && (func == LPTIM1_ETR_FUNC          )) ?  2 : \
	((gpio == PA04) && (func == I2C1_SCL_FUNC            )) ?  3 : \
	((gpio == PA04) && (func == USART2_CK_FUNC           )) ?  4 : \
	((gpio == PA04) && (func == TIM2_ETR_FUNC            )) ?  5 : \
	((gpio == PA04) && (func == LPUART1_TX_FUNC          )) ?  6 : \
	((gpio == PA04) && (func == COMP2_OUT_FUNC           )) ?  7 : \
	((gpio == PA05) && (func == SPI1_SCK_FUNC            )) ?  0 : \
	((gpio == PA05) && (func == LPTIM1_IN2_FUNC          )) ?  1 : \
	((gpio == PA05) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA05) && (func == TIM2_CH1_FUNC            )) ?  5 : \
	((gpio == PA06) && (func == SPI1_MISO_FUNC           )) ?  0 : \
	((gpio == PA06) && (func == LPTIM1_ETR_FUNC          )) ?  1 : \
	((gpio == PA06) && (func == LPUART1_CTS_FUNC         )) ?  4 : \
	((gpio == PA06) && (func == EVENTOUT_FUNC            )) ?  6 : \
	((gpio == PA06) && (func == COMP1_OUT_FUNC           )) ?  7 : \
	((gpio == PA07) && (func == SPI1_MOSI_FUNC           )) ?  0 : \
	((gpio == PA07) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PA07) && (func == USART2_CTS_FUNC          )) ?  4 : \
	((gpio == PA07) && (func == TIM21_ETR_FUNC           )) ?  5 : \
	((gpio == PA07) && (func == EVENTOUT_FUNC            )) ?  6 : \
	((gpio == PA07) && (func == COMP2_OUT_FUNC           )) ?  7 : \
	((gpio == PA08) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PA08) && (func == LPTIM1_IN1_FUNC          )) ?  2 : \
	((gpio == PA08) && (func == EVENTOUT_FUNC            )) ?  3 : \
	((gpio == PA08) && (func == USART2_CK_FUNC           )) ?  4 : \
	((gpio == PA08) && (func == TIM2_CH1_FUNC            )) ?  5 : \
	((gpio == PA09) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PA09) && (func == I2C1_SCL_FUNC            )) ?  1 : \
	((gpio == PA09) && (func == LPTIM1_OUT_FUNC          )) ?  2 : \
	((gpio == PA09) && (func == USART2_TX_FUNC           )) ?  4 : \
	((gpio == PA09) && (func == TIM21_CH2_FUNC           )) ?  5 : \
	((gpio == PA09) && (func == COMP1_OUT_FUNC           )) ?  7 : \
	((gpio == PA10) && (func == TIM21_CH1_FUNC           )) ?  0 : \
	((gpio == PA10) && (func == I2C1_SDA_FUNC            )) ?  1 : \
	((gpio == PA10) && (func == RTC_REFIN_FUNC           )) ?  2 : \
	((gpio == PA10) && (func == USART2_RX_FUNC           )) ?  4 : \
	((gpio == PA10) && (func == TIM2_CH3_FUNC            )) ?  5 : \
	((gpio == PA10) && (func == COMP1_OUT_FUNC           )) ?  7 : \
	((gpio == PA11) && (func == SPI1_MISO_FUNC           )) ?  0 : \
	((gpio == PA11) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PA11) && (func == EVENTOUT_FUNC            )) ?  2 : \
	((gpio == PA11) && (func == USART2_CTS_FUNC          )) ?  4 : \
	((gpio == PA11) && (func == TIM21_CH2_FUNC           )) ?  5 : \
	((gpio == PA11) && (func == COMP1_OUT_FUNC           )) ?  7 : \
	((gpio == PA12) && (func == SPI1_MOSI_FUNC           )) ?  0 : \
	((gpio == PA12) && (func == EVENTOUT_FUNC            )) ?  2 : \
	((gpio == PA12) && (func == USART2_DE_FUNC           )) ?  4 : \
	((gpio == PA12) && (func == USART2_RTS_FUNC          )) ?  4 : \
	((gpio == PA12) && (func == COMP2_OUT_FUNC           )) ?  7 : \
	((gpio == PA13) && (func == SYS_SWDIO_FUNC           )) ?  0 : \
	((gpio == PA13) && (func == LPTIM1_ETR_FUNC          )) ?  1 : \
	((gpio == PA13) && (func == I2C1_SDA_FUNC            )) ?  3 : \
	((gpio == PA13) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PA13) && (func == LPUART1_RX_FUNC          )) ?  6 : \
	((gpio == PA13) && (func == COMP1_OUT_FUNC           )) ?  7 : \
	((gpio == PA14) && (func == SYS_SWCLK_FUNC           )) ?  0 : \
	((gpio == PA14) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PA14) && (func == I2C1_SMBA_FUNC           )) ?  3 : \
	((gpio == PA14) && (func == USART2_TX_FUNC           )) ?  4 : \
	((gpio == PA14) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PA14) && (func == LPUART1_TX_FUNC          )) ?  6 : \
	((gpio == PA14) && (func == COMP2_OUT_FUNC           )) ?  7 : \
	((gpio == PA15) && (func == SPI1_NSS_FUNC            )) ?  0 : \
	((gpio == PA15) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA15) && (func == EVENTOUT_FUNC            )) ?  3 : \
	((gpio == PA15) && (func == USART2_RX_FUNC           )) ?  4 : \
	((gpio == PA15) && (func == TIM2_CH1_FUNC            )) ?  5 : \
	((gpio == PB00) && (func == EVENTOUT_FUNC            )) ?  0 : \
	((gpio == PB00) && (func == SPI1_MISO_FUNC           )) ?  1 : \
	((gpio == PB00) && (func == TIM2_CH2_FUNC            )) ?  2 : \
	((gpio == PB00) && (func == USART2_DE_FUNC           )) ?  4 : \
	((gpio == PB00) && (func == USART2_RTS_FUNC          )) ?  4 : \
	((gpio == PB00) && (func == TIM2_CH3_FUNC            )) ?  5 : \
	((gpio == PB01) && (func == USART2_CK_FUNC           )) ?  0 : \
	((gpio == PB01) && (func == SPI1_MOSI_FUNC           )) ?  1 : \
	((gpio == PB01) && (func == LPTIM1_IN1_FUNC          )) ?  2 : \
	((gpio == PB01) && (func == LPUART1_DE_FUNC          )) ?  4 : \
	((gpio == PB01) && (func == LPUART1_RTS_FUNC         )) ?  4 : \
	((gpio == PB01) && (func == TIM2_CH4_FUNC            )) ?  5 : \
	((gpio == PB02) && (func == LPTIM1_OUT_FUNC          )) ?  2 : \
	((gpio == PB03) && (func == SPI1_SCK_FUNC            )) ?  0 : \
	((gpio == PB03) && (func == TIM2_CH2_FUNC            )) ?  2 : \
	((gpio == PB03) && (func == EVENTOUT_FUNC            )) ?  4 : \
	((gpio == PB04) && (func == SPI1_MISO_FUNC           )) ?  0 : \
	((gpio == PB04) && (func == EVENTOUT_FUNC            )) ?  2 : \
	((gpio == PB05) && (func == SPI1_MOSI_FUNC           )) ?  0 : \
	((gpio == PB05) && (func == LPTIM1_IN1_FUNC          )) ?  2 : \
	((gpio == PB05) && (func == I2C1_SMBA_FUNC           )) ?  3 : \
	((gpio == PB05) && (func == TIM21_CH1_FUNC           )) ?  5 : \
	((gpio == PB06) && (func == USART2_TX_FUNC           )) ?  0 : \
	((gpio == PB06) && (func == I2C1_SCL_FUNC            )) ?  1 : \
	((gpio == PB06) && (func == LPTIM1_ETR_FUNC          )) ?  2 : \
	((gpio == PB06) && (func == TIM2_CH3_FUNC            )) ?  5 : \
	((gpio == PB06) && (func == LPUART1_TX_FUNC          )) ?  6 : \
	((gpio == PB07) && (func == USART2_RX_FUNC           )) ?  0 : \
	((gpio == PB07) && (func == I2C1_SDA_FUNC            )) ?  1 : \
	((gpio == PB07) && (func == LPTIM1_IN2_FUNC          )) ?  2 : \
	((gpio == PB07) && (func == TIM2_CH4_FUNC            )) ?  5 : \
	((gpio == PB07) && (func == LPUART1_RX_FUNC          )) ?  6 : \
	((gpio == PB08) && (func == USART2_TX_FUNC           )) ?  0 : \
	((gpio == PB08) && (func == EVENTOUT_FUNC            )) ?  2 : \
	((gpio == PB08) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB08) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	AF_NO_SUCH_PINFUNC)

# define TIMERMUX(gpio, timer_nr, channel_nr) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (timer_nr ==  2) && (channel_nr ==  1)) ?  2 : \
	((gpio == PA01) && (timer_nr ==  2) && (channel_nr ==  2)) ?  2 : \
	((gpio == PA02) && (timer_nr == 21) && (channel_nr ==  1)) ?  0 : \
	((gpio == PA02) && (timer_nr ==  2) && (channel_nr ==  3)) ?  2 : \
	((gpio == PA03) && (timer_nr == 21) && (channel_nr ==  2)) ?  0 : \
	((gpio == PA03) && (timer_nr ==  2) && (channel_nr ==  4)) ?  2 : \
	((gpio == PA05) && (timer_nr ==  2) && (channel_nr ==  1)) ?  5 : \
	((gpio == PA08) && (timer_nr ==  2) && (channel_nr ==  1)) ?  5 : \
	((gpio == PA09) && (timer_nr == 21) && (channel_nr ==  2)) ?  5 : \
	((gpio == PA10) && (timer_nr == 21) && (channel_nr ==  1)) ?  0 : \
	((gpio == PA10) && (timer_nr ==  2) && (channel_nr ==  3)) ?  5 : \
	((gpio == PA11) && (timer_nr == 21) && (channel_nr ==  2)) ?  5 : \
	((gpio == PA15) && (timer_nr ==  2) && (channel_nr ==  1)) ?  5 : \
	((gpio == PB00) && (timer_nr ==  2) && (channel_nr ==  2)) ?  2 : \
	((gpio == PB00) && (timer_nr ==  2) && (channel_nr ==  3)) ?  5 : \
	((gpio == PB01) && (timer_nr ==  2) && (channel_nr ==  4)) ?  5 : \
	((gpio == PB03) && (timer_nr ==  2) && (channel_nr ==  2)) ?  2 : \
	((gpio == PB05) && (timer_nr == 21) && (channel_nr ==  1)) ?  5 : \
	((gpio == PB06) && (timer_nr ==  2) && (channel_nr ==  3)) ?  5 : \
	((gpio == PB07) && (timer_nr ==  2) && (channel_nr ==  4)) ?  5 : \
	AF_NO_SUCH_PINFUNC)
#endif
