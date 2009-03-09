#
# Copyright (C) 2005-2007 by egnite Software GmbH. All rights reserved.
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

## Process this file with autoconf to produce a configure script.

#
# $Log$
# Revision 1.44  2009/03/09 08:15:14  haraldkipp
# Release branch 4.8 announced
#
# Revision 1.43  2009/01/09 17:54:14  haraldkipp
# Next beta will be 4.7.5.
#
# Revision 1.42  2009/01/04 20:54:11  olereinhardt
# 2009-01-04  Ole Reinhardt <ole.reinhardt@thermotemp.de>
#
#         * configure.ac: Checks and warns about an empty $LUA_HEADERS
#                         also looks in more places.
#
# Revision 1.41  2008/10/10 11:52:24  haraldkipp
# Lua added.
#
# Revision 1.40  2008/08/29 15:03:03  haraldkipp
# Fixed Configurator bug #2082123. Options now correctly enabled.
#
# Revision 1.39  2008/08/28 16:11:31  haraldkipp
# Fixed bitbanging SPI for ARM targets.
#
# Revision 1.38  2008/08/25 17:05:26  haraldkipp
# Beta release 4.7.1.
#
# Revision 1.37  2008/07/29 13:06:21  haraldkipp
# Starting beta 4.7
#
# Revision 1.36  2008/07/29 13:04:17  haraldkipp
# Release branch 4.6 created
#
# Revision 1.35  2008/07/29 12:13:36  haraldkipp
# Release branch 4.6 announced
#
# Revision 1.34  2008/07/26 09:39:16  haraldkipp
# Preparing 4.5.7 beta.
#
# Revision 1.33  2008/07/24 17:06:07  haraldkipp
# Preparing 4.5.6 beta.
#
# Revision 1.32  2008/07/10 12:17:41  haraldkipp
# Preparing beta version 4.5.5.
#
# Revision 1.31  2008/04/18 13:12:31  haraldkipp
# ICCAVR fixes, many shorts to int, PPP+RS485 for AT91.
#
# Revision 1.30  2008/04/02 09:39:28  haraldkipp
# Fixed another PHAT file pointer bug.
#
# Revision 1.29  2008/04/01 12:07:58  haraldkipp
# *** empty log message ***
#
# Revision 1.28  2008/04/01 10:09:25  haraldkipp
# Beta version 4.5.2.
# Bug #1892070 fixed. Indeed wxWidgets 2.8 is required at least.
#
# Revision 1.27  2008/02/15 16:50:33  haraldkipp
# Beta release 4.5.1.
#
# Revision 1.26  2008/01/31 15:33:33  haraldkipp
# Linux fixes.
#
# Revision 1.25  2008/01/28 16:39:28  haraldkipp
# Added nutdisc build (Steffen Ritter).
#
# Revision 1.24  2007/09/11 15:40:45  haraldkipp
# Release branch 4.4 created
#
# Revision 1.23  2007/08/29 13:35:18  haraldkipp
# Announcing release branch 4.4.
#
# Revision 1.22  2007/07/18 11:44:23  haraldkipp
# Automatically determine Lua header directory.
#
# Revision 1.21  2007/07/16 15:48:33  haraldkipp
# Supports latest autoconf.
#
# Revision 1.20  2007/05/02 11:35:25  haraldkipp
# ATmega2561 and other fixes.
# IGMP support started, but not finished.
# Socket ioctl started, but not finished.
#
# Revision 1.19  2007/04/12 08:54:38  haraldkipp
# Supports configurable SPI, VS10XX MP3 decoder chips and Mega256 for
# avr-gcc 4.1.1.
#
# Revision 1.18  2006/10/17 11:26:02  haraldkipp
# Release branch 4.2 created.
#
# Revision 1.17  2006/10/06 17:51:32  haraldkipp
# *** empty log message ***
#
# Revision 1.16  2006/09/08 16:02:29  haraldkipp
# Release candidate 9
#
# Revision 1.15  2006/08/01 07:47:25  haraldkipp
# Additional spaces at some line ends let Autoconf on Windows fail with
# 'already registered with AC_CONFIG_FILES'. Thanks to Andras Albert,
# who detected and fixed it.
#
# Revision 1.14  2006/05/25 09:37:40  haraldkipp
# *** empty log message ***
#
# Revision 1.13  2006/05/15 12:48:49  haraldkipp
# Prepare pre-release 4.1.6.
#
# Revision 1.12  2006/04/07 13:59:54  haraldkipp
# Pre-release 4.1.5.4.
#
# Revision 1.11  2006/02/08 15:17:35  haraldkipp
# Bugs fixed.
#
# Revision 1.10  2006/01/22 17:46:18  haraldkipp
# Pre-release 4.1.3
#
# Revision 1.9  2005/12/04 18:42:19  haraldkipp
# Release branch X.Y created.
#
# Revision 1.8  2005/12/02 11:36:12  haraldkipp
# Release 4.0 scheduled
#
# Revision 1.7  2005/11/24 09:46:06  haraldkipp
# Linker flags -lm and -ldl needed by lualib are not automatically applied
# by configure. Checks added by Torben Mikael Hansen.
#
# Revision 1.6  2005/11/22 20:04:49  haraldkipp
# New release.
#
# Revision 1.5  2005/07/20 09:14:39  haraldkipp
# Release 3.9.8
#
# Revision 1.4  2005/06/10 13:27:24  haraldkipp
# Lua check failed because autoconf test needs additional libraries.
# Thanks to Tim Lapawa for this fix.
#
# Revision 1.3  2005/06/06 10:40:31  haraldkipp
# Emergency bugfix release 3.9.7.1.
#
# Revision 1.2  2005/05/22 14:48:46  haraldkipp
# Added option to completely disable tool build.
# Check versions of Lua and wxWidgets.
#
# Revision 1.1  2005/04/28 14:43:43  haraldkipp
# First check in
#
#

