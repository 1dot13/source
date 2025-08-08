/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "BaseTable.h"
#include "Font.h"
#include "laptop.h"
#include "Cursors.h"
#include "Utilities.h"
#include "vobject.h"
#include "WCheck.h"
#include "line.h"
#include "WordWrap.h"
#include "input.h"		// for gfLeftButtonState
#include "insurance.h"	// for DisplaySmallColouredLineWithShadow
#include "Sound Control.h"

#define	TESTTABLE_FONT_MED					FONT12ARIAL
#define TESTTABLE_OFFSET_ROW				20
#define	TESTTABLE_FONT_COLOR_REGULAR		2

BaseTable::BaseTable( )
: WidgetBase( ),
	mName(L""),
	fInit(FALSE),
	musHeight(0),
	musWidth(0),
	musFrameWorkSpace(0),
	mActive(TRUE),
	mMouseRegionActive(TRUE)
{
	SetColorLine( Get16BPPColor( FROMRGB( 128, 128, 128 ) ) );
	SetColorLineShadow( Get16BPPColor( FROMRGB( 255, 78, 255 ) ) );
	SetColorMarked( Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );
	SetColorHighLight( Get16BPPColor( FROMRGB( 235, 222, 171 ) ) );
}

void
BaseTable::Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	SetX( sX );
	SetY( sY );

	SetWidth( sX_End - sX );
	SetHeight( sY_End - sY );

	for ( std::vector<BaseTable*>::iterator it = mRegisteredMembers.begin( ); it != mRegisteredMembers.end( ); ++it )
	{
		if ( (*it) )
			(*it)->Init( sX, sY, sX_End, sY_End );
	}

	SetInit( TRUE );
}

void
BaseTable::Create( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	Destroy( );

	Init( sX, sY, sX_End, sY_End );
}

void
BaseTable::Destroy( )
{
	if ( !IsInit() )
		return;

	DestroyMouseRegions();

	for ( std::vector<BaseTable*>::iterator it = mRegisteredMembers.begin( ); it != mRegisteredMembers.end( ); ++it )
	{
		if ( (*it) )
			(*it)->Destroy( );
	}

	mRegisteredMembers.clear( );

	SetInit(FALSE);	

	SetRefresh( );
}

void
BaseTable::Display( )
{
	if ( !IsInit() )
		return;
	
	for ( std::vector<BaseTable*>::iterator it = mRegisteredMembers.begin( ); it != mRegisteredMembers.end( ); ++it )
	{
		if ( (*it) && (*it)->IsActive() )
			(*it)->Display( );
	}
}

void
BaseTable::DestroyMouseRegions( )
{
	if ( !IsInit( ) )
		return;

	for ( std::vector<BaseTable*>::iterator it = mRegisteredMembers.begin( ); it != mRegisteredMembers.end( ); ++it )
	{
		if ( (*it) )
			(*it)->DestroyMouseRegions( );
	}
}

STR16 cdp_string_func_dummy( UINT32 aNum )
{ 
	return L"nothing found";
}

UINT8 cdp_textcolour_func_dummy( UINT32 aNum )
{ 
	return TESTTABLE_FONT_COLOR_REGULAR;
}

void cdp_image_func_dummy( UINT32 aNum, UINT32& arImageLib, UINT16& arImage )
{

}

void cdp_statusbar_func_dummy( UINT32 aId, UINT16& arCol1, UINT16& arVal1, UINT16& arCol2, UINT16& arVal2, UINT16& arCol3, UINT16& arVal3, UINT16& arCol4, UINT16& arVal4 )
{

}

void
ColumnDataProvider::CalcRequiredLength( )
{
	mRequiredLength = max( mRequiredLength, 6 * wcslen( mName ) );

	switch ( GetProviderType() )
	{
	case CDP_STRING:
		for ( UINT32 i = 0; i < mNumberOfEntries; ++i )
		{
			mRequiredLength = max( mRequiredLength, 6 * wcslen( GetString( i ) ) );
		}
		break;
		
	case CDP_IMAGE:
		/*for ( UINT32 i = 0; i < mNumberOfEntries; ++i )
		{
			// TODO...
		}*/
		break;

	case CDP_STATUSBAR:
		mRequiredLength = max( mRequiredLength, 4);
		break;
	}
}

