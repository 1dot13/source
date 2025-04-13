#include "PopUpBox.h"
	#include "sysutil.h"


#define BORDER_WIDTH	16
#define BORDER_HEIGHT	8
#define TOP_LEFT_CORNER	 0
#define TOP_EDGE			4
#define TOP_RIGHT_CORNER	1
#define SIDE_EDGE			5
#define BOTTOM_LEFT_CORNER	2
#define BOTTOM_EDGE		 4
#define BOTTOM_RIGHT_CORNER 3


BOOLEAN DrawBox(UINT32 uiCounter);
BOOLEAN DrawBoxText(UINT32 uiCounter);

void RemoveCurrentBoxPrimaryText( INT32 hStringHandle );
void RemoveCurrentBoxSecondaryText( INT32 hStringHandle );
void RemoveCurrentBoxText( INT32 hStringHandle, UINT8 column );



void InitPopUpBoxList()
{
	memset( &PopUpBoxList, 0, sizeof( PopUpBoxPt ) );
	return;
}


void InitPopUpBox(INT32 hBoxHandle)
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );
	memset(PopUpBoxList[hBoxHandle], 0, sizeof(PopUpBo));
}



void SetLineSpace(INT32 hBoxHandle, UINT32 uiLineSpace)
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

 Assert( PopUpBoxList[ hBoxHandle ] );
 PopUpBoxList[hBoxHandle]->uiLineSpace=uiLineSpace;
 return;
}


UINT32 GetLineSpace( INT32 hBoxHandle )
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return( 0 );

	Assert( PopUpBoxList[ hBoxHandle ] );
	// return number of pixels between lines for this box
	return( PopUpBoxList[hBoxHandle]->uiLineSpace );
}



void SpecifyBoxMinWidth( INT32 hBoxHandle, INT32 iMinWidth )
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );

	PopUpBoxList[hBoxHandle]->uiBoxMinWidth = iMinWidth;

	// check if the box is currently too small
	if( PopUpBoxList[hBoxHandle]->Dimensions.iRight < iMinWidth )
	{
		PopUpBoxList[hBoxHandle]->Dimensions.iRight = iMinWidth;
	}

	return;
}


BOOLEAN CreatePopUpBox(INT32 *phBoxHandle, SGPRect Dimensions, SGPPoint Position, UINT32 uiFlags)
{
	INT32 iCounter=0;
	INT32 iCount=0;
	PopUpBoxPt pBox=NULL;


	// find first free box
	for(iCounter=0; ( iCounter < MAX_POPUP_BOX_COUNT ) && ( PopUpBoxList[iCounter] != NULL ); iCounter++);

	if( iCounter >= MAX_POPUP_BOX_COUNT )
	{
		// ran out of available popup boxes - probably not freeing them up right!
		Assert(0);
		return FALSE;
	}

	iCount=iCounter;
	*phBoxHandle=iCount;

	pBox= (PopUpBoxPt) MemAlloc(sizeof(PopUpBo));
	if (pBox == NULL)
	{
		return FALSE;
	}
	PopUpBoxList[iCount]=pBox;

	InitPopUpBox(iCount);
	SetBoxPosition(iCount, Position);
	SetBoxSize(iCount, Dimensions);
	SetBoxFlags(iCount, uiFlags);

	for ( UINT8 col = 0; col < MAX_POPUP_BOX_COLUMNS; ++col )
	{
		for ( iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; ++iCounter)
		{
			PopUpBoxList[iCount]->Text[col][iCounter] = NULL;
		}
	}

	for ( iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
	{
		PopUpBoxList[iCount]->pSecondColumnString[iCounter] = NULL;
	}

	SetCurrentBox(iCount);
	SpecifyBoxMinWidth( iCount, 0 );
	SetBoxSecondColumnMinimumOffset( iCount, 0 );
	SetBoxSecondColumnCurrentOffset( iCount, 0 );

	PopUpBoxList[iCount]->fUpdated = FALSE;

	return TRUE;
}


void SetBoxFlags( INT32 hBoxHandle, UINT32 uiFlags)
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );

	PopUpBoxList[hBoxHandle]->uiFlags=uiFlags;
	PopUpBoxList[hBoxHandle]->fUpdated = FALSE;

 return;
}


void SetMargins(INT32 hBoxHandle, UINT32 uiLeft, UINT32 uiTop, UINT32 uiBottom, UINT32 uiRight)
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );

	PopUpBoxList[hBoxHandle]->uiLeftMargin=uiLeft;
	PopUpBoxList[hBoxHandle]->uiRightMargin=uiRight;
	PopUpBoxList[hBoxHandle]->uiTopMargin=uiTop;
	PopUpBoxList[hBoxHandle]->uiBottomMargin=uiBottom;

	PopUpBoxList[hBoxHandle]->fUpdated = FALSE;

 return;
}


UINT32 GetTopMarginSize( INT32 hBoxHandle )
{
	// return size of top margin, for mouse region offsets

	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return( 0 );

	Assert( PopUpBoxList[ hBoxHandle ] );

	return( PopUpBoxList[hBoxHandle]->uiTopMargin );
}


void ShadeStringInBox( INT32 hBoxHandle, INT32 iLineNumber )
{
	// shade iLineNumber Line in box indexed by hBoxHandle

	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[hBoxHandle] );

	if ( PopUpBoxList[hBoxHandle]->Text[0][iLineNumber] != NULL )
	{
		// set current box
		SetCurrentBox( hBoxHandle );

		// shade line
		PopUpBoxList[hBoxHandle]->Text[0][iLineNumber]->fShadeFlag = TRUE;
	}
}

void UnShadeStringInBox( INT32 hBoxHandle, INT32 iLineNumber )
{
	// unshade iLineNumber in box indexed by hBoxHandle

	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );

	if( PopUpBoxList[hBoxHandle]->Text[0][iLineNumber]!=NULL)
	{
		// set current box
		SetCurrentBox( hBoxHandle );

		// shade line
		PopUpBoxList[hBoxHandle]->Text[0][iLineNumber]->fShadeFlag = FALSE;
	}
}


void SecondaryShadeStringInBox( INT32 hBoxHandle, INT32 iLineNumber )
{
	// shade iLineNumber Line in box indexed by hBoxHandle

	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );

	if( PopUpBoxList[hBoxHandle]->Text[0][iLineNumber]!=NULL)
	{
		// set current box
		SetCurrentBox( hBoxHandle );

		// shade line
		PopUpBoxList[hBoxHandle]->Text[0][iLineNumber]->fSecondaryShadeFlag = TRUE;
	}
}

void UnSecondaryShadeStringInBox( INT32 hBoxHandle, INT32 iLineNumber )
{
	// unshade iLineNumber in box indexed by hBoxHandle

	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );

	if( PopUpBoxList[hBoxHandle]->Text[0][iLineNumber]!=NULL)
	{
		// set current box
		SetCurrentBox( hBoxHandle );

		// shade line
		PopUpBoxList[hBoxHandle]->Text[0][iLineNumber]->fSecondaryShadeFlag = FALSE;
	}
}



void SetBoxBuffer(INT32 hBoxHandle, UINT32 uiBuffer)
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );

	PopUpBoxList[hBoxHandle]->uiBuffer=uiBuffer;

	PopUpBoxList[hBoxHandle]->fUpdated = FALSE;
}


void SetBoxPosition( INT32 hBoxHandle,SGPPoint Position )
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );

	PopUpBoxList[hBoxHandle]->Position.iX=Position.iX;
	PopUpBoxList[hBoxHandle]->Position.iY=Position.iY;

	PopUpBoxList[hBoxHandle]->fUpdated = FALSE;
}


void GetBoxPosition( INT32 hBoxHandle, SGPPoint *Position )
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );

	Position->iX = PopUpBoxList[hBoxHandle]->Position.iX;
	Position->iY = PopUpBoxList[hBoxHandle]->Position.iY;
}

