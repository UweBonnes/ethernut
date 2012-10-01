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

#include <cfg/os.h>
#include <cfg/arch.h>
#include <cfg/arch/gpio.h>

#include <arch/arm.h>

#include <stdlib.h>
#include <string.h>

#include <dev/gpio.h>

/*!
 * \brief Set all pin levels of a bank/port.
 *
 * This routine can be used to simultaneously set all pins of a specific
 * port. However, in some implementations the high bit values will be
 * set before the low bit values. If this is a problem, you should use
 * GpioPortSetHigh() and GpioPortSetLow().
 *
 * \param bank  GPIO bank/port number.
 * \param value Pin levels are set to high, if the corresponding
 *              bit in this mask is 1. All other pin levels are
 *              set to low.
 */
static inline void GpioPortSet(int bank, unsigned int value)
{
    GpioPortSetHigh(bank, value);
    GpioPortSetLow(bank, ~value);
}

/*!
 * \brief Get pin configuration.
 *
 * \param bank GPIO bank/port number.
 * \param bit  Bit number of the specified bank/port.
 *
 * \return Attribute flags of the pin.
 */
uint32_t GpioPinConfigGet(int bank, int bit)
{
    uint32_t rc = 0;
    rc |= (inr(bank + PIO_PSR_OFF) & _BV(bit))?GPIO_CFG_DISABLED:0;
    rc |= (inr(bank + PIO_OSR_OFF) & _BV(bit))?GPIO_CFG_OUTPUT:0;

#ifdef PIO_IFSR
    rc |= (inr(bank + PIO_IFSR_OFF) & _BV(bit))?GPIO_CFG_DEBOUNCE:0;
#endif /* PIO_IFSR */

#ifdef PIO_MDSR
    rc |= (inr(bank + PIO_MDSR_OFF) & _BV(bit))?GPIO_CFG_MULTIDRIVE:0;
#endif /* PIO_MDSR */

#ifdef PIO_PUSR
    rc |= (inr(bank + PIO_PUSR_OFF) & _BV(bit))?0:GPIO_CFG_PULLUP;
#endif /* PIO_PUSR */
    return rc;
}

void PIO_PMC_ENABLE(int bank)
{
#ifdef PMC_PCER
    switch (bank)
    {
#if defined (NUTGPIO_PORT) && defined(PIO_ID)
    case NUTGPIO_PORT:
            outr(PMC_PCER, _BV(PIO_ID));
            break;
#endif
#if defined (NUTGPIO_PORTA) && defined(PIOA_ID)
    case NUTGPIO_PORTA:
            outr(PMC_PCER, _BV(PIOA_ID));
            break;
#endif
#if defined (NUTGPIO_PORTB) && defined(PIOB_ID)
    case NUTGPIO_PORTB:
            outr(PMC_PCER, _BV(PIOB_ID));
            break;
#endif
#if defined (NUTGPIO_PORTC) && defined(PIOC_ID)
    case NUTGPIO_PORTC:
            outr(PMC_PCER, _BV(PIOC_ID));
            break;
#endif
#if defined (NUTGPIO_PORTD) && defined(PIOD_ID)
    case NUTGPIO_PORTD:
            outr(PMC_PCER, _BV(PIOD_ID));
            break;
#endif
#if defined (NUTGPIO_PORTE) && defined(PIOE_ID)
    case NUTGPIO_PORTE:
            outr(PMC_PCER, _BV(PIOE_ID));
            break;
#endif
    }
#endif
}

/*!
 * \brief Set port wide pin configuration.
 *
 * \note This function does not check for undefined ports and pins or
 *       invalid attributes. If this is required, use GpioPinConfigSet().
 *
 * \param bank  GPIO bank/port number.
 * \param mask  The given attributes are set for a specific pin, if the
 *              corresponding bit in this mask is 1.
 * \param flags Attribute flags to set.
 *
 * \return Always 0.
 */
int GpioPortConfigSet(int bank, uint32_t mask, uint32_t flags)
{
    if (flags & GPIO_CFG_DISABLED)
        outr(bank + PIO_PDR_OFF, mask);
    else
    {
        outr(bank + PIO_PER_OFF, mask);
        PIO_PMC_ENABLE(bank);
    }
#if defined(PIO_HAS_PULLUP)
    if (flags & GPIO_CFG_PULLUP)
        outr(bank + PIO_PUER_OFF, mask);
    else
        outr(bank + PIO_PUDR_OFF, mask);
#endif        
    if (flags & GPIO_CFG_DEBOUNCE)
        outr(bank + PIO_IFER_OFF, mask);
    else
        outr(bank + PIO_IFDR_OFF, mask);
    if (flags & GPIO_CFG_OUTPUT)
        outr(bank + PIO_OER_OFF, mask);
    else
        outr(bank + PIO_ODR_OFF, mask);
#ifdef PIO_MDER
     if (flags & GPIO_CFG_MULTIDRIVE) {
            outr(bank + PIO_MDER_OFF, mask);
        }
        else {
            outr(bank + PIO_MDDR_OFF, mask);
        }
#endif /* PIO_MDER */

    return 0;
}

