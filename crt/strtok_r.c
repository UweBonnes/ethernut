/*--------------------------------------------------------------------------*/
/*                                                                          */
/*  File:           STRTOK_R.C                                              */
/*  Created:        20-September-2002                                       */
/*  Author:         Peter Scandrett                                         */
/*  Description:    Module to provide a reentrant version of the 'C'        */
/*                      function STRTOK.                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*  Copyright (C) 2002 by ALSTOM Australia Limited. All rights reserved.    */
/*                                                                          */
/*  Redistribution and use in source and binary forms, with or without      */
/*  modification, are permitted provided that the following conditions      */
/*  are met:                                                                */
/*  1.  Redistributions of source code must retain the above copyright      */
/*      notice and this list of conditions.                                 */
/*  2.  Neither the name of ALSTOM Australia Limited nor the names of its   */
/*      contributors may be used to endorse or promote products derived     */
/*      from this software.                                                 */
/*                                                                          */
/*  THIS SOFTWARE IS PROVIDED BY ALSTOM AUSTRALIA LIMITED AND CONTRIBUTORS  */
/*  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     */
/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       */
/*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ALSTOM       */
/*  AUSTRALIA LIMITED OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,   */
/*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,    */
/*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS   */
/*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED      */
/*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,  */
/*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF   */
/*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH    */
/*  DAMAGE.                                                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/

/*
 * $Log$
 * Revision 1.4  2003/12/16 22:34:41  drsung
 * Portability issues
 *
 */

#include    <stdlib.h>

#include    "strtok_r.h"

/*!
 * \addtogroup xgCrtMisc
 *
 * The functions strtok_r(), strsep_r() and strsep_rs() are intended as a 
 * replacement for the strtok() function. While the strtok() function should 
 * be preferred for portability reasons (it conforms to ISO/IEC 9899:1990 
 * ("ISO C89")) it may not be able to be used in a multi-threaded environment 
 * (that is it is not reentrant). Functions strsep_r() and strsep_rs() can 
 * handle empty fields, (i.e. detect fields delimited by two adjacent 
 * delimiter characters). Function strsep() first appeared in 4.4BSD.
 */
/*@{*/

/*--------------------------------------------------------------------------*/

static char *end_tok(char **pp_str, const char *p_delim, char *p_sep)
{
    register const char *sp;
    char *p_tok;
    char *p_ch;

    /*  Use a local pointer. */
    p_ch = *pp_str;

    /*  Scan for next deliminator. */
    p_tok = p_ch;
    while (*p_ch != 0) {
        for (sp = p_delim; *sp != 0; sp++) {
            if (*sp == *p_ch) {
                if (p_sep != NULL) {
                    /*  Save terminator. */
                    *p_sep = *p_ch;
                }
                *p_ch++ = 0;
                *pp_str = p_ch;
                return p_tok;
            }
        }
        p_ch++;
    }

    /*  At end of string, so exit, but return last token. */
    *pp_str = p_ch;
    return p_tok;
}

/*--------------------------------------------------------------------------*/

/*!
 * \brief Thread safe variant of strsep.
 *
 * This function is identical in operation to strsep_r(), except it returns the 
 * deliminating character.
 */
char *strsep_rs(char **pp_str, const char *p_delim, char *p_sep)
{
    char *p_ch;

    /*  Assume terminator was end of string. */
    if (p_sep != NULL)
        *p_sep = 0;

    /*  Check not passed a NULL. */
    if (pp_str == NULL)
        return NULL;

    /*  Use a local pointer. */
    p_ch = *pp_str;
    if (p_ch == NULL)
        return NULL;

    if (*p_ch == 0)
        return NULL;

    /*  Check a valid delimiter string. */
    if ((p_delim == NULL) || (*p_delim == 0)) {
        *pp_str = NULL;
        return p_ch;
    }
    /*  Scan for next deliminator. */
    return end_tok(pp_str, p_delim, p_sep);
}

/*--------------------------------------------------------------------------*/

/*!
 * \brief Thread safe version of strsep.
 *
 * This function locates, in the string referenced by *str, the first occurrence of 
 * any character in the string delim (or the terminating `\0' character) and 
 * replaces it with a `\0'. The location of the next character after the delimiter 
 * character (or NULL, if the end of the string was reached) is stored in *str. 
 * The original value of *str is returned.
 *
 * An "empty" field, i.e. one caused by two adjacent delimiter characters, can be 
 * detected by comparing the location referenced by the pointer returned in *str 
 * to `\0'.
 *
 * If *str is initially NULL, strsep_r() returns NULL. 
 *
 * If delim is NULL or the empty string, strsep_r() returns *str.
 */
char *strsep_r(char **pp_str, const char *p_delim)
{
    return strsep_rs(pp_str, p_delim, NULL);
}

/*--------------------------------------------------------------------------*/
/*!
 * \brief Thread safe version of strtok.
 *
 * This function locates, in the string referenced by *str, the occurrence of 
 * any character in the string delim (or the terminating `\0' character) and 
 * replaces them with a `\0'. The location of the next character after the 
 * delimiter character (or NULL, if the end of the string was reached) is 
 * stored in *str. The first character not a delimiter character from the original 
 * value of *str is returned.
 *
 * \deprecated This function is supported by avrlibc, which uses different
 *             parameters. It's still available for ICCAVR, but should be
 *             avoided for portability reasons.
 */
#ifdef __IMAGECRAFT__
char *strtok_r(char **pp_str, const char *p_delim)
{
    register const char *sp;
    char *p_ch;

    /*  Check not passed a NULL. */
    if (pp_str == NULL)
        return NULL;

    /*  Use a local pointer. */
    p_ch = *pp_str;
    if (p_ch == NULL)
        return NULL;

    if (*p_ch == 0)
        return NULL;

    /*  Check a valid delimiter string. */
    if ((p_delim == NULL) || (*p_delim == 0)) {
        *pp_str = NULL;
        return p_ch;
    }
    /*  Skip leading deliminators. */
    while (*p_ch != 0) {
        for (sp = p_delim; *sp != 0; sp++) {
            if (*sp == *p_ch)
                /*  break on non-delimiter character. */
                break;
        }
        if (*sp == 0)
            break;
        p_ch++;
    }

    /*  Save point where tokenising stopped. */
    *pp_str = p_ch;

    /*  Exit if at end of string. */
    if (*p_ch == 0)
        return NULL;

    /*  Have found a non-deliminator character, so scan for next deliminator. */
    return end_tok(pp_str, p_delim, NULL);
}
#endif

/*@}*/

/*-------------------------- end of file STRTOK_R.C ------------------------*/
