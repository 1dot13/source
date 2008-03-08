#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	DEALER_POSSIBLE_INV		curInventory;
	DEALER_POSSIBLE_INV *	curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef inventoryParseData;

static void XMLCALL
inventoryStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	inventoryParseData * pData = (inventoryParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "INVENTORYLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(DEALER_POSSIBLE_INV)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "INVENTORY") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curInventory,0,sizeof(DEALER_POSSIBLE_INV));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "sItemIndex") == 0 ||
				strcmp(name, "ubOptimalNumber") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
inventoryCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	inventoryParseData * pData = (inventoryParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
inventoryEndElementHandle(void *userData, const XML_Char *name)
{
	inventoryParseData * pData = (inventoryParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "INVENTORYLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "INVENTORY") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curInventory.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curInventory.uiIndex] = pData->curInventory; //write the inventory into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curInventory.uiIndex	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "sItemIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curInventory.sItemIndex	= (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubOptimalNumber") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curInventory.ubOptimalNumber = (UINT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInInventoryStats(DEALER_POSSIBLE_INV *pInv, STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	inventoryParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Inventory.xml" );

	// Open inventory file
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


	XML_SetElementHandler(parser, inventoryStartElementHandle, inventoryEndElementHandle);
	XML_SetCharacterDataHandler(parser, inventoryCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = pInv;
	pData.maxArraySize = MAXITEMS;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Inventory.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
BOOLEAN WriteInventoryStats(DEALER_POSSIBLE_INV *pInv, STR fileName)
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeinventorystats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<INVENTORYLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<INVENTORY>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",								cnt );
			FilePrintf(hFile,"\t\t<sItemIndex>%d</sItemIndex>\r\n",								pInv[cnt].sItemIndex	);
			FilePrintf(hFile,"\t\t<ubOptimalNumber>%d</ubOptimalNumber>\r\n",								pInv[cnt].ubOptimalNumber	);

			FilePrintf(hFile,"\t</INVENTORY>\r\n");
		}
		FilePrintf(hFile,"</INVENTORYLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
