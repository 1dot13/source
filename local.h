// WANNE 2 <changed some lines>
#ifndef __LOCAL_DEFINES_
#define __LOCAL_DEFINES_

#include "Screens.h"





//
// Make sure the application has a name
//

#define APPLICATION_NAME      "Jagged Alliance 2"

//
// Basic defines for the video manager. These represent the starting values
//

#ifdef BOUNDS_CHECKER
	#define		WINDOWED_MODE
#endif

#ifdef _DEBUG
 #define WINDOWED_MODE
#endif

extern int  SCREEN_WIDTH;
extern int  SCREEN_HEIGHT;

extern int iResolution;			// Resolution id from the ini file
extern int iScreenWidthOffset;
extern int iScreenHeightOffset;

// WANNE 2
extern BOOLEAN fDisplayOverheadMap;

#define PIXEL_DEPTH           16

//
// These defines are used as MUTEX handles.
//

#define MAX_MUTEX_HANDLES     32

#define REFRESH_THREAD_MUTEX  0
#define FRAME_BUFFER_MUTEX    1
#define MOUSE_BUFFER_MUTEX    2
#define DIRTY_BUFFER_MUTEX    3
#define SCROLL_MESSAGE_MUTEX  4


#endif