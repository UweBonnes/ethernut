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
 * Revision 1.4  2008/07/17 11:33:32  olereinhardt
 * - Check for unique cgi names
 * - Alloc local copy of cgi name. Allows dynamicaly generated cgi names
 * - Function to register a different cgi-bin path. Multiple path allowed.
 *   See httpd demo for an example
 *
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
#include <alloca.h>
#include <sys/heap.h>

#include <pro/httpd.h>

/*!
 * \addtogroup xgHTTPD
 */
/*@{*/

CGIFUNCTION *volatile cgiFunctionList = 0;
char *cgiBinPath = NULL;

/*!
 * \brief Register a new cgi-bin path.
 *
 * This function allows to redfine the cgi-bin path. Default is "cgi-bin/"
 * \param path New path.
 */
     
void NutRegisterCgiBinPath(char *path)
{
    if (cgiBinPath) {
        NutHeapFree(cgiBinPath);
    }
    
    cgiBinPath = NutHeapAlloc(strlen(path) + 1);
    strcpy(cgiBinPath, path);    
}
     
/*!
 * \brief Check if request is a cgi call.
 *
 * This functions checks the request if it's a cgi all and in case calls the cgi
 *
 * \param stream Stream of the socket connection, previously opened for 
 *               binary read and write.
 * \param req    Contains the HTTP request.  
 */

int NutCgiCheckRequest(FILE * stream, REQUEST * req)
{
    char * cgi_bin = "cgi-bin/";
    char * tmp;
    char * save_ptr = NULL;
    
    if (cgiBinPath != NULL) {
        cgi_bin = cgiBinPath;
    }
    tmp = alloca(strlen(cgi_bin) + 1);
    strcpy(tmp, cgi_bin);
    
    tmp = strtok_r(tmp, ";", &save_ptr);
    
    do {
        if ((tmp != NULL) && (strncasecmp(req->req_url, tmp, strlen(tmp)) == 0)) {
            NutCgiProcessRequest(stream, req, strlen(tmp));
            return 1;
        }    
        tmp = strtok_r(NULL, ";", &save_ptr);
    } while (tmp != NULL);
    return 0;
}

/*!
 * \brief Register a CGI function.
 *
 * \param name Name of this CGI function. No dublicates allowed
 * \param func The function to be called, if the
 *             client requests the specified name.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutRegisterCgi(char *name, int (*func) (FILE *, REQUEST *))
{
    int unique_name = 1;
    CGIFUNCTION *cgi;
    
    cgi = cgiFunctionList;
    while (cgi != NULL) {
        if (strcmp(name, cgi->cgi_name) == 0) {
            unique_name = 0;
            break;
        }
        cgi = cgi->cgi_next;
    }
    
    if ((!unique_name) || ((cgi = NutHeapAlloc(sizeof(CGIFUNCTION))) == 0)) {
        return -1;
    }
    cgi->cgi_next = cgiFunctionList;
    cgi->cgi_name = NutHeapAlloc(strlen(name)+1);
    strcpy(cgi->cgi_name, name);
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
void NutCgiProcessRequest(FILE * stream, REQUEST * req, int name_pos)
{
    CGIFUNCTION *cgi;
    
    if (req->req_method != METHOD_GET && req->req_method != METHOD_POST) {
        NutHttpSendError(stream, req, 501);
        return;
    }
    
    for (cgi = cgiFunctionList; cgi; cgi = cgi->cgi_next) {
        if (strcasecmp(cgi->cgi_name, req->req_url + name_pos) == 0)
            break;
    }
    if (cgi == 0)
        NutHttpSendError(stream, req, 404);
    else if ((*cgi->cgi_func) (stream, req))
        NutHttpSendError(stream, req, 500);
    return;
}

/*@}*/
