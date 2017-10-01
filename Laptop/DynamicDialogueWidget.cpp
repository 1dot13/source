/**
* @file
* @author Flugente (bears-pit.com)
* @brief This file contains definitions of classes and functions used for dynamic dialogue
*/

#include "DynamicDialogueWidget.h"

#include "WCheck.h"
#include "renderworld.h"
#include "Font Control.h"
#include "Utilities.h"
#include "WordWrap.h"

#include "Soldier Profile.h"
#include "Cursors.h"
#include "random.h"
#include "faces.h"

extern FACETYPE *gpCurrentTalkingFace;

// whether we are showing the inventory pool graphic
extern BOOLEAN fShowMapInventoryPool;

extern void Display2Line2ShadowVertical( UINT16 usStartX, UINT16 usStartY, UINT16 EndX, UINT16 usColor1, UINT16 usColor2 );
extern void Display2Line2ShadowHorizontal( UINT16 usStartX, UINT16 usStartY, UINT16 EndY, UINT16 usColor1, UINT16 usColor2 );

#define		MYBOX_NUMBER							10

#define		MYBOX_FONT_DEF							FONT12ARIAL

#define		IMPDIALOGUECHOOSEBOX_BAR_Y_OFFSET		20
#define		IMPDIALOGUECHOOSEBOX_BAR_MAXLENGTH		500

DDBox::DDBox( UINT8 aID )
: WidgetBase( )
{
	// default settings
	musHeight = 0;
	musWidth = 0;						// width of text field

	swprintf( mText, L"" );

	musCreationTime = musEndTime = GetJA2Clock( );

	musID = aID;

	mfInit = FALSE;
	mfDisplayed = FALSE;
	mfFaceImageExists = FALSE;
}

void
DDBox::Init( UINT16 sX, UINT16 sY )
{
	SetX( sX );
	SetY( sY );

	SetX_Text( GetX( ) + MYBOX_FACE_OFFSET );
	SetY_Text( WidgetBase::GetY( ) );

	musWidth = min( MYBOX_TEXT_MAXWIDTH, StringPixLength( mText, MYBOX_FONT_DEF ) );

	musHeight = IanWrappedStringHeight( GetX_Text( ), GetY_Text( ) + 7, musWidth, 2, MYBOX_FONT_DEF, FONT_BLACK, mText, FONT_MCOLOR_BLACK, FALSE, 0 );

	musFontHeight = GetFontHeight( MYBOX_FONT_DEF );	// does not work on init of static objects, as the fonts do not yet exist!

	musEndTime = musCreationTime = GetJA2Clock( );

	musFaceImage = 0;

	mfInit = TRUE;
}

void
DDBox::Create( UINT16 sX, UINT16 sY )
{
	Destroy( );
	
	Init( sX, sY );
	
	mfDisplayed = TRUE;
}


void
DDBox::Destroy( )
{
	if ( !IsInit( ) )
		return;

	DeleteVideoObjectFromIndex( musFaceImage );

	musFaceImage = 0;
		
	mfInit = FALSE;
	mfDisplayed = FALSE;
	mfFaceImageExists = FALSE;

	SetRenderFlags( RENDER_FLAG_FULL );

	Refresh( );
	
	RemoveDDBox( GetID( ) );
}

