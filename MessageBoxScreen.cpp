#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include "sgp.h"
	#include "screenids.h"
	#include "fade screen.h"
	#include "sysutil.h"
	#include "vobject_blitters.h"
	#include "MercTextBox.h"
	#include "cursors.h"
	#include "messageboxscreen.h"
	#include "font control.h"
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
	#include "DropDown.h"
#endif

#define		MSGBOX_DEFAULT_WIDTH							300

#define		MSGBOX_BUTTON_WIDTH								61
#define		MSGBOX_BUTTON_HEIGHT							20
#define		MSGBOX_BUTTON_X_SEP								15

#define		MSGBOX_SMALL_BUTTON_WIDTH					31
#define		MSGBOX_SMALL_BUTTON_X_SEP					8

typedef void (*MSGBOX_CALLBACK)( UINT8 bExitValue );

// old mouse x and y positions
SGPPoint pOldMousePosition;
SGPRect MessageBoxRestrictedCursorRegion;

// if the cursor was locked to a region
BOOLEAN fCursorLockedToArea = FALSE;
BOOLEAN	gfInMsgBox = FALSE;

//extern BOOLEAN fMapExitDueToMessageBox;
extern BOOLEAN fInMapMode;
extern BOOLEAN gfOverheadMapDirty;

//OJW - 20090208
CHAR16 gszMsgBoxInputString[255];

void		OKMsgBoxCallback(GUI_BUTTON *btn, INT32 reason );
void		YESMsgBoxCallback(GUI_BUTTON *btn, INT32 reason );
void		ContractMsgBoxCallback(GUI_BUTTON *btn, INT32 reason );
void		LieMsgBoxCallback(GUI_BUTTON *btn, INT32 reason );
void		NOMsgBoxCallback(GUI_BUTTON *btn, INT32 reason );
void		NumberedMsgBoxCallback(GUI_BUTTON *btn, INT32 reason );
void		MsgBoxClickCallback( MOUSE_REGION * pRegion, INT32 iReason );
UINT32	ExitMsgBox( INT8 ubExitCode );
UINT16	GetMSgBoxButtonWidth( INT32 iButtonImage );

SGPRect gOldCursorLimitRectangle;


MESSAGE_BOX_STRUCT	gMsgBox;
BOOLEAN							gfNewMessageBox = FALSE;
BOOLEAN							gfStartedFromGameScreen = FALSE;
BOOLEAN							gfStartedFromMapScreen = FALSE;
BOOLEAN							fRestoreBackgroundForMessageBox = FALSE;
BOOLEAN							gfDontOverRideSaveBuffer = TRUE;	//this variable can be unset if ur in a non gamescreen and DONT want the msg box to use the save buffer
extern void HandleTacticalUILoseCursorFromOtherScreen( );
extern STR16 pUpdatePanelButtons[];

CHAR16		gzUserDefinedButton1[ 128 ];
CHAR16		gzUserDefinedButton2[ 128 ];

// Flugente: made an array for user-defined buttons
CHAR16		gzUserDefinedButton[ NUM_CUSTOM_BUTTONS ][ 128 ];
// sevenfm: added color for buttons
UINT16	gzUserDefinedButtonColor[ NUM_CUSTOM_BUTTONS ];

