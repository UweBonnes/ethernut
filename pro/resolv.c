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
 */

/*
 * $Log$
 * Revision 1.8  2005/04/30 16:42:42  chaac
 * Fixed bug in handling of NUTDEBUG. Added include for cfg/os.h. If NUTDEBUG
 * is defined in NutConf, it will make effect where it is used.
 *
 * Revision 1.7  2004/10/14 16:43:07  drsung
 * Fixed compiler warning "comparison between signed and unsigned"
 *
 * Revision 1.6  2004/07/28 19:23:15  drsung
 * call to DumpDnsResource commented out.
 *
 * Revision 1.5  2004/04/15 18:38:58  drsung
 * Bugfix if the DNS server sends more than one answer.
 *
 * Revision 1.4  2004/03/18 13:39:05  haraldkipp
 * Deprecated header file replaced
 *
 * Revision 1.3  2004/02/28 20:14:38  drsung
 * Merge from nut-3_4-release b/c of bugfixes.
 *
 * Revision 1.2.2.1  2004/02/28 19:15:07  drsung
 * Memory leak fixed in CreateDnsQuestion.
 * Thanks to Jean Pierre Gauthier.
 *
 * Revision 1.2  2003/07/20 18:25:40  haraldkipp
 * Support secondary DNS.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:59  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.5  2003/02/04 18:17:32  harald
 * Version 3 released
 *
 * Revision 1.4  2002/06/26 17:29:50  harald
 * First pre-release with 2.4 stack
 *
 */

#include <cfg/os.h>
#include <string.h>

#include <sys/device.h>
#include <sys/timer.h>
#include <sys/heap.h>

#include <arpa/inet.h>
#include <net/if_var.h>
#include <sys/socket.h>

#ifdef NUTDEBUG
#include <stdio.h>
#endif

/*!
 * \addtogroup xgDNS
 */
/*@{*/

typedef struct {
    u_char *doc_hostname;
    u_char *doc_domain;
    u_long doc_ip1;
    u_long doc_ip2;
} DNSCONFIG;

static DNSCONFIG doc;

typedef struct {
    u_short doh_id;
    u_short doh_flags;
    u_short doh_quests;
    u_short doh_answers;
    u_short doh_authrr;
    u_short doh_addrr;
} DNSHEADER;

typedef struct {
    u_char *doq_name;
    u_short doq_type;
    u_short doq_class;
} DNSQUESTION;

typedef struct {
    u_char *dor_name;
    u_short dor_type;
    u_short dor_class;
    u_long dor_ttl;
    u_short dor_len;
    u_char *dor_data;
} DNSRESOURCE;

#ifdef NUTDEBUG
void DumpDnsHeader(FILE * stream, DNSHEADER * doh)
{
    fprintf(stream,
            "HEADER: id=%u flg=%04X #q=%u #an=%u #au=%u #ad=%u\r\n",
            doh->doh_id, doh->doh_flags, doh->doh_quests, doh->doh_answers, doh->doh_authrr, doh->doh_addrr);
}

void DumpDnsQuestion(FILE * stream, DNSQUESTION * doq)
{
    fprintf(stream, "QUESTION: name='%s' type=%u class=%u\r\n", doq->doq_name, doq->doq_type, doq->doq_class);
}

void DumpDnsResource(FILE * stream, DNSRESOURCE * dor)
{
    u_short i;

    fprintf(stream, "RESOURCE: name='%s' type=%u class=%u ttl=%lu len=%u ",
            dor->dor_name, dor->dor_type, dor->dor_class, dor->dor_ttl, dor->dor_len);
    for (i = 0; i < dor->dor_len; i++)
        fprintf(stream, "%02X ", dor->dor_data[i]);
    fputc('\n', stream);
}
#endif

static u_short AddShort(u_char * cp, u_short val)
{
    *cp++ = (u_char) (val >> 8);
    *cp++ = (u_char) val;

    return 2;
}

static u_short AddName(u_char * cp, CONST u_char * name)
{
    u_char *lcp;
    u_short rc = strlen(name) + 2;

    lcp = cp++;
    *lcp = 0;
    while (*name) {
        if (*name == '.') {
            lcp = cp++;
            *lcp = 0;
            name++;
        } else {
            *cp++ = *name++;
            (*lcp)++;
        }
    }
    *cp = 0;

    return rc;
}

static u_short ScanShort(u_char * cp, u_short * val)
{
    *val = (*cp++) << 8;
    *val |= *cp;

    return 2;
}

static u_short ScanLong(u_char * cp, u_long * val)
{
    *val = *cp++;
    *val <<= 8;
    *val |= *cp++;
    *val <<= 8;
    *val |= *cp++;
    *val <<= 8;
    *val |= *cp;

    return 4;
}

