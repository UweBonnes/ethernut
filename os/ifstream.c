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
 * Revision 1.18  2003/02/04 18:15:57  harald
 * Version 3 released
 *
 * Revision 1.17  2002/09/03 17:39:47  harald
 * Comment about broken connection
 *
 * Revision 1.16  2002/06/26 17:29:44  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/ifstream.h>

/*!
 * \addtogroup xgIfStream
 */
/*@{*/

/*!
 * \brief Read from a stream device.
 *
 * \deprecated Use _read() or fread() in new programs.
 *
 * \param dev  Identifies the device to read from. This pointer
 *             must have been retrieved by calling NutDeviceOpen().
 * \param data Pointer to the buffer that receives the data. Set to
 *             null to flush the input buffer.
 * \param size Size of the buffer. Ignored if the data pointer is
 *             null.
 *
 * \return The number of bytes read, 0 on timeouts and broken
 *         connections or -1 in case of an error.
 */
int NutIfStreamRead(NUTDEVICE * dev, char *data, int size)
{
    int rc;
    IFSTREAM *ifs;

    /*
     * Check for valid device pointer.
     */
    if ((void *) dev < RAMSTART)
        return -1;

    /*
     * Check if input is supported.
     */
    ifs = (IFSTREAM *) dev->dev_icb;
    if (ifs->if_input == 0)
        return -1;

    /*
     * Call without data pointer discards receive buffer.
     */
    if (data == 0) {
        ifs->if_rd_idx = ifs->if_rx_idx;
        return 0;
    }

    /*
     * Check for valid buffer pointer.
     */
    if ((void *) data < RAMSTART)
        return -1;

    /*
     * If buffer empty, wait for input. 
     * Return 0 in case of timeout.
     */
    if (ifs->if_rd_idx == ifs->if_rx_idx)
        if ((*ifs->if_input) (dev))
            return 0;

    /*
     * Get characters from receive buffer.
     */
    for (rc = 0; rc < size; rc++) {
        if (ifs->if_rd_idx == ifs->if_rx_idx)
            break;
        *data++ = ifs->if_rx_buf[ifs->if_rd_idx++];
    }
    return rc;
}

/*!
 * \brief Get a line from a stream device.
 *
 * Reads a string from the specified device. Characters are
 * read up to and including the first end of line character
 * or until the number of characters read is equal to the
 * specified maximum or until a timeout occurs, whichever 
 * comes first.
 *
 * \deprecated Use fgets() in new programs.
 *
 * \param dev  Identifies the device to read from. This pointer
 *             must have been retrieved by calling NutDeviceOpen().
 * \param data Points to the buffer that receives the zero 
 *             terminated string. End of line characters are not 
 *             stored.
 * \param size Specifies the size of the buffer. The maximum
 *             number of characters read is one less because
 *             of the terminating null character.
 *
 * \return The number of bytes read or -1 in case of an error.
 *         Receiving no character is being considered an error.
 */
int NutIfStreamGetLine(NUTDEVICE * dev, char *data, int size)
{
    int rc;
    IFSTREAM *ifs;
    u_char ch;

    /*
     * Check for valid device pointer.
     */
    if ((void *) dev < RAMSTART)
        return -1;

    /*
     * Check if input is supported.
     */
    ifs = (IFSTREAM *) dev->dev_icb;
    if (ifs->if_input == 0)
        return -1;

    /*
     * Check for valid buffer pointer.
     */
    if ((void *) data < RAMSTART)
        return -1;

    for (rc = 0; rc < size - 1;) {
        /*
         * If buffer is empty, call input handler.
         */
        if (ifs->if_rd_idx == ifs->if_rx_idx)
            if ((*ifs->if_input) (dev)) {
                if (rc == 0)
                    rc = -1;
                break;
            }

        ch = ifs->if_rx_buf[ifs->if_rd_idx++];
        if (ch == '\r' || ch == '\n') {
            if (ifs->if_last_eol == 0 || ifs->if_last_eol == ch) {
                ifs->if_last_eol = ch;
                break;
            }
        } else {
            ifs->if_last_eol = 0;
            *data++ = ch;
            rc++;
        }
    }
    *data = 0;

    return rc;
}


