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
# Revision 1.4  2009/02/17 14:46:11  haraldkipp
# Added lua and removed mod.
#
# Revision 1.3  2008/01/31 15:33:33  haraldkipp
# Linux fixes.
#
# Revision 1.2  2007/08/29 15:05:27  haraldkipp
# New directory for bootloaders.
#
# Revision 1.1  2005/04/28 14:43:43  haraldkipp
# First check in
#
#

EXTRA_DIST = nutsetup reconf Makedefs.arm-gcc Makerules.arm-gcc \
Makedefs.arm-gccdbg Makerules.arm-gccdbg \
Makedefs.avr-gcc Makerules.avr-gcc \
Makedefs.avr-gccdbg Makerules.avr-gccdbg \
Makedefs.avr-icc Makerules.avr-icc \
Makedefs.gcc Makerules.gcc Makedefs.h8-gcc Makerules.h8-gcc \
Makedefs.icc Makerules.icc Makedefs.unix-gcc Makerules.unix-gcc

SUBDIRS = doc tools/crurom tools/nutconf tools/nutdisc tools/linux

dist-hook:
	mkdir -p $(distdir)/app
	cp -R $(srcdir)/app/* $(distdir)/app
	mkdir -p $(distdir)/appicc
	cp -R $(srcdir)/appicc/* $(distdir)/appicc
	mkdir -p $(distdir)/appload
	cp -R $(srcdir)/appload/* $(distdir)/appload
	mkdir -p $(distdir)/arch
	cp -R $(srcdir)/arch/* $(distdir)/arch
	mkdir -p $(distdir)/bin
	cp -R $(srcdir)/bin/* $(distdir)/bin
	mkdir -p $(distdir)/boot
	cp -R $(srcdir)/boot/* $(distdir)/boot
	mkdir -p $(distdir)/c
	cp -R $(srcdir)/c/* $(distdir)/c
	mkdir -p $(distdir)/conf
	cp -R $(srcdir)/conf/* $(distdir)/conf
	mkdir -p $(distdir)/cpp
	cp -R $(srcdir)/cpp/* $(distdir)/cpp
	mkdir -p $(distdir)/crt
	cp -R $(srcdir)/crt/* $(distdir)/crt
	mkdir -p $(distdir)/dev
	cp -R $(srcdir)/dev/* $(distdir)/dev
	mkdir -p $(distdir)/eboot
	cp -R $(srcdir)/eboot/* $(distdir)/eboot
	mkdir -p $(distdir)/fs
	cp -R $(srcdir)/fs/* $(distdir)/fs
	mkdir -p $(distdir)/include
	cp -R $(srcdir)/include/* $(distdir)/include
	mkdir -p $(distdir)/lib
	cp -R $(srcdir)/lib/* $(distdir)/lib
	mkdir -p $(distdir)/lua
	cp -R $(srcdir)/lua/* $(distdir)/lua
	mkdir -p $(distdir)/net
	cp -R $(srcdir)/net/* $(distdir)/net
	mkdir -p $(distdir)/os
	cp -R $(srcdir)/os/* $(distdir)/os
	mkdir -p $(distdir)/pro
	cp -R $(srcdir)/pro/* $(distdir)/pro
	rm -rf `find $(distdir) -name CVS`