static u_short ScanName(u_char * cp, u_char ** npp)
{
    u_char len;
    u_short rc;
    u_char *np;

    if (*npp) {
        NutHeapFree(*npp);
        *npp = 0;
    }

    if ((*cp & 0xC0) == 0xC0)
        return 2;

    rc = strlen(cp) + 1;
    np = *npp = NutHeapAlloc(rc);
    len = *cp++;
    while (len) {
        while (len--)
            *np++ = *cp++;
        if ((len = *cp++) != 0)
            *np++ = '.';
    }
    *np = 0;

    return rc;
}

static u_short ScanBinary(u_char * cp, u_char ** npp, u_short len)
{
    if (*npp)
        NutHeapFree(*npp);
    *npp = NutHeapAlloc(len);
    memcpy(*npp, cp, len);

    return len;
}

static DNSHEADER *CreateDnsHeader(DNSHEADER * doh, u_short id)
{
    if (doh == 0)
        doh = NutHeapAllocClear(sizeof(DNSHEADER));
    if (doh) {
        doh->doh_id = id;
        doh->doh_flags = 0x0100;
        doh->doh_quests = 1;
    }
    return doh;
}

static void ReleaseDnsHeader(DNSHEADER * doh)
{
    if (doh)
        NutHeapFree(doh);
}

static u_short EncodeDnsHeader(u_char * buf, DNSHEADER * doh)
{
    u_short rc;

    rc = AddShort(buf, doh->doh_id);
    rc += AddShort(buf + rc, doh->doh_flags);
    rc += AddShort(buf + rc, doh->doh_quests);
    rc += AddShort(buf + rc, doh->doh_answers);
    rc += AddShort(buf + rc, doh->doh_authrr);
    rc += AddShort(buf + rc, doh->doh_addrr);

    return rc;
}

static u_short DecodeDnsHeader(DNSHEADER * doh, u_char * buf)
{
    u_short rc;

    rc = ScanShort(buf, &doh->doh_id);
    rc += ScanShort(buf + rc, &doh->doh_flags);
    rc += ScanShort(buf + rc, &doh->doh_quests);
    rc += ScanShort(buf + rc, &doh->doh_answers);
    rc += ScanShort(buf + rc, &doh->doh_authrr);
    rc += ScanShort(buf + rc, &doh->doh_addrr);

    return rc;
}

static DNSQUESTION *CreateDnsQuestion(DNSQUESTION * doq, CONST u_char * name, u_short type)
{
    if (doq == 0)
        doq = NutHeapAllocClear(sizeof(DNSQUESTION));
    if (doq) {
        if (doq->doq_name)
            NutHeapFree(doq->doq_name);
        doq->doq_name = NutHeapAlloc(strlen(name) + 1);
        strcpy(doq->doq_name, name);
        doq->doq_type = type;
        doq->doq_class = 1;
    }
    return doq;
}

static void ReleaseDnsQuestion(DNSQUESTION * doq)
{
    if (doq) {
        if (doq->doq_name)
            NutHeapFree(doq->doq_name);
        NutHeapFree(doq);
    }
}

static u_short EncodeDnsQuestion(u_char * buf, DNSQUESTION * doq)
{
    u_short rc;

    rc = AddName(buf, doq->doq_name);
    rc += AddShort(buf + rc, doq->doq_type);
    rc += AddShort(buf + rc, doq->doq_class);

    return rc;
}

static u_short DecodeDnsQuestion(DNSQUESTION * doq, u_char * buf)
{
    u_short rc;

    rc = ScanName(buf, &doq->doq_name);
    rc += ScanShort(buf + rc, &doq->doq_type);
    rc += ScanShort(buf + rc, &doq->doq_class);

    return rc;
}

static DNSRESOURCE *CreateDnsResource(DNSRESOURCE * dor)
{
    if (dor == 0)
        dor = NutHeapAllocClear(sizeof(DNSRESOURCE));
    return dor;
}

static void ReleaseDnsResource(DNSRESOURCE * dor)
{
    if (dor) {
        if (dor->dor_name)
            NutHeapFree(dor->dor_name);
        if (dor->dor_data)
            NutHeapFree(dor->dor_data);
        NutHeapFree(dor);
    }
}

static u_short DecodeDnsResource(DNSRESOURCE * dor, u_char * buf)
{
    u_short rc;

    rc = ScanName(buf, &dor->dor_name);
    rc += ScanShort(buf + rc, &dor->dor_type);
    rc += ScanShort(buf + rc, &dor->dor_class);
    rc += ScanLong(buf + rc, &dor->dor_ttl);
    rc += ScanShort(buf + rc, &dor->dor_len);
    rc += ScanBinary(buf + rc, &dor->dor_data, dor->dor_len);

    return rc;
}

/*!
 * \brief Set DNS configuration.
 *
 * \param hostname DNS name of the local host.
 * \param domain Name of the domain of the local host.
 * \param pdnsip IP address of the primary DNS server.
 * \param sdnsip IP address of the secondary DNS server.
 */
