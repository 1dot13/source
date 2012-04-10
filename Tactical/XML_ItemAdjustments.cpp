#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "gamesettings.h"
	#include "XML.h"
	#include "Item Types.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	TransformInfoStruct curTransform;
	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			curResultIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef transformParseData;

static void XMLCALL
transformStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	transformParseData * pData = (transformParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "TRANSFORMATIONS_LIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			// Reset all transformation indexes to -1, indicating they are "Not Taken"
			for (UINT16 x = 0; x < MAXITEMS; x++)
			{
				Transform[x].usItem = -1;
			}

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "TRANSFORM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			// Reset values
			pData->curTransform.usItem = -1;
			for (UINT8 x = 0; x < MAX_NUM_TRANSFORMATION_RESULTS; x++)
			{
				pData->curTransform.usResult[x] = 0;
			}
			pData->curTransform.usAPCost = 0;
			swprintf(pData->curTransform.szMenuRowText, L"");
			swprintf(pData->curTransform.szTooltipText, L"");

			pData->maxReadDepth++; //we are not skipping this element
			pData->curIndex++;
			pData->curResultIndex = 0;
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "usItem") == 0 ||
				strcmp(name, "usResult") == 0 ||
				strcmp(name, "usAPCost") == 0 ||
				strcmp(name, "iBPCost") == 0 ||
				strcmp(name, "szMenuRowText") == 0 ||
				strcmp(name, "szTooltipText") == 0))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
transformCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	transformParseData * pData = (transformParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
transformEndElementHandle(void *userData, const XML_Char *name)
{
	transformParseData * pData = (transformParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "TRANSFORMATIONS_LIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "TRANSFORM") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curIndex < pData->maxArraySize)
			{
				//DebugMsg(TOPIC_JA2, DBG_LEVEL_3,"MergeStartElementHandle: writing merge to array");
				Transform[pData->curIndex].usItem = pData->curTransform.usItem; //write the merge into the table
				for (INT32 x = 0; x < MAX_NUM_TRANSFORMATION_RESULTS; x++)
				{
					Transform[pData->curIndex].usResult[x] = pData->curTransform.usResult[x];
				}
				Transform[pData->curIndex].usAPCost = pData->curTransform.usAPCost;
				Transform[pData->curIndex].iBPCost = pData->curTransform.iBPCost;
				wcscpy( Transform[pData->curIndex].szMenuRowText, pData->curTransform.szMenuRowText );
				wcscpy( Transform[pData->curIndex].szTooltipText, pData->curTransform.szTooltipText );
			}
		}
		else if(strcmp(name, "usItem") == 0)
		{
			pData->curElement = ELEMENT;
			INT16 usItem = (INT16) atol(pData->szCharData);
			if (usItem <= 0)
			{
				AssertMsg( 0, String( "Item_Transformations.XML error: Entry #%d missing proper <usItem> tag!", pData->curIndex ) );
			}
			pData->curTransform.usItem = usItem;
		}
		else if(strcmp(name, "usResult") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTransform.usResult[pData->curResultIndex] = (UINT16) atol(pData->szCharData);
			pData->curResultIndex++;
		}
		else if(strcmp(name, "usAPCost") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTransform.usAPCost = (UINT16) atol(pData->szCharData);
			pData->curTransform.usAPCost = (UINT16)DynamicAdjustAPConstants(pData->curTransform.usAPCost, pData->curTransform.usAPCost);
		}
		else if(strcmp(name, "iBPCost") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTransform.iBPCost = (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "szMenuRowText") == 0)
		{
			pData->curElement = ELEMENT;
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curTransform.szMenuRowText, sizeof(pData->curTransform.szMenuRowText)/sizeof(pData->curTransform.szMenuRowText[0]) );
			pData->curTransform.szMenuRowText[sizeof(pData->curTransform.szMenuRowText)/sizeof(pData->curTransform.szMenuRowText[0]) - 1] = '\0';
		}
		else if(strcmp(name, "szTooltipText") == 0)
		{
			pData->curElement = ELEMENT;
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curTransform.szTooltipText, sizeof(pData->curTransform.szTooltipText)/sizeof(pData->curTransform.szTooltipText[0]) );
			pData->curTransform.szTooltipText[sizeof(pData->curTransform.szTooltipText)/sizeof(pData->curTransform.szTooltipText[0]) - 1] = '\0';
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInTransformationStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	transformParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Item_Transformations.xml" );

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


	XML_SetElementHandler(parser, transformStartElementHandle, transformEndElementHandle);
	XML_SetCharacterDataHandler(parser, transformCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXITEMS;
	pData.curIndex = -1;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Item_Transformations.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}

