
/*
  * Copyright (C) 2004 by Jean Pierre Gauthier. All rights reserved.
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
  * THIS SOFTWARE IS PROVIDED BY <YOUR NAME> AND CONTRIBUTORS
  * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <YOUR NAME>
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
  * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  * SUCH DAMAGE.
  */

/* ********************************************************* */
/* 
Netbios WINS (RFC 1002) Name Query. 
Only Query Request Client Routine sending/Positive Name Query Response receiving
are implemented. 
When the Netbios Name Query request UDP datagram is on the ethernet network, asking 
"Who is 'name'?", NutWinsNameQuery answers with the specified 'ipaddr' Ethernut IP address. 
Answer to Microsoft Windows/Internet Explorer calls by "http://name" command line
(and even directly "name" as command line if "name" is not a shared folder). 

Launch for example :
THREAD(wins_deamon, arg)
{
NutWinsNameQuery  (  "myboard", inet_addr(MYIP) ) ;
} 
*/
/* ********************************************************* */

#include <cfg/os.h>
#include <string.h>

#include <sys/heap.h>
#include <sys/socket.h>
#include <sys/sock_var.h>
#include <sys/types.h>
#include <pro/wins.h>
#include <netinet/in.h>
#ifdef NUTDEBUG
#include <stdio.h>
#endif

extern u_char toupper(u_char);
/* ********************************************************* */
typedef struct {
    u_short id;
    u_short flags;
    u_short quests;
    u_short answers;
    u_short authrr;
    u_short addrr;
    u_char namelen;
    u_char name[33];
    u_short type;
    u_short class;              /* end of request */
    u_long ttl;
    u_short len_rep;
    u_char node_flags;
    u_char node_type;
    u_long ip_addr;             /* end of answer */
} WINSHEADER;


/* ********************************************************* */
/* name : netbios label (15 chars max), ipaddr : network ordered IP address bytes */
int NutWinsNameQuery(u_char * name, u_long ipaddr)
{
    WINSHEADER *pkt = NULL;
    u_char *encoded = NULL;
    UDPSOCKET *sock;
    u_long raddr;
    u_short rport;
    u_char car;
    int i, j;
    if (strlen(name) > 15)
        return -1;
    if (((pkt = NutHeapAllocClear(sizeof(WINSHEADER))) == NULL) ||      /* */
        ((encoded = NutHeapAllocClear(33)) == NULL) ||  /* */
        ((sock = NutUdpCreateSocket(137)) == 0) /* NETBIOS UDP port */
        ) {
        if (pkt != NULL)
            NutHeapFree(pkt);
        if (encoded != NULL)
            NutHeapFree(encoded);
        return -1;
    }
    j = 0;
    for (i = 0; i < 16; i++) {  /* label  'compression' */
        car = toupper(i < strlen(name) ? name[i] : ' ');
        if (i == 15)
            car = 0;
        encoded[j] = (car >> 4) + 'A';
        encoded[j + 1] = (car & 0xf) + 'A';
        j += 2;
    }
    encoded[j] = 0;
     /* printf("local compressed name=\n%s \n", encoded); */
    for (;;) {                  /* infinite loop / Netbios deamon */
        NutUdpReceiveFrom(sock, &raddr, &rport, pkt, sizeof(WINSHEADER), 0);
        /* RFC1002 Name Query Request verification */
        if (((ntohs(pkt->flags) & 0xf800) != 0) ||      /* */
            (ntohs(pkt->quests) != 1) ||        /* */
            (pkt->namelen != 0x20) ||   /* */
            (ntohs(pkt->type) != 32) || /* */
            (ntohs(pkt->class) != 1) || /* */
            (strcmp(pkt->name, encoded)))
            continue;           /* bad request, try again */
         /* printf("Name=%s recognized\r\n", name); */
        /* build RFC1002 Positive Name Query Response */
        pkt->flags = htons(0x8580);     /* Response flags */
        pkt->answers = htons(1);
        pkt->ttl = htonl((u_long) 60);  /* 60 seconds validity */
        pkt->len_rep = htons(6);
        pkt->node_flags = pkt->node_type = pkt->quests = 0;     /* B-type node, etc... */
        pkt->ip_addr = ipaddr;  /* Returned IP Address, end of answer */
        NutUdpSendTo(sock, raddr, 137, pkt, sizeof(WINSHEADER));        /* send to netbios port */
        memset(pkt, 0, sizeof(WINSHEADER));
    }
}

/*@}*/
