#ifndef _DEV_CHAT_H_
#define _DEV_CHAT_H_

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
 */

/*
 * $Log$
 * Revision 1.4  2004/03/08 11:18:54  haraldkipp
 * Debug output added.
 *
 * Revision 1.3  2004/01/06 18:12:36  drsung
 * Bugfix multiple define: '_chat_report' under ICCAVR
 *
 * Revision 1.2  2003/10/13 10:15:54  haraldkipp
 * Added Jelle's report function
 *
 * Revision 1.1.1.1  2003/05/09 14:41:05  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.1  2003/03/31 14:53:23  harald
 * Prepare release 3.1
 *
 */

#define CHAT_ARG_SEND           0
#define CHAT_ARG_ABORT          1
#define CHAT_ARG_TIMEOUT        2
#define CHAT_ARG_REPORT					3

#define CHAT_MAX_ABORTS         10
#define CHAT_MAX_REPORT_SIZE    32

#define CHAT_DEFAULT_TIMEOUT    45

typedef struct {
    int chat_fd;
    u_char chat_arg;
    u_char chat_aborts;
    u_char *chat_abort[CHAT_MAX_ABORTS];
    u_char chat_abomat[CHAT_MAX_ABORTS];
    u_char *chat_report_search;
    u_char chat_repmat;
    char chat_report_state;
} NUTCHAT;

extern u_char *chat_report;

#ifdef NUTDEBUG
#include <stdio.h>
#endif

__BEGIN_DECLS

#ifdef NUTDEBUG
void NutTraceChat(FILE * stream, u_char flags);
#endif

int NutChatExpectString(NUTCHAT *ci, char *str);
int NutChatExpect(NUTCHAT *ci, char *str);
int NutChatSend(NUTCHAT *ci, char *str);
NUTCHAT *NutChatCreate(int fd);
void NutChatDestroy(NUTCHAT *ci);
int NutChat(int fd, CONST char *script);
int NutChat_P(int fd, PGM_P script);

__END_DECLS

#endif
