#ifndef _CONFIG_H
#define _CONFIG_H

/*
 * Copyright (C) 2005-2007 by egnite Software GmbH
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
 *
 */

/*
 * $Id$
 */

#include <stddef.h>

/*!
 * \brief Network configuration structure.
 */
typedef struct __attribute__ ((packed)) _CONFNET {
    unsigned char cd_size;      /*!< \brief Size of this structure. */
    unsigned char cd_name[9];   /*!< \brief Magic cookie. */
    unsigned char cdn_mac[6];   /*!< \brief Ethernet MAC address. */
    unsigned long cdn_ip_addr;  /*!< \brief Last used IP address. */
    unsigned long cdn_ip_mask;  /*!< \brief IP netmask. */
    unsigned long cdn_gateway;  /*!< \brief Default route. */
    unsigned long cdn_cip_addr; /*!< \brief Configured IP address. */
} CONFNET;

extern CONFNET confnet;

/*!
 * \brief Boot configuration structure.
 */
typedef struct __attribute__ ((packed)) _CONFBOOT {
    unsigned char cb_size;      /*!< \brief Size of this structure. */
    unsigned char cb_flags;
    unsigned long cb_tftp_ip;
    unsigned char cb_image[58];
} CONFBOOT;

extern CONFBOOT confboot;

extern int BootConfigRead(void);
extern void BootConfigWrite(void);

#endif
