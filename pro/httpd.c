/*
 * Copyright (C) 2001-2005 by egnite Software GmbH. All rights reserved.
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
 */

#include <cfg/arch.h>

#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>

#include <sys/heap.h>
#include <sys/version.h>

#include "dencode.h"

#include <pro/httpd.h>

/*!
 * \addtogroup xgHTTPD
 */
/*@{*/

static struct {
    char *ext;
    char *type;
} mimeTypes[] = {
    {
    ".txt", "text/plain"}, {
    ".html", "text/html"}, {
    ".htm", "text/html"}, {
    ".gif", "image/gif"}, {
    ".jpg", "image/jpeg"}, {
    ".pdf", "application/pdf"}, {
    ".js", "application/x-javascript"}, {
    ".css", "text/css"}, {
    ".xml", "text/xml"}
};

static char *http_root;

/*!
 * \brief Send top lines of a standard HTML header.
 *
 * Sends HTTP and Server version lines.
 *
 * \param stream Stream of the socket connection, previously opened for 
 *               binary read and write.
 * \param req    The associated client request.
 * \param status Response status, error code or 200, if no error occured.
 * \param title  Error text, or OK, if no error occured.
 */
void NutHttpSendHeaderTop(FILE * stream, REQUEST * req, int status, char *title)
{
    static prog_char fmt_P[] = "HTTP/%d.%d %d %s\r\nServer: Ethernut %s\r\n";

    fprintf_P(stream, fmt_P, req->req_version / 10, req->req_version % 10, status, title, NutVersionString());
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
void NutHttpSendHeaderBot(FILE * stream, char *mime_type, long bytes)
{
    static prog_char typ_fmt_P[] = "Content-Type: %s\r\n";
    static prog_char len_fmt_P[] = "Content-Length: %ld\r\n";
    static prog_char ccl_str_P[] = "Connection: close\r\n\r\n";

    if (mime_type)
        fprintf_P(stream, typ_fmt_P, mime_type);
    if (bytes >= 0)
        fprintf_P(stream, len_fmt_P, bytes);
    fputs_P(ccl_str_P, stream);
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
    static prog_char err_fmt_P[] = "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD><BODY>%d %s</BODY></HTML>";
    static prog_char auth_fmt_P[] = "WWW-Authenticate: Basic realm=\"%s\"\r\n";
    char *title;

    switch (status) {
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

    NutHttpSendHeaderTop(stream, req, status, title);
    if (status == 401) {
        char *cp = 0;
        char *realm = req->req_url;

        if ((cp = strrchr(realm, '/')) != 0)
            *cp = 0;
        else
            realm = ".";
        fprintf_P(stream, auth_fmt_P, realm);
        if (cp)
            *cp = '/';
    }
    NutHttpSendHeaderBot(stream, "text/html", -1);
    fprintf_P(stream, err_fmt_P, status, title, status, title);
}

/*!
 * \brief Return the mime type description of a specified file name.
 *
 * The mime type returned is based on the file extension.
 *
 * \todo Function to register additional mime types. Currently only
 *       .txt, .html, .gif and .jpg are supported.
 *
 * \param name Name of the file.
 *
 * \return A pointer to a static string, containing the
 *         associated mime type description. If the extension
 *         is not registered, "text/plain; charset=iso-8859-1"
 *         is returned. If the filename is empty, then
 *         "text/hatml; charset=iso-8859-1" is returned.
 */
char *NutGetMimeType(char *name)
{
    size_t i;
    int fl;

    if (name == 0 || (fl = strlen(name)) == 0)
        return mimeTypes[1].type;
    for (i = 0; i < sizeof(mimeTypes) / sizeof(*mimeTypes); i++)
        if (strcasecmp(&(name[fl - strlen(mimeTypes[i].ext)]), mimeTypes[i].ext) == 0)
            return mimeTypes[i].type;
    return mimeTypes[0].type;
}

/*!
 * \brief URLEncodes a string
 *
 * \param str String to encode
 *
 * \return A new allocated encoded string, or NULL if
 * 	   str is null, or if there's not enough RAM
 *         for the new string.
 *
 * \note This is done in the simplies way, i.e. we
 * 	 encode everything that isn't alphanumeric.
 */
#ifndef ARM_GCC_NOLIBC
static char *hexdigits = "0123456789ABCDEF";

char *NutHttpURLEncode(char *str)
{
    register char *ptr1, *ptr2;
    char *encstring;
    int numEncs = 0;

    if (!str)
        return 0;

    /* Calculate how many characters we need to encode */
    for (ptr1 = str; *ptr1; ptr1++) {
        if (!isalnum(*ptr1) || *ptr1 == ' ')
            numEncs++;
    }
    /* Now we can calculate the encoded string length */
    encstring = (char *) NutHeapAlloc(strlen(str) + 2 * numEncs + 1);

    /* Encode the string. ptr1 refers to the original string,
     * and ptr2 refers to the new string. */
    ptr2 = encstring;
    for (ptr1 = str; *ptr1; ptr1++) {
        if (*ptr1 == ' ')
            *ptr2++ = '+';
        else if (isalnum(*ptr1))
            *ptr2++ = *ptr1;
        else {
            *ptr2++ = '%';
            *ptr2++ = hexdigits[(*ptr1 >> 4)];
            *ptr2++ = hexdigits[*ptr1 & 0x0F];
        }
    }
    *ptr2++ = 0;
    return encstring;
}
#endif

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
#ifndef ARM_GCC_NOLIBC
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
#endif

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
#ifndef ARM_GCC_NOLIBC
static void NutHttpProcessQueryString(REQUEST * req)
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
    if (!req->req_qptrs) {
        /* Out of memory */
        req->req_numqptrs = 0;
        return;
    }
    req->req_qptrs[0] = req->req_query;
    req->req_qptrs[1] = 0;
    for (ptr = req->req_query, i = 2; *ptr; ptr++) {
        if (*ptr == '&') {
            req->req_qptrs[i] = ptr + 1;
            req->req_qptrs[i + 1] = 0;
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
#endif

/*!
 * \brief Gets a request parameter value by name
 * 
 * \param req Request object
 * \param name Name of parameter
 *
 * \return Pointer to the parameter value. 
 */
char *NutHttpGetParameter(REQUEST * req, char *name)
{
    int i;
    for (i = 0; i < req->req_numqptrs; i++)
        if (strcmp(req->req_qptrs[i * 2], name) == 0)
            return req->req_qptrs[i * 2 + 1];
    return NULL;
}

/*!
 * \brief Gets the number of request parameters
 *
 * \param req Request object
 *
 * \return The number of request parameters
 */
int NutHttpGetParameterCount(REQUEST * req)
{
    return req->req_numqptrs;
}

/*!
 * \brief Gets the name of a request parameter
 *
 * \param req Request object
 * \param index Index of the requested parameter.
 *
 * \return Pointer to the parameter name at the given index,
 *         or NULL if index is out of range.
 */
char *NutHttpGetParameterName(REQUEST * req, int index)
{
    if (index < 0 || index >= NutHttpGetParameterCount(req))
        return NULL;
    return req->req_qptrs[index * 2];
}

/*!
 * \brief Get the value of a request paramter
 *
 * \param req Request object
 * \param index Index to the requested parameter.
 *
 * \return Pointer to the paramter value at the given index,
 *         or NULL if index is out of range.
 */
char *NutHttpGetParameterValue(REQUEST * req, int index)
{
    if (index < 0 || index >= NutHttpGetParameterCount(req))
        return NULL;
    return req->req_qptrs[index * 2 + 1];
}

static void NutHttpProcessFileRequest(FILE * stream, REQUEST * req)
{
    int fd;
    int n;
    char *data;
    int size;
    long file_len;
    char *filename = NULL;

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
    if (strncasecmp(req->req_url, "cgi-bin/", 8) == 0) {
        NutCgiProcessRequest(stream, req);
        return;
    }

    /*
     * Process file.
     */
    if (http_root) {
        filename = NutHeapAlloc(strlen(http_root) + strlen(req->req_url) + 1);
        strcpy(filename, http_root);
    } else {
        filename = NutHeapAlloc(strlen(req->req_url) + 6);
        strcpy(filename, "UROM:");
    }
    strcat(filename, req->req_url);

    fd = _open(filename, _O_BINARY | _O_RDONLY);
    NutHeapFree(filename);
    if (fd == -1) {
        u_char *index;
        u_short urll;


        urll = strlen(req->req_url);
        if ((index = NutHeapAllocClear(urll + 12)) == 0) {
            NutHttpSendError(stream, req, 500);
            return;
        }
        if (urll)
            strcpy(index, req->req_url);
        if (urll && index[urll - 1] != '/')
            strcat(index, "/");
        strcat(index, "index.html");

        if (http_root) {
            filename = NutHeapAlloc(strlen(http_root) + strlen(index) + 1);
            strcpy(filename, http_root);
        } else {
            filename = NutHeapAlloc(strlen(index) + 6);
            strcpy(filename, "UROM:");
        }
        strcat(filename, index);

        NutHeapFree(index);

        fd = _open(filename, _O_BINARY | _O_RDONLY);
        NutHeapFree(filename);
        if (fd == -1) {
            NutHttpSendError(stream, req, 404);
            return;
        }
    }
    file_len = _filelength(fd);
    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, NutGetMimeType(req->req_url), file_len);
    if (req->req_method != METHOD_HEAD) {
        size = 512;
        if ((data = NutHeapAlloc(size)) != 0) {
            while (file_len) {
                if (file_len < 512L)
                    size = (int) file_len;

                n = _read(fd, data, size);
                if (fwrite(data, 1, n, stream) == 0)
                    break;
                file_len -= (long) n;
            }
            NutHeapFree(data);
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
 *
 */
static REQUEST *CreateRequestInfo(void)
{
    return NutHeapAllocClear(sizeof(REQUEST));
}

/*!
 *
 */
static void DestroyRequestInfo(REQUEST * req)
{
    if (req->req_url)
        NutHeapFree(req->req_url);
    if (req->req_query)
        NutHeapFree(req->req_query);
    if (req->req_type)
        NutHeapFree(req->req_type);
    if (req->req_cookie)
        NutHeapFree(req->req_cookie);
    if (req->req_auth)
        NutHeapFree(req->req_auth);
    if (req->req_agent)
        NutHeapFree(req->req_agent);
    if (req->req_qptrs)
        NutHeapFree(req->req_qptrs);
    NutHeapFree(req);
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
        if ((http_root = NutHeapAlloc(len + 1)) != 0)
            strcpy(http_root, path);
    } else
        http_root = 0;

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
    REQUEST *req;
    char *method;
    char *path;
    char *line;
    char *protocol;
    char *cp;

    /*
     * Parse the first line of the request.
     * <method> <path> <protocol>
     */
    if ((req = CreateRequestInfo()) == 0)
        return;
    if ((method = NutHeapAlloc(256)) == 0) {
        DestroyRequestInfo(req);
        return;
    }
    fgets(method, 256, stream);
    if ((cp = strchr(method, '\r')) != 0)
        *cp = 0;
    if ((cp = strchr(method, '\n')) != 0)
        *cp = 0;

    /*
     * Parse remaining request headers.
     */
    if ((line = NutHeapAlloc(256)) == 0) {
        NutHeapFree(method);
        DestroyRequestInfo(req);
        return;
    }
    for (;;) {
        if (fgets(line, 256, stream) == 0)
            break;
        if ((cp = strchr(line, '\r')) != 0)
            *cp = 0;
        if ((cp = strchr(line, '\n')) != 0)
            *cp = 0;
        if (*line == 0)
            break;
        if (strncasecmp(line, "Authorization:", 14) == 0) {
            if (req->req_auth == 0) {
                cp = &line[14];
                while (*cp == ' ' || *cp == '\t')
                    cp++;
                if ((req->req_auth = NutHeapAlloc(strlen(cp) + 1)) == 0)
                    break;
                strcpy(req->req_auth, cp);
            }
        } else if (strncasecmp(line, "Content-Length:", 15) == 0)
            req->req_length = 0 /*atoi(&line[15]) */ ;
        else if (strncasecmp(line, "Content-Type:", 13) == 0) {
            if (req->req_type == 0) {
                cp = &line[13];
                while (*cp == ' ' || *cp == '\t')
                    cp++;
                if ((req->req_type = NutHeapAlloc(strlen(cp) + 1)) == 0)
                    break;
                strcpy(req->req_type, cp);
            }
        } else if (strncasecmp(line, "Cookie:", 7) == 0) {
            if (req->req_cookie == 0) {
                cp = &line[7];
                while (*cp == ' ' || *cp == '\t')
                    cp++;
                if ((req->req_cookie = NutHeapAlloc(strlen(cp) + 1)) == 0)
                    break;
                strcpy(req->req_cookie, cp);
            }
        } else if (strncasecmp(line, "User-Agent:", 11) == 0) {
            if (req->req_agent == 0) {
                cp = &line[11];
                while (*cp == ' ' || *cp == '\t')
                    cp++;
                if ((req->req_agent = NutHeapAlloc(strlen(cp) + 1)) == 0)
                    break;
                strcpy(req->req_agent, cp);
            }
        }
    }
    NutHeapFree(line);

    path = NextWord(method);
    protocol = NextWord(path);
    NextWord(protocol);

    if (strcasecmp(method, "GET") == 0)
        req->req_method = METHOD_GET;
    else if (strcasecmp(method, "HEAD") == 0)
        req->req_method = METHOD_HEAD;
    else if (strcasecmp(method, "POST") == 0)
        req->req_method = METHOD_POST;
    else {
        NutHeapFree(method);
        DestroyRequestInfo(req);
        NutHttpSendError(stream, req, 501);
        return;
    }
    if (*path == 0 || *protocol == 0) {
        NutHeapFree(method);
        DestroyRequestInfo(req);
        NutHttpSendError(stream, req, 400);
        return;
    }

    req->req_version = 10;
    if (strcasecmp(protocol, "HTTP/0.9") == 0)
        req->req_version = 9;

#ifndef ARM_GCC_NOLIBC
    if ((cp = strchr(path, '?')) != 0) {
        *cp++ = 0;
        if ((req->req_query = NutHeapAlloc(strlen(cp) + 1)) == 0) {
            NutHeapFree(method);
            DestroyRequestInfo(req);
            return;
        }
        strcpy(req->req_query, cp);

        NutHttpProcessQueryString(req);
    }
#endif
    if ((req->req_url = NutHeapAlloc(strlen(path) + 1)) == 0) {
        NutHeapFree(method);
        DestroyRequestInfo(req);
        return;
    }
    strcpy(req->req_url, path);

    NutHeapFree(method);

    if (NutDecodePath(req->req_url) == 0)
        NutHttpSendError(stream, req, 400);
    else
        NutHttpProcessFileRequest(stream, req);

    DestroyRequestInfo(req);
}

/*@}*/
