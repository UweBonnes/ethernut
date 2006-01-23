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
 * Revision 1.8  2006/01/23 19:50:48  haraldkipp
 * Dummy NVMEM configuration for GBA added.
 *
 * Revision 1.7  2006/01/23 17:26:17  haraldkipp
 * Platform independant routines added, which provide generic access to
 * non-volatile memory.
 *
 * Revision 1.6  2005/07/26 15:49:59  haraldkipp
 * Cygwin support added.
 *
 * Revision 1.5  2004/09/08 10:24:34  haraldkipp
 * No EEPROM support for AT91
 *
 * Revision 1.4  2004/04/07 12:13:58  haraldkipp
 * Matthias Ringwald's *nix emulation added
 *
 * Revision 1.3  2004/03/16 16:48:45  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.2  2004/03/03 17:42:19  drsung
 * Bugfix in NutSaveConfig. Write only to eeprom if actual byte in
 * eeprom differs from byte to write.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:46  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.1  2003/02/04 18:17:07  harald
 * Version 3 released
 *
 */

#include <cfg/arch.h>
#include <sys/confos.h>
#include <dev/nvmem.h>

CONFOS confos;

/*!
 * \brief Load Nut/OS configuration from non volatile memory.
 *
 * This routine is automatically called during system
 * initialization.
 *
 * \return 0 if OK, -1 if configuration isn't available.
 */
int NutLoadConfig(void)
{
#if !defined(__linux__) && !defined(__APPLE__) && !defined(__CYGWIN__) && !defined(MCU_GBA)
    if (NutNvMemLoad(CONFOS_EE_OFFSET, &confos, sizeof(CONFOS))) {
        return -1;
    }
    if (confos.size != sizeof(CONFOS) || confos.magic[0] != 'O' || confos.magic[1] != 'S') {
        return -1;
    }
#endif
    return 0;
}

/*!
 * \brief Save Nut/OS configuration in non volatile memory.
 *
 * \return 0 if OK, -1 on failures.
 */
int NutSaveConfig(void)
{
#if !defined(__linux__) && !defined(__APPLE__) && !defined(__CYGWIN__) && !defined(MCU_GBA)
    confos.size = sizeof(CONFOS);
    confos.magic[0] = 'O';
    confos.magic[1] = 'S';
    if (NutNvMemSave(CONFOS_EE_OFFSET, &confos, sizeof(CONFOS))) {
        return -1;
    }
#endif
    return 0;
}
