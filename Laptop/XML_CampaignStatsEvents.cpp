	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "CampaignStats.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	CAMPAIGNSTATSEVENT		curItem;
	CAMPAIGNSTATSEVENT*		curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
	CHAR16 gzBackground[MAX_ENEMY_NAMES_CHARS];
}
typedef CSEParseData;

BOOLEAN localizedTextOnly_CSE;

static void XMLCALL
CSEStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	CSEParseData * pData = (CSEParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "CAMPAIGNSTATSEVENTS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			if ( !localizedTextOnly_CSE )
				memset(pData->curArray,0,sizeof(CAMPAIGNSTATSEVENT)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "EVENT") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			if ( !localizedTextOnly_CSE )
				memset(&pData->curItem,0,sizeof(CAMPAIGNSTATSEVENT));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||		
				strcmp(name, "szText0") == 0 ||
				strcmp(name, "szText1") == 0 ||
				strcmp(name, "szText2") == 0 ||
				strcmp(name, "szText3") == 0 ||
				strcmp(name, "szText4") == 0 ||
				strcmp(name, "szText5") == 0 ||
				strcmp(name, "szText6") == 0 ||
				strcmp(name, "szText7") == 0 ||
				strcmp(name, "szText8") == 0 ||
				strcmp(name, "szText9") == 0 ||
				strcmp(name, "usCityTaken") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
CSECharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	CSEParseData * pData = (CSEParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}

static void XMLCALL
CSEEndElementHandle(void *userData, const XML_Char *name)
{
	CSEParseData * pData = (CSEParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "CAMPAIGNSTATSEVENTS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "EVENT") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if(pData->curItem.uiIndex < pData->maxArraySize)
			{
				if ( localizedTextOnly_CSE )
				{
					// WANNE: Do not only copy the first szText, but copy ALL the szTexts!
					//wcscpy(pData->curArray[pData->curItem.uiIndex].szText[0],pData->curItem.szText[0]);
					for (int i = 0; i < MAX_CAMPAIGNSTATSEVENTS_TEXTS; i++)
					{
						wcscpy(zCampaignStatsEvent[pData->curItem.uiIndex].szText[i], pData->curItem.szText[i]);
					}					
				}
				else
				{
					pData->curArray[pData->curItem.uiIndex] = pData->curItem;
				}
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "usCityTaken") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.usCityTaken = (UINT8) atol(pData->szCharData);
		}
		else
		{
			for(int i = 0; i < MAX_CAMPAIGNSTATSEVENTS_TEXTS; ++i)
			{
				char txt[10];
				sprintf(txt, "szText%d", i);

				if(strcmp(name, txt) == 0 )
				{
					pData->curElement = ELEMENT;
			
					MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curItem.szText[i], sizeof(pData->curItem.szText[i])/sizeof(pData->curItem.szText[i][0]) );
					pData->curItem.szText[i][sizeof(pData->curItem.szText[i])/sizeof(pData->curItem.szText[i][0]) - 1] = '\0';

					break;
				}
			}
		}

		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInCampaignStatsEvents(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	CSEParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading CampaignStatsEvents.xml" );

	localizedTextOnly_CSE = localizedVersion;
		
	// Open file
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


	XML_SetElementHandler(parser, CSEStartElementHandle, CSEEndElementHandle);
	XML_SetCharacterDataHandler(parser, CSECharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = zCampaignStatsEvent;
	pData.maxArraySize = NUM_CAMPAIGNSTATSEVENTS;

	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in CampaignStatsEvents.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}


BOOLEAN WriteCampaignStatsEvents( STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<CAMPAIGNSTATSEVENTS>\r\n");
		for(cnt = 0; cnt < NUM_CAMPAIGNSTATSEVENTS; ++cnt)
		{
			FilePrintf(hFile,"\t<EVENT>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",				cnt);
			
			FilePrintf(hFile,"\t</EVENT>\r\n");
		}
		FilePrintf(hFile,"</CAMPAIGNSTATSEVENTS>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
