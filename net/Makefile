#
# Copyright (C) 2001-2002 by egnite Software GmbH. All rights reserved.
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
# Revision 1.2  2004/03/16 16:48:45  haraldkipp
# Added Jan Dubiec's H8/300 port.
#
# Revision 1.1.1.1  2003/05/09 14:41:34  haraldkipp
# Initial using 3.2.1
#
# Revision 1.12  2003/05/06 18:22:17  harald
# Separate PPP debug module added.
#
# Revision 1.11  2003/03/31 11:44:05  harald
# New PPP structure added.
#
# Revision 1.10  2003/02/04 18:14:56  harald
# Version 3 released
#
# Revision 1.9  2002/10/31 16:35:18  harald
# Mods by troth for Linux
#
# Revision 1.8  2002/08/11 12:28:43  harald
# Using hex file extension now
#
# Revision 1.7  2002/06/26 17:29:35  harald
# First pre-release with 2.4 stack
#
# Revision 1.6  2002/06/04 18:40:28  harald
# New make environment
#
# Revision 1.5  2002/05/08 16:03:06  harald
# First Imagecraft compilation
#
# Revision 1.4  2001/09/01 17:39:11  harald
# Debug module added
#
# Revision 1.3  2001/06/09 10:26:06  harald
# Cleanup
#
# Revision 1.2  2001/05/27 19:59:09  harald
# New device structure implemented
#
# Revision 1.1  2001/05/19 12:30:35  harald
# First check in
#
#

PROJ = libnutnet

top_srcdir = ..

include $(top_srcdir)/Makedefs

SRC1 =  sostream.c tcpsock.c tcpsm.c tcpin.c tcpout.c tcputil.c \
	udpsock.c udpin.c udpout.c \
	icmpin.c icmpout.c \
	lcpin.c ipcpin.c pppsm.c papin.c papout.c

SRC2 =  ipin.c ipout.c ipcsum.c route.c inet.c \
	netdebug.c pppdebug.c \
	arpin.c arpout.c arpcache.c \
	ethin.c ethout.c ifconfig.c confnet.c \
	pppin.c pppout.c lcpout.c ipcpout.c

OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)

TARG = $(PROJ).a

all: $(TARG)

include $(top_srcdir)/Makerules

$(TARG): $(OBJ1) $(OBJ2)
	$(AR) $(ARFLAGS) $(TARG) $(OBJ1)
	$(AR) $(ARFLAGS) $(TARG) $(OBJ2)

install: $(TARG)
	$(CP) $(TARG) $(LIBDIR)/$(TARG)

clean:
	-rm -f $(TARG)
	-rm -f $(OBJ1) $(OBJ2)
	-rm -f $(SRC1:.c=.lst) $(SRC2:.c=.lst)
	-rm -f $(SRC1:.c=.lis) $(SRC2:.c=.lis)
	-rm -f $(SRC1:.c=.s) $(SRC2:.c=.s)
	-rm -f $(SRC1:.c=.bak) $(SRC2:.c=.bak)
	-rm -f $(SRC1:.c=.c~) $(SRC2:.c=.c~)
