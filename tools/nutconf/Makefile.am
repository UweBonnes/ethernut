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

if BUILD_NUTCONF_TOOLS

bin_PROGRAMS = nutconfigure

if BUILD_NUTCONF_GUI

bin_PROGRAMS += nutconf

nutconf_SOURCES = nutconf.cpp nutcomponent.c appoptdlg.cpp bldoptdlg.cpp \
configitem.cpp configtree.cpp enumeditctrl.cpp iconinfo.cpp iconlist.cpp \
infowindow.cpp inteditctrl.cpp mainframe.cpp nutconfdoc.cpp nutconfhint.cpp \
nutconfview.cpp propertylist.cpp repoptdlg.cpp scrolledtree.cpp settings.cpp \
settingsdlg.cpp splitscroll.cpp texteditctrl.cpp tooloptdlg.cpp treecomp.cpp \
treeitemdata.cpp utils.cpp valuewindow.cpp \
appoptdlg.h iconinfo.h nutconfdoc.h scrolledtree.h treeitemdata.h \
bldoptdlg.h iconlist.h nutconf.h settingsdlg.h utils.h \
configitem.h ids.h nutconfhint.h settings.h valuewindow.h \
configtree.h infowindow.h nutconfview.h splitscroll.h \
inteditctrl.h propertylist.h texteditctrl.h \
enumeditctrl.h mainframe.h repoptdlg.h tooloptdlg.h \
nutcomponent.h resource.h treecomp.h \
bitmaps/application.xpm \
bitmaps/checked_dis.xpm bitmaps/module.xpm \
bitmaps/checked.xpm bitmaps/nutconf.xpm \
bitmaps/closedfolder_dis.xpm \
bitmaps/closedfolder.xpm bitmaps/radiooff_dis.xpm \
bitmaps/copy.xpm bitmaps/radiooff.xpm \
bitmaps/enumerated_dis.xpm bitmaps/radioon_dis.xpm \
bitmaps/enumerated.xpm bitmaps/radioon.xpm \
bitmaps/integer_dis.xpm \
bitmaps/integer.xpm bitmaps/text_dis.xpm \
bitmaps/library_dis.xpm bitmaps/text.xpm \
bitmaps/library.xpm bitmaps/unchecked_dis.xpm \
bitmaps/module_dis.xpm bitmaps/unchecked.xpm \
bitmaps/buildlibrary.xpm bitmaps/buildlibrary_large.xpm \
bitmaps/help.xpm bitmaps/help_large.xpm \
bitmaps/open.xpm bitmaps/open_large.xpm \
bitmaps/save.xpm bitmaps/save_large.xpm \
bitmaps/search.xpm bitmaps/search_large.xpm
nutconf_CXXFLAGS = @WX_CXXFLAGS@
nutconf_LDADD = $(WX_LIBS)

endif

nutconfigure_SOURCES = nutcomponent.c nutcomponent.h
nutconfigure_CPPFLAGS = -DNUT_CONFIGURE_EXEC

endif