INT32 DoMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT32 usFlags, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect, UINT8 ubDefaultButton )
{
	VSURFACE_DESC		vs_desc;
	UINT16	usTextBoxWidth;
	UINT16	usTextBoxHeight;
	SGPRect	aRect;
	UINT32 uiDestPitchBYTES, uiSrcPitchBYTES;
	UINT8	*pDestBuf, *pSrcBuf;
	INT16	sButtonX, sButtonY, sBlankSpace;
	UINT8	ubMercBoxBackground = BASIC_MERC_POPUP_BACKGROUND, ubMercBoxBorder = BASIC_MERC_POPUP_BORDER;
	UINT8	ubFontColor, ubFontShadowColor;
	UINT16	usCursor;
	INT32 iId = -1;

	GetMousePos( &pOldMousePosition );

	//this variable can be unset if ur in a non gamescreen and DONT want the msg box to use the save buffer
	gfDontOverRideSaveBuffer = TRUE;

	SetCurrentCursorFromDatabase( CURSOR_NORMAL );

	if( gMsgBox.BackRegion.uiFlags & MSYS_REGION_EXISTS )
	{
		return( 0 );
	}

	// Based on style....
	switch( ubStyle )
	{
		//default
		case 	MSG_BOX_BASIC_STYLE:

			ubMercBoxBackground = DIALOG_MERC_POPUP_BACKGROUND;
			ubMercBoxBorder			= DIALOG_MERC_POPUP_BORDER;

			// Add button images
			gMsgBox.iButtonImages			= LoadButtonImage( "INTERFACE\\popupbuttons.sti", -1,0,-1,1,-1 );
			ubFontColor	= FONT_MCOLOR_WHITE;
			ubFontShadowColor = DEFAULT_SHADOW;
			usCursor = CURSOR_NORMAL;

			break;

		case MSG_BOX_RED_ON_WHITE:
			ubMercBoxBackground = WHITE_MERC_POPUP_BACKGROUND;
			ubMercBoxBorder			= RED_MERC_POPUP_BORDER;

			// Add button images
			gMsgBox.iButtonImages			= LoadButtonImage( "INTERFACE\\msgboxRedButtons.sti", -1,0,-1,1,-1 );

			ubFontColor	= 2;
			ubFontShadowColor = NO_SHADOW;
			usCursor = CURSOR_LAPTOP_SCREEN;
			break;

		case MSG_BOX_BLUE_ON_GREY:
			ubMercBoxBackground = GREY_MERC_POPUP_BACKGROUND;
			ubMercBoxBorder			= BLUE_MERC_POPUP_BORDER;

			// Add button images
			gMsgBox.iButtonImages			= LoadButtonImage( "INTERFACE\\msgboxGreyButtons.sti", -1,0,-1,1,-1 );

			ubFontColor	= 2;
			ubFontShadowColor = FONT_MCOLOR_WHITE;
			usCursor = CURSOR_LAPTOP_SCREEN;
			break;
	case MSG_BOX_IMP_STYLE:
			ubMercBoxBackground = IMP_POPUP_BACKGROUND;
			ubMercBoxBorder			= DIALOG_MERC_POPUP_BORDER;

			// Add button images
			gMsgBox.iButtonImages			= LoadButtonImage( "INTERFACE\\msgboxGreyButtons.sti", -1,0,-1,1,-1 );

			ubFontColor	= 2;
			ubFontShadowColor = FONT_MCOLOR_WHITE;
			usCursor = CURSOR_LAPTOP_SCREEN;
			break;
		case MSG_BOX_BASIC_SMALL_BUTTONS:

			ubMercBoxBackground = DIALOG_MERC_POPUP_BACKGROUND;
			ubMercBoxBorder			= DIALOG_MERC_POPUP_BORDER;

			// Add button images
			gMsgBox.iButtonImages			= LoadButtonImage( "INTERFACE\\popupbuttons.sti", -1,2,-1,3,-1 );
			ubFontColor	= FONT_MCOLOR_WHITE;
			ubFontShadowColor = DEFAULT_SHADOW;
			usCursor = CURSOR_NORMAL;

			break;

		case MSG_BOX_LAPTOP_DEFAULT:
			ubMercBoxBackground = LAPTOP_POPUP_BACKGROUND;
			ubMercBoxBorder			= LAPTOP_POP_BORDER;

			// Add button images
			gMsgBox.iButtonImages			= LoadButtonImage( "INTERFACE\\popupbuttons.sti", -1,0,-1,1,-1 );
			ubFontColor	= FONT_MCOLOR_WHITE;
			ubFontShadowColor = DEFAULT_SHADOW;
			usCursor = CURSOR_LAPTOP_SCREEN;
			break;

		case MSG_BOX_BASIC_MEDIUM_BUTTONS:
			ubMercBoxBackground = DIALOG_MERC_POPUP_BACKGROUND;
			ubMercBoxBorder			= DIALOG_MERC_POPUP_BORDER;

			// Add button images
			gMsgBox.iButtonImages			= LoadButtonImage( "INTERFACE\\PreferencesButtons.sti", -1,0,-1,1,-1 );
			ubFontColor	= FONT_MCOLOR_WHITE;
			ubFontShadowColor = DEFAULT_SHADOW;
			usCursor = CURSOR_NORMAL;

			break;

		default:
			ubMercBoxBackground = BASIC_MERC_POPUP_BACKGROUND;
			ubMercBoxBorder			= BASIC_MERC_POPUP_BORDER;

			// Add button images
			gMsgBox.iButtonImages			= LoadButtonImage( "INTERFACE\\msgboxbuttons.sti", -1,0,-1,1,-1 );
			ubFontColor	= FONT_MCOLOR_WHITE;
			ubFontShadowColor = DEFAULT_SHADOW;
			usCursor = CURSOR_NORMAL;
			break;
	}
	
	if ( usFlags & MSG_BOX_FLAG_USE_CENTERING_RECT && pCenteringRect != NULL )
	{
		aRect.iTop = 	pCenteringRect->iTop;
		aRect.iLeft = 	pCenteringRect->iLeft;
		aRect.iBottom = 	pCenteringRect->iBottom;
		aRect.iRight = 	pCenteringRect->iRight;
	}
	else
	{
		// Use default!
		aRect.iTop		= 	0;
		aRect.iLeft		= 	0;
		aRect.iBottom = 	SCREEN_HEIGHT;
		aRect.iRight	= 	SCREEN_WIDTH;
	}

	// Set some values!
	gMsgBox.usFlags				= usFlags;
	gMsgBox.uiExitScreen	= uiExitScreen;
	gMsgBox.ExitCallback	= ReturnCallback;
	gMsgBox.fRenderBox		= TRUE;
	gMsgBox.bHandled			= 0;

	// Flugente: increase the height of the display box under certain conditions
	UINT16 heightincrease = 0;
	if ( usFlags & MSG_BOX_FLAG_GENERIC_FOUR_BUTTONS )
		heightincrease = 120;
	if ( usFlags & MSG_BOX_FLAG_GENERIC_EIGHT_BUTTONS )
	{		
		if( ubStyle == MSG_BOX_BASIC_MEDIUM_BUTTONS )
			heightincrease = 120;
		else
			heightincrease = 50;
	}

	if ( usFlags & MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS )
		heightincrease = 90;

	if ( usFlags & MSG_BOX_FLAG_DROPDOWN )
		heightincrease = 130;

	UINT16 usMBWidth=MSGBOX_DEFAULT_WIDTH;
	BOOLEAN bFixedWidth = FALSE;
	// sevenfm: custom width for 16-medium-button  messagebox
	if( usFlags & MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS && ubStyle == MSG_BOX_BASIC_STYLE )
	{
		usMBWidth = MSGBOX_BUTTON_WIDTH * 4;
		bFixedWidth = TRUE;
	}
	// custom width for 8-large-button messagebox
	if( usFlags & MSG_BOX_FLAG_GENERIC_EIGHT_BUTTONS && ubStyle == MSG_BOX_BASIC_MEDIUM_BUTTONS )
	{
		usMBWidth = MSGBOX_BUTTON_WIDTH * 4;
		bFixedWidth = TRUE;
	}

	// Init message box
	gMsgBox.iBoxId = PrepareMercPopupBox( iId, ubMercBoxBackground, ubMercBoxBorder, zString, usMBWidth, 40, 10, 30 + heightincrease, &usTextBoxWidth, &usTextBoxHeight, bFixedWidth );

	if( gMsgBox.iBoxId == -1 )
	{
#ifdef JA2BETAVERSION
		AssertMsg( 0, "Failed in DoMessageBox().	Probable reason is because the string was too large to fit in max message box size." );
#endif
		return 0;
	}

	// Save height,width
	gMsgBox.usWidth = usTextBoxWidth;
	gMsgBox.usHeight = usTextBoxHeight;

	// Determine position ( centered in rect )
	gMsgBox.sX = (INT16)( ( ( ( aRect.iRight	- aRect.iLeft ) - usTextBoxWidth ) / 2 ) + aRect.iLeft );
	gMsgBox.sY = (INT16)( ( ( ( aRect.iBottom - aRect.iTop ) - usTextBoxHeight ) / 2 ) + aRect.iTop );

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
	SetPendingNewScreen( MSG_BOX_SCREEN);

	// Init save buffer
	vs_desc.fCreateFlags = VSURFACE_CREATE_DEFAULT | VSURFACE_SYSTEM_MEM_USAGE;
	vs_desc.usWidth = usTextBoxWidth;
	vs_desc.usHeight = usTextBoxHeight;
	vs_desc.ubBitDepth = 16;

	if( AddVideoSurface( &vs_desc, &gMsgBox.uiSaveBuffer) == FALSE )
	{
		return( - 1 );
	}

	//Save what we have under here...
	pDestBuf = LockVideoSurface( gMsgBox.uiSaveBuffer, &uiDestPitchBYTES);
	pSrcBuf = LockVideoSurface( FRAME_BUFFER, &uiSrcPitchBYTES);

	Blt16BPPTo16BPP((UINT16 *)pDestBuf, uiDestPitchBYTES,
				(UINT16 *)pSrcBuf, uiSrcPitchBYTES,
				0 , 0,
				gMsgBox.sX , gMsgBox.sY,
				usTextBoxWidth, usTextBoxHeight );

	UnLockVideoSurface( gMsgBox.uiSaveBuffer );
	UnLockVideoSurface( FRAME_BUFFER );
		
	// Create top-level mouse region
	MSYS_DefineRegion( &(gMsgBox.BackRegion), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGHEST - 1,	usCursor, MSYS_NO_CALLBACK, MsgBoxClickCallback );

	if( gGameSettings.fOptions[ TOPTION_DONT_MOVE_MOUSE ] == FALSE )
	{
		if( usFlags & MSG_BOX_FLAG_OK )
		{
			SimulateMouseMovement( ( gMsgBox.sX + ( usTextBoxWidth / 2 ) + 27 ), ( gMsgBox.sY + ( usTextBoxHeight - 10 ) ) );
		}
		// silversurfer: new mouse positions over a default button
		// This can be expanded for other message box types if the need arises.
		else if( ubDefaultButton != MSG_BOX_DEFAULT_BUTTON_NONE )
		{
			if( usFlags & MSG_BOX_FLAG_GENERIC_FOUR_BUTTONS ) // 4 rows, 1 column, medium button size
			{
				sButtonX = usTextBoxWidth / 2;
				sButtonY = usTextBoxHeight - ( (4 - ubDefaultButton) * (MSGBOX_BUTTON_HEIGHT + MSGBOX_SMALL_BUTTON_X_SEP + 5) + 35 );
			}
			else if( usFlags & MSG_BOX_FLAG_GENERIC_EIGHT_BUTTONS )
			{
				if( ubStyle == MSG_BOX_BASIC_MEDIUM_BUTTONS ) // 4 rows, 2 columns, medium button size
				{
					// left or right
					if( ubDefaultButton % 2)
						sButtonX = usTextBoxWidth / 2 - MSGBOX_BUTTON_WIDTH;
					else
						sButtonX = usTextBoxWidth / 2 + MSGBOX_BUTTON_WIDTH;

					sButtonY = usTextBoxHeight - ( (4 - (UINT8)(ubDefaultButton / 2.0f + 0.6f) ) * (MSGBOX_BUTTON_HEIGHT + MSGBOX_SMALL_BUTTON_X_SEP + 5) + 35 );
				}
				else // 2 rows, 4 columns, small button size
				{
					// left or right
					if( (UINT8)(ubDefaultButton / 2.0f + 0.6f) % 2 )
						sButtonX = usTextBoxWidth / 2 - MSGBOX_SMALL_BUTTON_WIDTH / 2 - (ubDefaultButton % 2) * MSGBOX_SMALL_BUTTON_WIDTH - MSGBOX_SMALL_BUTTON_X_SEP;
					else
						sButtonX = usTextBoxWidth / 2 + MSGBOX_SMALL_BUTTON_WIDTH / 2 + (1 - ubDefaultButton % 2) * MSGBOX_SMALL_BUTTON_WIDTH + MSGBOX_SMALL_BUTTON_X_SEP;

					sButtonY = usTextBoxHeight - ( (2 - (UINT8)(ubDefaultButton / 4.0f + 0.8f) ) * (MSGBOX_BUTTON_HEIGHT + 10) + 30 );
				}
			}
			else if( usFlags & MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS )
			{
				if( ubStyle == MSG_BOX_BASIC_STYLE ) // 4x4 medium button size
				{
					// left or right
					if( (UINT8)(ubDefaultButton / 2.0f + 0.6f) % 2 )
						sButtonX = usTextBoxWidth / 2 - MSGBOX_BUTTON_WIDTH / 2 - (ubDefaultButton % 2) * MSGBOX_BUTTON_WIDTH;
					else
						sButtonX = usTextBoxWidth / 2 + MSGBOX_BUTTON_WIDTH / 2 + (1 - ubDefaultButton % 2) * MSGBOX_BUTTON_WIDTH;

					sButtonY = usTextBoxHeight - ( (4 - (UINT8)(ubDefaultButton / 4.0f + 0.8f) ) * (MSGBOX_BUTTON_HEIGHT + 5) + 30 );
				}
				else // 4x4 small button size
				{
					// left or right
					if( (UINT8)(ubDefaultButton / 2.0f + 0.6f) % 2 )
						sButtonX = usTextBoxWidth / 2 - MSGBOX_SMALL_BUTTON_WIDTH / 2 - (ubDefaultButton % 2) * MSGBOX_SMALL_BUTTON_WIDTH - MSGBOX_SMALL_BUTTON_X_SEP;
					else
						sButtonX = usTextBoxWidth / 2 + MSGBOX_SMALL_BUTTON_WIDTH / 2 + (1 - ubDefaultButton % 2) * MSGBOX_SMALL_BUTTON_WIDTH + MSGBOX_SMALL_BUTTON_X_SEP;

					sButtonY = usTextBoxHeight - ( (4 - (UINT8)(ubDefaultButton / 4.0f + 0.8f) ) * (MSGBOX_BUTTON_HEIGHT + 10) + 20 );
				}
			}

			SimulateMouseMovement( gMsgBox.sX + sButtonX , gMsgBox.sY + sButtonY );
		}
		// old default mouse position at bottom center of message box
		else
			SimulateMouseMovement( gMsgBox.sX + usTextBoxWidth / 2 , gMsgBox.sY + usTextBoxHeight - 4 );
	}

	// Add region
	MSYS_AddRegion(&(gMsgBox.BackRegion) );

	// findout if cursor locked, if so, store old params and store, restore when done
	if( IsCursorRestricted() )
	{
		fCursorLockedToArea = TRUE;
		GetRestrictedClipCursor( &MessageBoxRestrictedCursorRegion );
		FreeMouseCursor( FALSE );
	}

	// Create four numbered buttons
	if ( usFlags & MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS )
	{
		// This is exclusive of any other buttons... no ok, no cancel, no nothing

		sBlankSpace = usTextBoxWidth - MSGBOX_SMALL_BUTTON_WIDTH * 4 - MSGBOX_SMALL_BUTTON_X_SEP * 3;
		sButtonX = sBlankSpace / 2;
		sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10;

		gMsgBox.uiButton[0] = CreateIconAndTextButton( gMsgBox.iButtonImages, L"1", FONT12ARIAL,
														ubFontColor, ubFontShadowColor,
														ubFontColor, ubFontShadowColor,
														TEXT_CJUSTIFIED,
														(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
														DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NumberedMsgBoxCallback );
		MSYS_SetBtnUserData( gMsgBox.uiButton[0], 0, 1);
		SetButtonCursor(gMsgBox.uiButton[0], usCursor);

		sButtonX += MSGBOX_SMALL_BUTTON_WIDTH + MSGBOX_SMALL_BUTTON_X_SEP;
		gMsgBox.uiButton[1] = CreateIconAndTextButton( gMsgBox.iButtonImages, L"2", FONT12ARIAL,
														ubFontColor, ubFontShadowColor,
														ubFontColor, ubFontShadowColor,
														TEXT_CJUSTIFIED,
														(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
														DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NumberedMsgBoxCallback );
		MSYS_SetBtnUserData( gMsgBox.uiButton[1], 0, 2);
		SetButtonCursor(gMsgBox.uiButton[1], usCursor);

		sButtonX += MSGBOX_SMALL_BUTTON_WIDTH + MSGBOX_SMALL_BUTTON_X_SEP;
		gMsgBox.uiButton[2] = CreateIconAndTextButton( gMsgBox.iButtonImages, L"3", FONT12ARIAL,
														ubFontColor, ubFontShadowColor,
														ubFontColor, ubFontShadowColor,
														TEXT_CJUSTIFIED,
														(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
														DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NumberedMsgBoxCallback );
		MSYS_SetBtnUserData( gMsgBox.uiButton[2], 0, 3);
		SetButtonCursor(gMsgBox.uiButton[2], usCursor);

		sButtonX += MSGBOX_SMALL_BUTTON_WIDTH + MSGBOX_SMALL_BUTTON_X_SEP;
		gMsgBox.uiButton[3] = CreateIconAndTextButton( gMsgBox.iButtonImages, L"4", FONT12ARIAL,
														ubFontColor, ubFontShadowColor,
														ubFontColor, ubFontShadowColor,
														TEXT_CJUSTIFIED,
														(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
														DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NumberedMsgBoxCallback );
		MSYS_SetBtnUserData( gMsgBox.uiButton[3], 0, 4);
		SetButtonCursor(gMsgBox.uiButton[3], usCursor);

		for ( INT8 i = 3; i >= 0; --i)
		{
			ForceButtonUnDirty( gMsgBox.uiButton[i] );
		}
	}
	// Create four numbered buttons
	else if ( usFlags & MSG_BOX_FLAG_GENERIC_FOUR_BUTTONS )
	{
		sButtonX = (usTextBoxWidth - 115)/ 2;
		sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 130 - MSGBOX_SMALL_BUTTON_WIDTH - MSGBOX_SMALL_BUTTON_X_SEP;

		for ( INT8 j = 0; j < 4; ++j)
		{
			sButtonY += 35;
			gMsgBox.uiButton[j] = CreateIconAndTextButton( gMsgBox.iButtonImages, gzUserDefinedButton[j], FONT12ARIAL,
													ubFontColor, ubFontShadowColor,
													ubFontColor, ubFontShadowColor,
													TEXT_CJUSTIFIED,
													(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
													DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NumberedMsgBoxCallback );
			MSYS_SetBtnUserData( gMsgBox.uiButton[j], 0, j+1);
			SetButtonCursor(gMsgBox.uiButton[j], usCursor);
			ForceButtonUnDirty( gMsgBox.uiButton[j] );
		}
	}
	// Create eight numbered buttons
	else if ( usFlags & MSG_BOX_FLAG_GENERIC_EIGHT_BUTTONS )
	{
		if(ubStyle == MSG_BOX_BASIC_MEDIUM_BUTTONS)
		{
			//sBlankSpace = usTextBoxWidth - MSGBOX_BUTTON_WIDTH * 4 - MSGBOX_SMALL_BUTTON_X_SEP * 3;
			sBlankSpace = usTextBoxWidth - MSGBOX_BUTTON_WIDTH * 4 + MSGBOX_SMALL_BUTTON_X_SEP;
			sButtonX = sBlankSpace / 2;
			sButtonY = usTextBoxHeight - 3*MSGBOX_BUTTON_HEIGHT  - heightincrease - 10;

			for ( INT8 i = 0; i < 4; ++i)
			{
				// new row
				sButtonY += MSGBOX_SMALL_BUTTON_WIDTH + 5;
				// begin from the front
				//sButtonX = sBlankSpace / 2 -  MSGBOX_BUTTON_WIDTH*2;
				sButtonX = sBlankSpace / 2;
				for ( INT8 j = 0; j < 2; ++j)
				{
					INT8 k = 2*i + j;

					gMsgBox.uiButton[k] = CreateIconAndTextButton( gMsgBox.iButtonImages, gzUserDefinedButton[k], FONT12ARIAL,
						ubFontColor, ubFontShadowColor,
						ubFontColor, ubFontShadowColor,
						TEXT_CJUSTIFIED,
						(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
						DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NumberedMsgBoxCallback );
					MSYS_SetBtnUserData( gMsgBox.uiButton[k], 0, k+1);
					SetButtonCursor(gMsgBox.uiButton[k], usCursor);
					ForceButtonUnDirty( gMsgBox.uiButton[k] );

					sButtonX += MSGBOX_BUTTON_WIDTH*2;
				}
			}
		}
		else
		{
			sBlankSpace = usTextBoxWidth - MSGBOX_SMALL_BUTTON_WIDTH * 4 - MSGBOX_SMALL_BUTTON_X_SEP * 3;
			sButtonX = sBlankSpace / 2;
			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10 - heightincrease;
			sButtonY -= MSGBOX_SMALL_BUTTON_WIDTH - MSGBOX_SMALL_BUTTON_X_SEP;

			for ( INT8 i = 0; i < 2; ++i)
			{
				// new row
				sButtonY += MSGBOX_SMALL_BUTTON_WIDTH;

				// begin from the front
				sButtonX = sBlankSpace / 2;

				for ( INT8 j = 0; j < 4; ++j)
				{
					INT8 k = 4*i + j;

					gMsgBox.uiButton[k] = CreateIconAndTextButton( gMsgBox.iButtonImages, gzUserDefinedButton[k], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NumberedMsgBoxCallback );
					MSYS_SetBtnUserData( gMsgBox.uiButton[k], 0, k+1);
					SetButtonCursor(gMsgBox.uiButton[k], usCursor);
					ForceButtonUnDirty( gMsgBox.uiButton[k] );

					//sButtonX += 75 + MSGBOX_SMALL_BUTTON_WIDTH + MSGBOX_SMALL_BUTTON_X_SEP;
					sButtonX += MSGBOX_SMALL_BUTTON_WIDTH + MSGBOX_SMALL_BUTTON_X_SEP;
				}
			}
		}
	}
	// Create sixteen numbered buttons
	else if ( usFlags & MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS )
	{
		if( ubStyle == MSG_BOX_BASIC_STYLE )
		{
			sBlankSpace = usTextBoxWidth - MSGBOX_BUTTON_WIDTH * 4;
			sButtonX = sBlankSpace / 2;
			sButtonY = usTextBoxHeight - 2*MSGBOX_BUTTON_HEIGHT  - heightincrease - 10;
		}
		else
		{
			sBlankSpace = usTextBoxWidth - MSGBOX_SMALL_BUTTON_WIDTH * 4 - MSGBOX_SMALL_BUTTON_X_SEP * 3;
			sButtonX = sBlankSpace / 2;
			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10 - heightincrease - 6;
			sButtonY -= MSGBOX_SMALL_BUTTON_WIDTH - MSGBOX_SMALL_BUTTON_X_SEP;
		}
				
		for ( INT8 i = 0; i < 4; ++i)
		{
			if( ubStyle == MSG_BOX_BASIC_STYLE )
			{
			// new row
				sButtonY += MSGBOX_BUTTON_HEIGHT+5;
				// begin from the front
				sButtonX = sBlankSpace / 2 -  MSGBOX_BUTTON_WIDTH;
			}
			else
			{
				// new row
				sButtonY += MSGBOX_SMALL_BUTTON_WIDTH - 2;// + MSGBOX_SMALL_BUTTON_X_SEP;
				// begin from the front
				sButtonX = sBlankSpace / 2 -  MSGBOX_SMALL_BUTTON_WIDTH - MSGBOX_SMALL_BUTTON_X_SEP;
			}

			for ( INT8 j = 0; j < 4; ++j)
			{
				INT8 k = 4*i + j;
				
				if( ubStyle == MSG_BOX_BASIC_STYLE )
					sButtonX += MSGBOX_BUTTON_WIDTH;
				else
				sButtonX += MSGBOX_SMALL_BUTTON_WIDTH + MSGBOX_SMALL_BUTTON_X_SEP;
				// sevenfm: added color table for 16-button messagebox
				gMsgBox.uiButton[k] = CreateIconAndTextButton( gMsgBox.iButtonImages, gzUserDefinedButton[k], FONT12ARIAL,
														gzUserDefinedButtonColor[k] ? gzUserDefinedButtonColor[k] : ubFontColor, ubFontShadowColor,
														ubFontColor, ubFontShadowColor,
														TEXT_CJUSTIFIED,
														(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
														DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NumberedMsgBoxCallback );
				MSYS_SetBtnUserData( gMsgBox.uiButton[k], 0, k+1);
				SetButtonCursor(gMsgBox.uiButton[k], usCursor);
				ForceButtonUnDirty( gMsgBox.uiButton[k] );
			}
		}
	}
	else if (usFlags & MSG_BOX_FLAG_INPUTBOX)
	{
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

		int ibx = gMsgBox.sX + 10;
		int iby = gMsgBox.sY +(usTextBoxHeight - 20 - 10);
		//Add Player Name textbox 
		AddTextInputField(	ibx,
							iby, 
							usTextBoxWidth - 20,
							20,
							MSYS_PRIORITY_HIGH+2,
							gszMsgBoxInputString,
							255,
							INPUTTYPE_ASCII );//23

		// exit text input mode in this screen and clean up text boxes
		SetActiveField( 0 );

		// initialise the chat toggle boxes
		/*int usPosY = gMsgBox.sY + (usTextBoxHeight - 45);
		int usPosX = gMsgBox.sX + (usTextBoxWidth / 3);

		guiChatToggles[ 0 ] = CreateCheckBoxButton( usPosX, usPosY,
													"INTERFACE\\OptionsCheckBoxes_12x12.sti", MSYS_PRIORITY_HIGH+10,
													BtnChatTogglesCallback );
		MSYS_SetBtnUserData( guiOptionsToggles[ 0 ], 0, 0 );

		guiChatToggles[ 1 ] = CreateCheckBoxButton( usPosX, usPosY,
													"INTERFACE\\OptionsCheckBoxes_12x12.sti", MSYS_PRIORITY_HIGH+10,
													BtnChatTogglesCallback );
		MSYS_SetBtnUserData( guiOptionsToggles[ 1 ], 0, 1 );*/

	}
	else
	{
		// Create text button
		if ( usFlags & MSG_BOX_FLAG_OK )
		{

//			sButtonX = ( usTextBoxWidth - MSGBOX_BUTTON_WIDTH ) / 2;
			sButtonX = ( usTextBoxWidth - GetMSgBoxButtonWidth( gMsgBox.iButtonImages ) ) / 2;

			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10;

			gMsgBox.uiOKButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_OK ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)OKMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiOKButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiOKButton );
		}
		
		// Create text button
		if ( usFlags & MSG_BOX_FLAG_CANCEL )
		{
			sButtonX = ( usTextBoxWidth - GetMSgBoxButtonWidth( gMsgBox.iButtonImages ) ) / 2;
			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10;

			gMsgBox.uiOKButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_CANCEL ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)OKMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiOKButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiOKButton );

		}

		if ( usFlags & MSG_BOX_FLAG_YESNO )
		{
			sButtonX = ( usTextBoxWidth - ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ) / 2;
			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10;

			gMsgBox.uiYESButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_YES ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)YESMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiYESButton, usCursor);

			ForceButtonUnDirty( gMsgBox.uiYESButton );

			gMsgBox.uiNOButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_NO ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX + ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NOMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiNOButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiNOButton );

		}

		if ( usFlags & MSG_BOX_FLAG_CONTINUESTOP )
		{
			sButtonX = ( usTextBoxWidth - ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ) / 2;
			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10;

			gMsgBox.uiYESButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pUpdatePanelButtons[ 0 ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)YESMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiYESButton, usCursor);

			ForceButtonUnDirty( gMsgBox.uiYESButton );

			gMsgBox.uiNOButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pUpdatePanelButtons[ 1 ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX + ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NOMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiNOButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiNOButton );
		}

		if ( usFlags & MSG_BOX_FLAG_OKCONTRACT )
		{
			sButtonX = ( usTextBoxWidth - ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ) / 2;
			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10;

			gMsgBox.uiYESButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_OK ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)OKMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiYESButton, usCursor);

			ForceButtonUnDirty( gMsgBox.uiYESButton );

			gMsgBox.uiNOButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_REHIRE ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX + ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ContractMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiNOButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiNOButton );
		}

		if ( usFlags & MSG_BOX_FLAG_YESNOCONTRACT )
		{
			sButtonX = ( usTextBoxWidth - ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ) / 3;
			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10;

			gMsgBox.uiYESButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_YES ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)YESMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiYESButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiYESButton );


			gMsgBox.uiNOButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_NO ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX + ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NOMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiNOButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiNOButton );

			gMsgBox.uiOKButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_REHIRE ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX + 2 * ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ContractMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiOKButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiOKButton );
		}

		if ( usFlags & MSG_BOX_FLAG_GENERICCONTRACT )
		{
			sButtonX = ( usTextBoxWidth - ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ) / 3;
			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10;

			gMsgBox.uiYESButton = CreateIconAndTextButton( gMsgBox.iButtonImages, gzUserDefinedButton1, FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)YESMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiYESButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiYESButton );


			gMsgBox.uiNOButton = CreateIconAndTextButton( gMsgBox.iButtonImages, gzUserDefinedButton2, FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX + ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NOMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiNOButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiNOButton );

			gMsgBox.uiOKButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_REHIRE ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX + 2 * ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ContractMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiOKButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiOKButton );
		}

		if ( usFlags & MSG_BOX_FLAG_GENERIC_TWO_BUTTONS )
		{
			sButtonX = ( usTextBoxWidth - ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ) / 2;
			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10;

			gMsgBox.uiYESButton = CreateIconAndTextButton( gMsgBox.iButtonImages, gzUserDefinedButton1, FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)YESMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiYESButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiYESButton );


			gMsgBox.uiNOButton = CreateIconAndTextButton( gMsgBox.iButtonImages, gzUserDefinedButton2, FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX + ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NOMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiNOButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiNOButton );
		}

		if ( usFlags & MSG_BOX_FLAG_YESNOLIE )
		{
			sButtonX = ( usTextBoxWidth - ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ) / 3;
			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10;

			gMsgBox.uiYESButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_YES ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)YESMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiYESButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiYESButton );


			gMsgBox.uiNOButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_NO ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX + ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NOMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiNOButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiNOButton );

			gMsgBox.uiOKButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_LIE ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX + 2 * ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)LieMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiOKButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiOKButton );
		}

		if ( usFlags & MSG_BOX_FLAG_OKSKIP )
		{
			sButtonX = ( usTextBoxWidth - ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ) / 2;
			sButtonY = usTextBoxHeight - MSGBOX_BUTTON_HEIGHT - 10;

			gMsgBox.uiYESButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_OK ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)YESMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiYESButton, usCursor);

			ForceButtonUnDirty( gMsgBox.uiYESButton );

			gMsgBox.uiNOButton = CreateIconAndTextButton( gMsgBox.iButtonImages, pMessageStrings[ MSG_SKIP ], FONT12ARIAL,
															ubFontColor, ubFontShadowColor,
															ubFontColor, ubFontShadowColor,
															TEXT_CJUSTIFIED,
															(INT16)(gMsgBox.sX + sButtonX + ( MSGBOX_BUTTON_WIDTH + MSGBOX_BUTTON_X_SEP ) ), (INT16)(gMsgBox.sY + sButtonY ), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)NOMsgBoxCallback );
			SetButtonCursor(gMsgBox.uiNOButton, usCursor);
			ForceButtonUnDirty( gMsgBox.uiNOButton );
		}
	}

	if ( gMsgBox.usFlags & MSG_BOX_FLAG_DROPDOWN )
	{
		DropDownTemplate<DROPDOWNNR_MSGBOX>::getInstance( ).Create( (INT16)(gMsgBox.sX + 15), (INT16)(gMsgBox.sY + 30) );
	}

