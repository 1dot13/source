/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "DropDown.h"
#include "Font.h"
#include "laptop.h"
#include "Cursors.h"
#include "Utilities.h"
#include "vobject.h"
#include "WCheck.h"
#include "line.h"
#include "WordWrap.h"
#include "input.h"		// for gfLeftButtonState

// define now, make a parameter later
#define		DEF_DROPDOWN_FONT							FONT12ARIAL
#define		DEF_SCROLL_ARROW_HEIGHT						24
#define		CITY_NAME_OFFSET							6
#define		STATIC_TEXT_COLOR							145

UINT32	guGoldArrowImages = 0;

// helpful draw functions, as adding sti objects to paint boundaries is... lets say, inefficient
void DisplaySmallLine( UINT16 usStartX, UINT16 usStartY, UINT16 EndX, UINT16 EndY, UINT16 usColor )
{
	UINT32 uiDestPitchBYTES;
	UINT8 *pDestBuf;

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	// draw the line
	LineDraw( FALSE, usStartX, usStartY, EndX, EndY, usColor, pDestBuf );

	// unlock frame buffer
	UnLockVideoSurface( FRAME_BUFFER );
}

void Display2Line2ShadowHorizontal( UINT16 usStartX, UINT16 usStartY, UINT16 EndX, UINT16 usColor1, UINT16 usColor2 )
{
	DisplaySmallLine( usStartX, usStartY, EndX, usStartY, usColor1 );
	DisplaySmallLine( usStartX, usStartY + 1, EndX, usStartY + 1, usColor1 );
	DisplaySmallLine( usStartX, usStartY + 2, EndX, usStartY + 2, usColor2 );
	DisplaySmallLine( usStartX, usStartY + 3, EndX, usStartY + 3, usColor2 );
}

void Display2Line2ShadowVertical( UINT16 usStartX, UINT16 usStartY, UINT16 EndY, UINT16 usColor1, UINT16 usColor2 )
{
	DisplaySmallLine( usStartX, usStartY, usStartX, EndY, usColor1 );
	DisplaySmallLine( usStartX + 1, usStartY, usStartX + 1, EndY, usColor1 );
	DisplaySmallLine( usStartX + 2, usStartY, usStartX + 2, EndY, usColor2 );
	DisplaySmallLine( usStartX + 3, usStartY, usStartX + 3, EndY, usColor2 );
}

WidgetBase::WidgetBase( )
{
	// default settings
	musStartX = 0;
	musStartY = 0;

	mColorLine = Get16BPPColor( FROMRGB( 231, 199, 90 ) );
	mColorLineShadow = Get16BPPColor( FROMRGB( 33, 24, 8 ) );
	mColorMarked = Get16BPPColor( FROMRGB( 200, 169, 87 ) );
	mColorHighLight = Get16BPPColor( FROMRGB( 235, 222, 171 ) );
}

DropDownBase::DropDownBase( )
: WidgetBase( )
{
	// default settings
	musWidth = 100;						// width of text field
	musArrowWidth = 22;					// width of arrow (and thus scrollbar)

	mEntryVector.clear( );

	swprintf( mHelpText, L"" );

	mSelectedEntry = 0;
	mFirstShownEntry = 0;
}

void
DropDownBase::Init( UINT16 sX, UINT16 sY )
{
	mfMouseRegionsCreated = FALSE;

	SetX( sX );
	SetY( sY );

	mSelectedEntry = min( mSelectedEntry, mEntryVector.size( ) - 1 );
	mNumDisplayedEntries = min( DROPDOWN_REGIONS, mEntryVector.size( ) );
	mFirstShownEntry = max( 0, min( mFirstShownEntry, mEntryVector.size( ) - 1 - mNumDisplayedEntries ) );

	musWidth = 0;
	UINT16 size = mEntryVector.size( );
	for ( UINT16 i = 0; i < size; ++i )
	{
		musWidth = max( musWidth, StringPixLength( mEntryVector[i].second, DEF_DROPDOWN_FONT ) );
	}

	// account for a bit of space left and right
	musWidth += 2 * CITY_NAME_OFFSET;

	musUpArrowX = GetX( ) + musWidth;
	musUpArrowY = GetY( ) + 2;

	musFontHeight = GetFontHeight( DEF_DROPDOWN_FONT );	// does not work on init of static objects, as the fonts do not yet exist!

	mfDropScrollBar = (size > DROPDOWN_REGIONS);
}

