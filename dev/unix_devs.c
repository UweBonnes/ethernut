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
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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
#include <fcntl_orig.h>
#include <arch/unix.h>
#include <sys/device.h>
#include <sys/file.h>
#include <sys/timer.h>
#include <sys/thread.h>
#include <dev/usart.h>
#include <errno.h> 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include <dev/unix_devs.h>

/* private prototypes */
int UnixDevIOCTL(NUTDEVICE * dev, int req, void *conf);

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
	long	baud = USART_INITSPEED;

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
		// store unix fd in dev
		dev->dev_dcb = nativeFile;

    } else {

        nativeFile = fopen(nativeName, modeString);

        if (tcgetattr(fileno(nativeFile), &t) == 0) {

            cfmakeraw(&t);

            // regular device file
            t.c_cflag |= CS8 | CLOCAL;
            t.c_oflag = 0;

            // no flow control
            t.c_iflag &= ~(IXON | IXOFF | IXANY);
            t.c_cflag &= ~CRTSCTS;

            cfsetospeed(&t, baud);
            cfsetispeed(&t, baud);
			
            // apply file descriptor options
            if ( tcsetattr(fileno(nativeFile), TCSANOW, &t) < 0) 
			{
				printf("UnixDevOpen: tcsetattr failed\n\r" );
			}

            setvbuf(nativeFile, NULL, _IONBF, 0);
        }

		// store unix fd in dev
		dev->dev_dcb = nativeFile;

		// set initial speed to UART_SETSPEED
		UnixDevIOCTL(dev, UART_SETSPEED, &baud);
    }

    if (nativeFile == NULL)
        return NULL;

	// set non-blocking
	if ( fcntl(fileno(nativeFile), F_SETFL, O_NONBLOCK) < 0 )
	{
		printf("UnixDevOpen: fcntl O_NONBLOCK failed");
	}
		
	// printf("UnixDevOpen: %s, FILE * %lx, int %d\n\r", nativeName, (long) nativeFile, fileno(nativeFile));
	
    // create new NUTFILE using malloc
    nf = malloc(sizeof(NUTFILE));

    // enter data
    nf->nf_next = 0;
    nf->nf_dev = dev;
    nf->nf_fcb = 0;

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
    int rc = 0;
    int newBytes;
	// printf("UnixDevRead: called: len = %d\n\r",len);

    do {
	
        newBytes = read(fileno((FILE *) fp->nf_dev->dev_dcb), buffer, len);

        /* error or timeout ? */
        if (newBytes < 0) {
		
			if (errno == EAGAIN ) {
				// timeout. No data available right now
				// printf("UnixDevRead: no bytes available, trying again\n\r");
				errno = 0;
				NutSleep( 100 );
				continue;
			}
			else {

				printf("UnixDevRead: error %d occured, giving up\n\r", errno);
				return newBytes;
			}
		}
		else
			rc += newBytes;

#ifdef UART_SETBLOCKREAD
		// printf("UnixDevRead: UART_SETBLOCKREAD defined\n\r");
		// check for blocking read: all bytes received
		if ( (((UNIXDCB *) fp->nf_dev->dev_dcb)->dcb_modeflags & USART_MF_BLOCKREAD)  && (rc < len) )
		{
			// printf("UnixDevRead: block read enabled, but not enough bytes rad \n\r");
			continue;
		}
#endif
		// did we got one?
		if (rc > 0)
			break;

	} while (1);	

    return rc;
}

