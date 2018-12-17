/*
 * Copyright (C) 2017 by Uwe Bonnes
 *                              (bon@elektron.ikp.physik.tu-darmstadt.de)
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

#include <sys/nutdebug.h>

#include <arch/cm3.h>

#include <arch/cm3/timer.h>
#include <arch/cm3/stm/stm32_clk.h>
#include <cfg/clock.h>
#include <dev/rtc.h>
#include <dev/irqreg.h>

#include <arch/cm3/stm/stm32xxxx.h>

#define HSI64_VALUE 64000000

/* Define PLL Input Clock */
# define PLLCLK_IN (HSI64_VALUE / 8)

/* VCO limits for H7*/
# define PLLVCO_MAX      836000000
# define PLLVCO_MIN      192000000

//static uint32_t sys_clock;
//static uint8_t clk_shift[HWCLK_SYS];

uint32_t Stm32ClockGet(clock_index_t idx)
{
    (void) idx;
    return HSI64_VALUE;
}

/*!
 * \brief  Configures the System clock source: HSI, HSE or PLL.
 * \note   This function should be used only with SYSCLK == HSI or
 * \note   HSE and peripheralks clocks disabled
 * \param  src is one of SYSCLK_HSE, SYSCLK_HSI or SYSCLK_PLL.
 * \return 0 if selected clock is running else -1.
 */
static int SetSysClockSource(int src)
{
    (void) src;
    return 0;
}

int SetSysClock(void)
{
    return SetSysClockSource(SYSCLK_SOURCE);
}
