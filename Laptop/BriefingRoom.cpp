#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "Laptop All.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "timer control.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Encrypted File.h"
	#include "email.h"
	#include "Game Clock.h"
	#include "Text.h"
	#include "soldier profile type.h"
#endif

#include "BriefingRoom_Data.h"
#include "BriefingRoom.h"

// Link Images
#define	BRIEFINGROOM_BUTTON_SIZE_X						205
#define	BRIEFINGROOM_BUTTON_SIZE_Y						19

#define	BRIEFINGROOM_COPYRIGHT_X						iScreenWidthOffset + 160
#define	BRIEFINGROOM_COPYRIGHT_Y						iScreenHeightOffset + 415 // + LAPTOP_SCREEN_WEB_DELTA_Y
#define	BRIEFINGROOM_COPYRIGHT_WIDTH					400
#define	BRIEFINGROOM_COPYRIGHT_GAP						9

#define	IMAGE_OFFSET_X						LAPTOP_SCREEN_UL_X
#define	IMAGE_OFFSET_Y						LAPTOP_SCREEN_WEB_UL_Y


//262, 28

// RustBackGround
#define	RUSTBACKGROUND_SIZE_X				125
#define	RUSTBACKGROUND_SIZE_Y				100

#define	RUSTBACKGROUND_1_X					IMAGE_OFFSET_X
#define	RUSTBACKGROUND_1_Y					IMAGE_OFFSET_Y

#define	BRIEFINGROOM_FONT12ARIAL			FONT12ARIAL
#define	BRIEFINGROOM_FONT_MCOLOR_WHITE		FONT_MCOLOR_WHITE

#define	BRIEFINGROOM_BUTTONS_DEF 			1
#define	BRIEFINGROOM_SUBTITLE_Y				iScreenHeightOffset + 150 + LAPTOP_SCREEN_WEB_DELTA_Y
#define BRIEFINGROOM_CONTENTBUTTON_X		iScreenWidthOffset + 259
#define BRIEFINGROOM_CONTENTBUTTON_Y		BRIEFINGROOM_SUBTITLE_Y
#define BRIEFINGROOM_TOC_X					BRIEFINGROOM_CONTENTBUTTON_X
#define BRIEFINGROOM_TOC_Y					5
#define	BRIEFINGROOM_TOC_GAP_Y				25	

UINT32		guiRustBriefingRoomBackGround;
UINT32		guiRustBriefingRoomLogoAim;

MOUSE_REGION	gSelectedBriefingRoomTocMenuRegion[ 6 ];
UINT32			guiContentButtonBriefingRoom;

UINT32 guiBRIEFINGROOMACTIVATIONINDENT;

void SelectBriefingRoomLocationButton(MOUSE_REGION * pRegion, INT32 iReason );
void SelectBriefingRoomCharacterButton(MOUSE_REGION * pRegion, INT32 iReason );
void SelectBriefingRoomQuestsButton(MOUSE_REGION * pRegion, INT32 iReason );
void SelectBriefingRoomInentoryButton(MOUSE_REGION * pRegion, INT32 iReason );

void		LaptopInitBriefingRoom();

BOOLEAN		fFirstTimeInBriefingRoom = TRUE;

BOOLEAN DrawBriefingRoomLogoAim();

//------------
void DisplayPlayerActivationBriefingRoomString( void );
void ProcessPlayerInputActivationBriefingRoomString( void );
void GetPlayerKeyBoardInputForBriefingRoomHomePage( void );
void HandleBriefingRoomTextEvent( UINT32 uiKey );
void DisplayActivationBriefingRoomStringCursor( void );

INT32 BriefingRoomGlowColorsList[][3] ={
	{ 0,0,0 },
	{ 0,25,0 },
	{ 0,50,0 },
	{ 0,75,0 },
	{ 0,100,0 },
	{ 0,125,0 },
	{ 0,150,0 },
	{ 0,175,0 },
	{ 0,200,0 },
	{ 0,225,0 },
	{ 0,255,0 },
};

