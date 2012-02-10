## Process this file with automake to produce Makefile.in
## Created by Anjuta

if BUILD_NUTDISC

SUBDIRS = src 

nutdiscdocdir = ${prefix}/doc/nutdisc

nutdiscdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(nutdiscdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(nutdiscdocdir)

endif
