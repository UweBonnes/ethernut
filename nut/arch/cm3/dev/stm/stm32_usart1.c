/*
 * Copyright (C) 2010 by Ulrich Prinz (uprinz2@netscape.net)
 * Copyright (C) 2010 by Rittal GmbH & Co. KG. All rights reserved.
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

#include <arch/cm3/stm/stm32xxxx.h>
#include <arch/cm3/stm/stm32_gpio.h>
#include <arch/cm3/stm/stm32_usart.h>

/*!
 * \addtogroup xgNutArchArmStm32Us
 */
/*@{*/

/*!
 * \brief USART1 device control block structure.
 */
static USARTDCB dcb_usart1 = {
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
 * \name STM32 USART1 Device
 */
/*@{*/
/*!
 * \brief USART1 device information structure.
 *
 * An application must pass a pointer to this structure to
 * NutRegisterDevice() before using the serial communication
 * driver of the AT91's on-chip USART1.
 *
 * The device is named \b uart1.
 *
 * \showinitializer
 */
NUTDEVICE devUsartStm32_1 = {
    0,                          /* Pointer to next device, dev_next. */
    {'u', 's', 'a', 'r', 't', '1', 0, 0, 0},    /* Unique device name, dev_name. */
    IFTYP_CHAR,                 /* Type of device, dev_type. */
    USART1_BASE,                /* Base address, dev_base. */
    USART1_IRQn,                /* First interrupt number, dev_irq. */
    NULL,                       /* Interface control block, dev_icb. */
    &dcb_usart1,                /* Driver control block, dev_dcb. */
    UsartInit,                  /* Driver initialization routine, dev_init. */
    UsartIOCtl,                 /* Driver specific control function, dev_ioctl. */
    UsartRead,                  /* Read from device, dev_read. */
    UsartWrite,                 /* Write to device, dev_write. */
    UsartOpen,                  /* Open a device or file, dev_open. */
    UsartClose,                 /* Close a device or file, dev_close. */
    UsartSize,                  /* Request file size, dev_size. */
    UsartSelect,                /* Select function, dev_select */
};

/*@}*/

/*!
 * \brief USART1 GPIO configuartion and assignment.
 */
/*
 * F1  NOREMAP REMAP  F30  F30  F30  F0
 * CK  PA8     PA8
 * TX  PA9     PB6    PC4  PB6  PE0 PB6
 * RX  PA10    PB7    PC5  PB7  PE1 PB7
 * CTS PA11    PA11
 * RTS PA12    PA12
 *
 * RX on PA14: F0
 * RX on PB3 : F411
 * RX on PC5 : F30
 * RX on PE0 : F30
 * TX on PA15: F0/F411
 * TX on PC4 : F30
 * TX on PE1 : F30
 */
#if defined(MCU_STM32F1)
 #define STM_USART_REMAP_MASK AFIO_MAPR_USART1_REMAP
 #if defined(USART1_REMAP_USART)
  #define STM_USART_REMAP_VALUE AFIO_MAPR_USART1_REMAP
  #define TX_GPIO_PORT    NUTGPIO_PORTB
  #define TX_GPIO_PIN     6
  #define RX_GPIO_PORT    NUTGPIO_PORTB
  #define RX_GPIO_PIN     7
 #else
  #define STM_USART_REMAP_VALUE DISABLE
  #define TX_GPIO_PORT    NUTGPIO_PORTA
  #define TX_GPIO_PIN      9
  #define RX_GPIO_PORT    NUTGPIO_PORTA
  #define RX_GPIO_PIN     10
 #endif
#else /* L1/F2/F3/F4*/
 #if defined(MCU_STM32F3)
  #define STM_USART_REMAP  GPIO_AF_7
 #elif defined(MCU_STM32F0)
  #define STM_USART_REMAP  GPIO_AF_1
/* Fixme: May also be GPIO_AF_0*/
 #else
  #define STM_USART_REMAP  GPIO_AF_USART1
 #endif
 #if !defined(USART1_TX_PIN)
  #if defined(USART1_REMAP_USART)
   #define TX_GPIO_PORT  NUTGPIO_PORTB
   #define TX_GPIO_PIN   6
  #else
   #define TX_GPIO_PORT    NUTGPIO_PORTA
   #define TX_GPIO_PIN      9
  #endif
 #elif USART1_TX_PIN == -1
/* Allow non-used TX PIN*/
 #elif USART1_TX_PIN == 9
  #define TX_GPIO_PORT    NUTGPIO_PORTA
  #define TX_GPIO_PIN     9
 #elif USART1_TX_PIN == 6
  #define TX_GPIO_PORT    NUTGPIO_PORTB
  #define TX_GPIO_PIN     6
 #elif defined(MCU_STM32F3) && USART1_TX_PIN == 4
  #define TX_GPIO_PORT    NUTGPIO_PORTC
  #define TX_GPIO_PIN      4
 #elif defined(MCU_STM32F3) && USART1_TX_PIN == 0
  #define TX_GPIO_PORT    NUTGPIO_PORTE
  #define TX_GPIO_PIN      0
 #elif defined(STM32F411) && USART1_TX_PIN == 15
  #define TX_GPIO_PORT    NUTGPIO_PORTA
  #define TX_GPIO_PIN      15
 #else
  #warning "Illegal USART1 TX pin assignement"
 #endif
 #if !defined(USART1_RX_PIN)
  #if defined(USART1_REMAP_USART)
   #define TX_GPIO_PORT  NUTGPIO_PORTB
   #define TX_GPIO_PIN   7
  #else
   #define RX_GPIO_PORT    NUTGPIO_PORTA
   #define RX_GPIO_PIN      10
 #endif
 #elif USART1_RX_PIN == -1
/* Allow non-used RX PIN*/
 #elif USART1_RX_PIN == 10
  #define RX_GPIO_PORT    NUTGPIO_PORTA
  #define RX_GPIO_PIN     10
 #elif USART1_RX_PIN == 7
  #define RX_GPIO_PORT    NUTGPIO_PORTB
  #define RX_GPIO_PIN     7
 #elif defined(MCU_STM32F3) && USART1_RX_PIN == 5
  #define RX_GPIO_PORT    NUTGPIO_PORTC
  #define RX_GPIO_PIN      5
 #elif defined(MCU_STM32F3) && USART1_RX_PIN == 1
  #define RX_GPIO_PORT    NUTGPIO_PORTE
  #define RX_GPIO_PIN      1
 #elif defined(STM32F411) && USART1_RX_PIN == 3
  #define RX_GPIO_PORT    NUTGPIO_PORTB
  #define RX_GPIO_PIN      3
 #else
  #warning "Illegal USART1 RX pin assignement"
 #endif
#endif

#ifdef USART1_HARDWARE_HANDSHAKE
#define RTS_GPIO_PORT   NUTGPIO_PORTA
#define RTS_GPIO_PIN    12
#define CTS_GPIO_PORT   NUTGPIO_PORTA
#define CTS_GPIO_PIN    11
#endif /* USART1_HARDWARE_HANDSHAKE */

#ifdef USART1_RS485_CTRL
#define USART_485_CTRL
#ifdef USART1_485DE_INV
#define USART_485DE_INV
#endif
#ifdef USART1_485RE_INV
#define USART_485RE_INV
#endif
#if defined(USART1_485DE_PORT) && defined(USART1_485DE_PIN)
  #define DE_GPIO_BASE  GPIO_ID2GPIO(USART1_485DE_PORT)
  #define DE_GPIO_PORT  USART1_485DE_PORT
  #define DE_GPIO_PIN   USART1_485DE_PIN
#endif
#if defined(USART1_485RE_PORT) && defined(USART1_485RE_PIN)
#define NRE_GPIO_BASE   GPIO_ID2GPIO(USART1_485RE_PORT)
#define NRE_GPIO_PORT   USART1_485RE_PORT
#define NRE_GPIO_PIN    USART1_485RE_PIN
#endif
#endif /* USART1_RS485_CTRL */

/*!
 * \brief USART1 base configuration.
 */
static void  StmUsartClkEnable(int enable)
{
    if (enable)
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->APB2RSTR |= RCC_APB2RSTR_USART1RST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST;
    if (!enable)
        RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
}

#ifdef USART1_INIT_BAUDRATE
#define USART_INIT_BAUTRATE USART1_INIT_BAUDRATE
#endif

#ifdef USART1_HARDWARE_HANDSHAKE
  #define USART_HWFLOWCTRL USART_HardwareFlowControl_RTS_CTS
  #define US_MODE_HWHANDSHAKE
#else
  #define USART_HWFLOWCTRL USART_HardwareFlowControl_None
  #undef  US_MODE_HWHANDSHAKE
#endif

#ifdef USART1_XONXOFF_CONTROL
#define USART_XONXOFF_CONTROL
#else
#undef  USART_XONXOFF_CONTROL
#endif

#ifdef USART1_MODE_IRDA
#define USART_MODE_IRDA
#else
#undef  USART_MODE_IRDA
#endif

#ifdef USART1_HARDWARE_HDX
#define USART_HARDWARE_HDX
#else
#undef USART_HARDWARE_HDX
#endif

#ifdef USART1_SUPPORT_DMA
 #if defined(MCU_STM32F1)||defined(MCU_STM32L1)
  #define UART_DMA_TXCHANNEL  DMA1_C4
  #define UART_DMA_RXCHANNEL  DMA1_C5
 #elif  defined(MCU_STM32F2)||defined(MCU_STM32F4)
  #define UART_DMA_TXCHANNEL  USART1_TX_DMA
  #define UART_DMA_RXCHANNEL  USART1_RX_DMA
 #else
  #warning "STM32 family has no implemented DMA"
 #endif
#else
#undef UART_DMA_TXCHANNEL
#undef UART_DMA_RXCHANNEL
#endif

#if defined(USART1_TX_PIN_INV)
#define USART_TX_PIN_INV
#else
#undef USART_TX_PIN_INV
#endif
#if defined(USART1_RX_PIN_INV)
#define USART_RX_PIN_INV
#else
#undef USART_RX_PIN_INV
#endif

#if defined(USART1_SWAP)
#define USART_SWAP
#else
#undef USART_SWAP
#endif

#define USARTn      USART1
#define USARTnBase  USART1_BASE
#define USARTirqn   USART1_IRQn
#if defined(MCU_STM32F0)
#define USARTclk    NUT_HWCLK_PCLK1
#else
#define USARTclk    NUT_HWCLK_PCLK2
#endif
#define UART_DR_PTR (uint32_t*)(USARTnBase+4)

#define SigUSART    sig_USART1
#define DcbUSART    dcb_usart1

/*@}*/
#include "stm32_usart.c"