/*!
 * \brief Set pin configuration.
 *
 * Applications may also use this function to make sure, that a specific
 * attribute is available for a specific pin.
 *
 * \note GPIO pins are typically initialized to a safe state after power
 *       up. This routine is not able to determine the consequences of
 *       changing pin configurations. In the worst case you may permanently
 *       damage your hardware by bad pin settings.
 *
 * \param bank  GPIO bank/port number.
 * \param bit   Bit number of the specified bank/port.
 * \param flags Attribute flags.
 *
 * \return 0 if all attributes had been set, -1 otherwise.
 */
int GpioPinConfigSet(int bank, int bit, uint32_t flags)
{
    GpioPortConfigSet(bank, _BV(bit), flags);

    /* Check the result. */
    if (GpioPinConfigGet(bank, bit) != flags) {
        return -1;
    }
    return 0;
}

/*!
 * \brief Register a GPIO pin interrupt handler.
 *
 * Generating interrupts on GPIO pin changes is not supported on all
 * platforms. In this case dedicated external interrupt pins may
 * be used with NutRegisterIrqHandler().
 *
 * Interrupts are triggered on rising and falling edges. Level triggering
 * or triggering on specific edges is not supported.
 *
 * After registering, interrupts are disabled. Calling GpioIrqEnable()
 * is required to activate the interrupt.
 *
 * The following code fragment registers an interrupt handler which is
 * called on each change of bit 4 of the first GPIO port:
 * \code
 * #include <dev/gpio.h>
 *
 * static void PinChange(void *arg)
 * {
 *     ...
 * }
 *
 * {
 *     ...
 *     GpioPinConfigSet(0, 4, GPIO_CFG_PULLUP);
 *     GpioRegisterIrqHandler(&sig_GPIO, 4, PinChange, NULL);
 *     GpioIrqEnable(&sig_GPIO, 4);
 *     ...
 * }
 * \endcode
 *
 * \param sig     Bank/port interrupt to be associated with this handler.
 * \param bit     Bit number of the specified bank/port.
 * \param handler This routine will be called by Nut/OS, when the specified
 *                pin changes its state.
 * \param arg     Argument to be passed to the interrupt handler routine.
 *
 * \return 0 on success, -1 otherwise.
 */
int GpioRegisterIrqHandler(GPIO_SIGNAL * sig, int bit, void (*handler) (void *), void *arg)
{
    int rc = 0;

    if (sig->ios_vector == 0) {
        /* This is the first call. Allocate the vector table. */
        sig->ios_vector = malloc(sizeof(GPIO_VECTOR) * 32);
        if (sig->ios_vector) {
            memset(sig->ios_vector, 0, sizeof(GPIO_VECTOR) * 32);
            /* Register our internal PIO interrupt service. */
            rc = NutRegisterIrqHandler(sig->ios_sig, sig->ios_handler, sig->ios_vector);
            if (rc == 0) {
                rc = NutIrqEnable(sig->ios_sig);
            }
        }
        else {
            rc = -1;
        }
    }
    sig->ios_vector[bit].iov_handler = handler;
    sig->ios_vector[bit].iov_arg = arg;

    return rc;
}

/*!
 * \brief Enable a specified GPIO interrupt.
 *
 * A related interrupt handler must have been registered before calling
 * this function. See GpioRegisterIrqHandler().
 *
 * \param sig Interrupt to enable.
 * \param bit Bit number of the specified bank/port.
 *
 * \return 0 on success, -1 otherwise.
 */
int GpioIrqEnable(GPIO_SIGNAL * sig, int bit)
{
    return (sig->ios_ctl) (NUT_IRQCTL_ENABLE, NULL, bit);
}

/*!
 * \brief Disable a specified GPIO interrupt.
 *
 * \param sig Interrupt to disable.
 * \param bit Bit number of the specified bank/port.
 *
 * \return 0 on success, -1 otherwise.
 */
int GpioIrqDisable(GPIO_SIGNAL * sig, int bit)
{
    return (sig->ios_ctl) (NUT_IRQCTL_DISABLE, NULL, bit);
}
