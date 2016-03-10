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
#include "BriefingRoomM.h"

// Link Images
#define	BRIEFINGROOM_MISSION_BUTTON_SIZE_X						121
#define	BRIEFINGROOM_MISSION_BUTTON_SIZE_Y						42

#define	BRIEFINGROOM_MISSION_COPYRIGHT_X						iScreenWidthOffset + 160
#define	BRIEFINGROOM_MISSION_COPYRIGHT_Y						iScreenHeightOffset + 415 // + LAPTOP_SCREEN_WEB_DELTA_Y
#define	BRIEFINGROOM_MISSION_COPYRIGHT_WIDTH					400
#define	BRIEFINGROOM_MISSION_COPYRIGHT_GAP						9

#define	IMAGE_OFFSET_X						LAPTOP_SCREEN_UL_X
#define	IMAGE_OFFSET_Y						LAPTOP_SCREEN_WEB_UL_Y


//262, 28

// RustBackGround
#define	RUSTBACKGROUND_SIZE_X				125
#define	RUSTBACKGROUND_SIZE_Y				100

#define	RUSTBACKGROUND_1_X					IMAGE_OFFSET_X
#define	RUSTBACKGROUND_1_Y					IMAGE_OFFSET_Y

#define	BRIEFINGROOM_MISSION_FONT12ARIAL			FONT12ARIAL
#define	BRIEFINGROOM_MISSION_FONT_MCOLOR_WHITE		FONT_MCOLOR_WHITE

#define	BRIEFINGROOM_MISSION_BUTTONS_DEF 			1
#define	BRIEFINGROOM_MISSION_SUBTITLE_Y				iScreenHeightOffset + 150 + LAPTOP_SCREEN_WEB_DELTA_Y
#define BRIEFINGROOM_MISSION_CONTENTBUTTON_X		iScreenWidthOffset + 259
#define BRIEFINGROOM_MISSION_CONTENTBUTTON_Y		BRIEFINGROOM_MISSION_SUBTITLE_Y
#define BRIEFINGROOM_MISSION_TOC_X					BRIEFINGROOM_MISSION_CONTENTBUTTON_X
#define BRIEFINGROOM_MISSION_TOC_Y					5
#define	BRIEFINGROOM_MISSION_TOC_GAP_Y				25	

#define		BRIEFINGROOM_MISSION_LOCATION_BOX_X						LAPTOP_SCREEN_UL_X + (500-BRIEFINGROOM_MISSION_LOCATION_BOX_WIDTH)/2
#define		BRIEFINGROOM_MISSION_LOCATION_BOX_Y						iScreenHeightOffset + 260 + LAPTOP_SCREEN_WEB_DELTA_Y
#define		EBRIEFINGROOM_MISSION_LOCATION_BOX_SHADOW_GAP			4
#define		BRIEFINGROOM_MISSION_LOCATION_BOX_WIDTH					400//309
#define		BRIEFINGROOM_MISSION_LOCATION_BOX_SHADOW_GAP			4

#define		BRIEFINGROOM_MISSION_BOX_FONT				FONT10ARIAL
#define		BRIEFINGROOM_MISSION_BOX_COLOR				FONT_MCOLOR_WHITE
#define		BRIEFINGROOM_MISSION_PAGE_FONT				FONT10ARIAL
#define		BRIEFINGROOM_MISSION_BOX_X					LAPTOP_SCREEN_UL_X + (500-BRIEFINGROOM_MISSION_BOX_WIDTH)/2
#define		BRIEFINGROOM_MISSION_BOX_Y					iScreenHeightOffset + 260 + LAPTOP_SCREEN_WEB_DELTA_Y
#define		BRIEFINGROOM_MISSION_BOX_WIDTH				400
#define		BRIEFINGROOM_MISSION_BOX_TEXT_WIDTH			250
#define		BRIEFINGROOM_MISSION_BOX_DESC_X				BRIEFINGROOM_MISSION_BOX_X + 8
#define		BRIEFINGROOM_MISSION_BOX_DESC_Y				BRIEFINGROOM_MISSION_PANEL_Y + BRIEFINGROOM_MISSION_PANEL_HEIGHT + 5
#define		BRIEFINGROOM_MISSION_PANEL_HEIGHT			0
#define		BRIEFINGROOM_MISSION_PANEL_Y				BRIEFINGROOM_MISSION_BOX_Y + 6

