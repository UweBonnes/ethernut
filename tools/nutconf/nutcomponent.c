/*
 * Copyright (C) 2004 by egnite Software GmbH. All rights reserved.
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
 *
 */

/*
 * $Log$
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

/*
 * This module had been intentionally written in C, not C++,
 * to make its use in simple non-GUI applications as easy as
 * possible.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "nutcomponent.h"

char configuration_path[] = "nut/conf/ethernut2.conf";

#if 0

/* Quite helpful during debugging. */
void ShowStack(lua_State * ls)
{
    int top;
    int i;
    int type;

    printf("Stack:\n");
    top = lua_gettop(ls);
    for (i = top; i > 0; i--) {
        type = lua_type(ls, i);
        printf("%d %s=", i, lua_typename(ls, type));
        switch (type) {
        case LUA_TNONE:
            break;
        case LUA_TNIL:
            break;
        case LUA_TBOOLEAN:
            break;
        case LUA_TLIGHTUSERDATA:
            break;
        case LUA_TNUMBER:
            break;
        case LUA_TSTRING:
            printf("%s\n", lua_tostring(ls, -1));
            break;
        case LUA_TTABLE:
            printf("\n");
            lua_pushnil(ls);
            while (lua_next(ls, i)) {
                if (lua_type(ls, -2) == LUA_TSTRING) {
                    printf("  %s - %s\n", lua_tostring(ls, -2), lua_typename(ls, lua_type(ls, -1)));
                } else if (lua_type(ls, -2) == LUA_TNUMBER) {
                    printf("  [%.0lf] %s\n", lua_tonumber(ls, -2), lua_typename(ls, lua_type(ls, -1)));
                } else {
                    printf("  %s - %s\n", lua_typename(ls, lua_type(ls, -2)), lua_typename(ls, lua_type(ls, -1)));
                }
                lua_pop(ls, 1);
            }
            break;
        case LUA_TFUNCTION:
            break;
        case LUA_TUSERDATA:
            break;
        case LUA_TTHREAD:
            break;
        }
        printf("\n");
    }
}

void ShowCompoTree(NUTCOMPONENT * compo, int level)
{
    NUTCOMPONENTOPTION *opts;
    int i;

    while (compo) {
        for (i = 0; i < level; i++) {
            printf("  ");
        }
        printf("%s\n", compo->nc_name);
        opts = compo->nc_opts;
        while (opts) {
            if (opts->nco_value) {
                for (i = 0; i <= level; i++) {
                    printf("  ");
                }
                printf("%s = %s\n", opts->nco_name, opts->nco_value);
            }
            opts = opts->nco_nxt;
        }
        if (compo->nc_child) {
            ShowCompoTree(compo->nc_child, level + 1);
        }
        compo = compo->nc_nxt;
    }
}

#endif

/*!
 * \brief Get a string value from a named item of a table.
 *
 * The table is expected at a specified position of the Lua stack.
 *
 * \param ls   Lua state.
 * \param idx  Stack position of the table.
 * \param name Name of the table item.
 * \param dst  Pointer to buffer that receives the string. If NULL, the
 *             buffer will be allocated from heap.
 * \param siz  Size of the buffer. Ignored if dst is NULL.
 *
 * \return Pointer to the buffer containing the retrieved value or NULL,
 *         if there is no table item with the specified name.
 */
char *GetStringByNameFromTable(lua_State * ls, int idx, char *name, char *dst, size_t siz)
{
    lua_pushstring(ls, name);
    lua_gettable(ls, idx < 0 ? idx - 1 : idx + 1);
    if (lua_isstring(ls, -1)) {
        if (dst == NULL) {
            dst = strdup(lua_tostring(ls, -1));
        } else if (lua_strlen(ls, -1) < siz) {
            strcpy(dst, lua_tostring(ls, -1));
        } else {
            dst = NULL;
        }
    } else {
        dst = NULL;
    }
    lua_pop(ls, 1);

    return dst;
}

/*!
 * \brief Get an array of string values from a named item of a table.
 *
 * The table is expected at a specified position of the Lua stack.
 *
 * \param ls   Lua state.
 * \param idx  Stack position of the table.
 * \param name Name of the table item.
 *
 * \return Pointer to an array of string pointers to the retrieved values 
 *         or NULL, if there is no table item with the specified name.
 *         The string buffers as well as the array itself are allocated
 *         from heap.
 */
