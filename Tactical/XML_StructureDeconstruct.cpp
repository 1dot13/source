#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "Handle Items.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	STRUCTURE_DECONSTRUCT		curFood;
	STRUCTURE_DECONSTRUCT *		curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef structuredeconstructParseData;

UINT16 structuredeconstructcnt = 0;

static void XMLCALL
structuredeconstructStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	structuredeconstructParseData * pData = (structuredeconstructParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "STRUCTURESLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(STRUCTURE_DECONSTRUCT)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "STRUCTURE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curFood,0,sizeof(STRUCTURE_DECONSTRUCT));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "usDeconstructItem") == 0 ||
				strcmp( name, "usItemToCreate" ) == 0 ||
				strcmp( name, "usCreatedItemStatus" ) == 0 ||
				strcmp(name, "szTileSetName") == 0 ||
				strcmp(name, "allowedtile") == 0 )) 
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
structuredeconstructCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	structuredeconstructParseData * pData = (structuredeconstructParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
structuredeconstructEndElementHandle(void *userData, const XML_Char *name)
{
	static std::vector<UINT8> statictilevector;

	structuredeconstructParseData * pData = (structuredeconstructParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "STRUCTURESLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "STRUCTURE") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(structuredeconstructcnt < pData->maxArraySize)
			{
				// for whatever reasons the game crashes in VS2008 Release builds when copying over the tilevector
				// this seems odd, as this works just fine in VS2010 and VS2013, and also works in VS205 debug builds
				// for now, copy over the content by hand
				pData->curArray[structuredeconstructcnt].usDeconstructItem = pData->curFood.usDeconstructItem;
				pData->curArray[structuredeconstructcnt].usItemToCreate= pData->curFood.usItemToCreate;
				pData->curArray[structuredeconstructcnt].usCreatedItemStatus = pData->curFood.usCreatedItemStatus;
				strncpy(pData->curArray[structuredeconstructcnt].szTileSetName, pData->curFood.szTileSetName, 20);
				pData->curArray[structuredeconstructcnt].tilevector = statictilevector;

				statictilevector.clear();
			}

			++structuredeconstructcnt;
		}
		else if ( strcmp( name, "usDeconstructItem" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curFood.usDeconstructItem = (UINT16)atol( pData->szCharData );
		}
		else if(strcmp(name, "usItemToCreate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curFood.usItemToCreate	= (UINT16) atol(pData->szCharData);
		}
		else if ( strcmp( name, "usCreatedItemStatus" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curFood.usCreatedItemStatus = (UINT8)atol( pData->szCharData );
		}
		else if(strcmp(name, "szTileSetName") == 0)
		{
			pData->curElement = ELEMENT;

			strncpy(pData->curFood.szTileSetName, pData->szCharData, 20);
		}
		else if(strcmp(name, "allowedtile") == 0)
		{
			pData->curElement = ELEMENT;
			statictilevector.push_back( (UINT8) atol(pData->szCharData) );
		}
		
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInStructureDeconstructStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	structuredeconstructParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading StructureDeconstruct.xml" );

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


	XML_SetElementHandler(parser, structuredeconstructStartElementHandle, structuredeconstructEndElementHandle);
	XML_SetCharacterDataHandler(parser, structuredeconstructCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = gStructureDeconstruct;
	pData.maxArraySize = STRUCTURE_DECONSTRUCT_MAX;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in StructureDeconstruct.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteStructureDeconstructStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writefoodsstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\StructureDeconstruct out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<STRUCTURESLIST>\r\n");
		for(cnt = 0; cnt < STRUCTURE_DECONSTRUCT_MAX; ++cnt)
		{
			FilePrintf(hFile,"\t<STRUCTURE>\r\n");

			FilePrintf(hFile,"\t\t<usDeconstructItem>%d</usDeconstructItem>\r\n",	gStructureDeconstruct[cnt].usDeconstructItem );
			FilePrintf(hFile,"\t\t<usItemToCreate>%d</usItemToCreate>\r\n",			gStructureDeconstruct[cnt].usItemToCreate );
			FilePrintf(hFile,"\t\t<usCreatedItemStatus>%d</usCreatedItemStatus>\r\n", gStructureDeconstruct[cnt].usCreatedItemStatus );
			FilePrintf(hFile,"\t\t<szTileSetName>%s</szTileSetName>\r\n",			gStructureDeconstruct[cnt].szTileSetName	);

			FilePrintf(hFile,"\t</STRUCTURE>\r\n");
		}
		FilePrintf(hFile,"</STRUCTURESLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
