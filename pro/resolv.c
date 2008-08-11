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
 * Revision 1.14  2008/08/11 07:00:35  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.13  2008/02/15 17:07:09  haraldkipp
 * Added routine to query DNS IP settings.
 *
 * Revision 1.12  2006/10/08 16:48:22  haraldkipp
 * Documentation fixed
 *
 * Revision 1.11  2006/03/16 15:25:39  haraldkipp
 * Changed human readable strings from u_char to char to stop GCC 4 from
 * nagging about signedness.
 *
 * Revision 1.10  2006/01/23 19:52:10  haraldkipp
 * Added required typecasts before left shift.
 *
 * Revision 1.9  2006/01/23 17:33:47  haraldkipp
 * Avoid memory alignment errors.
 *
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
    uint8_t *doc_hostname;
    uint8_t *doc_domain;
    uint32_t doc_ip1;
    uint32_t doc_ip2;
} DNSCONFIG;

static DNSCONFIG doc;

typedef struct {
    uint16_t doh_id;
    uint16_t doh_flags;
    uint16_t doh_quests;
    uint16_t doh_answers;
    uint16_t doh_authrr;
    uint16_t doh_addrr;
} DNSHEADER;

typedef struct {
    uint8_t *doq_name;
    uint16_t doq_type;
    uint16_t doq_class;
} DNSQUESTION;

typedef struct {
    uint8_t *dor_name;
    uint16_t dor_type;
    uint16_t dor_class;
    uint32_t dor_ttl;
    uint16_t dor_len;
    uint8_t *dor_data;
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
    uint16_t i;

    fprintf(stream, "RESOURCE: name='%s' type=%u class=%u ttl=%lu len=%u ",
            dor->dor_name, dor->dor_type, dor->dor_class, dor->dor_ttl, dor->dor_len);
    for (i = 0; i < dor->dor_len; i++)
        fprintf(stream, "%02X ", dor->dor_data[i]);
    fputc('\n', stream);
}
#endif

static uint16_t AddShort(uint8_t * cp, uint16_t val)
{
    *cp++ = (uint8_t) (val >> 8);
    *cp++ = (uint8_t) val;

    return 2;
}

