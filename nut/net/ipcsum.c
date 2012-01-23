/*
 * Copyright (C) 2001-2005 by egnite Software GmbH
 * Copyright (c) 1993 by Digital Equipment Corporation
 * Copyright (c) 1983, 1993 by The Regents of the University of California
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
 * \file net/ipcsum.c
 * \brief IP checksum calculation.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <netinet/ipcsum.h>

/*!
 * \addtogroup xgIP
 *
 * \brief Internet Protocol checksum and related support.
 *
 */

/*@{*/


/*!
 * \brief Calculate a partial IP checksum of a buffer.
 *
 * The caller must create the one's complement of the final result.
 *
 * \param ics Initial checksum from previous parts.
 * \param buf Pointer to the buffer.
 * \param len Number of bytes in the buffer.
 *
 * \return Partial checksum in network byte order.
 */
uint16_t NutIpChkSumPartial(uint16_t ics, CONST void *buf, int len)
{
    register uint32_t sum = ics;
    register uint8_t *cp = (uint8_t *) buf;

    /* Sum up 16 bit values. */
    while (len > 1) {
#ifdef __BIG_ENDIAN__
        sum += ((uint16_t)*cp << 8) | *(cp + 1);
#else
        sum += ((uint16_t)*(cp + 1) << 8) | *cp;
#endif
        cp += 2;
        len -= 2;
    }

    /* Add remaining byte on odd lengths. */
    if (len) {
#ifdef __BIG_ENDIAN__
        sum += (uint16_t)*cp << 8;
#else
        sum += *cp;
#endif
    }

    /* Fold upper 16 bits to lower ones. */
    while (sum >> 16) {
        sum = (uint16_t)sum + (sum >> 16);
    }
    return (uint16_t) sum;
}

/*!
 * \brief Calculates an the final IP checksum over a block of data.
 *
 * Unlike the partial checksum in NutIpChkSumPartial(), this function takes
 * the one's complement of the final result, thus making it the full checksum.
 */
uint16_t NutIpChkSum(uint16_t ics, CONST void *buf, int len)
{
    return ~NutIpChkSumPartial(ics, buf, len);
}

/*
 * Details of the pseudo header used as part of the
 * calculation of UDP and TCP header checksums.
 */
struct __attribute__ ((packed)) pseudo_hdr {
    uint32_t ph_src_addr;
    uint32_t ph_dest_addr;
    uint8_t ph_zero;
    uint8_t ph_protocol;
    uint16_t ph_len;
};

/*!
 * \brief Calculates the partial IP pseudo checksum.
 *
 */
uint32_t NutIpPseudoChkSumPartial(uint32_t src_addr, uint32_t dest_addr, uint8_t protocol, int len)
{
    struct pseudo_hdr ph;

    ph.ph_src_addr = src_addr;
    ph.ph_dest_addr = dest_addr;
    ph.ph_zero = 0;
    ph.ph_protocol = protocol;
    ph.ph_len = len;

    return NutIpChkSumPartial(0, &ph, sizeof(struct pseudo_hdr));
}

/*@}*/
