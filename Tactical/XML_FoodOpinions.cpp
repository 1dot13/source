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
	FOODOPINIONS	curFood;
	FOODOPINIONS *  curArray;
	UINT16			usIndex;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef foodopinionParseData;

// haxxor
UINT16 usCurrentItem = 0;

static void XMLCALL
foodopinionStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	foodopinionParseData * pData = (foodopinionParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "FOODOPINIONSLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(FOODTYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "FOODOPINION") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curFood,0,sizeof(FOODTYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uProfile") == 0 )) 
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "PROFILE_OPINION") == 0 ))
		{
			pData->curElement = ELEMENT_SUBLIST;

			++pData->maxReadDepth;
		}
		else if(pData->curElement == ELEMENT_SUBLIST &&
				(strcmp(name, "FoodNumber") == 0 ||
				 strcmp(name, "MoraleMod") == 0 ))
		{
			pData->curElement = ELEMENT_SUBLIST_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
foodopinionCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	foodopinionParseData * pData = (foodopinionParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
foodopinionEndElementHandle(void *userData, const XML_Char *name)
{
	foodopinionParseData * pData = (foodopinionParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "FOODOPINIONSLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "FOODOPINION") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			// we do NOT want to read the first entry -> move stuff by 1
			if ( pData->usIndex < pData->maxArraySize )
			{
				pData->curArray[pData->usIndex] = pData->curFood; //write the food into the table
			}
		}
		else if(strcmp(name, "uProfile") == 0)
		{
			pData->curElement = ELEMENT;
			pData->usIndex = (UINT8)atol( pData->szCharData );
		}

		else if(strcmp(name, "PROFILE_OPINION") == 0 )
		{
			pData->curElement = ELEMENT;
		}
		else if(strcmp(name, "FoodNumber") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			usCurrentItem = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "MoraleMod") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curFood.sFoodOpinion[usCurrentItem] = (INT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInFoodOpinionStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	foodopinionParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading FoodOpinion.xml" );

	// Open file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );

	// if the file does not exist, exit
	// Flugente: no need to quit the game if the xml does not exist - this data just won't be there, but the game will still work
	if ( !hFile )
		return( TRUE );

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

	XML_SetElementHandler(parser, foodopinionStartElementHandle, foodopinionEndElementHandle);
	XML_SetCharacterDataHandler(parser, foodopinionCharacterDataHandle);

	memset(&pData,0,sizeof(pData));
	pData.curArray = FoodOpinions;
	pData.maxArraySize = NUM_PROFILES;

	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in FoodOpinion.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return( TRUE );
}

BOOLEAN WriteFoodOpinionStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writefoodsstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\FoodOpinion out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<FOODOPINIONSLIST>\r\n");
		for(cnt = 0; cnt < FOOD_TYPE_MAX; ++cnt)
		{
			FilePrintf(hFile,"\t<FOODOPINIONS>\r\n");

			FilePrintf(hFile,"\t\t<uProfile>%d</uProfile>\r\n",				cnt );

			FilePrintf(hFile,"\t</FOODOPINIONS>\r\n");
		}
		FilePrintf(hFile,"</FOODOPINIONSLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
