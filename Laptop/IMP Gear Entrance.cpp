/**
* @file
* @author Flugente (bears-pit.com)
*/

#ifdef PRECOMPILEDHEADERS
#include "Laptop All.h"
#else
#include "CharProfile.h"
#include "IMP Gear Entrance.h"
#include "IMP MainPage.h"
#include "IMPVideoObjects.h"
#include "Utilities.h"
#include "Debug.h"
#include "WordWrap.h"
#include "Encrypted File.h"
#include "cursors.h"
#include "laptop.h"
#include "IMP Text System.h"
#include "Text.h"
#endif

// the buttons
UINT32 giIMPGearEntranceButtonImage[2];
UINT32 giIMPGearEntranceButton[2];

// function definitions
void DestroyIMPGearEntranceButtons( void );
void CreateIMPGearEntranceButtons( void );


// callbacks
void BtnIMPGearBeginCallbackOld( GUI_BUTTON *btn, INT32 reason );
void BtnIMPGearBeginCallbackNew( GUI_BUTTON *btn, INT32 reason );

BOOLEAN fNewIMPGearMethodUsed = FALSE;

BOOLEAN IsIMPGearUsed()
{
	return fNewIMPGearMethodUsed;
}

void EnterIMPGearEntrance( void )
{
	CreateIMPGearEntranceButtons( );
}

void RenderIMPGearEntrance( void )
{
	// the background
	RenderProfileBackGround( );

	ColorFillVideoSurfaceArea( FRAME_BUFFER, LAPTOP_SCREEN_UL_X + 50, LAPTOP_SCREEN_WEB_UL_Y + 50, LAPTOP_SCREEN_LR_X - 50, LAPTOP_SCREEN_WEB_UL_Y + 200, Get16BPPColor( FROMRGB( 100, 100, 100 ) ) );
	
	UINT16 usY = DisplayWrappedString( LAPTOP_SCREEN_UL_X + 60, LAPTOP_SCREEN_WEB_UL_Y + 60, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X - 120, 2, FONT14ARIAL, FONT_WHITE,
									   szIMPGearWebSiteText[0], FONT_MCOLOR_BLACK, FALSE, 0 );
	usY += DisplayWrappedString( LAPTOP_SCREEN_UL_X + 60, LAPTOP_SCREEN_WEB_UL_Y + 60 + usY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X - 120, 2, FONT14ARIAL, FONT_WHITE,
								 szIMPGearWebSiteText[1], FONT_MCOLOR_BLACK, FALSE, 0 );
	usY += DisplayWrappedString( LAPTOP_SCREEN_UL_X + 60, LAPTOP_SCREEN_WEB_UL_Y + 60 + usY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X - 120, 2, FONT14ARIAL, FONT_WHITE,
								 szIMPGearWebSiteText[2], FONT_MCOLOR_BLACK, FALSE, 0 );
}

void ExitIMPGearEntrance( void )
{
	// destroy the finish buttons
	DestroyIMPGearEntranceButtons( );
}

void HandleIMPGearEntrance( void )
{
	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}

void CreateIMPGearEntranceButtons( void )
{
	// the begin button
	giIMPGearEntranceButtonImage[0] = LoadButtonImage( "LAPTOP\\button_2.sti", -1, 0, -1, 1, -1 );
		
	giIMPGearEntranceButton[0] = CreateIconAndTextButton( giIMPGearEntranceButtonImage[0], szIMPGearWebSiteText[3], FONT12ARIAL,
															   FONT_WHITE, DEFAULT_SHADOW,
															   FONT_WHITE, DEFAULT_SHADOW,
															   TEXT_CJUSTIFIED,
															   LAPTOP_SCREEN_UL_X + (136), LAPTOP_SCREEN_WEB_UL_Y + (220), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															   BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPGearBeginCallbackOld );

	giIMPGearEntranceButton[1] = CreateIconAndTextButton( giIMPGearEntranceButtonImage[0], szIMPGearWebSiteText[4], FONT12ARIAL,
														  FONT_WHITE, DEFAULT_SHADOW,
														  FONT_WHITE, DEFAULT_SHADOW,
														  TEXT_CJUSTIFIED,
														  LAPTOP_SCREEN_UL_X + (136), LAPTOP_SCREEN_WEB_UL_Y + (280), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														  BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPGearBeginCallbackNew );

	SetButtonCursor( giIMPGearEntranceButton[0], CURSOR_WWW );
}


void DestroyIMPGearEntranceButtons( void )
{
	RemoveButton( giIMPGearEntranceButton[0] );
	RemoveButton( giIMPGearEntranceButton[1] );
	UnloadButtonImage( giIMPGearEntranceButtonImage[0] );
}


void BtnIMPGearBeginCallbackOld( GUI_BUTTON *btn, INT32 reason )
{
	if ( !(btn->uiFlags & BUTTON_ENABLED) )
		return;

	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= (BUTTON_CLICKED_ON);
	}
	else if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if ( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);
			
			iCurrentImpPage = IMP_FINISH;
			iCurrentProfileMode = IMP__FINISH;

			fButtonPendingFlag = TRUE;

			fNewIMPGearMethodUsed = FALSE;
		}
	}
}

void BtnIMPGearBeginCallbackNew( GUI_BUTTON *btn, INT32 reason )
{
	if ( !(btn->uiFlags & BUTTON_ENABLED) )
		return;

	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= (BUTTON_CLICKED_ON);
	}
	else if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if ( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);
			iCurrentImpPage = IMP_GEAR;
			fButtonPendingFlag = TRUE;

			fNewIMPGearMethodUsed = TRUE;
		}
	}
}
