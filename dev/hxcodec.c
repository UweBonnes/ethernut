/*
 * Copyright (C) 2008 by egnite GmbH.
 * Copyright (C) 2001-2007 by egnite Software GmbH.
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

/*
 * $Log$
 * Revision 1.1  2008/10/05 16:56:15  haraldkipp
 * Added Helix audio device.
 *
 */

#include <cfg/arch/avr.h>
#include <cfg/audio.h>

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/heap.h>

#include <dev/irqreg.h>

#include <contrib/hxmp3/mp3dec.h>
#include <contrib/hxmp3/hermite.h>
#include <dev/hxcodec.h>
#include <dev/tlv320dac.h>

#include <sys/bankmem.h>

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#if 0
/* Use for local debugging. */
#define NUTDEBUG
#include <stdio.h>
#endif

#ifndef DAC_OUTPUT_RATE
#if defined (AT91SAM9260_EK)
#define DAC_OUTPUT_RATE     44100
#else
#define DAC_OUTPUT_RATE     8000
#endif
#endif

#ifndef MP3_BUFSIZ
#if defined (AT91SAM9260_EK)
#define MP3_BUFSIZ          1048576
#else
#define MP3_BUFSIZ          (4 * MAINBUF_SIZE)
#endif
#endif

#ifndef ENABLE_RESAMPLER
#if defined (AT91SAM9260_EK)
#define ENABLE_RESAMPLER
#endif
#endif

#ifdef ENABLE_RESAMPLER
static int rs_maxout;
static short *rs_pcmbuf;
#endif

/*!
 * \addtogroup xgHelixCodec
 */
/*@{*/

typedef struct _HXDCB {
    int dcb_pbstat;     /*!< \brief Playback status. */
    uint32_t dcb_scmd;    /*!< \brief Requested command flags, see HXREQ_ flags. */
    int dcb_crvol;      /*!< \brief Current volume of right channel. */
    int dcb_srvol;      /*!< \brief Requested volume of right channel. */
    int dcb_clvol;      /*!< \brief Current volume of left channel. */
    int dcb_slvol;      /*!< \brief Requested volume of left channel. */
    uint32_t dcb_pbwlo;   /*!< \brief Playback buffer low watermark. */
    uint32_t dcb_pbwhi;   /*!< \brief Playback buffer high watermark. */
} HXDCB;

static HXDCB dcb;
static void *hres;

typedef struct _MP3PLAYERINFO {
    HMP3Decoder mpi_mp3dec;
    MP3FrameInfo mpi_frameinfo;
} MP3PLAYERINFO;

static MP3PLAYERINFO mpi;

static short pi_pcmbuf[MAX_NCHAN * MAX_NGRAN * MAX_NSAMP];

/*! \brief Receiver buffer.
 * 
 * The receiver plug-in will store incoming data in this ring buffer.
 */
static u_char ri_buff[MP3_BUFSIZ + 2 * MAINBUF_SIZE];

/*! \brief Number of bytes available in the ring buffer.
 */
static u_int ri_avail;

static int first_frame;
static int samprate;

static int HelixPlayerFlush(void)
{
    int tmo = 1000; /* TODO: Configurable timeout. */

    /* Stupid polling for now. */
    while(dcb.dcb_pbstat == CODEC_STATUS_PLAYING) {
        NutSleep(1);
        if (tmo-- <= 0) {
            return -1;
        }
    }
    return 0;
}

/*!
 * \brief Send data to the decoder.
 *
 * A carriage return character will be automatically appended 
 * to any linefeed.
 *
 * \return Number of characters sent.
 */
