/*
 * Copyright (C) 2001-2004 by Ole Reinhardt <ole.reinhardt@embedded-it.de>. 
 * All rights reserved.
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
 * Revision 1.1  2005/08/05 11:22:14  olereinhardt
 * Added Server side include support. Initial checkin
 *
 */  

/*!
 * \addtogroup xgHTTPD
 */
/*@{*/

#include <cfg/arch.h>

#include <io.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>


#include <sys/heap.h>
#include <sys/version.h>

#include <pro/httpd.h>
#include <pro/ssi.h>
#include "dencode.h"

#define BUFSIZE 512

#define MIN(a,b) (a<b?a:b)

#define SSI_TYPE_FILE    0x01
#define SSI_TYPE_VIRTUAL 0x02
#define SSI_TYPE_EXEC    0x03


/*!
 * \brief Send included file to the stream
 *
 * Load a file from filesystem and send ("include") it to the http-stream
 *
 * \param stream Stream of the socket connection, previously opened for 
 *               binary read and write.
 * \param filename Name of the included file. e.g."UROM:test.txt"
 */

static void NutSsiProcessFile(FILE * stream, u_char *filename)
{
    int fd;
    int n;
    char *data;
    int size;
    long file_len;

    fd = _open(filename, _O_BINARY | _O_RDONLY);

    if (fd == -1) {                     // No such file found... send a 404 string.
        fprintf_P(stream, PSTR("404 Not found: %s\n"), filename);
        return;
    }
    
    file_len = _filelength(fd);
    
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

    _close(fd);
}

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
 * \brief Send a file or cgi with a path relativ to http-root
 *
 * Processes an included local url with a path relativ to http-root. This could also be a 
 * cgi script. Nearly the same as NutHttpProcessFileRequest
 *
 * \param stream Stream of the socket connection, previously opened for 
 *               binary read and write.
 * \param url    URL of the file to be included e.g. "/include/test.js"
 * \param http_root The root path of the http-deamon
 * \param orig_req The http request struct of the top most http_request
 */

static void NutSsiProcessVirtual(FILE * stream, u_char *url, u_char* http_root, REQUEST *orig_req)
{
    int fd;
    int n;
    char *data;
    int size;
    long file_len;
    char *filename = NULL;
    void (*handler)(FILE *stream, int fd, int file_len, u_char *http_root, REQUEST *req);
    
    
    char *cp;
    REQUEST * req;

    if (NutDecodePath(url) == 0) {
        fprintf_P(stream, PSTR("400 Bead request\n"));
        return;
    }
    
    /*
     * Process CGI.
     */
    if (strncasecmp(url, "cgi-bin/", 8) == 0) {
        if ((req = NutHeapAllocClear(sizeof(REQUEST))) == 0) {
            fprintf_P(stream, PSTR("500 Internal error\n"));
            return;
        }
        req->req_method = METHOD_GET;
        req->req_version = orig_req->req_version;
        req->req_length = 0;
        
        if (orig_req->req_agent != NULL) {
            if ((req->req_agent = NutHeapAlloc((strlen(orig_req->req_agent) + 1))) == 0) {
                fprintf_P(stream, PSTR("500 Internal error\n"));
                DestroyRequestInfo(req);
                return;
            }
            strcpy(req->req_agent, orig_req->req_agent);
        }
        if (orig_req->req_cookie!= NULL) {
            if ((req->req_cookie = NutHeapAlloc((strlen(orig_req->req_cookie) + 1))) == 0) {
                fprintf_P(stream, PSTR("500 Internal error\n"));
                DestroyRequestInfo(req);
                return;
            }
            strcpy(req->req_cookie, orig_req->req_cookie);
        }
        if ((cp = strchr(url, '?')) != 0) {
            *cp++ = 0;
            if ((req->req_query = NutHeapAlloc(strlen(cp) + 1)) == 0) {
                fprintf_P(stream, PSTR("500 Internal error\n"));
                DestroyRequestInfo(req);
                return;
            }
            strcpy(req->req_query, cp);
    
            NutHttpProcessQueryString(req);
        }
        if ((req->req_url = NutHeapAlloc(strlen(url) + 1)) == 0) {
            fprintf_P(stream, PSTR("500 Internal error\n"));
            DestroyRequestInfo(req);
            return;
        }
        strcpy(req->req_url, url);

        NutCgiProcessRequest(stream, req);
        DestroyRequestInfo(req);
        return;
    }
    
    /*
     * Process file.
     */
    if (http_root) {
        filename = NutHeapAlloc(strlen(http_root) + strlen(url) + 1);
        strcpy(filename, http_root);
    } else {
        filename = NutHeapAlloc(strlen(url) + 6);
        strcpy(filename, "UROM:");
    }
    
    strcat(filename, url);

    handler = NutGetMimeHandler(filename);

    fd = _open(filename, _O_BINARY | _O_RDONLY);
    NutHeapFree(filename);
    if (fd == -1) {                     // Search for index.html
        u_char *index;
        u_short urll;


        urll = strlen(url);
        if ((index = NutHeapAllocClear(urll + 12)) == 0) {
            fprintf_P(stream, PSTR("500 Internal error\n"));
            return;
        }
        if (urll)
            strcpy(index, url);
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

        handler = NutGetMimeHandler(filename);

        fd = _open(filename, _O_BINARY | _O_RDONLY);
        NutHeapFree(filename);
        if (fd == -1) {                 // We have no index.html. But perhaps an index.shtml?
            urll = strlen(url);
            if ((index = NutHeapAllocClear(urll + 13)) == 0) {
                fprintf_P(stream, PSTR("500 Internal error\n"));
                return;
            }
            if (urll)
                strcpy(index, url);
            if (urll && index[urll - 1] != '/')
                strcat(index, "/");
            strcat(index, "index.shtml");
    
            if (http_root) {
                filename = NutHeapAlloc(strlen(http_root) + strlen(index) + 1);
                strcpy(filename, http_root);
            } else {
                filename = NutHeapAlloc(strlen(index) + 6);
                strcpy(filename, "UROM:");
            }
            strcat(filename, index);
    
            NutHeapFree(index);

            handler = NutGetMimeHandler(filename);

            fd = _open(filename, _O_BINARY | _O_RDONLY);
            NutHeapFree(filename);
            if (fd == -1) {            // Non of both found... send a 404
                fprintf_P(stream, PSTR("404 Not found: %s\n"), filename);
                return;
            }
        }
    }
    
    file_len = _filelength(fd);
    
    if (handler == NULL) {
        size = 512;                 // If we have not registered a mime handler handle default.
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
    } else handler(stream, fd, file_len, http_root, orig_req);
    _close(fd);
}