/*!
 * \brief Write to a stream device.
 *
 * The data isn't immediately transfered to the physical device, 
 * but buffered in a transmit buffer. Transmission starts either 
 * when the buffer is full or when this function is called with
 * a zero data pointer.
 *
 * \deprecated Use _write() or fwrite() in new programs.
 *
 * \param dev  Identifies the device to write to. This pointer
 *             must have been retrieved by calling NutDeviceOpen().
 * \param data Pointer to data to be written. If this pointer is
 *             NULL, the stream device starts transmitting the
 *             buffered data.
 * \param len  Number of bytes to write.
 *
 * \return The number of bytes written or -1 in case of an error.
 */
int NutIfStreamWrite(NUTDEVICE * dev, CONST char *data, int len)
{
    int rc;
    IFSTREAM *ifs;
    u_char ix;

    /*
     * Check for valid device pointer.
     */
    if ((void *) dev < RAMSTART)
        return -1;

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
     * Check for valid buffer pointer.
     */
    if ((void *) data < RAMSTART)
        return -1;

    /*
     * Start transmission if output buffer full.
     */
    if ((ix = ifs->if_wr_idx + 1) == ifs->if_tx_idx)
        if ((*ifs->if_output) (dev) < 0)
            return -1;

    /*
     * Put characters in transmit buffer.
     */
    for (rc = 0; rc < len; rc++) {
        if (ix == ifs->if_tx_idx)
            break;
        ifs->if_tx_buf[ifs->if_wr_idx] = *data++;
        ifs->if_wr_idx = ix++;
    }
    return rc;
}

/*!
 * \brief Write program space data to a stream device.
 *
 * \deprecated Use _write_P() or fwrite_P() in new programs.
 *
 * \param dev  Identifies the device to write to. This pointer
 *             must have been retrieved by calling NutDeviceOpen().
 * \param data Pointer to data in program space to be written.
 * \param len  Number of bytes to write.
 *
 * \return The number of bytes written or -1 in case of an error.
 */
int NutIfStreamWrite_P(NUTDEVICE * dev, PGM_P data, int len)
{
    int rc;
    IFSTREAM *ifs;
    u_char ix;

    /*
     * Check for valid device pointer.
     */
    if ((void *) dev < RAMSTART)
        return -1;

    /*
     * Check if output is supported.
     */
    ifs = (IFSTREAM *) dev->dev_icb;
    if (ifs->if_output == 0)
        return -1;

    /*
     * If buffer full, wait for output.
     */
    if ((ix = ifs->if_wr_idx + 1) == ifs->if_tx_idx)
        if ((*ifs->if_output) (dev) < 0)
            return -1;

    /*
     * Put characters in transmit buffer.
     */
    for (rc = 0; rc < len; rc++) {
        if (ix == ifs->if_tx_idx)
            break;
        ifs->if_tx_buf[ifs->if_wr_idx] = PRG_RDB(data);
        data++;
        ifs->if_wr_idx = ix++;
    }
    return rc;
}

/*!
 * \brief Flush stream device buffer.
 *
 * \deprecated Use fflush() in new programs.
 *
 * Flushes the output buffer for the specified device by
 * writing its current contents to the corresponding device.
 *
 * \param dev  Identifies the device to flush. This pointer
 *             must have been retrieved by directly or indirectly
 *             calling NutDeviceOpen().
 *
 * \return 0 on success, -1 on failures.
 */
int NutIfStreamFlush(NUTDEVICE * dev)
{
    IFSTREAM *ifs;

    /*
     * Check for valid device pointer.
     */
    if ((void *) dev >= RAMSTART) {
        if ((void *) (ifs = (IFSTREAM *) dev->dev_icb) >= RAMSTART
            && ifs->if_flush)
            return (*ifs->if_flush) (dev);
    }
    return -1;
}

/*@}*/
