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
 * Revision 1.1  2003/05/09 14:41:47  haraldkipp
 * Initial revision
 *
 * Revision 1.2  2003/02/04 18:15:56  harald
 * Version 3 released
 *
 * Revision 1.1  2003/01/17 18:55:06  harald
 * Device routines splitted
 *
 */

#include <string.h>

#include <sys/ifstream.h>
#include <sys/uart.h>

/*!
 * \addtogroup xgDevice
 */
/*@{*/

/*!
 * \brief Perform device specific control functions.
 *
 * \deprecated Use _iocntl() in new programs.
 *
 * \param dev  Identifies the device to control. This pointer must have 
 *             been retrieved by directly or indirectly calling 
 *             NutDeviceOpen().
 * \param req  Requested control function.
 * \param conf Points to a buffer that contains any data required for
 *             the given control function or receives data from that
 *             function.
 */
int NutDeviceIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    if (dev == 0)
        return UartAvrIOCtl(0, req, conf);
    else if ((void *) dev >= RAMSTART && dev->dev_ioctl)
        return (*dev->dev_ioctl) (dev, req, conf);

    return -1;
}

/*@}*/
