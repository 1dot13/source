#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "Encrypted File.h"
#else
	#include "sgp.h"
	#include "screenids.h"
	#include "Timer Control.h"
	#include "sys globals.h"
	#include "fade screen.h"
	#include "sysutil.h"
	#include "vobject_blitters.h"
	#include "MercTextBox.h"
	#include "wcheck.h"
	#include "cursors.h"
	#include "font control.h"
	#include "mainmenuscreen.h"
	#include "cursor control.h"
	#include "render dirty.h"
	#include "music control.h"
	#include "GameSettings.h"
	#include "GameScreen.h"
	#include "SaveLoadScreen.h"
	#include "SaveLoadGame.h"
	#include "Options Screen.h"
	#include	"English.h"
	#include	"Gameloop.h"
	#include	"Game Init.h"
	#include	"Utilities.h"
	#include	"WordWrap.h"
	#include "Font Control.h"
	#include "text.h"
	#include "Multi Language Graphic Utils.h"
	#include "Encrypted File.h"
	#include "ja2 splash.h"
	#include "GameVersion.h"
#endif

#include "mercs.h"
#include "gamesettings.h"
#include "connect.h"
#include "strategic.h"
#include "strategic movement.h"
#include "overhead.h"
#include "init.h"
#include "xml.h"
#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_profile.h>

#ifdef JA2UB
#include "ub_config.h"
#endif

#define	MAINMENU_TEXT_FILE						"LoadScreens\\MainMenu.edt"
#define MAINMENU_RECORD_SIZE					80 * 2

//externals
extern HVSURFACE ghFrameBuffer;
extern	HVSURFACE		ghBackBuffer;
// MENU ITEMS
enum
{
	NEW_GAME,
	NEW_MP_GAME,
	LOAD_GAME,
	PREFERENCES,
	CREDITS,
	QUIT,
	NUM_MENU_ITEMS
};

UINT32 MAINMENU_Y;
UINT32 MAINMENU_X;
UINT32 MAINMENU_Y_SPACE;

INT32						iMenuImages[ NUM_MENU_ITEMS ];
INT32						iMenuButtons[ NUM_MENU_ITEMS ];

UINT16						gusMainMenuButtonWidths[ NUM_MENU_ITEMS ];

MOUSE_REGION				gBackRegion;
INT8						gbHandledMainMenu = 0;
BOOLEAN						fInitialRender = FALSE;

BOOLEAN						gfMainMenuScreenEntry = FALSE;
BOOLEAN						gfMainMenuScreenExit = FALSE;

UINT32						guiMainMenuExitScreen = MAINMENU_SCREEN;

extern	BOOLEAN		gfLoadGameUponEntry;

void ExitMainMenu( );
void MenuButtonCallback(GUI_BUTTON *btn, INT32 reason);
void HandleMainMenuInput();
void HandleMainMenuScreen();
void DisplayAssignmentText();
void ClearMainMenu();
void HandleHelpScreenInput();
void SelectMainMenuBackGroundRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void SetMainMenuExitScreen( UINT32 uiNewScreen );
void CreateDestroyBackGroundMouseMask( BOOLEAN fCreate );
BOOLEAN CreateDestroyMainMenuButtons( BOOLEAN fCreate );
void RenderMainMenu();
void RestoreButtonBackGrounds();

//Main Menu layout by Jazz
MAIN_MENU_VALUES gMainMenulayout[MAX_MAIN_MENU_IMAGE];
//VSURFACE_DESC		vs_desc;

extern void InitSightRange(); //lal

UINT32	MainMenuScreenInit( )
{
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Version Label: %S", zVersionLabel ));
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Version #:	 %s", czVersionNumber ));
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Tracking #:	%S", zTrackingNumber ));

	return( TRUE );
}

