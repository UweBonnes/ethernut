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

## Process this file with autoconf to produce a configure script.

#
# $Log$
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

AC_INIT(ethernut,3.9.9)
AC_CONFIG_HEADERS([config.h])
AC_PREREQ(2.59)
AC_COPYRIGHT([Copyright (C) 2005 egnite Software GmbH])
AC_REVISION($Revision$)

# Make sure we got a valid path to the sources.
AC_CONFIG_SRCDIR([tools/nutconf/nutcomponent.c])

AM_INIT_AUTOMAKE([dist-bzip2])

# Check for programs.
AC_PROG_CC
AC_PROG_CXX

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

AC_ARG_ENABLE(nutconf-tools,
  AS_HELP_STRING([--disable-nutconf-tools],[disable configuration tool build]),
  [],
  [enable_nutconf_tools=yes]
)

if test "x$enable_nutconf_tools" = "xno"; then
  nutconf_tools_ok=no
else
  AC_PATH_PROG(LUA_EXEC, lua, no)
  if [[ "$LUA_EXEC" = "no" ]] ; then
    AC_MSG_WARN([Lua not found, will not build configuration tools])
    nutconf_tools_ok=no
  else
    lua_ver="`$LUA_EXEC -v 2>&1`"
    lua_ver=`expr "$lua_ver" : '.*\(@<:@0-9@:>@.@<:@0-9@:>@\.@<:@0-9@:>@\).*'`
    lua_ver_major=`expr "$lua_ver" : '\(@<:@0-9@:>@\)\..*'`
    if test $lua_ver_major -lt "5" ; then
      AC_MSG_WARN([Lua $lua_ver is too old.])
      nutconf_tools_ok=no
    else
      AC_SEARCH_LIBS([lua_open], [lua lua50], [nutconf_tools_ok="yes"], [nutconf_tools_ok="no"])
      AC_SEARCH_LIBS([luaL_loadfile], [lualib lualib50], [nutconf_tools_ok="yes"], [nutconf_tools_ok="no"],[-lm -ldl])
      if test "x$nutconf_tools_ok" = "xno"; then
        AC_MSG_WARN([Lua library missing, will not build configuration tools.])
      fi
    fi
  fi
fi

AM_CONDITIONAL(BUILD_NUTCONF_TOOLS, test x$nutconf_tools_ok = xyes)

# Set wxWidgets environment

AC_ARG_ENABLE(nutconf-gui,
  AS_HELP_STRING([--disable-nutconf-gui],[disable GUI configuration tool build]),
  [],
  [enable_nutconf_gui=yes]
)

if test "x$nutconf_tools_ok" = "xno"; then
  nutconf_ok=no
else
  if test "x$enable_nutconf_gui" = "xno"; then
    nutconf_ok=no
  else
    AC_PATH_PROG(WX_CONFIG, wx-config, no)
    if [[ "$WX_CONFIG" = "no" ]] ; then
        AC_MSG_WARN([wxWidgets not found, will not build the nutconf GUI tool])
        nutconf_ok=no
    else
      wxwidgets_ver=`$WX_CONFIG --version`
      wxwidgets_ver_major=`expr "$wxwidgets_ver" : '\(@<:@0-9@:>@\)\..*'`
      wxwidgets_ver_minor=`expr "$wxwidgets_ver" : '@<:@0-9@:>@\.\(@<:@0-9@:>@\).*'`
      if test $wxwidgets_ver_major -lt "2" -o $wxwidgets_ver_minor -lt "6" ; then
        AC_MSG_WARN([wxWidgets $wxwidgets_ver is too old.])
        nutconf_ok=no
      else
        AC_SUBST(WX_CXXFLAGS,`wx-config --cxxflags`)
        AC_SUBST(WX_LIBS,`wx-config --libs`)
        nutconf_ok=yes
      fi
    fi
  fi
fi

AM_CONDITIONAL(BUILD_NUTCONF_GUI, test x$nutconf_ok = xyes)

AC_CONFIG_FILES([
	Makefile 
	tools/crurom/Makefile
	tools/nutconf/Makefile
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

