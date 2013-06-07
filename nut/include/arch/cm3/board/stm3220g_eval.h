/*
 * Copyright (c) 2013 proconX Pty Ltd <www.proconX.com>
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

/*
 * \file arch/cm3/board/stm3230g_eval.h
 * \brief STM3220G-EVAL board specific settings.
 */

#ifndef _DEV_BOARD_H_
#error "Do not include this file directly. Use dev/board.h instead!"
#endif


/*
 * Sertial devices
 */
#ifndef DEV_DEBUG
#define DEV_DEBUG       devUsartStm32_3
#endif
#ifndef DEV_DEBUG_NAME
#define DEV_DEBUG_NAME  devUsartStm32_3.dev_name
#endif

#ifndef DEV_UART
#define DEV_UART        devUsartStm32_3
#endif
#ifndef DEV_UART_NAME
#define DEV_UART_NAME   devUsartStm32_3.dev_name
#endif


/*
 * CAN devices
 */
#define CAN1_RX_PIN          0
#define CAN1_TX_PIN          1
#define CAN2_RX_PIN          5
#define CAN2_TX_PIN          13


/*
 * Ethernet devices, incl. board specific settings for alternate function pins
 */
#include <dev/stm32_emac.h>

//#define EMAC_USE_RMII_MODE /* requires 50 MHz oscillator mounted on STM3230G_EVAL board */

#ifndef EMAC_PHY_CLOCK_MCO
#define EMAC_PHY_CLOCK_MCO
#endif

#ifndef EMAC_CRS_PORT
#define EMAC_CRS_PORT        NUTGPIO_PORTH
#define EMAC_CRS_PIN         2
#endif

#ifndef EMAC_COL_PORT
#define EMAC_COL_PORT        NUTGPIO_PORTH
#endif

#ifndef EMAC_RXD2_PORT
#define EMAC_RXD2_PORT       NUTGPIO_PORTH
#define EMAC_RXD2_PIN        6
#endif

#ifndef EMAC_RXD3_PORT
#define EMAC_RXD3_PORT       NUTGPIO_PORTH
#define EMAC_RXD3_PIN        7
#endif

#ifndef EMAC_RX_ER_PORT
#define EMAC_RX_ER_PORT      NUTGPIO_PORTI
#endif

#ifndef EMAC_TXEN_PORT
#define EMAC_TXEN_PORT       NUTGPIO_PORTG
#endif

#ifndef EMAC_TXD0_PORT
#define EMAC_TXD0_PORT       NUTGPIO_PORTG
#define EMAC_TXD0_PIN        13
#endif

#ifndef EMAC_TXD1_PORT
#define EMAC_TXD1_PORT       NUTGPIO_PORTG
#define EMAC_TXD1_PIN        14
#endif

