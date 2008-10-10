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
#ifndef LAU_LOADLIB_NOT_IMPLEMENTED
  {LUA_LOADLIBNAME, luaopen_package},
#endif
#ifndef LAU_TABLIB_NOT_IMPLEMENTED
  {LUA_TABLIBNAME, luaopen_table},
#endif
#ifndef LAU_IOLIB_NOT_IMPLEMENTED
  {LUA_IOLIBNAME, luaopen_io},
#endif
#ifndef LAU_OSLIB_NOT_IMPLEMENTED
  {LUA_OSLIBNAME, luaopen_os},
#endif
#ifndef LAU_STRLIB_NOT_IMPLEMENTED
  {LUA_STRLIBNAME, luaopen_string},
#endif
#ifndef LAU_MATHLIB_NOT_IMPLEMENTED
  {LUA_MATHLIBNAME, luaopen_math},
#endif
#ifndef LAU_DEBUGLIB_NOT_IMPLEMENTED
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