static int HelixWrite(NUTFILE * fp, CONST void *data, int len)
{
    int ec;
    int rbytes;
    int skip;
    u_char *bufptr;

    /* Flush buffer if data pointer is a NULL pointer. */
    if (data == NULL || len == 0) {
        return HelixPlayerFlush();
    }
    /* Add the data to the MP3 buffer. */
    if (len) {
        memcpy(&ri_buff[ri_avail], data, len);
        ri_avail += len;
    }
    /* If we have a full frame, play it. */
    if (ri_avail >= 4 * MAINBUF_SIZE) {
        rbytes = ri_avail;
        bufptr = ri_buff;
        while (rbytes > 2 * MAINBUF_SIZE) {
            if ((skip = MP3FindSyncWord(bufptr, rbytes)) < 0) {
			    break;
		    } else if (skip) {
                rbytes -= skip;
                bufptr += skip;
            } else {
                ec = MP3Decode(mpi.mpi_mp3dec, &bufptr, &rbytes, pi_pcmbuf, 0);
                if (ec) {
                    break;
                }
                if (first_frame) {
                    MP3GetLastFrameInfo(mpi.mpi_mp3dec, &mpi.mpi_frameinfo);
                    if (mpi.mpi_frameinfo.nChans == 1) {
                        samprate = mpi.mpi_frameinfo.samprate / 2;
                    }
                    else if (mpi.mpi_frameinfo.nChans == 2) {
                        samprate = mpi.mpi_frameinfo.samprate;
                    }
                    else {
                        samprate = 0;
                        ec = -1;
                    }
                    if (mpi.mpi_frameinfo.samprate < 8000 || samprate > DAC_OUTPUT_RATE) {
                        ec = -1;
                    }
                    if (mpi.mpi_frameinfo.bitsPerSample != 16) {
                        ec = -1;
                    }
#ifdef ENABLE_RESAMPLER
                    /* If needed, initialize resampler. */
                    if (ec == 0 && samprate != DAC_OUTPUT_RATE) {
                        if ((hres = RAInitResamplerHermite(samprate, DAC_OUTPUT_RATE, mpi.mpi_frameinfo.nChans)) == NULL) {
                            ec = -1;
                        }
                        rs_maxout = RAGetMaxOutputHermite(mpi.mpi_frameinfo.outputSamps, hres);
                        if ((rs_pcmbuf = malloc(rs_maxout * 2)) == NULL) {
                            ec = -1;
                        }
                    }
#endif
                    if (ec == 0) {
                        first_frame = 0;
                    }
                }
                if (ec == 0) {
                    if (Tlv320DacWrite(pi_pcmbuf, mpi.mpi_frameinfo.outputSamps)) {
                        return -1;
                    }
                }
            }
        }
        if (rbytes > 0) {
            memcpy(ri_buff, &ri_buff[ri_avail - rbytes], rbytes);
            ri_avail = rbytes;
        } else {
            ri_avail = 0;
        }
    }
    return len;
}

#ifdef __HARVARD_ARCH__
/*! 
 * \brief Write to device.
 *
 * Similar to HelixWrite() except that the data is expected in program memory.
 *
 * This function is implemented for CPUs with Harvard Architecture only.
 *
 * This function is called by the low level output routines of the 
 * \ref xrCrtLowio "C runtime library", using the 
 * \ref _NUTDEVICE::dev_write_P entry.
 *
 * \param nfp    Pointer to a \ref NUTFILE structure, obtained by a previous 
 *               call to PnutFileOpen().
 * \param buffer Pointer to the data in program space. If zero, then the
 *               output buffer will be flushed.
 * \param len    Number of bytes to write.
 *
 * \return The number of bytes written. A return value of -1 indicates an 
 *         error. Currently this function is not implemented and always
 *         returns -1.
 *
 */
static int HelixWrite_P(NUTFILE * nfp, PGM_P buffer, int len)
{
    return -1;
}
#endif

/*!
 * \brief Open codec device.
 *
 * \return Pointer to a static NUTFILE structure.
 */
static NUTFILE *HelixOpen(NUTDEVICE * dev, CONST char *name, int mode, int acc)
{
    NUTFILE *nfp;

    ri_avail = 0;

    if ((mpi.mpi_mp3dec = MP3InitDecoder()) == NULL) {
        return NUTFILE_EOF;
    }

    first_frame = 1;

    NutSleep(2);

    nfp = malloc(sizeof(NUTFILE));
    nfp->nf_next = NULL;
    nfp->nf_dev = dev;
    nfp->nf_fcb = NULL;

    return nfp;
}

/*! 
 * \brief Close codec device.
 */
static int HelixClose(NUTFILE * nfp)
{
    int rc = HelixPlayerFlush();

    if (nfp) {
        if (mpi.mpi_mp3dec) {
            MP3FreeDecoder(mpi.mpi_mp3dec);
            mpi.mpi_mp3dec = NULL;
        }
        free(nfp);
    }
    return rc;
}

