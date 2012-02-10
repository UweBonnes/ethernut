## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	$(WX_CXXFLAGS) \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(NUTCONF_CFLAGS)

AM_CFLAGS =\
	 -Wall \
	 -g \
	 $(WX_CPPFLAGS) \
	 -I$(srcdir)


bin_PROGRAMS = nutconfigure

if BUILD_NUTCONF_GUI

bin_PROGRAMS += nutconf

nutconf_SOURCES = \
	nutconf.cpp       \
	nutconf.h         \
	nutcomponent.c    \
	nutcomponent.h    \
	appoptdlg.cpp     \
	appoptdlg.h       \
	bldoptdlg.cpp     \
	bldoptdlg.h       \
	configitem.cpp    \
	configitem.h      \
	configtree.cpp    \
	configtree.h      \
	enumeditctrl.cpp  \
	enumeditctrl.h    \
	iconinfo.cpp      \
	iconinfo.h        \
	iconlist.cpp      \
	iconlist.h        \
	ids.h             \
	infowindow.cpp    \
	infowindow.h      \
	inteditctrl.cpp   \
	inteditctrl.h     \
	mainframe.cpp     \
	mainframe.h       \
	nutconfdoc.cpp    \
	nutconfdoc.h      \
	nutconfhint.cpp   \
	nutconfhint.h     \
	nutconfview.cpp   \
	nutconfview.h     \
	propertylist.cpp  \
	propertylist.h    \
	repoptdlg.cpp     \
	repoptdlg.h       \
	scrolledtree.cpp  \
	scrolledtree.h    \
	settings.cpp      \
	settings.h        \
	settingsdlg.cpp   \
	settingsdlg.h     \
	splitscroll.cpp   \
	splitscroll.h     \
	texteditctrl.cpp  \
	texteditctrl.h    \
	tooloptdlg.cpp    \
	tooloptdlg.h      \
	treecomp.cpp      \
	treecomp.h        \
	treeitemdata.cpp  \
	treeitemdata.h    \
	utils.cpp         \
	utils.h           \
	valuewindow.cpp   \
	valuewindow.h     \
	finddlg.cpp       \
	finddlg.h         \
	pathvalidator.cpp \
	pathvalidator.h

nutconf_CXXFLAGS = @WX_CXXFLAGS@ 

nutconf_LDFLAGS = 

nutconf_LDADD = \
	$(WX_LIBS) \
	$(NUTCONF_LIBS)

endif

nutconfigure_SOURCES = \
	nutcomponent.c \
	nutcomponent.h

nutconfigure_CPPFLAGS = \
	-DNUT_CONFIGURE_EXEC \
	$(NUTCONF_CFLAGS)

nutconfigure_LDADD = $(NUTCONF_LIBS)

