#ifdef PRECOMPILEDHEADERS
	#include "Editor All.h"
#else
	#include "builddefines.h"
#endif

#ifdef JA2EDITOR


#ifndef PRECOMPILEDHEADERS
	#include <stdio.h>
	#include "types.h"
	#include "Sector Summary.h"
	#include "Timer Control.h"
	#include "vsurface.h"
	#include "Button System.h"
	#include "Font Control.h"
	#include "Simple Render Utils.h"
	#include "Editor Taskbar Utils.h"
	#include "line.h"
	#include "input.h"
	#include "vobject_blitters.h"
	#include "loadscreen.h"
	#include "Text Input.h"
	#include "mousesystem.h"
	#include "strategicmap.h"
	#include "Fileman.h"
	#include "Exit Grids.h"
	#include "Map Information.h"
	#include "Summary Info.h"
	#include "Animated ProgressBar.h"
	#include "worlddef.h"
	#include "worlddat.h"
	#include "EditorDefines.h"
	#include "editscreen.h"
	#include "english.h"
	#include "World Items.h"
	#include "text.h"
	#include "Soldier Create.h"
	#include "GameVersion.h"
	#include "Campaign Types.h"
	#include "GameSettings.h"
	#include "EditorTerrain.h"//dnl ch78 261113
	#include "Render Dirty.h"//dnl ch78 271113
#endif

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_file_raii.h>

extern BOOLEAN gfOverheadMapDirty;

#define MAP_SIZE			208
#define MAP_LEFT			(iScreenWidthOffset + 417)
#define MAP_TOP				(iScreenHeightOffset + 15)
#define MAP_RIGHT			(MAP_LEFT+MAP_SIZE)
#define MAP_BOTTOM			(MAP_TOP+MAP_SIZE)

enum{
	PRE_ALPHA,
	ALPHA,
	DEMO,
	BETA,
	RELEASE
};
CHAR16 gszVersionType[5][10] = { L"Pre-Alpha", L"Alpha", L"Demo", L"Beta", L"Release" };
#define GLOBAL_SUMMARY_STATE			RELEASE

//Regular masks
#define GROUND_LEVEL_MASK			0x01
#define BASEMENT1_LEVEL_MASK	0x02
#define BASEMENT2_LEVEL_MASK	0x04
#define BASEMENT3_LEVEL_MASK	0x08
#define ALL_LEVELS_MASK				0x0f
//Alternate masks
#define ALTERNATE_GROUND_MASK 0x10
#define ALTERNATE_B1_MASK			0x20
#define ALTERNATE_B2_MASK			0x40
#define ALTERNATE_B3_MASK			0x80
#define ALTERNATE_LEVELS_MASK	0xf0

void ExtractTempFilename();

INT32 giCurrLevel;

BOOLEAN gfOutdatedDenied;
UINT16 gusNumEntriesWithOutdatedOrNoSummaryInfo;

void UpdateMasterProgress();
BOOLEAN gfUpdatingNow;
UINT16 gusTotal, gusCurrent;

BOOLEAN gfMustForceUpdateAllMaps = FALSE;
UINT16 gusNumberOfMapsToBeForceUpdated = 0;
BOOLEAN gfMajorUpdate = FALSE;

BOOLEAN LoadSummary(STR8 pSector, UINT8 ubLevel, FLOAT dMajorMapVersion);//dnl ch28 240909

void RegenerateSummaryInfoForAllOutdatedMaps();

void SetupItemDetailsMode(BOOLEAN fAllowRecursion);//dnl ch81 051213

INT32 giCurrentViewLevel = ALL_LEVELS_MASK;

BOOLEAN gbSectorLevels[16][16];
BOOLEAN gfGlobalSummaryLoaded = FALSE;

SUMMARYFILE *gpSectorSummary[16][16][8] = {};
SUMMARYFILE *gpCurrentSectorSummary;
SUMMARYFILE gCustomFileSectorSummary;//dnl ch30 150909

MOUSE_REGION MapRegion;

extern INT8 gbMercSlotTypes[9];

extern void UpdateSummaryInfo( SUMMARYFILE *pSummary );

void SummaryOkayCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryToggleGridCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryToggleProgressCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryToggleLevelCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryLoadMapCallback( GUI_BUTTON *btn, INT32 reason );
void SummarySaveMapCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryOverrideCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryNewGroundLevelCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryNewBasementLevelCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryNewCaveLevelCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryUpdateCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryToggleAlternateCallback( GUI_BUTTON *btn, INT32 reason );
void SummarySciFiCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryRealCallback( GUI_BUTTON *btn, INT32 reason );
void SummaryEnemyCallback( GUI_BUTTON *btn, INT32 reason );
void MapMoveCallback( MOUSE_REGION *reg, INT32 reason );
void MapClickCallback( MOUSE_REGION *reg, INT32 reason );

//Set if there is an existing global summary.  The first time this is run on your computer, it
//will not exist, and will have to be generated before this will be set.
BOOLEAN gfGlobalSummaryExists;
//If you don't wish to create a global summary, you can deny it.  This safely locks the system
//from generating one.
BOOLEAN gfDeniedSummaryCreation;
//Set whenever the entire display is to be marked dirty.
BOOLEAN gfRenderSummary;
//Used externally to determine if the summary window is up or not.
BOOLEAN gfSummaryWindowActive;
//When set, the summary window stays up until told otherwise.  When clear, the summary will disappear
//when the assigned key (F5) is released.  The latter mode is initiated when F5 is held down for longer
//than .4 seconds, and is useful for quickly looking at the information in the current map being edited.
BOOLEAN gfPersistantSummary;
//When set, a grid is overlayed on top of the sector.  This grid defines each of the 256 sectors.  It is
//on by default.
BOOLEAN gfRenderGrid;
//When set, parts of the map are darkened, showing that those sectors don't exist in the currently selected
//layer.  When clear, the entire map is shown in full color.
BOOLEAN gfRenderProgress;
//When set, only the map section is rerendered.

extern BOOLEAN gfRenderMap; // symbol already declared globally in AI Viewer.cpp (jonathanl)
//Set whenever the ctrl key is held down.  This is used in conjunction with gfFileIO to determine whether the 
//selected sector is to be saved instead of loaded when clear.
BOOLEAN gfCtrlPressed;
//When set, it is time to load or save the selected sector.  If gfCtrlPressed is set, the the map is saved, 
//instead of loaded.  It is impossible to load a map that doesn't exist.
BOOLEAN gfFileIO;
//When set, then we are overriding the ability to use normal methods for selecting sectors for saving and 
//loading.  Immediately upon entering the text input mode; for the temp file; then we are assuming that
//the user will type in a name that doesn't follow standard naming conventions for the purposes of the 
//campaign editor.  This is useful for naming a temp file for saving or loading.
BOOLEAN gfTempFile;
//When set, only the alternate version of the maps will be displayed.  This is used for alternate maps in 
//particular sectors, such as the SkyRider quest which could be located at one of four maps randomly.  If
//that particular map is chosen, then the alternate map will be used.
BOOLEAN gfAlternateMaps = FALSE;


enum
{
	ITEMMODE_SCIFI,
	ITEMMODE_REAL,
	ITEMMODE_ENEMY,
};
UINT8 gubSummaryItemMode = ITEMMODE_SCIFI;

BOOLEAN gfItemDetailsMode = FALSE;

WORLDITEM *gpWorldItemsSummaryArray = NULL;
UINT16 gusWorldItemsSummaryArraySize = 0;
OBJECTTYPE *gpPEnemyItemsSummaryArray = NULL;
UINT16 gusPEnemyItemsSummaryArraySize = 0;
OBJECTTYPE *gpNEnemyItemsSummaryArray = NULL;
UINT16 gusNEnemyItemsSummaryArraySize = 0;

BOOLEAN gfSetupItemDetailsMode = TRUE;

BOOLEAN gfUpdateSummaryInfo;

UINT16 usNumSummaryFilesOutOfDate;

BOOLEAN gfMapFileDirty;

//Override status.  Hide is when there is nothing to override, readonly, when checked is to override a
//readonly status file, so that you can write to it, and overwrite, when checked, allows you to save,
//replacing the existing file.  These states are not persistant, which forces the user to check the 
//box before saving.
enum{ INACTIVE, READONLY, OVERWRITE };
UINT8 gubOverrideStatus;
//Set when the a new sector/level is selected, forcing the user to reselect the override status.
BOOLEAN gfOverrideDirty;
//The state of the override button, true if overriden intended.
BOOLEAN gfOverride;

//The sector coordinates of the map currently loaded in memory (blue)
INT16 gsSectorX, gsSectorY;
//The layer of the sector that is currently loaded in memory.
INT32 gsSectorLayer;
//The sector coordinates of the mouse position (yellow)

extern INT16 gsHiSectorX; // symbol already declared globally in AI Viewer.cpp (jonathanl)
extern INT16 gsHiSectorY; // symbol already declared globally in AI Viewer.cpp (jonathanl)
//The sector coordinates of the selected sector (red)

extern INT16 gsSelSectorX; // symbol already declared globally in AI Viewer.cpp (jonathanl)
extern INT16 gsSelSectorY; // symbol already declared globally in AI Viewer.cpp (jonathanl)
//Used to determine how long the F5 key has been held down for to determine whether or not the
//summary is going to be persistant or not.
UINT32 giInitTimer;

//dnl ch39 190909
CHAR16 gszFilename[FILENAME_BUFLEN];
CHAR16 gszTempFilename[FILENAME_BUFLEN];
CHAR16 gszDisplayName[FILENAME_BUFLEN];

void CalculateOverrideStatus();

enum{
	SUMMARY_BACKGROUND,
	SUMMARY_OKAY,
	SUMMARY_GRIDCHECKBOX,
	SUMMARY_PROGRESSCHECKBOX,
	SUMMARY_ALL,
	SUMMARY_G,
	SUMMARY_B1,
	SUMMARY_B2,
	SUMMARY_B3,
	SUMMARY_ALTERNATE,
	SUMMARY_LOAD,
	SUMMARY_SAVE,
	SUMMARY_OVERRIDE,
#if 0
	SUMMARY_NEW_GROUNDLEVEL,
	SUMMARY_NEW_BASEMENTLEVEL,
	SUMMARY_NEW_CAVELEVEL,
#endif
	SUMMARY_UPDATE,
	SUMMARY_SCIFI,
	SUMMARY_REAL,
	SUMMARY_ENEMY,
	NUM_SUMMARY_BUTTONS
};
INT32 iSummaryButton[ NUM_SUMMARY_BUTTONS ];

void CreateSummaryWindow()
{
	INT32 i;
	
	if( !gfGlobalSummaryLoaded )
	{
		LoadGlobalSummary();
		gfGlobalSummaryLoaded = TRUE;
	}
	
	if( gfSummaryWindowActive )
		return;

	DisableEditorTaskbar();
	DisableAllTextFields();
	//dnl ch78 261113
	HideTerrainTileButtons();
	DisableEditorRegion(ITEM_REGION_ID);
	RestoreBackgroundRects();

	GetCurrentWorldSector( &gsSectorX, &gsSectorY );
	gsSelSectorX = gsSectorX;
	gsSelSectorY = gsSectorY;
	gfSummaryWindowActive = TRUE;
	gfPersistantSummary = FALSE;
	giInitTimer = GetJA2Clock();
	gfDeniedSummaryCreation = FALSE;
	gfRenderSummary = TRUE;
	if( gfWorldLoaded )
		gfMapFileDirty = TRUE;
	//Create all of the buttons here
	iSummaryButton[ SUMMARY_BACKGROUND ] = 
		CreateTextButton( 0, 0, 0, 0, BUTTON_USE_DEFAULT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - 120, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1, 
		BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK );
	SpecifyDisabledButtonStyle( iSummaryButton[ SUMMARY_BACKGROUND ], DISABLED_STYLE_NONE );
	DisableButton( iSummaryButton[ SUMMARY_BACKGROUND ] );

	iSummaryButton[ SUMMARY_OKAY ] = 
		CreateTextButton(pCreateSummaryWindowText[0], FONT12POINT1, FONT_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT, MAP_LEFT+160, MAP_BOTTOM+99, 50, 30, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, SummaryOkayCallback);//dnl ch36 190909
	//GiveButtonDefaultStatus( iSummaryButton[ SUMMARY_OKAY ], DEFAULT_STATUS_WINDOWS95 );

	iSummaryButton[ SUMMARY_GRIDCHECKBOX ] = 
		CreateCheckBoxButton(	MAP_LEFT, ( INT16 ) ( MAP_BOTTOM + 5 ), "EDITOR//smcheckbox.sti", MSYS_PRIORITY_HIGH, SummaryToggleGridCallback );
	ButtonList[ iSummaryButton[ SUMMARY_GRIDCHECKBOX ] ]->uiFlags |= BUTTON_CLICKED_ON;
	gfRenderGrid = TRUE;

	iSummaryButton[ SUMMARY_PROGRESSCHECKBOX ] = 
		CreateCheckBoxButton(	( INT16 ) ( MAP_LEFT + 50 ), ( INT16 ) ( MAP_BOTTOM + 5 ), "EDITOR//smcheckbox.sti", MSYS_PRIORITY_HIGH, SummaryToggleProgressCallback );
	ButtonList[ iSummaryButton[ SUMMARY_PROGRESSCHECKBOX ] ]->uiFlags |= BUTTON_CLICKED_ON;
	gfRenderProgress = TRUE;
	
	iSummaryButton[ SUMMARY_ALL ] = 
		CreateTextButton( pCreateSummaryWindowText[1], SMALLCOMPFONT, FONT_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT,
		MAP_LEFT+110, MAP_BOTTOM+5, 16, 16, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK,
		SummaryToggleLevelCallback );
	if( giCurrentViewLevel == ALL_LEVELS_MASK || giCurrentViewLevel == ALTERNATE_LEVELS_MASK )
		ButtonList[ iSummaryButton[ SUMMARY_ALL ] ]->uiFlags |= BUTTON_CLICKED_ON;
	iSummaryButton[ SUMMARY_G ] = 
		CreateTextButton( pCreateSummaryWindowText[2], SMALLCOMPFONT, FONT_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT,
		MAP_LEFT+128, MAP_BOTTOM+5, 16, 16, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK,
		SummaryToggleLevelCallback );
	if( giCurrentViewLevel == GROUND_LEVEL_MASK || giCurrentViewLevel == ALTERNATE_GROUND_MASK )
		ButtonList[ iSummaryButton[ SUMMARY_G ] ]->uiFlags |= BUTTON_CLICKED_ON;
	iSummaryButton[ SUMMARY_B1 ] = 
		CreateTextButton( pCreateSummaryWindowText[3], SMALLCOMPFONT, FONT_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT,
		MAP_LEFT+146, MAP_BOTTOM+5, 16, 16, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK,
		SummaryToggleLevelCallback );
	if( giCurrentViewLevel == BASEMENT1_LEVEL_MASK || giCurrentViewLevel == ALTERNATE_B1_MASK )
		ButtonList[ iSummaryButton[ SUMMARY_B1 ] ]->uiFlags |= BUTTON_CLICKED_ON;
	iSummaryButton[ SUMMARY_B2 ] = 
		CreateTextButton( pCreateSummaryWindowText[4], SMALLCOMPFONT, FONT_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT,
		MAP_LEFT+164, MAP_BOTTOM+5, 16, 16, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK,
		SummaryToggleLevelCallback );
	if( giCurrentViewLevel == BASEMENT2_LEVEL_MASK || giCurrentViewLevel == ALTERNATE_B2_MASK )
		ButtonList[ iSummaryButton[ SUMMARY_B2 ] ]->uiFlags |= BUTTON_CLICKED_ON;
	iSummaryButton[ SUMMARY_B3 ] = 
		CreateTextButton( pCreateSummaryWindowText[5], SMALLCOMPFONT, FONT_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT,
		MAP_LEFT+182, MAP_BOTTOM+5, 16, 16, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK,
		SummaryToggleLevelCallback );
	if( giCurrentViewLevel == BASEMENT3_LEVEL_MASK || giCurrentViewLevel == ALTERNATE_B3_MASK )
		ButtonList[ iSummaryButton[ SUMMARY_B3 ] ]->uiFlags |= BUTTON_CLICKED_ON;

	iSummaryButton[ SUMMARY_ALTERNATE ] = 
		CreateCheckBoxButton(	MAP_LEFT, ( INT16 ) ( MAP_BOTTOM + 25 ), "EDITOR//smcheckbox.sti", MSYS_PRIORITY_HIGH, SummaryToggleAlternateCallback );
	if( gfAlternateMaps )
		ButtonList[ iSummaryButton[ SUMMARY_ALTERNATE ] ]->uiFlags |= BUTTON_CLICKED_ON;

	iSummaryButton[ SUMMARY_LOAD ] = 
		CreateTextButton( pCreateSummaryWindowText[6], FONT12POINT1, FONT_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT,
		MAP_LEFT, MAP_BOTTOM+45, 50, 26, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK,
		SummaryLoadMapCallback );
	iSummaryButton[ SUMMARY_SAVE ] = 
		CreateTextButton( pCreateSummaryWindowText[7], FONT12POINT1, FONT_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT,
		MAP_LEFT+55, MAP_BOTTOM+45, 50, 26, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK,
		SummarySaveMapCallback );
	iSummaryButton[ SUMMARY_OVERRIDE ] = 
		CreateCheckBoxButton( ( INT16 ) ( MAP_LEFT + 110 ), ( INT16 ) ( MAP_BOTTOM + 59 ), "EDITOR\\smcheckbox.sti", MSYS_PRIORITY_HIGH, SummaryOverrideCallback );

	
#if 0
	iSummaryButton[ SUMMARY_NEW_GROUNDLEVEL ] = 
		CreateSimpleButton( MAP_LEFT, MAP_BOTTOM+58, "EDITOR\\new.sti", BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, SummaryNewGroundLevelCallback );
	SetButtonFastHelpText( iSummaryButton[ SUMMARY_NEW_GROUNDLEVEL ], L"New map" );
	iSummaryButton[ SUMMARY_NEW_BASEMENTLEVEL ] = 
		CreateSimpleButton( MAP_LEFT+32, MAP_BOTTOM+58, "EDITOR\\new.sti", BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, SummaryNewBasementLevelCallback );
	SetButtonFastHelpText( iSummaryButton[ SUMMARY_NEW_BASEMENTLEVEL ], L"New basement" );
	iSummaryButton[ SUMMARY_NEW_CAVELEVEL ] = 
		CreateSimpleButton( MAP_LEFT+64, MAP_BOTTOM+58, "EDITOR\\new.sti", BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, SummaryNewCaveLevelCallback );
	SetButtonFastHelpText( iSummaryButton[ SUMMARY_NEW_CAVELEVEL ], L"New cave level" );
#endif
	

	iSummaryButton[ SUMMARY_UPDATE ] = 
		CreateTextButton( pCreateSummaryWindowText[8], FONT12POINT1, FONT_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 255, iScreenHeightOffset + 15, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK,
		SummaryUpdateCallback );

	iSummaryButton[ SUMMARY_REAL ] = 
		CreateCheckBoxButton( iScreenWidthOffset + 350, iScreenHeightOffset + 47, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_HIGH, SummaryRealCallback );
	iSummaryButton[ SUMMARY_SCIFI ] = 
		CreateCheckBoxButton( iScreenWidthOffset + 376, iScreenHeightOffset + 47, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_HIGH, SummarySciFiCallback );
	iSummaryButton[ SUMMARY_ENEMY ] = 
		CreateCheckBoxButton( iScreenWidthOffset + 350, iScreenHeightOffset + 60, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_HIGH, SummaryEnemyCallback );

	//SetButtonFastHelpText( iSummaryButton[ SUMMARY_SCIFI ], L"Display items that appear in SciFi mode." );
	//SetButtonFastHelpText( iSummaryButton[ SUMMARY_REAL ], L"Display items that appear in Realistic mode." );
	switch( gubSummaryItemMode )
	{
		case ITEMMODE_SCIFI:
			ButtonList[ iSummaryButton[ SUMMARY_SCIFI ] ]->uiFlags |= BUTTON_CLICKED_ON;
			break;
		case ITEMMODE_REAL:
			ButtonList[ iSummaryButton[ SUMMARY_REAL ] ]->uiFlags |= BUTTON_CLICKED_ON;
			break;
		case ITEMMODE_ENEMY:
			ButtonList[ iSummaryButton[ SUMMARY_ENEMY ] ]->uiFlags |= BUTTON_CLICKED_ON;
			break;
	}

	//Init the textinput field.
	InitTextInputModeWithScheme( DEFAULT_SCHEME );
	AddUserInputField( NULL );  //just so we can use short cut keys while not typing.
	AddTextInputField(MAP_LEFT+110, MAP_BOTTOM+75, 100, 18, MSYS_PRIORITY_HIGH, L"", (FILENAME_BUFLEN-4-1), INPUTTYPE_EXCLUSIVE_DOSFILENAME);//dnl ch39 190909
	for( i = 1; i < NUM_SUMMARY_BUTTONS; i++ )
		HideButton( iSummaryButton[ i ] );

	MSYS_DefineRegion( &MapRegion, MAP_LEFT, MAP_TOP, MAP_RIGHT, MAP_BOTTOM, MSYS_PRIORITY_HIGH, 0,
		MapMoveCallback, MapClickCallback );
	MSYS_DisableRegion( &MapRegion );

	//if( gfItemDetailsMode )
//	{
		gfItemDetailsMode = FALSE;
	//	gfSetupItemDetailsMode = TRUE;
	//}
	if( !gfWorldLoaded )
	{
		gfConfirmExitFirst = FALSE;
		ReleaseSummaryWindow();
		DisableButton( iSummaryButton[ SUMMARY_OKAY ] );
		DisableButton( iSummaryButton[ SUMMARY_SAVE ] );
	}
	if( gfAutoLoadA9 )
	{
		gfAutoLoadA9++;
		gsSelSectorX = gGameExternalOptions.ubDefaultArrivalSectorX;//9;
		gsSelSectorY = gGameExternalOptions.ubDefaultArrivalSectorY;//1;
		//gpCurrentSectorSummary = gpSectorSummary[ 8 ][ 0 ][ 0 ];
		gpCurrentSectorSummary = gpSectorSummary[ gGameExternalOptions.ubDefaultArrivalSectorX - 1 ][ gGameExternalOptions.ubDefaultArrivalSectorY - 1 ][ 0 ];
		ButtonList[ iSummaryButton[ SUMMARY_LOAD ] ]->uiFlags |= BUTTON_CLICKED_ON;
	}
}