/*!
 * \brief Skip whitespaces in a string from the given position on
 *
 * Whitespaces are defined as \n \r \t and space. Pos will be set to the first charachter which is not a whitespace
 *
 * \param buffer String to work on
 * \param pos    Start position of the search
 * \param end    last position we should check for
 * \return       Nothing. pos is set to the first character which is not a white space
 */


static void NutSsiSkipWhitespace(char *buffer, u_short *pos, u_short end)
{
    while ((*pos < end) && (
           (buffer[*pos] == '\n') || (buffer[*pos] == '\r') ||
           (buffer[*pos] == '\t') || (buffer[*pos] == ' ')))
        (*pos) ++;
}

/*!
 * \brief Check if a comment is a ssi directive 
 *
 * Check if a comment is a ssi directive and replace the directive by the included data.
 * Allowed diretives are:
 * 
 * <!--#include virtual="/news/news.htm" -->
 * <!--#include file="UROM:/news/news.htm" -->
 * <!--#exec cgi="/cgi-bin/counter.cgi" --> 
 * 
 * \param stream Stream of the socket connection, previously opened for 
 *               binary read and write.
 * \param buffer Current file buffer so search in. The buffer is set to the start of a html comment
 * \param end    End position of the comment.
 * \param http_root The root path of the http-deamon
 * \param req    The http request struct of the top most http_request
 */

