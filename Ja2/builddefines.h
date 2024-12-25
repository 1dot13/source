#ifndef _BUILDDEFINES_H
#define _BUILDDEFINES_H

//----- Briefing Room (Mission based JA2 like in JA/DG) - by Jazz -----
// Once enabled here and also enabled in the ja2_options.ini (BRIEFING_ROOM), 
// you can access the briefing room feature from the laptop
// Of course, you also need a briefing room 1.13-MOD, otherwise you have a blank briefing room.
// There are 2 examples (Briefing Room Example 1.zip, Briefing Room Example 2.zip) in the documents svn folder:
// https://ja2svn.dyndns.org/source/ja2/trunk/Documents/1.13%20Modding/Modding%20Examples
#define ENABLE_BRIEFINGROOM

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
// WANNE: With this defin we can build JA2 DEMO version that runs with 1.13
// The 2 UB defines (JA2UB  and JA2UBMAPS) should be disabled, because we only want to build the demo version from the JA2 code!

//#define JA113DEMO
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
