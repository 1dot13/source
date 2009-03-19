#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "Intro.h"
#else
	#include "Types.h"
	#include "MPJoinScreen.h"
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
#include "connect.h"
#include "network.h" // for client name
#include "saveloadscreen.h"


////////////////////////////////////////////
//
//	Global Defines
//
///////////////////////////////////////////

#define		MPJ_TITLE_FONT							FONT14ARIAL//FONT16ARIAL
#define		MPJ_TITLE_COLOR									FONT_MCOLOR_WHITE

#define		MPJ_LABEL_TEXT_FONT					FONT12ARIAL//FONT16ARIAL
#define		MPJ_LABEL_TEXT_COLOR							FONT_MCOLOR_WHITE

//buttons
#define		MPJ_BTN_JOIN_X							iScreenWidthOffset + 320 + 143
#define		MPJ_BTN_JOIN_Y							iScreenHeightOffset + 435
#define		MPJ_BTN_HOST_X							MPJ_BTN_JOIN_X-180
#define		MPJ_BTN_HOST_Y							iScreenHeightOffset + 435
#define		MPJ_CANCEL_X							iScreenWidthOffset + ((320 - 115) / 2)

//textboxes
#define		MPJ_TXT_HANDLE_X						iScreenWidthOffset + 100
#define		MPJ_TXT_HANDLE_Y						iScreenHeightOffset + 60
#define		MPJ_TXT_HANDLE_WIDTH					120
#define		MPJ_TXT_HANDLE_HEIGHT					17
#define		MPJ_TXT_IP_X							iScreenWidthOffset + 100
#define		MPJ_TXT_IP_Y							iScreenHeightOffset + 400
#define		MPJ_TXT_IP_WIDTH						100
#define		MPJ_TXT_IP_HEIGHT						17
#define		MPJ_TXT_PORT_X							MPJ_TXT_IP_X + MPJ_TXT_IP_WIDTH + 40
#define		MPJ_TXT_PORT_Y							iScreenHeightOffset + 400
#define		MPJ_TXT_PORT_WIDTH						40
#define		MPJ_TXT_PORT_HEIGHT						17


//main title
#define		MPJ_MAIN_TITLE_X								0
#define		MPJ_MAIN_TITLE_Y						iScreenHeightOffset + 10
#define		MPJ_MAIN_TITLE_WIDTH					SCREEN_WIDTH

//labels
#define		MPJ_LABEL_HANDLE_X						MPJ_TXT_HANDLE_X - 80
#define		MPJ_LABEL_HANDLE_Y						MPJ_TXT_HANDLE_Y + 3
#define		MPJ_LABEL_HANDLE_WIDTH					80
#define		MPJ_LABEL_HANDLE_HEIGHT					17
#define		MPJ_LABEL_IP_X							MPJ_TXT_IP_X - 80
#define		MPJ_LABEL_IP_Y							MPJ_TXT_IP_Y + 3
#define		MPJ_LABEL_IP_WIDTH						80
#define		MPJ_LABEL_IP_HEIGHT						17
#define		MPJ_LABEL_PORT_X						MPJ_TXT_PORT_X - 30
#define		MPJ_LABEL_PORT_Y						MPJ_TXT_PORT_Y + 3
#define		MPJ_LABEL_PORT_WIDTH					80
#define		MPJ_LABEL_PORT_HEIGHT					17


////////////////////////////////////////////
//
//	Global Variables
//
///////////////////////////////////////////

BOOLEAN		gfMPJScreenEntry = TRUE;
BOOLEAN		gfMPJScreenExit	= FALSE;
BOOLEAN		gfReRenderMPJScreen=TRUE;
BOOLEAN		gfMPJButtonsAllocated = FALSE;

//enum for different states of screen
enum
{
	MPJ_NOTHING,
	MPJ_CANCEL,
	MPJ_EXIT,
	MPJ_HOST,
	MPJ_JOIN
};

