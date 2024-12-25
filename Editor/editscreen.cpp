#include "builddefines.h"

#ifdef JA2EDITOR

	#include "sgp.h"
	#include "vobject.h"
	#include "worlddef.h"
	#include "renderworld.h"
	#include "input.h"
	#include "smooth.h"
	#include "worldman.h"
	#include "edit_sys.h"
	#include "screenids.h"
	#include "editscreen.h"
	#include "sys globals.h"
	#include "SmartMethod.h"
	#include "selectwin.h"
	#include "Interface.h"
	#include "lighting.h"
	#include "Interactive Tiles.h"
	#include "overhead types.h"
	#include "Overhead.h"
	#include "Handle UI.h"
	#include "Event Pump.h"
	#include "world items.h"
	#include "loadscreen.h"
	#include "render dirty.h"
	#include "isometric utils.h"
	#include "message.h"
	#include "render fun.h"
	#include "popupmenu.h"
	#include "overhead map.h"
	#include "EditorDefines.h"
	#include "EditorTerrain.h"
	#include "EditorBuildings.h"
	#include "EditorItems.h"
	#include "EditorMercs.h"
	#include "EditorMapInfo.h"
	#include "newsmooth.h"
	#include "Smoothing Utils.h"
	#include "messagebox.h"
	#include "messageboxscreen.h"
	#include "Soldier Create.h"
	#include "Soldier Init List.h"
	#include "Text Input.h"
	#include "Cursor Modes.h"
	#include "Editor Taskbar Utils.h"
	#include "Editor Modes.h"
	#include "Editor Undo.h"
	#include "Exit Grids.h"
	#include "Item Statistics.h"
	#include "Map Information.h"
	#include "Sector Summary.h"
	#include "Game Clock.h"
	#include "Game Init.h"
	#include "environment.h"
	#include "Simple Render Utils.h"
	#include "Map Edgepoints.h"
	#include "line.h"
	#include "english.h"
	#include "random.h"
	#include "scheduling.h"
	#include "Road Smoothing.h"
	#include "strategicmap.h"
	#include "message.h"
	#include "pits.h"
	#include "Inventory Choosing.h"
	#include "Music Control.h"
	#include "Soldier Profile.h"
	#include "GameSettings.h"
	#include "Summary Info.h"
	#include "connect.h"//dnl
	#include "cursors.h"//dnl ch2 210909
	#include "Cursor Control.h"//dnl ch2 210909
	#include "maputility.h"//dnl ch49 061009
	#include "Text.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

BOOLEAN gfCorruptMap = FALSE;
BOOLEAN gfCorruptSchedules = FALSE;
BOOLEAN gfProfileDataLoaded = FALSE;

extern void ToggleMapEdgepoints();//dnl ch23 210909

extern void RemoveMercsInSector();
extern void ReverseSchedules();
extern void ClearAllSchedules();

extern UINT32 guiCurUICursor;

void DrawObjectsBasedOnSelectionRegion();
UINT32 ProcessEditscreenMessageBoxResponse();

void ReloadMap();

//defined in sector summary.c
extern BOOLEAN HandleSummaryInput( InputAtom *pEvent );

GUI_BUTTON *gpPersistantButton;

// These are global variables used by the main game loop

UINT32 guiSaveTacticalStatusFlags;	//saves the tactical status flags when entering the editor.

BOOLEAN			gfAutoLoadA9 = FALSE;
//new vars added by Kris
BOOLEAN			gfRenderWorld = FALSE;
BOOLEAN			gfRenderTaskbar = FALSE;
BOOLEAN			gfRenderDrawingMode = FALSE;
BOOLEAN			gfFirstPlacement = TRUE;
BOOLEAN			gfPendingBasement = FALSE;
BOOLEAN			gfPendingCaves = FALSE;
BOOLEAN			gfNeedToInitGame = FALSE;
BOOLEAN			gfScheduleReversalPending = FALSE;
BOOLEAN			gfRemoveLightsPending = FALSE;
BOOLEAN			gfScheduleClearPending = FALSE;
BOOLEAN			gfConfirmExitFirst = TRUE;
BOOLEAN			gfConfirmExitPending = FALSE;
BOOLEAN			gfIntendOnEnteringEditor = FALSE;

//original
INT16				gsBanksSubIndex = 0;
INT16				gsOldBanksSubIndex = 1;
INT16				gsCliffsSubIndex = 0;
INT16				gsOldCliffsSubIndex = 1;
BOOLEAN			gfSwitchScreen = FALSE;
BOOLEAN			gDoTest = FALSE;
BOOLEAN			gDoTest2 = FALSE;
FLOAT				gShadePercent = (FLOAT)0.65;
INT16				gusCurrentRoofType = ONELEVELTYPEONEROOF;

UINT16			gusLightLevel = 0;
UINT16			gusGameLightLevel = 0;
UINT16			gusSavedLightLevel = 0;
BOOLEAN			gfFakeLights = FALSE;

INT16				gsLightRadius = 5;

BOOLEAN			gfOldDoVideoScroll;			// Saved for returning to previous settings
UINT8				gubOldCurScrollSpeedID; // Saved for returning to previous settings

INT32 iOldTaskMode = TASK_OPTIONS;

INT32 iTaskMode = TASK_NONE;

INT32 iEditorTBarButton[NUMBER_EDITOR_BUTTONS];	// For Toolbars

BOOLEAN gfMercResetUponEditorEntry;


BOOLEAN fHelpScreen = FALSE;

BOOLEAN fDontUseRandom = TRUE;//dnl ch8 210909
BOOLEAN fDontUseClick = FALSE;//dnl ch7 210909
BOOLEAN fShowHighGround = FALSE;//dnl ch2 210909
BOOLEAN fRaiseWorld = FALSE;//dnl ch3 210909
BOOLEAN gfVanillaMode = FALSE;//dnl ch33 091009 //dnl ch74 191013 by default is better to save all maps in newest format
BOOLEAN gfResizeMapOnLoading = FALSE;

INT32 TestButtons[10];

LEVELNODE *gCursorNode = NULL;
//LEVELNODE *gBasicCursorNode = NULL;
INT32			gsCursorGridNo;

INT32 giMusicID = 0;
NewMusicList gMusicMode = MUSICLIST_MAIN_MENU;

void EraseWorldData(	);

BOOLEAN EditModeInit( void );
BOOLEAN EditModeShutdown( void );
void EnsureStatusOfEditorButtons();

extern INT8 gbEditingMode;

extern BOOLEAN fAllDone;
extern DisplayList Selection;

extern void DisableButtonHelpTextRestore();
extern void EnableButtonHelpTextRestore();

void UpdateLastActionBeforeLeaving();

BOOLEAN				gfEditorDirty = TRUE;

BOOLEAN fRaiseHeight = FALSE;

INT32 iDrawMode = DRAW_MODE_NOTHING;
INT32 iCurrentAction,iActionParam;
INT32 iEditAction = ACTION_NULL;

INT32 iEditorButton[NUMBER_EDITOR_BUTTONS];
INT32 iEditorToolbarState;
INT32 iJA2ToolbarLastWallState;

INT32 iCurrentTaskbar;

UINT32 iCurBankMapIndex;

InputAtom EditorInputEvent;
BOOLEAN fBeenWarned = FALSE;
BOOLEAN fEditModeFirstTime = TRUE;
BOOLEAN fFirstTimeInEditModeInit = TRUE;
BOOLEAN fSelectionWindow = FALSE;
BOOLEAN gfRealGunNut = TRUE;

INT16 sGridX, sGridY;
INT32 iMapIndex;
BOOLEAN fNewMap = FALSE;
BOOLEAN fNewMapSaved = TRUE;

INT32 iPrevDrawMode = DRAW_MODE_NOTHING;
UINT16 PrevCurrentPaste = FIRSTTEXTURE;
INT32 gPrevCurrentBackground = FIRSTTEXTURE;
INT32 iPrevJA2ToolbarState = TBAR_MODE_NONE;
INT32 PrevTerrainTileDrawMode = TERRAIN_TILES_NODRAW;

UINT16 gusEditorTaskbarColor;
UINT16 gusEditorTaskbarHiColor;
UINT16 gusEditorTaskbarLoColor;

void CreateGotoGridNoUI();
void RemoveGotoGridNoUI();
BOOLEAN gfGotoGridNoUI = FALSE;
INT32 guiGotoGridNoUIButtonID;
MOUSE_REGION GotoGridNoUIRegion;

//Buggler: Create item with keyboard shortcut
void EditorKeyboardItemCreationCallBack( UINT8 ubResult );
void CreateKeyboardItemCreationUI();
void RemoveKeyboardItemCreationUI();
BOOLEAN gfKeyboardItemCreationUI = FALSE;
INT32 guiKeyboardItemCreationUIButtonID;
MOUSE_REGION KeyboardItemCreationUIRegion;
INT32 giCreateItemCursorMapIndex;
BOOLEAN fEditorCreateItemFromKeyboard = FALSE;
UINT16 usEditorTempItem = 1632;

//----------------------------------------------------------------------------------------------
//	EditScreenInit
//
//	This function is called once at SGP (and game) startup
//
UINT32 EditScreenInit(void)
{
	gfFakeLights = FALSE;

	eInfo.fGameInit = TRUE;
	GameInitEditorBuildingInfo();
	GameInitEditorMercsInfo();

	//Set the editor colors.
	//gusEditorTaskbarColor = 9581;
	//gusEditorTaskbarColor =		Get16BPPColor( FROMRGB(	72,	88, 104 ) );
	//gusEditorTaskbarHiColor = Get16BPPColor( FROMRGB( 136, 138, 135 ) );
	//gusEditorTaskbarLoColor = Get16BPPColor( FROMRGB(	24,	61,	81 ) );

	gusEditorTaskbarColor	= Get16BPPColor( FROMRGB(	65,	79,	94 ) );
	gusEditorTaskbarHiColor = Get16BPPColor( FROMRGB( 122, 124, 121 ) );
	gusEditorTaskbarLoColor = Get16BPPColor( FROMRGB(	22,	55,	73 ) );

	InitClipboard();

	InitializeRoadMacros();

	InitArmyGunTypes();

	return TRUE;
}


//----------------------------------------------------------------------------------------------
//	EditScreenShutdown
//
//	This function is called once at shutdown of the game
//
UINT32 EditScreenShutdown(void)
{
	GameShutdownEditorMercsInfo();
	RemoveAllFromUndoList();
	KillClipboard();
	if( gfWorldLoaded )
	{
		KillItemPoolList();
	}
	return TRUE;
}


//----------------------------------------------------------------------------------------------
//	EditModeInit
//
//	Editor's Init code. Called each time we enter edit mode from the game.
//
BOOLEAN EditModeInit( void )
{
	UINT32 x;
	INT32 i;
	SGPPaletteEntry	LColors[2];

	OutputDebugString( "Entering editor mode...\n" );

	//dnl after multiplayer stuff was implemeted, editor goes to assertion when you leave editor mode, because InitGameOption() is not called anymore from InitializeGame()
	is_networked = FALSE;
	//InitGameOptions();
	GameInitOptionsScreenInit();

	ResetCustomFileSectorSummary();//dnl ch30 150909

	gfRealGunNut = gGameOptions.fGunNut;
	gGameOptions.fGunNut = TRUE;


	if( !gfProfileDataLoaded )
		LoadMercProfiles();

	ClearTacticalMessageQueue( );

	PauseGame();
	fEditModeFirstTime = FALSE;

	DisableButtonHelpTextRestore();

	if( fFirstTimeInEditModeInit )
	{
		if( gfWorldLoaded )
			InitJA2SelectionWindow();
		fFirstTimeInEditModeInit = FALSE;
	}

	// Clear the summary info array
	ClearSummaryInfo();

	//Initialize editor specific stuff for each Taskbar.
	EntryInitEditorTerrainInfo();
	EntryInitEditorItemsInfo();
	EntryInitEditorMercsInfo();

	LightGetColors( LColors );
	gEditorLightColor = LColors[0];

	//essentially, we are turning the game off so the game doesn't process in conjunction with the
	//editor.
	guiSaveTacticalStatusFlags = (gTacticalStatus.uiFlags & ~DEMOMODE);
	gTacticalStatus.uiFlags &= ~REALTIME;
	gTacticalStatus.uiFlags |= TURNBASED | SHOW_ALL_ITEMS;
	gTacticalStatus.uiTimeOfLastInput = GetJA2Clock();
	gTacticalStatus.uiTimeSinceDemoOn = gTacticalStatus.uiTimeOfLastInput;
	gTacticalStatus.fGoingToEnterDemo = FALSE;

	//Remove the radar from the interface.
	RemoveCurrentTacticalPanelButtons( );
	MSYS_DisableRegion(&gRadarRegion);

	CreateEditorTaskbar();

	//Hide all of the buttons here.	DoTaskbar() will handle the
	//showing and hiding of the buttons.
	for( x = LAST_EDITORTAB_BUTTON+1; x < NUMBER_EDITOR_BUTTONS; x++ )
		HideButton( iEditorButton[x] );

	iEditorToolbarState = iPrevJA2ToolbarState;
	iDrawMode = iPrevDrawMode;
	CurrentPaste = PrevCurrentPaste;
	gCurrentBackground = gPrevCurrentBackground;

	iCurrentTaskbar = TASK_NONE;
	iTaskMode = iOldTaskMode;

	DoTaskbar();

//	fDontUseRandom = FALSE;//dnl ch8 210909
//	fDontUseClick = FALSE;//dnl ch7 210909

	fSelectionWindow = FALSE;

	// Set renderer to ignore redundency
	gTacticalStatus.uiFlags |= NOHIDE_REDUNDENCY;

	fHelpScreen = FALSE;

	gfEditMode = TRUE;
	fNewMap = FALSE;
	fNewMapSaved = TRUE;

	gfEditingDoor = FALSE;

	gusGameLightLevel = LightGetAmbient();
	if( !gfBasement && !gfCaves )
		gusLightLevel =	12;//EDITOR_LIGHT_MAX - (UINT16)LightGetAmbient();
	else
		gusLightLevel = EDITOR_LIGHT_MAX - (UINT16)LightGetAmbient();

	if( gfFakeLights )
	{
		gusSavedLightLevel = gusLightLevel;
		gusLightLevel = EDITOR_LIGHT_FAKE;
		ClickEditorButton( MAPINFO_TOGGLE_FAKE_LIGHTS );
	}

	gfRenderWorld = TRUE;
	gfRenderTaskbar = TRUE;

	//Reset the corruption detection flags.
	gfCorruptMap = FALSE;
	gfCorruptSchedules = FALSE;

	// save old scrolling mode, set renderer to not use video scroll
	gfOldDoVideoScroll = gfDoVideoScroll;
	gubOldCurScrollSpeedID = gubCurScrollSpeedID;
	/// set new ones

	gfRoofPlacement = FALSE;

	EnableUndo();

	RemoveMercsInSector( );
	if( gfWorldLoaded )
	{
		gfConfirmExitFirst = TRUE;
		ShowEntryPoints();
		PrepareSchedulesForEditorEntry();
		if( gfMercResetUponEditorEntry )
		{
			UseEditorOriginalList();
			ResetAllMercPositions();
		}
		else
			UseEditorAlternateList();
		BuildItemPoolList();
		SetEditorSmoothingMode( gMapInformation.ubEditorSmoothingType );
		AddLockedDoorCursors();

		for( i = 0; i < MAX_LIGHT_SPRITES; i++ )
		{
			if( LightSprites[ i ].uiFlags & LIGHT_SPR_ACTIVE && !(LightSprites[ i ].uiFlags & (LIGHT_SPR_ON|MERC_LIGHT) ) )
			{
				LightSpritePower( i, TRUE );
			}
		}

		if( gfShowPits )
		{
			AddAllPits();
		}

		LightSetBaseLevel( (UINT8)(EDITOR_LIGHT_MAX - gusLightLevel) );
		ShowLightPositionHandles();
		LightSpriteRenderAll();
	}
	else
	{
		OutputDebugString( "Creating summary window...\n" );
		CreateSummaryWindow();
		gfNeedToInitGame = TRUE;
	}

	SetMercTeamVisibility( ENEMY_TEAM, gfShowEnemies );
	SetMercTeamVisibility( CREATURE_TEAM, gfShowCreatures );
	SetMercTeamVisibility( MILITIA_TEAM, gfShowRebels );
	SetMercTeamVisibility( CIV_TEAM, gfShowCivilians );

	// remove mouse region for pause of game
	RemoveMouseRegionForPauseOfClock( );

	gfIntendOnEnteringEditor = FALSE;

	OutputDebugString( "Finished entering editor mode...\n" );

	return(TRUE);
}


//----------------------------------------------------------------------------------------------
//	EditModeShutdown
//
//	The above function's counterpart. Called when exiting the editor back to the game.
BOOLEAN EditModeShutdown( void )
{
	if( gfConfirmExitFirst )
	{
		gfConfirmExitPending = TRUE;
		CreateMessageBox( pEditModeShutdownText[0] );
		return FALSE;
	}

	gfEditMode = FALSE;
	fEditModeFirstTime = TRUE;

	UpdateLastActionBeforeLeaving();

	DeleteEditorTaskbar();

	// create clock mouse region for clock pause
//	CreateMouseRegionForPauseOfClock( CLOCK_REGION_START_X, CLOCK_REGION_START_Y );
	CreateMouseRegionForPauseOfClock( INTERFACE_CLOCK_X, INTERFACE_CLOCK_Y );

	iOldTaskMode = iCurrentTaskbar;
	gTacticalStatus.uiFlags = guiSaveTacticalStatusFlags;

	RemoveLightPositionHandles( );

	RemoveCursors();

	fHelpScreen = FALSE;
	// Set render back to normal mode
	gTacticalStatus.uiFlags &= ~(NOHIDE_REDUNDENCY | SHOW_ALL_ITEMS);
	UpdateRoofsView( );
	UpdateWallsView( );
	// Invalidate all redundency checks
	InvalidateWorldRedundency();


	// If false lighting is on in the editor, turn it off!
	if ( gfFakeLights )
	{
		gusLightLevel = gusSavedLightLevel;
	}

	LightSpriteRenderAll();

	if( gfWorldLoaded )
	{
		ClearRenderFlags( RENDER_FLAG_SAVEOFF );
		MarkWorldDirty();
		RenderWorld( );
	}

	InvalidateScreen( );
	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender( );

	MSYS_EnableRegion(&gRadarRegion);
	CreateCurrentTacticalPanelButtons( );

	// Make sure to turn off demo mode!
	gTacticalStatus.uiTimeOfLastInput = GetJA2Clock();
	gTacticalStatus.uiTimeSinceDemoOn = GetJA2Clock();
	gTacticalStatus.fGoingToEnterDemo = FALSE;

	// RETURN TO PREVIOUS SCROLL MODE
	gfDoVideoScroll = gfOldDoVideoScroll;
	gubCurScrollSpeedID = gubOldCurScrollSpeedID;


	DisableUndo();

	SetFontShadow( DEFAULT_SHADOW );


	if( gfWorldLoaded )
	{
		CompileWorldMovementCosts();
		RaiseWorldLand();
		CompileInteractiveTiles();
		HideEntryPoints();
		KillItemPoolList();
		PrepareSchedulesForEditorExit();
		RemoveAllObjectsOfTypeRange( gsSelectedMercGridNo, CONFIRMMOVE, CONFIRMMOVE );
		RemoveLockedDoorCursors();
		if( gfShowPits )
		{
			RemoveAllPits();
		}
	}

	EnableButtonHelpTextRestore();

	if( gfNeedToInitGame )
	{
		InitStrategicLayer();
		WarpGameTime( 1 , WARPTIME_PROCESS_EVENTS_NORMALLY ); //to avoid helicopter setup
		gfNeedToInitGame = FALSE;
	}
	else
	{
		if( !gfBasement && !gfCaves )
			LightSetBaseLevel( (UINT8)gusGameLightLevel );
		UpdateMercsInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
	}

	gGameOptions.fGunNut = gfRealGunNut;

	DeleteBuildingLayout();

	HideExitGrids();

	// Clear the summary info array
	ClearSummaryInfo();

	UnPauseGame();

	return TRUE;
}

//----------------------------------------------------------------------------------------------
//	SetBackgroundTexture
//
//	Forces all land on the map to have the same ground texture.
//
void SetBackgroundTexture( )
{
	INT32				cnt;
	UINT16				usIndex, Dummy;

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		// Erase old layers
		RemoveAllLandsOfTypeRange( cnt, FIRSTTEXTURE, DEEPWATERTEXTURE );

		// Add level
		usIndex = (UINT16)(rand( ) % 10 );

		// Adjust for type
		usIndex = usIndex + gTileTypeStartIndex[ gCurrentBackground ];

		// Set land index
		if( TypeRangeExistsInLandLayer( cnt, FIRSTFLOOR, LASTFLOOR, &Dummy ) )
			AddLandToTail( cnt, usIndex ); //show the land below the floor.
		else
			AddLandToHead( cnt, usIndex ); //no floor so no worries.
	}
}


