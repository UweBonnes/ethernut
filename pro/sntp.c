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
 * Thanks to Lars H. Andersson, who submitted the first idea of this simple function
 */
/*
 * $Log$
 * Revision 1.1  2003/11/24 18:13:22  drsung
 * first release
 *
 *
 */

#include <pro/sntp.h>
#include <sys/socket.h>
#include <sys/heap.h>
#include <string.h>
#include "../crt/ctime.h"
#include <stdio.h>
#include <sys/thread.h>
#include <sys/timer.h>

typedef struct _sntpframe sntpframe;
struct _sntpframe {
    u_char mode;
    u_char stratum;
    u_char poll;
    u_char precision;
    u_long root_delay;
    u_long root_dispersion;
    u_long reference_identifier;
    u_long reference_ts_sec;
    u_long reference_ts_frac;
    u_long originate_ts_sec;
    u_long originate_ts_frac;
    u_long receive_ts_sec;
    u_long receive_ts_frac;
    u_long transmit_ts_sec;
    u_long transmit_ts_frac;
};


#define NTP_PORT	123
#define SNTP_PORT NTP_PORT

struct SNTP_resync_args {
    u_long server_addr;
    u_long interval;
};

static THREAD(SNTP_resync, arg)
{
    u_long server_addr = ((struct SNTP_resync_args *) arg)->server_addr;
    u_long interval = ((struct SNTP_resync_args *) arg)->interval;
    u_long cur_server_addr = server_addr;
    NutHeapFree(arg);

    NutThreadSetPriority(63);
    int retry = 0;
    time_t t;
    for (;;) {
        if (NutSNTPGetTime(&cur_server_addr, &t)) {     // if any error retry
            if (cur_server_addr != server_addr && server_addr == 0xFFFFFFFF) {
                cur_server_addr = server_addr;
                continue;
            }

            if (retry++ >= 3) { // if numer of retries >= 3 wait 30 secs before next retry sequence ...
                retry = 0;
                NutSleep(30000);
            } else              // ... else wait 5 secs for next retry
                NutSleep(5000);
        } else {                // no error
            stime(&t);          // so set the time
            retry = 0;
            NutSleep(interval); // and wait the interval time
        }
    }
}

int NutSNTPGetTime(u_long * server_adr, time_t * t)
{
    //first check the pointers
    u_long rec_addr;
    if (t == NULL)
        return -1;
    if (server_adr == NULL)
        return -1;

    UDPSOCKET *sock = NULL;     // the udp socket
    sntpframe *data;            // we're using the heap to save stack space
    u_short port;               // source port from incoming packet
    int len;
    int result = -1;

    if ((data = NutHeapAllocClear(sizeof(*data))) == NULL)
        goto error;

    sock = NutUdpCreateSocket(0);       // allocate new udp socket
    if (sock == NULL)
        goto error;

    data->mode = 0x1B;          // LI, VN and Mode bit fields (all in u_char mode);
    if (NutUdpSendTo(sock, *server_adr, SNTP_PORT, data, sizeof(*data)))        // Send packet to server
        goto error;             // on error return -1

  retry:
    rec_addr = 0;
    len = NutUdpReceiveFrom(sock, &rec_addr, &port, data, sizeof(*data), 5000); // Receive packet with timeout of 5s
    if (len <= 0) {
        goto error;             // error or timeout occured
    }

    if (port != SNTP_PORT || (data->mode & 0xc0) == 0xc0)       // if source port is not SNTP_PORT or server is not in sync return
    {
        if (*server_adr == 0xFFFFFFFF)
            goto retry;         // unusable packets will be just ignored.
        else
            goto error;
    }

    *t = ntohl(data->transmit_ts_sec) - (70 * 365 + _LEAP_YEAR_ADJUST) * _DAY_SEC;
    *server_adr = rec_addr;
    result = 0;
  error:
    if (sock)
        NutUdpDestroySocket(sock);
    if (data)
        NutHeapFree(data);
    return result;

}

int NutSNTPStartThread(u_long server_addr, u_long interval)
{
    struct SNTP_resync_args *arg = NutHeapAlloc(sizeof(struct SNTP_resync_args));
    if (!arg)
        return -1;
    arg->server_addr = server_addr;
    arg->interval = interval;
    if (NutThreadCreate("sntpc", SNTP_resync, arg, 256))
        return 0;
    else {
        NutHeapFree(arg);
        return -1;
    }
}
