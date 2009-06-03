/*
** $Id$
** Initialization of libraries for lua.c
** See Copyright Notice in lua.h
*/


#define linit_c
#define LUA_LIB

#include <lua/lua.h>

#include <lua/lualib.h>
#include <lua/lauxlib.h>


static const luaL_Reg lualibs[] = {
  {"", luaopen_base},
#ifdef NUTLUA_LOADLIB_IS_STANDARD
  {LUA_LOADLIBNAME, luaopen_package},
#endif
#ifdef NUTLUA_TABLIB_IS_STANDARD
  {LUA_TABLIBNAME, luaopen_table},
#endif
#ifdef NUTLUA_IOLIB_IS_STANDARD
  {LUA_IOLIBNAME, luaopen_io},
#endif
#ifdef NUTLUA_OSLIB_IS_STANDARD
  {LUA_OSLIBNAME, luaopen_os},
#endif
#ifdef NUTLUA_STRLIB_IS_STANDARD
  {LUA_STRLIBNAME, luaopen_string},
#endif
#ifdef NUTLUA_MATHLIB_IS_STANDARD
  {LUA_MATHLIBNAME, luaopen_math},
#endif
#ifdef NUTLUA_DEBUGLIB_IS_STANDARD
  {LUA_DBLIBNAME, luaopen_debug},
#endif
  {NULL, NULL}
};


LUALIB_API void luaL_openlibs (lua_State *L) {
  const luaL_Reg *lib = lualibs;
  for (; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_pushstring(L, lib->name);
    lua_call(L, 1, 0);
  }
}

