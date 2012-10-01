/*
 * Copyright (C) 2012 by Uwe Bonnes
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
 * $Id: stm32_can2.c 3108 2010-09-15 21:11:15Z Astralix $
 * \endverbatim
 */

#ifndef _DEV_GPIO_AT91_H_
#define _DEV_GPIO_AT91_H_

#if defined(PIO_BASE)
#define NUTGPIO_PORT PIO_BASE
#endif

#if defined(PIOA_BASE)
#define NUTGPIO_PORTA PIOA_BASE
#endif

#if defined(PIOB_BASE)
#define NUTGPIO_PORTB PIOB_BASE
#endif

#if defined(PIOC_BASE)
#define NUTGPIO_PORTC PIOC_BASE
#endif

#if defined(PIOD_BASE)
#define NUTGPIOPORTD PIOD_BASE
#endif

#if defined(PIOE_BASE)
#define NUTGPIO_PORTE PIOE_BASE
#endif

/*!
 * \brief GPIO input.
 *
 * Will configure the pin as input. This is the default state, when no other
 * config option is given.
 */

#define GPIO_CFG_INPUT      0x00000000

/*!
 * \brief GPIO disabled.
 *
 * Will activate the pins alternate function if set. This may not work
 * on all platforms.
 */
#define GPIO_CFG_DISABLED   0x00000001

/*!
 * \brief GPIO output direction enabled.
 *
 * If set, the pin is configured as an output. Otherwise it is in
 * input mode or z-state.
 */
#define GPIO_CFG_OUTPUT     0x00000002

/*!
 * \brief GPIO pull-up enabled.
 */
#define GPIO_CFG_PULLUP     0x00000004

/*!
 * \brief GPIO open drain output feature enabled.
 *
 * If not set, the output will use push pull mode.
 */
#define GPIO_CFG_MULTIDRIVE 0x00000008

/*!
 * \brief GPIO input glitch filter enabled.
 */
#define GPIO_CFG_DEBOUNCE   0x00000010

/*!
 * \brief GPIO Peripheral 0 mux.
 * Switch pin function to peripheral 0.
 */
#define GPIO_CFG_PERIPHERAL0 0x00000020

/*!
 * \brief GPIO Peripheral 1 mux.
 * Switch pin function to peripheral 1.
 */
#define GPIO_CFG_PERIPHERAL1 0x00000040

/*!
 * \brief GPIO Peripheral 2 mux.
 * Switch pin function to peripheral 2.
 */
#define GPIO_CFG_PERIPHERAL2 0x00000080

/*!
 * \brief GPIO Peripheral 3 mux.
 * Switch pin function to peripheral 3.
 */
#define GPIO_CFG_PERIPHERAL3 0x00000100

typedef struct {
    void (*iov_handler) (void *);
    void *iov_arg;
} GPIO_VECTOR;

typedef struct {
    IRQ_HANDLER *ios_sig;
    void (*ios_handler) (void *);
    int (*ios_ctl) (int cmd, void *param, int bit);
    GPIO_VECTOR *ios_vector;
} GPIO_SIGNAL;

#if defined(PIO_ISR)
extern GPIO_SIGNAL sig_GPIO;
#endif
#if defined(PIOA_ISR)
extern GPIO_SIGNAL sig_GPIO1;
#endif
#if defined(PIOB_ISR)
extern GPIO_SIGNAL sig_GPIO2;
#endif
#if defined(PIOC_ISR)
extern GPIO_SIGNAL sig_GPIO3;
#endif

extern uint32_t GpioPinConfigGet(int bank, int bit);
extern int GpioPinConfigSet(int bank, int bit, uint32_t flags);
extern int GpioPortConfigSet(int bank, uint32_t mask, uint32_t flags);

#define GpioPinGet(bank, bit) ((inr((bank) + PIO_PDSR_OFF) &  (_BV(bit)))?1:0)

#define GpioPinSetLow(bank, bit) outr((bank) + PIO_CODR_OFF, (_BV(bit)))

#define GpioPinSetHigh(bank, bit) outr((bank) + PIO_SODR_OFF, (_BV(bit)))

#define GpioPinSet(bank, bit, value) (value)?GpioPinSetHigh((bank), (_BV(bit))) \
        :GpioPinSetLow((bank), (_BV(bit)))

#define GpioPinDrive(bank, bit) outr((bank) + PIO_OER_OFF, (_BV(bit)))

#define GpioPinRelease(bank, bit) outr((bank) + PIO_ODR_OFF, (_BV(bit)))

#define GpioPortGet(bank) (inr((bank) + PIO_PDSR_OFF))

#define GpioPortSetHigh(bank, mask) outr((bank) + PIO_SODR_OFF, (mask))

#define GpioPortSetLow(bank, mask) outr((bank) + PIO_CODR_OFF, (mask))

extern int GpioRegisterIrqHandler(GPIO_SIGNAL * sig, int bit, void (*handler) (void *), void *arg);
extern int GpioIrqEnable(GPIO_SIGNAL * sig, int bit);
extern int GpioIrqDisable(GPIO_SIGNAL * sig, int bit);

#endif
