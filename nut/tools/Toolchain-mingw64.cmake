#from http://www.cmake.org/Wiki/CmakeMingw

# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)
SET(MINGW_VER x86_64-w64-mingw32)
# which compilers to use for C and C++
SET(CMAKE_C_COMPILER ${MINGW_VER}-gcc)
SET(CMAKE_CXX_COMPILER ${MINGW_VER}-g++)
SET(CMAKE_RC_COMPILER ${MINGW_VER}-windres)
SET(PKG_CONFIG_EXECUTABLE ${MINGW_VER}-pkg-config)

# here are the find_package helper files
SET(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32/sys-root/mingw/lib/cmake)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