void SetBoxSize(INT32 hBoxHandle,SGPRect Dimensions)
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );

	PopUpBoxList[hBoxHandle]->Dimensions.iLeft=Dimensions.iLeft;
	PopUpBoxList[hBoxHandle]->Dimensions.iBottom=Dimensions.iBottom;
	PopUpBoxList[hBoxHandle]->Dimensions.iRight=Dimensions.iRight;
	PopUpBoxList[hBoxHandle]->Dimensions.iTop=Dimensions.iTop;

	PopUpBoxList[hBoxHandle]->fUpdated = FALSE;
}


void GetBoxSize( INT32 hBoxHandle, SGPRect *Dimensions )
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );

	Dimensions->iLeft = PopUpBoxList[hBoxHandle]->Dimensions.iLeft;
	Dimensions->iBottom = PopUpBoxList[hBoxHandle]->Dimensions.iBottom;
	Dimensions->iRight = PopUpBoxList[hBoxHandle]->Dimensions.iRight;
	Dimensions->iTop = PopUpBoxList[hBoxHandle]->Dimensions.iTop;
}


void SetBorderType(INT32 hBoxHandle, INT32 iBorderObjectIndex)
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );
	PopUpBoxList[hBoxHandle]->iBorderObjectIndex=iBorderObjectIndex;
}

void SetBackGroundSurface(INT32 hBoxHandle, INT32 iBackGroundSurfaceIndex)
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ hBoxHandle ] );
	PopUpBoxList[hBoxHandle]->iBackGroundSurface=iBackGroundSurfaceIndex;
}


// adds a FIRST column string to the CURRENT popup box
void AddMonoString( UINT32 *hStringHandle, STR16 pString )
{
	STR16 pLocalString = NULL;
	POPUPSTRINGPTR pStringSt = NULL;
	INT32 iCounter = 0;


	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );

	// find first free slot in list
	for ( iCounter = 0; (iCounter < MAX_POPUP_BOX_STRING_COUNT) && (PopUpBoxList[guiCurrentBox]->Text[0][iCounter] != NULL); iCounter++ );

	if ( iCounter >= MAX_POPUP_BOX_STRING_COUNT )
	{
		// using too many text lines, or not freeing them up properly
		Assert( 0 );
		return;
	}

	pStringSt = (POPUPSTRING *)(MemAlloc( sizeof( POPUPSTRING ) ));
	if ( pStringSt == NULL )
		return;

	pLocalString = (STR16)MemAlloc( (wcslen( pString ) + 1) * sizeof( CHAR16 ) );
	if ( pLocalString == NULL )
		return;

	wcscpy( pLocalString, pString );

	RemoveCurrentBoxPrimaryText( iCounter );

	PopUpBoxList[guiCurrentBox]->Text[0][iCounter] = pStringSt;
	PopUpBoxList[guiCurrentBox]->Text[0][iCounter]->fColorFlag = FALSE;
	PopUpBoxList[guiCurrentBox]->Text[0][iCounter]->pString = pLocalString;
	PopUpBoxList[guiCurrentBox]->Text[0][iCounter]->fShadeFlag = FALSE;
	PopUpBoxList[guiCurrentBox]->Text[0][iCounter]->fHighLightFlag = FALSE;
	PopUpBoxList[guiCurrentBox]->Text[0][iCounter]->fSecondaryShadeFlag = FALSE;

	*hStringHandle = iCounter;
	PopUpBoxList[guiCurrentBox]->fUpdated = FALSE;
	return;
}


// Adds string to the current popup box 2nd column. !!! String's position is the LAST used position in the 1st column !!!
void AddSecondColumnMonoString( UINT32 *hStringHandle, STR16 pString )
{
	STR16 pLocalString=NULL;
	POPUPSTRINGPTR pStringSt=NULL;
	INT32 iCounter=0;


	if ( ( guiCurrentBox < 0 ) || ( guiCurrentBox >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );

	// find the LAST USED text string index
	for( iCounter = 0; ( iCounter + 1 < MAX_POPUP_BOX_STRING_COUNT ) && ( PopUpBoxList[guiCurrentBox]->Text[0][ iCounter + 1 ] != NULL ); iCounter++ );

	if ( iCounter >= MAX_POPUP_BOX_STRING_COUNT )
	{
		// using too many text lines, or not freeing them up properly
		Assert(0);
		return;
	}

	pStringSt=(POPUPSTRING *)(MemAlloc(sizeof(POPUPSTRING)));
	if (pStringSt == NULL)
		return;

	pLocalString=(STR16)MemAlloc((wcslen(pString)+1)*sizeof(CHAR16));
	if (pLocalString == NULL)
		return;

	wcscpy(pLocalString, pString);

	RemoveCurrentBoxSecondaryText( iCounter );

	PopUpBoxList[guiCurrentBox]->pSecondColumnString[iCounter]=pStringSt;
	PopUpBoxList[guiCurrentBox]->pSecondColumnString[iCounter]->fColorFlag=FALSE;
	PopUpBoxList[guiCurrentBox]->pSecondColumnString[iCounter]->pString=pLocalString;
	PopUpBoxList[guiCurrentBox]->pSecondColumnString[iCounter]->fShadeFlag = FALSE;
	PopUpBoxList[guiCurrentBox]->pSecondColumnString[iCounter]->fHighLightFlag = FALSE;

	*hStringHandle=iCounter;

	return;
}

// Adds a string to the first available slot in the second column
void AddMonoStringToSecondColumn(UINT32* hStringHandle, STR16 pString)
{
	STR16 pLocalString = NULL;
	POPUPSTRINGPTR pStringSt = NULL;
	INT32 iCounter = 0;


	if ((guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT))
		return;

	Assert(PopUpBoxList[guiCurrentBox] != NULL);

	// find the free text string index in 2nd column
	for (iCounter = 0; (iCounter < MAX_POPUP_BOX_STRING_COUNT) && (PopUpBoxList[guiCurrentBox]->pSecondColumnString[iCounter] != NULL); iCounter++);

	if (iCounter >= MAX_POPUP_BOX_STRING_COUNT)
	{
		// using too many text lines, or not freeing them up properly
		Assert(0);
		return;
	}

	pStringSt = (POPUPSTRING*)(MemAlloc(sizeof(POPUPSTRING)));
	if (pStringSt == NULL)
		return;

	pLocalString = (STR16)MemAlloc((wcslen(pString) + 1) * sizeof(CHAR16));
	if (pLocalString == NULL)
		return;

	wcscpy(pLocalString, pString);

	RemoveCurrentBoxSecondaryText(iCounter);

	PopUpBoxList[guiCurrentBox]->pSecondColumnString[iCounter] = pStringSt;
	PopUpBoxList[guiCurrentBox]->pSecondColumnString[iCounter]->fColorFlag = FALSE;
	PopUpBoxList[guiCurrentBox]->pSecondColumnString[iCounter]->pString = pLocalString;
	PopUpBoxList[guiCurrentBox]->pSecondColumnString[iCounter]->fShadeFlag = FALSE;
	PopUpBoxList[guiCurrentBox]->pSecondColumnString[iCounter]->fHighLightFlag = FALSE;

	*hStringHandle = iCounter;

	return;
}

// Adds a string to the first available slot in the column
void AddStringToBoxColumn( UINT32 *hStringHandle, STR16 pString, UINT8 column )
{
	STR16 pLocalString = NULL;
	POPUPSTRINGPTR pStringSt = NULL;
	INT32 iCounter = 0;


	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) || column >= MAX_POPUP_BOX_COLUMNS )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );

	// find first free slot in list
	for ( iCounter = 0; (iCounter < MAX_POPUP_BOX_STRING_COUNT) && (PopUpBoxList[guiCurrentBox]->Text[column][iCounter] != NULL); iCounter++ );

	if ( iCounter >= MAX_POPUP_BOX_STRING_COUNT )
	{
		// using too many text lines, or not freeing them up properly
		Assert( 0 );
		return;
	}

	pStringSt = (POPUPSTRING *)(MemAlloc( sizeof( POPUPSTRING ) ));
	if ( pStringSt == NULL )
		return;

	pLocalString = (STR16)MemAlloc( (wcslen( pString ) + 1) * sizeof( CHAR16 ) );
	if ( pLocalString == NULL )
		return;

	wcscpy( pLocalString, pString );

	RemoveCurrentBoxText( iCounter, column );

	PopUpBoxList[guiCurrentBox]->Text[column][iCounter] = pStringSt;
	PopUpBoxList[guiCurrentBox]->Text[column][iCounter]->fColorFlag = FALSE;
	PopUpBoxList[guiCurrentBox]->Text[column][iCounter]->pString = pLocalString;
	PopUpBoxList[guiCurrentBox]->Text[column][iCounter]->fShadeFlag = FALSE;
	PopUpBoxList[guiCurrentBox]->Text[column][iCounter]->fHighLightFlag = FALSE;
	PopUpBoxList[guiCurrentBox]->Text[column][iCounter]->fSecondaryShadeFlag = FALSE;

	*hStringHandle = iCounter;
	PopUpBoxList[guiCurrentBox]->fUpdated = FALSE;
	return;
}

