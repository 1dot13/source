#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include	"Types.h"
	#include	"SaveLoadScreen.h"
	#include	"Video.h"
	#include	"Font Control.h"
	#include	"Game Clock.h"
	#include	"Render Dirty.h"
	#include	"Text Input.h"
	#include	"SaveLoadGame.h"
	#include	"Stdio.h"
	#include	"WordWrap.h"
	#include	"StrategicMap.h"
	#include	"Finances.h"
	#include	"WCheck.h"
	#include	"Utilities.h"
	#include	"Cursors.h"
	#include	"VObject.h"
	#include	"Merc Hiring.h"
	#include	"LaptopSave.h"
	#include	"Options Screen.h"
	#include	"GameVersion.h"
	#include	"SysUtil.h"
	#include	"Tactical Save.h"
	#include	"OverHead.h"
	#include	"gamescreen.h"
	#include	"GameSettings.h"
	#include	"fade screen.h"
	#include	"English.h"
	#include	"Gameloop.h"
	#include	"Game Init.h"
	#include	"Text.h"
	#include	"Message.h"
	#include	"Map Screen Interface.h"
	#include	"Multi Language Graphic Utils.h"
	#include	"Campaign Types.h"
	#include	"PostalService.h"
#endif

#include "connect.h"

#ifdef JA2UB
#include "Strategic Movement.h"

//#include "Strategic Movement Costs.h"

#include "LuaInitNPCs.h"
#endif

BOOLEAN gfSchedulesHosed = FALSE;
extern UINT32 guiBrokenSaveGameVersion;

UINT16 NUM_SLOT;

//////////////////////////////////////////////////////
//
//	Defines
//
//////////////////////////////////////////////////////

#define		SAVE_LOAD_TITLE_FONT							FONT14ARIAL
#define		SAVE_LOAD_TITLE_COLOR							FONT_MCOLOR_WHITE

#define		SAVE_LOAD_NORMAL_FONT							FONT12ARIAL
#define		SAVE_LOAD_NORMAL_COLOR							FONT_GRAY2
#define		SAVE_LOAD_NORMAL_SHADOW_COLOR					FONT_NEARBLACK

#define		SAVE_LOAD_QUICKSAVE_FONT						FONT12ARIAL
#define		SAVE_LOAD_QUICKSAVE_COLOR						2
#define		SAVE_LOAD_QUICKSAVE_SHADOW_COLOR				189 

#define		SAVE_LOAD_EMPTYSLOT_FONT						FONT12ARIAL
#define		SAVE_LOAD_EMPTYSLOT_COLOR						2
#define		SAVE_LOAD_EMPTYSLOT_SHADOW_COLOR				189

#define		SAVE_LOAD_HIGHLIGHTED_FONT						FONT12ARIAL
#define		SAVE_LOAD_HIGHLIGHTED_COLOR						FONT_MCOLOR_WHITE
#define		SAVE_LOAD_HIGHLIGHTED_SHADOW_COLOR				2

#define		SAVE_LOAD_SELECTED_FONT							FONT12ARIAL
#define		SAVE_LOAD_SELECTED_COLOR						2
#define		SAVE_LOAD_SELECTED_SHADOW_COLOR					130

#define		SAVE_LOAD_NUMBER_FONT							FONT12ARIAL
#define		SAVE_LOAD_NUMBER_COLOR							FONT_MCOLOR_WHITE

#define		SLG_SELECTED_COLOR								FONT_MCOLOR_WHITE
#define		SLG_UNSELECTED_COLOR							FONT_MCOLOR_DKWHITE

#define		SLG_SAVELOCATION_WIDTH							605
#define		SLG_FIRST_SAVED_SPOT_X							iScreenWidthOffset + 17
#define		SLG_FIRST_SAVED_SPOT_Y							iScreenHeightOffset + 49

#define		SLG_DATE_OFFSET_X								13

#define		SLG_SECTOR_OFFSET_X								95
#define		SLG_SECTOR_OFFSET_Y								SLG_DATE_OFFSET_Y
#define		SLG_SECTOR_WIDTH										98

#define		SLG_NUM_MERCS_OFFSET_X							196
#define		SLG_NUM_MERCS_OFFSET_Y							SLG_DATE_OFFSET_Y

#define		SLG_BALANCE_OFFSET_X							260
#define		SLG_BALANCE_OFFSET_Y							SLG_DATE_OFFSET_Y

#define		SLG_SAVE_GAME_DESC_X							318
#define		SLG_SAVE_GAME_DESC_Y							SLG_DATE_OFFSET_Y

#define		SLG_TITLE_POS_X									iScreenWidthOffset
#define		SLG_TITLE_POS_Y									iScreenHeightOffset

#define		SLG_SAVE_CANCEL_POS_X							iScreenWidthOffset + 226
#define		SLG_LOAD_CANCEL_POS_X							iScreenWidthOffset + 329
#define		SLG_CANCEL_POS_Y								iScreenHeightOffset + 438

#define		SLG_SAVE_LOAD_BTN_POS_X							iScreenWidthOffset + 123
#define		SLG_SAVE_LOAD_BTN_POS_Y							iScreenHeightOffset + 438

#define		SLG_DOUBLE_CLICK_DELAY							500

//defines for saved game version status
enum
{
	SLS_HEADER_OK,
	SLS_SAVED_GAME_VERSION_OUT_OF_DATE,
	SLS_GAME_VERSION_OUT_OF_DATE,
	SLS_BOTH_SAVE_GAME_AND_GAME_VERSION_OUT_OF_DATE,
};

//////////////////////////////////////////////////////
//
//	Global Variables
//
//////////////////////////////////////////////////////

extern void NextLoopCheckForEnoughFreeHardDriveSpace();

BOOLEAN		gfSaveLoadScreenEntry = TRUE;
BOOLEAN		gfSaveLoadScreenExit	= FALSE;
BOOLEAN		gfRedrawSaveLoadScreen = TRUE;

BOOLEAN		gfExitAfterMessageBox = FALSE;
INT32		giSaveLoadMessageBox = -1;					// SaveLoad pop up messages index value

UINT32		guiSaveLoadExitScreen = SAVE_LOAD_SCREEN;

//Contains the array of valid save game locations
BOOLEAN		gbSaveGameArray[ NUM_SAVE_GAMES ];

BOOLEAN		gfDoingQuickLoad = FALSE;

BOOLEAN		gfFailedToSaveGameWhenInsideAMessageBox = FALSE;

//This flag is used to diferentiate between loading a game and saveing a game.
// gfSaveGame=TRUE		For saving a game
// gfSaveGame=FALSE		For loading a game
BOOLEAN		gfSaveGame=TRUE;

BOOLEAN		gfSaveLoadScreenButtonsCreated = FALSE;

INT8		gbSaveGameSelectedLocation[ NUM_SAVE_GAMES ];
INT32		gbSelectedSaveLocation=-1;
INT8		gbHighLightedLocation=-1;
INT8		gbLastHighLightedLocation=-1;
INT32		gbSetSlotToBeSelected=-1;

UINT32		guiSlgBackGroundImage;
UINT32		guiBackGroundAddOns;

UINT32		guiBackGroundAddOns_New;
UINT32		SLG_SAVELOCATION_HEIGHT;
UINT32		SLG_GAP_BETWEEN_LOCATIONS;
UINT32		SLG_DATE_OFFSET_Y;
UINT32		SLG_SELECTED_SLOT_GRAPHICS_NUMBER;
UINT32		SLG_UNSELECTED_SLOT_GRAPHICS_NUMBER;

// The string that will contain the game desc text
CHAR16		gzGameDescTextField[ SIZE_OF_SAVE_GAME_DESC ] = {0} ;

BOOLEAN		gfUserInTextInputMode = FALSE;
UINT8		gubSaveGameNextPass=0;

BOOLEAN		gfStartedFadingOut = FALSE;
BOOLEAN		gfCameDirectlyFromGame = FALSE;
BOOLEAN		gfLoadedGame = FALSE;	//Used to know when a game has been loaded, the flag in gtacticalstatus might have been reset already
BOOLEAN		gfLoadGameUponEntry = FALSE;
BOOLEAN		gfGettingNameFromSaveLoadScreen = FALSE;

#ifdef JA2BETAVERSION
extern	BOOLEAN		gfDisplaySaveGamesNowInvalidatedMsg;
#endif

//
//Buttons
//
INT32		guiSlgButtonImage;

// Cancel Button
void BtnSlgCancelCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiSlgCancelBtn;

// Save game Button
void BtnSlgSaveLoadCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiSlgSaveLoadBtn;
INT32	guiSaveLoadImage;

INT32 guiNewButtonImage;
INT32 guiPrevButton, guiNextButton;
void BtnNewPrevButtonCallback(GUI_BUTTON *btn,INT32 reason);
void BtnNewNextButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT16 PAGE_SLOT = 0;
UINT16 VAL_SLOT_START = 0;
UINT16 VAL_SLOT_END = 0;
BOOLEAN	RenderImage = TRUE;
BOOLEAN DoAutoSave( int ubSaveGameID, STR16 pGameDesc );
BOOLEAN AutoSaveToSlot[5];

// WANNE: Used For auto save games
UINT32	lastLoadedSaveGameDay = 0;
UINT8	lastLoadedSaveGameHour = 0;

#define MAX_PAGE_SLOT 12

//Mouse regions for the currently selected save game
MOUSE_REGION	gSelectedSaveRegion[ NUM_SAVE_GAMES ];
void		SelectedSaveRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void		SelectedSaveRegionMovementCallBack(MOUSE_REGION * pRegion, INT32 reason );

MOUSE_REGION		gSLSEntireScreenRegion;
void		SelectedSLSEntireRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


//////////////////////////////////////////////////////
//
//	Function Prototypes
//
//////////////////////////////////////////////////////

BOOLEAN		EnterSaveLoadScreen();
void		RenderSaveLoadScreen();
void		ExitSaveLoadScreen();
void		HandleSaveLoadScreen();
void		GetSaveLoadScreenUserInput();
void		SaveLoadGameNumber( INT32 bSaveGameID );
BOOLEAN		LoadSavedGameHeader( INT32 bEntry, SAVED_GAME_HEADER *pSaveGameHeader );
BOOLEAN		DisplaySaveGameEntry( INT32 bEntryID );//, UINT16 usPosY );
BOOLEAN		DisplaySaveGameList();
void		InitSaveLoadScreenTextInputBoxes();
void		DestroySaveLoadTextInputBoxes();
void		SetSelection( UINT8 ubNewSelection );
UINT8		CompareSaveGameVersion( INT32 bSaveGameID );
void		LoadSavedGameWarningMessageBoxCallBack( UINT8 bExitValue );
void		LoadSavedGameDeleteAllSaveGameMessageBoxCallBack( UINT8 bExitValue );
void		DeleteAllSaveGameFile( );
void		DisplayOnScreenNumber( BOOLEAN fErase );
void		DisableSelectedSlot();
void		ConfirmSavedGameMessageBoxCallBack( UINT8 bExitValue );
void		ConfirmLoadSavedGameMessageBoxCallBack( UINT8 bExitValue );
void		FailedLoadingGameCallBack( UINT8 bExitValue );
void		SetSaveLoadExitScreen( UINT32 uiScreen );
void		NotEnoughHardDriveSpaceForQuickSaveMessageBoxCallBack( UINT8 bExitValue );
void		NotEnoughHardDriveSpaceForNormalSaveMessageBoxCallBack( UINT8 bExitValue );
void		RedrawSaveLoadScreenAfterMessageBox( UINT8 bExitValue );
void		ClearSelectedSaveSlot();
void		MoveSelectionUpOrDown( BOOLEAN fUp );
void		SaveGameToSlotNum();
void		StartFadeOutForSaveLoadScreen();
#ifdef JA2UB
extern void MakeBadSectorListFromMapsOnHardDrive( BOOLEAN fDisplayMessages ); // ja25 UB
#endif

//////////////////////////////////////////////////////
//
//	Code
//
//////////////////////////////////////////////////////

void LoadSaveGameOldOrNew()
{
	if (gGameExternalOptions.fSaveGameSlot == TRUE)
	{
		NUM_SLOT = NUM_SAVE_GAMES_NEW;			
		SLG_SAVELOCATION_HEIGHT 			= 15;
		SLG_GAP_BETWEEN_LOCATIONS 			= 20;
		SLG_DATE_OFFSET_Y 					= 5;
		SLG_SELECTED_SLOT_GRAPHICS_NUMBER	= 1;
		SLG_UNSELECTED_SLOT_GRAPHICS_NUMBER	= 0;
	}
	else
	{
		NUM_SLOT = NUM_SAVE_GAMES_OLD;
		SLG_SAVELOCATION_HEIGHT 			= 30;
		SLG_GAP_BETWEEN_LOCATIONS			= 35;
		SLG_DATE_OFFSET_Y 					= 11;
		SLG_SELECTED_SLOT_GRAPHICS_NUMBER	= 3;
		SLG_UNSELECTED_SLOT_GRAPHICS_NUMBER	= 2;
	}
}