void AutoLoadMap()
{
	SummaryLoadMapCallback( ButtonList[ iSummaryButton[ SUMMARY_LOAD ] ], MSYS_CALLBACK_REASON_LBUTTON_UP );
	ButtonList[ iSummaryButton[ SUMMARY_LOAD ] ]->uiFlags &= ~BUTTON_CLICKED_ON;//dnl ch36 210909
	if( gfWorldLoaded )
		DestroySummaryWindow();
	gfAutoLoadA9 = FALSE;
	gfConfirmExitFirst = TRUE;
}

void ReleaseSummaryWindow()
{
	INT32 i;
	UINT32 uiCurrTimer;
	if( !gfSummaryWindowActive || gfPersistantSummary )
		return;
	uiCurrTimer = GetJA2Clock();
	if( !gfWorldLoaded || uiCurrTimer - giInitTimer < 400 )
	{ //make window persistant
		for( i = 1; i < NUM_SUMMARY_BUTTONS; i++ )
			ShowButton( iSummaryButton[ i ] );
		HideButton( iSummaryButton[ SUMMARY_UPDATE ] );
		HideButton( iSummaryButton[ SUMMARY_OVERRIDE ] );
		HideButton( iSummaryButton[ SUMMARY_REAL ] );
		HideButton( iSummaryButton[ SUMMARY_SCIFI ] );
		HideButton( iSummaryButton[ SUMMARY_ENEMY ] );
		MSYS_EnableRegion( &MapRegion );
		gfPersistantSummary = TRUE;
		gfOverrideDirty = TRUE;
		gfRenderSummary = TRUE;
	}
	else
	{
		DestroySummaryWindow();
	}
}

void DestroySummaryWindow()
{
	INT32 i;
	if( !gfSummaryWindowActive )
		return;
	for( i = 0; i < NUM_SUMMARY_BUTTONS; i++ )
	{
		RemoveButton( iSummaryButton[ i ] );
	}

	MSYS_RemoveRegion( &MapRegion );
	gfRenderTaskbar = TRUE;//dnl ch52 091009
	gfSummaryWindowActive = FALSE;
	gfPersistantSummary = FALSE;
	MarkWorldDirty();
	KillTextInputMode();
	EnableEditorTaskbar();
	EnableAllTextFields();
	//dnl ch78 261113
	if(iCurrentTaskbar == TASK_TERRAIN)
		ShowTerrainTileButtons();
	else if(iCurrentTaskbar == TASK_ITEMS)
		EnableEditorRegion(ITEM_REGION_ID);

	if( gpWorldItemsSummaryArray )
	{
		delete[]( gpWorldItemsSummaryArray );
		gpWorldItemsSummaryArray = NULL;
		gusWorldItemsSummaryArraySize = 0;
	}
	if( gpPEnemyItemsSummaryArray )
	{
		delete[]( gpPEnemyItemsSummaryArray );
		gpPEnemyItemsSummaryArray = NULL;
		gusPEnemyItemsSummaryArraySize = 0;
	}
	if( gpNEnemyItemsSummaryArray )
	{
		delete[]( gpNEnemyItemsSummaryArray );
		gpNEnemyItemsSummaryArray = NULL;
		gusNEnemyItemsSummaryArraySize = 0;
	}
	if( gfWorldLoaded )
	{
		gfConfirmExitFirst = TRUE;
	}
}

void RenderSectorInformation()
{
	//CHAR16 str[ 100 ];
	MAPCREATE_STRUCT *m;
	SUMMARYFILE *s;
	UINT8 ePoints = 0;
	UINT16 usLine = 35;
	INT32 iOverall;
	
	SetFont( FONT10ARIAL );
	SetFontShadow( FONT_NEARBLACK );

	s = gpCurrentSectorSummary;
	m = &gpCurrentSectorSummary->MapInfo;

	if( m->sNorthGridNo != -1 ) 
		ePoints++;
	if( m->sEastGridNo != -1 ) 
		ePoints++;
	if( m->sSouthGridNo != -1 ) 
		ePoints++;
	if( m->sWestGridNo != -1 ) 
		ePoints++;
	if( m->sCenterGridNo != -1 ) 
		ePoints++;
	if( m->sIsolatedGridNo != -1 ) 
		ePoints++;
	//start at 10,35
	SetFontForeground( FONT_ORANGE );
	mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 32,		pRenderSectorInformationText[0], gTilesets[ s->ubTilesetID ].zName );
	if( m->ubMapVersion < 10 )
		SetFontForeground( FONT_RED );
	mprintf(iScreenWidthOffset+10, iScreenHeightOffset+42, pRenderSectorInformationText[1], s->ubSummaryVersion, s->dMajorMapVersion, m->ubMapVersion);//dnl ch30 240909
	SetFontForeground( FONT_GRAY2 );
	mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 55,		pRenderSectorInformationText[2], s->usNumItems );
	mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 65,		pRenderSectorInformationText[3], s->usNumLights );
	mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 75,		pRenderSectorInformationText[4], ePoints );
	if( ePoints )
	{
		INT32 x;
		x = iScreenWidthOffset + 140;
		mprintf( x, iScreenHeightOffset + 75, L"(" );
		x += StringPixLength( L"(", FONT10ARIAL ) + 2;
		if( m->sNorthGridNo			!= -1	)	{	mprintf( x, iScreenHeightOffset + 75, pRenderSectorInformationText[5] );	x += StringPixLength( pRenderSectorInformationText[5], FONT10ARIAL ) + 2; }
		if( m->sEastGridNo			!= -1	)	{	mprintf( x, iScreenHeightOffset + 75, pRenderSectorInformationText[6] );	x += StringPixLength( pRenderSectorInformationText[6], FONT10ARIAL ) + 2; }
		if( m->sSouthGridNo			!= -1	)	{	mprintf( x, iScreenHeightOffset + 75, pRenderSectorInformationText[7] );	x += StringPixLength( pRenderSectorInformationText[7], FONT10ARIAL ) + 2; }
		if( m->sWestGridNo			!= -1	)	{	mprintf( x, iScreenHeightOffset + 75, pRenderSectorInformationText[8] );	x += StringPixLength( pRenderSectorInformationText[8], FONT10ARIAL ) + 2; }
		if( m->sCenterGridNo		!= -1	)	{	mprintf( x, iScreenHeightOffset + 75, pRenderSectorInformationText[9] );	x += StringPixLength( pRenderSectorInformationText[9], FONT10ARIAL ) + 2; }
		if( m->sIsolatedGridNo		!= -1	)	{	mprintf( x, iScreenHeightOffset + 75, pRenderSectorInformationText[10] );	x += StringPixLength( pRenderSectorInformationText[10], FONT10ARIAL ) + 2; }
		mprintf( x, iScreenHeightOffset + 75, L")" );
	}
	mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 85,		pRenderSectorInformationText[11], s->ubNumRooms );
	mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 95,		pRenderSectorInformationText[12], m->ubNumIndividuals );
	mprintf( iScreenWidthOffset + 20, iScreenHeightOffset + 105,	pRenderSectorInformationText[13], s->EnemyTeam.ubTotal );
	mprintf( iScreenWidthOffset + 30, iScreenHeightOffset + 115,	pRenderSectorInformationText[14], s->ubNumAdmins );
	if( s->ubNumAdmins )
		mprintf( iScreenWidthOffset + 100, iScreenHeightOffset + 115,		pRenderSectorInformationText[15], s->ubAdminDetailed, s->ubAdminProfile, s->ubAdminExistance );
	mprintf( iScreenWidthOffset + 30, iScreenHeightOffset + 125,			pRenderSectorInformationText[16], s->ubNumTroops );
	if( s->ubNumTroops )
		mprintf( iScreenWidthOffset + 100, iScreenHeightOffset + 125,		pRenderSectorInformationText[17], s->ubTroopDetailed, s->ubTroopProfile, s->ubTroopExistance );
	mprintf( iScreenWidthOffset + 30, iScreenHeightOffset + 135,			pRenderSectorInformationText[18], s->ubNumElites );
	if( s->ubNumElites )
		mprintf( iScreenWidthOffset + 100, iScreenHeightOffset + 135,		pRenderSectorInformationText[19], s->ubEliteDetailed, s->ubEliteProfile, s->ubEliteExistance );
	mprintf( iScreenWidthOffset + 20, iScreenHeightOffset + 145,			pRenderSectorInformationText[20], s->CivTeam.ubTotal );
	if( s->CivTeam.ubTotal )
		mprintf( iScreenWidthOffset + 100, iScreenHeightOffset + 145,		pRenderSectorInformationText[21], s->CivTeam.ubDetailed, s->CivTeam.ubProfile, s->CivTeam.ubExistance ); 
	if( s->ubSummaryVersion >= 9 )
	{
		mprintf( iScreenWidthOffset + 30, iScreenHeightOffset + 155,		pRenderSectorInformationText[22], s->CivTeam.ubTotal - s->ubCivCows - s->ubCivBloodcats );
		mprintf( iScreenWidthOffset + 30, iScreenHeightOffset + 165,		pRenderSectorInformationText[23], s->ubCivCows );
		mprintf( iScreenWidthOffset + 30, iScreenHeightOffset + 175,		pRenderSectorInformationText[24], s->ubCivBloodcats );
	}
	mprintf( iScreenWidthOffset + 20, iScreenHeightOffset + 185,			pRenderSectorInformationText[25], s->CreatureTeam.ubTotal );
	if( s->ubSummaryVersion >= 9 )
	{
		mprintf( iScreenWidthOffset + 30, iScreenHeightOffset + 195,     	pRenderSectorInformationText[26], s->CreatureTeam.ubTotal - s->CreatureTeam.ubNumAnimals );
		mprintf( iScreenWidthOffset + 30, iScreenHeightOffset + 205,     	pRenderSectorInformationText[27], s->CreatureTeam.ubNumAnimals );
	}
	mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 215,			pRenderSectorInformationText[28], s->ubNumDoors );
	mprintf( iScreenWidthOffset + 20, iScreenHeightOffset + 225,			pRenderSectorInformationText[29], s->ubNumDoorsLocked );
	mprintf( iScreenWidthOffset + 20, iScreenHeightOffset + 235,			pRenderSectorInformationText[30], s->ubNumDoorsTrapped );
	mprintf( iScreenWidthOffset + 20, iScreenHeightOffset + 245,			pRenderSectorInformationText[31], s->ubNumDoorsLockedAndTrapped );
	if( s->ubSummaryVersion >= 8 )
		mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 255,		pRenderSectorInformationText[32], s->ubCivSchedules );
	if( s->ubSummaryVersion >= 10 )
	{
		if( s->fTooManyExitGridDests )
		{
			SetFontForeground( FONT_RED );
			mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 265, pRenderSectorInformationText[33]);
		}
		else
		{
			UINT8 i;
			UINT8 ubNumInvalid = 0;
			for( i = 0; i < 4; i++ )
			{
				if( s->fInvalidDest[i] )
					ubNumInvalid++;
			}
			if( ubNumInvalid )
			{
				SetFontForeground( FONT_RED );
				mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 265, pRenderSectorInformationText[34], s->ubNumExitGridDests, ubNumInvalid );
			}
			else switch( s->ubNumExitGridDests )
			{
				case 0:
					mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 265, pRenderSectorInformationText[35] );
					break;
				case 1:
					mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 265, pRenderSectorInformationText[36], s->usExitGridSize[0] );
					break;
				case 2:
					mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 265, pRenderSectorInformationText[37], s->usExitGridSize[0], s->usExitGridSize[1] );
					break;
				case 3:
					mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 265, pRenderSectorInformationText[38], 
						s->usExitGridSize[0], s->usExitGridSize[1], s->usExitGridSize[2] );
					break;
				case 4:
					mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 265, pRenderSectorInformationText[39], 
						s->usExitGridSize[0], s->usExitGridSize[1], s->usExitGridSize[2], s->usExitGridSize[3] );
					break;
			}
		}
	}
	iOverall = - ( 2 * s->EnemyTeam.ubBadA ) - s->EnemyTeam.ubPoorA + s->EnemyTeam.ubGoodA + ( 2 * s->EnemyTeam.ubGreatA );
	usLine = iScreenHeightOffset + 275;
	mprintf( iScreenWidthOffset + 10, usLine, pRenderSectorInformationText[40],
		s->EnemyTeam.ubBadA, 
		s->EnemyTeam.ubPoorA, 
		s->EnemyTeam.ubAvgA, 
		s->EnemyTeam.ubGoodA, 
		s->EnemyTeam.ubGreatA,
		iOverall );
	iOverall = - ( 2 * s->EnemyTeam.ubBadE ) - s->EnemyTeam.ubPoorE + s->EnemyTeam.ubGoodE + ( 2 * s->EnemyTeam.ubGreatE );
	usLine += 10;
	mprintf( iScreenWidthOffset + 10, usLine, pRenderSectorInformationText[41],
		s->EnemyTeam.ubBadE, 
		s->EnemyTeam.ubPoorE, 
		s->EnemyTeam.ubAvgE, 
		s->EnemyTeam.ubGoodE, 
		s->EnemyTeam.ubGreatE,
		iOverall );
	usLine += 10;
	if( s->ubSummaryVersion >= 11 )
	{
		if( s->ubEnemiesReqWaypoints )
		{
			SetFontForeground( FONT_RED );
			mprintf( iScreenWidthOffset + 10, usLine, pRenderSectorInformationText[42], s->ubEnemiesReqWaypoints );
			usLine += 10;
		}
	}
	if( s->ubSummaryVersion >= 13 )
	{
		if( s->ubEnemiesHaveWaypoints )
		{
			SetFontForeground( FONT_RED );
			mprintf( iScreenWidthOffset + 10, usLine, pRenderSectorInformationText[43], s->ubEnemiesHaveWaypoints );
			usLine += 10;
		}
	}
	if( s->ubSummaryVersion >= 12 )
	{
		if( s->usWarningRoomNums )
		{
			SetFontForeground( FONT_RED );
			mprintf( iScreenWidthOffset + 10, usLine, pRenderSectorInformationText[44], s->usWarningRoomNums );
		}
	}
}