AC_INIT(ethernut,4.7.99)
AC_CONFIG_HEADERS([config.h])
AC_PREREQ(2.59)
AC_COPYRIGHT([Copyright (C) 2005-2009 egnite GmbH])
AC_REVISION($Revision$)

# Make sure we got a valid path to the sources.
AC_CONFIG_SRCDIR([tools/nutconf/nutcomponent.c])

AM_INIT_AUTOMAKE([dist-bzip2])

# Check for programs.
#AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O

AC_ARG_ENABLE(html-docs,
  AS_HELP_STRING([--disable-html-docs],[disable generation of API documents]),
  [],
  [ enable_html_docs=yes]
)

if test "x$enable_html_docs" = "xno"; then
  doxygen_ok=no
else
  AC_CHECK_PROG(HAVE_DOXYGEN,doxygen,true,false)
  if test $HAVE_DOXYGEN = "false"; then
    AC_MSG_WARN([Doxygen not found, will not create API reference.])
    doxygen_ok=no
  else
    doxygen_ver=`doxygen --version`
    doxygen_ver_major=`expr "$doxygen_ver" : '\(@<:@0-9@:>@\)\..*'`
    doxygen_ver_minor=`expr "$doxygen_ver" : '@<:@0-9@:>@\.\(@<:@0-9@:>@\).*'`
    if test $doxygen_ver_major -lt "1" -o $doxygen_ver_minor -lt "4" ; then
      AC_MSG_WARN([Doxygen $doxygen_ver is too old.])
      doxygen_ok=no
    else
      doxygen_ok=yes
    fi
  fi
  AC_CHECK_PROG(HAVE_GRAPHVIZ,[dot],true,false)
  if test $HAVE_GRAPHVIZ = "false"; then
    AC_MSG_WARN([Graphviz not found, will create incomplete API reference.])
  fi
fi

AM_CONDITIONAL(USE_DOXYGEN, test x$doxygen_ok = xyes)
AM_CONDITIONAL(HAVE_GRAPHVIZ, $HAVE_GRAPHVIZ)

# Check for libraries.

# Check for header files.
AC_STDC_HEADERS
AC_HAVE_HEADERS(string.h fcntl.h)

# Check for tools build.

# Commandline arguments ###################################################################################
AC_ARG_ENABLE(nutconf-tools,
  AS_HELP_STRING([--disable-nutconf-tools],[disable configuration tool build]),
  [],
  [enable_nutconf_tools=yes]
)

AC_ARG_ENABLE(nutconf-gui,
  AS_HELP_STRING([--disable-nutconf-gui],[disable GUI configuration tool build]),
  [],
  [enable_nutconf_gui=yes]
)

AC_ARG_ENABLE(wxwidgets,
  AS_HELP_STRING([--disable-wxwidgets],[disable wxWidgets]),
  [],
  [enable_wxwidgets=yes]
)