UINT8		gubMPJScreenHandler=MPJ_NOTHING;	// State changer for HandleMPJScreen()

UINT32		gubMPJExitScreen = MP_JOIN_SCREEN;	// The screen that is in control next iteration of the game_loop

UINT32		guiMPJMainBackGroundImage;

// Wide-char strings that will hold the variables until they are transferred to the CHAR ascii fields
CHAR16		gzPlayerHandleField[ 30 ] = {0} ;
CHAR16		gzServerIPField[ 15 ] = {0} ;
CHAR16		gzServerPortField[ 5 ] = {0} ;

////////////////////////////////////////////
//
//	Screen Controls
//
///////////////////////////////////////////

// Join Button
void BtnMPJoinCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiMPJoinButton;
INT32		giMPJoinBtnImage;

// Host Button
void	BtnMPJHostCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiMPJHostButton;
INT32	giMPJHostBtnImage;

// Cancel Button
void	BtnMPJCancelCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiMPJCancelButton;
INT32	giMPJCancelBtnImage;

// Message Box handle
INT8	giMPJMessageBox = -1;


////////////////////////////////////////////
//
//	Local Function Prototypes
//
///////////////////////////////////////////

extern void ClearMainMenu();

BOOLEAN		EnterMPJScreen();
BOOLEAN		ExitMPJScreen();
void		HandleMPJScreen();
BOOLEAN		RenderMPJScreen();
void		GetMPJScreenUserInput();
void		SaveJoinSettings();
bool		ValidateJoinSettings(bool bSkipServerAddress);
BOOLEAN DoMPJMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback );
void			DoneFadeOutForExitMPJScreen( void );
void			DoneFadeInForExitMPJScreen( void );


UINT32	MPJoinScreenInit( void )
{
	// read settings from JA2 ini
	GetPrivateProfileStringW( L"Ja2_mp Settings",L"SERVER_IP", L"", gzServerIPField, 15, L"..\\Ja2_mp.ini" );
	GetPrivateProfileStringW( L"Ja2_mp Settings",L"SERVER_PORT", L"", gzServerPortField, 5, L"..\\Ja2_mp.ini" );
	GetPrivateProfileStringW( L"Ja2_mp Settings",L"CLIENT_NAME", L"Fresh Meat", gzPlayerHandleField, 30 , L"..\\Ja2_mp.ini" );
	return( 1 );
}

void		SaveJoinSettings()
{
	Get16BitStringFromField( 0, gzPlayerHandleField ); // these indexes are based on the order created
	Get16BitStringFromField( 1, gzServerIPField );
	Get16BitStringFromField( 2, gzServerPortField );

	// save settings to JA2_mp.ini
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"SERVER_IP",  gzServerIPField, L"..\\Ja2_mp.ini" );
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"SERVER_PORT", gzServerPortField, L"..\\Ja2_mp.ini" );
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"CLIENT_NAME", gzPlayerHandleField , L"..\\Ja2_mp.ini" );
}