UINT32	SaveLoadScreenInit()
{
	//Set so next time we come in, we can set up
	gfSaveLoadScreenEntry = TRUE;

	memset( gbSaveGameArray, -1, NUM_SAVE_GAMES);

	ClearSelectedSaveSlot();

	gbHighLightedLocation=-1;

	return( TRUE );
}

UINT32	SaveLoadScreenHandle()
{
	StartFrameBufferRender();

	if( gfSaveLoadScreenEntry )
	{
		EnterSaveLoadScreen();
		gfSaveLoadScreenEntry = FALSE;
		gfSaveLoadScreenExit = FALSE;

		PauseGame();

		//save the new rect
		BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	}

	RestoreBackgroundRects();

	//to guarentee that we do not accept input when we are fading out
	if( !gfStartedFadingOut )
	{
		GetSaveLoadScreenUserInput();
	}
	else
		gfRedrawSaveLoadScreen = FALSE;

	//if we have exited the save load screen, exit
	if( !gfSaveLoadScreenButtonsCreated )
		return( guiSaveLoadExitScreen );

	RenderAllTextFields();

	HandleSaveLoadScreen();

	if( gfRedrawSaveLoadScreen )
	{
		RenderSaveLoadScreen();
		MarkButtonsDirty( );
		RenderButtons();

		gfRedrawSaveLoadScreen = FALSE;
	}

	if( gubSaveGameNextPass != 0 )
	{
		gubSaveGameNextPass++;

		if( gubSaveGameNextPass == 5 )
		{
			gubSaveGameNextPass = 0;
			SaveLoadGameNumber( gbSelectedSaveLocation );
		}
	}

	//If we are not exiting the screen, render the buttons
	if( !gfSaveLoadScreenExit && guiSaveLoadExitScreen == SAVE_LOAD_SCREEN )
	{
		// render buttons marked dirty
		RenderButtons( );
	}

	// ATE: Put here to save RECTS before any fast help being drawn...
	SaveBackgroundRects( );
	RenderButtonsFastHelp();

	ExecuteBaseDirtyRectQueue( );
	EndFrameBufferRender( );

	if ( HandleFadeOutCallback( ) )
	{
		return( guiSaveLoadExitScreen );
	}

	if ( HandleBeginFadeOut( SAVE_LOAD_SCREEN ) )
	{
		return( SAVE_LOAD_SCREEN );
	}

	if( gfSaveLoadScreenExit )
	{
		ExitSaveLoadScreen();
	}

	if ( HandleFadeInCallback( ) )
	{
		// Re-render the scene!
		RenderSaveLoadScreen();
	}

	if ( HandleBeginFadeIn( SAVE_LOAD_SCREEN ) )
	{
	}

	return( guiSaveLoadExitScreen );
}

UINT32	SaveLoadScreenShutdown()
{
	return( TRUE );
}

void SetSaveLoadExitScreen( UINT32 uiScreen )
{
	if( uiScreen == GAME_SCREEN )
	{
		EnterTacticalScreen( );
	}

	gfSaveLoadScreenExit	= TRUE;

	guiSaveLoadExitScreen = uiScreen;

	SetPendingNewScreen( uiScreen );

	ExitSaveLoadScreen();

	DestroySaveLoadTextInputBoxes();
}

void InitMSysButtons(BOOLEAN delRegion)
{
	UINT16	usPosX = SLG_FIRST_SAVED_SPOT_X;
	UINT16	usPosY = SLG_FIRST_SAVED_SPOT_Y;
	UINT8	i;
	CHAR16	zString[512]; 
	CHAR16	zString2[512];
  
	if ( delRegion == TRUE )
	{
		for(i=0; i<NUM_SLOT; i++)
		{
			MSYS_RemoveRegion( &gSelectedSaveRegion[i]);
		}
	}

	usPosX = SLG_FIRST_SAVED_SPOT_X;
	usPosY = SLG_FIRST_SAVED_SPOT_Y;

	for(i=0; i<NUM_SLOT; i++)
	{
		MSYS_DefineRegion( &gSelectedSaveRegion[i], usPosX, usPosY, (UINT16)(usPosX+SLG_SAVELOCATION_WIDTH), (UINT16)(usPosY+SLG_SAVELOCATION_HEIGHT), MSYS_PRIORITY_HIGH,
								CURSOR_NORMAL, SelectedSaveRegionMovementCallBack, SelectedSaveRegionCallBack );
		MSYS_AddRegion(&gSelectedSaveRegion[i]);
		MSYS_SetRegionUserData( &gSelectedSaveRegion[ i ], 0, i);
		
		if ( PAGE_SLOT == 0 && i == 0 )
		{			
			swprintf( zString, pMessageStrings[MSG_SAVE_QUICKSAVE_SLOT] );
		}
		else if (  PAGE_SLOT == 0 && i >= SAVE__TIMED_AUTOSAVE_SLOT1 && i <= SAVE__TIMED_AUTOSAVE_SLOT5 )
		{
			swprintf( zString, pMessageStrings[MSG_SAVE_AUTOSAVE_SLOT], VAL_SLOT_START + i );
		}
		else if (  PAGE_SLOT == 0 && i >= SAVE__END_TURN_NUM_1 && i <= SAVE__END_TURN_NUM_2 )
		{
			swprintf( zString, pMessageStrings[MSG_SAVE_AUTOSAVE_ENDTURN_SLOT], VAL_SLOT_START - SAVE__TIMED_AUTOSAVE_SLOT5 - 1 + i );
		}
		else
		{
			swprintf( zString, pMessageStrings[MSG_SAVE_NORMAL_SLOT], VAL_SLOT_START + i - SAVE__END_TURN_NUM_2 );
		}
			
		wcscpy( zString2,zString );
		SetRegionFastHelpText( &gSelectedSaveRegion[ i ], zString2 );

		//if we are to Load a game
		if( !gfSaveGame )
		{
			//We cannot load a game that hasnt been saved
			if( !gbSaveGameArray[ VAL_SLOT_START + i ] )
				MSYS_DisableRegion( &gSelectedSaveRegion[ i ] );
		}

		usPosY += SLG_GAP_BETWEEN_LOCATIONS;
	}	
}

BOOLEAN	NewEnterSaveLoadScreen()
{
	VOBJECT_DESC	VObjectDesc;
	UINT16 usPosX = SLG_FIRST_SAVED_SPOT_X;
	UINT16 usPosY = SLG_FIRST_SAVED_SPOT_Y;
	
	RenderImage = FALSE;

	if (guiBackGroundAddOns_New != -1)
	DeleteVideoObjectFromIndex( guiBackGroundAddOns_New );
	
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\LOADSCREENADDONS_SAVEGAME.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiBackGroundAddOns_New));

	//init the list
	InitSaveGameArray();

	//Clear out all the saved background rects
	EmptyBackgroundRects( );

	//if the user has asked to load the selected save
	if( gfLoadGameUponEntry )
	{
		//make sure the save is valid
		if( gGameSettings.bLastSavedGameSlot != -1 && gbSaveGameArray[ gGameSettings.bLastSavedGameSlot ] )
		{
			gbSelectedSaveLocation = gGameSettings.bLastSavedGameSlot;

			//load the saved game
			ConfirmLoadSavedGameMessageBoxCallBack( MSG_BOX_RETURN_YES );
		}
		else
		{ //else the save isnt valid, so dont load it
			gfLoadGameUponEntry = FALSE;
		}
	}

	if (PAGE_SLOT == 0 || PAGE_SLOT < 0 )
	DisableButton( guiPrevButton );
	
	if (PAGE_SLOT == MAX_PAGE_SLOT || PAGE_SLOT > MAX_PAGE_SLOT )
	DisableButton( guiNextButton );

	SpecifyDisabledButtonStyle( guiSlgSaveLoadBtn, DISABLED_STYLE_HATCHED );

	if( gbSetSlotToBeSelected == -1 )
		DisableButton( guiSlgSaveLoadBtn );
	
	InitMSysButtons(TRUE);

	ClearSelectedSaveSlot();

	//Reset the highlight
	gbHighLightedLocation = -1;

	gzGameDescTextField[0] = '\0';

	SpecifyDisabledButtonStyle( guiSlgSaveLoadBtn, DISABLED_STYLE_HATCHED );

	//if the last saved game slot is ok, set the selected slot to the last saved slot]
	if( gGameSettings.bLastSavedGameSlot != -1 )
	{
		//if the slot is valid
		if( gbSaveGameArray[ gGameSettings.bLastSavedGameSlot ] )
		{
			SAVED_GAME_HEADER SaveGameHeader;

			memset( &SaveGameHeader, 0, sizeof( SAVED_GAME_HEADER ) );

			//if it is not the Quick Save slot, and we are loading
			if( !gfSaveGame || gfSaveGame && gGameSettings.bLastSavedGameSlot != 0 )
			{
				gbSelectedSaveLocation = gGameSettings.bLastSavedGameSlot%NUM_SLOT;
				gbSaveGameSelectedLocation[ gbSelectedSaveLocation ] = SLG_SELECTED_SLOT_GRAPHICS_NUMBER;

				//load the save gamed header string

				//Get the heade for the saved game
				if( !LoadSavedGameHeader( gbSelectedSaveLocation, &SaveGameHeader ) )
				{
					memset( &SaveGameHeader, 0, sizeof( SAVED_GAME_HEADER ) );
					gbSaveGameSelectedLocation[ gbSelectedSaveLocation ] = SLG_UNSELECTED_SLOT_GRAPHICS_NUMBER;

					if (gbSelectedSaveLocation != SAVE__END_TURN_NUM)
					{
						gbSaveGameArray[ gbSelectedSaveLocation ] = FALSE;
					}

					gbSelectedSaveLocation = gGameSettings.bLastSavedGameSlot = -1;
				}

				wcscpy( gzGameDescTextField, SaveGameHeader.sSavedGameDesc );
			}
		}
	}

	//if we are loading and the there is no slot selected
	if( gbSelectedSaveLocation == -1 )
		DisableButton( guiSlgSaveLoadBtn );
	else
		EnableButton( guiSlgSaveLoadBtn );
	
	RenderSaveLoadScreen();

	// Save load buttons are created
	gfSaveLoadScreenButtonsCreated = TRUE;

	gfDoingQuickLoad = FALSE;

	//reset
	gfStartedFadingOut = FALSE;

	DisableScrollMessages();

	gfLoadedGame = FALSE;

	if( gfLoadGameUponEntry )
	{
		UINT32	uiDestPitchBYTES;
		UINT8	*pDestBuf;

		//unmark the 2 buttons from being dirty
		ButtonList[ guiSlgCancelBtn ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ guiSlgSaveLoadBtn ]->uiFlags |= BUTTON_FORCE_UNDIRTY;

		// CLEAR THE FRAME BUFFER
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		memset(pDestBuf, 0, SCREEN_HEIGHT * uiDestPitchBYTES );
		UnLockVideoSurface( FRAME_BUFFER );

		// CLEAR THE guiRENDERBUFFER
		pDestBuf = LockVideoSurface( guiRENDERBUFFER, &uiDestPitchBYTES );
		memset(pDestBuf, 0, SCREEN_HEIGHT * uiDestPitchBYTES );
		UnLockVideoSurface( guiRENDERBUFFER );
	}

	gfGettingNameFromSaveLoadScreen = FALSE;

	return( TRUE );
}

