//--------------------------------------------------------------------------//
//                                                                          //
//  File:           STRTOK_R.C                                              //
//  Created:        20-September-2002                                       //
//  Author:         Peter Scandrett                                         //
//  Description:    Module to provide a reentrant version of the 'C'        //
//                      function STRTOK.                                    //
//                                                                          //
//--------------------------------------------------------------------------//
//                                                                          //
//  Copyright (C) 2002 by ALSTOM Australia Limited. All rights reserved.    //
//                                                                          //
//  Redistribution and use in source and binary forms, with or without      //
//  modification, are permitted provided that the following conditions      //
//  are met:                                                                //
//  1.  Redistributions of source code must retain the above copyright      //
//      notice and this list of conditions.                                 //
//  2.  Neither the name of ALSTOM Australia Limited nor the names of its   //
//      contributors may be used to endorse or promote products derived     //
//      from this software.                                                 //
//                                                                          //
//  THIS SOFTWARE IS PROVIDED BY ALSTOM AUSTRALIA LIMITED AND CONTRIBUTORS  //
//  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     //
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       //
//  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ALSTOM       //
//  AUSTRALIA LIMITED OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,   //
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,    //
//  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS   //
//  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED      //
//  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,  //
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF   //
//  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH    //
//  DAMAGE.                                                                 //
//                                                                          //
//--------------------------------------------------------------------------//

#include    <stdlib.h>

#include    "strtok_r.h"

/*!
 * \addtogroup xgCrtMisc
 */
/*@{*/

//--------------------------------------------------------------------------//

static char *end_tok(char **pp_str, const char *p_delim, char *p_sep)
{
    register const char *sp;
    char *p_tok;
    char *p_ch;

    //  Use a local pointer.
    //
    p_ch = *pp_str;

    //  Scan for next deliminator.
    //
    p_tok = p_ch;
    while (*p_ch != 0) {
        for (sp = p_delim; *sp != 0; sp++) {
            if (*sp == *p_ch) {
                if (p_sep != NULL) {
                    //  Save terminator.
                    *p_sep = *p_ch;
                }
                *p_ch++ = 0;
                *pp_str = p_ch;
                return p_tok;
            }
        }
        p_ch++;
    }

    //  At end of string, so exit, but return last token.
    //
    *pp_str = p_ch;
    return p_tok;
}

//--------------------------------------------------------------------------//

char *strsep_rs(char **pp_str, const char *p_delim, char *p_sep)
{
    char *p_ch;

    //  Assume terminator was end of string.
    if (p_sep != NULL)
        *p_sep = 0;

    //  Check not passed a NULL.
    //
    if (pp_str == NULL)
        return NULL;

    //  Use a local pointer.
    //
    p_ch = *pp_str;
    if (p_ch == NULL)
        return NULL;

    if (*p_ch == 0)
        return NULL;

    //  Check a valid delimiter string.
    //
    if ((p_delim == NULL) || (*p_delim == 0)) {
        *pp_str = NULL;
        return p_ch;
    }
    //  Scan for next deliminator.
    //
    return end_tok(pp_str, p_delim, p_sep);
}

//--------------------------------------------------------------------------//

/*!
 * \brief Thread safe version of strsep.
 */
char *strsep_r(char **pp_str, const char *p_delim)
{
    return strsep_rs(pp_str, p_delim, NULL);
}

//--------------------------------------------------------------------------//
/*!
 * \brief Thread safe version of strtok.
 */
char *strtok_r(char **pp_str, const char *p_delim)
{
    register const char *sp;
    char *p_ch;

    //  Check not passed a NULL.
    //
    if (pp_str == NULL)
        return NULL;

    //  Use a local pointer.
    //
    p_ch = *pp_str;
    if (p_ch == NULL)
        return NULL;

    if (*p_ch == 0)
        return NULL;

    //  Check a valid delimiter string.
    //
    if ((p_delim == NULL) || (*p_delim == 0)) {
        *pp_str = NULL;
        return p_ch;
    }
    //  Skip leading deliminators.
    //
    while (*p_ch != 0) {
        for (sp = p_delim; *sp != 0; sp++) {
            if (*sp == *p_ch)
                //  break on non-delimiter character.
                break;
        }
        if (*sp == 0)
            break;
        p_ch++;
    }

    //  Save point where tokenising stopped.
    //
    *pp_str = p_ch;

    //  Exit if at end of string.
    //
    if (*p_ch == 0)
        return NULL;

    //  Have found a non-deliminator character, so scan for next
    //      deliminator.
    //
    return end_tok(pp_str, p_delim, NULL);
}

/*@}*/

//-------------------------- end of file STRTOK_R.C ------------------------//