// btn callbacks
void BtnBriefingRoomAboutUsCallback(GUI_BUTTON *btn,INT32 reason);

void ResetActivationBriefingRoomStringTextBox(void);

// position defines
#define IMP_PLAYER_ACTIVATION_STRING_X LAPTOP_SCREEN_UL_X + 203//261
#define IMP_PLAYER_ACTIVATION_STRING_Y LAPTOP_SCREEN_WEB_UL_Y + 266 //336
#define CURSOR_Y IMP_PLAYER_ACTIVATION_STRING_Y - 5
#define CURSOR_HEIGHT GetFontHeight( FONT14ARIAL ) + 6


// the player activation string
CHAR16 pPlayerBriefingRoomActivationString[32];

// position within player activation string
INT32 iStringBriefingRoomPos = 0;
UINT16 uiCursorBriefingRoomPosition = IMP_PLAYER_ACTIVATION_STRING_X;


// has a new char been added or deleted?
BOOLEAN fNewCharInActivationBriefingRoomString = FALSE;
//------------

void GameInitBriefingRoom()
{
	LaptopInitBriefingRoom();
}

BOOLEAN EnterBriefingRoom()
{
	VOBJECT_DESC    VObjectDesc;
  
    //-----------------------------------------
	// upon entry to Imp home page
	memset(pPlayerBriefingRoomActivationString, 0, sizeof(pPlayerBriefingRoomActivationString));

	// reset string position
	iStringBriefingRoomPos =0;

	// reset activation	cursor position
	uiCursorBriefingRoomPosition = IMP_PLAYER_ACTIVATION_STRING_X;

	// we have now vsisited IMP, reset fact we haven't
	fNotVistedImpYet = FALSE;
	//------------------------

	LaptopInitBriefingRoom();

	InitBriefingRoomDefaults();
	
	// load the Rust bacground graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	//FilenameForBPP("LAPTOP\\rustbackground.sti", VObjectDesc.ImageFile);
	FilenameForBPP("BriefingRoom\\aimlogo_big.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiRustBriefingRoomLogoAim));
/*
	// load the MemberShipcard graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("BriefingRoom\\CONTENTBUTTON.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiContentButtonBriefingRoom));
*/	
	// this procedure will load the activation indent into memory
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\ActivationIndent.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiBRIEFINGROOMACTIVATIONINDENT));

	//** Mouse Regions **
/*	
	usPosY = BRIEFINGROOM_CONTENTBUTTON_Y + 120;
	for(i=0; i<BRIEFINGROOM_BUTTONS_DEF; i++)
	{
			if ( i == 0 )
		MSYS_DefineRegion( &gSelectedBriefingRoomTocMenuRegion[i], BRIEFINGROOM_TOC_X, usPosY, (UINT16)(BRIEFINGROOM_TOC_X + BRIEFINGROOM_BUTTON_SIZE_X), (UINT16)(usPosY + BRIEFINGROOM_BUTTON_SIZE_Y), MSYS_PRIORITY_HIGH,
									CURSOR_WWW, MSYS_NO_CALLBACK, SelectBriefingRoomLocationButton);												
									
		MSYS_AddRegion(&gSelectedBriefingRoomTocMenuRegion[i]);
		
		usPosY += BRIEFINGROOM_TOC_GAP_Y;
	}
*/
	fFirstTimeInBriefingRoom = FALSE;
	
	RenderBriefingRoom();

	return( TRUE );
}

void LaptopInitBriefingRoom()
{

}

void ExitBriefingRoom()
{	
	RemoveBriefingRoomDefaults();

	//DeleteVideoObjectFromIndex(guiContentButtonBriefingRoom);
	DeleteVideoObjectFromIndex(guiRustBriefingRoomLogoAim);
	
	// remove activation indent symbol
	DeleteVideoObjectFromIndex( guiBRIEFINGROOMACTIVATIONINDENT );
}

