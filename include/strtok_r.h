/*--------------------------------------------------------------------------*/
/*                                                                          */
/*  File:           STRTOK_R.H                                              */
/*  Created:        20-September-2002                                       */
/*  Author:         Peter Scandrett                                         */
/*  Description:    Module to provide a reentrant version of the 'C'        */
/*                      function STRTOK.                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

/*
 * $Log$
 * Revision 1.3  2003/12/16 22:35:08  drsung
 * Portability issues
 *
 */

#ifdef __IMAGECRAFT__
char * strtok_r(  char ** pp_str, const char * p_delim );
#endif
char * strsep_r(  char ** pp_str, const char * p_delim );
char * strsep_rs( char ** pp_str, const char * p_delim, char * p_term );

/*------------------------- end of file STRTOK_R.H -------------------------*/
