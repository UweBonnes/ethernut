#ifndef _NET_NETDEBUG_H_
#define _NET_NETDEBUG_H_

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
 * Revision 1.1  2003/05/09 14:41:12  haraldkipp
 * Initial revision
 *
 * Revision 1.6  2003/05/06 18:45:34  harald
 * PPP debug functions added
 *
 * Revision 1.5  2003/02/04 18:00:42  harald
 * Version 3 released
 *
 * Revision 1.4  2002/10/29 15:40:49  harald
 * *** empty log message ***
 *
 * Revision 1.3  2002/06/26 17:29:18  harald
 * First pre-release with 2.4 stack
 *
 */

#include <stdio.h>

#include <sys/socket.h>
#include <netinet/tcp.h>
#include <net/errno.h>

#ifdef __cplusplus
extern "C" {
#endif

extern FILE *__tcp_trs;
extern u_char __tcp_trf;

extern FILE *__ppp_trs;
extern u_char __ppp_trf;

extern void NutTraceTcp(FILE *stream, u_char flags);
extern void NutTracePPP(FILE *stream, u_char flags);

extern void NutDumpTcpHeader(FILE *stream, u_char *ds, TCPSOCKET *sock, NETBUF *nb);
extern void NutDumpSockState(FILE *stream, u_char state, u_char *lead, u_char *trail);
extern void NutDumpSocketList(FILE *stream);

extern void NutDumpLcpOption(FILE * stream, NETBUF * nb);
extern void NutDumpLcp(FILE * stream, NETBUF * nb);
extern void NutDumpPpp(FILE * stream, NETBUF * nb);


#ifdef __cplusplus
}
#endif

#endif
