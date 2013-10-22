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

// helpful draw functions, as adding sti objects to paint boundaries is... lets say, inefficient
void DisplaySmallLine( UINT16 usStartX, UINT16 usStartY, UINT16 EndX, UINT16 EndY, UINT16 usColor )
{
	UINT32 uiDestPitchBYTES;
	UINT8 *pDestBuf;

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// draw the line
	LineDraw(FALSE, usStartX, usStartY, EndX, EndY, usColor, pDestBuf);

	// unlock frame buffer
	UnLockVideoSurface( FRAME_BUFFER );
}

void Display2Line2ShadowHorizontal( UINT16 usStartX, UINT16 usStartY, UINT16 EndX, UINT16 usColor1, UINT16 usColor2 )
{
	DisplaySmallLine(usStartX, usStartY  , EndX, usStartY,   usColor1);
	DisplaySmallLine(usStartX, usStartY+1, EndX, usStartY+1, usColor1);
	DisplaySmallLine(usStartX, usStartY+2, EndX, usStartY+2, usColor2);
	DisplaySmallLine(usStartX, usStartY+3, EndX, usStartY+3, usColor2);
}

void Display2Line2ShadowVertical( UINT16 usStartX, UINT16 usStartY, UINT16 EndY, UINT16 usColor1, UINT16 usColor2 )
{
	DisplaySmallLine(usStartX,   usStartY, usStartX,   EndY, usColor1);
	DisplaySmallLine(usStartX+1, usStartY, usStartX+1, EndY, usColor1);
	DisplaySmallLine(usStartX+2, usStartY, usStartX+2, EndY, usColor2);
	DisplaySmallLine(usStartX+3, usStartY, usStartX+3, EndY, usColor2);
}

DropDownBase::DropDownBase()
{
	// default settings
	musWidth = 100;						// width of text field
	musArrowWidth = 22;					// width of arrow (and thus scrollbar)

	mEntryVector.clear();

	swprintf( mHelpText, L"" );

	mSelectedEntry = 0;
	mFirstShownEntry = 0;

	mColorLine			= Get16BPPColor( FROMRGB( 231, 199,  90 ) );
	mColorLineShadow	= Get16BPPColor( FROMRGB(  33,  24,   8 ) );
	mColorMarked		= Get16BPPColor( FROMRGB( 200, 169,  87 ) );
	mColorHighLight		= Get16BPPColor( FROMRGB( 235, 222, 171 ) );
}

void
DropDownBase::Init(UINT16 sX, UINT16 sY)
{
	mfMouseRegionsCreated = FALSE;

	musStartX = sX;
	musStartY = sY;
			
	mSelectedEntry			= min(mSelectedEntry, mEntryVector.size() - 1);
	mNumDisplayedEntries	= min(DROPDOWN_REGIONS, mEntryVector.size() );
	mFirstShownEntry		= max(0, min(mFirstShownEntry, mEntryVector.size() - 1 - mNumDisplayedEntries));

	musWidth = 0;
	UINT8 size = mEntryVector.size();
	for( UINT8 i = 0; i < size; ++i)
	{
		musWidth = max(musWidth, StringPixLength ( mEntryVector[i].second, DEF_DROPDOWN_FONT ));
	}

	// account for a bit of space let and right
	musWidth += 2 * CITY_NAME_OFFSET;

	musUpArrowX = musStartX + musWidth;
	musUpArrowY = musStartY + 2;
	
	musFontHeight = GetFontHeight( DEF_DROPDOWN_FONT );	// does not work on init of static objects, as the fonts do not yet exist!
}

void
DropDownBase::Init_Drop()
{
	mfMouseRegionsCreated_Drop = FALSE;

	musStartX_Drop = musStartX;
	musStartY_Drop = musStartY + DEF_SCROLL_ARROW_HEIGHT;
	musScrollAreaX = musStartX_Drop + musWidth;
				
	musDownArrowX = musUpArrowX;
	musAreaHeight = mNumDisplayedEntries * (musFontHeight + 4);
	musDownArrowY = musStartY_Drop + musAreaHeight - 24 + 2;
}

