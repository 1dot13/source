#ifndef _MAP_SCREEN_INTERFACE_MAP_H
#define _MAP_SCREEN_INTERFACE_MAP_H


#include "Vehicles.h"
#include "mapscreen.h"

// functions
void DrawMapIndexBigMap( BOOLEAN fSelectedCursorIsYellow );
//void DrawMapIndexSmallMap( BOOLEAN fSelectedCursorIsYellow );

UINT32 DrawMap( void );

#define MAX_ICON_CHARS 500

typedef struct
{
	CHAR8	IconSti[MAX_ICON_CHARS];
	INT32 IconX;
	INT32 IconY;
}	ICON_FILE;

typedef struct
{
	UINT32 uiIndex;
	CHAR16 sType[50];
	UINT16 Type;
	CHAR16 sMinerals[50];
	
} MINERALS_VALUES;

// map shading colors
enum{
	MAP_SHADE_BLACK =0,

	MAP_SHADE_LT_GREEN,
	MAP_SHADE_MD_GREEN,
	MAP_SHADE_DK_GREEN,

	MAP_SHADE_LT_RED,
	MAP_SHADE_MD_RED,
	MAP_SHADE_DK_RED,

	MAP_SHADE_LT_YELLOW,
	MAP_SHADE_MD_YELLOW,
	MAP_SHADE_DK_YELLOW,

	MAP_SHADE_LT_CYAN,
	MAP_SHADE_MD_CYAN,
	MAP_SHADE_DK_CYAN,

	MAP_SHADE_LT_GREY,
	MAP_SHADE_MD_GREY,
	MAP_SHADE_DK_GREY,

	MAP_SHADE_LT_BLUE,
	MAP_SHADE_MD_BLUE,
	MAP_SHADE_DK_BLUE,

	MAP_SHADE_LT_PINK,
	
	MAP_SHADE_ORANGE,

	// Flugente: more colours
	MAP_SHADE_MIX_RED,
	MAP_SHADE_MIX_GREEN,
	MAP_SHADE_MIX_BLUE,
	MAP_SHADE_MIX_YELLOW,

	MAP_SHADE_MIX_RED_GREEN,
	MAP_SHADE_MIX_RED_BLUE,
	MAP_SHADE_MIX_RED_YELLOW,

	MAP_SHADE_MIX_GREEN_BLUE,
	MAP_SHADE_MIX_GREEN_YELLOW,

	MAP_SHADE_MIX_BLUE_YELLOW,

	MAP_SHADE_MIX_RED_GREEN_BLUE,
	MAP_SHADE_MIX_RED_GREEN_YELLOW,
	MAP_SHADE_MIX_RED_BLUE_YELLOW,

	MAP_SHADE_MIX_GREEN_BLUE_YELLOW,

	MAP_SHADE_MIX_RED_GREEN_BLUE_YELLOW,

	MAP_SHADE_MAX,
};

extern MINERALS_VALUES MineralsName[255];

extern ICON_FILE gHiddenIcon[ 256 ];

extern UINT32	guiIcon2[256];

typedef struct InqMapInfo
{
	UINT8 mapcolour;		// map sector will get this colour

	CHAR16 text[1024];		// texts to appear in the infobox, \n start a new line
	CHAR16 shorttext[128];	// this text will appear on the map itself

	std::vector<int> symbols;	// all the symbols we have to display
} InqMapInfo;

extern InqMapInfo gMapIntelData[256];

extern BOOLEAN LoadHiddenTownFromLoadGameFile( HWFILE hFile );
extern BOOLEAN SaveHiddenTownToSaveGameFile( HWFILE hFile );

void GetScreenXYFromMapXY( INT16 sMapX, INT16 sMapY, INT16 *psX, INT16 *psY );
void GetScreenXYFromMapXYStationary( INT16 sMapX, INT16 sMapY, INT16 *psX, INT16 *psY );

