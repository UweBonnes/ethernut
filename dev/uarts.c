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
 * Revision 1.2  2004/03/18 14:00:12  haraldkipp
 * Comments updated
 *
 * Revision 1.1.1.1  2003/05/09 14:40:55  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.8  2003/02/04 17:50:55  harald
 * Version 3 released
 *
 * Revision 1.7  2002/06/26 17:29:08  harald
 * First pre-release with 2.4 stack
 *
 */

#include <dev/uartspi.h>

/*!
 * \addtogroup xgUartsDev
 */
/*@{*/


static UARTSDCB dcb_uart1;
static UARTSDCB dcb_uart2;
static UARTSDCB dcb_uart3;
static UARTSDCB dcb_uart4;
static UARTSDCB dcb_uart5;
static UARTSDCB dcb_uart6;
static UARTSDCB dcb_uart7;
static UARTSDCB dcb_uart8;

/*!
 * \brief Device information structure.
 *
 * Applications must pass this structure to NutRegisterDevice() 
 * to bind this serial port device driver to the Nut/OS kernel.
 */
NUTDEVICE devUarts[] = {
    {
     0,                         /*!< Pointer to next device. */
     {'u', 'a', 'r', 't', 's', '0', 0, 0, 0},   /*!< Unique device name. */
     IFTYP_STREAM,              /*!< Type of device. */
     1,                         /*!< Base address. */
     0,                         /*!< First interrupt number. */
     0,                         /*!< Interface control block. */
     &dcb_uart1,                /*!< Driver control block. */
     UartSpiInit,               /*!< Driver initialization routine. */
     UartSpiIOCtl               /*!< Driver specific control function. */
     },

    {
     0,                         /*!< Pointer to next device. */
     {'u', 'a', 'r', 't', 's', '1', 0, 0, 0},   /*!< Unique device name. */
     IFTYP_STREAM,              /*!< Type of device. */
     2,                         /*!< Base address. */
     0,                         /*!< First interrupt number. */
     0,                         /*!< Interface control block. */
     &dcb_uart2,                /*!< Driver control block. */
     UartSpiInit,               /*!< Driver initialization routine. */
     UartSpiIOCtl               /*!< Driver specific control function. */
     },

    {
     0,                         /*!< Pointer to next device. */
     {'u', 'a', 'r', 't', 's', '2', 0, 0, 0},   /*!< Unique device name. */
     IFTYP_STREAM,              /*!< Type of device. */
     3,                         /*!< Base address. */
     0,                         /*!< First interrupt number. */
     0,                         /*!< Interface control block. */
     &dcb_uart3,                /*!< Driver control block. */
     UartSpiInit,               /*!< Driver initialization routine. */
     UartSpiIOCtl               /*!< Driver specific control function. */
     },

    {
     0,                         /*!< Pointer to next device. */
     {'u', 'a', 'r', 't', 's', '3', 0, 0, 0},   /*!< Unique device name. */
     IFTYP_STREAM,              /*!< Type of device. */
     4,                         /*!< Base address. */
     0,                         /*!< First interrupt number. */
     0,                         /*!< Interface control block. */
     &dcb_uart4,                /*!< Driver control block. */
     UartSpiInit,               /*!< Driver initialization routine. */
     UartSpiIOCtl               /*!< Driver specific control function. */
     },

    {
     0,                         /*!< Pointer to next device. */
     {'u', 'a', 'r', 't', 's', '4', 0, 0, 0},   /*!< Unique device name. */
     IFTYP_STREAM,              /*!< Type of device. */
     5,                         /*!< Base address. */
     0,                         /*!< First interrupt number. */
     0,                         /*!< Interface control block. */
     &dcb_uart5,                /*!< Driver control block. */
     UartSpiInit,               /*!< Driver initialization routine. */
     UartSpiIOCtl               /*!< Driver specific control function. */
     },

    {
     0,                         /*!< Pointer to next device. */
     {'u', 'a', 'r', 't', 's', '5', 0, 0, 0},   /*!< Unique device name. */
     IFTYP_STREAM,              /*!< Type of device. */
     6,                         /*!< Base address. */
     0,                         /*!< First interrupt number. */
     0,                         /*!< Interface control block. */
     &dcb_uart6,                /*!< Driver control block. */
     UartSpiInit,               /*!< Driver initialization routine. */
     UartSpiIOCtl               /*!< Driver specific control function. */
     },

    {
     0,                         /*!< Pointer to next device. */
     {'u', 'a', 'r', 't', 's', '6', 0, 0, 0},   /*!< Unique device name. */
     IFTYP_STREAM,              /*!< Type of device. */
     7,                         /*!< Base address. */
     0,                         /*!< First interrupt number. */
     0,                         /*!< Interface control block. */
     &dcb_uart7,                /*!< Driver control block. */
     UartSpiInit,               /*!< Driver initialization routine. */
     UartSpiIOCtl               /*!< Driver specific control function. */
     },

    {
     0,                         /*!< Pointer to next device. */
     {'u', 'a', 'r', 't', 's', '7', 0, 0, 0},   /*!< Unique device name. */
     IFTYP_STREAM,              /*!< Type of device. */
     8,                         /*!< Base address. */
     0,                         /*!< First interrupt number. */
     0,                         /*!< Interface control block. */
     &dcb_uart8,                /*!< Driver control block. */
     UartSpiInit,               /*!< Driver initialization routine. */
     UartSpiIOCtl               /*!< Driver specific control function. */
     },
};

/*@}*/