BOOLEAN	EnterSaveLoadScreen()
{
	VOBJECT_DESC	VObjectDesc;
	UINT16 usPosX = SLG_FIRST_SAVED_SPOT_X;
	UINT16 usPosY = SLG_FIRST_SAVED_SPOT_Y;
	
	PAGE_SLOT = 0;
	VAL_SLOT_START = 0;
	RenderImage = TRUE;

	// this is where we came from; if loading of resources fails, this is also where we go next/back
	extern UINT32 guiPreviousScreen;
	guiSaveLoadExitScreen = guiPreviousScreen;

	//init the list
	InitSaveGameArray();

	//Clear out all the saved background rects
	EmptyBackgroundRects( );

	//if the user has asked to load the selected save
	if( gfLoadGameUponEntry )
	{
		//make sure the save is valid
		if( gGameSettings.bLastSavedGameSlot != -1 && gbSaveGameArray[ gGameSettings.bLastSavedGameSlot ] )
		{
			gbSelectedSaveLocation = gGameSettings.bLastSavedGameSlot;

			//load the saved game
			ConfirmLoadSavedGameMessageBoxCallBack( MSG_BOX_RETURN_YES );
		}
		else
		{ //else the save isnt valid, so dont load it
			gfLoadGameUponEntry = FALSE;
		}
	}

	// load Main background	graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\LoadScreen.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiSlgBackGroundImage));

	// load Load Screen Add ons graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_LOADSAVEHEADER );
	CHECKF(AddVideoObject(&VObjectDesc, &guiBackGroundAddOns));
	
	if (gGameExternalOptions.fSaveGameSlot == TRUE)
	{
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\LOADSCREENADDONS_SAVEGAME.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiBackGroundAddOns_New));
	}

	guiSlgButtonImage = LoadButtonImage("INTERFACE\\LoadScreenAddOns.sti", -1,6,-1,9,-1 );
	usPosX = SLG_LOAD_CANCEL_POS_X;

	guiSlgCancelBtn = CreateIconAndTextButton( guiSlgButtonImage, zSaveLoadText[SLG_CANCEL], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													usPosX, SLG_CANCEL_POS_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnSlgCancelCallback );

	//Either the save or load button
	if( gfSaveGame )
	{

		//If we are saving, dont have the save game button
		guiSaveLoadImage = UseLoadedButtonImage( guiSlgButtonImage, -1,5,-1,8,-1 );

		guiSlgSaveLoadBtn = CreateIconAndTextButton( guiSaveLoadImage, zSaveLoadText[SLG_SAVE_GAME], OPT_BUTTON_FONT,
														OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
														OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														SLG_SAVE_LOAD_BTN_POS_X, SLG_SAVE_LOAD_BTN_POS_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														DEFAULT_MOVE_CALLBACK, BtnSlgSaveLoadCallback );
	}
	else
	{
		guiSaveLoadImage = UseLoadedButtonImage( guiSlgButtonImage, -1,4,-1,7,-1 );

		guiSlgSaveLoadBtn = CreateIconAndTextButton( guiSaveLoadImage, zSaveLoadText[SLG_LOAD_GAME], OPT_BUTTON_FONT,
														OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
														OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														SLG_SAVE_LOAD_BTN_POS_X, SLG_SAVE_LOAD_BTN_POS_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														DEFAULT_MOVE_CALLBACK, BtnSlgSaveLoadCallback );
	}
	
	guiNewButtonImage = LoadButtonImage("INTERFACE\\LoadScreenButtons.sti", -1,0,-1,1,-1 );

	guiPrevButton = CreateIconAndTextButton( guiNewButtonImage, zOptionsText[4], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													SLG_SAVE_LOAD_BTN_POS_X-105, SLG_SAVE_LOAD_BTN_POS_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnNewPrevButtonCallback );
													
	
	guiNextButton = CreateIconAndTextButton( guiNewButtonImage, zOptionsText[3], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													SLG_SAVE_LOAD_BTN_POS_X+395, SLG_SAVE_LOAD_BTN_POS_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnNewNextButtonCallback );
	
	if (PAGE_SLOT == 0 || PAGE_SLOT < 0 )
		DisableButton( guiPrevButton );
	
	if (PAGE_SLOT == MAX_PAGE_SLOT ||  PAGE_SLOT > MAX_PAGE_SLOT )
		DisableButton( guiNextButton );

	SpecifyDisabledButtonStyle( guiSlgSaveLoadBtn, DISABLED_STYLE_HATCHED );

	if( gbSetSlotToBeSelected == -1 )
		DisableButton( guiSlgSaveLoadBtn );
	
	InitMSysButtons(FALSE);

	//Create the screen mask to enable ability to righ click to cancel the sace game
	MSYS_DefineRegion( &gSLSEntireScreenRegion, 0, 0, SCREEN_WIDTH , SCREEN_HEIGHT , MSYS_PRIORITY_HIGH-10,
							CURSOR_NORMAL, MSYS_NO_CALLBACK, SelectedSLSEntireRegionCallBack );
	MSYS_AddRegion(&gSLSEntireScreenRegion);

	ClearSelectedSaveSlot();

	//Remove the mouse region over the clock
	RemoveMouseRegionForPauseOfClock(	);

	//Reset the highlight
	gbHighLightedLocation = -1;

	gzGameDescTextField[0] = '\0';

	SpecifyDisabledButtonStyle( guiSlgSaveLoadBtn, DISABLED_STYLE_HATCHED );

	//if the last saved game slot is ok, set the selected slot to the last saved slot]
	if( gGameSettings.bLastSavedGameSlot != -1 )
	{
		//if the slot is valid
		if( gbSaveGameArray[ gGameSettings.bLastSavedGameSlot ] )
		{
			SAVED_GAME_HEADER SaveGameHeader;

			memset( &SaveGameHeader, 0, sizeof( SAVED_GAME_HEADER ) );

			//if it is not the Quick Save slot, and we are loading
			if( !gfSaveGame || gfSaveGame && gGameSettings.bLastSavedGameSlot != 0 )
			{
				gbSelectedSaveLocation = gGameSettings.bLastSavedGameSlot;
				gbSaveGameSelectedLocation[ gbSelectedSaveLocation ] = SLG_SELECTED_SLOT_GRAPHICS_NUMBER;

				//Get the heade for the saved game
				if( !LoadSavedGameHeader( gbSelectedSaveLocation, &SaveGameHeader ) )
				{
					memset( &SaveGameHeader, 0, sizeof( SAVED_GAME_HEADER ) );
					gbSaveGameSelectedLocation[ gbSelectedSaveLocation ] = SLG_UNSELECTED_SLOT_GRAPHICS_NUMBER;

					if (gbSelectedSaveLocation != SAVE__END_TURN_NUM)
					{
						gbSaveGameArray[ gbSelectedSaveLocation ] = FALSE;
					}

					gbSelectedSaveLocation = gGameSettings.bLastSavedGameSlot = -1;
				}

				wcscpy( gzGameDescTextField, SaveGameHeader.sSavedGameDesc );
			}
		}
	}

	//if we are loading and the there is no slot selected
	if( gbSelectedSaveLocation == -1 )
		DisableButton( guiSlgSaveLoadBtn );
	else
		EnableButton( guiSlgSaveLoadBtn );

	RenderSaveLoadScreen();

	// Save load buttons are created
	gfSaveLoadScreenButtonsCreated = TRUE;

	// OK, loading of resources was successfull; time to enter the SAVE_LOAD screen
	guiSaveLoadExitScreen = SAVE_LOAD_SCREEN;

	gfDoingQuickLoad = FALSE;

	//reset
	gfStartedFadingOut = FALSE;

	DisableScrollMessages();

	gfLoadedGame = FALSE;

	if( gfLoadGameUponEntry )
	{
		UINT32	uiDestPitchBYTES;
		UINT8	*pDestBuf;

		//unmark the 2 buttons from being dirty
		ButtonList[ guiSlgCancelBtn ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ guiSlgSaveLoadBtn ]->uiFlags |= BUTTON_FORCE_UNDIRTY;

		// CLEAR THE FRAME BUFFER
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		memset(pDestBuf, 0, SCREEN_HEIGHT * uiDestPitchBYTES );
		UnLockVideoSurface( FRAME_BUFFER );

		// CLEAR THE guiRENDERBUFFER
		pDestBuf = LockVideoSurface( guiRENDERBUFFER, &uiDestPitchBYTES );
		memset(pDestBuf, 0, SCREEN_HEIGHT * uiDestPitchBYTES );
		UnLockVideoSurface( guiRENDERBUFFER );
	}

	gfGettingNameFromSaveLoadScreen = FALSE;

	//Go to page with last saved game
	if ( gGameSettings.bLastSavedGameSlot >= NUM_SLOT)
	{
		PAGE_SLOT = gGameSettings.bLastSavedGameSlot/NUM_SLOT;
		VAL_SLOT_START = (PAGE_SLOT * NUM_SLOT);
		
		if ( PAGE_SLOT > 0 ) 
			EnableButton( guiPrevButton );
	
		if (PAGE_SLOT == MAX_PAGE_SLOT ||  PAGE_SLOT > MAX_PAGE_SLOT )
			DisableButton( guiNextButton );

		DestroySaveLoadTextInputBoxes();
		NewEnterSaveLoadScreen();
	}

	return( TRUE );
}

void ExitSaveLoadScreen()
{
	UINT8	i;
	
	PAGE_SLOT = 0;
	VAL_SLOT_START = 0;
	RenderImage = TRUE;

	gfLoadGameUponEntry = FALSE;

	if( !gfSaveLoadScreenButtonsCreated )
		return;

	gfSaveLoadScreenExit = FALSE;
	gfSaveLoadScreenEntry = TRUE;
	gfExitAfterMessageBox = FALSE;

	UnloadButtonImage( guiSlgButtonImage );

	RemoveButton( guiSlgCancelBtn );

	RemoveButton( guiNextButton );
	RemoveButton( guiPrevButton );	
	UnloadButtonImage( guiNewButtonImage );	
		
	RemoveButton( guiSlgSaveLoadBtn );
	UnloadButtonImage( guiSaveLoadImage );

	for(i=0; i<NUM_SLOT; i++)
	{
		MSYS_RemoveRegion( &gSelectedSaveRegion[i]);
	}

	DeleteVideoObjectFromIndex( guiSlgBackGroundImage );
	DeleteVideoObjectFromIndex( guiBackGroundAddOns );
	
	if (gGameExternalOptions.fSaveGameSlot == TRUE)
	{	
		DeleteVideoObjectFromIndex( guiBackGroundAddOns_New );
	}

	//Destroy the text fields ( if created )
	DestroySaveLoadTextInputBoxes();

	MSYS_RemoveRegion( &gSLSEntireScreenRegion );

	gfSaveLoadScreenEntry = TRUE;
	gfSaveLoadScreenExit = FALSE;

	if( !gfLoadedGame )
	{
		UnLockPauseState( );
		UnPauseGame();
	}

	gfSaveLoadScreenButtonsCreated = FALSE;

	gfCameDirectlyFromGame = FALSE;
	gfGettingNameFromSaveLoadScreen = FALSE;
}

void RenderSaveLoadScreen()
{
	HVOBJECT hPixHandle;

	//if we are going to be instantly leaving the screen, dont draw the numbers
	if( gfLoadGameUponEntry )
	{
		return;
	}
	if ( RenderImage == TRUE )
	{
		GetVideoObject(&hPixHandle, guiSlgBackGroundImage);
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0, iScreenWidthOffset, iScreenHeightOffset, VO_BLT_SRCTRANSPARENCY,NULL);
	}

	if( gfSaveGame )
	{
		if (RenderImage == TRUE)
		{
			GetVideoObject(&hPixHandle, guiBackGroundAddOns);
			BltVideoObject(FRAME_BUFFER, hPixHandle, 1, SLG_TITLE_POS_X, SLG_TITLE_POS_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		}
	}
	else
	{
		//Display the Title
		if ( RenderImage == TRUE )
		{
			GetVideoObject(&hPixHandle, guiBackGroundAddOns);
			BltVideoObject(FRAME_BUFFER, hPixHandle, 0, SLG_TITLE_POS_X, SLG_TITLE_POS_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		}

	}

	DisplaySaveGameList();

	InvalidateRegion( 0, 0, SCREEN_WIDTH	, SCREEN_HEIGHT	);
}

void HandleSaveLoadScreen()
{
	//If the game failed when in a message box, pop up a message box stating this
	if( gfFailedToSaveGameWhenInsideAMessageBox )
	{
		gfFailedToSaveGameWhenInsideAMessageBox = FALSE;

		DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_SAVE_GAME_ERROR], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, RedrawSaveLoadScreenAfterMessageBox );

		gbHighLightedLocation=-1;

		ClearSelectedSaveSlot();
	}
}

