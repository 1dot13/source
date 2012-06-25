#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
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
	#include "Game Clock.h"
	#include "Map Screen Interface.h"
	#include "renderworld.h"
	#include "gameloop.h"
	#include "english.h"
	#include "GameSettings.h"
	#include "Interface Control.h"
	#include "cursor control.h"
	#include "laptop.h"
	#include "text.h"
	#include "Text Input.h"
	#include "overhead map.h"
	#include "MPChatScreen.h"
	#include "WordWrap.h"
	#include "Render Dirty.h"
#include "message.h"
#include "utilities.h"
#include "connect.h"
#endif

#define		CHATBOX_WIDTH							310 // 350 is the max size, the PrepareMercPopupBox will add the X_MARGIN to both sides
#define		CHATBOX_Y_MARGIN_NOLOG					25
#define		CHATBOX_Y_MARGIN_LOG					80
#define		CHATBOX_Y_MARGIN_BOTTOM					80
#define		CHATBOX_X_MARGIN						30 // pad box width 20 either side of text pixel length, but i use half of this value for control margins

#define		CHATBOX_LOG_WIDTH						300
#define		CHATBOX_LOG_HEIGHT						80
#define		CHATBOX_LOG_TXTBORDER					4  // offset till text renderable area on all sides

#define		CHATBOX_SCROLL_WIDTH					17
#define		CHATBOX_SCROLL_HEIGHT					80
#define		CHATBOX_SCROLL_GAPX						5

#define		CHATBOX_BUTTON_WIDTH					61
#define		CHATBOX_BUTTON_HEIGHT					20
#define		CHATBOX_BUTTON_X_SEP					15

#define		CHATBOX_SMALL_BUTTON_WIDTH				31
#define		CHATBOX_SMALL_BUTTON_X_SEP				8

#define		CHATBOX_TOGGLE_WIDTH					120
#define		CHATBOX_TOGGLE_HEIGHT					12

#define		CHATBOX_Y_GAP							10
#define		CHATBOX_SLIDER_GAP						5

#define		CHATBOX_INPUT_HEIGHT					20

#define		CHATBOX_FONT_COLOR						73
#define		CHATBOX_FONT_TITLE						FONT14ARIAL
#define		CHATBOX_FONT_TOGGLE						FONT10ARIAL

// old mouse x and y positions
extern SGPPoint pOldMousePosition;
SGPRect ChatBoxRestrictedCursorRegion;

// if the cursor was locked to a region
extern BOOLEAN fCursorLockedToArea;
BOOLEAN	gfInChatBox = FALSE;

//extern BOOLEAN fMapExitDueToMessageBox;
extern BOOLEAN fInMapMode;
extern BOOLEAN gfOverheadMapDirty;

//OJW - 20090208
CHAR16 gszChatBoxInputString[255];
BOOLEAN gbChatSendToAll = true;

void		OKChatBoxCallback(GUI_BUTTON *btn, INT32 reason );
void		CancelChatBoxCallback(GUI_BUTTON *btn, INT32 reason );
void		ChatBoxClickCallback( MOUSE_REGION * pRegion, INT32 iReason );
UINT32	ExitChatBox( INT8 ubExitCode );
UINT16	GetChatBoxButtonWidth( INT32 iButtonImage );

extern SGPRect gOldCursorLimitRectangle;


MESSAGE_BOX_STRUCT	gChatBox;
BOOLEAN							gfNewChatBox = FALSE;
extern BOOLEAN							gfStartedFromGameScreen;
extern BOOLEAN							gfStartedFromMapScreen;
BOOLEAN									fRestoreBackgroundForChatBox = FALSE;
extern BOOLEAN							gfDontOverRideSaveBuffer;	//this variable can be unset if ur in a non gamescreen and DONT want the msg box to use the save buffer


extern void HandleTacticalUILoseCursorFromOtherScreen( );
extern STR16 pUpdatePanelButtons[];


#define		NUM_CHAT_TOGGLES		2
INT32 guiChatToggles[ NUM_CHAT_TOGGLES ];
void BtnChatTogglesCallback(GUI_BUTTON *btn,INT32 reason);
void RestoreChatToggleBackGrounds();

bool gIncludeChatLog = false;
SGPRect gChatTextBoxRegion;

// OPTIONAL CHAT MESSAGE LOG SETUP
SGPRect gChatMessageLogRegion;

UINT32 guiCHATLOGIMG; // Images for chat message log

// button enums
enum{
	CHAT_SCROLL_MESSAGE_UP =0,
	CHAT_SCROLL_MESSAGE_DOWN,
};

UINT32 guiChatSliderBar;

UINT16 CHATLOG_SCROLL_AREA_START_Y;				//(SCREEN_HEIGHT - 90)	//390
UINT16 CHATLOG_SCROLL_AREA_END_Y;				//(SCREEN_HEIGHT - 32)		//448
UINT16 CHATLOG_SCROLL_AREA_HEIGHT;				//( CHATLOG_SCROLL_AREA_END_Y - CHATLOG_SCROLL_AREA_START_Y + 1 )

// CHRISL: Use these if we want scroll bar based on left edge of screen
UINT16 CHATLOG_SCROLL_AREA_START_X;				//330
UINT16 CHATLOG_SCROLL_AREA_END_X;				//344
UINT16 CHATLOG_SCROLL_AREA_WIDTH;				//( CHATLOG_SCROLL_AREA_END_X - CHATLOG_SCROLL_AREA_START_X + 1 )

#define CHATLOG_BTN_SCROLL_TIME					100

#define CHAT_SLIDER_HEIGHT							11
#define CHAT_SLIDER_WIDTH							11

#define MAX_CHATLOG_MESSAGES	8

UINT16  CHAT_SLIDER_BAR_RANGE;

// buttons
UINT32 guiChatLogScrollButtons[ 2 ];

// buttons images
UINT32 guiChatLogScrollButtonsImage[ 2 ];

// mouse regions
MOUSE_REGION gChatLogScrollBarRegion;

void LoadChatLogSliderBar( void );
void DeleteChatLogSliderBar( void );
void DisplayChatLogScrollBarSlider( );
void CreateChatLogMessageScrollBarRegion( void );
void DeleteChatLogMessageScrollRegion( void );

void ChatScreenMsgScrollDown( UINT8 ubLinesDown );
void ChatScreenMsgScrollUp( UINT8 ubLinesUp );
void ChangeCurrentChatScreenMessageIndex( UINT8 ubNewMessageIndex );
void MoveToEndOfChatScreenMessageList( void );

void BtnMessageDownChatLogCallback( GUI_BUTTON *btn,INT32 reason );
void BtnMessageUpChatLogCallback( GUI_BUTTON *btn,INT32 reason );
void ChatScreenMessageScrollBarCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void EnableDisableChatLogScrollButtonsAndRegions( void );

// the local logical message index
UINT8 gubFirstChatLogMessageIndex = 0;

// Chat Log Messages List
UINT8 gubStartOfChatLogMessageList = 0; // Front of the Message Queue
UINT8 gubEndOfChatLogMessageList = 0; // End of the Message Queue
UINT8 gubCurrentChatLogMessageString = 0; // // index of the current string we are looking at
static ScrollStringStPtr gChatLogMessageList[ 256 ]; // The message Queue

