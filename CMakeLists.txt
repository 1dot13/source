cmake_minimum_required(VERSION 3.20)

project(ja2)

include(cmake/CopyUserPresetTemplate.cmake)
CopyUserPresetTemplate()

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

option(ADDRESS_SANITIZER OFF)
if(ADDRESS_SANITIZER)
  message(STATUS "AddressSanitizer ENABLED for non-Release builds")
  add_compile_options($<IF:$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>,-fsanitize=address,>)
endif()

if(MSVC)
  add_compile_options("/wd4838")
endif()

# whether we are using MSBuild as a generator
set(usingMsBuild $<STREQUAL:${CMAKE_VS_PLATFORM_NAME},Win32>)

# lua51.lib and lua51.vc9.lib have been built with /MTx, so we must as well
# TODO: build our own Lua 5.1.2 from source so we can use whichever
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_compile_definitions(CINTERFACE XML_STATIC VFS_STATIC VFS_WITH_SLF VFS_WITH_7ZIP _CRT_SECURE_NO_DEPRECATE)
include_directories(
  "${CMAKE_SOURCE_DIR}/Ja2"
  "${CMAKE_SOURCE_DIR}/ext/VFS/include"
  "${CMAKE_SOURCE_DIR}/Utils"
  "${CMAKE_SOURCE_DIR}/TileEngine"
  "${CMAKE_SOURCE_DIR}/TacticalAI"
  "${CMAKE_SOURCE_DIR}/ModularizedTacticalAI/include"
  "${CMAKE_SOURCE_DIR}/Tactical"
  "${CMAKE_SOURCE_DIR}/Strategic"
  "${CMAKE_SOURCE_DIR}/sgp"
  "${CMAKE_SOURCE_DIR}/Ja2/Res"
  "${CMAKE_SOURCE_DIR}/Lua"
  "${CMAKE_SOURCE_DIR}/Laptop"
  "${CMAKE_SOURCE_DIR}/Multiplayer"
  "${CMAKE_SOURCE_DIR}/Editor"
  "${CMAKE_SOURCE_DIR}/Console"
  "${CMAKE_SOURCE_DIR}/i18n/include"
)

# external libraries
add_subdirectory("ext/libpng")
add_subdirectory("ext/zlib")
add_subdirectory("ext/VFS")
target_link_libraries(bfVFS PRIVATE 7z)

# ja2export utility
add_subdirectory("ext/export/src")

# static libraries whose translation units don't rely on Application preprocessor definitions.
add_subdirectory(lua)
add_subdirectory(Multiplayer)

set(Ja2_Libraries
"${CMAKE_SOURCE_DIR}/binkw32.lib"
"${CMAKE_SOURCE_DIR}/libexpatMT.lib"
"${CMAKE_SOURCE_DIR}/lua51.lib"
"${CMAKE_SOURCE_DIR}/lua51.vc9.lib"
"${CMAKE_SOURCE_DIR}/SMACKW32.LIB"
"Dbghelp.lib"
"Winmm.lib"
"ws2_32.lib"
bfVFS
Lua
Multiplayer
)

# static libraries whose translation units rely on Application preprocessor definitions.
set(Ja2_Libs
  Console
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

# language library relies on Application _and_ Language preprocessor definition. very bad.
add_subdirectory(i18n)

# simple function to validate Languages and Application choices
include(cmake/ValidateOptions.cmake)

set(ValidLanguages CHINESE DUTCH ENGLISH FRENCH GERMAN ITALIAN POLISH RUSSIAN)
ValidateOptions("${ValidLanguages}" "Languages" "${Languages}" "LangTargets")

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

  foreach(lang IN LISTS LangTargets)
    # executable for an application/language combination, e.g. JA2_ENGLISH.exe
    set(exe ${app}_${lang})
    add_executable(${exe} WIN32
      sgp/sgp.cpp
      Ja2/Res/ja2.rc
    )
    target_link_libraries(${exe} PRIVATE ${Ja2_Libraries} $<IF:${usingMsBuild},legacy_stdio_definitions.lib,>)
    target_link_options(${exe} PRIVATE $<IF:${usingMsBuild},/SAFESEH:NO,>)

    # language library for an application, e.g. JA2MAPEDITOR_ENGLISH_i18n
    set(language_library ${exe}_i18n)
    add_library(${language_library})
    target_sources(${language_library} PRIVATE ${i18nSrc})
    target_compile_definitions(${language_library} PRIVATE ${compilationFlags} ${debugFlags} ${lang})
    target_link_libraries(${exe} PRIVATE ${language_library})

    # go through all game libraries again and link them to the app/language executable
    foreach(lib IN LISTS Ja2_Libs)
      target_link_libraries(${exe} PRIVATE ${app}_${lib})
    endforeach()
  endforeach()

  # for SGP only
  target_link_libraries(${app}_sgp PRIVATE "ddraw.lib" "${PROJECT_SOURCE_DIR}/fmodvc.lib")
  target_link_libraries(${app}_sgp PRIVATE libpng)
  target_compile_definitions(${app}_sgp PRIVATE NO_ZLIB_COMPRESSION)
endforeach()
