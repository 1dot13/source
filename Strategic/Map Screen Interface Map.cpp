#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "GameSettings.h"
#else
	#include "Font.h"
	#include "Font Control.h"
	#include "mapscreen.h"
	#include "Map Screen Interface.h"
	#include "Map Screen Interface Map.h"
	#include "Map Screen Interface Border.h"
	#include "Overhead.h"
	#include "Render Dirty.h"
	#include "sysutil.h"
	#include "strategic.h"
	#include "strategicmap.h"
	#include "Strategic Pathing.h"
	#include "Text.h"
	#include "vsurface.h"
	#include "video.h"
	#include "vobject_blitters.h"
	#include "WCheck.h"
	#include "Assignments.h"
	#include "Squads.h"
	#include "Message.h"
	#include "Soldier Profile.h"
	#include "Player Command.h"
	#include "Strategic Movement.h"
	#include "Queen Command.h"
	#include "Campaign Types.h"
	#include "Strategic Town Loyalty.h"
	#include "Strategic Mines.h"
	#include "Vehicles.h"
	#include "Map Screen Helicopter.h"
	#include "Game Clock.h"
	#include "finances.h"
	#include "line.h"
	#include "english.h"
	#include "PreBattle Interface.h"
	#include "Utilities.h"
	#include "Town Militia.h"
	#include "Militia Control.h"
	#include "GameSettings.h"
	#include "Tactical Save.h"
	#include "Map Information.h"
	#include "Air Raid.h"
	#include "Auto Resolve.h"
#endif

#include "Quests.h"
#include "connect.h"
#include "Strategic Mines.h"
// HEADROCK HAM 3.5: included for detectionlevels
#include "Facilities.h"
#include "MilitiaSquads.h"

#include "LaptopSave.h"

// added by Flugente
extern CHAR16 gzSectorNames[256][4][MAX_SECTOR_NAME_LENGTH];

extern UINT8 gMilitiaGroupId;
extern UINT8 gNewMilitiaGroupId;
extern PathStPtr gpMilitiaPreviousMercPath;

UINT16	MAP_GRID_X;
UINT16  MAP_GRID_Y;

UINT16	MAP_BORDER_X_OFFSET;
UINT16	MAP_BORDER_Y_OFFSET;

UINT16	MAP_VIEW_START_X;
UINT16	MAP_VIEW_START_Y;
UINT16	MAP_VIEW_WIDTH;
UINT16	MAP_VIEW_HEIGHT;

INT32 ETA_FONT;

INT32	MAP_FONT;
// x start of hort index
UINT16	MAP_HORT_INDEX_X;
// y position of hort index
UINT16	MAP_HORT_INDEX_Y;
// height of hort index
UINT16 MAP_HORT_HEIGHT;

// The letters on the left side of the map
// vert index start x
UINT16 MAP_VERT_INDEX_X;
// vert index start y
UINT16 MAP_VERT_INDEX_Y;
// vert width
UINT16 MAP_VERT_WIDTH;

// Helicopter eta popup position
UINT16 MAP_HELICOPTER_ETA_POPUP_X;
UINT16 MAP_HELICOPTER_ETA_POPUP_Y;
UINT16 MAP_HELICOPTER_UPPER_ETA_POPUP_Y;
UINT16 MAP_HELICOPTER_ETA_POPUP_WIDTH;
UINT16 MAP_HELICOPTER_ETA_POPUP_HEIGHT;
UINT16 MAP_HELICOPTER_ETA_POPUP_ALTERNATE_HEIGHT;

// sublevel text string position
UINT16 MAP_LEVEL_STRING_X;
UINT16 MAP_LEVEL_STRING_Y;

// font
//#define MAP_FONT BLOCKFONT2
// ----------------------

// index color
#define MAP_INDEX_COLOR 32*4-9

// max number of sectors viewable
#define MAX_VIEW_SECTORS	  16

// "Militia" Icons
#define POPUP_FRAMEBORDER			0
#define BIG_NORMAL_BUTTON			1
#define BIG_PRESSED_BUTTON			2
#define SMALL_NORMAL_BUTTON			3
#define SMALL_PRESSED_BUTTON		4
#define SMALL_GREEN_BOX				5
#define SMALL_CYAN_BOX				6
#define SMALL_BLUE_BOX				7
#define POPUP_GREEN_BOX				8
#define POPUP_CYAN_BOX				9
#define POPUP_BLUE_BOX				10
#define BIG_GREEN_BOX				11
#define BIG_CYAN_BOX				12
#define BIG_BLUE_BOX				13
#define SMALL_GREEN_10X_BOX			14
#define SMALL_CYAN_10X_BOX			15
#define SMALL_BLUE_10X_BOX			16
#define POPUP_GREEN_10X_BOX			17
#define POPUP_CYAN_10X_BOX			18
#define POPUP_BLUE_10X_BOX			19

// "Boxes" Icons
#define SMALL_YELLOW_BOX			0
#define BIG_YELLOW_BOX				1
#define SMALL_DULL_YELLOW_BOX		2
#define BIG_DULL_YELLOW_BOX			3
#define SMALL_WHITE_BOX				4
#define BIG_WHITE_BOX				5
#define SMALL_RED_BOX				6
#define BIG_RED_BOX					7
#define SMALL_QUESTION_MARK			8
#define BIG_QUESTION_MARK			9
#define VIP_SYMBOL					10
#define ENEMY_TANK_BIG				11
#define SMALL_YELLOW_10X_BOX		12
#define SMALL_DULL_YELLOW_10X_BOX	13
#define SMALL_WHITE_10X_BOX			14
#define SMALL_RED_10X_BOX			15

#define MERC_ICONS_PER_LINE			6
#define ROWS_PER_SECTOR				5

// Arrow Offsets
#define UP_X				13
#define UP_Y				 7
#define DOWN_X			   0
#define DOWN_Y			  -2
#define RIGHT_X			 -2
#define RIGHT_Y			 11
#define LEFT_X			   2
#define LEFT_Y			   5


// The Path Lines
#define NORTH_LINE		   1
#define SOUTH_LINE		   0
#define WEST_LINE			3
#define EAST_LINE			2
#define N_TO_E_LINE		  4
#define E_TO_S_LINE		  5
#define W_TO_N_LINE		  6
#define S_TO_W_LINE		  7
#define W_TO_S_LINE		  8
#define N_TO_W_LINE		  9
#define S_TO_E_LINE		 10
#define E_TO_N_LINE		 11
#define W_TO_E_LINE		 12
#define N_TO_S_LINE		 13
#define E_TO_W_LINE		 14
#define S_TO_N_LINE		 15
#define W_TO_E_PART1_LINE   16
#define W_TO_E_PART2_LINE   17
#define E_TO_W_PART1_LINE   18
#define E_TO_W_PART2_LINE   19
#define N_TO_S_PART1_LINE   20
#define N_TO_S_PART2_LINE   21
#define S_TO_N_PART1_LINE   22
#define S_TO_N_PART2_LINE   23
#define GREEN_X_WEST		36
#define GREEN_X_EAST		37
#define GREEN_X_NORTH	   38
#define GREEN_X_SOUTH	   39
#define RED_X_WEST		  40
#define RED_X_EAST		  41
#define RED_X_NORTH		 42
#define RED_X_SOUTH		 43

// The arrows
#define Y_NORTH_ARROW	   24
#define Y_SOUTH_ARROW	   25
#define Y_EAST_ARROW		26
#define Y_WEST_ARROW		27
#define W_NORTH_ARROW	   28
#define W_SOUTH_ARROW	   29
#define W_EAST_ARROW		30
#define W_WEST_ARROW		31
#define NORTH_ARROW		 32
#define SOUTH_ARROW		 33
#define EAST_ARROW		  34
#define WEST_ARROW		  35

//#define ZOOM_Y_NORTH_ARROW  68
//#define ZOOM_Y_SOUTH_ARROW  69
//#define ZOOM_Y_EAST_ARROW   70
//#define ZOOM_Y_WEST_ARROW   71
//#define ZOOM_W_NORTH_ARROW  72
//#define ZOOM_W_SOUTH_ARROW  73
//#define ZOOM_W_EAST_ARROW   74
//#define ZOOM_W_WEST_ARROW   75
//#define ZOOM_NORTH_ARROW	76
//#define ZOOM_SOUTH_ARROW	77
//#define ZOOM_EAST_ARROW	 78
//#define ZOOM_WEST_ARROW	 79
#define ARROW_DELAY		 70 // HEADROCK HAM 5: Slowed down as part of new arrow system
#define PAUSE_DELAY	   1000

// The zoomed in path lines
//#define SOUTH_ZOOM_LINE		 44
//#define NORTH_ZOOM_LINE		 45
//#define EAST_ZOOM_LINE		  46
//#define WEST_ZOOM_LINE		  47
//#define N_TO_E_ZOOM_LINE		48
//#define E_TO_S_ZOOM_LINE		49
//#define W_TO_N_ZOOM_LINE		50
//#define S_TO_W_ZOOM_LINE		51
//#define W_TO_S_ZOOM_LINE		52
//#define N_TO_W_ZOOM_LINE		53
//#define S_TO_E_ZOOM_LINE		54
//#define E_TO_N_ZOOM_LINE		55
//#define W_TO_E_ZOOM_LINE		56
//#define N_TO_S_ZOOM_LINE		57
//#define E_TO_W_ZOOM_LINE		58
//#define S_TO_N_ZOOM_LINE		59
//#define ZOOM_GREEN_X_WEST	   80
//#define ZOOM_GREEN_X_EAST	   81
//#define ZOOM_GREEN_X_NORTH	  82
//#define ZOOM_GREEN_X_SOUTH	  83
//#define ZOOM_RED_X_WEST		 84
//#define ZOOM_RED_X_EAST		 85
//#define ZOOM_RED_X_NORTH		86
//#define ZOOM_RED_X_SOUTH		87

#define CHAR_FONT_COLOR 32*4-9

// Arrow Offsets
#define EAST_OFFSET_X  11
#define EAST_OFFSET_Y  0
#define NORTH_OFFSET_X 9
#define NORTH_OFFSET_Y -9
#define SOUTH_OFFSET_X -9
#define SOUTH_OFFSET_Y 9
#define WEST_OFFSET_X  -11
#define WEST_OFFSET_Y  0
#define WEST_TO_SOUTH_OFFSET_Y  0
#define EAST_TO_NORTH_OFFSET_Y  0
#define RED_WEST_OFF_X  -MAP_GRID_X
#define RED_EAST_OFF_X  MAP_GRID_X
#define RED_NORTH_OFF_Y -21
#define RED_SOUTH_OFF_Y 21

// the font use on the mvt icons for mapscreen
// HEADROCK HAM 5: Externalized for larger map fonts.
//#define MAP_MVT_ICON_FONT SMALLCOMPFONT

// the big map .pcx
UINT32 guiBIGMAP;

// orta .sti icon
UINT32 guiORTAICON;
UINT32 guiTIXAICON;

// boxes for characters on the map
UINT32 guiCHARICONS;

// the merc arrival sector landing zone icon
UINT32 guiBULLSEYE;

UINT16 MAP_MILITIA_BOX_POS_X;
UINT16 MAP_MILITIA_BOX_POS_Y;

// the max allowable towns militia in a sector
#define MAP_MILITIA_MAP_X 4
#define MAP_MILITIA_MAP_Y 20
#define MAP_MILITIA_LOWER_ROW_Y 250
#define NUMBER_OF_MILITIA_ICONS_PER_LOWER_ROW 50
#define MILITIA_BOX_ROWS 6
#define MILITIA_BOX_BOX_HEIGHT 36
#define MILITIA_BOX_BOX_WIDTH 42

#define POPUP_MILITIA_ICONS_PER_ROW 5				// max 6 rows gives the limit of 30 militia
#define MEDIUM_MILITIA_ICON_SPACING 5
#define LARGE_MILITIA_ICON_SPACING  6

#define MILITIA_BTN_OFFSET_X 26
#define MILITIA_BTN_HEIGHT 11
#define MILITIA_BOX_WIDTH 266
#define MILITIA_BOX_TEXT_OFFSET_Y 4
#define MILITIA_BOX_UNASSIGNED_TEXT_OFFSET_Y 240
#define MILITIA_BOX_TEXT_TITLE_HEIGHT 13

#define MAP_MILITIA_BOX_AUTO_BOX_X 101
#define MAP_MILITIA_BOX_AUTO_BOX_Y 275
#define MAP_MILITIA_BOX_DONE_BOX_X 164
#define MAP_MILITIA_BOX_REMOVE_BOX_X 38

#define HELI_ICON					0
#define HELI_SHADOW_ICON	1

// HEADROCK HAM 5: Now dynamically read from helicopter icon file.
//#define HELI_ICON_WIDTH		20
//#define HELI_ICON_HEIGHT	10
// HEADROCK HAM 5: No longer drawn.
#define HELI_SHADOW_ICON_WIDTH	19
#define HELI_SHADOW_ICON_HEIGHT	11


// the militia box buttons and images
INT32 giMapMilitiaButtonImage[ 6 ];
INT32 giMapMilitiaButton[ 6 ] = { -1, -1, -1, -1, -1, -1 };

extern SECTOR_EXT_DATA	SectorExternalData[256][4];

INT16 gsMilitiaSectorButtonColors[]={
	FONT_LTGREEN,
	FONT_LTBLUE,
	16,
};

// track number of townspeople picked up
INT16 sGreensOnCursor = 0;
INT16 sRegularsOnCursor = 0;
INT16 sElitesOnCursor = 0;

// the current militia town id
INT16 sSelectedMilitiaTown = 0;

//extern MINE_LOCATION_TYPE gMineLocation[];
//extern MINE_STATUS_TYPE gMineStatus[];

// sublevel graphics
UINT32 guiSubLevel1, guiSubLevel2, guiSubLevel3;

// the between sector icons
UINT32	guiCHARBETWEENSECTORICONS;
// HEADROCK HAM 5.1: Enemies between sectors icons
UINT32  guiENEMYBETWEENSECTORICONS;

extern BOOLEAN fMapScreenBottomDirty;

// tixa found
BOOLEAN fFoundTixa = FALSE;

// selected sector
// HEADROCK HAM 3.5: Default selected sector doesn't have to be Omerta does it? :)
UINT16		sSelMapX = gGameExternalOptions.ubDefaultArrivalSectorX;
UINT16		sSelMapY = gGameExternalOptions.ubDefaultArrivalSectorY;

// highlighted sector
INT16 gsHighlightSectorX=-1;
INT16 gsHighlightSectorY=-1;

// the current sector Z value of the map being displayed
INT32 iCurrentMapSectorZ = 0;

// the palettes
// Flugente: unified palette code
UINT16* pMapPalette[MAP_SHADE_MAX];

// the map border eta pop up
UINT32 guiMapBorderEtaPopUp;

// heli pop up
UINT32 guiMapBorderHeliSectors;

// anv: alternate heli pop up for alternative fuel system
UINT32 guiMapBorderHeliSectorsAlternate;

// list of map sectors that player isn't allowed to even highlight
BOOLEAN sBadSectorsList[ WORLD_MAP_X ][ WORLD_MAP_X ];

MINERALS_VALUES MineralsName[255];

INT16 sBaseSectorList[ MAX_TOWNS - 1 ];/*={
	// NOTE: These co-ordinates must match the top left corner of the 3x3 town tiles cutouts in Interface/MilitiaMaps.sti!
	SECTOR(  9, 1 ), // Omerta
	SECTOR( 13, 2 ), // Drassen
	SECTOR( 13, 8 ), // Alma
	SECTOR(  1, 7 ), // Grumm
	SECTOR(  8, 9 ), // Tixa
	SECTOR(  8, 6 ), // Cambria
	SECTOR(  4, 2 ), // San Mona
	SECTOR(  5, 8 ), // Estoni
	SECTOR(  3,10 ), // Orta
	SECTOR( 11,11 ), // Balime
	SECTOR(  3,14 ), // Meduna
	SECTOR(  2, 1 ), // Chitzena
};*/

// position of town names on the map
// these are no longer PIXELS, but 10 * the X,Y position in SECTORS (fractions possible) to the X-CENTER of the town
POINT pTownPoints[ MAX_TOWNS ];/*={
	{ 0 ,  0 },
	{ 90, 10}, // Omerta
	{125, 40}, // Drassen
	{130, 90}, // Alma
	{ 15, 80}, // Grumm
	{ 85,100}, // Tixa
	{ 95, 70}, // Cambria
	{ 45, 40}, // San Mona
	{ 55, 90}, // Estoni
	{ 35,110}, // Orta
	{110,120}, // Balime
	{ 45,150}, // Meduna
	{ 15, 20}, // Chitzena
};
*/


extern BOOLEAN fSamSiteFoundOrig[ MAX_NUMBER_OF_SAMS ];

// map region
/*
SGPRect MapScreenRect={	(MAP_VIEW_START_X+MAP_GRID_X - 2),
						( MAP_VIEW_START_Y+MAP_GRID_Y - 1), 
						MAP_VIEW_START_X + MAP_VIEW_WIDTH - 1 + MAP_GRID_X , 
						MAP_VIEW_START_Y+MAP_VIEW_HEIGHT-10+MAP_GRID_Y};
*/

SGPRect MapScreenRect;

extern SGPRect gOldClipRect; // symbol already declared globally in renderworld.cpp (jonathanl)

// screen region
SGPRect FullScreenRect;//={ 0, 0, 1024, 768 };

// temp helicopter path
PathStPtr pTempHelicopterPath = NULL;

// character temp path
PathStPtr pTempCharacterPath = NULL;

// militia temp path
PathStPtr pTempMilitiaPath = NULL;

// draw temp path?
BOOLEAN fDrawTempMilitiaPath = FALSE;

// draw temp path?
BOOLEAN fDrawTempHeliPath = FALSE;

// the map arrows graphics
UINT32 guiMAPCURSORS;

// HEADROCK HAM 5: New pathing arrows may replace the above eventually, but for now a separate variable will do.
UINT32 guiMapPathingArrows;

// destination plotting character
INT8 bSelectedDestChar = -1;

// assignment selection character
INT8 bSelectedAssignChar=-1;

// current contract char
INT8 bSelectedContractChar = -1;


// has the temp path for character or helicopter been already drawn?
BOOLEAN   fTempPathAlreadyDrawn = FALSE;

// the regions for the mapscreen militia box
#define MILITIA_BOXREGIONS	36
MOUSE_REGION gMapScreenMilitiaBoxRegions[ MILITIA_BOXREGIONS ];
MOUSE_REGION gMapScreenMilitiaRegion;

// the mine icon
UINT32 guiMINEICON;

// militia graphics
UINT32 guiMilitia;
UINT32 guiMilitiaMaps;
UINT32 guiMilitiaSectorHighLight;
UINT32 guiMilitiaSectorOutline;

// the sector that is highlighted on the militia map
INT16 sSectorMilitiaMapSector = -1;
BOOLEAN fMilitiaMapButtonsCreated = FALSE;
INT16 sSectorMilitiaMapSectorOutline = -1;

// have any nodes in the current path list been deleted?
BOOLEAN fDeletedNode = FALSE;

UINT16 gusUndergroundNearBlack;

BOOLEAN gfMilitiaPopupCreated = FALSE;

INT32 giAnimateRouteBaseTime = 0;
INT32 giPotHeliPathBaseTime = 0;
INT32 giPotMilitiaPathBaseTime = 0;
INT32 giClickHeliIconBaseTime = 0;

// display the level string on the strategic map
void DisplayLevelString( void );

void ShowTownText( void );
void DrawTownLabels(STR16 pString, STR16 pStringA,UINT16 usFirstX, UINT16 usFirstY);
void ShowTeamAndVehicles(INT32 fShowFlags);
BOOLEAN ShadeMapElem( INT16 sMapX, INT16 sMapY, INT32 iColor );
void AdjustXForLeftMapEdge(STR16 wString, INT16 *psX, INT32 iFont);
void BlitTownGridMarkers( void );
void BlitMineGridMarkers( void );
void BlitSAMGridMarkers( void );
void BlitMineIcon( INT16 sMapX, INT16 sMapY );
void BlitMineText( INT16 sMapX, INT16 sMapY );
extern BOOLEAN GetMouseMapXY( INT16 *psMapWorldX, INT16 *psMapWorldY );
INT16 GetBaseSectorForCurrentTown( void );
void RenderIconsPerSectorForSelectedTown( void );
void MilitiaRegionClickCallback( MOUSE_REGION *pRegion, INT32 iReason );
void MilitiaRegionMoveCallback( MOUSE_REGION *pRegion, INT32 iReason );
void CreateDestroyMilitiaSectorButtons( void );
void ShowHighLightedSectorOnMilitiaMap( void );
void SetMilitiaMapButtonsText( void );
void MilitiaButtonCallback(GUI_BUTTON *btn,INT32 reason);
void DisplayUnallocatedMilitia( void );
BOOLEAN IsThisMilitiaTownSectorAllowable( INT16 sSectorIndexValue );
void DrawTownMilitiaName( void );
void HandleShutDownOfMilitiaPanelIfPeopleOnTheCursor( INT16 sTownValue );
void HandleRemovalOfAllTroopsAmongstSectors( void );
void HandleEveningOutOfTroopsAmongstSectors( void );
void CreateMilitiaPanelBottomButton( void );
void DeleteMilitiaPanelBottomButton( void );
void MilitiaDoneButtonCallback(GUI_BUTTON *btn,INT32 reason);
void MilitiaAutoButtonCallback(GUI_BUTTON *btn,INT32 reason);
void MilitiaRemoveButtonCallback(GUI_BUTTON *btn,INT32 reason);
void MilitiaDisbandYesNoBoxCallback( UINT8 bExitValue ); // HEADROCK HAM 3.6: Disband militia callback
void RenderShadingForUnControlledSectors( void );
void DrawTownMilitiaForcesOnMap( void );
void CheckAndUpdateStatesOfSelectedMilitiaSectorButtons( void );
void HandleLowerLevelMapBlit( void );
void ShowSAMSitesOnStrategicMap( void );

//UINT8 NumActiveCharactersInSector( INT16 sSectorX, INT16 sSectorY, INT16 bSectorZ );
//UINT8 NumFriendlyInSector( INT16 sX, INT16 sY, INT8 bZ );

// callbacks
void MilitiaBoxMaskBtnCallback(MOUSE_REGION * pRegion, INT32 iReason );

// display potential path, yes or no?
void ShowEnemiesInSector( INT16 sSectorX, INT16 sSectorY, INT16 sNumberOfEnemies, UINT16 usNumTanks, UINT8 ubIconPosition );
void ShowUncertainNumberEnemiesInSector( INT16 sSectorX, INT16 sSectorY );
void HandleShowingOfEnemyForcesInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 ubIconPosition );

BOOLEAN CanMilitiaAutoDistribute( void );

void ShowItemsOnMap( void );
void ShowDiseaseOnMap( );
void DrawMapBoxIcon( HVOBJECT hIconHandle, UINT16 usVOIndex, INT16 sMapX, INT16 sMapY, UINT8 ubIconPosition );
void DisplayDestinationOfHelicopter( void );
void DrawOrta();
void DrawTixa();
void DrawBullseye();
void HideExistenceOfUndergroundMapSector( UINT8 ubSectorX, UINT8 ubSectorY );

BOOLEAN CanMercsScoutThisSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

BOOLEAN TraceCharAnimatedRoute( PathStPtr pPath, BOOLEAN fCheckFlag, BOOLEAN fForceUpDate );
void AnimateRoute( PathStPtr pPath );

extern void EndConfirmMapMoveMode( void );
extern BOOLEAN CanDrawSectorCursor( void );

void DrawIconL( INT32 MAP_GRID_X2, INT32 MAP_GRID_Y2, INT32 i ); //Legion
UINT32	guiIcon2[256];
ICON_FILE gHiddenIcon[ 256 ];

#ifdef JA2UB
void SetUpValidCampaignSectors( void );
#endif

BOOLEAN LoadHiddenTownFromLoadGameFile( HWFILE hFile );
BOOLEAN SaveHiddenTownToSaveGameFile( HWFILE hFile );

//--------------Legion 2----Jazz-----------

void DrawIconL(INT32 MAP_GRID_X2, INT32 MAP_GRID_Y2, INT32 i )
{
	HVOBJECT hHandle;
		
	INT16 sX = (UINT16)(MAP_VIEW_START_X + MAP_GRID_X + (MAP_GRID_X2 * MAP_GRID_X) / 10);
	INT16 sY = (UINT16)(MAP_VIEW_START_Y + MAP_GRID_Y + ((MAP_GRID_Y2 * MAP_GRID_Y) / 10) + 1);
	
	INT8 ubVidObjIndex = 1;

	// draw Tixa in its sector
	GetVideoObject( &hHandle, guiIcon2[i]);
	BltVideoObject( guiSAVEBUFFER, hHandle, ubVidObjIndex, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
}

//-----------------------------------

// This method outputs the numbers (1-16) and letters (A - P)
void DrawMapIndexBigMap( BOOLEAN fSelectedCursorIsYellow )
{
	// this procedure will draw the coord indexes on the zoomed out map
	INT16 usX, usY;
	INT32 iCount=0;
	BOOLEAN fDrawCursors;

	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
	SetFont(MAP_FONT);
	SetFontForeground(MAP_INDEX_COLOR);
	// Dk Red is 163
	SetFontBackground(FONT_MCOLOR_BLACK);

	fDrawCursors = CanDrawSectorCursor( );

	for(iCount=1; iCount <= MAX_VIEW_SECTORS; ++iCount)
	{
		if ( fDrawCursors && (iCount == sSelMapX) && (bSelectedDestChar == -1) && !fPlotForHelicopter && !fPlotForMilitia )
			SetFontForeground( ( UINT8 ) ( fSelectedCursorIsYellow ? FONT_YELLOW : FONT_WHITE ) );
		else if( fDrawCursors && ( iCount == gsHighlightSectorX ) )
			SetFontForeground(FONT_WHITE);
		else
			SetFontForeground(MAP_INDEX_COLOR);

		FindFontCenterCoordinates(((INT16)(MAP_HORT_INDEX_X+( iCount - 1) *MAP_GRID_X)), MAP_HORT_INDEX_Y, MAP_GRID_X, MAP_HORT_HEIGHT, pMapHortIndex[iCount], MAP_FONT, &usX, &usY);
		mprintf(usX,usY,pMapHortIndex[iCount]);

		if ( fDrawCursors && (iCount == sSelMapY) && (bSelectedDestChar == -1) && !fPlotForHelicopter && !fPlotForMilitia )
			SetFontForeground( ( UINT8 ) ( fSelectedCursorIsYellow ? FONT_YELLOW : FONT_WHITE ) );
		else if( fDrawCursors && ( iCount == gsHighlightSectorY ) )
			SetFontForeground(FONT_WHITE);
		else
			SetFontForeground(MAP_INDEX_COLOR);

		FindFontCenterCoordinates(MAP_VERT_INDEX_X, ((INT16)(MAP_VERT_INDEX_Y+ ( iCount - 1) *MAP_GRID_Y)), MAP_HORT_HEIGHT, MAP_GRID_Y, pMapVertIndex[iCount], MAP_FONT, &usX, &usY);
		mprintf(usX,usY,pMapVertIndex[iCount]);
	}

	InvalidateRegion(MAP_VERT_INDEX_X, MAP_VERT_INDEX_Y,MAP_VERT_INDEX_X+MAP_HORT_HEIGHT,  MAP_VERT_INDEX_Y+( iCount - 1 ) * MAP_GRID_Y );
	InvalidateRegion(MAP_HORT_INDEX_X, MAP_HORT_INDEX_Y,MAP_HORT_INDEX_X + ( iCount - 1) * MAP_GRID_X,  MAP_HORT_INDEX_Y+ MAP_HORT_HEIGHT);

	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
}


void HandleShowingOfEnemiesWithMilitiaOn( void )
{
	INT32 iNumberOfGreens = 0;
	INT32 iNumberOfRegulars = 0;
	INT32 iNumberOfElites = 0;
	INT32 iTotalNumberOfIcons = 0;

	// if show militia flag is false, leave
	if( !fShowMilitia )
	{
		return;
	}

	for ( INT16 sX = 1; sX < (MAP_WORLD_X - 1); ++sX )
	{
		for ( INT16 sY = 1; sY < (MAP_WORLD_Y - 1); ++sY )
		{
			// get number of each
			iNumberOfGreens = MilitiaInSectorOfRank( sX, sY, GREEN_MILITIA );
			iNumberOfRegulars = MilitiaInSectorOfRank( sX, sY, REGULAR_MILITIA );
			iNumberOfElites = MilitiaInSectorOfRank( sX, sY, ELITE_MILITIA );
			
			// militia icons drawn
			iTotalNumberOfIcons = iNumberOfGreens / 10 + iNumberOfGreens % 10 + iNumberOfRegulars / 10 + iNumberOfRegulars % 10
				+ iNumberOfElites / 10 + iNumberOfElites % 10;

			HandleShowingOfEnemyForcesInSector( sX, sY, ( INT8 )iCurrentMapSectorZ, iTotalNumberOfIcons );
		}
	}
}


UINT32 DrawMap( void )
{
	HVSURFACE hSrcVSurface;
	UINT32 uiDestPitchBYTES;
	UINT32 uiSrcPitchBYTES;
	UINT16  *pDestBuf;
	UINT8  *pSrcBuf;
	INT16 cnt, cnt2;
	INT32 iCounter = 0;
	INT32 iCounter2 = 0;
	INT16 sBaseSectorValue = 0;

	//MAP_VIEW_START_X = (SCREEN_WIDTH - 370);
	//MAP_VIEW_START_Y = 10;

	MapScreenRect.iLeft = MAP_VIEW_START_X+MAP_GRID_X - 2;
	MapScreenRect.iTop = MAP_VIEW_START_Y+MAP_GRID_Y - 1;
	MapScreenRect.iRight = MAP_VIEW_START_X + MAP_VIEW_WIDTH - 1 + MAP_GRID_X;
	MapScreenRect.iBottom = MAP_VIEW_START_Y+MAP_VIEW_HEIGHT-10+MAP_GRID_Y;

	//MapScreenRect={	(MAP_VIEW_START_X+MAP_GRID_X - 2),	( MAP_VIEW_START_Y+MAP_GRID_Y - 1), MAP_VIEW_START_X + MAP_VIEW_WIDTH - 1 + MAP_GRID_X , MAP_VIEW_START_Y+MAP_VIEW_HEIGHT-10+MAP_GRID_Y};

	if( !iCurrentMapSectorZ )
	{
		pDestBuf = (UINT16*)LockVideoSurface( guiSAVEBUFFER, &uiDestPitchBYTES);

		CHECKF( GetVideoSurface( &hSrcVSurface, guiBIGMAP) );
		pSrcBuf = LockVideoSurface( guiBIGMAP, &uiSrcPitchBYTES);

		// clip blits to mapscreen region
		//ClipBlitsToMapViewRegion( );

		Blt8BPPDataTo16BPPBufferHalf( pDestBuf, uiDestPitchBYTES, hSrcVSurface, pSrcBuf, uiSrcPitchBYTES, MAP_VIEW_START_X + 1, MAP_VIEW_START_Y );

		UnLockVideoSurface( guiBIGMAP );
		UnLockVideoSurface( guiSAVEBUFFER );

		if ( !iCurrentMapSectorZ )
		{
			// shade map sectors (must be done after Tixa/Orta/Mine icons have been blitted, but before icons!)
			for ( cnt = 1; cnt < MAP_WORLD_X - 1; ++cnt )
			{
				for ( cnt2 = 1; cnt2 < MAP_WORLD_Y - 1; ++cnt2 )
				{
					// LATE DESIGN CHANGE: darken sectors not yet visited, instead of those under known enemy control
					if(!is_networked) //hayden - dont darken anything
					{
						// HEADROCK HAM 4: Mobile View shows all tiles as explored.
						if (!fShowMobileRestrictionsFlag )
						{
							if( GetSectorFlagStatus( cnt, cnt2, ( UINT8 ) iCurrentMapSectorZ, SF_ALREADY_VISITED ) == FALSE )
							// if ( IsTheSectorPerceivedToBeUnderEnemyControl( cnt, cnt2, ( INT8 )( iCurrentMapSectorZ ) ) )
							{
								if( fShowAircraftFlag )
								{
									if ( StrategicMap[cnt + cnt2 * WORLD_MAP_X].usAirType == AIRSPACE_PLAYER_ACTIVE )
									{
										// sector not visited but air controlled
										ShadeMapElem( cnt, cnt2, MAP_SHADE_DK_GREEN );
									}
									else if ( StrategicMap[cnt + cnt2 * WORLD_MAP_X].usAirType == AIRSPACE_ENEMY_ACTIVE )
									{
										// sector not visited and not air controlled
										ShadeMapElem( cnt, cnt2, MAP_SHADE_DK_RED );
									}
									else
									{
										// sector not visited, currently nobody controls the airspace
										ShadeMapElem( cnt, cnt2, MAP_SHADE_DK_BLUE );
									}
								}
								else if ( fShowStrategicDiseaseFlag )
								{
									ShadeMapElem( cnt, cnt2, GetMapColour(cnt, cnt2, fShowStrategicDiseaseFlag) );
								}
								else
								{
									// not visited
									if (gGameSettings.fOptions[TOPTION_ALT_MAP_COLOR])
									{
										// HEADROCK HAM 4: An alternate display for the map in normal mode. Unexplored sectors
										// are in grey, similar to a recon map. Others are left in normal vivid color.
										ShadeMapElem( cnt, cnt2, MAP_SHADE_DK_GREY );
									}
									else
									{
										ShadeMapElem( cnt, cnt2, MAP_SHADE_BLACK );
									}
								}
							}
							else
							{
								if( fShowAircraftFlag )
								{
									if ( StrategicMap[cnt + cnt2 * WORLD_MAP_X].usAirType == AIRSPACE_PLAYER_ACTIVE )
									{
										// sector visited and air controlled
										ShadeMapElem( cnt, cnt2, MAP_SHADE_LT_GREEN);
									}
									else if ( StrategicMap[cnt + cnt2 * WORLD_MAP_X].usAirType == AIRSPACE_ENEMY_ACTIVE )
									{
										// sector visited but not air controlled
										ShadeMapElem( cnt, cnt2, MAP_SHADE_LT_RED );
									}
									else
									{
										// sector visited, currently nobody controls the airspace
										ShadeMapElem( cnt, cnt2, MAP_SHADE_LT_BLUE );
									}
								}
								else if ( fShowStrategicDiseaseFlag )
								{
									ShadeMapElem( cnt, cnt2, GetMapColour(cnt, cnt2, fShowStrategicDiseaseFlag) );
								}
							}
						}
						else if (fShowMobileRestrictionsFlag )
						{
							// HEADROCK HAM 4: Show Manual Mobile Militia Restrictions
							UINT8 ubManualMobileMovementAllowed = ManualMobileMovementAllowed( SECTOR(cnt, cnt2) );
							switch ( ubManualMobileMovementAllowed )
							{
								case 0:
									// Mobiles not allowed here at all.
									ShadeMapElem( cnt, cnt2, MAP_SHADE_DK_GREY );
									break;
								case 1:
									// Mobiles forbidden by player
									ShadeMapElem( cnt, cnt2, MAP_SHADE_MD_RED );
									break;
								case 2:
									// Mobiles can enter but not leave
									ShadeMapElem( cnt, cnt2, MAP_SHADE_LT_YELLOW );
									break;
								case 3:
									// Mobiles can enter
									ShadeMapElem( cnt, cnt2, MAP_SHADE_LT_GREEN );
									break;
							}
						}
					}
				}
			}
		}

		// UNFORTUNATELY, WE CAN'T SHADE THESE ICONS AS PART OF SHADING THE MAP, BECAUSE FOR AIRSPACE, THE SHADE FUNCTION
		// DOESN'T MERELY SHADE THE EXISTING MAP SURFACE, BUT INSTEAD GRABS THE ORIGINAL GRAPHICS FROM BIGMAP, AND CHANGES
		// THEIR PALETTE.  BLITTING ICONS PRIOR TO SHADING WOULD MEAN THEY DON'T SHOW UP IN AIRSPACE VIEW AT ALL.
		
		for (cnt = 1; cnt < NUM_TOWNS; ++cnt)
		{
			if ( gfHiddenTown[cnt] && gfIconTown[cnt] && gfDrawHiddenTown[cnt] )
			{
				sBaseSectorValue = sBaseSectorList[ cnt-1 ];
					
				INT8 bTownId = GetTownIdForSector( SECTORX( sBaseSectorValue ), SECTORY( sBaseSectorValue ) );
				if ( /*bTownId != 0 &&*/ bTownId < NUM_TOWNS )
				{
					DrawIconL( gHiddenIcon[cnt].IconX, gHiddenIcon[cnt].IconY, cnt );
				}	
			}
		}

		/*
		// if Orta found
		if( fFoundOrta )
		{
			DrawOrta();
		}

		// if Tixa found
		if( fFoundTixa )
		{
			DrawTixa();
		}
		*/
		
		// draw SAM sites
		ShowSAMSitesOnStrategicMap( );

		// draw mine icons
		for( iCounter = 0; iCounter < MAX_NUMBER_OF_MINES; ++iCounter )
		{
			//BlitMineIcon( gMineLocation[ iCounter ].sSectorX, gMineLocation[ iCounter ].sSectorY );
			BlitMineIcon( gMineStatus[ iCounter ].sSectorX, gMineStatus[ iCounter ].sSectorY );
		}

		// if mine details filter is set
		if( fShowMineFlag )
		{
			// show mine name/production text
			for( iCounter = 0; iCounter < MAX_NUMBER_OF_MINES; ++iCounter )
			{
				//BlitMineText( gMineLocation[ iCounter ].sSectorX, gMineLocation[ iCounter ].sSectorY );
				BlitMineText( gMineStatus[ iCounter ].sSectorX, gMineStatus[ iCounter ].sSectorY );
			}
		}

		// draw towns names & loyalty ratings, and grey town limit borders
		if( fShowTownFlag )
		{
			BlitTownGridMarkers( );
			ShowTownText( );
		}

		// draw militia icons
		if( fShowMilitia )
		{
			DrawTownMilitiaForcesOnMap( );
		}
		
		if ( fShowAircraftFlag && !gfInChangeArrivalSectorMode )
		{
			DrawBullseye();
		}
		else if(is_networked)
		{
			DrawBullseye();//haydent (allow client to see map deployment bullseye)
		}
	}
	else
	{
		HandleLowerLevelMapBlit( );
	}

	// show mine outlines even when viewing underground sublevels - they indicate where the mine entrances are
	if ( fShowMineFlag )
	{
		// draw grey mine sector borders
		BlitMineGridMarkers( );
	}

	// do not show mercs/vehicles when airspace is ON
// commented out on a trial basis!
//	if( !fShowAircraftFlag )
	{
		if( fShowTeamFlag )
			ShowTeamAndVehicles( SHOW_TEAMMATES | SHOW_VEHICLES );
		else
			HandleShowingOfEnemiesWithMilitiaOn( );

/*
		if((fShowTeamFlag)&&(fShowVehicleFlag))
		ShowTeamAndVehicles(SHOW_TEAMMATES | SHOW_VEHICLES);
		else if(fShowTeamFlag)
			ShowTeamAndVehicles(SHOW_TEAMMATES);
		else if(fShowVehicleFlag)
			ShowTeamAndVehicles(SHOW_VEHICLES);
		else
		{
			HandleShowingOfEnemiesWithMilitiaOn( );
		}
*/
	}

	if ( fShowItemsFlag )
	{
		ShowItemsOnMap();
	}

	if ( fShowStrategicDiseaseFlag )
	{
		ShowDiseaseOnMap( );
	}

	DisplayLevelString( );

	RestoreClipRegionToFullScreen( );

	// Flugente: this function does not do anything... so I'm commenting its use out
	//CheckForSoldiersWhoRetreatedIntoMilitiaHeldSectors();

	return( TRUE );
}


void GetScreenXYFromMapXY( INT16 sMapX, INT16 sMapY, INT16 *psX, INT16 *psY )
{
	*psX = ( sMapX * MAP_GRID_X ) + MAP_VIEW_START_X;
	*psY = ( sMapY * MAP_GRID_Y ) + MAP_VIEW_START_Y;
}


void ShowTownText( void )
{
	CHAR16 sString[ 32 ];
	CHAR16 sStringA[ 32 ];
	INT8 bTown = 0;
	UINT16 usX,usY;
	BOOLEAN fLoyaltyTooLowToTrainMilitia;

	INT32 iMinLoyaltyToTrain = gGameExternalOptions.iMinLoyaltyToTrain;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Map Screen1");

	// this procedure will display the town names on the screen

	SetFont(MAP_FONT);
	SetFontBackground(FONT_MCOLOR_BLACK);

 	for( bTown = FIRST_TOWN; bTown < NUM_TOWNS; ++bTown)
	{
		// skip Orta/Tixa until found
		//if( ( ( fFoundOrta != FALSE ) || ( bTown != ORTA ) ) && ( ( bTown != TIXA ) || ( fFoundTixa != FALSE) ) )
		if( gfHiddenTown[ bTown ] == TRUE )
		{
			swprintf( sString, L"%s", pTownNames[ bTown ] );

			fLoyaltyTooLowToTrainMilitia = FALSE;

			// don't show loyalty string until loyalty tracking for that town has been started
			if( gTownLoyalty[ bTown ].fStarted && gfTownUsesLoyalty[ bTown ])
			{
				#ifdef CHINESE
					swprintf( sStringA, L"%d%ге%% %s", gTownLoyalty[ bTown ].ubRating, gsLoyalString[ 0 ]);
				#else
					swprintf( sStringA, L"%d%%%% %s", gTownLoyalty[ bTown ].ubRating, gsLoyalString[ 0 ]);
				#endif
				
				// if loyalty is too low to train militia, and militia training is allowed here
				if ( ( gTownLoyalty[ bTown ].ubRating < iMinLoyaltyToTrain ) && MilitiaTrainingAllowedInTown( bTown ) )
				{
					fLoyaltyTooLowToTrainMilitia = TRUE;
				}
			}
			else
			{
				wcscpy( sStringA, L"");
			}

			usX = (UINT16) (MAP_VIEW_START_X + MAP_GRID_X +  (pTownPoints[ bTown ].x * MAP_GRID_X) / 10);
			usY = (UINT16) (MAP_VIEW_START_Y + MAP_GRID_Y + ((pTownPoints[ bTown ].y * MAP_GRID_Y) / 10) + 1);

			// red for low loyalty, green otherwise
			SetFontForeground( ( UINT8 ) ( fLoyaltyTooLowToTrainMilitia ? FONT_MCOLOR_RED : FONT_MCOLOR_LTGREEN ) );
			DrawTownLabels(sString, sStringA, usX, usY);
		}
	}
}


void DrawTownLabels(STR16 pString, STR16 pStringA, UINT16 usFirstX, UINT16 usFirstY)
{
	// this procedure will draw the given strings on the screen centered around the given x and at the given y

	INT16 sSecondX, sSecondY;
	INT16 sPastEdge;

	// HEADROCK HAM 5: Now variable...
	INT32 MapTownLabelsFont;
	if (iResolution <= _800x600 )
	{
		MapTownLabelsFont = MAP_FONT;
	}
	else
	{
		MapTownLabelsFont = FONT14ARIAL;
	}
	SetFont( MapTownLabelsFont );

	// if within view region...render, else don't
	if( ( usFirstX > MAP_VIEW_START_X + MAP_VIEW_WIDTH )||( usFirstX < MAP_VIEW_START_X )|| (usFirstY < MAP_VIEW_START_Y ) || ( usFirstY > MAP_VIEW_START_Y + MAP_VIEW_HEIGHT ) )
	{
		return;
	}

	SetFontDestBuffer( guiSAVEBUFFER, MapScreenRect.iLeft + 2, MapScreenRect.iTop, MapScreenRect.iRight, MapScreenRect.iBottom , FALSE );

	// clip blits to mapscreen region
	ClipBlitsToMapViewRegion( );

	// we're CENTERING the first string AROUND usFirstX, so calculate the starting X
	usFirstX -= StringPixLength( pString, MapTownLabelsFont) / 2;

	// print first string
	gprintfdirty( usFirstX, usFirstY, pString);
	mprintf(usFirstX, usFirstY, pString);

	// calculate starting coordinates for the second string
	VarFindFontCenterCoordinates(( INT16 )( usFirstX ), ( INT16 )usFirstY, StringPixLength( pString, MapTownLabelsFont), 0, MapTownLabelsFont, &sSecondX, &sSecondY, pStringA);

	// make sure we don't go past left edge (Grumm)
	sPastEdge = (MAP_VIEW_START_X + 23) - sSecondX;

	if (sPastEdge > 0)
		sSecondX += sPastEdge;

	// print second string beneath first
	sSecondY = ( INT16 )( usFirstY + GetFontHeight( MapTownLabelsFont ) );
	gprintfdirty( sSecondX, sSecondY, pStringA );
	mprintf( sSecondX, sSecondY, pStringA );

	// restore clip blits
	RestoreClipRegionToFullScreen( );
}



// "on duty" includes mercs inside vehicles
INT32 ShowOnDutyTeam( INT16 sMapX, INT16 sMapY )
{
	UINT8 ubCounter = 0, ubIconPosition = 0;
	HVOBJECT hIconHandle;
	SOLDIERTYPE *pSoldier = NULL;
	UINT8 i, ubIcon, ub10xIcon;
	INT16 sNumberOnDuty = 0;

	GetVideoObject(&hIconHandle, guiCHARICONS);

	// run through list
	while(gCharactersList[ubCounter].fValid)
	{
		pSoldier = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];

		if( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) &&
				( pSoldier->sSectorX == sMapX) &&
				( pSoldier->sSectorY == sMapY) &&
				( pSoldier->bSectorZ == iCurrentMapSectorZ ) &&
				( ( pSoldier->bAssignment < ON_DUTY ) || ( ( pSoldier->bAssignment == VEHICLE ) && ( pSoldier->iVehicleId != iHelicopterVehicleId ) ) ) &&
				( pSoldier->stats.bLife > 0) &&
				( !PlayerIDGroupInMotion( pSoldier->ubGroupID ) ) )
		{
			++sNumberOnDuty;
		}

		++ubCounter;
	}

	ub10xIcon = sNumberOnDuty / 10;
	ubIcon = sNumberOnDuty % 10;

	// draw 10x icon
	for ( i = 0; i < ub10xIcon; ++i )
	{
		DrawMapBoxIcon( hIconHandle, SMALL_YELLOW_10X_BOX, sMapX, sMapY, ubIconPosition );
		++ubIconPosition;
	}
	
	// draw 1x icon
	for ( i = 0; i < ubIcon; ++i )
	{
		DrawMapBoxIcon( hIconHandle, SMALL_YELLOW_BOX, sMapX, sMapY, ubIconPosition );
		++ubIconPosition;
	}

	return ubIconPosition;
}


INT32 ShowAssignedTeam(INT16 sMapX, INT16 sMapY, INT32 iCount)
{
 	UINT8 ubCounter = 0, ubIconPosition;
	HVOBJECT hIconHandle;
	SOLDIERTYPE *pSoldier = NULL;
	UINT8 i, ubIcon, ub10xIcon;
	INT16 sNumberOfAssigned = 0;

	GetVideoObject(&hIconHandle, guiCHARICONS);

	// run through list
	ubIconPosition = ( UINT8 ) iCount;

	while(gCharactersList[ubCounter].fValid)
	{
		pSoldier = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];

		// given number of on duty members, find number of assigned chars
		// start at beginning of list, look for people who are in sector and assigned
		if( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) &&
				( pSoldier->sSectorX == sMapX) &&
				( pSoldier->sSectorY == sMapY) &&
				( pSoldier->bSectorZ == iCurrentMapSectorZ ) &&
				( pSoldier->bAssignment >= ON_DUTY ) && ( pSoldier->bAssignment != VEHICLE ) &&
				( pSoldier->bAssignment != IN_TRANSIT ) &&
				( pSoldier->bAssignment != ASSIGNMENT_POW ) &&
				( pSoldier->stats.bLife > 0 ) &&
				( !PlayerIDGroupInMotion( pSoldier->ubGroupID ) ) )
		{
			// skip mercs inside the helicopter if we're showing airspace level - they show up inside chopper icon instead
			if ( !fShowAircraftFlag || ( pSoldier->bAssignment != VEHICLE ) || ( pSoldier->iVehicleId != iHelicopterVehicleId ) )
			{
				++sNumberOfAssigned;
			}
		}

		++ubCounter;
	}
	
	ub10xIcon = sNumberOfAssigned / 10;
	ubIcon = sNumberOfAssigned % 10;

	// draw 10x icon
	for( i = 0; i < ub10xIcon; ++i )
	{
		DrawMapBoxIcon( hIconHandle, SMALL_DULL_YELLOW_10X_BOX, sMapX, sMapY, ubIconPosition );
		++ubIconPosition;
	}
	
	// draw 1x icon
	for ( i = 0; i < ubIcon; ++i )
	{
		DrawMapBoxIcon( hIconHandle, SMALL_DULL_YELLOW_BOX, sMapX, sMapY, ubIconPosition );
		++ubIconPosition;
	}
	return ubIconPosition;
}

INT32 ShowVehicles(INT16 sMapX, INT16 sMapY, INT32 iCount)
{
	UINT8 ubCounter = 0, ubIconPosition;
	HVOBJECT hIconHandle;
	SOLDIERTYPE *pVehicleSoldier;
	UINT8 i, ubIcon, ub10xIcon;
	INT16 sNumberOfVehiclesInSector = 0;
	
	GetVideoObject(&hIconHandle, guiCHARICONS);

	ubIconPosition = (UINT8) iCount;

	// run through list of vehicles
	while( ubCounter < ubNumberOfVehicles )
	{
		// skip the chopper, it has its own icon and displays in airspace mode
		if ( ubCounter != iHelicopterVehicleId )
		{
			if ( (pVehicleList[ ubCounter ].sSectorX == sMapX) && ( pVehicleList[ ubCounter ].sSectorY == sMapY ) )
			{
				// don't show vehicles between sectors (in motion - they're counted as "people in motion"
				if( ( pVehicleList[ ubCounter ].sSectorZ == iCurrentMapSectorZ ) &&
						!PlayerIDGroupInMotion( pVehicleList[ ubCounter ].ubMovementGroup ) )
				{
					// ATE: Check if this vehicle has a soldier and it's on our team.....
					pVehicleSoldier = GetSoldierStructureForVehicle( ubCounter );

					// this skips the chopper, which has no soldier
					if ( pVehicleSoldier )
					{
						if ( pVehicleSoldier->bTeam == gbPlayerNum )
						{
							++sNumberOfVehiclesInSector;
						}
					}
				}
			}
		}
		++ubCounter;
	}

	ub10xIcon = sNumberOfVehiclesInSector / 10;
	ubIcon = sNumberOfVehiclesInSector % 10;

	// draw 10x icon
	for( i = 0; i < ub10xIcon; ++i )
	{
		DrawMapBoxIcon( hIconHandle, SMALL_WHITE_10X_BOX, sMapX, sMapY, ubIconPosition );
		++ubIconPosition;
	}

	// draw 1x icon
	for( i = 0; i < ubIcon; ++i )
	{
		DrawMapBoxIcon( hIconHandle, SMALL_WHITE_BOX, sMapX, sMapY, ubIconPosition );
		++ubIconPosition;
	}

	return ubIconPosition;
}


void ShowEnemiesInSector( INT16 sSectorX, INT16 sSectorY, INT16 sNumberOfEnemies, UINT16 usNumTanks, UINT8 ubIconPosition )
{
	HVOBJECT hIconHandle;
	UINT8 i, ubEnemy, ub10xEnemy;
	INT16 sEnemyLessTank;

	// get the video object
	GetVideoObject(&hIconHandle, guiCHARICONS);
	
	sEnemyLessTank = sNumberOfEnemies - usNumTanks;
	
	// no 10x icon for easy enemy quantity gauge in low resolution
	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		ub10xEnemy = 0;
		ubEnemy = sNumberOfEnemies;
	}
	else
	{
		ub10xEnemy = sNumberOfEnemies / 10;
		ubEnemy = sNumberOfEnemies % 10;
	}

	// Flugente: if we display tanks, we need to know that for several rows
	INT16 secondtankrowstart = -1;
	
	for( i = 0; i < sNumberOfEnemies; ++i )
	{
		// Flugente: the tank icon has is 4 icons wide and 2 rows high, thus this odd code bit
		// if we want to display a tank and have not yet done so, pick the first position that still has enough space to the right
		if ( usNumTanks && secondtankrowstart < 0 && ( ubIconPosition % MERC_ICONS_PER_LINE ) < 3 )
		{
			DrawMapBoxIcon( hIconHandle, ENEMY_TANK_BIG, sSectorX, sSectorY, ubIconPosition );
			secondtankrowstart = ubIconPosition + 6;
			ubIconPosition += 4;
		}
		// if this is the second row of a tank, don't draw anything
		else if ( usNumTanks && secondtankrowstart > -1 && ubIconPosition == secondtankrowstart )
		{
			// for now display only one tank icon
			usNumTanks = 0;
			ubIconPosition += 4;		
			ubEnemy = max( 0, ubEnemy - 1);
		}
		else if ( ub10xEnemy > 0 )
		{
			DrawMapBoxIcon( hIconHandle, SMALL_RED_10X_BOX, sSectorX, sSectorY, ubIconPosition );
			--ub10xEnemy;
			++ubIconPosition;
		}
		else if ( ubEnemy > 0 )
		{
			DrawMapBoxIcon( hIconHandle, SMALL_RED_BOX, sSectorX, sSectorY, ubIconPosition );
			--ubEnemy;
			++ubIconPosition;
		}
	}
}

void ShowUncertainNumberEnemiesInSector( INT16 sSectorX, INT16 sSectorY )
{
	INT16 sXPosition = 0, sYPosition = 0;
	HVOBJECT hIconHandle;

	UINT8 iconOffsetX = 0;
	UINT8 iconOffsetY = 0;
	
	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		iconOffsetX = 2;
		iconOffsetY = 9;
	}
	else if (iResolution < _1024x768)
	{
		iconOffsetX = 8;
		iconOffsetY = 12;
	}
	else
	{
		iconOffsetX = 12;
		iconOffsetY = 13;
	}

	// grab the x and y postions
	sXPosition = sSectorX;
	sYPosition = sSectorY;

	// get the video object
	GetVideoObject(&hIconHandle, guiCHARICONS);

	// check if we are zoomed in...need to offset in case for scrolling purposes
	sXPosition = ( INT16 )( iconOffsetX + ( MAP_VIEW_START_X + ( sSectorX * MAP_GRID_X + 1 )  ) - 1 );
	sYPosition = ( INT16 )( ( ( iconOffsetY + ( yResOffset + sSectorY * MAP_GRID_Y ) + 1 )  ) );
	sYPosition -= 2;

	// small question mark
	BltVideoObject(guiSAVEBUFFER, hIconHandle, SMALL_QUESTION_MARK, sXPosition, sYPosition, VO_BLT_SRCTRANSPARENCY, NULL );
	InvalidateRegion( sXPosition ,sYPosition, sXPosition + DMAP_GRID_X, sYPosition + DMAP_GRID_Y );
}

void ShowVIPSymbol( INT16 sSectorX, INT16 sSectorY )
{
	INT16 sXPosition = 0, sYPosition = 0;
	HVOBJECT hIconHandle;

	UINT8 iconOffsetX = 0;
	UINT8 iconOffsetY = 0;

	if ( iResolution >= _640x480 && iResolution < _800x600 )
	{
		iconOffsetX = 2;
		iconOffsetY = 9;
	}
	else if ( iResolution < _1024x768 )
	{
		iconOffsetX = 8;
		iconOffsetY = 12;
	}
	else
	{
		iconOffsetX = 12;
		iconOffsetY = 13;
	}

	// grab the x and y postions
	sXPosition = sSectorX;
	sYPosition = sSectorY;

	// get the video object
	GetVideoObject( &hIconHandle, guiCHARICONS );

	// check if we are zoomed in...need to offset in case for scrolling purposes
	sXPosition = (INT16)(iconOffsetX + ( MAP_VIEW_START_X + (sSectorX * MAP_GRID_X + 1)) - 1);
	sYPosition = (INT16)(((iconOffsetY + ( yResOffset + sSectorY * MAP_GRID_Y ) + 1)));
	sYPosition -= 2;

	// small VIP symbol
	BltVideoObject( guiSAVEBUFFER, hIconHandle, VIP_SYMBOL, sXPosition, sYPosition, VO_BLT_SRCTRANSPARENCY, NULL );
	InvalidateRegion( sXPosition, sYPosition, sXPosition + DMAP_GRID_X, sYPosition + DMAP_GRID_Y );
}


void ShowTeamAndVehicles(INT32 fShowFlags)
{
	// go through each sector, display the on duty, assigned, and vehicles
	INT16 sMapX = 0;
	INT16 sMapY = 0;
	INT32 iIconOffset = 0;
	BOOLEAN fContemplatingRetreating = FALSE;
	
	if( gfDisplayPotentialRetreatPaths && gpBattleGroup )
	{
		fContemplatingRetreating = TRUE;
	}

	for(sMapX=1; sMapX <MAP_WORLD_X-1; sMapX++)
	{
		for(sMapY=1; sMapY <MAP_WORLD_Y-1; sMapY++)
		{
			// don't show mercs/vehicles currently in this sector if player is contemplating retreating from THIS sector
			if ( !fContemplatingRetreating || ( sMapX != gpBattleGroup->ubSectorX ) || ( sMapY != gpBattleGroup->ubSectorY ) )
			{
				if(fShowFlags & SHOW_TEAMMATES)
				{
					iIconOffset = ShowOnDutyTeam( sMapX, sMapY );
					iIconOffset = ShowAssignedTeam( sMapX, sMapY, iIconOffset );
				}

				if(fShowFlags & SHOW_VEHICLES)
					iIconOffset = ShowVehicles( sMapX, sMapY, iIconOffset );
			}

			if(fShowFlags & SHOW_TEAMMATES)
			{
				HandleShowingOfEnemyForcesInSector( sMapX, sMapY, ( INT8 )iCurrentMapSectorZ, ( UINT8 ) iIconOffset );
				ShowPeopleInMotion( sMapX, sMapY );
			}
		}
	}
}


BOOLEAN ShadeMapElem( INT16 sMapX, INT16 sMapY, INT32 iColor )
{
	INT16	sScreenX, sScreenY;
	HVSURFACE hSrcVSurface;
	UINT32 uiDestPitchBYTES;
	UINT32 uiSrcPitchBYTES;
	UINT16  *pDestBuf;
	UINT8  *pSrcBuf;
	SGPRect clip;
	UINT16 *pOriginalPallette;

	// get original video surface palette
	CHECKF( GetVideoSurface( &hSrcVSurface, guiBIGMAP ) );

	pOriginalPallette = hSrcVSurface->p16BPPPalette;

	GetScreenXYFromMapXY( sMapX, sMapY, &sScreenX, &sScreenY );

	// compensate for original BIG_MAP blit being done at MAP_VIEW_START_X + 1
	sScreenX += 1;

	// compensate for original BIG_MAP blit being done at MAP_VIEW_START_X + 1
	clip.iLeft = 2 * ( sScreenX - ( MAP_VIEW_START_X + 1 ) );
	clip.iTop  = 2 * ( sScreenY - MAP_VIEW_START_Y );
	clip.iRight  = clip.iLeft + ( 2 * MAP_GRID_X );
	clip.iBottom = clip.iTop  + ( 2 * MAP_GRID_Y );

	if( iColor == MAP_SHADE_BLACK )
	{
		// non-airspace
		sScreenY -= 1;

		// simply shade darker
		ShadowVideoSurfaceRect( guiSAVEBUFFER, sScreenX, sScreenY, sScreenX + MAP_GRID_X - 1, sScreenY + MAP_GRID_Y - 1 );
	}
	else if ( iColor < MAP_SHADE_MAX )
	{
		// grab video surface and set palette
		CHECKF( GetVideoSurface( &hSrcVSurface, guiBIGMAP ) );
		hSrcVSurface->p16BPPPalette = pMapPalette[iColor];

		// lock source and dest buffers
		pDestBuf = (UINT16*)LockVideoSurface( guiSAVEBUFFER, &uiDestPitchBYTES );
		CHECKF( GetVideoSurface( &hSrcVSurface, guiBIGMAP ) );
		pSrcBuf = LockVideoSurface( guiBIGMAP, &uiSrcPitchBYTES );

		Blt8BPPDataTo16BPPBufferHalfRect( pDestBuf, uiDestPitchBYTES, hSrcVSurface, pSrcBuf, uiSrcPitchBYTES, sScreenX, sScreenY, &clip );

		// unlock source and dest buffers
		UnLockVideoSurface( guiBIGMAP );
		UnLockVideoSurface( guiSAVEBUFFER );
	}

	// restore original palette
	CHECKF( GetVideoSurface( &hSrcVSurface, guiBIGMAP) );
	hSrcVSurface->p16BPPPalette = pOriginalPallette;

	return ( TRUE );
}


void InitializeMilitiaPopup(void)
{
	UINT16 xVal = 330 + xResOffset;
	UINT16 yVal = 25 + yResOffset;

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		MAP_MILITIA_BOX_POS_X = xVal;
		MAP_MILITIA_BOX_POS_Y = yVal;
	}
	else if (iResolution < _1024x768)
	{
		MAP_MILITIA_BOX_POS_X = xVal + 77;
		MAP_MILITIA_BOX_POS_Y = yVal + 116;
	}
	else
	{
		MAP_MILITIA_BOX_POS_X = xVal + 190;
		MAP_MILITIA_BOX_POS_Y = yVal + 285;
	}
}

BOOLEAN InitializePalettesForMap( void )
{
	// init palettes
	HVSURFACE hSrcVSurface;
	SGPPaletteEntry pPalette[ 256 ];
	VSURFACE_DESC		vs_desc;
	UINT32 uiTempMap;

	// load image
	vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;

	if (iResolution >= _640x480 && iResolution < _800x600)
 		strcpy(vs_desc.ImageFile, "INTERFACE\\b_map.pcx");
	else if (iResolution < _1024x768)
		strcpy(vs_desc.ImageFile, "INTERFACE\\b_map_800x600.pcx");
	else
		strcpy(vs_desc.ImageFile, "INTERFACE\\b_map_1024x768.pcx");

	CHECKF(AddVideoSurface(&vs_desc, &uiTempMap));

	// get video surface
	CHECKF( GetVideoSurface( &hSrcVSurface, uiTempMap) );
	GetVSurfacePaletteEntries( hSrcVSurface, pPalette );

	// set up various palettes
	pMapPalette[MAP_SHADE_LT_GREEN]		= Create16BPPPaletteShaded( pPalette, 100, 300, 0, TRUE );
	pMapPalette[MAP_SHADE_MD_GREEN]		= Create16BPPPaletteShaded( pPalette, 50, 250, 0, TRUE );
	pMapPalette[MAP_SHADE_DK_GREEN]		= Create16BPPPaletteShaded( pPalette, 0, 200, 0, TRUE );

	pMapPalette[MAP_SHADE_LT_RED]		= Create16BPPPaletteShaded( pPalette, 400, 0, 0, TRUE );
	pMapPalette[MAP_SHADE_MD_RED]		= Create16BPPPaletteShaded( pPalette, 300, 0, 0, TRUE );
	pMapPalette[MAP_SHADE_DK_RED]		= Create16BPPPaletteShaded( pPalette, 200, 0, 0, TRUE );
	
	pMapPalette[MAP_SHADE_LT_YELLOW]	= Create16BPPPaletteShaded( pPalette, 400, 350, 0, TRUE );
	pMapPalette[MAP_SHADE_MD_YELLOW]	= Create16BPPPaletteShaded( pPalette, 300, 275, 0, TRUE );
	pMapPalette[MAP_SHADE_DK_YELLOW]	= Create16BPPPaletteShaded( pPalette, 200, 200, 0, TRUE );
	
	pMapPalette[MAP_SHADE_LT_CYAN]		= Create16BPPPaletteShaded( pPalette, 0, 350, 350, TRUE );
	pMapPalette[MAP_SHADE_MD_CYAN]		= Create16BPPPaletteShaded( pPalette, 0, 275, 275, TRUE );
	pMapPalette[MAP_SHADE_DK_CYAN]		= Create16BPPPaletteShaded( pPalette, 0, 200, 200, TRUE );

	pMapPalette[MAP_SHADE_LT_GREY]		= Create16BPPPaletteShaded( pPalette, 400, 400, 400, TRUE );
	pMapPalette[MAP_SHADE_MD_GREY]		= Create16BPPPaletteShaded( pPalette, 325, 325, 325, TRUE );
	pMapPalette[MAP_SHADE_DK_GREY]		= Create16BPPPaletteShaded( pPalette, 250, 250, 250, TRUE );

	pMapPalette[MAP_SHADE_LT_BLUE]		= Create16BPPPaletteShaded( pPalette, 0, 0, 400, TRUE );
	pMapPalette[MAP_SHADE_MD_BLUE]		= Create16BPPPaletteShaded( pPalette, 0, 0, 300, TRUE );
	pMapPalette[MAP_SHADE_DK_BLUE]		= Create16BPPPaletteShaded( pPalette, 0, 0, 200, TRUE );

	pMapPalette[MAP_SHADE_LT_PINK]		= Create16BPPPaletteShaded( pPalette, 200, 40, 175, TRUE );
	pMapPalette[MAP_SHADE_ORANGE]		= Create16BPPPaletteShaded( pPalette, 255, 127, 40, TRUE );

	// delete image
	DeleteVideoSurfaceFromIndex(uiTempMap);

	return ( TRUE );
}


void ShutDownPalettesForMap( void )
{
	for ( UINT8 i = 0; i < MAP_SHADE_MAX; ++i )
	{
		MemFree( pMapPalette[i] );
		pMapPalette[i] = NULL;
	}
}

void PlotPathForCharacter( SOLDIERTYPE *pCharacter, INT16 sX, INT16 sY, BOOLEAN fTacticalTraversal )
{
	// will plot a path for this character

	// is cursor allowed here?..if not..don't build path
	if( !IsTheCursorAllowedToHighLightThisSector( sX, sY ) )
	{
		return;
	}

	// is the character in transit?..then leave
	if( pCharacter->bAssignment == IN_TRANSIT )
	{
		// leave
		return;
	}


	if( pCharacter->bSectorZ != 0 )
	{
		if( pCharacter->bAssignment >= ON_DUTY )
		{
			// not on the surface, character won't move until they reach surface..info player of this fact
			MapScreenMessage( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"%s %s", pCharacter->name, gsUndergroundString[0] );
		}
		else	// squad
		{
			if ( gGameExternalOptions.fUseXMLSquadNames )
				MapScreenMessage( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"%s %s", SquadNames[ pCharacter->bAssignment ].squadname, gsUndergroundString[0] );
			else
				MapScreenMessage( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"%s %s", pLongAssignmentStrings[ pCharacter->bAssignment ], gsUndergroundString[0] );
		}
		return;
	}

	if( ( pCharacter->bAssignment == VEHICLE ) || ( pCharacter->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		SetUpMvtGroupForVehicle( pCharacter );
	}

	// make sure we are at the beginning
	pCharacter->pMercPath = MoveToBeginningOfPathList( pCharacter->pMercPath );

	// will plot a path from current position to sX, sY
	// get last sector in characters list, build new path, remove tail section, move to beginning of list, and append onto old list
	pCharacter->pMercPath = AppendStrategicPath( MoveToBeginningOfPathList ( BuildAStrategicPath( NULL, GetLastSectorIdInCharactersPath( pCharacter ), ( INT16 )( sX + sY*( MAP_WORLD_X ) ), GetSoldierGroupId( pCharacter ), fTacticalTraversal /*, FALSE */ ) ), pCharacter->pMercPath );

	// move to beginning of list
	pCharacter->pMercPath = MoveToBeginningOfPathList( pCharacter->pMercPath );

	// check if in vehicle, if so, copy path to vehicle
	if( ( pCharacter->bAssignment == VEHICLE ) || ( pCharacter->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		MoveCharactersPathToVehicle( pCharacter );
	}
	else
	{
		CopyPathToCharactersSquadIfInOne( pCharacter );
	}
}



void PlotATemporaryPathForCharacter( SOLDIERTYPE *pCharacter, INT16 sX, INT16 sY )
{
	// make sure we're at the beginning
	pTempCharacterPath = MoveToBeginningOfPathList( pTempCharacterPath );

	// clear old temp path
	pTempCharacterPath = ClearStrategicPathList( pTempCharacterPath, -1 );

	// is cursor allowed here?..if not..don't build temp path
	if( !IsTheCursorAllowedToHighLightThisSector( sX, sY ) )
	{
		return;
	}

	// build path
	pTempCharacterPath = BuildAStrategicPath( pTempCharacterPath, GetLastSectorIdInCharactersPath( pCharacter ) , ( INT16 )( sX + sY*( MAP_WORLD_X ) ), GetSoldierGroupId( pCharacter ), FALSE /*, TRUE */ );

	return;
}



// clear out character path list, after and including this sector
UINT32 ClearPathAfterThisSectorForCharacter( SOLDIERTYPE *pCharacter, INT16 sX, INT16 sY )
{
	INT32 iOrigLength = 0;
	VEHICLETYPE *pVehicle = NULL;


	iOrigLength = GetLengthOfMercPath( pCharacter );

	if( !iOrigLength )
	{
		// no previous path, nothing to do
		return( ABORT_PLOTTING );
	}


	// if we're clearing everything beyond the current sector, that's quite different.  Since we're basically cancelling
	// his movement completely, we must also make sure his next X,Y are changed and he officially "returns" to his sector
	if ( ( sX == pCharacter->sSectorX ) && ( sY == pCharacter->sSectorY ) )
	{
		// if we're in confirm map move mode, cancel that (before new UI messages are issued)
		EndConfirmMapMoveMode( );

		CancelPathsOfAllSelectedCharacters();
		return( PATH_CLEARED );
	}
	else	// click not in the current sector
	{
		// if the clicked sector is along current route, this will repath only as far as it.  If not, the entire path will
		// be canceled.

		// if a vehicle
		if( pCharacter->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			pVehicle = &( pVehicleList[ pCharacter->bVehicleID ] );
		}
		// or in a vehicle
		else if( pCharacter->bAssignment == VEHICLE )
		{
			pVehicle = &( pVehicleList[ pCharacter->iVehicleId ] );
		}
		else
		{
			// foot soldier
			pCharacter->pMercPath = ClearStrategicPathListAfterThisSector( pCharacter->pMercPath, sX, sY, pCharacter->ubGroupID );
		}

		// if there's an associated vehicle structure
		if ( pVehicle != NULL )
		{
			// do it for the vehicle, too
			pVehicle->pMercPath = ClearStrategicPathListAfterThisSector( pVehicle->pMercPath, sX, sY, pVehicle->ubMovementGroup );
		}

		if( GetLengthOfMercPath( pCharacter ) < iOrigLength )
		{
			CopyPathToAllSelectedCharacters( pCharacter->pMercPath );
			// path WAS actually shortened
			return( PATH_SHORTENED );
		}
		else
		{
			// same path as before - it's not any shorter
			return ( ABORT_PLOTTING );
		}
	}
}


void CancelPathForCharacter( SOLDIERTYPE *pCharacter )
{
	// clear out character's entire path list, he and his squad will stay/return to his current sector.
	pCharacter->pMercPath = ClearStrategicPathList( pCharacter->pMercPath, pCharacter->ubGroupID );
	// NOTE: This automatically calls RemoveGroupWaypoints() internally for valid movement groups

	// This causes the group to effectively reverse directions (even if they've never actually left), so handle that.
	// They are going to return to their current X,Y sector.
	RebuildWayPointsForGroupPath( pCharacter->pMercPath, pCharacter->ubGroupID );
//	GroupReversingDirectionsBetweenSectors( GetGroup( pCharacter->ubGroupID ), ( UINT8 )( pCharacter->sSectorX ), ( UINT8 )( pCharacter->sSectorY ), FALSE );


	// if he's in a vehicle, clear out the vehicle, too
	if( pCharacter->bAssignment == VEHICLE )
	{
		CancelPathForVehicle( &( pVehicleList[ pCharacter->iVehicleId ] ), TRUE );
	}
	else
	{
		// display "travel route canceled" message
		MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pMapPlotStrings[ 3 ] );
	}


	CopyPathToCharactersSquadIfInOne( pCharacter );

	fMapPanelDirty = TRUE;
	fTeamPanelDirty = TRUE;
	fCharacterInfoPanelDirty = TRUE;		// to update ETA
}


void CancelPathForVehicle( VEHICLETYPE *pVehicle, BOOLEAN fAlreadyReversed )
{
	// we're clearing everything beyond the *current* sector, that's quite different.  Since we're basically cancelling
	// his movement completely, we must also make sure his next X,Y are changed and he officially "returns" to his sector
	pVehicle->pMercPath = ClearStrategicPathList( pVehicle->pMercPath, pVehicle->ubMovementGroup );
	// NOTE: This automatically calls RemoveGroupWaypoints() internally for valid movement groups

	// if we already reversed one of the passengers, flag will be TRUE,
	// don't do it again or we're headed back where we came from!
	if ( !fAlreadyReversed )
	{
		// This causes the group to effectively reverse directions (even if they've never actually left), so handle that.
		// They are going to return to their current X,Y sector.
		RebuildWayPointsForGroupPath( pVehicle->pMercPath, pVehicle->ubMovementGroup );
//		GroupReversingDirectionsBetweenSectors( GetGroup( pVehicle->ubMovementGroup ), ( UINT8 ) ( pVehicle->sSectorX ), ( UINT8 ) ( pVehicle->sSectorY ), FALSE );
	}

	// display "travel route canceled" message
	MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pMapPlotStrings[ 3 ] );

	// turn the helicopter flag off here, this prevents the "route aborted" msg from coming up
	fPlotForHelicopter = FALSE;

	fTeamPanelDirty = TRUE;
	fMapPanelDirty = TRUE;
	fCharacterInfoPanelDirty = TRUE;		// to update ETA
}


void CancelPathForGroup( GROUP *pGroup )
{
	INT32 iVehicleId;

	// if it's the chopper, but player can't redirect it
	if ( pGroup->usGroupTeam == OUR_TEAM && IsGroupTheHelicopterGroup( pGroup ) && (CanHelicopterFly( ) == FALSE) )
	{
		// explain & ignore
		ExplainWhySkyriderCantFly();
		return;
	}

	// is it a non-vehicle group?
	if ( (pGroup->usGroupTeam == OUR_TEAM) && (pGroup->fVehicle == FALSE) )
	{
		if( pGroup->pPlayerList )
		{
			if( pGroup->pPlayerList->pSoldier )
			{
				// clearing one merc should be enough, it copies changes to his squad on its own
				CancelPathForCharacter( pGroup->pPlayerList->pSoldier );
			}
		}
	}
	// is it a vehicle group?
	else if( pGroup->fVehicle )
	{
		iVehicleId = GivenMvtGroupIdFindVehicleId( pGroup->ubGroupID );

		// must be valid!
		Assert( iVehicleId != -1 );
		if ( iVehicleId == -1 )
			return;

		CancelPathForVehicle( &( pVehicleList[ iVehicleId ] ), FALSE );
	}
	else if ( pGroup->usGroupTeam == MILITIA_TEAM )
	{
		CancelPathForMilitiaGroup( pGroup->ubGroupID );
	}
}

void CancelPathForMilitiaGroup( UINT8 uGroupId )
{
	// we're clearing everything beyond the *current* sector, that's quite different.  Since we're basically cancelling
	// his movement completely, we must also make sure his next X,Y are changed and he officially "returns" to his sector
	INT16 militiapathslot = GetMilitiaPathSlot( uGroupId );

	if ( militiapathslot > -1 )
	{
		gMilitiaPath[militiapathslot].path = ClearStrategicPathList( gMilitiaPath[militiapathslot].path, uGroupId );
		// NOTE: This automatically calls RemoveGroupWaypoints() internally for valid movement groups

		// if we already reversed one of the passengers, flag will be TRUE,
		// don't do it again or we're headed back where we came from!
		/*if ( !fAlreadyReversed )
		{
			// This causes the group to effectively reverse directions (even if they've never actually left), so handle that.
			// They are going to return to their current X,Y sector.
			RebuildWayPointsForGroupPath( pVehicle->pMercPath, pVehicle->ubMovementGroup );
			//		GroupReversingDirectionsBetweenSectors( GetGroup( pVehicle->ubMovementGroup ), ( UINT8 ) ( pVehicle->sSectorX ), ( UINT8 ) ( pVehicle->sSectorY ), FALSE );
		}*/

		RebuildWayPointsForGroupPath( gMilitiaPath[militiapathslot].path, uGroupId );

		// remove group's next and prev to make them stay here
		GROUP* pGroup = GetGroup( uGroupId );

		if ( pGroup )
		{
			pGroup->ubNextX = pGroup->ubSectorX;
			pGroup->ubNextY = pGroup->ubSectorY;

			pGroup->ubPrevX = 0;
			pGroup->ubPrevY = 0;
		}
	}

	// display "travel route canceled" message
	MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pMapPlotStrings[3] );

	MilitiaplotFinish();

	fPlotForMilitia = FALSE;

	fTeamPanelDirty = TRUE;
	fMapPanelDirty = TRUE;
	fCharacterInfoPanelDirty = TRUE;		// to update ETA
}


void CopyPathToCharactersSquadIfInOne( SOLDIERTYPE *pCharacter )
{
	INT8 bSquad = 0;

	// check if on a squad, if so, do same thing for all characters

	// what squad is character in?
	bSquad = SquadCharacterIsIn( pCharacter );

	// check to see if character is on a squad, if so, copy path to squad
	if( bSquad != -1)
	{
		// copy path over
		CopyPathOfCharacterToSquad( pCharacter, bSquad );
	}
}



void DisplaySoldierPath( SOLDIERTYPE *pCharacter )
{
	PathStPtr pPath = NULL;

/* ARM: Hopefully no longer required once using GetSoldierMercPathPtr() ???
	// check if in vehicle, if so, copy path to vehicle
	if( ( pCharacter->bAssignment == VEHICLE )||( pCharacter->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		// get the real path from vehicle's structure and copy it into this soldier's
		CopyVehiclePathToSoldier( pCharacter );
	}
*/

	pPath = GetSoldierMercPathPtr( pCharacter );

  // trace real route
	TracePathRoute( FALSE, TRUE, pPath );
	AnimateRoute( pPath );
}


void DisplaySoldierTempPath( SOLDIERTYPE *pCharacter )
{
	// now render temp route
	TracePathRoute( FALSE, TRUE,  pTempCharacterPath );
}



void DisplayHelicopterPath( void )
{
	// move to beginning of path list
	pVehicleList[ iHelicopterVehicleId ].pMercPath = MoveToBeginningOfPathList( pVehicleList[ iHelicopterVehicleId ].pMercPath );

	// clip to map
	ClipBlitsToMapViewRegion( );

	// trace both lists..temp is conditional if cursor has sat in same sector grid long enough
	TracePathRoute( TRUE, TRUE, pVehicleList[ iHelicopterVehicleId ].pMercPath );
	AnimateRoute( pVehicleList[ iHelicopterVehicleId ].pMercPath );

	// restore
	RestoreClipRegionToFullScreen( );
}

void DisplayHelicopterTempPath( void )
{
	//should we draw temp path?
	if( fDrawTempHeliPath )
	{
		TracePathRoute( TRUE, TRUE, pTempHelicopterPath );
	}
}

void DisplayMilitiaPath( void )
{
	// move to beginning of path list
	gMilitiaPath[gMilitiaGroupId].path = MoveToBeginningOfPathList( gMilitiaPath[gMilitiaGroupId].path );

	// clip to map
	ClipBlitsToMapViewRegion( );

	// trace both lists..temp is conditional if cursor has sat in same sector grid long enough
	TracePathRoute( TRUE, TRUE, gMilitiaPath[gMilitiaGroupId].path );
	AnimateRoute( gMilitiaPath[gMilitiaGroupId].path );

	// restore
	RestoreClipRegionToFullScreen( );
}

void DisplayMilitiaTempPath( void )
{
	//should we draw temp path?
	if ( fDrawTempMilitiaPath )
	{
		TracePathRoute( TRUE, TRUE, pTempMilitiaPath );
	}
}

extern BOOLEAN HeliCharacterDialogue( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum );

void PlotPathForHelicopter( INT16 sX, INT16 sY )
{
	SOLDIERTYPE *pSkyRider = NULL;

	// will plot the path for the helicopter

	// no heli...go back
	if( !fShowAircraftFlag || !fHelicopterAvailable )
	{
		return;
	}

	// is cursor allowed here?..if not..don't build path
	if( !IsTheCursorAllowedToHighLightThisSector( sX, sY ) )
	{
		return;
	}

	// set up mvt group for helicopter
	SetUpHelicopterForMovement( );

	//CHRISL: If we've plotted a path through enemy controlled airspace, we have a new Skyrider speech we want to hear.
	if( GetNumUnSafeSectorsInPath( ) > 0 && !gGameSettings.fOptions[TOPTION_SILENT_SKYRIDER] )
	{
		HeliCharacterDialogue( pSkyRider, HELI_PATH_THROUGH_ENEMEY_AIRSPACE );
	}

	// move to beginning of list
	//pHelicopterPath = MoveToBeginningOfPathList( pVehicleList[ iHelicopterVehicleId ].pMercPath );
	MoveToBeginningOfPathList( pVehicleList[ iHelicopterVehicleId ].pMercPath );

	// will plot a path from current position to sX, sY
	// get last sector in helicopters list, build new path, remove tail section, move to beginning of list, and append onto old list
	pVehicleList[ iHelicopterVehicleId ].pMercPath = AppendStrategicPath( MoveToBeginningOfPathList( BuildAStrategicPath( NULL, GetLastSectorOfHelicoptersPath( ), ( INT16 )( sX + sY*( MAP_WORLD_X ) ), pVehicleList[ iHelicopterVehicleId ].ubMovementGroup, FALSE /*, FALSE */ ) ), pVehicleList[ iHelicopterVehicleId ].pMercPath );

	// move to beginning of list
	pVehicleList[ iHelicopterVehicleId ].pMercPath = MoveToBeginningOfPathList( pVehicleList[ iHelicopterVehicleId ].pMercPath );

	fMapPanelDirty = TRUE;
}

void PlotATemporaryPathForHelicopter( INT16 sX, INT16 sY )
{
	// clear old temp path
	pTempHelicopterPath = MoveToBeginningOfPathList( pTempHelicopterPath);
	pTempHelicopterPath = ClearStrategicPathList( pTempHelicopterPath, 0 );

	// is cursor allowed here?..if not..don't build temp path
	if( !IsTheCursorAllowedToHighLightThisSector( sX, sY ) )
	{
		return;
	}

	// build path
	pTempHelicopterPath = BuildAStrategicPath( NULL, GetLastSectorOfHelicoptersPath( ), ( INT16 )( sX + sY*( MAP_WORLD_X ) ), pVehicleList[ iHelicopterVehicleId ].ubMovementGroup, FALSE /*, TRUE */ );
}

void PlotPathForMilitia( INT16 sX, INT16 sY )
{
	//SOLDIERTYPE *pSkyRider = NULL;

	// will plot the path for the helicopter

	// no heli...go back
	if ( !fShowMilitia )
	{
		return;
	}

	// is cursor allowed here?..if not..don't build path
	if ( !IsTheCursorAllowedToHighLightThisSector( sX, sY ) )
	{
		return;
	}

	// preparations for militia path plotting
	SetUpMilitiaForMovement( );

	if ( gMilitiaPath[gMilitiaGroupId].sGroupid < 0 )
		return;

	//CHRISL: If we've plotted a path through enemy controlled airspace, we have a new Skyrider speech we want to hear.
	/*if ( GetNumUnSafeSectorsInPath( ) > 0 && !gGameSettings.fOptions[TOPTION_SILENT_SKYRIDER] )
	{
		HeliCharacterDialogue( pSkyRider, HELI_PATH_THROUGH_ENEMEY_AIRSPACE );
	}*/

	// move to beginning of list
	//pHelicopterPath = MoveToBeginningOfPathList( pVehicleList[ iHelicopterVehicleId ].pMercPath );
	MoveToBeginningOfPathList( gMilitiaPath[gMilitiaGroupId].path );

	// will plot a path from current position to sX, sY
	// get last sector in helicopters list, build new path, remove tail section, move to beginning of list, and append onto old list
	gMilitiaPath[gMilitiaGroupId].path = AppendStrategicPath( MoveToBeginningOfPathList( BuildAStrategicPath( NULL, GetLastSectorOfMilitiaPath( ), (INT16)(sX + sY*(MAP_WORLD_X)), gMilitiaPath[gMilitiaGroupId].sGroupid, TRUE /*, FALSE */ ) ), gMilitiaPath[gMilitiaGroupId].path );

	// move to beginning of list
	gMilitiaPath[gMilitiaGroupId].path = MoveToBeginningOfPathList( gMilitiaPath[gMilitiaGroupId].path );

	fMapPanelDirty = TRUE;
}

void PlotATemporaryPathForMilitia( INT16 sX, INT16 sY )
{
	// clear old temp path
	pTempMilitiaPath = MoveToBeginningOfPathList( pTempMilitiaPath );
	pTempMilitiaPath = ClearStrategicPathList( pTempMilitiaPath, 0 );

	// is cursor allowed here?..if not..don't build temp path
	if ( !IsTheCursorAllowedToHighLightThisSector( sX, sY ) )
	{
		return;
	}

	if ( gMilitiaPath[gMilitiaGroupId].sGroupid < 0 )
		return;

	// build path
	pTempMilitiaPath = BuildAStrategicPath( NULL, GetLastSectorOfMilitiaPath( ), (INT16)(sX + sY*(MAP_WORLD_X)), gMilitiaPath[gMilitiaGroupId].sGroupid, TRUE /*, TRUE */ );
}


// clear out helicopter path list, after and including this sector
UINT32 ClearPathAfterThisSectorForHelicopter( INT16 sX, INT16 sY )
{
	VEHICLETYPE *pVehicle = NULL;
	INT32 iOrigLength = 0;

	// clear out helicopter path list, after and including this sector
	if( iHelicopterVehicleId == -1 ||  !CanHelicopterFly() )
	{
		// abort plotting, shouldn't even be here
		return( ABORT_PLOTTING );
	}

	pVehicle = &( pVehicleList[ iHelicopterVehicleId ] );
	
	iOrigLength = GetLengthOfPath( pVehicle->pMercPath );
	if( !iOrigLength )
	{
		// no previous path, nothing to do, and we didn't shorten it
		return( ABORT_PLOTTING );
	}

	// are we clearing everything beyond the helicopter's CURRENT sector?
	if ( ( sX == pVehicle->sSectorX ) && ( sY == pVehicle->sSectorY ) )
	{
		// if we're in confirm map move mode, cancel that (before new UI messages are issued)
		EndConfirmMapMoveMode( );

		CancelPathForVehicle( pVehicle, FALSE );
		return( PATH_CLEARED );
	}
	else	// click not in the current sector
	{
		// if the clicked sector is along current route, this will repath only as far as it.  If not, the entire path will
		// be canceled.
		pVehicle->pMercPath = ClearStrategicPathListAfterThisSector( pVehicle->pMercPath, sX, sY, pVehicle->ubMovementGroup );

		if( GetLengthOfPath( pVehicle->pMercPath ) < iOrigLength )
		{
			// really shortened!
			return( PATH_SHORTENED );
		}
		else
		{
			// same path as before - it's not any shorter
			return ( ABORT_PLOTTING );
		}
	}
}

// clear out militia path list, after and including this sector
UINT32 ClearPathAfterThisSectorForMilitia( INT16 sX, INT16 sY )
{
	//VEHICLETYPE *pVehicle = NULL;
	INT32 iOrigLength = 0;

	if ( gMilitiaPath[gMilitiaGroupId].sGroupid < 0 )
		return(ABORT_PLOTTING);

	// clear out helicopter path list, after and including this sector
	/*if ( iHelicopterVehicleId == -1 || !CanHelicopterFly( ) )
	{
		// abort plotting, shouldn't even be here
		return(ABORT_PLOTTING);
	}

	pVehicle = &(pVehicleList[iHelicopterVehicleId]);*/

	iOrigLength = GetLengthOfPath( gMilitiaPath[gMilitiaGroupId].path );
	if ( !iOrigLength )
	{
		// no previous path, nothing to do, and we didn't shorten it
		return(ABORT_PLOTTING);
	}

	GROUP* pGroup = GetGroup( (UINT8)gMilitiaPath[gMilitiaGroupId].sGroupid );

	// are we clearing everything beyond the helicopter's CURRENT sector?
	if ( pGroup && (sX == pGroup->ubSectorX) && (sY == pGroup->ubSectorY) )
	{
		// if we're in confirm map move mode, cancel that (before new UI messages are issued)
		EndConfirmMapMoveMode( );

		CancelPathForGroup( pGroup );
		return(PATH_CLEARED);
	}
	else	// click not in the current sector
	{
		// if the clicked sector is along current route, this will repath only as far as it.  If not, the entire path will
		// be canceled.
		gMilitiaPath[gMilitiaGroupId].path = ClearStrategicPathListAfterThisSector( gMilitiaPath[gMilitiaGroupId].path, sX, sY, gMilitiaPath[gMilitiaGroupId].sGroupid );

		if ( GetLengthOfPath( gMilitiaPath[gMilitiaGroupId].path ) < iOrigLength )
		{
			// really shortened!
			return(PATH_SHORTENED);
		}
		else
		{
			// same path as before - it's not any shorter
			return (ABORT_PLOTTING);
		}
	}
}


INT16 GetLastSectorOfHelicoptersPath( void )
{
	// will return the last sector of the helicopter's current path
	INT16 sLastSector = pVehicleList[ iHelicopterVehicleId ].sSectorX + pVehicleList[ iHelicopterVehicleId ].sSectorY * MAP_WORLD_X ;
	PathStPtr pNode = NULL;

	pNode = pVehicleList[ iHelicopterVehicleId ].pMercPath;

	while( pNode )
	{
		sLastSector = ( INT16 ) ( pNode->uiSectorId );
		pNode = pNode->pNext;
	}

	return sLastSector;
}

INT16 GetLastSectorOfMilitiaPath( void )
{
	// will return the last sector of the helicopter's current path
	INT16 sLastSector = gMilitiaPlotStartSector;//pVehicleList[iHelicopterVehicleId].sSectorX + pVehicleList[iHelicopterVehicleId].sSectorY * MAP_WORLD_X;
	PathStPtr pNode = NULL;

	pNode = gMilitiaPath[gMilitiaGroupId].path;

	while ( pNode )
	{
		sLastSector = (INT16)(pNode->uiSectorId);
		pNode = pNode->pNext;
	}

	return sLastSector;
}


BOOLEAN TracePathRoute(BOOLEAN fCheckFlag, BOOLEAN fForceUpDate, PathStPtr pPath )
{
	BOOLEAN fSpeedFlag=FALSE;
	INT32 iArrow=-1;
	INT32 iX, iY;
	INT32 iArrowX, iArrowY;
	INT32 iDeltaA, iDeltaB, iDeltaB1;
	INT32 iDirection = 0;
	BOOLEAN fUTurnFlag=FALSE;
	PathStPtr pNode=NULL;
	PathStPtr pPastNode=NULL;
	PathStPtr pNextNode=NULL;
	HVOBJECT hMapHandle;

	if ( pPath==NULL )
	{
		return FALSE;
	}

	while( pPath->pPrev )
	{
		pPath = pPath->pPrev;
	}

	pNode = pPath;

	iDirection=-1;
	
	if (pNode->pNext)
		pNextNode=pNode->pNext;
	else
		pNextNode=NULL;
	
	if (pNode->pPrev)
		pPastNode=pNode->pPrev;
	else
		pPastNode=NULL;

	GetVideoObject(&hMapHandle, guiMAPCURSORS);
	
	// go through characters list and display arrows for path
	while(pNode)
	{
		fUTurnFlag=FALSE;
		if ((pPastNode)&&(pNextNode))
		{
			iDeltaA=(INT16)pNode->uiSectorId-(INT16)pPastNode->uiSectorId;
			iDeltaB=(INT16)pNode->uiSectorId-(INT16)pNextNode->uiSectorId;
			if (iDeltaA ==0)
				return( FALSE );
	 
			if(pNode->fSpeed)
				fSpeedFlag=FALSE;
			else
				fSpeedFlag=TRUE;
			
			iX=(pNode->uiSectorId%MAP_WORLD_X);
			iY=(pNode->uiSectorId/MAP_WORLD_X);
			iX=(iX*MAP_GRID_X)+MAP_VIEW_START_X;
			iY=(iY*MAP_GRID_Y)+MAP_VIEW_START_Y;

			iArrowX=iX;
			iArrowY=iY;

			if ((pPastNode->pPrev)&&(pNextNode->pNext))
			{
				fUTurnFlag=FALSE;
				// check to see if out-of sector U-turn
				// for placement of arrows
				iDeltaB1=pNextNode->uiSectorId-pNextNode->pNext->uiSectorId;
				if ((iDeltaB1==-WORLD_MAP_X)&&(iDeltaA==-WORLD_MAP_X)&&(iDeltaB==-1))
				{
					fUTurnFlag=TRUE;
				}
				else if((iDeltaB1==-WORLD_MAP_X)&&(iDeltaA==-WORLD_MAP_X)&&(iDeltaB==1))
				{
					fUTurnFlag=TRUE;
				}
				else if((iDeltaB1==WORLD_MAP_X)&&(iDeltaA==WORLD_MAP_X)&&(iDeltaB==1))
				{
					fUTurnFlag=TRUE;
				}
				else if((iDeltaB1==-WORLD_MAP_X)&&(iDeltaA==-WORLD_MAP_X)&&(iDeltaB==1))
				{
					fUTurnFlag=TRUE;
				}
				else if((iDeltaB1==-1)&&(iDeltaA==-1)&&(iDeltaB==-WORLD_MAP_X))
				{
					fUTurnFlag=TRUE;
				}
				else if((iDeltaB1==-1)&&(iDeltaA==-1)&&(iDeltaB==WORLD_MAP_X))
				{
					fUTurnFlag=TRUE;
				}
				else if((iDeltaB1==1)&&(iDeltaA==1)&&(iDeltaB==-WORLD_MAP_X))
				{
					fUTurnFlag=TRUE;
				}
				else if((iDeltaB1==1)&&(iDeltaA==1)&&(iDeltaB==WORLD_MAP_X))
				{
					fUTurnFlag=TRUE;
				}
				else
					fUTurnFlag=FALSE;
			}

			if ((pPastNode->uiSectorId==pNextNode->uiSectorId))
			{
				if (pPastNode->uiSectorId+WORLD_MAP_X==pNode->uiSectorId)
				{
					if(!(pNode->fSpeed))
						fSpeedFlag=TRUE;
					else
						fSpeedFlag=FALSE;

					iDirection=S_TO_N_LINE;
					if(fSpeedFlag)
						iArrow=Y_NORTH_ARROW;
					else
						iArrow=NORTH_ARROW;
					iArrowX+=NORTH_OFFSET_X;
					iArrowY+=NORTH_OFFSET_Y;
				}
				else if(pPastNode->uiSectorId-WORLD_MAP_X==pNode->uiSectorId)
				{
					iDirection=N_TO_S_LINE;
					if(fSpeedFlag)
						iArrow=Y_SOUTH_ARROW;
					else
						iArrow=SOUTH_ARROW;
					iArrowX+=SOUTH_OFFSET_X;
					iArrowY+=SOUTH_OFFSET_Y;
				}
				else if (pPastNode->uiSectorId+1==pNode->uiSectorId)
				{
					iDirection=E_TO_W_LINE;
					if(fSpeedFlag)
						iArrow=Y_WEST_ARROW;
					else
						iArrow=WEST_ARROW;
					iArrowX+=WEST_OFFSET_X;
					iArrowY+=WEST_OFFSET_Y;
				}
				else
				{
					iDirection=W_TO_E_LINE;
					if(fSpeedFlag)
						iArrow=Y_EAST_ARROW;
					else
						iArrow=EAST_ARROW;
					iArrowX+=EAST_OFFSET_X;
					iArrowY+=EAST_OFFSET_Y;
				}
			}
			else
			{
				if ((iDeltaA==-1)&&(iDeltaB==1))
				{
/*
					if( pPastNode == NULL )
					{
						fSpeedFlag = !fSpeedFlag;
					}

*/
					iDirection=WEST_LINE;
					if(fSpeedFlag)
						iArrow=Y_WEST_ARROW;
					else
						iArrow=WEST_ARROW;
					iArrowX+=WEST_OFFSET_X;
					iArrowY+=WEST_OFFSET_Y;
				}
				else if((iDeltaA==1)&&(iDeltaB==-1))
				{
					iDirection=EAST_LINE;
					if(fSpeedFlag)
						iArrow=Y_EAST_ARROW;
					else
						iArrow=EAST_ARROW;
					iArrowX+=EAST_OFFSET_X;
					iArrowY+=EAST_OFFSET_Y;
				}
				else if((iDeltaA==-WORLD_MAP_X)&&(iDeltaB==WORLD_MAP_X))
				{
					iDirection=NORTH_LINE;
					if(fSpeedFlag)
						iArrow=Y_NORTH_ARROW;
					else
						iArrow=NORTH_ARROW;
					iArrowX+=NORTH_OFFSET_X;
					iArrowY+=NORTH_OFFSET_Y;
				}
				else if((iDeltaA==WORLD_MAP_X)&&(iDeltaB==-WORLD_MAP_X))
				{
					iDirection=SOUTH_LINE;
					if(fSpeedFlag)
						iArrow=Y_SOUTH_ARROW;
					else
						iArrow=SOUTH_ARROW;
					iArrowX+=SOUTH_OFFSET_X;
					iArrowY+=SOUTH_OFFSET_Y;
				}
				else if((iDeltaA==-WORLD_MAP_X)&&(iDeltaB==-1))
				{
					iDirection=N_TO_E_LINE;
					if(fSpeedFlag)
						iArrow=Y_EAST_ARROW;
					else
						iArrow=EAST_ARROW;
					iArrowX+=EAST_OFFSET_X;
					iArrowY+=EAST_OFFSET_Y;
				}
				else if((iDeltaA==WORLD_MAP_X)&&(iDeltaB==1))
				{
					iDirection=S_TO_W_LINE;
					if(fSpeedFlag)
						iArrow=Y_WEST_ARROW;
					else
						iArrow=WEST_ARROW;
					iArrowX+=WEST_OFFSET_X;
					iArrowY+=WEST_OFFSET_Y;
				}
				else if((iDeltaA==1)&&(iDeltaB==-WORLD_MAP_X))
				{
					iDirection=E_TO_S_LINE;
					if(fSpeedFlag)
						iArrow=Y_SOUTH_ARROW;
					else
						iArrow=SOUTH_ARROW;
					iArrowX+=SOUTH_OFFSET_X;
					iArrowY+=SOUTH_OFFSET_Y;
				}
				else if ((iDeltaA==-1)&&(iDeltaB==WORLD_MAP_X))
				{
					iDirection=W_TO_N_LINE;
					if(fSpeedFlag)
						iArrow=Y_NORTH_ARROW;
					else
						iArrow=NORTH_ARROW;
					iArrowX+=NORTH_OFFSET_X;
					iArrowY+=NORTH_OFFSET_Y;
				}
				else if ((iDeltaA==-1)&&(iDeltaB==-WORLD_MAP_X))
				{
					iDirection=W_TO_S_LINE;
					if(fSpeedFlag)
						iArrow=Y_SOUTH_ARROW;
					else
						iArrow=SOUTH_ARROW;
					iArrowX+=SOUTH_OFFSET_X;
					iArrowY+=(SOUTH_OFFSET_Y+WEST_TO_SOUTH_OFFSET_Y);
				}
				else if ((iDeltaA==-WORLD_MAP_X)&&(iDeltaB==1))
				{
					iDirection=N_TO_W_LINE;
					if(fSpeedFlag)
						iArrow=Y_WEST_ARROW;
					else
						iArrow=WEST_ARROW;
					iArrowX+=WEST_OFFSET_X;
					iArrowY+=WEST_OFFSET_Y;
				}
				else if ((iDeltaA==WORLD_MAP_X)&&(iDeltaB==-1))
				{
					iDirection=S_TO_E_LINE;
					if(fSpeedFlag)
						iArrow=Y_EAST_ARROW;
					else
						iArrow=EAST_ARROW;
					iArrowX+=EAST_OFFSET_X;
					iArrowY+=EAST_OFFSET_Y;
				}
				else if ((iDeltaA==1)&&(iDeltaB==WORLD_MAP_X))
				{
					iDirection=E_TO_N_LINE;
					if(fSpeedFlag)
						iArrow=Y_NORTH_ARROW;
					else
						iArrow=NORTH_ARROW;
					iArrowX+=NORTH_OFFSET_X;
					iArrowY+=NORTH_OFFSET_Y+EAST_TO_NORTH_OFFSET_Y;
				}
			}
		}
		else
		{
			iX=(pNode->uiSectorId%MAP_WORLD_X);
			iY=(pNode->uiSectorId/MAP_WORLD_X);
			iX=(iX*MAP_GRID_X)+MAP_VIEW_START_X;
			iY=(iY*MAP_GRID_Y)+MAP_VIEW_START_Y;

			iArrowX=iX;
			iArrowY=iY;
	  
			if((pNode->fSpeed))
				fSpeedFlag=FALSE;
			else
				fSpeedFlag=TRUE;
			
			// display enter and exit 'X's
			if (pPastNode)
			{
  				fUTurnFlag=TRUE;
				iDeltaA=(INT16)pNode->uiSectorId-(INT16)pPastNode->uiSectorId;
				if (iDeltaA==-1)
				{
					iDirection=RED_X_WEST;
					//iX+=RED_WEST_OFF_X;
				}
				else if (iDeltaA==1)
				{
					iDirection=RED_X_EAST;
					//iX+=RED_EAST_OFF_X;
				}
				else if(iDeltaA==-WORLD_MAP_X)
				{
					iDirection=RED_X_NORTH;
					//iY+=RED_NORTH_OFF_Y;
				}
				else
				{
					iDirection=RED_X_SOUTH;
				//	iY+=RED_SOUTH_OFF_Y;
				}
			}
			if (pNextNode)
			{
				fUTurnFlag=FALSE;
				iDeltaB=(INT16)pNode->uiSectorId-(INT16)pNextNode->uiSectorId;
				if((pNode->fSpeed))
					fSpeedFlag=FALSE;
				else
					fSpeedFlag=TRUE;

				if (iDeltaB==-1)
				{
					iDirection=GREEN_X_EAST;
					if(fSpeedFlag)
						iArrow=Y_EAST_ARROW;
					else
						iArrow=EAST_ARROW;
					iArrowX+=EAST_OFFSET_X;
					iArrowY+=EAST_OFFSET_Y;

					//iX+=RED_EAST_OFF_X;
				}
				else if (iDeltaB==1)
				{
					iDirection=GREEN_X_WEST;
					if(fSpeedFlag)
						iArrow=Y_WEST_ARROW;
					else
						iArrow=WEST_ARROW;
					iArrowX+=WEST_OFFSET_X;
					iArrowY+=WEST_OFFSET_Y;

					//iX+=RED_WEST_OFF_X;
				}
				else if(iDeltaB==WORLD_MAP_X)
				{
					iDirection=GREEN_X_NORTH;
					if(fSpeedFlag)
						iArrow=Y_NORTH_ARROW;
					else
						iArrow=NORTH_ARROW;
					iArrowX+=NORTH_OFFSET_X;
					iArrowY+=NORTH_OFFSET_Y;

					//iY+=RED_NORTH_OFF_Y;
				}
				else
				{
					iDirection=GREEN_X_SOUTH;
					if(fSpeedFlag)
						iArrow=Y_SOUTH_ARROW;
					else
						iArrow=SOUTH_ARROW;
					iArrowX+=SOUTH_OFFSET_X;
					iArrowY+=SOUTH_OFFSET_Y;

					//iY+=RED_SOUTH_OFF_Y;
				}
			}
		}

		if ((iDirection !=-1))
		{
			BltVideoObject(FRAME_BUFFER, hMapHandle, (UINT16)iDirection, iX,iY, VO_BLT_SRCTRANSPARENCY, NULL );

			if(!fUTurnFlag)
			{
				BltVideoObject(FRAME_BUFFER, hMapHandle, (UINT16)iArrow, iArrowX, iArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
				InvalidateRegion( iArrowX, iArrowY, iArrowX + 2 * MAP_GRID_X, iArrowY + 2 * MAP_GRID_Y );
			}

			InvalidateRegion( iX, iY, iX + 2 * MAP_GRID_X, iY + 2 * MAP_GRID_Y );

			fUTurnFlag=FALSE;
		}
		// check to see if there is a turn


		pPastNode=pNode;
		pNode=pNode->pNext;

		if(!pNode)
			return( FALSE );

		if (pNode->pNext)
			pNextNode=pNode->pNext;
		else
			pNextNode=NULL;
		}

		return ( TRUE );
}


void AnimateRoute( PathStPtr pPath )
{
	// set buffer
	SetFontDestBuffer( FRAME_BUFFER, 0,0,SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	// the animated path
	if( TraceCharAnimatedRoute( pPath, FALSE, FALSE ))
	{
		// ARM? Huh?  Why the same thing twice more?
		TraceCharAnimatedRoute( pPath, FALSE, TRUE );
		TraceCharAnimatedRoute( pPath, FALSE, TRUE );
	}
}


BOOLEAN TraceCharAnimatedRoute( PathStPtr pPath, BOOLEAN fCheckFlag, BOOLEAN fForceUpDate )
{
	static PathStPtr pCurrentNode=NULL;
	static BOOLEAN fPauseFlag=TRUE;

	HVOBJECT hMapHandle;
	INT32 iDifference=0;
	INT32 iArrow=-1;
	INT32 iX = 0, iY = 0;
	INT32 iArrowX, iArrowY;
	INT32 iDeltaNext, iDeltaPrev;
	INT32 iDirectionArriving, iDirectionExiting;
	UINT32 uiArrowNumToDraw;
	UINT16 usArrowWidth;
	UINT16 usArrowHeight;
	PathStPtr pTempNode=NULL;
	PathStPtr pNode=NULL;
	PathStPtr pPastNode=NULL;
	PathStPtr pNextNode=NULL;

	// must be plotting movement
	if ( (bSelectedDestChar == -1) && !fPlotForHelicopter && !fPlotForMilitia )
	{
		return FALSE;
	}

	// if any nodes have been deleted, reset current node to beginning of the list
	if( fDeletedNode )
	{
		fDeletedNode = FALSE;
		pCurrentNode = NULL;
	}

	// Valid path?
	if ( pPath == NULL )
	{
		return FALSE;
	}
	else
	{
		if(pCurrentNode == NULL)
		{
			pCurrentNode = pPath;
		}
	}

	// Check Timer
	if (giAnimateRouteBaseTime==0)
	{
		giAnimateRouteBaseTime=GetJA2Clock();
		return FALSE;
	}

	// check difference in time
	iDifference=GetJA2Clock()-giAnimateRouteBaseTime;

	// if pause flag, check time, if time passed, reset, continue on, else return
	if(fPauseFlag)
	{
		if(iDifference < PAUSE_DELAY)
		{
			return FALSE;
		}
		else
		{
			fPauseFlag=FALSE;
			giAnimateRouteBaseTime=GetJA2Clock();
		}
	}

	// If we aren't being forced to update the arrows...
	if(!fForceUpDate)
	{
		// If we're ready to draw the next arrow,
		if(iDifference >= ARROW_DELAY)
		{
			// sufficient time has passed, update base time
			giAnimateRouteBaseTime=GetJA2Clock();
			pCurrentNode = pCurrentNode->pNext;

			if (pCurrentNode == NULL)
			{
				fPauseFlag = TRUE;
				return FALSE;
			}

			// Are we just checking?
			if(fCheckFlag)
			{
				// Then return true to signal that we're ready to draw the next arrow.
				return TRUE;
			}
		}
	}

	// check to see if Current node has not been deleted

	// Clone the first node in the path
	pTempNode = pPath;

	// Scan through this cloned path
	while(pTempNode)
	{
		// Have we found our node?
		if(pTempNode==pCurrentNode)
		{
			//Good, it hasn't been deleted
			break;
		}
		else
		{
			// Continue scanning
			pTempNode=pTempNode->pNext;
		}
	}

	// if deleted, restart at beginning
	if(pTempNode==NULL)
	{
		pCurrentNode = pPath; // First node in the path

		// No path?
		if(!pCurrentNode)
		{
			// FALSE!
			return FALSE;
		}
	}

	// Get the X/Y of the top left corner of this sector.
	iX = (pCurrentNode->uiSectorId % MAP_WORLD_X);
	iY = (pCurrentNode->uiSectorId / MAP_WORLD_X);

	iX = (iX*MAP_GRID_X) + MAP_VIEW_START_X;
	iY = (iY*MAP_GRID_Y) + MAP_VIEW_START_Y;

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		iArrowX = iX - 4;
		iArrowY = iY - 4;
	}
	else if (iResolution < _1024x768)
	{
		iArrowX = iX - 5;
		iArrowY = iY - 5;
	}
	else 
	{
		iArrowX = iX - 6;
		iArrowY = iY - 6;
	}

	// Find the next node
	if (pCurrentNode->pNext)
	{
		pNextNode=pCurrentNode->pNext;
		iDeltaNext=(INT16)pNextNode->uiSectorId - (INT16)pCurrentNode->uiSectorId;
	}
	else
	{
		pNextNode=NULL;
		iDeltaNext = 0;
	}

	// Find the previous node.
	if (pCurrentNode->pPrev)
	{
		pPastNode=pCurrentNode->pPrev;
		iDeltaPrev=(INT16)pPastNode->uiSectorId - (INT16)pCurrentNode->uiSectorId;
	}
	else
	{
		pPastNode=NULL;
		iDeltaPrev = 0;
	}

	// Figure out direction of movement
	switch (iDeltaPrev)
	{
		case -MAP_WORLD_Y:
			iDirectionArriving = 1;
			break;
		case 1:
			iDirectionArriving = 2;
			break;
		case MAP_WORLD_Y:
			iDirectionArriving = 3;
			break;
		case -1:
			iDirectionArriving = 4;
			break;
		default:
			iDirectionArriving = 0;
			break;
	}

	switch (iDeltaNext)
	{
		case -MAP_WORLD_Y:
			iDirectionExiting = 0;
			break;
		case 1:
			iDirectionExiting = 1;
			break;
		case MAP_WORLD_Y:
			iDirectionExiting = 2;
			break;
		case -1:
			iDirectionExiting = 3;
			break;
		default:
			return (FALSE);
	}

	if (iDirectionArriving == 0)
	{
		if (pPastNode)
		{
			if (pPastNode->pPrev)
			{
				INT8 A = 1;
			}
		}
	}

	uiArrowNumToDraw = 20 + (iDirectionArriving * 4) + iDirectionExiting;

	// Get the video object containing all movement arrows.
	GetVideoObject(&hMapHandle, guiMapPathingArrows);
	usArrowWidth = hMapHandle->pETRLEObject[ uiArrowNumToDraw ].usWidth;
	usArrowHeight = hMapHandle->pETRLEObject[ uiArrowNumToDraw ].usHeight;

	BltVideoObject(FRAME_BUFFER, hMapHandle, uiArrowNumToDraw, iArrowX, iArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
	InvalidateRegion( iArrowX + xResOffset, iArrowY + yResOffset, iArrowX + usArrowWidth, iArrowY + usArrowHeight );

	//ARM who knows what it should return here?
	return FALSE;
}


void DisplayThePotentialPathForHelicopter(INT16 sMapX, INT16 sMapY )
{
	// simply check if we want to refresh the screen to display path
	static BOOLEAN fOldShowAirCraft = FALSE;
	static INT16  sOldMapX, sOldMapY;
	INT32 iDifference = 0;

	if( fOldShowAirCraft != fShowAircraftFlag )
	{
		fOldShowAirCraft = fShowAircraftFlag;
		giPotHeliPathBaseTime = GetJA2Clock( );

		sOldMapX = sMapX;
		sOldMapY = sMapY;
		fTempPathAlreadyDrawn = FALSE;
		fDrawTempHeliPath = FALSE;
	}

	if( ( sMapX != sOldMapX) || ( sMapY != sOldMapY ) )
	{
		giPotHeliPathBaseTime = GetJA2Clock( );

		sOldMapX = sMapX;
		sOldMapY = sMapY;

		// path was plotted and we moved, re draw map..to clean up mess
		if( fTempPathAlreadyDrawn )
		{
			fMapPanelDirty = TRUE;
		}

		fTempPathAlreadyDrawn = FALSE;
		fDrawTempHeliPath = FALSE;
	}

	iDifference = GetJA2Clock( ) - giPotHeliPathBaseTime ;

	if( fTempPathAlreadyDrawn )
	{
		return;
	}

	if( iDifference > MIN_WAIT_TIME_FOR_TEMP_PATH )
	{
		fDrawTempHeliPath = TRUE;
		giPotHeliPathBaseTime = GetJA2Clock( );
		fTempPathAlreadyDrawn = TRUE;
	}
}

void DisplayThePotentialPathForMilitia( INT16 sMapX, INT16 sMapY )
{
	// simply check if we want to refresh the screen to display path
	static BOOLEAN fOldShow = FALSE;
	static INT16  sOldMapX, sOldMapY;
	INT32 iDifference = 0;

	if ( fOldShow != fShowMilitia )
	{
		fOldShow = fShowMilitia;
		giPotMilitiaPathBaseTime = GetJA2Clock( );

		sOldMapX = sMapX;
		sOldMapY = sMapY;
		fTempPathAlreadyDrawn = FALSE;
		fDrawTempMilitiaPath = FALSE;
	}

	if ( (sMapX != sOldMapX) || (sMapY != sOldMapY) )
	{
		giPotMilitiaPathBaseTime = GetJA2Clock( );

		sOldMapX = sMapX;
		sOldMapY = sMapY;

		// path was plotted and we moved, re draw map..to clean up mess
		if ( fTempPathAlreadyDrawn )
		{
			fMapPanelDirty = TRUE;
		}

		fTempPathAlreadyDrawn = FALSE;
		fDrawTempMilitiaPath = FALSE;
	}

	iDifference = GetJA2Clock( ) - giPotMilitiaPathBaseTime;

	if ( fTempPathAlreadyDrawn )
	{
		return;
	}

	//if ( iDifference > MIN_WAIT_TIME_FOR_TEMP_PATH )
	{
		fDrawTempMilitiaPath = TRUE;
		giPotMilitiaPathBaseTime = GetJA2Clock( );
		fTempPathAlreadyDrawn = TRUE;
	}
}


BOOLEAN IsTheCursorAllowedToHighLightThisSector( INT16 sSectorX, INT16 sSectorY )
{
	// check to see if this sector is a blocked out sector?
	if( sBadSectorsList[ sSectorX ][ sSectorY ] )
		return  ( FALSE );

	// return cursor is allowed to highlight this sector
	return (TRUE);
}


void RestoreBackgroundForMapGrid( INT16 sMapX, INT16 sMapY )
{
	// screen values
	INT16 sX = (sMapX * MAP_GRID_X) + MAP_VIEW_START_X;
	INT16 sY = (sMapY * MAP_GRID_Y) + MAP_VIEW_START_Y;

	// restore background
	RestoreExternBackgroundRect( sX, sY ,DMAP_GRID_X ,DMAP_GRID_Y );
}


void ClipBlitsToMapViewRegion( void )
{
	// the standard mapscreen rectangle doesn't work for clipping while zoomed...
	SGPRect ZoomedMapScreenClipRect={	MAP_VIEW_START_X + MAP_GRID_X, 
										MAP_VIEW_START_Y + MAP_GRID_Y - 1, 
										MAP_VIEW_START_X + MAP_VIEW_WIDTH + MAP_GRID_X, 
										MAP_VIEW_START_Y + MAP_VIEW_HEIGHT + MAP_GRID_Y - 10 };
	SGPRect *pRectToUse;

	pRectToUse = &MapScreenRect;

	SetClippingRect( pRectToUse );
	memcpy( &gOldClipRect, &gDirtyClipRect, sizeof( gOldClipRect ) );
	memcpy( &gDirtyClipRect, pRectToUse, sizeof( gDirtyClipRect ) );
}

void RestoreClipRegionToFullScreen( void )
{
	FullScreenRect.iLeft	= 0;
	FullScreenRect.iTop		= 0;
	FullScreenRect.iRight	= SCREEN_WIDTH;
	FullScreenRect.iBottom  = SCREEN_HEIGHT;

	SetClippingRect( &FullScreenRect );
	memcpy( &gDirtyClipRect, &gOldClipRect, sizeof( gDirtyClipRect ) );
}


void ClipBlitsToMapViewRegionForRectangleAndABit( UINT32 uiDestPitchBYTES )
{
	// clip blits to map view region
	// because MC's map coordinates system is so screwy, these had to be hand-tuned to work right...  ARM
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, MapScreenRect.iLeft - 1, MapScreenRect.iTop - 1, MapScreenRect.iRight - MapScreenRect.iLeft + 3, MapScreenRect.iBottom - MapScreenRect.iTop + 2 );
}

void RestoreClipRegionToFullScreenForRectangle( UINT32 uiDestPitchBYTES )
{
	// clip blits to map view region
	//SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, 640, 480 );

	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
}


// mvt icon offset defines
#define SOUTH_Y_MVT_OFFSET +10
#define SOUTH_X_MVT_OFFSET 0
#define NORTH_Y_MVT_OFFSET -10
#define NORTH_X_MVT_OFFSET  +10
#define NORTH_SOUTH_CENTER_OFFSET +5


#define SOUTH_Y_MVT_OFFSET_ZOOM +30
#define SOUTH_X_MVT_OFFSET_ZOOM +5
#define NORTH_Y_MVT_OFFSET_ZOOM -8
#define NORTH_X_MVT_OFFSET_ZOOM +25
#define NORTH_SOUTH_CENTER_OFFSET_ZOOM +15

#define EAST_Y_MVT_OFFSET + 8
#define EAST_X_MVT_OFFSET 15
#define WEST_Y_MVT_OFFSET -2
#define WEST_X_MVT_OFFSET -8
#define EAST_WEST_CENTER_OFFSET +2

#define EAST_Y_MVT_OFFSET_ZOOM +24
#define EAST_X_MVT_OFFSET_ZOOM +36
#define WEST_Y_MVT_OFFSET_ZOOM  +5
#define WEST_X_MVT_OFFSET_ZOOM -10
#define EAST_WEST_CENTER_OFFSET_ZOOM +12

#define NORTH_TEXT_X_OFFSET +1
#define NORTH_TEXT_Y_OFFSET +4
#define SOUTH_TEXT_X_OFFSET +1
#define SOUTH_TEXT_Y_OFFSET +2

#define EAST_TEXT_X_OFFSET + 2
#define EAST_TEXT_Y_OFFSET 0
#define WEST_TEXT_X_OFFSET + 4
#define WEST_TEXT_Y_OFFSET 0


#define ICON_WIDTH 8



void ShowPeopleInMotion( INT16 sX, INT16 sY )
{
	INT32 sExiting = 0;
	INT32 sEntering = 0;
	INT16 sDest = 0;
	INT16 sSource = 0;
	INT16 sOffsetX = 0, sOffsetY = 0;
	INT16 iX = sX, iY = sY;
	INT32 iDirection = 0;
	HVOBJECT hIconHandle;
	BOOLEAN fAboutToEnter = FALSE;
	INT32 iWidth = 0, iHeight = 0;
	INT32 iDeltaXForError = 0, iDeltaYForError = 0;
	UINT16 usArrowWidth;
	UINT16 usArrowHeight;

	INT32 MAP_MVT_ICON_FONT;
	if (iResolution < _800x600 )
	{
		MAP_MVT_ICON_FONT = TINYFONT1;
	}
	else
	{
		MAP_MVT_ICON_FONT = FONT10ARIAL;
	}

	if( iCurrentMapSectorZ != 0 )
	{
		return;
	}

	// show the icons for people in motion from this sector to the next guy over
	for( iDirection = 0; iDirection < 4; iDirection++ )
	{
		// find how many people are coming and going in this sector
		sExiting = 0;
		sEntering = 0;
		sSource = CALCULATE_STRATEGIC_INDEX( sX, sY );
		sOffsetX = 0;
		sOffsetY = 0;
		iX = sX;
		iY = sY;

		// reset fact about to enter
		fAboutToEnter = FALSE;

		sDest = sSource;

		if( ( iDirection == 0 ) && sY > MINIMUM_VALID_Y_COORDINATE )
		{
			sDest += NORTH_MOVE;
		}
		else if( ( iDirection == 1 ) && ( sX < MAXIMUM_VALID_X_COORDINATE ) )
		{
			sDest += EAST_MOVE;
		}
		else if( ( iDirection == 2 ) && ( sY < MAXIMUM_VALID_Y_COORDINATE ) )
		{
			sDest += SOUTH_MOVE;
		}
		else if( ( iDirection == 3 ) && ( sX > MINIMUM_VALID_X_COORDINATE ) )
		{
			sDest += WEST_MOVE;
		}

		// if not at edge of map
		if ( sDest != sSource )
		{
			int tXS = sSource % MAP_WORLD_X;
			int tYS = sSource / MAP_WORLD_X;
			int tXD = sDest % MAP_WORLD_X;
			int tYD = sDest / MAP_WORLD_X;

			// Find sector's top-left pixel on the screen.
			INT16 sSectorPosX = MAP_VIEW_START_X + (sX * MAP_GRID_X);
			INT16 sSectorPosY = MAP_VIEW_START_Y + (sY * MAP_GRID_Y);

			// This is the sectorID in plain 0-256. Most of the functions will use this instead of sSource and sDest.
			INT16 sSourceSector = SECTOR( tXS, tYS );
			INT16 sDestSector = SECTOR( tXD, tYD );

			// HEADROCK HAM 5: We are no longer running a subfunction (PlayersBetweenTheseSectors())
			// to determine whether mercs are moving between these sectors. Instead, we'll go through
			// each group, and if it is moving out of the currently-checked sector, we'll draw an arrow
			// for it.
			//
			// Each arrow denotes one group, and its color and position will reflect how close the group
			// is to entering its target sector.
			//
			// Additionally we also check for groups traveling in the other direction, as we'll want to
			// offset the arrow appropriately.

			// Current group being checked.
			GROUP *curr;
			// Sector of current battle
			INT16 sBattleSector = -1;
			BOOLEAN fMayRetreatFromBattle = FALSE;
			BOOLEAN fRetreatingFromBattle = FALSE;
			BOOLEAN fHelicopterGroup = FALSE;
			BOOLEAN fEntering = FALSE;
			BOOLEAN fDrawOnlyOne = FALSE;

			// Find any battle raging at the moment.
			if( gpBattleGroup )
			{
				// Get its sector
				sBattleSector = (INT16)SECTOR( gpBattleGroup->ubSectorX, gpBattleGroup->ubSectorY );
			}

			// If we are showing escape routes from the battle sector, we'll need to handle the arrows
			// differently.
			BOOLEAN fShowingPossibleRetreatInThisDirection = FALSE;
			if (gfDisplayPotentialRetreatPaths)
			{
				// Is our currently-examined source sector the battle sector?
				if ( sBattleSector == sSourceSector )
				{
					curr = gpGroupList;
					// If so, is any of the groups in the sector able to retreat in the current direction?
					while (curr)
					{
						if ( curr->usGroupTeam == OUR_TEAM && // Player group
							curr->ubSectorX == sX && curr->ubSectorY == sY && // Group is in the battle sector
							curr->ubPrevX == tXD && curr->ubPrevY == tYD) // Group came from the examined destination
						{
							// Not helicopter!
							if ( !IsGroupTheHelicopterGroup( curr ) )
							{
								// Draw only one arrow exiting this sector in the current direction, in orange.
								fShowingPossibleRetreatInThisDirection = TRUE;
							}
						}
						curr = curr->next;
					}
				}
				// Is our currently-examined destination sector the battle sector?
				else if ( sBattleSector == sDestSector )
				{
					// If so, is any of the groups there capable of retreating in the opposite direction?
					curr = gpGroupList;
					while (curr)
					{
						if ( curr->usGroupTeam == OUR_TEAM && // Player group
							curr->ubSectorX == tXD && curr->ubSectorY == tYD && // Group is in the battle sector
							curr->ubPrevX == sX && curr->ubPrevY == sY) // Group came from the examined source
						{
							// Not helicopter!
							if ( !IsGroupTheHelicopterGroup( curr ) )
							{
								// We've got an arrow entering the sector.
								fEntering = TRUE;
							}
						}
						curr = curr->next;
					}
				}
			}
			// If none of the flags have been set yet, check for groups entering the sector at this time.
			if (!fEntering)
			{
				curr = gpGroupList;
				while (curr)
				{
					if ( curr->usGroupTeam == OUR_TEAM &&	// Player group
						curr->ubNextX == tXS && curr->ubNextY == tYS && // Heading into this sector
						!IsGroupTheHelicopterGroup( curr ) )	// Not a helicopter group!
					{
						// There are people entering from this direction
						fEntering = TRUE;
					}
					curr = curr->next;
				}
			}
			
			// Offset the arrows exiting this sector so that they don't overlap with the arrows entering the
			// sector.

			GetVideoObject(&hIconHandle, guiCHARBETWEENSECTORICONS );

			usArrowWidth = hIconHandle->pETRLEObject[ iDirection ].usWidth;
			usArrowHeight = hIconHandle->pETRLEObject[ iDirection ].usHeight;

			if( !( iDirection % 2 ) )
			{
				// guys exiting north or south from this sector. Center the arrow.
				sOffsetX = (MAP_GRID_X / 2);

				if( fEntering > 0 )
				{
					// A team is also entering this sector from the same direction.
					
					// Is the team in this sector heading north?
					if (iDirection == 0)
					{
						sOffsetX += 1;
					}
					else
					{
						sOffsetX -= usArrowWidth+1;
					}
				}
				else
				{
					// Center the arrow along the X axis.
					sOffsetX -= (usArrowWidth / 2);
				}

				if( iDirection == 0 )
				{
					// going north
					sOffsetY = -usArrowHeight;
				}
				else
				{
					// going south
					sOffsetY = MAP_GRID_Y;
				}
			}
			else
			{
				// guys exiting east or west from this sector. Center the arrow.

				sOffsetY = (MAP_GRID_Y / 2);

				if( fEntering > 0 )
				{
					// A team is also entering this sector from the same direction.
					
					// Is the team in this sector heading east?
					if (iDirection == 1)
					{
						sOffsetY += 1;
					}
					else
					{
						sOffsetY -= usArrowHeight+1;
					}
				}
				else
				{
					// Center the arrow along the Y axis.
					sOffsetY -= (usArrowHeight / 2);
				}

				if( iDirection == 1 )
				{
					// going east
					sOffsetX = MAP_GRID_X;
				}
				else
				{
					// going west
					sOffsetX = -usArrowWidth;
				}
			}

			// Have we found a group that can retreat in this direction?
			if (fShowingPossibleRetreatInThisDirection)
			{
				// Draw only its arrow, in orange.

				iX = sSectorPosX + sOffsetX;
				iY = (sSectorPosY + sOffsetY) -1;

				hIconHandle->p16BPPPalette[2] = Get16BPPColor( (UINT16)FROMRGB( 244, 103, 245 ) ) ;
				hIconHandle->p16BPPPalette[3] = Get16BPPColor( (UINT16)FROMRGB( 239, 182, 143 ) ) ;
				hIconHandle->p16BPPPalette[4] = Get16BPPColor( (UINT16)FROMRGB( 234, 158, 105 ) ) ;
				hIconHandle->p16BPPPalette[5] = Get16BPPColor( (UINT16)FROMRGB( 225, 118, 45 ) ) ;

				BltVideoObject(guiSAVEBUFFER, hIconHandle, ( UINT16 )iDirection , ( INT16 ) iX, ( INT16 ) iY , VO_BLT_SRCTRANSPARENCY, NULL );
				
				// We've drawn the only arrow we need to, so continue to check the next direction.
				continue;
			}

			// At this point we know that this direction should be checked normally, for any groups currently leaving it.

			// First lets set some basic values depending on direction.
			INT16 sNoProgressOffsetX = 0;
			INT16 sNoProgressOffsetY = 0;
			if (iDirection == 0)
			{
				sNoProgressOffsetY = usArrowHeight;
			}
			else if (iDirection == 1)
			{
				sNoProgressOffsetX = -usArrowWidth;
			}
			else if (iDirection == 2)
			{
				sNoProgressOffsetY = -usArrowHeight;
			}
			else if (iDirection == 3)
			{
				sNoProgressOffsetX = usArrowWidth;
			}

			// We run through all groups one by one, and find the group with the longest travel time remaining. We draw
			// an arrow for it, and then check for the group with the next longest travel time, and so on until we have
			// no more arrows to draw.

			//	check all groups
			BOOLEAN fMoreGroupsToCheck = TRUE;
			BOOLEAN fFoundFirstGroup = FALSE;
			FLOAT dLastTravelProgress = 10000.0f; // Stores the fractional travel time for the last group drawn.
			FLOAT dFirstTravelProgress = 0;
			INT32 dLastTravelTimeRemaining = 0;
			while (fMoreGroupsToCheck)
			{
				fMoreGroupsToCheck = FALSE; // Assume there are no more groups until we find them.

				// If we have not yet found the group with the least progress...
				if (!fFoundFirstGroup)
				{
					curr = gpGroupList;
					while( curr )
					{
						if ( curr->usGroupTeam == OUR_TEAM &&	// Player group
							curr->fBetweenSectors == TRUE && // Currently in motion
							SECTOR(curr->ubSectorX, curr->ubSectorY) == sSourceSector && // Heading out of this sector
							SECTOR(curr->ubNextX, curr->ubNextY) == sDestSector ) // Heading to the destination we're examining
						{
							if (!IsGroupTheHelicopterGroup( curr ) )	// Not a helicopter group! 
							{
								// How long does it take to travel from source to dest?
								INT32 iFullTravelTime = curr->uiTraverseTime;
								// How far is this group from the starting point?
								INT32 iCurTravelTime = iFullTravelTime - (curr->uiArrivalTime - GetWorldTotalMin( ));
	
								iCurTravelTime = __max(0, iCurTravelTime);
								iCurTravelTime = __min(iFullTravelTime, iCurTravelTime);
	
								// If the group is just starting to move, this will be 0.0.
								// If it is almsot at the destination, this will be close to 1.0.
								FLOAT dCurTravelProgress = (FLOAT)iCurTravelTime / (FLOAT)iFullTravelTime;

								// Is this the group with the most distance to the destination?
								if ( dCurTravelProgress < dLastTravelProgress )
								{
									// If this isn't the first group we've found...
									if (dLastTravelProgress < 10000.0f)
									{
										// We've skipped at least one group. Set the flag so we know to check for
										// more groups later.
										fMoreGroupsToCheck = TRUE;
									}

									// Ok, this is the group with the most distance to travel thus far. 
									// Lets store the data.
									dLastTravelProgress = dCurTravelProgress;
								}
								else
								{
									// Found a group that needs to be checked later. Set the flag.
									fMoreGroupsToCheck = TRUE;
								}
							}
						}
						curr = curr->next;
					}
					if (dLastTravelProgress < 10000.0f)
					{
						// We've found the first group - i.e. the group with the longest travel time remaining.
						fFoundFirstGroup = TRUE;

						FLOAT dProgressForDrawing = dLastTravelProgress;
						if (dProgressForDrawing < 0.49f)
						{
							dProgressForDrawing = __max(0.0f, dProgressForDrawing - 0.1f); // Bias downwards.
						}
						else if (dProgressForDrawing > 0.51f)
						{
							dProgressForDrawing = __min(1.0f, dProgressForDrawing + 0.1f); // Bias upwards.
						}

						INT16 sProgressOffsetX = (INT16)((1.0f-dProgressForDrawing) * (FLOAT)sNoProgressOffsetX);
						INT16 sProgressOffsetY = (INT16)((1.0f-dProgressForDrawing) * (FLOAT)sNoProgressOffsetY);

						UINT8 ubDark[3];
						UINT8 ubMid[3];
						UINT8 ubLight[3];
						UINT8 ubLighter[3];

						ubLighter[0] = (UINT8)(173 + ((FLOAT)(247 - 173) * dProgressForDrawing));
						ubLighter[1] = (UINT8)(199 + ((FLOAT)(245 - 199) * dProgressForDrawing));
						ubLighter[2] = (UINT8)(247 + ((FLOAT)(173 - 247) * dProgressForDrawing));

						ubLight[0] = (UINT8)(148 + ((FLOAT)(239 - 148) * dProgressForDrawing));
						ubLight[1] = (UINT8)(178 + ((FLOAT)(249 - 178) * dProgressForDrawing));
						ubLight[2] = (UINT8)(239 + ((FLOAT)(138 - 239) * dProgressForDrawing));

						ubMid[0] = (UINT8)(107 + ((FLOAT)(228 - 107) * dProgressForDrawing));
						ubMid[1] = (UINT8)(146 + ((FLOAT)(231 - 146) * dProgressForDrawing));
						ubMid[2] = (UINT8)(231 + ((FLOAT)(107 - 231) * dProgressForDrawing));

						ubDark[0] = (UINT8)(82 + ((FLOAT)(179 - 82) * dProgressForDrawing));
						ubDark[1] = (UINT8)(113 + ((FLOAT)(181 - 113) * dProgressForDrawing));
						ubDark[2] = (UINT8)(181 + ((FLOAT)(82 - 181) * dProgressForDrawing));

						hIconHandle->p16BPPPalette[2] = Get16BPPColor( (UINT32)FROMRGB( ubLighter[0], ubLighter[1], ubLighter[2] ) ) ;
						hIconHandle->p16BPPPalette[3] = Get16BPPColor( (UINT32)FROMRGB( ubLight[0], ubLight[1], ubLight[2] ) ) ;
						hIconHandle->p16BPPPalette[4] = Get16BPPColor( (UINT32)FROMRGB( ubMid[0], ubMid[1], ubMid[2] ) ) ;
						hIconHandle->p16BPPPalette[5] = Get16BPPColor( (UINT32)FROMRGB( ubDark[0], ubDark[1], ubDark[2] ) ) ;

						iX = sSectorPosX + sOffsetX + sProgressOffsetX;
						iY = ((sSectorPosY + sOffsetY) -1) + sProgressOffsetY;

						BltVideoObject(guiSAVEBUFFER, hIconHandle, ( UINT16 )iDirection , ( INT16 ) iX, ( INT16 ) iY , VO_BLT_SRCTRANSPARENCY, NULL );
					}
					else
					{
						// No squads moving at all! Because the flag is set, this will end the check.
						fMoreGroupsToCheck = FALSE;
						continue;
					}

					// Store the progress of the first group we've found.
					dFirstTravelProgress = dLastTravelProgress;
				}

				// If we've already found the first group, we need to look for the next group that is closer to the
				// destination.
				else
				{
					// Run through the list again.
					curr = gpGroupList;
					dLastTravelProgress = 10000.0f;
					while( curr )
					{
						if ( curr->usGroupTeam == OUR_TEAM &&	// Player group
							curr->fBetweenSectors == TRUE && // Currently in motion
							SECTOR(curr->ubSectorX, curr->ubSectorY) == sSourceSector && // Heading out of this sector
							SECTOR(curr->ubNextX, curr->ubNextY) == sDestSector ) // Heading to the destination we're examining
						{
							if (!IsGroupTheHelicopterGroup( curr ) )	// Not a helicopter group! 
							{
								// How long does it take to travel from source to dest?
								INT32 iFullTravelTime = curr->uiTraverseTime;
								// How far is this group from the starting point?
								INT32 iCurTravelTime = iFullTravelTime - (curr->uiArrivalTime - GetWorldTotalMin( ));
	
								iCurTravelTime = __max(0, iCurTravelTime);
								iCurTravelTime = __min(iFullTravelTime, iCurTravelTime);
	
								// If the group is just starting to move, this will be 0.0.
								// If it is almsot at the destination, this will be close to 1.0.
								FLOAT dCurTravelProgress = (FLOAT)iCurTravelTime / (FLOAT)iFullTravelTime;

								// Check this group's travel time against the last group we've drawn an arrow for.
								if ( dCurTravelProgress > dFirstTravelProgress && // This group is closer to the destination than the previous one
									dCurTravelProgress < dLastTravelProgress ) // And is the next furthest one from the destination so far
								{
									// If this isn't the first group we've found...
									if (dLastTravelProgress < 10000.0f)
									{
										// We've skipped at least one group. Set the flag so we know to check for
										// more groups later.
										fMoreGroupsToCheck = TRUE;
									}

									// Brilliant, lets store the data.
									dLastTravelProgress = dCurTravelProgress;
								}
								// Is this group nonetheless closer to the destination than the last group we drew
								// an arrow for?
								else if (dCurTravelProgress > dFirstTravelProgress)
								{
									// Found a group that needs to be checked later. Set the flag.
									fMoreGroupsToCheck = TRUE;
								}
							}
						}
						curr = curr->next;
					}
					if (dLastTravelProgress < 10000.0f)
					{
						FLOAT dProgressForDrawing = dLastTravelProgress;
						if (dProgressForDrawing < 0.49f)
						{
							dProgressForDrawing = __max(0.0f, dProgressForDrawing - 0.1f); // Bias downwards.
						}
						else if (dProgressForDrawing > 0.51f)
						{
							dProgressForDrawing = __min(1.0f, dProgressForDrawing + 0.1f); // Bias upwards.
						}

						INT16 sProgressOffsetX = (INT16)((1.0f-dProgressForDrawing) * (FLOAT)sNoProgressOffsetX);
						INT16 sProgressOffsetY = (INT16)((1.0f-dProgressForDrawing) * (FLOAT)sNoProgressOffsetY);

						UINT8 ubDark[3];
						UINT8 ubMid[3];
						UINT8 ubLight[3];
						UINT8 ubLighter[3];

						ubLighter[0] = (UINT8)(173 + ((FLOAT)(247 - 173) * dProgressForDrawing));
						ubLighter[1] = (UINT8)(199 + ((FLOAT)(245 - 199) * dProgressForDrawing));
						ubLighter[2] = (UINT8)(247 + ((FLOAT)(173 - 247) * dProgressForDrawing));

						ubLight[0] = (UINT8)(148 + ((FLOAT)(239 - 148) * dProgressForDrawing));
						ubLight[1] = (UINT8)(178 + ((FLOAT)(249 - 178) * dProgressForDrawing));
						ubLight[2] = (UINT8)(239 + ((FLOAT)(138 - 239) * dProgressForDrawing));

						ubMid[0] = (UINT8)(107 + ((FLOAT)(228 - 107) * dProgressForDrawing));
						ubMid[1] = (UINT8)(146 + ((FLOAT)(231 - 146) * dProgressForDrawing));
						ubMid[2] = (UINT8)(231 + ((FLOAT)(107 - 231) * dProgressForDrawing));

						ubDark[0] = (UINT8)(82 + ((FLOAT)(179 - 82) * dProgressForDrawing));
						ubDark[1] = (UINT8)(113 + ((FLOAT)(181 - 113) * dProgressForDrawing));
						ubDark[2] = (UINT8)(181 + ((FLOAT)(82 - 181) * dProgressForDrawing));

						hIconHandle->p16BPPPalette[2] = Get16BPPColor( (UINT32)FROMRGB( ubLighter[0], ubLighter[1], ubLighter[2] ) ) ;
						hIconHandle->p16BPPPalette[3] = Get16BPPColor( (UINT32)FROMRGB( ubLight[0], ubLight[1], ubLight[2] ) ) ;
						hIconHandle->p16BPPPalette[4] = Get16BPPColor( (UINT32)FROMRGB( ubMid[0], ubMid[1], ubMid[2] ) ) ;
						hIconHandle->p16BPPPalette[5] = Get16BPPColor( (UINT32)FROMRGB( ubDark[0], ubDark[1], ubDark[2] ) ) ;

						iX = sSectorPosX + sOffsetX + sProgressOffsetX;
						iY = ((sSectorPosY + sOffsetY) -1) + sProgressOffsetY;

						BltVideoObject(guiSAVEBUFFER, hIconHandle, ( UINT16 )iDirection , ( INT16 ) iX, ( INT16 ) iY , VO_BLT_SRCTRANSPARENCY, NULL );
					}
					else
					{
						// No squads moving at all! Because the flag is set, this will end the check.
						fMoreGroupsToCheck = FALSE;
						continue;
					}
					dFirstTravelProgress = dLastTravelProgress;
				}


				/*
				switch( iDirection % 2 )
				{
					case 0 :
						// north south
						iWidth = 10;
						iHeight = 12;
					break;
					case 1 :
						// east west
						iWidth = 12;
						iHeight = 7;
					break;

				}

				// error correction for scrolling with people on the move
				if( iX < 0 )
				{
					iDeltaXForError = 0 - iX;
					iWidth -= iDeltaXForError;
					iX = 0;
				}

				if( iY < 0 )
				{
					iDeltaYForError = 0 - iY;
					iHeight -= iDeltaYForError;
					iY = 0;
				}

				if( ( iWidth > 0 )&&( iHeight > 0 ) )
				{
					RestoreExternBackgroundRect( iX, iY, ( INT16 )iWidth, ( INT16 )iHeight );
				}
				*/
			}
		}
	}

	// restore buffer
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
}

// HEADROCK HAM 5.1: Show enemy group destinations.
void ShowNonPlayerGroupsInMotion( INT16 sX, INT16 sY, UINT8 usTeam )
{
	INT32 sExiting = 0;
	INT32 sEntering = 0;
	INT16 sDest = 0;
	INT16 sSource = 0;
	INT16 sOffsetX = 0, sOffsetY = 0;
	INT16 sXPosition = 0, sYPosition = 0;
	INT32 iCounter = 0;
	HVOBJECT hIconHandle;
	INT16 sTextXOffset = 0;
	INT16 sTextYOffset = 0;
	INT32 iWidth = 0, iHeight = 0;
	INT32 iDeltaXForError = 0, iDeltaYForError = 0;
	
	if( iCurrentMapSectorZ != 0 )
		return;

	// show the icons for people in motion from this sector to the next guy over

	GROUP *curr;
	curr = gpGroupList;
	while( curr )
	{
		if ( curr->usGroupTeam != OUR_TEAM && curr->usGroupTeam == usTeam && curr->ubSectorX == sX && curr->ubSectorY == sY && curr->ubSectorZ == 0 )
		{
			if (curr->ubNextX != sX || curr->ubNextY != sY)
			{
				if (curr->ubNextX >= MINIMUM_VALID_X_COORDINATE && curr->ubNextX <= MAXIMUM_VALID_X_COORDINATE &&
					curr->ubNextY >= MINIMUM_VALID_Y_COORDINATE && curr->ubNextY <= MAXIMUM_VALID_Y_COORDINATE ) // 3 hours to arrival, tops
				{
					INT16 iX = sX, iY = sY;
					UINT8 ubDirection = 0;
					INT16 sDeltaX = curr->ubNextX - sX;
					INT16 sDeltaY = curr->ubNextY - sY;

					GetVideoObject(&hIconHandle, guiENEMYBETWEENSECTORICONS );

					INT16 iconOffsetX = 0;
					INT16 iconOffsetY = 0;

					if (sDeltaX != 0 && sDeltaY != 0)
					{
						// Group moving diagonally? Abort.
						continue;
					}
					
					if (sDeltaY < 0)
					{
						ubDirection = 0;
						sOffsetX = (MAP_GRID_X / 2);
						sOffsetY = 0;
						iconOffsetX = -(hIconHandle->pETRLEObject[ubDirection].usWidth / 2);
						iconOffsetY = -(hIconHandle->pETRLEObject[ubDirection].usHeight);
					}
					else if (sDeltaX > 0)
					{
						ubDirection = 1;
						sOffsetX = MAP_GRID_X;
						sOffsetY = (MAP_GRID_Y / 2);
						iconOffsetY = -(hIconHandle->pETRLEObject[ubDirection].usHeight / 2);
					}
					else if (sDeltaY > 0)
					{
						ubDirection = 2;
						sOffsetX = (MAP_GRID_X / 2);
						sOffsetY = MAP_GRID_Y;
						iconOffsetX = -(hIconHandle->pETRLEObject[ubDirection].usWidth / 2);
					}
					else if (sDeltaX < 0)
					{
						ubDirection = 3;
						sOffsetX = 0;
						sOffsetY = (MAP_GRID_Y / 2);
						iconOffsetX = -(hIconHandle->pETRLEObject[ubDirection].usWidth);
						iconOffsetY = -(hIconHandle->pETRLEObject[ubDirection].usHeight / 2);
					}

					if (iResolution >= _1024x768 )
					{
						iconOffsetY -= 1;
					}
					
					iX = MAP_VIEW_START_X+( sX * MAP_GRID_X ) + sOffsetX + iconOffsetX;
					iY = MAP_VIEW_START_Y + ( sY * MAP_GRID_Y ) + sOffsetY + iconOffsetY;

					// Flugente: depending on which team's direction we show there is an offset to the images
					// Note that we also need a x or y offset if multiple teamsare moving in the same sector, but currently that won't happen
					if ( curr->usGroupTeam == MILITIA_TEAM )
						ubDirection += 4;
		
					BltVideoObject(guiSAVEBUFFER, hIconHandle, ubDirection , ( INT16 ) iX, ( INT16 ) iY , VO_BLT_SRCTRANSPARENCY, NULL );
				
					iWidth = 12;
					iHeight = 7;

					// error correction for scrolling with people on the move
					if( iX < 0 )
					{
						iDeltaXForError = 0 - iX;
						iWidth -= iDeltaXForError;
						iX = 0;
					}

					if( iY < 0 )
					{
						iDeltaYForError = 0 - iY;
						iHeight -= iDeltaYForError;
						iY = 0;
					}

					if( ( iWidth > 0 )&&( iHeight > 0 ) )
					{
						RestoreExternBackgroundRect( iX, iY, ( INT16 )iWidth, ( INT16 )iHeight );
					}
				}
			}
		}
		curr = curr->next;
	}
}

// Flugente: show militia movement
void ShowMilitiaInMotion( INT16 sX, INT16 sY )
{
	INT32 sExiting = 0;
	INT32 sEntering = 0;
	INT16 sDest = 0;
	INT16 sSource = 0;
	INT16 sOffsetX = 0, sOffsetY = 0;
	INT16 sXPosition = 0, sYPosition = 0;
	INT32 iCounter = 0;
	HVOBJECT hIconHandle;
	INT16 sTextXOffset = 0;
	INT16 sTextYOffset = 0;
	INT32 iWidth = 0, iHeight = 0;
	INT32 iDeltaXForError = 0, iDeltaYForError = 0;

	if( iCurrentMapSectorZ )
		return;
	
	if ( !NumNonPlayerTeamMembersInSector( sX, sY, MILITIA_TEAM ) )
		return;

	if ( ( StrategicMap[ sX + ( sY * MAP_WORLD_X ) ].usFlags & MILITIA_MOVE_ALLDIRS ) == 0 )
		return;

	// show the icons for people in motion from this sector to the next guy over

	INT16 iX = sX, iY = sY;
	UINT8 ubDirection = 0;

	GetVideoObject(&hIconHandle, guiENEMYBETWEENSECTORICONS );

	INT16 iconOffsetX = 0;
	INT16 iconOffsetY = 0;

	if ( StrategicMap[ sX + ( sY * MAP_WORLD_X ) ].usFlags & MILITIA_MOVE_NORTH )
	{
		ubDirection = 4;
		sOffsetX = (MAP_GRID_X / 2);
		sOffsetY = 0;
		iconOffsetX = -(hIconHandle->pETRLEObject[ubDirection].usWidth / 2);
		iconOffsetY = -(hIconHandle->pETRLEObject[ubDirection].usHeight);
	}
	else if ( StrategicMap[ sX + ( sY * MAP_WORLD_X ) ].usFlags & MILITIA_MOVE_EAST )
	{
		ubDirection = 5;
		sOffsetX = MAP_GRID_X;
		sOffsetY = (MAP_GRID_Y / 2);
		iconOffsetY = -(hIconHandle->pETRLEObject[ubDirection].usHeight / 2);
	}
	else if ( StrategicMap[ sX + ( sY * MAP_WORLD_X ) ].usFlags & MILITIA_MOVE_SOUTH )
	{
		ubDirection = 6;
		sOffsetX = (MAP_GRID_X / 2);
		sOffsetY = MAP_GRID_Y;
		iconOffsetX = -(hIconHandle->pETRLEObject[ubDirection].usWidth / 2);
	}
	else if ( StrategicMap[ sX + ( sY * MAP_WORLD_X ) ].usFlags & MILITIA_MOVE_WEST )
	{
		ubDirection = 7;
		sOffsetX = 0;
		sOffsetY = (MAP_GRID_Y / 2);
		iconOffsetX = -(hIconHandle->pETRLEObject[ubDirection].usWidth);
		iconOffsetY = -(hIconHandle->pETRLEObject[ubDirection].usHeight / 2);
	}

	if (iResolution >= _1024x768 )
	{
		iconOffsetY -= 1;
	}
					
	iX = MAP_VIEW_START_X+( sX * MAP_GRID_X ) + sOffsetX + iconOffsetX;
	iY = MAP_VIEW_START_Y + ( sY * MAP_GRID_Y ) + sOffsetY + iconOffsetY;
		
	BltVideoObject(guiSAVEBUFFER, hIconHandle, ubDirection , ( INT16 ) iX, ( INT16 ) iY , VO_BLT_SRCTRANSPARENCY, NULL );
				
	iWidth = 12;
	iHeight = 7;

	// error correction for scrolling with people on the move
	if( iX < 0 )
	{
		iDeltaXForError = 0 - iX;
		iWidth -= iDeltaXForError;
		iX = 0;
	}

	if( iY < 0 )
	{
		iDeltaYForError = 0 - iY;
		iHeight -= iDeltaYForError;
		iY = 0;
	}

	if( ( iWidth > 0 )&&( iHeight > 0 ) )
	{
		RestoreExternBackgroundRect( iX, iY, ( INT16 )iWidth, ( INT16 )iHeight );
	}
}

void DisplayDistancesForHelicopter( void )
{
	// calculate the distance travelled, the proposed distance, and total distance one can go
	// display these on screen
	INT16 sDistanceToGo = 0, sDistanceSoFar = 0, sTotalCanTravel = 0;
	INT16 sRemainingFuel = 0, sDistToRefuelSite = 0;
	INT16 sX = 0, sY = 0;
	CHAR16 sString[ 32 ];
	HVOBJECT hHandle;
	INT16 sTotalOfTrip = 0;
	INT32 iTime = 0;
	INT16 sMapX, sMapY;
	INT16 sYPosition = 0;
	static INT16 sOldYPosition = 0;
	INT16 sNumSafeSectors;
	INT16 sNumUnSafeSectors;
	UINT32 uiTripCost;
	
	if ( GetMouseMapXY( &sMapX, &sMapY ) && ( sMapY >= 13 ) )
	{
		sYPosition = MAP_HELICOPTER_UPPER_ETA_POPUP_Y;
	}
	else
	{
		sYPosition = MAP_HELICOPTER_ETA_POPUP_Y;
	}

	if( ( sOldYPosition != 0 ) && ( sOldYPosition != sYPosition ) )
	{
		if( !gGameExternalOptions.fAlternativeHelicopterFuelSystem )
			RestoreExternBackgroundRect( MAP_HELICOPTER_ETA_POPUP_X, sOldYPosition, MAP_HELICOPTER_ETA_POPUP_WIDTH + 20, MAP_HELICOPTER_ETA_POPUP_HEIGHT );
		else
			RestoreExternBackgroundRect( MAP_HELICOPTER_ETA_POPUP_X, sOldYPosition, MAP_HELICOPTER_ETA_POPUP_WIDTH + 20, MAP_HELICOPTER_ETA_POPUP_ALTERNATE_HEIGHT );
	}

	sOldYPosition = sYPosition;

	// blit in background
	UINT8 imageIndex = 0;
	if( gGameExternalOptions.fAlternativeHelicopterFuelSystem )
		GetVideoObject( &hHandle, guiMapBorderHeliSectorsAlternate );
	else
		GetVideoObject( &hHandle, guiMapBorderHeliSectors );

	if (iResolution >= _640x480 && iResolution < _800x600)
		imageIndex = 0;
	else if (iResolution < _1024x768)
		imageIndex = 1;
	else
		imageIndex = 2;
		
	BltVideoObject( FRAME_BUFFER, hHandle, imageIndex, MAP_HELICOPTER_ETA_POPUP_X, sYPosition, VO_BLT_SRCTRANSPARENCY, NULL );

	sTotalCanTravel = ( INT16 )GetTotalDistanceHelicopterCanTravel( );
	sDistanceToGo = ( INT16 )DistanceOfIntendedHelicopterPath( );
	sTotalOfTrip = sDistanceToGo;		// + ( INT16 ) ( DistanceToNearestRefuelPoint( ( INT16 )( LastSectorInHelicoptersPath() % MAP_WORLD_X ), ( INT16 ) ( LastSectorInHelicoptersPath() / MAP_WORLD_X ) ) );
	sNumSafeSectors = GetNumSafeSectorsInPath( );
	sNumUnSafeSectors = GetNumUnSafeSectorsInPath( );

	//sDistanceSoFar = ( INT16 )HowFarHelicopterhasTravelledSinceRefueling( );
	//sTotalDistanceOfTrip = ( INT16 )DistanceToNearestRefuelPoint( );
	sRemainingFuel = ( INT16 )(gHelicopterSettings.ubHelicopterDistanceWithoutRefuel - iTotalHeliDistanceSinceRefuel - sDistanceToGo);
	sDistToRefuelSite = ( INT16 )DistanceToNearestRefuelPoint( sMapX, sMapY );

	if( sDistanceToGo == 9999)
	{
		sDistanceToGo = 0;
	}

	// set the font stuff
	SetFont( MAP_FONT );
	SetFontForeground( FONT_LTGREEN );
	SetFontBackground( FONT_BLACK );

	swprintf( sString, L"%s", pHelicopterEtaStrings[ STR_HELI_ETA_TOTAL_DISTANCE ] );
	mprintf( MAP_HELICOPTER_ETA_POPUP_X + 5, sYPosition + 5, sString );

/*
  if ( IsSectorOutOfTheWay( sMapX, sMapY ) )
  {
		SetFontForeground( FONT_RED );
	}
	else
*/
	{
		SetFontForeground( FONT_LTGREEN );
	}

	swprintf( sString, L"%d", sTotalOfTrip );
	FindFontRightCoordinates( MAP_HELICOPTER_ETA_POPUP_X + 5, MAP_HELICOPTER_ETA_POPUP_Y + 5, MAP_HELICOPTER_ETA_POPUP_WIDTH, 0,  sString, MAP_FONT,  &sX, &sY );
	mprintf( sX, sYPosition + 5, sString );

	SetFontForeground( FONT_LTGREEN );

	swprintf( sString, L"%s", pHelicopterEtaStrings[ STR_HELI_ETA_SAFE ] );
	mprintf( MAP_HELICOPTER_ETA_POPUP_X + 5, sYPosition + 5 + GetFontHeight( MAP_FONT ), sString );

	swprintf( sString, L"%d", sNumSafeSectors );
	FindFontRightCoordinates( MAP_HELICOPTER_ETA_POPUP_X + 5, ( INT16 ) ( MAP_HELICOPTER_ETA_POPUP_Y + 5 + 2 * GetFontHeight( MAP_FONT ) ), MAP_HELICOPTER_ETA_POPUP_WIDTH, 0,  sString, MAP_FONT,  &sX, &sY );
	mprintf( sX, ( INT16 ) ( sYPosition + 5 + GetFontHeight( MAP_FONT ) ), sString );

	swprintf( sString, L"%s", pHelicopterEtaStrings[ STR_HELI_ETA_UNSAFE ] );
	mprintf( MAP_HELICOPTER_ETA_POPUP_X + 5, sYPosition + 5 + 2 * GetFontHeight( MAP_FONT ), sString );

	swprintf( sString, L"%d", sNumUnSafeSectors );
	FindFontRightCoordinates( MAP_HELICOPTER_ETA_POPUP_X + 5, ( INT16 ) ( MAP_HELICOPTER_ETA_POPUP_Y + 5 + 2 * GetFontHeight( MAP_FONT ) ), MAP_HELICOPTER_ETA_POPUP_WIDTH, 0,  sString, MAP_FONT,  &sX, &sY );
	mprintf( sX, ( INT16 ) ( sYPosition + 5 + 2 * GetFontHeight( MAP_FONT ) ), sString );

	swprintf( sString, L"%s", pHelicopterEtaStrings[ STR_HELI_ETA_TOTAL_COST ] );
	mprintf( MAP_HELICOPTER_ETA_POPUP_X + 5, sYPosition + 5 + 3 * GetFontHeight( MAP_FONT ), sString );
	
	// calculate the cost of the trip based on the number of safe and unsafe sectors it will pass through
	// HEADROCK HAM 3.5: Externalized Base Cost. Also includes hourly-calculated facility modifier.
	UINT32 uiCostGreen = __max(0,gGameExternalOptions.usHelicopterBaseCostPerGreenTile + gsSkyriderCostModifier);
	UINT32 uiCostRed = __max(0,gGameExternalOptions.usHelicopterBaseCostPerRedTile + gsSkyriderCostModifier);
	uiTripCost = ( sNumSafeSectors * uiCostGreen ) + ( sNumUnSafeSectors * uiCostRed );

	if( (INT32)uiTripCost > LaptopSaveInfo.iCurrentBalance )
		SetFontForeground( FONT_LTRED );
		
	swprintf( sString, L"%d", uiTripCost );
	InsertCommasForDollarFigure( sString );
	InsertDollarSignInToString( sString );
	FindFontRightCoordinates( MAP_HELICOPTER_ETA_POPUP_X + 5, ( INT16 ) ( MAP_HELICOPTER_ETA_POPUP_Y + 5 + 3 * GetFontHeight( MAP_FONT ) ), MAP_HELICOPTER_ETA_POPUP_WIDTH, 0,  sString, MAP_FONT,  &sX, &sY );
	mprintf( sX, ( INT16 ) ( sYPosition + 5 + 3 * GetFontHeight( MAP_FONT ) ), sString );

	SetFontForeground( FONT_LTGREEN );

	swprintf( sString, L"%s", pHelicopterEtaStrings[ STR_HELI_ETA_ETA ] );
	mprintf( MAP_HELICOPTER_ETA_POPUP_X + 5, sYPosition + 5 + 4 * GetFontHeight( MAP_FONT ), sString );

	// get travel time for the last path segment
	iTime = GetPathTravelTimeDuringPlotting( pTempHelicopterPath );

	// add travel time for any prior path segments (stored in the helicopter's mercpath, but waypoints aren't built)
	iTime += GetPathTravelTimeDuringPlotting( pVehicleList[ iHelicopterVehicleId ].pMercPath );

	swprintf( sString, L"%d%s %d%s", iTime / 60, gsTimeStrings[0], iTime % 60, gsTimeStrings[1] );
	FindFontRightCoordinates( MAP_HELICOPTER_ETA_POPUP_X + 5, ( INT16 ) ( sYPosition + 5 + 4 * GetFontHeight( MAP_FONT ) ), MAP_HELICOPTER_ETA_POPUP_WIDTH, 0,  sString, MAP_FONT,  &sX, &sY );
	mprintf( sX, ( INT16 ) ( sYPosition + 5 + 4 * GetFontHeight( MAP_FONT ) ), sString );


	// show # of passengers aboard the chopper
	mprintf( MAP_HELICOPTER_ETA_POPUP_X + 5, sYPosition + 5 + 5 * GetFontHeight( MAP_FONT ), pHelicopterEtaStrings[ STR_HELI_ETA_PASSENGERS ] );
	swprintf( sString, L"%d", GetNumberOfPassengersInHelicopter() );
	FindFontRightCoordinates( MAP_HELICOPTER_ETA_POPUP_X + 5, ( INT16 ) ( MAP_HELICOPTER_ETA_POPUP_Y + 5 + 5 * GetFontHeight( MAP_FONT ) ), MAP_HELICOPTER_ETA_POPUP_WIDTH, 0,  sString, MAP_FONT,  &sX, &sY );
	mprintf( sX, ( INT16 ) ( sYPosition + 5 + 5 * GetFontHeight( MAP_FONT ) ), sString );

	// show remaining fuel
	mprintf( MAP_HELICOPTER_ETA_POPUP_X + 5, sYPosition + 5 + 6 * GetFontHeight( MAP_FONT ), pHelicopterEtaStrings[ STR_HELI_ETA_REMAINING_FUEL ] );
	swprintf( sString, L"%d", sRemainingFuel );
	FindFontRightCoordinates( MAP_HELICOPTER_ETA_POPUP_X + 5, ( INT16 ) ( MAP_HELICOPTER_ETA_POPUP_Y + 5 + 6 * GetFontHeight( MAP_FONT ) ), MAP_HELICOPTER_ETA_POPUP_WIDTH, 0,  sString, MAP_FONT,  &sX, &sY );
	if( sRemainingFuel < sDistToRefuelSite )
		SetFontForeground( FONT_LTRED );
	mprintf( sX, ( INT16 ) ( sYPosition + 5 + 6 * GetFontHeight( MAP_FONT ) ), sString );
	SetFontForeground( FONT_LTGREEN );

	// show distance to the nearest refuel site
	mprintf( MAP_HELICOPTER_ETA_POPUP_X + 5, sYPosition + 5 + 7 * GetFontHeight( MAP_FONT ), pHelicopterEtaStrings[ STR_HELI_ETA_DISTANCE_TO_REFUEL_SITE ] );
	swprintf( sString, L"%d", sDistToRefuelSite );
	FindFontRightCoordinates( MAP_HELICOPTER_ETA_POPUP_X + 5, ( INT16 ) ( MAP_HELICOPTER_ETA_POPUP_Y + 5 + 7 * GetFontHeight( MAP_FONT ) ), MAP_HELICOPTER_ETA_POPUP_WIDTH, 0,  sString, MAP_FONT,  &sX, &sY );
	if( sRemainingFuel < sDistToRefuelSite )
		SetFontForeground( FONT_LTRED );
	mprintf( sX, ( INT16 ) ( sYPosition + 5 + 7 * GetFontHeight( MAP_FONT ) ), sString );
	SetFontForeground( FONT_LTGREEN );

	if( !gGameExternalOptions.fAlternativeHelicopterFuelSystem )
		InvalidateRegion( MAP_HELICOPTER_ETA_POPUP_X, sOldYPosition,  MAP_HELICOPTER_ETA_POPUP_X + MAP_HELICOPTER_ETA_POPUP_WIDTH + 20, sOldYPosition + MAP_HELICOPTER_ETA_POPUP_HEIGHT );
	else
		InvalidateRegion( MAP_HELICOPTER_ETA_POPUP_X, sOldYPosition,  MAP_HELICOPTER_ETA_POPUP_X + MAP_HELICOPTER_ETA_POPUP_WIDTH + 20, sOldYPosition + MAP_HELICOPTER_ETA_POPUP_ALTERNATE_HEIGHT );
}


// grab position of helicopter and blt to screen
void DisplayPositionOfHelicopter( void )
{
	static INT16 sOldMapX = 0, sOldMapY = 0;
//	INT16 sX =0, sY = 0;
	FLOAT flRatio = 0.0;
	UINT32 x,y;
	UINT16 minX, minY, maxX, maxY;
	GROUP *pGroup;
	HVOBJECT hHandle;
	INT32 iNumberOfPeopleInHelicopter = 0;
	CHAR16 sString[ 4 ];

	INT32 MAP_MVT_ICON_FONT = TINYFONT1;

	AssertMsg( ( sOldMapX >= 0 ) && ( sOldMapX < SCREEN_WIDTH ), String( "DisplayPositionOfHelicopter: Invalid sOldMapX = %d", sOldMapX ) );
	AssertMsg( ( sOldMapY >= 0 ) && ( sOldMapY < SCREEN_HEIGHT ), String( "DisplayPositionOfHelicopter: Invalid sOldMapY = %d", sOldMapY ) );
				
	// HEADROCK HAM 5: Now has to be done here to get the size of the helicopter icon.
	GetVideoObject( &hHandle, guiHelicopterIcon );

	UINT16 usIconWidth = hHandle->pETRLEObject[ 0 ].usWidth;
	UINT16 usIconHeight = hHandle->pETRLEObject[ 0 ].usHeight;
	UINT16 usIconOffsetX = hHandle->pETRLEObject[ 0 ].sOffsetX;
	UINT16 usIconOffsetY = hHandle->pETRLEObject[ 0 ].sOffsetY;

	// restore background on map where it is
	if( sOldMapX != 0 ) 
	{
		RestoreExternBackgroundRect( sOldMapX + usIconOffsetX, sOldMapY + usIconOffsetY, usIconWidth, usIconHeight );
		sOldMapX = 0;
	}

	if( iHelicopterVehicleId != -1 )
	{
		// draw the destination icon first, so when they overlap, the real one is on top!
		// HEADROCK HAM 5: No reason to display this anymore: we now do it with a cool new icon displayed 
		// while drawing the path.

		//DisplayDestinationOfHelicopter( );

		// check if mvt group
		if( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup != 0 ) {
			pGroup = GetGroup( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );

			if (!pGroup) {
				// Something is really goofed up, there's no group for the helicopter.  Re-create it.
				static bool heliMsg1Given = false;
				if (!heliMsg1Given) {
					DoScreenIndependantMessageBox( L"The helicopter data is corrupted.  Attempting to repair it...", MSG_BOX_FLAG_OK, NULL );
					heliMsg1Given = true;
				}
				RemoveVehicleFromList (iHelicopterVehicleId);
				InitAVehicle (iHelicopterVehicleId, 13, MAP_ROW_B);
				fSkyRiderSetUp = FALSE;
				SetUpHelicopterForPlayer( 13, MAP_ROW_B, gNewVehicle[ HELICOPTER ].NewPilot, HELICOPTER );
				pGroup = GetGroup( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );
				if (!pGroup) {
					static bool heliMsg2Given = false;
					if (!heliMsg2Given) {
						DoScreenIndependantMessageBox( L"The helicopter data is corrupted beyond repair.  Please report this and send a save file and your .ini file.  You can continue playing but the helicopter will not work properly.", MSG_BOX_FLAG_OK, NULL );
						heliMsg2Given = true;
					}
					return;
				}
			}
			if( ( pGroup->uiTraverseTime > 0 ) && ( pGroup->uiTraverseTime != 0xffffffff ) ) {
				flRatio = ( ( pGroup->uiTraverseTime + GetWorldTotalMin() ) - pGroup->uiArrivalTime ) / ( float ) pGroup->uiTraverseTime;
			}

			if ( flRatio < 0 ) {
				flRatio = 0;
			} else if ( flRatio > 100 ) {
				flRatio = 100;
			}

			// grab min and max locations to interpolate sub sector position
			minX = MAP_VIEW_START_X + MAP_GRID_X * ( pGroup->ubSectorX );
			maxX = MAP_VIEW_START_X + MAP_GRID_X * ( pGroup->ubNextX );
			minY = MAP_VIEW_START_Y + MAP_GRID_Y * ( pGroup->ubSectorY );
			maxY = MAP_VIEW_START_Y + MAP_GRID_Y * ( pGroup->ubNextY );

			AssertMsg( ( minX >= 0 ) && ( minX < SCREEN_WIDTH ), String( "DisplayPositionOfHelicopter: Invalid minX = %d", minX ) );
			AssertMsg( ( maxX >= 0 ) && ( maxX < SCREEN_WIDTH ), String( "DisplayPositionOfHelicopter: Invalid maxX = %d", maxX ) );
			AssertMsg( ( minY >= 0 ) && ( minY < SCREEN_WIDTH ), String( "DisplayPositionOfHelicopter: Invalid minY = %d", minY ) );
			AssertMsg( ( maxY >= 0 ) && ( maxY < SCREEN_WIDTH ), String( "DisplayPositionOfHelicopter: Invalid maxY = %d", maxY ) );

			// IMPORTANT: Since min can easily be larger than max, we gotta cast to as signed value
			x = ( UINT32 )( minX + flRatio * ( ( INT16 ) maxX - ( INT16 ) minX ) );
 			y = ( UINT32 )( minY + flRatio * ( ( INT16 ) maxY - ( INT16 ) minY ) );

			// HEADROCK HAM 5: Not necessary?
			//x += 1;
			//y += 3;

			AssertMsg( ( x >= 0 ) && ( x < (UINT32)SCREEN_WIDTH ), String( "DisplayPositionOfHelicopter: Invalid x = %d.  At %d,%d.  Next %d,%d.  Min/Max X = %d/%d",
						x, pGroup->ubSectorX, pGroup->ubSectorY, pGroup->ubNextX, pGroup->ubNextY, minX, maxX ) );
			AssertMsg( ( y >= 0 ) && ( y < (UINT32)SCREEN_HEIGHT ), String( "DisplayPositionOfHelicopter: Invalid y = %d.  At %d,%d.  Next %d,%d.  Min/Max Y = %d/%d",
						y, pGroup->ubSectorX, pGroup->ubSectorY, pGroup->ubNextX, pGroup->ubNextY, minY, maxY ) );

			// clip blits to mapscreen region
			ClipBlitsToMapViewRegion( );
			// HEADROCK HAM 5: Not necessary?
			/*
			GetVideoObject( &hHandle, guiHelicopterIcon );

			if (iResolution >= _800x600)
			{
				x = x + (MAP_GRID_X / 2) - 10;
				y = y + 1 + (MAP_GRID_Y / 2) - 10;
			}
			*/

			BltVideoObject( FRAME_BUFFER, hHandle, HELI_ICON, x, y, VO_BLT_SRCTRANSPARENCY, NULL );

			// now get number of people and blit that too		
			iNumberOfPeopleInHelicopter =  GetNumberOfPassengersInHelicopter( );
			swprintf( sString, L"%d", iNumberOfPeopleInHelicopter );

			SetFont( MAP_MVT_ICON_FONT );
			SetFontForeground( FONT_WHITE );
			SetFontBackground( FONT_BLACK );

			mprintf( x + (MAP_GRID_X / 2) - 4, y + (MAP_GRID_Y / 2) - 4, sString );

			InvalidateRegion( x, y, x + usIconWidth, y + usIconHeight );

			RestoreClipRegionToFullScreen( );

			// now store the old stuff
			sOldMapX = ( INT16 ) x;
			sOldMapY = ( INT16 ) y;
		}
	}
}


void DisplayDestinationOfHelicopter( void )
{
	static INT16 sOldMapX = 0, sOldMapY = 0;
	INT16 sSector;
	INT16 sMapX, sMapY;
	UINT32 x,y;
	HVOBJECT hHandle;

	AssertMsg( ( sOldMapX >= 0 ) && ( sOldMapX < SCREEN_WIDTH ), String( "DisplayDestinationOfHelicopter: Invalid sOldMapX = %d", sOldMapX ) );
	AssertMsg( ( sOldMapY >= 0 ) && ( sOldMapY < SCREEN_HEIGHT ), String( "DisplayDestinationOfHelicopter: Invalid sOldMapY = %d", sOldMapY ) );

	// restore background on map where it is
	if( sOldMapX != 0 )
	{
		RestoreExternBackgroundRect( sOldMapX, sOldMapY, HELI_SHADOW_ICON_WIDTH, HELI_SHADOW_ICON_HEIGHT );
		sOldMapX = 0;
	}

	// if helicopter is going somewhere
	if ( GetLengthOfPath( pVehicleList[ iHelicopterVehicleId ].pMercPath ) > 1 )
	{
		// get destination
		sSector = GetLastSectorIdInVehiclePath( iHelicopterVehicleId );
		sMapX = sSector % MAP_WORLD_X;
		sMapY = sSector / MAP_WORLD_X;

		x = MAP_VIEW_START_X + ( MAP_GRID_X * sMapX ) + 1;
		y = MAP_VIEW_START_Y + ( MAP_GRID_Y * sMapY ) + 3;

		AssertMsg( ( x >= 0 ) && ( x < (UINT32)SCREEN_WIDTH ), String( "DisplayDestinationOfHelicopter: Invalid x = %d.  Dest %d,%d", x, sMapX, sMapY ) );
		AssertMsg( ( y >= 0 ) && ( y < (UINT32)SCREEN_HEIGHT ), String( "DisplayDestinationOfHelicopter: Invalid y = %d.  Dest %d,%d", y, sMapX, sMapY ) );

		// clip blits to mapscreen region
		ClipBlitsToMapViewRegion( );

		GetVideoObject( &hHandle, guiHelicopterIcon );

		BltVideoObject( FRAME_BUFFER, hHandle, HELI_SHADOW_ICON, x, y, VO_BLT_SRCTRANSPARENCY, NULL );

		if (iResolution >= _800x600)
		{
			x = x + (MAP_GRID_X / 2) - 10;
			y = y + 1 + (MAP_GRID_Y / 2) - 10;
		}

		InvalidateRegion( x, y, x + HELI_SHADOW_ICON_WIDTH, y + HELI_SHADOW_ICON_HEIGHT );

		RestoreClipRegionToFullScreen( );

		// now store the old stuff
		sOldMapX = ( INT16 ) x;
		sOldMapY = ( INT16 ) y;
	}
}



BOOLEAN CheckForClickOverHelicopterIcon( INT16 sClickedSectorX, INT16 sClickedSectorY )
{
	INT32 iDeltaTime = 0;
	BOOLEAN fIgnoreClick = FALSE;
	GROUP *pGroup = NULL;
	BOOLEAN fHelicopterOverNextSector = FALSE;
	FLOAT flRatio = 0.0;
	INT16 sSectorX;
	INT16 sSectorY;
	
	iDeltaTime = GetJA2Clock() - giClickHeliIconBaseTime;
	giClickHeliIconBaseTime = GetJA2Clock();

	if( iDeltaTime < 400 )
	{
		fIgnoreClick = TRUE;
	}

	if( !fHelicopterAvailable || !fShowAircraftFlag )
	{
		return( FALSE );
	}

	if ( iHelicopterVehicleId == -1 )
	{
		return( FALSE );
	}

	// figure out over which sector the helicopter APPEARS to be to the player (because we slide it smoothly across the
	// map, unlike groups travelling on the ground, it can appear over its next sector while it's not there yet.

	pGroup = GetGroup( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );
	Assert( pGroup );

	if ( pGroup->fBetweenSectors )
	{
		// this came up in one bug report!
		Assert( pGroup->uiTraverseTime != -1 );

		if( ( pGroup->uiTraverseTime > 0 ) && ( pGroup->uiTraverseTime != 0xffffffff ) )
		{
			flRatio = ( pGroup->uiTraverseTime - pGroup->uiArrivalTime + GetWorldTotalMin() ) / ( float ) pGroup->uiTraverseTime;
		}

		// if more than halfway there, the chopper appears more over the next sector, not over its current one(!)
		if ( flRatio > 0.5 )
		{
			fHelicopterOverNextSector = TRUE;
		}
	}

	if ( fHelicopterOverNextSector )
	{
		// use the next sector's coordinates
		sSectorX = pGroup->ubNextX;
		sSectorY = pGroup->ubNextY;
	}
	else
	{
		// use current sector's coordinates
		sSectorX = pVehicleList[ iHelicopterVehicleId ].sSectorX;
		sSectorY = pVehicleList[ iHelicopterVehicleId ].sSectorY;
	}

	// check if helicopter appears where he clicked
	if( ( sSectorX != sClickedSectorX ) || ( sSectorY != sClickedSectorY ) )
	{
		return( FALSE );
	}

	return( TRUE );
}


void BlitMineIcon( INT16 sMapX, INT16 sMapY )
{
	HVOBJECT hHandle;
	UINT32 uiDestPitchBYTES;
	UINT8 *pDestBuf2;
	INT16 sScreenX, sScreenY;
	
	GetVideoObject( &hHandle, guiMINEICON );

	pDestBuf2 = LockVideoSurface( guiSAVEBUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, MAP_VIEW_START_X+MAP_GRID_X - 1, MAP_VIEW_START_Y+MAP_GRID_Y - 1, MAP_VIEW_WIDTH+1,MAP_VIEW_HEIGHT-9 );
	UnLockVideoSurface(guiSAVEBUFFER);

	GetScreenXYFromMapXY( ( INT16 )( sMapX ), ( INT16 )( sMapY ), &sScreenX, &sScreenY );
	// when not zoomed, the x,y returned is the top left CORNER of the map square in question
	BltVideoObject( guiSAVEBUFFER, hHandle, 1, sScreenX + MAP_GRID_X / 4, sScreenY + MAP_GRID_Y / 4, VO_BLT_SRCTRANSPARENCY, NULL );
}


void BlitMineText( INT16 sMapX, INT16 sMapY )
{
	INT16 sScreenX, sScreenY;
	CHAR16 wString[ 32 ], wSubString[ 32 ];
	UINT8 ubMineIndex;
	UINT8 ubLineCnt = 0;

	GetScreenXYFromMapXY( ( INT16 )( sMapX ), ( INT16 )( sMapY ), &sScreenX, &sScreenY );

	// set coordinates for start of mine text
	sScreenX += MAP_GRID_X / 2;			// centered around middle of mine square
	sScreenY += MAP_GRID_Y + 1;			// slightly below

	// show detailed mine info (name, production rate, daily production)

	SetFontDestBuffer( guiSAVEBUFFER, MAP_VIEW_START_X, MAP_VIEW_START_Y, MAP_VIEW_START_X+MAP_VIEW_WIDTH+MAP_GRID_X, MAP_VIEW_START_Y+MAP_VIEW_HEIGHT+7, FALSE );

	// HEADROCK HAM 5: Variable Font
	INT32 MapMineLabelsFont;
	if (iResolution <= _800x600 )
	{
		MapMineLabelsFont = MAP_FONT;
	}
	else
	{
		MapMineLabelsFont = FONT14ARIAL;
	}

	SetFont(MapMineLabelsFont);
	SetFontForeground( FONT_LTGREEN );
	SetFontBackground( FONT_BLACK );

	ubMineIndex = GetMineIndexForSector( sMapX, sMapY );

	// display associated town name, followed by "mine"
	//swprintf( wString, L"%s %s", pTownNames[ GetTownAssociatedWithMine( GetMineIndexForSector( sMapX, sMapY ) ) ],  pwMineStrings[ 0 ] );
	swprintf( wString, L"%s %s", pTownNames[ GetTownAssociatedWithMine( GetMineIndexForSector( sMapX, sMapY ) ) ],  MineralsName[gMineStatus[ubMineIndex].ubMineType].sType );
	AdjustXForLeftMapEdge(wString, &sScreenX, MapMineLabelsFont);
	mprintf( ( sScreenX - StringPixLength( wString, MapMineLabelsFont ) / 2 ) , sScreenY + ubLineCnt * GetFontHeight( MapMineLabelsFont ) , wString );
	++ubLineCnt;

	// check if mine is empty (abandoned) or running out
	if (gMineStatus[ ubMineIndex ].fEmpty)
	{
		swprintf( wString, L"%s", pwMineStrings[ 5 ] );
		AdjustXForLeftMapEdge(wString, &sScreenX, MapMineLabelsFont);
		mprintf( ( sScreenX - StringPixLength( wString, MapMineLabelsFont ) / 2 ) , sScreenY + ubLineCnt * GetFontHeight( MapMineLabelsFont ) , wString );
		++ubLineCnt;
	}
	else if (gMineStatus[ ubMineIndex ].fShutDown)
	{
		swprintf( wString, L"%s", pwMineStrings[ 6 ] );
		AdjustXForLeftMapEdge(wString, &sScreenX, MapMineLabelsFont);
		mprintf( ( sScreenX - StringPixLength( wString, MapMineLabelsFont ) / 2 ) , sScreenY + ubLineCnt * GetFontHeight( MapMineLabelsFont ) , wString );
		++ubLineCnt;
	}
	else if (gMineStatus[ ubMineIndex ].fRunningOut)
	{
		swprintf( wString, L"%s", pwMineStrings[ 7 ] );
		AdjustXForLeftMapEdge(wString, &sScreenX, MapMineLabelsFont);
		mprintf( ( sScreenX - StringPixLength( wString, MapMineLabelsFont ) / 2 ) , sScreenY + ubLineCnt * GetFontHeight( MapMineLabelsFont ) , wString );
		++ubLineCnt;
	}


	// only show production if player controls it and it's actually producing
	if (PlayerControlsMine(ubMineIndex) && !gMineStatus[ ubMineIndex ].fEmpty)
	{
		// show current production
		swprintf( wSubString, L"%d", PredictDailyIncomeFromAMine(ubMineIndex, TRUE) );
		InsertCommasForDollarFigure( wSubString );
		InsertDollarSignInToString( wSubString );
		wcscpy( wString, wSubString );

/*
		// show maximum potential production
		swprintf( wSubString, L"%d", GetMaxDailyRemovalFromMine(ubMineIndex) );
		InsertCommasForDollarFigure( wSubString );
		InsertDollarSignInToString( wSubString );
		wcscat( wString, L" / ");
		wcscat( wString, wSubString );
*/

		// if potential is not nil, show percentage of the two
		if (GetMaxPeriodicRemovalFromMine(ubMineIndex) > 0)
		{
			#ifdef CHINESE
				swprintf( wSubString, L" (%d%ге%%)", (PredictDailyIncomeFromAMine(ubMineIndex, TRUE) * 100 ) / GetMaxDailyRemovalFromMine(ubMineIndex) );
			#else
				swprintf( wSubString, L" (%d%%%%)", (PredictDailyIncomeFromAMine(ubMineIndex, TRUE) * 100 ) / GetMaxDailyRemovalFromMine(ubMineIndex) );
			#endif
			
			wcscat( wString, wSubString );
		}

		AdjustXForLeftMapEdge(wString, &sScreenX, MapMineLabelsFont);
		mprintf( ( sScreenX - StringPixLengthArg( MapMineLabelsFont, wcslen(wString), wString ) / 2 ) , sScreenY + ubLineCnt * GetFontHeight( MapMineLabelsFont ), wString );
		++ubLineCnt;
	}

	SetFontDestBuffer( FRAME_BUFFER, MAP_VIEW_START_X, MAP_VIEW_START_Y, MAP_VIEW_START_X+MAP_VIEW_WIDTH+MAP_GRID_X, MAP_VIEW_START_Y+MAP_VIEW_HEIGHT+7, FALSE );
}


// HEADROCK HAM 5: Now takes argument for font.
void AdjustXForLeftMapEdge(STR16 wString, INT16 *psX, INT32 iFont)
{
	INT16 sStartingX, sPastEdge;

	sStartingX = *psX - (StringPixLengthArg( iFont, wcslen(wString), wString ) / 2);
	sPastEdge = (MAP_VIEW_START_X + 23) - sStartingX;

	if (sPastEdge > 0)
		*psX += sPastEdge;
}


void BlitTownGridMarkers( void )
{
	INT16 sScreenX = 0, sScreenY = 0;
	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;
	UINT16 usColor = 0;
	INT32 iCounter = 0;
	INT16 sWidth = 0, sHeight = 0;

	// get 16 bpp color
	usColor = Get16BPPColor( FROMRGB( 100, 100, 100) );
	
	// blit in the highlighted sector
	pDestBuf = LockVideoSurface( guiSAVEBUFFER, &uiDestPitchBYTES );

	// clip to view region
	ClipBlitsToMapViewRegionForRectangleAndABit( uiDestPitchBYTES );

	// go through list of towns and place on screen
	while( pTownNamesList[ iCounter ] != 0 )
	{
		// skip Orta/Tixa until found
		//if( ( ( fFoundOrta != FALSE ) || ( pTownNamesList[ iCounter ] != ORTA ) ) && ( ( pTownNamesList[ iCounter ] != TIXA ) || ( fFoundTixa != FALSE) ) )
		if( gfHiddenTown[ pTownNamesList[ iCounter ] ] == TRUE )
		{
			// get location on screen
			GetScreenXYFromMapXY( ( INT16 )( pTownLocationsList[ iCounter ] % MAP_WORLD_X ), ( INT16 )( pTownLocationsList[ iCounter ] / MAP_WORLD_X ), &sScreenX, &sScreenY );
			sWidth = MAP_GRID_X - 1;
			sHeight= MAP_GRID_Y;

			sScreenX += 2;

			if( StrategicMap[ pTownLocationsList[ iCounter ] - MAP_WORLD_X ].bNameId != StrategicMap[ pTownLocationsList[ iCounter ] ].bNameId )
			{
				LineDraw( TRUE,  sScreenX - 1, sScreenY - 1, sScreenX + sWidth - 1, sScreenY - 1, usColor, pDestBuf );
			}

			if( ( StrategicMap[ pTownLocationsList[ iCounter ] + MAP_WORLD_X ].bNameId != StrategicMap[ pTownLocationsList[ iCounter ] ].bNameId ) /* || ( StrategicMap[ pTownLocationsList[ iCounter ] + MAP_WORLD_X ].bNameId == PALACE ) */ )
			{
				LineDraw( TRUE,  sScreenX - 1, sScreenY + sHeight - 1, sScreenX + sWidth - 1, sScreenY + sHeight - 1, usColor, pDestBuf );
			}

			if( StrategicMap[ pTownLocationsList[ iCounter ] - 1 ].bNameId != StrategicMap[ pTownLocationsList[ iCounter ] ].bNameId )
			{
				LineDraw( TRUE,  sScreenX - 2, sScreenY - 1, sScreenX - 2, sScreenY + sHeight - 1, usColor, pDestBuf );
			}

			if( StrategicMap[ pTownLocationsList[ iCounter ] + 1 ].bNameId != StrategicMap[ pTownLocationsList[ iCounter ] ].bNameId )
			{
				LineDraw( TRUE,  sScreenX + sWidth - 1, sScreenY - 1, sScreenX + sWidth - 1, sScreenY + sHeight - 1, usColor, pDestBuf );
			}
		}

		++iCounter;
	}

	// restore clips
	RestoreClipRegionToFullScreenForRectangle( uiDestPitchBYTES );

	// unlock surface
	UnLockVideoSurface( guiSAVEBUFFER );
}


void BlitMineGridMarkers( void )
{
	INT16 sScreenX = 0, sScreenY = 0;
	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;
	UINT16 usColor = 0;
	INT32 iCounter = 0;
	INT16 sWidth = 0, sHeight = 0;

	// get 16 bpp color
	usColor = Get16BPPColor( FROMRGB( 100, 100, 100) );

	// blit in the highlighted sector
	pDestBuf = LockVideoSurface( guiSAVEBUFFER, &uiDestPitchBYTES );

	// clip to view region
	ClipBlitsToMapViewRegionForRectangleAndABit( uiDestPitchBYTES );

	for( iCounter = 0; iCounter < MAX_NUMBER_OF_MINES; ++iCounter )
	{
		// get location on screen
		//GetScreenXYFromMapXY( ( INT16 )(  gMineLocation[ iCounter ].sSectorX ), ( INT16 )(  gMineLocation[ iCounter ].sSectorY ), &sScreenX, &sScreenY );
		GetScreenXYFromMapXY( ( INT16 )(  gMineStatus[ iCounter ].sSectorX ), ( INT16 )(  gMineStatus[ iCounter ].sSectorY ), &sScreenX, &sScreenY );
		sWidth = MAP_GRID_X;
		sHeight= MAP_GRID_Y;

		// draw rectangle
		RectangleDraw( TRUE, sScreenX, sScreenY - 1, sScreenX + sWidth, sScreenY + sHeight - 1, usColor, pDestBuf );
	}

	// restore clips
	RestoreClipRegionToFullScreenForRectangle( uiDestPitchBYTES );

	// unlock surface
	UnLockVideoSurface( guiSAVEBUFFER );
}


/*
void CheckIfAnyoneLeftInSector( INT16 sX, INT16 sY, INT16 sNewX, INT16 sNewY, INT8 bZ )
{
	// if this map sector is currently selected
	if( ( sX == sSelMapX ) && ( sY == sSelMapY ) && ( bZ == iCurrentMapSectorZ ) )
	{
		// check if anyone left in the old sector
		if( NumFriendlyInSector( (INT8)sX, (INT8)sY, bZ ) == 0)
		{
			ChangeSelectedMapSector( sNewX, sNewY, bZ );
			fMapScreenBottomDirty = TRUE;
		}
	}

	return;
}


UINT8 NumFriendlyInSector( INT16 sX, INT16 sY, INT8 bZ )
{
	SOLDIERTYPE *pTeamSoldier;
	INT32				cnt = 0;
	UINT8				ubNumFriendlies = 0;

	// Check if the battle is won!
	// Loop through all mercs and make go
	for ( pTeamSoldier = Menptr, cnt = 0; cnt < TOTAL_SOLDIERS; pTeamSoldier++, cnt++ )
	{
		if ( pTeamSoldier->bActive && pTeamSoldier->stats.bLife > 0 )
		{
			if ( (pTeamSoldier->bSide == gbPlayerNum ) && ( pTeamSoldier->sSectorX == sX ) && ( pTeamSoldier->sSectorY == sY ) && ( pTeamSoldier->bSectorZ == bZ ) )
			{
				ubNumFriendlies++;
			}
		}
	}

	return( ubNumFriendlies );

}
*/


void DisplayLevelString( void )
{
	CHAR16 sString[ 32 ];

	// given the current level being displayed on the map, show a sub level message

	// at the surface
	if( !iCurrentMapSectorZ )
	{
		return;
	}

	SetFontDestBuffer( guiSAVEBUFFER, xResOffset + MAP_VIEW_START_X, MAP_VIEW_START_Y, xResOffset + MAP_VIEW_START_X+MAP_VIEW_WIDTH+MAP_GRID_X, MAP_VIEW_START_Y+MAP_VIEW_HEIGHT+7, FALSE );

	SetFont( MAP_FONT );
	SetFontForeground( MAP_INDEX_COLOR );
	SetFontBackground( FONT_BLACK );
	swprintf( sString, L"%s %d", sMapLevelString[ 0 ], iCurrentMapSectorZ );

	mprintf(  MAP_LEVEL_STRING_X, MAP_LEVEL_STRING_Y, sString  );

	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
}


// function to manipulate the number of towns people on the cursor
BOOLEAN PickUpATownPersonFromSector( UINT8 ubType, INT16 sX, INT16 sY )
{
	// see if there are any militia of this type in this sector
	if( !SectorInfo[ SECTOR( sX, sY ) ].ubNumberOfCivsAtLevel[ ubType ] )
	{
		// failed, no one here
		return( FALSE );
	}

	// are they in the same town as they were pickedup from
	if( GetTownIdForSector( sX, sY ) != sSelectedMilitiaTown )
	{
		return( FALSE );
	}

	if( !SectorOursAndPeaceful( sX, sY, 0 ) )
	{
		return( FALSE );
	}

	if( sX == gWorldSectorX && sY == gWorldSectorY )
	{
		gfStrategicMilitiaChangesMade = TRUE;
	}

	// the number of units transferred
	INT16 countMovedCivs = 1;
	// all units in sector
	if (_KeyDown(SHIFT))
		countMovedCivs = SectorInfo[ SECTOR( sX, sY )].ubNumberOfCivsAtLevel[ ubType ];
	// 5 units in sector
	else if (_KeyDown(CTRL))
		countMovedCivs = min(5, SectorInfo[ SECTOR( sX, sY )].ubNumberOfCivsAtLevel[ ubType ]);

	// otherwise pick this guy up
	switch( ubType )
	{
		case( GREEN_MILITIA ):
			sGreensOnCursor += countMovedCivs;
		break;
		case( REGULAR_MILITIA ):
			sRegularsOnCursor += countMovedCivs;
		break;
		case( ELITE_MILITIA ):
			sElitesOnCursor += countMovedCivs;
		break;
	}

	// reduce number in this sector
	SectorInfo[ SECTOR( sX, sY )].ubNumberOfCivsAtLevel[ ubType ] -= countMovedCivs;

	fMapPanelDirty = TRUE;

	return( TRUE );
}

BOOLEAN DropAPersonInASector( UINT8 ubType, INT16 sX, INT16 sY )
{
	INT32 iMaxMilitiaPerSector = gGameExternalOptions.iMaxMilitiaPerSector;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Map Screen2");
	// are they in the same town as they were pickedup from
	if( GetTownIdForSector( sX, sY ) != sSelectedMilitiaTown )
	{
		return( FALSE );
	}

	// to move a group of militia
	// Flugente: we have to account for militia in groups!
	INT32 countFreeCivPlace = iMaxMilitiaPerSector - NumNonPlayerTeamMembersInSector( sX, sY, MILITIA_TEAM ); 

	if( countFreeCivPlace <= 0 )
	{
		return( FALSE );
	}

	if( !SectorOursAndPeaceful(sX, sY, 0 ) )
	{
		return( FALSE );
	}

	if( sX == gWorldSectorX && sY == gWorldSectorY )
	{
		gfStrategicMilitiaChangesMade = TRUE;
	}

	// the number of units transferred
	INT16 countMovedCivs = 1;

	// drop the guy into this sector
	switch( ubType )
	{
		case( GREEN_MILITIA ):

			if( !sGreensOnCursor )
			{
				return( FALSE );
			}
			
			// drop max allowable units
			if (_KeyDown(SHIFT)) 
			{
				if (countFreeCivPlace <= sGreensOnCursor)
					countMovedCivs = countFreeCivPlace;
				else 
					countMovedCivs = sGreensOnCursor;
			}
			// drop 5 units
			else if (_KeyDown(CTRL))
			{
				if (countFreeCivPlace <= 5)
					countMovedCivs = min(countFreeCivPlace, sGreensOnCursor);
				else 
					countMovedCivs = min(5, sGreensOnCursor);
			}
			sGreensOnCursor -= countMovedCivs;
		break;
		case( REGULAR_MILITIA ):
			if( !sRegularsOnCursor )
			{
				return( FALSE );
			}

			// drop max allowable units
			if (_KeyDown(SHIFT)) 
			{
				if (countFreeCivPlace <= sRegularsOnCursor)
					countMovedCivs = countFreeCivPlace;
				else
					countMovedCivs = sRegularsOnCursor;
			}
			// drop 5 units
			else if (_KeyDown(CTRL))
			{
				if (countFreeCivPlace <= 5)
					countMovedCivs = min(countFreeCivPlace, sRegularsOnCursor);
				else 
					countMovedCivs = min(5, sRegularsOnCursor);
			}

			sRegularsOnCursor -= countMovedCivs;
		break;
		case( ELITE_MILITIA ):
			if( !sElitesOnCursor )
			{
				return( FALSE );
			}

			// drop max allowable units
			if (_KeyDown(SHIFT)) 
			{
				if (countFreeCivPlace <= sElitesOnCursor)
					countMovedCivs = countFreeCivPlace;
				else
					countMovedCivs = sElitesOnCursor;
			}
			// drop 5 units
			else if (_KeyDown(CTRL))
			{
				if (countFreeCivPlace <= 5)
					countMovedCivs = min(countFreeCivPlace, sElitesOnCursor);
				else 
					countMovedCivs = min(5, sElitesOnCursor);
			}

			sElitesOnCursor -= countMovedCivs;
		break;
	}

	// up the number in this sector of this type of militia
	SectorInfo[ SECTOR( sX, sY )].ubNumberOfCivsAtLevel[ ubType ] += countMovedCivs;

	fMapPanelDirty = TRUE;

	return( TRUE );
}



BOOLEAN LoadMilitiaPopUpBox( void )
{
	VOBJECT_DESC	 VObjectDesc;

	// load the militia pop up box
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

	if (iResolution >= _640x480 && iResolution < _800x600)
		FilenameForBPP("INTERFACE\\Militia.sti", VObjectDesc.ImageFile);
	else if (iResolution < _1024x768)
		FilenameForBPP("INTERFACE\\Militia_800x600.sti", VObjectDesc.ImageFile);
	else
		FilenameForBPP("INTERFACE\\Militia_1024x768.sti", VObjectDesc.ImageFile);

	CHECKF(AddVideoObject(&VObjectDesc, &guiMilitia));

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\Militiamaps.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiMilitiaMaps));

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\MilitiamapsectorOutline2.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiMilitiaSectorHighLight));

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\MilitiamapsectorOutline.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiMilitiaSectorOutline ));

	return( TRUE );
}


void RemoveMilitiaPopUpBox( void )
{
	// delete the militia pop up box graphic
	DeleteVideoObjectFromIndex( guiMilitia );
	DeleteVideoObjectFromIndex( guiMilitiaMaps );
	DeleteVideoObjectFromIndex( guiMilitiaSectorHighLight );
	DeleteVideoObjectFromIndex( guiMilitiaSectorOutline );
}

BOOLEAN DrawMilitiaPopUpBox( void )
{
	HVOBJECT						hVObject;
	ETRLEObject						*pTrav;

	if( !fShowMilitia )
	{
		sSelectedMilitiaTown = 0;
	}

	// create buttons
	CreateDestroyMilitiaSectorButtons( );

	// create mouse regions if we need to
	CreateDestroyMilitiaPopUPRegions( );

	if( !sSelectedMilitiaTown )
	{
		return( FALSE );
	}

	// update states of militia selected sector buttons
	CheckAndUpdateStatesOfSelectedMilitiaSectorButtons( );

	// get the properties of the militia object
	GetVideoObject( &hVObject, guiMilitia );

	BltVideoObject( FRAME_BUFFER, hVObject, POPUP_FRAMEBORDER, MAP_MILITIA_BOX_POS_X, MAP_MILITIA_BOX_POS_Y, VO_BLT_SRCTRANSPARENCY, NULL );

	GetVideoObject( &hVObject, guiMilitiaMaps );
	BltVideoObject( FRAME_BUFFER, hVObject, ( UINT16 )( sSelectedMilitiaTown - 1 ), MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X, MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_MAP_Y, VO_BLT_SRCTRANSPARENCY, NULL );

	// set font color for labels and "total militia" counts
	SetFontForeground(FONT_WHITE);

	// draw name of town, and the "unassigned" label
	DrawTownMilitiaName( );

	// render the icons for each sector in the town
	RenderIconsPerSectorForSelectedTown( );

	// shade any sectors not under our control
	RenderShadingForUnControlledSectors( );

	// display anyone picked up
	DisplayUnallocatedMilitia( );

	// draw the highlight last
	ShowHighLightedSectorOnMilitiaMap( );

	GetVideoObject( &hVObject, guiMilitia );
	pTrav = &(hVObject->pETRLEObject[ 0 ] );

	InvalidateRegion( MAP_MILITIA_BOX_POS_X, MAP_MILITIA_BOX_POS_Y, MAP_MILITIA_BOX_POS_X + pTrav->usWidth, MAP_MILITIA_BOX_POS_Y + pTrav->usHeight );

	// set the text for the militia map sector info buttons
	SetMilitiaMapButtonsText( );

	// render buttons
	MarkButtonsDirty( );

	return( TRUE );
}



void CreateDestroyMilitiaPopUPRegions( void )
{
	static INT16 sOldTown = 0;
	INT32 iCounter = 0;

	// create destroy militia pop up regions for mapscreen militia pop up box
	if( sSelectedMilitiaTown != 0 )
	{
		sOldTown = sSelectedMilitiaTown;
	}

	if( fShowMilitia && sSelectedMilitiaTown && !gfMilitiaPopupCreated )
	{
		for( iCounter = 0; iCounter < MILITIA_BOXREGIONS; ++iCounter )
		{
			MSYS_DefineRegion( &gMapScreenMilitiaBoxRegions[ iCounter ], ( INT16 ) ( MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X + ( iCounter % MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_WIDTH ), ( INT16 )( MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_MAP_Y + ( iCounter / MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_HEIGHT ), ( INT16 )( MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X + ( ( ( iCounter  ) % MILITIA_BOX_ROWS ) + 1 ) * MILITIA_BOX_BOX_WIDTH ), ( INT16 )( MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_MAP_Y + ( ( ( iCounter ) / MILITIA_BOX_ROWS ) + 1 ) * MILITIA_BOX_BOX_HEIGHT ), MSYS_PRIORITY_HIGHEST - 3,
							MSYS_NO_CURSOR, MilitiaRegionMoveCallback, MilitiaRegionClickCallback );

			MSYS_SetRegionUserData( &gMapScreenMilitiaBoxRegions[ iCounter ], 0, iCounter );
		}

		// create militia panel buttons
		CreateMilitiaPanelBottomButton( );
		
		gfMilitiaPopupCreated = TRUE;
	}
	else if( gfMilitiaPopupCreated  && ( !fShowMilitia || !sSelectedMilitiaTown ) )
	{
		for( iCounter = 0; iCounter < MILITIA_BOXREGIONS; ++iCounter )
		{
			// remove region
			MSYS_RemoveRegion( &gMapScreenMilitiaBoxRegions[ iCounter ] );
		}

		// handle the shutdown of the panel...there maybe people on the cursor, distribute them evenly over all the sectors
		HandleShutDownOfMilitiaPanelIfPeopleOnTheCursor( sOldTown );

		DeleteMilitiaPanelBottomButton( );

		gfMilitiaPopupCreated = FALSE;
	}
}

void RenderIconsPerSectorForSelectedTown( void )
{
	INT16 sBaseSectorValue = 0;
	INT16 sCurrentSectorValue = 0;
	INT32 iCounter = 0;
	INT32 iNumberOfGreens = 0;
	INT32 iNumberOfRegulars = 0;
	INT32 iNumberOfElites = 0;
	INT32 iTotalNumberOfTroops = 0;
	INT32 iCurrentTroopIcon = 0;
	INT32 iTotalNumberOfIcons = 0;
	HVOBJECT hVObject;
	UINT16 usIconValue = 0;
	INT16 sX, sY;
	CHAR16 sString[ 32 ];
	INT16 sSectorX = 0, sSectorY = 0;
	UINT8 ubGreen, ubRegular, ubElite, ub10xGreen, ub10xRegular, ub10xElite;

	// get the sector value for the upper left corner
	sBaseSectorValue = GetBaseSectorForCurrentTown( );

	// get militia video object
	GetVideoObject( &hVObject, guiMilitia );

	// render icons for map
	for ( iCounter = 0; iCounter < MILITIA_BOX_ROWS * MILITIA_BOX_ROWS; ++iCounter )
	{
		// grab current sector value
		sCurrentSectorValue = sBaseSectorValue + ( ( iCounter % MILITIA_BOX_ROWS ) + ( iCounter / MILITIA_BOX_ROWS ) * ( 16 ) );
		
		// skip sectors that's beyond sector P16
		if( sCurrentSectorValue > SEC_P16 )
		{
			continue;
		}

		sSectorX = SECTORX( sCurrentSectorValue );
		sSectorY = SECTORY( sCurrentSectorValue );

		// skip sectors not in the selected town (nearby other towns or wilderness SAM Sites)
		if( GetTownIdForSector( sSectorX, sSectorY ) != sSelectedMilitiaTown )
		{
			continue;
		}

		// get number of each
		iNumberOfGreens = SectorInfo[ sCurrentSectorValue ].ubNumberOfCivsAtLevel[ GREEN_MILITIA ];
		iNumberOfRegulars = SectorInfo[ sCurrentSectorValue ].ubNumberOfCivsAtLevel[ REGULAR_MILITIA ];
		iNumberOfElites = SectorInfo[ sCurrentSectorValue ].ubNumberOfCivsAtLevel[ ELITE_MILITIA ];

		// get total
		iTotalNumberOfTroops = iNumberOfGreens + iNumberOfRegulars + iNumberOfElites;

		// printf number of troops
		SetFont( FONT10ARIAL );
		swprintf( sString, L"%d", iTotalNumberOfTroops );
		FindFontRightCoordinates( ( INT16 )( MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X + ( ( iCounter % MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_WIDTH ) ), ( INT16 ) ( MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_MAP_Y + ( ( iCounter / MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_HEIGHT ) ),  MILITIA_BOX_BOX_WIDTH, 0, sString, FONT10ARIAL, &sX, &sY );

		if( StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( sCurrentSectorValue ) ].bNameId != BLANK_SECTOR &&
				!StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( sCurrentSectorValue ) ].fEnemyControlled )
		{
			if( sSectorMilitiaMapSector != iCounter )
			{
				mprintf( sX, ( INT16 )( sY + MILITIA_BOX_BOX_HEIGHT - 5), sString );
			}
			else
			{
				mprintf( sX - 15, ( INT16 )( sY + MILITIA_BOX_BOX_HEIGHT - 5), sString );
			}
		}

		// get cumulative icons
		ub10xElite = iNumberOfElites / 10;
		ubElite = iNumberOfElites % 10 + ub10xElite;
		ub10xRegular = iNumberOfRegulars / 10 + ubElite;
		ubRegular = iNumberOfRegulars % 10 + ub10xRegular;
		ub10xGreen = iNumberOfGreens / 10 + ubRegular;
		ubGreen = iNumberOfGreens % 10 + ub10xGreen;
		iTotalNumberOfIcons = ubGreen;

		// now display
		for( iCurrentTroopIcon = 0; iCurrentTroopIcon < iTotalNumberOfIcons; ++iCurrentTroopIcon )
		{
			sX =  ( iCurrentTroopIcon % POPUP_MILITIA_ICONS_PER_ROW ) * MEDIUM_MILITIA_ICON_SPACING + MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X + ( ( iCounter % MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_WIDTH ) + 3;
			sY =  ( iCurrentTroopIcon / POPUP_MILITIA_ICONS_PER_ROW ) * ( MEDIUM_MILITIA_ICON_SPACING ) + MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_MAP_Y + ( ( iCounter / MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_HEIGHT ) + 3;
			
			if( iCurrentTroopIcon < ub10xElite )
			{
				usIconValue = POPUP_BLUE_10X_BOX;
			}
			else if( iCurrentTroopIcon < ubElite )
			{
				usIconValue = POPUP_BLUE_BOX;
			}
			// then draw regulars
			else if( iCurrentTroopIcon < ub10xRegular )
			{
				usIconValue = POPUP_CYAN_10X_BOX;
			}
			else if( iCurrentTroopIcon < ubRegular )
			{
				usIconValue = POPUP_CYAN_BOX;
			}
			// last draw greens
			else if( iCurrentTroopIcon < ub10xGreen )
			{
				usIconValue = POPUP_GREEN_10X_BOX;
			}
			else
			{
				usIconValue = POPUP_GREEN_BOX;
			}

			BltVideoObject( FRAME_BUFFER, hVObject, usIconValue, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}
}


INT16 GetBaseSectorForCurrentTown( void )
{
	INT16 sBaseSector = 0;

	// is the current town
	if( sSelectedMilitiaTown != 0 )
	{
		sBaseSector = sBaseSectorList[ ( INT16 )( sSelectedMilitiaTown - 1 ) ];
	}

	// return the current sector value
	return( sBaseSector );
}

void ShowHighLightedSectorOnMilitiaMap( void )
{
	// show the highlighted sector on the militia map
	HVOBJECT hVObject;
	INT16 sX = 0, sY = 0;

	if( sSectorMilitiaMapSector != -1 )
	{
		sX = MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X + ( ( sSectorMilitiaMapSector % MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_WIDTH );
		sY = MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_MAP_Y + ( ( sSectorMilitiaMapSector / MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_HEIGHT );

		// get the object
		GetVideoObject( &hVObject, guiMilitiaSectorHighLight );

		// blt the object
		BltVideoObject( FRAME_BUFFER, hVObject, 0, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if( sSectorMilitiaMapSectorOutline != -1 )
	{
		sX = MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X + ( ( sSectorMilitiaMapSectorOutline % MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_WIDTH );
		sY = MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_MAP_Y + ( ( sSectorMilitiaMapSectorOutline / MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_HEIGHT );

		// get the object
		GetVideoObject( &hVObject, guiMilitiaSectorOutline );

		// blt the object
		BltVideoObject( FRAME_BUFFER, hVObject, 0, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
	}
}


void MilitiaRegionClickCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = 0;

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if( ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP ) )
	{
		if( IsThisMilitiaTownSectorAllowable( (INT16)iValue ) )
		{
			if( sSectorMilitiaMapSector == (INT16)iValue )
			{
				sSectorMilitiaMapSector = -1;
			}
			else
			{
				sSectorMilitiaMapSector = ( INT16 )iValue;
			}
		}
		else
		{
			sSectorMilitiaMapSector = -1;
		}
	}

	if( ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP ) )
	{
		sSectorMilitiaMapSector = -1;
	}
}


void MilitiaRegionMoveCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = 0;

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if( ( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE ) )
	{
		if( IsThisMilitiaTownSectorAllowable( ( INT16 )iValue ) )
		{
			sSectorMilitiaMapSectorOutline = ( INT16 )iValue;
		}
		else
		{
			sSectorMilitiaMapSectorOutline = -1;
		}
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		sSectorMilitiaMapSectorOutline = -1;
	}
}

void CreateDestroyMilitiaSectorButtons( void )
{
	static BOOLEAN fCreated = FALSE;
	static INT16 sOldSectorValue = -1;
	INT16 sX = 0, sY = 0;
	INT32 iCounter = 0;
	HVOBJECT						hVObject;
	ETRLEObject						*pTrav;

	if( sOldSectorValue == sSectorMilitiaMapSector && fShowMilitia && sSelectedMilitiaTown && !fCreated && sSectorMilitiaMapSector != -1 )
	{
		fCreated = TRUE;

		// given sector..place down the 3 buttons

		for( iCounter = 0; iCounter < 3; ++iCounter )
		{
			// set screen x and y positions
			sX = MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X + ( ( sSectorMilitiaMapSector % MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_WIDTH );
			sY = MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_MAP_Y + ( ( sSectorMilitiaMapSector / MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_HEIGHT );

			// adjust offsets
			sX += MILITIA_BTN_OFFSET_X;
			sY += ( iCounter * ( MILITIA_BTN_HEIGHT ) + 2 );

			// set the button image
			if (iResolution >= _640x480 && iResolution < _800x600)
				giMapMilitiaButtonImage[ iCounter ]=  LoadButtonImage( "INTERFACE\\militia.sti", -1,SMALL_NORMAL_BUTTON,-1,SMALL_PRESSED_BUTTON,-1 );
			else if (iResolution < _1024x768)
				giMapMilitiaButtonImage[ iCounter ]=  LoadButtonImage( "INTERFACE\\militia_800x600.sti", -1,SMALL_NORMAL_BUTTON,-1,SMALL_PRESSED_BUTTON,-1 );
			else
				giMapMilitiaButtonImage[ iCounter ]=  LoadButtonImage( "INTERFACE\\militia_1024x768.sti", -1,SMALL_NORMAL_BUTTON,-1,SMALL_PRESSED_BUTTON,-1 );

			// set the button value
			giMapMilitiaButton[ iCounter ] = QuickCreateButton( giMapMilitiaButtonImage[ iCounter ], sX, sY,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)MilitiaButtonCallback);

			// set button user data
			MSYS_SetBtnUserData( 	giMapMilitiaButton[ iCounter ], 0, iCounter );
			SpecifyButtonFont( giMapMilitiaButton[ iCounter ], FONT10ARIAL );
			SpecifyButtonUpTextColors( giMapMilitiaButton[ iCounter ], gsMilitiaSectorButtonColors[ iCounter ], FONT_BLACK );
			SpecifyButtonDownTextColors( giMapMilitiaButton[ iCounter ], gsMilitiaSectorButtonColors[ iCounter ], FONT_BLACK );

			GetVideoObject( &hVObject, guiMilitia );
			pTrav = &(hVObject->pETRLEObject[ 0 ] );

			SetButtonFastHelpText( giMapMilitiaButton[ iCounter ], pMilitiaButtonsHelpText[ iCounter ] );
		}

		// mark here the militia box left click region
		//MSYS_DefineRegion( &gMapScreenMilitiaRegion, ( INT16 ) ( MAP_MILITIA_BOX_POS_X ), ( INT16 )( MAP_MILITIA_BOX_POS_Y  ), ( INT16 )( MAP_MILITIA_BOX_POS_X + pTrav->usWidth ), ( INT16 )( MAP_MILITIA_BOX_POS_Y + pTrav->usHeight ), MSYS_PRIORITY_HIGHEST - 2,
			//				MSYS_NO_CURSOR, MilitiaRegionMoveCallback, MilitiaBoxMaskBtnCallback );

		CreateScreenMaskForMoveBox( );

		// ste the fact that the buttons were in fact created
		fMilitiaMapButtonsCreated = TRUE;
	}
	else if( fCreated && ( sOldSectorValue != sSectorMilitiaMapSector || !fShowMilitia || !sSelectedMilitiaTown || sSectorMilitiaMapSector == -1 ) )
	{
		sOldSectorValue = sSectorMilitiaMapSector;
		fCreated = FALSE;

		// the militia box left click region
	//	MSYS_RemoveRegion( &gMapScreenMilitiaRegion );

		// get rid of the buttons
		for( iCounter = 0 ; iCounter < 3; iCounter++ )
		{
			RemoveButton( giMapMilitiaButton[ iCounter ] );
			UnloadButtonImage( giMapMilitiaButtonImage[ iCounter ] );
		}

		if( !fShowMilitia || !sSelectedMilitiaTown )
		{
			sSectorMilitiaMapSector = -1;
			sSelectedMilitiaTown = 0;
		}

		RemoveScreenMaskForMoveBox( );

		// set the fact that the buttons were destroyed
		fMilitiaMapButtonsCreated = FALSE;
	}

	sOldSectorValue = sSectorMilitiaMapSector;
}



void SetMilitiaMapButtonsText( void )
{
	// now set the militia map button text
	CHAR16 sString[ 64 ];
	INT32 iNumberOfGreens = 0, iNumberOfRegulars = 0, iNumberOfElites = 0;
	INT16 sBaseSectorValue = 0, sGlobalMapSector = 0;

	if( !fMilitiaMapButtonsCreated )
	{
		return;
	}

	// grab the appropriate global sector value in the world
	sBaseSectorValue = GetBaseSectorForCurrentTown( );
	sGlobalMapSector = sBaseSectorValue + ( ( sSectorMilitiaMapSector % MILITIA_BOX_ROWS ) + ( sSectorMilitiaMapSector / MILITIA_BOX_ROWS ) * ( 16 ) );

	// Flugente: we can only redistribute those militia that are not in any group
	iNumberOfGreens = MilitiaInSectorOfRankStationary( SECTORX( sGlobalMapSector ), SECTORY( sGlobalMapSector ), GREEN_MILITIA );
	iNumberOfRegulars = MilitiaInSectorOfRankStationary( SECTORX( sGlobalMapSector ), SECTORY( sGlobalMapSector ), REGULAR_MILITIA );
	iNumberOfElites = MilitiaInSectorOfRankStationary( SECTORX( sGlobalMapSector ), SECTORY( sGlobalMapSector ), ELITE_MILITIA );

	// the greens in this sector
	swprintf( sString, L"%d", iNumberOfGreens );
	SpecifyButtonText( giMapMilitiaButton[ 0 ], sString );

	// the regulars in this sector
	swprintf( sString, L"%d", iNumberOfRegulars );
	SpecifyButtonText( giMapMilitiaButton[ 1 ], sString );

	// the number of elites in this sector
	swprintf( sString, L"%d", iNumberOfElites );
	SpecifyButtonText( giMapMilitiaButton[ 2 ], sString );
}


void MilitiaButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT16 sGlobalMapSector = 0;
	INT16 sBaseSectorValue = 0;
	INT32 iValue = 0;

	// is the button enabled
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	// get the value
	iValue = MSYS_GetBtnUserData( btn, 0 );

	// get the sector value for the upper left corner
	sBaseSectorValue = GetBaseSectorForCurrentTown( );
	sGlobalMapSector = sBaseSectorValue + ( ( sSectorMilitiaMapSector % MILITIA_BOX_ROWS ) + ( sSectorMilitiaMapSector / MILITIA_BOX_ROWS ) * ( 16 ) );

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			DropAPersonInASector( ( UINT8 )( iValue ), ( INT16 )( ( sGlobalMapSector % 16 ) + 1 ), ( INT16 )( ( sGlobalMapSector / 16 ) + 1 )  );
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			PickUpATownPersonFromSector( ( UINT8 )( iValue ), ( INT16 )( ( sGlobalMapSector % 16 ) + 1 ), ( INT16 )( ( sGlobalMapSector / 16 ) + 1 )  );
		}
	}
}

void DisplayUnallocatedMilitia( void )
{
	// show the number on the cursor
	INT32 iTotalNumberOfIcons = 0, iNumberOfGreens = 0, iNumberOfRegulars = 0, iNumberOfElites = 0, iCurrentTroopIcon = 0;
	INT32 iCurrentIcon = 0;
	INT16 sX = 0, sY = 0;
	HVOBJECT hVObject;
	UINT8 ubGreen, ubRegular, ubElite, ub10xGreen, ub10xRegular, ub10xElite;
	UINT8 ubRow = 0;

	// get militia video object
	GetVideoObject( &hVObject, guiMilitia );

	// get number of each
	iNumberOfGreens = sGreensOnCursor;
	iNumberOfRegulars = sRegularsOnCursor;
	iNumberOfElites = sElitesOnCursor;
	
	// get number of green icons
	ub10xGreen = iNumberOfGreens / 10;
	ubGreen = iNumberOfGreens % 10;
	iTotalNumberOfIcons = ub10xGreen + ubGreen;

	// now display green icons
	for( iCurrentTroopIcon = 0; iCurrentTroopIcon < iTotalNumberOfIcons; ++iCurrentTroopIcon )
	{
		// get screen x and y coords
		sX =  ( iCurrentTroopIcon % NUMBER_OF_MILITIA_ICONS_PER_LOWER_ROW ) * MEDIUM_MILITIA_ICON_SPACING + MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X + 1;
		sY =  ( iCurrentTroopIcon / NUMBER_OF_MILITIA_ICONS_PER_LOWER_ROW ) * MEDIUM_MILITIA_ICON_SPACING + MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_LOWER_ROW_Y;

		if( iCurrentTroopIcon < ub10xGreen )
		{
			iCurrentIcon = POPUP_GREEN_10X_BOX;
		}
		else
		{
			iCurrentIcon = POPUP_GREEN_BOX;
		}

		BltVideoObject( FRAME_BUFFER, hVObject, ( UINT16 )( iCurrentIcon ), sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// offset row for regulars
	ubRow += iTotalNumberOfIcons / NUMBER_OF_MILITIA_ICONS_PER_LOWER_ROW + 1;

	// get number of cyan icons
	ub10xRegular = iNumberOfRegulars / 10;
	ubRegular = iNumberOfRegulars % 10;
	iTotalNumberOfIcons = ub10xRegular + ubRegular;

	// now display cyan icons
	for( iCurrentTroopIcon = 0; iCurrentTroopIcon < iTotalNumberOfIcons; ++iCurrentTroopIcon )
	{
		// get screen x and y coords
		sX =  ( iCurrentTroopIcon % NUMBER_OF_MILITIA_ICONS_PER_LOWER_ROW ) * MEDIUM_MILITIA_ICON_SPACING + MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X + 1;
		sY =  ( ubRow + iCurrentTroopIcon / NUMBER_OF_MILITIA_ICONS_PER_LOWER_ROW ) * MEDIUM_MILITIA_ICON_SPACING + MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_LOWER_ROW_Y;

		if( iCurrentTroopIcon < ub10xRegular )
		{
			iCurrentIcon = POPUP_CYAN_10X_BOX;
		}
		else
		{
			iCurrentIcon = POPUP_CYAN_BOX;
		}

		BltVideoObject( FRAME_BUFFER, hVObject, ( UINT16 )( iCurrentIcon ), sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// offset row for elites
	ubRow += iTotalNumberOfIcons / NUMBER_OF_MILITIA_ICONS_PER_LOWER_ROW + 1;

	// get number of blue icons
	ub10xElite = iNumberOfElites / 10;
	ubElite = iNumberOfElites % 10;
	iTotalNumberOfIcons = ub10xElite + ubElite;

	// now display blue icons
	for( iCurrentTroopIcon = 0; iCurrentTroopIcon < iTotalNumberOfIcons; ++iCurrentTroopIcon )
	{
		// get screen x and y coords
		sX =  ( iCurrentTroopIcon % NUMBER_OF_MILITIA_ICONS_PER_LOWER_ROW ) * MEDIUM_MILITIA_ICON_SPACING + MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X + 1;
		sY =  ( ubRow + iCurrentTroopIcon / NUMBER_OF_MILITIA_ICONS_PER_LOWER_ROW ) * MEDIUM_MILITIA_ICON_SPACING + MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_LOWER_ROW_Y;

		if( iCurrentTroopIcon < ub10xElite )
		{
			iCurrentIcon = POPUP_BLUE_10X_BOX;
		}
		else
		{
			iCurrentIcon = POPUP_BLUE_BOX;
		}

		BltVideoObject( FRAME_BUFFER, hVObject, ( UINT16 )( iCurrentIcon ), sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
	}
}

BOOLEAN IsThisMilitiaTownSectorAllowable( INT16 sSectorIndexValue )
{
	INT16 sBaseSectorValue = 0, sGlobalMapSector = 0;
	INT16 sSectorX, sSectorY;

	// is this sector allowed to be clicked on?
	sBaseSectorValue = GetBaseSectorForCurrentTown( );
	sGlobalMapSector = sBaseSectorValue + ( ( sSectorIndexValue % MILITIA_BOX_ROWS ) + ( sSectorIndexValue / MILITIA_BOX_ROWS ) * 16 );
	
	// skip sectors that's beyond sector P16
	if( sGlobalMapSector > SEC_P16 )
	{
		return( FALSE );
	}	

	sSectorX = SECTORX( sGlobalMapSector );
	sSectorY = SECTORY( sGlobalMapSector );

	// is this in fact part of a town?
	if( StrategicMap[ CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY ) ].bNameId == BLANK_SECTOR )
	{
		return( FALSE );
	}

	if( !SectorOursAndPeaceful( sSectorX, sSectorY, 0 ) )
	{
		return( FALSE );
	}

	// valid
	return( TRUE );
}


void DrawTownMilitiaName( void )
{
	CHAR16 sString[ 64 ];
	INT16 sX, sY;

	// get the name for the current militia town
	swprintf( sString, L"%s %s", pTownNames[ sSelectedMilitiaTown ], pMilitiaString[ 0 ] );
	FindFontCenterCoordinates( MAP_MILITIA_BOX_POS_X, MAP_MILITIA_BOX_POS_Y + MILITIA_BOX_TEXT_OFFSET_Y, MILITIA_BOX_WIDTH, MILITIA_BOX_TEXT_TITLE_HEIGHT, sString, FONT10ARIAL, &sX, &sY );
	mprintf(  sX, sY, sString );

	// might as well show the unassigned string
	swprintf( sString, L"%s", pMilitiaString[ 1 ] );
	FindFontCenterCoordinates( MAP_MILITIA_BOX_POS_X, MAP_MILITIA_BOX_POS_Y + MILITIA_BOX_UNASSIGNED_TEXT_OFFSET_Y, MILITIA_BOX_WIDTH, GetFontHeight( FONT10ARIAL ), sString, FONT10ARIAL, &sX, &sY );
	mprintf(  sX, sY, sString );

	// show the amount of unassigned militia
	swprintf( sString, L"(%d/%d/%d/%d)", sGreensOnCursor, sRegularsOnCursor, sElitesOnCursor, sGreensOnCursor + sRegularsOnCursor + sElitesOnCursor );
	mprintf(  MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X, MAP_MILITIA_BOX_POS_Y + MILITIA_BOX_UNASSIGNED_TEXT_OFFSET_Y, sString );	
}


void HandleShutDownOfMilitiaPanelIfPeopleOnTheCursor( INT16 sTownValue  )
{
	INT32 iCounter = 0, iCounterB = 0, iNumberUnderControl = 0, iNumberThatCanFitInSector= 0, iCount = 0;
	BOOLEAN fLastOne = FALSE;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Map Screen3");

	// check if anyone still on the cursor
	if( !sGreensOnCursor && !sRegularsOnCursor && !sElitesOnCursor )
	{
		return;
	}

	// yep someone left
	iNumberUnderControl = GetTownSectorsUnderControl( ( INT8 ) sTownValue );

	// find number of sectors under player's control
	while( pTownNamesList[ iCounter ] != 0 )
	{
		if( pTownNamesList[ iCounter] == sTownValue )
		{
			INT16 sX = GET_X_FROM_STRATEGIC_INDEX( pTownLocationsList[iCounter] );
			INT16 sY = GET_Y_FROM_STRATEGIC_INDEX( pTownLocationsList[iCounter] );

			if ( SectorOursAndPeaceful( sX, sY, 0 ) )
			{
				iCount = 0;
				iNumberThatCanFitInSector = gGameExternalOptions.iMaxMilitiaPerSector - NumNonPlayerTeamMembersInSector( sX, sY, MILITIA_TEAM );
				
				while( ( iCount < iNumberThatCanFitInSector ) && ( sGreensOnCursor || sRegularsOnCursor || sElitesOnCursor ) )
				{
					// green
					if( ( iCount < iNumberThatCanFitInSector ) && sGreensOnCursor )
					{
						StrategicAddMilitiaToSector( sX, sY, GREEN_MILITIA, 1 );
						++iCount;
						--sGreensOnCursor;
					}

					// regular
					if( ( iCount < iNumberThatCanFitInSector ) && sRegularsOnCursor )
					{
						StrategicAddMilitiaToSector( sX, sY, REGULAR_MILITIA, 1 );
						++iCount;
						--sRegularsOnCursor;
					}

					// elite
					if( ( iCount < iNumberThatCanFitInSector ) && sElitesOnCursor )
					{
						StrategicAddMilitiaToSector( sX, sY, ELITE_MILITIA, 1 );
						++iCount;
						--sElitesOnCursor;
					}
				}
			}

			fLastOne = TRUE;

			iCounterB = iCounter + 1;

			while( pTownNamesList[ iCounterB ] != 0 )
			{
				if( pTownNamesList[ iCounterB ] == sTownValue )
				{
					fLastOne = FALSE;
				}

				++iCounterB;
			}

			if( fLastOne )
			{
				StrategicAddMilitiaToSector( sX, sY, GREEN_MILITIA, (UINT8)(sGreensOnCursor % iNumberUnderControl) );
				StrategicAddMilitiaToSector( sX, sY, REGULAR_MILITIA, (UINT8)(sRegularsOnCursor % iNumberUnderControl) );
				StrategicAddMilitiaToSector( sX, sY, ELITE_MILITIA, (UINT8)(sElitesOnCursor % iNumberUnderControl) );
			}
		}

		++iCounter;
	}

	// zero out numbers on the cursor
	sGreensOnCursor = 0;
	sRegularsOnCursor = 0;
	sElitesOnCursor = 0;
}

void HandleRemovalOfAllTroopsAmongstSectors( void )
{
	// even out troops among the town
	INT32 iCounter = 0, iNumberUnderControl = 0, iNumberOfGreens = 0, iNumberOfRegulars = 0, iNumberOfElites = 0;
	INT32 iNumberLeftOverGreen = 0, iNumberLeftOverRegular = 0, iNumberLeftOverElite = 0;
	INT16 sBaseSectorValue = 0, sCurrentSectorValue = 0;
	INT16 sSectorX = 0, sSectorY = 0;
	
	// how many sectors in the selected town do we control?
	iNumberUnderControl = GetTownSectorsUnderControl( ( INT8 ) sSelectedMilitiaTown );

	// if none, there's nothing to be done!
	if( !iNumberUnderControl )
	{
		return;
	}

	// get the sector value for the upper left corner
	sBaseSectorValue = GetBaseSectorForCurrentTown( );

	// render icons for map
	for( iCounter = 0; iCounter < MILITIA_BOXREGIONS; ++iCounter )
	{
		// grab current sector value
		sCurrentSectorValue = sBaseSectorValue + ( ( iCounter % MILITIA_BOX_ROWS ) + ( iCounter / MILITIA_BOX_ROWS ) * ( 16 ) );

		sSectorX = SECTORX( sCurrentSectorValue );
		sSectorY = SECTORY( sCurrentSectorValue );

		// skip sectors not in the selected town (nearby other towns or wilderness SAM Sites)
		if( GetTownIdForSector( sSectorX, sSectorY ) != sSelectedMilitiaTown )
		{
			continue;
		}

		if( !StrategicMap[ CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY ) ].fEnemyControlled )
		{
			// Flugente: we no use these handy functions, as this will make altering militia distribution behaviour much easier
			// get number of each
			iNumberOfGreens += MilitiaInSectorOfRankStationary( sSectorX, sSectorY, GREEN_MILITIA );
			iNumberOfRegulars += MilitiaInSectorOfRankStationary( sSectorX, sSectorY, REGULAR_MILITIA );
			iNumberOfElites += MilitiaInSectorOfRankStationary( sSectorX, sSectorY, ELITE_MILITIA );

			//zero out all numbers in sectors
			StrategicRemoveAllStaticMilitiaFromSector( sSectorX, sSectorY, GREEN_MILITIA );
			StrategicRemoveAllStaticMilitiaFromSector( sSectorX, sSectorY, REGULAR_MILITIA );
			StrategicRemoveAllStaticMilitiaFromSector( sSectorX, sSectorY, ELITE_MILITIA );
		}
	}

	// now add to those on the cursor
	sGreensOnCursor += iNumberOfGreens;
	sRegularsOnCursor += iNumberOfRegulars;
	sElitesOnCursor += iNumberOfElites;

	if( ( sGreensOnCursor > 0 ) || ( sRegularsOnCursor > 0 ) || ( sElitesOnCursor > 0 ) )
		SpecifyButtonText( giMapMilitiaButton[ 4 ], pMilitiaButtonString[ 2 ] );
}

// Flugente: evening out distributes all militia evenly across town sectors. Note that militia in groups are not affected - they are in a sector, but do not get redistributed
void HandleEveningOutOfTroopsAmongstSectors( void )
{
	INT32 iMaxMilitiaPerSector = gGameExternalOptions.iMaxMilitiaPerSector;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Map Screen4");
	// even out troops among the town
	INT32 iCounter = 0, iNumberUnderControl = 0, iNumberOfGreens = 0, iNumberOfRegulars = 0, iNumberOfElites = 0, iTotalNumberOfTroops = 0;
	INT32 iNumberLeftOverGreen = 0, iNumberLeftOverRegular = 0, iNumberLeftOverElite = 0;
	INT16 sBaseSectorValue = 0, sCurrentSectorValue = 0;
	INT16 sSectorX = 0, sSectorY = 0, sSector = 0;
	INT16 sTotalSoFar = 0;

	// how many sectors in the selected town do we control?
	iNumberUnderControl = GetTownSectorsUnderControl( ( INT8 ) sSelectedMilitiaTown );

	// if none, there's nothing to be done!
	if( !iNumberUnderControl )
	{
		return;
	}

	// get the sector value for the upper left corner
	sBaseSectorValue = GetBaseSectorForCurrentTown( );

	// render icons for map
	for( iCounter = 0; iCounter < MILITIA_BOXREGIONS; ++iCounter )
	{
		// grab current sector value
		sCurrentSectorValue = sBaseSectorValue + ( ( iCounter % MILITIA_BOX_ROWS ) + ( iCounter / MILITIA_BOX_ROWS ) * ( 16 ) );
		
		// skip sectors that's beyond sector P16
		if( sCurrentSectorValue > SEC_P16 )
		{
			continue;
		}

		sSectorX = SECTORX( sCurrentSectorValue );
		sSectorY = SECTORY( sCurrentSectorValue );

		// skip sectors not in the selected town (nearby other towns or wilderness SAM Sites)
		if( GetTownIdForSector( sSectorX, sSectorY ) != sSelectedMilitiaTown )
		{
			continue;
		}

		if( !StrategicMap[ CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY ) ].fEnemyControlled )
		{
			// get number of each
			iNumberOfGreens += MilitiaInSectorOfRankStationary( sSectorX, sSectorY, GREEN_MILITIA );
			iNumberOfRegulars += MilitiaInSectorOfRankStationary( sSectorX, sSectorY, REGULAR_MILITIA );
			iNumberOfElites += MilitiaInSectorOfRankStationary( sSectorX, sSectorY, ELITE_MILITIA );
		}
	}

	// now grab those on the cursor
	iNumberOfGreens += sGreensOnCursor;
	iNumberOfRegulars += sRegularsOnCursor;
	iNumberOfElites += sElitesOnCursor;

	// now get the left overs
	iNumberLeftOverGreen = iNumberOfGreens % iNumberUnderControl;
	iNumberLeftOverRegular = iNumberOfRegulars % iNumberUnderControl;
	iNumberLeftOverElite = iNumberOfElites % iNumberUnderControl;

	// get total
	iTotalNumberOfTroops = iNumberOfGreens + iNumberOfRegulars + iNumberOfElites;

	if( !iTotalNumberOfTroops )
	{
		return;
	}

	iCounter = 0;

	while( pTownNamesList[ iCounter ] != 0 )
	{
		if( pTownNamesList[ iCounter] == sSelectedMilitiaTown )
		{
			sSectorX = GET_X_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );
			sSectorY = GET_Y_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );

			if( !StrategicMap[ pTownLocationsList[ iCounter ] ].fEnemyControlled && !NumHostilesInSector( sSectorX, sSectorY, 0 ) )
			{
				sSector = SECTOR( sSectorX, sSectorY );

				// distribute here
				SectorInfo[ sSector ].ubNumberOfCivsAtLevel[ GREEN_MILITIA ] =  ( UINT8 )( iNumberOfGreens / iNumberUnderControl );
				SectorInfo[ sSector ].ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] =  ( UINT8 )( iNumberOfRegulars / iNumberUnderControl );
				SectorInfo[ sSector ].ubNumberOfCivsAtLevel[ ELITE_MILITIA ] =  ( UINT8 )( iNumberOfElites / iNumberUnderControl );

				// Flugente: as we do not move the group militia, we have to make sure we don't accidentally overfill a sector
				sTotalSoFar = NumNonPlayerTeamMembersInSector( sSectorX, sSectorY, MILITIA_TEAM );

				// add leftovers that weren't included in the div operation
				if( ( iNumberLeftOverGreen ) && ( sTotalSoFar < iMaxMilitiaPerSector ) )
				{
					SectorInfo[ sSector ].ubNumberOfCivsAtLevel[ GREEN_MILITIA ]++;
					++sTotalSoFar;
					--iNumberLeftOverGreen;
				}

				if( ( iNumberLeftOverRegular )&&( sTotalSoFar < iMaxMilitiaPerSector ) )
				{
					SectorInfo[ sSector ].ubNumberOfCivsAtLevel[ REGULAR_MILITIA ]++;
					++sTotalSoFar;
					--iNumberLeftOverRegular;
				}

				if( ( iNumberLeftOverElite )&&( sTotalSoFar < iMaxMilitiaPerSector ) )
				{
					SectorInfo[ sSector ].ubNumberOfCivsAtLevel[ ELITE_MILITIA ]++;
					++sTotalSoFar;
					--iNumberLeftOverElite;
				}

				// if this sector is currently loaded
				if( (gWorldSectorX != 0) &&
					(gWorldSectorY != 0) && 
					(sSector == SECTOR( gWorldSectorX, gWorldSectorY )) )
				{
					gfStrategicMilitiaChangesMade = TRUE;
				}
			}
		}

		++iCounter;
	}


	// zero out numbers on the cursor
	sGreensOnCursor = 0;
	sRegularsOnCursor = 0;
	sElitesOnCursor = 0;

	SpecifyButtonText( giMapMilitiaButton[ 4 ], pMilitiaButtonString[ 1 ] );
}

void CreateMilitiaPanelBottomButton( void )
{
	// set the button image
	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		giMapMilitiaButtonImage[ 3 ]=  LoadButtonImage( "INTERFACE\\militia.sti" ,-1,BIG_NORMAL_BUTTON,-1,BIG_PRESSED_BUTTON,-1 );
		giMapMilitiaButtonImage[ 4 ]=  LoadButtonImage( "INTERFACE\\militia.sti" ,-1,BIG_NORMAL_BUTTON,-1,BIG_PRESSED_BUTTON,-1 );
		giMapMilitiaButtonImage[ 5 ]=  LoadButtonImage( "INTERFACE\\militia.sti" ,-1,BIG_NORMAL_BUTTON,-1,BIG_PRESSED_BUTTON,-1 );
	}
	else if (iResolution < _1024x768)
	{
		giMapMilitiaButtonImage[ 3 ]=  LoadButtonImage( "INTERFACE\\militia_800x600.sti" ,-1,BIG_NORMAL_BUTTON,-1,BIG_PRESSED_BUTTON,-1 );
		giMapMilitiaButtonImage[ 4 ]=  LoadButtonImage( "INTERFACE\\militia_800x600.sti" ,-1,BIG_NORMAL_BUTTON,-1,BIG_PRESSED_BUTTON,-1 );
		giMapMilitiaButtonImage[ 5 ]=  LoadButtonImage( "INTERFACE\\militia_800x600.sti" ,-1,BIG_NORMAL_BUTTON,-1,BIG_PRESSED_BUTTON,-1 );
	}
	else
	{
		giMapMilitiaButtonImage[ 3 ]=  LoadButtonImage( "INTERFACE\\militia_1024x768.sti" ,-1,BIG_NORMAL_BUTTON,-1,BIG_PRESSED_BUTTON,-1 );
		giMapMilitiaButtonImage[ 4 ]=  LoadButtonImage( "INTERFACE\\militia_1024x768.sti" ,-1,BIG_NORMAL_BUTTON,-1,BIG_PRESSED_BUTTON,-1 );
		giMapMilitiaButtonImage[ 5 ]=  LoadButtonImage( "INTERFACE\\militia_1024x768.sti" ,-1,BIG_NORMAL_BUTTON,-1,BIG_PRESSED_BUTTON,-1 );
	}

	giMapMilitiaButton[ 3 ] = QuickCreateButton( giMapMilitiaButtonImage[ 3 ], MAP_MILITIA_BOX_POS_X + MAP_MILITIA_BOX_AUTO_BOX_X, MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_BOX_AUTO_BOX_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK) MilitiaAutoButtonCallback );

	giMapMilitiaButton[ 4 ] = QuickCreateButton( giMapMilitiaButtonImage[ 4 ], MAP_MILITIA_BOX_POS_X + MAP_MILITIA_BOX_DONE_BOX_X, MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_BOX_AUTO_BOX_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK) MilitiaDoneButtonCallback );

	giMapMilitiaButton[ 5 ] = QuickCreateButton( giMapMilitiaButtonImage[ 3 ], MAP_MILITIA_BOX_POS_X + MAP_MILITIA_BOX_REMOVE_BOX_X, MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_BOX_AUTO_BOX_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK) MilitiaRemoveButtonCallback );


	SpecifyButtonFont( giMapMilitiaButton[ 3 ], FONT10ARIAL );
	SpecifyButtonUpTextColors( giMapMilitiaButton[ 3 ], FONT_BLACK, FONT_BLACK );
	SpecifyButtonDownTextColors( giMapMilitiaButton[ 3 ], FONT_BLACK, FONT_BLACK );

	SpecifyButtonFont( giMapMilitiaButton[ 4 ], FONT10ARIAL );
	SpecifyButtonUpTextColors( giMapMilitiaButton[ 4 ], FONT_BLACK, FONT_BLACK );
	SpecifyButtonDownTextColors( giMapMilitiaButton[ 4 ], FONT_BLACK, FONT_BLACK );

	SpecifyButtonFont( giMapMilitiaButton[ 5 ], FONT10ARIAL );
	SpecifyButtonUpTextColors( giMapMilitiaButton[ 5 ], FONT_BLACK, FONT_BLACK );
	SpecifyButtonDownTextColors( giMapMilitiaButton[ 5 ], FONT_BLACK, FONT_BLACK );

	SpecifyButtonText( giMapMilitiaButton[ 3 ], pMilitiaButtonString[ 0 ] );
	SpecifyButtonText( giMapMilitiaButton[ 4 ], pMilitiaButtonString[ 1 ] );
	SpecifyButtonText( giMapMilitiaButton[ 5 ], pMilitiaButtonString[ 3 ] );

	// AUTO button help
	SetButtonFastHelpText( giMapMilitiaButton[ 3 ], pMilitiaButtonsHelpText[ 3 ] );


	// if auto-distribution is not possible
	if ( !CanMilitiaAutoDistribute( ) )
	{
		// disable the AUTO button
		DisableButton( giMapMilitiaButton[ 3 ] );
	}

	return;
}


void DeleteMilitiaPanelBottomButton( void )
{

	// delete militia panel bottom
	RemoveButton( giMapMilitiaButton[ 3 ] );
	RemoveButton( giMapMilitiaButton[ 4 ] );
	RemoveButton( giMapMilitiaButton[ 5 ] );

	UnloadButtonImage( giMapMilitiaButtonImage[ 3 ] );
	UnloadButtonImage( giMapMilitiaButtonImage[ 4 ] );
	UnloadButtonImage( giMapMilitiaButtonImage[ 5 ] );

	if( sSelectedMilitiaTown != 0 )
	{
		HandleShutDownOfMilitiaPanelIfPeopleOnTheCursor( sSelectedMilitiaTown );
	}

	// redraw the map
	fMapPanelDirty = TRUE;

}


void MilitiaAutoButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			btn->uiFlags &= ~( BUTTON_CLICKED_ON );

			// distribute troops over all the sectors under control
			HandleEveningOutOfTroopsAmongstSectors( );
			fMapPanelDirty = TRUE;
		}
	}

	return;
}


void MilitiaDoneButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			btn->uiFlags &= ~( BUTTON_CLICKED_ON );

			// HEADROCK HAM 3.6: Clicking the done button while militia are on the cursor causes a pop-up
			// for authorizing DISBANDING those militia.
			if (sGreensOnCursor || sRegularsOnCursor || sElitesOnCursor)
			{
				DoMapMessageBox( MSG_BOX_BASIC_STYLE, pMilitiaString[ 3 ], MAP_SCREEN, MSG_BOX_FLAG_YESNO, MilitiaDisbandYesNoBoxCallback );
			}
			else
			{
				// reset fact we are in the box
				sSelectedMilitiaTown = 0;
				fMapPanelDirty = TRUE;
				// Go ahead and reset the militia in the selected sector (even if we didn't change it here)
				if (gfStrategicMilitiaChangesMade)
				{
					ResetMilitia();
				}
			}
		}
	}

	return;
}

void MilitiaRemoveButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			btn->uiFlags &= ~( BUTTON_CLICKED_ON );

			// remove all troops to pool
			HandleRemovalOfAllTroopsAmongstSectors( );
			fMapPanelDirty = TRUE;
		}
	}

	return;
}

void RenderShadingForUnControlledSectors( void )
{
	// now render shading over any uncontrolled sectors
	// get the sector value for the upper left corner
	INT16 sBaseSectorValue = 0, sCurrentSectorValue = 0, sSectorX = 0, sSectorY = 0, sX = 0, sY = 0;
	INT32 iCounter = 0;

	// get the base sector value
	sBaseSectorValue = GetBaseSectorForCurrentTown( );

	// render icons for map
	for( iCounter = 0; iCounter < MILITIA_BOXREGIONS; ++iCounter )
	{
		// grab current sector value
		sCurrentSectorValue = sBaseSectorValue + ( ( iCounter % MILITIA_BOX_ROWS ) + ( iCounter / MILITIA_BOX_ROWS ) * ( 16 ) );
		
		// skip sectors that's beyond sector P16
		if( sCurrentSectorValue > SEC_P16 )
		{
			continue;
		}

		sSectorX = SECTORX( sCurrentSectorValue );
		sSectorY = SECTORY( sCurrentSectorValue );

		// skip sectors not in the selected town (nearby other towns or wilderness SAM Sites), already shaded in sti image
		if( GetTownIdForSector( sSectorX, sSectorY ) != sSelectedMilitiaTown )
		{
			continue;
		}

		if( ( StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( sCurrentSectorValue ) ].bNameId != BLANK_SECTOR ) &&
			( ( StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( sCurrentSectorValue ) ].fEnemyControlled ) ||
				( NumHostilesInSector( sSectorX, sSectorY, 0 ) ) ) )
		{
			// shade this sector, not under our control
			sX = MAP_MILITIA_BOX_POS_X + MAP_MILITIA_MAP_X + ( ( iCounter % MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_WIDTH );
			sY = MAP_MILITIA_BOX_POS_Y + MAP_MILITIA_MAP_Y + ( ( iCounter / MILITIA_BOX_ROWS ) * MILITIA_BOX_BOX_HEIGHT );

			ShadowVideoSurfaceRect( FRAME_BUFFER , sX, sY, sX + MILITIA_BOX_BOX_WIDTH - 1, sY + MILITIA_BOX_BOX_HEIGHT - 1 );
		}
	}

	return;
}

void DrawTownMilitiaForcesOnMap( void )
{
	INT32 iCounterB = 0, iTotalNumberOfTroops = 0;
	UINT16 usIconValue = 0;
	INT32 iNumberOfGreens = 0, iNumberOfRegulars = 0,  iNumberOfElites = 0;
	HVOBJECT hVObject;
	INT16 sSectorX = 0, sSectorY = 0;
	UINT8 ubGreen, ubRegular, ubElite, ub10xGreen, ub10xRegular, ub10xElite;
	UINT8 i, ubIconPosition;

	// get militia video object
	GetVideoObject( &hVObject, guiMilitia );

	// clip blits to mapscreen region
	ClipBlitsToMapViewRegion( );

	// now handle militia for all sectors
	for( sSectorX = MINIMUM_VALID_X_COORDINATE; sSectorX <= MAXIMUM_VALID_X_COORDINATE ; ++sSectorX )
	{
		for( sSectorY = MINIMUM_VALID_Y_COORDINATE; sSectorY <= MAXIMUM_VALID_Y_COORDINATE ; ++sSectorY )
		{
			ubIconPosition = 0;

			// get number of each
			iNumberOfGreens = MilitiaInSectorOfRank( sSectorX, sSectorY, GREEN_MILITIA );
			iNumberOfRegulars = MilitiaInSectorOfRank( sSectorX, sSectorY, REGULAR_MILITIA );
			iNumberOfElites = MilitiaInSectorOfRank( sSectorX, sSectorY, ELITE_MILITIA );

			// get number of icons of each
			ub10xGreen = iNumberOfGreens / 10;
			ubGreen = iNumberOfGreens % 10;
			ub10xRegular = iNumberOfRegulars / 10;
			ubRegular = iNumberOfRegulars % 10;
			ub10xElite = iNumberOfElites / 10;
			ubElite = iNumberOfElites % 10;

			// first draw elites as they are more important
			for ( i = 0; i < ub10xElite; ++i )
			{
				DrawMapBoxIcon( hVObject, SMALL_BLUE_10X_BOX, sSectorX, sSectorY, ubIconPosition );
				++ubIconPosition;
			}

			for ( i = 0; i < ubElite; ++i )
			{
				DrawMapBoxIcon( hVObject, SMALL_BLUE_BOX, sSectorX, sSectorY, ubIconPosition );
				++ubIconPosition;
			}

			// then draw regulars
			for ( i = 0; i < ub10xRegular; ++i )
			{
				DrawMapBoxIcon( hVObject, SMALL_CYAN_10X_BOX, sSectorX, sSectorY, ubIconPosition );
				++ubIconPosition;
			}

			for ( i = 0; i < ubRegular; ++i )
			{
				DrawMapBoxIcon( hVObject, SMALL_CYAN_BOX, sSectorX, sSectorY, ubIconPosition );
				++ubIconPosition;
			}

			// last draw greens
			for ( i = 0; i < ub10xGreen; ++i )
			{
				DrawMapBoxIcon( hVObject, SMALL_GREEN_10X_BOX, sSectorX, sSectorY, ubIconPosition );
				++ubIconPosition;
			}

			for ( i = 0; i < ubGreen; ++i )
			{
				DrawMapBoxIcon( hVObject, SMALL_GREEN_BOX, sSectorX, sSectorY, ubIconPosition );
				++ubIconPosition;
			}
		}
	}
	// restore clip blits
	RestoreClipRegionToFullScreen( );
}


void CheckAndUpdateStatesOfSelectedMilitiaSectorButtons( void )
{
	// now set the militia map button text
	INT32 iNumberOfGreens = 0, iNumberOfRegulars = 0, iNumberOfElites = 0;
	INT16 sBaseSectorValue = 0, sGlobalMapSector = 0;
	
	if( !fMilitiaMapButtonsCreated )
	{
		EnableButton( giMapMilitiaButton[ 4 ] );
		return;
	}

	// grab the appropriate global sector value in the world
	sBaseSectorValue = GetBaseSectorForCurrentTown( );
	sGlobalMapSector = sBaseSectorValue + ( ( sSectorMilitiaMapSector % MILITIA_BOX_ROWS ) + ( sSectorMilitiaMapSector / MILITIA_BOX_ROWS ) * ( 16 ) );

	iNumberOfGreens = MilitiaInSectorOfRankStationary( SECTORX( sGlobalMapSector ), SECTORY( sGlobalMapSector ), GREEN_MILITIA ) + sGreensOnCursor;
	iNumberOfRegulars = MilitiaInSectorOfRankStationary( SECTORX( sGlobalMapSector ), SECTORY( sGlobalMapSector ), REGULAR_MILITIA ) + sRegularsOnCursor;
	iNumberOfElites = MilitiaInSectorOfRankStationary( SECTORX( sGlobalMapSector ), SECTORY( sGlobalMapSector ), ELITE_MILITIA ) + sElitesOnCursor;

	// HEADROCK HAM 3.6: This button is no longer disabled when there are unassigned militia. In fact,
	// clicking the button when militia are unassigned will prompt the player to authorize disbanding them.
	if( ( sGreensOnCursor > 0 ) || ( sRegularsOnCursor > 0 ) || ( sElitesOnCursor > 0 ) )
	{
	//	DisableButton( giMapMilitiaButton[ 4 ] );	// DONE
		SpecifyButtonText( giMapMilitiaButton[ 4 ], pMilitiaButtonString[ 2 ] );
	}
	else
	{
	//	EnableButton( giMapMilitiaButton[ 4 ] );	// DONE
		SpecifyButtonText( giMapMilitiaButton[ 4 ], pMilitiaButtonString[ 1 ] );
	}

	// greens button
	if( !iNumberOfGreens )
	{
		DisableButton( giMapMilitiaButton[ 0 ] );
	}
	else
	{
		EnableButton( giMapMilitiaButton[ 1 ] );
	}

	// regulars button
	if( !iNumberOfRegulars )
	{
		DisableButton( giMapMilitiaButton[ 1 ] );
	}
	else
	{
		EnableButton( giMapMilitiaButton[ 1 ] );
	}

	// elites button
	if( !iNumberOfElites )
	{
		DisableButton( giMapMilitiaButton[ 2 ] );
	}
	else
	{
		EnableButton( giMapMilitiaButton[ 2 ] );
	}
}


BOOLEAN ShadeUndergroundMapElem( INT16 sSectorX, INT16 sSectorY )
{
	INT16 sScreenX, sScreenY;

	GetScreenXYFromMapXY( sSectorX, sSectorY, &sScreenX, &sScreenY );

	sScreenX += 1;

	ShadowVideoSurfaceRect( guiSAVEBUFFER, sScreenX, sScreenY, sScreenX + MAP_GRID_X - 2, sScreenY + MAP_GRID_Y - 2 );

	return( TRUE );
}


void ShadeSubLevelsNotVisited( void )
{
	UNDERGROUND_SECTORINFO *pNode = gpUndergroundSectorInfoHead;


	// obtain the 16-bit version of the same color used in the mine STIs
 	gusUndergroundNearBlack = Get16BPPColor( FROMRGB( 2, 2, 0 ) );

	// run through all (real & possible) underground sectors
	while( pNode )
	{
		// if the sector is on the currently displayed sublevel, and has never been visited
		if( pNode->ubSectorZ == (UINT8)iCurrentMapSectorZ && !pNode->fVisited )
		{
			// remove that portion of the "mine" graphics from view
			HideExistenceOfUndergroundMapSector( pNode->ubSectorX, pNode->ubSectorY );
		}

		pNode = pNode->next;
	}
}


void HandleLowerLevelMapBlit( void )
{
	UINT8 offsetX = 0;
	UINT8 offsetY = 0;

	HVOBJECT hHandle;

	// blits the sub level maps
	switch( iCurrentMapSectorZ )
	{
		case( 1 ):
			GetVideoObject( &hHandle, guiSubLevel1 );
			break;
		case( 2 ):
			GetVideoObject( &hHandle, guiSubLevel2 );
			break;
		case( 3 ):
			GetVideoObject( &hHandle, guiSubLevel3 );
			break;
		default:
			AssertMsg( 0, "Invalid underground sector level");
			return;
	}

	UINT8 xVal = 21;
	UINT8 yVal = 17;
	UINT8 imageIndex = 0;
	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		offsetX = xVal;
		offsetY = yVal;
		imageIndex = 0;
	}
	else if (iResolution < _1024x768)
	{
		offsetX = xVal + 8;
		offsetY = yVal + 7;
		imageIndex = 1;
	}
	else
	{
		offsetX = xVal + 21;
		offsetY = yVal + 17;
		imageIndex = 2;
	}

	// handle the blt of the sublevel
	BltVideoObject( guiSAVEBUFFER, hHandle, imageIndex, MAP_VIEW_START_X + offsetX, MAP_VIEW_START_Y + offsetY, VO_BLT_SRCTRANSPARENCY, NULL );

	// handle shading of sublevels
	ShadeSubLevelsNotVisited( );

	return;
}


void MilitiaBoxMaskBtnCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment screen mask region
	if( ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )  )
	{
		sSectorMilitiaMapSector = -1;
	}
	else if( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		sSectorMilitiaMapSector = -1;
	}
}

//There is a special case flag used when players encounter enemies in a sector, then retreat.  The number of enemies
//will display on mapscreen until time is compressed.  When time is compressed, the flag is cleared, and
//a question mark is displayed to reflect that the player no longer knows.  This is the function that clears that
//flag.
void ClearAnySectorsFlashingNumberOfEnemies()
{
	for ( INT32 i = 0; i < 256; ++i )
	{
		SectorInfo[ i ].uiFlags &= ~SF_PLAYER_KNOWS_ENEMIES_ARE_HERE;
	}

	// redraw map
	fMapPanelDirty = TRUE;
}



UINT32 WhatPlayerKnowsAboutEnemiesInSector( INT16 sSectorX, INT16 sSectorY )
{
	UINT32 uiSectorFlags = SectorInfo[ SECTOR( sSectorX, sSectorY ) ].uiFlags;
	BOOLEAN fDetection = FALSE;
	BOOLEAN fCount = FALSE;
	BOOLEAN fDirection = FALSE;

	Assert(sSectorX > 0 && sSectorY > 0 && sSectorX < 17 && sSectorY < 17);

	// HEADROCK HAM 3.5: There are now several ways to control the detection of enemy units. This function has been revamped
	// to consider all options. Depending on externalized settings, different conditions yield different results.
	//
	// Option #1: Vanilla Default. 
	//		It is possible to detect all enemies in explored territory, including cities. 
	//		Militia in an adjacent sector can count how many enemies are there, except standing garrisons.
	// Option #2: HAM 3.2. No Detection without Recon.
	//      Mobile enemy groups are only detected if there are militia in any adjacent sector. Otherwise, they can 
	//		travel through explored sectors without being seen.
	//
	// Option #3: This is a new feature for HAM 3.5, which ties together with Facility Externalization. 
	//		In simple words, facilities can enhance detection abilities. Facilities that do so may be placed around the 
	//		map in any sector. When a merc with sufficient wisdom is resting here, he increases the detection "level",
	//		making it easier to detect or even count enemies in distant sectors.
	//		This option can be combined with either of the two above.
	

	// Detection through active recon.
	// Mercs provide recon in the same sector they're in.
	// Militia provide recon in any adjacent sector (diagonals included)
	// There's also a special case flag used when players encounter enemies in a sector, then retreat. You can only
	// see the size of their force while the clock is paused. When unpaused, the flag is reset.
	if ( CanMercsScoutThisSector( sSectorX, sSectorY, 0 ) ||
			CanSomeoneNearbyScoutThisSector( sSectorX, sSectorY, FALSE ) || // merged militia check with scouting check - SANDRO
			( uiSectorFlags & SF_PLAYER_KNOWS_ENEMIES_ARE_HERE ) )
	{
		fDetection = TRUE;

		// if all enemies are mobile (I.E. not garrison troops)
		if ( NumStationaryEnemiesInSector( sSectorX, sSectorY ) == 0 )
		{
			// They're also counted by default.
			fCount = TRUE;
		}
	}
	// SADNRO - Scouting trait check for detection of nearby enemies
	if (CanSomeoneNearbyScoutThisSector( sSectorX, sSectorY, TRUE )) // scouting trait check
	{
		if (gSkillTraitValues.fSCCanDetectEnemyPresenseAround)
		{
			// show their presence
			fDetection = TRUE;
			// show their numbers
			if (gSkillTraitValues.fSCCanDetermineEnemyNumbersAround)
				fCount = TRUE;
		}
	}

	// Explored Sector Detection
	// Enemy can be detected in any previously-visited sector.
	// This is also enabled by some facilities, provided a merc is present and available to do it.
	if( GetSectorFlagStatus( sSectorX, sSectorY, 0, SF_ALREADY_VISITED ) == TRUE )
	{
		// HEADROCK HAM 3.2: When enabled, this INI setting disallows detection of enemy roamers beyond merc/militia
		// recon range.
		if (!gGameExternalOptions.fNoEnemyDetectionWithoutRecon)
		{
			// then he always knows about any enemy presence for the remainder of the game, but not exact numbers
			fDetection = TRUE;
		}
	}

	// if Skyrider noticed the enemis in the sector recently
	if ( uiSectorFlags & SF_SKYRIDER_NOTICED_ENEMIES_HERE )
	{
		// and Skyrider is still in this sector, flying
		if( IsSkyriderIsFlyingInSector( sSectorX, sSectorY ) )
		{
			// player remains aware of them as long as Skyrider remains in the sector
			fDetection = TRUE;
		}
		else
		{
			// Skyrider is gone, reset the flag that he noticed enemies here
			SectorInfo[ SECTOR( sSectorX, sSectorY ) ].uiFlags &= ~SF_SKYRIDER_NOTICED_ENEMIES_HERE;
		}
	}

	// Flugente: we know that enemies are here
	if ( uiSectorFlags & SF_ASSIGN_NOTICED_ENEMIES_HERE )
	{
		fDetection = TRUE;

		// we know how many there are
		if ( uiSectorFlags & SF_ASSIGN_NOTICED_ENEMIES_KNOW_NUMBER )
		{
			fCount = TRUE;
		}

		// we know the direction they are moving in
		if ( uiSectorFlags & SF_ASSIGN_NOTICED_ENEMIES_KNOW_DIRECTION )
		{
			fDirection = TRUE;
		}
	}

	// Facilities can set a flag that allows detection in some sectors. We can read flags directly from the sector
	// data to know whether we should show the enemies there. This overrides ANYTHING else.
	if (SectorInfo[ SECTOR( sSectorX, sSectorY ) ].ubDetectionLevel & 1)
	{
		fDetection = TRUE;
		fDirection = TRUE;
	}
	if (SectorInfo[ SECTOR( sSectorX, sSectorY ) ].ubDetectionLevel & (1<<2) )
	{
		fCount = TRUE;
		fDirection = TRUE;		
	}

	// HEADROCK HAM 5: New cases below
	if (!fDetection)
	{
		// no information available
		return KNOWS_NOTHING;
	}
	else if (!fCount && !fDirection)
	{
		// No accurate information
		return KNOWS_THEYRE_THERE;
	}
	else if (!fCount)
	{
		// No accurate information but have direction
		return KNOWS_THEYRE_THERE_AND_WHERE_GOING;
	}
	else if (!fDirection)
	{
		// Accurate information
		return KNOWS_HOW_MANY;
	}
	
	// Accurate information including direction of travel!
	return KNOWS_HOW_MANY_AND_WHERE_GOING;
}


BOOLEAN CanMercsScoutThisSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	INT32 iFirstId = 0, iLastId = 0;
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;


	// to speed it up a little?
	iFirstId = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	iLastId = gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	for( iCounter = iFirstId; iCounter <= iLastId; iCounter++ )
	{
		// get the soldier
		pSoldier = &Menptr[ iCounter ];

		// is the soldier active
		if( pSoldier->bActive == FALSE )
		{
			continue;
		}

		// vehicles can't scout!
		if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			continue;
		}

		// POWs, dead guys, guys in transit, sleeping, and really hurt guys can't scout!
		if ( ( pSoldier->bAssignment == IN_TRANSIT ) ||
				( pSoldier->bAssignment == ASSIGNMENT_POW ) ||
				( pSoldier->bAssignment == ASSIGNMENT_DEAD ) ||
				( pSoldier->flags.fMercAsleep == TRUE ) ||
				( pSoldier->stats.bLife < OKLIFE ) )
		{
			continue;
		}

		// don't count mercs aboard Skyrider
		if ( ( pSoldier->bAssignment == VEHICLE ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			continue;
		}

		// mercs on the move can't scout
		if ( pSoldier->flags.fBetweenSectors )
		{
			continue;
		}

		// is he here?
		if( ( pSoldier->sSectorX == sSectorX ) && ( pSoldier->sSectorY == sSectorY ) && ( pSoldier->bSectorZ == bSectorZ ) )
		{
			return( TRUE );
		}
	}

	// none here who can scout
	return( FALSE );
}



void HandleShowingOfEnemyForcesInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 ubIconPosition )
{
	INT16 sNumberOfEnemies = 0;

	// ATE: If game has just started, don't do it!
	if ( DidGameJustStart() )
	{
		return;
	}

	// never display enemies underground - sector info doesn't have support for it!
	if( bSectorZ != 0 )
	{
		return;
	}

	// Flugente: note if we have detected a VIP here
	if ( PlayerKnowsAboutVIP( sSectorX, sSectorY ) )
		ShowVIPSymbol( sSectorX, sSectorY );

	// Flugente: show militia in motion
	if ( NumNonPlayerTeamMembersInSector( sSectorX, sSectorY, MILITIA_TEAM ) )
	{
		ShowMilitiaInMotion( sSectorX, sSectorY );

		// replace the above function with this once militia movement is properly GROUP-based
		//ShowNonPlayerGroupsInMotion( sSectorX, sSectorY, MILITIA_TEAM );
	}

	// Flugente: show moving militia groups
	GROUP* pGroup = gpGroupList;
	while ( pGroup )
	{
		if ( pGroup->usGroupTeam == MILITIA_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			ShowNonPlayerGroupsInMotion( sSectorX, sSectorY, MILITIA_TEAM );

			break;
		}
		pGroup = pGroup->next;
	}

	// get total number of badguys here
	sNumberOfEnemies = NumNonPlayerTeamMembersInSector( sSectorX, sSectorY, ENEMY_TEAM );

	if ( sNumberOfEnemies )
	{
		// silversurfer: disabled this for now as it always leads to false positives with scouting trait and
		// gSkillTraitValues.fSCCanDetectEnemyPresenseAround == true and gSkillTraitValues.fSCCanDetermineEnemyNumbersAround == false. 
		// If someone comes up with a good and believable way to implement a certain probability for false reports please go ahead and implement that instead of a 100 percent chance.
		// Flugente: snitch reports can be false - we assume an enemy patrol where there is none. Unfortunately, that would always be the case if fNoEnemyDetectionWithoutReconis false - we detect something everywhere,
		// so every sector gets a red question mark. In that case, get out of here
		// anv: probability of false enemy reports!
		// anyone here?
		/*if( !sNumberOfEnemies ) // && !gGameExternalOptions.fNoEnemyDetectionWithoutRecon )
		{
			// nope - display nothing
			return;
		}*/

		// Flugente: tanks get a special icon, so we need to count them separately
		UINT16 usNumTanks = NumTanksInSector( sSectorX, sSectorY, ENEMY_TEAM );

		switch ( WhatPlayerKnowsAboutEnemiesInSector( sSectorX, sSectorY ) )
		{
			// HEADROCK HAM 5: New cases below for showing enemy group heading.
			case KNOWS_NOTHING:
				// display nothing
				break;

			case KNOWS_THEYRE_THERE:
				// display a question mark
				ShowUncertainNumberEnemiesInSector( sSectorX, sSectorY );
				break;

			case KNOWS_THEYRE_THERE_AND_WHERE_GOING:
				// display a question mark
				ShowUncertainNumberEnemiesInSector( sSectorX, sSectorY );
				// display their direction of movement, if valid.
				ShowNonPlayerGroupsInMotion( sSectorX, sSectorY, ENEMY_TEAM );
				break;

			case KNOWS_HOW_MANY:
				// display individual icons for each enemy, starting at the received icon position index
				ShowEnemiesInSector( sSectorX, sSectorY, sNumberOfEnemies, usNumTanks, ubIconPosition );
				break;

			// HEADROCK HAM 5: New case for showing enemy groups AND where the are headed.
			case KNOWS_HOW_MANY_AND_WHERE_GOING:
				// display individual icons for each enemy, starting at the received icon position index
				ShowEnemiesInSector( sSectorX, sSectorY, sNumberOfEnemies, usNumTanks, ubIconPosition );
				// display their direction of movement, if valid.
				ShowNonPlayerGroupsInMotion( sSectorX, sSectorY, ENEMY_TEAM );
				break;
		}
	}
}


/*
UINT8 NumActiveCharactersInSector( INT16 sSectorX, INT16 sSectorY, INT16 bSectorZ )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;
	UINT8 ubNumberOnTeam = 0;

	for( iCounter = 0; iCounter < CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( gCharactersList[ iCounter ].fValid )
		{
			pSoldier = &( Menptr[ gCharactersList[ iCounter ].usSolID ] );

			if( pSoldier->bActive && ( pSoldier->stats.bLife > 0 ) &&
					( pSoldier->bAssignment != ASSIGNMENT_POW ) && ( pSoldier->bAssignment != IN_TRANSIT ) )
			{
				if( ( pSoldier->sSectorX == sSectorX ) && ( pSoldier->sSectorY == sSectorY ) && ( pSoldier->bSectorZ == bSectorZ ) )
					ubNumberOnTeam++;
			}
		}
	}

	return( ubNumberOnTeam );
}
*/


void ShowSAMSitesOnStrategicMap( void )
{
	INT32 iCounter = 0;
	INT16 sSectorX = 0, sSectorY = 0;
	INT16 sX = 0, sY = 0;
	HVOBJECT hHandle;
	INT8 ubVidObjIndex = 0;
	CHAR16 wString[ 40 ];


	if( fShowAircraftFlag )
	{
		BlitSAMGridMarkers( );
	}

	// sam site text
	for( iCounter = 0; iCounter < NUMBER_OF_SAMS; iCounter++ )
	{
		// has the sam site here been found?
		if( !fSamSiteFound[ iCounter ] )
		{
			continue;
		}

		// get the sector x and y
		sSectorX = gpSamSectorX[ iCounter ];
		sSectorY = gpSamSectorY[ iCounter ];
	
		GetScreenXYFromMapXY( sSectorX, sSectorY, &sX, &sY );
		sX += 5;
		sY += 3;
		ubVidObjIndex = 1;

		INT16 sIconX = sX + 5;
		INT16 sIconY = sY + 3;

		// draw SAM site icon
		GetVideoObject( &hHandle, guiSAMICON);
		BltVideoObject( guiSAVEBUFFER, hHandle, ubVidObjIndex, sIconX, sIconY, VO_BLT_SRCTRANSPARENCY, NULL );

		if( fShowAircraftFlag )
		{
			// write "SAM Site" centered underneath

			// HEADROCK HAM 5: Font size is now dynamic.

			INT32 MapSAMSiteFont;
			if (iResolution <= _800x600 )
			{
				MapSAMSiteFont = MAP_FONT;
			}
			else
			{
				MapSAMSiteFont = FONT14ARIAL;
			}

			INT16 sLabelX = sX + (MAP_GRID_X / 2);
			INT16 sLabelY = sY + MAP_GRID_Y + 2;

			wcscpy( wString, pLandTypeStrings[ SAM_SITE ] );

			// we're CENTERING the first string AROUND sX, so calculate the starting X value
			sLabelX -= StringPixLength( wString, MapSAMSiteFont) / 2;

			// if within view region...render, else don't
			if( ( sLabelX > MAP_VIEW_START_X + MAP_VIEW_WIDTH  ) || ( sLabelX < MAP_VIEW_START_X ) ||
					( sLabelY > MAP_VIEW_START_Y + MAP_VIEW_HEIGHT ) || ( sLabelY < MAP_VIEW_START_Y ) )
			{
				continue;
			}


			SetFontDestBuffer( guiSAVEBUFFER, MapScreenRect.iLeft + 2, MapScreenRect.iTop, MapScreenRect.iRight, MapScreenRect.iBottom, FALSE );

			// clip blits to mapscreen region
			ClipBlitsToMapViewRegion( );

			SetFont(MapSAMSiteFont);
			// Green on green doesn't contrast well, use Yellow
			SetFontForeground(FONT_MCOLOR_LTYELLOW);
			SetFontBackground(FONT_MCOLOR_BLACK);

			// draw the text
			gprintfdirty( sLabelX, sLabelY, wString );
			mprintf( sLabelX, sLabelY, wString);

			// restore clip blits
			RestoreClipRegionToFullScreen( );
		}
	}

	// refuel site text
	for( iCounter = 0; iCounter < NUMBER_OF_REFUEL_SITES; iCounter++ )
	{
		// has the refuel site here been known or found?
		if( !fRefuelingSiteKnown[ iCounter ] && !GetSectorFlagStatus( sRefuelSectorX[ iCounter ], sRefuelSectorY[ iCounter ], 0, SF_ALREADY_VISITED ) )
		{
			continue;
		}

		// get the sector x and y
		sSectorX = sRefuelSectorX[ iCounter ];
		sSectorY = sRefuelSectorY[ iCounter ];
	
		GetScreenXYFromMapXY( sSectorX, sSectorY, &sX, &sY );
		sX += 5;
		sY += 3;

		if( fShowAircraftFlag )
		{
			// write "Refuel Site" centered underneath

			// HEADROCK HAM 5: Font size is now dynamic.

			INT32 MapHeliSiteFont;
			if (iResolution <= _800x600 )
			{
				MapHeliSiteFont = MAP_FONT;
			}
			else
			{
				MapHeliSiteFont = FONT14ARIAL;
			}

			INT16 sLabelX = sX + (MAP_GRID_X / 2);
			INT16 sLabelY;
			if ( IsThisSectorASAMSector( sSectorX, sSectorY, 0 ) )
			{
				if (iResolution <= _800x600 )
					sLabelY = sY + MAP_GRID_Y + 10;
				else
					sLabelY = sY + MAP_GRID_Y + 16;
			}
			else
				sLabelY = sY + MAP_GRID_Y + 2;

			wcscpy( wString, pLandTypeStrings[ REFUEL_SITE ] );

			// we're CENTERING the first string AROUND sX, so calculate the starting X value
			sLabelX -= StringPixLength( wString, MapHeliSiteFont) / 2;

			// if within view region...render, else don't
			if( ( sLabelX > MAP_VIEW_START_X + MAP_VIEW_WIDTH  ) || ( sLabelX < MAP_VIEW_START_X ) ||
					( sLabelY > MAP_VIEW_START_Y + MAP_VIEW_HEIGHT ) || ( sLabelY < MAP_VIEW_START_Y ) )
			{
				continue;
			}


			SetFontDestBuffer( guiSAVEBUFFER, MapScreenRect.iLeft + 2, MapScreenRect.iTop, MapScreenRect.iRight, MapScreenRect.iBottom, FALSE );

			// clip blits to mapscreen region
			ClipBlitsToMapViewRegion( );

			SetFont(MapHeliSiteFont);
			// Green on green doesn't contrast well, use Yellow
			if ( fRefuelingSiteAvailable[ iCounter ] )
				SetFontForeground(FONT_MCOLOR_LTYELLOW);
			else
				SetFontForeground(FONT_MCOLOR_LTGRAY);
			SetFontBackground(FONT_MCOLOR_BLACK);

			// draw the text
			gprintfdirty( sLabelX, sLabelY, wString );
			mprintf( sLabelX, sLabelY, wString);

			// restore clip blits
			RestoreClipRegionToFullScreen( );
		}
	}

	return;
}



void BlitSAMGridMarkers( void )
{
	INT16 sScreenX = 0, sScreenY = 0;
	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;
	UINT16 usColor = 0;
	INT32 iCounter = 0;
	INT16 sWidth = 0, sHeight = 0;


	// get 16 bpp color
	usColor = Get16BPPColor( FROMRGB( 100, 100, 100) );

	pDestBuf = LockVideoSurface( guiSAVEBUFFER, &uiDestPitchBYTES );

	// clip to view region
	ClipBlitsToMapViewRegionForRectangleAndABit( uiDestPitchBYTES );

	// sam site rectangles
	for( iCounter = 0; iCounter < NUMBER_OF_SAMS; iCounter++ )
	{
		// has the sam site here been found?
		if( !fSamSiteFound[ iCounter ] )
		{
			continue;
		}

		// get location on screen
		GetScreenXYFromMapXY( gpSamSectorX[ iCounter ], gpSamSectorY[ iCounter ], &sScreenX, &sScreenY );
		sWidth = MAP_GRID_X;
		sHeight= MAP_GRID_Y;

		// draw rectangle
		RectangleDraw( TRUE, sScreenX, sScreenY - 1, sScreenX + sWidth, sScreenY + sHeight - 1, usColor, pDestBuf );
	}

	// refuel site rectangles
	for( iCounter = 0; iCounter < NUMBER_OF_REFUEL_SITES; iCounter++ )
	{
		// has the refuel site here been known or found?
		if( !fRefuelingSiteKnown[ iCounter ] && !GetSectorFlagStatus( sRefuelSectorX[ iCounter ], sRefuelSectorY[ iCounter ], 0, SF_ALREADY_VISITED ) )
		{
			continue;
		}

		// get location on screen
		GetScreenXYFromMapXY( sRefuelSectorX[ iCounter ], sRefuelSectorY[ iCounter ], &sScreenX, &sScreenY );
		sWidth = MAP_GRID_X;
		sHeight= MAP_GRID_Y;

		// draw rectangle
		RectangleDraw( TRUE, sScreenX, sScreenY - 1, sScreenX + sWidth, sScreenY + sHeight - 1, usColor, pDestBuf );
	}

	// restore clips
	RestoreClipRegionToFullScreenForRectangle( uiDestPitchBYTES );

	// unlock surface
	UnLockVideoSurface( guiSAVEBUFFER );


	return;
}


BOOLEAN CanMilitiaAutoDistribute( void )
{
	INT32 iTotalTroopsOnCursor = 0;
	INT32 iCounter = 0;
	INT16 sBaseSectorValue = 0, sCurrentSectorValue = 0;
	INT16 sSectorX = 0, sSectorY = 0;
	INT32 iTotalTroopsInTown = 0;

	// can't auto-distribute if we don't have a town selected (this excludes SAM sites)
	if( sSelectedMilitiaTown == BLANK_SECTOR )
		return( FALSE );

	// can't auto-distribute if we don't control any sectors in the the town
	if( !GetTownSectorsUnderControl( ( INT8 ) sSelectedMilitiaTown ) )
		return( FALSE );

	// get the sector value for the upper left corner
	sBaseSectorValue = GetBaseSectorForCurrentTown( );

	// render icons for map
	for( iCounter = 0; iCounter < MILITIA_BOXREGIONS; ++iCounter )
	{
		// grab current sector value
		sCurrentSectorValue = sBaseSectorValue + ( ( iCounter % MILITIA_BOX_ROWS ) + ( iCounter / MILITIA_BOX_ROWS ) * ( 16 ) );
		
		// skip sectors that's beyond sector P16
		if( sCurrentSectorValue > SEC_P16 )
		{
			continue;
		}

		sSectorX = SECTORX( sCurrentSectorValue );
		sSectorY = SECTORY( sCurrentSectorValue );

		// skip sectors not in the selected town (nearby other towns or wilderness SAM Sites)
		if( GetTownIdForSector( sSectorX, sSectorY ) != sSelectedMilitiaTown )
		{
			continue;
		}

		if( !StrategicMap[ CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY ) ].fEnemyControlled )
		{
			// get number of each
			iTotalTroopsInTown += MilitiaInSectorOfRankStationary( sSectorX, sSectorY, GREEN_MILITIA )
				+ MilitiaInSectorOfRankStationary( sSectorX, sSectorY, REGULAR_MILITIA )
				+ MilitiaInSectorOfRankStationary( sSectorX, sSectorY, ELITE_MILITIA );
		}
	}

	iTotalTroopsOnCursor = sGreensOnCursor + sRegularsOnCursor + sElitesOnCursor;

	// can't auto-distribute if we don't have any militia in the town
	if( !iTotalTroopsInTown )
		return( FALSE );

	// can auto-distribute
	return( TRUE );
}

void ShowDiseaseOnMap()
{
	INT16 sMapX, sMapY;
	INT16 sXCorner, sYCorner;
	INT16 usXPos, usYPos;
	CHAR16 sString[ 10 ];

	// clip blits to mapscreen region
	ClipBlitsToMapViewRegion( );

	SetFontDestBuffer( guiSAVEBUFFER, MapScreenRect.iLeft + 2, MapScreenRect.iTop, MapScreenRect.iRight, MapScreenRect.iBottom , FALSE );

	// HEADROCK HAM 5: Map Font now dynamic.
	INT32 MapItemsFont;
	if (iResolution <= _800x600 )
	{
		MapItemsFont = MAP_FONT;
	}
	else
	{
		MapItemsFont = FONT14ARIAL;
	}

	SetFont(MapItemsFont);
	SetFontForeground(FONT_MCOLOR_LTGREEN);
	SetFontBackground(FONT_MCOLOR_BLACK);

	// run through sectors
	for( sMapX = 1; sMapX < ( MAP_WORLD_X - 1 ); ++sMapX )
	{
		for( sMapY = 1; sMapY < ( MAP_WORLD_Y - 1 ); ++sMapY )
		{
			UINT8 sector = SECTOR( sMapX, sMapY );

			SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

			if ( pSectorInfo && ((pSectorInfo->usInfectionFlag & SECTORDISEASE_DIAGNOSED_PLAYER) || (gubFact[FACT_DISEASE_WHODATA_ACCESS] && pSectorInfo->usInfectionFlag & SECTORDISEASE_DIAGNOSED_WHO)) )
			{
				if ( fShowStrategicDiseaseFlag == MAPMODE_DISEASE )
				// hide this information, as it might allow the player to deduct enemy positions and numbers
				/*{
					UINT16 population = GetSectorPopulation( sMapX, sMapY );

					if ( population )
					{
						sXCorner = (INT16)(MAP_VIEW_START_X + (sMapX * MAP_GRID_X));
						sYCorner = (INT16)(MAP_VIEW_START_Y + (sMapY * MAP_GRID_Y));

						SetFontForeground( FONT_MCOLOR_WHITE );
						if ( pSectorInfo->usInfectionFlag & SECTORDISEASE_OUTBREAK )
							swprintf( sString, L"%d/%d", pSectorInfo->usInfected, population );
						else
							swprintf( sString, L"%d", population );

						FindFontCenterCoordinates( sXCorner, sYCorner, MAP_GRID_X, MAP_GRID_Y, sString, MapItemsFont, &usXPos, &usYPos );

						mprintf( usXPos, usYPos, sString );
					}
				}
				else if ( fShowStrategicDiseaseFlag == MAPMODE_SEVERITY )*/
				{
					if ( pSectorInfo->fInfectionSeverity > 0 )
					{
						sXCorner = (INT16)(MAP_VIEW_START_X + (sMapX * MAP_GRID_X));
						sYCorner = (INT16)(MAP_VIEW_START_Y + (sMapY * MAP_GRID_Y));

						SetFontForeground( FONT_MCOLOR_WHITE );
						swprintf( sString, L"%4.1f%%", 100 * pSectorInfo->fInfectionSeverity );

						FindFontCenterCoordinates( sXCorner, sYCorner, MAP_GRID_X, MAP_GRID_Y, sString, MapItemsFont, &usXPos, &usYPos );

						mprintf( usXPos, usYPos, sString );
					}
				}
			}
		}
	}

	// restore clip blits
	RestoreClipRegionToFullScreen( );
}

void ShowItemsOnMap( void )
{
	INT16 sMapX, sMapY;
	INT16 sXCorner, sYCorner;
	INT16 usXPos, usYPos;
	UINT32 uiItemCnt;
	CHAR16 sString[ 10 ];

	// clip blits to mapscreen region
	ClipBlitsToMapViewRegion( );

	SetFontDestBuffer( guiSAVEBUFFER, MapScreenRect.iLeft + 2, MapScreenRect.iTop, MapScreenRect.iRight, MapScreenRect.iBottom , FALSE );

	// HEADROCK HAM 5: Map Font now dynamic.
	INT32 MapItemsFont;
	if (iResolution <= _800x600 )
	{
		MapItemsFont = MAP_FONT;
	}
	else
	{
		MapItemsFont = FONT14ARIAL;
	}

	SetFont(MapItemsFont);
  SetFontForeground(FONT_MCOLOR_LTGREEN);
  SetFontBackground(FONT_MCOLOR_BLACK);

	// run through sectors
	for( sMapX = 1; sMapX < ( MAP_WORLD_X - 1 ); ++sMapX )
	{
		for( sMapY = 1; sMapY < ( MAP_WORLD_Y - 1 ); ++sMapY )
		{
			// to speed this up, only look at sector that player has visited
			if( GetSectorFlagStatus( sMapX, sMapY, ( UINT8 ) iCurrentMapSectorZ, SF_ALREADY_VISITED ) )
			{
//				uiItemCnt = GetSizeOfStashInSector( sMapX, sMapY, ( INT16 ) iCurrentMapSectorZ, FALSE );
				uiItemCnt = GetNumberOfVisibleWorldItemsFromSectorStructureForSector( sMapX, sMapY, ( UINT8 ) iCurrentMapSectorZ );

				if ( uiItemCnt > 0 )
				{
					sXCorner = ( INT16 )( MAP_VIEW_START_X + ( sMapX * MAP_GRID_X ) );
					sYCorner = ( INT16 )( MAP_VIEW_START_Y + ( sMapY * MAP_GRID_Y ) );

					swprintf( sString, L"%d", uiItemCnt );

					FindFontCenterCoordinates( sXCorner, sYCorner, MAP_GRID_X, MAP_GRID_Y, sString, MapItemsFont, &usXPos, &usYPos );
	//				sXPos -= StringPixLength( sString, MAP_FONT ) / 2;

					gprintfdirty( usXPos, usYPos, sString );
					mprintf( usXPos, usYPos, sString );
				}
			}
		}
	}

	// restore clip blits
	RestoreClipRegionToFullScreen( );
}


void DrawMapBoxIcon( HVOBJECT hIconHandle, UINT16 usVOIndex, INT16 sMapX, INT16 sMapY, UINT8 ubIconPosition )
{
	INT32 iRowNumber, iColumnNumber;
	INT32 iX, iY;

	UINT8 iconOffsetX = 2;
	UINT8 iconOffsetY = 1;
	UINT8 iconSize = 0;
	
	// don't show any more icons than will fit into one sector, to keep them from spilling into sector(s) beneath
	if ( ubIconPosition >= (MERC_ICONS_PER_LINE * ROWS_PER_SECTOR) )
	{
		return;
	}

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		iconSize = 3;
	}
	else if (iResolution < _1024x768)
	{
		iconSize = 4;
	}
	else
	{
		iconSize = 6;
	}

	iColumnNumber = ubIconPosition % MERC_ICONS_PER_LINE;
	iRowNumber	= ubIconPosition / MERC_ICONS_PER_LINE;

	iX = MAP_VIEW_START_X + ( sMapX * MAP_GRID_X ) + iconOffsetX + ( iconSize * iColumnNumber );
	iY = MAP_VIEW_START_Y + ( sMapY * MAP_GRID_Y ) + iconOffsetY + ( iconSize * iRowNumber );

	BltVideoObject( guiSAVEBUFFER, hIconHandle, usVOIndex , iX, iY, VO_BLT_SRCTRANSPARENCY, NULL );
	InvalidateRegion( iX, iY, iX + DMAP_GRID_X, iY + DMAP_GRID_Y );
}



void DrawOrta()
{
	INT16 sX, sY;
	UINT8 ubVidObjIndex;
	HVOBJECT hHandle;

	GetScreenXYFromMapXY( ORTA_SECTOR_X, ORTA_SECTOR_Y, &sX, &sY );
	sX += +2;
	sY += -3;
	ubVidObjIndex = 1;

	// draw Orta in its sector
	GetVideoObject( &hHandle, guiORTAICON);
	BltVideoObject( guiSAVEBUFFER, hHandle, ubVidObjIndex, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
}


void DrawTixa()
{
	INT16 sX, sY;
	UINT8 ubVidObjIndex;
	HVOBJECT hHandle;

	GetScreenXYFromMapXY( TIXA_SECTOR_X, TIXA_SECTOR_Y, &sX, &sY );
	sY += +2;
	ubVidObjIndex = 1;

	// draw Tixa in its sector
	GetVideoObject( &hHandle, guiTIXAICON);
	BltVideoObject( guiSAVEBUFFER, hHandle, ubVidObjIndex, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
}



void DrawBullseye()
{
	INT16 sX, sY;
	HVOBJECT hHandle;

	GetScreenXYFromMapXY( gsMercArriveSectorX, gsMercArriveSectorY, &sX, &sY );
	sY -= 2;

	// draw the bullseye in that sector
	GetVideoObject( &hHandle, guiBULLSEYE);

	if (iResolution >= _800x600)
	{
		sX = sX + MAP_GRID_X / 2 - 10;
		sY = sY + 1 + MAP_GRID_Y / 2 - 10;
	}

	BltVideoObject( guiSAVEBUFFER, hHandle, 0, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
}



void HideExistenceOfUndergroundMapSector( UINT8 ubSectorX, UINT8 ubSectorY )
{
	INT16 sScreenX;
	INT16 sScreenY;

	GetScreenXYFromMapXY( ubSectorX, ubSectorY, &sScreenX, &sScreenY );

	// fill it with near black
	ColorFillVideoSurfaceArea( guiSAVEBUFFER, sScreenX + 1, sScreenY, sScreenX + MAP_GRID_X,	sScreenY + MAP_GRID_Y - 1, gusUndergroundNearBlack );
}


void InitMapSecrets( void )
{
	//UINT8 ubSamIndex;
	INT32 iCounter2 = 0;

	fFoundTixa = FALSE;
	fFoundOrta = FALSE;
	
		for( iCounter2 = 1; iCounter2 < NUM_TOWNS; iCounter2++ )
		{
			gfHiddenTown [ iCounter2 ] = gfHiddenTownTemp [ iCounter2 ]; 
			gfDrawHiddenTown[ iCounter2 ] = FALSE; 
			gfIconTown [ iCounter2 ] = gfIconTownTemp [ iCounter2 ]; 
		}

	//for( ubSamIndex = 0; ubSamIndex < NUMBER_OF_SAMS; ubSamIndex++ )
	//{
	//	fSamSiteFound[ ubSamIndex ] = fSamSiteFoundOrig[ ubSamIndex ];
	//}
	memcpy(fSamSiteFound, fSamSiteFoundOrig, sizeof(fSamSiteFound));
}



BOOLEAN CanRedistributeMilitiaInSector( INT16 sClickedSectorX, INT16 sClickedSectorY, INT8 bClickedTownId )
{
	// if no world is loaded, we can't be in combat (PBI/Auto-resolve locks out normal mapscreen interface for this)
	if( !gfWorldLoaded )
	{
		// ok to redistribute
		return( TRUE );
	}

	// if tactically not in combat, hostile sector, or air-raid
	if( !( gTacticalStatus.uiFlags & INCOMBAT ) && !( gTacticalStatus.fEnemyInSector ) && !InAirRaid( ) )
	{
		// ok to redistribute
		return( TRUE );
	}

	// if the fight is underground
	if ( gbWorldSectorZ != 0 )
	{
		// ok to redistribute
		return( TRUE );
	}

	INT32 iCounter = 0;
	INT16 sBaseSectorValue = 0, sCurrentSectorValue = 0;
	INT16 sSectorX = 0, sSectorY = 0;
	// currently loaded surface sector IS hostile - so we must check if it's also one of the sectors in this "militia map"

	// get the sector value for the upper left corner
	sBaseSectorValue = sBaseSectorList[ bClickedTownId - 1 ];

	// render icons for map
	for( iCounter = 0; iCounter < MILITIA_BOXREGIONS; ++iCounter )
	{
		// grab current sector value
		sCurrentSectorValue = sBaseSectorValue + ( ( iCounter % MILITIA_BOX_ROWS ) + ( iCounter / MILITIA_BOX_ROWS ) * ( 16 ) );
		
		// skip sectors that's beyond sector P16
		if( sCurrentSectorValue > SEC_P16 )
		{
			continue;
		}

		sSectorX = SECTORX( sCurrentSectorValue );
		sSectorY = SECTORY( sCurrentSectorValue );

		// not in the same town?
		if( StrategicMap[ CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY ) ].bNameId != bClickedTownId )
		{
			continue;
		}

		// if this is the loaded sector that is currently hostile
		if ( ( sSectorX == gWorldSectorX ) && ( sSectorY == gWorldSectorY ) )
		{
			// the fight is within this town!  Can't redistribute.
			return( FALSE );
		}
		// to prevent radio operator exploits for hostile sectors adjacent to towns
		else if ( ( sSectorX == gWorldSectorX  &&  sSectorY == gWorldSectorY + 1 ) || ( sSectorX == gWorldSectorX + 1 &&  sSectorY == gWorldSectorY ) ||
				( sSectorX == gWorldSectorX - 1 &&  sSectorY == gWorldSectorY ) || ( sSectorX == gWorldSectorX  &&  sSectorY == gWorldSectorY - 1 ) )
		{
			return( FALSE );
		}
	}

	// the fight is elsewhere - ok to redistribute
	return( TRUE );
}

// Callback for disbanding militia in the Militia Box
void MilitiaDisbandYesNoBoxCallback( UINT8 bExitValue )
{
	// yes
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		if( sSelectedMilitiaTown != BLANK_SECTOR )
		{
			// HEADROCK HAM 3.6: Adjust loyalty based on how many militia were disbanded, compared to
			// how many can be trained in a single session (max).
			UINT32 uiMilitiaToDisband = sGreensOnCursor + (2*sRegularsOnCursor) + (3*sElitesOnCursor);
			UINT32 uiTownLoyaltyBonus = (LOYALTY_BONUS_FOR_TOWN_TRAINING * uiMilitiaToDisband) / gGameExternalOptions.iTrainingSquadSize;
			// loyalty in this town increases a bit because we obviously care about them...
			DecrementTownLoyalty( (UINT8)sSelectedMilitiaTown, uiTownLoyaltyBonus );
		}

		// zero out numbers on the cursor
		sGreensOnCursor = 0;
		sRegularsOnCursor = 0;
		sElitesOnCursor = 0;

		// reset fact we are in the box
		sSelectedMilitiaTown = 0;
		fMapPanelDirty = TRUE;
		// Go ahead and reset the militia in the selected sector
		ResetMilitia();
	}
	else if( bExitValue == MSG_BOX_RETURN_NO )
	{

}
}

#ifdef JA2UB
void SetUpValidCampaignSectors( void )
{
/*
INT32			iRow, iCol;
	
	for( iRow=1; iRow<=16; iRow++ )
	{
		for( iCol=1; iCol<=16; iCol++ )
		{
			SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fValidSector = FALSE;
			sBadSectorsList[iCol][iRow] = 1;
//			SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fCampaignSector = FALSE;
		}
	}
*/
	
#ifdef JA2UB_PRESS_PREVIEW	

	INT32			iRow, iCol;
	
	for( iRow=1; iRow<=16; iRow++ )
	{
		for( iCol=1; iCol<=16; iCol++ )
		{
			SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fValidSector = FALSE;	
			SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fCampaignSector = FALSE;
		}
	}

#endif


	SectorInfo[ ( SECTOR( 7 , 8 ) ) ].fValidSector = TRUE;		//H7
	SectorInfo[ ( SECTOR( 7 , 8 ) ) ].fCampaignSector = TRUE;

	SectorInfo[ ( SECTOR( 8 , 8 ) ) ].fValidSector = TRUE;		//H8
	SectorInfo[ ( SECTOR( 8 , 8 ) ) ].fCampaignSector = TRUE;	

	SectorInfo[ ( SECTOR( 9 , 8 ) ) ].fValidSector = TRUE;		//H9
	SectorInfo[ ( SECTOR( 9 , 8 ) ) ].fCampaignSector = TRUE;		//H9

	SectorInfo[ ( SECTOR( 10 , 8 ) ) ].fValidSector = TRUE;		//H10
	SectorInfo[ ( SECTOR( 10 , 8 ) ) ].fCampaignSector = TRUE;		

#ifndef JA2UB_PRESS_PREVIEW

	SectorInfo[ ( SECTOR( 11, 8 ) ) ].fValidSector = TRUE;		//H11
	SectorInfo[ ( SECTOR( 11, 8 ) ) ].fCampaignSector = TRUE;		

#endif

	SectorInfo[ ( SECTOR( 9 , 9 ) ) ].fValidSector = TRUE;		//I9
	SectorInfo[ ( SECTOR( 9 , 9 ) ) ].fCampaignSector = TRUE;		

	SectorInfo[ ( SECTOR( 10 , 9 ) ) ].fValidSector = TRUE;		//I10
	SectorInfo[ ( SECTOR( 10 , 9 ) ) ].fCampaignSector = TRUE;		

#ifndef JA2UB_PRESS_PREVIEW

	SectorInfo[ ( SECTOR( 11 , 9 ) ) ].fValidSector = TRUE;		//I11
	SectorInfo[ ( SECTOR( 11 , 9 ) ) ].fCampaignSector = TRUE;	

	SectorInfo[ ( SECTOR( 12 , 9 ) ) ].fValidSector = TRUE;		//I12
	SectorInfo[ ( SECTOR( 12 , 9 ) ) ].fCampaignSector = TRUE;		

	SectorInfo[ ( SECTOR( 13 , 9 ) ) ].fValidSector = TRUE;		//I13
	SectorInfo[ ( SECTOR( 13 , 9 ) ) ].fCampaignSector = TRUE;		

	SectorInfo[ ( SECTOR( 11 , 10 ) ) ].fValidSector = TRUE;		//J11
	SectorInfo[ ( SECTOR( 11 , 10 ) ) ].fCampaignSector = TRUE;		

	SectorInfo[ ( SECTOR( 12 , 10 ) ) ].fValidSector = TRUE;		//J12
	SectorInfo[ ( SECTOR( 12 , 10 ) ) ].fCampaignSector = TRUE;		

	SectorInfo[ ( SECTOR( 13 , 10 ) ) ].fValidSector = TRUE;		//J13
	SectorInfo[ ( SECTOR( 13 , 10 ) ) ].fCampaignSector = TRUE;		

	SectorInfo[ ( SECTOR( 14 , 10 ) ) ].fValidSector = TRUE;		//J14
	SectorInfo[ ( SECTOR( 14 , 10 ) ) ].fCampaignSector = TRUE;		

	SectorInfo[ ( SECTOR( 14 , 11 ) ) ].fValidSector = TRUE;		//K14
	SectorInfo[ ( SECTOR( 14 , 11 ) ) ].fCampaignSector = TRUE;		

	SectorInfo[ ( SECTOR( 15 , 11 ) ) ].fValidSector = TRUE;		//K15
	SectorInfo[ ( SECTOR( 15 , 11 ) ) ].fCampaignSector = TRUE;		

	SectorInfo[ ( SECTOR( 16 , 11 ) ) ].fValidSector = TRUE;		//K16
	SectorInfo[ ( SECTOR( 16 , 11 ) ) ].fCampaignSector = TRUE;		

	SectorInfo[ ( SECTOR( 15 , 12 ) ) ].fValidSector = TRUE;		//L15
	SectorInfo[ ( SECTOR( 15 , 12 ) ) ].fCampaignSector = TRUE;		

#endif


}
#endif

BOOLEAN SaveHiddenTownToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;

	FileWrite( hFile, &gfHiddenTown, sizeof( gfHiddenTown), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gfHiddenTown ) )
	{
		return( FALSE );
	}
	
	FileWrite( hFile, &gfDrawHiddenTown, sizeof( gfDrawHiddenTown), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gfDrawHiddenTown ) )
	{
		return( FALSE );
	}
	
	FileWrite( hFile, &gfIconTown, sizeof( gfIconTown), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gfIconTown ) )
	{
		return( FALSE );
	}
	
	return( TRUE );
}

BOOLEAN LoadHiddenTownFromLoadGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;

	FileRead( hFile, &gfHiddenTown, sizeof( gfHiddenTown), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gfHiddenTown ) )
	{
		return( FALSE );
	}
	
	FileRead( hFile, &gfDrawHiddenTown, sizeof( gfDrawHiddenTown), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gfDrawHiddenTown ) )
	{
		return( FALSE );
	}
	
	FileRead( hFile, &gfIconTown, sizeof( gfIconTown), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gfIconTown ) )
	{
		return( FALSE );
	}

	return( TRUE );
}

// Flugente: militia pathing
BOOLEAN gMilitiaGroupBoxCreated = FALSE;
UINT16 gMilitiaGroupBoxX = 0;
UINT16 gMilitiaGroupBoxY = 0;

#define MILITIAGROUPBOX_REGION_MAX	10

BOOLEAN gMilitiaGroupBoxRegionDefined[MILITIAGROUPBOX_REGION_MAX];
MOUSE_REGION	gMilitiaGroupBoxRegion[MILITIAGROUPBOX_REGION_MAX];

BOOLEAN gMilitiaGroupBoxDummyRegionDefined;
MOUSE_REGION	gMilitiaGroupBoxDummyRegion;

// +/- buttons to add/remove militia from a group
INT32 gMilitiaGroupBoxButtonImage[2] = {-1};
INT32 gMilitiaGroupBoxButton[2 * MAX_MILITIA_LEVELS] = {-1};
BOOLEAN gMilitiaGroupBoxButtonCreated[2 * MAX_MILITIA_LEVELS] = {FALSE};

UINT32 guiMilitiaGroupBoxTypeSymbols;

void CreateMilitiaGroupBoxVideoObjects()
{
	gMilitiaGroupBoxButtonImage[0] = LoadButtonImage( "INTERFACE\\plusminusbuttons.sti", -1, 2, -1, 2, -1 );
	gMilitiaGroupBoxButtonImage[1] = LoadButtonImage( "INTERFACE\\plusminusbuttons.sti", -1, 3, -1, 3, -1 );
}

void CreateMilitiaGroupBox()
{
	INT16 sMapX, sMapY;
	
	if ( GetMouseMapXY( &sMapX, &sMapY ) )
	{
		// if we are plotting in the upper left corner, get another position for the box
		if ( sMapX < 8 && sMapY < 8 )
		{
			gMilitiaGroupBoxX = MapScreenRect.iRight - 210;
			gMilitiaGroupBoxY = MapScreenRect.iBottom - 200;
		}
		else
		{
			gMilitiaGroupBoxX = MapScreenRect.iLeft;
			gMilitiaGroupBoxY = MapScreenRect.iTop;
		}

		gMilitiaGroupBoxCreated = TRUE;
	}
}

void DestroyMilitiaGroupBox()
{
	for ( int i = 0; i < MILITIAGROUPBOX_REGION_MAX; ++i )
	{
		if ( gMilitiaGroupBoxRegionDefined[i] )
		{
			MSYS_RemoveRegion( &gMilitiaGroupBoxRegion[i] );
			gMilitiaGroupBoxRegionDefined[i] = FALSE;
		}
	}

	if ( gMilitiaGroupBoxDummyRegionDefined )
	{
		MSYS_RemoveRegion( &gMilitiaGroupBoxDummyRegion );
		gMilitiaGroupBoxDummyRegionDefined = FALSE;
	}

	if ( gMilitiaGroupBoxButtonImage[0] > -1 )
	{
		UnloadButtonImage( gMilitiaGroupBoxButtonImage[0] );
		gMilitiaGroupBoxButtonImage[0] = -1;
	}

	if ( gMilitiaGroupBoxButtonImage[1] > -1 )
	{
		UnloadButtonImage( gMilitiaGroupBoxButtonImage[1] );
		gMilitiaGroupBoxButtonImage[1] = -1;
	}

	for ( int i = 0; i < 2 * MAX_MILITIA_LEVELS; ++i )
	{
		if ( gMilitiaGroupBoxButtonCreated[i] )
		{
			// delete militia panel bottom
			RemoveButton( gMilitiaGroupBoxButton[i] );

			gMilitiaGroupBoxButtonCreated[i] = FALSE;
		}
	}
}

void MilitiaGroupBoxRegionClickCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	if ( (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP) )
	{
		UINT8 usValue = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );

		INT16 slot = GetMilitiaPathSlot( usValue );

		if ( slot > -1 )
		{
			gMilitiaGroupId = (UINT8)slot;
		}
	}

	if ( (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP) )
	{
		
	}
}

void MilitiaGroupBoxButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	/*{
		btn->uiFlags |= (BUTTON_CLICKED_ON);
	}
	else if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )*/
	{
		if ( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);

			GROUP* pGroup = GetGroup( gMilitiaPath[gMilitiaGroupId].sGroupid );

			if ( pGroup && pGroup->usGroupTeam == MILITIA_TEAM )
			{
				UINT8 id = (UINT8)btn->UserData[0];

				UINT8 militiatype = id / 2;

				// for now, always alter by 1
				UINT8 howmany = 1;

				// either add or remove a militia from currently selected group
				if ( id % 2 )
				{
					// remove militia from group
					// first, check whether we can reduce the number of militia in the group
					switch ( militiatype )
					{
					case GREEN_MILITIA:		if ( pGroup->pEnemyGroup->ubNumAdmins < howmany )	return;	break;
					case REGULAR_MILITIA:	if ( pGroup->pEnemyGroup->ubNumTroops < howmany )	return;	break;
					case ELITE_MILITIA:		if ( pGroup->pEnemyGroup->ubNumElites < howmany )	return;	break;
						// this shouldn't happen...
						default: return; break;
					}

					// add one militia to the sector garrison
					StrategicAddMilitiaToSector( pGroup->ubSectorX, pGroup->ubSectorY, militiatype, howmany );

					// remove one militia from the group
					switch ( militiatype )
					{
					case GREEN_MILITIA:		pGroup->pEnemyGroup->ubNumAdmins -= howmany;		break;
					case REGULAR_MILITIA:	pGroup->pEnemyGroup->ubNumTroops -= howmany;		break;
					case ELITE_MILITIA:		pGroup->pEnemyGroup->ubNumElites -= howmany;		break;
					}
				}
				else
				{
					// add militia to group
					// first, check whether we can reduce the number of militia in the sector
					INT16 sMapX = GET_X_FROM_STRATEGIC_INDEX( gMilitiaPlotStartSector );
					INT16 sMapY = GET_Y_FROM_STRATEGIC_INDEX( gMilitiaPlotStartSector );
					if ( MilitiaInSectorOfRankStationary( sMapX, sMapY, militiatype ) < howmany )
					{
						return;
					}

					// remove one militia from the sector garrison
					StrategicRemoveMilitiaFromSector( pGroup->ubSectorX, pGroup->ubSectorY, militiatype, howmany );

					switch ( militiatype )
					{
					case GREEN_MILITIA:		pGroup->pEnemyGroup->ubNumAdmins += howmany;	break;
					case REGULAR_MILITIA:	pGroup->pEnemyGroup->ubNumTroops += howmany;	break;
					case ELITE_MILITIA:		pGroup->pEnemyGroup->ubNumElites += howmany;	break;
					}
				}
			}
		}
	}
}

void DisplayMilitiaGroupBox()
{
	// if we play with a limited militia pool, show us how many we have
	if ( fShowMilitia && gGameExternalOptions.fMilitiaVolunteerPool )
	{
		CHAR16 sVolunteerString[200];

		SetFont( FONT12ARIAL );

		if ( GetVolunteerPool() >= 2 * gGameExternalOptions.iTrainingSquadSize )
			SetFontForeground( FONT_FCOLOR_GREEN );
		else if ( GetVolunteerPool( ) >= gGameExternalOptions.iTrainingSquadSize )
			SetFontForeground( FONT_FCOLOR_YELLOW );
		else
			SetFontForeground( FONT_FCOLOR_RED );

		// header
		swprintf( sVolunteerString, szMilitiaStrategicMovementText[8], GetVolunteerPool( ), CalcHourlyVolunteerGain( ) );
		mprintf( MapScreenRect.iLeft + 20, MapScreenRect.iBottom - 10, sVolunteerString );
	}

	if ( !gMilitiaGroupBoxCreated )
		return;

	// destroy the old stuff anyway...
	DestroyMilitiaGroupBox();

	if ( !fPlotForMilitia )
	{
		gMilitiaGroupBoxCreated = FALSE;
		return;
	}

	SetFontForeground( FONT_FCOLOR_WHITE );

	CreateMilitiaGroupBoxVideoObjects();

	// move to defines later
	UINT16 MILITIAGROUPBOX_WIDTH_NAME = 80;
	UINT16 MILITIAGROUPBOX_WIDTH_TYPEBOX = 20;
	UINT16 MILITIAGROUPBOX_WIDTH_SECTOR = 30;
	UINT16 MILITIAGROUPBOX_WIDTH_ETA = 30;

	UINT16 MILITIAGROUPBOX_HEIGHT_LINE = 14;

	UINT16 MILITIAGROUPBOX_WIDTH = MILITIAGROUPBOX_WIDTH_NAME + 3 * MILITIAGROUPBOX_WIDTH_TYPEBOX + 2 * MILITIAGROUPBOX_WIDTH_SECTOR + MILITIAGROUPBOX_WIDTH_ETA;

	// we need a header line, one for the sector, and one for each group present
	UINT8 usMapX = GET_X_FROM_STRATEGIC_INDEX( gMilitiaPlotStartSector );
	UINT8 usMapY = GET_Y_FROM_STRATEGIC_INDEX( gMilitiaPlotStartSector );

	int linecnt = 2;
	{
		GROUP *pGroup = gpGroupList;
		while ( pGroup )
		{
			if ( pGroup->usGroupTeam == MILITIA_TEAM && pGroup->ubSectorX == usMapX && pGroup->ubSectorY == usMapY )
				++linecnt;

			pGroup = pGroup->next;
		}
	}

	// not more groups than allowed though
	linecnt = min( linecnt, 2 + MILITIAGROUPBOX_REGION_MAX );

	UINT16 MILITIAGROUPBOX_HEIGHT = linecnt * MILITIAGROUPBOX_HEIGHT_LINE;

	SetFontForeground( FONT_FCOLOR_WHITE );
	SetFontBackground( FONT_MCOLOR_BLACK );
	
	//Display the background
	ColorFillVideoSurfaceArea( FRAME_BUFFER, gMilitiaGroupBoxX, gMilitiaGroupBoxY, gMilitiaGroupBoxX + MILITIAGROUPBOX_WIDTH, gMilitiaGroupBoxY + MILITIAGROUPBOX_HEIGHT, Get16BPPColor( FROMRGB( 50, 50, 50 ) ) );

	// mouse region - clicking on a group name selects that group
	if ( !gMilitiaGroupBoxDummyRegionDefined )
	{
		MSYS_DefineRegion( &gMilitiaGroupBoxDummyRegion, gMilitiaGroupBoxX, gMilitiaGroupBoxY, gMilitiaGroupBoxX + MILITIAGROUPBOX_WIDTH, gMilitiaGroupBoxY + MILITIAGROUPBOX_HEIGHT, MSYS_PRIORITY_HIGH,
						   MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
		MSYS_AddRegion( &gMilitiaGroupBoxDummyRegion );
		gMilitiaGroupBoxDummyRegionDefined = TRUE;
	}
	
	UINT16 dispX = gMilitiaGroupBoxX;
	UINT16 dispY = gMilitiaGroupBoxY + 2;
	
	CHAR16 sString[200];
	
	// header
	swprintf( sString, szMilitiaStrategicMovementText[2] );
	mprintf( dispX, dispY, sString );
	dispX += MILITIAGROUPBOX_WIDTH_NAME;
	
	// display coloured boxes to represent different militia types
	ColorFillVideoSurfaceArea( FRAME_BUFFER, dispX, dispY, dispX + 6, dispY + 6, Get16BPPColor( FROMRGB( 0, 231, 0 ) ) );
	dispX += MILITIAGROUPBOX_WIDTH_TYPEBOX;
	ColorFillVideoSurfaceArea( FRAME_BUFFER, dispX, dispY, dispX + 6, dispY + 6, Get16BPPColor( FROMRGB( 0, 162, 247 ) ) );
	dispX += MILITIAGROUPBOX_WIDTH_TYPEBOX;
	ColorFillVideoSurfaceArea( FRAME_BUFFER, dispX, dispY, dispX + 6, dispY + 6, Get16BPPColor( FROMRGB( 0, 113, 247 ) ) );
	dispX += MILITIAGROUPBOX_WIDTH_TYPEBOX;
	
	swprintf( sString, szMilitiaStrategicMovementText[3] );
	mprintf( dispX, dispY, sString );
	dispX += MILITIAGROUPBOX_WIDTH_SECTOR;

	swprintf( sString, szMilitiaStrategicMovementText[4] );
	mprintf( dispX, dispY, sString );
	dispX += MILITIAGROUPBOX_WIDTH_ETA;

	swprintf( sString, szMilitiaStrategicMovementText[7] );
	mprintf( dispX, dispY, sString );
	dispX += MILITIAGROUPBOX_WIDTH_SECTOR;

	dispX = gMilitiaGroupBoxX;
	dispY += MILITIAGROUPBOX_HEIGHT_LINE;

	ColorFillVideoSurfaceArea( FRAME_BUFFER, dispX, dispY - 2, dispX + MILITIAGROUPBOX_WIDTH, dispY - 1, Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );
	
	// stationary in sector
	UINT8 militiastat[MAX_MILITIA_LEVELS];
	for ( int i = 0; i < MAX_MILITIA_LEVELS; ++i )
		militiastat[i] = MilitiaInSectorOfRankStationary( usMapX, usMapY, i );

	swprintf( sString, szMilitiaStrategicMovementText[1] );
	mprintf( dispX, dispY, sString );
	dispX += MILITIAGROUPBOX_WIDTH_NAME;

	swprintf( sString, L"%d", militiastat[GREEN_MILITIA] );
	mprintf( dispX, dispY, sString );
	dispX += MILITIAGROUPBOX_WIDTH_TYPEBOX;

	swprintf( sString, L"%d", militiastat[REGULAR_MILITIA] );
	mprintf( dispX, dispY, sString );
	dispX += MILITIAGROUPBOX_WIDTH_TYPEBOX;

	swprintf( sString, L"%d", militiastat[ELITE_MILITIA] );
	mprintf( dispX, dispY, sString );
	dispX += MILITIAGROUPBOX_WIDTH_TYPEBOX;

	swprintf( sString, L"--" );
	mprintf( dispX, dispY, sString );
	dispX += MILITIAGROUPBOX_WIDTH_SECTOR;

	swprintf( sString, L"--:--" );
	mprintf( dispX, dispY, sString );
	dispX += MILITIAGROUPBOX_WIDTH_ETA;

	swprintf( sString, L"--" );
	mprintf( dispX, dispY, sString );
	dispX += MILITIAGROUPBOX_WIDTH_SECTOR;

	dispY += MILITIAGROUPBOX_HEIGHT_LINE;

	int groupcnt = 0;
	int buttoncnt = 0;

	GROUP *pGroup = gpGroupList;
	while ( pGroup && groupcnt < MILITIAGROUPBOX_REGION_MAX )
	{
		if ( pGroup->usGroupTeam == MILITIA_TEAM && pGroup->ubSectorX == usMapX && pGroup->ubSectorY == usMapY )
		{
			dispX = gMilitiaGroupBoxX;

			if ( gMilitiaPath[gMilitiaGroupId].sGroupid == pGroup->ubGroupID )
			{
				SetFontForeground( FONT_FCOLOR_YELLOW );
			}
			else
			{
				SetFontForeground( FONT_FCOLOR_WHITE );
			}

			CHAR16 wSectorName[64];
			GetShortSectorString( pGroup->ubNextX, pGroup->ubNextY, wSectorName );

			CHAR16 wFinalSectorName[64];
			swprintf( sString, L"--" );

			// determine the final destination of this group
			{
				INT16 finalsector = GetLastSectorIdInMilitiaGroupPath( pGroup->ubGroupID );
				if ( finalsector > -1 )
				{
					GetShortSectorString( SECTORX( finalsector ), SECTORY( finalsector ), wFinalSectorName );
				}
				else
				{
					GetShortSectorString( pGroup->ubNextX, pGroup->ubNextY, wFinalSectorName );
				}
			}

			// show ETA
			CHAR16 timestring[64];
			UINT32 uiArrivalTime = GetWorldTotalMin( ) + CalculateTravelTimeOfGroup( pGroup );
			ConvertMinTimeToDayHourMinString( uiArrivalTime, timestring );

			// mouse region - clicking on a group name selects that group
			if ( !gMilitiaGroupBoxRegionDefined[groupcnt] )
			{
				MSYS_DefineRegion( &gMilitiaGroupBoxRegion[groupcnt], dispX, dispY, dispX + MILITIAGROUPBOX_WIDTH_NAME, dispY + MILITIAGROUPBOX_HEIGHT_LINE, MSYS_PRIORITY_HIGH,
								   MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MilitiaGroupBoxRegionClickCallback );
				MSYS_AddRegion( &gMilitiaGroupBoxRegion[groupcnt] );
				MSYS_SetRegionUserData( &gMilitiaGroupBoxRegion[groupcnt], 0, pGroup->ubGroupID );
				gMilitiaGroupBoxRegionDefined[groupcnt] = TRUE;
			}

			if ( pGroup->ubGroupID == gNewMilitiaGroupId )
				swprintf( sString, szMilitiaStrategicMovementText[5], pGroup->ubGroupID );
			else
				swprintf( sString, szMilitiaStrategicMovementText[6], pGroup->ubGroupID );
			mprintf( dispX, dispY, sString );
			dispX += MILITIAGROUPBOX_WIDTH_NAME;

			swprintf( sString, L"%d", pGroup->pEnemyGroup->ubNumAdmins );
			mprintf( dispX, dispY, sString );

			if ( gMilitiaPath[gMilitiaGroupId].sGroupid == pGroup->ubGroupID )
			{
				// '+' button
				if ( militiastat[GREEN_MILITIA] && !gMilitiaGroupBoxButtonCreated[buttoncnt] )
				{
					gMilitiaGroupBoxButton[buttoncnt] = QuickCreateButton( gMilitiaGroupBoxButtonImage[0], dispX + 14, dispY, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
																		   DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)MilitiaGroupBoxButtonCallback );
					ButtonList[gMilitiaGroupBoxButton[buttoncnt]]->UserData[0] = buttoncnt;
					gMilitiaGroupBoxButtonCreated[buttoncnt] = TRUE;
				}
				++buttoncnt;

				// '-' button
				if ( pGroup->pEnemyGroup->ubNumAdmins && !gMilitiaGroupBoxButtonCreated[buttoncnt] )
				{
					gMilitiaGroupBoxButton[buttoncnt] = QuickCreateButton( gMilitiaGroupBoxButtonImage[1], dispX + 14, dispY + 6, BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
																		   DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)MilitiaGroupBoxButtonCallback );
					ButtonList[gMilitiaGroupBoxButton[buttoncnt]]->UserData[0] = buttoncnt;
					gMilitiaGroupBoxButtonCreated[buttoncnt] = TRUE;
				}
				++buttoncnt;
			}

			dispX += MILITIAGROUPBOX_WIDTH_TYPEBOX;
						
			swprintf( sString, L"%d", pGroup->pEnemyGroup->ubNumTroops );
			mprintf( dispX, dispY, sString );

			if ( gMilitiaPath[gMilitiaGroupId].sGroupid == pGroup->ubGroupID )
			{
				// '+' button
				if ( militiastat[REGULAR_MILITIA] && !gMilitiaGroupBoxButtonCreated[buttoncnt] )
				{
					gMilitiaGroupBoxButton[buttoncnt] = QuickCreateButton( gMilitiaGroupBoxButtonImage[0], dispX + 14, dispY, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
																		   DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)MilitiaGroupBoxButtonCallback );
					ButtonList[gMilitiaGroupBoxButton[buttoncnt]]->UserData[0] = buttoncnt;
					gMilitiaGroupBoxButtonCreated[buttoncnt] = TRUE;
				}
				++buttoncnt;

				// '-' button
				if ( pGroup->pEnemyGroup->ubNumTroops && !gMilitiaGroupBoxButtonCreated[buttoncnt] )
				{
					gMilitiaGroupBoxButton[buttoncnt] = QuickCreateButton( gMilitiaGroupBoxButtonImage[1], dispX + 14, dispY + 6, BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
																		   DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)MilitiaGroupBoxButtonCallback );
					ButtonList[gMilitiaGroupBoxButton[buttoncnt]]->UserData[0] = buttoncnt;
					gMilitiaGroupBoxButtonCreated[buttoncnt] = TRUE;
				}
				++buttoncnt;
			}

			dispX += MILITIAGROUPBOX_WIDTH_TYPEBOX;

			swprintf( sString, L"%d", pGroup->pEnemyGroup->ubNumElites );
			mprintf( dispX, dispY, sString );

			if ( gMilitiaPath[gMilitiaGroupId].sGroupid == pGroup->ubGroupID )
			{
				// '+' button
				if ( militiastat[ELITE_MILITIA] && !gMilitiaGroupBoxButtonCreated[buttoncnt] )
				{
					gMilitiaGroupBoxButton[buttoncnt] = QuickCreateButton( gMilitiaGroupBoxButtonImage[0], dispX + 14, dispY, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
																		   DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)MilitiaGroupBoxButtonCallback );
					ButtonList[gMilitiaGroupBoxButton[buttoncnt]]->UserData[0] = buttoncnt;
					gMilitiaGroupBoxButtonCreated[buttoncnt] = TRUE;
				}
				++buttoncnt;

				// '-' button
				if ( pGroup->pEnemyGroup->ubNumElites && !gMilitiaGroupBoxButtonCreated[buttoncnt] )
				{
					gMilitiaGroupBoxButton[buttoncnt] = QuickCreateButton( gMilitiaGroupBoxButtonImage[1], dispX + 14, dispY + 6, BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
																		   DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)MilitiaGroupBoxButtonCallback );
					ButtonList[gMilitiaGroupBoxButton[buttoncnt]]->UserData[0] = buttoncnt;
					gMilitiaGroupBoxButtonCreated[buttoncnt] = TRUE;
				}
				++buttoncnt;
			}

			dispX += MILITIAGROUPBOX_WIDTH_TYPEBOX;

			swprintf( sString, L"%s", wSectorName );
			mprintf( dispX, dispY, sString );
			dispX += MILITIAGROUPBOX_WIDTH_SECTOR;

			swprintf( sString, L"%s", timestring );
			mprintf( dispX, dispY, sString );
			dispX += MILITIAGROUPBOX_WIDTH_ETA;

			swprintf( sString, L"%s", wFinalSectorName );
			mprintf( dispX, dispY, sString );
			dispX += MILITIAGROUPBOX_WIDTH_SECTOR;
			
			dispY += MILITIAGROUPBOX_HEIGHT_LINE;

			++groupcnt;
		}
		pGroup = pGroup->next;
	}
}
