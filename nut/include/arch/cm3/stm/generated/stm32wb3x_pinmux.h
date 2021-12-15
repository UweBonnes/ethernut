/* Do not edit!
 * Generated from /devel/cubemx/db/mcu/IPGPIO-STM32WB3x_gpio_v1_0_Modes.xml*/

#if !defined(_STM32WB3x_PINMUX_H_)
# define _STM32WB3x_PINMUX_H_

#define CM4_EVENTOUT_FUNC        0
#define COMP1_OUT_FUNC           1
#define COMP2_OUT_FUNC           2
#define CRS_SYNC_FUNC            3
#define EXT_PA_TX_FUNC           4
#define I2C1_SCL_FUNC            5
#define I2C1_SDA_FUNC            6
#define I2C1_SMBA_FUNC           7
#define I2C3_SCL_FUNC            8
#define I2C3_SDA_FUNC            9
#define I2C3_SMBA_FUNC           10
#define IR_OUT_FUNC              11
#define LPTIM1_ETR_FUNC          12
#define LPTIM1_IN1_FUNC          13
#define LPTIM1_IN2_FUNC          14
#define LPTIM1_OUT_FUNC          15
#define LPTIM2_ETR_FUNC          16
#define LPTIM2_IN1_FUNC          17
#define LPTIM2_OUT_FUNC          18
#define LPUART1_CTS_FUNC         19
#define LPUART1_DE_FUNC          20
#define LPUART1_RTS_FUNC         21
#define LPUART1_RX_FUNC          22
#define LPUART1_TX_FUNC          23
#define QUADSPI_BK1_IO0_FUNC     24
#define QUADSPI_BK1_IO1_FUNC     25
#define QUADSPI_BK1_IO2_FUNC     26
#define QUADSPI_BK1_IO3_FUNC     27
#define QUADSPI_BK1_NCS_FUNC     28
#define QUADSPI_CLK_FUNC         29
#define RCC_LSCO_FUNC            30
#define RCC_MCO_FUNC             31
#define SAI1_CK1_FUNC            32
#define SAI1_CK2_FUNC            33
#define SAI1_D1_FUNC             34
#define SAI1_D2_FUNC             35
#define SAI1_DI1_FUNC            36
#define SAI1_DI2_FUNC            37
#define SAI1_EXTCLK_FUNC         38
#define SAI1_FS_A_FUNC           39
#define SAI1_FS_B_FUNC           40
#define SAI1_MCLK_A_FUNC         41
#define SAI1_MCLK_B_FUNC         42
#define SAI1_SCK_A_FUNC          43
#define SAI1_SCK_B_FUNC          44
#define SAI1_SD_A_FUNC           45
#define SAI1_SD_B_FUNC           46
#define SPI1_MISO_FUNC           47
#define SPI1_MOSI_FUNC           48
#define SPI1_NSS_FUNC            49
#define SPI1_SCK_FUNC            50
#define SYS_JTCK_SWCLK_FUNC      51
#define SYS_JTDI_FUNC            52
#define SYS_JTDO_SWO_FUNC        53
#define SYS_JTMS_SWDIO_FUNC      54
#define SYS_JTRST_FUNC           55
#define TIM16_BKIN_FUNC          56
#define TIM16_CH1_FUNC           57
#define TIM16_CH1N_FUNC          58
#define TIM17_BKIN_FUNC          59
#define TIM17_CH1_FUNC           60
#define TIM17_CH1N_FUNC          61
#define TIM1_BKIN_FUNC           62
#define TIM1_BKIN2_FUNC          63
#define TIM1_CH1_FUNC            64
#define TIM1_CH1N_FUNC           65
#define TIM1_CH2_FUNC            66
#define TIM1_CH2N_FUNC           67
#define TIM1_CH3_FUNC            68
#define TIM1_CH3N_FUNC           69
#define TIM1_CH4_FUNC            70
#define TIM1_ETR_FUNC            71
#define TIM2_CH1_FUNC            72
#define TIM2_CH2_FUNC            73
#define TIM2_CH3_FUNC            74
#define TIM2_CH4_FUNC            75
#define TIM2_ETR_FUNC            76
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
#define USB_DM_FUNC              84
#define USB_DP_FUNC              85
#define USB_NOE_FUNC             86

