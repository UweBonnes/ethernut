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
 * Revision 1.1  2003/05/09 14:41:02  haraldkipp
 * Initial revision
 *
 * Revision 1.12  2003/04/21 16:58:20  harald
 * Make use of predefined eof
 *
 * Revision 1.11  2003/02/04 17:57:14  harald
 * Version 3 released
 *
 * Revision 1.10  2002/11/02 15:16:09  harald
 * Compiler warning avoided
 *
 * Revision 1.9  2002/06/26 17:29:13  harald
 * First pre-release with 2.4 stack
 *
 */

#include <string.h>
#include <errno.h>

#include <sys/heap.h>
#include <sys/file.h>
#include <sys/device.h>

#include <dev/urom.h>
#include <fs/uromfs.h>

static int UromRead(NUTFILE * fp, void *buffer, int size);
static int UromWrite(NUTFILE * fp, CONST void *buffer, int len);
static int UromWrite_P(NUTFILE * fp, PGM_P buffer, int len);
static NUTFILE *UromOpen(NUTDEVICE * dev, CONST char *name, int mode,
                         int acc);
static int UromClose(NUTFILE * fp);
static long UromSize(NUTFILE * fp);

/*!
 * \addtogroup xgurom
 */
/*@{*/

/*!
 * \brief UROM device information structure.
 *
 * All this UROM stuff is so simple, that it even mimics
 * its own device driver.
 */
NUTDEVICE devUrom = {
    0,                          /*!< Pointer to next device. */
    {'U', 'R', 'O', 'M', 0, 0, 0, 0, 0},        /*!< Unique device name. */
    IFTYP_ROM,                  /*!< Type of device. Obsolete. */
    0,                          /*!< Base address. Unused. */
    0,                          /*!< First interrupt number. Unused. */
    0,                          /*!< Interface control block. Unused. */
    0,                          /*!< Driver control block. Unused. */
    0,                          /*!< Driver initialization routine. Not supported. */
    0,                          /*!< Driver specific control function. Not supported. */
    UromRead,                   /*!< Read data from a file. */
    UromWrite,                  /*!< Write data to a file. */
    UromWrite_P,                /*!< Write data from program space to a file. */
    UromOpen,                   /*!< Open a file. */
    UromClose,                  /*!< Close a file. */
    UromSize                    /*!< Return file size. */
};

/*!
 * \brief Opens an existing file for reading.
 *
 * \deprecated Use _open() or fopen() in new programs.
 *
 * \param name Points to a string that specifies the name of the
 *             file to open. The name must exactly match the
 *             full pathname of the file.
 *
 * \return A pointer to a ROMFILE structure that can be used
 *         to read from the file and retrieve information about
 *         the file.
 */
ROMFILE *NutRomFileOpen(char *name)
{
    ROMENTRY *rome;
    ROMFILE *romf = 0;

    for (rome = romEntryList; rome; rome = rome->rome_next) {
        if (strcmp(rome->rome_name, name) == 0)
            break;
    }
    if (rome) {
        if ((romf = NutHeapAllocClear(sizeof(ROMFILE))) != 0)
            romf->romf_entry = rome;
    }
    return romf;
}

/*!
 * \brief Close a previously opened file.
 *
 * \deprecated Use _close() or fclose() in new programs.
 *
 * \param romf Identifies the file to close. This pointer must
 *             have been created by calling NutRomFileOpen().
 *
 * \return 0 if the function is successful, -1 otherwise.
 */
int NutRomFileClose(ROMFILE * romf)
{
    return NutHeapFree(romf);
}

/*!
 * \brief Read data from a file.
 *
 * Read up to a specified number of bytes of data from a
 * file into a buffer. The function may read fewer than
 * the specified number of bytes if it reaches the end of
 * the file.
 *
 * \deprecated New programs should use stdio.
 *
 * \param romf Identifies the file to read from. This pointer must
 *             have been created by calling NutRomFileOpen().
 * \param data Points to the buffer that receives the data.
 * \param size Specifies the number of bytes to read from the file.
 *
 * \return The number of bytes read from the file or -1 if
 *         an error occured.
 */