void InitChatMessageList( void );
void FreeChatMessageList( void );
void DisplayStringsInChatLogMessageList( void );
void AddStringToChatLogMessageList( STR16 pString, UINT16 usColor, UINT32 uiFont, BOOLEAN fStartOfNewString, UINT8 ubPriority );
UINT8 GetRangeOfChatLogMessages( void );

void ClearWrappedStringsCHAT( WRAPPED_STRING *pStringWrapperHead );

#define CHAT_LINE_WIDTH				292
#define CHAT_MESSAGE_FONT			TINYFONT1




INT32 DoChatBox( bool bIncludeChatLog, const STR16 zString, UINT32 uiExitScreen, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect )
{
	VSURFACE_DESC		vs_desc;
	UINT16	usTextBoxWidth;
	UINT16	usTextBoxHeight;
	UINT16	usYMargin;
	SGPRect	aRect;
	UINT32 uiDestPitchBYTES, uiSrcPitchBYTES;
	UINT8	*pDestBuf, *pSrcBuf;
	INT16	sButtonX, sButtonY;
	UINT8	ubMercBoxBackground = BASIC_MERC_POPUP_BACKGROUND, ubMercBoxBorder = BASIC_MERC_POPUP_BORDER;
	UINT8	ubFontColor, ubFontShadowColor;
	UINT16	usCursor;
	INT32 iId = -1;

	// clear the ouput string
	memset(gszChatBoxInputString,0,sizeof(CHAR16)*255);

	gIncludeChatLog = bIncludeChatLog;

	GetMousePos( &pOldMousePosition );

	if (bIncludeChatLog)
		usYMargin = CHATBOX_Y_MARGIN_LOG;
	else
		usYMargin = CHATBOX_Y_MARGIN_NOLOG;

	//this variable can be unset if ur in a non gamescreen and DONT want the msg box to use the save buffer
	gfDontOverRideSaveBuffer = TRUE;

	SetCurrentCursorFromDatabase( CURSOR_NORMAL );

	if( gChatBox.BackRegion.uiFlags & MSYS_REGION_EXISTS )
	{
		return( 0 );
	}

	// set style
	ubMercBoxBackground = DIALOG_MERC_POPUP_BACKGROUND;
	ubMercBoxBorder			= DIALOG_MERC_POPUP_BORDER;

	// Add button images
	gChatBox.iButtonImages			= LoadButtonImage( "INTERFACE\\popupbuttons.sti", -1,0,-1,1,-1 );
	ubFontColor	= CHATBOX_FONT_COLOR;
	ubFontShadowColor = DEFAULT_SHADOW;
	usCursor = CURSOR_NORMAL;

			


	// Use default!
	aRect.iTop		= 	0;
	aRect.iLeft		= 	0;
	aRect.iBottom = 	SCREEN_HEIGHT;
	aRect.iRight	= 	SCREEN_WIDTH;

	// Set some values!
	//gChatBox.usFlags			= usFlags;
	gChatBox.uiExitScreen	= uiExitScreen;
	gChatBox.ExitCallback	= ReturnCallback;
	gChatBox.fRenderBox		= TRUE;
	gChatBox.bHandled		= 0;

	// Init message box
	if (bIncludeChatLog)
		// we need a string just long enough to give 1 line, but max length of the box, we render the chatlog over this string so well never see it. DONT DELETE ANY SPACES
		gChatBox.iBoxId = PrepareMercPopupBox( iId, ubMercBoxBackground, ubMercBoxBorder, L"A string that will be hidden,                          ", CHATBOX_WIDTH, CHATBOX_X_MARGIN, usYMargin, CHATBOX_Y_MARGIN_BOTTOM, &usTextBoxWidth, &usTextBoxHeight );
	else
		gChatBox.iBoxId = PrepareMercPopupBox( iId, ubMercBoxBackground, ubMercBoxBorder, zString, CHATBOX_WIDTH, CHATBOX_X_MARGIN, usYMargin, CHATBOX_Y_MARGIN_BOTTOM, &usTextBoxWidth, &usTextBoxHeight );

	if( gChatBox.iBoxId == -1 )
	{
		#ifdef JA2BETAVERSION
			AssertMsg( 0, "Failed in DoMessageBox().	Probable reason is because the string was too large to fit in max message box size." );
		#endif
		return 0;
	}

	// Save height,width
	gChatBox.usWidth = usTextBoxWidth;
	gChatBox.usHeight = usTextBoxHeight;

	// Determine position ( centered in rect )
	gChatBox.sX = (INT16)( ( ( ( aRect.iRight	- aRect.iLeft ) - usTextBoxWidth ) / 2 ) + aRect.iLeft );
	gChatBox.sY = (INT16)( ( ( ( aRect.iBottom - aRect.iTop ) - usTextBoxHeight ) / 2 ) + aRect.iTop );

	if ( guiCurrentScreen == GAME_SCREEN )
	{
		gfStartedFromGameScreen = TRUE;
	}

	if ( (fInMapMode == TRUE ) )
	{
//		fMapExitDueToMessageBox = TRUE;
		gfStartedFromMapScreen = TRUE;
		fMapPanelDirty = TRUE;
	}


	// Set pending screen
	SetPendingNewScreen( MP_CHAT_SCREEN);

	// Init save buffer
	vs_desc.fCreateFlags = VSURFACE_CREATE_DEFAULT | VSURFACE_SYSTEM_MEM_USAGE;
	vs_desc.usWidth = usTextBoxWidth;
	vs_desc.usHeight = usTextBoxHeight;
	vs_desc.ubBitDepth = 16;

	if( AddVideoSurface( &vs_desc, &gChatBox.uiSaveBuffer) == FALSE )
	{
		return( - 1 );
	}

	//Save what we have under here...
	pDestBuf = LockVideoSurface( gChatBox.uiSaveBuffer, &uiDestPitchBYTES);
	pSrcBuf = LockVideoSurface( FRAME_BUFFER, &uiSrcPitchBYTES);

	Blt16BPPTo16BPP((UINT16 *)pDestBuf, uiDestPitchBYTES,
				(UINT16 *)pSrcBuf, uiSrcPitchBYTES,
				0 , 0,
				gChatBox.sX , gChatBox.sY,
				usTextBoxWidth, usTextBoxHeight );

	UnLockVideoSurface( gChatBox.uiSaveBuffer );
	UnLockVideoSurface( FRAME_BUFFER );

	// Create top-level mouse region
	MSYS_DefineRegion( &(gChatBox.BackRegion), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGHEST,
						usCursor, MSYS_NO_CALLBACK, ChatBoxClickCallback );

	// Add region
	MSYS_AddRegion(&(gChatBox.BackRegion) );

	// findout if cursor locked, if so, store old params and store, restore when done
	if( IsCursorRestricted() )
	{
		fCursorLockedToArea = TRUE;
		GetRestrictedClipCursor( &ChatBoxRestrictedCursorRegion );
		FreeMouseCursor( );
	}

	// vars for positioning controls on the chatbox
	int usPosX = 0;
	int usPosY = gChatBox.sY + GetFontHeight(CHATBOX_FONT_TITLE) + CHATBOX_Y_GAP + 5;

	if (bIncludeChatLog)
	{
		// CREATE BUTTONS AND IMAGES FOR CHATLOG

		VOBJECT_DESC	VObjectDesc;
		// will create buttons for interface bottom
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP( "INTERFACE\\mpchatbox.sti", VObjectDesc.ImageFile );
		if( !AddVideoObject( &VObjectDesc, &guiCHATLOGIMG ) )
			Assert( false );

		gChatMessageLogRegion.iTop = usPosY;
		gChatMessageLogRegion.iLeft = gChatBox.sX + (CHATBOX_X_MARGIN / 2);
		gChatMessageLogRegion.iBottom = usPosY + CHATBOX_LOG_HEIGHT;
		gChatMessageLogRegion.iRight =  gChatMessageLogRegion.iLeft + CHATBOX_LOG_WIDTH;

		// SETUP SCROLLING AREA BOUNDS
		CHATLOG_SCROLL_AREA_START_Y = gChatMessageLogRegion.iTop+20;
		CHATLOG_SCROLL_AREA_END_Y = gChatMessageLogRegion.iBottom-20;
		CHATLOG_SCROLL_AREA_HEIGHT = ( CHATLOG_SCROLL_AREA_END_Y - CHATLOG_SCROLL_AREA_START_Y + 1 );

		CHATLOG_SCROLL_AREA_START_X = gChatMessageLogRegion.iRight + CHATBOX_SLIDER_GAP + 1; 
		CHATLOG_SCROLL_AREA_END_X = gChatMessageLogRegion.iRight + CHATBOX_SLIDER_GAP + 1 + CHAT_SLIDER_WIDTH;
		CHATLOG_SCROLL_AREA_WIDTH = ( CHATLOG_SCROLL_AREA_END_X - CHATLOG_SCROLL_AREA_START_X + 1 );

		CHAT_SLIDER_BAR_RANGE = ( CHATLOG_SCROLL_AREA_HEIGHT - CHAT_SLIDER_HEIGHT );

		LoadChatLogSliderBar();

		// Load Scroll button images
		guiChatLogScrollButtonsImage[ CHAT_SCROLL_MESSAGE_UP ]=  LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,11,4,-1,6,-1 );
 
		guiChatLogScrollButtonsImage[ CHAT_SCROLL_MESSAGE_DOWN ]=  LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,12,5,-1,7,-1 );

		// Create buttons
		guiChatLogScrollButtons[ CHAT_SCROLL_MESSAGE_UP ] = QuickCreateButton( guiChatLogScrollButtonsImage[ CHAT_SCROLL_MESSAGE_UP ], gChatMessageLogRegion.iRight + CHATBOX_SLIDER_GAP + 2 , gChatMessageLogRegion.iTop + 1 ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMessageUpChatLogCallback);
		guiChatLogScrollButtons[ CHAT_SCROLL_MESSAGE_DOWN ] = QuickCreateButton( guiChatLogScrollButtonsImage[ CHAT_SCROLL_MESSAGE_DOWN ], gChatMessageLogRegion.iRight + CHATBOX_SLIDER_GAP + 2, gChatMessageLogRegion.iBottom - 16,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMessageDownChatLogCallback);



		usPosY = gChatBox.sY + CHATBOX_Y_MARGIN_LOG + (CHATBOX_Y_GAP * 2) + GetFontHeight(FONT12ARIAL) + 5;
		// END CREATE CHATLOG
	}
	else
		usPosY = gChatBox.sY + CHATBOX_Y_MARGIN_NOLOG + (CHATBOX_Y_GAP * 2) + GetFontHeight(FONT12ARIAL);
	
	// get the middle of the box
	UINT16 middleBox = ( usTextBoxWidth / 2 );

	// CREATE SEND TO ALLIES / ALL TOGGLES
	// send to all
	int toggleWidth = 32 + StringPixLength( gzMPChatToggleText[ 0 ], CHATBOX_FONT_TOGGLE );
	usPosX = gChatBox.sX + ((middleBox - toggleWidth)/2);

	guiChatToggles[ 0 ] = CreateCheckBoxButton( usPosX, usPosY,
												"INTERFACE\\OptionsCheckBoxes_12x12.sti", MSYS_PRIORITY_HIGHEST,
												BtnChatTogglesCallback );
	MSYS_SetBtnUserData( guiChatToggles[ 0 ], 0, 0 );

	// send to allies
	toggleWidth = 32 + StringPixLength( gzMPChatToggleText[ 1 ], CHATBOX_FONT_TOGGLE );
	usPosX = gChatBox.sX + middleBox + ((middleBox - toggleWidth)/2);


	guiChatToggles[ 1 ] = CreateCheckBoxButton( usPosX, usPosY,
												"INTERFACE\\OptionsCheckBoxes_12x12.sti", MSYS_PRIORITY_HIGHEST,
												BtnChatTogglesCallback );
	MSYS_SetBtnUserData( guiChatToggles[ 1 ], 0, 1 );

	usPosY += CHATBOX_TOGGLE_HEIGHT + CHATBOX_Y_GAP;

	// SET DEFAULT FLAGGED

	if (gbChatSendToAll)
		ButtonList[ guiChatToggles[ 0 ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiChatToggles[ 1 ] ]->uiFlags |= BUTTON_CLICKED_ON;

	// END CREATE TOGGLES

	// CREATE TEXT INPUT BOX
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

	usPosX = gChatBox.sX + (CHATBOX_X_MARGIN / 2);
	//Add Player Name textbox 
	AddTextInputField(	usPosX ,
						usPosY , 
						usTextBoxWidth - CHATBOX_X_MARGIN,
						20,
						MSYS_PRIORITY_HIGH+2,
						gszChatBoxInputString,
						255,
						INPUTTYPE_ASCII );//23

	gChatTextBoxRegion.iTop = usPosY;
	gChatTextBoxRegion.iLeft = usPosX;
	gChatTextBoxRegion.iBottom = usPosY+20;
	gChatTextBoxRegion.iRight = usPosX+usTextBoxWidth - CHATBOX_X_MARGIN;

	// exit text input mode in this screen and clean up text boxes
	SetActiveField( 0 );

	usPosY += CHATBOX_INPUT_HEIGHT + CHATBOX_Y_GAP;
	// END CREATE TEXT INPUT BOX

	// CREATE OK AND CANCEL BUTTONS

	// get the button width
	UINT16 btnWidth = GetChatBoxButtonWidth( gChatBox.iButtonImages );

	// Create OK Button
	sButtonX = middleBox + ((middleBox - btnWidth)/2);
	sButtonY = usTextBoxHeight - CHATBOX_BUTTON_HEIGHT - 10;

	gChatBox.uiOKButton = CreateIconAndTextButton( gChatBox.iButtonImages, pMessageStrings[ MSG_OK ], FONT12ARIAL,
													CHATBOX_FONT_COLOR, ubFontShadowColor,
													CHATBOX_FONT_COLOR, ubFontShadowColor,
													TEXT_CJUSTIFIED,
													(INT16)(gChatBox.sX + sButtonX ), (INT16)(gChatBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
													DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)OKChatBoxCallback );
	SetButtonCursor(gChatBox.uiOKButton, usCursor);
	ForceButtonUnDirty( gChatBox.uiOKButton );

	// move the mouse over the ok button
	if( gGameSettings.fOptions[ TOPTION_DONT_MOVE_MOUSE ] == FALSE )
	{
		SimulateMouseMovement( ( gChatBox.sX + sButtonX + 27 ), ( gChatBox.sY + sButtonY + 10) );
	}
	
	// Create Cancel Button
	sButtonX = ((middleBox - btnWidth)/2);
	sButtonY = usTextBoxHeight - CHATBOX_BUTTON_HEIGHT - 10;

	gChatBox.uiNOButton = CreateIconAndTextButton( gChatBox.iButtonImages, pMessageStrings[ MSG_CANCEL ], FONT10ARIAL,
													CHATBOX_FONT_COLOR, ubFontShadowColor,
													CHATBOX_FONT_COLOR, ubFontShadowColor,
													TEXT_CJUSTIFIED,
													(INT16)(gChatBox.sX + sButtonX ), (INT16)(gChatBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
													DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)CancelChatBoxCallback );
	SetButtonCursor(gChatBox.uiNOButton, usCursor);
	ForceButtonUnDirty( gChatBox.uiNOButton );

	// END CREATE BUTTONS

#if 0
	gChatBox.fWasPaused = GamePaused();
	if (!gChatBox.fWasPaused)
	{
		InterruptTime();
		PauseGame();
		LockPauseState( 1 );
		// Pause timers as well....
		PauseTime( TRUE );
	}
#endif

	// Save mouse restriction region...
	GetRestrictedClipCursor( &gOldCursorLimitRectangle );
	FreeMouseCursor( );

	gfNewChatBox = TRUE;

	gfInChatBox			= TRUE;

	return( iId );
}


