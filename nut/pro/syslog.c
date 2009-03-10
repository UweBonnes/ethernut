/*
 * Copyright (C) 2004 by egnite Software GmbH. All rights reserved.
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
 * -
 * Portions Copyright (c) 1983, 1988, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*!
 * \file pro/syslog.c
 * \brief Syslog Client
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.5  2009/02/13 14:52:05  haraldkipp
 * Include memdebug.h for heap management debugging support.
 *
 * Revision 1.4  2008/08/11 07:00:36  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.3  2005/08/02 17:47:04  haraldkipp
 * Major API documentation update.
 *
 * Revision 1.2  2004/10/03 18:41:43  haraldkipp
 * RAM saving calls added
 *
 * Revision 1.1  2004/09/19 11:18:45  haraldkipp
 * Syslog client added
 *
 * \endverbatim
 */

#include <cfg/syslog.h>
#include <sys/confos.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>
#include <time.h>
#include <memdebug.h>

#include <sys/syslog.h>

#ifndef SYSLOG_PERROR_ONLY

#include <sys/socket.h>
#include <arpa/inet.h>
static UDPSOCKET *syslog_sock;

#endif                          /* SYSLOG_PERROR_ONLY */

/*!
 * \addtogroup xgSyslog
 */
/*@{*/

#ifndef SYSLOG_MAXBUF
/*!
 * \brief Syslog message buffer size.
 */
#define SYSLOG_MAXBUF 256
#endif

static uint16_t syslog_port = 514;
static int syslog_fac = LOG_USER;
static int syslog_mask = 0xFF;

static uint32_t syslog_server;
static int syslog_stat;
static size_t syslog_taglen;
static char *syslog_tag;
static char *syslog_buf;

static char mon_name[] = "JanFebMarAprMayJunJulAugSepOctNovDec";


/*!
 * \brief Print log message.
 *
 * Alternate form of syslog(), in which the arguments have already been captured
 * using the variable-length argument facilities.
 *
 * \param pri Priority level of this message. See syslog().
 * \param fmt Format string containing conversion specifications like printf.
 * \param ap  List of arguments.
 */
void vsyslog(int pri, CONST char *fmt, va_list ap)
{
    time_t now;
    struct _tm *tip;
    size_t cnt;

    /* Remove invalid bits. */
    pri &= LOG_PRIMASK | LOG_FACMASK;

    /* Check priority against setlog mask values. */
    if ((LOG_MASK(LOG_PRI(pri)) & syslog_mask) == 0) {
        return;
    }

    /* Open log if not done before. */
    if (syslog_buf == 0) {
        openlog(0, syslog_stat | LOG_NDELAY, syslog_fac);
    }

    /* Set default facility if none specified. */
    if ((pri & LOG_FACMASK) == 0) {
        pri |= syslog_fac;
    }

    time(&now);
    tip = localtime(&now);
    sprintf(syslog_buf, "<%d>%.3s%3d %02d:%02d:%02d %s ", pri, &mon_name[tip->tm_mon * 3],
            tip->tm_mday, tip->tm_hour, tip->tm_min, tip->tm_sec, confos.hostname);
    cnt = strlen(syslog_buf);

    if (syslog_taglen) {
        cnt += syslog_taglen + 2;
        if (cnt >= SYSLOG_MAXBUF) {
            return;
        }
        strcat(syslog_buf, syslog_tag);
        strcat(syslog_buf, ": ");
    }

    /* Potentially dangerous. We need vsnprintf() */
    if (cnt + strlen(fmt) >= SYSLOG_MAXBUF) {
        return;
    }
    vsprintf(&syslog_buf[cnt], fmt, ap);
    cnt = strlen(syslog_buf);

    /* Output to stderr if requested */
    if (syslog_stat & LOG_PERROR) {
        _write(_fileno(stderr), syslog_buf, cnt);
        _write(_fileno(stderr), "\n", 1);
    }
#ifndef SYSLOG_PERROR_ONLY
    /*
     * Output the message to a remote logger.
     */
    if (syslog_server) {
        NutUdpSendTo(syslog_sock, syslog_server, syslog_port, syslog_buf, cnt);
    }
#endif
}

