#
# Copyright (C) 2001-2007 by egnite Software GmbH
# Copyright (C) 2010 by egnite GmbH
#
# All rights reserved.
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
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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
# $Id$
#

PROJ = caltime

include ../Makedefs

# Comment the following line to exclude the line editor.
HWDEF += -DUSE_LINE_EDITOR

# If battery backed RTC hardware is available, the initial
# time will be taken from the compile time. If you don't
# want this, then comment the next line.
HWDEF += -DUSE_BUILD_TIME

# Uncomment to use other zone offset than configured. E.g. EST is +5 * 60.
# HWDEF += -DUSE_TIME_ZONE=300

SRCS =  $(PROJ).c
OBJS =  $(SRCS:.c=.o)
LIBS =  $(LIBDIR)/nutinit.o -lnutpro -lnutgorp -lnutcrt -lnutos -lnutdev -lnutarch
TARG =  $(PROJ).hex

ifndef SMALL_FLASH
all: $(OBJS) $(TARG) $(ITARG) $(DTARG)
else
all:
endif

include ../Makerules

clean:
	-rm -f $(OBJS)
	-rm -f $(TARG) $(ITARG) $(DTARG)
	-rm -f $(PROJ).eep
	-rm -f $(PROJ).obj
	-rm -f $(PROJ).map
	-rm -f $(SRCS:.c=.lst)
	-rm -f $(SRCS:.c=.bak)
	-rm -f $(SRCS:.c=.i)
	-rm -f $(SRCS:.c=.d)
