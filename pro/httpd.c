/*
 * Copyright (C) 2008 by egnite GmbH. All rights reserved.
 * Copyright (C) 2001-2006 by egnite Software GmbH. All rights reserved.
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

/*!
 * \file pro/httpd.c
 * \brief HTTP server support routines.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.24  2008/08/11 07:00:35  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.23  2008/07/17 11:36:32  olereinhardt
 * - Moved some functions used in httpd.c as well as in ssi.c into httpd_p.c
 * - Implemeted $QUERY_STRING parameter in for CGIs included by a ssi file
 *
 * Revision 1.22  2008/07/14 13:11:15  haraldkipp
 * Added file length check to avoid loading directories when using PHAT.
 *
 * Revision 1.21  2008/07/10 12:09:39  haraldkipp
 * Wrong mime type was returned for default files within subdirectories.
 * Put duplicate code in a new static function GetMimeEntry.
 *
 * Revision 1.20  2008/07/08 13:27:55  haraldkipp
 * Several HTTP server options are now configurable.
 * Keepalive is now disabled by default to maintain backward compatibility.
 *
 * Revision 1.19  2008/05/21 14:10:19  thiagocorrea
 * Workaround HTTP stall connections. Details in bug id 1968754
 *
 * Revision 1.18  2008/05/16 03:38:27  thiagocorrea
 * Revert httpd memory allocation calls to NutHeapAlloc for consistency and
 * move DestroyRequestInfo to a shared file (reduces code size and remove duplicates
 * from httpd.c and ssi.c)
 *
 * Revision 1.17  2008/04/15 05:13:30  hwmaier
 * Fixed compilation error with avr-gcc 3.4.6
 *
 * Revision 1.16  2008/04/01 10:11:35  haraldkipp
 * Added the new, enhanced httpd API library.
 * Bugs #1839026 and #1839029 fixed.
 *
 * Revision 1.15  2006/11/08 08:52:31  haraldkipp
 * Bugfix, kindly provided by Steve Venroy. Already released request
 * structure was passed to NutHttpSendError().
 *
 * Revision 1.14  2006/10/08 16:48:22  haraldkipp
 * Documentation fixed
 *
 * Revision 1.13  2006/03/16 15:25:38  haraldkipp
 * Changed human readable strings from u_char to char to stop GCC 4 from
 * nagging about signedness.
 *
 * Revision 1.12  2006/01/06 09:19:42  haraldkipp
 * NutHttpURLEncode() no longer encodes everything that isn't alphanumeric.
 * See RFC2396. Thanks to Lloyd Bailey for this update.
 *
 * Revision 1.11  2005/10/24 11:02:28  haraldkipp
 * Integer division hack for ARM without CRT removed.
 *
 * Revision 1.10  2005/08/26 14:12:39  olereinhardt
 * Added NutHttpProcessPostQuery(FILE *stream, REQUEST * req)
 *
 * Revision 1.9  2005/08/05 11:23:11  olereinhardt
 * Added support to register a custom handler for mime types.
 * Added Server side include support and ASP support.
 *
 * Revision 1.8  2005/04/30 13:08:15  chaac
 * Added support for parsing Content-Length field in HTTP requests.
 *
 * Revision 1.7  2005/04/05 17:58:02  haraldkipp
 * Avoid integer division on ARM platform as long as we run without crtlib.
 *
 * Revision 1.6  2004/12/16 10:17:18  haraldkipp
 * Added Mikael Adolfsson's excellent parameter parsing routines.
 *
 * Revision 1.5  2004/07/30 19:45:48  drsung
 * Slightly improved handling if socket was closed by peer.
 *
 * Revision 1.4  2004/03/02 10:09:59  drsung
 * Small bugfix in NutHttpSendError. Thanks to Damian Slee.
 *
 * Revision 1.3  2003/07/20 16:03:27  haraldkipp
 * Saved some RAM by moving string literals to program memory.
 *
 * Revision 1.2  2003/07/17 12:28:21  haraldkipp
 * Memory hole bugfix
 *
 * Revision 1.1.1.1  2003/05/09 14:41:58  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.14  2003/02/04 18:17:32  harald
 * Version 3 released
 *
 * Revision 1.13  2003/01/14 17:04:20  harald
 * Using FAT file system and added types
 *
 * Revision 1.12  2002/10/31 16:32:45  harald
 * Mods by troth for Linux
 *
 * Revision 1.11  2002/09/15 17:08:44  harald
 * Allow different character sets
 *
 * Revision 1.10  2002/06/26 17:29:49  harald
 * First pre-release with 2.4 stack
 *
 * \endverbatim
 */

