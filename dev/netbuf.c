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
 * -
 * Portions Copyright (C) 2000 David J. Hudson <dave@humbug.demon.co.uk>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "copying-gpl.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "copying-liquorice.txt" for details.
 * -
 * Portions Copyright (c) 1983, 1993 by
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * -
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/*
 * $Log$
 * Revision 1.1  2003/05/09 14:40:46  haraldkipp
 * Initial revision
 *
 * Revision 1.12  2003/03/31 14:53:07  harald
 * Prepare release 3.1
 *
 * Revision 1.11  2003/02/04 17:50:54  harald
 * Version 3 released
 *
 * Revision 1.10  2002/06/26 17:29:08  harald
 * First pre-release with 2.4 stack
 *
 */

#include <string.h>

#include <sys/heap.h>
#include <dev/netbuf.h>

#ifdef NUTDEBUG
#include <sys/osdebug.h>
#endif

/*!
 * \addtogroup xgnetbuf
 */
/*@{*/

static int NutNetBufAllocData(NBDATA * nbd, u_short size)
{
    if ((nbd->vp = NutHeapAlloc(size)) == 0) {
        nbd->sz = 0;
        return -1;
    }
    nbd->sz = size;
    return 0;
}

static void NutNetBufFreeData(NBDATA * nbd)
{
    NutHeapFree(nbd->vp);
    nbd->vp = 0;
    nbd->sz = 0;
}

/*!
 * \brief Allocate or re-allocate a network buffer part.
 *
 * \param nb   Points to an existing network buffer structure or NULL, 
 *             if a new structure should be created. An existing buffer
 *             must not be used any further if this function returns
 *             a null pointer.
 * \param type Part of the buffer to be allocated. This can be any of
 *             the following:
 *             - NBAF_DATALINK
 *             - NBAF_NETWORK
 *             - NBAF_TRANSPORT
 *             - NBAF_APPLICATION
 * \param size Size of the part to be allocated.
 *
 * \return Pointer to the allocated network buffer structure. A null
 *         pointer is returned if not enough memory is available and
 *         the whole structure is released.
 */
NETBUF *NutNetBufAlloc(NETBUF * nb, u_char type, u_short size)
{
    if (nb == 0) {
        nb = NutHeapAllocClear(sizeof(NETBUF));
    }

    if (nb && type) {
        if (size) {
            if (type & NBAF_DATALINK) {
                if (nb->nb_flags & NBAF_DATALINK) {
                    if (nb->nb_dl.sz < size) {
                        NutNetBufFreeData(&nb->nb_dl);
                        if (NutNetBufAllocData(&nb->nb_dl, size)) {
                            NutNetBufFree(nb);
                            return 0;
                        }
                    }
                } else if (NutNetBufAllocData(&nb->nb_dl, size)) {
                    NutNetBufFree(nb);
                    return 0;
                }
            }

            if (type & NBAF_NETWORK) {
                if (nb->nb_flags & NBAF_NETWORK) {
                    if (nb->nb_nw.sz < size) {
                        NutNetBufFreeData(&nb->nb_nw);
                        if (NutNetBufAllocData(&nb->nb_nw, size)) {
                            NutNetBufFree(nb);
                            return 0;
                        }
                    }
                } else if (NutNetBufAllocData(&nb->nb_nw, size)) {
                    NutNetBufFree(nb);
                    return 0;
                }
            }

            if (type & NBAF_TRANSPORT) {
                if (nb->nb_flags & NBAF_TRANSPORT) {
                    if (nb->nb_tp.sz < size) {
                        NutNetBufFreeData(&nb->nb_tp);
                        if (NutNetBufAllocData(&nb->nb_tp, size)) {
                            NutNetBufFree(nb);
                            return 0;
                        }
                    }
                } else if (NutNetBufAllocData(&nb->nb_tp, size)) {
                    NutNetBufFree(nb);
                    return 0;
                }
            }

            if (type & NBAF_APPLICATION) {
                if (nb->nb_flags & NBAF_APPLICATION) {
                    if (nb->nb_ap.sz < size) {
                        NutNetBufFreeData(&nb->nb_ap);
                        if (NutNetBufAllocData(&nb->nb_ap, size)) {
                            NutNetBufFree(nb);
                            return 0;
                        }
                    }
                } else if (NutNetBufAllocData(&nb->nb_ap, size)) {
                    NutNetBufFree(nb);
                    return 0;
                }
            }
            nb->nb_flags |= type;
        } else {
            type &= nb->nb_flags;
            if (type & NBAF_DATALINK)
                NutNetBufFreeData(&nb->nb_dl);
            if (type & NBAF_NETWORK)
                NutNetBufFreeData(&nb->nb_nw);
            if (type & NBAF_TRANSPORT)
                NutNetBufFreeData(&nb->nb_tp);
            if (type & NBAF_APPLICATION)
                NutNetBufFreeData(&nb->nb_ap);
            nb->nb_flags &= ~type;
        }
    }
    return nb;
}

/*!
 * \brief Create a copy of an existing network buffer
 *        structure.
 *
 * \param nb Points to an existing network buffer
 *           structure, previously allocated by
 *           NutNetBufAlloc().
 *
 * \return Pointer to a newly allocated copy.
 */
NETBUF *NutNetBufClone(NETBUF * nb)
{
    NETBUF *clone;

    if ((clone = NutHeapAllocClear(sizeof(NETBUF))) == 0)
        return 0;

    if (nb->nb_dl.sz) {
        if (NutNetBufAllocData(&clone->nb_dl, nb->nb_dl.sz)) {
            NutNetBufFree(clone);
            return 0;
        }
        memcpy(clone->nb_dl.vp, nb->nb_dl.vp, nb->nb_dl.sz);
        clone->nb_flags |= NBAF_DATALINK;
    }
    if (nb->nb_nw.sz) {
        if (NutNetBufAllocData(&clone->nb_nw, nb->nb_nw.sz)) {
            NutNetBufFree(clone);
            return 0;
        }
        memcpy(clone->nb_nw.vp, nb->nb_nw.vp, nb->nb_nw.sz);
        clone->nb_flags |= NBAF_NETWORK;
    }
    if (nb->nb_tp.sz) {
        if (NutNetBufAllocData(&clone->nb_tp, nb->nb_tp.sz)) {
            NutNetBufFree(clone);
            return 0;
        }
        memcpy(clone->nb_tp.vp, nb->nb_tp.vp, nb->nb_tp.sz);
        clone->nb_flags |= NBAF_TRANSPORT;
    }
    if (nb->nb_ap.sz) {
        if (NutNetBufAllocData(&clone->nb_ap, nb->nb_ap.sz)) {
            NutNetBufFree(clone);
            return 0;
        }
        memcpy(clone->nb_ap.vp, nb->nb_ap.vp, nb->nb_ap.sz);
        clone->nb_flags |= NBAF_APPLICATION;
    }
    return clone;
}

/*!
 * \brief Release a network buffer structure.
 *
 * Returns all memory previously allocated by a
 * network buffer to the available heap space.
 *
 * \param nb Points to an existing network buffer
 *           structure, previously allocated by
 *           NutNetBufAlloc().
 *
 * \return 0 if successfull or -1 if the structure
 *         contains previously released memory space.
 */
int NutNetBufFree(NETBUF * nb)
{
    if(nb) {
        NutNetBufAlloc(nb, nb->nb_flags, 0);
        if (NutHeapFree(nb)) {
            return -1;
        }
    }
    return 0;
}

/*@}*/
