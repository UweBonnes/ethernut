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
# Revision 1.4  2004/04/07 12:14:00  haraldkipp
# Matthias Ringwald's *nix emulation added
#
# Revision 1.3  2004/03/16 16:48:46  haraldkipp
# Added Jan Dubiec's H8/300 port.
#
# Revision 1.2  2003/11/24 18:12:40  drsung
# Added sntp.c to sources
#
# Revision 1.1.1.1  2003/05/09 14:41:58  haraldkipp
# Initial using 3.2.1
#
# Revision 1.7  2003/02/04 18:18:46  harald
# Clean up ICC leftovers
#
# Revision 1.6  2002/10/31 16:33:34  harald
# Mods by troth for Linux
#
# Revision 1.5  2002/06/26 17:29:48  harald
# First pre-release with 2.4 stack
#
# Revision 1.4  2002/06/04 18:39:18  harald
# New make environment
#
# Revision 1.3  2002/05/08 16:03:20  harald
# First Imagecraft compilation
#
# Revision 1.2  2001/08/10 18:16:53  harald
# GCC version 3 update
#
# Revision 1.1  2001/06/09 10:36:06  harald
# First check in
#
#

PROJ = libnutpro

top_srcdir = ..

include $(top_srcdir)/Makedefs

SRCS =  dhcpc.c resolv.c httpd.c auth.c cgi.c dencode.c sntp.c

OBJS = $(SRCS:.c=.o)

TARG = $(PROJ).a

all: $(TARG)

include $(top_srcdir)/Makerules

$(TARG): $(OBJS)
	$(AR) $(ARFLAGS) $(TARG) $(OBJS)

install: $(TARG)
	$(CP)     $(TARG) $(LIBDIR)/$(TARG)
	$(RANLIB) $(LIBDIR)/$(TARG)

clean:
	-rm -f $(OBJS)
	-rm -f $(TARG)
	-rm -f $(SRCS:.c=.lst)
	-rm -f $(SRCS:.c=.lis)
	-rm -f $(SRCS:.c=.s)
	-rm -f $(SRCS:.c=.bak)
	-rm -f $(SRCS:.c=.c~)
