#
# Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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
# Revision 1.1  2003/05/09 14:40:30  haraldkipp
# Initial revision
#
# Revision 1.2  2003/03/31 14:53:05  harald
# Prepare release 3.1
#
# Revision 1.1  2003/02/04 17:49:04  harald
# *** empty log message ***
#
#

PROJ = libnutcrt

top_srcdir = ..

include $(top_srcdir)/Makedefs

SRCC =  clrerr.c fclose.c fcloseall.c fdopen.c feof.c ferror.c fflush.c \
	fgetc.c fgets.c fileno.c flushall.c fmode.c fopen.c fprintf.c \
	fprintf_p.c fputc.c fputs.c fputs_p.c fread.c freopen.c \
	fscanf.c fscanf_p.c fseek.c ftell.c fwrite.c fwrite_p.c \
	getc.c getchar.c gets.c printf.c printf_p.c putc.c putchar.c \
	puts.c puts_p.c scanf.c scanf_p.c sprintf.c sprintf_p.c \
	sscanf.c sscanf_p.c ungetc.c vfprintf.c vfprintf_p.c \
	vfscanf.c vfscanf_p.c vsprintf.c vsprintf_p.c \
	vsscanf.c vsscanf_p.c \
	close.c ioctl.c open.c read.c write.c write_p.c filelength.c \
	malloc.c strtok_r.c
SRCS =  $(SRCC) putf.c getf.c
SRCF =  $(SRCC) putff.c getff.c

OBJS = $(SRCS:.c=.o)
OBJF = $(SRCF:.c=.o)

TARG  = $(PROJ).a
TARGF = $(PROJ)f.a
TARGS = $(TARG) $(TARGF)

all: $(TARGS)

include $(top_srcdir)/Makerules

$(TARG): $(OBJS)
	$(AR) $(ARFLAGS) $(TARG) $(OBJS)

$(TARGF): $(OBJF)
	$(AR) $(ARFLAGS) $(TARGF) $(OBJF)

install: $(TARGS)
	$(CP) $(TARG) $(LIBDIR)/$(TARG)
	$(CP) $(TARGF) $(LIBDIR)/$(TARGF)

clean:
	-rm -f $(OBJS)
	-rm -f $(OBJF)
	-rm -f $(TARGS)
	-rm -f $(SRCS:.c=.lst) $(SRCF:.c=.lst)
	-rm -f $(SRCS:.c=.lis) $(SRCF:.c=.lis)
	-rm -f $(SRCS:.c=.s) $(SRCF:.c=.s)
	-rm -f $(SRCS:.c=.bak) $(SRCF:.c=.bak)
