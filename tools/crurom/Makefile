OBJS = crurom.o 
# i need this
LDFLAGS = -L/usr/lib 
# link flags
LLDLIBS = -lm 
# your C-compiler
CC = gcc

crurom:	$(OBJS)
	$(CC) -o crurom $(OBJS) $(LDFLAGS) $(LLDLIBS)
clean:
	rm -f crurom glload *.o core
# $Log: Makefile,v $
# Revision 1.2  2004/03/16 16:48:46  haraldkipp
# Added Jan Dubiec's H8/300 port.
#
# Revision 1.1  2003/07/20 19:18:16  haraldkipp
# First check in
#
# Revision 1.1  2001/06/28 19:16:03  harald
# Preview release
#
# Revision 1.3  2000/03/08 13:02:58  eilemann
# That should it be
#
