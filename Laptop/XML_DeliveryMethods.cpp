#include "sgp.h"
#include "overhead types.h"
#include "overhead.h"
#include "text.h"
#include "Debug Control.h"
#include "expat.h"
#include "XML.h"
#include "PostalService.h"
#include <string>
#include <vector>

using namespace std;

extern CPostalService gPostalService;

typedef struct
{
	UINT32	uiDestinationIndex;
	UINT16	usDestinationFee;
	INT8	bDaysAhead;
}DestinationDeliveryInfoReadInStruct;
typedef DestinationDeliveryInfoReadInStruct& RefToDestinationDeliveryInfoReadInStruct;
typedef vector<DestinationDeliveryInfoReadInStruct> DestinationDeliveryInfoReadInTable;
typedef DestinationDeliveryInfoReadInTable& RefToDestinationDeliveryInfoReadInTable;
typedef DestinationDeliveryInfoReadInTable::iterator DestinationDeliveryInfoReadInTableIterator;
typedef DestinationDeliveryInfoReadInTableIterator& RefToDestinationDeliveryInfoReadInTableIterator;

enum
{
	DELIVERYMETHOD_ELEMENT_NONE=0,
	DELIVERYMETHOD_ELEMENT_TABLE,
	DELIVERYMETHOD_ELEMENT,
	DELIVERYMETHOD_ELEMENT_PROPERTY,
	DESTINATIONDELIVERYINFO_ELEMENT_TABLE,
	DESTINATIONDELIVERYINFO_ELEMENT,
	DESTINATIONDELIVERYINFO_ELEMENT_PROPERTY
}typedef DELIVERYMETHOD_PARSE_STAGE;

struct
{
	DELIVERYMETHOD_PARSE_STAGE	curElement;

	CHAR8						szCharData[MAX_CHAR_DATA_LENGTH+1];

	struct
	{
		CHAR16 szDescription[MAX_DELIVERYMETHOD_DESC_LENGTH+1];
		DestinationDeliveryInfoReadInTable* DestinationDeliveryInfos;
	}CurDeliveryMethod;

	struct
	{
		UINT32	uiDestinationIndex;
		UINT16	usDestinationFee;
		INT8	bDaysAhead;
	}CurDestinationDeliveryInfo;

	UINT32						maxArraySize;
	UINT32						currentDepth;
	UINT32						maxReadDepth;
}
typedef deliveryMethodParseData;

