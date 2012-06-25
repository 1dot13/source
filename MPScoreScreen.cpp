#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "Intro.h"
#else
	#include "Types.h"
	#include "MPScoreScreen.h"
	#include "GameSettings.h"
	#include "Utilities.h"
	#include "wCheck.h"
	#include "Font Control.h"
	#include "WordWrap.h"
	#include "Render Dirty.h"
	#include "Input.h"
	#include "Options Screen.h"
	#include "English.h"
	#include "Sysutil.h"
	#include "Fade Screen.h"
	#include "Cursor Control.h"
	#include "Music Control.h"
	#include "cursors.h"
	#include "Intro.h"
	#include "Text.h"
	#include "Text Input.h"
	#include "_Ja25EnglishText.h"
	#include "Soldier Profile.h"
#endif

#include "gameloop.h"
#include "Game Init.h"
#include "connect.h"
#include "network.h" // for client name
#include "saveloadscreen.h"


////////////////////////////////////////////
//
//	Global Defines
//
///////////////////////////////////////////

#define		MPS_TITLE_FONT							FONT14ARIAL//FONT16ARIAL
#define		MPS_TITLE_COLOR									FONT_MCOLOR_WHITE

#define		MPS_LABEL_TEXT_FONT						FONT12ARIAL//FONT16ARIAL
#define		MPS_LABEL_TEXT_COLOR							FONT_MCOLOR_WHITE

//buttons
#define		MPS_BTN_CONTINUE_X						iScreenWidthOffset + 320 + 143
#define		MPS_BTN_CONTINUE_Y						iScreenHeightOffset + 435
//#define		MPJ_BTN_HOST_X							MPJ_BTN_JOIN_X-180
//#define		MPJ_BTN_HOST_Y							iScreenHeightOffset + 435
#define		MPS_BTN_CANCEL_X						iScreenWidthOffset + ((320 - 115) / 2)
#define		MPS_BTN_CANCEL_Y						MPS_BTN_CONTINUE_Y

//main title
#define		MPS_MAIN_TITLE_X								0
#define		MPS_MAIN_TITLE_Y						iScreenHeightOffset + 10
#define		MPS_MAIN_TITLE_WIDTH					SCREEN_WIDTH

//labels
#define		MPS_LABEL_PLAYER_X						iScreenWidthOffset + 80
#define		MPS_LABEL_PLAYER_Y						iScreenHeightOffset + 80
#define		MPS_LABEL_PLAYER_WIDTH					80
#define		MPS_LABEL_KILLS_X						MPS_LABEL_PLAYER_X + MPS_LABEL_PLAYER_WIDTH + 20
#define		MPS_LABEL_KILLS_Y						MPS_LABEL_PLAYER_Y
#define		MPS_LABEL_KILLS_WIDTH					50
#define		MPS_LABEL_DEATHS_X						MPS_LABEL_KILLS_X + MPS_LABEL_KILLS_WIDTH + 20
#define		MPS_LABEL_DEATHS_Y						MPS_LABEL_PLAYER_Y
#define		MPS_LABEL_DEATHS_WIDTH					50
#define		MPS_LABEL_HITS_X						MPS_LABEL_DEATHS_X + MPS_LABEL_DEATHS_WIDTH + 20
#define		MPS_LABEL_HITS_Y						MPS_LABEL_PLAYER_Y
#define		MPS_LABEL_HITS_WIDTH					50
#define		MPS_LABEL_MISSES_X						MPS_LABEL_HITS_X + MPS_LABEL_HITS_WIDTH + 20
#define		MPS_LABEL_MISSES_Y						MPS_LABEL_PLAYER_Y
#define		MPS_LABEL_MISSES_WIDTH					50
#define		MPS_LABEL_ACCURACY_X					MPS_LABEL_MISSES_X + MPS_LABEL_MISSES_WIDTH + 20
#define		MPS_LABEL_ACCURACY_Y					MPS_LABEL_PLAYER_Y
#define		MPS_LABEL_ACCURACY_WIDTH				70


#define		MPS_PLAYERLIST_Y						MPS_LABEL_PLAYER_Y + 30
#define		MPS_PLAYER_GAP							17



