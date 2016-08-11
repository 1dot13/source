/**
* @file
* @author Flugente (bears-pit.com)
*/

#ifdef PRECOMPILEDHEADERS
#include "Tactical All.h"
#else
#include "sgp.h"
#include "Debug Control.h"
#include "expat.h"
#include "gamesettings.h"
#include "XML.h"
#include "FileMan.h"
#include "Handle Items.h"
#endif

INTERACTIVE_STRUCTURE gInteractiveStructure[INTERACTIVE_STRUCTURE_MAX];
UINT32 gMaxInteractiveStructureRead = 0;

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH + 1];
	INTERACTIVE_STRUCTURE*		curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef interactiveactionsParseData;

static void XMLCALL
interactiveactionsStartElementHandle( void *userData, const XML_Char *name, const XML_Char **atts )
{
	interactiveactionsParseData * pData = (interactiveactionsParseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth ) //are we reading this element?
	{
		if ( strcmp( name, "INTERACTIVEACTIONS" ) == 0 && pData->curElement == ELEMENT_NONE )
		{
			pData->curElement = ELEMENT_LIST;

			memset( pData->curArray, 0, sizeof(INTERACTIVE_STRUCTURE)*pData->maxArraySize );

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "ACTION" ) == 0 && pData->curElement == ELEMENT_LIST )
		{
			pData->curElement = ELEMENT;

			pData->curArray[pData->curIndex].reset( );

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( pData->curElement == ELEMENT &&
				  (strcmp( name, "SectorGrid" ) == 0 ||
				  strcmp( name, "sectorlevel" ) == 0 ||
				  strcmp( name, "szTileSetName" ) == 0 ||
				  strcmp( name, "usTileIndex" ) == 0 ||
				  strcmp( name, "sStructureGridno" ) == 0 ||
				  strcmp( name, "sLevel" ) == 0 ||
				  strcmp( name, "sActionType" ) == 0 ||
				  strcmp( name, "difficulty" ) == 0 ||
				  strcmp( name, "luaactionid" ) == 0
				  ) )
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
interactiveactionsDataHandle( void *userData, const XML_Char *str, int len )
{
	interactiveactionsParseData * pData = (interactiveactionsParseData *)userData;

	if ( (pData->currentDepth <= pData->maxReadDepth) &&
		 (strlen( pData->szCharData ) < MAX_CHAR_DATA_LENGTH)
		 )
	{
		strncat( pData->szCharData, str, __min( (unsigned int)len, MAX_CHAR_DATA_LENGTH - strlen( pData->szCharData ) ) );
	}
}

static void XMLCALL
interactiveactionsEndElementHandle( void *userData, const XML_Char *name )
{
	static std::vector<UINT16> tileindexvector;
	static std::vector<INT32> gridnovector;

	interactiveactionsParseData * pData = (interactiveactionsParseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth ) //we're at the end of an element that we've been reading
	{
		if ( strcmp( name, "INTERACTIVEACTIONS" ) == 0 )
		{
			pData->curElement = ELEMENT_NONE;

			gMaxInteractiveStructureRead = pData->curIndex;
		}
		else if ( strcmp( name, "ACTION" ) == 0 )
		{
			pData->curElement = ELEMENT_LIST;
			
			if ( pData->curIndex < pData->maxArraySize )
			{
				// for whatever reasons the game crashes in VS2008 Release builds when copying over the tilevector
				// this seems odd, as this works just fine in VS2010 and VS2013, and also works in VS205 debug builds
				// for now, copy over the content by hand
				// check if the vector is empty because assigning an empty vector will crash VS2010 debug builds!
				if ( !tileindexvector.empty() )
				{
					pData->curArray[pData->curIndex].tileindexvector = tileindexvector;
					tileindexvector.clear( );
				}

				if ( !gridnovector.empty() )
				{
					pData->curArray[pData->curIndex].gridnovector = gridnovector;
					gridnovector.clear();
				}
			}

			pData->curIndex++;
		}

		else if ( strcmp( name, "SectorGrid" ) == 0 )
		{
			UINT8	x, y;
			pData->curElement = ELEMENT;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol( &pData->szCharData[1] );
			if ( x > 0 && x <= 16 && y > 0 && y <= 16 )
			{
				pData->curArray[pData->curIndex].sector = SECTOR( x, y );
			}
		}
		else if ( strcmp( name, "sectorlevel" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curArray[pData->curIndex].sectorlevel = min( 3, max( -1, (INT8)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "szTileSetName" ) == 0 )
		{
			pData->curElement = ELEMENT;

			strncpy( pData->curArray[pData->curIndex].szTileSetName, pData->szCharData, 20 );
		}
		else if ( strcmp( name, "usTileIndex" ) == 0 )
		{
			pData->curElement = ELEMENT;
			tileindexvector.push_back( (UINT16)atol( pData->szCharData ) );
		}
		else if ( strcmp( name, "sStructureGridno" ) == 0 )
		{
			pData->curElement = ELEMENT;
			gridnovector.push_back( (INT32)atol( pData->szCharData ) );
		}
		else if ( strcmp( name, "sLevel" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curArray[pData->curIndex].sLevel = min( 1, max( 0, (INT8)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "sActionType" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curArray[pData->curIndex].sActionType = (UINT16)atol( pData->szCharData );
		}
		else if ( strcmp( name, "difficulty" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curArray[pData->curIndex].difficulty = (INT32)atol( pData->szCharData );
		}
		else if ( strcmp( name, "luaactionid" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curArray[pData->curIndex].luaactionid = (INT32)atol( pData->szCharData );
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}

BOOLEAN ReadInInteractiveActionsStats( STR fileName )
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate( NULL );

	interactiveactionsParseData pData;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Loading InteractiveActions.xml" );

	// Open merges file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return(FALSE);

	uiFSize = FileGetSize( hFile );
	lpcBuffer = (CHAR8 *)MemAlloc( uiFSize + 1 );

	//Read in block
	if ( !FileRead( hFile, lpcBuffer, uiFSize, &uiBytesRead ) )
	{
		MemFree( lpcBuffer );
		return(FALSE);
	}

	lpcBuffer[uiFSize] = 0; //add a null terminator

	FileClose( hFile );
	
	XML_SetElementHandler( parser, interactiveactionsStartElementHandle, interactiveactionsEndElementHandle );
	XML_SetCharacterDataHandler( parser, interactiveactionsDataHandle );
	
	memset( &pData, 0, sizeof(pData) );
	pData.maxArraySize = INTERACTIVE_STRUCTURE_MAX;
	pData.curIndex = 0;
	pData.curArray = gInteractiveStructure;

	XML_SetUserData( parser, &pData );

	if ( !XML_Parse( parser, lpcBuffer, uiFSize, TRUE ) )
	{
		CHAR8 errorBuf[511];

		sprintf( errorBuf, "XML Parser Error in InteractiveActions.xml: %s at line %d", XML_ErrorString( XML_GetErrorCode( parser ) ), XML_GetCurrentLineNumber( parser ) );
		LiveMessage( errorBuf );

		MemFree( lpcBuffer );
		return FALSE;
	}

	MemFree( lpcBuffer );

	XML_ParserFree( parser );

	return(TRUE);
}
