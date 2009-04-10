#ifndef _BUILDDEFINES_H
#define _BUILDDEFINES_H

#include "Language Defines.h"


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

//#define BLOOD_N_GORE_ENABLED // arynn : del_me : defunct, german build's differed with blood and gore option.. it got universalized

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

#include "Profiler.h"

#endif
