/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "MilitiaInterface.h"

#include "Font.h"
#include "laptop.h"
#include "Cursors.h"
#include "Utilities.h"
#include "vobject.h"
#include "WCheck.h"
#include "line.h"
#include "WordWrap.h"
#include "input.h"		// for gfLeftButtonState
#include "personnel.h"
#include "Animation Data.h"
#include "Text.h"
#include "MilitiaWebsite.h"
#include "Game Clock.h"
#include "Militia Control.h"
#include "Town Militia.h"
#include "Interface Items.h"
#include "Interface.h"		// for DIRTYLEVEL2

extern BOOLEAN gfMilitiaWebsiteMainRedraw;

UINT32	guTraitImages = 0;

MilitiaServiceRecordTable::MilitiaServiceRecordTable( )
: BaseTable( )
{
}

void
MilitiaServiceRecordTable::Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	SetColorLine( Get16BPPColor( FROMRGB( 128, 128, 128 ) ) );
	SetColorLineShadow( Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );
	SetColorMarked( Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );
	SetColorHighLight( Get16BPPColor( FROMRGB( 235, 222, 171 ) ) );

	BaseTable::Init( sX, sY, sX_End, sY_End );
}

void
MilitiaServiceRecordTable::Create( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	Destroy( );

	Init( sX, sY, sX_End, sY_End );

	TestTableTemplate<2>::getInstance( ).Create( sX + 4, sY + 14, sX_End, sY_End );
	RegisterMemberThingy( &TestTableTemplate<2>::getInstance( ) );
}

void
MilitiaServiceRecordTable::Destroy( )
{
	if ( !IsInit( ) )
		return;
	
	BaseTable::Destroy( );
}

void
MilitiaServiceRecordTable::Display( )
{
	if ( !IsInit( ) )
		return;

	DestroyMouseRegions( );

	SetFontShadow( NO_SHADOW );
	
	CHAR16	sText[800];
	swprintf( sText, L"" );
	UINT16 usPosX = GetX( ) + 10;
	UINT16 usPosY = GetY( ) + 10;
	
	MILITIA militia;
	if ( GetMilitia( mMilitiaId, &militia ) )
	{
		TestTableTemplate<2>::getInstance( ).ClearColumnDataProvider( );

		ColumnDataProvider namcol( szIdividualMilitiaWebsiteText[0] );
		namcol.SetMethodString( OperationText );
		namcol.SetNumberOfEntries( militia.history.size( ) );
		namcol.SetCallBackType( ColumnDataProvider::CDP_MILITIADETAIL );

		TestTableTemplate<2>::getInstance( ).AddColumnDataProvider( namcol );

		TestTableTemplate<2>::getInstance( ).CalcRows( );

		BaseTable::Display();
	}
}

void
MilitiaServiceRecordTable::SetRefresh( )
{
	gfMilitiaWebsiteMainRedraw = TRUE;
}

/////////////////////////////////////////////////////
MilitiaPersonalDataTable::MilitiaPersonalDataTable( )
: TestPanel( )
, mButtonFire(-1)
, mButtonFireCreated(FALSE)
{
}

void
MilitiaPersonalDataTable::Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	SetColorLine( Get16BPPColor( FROMRGB( 128, 128, 128 ) ) );
	SetColorLineShadow( Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );
	SetColorMarked( Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );
	SetColorHighLight( Get16BPPColor( FROMRGB( 235, 222, 171 ) ) );

	TestPanel::Init( sX, sY, sX_End, sY_End );
}

////// callback for buttons ////////
void
MilitiaPersonalDataTableFireConfirmationCallback( UINT8 val )
{
	if ( val == MSG_BOX_RETURN_YES )
	{
		MILITIA militia;
		if ( GetMilitia( gusCurrentMilitia, &militia ) )
		{
			if ( !(militia.flagmask & (MILITIAFLAG_DEAD | MILITIAFLAG_FIRED | MILITIAFLAG_DESERTION )) )
			{
				militia.Fire();

				UpdateMilitia( militia );
								
				StrategicRemoveMilitiaFromSector( SECTORX( militia.sector ), SECTORY( militia.sector ), militia.militiarank, 1 );
				
				ResetMilitia();

				militiaindividualmainwidget.SetRefresh();
			}
		}
	}
}