extern void callbackmilitialist( INT32 usId );
extern void callbackmilitiadetail( INT32 usId );
extern void callbacktoggle( INT32 usId );

void
ColumnDataProvider::RegionClickCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CDP_CALLBACKTYPE chosenentry = (CDP_CALLBACKTYPE)(pRegion->UserData[0]);
		
		// do stuff
		switch ( chosenentry )
		{
		case CDP_MILITIA_LIST:
			callbackmilitialist( pRegion->UserData[1] );
			break;

		case CDP_MILITIADETAIL:
			callbackmilitiadetail( pRegion->UserData[1] );
			break;

		case CDP_TOGGLE:
			callbacktoggle( pRegion->UserData[1] );

			// we pretend to be a button, thus we make a sound
			PlayJA2Sample( BIG_SWITCH3_IN, RATE_11025, BTNVOLUME, 1, MIDDLEPAN );

			break;

		case CDP_DEFAULT:
			default:
				break;
		}
	}
	else if ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
	}
}

void
ColumnDataProvider::DestroyMouseRegions( )
{
	for ( int i = 0; i < COLUMNDATAPROVIDER_MOUSEREGIONS; ++i )
	{
		if ( mMouseRegion_Defined[i] )
		{
			MSYS_RemoveRegion( &mMouseRegion[i] );
			mMouseRegion_Defined[i] = FALSE;
		}
	}
}

// Flugente: this unused page will now be a testbed for the BaseTable development

////////////// TestPanel///////////////////////////////////////
TestPanel::TestPanel( )
: BaseTable( )
{
	SetFrameWorkSpace( 4 );
}

void
TestPanel::Display( )
{
	if ( !IsInit( ) )
		return;
	
	//Display the background 
	ColorFillVideoSurfaceArea( FRAME_BUFFER, GetX( ), GetY( ), GetX( ) + GetWidth( ), GetY( ) + GetHeight( ), GetColorMarked( ) );

	// framework
	// top
	Display2Line2ShadowHorizontal( GetX( ), GetY( ), GetX( ) + GetWidth( ), GetColorLine( ), GetColorLineShadow( ) );

	// bottom
	Display2Line2ShadowHorizontal( GetX( ), GetY( ) + GetHeight( ) - GetFrameWorkSpace( ) * 3 / 2, GetX( ) + GetWidth( ), GetColorLineShadow( ), GetColorLine( ) );

	// left
	Display2Line2ShadowVertical( GetX( ), GetY( ) + GetFrameWorkSpace( )/2, GetY( ) + GetHeight( ) - 5, GetColorLine( ), GetColorLineShadow( ) );

	// right
	Display2Line2ShadowVertical( GetX( ) + GetWidth( ) - GetFrameWorkSpace( ) * 3 / 2, GetY( ) + GetFrameWorkSpace( ) / 2, GetY( ) + GetHeight( ) - 5, GetColorLineShadow( ), GetColorLine( ) );

	BaseTable::Display( );
}

void
TestPanel::SetRefresh( )
{

}

TestPanel gTestPanel3;
////////////////////////// TestPanel /////////////////////////////////////

////////////////////////// TabBox /////////////////////////////////////
TabBox::TabBox( )
: BaseTable( )
{
	SetFrameWorkSpace(4);
}

void
TabBox::Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	SetColorLine( Get16BPPColor( FROMRGB( 128, 128, 128 ) ) );
	SetColorLineShadow( Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );
	SetColorMarked( Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );
	SetColorHighLight( Get16BPPColor( FROMRGB( 235, 222, 171 ) ) );
		
	BaseTable::Init( sX, sY, sX_End, sY_End );
}


