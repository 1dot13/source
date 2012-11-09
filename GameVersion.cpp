#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include "Types.h"
	#include "GameVersion.h"
#endif
 
//
//	Keeps track of the game version
//
 
// ------------------------------
// MAP EDITOR (Release and Debug) BUILD VERSION
// ------------------------------
#ifdef JA2EDITOR
	
	#ifdef JA2UB
		CHAR16 zVersionLabel[256]		= { L"Unfinished Business - Map Editor v1.13.5520 (Development Build)" }; 
	#else
		CHAR16 zVersionLabel[256]		= { L"Map Editor v1.13.5520 (Development Build)" }; 
	#endif

// ------------------------------
// DEBUG BUILD VERSIONS
// ------------------------------
#elif defined JA2BETAVERSION

	//DEBUG BUILD VERSION	
	#ifdef JA2UB
		CHAR16 zVersionLabel[256]		= { L"Debug: Unfinished Business - v1.13.5520 (Development Build)" }; 
	#elif defined (JA113DEMO)
		CHAR16 zVersionLabel[256]		= { L"Debug: JA2 Demo - v1.13.5520 (Development Build)" }; 
	#else
		CHAR16 zVersionLabel[256]		= { L"Debug: v1.13.5520 (Development Build)" }; 
	#endif

#elif defined CRIPPLED_VERSION

	//RELEASE BUILD VERSION s
	CHAR16 zVersionLabel[256]		= { L"Beta v. 0.98" }; 

// ------------------------------
// RELEASE BUILD VERSIONS
// ------------------------------
#else

	//RELEASE BUILD VERSION	
	#ifdef JA2UB
		CHAR16		zVersionLabel[256]		= { L"Release Unfinished Business - v1.13.5520 (Development Build)" };
	#elif defined (JA113DEMO)
		CHAR16		zVersionLabel[256]		= { L"Release JA2 Demo - v1.13.5520 (Development Build)" };
	#else
		CHAR16		zVersionLabel[256]		= { L"Release v1.13.5521 (Development Build)" };
	#endif

#endif
										
CHAR8		czVersionNumber[16]	= { "Build 12.08.24" };		//YY.MM.DD
CHAR16		zTrackingNumber[16]	= { L"Z" };
	
// SAVE_GAME_VERSION is defined in header, change it there