#if 0
	gMsgBox.fWasPaused = GamePaused();
	if (!gMsgBox.fWasPaused)
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
	FreeMouseCursor( FALSE );

	gfNewMessageBox = TRUE;

	gfInMsgBox			= TRUE;

	return( iId );
}

// this has to be defined. As the MessageBoxScreenHandle() runs all time, there is nothing to do here though
template<>	void	DropDownTemplate<DROPDOWNNR_MSGBOX>::SetRefresh( )
{
	// for now, nothing needed, update happens automatically
}


void MsgBoxClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	///if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	//{
	//	gMsgBox.bHandled = MSG_BOX_RETURN_NO;
	//}
	//
}

void OKMsgBoxCallback(GUI_BUTTON *btn, INT32 reason )
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
		gMsgBox.bHandled = MSG_BOX_RETURN_OK;
	}
	else if ( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fLButtonDown = FALSE;
	}


}

void YESMsgBoxCallback(GUI_BUTTON *btn, INT32 reason )
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
		gMsgBox.bHandled = MSG_BOX_RETURN_YES;
	}
	else if ( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fLButtonDown = FALSE;
	}
}

void NOMsgBoxCallback(GUI_BUTTON *btn, INT32 reason )
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
		gMsgBox.bHandled = MSG_BOX_RETURN_NO;
	}
	else if ( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fLButtonDown = FALSE;
	}
}