//2)  CODE TRIGGER/ACTION NAMES
void RenderItemDetails()
{
	FLOAT dAvgExistChance, dAvgStatus;
	OBJECTTYPE *pItem;
	INT32 index, i;
	CHAR16 str[100];
	UINT32 uiQuantity, uiExistChance, uiStatus;
	UINT32 uiTriggerQuantity[8], uiActionQuantity[8], uiTriggerExistChance[8], uiActionExistChance[8];
	UINT32 xp, yp;
	INT8 bFreqIndex;
	SetFont( FONT10ARIAL );
	SetFontForeground( FONT_GRAY2 );
	SetFontShadow( FONT_NEARBLACK );
	mprintf( iScreenWidthOffset + 364, iScreenHeightOffset + 49, pRenderItemDetailsText[0] );
	mprintf( iScreenWidthOffset + 390, iScreenHeightOffset + 49, pRenderItemDetailsText[1] );
	mprintf( iScreenWidthOffset + 364, iScreenHeightOffset + 62, pRenderItemDetailsText[2] );

	xp = iScreenWidthOffset + 5;
	yp = iScreenHeightOffset + 20;

	if( gubSummaryItemMode != ITEMMODE_ENEMY && gpWorldItemsSummaryArray )
	{
		memset( uiTriggerQuantity, 0, 32 );
		memset( uiActionQuantity, 0, 32 );
		memset( uiTriggerExistChance, 0, 32 );
		memset( uiActionExistChance, 0, 32 );
		for( index = 1; index < MAXITEMS; index++ )
		{
			if ( Item[index].usItemClass  == 0 )
				break;
			uiQuantity = 0;
			uiExistChance = 0;
			uiStatus = 0;
			for( i = 0; i < gusWorldItemsSummaryArraySize; i++ )
			{
				if( index == SWITCH || index == ACTION_ITEM )
				{
					if( gpWorldItemsSummaryArray[ i ].object.usItem == index )
					{
						if( gubSummaryItemMode == ITEMMODE_SCIFI && !(gpWorldItemsSummaryArray[ i ].usFlags & WORLD_ITEM_REALISTIC_ONLY) || 
							gubSummaryItemMode == ITEMMODE_REAL && !(gpWorldItemsSummaryArray[ i ].usFlags & WORLD_ITEM_SCIFI_ONLY) )
						{
							pItem = &gpWorldItemsSummaryArray[ i ].object;
							if( !(*pItem)[0]->data.misc.bFrequency )
								bFreqIndex = 7;
							else if( (*pItem)[0]->data.misc.bFrequency == PANIC_FREQUENCY )
								bFreqIndex = 0;
							else if( (*pItem)[0]->data.misc.bFrequency == PANIC_FREQUENCY_2 )
								bFreqIndex = 1;
							else if( (*pItem)[0]->data.misc.bFrequency == PANIC_FREQUENCY_3 )
								bFreqIndex = 2;
							else if( (*pItem)[0]->data.misc.bFrequency == FIRST_MAP_PLACED_FREQUENCY + 1 )
								bFreqIndex = 3;
							else if( (*pItem)[0]->data.misc.bFrequency == FIRST_MAP_PLACED_FREQUENCY + 2 )
								bFreqIndex = 4;
							else if( (*pItem)[0]->data.misc.bFrequency == FIRST_MAP_PLACED_FREQUENCY + 3 )
								bFreqIndex = 5;
							else if( (*pItem)[0]->data.misc.bFrequency == FIRST_MAP_PLACED_FREQUENCY + 4 )
								bFreqIndex = 6;
							else
								continue;
							if( index == SWITCH )
							{
								uiTriggerQuantity[ bFreqIndex ]++;
								uiTriggerExistChance[ bFreqIndex ] += 100 - gpWorldItemsSummaryArray[ i ].ubNonExistChance;
							}
							else
							{
								uiActionQuantity[ bFreqIndex ]++;
								uiActionExistChance[ bFreqIndex ] += 100 - gpWorldItemsSummaryArray[ i ].ubNonExistChance;
							}
						}
					}
					continue;
				}
				if( gpWorldItemsSummaryArray[ i ].object.usItem == index )
				{
					if( gubSummaryItemMode == ITEMMODE_SCIFI && !(gpWorldItemsSummaryArray[ i ].usFlags & WORLD_ITEM_REALISTIC_ONLY) || 
						gubSummaryItemMode == ITEMMODE_REAL	&& !(gpWorldItemsSummaryArray[ i ].usFlags & WORLD_ITEM_SCIFI_ONLY) )
					{
						pItem = &gpWorldItemsSummaryArray[ i ].object;
						uiExistChance += (100 - gpWorldItemsSummaryArray[ i ].ubNonExistChance) * pItem->ubNumberOfObjects;
						uiStatus += (*pItem)[0]->data.objectStatus;
						uiQuantity += pItem->ubNumberOfObjects;
					}
				}
			}
			if( uiQuantity )
			{
				if( !(yp % 20) )
					SetFontForeground( FONT_LTKHAKI );
				else
					SetFontForeground( FONT_GRAY2 );
				//calc averages
				dAvgExistChance = (FLOAT)(uiExistChance / 100.0);
				dAvgStatus = uiStatus / (FLOAT)uiQuantity;
				//Display stats.
				LoadShortNameItemInfo( (UINT16)index, str );
				mprintf( xp, yp, L"%s", str );
				mprintf( xp + 85, yp, L"%3.02f", dAvgExistChance ); 
				mprintf( xp + 110, yp, L"@ %3.02f%%", dAvgStatus );
				yp += 10;
				if( yp >= (UINT32)(iScreenHeightOffset + 355 ))
				{
					xp += 170;
					yp = iScreenHeightOffset + 20;
					if( xp >= (UINT32)(iScreenWidthOffset + 300 ))
					{
						SetFontForeground( FONT_RED );
						mprintf( iScreenWidthOffset + 350, iScreenHeightOffset + 350, pRenderItemDetailsText[3]);
						return;
					}
				}
			}
		}
		//Now list the number of actions/triggers of each type
		for( i = 0; i < 8; i++ )
		{
			if( uiTriggerQuantity[i] || uiActionQuantity[i] )
			{
				if( i == 7 )
					SetFontForeground( FONT_DKYELLOW );
				else if( !uiTriggerQuantity[i] || !uiActionQuantity[i] )
					SetFontForeground( FONT_RED );
				else
					SetFontForeground( 77 );
				switch( i )
				{
					case 0: swprintf( str, pRenderItemDetailsText[4] );		break;
					case 1:	swprintf( str, pRenderItemDetailsText[5] );		break;
					case 2:	swprintf( str, pRenderItemDetailsText[6] );		break;
					case 3:	swprintf( str, pRenderItemDetailsText[7] );		break;
					case 4:	swprintf( str, pRenderItemDetailsText[8] );		break;
					case 5:	swprintf( str, pRenderItemDetailsText[9] );		break;
					case 6:	swprintf( str, pRenderItemDetailsText[10] );		break;
					case 7:	swprintf( str, pRenderItemDetailsText[11] );		break;
				}
				if( i < 7 )
				{
					dAvgExistChance = (FLOAT)(uiTriggerExistChance[i] / 100.0);
					dAvgStatus = (FLOAT)(uiActionExistChance[i] / 100.0);
					mprintf( xp, yp, L"%s:  %3.02f trigger(s), %3.02f action(s)", str, dAvgExistChance, dAvgStatus );
				}
				else 
				{
					dAvgExistChance = (FLOAT)(uiActionExistChance[i] / 100.0);
					mprintf( xp, yp, L"%s:  %3.02f", str, dAvgExistChance );
				}
				yp += 10;
				if( yp >= (UINT32)(iScreenHeightOffset + 355 ))
				{
					xp += 170;
					yp = iScreenHeightOffset + 20;
					if( xp >= (UINT32)(iScreenWidthOffset + 300 ))
					{
						SetFontForeground( FONT_RED );
						mprintf( iScreenWidthOffset + 350, iScreenHeightOffset + 350, pRenderItemDetailsText[12]);
						return;
					}
				}
			}
		}
	}
	else if( gubSummaryItemMode == ITEMMODE_ENEMY )
	{
		
		SetFontForeground( FONT_YELLOW );
		mprintf( xp, yp, pRenderItemDetailsText[13] );
		yp += 10;

		//Do the priority existance guys first
		if( !gpPEnemyItemsSummaryArray )
		{
			SetFontForeground( FONT_DKYELLOW );
			mprintf( xp, yp, pRenderItemDetailsText[14] );
			yp += 10;
		}
		else for( index = 1; index < MAXITEMS; index++ )
		{
			if ( Item[index].usItemClass  == 0 )
				break;
			uiQuantity = 0;
			uiExistChance = 0;
			uiStatus = 0;
			for( i = 0; i < gusPEnemyItemsSummaryArraySize; i++ )
			{
				if( gpPEnemyItemsSummaryArray[ i ].usItem == index )
				{
					pItem = &gpPEnemyItemsSummaryArray[ i ];
					uiExistChance += 100 * pItem->ubNumberOfObjects;
					uiStatus += (*pItem)[0]->data.objectStatus;
					uiQuantity += pItem->ubNumberOfObjects;
				}
			}
			if( uiQuantity )
			{
				if( !(yp % 20) )
					SetFontForeground( FONT_LTKHAKI );
				else
					SetFontForeground( FONT_GRAY2 );
				//calc averages
				dAvgExistChance = (FLOAT)(uiExistChance / 100.0);
				dAvgStatus = uiStatus / (FLOAT)uiQuantity;
				//Display stats.
				LoadShortNameItemInfo( (UINT16)index, str );
				mprintf( xp, yp, L"%s", str );
				mprintf( xp + 85, yp, L"%3.02f", dAvgExistChance ); 
				mprintf( xp + 110, yp, L"@ %3.02f%%", dAvgStatus );
				yp += 10;
				if( yp >= (UINT32)(iScreenHeightOffset + 355 ))
				{
					xp += 170;

					yp = (iScreenHeightOffset + 20);

					if( xp >= (UINT32)(iScreenWidthOffset + 300 ))
					{
						SetFontForeground( FONT_RED );
						mprintf( iScreenWidthOffset + 350, iScreenHeightOffset + 350, pRenderItemDetailsText[15] );
						return;
					}
				}
			}
		}

		yp += 5;
	
		SetFontForeground( FONT_YELLOW );
		mprintf( xp, yp, pRenderItemDetailsText[16] );
		yp += 10;
		if( yp >= (UINT32)(iScreenHeightOffset + 355 ))
		{
			xp += 170;
			yp = (iScreenHeightOffset + 20);
			if( xp >= (UINT32)(iScreenWidthOffset + 300 ))
			{
				SetFontForeground( FONT_RED );
				mprintf( iScreenWidthOffset + 350, iScreenHeightOffset + 350, pRenderItemDetailsText[17] );
				return;
			}
		}

		//Do the priority existance guys first
		if( !gpNEnemyItemsSummaryArray )
		{
			SetFontForeground( FONT_DKYELLOW );
			mprintf( xp, yp, pRenderItemDetailsText[18] );
			yp += 10;
		}
		for( index = 1; index < MAXITEMS; index++ )
		{
			if ( Item[index].usItemClass  == 0 )
				break;
			uiQuantity = 0;
			uiExistChance = 0;
			uiStatus = 0;
			for( i = 0; i < gusNEnemyItemsSummaryArraySize; i++ )
			{
				if( gpNEnemyItemsSummaryArray[ i ].usItem == index )
				{
					pItem = &gpNEnemyItemsSummaryArray[ i ];
					uiExistChance += 100 * pItem->ubNumberOfObjects;
					uiStatus += (*pItem)[0]->data.objectStatus;
					uiQuantity += pItem->ubNumberOfObjects;
				}
			}
			if( uiQuantity )
			{
				if( !(yp % 20) )
					SetFontForeground( FONT_LTKHAKI );
				else
					SetFontForeground( FONT_GRAY2 );
				//calc averages
				dAvgExistChance = (FLOAT)(uiExistChance / 100.0);
				dAvgStatus = uiStatus / (FLOAT)uiQuantity;
				//Display stats.
				LoadShortNameItemInfo( (UINT16)index, str );
				mprintf( xp, yp, L"%s", str );
				mprintf( xp + 85, yp, L"%3.02f", dAvgExistChance ); 
				mprintf( xp + 110, yp, L"@ %3.02f%%", dAvgStatus );
				yp += 10;
				if( yp >= (UINT32)(iScreenHeightOffset + 355 ))
				{
					xp += 170;
					yp = iScreenHeightOffset + 20;
					if( xp >= (UINT32)(iScreenWidthOffset + 300 ))
					{
						SetFontForeground( FONT_RED );
						mprintf( iScreenWidthOffset + 350, iScreenHeightOffset + 350, pRenderItemDetailsText[19] );
						return;
					}
				}
			}
		}
	
	
	
	}
	else
	{
		SetFontForeground( FONT_RED );
		mprintf( iScreenWidthOffset + 5, iScreenHeightOffset + 50, pRenderItemDetailsText[20] );
	}
}

