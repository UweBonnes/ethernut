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
 * Revision 1.4  2004/08/18 14:05:22  haraldkipp
 * Fill error text if script not found
 *
 * Revision 1.3  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.2  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

/*
 * This module had been intentionally written in C, not C++,
 * to make its use in simple non-GUI applications as easy as
 * possible.
 */
//#define NUT_CONFIGURE_EXEC
#define NUT_CONFIGURE_VERSION   "0.0.0"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifdef _WIN32

#include <io.h>
#include <direct.h>
#define mkdir(P, M) _mkdir(P)
#define access _access
#define strcasecmp stricmp

#else

#include <unistd.h>

#endif


#include <lua.h>
#include <lauxlib.h>

#include "nutcomponent.h"

/*! \brief Internal component name.
 * Must be unique.
 */
#define TKN_NAME    "name"

/*! \brief Name of a macro.
 * Identifier used in configuration include files.
 */
#define TKN_MACRO   "macro"

/*! \brief
 */
#define TKN_SCRIPT  "script"

/*! \brief
 */
#define TKN_SUBDIR  "subdir"

/*! \brief Short component description.
 */
#define TKN_BRIEF   "brief"

/*! \brief Detailed component description.
 */
#define TKN_DESC    "description"

#define TKN_REQUIRES "requires"
#define TKN_PROVIDES "provides"

/*! \brief Component option list.
 */
#define TKN_OPTIONS "options"

/*! \brief
 */
#define TKN_ACTIF   "active_if"

/*! \brief Component's option flavor.
 * Possible values are
 * - boolean: Option may be disabled or enabled
 * - booldata: Option may be disabled or enabled and contains a value if enabled.
 */
#define TKN_FLAVOR  "flavor"

/*! \brief Option's value type.
 * - integer
 * - bool
 * - enumerated
 */
#define TKN_TYPE    "type"

/*! \brief
 */
#define TKN_CTYPE   "ctype"

/*! \brief Relative path of the configuration include file.
 */
#define TKN_FILE    "file"

/*! \brief List of source files.
 */
#define TKN_SOURCES "sources"

/*! \brief List of explicit target files.
 */
#define TKN_TARGETS "targets"

/*! \brief
 */
#define TKN_CHOICES "choices"

/*! \brief
 */
#define TKN_MAKEDEFS "makedefs"

static char errtxt[1024];

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

const char *GetScriptErrorString(void)
{
    return errtxt;
}

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

    /* Push the option table on the stack. */
    lua_pushstring(ls, TKN_OPTIONS);
    lua_gettable(ls, -2);
    if (lua_istable(ls, -1)) {
        opts = compo->nc_opts;
        while (opts && opts->nco_nxt) {
            opts = opts->nco_nxt;
        }
        lua_pushnil(ls);
        while (lua_next(ls, -2)) {
            name = GetStringByNameFromTable(ls, -1, TKN_MACRO, NULL, 0);
            if (name) {
                if (opts) {
                    opts->nco_nxt = calloc(1, sizeof(NUTCOMPONENTOPTION));
                    opts = opts->nco_nxt;
                } else {
                    opts = calloc(1, sizeof(NUTCOMPONENTOPTION));
                    compo->nc_opts = opts;
                }
                opts->nco_name = name;
                opts->nco_brief = GetStringByNameFromTable(ls, -1, TKN_BRIEF, NULL, 0);
                opts->nco_description = GetStringByNameFromTable(ls, -1, TKN_DESC, NULL, 0);
                opts->nco_requires = GetStringArrayByNameFromTable(ls, -1, TKN_REQUIRES);
                opts->nco_provides = GetStringArrayByNameFromTable(ls, -1, TKN_PROVIDES);
                opts->nco_active_if = GetStringByNameFromTable(ls, -1, TKN_ACTIF, NULL, 0);
                opts->nco_flavor = GetStringByNameFromTable(ls, -1, TKN_FLAVOR, NULL, 0);
                opts->nco_type = GetStringByNameFromTable(ls, -1, TKN_TYPE, NULL, 0);
                opts->nco_ctype = GetStringByNameFromTable(ls, -1, TKN_CTYPE, NULL, 0);
                opts->nco_file = GetStringByNameFromTable(ls, -1, TKN_FILE, NULL, 0);
                opts->nco_choices = GetStringArrayByNameFromTable(ls, -1, TKN_CHOICES);
                opts->nco_makedefs = GetStringArrayByNameFromTable(ls, -1, TKN_MAKEDEFS);
            }
            lua_pop(ls, 1);
        }
    }
    lua_pop(ls, 1);
}