/*! 
 * \brief Close ...
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
 *             - \ref UART_SETSTOPBITS
 *             - \ref UART_GETSTOPBITS
 *             - \ref UART_SETPARITY
 *             - \ref UART_GETPARITY
 *             - \ref UART_SETFLOWCONTROL
 *             - \ref UART_GETFLOWCONTROL
 *			   - \ref UART_SETBLOCKREAD
 *			   - \ref UART_GETBLOCKREAD
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

	// printf("UnixDevIOCTL, native %lx, req: %d, lv: %ld\n\r", (long) dev->dev_dcb, req, lv);

    switch (req) {

    case UART_SETSPEED:
    case UART_SETFLOWCONTROL:
    case UART_SETPARITY:
    case UART_SETDATABITS:
    case UART_SETSTOPBITS:

	    if (fileno(dev->dev_dcb) <= STDERR_FILENO)
			return -1;
			
        if (tcgetattr(fileno(dev->dev_dcb), &t))
		{
			printf("UnixDevIOCTL, tcgetattr failed\n\r");
            return -1;
		}

        switch (req) {

        case UART_SETSPEED:
			cfsetospeed(&t, lv);
			cfsetispeed(&t, lv);

            break;

        case UART_SETFLOWCONTROL:
            switch (lv) {
            case 0:
                t.c_cflag &= ~CRTSCTS;
                t.c_iflag &= ~(IXON | IXOFF | IXANY);
                break;
            case UART_HS_SOFT:
                t.c_cflag &= ~CRTSCTS;
                t.c_iflag |= (IXON | IXOFF | IXANY);
                return -1;
            case UART_HS_MODEM:
                return -1;
            case UART_HS_RTSCTS:
                t.c_cflag |= CRTSCTS;
                t.c_iflag &= ~(IXON | IXOFF | IXANY);
                break;
            default:
                return -1;
            }
            break;

        case UART_SETPARITY:
            // 0 (none), 1 (odd) or 2 (even).
            t.c_cflag &= ~(PARODD | PARENB);
            switch (lv) {
            case 0:
                break;
            case 1:
                t.c_cflag |= PARENB | PARODD;
                break;
            case 2:
                t.c_cflag |= PARENB;
            default:
                return -1;
            }
            break;


        case UART_SETDATABITS:
            t.c_cflag &= ~CSIZE;
            switch (lv) {
            case 5:
                t.c_cflag |= CS5;
                break;
            case 6:
                t.c_cflag |= CS6;
                break;
            case 7:
                t.c_cflag |= CS7;
                break;
            case 8:
                t.c_cflag |= CS8;
                break;
            default:
                return -1;
            }
            break;

        case UART_SETSTOPBITS:
            switch (lv) {
            case 1:
                t.c_cflag &= ~CSTOPB;
                break;
            case 2:
                t.c_cflag |= CSTOPB;
                break;
            default:
                return -1;
            }
            break;

        }


		if ( tcdrain(fileno(dev->dev_dcb)) < 0 ) {
			printf("UnixDevIOCTL: tcdrain failed: errno: %d\n\r", errno);
			errno = 0;
			return -1;
		}
		
		if ( tcsetattr(fileno(dev->dev_dcb), TCSANOW, &t ) < 0) { 
			printf("UnixDevIOCTL: tcsetattr failed: errno: %d\n\r", errno);
			errno = 0;
			return -1;
		}
		return 0;
		
    case UART_GETSPEED:
    case UART_GETFLOWCONTROL:
    case UART_GETPARITY:
    case UART_GETDATABITS:
    case UART_GETSTOPBITS:

	    if (fileno(dev->dev_dcb) <= STDERR_FILENO)
			return -1;

        if (tcgetattr(fileno(dev->dev_dcb), &t) != 0)
            return -1;

        switch (req) {

        case UART_GETSPEED:
            *lvp = cfgetospeed(&t);
            break;

        case UART_GETFLOWCONTROL:
            if (t.c_cflag & CRTSCTS)
                *lvp = UART_HS_RTSCTS;
            else if (t.c_iflag & IXANY)
                *lvp = UART_HS_SOFT;
            else
                *lvp = 0;
            break;

        case UART_GETPARITY:
            if (t.c_cflag & PARENB) {
                if (t.c_cflag & PARODD)
                    *lvp = 1;
                else
                    *lvp = 2;
            } else
                *lvp = 0;
            break;

        case UART_GETDATABITS:
            switch (t.c_cflag & CSIZE) {
            case CS5:
                *lvp = 5;
                break;
            case CS6:
                *lvp = 6;
                break;
            case CS7:
                *lvp = 7;
                break;
            case CS8:
                *lvp = 8;
                break;
            default:
                return -1;
            }
            break;

        case UART_GETSTOPBITS:
            if (t.c_cflag & CSTOPB)
                *lvp = 2;
            else
                *lvp = 1;
            break;
        }
        return 0;

#ifdef UART_SETBLOCKREAD
    case UART_SETBLOCKREAD:
        if (lv)
            ((UNIXDCB *) dev->dev_dcb)->dcb_modeflags |= USART_MF_BLOCKREAD;
        else
            ((UNIXDCB *) dev->dev_dcb)->dcb_modeflags &= ~USART_MF_BLOCKREAD;
        return 0;

    case UART_GETBLOCKREAD:
        if (((UNIXDCB *) dev->dev_dcb)->dcb_modeflags & USART_MF_BLOCKREAD)
            *lvp = 1;
        else
            *lvp = 0;
        return 0;
#endif
    default:
        return -1;
    }
    return -1;
}

/* ======================= Devices ======================== */

/*!
 * \brief USART0 device control block structure.
 */
static UNIXDCB dcb_usart0 = {
    0,                          /* dcb_modeflags */
    0,                          /* dcb_statusflags */
    0,                          /* dcb_rtimeout */
    0,                          /* dcb_wtimeout */
    0,                          /* dbc_last_eol */
};

/*!
 * \brief USART0 device control block structure.
 */
static UNIXDCB dcb_usart1 = {
    0,                          /* dcb_modeflags */
    0,                          /* dcb_statusflags */
    0,                          /* dcb_rtimeout */
    0,                          /* dcb_wtimeout */
    0,                          /* dbc_last_eol */
};

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
    &dcb_usart0,                /*!< Driver control block. */
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
    &dcb_usart1,                /*!< Driver control block. */
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
    &dcb_usart0,                /*!< Driver control block. */
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
    &dcb_usart1,                /*!< Driver control block. */
    0,                          /*!< Driver initialization routine. */
    UnixDevIOCTL,               /*!< Driver specific control function. */
    UnixDevRead,
    UnixDevWrite,
    UnixDevOpen,
    UnixDevClose,
    0
};



/*@}*/
