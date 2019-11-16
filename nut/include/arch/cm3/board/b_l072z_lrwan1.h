/*
 * Copyright 2019 by Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
 *
 * All rights reserved.
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
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

#ifndef LED1_PORT
# define LED1_PORT NUTGPIO_PORTB
#endif
#ifndef LED1_PIN
# define LED1_PIN  5
#endif
#ifndef  LED2_PORT
# define LED2_PORT NUTGPIO_PORTA
#endif
#ifndef LED2_PIN
# define LED2_PIN  5
#endif
#ifndef  LED3_PORT
# define LED3_PORT NUTGPIO_PORTB
#endif
#ifndef LED3_PIN
# define LED3_PIN  6
#endif
#ifndef  LED4_PORT
# define LED4_PORT NUTGPIO_PORTB
#endif
#ifndef LED4_PIN
# define LED4_PIN  7
#endif

#ifndef SW1_PORT
# define SW1_PORT  NUTGPIO_PORTB
#endif
#ifndef SW1_PIN
# define SW1_PIN   2
#endif

#ifndef DEV_CONSOLE
# define DEV_CONSOLE devUsartStm32_2
#endif
#ifndef DEV_CONSOLE_NAME
# define DEV_CONSOLE_NAME devUsartStm32_2.dev_name
#endif

#ifndef SPI_SX1276
# include <dev/spibus_stm32.h>
# define SPI_SX1276 spiBus1Stm32Cb
#endif

/*These definitions should go into cfg/sx1276.h*/

#ifndef RADIO_RESET_PORT
# define RADIO_RESET_PORT NUTGPIO_PORTC
#endif
#ifndef RADIO_RESET_PIN
# define RADIO_RESET_PIN 0
#endif

#ifndef RADIO_DIO_0_PORT
# define RADIO_DIO_0_PORT NUTGPIO_PORTB
#endif
#ifndef  RADIO_DIO_0_PIN
# define  RADIO_DIO_0_PIN 4
#endif

#ifndef RADIO_DIO_1_PORT
# define RADIO_DIO_1_PORT NUTGPIO_PORTB
#endif
#ifndef  RADIO_DIO_1_PIN
# define  RADIO_DIO_1_PIN 1
#endif

#ifndef RADIO_DIO_2_PORT
# define RADIO_DIO_2_PORT NUTGPIO_PORTB
#endif
#ifndef  RADIO_DIO_2_PIN
# define  RADIO_DIO_2_PIN 0
#endif

#ifndef RADIO_DIO_3_PORT
# define RADIO_DIO_3_PORT NUTGPIO_PORTC
#endif
#ifndef  RADIO_DIO_3_PIN
# define  RADIO_DIO_3_PIN 13
#endif

#ifndef RADIO_DIO_4_PORT
# define RADIO_DIO_4_PORT NUTGPIO_PORTA
#endif
#ifndef  RADIO_DIO_4_PIN
# define  RADIO_DIO_4_PIN 5
#endif

#ifndef RADIO_DIO_5_PORT
# define RADIO_DIO_5_PORT NUTGPIO_PORTA
#endif
#ifndef  RADIO_DIO_5_PIN
# define  RADIO_DIO_5_PIN 4
#endif

#ifndef RADIO_TCXO_POWER_PORT
# define RADIO_TCXO_POWER_PORT NUTGPIO_PORTA
#endif
#ifndef RADIO_TCXO_POWER_PIN
# define RADIO_TCXO_POWER_PIN 12
#endif

#ifndef RADIO_ANT_SWITCH_RX_PORT
# define RADIO_ANT_SWITCH_RX_PORT NUTGPIO_PORTA
#endif
#ifndef RADIO_ANT_SWITCH_RX_PIN
# define RADIO_ANT_SWITCH_RX_PIN 1
#endif

#ifndef  RADIO_ANT_SWITCH_TX_BOOST_PORT
# define  RADIO_ANT_SWITCH_TX_BOOST_PORT NUTGPIO_PORTC
#endif
#ifndef  RADIO_ANT_SWITCH_TX_BOOST_PIN
# define  RADIO_ANT_SWITCH_TX_BOOST_PIN 1
#endif

#ifndef RADIO_ANT_SWITCH_TX_RFO_PORT
# define RADIO_ANT_SWITCH_TX_RFO_PORT NUTGPIO_PORTC
#endif
#ifndef RADIO_ANT_SWITCH_TX_RFO_PIN
# define RADIO_ANT_SWITCH_TX_RFO_PIN 1
#endif

#ifndef RADIO_DBG_PIN_TX_PORT
# define RADIO_DBG_PIN_TX_PORT NUTGPIO_PORTB
#endif
#ifndef RADIO_DBG_PIN_TX_PIN
# define RADIO_DBG_PIN_TX_PIN 13
#endif

#ifndef RADIO_DBG_PIN_RX_PORT
# define RADIO_DBG_PIN_RX_PORT NUTGPIO_PORTB
#endif
#ifndef RADIO_DBG_PIN_XX_PIN
# define RADIO_DBG_PIN_RX_PIN 1
#endif

#define BOARD_TCXO_WAKEUP_TIME 5
#define MIN_ALARM_DELAY 3