UINT32	MainMenuScreenHandle( )
{
	UINT32 cnt;
	UINT32 uiTime;

	if( guiSplashStartTime + 4000 > GetJA2Clock() )
	{
		SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
		SetMusicMode( MUSIC_NONE );
		return MAINMENU_SCREEN;	//The splash screen hasn't been up long enough yet.
	}
	if( guiSplashFrameFade )
	{ 
		uiTime = GetJA2Clock();
		if( guiSplashFrameFade > 2 )
			ShadowVideoSurfaceRectUsingLowPercentTable( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
		else if( guiSplashFrameFade > 1 )
			ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
		else
		{
			uiTime = GetJA2Clock();
			SetMusicMode( MUSIC_MAIN_MENU );
		}

		guiSplashFrameFade--;

		InvalidateScreen();
		EndFrameBufferRender();

		SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

		return MAINMENU_SCREEN;
	}

	SetCurrentCursorFromDatabase( CURSOR_NORMAL );

	if( gfMainMenuScreenEntry )
	{
		InitMainMenu( );
		gfMainMenuScreenEntry = FALSE;
		gfMainMenuScreenExit = FALSE;
		guiMainMenuExitScreen = MAINMENU_SCREEN;
		SetMusicMode( MUSIC_MAIN_MENU );
	}

	if ( fInitialRender )
	{
		ClearMainMenu();
		RenderMainMenu();

		fInitialRender = FALSE;
	}

	RestoreButtonBackGrounds();

	// Render buttons
	for ( cnt = 0; cnt < NUM_MENU_ITEMS; cnt++ )
	{
		MarkAButtonDirty( iMenuButtons[ cnt ] );
	}

	RenderButtons( );

	EndFrameBufferRender( );

	HandleMainMenuInput();
	HandleMainMenuScreen();

	if( gfMainMenuScreenExit )
	{
		ExitMainMenu( );
		gfMainMenuScreenExit = FALSE;
		gfMainMenuScreenEntry = TRUE;
	}

	if( guiMainMenuExitScreen != MAINMENU_SCREEN )
		gfMainMenuScreenEntry = TRUE;

	return( guiMainMenuExitScreen );
}

UINT32	MainMenuScreenShutdown(	)
{
	return( FALSE );
}

void HandleMainMenuScreen()
{
	if ( gbHandledMainMenu != 0 )
	{
		// Exit according to handled value!
		switch( gbHandledMainMenu )
		{
			case QUIT:
				gfMainMenuScreenExit = TRUE;
				gfProgramIsRunning = FALSE;
				break;

			case NEW_GAME:
				break;

			case NEW_MP_GAME:
				break;

			case LOAD_GAME:
				// Select the game which is to be restored
				guiPreviousOptionScreen = guiCurrentScreen;
				guiMainMenuExitScreen = SAVE_LOAD_SCREEN;
				gbHandledMainMenu = 0;
				gfSaveGame = FALSE;
				gfMainMenuScreenExit = TRUE;
				break;

			case PREFERENCES:
				guiPreviousOptionScreen = guiCurrentScreen;
				guiMainMenuExitScreen = OPTIONS_SCREEN;
				gbHandledMainMenu = 0;
				gfMainMenuScreenExit = TRUE;
				break;

			case CREDITS:
				guiMainMenuExitScreen = CREDIT_SCREEN;
				gbHandledMainMenu = 0;
				gfMainMenuScreenExit = TRUE;
				break;
		}
	}
}

BOOLEAN InitMainMenu( )
{
	//VOBJECT_DESC	VObjectDesc;	
	VSURFACE_DESC		vs_desc = {};

	//main Menu by JAzz
	UINT16 iCounter2; 

	if(is_networked)
	{	
		is_networked = FALSE;

#ifdef USE_VFS
		// remove Multiplayer profile if it exists
		vfs::CProfileStack *PS = getVFS()->getProfileStack();
		vfs::CVirtualProfile *pProf = PS->getProfile("_MULTIPLAYER");
		if( pProf && (pProf == PS->topProfile()) )
		{
			SGP_THROW_IFFALSE(PS->popProfile(), "Leaving Multiplayer mode : Could not remove \"_MULTIPLAYER\" profile");
		}
#endif

		// Snap: UN-Init MP save game directory
		if ( !InitSaveDir() )
		{
			//if something didnt work, dont even know how to make error code...//hayden
		}
	}

	//Check to see whatr saved game files exist
	InitSaveGameArray();

	//Create the background mouse mask
	CreateDestroyBackGroundMouseMask( TRUE );

	CreateDestroyMainMenuButtons( TRUE );
		
	// load background graphic and add it	
	//Main Menu by Jazz
	for( iCounter2 = 1; iCounter2 < MAX_ELEMENT; iCounter2++ )
	{
		//VObjectDesc.fCreateFlags = VSURFACE_CREATE_FROMFILE;		
		vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE | VSURFACE_CREATE_FROMPNG_FALLBACK;

		if (gMainMenulayout[iCounter2].Visible == 1)
		{
			strcpy(vs_desc.ImageFile, gMainMenulayout[iCounter2].FileName);
			
			if( !AddVideoSurface( &vs_desc, &gMainMenulayout[iCounter2].uiIndex ) )
			AssertMsg(0, String( "Missing %s", gMainMenulayout[iCounter2].FileName ) );	


			//if (iResolution >= _640x480 && iResolution < _800x600)
			//{				
			//	strcpy(VObjectDesc.ImageFile, gMainMenulayout[iCounter2].FileName);

			//	if( !AddVideoObject( &VObjectDesc, &gMainMenulayout[iCounter2].uiIndex ) )
			//		AssertMsg(0, String( "Missing %s", gMainMenulayout[iCounter2].FileName ) );				
			//}
			//else if (iResolution < _1024x768)
			//{		
			//	strcpy(VObjectDesc.ImageFile, gMainMenulayout[iCounter2].FileName800x600);

			//	if( !AddVideoObject( &VObjectDesc, &gMainMenulayout[iCounter2].uiIndex ) )
			//		AssertMsg(0, String( "Missing %s", gMainMenulayout[iCounter2].FileName800x600 ) );				
			//}
			//else
			//{		
			//	strcpy(VObjectDesc.ImageFile, gMainMenulayout[iCounter2].FileName1024x768);

			//	if( !AddVideoObject( &VObjectDesc, &gMainMenulayout[iCounter2].uiIndex ) )
			//		AssertMsg(0, String( "Missing %s", gMainMenulayout[iCounter2].FileName1024x768 ) );				
			//}
		}
	}
	
	//if there are no saved games, disable the button
	if( !IsThereAnySavedGameFiles() )
		DisableButton( iMenuButtons[ LOAD_GAME ] );
		
		
	#ifdef JA113DEMO
		DisableButton( iMenuButtons[ NEW_MP_GAME ] );
	#endif

	gbHandledMainMenu = 0;
	fInitialRender		= TRUE;

	SetPendingNewScreen( MAINMENU_SCREEN);
	guiMainMenuExitScreen = MAINMENU_SCREEN;

	DequeueAllKeyBoardEvents();

	return( TRUE );
}


void ExitMainMenu( )
{
	UINT32 iCounter2 ;

	CreateDestroyBackGroundMouseMask( FALSE );
	CreateDestroyMainMenuButtons( FALSE );

	for( iCounter2 = 1; iCounter2 < MAX_ELEMENT; iCounter2++ )
	{	
		if (gMainMenulayout[iCounter2].Visible == 1)
		{
//			DeleteVideoObjectFromIndex( gMainMenulayout[iCounter2].uiIndex );
			DeleteVideoSurfaceFromIndex( gMainMenulayout[iCounter2].uiIndex );
		}
	}
	





	gMsgBox.uiExitScreen = MAINMENU_SCREEN;
}

// WANNE - MP: This method initializes variables that should be initialized
// differently for single and multiplayer
void InitDependingGameStyleOptions()
{
	// Yes, also initialize these ones, because of SP and MP game!
	InitStrategicEngine();
	InitStrategicMovementCosts();
	InitializeFaceGearGraphics();
	
	// WANNE: Initialize again, because if differs from SP to MP game!
	LoadGameSettings();

	FreeGameExternalOptions();

	// Load APBPConstants.ini
	LoadGameAPBPConstants();
	// Load ja2_options.ini
	LoadGameExternalOptions();
	// Load new STOMP ini - SANDRO
	LoadSkillTraitsExternalSettings();
	// HEADROCK HAM 4: CTH constants
	LoadCTHConstants();

#ifdef JA2UB
	LoadGameUBOptions(); // JA25 UB
#endif

	InitSightRange(); //lal

	ReStartingGame();	
}


// WANNE - MP: In this method we decide if we want to play a single or multiplayer game
void MenuButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT8	bID;

	bID = (UINT8)btn->UserData[0];

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		// handle menu
		gbHandledMainMenu = bID;
		RenderMainMenu();

		if( gbHandledMainMenu == NEW_GAME )
		{
			giMAXIMUM_NUMBER_OF_PLAYER_SLOTS = CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS;
			if(is_networked)
			{
				is_networked = FALSE;
				// Snap: UN-Init MP save game directory
				if ( !InitSaveDir() )
				{
					//if something didnt work, dont even know how to make error code...//hayden
				}
			};

			SetMainMenuExitScreen( GAME_INIT_OPTIONS_SCREEN );
		}
		else if (gbHandledMainMenu == NEW_MP_GAME)
		{
			is_networked = TRUE;

			// WANNE - MP: Only reset this here, because otherwise after a MP game ends and a new starts, we would receive the files again.
			fClientReceivedAllFiles = FALSE;

			giMAXIMUM_NUMBER_OF_PLAYER_SLOTS = 7;

			// Snap: Re-Init MP save game directory
			if ( !InitSaveDir() )
			{

				//if something didnt work, dont even know how to make error code...//hayden
			}

			// Reload the external gameplay data, because maybe we started a MP game before!
			LoadExternalGameplayData(TABLEDATA_DIRECTORY);

			SetMainMenuExitScreen( MP_JOIN_SCREEN ); // OJW - 20081129
			//SetMainMenuExitScreen( GAME_INIT_OPTIONS_SCREEN );
		}
		else if( gbHandledMainMenu == LOAD_GAME )
		{
			giMAXIMUM_NUMBER_OF_PLAYER_SLOTS = CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS;
			if(is_networked)
			{	
				is_networked = FALSE;
				// Snap: UN-Init MP save game directory
				if ( !InitSaveDir() )
				{

					//if something didnt work, dont even know how to make error code...//hayden
				}
			}

			if( gfKeyState[ ALT ] )
				gfLoadGameUponEntry = TRUE;
		}

		InitDependingGameStyleOptions();

		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RenderMainMenu();
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
}

void MenuButtonMoveCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		RenderMainMenu();
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_GAIN_MOUSE)
	{
		RenderMainMenu();
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void HandleMainMenuInput()
{
	InputAtom	InputEvent;

	// Check for esc
	while (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
	if( InputEvent.usEvent == KEY_UP )
		{
			switch( InputEvent.usParam )
			{
//#ifdef JA2TESTVERSION

				case 'n':
					gbHandledMainMenu = NEW_GAME;
					gfMainMenuScreenExit = TRUE;
					SetMainMenuExitScreen( INIT_SCREEN );
					break;

				case 'i':
					SetPendingNewScreen( INTRO_SCREEN );
					gfMainMenuScreenExit = TRUE;
					break;
//#endif
				case 'c':
					gbHandledMainMenu = LOAD_GAME;

					if( gfKeyState[ ALT ] )
						gfLoadGameUponEntry = TRUE;

					// WANNE: Some initializing was missing when directly loading last savegame
					// form main menu with ALT + C
					giMAXIMUM_NUMBER_OF_PLAYER_SLOTS = CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS;
					InitDependingGameStyleOptions();

					break;

				case 'o':
					gbHandledMainMenu = PREFERENCES;
					break;

				case 's':
					gbHandledMainMenu = CREDITS;
					break;

				case 'q':
					gfMainMenuScreenExit = TRUE;
					gfProgramIsRunning = FALSE;
					break;
			}
		}
	}
}

void HandleHelpScreenInput()
{
	InputAtom									InputEvent;

	// Check for key
	while (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
		switch( InputEvent.usEvent )
		{
			case KEY_UP:
				SetMainMenuExitScreen( INIT_SCREEN );
			break;
		}
	}
}


void ClearMainMenu()
{
	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;

	// CLEAR THE FRAME BUFFER
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	memset(pDestBuf, 0, SCREEN_HEIGHT * uiDestPitchBYTES );
	UnLockVideoSurface( FRAME_BUFFER );
	InvalidateScreen( );
}


void SelectMainMenuBackGroundRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}