void * gDropObj;

void
DropDownBase::Create(UINT16 sX, UINT16 sY)
{
	Destroy();

	Init(sX, sY);

	VOBJECT_DESC	VObjectDesc;

	// Gold Arrow for the scroll area
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "LAPTOP\\GoldArrows.sti", VObjectDesc.ImageFile );
	if ( !AddVideoObject( &VObjectDesc, &muiGoldArrowImages ) )
		return;

	gDropObj = (void*) this;
	
	MSYS_DefineRegion( &mSelectedOpenDropDownRegion, musStartX + musWidth, musStartY, musStartX + musWidth + musArrowWidth, musStartY + DEF_SCROLL_ARROW_HEIGHT, MSYS_PRIORITY_HIGH,
							CURSOR_WWW, MSYS_NO_CALLBACK, CallBackWrapper((void*) this, DROPDOWN_OPEN, &DropDownBase::Dummyfunc) );	
	MSYS_AddRegion(&mSelectedOpenDropDownRegion);

	//click anywhere on the screen to close the window( only when the drop down window is active)
	MSYS_DefineRegion( &mSelectedCloseDropDownRegion, LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y , LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y, MSYS_PRIORITY_HIGH-1,
							CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, CallBackWrapper((void*) this, DROPDOWN_CLOSE, &DropDownBase::Dummyfunc) );
	MSYS_AddRegion(&mSelectedCloseDropDownRegion);
	MSYS_DisableRegion(&mSelectedCloseDropDownRegion);

	MSYS_DefineRegion( &mBubbleHelpRegion, musStartX, musStartY, musStartX + musWidth, musStartY + DEF_SCROLL_ARROW_HEIGHT, MSYS_PRIORITY_HIGH,
							CURSOR_WWW, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );	
	MSYS_AddRegion(&mBubbleHelpRegion);
			
	mfMouseRegionsCreated = TRUE;
}

void
DropDownBase::Create_Drop()
{
	Destroy_Drop();

	Init_Drop();

	UINT16 i;
	UINT16 usPosX = musStartX_Drop;
	UINT16 usPosY = musStartY_Drop;
	UINT16 usHeight = 0;

	for( i=0; i< DROPDOWN_REGIONS; ++i)
	{
		MSYS_DefineRegion( &mDropDownRegion[i], usPosX, (UINT16)(usPosY+4), (UINT16)(usPosX+musWidth-6), (UINT16)(usPosY+musFontHeight+7), MSYS_PRIORITY_HIGH,
								CURSOR_WWW, CallBackWrapper((void*) this, DROPDOWN_MOVEMENT, &DropDownBase::Dummyfunc), CallBackWrapper((void*) this, DROPDOWN_REGION, &DropDownBase::Dummyfunc));
		MSYS_AddRegion(&mDropDownRegion[i]);
		MSYS_SetRegionUserData( &mDropDownRegion[ i ], 0, i);

		usPosY += musFontHeight + 2;
	}
	usHeight = usPosY - musStartY_Drop + 10;
	
	//create the scroll bars regions
	//up arrow
	usPosX = musUpArrowX;
	usPosY = musUpArrowY;
	for(i=0; i<2; ++i)
	{
		MSYS_DefineRegion( &mgSelectedUpDownArrowOnScrollAreaRegion[i], usPosX, usPosY, (UINT16)(usPosX+musWidth), (UINT16)(usPosY+DEF_SCROLL_ARROW_HEIGHT), MSYS_PRIORITY_HIGH,
								CURSOR_WWW, MSYS_NO_CALLBACK, CallBackWrapper((void*) this, DROPDOWN_ARROW, &DropDownBase::Dummyfunc));
		MSYS_AddRegion(&mgSelectedUpDownArrowOnScrollAreaRegion[i]);
		MSYS_SetRegionUserData( &mgSelectedUpDownArrowOnScrollAreaRegion[ i ], 0, i);
		usPosX = musDownArrowX;
		usPosY = musDownArrowY;
	}

	//the scroll area itself
	usPosX = musUpArrowX;
	usPosY = musUpArrowY + DEF_SCROLL_ARROW_HEIGHT;
	
	usHeight = ( musAreaHeight - DEF_SCROLL_ARROW_HEIGHT - 4 ) / (mNumDisplayedEntries);
	for(i=0; i<DROPDOWN_REGIONS - 1; ++i)
	{
		MSYS_DefineRegion( &mSelectedScrollAreaDropDownRegion[i], usPosX, usPosY, (UINT16)(usPosX+musWidth), (UINT16)(usPosY+usHeight), MSYS_PRIORITY_HIGH+1,
								CURSOR_LAPTOP_SCREEN, CallBackWrapper((void*) this, DROPDOWN_SCROLL_MOVEMENT, &DropDownBase::Dummyfunc), CallBackWrapper((void*) this, DROPDOWN_SCROLL_REGION, &DropDownBase::Dummyfunc));
		MSYS_AddRegion(&mSelectedScrollAreaDropDownRegion[i]);
		MSYS_SetRegionUserData( &mSelectedScrollAreaDropDownRegion[ i ], 0, i);
		usPosY += usHeight;
	}
	//put the last one down to cover the remaining area
	MSYS_DefineRegion( &mSelectedScrollAreaDropDownRegion[i], usPosX, usPosY, (UINT16)(usPosX+musWidth), musDownArrowY, MSYS_PRIORITY_HIGH+1,
							CURSOR_LAPTOP_SCREEN, CallBackWrapper((void*) this, DROPDOWN_SCROLL_MOVEMENT, &DropDownBase::Dummyfunc), CallBackWrapper((void*) this, DROPDOWN_SCROLL_REGION, &DropDownBase::Dummyfunc));
	MSYS_AddRegion(&mSelectedScrollAreaDropDownRegion[i]);
	MSYS_SetRegionUserData( &mSelectedScrollAreaDropDownRegion[ i ], 0, i);
	
	MSYS_EnableRegion(&mSelectedCloseDropDownRegion);

	mfMouseRegionsCreated_Drop = TRUE;
}