void RenderSummaryWindow()
{
	UINT8 *pDestBuf;
	UINT32 uiDestPitchBYTES;
	SGPRect ClipRect;
	INT32 i, x, y;
	if( (GetActiveFieldID() == 1 ) != gfTempFile )
	{
		gfTempFile ^= 1;
		SetInputFieldStringWith16BitString( 1, L"" );
		gfRenderSummary = TRUE;
	}
	if( gfTempFile ) //constantly extract the temp filename for updating purposes.
		ExtractTempFilename();
	if( gfRenderSummary )
	{
		gfRenderSummary = FALSE;
		gfRenderMap = TRUE;
		for( i = 1; i < NUM_SUMMARY_BUTTONS; i++ )
		{
			MarkAButtonDirty( iSummaryButton[ i ] );
		}

		DrawButton( iSummaryButton[ SUMMARY_BACKGROUND ] );
		InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - 120 );
		
		SetFont( BLOCKFONT2 );
		SetFontForeground( FONT_LTKHAKI );
		SetFontShadow( FONT_DKKHAKI );
		SetFontBackground( 0 );
		if( !gfItemDetailsMode )
		{
			mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 5, pRenderSummaryWindowText[0], 
				gszVersionType[ GLOBAL_SUMMARY_STATE ], GLOBAL_SUMMARY_VERSION );
		}
		
		//This section builds the proper header to be displayed for an existing global summary.
		if( !gfWorldLoaded )
		{
			SetFontForeground( FONT_RED );
			SetFontShadow( FONT_NEARBLACK );
			mprintf( iScreenWidthOffset + 270, iScreenHeightOffset + 5, pRenderSummaryWindowText[1]);
		}
		SetFont( FONT10ARIAL );
		SetFontShadow( FONT_NEARBLACK );
		if( gfGlobalSummaryExists )
		{
			CHAR16 str[100];
			BOOLEAN fSectorSummaryExists = FALSE;
			if( gusNumEntriesWithOutdatedOrNoSummaryInfo && !gfOutdatedDenied )
			{
				DisableButton( iSummaryButton[ SUMMARY_LOAD ] );
				SetFontForeground( FONT_YELLOW );
				mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 20, pRenderSummaryWindowText[2], gusNumEntriesWithOutdatedOrNoSummaryInfo);
				mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 30, pRenderSummaryWindowText[3]);
				mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 40, pRenderSummaryWindowText[4]);
				mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 50, pRenderSummaryWindowText[5]);
				SetFontForeground( FONT_LTRED );
				mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 65, pRenderSummaryWindowText[6] );
			}
			else if( !gsSelSectorX && !gsSectorX || gfTempFile )
			{
				DisableButton( iSummaryButton[ SUMMARY_LOAD ] );
				SetFontForeground( FONT_LTRED );
				if( !gfItemDetailsMode )//dnl ch81 031213
					mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 20, pRenderSummaryWindowText[7] );
				if( gfTempFile )
				{
					SetFontForeground( FONT_YELLOW );
					mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 30, pRenderSummaryWindowText[8] );
					goto SPECIALCASE_LABEL;  //OUCH!!!
				}
				else if( !gfWorldLoaded )
				{
					SetFontForeground( FONT_YELLOW );
					mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 30, pRenderSummaryWindowText[9] );
					mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 40, pRenderSummaryWindowText[10] );
				}
				else if(gCustomFileSectorSummary.ubSummaryVersion)//dnl ch30 150909 //dnl ch81 031213
				{
					gpCurrentSectorSummary = &gCustomFileSectorSummary;
					wcscpy(gszFilename, gzFilename);
					wcscpy(gszDisplayName, gszFilename);
					if(gfItemDetailsMode)
					{
						SetupItemDetailsMode(TRUE);
						SetFontForeground(FONT_YELLOW);
						mprintf(iScreenWidthOffset+10, iScreenHeightOffset+5, pRenderSummaryWindowText[19], gszDisplayName);
						RenderItemDetails();
					}
					else
					{
						HideButton(iSummaryButton[SUMMARY_REAL]);
						HideButton(iSummaryButton[SUMMARY_SCIFI]);
						HideButton(iSummaryButton[SUMMARY_ENEMY]);
						RenderSectorInformation();
					}
				}
			}
			else
			{
				//Build sector string
				if( gsSelSectorX )
					x = gsSelSectorX - 1, y = gsSelSectorY - 1;
				else
					x = gsSectorX - 1, y = gsSectorY - 1;
				swprintf( str, L"%c%d", y + 'A', x + 1 );
				swprintf( gszFilename, str );
				giCurrLevel = giCurrentViewLevel;
				switch( giCurrentViewLevel )
				{
					case ALL_LEVELS_MASK:
						//search for the highest level
						fSectorSummaryExists = TRUE;
						if( gbSectorLevels[x][y] & GROUND_LEVEL_MASK )
							giCurrLevel = GROUND_LEVEL_MASK;
						else if( gbSectorLevels[x][y] & BASEMENT1_LEVEL_MASK )
							giCurrLevel = BASEMENT1_LEVEL_MASK;
						else if( gbSectorLevels[x][y] & BASEMENT2_LEVEL_MASK )
							giCurrLevel = BASEMENT2_LEVEL_MASK;
						else if( gbSectorLevels[x][y] & BASEMENT3_LEVEL_MASK )
							giCurrLevel = BASEMENT3_LEVEL_MASK;
						else
							fSectorSummaryExists = FALSE;
						break;
					case ALTERNATE_LEVELS_MASK:
						//search for the highest alternate level
						fSectorSummaryExists = TRUE;
						if( gbSectorLevels[x][y] & ALTERNATE_GROUND_MASK )
							giCurrLevel = ALTERNATE_GROUND_MASK;
						else if( gbSectorLevels[x][y] & ALTERNATE_B1_MASK )
							giCurrLevel = ALTERNATE_B1_MASK;
						else if( gbSectorLevels[x][y] & ALTERNATE_B2_MASK )
							giCurrLevel = ALTERNATE_B2_MASK;
						else if( gbSectorLevels[x][y] & ALTERNATE_B3_MASK )
							giCurrLevel = ALTERNATE_B3_MASK;
						else
							fSectorSummaryExists = FALSE;
						break;
					case GROUND_LEVEL_MASK:
						if( gbSectorLevels[x][y] & GROUND_LEVEL_MASK )
							fSectorSummaryExists = TRUE;
						break;
					case BASEMENT1_LEVEL_MASK:
						if( gbSectorLevels[x][y] & BASEMENT1_LEVEL_MASK )
							fSectorSummaryExists = TRUE;
						break;
					case BASEMENT2_LEVEL_MASK:
						if( gbSectorLevels[x][y] & BASEMENT2_LEVEL_MASK )
							fSectorSummaryExists = TRUE;
						break;
					case BASEMENT3_LEVEL_MASK:
						if( gbSectorLevels[x][y] & BASEMENT3_LEVEL_MASK )
							fSectorSummaryExists = TRUE;
						break;
					case ALTERNATE_GROUND_MASK:
						if( gbSectorLevels[x][y] & ALTERNATE_GROUND_MASK )
							fSectorSummaryExists = TRUE;
						break;
					case ALTERNATE_B1_MASK:
						if( gbSectorLevels[x][y] & ALTERNATE_B1_MASK )
							fSectorSummaryExists = TRUE;
						break;
					case ALTERNATE_B2_MASK:
						if( gbSectorLevels[x][y] & ALTERNATE_B2_MASK )
							fSectorSummaryExists = TRUE;
						break;
					case ALTERNATE_B3_MASK:
						if( gbSectorLevels[x][y] & ALTERNATE_B3_MASK )
							fSectorSummaryExists = TRUE;
						break;
				}
				if( gbSectorLevels[x][y] )
				{
					switch( giCurrLevel )
					{
						case GROUND_LEVEL_MASK:			
							giCurrLevel = 0;
							wcscat( str, pRenderSummaryWindowText[11] );					
							gpCurrentSectorSummary = gpSectorSummary[x][y][0];
							break;
						case BASEMENT1_LEVEL_MASK:	
							giCurrLevel = 1;
							wcscat( str, pRenderSummaryWindowText[12] );	
							gpCurrentSectorSummary = gpSectorSummary[x][y][1];
							break;
						case BASEMENT2_LEVEL_MASK:	
							giCurrLevel = 2;
							wcscat( str, pRenderSummaryWindowText[13] );	
							gpCurrentSectorSummary = gpSectorSummary[x][y][2];
							break;
						case BASEMENT3_LEVEL_MASK:	
							giCurrLevel = 3;
							wcscat( str, pRenderSummaryWindowText[14] );	
							gpCurrentSectorSummary = gpSectorSummary[x][y][3];
							break;
						case ALTERNATE_GROUND_MASK:			
							giCurrLevel = 4;
							wcscat( str, pRenderSummaryWindowText[15] );					
							gpCurrentSectorSummary = gpSectorSummary[x][y][4];
							break;
						case ALTERNATE_B1_MASK:	
							giCurrLevel = 5;
							wcscat( str, pRenderSummaryWindowText[16] );	
							gpCurrentSectorSummary = gpSectorSummary[x][y][5];
							break;
						case ALTERNATE_B2_MASK:	
							giCurrLevel = 6;
							wcscat( str, pRenderSummaryWindowText[17] );	
							gpCurrentSectorSummary = gpSectorSummary[x][y][6];
							break;
						case ALTERNATE_B3_MASK:	
							giCurrLevel = 7;
							wcscat( str, pRenderSummaryWindowText[18] );	
							gpCurrentSectorSummary = gpSectorSummary[x][y][7];
							break;
					}
				}
				else
				{
					DisableButton( iSummaryButton[ SUMMARY_LOAD ] );
				}
				if( fSectorSummaryExists )
				{
					switch( giCurrLevel )
					{
						case 0:	wcscat( gszFilename, L".dat" );				break;
						case 1:	wcscat( gszFilename, L"_b1.dat" );		break;
						case 2:	wcscat( gszFilename, L"_b2.dat" );		break;
						case 3:	wcscat( gszFilename, L"_b3.dat" );		break;
						case 4:	wcscat( gszFilename, L"_a.dat" );			break;
						case 5:	wcscat( gszFilename, L"_b1_a.dat" );	break;
						case 6:	wcscat( gszFilename, L"_b2_a.dat" );	break;
						case 7:	wcscat( gszFilename, L"_b3_a.dat" );	break;
					}
					swprintf( gszDisplayName, gszFilename );
					EnableButton( iSummaryButton[ SUMMARY_LOAD ] );
					if( gpCurrentSectorSummary )
					{
						if( gpCurrentSectorSummary->ubSummaryVersion < GLOBAL_SUMMARY_VERSION )
							ShowButton( iSummaryButton[ SUMMARY_UPDATE ] );
						else
							HideButton( iSummaryButton[ SUMMARY_UPDATE ] );
						if( !gfAlternateMaps )
							SetFontForeground( FONT_YELLOW );
						else
							SetFontForeground( FONT_LTBLUE );
						if( gfItemDetailsMode )
						{
							if( gfSetupItemDetailsMode )
							{
								SetupItemDetailsMode( TRUE );
								gfSetupItemDetailsMode = FALSE;
							}
							mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 5, pRenderSummaryWindowText[19], str );
							RenderItemDetails();
						}
						else
						{
							mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 20, pRenderSummaryWindowText[20], str );
							HideButton( iSummaryButton[ SUMMARY_REAL ] );
							HideButton( iSummaryButton[ SUMMARY_SCIFI ] );
							HideButton( iSummaryButton[ SUMMARY_ENEMY ] );
							RenderSectorInformation();
						}
					}
					else
					{
						SetFontForeground( FONT_RED );
						if( gfItemDetailsMode )
						{
							mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 5, pRenderSummaryWindowText[21] , str );
							mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 15, pRenderSummaryWindowText[22] );
						}
						else
						{
							mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 20, pRenderSummaryWindowText[23] , str );
							mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 30, pRenderSummaryWindowText[24] );
						}
						ShowButton( iSummaryButton[ SUMMARY_UPDATE ] );
						//dnl ch81 041213
						HideButton(iSummaryButton[SUMMARY_REAL]);
						HideButton(iSummaryButton[SUMMARY_SCIFI]);
						HideButton(iSummaryButton[SUMMARY_ENEMY]);
					}
				}
				else
				{
					if( !gfAlternateMaps )
						SetFontForeground( FONT_ORANGE );
					else
					{
						SetFontForeground( FONT_DKBLUE );
						SetFontShadow( 0 );
					}
					if( gfItemDetailsMode )
						mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 5, pRenderSummaryWindowText[25], str );
					else
						mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 20, pRenderSummaryWindowText[26], str );
					SetFontShadow( FONT_NEARBLACK );

					switch( giCurrentViewLevel )
					{
						case ALL_LEVELS_MASK:
						case GROUND_LEVEL_MASK:			wcscat( gszFilename, L".dat" );				break;
						case BASEMENT1_LEVEL_MASK:	wcscat( gszFilename, L"_b1.dat" );		break;
						case BASEMENT2_LEVEL_MASK:	wcscat( gszFilename, L"_b2.dat" );		break;
						case BASEMENT3_LEVEL_MASK:	wcscat( gszFilename, L"_b3.dat" );		break;
						case ALTERNATE_LEVELS_MASK:
						case ALTERNATE_GROUND_MASK:	wcscat( gszFilename, L"_a.dat" );			break;
						case ALTERNATE_B1_MASK:			wcscat( gszFilename, L"_b1_a.dat" );	break;
						case ALTERNATE_B2_MASK:			wcscat( gszFilename, L"_b2_a.dat" );	break;
						case ALTERNATE_B3_MASK:			wcscat( gszFilename, L"_b3_a.dat" );	break;
					}
					swprintf( gszDisplayName, gszFilename );
					DisableButton( iSummaryButton[ SUMMARY_LOAD ] );
					//dnl ch81 031213
					HideButton(iSummaryButton[SUMMARY_UPDATE]);
					HideButton(iSummaryButton[SUMMARY_REAL]);
					HideButton(iSummaryButton[SUMMARY_SCIFI]);
					HideButton(iSummaryButton[SUMMARY_ENEMY]);
				}
				SPECIALCASE_LABEL:
				if( gfOverrideDirty && gfPersistantSummary )
					CalculateOverrideStatus();
				if( gubOverrideStatus == INACTIVE )
				{
					if( !gfAlternateMaps )
						SetFontForeground( FONT_LTKHAKI );
					else
						SetFontForeground( FONT_LTBLUE );
					mprintf( MAP_LEFT+110, MAP_BOTTOM+55, pRenderSummaryWindowText[27], gszDisplayName );
				}
				else //little higher to make room for the override checkbox and text.
				{
					if( !gfAlternateMaps )
						SetFontForeground( FONT_LTKHAKI );
					else
						SetFontForeground( FONT_LTBLUE );
					mprintf( MAP_LEFT+110, MAP_BOTTOM+46, pRenderSummaryWindowText[28], gszDisplayName );
					if( gubOverrideStatus == READONLY )
					{
						SetFontForeground( (UINT8)(gfOverride ? FONT_YELLOW : FONT_LTRED) );
						mprintf( MAP_LEFT+124, MAP_BOTTOM+61, pRenderSummaryWindowText[29] );
					}
					else
					{
						SetFontForeground( (UINT8)(gfOverride ? FONT_YELLOW: FONT_ORANGE ) );
						mprintf( MAP_LEFT+124, MAP_BOTTOM+61, pRenderSummaryWindowText[30]);
					}
				}
			}
		}
		else if( !gfDeniedSummaryCreation )
		{
			SetFontForeground( FONT_GRAY1 );
			mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 20, pRenderSummaryWindowText[31] );
			mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 30, pRenderSummaryWindowText[32] );
			mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 40, pRenderSummaryWindowText[33] );
			mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 50, pRenderSummaryWindowText[34] );
			mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 60, pRenderSummaryWindowText[35] );  
			mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 70, pRenderSummaryWindowText[36] );
			SetFontForeground( FONT_LTRED );
			mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 85, pRenderSummaryWindowText[37] );
		}
		else
		{
			SetFontForeground( FONT_LTRED );
			mprintf( iScreenWidthOffset + 10, iScreenHeightOffset + 20, pRenderSummaryWindowText[38] );
		}

		SetFont( FONT10ARIAL );
		SetFontForeground( FONT_GRAY3 );
		mprintf( MAP_LEFT + 15, MAP_BOTTOM + 7, pRenderSummaryWindowText[39] );
		mprintf( MAP_LEFT + 65, MAP_BOTTOM + 7,pRenderSummaryWindowText[40] );
		mprintf( MAP_LEFT + 15, MAP_BOTTOM + 27, pRenderSummaryWindowText[41] );
		//Draw the mode tabs
		SetFontForeground( FONT_YELLOW );
		mprintf( iScreenWidthOffset + 354, iScreenHeightOffset + 18, pRenderSummaryWindowText[42] );
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
		RectangleDraw( TRUE, iScreenWidthOffset + 350, iScreenHeightOffset + 15, iScreenWidthOffset + 405, iScreenHeightOffset + 28, 0, pDestBuf );
		UnLockVideoSurface( FRAME_BUFFER );
		ShadowVideoSurfaceRectUsingLowPercentTable( FRAME_BUFFER, iScreenWidthOffset + 351, iScreenHeightOffset + 16, iScreenWidthOffset + 404, iScreenHeightOffset + 27 );
		if( gpCurrentSectorSummary ) 
			/*&& gpCurrentSectorSummary->usNumItems || 
				gpPEnemyItemsSummaryArray && gusPEnemyItemsSummaryArraySize ||
				gpNEnemyItemsSummaryArray && gusNEnemyItemsSummaryArraySize )*/
		{
			SetFontForeground( FONT_YELLOW );
		}
		else
		{
			SetFontForeground( FONT_RED );
		}
		mprintf( iScreenWidthOffset + 354, iScreenHeightOffset + 33, pRenderSummaryWindowText[43] );
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
		RectangleDraw( TRUE, iScreenWidthOffset + 350, iScreenHeightOffset + 30, iScreenWidthOffset + 405, iScreenHeightOffset + 43, 0, pDestBuf );
		UnLockVideoSurface( FRAME_BUFFER );
		if( gpCurrentSectorSummary )
			/*&& gpCurrentSectorSummary->usNumItems || 
				gpPEnemyItemsSummaryArray && gusPEnemyItemsSummaryArraySize ||
				gpNEnemyItemsSummaryArray && gusNEnemyItemsSummaryArraySize )
				*/
		{
			ShadowVideoSurfaceRectUsingLowPercentTable( FRAME_BUFFER, iScreenWidthOffset + 351, iScreenHeightOffset + 31, iScreenWidthOffset + 404, iScreenHeightOffset + 42 );
		}
		else
		{
			ShadowVideoSurfaceRect( FRAME_BUFFER, iScreenWidthOffset + 351, iScreenHeightOffset + 31, iScreenWidthOffset + 404, iScreenHeightOffset + 42 );
		}
		SetFontForeground( FONT_GRAY2 );
	}

	RenderButtons();

	if( gfRenderMap )
	{
		gfRenderMap = FALSE;
		BltVideoObjectFromIndex( FRAME_BUFFER, guiOmertaMap, 0, MAP_LEFT-2, MAP_TOP-2, VO_BLT_SRCTRANSPARENCY, NULL );
		InvalidateRegion( MAP_LEFT-1, MAP_TOP-1, MAP_RIGHT+1, MAP_BOTTOM+1 );
		//Draw the coordinates
		SetFont( SMALLCOMPFONT );
		SetFontForeground( FONT_BLACK );
		for( y = 0; y < 16; y++ )
		{
			mprintf( MAP_LEFT-8, MAP_TOP+4+y*13, L"%c", 65 + y );
		}
		for( x = 1; x <= 16; x++ )
		{
			CHAR16 str[3];
			swprintf( str, L"%d", x );
			mprintf( MAP_LEFT+x*13-(13+StringPixLength( str, SMALLCOMPFONT ))/2, MAP_TOP-8, str );
		}
		if( gfRenderGrid )
		{
			UINT16 pos;
			pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
			SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
			for( i = 1; i <= 15; i++ )
			{
				//draw vertical lines
				pos = (UINT16)(i * 13 + MAP_LEFT);
				LineDraw( TRUE, pos, MAP_TOP, pos, MAP_BOTTOM-1, 0, pDestBuf );
				//draw horizontal lines
				pos = (UINT16)(i * 13 + MAP_TOP);
				LineDraw( TRUE, MAP_LEFT, pos, MAP_RIGHT-1, pos, 0, pDestBuf );
			}
			UnLockVideoSurface( FRAME_BUFFER );
		}
		if( gfRenderProgress )
		{
			UINT8 ubNumUndergroundLevels;
			CHAR16 str[2];
			for( y = 0; y < 16; y++ ) 
			{
				ClipRect.iTop = MAP_TOP + y*13;
				ClipRect.iBottom = ClipRect.iTop + 12;
				for( x = 0; x < 16; x++ )
				{
					if( giCurrentViewLevel == ALL_LEVELS_MASK )
					{
						ubNumUndergroundLevels = 0;
						if( gbSectorLevels[x][y] & BASEMENT1_LEVEL_MASK )		ubNumUndergroundLevels++;
						if( gbSectorLevels[x][y] & BASEMENT2_LEVEL_MASK )		ubNumUndergroundLevels++;
						if( gbSectorLevels[x][y] & BASEMENT3_LEVEL_MASK )		ubNumUndergroundLevels++;
						if( ubNumUndergroundLevels )
						{ //display the number of underground levels.  If there
							//is no ground level, then it'll be shadowed.
							SetFont( SMALLCOMPFONT );
							SetFontForeground( FONT_YELLOW );
							swprintf( str, L"%d", ubNumUndergroundLevels );
							mprintf( MAP_LEFT + x*13 + 4, ClipRect.iTop + 4, str );
						}
						if( gbSectorLevels[x][y] & GROUND_LEVEL_MASK )
						{
							if( !gfItemDetailsMode || !gpSectorSummary[x][y][0] || gpSectorSummary[x][y][0]->usNumItems )
								continue;
						}
					}
					else if( giCurrentViewLevel == ALTERNATE_LEVELS_MASK )
					{
						ubNumUndergroundLevels = 0;
						if( gbSectorLevels[x][y] & ALTERNATE_B1_MASK )		ubNumUndergroundLevels++;
						if( gbSectorLevels[x][y] & ALTERNATE_B2_MASK )		ubNumUndergroundLevels++;
						if( gbSectorLevels[x][y] & ALTERNATE_B3_MASK )		ubNumUndergroundLevels++;
						if( ubNumUndergroundLevels )
						{ //display the number of underground levels.  If there
							//is no ground level, then it'll be shadowed.
							SetFont( SMALLCOMPFONT );
							SetFontForeground( FONT_YELLOW );
							swprintf( str, L"%d", ubNumUndergroundLevels );
							mprintf( MAP_LEFT + x*13 + 4, ClipRect.iTop + 4, str );
						}
						if( gbSectorLevels[x][y] & ALTERNATE_GROUND_MASK )
						{
							if( !gfItemDetailsMode || !gpSectorSummary[x][y][4] || gpSectorSummary[x][y][4]->usNumItems )
								continue;
						}
					}
					else if( gbSectorLevels[x][y] & giCurrentViewLevel )
					{
						if( !gfItemDetailsMode || !gpSectorSummary[x][y][giCurrLevel] || gpSectorSummary[x][y][giCurrLevel]->usNumItems )
							continue;
					}
					ClipRect.iLeft = MAP_LEFT + x*13;
					ClipRect.iRight = ClipRect.iLeft + 12;
					pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
					Blt16BPPBufferShadowRect( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect );
					if( giCurrentViewLevel == BASEMENT1_LEVEL_MASK ||
						  giCurrentViewLevel == BASEMENT2_LEVEL_MASK ||
							giCurrentViewLevel == BASEMENT3_LEVEL_MASK || 
							giCurrentViewLevel == ALTERNATE_B1_MASK		 ||
							giCurrentViewLevel == ALTERNATE_B2_MASK		 ||
							giCurrentViewLevel == ALTERNATE_B3_MASK		 )
						Blt16BPPBufferShadowRect( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect );
					UnLockVideoSurface( FRAME_BUFFER );
				}
			}
		}
	}

	if( gfGlobalSummaryExists )
	{
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
		//Render the grid for the map currently residing in memory (blue).
		if( gfWorldLoaded && !gfTempFile && gsSectorX )
		{
			x = MAP_LEFT + (gsSectorX-1) * 13 + 1;
			y = MAP_TOP + (gsSectorY-1) * 13 + 1;
			RectangleDraw( TRUE, x, y, x+11, y+11, Get16BPPColor( FROMRGB( 50, 50, 200 ) ), pDestBuf );
		}

		//Render the grid for the sector currently in focus (red).
		if( gsSelSectorX > 0 && !gfTempFile )
		{
			x = MAP_LEFT + (gsSelSectorX-1) * 13 ;
			y = MAP_TOP + (gsSelSectorY-1) * 13 ;
			RectangleDraw( TRUE, x, y, x+13, y+13, Get16BPPColor( FROMRGB( 200, 50, 50 ) ), pDestBuf );
		}

		//Render the grid for the sector if the mouse is over it (yellow).
		if( gsHiSectorX > 0 )
		{
			x = MAP_LEFT + (gsHiSectorX-1) * 13 - 1;
			y = MAP_TOP + (gsHiSectorY-1) * 13 - 1;
			RectangleDraw( TRUE, x, y, x+15, y+15, Get16BPPColor( FROMRGB( 200, 200, 50 ) ), pDestBuf );
		}
		UnLockVideoSurface( FRAME_BUFFER );
	}
	//Check to see if the user clicked on one of the hot spot mode change areas.
	if( gfLeftButtonState )
	{
		if( !gfItemDetailsMode )
		{
			if( gpCurrentSectorSummary )
				/*&& gpCurrentSectorSummary->usNumItems || 
					gpPEnemyItemsSummaryArray && gusPEnemyItemsSummaryArraySize ||
					gpNEnemyItemsSummaryArray && gusNEnemyItemsSummaryArraySize )*/
			{
				if( gusMouseXPos >= (iScreenWidthOffset + 350) && gusMouseYPos >= (iScreenHeightOffset + 30) && gusMouseXPos <= (iScreenWidthOffset + 404) && gusMouseYPos <= (iScreenHeightOffset + 42) )
				{
					gfItemDetailsMode = TRUE;
					gfSetupItemDetailsMode = TRUE;
					gfRenderSummary = TRUE;
				}
			}
		}
		else if( gfItemDetailsMode && gusMouseXPos >= (iScreenWidthOffset + 350) && gusMouseYPos >= (iScreenHeightOffset + 15) && gusMouseXPos <= (iScreenWidthOffset + 404) && gusMouseYPos <= (iScreenHeightOffset + 27) )
		{
			gfItemDetailsMode = FALSE;
			gfRenderSummary = TRUE;
		}
	}
}

//dnl ch30 150909
void ResetCustomFileSectorSummary(void)
{
	memset(&gCustomFileSectorSummary, 0, sizeof(SUMMARYFILE));
}

