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
 * Revision 1.1  2003/05/09 14:41:38  haraldkipp
 * Initial revision
 *
 * Revision 1.11  2003/03/31 12:29:03  harald
 * Mark routines deprecated
 *
 * Revision 1.10  2003/02/04 18:14:57  harald
 * Version 3 released
 *
 * Revision 1.9  2002/06/26 17:29:36  harald
 * First pre-release with 2.4 stack
 *
 */

/*!
 * \addtogroup xgSoStream
 */
/*@{*/

#include <sys/heap.h>
#include <netinet/sostream.h>

/*!
 * \brief Wait for input.
 *
 * This function checks the input buffer for any data. If
 * the buffer is empty, the calling \ref xrThread "thread" 
 * will be blocked until new data arrives. 
 *
 * \deprecated Use _read() or fread() in new programs.
 *
 * \param dev Identifies the stream device, which has been created 
 *            previously by calling NutSoStreamCreate().
 *
 * \return 0 on success, -1 otherwise.
 */
static int NutSoStreamInput(NUTDEVICE * dev)
{
    IFSTREAM *ifs;
    TCPSOCKET *sock;
    int got;

    ifs = dev->dev_icb;
    ifs->if_rd_idx = 0;
    sock = dev->dev_dcb;
    if ((got =
         NutTcpReceive(sock, ifs->if_rx_buf,
                       sizeof(ifs->if_rx_buf))) <= 0) {
        ifs->if_rx_idx = 0;
        return got;
    }
    ifs->if_rx_idx = (u_char) got;
    return 0;
}

/*!
 * \brief Initiate output.
 *
 * This function checks the output buffer. If it contains
 * any data, a data segment is transmitted to the remote 
 * host.
 *
 * \deprecated Will be removed soon.
 *
 * \param dev Identifies the stream device, which has been created 
 *            previously by calling NutSoStreamCreate().
 *
 * \return 0 on success, -1 otherwise.
 */
static int NutSoStreamOutput(NUTDEVICE * dev)
{
    IFSTREAM *ifs;
    TCPSOCKET *sock;
    u_char n;
    int c;

    ifs = dev->dev_icb;
    if (ifs->if_wr_idx) {
        sock = dev->dev_dcb;
        for (n = 0; n < ifs->if_wr_idx; n += c)
            if ((c =
                 NutTcpSend(sock, ifs->if_tx_buf + n,
                            ifs->if_wr_idx - n)) < 0)
                return -1;
        ifs->if_wr_idx = 0;
    }
    return 0;
}

/*!
 * \brief Wait for output buffer empty.
 *
 * If the output buffer contains any data, the calling
 * thread is suspended until all data has been transmitted.
 *
 * \deprecated Use fflush() in new programs.
 *
 * \param dev Identifies the stream device, which has been created 
 *            previously by calling NutSoStreamCreate().
 */
static int NutSoStreamFlush(NUTDEVICE * dev)
{
    return NutSoStreamOutput(dev);
}

/*!
 * \brief Create a virtual stream device from a specified socket.
 *
 * \param sock Specifies the socket descriptor, which must have been 
 *             retrieved by calling NutTcpCreateSocket(). In
 *             addition a connection must already have been established
 *             by calling NutTcpConnect() or NutTcpAccept().
 */
NUTDEVICE *NutSoStreamCreate(TCPSOCKET * sock)
{
    NUTDEVICE *dev;
    IFSTREAM *ifs;

    if ((dev = NutHeapAllocClear(sizeof(NUTDEVICE))) != 0) {
        dev->dev_type = IFTYP_STREAM;
        dev->dev_dcb = sock;
        if ((dev->dev_icb = NutHeapAllocClear(sizeof(IFSTREAM))) != 0) {
            ifs = dev->dev_icb;
            ifs->if_input = NutSoStreamInput;
            ifs->if_output = NutSoStreamOutput;
            ifs->if_flush = NutSoStreamFlush;
        } else {
            NutHeapFree(dev);
            dev = 0;
        }
    }

    return dev;
}

/*!
 * \brief Destroy a previously created stream device.
 *
 * This will flush the stream output buffer and release any
 * occupied memory. To terminate the connection and close
 * the socket, the application must call NutTcpCloseSocket().
 *
 * \deprecated Use _close() or fclose() in new programs.
 *
 * \param dev Identifies the stream device, which has been created 
 *            previously by calling NutSoStreamCreate().
 */
int NutSoStreamDestroy(NUTDEVICE * dev)
{
    NutSoStreamFlush(dev);
    NutHeapFree(dev->dev_icb);
    NutHeapFree(dev);

    return 0;
}


/*@}*/