#include <cfg/arch.h>
#include <cfg/http.h>

#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <sys/heap.h>
#include <sys/version.h>

#include <pro/rfctime.h>
#include <pro/httpd.h>

#include "dencode.h"
#include "httpd_p.h"


/*! \brief Local major HTTP version. */
#ifndef HTTP_MAJOR_VERSION  
#define HTTP_MAJOR_VERSION  1
#endif

/*! \brief Local minor HTTP version. */
#ifndef HTTP_MINOR_VERSION  
#define HTTP_MINOR_VERSION  1
#endif

/*! \brief Maximum number of requests per connection. */
#ifndef HTTP_KEEP_ALIVE_REQ
#define HTTP_KEEP_ALIVE_REQ 0
#endif

/*! \brief Maximum size of an incoming request line. */
#ifndef HTTP_MAX_REQUEST_SIZE
#define HTTP_MAX_REQUEST_SIZE 256
#endif

/*! \brief Chunk size while sending files. */
#ifndef HTTP_FILE_CHUNK_SIZE
#define HTTP_FILE_CHUNK_SIZE 512
#endif

/*!
 * \addtogroup xgHTTPD
 */
/*@{*/

/*!
 * \brief Known mime types. 
 */
MIMETYPES mimeTypes[] = {
    {
    ".txt", "text/plain", NULL}, {
    ".html", "text/html", NULL}, {
    ".shtml", "text/html", NULL}, {    
    ".asp", "text/html", NULL}, {
    ".htm", "text/html", NULL}, {
    ".gif", "image/gif", NULL}, {
    ".jpg", "image/jpeg", NULL}, {
    ".png", "image/png", NULL}, {    
    ".pdf", "application/pdf", NULL}, {
    ".js",  "application/x-javascript", NULL}, {
    ".jar", "application/x-java-archive", NULL}, {
    ".css", "text/css", NULL}, {
    ".xml", "text/xml", NULL}, {
    NULL, NULL, NULL}
};

static uint32_t http_optflags;

/*!
 * \brief Send top lines of a standard HTML header.
 *
 * Sends HTTP and Server version lines.
 *
 * \param stream Stream of the socket connection, previously opened for 
 *               binary read and write.
 * \param req    The associated client request.
 * \param status Response status, error code or 200, if no error occurred.
 * \param title  Error text, or OK, if no error occurred.
 */
void NutHttpSendHeaderTop(FILE * stream, REQUEST * req, int status, char *title)
{
    static prog_char fmt_P[] = "HTTP/%d.%d %d %s\r\nServer: Ethernut %s\r\n";

    fprintf_P(stream, fmt_P, HTTP_MAJOR_VERSION, HTTP_MINOR_VERSION, status, title, NutVersionString());
#if !defined(HTTPD_EXCLUDE_DATE)
    if (http_optflags & HTTP_OF_USE_HOST_TIME) {
        time_t now = time(NULL);
        fprintf(stream, "Date: %s GMT\r\n", Rfc1123TimeString(gmtime(&now)));
    }
#endif
}

/*!
 * \brief Send bottom lines of a standard HTML header.
 *
 * Sends Content-Type and Content-Length.
 *
 * \deprecated Use NutHttpSendHeaderBottom().
 *
 * \param stream    Stream of the socket connection, previously opened 
 *                  for  binary read and write.
 * \param mime_type Points to a string that specifies the content type. 
 *                  Examples are "text/html", "image/png", 
 *                  "image/gif", "video/mpeg" or "text/css".
 *                  A null pointer is ignored.
 * \param bytes     Content length of the data following this
 *                  header. Ignored, if negative.
 */
