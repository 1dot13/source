# TODO: make this work
# Usage:
# cmake --toolchain cmake/toolchains/mingw.cmake ...
#
# Unlike clang-cl.cmake this needs no MSVC_SDK: mingw-w64 ships its own Windows
# headers and import libraries in its sysroot, so the compiler finds everything
# itself. Install the i686 cross toolchain (mingw-w64-gcc) and it is on PATH as
# i686-w64-mingw32-*.

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR i686)

set(triple ${CMAKE_SYSTEM_PROCESSOR}-w64-mingw32)

# Find every tool here rather than trusting a bare name, so the toolchain file
# alone decides which cross compiler, rc and dlltool are used.
foreach(tool gcc g++ windres dlltool ar)
  string(TOUPPER "${tool}" _variable)
  string(REPLACE "+" "X" _variable "${_variable}")
  find_program(MINGW_${_variable} NAMES ${triple}-${tool} REQUIRED)
endforeach()

set(CMAKE_C_COMPILER "${MINGW_GCC}")
set(CMAKE_CXX_COMPILER "${MINGW_GXX}")
set(CMAKE_RC_COMPILER "${MINGW_WINDRES}")
set(CMAKE_AR "${MINGW_AR}")

set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER_TARGET ${triple})

# GNU ld cannot bind the Bink import library the game ships either, so rebuild a
# bindable one from binkw32.def (which explains why) and hand it to the top-level
# CMakeLists through binkw32_lib. MSVC keeps using the shipped library.
execute_process(
  COMMAND "${MINGW_DLLTOOL}" -m i386
          -d "${CMAKE_CURRENT_LIST_DIR}/../../binkw32.def"
          -l "${CMAKE_BINARY_DIR}/binkw32.lib"
  RESULT_VARIABLE _binkw32DlltoolResult)
if(NOT _binkw32DlltoolResult EQUAL 0)
  message(FATAL_ERROR "dlltool failed to build the binkw32 import library")
endif()
set(binkw32_lib "${CMAKE_BINARY_DIR}/binkw32.lib")
