/**************************************************************************
*  Copyright (c) 2014 by Michael Fischer. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*  3. Neither the name of the author nor the names of its contributors may
*     be used to endorse or promote products derived from this software
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
*  SUCH DAMAGE.
*
*/
/* Simple server that answers requests from iperf2 clients.
 *
 * Dec 2, 2018: NutOs Network stack can not yet handle iperf3 requests.
 */

#define __IPERF_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <io.h>

#include <dev/board.h>
#include <dev/hw_signature.h>
#include <sys/version.h>
#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/route.h>

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define MY_MAC    "\x00\x06\x98\x30\x00\x35"
#define MY_IPADDR "10.0.1.200"
#define MY_IPMASK "255.255.255.0"
#define MY_IPGATE "0.0.0.0"

/* Iperf thread stack size. */
#ifndef IPERF_SERVICE_STACK

#if defined(__CORTEX__)
#define IPERF_SERVICE_STACK   ((1024 * NUT_THREAD_STACK_MULT) + NUT_THREAD_STACK_ADD)
#else
#define IPERF_SERVICE_STACK   ((2048 * NUT_THREAD_STACK_MULT) + NUT_THREAD_STACK_ADD)
#endif

#endif /* IPERF_SERVICE_STACK */

/* Default Iperf3 port */
#define IPERF_TCP_PORT        5201

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* Dummy read buffer */
static uint8_t DummyRxBuffer[4096];
static volatile uint32_t total;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  IperfTask                                                            */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
THREAD(IperfTask, arg)
{
    TCPSOCKET *sock;
    FILE      *stream;
    size_t     Result;

    (void)arg;

    /*
     * Now loop endless for connections.
     */
    for (;;)
    {
       /*
        * Create a socket.
        */
       if ((sock = NutTcpCreateSocket()) == 0)
       {
          printf("Creating socket failed\n");
          NutSleep(5000);
          continue;
       }
       /*
        * Listen on port IPERF_TCP_PORT. This call will block
        * until we get a connection from a client.
        */
       NutTcpAccept(sock, IPERF_TCP_PORT);
       /*
        * Associate a stream with the socket so we can use standard I/O calls.
        */
       if ((stream = _fdopen((int) ((uintptr_t) sock), "r+b")) == 0)
       {
          printf("Creating stream device failed\n");
       }
       else
       {
          /*
           * Receive IPerf data
           */
          do
          {
             Result = fread(DummyRxBuffer, 1, sizeof(DummyRxBuffer), stream);
             if (Result <= 0)
             {
                break;
             }
             total += Result;
          } while (1);

          /*
           * Destroy the virtual stream device.
           */
          fclose(stream);
       } /* end create stream */

       /*
        * Close our socket.
        */
       NutTcpCloseSocket(sock);
    } /* end for (;;) */

} /* IperfTask */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  main                                                                 */
/*                                                                       */
/*  Nut/OS automatically calls this entry after initialization.          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
int main (void)
{
#if defined(UNIQUE_PRIVATE_MAC)
    uint8_t  mac[6];
    UNIQUE_PRIVATE_MAC(mac);
#else
    uint8_t  mac[] = MY_MAC;
#endif
    uint32_t ip_addr = inet_addr(MY_IPADDR);
    uint32_t ip_mask = inet_addr(MY_IPMASK);
    uint32_t ip_gate = inet_addr(MY_IPGATE);
    uint32_t baud = 115200;
    uint8_t  i;

    /*
     * Initialize the uart device.
     */
    NutRegisterDevice(&DEV_CONSOLE, 0, 0);
    freopen(DEV_CONSOLE.dev_name, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);
    NutSleep(200);
    printf("\n\nNut/OS %s Iperf test\n", NutVersionString());

    /*
     * Register Ethernet controller.
     */
    if (NutRegisterDevice(&DEV_ETHER, 0, 0))
    {
       puts("Registering device failed");
    }

    printf("Configure %s...", DEV_ETHER_NAME);
    if (NutNetIfConfig(DEV_ETHER_NAME, mac, ip_addr, ip_mask) == 0)
    {
       /* Without DHCP we had to set the default gateway manually.*/
       if(ip_gate)
       {
          printf("hard coded gate...");
          NutIpRouteAdd(0, 0, ip_gate, &DEV_ETHER);
       }
       puts("OK");
    }
    else
    {
       puts("failed");
    }
    printf("%s ready\n", inet_ntoa(ip_addr));


    /*
     * Start four Iperf threads.
     */
    for (i = 1; i <= 4; i++)
    {
       char thname[] = "iperf0";

       thname[5] = '0' + i;
       NutThreadCreate(thname, IperfTask, NULL, IPERF_SERVICE_STACK);
    }

    /*
     * We could do something useful here, like serving a watchdog.
     */
    NutThreadSetPriority(254);
    uint32_t last_total = total;
    for (;;)
    {
       NutSleep(1000);
       uint32_t new_total = total;
       if (new_total != last_total) {
           printf("%lu bytes/sec\n", new_total - last_total);
           last_total = new_total;
       }
    }
    return(0);
} /* main */

