#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include "Types.h"
	#include "MPConnectScreen.h"
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
	#include "Text.h"
	#include "Text Input.h"
	#include "Soldier Profile.h"
	#include "Animated ProgressBar.h"
	#include "mainmenuscreen.h"
#endif

#include "gameloop.h"
#include "Game Init.h"
#include "connect.h"
#include "network.h" // for client name
#include "message.h"
#include "init.h"
#include "xml.h"


////////////////////////////////////////////
//
//	Global Defines
//
///////////////////////////////////////////

#define		MPC_TITLE_FONT							FONT14ARIAL//FONT16ARIAL
#define		MPC_TITLE_COLOR									FONT_MCOLOR_WHITE

#define		MPC_SUBHEADING_FONT					FONT12ARIAL//FONT16ARIAL
#define		MPC_SUBHEADING_COLOR							FONT_MCOLOR_WHITE

//buttons
#define		MPC_BTN_CANCEL_X						iScreenWidthOffset + ((320 - 115) / 2)
#define		MPC_BTN_CANCEL_Y						iScreenHeightOffset + SCREEN_HEIGHT - 50

//main title
#define		MPC_MAIN_TITLE_X						0
#define		MPC_MAIN_TITLE_Y						iScreenHeightOffset + 10
#define		MPC_MAIN_TITLE_WIDTH					SCREEN_WIDTH

//subtitle
#define		MPC_SUBHEADING_X						0
#define		MPC_SUBHEADING_Y						MPC_MAIN_TITLE_Y + 30
#define		MPC_SUBHEADING_WIDTH					SCREEN_WIDTH

//help
#define		MPC_HELP_X								0
#define		MPC_HELP_Y								MPC_SUBHEADING_Y + 30
#define		MPC_HELP_WIDTH							SCREEN_WIDTH


////////////////////////////////////////////
//
//	Global Variables
//
///////////////////////////////////////////

BOOLEAN		gfMPCScreenEntry = TRUE;
BOOLEAN		gfMPCScreenExit	= FALSE;
BOOLEAN		gfReRenderMPCScreen=TRUE;
BOOLEAN		gfMPCButtonsAllocated = FALSE;
BOOLEAN		gfMPConnectScreenCanContinue = FALSE;

//enum for different states of screen
enum
{
	MPC_NOTHING,
	MPC_CANCEL,
	MPC_CONTINUE,
};

UINT8		gubMPCScreenHandler=MPC_NOTHING;	// State changer for HandleMPCNScreen()

UINT32		gubMPCExitScreen = MP_CONNECT_SCREEN;	// The screen that is in control next iteration of the game_loop

UINT8		loadscreenID = 0;

////////////////////////////////////////////
//
//	Screen Controls
//
///////////////////////////////////////////

// Cancel Button
void BtnMPCCancelCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiMPCCancelButton;
INT32		giMPCCancelBtnImage;

UINT32		guiMPCMainBackGroundImage;

// Message Box handle
INT8	giMPCMessageBox = -1;

// connect screen message
CHAR16	gszConnectHeadingMsg[512];
CHAR16	gszConnectSubMsg[512];

////////////////////////////////////////////
//
//	Local Function Prototypes
//
///////////////////////////////////////////

extern void ClearMainMenu();

BOOLEAN		EnterMPCScreen();
BOOLEAN		ExitMPCScreen();
void		HandleMPCScreen();
BOOLEAN		RenderMPCScreen();

void		GetMPCScreenUserInput();
void			DoneFadeOutForExitMPCScreen( void );
void			DoneFadeInForExitMPCScreen( void );
void		CheckForContinue();


UINT32	MPConnectScreenInit( void )
{
	return (1);
}