//----------------------------------------------------------------------------------------------
//	DoWindowSelection
//
//	Displays the selection window and handles it's exit condition. Used by WaitForSelectionWindow
//
BOOLEAN DoWindowSelection( void )
{
	RenderSelectionWindow( );
	RenderButtonsFastHelp( );
	if ( fAllDone )
	{
		switch ( iDrawMode )
		{
			case DRAW_MODE_WALLS:
			case DRAW_MODE_DOORS:
			case DRAW_MODE_WINDOWS:
			case DRAW_MODE_ROOFS:
			case DRAW_MODE_BROKEN_WALLS:
			case DRAW_MODE_DECOR:
			case DRAW_MODE_DECALS:
			case DRAW_MODE_FLOORS:
			case DRAW_MODE_TOILET:
			case DRAW_MODE_ROOM:
			case DRAW_MODE_NEWROOF:
			case DRAW_MODE_OSTRUCTS:
			case DRAW_MODE_OSTRUCTS1:
			case DRAW_MODE_OSTRUCTS2:
			case DRAW_MODE_DEBRIS:
			case DRAW_MODE_BANKS:
			case DRAW_MODE_ROADS:
				break;
			default:
				iDrawMode = DRAW_MODE_NOTHING;
				break;
 		}
		RemoveJA2SelectionWindow( );
		return ( TRUE );
	}
	return ( FALSE );
}


//Whenever the mouse attaches an object to the cursor, it has to be removed, so it doesn't stay
//in the world.
void RemoveTempMouseCursorObject( void )
{
	if ( iCurBankMapIndex < 0x80000000 )
	{
		ForceRemoveStructFromTail( iCurBankMapIndex );
		gCursorNode = NULL;
	}

}

//Whenever the editor wishes to show an object in the world, it will temporarily attach it to
//the mouse cursor, to indicate what is about to be drawn.
BOOLEAN DrawTempMouseCursorObject(void)
{
	INT16		sMouseX_M, sMouseY_M;
	UINT16	usUseIndex;
	UINT16	usUseObjIndex;

	switch ( iDrawMode )
	{
		case DRAW_MODE_ROOM:
			pSelList = SelRoom;
			pNumSelList = &iNumRoomsSelected;
			return FALSE;	//a special case where we just want to get the info and not display a cursor.
		case DRAW_MODE_NEWROOF:
			pSelList = SelSingleNewRoof;
			pNumSelList = &iNumNewRoofsSelected;
			break;
		case DRAW_MODE_WALLS:
			pSelList = SelSingleWall;
			pNumSelList = &iNumWallsSelected;
			break;
		case DRAW_MODE_DOORS:
			pSelList = SelSingleDoor;
			pNumSelList = &iNumDoorsSelected;
			break;
		case DRAW_MODE_WINDOWS:
			pSelList = SelSingleWindow;
			pNumSelList = &iNumWindowsSelected;
			break;
		case DRAW_MODE_ROOFS:
			pSelList = SelSingleRoof;
			pNumSelList = &iNumRoofsSelected;
			break;
		case DRAW_MODE_BROKEN_WALLS:
			pSelList = SelSingleBrokenWall;
			pNumSelList = &iNumBrokenWallsSelected;
			break;
		case DRAW_MODE_DECOR:
			pSelList = SelSingleDecor;
			pNumSelList = &iNumDecorSelected;
			break;
		case DRAW_MODE_DECALS:
			pSelList = SelSingleDecal;
			pNumSelList = &iNumDecalsSelected;
			break;
		case DRAW_MODE_FLOORS:
			pSelList = SelSingleFloor;
			pNumSelList = &iNumFloorsSelected;
			break;
		case DRAW_MODE_TOILET:
			pSelList = SelSingleToilet;
			pNumSelList = &iNumToiletsSelected;
			break;
		case DRAW_MODE_BANKS:
			pSelList = SelBanks;
			pNumSelList = &iNumBanksSelected;
			break;
		case DRAW_MODE_ROADS:
			pSelList = SelRoads;
			pNumSelList = &iNumRoadsSelected;
			break;
		case DRAW_MODE_OSTRUCTS:
			pSelList = SelOStructs;
			pNumSelList = &iNumOStructsSelected;
			break;
		case DRAW_MODE_OSTRUCTS1:
			pSelList = SelOStructs1;
			pNumSelList = &iNumOStructs1Selected;
			break;
		case DRAW_MODE_OSTRUCTS2:
			pSelList = SelOStructs2;
			pNumSelList = &iNumOStructs2Selected;
			break;
		case DRAW_MODE_DEBRIS:
			pSelList = SelDebris;
			pNumSelList = &iNumDebrisSelected;
			break;
	}

	if ( GetMouseXY( &sMouseX_M, &sMouseY_M ) )
	{
		if ( (iCurBankMapIndex = MAPROWCOLTOPOS( sMouseY_M, sMouseX_M )) < 0x80000000 )
		{
			if( gfRoofPlacement && FlatRoofAboveGridNo( iCurBankMapIndex ) )
				iCurBankMapIndex += ROOF_OFFSET;

			//Hook into the smart methods to override the selection window methods.
			if( iDrawMode == DRAW_MODE_SMART_WALLS )
			{
				if( !CalcWallInfoUsingSmartMethod( iCurBankMapIndex, &usUseObjIndex, &usUseIndex ) )
				{
					return FALSE;
				}
			}
			else if( iDrawMode == DRAW_MODE_SMART_DOORS )
			{
				if( !CalcDoorInfoUsingSmartMethod( iCurBankMapIndex, &usUseObjIndex, &usUseIndex ) )
				{
					return FALSE;
				}
			}
			else if( iDrawMode == DRAW_MODE_SMART_WINDOWS )
			{
				if( !CalcWindowInfoUsingSmartMethod( iCurBankMapIndex, &usUseObjIndex, &usUseIndex ) )
				{
					return FALSE;
				}
			}
			else if( iDrawMode == DRAW_MODE_SMART_BROKEN_WALLS )
			{
				if( !CalcBrokenWallInfoUsingSmartMethod( iCurBankMapIndex, &usUseObjIndex, &usUseIndex ) )
				{
					return FALSE;
				}
				if( usUseObjIndex == 0xffff ||	usUseIndex == 0xffff )
				{
					return FALSE;
				}
			}
			else
			{
				usUseIndex = pSelList[ iCurBank ].usIndex;
				usUseObjIndex = (UINT16)pSelList[ iCurBank ].uiObject;
			}
			gCursorNode = ForceStructToTail( iCurBankMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) );
			// ATE: Set this levelnode as dynamic!
			gCursorNode->uiFlags |= LEVELNODE_DYNAMIC;
			return( TRUE );
		}
		else
			return( FALSE );
	}

	return( FALSE );
}


