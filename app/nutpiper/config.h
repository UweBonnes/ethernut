#ifndef _CONFIG_H_
#define _CONFIG_H_

/*
 * Copyright (C) 2003 by egnite Software GmbH. All rights reserved.
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

/*!
 * $Log$
 * Revision 1.2  2009/02/18 12:18:58  olereinhardt
 * 2009-02-18  Ole Reinhardt <ole.reinhardt@thermotemp.de>
 *
 *           Fixed compilier warnings. Especialy signedness of char buffers
 *           as well as unused code on arm platform and main functions without
 *           return value
 *
 * Revision 1.1  2003/07/21 17:50:48  haraldkipp
 * First check in
 *
 */

#define CONFAPP_EE_OFFSET   512
#define CONFAPP_EE_NAME     "NUTPIPER"

#define MAXLEN_URL          32
#define MAXNUM_STATIONS     128

#define NEXT_STATION        255
#define PREV_STATION        254

typedef struct {
    u_short rs_port;
    u_long rs_ip;
    char  *rs_url;
    char  *rs_name;
    char  *rs_genre;
    u_long rs_metaint;
    u_short rs_bitrate;
    u_char rs_scandead;
    char  *rs_scantitle;
} RADIOSTATION;

extern RADIOSTATION *station;

#define DIST_NONE       0
#define DIST_WAIT       1
#define DIST_CONNECTING 2
#define DIST_CONNECTED  3
#define DIST_BUFFER     4
#define DIST_TITLE      5
#define DIST_STATION    6
#define DIST_VOLUME     7
#define DIST_DEAD       8
#define DIST_FORCE      9


typedef struct {
    u_char rc_off;              /*!< \brief Set when switched off. */
    u_char rc_cstatus;          /*!< \brief Current status. */
    u_char rc_dstatus;          /*!< \brief Displayed status. */
    u_char rc_cstation;         /*!< \brief Current station. */
    u_char rc_rstation;         /*!< \brief Requested station. */
    u_char rc_cvolume;          /*!< \brief Current volume. */
    u_char rc_rvolume;          /*!< \brief Requested station. */
    u_char rc_cmute;            /*!< \brief Muted. */
    u_char rc_rmute;            /*!< \brief Muted requested. */
} RADIOCONTROL;

extern RADIOCONTROL radio;

extern size_t ConfigSize(void);
extern int ConfigLoad(void);
extern void ConfigResetFactory(void);
extern void ConfigSave(void);
extern void ConfigSaveControl(void);
extern int ConfigStation(u_char idx, CONST char * url);

#endif
