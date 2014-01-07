/*
 * Copyright (C) 2014 by Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
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
 * \file dev/owibus0gpio.c
 * \brief OWI bus 0 for UART declaration file.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */
#include <cfg/owi.h>
#include <dev/owibus.h>
#include <dev/usartstm32.h>

#if defined (OWIBUS0_HALDUPLEX)
#define OWIBUS0_HALDUPLEX_MODE 1
#else
#define OWIBUS0_HALDUPLEX_MODE 0
#endif

#define OWIBUS0_UART devUsartStm32_2

static NUTOWIINFO_UART owcb0 = {
    0,
    0,
    0
};

static int Uart_OwiSetup(NUTOWIBUS *bus)
{
    int res;

    if (NutRegisterDevice( &OWIBUS0_UART, 0, 0)) {
        return OWI_INVALID_HW;
    }
    res = Uart_OwiInit(&owcb0, &OWIBUS0_UART, OWIBUS0_HALDUPLEX_MODE);
    if (res)
        return res;
    return OWI_SUCCESS;
}

/*!
 * \brief Library compile time configured OWI bus driver for UARTs
 *
 */
 NUTOWIBUS owiBus0Uart = {
    (uintptr_t )&owcb0,              /*!< \brief OWIBUSBUS::owibus_info */
    OWI_MODE_NORMAL,    /*!< \brief OWIBUSBUS::mode */
    Uart_OwiSetup,      /*!< \brief OWIBUSBUS::OwiSetup */
    Uart_OwiTouchReset, /*!< \brief OWIBUSBUS::OwiTouchReset*/
    Uart_OwiReadBlock,  /*!< \brief OWIBUSBUS::OwiReadBlock */
    Uart_OwiWriteBlock  /*!< \brief OWIBUSBUS::OwiWriteBlock */
 };

