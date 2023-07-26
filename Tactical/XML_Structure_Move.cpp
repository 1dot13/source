#include "sgp.h"
#include "overhead.h"
#include "Handle Items.h"
#include "Debug Control.h"
#include "expat.h"
#include "XML.h"

STRUCTURE_MOVEPOSSIBLE gStructureMovePossible[STRUCTURE_MOVEPOSSIBLE_MAX];

struct
{
	PARSE_STAGE	curElement;

	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH + 1];
	STRUCTURE_MOVEPOSSIBLE*		curArray;
	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef structuremoveParseData;

static void XMLCALL
structuredemoveStartElementHandle( void *userData, const XML_Char *name, const XML_Char **atts )
{
	structuremoveParseData * pData = (structuremoveParseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth ) //are we reading this element?
	{
		if ( strcmp( name, "STRUCTURESLIST" ) == 0 && pData->curElement == ELEMENT_NONE )
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "STRUCTURE" ) == 0 && pData->curElement == ELEMENT_LIST )
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( pData->curElement == ELEMENT &&
			( strcmp( name, "szTileSetDisplayName" ) == 0 ||
				strcmp( name, "szTileSetName" ) == 0 ||
				strcmp( name, "allowedtile" ) == 0 ) )
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
structuredemoveCharacterDataHandle( void *userData, const XML_Char *str, int len )
{
	structuremoveParseData * pData = (structuremoveParseData *)userData;

	if ( ( pData->currentDepth <= pData->maxReadDepth ) &&
		( strlen( pData->szCharData ) < MAX_CHAR_DATA_LENGTH )
		) {
		strncat( pData->szCharData, str, __min( (unsigned int)len, MAX_CHAR_DATA_LENGTH - strlen( pData->szCharData ) ) );
	}
}


static void XMLCALL
structuredemoveEndElementHandle( void *userData, const XML_Char *name )
{
	static std::vector<UINT8> statictilevector;

	structuremoveParseData * pData = (structuremoveParseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth ) //we're at the end of an element that we've been reading
	{
		if ( strcmp( name, "STRUCTURESLIST" ) == 0 )
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if ( strcmp( name, "STRUCTURE" ) == 0 )
		{
			pData->curElement = ELEMENT_LIST;

			if ( pData->curIndex < pData->maxArraySize )
			{
				// for whatever reasons the game crashes in VS2008 Release builds when copying over the tilevector
				// this seems odd, as this works just fine in VS2010 and VS2013, and also works in VS205 debug builds
				// for now, copy over the content by hand
				// check if the vector is empty because assigning an empty vector will crash VS2010 debug builds!
				if ( !statictilevector.empty() )
				{
					pData->curArray[pData->curIndex].tilevector = statictilevector;
					statictilevector.clear();
				}
			}

			pData->curIndex++;
		}
		else if ( strcmp( name, "szTileSetDisplayName" ) == 0 )
		{
			pData->curElement = ELEMENT;

			strncpy( pData->curArray[pData->curIndex].szTileSetDisplayName, pData->szCharData, 20 );
		}
		else if ( strcmp( name, "szTileSetName" ) == 0 )
		{
			pData->curElement = ELEMENT;

			strncpy( pData->curArray[pData->curIndex].szTileSetName, pData->szCharData, 20 );
		}
		else if ( strcmp( name, "allowedtile" ) == 0 )
		{
			pData->curElement = ELEMENT;
			statictilevector.push_back( (UINT8)atol( pData->szCharData ) );
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInStructureMoveStats( STR fileName )
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate( NULL );

	structuremoveParseData pData;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Loading StructureMove.xml" );

	// Open foods file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( FALSE );

	uiFSize = FileGetSize( hFile );
	lpcBuffer = (CHAR8 *)MemAlloc( uiFSize + 1 );

	//Read in block
	if ( !FileRead( hFile, lpcBuffer, uiFSize, &uiBytesRead ) )
	{
		MemFree( lpcBuffer );
		return( FALSE );
	}

	lpcBuffer[uiFSize] = 0; //add a null terminator

	FileClose( hFile );

	XML_SetElementHandler( parser, structuredemoveStartElementHandle, structuredemoveEndElementHandle );
	XML_SetCharacterDataHandler( parser, structuredemoveCharacterDataHandle );

	memset( &pData, 0, sizeof( pData ) );
	pData.curArray = gStructureMovePossible;
	pData.curIndex = 0;
	pData.maxArraySize = STRUCTURE_MOVEPOSSIBLE_MAX;

	XML_SetUserData( parser, &pData );

	if ( !XML_Parse( parser, lpcBuffer, uiFSize, TRUE ) )
	{
		CHAR8 errorBuf[511];

		sprintf( errorBuf, "XML Parser Error in StructureMove.xml: %s at line %d", XML_ErrorString( XML_GetErrorCode( parser ) ), XML_GetCurrentLineNumber( parser ) );
		LiveMessage( errorBuf );

		MemFree( lpcBuffer );
		return FALSE;
	}

	MemFree( lpcBuffer );

	XML_ParserFree( parser );

	return( TRUE );
}

BOOLEAN WriteStructureMoveStats()
{
	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	HWFILE hFile = FileOpen( "TABLEDATA\\StructureMove out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		FilePrintf( hFile, "<STRUCTURESLIST>\r\n" );
		for ( UINT32 cnt = 0; cnt < STRUCTURE_MOVEPOSSIBLE_MAX; ++cnt )
		{
			FilePrintf( hFile, "\t<STRUCTURE>\r\n" );

			FilePrintf( hFile, "\t\t<szTileSetDisplayName>%s</szTileSetDisplayName>\r\n", gStructureMovePossible[cnt].szTileSetDisplayName );
			FilePrintf( hFile, "\t\t<szTileSetName>%s</szTileSetName>\r\n", gStructureMovePossible[cnt].szTileSetName );

			FilePrintf( hFile, "\t</STRUCTURE>\r\n" );
		}
		FilePrintf( hFile, "</STRUCTURESLIST>\r\n" );
	}
	FileClose( hFile );

	return( TRUE );
}