void
DropDownBase::Destroy()
{
	Destroy_Drop();

	if( !mfMouseRegionsCreated )
		return;
	
	mfMouseRegionsCreated	= FALSE;

	MSYS_RemoveRegion( &mSelectedOpenDropDownRegion);
	MSYS_RemoveRegion( &mSelectedCloseDropDownRegion);
	MSYS_RemoveRegion( &mBubbleHelpRegion);

	DeleteVideoObjectFromIndex(muiGoldArrowImages);

	SetRefresh();
}

void
DropDownBase::Destroy_Drop()
{
	if( !mfMouseRegionsCreated_Drop )
		return;

	UINT8 i;
	for( i=0; i< DROPDOWN_REGIONS; ++i)
		MSYS_RemoveRegion( &mDropDownRegion[i]);
	
	//destroy the scroll bar arrow regions
	for( i=0; i< 2; ++i)
		MSYS_RemoveRegion( &mgSelectedUpDownArrowOnScrollAreaRegion[i]);

	//destroy the scroll bar regions
	for( i=0; i<DROPDOWN_REGIONS; ++i)
		MSYS_RemoveRegion( &mSelectedScrollAreaDropDownRegion[i]);

	//disable the close region
	MSYS_DisableRegion(&mSelectedCloseDropDownRegion);
	
	mfMouseRegionsCreated_Drop	= FALSE;

	SetRefresh();
}