void
TabBox::Create( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	Destroy( );

	Init( sX, sY, sX_End, sY_End );

	UINT16 usPosX = GetX( );

	UINT16 panelcnt = 0;
	for ( std::vector<TestPanelButtonStruct>::iterator it = mRegisteredTables.begin( ); it != mRegisteredTables.end( ); ++it )
	{
		if ( !(*it).mMouseRegion_Defined && IsMouseRegionActive() )
		{
			// determine required width of mouse region
			UINT32 textwidth = 7 * wcslen( (*it).mTestPanel->GetName( ) );

			MSYS_DefineRegion( &(*it).mMouseRegion, usPosX, GetY( ), usPosX + textwidth, GetY( ) + TESTTABLE_OFFSET_ROW, MSYS_PRIORITY_HIGH,
							   CURSOR_WWW,
							   RegionCallBackWrapper( (void*) this, CALLBACK_REGION_MOVE, &RegionDummyfunc ),
							   RegionCallBackWrapper( (void*) this, CALLBACK_REGION_CLICK, &RegionDummyfunc ) );
			MSYS_AddRegion( &(*it).mMouseRegion );

			MSYS_SetRegionUserData( &(*it).mMouseRegion, 0, panelcnt );
			(*it).mMouseRegion_Defined = TRUE;
			++panelcnt;

			usPosX += textwidth;
		}
	}
}

void
TabBox::Destroy( )
{
	if ( !IsInit( ) )
		return;

	for ( std::vector<TestPanelButtonStruct>::iterator it = mRegisteredTables.begin( ); it != mRegisteredTables.end( ); ++it )
	{
		if ( (*it).mMouseRegion_Defined )
		{
			MSYS_RemoveRegion( &(*it).mMouseRegion );
			(*it).mMouseRegion_Defined = FALSE;
		}
	}

	mRegisteredTables.clear( );

	BaseTable::Destroy( );
}