void ContractMsgBoxCallback(GUI_BUTTON *btn, INT32 reason )
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
		gMsgBox.bHandled = MSG_BOX_RETURN_CONTRACT;
	}
	else if ( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fLButtonDown = FALSE;
	}
}

void LieMsgBoxCallback(GUI_BUTTON *btn, INT32 reason )
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
		gMsgBox.bHandled = MSG_BOX_RETURN_LIE;
	}
	else if ( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fLButtonDown = FALSE;
	}
}


void NumberedMsgBoxCallback(GUI_BUTTON *btn, INT32 reason )
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP	)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		// OK, exit
		gMsgBox.bHandled = (INT8) MSYS_GetBtnUserData( btn, 0);
	}

}

UINT32	ExitMsgBox( INT8 ubExitCode )
{
	UINT32 uiDestPitchBYTES, uiSrcPitchBYTES;
	UINT8	*pDestBuf, *pSrcBuf;
	SGPPoint pPosition;

	// Delete popup!
	RemoveMercPopupBoxFromIndex( gMsgBox.iBoxId );
	gMsgBox.iBoxId = -1;

	//Delete buttons!
	if ( gMsgBox.usFlags & MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS )
	{
		for (UINT8 i = 0; i < 4; ++i)
		{
			RemoveButton( gMsgBox.uiButton[i] );
		}
	}
	else if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERIC_FOUR_BUTTONS )
	{
		for (UINT8 i = 0; i < 4; ++i)
		{
			RemoveButton( gMsgBox.uiButton[i] );
		}
	}
	else if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERIC_EIGHT_BUTTONS )
	{
		for (UINT8 i = 0; i < 8; ++i)
		{
			RemoveButton( gMsgBox.uiButton[i] );
		}
	}
	else if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS )
	{
		for (UINT8 i = 0; i < 16; ++i)
		{
			RemoveButton( gMsgBox.uiButton[i] );
		}
	}
	// OJW - 20090208 - Add text input box type
	else if (gMsgBox.usFlags & MSG_BOX_FLAG_INPUTBOX)
	{
		// exit text input mode in this screen and clean up text boxes
		KillAllTextInputModes();
	}
	else
	{
		if ( gMsgBox.usFlags & MSG_BOX_FLAG_OK )
		{
			RemoveButton( gMsgBox.uiOKButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_YESNO )
		{
			RemoveButton( gMsgBox.uiYESButton );
			RemoveButton( gMsgBox.uiNOButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_OKCONTRACT )
		{
			RemoveButton( gMsgBox.uiYESButton );
			RemoveButton( gMsgBox.uiNOButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_YESNOCONTRACT )
		{
			RemoveButton( gMsgBox.uiYESButton );
			RemoveButton( gMsgBox.uiNOButton );
			RemoveButton( gMsgBox.uiOKButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERICCONTRACT )
		{
			RemoveButton( gMsgBox.uiYESButton );
			RemoveButton( gMsgBox.uiNOButton );
			RemoveButton( gMsgBox.uiOKButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERIC_TWO_BUTTONS )
		{
			RemoveButton( gMsgBox.uiYESButton );
			RemoveButton( gMsgBox.uiNOButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_YESNOLIE )
		{
			RemoveButton( gMsgBox.uiYESButton );
			RemoveButton( gMsgBox.uiNOButton );
			RemoveButton( gMsgBox.uiOKButton );
		}

		if( gMsgBox.usFlags & MSG_BOX_FLAG_CONTINUESTOP )
		{
			RemoveButton( gMsgBox.uiYESButton );
			RemoveButton( gMsgBox.uiNOButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_OKSKIP )
		{
			RemoveButton( gMsgBox.uiYESButton );
			RemoveButton( gMsgBox.uiNOButton );
		}
	}

	if ( gMsgBox.usFlags & MSG_BOX_FLAG_DROPDOWN )
	{
		DropDownTemplate<DROPDOWNNR_MSGBOX>::getInstance( ).Destroy( );
	}

	// Delete button images
	UnloadButtonImage( gMsgBox.iButtonImages );

#if 0
	if (!gMsgBox.fWasPaused)
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

	gfInMsgBox = FALSE;

	// Call done callback!
	if ( gMsgBox.ExitCallback != NULL )
	{
		(*(gMsgBox.ExitCallback))( ubExitCode );
	}
	
	//if ur in a non gamescreen and DONT want the msg box to use the save buffer, unset gfDontOverRideSaveBuffer in ur callback
	if( ( ( gMsgBox.uiExitScreen != GAME_SCREEN ) || ( fRestoreBackgroundForMessageBox == TRUE ) ) && gfDontOverRideSaveBuffer )
	{
		// restore what we have under here...
		pSrcBuf = LockVideoSurface( gMsgBox.uiSaveBuffer, &uiSrcPitchBYTES);
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES);

		Blt16BPPTo16BPP((UINT16 *)pDestBuf, uiDestPitchBYTES,
					(UINT16 *)pSrcBuf, uiSrcPitchBYTES,
					gMsgBox.sX , gMsgBox.sY,
					0, 0,
					gMsgBox.usWidth, gMsgBox.usHeight );

		UnLockVideoSurface( gMsgBox.uiSaveBuffer );
		UnLockVideoSurface( FRAME_BUFFER );

		InvalidateRegion( gMsgBox.sX, gMsgBox.sY, (INT16)( gMsgBox.sX + gMsgBox.usWidth ), (INT16)( gMsgBox.sY + gMsgBox.usHeight ) );
	}

	fRestoreBackgroundForMessageBox = FALSE;
	gfDontOverRideSaveBuffer = TRUE;

	if( fCursorLockedToArea == TRUE )
	{
		// silversurfer: Why would we only reset cursor position if it is outside of some boundary? If "fCursorLockedToArea" is set we put the cursor back to its old position no matter where it is!
		//GetMousePos( &pPosition );
		//if( ( pPosition.iX > MessageBoxRestrictedCursorRegion.iRight ) || ( pPosition.iX > MessageBoxRestrictedCursorRegion.iLeft ) && ( pPosition.iY < MessageBoxRestrictedCursorRegion.iTop ) && ( pPosition.iY > MessageBoxRestrictedCursorRegion.iBottom ) )
		{
			SimulateMouseMovement( pOldMousePosition.iX , pOldMousePosition.iY );
		}

		fCursorLockedToArea = FALSE;
		RestrictMouseCursor( &MessageBoxRestrictedCursorRegion );
	}

	// Remove region
	MSYS_RemoveRegion(&(gMsgBox.BackRegion) );

	// Remove save buffer!
	DeleteVideoSurfaceFromIndex( gMsgBox.uiSaveBuffer );
	
	switch( gMsgBox.uiExitScreen )
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

	return( gMsgBox.uiExitScreen );
}

UINT32	MessageBoxScreenInit( )
{
	return( TRUE );
}


UINT32	MessageBoxScreenHandle( )
{
	InputAtom	InputEvent;

	if ( gfNewMessageBox )
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
/*
			// Save what we have under here...
			pDestBuf = LockVideoSurface( gMsgBox.uiSaveBuffer, &uiDestPitchBYTES);
			pSrcBuf = LockVideoSurface( FRAME_BUFFER, &uiSrcPitchBYTES);

			Blt16BPPTo16BPP((UINT16 *)pDestBuf, uiDestPitchBYTES,
						(UINT16 *)pSrcBuf, uiSrcPitchBYTES,
						0 , 0,
						gMsgBox.sX , gMsgBox.sY,
						gMsgBox.usWidth, gMsgBox.usHeight );

			UnLockVideoSurface( gMsgBox.uiSaveBuffer );
			UnLockVideoSurface( FRAME_BUFFER );
*/
		}

		gfNewMessageBox = FALSE;

		return( MSG_BOX_SCREEN );
	}



	UnmarkButtonsDirty( );

	// Render the box!
	if ( gMsgBox.fRenderBox )
	{
		if ( gMsgBox.usFlags & MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS )
		{
			for (UINT8 i = 0; i < 4; ++i)
			{
				MarkAButtonDirty( gMsgBox.uiButton[i] );
			}
		}
		else if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERIC_FOUR_BUTTONS )
		{
			for (UINT8 i = 0; i < 4; ++i)
			{
				MarkAButtonDirty( gMsgBox.uiButton[i] );
			}
		}
		else if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERIC_EIGHT_BUTTONS )
		{
			for (UINT8 i = 0; i < 8; ++i)
			{
				MarkAButtonDirty( gMsgBox.uiButton[i] );
			}
		}
		else if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS )
		{
			for (UINT8 i = 0; i < 16; ++i)
			{
				MarkAButtonDirty( gMsgBox.uiButton[i] );
			}
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_OK )
		{
			MarkAButtonDirty( gMsgBox.uiOKButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_CANCEL )
		{
			MarkAButtonDirty( gMsgBox.uiOKButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_YESNO )
		{
			MarkAButtonDirty( gMsgBox.uiYESButton );
			MarkAButtonDirty( gMsgBox.uiNOButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_OKCONTRACT )
		{
			MarkAButtonDirty( gMsgBox.uiYESButton );
			MarkAButtonDirty( gMsgBox.uiNOButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_YESNOCONTRACT )
		{
			MarkAButtonDirty( gMsgBox.uiYESButton );
			MarkAButtonDirty( gMsgBox.uiNOButton );
			MarkAButtonDirty( gMsgBox.uiOKButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERICCONTRACT )
		{
			MarkAButtonDirty( gMsgBox.uiYESButton );
			MarkAButtonDirty( gMsgBox.uiNOButton );
			MarkAButtonDirty( gMsgBox.uiOKButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERIC_TWO_BUTTONS )
		{
			MarkAButtonDirty( gMsgBox.uiYESButton );
			MarkAButtonDirty( gMsgBox.uiNOButton );
		}

		if( gMsgBox.usFlags & MSG_BOX_FLAG_CONTINUESTOP )
		{
			// Exit messagebox
			MarkAButtonDirty( gMsgBox.uiYESButton );
			MarkAButtonDirty( gMsgBox.uiNOButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_YESNOLIE )
		{
			MarkAButtonDirty( gMsgBox.uiYESButton );
			MarkAButtonDirty( gMsgBox.uiNOButton );
			MarkAButtonDirty( gMsgBox.uiOKButton );
		}

		if ( gMsgBox.usFlags & MSG_BOX_FLAG_OKSKIP )
		{
			MarkAButtonDirty( gMsgBox.uiYESButton );
			MarkAButtonDirty( gMsgBox.uiNOButton );
		}


		RenderMercPopUpBoxFromIndex( gMsgBox.iBoxId, gMsgBox.sX, gMsgBox.sY,	FRAME_BUFFER );
		//gMsgBox.fRenderBox = FALSE;
		// ATE: Render each frame...
	}

	// Render buttons
	RenderButtons( );

	if (gMsgBox.usFlags & MSG_BOX_FLAG_INPUTBOX)
	{
		// render text boxes
		RenderAllTextFields(); // textbox system call
	}
	EndFrameBufferRender( );

	// carter, need key shortcuts for clearing up message boxes
	// Check for esc
	while (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
		if( !HandleTextInput( &InputEvent ) && InputEvent.usEvent == KEY_DOWN )
		{
			if( ( InputEvent.usParam == ESC ) || ( InputEvent.usParam == 'n') )
			{
				if ( gMsgBox.usFlags & MSG_BOX_FLAG_YESNO )
				{
						// Exit messagebox
						gMsgBox.bHandled = MSG_BOX_RETURN_NO;
				}
				//OJW - 20090208 - Input Box
				else if( gMsgBox.usFlags & MSG_BOX_FLAG_INPUTBOX )
				{
					// Exit messagebox
					gMsgBox.bHandled = MSG_BOX_RETURN_NO;
					memset(gszMsgBoxInputString,0,sizeof(CHAR16)*255);
				}
			}

			if( InputEvent.usParam == ENTER )
			{
				if ( gMsgBox.usFlags & MSG_BOX_FLAG_YESNO )
				{
					// Exit messagebox
					gMsgBox.bHandled = MSG_BOX_RETURN_YES;
				}
				else if( gMsgBox.usFlags & MSG_BOX_FLAG_OK )
				{
					// Exit messagebox
					gMsgBox.bHandled = MSG_BOX_RETURN_OK;
				}
				else if( gMsgBox.usFlags & MSG_BOX_FLAG_CONTINUESTOP )
				{
					// Exit messagebox
					gMsgBox.bHandled = MSG_BOX_RETURN_OK;
				}
				//OJW - 20090208 - Input Box
				else if( gMsgBox.usFlags & MSG_BOX_FLAG_INPUTBOX )
				{
					// retrieve the string from the text box
					Get16BitStringFromField( 0, gszMsgBoxInputString, 255 ); // these indexes are based on the order created
					// Exit messagebox
					gMsgBox.bHandled = MSG_BOX_RETURN_OK;
				}
			}

			if( InputEvent.usParam == 'o' )
			{
				if( gMsgBox.usFlags & MSG_BOX_FLAG_OK )
				{
					// Exit messagebox
					gMsgBox.bHandled = MSG_BOX_RETURN_OK;
				}
			}
			if( InputEvent.usParam == 'y' )
			{
				if( gMsgBox.usFlags & MSG_BOX_FLAG_YESNO )
				{
					// Exit messagebox
					gMsgBox.bHandled = MSG_BOX_RETURN_YES;
				}
			}

			// box with four buttons
			UINT32 four[8] = {
				'1',
				'2',
				'3',
				'4',
			};

			for ( INT8 i = 0; i < 4; ++i )
			{
				if( InputEvent.usParam == four[i] )
				{
					if ( gMsgBox.usFlags & MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS )
					{
						// Exit messagebox
						gMsgBox.bHandled = 1;
					}
				}
			}

			// box with eight buttons
			UINT32 eighttxt[8] = {
				'1',
				'2',
				'3',
				'4',
				'A',
				'B',
				'C',
				'D',
			};

			for ( INT8 i = 0; i < 8; ++i )
			{
				if( InputEvent.usParam == eighttxt[i] )
				{
					if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERIC_EIGHT_BUTTONS )
					{
						// Exit messagebox
						gMsgBox.bHandled = 1;
					}
				}
			}
				
			// box with sixteen buttons
			UINT32 sixteentxt[16] = {
				'1-A',
				'1-B',
				'1-C',
				'1-D',
				'2-A',
				'2-B',
				'2-C',
				'2-D',
				'3-A',
				'3-B',
				'3-C',
				'3-D',
				'4-A',
				'4-B',
				'4-C',
				'4-D',
			};

			for ( INT8 i = 0; i < 16; ++i )
			{
				if( InputEvent.usParam == sixteentxt[i] )
				{
					if ( gMsgBox.usFlags & MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS )
					{
						// Exit messagebox
						gMsgBox.bHandled = 1;
					}
				}
			}
		}
	}

	if ( gMsgBox.usFlags & MSG_BOX_FLAG_DROPDOWN )
	{
		DropDownTemplate<DROPDOWNNR_MSGBOX>::getInstance( ).Display( );
	}

	if ( gMsgBox.bHandled )
	{
		SetRenderFlags( RENDER_FLAG_FULL );
		return( ExitMsgBox( gMsgBox.bHandled ) );
	}

	return( MSG_BOX_SCREEN );
}

UINT32	MessageBoxScreenShutdown(	)
{
	return( FALSE );
}


// a basic box that don't care what screen we came from
void DoScreenIndependantMessageBox( const STR16 zString, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect CenteringRect= {0, 0, SCREEN_WIDTH, INV_INTERFACE_START_Y };
	DoScreenIndependantMessageBoxWithRect(	zString, usFlags, ReturnCallback, &CenteringRect );
}

// a basic box that don't care what screen we came from
void DoUpperScreenIndependantMessageBox( const STR16 zString, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect CenteringRect= {0, 0, SCREEN_WIDTH, INV_INTERFACE_START_Y / 2 };
	DoScreenIndependantMessageBoxWithRect( zString, usFlags, ReturnCallback, &CenteringRect );
}

// a basic box that don't care what screen we came from
void DoLowerScreenIndependantMessageBox( STR16 zString, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect CenteringRect= {0, INV_INTERFACE_START_Y / 2, SCREEN_WIDTH, INV_INTERFACE_START_Y };
	DoScreenIndependantMessageBoxWithRect( zString, usFlags, ReturnCallback, &CenteringRect );
}


void DoScreenIndependantMessageBoxWithRect( const STR16 zString, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect )
{

	/// which screen are we in?

	// Map Screen (excluding AI Viewer)
#ifdef JA2BETAVERSION
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) && ( guiCurrentScreen != AIVIEWER_SCREEN ) )
#else
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
#endif
	{

		// auto resolve is a special case
		if( guiCurrentScreen == AUTORESOLVE_SCREEN )
		{
			DoMessageBox( MSG_BOX_BASIC_STYLE, zString, AUTORESOLVE_SCREEN, usFlags, ReturnCallback, pCenteringRect );
		}
		else
		{
			// set up for mapscreen
			DoMapMessageBoxWithRect( MSG_BOX_BASIC_STYLE, zString, MAP_SCREEN, usFlags, ReturnCallback, pCenteringRect );
		}
	}

	//Laptop
	else if( guiCurrentScreen == LAPTOP_SCREEN )
	{
		// set up for laptop
		DoLapTopSystemMessageBoxWithRect( MSG_BOX_LAPTOP_DEFAULT, zString, LAPTOP_SCREEN, usFlags, ReturnCallback, pCenteringRect );
	}

	//Save Load Screen
	else if( guiCurrentScreen == SAVE_LOAD_SCREEN )
	{
		DoSaveLoadMessageBoxWithRect( MSG_BOX_BASIC_STYLE, zString, SAVE_LOAD_SCREEN, usFlags, ReturnCallback, pCenteringRect );
	}

	//Options Screen
	else if( guiCurrentScreen == OPTIONS_SCREEN )
	{
		DoOptionsMessageBoxWithRect( MSG_BOX_BASIC_STYLE, zString, OPTIONS_SCREEN, usFlags, ReturnCallback, pCenteringRect );
	}

	// Tactical
	else if( guiCurrentScreen == GAME_SCREEN )
	{
		DoMessageBox(	MSG_BOX_BASIC_STYLE, zString,	guiCurrentScreen, usFlags,	ReturnCallback,	pCenteringRect );
	}
}

UINT16 GetMSgBoxButtonWidth( INT32 iButtonImage )
{
	return( GetWidthOfButtonPic( (UINT16)iButtonImage, ButtonPictures[iButtonImage].OnNormal ) );
}