BOOLEAN InitializePalettesForMap( void );
void InitializeMilitiaPopup(void);

void ShutDownPalettesForMap( void );

// plot path for helicopter
void PlotPathForHelicopter( INT16 sX, INT16 sY );

// the temp path, where the helicopter could go
void PlotATemporaryPathForHelicopter( INT16 sX, INT16 sY );

// plot path for helicopter
void PlotPathForMilitia( INT16 sX, INT16 sY );

// the temp path, where the helicopter could go
void PlotATemporaryPathForMilitia( INT16 sX, INT16 sY );

// trace a route for a passed path...doesn't require dest char - most more general
BOOLEAN TracePathRoute( BOOLEAN fCheckFlag, BOOLEAN fForceUpDate, PathStPtr pPath );

// show arrows for this char
void DisplayPathArrows(UINT16 usCharNum, HVOBJECT hMapHandle);

// restore backgrounds for arrows
void RestoreArrowBackgroundsForTrace(INT32 iArrow, INT32 iArrowX, INT32 iArrowY, BOOLEAN fZoom);

// build path for character
void PlotPathForCharacter( SOLDIERTYPE *pCharacter, INT16 sX, INT16 sY, BOOLEAN fTacticalTraversal );

// build temp path for character
void PlotATemporaryPathForCharacter( SOLDIERTYPE *pCharacter, INT16 sX, INT16 sY );


// display current/temp paths
void DisplaySoldierPath( SOLDIERTYPE *pCharacter );
void DisplaySoldierTempPath( SOLDIERTYPE *pCharacter );
void DisplayHelicopterPath( void );
void DisplayHelicopterTempPath( void );
void DisplayMilitiaPath( void );
void DisplayMilitiaTempPath( void );


// clear path after this sector
UINT32 ClearPathAfterThisSectorForCharacter( SOLDIERTYPE *pCharacter, INT16 sX, INT16 sY );

// cancel path : clear the path completely and gives player feedback message that the route was canceled
void CancelPathForCharacter( SOLDIERTYPE *pCharacter );
void CancelPathForVehicle( VEHICLETYPE *pVehicle, BOOLEAN fAlreadyReversed );
void CancelPathForGroup( GROUP *pGroup );

void CancelPathForMilitiaGroup( UINT8 uGroupId );

// check if we have waited long enought object update temp path
void DisplayThePotentialPathForHelicopter(INT16 sMapX, INT16 sMapY );

void DisplayThePotentialPathForMilitia( INT16 sMapX, INT16 sMapY );

// clear out helicopter list after this sector
UINT32 ClearPathAfterThisSectorForHelicopter( INT16 sX, INT16 sY );

UINT32 ClearPathAfterThisSectorForMilitia( INT16 sX, INT16 sY );


// check to see if sector is highlightable
BOOLEAN IsTheCursorAllowedToHighLightThisSector( INT16 sSectorX, INT16 sSectorY );

// set bad sectors list
void SetUpBadSectorsList( void );

// restore background for map grids
void RestoreBackgroundForMapGrid( INT16 sMapX, INT16 sMapY );

// clip blits to map view region
void ClipBlitsToMapViewRegion( void );
void ClipBlitsToMapViewRegionForRectangleAndABit( UINT32 uiDestPitchBYTES );

// clip blits to full screen....restore after use of ClipBlitsToMapViewRegion( )
void RestoreClipRegionToFullScreen( void );
void RestoreClipRegionToFullScreenForRectangle( UINT32 uiDestPitchBYTES );

// show the icons for people in motion
void ShowPeopleInMotion( INT16 sX, INT16 sY );
void ShowNonPlayerGroupsInMotion( INT16 sX, INT16 sY, UINT8 usTeam );
void ShowMilitiaInMotion( INT16 sX, INT16 sY);	// added by Flugente

// last sector in helicopter's path
INT16 GetLastSectorOfHelicoptersPath( void );

