#
# Copyright (C) 2004 by egnite Software GmbH. All rights reserved.
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
# Revision 1.3  2005/09/17 09:25:15  drsung
# irqstack.c also moved to arch
#
# Revision 1.2  2005/07/26 16:18:28  haraldkipp
# Build new architecture library.
#
# Revision 1.1  2004/09/19 12:37:42  haraldkipp
# ARM7 initialization added
#
#

PROJ = libnutarch

top_srcdir = ..
top_blddir = ..

VPATH = $(top_srcdir)/arch

LIBDIR = $(top_blddir)/lib

include $(top_blddir)/UserConf.mk

SRCS =
ifeq ($(ARCH), avr)
SRCS = avr/os/context_gcc.c avr/dev/ostimer.c avr/dev/ivect01.c \
	avr/dev/ivect02.c avr/dev/ivect03.c avr/dev/ivect04.c avr/dev/ivect05.c \
	avr/dev/ivect06.c avr/dev/ivect07.c avr/dev/ivect08.c avr/dev/ivect09.c \
	avr/dev/ivect10.c avr/dev/ivect11.c avr/dev/ivect12.c avr/dev/ivect13.c \
	avr/dev/ivect14.c avr/dev/ivect15.c avr/dev/ivect16.c avr/dev/ivect17.c \
	avr/dev/ivect18.c avr/dev/ivect19.c avr/dev/ivect20.c avr/dev/ivect21.c \
	avr/dev/ivect22.c avr/dev/ivect23.c avr/dev/ivect24.c avr/dev/ivect25.c \
	avr/dev/ivect26.c avr/dev/ivect27.c avr/dev/ivect28.c avr/dev/ivect29.c \
	avr/dev/ivect30.c avr/dev/ivect31.c avr/dev/ivect32.c avr/dev/ivect33.c \
	avr/dev/ivect34.c avr/dev/ivect35.c avr/dev/ivect36.c avr/dev/ahdlc0.c \
	avr/dev/ahdlc1.c avr/dev/ahdlcavr.c avr/dev/debug0.c avr/dev/debug1.c \
	avr/dev/usart0avr.c avr/dev/usart1avr.c avr/dev/uart0.c avr/dev/uart1.c \
	avr/dev/uartavr.c avr/dev/lanc111.c avr/dev/nicrtl.c avr/dev/eth0rtl.c avr/dev/ide.c \
	avr/dev/cs8900.c avr/dev/eth0cs.c avr/dev/wlan.c avr/dev/wlandrv.c avr/dev/adc.c \
	avr/dev/ir.c avr/dev/irsony.c avr/dev/hd44780.c avr/dev/hd44780_bus.c \
	avr/dev/sja1000.c avr/dev/spidigio.c avr/dev/twif.c avr/dev/vs1001k.c \
	avr/dev/pcmcia.c avr/dev/irqstack.c
endif

ifeq ($(ARCH), unix)

SRCS = arm/os/context.c

ifeq ($(DEVICE), at91r40008)
SRCS += arm/dev/ostimer_at91.c arm/dev/debug_at91.c arm/dev/ax88796.c
OBJ1 = arm/init/crt$(LDNAME).o
endif

ifeq ($(DEVICE), gba)
SRCS += arm/dev/ostimer_gba.c arm/dev/ihndlr_gba.c arm/dev/debug_gba.c
OBJ1 = arm/init/crt$(LDNAME).o
endif

endif

ifeq ($(ARCH), unix)
SRCS = unix/dev/ostimer.c unix/dev/unix_devs.c
endif

OBJS = $(SRCS:.c=.o)

include $(top_srcdir)/Makedefs

all: $(PROJ).a $(OBJS) $(OBJ1)

install: $(PROJ).a $(OBJ1)
	$(CP) $(PROJ).a $(LIBDIR)/$(PROJ).a
	-$(CP) $(OBJ1) $(LIBDIR)/$(notdir $(OBJ1))

include $(top_srcdir)/Makerules

.PHONY: clean
clean: cleancc cleanedit
	-rm -f $(PROJ).a
	-rm -f $(OBJ1)
