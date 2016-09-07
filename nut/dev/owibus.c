/*
 * Copyright (C) 2012 by Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de)
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
 * \file dev/owibus.c
 * \brief Implementation of the One-Wire API.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <stdint.h>
#include <sys/timer.h>
#include <dev/owibus.h>

/*!
 * \addtogroup xgOwibus
 */
/*@{*/

/* Values from http://www.maxim-ic.com/app-notes/index.mvp/id/126 */
const uint16_t owi_timervalues_250ns[OWI_MODE_NONE][OWI_CMD_NONE][OWI_PHASE_NONE] = {
    {
        {
            4 * 3,
            4 * (3 + 480),           /* H */
            4 * (3 + 480),           /* H*/
            4 * (3 + 480 + 70),      /* H + I */
            4 * (3 + 480 + 70 + 410) /* H + I + J */
        },
        {
            4 * 3,
            4 * (3 + 6),             /* A */
            4 * (3 + 60),            /* C */
            4 * (3 + 6 + 9),         /* A + E */
            4 * (3 + 6 + 9 + 55)     /* A + E + F*/
        }
    },
    {
        {
            10,
            10 + 280,
            10 + 280,
            10 + 280 + 34,
            10 + 280 + 34 + 160
        },
        {
            10,
            10 + 4,
            10 + 30,
            10 + 4 + 4,
            10 + 4 + 4 + 28
        }
    }
};

/*!
 * \brief Search the connected One-Wire bus for devices.
 *
 * \param bus   Specifies the One-Wire bus.
 * \param diff  On entry, pointer to either OWI_SEARCH_FIRST or the
 *              device found in the last call. On exit, pointer to
 *              either OWI_LAST_DEVICE or the diff to use in the next
 *              call.
 * \param value Pointer to the Hardware ID found.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
int OwiRomSearch(NUTOWIBUS *bus, uint8_t *diff, uint64_t *hid)
{
    uint_fast8_t i, j, next_diff;
    uint8_t b, c, command;
    uint8_t *id = (uint8_t *) hid;
    int res;

    res = bus->OwiTouchReset(bus);
    if (res) {
        return res;
    }
    command = OWI_SEARCH_ROM;
    res = bus->OwiWriteBlock(bus, &command, 8);
    if (res) {
        return res;
    }
    next_diff = OWI_LAST_DEVICE;    /* unchanged on last device */
    i = 8 * 8;                      /* 8 bytes */
    do {
        j = 8;                      /* 8 bits */
        do {
            res |= bus->OwiReadBlock(bus, &b, 1);
            res |= bus->OwiReadBlock(bus, &c, 1);
            if (c) {                /* read bit */
                if (b) {            /* read complement bit */
                    return OWI_DATA_ERROR;  /* error: no reaction on bus */
                }
            } else {
                if (!b) {           /* Two devices with different bits here */
                    if (*diff > i || ((*id & 1) && *diff != i)) {
                        b = 1;      /* Choose device with '1' for now */
                        next_diff = i;  /* Choose device with '0' on next pass */
                    }
                }
            }
            res |= bus->OwiWriteBlock(bus, &b, 1);  /* write bit */
            *id >>= 1;
            if (b) {                /* store bit as id */
                *id |= 0x80;
            }
            i--;
        } while (--j && !res);
        id++;                       /* next byte */
    } while (i && !res);
    *diff = next_diff;
    return res;
}

/*!
 * \brief Send a command to the connected devices.
 *
 * \param bus Specifies the One-Wire bus.
 * \param cmd Command to send.
 * \param hid Device to select or NULL for broadcast.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
int OwiCommand(NUTOWIBUS *bus, uint8_t cmd, uint64_t *hid)
{
    int res;
    uint8_t data[10];
    uint8_t *c;
    int len;

    res = bus->OwiTouchReset(bus);
    if (res) {
        return res;
    }
    c = data;
    if (hid) {
        *c++ = OWI_MATCH_ROM;
        memcpy(c, hid, 8);
        len = 80;
        c += 8;
    } else {
        *c++ = OWI_SKIP_ROM; /* to all devices */
        len = 16;
    }
    *c = cmd;
    res = bus->OwiWriteBlock(bus, data, len);
    return res;
}

/*!
 * \brief Read a block of data
 *
 * \param bus  Specifies the One-Wire bus.
 * \param data Data read.
 * \param len  Number of bits to read.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
int OwiReadBlock(NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len)
{
    return bus->OwiReadBlock(bus, data, len);
}

/*!
 * \brief Write a block of data
 *
 * \param bus  Specifies the One-Wire bus.
 * \param data Data to write.
 * \param len  Number of bits to write.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
int OwiWriteBlock(NUTOWIBUS *bus, uint8_t *data, uint_fast8_t len)
{
    return bus->OwiWriteBlock(bus, data, len);
}

/*!
 * \brief Set/Reset One-Wire Mode(s)
 *
 * \param bus  Specifies the One-Wire bus.
 * \param mode Bitmask of mode to set, at present only OWI_OVERDRIVE
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
int OwiSetMode(NUTOWIBUS *bus, uint_fast8_t mode)
{
    int res;

    if (mode & OWI_OVERDRIVE) {
        uint8_t command[1] = { OWI_OVERDRIVE_SKIP_ROM };

        res = bus->OwiTouchReset(bus);
        if (res) {
            return res;
        }
        bus->OwiWriteBlock(bus, command, 8);

        bus->mode |= OWI_OVERDRIVE;

        res = bus->OwiTouchReset(bus);
        if (res) {
            bus->mode &= ~OWI_OVERDRIVE;
            return res;
        }
    } else {
        res = bus->OwiTouchReset(bus);
    }
    return res;
}

/*!
 * \brief Set/Reset One-Wire Mode(s)
 *
 * \param bus Specifies the One-Wire bus.
 *
 * \return Mask of set modes
 */
int OWIGetMode(NUTOWIBUS *bus)
{
    return bus->mode;
}

/*!
 * \brief Initialize the Owi Bus
 *
 * \param bus Specifies the One-Wire bus.
 *
 * \return OWI_SUCCESS on success, a negative value otherwise.
 */
int OwiInit(NUTOWIBUS *bus)
{
    if (bus->OwiSetup) {
        int res = bus->OwiSetup(bus);
        NutSleep(1);
        return res;
    }
    else
        return OWI_SUCCESS;
}

/*@}*/