void
DropDownBase::Init_Drop( )
{
	mfMouseRegionsCreated_Drop = FALSE;

	musStartX_Drop = GetX( );
	musStartY_Drop = GetY( ) + DEF_SCROLL_ARROW_HEIGHT;
	musScrollAreaX = musStartX_Drop + musWidth;

	musDownArrowX = musUpArrowX;
	musAreaHeight = mNumDisplayedEntries * (musFontHeight + 4);
	musDownArrowY = GetY( ) + musAreaHeight + 2;
}

void * gDropObj;

void
DropDownBase::Create( UINT16 sX, UINT16 sY )
{
	Destroy( );

	Init( sX, sY );

	VOBJECT_DESC	VObjectDesc;

	// Gold Arrow for the scroll area
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "LAPTOP\\GoldArrows.sti", VObjectDesc.ImageFile );
	if ( !AddVideoObject( &VObjectDesc, &guGoldArrowImages ) )
		return;

	gDropObj = (void*) this;

	MSYS_DefineRegion( &mSelectedOpenDropDownRegion, GetX( ) + musWidth, GetY( ), GetX( ) + musWidth + musArrowWidth, GetY( ) + DEF_SCROLL_ARROW_HEIGHT, MSYS_PRIORITY_HIGHEST,
					   CURSOR_WWW, MSYS_NO_CALLBACK, CallBackWrapper( (void*) this, DROPDOWN_OPEN, &DropDownBase::Dummyfunc ) );
	MSYS_AddRegion( &mSelectedOpenDropDownRegion );

	//click anywhere on the screen to close the window( only when the drop down window is active)
	MSYS_DefineRegion( &mSelectedCloseDropDownRegion, LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y, MSYS_PRIORITY_HIGHEST,
					   CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, CallBackWrapper( (void*) this, DROPDOWN_CLOSE, &DropDownBase::Dummyfunc ) );
	MSYS_AddRegion( &mSelectedCloseDropDownRegion );
	MSYS_DisableRegion( &mSelectedCloseDropDownRegion );

	MSYS_DefineRegion( &mBubbleHelpRegion, GetX( ), GetY( ), GetX( ) + musWidth, GetY( ) + DEF_SCROLL_ARROW_HEIGHT, MSYS_PRIORITY_HIGHEST,
					   CURSOR_WWW, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	MSYS_AddRegion( &mBubbleHelpRegion );

	mfMouseRegionsCreated = TRUE;
}

