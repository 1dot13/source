	#include "Types.h"
	#include "GameVersion.h"

//
//	Keeps track of the game version
//

#ifdef JA2EDITOR // map editor
	
	#ifdef JA2UB
		CHAR16 zProductLabel[64]		= { L"JA2 1.13 Unfinished Business - Map Editor" };
	#else
		CHAR16 zProductLabel[64]		= { L"JA2 1.13 - Map Editor" };
	#endif

#elif defined JA2BETAVERSION // debug

	#ifdef JA2UB
		CHAR16 zProductLabel[64]		= { L"Debug: JA2 1.13 Unfinished Business" };
	#elif defined (JA113DEMO)
		CHAR16 zProductLabel[64]		= { L"Debug: JA2 1.13 Demo" };
	#else
		CHAR16 zProductLabel[64]		= { L"Debug: JA2 1.13" };
	#endif

#elif defined CRIPPLED_VERSION

	CHAR16 zProductLabel[64]		= { L"JA2 113 Beta-0.98" };

#else // release

	#ifdef JA2UB
		CHAR16		zProductLabel[64]		= { L"JA2 1.13 Unfinished Business" };
	#elif defined (JA113DEMO)
		CHAR16		zProductLabel[64]		= { L"JA2 1.13 Demo" };
	#else
		CHAR16		zProductLabel[64]		= { L"JA2 1.13" };
	#endif

#endif

CHAR8		czVersionString[16]	= { "@Version@" };
CHAR16		zBuildInformation[256]	= { L"@Build@" };

// SAVE_GAME_VERSION is defined in header, change it there
