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
 * Revision 1.1.1.1  2003/05/09 14:41:48  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.2  2003/02/04 18:15:56  harald
 * Version 3 released
 *
 * Revision 1.1  2003/01/17 18:55:43  harald
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
 * \brief Write a buffer to a specified device.
 *
 * This is a raw output without any character translation
 * like EOL (end of line).
 *
 * \deprecated Use _write() or fwrite() in new programs.
 *
 * \param dev  Identifies the device to write to. A null pointer may be used 
 *             for unbuffered output to the first on-chip UART. If this 
 *             pointer is not null, it must have been retrieved by directly 
 *             or indirectly calling NutDeviceOpen() for real devices. For 
 *             virtual devices this pointer is returned by the function that 
 *             creates the device.
 * \param data Buffer to be written.
 * \param len  Number of characters to be printed.
 *
 * \return If successful, the number of bytes added to the
 *         output buffer. This may be less than the specified 
 *         number of bytes to print. The return value -1 
 *         indicates an error.
 */
int NutDeviceWrite(NUTDEVICE * dev, CONST void *data, int len)
{
    if (dev == 0) {
        if (data >= RAMSTART && len > 0)
            return UartAvrPutRaw(*((char *) data));
    } else if (dev->dev_type == IFTYP_STREAM)
        return NutIfStreamWrite(dev, data, len);

    return -1;
}

/*!
 * \brief Write a buffer in program space to a specified device.
 *
 * This is a raw output without any character translation
 * like EOL (end of line).
 *
 * \deprecated Use _write_P() or fwrite_P() in new programs..
 *
 * \param dev  Identifies the device to write to. A null pointer may be used 
 *             for unbuffered output to the first on-chip UART. If this 
 *             pointer is not null, it must have been retrieved by directly 
 *             or indirectly calling NutDeviceOpen() for real devices. For 
 *             virtual devices this pointer is returned by the function that 
 *             creates the device.
 * \param data Buffer to be written.
 * \param len  Number of characters to be printed.
 *
 * \return If successful, the number of bytes added to the
 *         output buffer. This may be less than the specified 
 *         number of bytes to print. The return value -1 
 *         indicates an error.
 */
int NutDeviceWrite_P(NUTDEVICE * dev, PGM_P data, int len)
{
    if (dev == 0) {
        if (len > 0)
            return UartAvrPutRaw(PRG_RDB(data));
    } else if (dev->dev_type == IFTYP_STREAM)
        return NutIfStreamWrite_P(dev, data, len);

    return -1;
}

/*@}*/
