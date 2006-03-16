#ifndef _SYS_CONFOS_H_
#define _SYS_CONFOS_H_

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
 * Revision 1.5  2006/03/16 15:25:34  haraldkipp
 * Changed human readable strings from u_char to char to stop GCC 4 from
 * nagging about signedness.
 *
 * Revision 1.4  2006/01/23 17:34:29  haraldkipp
 * Configuration structures must be packed.
 *
 * Revision 1.3  2005/07/26 16:02:57  haraldkipp
 * Avoid redefinition of CONFOS_EE_OFFSET.
 *
 * Revision 1.2  2004/03/03 17:52:25  drsung
 * New field 'hostname' added to structure confos.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:19  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.4  2003/02/04 18:00:52  harald
 * Version 3 released
 *
 * Revision 1.3  2002/06/26 17:29:28  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/types.h>
#include <cfg/eeprom.h>

/*!
 * \file sys/confos.h
 * \brief Nut/OS configuration.
 */

#ifndef CONFOS_EE_OFFSET
#define CONFOS_EE_OFFSET    0
#endif

#define CONFOS_EE_MAGIC     "OS"

/*!
 * \brief Operating system configuration type.
 */
typedef struct __attribute__ ((packed)) _CONFOS CONFOS;

/*!
 * \struct _CONFOS confos.h sys/confos.h
 * \brief Operating system configuration structure.
 *
 */
struct __attribute__ ((packed)) _CONFOS {
    u_char size;            /*!< \brief Size of this structure. */
    u_char magic[2];        /*!< \brief Magic cookie. */
    char hostname[16];      /*!< \brief host name of the system. */
};

extern CONFOS confos;

__BEGIN_DECLS
/* Function prototypes */

extern int NutLoadConfig(void);
extern int NutSaveConfig(void);

__END_DECLS
/* End of prototypes */

#endif