char **GetStringArrayByNameFromTable(lua_State * ls, int idx, char *name)
{
    int cnt = 0;
    char **result = NULL;

    lua_pushstring(ls, name);
    lua_gettable(ls, idx < 0 ? idx - 1 : idx + 1);
    if (lua_istable(ls, -1)) {
        lua_pushnil(ls);
        while (lua_next(ls, -2)) {
            if (lua_isstring(ls, -1)) {
                cnt++;
            }
            lua_pop(ls, 1);
        }
    }
    lua_pop(ls, 1);

    if (cnt) {
        result = calloc(cnt + 1, sizeof(char *));
        cnt = 0;
        lua_pushstring(ls, name);
        lua_gettable(ls, idx < 0 ? idx - 1 : idx + 1);
        if (lua_istable(ls, -1)) {
            lua_pushnil(ls);
            while (lua_next(ls, -2)) {
                if (lua_isstring(ls, -1)) {
                    result[cnt++] = strdup(lua_tostring(ls, -1));
                }
                lua_pop(ls, 1);
            }
        }
        result[cnt] = NULL;
        lua_pop(ls, 1);
    }
    return result;
}

NUTCOMPONENT *FindComponentByName(NUTCOMPONENT * root, char *name)
{
    NUTCOMPONENT *walk;
    NUTCOMPONENT *compo = NULL;

    walk = root->nc_child;
    while (walk && compo == NULL) {
        if (strcmp(walk->nc_name, name) == 0) {
            compo = walk;
            break;
        } else if (walk->nc_child) {
            compo = FindComponentByName(walk, name);
        }
        walk = walk->nc_nxt;
    }
    return compo;
}

/*
 * The components table is expected on top of the LUA stack.
 */
void LoadComponentOptions(lua_State * ls, NUTCOMPONENT * compo)
{
    char *name;
    NUTCOMPONENTOPTION *opts;

    printf("Loading options of %s\n", compo->nc_name);
    /* Push the option table on the stack. */
    lua_pushstring(ls, "options");
    lua_gettable(ls, -2);
    if (lua_istable(ls, -1)) {
        opts = compo->nc_opts;
        while (opts && opts->nco_nxt) {
            opts = opts->nco_nxt;
        }
        lua_pushnil(ls);
        while (lua_next(ls, -2)) {
            name = GetStringByNameFromTable(ls, -1, "macro", NULL, 0);
            if (name) {
                printf("Option: '%s'\n", name);
                if (opts) {
                    opts->nco_nxt = calloc(1, sizeof(NUTCOMPONENTOPTION));
                    opts = opts->nco_nxt;
                } else {
                    opts = calloc(1, sizeof(NUTCOMPONENTOPTION));
                    compo->nc_opts = opts;
                }
                opts->nco_name = name;
                opts->nco_brief = GetStringByNameFromTable(ls, -1, "brief", NULL, 0);
                opts->nco_description = GetStringByNameFromTable(ls, -1, "description", NULL, 0);
                opts->nco_active_if = GetStringByNameFromTable(ls, -1, "active_if", NULL, 0);
                opts->nco_flavor = GetStringByNameFromTable(ls, -1, "flavor", NULL, 0);
                opts->nco_type = GetStringByNameFromTable(ls, -1, "type", NULL, 0);
                opts->nco_ctype = GetStringByNameFromTable(ls, -1, "ctype", NULL, 0);
                opts->nco_file = GetStringByNameFromTable(ls, -1, "file", NULL, 0);
                opts->nco_choices = GetStringArrayByNameFromTable(ls, -1, "choices");
            }
            lua_pop(ls, 1);
        }
    }
    lua_pop(ls, 1);
}

void LoadOptions(lua_State * ls, NUTCOMPONENT * root)
{
    NUTCOMPONENT *compo;
    NUTCOMPONENT *subc;
    char *name;

    compo = root->nc_child;
    while (compo) {
        lua_getglobal(ls, compo->nc_name);
        LoadComponentOptions(ls, compo);
        lua_pushnil(ls);
        while (lua_next(ls, -2)) {
            if (lua_isnumber(ls, -2)) {
                name = GetStringByNameFromTable(ls, -1, "name", NULL, 0);
                if (name) {
                    subc = FindComponentByName(root, name);
                    if (subc) {
                        LoadComponentOptions(ls, subc);
                    }
                }
            }
            lua_pop(ls, 1);
        }
        lua_pop(ls, 1);
        compo = compo->nc_nxt;
    }
}

/*
 * Option name is a global LUA variable.
 */