void
DropDownBase::Display()
{
	if( !mfMouseRegionsCreated )
		return;

	HVOBJECT	hArrowHandle;
			
	//Display the background for the drop down window
	ColorFillVideoSurfaceArea( FRAME_BUFFER, musStartX, musStartY, musStartX + musWidth + musArrowWidth, musStartY + DEF_SCROLL_ARROW_HEIGHT, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

	// top
	Display2Line2ShadowHorizontal( musStartX, musStartY, musStartX + musWidth + musArrowWidth - 1, mColorLine, mColorLineShadow );

	// left
	Display2Line2ShadowVertical( musStartX, musStartY+2, musStartY + DEF_SCROLL_ARROW_HEIGHT - 1, mColorLine, mColorLineShadow );

	// right between text and arrow
	Display2Line2ShadowVertical( musStartX + musWidth - 4, musStartY+2, musStartY + DEF_SCROLL_ARROW_HEIGHT - 1, mColorLine, mColorLineShadow );
		
	// bottom
	Display2Line2ShadowHorizontal( musStartX, musStartY + DEF_SCROLL_ARROW_HEIGHT - 4, musStartX + musWidth + musArrowWidth - 1, mColorLine, mColorLineShadow );
	
	// right
	Display2Line2ShadowVertical( musStartX + musWidth + musArrowWidth - 4, musStartY, musStartY + DEF_SCROLL_ARROW_HEIGHT - 3, mColorLine, mColorLineShadow );
	
	DrawTopEntry();

	//get and display the up and down arrows
	GetVideoObject(&hArrowHandle, muiGoldArrowImages);
	//top arrow
	BltVideoObject(FRAME_BUFFER, hArrowHandle, 0, musUpArrowX, musUpArrowY, VO_BLT_SRCTRANSPARENCY,NULL);
		
	// Set region help text
	if ( StringPixLength ( mHelpText, DEF_DROPDOWN_FONT ) > 0 )
	{
		SetRegionFastHelpText( &mBubbleHelpRegion, mHelpText );
		SetRegionHelpEndCallback( &mBubbleHelpRegion, MSYS_NO_CALLBACK );
	}
	
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);

	// display dropped region if necessary
	Display_Drop();
}


