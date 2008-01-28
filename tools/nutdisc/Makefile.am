#
# Copyright (C) 2005 by egnite Software GmbH. All rights reserved.
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
# 3. Neither the name of the copyright holders nor the names of
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
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

## Process this file with automake to generate Makefile.in

#
# $Log$
# Revision 1.1  2008/01/28 16:43:11  haraldkipp
# Version 2.2
#
# Revision 1.8  2007/07/18 11:44:23  haraldkipp
# Automatically determine Lua header directory.
#
# Revision 1.7  2007/05/02 13:16:06  haraldkipp
# *** empty log message ***
#
# Revision 1.6  2006/10/06 17:51:02  haraldkipp
# *** empty log message ***
#
# Revision 1.5  2006/04/07 18:48:59  haraldkipp
# Added maintainer-clean targets
#
# Revision 1.4  2005/11/22 20:03:52  haraldkipp
# Added missing bitmaps.
#
# Revision 1.3  2005/07/20 11:15:23  haraldkipp
# Linux application icon added
#
# Revision 1.2  2005/05/22 14:55:16  haraldkipp
# Lua libs are determined in configure.ac.
# Option to build no configuration tools.
#
# Revision 1.1  2005/04/28 15:08:35  haraldkipp
# First check in
#
#

MAINTAINERCLEANFILES = Makefile.in

if BUILD_NUTDISC

bin_PROGRAMS = nutdisc

nutdisc_SOURCES = discovery.h \
mainframe.cpp \
mainframe.h \
nutdisc.cpp \
nutdisc.h \
propdialog.cpp \
propdialog.h \
resource.h \
setup.cpp \
setup.h \
taskbaricon.cpp \
taskbaricon.h \
udpthread.cpp \
udpthread.h \
version.h \
bitmaps/nutdisc.ico
nutdisc_CXXFLAGS = @WX_CXXFLAGS@ 
nutdisc_CPPFLAGS = -I$(LUA_HEADERS)
nutdisc_LDADD = /usr/local/lib/libwx_gtk2_propgrid-2.8.a $(WX_LIBS)

endif
