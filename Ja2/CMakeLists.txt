# Stamp the build's identity into GameVersion.cpp. Both strings come from the
# caller with -D, and CI passes them: it pins them once, where it decides what the
# release is, so the parallel build jobs cannot stamp identities that disagree.
#
# A build nobody identified is a local build, and says so rather than guessing
# from HEAD. Keeping such a guess honest would mean re-running configure on every
# commit, and it would buy nothing: only what we package is archived with its PDB,
# and a local crash report is symbolized against the PDB next to the exe.
#
# GIT_SHA lands in czVersionString, which is both the savegame compatibility key
# and the crash-report key. It is the bare short SHA and nothing else — a `git
# describe` string cannot serve here, because czVersionString is only 16 bytes and
# describe puts the SHA last, so it is exactly the discriminating part that would
# get clipped off. The budget is 15 chars + NUL: the string is stored in the
# savegame header as SAVED_GAME_HEADER::zGameVersionNumber (GAME_VERSION_LENGTH),
# so its size is a binary format constant and cannot grow. CI passes 9 hex, well
# past what uniqueness needs (git's own auto-abbrev picks 8 for this repo).
# Overshooting it is an error rather than a truncation, because a clipped SHA is
# how two different builds end up claiming to be the same one.
if(NOT GIT_SHA)
	set(GIT_SHA "local")
endif()
string(LENGTH "${GIT_SHA}" _length)
if(_length GREATER 15)
	message(FATAL_ERROR "GIT_SHA is ${_length} characters; czVersionString holds 15")
endif()

# zBuildInformation is the version line shown on screen, CHAR16[256]. Nothing
# parses it, so it carries the version in full and truncation costs only
# readability.
if(NOT GAME_BUILD_INFORMATION)
	string(TIMESTAMP _buildDate "%Y-%m-%d" UTC)
	set(GAME_BUILD_INFORMATION "local build ${_buildDate}")
endif()
string(LENGTH "${GAME_BUILD_INFORMATION}" _length)
if(_length GREATER 255)
	string(SUBSTRING "${GAME_BUILD_INFORMATION}" 0 255 GAME_BUILD_INFORMATION)
endif()

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/GameVersion.cpp.in"
	"${CMAKE_CURRENT_BINARY_DIR}/GameVersion.cpp"
	@ONLY)

set(Ja2Src
"${CMAKE_CURRENT_SOURCE_DIR}/aniviewscreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Credits.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Fade Screen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/FeaturesScreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/GameInitOptionsScreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/gameloop.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/gamescreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/GameSettings.cpp"
"${CMAKE_CURRENT_BINARY_DIR}/GameVersion.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/HelpScreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Init.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Intro.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/JA2 Splash.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Ja25Update.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/jascreens.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Loading Screen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/MainMenuScreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/MessageBoxScreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/MPChatScreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/MPConnectScreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/MPHostScreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/MPJoinScreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/MPScoreScreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/MPXmlTeams.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Options Screen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/profiler.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/SaveLoadGame.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/SaveLoadScreen.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Screens.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Sys Globals.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/TimeLogging.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/ub_config.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/XML_DifficultySettings.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/XML_IntroFiles.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/XML_Layout_MainMenu.cpp"
PARENT_SCOPE)
