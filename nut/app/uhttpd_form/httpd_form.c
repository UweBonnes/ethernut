#if defined(HTTPD_FORM_SAMPLE)

/*
 * Copyright (C) 2012 by egnite GmbH
 *
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
 * $Id$
 */

#include <pro/uhttp/mediatypes.h>
#include <pro/uhttp/modules/mod_redir.h>
#include <pro/uhttp/modules/mod_cgi_func.h>

static int SendResult(HTTPD_SESSION *hs, char *first, char *last)
{
    static const char head[] = 
        "<html>"
        "<head>"
        "<title>Form Result</title>"
        "</head>";
    static const char body[] = 
        "<body>"
        "<p>Hello %s %s!</p>"
        "<a href=\"/index.html\">back</a>"
        "</body>"
        "<html>";

    HttpSendHeaderTop(hs, 200);
    HttpSendHeaderBottom(hs, "text", "html", -1);

    s_puts(head, hs->s_stream);
    s_printf(hs->s_stream, body, first, last);
    s_flush(hs->s_stream);

    return 0;
}

static int CgiGetForm(HTTPD_SESSION *hs)
{
    char *arg;
    char *val;
    char *first = NULL;
    char *last = NULL;

    for (arg = HttpArgParseFirst(&hs->s_req); arg; arg = HttpArgParseNext(&hs->s_req)) {
        val = HttpArgValue(&hs->s_req);
        if (val) {
            if (strcmp(arg, "firstname") == 0) {
                first = strdup(val);
            }
            else if (strcmp(arg, "familyname") == 0) {
                last = strdup(val);
            }
        }
    }
    SendResult(hs, first, last);
    free(first);
    free(last);

    return 0;
}

static int CgiPostForm(HTTPD_SESSION *hs)
{
    char *arg;
    char *val;
    char *first = NULL;
    char *last = NULL;
    long avail;

    avail = hs->s_req.req_length;
    while (avail) {
        arg = HttpArgReadNext(hs, &avail);
        if (arg) {
            val = HttpArgValue(&hs->s_req);
            if (val) {
                if (strcmp(arg, "firstname") == 0) {
                    first = strdup(val);
                }
                else if (strcmp(arg, "familyname") == 0) {
                    last = strdup(val);
                }
            }
        }
    }
    SendResult(hs, first, last);
    free(first);
    free(last);

    return 0;
}

int main(void)
{
    puts("uHTTP form sample\nBuild " __DATE__ " " __TIME__);

    StreamInit();
    MediaTypeInitDefaults();
    HttpRegisterRedir("", "/index.html", 301);
    HttpRegisterCgiFunction("getform.cgi", CgiGetForm);
    HttpRegisterCgiFunction("postform.cgi", CgiPostForm);
    HttpRegisterMediaType("cgi", NULL, NULL, HttpCgiFunctionHandler);
    StreamClientAccept(HttpdClientHandler, "8088");
}

#endif
