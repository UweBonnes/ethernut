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
# Revision 1.2  2003/07/21 18:12:44  haraldkipp
# IR and banked memory support added
#
# Revision 1.1.1.1  2003/05/09 14:41:51  haraldkipp
# Initial using 3.2.1
#
# Revision 1.13  2003/02/04 18:15:56  harald
# Version 3 released
#
# Revision 1.12  2003/01/17 18:56:28  harald
# Device routines splitted
#
# Revision 1.11  2002/10/31 16:34:41  harald
# Mods by troth for Linux
#
# Revision 1.10  2002/07/03 16:45:42  harald
# Using GCC 3.2
#
# Revision 1.9  2002/06/26 17:29:44  harald
# First pre-release with 2.4 stack
#
# Revision 1.8  2002/06/04 18:40:21  harald
# New make environment
#
# Revision 1.7  2002/05/08 16:03:15  harald
# First Imagecraft compilation
#
# Revision 1.6  2002/01/28 19:31:08  harald
# *** empty log message ***
#
# Revision 1.5  2001/12/04 16:12:30  harald
# First check in
#
# Revision 1.4  2001/06/09 10:25:32  harald
# Cleanup
#
# Revision 1.3  2001/05/27 20:01:52  harald
# New device structure implemented
#
# Revision 1.2  2001/05/19 14:06:18  harald
# Removed init from lib
#
#

PROJ = libnutos

top_srcdir = ..

include $(top_srcdir)/Makedefs

SRC1 = 	init.c
SRC2 = 	nutinit.c
SRC3 =  print.c ir.c \
	devclose.c devioctl.c devopen.c devread.c devreg.c \
	devtran.c devwrite.c \
	ifstream.c timer.c \
	event.c thread.c bankmem.c heap.c osdebug.c confos.c version.c

OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)
OBJ3 = $(SRC3:.c=.o)

TRG1 = $(OBJ1)
TRG2 = $(OBJ2)
TRG3 = $(PROJ).a

all: $(TRG1) $(TRG2) $(TRG3)

include $(top_srcdir)/Makerules

$(TRG3): $(OBJ3)
	$(AR) $(ARFLAGS) $(TRG3) $(OBJ3)

install: $(TRG1) $(TRG2) $(TRG3)
	$(CP) $(TRG1) $(LIBDIR)/$(TRG1)
	$(CP) $(TRG2) $(LIBDIR)/$(TRG2)
	$(CP) $(TRG3) $(LIBDIR)/$(TRG3)

clean:
	-rm -f $(OBJ1) $(OBJ2) $(OBJ3)
	-rm -f $(TRG1) $(TRG2) $(TRG3)
	-rm -f $(SRC1:.c=.lst) $(SRC2:.c=.lst) $(SRC3:.c=.lst)
	-rm -f $(SRC1:.c=.lis) $(SRC2:.c=.lis) $(SRC3:.c=.lis)
	-rm -f $(SRC1:.c=.s) $(SRC2:.c=.s) $(SRC3:.c=.s)
	-rm -f $(SRC1:.c=.bak) $(SRC2:.c=.bak) $(SRC3:.c=.bak)
