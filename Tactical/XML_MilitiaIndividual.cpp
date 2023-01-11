/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "sgp.h"
#include "Debug Control.h"
#include "expat.h"
#include "XML.h"
#include "MilitiaIndividual.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH + 1];
	MilitiaOriginData	curBackground;
	MilitiaOriginData *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
	CHAR16 gzBackground[MAX_ENEMY_NAMES_CHARS];
}
typedef parseData;

BOOLEAN localizedTextOnly_MI;

static void XMLCALL
militiaindividualStartElementHandle( void *userData, const XML_Char *name, const XML_Char **atts )
{
	parseData * pData = (parseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth ) //are we reading this element?
	{
		if ( strcmp( name, "MILITIAORIGINDATA" ) == 0 && pData->curElement == ELEMENT_NONE )
		{
			pData->curElement = ELEMENT_LIST;

			for (UINT32 i = 0; i < pData->maxArraySize; ++i) {
				pData->curArray[i] = MilitiaOriginData{};
			}

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "ORIGINDATA" ) == 0 && pData->curElement == ELEMENT_LIST )
		{
			pData->curElement = ELEMENT;

			if ( !localizedTextOnly_MI )
				pData->curBackground = MilitiaOriginData{};

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( pData->curElement == ELEMENT &&
				  (strcmp( name, "male_forename" ) == 0 ||
				  strcmp( name, "male_surname" ) == 0 ||
				  strcmp( name, "female_forename" ) == 0 ||
				  strcmp( name, "female_surname" ) == 0 ||
				  strcmp( name, "chance_bodytype_REGMALE" ) == 0 ||
				  strcmp( name, "chance_bodytype_BIGMALE" ) == 0 ||
				  strcmp( name, "chance_bodytype_STOCKYMALE" ) == 0 ||
				  strcmp( name, "chance_bodytype_REGFEMALE" ) == 0 ||
				  strcmp( name, "chance_skin_PINKSKIN" ) == 0 ||
				  strcmp( name, "chance_skin_TANSKIN" ) == 0 ||
				  strcmp( name, "chance_skin_DARKSKIN" ) == 0 ||
				  strcmp( name, "chance_skin_BLACKSKIN" ) == 0 ||
				  strcmp( name, "dailycost_green" ) == 0 ||
				  strcmp( name, "dailycost_regular" ) == 0 ||
				  strcmp( name, "dailycost_elite" ) == 0) )
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
militiaindividualCharacterDataHandle( void *userData, const XML_Char *str, int len )
{
	parseData * pData = (parseData *)userData;

	if ( (pData->currentDepth <= pData->maxReadDepth) &&
		 (strlen( pData->szCharData ) < MAX_CHAR_DATA_LENGTH)
		 ){
		strncat( pData->szCharData, str, __min( (unsigned int)len, MAX_CHAR_DATA_LENGTH - strlen( pData->szCharData ) ) );
	}
}

static void XMLCALL
militiaindividualEndElementHandle( void *userData, const XML_Char *name )
{
	parseData * pData = (parseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth )
	{
		if ( strcmp( name, "MILITIAORIGINDATA" ) == 0 )
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if ( strcmp( name, "ORIGINDATA" ) == 0 )
		{
			pData->curElement = ELEMENT_LIST;

			if ( pData->curIndex < pData->maxArraySize )
			{
				pData->curArray[pData->curIndex] = pData->curBackground;
			}

			++pData->curIndex;
		}
		else if ( strcmp( name, "male_forename" ) == 0 )
		{
			pData->curElement = ELEMENT;
			
			CHAR16 bla[30];

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, bla, sizeof(bla) / sizeof(bla[0]) );
			bla[sizeof(bla) / sizeof(bla[0]) - 1] = '\0';

			pData->curBackground.szMale_Forename.push_back( bla );
		}
		else if ( strcmp( name, "male_surname" ) == 0 )
		{
			pData->curElement = ELEMENT;

			CHAR16 bla[30];

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, bla, sizeof(bla) / sizeof(bla[0]) );
			bla[sizeof(bla) / sizeof(bla[0]) - 1] = '\0';

			pData->curBackground.szMale_Surname.push_back( bla );
		}
		else if ( strcmp( name, "female_forename" ) == 0 )
		{
			pData->curElement = ELEMENT;

			CHAR16 bla[30];

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, bla, sizeof(bla) / sizeof(bla[0]) );
			bla[sizeof(bla) / sizeof(bla[0]) - 1] = '\0';

			pData->curBackground.szFemale_Forename.push_back( bla );
		}
		else if ( strcmp( name, "female_surname" ) == 0 )
		{
			pData->curElement = ELEMENT;

			CHAR16 bla[30];

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, bla, sizeof(bla) / sizeof(bla[0]) );
			bla[sizeof(bla) / sizeof(bla[0]) - 1] = '\0';

			pData->curBackground.szFemale_Surname.push_back( bla );
		}
		else if ( strcmp( name, "chance_bodytype_REGMALE" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.chance_bodytype[REGMALE] = min( 100, max( 0, (UINT8)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "chance_bodytype_BIGMALE" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.chance_bodytype[BIGMALE] = min( 100, max( 0, (UINT8)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "chance_bodytype_STOCKYMALE" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.chance_bodytype[STOCKYMALE] = min( 100, max( 0, (UINT8)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "chance_bodytype_REGFEMALE" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.chance_bodytype[REGFEMALE] = min( 100, max( 0, (UINT8)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "chance_skin_PINKSKIN" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.chance_skin[PINKSKIN] = min( 100, max( 0, (UINT8)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "chance_skin_TANSKIN" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.chance_skin[TANSKIN] = min( 100, max( 0, (UINT8)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "chance_skin_DARKSKIN" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.chance_skin[DARKSKIN] = min( 100, max( 0, (UINT8)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "chance_skin_BLACKSKIN" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.chance_skin[BLACKSKIN] = min( 100, max( 0, (UINT8)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "dailycost_green" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.dailycost[GREEN_MILITIA] = (UINT16)atol( pData->szCharData );
		}
		else if ( strcmp( name, "dailycost_regular" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.dailycost[REGULAR_MILITIA] = (UINT16)atol( pData->szCharData );
		}
		else if ( strcmp( name, "dailycost_elite" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.dailycost[ELITE_MILITIA] = (UINT16)atol( pData->szCharData );
		}

		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInMilitiaIndividual( STR fileName, BOOLEAN localizedVersion )
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate( NULL );

	parseData pData;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Loading MilitiaIndividual.xml" );

	localizedTextOnly_MI = localizedVersion;

	// Open file
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

	XML_SetElementHandler( parser, militiaindividualStartElementHandle, militiaindividualEndElementHandle );
	XML_SetCharacterDataHandler( parser, militiaindividualCharacterDataHandle );

	memset( &pData, 0, sizeof(pData) );
	pData.curArray = gMilitiaOriginData;
	pData.maxArraySize = MO_MAX;

	XML_SetUserData( parser, &pData );

	if ( !XML_Parse( parser, lpcBuffer, uiFSize, TRUE ) )
	{
		CHAR8 errorBuf[511];

		sprintf( errorBuf, "XML Parser Error in MilitiaIndividual.xml: %s at line %d", XML_ErrorString( XML_GetErrorCode( parser ) ), XML_GetCurrentLineNumber( parser ) );
		LiveMessage( errorBuf );

		MemFree( lpcBuffer );
		return FALSE;
	}

	MemFree( lpcBuffer );

	XML_ParserFree( parser );

	return(TRUE);
}


BOOLEAN WriteMilitiaIndividual( STR fileName )
{
	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	HWFILE hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return(FALSE);

	{
		FilePrintf( hFile, "<MILITIAORIGINDATA>\r\n" );
		for ( UINT32 cnt = 0; cnt < NUM_BACKGROUND; ++cnt )
		{
			FilePrintf( hFile, "\t<ORIGINDATA>\r\n" );
			FilePrintf( hFile, "\t\t<Origin>%d</Origin>\r\n", cnt );

		}
		FilePrintf( hFile, "</ORIGINDATA>\r\n" );
	}
	FileClose( hFile );

	return(TRUE);
}
