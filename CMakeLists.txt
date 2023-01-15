cmake_minimum_required(VERSION 3.24)

project(ja2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# lua51.lib and lua51.vc9.lib have been built with /MTx, so we must as well
# TODO: build our own Lua 5.1.2 from source so we can use whichever
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_compile_definitions(CINTERFACE XML_STATIC VFS_STATIC VFS_WITH_SLF VFS_WITH_7ZIP USE_VFS _CRT_SECURE_NO_DEPRECATE)
include_directories(${CMAKE_SOURCE_DIR} "ext/VFS/include" Utils TileEngine TacticalAI ModularizedTacticalAI Tactical Strategic "Standard Gaming Platform" Res Lua Laptop Multiplayer "Multiplayer/raknet" Editor Console)

# external libraries
add_subdirectory("ext/libpng")
add_subdirectory("ext/zlib")
add_subdirectory("ext/VFS")
target_link_libraries(bfVFS PRIVATE 7z)

# internal libraries that are Good
add_subdirectory(Lua)

# internal libraries that live in Preprocessor Hell, because they are Bad
add_subdirectory(TileEngine)
add_subdirectory(TacticalAI)
add_subdirectory(Utils)
add_subdirectory(Strategic)
add_subdirectory("Standard Gaming Platform")
add_subdirectory(Laptop)
add_subdirectory(Editor)
add_subdirectory(Console)
add_subdirectory(Tactical)
add_subdirectory(ModularizedTacticalAI)
# TODO: Rename 'Standard Gaming Platform' directory to 'SGP' so this can be refactored away
set(Ja2_Libs
TileEngine
TacticalAI
Utils
Strategic
SGP
Laptop
Editor
Console
Tactical
ModularizedTacticalAI
)

# TODO: Move these units into their own directory to declutter the root dir and CMakeLists.txt file
set(Ja2Src
"aniviewscreen.cpp"
"Credits.cpp"
"Fade Screen.cpp"
"FeaturesScreen.cpp"
"GameInitOptionsScreen.cpp"
"gameloop.cpp"
"gamescreen.cpp"
"GameSettings.cpp"
"GameVersion.cpp"
"HelpScreen.cpp"
"Init.cpp"
"Intro.cpp"
"JA2 Splash.cpp"
"Ja25Update.cpp"
"jascreens.cpp"
"Language Defines.cpp"
"Loading Screen.cpp"
"MainMenuScreen.cpp"
"MessageBoxScreen.cpp"
"MPChatScreen.cpp"
"MPConnectScreen.cpp"
"MPHostScreen.cpp"
"MPJoinScreen.cpp"
"MPScoreScreen.cpp"
"MPXmlTeams.cpp"
"Multiplayer/client.cpp"
"Multiplayer/server.cpp"
"Multiplayer/transfer_rules.cpp"
"Options Screen.cpp"
"profiler.cpp"
"SaveLoadGame.cpp"
"SaveLoadScreen.cpp"
"SCREENS.cpp"
"Sys Globals.cpp"
"ub_config.cpp"
"XML_DifficultySettings.cpp"
"XML_IntroFiles.cpp"
"XML_Layout_MainMenu.cpp"
Res/ja2.rc
)

set(Ja2_Libraries
"${PROJECT_SOURCE_DIR}/libexpatMT.lib"
"Dbghelp.lib"
Lua
"${PROJECT_SOURCE_DIR}/lua51.lib"
"${PROJECT_SOURCE_DIR}/lua51.vc9.lib"
"Winmm.lib"
"${PROJECT_SOURCE_DIR}/SMACKW32.LIB"
"${PROJECT_SOURCE_DIR}/binkw32.lib"
bfVFS
"${PROJECT_SOURCE_DIR}/Multiplayer/raknet/RakNetLibStatic.lib"
"ws2_32.lib"
)

include(cmake/ValidateOptions.cmake)

set(ValidLanguages CHINESE DUTCH ENGLISH FRENCH GERMAN ITALIAN POLISH RUSSIAN)
ValidateOptions("${ValidLanguages}" "Languages" "${Languages}" "LangTargets")

set(ValidApplications JA2 JA2MAPEDITOR JA2UB JA2UBMAPEDITOR)
ValidateOptions("${ValidApplications}" "Applications" "${Applications}" "ApplicationTargets")


# Due to widespread preprocessor definition abuse in the codebase, practically
# every library-language-executable combination is its own compilation target
# TODO: refactor preprocessor usage onto, ideally, a single translation unit
foreach(lang IN LISTS LangTargets)
	foreach(exe IN LISTS ApplicationTargets)
		set(targPrefix ${exe}_${lang})

		# make a copy of the library list for each language/library combination
		list(SUBLIST Ja2_Libs 0 -1 ${targPrefix}_Targets)

		add_executable(${targPrefix} WIN32)
		target_sources(${targPrefix} PRIVATE ${Ja2Src})
		target_link_libraries(${targPrefix} PRIVATE ${Ja2_Libraries})

		foreach(lib IN LISTS ${targPrefix}_Targets)
			set(tgt ${targPrefix}_${lib})

			add_library(${tgt})
			target_sources(${tgt} PRIVATE ${${lib}Src})

			set(isEditor $<STREQUAL:${exe},JA2MAPEDITOR>)
			set(isUb $<STREQUAL:${exe},JA2UB>)
			set(isUbEditor $<STREQUAL:${exe},JA2UBMAPEDITOR>)

			target_compile_definitions(${tgt} PUBLIC
				$<IF:${isEditor},JA2EDITOR;JA2BETAVERSION,>
				$<IF:${isUb},JA2UB;JA2UBMAPS,>
				$<IF:${isUbEditor},JA2UB;JA2UBMAPS;JA2EDITOR;JA2BETAVERSION,>
				$<IF:$<CONFIG:Debug>,JA2BETAVERSION;JA2TESTVERSION;DEBUG_ATTACKBUSY,>
				${lang}
			)
			target_link_libraries(${targPrefix} PUBLIC ${tgt})
		endforeach()

		# SGP is the only one calling these, so they can go here
		target_link_libraries(${targPrefix}_SGP PRIVATE "ddraw.lib" "${PROJECT_SOURCE_DIR}/fmodvc.lib")
		target_link_libraries(${targPrefix}_SGP PUBLIC libpng)
	endforeach()
endforeach()