UINT32	ExitChatBox( INT8 ubExitCode )
{
	UINT32 uiDestPitchBYTES, uiSrcPitchBYTES;
	UINT8	*pDestBuf, *pSrcBuf;
	SGPPoint pPosition;

	// Delete popup!
	RemoveMercPopupBoxFromIndex( gChatBox.iBoxId );
	gChatBox.iBoxId = -1;

	
	// OJW - 20090208 - Add text input box type
	// exit text input mode in this screen and clean up text boxes
	KillAllTextInputModes();
	
	RemoveButton( gChatBox.uiOKButton );
	RemoveButton( gChatBox.uiNOButton );

	// Delete button images
	UnloadButtonImage( gChatBox.iButtonImages );

	//Remove the toggle buttons
	for(int cnt=0; cnt<NUM_CHAT_TOGGLES; cnt++)
	{
		RemoveButton( guiChatToggles[ cnt ] );
	}

	// delete graphics and scrolling buttons / slider
	if (gIncludeChatLog)
	{
		RemoveButton( guiChatLogScrollButtons[ 0 ] );
		RemoveButton( guiChatLogScrollButtons[ 1 ] );
		UnloadButtonImage( guiChatLogScrollButtonsImage[ 0 ] );
		UnloadButtonImage( guiChatLogScrollButtonsImage[ 1 ] );

		DeleteVideoObjectFromIndex( guiCHATLOGIMG );

		DeleteChatLogSliderBar();
	}

#if 0
	if (!gChatBox.fWasPaused)
	{
		// Unpause game....
		UnLockPauseState();
		UnPauseGame();
		// UnPause timers as well....
		PauseTime( FALSE );
	}
#endif

	// Restore mouse restriction region...
	RestrictMouseCursor( &gOldCursorLimitRectangle );


	gfInChatBox = FALSE;

	// Call done callback!
	if ( gChatBox.ExitCallback != NULL )
	{
		(*(gChatBox.ExitCallback))( ubExitCode );
	}


	//if ur in a non gamescreen and DONT want the msg box to use the save buffer, unset gfDontOverRideSaveBuffer in ur callback
	if( ( ( gChatBox.uiExitScreen != GAME_SCREEN ) || ( fRestoreBackgroundForMessageBox == TRUE ) ) && gfDontOverRideSaveBuffer )
	{
		// restore what we have under here...
		pSrcBuf = LockVideoSurface( gChatBox.uiSaveBuffer, &uiSrcPitchBYTES);
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES);

		Blt16BPPTo16BPP((UINT16 *)pDestBuf, uiDestPitchBYTES,
					(UINT16 *)pSrcBuf, uiSrcPitchBYTES,
					gChatBox.sX , gChatBox.sY,
					0, 0,
					gChatBox.usWidth, gChatBox.usHeight );

		UnLockVideoSurface( gChatBox.uiSaveBuffer );
		UnLockVideoSurface( FRAME_BUFFER );

		InvalidateRegion( gChatBox.sX, gChatBox.sY, (INT16)( gChatBox.sX + gChatBox.usWidth ), (INT16)( gChatBox.sY + gChatBox.usHeight ) );
	}

	fRestoreBackgroundForMessageBox = FALSE;
	gfDontOverRideSaveBuffer = TRUE;

	if( fCursorLockedToArea == TRUE )
	{
		GetMousePos( &pPosition );

		if( ( pPosition.iX > ChatBoxRestrictedCursorRegion.iRight ) || ( pPosition.iX > ChatBoxRestrictedCursorRegion.iLeft ) && ( pPosition.iY < ChatBoxRestrictedCursorRegion.iTop ) && ( pPosition.iY > ChatBoxRestrictedCursorRegion.iBottom ) )
		{
			SimulateMouseMovement( pOldMousePosition.iX , pOldMousePosition.iY );
		}

		fCursorLockedToArea = FALSE;
		RestrictMouseCursor( &ChatBoxRestrictedCursorRegion );
	}

	// Remove region
	MSYS_RemoveRegion(&(gChatBox.BackRegion) );

	// Remove save buffer!
	DeleteVideoSurfaceFromIndex( gChatBox.uiSaveBuffer );


	switch( gChatBox.uiExitScreen )
	{
		case GAME_SCREEN:

		if ( InOverheadMap( ) )
		{
		gfOverheadMapDirty = TRUE;
		}
		else
		{
			SetRenderFlags( RENDER_FLAG_FULL );
		}
			break;
		case MAP_SCREEN:
			fMapPanelDirty = TRUE;
			break;
	}

	if ( gfFadeInitialized )
	{
	SetPendingNewScreen(FADE_SCREEN);
	return( FADE_SCREEN );
	}

	return( gChatBox.uiExitScreen );
}

