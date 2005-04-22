#ifndef _GETOPT_H_
#define _GETOPT_H_

/*
 * $Id$
 */

/*
 * $Log$
 * Revision 1.1  2005/04/22 15:12:04  haraldkipp
 * Added to support non-GUI Configurator.
 *
 */

#ifdef __cplusplus
    extern "C" {
#endif

extern int getopt(int argc, char **argv, char *opts);

extern int optind;
extern char *optarg;

#ifdef __cplusplus
    }
#endif

#endif
