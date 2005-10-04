/*
 * Copyright (c) 2005 FOCUS Software Engineering Pty Ltd <www.focus-sw.com>
 * Copyright (c) 2005 proconX <www.proconx.com>
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
 *
 */

/*!
 * \file include/dev/atcan.h
 * \brief Header for Atmel's internal Full CAN controller driver
 */

/*!
 * \addtogroup xgCanAvr
 */
/*@{*/

#ifndef _ATCAN_H_
#define _ATCAN_H_

#include <dev/can_dev.h>


/*
 * CAN Baud rates
 */
#define CAN_SPEED_10K      0   ///< 10 kbit/s, max. cable length 5000 m
#define CAN_SPEED_20K      1   ///< 20 kbit/s, max. cable length 2500 m
#define CAN_SPEED_50K      2   ///< 50 kbit/s, max. cable length 1000 m
#define CAN_SPEED_100K     3   ///< 100 kbit/s, max. cable length 600 m
#define CAN_SPEED_125K     4   ///< 125 kbit/s, max. cable length 500 m
#define CAN_SPEED_250K     5   ///< 250 kbit/s, max. cable length 250 m
#define CAN_SPEED_500K     6   ///< 500 kbit/s, max. cable length 100 m
#define CAN_SPEED_800K     7   ///< 800 kbit/s, max. cable length 50 m
#define CAN_SPEED_1M       8   ///< 1 Mbit/s, max. cable length 25 m
#define CAN_SPEED_CUSTOM 255


/*
 * CAN result codes
 */
#define errCAN_INVALID_BAUD      7 //ttt ask ole about errCAN...
#define CAN_OK                0
#define CAN_ERR               1
#define CAN_INIT_SPEED_ERR    1
#define CAN_NOT_INITIATED     1
#define CAN_NOT_STARTED       1
#define CAN_TXBUF_FULL        2
#define CAN_RXBUF_EMPTY       2
#define CAN_INVALID_SPEED     7  ///< Invalid baud rate parameter
                                 //
                                 //
                                 //


                                 //ttt for ole: I suggest neagtive vaules
//ttt for ole: I suggest continuous numbers

extern NUTDEVICE devAtCan;

#endif
/*@}*/

