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

/* unix_devs.c - a nut/os device driver for native unix devices
 *
 * 2004.04.01 Matthias Ringwald <matthias.ringwald@inf.ethz.ch>
 *
 */

/* avoid stdio nut wrapper */
#define NO_STDIO_NUT_WRAPPER

#include <fcntl.h>
#include <arch/unix.h>
#include <sys/device.h>
#include <sys/file.h>
#include <sys/timer.h>
#include <dev/uart.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

/*
 * functions available on avr somehow -- not implemented properly here :(
 */
char *dtostre(double f, char *str, u_char prec, u_char flags);
char *dtostre(double f, char *str, u_char prec, u_char flags)
{
    sprintf(str, "%e", f);
    return str;
}
char *dtostrf(double f, char width, char prec, char *str);
char *dtostrf(double f, char width, char prec, char *str)
{
    sprintf(str, "%f", f);
    return str;
}


/*!
 * \brief Open UnixDev
 *
 * \return Pointer to a static NUTFILE structure.
 */
static NUTFILE *UnixDevOpen(NUTDEVICE * dev, const char *name, int mode, int acc)
{
    NUTFILE *nf;

    FILE *nativeFile;
    char *nativeName;
    const char *modeString;
    struct termios t;

    // map from dev->name to unix name
    if (strncmp("uart", dev->dev_name, 4) == 0) {
        // uart
        if (dev->dev_name[4] == '0') {
            nativeName = emulation_options.uart_options[0].device;
        } else {
            nativeName = emulation_options.uart_options[1].device;
        }
    } else
        return NULL;

    // construct mode from flags
    if (mode & (_O_APPEND || _O_RDWR)) {
        if (mode & _O_BINARY)
            modeString = "a+b";
        else
            modeString = "a+";
    } else if (mode & _O_APPEND) {
        if (mode & _O_BINARY)
            modeString = "ab";
        else
            modeString = "a";
    } else if (mode & _O_RDWR) {
        if (mode & _O_BINARY)
            modeString = "r+b";
        else
            modeString = "r+";
    } else if (mode & _O_RDONLY) {
        if (mode & _O_BINARY)
            modeString = "rb";
        else
            modeString = "r";
    } else if (mode & _O_WRONLY) {
        if (mode & _O_BINARY)
            modeString = "wb";
        else
            modeString = "w";
    } else
        // default mode
        modeString = "a";

    // fopen unix device
    if (strcmp("stdio", nativeName) == 0) {
        nativeFile = stdout;
    } else {
        // printf("UnixDevOpen: uart%c -> %s, mode: %s\n", dev->dev_name[4], nativeName, modeString);
        nativeFile = fopen(nativeName, modeString);

        if (tcgetattr(fileno(nativeFile), &t) == 0) {

            cfmakeraw(&t);
            // regular device file
            t.c_cflag |= CS8 | CLOCAL;
            t.c_oflag = 0;

            // _fd_set_flowctrl(&t); =
            t.c_cflag &= ~CRTSCTS;

            // apply file descriptor options
            tcsetattr(fileno(nativeFile), TCSANOW, &t);

            setvbuf(nativeFile, NULL, _IONBF, 0);
        }
    }

    if (nativeFile == NULL)
        return NULL;

    // create new NUTFILE using malloc
    nf = malloc(sizeof(NUTFILE));

    // enter data
    nf->nf_next = 0;
    nf->nf_dev = dev;
    nf->nf_fcb = 0;

    // store unix fd in dev
    dev->dev_dcb = nativeFile;

    // set stdio unbuffered, if used
    if (nativeFile == stdout)
        setvbuf(stdout, NULL, _IONBF, 0);

    return nf;
}


/*!
 * \brief ...
 *
 * \return Number of characters sent.
 */
static int UnixDevWrite(NUTFILE * fp, CONST void *buffer, int len)
{
    int rc;
    // printf("UnixDevWrite: nutfile %0Xl, native handle %0Xl, text %s, len %d\n", (int) fp, (int) fp->nf_dev->dev_dcb,(char*) buffer, len);
    // return fwrite(buffer, (size_t) 1, (size_t) len, (FILE *) fp->nf_dev->dev_dcb);
    rc = write(fileno((FILE *) fp->nf_dev->dev_dcb), buffer, len);
    return rc;
}

/*!
 * \brief ...
 *
 * \return Number of characters read.
 */
static int UnixDevRead(NUTFILE * fp, void *buffer, int len)
{
    int rc;
    // rc = fread(buffer, (size_t) 1, (size_t) len, (FILE *) fp->nf_dev->dev_dcb);
    rc = read(fileno((FILE *) fp->nf_dev->dev_dcb), buffer, len);
    return rc;
}

/*! 
 * \brief Close stdout.
 *
 * \return Always 0.
 */
static int UnixDevClose(NUTFILE * fp)
{
    return 0;
}

