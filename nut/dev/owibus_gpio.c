/*
 * Copyright (C) 2013 by Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
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

/*!
 * \file dev/owibus_gpio.c
 * \brief Implementation of One-Wire primitives with Bitbanging, configured
 *        at library compile time.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <cfg/arch.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/timer.h>

#include <dev/gpio.h>
#include <dev/owibus.h>

#if defined(OWI0_PORT) && defined(OWI0_PIN)
#undef GPIO_ID
#define GPIO_ID OWI0_PORT
#include <cfg/arch/piotran.h>
static INLINE void OWI0_INIT(void) { GPIO_INIT(OWI0_PIN); GPIO_PULLUP_ON(OWI0_PIN); }
static INLINE void OWI0_LO(void) { GPIO_SET_LO(OWI0_PIN); GPIO_OUTPUT(OWI0_PIN); }
static INLINE void OWI0_HI(void) { GPIO_INPUT(OWI0_PIN); GPIO_SET_HI(OWI0_PIN); }
static INLINE int  OWI0_GET(void) { return GPIO_GET(OWI0_PIN); }
#else
#warning X
#define OWI0_INIT()
#define OWI0_LO()
#define OWI0_HI()
#define OWI0_GET() 0
#endif

/*!
 * \addtogroup xgOwibusBb
 */
/*@{*/

/*!
 * \brief Perform One-Wire transaction.
 *
 * \param bus     Specifies the One-Wire bus.
 * \param command Either OWI_CMD_RESET or OWI_CMD_RWBIT.
 * \param value   The value to send.
 *
 * \return The value read on success, a negative value otherwise.
 */
static int Gpio0_OwiTransaction(NUTOWIBUS *bus, int_fast8_t command, int_fast8_t value)
{
    int res;
    int16_t delay1 =
        (owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_SYNC_PULSE] -
         owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_SETUP]) >> 2;
    int16_t delay2 =
        (owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RW] -
         owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_SYNC_PULSE]) >> 2;
    int16_t delay3 =
        (owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RELEASE] -
         owi_timervalues_250ns[bus->mode & OWI_OVERDRIVE][command][OWI_PHASE_RW]) >> 2;

    /* Be nice! Allow other thing to happen now before we block
     * cooperative multitasking for up to 480 us
     */
    NutSleep(0);
    OWI0_LO();
    NutMicroDelay(delay1);
    if (value == 0)
        OWI0_LO();
    else
        OWI0_HI();
    NutMicroDelay(delay2);
    res = OWI0_GET();
#if 0
    if (value)
        /* If the TXRX line is allready pull up, we only need to wait,
         * but no time sensitive action must be performed. We block for
         * up to 410 us now. So be nice again!
         */
        NutSleep(0);
#endif
    NutMicroDelay(delay3);
    OWI0_HI();
    NutSleep(1);
    return res;
}

/*!
 * \brief Reset the One-Wire bus and check if device(s) present.
 *
 * \param bus Specifies the One-Wire bus.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
static int Gpio0_OwiTouchReset(NUTOWIBUS *bus)
{
    return Gpio0_OwiTransaction(bus, OWI_CMD_RESET, 1);
}

/*!
 * \brief Exchange one bit on the One-Wire bus.
 *
 * \param bus Specifies the One-Wire bus.
 * \param bit Value for the bit to send.
 *
 * \return The bus state at the read slot on success, a negative value
 *         otherwise.
 */
static int Gpio0_OwiRWBit(NUTOWIBUS *bus, uint_fast8_t bit)
{
    return Gpio0_OwiTransaction(bus, OWI_CMD_RWBIT, bit);
}

/*!
 * \brief Write a block of data bits to the One-Wire bus.
 *
 * \param bus  Specifies the One-Wire bus.
 * \param data Data bits to send.
 * \param len  Number of bits to send.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
static int Gpio0_OwiWriteBlock(NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len)
{
    int res;
    int i;

    for (i = 0; i < len; i++) {
        res = Gpio0_OwiRWBit(bus, data[i >> 3] & (1 << (i & 0x7)));
        if (res < 0)
            return OWI_HW_ERROR;
    }
    return OWI_SUCCESS;
}

/*!
 * \brief Read a block of data bits from the One-Wire bus.
 *
 * \param bus  Specifies the One-Wire bus.
 * \param data Data bits received.
 * \param len  Number of bits to read.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
static int Gpio0_OwiReadBlock(NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len)
{
    int res;
    int i;

    memset(data, 0, (len >> 3) + 1);
    for (i = 0; i < len; i++) {
        res = Gpio0_OwiRWBit(bus, 1);
        if (res < 0)
            return OWI_HW_ERROR;
        data[i >> 3] |= (res << (i & 0x7));
    }
    return OWI_SUCCESS;
}

/*!
 * \brief Read a block of data bits from the One-Wire bus.
 *
 * \param bus  Specifies the One-Wire bus.
 * \param data Data bits received.
 * \param len  Number of bits to read.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
static int Gpio0_Setup(NUTOWIBUS *bus)
{
    (void) bus;

#if !defined(OWI0_PORT) || !defined(OWI0_PIN)
    return OWI_INVALID_HW;
#else
    OWI0_INIT();
    return OWI_SUCCESS;
#endif
}

/*!
 * \brief Library compile time configured OWI bus driver for GPIO.
 *
 */
 NUTOWIBUS owiBus0Gpio = {
    0,                   /*!< \brief OWIBUSBUS::owibus_info */
    OWI_MODE_NORMAL,     /*!< \brief OWIBUSBUS::mode */
    Gpio0_Setup,         /*!< \brief OWIBUSBUS::OwiSetup */
    Gpio0_OwiTouchReset, /*!< \brief OWIBUSBUS::OwiTouchReset*/
    Gpio0_OwiReadBlock,  /*!< \brief OWIBUSBUS::OwiReadBlock */
    Gpio0_OwiWriteBlock  /*!< \brief OWIBUSBUS::OwiWriteBlock */
 };
/*@}*/
