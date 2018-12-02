/*
 * Copyright (C) 2015 Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
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
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 */
#include <stdint.h>
#include <cfg/arch.h>
#include <dev/hw_signature.h>

#if !defined(MCU_STM32F030)
/*!
 * \brief Get a (hopefully) unique MAC id from the hardware ID
 *
 * \param mac Mac Address to fill in
 *
 * As first 3 byte we use the Egnite OUI, the other 3 byte are
 * the bytes of the 3 unique ID words xored together.
 *.
 */
void Stm32GetUniquePrivateMac(void* mac)
{
    uint8_t  *id, *mac_p;

    mac_p = (uint8_t *) mac;
    mac_p[0] = 0;
    mac_p[1] = 6;
    mac_p[2] = 0x98;
    id = (uint8_t *) UNIQUE_ID_REG_H;
    mac_p[3] = id[0] ^ id[1] ^ id[2] ^ id[3];
    id = (uint8_t *) UNIQUE_ID_REG_M;
    mac_p[4] = id[0] ^ id[1] ^ id[2] ^ id[3];
    id = (uint8_t *) UNIQUE_ID_REG_L;
    mac_p[5] = id[0] ^ id[1] ^ id[2] ^ id[3];
}
#endif