void HandleBriefingRoom()
{
	// handle keyboard input for this screen
	GetPlayerKeyBoardInputForBriefingRoomHomePage( );

	// has a new char been added to activation string
	if( fNewCharInActivationBriefingRoomString )
	{
		// display string
	DisplayPlayerActivationBriefingRoomString( );
	}

	// render the cursor
	DisplayActivationBriefingRoomStringCursor( );
}

void RenderBriefingRoom()
{
	UINT16		usWidth=0;
	
	HVOBJECT hHandle;
	
	DrawBriefingRoomDefaults();
	
	DrawBriefingRoomLogoAim();
	
	// get the video object
	GetVideoObject(&hHandle, guiBRIEFINGROOMACTIVATIONINDENT);
	// blt to sX, sY relative to upper left corner
	BltVideoObject(FRAME_BUFFER, hHandle, 0, LAPTOP_SCREEN_UL_X + 200, LAPTOP_SCREEN_WEB_UL_Y + 260 , VO_BLT_SRCTRANSPARENCY,NULL);
	
	DisplayBriefingRoomSlogan();

	DisplayBriefingRoomCopyright();

	RenderWWWProgramTitleBar( );	
	
	// render the	activation string
	//-------------
	DisplayPlayerActivationBriefingRoomString( );
	//-------------

  InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void SelectBriefingRoomLocationButton(MOUSE_REGION * pRegion, INT32 iReason )
{ 
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{

	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		bBriefingRoom  = TRUE;
		if(!fFirstTimeInBriefingRoom) guiCurrentLaptopMode = LAPTOP_MODE_BRIEFING_ROOM;	
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	} 
}

BOOLEAN InitBriefingRoomDefaults()
{
  VOBJECT_DESC    VObjectDesc;

	// load the Rust bacground graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	//FilenameForBPP("LAPTOP\\rustbackground.sti", VObjectDesc.ImageFile);
	FilenameForBPP("BriefingRoom\\background.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiRustBriefingRoomBackGround));

	return(TRUE);
}

BOOLEAN RemoveBriefingRoomDefaults()
{
	DeleteVideoObjectFromIndex(guiRustBriefingRoomBackGround);

	return(TRUE);
}

BOOLEAN DrawBriefingRoomLogoAim()
{
	HVOBJECT hLogoEncyklopedia;

	GetVideoObject(&hLogoEncyklopedia, guiRustBriefingRoomLogoAim);
	BltVideoObject(FRAME_BUFFER, hLogoEncyklopedia, 0,BRIEFINGROOM_TOC_X, RUSTBACKGROUND_1_Y + 40, VO_BLT_SRCTRANSPARENCY,NULL);

	return(TRUE);
}

BOOLEAN DrawBriefingRoomDefaults()
{
	HVOBJECT hRustBackGroundHandle;	

	// Blt the rust background
	GetVideoObject(&hRustBackGroundHandle, guiRustBriefingRoomBackGround);
	BltVideoObject(FRAME_BUFFER, hRustBackGroundHandle, 0,RUSTBACKGROUND_1_X, RUSTBACKGROUND_1_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	return(TRUE);
}

BOOLEAN DisplayBriefingRoomSlogan()
{

	return(TRUE);
}

BOOLEAN DisplayBriefingRoomCopyright()
{


	return(TRUE);
}


void DisplayPlayerActivationBriefingRoomString( void )
{

HVOBJECT hHandle;

	// this function will grab the string that the player will enter for activation

	// player gone too far, move back
	if(iStringBriefingRoomPos > 64)
	{
		iStringBriefingRoomPos = 64;
	}

	// restore background
	//RenderActivationIndent( 257, 328 );
	// get the video object
	GetVideoObject(&hHandle, guiBRIEFINGROOMACTIVATIONINDENT);

	// blt to sX, sY relative to upper left corner
	BltVideoObject(FRAME_BUFFER, hHandle, 0, LAPTOP_SCREEN_UL_X + 200, LAPTOP_SCREEN_WEB_UL_Y + 260 , VO_BLT_SRCTRANSPARENCY,NULL);

	// setup the font stuff
	SetFont( FONT14ARIAL );
	SetFontForeground( 184 );
	SetFontBackground(FONT_BLACK);


	// reset shadow
	SetFontShadow(DEFAULT_SHADOW);
	mprintf(IMP_PLAYER_ACTIVATION_STRING_X, IMP_PLAYER_ACTIVATION_STRING_Y, pPlayerBriefingRoomActivationString);


	fNewCharInActivationBriefingRoomString = FALSE;
	fReDrawScreenFlag = TRUE;
	return;
}

void DisplayActivationBriefingRoomStringCursor( void )
{
	// this procdure will draw the activation string cursor on the screen at position cursorx cursory
	UINT32 uiDestPitchBYTES;
	static UINT32 uiBaseTime = 0;
	UINT32 uiDeltaTime = 0;
	UINT8 *pDestBuf;
	static UINT32 iCurrentState = 0;
	static BOOLEAN fIncrement = TRUE;

	if(uiBaseTime == 0)
	{
		uiBaseTime = GetJA2Clock();
	}

	// get difference
	uiDeltaTime = GetJA2Clock() - uiBaseTime;

	// if difference is long enough, rotate colors
	if(uiDeltaTime > MIN_GLOW_DELTA)
	{
		if( iCurrentState == 10)
		{
		// start rotating downward
			fIncrement = FALSE;
		}
		if( iCurrentState == 0)
		{
			// rotate colors upward
			fIncrement = TRUE;
		}
		// if increment upward, increment iCurrentState
	if(fIncrement)
		{
			iCurrentState++;
		}
		else
		{
			// else downwards
			iCurrentState--;
		}
	// reset basetime to current clock
		uiBaseTime = GetJA2Clock( );
	}

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );


	// draw line in current state
	LineDraw(TRUE, (UINT16) uiCursorBriefingRoomPosition, CURSOR_Y, (UINT16)uiCursorBriefingRoomPosition, CURSOR_Y + CURSOR_HEIGHT, Get16BPPColor( FROMRGB( BriefingRoomGlowColorsList[ iCurrentState ][ 0 ], BriefingRoomGlowColorsList[ iCurrentState ][ 1 ], BriefingRoomGlowColorsList[ iCurrentState ][ 2 ] ) ),
			pDestBuf );

	// unlock frame buffer
	UnLockVideoSurface( FRAME_BUFFER );

	InvalidateRegion((UINT16) uiCursorBriefingRoomPosition , CURSOR_Y , (UINT16)uiCursorBriefingRoomPosition + 1, CURSOR_Y + CURSOR_HEIGHT + 1);


	return;
}



void GetPlayerKeyBoardInputForBriefingRoomHomePage( void )
{
	InputAtom					InputEvent;
	POINT	MousePos;

	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	while (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
		// HOOK INTO MOUSE HOOKS
		/*
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
	}
*/
		if(	!HandleTextInput( &InputEvent ) && (InputEvent.usEvent == KEY_DOWN || InputEvent.usEvent == KEY_REPEAT || InputEvent.usEvent == KEY_UP ) )
		{
		switch( InputEvent.usParam )
			{
			case (( ENTER ) ):
					if(( InputEvent.usEvent == KEY_UP ) )
					{
						// return hit, check to see if current player activation string is a valid one
						ProcessPlayerInputActivationBriefingRoomString( );

					fNewCharInActivationBriefingRoomString = TRUE;
					}
				break;
				case (( ESC )):
			HandleLapTopESCKey();		// WANNE: Fix for proper closing of the IMP laptop page
			LeaveLapTopScreen( );
				break;
				default:
					if(InputEvent.usEvent == KEY_DOWN || InputEvent.usEvent == KEY_REPEAT )
					{
						HandleBriefingRoomTextEvent( InputEvent.usParam );
					}
				break;
			}
		}
	}

	return;
}


void HandleBriefingRoomTextEvent( UINT32 uiKey )
{
	// this function checks to see if a letter or a backspace was pressed, if so, either put char to screen
	// or delete it
	switch( uiKey )
	{
	case ( BACKSPACE ):

			if( iStringBriefingRoomPos >= 0 )
			{

				if( iStringBriefingRoomPos > 0 )
				{
					// decrement iStringBriefingRoomPosition
					iStringBriefingRoomPos-=1;
				}

				// null out char
		pPlayerBriefingRoomActivationString[iStringBriefingRoomPos ] = 0;

				// move back cursor
		uiCursorBriefingRoomPosition = StringPixLength( pPlayerBriefingRoomActivationString, FONT14ARIAL ) + IMP_PLAYER_ACTIVATION_STRING_X;



				// string has been altered, redisplay
		fNewCharInActivationBriefingRoomString = TRUE;

		}

		break;

	default:
	 if( uiKey >= 'A' && uiKey <= 'Z' ||
					uiKey >= 'a' && uiKey <= 'z' ||
					uiKey >= '0' && uiKey <= '9' ||
					uiKey == '_' || uiKey == '.' || uiKey ==' ')
			{
				// if the current string position is at max or great, do nothing
		if( iStringBriefingRoomPos >= 8 )
		{
					break;
				}
				else
				{
					if(iStringBriefingRoomPos < 0 )
					{
						iStringBriefingRoomPos = 0;
					}
			// valid char, capture and convert to CHAR16
			pPlayerBriefingRoomActivationString[iStringBriefingRoomPos] = ( CHAR16 )uiKey;

					// null out next char position
					pPlayerBriefingRoomActivationString[iStringBriefingRoomPos + 1] = 0;

					// move cursor position ahead
			uiCursorBriefingRoomPosition = StringPixLength( pPlayerBriefingRoomActivationString, FONT14ARIAL ) + IMP_PLAYER_ACTIVATION_STRING_X;

					// increment string position
					iStringBriefingRoomPos +=1;

				// string has been altered, redisplay
			fNewCharInActivationBriefingRoomString = TRUE;

				}

			}

		break;

	}


	return;
}

void ProcessPlayerInputActivationBriefingRoomString( void )
{
	// prcess string to see if it matches activation string
	char charPlayerActivationString[32];
	wcstombs(charPlayerActivationString,pPlayerBriefingRoomActivationString,32);

	if( ( ( wcscmp(pPlayerBriefingRoomActivationString, L"SN5631") == 0 ) || ( wcscmp(pPlayerBriefingRoomActivationString, L"sn5631") == 0 ) ) ) //&&( LaptopSaveInfo.gfNewGameLaptop < 2 ) )
	{
	
		//bEncyclopediaLocation = FALSE;
		//bEncyclopediaCharacter = FALSE;
		//bEncyclopediaInventory = FALSE;
		//bEncyclopediaQuests  = FALSE;
		//bBriefingRoom  = FALSE;
		//bBriefingRoomSpecialMission = TRUE;
		
		ResetTemp();
		CopyToTemp (gBriefingRoomData, TRUE, 0 , -1, FALSE );
		
		if(!fFirstTimeInBriefingRoom) guiCurrentLaptopMode = LAPTOP_MODE_BRIEFING_ROOM_ENTER;	

		//	DoLapTopMessageBox( MSG_BOX_LAPTOP_DEFAULT, AimPopUpText[ AIM_MEMBER_ALREADY_HAVE_MAX_MERCS ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
	}

	return;
}

void ResetActivationBriefingRoomStringTextBox(void)
{
	// Reset activation text box
	for (int i = 0; i < iStringBriefingRoomPos; i++)
	{
		pPlayerBriefingRoomActivationString[i] = 0;
	}

	iStringBriefingRoomPos = 0;

	uiCursorBriefingRoomPosition = StringPixLength( pPlayerBriefingRoomActivationString, FONT14ARIAL ) + IMP_PLAYER_ACTIVATION_STRING_X;
	DisplayPlayerActivationBriefingRoomString();
	DisplayActivationBriefingRoomStringCursor();
}
