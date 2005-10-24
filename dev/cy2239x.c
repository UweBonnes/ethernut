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
 * \file dev/cy2239x.c
 * \brief Routines for the Cypress CY22393/4/5 clock chips.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2005/10/24 10:21:57  haraldkipp
 * Initial check in.
 *
 *
 * \endverbatim
 */

#include <sys/event.h>
#include <dev/twif.h>
#include <dev/cy2239x.h>

#ifndef I2C_SLA_PLL
#define I2C_SLA_PLL     0x69
#endif

#ifndef PLL_BASE_FREQ
#define PLL_BASE_FREQ   25000000UL
#endif

/*!
 * \brief Calculate PLL frequency.
 *
 * \parm reg Points to an array with the three PLL registers.
 *
 * \return Frequency in Hertz.
 */
static u_long PllFreq(u_char * reg)
{
    u_long p;
    u_long pt;
    u_long qt;

    p = (u_long) (reg[2] & 0x03) << 8 | reg[1];
    pt = 2 * (p + 3) + ((reg[2] >> 2) & 1);
    qt = reg[0] + 2;

    return (((PLL_BASE_FREQ * 10UL + 5UL) / qt) * pt) / 10UL;
}

/*!
 * \brief Get the frequency of a specified output.
 *
 * On Ethernut 3 clock A provides the Ethernet Controller clock
 * and clock C is used as the CPU clock.
 *
 * \param clk  Specifies the output. 0 = ClkA, 1 = ClkB etc.
 * \param mode Level on the S2 input, 0 or 1.
 *
 * \return Frequency in Hertz.
 */
u_long Cy2239xGetFreq(int clk, int mode)
{
    u_long rc;
    u_long d;
    u_char clk_loc = 0x08;
    u_char clk_reg[8];
    u_char pll_loc[4] = { 0x49, 0x55, 0x11, 0x14 };
    u_char pll_reg[3];
    int div_idx = clk + 2;
    int pll_idx;

    if (TwMasterTransact(I2C_SLA_PLL, &clk_loc, 1, clk_reg, 8, NUT_WAIT_INFINITE) != 8) {
        return 0;
    }
    if (clk == 4) {
        pll_idx = 1;
    } else {
        pll_idx = (clk_reg[6] >> (2 * clk)) & 0x03;
    }
    if (pll_idx) {
        if (pll_idx == 1) {
            pll_idx = mode;
        }
        if (TwMasterTransact(I2C_SLA_PLL, &pll_loc[pll_idx], 1, pll_reg, 3, NUT_WAIT_INFINITE) != 3) {
            return 0;
        }
        rc = PllFreq(pll_reg);
        if (pll_idx < 2 && clk < 2) {
            div_idx = clk * 2 + (pll_reg[2] >> 7);
        }
    } else {
        rc = PLL_BASE_FREQ;
    }
    if (div_idx < 6) {
        d = clk_reg[div_idx] & 0x7F;
    } else {
        d = clk_reg[7] & 3;
        if (d == 1) {
            d = 4;
        }
    }
    if (d) {
        rc /= d;
    } else {
        rc = 0;
    }
    return rc;
}
