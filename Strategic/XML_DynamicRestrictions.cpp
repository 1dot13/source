///////////////////////////////////////////////////////////////////////////////
// HEADROCK HAM 3.4: Dynamic Roaming Restrictions XMLization
//
// This is a new part of the Dynamic Roaming Restrictions system. It reads in
// data about sectors to "un-restrict", and the towns that must be liberated
// for them to be unrestricted. There can be several unrestriction conditions
// for each sector, and only one of the conditions must be true for the sector
// to become allowed.
//
///////////////////////////////////////////////////////////////////////////////

#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "gamesettings.h"
	#include "XML.h"
	#include "FileMan.h"
	#include "campaign types.h"
	#include "militiasquads.h"
	#include "mapscreen.h" 
#endif

// HEADROCK HAM 3.4: Stores restriction data from XML, including required cities for each sector to be allowed.
extern DYNAMICRESTRICTIONS gDynamicRestrictions[5001];

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	INT16			sCurSectorX;
	INT16			sCurSectorY;
	UINT32			uiReqTownFlags;
	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef dynamicroamingParseData;


static void XMLCALL
dynamicroamingStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	dynamicroamingParseData * pData = (dynamicroamingParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "DYNAMICROAMING") == 0 && pData->curElement == ELEMENT_NONE)
		{
			// Set all sector IDs to -1 ("unused")
			for (UINT16 cnt=0; cnt<5001; cnt++)
			{
				gDynamicRestrictions[cnt].bSectorID = -1;
			}
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "RESTRICTION") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;
			pData->uiReqTownFlags = 0;

			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3,"MergeStartElementHandle: setting memory for curMerge");

			pData->maxReadDepth++; //we are not skipping this element

			//pData->curIndex++;
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "SectorGrid") == 0  ||
				strcmp(name, "ReqTown") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
dynamicroamingCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	dynamicroamingParseData * pData = (dynamicroamingParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
dynamicroamingEndElementHandle(void *userData, const XML_Char *name)
{
	dynamicroamingParseData * pData = (dynamicroamingParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "DYNAMICROAMING") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "RESTRICTION") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if (SECTOR(pData->sCurSectorX, pData->sCurSectorY) >= 0 && SECTOR(pData->sCurSectorX, pData->sCurSectorY) < 256)
			{
				pData->curIndex++;
				gDynamicRestrictions[pData->curIndex].bSectorID = SECTOR(pData->sCurSectorX,pData->sCurSectorY);
				gDynamicRestrictions[pData->curIndex].uiReqTownFlags |= pData->uiReqTownFlags;
			}
		}

		else if(strcmp(name, "SectorGrid") == 0 )
		{
			UINT8	x, y;
			pData->curElement = ELEMENT;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol(&pData->szCharData[1]);
			if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
			{
				pData->sCurSectorX	= x;
				pData->sCurSectorY  = y;
			}
		}
		else if(strcmp(name, "ReqTown") == 0 )
		{
			if ((UINT8) atol(pData->szCharData) > 0 && (UINT8) atol(pData->szCharData) < NUM_TOWNS )
			{
				pData->curElement = ELEMENT;
				pData->uiReqTownFlags |= (1 << ((UINT8) atol(pData->szCharData) - 1));
			}
			else
			{
				pData->uiReqTownFlags = 0;
			}
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInDynamicRoamingRestrictions(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	dynamicroamingParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading DynamicRestrictions.xml" );

	// Open merges file
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


	XML_SetElementHandler(parser, dynamicroamingStartElementHandle, dynamicroamingEndElementHandle);
	XML_SetCharacterDataHandler(parser, dynamicroamingCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXITEMS;
	pData.curIndex = -1;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in DynamicRestrictions.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}