void
TabBox::Display( )
{
	if ( !IsInit( ) )
		return;
	
	DestroyMouseRegions();

	CHAR16	sText[800];
	swprintf( sText, L"" );
	UINT16 usPosX = GetX( );

	UINT16 panelcnt = 0;

	UINT16 selectedx = 0;
	UINT16 selectedwidth = 0;

	SetFontShadow( NO_SHADOW );
	
	for ( std::vector<TestPanelButtonStruct>::iterator it = mRegisteredTables.begin( ); it != mRegisteredTables.end( ); ++it )
	{
		if ( IsActive() )
		{
			// determine required width
			UINT32 textwidth = 7 * wcslen( (*it).mTestPanel->GetName( ) );

			if ( (*it).mTestPanel->IsActive( ) )
			{
				ColorFillVideoSurfaceArea( FRAME_BUFFER,
											usPosX, GetY( ),
											usPosX + textwidth, GetY( ) + TESTTABLE_OFFSET_ROW,
											GetColorMarked() );

				selectedx = usPosX;
				selectedwidth = textwidth;
			}
			else
			{
				ColorFillVideoSurfaceArea( FRAME_BUFFER,
											usPosX, GetY( ),
											usPosX + textwidth, GetY( ) + TESTTABLE_OFFSET_ROW,
											GetColorHighLight() );
			}
			
			// framework
			// top
			Display2Line2ShadowHorizontal( usPosX, GetY( ), usPosX + textwidth, GetColorLine( ), GetColorLineShadow( ) );

			// left
			Display2Line2ShadowVertical( usPosX, GetY( ) + GetFrameWorkSpace( ) / 2, GetY( ) + TESTTABLE_OFFSET_ROW - 1, GetColorLine( ), GetColorLineShadow( ) );

			// right
			Display2Line2ShadowVertical( usPosX + textwidth - 3, GetY( ) + GetFrameWorkSpace( ) / 2, GetY( ) + TESTTABLE_OFFSET_ROW, GetColorLineShadow( ), GetColorLine( ) );
		
			swprintf( sText, (*it).mTestPanel->GetName( ) );
			DrawTextToScreen( sText, usPosX + 7, GetY( ) + 7, GetWidth( ), TESTTABLE_FONT_MED, TESTTABLE_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

			// mouse region
			if ( !(*it).mMouseRegion_Defined && IsMouseRegionActive( ) )
			{
				MSYS_DefineRegion( &(*it).mMouseRegion, usPosX, GetY( ), usPosX + textwidth, GetY( ) + TESTTABLE_OFFSET_ROW, MSYS_PRIORITY_HIGH,
									CURSOR_WWW,
									RegionCallBackWrapper( (void*) this, CALLBACK_REGION_MOVE, &RegionDummyfunc ),
									RegionCallBackWrapper( (void*) this, CALLBACK_REGION_CLICK, &RegionDummyfunc ) );
				MSYS_AddRegion( &(*it).mMouseRegion );

				MSYS_SetRegionUserData( &(*it).mMouseRegion, 0, panelcnt );
				(*it).mMouseRegion_Defined = TRUE;
				++panelcnt;
			}

			usPosX += textwidth;
		}
	}

	BaseTable::Display( );

	// to indicate which frame is the active one we 'delete' the border to the content below
	ColorFillVideoSurfaceArea( FRAME_BUFFER,
							   selectedx + GetFrameWorkSpace( ), GetY( ) + TESTTABLE_OFFSET_ROW,
							   selectedx + selectedwidth - 3, GetY( ) + TESTTABLE_OFFSET_ROW + GetFrameWorkSpace( ),
							   GetColorMarked( ) );
	
	// left
	Display2Line2ShadowVertical( selectedx, GetY( ) + TESTTABLE_OFFSET_ROW - 3, GetY( ) + TESTTABLE_OFFSET_ROW + 1, GetColorLine( ), GetColorLineShadow( ) );

	// right
	Display2Line2ShadowVertical( selectedx + selectedwidth - 3, GetY( ) + TESTTABLE_OFFSET_ROW - 3, GetY( ) + TESTTABLE_OFFSET_ROW + 1, GetColorLineShadow( ), GetColorLine( ) );
}

void
TabBox::DestroyMouseRegions( )
{
	if ( !IsInit( ) )
		return;

	for ( std::vector<TestPanelButtonStruct>::iterator it = mRegisteredTables.begin( ); it != mRegisteredTables.end( ); ++it )
	{
		if ( (*it).mMouseRegion_Defined )
		{
			MSYS_RemoveRegion( &(*it).mMouseRegion );
			(*it).mMouseRegion_Defined = FALSE;
		}
	}

	BaseTable::DestroyMouseRegions( );
}

void
TabBox::RegionClickCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		for ( std::vector<TestPanelButtonStruct>::iterator it = mRegisteredTables.begin(); it != mRegisteredTables.end(); ++it )
		{
			if ( pRegion->IDNumber == (*it).mMouseRegion.IDNumber )
				(*it).mTestPanel->SetActive( TRUE );
			else
				(*it).mTestPanel->SetActive( FALSE );
		}

		SetRefresh( );
	}
	else if ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
	}
}

////////////////////////// TabBox /////////////////////////////////////

////////////////////////// TestTable /////////////////////////////////////
#define SCROLLARROW_WIDTH			20
#define SCROLLARROW_HEIGTH			20

TestTable::TestTable( )
: BaseTable( ),
mScrollBarDefined( FALSE ),
mFirstEntryShown(0),
mLastEntryShown( 0 ),
mColorSeparator( FROMRGB( 0, 255, 0 ) )
{
}

void
TestTable::Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	SetX( sX );
	SetY( sY );

	SetHeight( sY_End - sY );
	SetWidth( sX_End - sX );
	
	CalcRows( );

	SetInit( TRUE );
}

extern UINT32 guiHelpScreenBackGround;

void
TestTable::Create( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	VOBJECT_DESC	VObjectDesc;
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "INTERFACE\\HelpScreen.sti", VObjectDesc.ImageFile );
	if ( !AddVideoObject( &VObjectDesc, &guiHelpScreenBackGround ) )
		return;

	Destroy( );

	Init( sX, sY, sX_End, sY_End );
}

