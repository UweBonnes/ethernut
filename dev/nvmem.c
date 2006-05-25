/*
 * Copyright (C) 2006 by egnite Software GmbH. All rights reserved.
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

/*!
 * \file dev/nvmem.c
 * \brief Non-volatile memory access.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.3  2006/05/25 09:09:57  haraldkipp
 * API documentation updated and corrected.
 *
 * Revision 1.2  2006/04/07 12:29:42  haraldkipp
 * AT49BV flash memory support added. A single sector may be used to
 * store system configurations in case there is no EEPROM available.
 *
 * Revision 1.1  2006/01/23 17:26:19  haraldkipp
 * Platform independant routines added, which provide generic access to
 * non-volatile memory.
 *
 *
 * \endverbatim
 */

#include <dev/nvmem.h>

#if defined(NUT_CONFIG_X12RTC)
#include <dev/x12rtc.h>
#elif defined(NUT_CONFIG_AT49BV)
#include <dev/at49bv.h>
#endif

/*!
 * \addtogroup xgNutNvMem
 */
/*@{*/

/*!
 * \brief Read data from non-volatile memory.
 *
 * This routine provides platform independent access to non-volatile
 * configuration data.
 *
 * \param addr Location to read from.
 * \param buff Pointer to a buffer that receives the data.
 * \param siz  Number of bytes to read.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutNvMemLoad(u_int addr, void *buff, size_t siz)
{
#if defined(NUT_CONFIG_X12RTC)
    return X12EepromRead(addr, buff, siz);
#elif defined(NUT_CONFIG_AT49BV)
    return At49bvParamRead(addr, buff, siz);
#elif defined(__AVR__)
    return OnChipNvMemLoad(addr, buff, siz);
#else
    return -1;
#endif
}

/*!
 * \brief Save data in non-volatile memory.
 *
 * This routine provides platform independent access to non-volatile
 * configuration data.
 *
 * \param addr Location to write to.
 * \param buff Pointer to a buffer that contains the data.
 * \param len  Number of bytes to write.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutNvMemSave(u_int addr, CONST void *buff, size_t len)
{
#if defined(NUT_CONFIG_X12RTC)
    return X12EepromWrite(addr, buff, len);
#elif defined(NUT_CONFIG_AT49BV)
    return At49bvParamWrite(addr, buff, len);
#elif defined(__AVR__)
    return OnChipNvMemSave(addr, buff, len);
#else
    return -1;
#endif
}


/*@}*/