void NutHttpSendHeaderBot(FILE * stream, char *mime_type, long bytes)
{
	NutHttpSendHeaderBottom( stream, 0, mime_type, bytes );
}

/*!
 * \brief Send bottom lines of a standard HTML header.
 *
 * Sends Content-Type, Content-Lenght and Connection lines.
 *
 * \param stream    Stream of the socket connection, previously opened 
 *                  for  binary read and write.
 * \param mime_type Points to a string that specifies the content type. 
 *                  Examples are "text/html", "image/png", 
 *                  "image/gif", "video/mpeg" or "text/css".
 *                  A null pointer is ignored.
 * \param bytes     Content length of the data following this
 *                  header. Ignored, if negative.
 */
void NutHttpSendHeaderBottom(FILE * stream, REQUEST * req, char *mime_type, long bytes)
{
    static prog_char typ_fmt_P[] = "Content-Type: %s\r\n";
    static prog_char len_fmt_P[] = "Content-Length: %ld\r\n";
    static prog_char con_str_P[] = "Connection: ";
    static prog_char ccl_str_P[] = "close\r\n\r\n";

    if (mime_type)
        fprintf_P(stream, typ_fmt_P, mime_type);
    if (bytes >= 0)
        fprintf_P(stream, len_fmt_P, bytes);
    fputs_P(con_str_P, stream);
#if HTTP_KEEP_ALIVE_REQ
    if ( req && req->req_connection == HTTP_CONN_KEEP_ALIVE) {
        static prog_char cka_str_P[] = "Keep-Alive\r\n\r\n";
        fputs_P(cka_str_P, stream);
    }
    else {
        fputs_P(ccl_str_P, stream);
    }
#else
    fputs_P(ccl_str_P, stream);
#endif
}

/*!
 * \brief Send a HTTP error response.
 *
 * A canned error file is used.
 *
 * \param stream Stream of the socket connection, previously opened for 
 *               binary read and write.
 * \param req    Contains the HTTP request.
 * \param status Error code to be returned.
 */
void NutHttpSendError(FILE * stream, REQUEST * req, int status)
{
    static prog_char err_fmt_P[] = "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD><BODY>%d %s</BODY></HTML>\r\n";
    static prog_char auth_fmt_P[] = "WWW-Authenticate: Basic realm=\"%s\"\r\n";
    char *title;

    switch (status) {
    case 304:
        title = "Not Modified";
        break;
    case 400:
        title = "Bad Request";
        break;
    case 401:
        title = "Unauthorized";
        break;
    case 404:
        title = "Not Found";
        break;
    case 500:
        title = "Internal Error";
        break;
    case 501:
        title = "Not Implemented";
        break;
    default:
        title = "Error";
        break;
    }
#if HTTP_KEEP_ALIVE_REQ
    if (status >= 400) {
        req->req_connection = HTTP_CONN_CLOSE;
    }
#endif
    NutHttpSendHeaderTop(stream, req, status, title);
    if (status == 401) {
        char *cp = 0;
        char *realm = req->req_url;

        if ((cp = strrchr(realm, '/')) != NULL)
            *cp = 0;
        else
            realm = ".";
        fprintf_P(stream, auth_fmt_P, realm);
        if (cp)
            *cp = '/';
    }
    NutHttpSendHeaderBottom(stream, req, "text/html", -1);
    fprintf_P(stream, err_fmt_P, status, title, status, title);
}

static MIMETYPES *GetMimeEntry(char *name)
{
    MIMETYPES *rc;
    int fl;

    if (name == NULL || (fl = strlen(name)) == 0) {
        return &mimeTypes[1];
    }
    for (rc = mimeTypes; rc->mtyp_ext; rc++) {
        if (strcasecmp(&(name[fl - strlen(rc->mtyp_ext)]), rc->mtyp_ext) == 0) {
            return rc;
        }
    }
    return &mimeTypes[0];
}

