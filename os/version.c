/*
 * Copyright (C) 2001-2005 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.26  2006/01/22 17:46:18  haraldkipp
 * Pre-release 4.1.3
 *
 * Revision 1.25  2006/01/19 18:39:33  haraldkipp
 * Pre-release 4.1.2.
 *
 * Revision 1.24  2006/01/08 11:55:52  haraldkipp
 * Prepare preview snapshot.
 *
 * Revision 1.23  2005/12/04 18:42:21  haraldkipp
 * Release branch X.Y created.
 *
 * Revision 1.22  2005/12/02 11:36:12  haraldkipp
 * Release 4.0 scheduled
 *
 * Revision 1.21  2005/11/20 14:36:07  haraldkipp
 * Preparing last version 4 preview.
 *
 * Revision 1.20  2005/08/02 17:47:04  haraldkipp
 * Major API documentation update.
 *
 * Revision 1.19  2005/07/20 09:14:40  haraldkipp
 * Release 3.9.8
 *
 * Revision 1.18  2005/06/06 10:40:31  haraldkipp
 * Emergency bugfix release 3.9.7.1.
 *
 * Revision 1.17  2005/05/25 10:04:58  haraldkipp
 * Win 32 release.
 *
 * Revision 1.16  2005/05/24 16:12:51  haraldkipp
 * Sigh!
 *
 * Revision 1.15  2005/05/22 16:05:40  haraldkipp
 * *** empty log message ***
 *
 * Revision 1.14  2005/04/05 17:37:12  haraldkipp
 * First Nut/OS running ARM responds to http requests
 *
 * Revision 1.13  2005/02/07 15:24:46  haraldkipp
 * ARP cache bugfix.
 *
 * Revision 1.12  2005/02/06 16:35:33  haraldkipp
 * *** empty log message ***
 *
 * Revision 1.11  2005/01/02 10:07:10  haraldkipp
 * Replaced platform dependant formats in debug outputs.
 *
 * Revision 1.10  2004/10/03 18:44:40  haraldkipp
 * A little bit of Game Boy
 *
 * Revision 1.9  2004/09/08 09:55:19  haraldkipp
 * Although this is no final release, there is a lot of new hardware waiting
 * for Nut/OS support. So I marked this with version 3.9.1 before creating
 * the long awaited 4.0 branch.
 *
 * Revision 1.8  2004/01/30 11:50:26  haraldkipp
 * Release branch 3.4 created
 *
 * Revision 1.7  2004/01/27 13:41:52  haraldkipp
 * Branch 3.4 announced
 *
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

#define OS_VERSION_NUMBER  0x04010300UL
static CONST char os_version_string[] = "4.1.3.0 pre";

/*!
 * \addtogroup xgNutVersion
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