int LoadComponents(lua_State * ls, NUTCOMPONENT * parent, const char *path, const char *file)
{
    int rc = 0;
    char script[255];
    char *name;
    NUTCOMPONENT *compo = NULL;

    strcpy(script, path);
    strcat(script, file);
    if (_access(script, 0)) {
        return -1;
    }
    printf("Loading %s from %s\n", parent->nc_name, script);
    if ((rc = lua_dofile(ls, script)) != 0) {
        fprintf(stderr, "Bad script in %s\n", path);
        return -1;
    }

    lua_getglobal(ls, parent->nc_name);
    if (!lua_istable(ls, -1)) {
        fprintf(stderr, "Bad type for %s\n", parent->nc_name);
        lua_pop(ls, 1);
        return -1;
    }

    /* Enumerate */
    lua_pushnil(ls);
    while (lua_next(ls, -2)) {
        if (lua_isnumber(ls, -2)) {
            name = GetStringByNameFromTable(ls, -1, "name", NULL, 0);
            if (name) {
                /* First component on this level. */
                if (compo == NULL) {
                    compo = calloc(1, sizeof(NUTCOMPONENT));
                    parent->nc_child = compo;
                } else {
                    compo->nc_nxt = calloc(1, sizeof(NUTCOMPONENT));
                    compo->nc_nxt->nc_prv = compo;
                    compo = compo->nc_nxt;
                }
                compo->nc_parent = parent;
                compo->nc_name = name;
                compo->nc_brief = GetStringByNameFromTable(ls, -1, "brief", NULL, 0);
                compo->nc_description = GetStringByNameFromTable(ls, -1, "description", NULL, 0);
                compo->nc_subdir = GetStringByNameFromTable(ls, -1, "subdir", NULL, 0);
                compo->nc_sources = GetStringArrayByNameFromTable(ls, -1, "sources");
                if (GetStringByNameFromTable(ls, -1, "script", script, sizeof(script))) {
                    LoadComponents(ls, compo, path, script);
                }
            }
        }
        lua_pop(ls, 1);
    }
    lua_pop(ls, 1);

    return rc;
}


void LoadConfigValues(lua_State * ls, NUTCOMPONENT * compo)
{
    NUTCOMPONENTOPTION *opts;

    while (compo) {
        opts = compo->nc_opts;
        while (opts) {
            lua_getglobal(ls, opts->nco_name);
            if (lua_isstring(ls, -1)) {
                opts->nco_value = strdup(lua_tostring(ls, -1));
            }
            lua_pop(ls, 1);
            opts = opts->nco_nxt;
        }
        if (compo->nc_child) {
            LoadConfigValues(ls, compo->nc_child);
        }
        compo = compo->nc_nxt;
    }
}

void LoadConfiguration(lua_State * ls, NUTCOMPONENT * root, const char *file)
{
    int rc;

    /* NAME.value */
    if ((rc = lua_dofile(ls, file)) != 0) {
        fprintf(stderr, "Bad script in %s\n", file);
        return;
    }
    LoadConfigValues(ls, root);
}

/*!
 * \brief Load a Nut/OS configuration repository.
 *
 * \param file Pathname of the repository file. Use slashes, not backslashes.
 *
 * \return Root pointer of a linked tree of NUTCOMPONENT structures.
 */
NUTCOMPONENT *LoadRepository(const char *file)
{
    lua_State *ls;
    NUTCOMPONENT *root;
    char path[255];
    char *np;

    strcpy(path, file);
    if ((np = strrchr(path, '/')) != NULL) {
        *++np = 0;
        file += strlen(path);
    } else {
        path[0] = 0;
    }

    /* Create root component. */
    root = calloc(1, sizeof(NUTCOMPONENT));
    root->nc_name = strdup("repository");

    /*
     * Create a LUA state.
     */
    ls = lua_open();
    LoadComponents(ls, root, path, file);

    LoadOptions(ls, root);

    LoadConfiguration(ls, root, configuration_path);

    lua_close(ls);

    return root;
}

void WriteMakeSources(FILE * fp, NUTCOMPONENT * compo)
{
    int i;
    int c = 8;

    fprintf(fp, "SRCS =\t");
    while (compo) {
        for (i = 0; compo->nc_sources[i]; i++) {
            c += strlen(compo->nc_sources[i]);
            if (c > 72) {
                fprintf(fp, " \\\n\t");
                c = 8;
            }
            fprintf(fp, " %s", compo->nc_sources[i]);
        }
        compo = compo->nc_nxt;
    }
    fputc('\n', fp);
    fputc('\n', fp);
}

void WriteMakeRootLines(FILE * fp, NUTCOMPONENT * compo, char *target)
{
    if (target) {
        fprintf(fp, "%s:\n", target);
    } else {
        fprintf(fp, "all:\n");
    }
    while (compo) {
        if (compo->nc_subdir) {
            fprintf(fp, "\t$(MAKE) -C %s", compo->nc_subdir);
            if (target) {
                fprintf(fp, " %s", target);
            }
            fputc('\n', fp);
        }
        compo = compo->nc_nxt;
    }
    fprintf(fp, "\n");
}

/*!
 * \brief Create makefiles from a specified NUTCOMPONENT tree.
 */