static u_char NutSsiCheckForSsi(FILE *stream, char *buffer, u_short end, u_char* http_root, REQUEST *req)
{
    u_short pos = 4; // First character after comment start
    u_char * filename;
    u_char type;

    pos = 4;
    NutSsiSkipWhitespace(buffer, &pos, end);        // Skip whitespaces after comment start
    if (pos == end) return 0;
    
    if (strncasecmp(&buffer[pos], "#include", 8) == 0) { // Search include directive
        pos += 8;
        type = SSI_TYPE_VIRTUAL;
    } else 
    if (strncasecmp(&buffer[pos], "#exec", 5) == 0) { // Search include or exec directive
        pos += 5;
        type = SSI_TYPE_EXEC;
    } else return 0;                                // No include or exec found. Skip the rest of this comment...
    if (pos >= end) return 0;

    NutSsiSkipWhitespace(buffer, &pos, end);        // Skip whitespaces after #include directive
    if (pos == end) return 0;
    
    if (type == SSI_TYPE_VIRTUAL) {
        if (strncasecmp(&buffer[pos], "virtual", 7) == 0) {  // Search virtual directive
            pos += 7;
        } else                                      // No virtual found. Test for file...
        if (strncasecmp(&buffer[pos], "file", 4) == 0) {  // Search file directive
            pos += 4;
            type = SSI_TYPE_FILE;
        } else return 0;                            // No file found. Test for file...
    } else {
        if (strncasecmp(&buffer[pos], "cgi", 3) == 0) {  // Search cgi directive
            pos += 3;
        } else return 0;                            // No cgi found. return...
    }
    if (pos >= end) return 0;
    
    NutSsiSkipWhitespace(buffer, &pos, end);        // Skip whitespaces after virtual, file or cgi directive
    if (pos == end) return 0;

    if (buffer[pos] != '=') return 0;               // check for assertion
    pos ++; 

    NutSsiSkipWhitespace(buffer, &pos, end);        // Skip whitespaces after assertion
    if (pos == end) return 0;    
    
    if (buffer[pos] == '"') {                       // Search for filename and pass to output function
        pos ++;
        if (pos == end) return 0;
        filename = &buffer[pos];
        while (buffer[pos] != '"') {
            pos ++;
            if (pos == end) return 0;
        }
        buffer[pos] = '\0';
        switch (type) {
            case SSI_TYPE_FILE:
                NutSsiProcessFile(stream, filename);
                break;
            case SSI_TYPE_VIRTUAL:
                NutSsiProcessVirtual(stream, filename, http_root, req);
                break;
            case SSI_TYPE_EXEC:
                NutSsiProcessVirtual(stream, filename, http_root, req);
                break;
        }
    }
    return 1;
}

/*!
 * \brief Check a file for html comments 
 *
 * Check a file for html comments and then call NutSsiCheckForSsi to seach a ssi directive
 * Allowed diretives are:
 * 
 * <!--#include virtual="/news/news.htm" -->
 * <!--#include file="UROM:/news/news.htm" -->
 * <!--#exec cgi="/cgi-bin/counter.cgi" --> 
 * 
 * \param stream Stream of the socket connection, previously opened for 
 *               binary read and write.
 * \param fd     Filedescriptor pointing to a just opened file.
 * \param file_len length of this file
 * \param http_root The root path of the http-deamon
 * \param req    The http request struct of the top most http_request
 */


static void NutHttpProcessSHTML(FILE * stream, int fd, int file_len, u_char* http_root, REQUEST *req)
{
    u_char * buffer;
    u_char in_comment;
    int buffsize;
    int fpos;
    int n;
    u_char *index;
    u_char found;
    buffsize = MIN(BUFSIZE, file_len);
    buffer = NutHeapAlloc(buffsize+1);
    in_comment = 0;
    fpos = 0;
    while (file_len != fpos) {
        memset(buffer, 0, buffsize+1);
        n = _read(fd, buffer, MIN(buffsize, file_len-fpos));
        
        if (!in_comment) {        
            
            index = strstr(buffer, "<!--");
            if (index == NULL) {                    // Nothing found. print next 412 characters, seek to next startpoint.
                if (file_len > buffsize) {
                    fwrite(buffer, 1, MIN(buffsize-100, n), stream);
                    fpos += MIN(buffsize-100, n);
                    _seek(fd, fpos, SEEK_SET);
                } else {
                    fwrite(buffer, 1, n, stream);
                    fpos += n;
                }
                
            } else {
                found = (int)index - (int)buffer;   // We have found a comment initializer. Seek to the startpoint and print the beginning of the buffer.
                fwrite (buffer, 1, found, stream);
                fpos += found;
                _seek(fd, fpos, SEEK_SET);
                in_comment = 1;
            }
        } else {                                    // Ok, we assume we are "into" a comment.    
            index = strstr(buffer, "-->");
            if (index == NULL) {                    // We have not found the end of the comment in the next 512 characters. Byepass this comment.
                fwrite(buffer, 1, MIN(buffsize, n), stream);
                fpos += MIN(buffsize, n);
                in_comment = 0;
            } else {                                // Ok. This seems to be a comment with maximum length of 512 bytes. We now search for ssi code.    
                found = (int)index - (int)buffer;
                if (!NutSsiCheckForSsi(stream, buffer, found, http_root, req)) {
                    fwrite(buffer, 1, found+3, stream);
                }
                fpos += found+3;
                _seek(fd, fpos, SEEK_SET);
                in_comment = 0;
            }
        }
    }
    
    NutHeapFree(buffer);
}

/*!
 * \brief Register SSI handler for shtml files.
 * 
 * shtml files may use the following ssi commands:
 *
 * <!--#include virtual="/news/news.htm" -->
 * <!--#include file="UROM:/news/news.htm" -->
 * <!--#exec cgi="/cgi-bin/counter.cgi" -->
 */

void NutRegisterSsi(void)
{
    NutSetMimeHandler(".shtml", NutHttpProcessSHTML);
}

/*@}*/