//dnl ch31 150909
void GetSectorFromFileName(STR16 szFileName, INT16& sSectorX, INT16& sSectorY, INT8& bSectorZ, BOOLEAN& fAltMap)
{
	const int strsz = 16;
	CHAR16 str[strsz] = L"";
	if(szFileName[2] == L'.' || szFileName[2] == L'_')
	{
		str[0] = szFileName[0];
		str[1] = L'0';
		wmemcpy(str+2, szFileName+1, strsz-2);
	}
	else if(szFileName[1] == L'1')
		wmemcpy(str, szFileName, strsz);
	for(int i=0; i<strsz; i++)
		str[i] = towupper(str[i]);
	if((str[0] >= L'A'  && str[0] <= L'P') && (str[1] >= L'0' && str[1] <= L'9') && (str[2] >= L'0' && str[2] <= L'9'))
	{
		sSectorX = (str[1] - L'0') * 10 + str[2] - L'0';
		sSectorY = str[0] - L'A' + 1;
		bSectorZ = 0;
		fAltMap = FALSE;
		if(!(sSectorX < 1 || sSectorX > 16 || sSectorY < 1 || sSectorY > 16))
		{
			if(wcscmp(&str[3], L".DAT") == 0)
				return;
			else if(wcscmp(&str[3], L"_A.DAT") == 0)
			{
				fAltMap = TRUE;// Alternate Ground Map
				return;
			}
			else if(str[3] == L'_' && str[4] == L'B' && str[5] >= L'1' && str[5] <= L'3')
			{
				bSectorZ = str[5] - L'0';
				if(wcscmp(&str[6], L".DAT") == 0)
					return;
				else if(wcscmp(&str[6], L"_A.DAT") == 0)
				{
					fAltMap = TRUE;// Alternate Underground Map
					return;
				}
			}
		}
	}
	sSectorX = 0;
	sSectorY = 0;
	bSectorZ = 0;
	fAltMap = FALSE;
}

void UpdateSectorSummary( STR16 gszFilename, BOOLEAN fUpdate )
{
	CHAR16 str[50];
	CHAR8 szCoord[40];
	//dnl ch31 130909
	INT8 bSectorZ;
	BOOLEAN fAltMap;
	GetSectorFromFileName(gszFilename, gsSelSectorX, gsSelSectorY, bSectorZ, fAltMap);
	gfRenderSummary = TRUE;
	gfMapFileDirty = FALSE;
	gsSectorX = gsSelSectorX;
	gsSectorY = gsSelSectorY;
	if(!gsSectorX)
	{
		CHAR8 ubNewFilename[50];
		sprintf(ubNewFilename, "%S", gszFilename);
		ReEvaluateWorld(ubNewFilename);//dnl ch30 150909
		return;
	}
	switch(bSectorZ)
	{
	case 0:
		if(fAltMap)
		{
			gsSectorLayer = ALTERNATE_GROUND_MASK;
			giCurrLevel = 4;
		}
		else
		{
			gsSectorLayer = GROUND_LEVEL_MASK;
			giCurrLevel = 0;
		}
		break;
	case 1:
		if(fAltMap)
		{
			gsSectorLayer = ALTERNATE_B1_MASK;
			giCurrLevel = 5;
		}
		else
		{
			gsSectorLayer = BASEMENT1_LEVEL_MASK;
			giCurrLevel = 1;
		}
		break;
	case 2:
		if(fAltMap)
		{
			gsSectorLayer = ALTERNATE_B2_MASK;
			giCurrLevel = 6;
		}
		else
		{
			gsSectorLayer = BASEMENT2_LEVEL_MASK;
			giCurrLevel = 2;
		}
		break;
	case 3:
		if(fAltMap)
		{
			gsSectorLayer = ALTERNATE_B3_MASK;
			giCurrLevel = 7;
		}
		else
		{
			gsSectorLayer = BASEMENT3_LEVEL_MASK;
			giCurrLevel = 3;
		}
		break;
	}
	giCurrentViewLevel = gsSectorLayer;
	if( !(gbSectorLevels[gsSectorX-1][gsSectorY-1] & gsSectorLayer) )
	{
		//new sector map saved, so update the global file.
		gbSectorLevels[gsSectorX-1][gsSectorY-1] |= gsSectorLayer;
	}

	if( fUpdate )
	{
		SetFont( FONT10ARIAL );
		SetFontForeground( FONT_LTKHAKI );
		SetFontShadow( FONT_NEARBLACK );
		swprintf( str, pUpdateSectorSummaryText[0], gszFilename );

		if( gfSummaryWindowActive )
		{
			mprintf( MAP_LEFT, MAP_BOTTOM+100, str );
			InvalidateRegion( MAP_LEFT, MAP_BOTTOM+100, MAP_LEFT+150,	MAP_BOTTOM+110 );
			CreateProgressBar( 0, MAP_LEFT, MAP_BOTTOM+110, MAP_LEFT+140, MAP_BOTTOM+120 );
		}
		else
		{
			mprintf( (iScreenWidthOffset + 320) - StringPixLength( str, FONT10ARIAL )/2, iScreenHeightOffset + 190, str );
			InvalidateRegion( iScreenWidthOffset + 200, iScreenHeightOffset + 190, iScreenWidthOffset + 400, iScreenHeightOffset + 200 );
			CreateProgressBar( 0, iScreenWidthOffset + 250, iScreenHeightOffset + 200, iScreenWidthOffset + 390, iScreenHeightOffset + 210 );
		}

		sprintf( szCoord, "%S", gszFilename );
		if( gsSectorX > 9 )
			szCoord[3] = '\0';
		else
			szCoord[2] = '\0';
		gusNumEntriesWithOutdatedOrNoSummaryInfo++;
		EvaluateWorld( szCoord, (UINT8)giCurrLevel );

		RemoveProgressBar( 0 );
	}
	else
		gusNumEntriesWithOutdatedOrNoSummaryInfo++;
}

void SummaryOkayCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		DestroySummaryWindow();
	}
}

void SummaryToggleGridCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gfRenderGrid = (BOOLEAN)(btn->uiFlags & BUTTON_CLICKED_ON);
		gfRenderMap = TRUE;
	}
}

void SummaryToggleAlternateCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			giCurrentViewLevel <<= 4;
			gfAlternateMaps = TRUE;
		}
		else
		{
			giCurrentViewLevel >>= 4;
			gfAlternateMaps = FALSE;
		}
		gfRenderSummary = TRUE;
		if(gfItemDetailsMode)//dnl ch81 041213
			gfSetupItemDetailsMode = TRUE;
	}
}

void SummarySciFiCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ButtonList[ iSummaryButton[ SUMMARY_SCIFI ] ]->uiFlags |= (BUTTON_CLICKED_ON | BUTTON_DIRTY);
		ButtonList[ iSummaryButton[ SUMMARY_REAL ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ iSummaryButton[ SUMMARY_REAL ] ]->uiFlags |= BUTTON_DIRTY;
		ButtonList[ iSummaryButton[ SUMMARY_ENEMY ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ iSummaryButton[ SUMMARY_ENEMY ] ]->uiFlags |= BUTTON_DIRTY;
		gubSummaryItemMode = ITEMMODE_SCIFI;
		gfRenderSummary = TRUE;
	}
}

void SummaryRealCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ButtonList[ iSummaryButton[ SUMMARY_SCIFI ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ iSummaryButton[ SUMMARY_SCIFI ] ]->uiFlags |= BUTTON_DIRTY;
		ButtonList[ iSummaryButton[ SUMMARY_REAL ] ]->uiFlags |= (BUTTON_CLICKED_ON | BUTTON_DIRTY);
		ButtonList[ iSummaryButton[ SUMMARY_ENEMY ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ iSummaryButton[ SUMMARY_ENEMY ] ]->uiFlags |= BUTTON_DIRTY;
		gubSummaryItemMode = ITEMMODE_REAL;
		gfRenderSummary = TRUE;
	}
}

void SummaryEnemyCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ButtonList[ iSummaryButton[ SUMMARY_SCIFI ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ iSummaryButton[ SUMMARY_SCIFI ] ]->uiFlags |= BUTTON_DIRTY;
		ButtonList[ iSummaryButton[ SUMMARY_REAL ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ iSummaryButton[ SUMMARY_REAL ] ]->uiFlags |= BUTTON_DIRTY;
		ButtonList[ iSummaryButton[ SUMMARY_ENEMY ] ]->uiFlags |= (BUTTON_CLICKED_ON | BUTTON_DIRTY);
		gubSummaryItemMode = ITEMMODE_ENEMY;
		gfRenderSummary = TRUE;
	}
}

void SummaryToggleProgressCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gfRenderProgress = (BOOLEAN)(btn->uiFlags & BUTTON_CLICKED_ON);
		gfRenderMap = TRUE;
	}
}

#include "Tile Surface.h"

void PerformTest()
{
#if 0
	OutputDebugString( "PERFORMING A NEW TEST -------------------------------------------------\n" );
	memset( gbDefaultSurfaceUsed, 0, sizeof( gbDefaultSurfaceUsed ) );
	giCurrentTilesetID = -1;
	switch( Random( 3 ) )
	{
		case 0:
			LoadWorld( "J9.dat" );
			break;
		case 1:
			LoadWorld( "J9_b1.dat" );
			break;
		case 2:
			LoadWorld( "J9_b2.dat" );
			break;
	}
#endif
}


BOOLEAN HandleSummaryInput( InputAtom *pEvent )
{
	if( !gfSummaryWindowActive )
		return FALSE;
	gfCtrlPressed = pEvent->usKeyState & CTRL_DOWN;
	if( !HandleTextInput( pEvent ) && pEvent->usEvent == KEY_DOWN || pEvent->usEvent == KEY_REPEAT )
	{
		INT32 x;
		switch( pEvent->usParam )
		{
			case ESC:
				if( !gfWorldLoaded )
				{
					DestroySummaryWindow();
					pEvent->usParam = 'x';
					pEvent->usKeyState |= ALT_DOWN;
					gfOverheadMapDirty = TRUE;
					return FALSE;
				}
			case ENTER:
				if( GetActiveFieldID() == 1 )
					SelectNextField();
				else if( gfWorldLoaded )
					DestroySummaryWindow();
				break;
			case F6:
				PerformTest();
				break;
			case F7:
				for( x = 0; x < 10; x++ )
					PerformTest();
				break;
			case F8:
				for( x = 0; x < 100; x++ )
					PerformTest();
				break;
			case 'y':case 'Y':
				if( gusNumEntriesWithOutdatedOrNoSummaryInfo && !gfOutdatedDenied )
				{
					gfRenderSummary = TRUE;
					RegenerateSummaryInfoForAllOutdatedMaps();
				}
				if( !gfGlobalSummaryExists && !gfDeniedSummaryCreation )
				{
					gfGlobalSummaryExists = TRUE;
					CreateGlobalSummary();
					gfRenderSummary = TRUE;
				}
				break;
			case 'n':case 'N':
				if( gusNumEntriesWithOutdatedOrNoSummaryInfo && !gfOutdatedDenied )
				{
					gfOutdatedDenied = TRUE;
					gfRenderSummary = TRUE;
				}
				if( !gfGlobalSummaryExists && !gfDeniedSummaryCreation )
				{
					gfDeniedSummaryCreation = TRUE;
					gfRenderSummary = TRUE;
				}
				break;
			case 'x':
				if (pEvent->usKeyState & ALT_DOWN )
				{ 
					DestroySummaryWindow();
					return FALSE;
				}
				break;
			case RIGHTARROW:
				gfRenderSummary = TRUE;
				if( !gsSelSectorY )
					gsSelSectorY = 1;
				gsSelSectorX++;
				if( gsSelSectorX > 16 )
					gsSelSectorX = 1;
				break;
			case LEFTARROW:
				gfRenderSummary = TRUE;
				if( !gsSelSectorY )
					gsSelSectorY = 1;
				gsSelSectorX--;
				if( gsSelSectorX < 1 )
					gsSelSectorX = 16;
				break;
			case UPARROW:
				gfRenderSummary = TRUE;
				if( !gsSelSectorX )
					gsSelSectorX = 1;
				gsSelSectorY--;
				if( gsSelSectorY < 1 )
					gsSelSectorY = 16;
				break;
			case DNARROW:
				gfRenderSummary = TRUE;
				if( !gsSelSectorX )
					gsSelSectorX = 1;
				gsSelSectorY++;
				if( gsSelSectorY > 16 )
					gsSelSectorY = 1;
				break;
			case SHIFT_LEFTARROW:
				
				break;
			case SHIFT_RIGHTARROW:

				break;
#ifdef dnlTEST//dnl ch81 051213
			case '\'':
				break;
#endif
		}
	}
	else if( pEvent->usEvent == KEY_UP )
	{ //for releasing modes requiring persistant state keys
		switch( pEvent->usParam )
		{
			case F5:
				ReleaseSummaryWindow();
				break;
		}
	}
	return TRUE;
}

void CreateGlobalSummary()
{
#ifndef USE_VFS
	FILE *fp;
	STRING512			Dir;
	STRING512			ExecDir;
#endif
	OutputDebugString( "Generating GlobalSummary Information...\n" );

	gfGlobalSummaryExists = FALSE;
	//Set current directory to JA2\DevInfo which contains all of the summary data
#ifndef USE_VFS
	GetExecutableDirectory( ExecDir );
	sprintf( Dir, "%s\\DevInfo", ExecDir );

	// Snap: save current directory
	//GetFileManCurrentDirectory( DataDir );

	//Directory doesn't exist, so create it, and continue.
	if( !MakeFileManDirectory( Dir ) )
		AssertMsg( 0, "Can't create new directory, JA2\\DevInfo for summary information." );
	if( !SetFileManCurrentDirectory( Dir ) )
		AssertMsg( 0, "Can't set to new directory, JA2\\DevInfo for summary information." );

	//Generate a simple readme file.
	fp = fopen( "readme.txt", "w" );
	Assert( fp );
	fprintf( fp, "%s\n%s\n", "This information is used in conjunction with the editor.",
		"This directory or it's contents shouldn't be included with final release." );
	fclose( fp );
#else
	vfs::COpenWriteFile wfile(L"DevInfo\\readme.txt",true,true);
	std::string str = "This information is used in conjunction with the editor.\n";
	str += "This directory or it's contents shouldn't be included with final release.\n";
	SGP_TRYCATCH_RETHROW( wfile->write(str.c_str(), str.length()), L"" );
#endif

	// Snap: Restore the data directory once we are finished.
	//SetFileManCurrentDirectory( DataDir );
	//sprintf( Dir, "%s\\Data", ExecDir );
	//SetFileManCurrentDirectory( Dir );

	LoadGlobalSummary();
	RegenerateSummaryInfoForAllOutdatedMaps();
	gfRenderSummary = TRUE;

	OutputDebugString( "GlobalSummary Information generated successfully.\n" );
}

void MapMoveCallback( MOUSE_REGION *reg, INT32 reason )
{
	static INT16 gsPrevX = 0, gsPrevY = 0;
	//calc current sector highlighted.
	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		gsPrevX = gsHiSectorX = 0;
		gsPrevY = gsHiSectorY = 0;
		gfRenderMap = TRUE;
		return;
	}
	gsHiSectorX = min( (reg->RelativeXPos / 13) + 1, 16 );
	gsHiSectorY = min( (reg->RelativeYPos / 13) + 1, 16 );
	if( gsPrevX != gsHiSectorX || gsPrevY != gsHiSectorY )
	{
		gsPrevX = gsHiSectorX;
		gsPrevY = gsHiSectorY;
		gfRenderMap = TRUE;
	}
}

void MapClickCallback( MOUSE_REGION *reg, INT32 reason )
{
	static INT16 sLastX = -1, sLastY = -1;
	static INT32 iLastClickTime = 0;
	//calc current sector selected.
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( GetActiveFieldID() == 1 )
		{
			gsSelSectorX = 0;
			SelectNextField();
		}
		gsSelSectorX = min( (reg->RelativeXPos / 13) + 1, 16 );
		gsSelSectorY = min( (reg->RelativeYPos / 13) + 1, 16 );
		if( gsSelSectorX != sLastX || gsSelSectorY != sLastY )
		{ //clicked in a new sector
			gfOverrideDirty = TRUE;
			sLastX = gsSelSectorX;
			sLastY = gsSelSectorY;
			iLastClickTime = GetJA2Clock();
			switch( giCurrentViewLevel )
			{
				case ALL_LEVELS_MASK:
					if( gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 0 ] )
						gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 0 ];
					else if( gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 1 ] )
						gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 1 ];
					else if( gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 2 ] )
						gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 2 ];
					else if( gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 3 ] )
						gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 3 ];
					else
						gpCurrentSectorSummary = NULL;
					break;
				case GROUND_LEVEL_MASK: //already pointing to the correct level
					gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 0 ];
					break;
				case BASEMENT1_LEVEL_MASK:
					gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 1 ];
					break;
				case BASEMENT2_LEVEL_MASK:
					gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 2 ];
					break;
				case BASEMENT3_LEVEL_MASK:
					gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 3 ];
					break;
				case ALTERNATE_LEVELS_MASK:
					if( gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 4 ] )
						gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 4 ];
					else if( gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 5 ] )
						gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 5 ];
					else if( gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 6 ] )
						gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 6 ];
					else if( gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 7 ] )
						gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 7 ];
					else
						gpCurrentSectorSummary = NULL;
					break;
				case ALTERNATE_GROUND_MASK: //already pointing to the correct level
					gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 4 ];
					break;
				case ALTERNATE_B1_MASK:
					gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 5 ];
					break;
				case ALTERNATE_B2_MASK:
					gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 6 ];
					break;
				case ALTERNATE_B3_MASK:
					gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1 ][ gsSelSectorY - 1 ][ 7 ];
					break;
			}
			if( gpWorldItemsSummaryArray )
			{
				delete[]( gpWorldItemsSummaryArray );
				gpWorldItemsSummaryArray = NULL;
				gusWorldItemsSummaryArraySize = 0;
			}
			if( gfItemDetailsMode )
			{
				if( gpCurrentSectorSummary )
					/*&& gpCurrentSectorSummary->usNumItems || 
						gpPEnemyItemsSummaryArray && gusPEnemyItemsSummaryArraySize ||
						gpNEnemyItemsSummaryArray && gusNEnemyItemsSummaryArraySize )*/
				{
					gfSetupItemDetailsMode = TRUE;
				}
			}
		}
		else
		{ //clicked in same sector, check for double click
			INT32 iNewClickTime = GetJA2Clock();
			if( iNewClickTime - iLastClickTime < 400 )
			{
				gfFileIO = TRUE;
			}
			iLastClickTime = iNewClickTime;
		}	
		gfRenderSummary = TRUE;
	}
}

void SummaryToggleLevelCallback( GUI_BUTTON *btn, INT32 reason )
{
	INT8 i;
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( GetActiveFieldID() == 1 )
			SelectNextField();
		gfRenderSummary = TRUE;
		if(gfItemDetailsMode)//dnl ch81 041213
			gfSetupItemDetailsMode = TRUE;
		for( i = SUMMARY_ALL; i <= SUMMARY_B3; i++ )
		{
			if( btn->IDNum == iSummaryButton[ i ] )
			{
				switch( i )
				{
					case SUMMARY_ALL:
						giCurrentViewLevel = ALL_LEVELS_MASK;
						break;
					case SUMMARY_G:
						giCurrentViewLevel = GROUND_LEVEL_MASK;
						break;
					case SUMMARY_B1:
						giCurrentViewLevel = BASEMENT1_LEVEL_MASK;
						break;
					case SUMMARY_B2:
						giCurrentViewLevel = BASEMENT2_LEVEL_MASK;
						break;
					case SUMMARY_B3:
						giCurrentViewLevel = BASEMENT3_LEVEL_MASK;
						break;
				}
				if( gfAlternateMaps )
					giCurrentViewLevel <<= 4;
			}
			else
			{
				ButtonList[ iSummaryButton[ i ] ]->uiFlags &= (~BUTTON_CLICKED_ON);
			}
		}
	}
}

void SummaryLoadMapCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		STR16 ptr;
		CHAR16 str[ 50 ];
		gfRenderSummary = TRUE;
		
		SetFont( FONT10ARIAL );
		SetFontForeground( FONT_LTKHAKI );
		SetFontShadow( FONT_NEARBLACK );

		fNewMapSaved = TRUE;

		//swprintf( str, L"Loading map:  %s...", gszDisplayName );
		//mprintf( MAP_LEFT, MAP_BOTTOM+100, str );
		//InvalidateRegion( MAP_LEFT, MAP_BOTTOM+100, MAP_LEFT+150,	MAP_BOTTOM+110 );
		
		CreateProgressBar( 0, MAP_LEFT+5, MAP_BOTTOM+110, iScreenWidthOffset + 573, MAP_BOTTOM+120 );

		//DefineProgressBarPanel( 0, 65, 79, 94, MAP_LEFT, iScreenHeightOffset + 318, iScreenWidthOffset + 578, iScreenHeightOffset + 356 );
		DefineProgressBarPanel( 0, 65, 79, 94, MAP_LEFT, iScreenHeightOffset + 318, MAP_LEFT + 161, iScreenHeightOffset + 356 );
		swprintf( str, pSummaryLoadMapCallbackText[0], gszDisplayName );
		SetProgressBarTitle( 0, str, BLOCKFONT2, FONT_RED, FONT_NEARBLACK );
		SetProgressBarMsgAttributes( 0, SMALLCOMPFONT, FONT_BLACK, FONT_BLACK );

		// WANNE: When loading from summary map, never resize the map, just load it!
		gfResizeMapOnLoading = FALSE;

		if(	ExternalLoadMap( gszDisplayName ) )
		{
			EnableButton( iSummaryButton[ SUMMARY_OKAY ] );
			gsSectorX = gsSelSectorX;
			gsSectorY = gsSelSectorY;
			gfOverrideDirty = TRUE;
			gfMapFileDirty = FALSE;
		}
		RemoveProgressBar( 0 );
		ptr = wcsstr( gszDisplayName, L"_b" );
		if( !ptr || ptr[3] != L'.' )
		{
			gsSectorLayer = GROUND_LEVEL_MASK;
			giCurrLevel = 0;
		}
		else
		{
			switch( ptr[2] - L'0' )
			{
				case 1:		gsSectorLayer = BASEMENT1_LEVEL_MASK;	break;
				case 2:		gsSectorLayer = BASEMENT2_LEVEL_MASK;	break;
				case 3:		gsSectorLayer = BASEMENT3_LEVEL_MASK;	break;
				default:	gsSectorLayer = GROUND_LEVEL_MASK;		break;
			}
		}
	}
}

void SummarySaveMapCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gfRenderSummary = TRUE;
		if( gubOverrideStatus == INACTIVE || gfOverride == TRUE )
		{
			if( gubOverrideStatus == READONLY )
			{
				CHAR8 filename[40];
				sprintf( filename, "MAPS\\%S", gszDisplayName );
				FileClearAttributes( filename );
			}	
			if(	ExternalSaveMap( gszDisplayName ) )
			{
				if( gsSelSectorX && gsSelSectorY )
				{
					gsSectorX = gsSelSectorX;
					gsSectorY = gsSelSectorY;
					gfMapFileDirty = FALSE;
					gfOverrideDirty = TRUE;
				}
			}
		}
	}
}

void SummaryOverrideCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gfOverride ^= TRUE;
		gfRenderSummary = TRUE;
		if( gfOverride )
			EnableButton( iSummaryButton[ SUMMARY_SAVE ] );
		else
			DisableButton( iSummaryButton[ SUMMARY_SAVE ] );
	}
}

void CalculateOverrideStatus()
{
	GETFILESTRUCT FileInfo;
	CHAR8 szFilename[40];
	gfOverrideDirty = FALSE;
	gfOverride = FALSE;
	if( gfTempFile )
	{
		CHAR8 *ptr;
		sprintf( szFilename, "MAPS\\%S", gszTempFilename );
		if( strlen( szFilename ) == 5 )
			strcat( szFilename, "test.dat" );
		ptr = strstr( szFilename, "." );
		if( !ptr )
			strcat( szFilename, ".dat" );
		else
			sprintf( ptr, ".dat" );
	}
	else
		sprintf( szFilename, "MAPS\\%S", gszFilename );
	swprintf( gszDisplayName, L"%S", &(szFilename[5]) );
	if( GetFileFirst( szFilename, &FileInfo) )
	{
		if( gfWorldLoaded )
		{
			if( FileInfo.uiFileAttribs & ( FILE_IS_READONLY | FILE_IS_SYSTEM ) )
				gubOverrideStatus = READONLY;
			else
				gubOverrideStatus = OVERWRITE;
			ShowButton( iSummaryButton[ SUMMARY_OVERRIDE ] );
			ButtonList[ iSummaryButton[ SUMMARY_OVERRIDE ] ]->uiFlags &= (~BUTTON_CLICKED_ON);
			GetFileClose(&FileInfo);
			DisableButton( iSummaryButton[ SUMMARY_SAVE ] );
		}
		if( gfTempFile )
			EnableButton( iSummaryButton[ SUMMARY_LOAD ] );
	}
	else
	{
		gubOverrideStatus = INACTIVE;
		HideButton( iSummaryButton[ SUMMARY_OVERRIDE ] );
		if( gfWorldLoaded )
			EnableButton( iSummaryButton[ SUMMARY_SAVE ] );
	}
}

void LoadGlobalSummary()
{
	HWFILE	hfile;
#ifndef USE_VFS
	STRING512			DataDir;
	STRING512			ExecDir;
	STRING512			DevInfoDir;
	STRING512			MapsDir;
#endif
	UINT32 uiNumBytesRead;
	FLOAT	dMajorVersion;
	INT32 x,y;
	CHAR8 szFilename[40];
	CHAR8 szSector[6];

	OutputDebugString( "Executing LoadGlobalSummary()...\n" );
	// Snap: save current directory
#ifndef USE_VFS
	GetFileManCurrentDirectory( DataDir );
#endif
	gfMustForceUpdateAllMaps = FALSE;
	gusNumberOfMapsToBeForceUpdated = 0;
	gfGlobalSummaryExists = FALSE;

	//Set current directory to JA2\DevInfo which contains all of the summary data
#ifndef USE_VFS
	GetExecutableDirectory( ExecDir );
	sprintf( DevInfoDir, "%s\\DevInfo", ExecDir );
	sprintf( MapsDir, "%s\\Maps", DataDir );
	//Check to make sure we have a DevInfo directory.  If we don't create one!
	if( !SetFileManCurrentDirectory( DevInfoDir ) )
	{
		OutputDebugString( "LoadGlobalSummary() aborted -- doesn't exist on this local computer.\n");
		return;
	}
#endif
	//TEMP
#ifndef USE_VFS
	FileDelete( "_global.sum" );
#else
	if(FileExists("DevInfo\\_global.sum"))
	{
		FileDelete( "DevInfo\\_global.sum" );
	}
#endif
	gfGlobalSummaryExists = TRUE;
	
	//Analyse all sectors to see if matching maps exist.  For any maps found, the information
	//will be stored in the gbSectorLevels array.  Also, it attempts to load summaries for those
	//maps.  If the summary information isn't found, then the occurrences are recorded and reported
	//to the user when finished to give the option to generate them.
	for( y = 0; y < 16; y++ ) 
	{
		for( x = 0; x < 16; x++ )
		{
			gbSectorLevels[x][y] = 0;
			sprintf( szSector, "%c%d", 'A' + y, x + 1 );

			//main ground level
#ifndef USE_VFS
			sprintf( szFilename, "%c%d.dat", 'A' + y, x + 1 );
			SetFileManCurrentDirectory( MapsDir );
			hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			SetFileManCurrentDirectory( DevInfoDir );
#else
			sprintf( szFilename, "Maps\\%c%d.dat", 'A' + y, x + 1 );
			hfile = NULL;
			if(FileExists(szFilename))
			{
				hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			}
#endif
			if( hfile )
			{
				gbSectorLevels[x][y] |= GROUND_LEVEL_MASK;
				FileRead( hfile, &dMajorVersion, sizeof( FLOAT ), &uiNumBytesRead );
				FileClose( hfile );
				LoadSummary( szSector, 0, dMajorVersion );
			}
			else
			{
#ifndef USE_VFS
				sprintf( szFilename, "%s.sum", szSector );
#else
				sprintf( szFilename, "DevInfo\\%s.sum", szSector );
#endif
				FileDelete( szFilename );
			}
			//main B1 level
#ifndef USE_VFS
			sprintf( szFilename, "%c%d_b1.dat", 'A' + y, x + 1 );
			SetFileManCurrentDirectory( MapsDir );
			hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			SetFileManCurrentDirectory( DevInfoDir );
#else
			sprintf( szFilename, "Maps\\%c%d_b1.dat", 'A' + y, x + 1 );
			hfile = NULL;
			if(FileExists(szFilename))
			{
				hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			}
#endif
			if( hfile )
			{
				gbSectorLevels[x][y] |= BASEMENT1_LEVEL_MASK;
				FileRead( hfile, &dMajorVersion, sizeof( FLOAT ), &uiNumBytesRead );
				FileClose( hfile );
				LoadSummary( szSector, 1, dMajorVersion );
			}
			else
			{
#ifndef USE_VFS
				sprintf( szFilename, "%s_b1.sum", szSector );
#else
				sprintf( szFilename, "DevInfo\\%s_b1.sum", szSector );
#endif
				FileDelete( szFilename );
			}
			//main B2 level
#ifndef USE_VFS
			sprintf( szFilename, "%c%d_b2.dat", 'A' + y, x + 1 );
			SetFileManCurrentDirectory( MapsDir );
			hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			SetFileManCurrentDirectory( DevInfoDir );
#else
			sprintf( szFilename, "Maps\\%c%d_b2.dat", 'A' + y, x + 1 );
			hfile = NULL;
			if(FileExists(szFilename))
			{
				hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			}
#endif
			if( hfile )
			{
				gbSectorLevels[x][y] |= BASEMENT2_LEVEL_MASK;
				FileRead( hfile, &dMajorVersion, sizeof( FLOAT ), &uiNumBytesRead );
				FileClose( hfile );
				LoadSummary( szSector, 2, dMajorVersion );
			}
			else
			{
#ifndef USE_VFS
				sprintf( szFilename, "%s_b2.sum", szSector );
#else
				sprintf( szFilename, "DevInfo\\%s_b2.sum", szSector );
#endif
				FileDelete( szFilename );
			}
			//main B3 level
#ifndef USE_VFS
			sprintf( szFilename, "%c%d_b3.dat", 'A' + y, x + 1 );
			SetFileManCurrentDirectory( MapsDir );
			hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			SetFileManCurrentDirectory( DevInfoDir );
#else
			sprintf( szFilename, "Maps\\%c%d_b3.dat", 'A' + y, x + 1 );
			hfile = NULL;
			if(FileExists(szFilename))
			{
				hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			}
#endif
			if( hfile )
			{
				gbSectorLevels[x][y] |= BASEMENT3_LEVEL_MASK;
				FileRead( hfile, &dMajorVersion, sizeof( FLOAT ), &uiNumBytesRead );
				FileClose( hfile );
				LoadSummary( szSector, 3, dMajorVersion );
			}
			else
			{
#ifndef USE_VFS
				sprintf( szFilename, "%s_b3.sum", szSector );
#else
				sprintf( szFilename, "DevInfo\\%s_b3.sum", szSector );
#endif
				FileDelete( szFilename );
			}
			//alternate ground level
#ifndef USE_VFS
			sprintf( szFilename, "%c%d_a.dat", 'A' + y, x + 1 );
			SetFileManCurrentDirectory( MapsDir );
			hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			SetFileManCurrentDirectory( DevInfoDir );
#else
			sprintf( szFilename, "Maps\\%c%d_a.dat", 'A' + y, x + 1 );
			hfile = NULL;
			if(FileExists(szFilename))
			{
				hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			}
#endif
			if( hfile )
			{
				gbSectorLevels[x][y] |= ALTERNATE_GROUND_MASK;
				FileRead( hfile, &dMajorVersion, sizeof( FLOAT ), &uiNumBytesRead );
				FileClose( hfile );
				LoadSummary( szSector, 4, dMajorVersion );
			}
			else
			{
#ifndef USE_VFS
				sprintf( szFilename, "%s_a.sum", szSector );
#else
				sprintf( szFilename, "DevInfo\\%s_a.sum", szSector );
#endif
				FileDelete( szFilename );
			}
			//alternate B1 level
#ifndef USE_VFS
			sprintf( szFilename, "%c%d_b1_a.dat", 'A' + y, x + 1 );
			SetFileManCurrentDirectory( MapsDir );
			hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			SetFileManCurrentDirectory( DevInfoDir );
#else
			sprintf( szFilename, "Maps\\%c%d_b1_a.dat", 'A' + y, x + 1 );
			hfile = NULL;
			if(FileExists(szFilename))
			{
				hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			}
#endif
			if( hfile )
			{
				gbSectorLevels[x][y] |= ALTERNATE_B1_MASK;
				FileRead( hfile, &dMajorVersion, sizeof( FLOAT ), &uiNumBytesRead );
				FileClose( hfile );
				LoadSummary( szSector, 5, dMajorVersion );
			}
			else
			{
#ifndef USE_VFS
				sprintf( szFilename, "%s_b1_a.sum", szSector );
#else
				sprintf( szFilename, "DevInfo\\%s_b1_a.sum", szSector );
#endif
				FileDelete( szFilename );
			}
			//alternate B2 level
#ifndef USE_VFS
			sprintf( szFilename, "%c%d_b2_a.dat", 'A' + y, x + 1 );
			SetFileManCurrentDirectory( MapsDir );
			hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			SetFileManCurrentDirectory( DevInfoDir );
#else
			sprintf( szFilename, "Maps\\%c%d_b2_a.dat", 'A' + y, x + 1 );
			hfile = NULL;
			if(FileExists(szFilename))
			{
				hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			}
#endif
			if( hfile )
			{
				gbSectorLevels[x][y] |= ALTERNATE_B2_MASK;
				FileRead( hfile, &dMajorVersion, sizeof( FLOAT ), &uiNumBytesRead );
				FileClose( hfile );
				LoadSummary( szSector, 6, dMajorVersion );
			}
			else
			{
#ifndef USE_VFS
				sprintf( szFilename, "%s_b2_a.sum", szSector );
#else
				sprintf( szFilename, "DevInfo\\%s_b2_a.sum", szSector );
#endif
				FileDelete( szFilename );
			}
			//alternate B3 level
#ifndef USE_VFS
			sprintf( szFilename, "%c%d_b3_a.dat", 'A' + y, x + 1 );
			SetFileManCurrentDirectory( MapsDir );
			hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			SetFileManCurrentDirectory( DevInfoDir );
#else
			sprintf( szFilename, "Maps\\%c%d_b3_a.dat", 'A' + y, x + 1 );
			hfile = NULL;
			if(FileExists(szFilename))
			{
				hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			}
#endif
			if( hfile )
			{
				gbSectorLevels[x][y] |= ALTERNATE_B1_MASK;;
				FileRead( hfile, &dMajorVersion, sizeof( FLOAT ), &uiNumBytesRead );
				FileClose( hfile );
				LoadSummary( szSector, 7, dMajorVersion );
			}
			else
			{
#ifndef USE_VFS
				sprintf( szFilename, "%s_b3_a.sum", szSector );
#else
				sprintf( szFilename, "DevInfo\\%s_b3_a.sum", szSector );
#endif
				FileDelete( szFilename );
			}
		}
		OutputDebugString( (LPCSTR)String("Sector Row %c complete... \n", y + 'A') );
	}
#ifndef USE_VFS
	// Snap: Restore the data directory once we are finished.
	SetFileManCurrentDirectory( DataDir );
#endif
	//sprintf( MapsDir, "%s\\Data", ExecDir );
	//SetFileManCurrentDirectory( MapsDir );

	if( gfMustForceUpdateAllMaps )
	{
		OutputDebugString( (LPCSTR)String( "A MAJOR MAP UPDATE EVENT HAS BEEN DETECTED FOR %d MAPS!!!!.\n", gusNumberOfMapsToBeForceUpdated ) );
		//ADB update it!
		ApologizeOverrideAndForceUpdateEverything();
	}


	OutputDebugString( "LoadGlobalSummary() finished...\n" );
}

void GenerateSummaryList()
{
#ifndef USE_VFS
	FILE *fp;
	STRING512			DataDir;
	STRING512			ExecDir;
	STRING512			Dir;

	// Snap: save current directory
	GetFileManCurrentDirectory( DataDir );

	//Set current directory to JA2\DevInfo which contains all of the summary data
	GetExecutableDirectory( ExecDir );
	sprintf( Dir, "%s\\DevInfo", ExecDir );
	if( !SetFileManCurrentDirectory( Dir ) )
	{
		//Directory doesn't exist, so create it, and continue.
		if( !MakeFileManDirectory( Dir ) )
			AssertMsg( 0, "Can't create new directory, JA2\\DevInfo for summary information." );
		if( !SetFileManCurrentDirectory( Dir ) )
			AssertMsg( 0, "Can't set to new directory, JA2\\DevInfo for summary information." );
		//Generate a simple readme file.
		fp = fopen( "readme.txt", "w" );
		Assert( fp );
		fprintf( fp, "%s\n%s\n", "This information is used in conjunction with the editor.",
			"This directory or it's contents shouldn't be included with final release." );
		fclose( fp );
	}

	// Snap: Restore the data directory once we are finished.
	SetFileManCurrentDirectory( DataDir );
	//Set current directory back to data directory!
	//sprintf( Dir, "%s\\Data", ExecDir );
	//SetFileManCurrentDirectory( Dir );
#else
	try
	{
		vfs::COpenWriteFile wfile(L"DevInfo/readme.txt",true,true);
		std::string str = "This information is used in conjunction with the editor.\n";
		str += "This directory or it's contents shouldn't be included with final release.\n";
		SGP_TRYCATCH_RETHROW( wfile->write(str.c_str(), str.length()), L"");
	}
	catch(std::exception &ex)
	{
		SGP_RETHROW(L"Could not create readme.txt", ex);
	}
#endif
}

//dnl ch28 260909
void WriteSectorSummaryUpdate(STR8 sFileName, UINT8 ubLevel, SUMMARYFILE* pSummaryFileInfo)
{
	CHAR8 cFileName[2*FILENAME_BUFLEN];
	sprintf(cFileName, "DevInfo\\%s", sFileName);
	// Presumption is that file came with dat extension
	strcpy(cFileName+strlen(cFileName)-4, ".sum");
	if(!pSummaryFileInfo->Save(cFileName))
		return;
	UpdateMasterProgress();
	INT16 sSectorX, sSectorY;
	INT8 x, y, bSectorZ;
	BOOLEAN fAltMap;
	CHAR16 szFileName[FILENAME_BUFLEN];
	swprintf(szFileName, L"%S", sFileName);
	GetSectorFromFileName(szFileName, sSectorX, sSectorY, bSectorZ, fAltMap);
	if(!sSectorX)
		gCustomFileSectorSummary = *pSummaryFileInfo;//dnl ch30 150909
	else
	{
		if(gusNumEntriesWithOutdatedOrNoSummaryInfo > 0)
			gusNumEntriesWithOutdatedOrNoSummaryInfo--;
		x = sSectorX - 1;
		y = sSectorY - 1;
		if(gpSectorSummary[x][y][ubLevel])
		{
			MemFree(gpSectorSummary[x][y][ubLevel]);
			gpSectorSummary[x][y][ubLevel] = NULL;
		}
		gpSectorSummary[x][y][ubLevel] = pSummaryFileInfo;
	}
}

void SummaryNewGroundLevelCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gfPendingBasement = FALSE;
		gfPendingCaves = FALSE;
		if( gfWorldLoaded )
		{
			iCurrentAction = ACTION_NEW_MAP;
		}
		else
		{
			CreateNewMap();
		}
	}
}

void SummaryNewBasementLevelCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{

	}
}

void SummaryNewCaveLevelCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{

	}
}

//dnl ch28 260909
SUMMARYFILE& SUMMARYFILE::operator=(const _OLD_SUMMARYFILE& src)
{
	if((void*)this != (void*)&src)
	{
		ubSummaryVersion = src.ubSummaryVersion;
		ubSpecial = src.ubSpecial;
		ubNumRooms = src.ubNumRooms;
		dMajorMapVersion = src.dMajorMapVersion;
		ubTilesetID = src.ubTilesetID;
		usNumItems = src.usNumItems;
		usNumLights = src.usNumLights;
		ubNumDoors = src.ubNumDoors;
		ubNumDoorsLocked = src.ubNumDoorsLocked;
		ubNumDoorsTrapped = src.ubNumDoorsTrapped;
		ubNumDoorsLockedAndTrapped = src.ubNumDoorsLockedAndTrapped;
		ubNumElites = src.ubNumElites;
		ubNumAdmins = src.ubNumAdmins;
		ubNumTroops = src.ubNumTroops;
		ubEliteDetailed = src.ubEliteDetailed;
		ubAdminDetailed = src.ubAdminDetailed;
		ubTroopDetailed = src.ubTroopDetailed;
		ubEliteProfile = src.ubEliteProfile;
		ubAdminProfile = src.ubAdminProfile;
		ubTroopProfile = src.ubTroopProfile;
		ubEliteExistance = src.ubEliteExistance;
		ubAdminExistance = src.ubAdminExistance;
		ubTroopExistance = src.ubTroopExistance;
		ubCivSchedules = src.ubCivSchedules;
		ubCivCows = src.ubCivCows;
		ubCivBloodcats = src.ubCivBloodcats;
		ubEnemiesReqWaypoints = src.ubEnemiesReqWaypoints;
		usWarningRoomNums = src.usWarningRoomNums;
		ubEnemiesHaveWaypoints = src.ubEnemiesHaveWaypoints;
		uiNumItemsPosition = src.uiNumItemsPosition;
		uiEnemyPlacementPosition = src.uiEnemyPlacementPosition;
		EnemyTeam = src.EnemyTeam;
		CreatureTeam = src.CreatureTeam;
		RebelTeam = src.RebelTeam;
		CivTeam = src.CivTeam;
		MapInfo = src.MapInfo;
		for(int i=0; i<4; i++)
			ExitGrid[i] = src.ExitGrid[i];
		TranslateArrayFields(usExitGridSize, src.usExitGridSize, 4, UINT16_UINT16);
		TranslateArrayFields(fInvalidDest, src.fInvalidDest, 4, UINT8_UINT8);
		ubNumExitGridDests = src.ubNumExitGridDests;
		fTooManyExitGridDests = src.fTooManyExitGridDests;
	}
	return(*this);
}

BOOLEAN SUMMARYFILE::Load(STR sFileName)
{
	if(!FileExists(sFileName))
	{
		return FALSE;
	}
	HWFILE hFile = FileOpen(sFileName, FILE_ACCESS_READ, FALSE);
	if(!hFile)
	{
		return(FALSE);
	}
	UINT8 ubSummaryVersion = 0;
	UINT32 uiBytesRead = 0;
	FileRead(hFile, &ubSummaryVersion, sizeof(ubSummaryVersion), &uiBytesRead);
	if(ubSummaryVersion < 14 || ubSummaryVersion > GLOBAL_SUMMARY_VERSION)
	{
		FileClose(hFile);
		return(FALSE);
	}
	FileSeek(hFile, 0L, FILE_SEEK_FROM_START);
	if(ubSummaryVersion == 14)
	{
		_OLD_SUMMARYFILE OldSummaryFile;
		FileRead(hFile, &OldSummaryFile, sizeof(_OLD_SUMMARYFILE), &uiBytesRead);
		*this = OldSummaryFile;
	}
	else
	{
		FileRead(hFile, this, sizeof(SUMMARYFILE), &uiBytesRead);
	}
	FileClose(hFile);
	if(uiBytesRead == sizeof(_OLD_SUMMARYFILE) || uiBytesRead == sizeof(SUMMARYFILE))
	{
		return(TRUE);
	}
	return(FALSE);
}

BOOLEAN SUMMARYFILE::Save(STR sFileName)
{
	HWFILE hFile = FileOpen(sFileName, FILE_ACCESS_WRITE|FILE_CREATE_ALWAYS, FALSE);
	if(!hFile)
		return(FALSE);
	UINT32 uiBytesWritten = 0;
	FileWrite(hFile, this, sizeof(SUMMARYFILE), &uiBytesWritten);
	FileClose(hFile);
	if(uiBytesWritten != sizeof(SUMMARYFILE))
		return(FALSE);
	return(TRUE);
}

BOOLEAN LoadSummary(STR8 pSector, UINT8 ubLevel, FLOAT dMajorMapVersion)
{
	INT32 x, y;
	CHAR8 sFileName[2*FILENAME_BUFLEN];
	SUMMARYFILE SummaryFile;
	sprintf(sFileName, "DevInfo\\%s", pSector);
	if(ubLevel % 4)
	{
		CHAR8 str[4];
		sprintf(str, "_b%d", ubLevel%4);
		strcat(sFileName, str);
	}
	if(ubLevel >= 4)
		strcat(sFileName, "_a");
	strcat(sFileName, ".sum");
	if(!SummaryFile.Load(sFileName))
		return(FALSE);
	y = pSector[0] - 'A';
	if(pSector[2] >= '0' && pSector[2] <= '9')
		x = (pSector[1] - '0') * 10 + pSector[2] - '0' - 1;
	else
		x = pSector[1] - '0' - 1;
	if(gpSectorSummary[x][y][ubLevel])
	{
		MemFree(gpSectorSummary[x][y][ubLevel]);
		gpSectorSummary[x][y][ubLevel] = NULL;
	}
	gpSectorSummary[x][y][ubLevel] = (SUMMARYFILE*)MemAlloc(sizeof(SUMMARYFILE));
	if(gpSectorSummary[x][y][ubLevel])
		memcpy(gpSectorSummary[x][y][ubLevel], &SummaryFile, sizeof(SUMMARYFILE));
	if(gpSectorSummary[x][y][ubLevel]->ubSummaryVersion < GLOBAL_SUMMARY_VERSION)
		gusNumEntriesWithOutdatedOrNoSummaryInfo++;
	return(TRUE);
}

double MasterStart, MasterEnd;

void UpdateMasterProgress()
{
	if( gfUpdatingNow && gusTotal )
	{
		MasterStart = (gusCurrent / (double)gusTotal) * 100.0;
		gusCurrent++;
		MasterEnd = (gusCurrent / (double)gusTotal) * 100.0;
		if( gfMajorUpdate )
		{
			SetRelativeStartAndEndPercentage( 2, (UINT16)MasterStart, (UINT16)MasterEnd, NULL );
			RenderProgressBar( 2, 0 );
		}
		else
			SetRelativeStartAndEndPercentage( 1, (UINT16)MasterStart, (UINT16)MasterEnd, NULL );
	}
}

void ReportError( STR8 pSector, UINT8 ubLevel )
{
	static INT32 yp = iScreenHeightOffset + 180;
	CHAR16 str[40];
	CHAR16 temp[10];

	//Make sure the file exists... if not, then return false
	swprintf( str, L"%S", pSector );
	if( ubLevel % 4  )
	{
		swprintf( temp, L"_b%d.dat", ubLevel % 4 );
		wcscat( str, temp );
	}
	mprintf( iScreenWidthOffset + 10, yp, pReportErrorText[0] , str );
	InvalidateScreen();
	yp++;
}


void RegenerateSummaryInfoForAllOutdatedMaps()
{
	INT32 x, y;
	CHAR8 str[40];
	SUMMARYFILE *pSF;
	//CreateProgressBar( 0, 20, 120, 300, 132 ); //slave (individual)
	//CreateProgressBar( 1, 20, 100, 300, 112 ); //master (total)
	//DefineProgressBarPanel( 0, 65, 79, 94, 10, 80, 310, 152 );
	CreateProgressBar( 0, iScreenWidthOffset + 20, iScreenHeightOffset + 100, iScreenWidthOffset + 300, iScreenHeightOffset + 112 ); //master (total)
	DefineProgressBarPanel( 0, 65, 79, 94, iScreenWidthOffset + 10, iScreenHeightOffset + 80, iScreenWidthOffset + 310, iScreenHeightOffset + 132 );
	SetProgressBarTitle( 0, pRegenerateSummaryInfoForAllOutdatedMapsText[0], BLOCKFONT2, FONT_RED, FONT_NEARBLACK );
	SetProgressBarMsgAttributes( 0, SMALLCOMPFONT, FONT_BLACK, FONT_BLACK );
	gfUpdatingNow = TRUE;

	gusCurrent = 0;
	gusTotal = gusNumEntriesWithOutdatedOrNoSummaryInfo;
	UpdateMasterProgress();

	for( y = 0; y < 16; y++ ) for( x = 0; x < 16; x++ )
	{
		sprintf( str, "%c%d", y + 'A', x + 1 );
		if( gbSectorLevels[x][y] & GROUND_LEVEL_MASK )
		{
			pSF = gpSectorSummary[x][y][0];
			if( !pSF || pSF->ubSummaryVersion != GLOBAL_SUMMARY_VERSION )
				if( !EvaluateWorld( str, 0 ) )
					ReportError( str, 0 );
		}
		if( gbSectorLevels[x][y] & BASEMENT1_LEVEL_MASK )
		{
			pSF = gpSectorSummary[x][y][1];
			if( !pSF || pSF->ubSummaryVersion != GLOBAL_SUMMARY_VERSION )
				if( !EvaluateWorld( str, 1 ) )
					ReportError( str, 1 );
		}
		if( gbSectorLevels[x][y] & BASEMENT2_LEVEL_MASK )
		{
			pSF = gpSectorSummary[x][y][2];
			if( !pSF || pSF->ubSummaryVersion != GLOBAL_SUMMARY_VERSION )
				if( !EvaluateWorld( str, 2 ) )
					ReportError( str, 2 );
		}
		if( gbSectorLevels[x][y] & BASEMENT3_LEVEL_MASK )
		{
			pSF = gpSectorSummary[x][y][3];
			if( !pSF || pSF->ubSummaryVersion != GLOBAL_SUMMARY_VERSION )
				if( !EvaluateWorld( str, 3 ) )
					ReportError( str, 3 );
		}
		if( gbSectorLevels[x][y] & ALTERNATE_GROUND_MASK )
		{
			pSF = gpSectorSummary[x][y][4];
			if( !pSF || pSF->ubSummaryVersion != GLOBAL_SUMMARY_VERSION )
				if( !EvaluateWorld( str, 4 ) )
					ReportError( str, 4 );
		}
		if( gbSectorLevels[x][y] & ALTERNATE_B1_MASK )
		{
			pSF = gpSectorSummary[x][y][5];
			if( !pSF || pSF->ubSummaryVersion != GLOBAL_SUMMARY_VERSION )
				if( !EvaluateWorld( str, 5 ) )
					ReportError( str, 5 );
		}
		if( gbSectorLevels[x][y] & ALTERNATE_B2_MASK )
		{
			pSF = gpSectorSummary[x][y][6];
			if( !pSF || pSF->ubSummaryVersion != GLOBAL_SUMMARY_VERSION )
				if( !EvaluateWorld( str, 6 ) )
					ReportError( str, 6 );
		}
		if( gbSectorLevels[x][y] & ALTERNATE_B3_MASK )
		{
			pSF = gpSectorSummary[x][y][7];
			if( !pSF || pSF->ubSummaryVersion != GLOBAL_SUMMARY_VERSION )
				if( !EvaluateWorld( str, 7 ) )
					ReportError( str, 7 );
		}
	}
	RemoveProgressBar( 0 );
	RemoveProgressBar( 1 );
	gfUpdatingNow = FALSE;
}

void SummaryUpdateCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		CHAR8 str[40];
		CreateProgressBar( 0, iScreenWidthOffset + 20, iScreenHeightOffset + 100, iScreenWidthOffset + 300, iScreenHeightOffset + 112 ); //slave (individual)
		DefineProgressBarPanel( 0, 65, 79, 94, iScreenWidthOffset + 10, iScreenHeightOffset + 80, iScreenWidthOffset + 310, iScreenHeightOffset + 132 );
		SetProgressBarTitle( 0, pSummaryUpdateCallbackText[0], BLOCKFONT2, FONT_RED, FONT_NEARBLACK );
		SetProgressBarMsgAttributes( 0, SMALLCOMPFONT, FONT_BLACK, FONT_BLACK );

#if 0
		// 0verhaul:  This should NOT be freed.  An array index can be freed when it is recalculated,
		// as this function is about to do.  And then it SHOULD be freed first, which it wasn't doing.
		// Either way, using this pointer is not a safe way to free the current sector's summary data.
		if( gpCurrentSectorSummary )
		{
			MemFree( gpCurrentSectorSummary );
			gpCurrentSectorSummary = NULL;
		}
#endif

		sprintf( str, "%c%d", gsSelSectorY + 'A' - 1, gsSelSectorX );
		EvaluateWorld( str, (UINT8)giCurrLevel );

		// ADB: The current sector summary should be set to the array value, not the other way around.
		// Also the sector X and Y values are 1-based and the array is 0-based.  So subtract 1 from X and Y
		gpCurrentSectorSummary = gpSectorSummary[ gsSelSectorX - 1][ gsSelSectorY - 1][ giCurrLevel ];
	
		gfRenderSummary = TRUE;
		if(gfItemDetailsMode)//dnl ch81 041213
			gfSetupItemDetailsMode = TRUE;

		RemoveProgressBar( 0 );
	}
}

void ExtractTempFilename()
{
	//dnl ch39 190909
	CHAR16 str[FILENAME_BUFLEN-4];
	Get16BitStringFromField(1, str, FILENAME_BUFLEN-4);
	if( wcscmp( gszTempFilename, str ) )
	{
		wcscpy( gszTempFilename, str );
		gfRenderSummary = TRUE;
		gfOverrideDirty = TRUE;
	}
	if( !wcslen( str ) )
		swprintf( gszDisplayName, L"test.dat" );
}

//dnl ch30 170909
BOOLEAN ReEvaluateWorld(const STR8 puiFilename)
{
	UINT8 ubLevel;
	CHAR8 name[FILENAME_BUFLEN];//dnl ch81 021213
	INT16 sSectorX, sSectorY;
	INT8 bSectorZ;
	BOOLEAN fAltMap;
	CHAR16 szFileName[FILENAME_BUFLEN];//dnl ch81 021213
	swprintf(szFileName, L"%S", puiFilename);
	GetSectorFromFileName(szFileName, sSectorX, sSectorY, bSectorZ, fAltMap);
/*
	switch(bSector)
	{
	case 1:
		ubLevel = 1;
		if(fAltMap)
			ubLevel = 5;
		break;
	case 2:
		ubLevel = 2;
		if(fAltMap)
			ubLevel = 6;
		break;
	case 3:
		ubLevel = 3;
		if(fAltMap)
			ubLevel = 7;
		break;
	default:
		ubLevel = 0;
		if(fAltMap)
			ubLevel = 4;
		break;
	}
*/
	ubLevel = bSectorZ;
	if(fAltMap)
		ubLevel += 4;
	if(sSectorX)
		sprintf(name, "%c%d", (sSectorY-1+'A'), sSectorX);
	else
		strcpy(name, puiFilename);
	if(!EvaluateWorld(name, ubLevel))
		return(FALSE);
	return(TRUE);
}

void ApologizeOverrideAndForceUpdateEverything()
{
	INT32 x, y;
	CHAR16 str[ 128 ];
	CHAR8 name[50];
	SUMMARYFILE *pSF;
	//Create one huge assed button
	gfMajorUpdate = TRUE;
	iSummaryButton[ SUMMARY_BACKGROUND ] = 
		CreateTextButton( 0, 0, 0, 0, BUTTON_USE_DEFAULT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1, 
		BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK );
	SpecifyDisabledButtonStyle( iSummaryButton[ SUMMARY_BACKGROUND ], DISABLED_STYLE_NONE );
	DisableButton( iSummaryButton[ SUMMARY_BACKGROUND ] );
	//Draw it
	DrawButton( iSummaryButton[ SUMMARY_BACKGROUND ] );
	InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	SetFont( HUGEFONT );
	SetFontForeground( FONT_RED );
	SetFontShadow( FONT_NEARBLACK );
	swprintf( str, pApologizeOverrideAndForceUpdateEverythingText[0] );
	mprintf( (iScreenWidthOffset + 320) - StringPixLength( str, HUGEFONT )/2, iScreenHeightOffset + 105, str );
	SetFont( FONT10ARIAL );
	SetFontForeground( FONT_YELLOW );
	swprintf( str, pApologizeOverrideAndForceUpdateEverythingText[1], gusNumberOfMapsToBeForceUpdated );
	mprintf( (iScreenWidthOffset + 320) - StringPixLength( str, FONT10ARIAL )/2, iScreenHeightOffset + 130, str );

	CreateProgressBar( 2, iScreenWidthOffset + 120, iScreenHeightOffset + 170, iScreenWidthOffset + 520, iScreenHeightOffset + 202 ); 
	DefineProgressBarPanel( 2, 65, 79, 94, iScreenWidthOffset + 100, iScreenHeightOffset + 150, iScreenWidthOffset + 540, iScreenHeightOffset + 222 );
	SetProgressBarTitle( 2, pApologizeOverrideAndForceUpdateEverythingText[2], BLOCKFONT2, FONT_RED, 0 );
	SetProgressBarMsgAttributes( 2, SMALLCOMPFONT, FONT_BLACK, FONT_BLACK );

	gusCurrent = 0;
	gusTotal = gusNumberOfMapsToBeForceUpdated;
	gfUpdatingNow = TRUE;
	UpdateMasterProgress();

	for( y = 0; y < 16; y++ ) for( x = 0; x < 16; x++ )
	{
		sprintf( name, "%c%d", y + 'A', x + 1 );
		if( gbSectorLevels[x][y] & GROUND_LEVEL_MASK )
		{
			pSF = gpSectorSummary[x][y][0];
			if( !pSF || pSF->ubSummaryVersion < MINIMUMVERSION || pSF->dMajorMapVersion < gdMajorMapVersion )
			{
				gpCurrentSectorSummary = pSF;
				if( !EvaluateWorld( name, 0 ) )
					return;
			}
		}
		if( gbSectorLevels[x][y] & BASEMENT1_LEVEL_MASK )
		{
			pSF = gpSectorSummary[x][y][1];
			if( !pSF || pSF->ubSummaryVersion < MINIMUMVERSION || pSF->dMajorMapVersion < gdMajorMapVersion )
			{
				gpCurrentSectorSummary = pSF;
				if( !EvaluateWorld( name, 1 ) )
					return;
			}
		}
		if( gbSectorLevels[x][y] & BASEMENT2_LEVEL_MASK )
		{
			pSF = gpSectorSummary[x][y][2];
			if( !pSF || pSF->ubSummaryVersion < MINIMUMVERSION || pSF->dMajorMapVersion < gdMajorMapVersion )
			{
				gpCurrentSectorSummary = pSF;
				if( !EvaluateWorld( name, 2 ) )
					return;
			}
		}
		if( gbSectorLevels[x][y] & BASEMENT3_LEVEL_MASK )
		{
			pSF = gpSectorSummary[x][y][3];
			if( !pSF || pSF->ubSummaryVersion < MINIMUMVERSION || pSF->dMajorMapVersion < gdMajorMapVersion )
			{
				gpCurrentSectorSummary = pSF;
				if( !EvaluateWorld( name, 3 ) )
					return;
			}
		}
		if( gbSectorLevels[x][y] & ALTERNATE_GROUND_MASK )
		{
			pSF = gpSectorSummary[x][y][4];
			if( !pSF || pSF->ubSummaryVersion < MINIMUMVERSION || pSF->dMajorMapVersion < gdMajorMapVersion )
			{
				gpCurrentSectorSummary = pSF;
				if( !EvaluateWorld( name, 4 ) )
					return;
			}
		}
		if( gbSectorLevels[x][y] & ALTERNATE_B1_MASK )
		{
			pSF = gpSectorSummary[x][y][5];
			if( !pSF || pSF->ubSummaryVersion < MINIMUMVERSION || pSF->dMajorMapVersion < gdMajorMapVersion )
			{
				gpCurrentSectorSummary = pSF;
				if( !EvaluateWorld( name, 5 ) )
					return;
			}
		}
		if( gbSectorLevels[x][y] & ALTERNATE_B2_MASK )
		{
			pSF = gpSectorSummary[x][y][6];
			if( !pSF || pSF->ubSummaryVersion < MINIMUMVERSION || pSF->dMajorMapVersion < gdMajorMapVersion )
			{
				gpCurrentSectorSummary = pSF;
				if( !EvaluateWorld( name, 6 ) )
					return;
			}
		}
		if( gbSectorLevels[x][y] & ALTERNATE_B3_MASK )
		{
			pSF = gpSectorSummary[x][y][7];
			if( !pSF || pSF->ubSummaryVersion < MINIMUMVERSION || pSF->dMajorMapVersion < gdMajorMapVersion )
			{
				gpCurrentSectorSummary = pSF;
				if( !EvaluateWorld( name, 7 ) )
					return;
			}
		}
	}

	// ADB: The call to EvaluateWorld does not use the .dat extension
	EvaluateWorld( "A9", 0 );

	RemoveProgressBar( 2 );
	gfUpdatingNow = FALSE;
	InvalidateScreen();

	RemoveButton( iSummaryButton[ SUMMARY_BACKGROUND ] );
	gfMajorUpdate = FALSE;
	gfMustForceUpdateAllMaps = FALSE;
	gusNumberOfMapsToBeForceUpdated = 0;
}