// Adds string to the current popup box column besides first. !!! String's position is the LAST used position in the 1st column !!!
void AddColumnMonoString( UINT32 *hStringHandle, STR16 pString, UINT8 column )
{
	STR16 pLocalString = NULL;
	POPUPSTRINGPTR pStringSt = NULL;
	INT32 iCounter = 0;


	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) || column < 1 || column >= MAX_POPUP_BOX_COLUMNS)
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );

	// find the LAST USED text string index
	for ( iCounter = 0; (iCounter + 1 < MAX_POPUP_BOX_STRING_COUNT) && (PopUpBoxList[guiCurrentBox]->Text[0][iCounter + 1] != NULL); iCounter++ );

	if ( iCounter >= MAX_POPUP_BOX_STRING_COUNT )
	{
		// using too many text lines, or not freeing them up properly
		Assert( 0 );
		return;
	}

	pStringSt = (POPUPSTRING *)(MemAlloc( sizeof( POPUPSTRING ) ));
	if ( pStringSt == NULL )
		return;

	pLocalString = (STR16)MemAlloc( (wcslen( pString ) + 1) * sizeof( CHAR16 ) );
	if ( pLocalString == NULL )
		return;

	wcscpy( pLocalString, pString );

	RemoveCurrentBoxText( iCounter, column );

	PopUpBoxList[guiCurrentBox]->Text[column][iCounter] = pStringSt;
	PopUpBoxList[guiCurrentBox]->Text[column][iCounter]->fColorFlag = FALSE;
	PopUpBoxList[guiCurrentBox]->Text[column][iCounter]->pString = pLocalString;
	PopUpBoxList[guiCurrentBox]->Text[column][iCounter]->fShadeFlag = FALSE;
	PopUpBoxList[guiCurrentBox]->Text[column][iCounter]->fHighLightFlag = FALSE;

	*hStringHandle = iCounter;
	return;
}


// Adds a COLORED first column string to the CURRENT box
void AddColorString(INT32 *hStringHandle, STR16 pString)
{
 STR16 pLocalString;
 POPUPSTRINGPTR pStringSt=NULL;
 INT32 iCounter=0;


	if ( ( guiCurrentBox < 0 ) || ( guiCurrentBox >= MAX_POPUP_BOX_COUNT ) )
		return;

 Assert( PopUpBoxList[guiCurrentBox] != NULL );

 // find first free slot in list
 for( iCounter = 0; ( iCounter < MAX_POPUP_BOX_STRING_COUNT ) && ( PopUpBoxList[guiCurrentBox]->Text[0][iCounter] != NULL ); iCounter++ );

 if ( iCounter >= MAX_POPUP_BOX_STRING_COUNT )
 {
	// using too many text lines, or not freeing them up properly
	Assert(0);
	return;
 }

 pStringSt=(POPUPSTRING *)(MemAlloc(sizeof(POPUPSTRING)));
 if (pStringSt == NULL)
	return;

 pLocalString=(STR16)MemAlloc((wcslen(pString)+1)*sizeof(CHAR16));
 if (pLocalString == NULL)
	return;

 wcscpy(pLocalString, pString);

 RemoveCurrentBoxPrimaryText( iCounter );

 PopUpBoxList[guiCurrentBox]->Text[0][iCounter]=pStringSt;
 PopUpBoxList[guiCurrentBox]->Text[0][iCounter]->fColorFlag=TRUE;
 PopUpBoxList[guiCurrentBox]->Text[0][iCounter]->pString=pLocalString;

 *hStringHandle=iCounter;

 PopUpBoxList[guiCurrentBox]->fUpdated = FALSE;

 return;
}



