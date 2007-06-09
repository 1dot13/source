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
#endif

#define MAX_CHAR_DATA_LENGTH			500

extern INT32  iRestrictedSectorArraySize;
extern UINT32 gRestrictMilitia[256];


typedef enum
{
	ROAMING_ELEMENT_NONE = 0,
	ROAMING_ELEMENT_RESTRICTED_SECTORS,
	ROAMING_ELEMENT_SECTOR,
} ROAMING_PARSE_STAGE;




typedef struct
{
	UINT8	ubSectorID;
	BOOLEAN fValidSector;
} RoamingInfo;


typedef struct
{
	ROAMING_PARSE_STAGE	curElement;
	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	RoamingInfo		curRoamingInfo;
	UINT32			uiSectorCount;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
} RoamingParseData;


static void XMLCALL
RoamingStartElementHandle(void *userData, const XML_Char *name, const char **atts)
{
	RoamingParseData * pData = (RoamingParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{

		if(strcmp(name, "RESTRICTED_SECTORS") == 0 && pData->curElement == ROAMING_ELEMENT_NONE)
		{
			pData->curElement = ROAMING_ELEMENT_RESTRICTED_SECTORS;
			pData->maxReadDepth++; //we are not skipping this element
		}

		else if(strcmp(name, "SECTOR") == 0 && pData->curElement == ROAMING_ELEMENT_RESTRICTED_SECTORS)
		{
			pData->curElement = ROAMING_ELEMENT_SECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}



static void XMLCALL
RoamingCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	RoamingParseData * pData = (RoamingParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}


static void XMLCALL
RoamingEndElementHandle(void *userData, const XML_Char *name)
{
	RoamingParseData * pData = (RoamingParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "RESTRICTED_SECTORS") == 0 && pData->curElement == ROAMING_ELEMENT_RESTRICTED_SECTORS)
		{
			pData->curElement = ROAMING_ELEMENT_NONE;
			iRestrictedSectorArraySize = pData->uiSectorCount;
		}
		//Kaiden: Code shamelessly stolen from lesh to accept sector Aliases
		else if(strcmp(name, "SECTOR") == 0 && pData->curElement == ROAMING_ELEMENT_SECTOR)
		{
			UINT8	x, y;
			pData->curElement = ROAMING_ELEMENT_RESTRICTED_SECTORS;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol(&pData->szCharData[1]);
			if ( x > 0 && x <= 16  && y > 0 && y <= 16 )
			{
				pData->curRoamingInfo.fValidSector = TRUE;
				pData->curRoamingInfo.ubSectorID   = SECTOR(x,y);
				gRestrictMilitia[pData->uiSectorCount] = pData->curRoamingInfo.ubSectorID;
				pData->uiSectorCount++;
			}
			else
			{
				pData->curRoamingInfo.fValidSector = FALSE;
			}
		}
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInRoamingInfo(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	RoamingParseData pData;

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

	
	XML_SetElementHandler(parser, RoamingStartElementHandle, RoamingEndElementHandle);
	XML_SetCharacterDataHandler(parser, RoamingCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);
	iRestrictedSectorArraySize = 0;

    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];
		sprintf(errorBuf, "XML Parser Error in RestrictedRoamingMilitia.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return TRUE;
}