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
 */

/*
 * $Log$
 * Revision 1.6  2003/12/15 19:32:32  haraldkipp
 * Preview
 *
 * Revision 1.5  2003/11/04 17:59:04  haraldkipp
 * Corrected
 *
 * Revision 1.4  2003/11/03 16:28:20  haraldkipp
 * Release version 3.3.2
 *
 * Revision 1.3  2003/08/14 15:22:24  haraldkipp
 * New release
 *
 * Revision 1.2  2003/07/21 18:26:31  haraldkipp
 * Version 3.3
 *
 * Revision 1.1.1.1  2003/05/09 14:41:55  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.3  2003/05/06 18:58:16  harald
 * Prepare final release
 *
 * Revision 1.2  2003/03/31 14:40:57  harald
 * *** empty log message ***
 *
 * Revision 1.1  2003/02/04 18:17:07  harald
 * Version 3 released
 *
 */

#include <sys/version.h>

#define OS_VERSION_NUMBER  0x03030301UL
static CONST char os_version_string[] = "3.3.3.1pre";

/*!
 * \addtogroup xgNutInit
 */
/*@{*/


/*!
 * \brief Return Nut/OS version.
 *
 * \return Version coded into an unsigned long. Most significant byte
 *         is the major version number, followed by the minor version
 *         number, again followed by the release number and finally
 *         the build number in the least significant byte.
 */
u_long NutVersion(void)
{
    return OS_VERSION_NUMBER;
}

/*!
 * \brief Return Nut/OS version string.
 *
 * \return Pointer to a printable string containing the version number.
 *         The string contains 4 numbers separated by dots. The first
 *         is the major version number, followed by the minor version
 *         number, again followed by the release number and finally
 *         followed by the build number. In non-official releases this 
 *         may be optionally followed by a space and an additional 
 *         release state.
 */
CONST char *NutVersionString(void)
{
    return os_version_string;
}

/*@}*/