UINT32	MPChatScreenInit( )
{
	InitChatMessageList();
	return( TRUE );
}


UINT32	MPChatScreenHandle( )
{
	InputAtom	InputEvent;

	if ( gfNewChatBox )
	{
		// If in game screen....
		if ( ( gfStartedFromGameScreen )||( gfStartedFromMapScreen ) )
		{
			//UINT32 uiDestPitchBYTES, uiSrcPitchBYTES;
			//UINT8	*pDestBuf, *pSrcBuf;

			if( gfStartedFromGameScreen )
			{
				HandleTacticalUILoseCursorFromOtherScreen( );
			}
			else
			{
				HandleMAPUILoseCursorFromOtherScreen( );
			}

			gfStartedFromGameScreen = FALSE;
			gfStartedFromMapScreen = FALSE;

		}

		gfNewChatBox = FALSE;

		return( MP_CHAT_SCREEN );
	}



	UnmarkButtonsDirty( );

	// Render the box!
	if ( gChatBox.fRenderBox )
	{
	
		// Render the Background ( this includes the text string)
		RenderMercPopUpBoxFromIndex( gChatBox.iBoxId, gChatBox.sX, gChatBox.sY,	FRAME_BUFFER );
		

		UINT16 usWidth = StringPixLength( gzMPChatboxText[0], CHATBOX_FONT_TITLE );
		int usPosY = 0;
		int usPosX = 0;

		usPosY = gChatBox.sY + 10;
		usPosX = gChatBox.sX + ((gChatBox.usWidth - usWidth) / 2);

		DrawTextToScreen( gzMPChatboxText[0], usPosX, usPosY, usWidth, CHATBOX_FONT_TITLE, CHATBOX_FONT_COLOR, DEFAULT_SHADOW, FALSE, CENTER_JUSTIFIED | TEXT_SHADOWED );

		// Render the toggle button strings


		for(UINT8 cnt=0; cnt<NUM_CHAT_TOGGLES; cnt++)
		{
			GUI_BUTTON* btn = ButtonList[ guiChatToggles[ cnt ] ];
			usPosX = btn->XLoc + 12 + 10;
			usPosY = btn->YLoc;
			usWidth = StringPixLength( gzMPChatToggleText[ cnt ], CHATBOX_FONT_TOGGLE );

			DrawTextToScreen( gzMPChatToggleText[ cnt ], usPosX, usPosY, usWidth, CHATBOX_FONT_TOGGLE, CHATBOX_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

		}

		if (gIncludeChatLog)
		{
			// draw chatbox
			HVOBJECT hHandle;
			// get and blt panel
			GetVideoObject(&hHandle, guiCHATLOGIMG );
			BltVideoObject( FRAME_BUFFER , hHandle, 0, gChatMessageLogRegion.iLeft, gChatMessageLogRegion.iTop, VO_BLT_SRCTRANSPARENCY,NULL );
			BltVideoObject( FRAME_BUFFER , hHandle, 1, gChatMessageLogRegion.iRight+CHATBOX_SLIDER_GAP, gChatMessageLogRegion.iTop, VO_BLT_SRCTRANSPARENCY,NULL );
			// draw slider
			DisplayChatLogScrollBarSlider( );
			// draw chat log text
			DisplayStringsInChatLogMessageList();
		}
	}

	MarkButtonsDirty();

	EnableDisableChatLogScrollButtonsAndRegions();

	// Render buttons
	RenderButtons( );

	// render text boxes
	//SaveFontSettings();
	SetFontDestBuffer( FRAME_BUFFER, gChatTextBoxRegion.iLeft , gChatTextBoxRegion.iTop , gChatTextBoxRegion.iRight , gChatTextBoxRegion.iBottom, FALSE );
	RenderAllTextFields(); // textbox system call
	SetFontDestBuffer( FRAME_BUFFER, 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT , FALSE );
	//RestoreFontSettings();
	

	EndFrameBufferRender( );

	// carter, need key shortcuts for clearing up message boxes
	// Check for esc
	bool bHandled;

#ifdef USE_HIGHSPEED_GAMELOOP_TIMER
	while (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
#else
	while (DequeueEvent(&InputEvent) == TRUE)
#endif
	{
		bHandled = false;
		if(InputEvent.usEvent == KEY_DOWN )
		{
			if( ( InputEvent.usParam == ESC ) )
			{
				// Exit messagebox
				gChatBox.bHandled = MSG_BOX_RETURN_NO;
				memset(gszChatBoxInputString,0,sizeof(CHAR16)*255);
				bHandled = true;
			}
			
			if( InputEvent.usParam == ENTER )
			{
				// retrieve the string from the text box
				Get16BitStringFromField( 0, gszChatBoxInputString, 255 ); // these indexes are based on the order created
				// Exit messagebox
				gChatBox.bHandled = MSG_BOX_RETURN_OK;
				bHandled = true;
			}

			// OJW - 20090403 - add better key control
			UINT8 ubDesiredMessageIndex;
			UINT8 ubNumMessages;

			ubNumMessages = GetRangeOfChatLogMessages();

			if ( ubNumMessages > MAX_CHATLOG_MESSAGES )
			{
				if (InputEvent.usParam == PGUP)
				{
					//move up a page
					ChatScreenMsgScrollUp( MAX_CHATLOG_MESSAGES );
					bHandled = true;
				}

				if (InputEvent.usParam == PGDN)
				{
					// move down a page
					ChatScreenMsgScrollDown( MAX_CHATLOG_MESSAGES );
					bHandled = true;
				}

				if (InputEvent.usParam == HOME)
				{
					// move to the beginning
					ChangeCurrentChatScreenMessageIndex( 0 );
					bHandled = true;
				}

				if (InputEvent.usParam == END)
				{
					// move to end
					ubDesiredMessageIndex = ubNumMessages - MAX_CHATLOG_MESSAGES;
					ChangeCurrentChatScreenMessageIndex( ubDesiredMessageIndex );
					bHandled = true;
				}
			}
		}

		// send to text box
		if (!bHandled)
			HandleTextInput( &InputEvent );
	}

	if ( gChatBox.bHandled )
	{
		SetRenderFlags( RENDER_FLAG_FULL );
		return( ExitChatBox( gChatBox.bHandled ) );
	}

	return( MP_CHAT_SCREEN );
}

UINT32	MPChatScreenShutdown(	)
{
	FreeChatMessageList();
	return( FALSE );
}

UINT16 GetChatBoxButtonWidth( INT32 iButtonImage )
{
	return( GetWidthOfButtonPic( (UINT16)iButtonImage, ButtonPictures[iButtonImage].OnNormal ) );
}

void CancelChatBoxCallback(GUI_BUTTON *btn, INT32 reason )
{
	static BOOLEAN fLButtonDown = FALSE;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		fLButtonDown = TRUE;
	}
	else if( ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP ) && fLButtonDown )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		// OK, exit
		gChatBox.bHandled = MSG_BOX_RETURN_NO;
		// clear the ouput string
		memset(gszChatBoxInputString,0,sizeof(CHAR16)*255);
	}
	else if ( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fLButtonDown = FALSE;
	}
}

