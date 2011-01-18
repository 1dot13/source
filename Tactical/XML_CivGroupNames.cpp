#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "EditorMercs.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	CIV_NAMES_VALUES	curCivGroupNames;
	CIV_NAMES_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef civGroupNamesParseData;

BOOLEAN CivGroupNames_TextOnly;

static void XMLCALL
civGroupNamesStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	civGroupNamesParseData * pData = (civGroupNamesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "CIV_GROUP_NAMES") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "NAME") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "Enabled") == 0 ||
				strcmp(name, "Loyalty") == 0 ||
				strcmp(name, "AddToBattle") == 0 ||
				strcmp(name, "szGroup") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
civGroupNamesCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	civGroupNamesParseData * pData = (civGroupNamesParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
civGroupNamesEndElementHandle(void *userData, const XML_Char *name)
{
	civGroupNamesParseData * pData = (civGroupNamesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "CIV_GROUP_NAMES") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "NAME") == 0)
		{
			pData->curElement = ELEMENT_LIST;
				
			if (!CivGroupNames_TextOnly)
				{
					wcscpy(zCivGroupName[pData->curCivGroupNames.uiIndex].szCurGroup, pData->curCivGroupNames.szCurGroup);
					zCivGroupName[pData->curCivGroupNames.uiIndex].Enabled = pData->curCivGroupNames.Enabled;
					
					zCivGroupName[pData->curCivGroupNames.uiIndex].AddToBattle = pData->curCivGroupNames.AddToBattle;
					zCivGroupName[pData->curCivGroupNames.uiIndex].Loyalty = pData->curCivGroupNames.Loyalty;
				}
				else
				{
					wcscpy(zCivGroupName[pData->curCivGroupNames.uiIndex].szCurGroup, pData->curCivGroupNames.szCurGroup);
				}			
		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curCivGroupNames.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "Enabled") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curCivGroupNames.Enabled	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Loyalty") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curCivGroupNames.Loyalty	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "AddToBattle") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curCivGroupNames.AddToBattle	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "szGroup") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curCivGroupNames.szCurGroup, sizeof(pData->curCivGroupNames.szCurGroup)/sizeof(pData->curCivGroupNames.szCurGroup[0]) );
			pData->curCivGroupNames.szCurGroup[sizeof(pData->curCivGroupNames.szCurGroup)/sizeof(pData->curCivGroupNames.szCurGroup[0]) - 1] = '\0';
	
			#ifdef JA2EDITOR
			wcscpy(gszCivGroupNames[pData->curCivGroupNames.uiIndex], pData->curCivGroupNames.szCurGroup);
			#endif
		
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInCivGroupNamesStats( STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	civGroupNamesParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading CivGroupNames.xml" );
	
	CivGroupNames_TextOnly = localizedVersion;
	
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


	XML_SetElementHandler(parser, civGroupNamesStartElementHandle, civGroupNamesEndElementHandle);
	XML_SetCharacterDataHandler(parser, civGroupNamesCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in CivGroupNames.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}