void
TestTable::Destroy( )
{
	if ( !IsInit( ) )
		return;

	if ( mScrollBarDefined )
	{
		for ( UINT16 i = 0; i < 2; ++i )
		{
			RemoveButton( mScrollArrow[i] );
			UnloadButtonImage( mScrollArrowImage[i] );
		}

		MSYS_RemoveRegion( &mScrollRegion );

		mScrollBarDefined = FALSE;
	}

	for ( std::vector<ColumnDataProvider>::iterator it = mColumnDataProviderVector.begin( ), itend = mColumnDataProviderVector.end(); it != itend; ++it )
	{
		(*it).DestroyMouseRegions();
	}

	ClearColumnDataProvider();

	DeleteVideoObjectFromIndex( guiHelpScreenBackGround );

	SetInit( FALSE );

	SetRefresh( );
}

void
TestTable::Display( )
{
	if ( !IsInit( ) )
		return;

	CHAR16	sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;

	usPosX = GetX( );
	usPosY = GetY( );

	DestroyMouseRegions();

	if ( IsActive() )
	{
		for ( std::vector<ColumnDataProvider>::iterator it = mColumnDataProviderVector.begin( ); it != mColumnDataProviderVector.end( ); ++it )
		{
			swprintf( sText, (*it).GetName( ) );
			DrawTextToScreen( sText, usPosX, usPosY, GetWidth( ), TESTTABLE_FONT_MED, TESTTABLE_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

			usPosX += (*it).GetRequiredLength( );
		}

		usPosY += TESTTABLE_OFFSET_ROW - 4;

		UINT32 heightperrow = GetRequiredHeigthPerRow( );

		for ( UINT32 i = mFirstEntryShown; i < mLastEntryShown; ++i )
		{
			usPosX = GetX( );

			DisplaySmallColouredLineWithShadow( usPosX, usPosY - 2, usPosX + GetWidth( ) - SCROLLARROW_WIDTH, usPosY - 2, GetColorSeparator() );
			
			for ( std::vector<ColumnDataProvider>::iterator it = mColumnDataProviderVector.begin( ), itend = mColumnDataProviderVector.end(); it != itend; ++it )
			{
				if ( it->GetProviderType() == ColumnDataProvider::CDP_STRING )
				{
					swprintf( sText, ( *it ).GetString( i ) );

					DrawTextToScreen( sText, usPosX, usPosY + 7, GetWidth(), TESTTABLE_FONT_MED, ( *it ).GetColour( i ), FONT_MCOLOR_BLACK, FALSE, 0 );
				}
				else if ( it->GetProviderType() == ColumnDataProvider::CDP_IMAGE )
				{
					UINT32 imagelib = 0;
					UINT16 imageid = 0;

					it->GetImageData( i, imagelib, imageid );

					BltVideoObjectFromIndex( FRAME_BUFFER, imagelib, imageid, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
				else if ( it->GetProviderType() == ColumnDataProvider::CDP_STATUSBAR )
				{
					UINT16 col1 = 0, col2 = 0, col3 = 0, col4 = 0, val1 = 0, val2 = 0, val3 = 0, val4 = 0;

					it->GetStatusBarData( i, col1, val1, col2, val2, col3, val3, col4, val4 );

					UINT16 height1 = ( heightperrow * val1 ) / 100;
					UINT16 height2 = ( heightperrow * val2 ) / 100;
					UINT16 height3 = ( heightperrow * val3 ) / 100;
					UINT16 height4 = ( heightperrow * val4 ) / 100;

					UINT16 endY = usPosY + heightperrow - 1;

					// first a grey background (that way we get a grey 'border' for our bar, then the content
					UINT16 bg = Get16BPPColor( FROMRGB( 158, 158, 158 ) );
					DisplaySmallLine( usPosX + 0, usPosY, usPosX + 0, endY, bg );
					DisplaySmallLine( usPosX + 1, usPosY, usPosX + 1, endY, bg );
					DisplaySmallLine( usPosX + 2, usPosY, usPosX + 2, endY, bg );
					DisplaySmallLine( usPosX + 3, usPosY, usPosX + 3, endY, bg );

					DisplaySmallLine( usPosX + 1, endY - height1, usPosX + 1, endY, col1 );
					DisplaySmallLine( usPosX + 2, endY - height1, usPosX + 2, endY, col1 );
					DisplaySmallLine( usPosX + 1, endY - height2, usPosX + 1, endY, col2 );
					DisplaySmallLine( usPosX + 2, endY - height2, usPosX + 2, endY, col2 );
					DisplaySmallLine( usPosX + 1, endY - height3, usPosX + 1, endY, col3 );
					DisplaySmallLine( usPosX + 2, endY - height3, usPosX + 2, endY, col3 );
					DisplaySmallLine( usPosX + 1, endY - height4, usPosX + 1, endY, col4 );
					DisplaySmallLine( usPosX + 2, endY - height4, usPosX + 2, endY, col4 );
				}

				if ( i - mFirstEntryShown < COLUMNDATAPROVIDER_MOUSEREGIONS && IsMouseRegionActive() )
				{
					MSYS_DefineRegion( &it->mMouseRegion[i - mFirstEntryShown],
						usPosX, usPosY, usPosX + it->GetRequiredLength(), usPosY + heightperrow,
						MSYS_PRIORITY_HIGHEST-3, CURSOR_WWW,
						MSYS_NO_CALLBACK, ( *it ).RegionClickCallBack );
					MSYS_AddRegion( &it->mMouseRegion[i - mFirstEntryShown] );

					// first entry of userdata is the callback type - we need to know what kind of callback function should be called
					MSYS_SetRegionUserData( &it->mMouseRegion[i - mFirstEntryShown], 0, it->GetCallBackType() );

					// second entry: the id to use as input in the callback
					MSYS_SetRegionUserData( &it->mMouseRegion[i - mFirstEntryShown], 1, i );

					it->mMouseRegion_Defined[i - mFirstEntryShown] = TRUE;
				}

				usPosX += (*it).GetRequiredLength( );
			}

			usPosY += heightperrow;
		}

		UINT32 shownentries = GetNumberOfDataRowsShown( );
		UINT32 totalentries = GetNumberOfDataRows( );

		// do we need scrollbars in the first place?
		if ( shownentries < totalentries )
		{
			// length of scrollbar and length of scrollbar element
			UINT32 scrollbarheigth = GetHeight( ) - 2 * SCROLLARROW_HEIGTH;
			UINT32 scrollareaheigth = (scrollbarheigth * shownentries) / totalentries;
			UINT32 areabegin = (scrollbarheigth * mFirstEntryShown) / totalentries;

			ColorFillVideoSurfaceArea( FRAME_BUFFER,
									   GetX( ) + GetWidth( ) - SCROLLARROW_WIDTH - 2,
									   GetY( ) + SCROLLARROW_HEIGTH,
									   GetX( ) + GetWidth( ) - 2,
									   GetY( ) + SCROLLARROW_HEIGTH + GetHeight( ) - SCROLLARROW_HEIGTH,
									   GetColorLineShadow( ) );

			ColorFillVideoSurfaceArea( FRAME_BUFFER,
									   GetX( ) + GetWidth( ) - SCROLLARROW_WIDTH - 2,
									   GetY( ) + SCROLLARROW_HEIGTH + areabegin,
									   GetX( ) + GetWidth( ) - 2,
									   GetY( ) + SCROLLARROW_HEIGTH + areabegin + scrollareaheigth,
									   GetColorLine() );

			CreateScrollAreaButtons( );
		}
	}

	BaseTable::Display( );
}

void
TestTable::DestroyMouseRegions()
{
	if ( !IsInit( ) )
		return;

	std::vector<ColumnDataProvider>::iterator itend = mColumnDataProviderVector.end( );
	for ( std::vector<ColumnDataProvider>::iterator it = mColumnDataProviderVector.begin( ); it != itend; ++it )
	{
		(*it).DestroyMouseRegions( );
	}

	if ( mScrollBarDefined )
	{
		for ( UINT16 i = 0; i < 2; ++i )
		{
			RemoveButton( mScrollArrow[i] );
			UnloadButtonImage( mScrollArrowImage[i] );
		}

		MSYS_RemoveRegion( &mScrollRegion );

		mScrollBarDefined = FALSE;
	}

	BaseTable::DestroyMouseRegions( );
}

void
TestTable::RegionMoveCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		//		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
	else if ( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_MOVE )
	{
		if ( gfLeftButtonState )
		{
			// if mouse is outside of the bar, simply move bar up or down
			if ( pRegion->MouseYPos < mScrollRegion.RegionTopLeftY )
			{
				mLastEntryShown = max( 0, mLastEntryShown - 1 );
			}
			else if ( pRegion->MouseYPos > mScrollRegion.RegionBottomRightY )
			{
				++mLastEntryShown;
			}
			// inside the scroll bar, determine where the middle of the slider is to determine where to go
			else
			{
				// length of scrollbar
				UINT32 scrollbarheigth = GetHeight( ) - 2 * SCROLLARROW_HEIGTH;

				if ( scrollbarheigth > 0  )
				{					
					FLOAT rate = (FLOAT)(pRegion->MouseYPos - GetY( ) + SCROLLARROW_HEIGTH) / (FLOAT)(scrollbarheigth);

					mLastEntryShown = rate * GetNumberOfDataRows();
				}
			}

			// simply altering the position of mLastEntryShown and the recalculating the rows does the trick
			CalcRows();

			SetRefresh( );
		}
	}
}

void
TestTable::RegionClickCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	iReason = iReason;

	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		//gfScrollBoxIsScrolling = FALSE;
		//gHelpScreen.iLastMouseClickY = -1;
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		//gfScrollBoxIsScrolling = TRUE;
		//HelpScreenMouseMoveScrollBox( pRegion->MouseYPos );
	}
	else if ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
	}
}

