cmake_minimum_required(VERSION 3.20)

project(ja2)

include(cmake/CopyUserPresetTemplate.cmake)
CopyUserPresetTemplate()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ADDRESS_SANITIZER OFF)
if(ADDRESS_SANITIZER)
	message(STATUS "AddressSanitizer ENABLED for non-Release builds")
	add_compile_options($<IF:$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>,-fsanitize=address,>)
endif()

# whether we are using MSBuild as a generator
set(usingMsBuild $<STREQUAL:${CMAKE_VS_PLATFORM_NAME},Win32>)

# lua51.lib and lua51.vc9.lib have been built with /MTx, so we must as well
# TODO: build our own Lua 5.1.2 from source so we can use whichever
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_compile_definitions(CINTERFACE XML_STATIC VFS_STATIC VFS_WITH_SLF VFS_WITH_7ZIP _CRT_SECURE_NO_DEPRECATE)
include_directories(Ja2 "ext/VFS/include" Utils TileEngine TacticalAI "ModularizedTacticalAI/include" Tactical Strategic sgp "Ja2/Res" Lua Laptop Multiplayer Editor Console)

# external libraries
add_subdirectory("ext/libpng")
add_subdirectory("ext/zlib")
add_subdirectory("ext/VFS")
target_link_libraries(bfVFS PRIVATE 7z)

# ja2export utility
add_subdirectory("ext/export/src")

# static libraries whose source files, header files or header files included
# by header files do not rely on Applications or Languages preprocessor definitions,
# and therefore only need to be compiled once. Good.
add_subdirectory(Lua)
add_subdirectory(Multiplayer)

# static libraries whose source files, header files or header files included
# by header files rely on Application and Language preprocessor definitions, and
# therefore need to be compiled multiple times. Very Bad.
set(Ja2_Libs
TileEngine
TacticalAI
Utils
Strategic
sgp
Laptop
Editor
Console
Tactical
ModularizedTacticalAI
)
foreach(lib IN LISTS Ja2_Libs)
	add_subdirectory(${lib})
endforeach()

add_subdirectory(Ja2)

# simple function to validate Languages and Application choices
include(cmake/ValidateOptions.cmake)

set(ValidLanguages CHINESE DUTCH ENGLISH FRENCH GERMAN ITALIAN POLISH RUSSIAN)
ValidateOptions("${ValidLanguages}" "Languages" "${Languages}" "LangTargets")

set(ValidApplications JA2 JA2MAPEDITOR JA2UB JA2UBMAPEDITOR)
ValidateOptions("${ValidApplications}" "Applications" "${Applications}" "ApplicationTargets")


# preprocessor definitions for Debug build, per the legacy MSBuild
set(debugFlags $<IF:$<CONFIG:Debug>,JA2BETAVERSION;JA2TESTVERSION;DEBUG_ATTACKBUSY,>)

# Due to widespread preprocessor definition abuse in the codebase, practically
# every library-language-executable combination is its own compilation target
# TODO: refactor preprocessor usage onto, ideally, a single translation unit
foreach(lang IN LISTS LangTargets)
	foreach(exe IN LISTS ApplicationTargets)
		set(Executable ${exe}_${lang})

		# executable for an application/language combination, e.g. JA2_ENGLISH.exe
		add_executable(${Executable} WIN32)
		target_sources(${Executable} PRIVATE ${Ja2Src})

		# Good libraries have already been built, can be simply linked here
		target_link_libraries(${Executable} PRIVATE ${Ja2_Libraries} $<IF:${usingMsBuild},legacy_stdio_definitions.lib,>)
		target_link_options(${Executable} PRIVATE $<IF:${usingMsBuild},/SAFESEH:NO,>)

		# for each app/lang combination, the Very Bad libraries need to be built,
		# with the appropriate preprocessor definitions
		foreach(lib IN LISTS Ja2_Libs)
			# syntactic sugar to hopefully make this more readable
			set(VeryBadLib ${Executable}_${lib})
			set(isEditor $<STREQUAL:${exe},JA2MAPEDITOR>)
			set(isUb $<STREQUAL:${exe},JA2UB>)
			set(isUbEditor $<STREQUAL:${exe},JA2UBMAPEDITOR>)

			# static library for an app/lang combination, e.g. JA2_ENGLISH_sgp.lib
			add_library(${VeryBadLib})
			target_sources(${VeryBadLib} PRIVATE ${${lib}Src})

			target_compile_definitions(${VeryBadLib} PUBLIC
				$<IF:${isEditor},JA2EDITOR;JA2BETAVERSION,>
				$<IF:${isUb},JA2UB;JA2UBMAPS,>
				$<IF:${isUbEditor},JA2UB;JA2UBMAPS;JA2EDITOR;JA2BETAVERSION,>
				${debugFlags}
				${lang}
			)
			target_link_libraries(${Executable} PUBLIC ${VeryBadLib})
		endforeach()

		# for sgp only
		target_link_libraries(${Executable}_sgp PRIVATE "ddraw.lib" "${PROJECT_SOURCE_DIR}/fmodvc.lib")
		target_link_libraries(${Executable}_sgp PUBLIC libpng)
		target_compile_definitions(${Executable}_sgp PRIVATE NO_ZLIB_COMPRESSION)
	endforeach()
endforeach()
