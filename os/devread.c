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
 * Revision 1.2  2004/03/16 16:48:45  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:47  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.3  2003/04/21 17:06:02  harald
 * *** empty log message ***
 *
 * Revision 1.2  2003/02/04 18:15:56  harald
 * Version 3 released
 *
 * Revision 1.1  2003/01/17 18:55:18  harald
 * Device routines splitted
 *
 */

#include <string.h>

#include <sys/ifstream.h>
#include <dev/uart.h>

/*!
 * \addtogroup xgDevice
 */
/*@{*/

/*!
 * \brief Read up to a specified number of bytes from a device.
 *
 * The function may read fewer than the given number of bytes.
 *
 * \deprecated Use _read() or fread() in new programs.
 *
 * \param dev  Identifies the device to read from. A null pointer may be used 
 *             for unbuffered input from the first on-chip UART. If this 
 *             pointer is not null, it must have been retrieved by directly 
 *             or indirectly calling NutDeviceOpen() for real devices. For 
 *             virtual devices this pointer is returned by the function that 
 *             creates the device.
 * \param data Pointer to the buffer that receives the data.
 * \param size Size of the buffer.
 *
 * \return The number of bytes read, 0 on timeouts and broken
 *         connections or -1 in case of an error.
 */
int NutDeviceRead(NUTDEVICE * dev, void *data, int size)
{
    if (dev == 0) {
        if (data >= RAMSTART && size > 0)
            return UartAvrGetRaw(data);
    } else if (dev->dev_type == IFTYP_STREAM)
        return NutIfStreamRead(dev, data, size);

    return -1;
}

/*!
 * \brief Get a line from a specified device.
 *
 * Reads a string from the specified device. Characters are
 * read up to and including the first newline character, up
 * to the end of the stream, or until the number of characters
 * read is equal to the specified size, whichever comes first.
 *
 * \deprecated Use fscanf() in new programs.
 *
 * \param dev  Identifies the device to read from. A null pointer may be used 
 *             for unbuffered output to the first on-chip UART. If this 
 *             pointer is not null, it must have been retrieved by directly 
 *             or indirectly calling NutDeviceOpen() for real devices. For 
 *             virtual devices this pointer is returned by the function that 
 *             creates the device.
 * \param data Pointer to the buffer that receives the data.
 * \param size Size of the buffer.
 *
 * \return The number of bytes read or -1 in case of an error.
 */
int NutDeviceGetLine(NUTDEVICE * dev, void *data, int size)
{
    if (dev->dev_type == IFTYP_STREAM)
        return NutIfStreamGetLine(dev, data, size);

    return -1;
}

/*@}*/