UINT32		guiRustBriefingRoomEnterBackGround;
UINT32		guiRustBriefingRoomEnterLogoAim;

MOUSE_REGION	gSelectedBriefingRoomEnterTocMenuRegion[ 6 ];
UINT32			guiContentButtonBriefingRoomEnter;

UINT32 guiBRIEFINGROOM_MISSIONACTIVATIONINDENT;

void SelectBriefingRoomEnterLocationButton(MOUSE_REGION * pRegion, INT32 iReason );
void SelectBriefingRoomEnterCharacterButton(MOUSE_REGION * pRegion, INT32 iReason );
void SelectBriefingRoomEnterQuestsButton(MOUSE_REGION * pRegion, INT32 iReason );
void SelectBriefingRoomEnterInentoryButton(MOUSE_REGION * pRegion, INT32 iReason );

void		LaptopInitBriefingRoomEnter();

BOOLEAN		fFirstTimeInBriefingRoomEnter = TRUE;

BOOLEAN DrawBriefingRoomEnterLogoAim();

//------------
void DisplayPlayerActivationBriefingRoomEnterString( void );
void ProcessPlayerInputActivationBriefingRoomEnterString( void );
void GetPlayerKeyBoardInputForBriefingRoomEnterHomePage( void );
void HandleBriefingRoomEnterTextEvent( UINT32 uiKey );
void DisplayActivationBriefingRoomEnterStringCursor( void );

