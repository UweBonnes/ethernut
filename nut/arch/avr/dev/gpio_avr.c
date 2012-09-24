/*!
 * Copyright (C) 2007 by egnite Software GmbH. All rights reserved.
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
 * $Id$
 */

#include <arch/avr.h>

#include <stdlib.h>
#include <string.h>

#include <dev/gpio.h>

uint32_t GpioPinConfigGet(int bank, int bit)
{
    uint32_t rc = 0;

    if (AVR_DDRX(bank) & _BV(bit)) {
        rc |= GPIO_CFG_OUTPUT;
    }
    else if (AVR_PORTX(bank) & _BV(bit)) {
        rc |= GPIO_CFG_PULLUP;
    }
    return rc;
}

int GpioPortConfigSet(int bank, unsigned int mask, uint32_t flags)
{
    if (flags & GPIO_CFG_PULLUP) {
        AVR_PORTX(bank) |= mask;
    }
    else {
        AVR_PORTX(bank) &= ~mask;
        outb(PORTA, inb(PORTA) & ~mask);
    }
    if (flags & GPIO_CFG_OUTPUT) {
        AVR_DDRX(bank) |= mask;
    }
    else {
        AVR_DDRX(bank) &= ~mask;
    }
    return 0;
}

int GpioPinConfigSet(int bank, int bit, uint32_t flags)
{
    if (flags & GPIO_CFG_PULLUP)
        flags &= ~GPIO_CFG_OUTPUT;
    if (flags & GPIO_CFG_OUTPUT) {
        AVR_DDRX(bank) |= _BV(bit);
    }
    else {
        if (flags & GPIO_CFG_PULLUP) {
            AVR_PORTX(bank) |= BV(bit);
        }
        else {
            AVR_PORTX(bank) &= ~BV(bit);
        }
        AVR_DDRX(bank) &= ~_BV(bit);
    }

    /* Check the result. */
    if (GpioPinConfigGet(bank, bit) != flags) {
        return -1;
    }
    return 0;
}

int GpioRegisterIrqHandler(GPIO_SIGNAL * sig, int bit, void (*handler) (void *), void *arg)
{
    return -1;
}

int GpioIrqEnable(GPIO_SIGNAL * sig, int bit)
{
    return -1;
}

int GpioIrqDisable(GPIO_SIGNAL * sig, int bit)
{
    return -1;
}