bool	ValidateJoinSettings(bool bSkipServerAddress)
{
	// Check a Server name is entered
	Get16BitStringFromField( 0, gzPlayerHandleField ); // these indexes are based on the order created
	if (wcscmp(gzPlayerHandleField,L"")<=0)
	{
		DoMPJMessageBox( MSG_BOX_BASIC_STYLE, gzMPJScreenText[MPJ_HANDLE_INVALID], MP_JOIN_SCREEN, MSG_BOX_FLAG_OK, NULL );
		return false;
	}

	// dont check server address if we are going to HOST
	if (bSkipServerAddress)
		return true;

	// Verify the IP Address
	Get16BitStringFromField( 1, gzServerIPField );

	// loop through octets and check
	int numOctets = 0;
	wchar_t* tok;
	tok = wcstok(gzServerIPField,L".");
	while (tok != NULL)
	{
		numOctets++;
		INT32 oct = _wtoi(tok);
		// check for invalid conversion, ie alpha chars
		// wtoi returns 0 if it cant convert, but we need this value
		// therefore if tok <> 0 then it was a bad convert.
		if (oct == 0 && wcscmp(tok,L"0") != 0)
		{
			// force error
			numOctets=0;
			break;
		}

		if (oct < 0 || oct > 254) // dont allow broadcast nums
		{
			// bad octet, error
			numOctets=0;
			break;
		}

		// get next octet
		tok = wcstok(NULL,L".");
	}

	if (numOctets != 4)
	{
		// not a valid ip address
		DoMPJMessageBox( MSG_BOX_BASIC_STYLE, gzMPJScreenText[MPJ_SERVERIP_INVALID], MP_JOIN_SCREEN, MSG_BOX_FLAG_OK, NULL );
		return false;
	}
	
	// Verify the Server Port
	Get16BitStringFromField( 2, gzServerPortField );
	INT32 svrPort = _wtoi(gzServerPortField);
	if (svrPort < 1 || svrPort > 65535)
	{
		DoMPJMessageBox( MSG_BOX_BASIC_STYLE, gzMPJScreenText[MPJ_SERVERPORT_INVALID], MP_JOIN_SCREEN, MSG_BOX_FLAG_OK, NULL );
		return false;
	}

	return true;
}

UINT32	MPJoinScreenHandle( void )
{
	StartFrameBufferRender();

	if( gfMPJScreenEntry )
	{
//		PauseGame();

		EnterMPJScreen();
		gfMPJScreenEntry = FALSE;
		gfMPJScreenExit = FALSE;
		InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	}

	GetMPJScreenUserInput();


	HandleMPJScreen();

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
		return( gubMPJExitScreen );
	}

	if ( HandleBeginFadeOut( gubMPJExitScreen ) )
	{
		return( gubMPJExitScreen );
	}

	if ( HandleFadeInCallback( ) )
	{
		// Re-render the scene!
		RenderMPJScreen();
	}

	if ( HandleBeginFadeIn( gubMPJExitScreen ) )
	{

	}

	if( gfMPJScreenExit ) // we are exiting this screen
	{
		ExitMPJScreen(); // perform destruction
	}

	return( gubMPJExitScreen );
} // end MPJoinScreenHandle()


UINT32	MPJoinScreenShutdown( void )
{
	return( 1 );
}

