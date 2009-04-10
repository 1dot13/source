#ifndef __SYS_GLOBALS_H
#define __SYS_GLOBALS_H

#define			SHOW_MIN_FPS			0
#define			SHOW_FULL_FPS			1

extern CHAR8	gubErrorText[ 512 ];//for long filenames
extern BOOLEAN	gfAniEditMode;
extern BOOLEAN	gfEditMode;
extern BOOLEAN	fFirstTimeInGameScreen;
extern BOOLEAN	fDirtyRectangleMode;
extern CHAR8	gDebugStr[128];
extern CHAR8	gSystemDebugStr[128];

extern BOOLEAN	gfMode;
extern INT16	gsCurrentActionPoints;
extern INT8		gbFPSDisplay;
extern BOOLEAN	gfResetInputCheck;
extern BOOLEAN	gfGlobalError;

extern UINT32	guiGameCycleCounter;


// VIDEO OVERLAYS 
extern INT32	giFPSOverlay;
extern INT32	giCounterPeriodOverlay;

extern	BOOLEAN	SET_ERROR( const STR8 String, ...);

// arynn : note	: a varible version of #define JA2BETAVERSION, used for control flow when not debugging
//		 : note	:	i wasnt sure about any existing. i'd imagine there is one. if so, it will get fixed later.
extern BOOLEAN JA2BETAVERSION_FLAG; 

#endif
