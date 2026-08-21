cmake_minimum_required(VERSION 3.20)

# /Z7 embedded debug info so objects are self-contained and sccache-able.
# CMP0141 is a 3.25 policy; our floor is 3.20, so opt in explicitly where known.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif()

project(ja2)

option(USE_SCCACHE "Cache compiled objects with sccache if installed" ON)
if(USE_SCCACHE)
  find_program(SCCACHE_PROGRAM sccache)
  if(SCCACHE_PROGRAM)
    message(STATUS "Using sccache: ${SCCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER   "${SCCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${SCCACHE_PROGRAM}")
  endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LTO_OPTION "Enable link-time optimization if supported by compiler" OFF)

include(CheckIPOSupported)
check_ipo_supported(RESULT LinkTimeOptimization OUTPUT IpoError LANGUAGES C CXX)
if(LinkTimeOptimization AND LTO_OPTION)
  message(STATUS "Configuring WITH link-time optimization")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(STATUS "Configuring WITHOUT link-time optimization ${IpoError}")
endif()

# declares the ADDRESS_SANITIZER option and the _asan genexpr used below
include(cmake/AddressSanitizer.cmake)

# keep frame pointers in both MSVC and clang-cl
add_compile_options(/Oy-)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-fno-delete-null-pointer-checks)
endif()

# whether we are using MSBuild as a generator
set(usingMsBuild $<STREQUAL:${CMAKE_VS_PLATFORM_NAME},Win32>)

# RakNetLibStatic.lib carries /DEFAULTLIB:LIBCMT and /DEFAULTLIB:libcpmt, so the
# static runtime is forced on us until RakNet too is built from source. Debug
# keeps its /MTd only when asan is off: asan needs the release CRT (see below).
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<AND:$<CONFIG:Debug>,$<NOT:${_asan}>>:Debug>")
# make Debug builds /Z7 instead of /Zi so they can be sccache'd
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
  "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>")

add_compile_definitions(CINTERFACE XML_STATIC VFS_STATIC VFS_WITH_SLF VFS_WITH_7ZIP _CRT_SECURE_NO_DEPRECATE)
include_directories(
  "${CMAKE_SOURCE_DIR}/Ja2"
  "${CMAKE_SOURCE_DIR}/ext/VFS/include"
  "${CMAKE_SOURCE_DIR}/ext/libsmacker"
  "${CMAKE_SOURCE_DIR}/ext/lua-5.1.5/src"
  "${CMAKE_SOURCE_DIR}/Utils"
  "${CMAKE_SOURCE_DIR}/TileEngine"
  "${CMAKE_SOURCE_DIR}/TacticalAI"
  "${CMAKE_SOURCE_DIR}/ModularizedTacticalAI/include"
  "${CMAKE_SOURCE_DIR}/Tactical"
  "${CMAKE_SOURCE_DIR}/Strategic"
  "${CMAKE_SOURCE_DIR}/sgp"
  "${CMAKE_SOURCE_DIR}/Ja2/Res"
  "${CMAKE_SOURCE_DIR}/lua"
  "${CMAKE_SOURCE_DIR}/Laptop"
  "${CMAKE_SOURCE_DIR}/Multiplayer"
  "${CMAKE_SOURCE_DIR}/Editor"
  "${CMAKE_SOURCE_DIR}/i18n/include"
)

# external libraries
add_subdirectory("ext/libpng")
add_subdirectory("ext/libsmacker")
add_subdirectory("ext/lua-5.1.5")
add_subdirectory("ext/zlib")
add_subdirectory("ext/VFS")
target_link_libraries(bfVFS PRIVATE 7z)

# turn on asan for everything defined below; 3rd party libs above keep defaults
ja2_asan_instrument_first_party()

# from here on down our own code only: vendored code above keeps default flags
include(cmake/Warnings.cmake)

# ja2export utility
add_subdirectory("export/src")

# developer tools, built alongside the game
add_subdirectory(tools)

# static libraries whose translation units don't rely on Application preprocessor definitions.
add_subdirectory(lua)
add_subdirectory(Multiplayer)
add_subdirectory(wine)

# Rebuild the Bink import library rather than link the one the game ships: that one
# is a 2002 long-format ordinal library, and only link.exe still binds it. lld-link
# takes it without complaint but emits an import descriptor with an empty thunk
# table, so every Bink call reaches a stale address and BinkInitialize faults the
# process during the intro. mingw/GNU ar cannot do this at all, see mingw.cmake
#
# asan builds skip this entirely: they compile no-op Bink stubs into the exe
# (ja2_asan_link_binkw32_stub) so no binkw32.dll is imported at all.
if(NOT ADDRESS_SANITIZER AND NOT binkw32_lib)
  # Only the name shape differs by toolchain: lib.exe prepends the x86 leading
  # underscore to every name in the .def, llvm-lib and llvm-dlltool take the name as
  # written. The checked-in file carries the underscore for the llvm tools, so strip
  # it back off for lib.exe -- get this backwards either way and the Bink symbols
  # come out undefined at link time.
  # The test is on the compiler because it is what CMAKE_AR follows: clang-cl reports
  # COMPILER_ID Clang and brings llvm-lib, cl.exe reports MSVC and brings lib.exe. Do
  # not reach for the MSVC variable or COMPILER_FRONTEND_VARIANT here -- both are set
  # for clang-cl too, and either would strip the underscore for the wrong archiver.
  set(binkw32Def "${CMAKE_SOURCE_DIR}/binkw32.def")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    file(READ "${binkw32Def}" binkw32DefText)
    string(REPLACE "\n_" "\n" binkw32DefText "${binkw32DefText}")
    set(binkw32Def "${CMAKE_BINARY_DIR}/binkw32.msvc.def")
    file(WRITE "${binkw32Def}" "${binkw32DefText}")
  endif()
  execute_process(
    COMMAND "${CMAKE_AR}" /nologo /machine:X86
            "/def:${binkw32Def}" "/out:${CMAKE_BINARY_DIR}/binkw32.lib"
    RESULT_VARIABLE binkw32LibResult)
  if(NOT binkw32LibResult EQUAL 0)
    message(FATAL_ERROR "${CMAKE_AR} failed to build the binkw32 import library")
  endif()
  set(binkw32_lib "${CMAKE_BINARY_DIR}/binkw32.lib")
