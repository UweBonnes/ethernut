/*
 * Copyright (C) 2004 by Ole Reinhardt <ole.reinhardt@kernelconcepts.de>,
 *                       Kernelconcepts http://www.kernelconcepts.de
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

/*!
 * \file dev/can_dev.c
 * \brief Driver for CAN-Bus devices
 * 
 *
 */


/*
 * $Log$
 * Revision 1.3  2004/08/02 09:56:13  olereinhardt
 * changed typing of CAN_TryRxFrame
 *
 * Revision 1.2  2004/06/23 10:17:04  olereinhardt
 * Added buffer monitoring functions (free / avail)
 *
 * Revision 1.1  2004/06/07 15:15:28  olereinhardt
 * Initial checkin
 *
 */

/*!
 * \addtogroup xgCanDev
 */
/*@{*/


/* Not ported. */
#ifdef __GNUC__

#include <stdio.h>
#include <sys/timer.h>
#include <sys/device.h>
#include <dev/can_dev.h>

void CAN_TxFrame(NUTDEVICE *dev, CANFRAME *frame)
{
    (((IFCAN *)(dev->dev_icb))->can_send)(dev, frame);
}

u_char CAN_TryTxFrame(NUTDEVICE *dev, CANFRAME *frame)
{
    if (((IFCAN *)(dev->dev_icb))->can_txfree(dev)) {
        (((IFCAN *)(dev->dev_icb))->can_send)(dev, frame);
        return 0;
    }
    return 1;
}

u_char CAN_TxFree(NUTDEVICE *dev)
{
    return ((IFCAN *)(dev->dev_icb))->can_txfree(dev);
}

void CAN_RxFrame(NUTDEVICE *dev, CANFRAME *frame)
{
    (((IFCAN *)(dev->dev_icb))->can_recv)(dev, frame);
}

u_char CAN_TryRxFrame(NUTDEVICE *dev, CANFRAME *frame)
{
    if (((IFCAN *)(dev->dev_icb))->can_rxavail(dev)) {
        (((IFCAN *)(dev->dev_icb))->can_recv)(dev, frame);
        return 0;
    }
    return 1;
}

u_char CAN_RxAvail(NUTDEVICE *dev)
{
    return ((IFCAN *)(dev->dev_icb))->can_rxavail(dev);
}


#else

void keep_icc_happy(void)
{
}

#endif
/*@}*/
