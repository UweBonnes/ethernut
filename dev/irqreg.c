/*
 * Copyright (C) 2001-2004 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.5  2004/10/03 18:37:40  haraldkipp
 * GBA support
 *
 * Revision 1.4  2004/03/17 14:58:30  haraldkipp
 * Typos corrected
 *
 * Revision 1.2  2004/03/16 16:48:27  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1.1.1  2003/05/09 14:40:40  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.16  2003/02/04 17:50:54  harald
 * Version 3 released
 *
 * Revision 1.15  2003/01/14 16:09:13  harald
 * Using newer ICC include
 *
 * Revision 1.14  2002/11/02 15:15:13  harald
 * Library dependencies removed
 *
 * Revision 1.13  2002/09/15 16:38:38  harald
 * Pass ImageCraft MCU type
 *
 * Revision 1.12  2002/08/11 12:13:36  harald
 * ICC mods
 *
 * Revision 1.11  2002/08/08 17:14:39  harald
 * Imagecraft support by Klaus Ummenhofer
 *
 * Revision 1.10  2002/07/03 16:45:38  harald
 * Using GCC 3.2
 *
 * Revision 1.9  2002/06/26 17:29:08  harald
 * First pre-release with 2.4 stack
 *
 */

#include <cfg/arch.h>
#include <sys/atom.h>
#include <dev/irqreg.h>

//static IRQ_HANDLER irqHandler[IRQ_MAX];

#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega103__)
/* 
 * AVR is using a separate file for each vector,
 * which saves a lot of space and allows native
 * interrupts.
 */
#elif defined(MCU_GBA)
/* Not yet */
#elif defined(__arm__)
#include "arm_irqreg.c"
#elif defined(__H8300H__) || defined(__H8300S__)
#include "h8_irqreg.c"
#elif defined(__m68k__)
#include "m68k_irqreg.c"
#endif

