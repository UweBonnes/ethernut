#ifndef _PRO_HTTPD_H_
#define _PRO_HTTPD_H_

#include <stdio.h>

/*!
 * \file pro/httpd.h
 * \brief HTTP protocol definitions for daemons.
 */

#ifdef __cplusplus
extern "C" {
#endif

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
    int req_method;     /*!< \brief Request method. */
    int req_version;    /*!< \brief 11 = HTTP/1.1, 10 = HTTP/1.0, 9 = HTTP/0.9 */
    int req_length;     /*!< \brief Content length */
    char *req_url;      /*!< \brief URI portion of the GET or POST request line */
    char *req_query;    /*!< \brief Argument string. */
    char *req_type;     /*!< \brief Content type. */
    char *req_cookie;   /*!< \brief Cookie. */
    char *req_auth;     /*!< \brief Authorization info. */
    char *req_agent;    /*!< \brief User agent. */
};

extern void NutHttpProcessRequest(FILE *stream);

extern void NutHttpSendHeaderTop(FILE *stream, REQUEST *req, int status, char* title);
extern void NutHttpSendHeaderBot(FILE *stream, char* mime_type, long bytes);
extern void NutHttpSendError(FILE *stream, REQUEST *req, int status);
extern char *NutGetMimeType(char* name);


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

extern int NutHttpAuthValidate(REQUEST *req);
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
    int (*cgi_func)(FILE *, REQUEST *);    /*!< \brief Pointer to function code. */
};

/*@}*/


extern int NutRegisterCgi(char *name, int (*func)(FILE *, REQUEST *));
extern void NutCgiProcessRequest(FILE *stream, REQUEST *req);

#ifdef __cplusplus
}
#endif

#endif
