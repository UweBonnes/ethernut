#ifndef _GETOPT_H_
#define _GETOPT_H_

/*
 * $Id$
 */

/*
 * $Log$
 * Revision 1.1  2003/07/20 19:18:16  haraldkipp
 * First check in
 *
 * Revision 1.1  2002/08/08 16:24:05  harald
 * *** empty log message ***
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
