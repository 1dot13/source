	#include "Types.h"
	#include "GameVersion.h"
 
//
//	Keeps track of the game version
//
 
// ------------------------------
// MAP EDITOR (Release and Debug) BUILD VERSION
// ------------------------------
#ifdef JA2EDITOR
	
	#ifdef JA2UB
		CHAR16 zProductLabel[64]		= { L"Unfinished Business - Map Editor v1.13" };
	#else
		CHAR16 zProductLabel[64]		= { L"Map Editor v1.13" };
	#endif

// ------------------------------
// DEBUG BUILD VERSIONS
// ------------------------------
#elif defined JA2BETAVERSION

	//DEBUG BUILD VERSION	
	#ifdef JA2UB
		CHAR16 zProductLabel[64]		= { L"Debug: Unfinished Business - v1.13" };
	#elif defined (JA113DEMO)
		CHAR16 zProductLabel[64]		= { L"Debug: JA2 Demo - v1.13" };
	#else
		CHAR16 zProductLabel[64]		= { L"Debug: v1.13" };
	#endif

#elif defined CRIPPLED_VERSION

	//RELEASE BUILD VERSION s
	CHAR16 zProductLabel[64]		= { L"Beta v. 0.98" };

// ------------------------------
// RELEASE BUILD VERSIONS
// ------------------------------
#else

	//RELEASE BUILD VERSION	
	#ifdef JA2UB
		CHAR16		zProductLabel[64]		= { L"Unfinished Business - v1.13" };
	#elif defined (JA113DEMO)
		CHAR16		zProductLabel[64]		= { L"JA2 Demo - v1.13" };
	#else
		CHAR16		zProductLabel[64]		= { L"v1.13" };
	#endif

#endif

CHAR8		czVersionString[16]	= { "@Version@" };
CHAR16		zBuildInformation[256]	= { L"@Build@" };

// SAVE_GAME_VERSION is defined in header, change it there
