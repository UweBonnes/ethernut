/*
 * Copyright (C) 2017 Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 */
#if !defined(_STM32F1_PINREMAP_H_)
# define _STM32F1_PINREMAP_H_

# define PINMUX(gpio, func) (-1)

# if I2C1_REMAP_I2C == 1
#  define I2C1_SDA PB09
#  define I2C1_SCL PB08
# else
#  define I2C1_SDA PB07
#  define I2C1_SCL PB06
# endif
# if defined(I2C1_USE_SMBA)
#  define I2C1_SMBA PB05
# else
#  define I2C1_SMBA PIN_NONE
# endif
# define I2C2_SDA  PB11
# define I2C2_SCL  PB10
# if defined(I2C2_USE_SMBA)
#  define I2C1_SMBA  PB12
# else
#  define I2C2_SMBA PIN_NONE
# endif

# if SPI1_REMAP == 0
#  define SPI1_SCK  PA05
#  define SPI1_MISO PA06
#  define SPI1_MOSI PA07
# else
#  define SPI1_SCK  PB03
#  define SPI1_MISO PB04
#  define SPI1_MOSI PB05
# endif
# if defined(SPI1_USE_NSS)
#  define SPI1_NSS  ((SPI1_REMAP == 0) ? PA04 : PA15)
# else
#  define SPI1_NSS  PIN_NONE
# endif

# if defined(SPI2_USE_NSS)
#  define SPI2_NSS PB12
# else
#  define SPI2_NSS  PIN_NONE
# endif
# define SPI2_SCK  PB13
# define SPI2_MISO PB14
# define SPI2_MOSI PB15

# if SPI3_REMAP == 0
#  define SPI3_SCK  PB03
#  define SPI3_MISO PB04
#  define SPI3_MOSI PB05
# else
#  define SPI3_SCK  PC10
#  define SPI3_MISO PC11
#  define SPI3_MOSI PC12
# endif
# if defined(SPI3_USE_NSS)
#  define SPI3_NSS  ((SPI3_REMAP) ? PA15 : PA04)
# else
#  define SPI3_NSS  PIN_NONE
# endif

# if defined(USART1_REMAP)
#  define USART1_TX PB06
#  define USART1_RX PB07
# else
#  define USART1_TX PA09
#  define USART1_RX PA10
# endif
# if defined(USART1_USE_CTS)
#  define USART1_CTS PA11
# else
#  define USART1_CTS PIN_NONE
# endif
# if defined(USART1_USE_RTS)
#  define USART1_RTS PA12
# else
#  define USART1_RTS PIN_NONE
# endif
# if defined(USART1_USE_CK)
#  define USART1_CK  PA08
# else
#  define USART1_CK  PIN_NONE
# endif

# if USART2_REMAP == 1
#  define USART2_TX PD05
#  define USART2_RX PD06
# else
#  define USART2_TX PA02
#  define USART2_RX PA03
# endif

# if defined(USART2_USE_CTS)
#  define USART2_CTS ((USART2_REMAP) ? PD03 : PA00)
# else
#  define USART2_CTS PIN_NONE
# endif

# if defined(USART2_USE_RTS)
#  define USART2_RTS ((USART2_REMAP) ? PD04 : PA01)
# else
#  define USART2_RTS PIN_NONE
# endif

# if defined(USART2_USE_CK)
#  define USART2_CK ((USART2_REMAP) ? PD07 : PA04)
# else
#  define USART2_CK PIN_NONE
# endif

# if USART3_REMAP == 0
#  define USART3_TX   PB10
#  define USART3_RX   PB11
# elif USART3_REMAP == 1
#  define USART3_TX   PC10
#  define USART3_RX   PC11
# elif USART3_REMAP == 3
#  define USART3_TX   PD08
#  define USART3_RX   PD09
# endif

# if defined(USART3_USE_CTS)
#  define USART3_CTS ((USART3_REMAP == 3) ? PD11 : PB13)
# else
#  define USART3_CTS   PIN_NONE
# endif

# if defined(USART3_USE_RTS)
#  define USART3_RTS ((USART3_REMAP == 3) ? PD12 : PB14)
# else
#  define USART3_RTS   PIN_NONE
# endif
# if defined(USART3_USE_CK)
# define USART3_CK ((USART3_REMAP == 0)? PB12 :(USART3_REMAP == 1)? PC12:PD10)
# else
#  define USART3_CK   PIN_NONE
# endif

# define USART4_TX   PC10
# define USART4_RX   PC11
# define USART4_CTS  PIN_NONE
# define USART4_RTS  PIN_NONE
# define USART4_CK   PIN_NONE

# define USART5_TX   PC12
# define USART5_RX   PD02
# define USART5_CTS  PIN_NONE
# define USART5_RTS  PIN_NONE
# define USART5_CK   PIN_NONE

# if   (CAN1_REMAP == 3)
#  define CAN1_RX PD00
#  define CAN1_TX PD01
# elif (CAN1_REMAP == 1)
#  define CAN1_RX PB08
#  define CAN1_TX PB09
# else
#  define CAN1_RX PA11
#  define CAN1_TX PA12
# endif
# if   (CAN2_REMAP == 1)
#  define CAN2_RX PB12
#  define CAN2_TX PB13
# else
#  define CAN2_RX PB05
#  define CAN2_TX PB06
# endif
#endif