void ResizeBoxForSecondStrings( INT32 hBoxHandle )
{
	INT32 iCounter = 0;
	PopUpBoxPt pBox;
	UINT32 uiBaseWidth, uiThisWidth;


	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	pBox = ( PopUpBoxList[ hBoxHandle ] );
	Assert( pBox );

	uiBaseWidth = pBox->uiLeftMargin + pBox->uiSecondColumnMinimunOffset;

	// check string sizes
	for( iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
	{
		if( pBox->Text[0][ iCounter ] )
		{
			uiThisWidth = uiBaseWidth + StringPixLength( pBox->Text[0][ iCounter ]->pString, pBox->Text[0][ iCounter ]->uiFont );

			if( uiThisWidth > pBox->uiSecondColumnCurrentOffset )
			{
				pBox->uiSecondColumnCurrentOffset = uiThisWidth;
			}
		}
	}
}

static void ResizeStrategicMvtBoxForSecondStrings( INT32 hBoxHandle )
{
	INT32 iCounter = 0;
	PopUpBoxPt pBox;
	UINT32 uiBaseWidth, uiThisWidth;


	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	pBox = (PopUpBoxList[hBoxHandle]);
	Assert( pBox );

	// Determine last line for next loop
	UINT8 last = MAX_POPUP_BOX_STRING_COUNT;
	for ( iCounter = 1; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
	{
		if ( pBox->Text[0][iCounter] == nullptr )
		{
			// Do not consider the last blank, "cancel" & "Plot Travel route" lines
			last = iCounter - 3;
			break;
		}
	}

	uiBaseWidth = pBox->uiLeftMargin + pBox->uiSecondColumnMinimunOffset;

	// check string sizes, skip title line
	for ( iCounter = 1; iCounter < last; iCounter++ )
	{
		if ( pBox->Text[0][iCounter] )
		{
			uiThisWidth = uiBaseWidth + StringPixLength( pBox->Text[0][iCounter]->pString, pBox->Text[0][iCounter]->uiFont );

			if ( uiThisWidth > pBox->uiSecondColumnCurrentOffset )
			{
				// Maintain width if something is selected and the '*' characters have been added
				if ( wcsncmp( pBox->Text[0][iCounter]->pString, L"  *", 3 ) == 0 )
				{
					uiThisWidth -= 10;
				}
				pBox->uiSecondColumnCurrentOffset = uiThisWidth;
			}
		}
	}
}


UINT32 GetNumberOfLinesOfTextInBox( INT32 hBoxHandle )
{
	INT32 iCounter = 0;

	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return( 0 );

	// count number of lines
	// check string size
	for( iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
	{
		if( PopUpBoxList[ hBoxHandle ]->Text[0][ iCounter ] == NULL )
		{
			break;
		}
	}

	return( iCounter );
}


UINT32 GetNumberOfSecondaryLinesOfTextInBox(INT32 hBoxHandle)
{
	INT32 iCounter = 0;

	if ((hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT))
		return(0);

	// count number of lines
	// check string size
	for (iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++)
	{
		if (PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter] == NULL)
		{
			break;
		}
	}

	return(iCounter);
}

UINT32 GetTotalNumberOfLinesOfTextInBox( INT32 hBoxHandle )
{
	UINT32 lines = 0;
	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return(0);

	// count number of lines
	// check string size
	for ( size_t i = 0; i < MAX_POPUP_BOX_COLUMNS; i++ )
	{
		for ( INT32 iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
		{
			if ( PopUpBoxList[hBoxHandle]->Text[i][iCounter] == NULL )
			{
				break;
			}
			lines += 1;
		}
	}

	return(lines);
}


void SetBoxFont( INT32 hBoxHandle, UINT32 uiFont )
{
	UINT32 uiCounter;

	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	for ( uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
	{
		if ( PopUpBoxList[hBoxHandle]->Text[0][uiCounter] != NULL )
		{
			PopUpBoxList[hBoxHandle]->Text[0][uiCounter]->uiFont = uiFont;
		}
	}

	// set up the 2nd column font
	SetBoxSecondColumnFont( hBoxHandle, uiFont );

	PopUpBoxList[hBoxHandle]->fUpdated = FALSE;

	return;
}

void SetBoxSecondColumnMinimumOffset( INT32 hBoxHandle, UINT32 uiWidth )
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	PopUpBoxList[hBoxHandle]->uiSecondColumnMinimunOffset = uiWidth;
	return;
}

void SetBoxSecondColumnCurrentOffset( INT32 hBoxHandle, UINT32 uiCurrentOffset )
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	PopUpBoxList[hBoxHandle]->uiSecondColumnCurrentOffset = uiCurrentOffset;
	return;
}

void SetBoxSecondColumnOffsetAdjustment(INT32 hBoxHandle, INT16 x)
{
	if ((hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT))
		return;

	PopUpBoxList[hBoxHandle]->usSecondColumnOffsetAdjustment = x;
	return;
}

void SetBoxSecondColumnFont( INT32 hBoxHandle, UINT32 uiFont )
{
	UINT32 iCounter = 0;

	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	for ( iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
	{
		if ( PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter] )
		{
			PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter]->uiFont = uiFont;
		}
	}

	PopUpBoxList[hBoxHandle]->fUpdated = FALSE;

	return;
}

void SetBoxColumnFont( INT32 hBoxHandle, UINT32 uiFont, UINT8 column )
{
	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	for ( UINT32 uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
	{
		if ( PopUpBoxList[hBoxHandle]->Text[column][uiCounter] != NULL )
		{
			PopUpBoxList[hBoxHandle]->Text[column][uiCounter]->uiFont = uiFont;
		}
	}

	PopUpBoxList[hBoxHandle]->fUpdated = FALSE;
	return;
}



UINT32 GetBoxFont( INT32 hBoxHandle )
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return( 0 );

	Assert( PopUpBoxList[ hBoxHandle ] );
	Assert( PopUpBoxList[ hBoxHandle ]->Text[0][ 0 ] );

	// return font id of first line of text of box
	return( PopUpBoxList[hBoxHandle]->Text[0][ 0 ]->uiFont );
}


// set the foreground color of this string in this pop up box
void SetBoxLineForeground( INT32 iBox, INT32 iStringValue, UINT8 ubColor )
{
	if ( ( iBox < 0 ) || ( iBox >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ iBox ] );
	Assert( PopUpBoxList[ iBox ]->Text[0][iStringValue] );

	PopUpBoxList[iBox]->Text[0][iStringValue]->ubForegroundColor=ubColor;
	return;
}

void SetBoxSecondaryShade( INT32 iBox, UINT8 ubColor )
{
	UINT32 uiCounter;

	if ( ( iBox < 0 ) || ( iBox >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[ iBox ] );

	for ( uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
	{
		if (PopUpBoxList[iBox]->Text[0][uiCounter]!=NULL)
		{
			PopUpBoxList[iBox]->Text[0][uiCounter]->ubSecondaryShade=ubColor;
		}
	}
	return;
}


// The following functions operate on the CURRENT box

void SetPopUpStringFont(INT32 hStringHandle, UINT32 uiFont)
{
	if ( ( guiCurrentBox < 0 ) || ( guiCurrentBox >= MAX_POPUP_BOX_COUNT ) )
		return;

 Assert( PopUpBoxList[guiCurrentBox] != NULL );
 Assert( PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle] );

 PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]->uiFont=uiFont;
 return;
}


void SetPopUpSecondColumnStringFont(INT32 hStringHandle, UINT32 uiFont)
{
	if ( ( guiCurrentBox < 0 ) || ( guiCurrentBox >= MAX_POPUP_BOX_COUNT ) )
		return;

 Assert( PopUpBoxList[guiCurrentBox] != NULL );
 Assert( PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle] );

 PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle]->uiFont=uiFont;
 return;
}



void SetStringSecondaryShade( INT32 hStringHandle, UINT8 ubColor )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle] );

	PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]->ubSecondaryShade = ubColor;
	return;
}

void SetStringForeground( INT32 hStringHandle, UINT8 ubColor )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle] );

	PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]->ubForegroundColor = ubColor;
	return;
}

void SetStringBackground( INT32 hStringHandle, UINT8 ubColor )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle] );

	PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]->ubBackgroundColor = ubColor;
	return;
}

void SetStringHighLight( INT32 hStringHandle, UINT8 ubColor )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle] );

	PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]->ubHighLight = ubColor;
	return;
}


void SetStringShade( INT32 hStringHandle, UINT8 ubShade )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle] );

	PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]->ubShade = ubShade;
	return;
}

void SetStringSecondColumnForeground( INT32 hStringHandle, UINT8 ubColor )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle] );

	PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle]->ubForegroundColor = ubColor;
	return;
}

void SetStringSecondColumnBackground( INT32 hStringHandle, UINT8 ubColor )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle] );

	PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle]->ubBackgroundColor = ubColor;
	return;
}

void SetStringSecondColumnHighLight( INT32 hStringHandle, UINT8 ubColor )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle] );

	PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle]->ubHighLight = ubColor;
	return;
}


void SetStringSecondColumnShade( INT32 hStringHandle, UINT8 ubShade )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle] );

	PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle]->ubShade = ubShade;
	return;
}



void SetBoxForeground(INT32 hBoxHandle, UINT8 ubColor)
{
	UINT32 uiCounter;

	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[hBoxHandle] != NULL );

	for ( uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
	{
		if (PopUpBoxList[hBoxHandle]->Text[0][uiCounter]!=NULL)
		{
		PopUpBoxList[hBoxHandle]->Text[0][uiCounter]->ubForegroundColor=ubColor;
		}
	}
	return;
}

void SetBoxBackground(INT32 hBoxHandle, UINT8 ubColor)
{
	UINT32 uiCounter;

	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[hBoxHandle] != NULL );

	for ( uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
	{
		if (PopUpBoxList[hBoxHandle]->Text[0][uiCounter]!=NULL)
		{
		PopUpBoxList[hBoxHandle]->Text[0][uiCounter]->ubBackgroundColor=ubColor;
		}
	}
	return;
}

void SetBoxHighLight(INT32 hBoxHandle, UINT8 ubColor)
{
	UINT32 uiCounter;

	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[hBoxHandle] != NULL );

	for ( uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
	{
		if (PopUpBoxList[hBoxHandle]->Text[0][uiCounter]!=NULL)
		{
		PopUpBoxList[hBoxHandle]->Text[0][uiCounter]->ubHighLight=ubColor;
		}
	}
	return;
}

void SetBoxShade(INT32 hBoxHandle, UINT8 ubColor)
{
	UINT32 uiCounter;

	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[hBoxHandle] != NULL );

	for ( uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
	{
		if (PopUpBoxList[hBoxHandle]->Text[0][uiCounter]!=NULL)
		{
		PopUpBoxList[hBoxHandle]->Text[0][uiCounter]->ubShade=ubColor;
		}
	}
	return;
}

void SetBoxSecondColumnForeground( INT32 hBoxHandle, UINT8 ubColor )
{
	UINT32 iCounter = 0;

	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[hBoxHandle] != NULL );

	for ( iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
	{
		if ( PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter] )
		{
			PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter]->ubForegroundColor = ubColor;
		}
	}

	return;
}