INT32 BriefingRoomEnterGlowColorsList[][3] ={
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
void BtnBriefingRoomEnterAboutUsCallback(GUI_BUTTON *btn,INT32 reason);

void ResetActivationBriefingRoomEnterStringTextBox(void);

// position defines
#define IMP_PLAYER_ACTIVATION_STRING_X LAPTOP_SCREEN_UL_X + 103//261
#define IMP_PLAYER_ACTIVATION_STRING_Y LAPTOP_SCREEN_WEB_UL_Y + 246 //336
#define CURSOR_Y IMP_PLAYER_ACTIVATION_STRING_Y - 5
#define CURSOR_HEIGHT GetFontHeight( FONT14ARIAL ) + 6


// the player activation string
CHAR16 pPlayerBriefingRoomEnterActivationString[32];

// position within player activation string
INT32 iStringBriefingRoomEnterPos = 0;
UINT16 uiCursorBriefingRoomEnterPosition = IMP_PLAYER_ACTIVATION_STRING_X;


// has a new char been added or deleted?
BOOLEAN fNewCharInActivationBriefingRoomEnterString = FALSE;
//------------

void GameInitBriefingRoomEnter()
{
	LaptopInitBriefingRoomEnter();
}

BOOLEAN EnterBriefingRoomEnter()
{
  VOBJECT_DESC    VObjectDesc;
  UINT16		i, usPosY;
  
    //-----------------------------------------
	// upon entry to Imp home page
	memset(pPlayerBriefingRoomEnterActivationString, 0, sizeof(pPlayerBriefingRoomEnterActivationString));

	// reset string position
	iStringBriefingRoomEnterPos =0;

	// reset activation	cursor position
	uiCursorBriefingRoomEnterPosition = IMP_PLAYER_ACTIVATION_STRING_X;

	// we have now vsisited IMP, reset fact we haven't
	fNotVistedImpYet = FALSE;
	//------------------------

	LaptopInitBriefingRoomEnter();

	InitBriefingRoomEnterDefaults();
	
	// load the Rust bacground graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	//FilenameForBPP("LAPTOP\\rustbackground.sti", VObjectDesc.ImageFile);
	FilenameForBPP("BriefingRoom\\aimlogo.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiRustBriefingRoomEnterLogoAim));

	// load the MemberShipcard graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("BriefingRoom\\BUTTONS.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiContentButtonBriefingRoomEnter));
	
	// this procedure will load the activation indent into memory
	//off
	//VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	//FilenameForBPP("LAPTOP\\ActivationIndent.sti", VObjectDesc.ImageFile);
	//CHECKF(AddVideoObject(&VObjectDesc, &guiBRIEFINGROOM_MISSIONACTIVATIONINDENT));

	//** Mouse Regions **
	
	usPosY = BRIEFINGROOM_MISSION_CONTENTBUTTON_Y + 120;
	for(i=0; i<BRIEFINGROOM_MISSION_BUTTONS_DEF; i++)
	{
			if ( i == 0 )
		MSYS_DefineRegion( &gSelectedBriefingRoomEnterTocMenuRegion[i], BRIEFINGROOM_MISSION_TOC_X+120, usPosY, (UINT16)(BRIEFINGROOM_MISSION_TOC_X+120 + BRIEFINGROOM_MISSION_BUTTON_SIZE_X), (UINT16)(usPosY + BRIEFINGROOM_MISSION_BUTTON_SIZE_Y), MSYS_PRIORITY_HIGH,
									CURSOR_WWW, MSYS_NO_CALLBACK, SelectBriefingRoomEnterLocationButton);												
									
		MSYS_AddRegion(&gSelectedBriefingRoomEnterTocMenuRegion[i]);
		
		usPosY += BRIEFINGROOM_MISSION_TOC_GAP_Y;
	}

	fFirstTimeInBriefingRoomEnter = FALSE;
	
	RenderBriefingRoomEnter();

	return( TRUE );
}

void LaptopInitBriefingRoomEnter()
{

}

void ExitBriefingRoomEnter()
{
	UINT16 i;
	
	RemoveBriefingRoomEnterDefaults();

	DeleteVideoObjectFromIndex(guiContentButtonBriefingRoomEnter);
	DeleteVideoObjectFromIndex(guiRustBriefingRoomEnterLogoAim);

	
	// Remove Mouse Regions
	for(i=0; i<BRIEFINGROOM_MISSION_BUTTONS_DEF; i++)
		MSYS_RemoveRegion( &gSelectedBriefingRoomEnterTocMenuRegion[i]);
		
	// remove activation indent symbol
	//off
	//DeleteVideoObjectFromIndex( guiBRIEFINGROOM_MISSIONACTIVATIONINDENT );

}

void HandleBriefingRoomEnter()
{
	// handle keyboard input for this screen
	GetPlayerKeyBoardInputForBriefingRoomEnterHomePage( );

	/* off
	// has a new char been added to activation string
	if( fNewCharInActivationBriefingRoomEnterString )
	{
		// display string
	DisplayPlayerActivationBriefingRoomEnterString( );
	}

	// render the cursor
	DisplayActivationBriefingRoomEnterStringCursor( ); 
	*/
}

void RenderBriefingRoomEnter()
{
    UINT16		i, idText, usPosY;
	UINT16		usHeight;
	HVOBJECT	hContentButtonHandle;  
	UINT16		usWidth=0;
	
	HVOBJECT hHandle;
	
	DrawBriefingRoomEnterDefaults();
	
	DrawBriefingRoomEnterLogoAim();
	
	ShadowVideoSurfaceRect( FRAME_BUFFER, BRIEFINGROOM_MISSION_LOCATION_BOX_X+BRIEFINGROOM_MISSION_LOCATION_BOX_SHADOW_GAP+150, BRIEFINGROOM_MISSION_LOCATION_BOX_Y + 13 + BRIEFINGROOM_MISSION_LOCATION_BOX_SHADOW_GAP - 227 , BRIEFINGROOM_MISSION_LOCATION_BOX_X + 400 + BRIEFINGROOM_MISSION_LOCATION_BOX_SHADOW_GAP, BRIEFINGROOM_MISSION_LOCATION_BOX_Y + BRIEFINGROOM_MISSION_LOCATION_BOX_SHADOW_GAP-42);

	DisplayWrappedString(BRIEFINGROOM_MISSION_BOX_DESC_X + 150, BRIEFINGROOM_MISSION_BOX_DESC_Y - 210, BRIEFINGROOM_MISSION_BOX_TEXT_WIDTH, 6, BRIEFINGROOM_MISSION_BOX_FONT, BRIEFINGROOM_MISSION_BOX_COLOR, pSectorPageText[6], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

	
	
	GetVideoObject(&hContentButtonHandle, guiContentButtonBriefingRoomEnter);

	usHeight = GetFontHeight(BRIEFINGROOM_MISSION_FONT12ARIAL);
	usPosY = BRIEFINGROOM_MISSION_CONTENTBUTTON_Y + 120;
	for(i=0; i<BRIEFINGROOM_MISSION_BUTTONS_DEF; i++)
	{
	
	    idText = i + 16;
		BltVideoObject(FRAME_BUFFER, hContentButtonHandle, 0,BRIEFINGROOM_MISSION_TOC_X+120, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);

		usWidth = StringPixLength(pMenuStrings[idText], BRIEFINGROOM_MISSION_FONT12ARIAL);
		DrawTextToScreen(pMenuStrings[idText], BRIEFINGROOM_MISSION_TOC_X+120, (UINT16)(usPosY + BRIEFINGROOM_MISSION_TOC_Y+10), BRIEFINGROOM_MISSION_BUTTON_SIZE_X, BRIEFINGROOM_MISSION_FONT12ARIAL, BRIEFINGROOM_MISSION_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
		
		usPosY += BRIEFINGROOM_MISSION_TOC_GAP_Y;
	}
	
	// get the video object
	//off
	//GetVideoObject(&hHandle, guiBRIEFINGROOM_MISSIONACTIVATIONINDENT);
	// blt to sX, sY relative to upper left corner
	//off
	//BltVideoObject(FRAME_BUFFER, hHandle, 0, LAPTOP_SCREEN_UL_X + 100, LAPTOP_SCREEN_WEB_UL_Y + 240 , VO_BLT_SRCTRANSPARENCY,NULL);
	
	DisplayBriefingRoomEnterSlogan();

	DisplayBriefingRoomEnterCopyright();

	RenderWWWProgramTitleBar( );
	
	
	// render the	activation string
	//-------------
	//DisplayPlayerActivationBriefingRoomEnterString( ); //off
	//-------------

  InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void SelectBriefingRoomEnterLocationButton(MOUSE_REGION * pRegion, INT32 iReason )
{ 
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{

	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
	
		if ( gBriefingRoomData[0].Hidden == TRUE ) 
		{
			bBriefingRoomSpecialMission = FALSE;
			bBriefingRoom  = TRUE;
			
			ResetVal = FALSE;
		
			IDPageEncyData = PAGEBRIEFINGROOM; 
		
			if(!fFirstTimeInBriefingRoomEnter) guiCurrentLaptopMode = LAPTOP_MODE_BRIEFING_ROOM;
		}
		else
		{
			DoLapTopMessageBox( MSG_BOX_LAPTOP_DEFAULT, pSectorPageText[ 5 ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	} 
}

BOOLEAN InitBriefingRoomEnterDefaults()
{
  VOBJECT_DESC    VObjectDesc;

	// load the Rust bacground graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	//FilenameForBPP("LAPTOP\\rustbackground.sti", VObjectDesc.ImageFile);
	FilenameForBPP("BriefingRoom\\background.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiRustBriefingRoomEnterBackGround));

	return(TRUE);
}

BOOLEAN RemoveBriefingRoomEnterDefaults()
{
	DeleteVideoObjectFromIndex(guiRustBriefingRoomEnterBackGround);

	return(TRUE);
}

BOOLEAN DrawBriefingRoomEnterLogoAim()
{
	HVOBJECT hLogoEncyklopedia;	

	GetVideoObject(&hLogoEncyklopedia, guiRustBriefingRoomEnterLogoAim);
	BltVideoObject(FRAME_BUFFER, hLogoEncyklopedia, 0,BRIEFINGROOM_MISSION_TOC_X-120, RUSTBACKGROUND_1_Y + 20, VO_BLT_SRCTRANSPARENCY,NULL);

	return(TRUE);
}

BOOLEAN DrawBriefingRoomEnterDefaults()
{
	HVOBJECT hRustBackGroundHandle;

	// Blt the rust background
	GetVideoObject(&hRustBackGroundHandle, guiRustBriefingRoomEnterBackGround);
	BltVideoObject(FRAME_BUFFER, hRustBackGroundHandle, 0,RUSTBACKGROUND_1_X, RUSTBACKGROUND_1_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	return(TRUE);
}

BOOLEAN DisplayBriefingRoomEnterSlogan()
{

	return(TRUE);
}

BOOLEAN DisplayBriefingRoomEnterCopyright()
{


	return(TRUE);
}


void DisplayPlayerActivationBriefingRoomEnterString( void )
{

HVOBJECT hHandle;

	// this function will grab the string that the player will enter for activation

	// player gone too far, move back
	if(iStringBriefingRoomEnterPos > 64)
	{
		iStringBriefingRoomEnterPos = 64;
	}

	// restore background
	//RenderActivationIndent( 257, 328 );
	// get the video object
	//off
	//GetVideoObject(&hHandle, guiBRIEFINGROOM_MISSIONACTIVATIONINDENT);
	//off
	// blt to sX, sY relative to upper left corner
	//BltVideoObject(FRAME_BUFFER, hHandle, 0, LAPTOP_SCREEN_UL_X + 100, LAPTOP_SCREEN_WEB_UL_Y + 240 , VO_BLT_SRCTRANSPARENCY,NULL);

	// setup the font stuff
	SetFont( FONT14ARIAL );
	SetFontForeground( 184 );
	SetFontBackground(FONT_BLACK);


	// reset shadow
	//off
	//SetFontShadow(DEFAULT_SHADOW);
	//mprintf(IMP_PLAYER_ACTIVATION_STRING_X, IMP_PLAYER_ACTIVATION_STRING_Y, pPlayerBriefingRoomEnterActivationString);


	fNewCharInActivationBriefingRoomEnterString = FALSE;
	fReDrawScreenFlag = TRUE;
	return;
}

void DisplayActivationBriefingRoomEnterStringCursor( void )
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
	LineDraw(TRUE, (UINT16) uiCursorBriefingRoomEnterPosition, CURSOR_Y, (UINT16)uiCursorBriefingRoomEnterPosition, CURSOR_Y + CURSOR_HEIGHT, Get16BPPColor( FROMRGB( BriefingRoomEnterGlowColorsList[ iCurrentState ][ 0 ], BriefingRoomEnterGlowColorsList[ iCurrentState ][ 1 ], BriefingRoomEnterGlowColorsList[ iCurrentState ][ 2 ] ) ),
			pDestBuf );

	// unlock frame buffer
	UnLockVideoSurface( FRAME_BUFFER );

	InvalidateRegion((UINT16) uiCursorBriefingRoomEnterPosition , CURSOR_Y , (UINT16)uiCursorBriefingRoomEnterPosition + 1, CURSOR_Y + CURSOR_HEIGHT + 1);


	return;
}



void GetPlayerKeyBoardInputForBriefingRoomEnterHomePage( void )
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
					/* off
					if(( InputEvent.usEvent == KEY_UP ) )
					{
						// return hit, check to see if current player activation string is a valid one
						ProcessPlayerInputActivationBriefingRoomEnterString( );

					fNewCharInActivationBriefingRoomEnterString = TRUE;
					}
					*/
				break;
				case (( ESC )):
			HandleLapTopESCKey();		// WANNE: Fix for proper closing of the IMP laptop page
			LeaveLapTopScreen( );
				break;
				default:
					if(InputEvent.usEvent == KEY_DOWN || InputEvent.usEvent == KEY_REPEAT )
					{
						HandleBriefingRoomEnterTextEvent( InputEvent.usParam );
					}
				break;
			}
		}
	}

	return;
}


void HandleBriefingRoomEnterTextEvent( UINT32 uiKey )
{
	// this function checks to see if a letter or a backspace was pressed, if so, either put char to screen
	// or delete it
	switch( uiKey )
	{
	case ( BACKSPACE ):

			if( iStringBriefingRoomEnterPos >= 0 )
			{

				if( iStringBriefingRoomEnterPos > 0 )
				{
					// decrement iStringBriefingRoomEnterPosition
					iStringBriefingRoomEnterPos-=1;
				}

				// null out char
		pPlayerBriefingRoomEnterActivationString[iStringBriefingRoomEnterPos ] = 0;

				// move back cursor
		uiCursorBriefingRoomEnterPosition = StringPixLength( pPlayerBriefingRoomEnterActivationString, FONT14ARIAL ) + IMP_PLAYER_ACTIVATION_STRING_X;



				// string has been altered, redisplay
		fNewCharInActivationBriefingRoomEnterString = TRUE;

		}

		break;

	default:
	 if( uiKey >= 'A' && uiKey <= 'Z' ||
					uiKey >= 'a' && uiKey <= 'z' ||
					uiKey >= '0' && uiKey <= '9' ||
					uiKey == '_' || uiKey == '.' || uiKey ==' ')
			{
				// if the current string position is at max or great, do nothing
		if( iStringBriefingRoomEnterPos >= 8 )
		{
					break;
				}
				else
				{
					if(iStringBriefingRoomEnterPos < 0 )
					{
						iStringBriefingRoomEnterPos = 0;
					}
			// valid char, capture and convert to CHAR16
			pPlayerBriefingRoomEnterActivationString[iStringBriefingRoomEnterPos] = ( CHAR16 )uiKey;

					// null out next char position
					pPlayerBriefingRoomEnterActivationString[iStringBriefingRoomEnterPos + 1] = 0;

					// move cursor position ahead
			uiCursorBriefingRoomEnterPosition = StringPixLength( pPlayerBriefingRoomEnterActivationString, FONT14ARIAL ) + IMP_PLAYER_ACTIVATION_STRING_X;

					// increment string position
					iStringBriefingRoomEnterPos +=1;

				// string has been altered, redisplay
			fNewCharInActivationBriefingRoomEnterString = TRUE;

				}

			}

		break;

	}


	return;
}

void ProcessPlayerInputActivationBriefingRoomEnterString( void )
{
	// prcess string to see if it matches activation string
	char charPlayerActivationString[32];
	wcstombs(charPlayerActivationString,pPlayerBriefingRoomEnterActivationString,32);

	if( ( ( wcscmp(pPlayerBriefingRoomEnterActivationString, L"SN5631") == 0 ) || ( wcscmp(pPlayerBriefingRoomEnterActivationString, L"sn5631") == 0 ) ) ) //&&( LaptopSaveInfo.gfNewGameLaptop < 2 ) )
	{
	
	/*
		bBriefingRoom  = FALSE;
		bBriefingRoomSpecialMission = TRUE;
		
		ResetVal = FALSE;
		
		IDPageEncyData = PAGEBRIEFINGROOMSPECIALMISSION; 
		
		if(!fFirstTimeInBriefingRoomEnter) guiCurrentLaptopMode = LAPTOP_MODE_BRIEFING_ROOM;	
	*/
	}
	

	return;
}

void ResetActivationBriefingRoomEnterStringTextBox(void)
{
	// Reset activation text box
	for (int i = 0; i < iStringBriefingRoomEnterPos; i++)
	{
		pPlayerBriefingRoomEnterActivationString[i] = 0;
	}

	iStringBriefingRoomEnterPos = 0;

	uiCursorBriefingRoomEnterPosition = StringPixLength( pPlayerBriefingRoomEnterActivationString, FONT14ARIAL ) + IMP_PLAYER_ACTIVATION_STRING_X;
	DisplayPlayerActivationBriefingRoomEnterString();
	DisplayActivationBriefingRoomEnterStringCursor();
}