void
DropDownBase::Create_Drop( )
{
	Destroy_Drop( );

	Init_Drop( );

	UINT16 i;
	UINT16 usPosX = musStartX_Drop;
	UINT16 usPosY = musStartY_Drop;
	UINT16 usHeight = 0;

	for ( i = 0; i < mNumDisplayedEntries; ++i )
	{
		MSYS_DefineRegion( &mDropDownRegion[i], usPosX, (UINT16)(usPosY + 4), (UINT16)(usPosX + musWidth - 6), (UINT16)(usPosY + musFontHeight + 7), MSYS_PRIORITY_HIGHEST,
						   CURSOR_WWW, CallBackWrapper( (void*) this, DROPDOWN_MOVEMENT, &DropDownBase::Dummyfunc ), CallBackWrapper( (void*) this, DROPDOWN_REGION, &DropDownBase::Dummyfunc ) );
		MSYS_AddRegion( &mDropDownRegion[i] );
		MSYS_SetRegionUserData( &mDropDownRegion[i], 0, i );

		usPosY += musFontHeight + 2;
	}
	usHeight = usPosY - musStartY_Drop + 10;

	//create the scroll bars regions
	//up arrow
	usPosX = musUpArrowX;
	usPosY = musUpArrowY;
	for ( i = 0; i<2; ++i )
	{
		MSYS_DefineRegion( &mgSelectedUpDownArrowOnScrollAreaRegion[i], usPosX, usPosY, (UINT16)(usPosX + musWidth), (UINT16)(usPosY + DEF_SCROLL_ARROW_HEIGHT), MSYS_PRIORITY_HIGHEST,
						   CURSOR_WWW, MSYS_NO_CALLBACK, CallBackWrapper( (void*) this, DROPDOWN_ARROW, &DropDownBase::Dummyfunc ) );
		MSYS_AddRegion( &mgSelectedUpDownArrowOnScrollAreaRegion[i] );
		MSYS_SetRegionUserData( &mgSelectedUpDownArrowOnScrollAreaRegion[i], 0, i );
		usPosX = musDownArrowX;
		usPosY = musDownArrowY;
	}

	// create scrollbars only if they are needed
	if ( mfDropScrollBar )
	{
		//the scroll area itself
		usPosX = musUpArrowX;
		usPosY = musUpArrowY + DEF_SCROLL_ARROW_HEIGHT;

		usHeight = (musAreaHeight - DEF_SCROLL_ARROW_HEIGHT - 4) / (mNumDisplayedEntries);
		for ( i = 0; i<mNumDisplayedEntries - 1; ++i )
		{
			MSYS_DefineRegion( &mSelectedScrollAreaDropDownRegion[i], usPosX, usPosY, (UINT16)(usPosX + musWidth), (UINT16)(usPosY + usHeight), MSYS_PRIORITY_HIGHEST,
							   CURSOR_LAPTOP_SCREEN, CallBackWrapper( (void*) this, DROPDOWN_SCROLL_MOVEMENT, &DropDownBase::Dummyfunc ), CallBackWrapper( (void*) this, DROPDOWN_SCROLL_REGION, &DropDownBase::Dummyfunc ) );
			MSYS_AddRegion( &mSelectedScrollAreaDropDownRegion[i] );
			MSYS_SetRegionUserData( &mSelectedScrollAreaDropDownRegion[i], 0, i );
			usPosY += usHeight;
		}

		//put the last one down to cover the remaining area
		MSYS_DefineRegion( &mSelectedScrollAreaDropDownRegion[i], usPosX, usPosY, (UINT16)(usPosX + musWidth), musDownArrowY, MSYS_PRIORITY_HIGHEST,
						   CURSOR_LAPTOP_SCREEN, CallBackWrapper( (void*) this, DROPDOWN_SCROLL_MOVEMENT, &DropDownBase::Dummyfunc ), CallBackWrapper( (void*) this, DROPDOWN_SCROLL_REGION, &DropDownBase::Dummyfunc ) );
		MSYS_AddRegion( &mSelectedScrollAreaDropDownRegion[i] );
		MSYS_SetRegionUserData( &mSelectedScrollAreaDropDownRegion[i], 0, i );
	}

	MSYS_EnableRegion( &mSelectedCloseDropDownRegion );

	mfMouseRegionsCreated_Drop = TRUE;
}

void
DropDownBase::Destroy( )
{
	Destroy_Drop( );

	if ( !mfMouseRegionsCreated )
		return;

	mfMouseRegionsCreated = FALSE;

	MSYS_RemoveRegion( &mSelectedOpenDropDownRegion );
	MSYS_RemoveRegion( &mSelectedCloseDropDownRegion );
	MSYS_RemoveRegion( &mBubbleHelpRegion );

	DeleteVideoObjectFromIndex( guGoldArrowImages );

	SetRefresh( );
}

void
DropDownBase::Destroy_Drop( )
{
	if ( !mfMouseRegionsCreated_Drop )
		return;

	UINT16 i;
	for ( i = 0; i< mNumDisplayedEntries; ++i )
		MSYS_RemoveRegion( &mDropDownRegion[i] );

	//destroy the scroll bar arrow regions
	for ( i = 0; i< 2; ++i )
		MSYS_RemoveRegion( &mgSelectedUpDownArrowOnScrollAreaRegion[i] );

	// create scrollbars and arrows only if they are needed
	if ( mfDropScrollBar )
	{
		//destroy the scroll bar regions
		for ( i = 0; i<mNumDisplayedEntries; ++i )
			MSYS_RemoveRegion( &mSelectedScrollAreaDropDownRegion[i] );
	}

	//disable the close region
	MSYS_DisableRegion( &mSelectedCloseDropDownRegion );

	mfMouseRegionsCreated_Drop = FALSE;

	SetRefresh( );
}

