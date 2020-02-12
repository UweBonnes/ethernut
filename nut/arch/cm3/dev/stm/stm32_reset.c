/*
 * Copyright (C) 2016,2017 by Uwe Bonnes
 *                           (bon@elektron.ikp.physik.tu-darmstadt.de)
 *
 * All rights reserved.
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
 */

/*!
 * \file arch/cm3/dev/stm/stm32_reset.c
 *
 * \brief  Find cause of last reset.
 *
 */
#include <cfg/arch.h>
#include <dev/reset.h>

/*!
 * \brief Find cause of Reset.
 *
 * Ordering is important, PINRSTF is always set!
 * Only evaluate PINRSTF as last item.
 *
 * H7: RSR versus CSR
 * L4: No PORRSTF, handle BORRSTF as PORRSTF
 */
NUT_RSTTYP NutResetCause(void)
{
#if defined(RCC_CSR_RMVF)
# if !defined(RCC_CSR_PORRSTF)
#  define RCC_CSR_PORRSTF RCC_CSR_BORRSTF
# endif
    uint32_t csr = RCC->CSR;
    if (csr & RCC_CSR_PORRSTF)   {
        return NUT_RSTTYP_POWERUP;
    } else if (csr & (RCC_CSR_WWDGRSTF | RCC_CSR_IWDGRSTF))  {
        return NUT_RSTTYP_WATCHDOG;
    } else if (csr & RCC_CSR_LPWRRSTF) {
        return  NUT_RSTTYP_LOWPOWER;
    } else  if (csr & RCC_CSR_SFTRSTF)  {
        return NUT_RSTTYP_SOFTWARE;
# if defined(RCC_CSR_BORRSTF)
    } else if (csr & RCC_CSR_BORRSTF)   {
        return  NUT_RSTTYP_BROWNOUT;
# endif
# if defined(RCC_CSR_OBLRSTF)
    } else if (csr & RCC_CSR_OBLRSTF)   {
        return  NUT_RSTTYP_OPTIONLOADER;
# endif
# if defined(RCC_CSR_V18PWRSTF)
    } else if (csr & RCC_CSR_V18PWRSTF) {
        return  NUT_RSTTYP_COREPOWER;
# endif
# if defined(RCC_CSR_FWRSTF)
    } else if (csr & RCC_CSR_FWRSTF)    {
        return  NUT_RSTTYP_FIREWALL;
# endif
    } else if (csr & RCC_CSR_PINRSTF)   {
        return  NUT_RSTTYP_EXTERNAL;
    }
#else
    uint32_t rsr = RCC->RSR;
    if        (rsr & RCC_RSR_WDG1RSTF)  {
        return  NUT_RSTTYP_WATCHDOG;
    } else  if (rsr & RCC_RSR_SFTRSTF)  {
        return  NUT_RSTTYP_SOFTWARE;
    } else if (rsr & RCC_RSR_LPWRRSTF) {
        return  NUT_RSTTYP_LOWPOWER;
    } else if (rsr & RCC_RSR_IWDG1RSTF)  {
        return  NUT_RSTTYP_WATCHDOG;
    } else if (rsr & RCC_RSR_PINRSTF)   {
        return  NUT_RSTTYP_EXTERNAL;
    } else if (rsr & RCC_RSR_BORRSTF)   {
        return  NUT_RSTTYP_BROWNOUT;
    } else if (rsr & RCC_RSR_PORRSTF)   {
        return  NUT_RSTTYP_POWERUP;
    }
#endif
    return NUT_RSTTYP_UNKNOWN;
}

/*!
 * \brief Clear reset cause
 *
 * Typically Reset cause is queried early during boot with
 * static ram storage not yet initialized.
 * So keep the register value and only clear it when the cause
 * has been read out in the user code.
 *
 * If reset cause is evaluated, active clear must be done,
 * as reset cause is not cleared by reset and so may
 * accumulate
 */

void NutResetCauseClear(void)
{
#if defined(RCC_CSR_RMVF)
    RCC->CSR |= RCC_CSR_RMVF;
#else
    RCC->RSR |= RCC_RSR_RMVF;
#endif
}