void CreateMakeFiles(NUTCOMPONENT * root)
{
    FILE *fp;
    char path[255];
    NUTCOMPONENT *compo;

    /* Create the root Makefile */
    fp = fopen("nutbld/Makefile", "w");
    if (fp) {
        fprintf(fp, "# This file has been created automatically\n\n");
        WriteMakeRootLines(fp, root->nc_child, NULL);
        WriteMakeRootLines(fp, root->nc_child, "install");
        WriteMakeRootLines(fp, root->nc_child, "clean");
        fclose(fp);
    }

    /* Create library Makefiles */
    compo = root->nc_child;
    while (compo) {
        if (compo->nc_subdir) {
            sprintf(path, "nutbld/%s", compo->nc_subdir);
            _mkdir(path);
            strcat(path, "/Makefile");
            fp = fopen(path, "w");
            if (fp) {
                fprintf(fp, "# This file has been created automatically\n\n");
                fprintf(fp, "PROJ =\tlib%s\n\n", compo->nc_name);
                fprintf(fp, "top_srcdir = ../../nut\n\n");
                fprintf(fp, "top_blddir = ..\n\n");

                WriteMakeSources(fp, compo->nc_child);
                fprintf(fp, "include $(top_srcdir)/Makedefs\n\n");
                fprintf(fp, "all: $(PROJ).a\n\n");
                fprintf(fp, "include $(top_srcdir)/Makerules\n\n");
                fclose(fp);
            }
        }
        compo = compo->nc_nxt;
    }
}

typedef struct _NUTHEADERMACRO NUTHEADERMACRO;

struct _NUTHEADERMACRO {
    NUTHEADERMACRO *nhm_nxt;
    char *nhm_name;
    char *nhm_value;
};

typedef struct _NUTHEADERFILE NUTHEADERFILE;

struct _NUTHEADERFILE {
    NUTHEADERFILE *nhf_nxt;
    char *nhf_path;
    NUTHEADERMACRO *nhf_macros;
};

NUTHEADERFILE *nh_root;

void AddHeaderFileMacro(NUTCOMPONENTOPTION * opts)
{
    NUTHEADERFILE *nhf;
    NUTHEADERMACRO *nhm;

    if (nh_root) {
        nhf = nh_root;
        while (stricmp(nhf->nhf_path, opts->nco_file)) {
            if (nhf->nhf_nxt) {
                nhf = nhf->nhf_nxt;
            } else {
                nhf->nhf_nxt = calloc(1, sizeof(NUTHEADERFILE));
                nhf = nhf->nhf_nxt;
                nhf->nhf_path = opts->nco_file;
            }
        }
    } else {
        nhf = calloc(1, sizeof(NUTHEADERFILE));
        nhf->nhf_path = opts->nco_file;
        nh_root = nhf;
    }

    if (nhf->nhf_macros) {
        nhm = nhf->nhf_macros;
        while (stricmp(nhm->nhm_name, opts->nco_name)) {
            if (nhm->nhm_nxt) {
                nhm = nhm->nhm_nxt;
            } else {
                nhm->nhm_nxt = calloc(1, sizeof(NUTHEADERMACRO));
                nhm = nhm->nhm_nxt;
                nhm->nhm_name = opts->nco_name;
                nhm->nhm_value = opts->nco_value;
            }
        }
    } else {
        nhm = calloc(1, sizeof(NUTHEADERFILE));
        nhm->nhm_name = opts->nco_name;
        nhm->nhm_value = opts->nco_value;
        nhf->nhf_macros = nhm;
    }
}

/*!
 * \brief Create a linked list of header files and associated macros. 
 */
void CreateHeaderList(NUTCOMPONENT * compo)
{
    NUTCOMPONENTOPTION *opts;

    while (compo) {
        opts = compo->nc_opts;
        while (opts) {
            if (opts->nco_file) {
                AddHeaderFileMacro(opts);
            }
            opts = opts->nco_nxt;
        }
        if (compo->nc_child) {
            CreateHeaderList(compo->nc_child);
        }
        compo = compo->nc_nxt;
    }
}

/*!
 * \brief Create header files from a specified NUTCOMPONENT tree.
 */
void CreateHeaderFiles(NUTCOMPONENT * root)
{
    NUTHEADERFILE *nhf;
    NUTHEADERMACRO *nhm;
    FILE *fp;
    char path[255];

    CreateHeaderList(root->nc_child);

    /* Create the build directories. */
    _mkdir("nutbld");
    _mkdir("nutbld/cfg");

    for (nhf = nh_root; nhf; nhf = nhf->nhf_nxt) {
        strcpy(path, "nutbld/");
        strcat(path, nhf->nhf_path);
        if ((fp = fopen(path, "w")) != 0) {
            for (nhm = nhf->nhf_macros; nhm; nhm = nhm->nhm_nxt) {
                if (nhm->nhm_value) {
                    fprintf(fp, "#define %s %s\n", nhm->nhm_name, nhm->nhm_value);
                }
            }
            fclose(fp);
        }
    }
}
