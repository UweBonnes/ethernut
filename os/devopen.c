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
 * ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
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
 * Revision 1.4  2003/04/21 17:05:42  harald
 * Device init moved to registration
 *
 * Revision 1.3  2003/03/31 14:53:30  harald
 * Prepare release 3.1
 *
 * Revision 1.2  2003/02/04 18:15:56  harald
 * Version 3 released
 *
 * Revision 1.1  2003/01/17 18:55:12  harald
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
 * \brief Open a device by name.
 *
 * This function must be called after registering the device with
 * NutRegisterDevice() and before calling any read or write
 * function.
 *
 * \deprecated Use _open() or fopen() in new programs.
 *
 * \param name Case sensitiv name of the device. This name is fixed by the 
 *             specific device driver.
 *
 * \return Pointer to the device structure or null if the device
 *         has not been registered.
 */
NUTDEVICE *NutDeviceOpen(CONST char *name)
{
    return NutDeviceLookup(name);
}

/*@}*/
