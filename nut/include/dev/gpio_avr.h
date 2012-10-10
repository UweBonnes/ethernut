/*
 * Copyright (C) 2012 Uwe Bonnes. All rights reserved.
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

/*!
 * \file os/timer.c
 * \brief System timer support.
 *
 * This kernel module contains the hardware independent system timer routines.
 *
 * \verbatim
 *
 * Provide NUTGPIO functionality for AVR mostly as macros. This makes the
 * register addresses available to the preprocessor and the proprocessor
 * can decide to use cbi/sbi where appropriate. Inline functions would
 * allow to use asserts for code checking, but at least with
 * avr-gcc 4.3.3 no sensible coding was found
 *
 * The code relies on PINX/DDRX/PORTX at consequitive SFR addresses.
 *
 * Another thing to observe is GPIO_CFG_MULTIDRIVE. There is no register to set
 * a pin as multidrive and so this must be implemented in the access method.
 *
 * For Multidrive operation GpioPinDrive/Release are used to switch
 * DDR after setting PORT low with GpioPinSetLow.
 *
 */

#include <arch/avr.h>

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
#define GPIO_CFG_MULTIDRIVE 0x00000000

/*!
 * \brief GPIO input glitch filter enabled.
 *
 * Not implemented in AVR
 */
#define GPIO_CFG_DEBOUNCE   0x00000010

/*!
 * \brief GPIO Output Register inital value  Low
 */
#define GPIO_CFG_INIT_LOW    0x40000000
/*!
 * \brief GPIO Output Register inital value  High
 */
#define GPIO_CFG_INIT_HIGH   0x80000000

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

#if defined(NUTDEBUG)
#define _PORT_ASSERT(x) NUTASSERT(x);
#else
#define _PORT_ASSERT(x)
#endif

#if defined(PORTA)
#define PORTA_ASSERT(x) ((x) == (PORTA))
#define NUTGPIO_PORTA _SFR_IO_ADDR(PORTA)
#else
#define PORTA_ASSERT(x) 1
#endif

#if defined(PORTB)
#define PORTB_ASSERT(x) ((x) == (PORTB))
#define NUTGPIO_PORTB _SFR_IO_ADDR(PORTB)
#else
#define PORTB_ASSERT(x) 1
#endif

#if defined(PORTC)
#define PORTC_ASSERT(x) ((x) == (PORTC))
#define NUTGPIO_PORTC _SFR_IO_ADDR(PORTC)
#else
#define PORTC_ASSERT(x) 1
#endif

#if defined(PORTD)
#define PORTD_ASSERT(x) ((x) == (PORTD))
#define NUTGPIO_PORTD _SFR_IO_ADDR(PORTD)
#else
#define PORTD_ASSERT(x) 1
#endif

#if defined(PORTE)
#define PORTE_ASSERT(x) ((x) == (PORTE))
#define NUTGPIO_PORTE _SFR_IO_ADDR(PORTE)
#else
#define PORTE_ASSERT(x) 1
#endif

#if defined(PORTF)
#define PORTF_ASSERT(x) ((x) == PORTF)
#define NUTGPIO_PORTF _SFR_IO_ADDR(PORTF)
#else
#define PORTF_ASSERT(x) 1
#endif

#if defined(PORTG)
#define PORTG_ASSERT(x) ((x) == PORTG)
#define NUTGPIO_PORTG _SFR_IO_ADDR(PORTG)
#else
#define PORTG_ASSERT(x) 1
#endif

#if defined(PORTH)
#define PORTH_ASSERT(x) ((x) == PORTH)
#define NUTGPIO_PORTH _SFR_IO_ADDR(PORTH)
#else
#define PORTH_ASSERT(x) 1
#endif

#if defined(PORTI)
#define PORTI_ASSERT(x) ((x) == PORTI)
#define NUTGPIO_PORTI _SFR_IO_ADDR(PORTI)
#else
#define PORTI_ASSERT(x)
#endif

#if defined(PORTJ)
#define PORTJ_ASSERT(x) ((x) == PORTJ)
#define NUTGPIO_PORTJ _SFR_IO_ADDR(PORTJ)
#else
#define PORTJ_ASSERT(x) 1
#endif

#if defined(PORTK)
#define PORTK_ASSERT(x) ((x) == PORTK)
#define NUTGPIO_PORTK _SFR_IO_ADDR(PORTK)
#else
#define PORTK_ASSERT(x) 1
#endif

#if defined(PORTL)
#define PORTL_ASSERT(x) ((x) == PORTL)
#define NUTGPIO_PORTL _SFR_IO_ADDR(PORTL)
#else
#define PORTL_ASSERT(x) 1
#endif

#define AVR_PORTX(bank) (_SFR_IO8(bank   ))
#define AVR_DDRX(bank)  (_SFR_IO8(bank -1))
#define AVR_PINX( bank) (_SFR_IO8(bank -2))

#define GpioPinGet(bank, bit)     (AVR_PINX(bank)  &    _BV(bit))

#define GpioPinSetLow(bank, bit)  (AVR_PORTX(bank) &= ~(_BV(bit)))

#define GpioPinSetHigh(bank, bit) (AVR_PORTX(bank) |= _BV(bit))

#define GpioPinRelease(bank, bit) (AVR_DDRX(bank) &= ~(_BV(bit)))

#define GpioPinDrive(bank, bit)   (AVR_DDRX(bank) |= (_BV(bit)))

#define GpioPinSet(bank, bit, value)\
    (value)?GpioPinSetHigh(bank, bit):GpioPinSetLow(bank, bit)

#define GpioPortGet(bank) AVR_PINX(bank)

#define GpioPortSet(bank, value) (AVR_PORTX(bank) = (uint8_t)value))

#define GpioPortSetLow(bank, mask) GpioPortSet(bank, GpioPortGet(bank) & ~mask)

#define GpioPortSetHigh(bank, mask) GpioPortSet(bank, GpioPortGet(bank) |  mask)

extern uint32_t GpioPinConfigGet(int bank, int bit);
extern int GpioPinConfigSet(int bank, int bit, uint32_t flags);

extern int GpioPortConfigSet(int bank, unsigned int mask, uint32_t flags);

