#ifndef _DEV_SPI_7SEG_H_
#define _DEV_SPI_7SEG_H_
/*
 * Copyright (C) 2008-2009 by egnite GmbH
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
 * \file include/dev/spi_7seg.h
 * \brief ntrx
 *
 * \verbatim
 * $Id: spi_ntrx.h$
 * \endverbatim
 */

#include <sys/device.h>
#include <dev/spibus.h>

extern NUTSPINODE nodeSpi7SEG;
extern NUTDEVICE devSpi7SEG;

#define DOT_7SEG_SET    0
#define DOT_7SEG_CLEAR	1
#define DOT_7SEG_FLIP	2

__BEGIN_DECLS
/* Prototypes */
extern int Spi7segInit(NUTDEVICE *dev);
extern int Spi7segCommand(NUTDEVICE * dev, uint8_t addr, CONST void *txbuf, void *rxbuf, int xlen);

extern void display_7seg(NUTDEVICE * dev, uint16_t number);
extern void print_7seg(NUTDEVICE *dev, char* chars);
extern void dot_7seg(NUTDEVICE *dev, uint8_t dotNumber, uint8_t dotCommand);
/* Prototypes */
__BEGIN_DECLS

#endif
