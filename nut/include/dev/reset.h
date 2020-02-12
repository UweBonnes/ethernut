#ifndef _DEV_RESET_H_
#define _DEV_RESET_H_

/*
 * Copyright (C) 2008 by egnite GmbH.
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
 * \file dev/reset.h
 * \brief System reset function.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <compiler.h>

/*!
 * \addtogroup xgRese
 *
 * There is only on valid reset reason,
 * but some archs may still return a bitmap
 */
/*@{*/

typedef enum {
    NUT_RSTTYP_UNKNOWN         = 0,
    NUT_RSTTYP_POWERUP         = 1,
    NUT_RSTTYP_WATCHDOG        = 2,
    NUT_RSTTYP_EXTERNAL        = 4,
    NUT_RSTTYP_SOFTWARE        = 8,
    NUT_RSTTYP_BROWNOUT        = 0x10,
    NUT_RSTTYP_LOWPOWER        = 0x20,
    NUT_RSTTYP_COREPOWER       = 0x40,
    NUT_RSTTYP_FIREWALL        = 0x80,
    NUT_RSTTYP_OPTIONLOADER    = 0x100,
    NUT_RSTTYP_BACKUP_WATCHDOG = 0x200,
    NUT_RSTTYP_LOSS_OF_LOCK    = 0x400,
    NUT_RSTTYP_LOSS_OF_CLOCK   = 0x1000
}NUT_RSTTYP;

#if defined(MCU_AT91)
#include <arch/arm/atmel/at91_reset.h>
#elif defined(__AVR32__)
#include <arch/avr32/reset.h>
#elif defined(__CORTEX__)
#include <arch/cm3/cortexM3.h>
#elif defined(__m68k__)
#include <arch/m68k/reset.h>
#endif

extern void NutReset(void);
extern NUT_RSTTYP NutResetCause(void);
extern void NutResetCauseClear(void);
/*@}*/

#endif
