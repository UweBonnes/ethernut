/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.1  2003/05/09 14:41:50  haraldkipp
 * Initial revision
 *
 * Revision 1.2  2003/02/04 18:15:57  harald
 * Version 3 released
 *
 * Revision 1.1  2002/10/29 15:39:22  harald
 * Special function removed from standard lib
 *
 */

#include <sys/ifstream.h>
#include <sys/ifstran.h>

static char hexdigit[] = "0123456789ABCDEF";

/*!
 * \addtogroup xgIfStran
 */
/*@{*/

/*!
 * \brief Read from a stream device translating non printables.
 *
 * This call works like NutIfStreamRead(), but translates
 * character codes below 33, above 126 or equal 34 into
 * their hex representation prepended by a backslash and the
 * letter x. Backslashes are duplicated and the complete string
 * is terminated by character code zero. This offers the ability 
 * to store binary data in printable strings.
 *
 * \deprecated Will be removed.
 *
 * \param dev  Identifies the device to read from. This pointer
 *             must have been retrieved by calling NutDeviceOpen().
 * \param data Pointer to the buffer that receives the translated string.
 * \param size Size of the buffer.
 *
 * \return The number of bytes read or -1 in case of an error. Due
 *         to code translation, this may be less than the number of
 *         characters returned in the string.
 */
int NutIfStreamReadTran(NUTDEVICE * dev, char *data, int size)
{
    int rc = 0;
    int sc;
    u_char ch;
    IFSTREAM *ifs;

    /*
     * Check if input is supported.
     */
    ifs = (IFSTREAM *) dev->dev_icb;
    if (ifs->if_input == 0)
        return -1;

    /*
     * Call without data pointer discards receive buffer.
     */
    if (data == 0 || size == 0) {
        ifs->if_rd_idx = ifs->if_rx_idx;
        return 0;
    }

    /*
     * If buffer empty, wait for input.
     * Return 0 on timeout.
     */
    if (ifs->if_rd_idx == ifs->if_rx_idx)
        if ((*ifs->if_input) (dev))
            return 0;

    /*
     * Get characters from receive buffer.
     */
    size--;
    for (sc = 0; sc < size;) {
        if (ifs->if_rd_idx == ifs->if_rx_idx)
            break;
        ch = ifs->if_rx_buf[ifs->if_rd_idx];
        if (ch < 33 || ch > 126 || ch == 34) {
            if (sc + 4 >= size)
                break;
            *data++ = '\\';
            *data++ = 'x';
            *data++ = hexdigit[ch >> 4];
            *data++ = hexdigit[ch & 0x0f];
            sc += 4;
        } else {
            if (ch == '\\') {
                if (sc + 2 >= size)
                    break;
                *data++ = '\\';
                sc++;
            }
            *data++ = ch;
            sc++;
        }
        ifs->if_rd_idx++;
        rc++;
    }
    *data = 0;

    return rc;
}


/*!
 * \brief Translate and write a string to a stream device.
 *
 * This call works like NutIfStreamWrite(), but translates
 * any backslash followed by the letter x followed by two
 * hexadecimal uppercase digits into the binary representation. 
 * Simple backslashes are expected to be duplicated and the complete 
 * string must be terminated by character code zero. This offers the 
 * ability to write binary data from a printable strings.
 *
 * The data isn't immediately transfered to the physical device, 
 * but buffered in a transmit buffer. Transmission starts either 
 * when the buffer is full or when this function is called with
 * a zero data pointer.
 *
 * \deprecated Will be removed.
 *
 * \param dev  Identifies the device to write to. This pointer
 *             must have been retrieved by calling NutDeviceOpen().
 * \param data Pointer to a zero terminated string to be translated
 *             and written. If this pointer is NULL, the stream device 
 *             starts transmitting the buffered data.
 *
 * \return The number of characters written or -1 in  case of an error. 
 *         Due to code translation, this may be less than the number
 *         of characters contained in the string.
 */
int NutIfStreamWriteTran(NUTDEVICE * dev, CONST char *data)
{
    int rc = 0;
    IFSTREAM *ifs;
    u_char ix;
    u_char ch;

    /*
     * Check if output is supported.
     */
    ifs = dev->dev_icb;
    if (ifs->if_output == 0)
        return -1;

    /*
     * Call without data pointer starts transmission.
     */
    if (data == 0) {
        if ((*ifs->if_output) (dev) < 0)
            return -1;
        return 0;
    }

    /*
     * Start transmission if output buffer full.
     */
    if (ifs->if_wr_idx + 1 == ifs->if_tx_idx)
        if ((*ifs->if_output) (dev) < 0)
            return -1;

    /*
     * If the buffer is still full, we have write timeout.
     */
    if ((ix = ifs->if_wr_idx + 1) == ifs->if_tx_idx)
        return 0;

    /*
     * Put characters in transmit buffer.
     */
    while (*data) {
        if (ix == ifs->if_tx_idx)
            break;
        if (*data == '\\') {
            if (*++data == 'x') {
                data++;
                ch = ((*data > '9') ? *data - 55 : *data - 48) << 4;
                data++;
                ch += (*data > '9') ? *data - 55 : *data - 48;
                data++;
            } else {
                ch = *data++;
            }
        } else {
            ch = *data++;
        }
        ifs->if_tx_buf[ifs->if_wr_idx] = ch;
        ifs->if_wr_idx = ix++;
        rc++;
    }
    return rc;
}


/*@}*/