void OKChatBoxCallback(GUI_BUTTON *btn, INT32 reason )
{
	static BOOLEAN fLButtonDown = FALSE;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		fLButtonDown = TRUE;
	}
	else if( ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP ) && fLButtonDown )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		// OK, exit
		gChatBox.bHandled = MSG_BOX_RETURN_OK;
		// retrieve the string from the text box
		Get16BitStringFromField( 0, gszChatBoxInputString, 255 ); // these indexes are based on the order created
	}
	else if ( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fLButtonDown = FALSE;
	}

}

void BtnChatTogglesCallback( GUI_BUTTON *btn, INT32 reason )
{
	UINT8	ubButton = (UINT8)MSYS_GetBtnUserData( btn, 0 );

	if (ubButton == 1 && cGameType != MP_TYPE_TEAMDEATMATCH)
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			// disable all buttons in grp
			UINT8 cnt;
			for( cnt=0; cnt<NUM_CHAT_TOGGLES; cnt++)
			{
				ButtonList[ guiChatToggles[ cnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			}

			// reselect the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;

			gbChatSendToAll = ubButton == 0; // 0 is send to all
		}
		else
		{
			// check if any other buttons are checked
			UINT8 cnt;
			BOOLEAN fAnyChecked=FALSE;
			for( cnt=0; cnt<NUM_CHAT_TOGGLES; cnt++)
			{
				if( ButtonList[ guiChatToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
				{
					fAnyChecked = TRUE;
				}
			}

			//if none are checked, re check this one
			if( !fAnyChecked )
				btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}

}

void RestoreChatToggleBackGrounds()
{
	UINT8	cnt;

	//Redraw toggle buttons
	for( cnt=0; cnt<NUM_CHAT_TOGGLES; cnt++)
	{
		GUI_BUTTON* btn = ButtonList[ guiChatToggles[ cnt ] ];
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void		ChatBoxClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{

}

// SCROLLING FOR CHAT LOG

void BtnMessageDownChatLogCallback( GUI_BUTTON *btn,INT32 reason )
{
	static INT32 iLastRepeatScrollTime = 0;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		// redraw region
		if( btn->uiFlags & MSYS_HAS_BACKRECT )
		{
			gChatBox.fRenderBox = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);

	iLastRepeatScrollTime = 0;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// redraw region
			if( btn->uiFlags & MSYS_HAS_BACKRECT )
			{
				gChatBox.fRenderBox = TRUE;
			}

			// down a line
			ChatScreenMsgScrollDown( 1 );
	}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if( GetJA2Clock() - iLastRepeatScrollTime >= CHATLOG_BTN_SCROLL_TIME )
		{
			// down a line
			ChatScreenMsgScrollDown( 1 );

		iLastRepeatScrollTime = GetJA2Clock( );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		// redraw region
		if( btn->uiFlags & MSYS_HAS_BACKRECT )
		{
			gChatBox.fRenderBox = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);

	iLastRepeatScrollTime = 0;
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// redraw region
			if( btn->uiFlags & MSYS_HAS_BACKRECT )
			{
				gChatBox.fRenderBox = TRUE;
			}

			// down a page
			ChatScreenMsgScrollDown( MAX_CHATLOG_MESSAGES );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_RBUTTON_REPEAT )
	{
		if( GetJA2Clock() - iLastRepeatScrollTime >= CHATLOG_BTN_SCROLL_TIME )
		{
			// down a page
			ChatScreenMsgScrollDown( MAX_CHATLOG_MESSAGES );

		iLastRepeatScrollTime = GetJA2Clock( );
		}
	}
}


void BtnMessageUpChatLogCallback( GUI_BUTTON *btn,INT32 reason )
{
	static INT32 iLastRepeatScrollTime = 0;


	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

	// redraw region
		if( btn->Area.uiFlags & MSYS_HAS_BACKRECT )
		{
			gChatBox.fRenderBox = TRUE;
		}

	iLastRepeatScrollTime = 0;
	}

	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// redraw region
			if( btn->uiFlags & MSYS_HAS_BACKRECT )
			{
				gChatBox.fRenderBox = TRUE;
			}

			// up a line
			ChatScreenMsgScrollUp( 1 );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if( GetJA2Clock() - iLastRepeatScrollTime >= CHATLOG_BTN_SCROLL_TIME )
		{
			// up a line
			ChatScreenMsgScrollUp( 1 );

			iLastRepeatScrollTime = GetJA2Clock( );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		// redraw region
		if( btn->uiFlags & MSYS_HAS_BACKRECT )
		{
			gChatBox.fRenderBox = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);

	iLastRepeatScrollTime = 0;
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// redraw region
			if( btn->uiFlags & MSYS_HAS_BACKRECT )
			{
				gChatBox.fRenderBox = TRUE;
			}

			// up a page
			ChatScreenMsgScrollUp( MAX_CHATLOG_MESSAGES );
	}
	}
	else if( reason & MSYS_CALLBACK_REASON_RBUTTON_REPEAT )
	{
		if( GetJA2Clock() - iLastRepeatScrollTime >= CHATLOG_BTN_SCROLL_TIME )
		{
			// up a page
			ChatScreenMsgScrollUp( MAX_CHATLOG_MESSAGES );

		iLastRepeatScrollTime = GetJA2Clock( );
		}
	}
}

void EnableDisableChatLogScrollButtonsAndRegions( void )
{
	UINT8 ubNumMessages;

	ubNumMessages = GetRangeOfChatLogMessages();

	// if no scrolling required, or already showing the topmost message
	if( ( ubNumMessages <= MAX_CHATLOG_MESSAGES ) || ( gubFirstChatLogMessageIndex == 0 ) )
	{
		DisableButton( guiChatLogScrollButtons[ CHAT_SCROLL_MESSAGE_UP ] );
		ButtonList[ guiChatLogScrollButtons[ CHAT_SCROLL_MESSAGE_UP ] ]->uiFlags &= ~( BUTTON_CLICKED_ON );
	}
	else
	{
		EnableButton( guiChatLogScrollButtons[ CHAT_SCROLL_MESSAGE_UP ] );
	}

	// if no scrolling required, or already showing the last message
	if( ( ubNumMessages <= MAX_CHATLOG_MESSAGES ) ||
			( ( gubFirstChatLogMessageIndex + MAX_CHATLOG_MESSAGES ) >= ubNumMessages ) )
	{
		DisableButton( guiChatLogScrollButtons[ CHAT_SCROLL_MESSAGE_DOWN ] );
		ButtonList[ guiChatLogScrollButtons[ CHAT_SCROLL_MESSAGE_DOWN ] ]->uiFlags &= ~( BUTTON_CLICKED_ON );
	}
	else
	{
		EnableButton( guiChatLogScrollButtons[ CHAT_SCROLL_MESSAGE_DOWN ] );
	}

	if( ubNumMessages <= MAX_CHATLOG_MESSAGES )
	{
		MSYS_DisableRegion( &gChatLogScrollBarRegion );
	}
	else
	{
		MSYS_EnableRegion( &gChatLogScrollBarRegion );
	}

	if (cGameType==MP_TYPE_TEAMDEATMATCH)
	{
		// Only enable Allies toggle for team deathmatch
		EnableButton( guiChatToggles[1] );
	}
	else
	{
		gbChatSendToAll = true;
		DisableButton( guiChatToggles[1] );
	}
}

void ChatScreenMsgScrollDown( UINT8 ubLinesDown )
{
	UINT8 ubNumMessages;

	ubNumMessages = GetRangeOfChatLogMessages();

	// check if we can go that far, only go as far as we can
	if ( ( gubFirstChatLogMessageIndex + MAX_CHATLOG_MESSAGES + ubLinesDown ) > ubNumMessages )
	{
		ubLinesDown = ubNumMessages - gubFirstChatLogMessageIndex - min( ubNumMessages, MAX_CHATLOG_MESSAGES );
	}

	if ( ubLinesDown > 0 )
	{
		ChangeCurrentChatScreenMessageIndex( ( UINT8 ) ( gubFirstChatLogMessageIndex + ubLinesDown ) );
	}
}


void ChatScreenMsgScrollUp( UINT8 ubLinesUp )
{
	UINT8 ubNumMessages;

	ubNumMessages = GetRangeOfChatLogMessages();

	// check if we can go that far, only go as far as we can
	if ( gubFirstChatLogMessageIndex < ubLinesUp )
	{
		ubLinesUp = gubFirstChatLogMessageIndex;
	}

	if ( ubLinesUp > 0 )
	{
		ChangeCurrentChatScreenMessageIndex( ( UINT8 ) ( gubFirstChatLogMessageIndex - ubLinesUp ) );
	}
}

void MoveToEndOfChatScreenMessageList( void )
{
	UINT8 ubDesiredMessageIndex;
	UINT8 ubNumMessages;

	ubNumMessages = GetRangeOfChatLogMessages();

	ubDesiredMessageIndex = ubNumMessages - min( ubNumMessages, MAX_CHATLOG_MESSAGES );
	ChangeCurrentChatScreenMessageIndex( ubDesiredMessageIndex );
}



void ChangeCurrentChatScreenMessageIndex( UINT8 ubNewMessageIndex )
{
	Assert( ubNewMessageIndex + MAX_CHATLOG_MESSAGES <= max( MAX_CHATLOG_MESSAGES, GetRangeOfChatLogMessages() ) );

	gubFirstChatLogMessageIndex = ubNewMessageIndex;
	gubCurrentChatLogMessageString = ( gubStartOfChatLogMessageList + gubFirstChatLogMessageIndex ) % 256;

	// set fact we just went to a new message
//	gfNewScrollMessage = TRUE;

	// refresh screen
	gChatBox.fRenderBox = TRUE;
}

void LoadChatLogSliderBar( void )
{
	// this function will load the message slider bar
	VOBJECT_DESC	VObjectDesc;

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "INTERFACE\\map_screen_bottom_arrows.sti", VObjectDesc.ImageFile );
	if( !AddVideoObject( &VObjectDesc, &guiChatSliderBar ) )
		Assert(false);
	CreateChatLogMessageScrollBarRegion();
}

void DeleteChatLogSliderBar( void )
{
	// this function will delete message slider bar
	DeleteVideoObjectFromIndex( guiChatSliderBar );

	DeleteChatLogMessageScrollRegion();
}


void CreateChatLogMessageScrollBarRegion( void )
{
		MSYS_DefineRegion( &gChatLogScrollBarRegion, CHATLOG_SCROLL_AREA_START_X, CHATLOG_SCROLL_AREA_START_Y,
								CHATLOG_SCROLL_AREA_END_X, CHATLOG_SCROLL_AREA_END_Y,
								MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ChatScreenMessageScrollBarCallBack );
}


void DeleteChatLogMessageScrollRegion( void )
{
	MSYS_RemoveRegion( &gChatLogScrollBarRegion );
}



void ChatScreenMessageScrollBarCallBack( MOUSE_REGION *pRegion, INT32 iReason )
{
	POINT MousePos;
	UINT8 ubMouseYOffset;
	UINT8	ubDesiredSliderOffset;
	UINT8 ubDesiredMessageIndex;
	UINT8 ubNumMessages;


	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	return;
	}


	if ( iReason & ( MSYS_CALLBACK_REASON_LBUTTON_DWN | MSYS_CALLBACK_REASON_LBUTTON_REPEAT ) )
	{
		// how many messages are there?
		ubNumMessages = GetRangeOfChatLogMessages();

		// region is supposed to be disabled if there aren't enough messages to scroll.	Formulas assume this
		if ( ubNumMessages > MAX_CHATLOG_MESSAGES )
		{
			// where is the mouse?
			GetCursorPos( &MousePos );
			ScreenToClient(ghWindow, &MousePos); // In window coords!

			ubMouseYOffset = (UINT8) MousePos.y - CHATLOG_SCROLL_AREA_START_Y;

			// if clicking in the top 5 pixels of the slider bar
			if ( ubMouseYOffset < ( CHAT_SLIDER_HEIGHT / 2 ) )
			{
				// scroll all the way to the top
				ubDesiredMessageIndex = 0;
			}
			// if clicking in the bottom 6 pixels of the slider bar
			else if ( ubMouseYOffset >= ( CHATLOG_SCROLL_AREA_HEIGHT - ( CHAT_SLIDER_HEIGHT / 2 ) ) )
			{
				// scroll all the way to the bottom
				ubDesiredMessageIndex = ubNumMessages - MAX_CHATLOG_MESSAGES;
			}
			else
			{
				// somewhere in between
				ubDesiredSliderOffset = ubMouseYOffset - ( CHAT_SLIDER_HEIGHT / 2 );

				Assert( ubDesiredSliderOffset <= CHAT_SLIDER_BAR_RANGE );

				// calculate what the index should be to place the slider at this offset (round fractions of .5+ up)
				ubDesiredMessageIndex = ( ( ubDesiredSliderOffset * ( ubNumMessages - MAX_CHATLOG_MESSAGES ) ) + ( CHAT_SLIDER_BAR_RANGE / 2 ) ) / CHAT_SLIDER_BAR_RANGE;
			}

			// if it's a change
			if ( ubDesiredMessageIndex != gubFirstChatLogMessageIndex )
			{
				ChangeCurrentChatScreenMessageIndex( ubDesiredMessageIndex );
			}
		}
	}
}