int NutRomFileRead(ROMFILE * romf, void *data, size_t size)
{
    ROMENTRY *rome = romf->romf_entry;

    if (size > rome->rome_size - romf->romf_pos)
        size = rome->rome_size - romf->romf_pos;
    if (size) {
        memcpy_P(data, rome->rome_data + romf->romf_pos, size);
        romf->romf_pos += size;
    }
    return size;
}

/*!
 * \brief Retrieve the size of a file.
 *
 * \deprecated New programs should use stdio.
 *
 * \bug stdio doesn't support fstat yet.
 *
 * \param romf Identifies the file to query. This pointer must
 *             have been created by calling NutRomFileOpen().
 *
 * \return The number of bytes in this file or -1 if an
 *         error occured.
 */
int NutRomFileSize(ROMFILE * romf)
{
    return romf->romf_entry->rome_size;
}

/*!
 * \brief Move the file pointer to a new position.
 *
 * The file pointer is maintained by Nut/OS. It points to
 * the next byte to be read from a file. The file pointer
 * is automatically incremented for each byte read. When
 * the file is opened, it is at position 0, the beginning
 * of the file.
 *
 * \deprecated New programs should use stdio.
 *
 * \bug stdio's seek is not working.
 *
 * \param romf Identifies the file to seek. This pointer must
 *             have been created by calling NutRomFileOpen().
 * \param pos  Specifies the new absolute position of the file pointer.
 *
 * \return 0 if the function is successful, -1 otherwise.
 */
int NutRomFileSeek(ROMFILE * romf, int pos)
{
    if (pos < 0 || pos > NutRomFileSize(romf))
        return -1;
    romf->romf_pos = pos;

    return 0;
}

/*! 
 * \brief Read from device. 
 */
static int UromRead(NUTFILE * fp, void *buffer, int size)
{
    ROMFILE *romf = fp->nf_fcb;
    ROMENTRY *rome = romf->romf_entry;

    if ((u_short) size > rome->rome_size - romf->romf_pos)
        size = rome->rome_size - romf->romf_pos;
    if (size) {
        memcpy_P(buffer, rome->rome_data + romf->romf_pos, size);
        romf->romf_pos += size;
    }
    return size;
}


/*! 
 * \brief Write data to a file.
 *
 * \return Always returns -1.
 */
static int UromWrite(NUTFILE * fp, CONST void *buffer, int len)
{
    return -1;
}

/*! 
 * \brief Write data from progam space to a file.
 *
 * \return Always returns -1.
 */
static int UromWrite_P(NUTFILE * fp, PGM_P buffer, int len)
{
    return -1;
}


/*! 
 * \brief Open a file.
 */
static NUTFILE *UromOpen(NUTDEVICE * dev, CONST char *name, int mode,
                         int acc)
{
    NUTFILE *fp = NutHeapAlloc(sizeof(NUTFILE));
    ROMENTRY *rome;
    ROMFILE *romf = 0;

    if (fp == 0) {
        errno = ENOMEM;
        return NUTFILE_EOF;
    }

    for (rome = romEntryList; rome; rome = rome->rome_next) {
        if (strcmp(rome->rome_name, name) == 0)
            break;
    }
    if (rome) {
        if ((romf = NutHeapAllocClear(sizeof(ROMFILE))) != 0)
            romf->romf_entry = rome;
        else
            errno = ENOMEM;
    } else
        errno = ENOENT;

    if (romf) {
        fp->nf_next = 0;
        fp->nf_dev = dev;
        fp->nf_fcb = romf;
    } else {
        NutHeapFree(fp);
        fp = NUTFILE_EOF;
    }

    return fp;
}

/*! 
 * \brief Close a file. 
 */
static int UromClose(NUTFILE * fp)
{
    if (fp && fp != NUTFILE_EOF) {
        if (fp->nf_fcb)
            NutHeapFree(fp->nf_fcb);
        NutHeapFree(fp);
    }
    return 0;
}

/*! 
 * \brief Query file size.
 */
static long UromSize(NUTFILE * fp)
{
    ROMFILE *romf = fp->nf_fcb;
    ROMENTRY *rome = romf->romf_entry;

    return (long) rome->rome_size;
}

/*@}*/
