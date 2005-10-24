/*
 * Copyright (C) 2005 by egnite Software GmbH. All rights reserved.
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
 * \file arch/avr/dev/ivect35.c
 * \brief CAN serial transfer interrupt.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.3  2005/10/24 09:35:48  haraldkipp
 * New interrupt control function added to allow future platform
 * independant drivers.
 *
 * Revision 1.2  2005/10/04 05:23:37  hwmaier
 * Updated interrupt hooks to reflect new CAN interrupt names of avr-lib 1.2.3
 *
 * Revision 1.1  2005/07/26 18:02:40  haraldkipp
 * Moved from dev.
 *
 * Revision 1.1  2005/02/10 07:06:18  hwmaier
 * Changes to incorporate support for AT90CAN128 CPU
 *
 * \endverbatim
 */

#include <dev/irqreg.h>

/*!
 * \addtogroup xgIrqReg
 */
/*@{*/

#if defined(SIG_CAN_INTERRUPT1) || defined(iv_CAN_TRANSFER)

static int AvrCanTxIrqCtl(int cmd, void *param);

IRQ_HANDLER sig_CAN_TRANSFER = {
#ifdef NUT_PERFMON
    0,                          /* Interrupt counter, ir_count. */
#endif
    NULL,                       /* Passed argument, ir_arg. */
    NULL,                       /* Handler subroutine, ir_handler. */
    AvrCanTxIrqCtl              /* Interrupt control, ir_ctl. */
};

/*!
 * \brief CAN serial transfer interrupt control.
 *
 * \param cmd   Control command.
 * \param param Pointer to optional parameter.
 *
 * \return 0 on success, -1 otherwise.
 */
int AvrCanTxIrqCtl(int cmd, void *param)
{
    return -1;
}

/*! \fn SIG_CAN_INTERRUPT1(void)
 * \brief CAN serial transfer interrupt entry.
 */
#ifdef __IMAGECRAFT__
#pragma interrupt_handler SIG_CAN_INTERRUPT1:iv_CAN_TRANSFER
#endif
NUTSIGNAL(SIG_CAN_INTERRUPT1, sig_CAN_TRANSFER)
#endif
/*@}*/