//Displays the current drawing object in the small, lower left window of the editor's toolbar.
void ShowCurrentDrawingMode( void )
{
	SGPRect			ClipRect, NewRect;
	INT32				iShowMode;
	UINT16			usUseIndex;
	UINT16			usObjIndex;
	INT32				iStartX = iScreenWidthOffset + 50;
	INT32				iStartY = 2	* iScreenHeightOffset + 440;
	INT32				iPicHeight, iPicWidth;
	INT16				sTempOffsetX;
	INT16				sTempOffsetY;
	ETRLEObject *pETRLEObject;
	UINT32			uiDestPitchBYTES;
	UINT8				*pDestBuf;
	UINT16			usFillColor;
	INT32				iIndexToUse;

	// Set up a clipping rectangle for the display window.
	NewRect.iLeft = iScreenWidthOffset + 0;
	NewRect.iTop = 2 * iScreenHeightOffset + 400;
	NewRect.iRight = iScreenWidthOffset + 100;
	NewRect.iBottom = 2 * iScreenHeightOffset + 458;

	GetClippingRect(&ClipRect);
	SetClippingRect(&NewRect);

	// Clear it out
	ColorFillVideoSurfaceArea( FRAME_BUFFER, iScreenWidthOffset, 2 * iScreenHeightOffset + 400, iScreenWidthOffset + 100, 2 * iScreenHeightOffset + 458, 0 );

	iShowMode = iDrawMode;
	if ( iDrawMode >= DRAW_MODE_ERASE )
		iShowMode -= DRAW_MODE_ERASE;

	usUseIndex = usObjIndex = 0xffff;

	iIndexToUse = 0;
	if ( fDontUseRandom )
		iIndexToUse = iCurBank;

	// Select object to be displayed depending on the current editing mode.
	switch ( iShowMode )
	{
		case DRAW_MODE_ROOM:
		case DRAW_MODE_SLANTED_ROOF:
			if ( iNumWallsSelected > 0 )
			{
				usUseIndex = SelRoom[0].usIndex;
				usObjIndex = (UINT16)SelRoom[0].uiObject;
			}
			break;

		case DRAW_MODE_GROUND:
		case (DRAW_MODE_GROUND + DRAW_MODE_FILL_AREA):
			usUseIndex = 0;
			usObjIndex = CurrentPaste;
			break;

		case DRAW_MODE_OSTRUCTS:
			if ( iNumOStructsSelected > 0 )
			{
				usUseIndex = SelOStructs[ iIndexToUse ].usIndex;
				usObjIndex = (UINT16)SelOStructs[ iIndexToUse ].uiObject;
			}
			break;

		case DRAW_MODE_OSTRUCTS1:
			if ( iNumOStructs1Selected > 0 )
			{
				usUseIndex = SelOStructs1[ iIndexToUse ].usIndex;
				usObjIndex = (UINT16)SelOStructs1[ iIndexToUse ].uiObject;
			}
			break;

		case DRAW_MODE_OSTRUCTS2:
			if ( iNumOStructs2Selected > 0 )
			{
				usUseIndex = SelOStructs2[ iIndexToUse ].usIndex;
				usObjIndex = (UINT16)SelOStructs2[ iIndexToUse ].uiObject;
			}
			break;

		case DRAW_MODE_DEBRIS:
			if ( iNumDebrisSelected > 0 )
			{
				usUseIndex = SelDebris[ iIndexToUse ].usIndex;
				usObjIndex = (UINT16)SelDebris[ iIndexToUse ].uiObject;
			}
			break;

		case DRAW_MODE_BANKS:
			if ( iNumBanksSelected > 0 )
			{
				usUseIndex = SelBanks[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelBanks[ iCurBank ].uiObject;
			}
			break;

		case DRAW_MODE_ROADS:
			if ( iNumRoadsSelected > 0 )
			{
				usUseIndex = SelRoads[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelRoads[ iCurBank ].uiObject;
			}
			break;

		case DRAW_MODE_WALLS:
			if ( iNumWallsSelected > 0 )
			{
				usUseIndex = SelSingleWall[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelSingleWall[ iCurBank ].uiObject;
			}
			break;
		case DRAW_MODE_DOORS:
			if ( iNumDoorsSelected > 0 )
			{
				usUseIndex = SelSingleDoor[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelSingleDoor[ iCurBank ].uiObject;
			}
			break;
		case DRAW_MODE_WINDOWS:
			if( iNumWindowsSelected > 0 )
			{
				usUseIndex = SelSingleWindow[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelSingleWindow[ iCurBank ].uiObject;
			}
			break;
		case DRAW_MODE_ROOFS:
			if ( iNumRoofsSelected > 0 )
			{
				usUseIndex = SelSingleRoof[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelSingleRoof[ iCurBank ].uiObject;
			}
			break;
		case DRAW_MODE_NEWROOF:
			if( iNumNewRoofsSelected > 0 )
			{
				usUseIndex = SelSingleNewRoof[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelSingleNewRoof[ iCurBank ].uiObject;
			}
			break;
		case DRAW_MODE_BROKEN_WALLS:
			if ( iNumBrokenWallsSelected > 0 )
			{
				usUseIndex = SelSingleBrokenWall[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelSingleBrokenWall[ iCurBank ].uiObject;
			}
			break;
		case DRAW_MODE_DECOR:
			if ( iNumDecorSelected > 0 )
			{
				usUseIndex = SelSingleDecor[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelSingleDecor[ iCurBank ].uiObject;
			}
			break;
		case DRAW_MODE_DECALS:
			if ( iNumDecalsSelected > 0 )
			{
				usUseIndex = SelSingleDecal[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelSingleDecal[ iCurBank ].uiObject;
			}
			break;
		case DRAW_MODE_FLOORS:
			if ( iNumFloorsSelected > 0 )
			{
				usUseIndex = SelSingleFloor[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelSingleFloor[ iCurBank ].uiObject;
			}
			break;
		case DRAW_MODE_TOILET:
			if( iNumToiletsSelected > 0 )
			{
				usUseIndex = SelSingleToilet[ iCurBank ].usIndex;
				usObjIndex = (UINT16)SelSingleToilet[ iCurBank ].uiObject;
			}
			break;
		case DRAW_MODE_SMART_WALLS:
			if ( GetMouseXY( &sGridX, &sGridY ) )
			{
				iMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );
				if( CalcWallInfoUsingSmartMethod( iMapIndex, &usObjIndex, &usUseIndex ) )
					break;
			}
			CalcSmartWallDefault( &usObjIndex, &usUseIndex );
			break;
		case DRAW_MODE_SMART_DOORS:
			if ( GetMouseXY( &sGridX, &sGridY ) )
			{
				iMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );
				if( CalcDoorInfoUsingSmartMethod( iMapIndex, &usObjIndex, &usUseIndex ) )
					break;
			}
			CalcSmartDoorDefault( &usObjIndex, &usUseIndex );
			break;
		case DRAW_MODE_SMART_WINDOWS:
			if ( GetMouseXY( &sGridX, &sGridY ) )
			{
				iMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );
				if( CalcWindowInfoUsingSmartMethod( iMapIndex, &usObjIndex, &usUseIndex ) )
					break;
			}
			CalcSmartWindowDefault( &usObjIndex, &usUseIndex );
			break;
		case DRAW_MODE_SMART_BROKEN_WALLS:
			if ( GetMouseXY( &sGridX, &sGridY ) )
			{
				iMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );
				if( CalcBrokenWallInfoUsingSmartMethod( iMapIndex, &usObjIndex, &usUseIndex ) )
					break;
			}
			CalcSmartBrokenWallDefault( &usObjIndex, &usUseIndex );
			break;

		case DRAW_MODE_PLACE_ITEM:
			DisplayItemStatistics();
			break;
	}

	// If we actually have something to draw, draw it
	if ( (usUseIndex != 0xffff) && (usObjIndex != 0xffff) )
	{
		pETRLEObject = &(gTileDatabase[gTileTypeStartIndex[usObjIndex]].hTileSurface->pETRLEObject[usUseIndex]);

		iPicWidth = (INT32)pETRLEObject->usWidth;
		iPicHeight = (INT32)pETRLEObject->usHeight;

		// Center the picture in the display window.
		iStartX = ( 100 - iPicWidth ) / 2;
		iStartY = ( 60 - iPicHeight ) / 2;

		// We have to store the offset data in temp variables before zeroing them and blitting
		sTempOffsetX = pETRLEObject->sOffsetX;
		sTempOffsetY = pETRLEObject->sOffsetY;

		// Set the offsets used for blitting to 0
		pETRLEObject->sOffsetX = 0;
		pETRLEObject->sOffsetY = 0;

		SetObjectShade( gTileDatabase[gTileTypeStartIndex[usObjIndex]].hTileSurface, DEFAULT_SHADE_LEVEL );
		BltVideoObject( FRAME_BUFFER, gTileDatabase[gTileTypeStartIndex[usObjIndex]].hTileSurface,
									usUseIndex, (iScreenWidthOffset + 0 + iStartX), (2 * iScreenHeightOffset + 400 + iStartY),
									VO_BLT_SRCTRANSPARENCY, NULL);

		pETRLEObject->sOffsetX = sTempOffsetX;
		pETRLEObject->sOffsetY = sTempOffsetY;
	}

	// Set the color for the window's border. Blueish color = Normal, Red = Fake lighting is turned on
	usFillColor = GenericButtonFillColors[0];
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	if(gbPixelDepth==16)
		RectangleDraw( FALSE, iScreenWidthOffset + 0, 2 * iScreenHeightOffset + 400, iScreenWidthOffset + 99, 2 * iScreenHeightOffset + 458, usFillColor, pDestBuf );
	else if(gbPixelDepth==8)
		RectangleDraw8( FALSE, iScreenWidthOffset + 0, 2 * iScreenHeightOffset + 400, iScreenWidthOffset + 99, 2 * iScreenHeightOffset + 458, usFillColor, pDestBuf );

	UnLockVideoSurface( FRAME_BUFFER );

	InvalidateRegion( iScreenWidthOffset + 0, 2 * iScreenHeightOffset + 400, iScreenWidthOffset + 100, 2 * iScreenHeightOffset + 458 );
	SetClippingRect(&ClipRect);
}



//----------------------------------------------------------------------------------------------
//	HandleJA2ToolbarSelection
//
//	Select action to be taken based on user's toolbar selection.
//
void HandleJA2ToolbarSelection( void )
{
	BOOLEAN fPrevState;

	fPrevState = gfRenderTaskbar;
	gfRenderTaskbar = TRUE;

	switch (iEditorToolbarState)
	{

		case TBAR_MODE_SET_BGRND:
			iCurrentAction = ACTION_SET_NEW_BACKGROUND;
			break;

		case TBAR_MODE_DENS_DWN:
			iCurrentAction = ACTION_DENSITY_DOWN;
			break;

		case TBAR_MODE_DENS_UP:
			iCurrentAction = ACTION_DENSITY_UP;
			break;

		case TBAR_MODE_RAISE_LAND:
			iCurrentAction = ACTION_RAISE_LAND;
			break;

		case TBAR_MODE_LOWER_LAND:
			iCurrentAction = ACTION_LOWER_LAND;
			break;

		case TBAR_MODE_DEC_DIFF:
			if ( sCurBaseDiff > 0 )
				sCurBaseDiff--;
			break;

		case TBAR_MODE_INC_DIFF:
			if ( sCurBaseDiff < 4 )
				sCurBaseDiff++;
			break;

		case TBAR_MODE_FAKE_LIGHTING:
			gfFakeLights ^= TRUE;
			if ( gfFakeLights )
			{
				gusSavedLightLevel = gusLightLevel;
				gusLightLevel = EDITOR_LIGHT_FAKE;
			}
			else
				gusLightLevel = gusSavedLightLevel;
			LightSetBaseLevel( (UINT8)(EDITOR_LIGHT_MAX - gusLightLevel) );
			LightSpriteRenderAll();
			gfRenderWorld = TRUE;//dnl ch80 011213
			break;

		case TBAR_MODE_CHANGE_TILESET:
			InitPopupMenu( iEditorButton[OPTIONS_CHANGE_TILESET], CHANGETSET_POPUP, DIR_UPRIGHT );
			break;

		case TBAR_MODE_CIVILIAN_GROUP:
			InitPopupMenu( iEditorButton[MERCS_CIVILIAN_GROUP], CHANGECIVGROUP_POPUP, DIR_UPLEFT );
			break;

		case TBAR_MODE_GET_WALL:
			iCurrentAction = ACTION_GET_WALL;
			iDrawMode = DRAW_MODE_WALLS;
			break;
		case TBAR_MODE_GET_DOOR:
			iCurrentAction = ACTION_GET_DOOR;
			iDrawMode = DRAW_MODE_DOORS;
			break;
		case TBAR_MODE_GET_WINDOW:
			iCurrentAction = ACTION_GET_WINDOW;
			iDrawMode = DRAW_MODE_WINDOWS;
			break;
		case TBAR_MODE_GET_ROOF:
			iCurrentAction = ACTION_GET_ROOF;
			iDrawMode = DRAW_MODE_ROOFS;
			break;
		case TBAR_MODE_GET_BROKEN_WALL:
			iCurrentAction = ACTION_GET_BROKEN_WALL;
			iDrawMode = DRAW_MODE_BROKEN_WALLS;
			break;
		case TBAR_MODE_GET_DECOR:
			iCurrentAction = ACTION_GET_DECOR;
			iDrawMode = DRAW_MODE_DECOR;
			break;
		case TBAR_MODE_GET_DECAL:
			iCurrentAction = ACTION_GET_DECAL;
			iDrawMode = DRAW_MODE_DECALS;
			break;
		case TBAR_MODE_GET_FLOOR:
			iCurrentAction = ACTION_GET_FLOOR;
			iDrawMode = DRAW_MODE_FLOORS;
			break;
		case TBAR_MODE_GET_TOILET:
			iCurrentAction = ACTION_GET_TOILET;
			iDrawMode = DRAW_MODE_TOILET;
			break;
		case TBAR_MODE_GET_ROOM:
			iCurrentAction = ACTION_GET_ROOM;
			break;
		case TBAR_MODE_GET_NEW_ROOF:
			iCurrentAction = ACTION_GET_NEW_ROOF;
			iDrawMode = DRAW_MODE_NEWROOF;
			break;

		case TBAR_MODE_FILL_AREA:
			iDrawMode = DRAW_MODE_GROUND + DRAW_MODE_FILL_AREA;
			ClickEditorButton( TERRAIN_FGROUND_TEXTURES );
			TerrainTileDrawMode = TERRAIN_TILES_FOREGROUND;
			break;

		case TBAR_MODE_FILL_AREA_OFF:
			iDrawMode = DRAW_MODE_GROUND;
			ClickEditorButton( TERRAIN_FGROUND_TEXTURES );
			TerrainTileDrawMode = TERRAIN_TILES_FOREGROUND;
			break;

		case TBAR_MODE_GET_BANKS:
			iCurrentAction = ACTION_NEXT_BANK;
			iDrawMode = DRAW_MODE_BANKS;
			break;

		case TBAR_MODE_GET_ROADS:
			iCurrentAction = ACTION_NEXT_ROAD;
			iDrawMode = DRAW_MODE_ROADS;
			break;

		case TBAR_MODE_DRAW_BANKS:
			iCurrentAction = ACTION_NULL;
			iDrawMode = DRAW_MODE_BANKS;
			break;

		case TBAR_MODE_GET_DEBRIS:
			iCurrentAction = ACTION_NEXT_DEBRIS;
			iDrawMode = DRAW_MODE_DEBRIS;
			break;

		case TBAR_MODE_DRAW_DEBRIS:
			iCurrentAction = ACTION_NULL;
			iDrawMode = DRAW_MODE_DEBRIS;
			break;

		case TBAR_MODE_GET_OSTRUCTS:
			iCurrentAction = ACTION_NEXT_STRUCT;
			iDrawMode = DRAW_MODE_OSTRUCTS;
			break;

		case TBAR_MODE_DRAW_OSTRUCTS:
			iCurrentAction = ACTION_NULL;
			iDrawMode = DRAW_MODE_OSTRUCTS;
			break;

		case TBAR_MODE_GET_OSTRUCTS1:
			iCurrentAction = ACTION_NEXT_STRUCT1;
			iDrawMode = DRAW_MODE_OSTRUCTS1;
			break;

		case TBAR_MODE_DRAW_OSTRUCTS1:
			iCurrentAction = ACTION_NULL;
			iDrawMode = DRAW_MODE_OSTRUCTS1;
			break;

		case TBAR_MODE_GET_OSTRUCTS2:
			iCurrentAction = ACTION_NEXT_STRUCT2;
			iDrawMode = DRAW_MODE_OSTRUCTS2;
			break;

		case TBAR_MODE_DRAW_OSTRUCTS2:
			iCurrentAction = ACTION_NULL;
			iDrawMode = DRAW_MODE_OSTRUCTS2;
			break;

		case TBAR_MODE_EXIT_EDIT:
			iCurrentAction = ACTION_EXIT_EDITOR;
			break;

		case TBAR_MODE_QUIT_GAME:
			iCurrentAction = ACTION_QUIT_GAME;
			break;

		case TBAR_MODE_RADAR_MAP://dnl ch9 071009
			iCurrentAction = ACTION_RADAR_MAP;
			break;

		case TBAR_MODE_SAVE:
			iCurrentAction = ACTION_SAVE_MAP;
			break;

		case TBAR_MODE_LOAD:
			iCurrentAction = ACTION_LOAD_MAP;
			break;

		case TBAR_MODE_UNDO:
			iCurrentAction = ACTION_UNDO;
			break;

		case TBAR_MODE_CHANGE_BRUSH:
			iCurrentAction = ACTION_NEXT_SELECTIONTYPE;
			break;

		case TBAR_MODE_ERASE:
			switch ( iDrawMode )
			{
				case DRAW_MODE_EXITGRID:
				case DRAW_MODE_LIGHT:
				case DRAW_MODE_GROUND:
				case DRAW_MODE_OSTRUCTS:
				case DRAW_MODE_OSTRUCTS1:
				case DRAW_MODE_OSTRUCTS2:
				case DRAW_MODE_DEBRIS:
				case DRAW_MODE_BANKS:
				case DRAW_MODE_ROADS:

				case DRAW_MODE_WALLS:
				case DRAW_MODE_DOORS:
				case DRAW_MODE_WINDOWS:
				case DRAW_MODE_ROOFS:
				case DRAW_MODE_BROKEN_WALLS:
				case DRAW_MODE_DECALS:
				case DRAW_MODE_DECOR:
				case DRAW_MODE_FLOORS:
				case DRAW_MODE_TOILET:

				case DRAW_MODE_ROOM:
				case DRAW_MODE_SLANTED_ROOF:
				case DRAW_MODE_ROOMNUM:

				//dnl ch41 120909
				case DRAW_MODE_NORTHPOINT:
				case DRAW_MODE_WESTPOINT:
				case DRAW_MODE_EASTPOINT:
				case DRAW_MODE_SOUTHPOINT:
				case DRAW_MODE_CENTERPOINT:
				case DRAW_MODE_ISOLATEDPOINT:

				case DRAW_MODE_HIGH_GROUND://dnl ch1 210909
				case DRAW_MODE_DOORKEYS://dnl ch86 220214

					iDrawMode += DRAW_MODE_ERASE;
					break;
			}
			break;

		case TBAR_MODE_ERASE_OFF:
			if ( iDrawMode >= DRAW_MODE_ERASE )
				iDrawMode -= DRAW_MODE_ERASE;
			break;

		case TBAR_MODE_NEW_MAP:
			iCurrentAction = ACTION_NEW_MAP;
			break;

		case TBAR_MODE_ITEM_WEAPONS:
		case TBAR_MODE_ITEM_AMMO:
		case TBAR_MODE_ITEM_ARMOUR:
		case TBAR_MODE_ITEM_LBEGEAR:
		case TBAR_MODE_ITEM_EXPLOSIVES:
		case TBAR_MODE_ITEM_EQUIPMENT1:
		case TBAR_MODE_ITEM_EQUIPMENT2:
		case TBAR_MODE_ITEM_EQUIPMENT3:
		case TBAR_MODE_ITEM_TRIGGERS:
		case TBAR_MODE_ITEM_KEYS:
		case TBAR_MODE_ITEM_RANDOMITEM:
			//Set up the items by type.
			InitEditorItemsInfo(iEditorToolbarState);
			if( gubCurrMercMode != MERC_GETITEMMODE )
				iDrawMode = DRAW_MODE_PLACE_ITEM;
			break;
		case TBAR_MODE_NONE:
			iCurrentAction = ACTION_NULL;
			gfRenderTaskbar = fPrevState;
			break;
		default:
			iCurrentAction = ACTION_NULL;
			iDrawMode = DRAW_MODE_NOTHING;
			gfRenderTaskbar = fPrevState;
			break;
	}
	iEditorToolbarState = TBAR_MODE_NONE;
}

//----------------------------------------------------------------------------------------------
//	HandleKeyboardShortcuts
//
//	Select action to be taken based on the user's current key press (if any)
//
extern INT8 gbCurrSelect;
extern void DeleteSelectedMercsItem();
void HandleKeyboardShortcuts( )
{
	static INT32 iSavedMode;
	static BOOLEAN fShowTrees = TRUE;
	while( PeekSpecificEvent(KEY_DOWN | KEY_UP | KEY_REPEAT) && DequeueEvent( &EditorInputEvent ) )//dnl ch74 221013
	{
		if( !HandleSummaryInput( &EditorInputEvent ) && !HandleTextInput( &EditorInputEvent ) && EditorInputEvent.usEvent == KEY_DOWN )
		{
			if(InOverheadMap() && !(EditorInputEvent.usParam == ESC))//dnl ch81 031213
				return;
			if(gfEditingDoor && !(EditorInputEvent.usParam == ESC || EditorInputEvent.usParam == ENTER))//dnl ch86 220214
				return;
			if( gfGotoGridNoUI )
			{
				switch( EditorInputEvent.usParam )
				{
					case ESC:
						SetInputFieldStringWith16BitString( 0, L"" );
						RemoveGotoGridNoUI();
						break;
					case ENTER:
						RemoveGotoGridNoUI();
						break;
					case 'x':
						if( EditorInputEvent.usKeyState & ALT_DOWN )
						{
							SetInputFieldStringWith16BitString( 0, L"" );
							RemoveGotoGridNoUI();
							iCurrentAction = ACTION_QUIT_GAME;
						}
						break;
				}
			}
			else if( gfKeyboardItemCreationUI )
			{
				switch( EditorInputEvent.usParam )
				{
					case ESC:
						SetInputFieldStringWith16BitString( 0, L"" );
						RemoveKeyboardItemCreationUI();
						break;
					case ENTER:
						RemoveKeyboardItemCreationUI();
						break;
					case 'x':
						if( EditorInputEvent.usKeyState & ALT_DOWN )
						{
							SetInputFieldStringWith16BitString( 0, L"" );
							RemoveKeyboardItemCreationUI();
							iCurrentAction = ACTION_QUIT_GAME;
						}
						break;
				}
			}
			else switch( EditorInputEvent.usParam )
			{
				case HOME:
					gfFakeLights ^= TRUE;
					if ( gfFakeLights )
					{
						gusSavedLightLevel = gusLightLevel;
						gusLightLevel = EDITOR_LIGHT_FAKE;
						ClickEditorButton( MAPINFO_TOGGLE_FAKE_LIGHTS );
					}
					else
					{
						gusLightLevel = gusSavedLightLevel;
						UnclickEditorButton( MAPINFO_TOGGLE_FAKE_LIGHTS );
					}
					LightSetBaseLevel( (UINT8)(EDITOR_LIGHT_MAX - gusLightLevel) );
					LightSpriteRenderAll();
					gfRenderWorld = TRUE;//dnl ch80 011213
					break;

				case SPACE:
					if( iCurrentTaskbar == TASK_MERCS )
						IndicateSelectedMerc( SELECT_NEXT_MERC );
					else if( iCurrentTaskbar == TASK_ITEMS )
						SelectNextItemPool();
					else if( gfShowTerrainTileButtons && fUseTerrainWeights )
						ResetTerrainTileWeights();
					else
						LightSpriteRenderAll();
					break;

				case INSERT:
					if ( iDrawMode == DRAW_MODE_GROUND )
					{
						iDrawMode += DRAW_MODE_FILL_AREA;
						ClickEditorButton( TERRAIN_FILL_AREA );
						iEditorToolbarState = TBAR_MODE_FILL_AREA;
					}
					else if ( iDrawMode == (DRAW_MODE_GROUND + DRAW_MODE_FILL_AREA) )
					{
						iDrawMode -= DRAW_MODE_FILL_AREA;
						UnclickEditorButton( TERRAIN_FILL_AREA );
						iEditorToolbarState = TBAR_MODE_FILL_AREA_OFF;
					}
					break;
				case ENTER:
					if( gfEditingDoor )
					{
						ExtractAndUpdateDoorInfo();
						KillDoorEditing();
					}
					else if( iCurrentTaskbar == TASK_MERCS && gsSelectedMercID != -1 )
						ExtractCurrentMercModeInfo( FALSE );
					else if(iCurrentTaskbar == TASK_MAPINFO)//dnl ch80 011213
					{
						gfFakeLights = FALSE;
						UnclickEditorButton(MAPINFO_TOGGLE_FAKE_LIGHTS);
						ExtractAndUpdateMapInfo();
					}
					else if( iCurrentTaskbar == TASK_BUILDINGS )
						ExtractAndUpdateBuildingInfo();
					else if( gfShowItemStatsPanel && EditingText() )
						ExecuteItemStatsCmd( ITEMSTATS_APPLY );
					else if(iCurrentTaskbar == TASK_OPTIONS && GetActiveFieldID() == 1)//dnl ch52 091009
						SelectNextField();
					break;

				case BACKSPACE:
					iCurrentAction = ACTION_UNDO;
					break;

				case DEL:
					if( iCurrentTaskbar == TASK_ITEMS )
						DeleteSelectedItem();
					else if( gsSelectedMercID != -1 )
					{
						if( gubCurrMercMode == MERC_INVENTORYMODE && gbCurrSelect != -1 )
						{
							DeleteSelectedMercsItem();
							gbCurrSelect = -1;
							gfRenderTaskbar = TRUE;
						}
						else
							DeleteSelectedMerc();
					}
					else
						iCurrentAction = ACTION_QUICK_ERASE;
					break;

				case ESC:
					if ( InOverheadMap() )
					{
						KillOverheadMap();
					}
					else if( iDrawMode == DRAW_MODE_SCHEDULEACTION )//dnl ch81 031213
					{
						CancelCurrentScheduleAction();
					}
					else if( gfMercGetItem )
					{ //cancel getting an item for the merc.
						gfMercGetItem = FALSE;
						gusMercsNewItemIndex = 0xffff;
						SetMercEditingMode( MERC_INVENTORYMODE );
						ClearEditorItemsInfo();
					}
					else if( gfShowItemStatsPanel && EditingText() )
						ExecuteItemStatsCmd( ITEMSTATS_CANCEL );
					else if( gfEditingDoor )
						KillDoorEditing();
					else
						iCurrentAction = ACTION_EXIT_EDITOR;
					break;

				//Select next/prev terrain tile to draw with.
				case SHIFT_LEFTARROW:
					CurrentPaste -= ( gfShowTerrainTileButtons && CurrentPaste > 0 ) ? 1 : 0;
					break;
				case SHIFT_RIGHTARROW:
					CurrentPaste += ( gfShowTerrainTileButtons && CurrentPaste < 8 ) ? 1 : 0;
					break;

				case PGUP:
					if( iCurrentTaskbar == TASK_MERCS && !fBuildingShowRoofs )
					{
						gfRoofPlacement = TRUE;
						fBuildingShowRoofs = TRUE;
						UpdateRoofsView();
						gfRenderWorld = TRUE;
					}
					else switch( iDrawMode )
					{
						case DRAW_MODE_SMART_WALLS:					DecSmartWallUIValue();									break;
						case DRAW_MODE_SMART_DOORS:					DecSmartDoorUIValue();									break;
						case DRAW_MODE_SMART_WINDOWS:				DecSmartWindowUIValue();								break;
						case DRAW_MODE_SMART_BROKEN_WALLS:	DecSmartBrokenWallUIValue();						break;
						case DRAW_MODE_PLACE_ITEM:					SelectPrevItemInPool();									break;
						default:														iCurrentAction = ACTION_SUB_INDEX_UP;		break;
					}
					gfRenderDrawingMode = TRUE;
					break;
				case PGDN:
					if( iCurrentTaskbar == TASK_MERCS && fBuildingShowRoofs )
					{
						gfRoofPlacement = FALSE;
						fBuildingShowRoofs = FALSE;
						UpdateRoofsView();
						gfRenderWorld = TRUE;
					}
					else switch( iDrawMode )
					{
						case DRAW_MODE_SMART_WALLS:					IncSmartWallUIValue();									break;
						case DRAW_MODE_SMART_DOORS:					IncSmartDoorUIValue();									break;
						case DRAW_MODE_SMART_WINDOWS:				IncSmartWindowUIValue();								break;
						case DRAW_MODE_SMART_BROKEN_WALLS:	IncSmartBrokenWallUIValue();						break;
						case DRAW_MODE_PLACE_ITEM:					SelectNextItemInPool();									break;
						default:														iCurrentAction = ACTION_SUB_INDEX_DWN;	break;
					}
					gfRenderDrawingMode = TRUE;
					break;

				case F1:
					/*
					//Buggler: help screen not in used due to space limitations
					iCurrentAction = ACTION_HELPSCREEN;
					*/
					gfRenderWorld = TRUE;
					gfRenderTaskbar = TRUE;
					break;

				case F2:
					if (EditorInputEvent.usKeyState & ALT_DOWN )
					{
						ReloadMap();
					}
					break;

				case F3:
					if( EditorInputEvent.usKeyState & CTRL_DOWN )
					{
						ReplaceObsoleteRoads();
						MarkWorldDirty();
					}
					break;

				case F4:
					MusicPlay(gMusicMode, giMusicID);
					ScreenMsg( FONT_YELLOW, MSG_INTERFACE, L"%S", MusicLists[gMusicMode][giMusicID] );

					// Select next track 
					giMusicID++;
					if (giMusicID >= MusicLists[gMusicMode].size())
					{
						giMusicID = 0;
						for (size_t i = 0; i < MAX_MUSIC; i++)
						{
							if (gMusicMode == i)
							{
								gMusicMode = static_cast<NewMusicList>(i + 1);
								if (gMusicMode == MAX_MUSIC)
								{
									gMusicMode = MUSICLIST_MAIN_MENU;
								}
								break;
							}
						}
					}
					break;

				case F5:
					UpdateLastActionBeforeLeaving();
					CreateSummaryWindow();
					break;

				case F6:
					break;

				case F7:
					if( gfBasement )
					{
						INT32 i;
						UINT16 usRoofIndex, usRoofType, usTileIndex;
						pSelList = SelSingleRoof;
						pNumSelList = &iNumRoofsSelected;
						usRoofType = GetRandomIndexByRange( FIRSTROOF, LASTROOF );
						if( usRoofType == 0xffff )
							usRoofType = FIRSTROOF;
						for( i = 0; i < WORLD_MAX; i++ )
						{
							if( gusWorldRoomInfo[ i ] )
							{
								AddToUndoList( i );
								RemoveAllRoofsOfTypeRange( i, FIRSTTEXTURE, LASTITEM );
								RemoveAllOnRoofsOfTypeRange( i, FIRSTTEXTURE, LASTITEM );
								RemoveAllShadowsOfTypeRange( i, FIRSTROOF, LASTSLANTROOF );
								usRoofIndex = 9 + ( rand() % 3 );
								GetTileIndexFromTypeSubIndex( usRoofType, usRoofIndex, &usTileIndex );
								AddRoofToHead( i, usTileIndex );
							}
						}
						MarkWorldDirty();
					}
					break;

				case F8:
					SmoothAllTerrainWorld();
					break;

				case F9:
					break;
				case F10:
					CreateMessageBox( pHandleKeyboardShortcutsText[0] );
					gfRemoveLightsPending = TRUE;
					break;
				case F11:
					CreateMessageBox( pHandleKeyboardShortcutsText[1] );
					gfScheduleReversalPending = TRUE;
					break;
				case F12:
					CreateMessageBox( pHandleKeyboardShortcutsText[2] );
					gfScheduleClearPending = TRUE;
					break;

				case '[':
					iEditorToolbarState = TBAR_MODE_DENS_DWN;
					break;

				case ']':
					iEditorToolbarState = TBAR_MODE_DENS_UP;
					break;

				case '+':
					//if ( iDrawMode == DRAW_MODE_SHOW_TILESET )
					//	iCurrentAction = ACTION_MLIST_DWN;
					//else
						iCurrentAction = ACTION_SHADE_UP;
					break;

				case '-':
					//if ( iDrawMode == DRAW_MODE_SHOW_TILESET )
					//	iCurrentAction = ACTION_MLIST_UP;
					iCurrentAction = ACTION_SHADE_DWN;
					break;

				case '`':
					// Buggler: Swap Cursor Level
					if( !gfRoofPlacement )
					{
						gfRoofPlacement = TRUE;
						if( !fBuildingShowRoofs )
						{
							fBuildingShowRoofs = TRUE;
							ClickEditorButton( BUILDING_TOGGLE_ROOF_VIEW );
						}
					}
					else
					{
						gfRoofPlacement = FALSE;
						if( fBuildingShowRoofs )
						{
							fBuildingShowRoofs = FALSE;
							UnclickEditorButton( BUILDING_TOGGLE_ROOF_VIEW );
						}
					}
					UpdateRoofsView();
					break;

				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
						if ( iCurrentTaskbar == TASK_MERCS )
						{
							if ( iDrawMode >= DRAW_MODE_ERASE )
								iCurrentAction = ACTION_ERASE_WAYPOINT;
							else
								iCurrentAction = ACTION_SET_WAYPOINT;
							iActionParam = EditorInputEvent.usParam - '0';
						}
						else
						{
							iCurrentAction = ACTION_SET_FNAME;
							iActionParam = EditorInputEvent.usParam - '0';
						}
					break;

				case 'a':
					iCurrentAction = ACTION_PREV_SELECTIONTYPE;
					break;
				case 'b':
					if ( iCurrentTaskbar != TASK_BUILDINGS )
					{
						iTaskMode = TASK_BUILDINGS;
						DoTaskbar();
					}

					if( CheckForSlantRoofs() )
						iDrawMode = DRAW_MODE_SLANTED_ROOF;
					else
					{
						gusSelectionType = gusSavedBuildingSelectionType;
						iDrawMode = DRAW_MODE_ROOM;
					}
					if( gfCaves )
						SetEditorBuildingTaskbarMode( BUILDING_CAVE_DRAWING );
					else
						SetEditorBuildingTaskbarMode( BUILDING_NEW_ROOM );
					TerrainTileDrawMode = TERRAIN_TILES_BRETS_STRANGEMODE;
					break;
				case 'c':
					if( EditorInputEvent.usKeyState & CTRL_DOWN && iCurrentTaskbar == TASK_MERCS )
					{
						iCurrentAction = ACTION_COPY_MERC_PLACEMENT;
					}
					else
					{
						if( iCurrentTaskbar != TASK_TERRAIN )
						{
							iTaskMode = TASK_TERRAIN;
							DoTaskbar();
						}

						iCurrentAction = ACTION_NULL;
						SetEditorTerrainTaskbarMode( TERRAIN_PLACE_CLIFFS );
					}
					break;
				case 'C'://dnl ch7 210909 Clicked Placement Switch
					if(fDontUseClick)
					{
						fDontUseClick = FALSE;
						ScreenMsg(FONT_MCOLOR_WHITE, MSG_INTERFACE, pHandleKeyboardShortcutsText[3] );
					}
					else
					{
						fDontUseClick = TRUE;
						ScreenMsg(FONT_MCOLOR_WHITE, MSG_INTERFACE, pHandleKeyboardShortcutsText[4] );
					}
					break;
				case 'd': // debris
					if( iCurrentTaskbar != TASK_TERRAIN )
					{
						iTaskMode = TASK_TERRAIN;
						DoTaskbar();
					}

					iCurrentAction = ACTION_NULL;
					SetEditorTerrainTaskbarMode( TERRAIN_PLACE_DEBRIS );
					break;
				case 'e':
					if ( iDrawMode >= DRAW_MODE_ERASE )
					{
						iDrawMode -= DRAW_MODE_ERASE;
					}
					else
					{
						switch ( iDrawMode )
						{
							case DRAW_MODE_NORTHPOINT:
							case DRAW_MODE_WESTPOINT:
							case DRAW_MODE_EASTPOINT:
							case DRAW_MODE_SOUTHPOINT:
							case DRAW_MODE_CENTERPOINT:
							case DRAW_MODE_ISOLATEDPOINT:
							case DRAW_MODE_EXITGRID:
							case DRAW_MODE_LIGHT:
							case DRAW_MODE_GROUND:
							case DRAW_MODE_OSTRUCTS:
							case DRAW_MODE_OSTRUCTS1:
							case DRAW_MODE_OSTRUCTS2:
							case DRAW_MODE_DEBRIS:
							case DRAW_MODE_BANKS:
							case DRAW_MODE_ROADS:
							case DRAW_MODE_WALLS:
							case DRAW_MODE_DOORS:
							case DRAW_MODE_WINDOWS:
							case DRAW_MODE_ROOFS:
							case DRAW_MODE_BROKEN_WALLS:
							case DRAW_MODE_DECOR:
							case DRAW_MODE_DECALS:
							case DRAW_MODE_FLOORS:
							case DRAW_MODE_TOILET:
							case DRAW_MODE_ROOM:
							case DRAW_MODE_SLANTED_ROOF:
							case DRAW_MODE_ROOMNUM:
							case DRAW_MODE_HIGH_GROUND://dnl ch1 210909
								iDrawMode += DRAW_MODE_ERASE;
								break;
						}
					}
					break;
				case 'f':
					gbFPSDisplay = !gbFPSDisplay;
					DisableFPSOverlay( (BOOLEAN)!gbFPSDisplay );
					break;
				case 'g':	// ground
					if( EditorInputEvent.usKeyState & CTRL_DOWN )
					{
						CreateGotoGridNoUI();
					}
					else
					{
						if ( iCurrentTaskbar != TASK_TERRAIN )
						{
							iTaskMode = TASK_TERRAIN;
							DoTaskbar();
						}
						iCurrentAction = ACTION_NULL;
						SetEditorTerrainTaskbarMode( TERRAIN_FGROUND_TEXTURES );
					}
					break;
				case 'h':
					if( fBuildingShowRoofs ^= 1 )
						ClickEditorButton( BUILDING_TOGGLE_ROOF_VIEW );
					else
						UnclickEditorButton( BUILDING_TOGGLE_ROOF_VIEW );
					UpdateRoofsView();
					break;
				case 'i':
					if( !InOverheadMap() )
						GoIntoOverheadMap();
					else
						KillOverheadMap();
					break;
				case 'j'://dnl ch1 210909
					if(iDrawMode != DRAW_MODE_HIGH_GROUND)
					{
						iDrawMode = DRAW_MODE_HIGH_GROUND;
						ScreenMsg(FONT_MCOLOR_WHITE, MSG_INTERFACE, pHandleKeyboardShortcutsText[5] );
					}
					else
					{
						iDrawMode = DRAW_MODE_NOTHING;
						ScreenMsg(FONT_MCOLOR_WHITE, MSG_INTERFACE, pHandleKeyboardShortcutsText[6] );
					}
					break;
#ifdef dnlTEST
				case 'J':
					break;
#endif
				case 'k'://dnl ch2 210909
					if(fShowHighGround)
					{
						fShowHighGround = FALSE;
						ShowHighGround(3);
					}
					else
					{
						fShowHighGround = TRUE;
						ShowHighGround(1);
					}
					break;
				case 'K'://dnl ch5 210909
					ShowHighGround(4);
					break;
				case 'l':
					if (EditorInputEvent.usKeyState & CTRL_DOWN )
					{
						UpdateLastActionBeforeLeaving();
						iCurrentAction = ACTION_LOAD_MAP;
						break;
					}
					if ( iCurrentTaskbar != TASK_TERRAIN )
					{
						iTaskMode = TASK_TERRAIN;
						DoTaskbar();
					}
					break;
				case 'L'://dnl ch23 210909
					ScreenMsg(FONT_MCOLOR_WHITE, MSG_INTERFACE, pHandleKeyboardShortcutsText[7], gus1stNorthEdgepointArraySize, gus1stEastEdgepointArraySize, gus1stSouthEdgepointArraySize, gus1stWestEdgepointArraySize);
					ToggleMapEdgepoints();
					break;
#ifdef dnlTEST
				case 'm':
					break;
				case 'M':
					break;
#endif
				case 'n':
					if (EditorInputEvent.usKeyState & CTRL_DOWN )
					{					
						UpdateLastActionBeforeLeaving();
						iCurrentAction = ACTION_NEW_MAP;
						break;
					}
					if( fBuildingShowRoomInfo ^= 1 )
					{
						SetRenderFlags( RENDER_FLAG_ROOMIDS );
						ClickEditorButton( BUILDING_TOGGLE_INFO_VIEW );
					}
					else
					{
						ClearRenderFlags( RENDER_FLAG_ROOMIDS );
						UnclickEditorButton( BUILDING_TOGGLE_INFO_VIEW );
					}
					break;
				case 'o':
					if ( iCurrentTaskbar != TASK_TERRAIN )
					{
						iTaskMode = TASK_TERRAIN;
						DoTaskbar();
					}
					iCurrentAction = ACTION_NULL;
					SetEditorTerrainTaskbarMode( TERRAIN_PLACE_MISC );
					break;
				case 'p': // roads
					if( iCurrentTaskbar != TASK_TERRAIN )
					{
						iTaskMode = TASK_TERRAIN;
						DoTaskbar();
					}

					iCurrentAction = ACTION_NULL;
					SetEditorTerrainTaskbarMode( TERRAIN_PLACE_ROADS );
					break;
				case 'r': // rocks
					if ( iCurrentTaskbar != TASK_TERRAIN )
					{
						iTaskMode = TASK_TERRAIN;
						DoTaskbar();
					}
					iCurrentAction = ACTION_NULL;
					SetEditorTerrainTaskbarMode( TERRAIN_PLACE_ROCKS );
					break;
				case 'R'://dnl ch8 210909 Random Placement Switch
					if(fDontUseRandom)
					{
						fDontUseRandom = FALSE;
						ScreenMsg(FONT_MCOLOR_WHITE, MSG_INTERFACE, pHandleKeyboardShortcutsText[8] );
					}
					else
					{
						fDontUseRandom = TRUE;
						ScreenMsg(FONT_MCOLOR_WHITE, MSG_INTERFACE, pHandleKeyboardShortcutsText[9] );
					}
					break;
				case 's':
					if (EditorInputEvent.usKeyState & CTRL_DOWN )
					{
						iCurrentAction = ACTION_SAVE_MAP;
					}
					break;
				case 't':	// Trees
					if ( iCurrentTaskbar != TASK_TERRAIN )
					{
						iTaskMode = TASK_TERRAIN;
						DoTaskbar();
					}
					iCurrentAction = ACTION_NULL;
					SetEditorTerrainTaskbarMode( TERRAIN_PLACE_TREES );
					break;
				case 'T':
					if ( fShowTrees )
					{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pHandleKeyboardShortcutsText[10] );
						WorldHideTrees( );
					}
					else
					{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pHandleKeyboardShortcutsText[11] );
						WorldShowTrees( );
					}
					fShowTrees = !fShowTrees;

					break;
				//dnl ch3 210909
				case 'u':
				case 'U':
					gfRenderWorld = TRUE;
					if(fRaiseWorld)
					{
						fRaiseWorld = FALSE;
						for(int cnt=0; cnt<WORLD_MAX; cnt++)
							gpWorldLevelData[cnt].sHeight = 0;
						fShowHighGround = FALSE;
						ShowHighGround(3);
						ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pHandleKeyboardShortcutsText[12] );
					}
					else
					{
						fRaiseWorld = TRUE;
						if(EditorInputEvent.usParam == 'U')
						{
							RaiseWorldLandOld();
							ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pHandleKeyboardShortcutsText[13] );
						}
						else
						{
							RaiseWorldLand();
							ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pHandleKeyboardShortcutsText[14] );
						}
					}
					break;
				case 'w': // walls (full building)
					if( fBuildingShowWalls ^= 1 )
						ClickEditorButton( BUILDING_TOGGLE_WALL_VIEW );
					else
						UnclickEditorButton( BUILDING_TOGGLE_WALL_VIEW );
					UpdateWallsView();
					break;
				case 'v':
					if( EditorInputEvent.usKeyState & CTRL_DOWN && iCurrentTaskbar == TASK_MERCS )
					{
						iCurrentAction = ACTION_PASTE_MERC_PLACEMENT;
					}
					break;

				case 'x':
					if (EditorInputEvent.usKeyState & ALT_DOWN )
					{
						if( InOverheadMap() )
							KillOverheadMap();
						if( gfEditingDoor )
							KillDoorEditing();
						iCurrentAction = ACTION_QUIT_GAME;
					}
					break;
				case 'z':
					iCurrentAction = ACTION_NEXT_SELECTIONTYPE;
					break;
				case ',':
					// item left scroll
					if( iCurrentTaskbar == TASK_ITEMS || gubCurrMercMode == MERC_GETITEMMODE )
					{
						ScrollEditorItemsInfo(FALSE);
					}
					else
					{
						gusSelectionType = LINESELECTION;
						gusPreserveSelectionWidth--;
						if( !gusPreserveSelectionWidth )
							gusPreserveSelectionWidth = 8;
					}
					//gfRenderTaskbar = TRUE;
					break;
				case '.':
					// item right scroll
					if( iCurrentTaskbar == TASK_ITEMS || gubCurrMercMode == MERC_GETITEMMODE )
					{
						ScrollEditorItemsInfo(TRUE);
					}
					else
					{
						gusSelectionType = LINESELECTION;
						gusPreserveSelectionWidth++;
						if( gusPreserveSelectionWidth > 8 )
							gusPreserveSelectionWidth = 1;
					}
					//gfRenderTaskbar = TRUE;
					break;
				case '<':
					// item left scroll by page
					if( iCurrentTaskbar == TASK_ITEMS || gubCurrMercMode == MERC_GETITEMMODE )
					{
						ScrollEditorItemsInfo(FALSE);
					}
					//gfRenderTaskbar = TRUE;
					break;
				case '>':
					// item right scroll by page
					if( iCurrentTaskbar == TASK_ITEMS || gubCurrMercMode == MERC_GETITEMMODE )
					{
						ScrollEditorItemsInfo(TRUE);
					}
					//gfRenderTaskbar = TRUE;
					break;
				case '/':
					// Buggler: create item with keyboard input
					if( iCurrentTaskbar == TASK_ITEMS )
					{
						if( !GetMouseXY( &sGridX, &sGridY ) )
							return;

						if( !(gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA ) )
						{	//if mouse cursor not in the game screen.
							return;
						}
						
						fEditorCreateItemFromKeyboard = TRUE;

						giCreateItemCursorMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );
												
						if( EditorInputEvent.usKeyState & CTRL_DOWN )
							CreateKeyboardItemCreationUI();
						else
							AddSelectedItemToWorld( giCreateItemCursorMapIndex );

						gfRenderWorld = TRUE;
					}
					break;
#ifdef dnlTEST
				case '\'':
					break;
#endif
				default:
					iCurrentAction = ACTION_NULL;
					break;
			}
		}		
		/* //Buggler: Commented out specific keys repeating function as it will result in double character entry in input box
		else if ( !HandleSummaryInput( &EditorInputEvent ) && !HandleTextInput( &EditorInputEvent ) && EditorInputEvent.usEvent == KEY_REPEAT )
		{	
			switch( EditorInputEvent.usParam )
			{
				case ',':
					// item left scroll
					if( iCurrentTaskbar == TASK_ITEMS || gubCurrMercMode == MERC_GETITEMMODE )
					{
						if( eInfo.sScrollIndex )
						{
							if( EditorInputEvent.usKeyState & CTRL_DOWN )
								eInfo.sScrollIndex = __max(eInfo.sScrollIndex - 60, 0);
							else
								eInfo.sScrollIndex--;
							
							if( !eInfo.sScrollIndex )
								DisableButton( iEditorButton[ITEMS_LEFTSCROLL] );
							if( eInfo.sScrollIndex < ((eInfo.sNumItems+1)/2)-6 )
								EnableButton( iEditorButton[ITEMS_RIGHTSCROLL] );
						}
					}
					gfRenderTaskbar = TRUE;
					break;
				case '.':
					// item right scroll
					if( iCurrentTaskbar == TASK_ITEMS || gubCurrMercMode == MERC_GETITEMMODE )
					{
						if( eInfo.sScrollIndex < max( ((eInfo.sNumItems+1)/2)-6, 0) )
						{
							if( EditorInputEvent.usKeyState & CTRL_DOWN )
								eInfo.sScrollIndex = __min(eInfo.sScrollIndex + 60, (eInfo.sNumItems+1)/2-6);
							else
								eInfo.sScrollIndex++;
							
							EnableButton( iEditorButton[ITEMS_LEFTSCROLL] );
							if( eInfo.sScrollIndex == max( ((eInfo.sNumItems+1)/2)-6, 0) )
								DisableButton( iEditorButton[ITEMS_RIGHTSCROLL] );
						}
					}
					gfRenderTaskbar = TRUE;
					break;
				case '<':
					// item left scroll by page
					if( iCurrentTaskbar == TASK_ITEMS || gubCurrMercMode == MERC_GETITEMMODE )
					{
						if( eInfo.sScrollIndex )
						{
							eInfo.sScrollIndex = __max(eInfo.sScrollIndex - 6, 0);
							
							if( !eInfo.sScrollIndex )
								DisableButton( iEditorButton[ITEMS_LEFTSCROLL] );
							if( eInfo.sScrollIndex < ((eInfo.sNumItems+1)/2)-6 )
								EnableButton( iEditorButton[ITEMS_RIGHTSCROLL] );
						}
					}
					gfRenderTaskbar = TRUE;
					break;
				case '>':
					// item right scroll by page
					if( iCurrentTaskbar == TASK_ITEMS || gubCurrMercMode == MERC_GETITEMMODE )
					{
						if( eInfo.sScrollIndex < max( ((eInfo.sNumItems+1)/2)-6, 0) )
						{
							eInfo.sScrollIndex = __min(eInfo.sScrollIndex + 6, (eInfo.sNumItems+1)/2-6);
							
							EnableButton( iEditorButton[ITEMS_LEFTSCROLL] );
							if( eInfo.sScrollIndex == max( ((eInfo.sNumItems+1)/2)-6, 0) )
								DisableButton( iEditorButton[ITEMS_RIGHTSCROLL] );
						}
					}
					gfRenderTaskbar = TRUE;
					break;
			}
		}
		*/
	}
}



//----------------------------------------------------------------------------------------------
//	PerformSelectedAction
//
//	Perform the current user selected action, if any (or at least set things up for doing that)
//
UINT32 PerformSelectedAction( void )
{
	UINT32 uiRetVal;

	uiRetVal = EDIT_SCREEN;

	switch ( iCurrentAction )
	{
		case ACTION_DENSITY_DOWN:
			DecreaseSelectionDensity();
			break;
		case ACTION_DENSITY_UP:
			IncreaseSelectionDensity();
			break;

		case ACTION_ERASE_WAYPOINT:
			EraseMercWaypoint();
			break;

		case ACTION_SET_WAYPOINT:
			iMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );
			AddMercWaypoint( iMapIndex );
			break;

		case ACTION_RAISE_LAND:
			RaiseWorldLand();
			break;

		case ACTION_HELPSCREEN:
			DisableEditorTaskbar( );
			fHelpScreen = TRUE;
			fAllDone = FALSE;
			break;

		case ACTION_QUICK_ERASE:
			if ( (gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA) && GetMouseXY( &sGridX, &sGridY ) )
			{
				iMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );

				if ( iMapIndex < 0x80000000 )
				{
					QuickEraseMapTile( iMapIndex );
				}
			}
			break;

		case ACTION_RADAR_MAP://dnl ch9 071009 //dnl ch78 271113
			ScreenMsg(FONT_MCOLOR_WHITE, MSG_INTERFACE, pPerformSelectedActionText[0], gubFilename);
			fBuildingShowRoofs = TRUE;
			UpdateRoofsView();
			fBuildingShowWalls = TRUE;
			UpdateWallsView();
			fBuildingShowRoomInfo = FALSE;
			MarkWorldDirty();
			return(MAPUTILITY_SCREEN);
			break;

		case ACTION_QUIT_GAME:
			//gfProgramIsRunning = FALSE;//dnl ch75 251013 throw exception on exit without showing message box
		case ACTION_EXIT_EDITOR:
			if( EditModeShutdown( ) )
			{
				iPrevJA2ToolbarState = iEditorToolbarState;
				iPrevDrawMode = iDrawMode;
				PrevCurrentPaste = CurrentPaste;
				gPrevCurrentBackground = gCurrentBackground;
				fFirstTimeInGameScreen = TRUE;
				return GAME_SCREEN;
			}
			return EDIT_SCREEN;
		case ACTION_GET_WALL:
		case ACTION_GET_DOOR:
		case ACTION_GET_WINDOW:
		case ACTION_GET_ROOF:
		case ACTION_GET_NEW_ROOF:
		case ACTION_GET_BROKEN_WALL:
		case ACTION_GET_DECOR:
		case ACTION_GET_DECAL:
		case ACTION_GET_FLOOR:
		case ACTION_GET_TOILET:
		case ACTION_GET_ROOM:
			switch( iCurrentAction )
			{
				case ACTION_GET_WALL:				CreateJA2SelectionWindow( SELWIN_SINGLEWALL );			break;
				case ACTION_GET_DOOR:				CreateJA2SelectionWindow( SELWIN_SINGLEDOOR );			break;
				case ACTION_GET_WINDOW:			CreateJA2SelectionWindow( SELWIN_SINGLEWINDOW );		break;
				case ACTION_GET_ROOF:				CreateJA2SelectionWindow( SELWIN_SINGLEROOF );			break;
				case ACTION_GET_NEW_ROOF:		CreateJA2SelectionWindow( SELWIN_SINGLENEWROOF );		break;
				case ACTION_GET_BROKEN_WALL:CreateJA2SelectionWindow( SELWIN_SINGLEBROKENWALL );break;
				case ACTION_GET_DECOR:			CreateJA2SelectionWindow( SELWIN_SINGLEDECOR );			break;
				case ACTION_GET_DECAL:			CreateJA2SelectionWindow( SELWIN_SINGLEDECAL );			break;
				case ACTION_GET_FLOOR:			CreateJA2SelectionWindow( SELWIN_SINGLEFLOOR );			break;
				case ACTION_GET_TOILET:			CreateJA2SelectionWindow( SELWIN_SINGLETOILET );		break;
				case ACTION_GET_ROOM:				CreateJA2SelectionWindow( SELWIN_ROOM );						break;
			}
			fSelectionWindow = TRUE;
			fAllDone = FALSE;
			break;

		case ACTION_NEW_MAP:
			ExtractAndUpdateOptions();//dnl ch52 091009
			fNewMap = TRUE;
			if( gfPendingBasement )
				CreateMessageBox( pPerformSelectedActionText[1] );
			else if( gfPendingCaves )
				CreateMessageBox( pPerformSelectedActionText[2] );
			else
				CreateMessageBox( pPerformSelectedActionText[3] );
			break;

		case ACTION_SET_NEW_BACKGROUND:
			if (!fBeenWarned)
				CreateMessageBox( pPerformSelectedActionText[4] );
			else
			{
				gCurrentBackground = TerrainBackgroundTile;
				SetBackgroundTexture( );
				fBeenWarned = FALSE;
			}
			break;

		case ACTION_SUB_INDEX_UP:
			gusSelectionType = SMALLSELECTION;
			switch ( iDrawMode )
			{
				case DRAW_MODE_BANKS:
					if ( iNumBanksSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumBanksSelected;
					break;
				case DRAW_MODE_ROADS:
					if ( iNumRoadsSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumRoadsSelected;
					break;
				case DRAW_MODE_WALLS:
					if ( iNumWallsSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumWallsSelected;
					break;
				case DRAW_MODE_DOORS:
					if ( iNumDoorsSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumDoorsSelected;
					break;
				case DRAW_MODE_WINDOWS:
					if ( iNumWindowsSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumWindowsSelected;
					break;
				case DRAW_MODE_ROOFS:
					if( iNumRoofsSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumRoofsSelected;
					break;
				case DRAW_MODE_BROKEN_WALLS:
					if( iNumBrokenWallsSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumBrokenWallsSelected;
					break;
				case DRAW_MODE_DECOR:
					if ( iNumDecorSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumDecorSelected;
					break;
				case DRAW_MODE_DECALS:
					if ( iNumDecalsSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumDecalsSelected;
					break;
				case DRAW_MODE_FLOORS:
					if( iNumFloorsSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumFloorsSelected;
					break;
				case DRAW_MODE_TOILET:
					if( iNumToiletsSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumToiletsSelected;
					break;
				case DRAW_MODE_NEWROOF:
					if( iNumNewRoofsSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumNewRoofsSelected;
					break;
				case DRAW_MODE_OSTRUCTS:
					fDontUseRandom = TRUE;
					if ( iNumOStructsSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumOStructsSelected;
					break;

				case DRAW_MODE_OSTRUCTS1:
					fDontUseRandom = TRUE;
					if ( iNumOStructs1Selected > 0 )
						iCurBank = (iCurBank + 1) % iNumOStructs1Selected;
					break;

				case DRAW_MODE_OSTRUCTS2:
					fDontUseRandom = TRUE;
					if ( iNumOStructs2Selected > 0 )
						iCurBank = (iCurBank + 1) % iNumOStructs2Selected;
					break;

				case DRAW_MODE_DEBRIS:
					fDontUseRandom = TRUE;
					if ( iNumDebrisSelected > 0 )
						iCurBank = (iCurBank + 1) % iNumDebrisSelected;
					break;
			}
			break;

		case ACTION_SUB_INDEX_DWN:
			gusSelectionType = SMALLSELECTION;
			switch ( iDrawMode )
			{
				case DRAW_MODE_BANKS:
					if ( iNumBanksSelected > 0 )
						iCurBank = (iCurBank + (iNumBanksSelected - 1)) % iNumBanksSelected;
					break;
				case DRAW_MODE_ROADS:
					if ( iNumRoadsSelected > 0 )
						iCurBank = (iCurBank + (iNumRoadsSelected - 1)) % iNumRoadsSelected;
					break;
				case DRAW_MODE_WALLS:
					if ( iNumWallsSelected > 0 )
						iCurBank = (iCurBank + (iNumWallsSelected - 1)) % iNumWallsSelected;
					break;
				case DRAW_MODE_DOORS:
					if ( iNumDoorsSelected > 0 )
						iCurBank = (iCurBank + (iNumDoorsSelected - 1)) % iNumDoorsSelected;
					break;
				case DRAW_MODE_WINDOWS:
					if ( iNumWindowsSelected > 0 )
						iCurBank = (iCurBank + (iNumWindowsSelected - 1)) % iNumWindowsSelected;
					break;
				case DRAW_MODE_ROOFS:
					if( iNumRoofsSelected > 0 )
						iCurBank = (iCurBank + (iNumRoofsSelected - 1)) % iNumRoofsSelected;
					break;
				case DRAW_MODE_BROKEN_WALLS:
					if ( iNumBrokenWallsSelected > 0 )
						iCurBank = (iCurBank + (iNumBrokenWallsSelected - 1)) % iNumBrokenWallsSelected;
					break;
				case DRAW_MODE_DECOR:
					if ( iNumDecorSelected > 0 )
						iCurBank = (iCurBank + (iNumDecorSelected - 1)) % iNumDecorSelected;
					break;
				case DRAW_MODE_DECALS:
					if ( iNumDecalsSelected > 0 )
						iCurBank = (iCurBank + (iNumDecalsSelected - 1)) % iNumDecalsSelected;
					break;
				case DRAW_MODE_FLOORS:
					if( iNumFloorsSelected > 0 )
						iCurBank = (iCurBank + (iNumFloorsSelected - 1)) % iNumFloorsSelected;
					break;
				case DRAW_MODE_TOILET:
					if( iNumToiletsSelected > 0 )
						iCurBank = (iCurBank + (iNumToiletsSelected - 1)) % iNumToiletsSelected;
					break;
				case DRAW_MODE_NEWROOF:
					if( iNumNewRoofsSelected > 0 )
						iCurBank = (iCurBank + (iNumNewRoofsSelected - 1)) % iNumNewRoofsSelected;
					break;
				case DRAW_MODE_OSTRUCTS:
					fDontUseRandom = TRUE;
					if ( iNumOStructsSelected > 0 )
						iCurBank = (iCurBank + (iNumOStructsSelected - 1)) % iNumOStructsSelected;
					break;

				case DRAW_MODE_OSTRUCTS1:
					fDontUseRandom = TRUE;
					if ( iNumOStructs1Selected > 0 )
						iCurBank = (iCurBank + (iNumOStructs1Selected - 1)) % iNumOStructs1Selected;
					break;

				case DRAW_MODE_OSTRUCTS2:
					fDontUseRandom = TRUE;
					if ( iNumOStructs2Selected > 0 )
						iCurBank = (iCurBank + (iNumOStructs2Selected - 1)) % iNumOStructs2Selected;
					break;

				case DRAW_MODE_DEBRIS:
					fDontUseRandom = TRUE;
					if ( iNumDebrisSelected > 0 )
						iCurBank = (iCurBank + (iNumDebrisSelected - 1)) % iNumDebrisSelected;
					break;
			}
			break;
		case ACTION_NEXT_FGRND:
			break;

		case ACTION_NEXT_DEBRIS:
			CreateJA2SelectionWindow( SELWIN_DEBRIS );
			fSelectionWindow = TRUE;
			fAllDone = FALSE;
			break;

		case ACTION_PREV_SELECTIONTYPE:
			gusSelectionType--;
			if( gusSelectionType > AREASELECTION )
				gusSelectionType = AREASELECTION;
			gfRenderTaskbar = TRUE;
			break;
		case ACTION_NEXT_SELECTIONTYPE:
			gusSelectionType++;
			if( gusSelectionType > AREASELECTION )
				gusSelectionType = SMALLSELECTION;
			gfRenderTaskbar = TRUE;
			break;

		case ACTION_COPY_MERC_PLACEMENT:
			GetMouseXY( &sGridX, &sGridY );
			iMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );
			CopyMercPlacement( iMapIndex );
			break;
		case ACTION_PASTE_MERC_PLACEMENT:
			GetMouseXY( &sGridX, &sGridY );
			iMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );
			PasteMercPlacement( iMapIndex );
			break;

		case ACTION_SAVE_MAP:
			UpdateLastActionBeforeLeaving();
			return LOADSAVE_SCREEN;

		case ACTION_LOAD_MAP:
			UpdateLastActionBeforeLeaving();
			if(iCurrentTaskbar == TASK_OPTIONS)//dnl ch52 091009
				ExtractAndUpdateOptions();
			return(LOADSAVE_SCREEN);

		case ACTION_UNDO://dnl ch86 240213
			ExecuteUndoList();
			UpdateRoofsView();
			UpdateWallsView();
			gfRenderWorld = TRUE;
			break;

		case ACTION_NEXT_BANK:
			CreateJA2SelectionWindow( SELWIN_BANKS );
			fSelectionWindow = TRUE;
			fAllDone = FALSE;
			break;

		case ACTION_NEXT_ROAD:
			CreateJA2SelectionWindow( SELWIN_ROADS );
			fSelectionWindow = TRUE;
			fAllDone = FALSE;
			break;

		case ACTION_SHADE_UP:
			if ( EditorInputEvent.usKeyState & SHIFT_DOWN )
			{
				gShadePercent+= (FLOAT).05;
			}
			else
			{
				gShadePercent+= (FLOAT).01;
			}

			if ( gShadePercent > 1 )
			{
				gShadePercent = (FLOAT)0;
			}
			SetShadeTablePercent( gShadePercent );
			break;

		case ACTION_SHADE_DWN:
			if ( EditorInputEvent.usKeyState & SHIFT_DOWN )
			{
				gShadePercent-= (FLOAT).05;
			}
			else
			{
				gShadePercent-= (FLOAT).01;
			}

			if ( gShadePercent < 0 )
			{
				gShadePercent = (FLOAT)1;
			}
			SetShadeTablePercent( gShadePercent );
			break;

		case ACTION_WALL_PASTE1:	// Doors		//** Changes needed
			GetMouseXY( &sGridX, &sGridY );
			iMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );

			AddWallToStructLayer( iMapIndex, FIRSTWALL18, TRUE );
			break;

		case ACTION_WALL_PASTE2:	// Windows	//** Changes Needed
			GetMouseXY( &sGridX, &sGridY );
			iMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );

			AddWallToStructLayer( iMapIndex, FIRSTWALL19, TRUE );
			break;

		case ACTION_NEXT_STRUCT:
			CreateJA2SelectionWindow( SELWIN_OSTRUCTS );
			fSelectionWindow = TRUE;
			fAllDone = FALSE;
			break;

		case ACTION_NEXT_STRUCT1:
			CreateJA2SelectionWindow( SELWIN_OSTRUCTS1 );
			fSelectionWindow = TRUE;
			fAllDone = FALSE;
			break;

		case ACTION_NEXT_STRUCT2:
			CreateJA2SelectionWindow( SELWIN_OSTRUCTS2 );
			fSelectionWindow = TRUE;
			fAllDone = FALSE;
			break;

		default:
			break;
	}

	return( uiRetVal );
}

void CreateNewMap()
{
	if( gfSummaryWindowActive )
		DestroySummaryWindow();

	if( !gfWorldLoaded )
		LoadMapTileset( 0 );

	LightReset();
	HideEntryPoints();

	NewWorld( iNewMapWorldRows, iNewMapWorldCols );

	//dnl ch54 101009
	memset(&gMapInformation, 0, sizeof(MAPCREATE_STRUCT));
	gMapInformation.sNorthGridNo = NOWHERE;
	gMapInformation.sSouthGridNo = NOWHERE;
	gMapInformation.sWestGridNo = NOWHERE;
	gMapInformation.sEastGridNo = NOWHERE;
	gMapInformation.sIsolatedGridNo = NOWHERE;
	gMapInformation.sCenterGridNo = MAPROWCOLTOPOS(WORLD_ROWS/2, WORLD_COLS/2);
	gMapInformation.ubMapVersion = gubMinorMapVersion;

	if( gfPendingBasement )
	{
		INT32 i;
		UINT16 usIndex;
		for ( i = 0; i < WORLD_MAX; i++ )
		{
			// Set land index 9 + 3 variants
			GetTileIndexFromTypeSubIndex( FIRSTROOF, (UINT16)(9 + Random(3)), &usIndex );
			AddRoofToHead( i, usIndex );
		}
		SetEditorSmoothingMode( SMOOTHING_BASEMENT );
	}
	else if( gfPendingCaves )
	{
		INT32 i;
		UINT16 usIndex;
		for ( i = 0; i < WORLD_MAX; i++ )
		{
			//Set up the default cave here.
			GetTileIndexFromTypeSubIndex( FIRSTWALL, (UINT16)(60 + Random(6)), &usIndex );
			AddCave( i, usIndex );
		}
		SetEditorSmoothingMode( SMOOTHING_CAVES );
	}
	else
		SetEditorSmoothingMode( SMOOTHING_NORMAL );
	fNewMap = FALSE;
	RemoveAllFromUndoList( );
	UseEditorAlternateList();
	KillSoldierInitList();
	UseEditorOriginalList();
	KillSoldierInitList();
	gpItemPool = NULL;//dnl ch26 210909
	fShowHighGround = FALSE;//dnl ch2 210909
	fRaiseWorld = FALSE;//dnl ch3 210909
	ResetCustomFileSectorSummary();//dnl ch30 150909
	SetRenderCenter(WORLD_COLS/2, WORLD_ROWS/2);//dnl ch43 280909
	InitRenderParams(gMapInformation.ubRestrictedScrollID);//dnl ch54 101009

	fNewMapSaved = FALSE;
}


UINT32 ProcessEditscreenMessageBoxResponse()
{
	RemoveMessageBox();
	gfRenderWorld = TRUE;
	if( gfConfirmExitPending )
	{
		gfConfirmExitPending = FALSE;
		if( gfMessageBoxResult )
		{
			gfConfirmExitFirst = FALSE;
			iEditorToolbarState = TBAR_MODE_EXIT_EDIT;
			if(iCurrentAction == ACTION_QUIT_GAME)//dnl ch75 251013
			{
				iEditorToolbarState = TBAR_MODE_QUIT_GAME;
				gfProgramIsRunning = FALSE;
			}
		}
		return EDIT_SCREEN;
	}
	if( !gfMessageBoxResult )
		return EDIT_SCREEN;
	if( gfRemoveLightsPending )
	{
		INT32 i;
		LightReset();
		for( i = 0; i < WORLD_MAX; i++ )
		{
			RemoveAllObjectsOfTypeRange( i, GOODRING, GOODRING );
		}
		MarkWorldDirty();
		LightSetBaseLevel( (UINT8)(15 - ubAmbientLightLevel) );
		gfRemoveLightsPending = FALSE;
	}
	if( gfScheduleReversalPending )
	{
		IndicateSelectedMerc( SELECT_NO_MERC );
		ReverseSchedules();
		gfScheduleReversalPending = FALSE;
	}
	else if( gfScheduleClearPending )
	{
		IndicateSelectedMerc( SELECT_NO_MERC );
		ClearAllSchedules();
		gfScheduleClearPending = FALSE;
	}
	if( fNewMap )
	{
		CreateNewMap();
		return EDIT_SCREEN;
	}
	else if( iDrawMode == DRAW_MODE_NEW_GROUND )
	{
		gCurrentBackground = TerrainBackgroundTile;
		SetBackgroundTexture( );
		SetEditorTerrainTaskbarMode( TERRAIN_FGROUND_TEXTURES );
		return EDIT_SCREEN;
	}
	return EDIT_SCREEN;
}


//----------------------------------------------------------------------------------------------
//	WaitForHelpScreenResponse
//
//	Displays a help screen and waits for the user to wisk it away.
//
UINT32 WaitForHelpScreenResponse( void )
{
	InputAtom DummyEvent;
	BOOLEAN fLeaveScreen;

	ColorFillVideoSurfaceArea(FRAME_BUFFER,	iScreenWidthOffset + 50, iScreenHeightOffset + 50, iScreenWidthOffset + 590, iScreenHeightOffset + 310,
													Get16BPPColor(FROMRGB(136, 138, 135)) );
	ColorFillVideoSurfaceArea(FRAME_BUFFER,	iScreenWidthOffset + 51, iScreenHeightOffset + 51, iScreenWidthOffset + 590, iScreenHeightOffset + 310,
													Get16BPPColor(FROMRGB(24, 61, 81)) );
	ColorFillVideoSurfaceArea(FRAME_BUFFER,	iScreenWidthOffset + 51, iScreenHeightOffset + 51, iScreenWidthOffset + 589, iScreenHeightOffset + 309,
													GenericButtonFillColors[0]);

	SetFont( gp12PointFont1 );

	gprintf( iScreenWidthOffset + 55,  iScreenHeightOffset + 55, pWaitForHelpScreenResponseText[0] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 55, pWaitForHelpScreenResponseText[1] );

	gprintf( iScreenWidthOffset + 55, iScreenHeightOffset + 67, pWaitForHelpScreenResponseText[2] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 67, pWaitForHelpScreenResponseText[3] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 79, pWaitForHelpScreenResponseText[4] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 79, pWaitForHelpScreenResponseText[5] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 91, pWaitForHelpScreenResponseText[6] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 91, pWaitForHelpScreenResponseText[7] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 103, pWaitForHelpScreenResponseText[8] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 103, pWaitForHelpScreenResponseText[9] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 115, pWaitForHelpScreenResponseText[10] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 115, pWaitForHelpScreenResponseText[11] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 127, pWaitForHelpScreenResponseText[12] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 127, pWaitForHelpScreenResponseText[13] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 139, pWaitForHelpScreenResponseText[14] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 139, pWaitForHelpScreenResponseText[15] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 151, pWaitForHelpScreenResponseText[16] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 151, pWaitForHelpScreenResponseText[17] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 163, pWaitForHelpScreenResponseText[18] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 163, pWaitForHelpScreenResponseText[19] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 175, pWaitForHelpScreenResponseText[20] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 175, pWaitForHelpScreenResponseText[21] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 187, pWaitForHelpScreenResponseText[22] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 187, pWaitForHelpScreenResponseText[23] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 199, pWaitForHelpScreenResponseText[24] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 199, pWaitForHelpScreenResponseText[25] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 211, pWaitForHelpScreenResponseText[26] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 211, pWaitForHelpScreenResponseText[27] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 223, pWaitForHelpScreenResponseText[28] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 223, pWaitForHelpScreenResponseText[29] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 235, pWaitForHelpScreenResponseText[30] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 235, pWaitForHelpScreenResponseText[31] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 247, pWaitForHelpScreenResponseText[32] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 247, pWaitForHelpScreenResponseText[33] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 259, pWaitForHelpScreenResponseText[34] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 259, pWaitForHelpScreenResponseText[35] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 271, pWaitForHelpScreenResponseText[36] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 271, pWaitForHelpScreenResponseText[37] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 283, pWaitForHelpScreenResponseText[38] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 283, pWaitForHelpScreenResponseText[39] );

	gprintf( iScreenWidthOffset + 55,	iScreenHeightOffset + 295, pWaitForHelpScreenResponseText[40] );
	gprintf( iScreenWidthOffset + 205, iScreenHeightOffset + 295, pWaitForHelpScreenResponseText[41] );


	fLeaveScreen = FALSE;

	while (DequeueEvent(&DummyEvent) == TRUE)
	{
		if ( DummyEvent.usEvent == KEY_DOWN )
		{
			switch( DummyEvent.usParam )
			{
				case SPACE:
				case ESC:
				case ENTER:
				case F1:
					fLeaveScreen = TRUE;
					break;
			}
		}
	}


	if ( (_LeftButtonDown) || (_RightButtonDown) || fLeaveScreen )
	{
		fHelpScreen = FALSE;

		while( DequeueEvent( &DummyEvent ) )
			continue;

		EnableEditorTaskbar();
	}

	InvalidateScreen( );
	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();

	return( EDIT_SCREEN );
}



//----------------------------------------------------------------------------------------------
//	WaitForSelectionWindowResponse
//
//	Handles all keyboard input and display for a selection window.
//
UINT32 WaitForSelectionWindowResponse( void )
{
	InputAtom DummyEvent;

	while (DequeueEvent(&DummyEvent) == TRUE)
	{
		if ( DummyEvent.usEvent == KEY_DOWN )
		{
			switch( DummyEvent.usParam )
			{
				case SPACE:
					ClearSelectionList();
					break;

				case DNARROW:
					ScrollSelWinDown();
					break;

				case UPARROW:
					ScrollSelWinUp();
					break;

				case ESC:
					RestoreSelectionList();
				case ENTER:
					fAllDone = TRUE;
					break;
			}
		}
	}

	// Mousewheel scroll
	if (_WheelValue > 0)
	{
		while (_WheelValue--)
		{
			ScrollSelWinUp();
		}
	}
	else
	{
		while (_WheelValue++)
		{
			ScrollSelWinDown();
		}
	}
	_WheelValue = 0;

	if ( DoWindowSelection( ) )
	{
		fSelectionWindow = FALSE;
		ShutdownJA2SelectionWindow( );
		// Quick hack to trash the mouse event queue.
		while( DequeueEvent( &DummyEvent ) )
			continue;

		iCurBank = 0;

		if( iDrawMode == DRAW_MODE_SLANTED_ROOF || iDrawMode == DRAW_MODE_ROOM )
		{
			if( CheckForSlantRoofs() )
				iDrawMode = DRAW_MODE_SLANTED_ROOF;
		}
		InvalidateScreen( );
		ExecuteBaseDirtyRectQueue();
		EndFrameBufferRender( );
	}
	else
	{
		DisplaySelectionWindowGraphicalInformation();
		InvalidateScreen( );
		ExecuteBaseDirtyRectQueue();
		EndFrameBufferRender( );
	}

	return( EDIT_SCREEN );
}



//----------------------------------------------------------------------------------------------
//	FindTilesetComments
//
//	Retrieves the file comments from the master list for the current tileset
//
void FindTilesetComments(void)
{
}


//----------------------------------------------------------------------------------------------
//	GetMasterList
//
//	Loads the master list for the tileset comments
//
void GetMasterList(void)
{
}


//----------------------------------------------------------------------------------------------
//	ShowCurrentSlotSurface
//
//	Displays the image of the currently highlighted tileset slot if it's a video surface.
//	(usually a 16 bit image)
//
void ShowCurrentSlotSurface( UINT32 vSurface, INT32 iWindow )
{
	SGPRect			ClipRect, WinRect;
	INT32				iStartX;
	INT32				iStartY;
	INT32				iPicHeight, iPicWidth;
	HVSURFACE		hvSurface;
	INT32				iWinWidth, iWinHeight;
	blt_vs_fx		vSfx;

	WinRect.iLeft = (iWindow == 0) ? (iScreenWidthOffset + 336) : (iScreenWidthOffset + 488);
	WinRect.iTop = 2 * iScreenHeightOffset + 211;
	WinRect.iRight = (iWindow == 0) ? (iScreenWidthOffset + 485) : (iScreenWidthOffset + 637);
	WinRect.iBottom = 2 * iScreenHeightOffset + 399;

	ColorFillVideoSurfaceArea(FRAME_BUFFER, WinRect.iLeft - 1, WinRect.iTop - 1,
																					WinRect.iRight + 1, WinRect.iBottom + 1,
																					Get16BPPColor(FROMRGB(128, 0, 0)) );

	iWinWidth = WinRect.iRight - WinRect.iLeft;
	iWinHeight = WinRect.iBottom - WinRect.iTop;

	GetVideoSurface( &hvSurface, vSurface );

	iPicWidth = (INT32)hvSurface->usWidth;
	iPicHeight = (INT32)hvSurface->usHeight;

	if ( iPicWidth > iWinWidth )
	{
		ClipRect.iLeft = (iPicWidth - iWinWidth) / 2;
		ClipRect.iRight = ClipRect.iLeft + iWinWidth - 1;
		iStartX = WinRect.iLeft;
	}
	else
	{
		ClipRect.iLeft = 0;
		ClipRect.iRight = iPicWidth - 1;
		iStartX = ((iWinWidth - iPicWidth) / 2) + WinRect.iLeft;
	}

	if ( iPicHeight > iWinHeight )
	{
		ClipRect.iTop = (iPicHeight - iWinHeight) / 2;
		ClipRect.iBottom = ClipRect.iTop + iWinHeight - 1;
		iStartY = WinRect.iTop;
	}
	else
	{
		ClipRect.iTop = 0;
		ClipRect.iBottom = iPicHeight - 1;
		iStartY = ((iWinHeight - iPicHeight) / 2) + WinRect.iTop;
	}

	vSfx.SrcRect = ClipRect;
	BltVideoSurface( FRAME_BUFFER, vSurface, 0,
																iStartX, iStartY,
																VS_BLT_SRCSUBRECT, &vSfx );
}


//----------------------------------------------------------------------------------------------
//	ShowCurrentSlotImage
//
//	Displays the image of the currently highlighted tileset slot image. Usually this is for
//	8 bit image (.STI) files
//
void ShowCurrentSlotImage( HVOBJECT hVObj, INT32 iWindow )
{
	SGPRect			ClipRect, NewRect;
	INT32				iStartX;
	INT32				iStartY;
	INT32				iPicHeight, iPicWidth;
	INT16				sTempOffsetX;
	INT16				sTempOffsetY;
	ETRLEObject *pETRLEObject;
	INT32				iWinWidth, iWinHeight;

	NewRect.iLeft = (iWindow == 0) ? (iScreenWidthOffset + 336) : (iScreenWidthOffset + 488);
	NewRect.iTop = 2 * iScreenHeightOffset + 211;
	NewRect.iRight = (iWindow == 0) ? (iScreenWidthOffset + 485) : (iScreenWidthOffset + 637);
	NewRect.iBottom = 2 * iScreenHeightOffset + 399;

	iWinWidth = NewRect.iRight - NewRect.iLeft;
	iWinHeight = NewRect.iBottom - NewRect.iTop;

	GetClippingRect(&ClipRect);
	SetClippingRect(&NewRect);

	pETRLEObject = &(hVObj->pETRLEObject[0]);

	iPicWidth = (INT32)pETRLEObject->usWidth;
	iPicHeight = (INT32)pETRLEObject->usHeight;

	iStartX = ( (iWinWidth - iPicWidth) / 2 ) + NewRect.iLeft;
	iStartY = ( (iWinHeight - iPicHeight) / 2 ) + NewRect.iTop;

	// We have to store the offset data in temp variables before zeroing them and blitting
	sTempOffsetX = pETRLEObject->sOffsetX;
	sTempOffsetY = pETRLEObject->sOffsetY;

	// Set the offsets used for blitting to 0
	pETRLEObject->sOffsetX = 0;
	pETRLEObject->sOffsetY = 0;

	SetObjectShade( hVObj, DEFAULT_SHADE_LEVEL );
	BltVideoObject( FRAME_BUFFER, hVObj, 0, (iStartX), (iStartY),
								VO_BLT_SRCTRANSPARENCY, NULL);

	pETRLEObject->sOffsetX = sTempOffsetX;
	pETRLEObject->sOffsetY = sTempOffsetY;

	SetClippingRect(&ClipRect);
}

//----------------------------------------------------------------------------------------------
//	PlaceLight
//
//	Creates and places a light of selected radius and color into the world.
//
BOOLEAN PlaceLight( INT16 sRadius, INT16 iMapX, INT16 iMapY, INT16 sType, INT8 bLightType )//dnl ch86 210214
{
	INT32 iLightHandle;
	UINT8 ubIntensity;
	STRING512 Filename;
	INT32 iMapIndex;
	UINT16 usTmpIndex;

	sprintf( Filename, "L-R%02d.LHT", sRadius );

	// Attempt to create light
	if ( (iLightHandle = LightSpriteCreate( Filename, sType )) == (-1) )
	{
		// Couldn't load file because it doesn't exist. So let's make the file
		ubIntensity = (UINT8)((float)sRadius / LIGHT_DECAY);
		if ( (iLightHandle = LightCreateOmni( ubIntensity, sRadius )) == (-1) )
		{
			// Can't create light template
			DebugMsg(TOPIC_GAME, DBG_LEVEL_1, String("PlaceLight: Can't create light template for radius %d",sRadius) );
			return( FALSE );
		}

		if ( !LightSave( iLightHandle, Filename ) )
		{
			// Can't save light template
			DebugMsg(TOPIC_GAME, DBG_LEVEL_1, String("PlaceLight: Can't save light template for radius %d",sRadius) );
			return( FALSE );
		}

		if ( (iLightHandle = LightSpriteCreate( Filename, sType )) == (-1) )
		{
			// Can't create sprite
			DebugMsg(TOPIC_GAME, DBG_LEVEL_1, String("PlaceLight: Can't create light sprite of radius %d",sRadius) );
			return( FALSE );
		}
	}

	if ( !LightSpritePower( iLightHandle, TRUE ) )
	{
		// Can't turn this light on
		DebugMsg(TOPIC_GAME, DBG_LEVEL_1, String("PlaceLight: Can't turn on light %d",iLightHandle) );
		return( FALSE );
	}

	if ( !LightSpritePosition( iLightHandle, iMapX, iMapY ) )
	{
		// Can't set light's position
		DebugMsg(TOPIC_GAME, DBG_LEVEL_1, String("PlaceLight: Can't set light position for light %d",iLightHandle) );
		return( FALSE );
	}

	switch( bLightType )//dnl ch86 210214
	{
		case PRIMETIME_LIGHT:
			LightSprites[ iLightHandle ].uiFlags |= LIGHT_PRIMETIME;
			break;
		case NIGHTTIME_LIGHT:
			LightSprites[ iLightHandle ].uiFlags |= LIGHT_NIGHTTIME;
			break;
	}

	iMapIndex = ((INT32)iMapY * WORLD_COLS) + (INT32)iMapX;
	if ( !TypeExistsInObjectLayer( iMapIndex, GOODRING, &usTmpIndex ) )
	{
		AddObjectToHead( iMapIndex, GOODRING1 );
		gpWorldLevelData[ iMapIndex ].pObjectHead->ubShadeLevel = DEFAULT_SHADE_LEVEL;
	}

	//AddLightToUndoList( iMapIndex, 0, 0 );//dnl ch86 200214

	return( TRUE );
}


//----------------------------------------------------------------------------------------------
//	RemoveLight
//
//	Removes (erases) all lights at a given map tile location. Lights that are attached to a merc
//	are not deleted.
//
//	Returns TRUE if deleted the light, otherwise, returns FALSE.
//	i.e. FALSE is not an error condition!
//
BOOLEAN RemoveLight(INT16 iMapX, INT16 iMapY, INT8 bLightType)
{
	INT32 iCount, iMapIndex;
	UINT8 ubLightRadius, ubLightId;
	BOOLEAN fRemovedLight = FALSE;

	iMapIndex = FASTMAPROWCOLTOPOS(iMapY, iMapX);
	while(FindLight(iMapIndex, bLightType, &ubLightRadius, &ubLightId, &iCount))
	{
		LightSpritePower(iCount, FALSE);
		LightSpriteDestroy(iCount);
		fRemovedLight = TRUE;
	}
	if(fRemovedLight && !FindLight(iMapIndex, ANY_LIGHT, &ubLightRadius, &ubLightId))
		RemoveAllObjectsOfTypeRange(iMapIndex, GOODRING, GOODRING);
	return(fRemovedLight);
}

//----------------------------------------------------------------------------------------------
//	ShowLightPositionHandles
//
//	For all lights that are in the world (except lights attached to mercs), this function places
//	a marker at it's location for editing purposes.
//
void ShowLightPositionHandles( void )
{
	INT32 iCount;
	INT32 iMapIndex;
	UINT16 cnt;
	UINT16 usTmpIndex;
	SOLDIERTYPE *pSoldier;
	BOOLEAN fSoldierLight;

	// Check all lights and place a position handle there!
	for(iCount=0; iCount < MAX_LIGHT_SPRITES; iCount++)
	{
		if(LightSprites[iCount].uiFlags & LIGHT_SPR_ACTIVE)
		{
			// Found a light, so let's see if it belong to a merc!
			fSoldierLight = FALSE;
			for ( cnt = 0; cnt < MAX_NUM_SOLDIERS && !fSoldierLight; cnt++ )
			{
				if ( GetSoldier( &pSoldier, cnt ) )
				{
					if ( pSoldier->iLight == iCount )
						fSoldierLight = TRUE;
				}
			}

			if ( !fSoldierLight )
			{
 				iMapIndex = ((INT32)LightSprites[iCount].iY * WORLD_COLS) + (INT32)LightSprites[iCount].iX;
				if ( !TypeExistsInObjectLayer( iMapIndex, GOODRING, &usTmpIndex ) )
				{
					AddObjectToHead( iMapIndex, GOODRING1 );
					gpWorldLevelData[ iMapIndex ].pObjectHead->ubShadeLevel = DEFAULT_SHADE_LEVEL;
				}
			}
		}
	}
}


//----------------------------------------------------------------------------------------------
//	RemoveLightPositionHandles
//
//	Scans through all light currently in the world and removes any light markers that may be present.
//
void RemoveLightPositionHandles( void )
{
	INT32 iCount;
	INT32 iMapIndex;
	UINT16 cnt;
	SOLDIERTYPE *pSoldier;
	BOOLEAN fSoldierLight;

	// Check all lights and remove the position handle there!
	for(iCount=0; iCount < MAX_LIGHT_SPRITES; iCount++)
	{
		if(LightSprites[iCount].uiFlags & LIGHT_SPR_ACTIVE)
		{
			// Found a light, so let's see if it belong to a merc!
			fSoldierLight = FALSE;
			for ( cnt = 0; cnt < MAX_NUM_SOLDIERS && !fSoldierLight; cnt++ )
			{
				if ( GetSoldier( &pSoldier, cnt ) )
				{
					if ( pSoldier->iLight == iCount )
						fSoldierLight = TRUE;
				}
			}

			if ( !fSoldierLight )
			{
 				iMapIndex = ((INT32)LightSprites[iCount].iY * WORLD_COLS) + (INT32)LightSprites[iCount].iX;
				RemoveAllObjectsOfTypeRange( iMapIndex, GOODRING, GOODRING );
			}
		}
	}
}


//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	End of button callback functions
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------
//	CheckForSlantRoofs
//
//	Verifies whether there are any slant roofs selected in the "multiwall" or "room" selection list.
//	if so, and no flat roofs were selected, then we return TRUE, otherwise we return FALSE.
//
//	This function is used to determine if we should force the area selection of rooms to handle
//	slant roofs (which require at least one side to be exactly 8 tiles wide)
//
BOOLEAN CheckForSlantRoofs( void )
{
	UINT16 usCheck;

	pSelList = SelRoom;
	pNumSelList = &iNumRoomsSelected;

	usCheck = GetRandomIndexByRange( FIRSTROOF, LASTROOF );
	if ( usCheck != 0xffff )
		return( FALSE );

	usCheck = GetRandomIndexByRange( FIRSTSLANTROOF, LASTSLANTROOF );
	return ( usCheck != 0xffff );
}



//----------------------------------------------------------------------------------------------
//	CheckForFences
//
//	Verifies whether the selection list has only fence pieces. If so, turns off random selection
//	for that selection list.
//
//	if ANY piece other than a fence piece exists in the selection list, then we want to keep
//	random selections.
//
BOOLEAN CheckForFences( void )
{
	UINT16 usCheck;
	BOOLEAN fFence;
	TILE_ELEMENT *T;

	pSelList = SelOStructs2;
	pNumSelList = &iNumOStructs2Selected;

	fFence = TRUE;

	for ( usCheck = 0; usCheck < iNumOStructs2Selected; usCheck++ )
	{
		T = &gTileDatabase[gTileTypeStartIndex[pSelList[usCheck].uiObject]];
		if ( T->pDBStructureRef == NULL )
			fFence = FALSE;
		else if ( !(T->pDBStructureRef->pDBStructure->fFlags & STRUCTURE_ANYFENCE) )
			fFence = FALSE;
	}

	return( fFence );
}

void EnsureStatusOfEditorButtons()
{
	if ( iDrawMode >= DRAW_MODE_ERASE )
	{
		ClickEditorButton( TERRAIN_TOGGLE_ERASEMODE );
		ClickEditorButton( BUILDING_TOGGLE_ERASEMODE );
		ClickEditorButton( MAPINFO_TOGGLE_ERASEMODE );
	}
	else
	{
		UnclickEditorButton( TERRAIN_TOGGLE_ERASEMODE );
		UnclickEditorButton( BUILDING_TOGGLE_ERASEMODE );
		UnclickEditorButton( MAPINFO_TOGGLE_ERASEMODE );
	}

	//disable erase buttons if drawing caves
	if( iDrawMode == DRAW_MODE_CAVES || iDrawMode == DRAW_MODE_NEWROOF )
		DisableEditorButton( BUILDING_TOGGLE_ERASEMODE );
	else
		EnableEditorButton( BUILDING_TOGGLE_ERASEMODE );

	// Ensure that the fill button is on or off depending on our current mode
	if ( iDrawMode == (DRAW_MODE_GROUND + DRAW_MODE_FILL_AREA) )
		ClickEditorButton( TERRAIN_FILL_AREA );
	else
		UnclickEditorButton( TERRAIN_FILL_AREA );

	// Make sure that the fake-lighting button is down if appropriate
	if ( gfFakeLights )
		ClickEditorButton( MAPINFO_TOGGLE_FAKE_LIGHTS );
	else
		UnclickEditorButton( MAPINFO_TOGGLE_FAKE_LIGHTS );
}

#ifdef OLD_HandleMouseClicksInGameScreen//dnl ch7 210909
void HandleMouseClicksInGameScreen()
{
	INT16 sX, sY;
	BOOLEAN fPrevState;
	if( !GetMouseXY( &sGridX, &sGridY ) )
		return;
	if( iCurrentTaskbar == TASK_OPTIONS || iCurrentTaskbar == TASK_NONE )
	{ //if in taskbar modes which don't process clicks in the world.
		return;
	}
	if( !(gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA ) )
	{	//if mouse cursor not in the game screen.
		return;
	}

	iMapIndex = MAPROWCOLTOPOS( sGridY, sGridX );

	fPrevState = gfRenderWorld;

	if( _LeftButtonDown )
	{
		gfRenderWorld = TRUE;
		// Are we trying to erase something?
		if ( iDrawMode >= DRAW_MODE_ERASE )
		{
			// Erasing can have a brush size larger than 1 tile
			for ( sY = (INT16)gSelectRegion.iTop; sY <= (INT16)gSelectRegion.iBottom; sY++ )
			{
				for ( sX = (INT16)gSelectRegion.iLeft; sX <= (INT16)gSelectRegion.iRight; sX++ )
				{
					if ( iDrawMode == (DRAW_MODE_LIGHT + DRAW_MODE_ERASE) )
					{
						RemoveLight( sX, sY );
					}
					else
					EraseMapTile( MAPROWCOLTOPOS( sY, sX ) );
				}
			}

			if( iDrawMode == DRAW_MODE_LIGHT + DRAW_MODE_ERASE )
				LightSpriteRenderAll();	// To adjust building's lighting
			return;
		}

		switch ( iDrawMode )
		{
			case DRAW_MODE_SCHEDULEACTION:
				if( IsLocationSittableExcludingPeople( iMapIndex, FALSE ) )
				{
					iDrawMode = DRAW_MODE_SCHEDULECONFIRM;
					gfFirstPlacement = FALSE;
				}
				break;
			case DRAW_MODE_NORTHPOINT:
			case DRAW_MODE_WESTPOINT:
			case DRAW_MODE_EASTPOINT:
			case DRAW_MODE_SOUTHPOINT:
			case DRAW_MODE_CENTERPOINT:
			case DRAW_MODE_ISOLATEDPOINT:
				SpecifyEntryPoint( iMapIndex );
				break;

			case DRAW_MODE_ENEMY:
			case DRAW_MODE_CREATURE:
			case DRAW_MODE_REBEL:
			case DRAW_MODE_CIVILIAN:
				// Handle adding mercs to the world
				if( gfFirstPlacement )
				{
					AddMercToWorld( iMapIndex );
					gfFirstPlacement = FALSE;
				}
				break;

			case DRAW_MODE_LIGHT:
				// Add a normal light to the world
				if( gfFirstPlacement )
				{
					PlaceLight( gsLightRadius, sGridX, sGridY, 0 );
					gfFirstPlacement = FALSE;
				}
				break;

			case DRAW_MODE_SAW_ROOM:
			case DRAW_MODE_ROOM:
			case DRAW_MODE_CAVES:
				if( gusSelectionType >= SMALLSELECTION && gusSelectionType <= XLARGESELECTION )
					ProcessAreaSelection( TRUE );
				break;
			case DRAW_MODE_NEWROOF:
				ReplaceBuildingWithNewRoof( iMapIndex );
				break;

			case DRAW_MODE_WALLS:								PasteSingleWall( iMapIndex );					break;
			case DRAW_MODE_DOORS:								PasteSingleDoor( iMapIndex );					break;
			case DRAW_MODE_WINDOWS:							PasteSingleWindow( iMapIndex );				break;
			case DRAW_MODE_ROOFS:								PasteSingleRoof( iMapIndex );					break;
			case DRAW_MODE_BROKEN_WALLS:				PasteSingleBrokenWall( iMapIndex );		break;
			case DRAW_MODE_DECOR:								PasteSingleDecoration( iMapIndex );		break;
			case DRAW_MODE_DECALS:
				if( ValidDecalPlacement( iMapIndex ) )
					PasteSingleDecal( iMapIndex );
				break;
			case DRAW_MODE_TOILET:							PasteSingleToilet( iMapIndex );				break;
			case DRAW_MODE_SMART_WALLS:					PasteSmartWall( iMapIndex );					break;
			case DRAW_MODE_SMART_DOORS:					PasteSmartDoor( iMapIndex );					break;
			case DRAW_MODE_SMART_WINDOWS:				PasteSmartWindow( iMapIndex );				break;
			case DRAW_MODE_SMART_BROKEN_WALLS:	PasteSmartBrokenWall( iMapIndex );		break;
			case DRAW_MODE_EXITGRID:
			case DRAW_MODE_FLOORS:
			case DRAW_MODE_GROUND:
			case DRAW_MODE_OSTRUCTS:
			case DRAW_MODE_OSTRUCTS1:
			case DRAW_MODE_OSTRUCTS2:
			case DRAW_MODE_DEBRIS:
				if( gusSelectionType >= SMALLSELECTION && gusSelectionType <= XLARGESELECTION )
				{
					DrawObjectsBasedOnSelectionRegion();
				}
				else
					gfRenderWorld = fPrevState;
				break;
			case DRAW_MODE_DOORKEYS:
				InitDoorEditing( iMapIndex );
				break;
			case DRAW_MODE_KILL_BUILDING:
				KillBuilding( iMapIndex );
				break;
			case DRAW_MODE_COPY_BUILDING:
			case DRAW_MODE_MOVE_BUILDING:
				if( gfFirstPlacement )
				{
					CopyBuilding( iMapIndex );
					gfFirstPlacement = FALSE;
				}
				gfRenderWorld = fPrevState;
				break;
			case DRAW_MODE_BANKS:
				PasteBanks( iMapIndex, gsBanksSubIndex, TRUE );
				break;
			case DRAW_MODE_ROADS:
				PasteRoads( iMapIndex );
				break;
			case (DRAW_MODE_GROUND + DRAW_MODE_FILL_AREA):
				TerrainFill( iMapIndex );
				//BeginFill( iMapIndex );
				break;
			case DRAW_MODE_PLACE_ITEM:
				if( gfFirstPlacement )
				{
					AddSelectedItemToWorld( iMapIndex );
					gfFirstPlacement = FALSE;
				}
				break;
			default:
				gfRenderWorld = fPrevState;
				break;
		}
	}
	else if ( _RightButtonDown )
	{
		gfRenderWorld = TRUE;
		switch( iDrawMode )
		{
			// Handle right clicking on a merc (for editing/moving him)
			case DRAW_MODE_ENEMY:
			case DRAW_MODE_CREATURE:
			case DRAW_MODE_REBEL:
			case DRAW_MODE_CIVILIAN:
				HandleRightClickOnMerc( iMapIndex );
				break;
			case DRAW_MODE_PLACE_ITEM:
				HandleRightClickOnItem( iMapIndex );
				break;

			// Handle the right clicks in the main window to bring up the appropriate selection window
			case DRAW_MODE_WALLS:					iEditorToolbarState = TBAR_MODE_GET_WALL;					break;
			case DRAW_MODE_DOORS:					iEditorToolbarState = TBAR_MODE_GET_DOOR;					break;
			case DRAW_MODE_WINDOWS:				iEditorToolbarState = TBAR_MODE_GET_WINDOW;				break;
			case DRAW_MODE_ROOFS:					iEditorToolbarState = TBAR_MODE_GET_ROOF;					break;
			case DRAW_MODE_BROKEN_WALLS:	iEditorToolbarState = TBAR_MODE_GET_BROKEN_WALL;	break;
			case DRAW_MODE_DECOR:					iEditorToolbarState = TBAR_MODE_GET_DECOR;				break;
			case DRAW_MODE_DECALS:				iEditorToolbarState = TBAR_MODE_GET_DECAL;				break;
			case DRAW_MODE_FLOORS:				iEditorToolbarState = TBAR_MODE_GET_FLOOR;				break;
			case DRAW_MODE_TOILET:				iEditorToolbarState = TBAR_MODE_GET_TOILET;				break;

			case DRAW_MODE_ROOM:					iEditorToolbarState = TBAR_MODE_GET_ROOM;					break;
			case DRAW_MODE_NEWROOF:				iEditorToolbarState = TBAR_MODE_GET_NEW_ROOF;			break;
			case DRAW_MODE_SLANTED_ROOF:	iEditorToolbarState = TBAR_MODE_GET_ROOM;					break;
			case DRAW_MODE_DEBRIS:				iEditorToolbarState = TBAR_MODE_GET_DEBRIS;				break;
			case DRAW_MODE_OSTRUCTS:			iEditorToolbarState = TBAR_MODE_GET_OSTRUCTS;			break;
			case DRAW_MODE_OSTRUCTS1:			iEditorToolbarState = TBAR_MODE_GET_OSTRUCTS1;		break;
			case DRAW_MODE_OSTRUCTS2:			iEditorToolbarState = TBAR_MODE_GET_OSTRUCTS2;		break;
			case DRAW_MODE_BANKS:					iEditorToolbarState = TBAR_MODE_GET_BANKS;				break;
			case DRAW_MODE_ROADS:					iEditorToolbarState = TBAR_MODE_GET_ROADS;				break;

			case DRAW_MODE_CAVES:
				if( gusSelectionType >= SMALLSELECTION && gusSelectionType <= XLARGESELECTION )
					ProcessAreaSelection( FALSE );
				break;

			case DRAW_MODE_SMART_WALLS:
				EraseWalls( iMapIndex );
				break;
			case DRAW_MODE_SMART_BROKEN_WALLS:
			case DRAW_MODE_SMART_WINDOWS:
			case DRAW_MODE_SMART_DOORS:
				RestoreWalls( iMapIndex );
				break;
			case DRAW_MODE_EXITGRID:
				if( GetExitGrid( iMapIndex, &gExitGrid ) )
					ApplyNewExitGridValuesToTextFields();
				break;
			default:
				gfRenderWorld = fPrevState;
				break;
		}
	}
	else if( !_LeftButtonDown && !gfFirstPlacement )
	{
		switch( iDrawMode )
		{
			case DRAW_MODE_SCHEDULECONFIRM:
				if( IsLocationSittableExcludingPeople( iMapIndex, FALSE ) )
				{
					RegisterCurrentScheduleAction( iMapIndex );
				}
				break;
			case DRAW_MODE_COPY_BUILDING:
				PasteBuilding( iMapIndex );
				break;
			case DRAW_MODE_MOVE_BUILDING:
				MoveBuilding( iMapIndex );
				break;
		}
	}

}
#else
void HandleMouseClicksInGameScreen()//dnl ch80 011213
{
	static UINT32 iLastMapIndexLB(-1), iLastMapIndexRB(-1), iLastMapIndexMB(-1);
	INT16 sX, sY;
	BOOLEAN fPrevState;
	UINT8 ubLightRadius, ubLightId;//dnl ch86 210214

	if(!GetMouseXY(&sGridX, &sGridY))
		return;
	if(iCurrentTaskbar == TASK_OPTIONS || iCurrentTaskbar == TASK_NONE)// if in taskbar modes which don't process clicks in the world
		return;
	if(!(gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA))// if mouse cursor not in the game screen
		return;
	iMapIndex = MAPROWCOLTOPOS(sGridY, sGridX);
	fPrevState = gfRenderWorld;
	if(_LeftButtonDown)
	{
		if(iLastMapIndexLB != iMapIndex && iLastMapIndexLB == -1)
			iLastMapIndexLB = iMapIndex;
		gfRenderWorld = TRUE;
		if(iDrawMode >= DRAW_MODE_ERASE)// Are we trying to erase something?
		{
			for(sY=(INT16)gSelectRegion.iTop; sY<=(INT16)gSelectRegion.iBottom; sY++)// Erasing can have a brush size larger than 1 tile
			{
				for(sX=(INT16)gSelectRegion.iLeft; sX<=(INT16)gSelectRegion.iRight; sX++)
				{
					if(iDrawMode == (DRAW_MODE_LIGHT + DRAW_MODE_ERASE))
					{
						if(FindLight(iMapIndex, gbDefaultLightType, &ubLightRadius, &ubLightId))//dnl ch86 210214
						{
							AddToUndoList(iMapIndex);
							RemoveLight(sX, sY, gbDefaultLightType);
						}
					}
					else
						EraseMapTile(MAPROWCOLTOPOS(sY, sX));
				}
			}
			if(iDrawMode == DRAW_MODE_LIGHT + DRAW_MODE_ERASE)
				LightSpriteRenderAll();// To adjust building's lighting
			return;
		}
		switch(iDrawMode)
		{
		case DRAW_MODE_FLOORS:
		case DRAW_MODE_GROUND:
		case DRAW_MODE_OSTRUCTS:
		case DRAW_MODE_OSTRUCTS1:
		case DRAW_MODE_OSTRUCTS2:
		case DRAW_MODE_DEBRIS:
			if(fDontUseClick && gusSelectionType >= SMALLSELECTION && gusSelectionType <= XLARGESELECTION)
				DrawObjectsBasedOnSelectionRegion();
			else
				gfRenderWorld = fPrevState;
			break;
		case DRAW_MODE_COPY_BUILDING:
		case DRAW_MODE_MOVE_BUILDING:
			if(gfFirstPlacement)
			{
				CopyBuilding(iMapIndex);
				gfFirstPlacement = FALSE;
			}
			gfRenderWorld = fPrevState;
			break;
		default:
			gfRenderWorld = fPrevState;
			break;
		}
	}
	else if(_RightButtonDown)
	{
		if(iLastMapIndexRB != iMapIndex && iLastMapIndexRB == -1)
			iLastMapIndexRB = iMapIndex;
	}
	else if(_MiddleButtonDown)
	{
		if(iLastMapIndexMB != iMapIndex && iLastMapIndexMB == -1)
			iLastMapIndexMB = iMapIndex;
	}
	else if(!_LeftButtonDown)
	{
		if(iMapIndex == iLastMapIndexLB)// LeftClick performed on same tile
		{
			gfRenderWorld = TRUE;
			switch(iDrawMode)
			{
			case DRAW_MODE_SCHEDULEACTION:
				if(IsLocationSittableExcludingPeople(iMapIndex, FALSE))
					RegisterCurrentScheduleAction(iMapIndex);
				break;
			case DRAW_MODE_NORTHPOINT:
			case DRAW_MODE_WESTPOINT:
			case DRAW_MODE_EASTPOINT:
			case DRAW_MODE_SOUTHPOINT:
			case DRAW_MODE_CENTERPOINT:
			case DRAW_MODE_ISOLATEDPOINT:
				SpecifyEntryPoint(iMapIndex);
				break;
			case DRAW_MODE_ENEMY:
			case DRAW_MODE_CREATURE:
			case DRAW_MODE_REBEL:
			case DRAW_MODE_CIVILIAN:
				AddMercToWorld(iMapIndex);// Handle adding mercs to the world
				break;
			case DRAW_MODE_LIGHT://dnl ch86 210214
				if(FindLight(iMapIndex, gbDefaultLightType, &ubLightRadius, &ubLightId) && ubLightRadius == gsLightRadius && ubLightId == 0)
					break;
				AddToUndoList(iMapIndex);
				RemoveLight(sGridX, sGridY, gbDefaultLightType);
				PlaceLight(gsLightRadius, sGridX, sGridY, 0, gbDefaultLightType);// Add a normal light to the world
				break;
			case DRAW_MODE_SAW_ROOM:
			case DRAW_MODE_ROOM:
			case DRAW_MODE_CAVES:
				if(gusSelectionType >= SMALLSELECTION && gusSelectionType <= XLARGESELECTION)
					ProcessAreaSelection(TRUE);
				break;
			case DRAW_MODE_NEWROOF:
				ReplaceBuildingWithNewRoof(iMapIndex);
				break;
			case DRAW_MODE_WALLS:
				PasteSingleWall(iMapIndex);
				break;
			case DRAW_MODE_DOORS:
				PasteSingleDoor(iMapIndex);
				break;
			case DRAW_MODE_WINDOWS:
				PasteSingleWindow(iMapIndex);
				break;
			case DRAW_MODE_ROOFS:
				PasteSingleRoof(iMapIndex);
				break;
			case DRAW_MODE_BROKEN_WALLS:
				PasteSingleBrokenWall(iMapIndex);
				break;
			case DRAW_MODE_DECOR:
				PasteSingleDecoration(iMapIndex);
				break;
			case DRAW_MODE_DECALS:
				if(ValidDecalPlacement(iMapIndex))
					PasteSingleDecal(iMapIndex);
				break;
			case DRAW_MODE_TOILET:
				PasteSingleToilet(iMapIndex);
				break;
			case DRAW_MODE_SMART_WALLS:
				PasteSmartWall(iMapIndex);
				break;
			case DRAW_MODE_SMART_DOORS:
				PasteSmartDoor(iMapIndex);
				break;
			case DRAW_MODE_SMART_WINDOWS:
				PasteSmartWindow(iMapIndex);
				break;
			case DRAW_MODE_SMART_BROKEN_WALLS:
				PasteSmartBrokenWall(iMapIndex);
				break;
			case DRAW_MODE_EXITGRID:
			case DRAW_MODE_FLOORS:
			case DRAW_MODE_GROUND:
			case DRAW_MODE_OSTRUCTS:
			case DRAW_MODE_OSTRUCTS1:
			case DRAW_MODE_OSTRUCTS2:
			case DRAW_MODE_DEBRIS:
				if(gusSelectionType >= SMALLSELECTION && gusSelectionType <= XLARGESELECTION)
					DrawObjectsBasedOnSelectionRegion();
				else
					gfRenderWorld = fPrevState;
				break;
			case DRAW_MODE_DOORKEYS:
				InitDoorEditing(iMapIndex);
				break;
			case DRAW_MODE_KILL_BUILDING:
				KillBuilding(iMapIndex);
				break;
			case DRAW_MODE_BANKS:
				PasteBanks(iMapIndex, gsBanksSubIndex, TRUE);
				break;
			case DRAW_MODE_ROADS:
				PasteRoads(iMapIndex);
				break;
			case (DRAW_MODE_GROUND + DRAW_MODE_FILL_AREA):
				TerrainFill(iMapIndex);
				break;
			case DRAW_MODE_PLACE_ITEM:
				AddSelectedItemToWorld(iMapIndex);
				break;
			case DRAW_MODE_HIGH_GROUND:
				if(gpWorldLevelData[iMapIndex].sHeight)
				{
					gpWorldLevelData[iMapIndex].sHeight = 0;
					RemoveTopmost(iMapIndex, FIRSTPOINTERS6);
				}
				else
				{
					gpWorldLevelData[iMapIndex].sHeight = WORLD_CLIFF_HEIGHT;
					AddTopmostToHead(iMapIndex, FIRSTPOINTERS6);
					gpWorldLevelData[iMapIndex].pTopmostHead->ubShadeLevel = DEFAULT_SHADE_LEVEL;
					gpWorldLevelData[iMapIndex].pTopmostHead->ubNaturalShadeLevel = DEFAULT_SHADE_LEVEL;
				}
				break;
			default:
				gfRenderWorld = fPrevState;
				break;
			}
		}
		if(!gfFirstPlacement)
		{
			switch(iDrawMode)
			{
			case DRAW_MODE_COPY_BUILDING:
				PasteBuilding(iMapIndex);
				break;
			case DRAW_MODE_MOVE_BUILDING:
				MoveBuilding(iMapIndex);
				break;
			}
		}
		iLastMapIndexLB = -1;
	}
	if(!_RightButtonDown)
	{
		if(iMapIndex == iLastMapIndexRB)// RightClick performed on same tile
		{
			gfRenderWorld = TRUE;
			switch(iDrawMode)
			{
			case DRAW_MODE_ENEMY:
			case DRAW_MODE_CREATURE:
			case DRAW_MODE_REBEL:
			case DRAW_MODE_CIVILIAN:
				HandleRightClickOnMerc(iMapIndex);// Handle right clicking on a merc (for editing/moving him)
				break;
			case DRAW_MODE_PLACE_ITEM:
				HandleRightClickOnItem(iMapIndex);//dnl ch43 280909
				break;
			// Handle the right clicks in the main window to bring up the appropriate selection window
			case DRAW_MODE_WALLS:
				iEditorToolbarState = TBAR_MODE_GET_WALL;
				break;
			case DRAW_MODE_DOORS:
				iEditorToolbarState = TBAR_MODE_GET_DOOR;
				break;
			case DRAW_MODE_WINDOWS:
				iEditorToolbarState = TBAR_MODE_GET_WINDOW;
				break;
			case DRAW_MODE_ROOFS:
				iEditorToolbarState = TBAR_MODE_GET_ROOF;
				break;
			case DRAW_MODE_BROKEN_WALLS:
				iEditorToolbarState = TBAR_MODE_GET_BROKEN_WALL;
				break;
			case DRAW_MODE_DECOR:
				iEditorToolbarState = TBAR_MODE_GET_DECOR;
				break;
			case DRAW_MODE_DECALS:
				iEditorToolbarState = TBAR_MODE_GET_DECAL;
				break;
			case DRAW_MODE_FLOORS:
				iEditorToolbarState = TBAR_MODE_GET_FLOOR;
				break;
			case DRAW_MODE_TOILET:
				iEditorToolbarState = TBAR_MODE_GET_TOILET;
				break;
			case DRAW_MODE_ROOM:
				iEditorToolbarState = TBAR_MODE_GET_ROOM;
				break;
			case DRAW_MODE_NEWROOF:
				iEditorToolbarState = TBAR_MODE_GET_NEW_ROOF;
				break;
			case DRAW_MODE_SLANTED_ROOF:
				iEditorToolbarState = TBAR_MODE_GET_ROOM;
				break;
			case DRAW_MODE_DEBRIS:
				iEditorToolbarState = TBAR_MODE_GET_DEBRIS;
				break;
			case DRAW_MODE_OSTRUCTS:
				iEditorToolbarState = TBAR_MODE_GET_OSTRUCTS;
				break;
			case DRAW_MODE_OSTRUCTS1:
				iEditorToolbarState = TBAR_MODE_GET_OSTRUCTS1;
				break;
			case DRAW_MODE_OSTRUCTS2:
				iEditorToolbarState = TBAR_MODE_GET_OSTRUCTS2;
				break;
			case DRAW_MODE_BANKS:
				iEditorToolbarState = TBAR_MODE_GET_BANKS;
				break;
			case DRAW_MODE_ROADS:
				iEditorToolbarState = TBAR_MODE_GET_ROADS;
				break;
			case DRAW_MODE_CAVES:
				if(gusSelectionType >= SMALLSELECTION && gusSelectionType <= XLARGESELECTION)
					ProcessAreaSelection(FALSE);
				break;
			case DRAW_MODE_SMART_WALLS:
				EraseWalls(iMapIndex);
				break;
			case DRAW_MODE_SMART_BROKEN_WALLS:
			case DRAW_MODE_SMART_WINDOWS:
			case DRAW_MODE_SMART_DOORS:
				RestoreWalls(iMapIndex);
				break;
			case DRAW_MODE_EXITGRID:
				if(GetExitGrid(iMapIndex, &gExitGrid))//dnl ch43 280909
					ApplyNewExitGridValuesToTextFields();
				break;
			default:
				gfRenderWorld = fPrevState;
				break;
			}
		}
		iLastMapIndexRB = -1;
	}
	if(!_MiddleButtonDown)
	{
		if(iMapIndex == iLastMapIndexMB)// MiddleClick performed on same tile
		{
		}
		iLastMapIndexMB = -1;
	}
}
#endif

void HandleMouseWheel(void)//dnl ch4 210909 //dnl ch80 011213
{
	if(_WheelValue && !gfSummaryWindowActive)
	{
		if(!GetMouseXY(&sGridX, &sGridY) && (iCurrentTaskbar == TASK_ITEMS || gubCurrMercMode == MERC_GETITEMMODE))
		{
			if(_WheelValue > 0)
				while(_WheelValue--)
					ScrollEditorItemsInfo(FALSE);
			else
				while(_WheelValue++)
					ScrollEditorItemsInfo(TRUE);
			_WheelValue = 0;
			return;
		}
		switch(iDrawMode)
		{
		case DRAW_MODE_SMART_WALLS:
			if(_WheelValue > 0)
				while(_WheelValue--)
					DecSmartWallUIValue();
			else
				while(_WheelValue++)
					IncSmartWallUIValue();
			break;
		case DRAW_MODE_SMART_DOORS:
			if(_WheelValue > 0)
				while(_WheelValue--)
					DecSmartDoorUIValue();
			else
				while(_WheelValue++)
					IncSmartDoorUIValue();
			break;
		case DRAW_MODE_SMART_WINDOWS:
			if(_WheelValue > 0)
				while(_WheelValue--)
					DecSmartWindowUIValue();
			else
				while(_WheelValue++)
					IncSmartWindowUIValue();
			break;
		case DRAW_MODE_SMART_BROKEN_WALLS:
			if(_WheelValue > 0)
				while(_WheelValue--)
					DecSmartBrokenWallUIValue();
			else
				while(_WheelValue++)
					IncSmartBrokenWallUIValue();
			break;
		case DRAW_MODE_PLACE_ITEM:
			if(_WheelValue > 0)
				while(_WheelValue--)
					SelectPrevItemInPool();
			else
				while(_WheelValue++)
					SelectNextItemInPool();
			break;
		default:
			if(_WheelValue > 0)
				iCurrentAction = ACTION_SUB_INDEX_UP;
			else
				iCurrentAction = ACTION_SUB_INDEX_DWN;
			break;
		}
		gfRenderDrawingMode = TRUE;
	}
	_WheelValue = 0;
}

BOOLEAN DoIRenderASpecialMouseCursor()
{
	INT16 sMouseX_M, sMouseY_M;

	// Draw basic mouse
	if ( GetMouseXY( &sMouseX_M, &sMouseY_M ) )
	{
		if ( (gsCursorGridNo = MAPROWCOLTOPOS( sMouseY_M, sMouseX_M )) < 0x80000000 )
		{
			// Add basic cursor
			//gBasicCursorNode = AddTopmostToTail( gsCursorGridNo, FIRSTPOINTERS1 );
		}
	}

	if ( iCurrentTaskbar != TASK_OPTIONS )
	{
		switch ( iDrawMode )
		{
			case DRAW_MODE_OSTRUCTS2:
				if(CheckForFences())
					fDontUseRandom = TRUE;
			case DRAW_MODE_DEBRIS:							// These only show if you first hit PGUP/PGDOWN keys
			case DRAW_MODE_OSTRUCTS:
			case DRAW_MODE_OSTRUCTS1:
				if(!fDontUseRandom)
					break;
			case DRAW_MODE_BANKS:
			case DRAW_MODE_ROADS:
			case DRAW_MODE_WALLS:
			case DRAW_MODE_DOORS:
			case DRAW_MODE_WINDOWS:
			case DRAW_MODE_ROOFS:
			case DRAW_MODE_BROKEN_WALLS:
			case DRAW_MODE_DECOR:
			case DRAW_MODE_DECALS:
			case DRAW_MODE_FLOORS:
			case DRAW_MODE_TOILET:

			case DRAW_MODE_SMART_WALLS:
			case DRAW_MODE_SMART_DOORS:
			case DRAW_MODE_SMART_WINDOWS:
			case DRAW_MODE_SMART_BROKEN_WALLS:
			case DRAW_MODE_ROOM:
			case DRAW_MODE_NEWROOF:
				return DrawTempMouseCursorObject( );

			default:
				break;
		}
	}
	return FALSE;
}


extern INT32 iEditorToolbarState;
extern INT32 iEditorToolbarLastWallState;

void ShowEntryPoints()
{
	//make entry points visible
	if( gMapInformation.sNorthGridNo != -1 )
	AddTopmostToTail( gMapInformation.sNorthGridNo, FIRSTPOINTERS2 );
	if( gMapInformation.sEastGridNo != -1 )
	AddTopmostToTail( gMapInformation.sEastGridNo, FIRSTPOINTERS2 );
	if( gMapInformation.sSouthGridNo != -1 )
	AddTopmostToTail( gMapInformation.sSouthGridNo, FIRSTPOINTERS2 );
	if( gMapInformation.sWestGridNo != -1 )
	AddTopmostToTail( gMapInformation.sWestGridNo, FIRSTPOINTERS2 );
}

void HideEntryPoints()
{
	//remove entry point indicators
	if( gMapInformation.sNorthGridNo != -1 )
		RemoveAllTopmostsOfTypeRange( gMapInformation.sNorthGridNo, FIRSTPOINTERS, FIRSTPOINTERS );
	if( gMapInformation.sEastGridNo != -1 )
		RemoveAllTopmostsOfTypeRange( gMapInformation.sEastGridNo, FIRSTPOINTERS, FIRSTPOINTERS );
	if( gMapInformation.sSouthGridNo != -1 )
		RemoveAllTopmostsOfTypeRange( gMapInformation.sSouthGridNo, FIRSTPOINTERS, FIRSTPOINTERS );
	if( gMapInformation.sWestGridNo != -1 )
		RemoveAllTopmostsOfTypeRange( gMapInformation.sWestGridNo, FIRSTPOINTERS, FIRSTPOINTERS );
}

void TaskOptionsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		iTaskMode = TASK_OPTIONS;
	}
}

void TaskTerrainCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		iTaskMode = TASK_TERRAIN;
	}
}

void TaskBuildingCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		iTaskMode = TASK_BUILDINGS;
	}
}

void TaskItemsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		iTaskMode = TASK_ITEMS;
	}
}

void TaskMercsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		iTaskMode = TASK_MERCS;
	}
}

void TaskMapInfoCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		iTaskMode = TASK_MAPINFO;
	}
}

void ProcessAreaSelection( BOOLEAN fWithLeftButton )
{
	BOOLEAN fPrevState = gfRenderWorld;
	gfRenderWorld = TRUE;
	switch( iDrawMode )
	{
		case DRAW_MODE_ROOM:
		case DRAW_MODE_SLANTED_ROOF:
			AddBuildingSectionToWorld( &gSelectRegion );
			break;
		case DRAW_MODE_SAW_ROOM:
			RemoveBuildingSectionFromWorld( &gSelectRegion );
			break;
		case DRAW_MODE_CAVES:
			if( fWithLeftButton )
				AddCaveSectionToWorld( &gSelectRegion );
			else
				RemoveCaveSectionFromWorld( &gSelectRegion );
			break;
		case DRAW_MODE_ROOMNUM:
			DrawObjectsBasedOnSelectionRegion();
			if( gusCurrRoomNumber > 0 )
			{
				if( gusCurrRoomNumber == gusMaxRoomNumber )
					gusMaxRoomNumber++;
				
				//retain existing room number
				if ( _KeyDown( SHIFT ) )
				{
					//Do nothing
				}
				else
				{	
					gusCurrRoomNumber = gusMaxRoomNumber;
				}
				if( iCurrentTaskbar == TASK_BUILDINGS && TextInputMode() )
				{
					CHAR16 str[4];
					swprintf( str, L"%d", gusCurrRoomNumber );
					SetInputFieldStringWith16BitString( 1, str );
					SetActiveField( 0 );
				}
			}
			break;
		case DRAW_MODE_EXITGRID:
		case DRAW_MODE_FLOORS:
		case DRAW_MODE_GROUND:
		case DRAW_MODE_OSTRUCTS:
		case DRAW_MODE_OSTRUCTS1:
		case DRAW_MODE_OSTRUCTS2:
		case DRAW_MODE_DEBRIS:
			//These aren't normally supported by area selection, so
			//call the equivalent function that is normally associated with them.
			DrawObjectsBasedOnSelectionRegion();
			break;
		default:
			gfRenderWorld = fPrevState;
			break;
	}
}

