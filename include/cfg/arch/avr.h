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
 * Revision 1.1  2004/08/25 10:58:02  haraldkipp
 * New include directory cfg/arch added, which is used for target specific items,
 * mainly port usage or MCU specific register settings.
 *
 */

/*!
 * \file cfg/arch/avr.h
 * \brief AVR default hardware configuration.
 *
 * Values are geared to the Ethernut reference design and can be changed 
 * by the configurator.
 */

/*!
 * \addtogroup xgNet
 */
/*@{*/

/*!
 * \brief Settings for the Realtek RTL8019AS.
 */

#ifndef RTL_BASE_ADDR
/*!
 * \brief Memory mapped base address.
 */
#define RTL_BASE_ADDR 0x8300
#endif

#ifndef RTL_SIGNAL_IRQ
/*!
 * \brief Interrupt used by the controller.
 */
#define RTL_SIGNAL_IRQ INT5
#endif

#ifndef RTL_EESK_BIT
/*!
 * \brief Clock input for EEPROM emulation.
 *
 * This is enabled by default, but the driver will run a check before
 * jumping into the emulation.
 */
#define RTL_EESK_BIT 5
#endif

#ifndef RTL_EESK_PORT
#define RTL_EESK_PORT PORTC
#endif

#ifndef RTL_EEDO_BIT
#define RTL_EEDO_BIT 6
#endif

#ifndef RTL_EEDO_PORT
#define RTL_EEDO_PORT PORTC
#endif

#ifndef RTL_EEMU_BIT
#define RTL_EEMU_BIT 7
#endif

#ifndef RTL_EEMU_PORT
#define RTL_EEMU_PORT PORTC
#endif

/*@}*/

#endif
