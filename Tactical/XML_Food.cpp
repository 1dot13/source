#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "Food.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	FOODTYPE		curFood;
	FOODTYPE *		curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef foodParseData;

static void XMLCALL
foodStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	foodParseData * pData = (foodParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "FOODSLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(FOODTYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "FOOD") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curFood,0,sizeof(FOODTYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "szName") == 0 ||
				strcmp(name, "bFoodPoints") == 0 ||
				strcmp(name, "bDrinkPoints") == 0 ||
				strcmp(name, "ubPortionSize") == 0 ||
				strcmp(name, "bMoraleMod") == 0 ||
				strcmp(name, "usDecayRate") == 0 )) 
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
foodCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	foodParseData * pData = (foodParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
foodEndElementHandle(void *userData, const XML_Char *name)
{
	foodParseData * pData = (foodParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "FOODSLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "FOOD") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			// we do NOT want to read the first entry -> move stuff by 1
			if(pData->curFood.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curFood.uiIndex] = pData->curFood; //write the food into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curFood.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "szName") == 0)
		{
			pData->curElement = ELEMENT;
			// not needed, but it's there for informational purposes
		}
		else if(strcmp(name, "bFoodPoints") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curFood.bFoodPoints	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "bDrinkPoints") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curFood.bDrinkPoints = (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubPortionSize") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curFood.ubPortionSize	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bMoraleMod") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curFood.bMoraleMod	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "usDecayRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curFood.usDecayRate	= (FLOAT) atof(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInFoodStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	foodParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Food.xml" );

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


	XML_SetElementHandler(parser, foodStartElementHandle, foodEndElementHandle);
	XML_SetCharacterDataHandler(parser, foodCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = Food;
	pData.maxArraySize = FOOD_TYPE_MAX;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Food.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteFoodStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writefoodsstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Food out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<FOODSLIST>\r\n");
		for(cnt = 0; cnt < FOOD_TYPE_MAX; ++cnt)
		{
			FilePrintf(hFile,"\t<FOOD>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",				cnt );
			FilePrintf(hFile,"\t\t<bFoodPoints>%d</bFoodPoints>\r\n",		Food[cnt].bFoodPoints	);
			FilePrintf(hFile,"\t\t<bDrinkPoints>%d</bDrinkPoints>\r\n",		Food[cnt].bDrinkPoints	);
			FilePrintf(hFile,"\t\t<ubPortionSize>%d</ubPortionSize>\r\n",	Food[cnt].ubPortionSize	);
			FilePrintf(hFile,"\t\t<bMoraleMod>%d</bMoraleMod>\r\n",			Food[cnt].bMoraleMod	);
			FilePrintf(hFile,"\t\t<usDecayRate>%4.2f</usDecayRate>\r\n",	Food[cnt].usDecayRate	);

			FilePrintf(hFile,"\t</FOOD>\r\n");
		}
		FilePrintf(hFile,"</FOODSLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
