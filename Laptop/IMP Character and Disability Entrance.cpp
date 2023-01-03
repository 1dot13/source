#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "CharProfile.h"
	#include "IMP Character and Disability Entrance.h"
	#include "IMP MainPage.h"
	#include "IMPVideoObjects.h"
	#include "Utilities.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Encrypted File.h"
	#include "cursors.h"
	#include "laptop.h"
	#include "IMP Text System.h"
	#include "text.h"

#endif

// IMP personality entrance buttons
INT32 giIMPCharacterAndDisabilityEntranceButton[1];
INT32 giIMPCharacterAndDisabilityEntranceButtonImage[1];


// function definitions
void CreateIMPCharacterAndDisabilityEntranceButtons( void );
void BtnIMPCharacterAndDisabilityEntranceDoneCallback(GUI_BUTTON *btn,INT32 reason);
void WriteIMPCharacterAndDisabilityEntranceText( void );


void EnterIMPCharacterAndDisabilityEntrance( void )
{
	// create buttons needed
	CreateIMPCharacterAndDisabilityEntranceButtons( );


	return;
}

void RenderIMPCharacterAndDisabilityEntrance( void )
{
	// the background
	RenderProfileBackGround( );

	// the IMP symbol
	//RenderIMPSymbol( 112, 30 );

	// indent
	RenderAvgMercIndentFrame(90, 40 );
	return;
}

void ExitIMPCharacterAndDisabilityEntrance( void )
{
	// destroy buttons needed
	RemoveButton(giIMPCharacterAndDisabilityEntranceButton[0] );
	UnloadButtonImage(giIMPCharacterAndDisabilityEntranceButtonImage[0] );

	return;
}


void HandleIMPCharacterAndDisabilityEntrance( void )
{
	// Show the text
	WriteIMPCharacterAndDisabilityEntranceText( );

	return;
}

void CreateIMPCharacterAndDisabilityEntranceButtons( void )
{
	// this function will create the buttons needed for the IMP personality Page

	// ths begin button
	giIMPCharacterAndDisabilityEntranceButtonImage[0]=	LoadButtonImage( "LAPTOP\\button_2.sti" ,-1,0,-1,1,-1 );
	/*giIMPCharacterAndDisabilityEntranceButton[0] = QuickCreateButton( giIMPCharacterAndDisabilityEntranceButtonImage[0], LAPTOP_SCREEN_UL_X +	( 136 ), LAPTOP_SCREEN_WEB_UL_Y + ( 314 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPCharacterAndDisabilityEntranceDoneCallback);
*/
		giIMPCharacterAndDisabilityEntranceButton[0] = CreateIconAndTextButton( giIMPCharacterAndDisabilityEntranceButtonImage[0], pImpButtonText[ 1 ], FONT12ARIAL,
														FONT_WHITE, DEFAULT_SHADOW,
														FONT_WHITE, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														LAPTOP_SCREEN_UL_X +	( 136 ), LAPTOP_SCREEN_WEB_UL_Y + ( 314 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPCharacterAndDisabilityEntranceDoneCallback);


	SetButtonCursor(giIMPCharacterAndDisabilityEntranceButton[0], CURSOR_WWW);

	return;
}

void WriteIMPCharacterAndDisabilityEntranceText( void )
{	
	//Display the title
	DrawTextToScreen( pCharacterTraitBeginIMPStrings[ 0 ], LAPTOP_SCREEN_UL_X - 111, LAPTOP_TITLE_Y, LAPTOP_TEXT_WIDTH, FONT14ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// The personality part
	DisplayWrappedString( LAPTOP_SCREEN_UL_X + 106, LAPTOP_SCREEN_WEB_UL_Y + 63,  ( 400 - 100 ), 2, FONT12ARIAL, FONT_WHITE, pCharacterTraitBeginIMPStrings[ 1 ],FONT_BLACK,FALSE,CENTER_JUSTIFIED);

	// The disability part
	DisplayWrappedString( LAPTOP_SCREEN_UL_X + 106, LAPTOP_SCREEN_WEB_UL_Y + 148,  ( 400 - 100 ), 2, FONT12ARIAL, FONT_WHITE, pCharacterTraitBeginIMPStrings[ 2 ],FONT_BLACK,FALSE,CENTER_JUSTIFIED);

	return;
}

void BtnIMPCharacterAndDisabilityEntranceDoneCallback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP Begin Screen done button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

		// done with begin screen, next screen
			iCurrentImpPage = IMP_CHARACTER_PAGE;
		fButtonPendingFlag = TRUE;
		}
	}
}