void
DropDownBase::Display_Drop()
{
	if( !mfMouseRegionsCreated_Drop )
		return;

	HVOBJECT	hArrowHandle;
		
	//Display the background for the drop down window
	ColorFillVideoSurfaceArea( FRAME_BUFFER, musStartX_Drop, musStartY_Drop, musStartX_Drop+musWidth+musArrowWidth,	musStartY_Drop+musAreaHeight, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

	// top
	Display2Line2ShadowHorizontal( musStartX_Drop, musStartY_Drop-4, musStartX_Drop + musWidth + musArrowWidth - 1, mColorLine, mColorLineShadow );

	// left
	Display2Line2ShadowVertical( musStartX_Drop, musStartY_Drop-2, musStartY_Drop + musAreaHeight - 1, mColorLine, mColorLineShadow );

	// right between text and arrow
	Display2Line2ShadowVertical( musStartX_Drop + musWidth - 4, musStartY_Drop-2, musStartY_Drop + musAreaHeight - 1, mColorLine, mColorLineShadow );
		
	// bottom
	Display2Line2ShadowHorizontal( musStartX_Drop, musStartY_Drop + musAreaHeight - 4, musStartX_Drop + musWidth + musArrowWidth - 1, mColorLine, mColorLineShadow );
	
	// right
	Display2Line2ShadowVertical( musStartX_Drop + musWidth + musArrowWidth - 4, musStartY_Drop-4, musStartY_Drop + musAreaHeight - 3, mColorLine, mColorLineShadow );

	DrawSelectedCity();
	
	//get and display the up and down arrows
	GetVideoObject(&hArrowHandle, muiGoldArrowImages);
	// top arrow
	BltVideoObject(FRAME_BUFFER, hArrowHandle, 1, musUpArrowX, musUpArrowY, VO_BLT_SRCTRANSPARENCY,NULL);

	// down arrow
	BltVideoObject(FRAME_BUFFER, hArrowHandle, 0, musDownArrowX, musDownArrowY, VO_BLT_SRCTRANSPARENCY,NULL);

	//display the scroll rectangle
	DrawGoldRectangle();

	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void
DropDownBase::DrawTopEntry()
{
	if ( mSelectedEntry >= mEntryVector.size() )
		return;

	//display the name in the list
	ColorFillVideoSurfaceArea( FRAME_BUFFER, musStartX+4, musStartY+4, musStartX+musWidth-4, musStartY+musFontHeight+8, mColorMarked );

	SetFontShadow(NO_SHADOW);

	DrawTextToScreen( mEntryVector[mSelectedEntry].second, musStartX+CITY_NAME_OFFSET, (UINT16)(musStartY+7), 0, DEF_DROPDOWN_FONT, FONT_BLACK, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);;

	SetFontShadow(DEFAULT_SHADOW);
}

void
DropDownBase::DrawSelectedCity()
{
	UINT16 usPosY = musStartY_Drop;
	UINT16 usMaxY = 0;

	// make sure we dont go out of our array
	if( mFirstShownEntry+mNumDisplayedEntries > (UINT8)(mEntryVector.size() ) )
		mFirstShownEntry = mEntryVector.size() - mNumDisplayedEntries;
	
	for( UINT8 i = mFirstShownEntry; i < mFirstShownEntry+mNumDisplayedEntries; ++i)
	{
		usMaxY = min(usPosY+musFontHeight+2, musStartY_Drop + musAreaHeight - 4);

		if ( i == mSelectedEntry )
		{
			ColorFillVideoSurfaceArea( FRAME_BUFFER, musStartX_Drop+4, usPosY-2, musStartX_Drop+musWidth-4,	usMaxY, mColorMarked );
			SetFontShadow(NO_SHADOW);
			DrawTextToScreen( mEntryVector[i].second, musStartX_Drop+CITY_NAME_OFFSET, usPosY, 0, DEF_DROPDOWN_FONT, FONT_BLACK, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);
			SetFontShadow(DEFAULT_SHADOW);
		}
		else
		{
			ColorFillVideoSurfaceArea( FRAME_BUFFER, musStartX_Drop+4, usPosY-2, musStartX_Drop+musWidth-4,	usMaxY, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
			DrawTextToScreen( mEntryVector[i].second, musStartX_Drop+CITY_NAME_OFFSET, usPosY, 0, DEF_DROPDOWN_FONT, STATIC_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);
		}

		usPosY += musFontHeight + 4;
	}
}

void
DropDownBase::DrawGoldRectangle()
{
	UINT32 uiDestPitchBYTES;
	UINT8	*pDestBuf;
	UINT16 usWidth, usTempHeight, usTempPosY, usHeight;
	UINT16 usPosX, usPosY;

	UINT16 temp;
	
	usTempPosY = musStartY;
	usTempPosY += DEF_SCROLL_ARROW_HEIGHT;
	usPosX = musScrollAreaX;
	usWidth = musArrowWidth - 5;
	usTempHeight = ( musAreaHeight - DEF_SCROLL_ARROW_HEIGHT ) - 4;

	usHeight = usTempHeight / (mEntryVector.size() - 1);

	usPosY = usTempPosY + (UINT16)( ( usHeight * mSelectedEntry ) );

	temp = musStartY_Drop + musAreaHeight - DEF_SCROLL_ARROW_HEIGHT - usHeight;

	if( usPosY >= temp )
		usPosY = musStartY_Drop + musAreaHeight - DEF_SCROLL_ARROW_HEIGHT - usHeight - 5;

	// color everything black and then color te rectangle, that way we dont have to redraw the entire page
	ColorFillVideoSurfaceArea( FRAME_BUFFER, musScrollAreaX, usTempPosY, musScrollAreaX+usWidth, usTempPosY+usTempHeight, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
	ColorFillVideoSurfaceArea( FRAME_BUFFER, musScrollAreaX, usPosY, musScrollAreaX+usWidth, usPosY+usHeight, mColorMarked );

	//display the line
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// draw the gold highlite line on the top and left
	LineDraw(FALSE, usPosX, usPosY, usPosX+usWidth, usPosY, mColorHighLight, pDestBuf);
	LineDraw(FALSE, usPosX, usPosY, usPosX, usPosY+usHeight, mColorHighLight, pDestBuf);

	// draw the shadow line on the bottom and right
	LineDraw(FALSE, usPosX, usPosY+usHeight, usPosX+usWidth, usPosY+usHeight, mColorLineShadow, pDestBuf);
	LineDraw(FALSE, usPosX+usWidth, usPosY, usPosX+usWidth, usPosY+usHeight, mColorLineShadow, pDestBuf);

	// unlock frame buffer
	UnLockVideoSurface( FRAME_BUFFER );
}

void
DropDownBase::SelectDropDownMovementCallBack(MOUSE_REGION * pRegion, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		pRegion->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
	else if( reason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		pRegion->uiFlags |= BUTTON_CLICKED_ON ;

		mSelectedEntry = (UINT8)MSYS_GetRegionUserData( pRegion, 0 ) + mFirstShownEntry;
		
		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
		
		DrawTopEntry();
		DrawSelectedCity();
		DrawGoldRectangle();
	}
}

void
DropDownBase::SelectDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT8 ubSelected = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );
		mSelectedEntry = min(ubSelected + mFirstShownEntry, mEntryVector.size() -1) ;

		Destroy_Drop();
	}
}

void
DropDownBase::SelectUpDownArrowOnScrollAreaRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP || iReason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		UINT8	ubDownArrow = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );

		if( ubDownArrow )
		{
			if( mSelectedEntry < mEntryVector.size() - 1 )
			{
				mSelectedEntry++;
			}

			if( ( mSelectedEntry - mFirstShownEntry ) >= mNumDisplayedEntries && mFirstShownEntry <= mEntryVector.size() - 1 - mNumDisplayedEntries )
			{
				mFirstShownEntry++;
			}
		}
		else
		{
			if( mSelectedEntry > 0 )
				mSelectedEntry--;

			if( mSelectedEntry < mFirstShownEntry )
				mFirstShownEntry--;
		}
		
		DrawTopEntry();
		DrawSelectedCity();
		DrawGoldRectangle();
	}
}

