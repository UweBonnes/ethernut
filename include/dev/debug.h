#ifndef _DEV_DEBUG_H_
#define _DEV_DEBUG_H_

/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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

/*
 * $Log$
 * Revision 1.6  2005/02/10 07:06:50  hwmaier
 * Changes to incorporate support for AT90CAN128 CPU
 *
 * Revision 1.5  2004/10/03 18:39:12  haraldkipp
 * GBA debug output on screen
 *
 * Revision 1.4  2004/09/08 10:53:14  haraldkipp
 * Our first device for the EB40A
 *
 * Revision 1.3  2004/04/07 12:13:57  haraldkipp
 * Matthias Ringwald's *nix emulation added
 *
 * Revision 1.2  2004/03/16 16:48:28  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:05  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.1  2003/04/21 16:57:05  harald
 * First check in
 *
 */

#include <cfg/arch.h>
#include <sys/device.h>
#include <dev/uart.h>

/*!
 * \file dev/debug.h
 * \brief Debug device definitions.
 */

/*
 * Available devices.
 */
#if defined(__AVR__)

extern NUTDEVICE devDebug0;
#ifdef __AVR_ENHANCED__
extern NUTDEVICE devDebug1;
#endif

#elif defined(__H8300H__) || defined(__H8300S__)

/* SCIs (USARTs) in the H8/3068F; it should be adjusted for other H8 devices */
extern NUTDEVICE devDebug0;
extern NUTDEVICE devDebug1;
extern NUTDEVICE devDebug2;

#elif defined(__arm__)
#ifdef MCU_AT91R40008
extern NUTDEVICE devDebug1;
#elif defined(MCU_GBA)
extern NUTDEVICE devDebug0;
#endif
#elif defined(__m68k__)
#endif

#if defined(__linux__) || defined(__APPLE__)
extern NUTDEVICE devDebug0;
extern NUTDEVICE devDebug1;
#endif

#endif                          /* #ifndef _DEV_DEBUG_H_ */