/*!
 * \brief Perform USART control functions.
 *
 * This function is called by the ioctl() function of the C runtime
 * library.
 *
 * \param dev  Identifies the device that receives the device-control
 *             function.
 * \param req  Requested control function. May be set to one of the
 *             following constants:
 *             - \ref UART_SETSPEED
 *             - \ref UART_GETSPEED
 *             - \ref UART_SETDATABITS
 *             - \ref UART_GETDATABITS
 *             - \ref UART_SETPARITY
 *             - \ref UART_GETPARITY
 *             - \ref UART_SETSTOPBITS
 *             - \ref UART_GETSTOPBITS
 *             - \ref UART_SETSTATUS
 *             - \ref UART_GETSTATUS
 *             - \ref UART_SETREADTIMEOUT
 *             - \ref UART_GETREADTIMEOUT
 *             - \ref UART_SETWRITETIMEOUT
 *             - \ref UART_GETWRITETIMEOUT
 *             - \ref UART_SETLOCALECHO
 *             - \ref UART_GETLOCALECHO
 *             - \ref UART_SETFLOWCONTROL
 *             - \ref UART_GETFLOWCONTROL
 *             - \ref UART_SETCOOKEDMODE
 *             - \ref UART_GETCOOKEDMODE
 *             - \ref UART_SETCLOCKMODE
 *             - \ref UART_GETCLOCKMODE
 *             - \ref UART_SETTXBUFSIZ
 *             - \ref UART_GETTXBUFSIZ
 *             - \ref UART_SETRXBUFSIZ
 *             - \ref UART_GETRXBUFSIZ
 *             - \ref UART_SETTXBUFLWMARK
 *             - \ref UART_GETTXBUFLWMARK
 *             - \ref UART_SETTXBUFHWMARK
 *             - \ref UART_GETTXBUFHWMARK
 *             - \ref UART_SETRXBUFLWMARK
 *             - \ref UART_GETRXBUFLWMARK
 *             - \ref UART_SETRXBUFHWMARK
 *             - \ref UART_GETRXBUFHWMARK
 *
 * \param conf Points to a buffer that contains any data required for
 *             the given control function or receives data from that
 *             function.
 * \return 0 on success, -1 otherwise.
 *
 * \warning Timeout values are given in milliseconds and are limited to 
 *          the granularity of the system timer. To disable timeout,
 *          set the parameter to NUT_WAIT_INFINITE.
 */
int UnixDevIOCTL(NUTDEVICE * dev, int req, void *conf)
{
    struct termios t;
    u_long *lvp = (u_long *) conf;
    u_long lv = *lvp;

    switch (req) {
    case UART_GETFLOWCONTROL:
        break;

    case UART_SETFLOWCONTROL:
        break;

    case UART_GETSPEED:
        if (tcgetattr(fileno(dev->dev_dcb), &t) == 0) {
            *lvp = cfgetospeed(&t);
        } else
            return -1;
        break;

    case UART_SETSPEED:
        if (tcgetattr(fileno(dev->dev_dcb), &t) == 0) {
            cfsetospeed(&t, lv);
            cfsetispeed(&t, lv);
            // apply file descriptor options
            tcsetattr(fileno(dev->dev_dcb), TCSANOW, &t);
        } else
            return -1;
        break;
    }
    return -1;
}

/* ======================= Devices ======================== */


/*!
 * \brief Debug device 0 information structure.
 */
NUTDEVICE devDebug0 = {
    0,                          /*!< Pointer to next device. */
    {'u', 'a', 'r', 't', '0', 0, 0, 0, 0}
    ,                           /*!< Unique device name. */
    0,                          /*!< Type of device. */
    0,                          /*!< Base address. */
    0,                          /*!< First interrupt number. */
    0,                          /*!< Interface control block. */
    0,                          /*!< Driver control block. */
    0,                          /*!< Driver initialization routine. */
    UnixDevIOCTL,               /*!< Driver specific control function. */
    UnixDevRead,
    UnixDevWrite,
    UnixDevOpen,
    UnixDevClose,
    0
};

/*!
 * \brief Debug device 1 information structure.
 */
NUTDEVICE devDebug1 = {
    0,                          /*!< Pointer to next device. */
    {'u', 'a', 'r', 't', '1', 0, 0, 0, 0}
    ,                           /*!< Unique device name. */
    0,                          /*!< Type of device. */
    0,                          /*!< Base address. */
    0,                          /*!< First interrupt number. */
    0,                          /*!< Interface control block. */
    0,                          /*!< Driver control block. */
    0,                          /*!< Driver initialization routine. */
    UnixDevIOCTL,               /*!< Driver specific control function. */
    UnixDevRead,
    UnixDevWrite,
    UnixDevOpen,
    UnixDevClose,
    0
};


/*!
 * \brief Debug device 0 information structure.
 */
NUTDEVICE devUart0 = {
    0,                          /*!< Pointer to next device. */
    {'u', 'a', 'r', 't', '0', 0, 0, 0, 0}
    ,                           /*!< Unique device name. */
    0,                          /*!< Type of device. */
    0,                          /*!< Base address. */
    0,                          /*!< First interrupt number. */
    0,                          /*!< Interface control block. */
    0,                          /*!< Driver control block. */
    0,                          /*!< Driver initialization routine. */
    UnixDevIOCTL,               /*!< Driver specific control function. */
    UnixDevRead,
    UnixDevWrite,
    UnixDevOpen,
    UnixDevClose,
    0
};

/*!
 * \brief Debug device 1 information structure.
 */
NUTDEVICE devUart1 = {
    0,                          /*!< Pointer to next device. */
    {'u', 'a', 'r', 't', '1', 0, 0, 0, 0}
    ,                           /*!< Unique device name. */
    0,                          /*!< Type of device. */
    0,                          /*!< Base address. */
    0,                          /*!< First interrupt number. */
    0,                          /*!< Interface control block. */
    0,                          /*!< Driver control block. */
    0,                          /*!< Driver initialization routine. */
    UnixDevIOCTL,               /*!< Driver specific control function. */
    UnixDevRead,
    UnixDevWrite,
    UnixDevOpen,
    UnixDevClose,
    0
};


/*@}*/
