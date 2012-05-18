#ifndef _BUILDDEFINES_H
#define _BUILDDEFINES_H

#include "Language Defines.h"

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
// WANNE: With these defines we can build JA2 DEMO version that runs with 1.13
// The 2 UB defines (JA2UB  and JA2UBMAPS) have to be disabled in order to build a demo executable

// There are different combinations of the 2 defines

// 1.) Both defines are enabled: Build demo v1.13, which uses the slf files from orginal JA2 demo
// #define JA113DEMO
// #define JA113NODEMO

// 2.) !!!! Only JA113DEMO define enabled: Build demo v1.13, which uses the ja2 original files. This is the prefered way of building the 1.13 demo !!!!
// #define JA113DEMO
// // #define JA113NODEMO

// 3.) Only JA113NODEMO define enabled: This build a "normal" 1.13 version, with a little influence of a UB version (disable all meanwhiles, emails, no quest with queen, alien queen)
//     This version can be used for modders to have a "blank" 1.13 version with no special quests.
// // #define JA113DEMO
// #define JA113NODEMO


// WANNE: If both defines are disabled, we just build "normal" 1.13 or "unfinished business" 1.13 (when JA2UB and JA2UBMAPS enabled) version!
//#define JA113DEMO
//#define JA113NODEMO 

// *****************************
// -----------------------------

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
