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
 * Revision 1.5  2005/07/26 15:49:59  haraldkipp
 * Cygwin support added.
 *
 * Revision 1.4  2005/02/10 07:06:50  hwmaier
 * Changes to incorporate support for AT90CAN128 CPU
 *
 * Revision 1.3  2004/04/07 12:13:58  haraldkipp
 * Matthias Ringwald's *nix emulation added
 *
 * Revision 1.2  2003/07/13 19:03:06  haraldkipp
 * Make empty MAC broadcast.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:27  haraldkipp
 * Initial using 3.2.1
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
 * If no configuration is available in EEPROM, all configuration
 * parameters are cleared to zero. Except the MAC address, which
 * is set to the Ethernet broadcast address.
 *
 * \param name Name of the device.
 *
 * \return 0 if configuration has been read. Otherwise the
 *         return value is -1.
 */
int NutNetLoadConfig(CONST char *name)
{
/*
 * Note: eeprom is currently disabled for AT90CAN128 MCU as the current
 * avr-libc 1.2 and earlier versions do not support this function!
 * Refer to: http://lists.gnu.org/archive/html/avr-libc-dev/2004-04/msg00108.html
 */
#if defined(__AVR_AT90CAN128__)
/* TODO FIXME ttt */
#warning NutNetLoadConfig is not yet supported with this device!
#else
#if !defined(__linux__) && !defined(__APPLE__) && !defined(__CYGWIN__)
    eeprom_read_block(&confnet, (void *) CONFNET_EE_OFFSET, sizeof(CONFNET));
    if (confnet.cd_size == sizeof(CONFNET)
        && strcmp(confnet.cd_name, name) == 0)
        return 0;
#endif
#endif
    memset(&confnet, 0, sizeof(confnet));

    /*
     * Set initial MAC address to broadcast. Thanks to Tomohiro
     * Haraikawa, who pointed out that all zeroes is occupied by
     * Xerox and should not be used.
     */
    memset(confnet.cdn_mac, 0xFF, sizeof(confnet.cdn_mac));

    return -1;
}

/*!
 * \brief Save network configuration in EEPROM.
 *
 * \return Allways 0.
 */
int NutNetSaveConfig(void)
{
/*
 * Note: eeprom is currently disabled for AT90CAN128 MCU as the current
 * avr-libc 1.2 and earlier versions do not support this function!
 * Refer to: http://lists.gnu.org/archive/html/avr-libc-dev/2004-04/msg00108.html
 */
#if defined(__AVR_AT90CAN128__)
/* TODO FIXME ttt */
#warning NutNetLoadConfig is not yet supported with this device!
#else
#if !defined(__linux__) && !defined(__APPLE__) && !defined(__CYGWIN__)
    u_char *cp;
    size_t i;

    confnet.cd_size = sizeof(CONFNET);
    for (cp = (u_char *) & confnet, i = 0; i < sizeof(CONFNET); cp++, i++)
        if (eeprom_read_byte((void *) (i + CONFNET_EE_OFFSET)) != *cp)
            eeprom_write_byte((void *) (i + CONFNET_EE_OFFSET), *cp);

#endif
#endif
    return 0;
}
