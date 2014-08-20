#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "Soldier Profile.h"
	#include "Text.h"
	#include "history.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	HISTORY_VALUES		curMercHistorys;
	HISTORY_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
	//CHAR16 gzMercNames[MAX_ENEMY_NAMES_CHARS];
}
typedef mercHistoryParseData;

BOOLEAN MercHistory_TextOnly;

static void XMLCALL
mercHistoryStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	mercHistoryParseData * pData = (mercHistoryParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "STRINGS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "TEXT") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "sHistory") == 0 
				))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
mercHistoryCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	mercHistoryParseData * pData = (mercHistoryParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
mercHistoryEndElementHandle(void *userData, const XML_Char *name)
{
	mercHistoryParseData * pData = (mercHistoryParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "STRINGS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "TEXT") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if (!MercHistory_TextOnly)
				{
					wcscpy(HistoryName[pData->curMercHistorys.uiIndex].sHistory, pData->curMercHistorys.sHistory);	
					//wcscpy(pHistoryStrings[pData->curMercHistorys.uiIndex], pData->curMercHistorys.sHistory);
				}
				else
				{
					wcscpy(HistoryName[pData->curMercHistorys.uiIndex].sHistory, pData->curMercHistorys.sHistory);
					//wcscpy(pHistoryStrings[pData->curMercHistorys.uiIndex], pData->curMercHistorys.sHistory);					
				}		
		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercHistorys.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "sHistory") == 0)
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curMercHistorys.sHistory, sizeof(pData->curMercHistorys.sHistory)/sizeof(pData->curMercHistorys.sHistory[0]) );
			pData->curMercHistorys.sHistory[sizeof(pData->curMercHistorys.sHistory)/sizeof(pData->curMercHistorys.sHistory[0]) - 1] = '\0';
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}




BOOLEAN ReadInHistorys(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	mercHistoryParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading History.xml" );

	MercHistory_TextOnly = localizedVersion;
	
	// Open file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( localizedVersion );

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


	XML_SetElementHandler(parser, mercHistoryStartElementHandle, mercHistoryEndElementHandle);
	XML_SetCharacterDataHandler(parser, mercHistoryCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in History.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