void
DropDownBase::Display( )
{
	if ( !mfMouseRegionsCreated )
		return;

	HVOBJECT	hArrowHandle;

	//Display the background for the drop down window
	ColorFillVideoSurfaceArea( FRAME_BUFFER, GetX( ), GetY( ), GetX( ) + musWidth + musArrowWidth, GetY( ) + DEF_SCROLL_ARROW_HEIGHT, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

	// top
	Display2Line2ShadowHorizontal( GetX( ), GetY( ), GetX( ) + musWidth + musArrowWidth - 1, GetColorLine( ), GetColorLineShadow( ) );

	// left
	Display2Line2ShadowVertical( GetX( ), GetY( ) + 2, GetY( ) + DEF_SCROLL_ARROW_HEIGHT - 1, GetColorLine( ), GetColorLineShadow( ) );

	// right between text and arrow
	Display2Line2ShadowVertical( GetX( ) + musWidth - 4, GetY( ) + 2, GetY( ) + DEF_SCROLL_ARROW_HEIGHT - 1, GetColorLine( ), GetColorLineShadow( ) );

	// bottom
	Display2Line2ShadowHorizontal( GetX( ), GetY( ) + DEF_SCROLL_ARROW_HEIGHT - 4, GetX( ) + musWidth + musArrowWidth - 1, GetColorLine( ), GetColorLineShadow( ) );

	// right
	Display2Line2ShadowVertical( GetX( ) + musWidth + musArrowWidth - 4, GetY( ), GetY( ) + DEF_SCROLL_ARROW_HEIGHT - 3, GetColorLine( ), GetColorLineShadow( ) );

	DrawTopEntry( );

	//get and display the up and down arrows
	GetVideoObject( &hArrowHandle, guGoldArrowImages );

	//top arrow
	BltVideoObject( FRAME_BUFFER, hArrowHandle, 0, musUpArrowX, musUpArrowY, VO_BLT_SRCTRANSPARENCY, NULL );

	// Set region help text
	if ( StringPixLength( mHelpText, DEF_DROPDOWN_FONT ) > 0 )
	{
		SetRegionFastHelpText( &mBubbleHelpRegion, mHelpText );
		SetRegionHelpEndCallback( &mBubbleHelpRegion, MSYS_NO_CALLBACK );
	}

	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );

	// display dropped region if necessary
	Display_Drop( );
}