AC_ARG_ENABLE(wxpropgrid,
  AS_HELP_STRING([--disable-wxpropgrid],[disable xwProperty Grid]),
  [],
  [enable_wxpropgrid=yes]
)

AC_ARG_ENABLE(nutdisc,
  AS_HELP_STRING([--disable-nutdisc],[disable NutOs Discoverer]),
  [],
  [enable_nutdisc=yes]
)

AC_ARG_ENABLE(lua,
  AS_HELP_STRING([--disable-lua],[disable lua]),
  [],
  [enable_lua=yes]
)
# END Commandline arguments ###############################################################################
# lua #####################################################################################################
if test "x$enable_lua" = "xno"; then
  lua_ok=no
else
  AC_PATH_PROG(LUA_EXEC, lua, no)
  if [[ "$LUA_EXEC" = "no" ]] ; then
    AC_MSG_WARN([Lua not found, will not build configuration tools])
    lua_ok=no
  else
    lua_ver="`$LUA_EXEC -v 2>&1`"
    lua_ver=`expr "$lua_ver" : '.*\(@<:@0-9@:>@.@<:@0-9@:>@\.@<:@0-9@:>@\).*'`
    lua_ver_major=`expr "$lua_ver" : '\(@<:@0-9@:>@\)\..*'`
    if test $lua_ver_major -lt "5" ; then
      AC_MSG_WARN([Lua $lua_ver is too old.])
      lua_ok=no
    else
      AC_SEARCH_LIBS([lua_open], [lua lua50], [lua_ok="yes"], [lua_ok="no"])
      AC_SEARCH_LIBS([luaL_loadfile], [lualib lualib50],, [lua_ok="no"],[-lm -ldl])
      if test "x$lua_ok" = "xno"; then
        AC_MSG_WARN([Lua library missing, will not build configuration tools.])
      else
        AC_CHECK_HEADERS(/usr/include/lua50/lua.h, AC_SUBST(LUA_HEADERS, "/usr/include/lua50"))
        AC_CHECK_HEADERS(/usr/local/include/lua50/lua.h, AC_SUBST(LUA_HEADERS, "/usr/local/include/lua50"))
        AC_CHECK_HEADERS(/usr/local/include/lua.h, AC_SUBST(LUA_HEADERS, "/usr/local/include"))
        AC_CHECK_HEADERS(/usr/include/lua.h, AC_SUBST(LUA_HEADERS, "/usr/include"))
      fi
#    AC_CHECK_FUNC(sin, LIBS=$LIBS, LIBS="-lm $LIBS")
#    AC_CHECK_LIB(dl, dlopen)
    fi
  fi
fi

if test "x$LUA_HEADERS" = "x"; then
   AC_MSG_WARN(Lua Headers not found disabled)
   lua_ok=no
fi

# END lua #################################################################################################
# nutconfigure ############################################################################################
if test "x$enable_nutconf_tools" = "xno"; then
  AC_MSG_WARN(nutconf tools disabled)
  nutconf_tools_ok=no
else
  if test "x$lua_ok" = "xno"; then
    AC_MSG_WARN(lua disabled: required for nutconf tools)
    nutconf_tools_ok=no
  else
    AC_MSG_NOTICE(nutconf tools enabled)
    AC_CHECK_FUNC(sin, LIBS=$LIBS, LIBS="-lm $LIBS")
    AC_CHECK_LIB(dl, dlopen)
    nutconf_tools_ok=yes
  fi
fi
# END nutconfigure ########################################################################################
# GUI tools ###############################################################################################
# wxWidgets ###############################################################################################
if test "x$enable_wxwidgets" = "xyes"; then
  AC_MSG_NOTICE([wxWidgets enabled])
  AC_MSG_NOTICE([checking for wxWidgets])
  AC_PATH_PROG(WX_CONFIG, wx-config, no)
  if [[ "$WX_CONFIG" = "no" ]] ; then
    AC_MSG_WARN([wxWidgets not found, will not build the GUI tools])
    wxwidgets_ok=no
  else
    wxwidgets_ver=`$WX_CONFIG --version`
    wxwidgets_ver_major=`expr "$wxwidgets_ver" : '\(@<:@0-9@:>@\)\..*'`
    wxwidgets_ver_minor=`expr "$wxwidgets_ver" : '@<:@0-9@:>@\.\(@<:@0-9@:>@\).*'`
    if test $wxwidgets_ver_major -lt "2" -o $wxwidgets_ver_minor -lt "8" ; then
      AC_MSG_WARN([wxWidgets $wxwidgets_ver is too old.])
      wxwidgets_ok=no
    else
      AC_SUBST(WX_CXXFLAGS,`wx-config --cxxflags`)
      AC_SUBST(WX_LIBS,`wx-config --libs`)
      wxwidgets_ok=yes
    fi
  fi
