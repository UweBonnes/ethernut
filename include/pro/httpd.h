#ifndef _PRO_HTTPD_H_
#define _PRO_HTTPD_H_

/*
 * Copyright (C) 2001-2004 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.2  2004/12/16 10:17:18  haraldkipp
 * Added Mikael Adolfsson's excellent parameter parsing routines.
 *
 */

#include <stdio.h>

/*!
 * \file pro/httpd.h
 * \brief HTTP protocol definitions for daemons.
 */

/*!
 * \addtogroup xgHTTPD
 */
/*@{*/

#define METHOD_GET  1
#define METHOD_POST 2
#define METHOD_HEAD 3

typedef struct _REQUEST REQUEST;
/*!
 * \struct _REQUEST httpd.h pro/httpd.h
 * \brief HTTP request information structure.
 */
struct _REQUEST {
    int req_method;             /*!< \brief Request method. */
    int req_version;            /*!< \brief 11 = HTTP/1.1, 10 = HTTP/1.0, 9 = HTTP/0.9 */
    int req_length;             /*!< \brief Content length */
    char *req_url;              /*!< \brief URI portion of the GET or POST request line */
    char *req_query;            /*!< \brief Argument string. */
    char *req_type;             /*!< \brief Content type. */
    char *req_cookie;           /*!< \brief Cookie. */
    char *req_auth;             /*!< \brief Authorization info. */
    char *req_agent;            /*!< \brief User agent. */
    char **req_qptrs;           /*!< \brief Table of request parameters */
    int req_numqptrs;           /*!< \brief Number of request parameters */
};

extern void NutHttpProcessRequest(FILE * stream);

extern void NutHttpSendHeaderTop(FILE * stream, REQUEST * req, int status, char *title);
extern void NutHttpSendHeaderBot(FILE * stream, char *mime_type, long bytes);
extern void NutHttpSendError(FILE * stream, REQUEST * req, int status);
extern char *NutGetMimeType(char *name);


/*
 * Authorization
 */
typedef struct _AUTHINFO AUTHINFO;

/*!
 * \struct _AUTHINFO httpd.h pro/httpd.h
 * \brief HTTP authorization information structure.
 */
struct _AUTHINFO {
    AUTHINFO *auth_next;        /*!< \brief Link to next AUTHINFO structure */
    CONST char *auth_dirname;   /*!< \brief Pathname of protected directory */
    CONST char *auth_login;     /*!< \brief Login user and password, separated by a colon. */
};

extern int NutHttpAuthValidate(REQUEST * req);
extern int NutRegisterAuth(CONST char *dirname, CONST char *login);

/*
 * CGI
 */
typedef struct _CGIFUNCTION CGIFUNCTION;

/*!
 * \struct _CGIFUNCTION httpd.h pro/httpd.h
 * \brief Registered CGI function.
 */
struct _CGIFUNCTION {
    CGIFUNCTION *cgi_next;      /*!< \brief Link to next CGIFUNCTION structure */
    CONST char *cgi_name;       /*!< \brief Name of this function */
    int (*cgi_func) (FILE *, REQUEST *);        /*!< \brief Pointer to function code. */
};

/*@}*/


__BEGIN_DECLS
/* Function prototypes. */
extern int NutRegisterCgi(char *name, int (*func) (FILE *, REQUEST *));
extern void NutCgiProcessRequest(FILE * stream, REQUEST * req);
extern char *NutHttpURLEncode(char *str);
extern void NutHttpURLDecode(char *str);
extern char *NutHttpGetParameter(REQUEST * req, char *name);
extern int NutHttpGetParameterCount(REQUEST * req);
extern char *NutHttpGetParameterName(REQUEST * req, int index);
extern char *NutHttpGetParameterValue(REQUEST * req, int index);

__END_DECLS
/* */
#endif
