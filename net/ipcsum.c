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
 * Revision 1.1  2003/05/09 14:41:32  haraldkipp
 * Initial revision
 *
 * Revision 1.15  2003/03/31 14:53:27  harald
 * Prepare release 3.1
 *
 * Revision 1.14  2003/02/04 18:14:57  harald
 * Version 3 released
 *
 * Revision 1.13  2002/10/29 15:30:26  harald
 * Temporarly switched back from asm to C
 *
 * Revision 1.12  2002/09/15 16:48:34  harald
 * *** empty log message ***
 *
 * Revision 1.11  2002/08/11 12:39:34  harald
 * ICC Assembler version by Klaus Ummenhofer
 *
 * Revision 1.8  2002/06/26 17:29:36  harald
 * First pre-release with 2.4 stack
 *
 * Revision 1.10  2002/08/08 17:29:28  harald
 * Imagecraft support by Klaus Ummenhofer
 *
 */

#include <netinet/ipcsum.h>

/*!
 * \addtogroup xgIP
 *
 * \brief Internet Protocol checksum and related support.
 *
 *
 * \author Harald Kipp, egnite Software GmbH
 */

/*@{*/


/*!
 * \brief Calculates a partial IP checksum over a block of data.
 *
 * Note that this returns the checksum in network byte order, and thus does
 * not need to be converted via hton16(), etc.  Of course this means that
 * we mustn't use this value for normal arithmetic!
 *
 * This is a partial checksum because it doesn't take the 1's complement
 * of the overall sum.
 *
 * \note by Dave Hudson: This doesn't look particularly intuitive at 
 * first sight - in fact it probably looks plain wrong.  It does work 
 * however (take my word for it), but for some explanation the reader 
 * is referred to RFC1071 where the maths is explained in detail.
 *
 * \note by Harald Kipp: Yes, the GCC version looks wrong. I assume that
 *       this will not work for all packet sizes. Not yet confirmed.
 */
