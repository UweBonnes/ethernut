/*
 * Copyright (C) 2020 Uwe Bonnes bon@elektron.ikp.physik.tu-darmstadt.de
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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

/* ToDo: Allow to deregister DMA Signalhandlers.
 */

#include <stdlib.h>

#include <cfg/arch.h>
#include <cfg/uart.h>
#include <cfg/devices.h>
#include <arch/cm3.h>
#include <dev/irqreg.h>

#include <arch/cm3/stm/stm32_uart.h>
IRQ_HANDLER sig_USART3 = {0};
IRQ_HANDLER sig_USART4 = {0};
IRQ_HANDLER sig_LPUART1 = {0};

/*!
 * \brief Stm32UsartGroupHandler
 *
 * Try to serve the source of the interrupt. GO has
 * a ITLINE bit for active interrupts.
 *
 * \param  arg: Arguments for the interrupt
 *
 */
static void Stm32UsartGroupHandler(void *arg)
{
    if ((sig_USART3.ir_handler) &&
        (SYSCFG->IT_LINE_SR[29] & SYSCFG_ITLINE29_SR_USART3_GLB)) {
#if defined(NUT_PERFMON)
        sig_USART3.ir_count++;
#endif
        sig_USART3.ir_handler(sig_USART3.ir_arg);
    }
    if ((sig_USART4.ir_handler) &&
        (SYSCFG->IT_LINE_SR[29] & SYSCFG_ITLINE29_SR_USART4_GLB)) {
#if defined(NUT_PERFMON)
        sig_USART4.ir_count++;
#endif
        sig_USART4.ir_handler(sig_USART4.ir_arg);
    }
    if ((sig_LPUART1.ir_handler) &&
        (SYSCFG->IT_LINE_SR[29] & SYSCFG_ITLINE29_SR_LPUART1_GLB)) {
#if defined(NUT_PERFMON)
        sig_LPUART1.ir_count++;
#endif
        sig_LPUART1.ir_handler(sig_LPUART1.ir_arg);
    }
}

/*!
 * \brief Install handler for USART2_X combined interrupt
 *
 * If handler is already install, do nothing
 *
 * \param  void
 *
 * \return 0 on success, anything else on failure
 */
static int Stm32UsartGroupInstallHandler(IRQ_HANDLER *group)
{
    if (group->ir_handler == NULL) {
        int ret;
        ret = NutRegisterIrqHandler(group, Stm32UsartGroupHandler, NULL);
        if (ret) {
            return -1;
        } else {
            NutIrqEnable(group);
        }
    }
    return 0;
}

/*!
 * \brief Stm32UsartInstallHandler
 *
 * For single IRQs, enable the IRQ and return the IRQ.
 * For combined IRQs, install the handler for the combined IRQ
 * and return the separated IRQ
 *
 * \param  usart_nr: Zero-based UART number.
 *         LPUART1 is STM32_LPUART1_INDEX.
 * \param  sig: Uart signal handler.
 *
 * \return 0 on success, anything else on failure
 */
IRQ_HANDLER *Stm32UsartInstallHandler(int usart_nr, IRQ_HANDLER *sig)
{
    switch (usart_nr) {
    case 0:
    case 1:
        NutIrqEnable(sig);
        break;
    case 2:
    case 3:
    case STM32_LPUART1_INDEX:
        if (Stm32UsartGroupInstallHandler(&sig_USART_GROUP)) {
            return NULL;
        }
    }
    return sig;
}

#if 0
/*!
 * \brief Handle the combined AES/RNG interrupt
 *
 * We assume for each activated device that a handler has been
 * registered before!
 * Otherwise the interrupt will be unhandled!
 *
 * \param arg  Not used
 *
 * \return None
 */
static void Stm32AesRngHandler(void *arg)
{
    if ((sig_RNG.ir_handler) &&
        (SYSCFG->IT_LINE_SR[31] & SYSCFG_ITLINE31_SR_RNG)) {
#if defined(NUT_PERFMON)
        sig_RNG.ir_count++;
#endif
        sig_RNG.ir_handler(sig_RNG.ir_arg);
    }
    if ((sig_AES.ir_handler) && SYSCFG_ITLINE31_SR_AES) {
#if defined(NUT_PERFMON)
        sig_AES.ir_count++;
#endif
        sig_AES.ir_handler(sig_AES.ir_arg);
    }
}

/*!
 * \brief Install handler for AES/RNG/LPUART1 combined interrupt
 *
 * If handler is already install, do nothing
 *
 * \param  void
 *
 * \return 0 on success, anything else on failure
 */
static int Stm32AesRngInstallHandler(void)
{
    if (sig_AES_RNG.ir_handler == NULL) {
        IRQ_HANDLER *sig = &sig_AES_RNG;
        int ret;
        ret = NutRegisterIrqHandler(sig, Stm32AesRngHandler, NULL);
        if (ret) {
            return -1;
        } else {
            NutIrqEnable(sig);
        }
    }
    return 0;
}

IRQ_HANDLER *Stm32RngInstallHandler(IRQ_HANDLER *sig)
{
    if (Stm32AesRngLpuart1InstallHandler()) {
        return NULL;
    }
    return sig;
}

IRQ_HANDLER *Stm32AesInstallHandler(IRQ_HANDLER *sig)
{
    if (Stm32AesRngLpuart1InstallHandler()) {
        return NULL;
    }
    return sig;
}

#endif
