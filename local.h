#ifndef __LOCAL_DEFINES_
#define __LOCAL_DEFINES_

#include "Screens.h"





//
// Make sure the application has a name
//

#define APPLICATION_NAME		"Jagged Alliance 2 v1.13"
#define APPLICATION_NAMEW		L"Jagged Alliance 2 v1.13"

//
// Basic defines for the video manager. These represent the starting values
//

#ifdef BOUNDS_CHECKER
	#define		WINDOWED_MODE
#endif

#ifdef _DEBUG
 #define WINDOWED_MODE
#endif

extern UINT16	SCREEN_WIDTH;
extern UINT16	SCREEN_HEIGHT;

extern int iResolution;			// Resolution id from the ini file
extern int iPlayIntro;
extern int iUseWinFonts;
extern float fTooltipScaleFactor;
extern int iDisableMouseScrolling;
extern INT16 iScreenWidthOffset;
extern INT16 iScreenHeightOffset;

extern UINT16 xResOffset;			
extern UINT16 yResOffset;

extern UINT16 xResSize;		// 640 / 800 / 1024
extern UINT16 yResSize;		// 480 / 600 / 768

typedef enum
{
	// The 640x480 resolution should always be the FIRST entry in this enum
	_640x480	= 0,

	_960x540	= 1, 

	// Place all resolutions that have both (a lower height AND a lower width) BEFORE the 800x600 resolution
	_800x600	= 2,
	_1024x600	= 3,
	_1280x720	= 4,
	
	// Place all resolutions that have both (a lower height AND a lower width) BEFORE the 1024x768 resolution
	_1024x768	= 5,
	_1280x768	= 6,
	_1360x768	= 7,
	_1366x768	= 8,
	_1280x800	= 9,
	_1440x900	= 10,
	_1600x900	= 11,
	_1280x960	= 12,
	_1440x960	= 13,
	_1770x1000	= 14,
	_1280x1024	= 15,
	_1360x1024	= 16,
	_1600x1024	= 17,
	_1440x1050	= 18,
	_1680x1050	= 19,
	_1920x1080	= 20,
	_1600x1200	= 21,
	_1920x1200	= 22,
	_2560x1440	= 23,
	_2560x1600	= 24,
	_CustomRes	= 25,
} SCREEN_RESOLUTION;

extern BOOLEAN fDisplayOverheadMap;

#define PIXEL_DEPTH			16

#endif
