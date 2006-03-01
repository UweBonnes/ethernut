#
# Makefile - Simple Makefile for Nut/OS example programs
#
# $Id: Makefile,v 1.2 2006/03/01 02:17:18 hwmaier Exp $
#


PROJ = candemo

include ../Makedefs

SRCS =  $(PROJ).c
OBJS =  $(SRCS:.c=.o)
LIBS =  $(LIBDIR)/nutinit.o -lnutpro -lnutos -lnutarch -lnutdev -lnutnet -lnutfs -lnutcrt

TARG =  $(PROJ).hex

all: $(OBJS) $(TARG)

include ../Makerules

clean:
	-rm -f $(OBJS)
	-rm -f $(TARG)
	-rm -f $(PROJ).eep
	-rm -f $(PROJ).obj
	-rm -f $(PROJ).map
	-rm -f $(SRCS:.c=.lst)
	-rm -f $(SRCS:.c=.bak)
	-rm -f $(SRCS:.c=.i)

