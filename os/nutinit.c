/*
 * Copyright (C) 2001-2005 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.11  2005/01/22 19:29:56  haraldkipp
 * Initializing ms62_5 will move it to the data segment. Still no final
 * solution to make sure it's kept in AVR's internal memory. But helps
 * in most cases.
 *
 * Revision 1.10  2005/01/13 18:56:04  haraldkipp
 * Moved ms62_5 counter from timer.c to make sure this is located in internal
 * RAM (AVR platforms). This fixes the wrong baudrate bug for applications
 * occupying all internal RAM.
 *
 * Revision 1.9  2004/08/04 23:10:30  freckle
 * added an avr-libc compatible (but yet uncomplete) eeprom simulation that
 * uses file 'eeprom.bin' in the current directory to store eeprom content
 *
 * Revision 1.8  2004/04/07 12:13:58  haraldkipp
 * Matthias Ringwald's *nix emulation added
 *
 * Revision 1.7  2004/03/16 16:48:45  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.6  2004/03/03 17:52:26  drsung
 * New field 'hostname' added to structure confos.
 *
 * Revision 1.5  2004/02/18 13:51:06  drsung
 * Changed memory calculations to use u_short. Makes more sense than using signed integers, especially on hardware with more than 32KB static RAM...
 *
 * Revision 1.4  2003/12/15 19:30:19  haraldkipp
 * Thread termination support
 *
 * Revision 1.3  2003/12/05 22:39:46  drsung
 * New external RAM handling
 *
 * Revision 1.2  2003/09/29 16:35:25  haraldkipp
 * Replaced XRAMEND by NUTRAMEND
 *
 * Revision 1.1.1.1  2003/05/09 14:41:51  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.4  2003/05/06 18:53:43  harald
 * ICCAVR port
 *
 * Revision 1.3  2003/04/21 17:08:34  harald
 * Local var removed from naked function
 *
 * Revision 1.2  2003/03/31 14:39:05  harald
 * Fix GCC different handling of main
 *
 * Revision 1.1  2003/02/04 18:17:07  harald
 * Version 3 released
 *
 */

#define __NUTINIT__
#include <compiler.h>
#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/timer.h>

#include <sys/confos.h>
#include <string.h>

/*!
 * \brief System tick counter.
 *
 * For the time being we put this here to ensure that it will be placed
 * in lower RAM. This is essential for the AVR platform, where we use
 * this counter to determine the system clock and calculate the correct
 * baudrate factors. If this counter would be placed in external RAM,
 * additional wait states may apply.
 *
 * \todo Layout for internal RAM and place this into a special link segment.
 */
volatile u_char ms62_5 = 0;

#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega103__)
#include "arch/avr_nutinit.c"
#elif defined(__arm__)
#include "arch/arm_nutinit.c"
#elif defined(__H8300H__) || defined(__H8300S__)
#include "arch/h8_nutinit.c"
#elif defined(__m68k__)
#include "arch/m68k_nutinit.c"
#elif defined(__linux__) || defined(__APPLE__)
// avoid stdio nut wrapper */
#define NO_STDIO_NUT_WRAPPER
#include "arch/unix_nutinit.c"
#include "arch/unix_options.c"
#include "arch/unix_eeprom.c"
#endif

/*@}*/
