#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "MercTextBox.h"
	#include "WCheck.h"
	#include "renderworld.h"
	#include "Font Control.h"
	#include "Utilities.h"
	#include "WordWrap.h"
	#include "vobject_blitters.h"
	#include "Message.h"
#endif


#define		TEXT_POPUP_WINDOW_TEXT_OFFSET_X		8
#define		TEXT_POPUP_WINDOW_TEXT_OFFSET_Y		8
#define		TEXT_POPUP_STRING_WIDTH						296
#define		TEXT_POPUP_GAP_BN_LINES						10
#define		TEXT_POPUP_FONT										FONT12ARIAL
#define		TEXT_POPUP_COLOR									FONT_MCOLOR_WHITE

#define		MERC_TEXT_FONT										FONT12ARIAL
#define		MERC_TEXT_COLOR										FONT_MCOLOR_WHITE

#define		MERC_TEXT_MIN_WIDTH								10
#define		MERC_TEXT_POPUP_WINDOW_TEXT_OFFSET_X		10
#define		MERC_TEXT_POPUP_WINDOW_TEXT_OFFSET_Y		10

#define		MERC_BACKGROUND_WIDTH										350
#define		MERC_BACKGROUND_HEIGHT									207

// the max number of pop up boxes available to user
#define MAX_NUMBER_OF_POPUP_BOXES 10

// attempt to add box to pop up box list
INT32 AddPopUpBoxToList( MercPopUpBox *pPopUpTextBox );


// grab box with this id value
MercPopUpBox * GetPopUpBoxIndex( INT32 iId );


// both of the below are index by the enum for thier types - background and border in
// MercTextBox.h

// filenames for border popup .sti's
STR8 zMercBorderPopupFilenames[ ] = {
 "INTERFACE\\TactPopUp.sti",
 "INTERFACE\\TactRedPopUp.sti",
 "INTERFACE\\TactBluePopUp.sti",
 "INTERFACE\\TactPopUpMain.sti",
 "INTERFACE\\LaptopPopup.sti",
};

// filenames for background popup .pcx's
STR8 zMercBackgroundPopupFilenames[ ] = {
	"INTERFACE\\TactPopupBackground.pcx",
	"INTERFACE\\TactPopupWhiteBackground.pcx",
	"INTERFACE\\TactPopupGreyBackground.pcx",
	"INTERFACE\\TactPopupBackgroundMain.pcx",
	"INTERFACE\\LaptopPopupBackground.pcx",
	"INTERFACE\\imp_popup_background.pcx",
};


// the pop up box structure
MercPopUpBox	gBasicPopUpTextBox;

// the current pop up box
MercPopUpBox	*gPopUpTextBox = NULL;

// the old one
MercPopUpBox	*gOldPopUpTextBox = NULL;


// the list of boxes
MercPopUpBox *gpPopUpBoxList[ MAX_NUMBER_OF_POPUP_BOXES ];

// the flags
UINT32	guiFlags = 0;
UINT32	guiBoxIcons;
UINT32	guiSkullIcons;

BOOLEAN SetCurrentPopUpBox( UINT32 uiId )
{
	// given id of the box, find it in the list and set to current

	//make sure the box id is valid
	if( uiId == (UINT32) -1 )
	{
		//ScreenMsg( FONT_MCOLOR_WHITE, MSG_BETAVERSION, L"Error: Trying to set Current Popup Box using -1 as an ID" );
		return( FALSE );
	}

	// see if box inited
	if( gpPopUpBoxList[ uiId ] != NULL )
	{
		gPopUpTextBox = gpPopUpBoxList[ uiId ];
		return( TRUE );
	}
	return ( FALSE );
}

BOOLEAN OverrideMercPopupBox( MercPopUpBox *pMercBox )
{
	// store old box and set current this passed one
	gOldPopUpTextBox = gPopUpTextBox;

	gPopUpTextBox = pMercBox;

	return( TRUE );
}

BOOLEAN ResetOverrideMercPopupBox( )
{
	gPopUpTextBox = gOldPopUpTextBox;

	return( TRUE );
}


BOOLEAN InitMercPopupBox( )
{
	INT32 iCounter = 0;
	VOBJECT_DESC	VObjectDesc;

	// init the pop up box list
	for( iCounter = 0; iCounter < MAX_NUMBER_OF_POPUP_BOXES; iCounter++ )
	{
		// set ptr to null
		gpPopUpBoxList[ iCounter ] = NULL;
	}

	// LOAD STOP ICON...
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\msgboxicons.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiBoxIcons ) )
		AssertMsg(0, "Missing INTERFACE\\msgboxicons.sti" );

	// LOAD SKULL ICON...
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\msgboxiconskull.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiSkullIcons ) )
		AssertMsg(0, "Missing INTERFACE\\msgboxiconskull.sti" );
	
	return( TRUE );
}


