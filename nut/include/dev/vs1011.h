#ifndef _DEV_VS1011_H_
#define _DEV_VS1011_H_
/*
 * Copyright (C) 2009 by egnite GmbH
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
 */

/*
 * $Id$
 */

#define VS_HAS_SM_LAYER12       0
#define VS_HAS_SM_FFWD          0
#define VS_HAS_SM_MP12          0
#define VS_HAS_SM_OUTOFWAV      1
#define VS_HAS_SM_CANCEL        0
#define VS_HAS_SM_PDOWN         0
#define VS_HAS_SM_EARSPEAKER    0
#define VS_HAS_SM_DAC           0
#define VS_HAS_SM_TESTS         1
#define VS_HAS_SM_DACMONO       0
#define VS_HAS_SM_STREAM        1
#define VS_HAS_SM_BASS          0
#define VS_HAS_SM_IBMODE        0
#define VS_HAS_SM_SDISHARE      1
#define VS_HAS_SM_IBCLK         0
#define VS_HAS_SM_SDINEW        1
#define VS_HAS_SM_ADPCM         0
#define VS_HAS_SM_ADPCM_HP      0
#define VS_HAS_SM_LINE_IN       0
#define VS_HAS_SM_CLK_RANGE     0

#define VS_HAS_SS_AVOL          1
#define VS_HAS_SS_VREF_1_65     0
#define VS_HAS_SS_AD_CLOCK      0
#define VS_HAS_SS_VCM_DISABLE   0
#define VS_HAS_SS_VCM_OVERLOAD  0
#define VS_HAS_SS_SWING         0
#define VS_HAS_SS_DO_NOT_JUMP   0

#define VS_HAS_INT_FCTLH_REG    0
#define VS_HAS_BASS_REG         1
#define VS_HAS_SC_FREQ          1
#define VS_HAS_SC_X3FREQ        0
#define VS_HAS_SC_ADD           0
#define VS_HAS_SC_ADD2          0
#define VS_HAS_SC_MULT          0
#define VS_HAS_SC_MULT5         0
#define VS_HAS_CF_DOUBLER       1

#define VS_HAS_AD_STEREO_BIT0   1
#define VS_HAS_AD_SAMRATE       1
#define VS_HAS_AD_BITRATE       0
#define VS_HAS_AD_SAMRATE_IDX   0

#define VS_HAS_HDAT0_REG        1
#define VS_HAS_IN0_REG          0
#define VS_HAS_HDAT1_REG        1
#define VS_HAS_IN1_REG          0
#define VS_HAS_MIXERVOL_REG     0
#define VS_HAS_ADPCMRECCTL_REG  0
#define VS_HAS_AICTRL0_REG      1
#define VS_HAS_AICTRL_REG       0
#define VS_HAS_AICTRL1_REG      1
#define VS_HAS_AICTRL2_REG      1
#define VS_HAS_AICTRL3_REG      1

#define VS_FLUSH_BYTES          1024

#define VS_XDATA_USR_WRAM_START 0x1380
#define VS_XDATA_USR_RAM_SIZE   128
#define VS_YDATA_USR_WRAM_START 0x4780
#define VS_YDATA_USR_RAM_SIZE   128
#define VS_I_USR_WRAM_START     0x8030
#define VS_I_USR_RAM_SIZE       1232
#define VS_IO_START             0xC000
#define VS_IO_SIZE              16384

#define VS_DECODER_CAPS         (AUDIO_FMT_WAV_PCM | AUDIO_FMT_MPEG1_L1 | AUDIO_FMT_MPEG1_L2 | AUDIO_FMT_MPEG1_L3)
#define VS_ENCODER_CAPS         (0)

#endif

