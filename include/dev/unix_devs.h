#ifndef _DEV_UNIX_DEVS_H_
#define _DEV_UNIX_DEVS_H_


/*
 * Copyright (C) 2000-2004 by ETH Zurich
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
 * THIS SOFTWARE IS PROVIDED BY ETH ZURICH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH ZURICH
 *  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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

/* unix_devs.h - a nut/os device driver for native unix devices
 *
 * 2004.04.01 Matthias Ringwald <matthias.ringwald@inf.ethz.ch>
 *
 */

/*
 * $Log$
 * Revision 1.3  2004/06/21 10:57:25  freckle
 * dev/unix_devs.c: read operation is using extra pthread to only block the
 * current thread instead of all threads
 *
 * Revision 1.2  2004/04/16 17:50:35  freckle
 * Implemented the most common _IOCTL calls
 * Added block read functionality to read call
 *
 * Revision 1.1  2004/04/07 12:13:57  haraldkipp
 * Matthias Ringwald's *nix emulation added
 *
 */

#include <pthread.h>
#include <sys/device.h>
#include <dev/netbuf.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \addtogroup xgUnixEmulation
 */
/*@{*/

/*
 * Available devices.
 */
    extern NUTDEVICE devUart0;
    extern NUTDEVICE devUart1;
    extern NUTDEVICE devUsartAvr0;
    extern NUTDEVICE devUsartAvr1;
    extern NUTDEVICE devDebug0;
    extern NUTDEVICE devDebug1;

/*!
 * \struct _UNIXDCB unix_d  evs.h dev/unix_devs.h
 * \brief UNIX devices low level information structure.
 *
 */
struct _UNIXDCB {

    /*! \brief Mode flags.
     */
    u_long dcb_modeflags;

    /*! \brief Status flags.
     */
    u_long dcb_statusflags;

    /*! \brief Read timeout.
     */
    u_long dcb_rtimeout;

    /*! \brief Write timeout.
     */
    u_long dcb_wtimeout;

    /*! \brief Last EOL character.
     */
    u_char dcb_last_eol;

    /*! \brief Native file descriptor
    */
    int dcb_fd;
    
    /*! \brief Queue of threads waiting for a character in the input buffer.
     */
    HANDLE dcb_rx_rdy;
    
    /*! \brief Conditional Variable to trigger read thread 
    */
    pthread_cond_t dcb_rx_trigger;

};

/*!
 * USART device low level information type.
 */
    typedef struct _UNIXDCB UNIXDCB;

/*@}*/



#ifdef __cplusplus
}
#endif
#endif