void SetMainMenuExitScreen( UINT32 uiNewScreen )
{
	guiMainMenuExitScreen = uiNewScreen;

	//REmove the background region
	CreateDestroyBackGroundMouseMask( FALSE );

	gfMainMenuScreenExit = TRUE;
}


void CreateDestroyBackGroundMouseMask( BOOLEAN fCreate )
{
	static BOOLEAN fRegionCreated = FALSE;

	if( fCreate )
	{
		if( fRegionCreated )
			return;

		// Make a mouse region
		MSYS_DefineRegion( &(gBackRegion), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGHEST,
							CURSOR_NORMAL, MSYS_NO_CALLBACK, SelectMainMenuBackGroundRegionCallBack );
		// Add region
		MSYS_AddRegion(&(gBackRegion) );

		fRegionCreated = TRUE;
	}
	else
	{
		if( !fRegionCreated )
			return;

		MSYS_RemoveRegion( &gBackRegion );
		fRegionCreated = FALSE;
	}
}


BOOLEAN CreateDestroyMainMenuButtons( BOOLEAN fCreate )
{
	static BOOLEAN fButtonsCreated = FALSE;
	INT32 cnt;
	SGPFILENAME filename;
	SGPFILENAME filenameMP;
	INT16 sSlot;
	
	MAINMENU_X = SCREEN_WIDTH * gMainMenulayout[0].MAINMENU_X / 640;
	MAINMENU_Y = SCREEN_HEIGHT * gMainMenulayout[0].MAINMENU_Y / 480;
	MAINMENU_Y_SPACE = gMainMenulayout[0].MAINMENU_Y_SPACE;

	//if (iResolution >= _640x480 && iResolution < _800x600)
	//{
	//	MAINMENU_X =  gMainMenulayout[0].MAINMENU_X;
	//	MAINMENU_Y =  gMainMenulayout[0].MAINMENU_Y;
	//	MAINMENU_Y_SPACE = gMainMenulayout[0].MAINMENU_Y_SPACE;
	//}
	//else if (iResolution < _1024x768)
	//{
	//	MAINMENU_X =  gMainMenulayout[0].MAINMENU_800x600X + xResOffset;
	//	MAINMENU_Y =  gMainMenulayout[0].MAINMENU_800x600Y + yResOffset;
	//	MAINMENU_Y_SPACE = gMainMenulayout[0].MAINMENU_Y_SPACE;
	//}
	//else
	//{
	//	MAINMENU_X =  gMainMenulayout[0].MAINMENU_1024x768X + xResOffset;
	//	MAINMENU_Y =  gMainMenulayout[0].MAINMENU_1024x768Y + yResOffset;
	//	MAINMENU_Y_SPACE = gMainMenulayout[0].MAINMENU_Y_SPACE;
	//}

	if( fCreate )
	{
		if( fButtonsCreated )
			return( TRUE );

		//reset the variable that allows the user to ALT click on the continue save btn to load the save instantly
		gfLoadGameUponEntry = FALSE;

		// Load button images
		GetMLGFilename( filename, MLG_TITLETEXT );
		GetMLGFilename( filenameMP, MLG_TITLETEXT_MP );

#ifdef JA113DEMO
		// Load the "Start Demo" image
		iMenuImages[ NEW_GAME ] = LoadButtonImage( filename ,17,17,18,19,-1 );
#else
		iMenuImages[ NEW_GAME ]	= LoadButtonImage( filename, 0,0, 1, 2 ,-1 );
#endif
			
		iMenuImages[ NEW_MP_GAME ] = LoadButtonImage( filenameMP, 0, 0, 1, 2, -1 );

		sSlot = 0;
		iMenuImages[ LOAD_GAME ] = UseLoadedButtonImage( iMenuImages[ NEW_GAME ] ,6,3,4,5,-1 );

		iMenuImages[ PREFERENCES ] = UseLoadedButtonImage( iMenuImages[ NEW_GAME ] ,7,7,8,9,-1 );
		iMenuImages[ CREDITS ] = UseLoadedButtonImage( iMenuImages[ NEW_GAME ] ,13,10,11,12,-1 );
		iMenuImages[ QUIT ] = UseLoadedButtonImage( iMenuImages[ NEW_GAME ] ,14,14,15,16,-1 );

		for ( cnt = 0; cnt < NUM_MENU_ITEMS; cnt++ )
		{
			switch( cnt )
			{
				case NEW_GAME:		gusMainMenuButtonWidths[cnt] = GetWidthOfButtonPic( (UINT16)iMenuImages[cnt], sSlot );	break;
				case NEW_MP_GAME:	gusMainMenuButtonWidths[cnt] = GetWidthOfButtonPic( (UINT16)iMenuImages[cnt], 0);	break;
				case LOAD_GAME:		gusMainMenuButtonWidths[cnt] = GetWidthOfButtonPic( (UINT16)iMenuImages[cnt], 3 );			break;
				case PREFERENCES:	gusMainMenuButtonWidths[cnt] = GetWidthOfButtonPic( (UINT16)iMenuImages[cnt], 7 );			break;
				case CREDITS:			gusMainMenuButtonWidths[cnt] = GetWidthOfButtonPic( (UINT16)iMenuImages[cnt], 10 );			break;
				case QUIT:				gusMainMenuButtonWidths[cnt] = GetWidthOfButtonPic( (UINT16)iMenuImages[cnt], 15 );			break;
			}
#ifdef TESTFOREIGNFONTS
//			iMenuButtons[ cnt ] = QuickCreateButton( iMenuImages[ cnt ], (INT16)((SCREEN_WIDTH / 2) - gusMainMenuButtonWidths[cnt]/2), (INT16)( 0 + ( cnt * 18 ) ),
			iMenuButtons[ cnt ] = QuickCreateButton( iMenuImages[ cnt ], (INT16)(MAINMENU_X - gusMainMenuButtonWidths[cnt]/2), (INT16)( 0 + ( cnt * 18 ) ), //(INT16)((SCREEN_WIDTH / 2) - gusMainMenuButtonWidths[cnt]/2)
												BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
												DEFAULT_MOVE_CALLBACK, MenuButtonCallback );
#else			
			iMenuButtons[ cnt ] = QuickCreateButton( iMenuImages[ cnt ], (INT16)(MAINMENU_X - gusMainMenuButtonWidths[cnt]/2), (INT16)( MAINMENU_Y + ( cnt * MAINMENU_Y_SPACE ) ),
												BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
												DEFAULT_MOVE_CALLBACK, MenuButtonCallback );
#endif
			if ( iMenuButtons[ cnt ] == -1 )
			{
				return( FALSE );
			}

			ButtonList[ iMenuButtons[ cnt ] ]->UserData[0] = cnt;			
		}

		fButtonsCreated = TRUE;
	}
	else
	{
		if( !fButtonsCreated )
			return( TRUE );

		// Delete images/buttons
		for ( cnt = 0; cnt < NUM_MENU_ITEMS; cnt++ )
		{
			RemoveButton( iMenuButtons[ cnt ] );
			UnloadButtonImage( iMenuImages[ cnt ] );
		}
		fButtonsCreated = FALSE;
	}

	return( TRUE );
}


void RenderMainMenu()
{
	HVSURFACE hVSurface;	
	UINT32 iCounter2;
		
	//Get and display the background image
	for( iCounter2 = 1; iCounter2 < MAX_ELEMENT; iCounter2++ )
	{	
		if (gMainMenulayout[iCounter2].Visible == 1)
		{
			SGPRect SrcRect, DstRect;
			INT32 iPosX = 0, iPosY = 0;

			GetVideoSurface(&hVSurface, gMainMenulayout[iCounter2].uiIndex);
			
			SrcRect.iLeft = 0;
			SrcRect.iTop = 0;
			SrcRect.iRight = hVSurface->usWidth;
			SrcRect.iBottom = hVSurface->usHeight;
			
			//DstRect.iLeft = 0;
			//DstRect.iTop = 0;
			//DstRect.iRight = hVSurface->usWidth;
			//DstRect.iBottom = hVSurface->usHeight;
			
			//Stretch the background image to screen size
			if(gMainMenulayout[iCounter2].FitToScreen == 1)
			{
				DstRect.iLeft = 0;
				DstRect.iTop = 0;
				DstRect.iRight = SCREEN_WIDTH;
				DstRect.iBottom = SCREEN_HEIGHT;
			}
			else
			{
				if(gMainMenulayout[iCounter2].ImagePositionRelative == 1)
				{
					iPosX = SCREEN_WIDTH * gMainMenulayout[iCounter2].ImagePositionX / 640;
					iPosY = SCREEN_HEIGHT * gMainMenulayout[iCounter2].ImagePositionY / 480;
				}
				else
				{
					iPosX = gMainMenulayout[iCounter2].ImagePositionX;
					iPosY = gMainMenulayout[iCounter2].ImagePositionY;
				}

				DstRect.iLeft = iPosX;
				DstRect.iTop = iPosY;
				DstRect.iRight = iPosX + hVSurface->usWidth;
				DstRect.iBottom = iPosY + hVSurface->usHeight;

				if(gMainMenulayout[iCounter2].StretchImage == 1)
				{
					DstRect.iRight = iPosX + SCREEN_WIDTH * hVSurface->usWidth / 640;
					DstRect.iBottom = iPosY + SCREEN_HEIGHT * hVSurface->usHeight / 480;
				}

				if(gMainMenulayout[iCounter2].CenterImage == 1)
				{
					INT32 iTmpXSize = DstRect.iRight - DstRect.iLeft;

					DstRect.iLeft = SCREEN_WIDTH / 2 - iTmpXSize / 2;
					DstRect.iRight = DstRect.iLeft + iTmpXSize;
				}
			}

			BltStretchVideoSurface( FRAME_BUFFER, gMainMenulayout[iCounter2].uiIndex, 0, 0, VO_BLT_SRCTRANSPARENCY, &SrcRect, &DstRect );
			BltStretchVideoSurface( guiSAVEBUFFER, gMainMenulayout[iCounter2].uiIndex, 0, 0, VO_BLT_SRCTRANSPARENCY, &SrcRect, &DstRect );

			


			//if (iResolution >= _640x480 && iResolution < _800x600)
			//{	
			//	GetVideoObject(&hPixHandle, gMainMenulayout[iCounter2].uiIndex);
			//	BltVideoObject( guiSAVEBUFFER, hPixHandle, 0, gMainMenulayout[iCounter2].ImagePositionX + xResOffset, gMainMenulayout[iCounter2].ImagePositionY + yResOffset, VO_BLT_SRCTRANSPARENCY,NULL);
			//	BltVideoObject( FRAME_BUFFER, hPixHandle, 0, gMainMenulayout[iCounter2].ImagePositionX + xResOffset, gMainMenulayout[iCounter2].ImagePositionY + yResOffset, VO_BLT_SRCTRANSPARENCY,NULL);
			//}
			//else if (iResolution < _1024x768)
			//{	
			//	GetVideoObject(&hPixHandle, gMainMenulayout[iCounter2].uiIndex);
			//	BltVideoObject( guiSAVEBUFFER, hPixHandle, 0, gMainMenulayout[iCounter2].ImagePosition800x600X + xResOffset, gMainMenulayout[iCounter2].ImagePosition800x600Y + yResOffset, VO_BLT_SRCTRANSPARENCY,NULL);
			//	BltVideoObject( FRAME_BUFFER, hPixHandle, 0, gMainMenulayout[iCounter2].ImagePosition800x600X + xResOffset, gMainMenulayout[iCounter2].ImagePosition800x600Y + yResOffset, VO_BLT_SRCTRANSPARENCY,NULL);
			//}
			//else
			//{	
			//	GetVideoObject(&hPixHandle, gMainMenulayout[iCounter2].uiIndex);
			//	BltVideoObject( guiSAVEBUFFER, hPixHandle, 0, gMainMenulayout[iCounter2].ImagePosition1024x768X + xResOffset, gMainMenulayout[iCounter2].ImagePosition1024x768Y + yResOffset, VO_BLT_SRCTRANSPARENCY,NULL);
			//	BltVideoObject( FRAME_BUFFER, hPixHandle, 0, gMainMenulayout[iCounter2].ImagePosition1024x768X + xResOffset, gMainMenulayout[iCounter2].ImagePosition1024x768Y + yResOffset, VO_BLT_SRCTRANSPARENCY,NULL);
			//}
		}
	}

#ifdef TESTFOREIGNFONTS
	DrawTextToScreen( L"LARGEFONT1: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 105, 640, LARGEFONT1, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"SMALLFONT1: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 125, 640, SMALLFONT1, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"TINYFONT1: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 145, 640, TINYFONT1, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"FONT12POINT1: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 165, 640, FONT12POINT1, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"COMPFONT: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 185, 640, COMPFONT, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"SMALLCOMPFONT: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 205, 640, SMALLCOMPFONT, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"FONT10ROMAN: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 225, 640, FONT10ROMAN, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"FONT12ROMAN: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 245, 640, FONT12ROMAN, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"FONT14SANSERIF: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 255, 640, FONT14SANSERIF, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"MILITARYFONT: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 265, 640, MILITARYFONT1, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"FONT10ARIAL: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 285, 640, FONT10ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"FONT14ARIAL: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 305, 640, FONT14ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"FONT12ARIAL: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 325, 640, FONT12ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"FONT10ARIALBOLD: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 345, 640, FONT10ARIALBOLD, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"BLOCKFONT: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 365, 640, BLOCKFONT, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"BLOCKFONT2: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 385, 640, BLOCKFONT2, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"BLOCKFONT3: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 385, 640, BLOCKFONT3, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"FONT12ARIALFIXEDWIDTH: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 405, 640, FONT12ARIALFIXEDWIDTH, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"FONT16ARIAL: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 425, 640, FONT16ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"BLOCKFONTNARROW: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 445, 640, BLOCKFONTNARROW, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen( L"FONT14HUMANIST: ÄÀÁÂÇËÈÉÊÏÖÒÓÔÜÙÚÛäàáâçëèéêïöòóôüùúûÌÎìî"/*gzCopyrightText[ 0 ]*/, 0, 465, 640, FONT14HUMANIST, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
#else
	DrawTextToScreen( gzCopyrightText[ 0 ], 0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, FONT10ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
#endif

	InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
}

void RestoreButtonBackGrounds()
{
	UINT8	cnt;

#ifndef TESTFOREIGNFONTS

	MAINMENU_X = SCREEN_WIDTH * gMainMenulayout[0].MAINMENU_X / 640;
	MAINMENU_Y = SCREEN_HEIGHT * gMainMenulayout[0].MAINMENU_Y / 480;
	MAINMENU_Y_SPACE = gMainMenulayout[0].MAINMENU_Y_SPACE;

	//if (iResolution >= _640x480 && iResolution < _800x600)
	//{
	//	MAINMENU_Y =  gMainMenulayout[0].MAINMENU_Y;
	//	MAINMENU_X =  gMainMenulayout[0].MAINMENU_X;
	//	MAINMENU_Y_SPACE = gMainMenulayout[0].MAINMENU_Y_SPACE;
	//}
	//else if (iResolution < _1024x768)
	//{
	//	MAINMENU_Y =  gMainMenulayout[0].MAINMENU_800x600Y;
	//	MAINMENU_X =  gMainMenulayout[0].MAINMENU_800x600X;
	//	MAINMENU_Y_SPACE = gMainMenulayout[0].MAINMENU_Y_SPACE;
	//}
	//else
	//{
	//	MAINMENU_Y =  gMainMenulayout[0].MAINMENU_1024x768Y;
	//	MAINMENU_X =  gMainMenulayout[0].MAINMENU_1024x768X;
	//	MAINMENU_Y_SPACE = gMainMenulayout[0].MAINMENU_Y_SPACE;
	//}
			
	for ( cnt = 0; cnt < NUM_MENU_ITEMS; cnt++ )
	{		
		RestoreExternBackgroundRect( (UINT16)(MAINMENU_X - gusMainMenuButtonWidths[cnt]/2), (INT16)( MAINMENU_Y + ( cnt * MAINMENU_Y_SPACE )-1), (UINT16)(gusMainMenuButtonWidths[cnt]+1), 23 );
	}

#endif
}