void SetBoxSecondColumnBackground( INT32 hBoxHandle, UINT8 ubColor )
{
	UINT32 iCounter = 0;

	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[hBoxHandle] != NULL );

	for ( iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
	{
		if ( PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter] )
		{
			PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter]->ubBackgroundColor = ubColor;
		}
	}

	return;
}

void SetBoxSecondColumnHighLight( INT32 hBoxHandle, UINT8 ubColor )
{
	UINT32 iCounter = 0;

	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[hBoxHandle] != NULL );

	for ( iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
	{
		if ( PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter] )
		{
			PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter]->ubHighLight = ubColor;
		}
	}

	return;
}

void SetBoxSecondColumnShade( INT32 hBoxHandle, UINT8 ubColor )
{
	UINT32 iCounter = 0;

	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[hBoxHandle] != NULL );

	for ( iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
	{
		if ( PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter] )
		{
			PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter]->ubShade = ubColor;
		}
	}
	return;
}


void SetBoxColumnForeground( INT32 hBoxHandle, UINT8 ubColor, UINT8 column )
{
	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	auto box = PopUpBoxList[hBoxHandle];
	Assert( box != NULL );

	for ( UINT32 uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
	{
		if ( box->Text[column][uiCounter] != NULL )
		{
			box->Text[column][uiCounter]->ubForegroundColor = ubColor;
		}
	}
	return;
}

void SetBoxColumnBackground( INT32 hBoxHandle, UINT8 ubColor, UINT8 column )
{
	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	auto box = PopUpBoxList[hBoxHandle];
	Assert( box != NULL );

	for ( UINT32 uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
	{
		if ( box->Text[column][uiCounter] != NULL )
		{
			box->Text[column][uiCounter]->ubBackgroundColor = ubColor;
		}
	}
	return;
}

void SetBoxColumnHighLight( INT32 hBoxHandle, UINT8 ubColor, UINT8 column )
{
	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	auto box = PopUpBoxList[hBoxHandle];
	Assert( box != NULL );

	for ( UINT32 uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
	{
		if ( box->Text[column][uiCounter] != NULL )
		{
			box->Text[column][uiCounter]->ubHighLight = ubColor;
		}
	}
	return;
}

void SetBoxColumnShade( INT32 hBoxHandle, UINT8 ubColor, UINT8 column )
{
	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	auto box = PopUpBoxList[hBoxHandle];
	Assert( box != NULL );

	for ( UINT32 uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
	{
		if ( box->Text[column][uiCounter] != NULL )
		{
			box->Text[column][uiCounter]->ubShade = ubColor;
		}
	}
	return;
}



void HighLightLine( INT32 hStringHandle, UINT8 column )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	auto box = PopUpBoxList[guiCurrentBox];
	Assert( box != NULL );

	if ( !box->Text[column][hStringHandle] )
		return;
	box->Text[column][hStringHandle]->fHighLightFlag = TRUE;
	return;
}

void HighLight2ndLine( INT32 hStringHandle )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	auto box = PopUpBoxList[guiCurrentBox];
	Assert( box != NULL );

	if ( !box->pSecondColumnString[hStringHandle] )
		return;
	box->pSecondColumnString[hStringHandle]->fHighLightFlag = TRUE;
	return;
}

BOOLEAN GetShadeFlag( INT32 hStringHandle )
{
	if ( ( guiCurrentBox < 0 ) || ( guiCurrentBox >= MAX_POPUP_BOX_COUNT ) )
		return(FALSE);

	Assert( PopUpBoxList[guiCurrentBox] != NULL );

	if(!PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle])
	return( FALSE );

	return( PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]->fShadeFlag);
}

BOOLEAN GetSecondaryShadeFlag( INT32 hStringHandle )
{
	if ( ( guiCurrentBox < 0 ) || ( guiCurrentBox >= MAX_POPUP_BOX_COUNT ) )
		return(FALSE);

	Assert( PopUpBoxList[guiCurrentBox] != NULL );

	if(!PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle])
	return( FALSE );

	return( PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]->fSecondaryShadeFlag );
}


void HighLightBoxLine( INT32 hBoxHandle, INT32 iLineNumber, UINT8 column )
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	// highlight iLineNumber Line in box indexed by hBoxHandle

	if( PopUpBoxList[hBoxHandle]->Text[column][iLineNumber]!=NULL)
	{
		// set current box
		SetCurrentBox( hBoxHandle );

		// highlight line
		HighLightLine( iLineNumber, column );
	}

	return;
}

void HighLight2ndBoxLine( INT32 hBoxHandle, INT32 iLineNumber )
{
	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	// highlight iLineNumber Line in box indexed by hBoxHandle
	if ( PopUpBoxList[hBoxHandle]->pSecondColumnString[iLineNumber] != NULL )
	{
		SetCurrentBox( hBoxHandle );
		HighLight2ndLine( iLineNumber );
	}

	return;
}

BOOLEAN GetBoxShadeFlag( INT32 hBoxHandle, INT32 iLineNumber )
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return(FALSE);

	if( PopUpBoxList[hBoxHandle]->Text[0][iLineNumber]!=NULL)
	{
		return(	PopUpBoxList[hBoxHandle]->Text[0][iLineNumber]->fShadeFlag );
	}


	return( FALSE );
}

BOOLEAN GetBoxSecondaryShadeFlag( INT32 hBoxHandle, INT32 iLineNumber )
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return(FALSE);

	if( PopUpBoxList[hBoxHandle]->Text[0][iLineNumber]!=NULL)
	{
		return(	PopUpBoxList[hBoxHandle]->Text[0][iLineNumber]->fSecondaryShadeFlag );
	}


	return( FALSE );
}

void UnHighLightLine( INT32 hStringHandle )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );

	if ( !PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle] )
		return;
	PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]->fHighLightFlag = FALSE;
	return;
}

void UnHighLightBox( INT32 hBoxHandle )
{
	INT32 iCounter = 0;

	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	auto box = PopUpBoxList[hBoxHandle];
	for ( size_t i = 0; i < MAX_POPUP_BOX_COLUMNS; i++ )
	{
		for ( iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
		{
			if ( box->Text[i][iCounter] )
				box->Text[i][iCounter]->fHighLightFlag = FALSE;
		}
	}

	for ( iCounter = 0; iCounter < MAX_POPUP_BOX_STRING_COUNT; iCounter++ )
	{
		if ( box->pSecondColumnString[iCounter] )
			box->pSecondColumnString[iCounter]->fHighLightFlag = FALSE;
	}
}

void UnHighLightSecondColumnLine(INT32 hStringHandle)
{
	if ( ( guiCurrentBox < 0 ) || ( guiCurrentBox >= MAX_POPUP_BOX_COUNT ) )
		return;

 Assert( PopUpBoxList[guiCurrentBox] != NULL );

 if(!PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle])
	return;

 PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle]->fHighLightFlag=FALSE;
 return;
}

void UnHighLightSecondColumnBox(INT32 hBoxHandle)
{
 INT32 iCounter=0;

	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

 for(iCounter=0; iCounter <MAX_POPUP_BOX_STRING_COUNT;iCounter++)
 {
	if(PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter])
	PopUpBoxList[hBoxHandle]->pSecondColumnString[iCounter]->fHighLightFlag=FALSE;
 }
}

void RemoveOneCurrentBoxString(INT32 hStringHandle, BOOLEAN fFillGaps)
{
	UINT32 uiCounter=0;

	if ( ( guiCurrentBox < 0 ) || ( guiCurrentBox >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( hStringHandle < MAX_POPUP_BOX_STRING_COUNT );

	RemoveCurrentBoxPrimaryText( hStringHandle );
	RemoveCurrentBoxSecondaryText( hStringHandle );

	if ( fFillGaps )
	{
		// shuffle all strings down a slot to fill in the gap
		for ( uiCounter = hStringHandle; uiCounter < ( MAX_POPUP_BOX_STRING_COUNT - 1 ); uiCounter++ )
		{
			PopUpBoxList[guiCurrentBox]->Text[0][uiCounter]=PopUpBoxList[guiCurrentBox]->Text[0][uiCounter+1];
			PopUpBoxList[guiCurrentBox]->pSecondColumnString[uiCounter]=PopUpBoxList[guiCurrentBox]->pSecondColumnString[uiCounter+1];
		}
	}

	PopUpBoxList[guiCurrentBox]->fUpdated = FALSE;
}


void RemoveAllCurrentBoxStrings( void )
{
	UINT32 uiCounter;

	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) || !PopUpBoxList[guiCurrentBox] )
		return;

	for ( size_t i = 0; i < MAX_POPUP_BOX_COLUMNS; i++ )
	{
		for ( uiCounter = 0; uiCounter < MAX_POPUP_BOX_STRING_COUNT; uiCounter++ )
		{
			RemoveCurrentBoxText( uiCounter, i );
		}
	}
}


