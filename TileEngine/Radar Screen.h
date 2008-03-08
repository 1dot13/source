#ifndef __RADAR_SCREEN_H
#define __RADAR_SCREEN_H

#include "interface.h"

// RADAR MOUSE REGION STUFF
void RadarRegionMoveCallback( MOUSE_REGION * pRegion, INT32 iReason );
void RadarRegionButtonCallback( MOUSE_REGION * pRegion, INT32 iReason );

BOOLEAN LoadRadarScreenBitmap( CHAR8 * aFilename );

// CHRISL: Changed radar coord defines to standard variables and moved definition to radar screen.cpp
// RADAR WINDOW DEFINES
extern INT16	RADAR_WINDOW_TM_X;
extern INT16	RADAR_WINDOW_SM_X;
extern INT16	RADAR_WINDOW_TM_Y;
extern INT16	RADAR_WINDOW_SM_Y;
extern INT16	RADAR_WINDOW_WIDTH;
extern INT16	RADAR_WINDOW_HEIGHT;
extern INT16	RADAR_WINDOW_STRAT_X;
extern INT16	RADAR_WINDOW_STRAT_Y;

BOOLEAN InitRadarScreen( );
void RenderRadarScreen( );
void MoveRadarScreen( );

// disable rendering of radar screen
void DisableRadarScreenRender( void );

// enable rendering of radar screen
void EnableRadarScreenRender( void );

// toggle rendering flag of radar screen
void ToggleRadarScreenRender( void );

// create destroy squad list regions as needed
BOOLEAN CreateDestroyMouseRegionsForSquadList( void );

// clear out the video object for the radar map
void ClearOutRadarMapImage( void );

// do we render the radar screen?..or the squad list?
extern BOOLEAN	fRenderRadarScreen;

#endif