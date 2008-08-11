/*
 * Copyright (C) 2006 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.3  2008/08/11 06:59:13  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.2  2006/05/25 09:13:22  haraldkipp
 * Platform independent watchdog API added.
 *
 * Revision 1.1  2006/02/23 15:36:35  haraldkipp
 * Added support for AT91 watchdog timer.
 *
 */

#include <sys/timer.h>
#include <dev/watchdog.h>

/*!
 * \addtogroup xgNutArchAt91DevWatchDog
 */
/*@{*/

static ureg_t nested;

/*!
 * \brief Start the AT91 hardware watch dog timer.
 *
 * For portability, applications should use the platform independent 
 * \ref xgWatchDog "Watchdog Driver API".
 */
uint32_t At91WatchDogStart(uint32_t ms, uint32_t xmode)
{
    u_int cmval;

    At91WatchDogDisable();

    /*
     * The watchdog counts down a 16 bit value, of which the upper
     * 4 bits are configurable and the lower 12 bits are set to 1.
     * Calculate the number of cycles required to count down the
     * upper 4 bits.
     */
    cmval = ((NutGetCpuClock() / 1000) * ms) >> 13;
    
    /* Check if MCK/8 is slow enough. */
    if (cmval < WD_HPCV) {
        cmval = (cmval & WD_HPCV) | WD_WDCLKS_MCK8;
    }
    /* Check if MCK/32 is slow enough. */
    else if ((cmval >>= 2) < WD_HPCV) {
        cmval = (cmval & WD_HPCV) | WD_WDCLKS_MCK32;
    }
    /* Check if MCK/128 is slow enough. */
    else if ((cmval >>= 2) < WD_HPCV) {
        cmval = (cmval & WD_HPCV) | WD_WDCLKS_MCK128;
    }
    /* Check if MCK/1024 is slow enough. */
    else if ((cmval >>= 3) < WD_HPCV) {
        cmval = (cmval & WD_HPCV) | WD_WDCLKS_MCK1024;
    }
    /* Use maximum. */
    else {
        cmval = WD_HPCV | WD_WDCLKS_MCK1024;
    }
    outr(WD_CMR, WD_CKEY | cmval);
    if (xmode == 0) {
        xmode |= WD_RSTEN;
    }
    At91WatchDogRestart();
    outr(WD_OMR, WD_OKEY | WD_WDEN | xmode);
    nested = 1;

    return ms;
}

/*!
 * \brief Re-start the AT91 hardware watch dog timer.
 *
 * For portability, applications should use the platform independent 
 * \ref xgWatchDog "Watchdog Driver API".
 */
void At91WatchDogRestart(void)
{
    outr(WD_CR, WD_RSTKEY);
}

/*!
 * \brief Disable the AT91 hardware watch dog timer.
 *
 * For portability, applications should use the platform independent 
 * \ref xgWatchDog "Watchdog Driver API".
 */
void At91WatchDogDisable(void)
{
    if (nested) {
        nested++;
    }
    outr(WD_OMR, WD_OKEY | (inr(WD_OMR) & ~WD_WDEN));
}

/*!
 * \brief Enable the AT91 hardware watch dog timer.
 *
 * For portability, applications should use the platform independent 
 * \ref xgWatchDog "Watchdog Driver API".
 */
void At91WatchDogEnable(void)
{
    if (nested > 1 && --nested == 1) {
        At91WatchDogRestart();
        outr(WD_OMR, WD_OKEY | inr(WD_OMR) | WD_WDEN);
    }
}

/*@}*/