void
TestTable::ButtonClickCallBack( GUI_BUTTON *btn, INT32 reason )
{
	if ( reason & MSYS_CALLBACK_REASON_INIT )
	{
		return;
	}
	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		// up-arrow
		if ( btn->IDNum == this->mScrollArrow[0] )
		{
			// do things!
			if ( mFirstEntryShown )
				--mFirstEntryShown;

			mLastEntryShown = mFirstEntryShown + GetNumberOfDataRowsShown( );
		}
		// down-arrow
		else if ( btn->IDNum == this->mScrollArrow[1] )
		{
			// do slightly different things!
			mLastEntryShown = min( mLastEntryShown + 1, GetNumberOfDataRows( ) );

			mFirstEntryShown = mLastEntryShown - GetNumberOfDataRowsShown( );
		}
		// this should not happen
		else
		{
			return;
		}

		SetRefresh( );
	}
	else if ( reason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		// nothing yet
	}
}

void
TestTable::CalcRows()
{
	// don't show more entries than exist
	UINT32 numrows	 = GetNumberOfDataRows( );
	UINT32 rowsshown = GetNumberOfDataRowsShown( );

	mLastEntryShown = max( mLastEntryShown, rowsshown );
	mLastEntryShown = min( mLastEntryShown, numrows );

	if ( mLastEntryShown > rowsshown )
		mFirstEntryShown = mLastEntryShown - rowsshown;
	else
		mFirstEntryShown = 0;

	mLastEntryShown = mFirstEntryShown + rowsshown;
}