void RemoveBox( INT32 hBoxHandle )
{
	INT32 hOldBoxHandle;


	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	GetCurrentBox( &hOldBoxHandle );
	SetCurrentBox( hBoxHandle );

	RemoveAllCurrentBoxStrings();

	MemFree( PopUpBoxList[hBoxHandle] );
	PopUpBoxList[hBoxHandle] = NULL;

	if ( hOldBoxHandle != hBoxHandle )
		SetCurrentBox( hOldBoxHandle );

	return;
}



void ShowBox(INT32 hBoxHandle)
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	if( PopUpBoxList[hBoxHandle] != NULL )
	{
		if( PopUpBoxList[hBoxHandle]->fShowBox == FALSE )
		{
			PopUpBoxList[hBoxHandle]->fShowBox = TRUE;
			PopUpBoxList[hBoxHandle]->fUpdated = FALSE;
		}
	}
}

void HideBox(INT32 hBoxHandle)
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	if( PopUpBoxList[hBoxHandle] != NULL )
	{
		if( PopUpBoxList[hBoxHandle]->fShowBox == TRUE )
		{
			PopUpBoxList[hBoxHandle]->fShowBox = FALSE;
			PopUpBoxList[hBoxHandle]->fUpdated = FALSE;
		}
	}
}



void SetCurrentBox(INT32 hBoxHandle)
{
	if ( ( hBoxHandle < 0 ) || ( hBoxHandle >= MAX_POPUP_BOX_COUNT ) )
		return;

	guiCurrentBox = hBoxHandle;
}


void GetCurrentBox(INT32 *hBoxHandle)
{
	*hBoxHandle = guiCurrentBox;
}



void DisplayBoxes(UINT32 uiBuffer)
{
	UINT32 uiCounter;

	for( uiCounter=0; uiCounter < MAX_POPUP_BOX_COUNT; uiCounter++ )
	{
		DisplayOnePopupBox( uiCounter, uiBuffer );
	}
	return;
}


void DisplayOnePopupBox( UINT32 uiIndex, UINT32 uiBuffer )
{
	if ( (uiIndex < 0) || (uiIndex >= MAX_POPUP_BOX_COUNT) )
		return;

	if ( PopUpBoxList[uiIndex] != NULL )
	{
		if ( (PopUpBoxList[uiIndex]->uiBuffer == uiBuffer) && (PopUpBoxList[uiIndex]->fShowBox) )
		{
			DrawBox( uiIndex );
			DrawBoxText( uiIndex );
		}
	}
}



// force an update of this box
void ForceUpDateOfBox( UINT32 uiIndex )
{
	if ( ( uiIndex < 0 ) || ( uiIndex >= MAX_POPUP_BOX_COUNT ) )
		return;

	if( PopUpBoxList[ uiIndex ] != NULL )
	{
		PopUpBoxList[ uiIndex ]->fUpdated = FALSE;
	}
}



BOOLEAN DrawBox(UINT32 uiCounter)
{
	// will build pop up box in usTopX, usTopY with dimensions usWidth and usHeight
	UINT32 uiNumTilesWide;
	UINT32 uiNumTilesHigh;
	UINT32 uiCount=0;
	HVOBJECT hBoxHandle;
	HVSURFACE hSrcVSurface;
	UINT32 uiDestPitchBYTES;
	UINT32 uiSrcPitchBYTES;
	UINT16	*pDestBuf;
	UINT8	*pSrcBuf;
	SGPRect clip;
	UINT16 usTopX, usTopY;
	UINT16 usWidth, usHeight;


	if ( ( uiCounter < 0 ) || ( uiCounter >= MAX_POPUP_BOX_COUNT ) )
		return(FALSE);


	Assert( PopUpBoxList[uiCounter] != NULL );

	// only update if we need to

	if( PopUpBoxList[uiCounter]->fUpdated == TRUE )
	{
		return( FALSE );
	}

	PopUpBoxList[uiCounter]->fUpdated = TRUE;

	if( PopUpBoxList[uiCounter]->uiFlags & POPUP_BOX_FLAG_RESIZE )
	{
		ResizeBoxToText(uiCounter);
	}

	usTopX=(UINT16)PopUpBoxList[uiCounter]->Position.iX;
	usTopY=(UINT16)PopUpBoxList[uiCounter]->Position.iY;
	usWidth = ((UINT16)(PopUpBoxList[uiCounter]->Dimensions.iRight - PopUpBoxList[uiCounter]->Dimensions.iLeft + PopUpBoxList[uiCounter]->usSecondColumnOffsetAdjustment));
	usHeight=((UINT16)(PopUpBoxList[uiCounter]->Dimensions.iBottom-PopUpBoxList[uiCounter]->Dimensions.iTop));

	//CHRISL: Adjust position for screen limits
	if((usTopX + usWidth) > SCREEN_WIDTH && usTopX > ((usTopX + usWidth) - SCREEN_WIDTH))
	{
		usTopX -= ((usTopX + usWidth) - SCREEN_WIDTH);
		PopUpBoxList[uiCounter]->Position.iX -= (PopUpBoxList[uiCounter]->Position.iX - usTopX);
	}
	if((usTopY + usHeight) > SCREEN_HEIGHT && usTopY > ((usTopY + usHeight) - SCREEN_HEIGHT))
	{
		usTopY -= ((usTopY + usHeight) - SCREEN_HEIGHT);
		PopUpBoxList[uiCounter]->Position.iY -= (PopUpBoxList[uiCounter]->Position.iY - usTopY);
	}

	// check if we have a min width, if so then update box for such
	if( ( PopUpBoxList[uiCounter]->uiBoxMinWidth ) && ( usWidth < PopUpBoxList[uiCounter]->uiBoxMinWidth ) )
	{
		usWidth = ( INT16 )( PopUpBoxList[uiCounter]->uiBoxMinWidth );
	}

	// make sure it will fit on screen!
	Assert( usTopX + usWidth <= SCREEN_WIDTH );
	Assert( usTopY + usHeight <= SCREEN_HEIGHT );

	// subtract 4 because the 2 2-pixel corners are handled separately
	uiNumTilesWide=((usWidth-4)/BORDER_WIDTH);
	uiNumTilesHigh=((usHeight-4)/BORDER_HEIGHT);

	clip.iLeft=0;
	clip.iRight=clip.iLeft+usWidth;
	clip.iTop=0;
	clip.iBottom=clip.iTop+usHeight;

	// blit in texture first, then borders
	// blit in surface
	pDestBuf = (UINT16*)LockVideoSurface(PopUpBoxList[uiCounter]->uiBuffer, &uiDestPitchBYTES);
	CHECKF( GetVideoSurface( &hSrcVSurface, PopUpBoxList[uiCounter]->iBackGroundSurface) );
	pSrcBuf = LockVideoSurface( PopUpBoxList[uiCounter]->iBackGroundSurface, &uiSrcPitchBYTES);
	Blt8BPPDataSubTo16BPPBuffer( pDestBuf, uiDestPitchBYTES, hSrcVSurface, pSrcBuf,uiSrcPitchBYTES,usTopX,usTopY, &clip);
	UnLockVideoSurface( PopUpBoxList[uiCounter]->iBackGroundSurface);
	UnLockVideoSurface(PopUpBoxList[uiCounter]->uiBuffer);
	GetVideoObject(&hBoxHandle, PopUpBoxList[uiCounter]->iBorderObjectIndex);

	// blit in 4 corners (they're 2x2 pixels)
	BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, TOP_LEFT_CORNER,usTopX,usTopY, VO_BLT_SRCTRANSPARENCY, NULL );
	BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, TOP_RIGHT_CORNER,usTopX+usWidth-2,usTopY, VO_BLT_SRCTRANSPARENCY, NULL );
	BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, BOTTOM_RIGHT_CORNER,usTopX+usWidth-2,usTopY+usHeight-2, VO_BLT_SRCTRANSPARENCY, NULL );
	BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, BOTTOM_LEFT_CORNER,usTopX,usTopY+usHeight-2, VO_BLT_SRCTRANSPARENCY, NULL );

	// blit in edges
	if (uiNumTilesWide > 0)
	{
		// full pieces
	for (uiCount=0; uiCount <uiNumTilesWide; uiCount++)
		{
	 BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, TOP_EDGE,	usTopX+2+(uiCount*BORDER_WIDTH),usTopY, VO_BLT_SRCTRANSPARENCY, NULL );
	 BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, BOTTOM_EDGE,usTopX+2+(uiCount*BORDER_WIDTH),usTopY+usHeight-2, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// partial pieces
		BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, TOP_EDGE,	usTopX+usWidth-2-BORDER_WIDTH,usTopY, VO_BLT_SRCTRANSPARENCY, NULL );
		BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, BOTTOM_EDGE,usTopX+usWidth-2-BORDER_WIDTH,usTopY+usHeight-2, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	if (uiNumTilesHigh > 0)
	{
		// full pieces
	for (uiCount=0; uiCount <uiNumTilesHigh; uiCount++)
		{
		BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, SIDE_EDGE,usTopX,			usTopY+2+(uiCount*BORDER_HEIGHT), VO_BLT_SRCTRANSPARENCY, NULL );
	 BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, SIDE_EDGE,usTopX+usWidth-2,usTopY+2+(uiCount*BORDER_HEIGHT), VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// partial pieces
		BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, SIDE_EDGE,usTopX,			usTopY+usHeight-2-BORDER_HEIGHT, VO_BLT_SRCTRANSPARENCY, NULL );
		BltVideoObject(PopUpBoxList[uiCounter]->uiBuffer, hBoxHandle, SIDE_EDGE,usTopX+usWidth-2,usTopY+usHeight-2-BORDER_HEIGHT, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	InvalidateRegion( usTopX, usTopY, usTopX + usWidth, usTopY + usHeight );
	return TRUE;
}



