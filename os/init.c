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
 *
 * -
 * Portions Copyright (C) 2000 David J. Hudson <dave@humbug.demon.co.uk>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "copying-gpl.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "copying-liquorice.txt" for details.
 */

/*
 * $Log$
 * Revision 1.5  2004/03/03 17:52:26  drsung
 * New field 'hostname' added to structure confos.
 *
 * Revision 1.4  2004/02/18 13:51:06  drsung
 * Changed memory calculations to use u_short. Makes more sense than using signed integers, especially on hardware with more than 32KB static RAM...
 *
 * Revision 1.3  2004/01/04 19:24:21  drsung
 * Thread termination support
 *
 * Revision 1.2  2003/12/05 22:39:14  drsung
 * Using heap_start instead of bss_end
 *
 * Revision 1.1.1.1  2003/05/09 14:41:51  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.22  2003/03/31 14:53:30  harald
 * Prepare release 3.1
 *
 * Revision 1.21  2003/02/04 18:15:57  harald
 * Version 3 released
 *
 * Revision 1.20  2003/01/14 16:57:11  harald
 * New compiler
 *
 * Revision 1.19  2002/11/02 15:19:01  harald
 * Volatile bug fixed
 *
 * Revision 1.18  2002/10/29 15:35:24  harald
 * Detect external RAM
 *
 * Revision 1.17  2002/06/26 17:29:44  harald
 * First pre-release with 2.4 stack
 *
 */

#define __NUTINIT__
#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/timer.h>

#include <sys/confos.h>
#include <string.h>

CONFOS confos;

/*!
 * \addtogroup xgOldInit
 */
/*@{*/

extern void NutMain(void *arg) __attribute__ ((noreturn));

/*! \fn NutIdle(void *arg)
 * \brief Idle thread. 
 */
THREAD(NutIdle, arg)
{
    NutTimerInit();

    NutThreadCreate("main", NutMain, 0, 768);
    NutThreadSetPriority(254);
    for (;;) {
        NutThreadYield();
        NutThreadDestroy();
    }
}

/*!
 * \brief Load Nut/OS configuration from the EEPROM.
 *
 * This routine is automatically called during system
 * initialization.
 *
 * \return 0 if OK, -1 if configuration isn't available.
 */
int NutLoadConfig(void)
{
    eeprom_read_block(&confos, CONFOS_EE_OFFSET, sizeof(CONFOS));
    if (confos.size != sizeof(CONFOS) || confos.magic[0] != 'O'
        || confos.magic[1] != 'S') {
        return -1;
    }
    return 0;
}

/*!
 * \brief Save Nut/OS configuration in the EEPROM.
 *
 * \return 0 if OK, -1 on failures.
 */
int NutSaveConfig(void)
{
    u_char *cp;
    u_short i;

    confos.size = sizeof(CONFOS);
    confos.magic[0] = 'O';
    confos.magic[1] = 'S';
    for (cp = (u_char *) & confos, i = 0; i < sizeof(CONFOS); cp++, i++)
        if (eeprom_read_byte((void *) (i + CONFOS_EE_OFFSET)) != *cp)
            eeprom_write_byte((void *) (i + CONFOS_EE_OFFSET), *cp);

    return 0;
}

/*!
 * \brief Nut/OS main entry.
 *
 * \deprecated New programs should link to nutinit.
 *
 * This is the entry point for C programs. To hide neccessary
 * system initialization from the user program, this entry point
 * is included in the Nut/OS library as a separate object module.
 *
 * It will initialize memory management and the thread system
 * and start an idle thread, which in turn initializes the
 * timer functions. Finally NutMain() is called, which must
 * be defined by the application and may be used to start
 * additional application threads.
 * \internal
 */
int main(void)
{
    register volatile u_char *xramend = (volatile u_char *) 0x7FFF;

#ifdef NUTDEBUG
    outp(7, UBRR);
    outp(BV(RXEN) | BV(TXEN), UCR);
#endif

    /*
     * Read eeprom configuration.
     */
    if (NutLoadConfig())
    {
    	strcpy (confos.hostname, "ethernut");
        NutSaveConfig();
    }

    outp(BV(SRE) | BV(SRW), MCUCR);

    if ((u_short) RAMEND - (u_short) (&__heap_start) > 384)
        NutHeapAdd(&__heap_start,
                   (u_short) RAMEND - 256 - (u_short) (&__heap_start));

    *(xramend - 1) = 0x55;
    *xramend = 0xAA;
    if (*(xramend - 1) == 0x55 && *xramend == 0xAA)
        NutHeapAdd((void *) (RAMEND + 1), (u_short) xramend - RAMEND);

    NutThreadCreate("idle", NutIdle, 0, 384);

    return 0;
}

/*@}*/