void
TestTable::CreateScrollAreaButtons( )
{
	if ( !mScrollBarDefined )
	{
		if ( IsMouseRegionActive() )
		{
			MSYS_DefineRegion( &mScrollRegion, GetX( ) + GetWidth( ) - SCROLLARROW_WIDTH - 2, GetY( ), GetX( ) + GetWidth( ) - 2, GetY( ) + GetHeight( ) - 2, MSYS_PRIORITY_HIGHEST,
							   CURSOR_WWW,
							   RegionCallBackWrapper( (void*) this, CALLBACK_REGION_MOVE, &RegionDummyfunc ),
							   RegionCallBackWrapper( (void*) this, CALLBACK_REGION_CLICK, &RegionDummyfunc ) );
			MSYS_AddRegion( &mScrollRegion );
			MSYS_SetRegionUserData( &mScrollRegion, 0, 0 );

			mScrollArrowImage[0] = LoadButtonImage( "INTERFACE\\HelpScreen.sti", 14, 10, 11, 12, 13 );
			mScrollArrowImage[1] = UseLoadedButtonImage( mScrollArrowImage[0], 19, 15, 16, 17, 18 );

			//Create the scroll arrows
			mScrollArrow[0] = QuickCreateButton( mScrollArrowImage[0], GetX( ) + GetWidth( ) - SCROLLARROW_WIDTH - 2, GetY( ),
												 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
												 ButtonCallBackWrapper( (void*) this, CALLBACK_BUTTON_MOVE, &ButtonDummyfunc ),
												 ButtonCallBackWrapper( (void*) this, CALLBACK_BUTTON_CLICK, &ButtonDummyfunc ) );
			MSYS_SetBtnUserData( mScrollArrow[0], 0, 0 );

			//Create the scroll arrows
			mScrollArrow[1] = QuickCreateButton( mScrollArrowImage[1], GetX( ) + GetWidth( ) - SCROLLARROW_WIDTH - 2, GetY( ) + GetHeight( ) - SCROLLARROW_HEIGTH - 2,
												 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
												 ButtonCallBackWrapper( (void*) this, CALLBACK_BUTTON_MOVE, &ButtonDummyfunc ),
												 ButtonCallBackWrapper( (void*) this, CALLBACK_BUTTON_CLICK, &ButtonDummyfunc ) );
			MSYS_SetBtnUserData( mScrollArrow[1], 0, 1 );

			mScrollBarDefined = TRUE;
		}
		else
		{
			// if buttons are disabled, we'll display the images of the buttons instead
			HVOBJECT	hArrowHandle;

			//get and display the up and down arrows
			GetVideoObject( &hArrowHandle, guiHelpScreenBackGround );

			if ( hArrowHandle )
			{
				// top arrow
				BltVideoObject( FRAME_BUFFER, hArrowHandle, 10, 
								GetX( ) + GetWidth( ) - SCROLLARROW_WIDTH - 2, 
								GetY( ), 
								VO_BLT_SRCTRANSPARENCY, NULL );

				// down arrow
				BltVideoObject( FRAME_BUFFER, hArrowHandle, 17, 
								GetX( ) + GetWidth( ) - SCROLLARROW_WIDTH - 2,
								GetY( ) + GetHeight( ) - SCROLLARROW_HEIGTH - 2, 
								VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}
	}
}

UINT32
TestTable::GetNumberOfDataRows( )
{
	BOOLEAN fFound = FALSE;
	UINT32 minrowsinproviders = 1000;
	for ( std::vector<ColumnDataProvider>::iterator it = mColumnDataProviderVector.begin( ), itend = mColumnDataProviderVector.end(); it != itend; ++it )
	{
		minrowsinproviders = min( minrowsinproviders, (*it).GetNumberOfEntries( ) );

		fFound = TRUE;
	}

	if ( fFound && minrowsinproviders )
		return minrowsinproviders;

	return 0;
}

UINT32
TestTable::GetNumberOfDataRowsShown( )
{
	// determine total height that can be used for display - we have to substract space for the header and the column headers
	UINT32 heightperrow = GetRequiredHeigthPerRow();

	if ( !heightperrow )
		return 0;

	UINT32 totalheighttodistribute = GetHeight( );

	if ( totalheighttodistribute < heightperrow + TESTTABLE_OFFSET_ROW )
		return 0;

	totalheighttodistribute -= TESTTABLE_OFFSET_ROW;
	
	UINT32 rows = GetNumberOfDataRows( );

	if ( !rows )
		return 0;

	// we need TESTTABLE_OFFSET_ROW space for each row. We cannot display more than we have
	return min( totalheighttodistribute / heightperrow, rows );
}

UINT32
TestTable::GetRequiredHeigthPerRow( )
{
	UINT32 heigth = 0;
	for ( std::vector<ColumnDataProvider>::iterator it = mColumnDataProviderVector.begin( ), itend = mColumnDataProviderVector.end(); it != itend; ++it )
	{
		heigth = max( heigth, (*it).GetRequiredHeigth( ) );
	}

	return heigth;
}
////////////////////////// TestTable /////////////////////////////////////
