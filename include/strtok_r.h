//--------------------------------------------------------------------------//
//                                                                          //
//  File:           STRTOK_R.H                                              //
//  Created:        20-September-2002                                       //
//  Author:         Peter Scandrett                                         //
//  Description:    Module to provide a reentrant version of the 'C'        //
//                      function STRTOK.                                    //
//                                                                          //
//--------------------------------------------------------------------------//

char * strtok_r(  char ** pp_str, const char * p_delim );
char * strsep_r(  char ** pp_str, const char * p_delim );
char * strsep_rs( char ** pp_str, const char * p_delim, char * p_term );

//------------------------- end of file STRTOK_R.H -------------------------//