//For any drawing modes that support large cursors, or even area selection, this function calls the
//appropriate paste function for every gridno within the cursor.	This is not used for functions that
//rely completely on selection areas, such as buildings.
void DrawObjectsBasedOnSelectionRegion()
{
	INT32 x, y, iMapIndex;
	BOOLEAN fSkipTest;
	EXITGRID ExitGrid;//dnl ch86 190214

	//Certain drawing modes are placed with 100% density.	Those cases are checked here,
	//so the density test can be skipped.
	fSkipTest = FALSE;
	if( gusSelectionType == SMALLSELECTION ||
		iDrawMode == DRAW_MODE_GROUND ||
			iDrawMode == DRAW_MODE_FLOORS ||
		iDrawMode == DRAW_MODE_ROOMNUM ||
			iDrawMode == DRAW_MODE_EXITGRID )
		fSkipTest = TRUE;

	//The reason why I process the region from top to bottom then to the right is
	//to even out the binary tree undo placements.	Otherwise, the placements within
	//the undo binary tree would alway choose the right branch because the imapindex is
	//always greater than the previously positioned one.
	//Process the cursor area
	for ( x = gSelectRegion.iLeft; x <= gSelectRegion.iRight; x++ )
	{
		//process the region from
		for ( y = gSelectRegion.iTop; y <= (INT16)gSelectRegion.iBottom; y++ )
		{
			if( fSkipTest || PerformDensityTest() )
			{
				iMapIndex = MAPROWCOLTOPOS( y, x );
				switch( iDrawMode )
				{
					case DRAW_MODE_EXITGRID:
						if(GetExitGrid(iMapIndex, &ExitGrid) == FALSE || !ExitGrid.Equal(gExitGrid))//dnl ch86 190214
						{
							RemoveTopmost(iMapIndex, FIRSTPOINTERS8);
							AddToUndoList(iMapIndex);
							AddExitGridToWorld(iMapIndex, &gExitGrid);
							AddTopmostToTail(iMapIndex, FIRSTPOINTERS8);
						}
						break;
					case DRAW_MODE_DEBRIS:		PasteDebris( iMapIndex );													break;
					case DRAW_MODE_FLOORS:		PasteSingleFloor( iMapIndex );										break;
					case DRAW_MODE_GROUND:		PasteTexture( iMapIndex );												break;
					case DRAW_MODE_OSTRUCTS:	PasteStructure( iMapIndex );											break;
					case DRAW_MODE_OSTRUCTS1: PasteStructure1( iMapIndex );											break;
					case DRAW_MODE_OSTRUCTS2: PasteStructure2( iMapIndex );											break;
					case DRAW_MODE_ROOMNUM:	PasteRoomNumber( iMapIndex, gusCurrRoomNumber );	break;
					default: return; //no point in continuing...
				}
			}
		}
	}
}