void DisplayChatLogScrollBarSlider( )
{
	// will display the scroll bar icon
	UINT8 ubNumMessages;
	UINT8 ubSliderOffset;
	HVOBJECT hHandle;


	ubNumMessages = GetRangeOfChatLogMessages();

	// only show the slider if there are more messages than will fit on screen
	if ( ubNumMessages > MAX_CHATLOG_MESSAGES )
	{
		// calculate where slider should be positioned
		ubSliderOffset = ( CHAT_SLIDER_BAR_RANGE * gubFirstChatLogMessageIndex ) / ( ubNumMessages - MAX_CHATLOG_MESSAGES );

		GetVideoObject( &hHandle, guiChatSliderBar );
		BltVideoObject( FRAME_BUFFER, hHandle, 8, CHATLOG_SCROLL_AREA_START_X + 2, CHATLOG_SCROLL_AREA_START_Y + ubSliderOffset, VO_BLT_SRCTRANSPARENCY, NULL );
	}
}

// END MESSAGE SCROLLING

// CHAT MESSAGE ADDING AND PRINTING FUNCTIONS

void ChatLogMessage( UINT16 usColor, UINT8 ubPriority, STR16 pStringA, ... )
{
	// this function sets up the string into several single line structures

	//ScrollStringStPtr pStringSt;
	UINT32 uiFont = CHAT_MESSAGE_FONT;
	//STR16pString;
	va_list argptr;
	CHAR16	DestString[512];
	WRAPPED_STRING *pStringWrapper=NULL;
	WRAPPED_STRING *pStringWrapperHead=NULL;
	BOOLEAN fNewString = FALSE;
	UINT16	usLineWidthIfWordIsWiderThenWidth;


	/*pStringSt=pStringS;
	while(GetNextString(pStringSt))
		 pStringSt=GetNextString(pStringSt);*/

	va_start(argptr, pStringA);			// Set up variable argument pointer
	vswprintf(DestString, pStringA, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	// send message to tactical screen and map screen
	ScreenMsg( usColor, ubPriority, DestString );
	
	pStringWrapperHead=LineWrap(uiFont, CHAT_LINE_WIDTH, &usLineWidthIfWordIsWiderThenWidth, DestString);
	pStringWrapper=pStringWrapperHead;
	if(!pStringWrapper)
	return;

	fNewString = TRUE;

	while(pStringWrapper->pNextWrappedString!=NULL)
	{
		AddStringToChatLogMessageList(pStringWrapper->sString, usColor, uiFont, fNewString, ubPriority );
		fNewString = FALSE;

		pStringWrapper=pStringWrapper->pNextWrappedString;
	}

	AddStringToChatLogMessageList(pStringWrapper->sString, usColor, uiFont, fNewString, ubPriority );


	// clear up list of wrapped strings
	ClearWrappedStringsCHAT( pStringWrapperHead );

	// play new message beep
	//PlayNewMessageSound( );

	MoveToEndOfChatScreenMessageList( );

	//LeaveMutex(SCROLL_MESSAGE_MUTEX, __LINE__, __FILE__);
}



// add string to the chat log message list
void AddStringToChatLogMessageList( STR16 pString, UINT16 usColor, UINT32 uiFont, BOOLEAN fStartOfNewString, UINT8 ubPriority )
{
	ScrollStringStPtr pStringSt = NULL;


	pStringSt = (ScrollStringStPtr) MemAlloc(sizeof(ScrollStringSt));

	SetString(pStringSt, pString);
	SetStringColor(pStringSt, usColor);
	pStringSt->uiFont = uiFont;
	pStringSt->fBeginningOfNewString = fStartOfNewString;
	pStringSt->uiFlags = ubPriority;
	pStringSt->iVideoOverlay = -1;

	// next/previous are not used, it's strictly a wraparound queue
	SetStringNext(pStringSt, NULL);
	SetStringPrev(pStringSt, NULL);


	// Figure out which queue slot index we're going to use to store this
	// If queue isn't full, this is easy, if is is full, we'll re-use the oldest slot
	// Must always keep the wraparound in mind, although this is easy enough with a static, fixed-size queue.


	// always store the new message at the END index

	// check if slot is being used, if so, clear it up
	if( gChatLogMessageList[ gubEndOfChatLogMessageList ] != NULL )
	{
		MemFree( gChatLogMessageList[ gubEndOfChatLogMessageList ]->pString16 );
		MemFree( gChatLogMessageList[ gubEndOfChatLogMessageList ] );
	}

	// store the new message there
	gChatLogMessageList[ gubEndOfChatLogMessageList ] = pStringSt;

	// increment the end
	gubEndOfChatLogMessageList = ( gubEndOfChatLogMessageList + 1 ) % 256;

	// if queue is full, end will now match the start
	if ( gubEndOfChatLogMessageList == gubStartOfChatLogMessageList )
	{
		// if that's so, increment the start
		gubStartOfChatLogMessageList = ( gubStartOfChatLogMessageList + 1 ) % 256;
	}
}


void DisplayStringsInChatLogMessageList( void )
{
	UINT8 ubCurrentStringIndex;
	UINT8	ubLinesPrinted;
	INT16 sX, sY;
	UINT16 usSpacing;

	// Limit drawing to chat log region only, dont want any overdraw
	sX = gChatMessageLogRegion.iLeft + 4;
	SetFontDestBuffer( FRAME_BUFFER, sX , gChatMessageLogRegion.iTop + 4, gChatMessageLogRegion.iRight - 4, gChatMessageLogRegion.iBottom - 4, FALSE );

	SetFont( CHAT_MESSAGE_FONT );		// no longer supports variable fonts
	SetFontBackground( FONT_BLACK );
	SetFontShadow( DEFAULT_SHADOW );

	ubCurrentStringIndex = gubCurrentChatLogMessageString;

	sY = gChatMessageLogRegion.iTop + 4;

	usSpacing = GetFontHeight( CHAT_MESSAGE_FONT );

	for ( ubLinesPrinted = 0; ubLinesPrinted < MAX_CHATLOG_MESSAGES; ubLinesPrinted++ )
	{
		// reached the end of the list?
		if ( ubCurrentStringIndex == gubEndOfChatLogMessageList )
		{
			break;
		}

		// nothing stored there?
		if ( gChatLogMessageList[ ubCurrentStringIndex ] == NULL )
		{
			break;
		}

		// set font color
		SetFontForeground( ( UINT8 )( gChatLogMessageList[ ubCurrentStringIndex ]->usColor ) );

		// print this line
		mprintf_coded( sX, sY, gChatLogMessageList[ ubCurrentStringIndex ]->pString16 );

		sY = sY + usSpacing;

		// next message index to print (may wrap around)
		ubCurrentStringIndex = ( ubCurrentStringIndex + 1 ) % 256;
	}

	// reset region to whole screen
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
}

void InitChatMessageList( void )
{
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < 256; iCounter++ )
	{
		gChatLogMessageList[ iCounter ] = NULL;
	}

	gubEndOfChatLogMessageList = 0;
	gubStartOfChatLogMessageList = 0;
	gubCurrentChatLogMessageString = 0;

	return;
}


