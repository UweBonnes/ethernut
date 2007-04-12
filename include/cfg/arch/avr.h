#ifndef _CFG_ARCH_AVR_H_
#define _CFG_ARCH_AVR_H_

/*
 * Copyright (C) 2004 by egnite Software GmbH. All rights reserved.
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

/*
 * $Log$
 * Revision 1.6  2007/04/12 09:10:29  haraldkipp
 * PORTH added.
 *
 * Revision 1.5  2005/08/02 17:46:48  haraldkipp
 * Major API documentation update.
 *
 * Revision 1.4  2005/02/02 20:03:46  haraldkipp
 * All definitions had been moved to avrpio.h in order to fix the broken
 * port I/O without being forced to change other existing modules.
 *
 * Revision 1.3  2005/01/22 19:27:19  haraldkipp
 * Changed AVR port configuration names from PORTx to AVRPORTx.
 *
 * Revision 1.2  2004/09/22 08:18:57  haraldkipp
 * More configurable ports
 *
 * Revision 1.1  2004/08/25 10:58:02  haraldkipp
 * New include directory cfg/arch added, which is used for target specific items,
 * mainly port usage or MCU specific register settings.
 *
 */

/*!
 * \addtogroup xgConfigAvr
 */
/*@{*/

/*!
 * \file include/cfg/arch/avr.h
 * \brief AVR hardware configuration.
 */


#define AVRPORTA    1
#define AVRPORTB    2
#define AVRPORTC    3
#define AVRPORTD    4
#define AVRPORTE    5
#define AVRPORTF    6
#define AVRPORTG    7
#define AVRPORTH    8

#include <cfg/arch/avrpio.h>

/*@}*/

#endif