void GetSaveLoadScreenUserInput()
{
	InputAtom Event;
	POINT	MousePos;
	INT8		bActiveTextField;
	static BOOLEAN	fWasCtrlHeldDownLastFrame = FALSE;

	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	//if we are going to be instantly leaving the screen, dont draw the numbers
	if( gfLoadGameUponEntry )
	{
		return;
	}

	if( gfKeyState[ CTRL ] || fWasCtrlHeldDownLastFrame )
	{
		DisplaySaveGameEntry( gbSelectedSaveLocation );
	}

	fWasCtrlHeldDownLastFrame = gfKeyState[ CTRL ];

	while (DequeueSpecificEvent(&Event, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
		if( !HandleTextInput( &Event ) && Event.usEvent == KEY_DOWN )
		{
			switch( Event.usParam )
			{
				case '1':
					SetSelection( 0 );
					break;
				case '2':
					SetSelection( 1 );
					break;
				case '3':
					SetSelection( 2 );
					break;
				case '4':
					SetSelection( 3 );
					break;
				case '5':
					SetSelection( 4 );
					break;
				case '6':
					SetSelection( 5 );
					break;
				case '7':
					SetSelection( 6 );
					break;
				case '8':
					SetSelection( 7 );
					break;
				case '9':
					SetSelection( 8 );
					break;
				case '0':
					SetSelection( 9 );
					break;
				case '!':
					SetSelection( 10 );
					break;
				case '@':
					SetSelection( 11 );
					break;
				case '#':
					SetSelection( 12 );
					break;
				case '$':
					SetSelection( 13 );
					break;
				case '%':
					SetSelection( 14 );
					break;
				case '^':
					SetSelection( 15 );
					break;
				case '&':
					SetSelection( 16 );
					break;
				case '*':
					SetSelection( 17 );
					break;
				case '(':
					SetSelection( 18 );
					break;
			}
		}

		if( Event.usEvent == KEY_UP )
		{
			switch( Event.usParam )
			{
				case UPARROW:
					MoveSelectionUpOrDown( TRUE );
					break;

				case DNARROW:
					MoveSelectionUpOrDown( FALSE );
					break;
				
				case PGUP:
					// previous page, LEFTARROW not used due to interference with textbox navigation
					if ( PAGE_SLOT > 0 )
					{
						PAGE_SLOT = PAGE_SLOT - 1;
					
						if ( PAGE_SLOT == 0 || PAGE_SLOT < 0 )
							VAL_SLOT_START = 0; 
						else if ( PAGE_SLOT >= 1 )
							VAL_SLOT_START = (PAGE_SLOT * 19);
						
						if (PAGE_SLOT == 0 || PAGE_SLOT < 0 )
						{
							DisableButton( guiPrevButton );
							EnableButton( guiNextButton );
							PAGE_SLOT = 0;
						}
						else if (PAGE_SLOT < MAX_PAGE_SLOT)
						{
							EnableButton( guiNextButton );
						}

						DestroySaveLoadTextInputBoxes();

						NewEnterSaveLoadScreen();
					}
					break;
				
				case PGDN:
					// next page, RIGHTARROW not used due to interference with textbox navigation
					if ( PAGE_SLOT < MAX_PAGE_SLOT )
					{
						PAGE_SLOT = PAGE_SLOT + 1;

						if ( PAGE_SLOT >= 1 )
							VAL_SLOT_START = (PAGE_SLOT * 19);
						else if ( PAGE_SLOT >= MAX_PAGE_SLOT )
						{
							VAL_SLOT_START = (MAX_PAGE_SLOT * 19);
							PAGE_SLOT = MAX_PAGE_SLOT;
						}

						if (PAGE_SLOT > 0 )
							EnableButton( guiPrevButton );

						if (PAGE_SLOT > MAX_PAGE_SLOT || PAGE_SLOT == MAX_PAGE_SLOT )
							DisableButton( guiNextButton );

						DestroySaveLoadTextInputBoxes();

						NewEnterSaveLoadScreen();
					}
					break;
				
				case ESC:
					if( gbSelectedSaveLocation == -1 )
					{

						if( 	gfCameDirectlyFromGame )
							SetSaveLoadExitScreen( guiPreviousOptionScreen );

						else if( guiPreviousOptionScreen == MAINMENU_SCREEN )
							SetSaveLoadExitScreen( MAINMENU_SCREEN );
						else
							SetSaveLoadExitScreen( OPTIONS_SCREEN );
					}
					else
					{
						//Reset the selected slot background graphic
						gbSaveGameSelectedLocation[ gbSelectedSaveLocation ] = SLG_UNSELECTED_SLOT_GRAPHICS_NUMBER;

						//reset selected slot
						gbSelectedSaveLocation = -1;
						gfRedrawSaveLoadScreen = TRUE;
						DestroySaveLoadTextInputBoxes();

						DisableButton( guiSlgSaveLoadBtn );
					}
					break;


				case ENTER:

					if( gfSaveGame )
					{
						bActiveTextField = (INT32)GetActiveFieldID();
						if( bActiveTextField && bActiveTextField != -1 )
						{
							Get16BitStringFromField( (UINT8)bActiveTextField, gzGameDescTextField, 128 );
							SetActiveField(0);

							DestroySaveLoadTextInputBoxes();

							SaveLoadGameNumber( gbSelectedSaveLocation );
							return;
						}
						else
						{
							if( gbSelectedSaveLocation != -1 )
							{
								SaveLoadGameNumber( gbSelectedSaveLocation );
								return;
							}
						}
						//Enable the save/load button
						if( gbSelectedSaveLocation != -1 )
							if( !gfSaveGame )
								EnableButton( guiSlgSaveLoadBtn );

						gfRedrawSaveLoadScreen = TRUE;
					}
					else
						SaveLoadGameNumber( gbSelectedSaveLocation );

					break;
			}
		}
	}
}

void SaveLoadGameNumber( INT32 bSaveGameID )
{
	UINT8		ubRetVal=0;

	if( bSaveGameID >= NUM_SAVE_GAMES || bSaveGameID < 0 )
	{
		return;
	}

	if( gfSaveGame )
	{
		INT8		bActiveTextField;

		bActiveTextField = (INT32)GetActiveFieldID();
		if( bActiveTextField && bActiveTextField != -1 )
		{
			Get16BitStringFromField( (UINT8)bActiveTextField, gzGameDescTextField, 128 );
		}

		//if there is save game in the slot, ask for confirmation before overwriting
		if( gbSaveGameArray[ VAL_SLOT_START + bSaveGameID ] )
		{
			CHAR16	sText[512];

			swprintf( sText, zSaveLoadText[SLG_CONFIRM_SAVE], VAL_SLOT_START + bSaveGameID );

			DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, sText, SAVE_LOAD_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmSavedGameMessageBoxCallBack );
		}
		else
		{
			//Save the game
			SaveGameToSlotNum();
		}
	}
	else
	{
		//Check to see if the save game headers are the same
		ubRetVal = CompareSaveGameVersion( bSaveGameID );
		if( ubRetVal != SLS_HEADER_OK )
		{
			if( ubRetVal == SLS_GAME_VERSION_OUT_OF_DATE )
			{
				DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_GAME_VERSION_DIF], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_YESNO, LoadSavedGameWarningMessageBoxCallBack );
			}
			else if( ubRetVal == SLS_SAVED_GAME_VERSION_OUT_OF_DATE )
			{
				DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_SAVED_GAME_VERSION_DIF], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_YESNO, LoadSavedGameWarningMessageBoxCallBack );
			}
			else
			{
				DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_BOTH_GAME_AND_SAVED_GAME_DIF], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_YESNO, LoadSavedGameWarningMessageBoxCallBack );
			}
		}
		else
		{
			//Setup up the fade routines
			StartFadeOutForSaveLoadScreen();
		}
	}
}


BOOLEAN DoSaveLoadMessageBoxWithRect( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect )
{
	// do message box and return
	giSaveLoadMessageBox = DoMessageBox(ubStyle,zString,uiExitScreen,( UINT8 )( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),	ReturnCallback,	pCenteringRect );

	// send back return state
	return( ( giSaveLoadMessageBox != -1 ) );
}

BOOLEAN	DoSaveLoadMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect CenteringRect= {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 };

	// do message box and return
	giSaveLoadMessageBox = DoMessageBox(ubStyle,zString,uiExitScreen,( UINT8 ) ( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),ReturnCallback,	&CenteringRect );

	// send back return state
	return( ( giSaveLoadMessageBox != -1 ) );
}

BOOLEAN InitSaveGameArray()
{
	UINT8	cnt,cnt2;
	UINT8	i;
	CHAR8		zSaveGameName[ 512 ];
	SAVED_GAME_HEADER SaveGameHeader;
	
	for( i=0; i<NUM_SAVE_GAMES-1; i++)
	{
		gbSaveGameArray[i] = FALSE;
	}

	for( cnt=0; cnt<NUM_SLOT; cnt++) 
	{
	
		cnt2 = VAL_SLOT_START + cnt;
		CreateSavedGameFileNameFromNumber( cnt2, zSaveGameName );

		if( FileExists( zSaveGameName ) )
		{
			//Get the header for the saved game
			if( !LoadSavedGameHeader( cnt2, &SaveGameHeader ) )
				gbSaveGameArray[VAL_SLOT_START + cnt] = FALSE;
			else
				gbSaveGameArray[VAL_SLOT_START + cnt] = TRUE;
		}
		else
			gbSaveGameArray[VAL_SLOT_START + cnt] = FALSE;
	}

	return( TRUE );
}


BOOLEAN DisplaySaveGameList()
{
	UINT8	bLoop1;
	UINT16 usPosY = SLG_FIRST_SAVED_SPOT_Y;

	for( bLoop1=0; bLoop1<NUM_SLOT; bLoop1++)
	{
		//display all the information from the header
		DisplaySaveGameEntry( bLoop1 );
		usPosY += SLG_GAP_BETWEEN_LOCATIONS;
	}

	return( TRUE );
}

