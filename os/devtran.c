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
 * Revision 1.1  2003/05/09 14:41:48  haraldkipp
 * Initial revision
 *
 * Revision 1.3  2003/02/04 18:15:56  harald
 * Version 3 released
 *
 * Revision 1.2  2003/01/17 18:55:36  harald
 * Device routines splitted
 *
 * Revision 1.1  2002/10/29 15:39:22  harald
 * Special function removed from standard lib
 *
 */

#include <string.h>

#include <sys/ifstran.h>
#include <sys/devtran.h>

/*!
 * \addtogroup xgDevTran
 */
/*@{*/

/*!
 * \brief Read up to a specified number of bytes from a device 
 *        translating non printables.
 *
 * This call works like NutDeviceRead(), but translates
 * character codes below 33, above 126 or equal 34 into
 * their hex representation prepended by a backslash and the
 * letter x. Backslashes are duplicated and the complete string
 * is terminated by character code zero. This offers the ability 
 * to store binary data in printable strings.
 *
 * \deprecated Will be removed.
 *
 * \param dev Identifies the device that read from.
 *            A null pointer specifies the first on-chip UART.
 *            If this pointer is not null, it must have been 
 *            retrieved by using the NutDeviceOpen() function.
 * \param data Pointer to the buffer that receives the translated string.
 * \param size Size of the buffer.
 *
 * \return The number of bytes read, 0 on timeouts and broken
 *         connections or -1 in case of an error. Due to code 
 *         translation, the number of bytes read be less than 
 *         the number of characters returned in the string.
 */
int NutDeviceReadTran(NUTDEVICE * dev, void *data, int size)
{
    if (dev->dev_type == IFTYP_STREAM)
        return NutIfStreamReadTran(dev, data, size);

    return -1;
}

/*!
 * \brief Write a buffer to a specified device using character
 *        translation for non printables.
 *
 * This call works like NutDeviceWrite(), but translates hex 
 * representations prepended by a backslash and the letter x to 
 * equivalent characters. Duplicate backslashes are converted to a 
 * single backslash.
 *
 * \deprecated Will be removed.
 *
 * \param dev  Identifies the device to write to. A null pointer may be used 
 *             for unbuffered output to the first on-chip UART. If this 
 *             pointer is not null, it must have been retrieved by directly 
 *             or indirectly calling NutDeviceOpen() for real devices. For 
 *             virtual devices this pointer is returned by the function that 
 *             creates the device.
 * \param data Buffer to be written.
 *
 * \return If successful, the number of bytes added to the
 *         output buffer. This may be less than the specified 
 *         number of bytes to print. The return value -1 
 *         indicates an error.
 */
int NutDeviceWriteTran(NUTDEVICE * dev, CONST void *data)
{
    if (dev->dev_type == IFTYP_STREAM)
        return NutIfStreamWriteTran(dev, data);

    return -1;
}

/*@}*/