/*!
 * \brief Handle I/O controls for audio codec.
 *
 * - AUDIO_PLAY         Force playback start, even if the buffer level is too lower.
 * - AUDIO_CANCEL       Cancel playback and discard all buffered data.
 * - AUDIO_GET_STATUS   Sets an int to 1 if the player is running, 0 if idle.
 * - AUDIO_GET_PLAYGAIN Sets an int to the current playback gain, 0..-127.
 * - AUDIO_SET_PLAYGAIN Reads the requested playback gain from an int, 0..-127.
 * - AUDIO_GET_PBSIZE   Sets an unsigned long with the size of the playback buffer.
 * - AUDIO_SET_PBSIZE   Sets the size the playback buffer using an unsigned long.
 * - AUDIO_GET_PBLEVEL  Sets an unsigned long with the number of bytes in the playback buffer.
 * - AUDIO_GET_PBWLOW   Sets an unsigned long with the low watermark of the playback buffer.
 * - AUDIO_SET_PBWLOW   Sets the low watermark (unsigned long) of the playback buffer.
 * - AUDIO_GET_PBWHIGH  Sets an unsigned long with the high watermark of the playback buffer.
 * - AUDIO_SET_PBWHIGH  Sets the high watermark (unsigned long) of the playback buffer.
 * - AUDIO_BEEP         Plays a short sine wave beep.
 *
 * \return 0 on success, -1 otherwise.
 */
static int HelixIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    int rc = 0;
    int *ivp = (int *) conf;
    int iv = *ivp;

    switch (req) {
    case AUDIO_PLAY:
        /* Immediately start playing. */
        break;
    case AUDIO_CANCEL:
        /* Immediately stop playing and discard buffer. */
        break;
    case AUDIO_GET_STATUS:
        break;
    case AUDIO_GET_PLAYGAIN:
        break;
    case AUDIO_SET_PLAYGAIN:
        if (iv > AUDIO_DAC_MAX_GAIN) {
            iv = AUDIO_DAC_MAX_GAIN;
        }
        if (iv < AUDIO_DAC_MIN_GAIN) {
            iv = AUDIO_DAC_MIN_GAIN;
        }
        dcb.dcb_slvol = dcb.dcb_srvol = iv;
        Tlv320DacSetVolume(dcb.dcb_slvol, dcb.dcb_srvol);
        break;
    default:
        rc = -1;
        break;
    }
    return rc;
}

/*
 * Called via dev_init pointer when the device is registered.
 */
static int HelixInit(NUTDEVICE * dev)
{
    Tlv320DacInit(DAC_OUTPUT_RATE);

    dcb.dcb_srvol = -32;
    dcb.dcb_slvol = -32;
    Tlv320DacSetVolume(dcb.dcb_srvol, dcb.dcb_slvol);

    return 0;
}

/*!
 * \brief Device information structure.
 *
 * An application must pass a pointer to this structure to 
 * NutRegisterDevice() before using this driver.
 *
 * The device is named \b audio0.
 *
 * \showinitializer
 */
NUTDEVICE devHelixCodec = {
    0,              /* Pointer to next device, dev_next. */
    {'a', 'u', 'd', 'i', 'o', '0', 0, 0, 0},    /* Unique device name, dev_name. */
    IFTYP_CHAR,     /* Type of device, dev_type. */
    0,              /* Base address, dev_base (not used). */
    0,              /* First interrupt number, dev_irq (not used). */
    0,              /* Interface control block, dev_icb (not used). */
    &dcb,           /* Driver control block, dev_dcb. */
    HelixInit,         /* Driver initialization routine, dev_init. */
    HelixIOCtl,        /* Driver specific control function, dev_ioctl. */
    NULL,           /* Read from device, dev_read. */
    HelixWrite,        /* Write to device, dev_write. */
#ifdef __HARVARD_ARCH__
    HelixWrite_P,      /* Write data from program space to device, dev_write_P. */
#endif
    HelixOpen,         /* Open a device or file, dev_open. */
    HelixClose,        /* Close a device or file, dev_close. */
    NULL            /* Request file size, dev_size. */
};

/*@}*/
