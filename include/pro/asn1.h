#ifndef PRO_ASN1_H
#define PRO_ASN1_H

/*
 * Copyright 1998-2007 by egnite Software GmbH
 * Copyright 1988, 1989, 1991, 1992 by Carnegie Mellon University
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
 * \file pro/asn1.h
 * \brief Definitions for Abstract Syntax Notation One, ASN.1.
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

#include <sys/types.h>

#ifndef MAX_OID_LEN
#define MAX_OID_LEN     32
#endif

#define MAX_SUBID   0xFFFFFFFF

#define MIN_OID_LEN     2

#define ASN_BOOLEAN     0x01
#define ASN_INTEGER     0x02
#define ASN_BIT_STR     0x03
#define ASN_OCTET_STR   0x04
#define ASN_NULL        0x05
#define ASN_OBJECT_ID   0x06
#define ASN_SEQUENCE    0x10
#define ASN_SET         0x11

#define ASN_UNIVERSAL   0x00
#define ASN_APPLICATION 0x40
#define ASN_CONTEXT     0x80
#define ASN_PRIVATE     0xC0

#define ASN_PRIMITIVE   0x00
#define ASN_CONSTRUCTOR 0x20

#define ASN_LONG_LEN        0x80
#define ASN_EXTENSION_ID    0x1F

/* RFC 1157. */
#define ASN_IPADDRESS   (ASN_APPLICATION | 0)
#define ASN_COUNTER     (ASN_APPLICATION | 1)
#define ASN_GAUGE       (ASN_APPLICATION | 2)
#define ASN_UNSIGNED    (ASN_APPLICATION | 2)
#define ASN_TIMETICKS   (ASN_APPLICATION | 3)
#define ASN_OPAQUE      (ASN_APPLICATION | 4)

/*RFC 1442. */ 
#define ASN_NSAP        (ASN_APPLICATION | 5)
#define ASN_COUNTER64   (ASN_APPLICATION | 6)
#define ASN_UINTEGER    (ASN_APPLICATION | 7)

#define ACL_RONLY       0xAAAA  /* read access for everyone */
#define ACL_RWRITE      0xAABA  /* add write access for community private */
#define ACL_NOACCESS    0x0000  /* no access for anybody */

#define ASN_BIT8        0x80

typedef u_long OID;

/* 
 * Internal 64 bit representation.
 */
typedef struct {
    u_long high;
    u_long low;
} UNSIGNED64;

extern const u_char *AsnHeaderParse(const u_char *, size_t *, u_char *);
extern u_char *AsnHeaderBuild(u_char *, size_t *, u_char, size_t);

extern const u_char * AsnSequenceParse(const u_char *, size_t *, u_char);
extern u_char *AsnSequenceBuild(u_char *, size_t *, u_char, size_t);

extern const u_char *AsnIntegerParse(const u_char *, size_t *, u_char *, long *);
extern u_char *AsnIntegerBuild(u_char *, size_t *, u_char, long *);

extern const u_char *AsnUnsignedParse(const u_char *, size_t *, u_char *, u_long *);
extern u_char *AsnUnsignedBuild(u_char *, size_t *, u_char, u_long *);

extern const u_char *AsnOctetStringParse(const u_char *, size_t *, u_char *, u_char *, size_t *);
extern u_char *AsnOctetStringBuild(u_char *, size_t *, u_char, const u_char *, size_t);

extern const u_char *AsnOidParse(const u_char *, size_t *, u_char *, OID *, size_t *);
extern u_char *AsnOidBuild(u_char *, size_t *, u_char, const OID *, size_t);

extern const u_char *AsnNullParse(const u_char *, size_t *, u_char *);
extern u_char *AsnNullBuild(u_char *, size_t *, u_char);

extern const u_char *AsnBitStringParse(const u_char *, size_t *, u_char *, u_char *, size_t *);
extern u_char *AsnBitStringBuild(u_char *, size_t *, u_char, const u_char *, size_t);

extern const u_char *AsnUnsigned64Parse(const u_char *, size_t *, u_char *, UNSIGNED64 *);
extern u_char *AsnUnsigned64Build(u_char *, size_t *, u_char, const UNSIGNED64 *);


#endif