void NutDnsConfig2(u_char * hostname, u_char * domain, u_long pdnsip, u_long sdnsip)
{
    if (doc.doc_hostname) {
        NutHeapFree(doc.doc_hostname);
        doc.doc_hostname = 0;
    }
    if (doc.doc_domain) {
        NutHeapFree(doc.doc_domain);
        doc.doc_domain = 0;
    }
    if (hostname) {
        doc.doc_hostname = NutHeapAlloc(strlen(hostname) + 1);
        strcpy(doc.doc_hostname, hostname);
    }
    if (domain) {
        doc.doc_domain = NutHeapAlloc(strlen(domain) + 1);
        strcpy(doc.doc_domain, domain);
    }
    doc.doc_ip1 = pdnsip;
    doc.doc_ip2 = sdnsip;
}

/*!
 * \brief Sets DNS configuration.
 *
 * \deprecated New applications should use NutDnsConfig2().
 *
 * \param hostname DNS name of the local host.
 * \param domain Name of the domain of the local host.
 * \param dnsip IP address of the DNS server.
 */
void NutDnsConfig(u_char * hostname, u_char * domain, u_long dnsip)
{
    NutDnsConfig2(hostname, domain, dnsip, 0);
}

/*!
 * \brief Retrieves IP-address corresponding to a host name.
 *
 * This is a very simple implementation, which will not return
 * any other resource information than the IP address.
 *
 * \param hostname Fully qualified domain name of the host.
 *
 * \return IP address, which is zero, if the name could not
 *         be resolved.
 */
u_long NutDnsGetResource(CONST u_char * hostname, CONST u_short type);

u_long NutDnsGetHostByName(CONST u_char * hostname)
{
    return NutDnsGetResource(hostname, 1);
}

u_long NutDnsGetMxByDomain(CONST u_char * hostname)
{
    return NutDnsGetResource(hostname, 0x0F);
}

u_long NutDnsGetResource(CONST u_char * hostname, CONST u_short type)
{
    u_long ip = 0;
    u_char *pkt;
    u_short len;
    u_short id = 0;
    UDPSOCKET *sock;
    DNSHEADER *doh = 0;
    DNSQUESTION *doq = 0;
    DNSRESOURCE *dor = 0;
    int n;
    int retries;
    u_long raddr;
    u_short rport;

    /*
     * We need a configured DNS address.
     */
    if (doc.doc_ip1 == 0 && doc.doc_ip2 == 0)
        return 0;

    /*
     * Create client socket and allocate
     * a buffer for the UDP packet.
     */
    if ((sock = NutUdpCreateSocket(0)) == 0)
        return 0;
    pkt = NutHeapAlloc(512);

    for (retries = 0; retries < 6; retries++) {

        /*
         * Create standard header info structures.
         */
        doh = CreateDnsHeader(doh, ++id);
        doq = CreateDnsQuestion(doq, hostname, type);

#ifdef NUTDEBUG
        //DumpDnsHeader(doh);
        //DumpDnsQuestion(doq);
#endif

        /*
         * Encode the header info into the packet buffer
         * and send it to the DNS server.
         */
        len = EncodeDnsHeader(pkt, doh);
        len += EncodeDnsQuestion(pkt + len, doq);

        if ((retries & 1) == 0 || doc.doc_ip2 == 0) {
            if (NutUdpSendTo(sock, doc.doc_ip1, 53, pkt, len) < 0)
                break;
        } else {
            if (NutUdpSendTo(sock, doc.doc_ip2, 53, pkt, len) < 0)
                break;
        }

        /*
         * Loop until we receive a response with the
         * expected id or until timeout.
         */
        for (;;) {
            len = 0;
            n = NutUdpReceiveFrom(sock, &raddr, &rport, pkt, 512, 1000);
            if (n <= 0)
                break;
            if (n > 12) {
                len = DecodeDnsHeader(doh, pkt);
#ifdef NUTDEBUG
                //DumpDnsHeader(doh);
#endif
                if (doh->doh_id == id)
                    break;
            }
        }

        /*
         * Decode the answer.
         */
        if (len && doh->doh_quests == 1) {
            len += DecodeDnsQuestion(doq, pkt + len);
#ifdef NUTDEBUG
            //DumpDnsQuestion(doq);
#endif
            if (doh->doh_answers < 1)
                break;
            else {
                for (n = 1; n <= (int) doh->doh_answers; n++) {
                    dor = CreateDnsResource(dor);
                    len += DecodeDnsResource(dor, pkt + len);
#ifdef NUTDEBUG
                    //DumpDnsResource(dor);
#endif
                    if (dor->dor_type == 1)
                        break;
                }
                if (dor->dor_len == 4) {
                    ip = *((u_long *) (dor->dor_data));
                    break;
                }
                /* TBD: 18.3.2004 - for MX requests authoritative rrs should be skipped + additional rrs should be searched for IP address */
            }
        }
    }

    /*
     * Clean up.
     */
    ReleaseDnsHeader(doh);
    ReleaseDnsQuestion(doq);
    ReleaseDnsResource(dor);

    NutHeapFree(pkt);
    NutUdpDestroySocket(sock);

    return ip;
}

/*@}*/