BOOLEAN DisplaySaveGameEntry( INT32 bEntryID )
{
	CHAR16		zDateString[128];
	CHAR16		zLocationString[128];
	CHAR16		zNumMercsString[128];
	CHAR16		zBalanceString[128];
	SAVED_GAME_HEADER SaveGameHeader;
	HVOBJECT hPixHandle;
	UINT16		usPosX=SLG_FIRST_SAVED_SPOT_X;
	UINT32		uiFont= SAVE_LOAD_TITLE_FONT;
	UINT8			ubFontColor=SAVE_LOAD_TITLE_COLOR;
	UINT16		usPosY = SLG_FIRST_SAVED_SPOT_Y + ( SLG_GAP_BETWEEN_LOCATIONS * bEntryID );
	
	CHAR16		zString[128];
		
	//if we are going to be instantly leaving the screen, dont draw the numbers
	if( gfLoadGameUponEntry )
	{
		return( TRUE );
	}

	if( bEntryID == -1 )
		return( TRUE );

	//if we are currently fading out, leave
	if( gfStartedFadingOut )
		return( TRUE );

	//background
	if (gGameExternalOptions.fSaveGameSlot == TRUE)
	{
		GetVideoObject(&hPixHandle, guiBackGroundAddOns_New);
		BltVideoObject(FRAME_BUFFER, hPixHandle, gbSaveGameSelectedLocation[ bEntryID ], usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);
	}
	else
	{
		GetVideoObject(&hPixHandle, guiBackGroundAddOns);
		BltVideoObject(FRAME_BUFFER, hPixHandle, gbSaveGameSelectedLocation[ bEntryID ], usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);
	}

	//if its the QuickSave slot
	if( bEntryID == 0 && gfSaveGame && PAGE_SLOT == 0 )
	{
		SetFontShadow( SAVE_LOAD_QUICKSAVE_SHADOW_COLOR );
		ubFontColor = SAVE_LOAD_QUICKSAVE_COLOR;
		uiFont = SAVE_LOAD_QUICKSAVE_FONT;

		//Shadow the slot
		ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX, usPosY, usPosX+SLG_SAVELOCATION_WIDTH, usPosY+SLG_SAVELOCATION_HEIGHT );
	}
	
	//Auto Save
	if( bEntryID == SAVE__TIMED_AUTOSAVE_SLOT1 && gfSaveGame && PAGE_SLOT == 0 )
	{
		SetFontShadow( SAVE_LOAD_QUICKSAVE_SHADOW_COLOR );
		ubFontColor = SAVE_LOAD_QUICKSAVE_COLOR;
		uiFont = SAVE_LOAD_QUICKSAVE_FONT;

		ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX, usPosY, usPosX+SLG_SAVELOCATION_WIDTH, usPosY+SLG_SAVELOCATION_HEIGHT );
	}
	
	if( bEntryID == SAVE__TIMED_AUTOSAVE_SLOT2 && gfSaveGame && PAGE_SLOT == 0 )
	{
		SetFontShadow( SAVE_LOAD_QUICKSAVE_SHADOW_COLOR );
		ubFontColor = SAVE_LOAD_QUICKSAVE_COLOR;
		uiFont = SAVE_LOAD_QUICKSAVE_FONT;

		ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX, usPosY, usPosX+SLG_SAVELOCATION_WIDTH, usPosY+SLG_SAVELOCATION_HEIGHT );
	}
	
	if( bEntryID == SAVE__TIMED_AUTOSAVE_SLOT3 && gfSaveGame && PAGE_SLOT == 0 )
	{
		SetFontShadow( SAVE_LOAD_QUICKSAVE_SHADOW_COLOR );
		ubFontColor = SAVE_LOAD_QUICKSAVE_COLOR;
		uiFont = SAVE_LOAD_QUICKSAVE_FONT;

		ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX, usPosY, usPosX+SLG_SAVELOCATION_WIDTH, usPosY+SLG_SAVELOCATION_HEIGHT );
	}
	
	if( bEntryID == SAVE__TIMED_AUTOSAVE_SLOT4 && gfSaveGame && PAGE_SLOT == 0 )
	{
		SetFontShadow( SAVE_LOAD_QUICKSAVE_SHADOW_COLOR );
		ubFontColor = SAVE_LOAD_QUICKSAVE_COLOR;
		uiFont = SAVE_LOAD_QUICKSAVE_FONT;

		ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX, usPosY, usPosX+SLG_SAVELOCATION_WIDTH, usPosY+SLG_SAVELOCATION_HEIGHT );
	}
	
	if( bEntryID == SAVE__TIMED_AUTOSAVE_SLOT5 && gfSaveGame && PAGE_SLOT == 0 )
	{
		SetFontShadow( SAVE_LOAD_QUICKSAVE_SHADOW_COLOR );
		ubFontColor = SAVE_LOAD_QUICKSAVE_COLOR;
		uiFont = SAVE_LOAD_QUICKSAVE_FONT;

		ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX, usPosY, usPosX+SLG_SAVELOCATION_WIDTH, usPosY+SLG_SAVELOCATION_HEIGHT );
	}
	//Auto save end turn
	if( bEntryID == SAVE__END_TURN_NUM_1 && gfSaveGame && PAGE_SLOT == 0 )
	{
		SetFontShadow( SAVE_LOAD_QUICKSAVE_SHADOW_COLOR );
		ubFontColor = SAVE_LOAD_QUICKSAVE_COLOR;
		uiFont = SAVE_LOAD_QUICKSAVE_FONT;

		ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX, usPosY, usPosX+SLG_SAVELOCATION_WIDTH, usPosY+SLG_SAVELOCATION_HEIGHT );
	}
	
	if( bEntryID == SAVE__END_TURN_NUM_2 && gfSaveGame && PAGE_SLOT == 0 )
	{
		SetFontShadow( SAVE_LOAD_QUICKSAVE_SHADOW_COLOR );
		ubFontColor = SAVE_LOAD_QUICKSAVE_COLOR;
		uiFont = SAVE_LOAD_QUICKSAVE_FONT;

		ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX, usPosY, usPosX+SLG_SAVELOCATION_WIDTH, usPosY+SLG_SAVELOCATION_HEIGHT );
	}

	//else if its the currently selected location
	if( bEntryID == gbSelectedSaveLocation )
	{
		SetFontShadow( SAVE_LOAD_SELECTED_SHADOW_COLOR );
		ubFontColor = SAVE_LOAD_SELECTED_COLOR;
		uiFont = SAVE_LOAD_SELECTED_FONT;
	}

	//else it is the highlighted slot
	if( bEntryID == gbHighLightedLocation )
	{
		SetFontShadow( SAVE_LOAD_HIGHLIGHTED_SHADOW_COLOR );
		ubFontColor = SAVE_LOAD_HIGHLIGHTED_COLOR;
		uiFont = SAVE_LOAD_HIGHLIGHTED_FONT;
	}

	//if the file doesnt exists
	if( !gbSaveGameArray[ VAL_SLOT_START + bEntryID ] )
	{
		//if we are loading a game
		if( !gfSaveGame )
		{
			SetFontShadow( SAVE_LOAD_QUICKSAVE_SHADOW_COLOR );
			ubFontColor = SAVE_LOAD_QUICKSAVE_COLOR;
			uiFont = SAVE_LOAD_QUICKSAVE_FONT;

			//Shadow the surface
			ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX, usPosY, usPosX+SLG_SAVELOCATION_WIDTH, usPosY+SLG_SAVELOCATION_HEIGHT );
		}
		else
		{
			SetFontShadow( SAVE_LOAD_EMPTYSLOT_SHADOW_COLOR		);
			ubFontColor = SAVE_LOAD_EMPTYSLOT_COLOR;
			uiFont = SAVE_LOAD_EMPTYSLOT_FONT;
		}
	}
	else
	{
		SetFontShadow( SAVE_LOAD_NORMAL_SHADOW_COLOR );
		ubFontColor = SAVE_LOAD_NORMAL_COLOR;
		uiFont = SAVE_LOAD_NORMAL_FONT;
	}

	//if the file exists
	if( gbSaveGameArray[ VAL_SLOT_START + bEntryID ] || gbSelectedSaveLocation == bEntryID )
	{
		//
		// Setup the strings to be displayed
		//

		//if we are saving AND it is the currently selected slot
		if( gfSaveGame && gbSelectedSaveLocation == bEntryID )
		{
			//the user has selected a spot to save.	Fill out all the required information
			SaveGameHeader.uiDay = GetWorldDay();
			SaveGameHeader.ubHour = (UINT8)GetWorldHour();
			SaveGameHeader.ubMin = (UINT8)guiMin;

			//Get the sector value to save.
			GetBestPossibleSectorXYZValues( &SaveGameHeader.sSectorX, &SaveGameHeader.sSectorY, &SaveGameHeader.bSectorZ );

			SaveGameHeader.ubNumOfMercsOnPlayersTeam = NumberOfMercsOnPlayerTeam();
			SaveGameHeader.iCurrentBalance = LaptopSaveInfo.iCurrentBalance;
			wcscpy( SaveGameHeader.sSavedGameDesc, gzGameDescTextField );

			//copy over the initial game options
			memcpy( &SaveGameHeader.sInitialGameOptions, &gGameOptions, sizeof( GAME_OPTIONS ) );
		}
		else
		{
			//Get the header for the specified saved game
			if( !LoadSavedGameHeader( bEntryID, &SaveGameHeader ) )
			{
				memset( &SaveGameHeader, 0, sizeof( SaveGameHeader ) );
				return( FALSE );
			}
		}

		//if this is the currently selected location, move the text down a bit
		if( gbSelectedSaveLocation == bEntryID )
		{
			usPosX++;
			usPosY--;
		}

		//if the user is LOADING and holding down the CTRL key, display the additional info
		if( !gfSaveGame && gfKeyState[ CTRL ] && gbSelectedSaveLocation == bEntryID )
		{
			CHAR16		zMouseHelpTextString[256];
			CHAR16		zDifString[256];

			//Create a string for difficulty level
			swprintf( zDifString, L"%s %s", gzGIOScreenText[ GIO_EASY_TEXT + SaveGameHeader.sInitialGameOptions.ubDifficultyLevel - 1 ], zSaveLoadText[ SLG_DIFF ] );

			//make a string containing the extended options
			swprintf( zMouseHelpTextString, L"%20ls     %d     %d     %22ls     %22ls", zDifString,
				SaveGameHeader.sInitialGameOptions.ubBobbyRayQuality, 
				SaveGameHeader.sInitialGameOptions.ubBobbyRayQuantity, 
				SaveGameHeader.sInitialGameOptions.fGunNut ? zSaveLoadText[ SLG_ADDITIONAL_GUNS ] : zSaveLoadText[ SLG_NORMAL_GUNS ],
				SaveGameHeader.sInitialGameOptions.ubGameStyle == STYLE_SCIFI ? zSaveLoadText[ SLG_SCIFI ] : (SaveGameHeader.sInitialGameOptions.ubGameStyle == STYLE_PLATINUM ? zSaveLoadText[ SLG_PLATINUM ]: zSaveLoadText[ SLG_REALISTIC ]) );

			//The date
			DrawTextToScreen( zMouseHelpTextString, (UINT16)(usPosX+SLG_DATE_OFFSET_X), (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 0, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);
		}
		else
		{
			//Create the string for the Data
			if(is_networked)
				swprintf( zDateString, L"%s %d / %d", pMessageStrings[ MSG_CLIENT ], SaveGameHeader.uiDay, SaveGameHeader.ubHour );
			else
				swprintf( zDateString, L"%s %d, %02d:%02d", pMessageStrings[ MSG_DAY ], SaveGameHeader.uiDay, SaveGameHeader.ubHour, SaveGameHeader.ubMin );

			//Create the string for the current location
			if( SaveGameHeader.sSectorX == -1 && SaveGameHeader.sSectorY == -1 || SaveGameHeader.bSectorZ < 0 )
			{
				if( ( SaveGameHeader.uiDay * NUM_SEC_IN_DAY + SaveGameHeader.ubHour * NUM_SEC_IN_HOUR + SaveGameHeader.ubMin * NUM_SEC_IN_MIN ) <= gGameExternalOptions.iGameStartingTime )
					swprintf( zLocationString, gpStrategicString[ STR_PB_NOTAPPLICABLE_ABBREVIATION ] );
				else
					swprintf( zLocationString, gzLateLocalizedString[14] );
			}
			else
			{
				gfGettingNameFromSaveLoadScreen = TRUE;

				GetSectorIDString( SaveGameHeader.sSectorX, SaveGameHeader.sSectorY, SaveGameHeader.bSectorZ, zLocationString, FALSE );

				gfGettingNameFromSaveLoadScreen = FALSE;
			}

			//
			// Number of mercs on the team
			//
			if(!is_networked)
			{
				//if only 1 merc is on the team
				if( SaveGameHeader.ubNumOfMercsOnPlayersTeam == 1 )
				{
					//use "merc"
					swprintf( zNumMercsString, L"%d %s", SaveGameHeader.ubNumOfMercsOnPlayersTeam, MercAccountText[ MERC_ACCOUNT_MERC ] );
				}
				else
				{
					//use "mercs"
					swprintf( zNumMercsString, L"%d %s", SaveGameHeader.ubNumOfMercsOnPlayersTeam, pMessageStrings[ MSG_MERCS ] );
				}
			}
			else
			{
				//if only 1 merc is on the team
				if( SaveGameHeader.ubMin == 1 )
				{
					//use "merc"
					swprintf( zNumMercsString, L"%d / %d %s", SaveGameHeader.ubNumOfMercsOnPlayersTeam,SaveGameHeader.ubMin, MercAccountText[ MERC_ACCOUNT_MERC ] );
				}
				else
				{
					//use "mercs"
					swprintf( zNumMercsString, L"%d / %d %s", SaveGameHeader.ubNumOfMercsOnPlayersTeam,SaveGameHeader.ubMin, pMessageStrings[ MSG_MERCS ] );
				}
			}

			//Get the current balance
			swprintf( zBalanceString, L"%d", SaveGameHeader.iCurrentBalance);
			InsertCommasForDollarFigure( zBalanceString );
			InsertDollarSignInToString( zBalanceString );

			//
			// Display the Saved game information
			//

			//The date
			DrawTextToScreen( zDateString, (UINT16)(usPosX+SLG_DATE_OFFSET_X), (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 0, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);

			//if the sector string exceeds the width, and the ...
			ReduceStringLength( zLocationString, SLG_SECTOR_WIDTH, uiFont );

			//The Sector
			DrawTextToScreen( zLocationString, (UINT16)(usPosX+SLG_SECTOR_OFFSET_X), (UINT16)(usPosY+SLG_SECTOR_OFFSET_Y), 0, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);

			//The Num of mercs
			DrawTextToScreen( zNumMercsString, (UINT16)(usPosX+SLG_NUM_MERCS_OFFSET_X), (UINT16)(usPosY+SLG_NUM_MERCS_OFFSET_Y), 0, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);

			//The balance
			if(!is_networked)
				DrawTextToScreen( zBalanceString, (UINT16)(usPosX+SLG_BALANCE_OFFSET_X), (UINT16)(usPosY+SLG_BALANCE_OFFSET_Y), 0, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);

			if( gbSaveGameArray[ VAL_SLOT_START + bEntryID ] || ( gfSaveGame && !gfUserInTextInputMode && ( gbSelectedSaveLocation == bEntryID ) ) )
			{
				//The Saved Game description
				DrawTextToScreen( SaveGameHeader.sSavedGameDesc, (UINT16)(usPosX+SLG_SAVE_GAME_DESC_X), (UINT16)(usPosY+SLG_SAVE_GAME_DESC_Y), 0, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);
			}
		}
	}
	else
	{
		//if this is the quick save slot
		if( bEntryID == 0 && PAGE_SLOT == 0 )
		{
			//display the empty spot
			DrawTextToScreen( pMessageStrings[ MSG_EMPTY_QUICK_SAVE_SLOT ], usPosX, (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 609, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		}
		else if( bEntryID == SAVE__TIMED_AUTOSAVE_SLOT1 && PAGE_SLOT == 0 )
		{
			//display the empty spot
			swprintf( zString, L"%s%d", pMessageStrings[ MSG_SAVE_AUTOSAVE_EMPTY_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT1);
			DrawTextToScreen( zString, usPosX, (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 609, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		}
		else if( bEntryID == SAVE__TIMED_AUTOSAVE_SLOT2 && PAGE_SLOT == 0 )
		{
			//display the empty spot
			swprintf( zString, L"%s%d", pMessageStrings[ MSG_SAVE_AUTOSAVE_EMPTY_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT2);
			DrawTextToScreen( zString, usPosX, (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 609, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		}
		else if( bEntryID == SAVE__TIMED_AUTOSAVE_SLOT3 && PAGE_SLOT == 0 )
		{
			//display the empty spot
			swprintf( zString, L"%s%d", pMessageStrings[ MSG_SAVE_AUTOSAVE_EMPTY_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT3);
			DrawTextToScreen( zString, usPosX, (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 609, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		}
		else if( bEntryID == SAVE__TIMED_AUTOSAVE_SLOT4 && PAGE_SLOT == 0 )
		{
			//display the empty spot
			swprintf( zString, L"%s%d", pMessageStrings[ MSG_SAVE_AUTOSAVE_EMPTY_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT4);
			DrawTextToScreen( zString, usPosX, (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 609, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		}
		else if( bEntryID == SAVE__TIMED_AUTOSAVE_SLOT5 && PAGE_SLOT == 0 )
		{
			//display the empty spot
			swprintf( zString, L"%s%d", pMessageStrings[ MSG_SAVE_AUTOSAVE_EMPTY_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT5);
			DrawTextToScreen( zString, usPosX, (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 609, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		}
		else if( bEntryID == SAVE__END_TURN_NUM_1 && PAGE_SLOT == 0 )
		{
			//display the empty spot
			swprintf( zString, L"%s%d", pMessageStrings[ MSG_SAVE_AUTOSAVE_ENDTURN_EMPTY_TEXT ],SAVE__END_TURN_NUM_1 - SAVE__TIMED_AUTOSAVE_SLOT5);
			DrawTextToScreen( zString, usPosX, (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 609, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		}
		else if( bEntryID == SAVE__END_TURN_NUM_2 && PAGE_SLOT == 0 )
		{
			//display the empty spot
			swprintf( zString, L"%s%d", pMessageStrings[ MSG_SAVE_AUTOSAVE_ENDTURN_EMPTY_TEXT ],SAVE__END_TURN_NUM_2 - SAVE__TIMED_AUTOSAVE_SLOT5);
			DrawTextToScreen( zString, usPosX, (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 609, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		}
		else
		{
			//display the empty spot
			DrawTextToScreen( pMessageStrings[ MSG_EMPTYSLOT ], usPosX, (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 609, uiFont, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		}
	}

	//REset the shadow color
	SetFontShadow(DEFAULT_SHADOW);

	usPosX=SLG_FIRST_SAVED_SPOT_X;
	usPosY = SLG_FIRST_SAVED_SPOT_Y + ( SLG_GAP_BETWEEN_LOCATIONS * bEntryID );

	InvalidateRegion( usPosX, usPosY, usPosX+SLG_SAVELOCATION_WIDTH, usPosY+SLG_SAVELOCATION_HEIGHT );

	return( TRUE );
}

BOOLEAN LoadSavedGameHeader( INT32 bEntry, SAVED_GAME_HEADER *pSaveGameHeader )
{
	HWFILE hFile;
	CHAR8		zSavedGameName[512];
	UINT32	uiNumBytesRead;
	UINT8 newSlot;

	//make sure the entry is valid
	if( bEntry < 0 || bEntry > NUM_SAVE_GAMES /*NUM_SLOT*/ )
	{
		//memset( &pSaveGameHeader, 0, sizeof( SAVED_GAME_HEADER ) );
		memset( pSaveGameHeader, 0, sizeof( SAVED_GAME_HEADER ) );
		return( FALSE );
	}

	//Get the name of the file
	newSlot = VAL_SLOT_START + bEntry;
	CreateSavedGameFileNameFromNumber( newSlot, zSavedGameName );

	if( FileExists( zSavedGameName ) )
	{
		// create the save game file
		hFile = FileOpen( zSavedGameName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
		if( !hFile )
		{
			FileClose( hFile );
			gbSaveGameArray[ VAL_SLOT_START + bEntry ] = FALSE;
			return(FALSE);
		}

		//Load the Save Game header file
		FileRead( hFile, pSaveGameHeader, sizeof( SAVED_GAME_HEADER ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( SAVED_GAME_HEADER ) )
		{
			FileClose( hFile );
			gbSaveGameArray[ VAL_SLOT_START + bEntry ] = FALSE;
			return(FALSE);
		}

		FileClose( hFile );

		//
		// Do some Tests on the header to make sure it is valid
		//

		//Check to see if the desc field is bigger then it should be, ie no null char
		if( wcslen( pSaveGameHeader->sSavedGameDesc ) >= SIZE_OF_SAVE_GAME_DESC )
		{
			memset( pSaveGameHeader, 0, sizeof( SAVED_GAME_HEADER ) );
			gbSaveGameArray[ VAL_SLOT_START + bEntry ] = FALSE;
			return(FALSE);
		}

		//Check to see if the version # field is bigger then it should be, ie no null char
		if( strlen( pSaveGameHeader->zGameVersionNumber ) >= GAME_VERSION_LENGTH )
		{
			memset( pSaveGameHeader, 0, sizeof( SAVED_GAME_HEADER ) );
			gbSaveGameArray[ bEntry ] = FALSE;
			return(FALSE);
		}
	}
	else
	{
		memset( pSaveGameHeader, 0, sizeof( SAVED_GAME_HEADER ) );
#ifdef JA2BETAVERSION
		wcscpy( pSaveGameHeader->sSavedGameDesc, L"ERROR loading saved game header, file doesn't exist!!" );
#endif
	}

	return( TRUE );
}


void BtnSlgCancelCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		//Exit back
		if( gfCameDirectlyFromGame )
			SetSaveLoadExitScreen( guiPreviousOptionScreen );

		else if( guiPreviousOptionScreen == MAINMENU_SCREEN )
			SetSaveLoadExitScreen( MAINMENU_SCREEN );

		else if( guiPreviousOptionScreen == GAME_INIT_OPTIONS_SCREEN )//hayden
			SetSaveLoadExitScreen( GAME_INIT_OPTIONS_SCREEN );

		else
			SetSaveLoadExitScreen( OPTIONS_SCREEN );


		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnSlgSaveLoadCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		SaveLoadGameNumber( gbSelectedSaveLocation );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnNewPrevButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		PAGE_SLOT = PAGE_SLOT - 1;
		
		if ( PAGE_SLOT == 0 || PAGE_SLOT < 0 )
			VAL_SLOT_START = 0; 
		else if ( PAGE_SLOT >= 1 )
			VAL_SLOT_START = (PAGE_SLOT * 19);
			
		if (PAGE_SLOT == 0 || PAGE_SLOT < 0 )
		{
			DisableButton( guiPrevButton );
			EnableButton( guiNextButton );
			PAGE_SLOT = 0;
		}
		else if (PAGE_SLOT < MAX_PAGE_SLOT)
		{
			EnableButton( guiNextButton );
		}
		
		DestroySaveLoadTextInputBoxes();

		NewEnterSaveLoadScreen();

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnNewNextButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		
		PAGE_SLOT = PAGE_SLOT + 1;
		
		if ( PAGE_SLOT >= 1 )
			VAL_SLOT_START = (PAGE_SLOT * 19);
		else if ( PAGE_SLOT >= MAX_PAGE_SLOT )
		{
			VAL_SLOT_START = (MAX_PAGE_SLOT * 19);
			PAGE_SLOT = MAX_PAGE_SLOT;
		}

		if (PAGE_SLOT > 0 )
			EnableButton( guiPrevButton );

		if (PAGE_SLOT > MAX_PAGE_SLOT || PAGE_SLOT == MAX_PAGE_SLOT )
			DisableButton( guiNextButton );
		
		DestroySaveLoadTextInputBoxes();

		NewEnterSaveLoadScreen();
				
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void SelectedSaveRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	INT8		bActiveTextField;
	CHAR16	zString[128]; 

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT8	bSelected = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );
		static UINT32	uiLastTime = 0;
		UINT32	uiCurTime = GetJA2Clock();
		INT32	i;

		//If we are saving and this is the quick save slot
		if( gfSaveGame && bSelected == 0 && PAGE_SLOT == 0 )
		{
			//Display a pop up telling user what the quick save slot is
			DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, pMessageStrings[ MSG_QUICK_SAVE_RESERVED_FOR_TACTICAL ], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, RedrawSaveLoadScreenAfterMessageBox );
			return;
		}
		
		//If we are saving and this is the auto save slot
		if( gfSaveGame && bSelected == SAVE__TIMED_AUTOSAVE_SLOT1 && PAGE_SLOT == 0 )
		{
			swprintf( zString, L"%s", pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT_INFO ]);
			//Display a pop up telling user what the quick save slot is
			DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zString, SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, RedrawSaveLoadScreenAfterMessageBox );
			return;
		}
		
		//If we are saving and this is the auto save slot
		if( gfSaveGame && bSelected == SAVE__TIMED_AUTOSAVE_SLOT2 && PAGE_SLOT == 0 )
		{
			swprintf( zString, L"%s", pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT_INFO ]);
			//Display a pop up telling user what the quick save slot is
			DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zString, SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, RedrawSaveLoadScreenAfterMessageBox );			
			return;
		}
		
		//If we are saving and this is the auto save slot
		if( gfSaveGame && bSelected == SAVE__TIMED_AUTOSAVE_SLOT3 && PAGE_SLOT == 0 )
		{
			swprintf( zString, L"%s", pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT_INFO ]);
			//Display a pop up telling user what the quick save slot is
			DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zString, SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, RedrawSaveLoadScreenAfterMessageBox );			
			return;
		}
		
		//If we are saving and this is the auto save slot
		if( gfSaveGame && bSelected == SAVE__TIMED_AUTOSAVE_SLOT4 && PAGE_SLOT == 0 )
		{
			swprintf( zString, L"%s", pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT_INFO ]);
			//Display a pop up telling user what the quick save slot is
			DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zString, SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, RedrawSaveLoadScreenAfterMessageBox );			
			return;
		}
		
		//If we are saving and this is the auto save slot
		if( gfSaveGame && bSelected == SAVE__TIMED_AUTOSAVE_SLOT5 && PAGE_SLOT == 0 )
		{
			swprintf( zString, L"%s %d", pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT_INFO ],SAVE__TIMED_AUTOSAVE_SLOT5);
			//Display a pop up telling user what the quick save slot is
			DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zString, SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, RedrawSaveLoadScreenAfterMessageBox );			
			return;
		}
		
		//If we are saving and this is the auto save slot
		if( gfSaveGame && bSelected == SAVE__END_TURN_NUM_1 && PAGE_SLOT == 0 )
		{
			swprintf( zString, L"%s", pMessageStrings[ MSG_SAVE_AUTOSAVE_ENDTURN_TEXT_INFO ]);
			//Display a pop up telling user what the quick save slot is
			DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zString, SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, RedrawSaveLoadScreenAfterMessageBox );			
			return;
		}
		
		if( gfSaveGame && bSelected == SAVE__END_TURN_NUM_2 && PAGE_SLOT == 0 )
		{
			swprintf( zString, L"%s", pMessageStrings[ MSG_SAVE_AUTOSAVE_ENDTURN_TEXT_INFO ]);
			//Display a pop up telling user what the quick save slot is
			DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zString, SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, RedrawSaveLoadScreenAfterMessageBox );			
			return;
		}

		//Reset the regions
		for( i=0; i<NUM_SLOT; i++)
			gbSaveGameSelectedLocation[i] = SLG_UNSELECTED_SLOT_GRAPHICS_NUMBER;

		//if the user is selecting an unselected saved game slot
		if( gbSelectedSaveLocation != bSelected )
		{
			//Destroy the previous region
			DestroySaveLoadTextInputBoxes();

			gbSelectedSaveLocation = bSelected;

			//Reset the global string
			gzGameDescTextField[0] = '\0';

			//Init the text field for the game desc
			InitSaveLoadScreenTextInputBoxes();

			EnableButton( guiSlgSaveLoadBtn );

			uiLastTime = GetJA2Clock();

			//Set the selected region to be highlighted
			gbSaveGameSelectedLocation[ bSelected ] = SLG_SELECTED_SLOT_GRAPHICS_NUMBER;

			gfRedrawSaveLoadScreen = TRUE;

			uiLastTime = GetJA2Clock();
		}
		//the user is selecting the selected save game slot
		else
		{
			//if we are saving a game
			if( gfSaveGame )
			{
				//if the user is not currently editing the game desc
				if( !gfUserInTextInputMode )
				{
					if( ( uiCurTime - uiLastTime ) < SLG_DOUBLE_CLICK_DELAY )
					{
						//Load the saved game
						SaveLoadGameNumber( gbSelectedSaveLocation );
					}
					else
					{
						uiLastTime = GetJA2Clock();
					}

					InitSaveLoadScreenTextInputBoxes();

					//Set the selected region to be highlighted
					gbSaveGameSelectedLocation[ bSelected ] = SLG_SELECTED_SLOT_GRAPHICS_NUMBER;

					gfRedrawSaveLoadScreen = TRUE;
				}
				else
				{
					bActiveTextField = (INT32)GetActiveFieldID();
					if( bActiveTextField && bActiveTextField != -1 )
					{
						Get16BitStringFromField( (UINT8)bActiveTextField, gzGameDescTextField, 128 );
						SetActiveField(0);

						DestroySaveLoadTextInputBoxes();

						DisplaySaveGameEntry( gbLastHighLightedLocation );

						gfRedrawSaveLoadScreen = TRUE;

						if( ( uiCurTime - uiLastTime ) < SLG_DOUBLE_CLICK_DELAY )
						{
							gubSaveGameNextPass = 1;
						}
						else
						{
							uiLastTime = GetJA2Clock();
						}
					}
				}
				//Set the selected region to be highlighted
				gbSaveGameSelectedLocation[ bSelected ] = SLG_SELECTED_SLOT_GRAPHICS_NUMBER;
			}
			//else we are loading
			else
			{
				if( ( uiCurTime - uiLastTime ) < SLG_DOUBLE_CLICK_DELAY )
				{
					//Load the saved game
					SaveLoadGameNumber( bSelected );
				}
				else
				{
					uiLastTime = GetJA2Clock();
				}
			}
		}
		//Set the selected region to be highlighted
		gbSaveGameSelectedLocation[ bSelected ] = SLG_SELECTED_SLOT_GRAPHICS_NUMBER;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		DisableSelectedSlot();
	}
}

void SelectedSaveRegionMovementCallBack(MOUSE_REGION * pRegion, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		UINT8	bTemp;
		pRegion->uiFlags &= (~BUTTON_CLICKED_ON );

		bTemp = gbHighLightedLocation;
		gbHighLightedLocation = -1;
		DisplaySaveGameEntry( bTemp );

		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
	else if( reason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		pRegion->uiFlags |= BUTTON_CLICKED_ON ;

		//If we are saving and this is the quick save slot, leave
		if( gfSaveGame && MSYS_GetRegionUserData( pRegion, 0 ) != 0 )
		{
			return;
		}
		//Auto save slot 1-5
		else if( gfSaveGame && MSYS_GetRegionUserData( pRegion, 0 ) != 1 )
		{
			return;
		}
		else if( gfSaveGame && MSYS_GetRegionUserData( pRegion, 0 ) != 2 )
		{
			return;
		}
		else if( gfSaveGame && MSYS_GetRegionUserData( pRegion, 0 ) != 3 )
		{
			return;
		}
		else if( gfSaveGame && MSYS_GetRegionUserData( pRegion, 0 ) != 4 )
		{
			return;
		}
		else if( gfSaveGame && MSYS_GetRegionUserData( pRegion, 0 ) != 5 )
		{
			return;
		}
		//This slot is reserved for end turn Auto Saves
		else if( gfSaveGame && MSYS_GetRegionUserData( pRegion, 0 ) != 6 )
		{
			return;
		}
		else if( gfSaveGame && MSYS_GetRegionUserData( pRegion, 0 ) != 7 )
		{
			return;
		}
		
		gbLastHighLightedLocation = gbHighLightedLocation;
		gbHighLightedLocation = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );

		DisplaySaveGameEntry( gbLastHighLightedLocation );
		DisplaySaveGameEntry( gbHighLightedLocation );//, usPosY );

		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
}

void InitSaveLoadScreenTextInputBoxes()
{
	UINT16	usPosY;
	SAVED_GAME_HEADER SaveGameHeader;

	if( gbSelectedSaveLocation == -1 )
		return;

	if( !gfSaveGame )
		return;

	//if we are exiting, dont create the fields
	if( gfSaveLoadScreenExit || guiSaveLoadExitScreen != SAVE_LOAD_SCREEN )
		return;

	InitTextInputMode();

	SetTextInputCursor( CUROSR_IBEAM_WHITE );
	SetTextInputFont( (UINT16) FONT12ARIALFIXEDWIDTH );
	Set16BPPTextFieldColor( Get16BPPColor(FROMRGB( 0, 0, 0) ) );
	SetBevelColors( Get16BPPColor(FROMRGB(136, 138, 135)), Get16BPPColor(FROMRGB(24, 61, 81)) );
	SetTextInputRegularColors( FONT_WHITE, 2 );
	SetTextInputHilitedColors( 2, FONT_WHITE, FONT_WHITE	);
	SetCursorColor( Get16BPPColor(FROMRGB(255, 255, 255) ) );

	AddUserInputField( NULL );

	usPosY = SLG_FIRST_SAVED_SPOT_Y + SLG_GAP_BETWEEN_LOCATIONS * gbSelectedSaveLocation;

	//if there is already a string here, use its string
	if( gbSaveGameArray[ VAL_SLOT_START + gbSelectedSaveLocation ] )
	{
		//if we are modifying a previously modifed string, use it
		if( gzGameDescTextField[0] != '\0' )
		{
		}
		else
		{
			//Get the header for the specified saved game
			LoadSavedGameHeader( gbSelectedSaveLocation, &SaveGameHeader );
			wcscpy( gzGameDescTextField, SaveGameHeader.sSavedGameDesc );
		}
	}
	else
		gzGameDescTextField[0] = '\0';

	//Game Desc Field
	AddTextInputField( SLG_FIRST_SAVED_SPOT_X+SLG_SAVE_GAME_DESC_X, (INT16)(usPosY+SLG_SAVE_GAME_DESC_Y-5), SLG_SAVELOCATION_WIDTH-SLG_SAVE_GAME_DESC_X-7, 17, MSYS_PRIORITY_HIGH+2, gzGameDescTextField, 46, INPUTTYPE_ASCII );//23

	SetActiveField( 1 );

	gfUserInTextInputMode = TRUE;
}

void DestroySaveLoadTextInputBoxes()
{
	gfUserInTextInputMode = FALSE;
	KillAllTextInputModes();
	SetTextInputCursor( CURSOR_IBEAM );
}

void SetSelection( UINT8 ubNewSelection )
{
	//if we are loading and there is no entry, return
 	if( !gfSaveGame )
	{
		if( !gbSaveGameArray[VAL_SLOT_START + ubNewSelection] )
			return;
	}

	//Reset the selected slot background graphic
	if( gbSelectedSaveLocation != -1 )
	{
		gbSaveGameSelectedLocation[ gbSelectedSaveLocation ] = SLG_UNSELECTED_SLOT_GRAPHICS_NUMBER;

		//reset the slots help text
		SetRegionFastHelpText( &gSelectedSaveRegion[ gbSelectedSaveLocation ], L"\0" );
	}

	gfRedrawSaveLoadScreen = TRUE;
	DestroySaveLoadTextInputBoxes();

	//if we are loading,
	if( !gfSaveGame )
	{
		//Enable the save/load button
		EnableButton( guiSlgSaveLoadBtn );
	}

	//if we are to save
	if( gfSaveGame )
	{
		if( gbSelectedSaveLocation != ubNewSelection )
		{
			//Destroy the previous region
			DestroySaveLoadTextInputBoxes();

			//reset selected slot
			gbSelectedSaveLocation = ubNewSelection;

			//Null out the currently selected save game
			gzGameDescTextField[0] = '\0';

			//Init the text field for the game desc
			InitSaveLoadScreenTextInputBoxes();
		}

		//Enable the save/load button
		EnableButton( guiSlgSaveLoadBtn );
	}

	//reset selected slot
	gbSelectedSaveLocation = ubNewSelection;

	//Set the selected slot background graphic
	gbSaveGameSelectedLocation[ gbSelectedSaveLocation ] = SLG_SELECTED_SLOT_GRAPHICS_NUMBER;
}

UINT8 CompareSaveGameVersion( INT32 bSaveGameID )
{
	UINT8 ubRetVal=SLS_HEADER_OK;

	SAVED_GAME_HEADER SaveGameHeader;

	//Get the heade for the saved game
	LoadSavedGameHeader( bSaveGameID, &SaveGameHeader );

	// check to see if the saved game version in the header is the same as the current version
	if( SaveGameHeader.uiSavedGameVersion != SAVE_GAME_VERSION )
	{
		ubRetVal = SLS_SAVED_GAME_VERSION_OUT_OF_DATE;
	}

	if( strcmp( SaveGameHeader.zGameVersionNumber, czVersionNumber ) != 0 )
	{
		if( ubRetVal == SLS_SAVED_GAME_VERSION_OUT_OF_DATE )
			ubRetVal = SLS_BOTH_SAVE_GAME_AND_GAME_VERSION_OUT_OF_DATE;
		else
			ubRetVal = SLS_GAME_VERSION_OUT_OF_DATE;
	}

	return( ubRetVal );
}

void LoadSavedGameWarningMessageBoxCallBack( UINT8 bExitValue )
{
	// yes, load the game
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		//Setup up the fade routines
		StartFadeOutForSaveLoadScreen();
	}

	//The user does NOT want to continue..
	else
	{
		//ask if the user wants to delete all the saved game files
		DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_DELETE_ALL_SAVE_GAMES], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_YESNO, LoadSavedGameDeleteAllSaveGameMessageBoxCallBack );
	}
}

void LoadSavedGameDeleteAllSaveGameMessageBoxCallBack( UINT8 bExitValue )
{
	// yes, Delete all the save game files
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		DeleteAllSaveGameFile( );
		gfSaveLoadScreenExit = TRUE;
	}

	gfExitAfterMessageBox = TRUE;

	SetSaveLoadExitScreen( OPTIONS_SCREEN );

	gbSelectedSaveLocation=-1;
}

void DeleteAllSaveGameFile( )
{
	UINT8	cnt,cnt2;

	for( cnt=0; cnt<NUM_SLOT; cnt++)
	{
		cnt2 = VAL_SLOT_START + cnt;
		DeleteSaveGameNumber( cnt2 );
	}

	gGameSettings.bLastSavedGameSlot = -1;

	InitSaveGameArray();
}

void DeleteSaveGameNumber( UINT8 ubSaveGameSlotID )
{
	CHAR8		zSaveGameName[ 512 ];
	UINT8 newSlot;

	//Create the name of the file
	newSlot = VAL_SLOT_START + ubSaveGameSlotID;
	CreateSavedGameFileNameFromNumber( newSlot, zSaveGameName );

	//Delete the saved game file
	FileDelete( zSaveGameName );
}

void DisplayOnScreenNumber( BOOLEAN fErase )
{
	CHAR16		zTempString[16];
	UINT16		usPosX = 6;
	UINT16		usPosY;
	UINT8			bLoopNum;
	UINT8			bNum=0;

	usPosY = SLG_FIRST_SAVED_SPOT_Y;

	for( bLoopNum=0; bLoopNum<NUM_SLOT; bLoopNum++)
	{
		//Dont diplay it for the quicksave
		if( bLoopNum == 0 )
		{
			usPosY += SLG_GAP_BETWEEN_LOCATIONS;
			continue;
		}

		BlitBufferToBuffer(guiSAVEBUFFER, guiRENDERBUFFER, usPosX, (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 10, 10 );

		if( bLoopNum != 10 )
		{
			bNum = bLoopNum;
			swprintf( zTempString, L"%2d", bNum );
		}
		else
		{
			bNum = 0;
			swprintf( zTempString, L"%2d", bNum );
		}

		if( !fErase )
			DrawTextToScreen( zTempString, usPosX, (UINT16)(usPosY+SLG_DATE_OFFSET_Y), 0, SAVE_LOAD_NUMBER_FONT, SAVE_LOAD_NUMBER_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);

		InvalidateRegion( usPosX, usPosY+SLG_DATE_OFFSET_Y, usPosX+10, usPosY+SLG_DATE_OFFSET_Y+10 );

		usPosY += SLG_GAP_BETWEEN_LOCATIONS;
	}
}

#ifdef JA2BETAVERSION
	extern BOOLEAN ValidateSoldierInitLinks( UINT8 ubCode );
#endif

void DoneFadeOutForSaveLoadScreen( void )
{
	if( !LoadSavedGame( VAL_SLOT_START + gbSelectedSaveLocation ) )
	{
		if( guiBrokenSaveGameVersion < 95 && !gfSchedulesHosed )
		{ //Hack problem with schedules getting misaligned.
			gfSchedulesHosed = TRUE;
			if( !LoadSavedGame( VAL_SLOT_START + gbSelectedSaveLocation ) )
			{
				DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_LOAD_GAME_ERROR], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, FailedLoadingGameCallBack );
				NextLoopCheckForEnoughFreeHardDriveSpace();
			}
			else
			{
				gfSchedulesHosed = FALSE;
				goto SUCCESSFULLY_CORRECTED_SAVE;
			}
			gfSchedulesHosed = FALSE;
		}
		else
		{
			//CHRISL: New fail message if we failed because of screen res
			if(UsingNewInventorySystem() == true && iResolution >= _640x480 && iResolution < _800x600)
			{
				DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_INV_RES_ERROR], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, FailedLoadingGameCallBack );
				NextLoopCheckForEnoughFreeHardDriveSpace();
			}
			else if(UsingNewInventorySystem() == true && IsNIVModeValid(false) == FALSE)
			{
				DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_INV_CUSTUM_ERROR], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, FailedLoadingGameCallBack );
				NextLoopCheckForEnoughFreeHardDriveSpace();
			}
			else if ((gGameOptions.ubSquadSize == 8 && iResolution < _800x600) || 
				(gGameOptions.ubSquadSize == 10 && iResolution < _1024x768))
			{
				DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_SQUAD_SIZE_RES_ERROR], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, FailedLoadingGameCallBack );
				NextLoopCheckForEnoughFreeHardDriveSpace();
			}
			else
			{
				DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_LOAD_GAME_ERROR], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, FailedLoadingGameCallBack );
				NextLoopCheckForEnoughFreeHardDriveSpace();
			}
		}
	}
	else
	{
		SUCCESSFULLY_CORRECTED_SAVE:
		#ifdef JA2BETAVERSION
		ValidateSoldierInitLinks( 1 );
		{
		#endif
			//If we are to go to map screen after loading the game
			if( guiScreenToGotoAfterLoadingSavedGame == MAP_SCREEN )
			{
				gFadeInDoneCallback = DoneFadeInForSaveLoadScreen;

				SetSaveLoadExitScreen( guiScreenToGotoAfterLoadingSavedGame );
	
				FadeInNextFrame( );
			}

			else
			{
				//if we are to go to the Tactical screen after loading
				gFadeInDoneCallback = DoneFadeInForSaveLoadScreen;

				SetSaveLoadExitScreen( guiScreenToGotoAfterLoadingSavedGame );

				PauseTime( FALSE );
				FadeInGameScreen( );
			}
#ifdef JA2UB			
			//JA25 UB
			// ATE: Validate any new maps...
			// OK, if we're a camapign, check for new maps
			//if ( !InDefaultCampaign( ) )
			//{
			MakeBadSectorListFromMapsOnHardDrive( TRUE );
			LetLuaMakeBadSectorListFromMapsOnHardDrive( 0 );
			//}
#endif
			
		#ifdef JA2BETAVERSION
		}
		#endif
	}
	gfStartedFadingOut = FALSE;
}

