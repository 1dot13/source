	#include "sgp.h"
	#include "overhead.h"
	#include "Handle Items.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	STRUCTURE_CONSTRUCT*	curArray;
	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef structureconstructParseData;

static void XMLCALL
structureconstructStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	structureconstructParseData * pData = (structureconstructParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "STRUCTURESLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "STRUCTURE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "usCreationItem") == 0 ||
				strcmp(name, "usItemStatusLoss") == 0 ||
				strcmp(name, "szTileSetDisplayName") == 0 ||
				strcmp(name, "szTileSetName" ) == 0 ||
				strcmp(name, "dCreationCost" ) == 0 ||
				strcmp(name, "fFortifyAdjacentAdjustment" ) == 0 ||
				strcmp(name, "northfacingtile") == 0 ||
				strcmp(name, "southfacingtile" ) == 0 ||
				strcmp(name, "eastfacingtile" ) == 0 ||
				strcmp(name, "westfacingtile") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
structureconstructCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	structureconstructParseData * pData = (structureconstructParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
structureconstructEndElementHandle(void *userData, const XML_Char *name)
{
	static std::vector<UINT8> staticnorthtilevector, staticsouthtilevector, staticeasttilevector, staticwesttilevector;

	structureconstructParseData * pData = (structureconstructParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "STRUCTURESLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "STRUCTURE") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			// we do NOT want to read the first entry -> move stuff by 1
			if ( pData->curIndex < pData->maxArraySize )
			{
				// for whatever reasons the game crashes in VS2008 Release builds when copying over the tilevector
				// this seems odd, as this works just fine in VS2010 and VS2013, and also works in VS205 debug builds
				// for now, copy over the content by hand
				// check if the vector is empty because assigning an empty vector will crash VS2010 debug builds!
				if ( !staticnorthtilevector.empty( ) )
				{
					pData->curArray[pData->curIndex].northtilevector = staticnorthtilevector;
					staticnorthtilevector.clear( );
				}

				if ( !staticsouthtilevector.empty( ) )
				{
					pData->curArray[pData->curIndex].southtilevector = staticsouthtilevector;
					staticsouthtilevector.clear( );
				}

				if ( !staticeasttilevector.empty( ) )
				{
					pData->curArray[pData->curIndex].easttilevector = staticeasttilevector;
					staticeasttilevector.clear( );
				}

				if ( !staticwesttilevector.empty( ) )
				{
					pData->curArray[pData->curIndex].westtilevector = staticwesttilevector;
					staticwesttilevector.clear( );
				}
			}

			pData->curIndex++;
		}
		else if(strcmp(name, "usCreationItem") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curArray[pData->curIndex].usCreationItem = (UINT16)atol( pData->szCharData );
		}
		else if(strcmp(name, "usItemStatusLoss") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curArray[pData->curIndex].usItemStatusLoss = (UINT8)atol( pData->szCharData );
		}
		else if(strcmp(name, "szTileSetDisplayName") == 0)
		{
			pData->curElement = ELEMENT;

			strncpy( pData->curArray[pData->curIndex].szTileSetDisplayName, pData->szCharData, 20 );
		}
		else if ( strcmp( name, "szTileSetName" ) == 0 )
		{
			pData->curElement = ELEMENT;

			strncpy( pData->curArray[pData->curIndex].szTileSetName, pData->szCharData, 20 );
		}
		else if ( strcmp( name, "dCreationCost" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curArray[pData->curIndex].dCreationCost = (FLOAT)atof( pData->szCharData );
		}
		else if ( strcmp( name, "fFortifyAdjacentAdjustment" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curArray[pData->curIndex].fFortifyAdjacentAdjustment = (BOOLEAN)atol( pData->szCharData );
		}
		else if(strcmp(name, "northfacingtile") == 0)
		{
			pData->curElement = ELEMENT;
			staticnorthtilevector.push_back( (UINT8) atol(pData->szCharData) );
		}
		else if(strcmp(name, "southfacingtile") == 0)
		{
			pData->curElement = ELEMENT;
			staticsouthtilevector.push_back( (UINT8) atol(pData->szCharData) );
		}
		else if ( strcmp( name, "eastfacingtile" ) == 0 )
		{
			pData->curElement = ELEMENT;
			staticeasttilevector.push_back( (UINT8)atol( pData->szCharData ) );
		}
		else if ( strcmp( name, "westfacingtile" ) == 0 )
		{
			pData->curElement = ELEMENT;
			staticwesttilevector.push_back( (UINT8)atol( pData->szCharData ) );
		}
		
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInStructureConstructStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	structureconstructParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading StructureConstruct.xml" );

	// Open foods file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( FALSE );

	uiFSize = FileGetSize(hFile);
	lpcBuffer = (CHAR8 *) MemAlloc(uiFSize+1);

	//Read in block
	if ( !FileRead( hFile, lpcBuffer, uiFSize, &uiBytesRead ) )
	{
		MemFree(lpcBuffer);
		return( FALSE );
	}

	lpcBuffer[uiFSize] = 0; //add a null terminator

	FileClose( hFile );

	XML_SetElementHandler(parser, structureconstructStartElementHandle, structureconstructEndElementHandle);
	XML_SetCharacterDataHandler(parser, structureconstructCharacterDataHandle);

	memset(&pData,0,sizeof(pData));
	pData.curIndex = 0;
	pData.curArray = gStructureConstruct;
	pData.maxArraySize = STRUCTURE_CONSTRUCT_MAX;

	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in StructureConstruct.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return( TRUE );
}

BOOLEAN WriteStructureConstructStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writefoodsstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\StructureConstruct out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<STRUCTURESLIST>\r\n");
		for(cnt = 0; cnt < STRUCTURE_CONSTRUCT_MAX; ++cnt)
		{
			FilePrintf(hFile,"\t<STRUCTURE>\r\n");

			FilePrintf(hFile,"\t\t<usCreationItem>%d</usCreationItem>\r\n",		gStructureConstruct[cnt].usCreationItem );
			FilePrintf(hFile,"\t\t<usItemStatusLoss>%d</usItemStatusLoss>\r\n",	gStructureConstruct[cnt].usItemStatusLoss );
			FilePrintf(hFile,"\t\t<szTileSetDisplayName>%s</szTileSetDisplayName>\r\n", gStructureConstruct[cnt].szTileSetDisplayName );
			FilePrintf(hFile,"\t\t<szTileSetName>%s</szTileSetName>\r\n", gStructureConstruct[cnt].szTileSetName );
			FilePrintf(hFile,"\t\t<dCreationCost>%d</dCreationCost>\r\n",		gStructureConstruct[cnt].dCreationCost );
			FilePrintf(hFile,"\t\t<fFortifyAdjacentAdjustment>%d</fFortifyAdjacentAdjustment>\r\n", gStructureConstruct[cnt].fFortifyAdjacentAdjustment );
			
			FilePrintf(hFile,"\t</STRUCTURE>\r\n");
		}
		FilePrintf(hFile,"</STRUCTURESLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