UINT32	MPConnectScreenHandle( void )
{
	StartFrameBufferRender();

	RestoreBackgroundRects( );

	if( gfMPCScreenEntry )
	{
//		PauseGame();

		EnterMPCScreen();
		gfMPCScreenEntry = FALSE;
		gfMPCScreenExit = FALSE;
		InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	}

    // add scrolling string log
	ScrollString( );


	GetMPCScreenUserInput();

	// connect to the server
	NetworkAutoStart();

	// Check if we need to leave this screen
	CheckForContinue();

	HandleMPCScreen();

	// draw chat texts
	ExecuteVideoOverlays( );

	SaveBackgroundRects();
	//InvalidateScreen();

	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();

	// handle fades in and out
	if ( HandleFadeOutCallback( ) )
	{
		ClearMainMenu();
		return( gubMPCExitScreen );
	}

	if ( HandleBeginFadeOut( gubMPCExitScreen ) )
	{
		return( gubMPCExitScreen );
	}

	if ( HandleFadeInCallback( ) )
	{
		// Re-render the scene!
		RenderMPCScreen();
	}

	if ( HandleBeginFadeIn( gubMPCExitScreen ) )
	{

	}

	if( gfMPCScreenExit ) // we are exiting this screen
	{
		ExitMPCScreen(); // perform destruction
	}

	return( gubMPCExitScreen );
} // end MPScoreScreenHandle()

UINT32	MPConnectScreenShutdown( void )
{
	return( 1 );
}

BOOLEAN		EnterMPCScreen()
{
	// make sure only chat messages show up on connect screen
	ClearTacticalMessageQueue( );
	SetTacticalMessageFilter( MSG_CHAT );

	gfMPConnectScreenCanContinue = FALSE;

	VOBJECT_DESC	VObjectDesc;

	if( gfMPCButtonsAllocated )
		return( TRUE );

	SetCurrentCursorFromDatabase( CURSOR_NORMAL );

		// load the Main trade screen backgroiund image
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	if (iResolution >= _640x480 && iResolution < _800x600)
		FilenameForBPP("INTERFACE\\MPConnect.sti",VObjectDesc.ImageFile);
	else if (iResolution < _1024x768)
		FilenameForBPP("INTERFACE\\MPConnect_800x600.sti",VObjectDesc.ImageFile);
	else
		FilenameForBPP("INTERFACE\\MPConnect_1024x768.sti", VObjectDesc.ImageFile);


	CHECKF(AddVideoObject(&VObjectDesc, &guiMPCMainBackGroundImage ));


	CreateLoadingScreenProgressBar(TRUE);
	// default to red connecting color
	SetProgressBarColor(0,150,0,0);
	SetRelativeStartAndEndPercentage(0,0,100,L"");

	//Cancel button
	/*giMPCCancelBtnImage = LoadButtonImage("INTERFACE\\PreferencesButtons.sti", -1,0,-1,2,-1 );
	guiMPCCancelButton = CreateIconAndTextButton( giMPCCancelBtnImage, gzMPCScreenText[MPC_CANCEL_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MPC_BTN_CANCEL_X, MPC_BTN_CANCEL_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPCCancelCallback);

	SpecifyButtonSoundScheme( guiMPCCancelButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyDisabledButtonStyle( guiMPCCancelButton, DISABLED_STYLE_NONE );*/

	//Reset the exit screen - screen the main game loop will call next iteration
	gubMPCExitScreen = MP_CONNECT_SCREEN;

	//REnder the screen once so we can blt ot to ths save buffer
	RenderMPCScreen();

	BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	gfMPCButtonsAllocated = TRUE;

	return( TRUE );

} // End of EnterMPSScreen()


BOOLEAN		ExitMPCScreen()
{
	RemoveTacticalMessageFilter();

	if( !gfMPCButtonsAllocated )
		return( TRUE );

	//Delete the main options screen background
	DeleteVideoObjectFromIndex( guiMPCMainBackGroundImage );
	RemoveProgressBar(0);

	//RemoveButton( guiMPCCancelButton );
	//UnloadButtonImage( giMPCCancelBtnImage );

	// exit text input mode in this screen and clean up text boxes
	//KillAllTextInputModes();
	SetTextInputCursor( CURSOR_IBEAM );

	gfMPCButtonsAllocated = FALSE;

	//If we are starting the game stop playing the music
	if( gubMPCScreenHandler == MPC_CONTINUE )
		SetMusicMode( MUSIC_NONE );

	gfMPCScreenExit = FALSE;
	gfMPCScreenEntry = TRUE;

	return( TRUE );

} // End of ExitMPJScreen()