BOOLEAN		EnterMPJScreen()
{
	VOBJECT_DESC	VObjectDesc;
	UINT16					cnt;
	UINT16					usPosY;

	if( gfMPJButtonsAllocated )
		return( TRUE );

	SetCurrentCursorFromDatabase( CURSOR_NORMAL );

	// load the Main trade screen backgroiund image
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

	if (iResolution == 0)
	{
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround.sti", VObjectDesc.ImageFile);
	}
	else if (iResolution == 1)
	{
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround_800x600.sti", VObjectDesc.ImageFile);
	}
	else if (iResolution == 2)
	{
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround_1024x768.sti", VObjectDesc.ImageFile);
	}

	CHECKF(AddVideoObject(&VObjectDesc, &guiMPJMainBackGroundImage ));

	//Join button
	giMPJoinBtnImage = LoadButtonImage("INTERFACE\\PreferencesButtons.sti", -1,0,-1,2,-1 );
	guiMPJoinButton = CreateIconAndTextButton( giMPJoinBtnImage, gzMPJScreenText[MPJ_JOIN_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MPJ_BTN_JOIN_X, MPJ_BTN_JOIN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPJoinCallback);

	SpecifyButtonSoundScheme( guiMPJoinButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyDisabledButtonStyle( guiMPJoinButton, DISABLED_STYLE_NONE );

	//Host button
	giMPJHostBtnImage = UseLoadedButtonImage( giMPJoinBtnImage, -1,1,-1,3,-1 );
	guiMPJHostButton = CreateIconAndTextButton( giMPJHostBtnImage, gzMPJScreenText[MPJ_HOST_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MPJ_BTN_HOST_X, MPJ_BTN_HOST_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPJHostCallback);

	SpecifyButtonSoundScheme( guiMPJHostButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyDisabledButtonStyle( guiMPJHostButton, DISABLED_STYLE_NONE );

	//Cancel button
	giMPJCancelBtnImage = UseLoadedButtonImage( giMPJoinBtnImage, -1,1,-1,3,-1 );
	guiMPJCancelButton = CreateIconAndTextButton( giMPJCancelBtnImage, gzMPJScreenText[MPJ_CANCEL_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MPJ_CANCEL_X, MPJ_BTN_JOIN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPJCancelCallback );
	SpecifyButtonSoundScheme( guiMPJCancelButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );

	// Initialise Text Boxes
	InitTextInputMode(); // API call to initialise text input mode for this screen
						 // does not mean we are inputting text right away

	// Player Name field
	SetTextInputCursor( CUROSR_IBEAM_WHITE );
	SetTextInputFont( (UINT16) FONT12ARIALFIXEDWIDTH ); //FONT12ARIAL //FONT12ARIALFIXEDWIDTH
	Set16BPPTextFieldColor( Get16BPPColor(FROMRGB( 0, 0, 0) ) );
	SetBevelColors( Get16BPPColor(FROMRGB(136, 138, 135)), Get16BPPColor(FROMRGB(24, 61, 81)) );
	SetTextInputRegularColors( FONT_WHITE, 2 );
	SetTextInputHilitedColors( 2, FONT_WHITE, FONT_WHITE	);
	SetCursorColor( Get16BPPColor(FROMRGB(255, 255, 255) ) );

	//AddUserInputField( NULL ); // API Call that sets a special input-handling routine and method for the TAB key

	//Add Player Name textbox 
	AddTextInputField(	MPJ_TXT_HANDLE_X,
						MPJ_TXT_HANDLE_Y, 
						MPJ_TXT_HANDLE_WIDTH,
						MPJ_TXT_HANDLE_HEIGHT,
						MSYS_PRIORITY_HIGH+2,
						gzPlayerHandleField,
						30,
						INPUTTYPE_ASCII );//23

	//Add Server IP textbox 
	AddTextInputField(	MPJ_TXT_IP_X,
						MPJ_TXT_IP_Y, 
						MPJ_TXT_IP_WIDTH,
						MPJ_TXT_IP_HEIGHT,
						MSYS_PRIORITY_HIGH+2,
						gzServerIPField,
						15,
						INPUTTYPE_ASCII );//23

	//Add Server Port textbox 
	AddTextInputField(	MPJ_TXT_PORT_X,
						MPJ_TXT_PORT_Y, 
						MPJ_TXT_PORT_WIDTH,
						MPJ_TXT_PORT_HEIGHT,
						MSYS_PRIORITY_HIGH+2,
						gzServerPortField,
						5,
						INPUTTYPE_ASCII );//23

	SetActiveField( 0 ); // Playername textbox has focus

	//Reset the exit screen - screen the main game loop will call next iteration
	gubMPJExitScreen = MP_JOIN_SCREEN;

	//REnder the screen once so we can blt ot to ths save buffer
	RenderMPJScreen();

	BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	gfMPJButtonsAllocated = TRUE;

	return( TRUE );

} // End of EnterMPJScreen()


BOOLEAN		ExitMPJScreen()
{
	UINT16	cnt;

	if( !gfMPJButtonsAllocated )
		return( TRUE );

	//Delete the main options screen background
	DeleteVideoObjectFromIndex( guiMPJMainBackGroundImage );

	RemoveButton( guiMPJoinButton );
	RemoveButton( guiMPJHostButton );
	RemoveButton( guiMPJCancelButton );


	UnloadButtonImage( giMPJCancelBtnImage );
	UnloadButtonImage( giMPJHostBtnImage );
	UnloadButtonImage( giMPJoinBtnImage );

	// exit text input mode in this screen and clean up text boxes
	KillAllTextInputModes();
	SetTextInputCursor( CURSOR_IBEAM );

	gfMPJButtonsAllocated = FALSE;

	//If we are starting the game stop playing the music
	// <TODO> review this, i think MPJ_EXIT is the proceed mode...
	//if( gubMPJScreenHandler == MPJ_EXIT )
	//	SetMusicMode( MUSIC_NONE );

	gfMPJScreenExit = FALSE;
	gfMPJScreenEntry = TRUE;

	return( TRUE );

} // End of ExitMPJScreen()


void			HandleMPJScreen()
{
	if( gubMPJScreenHandler != MPJ_NOTHING )
	{
		switch( gubMPJScreenHandler )
		{
			case MPJ_CANCEL:
				gubMPJExitScreen = MAINMENU_SCREEN;
				gfMPJScreenExit	= TRUE;
				break;
			
			case MPJ_HOST:
				gubMPJExitScreen = MP_HOST_SCREEN;
				gfMPJScreenExit	= TRUE;
				break;


			case MPJ_JOIN:
			{
				//if we are already fading out, get out of here
				if( gFadeOutDoneCallback != DoneFadeOutForExitMPJScreen )
				{
					//Disable the ok button
					DisableButton( guiMPJoinButton );
					DisableButton( guiMPJHostButton );

					gFadeOutDoneCallback = DoneFadeOutForExitMPJScreen;

					FadeOutNextFrame( );
				}
				break;
			}

		}

		gubMPJScreenHandler = MPJ_NOTHING;
	}


	if( gfReRenderMPJScreen )
	{
		RenderMPJScreen();
		gfReRenderMPJScreen = FALSE;
	}

	// <TODO> add restore backgrounds in...
	//RestoreGIOButtonBackGrounds();
} // end of HandleMPJScreen


BOOLEAN		RenderMPJScreen()
{
	HVOBJECT	hPixHandle;
	UINT16		usPosY;

	//Get the main background screen graphic and blt it
	GetVideoObject(&hPixHandle, guiMPJMainBackGroundImage );
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,0,0, VO_BLT_SRCTRANSPARENCY,NULL);

	//Shade the background
	ShadowVideoSurfaceRect( FRAME_BUFFER, iScreenWidthOffset, iScreenHeightOffset, iScreenWidthOffset + 640, iScreenHeightOffset + 480 );

	//Display the title
	DrawTextToScreen( gzMPJScreenText[ MPJ_TITLE_TEXT ], MPJ_MAIN_TITLE_X, MPJ_MAIN_TITLE_Y, MPJ_MAIN_TITLE_WIDTH, MPJ_TITLE_FONT, MPJ_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	
	// Player name text label
	DisplayWrappedString( MPJ_LABEL_HANDLE_X, MPJ_LABEL_HANDLE_Y, MPJ_LABEL_HANDLE_WIDTH, 2, MPJ_LABEL_TEXT_FONT, MPJ_LABEL_TEXT_COLOR, gzMPJScreenText[ MPJ_HANDLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Server IP text label
	DisplayWrappedString( MPJ_LABEL_IP_X, MPJ_LABEL_IP_Y, MPJ_LABEL_IP_WIDTH, 2, MPJ_LABEL_TEXT_FONT, MPJ_LABEL_TEXT_COLOR, gzMPJScreenText[ MPJ_SERVERIP_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Server Port text label
	DisplayWrappedString( MPJ_LABEL_PORT_X, MPJ_LABEL_PORT_Y, MPJ_LABEL_PORT_WIDTH, 2, MPJ_LABEL_TEXT_FONT, MPJ_LABEL_TEXT_COLOR, gzMPJScreenText[ MPJ_SERVERPORT_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	return( TRUE );
} // end of RenderMPJScreen()



void			GetMPJScreenUserInput()
{
	InputAtom Event;
//	POINT	MousePos;

//	GetCursorPos(&MousePos);

	while( DequeueEvent( &Event ) )
	{
		// check if this event is swallowed by text input, otherwise process key
		if( !HandleTextInput( &Event ) && Event.usEvent == KEY_DOWN )
		{
			switch( Event.usParam )
			{

				case ESC:
					//Exit out of the screen
					gubMPJScreenHandler = MPJ_CANCEL;
					break;

				case ENTER:
					if (ValidateJoinSettings(false))
					{
						SaveJoinSettings();
						gubMPJScreenHandler = MPJ_JOIN;
					}
					break;
			}
		}
	}
} // end of GetMPJScreenUserInput()


// CALLBACKS

void BtnMPJoinCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		if (ValidateJoinSettings(false))
		{
			SaveJoinSettings();
			gubMPJScreenHandler = MPJ_JOIN;
			//gubMPJScreenHandler = MPJ_JOIN;
		}

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnMPJHostCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		if (ValidateJoinSettings(true))
		{
			SaveJoinSettings();
			gubMPJScreenHandler = MPJ_HOST;
		}
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnMPJCancelCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gubMPJScreenHandler = MPJ_CANCEL;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

BOOLEAN DoMPJMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect CenteringRect= {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 };

	// reset exit mode
//	gfExitGioDueToMessageBox = TRUE;

	// do message box and return
	giMPJMessageBox = DoMessageBox(	ubStyle,	zString,	uiExitScreen, ( UINT16 ) ( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),	ReturnCallback,	&CenteringRect );

	// send back return state
	return( ( giMPJMessageBox != -1 ) );
}

void DoneFadeOutForExitMPJScreen( void )
{
	// As we bypassed the GIO screen, set up some game options for multiplayer here
	// <TODO> most things i have left as thier defaults here for testing.
	is_networked = true;
	is_host = false; // we want to be a client, not we ARE a client yet (is_client)
	auto_retry = true;
	giNumTries = 5;

	// loop through and get the status of all the buttons
	// Madd
	/*gGameOptions.fGunNut = GetCurrentGunButtonSetting();
	gGameOptions.ubGameStyle = GetCurrentGameStyleButtonSetting();
	gGameOptions.ubDifficultyLevel = GetCurrentDifficultyButtonSetting() + 1;*/
	// JA2Gold: no more timed turns setting
	//gGameOptions.fTurnTimeLimit = GetCurrentTimedTurnsButtonSetting();//hayden : re-enabled

	if (is_networked)
		gGameOptions.fTurnTimeLimit = TRUE;
	else
		gGameOptions.fTurnTimeLimit = FALSE;
	
	// JA2Gold: iron man
	//gGameOptions.fIronManMode = GetCurrentGameSaveButtonSetting();

	// Bobby Rays - why would we want anything less than the best
	gGameOptions.ubBobbyRay = BR_AWESOME;
	

	// CHRISL:
	/*if(IsNIVModeValid() == TRUE){
		switch ( GetCurrentINVOptionButtonSetting() )
		{
			case GIO_INV_OLD:
				gGameOptions.ubInventorySystem = INVENTORY_OLD;
				break;
			case GIO_INV_NEW:
				gGameOptions.ubInventorySystem = INVENTORY_NEW;
				break;
		}
	}*/

	//	gubGIOExitScreen = INIT_SCREEN;
	gubMPJExitScreen = INTRO_SCREEN;

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
		SetIntroType( INTRO_BEGINING );

	ExitMPJScreen(); // cleanup please, if we called a fadeout then we didnt do it above

	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

void DoneFadeInForExitMPJScreen( void )
{
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}