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
 * Revision 1.3  2008/07/09 14:23:49  haraldkipp
 * Info about NutRegisterCgi's first parameter updated.
 *
 * Revision 1.2  2006/10/08 16:48:22  haraldkipp
 * Documentation fixed
 *
 * Revision 1.1.1.1  2003/05/09 14:41:56  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.7  2003/02/04 18:17:31  harald
 * Version 3 released
 *
 * Revision 1.6  2002/06/26 17:29:49  harald
 * First pre-release with 2.4 stack
 *
 */

#include <string.h>
#include <sys/heap.h>

#include <pro/httpd.h>

/*!
 * \addtogroup xgHTTPD
 */
/*@{*/

CGIFUNCTION *volatile cgiFunctionList = 0;

/*!
 * \brief Register a CGI function.
 *
 * \todo Duplicate names not checked.
 *
 * \param name Pointer to a buffer containing the name of this CGI function. 
 *             The caller must make sure, that the buffer reamins valid
 *             throughout the lifetime of the server. This can be done, for
 *             example, by using a global variable or by allocating the
 *             buffer from heap memory. Using string literals may not work
 *             on all targets or with all compilers.
 * \param func The function to be called, if the
 *             client requests the specified name.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutRegisterCgi(char *name, int (*func) (FILE *, REQUEST *))
{
    CGIFUNCTION *cgi;

    if ((cgi = NutHeapAlloc(sizeof(CGIFUNCTION))) == 0)
        return -1;
    cgi->cgi_next = cgiFunctionList;
    cgi->cgi_name = name;
    cgi->cgi_func = func;
    cgiFunctionList = cgi;

    return 0;
}

/*!
 * \brief Process an incoming CGI request.
 *
 * Applications do not need to call this function. It
 * is automatically called by NutHttpProcessRequest().
 *
 * \param stream Stream of the socket connection, previously opened for 
 *               binary read and write.
 * \param req    Contains the HTTP request.
 */
void NutCgiProcessRequest(FILE * stream, REQUEST * req)
{
    CGIFUNCTION *cgi;

    if (req->req_method != METHOD_GET && req->req_method != METHOD_POST) {
        NutHttpSendError(stream, req, 501);
        return;
    }
    for (cgi = cgiFunctionList; cgi; cgi = cgi->cgi_next) {
        if (strcasecmp(cgi->cgi_name, req->req_url + 8) == 0)
            break;
    }
    if (cgi == 0)
        NutHttpSendError(stream, req, 404);
    else if ((*cgi->cgi_func) (stream, req))
        NutHttpSendError(stream, req, 500);
    return;
}

/*@}*/
