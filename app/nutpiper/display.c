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
 * Revision 1.1  2003/07/21 17:50:48  haraldkipp
 * First check in
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dev/term.h>

#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/event.h>

#include <arpa/inet.h>

#include "config.h"
#include "player.h"
#include "display.h"

u_short lcd_offset;

#define DISPLAY_LINES       2
#define DISPLAY_VCOLUMNS    80

char *sline[2];
char *mline[2];
u_short mticks[2];

FILE *lcd;

HANDLE updevt;

/*
 * Background thread for playing stream.
 */
THREAD(Displayer, arg)
{
    u_int step[2] = { 0, 0 };
    u_int stepmx[2] = { 0, 0 };
    char *line;
    char *sptr = 0;
    u_char scrolling = DISPLAY_LINES;
    u_char ln;

    fputs(ESC_CURSOROFF, lcd);
    NutThreadSetPriority(128);
    for (;;) {
        NutEventWait(&updevt, 125);

        for (ln = 0; ln < DISPLAY_LINES; ln++) {
            if (mticks[ln]) {
                line = mline[ln];
                mticks[ln]--;
            } else
                line = sline[ln];
            if (stepmx[ln] != strlen(line)) {
                sptr = 0;
                scrolling = DISPLAY_LINES;
                stepmx[ln] = strlen(line);
                step[ln] = 0;
            }
            fprintf(lcd, ESC_POS "%c" "\x20", ln + 32);
            if (stepmx[ln] <= 16) {
                fputs(line, lcd);
                if (stepmx[ln] < 16)
                    fputs(ESC_CLREOL, lcd);
            } else {
                if (step[ln] == stepmx[ln]) {
                    if (scrolling == DISPLAY_LINES)
                        scrolling = ln;
                    if (scrolling == ln) {
                        if (sptr == 0)
                            sptr = line;
                        else {
                            sptr++;
                            if (strlen(sptr) <= 16)
                                step[ln]++;
                        }
                        fprintf(lcd, "%.16s", sptr);
                    }
                } else if (step[ln]++ == 0) {
                    fprintf(lcd, "%.16s", line);
                    sptr = 0;
                    scrolling = DISPLAY_LINES;
                } else if (step[ln] >= stepmx[ln] + 16)
                    step[ln] = 0;
            }
        }
    }
}

void DisplayStation(RADIOSTATION * rsp)
{
    if (rsp->rs_port) {
        if (rsp->rs_name && rsp->rs_name[0])
            strncpy(sline[0], rsp->rs_name, DISPLAY_VCOLUMNS);
        else
            strncpy(sline[0], inet_ntoa(rsp->rs_ip), DISPLAY_VCOLUMNS);
    } else
        sline[0][0] = 0;
}

/*!
 * \brief Display a specified status.
 *
 * \param status Status to be displayed.
 */
void DisplayStatus(u_char status)
{
    RADIOSTATION *rsp = &station[radio.rc_cstation];

    if (radio.rc_cstatus != status) {
        if (status == DIST_FORCE)
            status = radio.rc_cstatus;
        else
            radio.rc_cstatus = status;

        if (status == DIST_NONE) {
            strcpy(sline[0], "NutPiper");
            strcpy(sline[1], "Version 1.0");
            NutEventPost(&updevt);
        } else if (status == DIST_DEAD) {
            sprintf(sline[0], "Station %03u", radio.rc_cstation);
            strcpy(sline[1], "not available");
            NutEventPost(&updevt);
        } else if (status == DIST_CONNECTING) {
            DisplayStation(rsp);
            strcpy(sline[1], "Connecting...");
            NutEventPost(&updevt);
        } else if (status == DIST_CONNECTED) {
            DisplayStation(rsp);
            if (player.psi_metatitle && player.psi_metatitle[0])
                strncpy(sline[1], player.psi_metatitle, DISPLAY_VCOLUMNS);
            else if (rsp->rs_genre && rsp->rs_genre[0])
                strncpy(sline[1], rsp->rs_genre, DISPLAY_VCOLUMNS);
            NutEventPost(&updevt);
        }
    }
}

/*!
 * \brief Display a specified text.
 *
 * \param row  Row position of the message.
 * \param secs Number of seconds to display the message. Set to 0 for
 *             permanent display.
 * \param fmt  Format string containing conversion specifications.
 */
void DisplayMessage(u_char row, u_char secs, CONST char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    if (secs) {
        vsprintf(mline[row], fmt, ap);
        mticks[row] = secs * 4;
    } else
        vsprintf(sline[row], fmt, ap);
    va_end(ap);

    NutEventPost(&updevt);
}

/*!
 * \brief Display specified entry of the list of stations.
 *
 * \param rs Index of the list entry.
 */
void DisplayEntry(u_char rs)
{
    RADIOSTATION *rsp = &station[rs];

    if (rsp->rs_port && rsp->rs_name)
        sprintf(sline[0], "%03u %.12s", rs, rsp->rs_name);
    else
        sprintf(sline[0], "%03u", rs);

    if (rsp->rs_scandead)
        strcpy(sline[1], "not available");
    else if (rsp->rs_scantitle)
        strncpy(sline[1], rsp->rs_scantitle, DISPLAY_VCOLUMNS);
    else if (rsp->rs_genre)
        strncpy(sline[1], rsp->rs_genre, DISPLAY_VCOLUMNS);
    else if (rsp->rs_ip)
        strncpy(sline[1], inet_ntoa(rsp->rs_ip), DISPLAY_VCOLUMNS);
    else
        sline[1][0] = 0;

    NutEventPost(&updevt);
}

/*!
 * \brief Start background thread for display updates.
 *
 * \param name Display device name.
 *
 * \return 0 on success or -1 in case of a failure.
 */
int DisplayInit(char *name)
{
    if ((lcd = fopen(name, "w")) == 0)
        return -1;

    if ((sline[0] = malloc(DISPLAY_VCOLUMNS + 1)) == 0 ||
        (sline[1] = malloc(DISPLAY_VCOLUMNS + 1)) == 0 ||
        (mline[0] = malloc(DISPLAY_VCOLUMNS + 1)) == 0 || (mline[1] = malloc(DISPLAY_VCOLUMNS + 1)) == 0)
        return -1;
    sline[0][DISPLAY_VCOLUMNS] = 0;
    sline[1][DISPLAY_VCOLUMNS] = 0;

    if (NutThreadCreate("displ", Displayer, 0, 512) == 0)
        return -1;

    DisplayStatus(DIST_NONE);

    return 0;
}
