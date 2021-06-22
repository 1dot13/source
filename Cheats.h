#ifndef _CHEATS__H_
#define _CHEATS__H_

#include "Language Defines.h"

extern UINT8 gubCheatLevel;
extern BOOLEAN gfGodModeCheat;

// GLOBALS FOR CHEAT MODE......
#ifdef JA2TESTVERSION
	#define						STARTING_CHEAT_LEVEL						6
#elif defined JA2BETAVERSION
	#define						STARTING_CHEAT_LEVEL						3
#else
	#define						STARTING_CHEAT_LEVEL						0
#endif

// ATE: remove cheats unless we're doing a debug build
#define						INFORMATION_CHEAT_LEVEL( )			( gubCheatLevel >= 3 )
#define						CHEATER_CHEAT_LEVEL( )				( gubCheatLevel >= 5 )
#define						DEBUG_CHEAT_LEVEL( )				( gubCheatLevel >= 6 )

#define						RESET_CHEAT_LEVEL( )				( gubCheatLevel = 0 )
#define						ACTIVATE_CHEAT_LEVEL()				( gubCheatLevel = 6 )

#endif