#if 0//dnl ch81 041213 this function is screwed up so decide to rewrite it
//CHRISL: ADB changed the way this load file is handled
extern int gEnemyPreservedTempFileVersion[256];
extern int gCivPreservedTempFileVersion[256];
void SetupItemDetailsMode( BOOLEAN fAllowRecursion )
{
	HWFILE hfile;
	UINT32 uiNumBytesRead;
	UINT32 uiNumItems;
	CHAR8 szFilename[40];
	BASIC_SOLDIERCREATE_STRUCT basic;
	SOLDIERCREATE_STRUCT priority;
	INT32 i, j;
	UINT16 usNumItems;
	OBJECTTYPE *pItem;
	UINT16 usPEnemyIndex, usNEnemyIndex;

	SUMMARYFILE *s = gpCurrentSectorSummary;
	MAPCREATE_STRUCT *m = &gpCurrentSectorSummary->MapInfo;

	//Clear memory for all the item summaries loaded
	if( gpWorldItemsSummaryArray )
	{
		delete[]( gpWorldItemsSummaryArray );
		gpWorldItemsSummaryArray = NULL;
		gusWorldItemsSummaryArraySize = 0;
	}
	if( gpPEnemyItemsSummaryArray )
	{
		delete[]( gpPEnemyItemsSummaryArray );
		gpPEnemyItemsSummaryArray = NULL;
		gusPEnemyItemsSummaryArraySize = 0;
	}
	if( gpNEnemyItemsSummaryArray )
	{
		delete[]( gpNEnemyItemsSummaryArray );
		gpNEnemyItemsSummaryArray = NULL;
		gusNEnemyItemsSummaryArraySize = 0;
	}

	if( !gpCurrentSectorSummary->uiNumItemsPosition )
	{	//Don't have one, so generate them
		if( gpCurrentSectorSummary->ubSummaryVersion == GLOBAL_SUMMARY_VERSION )
			gusNumEntriesWithOutdatedOrNoSummaryInfo++;
		SummaryUpdateCallback( ButtonList[ iSummaryButton[ SUMMARY_UPDATE ] ], MSYS_CALLBACK_REASON_LBUTTON_UP );
	}
	//Open the original map for the sector
	sprintf( szFilename, "MAPS\\%S", gszFilename );
	hfile = FileOpen( szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( !hfile )
	{ //The file couldn't be found!
		return;
	}
	// ADB:  The uiNumItemsPosition may be 0 here due to the recursion further down.
	// If so, skip the read
	uiNumItems = 0;
	if (gpCurrentSectorSummary->uiNumItemsPosition)
	{
		//Now fileseek directly to the file position where the number of world items are stored
		if( !FileSeek( hfile, gpCurrentSectorSummary->uiNumItemsPosition, FILE_SEEK_FROM_START ) )
		{ //Position couldn't be found!
			FileClose( hfile );
			return;
		}
		//Now load the number of world items from the map.
		FileRead( hfile, &uiNumItems, 4, &uiNumBytesRead );
		if( uiNumBytesRead != 4 )
		{ //Invalid situation.
			FileClose( hfile );
			return;
		}
	}

	//Now compare this number with the number the summary thinks we should have.  If they are different,
	//then the summary doesn't match the map.  What we will do is force regenerate the map so that they do
	//match
	if( uiNumItems != gpCurrentSectorSummary->usNumItems && fAllowRecursion )
	{
		FileClose( hfile );
		gpCurrentSectorSummary->uiNumItemsPosition = 0;
		SetupItemDetailsMode( FALSE );
		return;
	}
	//Passed the gauntlet, so now allocate memory for it, and load all the world items into this array.
	ShowButton( iSummaryButton[ SUMMARY_SCIFI ] );
	ShowButton( iSummaryButton[ SUMMARY_REAL ] );
	ShowButton( iSummaryButton[ SUMMARY_ENEMY ] );
	gpWorldItemsSummaryArray = new WORLDITEM[	uiNumItems ];
	gusWorldItemsSummaryArraySize = gpCurrentSectorSummary->usNumItems;
	for (unsigned int x = 0; x < uiNumItems; ++x)
	{
		gpWorldItemsSummaryArray[x].Load(hfile, s->dMajorMapVersion, m->ubMapVersion);
	}

	//NOW, do the enemy's items!
	//We need to do two passes.  The first pass simply processes all the enemies and counts all the droppable items
	//keeping track of two different values.  The first value is the number of droppable items that come off of
	//enemy detailed placements, the other counter keeps track of the number of droppable items that come off of 
	//normal enemy placements.  After doing this, the memory is allocated for the tables that will store all the item
	//summary information, then the second pass will repeat the process, except it will record the actual items.

	//PASS #1
	if( !FileSeek( hfile, gpCurrentSectorSummary->uiEnemyPlacementPosition, FILE_SEEK_FROM_START ) )
	{ //Position couldn't be found!
		FileClose( hfile );
		return;
	}
	for( i = 0; i < gpCurrentSectorSummary->MapInfo.ubNumIndividuals ; i++ )
	{
		FileRead( hfile, &basic, sizeof( BASIC_SOLDIERCREATE_STRUCT ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( BASIC_SOLDIERCREATE_STRUCT ) )
		{ //Invalid situation.
			FileClose( hfile );
			return;
		}
		if( basic.fDetailedPlacement )
		{ //skip static priority placement 
			if ( !priority.Load(hfile, SAVE_GAME_VERSION, false) )
			{ //Invalid situation.
				FileClose( hfile );
				return;
			}
		}
		else
		{ //non detailed placements don't have items, so skip
			continue;
		}
		if( basic.bTeam == ENEMY_TEAM )
		{
			//Count the items that this enemy placement drops
			usNumItems = 0;
			for( j = 0; j < 9; j++ )
			{
				pItem = &priority.Inv[ gbMercSlotTypes[ j ] ];
				if( pItem->exists() == true && !( (*pItem).fFlags & OBJECT_UNDROPPABLE ) )
				{
					usNumItems++;
				}
			}
			if( basic.fPriorityExistance )
			{
				gusPEnemyItemsSummaryArraySize += usNumItems;
			}
			else
			{
				gusNEnemyItemsSummaryArraySize += usNumItems;
			}
		}
	}

	//Pass 1 completed, so now allocate enough space to hold all the items
	if( gusPEnemyItemsSummaryArraySize )
	{
		gpPEnemyItemsSummaryArray = new OBJECTTYPE[ gusPEnemyItemsSummaryArraySize ];
	}
	if( gusNEnemyItemsSummaryArraySize )
	{
		gpNEnemyItemsSummaryArray = new OBJECTTYPE[ gusNEnemyItemsSummaryArraySize ];
	}

	//PASS #2
	//During this pass, simply copy all the data instead of counting it, now that we have already done so.
	usPEnemyIndex = usNEnemyIndex = 0;
	if( !FileSeek( hfile, gpCurrentSectorSummary->uiEnemyPlacementPosition, FILE_SEEK_FROM_START ) )
	{ //Position couldn't be found!
		FileClose( hfile );
		return;
	}
	for( i = 0; i < gpCurrentSectorSummary->MapInfo.ubNumIndividuals ; i++ )
	{
		FileRead( hfile, &basic, sizeof( BASIC_SOLDIERCREATE_STRUCT ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( BASIC_SOLDIERCREATE_STRUCT ) )
		{ //Invalid situation.
			FileClose( hfile );
			return;
		}
		if( basic.fDetailedPlacement )
		{ //skip static priority placement 
			if ( !priority.Load(hfile, SAVE_GAME_VERSION, false) )
			{ //Invalid situation.
				FileClose( hfile );
				return;
			}
		}
		else
		{ //non detailed placements don't have items, so skip
			continue;
		}
		if( basic.bTeam == ENEMY_TEAM )
		{
			//Copy the items that this enemy placement drops
			usNumItems = 0;
			for( j = 0; j < 9; j++ )
			{
				pItem = &priority.Inv[ gbMercSlotTypes[ j ] ];
				if( pItem->exists() == true && !( (*pItem).fFlags & OBJECT_UNDROPPABLE ) )
				{
					if( basic.fPriorityExistance )
					{
						gpPEnemyItemsSummaryArray[ usPEnemyIndex ] = *pItem;
						usPEnemyIndex++;
					}
					else
					{
						gpNEnemyItemsSummaryArray[ usNEnemyIndex ] = *pItem;
						usNEnemyIndex++;
					}
				}
			}
		}
	}
	FileClose( hfile );
}
#else
void SetupItemDetailsMode(BOOLEAN fAllowRecursion)
{
	UINT32 uiNumItems, uiFileSize, uiBytesRead;
	INT32 i, j;
	UINT16 usNumItems, usPEnemyIndex, usNEnemyIndex;
	UINT8 ubMinorMapVersion;
	INT8 *pBuffer, *pBufferHead;
	CHAR8 szFilename[40];
	HWFILE hfile;
	FLOAT dMajorMapVersion;
	OBJECTTYPE *pItem;
	BASIC_SOLDIERCREATE_STRUCT basic;
	SOLDIERCREATE_STRUCT priority;

	//Clear memory for all the item summaries loaded
	if(gpWorldItemsSummaryArray)
	{
		delete[] gpWorldItemsSummaryArray;
		gpWorldItemsSummaryArray = NULL;
		gusWorldItemsSummaryArraySize = 0;
	}
	if(gpPEnemyItemsSummaryArray)
	{
		delete[] gpPEnemyItemsSummaryArray;
		gpPEnemyItemsSummaryArray = NULL;
		gusPEnemyItemsSummaryArraySize = 0;
	}
	if(gpNEnemyItemsSummaryArray)
	{
		delete[] gpNEnemyItemsSummaryArray;
		gpNEnemyItemsSummaryArray = NULL;
		gusNEnemyItemsSummaryArraySize = 0;
	}
	// Don't have one, so generate them
	if(!gpCurrentSectorSummary->uiNumItemsPosition)
	{
		if(gpCurrentSectorSummary->ubSummaryVersion == GLOBAL_SUMMARY_VERSION)
			gusNumEntriesWithOutdatedOrNoSummaryInfo++;
		SummaryUpdateCallback(ButtonList[iSummaryButton[SUMMARY_UPDATE]], MSYS_CALLBACK_REASON_LBUTTON_UP);
		if(!gpCurrentSectorSummary->uiNumItemsPosition)
			return;
	}
	//Open the original map for the sector
	sprintf(szFilename, "MAPS\\%S", gszFilename);
	hfile = FileOpen(szFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE);
	if(!hfile)// The file couldn't be found!
		return;
	uiFileSize = FileGetSize(hfile);
	if(!uiFileSize)
		return;
	pBuffer = (INT8*)MemAlloc(uiFileSize);
	pBufferHead = pBuffer;
	FileRead(hfile, pBuffer, uiFileSize, &uiBytesRead);
	FileClose(hfile);
	dMajorMapVersion = *(FLOAT *)pBuffer;
	if(dMajorMapVersion >= 4.00)
		ubMinorMapVersion = pBuffer[sizeof(FLOAT)];
	else
		ubMinorMapVersion = 0;
	uiNumItems = 0;
	if(gpCurrentSectorSummary->uiNumItemsPosition)
	{
		// Now fileseek directly to the file position where the number of world items are stored
		SKIPDATA(pBuffer, pBufferHead, gpCurrentSectorSummary->uiNumItemsPosition);
		if((UINT32)(pBuffer-pBufferHead) > uiFileSize)
		{
L01:
			if(fAllowRecursion)
			{
				gpCurrentSectorSummary->uiNumItemsPosition = 0;
				SetupItemDetailsMode(FALSE);
			}
			return;
		}
		// Now load the number of world items from the map
		LOADDATA(&uiNumItems, pBuffer, sizeof(UINT32));
		if((UINT32)(pBuffer-pBufferHead) > uiFileSize)
			goto L01;
	}
	// Now compare this number with the number the summary thinks we should have.  If they are different, then the summary doesn't match the map.  What we will do is force regenerate the map so that they do match
	if(uiNumItems != gpCurrentSectorSummary->usNumItems)
		goto L01;
	//Passed the gauntlet, so now allocate memory for it, and load all the world items into this array.
	ShowButton(iSummaryButton[SUMMARY_SCIFI]);
	ShowButton(iSummaryButton[SUMMARY_REAL]);
	ShowButton(iSummaryButton[SUMMARY_ENEMY]);
	gpWorldItemsSummaryArray = new WORLDITEM[uiNumItems];
	gusWorldItemsSummaryArraySize = gpCurrentSectorSummary->usNumItems;
	for(i=0; i<(INT32)uiNumItems; i++)
		gpWorldItemsSummaryArray[i].Load(&pBuffer, dMajorMapVersion, ubMinorMapVersion);
	// Now, do the enemy's items! We need to do two passes. The first pass simply processes all the enemies and counts all the droppable items keeping track of two different values.
	// The first value is the number of droppable items that come off of enemy detailed placements, the other counter keeps track of the number of droppable items that come off of normal enemy placements.
	// After doing this, the memory is allocated for the tables that will store all the item summary information, then the second pass will repeat the process, except it will record the actual items.
	// PASS #1
	SKIPDATA(pBuffer, pBufferHead, gpCurrentSectorSummary->uiEnemyPlacementPosition);
	for(i=0; i<gpCurrentSectorSummary->MapInfo.ubNumIndividuals; i++)
	{
		basic.Load(&pBuffer, dMajorMapVersion);
		if((UINT32)(pBuffer-pBufferHead) > uiFileSize)
			goto L01;
		if(basic.fDetailedPlacement)// skip static priority placement
		{
			priority.Load(&pBuffer, dMajorMapVersion, ubMinorMapVersion);
			if((UINT32)(pBuffer-pBufferHead) > uiFileSize)
				goto L01;
		}
		else//non detailed placements don't have items, so skip
			continue;
		if(basic.bTeam == ENEMY_TEAM)
		{
			//Count the items that this enemy placement drops
			usNumItems = 0;
			for(j=0; j<9; j++)
			{
				pItem = &priority.Inv[gbMercSlotTypes[j]];
				if(pItem->exists() == true && !((*pItem).fFlags & OBJECT_UNDROPPABLE))
					usNumItems++;
			}
			if(basic.fPriorityExistance)
				gusPEnemyItemsSummaryArraySize += usNumItems;
			else
				gusNEnemyItemsSummaryArraySize += usNumItems;
		}
	}
	// Pass 1 completed, so now allocate enough space to hold all the items
	if(gusPEnemyItemsSummaryArraySize)
		gpPEnemyItemsSummaryArray = new OBJECTTYPE[gusPEnemyItemsSummaryArraySize];
	if(gusNEnemyItemsSummaryArraySize)
		gpNEnemyItemsSummaryArray = new OBJECTTYPE[ gusNEnemyItemsSummaryArraySize ];
	// PASS #2
	// During this pass, simply copy all the data instead of counting it, now that we have already done so
	usPEnemyIndex = usNEnemyIndex = 0;
	SKIPDATA(pBuffer, pBufferHead, gpCurrentSectorSummary->uiEnemyPlacementPosition);
	for(i=0; i<gpCurrentSectorSummary->MapInfo.ubNumIndividuals; i++)
	{
		basic.Load(&pBuffer, dMajorMapVersion);
		if(basic.fDetailedPlacement)// skip static priority placement
			priority.Load(&pBuffer, dMajorMapVersion, ubMinorMapVersion);
		else// non detailed placements don't have items, so skip
			continue;
		if(basic.bTeam == ENEMY_TEAM)
		{
			// Copy the items that this enemy placement drops
			usNumItems = 0;
			for(j=0; j<9; j++)
			{
				pItem = &priority.Inv[gbMercSlotTypes[j]];
				if(pItem->exists() == true && !((*pItem).fFlags & OBJECT_UNDROPPABLE))
				{
					if(basic.fPriorityExistance)
					{
						gpPEnemyItemsSummaryArray[usPEnemyIndex] = *pItem;
						usPEnemyIndex++;
					}
					else
					{
						gpNEnemyItemsSummaryArray[usNEnemyIndex] = *pItem;
						usNEnemyIndex++;
					}
				}
			}
		}
	}
}
#endif

UINT8 GetCurrentSummaryVersion()
{
	if( gpCurrentSectorSummary )
	{
		return gpCurrentSectorSummary->MapInfo.ubMapVersion;
	}
	return 0;
}

void ClearSummaryInfo()
{
	for (int x = 0; x < 16; x++)
	{
		for (int y=0; y < 16; y++)
		{
			for (int z=0; z < 8; z++)
			{
				if (gpSectorSummary[x][y][z])
				{
					MemFree( gpSectorSummary[x][y][z]);
					gpSectorSummary[x][y][z] = NULL;
				}
			}
		}
	}
}

#endif




