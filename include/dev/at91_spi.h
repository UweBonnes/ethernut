#ifndef _DEV_AT91_SPI_H_
#define _DEV_AT91_SPI_H_
/*
 * Copyright (C) 2006 by egnite Software GmbH. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
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
 */

/*!
 * \file arch/arm/at91_spi.h
 * \brief AT91 peripherals.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/09/29 12:34:59  haraldkipp
 * Basic AT91 SPI support added.
 *
 *
 * \endverbatim
 */

#include <sys/types.h>
#include <cfg/arch.h>

/* Should be in dev/spi.h. */
#define SPI_SETSPEED        0x0401
#define SPI_GETSPEED        0x0402

#define SPI_SETMODE         0x0403
#define SPI_GETMODE         0x0404

#define SPIMF_MASTER        0x00000001  /* Master mode. */
#define SPIMF_PCSDEC        0x00000002  /* Decoded chip selects. */
#define SPIMF_MFDETECT      0x00000004  /* Mode fault detection. */
#define SPIMF_LOOPBACK      0x00000008  /* Loopback mode. */
#define SPIMF_SCKIAHI       0x00000010  /* Clock is high when inactive. */
#define SPIMF_CAPRISE       0x00000020  /* Data cpatured on rising edge. */
#define SPIMF_KEEPCS        0x00000040  /* Chip select remains active after transfer. */

#define SPI_SETDATABITS     0x0405
#define SPI_GETDATABITS     0x0406
/* dev/spi.h. */


__BEGIN_DECLS
/* Prototypes */
extern int At91Spi0Init(void);
extern int At91Spi0InitChipSelects(u_int mask);
extern int At91Spi0Enable(void);
#if defined(SPI1_BASE)
extern int At91Spi1Init(void);
extern int At91Spi1InitChipSelects(u_int mask);
extern int At91Spi1Enable(void);
#endif

extern int At91SpiInit(u_int base);
extern int At91SpiReset(u_int base);
extern int At91SpiInitChipSelects(u_int base, u_int mask);

extern int At91SpiSetRate(u_int base, u_int cs, u_long rate);
extern u_long At91SpiGetModeFlags(u_int base, u_int cs);
extern int At91SpiSetModeFlags(u_int base, u_int cs, u_long mode);
extern u_int At91SpiGetBits(u_int base, u_int cs);
extern int At91SpiSetBits(u_int base, u_int cs, u_int bits);
extern u_int At91SpiGetSckDelay(u_int base, u_int cs);
extern int At91SpiSetSckDelay(u_int base, u_int cs, u_int dly);
extern u_int At91SpiGetTxDelay(u_int base, u_int cs);
extern int At91SpiSetTxDelay(u_int base, u_int cs, u_int dly);
extern u_int At91SpiGetCsDelay(u_int base);
extern int At91SpiSetCsDelay(u_int base, u_int dly);

extern int At91SpiTransfer2(u_int base, u_int cs, CONST void *txbuf, void *rxbuf, int xlen,
                           CONST void *txnbuf, void *rxnbuf, int xnlen);

__END_DECLS
/* End of prototypes */
#endif
