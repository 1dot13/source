#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "Map Screen Interface Map.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	MINERALS_VALUES		curMinerals;
	MINERALS_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef mineralsParseData;

BOOLEAN Minerals_TextOnly;

static void XMLCALL
mineralsStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	mineralsParseData * pData = (mineralsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "MINERALS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "TYPE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			     strcmp(name, "sNameType") == 0 ||
				strcmp(name, "sMineral") == 0
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
mineralsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	mineralsParseData * pData = (mineralsParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
mineralsEndElementHandle(void *userData, const XML_Char *name)
{
	mineralsParseData * pData = (mineralsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "MINERALS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "TYPE") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if (!Minerals_TextOnly)
			{
				wcscpy(MineralsName[pData->curMinerals.uiIndex].sMinerals, pData->curMinerals.sMinerals);
				wcscpy(MineralsName[pData->curMinerals.uiIndex].sType, pData->curMinerals.sType);
				MineralsName[pData->curMinerals.uiIndex].Type = pData->curMinerals.Type;
			}
			else
			{
				wcscpy(MineralsName[pData->curMinerals.uiIndex].sMinerals, pData->curMinerals.sMinerals);
				wcscpy(MineralsName[pData->curMinerals.uiIndex].sType, pData->curMinerals.sType);	
			}		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMinerals.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "sNameType") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curMinerals.sType, sizeof(pData->curMinerals.sType)/sizeof(pData->curMinerals.sType[0]) );
			pData->curMinerals.sType[sizeof(pData->curMinerals.sType)/sizeof(pData->curMinerals.sType[0]) - 1] = '\0';
		}
		else if(strcmp(name, "sMineral") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curMinerals.sMinerals, sizeof(pData->curMinerals.sMinerals)/sizeof(pData->curMinerals.sMinerals[0]) );
			pData->curMinerals.sMinerals[sizeof(pData->curMinerals.sMinerals)/sizeof(pData->curMinerals.sMinerals[0]) - 1] = '\0';
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}




BOOLEAN ReadInMinerals(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	mineralsParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Minerals.xml" );

	Minerals_TextOnly = localizedVersion;
	
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


	XML_SetElementHandler(parser, mineralsStartElementHandle, mineralsEndElementHandle);
	XML_SetCharacterDataHandler(parser, mineralsCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Minerals.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