INT16 GetLastSectorOfMilitiaPath( void );

// display info about helicopter path
void DisplayDistancesForHelicopter( void );

// display where heli is
void DisplayPositionOfHelicopter( void );
void DisplayPositionOfEnemyHelicopter();

// check for click
BOOLEAN CheckForClickOverHelicopterIcon( INT16 sX, INT16 sY );

// show town markers
void BlitTownGridMarkers( void );

// load and delete the graphics for the map screen map militia 
BOOLEAN LoadMilitiaPopUpBox( void );
void RemoveMilitiaPopUpBox( void );

// check if anyone left behind, if not, move selected cursor along with movement group
//void CheckIfAnyoneLeftInSector( INT16 sX, INT16 sY, INT16 sNewX, INT16 sNewY, INT8 bZ );

// create destroy
void CreateDestroyMilitiaPopUPRegions( void );

// draw the militia box
BOOLEAN DrawMilitiaPopUpBox( void );


//Returns true if the player knows how many enemies are in the sector if that number is greater than 0.	
//Returns false for all other cases.
UINT32 WhatPlayerKnowsAboutEnemiesInSector( INT16 sSectorX, INT16 sSectorY );

//There is a special case flag used when players encounter enemies in a sector, then retreat.	The number of enemies
//will display on mapscreen until time is compressed.	When time is compressed, the flag is cleared, and
//a question mark is displayed to reflect that the player no longer knows.	This is the function that clears that
//flag.
void ClearAnySectorsFlashingNumberOfEnemies();

void CopyPathToCharactersSquadIfInOne( SOLDIERTYPE *pCharacter );

void InitMapSecrets( void );


enum {
	ABORT_PLOTTING = 0,
	PATH_CLEARED,
	PATH_SHORTENED,
};

// what the player knows about the enemies in a given sector
enum {
	KNOWS_NOTHING = 0,
	KNOWS_THEYRE_THERE,
	KNOWS_THEYRE_THERE_AND_WHERE_GOING,
	KNOWS_THEYRE_THERE_AND_HOW_MANY,
	KNOWS_THEYRE_THERE_AND_WHERE_GOING_AND_HOW_MANY,
};



// scroll bounds
#define EAST_ZOOM_BOUND 378
#define WEST_ZOOM_BOUND 42
#define SOUTH_ZOOM_BOUND 324
#define NORTH_ZOOM_BOUND 36

// WANNE: The starting top/left position of the background image
#define MAP_BORDER_X						261
#define MAP_BORDER_Y						0

// size of squares on the map
extern UINT16	MAP_GRID_X;
extern UINT16	MAP_GRID_Y;

extern UINT16	MAP_BORDER_X_OFFSET;
extern UINT16	MAP_BORDER_Y_OFFSET;

extern UINT16	MAP_VIEW_START_X;
extern UINT16	MAP_VIEW_START_Y;
extern UINT16	MAP_VIEW_WIDTH;
extern UINT16	MAP_VIEW_HEIGHT;

extern INT32	MAP_FONT;
extern INT32	ETA_FONT;

extern UINT16	MAP_HORT_INDEX_X;
extern UINT16	MAP_HORT_INDEX_Y;
extern UINT16	MAP_HORT_HEIGHT;
extern UINT16	MAP_VERT_INDEX_X;
extern UINT16	MAP_VERT_INDEX_Y;
extern UINT16	MAP_VERT_WIDTH;

// Helicopter eta popup position
extern UINT16 MAP_HELICOPTER_ETA_POPUP_X;
extern UINT16 MAP_HELICOPTER_ETA_POPUP_Y;
extern UINT16 MAP_HELICOPTER_UPPER_ETA_POPUP_Y;
extern UINT16 MAP_HELICOPTER_ETA_POPUP_WIDTH;
extern UINT16 MAP_HELICOPTER_ETA_POPUP_HEIGHT;
extern UINT16 MAP_HELICOPTER_ETA_POPUP_ALTERNATE_HEIGHT;