BOOLEAN DrawBoxText( UINT32 uiCounter )
{
	UINT32 uiCount = 0;
	INT16 uX, uY;
	CHAR16 sString[100];


	if ( (uiCounter < 0) || (uiCounter >= MAX_POPUP_BOX_COUNT) )
		return(FALSE);

	Assert( PopUpBoxList[uiCounter] != NULL );

	auto box = PopUpBoxList[uiCounter];
	//clip text?
	if ( box->uiFlags & POPUP_BOX_FLAG_CLIP_TEXT )
	{
		const auto x1 = box->Position.iX + box->uiLeftMargin - 1;
		const auto y1 = box->Position.iY + box->uiTopMargin;
		const auto x2 = box->Position.iX + box->Dimensions.iRight - box->uiRightMargin;
		const auto y2 = box->Position.iY + box->Dimensions.iBottom - box->uiBottomMargin;
		SetFontDestBuffer( box->uiBuffer, x1, y1, x2, y2, FALSE );
	}

	for ( size_t i = 0; i < MAX_POPUP_BOX_COLUMNS; i++ )
	{

		for ( uiCount = 0; uiCount < MAX_POPUP_BOX_STRING_COUNT; uiCount++ )
		{
			auto entry = box->Text[i][uiCount];
			// there is text in this line?
			if ( entry )
			{
				// set font
				SetFont( entry->uiFont );

				// are we highlighting?...shading?..or neither
				if ( (entry->fHighLightFlag == FALSE) && (entry->fShadeFlag == FALSE) && (entry->fSecondaryShadeFlag == FALSE) )
				{
					// neither
					SetFontForeground( entry->ubForegroundColor );
				}
				else if ( (entry->fHighLightFlag == TRUE) )
				{
					// highlight
					SetFontForeground( entry->ubHighLight );
				}
				else if ( (entry->fSecondaryShadeFlag == TRUE) )
				{
					SetFontForeground( entry->ubSecondaryShade );
				}
				else
				{
					//shading
					SetFontForeground( entry->ubShade );
				}

				// set background
				SetFontBackground( entry->ubBackgroundColor );

				// copy string
				wcsncpy( sString, entry->pString, wcslen( entry->pString ) + 1 );

				// centering?
				if ( box->uiFlags & POPUP_BOX_FLAG_CENTER_TEXT )
				{
					INT16 sLeft = box->Position.iX + box->uiLeftMargin;
					INT16 sTop = box->Position.iY + uiCount * GetFontHeight( entry->uiFont ) + box->uiTopMargin + uiCount * box->uiLineSpace;
					INT16 sWidth = box->Dimensions.iRight - (box->uiRightMargin + box->uiLeftMargin + 2);
					INT16 sHeight = GetFontHeight( entry->uiFont );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, (sString), ((INT32)entry->uiFont), &uX, &uY );
				}
				else
				{
					uX = box->Position.iX + box->uiLeftMargin + box->uiSecondColumnCurrentOffset*i;
					uY = ((INT16)(box->Position.iY + uiCount * GetFontHeight( entry->uiFont ) + box->uiTopMargin + uiCount * box->uiLineSpace));
				}

				// print
				mprintf( uX, uY, entry->pString );
			}
		}
	}

	for ( uiCount = 0; uiCount < MAX_POPUP_BOX_STRING_COUNT; uiCount++ )
	{
		// there is secondary text in this line?
		if ( box->pSecondColumnString[uiCount] )
		{
			// set font
			SetFont( box->pSecondColumnString[uiCount]->uiFont );

			// are we highlighting?...shading?..or neither
			if ( (box->pSecondColumnString[uiCount]->fHighLightFlag == FALSE) && (box->pSecondColumnString[uiCount]->fShadeFlag == FALSE) )
			{
				// neither
				SetFontForeground( box->pSecondColumnString[uiCount]->ubForegroundColor );
			}
			else if ( (box->pSecondColumnString[uiCount]->fHighLightFlag == TRUE) )
			{
				// highlight
				SetFontForeground( box->pSecondColumnString[uiCount]->ubHighLight );
			}
			else
			{
				//shading
				SetFontForeground( box->pSecondColumnString[uiCount]->ubShade );
			}

			// set background
			SetFontBackground( box->pSecondColumnString[uiCount]->ubBackgroundColor );

			// copy string
			wcsncpy( sString, box->pSecondColumnString[uiCount]->pString, wcslen( box->pSecondColumnString[uiCount]->pString ) + 1 );

			// centering?
			if ( box->uiFlags & POPUP_BOX_FLAG_CENTER_TEXT )
			{
				FindFontCenterCoordinates( ((INT16)(box->Position.iX + box->uiLeftMargin)), ((INT16)(box->Position.iY + uiCount * GetFontHeight( box->pSecondColumnString[uiCount]->uiFont ) + box->uiTopMargin + uiCount * box->uiLineSpace)), ((INT16)(box->Dimensions.iRight - (box->uiRightMargin + box->uiLeftMargin + 2))), ((INT16)GetFontHeight( box->pSecondColumnString[uiCount]->uiFont )), (sString), ((INT32)box->pSecondColumnString[uiCount]->uiFont), &uX, &uY );
			}
			else
			{
				uX = ((INT16)(box->Position.iX + box->uiLeftMargin + box->uiSecondColumnCurrentOffset + box->usSecondColumnOffsetAdjustment));
				uY = ((INT16)(box->Position.iY + uiCount * GetFontHeight( box->pSecondColumnString[uiCount]->uiFont ) + box->uiTopMargin + uiCount * box->uiLineSpace));
			}

			// print
			mprintf( uX, uY, box->pSecondColumnString[uiCount]->pString );
		}
	}


	if ( box->uiBuffer != guiSAVEBUFFER )
	{
		InvalidateRegion( box->Position.iX + box->uiLeftMargin - 1, box->Position.iY + box->uiTopMargin, box->Position.iX + box->Dimensions.iRight - box->uiRightMargin, box->Position.iY + box->Dimensions.iBottom - box->uiBottomMargin );
	}

	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	return TRUE;
}