////////////////////////////////////////////
//
//	Global Variables
//
///////////////////////////////////////////

BOOLEAN		gfMPSScreenEntry = TRUE;
BOOLEAN		gfMPSScreenExit	= FALSE;
BOOLEAN		gfReRenderMPSScreen=TRUE;
BOOLEAN		gfMPSButtonsAllocated = FALSE;
BOOLEAN		gfMPSScoreScreenCanContinue = FALSE;

//enum for different states of screen
enum
{
	MPS_NOTHING,
	MPS_CANCEL,
	MPS_CONTINUE,
};

UINT8		gubMPSScreenHandler=MPS_NOTHING;	// State changer for HandleMPJScreen()

UINT32		gubMPSExitScreen = MP_SCORE_SCREEN;	// The screen that is in control next iteration of the game_loop

UINT32		guiMPSMainBackGroundImage;

////////////////////////////////////////////
//
//	Screen Controls
//
///////////////////////////////////////////

// Cancel Button
void BtnMPSCancelCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiMPSCancelButton;
INT32		giMPSCancelBtnImage;

// Continue Button
void	BtnMPSContinueCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiMPSContinueButton;
INT32	giMPSContinueBtnImage;

// Message Box handle
INT8	giMPSMessageBox = -1;


////////////////////////////////////////////
//
//	Local Function Prototypes
//
///////////////////////////////////////////

extern void ClearMainMenu();

BOOLEAN		EnterMPSScreen();
BOOLEAN		ExitMPSScreen();
void		HandleMPSScreen();
BOOLEAN		RenderMPSScreen();

void		GetMPSScreenUserInput();
void			DoneFadeOutForExitMPSScreen( void );
void			DoneFadeInForExitMPSScreen( void );

UINT32	MPScoreScreenInit( void )
{
	return (1);
}

UINT32	MPScoreScreenHandle( void )
{
	StartFrameBufferRender();

	if( gfMPSScreenEntry )
	{
//		PauseGame();

		EnterMPSScreen();
		gfMPSScreenEntry = FALSE;
		gfMPSScreenExit = FALSE;
		InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	}

	GetMPSScreenUserInput();


	HandleMPSScreen();

	// render buttons marked dirty
	MarkButtonsDirty( );
	RenderButtons( );

	// render text boxes
	RenderAllTextFields(); // textbox system call

	// render help
//	RenderFastHelp( );
//	RenderButtonsFastHelp( );


	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();

	// handle fades in and out
	if ( HandleFadeOutCallback( ) )
	{
		ClearMainMenu();
		return( gubMPSExitScreen );
	}

	if ( HandleBeginFadeOut( gubMPSExitScreen ) )
	{
		return( gubMPSExitScreen );
	}

	if ( HandleFadeInCallback( ) )
	{
		// Re-render the scene!
		RenderMPSScreen();
	}

	if ( HandleBeginFadeIn( gubMPSExitScreen ) )
	{

	}

	if( gfMPSScreenExit ) // we are exiting this screen
	{
		ExitMPSScreen(); // perform destruction
	}

	return( gubMPSExitScreen );
} // end MPScoreScreenHandle()

UINT32	MPScoreScreenShutdown( void )
{
	return( 1 );
}