// sublevel text string position
extern UINT16 MAP_LEVEL_STRING_X;
extern UINT16 MAP_LEVEL_STRING_Y;


// number of units wide
#define	 WORLD_MAP_X			18	// WANNE: 18 darf nicht ge�ndert werden!

// Orta position on the map
#define ORTA_SECTOR_X 4
#define ORTA_SECTOR_Y 11

#define TIXA_SECTOR_X		gModSettings.ubTixaPrisonSectorX //9
#define TIXA_SECTOR_Y		gModSettings.ubTixaPrisonSectorY //10

// what are we showing?..teams/vehicles
// Show values
#define SHOW_TEAMMATES 1
#define SHOW_VEHICLES	2


// wait time until temp path is drawn, from placing cursor on a map grid
#define MIN_WAIT_TIME_FOR_TEMP_PATH 200

//extern INT32 MAP_VIEW_START_X;
//extern INT32 MAP_VIEW_START_Y;


// zoom UL coords
extern INT32 iZoomX;
extern INT32 iZoomY;

// the number of militia on the cursor
extern INT16 sGreensOnCursor;
extern INT16 sRegularsOnCursor;
extern INT16 sElitesOnCursor;

// highlighted sectors
extern INT16 gsHighlightSectorX;
extern INT16 gsHighlightSectorY;

// the big map
extern UINT32 guiBIGMAP;

// the orta icon
extern UINT32 guiORTAICON;

extern UINT32 guiTIXAICON;

// the character icons
extern UINT32 guiCHARICONS;

// the merc arrival sector landing zone icon
extern UINT32 guiBULLSEYE;

// Flugente: symbols indicating various things on the intel map
extern UINT32 guiINTEL;

// character between sector icons
extern UINT32 guiCHARBETWEENSECTORICONS;
// HEADROCK HAM 5: Enemies Between Sectors icons
extern UINT32 guiENEMYBETWEENSECTORICONS;

// the viewable map bound region
extern SGPRect MapScreenRect;

// draw temp path
extern BOOLEAN fDrawTempHeliPath;

extern BOOLEAN fDrawTempMilitiaPath;

// selected destination char
INT8 GetSelectedDestChar();
void SetSelectedDestChar( INT8 aVal );

extern bool gSquadEncumbranceCheckNecessary;

// current assignment character
extern INT8 bSelectedAssignChar;

// the info character
extern INT8 bSelectedInfoChar;

// the contract char 
extern INT8 bSelectedContractChar;

// map arrows graphical index value
extern UINT32 guiMAPCURSORS;

// HEADROCK HAM 5: New pathing arrows may replace the above eventually, but for now a separate variable will do.
extern UINT32 guiMapPathingArrows;

// has temp path for character path or helicopter been already drawn
extern BOOLEAN	fTempPathAlreadyDrawn;

// map view region clipping rect
extern SGPRect MapScreenRect;

// the map border eta pop up
extern UINT32 guiMapBorderEtaPopUp;

// heli pop up 
extern UINT32 guiMapBorderHeliSectors;

// anv: alternate heli pop up for alternative fuel system
extern UINT32 guiMapBorderHeliSectorsAlternate;

// the currently selected town militia
extern INT16 sSelectedMilitiaTown;

// the selected sectors
extern UINT16	sSelMapX;
extern UINT16	sSelMapY;


extern BOOLEAN fFoundTixa;

extern UINT32 guiSubLevel1, guiSubLevel2, guiSubLevel3;

extern INT16 sBaseSectorList[ MAX_TOWNS - 1 ];
extern POINT pTownPoints[ MAX_TOWNS ];

#ifdef JA2UB
extern void SetUpValidCampaignSectors( void );
#endif

void CreateMilitiaGroupBox( );
void DestroyMilitiaGroupBox( );
void DisplayMilitiaGroupBox( );

#endif