/*!
 * \brief Recursively load Nut/OS component options.
 */
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
                name = GetStringByNameFromTable(ls, -1, TKN_NAME, NULL, 0);
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

/*!
 * \brief Recursively load Nut/OS components.
 *
 * Components may have siblings and children. Right now, Nut/OS has two levels
 * only. The root component is the repository itself and all Nut/OS components
 * like os, crt, net etc. are children of this root. Anyway, we recursively
 * call this routine to be prepared for future subcomponents.
 *
 * \param ls     Lua state.
 * \param parent Parent component.
 * \param path   Top directory of the repository.
 * \param file   Relative pathname of the script file.
 *
 * \return 0 on success, -1 otherwise.
 *
 * Option name is a global LUA variable.
 */
int LoadComponentTree(lua_State * ls, NUTCOMPONENT * parent, const char *path, const char *file)
{
    int rc = 0;
    char script[255];
    char *name;
    NUTCOMPONENT *compo = NULL;

    /* Build the pathname and check if the script file exists. */
    strcpy(script, path);
    strcat(script, "/");
    strcat(script, file);
    if (access(script, 0)) {
        sprintf(errtxt, "%s: Script not found", file);
        return -1;
    }

    /* Let the interpreter load the script file. */
    if ((rc = luaL_loadfile(ls, script)) != 0) {
        strcpy(errtxt, lua_tostring(ls, -1));
        return -1;
    }
    if(lua_pcall(ls, 0, 0, 0)) {
        strcpy(errtxt, lua_tostring(ls, -1));
        return -1;
    }

    /* 
     * The component is defined by a Lua array, which name is the
     * name of the parent component. Push this array on top of the
     * Lua stack and make sure we got an array.
     */
    lua_getglobal(ls, parent->nc_name);
    if (!lua_istable(ls, -1)) {
        sprintf(errtxt, "%s: '%s' missing or not an array", file, parent->nc_name);
        lua_pop(ls, 1);
        return -1;
    }

    /*
     * Enumerate the array. Start with nil for the first key.
     */
    lua_pushnil(ls);

    /*
     * Each loop will take the key from the top of the stack and
     * push the next key followed by the corresponding value back
     * to the stack.
     */
    while (lua_next(ls, -2)) {
        /* 
         * Now the next value is on top and its key (array index) is below. Components
         * are specified without a named index. Thus, they have a numeric index.
         */ 
        if (lua_isnumber(ls, -2)) {

            /*
             * This is a numeric index. Now let's check the value, which is
             * expected to be an array containing a named index 'name'.
             */
            name = GetStringByNameFromTable(ls, -1, TKN_NAME, NULL, 0);
            if (name) {
                /*
                 * The value on top of the stack is an array, which contains
                 * a named item 'name'. We probably found a new component.
                 */
                if (compo == NULL) {
                    /* This is the first child component of our parent. */
                    compo = calloc(1, sizeof(NUTCOMPONENT));
                    parent->nc_child = compo;
                } else {
                    /* Siblings exist already. */
                    compo->nc_nxt = calloc(1, sizeof(NUTCOMPONENT));
                    compo->nc_nxt->nc_prv = compo;
                    compo = compo->nc_nxt;
                }

                /*
                 * Transfer the component's items from Lua to a C structure.
                 */
                compo->nc_parent = parent;
                compo->nc_name = name;
                compo->nc_brief = GetStringByNameFromTable(ls, -1, TKN_BRIEF, NULL, 0);
                compo->nc_description = GetStringByNameFromTable(ls, -1, TKN_DESC, NULL, 0);
                compo->nc_requires = GetStringArrayByNameFromTable(ls, -1, TKN_REQUIRES);
                compo->nc_provides = GetStringArrayByNameFromTable(ls, -1, TKN_PROVIDES);
                compo->nc_active_if = GetStringByNameFromTable(ls, -1, TKN_ACTIF, NULL, 0);
                compo->nc_subdir = GetStringByNameFromTable(ls, -1, TKN_SUBDIR, NULL, 0);
                compo->nc_sources = GetStringArrayByNameFromTable(ls, -1, TKN_SOURCES);
                compo->nc_targets = GetStringArrayByNameFromTable(ls, -1, TKN_TARGETS);
                compo->nc_makedefs = GetStringArrayByNameFromTable(ls, -1, TKN_MAKEDEFS);

                /* If this component got any subcomponent, then load it now. */
                if (GetStringByNameFromTable(ls, -1, TKN_SCRIPT, script, sizeof(script))) {
                    if(LoadComponentTree(ls, compo, path, script)) {
                        return -1;
                    }
                }
            }
        }

        /*
         * Remove the value from stack, so the next lua_next will find the
         * key (arry index) on top.
         */
        lua_pop(ls, 1);
    }

    /*
     * Remove the key in order to leave the stack like we found it.
     */
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
                opts->nco_active = 1;
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

#if 0
int LuaPanic(lua_State *ls)
{
    int i = 1;
    return 0;
}

int LuaError(lua_State *ls)
{
    return -1;
    exit(0);
}
#endif

/*!
 * \brief Open a Nut/OS component repository.
 *
 * \param pathname Pathname of the repository file. Use slashes, not backslashes.
 *
 * \return Pointer to a NUTREPOSITORY structure on success or NULL otherwise.
 */
NUTREPOSITORY *OpenRepository(const char *pathname)
{
    char *cp;
    NUTREPOSITORY *repo = malloc(sizeof(NUTREPOSITORY));

    if(pathname == NULL || access(pathname, 0)) {
        return NULL;
    }
    if((repo = calloc(1, sizeof(NUTREPOSITORY))) != NULL) {
        /*
         * Cut off the directory path of the repository script. This
         * directory is our root directory. All component scripts will
         * be below this point.
         */
        repo->nr_dir = strdup(pathname);
        if((repo->nr_dir = strdup(pathname)) == NULL) {
            free(repo);
            return NULL;
        }
        if ((cp = strrchr(repo->nr_dir, '/')) != NULL) {
            *cp++ = 0;
            repo->nr_name = cp;
        } else {
            repo->nr_dir[0] = 0;
        }

        /*
         * Create a LUA state.
         */
        repo->nr_ls = lua_open();

        //lua_atpanic(repo->nr_ls, LuaPanic);
        //lua_cpcall(repo->nr_ls, LuaError, NULL);
    }
    return repo;
}

/*!
 * \brief Close a Nut/OS component repository.
 *
 * \param repo Pointer to a NUTREPOSITORY structure.
 */
void CloseRepository(NUTREPOSITORY *repo)
{
    if(repo) {
        if(repo->nr_dir) {
            free(repo->nr_dir);
        }
        if(repo->nr_ls) {
            lua_close((lua_State *)(repo->nr_ls));
        }
    }
}

/*!
 * \brief Load Nut/OS repository components.
 *
 * \param repo Pointer to a NUTREPOSITORY structure.
 *
 * \return Root pointer to a linked tree of NUTCOMPONENT structures.
 */
NUTCOMPONENT *LoadComponents(NUTREPOSITORY *repo)
{
    lua_State *ls = (lua_State *)(repo->nr_ls);
    NUTCOMPONENT *root;

    /* Create a repository root component. */
    root = calloc(1, sizeof(NUTCOMPONENT));
    root->nc_name = strdup("repository");

    /* 
     * Collect the components first. As a result we will have a tree
     * structure of all components.
     */
    if(LoadComponentTree(ls, root, repo->nr_dir, repo->nr_name)) {
        free(root->nc_name);
        free(root);
        root = NULL;
    }

    /*
     * Now walk along the component tree and collect the options of
     * all components.
     */
    if(root) {
        LoadOptions(ls, root);
    }
    return root;
}

/*!
 * \brief Read Nut/OS component configuration values.
 *
 * \param repo Pointer to a NUTREPOSITORY structure.
 * \param root Pointer to a linked tree of NUTCOMPONENT structures.
 * \param pathname Pathname of the repository file. Use slashes, not backslashes.
 *
 * \return 0 on success or -1 otherwise.
 */
int ConfigureComponents(NUTREPOSITORY *repo, NUTCOMPONENT *root, const char *pathname)
{
    int rc;
    lua_State *ls = (lua_State *)(repo->nr_ls);

    if(ls == NULL || root == NULL || pathname == NULL || access(pathname, 0)) {
        return -1;
    }

    /* Let the interpreter load the script file. */
    if ((rc = luaL_loadfile(ls, pathname)) != 0) {
        strcpy(errtxt, lua_tostring(ls, -1));
        return -1;
    }
    if(lua_pcall(ls, 0, 0, 0)) {
        strcpy(errtxt, lua_tostring(ls, -1));
        return -1;
    }
    LoadConfigValues(ls, root);

    return 0;
}

int IsProvided(NUTCOMPONENT *compo, char *requirement)
{
    NUTCOMPONENTOPTION *opts;
    int i;

    while (compo) {
        if(compo->nc_enabled) {
            if(compo->nc_provides) {
                for (i = 0; compo->nc_provides[i]; i++) {
                    if(strcmp(compo->nc_provides[i], requirement) == 0) {
                        return 1;
                    }
                }
            }
            opts = compo->nc_opts;
            while (opts) {
                if(opts->nco_enabled && opts->nco_active && opts->nco_provides) {
                    for (i = 0; opts->nco_provides[i]; i++) {
                        if(strcmp(opts->nco_provides[i], requirement) == 0) {
                            return 1;
                        }
                    }
                }
                opts = opts->nco_nxt;
            }
            if (IsProvided(compo->nc_child, requirement)) {
                return 1;
            }
        }
        compo = compo->nc_nxt;
    }
    return 0;
}

void EnableSubComponents(NUTCOMPONENT *compo, int enable)
{
    NUTCOMPONENTOPTION *opts;

    while (compo) {
        compo->nc_enabled = enable;
        opts = compo->nc_opts;
        while (opts) {
            opts->nco_enabled = enable;
            opts = opts->nco_nxt;
        }
        EnableSubComponents(compo->nc_child, enable);
        compo = compo->nc_nxt;
    }
}

void EnableComponentTree(NUTCOMPONENT *compo, int enable)
{
    NUTCOMPONENTOPTION *opts;

    compo->nc_enabled = enable;
    opts = compo->nc_opts;
    while (opts) {
        opts->nco_enabled = enable;
        opts = opts->nco_nxt;
    }
    EnableSubComponents(compo->nc_child, enable);
}

int RefreshComponentTree(NUTCOMPONENT *root, NUTCOMPONENT *compo)
{
    int rc = 0;
    int i;
    NUTCOMPONENTOPTION *opts;

    while (compo) {
        if(compo->nc_requires) {
            int provided = 1;
            for (i = 0; compo->nc_requires[i]; i++) {
                if((provided = IsProvided(root, compo->nc_requires[i])) == 0) {
                    break;
                }
            }
            if(provided != compo->nc_enabled) {
                /* Update this component branch. */
                //compo->nc_enabled = provided;
                EnableComponentTree(compo, provided);
                rc++;
            }
        }
        opts = compo->nc_opts;
        while (opts) {
            if(opts->nco_requires) {
                int provided = 1;
                for (i = 0; opts->nco_requires[i]; i++) {
                    if((provided = IsProvided(root, opts->nco_requires[i])) == 0) {
                        break;
                    }
                }
                if(provided != opts->nco_enabled) {
                    opts->nco_enabled = provided;
                    rc++;
                }
            }
            opts = opts->nco_nxt;
        }
        rc += RefreshComponentTree(root, compo->nc_child);
        compo = compo->nc_nxt;
    }
    return rc;
}

int RefreshComponents(NUTCOMPONENT *root)
{
    int i;

    /* Enable all components. */
    EnableComponentTree(root, 1);

    for(i = 0; i < 10; i++) {
        if(RefreshComponentTree(root, root) == 0) {
            return 0;
        }
    }
    return -1;
}

/*!
 * \brief Add the source file list to the Makefile.
 *
 * \param fp      Pointer to an opened file.
 * \param compo   Pointer to a library component.
 * \param sub_dir Component's subdirectory.
 *
 * \return Number of explicit target files.
 */
int WriteMakeSources(FILE * fp, NUTCOMPONENT * compo, const char *sub_dir)
{
    int rc = 0;
    int i;
    int k;
    int c = 8;
    NUTCOMPONENT *cop = compo;

    fprintf(fp, "SRCS =\t");
    while (cop) {
        if(cop->nc_enabled && cop->nc_sources) {
            for (i = 0; cop->nc_sources[i]; i++) {
                if(strchr(cop->nc_sources[i], '/')) {
                    char path[255];
                    sprintf(path, "%s/%s", sub_dir, cop->nc_sources[i]);
                    CreateDirectoryPath(path);
                }

                /* Check if this source results in an explicit target. */
                if(cop->nc_targets && cop->nc_targets[i]) {
                    rc++;
                }
                else {
                    c += strlen(cop->nc_sources[i]);
                    if (c > 72) {
                        fprintf(fp, " \\\n\t");
                        c = 8;
                    }
                    fprintf(fp, " %s", cop->nc_sources[i]);
                }
            }
        }
        cop = cop->nc_nxt;
    }
    fputc('\n', fp);

    for(k = 0; k < rc; k++) {
        fprintf(fp, "SRC%d =\t", k + 1);
        c = 8;
        cop = compo;
        while (cop) {
            if(cop->nc_enabled && cop->nc_sources) {
                for (i = 0; cop->nc_sources[i]; i++) {
                    if(cop->nc_targets && cop->nc_targets[i]) {
                        c += strlen(cop->nc_sources[i]);
                        if (c > 72) {
                            fprintf(fp, " \\\n\t");
                            c = 8;
                        }
                        fprintf(fp, " %s", cop->nc_sources[i]);
                    }
                }
            }
            cop = cop->nc_nxt;
        }
        fputc('\n', fp);
    }
    fputc('\n', fp);

    return rc;
}

/*!
 * \brief Add the configured lines to the Makefile.
 *
 * \param fp Pointer to an opened file.
 * \param compo Pointer to a library component.
 *
 * \todo This is not yet finished. All 'name=value' pairs should 
 *       be collected and combined.
 */
void WriteMakedefLines(FILE * fp, NUTCOMPONENT * compo)
{
    NUTCOMPONENTOPTION *opts;
    int i;

    while (compo) {
        if(compo->nc_enabled && compo->nc_makedefs) {
            for (i = 0; compo->nc_makedefs[i]; i++) {
                fprintf(fp, "%s\n", compo->nc_makedefs[i]);
            }
        }
        opts = compo->nc_opts;
        while (opts) {
            if (opts->nco_enabled && opts->nco_active && opts->nco_makedefs) {
                for (i = 0; opts->nco_makedefs[i]; i++) {
                    fprintf(fp, "%s\n", opts->nco_makedefs[i]);
                }
            }
            opts = opts->nco_nxt;
        }
        if (compo->nc_child) {
            WriteMakedefLines(fp, compo->nc_child);
        }
        compo = compo->nc_nxt;
    }
}

/*!
 * \brief Add target to the Makefile in the top build directory.
 *
 * \param fp     Pointer to an opened file.
 * \param compo  Pointer to the first child of the root component.
 * \param target Makefile target, set to NULL for 'all'.
 */
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

/*
 * \brief Creates the complete directory path to a given pathname of a file.
 *
 * \param path The file's pathname.
 *
 * \return 0 on success, -1 if we failed to create any non existing subdirectoy.
 */
static int CreateDirectoryPath(const char *path)
{
    char subpath[255];
    char *cp;

    if(*path) {
        /*
         * Copy any optional WIN32 device/drive information.
         */
        if((cp = strchr(path, ':')) != 0) {
            for(cp = subpath; *path != ':'; path++, cp++) {
                *cp = *path;
            }
        }
        else {
            cp = subpath;
        }

        /*
         * Copy the first character unchecked. This way we avoid to check the
         * root directory.
         */
        if(*path) {
            *cp++ = *path++;
        }

        /*
         * Create the directory tree, processing path component by path component.
         */
        while(*path) {
            if(*path == '/') {
                *cp = 0;
                if(access(subpath, 0)) {
                    if(mkdir(subpath, S_IRWXU)) {
                        sprintf(errtxt, "Failed to make %s", subpath);
                        return -1;
                    }
                }
            }
            *cp++ = *path++;
        }
    }
    return 0;
}

/*!
 * \brief Create makefiles from a specified NUTCOMPONENT tree.
 *
 * This routine creates all required Makefiles, one in the top build directory
 * and one in each library's subdirectory. It will also create the NutConf.mk
 * and UserConf.mk. Except for UserConf.mk, any existing file will be replaced.
 *
 * \param root       Pointer to the root component.
 * \param bld_dir    Pathname of the top build directory.
 * \param src_dir    Pathname of the top source directory.
 * \param mak_ext    Filename extension of the Makedefs/Makerules to be used, e.g. avr-gcc.
 * \param ifirst_dir Optional include directory. Header files will be included first
 *                   and thus may replace standard Nut/OS headers with the same name.
 * \param ilast_dir  Optional include directory. Header files will be included last.
 *                   This parameter is typically used to specify the compilers runtime 
 *                   library. Header files with the same name as Nut/OS standard headers
 *                   are ignored.
 * \param ins_dir    Final target directory of the Nut/OS libraries. Will be used with
 *                   'make install'.
 *
 * \return 0 on success, otherwise return -1.
 *
 * \todo This function's parameter list is a bit overloaded. Either split the function
 *       or use a parameter structure.
 */
int CreateMakeFiles(NUTCOMPONENT *root, const char *bld_dir, const char *src_dir, const char *mak_ext, 
                     const char *ifirst_dir, const char *ilast_dir, const char *ins_dir)
{
    FILE *fp;
    char path[255];
    NUTCOMPONENT *compo;
    int targets;
    int i;
    struct tm *ltime;
    time_t now;

    time(&now);
    ltime = localtime(&now);

    /* Create the Makedefs file */
    sprintf(path, "%s/NutConf.mk", bld_dir);
    if(CreateDirectoryPath(path) == 0) {
        fp = fopen(path, "w");
        if (fp) {
            fprintf(fp, "# Automatically generated on %s", asctime(ltime));
            fprintf(fp, "#\n# Do not edit, modify UserConf.mk instead!\n#\n\n");
            WriteMakedefLines(fp, root->nc_child);
            fprintf(fp, "\n\ninclude %s/UserConf.mk\n", bld_dir);
            fclose(fp);
        }
    }

    /* Create the user's Makedefs file, if it doesn't exist */
    sprintf(path, "%s/UserConf.mk", bld_dir);
    if(access(path, 0)) {
        fp = fopen(path, "w");
        if (fp) {
            fprintf(fp, "# Automatically created on %s", asctime(ltime));
            fprintf(fp, "#\n# You can use this file to modify values in NutConf.mk\n#\n\n");
            fclose(fp);
        }
    }

    /* Create the root Makefile */
    sprintf(path, "%s/Makefile", bld_dir);
    if(CreateDirectoryPath(path) == 0) {
        fp = fopen(path, "w");
        if (fp) {
            fprintf(fp, "# Do not edit! Automatically generated on %s\n", asctime(ltime));
            WriteMakeRootLines(fp, root->nc_child, NULL);
            if(ins_dir && *ins_dir) {
                WriteMakeRootLines(fp, root->nc_child, "install");
            }
            WriteMakeRootLines(fp, root->nc_child, "clean");
            fclose(fp);
        }
    }

    /* Create library Makefiles */
    compo = root->nc_child;
    while (compo) {
        if (compo->nc_subdir) {
            sprintf(path, "%s/%s", bld_dir, compo->nc_subdir);
            strcat(path, "/Makefile");
            if(CreateDirectoryPath(path) == 0) {
                fp = fopen(path, "w");
                if (fp) {
                    fprintf(fp, "# Do not edit! Automatically generated on %s\n", asctime(ltime));
                    fprintf(fp, "PROJ =\tlib%s\n\n", compo->nc_name);
                    fprintf(fp, "top_srcdir = %s\n", src_dir);
                    fprintf(fp, "top_blddir = %s\n\n", bld_dir);
                    fprintf(fp, "VPATH = $(top_srcdir)/%s\n\n", compo->nc_subdir);

                    sprintf(path, "%s/%s", bld_dir, compo->nc_subdir);
                    targets = WriteMakeSources(fp, compo->nc_child, path);

                    fprintf(fp, "OBJS = $(SRCS:.c=.o)\n");
                    for(i = 0; i < targets; i++) {
                        fprintf(fp, "OBJ%d = $(SRC%d:.c=.o)\n", i + 1, i + 1);
                    }
                    fprintf(fp, "include $(top_blddir)/NutConf.mk\n\n", mak_ext);
                    fprintf(fp, "include $(top_srcdir)/Makedefs.%s\n\n", mak_ext);

                    if(ifirst_dir && *ifirst_dir) {
                        fprintf(fp, "INCFIRST=$(INCPRE)%s\n", ifirst_dir);
                    }
                    if(ilast_dir && *ilast_dir) {
                        fprintf(fp, "INCLAST = $(INCPRE)%s\n", ilast_dir);
                    }

                    fprintf(fp, "\nall: $(PROJ).a $(OBJS)");
                    for(i = 0; i < targets; i++) {
                        fprintf(fp, " $(OBJ%d)", i + 1);
                    }
                    fprintf(fp, "\n\n");

                    if(ins_dir && *ins_dir) {
                        fprintf(fp, "install: $(PROJ).a");
                        for(i = 0; i < targets; i++) {
                            fprintf(fp, " $(OBJ%d)", i + 1);
                        }
                        fprintf(fp, "\n\t$(CP) $(PROJ).a %s/$(PROJ).a\n", ins_dir);
                        for(i = 0; i < targets; i++) {
                            fprintf(fp, "\t$(CP) $(OBJ%d) %s/$(OBJ%d)\n", i + 1, ins_dir, i + 1);
                        }
                    }
                    fprintf(fp, "\ninclude $(top_srcdir)/Makerules.%s\n\n", mak_ext);
                    fclose(fp);
                }
            }
        }
        compo = compo->nc_nxt;
    }
    return 0;
}

typedef struct _NUTHEADERMACRO NUTHEADERMACRO;

/*!
 * \brief Linked list of header file macros.
 */
struct _NUTHEADERMACRO {
    NUTHEADERMACRO *nhm_nxt;
    char *nhm_name;
    char *nhm_value;
};

typedef struct _NUTHEADERFILE NUTHEADERFILE;

/*!
 * \brief Linked list of header files.
 */
struct _NUTHEADERFILE {
    NUTHEADERFILE *nhf_nxt;
    char *nhf_path;
    NUTHEADERMACRO *nhf_macros;
};

/*!
 * \brief Add option to header file list.
 */
NUTHEADERFILE *AddHeaderFileMacro(NUTHEADERFILE *nh_root, NUTCOMPONENTOPTION * opts)
{
    NUTHEADERFILE *nhf;
    NUTHEADERMACRO *nhm;

    /* Add to existing list. */
    if (nh_root) {
        nhf = nh_root;
        while (strcasecmp(nhf->nhf_path, opts->nco_file)) {
            if (nhf->nhf_nxt) {
                nhf = nhf->nhf_nxt;
            } else {
                nhf->nhf_nxt = calloc(1, sizeof(NUTHEADERFILE));
                nhf = nhf->nhf_nxt;
                nhf->nhf_path = opts->nco_file;
            }
        }
    } 
    /* First entry, create list root. */
    else {
        nhf = calloc(1, sizeof(NUTHEADERFILE));
        nhf->nhf_path = opts->nco_file;
        nh_root = nhf;
    }

    /* Add macro to existing header file entry. */
    if (nhf->nhf_macros) {
        nhm = nhf->nhf_macros;
        while (strcasecmp(nhm->nhm_name, opts->nco_name)) {
            if (nhm->nhm_nxt) {
                nhm = nhm->nhm_nxt;
            } else {
                nhm->nhm_nxt = calloc(1, sizeof(NUTHEADERMACRO));
                nhm = nhm->nhm_nxt;
                nhm->nhm_name = opts->nco_name;
                nhm->nhm_value = opts->nco_value;
            }
        }
    } 

    /* First entry of this header file. */
    else {
        nhm = calloc(1, sizeof(NUTHEADERFILE));
        nhm->nhm_name = opts->nco_name;
        nhm->nhm_value = opts->nco_value;
        nhf->nhf_macros = nhm;
    }
    return nh_root;
}

/*!
 * \brief Create a linked list of header files and associated macros. 
 */
NUTHEADERFILE *CreateHeaderList(NUTCOMPONENT * compo, NUTHEADERFILE *nh_root)
{
    NUTCOMPONENTOPTION *opts;

    while (compo) {
        opts = compo->nc_opts;
        while (opts) {
            if (opts->nco_file && opts->nco_enabled) {
                nh_root = AddHeaderFileMacro(nh_root, opts);
            }
            opts = opts->nco_nxt;
        }
        if (compo->nc_child) {
            nh_root = CreateHeaderList(compo->nc_child, nh_root);
        }
        compo = compo->nc_nxt;
    }
    return nh_root;
}

/*!
 * \brief Create header files from a specified NUTCOMPONENT tree.
 *
 * This routine creates all build specific header files in the build
 * directory. Existing files will be replaced.
 *
 * \param root    Pointer to the root component.
 * \param bld_dir Pathname of the top build directory.
 * 
 * \return 0 on success, otherwise return -1.
 *
 * \todo Release allocated heap space.
 */
int CreateHeaderFiles(NUTCOMPONENT * root, const char *bld_dir)
{
    NUTHEADERFILE *nh_root = NULL;
    NUTHEADERFILE *nhf;
    NUTHEADERMACRO *nhm;
    FILE *fp;
    char path[255];
    char exname[255];
    char *cp;
    struct tm *ltime;
    time_t now;

    time(&now);
    ltime = localtime(&now);

    /* Create a linked list of header files with active component options. */
    nh_root = CreateHeaderList(root->nc_child, nh_root);

    for (nhf = nh_root; nhf; nhf = nhf->nhf_nxt) {
        strcpy(path, bld_dir);
        strcat(path, "/");
        strcat(path, nhf->nhf_path);

        strcpy(exname, "_");
        strcat(exname, nhf->nhf_path);
        strcat(exname, "_");
        for(cp = exname; *cp; cp++) {
            if(*cp < '0') {
                *cp = '_';
            }
            else {
                *cp = (char)toupper(*cp);
            }
        }
        if(CreateDirectoryPath(path) == 0) {
            if ((fp = fopen(path, "w")) != 0) {
                fprintf(fp, "#ifndef %s\n", exname);
                fprintf(fp, "#define %s\n\n", exname);
                fprintf(fp, "/*\n * Do not edit! Automatically generated on %s */\n\n", asctime(ltime));
                for (nhm = nhf->nhf_macros; nhm; nhm = nhm->nhm_nxt) {
                    if (nhm->nhm_value) {
                        fprintf(fp, "#ifndef %s\n", nhm->nhm_name);
                        fprintf(fp, "#define %s %s\n", nhm->nhm_name, nhm->nhm_value);
                        fprintf(fp, "#endif\n\n");
                    }
                }
                fprintf(fp, "\n#endif\n");
                fclose(fp);
            }
            else {
                sprintf(errtxt, "Failed to create %s", path);
                return -1;
            }
        }
    }
    return 0;
}

/*!
 * \brief Create build directory for Nut/OS applications.
 *
 * This routine creates Makedefs and Makerules in the specified directory.
 * It will also create the NutConf.mk and UserConf.mk. Except for UserConf.mk, 
 * any existing file will be replaced.
 *
 * \param root       Pointer to the root component.
 * \param app_dir    Pathname of the application build directory.
 * \param src_dir    Pathname of the top source directory.
 * \param mak_ext    Filename extension of the platform specific Makedefs/Makerules, e.g. avr-gcc.
 * \param prg_ext    Filename extension of the programmer specific Makedefs/Makerules, e.g. uisp-avr.
 *
 * \return 0 on success, otherwise return -1.
 */
int CreateSampleDirectory(NUTCOMPONENT * root, const char *app_dir, const char *src_dir, 
                          const char *lib_dir, const char *mak_ext, const char *prg_ext)
{
    FILE *fp;
    char path[255];
    struct tm *ltime;
    time_t now;

    time(&now);
    ltime = localtime(&now);

    sprintf(path, "%s/NutConf.mk", app_dir);
    if(CreateDirectoryPath(path) == 0) {
        /* Create the configuration Makedefs file */
        fp = fopen(path, "w");
        if (fp) {
            fprintf(fp, "# Automatically generated on %s", asctime(ltime));
            fprintf(fp, "#\n# Do not edit, modify UserConf.mk instead!\n#\n\n");
            WriteMakedefLines(fp, root->nc_child);
            fprintf(fp, "\n\ninclude $(top_appdir)/UserConf.mk\n");
            fclose(fp);
        }

        /* Create the user's Makedefs file, if it doesn't exist */
        sprintf(path, "%s/UserConf.mk", app_dir);
        if(access(path, 0)) {
            fp = fopen(path, "w");
            if (fp) {
                fprintf(fp, "# Automatically created on %s", asctime(ltime));
                fprintf(fp, "#\n# You can use this file to modify values in NutConf.mk\n#\n\n");
                fclose(fp);
            }
        }

        /* Create the application Makedefs. */
        sprintf(path, "%s/Makedefs", app_dir);
        if ((fp = fopen(path, "w")) != 0) {
            fprintf(fp, "# Do not edit! Automatically generated on %s\n", asctime(ltime));
            fprintf(fp, "top_srcdir = %s\n", src_dir);
            fprintf(fp, "top_appdir = %s\n", app_dir);
            fprintf(fp, "LIBDIR = %s\n\n", lib_dir);

            fprintf(fp, "include $(top_appdir)/NutConf.mk\n");
            fprintf(fp, "include $(top_srcdir)/app/Makedefs.%s\n", mak_ext);
            fprintf(fp, "include $(top_srcdir)/app/Makeburn.%s\n\n", prg_ext);
            fclose(fp);
        }
        else {
            sprintf(errtxt, "Failed to create %s", path);
            return -1;
        }

        /* Create the application Makerules. */
        sprintf(path, "%s/Makerules", app_dir);
        if ((fp = fopen(path, "w")) != 0) {
            fprintf(fp, "# Do not edit! Automatically generated on %s\n\n", asctime(ltime));
            fprintf(fp, "include $(top_srcdir)/app/Makerules.%s\n", mak_ext);
            fclose(fp);
        }
        else {
            sprintf(errtxt, "Failed to create %s", path);
            return -1;
        }
    }
    else {
        sprintf(errtxt, "Failed to create directory for %s", path);
        return -1;
    }
    return 0;
}

#ifdef NUT_CONFIGURE_EXEC
/*!
 * \brief Running without GUI.
 *
 * All settings are passed as command line options.
 *
 * \todo Everything.
 */
int main(int argc, char **argv)
{
}
#endif