# define PINMUX(gpio, func) ((gpio == PIN_NONE) ? AF_PIN_NONE : 	\
	((gpio == PA00) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA00) && (func == COMP1_OUT_FUNC           )) ? 12 : \
	((gpio == PA00) && (func == SAI1_EXTCLK_FUNC         )) ? 13 : \
	((gpio == PA00) && (func == TIM2_ETR_FUNC            )) ? 14 : \
	((gpio == PA00) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA01) && (func == TIM2_CH2_FUNC            )) ?  1 : \
	((gpio == PA01) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PA01) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PA01) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA02) && (func == RCC_LSCO_FUNC            )) ?  0 : \
	((gpio == PA02) && (func == TIM2_CH3_FUNC            )) ?  1 : \
	((gpio == PA02) && (func == LPUART1_TX_FUNC          )) ?  8 : \
	((gpio == PA02) && (func == QUADSPI_BK1_NCS_FUNC     )) ? 10 : \
	((gpio == PA02) && (func == COMP2_OUT_FUNC           )) ? 12 : \
	((gpio == PA02) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA03) && (func == TIM2_CH4_FUNC            )) ?  1 : \
	((gpio == PA03) && (func == SAI1_CK1_FUNC            )) ?  3 : \
	((gpio == PA03) && (func == LPUART1_RX_FUNC          )) ?  8 : \
	((gpio == PA03) && (func == QUADSPI_CLK_FUNC         )) ? 10 : \
	((gpio == PA03) && (func == SAI1_MCLK_A_FUNC         )) ? 13 : \
	((gpio == PA03) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA04) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PA04) && (func == SAI1_FS_B_FUNC           )) ? 13 : \
	((gpio == PA04) && (func == LPTIM2_OUT_FUNC          )) ? 14 : \
	((gpio == PA04) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA05) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA05) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA05) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PA05) && (func == SAI1_SD_B_FUNC           )) ? 13 : \
	((gpio == PA05) && (func == LPTIM2_ETR_FUNC          )) ? 14 : \
	((gpio == PA05) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA06) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PA06) && (func == LPUART1_CTS_FUNC         )) ?  8 : \
	((gpio == PA06) && (func == QUADSPI_BK1_IO3_FUNC     )) ? 10 : \
	((gpio == PA06) && (func == TIM1_BKIN_FUNC           )) ? 12 : \
	((gpio == PA06) && (func == TIM16_CH1_FUNC           )) ? 14 : \
	((gpio == PA06) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA07) && (func == TIM1_CH1N_FUNC           )) ?  1 : \
	((gpio == PA07) && (func == I2C3_SCL_FUNC            )) ?  4 : \
	((gpio == PA07) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PA07) && (func == QUADSPI_BK1_IO2_FUNC     )) ? 10 : \
	((gpio == PA07) && (func == COMP2_OUT_FUNC           )) ? 12 : \
	((gpio == PA07) && (func == TIM17_CH1_FUNC           )) ? 14 : \
	((gpio == PA07) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA08) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PA08) && (func == TIM1_CH1_FUNC            )) ?  1 : \
	((gpio == PA08) && (func == SAI1_CK2_FUNC            )) ?  3 : \
	((gpio == PA08) && (func == USART1_CK_FUNC           )) ?  7 : \
	((gpio == PA08) && (func == SAI1_SCK_A_FUNC          )) ? 13 : \
	((gpio == PA08) && (func == LPTIM2_OUT_FUNC          )) ? 14 : \
	((gpio == PA08) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA09) && (func == TIM1_CH2_FUNC            )) ?  1 : \
	((gpio == PA09) && (func == SAI1_D2_FUNC             )) ?  3 : \
	((gpio == PA09) && (func == SAI1_DI2_FUNC            )) ?  3 : \
	((gpio == PA09) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PA09) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PA09) && (func == SAI1_FS_A_FUNC           )) ? 13 : \
	((gpio == PA09) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA10) && (func == TIM1_CH3_FUNC            )) ?  1 : \
	((gpio == PA10) && (func == SAI1_D1_FUNC             )) ?  3 : \
	((gpio == PA10) && (func == SAI1_DI1_FUNC            )) ?  3 : \
	((gpio == PA10) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PA10) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PA10) && (func == CRS_SYNC_FUNC            )) ? 10 : \
	((gpio == PA10) && (func == SAI1_SD_A_FUNC           )) ? 13 : \
	((gpio == PA10) && (func == TIM17_BKIN_FUNC          )) ? 14 : \
	((gpio == PA10) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA11) && (func == TIM1_CH4_FUNC            )) ?  1 : \
	((gpio == PA11) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PA11) && (func == USART1_CTS_FUNC          )) ?  7 : \
	((gpio == PA11) && (func == USART1_NSS_FUNC          )) ?  7 : \
	((gpio == PA11) && (func == USB_DM_FUNC              )) ? 10 : \
	((gpio == PA11) && (func == TIM1_BKIN2_FUNC          )) ? 12 : \
	((gpio == PA11) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA12) && (func == TIM1_ETR_FUNC            )) ?  1 : \
	((gpio == PA12) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PA12) && (func == USART1_DE_FUNC           )) ?  7 : \
	((gpio == PA12) && (func == USART1_RTS_FUNC          )) ?  7 : \
	((gpio == PA12) && (func == LPUART1_RX_FUNC          )) ?  8 : \
	((gpio == PA12) && (func == USB_DP_FUNC              )) ? 10 : \
	((gpio == PA12) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA13) && (func == SYS_JTMS_SWDIO_FUNC      )) ?  0 : \
	((gpio == PA13) && (func == IR_OUT_FUNC              )) ?  8 : \
	((gpio == PA13) && (func == USB_NOE_FUNC             )) ? 10 : \
	((gpio == PA13) && (func == SAI1_SD_B_FUNC           )) ? 13 : \
	((gpio == PA13) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA14) && (func == SYS_JTCK_SWCLK_FUNC      )) ?  0 : \
	((gpio == PA14) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PA14) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PA14) && (func == SAI1_FS_B_FUNC           )) ? 13 : \
	((gpio == PA14) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PA15) && (func == SYS_JTDI_FUNC            )) ?  0 : \
	((gpio == PA15) && (func == TIM2_CH1_FUNC            )) ?  1 : \
	((gpio == PA15) && (func == TIM2_ETR_FUNC            )) ?  2 : \
	((gpio == PA15) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PA15) && (func == RCC_MCO_FUNC             )) ?  6 : \
	((gpio == PA15) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB00) && (func == EXT_PA_TX_FUNC           )) ?  6 : \
	((gpio == PB00) && (func == COMP1_OUT_FUNC           )) ? 12 : \
	((gpio == PB00) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB01) && (func == LPUART1_DE_FUNC          )) ?  8 : \
	((gpio == PB01) && (func == LPUART1_RTS_FUNC         )) ?  8 : \
	((gpio == PB01) && (func == LPTIM2_IN1_FUNC          )) ? 14 : \
	((gpio == PB01) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB02) && (func == LPTIM1_OUT_FUNC          )) ?  1 : \
	((gpio == PB02) && (func == I2C3_SMBA_FUNC           )) ?  4 : \
	((gpio == PB02) && (func == SPI1_NSS_FUNC            )) ?  5 : \
	((gpio == PB02) && (func == SAI1_EXTCLK_FUNC         )) ? 13 : \
	((gpio == PB02) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB03) && (func == SYS_JTDO_SWO_FUNC        )) ?  0 : \
	((gpio == PB03) && (func == TIM2_CH2_FUNC            )) ?  1 : \
	((gpio == PB03) && (func == SPI1_SCK_FUNC            )) ?  5 : \
	((gpio == PB03) && (func == USART1_DE_FUNC           )) ?  7 : \
	((gpio == PB03) && (func == USART1_RTS_FUNC          )) ?  7 : \
	((gpio == PB03) && (func == SAI1_SCK_B_FUNC          )) ? 13 : \
	((gpio == PB03) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB04) && (func == SYS_JTRST_FUNC           )) ?  0 : \
	((gpio == PB04) && (func == I2C3_SDA_FUNC            )) ?  4 : \
	((gpio == PB04) && (func == SPI1_MISO_FUNC           )) ?  5 : \
	((gpio == PB04) && (func == USART1_CTS_FUNC          )) ?  7 : \
	((gpio == PB04) && (func == USART1_NSS_FUNC          )) ?  7 : \
	((gpio == PB04) && (func == SAI1_MCLK_B_FUNC         )) ? 13 : \
	((gpio == PB04) && (func == TIM17_BKIN_FUNC          )) ? 14 : \
	((gpio == PB04) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB05) && (func == LPTIM1_IN1_FUNC          )) ?  1 : \
	((gpio == PB05) && (func == I2C1_SMBA_FUNC           )) ?  4 : \
	((gpio == PB05) && (func == SPI1_MOSI_FUNC           )) ?  5 : \
	((gpio == PB05) && (func == USART1_CK_FUNC           )) ?  7 : \
	((gpio == PB05) && (func == LPUART1_TX_FUNC          )) ?  8 : \
	((gpio == PB05) && (func == COMP2_OUT_FUNC           )) ? 12 : \
	((gpio == PB05) && (func == SAI1_SD_B_FUNC           )) ? 13 : \
	((gpio == PB05) && (func == TIM16_BKIN_FUNC          )) ? 14 : \
	((gpio == PB05) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB06) && (func == RCC_MCO_FUNC             )) ?  0 : \
	((gpio == PB06) && (func == LPTIM1_ETR_FUNC          )) ?  1 : \
	((gpio == PB06) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB06) && (func == USART1_TX_FUNC           )) ?  7 : \
	((gpio == PB06) && (func == SAI1_FS_B_FUNC           )) ? 13 : \
	((gpio == PB06) && (func == TIM16_CH1N_FUNC          )) ? 14 : \
	((gpio == PB06) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB07) && (func == LPTIM1_IN2_FUNC          )) ?  1 : \
	((gpio == PB07) && (func == TIM1_BKIN_FUNC           )) ?  3 : \
	((gpio == PB07) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB07) && (func == USART1_RX_FUNC           )) ?  7 : \
	((gpio == PB07) && (func == TIM17_CH1N_FUNC          )) ? 14 : \
	((gpio == PB07) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB08) && (func == TIM1_CH2N_FUNC           )) ?  1 : \
	((gpio == PB08) && (func == SAI1_CK1_FUNC            )) ?  3 : \
	((gpio == PB08) && (func == I2C1_SCL_FUNC            )) ?  4 : \
	((gpio == PB08) && (func == QUADSPI_BK1_IO1_FUNC     )) ? 10 : \
	((gpio == PB08) && (func == SAI1_MCLK_A_FUNC         )) ? 13 : \
	((gpio == PB08) && (func == TIM16_CH1_FUNC           )) ? 14 : \
	((gpio == PB08) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
	((gpio == PB09) && (func == TIM1_CH3N_FUNC           )) ?  1 : \
	((gpio == PB09) && (func == SAI1_D2_FUNC             )) ?  3 : \
	((gpio == PB09) && (func == SAI1_DI2_FUNC            )) ?  3 : \
	((gpio == PB09) && (func == I2C1_SDA_FUNC            )) ?  4 : \
	((gpio == PB09) && (func == IR_OUT_FUNC              )) ?  8 : \
	((gpio == PB09) && (func == QUADSPI_BK1_IO0_FUNC     )) ? 10 : \
	((gpio == PB09) && (func == SAI1_FS_A_FUNC           )) ? 13 : \
	((gpio == PB09) && (func == TIM17_CH1_FUNC           )) ? 14 : \
	((gpio == PB09) && (func == CM4_EVENTOUT_FUNC        )) ? 15 : \
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
	AF_NO_SUCH_PINFUNC)
#endif