static uint16_t AddName(uint8_t * cp, CONST uint8_t * name)
{
    uint8_t *lcp;
    uint16_t rc = strlen((char *)name) + 2;

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

static uint16_t ScanShort(uint8_t * cp, uint16_t * val)
{
    *val = (uint16_t)(*cp++) << 8;
    *val |= *cp;

    return 2;
}

static uint16_t ScanLong(uint8_t * cp, uint32_t * val)
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

static uint16_t ScanName(uint8_t * cp, uint8_t ** npp)
{
    uint8_t len;
    uint16_t rc;
    uint8_t *np;

    if (*npp) {
        NutHeapFree(*npp);
        *npp = 0;
    }

    if ((*cp & 0xC0) == 0xC0)
        return 2;

    rc = strlen((char *)cp) + 1;
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

static uint16_t ScanBinary(uint8_t * cp, uint8_t ** npp, uint16_t len)
{
    if (*npp)
        NutHeapFree(*npp);
    *npp = NutHeapAlloc(len);
    memcpy(*npp, cp, len);

    return len;
}

static DNSHEADER *CreateDnsHeader(DNSHEADER * doh, uint16_t id)
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

static uint16_t EncodeDnsHeader(uint8_t * buf, DNSHEADER * doh)
{
    uint16_t rc;

    rc = AddShort(buf, doh->doh_id);
    rc += AddShort(buf + rc, doh->doh_flags);
    rc += AddShort(buf + rc, doh->doh_quests);
    rc += AddShort(buf + rc, doh->doh_answers);
    rc += AddShort(buf + rc, doh->doh_authrr);
    rc += AddShort(buf + rc, doh->doh_addrr);

    return rc;
}

static uint16_t DecodeDnsHeader(DNSHEADER * doh, uint8_t * buf)
{
    uint16_t rc;

    rc = ScanShort(buf, &doh->doh_id);
    rc += ScanShort(buf + rc, &doh->doh_flags);
    rc += ScanShort(buf + rc, &doh->doh_quests);
    rc += ScanShort(buf + rc, &doh->doh_answers);
    rc += ScanShort(buf + rc, &doh->doh_authrr);
    rc += ScanShort(buf + rc, &doh->doh_addrr);

    return rc;
}

static DNSQUESTION *CreateDnsQuestion(DNSQUESTION * doq, CONST uint8_t * name, uint16_t type)
{
    if (doq == 0)
        doq = NutHeapAllocClear(sizeof(DNSQUESTION));
    if (doq) {
        if (doq->doq_name)
            NutHeapFree(doq->doq_name);
        doq->doq_name = NutHeapAlloc(strlen((char *)name) + 1);
        strcpy((char *)doq->doq_name, (char *)name);
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

static uint16_t EncodeDnsQuestion(uint8_t * buf, DNSQUESTION * doq)
{
    uint16_t rc;

    rc = AddName(buf, doq->doq_name);
    rc += AddShort(buf + rc, doq->doq_type);
    rc += AddShort(buf + rc, doq->doq_class);

    return rc;
}

static uint16_t DecodeDnsQuestion(DNSQUESTION * doq, uint8_t * buf)
{
    uint16_t rc;

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

static uint16_t DecodeDnsResource(DNSRESOURCE * dor, uint8_t * buf)
{
    uint16_t rc;

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
void NutDnsConfig2(uint8_t * hostname, uint8_t * domain, uint32_t pdnsip, uint32_t sdnsip)
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
        doc.doc_hostname = NutHeapAlloc(strlen((char *)hostname) + 1);
        strcpy((char *)doc.doc_hostname, (char *)hostname);
    }
    if (domain) {
        doc.doc_domain = NutHeapAlloc(strlen((char *)domain) + 1);
        strcpy((char *)doc.doc_domain, (char *)domain);
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
void NutDnsConfig(uint8_t * hostname, uint8_t * domain, uint32_t dnsip)
{
    NutDnsConfig2(hostname, domain, dnsip, 0);
}

void NutDnsGetConfig2(char ** hostname, char ** domain, uint32_t *pdnsip, uint32_t *sdnsip)
{
    if (hostname) {
        *hostname = (char *)doc.doc_hostname;
    }
    if (domain) {
        *domain = (char *)doc.doc_domain;
    }
    if (pdnsip) {
        *pdnsip = doc.doc_ip1;
    }
    if (sdnsip) {
        *sdnsip = doc.doc_ip2;
    }
}

/*!
 * \brief Retrieves IP-address corresponding to a host name.
 *
 * This is a very simple implementation, which will not return
 * any other resource information than the IP address.
 *
 * \param hostname Fully qualified domain name of the host.
 * \param type     Request type.
 *
 * \return IP address, which is zero, if the name could not
 *         be resolved.
 */
uint32_t NutDnsGetResource(CONST uint8_t * hostname, CONST uint16_t type);

uint32_t NutDnsGetHostByName(CONST uint8_t * hostname)
{
    return NutDnsGetResource(hostname, 1);
}

uint32_t NutDnsGetMxByDomain(CONST uint8_t * hostname)
{
    return NutDnsGetResource(hostname, 0x0F);
}

uint32_t NutDnsGetResource(CONST uint8_t * hostname, CONST uint16_t type)
{
    uint32_t ip = 0;
    uint8_t *pkt;
    uint16_t len;
    uint16_t id = 0;
    UDPSOCKET *sock;
    DNSHEADER *doh = 0;
    DNSQUESTION *doq = 0;
    DNSRESOURCE *dor = 0;
    int n;
    int retries;
    uint32_t raddr;
    uint16_t rport;

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
                    ip = *dor->dor_data;
                    ip += (uint32_t)(*(dor->dor_data + 1)) << 8;
                    ip += (uint32_t)(*(dor->dor_data + 2)) << 16;
                    ip += (uint32_t)(*(dor->dor_data + 3)) << 24;
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