/*!
 * \brief Return the mime type description of a specified file name.
 *
 * The mime type returned is based on the file extension.
 *
 * \param name Name of the file.
 *
 * \return A pointer to a static string, containing the
 *         associated mime type description. If the extension
 *         is not registered, "text/plain; charset=iso-8859-1"
 *         is returned. If the filename is empty, then
 *         "text/html; charset=iso-8859-1" is returned.
 */
char *NutGetMimeType(char *name)
{
    return GetMimeEntry(name)->mtyp_type;
}

/*!
 * \brief Return the mime type handler of a specified file name.
 *
 * This is the function that handles / sends a specific file type to the 
 * client. Specially used for server side includes (shtml files)
 *
 * \param name Name of the file.
 *
 * \return A pointer to a function of the type void (u_char * filename)
 *         If the extension is not registered, the handler for 
 *         "text/plain; charset=iso-8859-1" is returned. 
 *         If the filename is empty, then the handler for 
 *         "text/html; charset=iso-8859-1" is returned.
 */

void *NutGetMimeHandler(char *name)
{
    return GetMimeEntry(name)->mtyp_handler;
}

/*!
 * \brief URLDecodes a string
 *
 * Takes a url-encoded string and decodes it.
 *
 * \param str String to decode. This is overwritten with
 * the decoded string
 * 
 * \warning To save RAM, the str parameter will be 
 * 	    overwritten with the encoded string.
 */
void NutHttpURLDecode(char *str)
{
    register char *ptr1, *ptr2, ch;
    char hexstr[3] = { 0, 0, 0 };
    for (ptr1 = ptr2 = str; *ptr1; ptr1++) {
        if (*ptr1 == '+')
            *ptr2++ = ' ';
        else if (*ptr1 == '%') {
            hexstr[0] = ptr1[1];
            hexstr[1] = ptr1[2];
            ch = strtol(hexstr, 0, 16);
            *ptr2++ = ch;
            ptr1 += 2;
        } else
            *ptr2++ = *ptr1;
    }
    *ptr2 = 0;
}

/*!
 * \brief Parses the QueryString
 *
 * Reads the QueryString from a request, and parses it into
 * name/value table. To save RAM, this method overwrites the
 * contents of req_query, and creates a table of pointers
 * into the req_query buffer.
 *
 * \param req Request object to parse
 */
void NutHttpProcessQueryString(REQUEST * req)
{
    register int i;
    register char *ptr;

    if (!req->req_query)
        return;

    req->req_numqptrs = 1;
    for (ptr = req->req_query; *ptr; ptr++)
        if (*ptr == '&')
            req->req_numqptrs++;

    req->req_qptrs = (char **) NutHeapAlloc(sizeof(char *) * (req->req_numqptrs * 2));
    if (req->req_qptrs == NULL) {
        /* Out of memory */
        req->req_numqptrs = 0;
        return;
    }
    req->req_qptrs[0] = req->req_query;
    req->req_qptrs[1] = NULL;
    for (ptr = req->req_query, i = 2; *ptr; ptr++) {
        if (*ptr == '&') {
            req->req_qptrs[i] = ptr + 1;
            req->req_qptrs[i + 1] = NULL;
            *ptr = 0;
            i += 2;
        }
    }

    for (i = 0; i < req->req_numqptrs; i++) {
        for (ptr = req->req_qptrs[i * 2]; *ptr; ptr++) {
            if (*ptr == '=') {
                req->req_qptrs[i * 2 + 1] = ptr + 1;
                *ptr = 0;
                NutHttpURLDecode(req->req_qptrs[i * 2 + 1]);
                break;
            }
        }
        NutHttpURLDecode(req->req_qptrs[i * 2]);
    }
}

