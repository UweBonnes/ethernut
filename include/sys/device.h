#ifndef _SYS_DEVICE_H_
#define _SYS_DEVICE_H_

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
 * Revision 1.1  2003/05/09 14:41:19  haraldkipp
 * Initial revision
 *
 * Revision 1.18  2003/05/06 17:58:04  harald
 * ATmega128 definitions moved to compiler include
 *
 * Revision 1.17  2003/03/31 14:34:08  harald
 * Added character device
 *
 * Revision 1.16  2003/02/04 18:00:52  harald
 * Version 3 released
 *
 * Revision 1.15  2003/01/14 16:35:04  harald
 * Definitions moved
 *
 * Revision 1.14  2002/11/02 15:17:01  harald
 * Library dependencies moved to compiler.h
 *
 * Revision 1.13  2002/09/15 16:46:28  harald
 * *** empty log message ***
 *
 * Revision 1.12  2002/08/08 17:24:21  harald
 * Using time constants by KU
 *
 * Revision 1.11  2002/07/03 16:45:41  harald
 * Using GCC 3.2
 *
 * Revision 1.10  2002/06/26 17:29:28  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/file.h>

/*!
 * \file sys/device.h
 * \brief Nut/OS device definitions.
 */

__BEGIN_DECLS

/*!
 * \addtogroup xgDevice
 */
/*@{*/


// wait times for emulation and reality
// has to be overworked

#ifndef __EMULATION__
#define WAIT5		5
#define WAIT50		50
#define WAIT100		100
#define WAIT250		250
#define WAIT500		500
#else
#define WAIT5		1
#define WAIT50		5
#define WAIT100		10
#define WAIT250		25
#define WAIT500		50
#endif

#define IFTYP_RAM       0	/*!< \brief RAM device */
#define IFTYP_ROM       1	/*!< \brief ROM device */
#define IFTYP_STREAM    2	/*!< \brief Stream device */
#define IFTYP_NET       3	/*!< \brief Net device */
#define IFTYP_TCPSOCK	4	/*!< \brief TCP socket */
#define IFTYP_CHAR      5	/*!< \brief Character stream device */

/*!
 * \brief Device structure type.
 */
typedef struct _NUTDEVICE NUTDEVICE;

/*!
 * \struct _NUTDEVICE device.h sys/device.h
 * \brief Device structure.
 */
struct _NUTDEVICE {

    /*! 
     * \brief Link to the next device structure. 
     */
    NUTDEVICE *dev_next;

    /*! 
     * \brief Unique device name. 
     */
    u_char dev_name[9];

    /*! 
     * \brief Type of interface. 
     *
     * May be any of the following:
     * - IFTYP_RAM
     * - IFTYP_ROM
     * - IFTYP_STREAM
     * - IFTYP_NET
     * - IFTYP_TCPSOCK
     * - IFTYP_CHAR
     */
    u_char dev_type;

    /*! 
     * \brief Hardware base address.
     *
     * Will be set by calling NutRegisterDevice(). On some device 
     * drivers this address may be fixed.
     */
    u_short dev_base;

    /*! \brief Interrupt registration number. 
     *
     * Will be set by calling NutRegisterDevice(). On some device 
     * drivers the interrupt may be fixed.
     */
    u_char dev_irq;

    /*! \brief Interface control block.
     *
     * With stream devices, this points to the IFSTREAM structure and 
     * with network devices this is a pointer to the IFNET structure.
     */
    void *dev_icb;

    /*! 
     * \brief Driver control block.
     *
     * Points to a device specific information block.
     */
    void *dev_dcb;

    /*! 
     * \brief Driver initialization routine. 
     *
     * With stream devices this is called during NutDeviceOpen(). For 
     * network devices this routine is called within NutNetIfConfig().
     */
    int (*dev_init) (NUTDEVICE *);

    /*! 
     * \brief Driver control function.
     *
     * Used to modify or query device specific settings.
     */
    int (*dev_ioctl) (NUTDEVICE *, int, void *);

    /*! 
     * \brief Read from device. 
     */
    int (*dev_read) (NUTFILE *, void *, int);

    /*! 
     * \brief Write to device. 
     */
    int (*dev_write) (NUTFILE *, CONST void *, int);

    /*! 
     * \brief Write to device. 
     */
    int (*dev_write_P) (NUTFILE *, PGM_P, int);

    /*! 
     * \brief Open a device or file. 
     */
    NUTFILE * (*dev_open) (NUTDEVICE *, CONST char *, int, int);

    /*! 
     * \brief Close a device or file. 
     */
    int (*dev_close) (NUTFILE *);

    /*! 
     * \brief Request file size. 
     */
    long (*dev_size) (NUTFILE *);

};

/*!
 * \brief Device structure type.
 */
typedef struct _NUTVIRTUALDEVICE NUTVIRTUALDEVICE;

/*!
 * \struct _NUTVIRTUALDEVICE device.h sys/device.h
 * \brief Virtual device structure.
 */
struct _NUTVIRTUALDEVICE {
    NUTVIRTUALDEVICE *vdv_next;
    NUTVIRTUALDEVICE *vdv_zero;
    u_char vdv_type;
    int (*vdv_read) (void *, void *, int);
    int (*vdv_write) (void *, CONST void *, int);
    int (*vdv_write_P) (void *, PGM_P, int);
    int (*vdv_ioctl) (void *, int, void *);
};


/*@}*/


extern NUTDEVICE *nutDeviceList;

extern int NutRegisterDevice(NUTDEVICE * dev, u_short base, u_char irq);
extern NUTDEVICE *NutDeviceLookup(CONST char *name);
extern NUTDEVICE *NutDeviceOpen(CONST char *name);
extern int NutDeviceClose(NUTDEVICE * dev);
extern int NutDeviceRead(NUTDEVICE * dev, void *data, int size);
extern int NutDeviceReadTran(NUTDEVICE * dev, void *data, int size);
extern int NutDeviceGetLine(NUTDEVICE * dev, void *data, int size);
extern int NutDeviceWrite(NUTDEVICE * dev, CONST void *data, int len);
extern int NutDeviceWriteTran(NUTDEVICE * dev, CONST void *data);
extern int NutDeviceWrite_P(NUTDEVICE * dev, PGM_P data, int len);
extern int NutDeviceIOCtl(NUTDEVICE * dev, int req, void *conf);

__END_DECLS

#endif
