#ifndef _NUTCOMPONENT_H_
#define _NUTCOMPONENT_H_

/*
 * Copyright (C) 2003-2004 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

/*
 * $Log$
 * Revision 1.2  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

/* This construct avoids automatic indention. */
#ifdef  __cplusplus
# define __BEGIN_DECLS  extern "C" {
# define __END_DECLS    }
#else
# define __BEGIN_DECLS
# define __END_DECLS
#endif

/*
 * Options
 *
 * enabled - can be modified.
 * active - macro is included.
 * active_if - automatically activated.
 *
 * requires - needs this function (may result in conflict)
 * provides - provides this funktion
 */

typedef struct _NUTCOMPONENTOPTION NUTCOMPONENTOPTION;

struct _NUTCOMPONENTOPTION {
    /*! \brief Pointer to next option. */
    NUTCOMPONENTOPTION *nco_nxt;
    /*! \brief Pointer to previous option. */
    NUTCOMPONENTOPTION *nco_prv;
    char *nco_name;
    char *nco_brief;
    char *nco_description;

    int  nco_enabled;
    int  nco_active;
    char *nco_active_if;
    char **nco_requires;
    char **nco_provides;

    char *nco_flavor;
    char *nco_type;
    char **nco_choices;
    char *nco_ctype;
    char *nco_value;
    char *nco_file;
};

typedef struct _NUTCOMPONENT NUTCOMPONENT;

struct _NUTCOMPONENT {
    /*! \brief Pointer to next sibling component. */
    NUTCOMPONENT *nc_nxt;
    /*! \brief Pointer to previous sibling component. */
    NUTCOMPONENT *nc_prv;
    /*! \brief Pointer to first parent component. */
    NUTCOMPONENT *nc_parent;
    /*! \brief Pointer to first child component. */
    NUTCOMPONENT *nc_child;
    /*! \brief Pointer to first option. */
    NUTCOMPONENTOPTION *nc_opts;
    char *nc_name;
    char *nc_brief;
    char *nc_description;
    int  nc_enabled;
    char **nc_requires;
    char **nc_provides;
    char *nc_active_if;
    char *nc_subdir;
    char **nc_sources;
};

typedef struct _NUTREPOSITORY NUTREPOSITORY;

struct _NUTREPOSITORY {
    void *nr_ls;
    char *nr_dir;
    char *nr_name;
};

__BEGIN_DECLS
/* Function prototypes */
extern NUTREPOSITORY *OpenRepository(const char *pathname);
extern NUTCOMPONENT *LoadComponents(NUTREPOSITORY *repo);
extern int ConfigureComponents(NUTREPOSITORY *repo, NUTCOMPONENT *root, const char *pathname);
extern void CloseRepository(NUTREPOSITORY *repo);

extern int RefreshComponents(NUTCOMPONENT *root);

extern void CreateMakeFiles(NUTCOMPONENT *root, const char *bld_dir, const char *src_dir, const char *mak_ext);
extern void CreateHeaderFiles(NUTCOMPONENT * root, const char *bld_dir);

__END_DECLS                     /* */
#endif
