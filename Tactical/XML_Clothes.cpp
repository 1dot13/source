#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	CLOTHES_STRUCT	curClothes;
	CLOTHES_STRUCT*	curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef clothesParseData;

// Array to hold clothes data
CLOTHES_STRUCT Clothes[CLOTHES_MAX];

static void XMLCALL
clothesStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	clothesParseData * pData = (clothesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "CLOTHESLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(CLOTHES_STRUCT)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "CLOTHES") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curClothes,0,sizeof(CLOTHES_STRUCT));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "szName") == 0 ||
				strcmp(name, "Vest") == 0 ||
				strcmp(name, "Pants") == 0 )) 
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
clothesCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	clothesParseData * pData = (clothesParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
clothesEndElementHandle(void *userData, const XML_Char *name)
{
	clothesParseData * pData = (clothesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "CLOTHESLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "CLOTHES") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			// we do NOT want to read the first entry -> move stuff by 1
			if(pData->curClothes.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curClothes.uiIndex] = pData->curClothes; //write the clothes into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curClothes.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "szName") == 0)
		{
			pData->curElement = ELEMENT;
			// not needed, but it's there for informational purposes
		}
		else if(strcmp(name, "Vest") == 0 )
		{
			pData->curElement = ELEMENT;

			strcpy(pData->curClothes.vest, pData->szCharData);
		}
		else if(strcmp(name, "Pants") == 0 )
		{
			pData->curElement = ELEMENT;

			strcpy(pData->curClothes.pants, pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInClothesStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	clothesParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Clothes.xml" );

	// Open clothess file
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


	XML_SetElementHandler(parser, clothesStartElementHandle, clothesEndElementHandle);
	XML_SetCharacterDataHandler(parser, clothesCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = Clothes;
	pData.maxArraySize = CLOTHES_MAX;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Clothes.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteClothesStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeclothessstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Clothes out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<CLOTHESLIST>\r\n");
		for(cnt = 0; cnt < CLOTHES_MAX; ++cnt)
		{
			FilePrintf(hFile,"\t<CLOTHES>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",				cnt );
			FilePrintf(hFile,"\t\t<vest>%d</vest>\r\n",						Clothes[cnt].vest	);
			FilePrintf(hFile,"\t\t<pants>%d</pants>\r\n",					Clothes[cnt].pants	);

			FilePrintf(hFile,"\t</CLOTHES>\r\n");
		}
		FilePrintf(hFile,"</CLOTHESLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