BOOLEAN ShutDownPopUpBoxes( )
{
	INT32 iCounter = 0;
	for( iCounter = 0; iCounter < MAX_NUMBER_OF_POPUP_BOXES ; ++iCounter )
	{
		// now attempt to remove this box
		RemoveMercPopupBoxFromIndex( iCounter );
	}

	return( TRUE );
}


//Pass in the background index, and pointers to the font and shadow color
void	GetMercPopupBoxFontColor( UINT8 ubBackgroundIndex, UINT8 *pubFontColor, UINT8 *pubFontShadowColor);

// Tactical Popup
BOOLEAN LoadTextMercPopupImages( UINT8 ubBackgroundIndex, UINT8 ubBorderIndex)
{
	VSURFACE_DESC		vs_desc;
	VOBJECT_DESC	VObjectDesc;

	// this function will load the graphics associated with the background and border index values

	// the background
	vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
	strcpy(vs_desc.ImageFile,	zMercBackgroundPopupFilenames [ ubBackgroundIndex ]);
	CHECKF(AddVideoSurface(&vs_desc, &gPopUpTextBox->uiMercTextPopUpBackground));

	// border
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( zMercBorderPopupFilenames[ ubBorderIndex ], VObjectDesc.ImageFile );
	CHECKF( AddVideoObject( &VObjectDesc, &gPopUpTextBox->uiMercTextPopUpBorder ) );

	gPopUpTextBox->fMercTextPopupInitialized = TRUE;

	// so far so good, return successful
	gPopUpTextBox->ubBackgroundIndex = ubBackgroundIndex;
	gPopUpTextBox->ubBorderIndex			= ubBorderIndex;

	return( TRUE );
}

void RemoveTextMercPopupImages( )
{
	//this procedure will remove the background and border video surface/object from the indecies
	if( gPopUpTextBox )
	{
		if( gPopUpTextBox->fMercTextPopupInitialized )
		{
			// the background
			DeleteVideoSurfaceFromIndex( gPopUpTextBox->uiMercTextPopUpBackground );

			// the border
			DeleteVideoObjectFromIndex( gPopUpTextBox->uiMercTextPopUpBorder );

			gPopUpTextBox->fMercTextPopupInitialized = FALSE;
		}
	}
}

BOOLEAN RenderMercPopUpBoxFromIndex( INT32 iBoxId, INT16 sDestX, INT16 sDestY, UINT32 uiBuffer )
{
	// set the current box
	if( SetCurrentPopUpBox( iBoxId ) == FALSE )
	{
		return ( FALSE );
	}

	// now attempt to render the box
	return( RenderMercPopupBox( sDestX,	sDestY,	uiBuffer ) );
}

BOOLEAN RenderMercPopupBox(INT16 sDestX, INT16 sDestY, UINT32 uiBuffer )
{
//	UINT32	uiDestPitchBYTES;
//	UINT32	uiSrcPitchBYTES;
//	UINT16	*pDestBuf;
//	UINT16	*pSrcBuf;
	
	// will render/transfer the image from the buffer in the data structure to the buffer specified by user
	BOOLEAN fReturnValue = TRUE;

	// grab the destination buffer
//	pDestBuf = ( UINT16* )LockVideoSurface( uiBuffer, &uiDestPitchBYTES );

	// now lock it
//	pSrcBuf = ( UINT16* )LockVideoSurface( gPopUpTextBox->uiSourceBufferIndex, &uiSrcPitchBYTES);
	
	//check to see if we are wanting to blit a transparent background
	if ( gPopUpTextBox->uiFlags & MERC_POPUP_PREPARE_FLAGS_TRANS_BACK )
		BltVideoSurface( uiBuffer, gPopUpTextBox->uiSourceBufferIndex, 0, sDestX, sDestY, VS_BLT_FAST | VS_BLT_USECOLORKEY, NULL );
	else
		BltVideoSurface( uiBuffer, gPopUpTextBox->uiSourceBufferIndex, 0, sDestX, sDestY, VS_BLT_FAST, NULL );
	
	// blt, and grab return value
//	fReturnValue = Blt16BPPTo16BPP(pDestBuf, uiDestPitchBYTES, pSrcBuf, uiSrcPitchBYTES, sDestX, sDestY, 0, 0, gPopUpTextBox->sWidth, gPopUpTextBox->sHeight);

	//Invalidate!
	if ( uiBuffer == FRAME_BUFFER )
	{
		InvalidateRegion( sDestX, sDestY, (INT16)( sDestX + gPopUpTextBox->sWidth ), (INT16)( sDestY + gPopUpTextBox->sHeight ) );
	}

	// unlock the video surfaces

	// source
//	UnLockVideoSurface( gPopUpTextBox->uiSourceBufferIndex );

	// destination
//	UnLockVideoSurface( uiBuffer );

	// return success or failure
	return fReturnValue;
}



