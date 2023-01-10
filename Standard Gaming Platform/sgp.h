#ifndef __SGP_
#define __SGP_

#include "local.h"
#include "types.h"
#include "timer.h"
#include "debug.h"

#include "video.h"


#ifdef __cplusplus
extern "C" {
#endif

extern BOOLEAN		gfProgramIsRunning; // Turn this to FALSE to exit program
extern CHAR8			gzCommandLine[100];	// Command line given
extern UINT8			gbPixelDepth;				// GLOBAL RUN-TIME SETTINGS
extern BOOLEAN		gfDontUseDDBlits;		// GLOBAL FOR USE OF DD BLITTING


// function prototypes
void SGPExit(void);
void ShutdownWithErrorBox(CHAR8 *pcMessage);

#ifdef __cplusplus
}
#endif

#endif