void
DropDownBase::Display_Drop( )
{
	if ( !mfMouseRegionsCreated_Drop )
		return;

	HVOBJECT	hArrowHandle;

	//Display the background for the drop down window
	ColorFillVideoSurfaceArea( FRAME_BUFFER, musStartX_Drop, musStartY_Drop, musStartX_Drop + musWidth + musArrowWidth, musStartY_Drop + musAreaHeight, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

	// top
	Display2Line2ShadowHorizontal( musStartX_Drop, musStartY_Drop - 4, musStartX_Drop + musWidth + musArrowWidth - 1, GetColorLine( ), GetColorLineShadow( ) );

	// left
	Display2Line2ShadowVertical( musStartX_Drop, musStartY_Drop - 2, musStartY_Drop + musAreaHeight - 1, GetColorLine( ), GetColorLineShadow( ) );

	// right between text and arrow
	Display2Line2ShadowVertical( musStartX_Drop + musWidth - 4, musStartY_Drop - 2, musStartY_Drop + musAreaHeight - 1, GetColorLine( ), GetColorLineShadow( ) );

	// bottom
	Display2Line2ShadowHorizontal( musStartX_Drop, musStartY_Drop + musAreaHeight - 4, musStartX_Drop + musWidth + musArrowWidth - 1, GetColorLine( ), GetColorLineShadow( ) );

	// right
	Display2Line2ShadowVertical( musStartX_Drop + musWidth + musArrowWidth - 4, musStartY_Drop - 4, musStartY_Drop + musAreaHeight - 3, GetColorLine( ), GetColorLineShadow( ) );

	DrawSelectedCity( );

	//get and display the up and down arrows
	GetVideoObject( &hArrowHandle, guGoldArrowImages );
	// top arrow
	BltVideoObject( FRAME_BUFFER, hArrowHandle, 1, musUpArrowX, musUpArrowY, VO_BLT_SRCTRANSPARENCY, NULL );

	// down arrow
	BltVideoObject( FRAME_BUFFER, hArrowHandle, 0, musDownArrowX, musDownArrowY, VO_BLT_SRCTRANSPARENCY, NULL );

	//display the scroll rectangle
	DrawGoldRectangle( );

	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}

void
DropDownBase::DrawTopEntry( )
{
	// make sure we don't get bogus values
	if ( !mEntryVector.empty( ) )
	{
		mSelectedEntry = min( mSelectedEntry, mEntryVector.size( ) - 1 );

		//display the name in the list
		ColorFillVideoSurfaceArea( FRAME_BUFFER, GetX( ) + 4, GetY( ) + 4, GetX( ) + musWidth - 4, GetY( ) + musFontHeight + 8, GetColorMarked( ) );

		SetFontShadow( NO_SHADOW );

		DrawTextToScreen( mEntryVector[mSelectedEntry].second, GetX( ) + CITY_NAME_OFFSET, (UINT16)(GetY( ) + 7), 0, DEF_DROPDOWN_FONT, FONT_BLACK, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

		SetFontShadow( DEFAULT_SHADOW );
	}
}

void
DropDownBase::DrawSelectedCity( )
{
	UINT16 usPosY = musStartY_Drop;
	UINT16 usMaxY = 0;
	UINT16 usMinY = 0;

	// make sure we dont go out of our array
	if ( mFirstShownEntry + mNumDisplayedEntries > (UINT16)(mEntryVector.size( )) )
		mFirstShownEntry = mEntryVector.size( ) - mNumDisplayedEntries;

	for ( UINT16 i = mFirstShownEntry; i < mFirstShownEntry + mNumDisplayedEntries; ++i )
	{
		usMaxY = min( usPosY + musFontHeight + 1, musStartY_Drop + musAreaHeight - 4 );
		usMinY = max( usPosY - 3, musStartY_Drop );

		if ( i == mSelectedEntry )
		{
			ColorFillVideoSurfaceArea( FRAME_BUFFER, musStartX_Drop + CITY_NAME_OFFSET - 2, usMinY, musStartX_Drop + musWidth - CITY_NAME_OFFSET + 2, usMaxY, GetColorMarked( ) );
			SetFontShadow( NO_SHADOW );
			DrawTextToScreen( mEntryVector[i].second, musStartX_Drop + CITY_NAME_OFFSET, usPosY, 0, DEF_DROPDOWN_FONT, FONT_BLACK, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
			SetFontShadow( DEFAULT_SHADOW );
		}
		else
		{
			ColorFillVideoSurfaceArea( FRAME_BUFFER, musStartX_Drop + CITY_NAME_OFFSET - 2, usMinY, musStartX_Drop + musWidth - CITY_NAME_OFFSET + 2, usMaxY, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
			DrawTextToScreen( mEntryVector[i].second, musStartX_Drop + CITY_NAME_OFFSET, usPosY, 0, DEF_DROPDOWN_FONT, STATIC_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
		}

		usPosY += musFontHeight + 4;
	}
}

void
DropDownBase::DrawGoldRectangle( )
{
	UINT32 uiDestPitchBYTES;
	UINT8	*pDestBuf;
	UINT16 usWidth, usTempHeight;
	FLOAT dHeight = 0.0f;
	UINT16 usPosX, usPosY;

	usPosX = musScrollAreaX;
	usWidth = musArrowWidth - 5;

	if ( mEntryVector.size( ) > 1 )
	{
		usTempHeight = (musAreaHeight - DEF_SCROLL_ARROW_HEIGHT) - 4;

		if ( mEntryVector.size( ) < DROPDOWN_REGIONS )
		{
			dHeight = usTempHeight;
			usPosY = musStartY_Drop;
		}
		else
		{
			dHeight = (FLOAT)(usTempHeight) / (FLOAT)(mEntryVector.size( ));

			usPosY = musStartY_Drop + (UINT16)((dHeight * mFirstShownEntry));

			dHeight *= mNumDisplayedEntries;
		}

		if ( mfDropScrollBar )
		{
			if ( usPosY >= musStartY_Drop + musAreaHeight - DEF_SCROLL_ARROW_HEIGHT - dHeight )
				usPosY = musStartY_Drop + musAreaHeight - DEF_SCROLL_ARROW_HEIGHT - dHeight - 5;
		}
		else
		{
			if ( usPosY >= musStartY_Drop + musAreaHeight - dHeight )
				usPosY = musStartY_Drop + musAreaHeight - dHeight - 5;
		}

		// color everything black and then color the rectangle, that way we dont have to redraw the entire page
		ColorFillVideoSurfaceArea( FRAME_BUFFER, musScrollAreaX, musStartY_Drop, musScrollAreaX + usWidth, musStartY_Drop + usTempHeight, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
		ColorFillVideoSurfaceArea( FRAME_BUFFER, musScrollAreaX, usPosY, musScrollAreaX + usWidth, usPosY + (UINT16)dHeight, GetColorMarked( ) );

		//display the line
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

		// draw the gold highlite line on the top and left
		LineDraw( FALSE, usPosX, usPosY, usPosX + usWidth, usPosY, GetColorHighLight( ), pDestBuf );
		LineDraw( FALSE, usPosX, usPosY, usPosX, usPosY + (UINT16)dHeight, GetColorHighLight( ), pDestBuf );

		// draw the shadow line on the bottom and right
		LineDraw( FALSE, usPosX, usPosY + (UINT16)dHeight, usPosX + usWidth, usPosY + (UINT16)dHeight, GetColorLineShadow( ), pDestBuf );
		LineDraw( FALSE, usPosX + usWidth, usPosY, usPosX + usWidth, usPosY + (UINT16)dHeight, GetColorLineShadow( ), pDestBuf );
	}

	// unlock frame buffer
	UnLockVideoSurface( FRAME_BUFFER );
}

void
DropDownBase::SelectDropDownMovementCallBack( MOUSE_REGION * pRegion, INT32 reason )
{
	if ( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		pRegion->uiFlags &= (~BUTTON_CLICKED_ON);
		InvalidateRegion( pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY );
	}
	else if ( reason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		pRegion->uiFlags |= BUTTON_CLICKED_ON;

		mSelectedEntry = (UINT16)MSYS_GetRegionUserData( pRegion, 0 ) + mFirstShownEntry;

		InvalidateRegion( musStartX_Drop - CITY_NAME_OFFSET + 2, musStartY_Drop, musStartX_Drop + musWidth - CITY_NAME_OFFSET + 2, musStartY_Drop + musAreaHeight - 4 );

		DrawTopEntry( );
		DrawGoldRectangle( );
		DrawSelectedCity( );
	}
}

void
DropDownBase::SelectDropDownRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		UINT16 ubSelected = (UINT16)MSYS_GetRegionUserData( pRegion, 0 );
		mSelectedEntry = min( ubSelected + mFirstShownEntry, (UINT16)(max( 0, mEntryVector.size( ) - 1 )) );

		Destroy_Drop( );
	}
}

void
DropDownBase::SelectUpDownArrowOnScrollAreaRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP || iReason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		UINT16	ubDownArrow = (UINT16)MSYS_GetRegionUserData( pRegion, 0 );

		if ( ubDownArrow )
		{
			if ( mSelectedEntry < mEntryVector.size( ) - 1 )
			{
				++mSelectedEntry;
			}

			if ( (mSelectedEntry - mFirstShownEntry) >= mNumDisplayedEntries && mFirstShownEntry <= mEntryVector.size( ) - 1 - mNumDisplayedEntries )
			{
				++mFirstShownEntry;
			}
		}
		else
		{
			if ( mSelectedEntry > 0 )
				--mSelectedEntry;

			if ( mSelectedEntry < mFirstShownEntry )
				--mFirstShownEntry;
		}

		DrawTopEntry( );
		DrawGoldRectangle( );
		DrawSelectedCity( );

		SetRefresh( );
	}
}

void
DropDownBase::SelectScrollAreaDropDownMovementCallBack( MOUSE_REGION * pRegion, INT32 reason )
{
	if ( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		pRegion->uiFlags &= (~BUTTON_CLICKED_ON);
		InvalidateRegion( pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY );
	}
	else if ( reason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if ( gfLeftButtonState )
		{
			// if mouse is outside of the bar, simply move bar up or down
			if ( pRegion->MouseYPos < pRegion->RegionTopLeftY )
			{
				pRegion->uiFlags &= ~BUTTON_CLICKED_ON;

				mFirstShownEntry = max( mFirstShownEntry - 1, 0 );

				if ( mSelectedEntry > mFirstShownEntry + mNumDisplayedEntries )
					mSelectedEntry = mFirstShownEntry + mNumDisplayedEntries;
			}
			else if ( pRegion->MouseYPos > pRegion->RegionBottomRightY )
			{
				pRegion->uiFlags &= ~BUTTON_CLICKED_ON;

				mFirstShownEntry = min( mFirstShownEntry + 1, (UINT16)(max( 0, mEntryVector.size( ) - 1 )) );

				if ( mSelectedEntry < mFirstShownEntry )
					++mSelectedEntry;
			}
			// inside the scroll bar, determine where the middle of the slider is to determine where to go
			else
			{
				pRegion->uiFlags |= BUTTON_CLICKED_ON;

				// go to the relative position in our list of entries depending on where we click
				if ( !mEntryVector.empty( ) && musAreaHeight - 2 * DEF_SCROLL_ARROW_HEIGHT > 0 )
				{
					FLOAT rate = (FLOAT)(pRegion->MouseYPos - musStartY_Drop) / (FLOAT)(musAreaHeight - DEF_SCROLL_ARROW_HEIGHT - 4);

					mFirstShownEntry = rate * mEntryVector.size( );

					if ( mSelectedEntry > mFirstShownEntry + mNumDisplayedEntries )
						mSelectedEntry = mFirstShownEntry + mNumDisplayedEntries;
					else if ( mSelectedEntry < mFirstShownEntry )
						mSelectedEntry = mFirstShownEntry;
				}
			}

			InvalidateRegion( musStartX_Drop, musStartY_Drop, musStartX_Drop + musWidth + musArrowWidth, musStartY_Drop + musAreaHeight - 4 );

			DrawTopEntry( );
			DrawGoldRectangle( );
			DrawSelectedCity( );
		}
	}
}

void
DropDownBase::SelectScrollAreaDropDownRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		UINT16	ubCityNum = (UINT16)MSYS_GetRegionUserData( pRegion, 0 );

		if ( ubCityNum < mSelectedEntry )
		{
			--mSelectedEntry;
			if ( mSelectedEntry < mFirstShownEntry )
				--mFirstShownEntry;
		}
		else if ( ubCityNum > mSelectedEntry )
		{
			++mSelectedEntry;
			if ( (mSelectedEntry - mFirstShownEntry) >= mNumDisplayedEntries )
				++mFirstShownEntry;
		}

		DrawTopEntry( );
		DrawGoldRectangle( );
		DrawSelectedCity( );
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		UINT16	ubCityNum = (UINT16)MSYS_GetRegionUserData( pRegion, 0 );

		pRegion->uiFlags |= BUTTON_CLICKED_ON;

		if ( ubCityNum < mSelectedEntry )
		{
			--mSelectedEntry;
			if ( mSelectedEntry < mFirstShownEntry )
				--mFirstShownEntry;
		}
		else if ( ubCityNum > mSelectedEntry )
		{
			++mSelectedEntry;
			if ( (mSelectedEntry - mFirstShownEntry) >= mNumDisplayedEntries )
				++mFirstShownEntry;
		}

		InvalidateRegion( pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY );

		DrawTopEntry( );
		DrawGoldRectangle( );
		DrawSelectedCity( );
	}
}

/*
* If aKey exists among our keys, set it as the current one
*/
void
DropDownBase::SetSelectedEntryKey( INT16 aKey )
{
	UINT16 cnt = 0;
	std::vector<std::pair<INT16, STR16> >::iterator itend = mEntryVector.end( );
	for ( std::vector<std::pair<INT16, STR16> >::iterator it = mEntryVector.begin( ); it != itend; ++it )
	{
		if ( (*it).first == aKey )
		{
			mSelectedEntry = cnt;
			return;
		}

		++cnt;
	}
}

/*
* set the nth entry as the selected one
*/
void
DropDownBase::SetNthEntry( UINT16 aNr )
{
	mSelectedEntry = min( aNr, max( 0, mEntryVector.size( ) - 1 ) );
}

void
DropDownBase::OpenDropDownRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		Create_Drop( );
		Display_Drop( );
	}
}

void
DropDownBase::CloseDropDownRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		Destroy_Drop( );
	}
}

