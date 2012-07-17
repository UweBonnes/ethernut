#ifndef _OWIBUS_H_
#define _OWIBUS_H_

/*
 * Copyright (C) 2012 by Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
 *
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
 * \verbatim
 * $Id$
 * \endverbatim
 */

/*!
 * \file dev/owibus.c
 * \brief Header for the One-Wire API
 */

#include <cfg/arch.h>
#include <stdint.h>

/* Overdrive Commands as in
 * http://www.ovro.caltech.edu/~dwh/correlator/pdf/onewire.pdf
 */
/*!
 * \brief One-Wire command codes for devices
 */
#define OWI_READ_ROM            0x33 /*!< \brief Read 64-bit serial ID (single device only)*/
#define OWI_OVERDRIVE_SKIP_ROM  0x3c /*!< \brief As OWI_SKIP_ROM but with command phase in fast mode*/
#define OWI_CONVERT_T           0x44 /*!< \brief DS18B20: Start conversion on selected device(s)*/
#define OWI_MATCH_ROM           0x55 /*!< \brief Qualify Command to single device*/
#define OWI_OVERDRIVE_MATCH_ROM 0x69 /*!< \brief As OWI_MATCH_ROM but with command phase in fast mode*/
#define OWI_SKIP_ROM            0xCC /*!< \brief Qualify Command as broadcast*/
#define OWI_READ                0xBE /*!< \brief Read Dat from addressed device*/
#define OWI_SEARCH_ROM          0xF0 /*!< \brief Prepare devices for ID Search*/

/*!
 * \brief Constanst used for OwiRomSearch
 */
#define OWI_LAST_DEVICE     0x00 /*!< \brief No more devices */
#define OWI_SEARCH_FIRST    0xFF /*!< \brief Start value for ID Search */

/*!
 * \brief OWI return codes
 */
enum OWI_ERRORS {
    OWI_SUCCESS         =   0, /*!< \brief All fine */
    OWI_PRESENCE_ERR    = - 1, /*!< \brief No device present */
    OWI_INVALID_HW      = - 2, /*!< \brief Selected Hardware can't do OWI transactions */
    OWI_OUT_OF_MEM      =  -3, /*!< \brief No more memory */
    OWI_HW_ERROR        = - 4, /*!< \brief Unexpected Hardware behaviour */
    OWI_DATA_ERROR      = - 5, /*!< \brief Unexpected Data */
    OWI_NOT_IMPLEMENTED = - 6, /*!< \brief No OWI Implementation available*/
};

/*!
 * \brief OWI speed modes
 */
enum OWIBUS_MODE{
    OWI_MODE_NORMAL = 0,    /*!< \brief Normal speed */
    OWI_MODE_OVERDRIVE = 1, /*!< \brief High speed (only some device types)*/
    OWI_MODE_NONE = 2,      /*!< \brief List end marker */
};

/*!
 * \brief OWI transactions
 */
enum STM32_OWITIMER_COMMANDS{
    OWI_CMD_RESET = 0,        /*!< \brief Presence pulse */
    OWI_CMD_RWBIT = 1,        /*!< \brief Bit transaction */
    OWI_CMD_NONE = 2,         /*!< \brief List end marker */
};

/*!
 * \brief OWI transactions phases
 */
enum STM32_OWITIMER_PHASES{
    OWI_PHASE_SETUP = 0,      /*!< \brief Pull Bus low */
    OWI_PHASE_SYNC_PULSE = 1, /*!< \brief Release Bus high for presence pulse, read and write '1' */
    OWI_PHASE_RW = 2,         /*!< \brief Pull Read bus state for presence and read*/
    OWI_PHASE_RELEASE = 3,    /*!< \brief Unconditionally release bus */
    OWI_PHASE_NONE = 4,       /*!< \brief List end marker */
};

/*!
 * \brief OWI bus modes
 */
#define OWI_OVERDRIVE 0x1 /*!< \brief Overdrive capable devices on the bus */
#define OWI_PULLUP    0x2 /*!< \brief Active pullup for loaded buses */
    
typedef struct _NUTOWIBUS NUTOWIBUS;

#include "owibus_bbif.h"
#include "owibus_uartif.h"

/* Include possible architecture specific OWI implementation here*/
#if defined(MCU_STM32)
#endif

/*!
 * \brief The OWIBUS structure. Storage provided by the caller
 */
struct _NUTOWIBUS {
    uint32_t    owibus_info;
    uint32_t    mode;
    int_fast8_t (*OwiTouchReset)(NUTOWIBUS*);
    int_fast8_t (*OwiReadBlock) (NUTOWIBUS* bus, uint8_t * data, uint_fast8_t);
    int_fast8_t (*OwiWriteBlock)(NUTOWIBUS* bus, uint8_t * data, uint_fast8_t);
};

int_fast8_t OwiRomSearch(NUTOWIBUS* bus, uint8_t *diff, uint64_t * hid);
int_fast8_t OwiCommand(NUTOWIBUS* bus, uint8_t cmd, uint64_t * hid);
int_fast8_t OwiReadBlock(NUTOWIBUS* bus, uint8_t *data, uint_fast8_t len);
int_fast8_t OwiWriteBlock(NUTOWIBUS* bus, uint8_t *data, uint_fast8_t len);
int_fast8_t OwiSetMode(NUTOWIBUS* bus, uint_fast8_t mode);
int_fast8_t OwiGetMode(NUTOWIBUS* bus);

#endif
