#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "XML.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include "XML.h"
	#include "expat.h"
	#include "string.h"
	#include "Campaign Types.h"
	#include "FileMan.h"
	#include "MemMan.h"
	#include "Debug Control.h"
	#include "mapscreen.h"
#endif

#define MAX_CHAR_DATA_LENGTH			500

extern UINT32 gCoolnessBySector[256];

typedef struct
{
	PARSE_STAGE	curElement;
	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	INT32			iCurCoolness;
	UINT32			uiRowNumber;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
} SectorCoolnessParseData;


static void XMLCALL
SectorCoolnessStartElementHandle(void *userData, const XML_Char *name, const char **atts)
{
	SectorCoolnessParseData * pData = (SectorCoolnessParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{

		if(strcmp(name, "COOLNESS_BY_SECTOR") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "MAP_ROW") == 0 && pData->curElement == ELEMENT_LIST)
		{
			UINT32 uiAttrIndex;
			pData->curElement = ELEMENT;

			for(uiAttrIndex = 0;atts[uiAttrIndex] != NULL;uiAttrIndex += 2)
				if(strcmp(atts[uiAttrIndex], "row") == 0)
				{
					pData->uiRowNumber = atol(atts[uiAttrIndex+1]);
					break;
				}

			if(atts[uiAttrIndex] != NULL && pData->uiRowNumber < MAP_WORLD_Y)
				pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}



static void XMLCALL
SectorCoolnessCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	SectorCoolnessParseData * pData = (SectorCoolnessParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}


static void XMLCALL
SectorCoolnessEndElementHandle(void *userData, const XML_Char *name)
{
	SectorCoolnessParseData * pData = (SectorCoolnessParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "COOLNESS_BY_SECTOR") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "MAP_ROW") == 0 && pData->curElement == ELEMENT)
		{
			STR8 curBuffer = pData->szCharData + strspn(pData->szCharData," \t\n\r");
			UINT32 curCellIndex = 0;
			UINT32 curNumber;

			pData->curElement = ELEMENT_LIST;

			while(curBuffer[0] != '\0')
			{
				sscanf( curBuffer,"%d",&curNumber);

				gCoolnessBySector[SECTOR(curCellIndex+1, pData->uiRowNumber) ] = curNumber;

				curCellIndex++;
				curBuffer += strcspn(curBuffer," \t\n\r\0");
				curBuffer += strspn(curBuffer," \t\n\r");
			}
		}
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInCoolnessBySector(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	SectorCoolnessParseData pData;

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


	XML_SetElementHandler(parser, SectorCoolnessStartElementHandle, SectorCoolnessEndElementHandle);
	XML_SetCharacterDataHandler(parser, SectorCoolnessCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];
		sprintf(errorBuf, "XML Parser Error in CoolnessBySector.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return TRUE;
}