extern void AutoLoadMap();

//The main loop of the editor.
UINT32	EditScreenHandle( void )
{
	UINT32 uiRetVal;
	BOOLEAN fShowingCursor;
	StartFrameBufferRender();

	if( gfWorldLoaded && gMapInformation.ubMapVersion <= 7 && !gfCorruptMap )
	{
		ScreenMsg( FONT_MCOLOR_RED, MSG_ERROR, pAutoLoadMapText[0] );
		gfCorruptMap = TRUE;
	}
	if( gfWorldLoaded && gubScheduleID > 40 && !gfCorruptSchedules )
	{
		OptimizeSchedules();
		if( gubScheduleID > 32 )
		{
			ScreenMsg( FONT_MCOLOR_RED, MSG_ERROR, pAutoLoadMapText[1] );
			gfCorruptSchedules = TRUE;
		}
	}

	if( gfAutoLoadA9 == 2 )
		AutoLoadMap();

	if ( fEditModeFirstTime )
	{
		EditModeInit( );
	}

	if ( InOverheadMap( ) && !gfSummaryWindowActive )
	{
		ScrollOverheadMap();//dnl ch45 061009
		HandleOverheadMap();
	}

	//Calculate general mouse information
	GetMouseXY( &sGridX, &sGridY );
	iMapIndex = (INT32)sGridY * WORLD_COLS + sGridX;

	DetermineUndoState();

	fBeenWarned = FALSE;

	uiRetVal = EDIT_SCREEN;

	// Handle the bottom task bar menu.
	DoTaskbar();

	//Process the variety of popup menus, dialogs, etc.

	if( gubMessageBoxStatus )
	{
		if( MessageBoxHandled( ) )
			return ProcessEditscreenMessageBoxResponse();
		return EDIT_SCREEN;
	}

	if( ProcessPopupMenuIfActive() )
		return EDIT_SCREEN;
	if ( fHelpScreen )
		return( WaitForHelpScreenResponse( ) );
	if ( fSelectionWindow )
		return( WaitForSelectionWindowResponse( ) );

	// If editing mercs, handle that stuff
	ProcessMercEditing();

	if(!gfSummaryWindowActive && !gfEditingDoor)//dnl ch78 261113 //dnl ch86 220214
		EnsureStatusOfEditorButtons();

	// Handle scrolling of the map if needed
	if( !gfGotoGridNoUI && !gfKeyboardItemCreationUI && iDrawMode != DRAW_MODE_SHOW_TILESET && !gfSummaryWindowActive &&
			!gfEditingDoor && !gfNoScroll && !InOverheadMap() )
		ScrollWorld();

	iCurrentAction = ACTION_NULL;

	UpdateCursorAreas();
	SetCurrentCursorFromDatabase(CURSOR_NORMAL);//dnl ch1 210909 show cursor on map screen, currently for debug purposes in solving cliffs problem
	HandleMouseClicksInGameScreen();

	if( !gfFirstPlacement && !gfLeftButtonState )
		gfFirstPlacement = TRUE;

	//If we are copying or moving a building, we process, then delete the building layout immediately
	//after releasing the mouse button.	If released in the world, then the building would have been
	//processed in above function, HandleMouseClicksInGameScreen().
	if( !_LeftButtonDown && gpBuildingLayoutList )
		DeleteBuildingLayout();

	fShowingCursor = DoIRenderASpecialMouseCursor();

	DequeAllGameEvents( TRUE );

	if ( fBuildingShowRoomInfo )
	{
		SetRenderFlags( RENDER_FLAG_ROOMIDS );
	}

	if ( gfRenderWorld )
	{
		if( gCursorNode )
			gCursorNode->uiFlags &= (~LEVELNODE_REVEAL);

		// This flag is the beast that makes the renderer do everything
		MarkWorldDirty();

		gfRenderWorld = FALSE;
	}

	// The default here for the renderer is to just do dynamic stuff...
	if( !gfSummaryWindowActive && !gfEditingDoor && !InOverheadMap() )
		RenderWorld();

	DisplayWayPoints();

	if ( fShowingCursor )
		RemoveTempMouseCursorObject();

	ProcessEditorRendering();

	// Handle toolbar selections
	HandleJA2ToolbarSelection( );

	// Handle keyboard shortcuts / selections
	HandleKeyboardShortcuts( );

	HandleMouseWheel();//dnl ch4 210909

	// Perform action based on current selection
	if ( (uiRetVal = PerformSelectedAction( )) != EDIT_SCREEN )
		return( uiRetVal );

	// Display Framerate
	DisplayFrameRate( );

	// Handle video overlays, for FPS and screen message stuff
	if ( gfScrollPending )
	{
		AllocateVideoOverlaysArea( );
		SaveVideoOverlaysArea( FRAME_BUFFER );
	}
	ExecuteVideoOverlays( );

	ScrollString( );

	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender( );

	return( uiRetVal );
}

