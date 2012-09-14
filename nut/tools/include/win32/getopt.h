#ifndef _GETOPT_H_
#define _GETOPT_H_

/*
 * $Id$
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