u_short NutIpChkSumPartial(u_short partial_csum, void *buf, u_short count)
{
#ifdef __GNUC__
    u_long sum = partial_csum;
    u_short *d2 = (u_short *) buf;
    while (count > 1) {
        sum += *d2++;
        count -= 2;
    }
    if (count > 0)
        sum += *(u_char *) d2++;

    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    partial_csum = sum;
#if 0
    u_short words;
    void *d1;
    u_short d2;
    d1 = buf;
    words = count >> 1;
    if (words != 0) {
        asm volatile ("\n"
                      "L_redo%=:\n\t"
                      "ld __tmp_reg__, %a1+\n\t"
                      "add %A0, __tmp_reg__\n\t"
                      "ld __tmp_reg__, %a1+\n\t"
                      "adc %B0, __tmp_reg__\n\t"
                      "adc %A0, r1\n\t"
                      "adc %B0, r1\n\t"
                      "dec %A2\n\t"
                      "brne L_redo%=\n\t"
                      "subi %B2, 1\n\t"
                      "brsh L_redo%=\n\t"
                      "\n\t":"=r" (partial_csum), "=e"(d1), "=w"(d2)
                      :"0"(partial_csum), "1"(d1), "2"(words)
            );
    }

    /*
     * Did we have an odd number of bytes to do?
     */
    if (count & 0x01) {
        asm volatile ("ld __tmp_reg__, %a1+\n\t"
                      "add %A0, __tmp_reg__\n\t"
                      "adc %B0, r1\n\t"
                      "adc %A0, r1\n\t":"=r" (partial_csum), "=e"(d1)
                      :"0"(partial_csum), "1"(d1)
            );
    }
#endif
#else

#if 0
    {
        /* Compute Internet Checksum for "count" bytes
         *         beginning at location "addr".
         */
        register long sum = 0;

        /*  This is the inner loop */
        while (count > 1) {
            sum += *(unsigned short) addr++;
            count -= 2;
        }

        /*  Add left-over byte, if any */
        if (count > 0)
            sum += *(unsigned char *) addr;

        /*  Fold 32-bit sum to 16 bits */
        while (sum >> 16)
            sum = (sum & 0xffff) + (sum >> 16);

        checksum = ~sum;
    }
#endif

//*KU* implementation of checksum
// has to be optimized later !

/*	u_long sum = partial_csum;
	u_short *d2 = (u_short*) buf;
	while (count > 1) 
	{
		sum += *d2++;	
		count -= 2;
	}
	if (count > 0)
		sum += *(u_char*) d2++;
		
	while (sum>>16)
		sum = (sum & 0xFFFF) + (sum >>16);

	partial_csum = sum;		*/


/*KU* this should be much faster */
    asm("ldd r24, Y+0");        // pop count from stack
    asm("ldd r25, Y+1");
    asm("clr r1");              // zero reg
    asm("clr r8");              // registers for short sum
    asm("clr r9");
    asm("clr r4");              // registers for long sum
    asm("clr r5");
    asm("clr r6");
    asm("clr r7");
    asm("add r4, r16");         // initialize sum with partial_csum
    asm("adc r5, r17");
    asm("movw r26, r24");       // check count if zero
    asm("subi r26, 0");
    asm("sbci r27, 0");
    asm("movw r8, r16");        // returnvalue if exit here
    asm("breq NutIPChk_Exit");
    asm("subi r26, 1");         // check count = 1
    asm("sbci r27, 0");
    asm("movw r30, r18");       // load buffer address
    asm("breq NutIPChk_OneByte");

    asm("NutIPChk_Start:");
    asm("ld r8, Z+");           // load value from buf
    asm("ld r9, Z+");
    asm("add r4, r8");          // add to long sum
    asm("adc r5, r9");
    asm("adc r6, r1");
    asm("adc r7, r1");
    asm("subi r24, 2");         // count - 2; next word
    asm("sbci r25, 0");
    asm("breq NutIPChk_Fold");
    asm("movw r26, r24");       // check count = 1
    asm("subi r26, 1");
    asm("sbci r27, 0");
    asm("breq NutIPChk_OneByte");
    asm("rjmp NutIPChk_Start"); // there are more words

    asm("NutIPChk_OneByte:");   // no word, only one byte left
    asm("ld r8, Z");            // load last byte
    asm("add r4, r8");          // add it to long sum
    asm("adc r5, r1");
    asm("adc r6, r1");
    asm("adc r7, r1");

    asm("NutIPChk_Fold:");      // see the while loop in the
    asm("movw r8, r4");         // source code above if you
    asm("add r8, r6");          // want to understand this
    asm("adc r9, r7");
    asm("brcc NutIPChk_Exit");  // fold again if sum>>16 = true
    asm("ldi r26, 1");
    asm("add r8, r26");
    asm("adc r9, r1");

    asm("NutIPChk_Exit:");
    asm("movw %partial_csum, r8");      // pass the return value

#endif

    return partial_csum;
}



/*!
 * \brief Calculates an the final IP checksum over a block of data.
 *
 * Unlike the partial checksum in NutIpChkSumPartial(), this function takes
 * the one's complement of the final result, thus making it the full checksum.
 */
u_short NutIpChkSum(u_short partial_csum, void *buf, u_short count)
{
    return NutIpChkSumPartial(partial_csum, buf, count) ^ 0xffff;
}

/*
 * Details of the pseudo header used as part of the
 * calculation of UDP and TCP header checksums.
 */
struct pseudo_hdr {
    u_long ph_src_addr;
    u_long ph_dest_addr;
    u_char ph_zero;
    u_char ph_protocol;
    u_short ph_len;
};

/*!
 * \brief Calculates the partial IP pseudo checksum.
 *
 */
u_long NutIpPseudoChkSumPartial(u_long src_addr, u_long dest_addr,
                                 u_char protocol, u_short len)
{
    struct pseudo_hdr ph;

    ph.ph_src_addr = src_addr;
    ph.ph_dest_addr = dest_addr;
    ph.ph_zero = 0;
    ph.ph_protocol = protocol;
    ph.ph_len = len;

    return NutIpChkSumPartial(0, &ph, sizeof(struct pseudo_hdr));
}

/*!
 * \brief Calculates the IP pseudo checksum.
 *
 */
u_short NutIpPseudoChkSum(u_long src_addr, u_long dest_addr,
                          u_char protocol, u_short len)
{
    return NutIpPseudoChkSumPartial(src_addr, dest_addr, protocol,
                                    len) ^ 0xffff;
}

/*@}*/