void
DropDownBase::SelectScrollAreaDropDownMovementCallBack(MOUSE_REGION * pRegion, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		pRegion->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
	else if (reason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( gfLeftButtonState )
		{
			UINT8	ubCityNum = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );

			pRegion->uiFlags |= BUTTON_CLICKED_ON;
			
			if( mFirstShownEntry + ubCityNum < mSelectedEntry )
			{
				mSelectedEntry = max(mFirstShownEntry + ubCityNum, 0);

				if( ubCityNum == 0 )
					mFirstShownEntry = max(mFirstShownEntry - 1, 0);
			}
			else if( mFirstShownEntry + ubCityNum > mSelectedEntry )
			{
				mSelectedEntry = min(mFirstShownEntry + ubCityNum, mEntryVector.size() - 1);

				if ( ubCityNum == mNumDisplayedEntries - 1 )
					mFirstShownEntry = min(mFirstShownEntry + 1, mEntryVector.size() - 1);
			}
			
			InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
			
			DrawTopEntry();
			DrawSelectedCity();
			DrawGoldRectangle();
		}
	}
}

void
DropDownBase::SelectScrollAreaDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT8	ubCityNum = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );

		if( ubCityNum < mSelectedEntry )
		{
			mSelectedEntry--;
			if( mSelectedEntry < mFirstShownEntry )
				mFirstShownEntry--;
		}
		else if( ubCityNum > mSelectedEntry )
		{
			mSelectedEntry++;
			if( ( mSelectedEntry - mFirstShownEntry ) >= mNumDisplayedEntries )
				mFirstShownEntry++;
		}
		
		DrawTopEntry();
		DrawSelectedCity();
		DrawGoldRectangle();
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT)
	{
		UINT8	ubCityNum = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );

		pRegion->uiFlags |= BUTTON_CLICKED_ON ;

		if( ubCityNum < mSelectedEntry )
		{
			mSelectedEntry--;
			if( mSelectedEntry < mFirstShownEntry )
				mFirstShownEntry--;
		}
		else if( ubCityNum > mSelectedEntry )
		{
			mSelectedEntry++;
			if( ( mSelectedEntry - mFirstShownEntry ) >= mNumDisplayedEntries )
				mFirstShownEntry++;
		}
		
		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
		
		DrawTopEntry();
		DrawSelectedCity();
		DrawGoldRectangle();
	}
}

void
DropDownBase::OpenDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		Create_Drop();
		Display_Drop();
	}
}

void
DropDownBase::CloseDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		Destroy_Drop();
	}
}