void MilitiaPersonalDataTableFireCallback( GUI_BUTTON *btn, INT32 reason )
{
	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		MILITIA militia;
		if ( GetMilitia( gusCurrentMilitia, &militia ) )
		{
			if ( !(militia.flagmask & (MILITIAFLAG_DEAD | MILITIAFLAG_FIRED | MILITIAFLAG_DESERTION )) )
			{
				CHAR16 sString[256];
				swprintf( sString, szIdividualMilitiaWebsiteText[1], militia.GetName( ) );

				DoLowerScreenIndependantMessageBox( sString, MSG_BOX_FLAG_YESNO, MilitiaPersonalDataTableFireConfirmationCallback );
			}
		}
	}
}
////// callback for buttons ////////

void
MilitiaPersonalDataTable::Create( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	Destroy( );

	Init( sX, sY, sX_End, sY_End );
}

void
MilitiaPersonalDataTable::Destroy( )
{
	if ( !IsInit( ) )
		return;

	if ( mButtonFireCreated )
	{
		RemoveButton( mButtonFire );
		mButtonFireCreated = FALSE;
	}

	TestPanel::Destroy( );
}

void
MilitiaPersonalDataTable::Display( )
{
	if ( !IsInit( ) )
		return;

	DestroyMouseRegions( );
	
	MILITIA militia;
	if ( GetMilitia( mMilitiaId, &militia ) )
	{
		CHAR16	sText[800];
		swprintf( sText, L"" );
		UINT16 usPosX = GetX( ) + 10;
		UINT16 usPosY = GetY( ) + 10;

		// show image of militia with correct visuals
		{
			VOBJECT_DESC	VObjectDesc;
			HVOBJECT hHandle;
			UINT32 uiGraphicHandle;
			BOOLEAN success = TRUE;
		
			// load it
			VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

			// Pick images based on body type
			switch ( militia.bodytype )
			{
			case REGMALE:		FilenameForBPP( "Laptop\\IMPSmallMercImages.sti", VObjectDesc.ImageFile );	break;
			case BIGMALE:		FilenameForBPP( "Laptop\\IMPBigMercImages.sti", VObjectDesc.ImageFile );	break;
			case STOCKYMALE:	FilenameForBPP( "Laptop\\IMPBigMercImages.sti", VObjectDesc.ImageFile );	break;
			case REGFEMALE:		FilenameForBPP( "Laptop\\IMPFemaleMercImages.sti", VObjectDesc.ImageFile ); break;
			default:
				success = FALSE;
				break;
			}

			// only show images if they can be loaded
			if ( success )
			{
				// safety check if loaded
				CHECKV( AddVideoObject( &VObjectDesc, &uiGraphicHandle ) );

				// Get it
				GetVideoObject( &hHandle, uiGraphicHandle );

				if ( hHandle )
				{
					UINT8 vest = GREENVEST;
					UINT8 pants = BEIGEPANTS;
					if ( militia.militiarank == REGULAR_MILITIA)
						vest = JEANVEST;
					else if ( militia.militiarank == ELITE_MILITIA)
						vest = BLUEVEST;

					// show background first
					BltVideoObject( FRAME_BUFFER, hHandle, 0, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
					// show skin (1 - 4)
					BltVideoObject( FRAME_BUFFER, hHandle, (militia.skin + 1), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
					// show head (5 - 9)
					BltVideoObject( FRAME_BUFFER, hHandle, (militia.hair + 5), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
					// show vest (10 - 20)
					BltVideoObject( FRAME_BUFFER, hHandle, (vest + 10), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
					// show pants (21 - 26)
					BltVideoObject( FRAME_BUFFER, hHandle, (pants + 21), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );

					DeleteVideoObjectFromIndex( uiGraphicHandle );
				}
			}
		}

		usPosX += 120;

		SetFontShadow( NO_SHADOW );
				
		if ( militia.flagmask & MILITIAFLAG_DEAD )
		{
			swprintf( sText, szIdividualMilitiaWebsiteText[5] );
			DrawTextToScreen( sText, usPosX, usPosY + 5, GetWidth( ), FONT12ARIAL, FONT_MCOLOR_RED, FONT_MCOLOR_BLACK, FALSE, 0 );
			usPosY += 20;
		}
		else if ( militia.flagmask & MILITIAFLAG_FIRED )
		{
			swprintf( sText, szIdividualMilitiaWebsiteText[6] );
			DrawTextToScreen( sText, usPosX, usPosY + 5, GetWidth( ), FONT12ARIAL, FONT_MCOLOR_DKGRAY, FONT_MCOLOR_BLACK, FALSE, 0 );
			usPosY += 20;
		}
		else if ( militia.flagmask & MILITIAFLAG_DESERTION )
		{
			swprintf( sText, szIdividualMilitiaWebsiteText[19] );
			DrawTextToScreen( sText, usPosX, usPosY + 5, GetWidth(), FONT12ARIAL, FONT_MCOLOR_DKGRAY, FONT_MCOLOR_BLACK, FALSE, 0 );
			usPosY += 20;
		}
		else
		{
			swprintf( sText, szIdividualMilitiaWebsiteText[7] );
			DrawTextToScreen( sText, usPosX, usPosY + 5, GetWidth( ), FONT12ARIAL, FONT_MCOLOR_LTGREEN, FONT_MCOLOR_BLACK, FALSE, 0 );
			usPosY += 20;
		}
		
		if ( gGameExternalOptions.fIndividualMilitia_ManageHealth )
		{
			swprintf( sText, szIdividualMilitiaWebsiteText[15], militia.healthratio );
			
			DrawTextToScreen( sText, usPosX, usPosY + 5, GetWidth( ), FONT12ARIAL, 2, FONT_MCOLOR_BLACK, FALSE, 0 );
			usPosY += 20;
		}
		
		swprintf( sText, szIdividualMilitiaWebsiteText[3], militia.GetWage( ), militia.age );
		DrawTextToScreen( sText, usPosX, usPosY + 5, GetWidth( ), FONT12ARIAL, 2, FONT_MCOLOR_BLACK, FALSE, 0 );
		usPosY += 20;

		swprintf( sText, szIdividualMilitiaWebsiteText[4], militia.kills, militia.assists );
		DrawTextToScreen( sText, usPosX, usPosY + 5, GetWidth( ), FONT12ARIAL, 2, FONT_MCOLOR_BLACK, FALSE, 0 );
		usPosY += 20;		

		usPosY += 4;
		usPosX -= 2;

		// button for firing this guy
		if ( mButtonFireCreated )
		{
			RemoveButton( mButtonFire );
			mButtonFireCreated = FALSE;
		}

		// if we can still fire this guy, set up the button
		if ( !(militia.flagmask & (MILITIAFLAG_DEAD | MILITIAFLAG_FIRED | MILITIAFLAG_DESERTION )) )
		{
			// we can only fire someone in sectors we control that are not in combat
			// if we've managed to misplace a militia into an impassable sector (HOW???), we can at least fire them, too
			if ( SectorOursAndPeaceful( SECTORX( militia.sector ), SECTORY( militia.sector ), 0 )
				|| SectorIsImpassable( militia.sector ) )
			{
				mButtonFire = CreateTextButton( szIdividualMilitiaWebsiteText[8], FONT12ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
												usPosX, usPosY, 120, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, MilitiaPersonalDataTableFireCallback );

				mButtonFireCreated = TRUE;
			}
			else
			{
				mButtonFire = CreateTextButton( szIdividualMilitiaWebsiteText[8], FONT12ARIAL, FONT_RED, FONT_BLACK, BUTTON_USE_DEFAULT,
												usPosX, usPosY, 120, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, NULL );

				mButtonFireCreated = TRUE;
			}
		}

		SetFontShadow( NO_SHADOW );
	}

	BaseTable::Display( );
}

void
MilitiaPersonalDataTable::SetRefresh( )
{
	gfMilitiaWebsiteMainRedraw = TRUE;
}

void
MilitiaPersonalDataTable::DestroyMouseRegions( )
{
	if ( !IsInit( ) )
		return;

	if ( mButtonFireCreated )
	{
		RemoveButton( mButtonFire );
		mButtonFireCreated = FALSE;
	}

	BaseTable::DestroyMouseRegions( );
}

/////////////////////////////////////////////
MilitiaPersonalInventoryTable::MilitiaPersonalInventoryTable( )
: TestPanel( )
{
}

void
MilitiaPersonalInventoryTable::Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	SetColorLine( Get16BPPColor( FROMRGB( 128, 128, 128 ) ) );
	SetColorLineShadow( Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );
	SetColorMarked( Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );
	SetColorHighLight( Get16BPPColor( FROMRGB( 235, 222, 171 ) ) );

	TestPanel::Init( sX, sY, sX_End, sY_End );
}

void
MilitiaPersonalInventoryTable::Create( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	Destroy( );

	Init( sX, sY, sX_End, sY_End );
}

void
MilitiaPersonalInventoryTable::Destroy( )
{
	if ( !IsInit( ) )
		return;

	TestPanel::Destroy( );
}

void
MilitiaPersonalInventoryTable::Display( )
{
	if ( !IsInit( ) )
		return;

	DestroyMouseRegions( );

	SetFontShadow( NO_SHADOW );

	CHAR16	sText[800];
	swprintf( sText, L"" );
	UINT16 usPosX = GetX( ) + 10;
	UINT16 usPosY = GetY( ) + 10;

	SOLDIERTYPE* pSoldier = GetUsedSoldierToIndividualMilitia( mMilitiaId );

	if ( pSoldier )
	{
		SetFontShadow( DEFAULT_SHADOW );

		for ( UINT8 pocketIndex = HELMETPOS; pocketIndex < NUM_INV_SLOTS; ++pocketIndex )
		{
			OBJECTTYPE* pObj = &(pSoldier)->inv[pocketIndex];

			if ( pObj == NULL || pObj->ubNumberOfObjects == NOTHING || pObj->usItem == NOTHING )
				continue;
						
			ColorFillVideoSurfaceArea( FRAME_BUFFER, usPosX, usPosY, usPosX + 60, usPosY + 23, Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );

			// do not use the pSoldier-pointer, as the displays look ugly (and some images might not be initialised here)
			INVRenderItem( FRAME_BUFFER, NULL, pObj, usPosX, usPosY, 60, 23, DIRTYLEVEL2, NULL, (UINT8)0, FALSE, 0, 0 );

			usPosY += 25;

			if ( usPosY > GetY() + GetHeight() - 27 )
			{
				usPosX += 62;
				usPosY = GetY( ) + 10;
			}
		}
	}
	else
	{
		MILITIA militia;
		if( GetMilitia( mMilitiaId, &militia ) )
		{
			swprintf( sText, szIdividualMilitiaWebsiteText[16], militia.GetName( ) );
			DrawTextToScreen( sText, usPosX, usPosY + 5, GetWidth( ), FONT12ARIAL, 2, FONT_MCOLOR_BLACK, FALSE, 0 );
			usPosY += 20;
		}
	}

	SetFontShadow( NO_SHADOW );

	BaseTable::Display( );
}

void
MilitiaPersonalInventoryTable::SetRefresh( )
{
	gfMilitiaWebsiteMainRedraw = TRUE;
}

void
MilitiaPersonalInventoryTable::DestroyMouseRegions( )
{
	if ( !IsInit( ) )
		return;

	BaseTable::DestroyMouseRegions( );
}

/////
extern void GetFaceData( UINT32 aNum, UINT32& arImageLib, UINT16& arImage );
//extern void ItemImage( UINT32 usItem, UINT32& arImageLib, UINT16& arImage );
extern STR16 ItemLongNamegetter( UINT32 aNum );

MilitiaInidividualMainWidget::MilitiaInidividualMainWidget( )
: TestPanel( ),
  mMilitiaId( 0 )
{
}

void
MilitiaInidividualMainWidget::Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	SetColorLine( Get16BPPColor( FROMRGB( 128, 128, 128 ) ) );
	SetColorLineShadow( Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );
	SetColorMarked( Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );
	SetColorHighLight( Get16BPPColor( FROMRGB( 235, 222, 171 ) ) );

	BaseTable::Init( sX, sY, sX_End, sY_End );
}

void
MilitiaInidividualMainWidget::Create( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	Destroy( );

	Init( sX, sY, sX_End, sY_End );

	TabBoxTemplate<1>::getInstance( ).Create( sX, sY + 30, sX_End, sY_End );
	
	mPersonalDataPanel.Create( sX, sY + 50, sX_End, sY_End );
	mPersonalDataPanel.SetName( szIdividualMilitiaWebsiteText[9] );
	mPersonalDataPanel.SetColorLine( Get16BPPColor( FROMRGB( 128, 128, 128 ) ) );
	mPersonalDataPanel.SetColorLineShadow( Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );
	mPersonalDataPanel.SetColorMarked( Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );
	mPersonalDataPanel.SetColorHighLight( Get16BPPColor( FROMRGB( 235, 222, 171 ) ) );
	mPersonalData.Create( sX, sY + 50, sX_End, sY_End );
	mPersonalDataPanel.RegisterMemberThingy( &mPersonalData );
	
	mServiceRecordPanel.Create( sX, sY + 50, sX_End, sY_End );
	mServiceRecordPanel.SetName( szIdividualMilitiaWebsiteText[10] );
	mServiceRecordPanel.SetColorLine( Get16BPPColor( FROMRGB( 128, 128, 128 ) ) );
	mServiceRecordPanel.SetColorLineShadow( Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );
	mServiceRecordPanel.SetColorMarked( Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );
	mServiceRecordPanel.SetColorHighLight( Get16BPPColor( FROMRGB( 235, 222, 171 ) ) );
	mServiceRecord.Create( sX, sY + 50, sX_End, sY_End );
	mServiceRecordPanel.RegisterMemberThingy( &mServiceRecord );

	mItemPanel.Create( sX, sY + 50, sX_End, sY_End );
	mItemPanel.SetName( szIdividualMilitiaWebsiteText[11] );
	mItemPanel.SetColorLine( Get16BPPColor( FROMRGB( 128, 128, 128 ) ) );
	mItemPanel.SetColorLineShadow( Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );
	mItemPanel.SetColorMarked( Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );
	mItemPanel.SetColorHighLight( Get16BPPColor( FROMRGB( 235, 222, 171 ) ) );
	mItemsTable.Create( sX, sY + 50, sX_End, sY_End );
	mItemPanel.RegisterMemberThingy( &mItemsTable );
	
	TabBoxTemplate<1>::getInstance( ).RegisterMemberThingy( &mPersonalDataPanel );
	TabBoxTemplate<1>::getInstance( ).RegisterMemberThingy( &mServiceRecordPanel );
	TabBoxTemplate<1>::getInstance( ).RegisterMemberThingy( &mItemPanel );

	RegisterMemberThingy( &TabBoxTemplate<1>::getInstance( ) );
}

template<>	void	TabBoxTemplate<1>::SetRefresh( )	{ gfMilitiaWebsiteMainRedraw = TRUE; }

void
MilitiaInidividualMainWidget::Destroy( )
{
	if ( !IsInit( ) )
		return;
	
	BaseTable::Destroy( );
}

void
MilitiaInidividualMainWidget::Display( )
{
	if ( !IsInit( ) )
		return;

	DestroyMouseRegions( );
	
	TestPanel::Display( );

	SetFontShadow( NO_SHADOW );
	
	MILITIA militia;
	if ( GetMilitia( mMilitiaId, &militia ) )
	{
		CHAR16	sText[800];
		swprintf( sText, L"" );
		UINT16 usPosX = GetX( );
		UINT16 usPosY = GetY( );

		// face
		UINT32 imagelib = 0;
		UINT16 imageid = 0;
		GetFaceData( mMilitiaId, imagelib, imageid );

		BltVideoObjectFromIndex( FRAME_BUFFER, imagelib, imageid, usPosX + 4, usPosY + 4, VO_BLT_SRCTRANSPARENCY, NULL );

		usPosX += 44;
		usPosY += 8;

		// name
		swprintf( sText, L"%s - %s", militia.GetName( ), militia.GetSector( ) );
		DrawTextToScreen( sText, usPosX, usPosY + 5, GetWidth( ), FONT12ARIAL, 2, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosX += 7 * wcslen( sText );

		/*// gun
		ItemImage( mMilitiaId, imagelib, imageid );
		BltVideoObjectFromIndex( FRAME_BUFFER, imagelib, imageid, usPosX + 4, usPosY - 3, VO_BLT_SRCTRANSPARENCY, NULL );

		usPosX += 90;*/

		//swprintf( sText, ItemLongNamegetter( mMilitiaId ) );
		//DrawTextToScreen( sText, usPosX, usPosY + 5, GetWidth( ), FONT12ARIAL, 2, FONT_MCOLOR_BLACK, FALSE, 0 );

		//usPosX += 7 * wcslen( sText );
		usPosY += 30;
	}

	mCloseImage = LoadButtonImage( "INTERFACE\\skilltree.sti", 71, 71, 71, 71, 71 );

	// save changes
	mCloseButton = QuickCreateButton( mCloseImage,
										 GetX( ) + GetWidth( ) - 26, GetY( ) + 4,
										 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										 MSYS_NO_CALLBACK,
										 CloseButtonCallback );
	mCloseButtonDefined = TRUE;
}

void
MilitiaInidividualMainWidget::SetRefresh( )
{
	gfMilitiaWebsiteMainRedraw = TRUE;
}

void
MilitiaInidividualMainWidget::DestroyMouseRegions( )
{
	if ( mCloseButtonDefined )
	{
		RemoveButton( mCloseButton );
		UnloadButtonImage( mCloseImage );

		mCloseButtonDefined = FALSE;
	}

	TestPanel::DestroyMouseRegions( );
}

extern BOOLEAN gfMilitiaDetailsOpen;

void
MilitiaInidividualMainWidget::CloseButtonCallback( GUI_BUTTON * btn, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		militiaindividualmainwidget.SetMouseRegionActive( FALSE );
		militiaindividualmainwidget.DestroyMouseRegions( );

		TestTableTemplate<3>::getInstance( ).SetMouseRegionActive( TRUE );

		gfMilitiaDetailsOpen = FALSE;

		TestTableTemplate<3>::getInstance( ).SetRefresh( );

		militiaindividualmainwidget.SetRefresh( );
	}
	else if ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
	}
}

MilitiaInidividualMainWidget militiaindividualmainwidget;