void FreeChatMessageList( void )
{
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < 256; iCounter++ )
	{
		// check if next unit is empty, if not...clear it up
		if( gChatLogMessageList[ iCounter ] != NULL )
		{
			MemFree( gChatLogMessageList[ iCounter ]->pString16 );
			MemFree( gChatLogMessageList[ iCounter ] );
		}
	}

	InitChatMessageList( );

	return;
}

UINT8 GetRangeOfChatLogMessages( void )
{
	UINT8 ubRange = 0;

	// NOTE: End is non-inclusive, so start/end 0/0 means no messages, 0/1 means 1 message, etc.
	if( gubStartOfChatLogMessageList <= gubEndOfChatLogMessageList )
	{
		ubRange = gubEndOfChatLogMessageList - gubStartOfChatLogMessageList;
	}
	else
	{
		// this should always be 255 now, since this only happens when queue fills up, and we never remove any messages
		ubRange = ( gubEndOfChatLogMessageList + 256 ) - gubStartOfChatLogMessageList;
	}

	return ( ubRange );
}

// helper function to clear a list of wrapped strings
// This same function exists in message.cpp
// copied here because i didnt want to include wordwrap.h in message.h
void ClearWrappedStringsCHAT( WRAPPED_STRING *pStringWrapperHead )
{
	WRAPPED_STRING *pNode = pStringWrapperHead;
	WRAPPED_STRING *pDeleteNode = NULL;
	// clear out a link list of wrapped string structures

	// error check, is there a node to delete?
	if( pNode == NULL )
	{
		// leave,
		return;
	}

	do
	{

		// set delete node as current node
		pDeleteNode = pNode;

		// set current node as next node
		pNode = pNode->pNextWrappedString;

		//delete the string
		MemFree( pDeleteNode->sString );
		pDeleteNode->sString = NULL;

		// clear out delete node
		MemFree( pDeleteNode );
		pDeleteNode = NULL;

	}	while( pNode );


//	MemFree( pNode );

	pStringWrapperHead = NULL;

}