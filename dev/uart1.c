/*
 * Copyright (C) 2002 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.2  2004/03/18 14:01:53  haraldkipp
 * Deprecated header file removed
 *
 * Revision 1.1.1.1  2003/05/09 14:40:53  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.4  2003/02/04 17:50:55  harald
 * Version 3 released
 *
 * Revision 1.3  2003/01/14 13:43:32  harald
 * Definintions were moved
 *
 * Revision 1.2  2002/06/26 17:29:08  harald
 * First pre-release with 2.4 stack
 *
 */

#include <dev/uartavr.h>

/*!
 * \addtogroup xgUartDev
 */
/*@{*/


static UARTDCB dcb_uart1;
static IFSTREAM ifs_uart1;

/*!
 * \brief UART 1 Device information structure.
 */
NUTDEVICE devUart1 = {
    0,                          /*!< Pointer to next device. */
    {'u', 'a', 'r', 't', '1', 0, 0, 0, 0},      /*!< Unique device name. */
    IFTYP_STREAM,               /*!< Type of device. */
    1,                          /*!< Base address. */
    0,                          /*!< First interrupt number. */
    &ifs_uart1,                 /*!< Interface control block. */
    &dcb_uart1,                 /*!< Driver control block. */
    UartAvrInit,                /*!< Driver initialization routine. */
    UartAvrIOCtl,               /*!< Driver specific control function. */
    UartAvrRead,
    UartAvrWrite,
    UartAvrWrite_P,
    UartAvrOpen,
    UartAvrClose,
    0
};

/*@}*/