/*!
 * \brief Print log message.
 *
 * The message is tagged with priority.
 *
 * \param pri Priority level of this message, selected from the following
 *            ordered list (high to low):
 *            - LOG_EMERG   A panic condition.
 *            - LOG_ALERT   A condition that should be corrected immediately.
 *            - LOG_CRIT    Critical conditions, e.g., hard device errors.
 *            - LOG_ERR     Errors.
 *            - LOG_WARNING Warning messages.
 *            - LOG_NOTICE  Conditions that are not error conditions, but should 
 *                          possibly be handled specially.
 *            - LOG_INFO    Informational messages.
 *            - LOG_DEBUG   Messages that contain information normally of use only 
 *                          when debugging a program.
 * \param fmt Format string containing conversion specifications like printf.
 */
void syslog(int pri, CONST char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vsyslog(pri, (char *) fmt, ap);
    va_end(ap);
}

#ifdef __HARVARD_ARCH__
/*!
 * \brief Print log message.
 *
 * Alternate form of syslog(), in which the arguments have already been captured
 * using the variable-length argument facilities.
 *
 * \param pri Priority level of this message. See syslog().
 * \param fmt Format string containing conversion specifications like printf.
 * \param ap  List of arguments.
 */
void vsyslog_P(int pri, PGM_P fmt, va_list ap)
{
    time_t now;
    struct _tm *tip;
    size_t cnt;

    /* Remove invalid bits. */
    pri &= LOG_PRIMASK | LOG_FACMASK;

    /* Check priority against setlog mask values. */
    if ((LOG_MASK(LOG_PRI(pri)) & syslog_mask) == 0) {
        return;
    }

    /* Open log if not done before. */
    if (syslog_buf == 0) {
        openlog(0, syslog_stat | LOG_NDELAY, syslog_fac);
    }

    /* Set default facility if none specified. */
    if ((pri & LOG_FACMASK) == 0) {
        pri |= syslog_fac;
    }

    time(&now);
    tip = localtime(&now);
    sprintf(syslog_buf, "<%d>%.3s%3d %02d:%02d:%02d %s ", pri, &mon_name[tip->tm_mon * 3],
            tip->tm_mday, tip->tm_hour, tip->tm_min, tip->tm_sec, confos.hostname);
    cnt = strlen(syslog_buf);

    if (syslog_taglen) {
        cnt += syslog_taglen + 2;
        if (cnt >= SYSLOG_MAXBUF) {
            return;
        }
        strcat(syslog_buf, syslog_tag);
        strcat(syslog_buf, ": ");
    }

    /* Potentially dangerous. We need vsnprintf() */
    if (cnt + strlen_P(fmt) >= SYSLOG_MAXBUF) {
        return;
    }
    vsprintf_P(&syslog_buf[cnt], fmt, ap);
    cnt = strlen(syslog_buf);

    /* Output to stderr if requested */
    if (syslog_stat & LOG_PERROR) {
        _write(_fileno(stderr), syslog_buf, cnt);
        _write(_fileno(stderr), "\n", 1);
    }
#ifndef SYSLOG_PERROR_ONLY
    /*
     * Output the message to a remote logger.
     */
    if (syslog_server) {
        NutUdpSendTo(syslog_sock, syslog_server, syslog_port, syslog_buf, cnt);
    }
#endif
}

/*!
 * \brief Print log message.
 *
 * The message is tagged with priority.
 *
 * \param pri Priority level of this message, selected from the following
 *            ordered list (high to low):
 *            - LOG_EMERG   A panic condition.
 *            - LOG_ALERT   A condition that should be corrected immediately.
 *            - LOG_CRIT    Critical conditions, e.g., hard device errors.
 *            - LOG_ERR     Errors.
 *            - LOG_WARNING Warning messages.
 *            - LOG_NOTICE  Conditions that are not error conditions, but should 
 *                          possibly be handled specially.
 *            - LOG_INFO    Informational messages.
 *            - LOG_DEBUG   Messages that contain information normally of use only 
 *                          when debugging a program.
 * \param fmt Format string containing conversion specifications like printf.
 */
