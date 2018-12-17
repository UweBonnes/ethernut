/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * Copyright (C) 2010 by Rittal GmbH & Co. KG. All rights reserved.
 * Copyright (C) 2017, 2018 Uwe Bonnes bon@elektron.ikp,physik.tu-darmdstadt.de
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
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <cfg/os.h>
#include <cfg/clock.h>
#include <cfg/arch.h>
#include <cfg/uart.h>
#include <cfg/arch/gpio.h>

#include <string.h>

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <dev/irqreg.h>
#include <dev/gpio.h>
#include <dev/usart.h>

#include PINMUXHEADER
#include <arch/cm3/stm/stm32_uart.h>
#include <arch/cm3/stm/stm32_usart.h>

/*!
 * \addtogroup xgNutArchArmStm32Us
 */
/*@{*/

/*!
 * \brief LPUART1 device control block structure.
 */
static USARTDCB dcb_lpuart1 = {
    0,                          /* dcb_modeflags */
    0,                          /* dcb_statusflags */
    0,                          /* dcb_rtimeout */
    0,                          /* dcb_wtimeout */
    { 0,0,0,0,0,0,0,0,0},       /* dcb_tx_rbf */
    { 0,0,0,0,0,0,0,0,0},       /* dcb_rx_rbf */
    0,                          /* dbc_last_eol */
    Stm32UsartInit,             /* dcb_init */
    Stm32UsartDeinit,           /* dcb_deinit */
    Stm32UsartTxStart,          /* dcb_tx_start */
    Stm32UsartRxStart,          /* dcb_rx_start */
    Stm32UsartSetFlowControl,   /* dcb_set_flow_control */
    Stm32UsartGetFlowControl,   /* dcb_get_flow_control */
    Stm32UsartSetSpeed,         /* dcb_set_speed */
    Stm32UsartGetSpeed,         /* dcb_get_speed */
    Stm32UsartSetDataBits,      /* dcb_set_data_bits */
    Stm32UsartGetDataBits,      /* dcb_get_data_bits */
    Stm32UsartSetParity,        /* dcb_set_parity */
    Stm32UsartGetParity,        /* dcb_get_parity */
    Stm32UsartSetStopBits,      /* dcb_set_stop_bits */
    Stm32UsartGetStopBits,      /* dcb_get_stop_bits */
    Stm32UsartSetStatus,        /* dcb_set_status */
    Stm32UsartGetStatus,        /* dcb_get_status */
    Stm32UsartSetClockMode,     /* dcb_set_clock_mode */
    Stm32UsartGetClockMode,     /* dcb_get_clock_mode */
};

/*!
 * \name STM32 LPUART1 Device
 */
/*@{*/
/*!
 * \brief LPUART1 device information structure.
 *
 * An application must pass a pointer to this structure to
 * NutRegisterDevice() before using the serial communication
 * driver of the STM32 on-chip LPUART1.
 *
 * The device is named \b lpuart1.
 *
 * \showinitializer
 */
NUTDEVICE devLpuartStm32_1 = {
    .dev_name = {'L', 'p', 'u', 'a', 'r', 't', '1', 0, 0},
    .dev_type = IFTYP_CHAR,
    .dev_base = LPUART1_BASE,
    .dev_dcb = &dcb_lpuart1,
    .dev_init = UsartInit,
    .dev_ioctl = UsartIOCtl,
    .dev_read = UsartRead,
    .dev_write = UsartWrite,
    .dev_open = UsartOpen,
    .dev_close = UsartClose,
    .dev_size = UsartSize,
    .dev_select = UsartSelect,                /* Select function, dev_select */
};

#define  Stm32F1UsartRemap()

#define UARTx_TX       LPUART1_TX
#define UARTx_TX_FUNC  LPUART1_TX_FUNC
#define UARTx_RX       LPUART1_RX
#define UARTx_RX_FUNC  LPUART1_RX_FUNC
#define UARTx_CTS      LPUART1_CTS
#define UARTx_CTS_FUNC LPUART1_CTS_FUNC
#define UARTx_RTS      LPUART1_RTS
#define UARTx_RTS_FUNC LPUART1_RTS_FUNC