void DoneFadeInForSaveLoadScreen( void )
{
	//Leave the screen
	//if we are supposed to stay in tactical, due nothing,
	//if we are supposed to goto mapscreen, leave tactical and go to mapscreen

	if( guiScreenToGotoAfterLoadingSavedGame == MAP_SCREEN )
	{
		if( !gfPauseDueToPlayerGamePause )
		{
			UnLockPauseState( );
			UnPauseGame( );
		}
	}
	else
	{
		//if the game is currently paused
		if( GamePaused() )
		{
			//need to call it twice
			HandlePlayerPauseUnPauseOfGame();
			HandlePlayerPauseUnPauseOfGame();
		}
	}
}

void SelectedSLSEntireRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		DisableSelectedSlot();
	}
}

void DisableSelectedSlot()
{
	//reset selected slot
	gbSelectedSaveLocation = -1;
	gfRedrawSaveLoadScreen = TRUE;
	DestroySaveLoadTextInputBoxes();

	if( !gfSaveGame )
		DisableButton( guiSlgSaveLoadBtn );

	//reset the selected graphic
	ClearSelectedSaveSlot();
}


void ConfirmSavedGameMessageBoxCallBack( UINT8 bExitValue )
{
	Assert( gbSelectedSaveLocation != -1 );

	// yes, load the game
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		SaveGameToSlotNum();
	}
}

