PROJ = nutconf

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
	$(CXX) -o $(PROJ) $(OBJS) `wx-config --libs` -llualib50 -lc

install: $(PROJ)
	$(CP) $(PROJ) ../linux/$(PROJ)

clean: 
	rm -f *.o $(PROJ)