endif()

set(Ja2_Libraries
"${CMAKE_SOURCE_DIR}/libexpatMT.lib"
"winmm.lib"
"ws2_32.lib"
"winhttp.lib"
bfVFS
Lua
Multiplayer
smacker
wine
)
# empty under asan (stubs compiled into the exe instead, see above)
if(binkw32_lib)
  list(APPEND Ja2_Libraries "${binkw32_lib}")
endif()

# static libraries whose translation units rely on Application preprocessor definitions.
set(Ja2_Libs
  Editor
  Ja2
  Laptop
  ModularizedTacticalAI
  sgp
  Strategic
  Tactical
  TacticalAI
  TileEngine
  Utils
)
foreach(lib IN LISTS Ja2_Libs)
  add_subdirectory(${lib})
endforeach()

# language library relies on Application preprocessor definition; compiled once per app
# with all 8 languages built in and selected at runtime (docs/plans/language-design.md).
add_subdirectory(i18n)

# simple function to validate the Application choice
include(cmake/ValidateOptions.cmake)

set(ValidApplications JA2 JA2MAPEDITOR JA2UB JA2UBMAPEDITOR)
ValidateOptions("${ValidApplications}" "Applications" "${Applications}" "ApplicationTargets")


# preprocessor definitions for Debug build, per the legacy MSBuild
set(debugFlags $<IF:$<CONFIG:Debug>,JA2BETAVERSION;JA2TESTVERSION;DEBUG_ATTACKBUSY,>)

foreach(app IN LISTS ApplicationTargets)
  set(isEditor $<STREQUAL:${app},JA2MAPEDITOR>)
  set(isUb $<STREQUAL:${app},JA2UB>)
  set(isUbEditor $<STREQUAL:${app},JA2UBMAPEDITOR>)
  set(compilationFlags
    $<IF:${isEditor},JA2EDITOR;JA2BETAVERSION,>
    $<IF:${isUb},JA2UB;JA2UBMAPS,>
    $<IF:${isUbEditor},JA2UB;JA2UBMAPS;JA2EDITOR;JA2BETAVERSION,>
  )

  foreach(lib IN LISTS Ja2_Libs)
    # library for an application, e.g. JA2UB_sgp
    set(game_library ${app}_${lib})
    add_library(${game_library})
    target_sources(${game_library} PRIVATE ${${lib}Src})
    target_compile_definitions(${game_library} PRIVATE ${compilationFlags} ${debugFlags})
  endforeach()

  # executable for the application, e.g. JA2.exe
  set(exe ${app})
  add_executable(${exe} WIN32
    sgp/sgp.cpp
    Ja2/Res/ja2.rc
  )
  # RakNetLibStatic.lib was built against an older CRT and still calls
  # _vsnprintf, which the UCRT only exports through this compatibility library.
  target_link_libraries(${exe} PRIVATE ${Ja2_Libraries} legacy_stdio_definitions.lib)
  target_compile_definitions(${exe} PRIVATE ${compilationFlags} ${debugFlags})

  ja2_asan_link_binkw32_stub(${exe})

  # language library for the application, e.g. JA2MAPEDITOR_i18n — one per app, all 8
  # languages compiled in, selected at runtime by BindLanguageStrings
  set(language_library ${exe}_i18n)
  add_library(${language_library})
  target_sources(${language_library} PRIVATE ${i18nSrc})
  target_compile_definitions(${language_library} PRIVATE ${compilationFlags} ${debugFlags})
  target_compile_options(${language_library} PRIVATE /w14062)
  target_link_libraries(${exe} PRIVATE ${language_library})

  # go through all game libraries again and link them to the app executable
  foreach(lib IN LISTS Ja2_Libs)
    target_link_libraries(${exe} PRIVATE ${app}_${lib})
  endforeach()

  # for Utils only
  target_link_libraries(${app}_Utils PRIVATE smacker)

  # for SGP only
  target_link_libraries(${app}_sgp PRIVATE "ddraw.lib" "${PROJECT_SOURCE_DIR}/fmodvc.lib")
  target_link_libraries(${app}_sgp PRIVATE libpng)
  target_compile_definitions(${app}_sgp PRIVATE ${compilationFlags} ${debugFlags} NO_ZLIB_COMPRESSION)
endforeach()