fi
# END wxWidgets #########################################################################################
# Property Grid #########################################################################################
if test "x$wxwidgets_ok" = "xyes"; then 
  AC_MSG_NOTICE([checking for propgrid])
  if test "x$enable_wxpropgrid" = "xyes" ; then
    AC_MSG_NOTICE([wxpropgrid enabled])
    AC_MSG_NOTICE([checking for wxpropgrid])
    propgridpath1=/usr/include/wx-`wx-config --release`/wx/propgrid
    propgridpath2=/usr/local/include/wx-`wx-config --release`/wx/propgrid
    propgrid_local=no
    AC_CHECK_FILE($propgridpath1/propgrid.h,[propgrid_ok="yes"],[propgrid_ok="no"])
    if test "x$propgrid_ok" = "xno"; then
        AC_CHECK_FILE($propgridpath2/propgrid.h,[propgrid_ok="yes"],[propgrid_ok="no"])
        propgrid_local=yes
    fi
    if test "x$propgrid_ok" = "xno"; then
      AC_MSG_WARN(wxpropgrid not found)
    else
      AC_MSG_NOTICE(wxpropgrid enabled)
    fi
  else
    propgrid_ok=no
    AC_MSG_WARN([wxpropgrid disabled])
  fi
fi
# END Property Grid #####################################################################################
# nutconfgui ############################################################################################
if test "x$wxwidgets_ok" = "xyes"; then 
  AC_MSG_NOTICE([checking for nutconfgui])
  if test "x$enable_nutconf_gui" = "xno"; then
    AC_MSG_NOTICE([nutconfgui wasnt enabled])
    nutconf_ok=no
  else
    if test "x$nutconf_tools_ok" = "xno"; then
      AC_MSG_WARN([nutconfigure wasn't build])
      nutconf_ok=no
    else
      AC_MSG_NOTICE([nutconfgui is enabled])
      nutconf_ok=yes
    fi
  fi
fi
# END nutconfgui ########################################################################################
# nutdiscoverer #########################################################################################
if test "x$wxwidgets_ok" = "xyes"; then 
  AC_MSG_NOTICE([checking for nutdisc])
  if test "x$enable_nutdisc" = "xno"; then
    AC_MSG_NOTICE([nutdisc disabled])
    nutdisc_ok=no
  else
    if test "x$lua_ok" = "xno"; then
      AC_MSG_NOTICE([nutdisc requires lua])
      nutdisc_ok=no
    else
      if test "x$propgrid_ok" = "xno"; then
        AC_MSG_NOTICE([nutdisc requires propgrid])
        nutdisc_ok=no
      else
        AC_MSG_NOTICE([nutdisc enabled])
        nutdisc_ok=yes
      fi
    fi
  fi
fi
# END nutdiscoverer #####################################################################################
# END GUI tools #########################################################################################

AM_CONDITIONAL(BUILD_NUTCONF_TOOLS, test x$nutconf_tools_ok = xyes)
AM_CONDITIONAL(BUILD_NUTCONF_GUI, test x$nutconf_ok = xyes)
AM_CONDITIONAL(BUILD_NUTDISC, test x$nutdisc_ok = xyes)
AM_CONDITIONAL(PROPGRID_LOCAL, test x$propgrid_local = xyes)
AC_CONFIG_FILES([
	Makefile
	tools/crurom/Makefile
	tools/nutconf/Makefile
	tools/nutdisc/Makefile
	tools/linux/Makefile
	tools/packaging/ethernut.spec
	doc/Makefile
	doc/gen/Makefile
	doc/gen/nut_en.cfg
	])
AC_OUTPUT

echo "===================================================================="
echo "  Your Nut/OS development environment has been configured."
echo "  After running make and make install, you may..."
echo "  ... run nutconf or nutconfigure to set up a Nut/OS build tree."
echo "  ... or run ./nutsetup to set up the environment for building "
echo "      Nut/OS in the source tree."
echo "===================================================================="