BOOLEAN		EnterMPSScreen()
{
	gfMPSScoreScreenCanContinue = FALSE;

	VOBJECT_DESC	VObjectDesc;

	if( gfMPSButtonsAllocated )
		return( TRUE );

	SetCurrentCursorFromDatabase( CURSOR_NORMAL );

	// load the Main trade screen backgroiund image
	ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

	if (iResolution >= _640x480 && iResolution < _800x600)
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround.sti", VObjectDesc.ImageFile);
	else if (iResolution < _1024x768)
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround_800x600.sti", VObjectDesc.ImageFile);
	else
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround_1024x768.sti", VObjectDesc.ImageFile);


	CHECKF(AddVideoObject(&VObjectDesc, &guiMPSMainBackGroundImage ));

	//Cancel button
	giMPSCancelBtnImage = LoadButtonImage("INTERFACE\\PreferencesButtons.sti", -1,0,-1,2,-1 );
	guiMPSCancelButton = CreateIconAndTextButton( giMPSCancelBtnImage, gzMPSScreenText[MPS_CANCEL_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MPS_BTN_CANCEL_X, MPS_BTN_CANCEL_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPSCancelCallback);

	SpecifyButtonSoundScheme( guiMPSCancelButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyDisabledButtonStyle( guiMPSCancelButton, DISABLED_STYLE_NONE );

	//Continue button
	giMPSContinueBtnImage = UseLoadedButtonImage( giMPSCancelBtnImage, -1,1,-1,3,-1 );
	guiMPSContinueButton = CreateIconAndTextButton( giMPSContinueBtnImage, gzMPSScreenText[MPS_CONTINUE_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MPS_BTN_CONTINUE_X, MPS_BTN_CONTINUE_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPSContinueCallback);

	SpecifyButtonSoundScheme( guiMPSContinueButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyDisabledButtonStyle( guiMPSContinueButton, DISABLED_STYLE_NONE );

	//Reset the exit screen - screen the main game loop will call next iteration
	gubMPSExitScreen = MP_SCORE_SCREEN;

	//REnder the screen once so we can blt ot to ths save buffer
	RenderMPSScreen();

	BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	gfMPSButtonsAllocated = TRUE;

	return( TRUE );

} // End of EnterMPSScreen()


BOOLEAN		ExitMPSScreen()
{
	if( !gfMPSButtonsAllocated )
		return( TRUE );

	//Delete the main options screen background
	DeleteVideoObjectFromIndex( guiMPSMainBackGroundImage );

	RemoveButton( guiMPSContinueButton );
	RemoveButton( guiMPSCancelButton );


	UnloadButtonImage( giMPSCancelBtnImage );
	UnloadButtonImage( giMPSContinueBtnImage );

	// exit text input mode in this screen and clean up text boxes
	//KillAllTextInputModes();
	SetTextInputCursor( CURSOR_IBEAM );

	gfMPSButtonsAllocated = FALSE;

	//If we are starting the game stop playing the music
	if( gubMPSScreenHandler == MPS_CONTINUE )
		SetMusicMode( MUSIC_NONE );

	gfMPSScreenExit = FALSE;
	gfMPSScreenEntry = TRUE;

	return( TRUE );

} // End of ExitMPJScreen()


void			HandleMPSScreen()
{
	if( gubMPSScreenHandler != MPS_NOTHING )
	{
		switch( gubMPSScreenHandler )
		{
			case MPS_CANCEL:

				if (is_server)
					server_disconnect();

				if (is_client)
					client_disconnect();

				gubMPSExitScreen = MAINMENU_SCREEN;
				gfMPSScreenExit	= TRUE;
				break;
			
			/*case MPJ_HOST:
				gubMPJExitScreen = MP_HOST_SCREEN;
				gfMPJScreenExit	= TRUE;
				break;*/


			case MPS_CONTINUE:
			{
				//if we are already fading out, get out of here
				if( gFadeOutDoneCallback != DoneFadeOutForExitMPSScreen )
				{
					//Disable the ok button
					DisableButton( guiMPSCancelButton );
					DisableButton( guiMPSContinueButton );

					gFadeOutDoneCallback = DoneFadeOutForExitMPSScreen;

					FadeOutNextFrame( );
				}
				break;
			}

		}

		gubMPSScreenHandler = MPS_NOTHING;
	}

	// disable the continue button
	if (!is_server)
	{
		if (gfMPSScoreScreenCanContinue)
			ShowButton( guiMPSContinueButton );
		else
			HideButton( guiMPSContinueButton );
	}
	


	if( gfReRenderMPSScreen )
	{
		RenderMPSScreen();
		gfReRenderMPSScreen = FALSE;
	}

	// <TODO> add restore backgrounds in...
	//RestoreGIOButtonBackGrounds();
} // end of HandleMPJScreen


BOOLEAN		RenderMPSScreen()
{
	HVOBJECT	hPixHandle;
	UINT16		usPosY;

	//Get the main background screen graphic and blt it
	GetVideoObject(&hPixHandle, guiMPSMainBackGroundImage );

		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,(SCREEN_WIDTH - xResSize)/2,(SCREEN_HEIGHT - yResSize)/2, VO_BLT_SRCTRANSPARENCY,NULL);

	//Shade the background
	ShadowVideoSurfaceRect( FRAME_BUFFER, iScreenWidthOffset, iScreenHeightOffset, iScreenWidthOffset + 640, iScreenHeightOffset + 480 );

	//Display the title
	DrawTextToScreen( gzMPSScreenText[ MPS_TITLE_TEXT ], MPS_MAIN_TITLE_X, MPS_MAIN_TITLE_Y, MPS_MAIN_TITLE_WIDTH, MPS_TITLE_FONT, MPS_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// Player Column
	DisplayWrappedString( MPS_LABEL_PLAYER_X, MPS_LABEL_PLAYER_Y, MPS_LABEL_PLAYER_WIDTH, 2, MPS_TITLE_FONT, MPS_TITLE_COLOR, gzMPSScreenText[ MPS_PLAYER_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	// Kills Column
	DisplayWrappedString( MPS_LABEL_KILLS_X, MPS_LABEL_KILLS_Y, MPS_LABEL_KILLS_WIDTH, 2, MPS_TITLE_FONT, MPS_TITLE_COLOR, gzMPSScreenText[ MPS_KILLS_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	// Deaths Column
	DisplayWrappedString( MPS_LABEL_DEATHS_X, MPS_LABEL_DEATHS_Y, MPS_LABEL_DEATHS_WIDTH, 2, MPS_TITLE_FONT, MPS_TITLE_COLOR, gzMPSScreenText[ MPS_DEATHS_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	// Hits Column
	DisplayWrappedString( MPS_LABEL_HITS_X, MPS_LABEL_HITS_Y, MPS_LABEL_HITS_WIDTH, 2, MPS_TITLE_FONT, MPS_TITLE_COLOR, gzMPSScreenText[ MPS_HITS_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	// Misses Column
	DisplayWrappedString( MPS_LABEL_MISSES_X, MPS_LABEL_MISSES_Y, MPS_LABEL_MISSES_WIDTH, 2, MPS_TITLE_FONT, MPS_TITLE_COLOR, gzMPSScreenText[ MPS_MISSES_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	// Accuracy Column
	DisplayWrappedString( MPS_LABEL_ACCURACY_X, MPS_LABEL_ACCURACY_Y, MPS_LABEL_ACCURACY_WIDTH, 2, MPS_TITLE_FONT, MPS_TITLE_COLOR, gzMPSScreenText[ MPS_ACCURACY_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Draw players
	wchar_t szPlayerName[30];
	wchar_t szPlayerKills[4];
	wchar_t szPlayerDeaths[4];
	wchar_t szPlayerHits[4];
	wchar_t szPlayerMisses[4];
	wchar_t szPlayerAccuracy[8];
	float flAccuracy = 0;
	for(int i=0; i < 4; i++)
	{
		if (client_names[i] != NULL)
		{
			if (strcmp(client_names[i],"")!=0)
			{
				// valid player
				usPosY = MPS_PLAYERLIST_Y + (i * MPS_PLAYER_GAP);

				// Draw Player Name Column
				memset(szPlayerName,0,30*sizeof(wchar_t));
				mbstowcs( szPlayerName,client_names[i],30);
				
				DisplayWrappedString( MPS_LABEL_PLAYER_X, usPosY, MPS_LABEL_PLAYER_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, szPlayerName, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

				// Draw Kills Column
				memset(szPlayerKills,0,4*sizeof(wchar_t));
				_itow(gMPPlayerStats[i].kills,szPlayerKills,10);
				DisplayWrappedString( MPS_LABEL_KILLS_X, usPosY, MPS_LABEL_KILLS_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, szPlayerKills, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

				// Draw Deaths Column
				memset(szPlayerDeaths,0,4*sizeof(wchar_t));
				_itow(gMPPlayerStats[i].deaths,szPlayerDeaths,10);
				DisplayWrappedString( MPS_LABEL_DEATHS_X, usPosY, MPS_LABEL_DEATHS_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, szPlayerDeaths, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

				// Draw Hits Column
				memset(szPlayerHits,0,4*sizeof(wchar_t));
				_itow(gMPPlayerStats[i].hits,szPlayerHits,10);
				DisplayWrappedString( MPS_LABEL_HITS_X, usPosY, MPS_LABEL_HITS_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, szPlayerHits, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

				// Draw Misses Column
				memset(szPlayerMisses,0,4*sizeof(wchar_t));
				_itow(gMPPlayerStats[i].misses,szPlayerMisses,10);
				DisplayWrappedString( MPS_LABEL_MISSES_X, usPosY, MPS_LABEL_MISSES_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, szPlayerMisses, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

				// Draw Accuracy Column
				if ( gMPPlayerStats[i].misses + gMPPlayerStats[i].hits > 0 )
					flAccuracy = ((float)gMPPlayerStats[i].hits / (float)(gMPPlayerStats[i].misses + gMPPlayerStats[i].hits)) * 100.0f;
				else
					flAccuracy = 0;
				memset(szPlayerAccuracy,0,8*sizeof(wchar_t));
				swprintf(szPlayerAccuracy,L"%i%%%%%%%%",(int)flAccuracy); // this thing goes through like three printfs before being rendered....
				DisplayWrappedString( MPS_LABEL_ACCURACY_X, usPosY, MPS_LABEL_ACCURACY_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, szPlayerAccuracy, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

			}

		}
	}

	if (cGameType==MP_TYPE_COOP)
	{
		// CO-OP Mode, show stats for queens army
		usPosY = MPS_PLAYERLIST_Y + (5 * MPS_PLAYER_GAP); // leave a space between players and AI

		// Draw AI Team Name
		DisplayWrappedString( MPS_LABEL_PLAYER_X, usPosY, MPS_LABEL_PLAYER_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, gzMPSScreenText[MPS_AITEAM_TEXT], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

		// Draw Kills Column
		memset(szPlayerKills,0,4*sizeof(wchar_t));
		_itow(gMPPlayerStats[4].kills,szPlayerKills,10);
		DisplayWrappedString( MPS_LABEL_KILLS_X, usPosY, MPS_LABEL_KILLS_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, szPlayerKills, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

		// Draw Deaths Column
		memset(szPlayerDeaths,0,4*sizeof(wchar_t));
		_itow(gMPPlayerStats[4].deaths,szPlayerDeaths,10);
		DisplayWrappedString( MPS_LABEL_DEATHS_X, usPosY, MPS_LABEL_DEATHS_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, szPlayerDeaths, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

		// Draw Hits Column
		memset(szPlayerHits,0,4*sizeof(wchar_t));
		_itow(gMPPlayerStats[4].hits,szPlayerHits,10);
		DisplayWrappedString( MPS_LABEL_HITS_X, usPosY, MPS_LABEL_HITS_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, szPlayerHits, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

		// Draw Misses Column
		memset(szPlayerMisses,0,4*sizeof(wchar_t));
		_itow(gMPPlayerStats[4].misses,szPlayerMisses,10);
		DisplayWrappedString( MPS_LABEL_MISSES_X, usPosY, MPS_LABEL_MISSES_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, szPlayerMisses, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

		// Draw Accuracy Column
		if ( gMPPlayerStats[4].misses + gMPPlayerStats[4].hits > 0 )
			flAccuracy = ((float)gMPPlayerStats[4].hits / (float)(gMPPlayerStats[4].misses + gMPPlayerStats[4].hits)) * 100.0f;
		else
			flAccuracy = 0;
		memset(szPlayerAccuracy,0,8*sizeof(wchar_t));
		swprintf(szPlayerAccuracy,L"%i%%%%%%%%",(int)flAccuracy);
		DisplayWrappedString( MPS_LABEL_ACCURACY_X, usPosY, MPS_LABEL_ACCURACY_WIDTH, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, szPlayerAccuracy, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	}

	// OJW - 20090403
	// Draw Please wait for server message
	if (!is_server)
	{
		if (!gfMPSScoreScreenCanContinue)
			DisplayWrappedString( MPS_BTN_CONTINUE_X, MPS_BTN_CONTINUE_Y, 100, 2, MPS_LABEL_TEXT_FONT, MPS_LABEL_TEXT_COLOR, gzMPSScreenText[MPS_WAITSERVER_TEXT], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	}

/*	
	// Player name text label
	DisplayWrappedString( MPJ_LABEL_HANDLE_X, MPJ_LABEL_HANDLE_Y, MPJ_LABEL_HANDLE_WIDTH, 2, MPJ_LABEL_TEXT_FONT, MPJ_LABEL_TEXT_COLOR, gzMPJScreenText[ MPJ_HANDLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Server IP text label
	DisplayWrappedString( MPJ_LABEL_IP_X, MPJ_LABEL_IP_Y, MPJ_LABEL_IP_WIDTH, 2, MPJ_LABEL_TEXT_FONT, MPJ_LABEL_TEXT_COLOR, gzMPJScreenText[ MPJ_SERVERIP_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Server Port text label
	DisplayWrappedString( MPJ_LABEL_PORT_X, MPJ_LABEL_PORT_Y, MPJ_LABEL_PORT_WIDTH, 2, MPJ_LABEL_TEXT_FONT, MPJ_LABEL_TEXT_COLOR, gzMPJScreenText[ MPJ_SERVERPORT_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
*/
	return( TRUE );
} // end of RenderMPSScreen()



void GetMPSScreenUserInput()
{
	InputAtom Event;

#ifdef USE_HIGHSPEED_GAMELOOP_TIMER
	while (DequeueSpecificEvent(&Event, KEY_DOWN|KEY_UP|KEY_REPEAT))
#else
	while( DequeueEvent( &Event ) )
#endif
	{
		// check if this event is swallowed by text input, otherwise process key
		if( !HandleTextInput( &Event ) && Event.usEvent == KEY_DOWN )
		{
			switch( Event.usParam )
			{

				case ESC:
					//Exit out of the screen
					gubMPSScreenHandler = MPS_CANCEL;
					break;

				case ENTER:					
					gubMPSScreenHandler = MPS_CONTINUE;
					break;
			}
		}
	}
} // end of GetMPSScreenUserInput()

void DoneFadeOutForExitMPSScreen( void )
{
	// As we bypassed the GIO screen, set up some game options for multiplayer here
	// <TODO> most things i have left as thier defaults here for testing.
	is_networked = true;
	
	ReStartingGame();

	if (is_client)
	{
		client_disconnect();
	}

	if(is_server)
	{
		server_disconnect();
	}

	//NetworkAutoStart will handle reconnection in Map Screen
	// reset client auto_retry for NetworkAutoStart called in MapScreenHandle
	auto_retry = true;
	giNumTries = 5;

	if (is_networked)
		gGameOptions.fTurnTimeLimit = TRUE;
	else
		gGameOptions.fTurnTimeLimit = FALSE;
	
	// Bobby Rays - why would we want anything less than the best
	gGameOptions.ubBobbyRayQuality = BR_AWESOME;
	gGameOptions.ubBobbyRayQuantity = BR_AWESOME;

	gubMPSExitScreen = INTRO_SCREEN;

	//set the fact that we should do the intro videos
//	gbIntroScreenMode = INTRO_BEGINING;
#ifdef JA2TESTVERSION
	if( gfKeyState[ ALT ] )
	{
		if( gfKeyState[ CTRL ] )
		{
			gMercProfiles[ MIGUEL ].bMercStatus = MERC_IS_DEAD;
			gMercProfiles[ SKYRIDER ].bMercStatus = MERC_IS_DEAD;
		}

		SetIntroType( INTRO_ENDING );
	}
	else
#endif

#ifdef JA2UB
	//SetIntroType( INTRO_BEGINNING );
#else
		SetIntroType( INTRO_BEGINNING );
#endif

	ExitMPSScreen(); // cleanup please, if we called a fadeout then we didnt do it above

	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

void DoneFadeInForExitMPSScreen( void )
{
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

void BtnMPSCancelCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gubMPSScreenHandler = MPS_CANCEL;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}

}

void BtnMPSContinueCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gubMPSScreenHandler = MPS_CONTINUE;


		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}