#ifdef LPUART1_RS485_CTRL
#define LPUART_485_CTRL
#ifdef LPUART1_485DE_INV
#define LPUART_485DE_INV
#endif
#ifdef LPUART1_485RE_INV
#define LPUART_485RE_INV
#endif
#if defined(LPUART1_485DE_PORT) && defined(LPUART1_485DE_PIN)
  #define DE_GPIO_BASE  GPIO_ID2GPIO(LPUART1_485DE_PORT)
  #define DE_GPIO_PORT  LPUART1_485DE_PORT
  #define DE_GPIO_PIN   LPUART1_485DE_PIN
#endif
#if defined(LPUART1_485RE_PORT) && defined(LPUART1_485RE_PIN)
#define NRE_GPIO_BASE   GPIO_ID2GPIO(LPUART1_485RE_PORT)
#define NRE_GPIO_PORT   LPUART1_485RE_PORT
#define NRE_GPIO_PIN    LPUART1_485RE_PIN
#endif
#endif /* LPUART1_RS485_CTRL */

/*!
 * \brief LPUART1 base configuration.
 */
static void  StmUsartClkEnable(int enable)
{
#if defined(RCC_APB1ENR2_LPUART1EN)
    if (enable)
        RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;
    RCC->APB1RSTR2 |= RCC_APB1RSTR2_LPUART1RST;
    RCC->APB1RSTR2 &= ~RCC_APB1RSTR2_LPUART1RST;
    if (!enable)
        RCC->APB1ENR2 &= ~RCC_APB1ENR2_LPUART1EN;
#elif defined(RCC_APB4ENR_LPUART1EN)
    if (enable)
        RCC->APB4ENR |= RCC_APB4ENR_LPUART1EN;
    RCC->APB4RSTR |= RCC_APB4RSTR_LPUART1RST;
    RCC->APB4RSTR &= ~RCC_APB4RSTR_LPUART1RST;
    if (!enable)
        RCC->APB4ENR &= ~RCC_APB4ENR_LPUART1EN;
#else
    if (enable)
        RCC->APB1ENR |= RCC_APB1ENR_LPUART1EN;
    RCC->APB1RSTR |= RCC_APB1RSTR_LPUART1RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_LPUART1RST;
    if (!enable)
        RCC->APB1ENR &= ~RCC_APB1ENR_LPUART1EN;
#endif

}

#ifdef LPUART1_INIT_BAUDRATE
#define USART_INIT_BAUTRATE LPUART1_INIT_BAUDRATE
#endif

#ifdef LPUART1_HARDWARE_HANDSHAKE
  #define USART_HWFLOWCTRL USART_HardwareFlowControl_RTS_CTS
  #define US_MODE_HWHANDSHAKE
#else
  #define USART_HWFLOWCTRL USART_HardwareFlowControl_None
  #undef  US_MODE_HWHANDSHAKE
#endif

#ifdef LPUART1_XONXOFF_CONTROL
#define USART_XONXOFF_CONTROL
#else
#undef  USART_XONXOFF_CONTROL
#endif

#ifdef LPUART1_MODE_IRDA
#define USART_MODE_IRDA
#else
#undef  USART_MODE_IRDA
#endif

#ifdef LPUART1_HARDWARE_HDX
#define USART_HARDWARE_HDX
#else
#undef USART_HARDWARE_HDX
#endif

#ifdef LPUART1_SUPPORT_DMA
# warning "STM32 family has no implemented DMA"
#else
# undef UART_DMA_TXCHANNEL
# undef UART_DMA_RXCHANNEL
#endif

#if defined(LPUART1_TX_PIN_INV)
#define USART_TX_PIN_INV
#else
#undef USART_TX_PIN_INV
#endif
#if defined(LPUART1_RX_PIN_INV)
#define USART_RX_PIN_INV
#else
#undef USART_RX_PIN_INV
#endif

#if defined(LPUART1_SWAP)
#define USART_SWAP
#else
#undef USART_SWAP
#endif

#define USARTidx    16
#define USARTn      LPUART1
#define USARTnBase  LPUART1_BASE
#define USARTclk    BASE2CLKSRC(LPUART1_BASE)
#define USARTnSIG   &sig_LPUART1
#define UART_DR_PTR (uint32_t*)(USARTnBase+4)

#define DcbUSART    dcb_lpuart1

/*@}*/
#include "stm32_usart.c"