void			HandleMPCScreen()
{
	if( gubMPCScreenHandler != MPC_NOTHING )
	{
		switch( gubMPCScreenHandler )
		{
			case MPC_CANCEL:
				gubMPCExitScreen = MP_JOIN_SCREEN;
				gfMPCScreenExit	= TRUE;

				// disconnect the client
				client_disconnect();
				break;
			
			case MPC_CONTINUE:
			{
				//if we are already fading out, get out of here
				if( gFadeOutDoneCallback != DoneFadeOutForExitMPCScreen )
				{
					//Disable the ok button
					DisableButton( guiMPCCancelButton );

					gFadeOutDoneCallback = DoneFadeOutForExitMPCScreen;

					FadeOutNextFrame( );
				}
				break;
			}

		}

		gubMPCScreenHandler = MPC_NOTHING;
	}

	if( gfReRenderMPCScreen )
	{
		RenderMPCScreen();
		gfReRenderMPCScreen = FALSE;
	}

} // end of HandleMPCScreen


BOOLEAN		RenderMPCScreen()
{
	HVOBJECT	hPixHandle;

	//Get the main background screen graphic and blt it
	GetVideoObject(&hPixHandle, guiMPCMainBackGroundImage );
	
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,(SCREEN_WIDTH - xResSize)/2,(SCREEN_HEIGHT - yResSize)/2, VO_BLT_SRCTRANSPARENCY,NULL);

	//Shade the background
	//ShadowVideoSurfaceRect( FRAME_BUFFER, iScreenWidthOffset, iScreenHeightOffset, iScreenWidthOffset + 640, iScreenHeightOffset + 480 );

	UINT8 iProgress = 0;	

	if (is_connecting)
	{
		iProgress = (UINT8)(100 * (float)((MAX_CONNECT_RETRIES-(float)giNumTries)/MAX_CONNECT_RETRIES));
		SetProgressBarColor(0,150,0,0);
		
		SetConnectScreenHeadingW(gzMPCScreenText[ MPC_CONNECTING_TEXT ]);
	}
	else if (is_connected)
	{
		if (!recieved_settings && !recieved_transfer_settings)
		{
			//Display the title
			SetConnectScreenHeadingW( gzMPCScreenText[ MPC_GETSETTINGS_TEXT ]);
		}
		else
		{
			if (serverSyncClientsDirectory==1)
			{
				SetConnectScreenHeadingW( gzMPCScreenText[ MPC_DOWNLOADING_TEXT ]);
				SetProgressBarColor(0,0,0,150);
				iProgress = client_progress[CLIENT_NUM-1];
			}
			else
			{
				SetConnectScreenHeadingW( gzMPCScreenText[ MPC_READY_TEXT ] );
			}
		}
	}

	//Display the title
	DrawTextToScreen( gszConnectHeadingMsg, MPC_MAIN_TITLE_X, MPC_MAIN_TITLE_Y, MPC_MAIN_TITLE_WIDTH, MPC_TITLE_FONT, MPC_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	InvalidateRegion(MPC_MAIN_TITLE_X,MPC_MAIN_TITLE_Y,MPC_MAIN_TITLE_WIDTH,MPC_MAIN_TITLE_Y+20);
	//Display the subheading
	DrawTextToScreen( gszConnectSubMsg, MPC_SUBHEADING_X, MPC_SUBHEADING_Y, MPC_SUBHEADING_WIDTH, MPC_SUBHEADING_FONT, MPC_SUBHEADING_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	InvalidateRegion(MPC_SUBHEADING_X,MPC_SUBHEADING_Y,MPC_MAIN_TITLE_WIDTH,MPC_SUBHEADING_Y+20);
	//Display the help text
	DrawTextToScreen( (is_connected && recieved_settings ? gzMPCScreenText[ MPC_HELP1_TEXT ] : gzMPCScreenText[MPC_HELP2_TEXT] ), MPC_HELP_X, MPC_HELP_Y, MPC_HELP_WIDTH, MPC_SUBHEADING_FONT, MPC_SUBHEADING_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	InvalidateRegion(MPC_HELP_X,MPC_HELP_Y,MPC_HELP_WIDTH,MPC_HELP_Y+20);

	SetProgressBarRenderBuffer(0,FRAME_BUFFER);
	RenderProgressBar(0,iProgress);
	
	return( TRUE );
} // end of RenderMPCScreen()



void			GetMPCScreenUserInput()
{
	InputAtom Event;
//	POINT	MousePos;

//	GetCursorPos(&MousePos);

	while (DequeueSpecificEvent(&Event, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
		// check if this event is swallowed by text input, otherwise process key
		if( Event.usEvent == KEY_DOWN )
		{
			switch( Event.usParam )
			{

				case ESC:
					//Exit out of the screen
					gubMPCScreenHandler = MPC_CANCEL;
					break;
				case 'y':
					if (is_connected && recieved_settings)
						OpenChatMsgBox();
					break;
			}
		}
	}
} // end of GetMPCScreenUserInput()

void DoneFadeOutForExitMPCScreen( void )
{
	// ------------------------------------------
	// Now Initialize the client again with all the files from the "_MULTIPLAYER" profile!!!	
	LoadExternalGameplayData(TABLEDATA_DIRECTORY, true);
	LoadAllExternalText();
	InitDependingGameStyleOptions();
	InitNewGame(false);
	
	// Overwrite some client settings with values from the server (cash, ...)
	reapplySETTINGS();
	// ------------------------------------------
	
	gubMPCExitScreen = MAP_SCREEN;
	ExitMPCScreen(); // cleanup please, if we called a fadeout then we didnt do it above
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

void DoneFadeInForExitMPCScreen( void )
{
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

void BtnMPCCancelCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gubMPCScreenHandler = MPC_CANCEL;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}

}

void	SetConnectScreenHeadingW( STR16 cmsg )
{
	wcscpy(gszConnectHeadingMsg,cmsg);
	gfReRenderMPCScreen=TRUE;
}

void	SetConnectScreenHeadingA( const char* cmsg )
{
	CHAR16 converted[512];
	MultiByteToWideChar( CP_UTF8, 0, cmsg, -1, (LPWSTR)converted, 512);
	SetConnectScreenHeadingW( converted );
}

void	SetConnectScreenSubMessageW( STR16 cmsg )
{
	wcscpy(gszConnectSubMsg,cmsg);
	gfReRenderMPCScreen=TRUE;
}

void	SetConnectScreenSubMessageA( const char* cmsg )
{
	CHAR16 converted[512];
	MultiByteToWideChar( CP_UTF8, 0, cmsg, -1, (LPWSTR)converted, 512);
	SetConnectScreenSubMessageW( converted );
}

void CheckForContinue()
{
	if (is_connected && is_client && recieved_settings)
	{
		if (serverSyncClientsDirectory == 1)
		{
			if (fClientReceivedAllFiles)
				gubMPCScreenHandler = MPC_CONTINUE;
		}
		else
		{
			gubMPCScreenHandler = MPC_CONTINUE;
		}
	}
}

#include <vfs/Core/vfs_os_functions.h>
#include <vfs/Core/vfs_profile.h>
#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_init.h>
#include <memory>

void ja2::mp::InitializeMultiplayerProfile(vfs::Path const& profileRoot)
{
	// does the profile root directory actually exist
	if(!vfs::OS::checkRealDirectory(profileRoot))
	{
		// OK, directory did not exist, probably a new profile
		SGP_THROW_IFFALSE( vfs::OS::createRealDirectory(profileRoot), 
			_BS(L"Could not create directory : ") << profileRoot << _BS::wget );
	}

	// create and initialize a new Multiplayer profile
	std::unique_ptr<vfs::CVirtualProfile> pProf( new vfs::CVirtualProfile("_MULTIPLAYER",profileRoot,true) );

	if(!vfs_init::initWriteProfile(*pProf))
	{
		SGP_THROW(_BS(L"Could not initialize client profile") << profileRoot << _BS::wget);
	}

	vfs::CProfileStack *PS = getVFS()->getProfileStack();

	// remove old Multiplayer profile (if it exists)
	vfs::CVirtualProfile *pOldProf = PS->getProfile("_MULTIPLAYER");
	if( pOldProf && (pOldProf == PS->topProfile()) )
	{
		SGP_THROW_IFFALSE(PS->popProfile(), L"Could not remove old \"_MULTIPLAYER\" profile");
	}

	// add new profile
	PS->pushProfile(pProf.release());
}
