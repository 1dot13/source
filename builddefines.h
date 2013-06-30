#ifndef _BUILDDEFINES_H
#define _BUILDDEFINES_H

#include "Language Defines.h"

//----- Encyclopedia and Briefing room - by Jazz -----
//#define ENABLE_ENCYCLOPEDIA		// WANNE: Be aware, encyclopedia uses a hugh amount of memory and the exe takes forever to start. There seems to be some memory leaks
#define ENABLE_BRIEFINGROOM
//----- End encyclopedia and Briefing room - by Jazz -----

// -----------------------------
// *****************************
// WANNE: Enable those 2 defines, if you want to build UB-Version!!

//JA2/Unfinished Business
//#define JA2UB

//Unfinished Business - Titles (ja2set.dat)
//#define JA2UBMAPS
// *****************************
// -----------------------------

// -----------------------------
// *****************************
// WANNE: With these define we can build JA2 DEMO version that runs with 1.13
// The 2 UB defines (JA2UB  and JA2UBMAPS) should be disabled, because we only want to build the demo version from the JA2 code!

//#define JA113DEMO
// *****************************
// -----------------------------


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// -----------------------------
// WANNE: Enable this define, to build an executable where you can enable/disable zombies in tactical (ingame option screen).
#define ENABLE_ZOMBIES
// -----------------------------
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Map Editor version - you should use the MapEditor configuration instead of messing with these defines (ChrisL)
//#define JA2BETAVERSION
//#define JA2EDITOR
// Normal test version
//#define JA2TESTVERSION

//#define SGP_DEBUG
//#define DEBUG_ATTACKBUSY
//#define DEBUGDECISIONS
#define ROBOT_ALWAYS_READY
#define	FORCE_ASSERTS_ON

//#ifdef _DEBUG
//	#ifndef JA2TESTVERSION
//		#define JA2TESTVERSION
//	#endif
//#endif

// Do combinations
//#ifdef JA2TESTVERSION
//	#define JA2BETAVERSION
//	#define JA2EDITOR
//#endif



#ifdef JA2BETAVERSION
	#ifndef JA2EDITOR
//		#define SGP_DEBUG
//		#define	FORCE_ASSERTS_ON
//		#define SGP_VIDEO_DEBUGGING
	#endif
#endif

//#define CRIPPLED_VERSION

// Huge speed and memory hog, but thorough -- will work with release builds.
// #define EXTREME_MEMORY_DEBUGGING 

// 0verhaul
// These squelch warnings in VC2K5 about printf's and string operations being unsafe.  Maybe we should work on building a better
// system that gives buffer sizes for copies (which would prevent future problems like a stack corruption issue I fixed a while back),
// but for now, just tell the compiler to shut up so I can see the real errors.
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#define _SCL_SECURE_NO_WARNINGS

#include "Profiler.h"

#endif
