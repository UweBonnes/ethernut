#
# Copyright (C) 2004-2005 by egnite Software GmbH. All rights reserved.
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
# Revision 1.5  2005/02/06 19:59:45  haraldkipp
# Added lib names for Fedora
#
# Revision 1.4  2004/09/17 13:03:48  haraldkipp
# New settings page for tool options
#
#



PROJ = nutconf

#LUALIBS=-llualib50
LUALIBS=-llua -llualib

CP = cp

CXX = $(shell wx-config --cxx)

OBJS = $(PROJ).o appoptdlg.o bldoptdlg.o configitem.o configtree.o enumeditctrl.o \
    iconinfo.o iconlist.o infowindow.o inteditctrl.o mainframe.o \
    nutconfdoc.o nutconfhint.o nutconfview.o propertylist.o repoptdlg.o \
    scrolledtree.o settings.o settingsdlg.o splitscroll.o texteditctrl.o \
    tooloptdlg.o treecomp.o treeitemdata.o utils.o valuewindow.o nutcomponent.o


.SUFFIXES:	.o .cpp .c

.cpp.o :
	$(CXX) -c `wx-config --cxxflags` -o $@ $<

.c.o :
	$(CC) -c -I/usr/include/lua50 -o $@ $<

all:    $(PROJ)

$(PROJ):	$(OBJS)
	$(CXX) -o $(PROJ) $(OBJS) `wx-config --libs` $(LUALIBS) -lc

install: $(PROJ)
	$(CP) $(PROJ) ../linux/$(PROJ)

clean: 
	rm -f *.o $(PROJ)