void ResizeBoxToText( INT32 hBoxHandle )
{
	// run through lines of text in box and size box width to longest line plus margins
	// height is sum of getfontheight of each line+ spacing
	INT32 iWidth = 0;
	INT32 iHeight = 0;
	INT32 iCurrString = 0;
	INT32 iSecondColumnLength = 0;


	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return;

	if ( !PopUpBoxList[hBoxHandle] )
		return;

	if ( hBoxHandle == ghMoveBox )
	{
		ResizeStrategicMvtBoxForSecondStrings( hBoxHandle );
	}
	else
	{
		ResizeBoxForSecondStrings( hBoxHandle );
	}

	auto box = PopUpBoxList[hBoxHandle];
	const UINT32 margins = box->uiLeftMargin + box->uiRightMargin;

	iHeight = box->uiTopMargin + box->uiBottomMargin;

	for ( iCurrString = 0; iCurrString < MAX_POPUP_BOX_STRING_COUNT; iCurrString++ )
	{
		if ( box->Text[0][iCurrString] != NULL )
		{

			if ( box->Text[3][iCurrString] != NULL  && wcscmp( box->Text[3][iCurrString]->pString, L"") != 0)
			{
				iSecondColumnLength = StringPixLength( box->Text[3][iCurrString]->pString, box->Text[3][iCurrString]->uiFont );
				if ( 3*box->uiSecondColumnCurrentOffset + iSecondColumnLength + margins > iWidth )
				{
					iWidth = 3*box->uiSecondColumnCurrentOffset + iSecondColumnLength + margins;
				}
			}
			else if ( box->Text[2][iCurrString] != NULL && wcscmp( box->Text[2][iCurrString]->pString, L"" ) != 0 )
			{
				iSecondColumnLength = StringPixLength( box->Text[2][iCurrString]->pString, box->Text[2][iCurrString]->uiFont );
				if ( 2*box->uiSecondColumnCurrentOffset + iSecondColumnLength + margins > iWidth )
				{
					iWidth = 2*box->uiSecondColumnCurrentOffset + iSecondColumnLength + margins;
				}
			}
			else if ( box->Text[1][iCurrString] != NULL && wcscmp( box->Text[1][iCurrString]->pString, L"" ) != 0 )
			{
				iSecondColumnLength = StringPixLength( box->Text[1][iCurrString]->pString, box->Text[1][iCurrString]->uiFont );
				if ( box->uiSecondColumnCurrentOffset + iSecondColumnLength + margins > iWidth )
				{
					iWidth = box->uiSecondColumnCurrentOffset + iSecondColumnLength + margins;
				}
			}


			if ( box->pSecondColumnString[iCurrString] != NULL )
			{
				iSecondColumnLength = StringPixLength( box->pSecondColumnString[iCurrString]->pString, box->pSecondColumnString[iCurrString]->uiFont );
				if ( box->uiSecondColumnCurrentOffset + iSecondColumnLength + margins > iWidth )
				{
					iWidth = box->uiSecondColumnCurrentOffset + iSecondColumnLength + margins;
				}
			}

			INT32 iFirstColumnLength = StringPixLength( box->Text[0][iCurrString]->pString, box->Text[0][iCurrString]->uiFont ) + margins;
			if ( iFirstColumnLength > iWidth )
				iWidth = iFirstColumnLength;

			//vertical
			iHeight += GetFontHeight( box->Text[0][iCurrString]->uiFont ) + box->uiLineSpace;
		}
		else
		{
			// doesn't support gaps in text array...
			break;
		}
	}

	// Flugente we shouldn't have added more popup options than we can display anyway, but I have no idea where to stop that, so at least we can fix this
	box->Dimensions.iBottom = min( iHeight, SCREEN_HEIGHT - box->Position.iY );
	box->Dimensions.iRight = min( iWidth, SCREEN_WIDTH - box->Position.iX );

	// Constrain popup box height to background graphics max height. Otherwise we get blue graphics glitches
	const auto popupBoxHeight = box->Dimensions.iBottom - box->Dimensions.iTop;
	if ( popupBoxHeight > 480 )
	{
		box->Dimensions.iBottom = box->Dimensions.iTop + 480;
	}
}


BOOLEAN IsBoxShown( UINT32 uiHandle )
{
	if ( ( uiHandle < 0 ) || ( uiHandle >= MAX_POPUP_BOX_COUNT ) )
		return(FALSE);

	if( PopUpBoxList[ uiHandle ] == NULL )
	{
		return ( FALSE );
	}

	return( PopUpBoxList[ uiHandle ]->fShowBox );
}


void MarkAllBoxesAsAltered( void )
{
	INT32 iCounter = 0;

	// mark all boxes as altered
	for( iCounter = 0; iCounter < MAX_POPUP_BOX_COUNT; iCounter++ )
	{
		ForceUpDateOfBox( iCounter );
	}

	return;
}


void HideAllBoxes( void )
{
	INT32 iCounter = 0;

	// hide all the boxes that are shown
	for(iCounter=0; iCounter < MAX_POPUP_BOX_COUNT; iCounter++)
	{
		HideBox( iCounter );
	}
}


void RemoveCurrentBoxPrimaryText( INT32 hStringHandle )
{
	if ( ( guiCurrentBox < 0 ) || ( guiCurrentBox >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( hStringHandle < MAX_POPUP_BOX_STRING_COUNT );

	// remove & release primary text
	if(PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle] != NULL)
	{
		if ( PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]->pString )
		{
			MemFree(PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]->pString);
		}

		MemFree(PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]);
		PopUpBoxList[guiCurrentBox]->Text[0][hStringHandle]=NULL;
	}
}

void RemoveCurrentBoxSecondaryText( INT32 hStringHandle )
{
	if ( ( guiCurrentBox < 0 ) || ( guiCurrentBox >= MAX_POPUP_BOX_COUNT ) )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( hStringHandle < MAX_POPUP_BOX_STRING_COUNT );

	// remove & release secondary strings
	if(PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle] != NULL)
	{
		if ( PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle]->pString )
		{
			MemFree(PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle]->pString);
		}

		MemFree(PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle]);
		PopUpBoxList[guiCurrentBox]->pSecondColumnString[hStringHandle]=NULL;
	}
}

void RemoveCurrentBoxText( INT32 hStringHandle, UINT8 column )
{
	if ( (guiCurrentBox < 0) || (guiCurrentBox >= MAX_POPUP_BOX_COUNT) || column >= MAX_POPUP_BOX_COLUMNS )
		return;

	Assert( PopUpBoxList[guiCurrentBox] != NULL );
	Assert( hStringHandle < MAX_POPUP_BOX_STRING_COUNT );

	// remove & release text
	auto textColumn = PopUpBoxList[guiCurrentBox]->Text[column];

	if ( textColumn[hStringHandle] != NULL )
	{
		if ( textColumn[hStringHandle]->pString )
		{
			MemFree( textColumn[hStringHandle]->pString );
		}

		MemFree( textColumn[hStringHandle] );
		textColumn[hStringHandle] = NULL;
	}
}

UINT32 GetBoxSecondColumnCurrentOffset( INT32 hBoxHandle )
{
	if ( (hBoxHandle < 0) || (hBoxHandle >= MAX_POPUP_BOX_COUNT) )
		return 0;

	return PopUpBoxList[hBoxHandle]->uiSecondColumnCurrentOffset;
}
