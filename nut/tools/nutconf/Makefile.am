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
# Revision 1.8.6.1  2009/03/10 15:45:08  thiagocorrea
# Update lua check to use pkg-config
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

if BUILD_NUTCONF_TOOLS

bin_PROGRAMS = nutconfigure

if BUILD_NUTCONF_GUI

bin_PROGRAMS += nutconf

nutconf_SOURCES = appoptdlg.cpp conftreectrl.cpp confvalrender.h infowindow.h \
nutcomponent.c nutconfhint.h propertylist.h settings.h appoptdlg.h conftreectrl.h \
mainframe.cpp nutcomponent.h nutconf.rc repoptdlg.cpp stateicontext.cpp \
conftreemodel.cpp finddlg.cpp mainframe.h nutconf.cpp nutconfview.cpp repoptdlg.h \
stateicontext.h bldoptdlg.cpp conftreemodel.h finddlg.h nutconfdoc.cpp nutconfview.h \
resource.h tooloptdlg.cpp bldoptdlg.h conftreenode.cpp nutconfdoc.h \
pathvalidator.cpp settings.cpp tooloptdlg.h confitemrender.cpp conftreenode.h \
ids.h nutconf.h pathvalidator.h settingsdlg.cpp utils.cpp confitemrender.h \
confvalrender.cpp infowindow.cpp nutconfhint.cpp propertylist.cpp settingsdlg.h \
utils.h \
bitmaps/application.xpm \
bitmaps/building_large.xpm bitmaps/building.xpm \
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
nutconf_CPPFLAGS = $(LUA_CFLAGS)
nutconf_LDADD = $(WX_LIBS) $(LUA_LIBS)

endif

nutconfigure_SOURCES = nutcomponent.c nutcomponent.h
nutconfigure_CPPFLAGS = -DNUT_CONFIGURE_EXEC $(LUA_CFLAGS)
nutconfigure_LDADD = $(LUA_LIBS)

endif