static void XMLCALL
deliveryMethodStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	deliveryMethodParseData * pData = (deliveryMethodParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth)
	{
		if(strcmp(name, "DELIVERYMETHODTABLE") == 0 && pData->curElement == DELIVERYMETHOD_ELEMENT_NONE)
		{
			pData->curElement = DELIVERYMETHOD_ELEMENT_TABLE;

			pData->maxReadDepth++;
		}
		else if(strcmp(name, "DELIVERYMETHOD") == 0 && pData->curElement == DELIVERYMETHOD_ELEMENT_TABLE)
		{
			pData->curElement = DELIVERYMETHOD_ELEMENT;
			pData->maxReadDepth++;
		}
		else if(pData->curElement == DELIVERYMETHOD_ELEMENT &&
				(strcmp(name, "description") == 0) )
		{
			pData->curElement = DELIVERYMETHOD_ELEMENT_PROPERTY;

			pData->maxReadDepth++;
		}
		else if(pData->curElement == DELIVERYMETHOD_ELEMENT &&
				(strcmp(name, "DESTINATIONDELIVERYINFOTABLE") == 0))
		{
			pData->curElement = DESTINATIONDELIVERYINFO_ELEMENT_TABLE;

			pData->maxReadDepth++;
		}
		else if(pData->curElement == DESTINATIONDELIVERYINFO_ELEMENT_TABLE &&
				(strcmp(name, "DESTINATIONDELIVERYINFO") == 0))
		{
			pData->curElement = DESTINATIONDELIVERYINFO_ELEMENT;

			pData->maxReadDepth++;
		}
		else if(pData->curElement == DESTINATIONDELIVERYINFO_ELEMENT &&
				(strcmp(name, "uiDestinationIndex") == 0 ||
				strcmp(name, "usDestinationFee") == 0 ||
				strcmp(name, "bDaysAhead") == 0 ) )
		{
			pData->curElement = DESTINATIONDELIVERYINFO_ELEMENT_PROPERTY;

			pData->maxReadDepth++;
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
deliveryMethodCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	deliveryMethodParseData * pData = (deliveryMethodParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}

static void XMLCALL
deliveryMethodEndElementHandle(void *userData, const XML_Char *name)
{
	deliveryMethodParseData * pData = (deliveryMethodParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth)
	{
		if(strcmp(name, "DELIVERYMETHODTABLE") == 0)
		{
			pData->curElement = DELIVERYMETHOD_ELEMENT_NONE;
		}
		else if(strcmp(name, "DELIVERYMETHOD") == 0)
		{
			pData->curElement = DELIVERYMETHOD_ELEMENT_TABLE;
			UINT8 ubCurrentDM = gPostalService.AddDeliveryMethod(pData->CurDeliveryMethod.szDescription);
			
			RefToDestinationDeliveryInfoReadInTableIterator dfriti = pData->CurDeliveryMethod.DestinationDeliveryInfos->begin();

			while(dfriti != pData->CurDeliveryMethod.DestinationDeliveryInfos->end())
			{
				gPostalService.SetDestinationDeliveryInfo(ubCurrentDM, ((RefToDestinationDeliveryInfoReadInStruct)*dfriti).uiDestinationIndex, ((RefToDestinationDeliveryInfoReadInStruct)*dfriti).usDestinationFee, ((RefToDestinationDeliveryInfoReadInStruct)*dfriti).bDaysAhead);
				dfriti++;
			}

			// Clean up
			pData->CurDeliveryMethod.DestinationDeliveryInfos->clear();
			memset(pData->CurDeliveryMethod.szDescription, 0, sizeof(pData->CurDeliveryMethod.szDescription));
			pData->CurDestinationDeliveryInfo.uiDestinationIndex = 0;
			pData->CurDestinationDeliveryInfo.uiDestinationIndex = 0;
		}
		else if(strcmp(name, "description") == 0)
		{
			pData->curElement = DELIVERYMETHOD_ELEMENT;
			MultiByteToWideChar(CP_UTF8, 0, pData->szCharData, -1, &pData->CurDeliveryMethod.szDescription[0], MAX_DELIVERYMETHOD_DESC_LENGTH);
		}
		else if(strcmp(name, "DESTINATIONDELIVERYINFOTABLE") == 0)
		{
			pData->curElement = DELIVERYMETHOD_ELEMENT;
		}
		else if(strcmp(name, "DESTINATIONDELIVERYINFO") == 0)
		{
			pData->curElement = DESTINATIONDELIVERYINFO_ELEMENT_TABLE;
			DestinationDeliveryInfoReadInStruct dfris;

			dfris.uiDestinationIndex = pData->CurDestinationDeliveryInfo.uiDestinationIndex;
			dfris.usDestinationFee = pData->CurDestinationDeliveryInfo.usDestinationFee;
			dfris.bDaysAhead = pData->CurDestinationDeliveryInfo.bDaysAhead;

			memset(&pData->CurDestinationDeliveryInfo, 0, sizeof(pData->CurDestinationDeliveryInfo));

			pData->CurDeliveryMethod.DestinationDeliveryInfos->push_back(dfris);
		}
		else if(strcmp(name, "uiDestinationIndex") == 0)
		{
			pData->curElement = DESTINATIONDELIVERYINFO_ELEMENT;
			pData->CurDestinationDeliveryInfo.uiDestinationIndex = strtoul(pData->szCharData, NULL, 10);
		}
		else if(strcmp(name, "usDestinationFee") == 0)
		{
			pData->curElement = DESTINATIONDELIVERYINFO_ELEMENT;
			pData->CurDestinationDeliveryInfo.usDestinationFee = (UINT16) strtoul(pData->szCharData, NULL, 10);
		}
		else if(strcmp(name, "bDaysAhead") == 0)
		{
			pData->curElement = DESTINATIONDELIVERYINFO_ELEMENT;
			pData->CurDestinationDeliveryInfo.bDaysAhead = (INT8) strtoul(pData->szCharData, NULL, 10);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}

BOOLEAN ReadInDeliveryMethods(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	deliveryMethodParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading DeliveryMethods.xml" );

	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( FALSE );

	uiFSize = FileGetSize(hFile);
	lpcBuffer = (CHAR8 *) MemAlloc(uiFSize+1);

	if ( !FileRead( hFile, lpcBuffer, uiFSize, &uiBytesRead ) )
	{
		MemFree(lpcBuffer);
		return( FALSE );
	}

	lpcBuffer[uiFSize] = 0;

	FileClose( hFile );


	XML_SetElementHandler(parser, deliveryMethodStartElementHandle, deliveryMethodEndElementHandle);
	XML_SetCharacterDataHandler(parser, deliveryMethodCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.CurDeliveryMethod.DestinationDeliveryInfos = new DestinationDeliveryInfoReadInTable();
	pData.maxArraySize = sizeof(UINT16);

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in DeliveryMethods.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	if(pData.CurDeliveryMethod.DestinationDeliveryInfos)
	{
		delete pData.CurDeliveryMethod.DestinationDeliveryInfos;
	}
	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	// Interface stuff for the old game code. Once Bobby Ray and the laptop are fully C++'ized and/or externalised, this stuff can go
	

	return( TRUE );
}