INT32 AddPopUpBoxToList( MercPopUpBox *pPopUpTextBox )
{
	INT32 iCounter = 0;

	// make sure is a valid box
	if( pPopUpTextBox == NULL )
	{
		return ( -1 );
	}

	// attempt to add box to list
	for( iCounter = 0; iCounter < MAX_NUMBER_OF_POPUP_BOXES; ++iCounter )
	{
		if( gpPopUpBoxList[ iCounter ] == NULL )
		{
			// found a spot, inset
			gpPopUpBoxList[ iCounter ] = pPopUpTextBox;

			// set as current
			SetCurrentPopUpBox( iCounter );

			// return index value
			return( iCounter );
		}
	}

	// return failure
	return( -1 );
}

// get box with this id
MercPopUpBox * GetPopUpBoxIndex( INT32 iId )
{
	return( gpPopUpBoxList[ iId ] );
}

INT32 PrepareMercPopupBox(	INT32 iBoxId, UINT8 ubBackgroundIndex, UINT8 ubBorderIndex, STR16 pString,
							UINT16 usWidth, UINT16 usMarginX, UINT16 usMarginTopY, UINT16 usMarginBottomY,
							UINT16 *pActualWidth, UINT16 *pActualHeight, BOOLEAN bFixedWidth)
{
	UINT16 usNumberVerticalPixels, usNumberOfLines;
	UINT16 usTextWidth, usHeight;
	UINT16 i;
	HVOBJECT	hImageHandle;
	UINT16 usPosY, usPosX;
	VSURFACE_DESC		vs_desc;
	UINT16 usStringPixLength;
	SGPRect DestRect;
	HVSURFACE hSrcVSurface;
	UINT32 uiDestPitchBYTES;
	UINT32 uiSrcPitchBYTES;
	UINT16	*pDestBuf;
	UINT8	*pSrcBuf;
	UINT8		ubFontColor, ubFontShadowColor;
	UINT16	usColorVal;
	UINT16	usLoopEnd;
	INT16		sDispTextXPos;
	MercPopUpBox *pPopUpTextBox = NULL;

	if( usWidth >= SCREEN_WIDTH )
		return( -1 );

	if( usWidth <= MERC_TEXT_MIN_WIDTH )
		usWidth = MERC_TEXT_MIN_WIDTH;

	// check id value, if -1, box has not been inited yet
	if( iBoxId == -1 )
	{
		// no box yet

		// create box
		pPopUpTextBox = (MercPopUpBox *) MemAlloc( sizeof( MercPopUpBox ) );

		// copy over ptr
		gPopUpTextBox = pPopUpTextBox;

			// Load appropriate images
		if( LoadTextMercPopupImages( ubBackgroundIndex, ubBorderIndex ) == FALSE )
		{
			MemFree( pPopUpTextBox );
			return( -1 );
		}
	}
	else
	{
		// has been created already,
		// Check if these images are different

		// grab box
		pPopUpTextBox = GetPopUpBoxIndex( iBoxId );

		// box has valid id and no instance?..error
		Assert( pPopUpTextBox );

			// copy over ptr
		gPopUpTextBox = pPopUpTextBox;

		if ( ubBackgroundIndex != pPopUpTextBox->ubBackgroundIndex || ubBorderIndex != pPopUpTextBox->ubBorderIndex || !pPopUpTextBox->fMercTextPopupInitialized)
		{
			//Remove old, set new
			RemoveTextMercPopupImages( );
			if( LoadTextMercPopupImages( ubBackgroundIndex, ubBorderIndex ) == FALSE )
			{
				return( -1 );
			}
		}
	}

	gPopUpTextBox->uiFlags = guiFlags;
	// reset flags
	guiFlags = 0;

	usStringPixLength = WFStringPixLength( pString, TEXT_POPUP_FONT);	

	// sevenfm: change messagbebox width only if bFixedWidth = FALSE
	if( !bFixedWidth && ( usStringPixLength < ( usWidth - ( MERC_TEXT_POPUP_WINDOW_TEXT_OFFSET_X ) * 2 ) ) )
	{
		usWidth = usStringPixLength + MERC_TEXT_POPUP_WINDOW_TEXT_OFFSET_X * 2;
		usTextWidth = usWidth - ( MERC_TEXT_POPUP_WINDOW_TEXT_OFFSET_X	) * 2 + 1;
	}
	else
	{
		usTextWidth = usWidth - ( MERC_TEXT_POPUP_WINDOW_TEXT_OFFSET_X	) * 2 + 1 - usMarginX;
	}

	usNumberVerticalPixels = IanWrappedStringHeight(0,0, usTextWidth, 2, TEXT_POPUP_FONT, MERC_TEXT_COLOR,	pString, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

	usNumberOfLines = usNumberVerticalPixels / TEXT_POPUP_GAP_BN_LINES;

	usHeight = usNumberVerticalPixels + MERC_TEXT_POPUP_WINDOW_TEXT_OFFSET_X * 2;

	// Add height for margins
	usHeight += usMarginTopY + usMarginBottomY;

	// Add width for margins
	usWidth += (usMarginX*2);

	// Add width for iconic...
	if ( ( pPopUpTextBox->uiFlags & ( MERC_POPUP_PREPARE_FLAGS_STOPICON | MERC_POPUP_PREPARE_FLAGS_SKULLICON ) ) )
	{
		// Make minimun height for box...
		if ( usHeight < 45 )
		{
			usHeight = 45;
		}
		usWidth += 35;
	}

	if( usWidth >= MERC_BACKGROUND_WIDTH )
		usWidth = MERC_BACKGROUND_WIDTH-1;

	if ( usHeight >= MERC_BACKGROUND_HEIGHT )
		usHeight = MERC_BACKGROUND_HEIGHT - 1;

	//make sure the area isnt bigger then the background texture
	if( ( usWidth >= MERC_BACKGROUND_WIDTH ) || usHeight >= MERC_BACKGROUND_HEIGHT)
	{
		if( iBoxId == -1 )
		{
			MemFree( pPopUpTextBox );
		}

		return( -1 );
	}

	// Create a background video surface to blt the face onto
	memset( &vs_desc, 0, sizeof( VSURFACE_DESC ) );
	vs_desc.fCreateFlags = VSURFACE_CREATE_DEFAULT | VSURFACE_SYSTEM_MEM_USAGE;
	vs_desc.usWidth = usWidth;
	vs_desc.usHeight = usHeight;
	vs_desc.ubBitDepth = 16;
	CHECKF( AddVideoSurface( &vs_desc, &pPopUpTextBox->uiSourceBufferIndex) );
	pPopUpTextBox->fMercTextPopupSurfaceInitialized = TRUE;

	pPopUpTextBox->sWidth = usWidth;
	pPopUpTextBox->sHeight = usHeight;

	*pActualWidth = usWidth;
	*pActualHeight = usHeight;

	DestRect.iLeft = 0;
	DestRect.iTop = 0;
	DestRect.iRight = DestRect.iLeft + usWidth;
	DestRect.iBottom = DestRect.iTop + usHeight;

	if ( pPopUpTextBox->uiFlags & MERC_POPUP_PREPARE_FLAGS_TRANS_BACK )
	{
		// Zero with yellow,
		// Set source transparcenty
		SetVideoSurfaceTransparency( pPopUpTextBox->uiSourceBufferIndex, FROMRGB(	255, 255, 0 ) );

		pDestBuf = (UINT16*)LockVideoSurface( pPopUpTextBox->uiSourceBufferIndex, &uiDestPitchBYTES);

		usColorVal = Get16BPPColor( FROMRGB( 255, 255, 0 ) );
		usLoopEnd	= ( usWidth * usHeight );

		for ( i = 0; i <usLoopEnd; ++i )
		{
			pDestBuf[ i ] = usColorVal;
		}

		UnLockVideoSurface(pPopUpTextBox->uiSourceBufferIndex);
	}
	else
	{
		if( !GetVideoSurface( &hSrcVSurface, pPopUpTextBox->uiMercTextPopUpBackground) )
		{
			AssertMsg( 0, String( "Failed to GetVideoSurface for PrepareMercPopupBox.	VSurfaceID:	%d",
				pPopUpTextBox->uiMercTextPopUpBackground ) );
		}

		pDestBuf = (UINT16*)LockVideoSurface( pPopUpTextBox->uiSourceBufferIndex, &uiDestPitchBYTES);
		pSrcBuf = LockVideoSurface( pPopUpTextBox->uiMercTextPopUpBackground, &uiSrcPitchBYTES);

		Blt8BPPDataSubTo16BPPBuffer( pDestBuf,	uiDestPitchBYTES, hSrcVSurface, pSrcBuf,uiSrcPitchBYTES,0,0, &DestRect);

		UnLockVideoSurface( pPopUpTextBox->uiMercTextPopUpBackground);
		UnLockVideoSurface(pPopUpTextBox->uiSourceBufferIndex);
	}

	GetVideoObject(&hImageHandle, pPopUpTextBox->uiMercTextPopUpBorder );

	usPosX = usPosY = 0;
	//blit top row of images
	for(i=TEXT_POPUP_GAP_BN_LINES; i< usWidth-TEXT_POPUP_GAP_BN_LINES; i+=TEXT_POPUP_GAP_BN_LINES)
	{
		//TOP ROW
		BltVideoObject(pPopUpTextBox->uiSourceBufferIndex, hImageHandle, 1,i, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);
		//BOTTOM ROW
		BltVideoObject(pPopUpTextBox->uiSourceBufferIndex, hImageHandle, 6,i, usHeight - TEXT_POPUP_GAP_BN_LINES+6, VO_BLT_SRCTRANSPARENCY,NULL);
	}

	//blit the left and right row of images
	usPosX = 0;
	for(i=TEXT_POPUP_GAP_BN_LINES; i< usHeight-TEXT_POPUP_GAP_BN_LINES; i+=TEXT_POPUP_GAP_BN_LINES)
	{
		BltVideoObject(pPopUpTextBox->uiSourceBufferIndex, hImageHandle, 3,usPosX, i, VO_BLT_SRCTRANSPARENCY,NULL);
		BltVideoObject(pPopUpTextBox->uiSourceBufferIndex, hImageHandle, 4,usPosX+usWidth-4, i, VO_BLT_SRCTRANSPARENCY,NULL);
	}

	//blt the corner images for the row
	//top left
	BltVideoObject(pPopUpTextBox->uiSourceBufferIndex, hImageHandle, 0, 0, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);
	//top right
	BltVideoObject(pPopUpTextBox->uiSourceBufferIndex, hImageHandle, 2, usWidth-TEXT_POPUP_GAP_BN_LINES, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);
	//bottom left
	BltVideoObject(pPopUpTextBox->uiSourceBufferIndex, hImageHandle, 5, 0, usHeight-TEXT_POPUP_GAP_BN_LINES, VO_BLT_SRCTRANSPARENCY,NULL);
	//bottom right
	BltVideoObject(pPopUpTextBox->uiSourceBufferIndex, hImageHandle, 7, usWidth-TEXT_POPUP_GAP_BN_LINES, usHeight-TEXT_POPUP_GAP_BN_LINES, VO_BLT_SRCTRANSPARENCY,NULL);

	// Icon if ness....
	if ( pPopUpTextBox->uiFlags & MERC_POPUP_PREPARE_FLAGS_STOPICON )
	{
		BltVideoObjectFromIndex( pPopUpTextBox->uiSourceBufferIndex, guiBoxIcons, 0, 5, 4, VO_BLT_SRCTRANSPARENCY,NULL);
	}
	if ( pPopUpTextBox->uiFlags & MERC_POPUP_PREPARE_FLAGS_SKULLICON )
	{
		BltVideoObjectFromIndex( pPopUpTextBox->uiSourceBufferIndex, guiSkullIcons, 0, 9, 4, VO_BLT_SRCTRANSPARENCY,NULL);
	}

	//Get the font and shadow colors
	GetMercPopupBoxFontColor( ubBackgroundIndex, &ubFontColor, &ubFontShadowColor );

	SetFontShadow( ubFontShadowColor );
	SetFontDestBuffer( pPopUpTextBox->uiSourceBufferIndex, 0, 0, usWidth, usHeight, FALSE );

	//Display the text
	sDispTextXPos = (INT16)(( MERC_TEXT_POPUP_WINDOW_TEXT_OFFSET_X + usMarginX ));

	if ( pPopUpTextBox->uiFlags & ( MERC_POPUP_PREPARE_FLAGS_STOPICON | MERC_POPUP_PREPARE_FLAGS_SKULLICON ) )
	{
		sDispTextXPos += 30;
	}
	
//if language represents words with a single char
#ifdef SINGLE_CHAR_WORDS
	{
		//Enable the use of single word wordwrap
		if( gfUseWinFonts )
		{
			UseSingleCharWordsForWordWrap( TRUE );
		}

		//Display the text
		DisplayWrappedString( sDispTextXPos, (INT16)(( MERC_TEXT_POPUP_WINDOW_TEXT_OFFSET_Y + usMarginTopY ) ), usTextWidth, 2, MERC_TEXT_FONT, ubFontColor,	pString, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

		//Disable the use of single word wordwrap
		UseSingleCharWordsForWordWrap( FALSE );
	}
#else
	{
		//Display the text
		DisplayWrappedString( sDispTextXPos, (INT16)(( MERC_TEXT_POPUP_WINDOW_TEXT_OFFSET_Y + usMarginTopY ) ), usTextWidth, 2, MERC_TEXT_FONT, ubFontColor,	pString, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	}
#endif
	
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
	SetFontShadow(DEFAULT_SHADOW);

	if( iBoxId == -1 )
	{
		// now return attemp to add to pop up box list, if successful will return index
		return( AddPopUpBoxToList( pPopUpTextBox ) );
	}


	// set as current box
	SetCurrentPopUpBox( iBoxId );

	return( iBoxId );
}

//Deletes the surface thats contains the border, background and the text.
BOOLEAN RemoveMercPopupBox()
{
	INT32 iCounter = 0;

	// make sure the current box does in fact exist
	if( gPopUpTextBox == NULL )
	{
		// failed..
		return( FALSE );
	}

	// now check to see if inited...
	if( gPopUpTextBox->fMercTextPopupSurfaceInitialized )
	{
		// now find this box in the list
		for( iCounter = 0; iCounter < MAX_NUMBER_OF_POPUP_BOXES; ++iCounter )
		{
			if( gpPopUpBoxList[ iCounter ] == gPopUpTextBox )
			{
				gpPopUpBoxList[ iCounter ] = NULL;
				break;
			}
		}

		// yep, get rid of the bloody...
		DeleteVideoSurfaceFromIndex(gPopUpTextBox->uiSourceBufferIndex);

		//DEF Added 5/26
		//Delete the background and the border
		RemoveTextMercPopupImages( );

		MemFree( gPopUpTextBox );

		// reset current ptr
		gPopUpTextBox = NULL;
	}

	return(TRUE);
}


BOOLEAN RemoveMercPopupBoxFromIndex( UINT32 uiId )
{
	// find this box, set it to current, and delete it
	if( SetCurrentPopUpBox( uiId ) == FALSE )
	{
		// failed
		return( FALSE );
	}

	// now try to remove it
	return( RemoveMercPopupBox( ) );
}


//Pass in the background index, and pointers to the font and shadow color
void	GetMercPopupBoxFontColor( UINT8 ubBackgroundIndex, UINT8 *pubFontColor, UINT8 *pubFontShadowColor)
{
	switch( ubBackgroundIndex )
	{
		case BASIC_MERC_POPUP_BACKGROUND:
			*pubFontColor = TEXT_POPUP_COLOR;
			*pubFontShadowColor = DEFAULT_SHADOW;
			break;

		case WHITE_MERC_POPUP_BACKGROUND:
			*pubFontColor = 2;
			*pubFontShadowColor = FONT_MCOLOR_WHITE;
			break;

		case GREY_MERC_POPUP_BACKGROUND:
			*pubFontColor = 2;
			*pubFontShadowColor = NO_SHADOW;
			break;

		case LAPTOP_POPUP_BACKGROUND:
			*pubFontColor = TEXT_POPUP_COLOR;
			*pubFontShadowColor = DEFAULT_SHADOW;
			break;

		default:
			*pubFontColor = TEXT_POPUP_COLOR;
			*pubFontShadowColor = DEFAULT_SHADOW;
			break;
	}
}

BOOLEAN	SetPrepareMercPopupFlags( UINT32 uiFlags )
{
	guiFlags |= uiFlags;
	return( TRUE );
}

BOOLEAN SetPrepareMercPopUpFlagsFromIndex( UINT32 uiFlags, UINT32 uiId )
{
	// find this box, set it to current, and delete it
	if( SetCurrentPopUpBox( uiId ) == FALSE )
	{
		// failed
		return( FALSE );
	}

	// now try to remove it
	return( SetPrepareMercPopupFlags( uiFlags ) );
}