static void NutHttpProcessFileRequest(FILE * stream, REQUEST * req)
{
    int fd;
    int n;
    char *data;
    long file_len;
    void (*handler)(FILE *stream, int fd, int file_len, char *http_root, REQUEST *req);
    char *mime_type;
    char *filename = NULL;
    char *modstr = NULL;
    
    /*
     * Validate authorization.
     */
    if (NutHttpAuthValidate(req)) {
        NutHttpSendError(stream, req, 401);
        return;
    }

    /*
     * Process CGI.
     */
    if (NutCgiCheckRequest(stream, req)) {
        return;
    }    

    for (n = 0, fd = -1; default_files[n]; n++) {
        filename = CreateFilePath(req->req_url, default_files[n]);
        if (filename == NULL) {
            NutHttpSendError(stream, req, 500);
            return;
        }
        /*
         * Note, that simple file systems may not provide stat() or access(),
         * thus trying to open the file is the only way to check for existence.
         * Another problem is, that PHAT allows to open directories. We use
         * the file length to ensure, that we got a normal file.
         */
        if ((fd = _open(filename, _O_BINARY | _O_RDONLY)) != -1) {
            if (_filelength(fd)) {
                break;
            }
            _close(fd);
        }
        NutHeapFree(filename);
    }
    if (fd == -1) {
        NutHttpSendError(stream, req, 404);
        return;
    }

    /* Check for mime type and handler. */
    mime_type = NutGetMimeType(filename);
    handler = NutGetMimeHandler(filename);

#if !defined(HTTPD_EXCLUDE_DATE)
    /*
     * Optionally process modification time.
     */
    if (handler == NULL && (http_optflags & HTTP_OF_USE_FILE_TIME)) {
        struct stat s;
        time_t ftime;
        char *time_str;

        if (stat(filename, &s) == 0) {
            ftime = s.st_mtime;
        }
        else {
            /* Use compile time if stat not available. */
            ftime = RfcTimeParse("Fri " __DATE__ " " __TIME__);
        }
            
        /* Check if-modified-since condition. */
        if (req->req_ims && s.st_mtime <= req->req_ims) {
            _close(fd);
            NutHttpSendError(stream, req, 304);
            NutHeapFree(filename);
            return;
        }

        /* Save static buffer contents. */
        time_str = Rfc1123TimeString(gmtime(&ftime));
        if ((modstr = NutHeapAlloc(strlen(time_str) + 1)) != NULL) {
            strcpy(modstr, time_str);
        }
    }
#endif /* HTTPD_EXCLUDE_DATE */

    /* Filename no longer used. */
    NutHeapFree(filename);

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    if (modstr) {
        fprintf(stream, "Last-Modified: %s GMT\r\n", modstr);
        NutHeapFree(modstr);
    }

    file_len = _filelength(fd);
    /* Use mime handler, if one has been registered. */
    if (handler) {
        NutHttpSendHeaderBottom(stream, req, mime_type, -1);
        handler(stream, fd, file_len, http_root, req);
    } 
    /* Use default transfer, if no registered mime handler is available. */
    else {
        NutHttpSendHeaderBottom(stream, req, mime_type, file_len);
        if (req->req_method != METHOD_HEAD) {
            size_t size = HTTP_FILE_CHUNK_SIZE;

            if ((data = NutHeapAlloc(size)) != NULL) {
                while (file_len) {
                    if (file_len < HTTP_FILE_CHUNK_SIZE)
                        size = (size_t) file_len;
    
                    n = _read(fd, data, size);
                    if (n <= 0) {
                        /* We can't do much here, the header is out already. */
                        break;
                    }
                    if (fwrite(data, 1, n, stream) == 0) {
                        break;
                    }
                    file_len -= (long) n;
                }
                NutHeapFree(data);
            }
        }
    }
    _close(fd);
}

/*!
 *
 */
static char *NextWord(char *str)
{
    while (*str && *str != ' ' && *str != '\t')
        str++;
    if (*str)
        *str++ = 0;
    while (*str == ' ' || *str == '\t')
        str++;
    return str;
}

/*!
 * \brief Create a new request info structure.
 *
 * \return Pointer to an allocated structure or NULL if out of memory.
 */
static REQUEST *CreateRequestInfo(void)
{
    REQUEST *req;

    if ((req = NutHeapAllocClear(sizeof(REQUEST))) != NULL) {
        req->req_version = HTTP_MAJOR_VERSION * 10 + HTTP_MINOR_VERSION;
    }
    return req;
}

