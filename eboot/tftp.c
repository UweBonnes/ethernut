/*
 * Copyright (C) 2002-2004 by egnite Software GmbH. All rights reserved.
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
 */

/*
 * $Log$
 * Revision 1.1  2004/04/15 09:34:45  haraldkipp
 * Checked in
 *
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "flash.h"
#include "eboot.h"
#include "tftp.h"

/*!
 * \addtogroup xgTftp
 */
/*@{*/

/*!
 * \brief Erase and program a page in the flash ROM.
 *
 * \param page The page number to program, 0..479.
 * \param data Pointer to the new page contents.
 * \param len  Number of bytes to program. If this is less than 256, 
 *             then the remaining bytes will be filled with 0xFF.
 */
static void FlashPage(u_short page, void *data, u_short len)
{
    u_short i;
    u_short *wp = data;

    if (len > 256)
        len = 256;

    if (page >= 256) {
        if (page >= 480)
            return;
        RAMPZ = 1;
    } else
        RAMPZ = 0;
    page <<= 8;

    SpmCommand(page, (1 << PGERS) | (1 << SPMEN));
    SpmCommand(0, (1 << RWWSRE) | (1 << SPMEN));

    for (i = 0; i < len; i += 2, wp++)
        SpmBufferFill(i, *wp);
    for (; i < 256; i += 2)
        SpmBufferFill(i, 0xFFFF);

    SpmCommand(page, (1 << PGWRT) | (1 << SPMEN));
    SpmCommand(0, (1 << RWWSRE) | (1 << SPMEN));
}

/*!
 * \brief Download a file from a TFTP server and burn it into the flash ROM.
 *
 * \return 0 on success, -1 otherwise.
 */
int TftpRecv(void)
{
    u_char retry;
    int rlen = 0;
    int slen;
    u_short tport = 69;
    u_short block = 0;
    u_char *cp;
    u_char *cp1;

    /*
     * Prepare the transmit buffer for a file request.
     */
    sframe.u.tftp.th_opcode = htons(TFTP_RRQ);
    slen = 2;
    cp = sframe.u.tftp.th_u.tu_stuff;
    cp1 = bootfile;
    do {
        *cp = *cp1++;
        slen++;
    } while(*cp++);
    *cp++ = 'o';
    *cp++ = 'c';
    *cp++ = 't';
    *cp++ = 'e';
    *cp++ = 't';
    *cp++ = 0;
    slen += 6;

    /*
     * Lopp until we receive a packet with less than 512 bytes of data.
     */
    do {

        /*
         * Send file request or acknowledge and receive
         * a data block.
         */
        for (retry = 0; retry < 3; retry++) {
            if (UdpOutput(server_ip, tport, SPORT, slen) >= 0) {
                if ((rlen = UdpInput(SPORT, 5000)) >= 4)
                    break;
            }
        }

        /*
         * Can't reach the TFTP server or got a malformed
         * repsonse.
         */
        if (rlen < 4)
            return -1;


        /*
         * Accept data blocks only. Anything else will stop
         * the transfer with an error.
         */
        if (ntohs(rframe.u.tftp.th_opcode) != TFTP_DATA)
            return -1;

        /*
         * If this was the first block we received, prepare
         * the send buffer for sending ACKs.
         */
        if (block == 0) {
            tport = ntohs(rframe.udp_hdr.uh_sport);
            sframe.u.tftp.th_opcode = htons(TFTP_ACK);
            slen = 4;
        }

        /*
         * If this block is out of sequence, we ignore it.
         * However, if we missed the first block, return
         * with an error.
         */
        if (ntohs(rframe.u.tftp.th_u.tu_block) != block + 1) {
            if (block == 0)
                return -1;
            continue;
        }

        /*
         * Burn the received data into the flash ROM.
         */
        if (rlen > 4) {
            FlashPage(block << 1, rframe.u.tftp.th_data, rlen - 4);
            if (rlen > 260)
                FlashPage((block << 1) + 1, &rframe.u.tftp.th_data[256], rlen - 260);
        }

        /*
         * Update our block counter.
         */
        block++;
        sframe.u.tftp.th_u.tu_block = htons(block);

    } while (rlen >= 516);

    /*
     * Send the last ACK.
     */
    UdpOutput(server_ip, tport, SPORT, slen);

    return 0;
}

/*@}*/