void
DDBox::Display( )
{
	if ( !IsDisplayed( ) )
		return;

	// it is possible that we cannot instantly create the video object. Thus we try until we are sucessful
	if ( !mfFaceImageExists )
	{
		VOBJECT_DESC	VObjectDesc;
		char			sTemp[100];

		// Load face file
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

		// IMP faces are stored elsewhere
		if ( gProfilesIMP[mEvent.usSpeaker].ProfilId == mEvent.usSpeaker )
		{
			sprintf( sTemp, "IMPFACES\\%02d.sti", gMercProfiles[mEvent.usSpeaker].ubFaceIndex );
		}
		else
		{
			sprintf( sTemp, "FACES\\%02d.sti", gMercProfiles[mEvent.usSpeaker].ubFaceIndex );
		}

		FilenameForBPP( sTemp, VObjectDesc.ImageFile );

		if ( AddVideoObject( &VObjectDesc, &musFaceImage ) )
			mfFaceImageExists = TRUE;
	}

	if ( mfFaceImageExists )
	{
		HVOBJECT		hPixHandle;
		//Get the merc's face
		if ( GetVideoObject( &hPixHandle, musFaceImage ) )
		{
			//if the merc is dead, shade the face red
			if ( IsMercDead( mEvent.usSpeaker ) )
			{
				// if the merc is dead, shade the face red, (to signify that he is dead)
				hPixHandle->pShades[0] = Create16BPPPaletteShaded( hPixHandle->pPaletteEntry, 255, 55, 55, TRUE );

				//set the red pallete to the face
				SetObjectHandleShade( musFaceImage, 0 );
			}

			//Get and display the mercs face
			BltVideoObject( FRAME_BUFFER, hPixHandle, 0, GetX( ), GetY( ), VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}

	//Display the background
	ColorFillVideoSurfaceArea( FRAME_BUFFER, GetX_Text( ), GetY_Text( ), GetX_Text( ) + musWidth, GetY_Text( ) + musFontHeight, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

	DrawTopEntry( );

	SetRenderFlags( RENDER_FLAG_FULL );
}

void
DDBox::Refresh( )
{
	// nothing to do if not initialized
	if ( !IsInit( ) )
	{
		return;
	}
	// if still being displayed
	else if ( IsDisplayed( ) )
	{
		// if not to be shown yet, hide
		if ( musCreationTime > GetJA2Clock( ) )
		{
			;
		}
		// outdated -> destroy
		else if ( musEndTime < GetJA2Clock( ) )
		{
			Destroy( );
		}
		else
		{
			Display( );
		}
	}
	else
	{
		Destroy( );
	}
}

UINT16
DDBox::GetY( )
{
	UINT16 sY = WidgetBase::GetY( );

	if ( mEvent.usSide == DOST_POSITION_LEFT && gpCurrentTalkingFace )
		sY += 130;

	return sY;
}

UINT16
DDBox::GetY_Text( )
{
	UINT16 sY = WidgetBase::GetY_Text( );

	if ( mEvent.usSide == DOST_POSITION_LEFT && gpCurrentTalkingFace )
		sY += 130;

	return sY;
}

void
DDBox::DrawTopEntry( )
{
	//display the name in the list
	ColorFillVideoSurfaceArea( FRAME_BUFFER, GetX_Text( ) - 4, GetY_Text( ) - 4, GetX_Text( ) + musWidth + 4, GetY_Text( ) + musHeight + 4, GetColorMarked( ) );

	SetFontShadow( NO_SHADOW );

	DisplayWrappedString( GetX_Text( ), GetY_Text( ) + 4, musWidth, 2, MYBOX_FONT_DEF, FONT_BLACK, mText, FONT_MCOLOR_BLACK, FALSE, 0 );

	SetFontShadow( DEFAULT_SHADOW );
}

void
DDBox::ResetCreationTime( )
{
	musCreationTime = GetJA2Clock( );
}


// the list of boxes
DDBox *gDDBoxList[MYBOX_NUMBER];

BOOLEAN
InitMyBoxes( )
{
	// init the pop up box list
	for ( UINT8 i = 0; i < MYBOX_NUMBER; ++i )
	{
		// set ptr to null
		gDDBoxList[i] = NULL;
	}

	return TRUE;
}

DDBox*
GetDDBox( UINT8 aID )
{
	// now find this box in the list
	for ( UINT8 i = 0; i < MYBOX_NUMBER; ++i )
	{
		if ( gDDBoxList[i] != NULL && gDDBoxList[i]->GetID( ) == aID )
		{
			return gDDBoxList[i];
		}
	}

	// create box
	DDBox* gDDBox = new DDBox( aID );

	if ( gDDBox )
	{
		// attempt to add box to list
		for ( UINT8 i = 0; i < MYBOX_NUMBER; ++i )
		{
			if ( gDDBoxList[i] == NULL )
			{
				// found a spot, insert
				gDDBoxList[i] = gDDBox;

				return gDDBoxList[i];
			}
		}
	}

	return NULL;
}

BOOLEAN RemoveDDBox( UINT8 aID )
{
	// now find this box in the list
	for ( UINT8 i = 0; i < MYBOX_NUMBER; ++i )
	{
		if ( gDDBoxList[i] != NULL && gDDBoxList[i]->GetID( ) == aID )
		{
			gDDBoxList[i]->Destroy( );

			MemFree( gDDBoxList[i] );

			gDDBoxList[i] = NULL;
			
			return TRUE;
		}
	}

	return FALSE;
}

void RefreshBoxes( )
{
	// if feature is not on, don't bother with this
	if ( !gGameExternalOptions.fDynamicDialogue )
		return;

	static UINT32 lasttimenhere = GetJA2Clock( );

	// dialogue should only happen in GAME_SCREEN or MAP_SCREEN, at other times it would be too distracting
	// also halt dialogue if sector inventory is open
	if ( guiCurrentScreen == GAME_SCREEN || guiCurrentScreen == MAP_SCREEN && !fShowMapInventoryPool )
	{
		// if we are scrolling, don't redraw the boxes as often
		if (gfScrollPending || gfScrollInertia)
		{
			UINT32 timepassednotdisplaying = max(0, GetJA2Clock() - lasttimenhere);

			if (timepassednotdisplaying < 100)
			{
				lasttimenhere = GetJA2Clock();
				return;
			}
		}

		// refresh all boxes, and while you're at it, check wether there are any
		BOOLEAN fFound = FALSE;
		for ( UINT8 i = 0; i < MYBOX_NUMBER; ++i )
		{
			if ( gDDBoxList[i] != NULL )
			{
				gDDBoxList[i]->Refresh( );

				fFound = TRUE;
			}
		}

		if ( IMPDialogueChooseBox_Static::getInstance( ).IsDisplayed( ) )
		{
			IMPDialogueChooseBox_Static::getInstance( ).Refresh( );

			fFound = TRUE;
		}

		// if no box is active, we are free to start another dialogue
		if ( !fFound )
			HandleDynamicOpinionSpeechEvents( );
		else
		{
			// boxes are active, thus we have to refresh them
			if (guiCurrentScreen == MAP_SCREEN)
				fMapPanelDirty = TRUE;
		}
	}
	else
	{
		// Our dynamic dialogue boxes are destroyed as time progresses. As we do not display them at the moment, we have to update their creation and end time.
		// As a result, no time will be lost for our dialogues if we are in a different screen
		UINT32 timepassednotdisplaying = max( 0, GetJA2Clock( ) - lasttimenhere );

		if ( timepassednotdisplaying )
			DelayBoxDestructionBy( timepassednotdisplaying );
	}

	lasttimenhere = GetJA2Clock( );
}

void DelayBoxDestruction( UINT32 aVal )
{
	// now find this box in the list
	for ( UINT8 i = 0; i < MYBOX_NUMBER; ++i )
	{
		if ( gDDBoxList[i] != NULL && gDDBoxList[i]->IsDisplayed( ) )
		{
			gDDBoxList[i]->SetEndTime( aVal );
		}
	}
}

void DelayBoxDestructionBy( INT32 aVal )
{
	// now find this box in the list
	for ( UINT8 i = 0; i < MYBOX_NUMBER; ++i )
	{
		if ( gDDBoxList[i] != NULL && gDDBoxList[i]->IsDisplayed( ) )
		{
			gDDBoxList[i]->SetStartTime( gDDBoxList[i]->GetStartTime( ) + aVal );
			gDDBoxList[i]->SetEndTime( gDDBoxList[i]->GetEndTime( ) + aVal );
		}
	}

	if ( IMPDialogueChooseBox_Static::getInstance( ).IsDisplayed( ) )
	{
		IMPDialogueChooseBox_Static::getInstance( ).SetStartTime( IMPDialogueChooseBox_Static::getInstance( ).GetStartTime( ) + aVal );
		IMPDialogueChooseBox_Static::getInstance( ).SetEndTime( IMPDialogueChooseBox_Static::getInstance( ).GetEndTime( ) + aVal );
	}
}


IMPDialogueChooseBox::IMPDialogueChooseBox( )
{
	// default settings
	musWidth = 0;						// width of text field

	mEntryVector.clear( );

	swprintf( mText, L"" );

	mSelectedEntry = 0;

	for ( UINT8 i = 0; i < DOST_CHOICE_MAX; ++i )
		mChoiceRegionDefined[i] = FALSE;

	musCreationTime = musEndTime = GetJA2Clock( );

	mfInit = FALSE;
	mfDisplayed = FALSE;
}


void
IMPDialogueChooseBox::Init( UINT16 sX, UINT16 sY )
{
	SetX( sX );
	SetY( sY );

	SetX_Text( GetX( ) + MYBOX_FACE_OFFSET );
	SetY_Text( WidgetBase::GetY( ) );

	mSelectedEntry = 0;

	musWidth = 0;

	UINT16 size = mEntryVector.size( );
	for ( UINT16 i = 0; i < size; ++i )
	{
		musWidth = max( musWidth, StringPixLength( mEntryVector[i].second, MYBOX_FONT_DEF ) );
	}

	musFontHeight = GetFontHeight( MYBOX_FONT_DEF );	// does not work on init of static objects, as the fonts do not yet exist!

	musCreationTime = GetJA2Clock( );

	musEndTime = musCreationTime;

	mfInit = TRUE;
}

void
IMPDialogueChooseBox::Create( UINT16 sX, UINT16 sY )
{
	Destroy( );

	Init( sX, sY );
	
	sY += IMPDIALOGUECHOOSEBOX_BAR_Y_OFFSET;

	UINT16 size = mEntryVector.size( );
	for ( UINT16 i = 0; i < size; ++i )
	{
		if ( !mChoiceRegionDefined[i] )
		{
			MSYS_DefineRegion( &mChoiceRegion[i], GetX( ) - 4, sY, GetX( ) + musWidth + 4, sY + musFontHeight, MSYS_PRIORITY_HIGH,
							   CURSOR_WWW, MSYS_NO_CALLBACK, CallBackWrapper( (void*) this, DROPDOWN_REGION, &IMPDialogueChooseBox::Dummyfunc ) );
			MSYS_AddRegion( &mChoiceRegion[i] );
			MSYS_SetRegionUserData( &mChoiceRegion[i], 0, i );

			mChoiceRegionDefined[i] = TRUE;
		}

		sY += musFontHeight + 2;
	}

	mfDisplayed = TRUE;
}

void
IMPDialogueChooseBox::Destroy( )
{
	if ( !IsInit( ) )
		return;

	for ( UINT8 i = 0; i < DOST_CHOICE_MAX; ++i )
	{
		if ( mChoiceRegionDefined[i] )
		{
			MSYS_RemoveRegion( &mChoiceRegion[i] );
			mChoiceRegionDefined[i] = FALSE;
		}
	}

	mfInit = FALSE;
	mfDisplayed = FALSE;

	SetRenderFlags( RENDER_FLAG_FULL );

	Refresh( );
}

void
IMPDialogueChooseBox::Display( )
{
	if ( !IsDisplayed( ) )
		return;

	//  we draw a line that shows us how much time we have for the decision
	UINT32 totaltime = max( 0, musEndTime - musCreationTime );

	if ( totaltime )
	{
		UINT32 timeleft = musEndTime - GetJA2Clock( );
		FLOAT factor = (FLOAT)timeleft / (FLOAT)totaltime;

		UINT32 maxwidth = min( IMPDIALOGUECHOOSEBOX_BAR_MAXLENGTH, SCREEN_WIDTH / 2 );

		UINT16 colour = Get16BPPColor( FROMRGB( 255 * (1 - factor), 255 * factor, 0 ) );
		UINT16 colourshadow = Get16BPPColor( FROMRGB( 127 * (1 - factor), 127 * factor, 0 ) );

		Display2Line2ShadowHorizontal( GetX_Text( ) + maxwidth / 2 - factor * maxwidth / 2, GetY( ), GetX_Text( ) + maxwidth / 2 + factor * maxwidth / 2, colour, colourshadow );
	}

	DrawTopEntry( );

	SetRenderFlags( RENDER_FLAG_FULL );
}

void
IMPDialogueChooseBox::Refresh( )
{
	// nothing to do if not initialized
	if ( !IsInit( ) )
	{
		return;
	}
	// if still being displayed
	else if ( IsDisplayed( ) )
	{
		// if not to be shown yet, hide
		if ( musCreationTime > GetJA2Clock( ) )
		{
			;
		}
		// outdated -> destroy
		else if ( musEndTime < GetJA2Clock( ) )
		{
			TimeOut( );
			Destroy( );
		}
		else
		{
			Display( );
		}
	}
	else
	{
		Destroy( );
	}
}

void
IMPDialogueChooseBox::DrawTopEntry( )
{
	// make sure we don't get bogus values
	mSelectedEntry = min( mSelectedEntry, mEntryVector.size( ) - 1 );

	UINT16 sY = GetY( ) + IMPDIALOGUECHOOSEBOX_BAR_Y_OFFSET;

	UINT16 cnt = 0;
	std::vector<std::pair<INT16, STR16> >::iterator itend = mEntryVector.end( );
	for ( std::vector<std::pair<INT16, STR16> >::iterator it = mEntryVector.begin( ); it != itend; ++it )
	{
		//display the name in the list
		ColorFillVideoSurfaceArea( FRAME_BUFFER, GetX( ) - 4, sY, GetX( ) + musWidth + 4, sY + musFontHeight, cnt == mSelectedEntry ? GetColorHighLight( ) : GetColorMarked( ) );

		SetFontShadow( NO_SHADOW );

		DrawTextToScreen( (*it).second, GetX( ), sY, 0, MYBOX_FONT_DEF, cnt == mSelectedEntry ? FONT_DKGREEN : FONT_BLACK, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

		SetFontShadow( DEFAULT_SHADOW );

		sY += musFontHeight + 2;

		++cnt;
	}
}

void
IMPDialogueChooseBox::SelectDropDownRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		mSelectedEntry = (UINT16)MSYS_GetRegionUserData( pRegion, 0 );
	}
}


void
IMPDialogueChooseBox::ResetCreationTime( )
{
	musCreationTime = GetJA2Clock( );
}

void
IMPDialogueChooseBox::TimeOut( )
{
	// if we are destroyed, we set up dialogues according to the players choices
	RemoveDDBox( mEvent.usSpeaker );

	DynamicOpinionSpeechEvent event = mEvent;

	switch ( mSelectedEntry )
	{
	case DOST_CHOICE_NOTHING:
		event.ubEventType = DOST_INTERJECTOR_NOTHING;
		break;

		// if we side with the victim or the cause, we either support them or take a stand against the other
		// for now, the choice is random
	case DOST_CHOICE_SIDEWITH_VICTIM:
		if ( Random( 2 ) )
			event.ubEventType = DOST_INTERJECTOR_TO_VICTIM_AGREE;
		else
			event.ubEventType = DOST_INTERJECTOR_TO_CAUSE_DENY;

		event.usSide = DOST_POSITION_LEFT;
		break;

	case DOST_CHOICE_SIDEWITH_CAUSE:
		if ( Random( 2 ) )
			event.ubEventType = DOST_INTERJECTOR_TO_CAUSE_AGREE;
		else
			event.ubEventType = DOST_INTERJECTOR_TO_VICTIM_DENY;
		event.usSide = DOST_POSITION_RIGHT;
		break;

	case DOST_CHOICE_REASON:
		event.ubEventType = DOST_INTERJECTOR_SOLVE_REASON;
		break;

	case DOST_CHOICE_AGGRESSIVE:
		event.ubEventType = DOST_INTERJECTOR_SOLVE_AGGRESSIVE;
		break;
	}

	event.usQueueNumber++;
	event.usStarttime = GetJA2Clock( );
	event.usNumonside = (UINT8)GetSidePosition( event.usSide );

	AddDynamicOpinionEvent_Continue( event );
}


void* IMPDialogueChooseBox_Static::mpSelf = NULL;

// are there still dynamic dialogue boxes active? We use this to check wether we can start new dialogue
BOOLEAN DynamicDialogueBoxesActive( )
{
	// now find this box in the list
	for ( UINT8 i = 0; i < MYBOX_NUMBER; ++i )
	{
		if ( gDDBoxList[i] != NULL )
			return TRUE;
	}

	if ( IMPDialogueChooseBox_Static::getInstance( ).IsInit( ) )
		return TRUE;

	return FALSE;
}

// used fo example when we load a game - dialogue has to start anew, so all boxes must be wiped
void DestroyAllDynamicDialogueBoxes( )
{
	// now find this box in the list
	for ( UINT8 i = 0; i < MYBOX_NUMBER; ++i )
	{
		if ( gDDBoxList[i] != NULL )
		{
			gDDBoxList[i]->Destroy( );

			MemFree( gDDBoxList[i] );

			gDDBoxList[i] = NULL;
		}
	}

	IMPDialogueChooseBox_Static::getInstance( ).Destroy( );
}
