#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include "builddefines.h"
	#include <stdarg.h>
	#include <stdio.h>
	#include <wchar.h>
	#include "types.h"
	#include "screenids.h"
	#include "Sys Globals.h"
	#include "gameloop.h"
#endif


// External globals
extern CHAR8	gubFilename[ 200 ]; // symbol already declared globally in editscreen.cpp (jonathanl)
BOOLEAN			gfEditMode = FALSE;
extern CHAR8		gDebugStr[128]; // symbol already defined in overhead.cpp (jonathanl)
CHAR8			gSystemDebugStr[128];
INT8			gbFPSDisplay = SHOW_MIN_FPS;
BOOLEAN			gfResetInputCheck = FALSE;
BOOLEAN			gfGlobalError			= FALSE;

UINT32	guiGameCycleCounter = 0;
CHAR8	gubErrorText[ 512 ];//for long filenames

BOOLEAN	SET_ERROR( const STR8 String, ...)
{
	va_list	ArgPtr;

	va_start(ArgPtr, String);
	vsprintf( gubErrorText, String, ArgPtr);
	va_end(ArgPtr);

	SetPendingNewScreen( ERROR_SCREEN );

	gfGlobalError = TRUE;

	return( FALSE );
}

//create a varible that can be tested outside the pre-compiler convention. 
// also see its "Sys Globals.h" declaration
#ifdef JA2BETAVERSION 
BOOLEAN JA2BETAVERSION_FLAG = 1;
#else
BOOLEAN JA2BETAVERSION_FLAG = 0;

#endif