void CreateGotoGridNoUI()
{
	gfGotoGridNoUI = TRUE;
	//Disable the rest of the editor
	DisableEditorTaskbar();
	//Create the background panel.
	guiGotoGridNoUIButtonID =
		CreateTextButton( L"Enter Gridno:", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 288, iScreenHeightOffset + 155, 64, 50, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MSYS_NO_CALLBACK );
	SpecifyDisabledButtonStyle( guiGotoGridNoUIButtonID, DISABLED_STYLE_NONE );
	SpecifyButtonTextOffsets( guiGotoGridNoUIButtonID, 5, 5, FALSE );
	DisableButton( guiGotoGridNoUIButtonID );
	//Create a blanket region so nobody can use
	MSYS_DefineRegion( &GotoGridNoUIRegion, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,	MSYS_PRIORITY_NORMAL+1, 0, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	//Init a text input field.
	InitTextInputModeWithScheme( DEFAULT_SCHEME );
	AddTextInputField( iScreenWidthOffset + 300, iScreenHeightOffset + 180, 40, 18, MSYS_PRIORITY_HIGH, L"", 6, INPUTTYPE_NUMERICSTRICT );
}

void RemoveGotoGridNoUI()
{
	INT32 iMapIndex;
	gfGotoGridNoUI = FALSE;
	//Enable the rest of the editor
	EnableEditorTaskbar();
	RemoveButton( guiGotoGridNoUIButtonID );
	iMapIndex = GetNumericStrictValueFromField( 0 );
	KillTextInputMode();
	MSYS_RemoveRegion( &GotoGridNoUIRegion );
	if( iMapIndex != -1 )
	{ //Warp the screen to the location of this gridno.
		CenterScreenAtMapIndex( iMapIndex );
	}
	else
	{
		MarkWorldDirty();
	}
}

void UpdateLastActionBeforeLeaving()
{
	if( iCurrentTaskbar == TASK_MERCS )
		IndicateSelectedMerc( SELECT_NO_MERC );
	SpecifyItemToEdit( NULL, -1 );
}

void ReloadMap()
{
	CHAR16 szFilename[30];
	swprintf( szFilename, L"%S", gubFilename );
	ExternalLoadMap( szFilename );
}

void ShowHighGround(INT32 iShowHighGroundCommand)//dnl ch2 210909
{
	switch(iShowHighGroundCommand)
	{
	case 0:
		for(int cnt=0; cnt<WORLD_MAX; cnt++)
		{
			if(!gpWorldLevelData[cnt].sHeight && GridNoOnVisibleWorldTile(cnt))//dnl ch86 210214
			{
				RemoveTopmost(cnt, FIRSTPOINTERS7);
				{
					AddTopmostToHead(cnt, FIRSTPOINTERS7);
					gpWorldLevelData[cnt].pTopmostHead->ubShadeLevel = DEFAULT_SHADE_LEVEL;
					gpWorldLevelData[cnt].pTopmostHead->ubNaturalShadeLevel = DEFAULT_SHADE_LEVEL;
				}
			}
		}
		break;
	case 1:
		ScreenMsg(FONT_MCOLOR_WHITE, MSG_INTERFACE, pShowHighGroundText[0] );
		for(int cnt=0; cnt<WORLD_MAX; cnt++)
		{
			if(gpWorldLevelData[cnt].sHeight && GridNoOnVisibleWorldTile(cnt))//dnl ch86 210214
			{
				RemoveTopmost(cnt, FIRSTPOINTERS11);
				{
					AddTopmostToHead(cnt, FIRSTPOINTERS11);
					gpWorldLevelData[cnt].pTopmostHead->ubShadeLevel = DEFAULT_SHADE_LEVEL;
					gpWorldLevelData[cnt].pTopmostHead->ubNaturalShadeLevel = DEFAULT_SHADE_LEVEL;
				}
			}
		}
		break;
	case 2:
		for(int cnt=0; cnt<WORLD_MAX; cnt++)
			RemoveTopmost(cnt, FIRSTPOINTERS7);
		break;
	case 3:
		ScreenMsg(FONT_MCOLOR_WHITE, MSG_INTERFACE, pShowHighGroundText[1] );
		for(int cnt=0; cnt<WORLD_MAX; cnt++)
			RemoveTopmost(cnt, FIRSTPOINTERS11);
		break;
	case 4:
		for(int cnt=0; cnt<WORLD_MAX; cnt++)
			RemoveAllTopmostsOfTypeRange(cnt, FIRSTPOINTERS, FIRSTPOINTERS);
		break;
	default:
		break;
	}
	gfRenderWorld = TRUE;
}

void CreateKeyboardItemCreationUI()
{
	gfKeyboardItemCreationUI = TRUE;
	//Disable the rest of the editor
	DisableEditorTaskbar();
	//Create the background panel.
	guiKeyboardItemCreationUIButtonID =
		CreateTextButton( L"Enter ItemID:", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 288, iScreenHeightOffset + 155, 64, 50, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MSYS_NO_CALLBACK );
	SpecifyDisabledButtonStyle( guiKeyboardItemCreationUIButtonID, DISABLED_STYLE_NONE );
	SpecifyButtonTextOffsets( guiKeyboardItemCreationUIButtonID, 5, 5, FALSE );
	DisableButton( guiKeyboardItemCreationUIButtonID );
	//Create a blanket region so nobody can use
	MSYS_DefineRegion( &KeyboardItemCreationUIRegion, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,	MSYS_PRIORITY_NORMAL+1, 0, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	//Init a text input field.
	InitTextInputModeWithScheme( DEFAULT_SCHEME );
	AddTextInputField( iScreenWidthOffset + 300, iScreenHeightOffset + 180, 40, 18, MSYS_PRIORITY_HIGH, L"", 4, INPUTTYPE_NUMERICSTRICT );
}

void RemoveKeyboardItemCreationUI()
{
	INT16 sTemp;

	gfKeyboardItemCreationUI = FALSE;
	//Enable the rest of the editor
	EnableEditorTaskbar();
	RemoveButton( guiKeyboardItemCreationUIButtonID );
	sTemp = GetNumericStrictValueFromField( 0 );
	KillTextInputMode();
	MSYS_RemoveRegion( &KeyboardItemCreationUIRegion );
	if ( sTemp != -1 )
	{
		usEditorTempItem = sTemp;
		AddSelectedItemToWorld( giCreateItemCursorMapIndex );
	}
	MarkWorldDirty();
}

BOOLEAN FindLight(INT32 iGridNo, INT8 bLightType, UINT8 *pubLightRadius, UINT8 *pubLightId, INT32 *piSprite)//dnl ch86 210214
{
	INT8 bSpriteLightType;
	INT16 sX, sY;
	INT32 iCount, cnt;
	SOLDIERTYPE *pSoldier;

	ConvertGridNoToXY(iGridNo, &sX, &sY);
	for(iCount=0; iCount<MAX_LIGHT_SPRITES; iCount++)
	{
		bSpriteLightType = (LightSprites[iCount].uiFlags & LIGHT_PRIMETIME ? PRIMETIME_LIGHT : (LightSprites[iCount].uiFlags & LIGHT_NIGHTTIME ? NIGHTTIME_LIGHT : ALWAYSON_LIGHT));
		if((LightSprites[iCount].uiFlags & LIGHT_SPR_ACTIVE) && LightSprites[iCount].iX == sX && LightSprites[iCount].iY == sY && (bLightType == bSpriteLightType || bLightType == ANY_LIGHT))
		{
			for(cnt=0; cnt<MAX_NUM_SOLDIERS; cnt++)
				if(GetSoldier(&pSoldier, cnt) && pSoldier->iLight == iCount)
					break;
			if(cnt == MAX_NUM_SOLDIERS)
			{
				*pubLightRadius = pLightNames[LightSprites[iCount].iTemplate][4] - '0';
				*pubLightId = LightSprites[iCount].uiLightType;
				if(piSprite)
					*piSprite = iCount;
				return(TRUE);
			}
		}
	}
	return(FALSE);
}

#else //non-editor version

#include "types.h"
#include "screenids.h"

UINT32 EditScreenInit()
{
	return TRUE ;
}

UINT32	EditScreenHandle( )
{
	//If this screen ever gets set, then this is a bad thing -- endless loop
	return ERROR_SCREEN ;
}

UINT32 EditScreenShutdown( )
{
	return TRUE ;
}

#endif