void syslog_P(int pri, PGM_P fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vsyslog_P(pri, fmt, ap);
    va_end(ap);
}

#endif /* __HARVARD_ARCH__ */

/*!
 * \brief Set the log priority mask level.
 *
 * Calls to syslog() with a priority not set are ignored. The default allows all 
 * priorities to be logged.
 *
 * \param logmask New priority mask.
 *
 * \return Previous mask.
 */
int setlogmask(int logmask)
{
    int rc = syslog_mask;

    if (logmask) {
        syslog_mask = logmask;
    }
    return rc;
}

/*!
 * \brief Set the log server's IP address.
 *
 * \param ip   IP address in network byte order. If 0, no messages will be sent out.
 * \param port Port number. If 0, then standard port is used.
 *
 * \return Previous IP.
 */
uint32_t setlogserver(uint32_t ip, uint16_t port)
{
    uint32_t rc = syslog_server;

    syslog_server = ip;
    if (port) {
        syslog_port = port;
    }
    return rc;
}

/*!
 * \brief Open logging for specialized processing.
 *
 * This function provides for more specialized processing of the messages sent by
 * syslog() and vsyslog().
 *
 * \param ident   This string that will be prepended to every message.
 * \param logstat A bit field specifying logging options, which is formed by
 *                OR'ing one or more of the following values:
 *                - LOG_CONS   Ignored but accepted for compatibility.
 *                - LOG_NDELAY Ignored but accepted for compatibility.
 *                - LOG_PERROR Additionally write the message to standard error output.
 *                - LOG_PID    Ignored but accepted for compatibility.
 * \param logfac  Encodes a default facility to be assigned to all messages that do not 
 *                have an explicit facility encoded:
 *                - LOG_AUTH     The authorization system:
 *                - LOG_AUTHPRIV The same as LOG_AUTH.
 *                - LOG_CRON     The cron daemon,
 *                - LOG_DAEMON   System daemons, that are not provided for explicitly by 
 *                               other facilities.
 *                - LOG_FTP      The file transfer protocol daemon,
 *                - LOG_KERN     Messages generated by the kernel. These should not be 
 *                               generated by applications.
 *                - LOG_LPR      The line printer spooling system.
 *                - LOG_MAIL     The mail system.
 *                - LOG_NEWS     The network news system.
 *                - LOG_SYSLOG   Messages generated internally by syslog.
 *                - LOG_USER     This is the default facility identifier if none is specified.
 *                - LOG_UUCP     The UUCP system.
 *                - LOG_LOCAL0..LOG_LOCAL7 Reserved for local use.
 */
void openlog(CONST char *ident, int logstat, int logfac)
{
    if (ident == 0) {
        ident = syslog_tag;
        syslog_taglen = 0;
    }

    closelog();

    syslog_stat = logstat;
    syslog_fac = logfac;
    if (ident && *ident) {
        syslog_taglen = strlen(ident);
        syslog_tag = malloc(syslog_taglen + 1);
        strcpy(syslog_tag, ident);
    }
    if (syslog_stat & LOG_NDELAY) {
        if (syslog_buf == 0) {
            syslog_buf = malloc(SYSLOG_MAXBUF);
        }
#ifndef SYSLOG_PERROR_ONLY
        if (syslog_sock == 0) {
            syslog_sock = NutUdpCreateSocket(514);
        }
#endif
    }
}

/*!
 * \brief Release system resources occupied by syslog().
 */
void closelog(void)
{
    if (syslog_buf) {
        free(syslog_buf);
        syslog_buf = 0;
    }
    if (syslog_taglen) {
        free(syslog_tag);
        syslog_taglen = 0;
    }
#ifndef SYSLOG_PERROR_ONLY
    if (syslog_sock) {
        NutUdpDestroySocket(syslog_sock);
        syslog_sock = 0;
    }
#endif
}

/*@}*/

