#ifndef _VS1001K_H_
#define _VS1001K_H_
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
 * Portions Copyright (C) 2001 Jesper Hansen <jesperh@telia.com>.
 * 
 * This file is part of the yampp system.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/*
 * $Log$
 * Revision 1.3  2004/03/16 16:48:28  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.2  2003/07/13 19:37:39  haraldkipp
 * Enable application to control decoder interrupts.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:10  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.9  2003/05/06 18:44:58  harald
 * Cleanup
 *
 * Revision 1.8  2003/04/21 16:46:53  harald
 * New functions
 *
 * Revision 1.7  2003/04/07 20:08:09  harald
 * Redesigned by Pavel Chromy
 *
 * Revision 1.5  2003/02/04 18:00:37  harald
 * Version 3 released
 *
 * Revision 1.4  2002/10/29 15:40:49  harald
 * *** empty log message ***
 *
 * Revision 1.3  2002/08/16 17:51:19  harald
 * First public release
 *
 * Revision 1.2  2002/06/26 17:29:15  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/nutconfig.h>
#include <sys/types.h>

/*!
 * \file dev/vs1001k.h
 * \brief Network interface controller definitions.
 */

/*!
 * \addtogroup xgVs1001
 */
/*@{*/



/*
 * Instruction opcodes.
 */
#define VS_OPCODE_READ  3
#define VS_OPCODE_WRITE 2

/*
 * Decoder registers.
 */
#define VS_MODE_REG         0
#define VS_STATUS_REG       1
#define VS_INT_FCTLH_REG    2
#define VS_CLOCKF_REG       3
#define VS_DECODE_TIME_REG  4
#define VS_AUDATA_REG       5
#define VS_WRAM_REG         6
#define VS_WRAMADDR_REG     7
#define VS_HDAT0_REG        8
#define VS_HDAT1_REG        9
#define VS_A1ADDR_REG      10
#define VS_VOL_REG         11
#define VS_A1CTRL_REG      13


/*
 * Mode register bits.
 */
#define VS_SM_DIFF      0x0001
#define VS_SM_FFWD      0x0002
#define VS_SM_RESET     0x0004
#define VS_SM_MP12      0x0008
#define VS_SM_PDOWN     0x0010
#define VS_SM_DAC       0x0020
#define VS_SM_DACMONO   0x0040
#define VS_SM_BASS      0x0080
#define VS_SM_DACT      0x0100
#define VS_SM_BYTEORD   0x0200
#define VS_SM_IBMODE    0x0400
#define VS_SM_IBCLK     0x0800


/*
 * Clock register bits.
 */
#define VS_CF_DOUBLER   0x8000


/*
 * Bytes needed to flush internal VS buffer (size of VS interbal buffer)
 */
#define VS_FLUSH_BYTES 2048


/*
 * Status of the decoder
 */
#define VS_STATUS_STOPPED 0
#define VS_STATUS_RUNNING 1
#define VS_STATUS_EOF     2
#define VS_STATUS_EMPTY   4

/*
 * Header info filled by VsGetHeaderInfo.
 */
#ifdef __GNUC__
typedef struct {
    u_short vshi_no_crc:1;
    u_short vshi_layer:2;
    u_short vshi_id:2;
    u_short vshi_syncword:11;

    u_short vshi_emphasis:2;
    u_short vshi_original:1;
    u_short vshi_copyright:1;
    u_short vshi_extension:2;
    u_short vshi_mode:2;
    u_short vshi_private_bit:1;
    u_short vshi_pad_bit:1;
    u_short vshi_sample_rate:2;
    u_short vshi_bitrate:4;
} VS_HEADERINFO;
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int VsPlayerInit(void);
extern int VsPlayerReset(u_short mode);
extern int VsPlayerSetMode(u_short mode);
extern int VsPlayerKick(void);
extern int VsPlayerStop(void);
extern int VsPlayerFlush(void);
extern u_char VsPlayerInterrupts(u_char enable);

extern u_char *VsBufferInit(u_short size);
extern u_char *VsBufferReset(void);
extern u_char *VsBufferRequest(u_short *sizep);
extern u_char *VsBufferAcknowledge(u_short nbytes);

extern u_short VsBufferAvailable(void);
extern u_short VsPlayTime(void);
extern u_char VsGetStatus(void);
#ifdef __GNUC__
extern int VsGetHeaderInfo(VS_HEADERINFO *vshi);
#endif
extern u_short VsMemoryTest(void);

extern int VsSetVolume(u_char left, u_char right);
extern int VsBeep(u_char fsin, u_char ms);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif
