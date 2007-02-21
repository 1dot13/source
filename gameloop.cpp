#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "HelpScreen.h"
	#include "Prebattle Interface.h"
#else
	#include <stdio.h>
	#include "sgp.h"
	#include "Gameloop.h"
	#include "Screens.h"
	#include "Wcheck.h"
	#include "cursors.h"  
	#include "init.h"
	#include "music control.h"
	#include "sys globals.h"
	#include "laptop.h"
	#include "mapscreen.h"
	#include "Game Clock.h"
	#include "Timer Control.h"
	#include "overhead.h"
	#include "LibraryDataBase.h"
	#include "Map Screen Interface.h"
	#include "Tactical Save.h"
	#include "Interface.h"
	#include "GameSettings.h"
	#include "mapscreen.h"
	#include "Interface Control.h"
	#include "fade screen.h"
	#include "text.h"
	#include "HelpScreen.h"
#endif

// rain
#include "Rain.h" 
// end rain


UINT32 guiCurrentScreen;
UINT32 guiPendingScreen = NO_PENDING_SCREEN;
UINT32 guiPreviousScreen = NO_PENDING_SCREEN;

INT32	 giStartingMemValue = 0;

#define	DONT_CHECK_FOR_FREE_SPACE		255
UINT8		gubCheckForFreeSpaceOnHardDriveCount=DONT_CHECK_FOR_FREE_SPACE;

extern	BOOLEAN	DoSkiMessageBox( UINT8 ubStyle, STR16 zString, UINT32 uiExitScreen, UINT8 ubFlags, MSGBOX_CALLBACK ReturnCallback );

extern void NotEnoughHardDriveSpaceForQuickSaveMessageBoxCallBack( UINT8 bExitValue );
extern BOOLEAN gfTacticalPlacementGUIActive;
extern BOOLEAN gfTacticalPlacementGUIDirty;
extern BOOLEAN gfValidLocationsChanged; 
extern BOOLEAN	gfInMsgBox;
extern void InitSightRange(); //lal


// callback to confirm game is over
void EndGameMessageBoxCallBack( UINT8 bExitValue );
void HandleNewScreenChange( UINT32 uiNewScreen, UINT32 uiOldScreen );

// The InitializeGame function is responsible for setting up all data and Gaming Engine
// tasks which will run the game

#ifdef JA2BETAVERSION
BOOLEAN gubReportMapscreenLock = 0;
void ReportMapscreenErrorLock()
{
	switch( gubReportMapscreenLock )
	{
		case 1:
			DoScreenIndependantMessageBox( L"You have just loaded the game which is in a state that you shouldn't be able to.  You can still play, but there should be a sector with enemies co-existing with mercs.  Please don't report that.", MSG_BOX_FLAG_OK, NULL );
			fDisableDueToBattleRoster = FALSE;
			fDisableMapInterfaceDueToBattle = FALSE;
			gubReportMapscreenLock = 0;
			break;
		case 2:
			DoScreenIndependantMessageBox( L"You have just saved the game which is in a state that you shouldn't be able to.  Please report circumstances (ex:  merc in other sector pipes up about enemies), etc.  Autocorrected, but if you reload the save, don't report the error appearing in load.", MSG_BOX_FLAG_OK, NULL );
			fDisableDueToBattleRoster = FALSE;
			fDisableMapInterfaceDueToBattle = FALSE;
			gubReportMapscreenLock = 0;
			break;
	}
}
#endif

BOOLEAN InitializeGame(void)
{ 
	UINT32				uiIndex;

	giStartingMemValue = MemGetFree( );


	ClearAllDebugTopics();
	RegisterJA2DebugTopic( TOPIC_JA2OPPLIST, "Reg" );
	//RegisterJA2DebugTopic( TOPIC_MEMORY_MANAGER, "Reg" );


	// Snap: Read options from an INI file in the default of custom Data directory
	// Moved this up because some settings are used during other inits
	LoadGameExternalOptions();
	InitSightRange(); //lal

	// Initlaize mouse subsystems
	MSYS_Init( );
	InitButtonSystem();
	InitCursors( );

	// Init Fonts
	if ( !InitializeFonts( ) )
	{
		// Send debug message and quit
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "COULD NOT INUT FONT SYSTEM...");
		return( ERROR_SCREEN );
	}

	//Deletes all the Temp files in the Maps\Temp directory
	InitTacticalSave( TRUE );

	// Initialize Game Screens.
	for (uiIndex = 0; uiIndex < MAX_SCREENS; uiIndex++)
	{ 
		if ((*(GameScreens[uiIndex].InitializeScreen))() == FALSE)
		{ // Failed to initialize one of the screens. 
			return FALSE;
		}
	}

	//Init the help screen system
	InitHelpScreenSystem();

	//Loads the saved (if any) general JA2 game settings
	LoadGameSettings();

	//Initialize the Game options ( Gun nut, scifi and dif. levels
	InitGameOptions();

	// preload mapscreen graphics
	HandlePreloadOfMapGraphics( );

	guiCurrentScreen = INIT_SCREEN;

	return TRUE;
}

// The ShutdownGame function will free up/undo all things that were started in InitializeGame()
// It will also be responsible to making sure that all Gaming Engine tasks exit properly

void ShutdownGame(void)
{ 
	// handle shutdown of game with respect to preloaded mapscreen graphics
	HandleRemovalOfPreLoadedMapGraphics( );

	 ShutdownJA2( );

	//Save the general save game settings to disk
	SaveGameSettings();


	 //shutdown the file database manager
	 ShutDownFileDatabase( );


	//Deletes all the Temp files in the Maps\Temp directory
	InitTacticalSave( FALSE );
}

 
// This is the main Gameloop. This should eventually be one big switch statement which represents
// the state of the game (i.e. Main Menu, PC Generation, Combat loop, etc....)
// This function exits constantly and reenters constantly

// rain
static BOOLEAN gfSkipFrame = FALSE;

void GameLoop(void)
{
	//	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop");

	InputAtom	InputEvent;
	POINT		MousePos;
	UINT32		uiOldScreen=guiCurrentScreen;
	clock_t		startTime = clock(); // decrease CPU load patch from defrog

	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: get mouse position");
	GetCursorPos(&MousePos);
	
	// Hook into mouse stuff for MOVEMENT MESSAGES
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: get mouse hook");
	MouseSystemHook(MOUSE_POS, (UINT16)MousePos.x ,(UINT16)MousePos.y ,_LeftButtonDown, _RightButtonDown);
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: get music");
	MusicPoll( FALSE );

	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: check for mouse events");
	while (DequeueSpecificEvent(&InputEvent, LEFT_BUTTON_REPEAT|RIGHT_BUTTON_REPEAT|LEFT_BUTTON_DOWN|LEFT_BUTTON_UP|RIGHT_BUTTON_DOWN|RIGHT_BUTTON_UP ) == TRUE )
	{
		// HOOK INTO MOUSE HOOKS
	  //DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("GameLoop: mouse event %d", InputEvent.usEvent ));
		switch(InputEvent.usEvent)
	  {
			case LEFT_BUTTON_DOWN:
				MouseSystemHook(LEFT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case LEFT_BUTTON_UP:
				MouseSystemHook(LEFT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y ,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_DOWN:
				MouseSystemHook(RIGHT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_UP: 
				MouseSystemHook(RIGHT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case LEFT_BUTTON_REPEAT: 
				MouseSystemHook(LEFT_BUTTON_REPEAT, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_REPEAT: 
				MouseSystemHook(RIGHT_BUTTON_REPEAT, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
	  }
	}



	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: check for errors");
	if ( gfGlobalError )
	{
		//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: global error");

		guiCurrentScreen = ERROR_SCREEN;
	}

/*
	// Madd: removed check because it kept coming up for me, even on new games, even though I have 12GB free!!  I think the "DoesUserHaveEnoughHardDriveSpace" function is busted.
	//if we are to check for free space on the hard drive
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: check hard drive");
	if( gubCheckForFreeSpaceOnHardDriveCount < DONT_CHECK_FOR_FREE_SPACE )
	{
		//only if we are in a screen that can get this check
		if( guiCurrentScreen == MAP_SCREEN || guiCurrentScreen == GAME_SCREEN || guiCurrentScreen == SAVE_LOAD_SCREEN )
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: check for free hard drive space");
			if( gubCheckForFreeSpaceOnHardDriveCount < 1 )
			{
				gubCheckForFreeSpaceOnHardDriveCount++;
			}
			else
			{
				// Make sure the user has enough hard drive space
				if( !DoesUserHaveEnoughHardDriveSpace() )
				{
					CHAR16	zText[512];
					CHAR16	zSpaceOnDrive[512];
					UINT32	uiSpaceOnDrive;
					CHAR16	zSizeNeeded[512];

					swprintf( zSizeNeeded, L"%d", REQUIRED_FREE_SPACE / BYTESINMEGABYTE );
					InsertCommasForDollarFigure( zSizeNeeded );

					uiSpaceOnDrive = GetFreeSpaceOnHardDriveWhereGameIsRunningFrom( );

					swprintf( zSpaceOnDrive, L"%.2f", uiSpaceOnDrive / (FLOAT)BYTESINMEGABYTE );

					swprintf( zText, pMessageStrings[ MSG_LOWDISKSPACE_WARNING ], zSpaceOnDrive, zSizeNeeded );

					if( guiPreviousOptionScreen == MAP_SCREEN )
						DoMapMessageBox( MSG_BOX_BASIC_STYLE, (INT16 *)zText, MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
					else
						DoMessageBox( MSG_BOX_BASIC_STYLE, zText, GAME_SCREEN, MSG_BOX_FLAG_OK, NULL, NULL );
				}
				gubCheckForFreeSpaceOnHardDriveCount = DONT_CHECK_FOR_FREE_SPACE;
			}
		}
	}
*/
	// ATE: Force to be in message box screen!
	if ( gfInMsgBox )
	{
		//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: message box");
		guiPendingScreen = MSG_BOX_SCREEN;
	}

	if ( guiPendingScreen != NO_PENDING_SCREEN )
	{
		// Based on active screen, deinit!
		if( guiPendingScreen != guiCurrentScreen )
		{
			switch( guiCurrentScreen )
			{
			case MAP_SCREEN:
				if( guiPendingScreen != MSG_BOX_SCREEN )
				{
					EndMapScreen( FALSE );
				}
				break;
			case LAPTOP_SCREEN:
				ExitLaptop();
				break;
			}
		}

		// if the screen has chnaged
		if( uiOldScreen != guiPendingScreen )
		{
			// Set the fact that the screen has changed
			uiOldScreen = guiPendingScreen;

			HandleNewScreenChange( guiPendingScreen, guiCurrentScreen );
		}
		guiCurrentScreen = guiPendingScreen;
		guiPendingScreen = NO_PENDING_SCREEN;

	}

	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: screen changed");


	uiOldScreen = (*(GameScreens[guiCurrentScreen].HandleScreen))(); 

	// if the screen has chnaged
	if( uiOldScreen != guiCurrentScreen )
	{
		HandleNewScreenChange( uiOldScreen, guiCurrentScreen );
		guiCurrentScreen = uiOldScreen;
	}

	// rain
	RenderRain();

	if( gfSkipFrame )
		gfSkipFrame = FALSE;
	else
		// end rain

		//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: refresh screen");
		RefreshScreen( NULL );

	guiGameCycleCounter++;

	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop: update clock");
	UpdateClock();

#ifdef JA2BETAVERSION
	if( gubReportMapscreenLock )
	{
		ReportMapscreenErrorLock();
	}
#endif

	if( gGameSettings.fOptions[ TOPTION_LOW_CPU_USAGE ] == TRUE )
	{
		// decrease CPU load patch from MTX (http://www.ja-galaxy-forum.com/board/ubbthreads.php/ubb/showflat/Number/102405/page/1#Post102405)
		//OK! 12% CPU:
		//Sleep(10);

		// decrease CPU load patch from defrog
		clock_t endTime = clock();

		//long sleeptime = 33 - (endTime-startTime)/(CLK_TCK/1000);
		long sleeptime = 33 - (endTime-startTime);

		if( sleeptime > 0 )
			Sleep(sleeptime);
	}

	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GameLoop done");
} 

void SetCurrentScreen( UINT32 uiNewScreen )
{
	guiCurrentScreen = uiNewScreen;
 (*(GameScreens[guiCurrentScreen].HandleScreen))(); 

}

void SetPendingNewScreen( UINT32 uiNewScreen )
{
	guiPendingScreen = uiNewScreen;
}

// rain
extern UINT32 guiRainLoop;
// end rain

// Gets called when the screen changes, place any needed in code in here
void HandleNewScreenChange( UINT32 uiNewScreen, UINT32 uiOldScreen )
{	
	//if we are not going into the message box screen, and we didnt just come from it
	if( ( uiNewScreen != MSG_BOX_SCREEN && uiOldScreen != MSG_BOX_SCREEN ) )
	{
		//reset the help screen
		NewScreenSoResetHelpScreen( );
	}

	//rain
	if( uiNewScreen == MAP_SCREEN )
	{
		if ( guiRainLoop != NO_SAMPLE )
		{
			SoundStop( guiRainLoop );
			guiRainLoop = NO_SAMPLE;
		}
	}
	// end rain
}

void HandleShortCutExitState( void )
{
	// look at the state of fGameIsRunning, if set false, then prompt user for confirmation

	// use YES/NO Pop up box, settup for particular screen
	SGPRect pCenteringRect= {0, 0, SCREEN_WIDTH, INV_INTERFACE_START_Y };

	if( guiCurrentScreen == ERROR_SCREEN )
	{ //an assert failure, don't bring up the box!
		gfProgramIsRunning = FALSE;
		return;
	}

	if( guiCurrentScreen == AUTORESOLVE_SCREEN )
	{
		DoMessageBox(  MSG_BOX_BASIC_STYLE, pMessageStrings[ MSG_EXITGAME ],  guiCurrentScreen, ( UINT8 ) ( MSG_BOX_FLAG_YESNO | MSG_BOX_FLAG_USE_CENTERING_RECT ),  EndGameMessageBoxCallBack,  &pCenteringRect );
		return;
	}

	/// which screen are we in?
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		// set up for mapscreen
		DoMapMessageBox( MSG_BOX_BASIC_STYLE,(INT16 *) pMessageStrings[ MSG_EXITGAME ], MAP_SCREEN, MSG_BOX_FLAG_YESNO, EndGameMessageBoxCallBack );

	}
	else if( guiCurrentScreen == LAPTOP_SCREEN )
	{
		// set up for laptop
		DoLapTopSystemMessageBox( MSG_BOX_LAPTOP_DEFAULT, (INT16 *) pMessageStrings[ MSG_EXITGAME ], LAPTOP_SCREEN, MSG_BOX_FLAG_YESNO, EndGameMessageBoxCallBack );
	}
	else if( guiCurrentScreen == SHOPKEEPER_SCREEN )
	{
		DoSkiMessageBox( MSG_BOX_BASIC_STYLE, (STR16)pMessageStrings[ MSG_EXITGAME ], SHOPKEEPER_SCREEN, MSG_BOX_FLAG_YESNO, EndGameMessageBoxCallBack );
	}
	else
	{

		// check if error or editor
#ifdef JA2BETAVERSION
		if ( guiCurrentScreen == AIVIEWER_SCREEN || guiCurrentScreen == QUEST_DEBUG_SCREEN )
		{
			// then don't prompt
			gfProgramIsRunning = FALSE;
			return;
		}
#endif

		if( ( guiCurrentScreen == ERROR_SCREEN ) || ( guiCurrentScreen == EDIT_SCREEN ) || ( guiCurrentScreen == DEBUG_SCREEN ) )
		{
			// then don't prompt
			gfProgramIsRunning = FALSE;
			return;
		}

		// set up for all otherscreens
		DoMessageBox(  MSG_BOX_BASIC_STYLE, pMessageStrings[ MSG_EXITGAME ],  guiCurrentScreen, ( UINT8 ) ( MSG_BOX_FLAG_YESNO | MSG_BOX_FLAG_USE_CENTERING_RECT ),  EndGameMessageBoxCallBack,  &pCenteringRect );
	}
}


void EndGameMessageBoxCallBack( UINT8 bExitValue )
{
	// yes, so start over, else stay here and do nothing for now
  if( bExitValue == MSG_BOX_RETURN_YES )
	{
		gfProgramIsRunning = FALSE;
	}

	//If we are in the tactical placement gui, we need this flag set so the interface is updated.
	if( gfTacticalPlacementGUIActive )
	{
		gfTacticalPlacementGUIDirty = TRUE;
		gfValidLocationsChanged = TRUE; 
	}

	return;
}


void NextLoopCheckForEnoughFreeHardDriveSpace()
{
	gubCheckForFreeSpaceOnHardDriveCount = 0;
}