/*!
 * \brief Register the HTTP server's root directory.
 *
 * Only one root directory is supported. Subsequent calls will
 * override previous settings.
 *
 * \param path Pathname of the root directory. Must include the
 *             device name followed by a colon followed by a
 *             directory path followed by a trailing slash.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutRegisterHttpRoot(char *path)
{
    int len;

    if (http_root)
        NutHeapFree(http_root);
    if (path && (len = strlen(path)) != 0) {
        if ((http_root = NutHeapAlloc(len + 1)) != NULL)
            strcpy(http_root, path);
        else
            return -1;
    } else
        http_root = NULL;

    return 0;
}

/*!
 * \brief Set HTTP option flags.
 *
 * \param flags Option flags to set. Any of the following may be or'ed:
 * - HTTP_OF_USE_HOST_TIME Date header will be included in response.
 * - HTTP_OF_USE_FILE_TIME Handle file modification time.
 *
 */
void NutHttpSetOptionFlags(uint32_t flags)
{
    http_optflags = flags;
}

/*!
 * \brief Retrieve HTTP option flags.
 *
 * \return Option flags.
 */
uint32_t NutHttpGetOptionFlags(void)
{
    return http_optflags;
}

/*!
 * \brief Allocate a buffer for a header field value.
 *
 * \param hfvp Points to the character pointer variable that will receive 
 *             the pointer to the header field value. If the variable does
 *             not contain a NULL pointer upon entry, the routine will
 *             return immediately and will not extract any value. If it
 *             contains a NULL pointer on entry, the routine will allocate
 *             heap memory to store a copy of the extracted value. In this
 *             case the caller is responsible to release the allocation.
 * \param str  Points into a request line, right after the colon. Leading
 *             spaces will be skipped before creating a copy.
 *
 * \return -1 if out of memory, otherwise 0.
 */
static int HeaderFieldValue(char **hfvp, CONST char *str)
{
    /* Do not override existing values. */
    if (*hfvp == NULL) {
        /* Skip spaces. */
        while (*str == ' ' || *str == '\t')
            str++;
        /* Allocate a string copy. */
        if ((*hfvp = NutHeapAlloc(strlen(str) + 1)) == NULL)
            return -1;
        strcpy(*hfvp, str);
    }
    return 0;
}

/*!
 * \brief Process the next HTTP request.
 *
 * Waits for the next HTTP request on an established connection
 * and processes it.
 *
 * \param stream Stream of the socket connection, previously opened for 
 *               binary read and write.
 */
