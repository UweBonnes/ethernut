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
 */

/*
 * $Log$
 * Revision 1.1  2003/05/09 14:41:27  haraldkipp
 * Initial revision
 *
 * Revision 1.2  2003/05/06 18:22:48  harald
 * EEPROM corruption fixed
 *
 * Revision 1.1  2003/02/04 18:15:26  harald
 * Version 3 released
 *
 */

#include <string.h>
#include <arpa/inet.h>
#include <sys/confnet.h>

CONFNET confnet;

/*!
 * \brief Load network configuration from EEPROM.
 *
 * \param name Name of the device.
 *
 * \return 0 if configuration has been read. Otherwise the
 *         return value is -1.
 */
int NutNetLoadConfig(CONST char *name)
{
    eeprom_read_block(&confnet, (void *) CONFNET_EE_OFFSET,
                      sizeof(CONFNET));
    if (confnet.cd_size == sizeof(CONFNET)
        && strcmp(confnet.cd_name, name) == 0)
        return 0;

    memset(&confnet, 0, sizeof(confnet));
    return -1;
}

/*!
 * \brief Save network configuration in EEPROM.
 *
 * \return Allways 0.
 */
int NutNetSaveConfig(void)
{
    u_char *cp;
    size_t i;

    confnet.cd_size = sizeof(CONFNET);
    for (cp = (u_char *) &confnet, i = 0; i < sizeof(CONFNET); cp++, i++)
        if(eeprom_read_byte((void *) (i + CONFNET_EE_OFFSET)) != *cp)
            eeprom_write_byte((void *) (i + CONFNET_EE_OFFSET), *cp);

    return 0;
}
