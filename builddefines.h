#ifndef _BUILDDEFINES_H
#define _BUILDDEFINES_H

#include "Language Defines.h"

// Beta version
//#define	JA2BETAVERSION


// Release version of the editor!
// Normal test version
//#define JA2TESTVERSION


// Remove comment to build the editor!
//#define JA2EDITOR


// Remove comment to build the editor!
#ifdef _DEBUG
	#ifndef JA2TESTVERSION
		#define JA2TESTVERSION
	#endif
#endif

// Remove comment to build the editor!

// Do combinations
#ifdef JA2TESTVERSION
	#define JA2BETAVERSION
	#define JA2EDITOR
#endif



#ifdef JA2BETAVERSION
	#ifndef JA2EDITOR
		#define SGP_DEBUG
		#define	FORCE_ASSERTS_ON
		#define SGP_VIDEO_DEBUGGING
	#endif
#endif

//#define CRIPPLED_VERSION

// Huge speed and memory hog, but thorough -- will work with release builds.
// #define EXTREME_MEMORY_DEBUGGING 

#endif