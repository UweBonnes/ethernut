/*
 * Copyright 2012 by Embedded Technologies s.r.o
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

#ifndef _MCF5_TWIF_H_
#define _MCF5_TWIF_H_

typedef struct _NUTTWIICB NUTTWIICB;

/*
 * Runtime Data container.
 * This is installed in heap at initializaton of a bus.
 */
struct _NUTTWIICB {

    /*! \brief Bus slave address.
     */
    volatile uint_fast16_t tw_mm_sla;

    /*! \brief Bus slave device found.
     */
    volatile uint_fast16_t tw_mm_sla_found;

    /*! \brief Bus current error condition.
     */
    volatile uint_fast8_t tw_mm_err;

    /*! \brief Bus last error condition.
     */
    volatile uint_fast8_t tw_mm_error;

    /*! \brief Bus nodes internal address register.
     */
    uint8_t *tw_mm_iadr;

    /*! \brief Bus nodes internal address register length.
     */
    volatile uint_fast8_t tw_mm_iadrlen;

    /*! \brief Bus transmission data buffer pointer.
     */
    const uint8_t *tw_mm_txbuf;

    /*! \brief Bus transmission data block length.
     */
    volatile uint_fast16_t tw_mm_txlen;

    /*! \brief Bus reception data buffer pointer.
     */
    uint8_t *tw_mm_rxbuf;

    /*! \brief Bus reception data block length.
     */
    volatile uint_fast16_t tw_mm_rxlen;

    /*! \brief Bus data direction.
     */
    volatile uint_fast8_t tw_mm_dir;

    /*! \brief Transmission Ongoing Mutex.
     */
    HANDLE tw_mm_mtx;
};

extern NUTTWIBUS Mcf5TwiBus0;
extern NUTTWIBUS Mcf5TwiBus1;

#ifndef DEF_TWIBUS
#ifdef I2CBUS1_AS_DEFAULT
#define DEF_TWIBUS Mcf5TwiBus1
#else
#define DEF_TWIBUS Mcf5TwiBus0
#endif
#endif

#define PIN_SET1    1
#define PIN_SET2    2
#define PIN_SET3    3
#define PIN_SET4    4

#endif /* _MCF5_TWIF_H_ */