void ConfirmLoadSavedGameMessageBoxCallBack( UINT8 bExitValue )
{
	Assert( gbSelectedSaveLocation != -1 );

	// yes, load the game
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		//Setup up the fade routines
		StartFadeOutForSaveLoadScreen();
	}
	else
	{
		gfExitAfterMessageBox = FALSE;
	}
}

#ifdef JA2BETAVERSION
void ErrorDetectedInSaveCallback( UINT8 bValue )
{
	//If we are to go to map screen after loading the game
	if( guiScreenToGotoAfterLoadingSavedGame == MAP_SCREEN )
	{
		gFadeInDoneCallback = DoneFadeInForSaveLoadScreen;
		SetSaveLoadExitScreen( guiScreenToGotoAfterLoadingSavedGame );
		FadeInNextFrame( );
	}
	else
	{
		//if we are to go to the Tactical screen after loading
		gFadeInDoneCallback = DoneFadeInForSaveLoadScreen;
		SetSaveLoadExitScreen( guiScreenToGotoAfterLoadingSavedGame );
		FadeInGameScreen( );
	}
	gfStartedFadingOut = TRUE;
}
#endif

void FailedLoadingGameCallBack( UINT8 bExitValue )
{
	// yes
	if( bExitValue == MSG_BOX_RETURN_OK )
	{
		//if the current screen is tactical
		if( guiPreviousOptionScreen == MAP_SCREEN )
		{
			SetPendingNewScreen( MAINMENU_SCREEN );
		}
		else
		{
			LeaveTacticalScreen( MAINMENU_SCREEN );
		}

		SetSaveLoadExitScreen( MAINMENU_SCREEN );


		//We want to reinitialize the game
		ReStartingGame();
	}
}

BOOLEAN DoAutoSave( int ubSaveGameID, STR16 pGameDesc )
{
	if( !SaveGame( ubSaveGameID, pGameDesc ) )
	{
		//Unset the fact that we are saving a game
		gTacticalStatus.uiFlags &= ~LOADING_SAVED_GAME;

		if( guiPreviousOptionScreen == MAP_SCREEN )
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_SAVE_GAME_ERROR], MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
		else
			DoMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_SAVE_GAME_ERROR], GAME_SCREEN, MSG_BOX_FLAG_OK, NULL, NULL );
	}

	return( TRUE );
}

BOOLEAN DoQuickSave()
{
	gzGameDescTextField[0] = '\0';

	if( !SaveGame( 0, gzGameDescTextField ) )
	{
		//Unset the fact that we are saving a game
		gTacticalStatus.uiFlags &= ~LOADING_SAVED_GAME;

		if( guiPreviousOptionScreen == MAP_SCREEN )
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_SAVE_GAME_ERROR], MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
		else
			DoMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_SAVE_GAME_ERROR], GAME_SCREEN, MSG_BOX_FLAG_OK, NULL, NULL );
	}

	return( TRUE );
}

BOOLEAN DoQuickLoad()
{
	//Build the save game array
	InitSaveGameArray();

	//if there is no save in the quick save slot
	if( !gbSaveGameArray[ 0 ] )
		return( FALSE );

	//Set the selection to be the quick save slot
	gbSelectedSaveLocation = 0;

	//if the game is paused, and we are in tactical, unpause
	if( guiCurrentScreen == GAME_SCREEN )
	{
		PauseTime( FALSE );
	}

	//Do a fade out before we load the game
	gFadeOutDoneCallback = DoneFadeOutForSaveLoadScreen;

	FadeOutNextFrame( );
	gfStartedFadingOut = TRUE;
	gfDoingQuickLoad = TRUE;

	return( TRUE );
}


BOOLEAN IsThereAnySavedGameFiles()
{
	UINT8	cnt,newSlot;
	CHAR8		zSaveGameName[ 512 ];

	for( cnt=0; cnt<NUM_SLOT; cnt++)
	{
		newSlot = VAL_SLOT_START + cnt;
		CreateSavedGameFileNameFromNumber( newSlot, zSaveGameName );

		if( FileExists( zSaveGameName ) )
			return( TRUE );
	}

	return( FALSE );
}

void NotEnoughHardDriveSpaceForQuickSaveMessageBoxCallBack( UINT8 bExitValue )
{
	if( !SaveGame( 0, gzGameDescTextField ) )
	{
		//Unset the fact that we are saving a game
		gTacticalStatus.uiFlags &= ~LOADING_SAVED_GAME;

		//Set a flag indicating that the save failed ( cant initiate a message box from within a mb callback )
		gfFailedToSaveGameWhenInsideAMessageBox = TRUE;
	}
}

void NotEnoughHardDriveSpaceForNormalSaveMessageBoxCallBack( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_OK )
	{
		//If the game failed to save
		if( !SaveGame( gbSelectedSaveLocation, gzGameDescTextField ) )
		{
			//Unset the fact that we are saving a game
			gTacticalStatus.uiFlags &= ~LOADING_SAVED_GAME;

			//Set a flag indicating that the save failed ( cant initiate a message box from within a mb callback )
			gfFailedToSaveGameWhenInsideAMessageBox = TRUE;
		}
		else
		{
			SetSaveLoadExitScreen( guiPreviousOptionScreen );
		}
	}
}

void RedrawSaveLoadScreenAfterMessageBox( UINT8 bExitValue )
{
	gfRedrawSaveLoadScreen = TRUE;
}

void MoveSelectionUpOrDown( BOOLEAN fUp )
{
	INT32	i;
	UINT8 pSlot;
	
	if ( PAGE_SLOT == 0 ) 
		pSlot = 1;
	else 
		pSlot = 0;
	
	//if we are saving, any slot otgher then the quick save slot is valid
	if( gfSaveGame )
	{
		if( fUp )
		{
			//if there is no selected slot, get out
			if( gbSelectedSaveLocation == -1 )
				return;

			//if the selected slot is above the first slot
			if( gbSelectedSaveLocation > pSlot )
			{
				SetSelection( (UINT8)(gbSelectedSaveLocation-1) );
			}
		}
		else
		{
			//if the selected slot is invalid
			if( gbSelectedSaveLocation == -1 )
			{
				SetSelection( pSlot );
			}
			else
			{
				if( gbSelectedSaveLocation >= pSlot && gbSelectedSaveLocation < NUM_SLOT-1 )
				{
					SetSelection( (UINT8)(gbSelectedSaveLocation + 1) );
				}
			}
		}
	}
	else
	{
		if( fUp )
		{
			for( i=gbSelectedSaveLocation-1; i>=0; i--)
			{
				if( gbSaveGameArray[VAL_SLOT_START + i] )
				{
					ClearSelectedSaveSlot();

					SetSelection( (UINT8)i );
					break;
				}
			}
		}
		else
		{
			//if there is no selected slot, move the selected slot to the first slot
			if( gbSelectedSaveLocation == -1 )
			{
				ClearSelectedSaveSlot();

				SetSelection( 0 );
			}
			else
			{
				for( i=gbSelectedSaveLocation+1; i<NUM_SLOT; i++)
				{
					if( gbSaveGameArray[VAL_SLOT_START + i] )
					{
						ClearSelectedSaveSlot();

						SetSelection( (UINT8)i );
						break;
					}
				}
			}
		}
	}
}

void ClearSelectedSaveSlot()
{
	INT32	i;
	for( i=0; i<NUM_SLOT; i++)
		gbSaveGameSelectedLocation[i] = SLG_UNSELECTED_SLOT_GRAPHICS_NUMBER;

	gbSelectedSaveLocation = -1;
}


void SaveGameToSlotNum()
{
	UINT8 newSlot;

	//Redraw the save load screen
	RenderSaveLoadScreen();

	//render the buttons
	MarkButtonsDirty( );
	RenderButtons();

	newSlot = VAL_SLOT_START + gbSelectedSaveLocation;
	
	if( !SaveGame( newSlot, gzGameDescTextField ) )
	{
		//Unset the fact that we are saving a game
		gTacticalStatus.uiFlags &= ~LOADING_SAVED_GAME;

		DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_SAVE_GAME_ERROR], SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, NULL);
	}

	// gfExitAfterMessageBox = TRUE;
	SetSaveLoadExitScreen( guiPreviousOptionScreen );
}

void StartFadeOutForSaveLoadScreen()
{
	//if the game is paused, and we are in tactical, unpause
	if( guiPreviousOptionScreen == GAME_SCREEN )
	{
		PauseTime( FALSE );
	}

	gFadeOutDoneCallback = DoneFadeOutForSaveLoadScreen;

	FadeOutNextFrame( );
	gfStartedFadingOut = TRUE;
	gfExitAfterMessageBox = TRUE;
}

BOOLEAN SaveDataSaveToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;

	FileWrite( hFile, &AutoSaveToSlot, sizeof( AutoSaveToSlot), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( AutoSaveToSlot ) )
	{
		return( FALSE );
	}
	
	return( TRUE );
}

BOOLEAN LoadDataSaveFromLoadGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;

	//Briefing room
	FileRead( hFile, &AutoSaveToSlot, sizeof( AutoSaveToSlot), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( AutoSaveToSlot ) )
	{
		return( FALSE );
	}

	// Set the slot, which will be the next auto save slot (current slot + 1)
	if (AutoSaveToSlot[0] == TRUE)
	{
		AutoSaveToSlot[0] = FALSE;
		AutoSaveToSlot[1] = TRUE;
	}
	else if (AutoSaveToSlot[1] == TRUE)
	{
		AutoSaveToSlot[1] = FALSE;
		AutoSaveToSlot[2] = TRUE;
	}
	else if (AutoSaveToSlot[2] == TRUE)
	{
		AutoSaveToSlot[2] = FALSE;
		AutoSaveToSlot[3] = TRUE;
	}
	else if (AutoSaveToSlot[3] == TRUE)
	{
		AutoSaveToSlot[3] = FALSE;
		AutoSaveToSlot[4] = TRUE;
	}
	else if (AutoSaveToSlot[4] == TRUE)
	{
		AutoSaveToSlot[4] = FALSE;
		AutoSaveToSlot[0] = TRUE;
	}

	return( TRUE );
}
