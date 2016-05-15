#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "Arms Dealer Init.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	ARMS_DEALER_INFO		curMerchant;
	ARMS_DEALER_INFO *		curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef merchantParseData;

static void XMLCALL
merchantStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	merchantParseData * pData = (merchantParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "MERCHANTSLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(ARMS_DEALER_INFO)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "MERCHANT") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curMerchant,0,sizeof(ARMS_DEALER_INFO));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "szName") == 0 ||
				strcmp(name, "dBuyModifier") == 0 ||
				strcmp(name, "dSellModifier") == 0 ||
				strcmp(name, "ubShopKeeperID") == 0 ||
				strcmp(name, "ubTypeOfArmsDealer") == 0 ||
				strcmp(name, "iInitialCash") == 0 ||
				strcmp(name, "uiFlags") == 0 ||
				strcmp(name, "dailyIncrement") == 0 ||
				strcmp(name, "dailyMaximum") == 0 ||
				strcmp(name, "dailyRetained") == 0 ||
				strcmp(name, "minCoolness") == 0 ||
				strcmp(name, "maxCoolness") == 0 ||
				strcmp(name, "addToCoolness") == 0 ||
				strcmp(name, "coolnessProgressRate") == 0 ||
				strcmp(name, "daysDelayMin") == 0 ||
				strcmp(name, "daysDelayMax") == 0 ||
				strcmp(name, "useBRSetting") == 0 ||
				strcmp(name, "allInventoryAlwaysAvailable" ) == 0 ||
				strcmp(name, "nonprofile_loyaltyrequired") == 0 )) 
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
merchantCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	merchantParseData * pData = (merchantParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
merchantEndElementHandle(void *userData, const XML_Char *name)
{
	merchantParseData * pData = (merchantParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "MERCHANTSLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "MERCHANT") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			// we do NOT want to read the first entry -> move stuff by 1
			if(pData->curMerchant.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curMerchant.uiIndex] = pData->curMerchant; //write the merchant into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "szName") == 0)
		{
			pData->curElement = ELEMENT;
			// not needed, but it's there for informational purposes
		}
		else if(strcmp(name, "dBuyModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.dBuyModifier	= (FLOAT) atof(pData->szCharData);

			// fit for reasonable values
			pData->curMerchant.dBuyModifier = min(5.0f, max(0.1f, pData->curMerchant.dBuyModifier) );
		}
		else if(strcmp(name, "dSellModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.dSellModifier = (FLOAT) atof(pData->szCharData);

			// fit for reasonable values
			pData->curMerchant.dSellModifier = min(5.0f, max(0.1f, pData->curMerchant.dSellModifier) );
		}
		else if(strcmp(name, "ubShopKeeperID") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.ubShopKeeperID	= (UINT8) atol(pData->szCharData);						
		}
		else if(strcmp(name, "ubTypeOfArmsDealer") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.ubTypeOfArmsDealer	= (UINT8) atol(pData->szCharData);

			// fit for reasonable values
			pData->curMerchant.ubTypeOfArmsDealer = min(ARMS_DEALER_REPAIRS, pData->curMerchant.ubTypeOfArmsDealer);
		}
		else if(strcmp(name, "iInitialCash") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.iInitialCash	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "uiFlags") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.uiFlags	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "dailyIncrement") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.dailyIncrement	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "dailyMaximum") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.dailyMaximum	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "dailyRetained") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.dailyRetained	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "minCoolness") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.minCoolness	= (UINT32) strtoul(pData->szCharData, NULL, 0);

			// fit for reasonable values
			pData->curMerchant.minCoolness = min(10, max(1, pData->curMerchant.minCoolness) );
		}
		else if(strcmp(name, "maxCoolness") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.maxCoolness	= (UINT32) strtoul(pData->szCharData, NULL, 0);

			// fit for reasonable values
			pData->curMerchant.maxCoolness = min(10, max(1, pData->curMerchant.maxCoolness) );
		}
		else if(strcmp(name, "addToCoolness") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.addToCoolness	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "coolnessProgressRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.coolnessProgressRate	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "daysDelayMin") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.daysDelayMin	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "daysDelayMax") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.daysDelayMax	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "useBRSetting") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.useBRSetting	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "allInventoryAlwaysAvailable") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.allInventoryAlwaysAvailable	= (BOOLEAN) atol(pData->szCharData);
		}
		else if ( strcmp( name, "nonprofile_loyaltyrequired" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curMerchant.nonprofile_loyaltyrequired = (UINT8)strtoul( pData->szCharData, NULL, 0 );
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInMerchantStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	merchantParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Merchants.xml" );

	// Open merchants file
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
	
	XML_SetElementHandler(parser, merchantStartElementHandle, merchantEndElementHandle);
	XML_SetCharacterDataHandler(parser, merchantCharacterDataHandle);
	
	ARMS_DEALER_INFO Merchants[NUM_ARMS_DEALERS];	// exists solely for this xml

	memset(&pData,0,sizeof(pData));
	pData.curArray = Merchants;
	pData.maxArraySize = NUM_ARMS_DEALERS;

	XML_SetUserData(parser, &pData);
		
	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Merchant.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}
	
	MemFree(lpcBuffer);

	// now fill the internal merchant vector
	std::copy(Merchants, Merchants + sizeof(Merchants)/sizeof(*Merchants), armsDealerInfo.begin());

	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteMerchantStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writemerchantsstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Merchant out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<MERCHANTSLIST>\r\n");
		for(cnt = 0; cnt < NUM_ARMS_DEALERS; ++cnt)
		{
			FilePrintf(hFile,"\t<MERCHANT>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",											armsDealerInfo[cnt].uiIndex );

			FilePrintf(hFile,"\t\t<dBuyModifier>%2.2f</dBuyModifier>\r\n",								armsDealerInfo[cnt].dBuyModifier	);
			FilePrintf(hFile,"\t\t<dSellModifier>%2.2f</dSellModifier>\r\n",							armsDealerInfo[cnt].dSellModifier	);
			FilePrintf(hFile,"\t\t<ubShopKeeperID>%d</ubShopKeeperID>\r\n",								armsDealerInfo[cnt].ubShopKeeperID	);
			FilePrintf(hFile,"\t\t<ubTypeOfArmsDealer>%d</ubTypeOfArmsDealer>\r\n",						armsDealerInfo[cnt].ubTypeOfArmsDealer	);
			FilePrintf(hFile,"\t\t<iInitialCash>%d</iInitialCash>\r\n",									armsDealerInfo[cnt].iInitialCash	);
			FilePrintf(hFile,"\t\t<uiFlags>%d</uiFlags>\r\n",											armsDealerInfo[cnt].uiFlags	);
			FilePrintf(hFile,"\t\t<dailyIncrement>%d</dailyIncrement>\r\n",								armsDealerInfo[cnt].dailyIncrement	);
			FilePrintf(hFile,"\t\t<dailyMaximum>%d</dailyMaximum>\r\n",									armsDealerInfo[cnt].dailyMaximum	);
			FilePrintf(hFile,"\t\t<dailyRetained>%d</dailyRetained>\r\n",								armsDealerInfo[cnt].dailyRetained	);
			FilePrintf(hFile,"\t\t<minCoolness>%d</minCoolness>\r\n",									armsDealerInfo[cnt].minCoolness	);
			FilePrintf(hFile,"\t\t<maxCoolness>%d</maxCoolness>\r\n",									armsDealerInfo[cnt].maxCoolness	);
			FilePrintf(hFile,"\t\t<addToCoolness>%d</addToCoolness>\r\n",								armsDealerInfo[cnt].addToCoolness	);
			FilePrintf(hFile,"\t\t<coolnessProgressRate>%d</coolnessProgressRate>\r\n",					armsDealerInfo[cnt].coolnessProgressRate	);
			FilePrintf(hFile,"\t\t<daysDelayMin>%d</daysDelayMin>\r\n",									armsDealerInfo[cnt].daysDelayMin	);
			FilePrintf(hFile,"\t\t<daysDelayMax>%d</daysDelayMax>\r\n",									armsDealerInfo[cnt].daysDelayMax	);
			FilePrintf(hFile,"\t\t<useBRSetting>%d</useBRSetting>\r\n",									armsDealerInfo[cnt].useBRSetting	);
			FilePrintf(hFile,"\t\t<allInventoryAlwaysAvailable>%d</allInventoryAlwaysAvailable>\r\n",	armsDealerInfo[cnt].allInventoryAlwaysAvailable	);
			FilePrintf(hFile,"\t\t<nonprofile_loyaltyrequired>%d</nonprofile_loyaltyrequired>\r\n",		armsDealerInfo[cnt].nonprofile_loyaltyrequired );

			FilePrintf(hFile,"\t</MERCHANT>\r\n");						
		}
		FilePrintf(hFile,"</MERCHANTSLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