void NutHttpProcessRequest(FILE * stream)
{
    REQUEST *req = NULL;
    char *method = NULL;
    char *path;
    char *line;
    char *protocol;
    char *cp;
#if HTTP_KEEP_ALIVE_REQ
    int keep_alive_max = HTTP_KEEP_ALIVE_REQ;
#endif

    for(;;) {
        /* Release resources used on the previous connect. */
        DestroyRequestInfo(req);
        if ((req = CreateRequestInfo()) == NULL)
            break;
        if (method)
            NutHeapFree(method);

        /* The first line contains method, path and protocol. */
        if ((method = NutHeapAlloc(HTTP_MAX_REQUEST_SIZE)) == NULL) {
            break;
        }
        if (fgets(method, HTTP_MAX_REQUEST_SIZE, stream) == NULL) {
            break;
        }
        if ((cp = strchr(method, '\r')) != NULL)
            *cp = 0;
        if ((cp = strchr(method, '\n')) != NULL)
            *cp = 0;

        /*
        * Parse remaining request header lines.
        */
        if ((line = NutHeapAlloc(HTTP_MAX_REQUEST_SIZE)) == NULL) {
            break;
        }
        for (;;) {
            /* Read a line and chop off CR/LF. */
            if (fgets(line, HTTP_MAX_REQUEST_SIZE, stream) == NULL)
                break;
            if ((cp = strchr(line, '\r')) != 0)
                *cp = 0;
            if ((cp = strchr(line, '\n')) != 0)
                *cp = 0;
            if (*line == 0)
                /* Empty line marks the end of the request header. */
                break;
            if (strncasecmp(line, "Authorization:", 14) == 0) {
                if (HeaderFieldValue(&req->req_auth, line + 14))
                    break;
            } else if (strncasecmp(line, "Content-Length:", 15) == 0) {
                req->req_length = atol(line + 15);
            } else if (strncasecmp(line, "Content-Type:", 13) == 0) {
                if (HeaderFieldValue(&req->req_type, line + 13))
                    break;
            } else if (strncasecmp(line, "Cookie:", 7) == 0) {
                if (HeaderFieldValue(&req->req_cookie, line + 7))
                    break;
            } else if (strncasecmp(line, "User-Agent:", 11) == 0) {
                if (HeaderFieldValue(&req->req_agent, line + 11))
                    break;
#if !defined(HTTPD_EXCLUDE_DATE)
            } else if (strncasecmp(line, "If-Modified-Since:", 18) == 0) {
                req->req_ims = RfcTimeParse(line + 18);
#endif
            } else if (strncasecmp(line, "Referer:", 8) == 0) {
                if (HeaderFieldValue(&req->req_referer, line + 8))
                    break;
            } else if (strncasecmp(line, "Host:", 5) == 0) {
                if (HeaderFieldValue(&req->req_host, line + 5))
                    break;
            }
#if HTTP_KEEP_ALIVE_REQ
            else if (strncasecmp(line, "Connection:", 11) == 0) {
                if (strncasecmp(line + 12, "close", 5) == 0) {
                    req->req_connection = HTTP_CONN_CLOSE;
                }
                else if (strncasecmp(line + 12, "Keep-Alive", 10) == 0) {
                    req->req_connection = HTTP_CONN_KEEP_ALIVE;
                }
            }
#endif
        }
        if (line) {
            NutHeapFree(line);
        }
        path = NextWord(method);
        protocol = NextWord(path);
        NextWord(protocol);

        /* Determine the request method. */
        if (strcasecmp(method, "GET") == 0)
            req->req_method = METHOD_GET;
        else if (strcasecmp(method, "HEAD") == 0)
            req->req_method = METHOD_HEAD;
        else if (strcasecmp(method, "POST") == 0)
            req->req_method = METHOD_POST;
        else {
            NutHttpSendError(stream, req, 501);
            break;
        }
        if (*path == 0 || *protocol == 0) {
            NutHttpSendError(stream, req, 400);
            break;
        }

        /* Determine the client's HTTP version. */
        if (strcasecmp(protocol, "HTTP/1.0") == 0) {
            req->req_version = 10;
#if HTTP_KEEP_ALIVE_REQ
            if (req->req_connection != HTTP_CONN_KEEP_ALIVE) {
                req->req_connection = HTTP_CONN_CLOSE;
            }
#endif
        }
#if HTTP_KEEP_ALIVE_REQ
        else if (req->req_connection != HTTP_CONN_CLOSE) {
            req->req_connection = HTTP_CONN_KEEP_ALIVE;
        }

        /* Limit the number of requests per connection. */
        if (keep_alive_max > 0) {
            keep_alive_max--;
        }
        else {
            req->req_connection = HTTP_CONN_CLOSE;
        }
#else
        req->req_connection = HTTP_CONN_CLOSE;
#endif

        if ((cp = strchr(path, '?')) != 0) {
            *cp++ = 0;
            if ((req->req_query = NutHeapAlloc(strlen(cp) + 1)) == NULL) {
                break;
            }
            strcpy(req->req_query, cp);

            NutHttpProcessQueryString(req);
        }

        if ((req->req_url = NutHeapAlloc(strlen(path) + 1)) == NULL) {
            break;
        }
        strcpy(req->req_url, path);

        if (NutDecodePath(req->req_url) == 0) {
            NutHttpSendError(stream, req, 400);
        } else {
            NutHttpProcessFileRequest(stream, req);
        }
        fflush(stream);

        if (req->req_connection == HTTP_CONN_CLOSE) {
            break;
        }
    }
    DestroyRequestInfo(req);
    if (method)
        NutHeapFree(method);
}

/*@}*/
