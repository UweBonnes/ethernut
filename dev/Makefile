#
# Copyright (C) 2001-2004 by egnite Software GmbH. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this
#    software must display the following acknowledgement:
#
#    This product includes software developed by egnite Software GmbH
#    and its contributors.
#
# THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
# ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
# SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
# THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# For additional information see http://www.ethernut.de/
#
# $Log: Makefile,v $
# Revision 1.8  2004/03/08 11:16:54  haraldkipp
# Asynchronous HDLC driver added
#
# Revision 1.7  2004/01/30 17:02:19  drsung
# Separate interrupt stack for avr-gcc only added.
#
# Revision 1.6  2003/12/15 19:24:49  haraldkipp
# New USART driver added
#
# Revision 1.5  2003/11/23 15:05:21  drsung
# irqreg.c removed from source list
#
# Revision 1.4  2003/10/13 10:13:49  haraldkipp
# First release
#
# Revision 1.3  2003/07/21 18:07:54  haraldkipp
# Sony infrared remote control driver added
#
# Revision 1.2  2003/07/20 16:37:21  haraldkipp
# CrystalTek 8900A driver added.
#
# Revision 1.1.1.1  2003/05/09 14:40:45  haraldkipp
# Initial using 3.2.1
#
# Revision 1.12  2003/05/06 18:27:02  harald
# Cleanup
#
# Revision 1.11  2003/04/07 12:20:02  harald
# Debug devices added
#
# Revision 1.10  2003/03/31 14:53:05  harald
# Prepare release 3.1
#
# Revision 1.9  2003/02/04 17:50:54  harald
# Version 3 released
#
# Revision 1.8  2003/01/14 16:28:03  harald
# *** empty log message ***
#
# Revision 1.7  2002/11/02 15:14:54  harald
# Adapted to configure tool
#
# Revision 1.6  2002/10/29 12:44:36  harald
# PPP support added
#
# Revision 1.5  2002/08/16 17:56:29  harald
# Fixed: Second UART not included
#
# Revision 1.4  2002/06/26 17:29:08  harald
# First pre-release with 2.4 stack
#
# Revision 1.3  2002/06/04 18:39:53  harald
# New make environment
#
# Revision 1.2  2001/12/04 15:52:14  harald
# Added uartspi support
#
# Revision 1.1  2001/05/27 20:05:38  harald
# New device structure implemented
#
#
#

PROJ = libnutdev

top_srcdir = ..

include $(top_srcdir)/Makedefs

SRCS =  irqstack.c \
	uartavr.c uart0.c uart1.c usart.c usart0avr.c usart1avr.c uartspi.c uarts.c ahdlc0.c ahdlc1.c ahdlcavr.c \
	nicrtl.c lanc111.c eth0rtl.c cs8900.c eth0cs.c netbuf.c \
	vs1001k.c irsony.c spidigio.c spiflash.c \
	ide.c twif.c hd44780.c term.c debug0.c debug1.c \
	ppp.c chat.c \
	ivect01.c ivect02.c ivect03.c ivect04.c ivect05.c ivect06.c \
	ivect07.c ivect08.c ivect09.c ivect10.c ivect11.c ivect12.c \
	ivect13.c ivect14.c ivect15.c ivect16.c ivect17.c ivect18.c \
	ivect19.c ivect20.c ivect21.c ivect22.c ivect23.c ivect24.c \
	ivect25.c ivect26.c ivect27.c ivect28.c ivect29.c ivect30.c \
	ivect31.c ivect32.c ivect33.c ivect34.c ihndlr.c

OBJS = $(SRCS:.c=.o)

TARG = $(PROJ).a

all: $(TARG)

include $(top_srcdir)/Makerules

$(TARG): $(OBJS)
	$(AR) $(ARFLAGS) $(TARG) $(OBJS)

install: $(TARG)
	$(CP) $(TARG) $(LIBDIR)/$(TARG)

clean:
	-rm -f $(OBJS)
	-rm -f $(TARG)
	-rm -f $(SRCS:.c=.lst)
	-rm -f $(SRCS:.c=.lis)
	-rm -f $(SRCS:.c=.bak)
	-rm -f $(SRCS:.c=.s)
