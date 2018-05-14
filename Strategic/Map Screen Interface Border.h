#ifndef __MAP_INTERFACE_BORDER_H
#define __MAP_INTERFACE_BORDER_H

#include "Types.h"

//#define	 MAP_BORDER_START_X	261
//#define	 MAP_BORDER_START_Y	0

// scroll directions
enum{
	ZOOM_MAP_SCROLL_UP =0,
	ZOOM_MAP_SCROLL_DWN,
	ZOOM_MAP_SCROLL_RIGHT,
	ZOOM_MAP_SCROLL_LEFT,
};

enum{
	EAST_DIR =	0,
	WEST_DIR,	
	NORTH_DIR, 
	SOUTH_DIR, 
};
enum{
	MAP_BORDER_TOWN_BTN =0,
	MAP_BORDER_MINE_BTN,
	MAP_BORDER_TEAMS_BTN,
	MAP_BORDER_AIRSPACE_BTN,
	MAP_BORDER_ITEM_BTN,
	MAP_BORDER_MILITIA_BTN,
	MAP_BORDER_DISEASE_BTN,		// Flugente: toggle disease views
	MAP_BORDER_WEATHER_BTN,		// Flugente: toggle weather display
	MAP_BORDER_INTEL_BTN,		// Flugente: intel display
	NUM_MAP_BORDER_BTNS,		// end enumeration marker
};

// HEADROCK HAM 4: Turned these into EXTERNS to allow dynamic modification
//#define MAP_LEVEL_MARKER_X			(MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X) / 2 + 114))	//MAP_BORDER_X + MAP_BORDER_X_OFFSET + 384		//(SCREEN_WIDTH - 75)			//565
extern UINT16 MAP_LEVEL_MARKER_X;
extern UINT16 MAP_LEVEL_MARKER_Y;
extern UINT16 MAP_LEVEL_MARKER_DELTA;
extern UINT16 MAP_LEVEL_MARKER_WIDTH;

extern BOOLEAN fShowTownFlag;
extern BOOLEAN fShowMineFlag;
extern BOOLEAN fShowTeamFlag;
extern BOOLEAN fShowMilitia;
extern BOOLEAN fShowItemsFlag;

// different types of map displays that colour sectors
enum
{
	MAP_DISPLAY_NORMAL,
	MAP_DISPLAY_AIRSPACE,
	MAP_DISPLAY_AIRSPACE_COLOURED_SAMS,
	MAP_DISPLAY_DISEASE,
	MAP_DISPLAY_WEATHER,
	MAP_DISPLAY_INTEL,
};

extern UINT8 gusMapDisplayColourMode;
//extern BOOLEAN fShowVehicleFlag;

// scroll animation
extern INT32 giScrollButtonState;

BOOLEAN LoadMapBorderGraphics( void );
void DeleteMapBorderGraphics( void );
void RenderMapBorder( void );

void ToggleShowTownsMode( void );
void ToggleShowMinesMode( void );
void ToggleShowMilitiaMode( void );
void ToggleShowTeamsMode( void );
void ToggleAirspaceMode( void );
void ToggleItemsFilter( void );

// Flugente: more filters
void ToggleDiseaseFilter( );
void ToggleWeatherFilter( );
void ToggleIntelFilter();

void TurnOnShowTeamsMode( void );
void TurnOnAirSpaceMode( void );
void TurnOnItemFilterMode( void );

// Flugente: more modes
void TurnOnDiseaseFilterMode();
void TurnOnWeatherFilterMode( );
void TurnOnIntelFilterMode();

// create/destroy buttons for map border region
void DeleteMapBorderButtons( void );
BOOLEAN CreateButtonsForMapBorder( void );

// render the pop up for eta	in path plotting in map screen
void RenderMapBorderEtaPopUp( void );
BOOLEAN DoesPlayerHaveAnyMilitia( void );

// create mouse regions for level markers
void CreateMouseRegionsForLevelMarkers( void );
void DeleteMouseRegionsForLevelMarkers( void );

void InitMapScreenFlags( void );

void MapBorderButtonOff( UINT8 ubBorderButtonIndex );
void MapBorderButtonOn( UINT8 ubBorderButtonIndex );

// HEADROCK HAM 4: Initialize coordinates for Map Border buttons
void InitMapBorderButtonCoordinates();

#endif