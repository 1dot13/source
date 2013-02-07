#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "GameSettings.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	POCKETTYPE		curLBEPocket;
//	POCKETTYPE *	curArray;
//	UINT32			maxArraySize;
	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef lbepocketParseData;


BOOLEAN onlyLocalizedText;
BOOLEAN lbepocketStartElementHandleLoop(const XML_Char *name, lbepocketParseData * pData) //Tais
{
	//DBrot: same routine as the actual reading part below, but no data is read here, only the validity check
	CHAR8 refstr[20] = "ItemCapacityPerSize";
	UINT8 pos = 19;
	if(strncmp(refstr, name, 19) == 0){
		if(name[pos]=='.'){
			pos++;
		}
		CHAR8 index[5] = "\0\0\0\0";
		UINT8 i = 0;
		while(name[pos]!='\0' && i < 5){
			index[i]=name[pos];
			pos++;
			i++;
		}
		UINT16 size = (UINT16) atol(index);
		if(size<=gGameExternalOptions.guiMaxItemSize){
			return true;
		}
	}
	return false;
}
/*	DBrot: Replaced with a more efficient way
	UINT16 i=0;
	CHAR8 str1 [40] = "";
	for (i=0; i<=gGameExternalOptions.guiMaxItemSize;i++)
	{
		sprintf(str1,"ItemCapacityPerSize%d",i);
		if(strcmp(name, str1)==0)
		{
			return true;
		}
	}
	for (i=0; i<=gGameExternalOptions.guiMaxItemSize;i++)
	{
		sprintf(str1,"ItemCapacityPerSize.%d",i);
		if(strcmp(name, str1)==0)
		{
			return true;
		}
	}
	return false;
}*/
static void XMLCALL 
lbepocketStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	lbepocketParseData * pData = (lbepocketParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "POCKETLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			//memset(pData->curArray,0,sizeof(POCKETTYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "POCKET") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			//memset(&pData->curLBEPocket,0,sizeof(POCKETTYPE));
			pData->curLBEPocket = POCKETTYPE();

			pData->maxReadDepth++; //we are not skipping this element
		}//DBrot: MOLLE added tag
		else if(pData->curElement == ELEMENT &&	
				(strcmp(name, "pIndex") == 0 ||
				strcmp(name, "pName") == 0 ||
				strcmp(name, "pSilhouette") == 0 ||
				strcmp(name, "pType") == 0 ||
				strcmp(name, "pRestriction") == 0 ||
				strcmp(name, "pVolume") == 0 ||
				lbepocketStartElementHandleLoop(name, pData)))
				/*	JMich - These are no longer needed
				strcmp(name, "ItemCapacityPerSize.0") == 0 || strcmp(name, "ItemCapacityPerSize0") == 0 ||
				strcmp(name, "ItemCapacityPerSize.1") == 0 || strcmp(name, "ItemCapacityPerSize1") == 0 ||
				strcmp(name, "ItemCapacityPerSize.2") == 0 || strcmp(name, "ItemCapacityPerSize2") == 0 ||
				strcmp(name, "ItemCapacityPerSize.3") == 0 || strcmp(name, "ItemCapacityPerSize3") == 0 ||
				strcmp(name, "ItemCapacityPerSize.4") == 0 || strcmp(name, "ItemCapacityPerSize4") == 0 ||
				strcmp(name, "ItemCapacityPerSize.5") == 0 || strcmp(name, "ItemCapacityPerSize5") == 0 ||
				strcmp(name, "ItemCapacityPerSize.6") == 0 || strcmp(name, "ItemCapacityPerSize6") == 0 ||
				strcmp(name, "ItemCapacityPerSize.7") == 0 || strcmp(name, "ItemCapacityPerSize7") == 0 ||
				strcmp(name, "ItemCapacityPerSize.8") == 0 || strcmp(name, "ItemCapacityPerSize8") == 0 ||
				strcmp(name, "ItemCapacityPerSize.9") == 0 || strcmp(name, "ItemCapacityPerSize9") == 0 ||
				strcmp(name, "ItemCapacityPerSize.10") == 0 || strcmp(name, "ItemCapacityPerSize10") == 0 ||
				strcmp(name, "ItemCapacityPerSize.11") == 0 || strcmp(name, "ItemCapacityPerSize11") == 0 ||
				strcmp(name, "ItemCapacityPerSize.12") == 0 || strcmp(name, "ItemCapacityPerSize12") == 0 ||
				strcmp(name, "ItemCapacityPerSize.13") == 0 || strcmp(name, "ItemCapacityPerSize13") == 0 ||
				strcmp(name, "ItemCapacityPerSize.14") == 0 || strcmp(name, "ItemCapacityPerSize14") == 0 ||
				strcmp(name, "ItemCapacityPerSize.15") == 0 || strcmp(name, "ItemCapacityPerSize15") == 0 ||
				strcmp(name, "ItemCapacityPerSize.16") == 0 || strcmp(name, "ItemCapacityPerSize16") == 0 ||
				strcmp(name, "ItemCapacityPerSize.17") == 0 || strcmp(name, "ItemCapacityPerSize17") == 0 ||
				strcmp(name, "ItemCapacityPerSize.18") == 0 || strcmp(name, "ItemCapacityPerSize18") == 0 ||
				strcmp(name, "ItemCapacityPerSize.19") == 0 || strcmp(name, "ItemCapacityPerSize19") == 0 ||
				strcmp(name, "ItemCapacityPerSize.20") == 0 || strcmp(name, "ItemCapacityPerSize20") == 0 ||
				strcmp(name, "ItemCapacityPerSize.21") == 0 || strcmp(name, "ItemCapacityPerSize21") == 0 ||
				strcmp(name, "ItemCapacityPerSize.22") == 0 || strcmp(name, "ItemCapacityPerSize22") == 0 ||
				strcmp(name, "ItemCapacityPerSize.23") == 0 || strcmp(name, "ItemCapacityPerSize23") == 0 ||
				strcmp(name, "ItemCapacityPerSize.24") == 0 || strcmp(name, "ItemCapacityPerSize24") == 0 ||
				strcmp(name, "ItemCapacityPerSize.25") == 0 || strcmp(name, "ItemCapacityPerSize25") == 0 ||
				strcmp(name, "ItemCapacityPerSize.26") == 0 || strcmp(name, "ItemCapacityPerSize26") == 0 ||
				strcmp(name, "ItemCapacityPerSize.27") == 0 || strcmp(name, "ItemCapacityPerSize27") == 0 ||
				strcmp(name, "ItemCapacityPerSize.28") == 0 || strcmp(name, "ItemCapacityPerSize28") == 0 ||
				strcmp(name, "ItemCapacityPerSize.29") == 0 || strcmp(name, "ItemCapacityPerSize29") == 0 ||
				strcmp(name, "ItemCapacityPerSize.30") == 0 || strcmp(name, "ItemCapacityPerSize30") == 0 ||
				strcmp(name, "ItemCapacityPerSize.31") == 0 || strcmp(name, "ItemCapacityPerSize31") == 0 ||
				strcmp(name, "ItemCapacityPerSize.32") == 0 || strcmp(name, "ItemCapacityPerSize32") == 0 ||
				strcmp(name, "ItemCapacityPerSize.33") == 0 || strcmp(name, "ItemCapacityPerSize33") == 0 ||
				strcmp(name, "ItemCapacityPerSize.34") == 0 || strcmp(name, "ItemCapacityPerSize34") == 0 ))*/
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
lbepocketCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	lbepocketParseData * pData = (lbepocketParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	  }
}


static void XMLCALL
lbepocketEndElementHandle(void *userData, const XML_Char *name)
{
//UINT16 i=0;
//CHAR8 str1 [40] = "";
lbepocketParseData * pData = (lbepocketParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "POCKETLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "POCKET") == 0)
		{
			pData->curElement = ELEMENT_LIST;
			if ( !onlyLocalizedText )
				LBEPocketType.push_back(pData->curLBEPocket);
			else
				wcscpy(LBEPocketType[pData->curLBEPocket.pIndex].pName,pData->curLBEPocket.pName);
		}
		else if(strcmp(name, "pIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.pIndex   = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "pName") == 0)
		{
			pData->curElement = ELEMENT;
#if 0
			if(MAX_CHAR_DATA_LENGTH >= strlen(pData->szCharData))
				strcpy(pData->curLBEPocket.pName,pData->szCharData);
			else
			{
				strncpy(pData->curLBEPocket.pName,pData->szCharData,MAX_CHAR_DATA_LENGTH);
				pData->curLBEPocket.pName[MAX_CHAR_DATA_LENGTH] = '\0';
			}
#else
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curLBEPocket.pName, sizeof(pData->curLBEPocket.pName)/sizeof(pData->curLBEPocket.pName[0]) );
			pData->curLBEPocket.pName[sizeof(pData->curLBEPocket.pName)/sizeof(pData->curLBEPocket.pName[0]) - 1] = '\0';
#endif
		}
		else if(strcmp(name, "pSilhouette") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.pSilhouette  = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "pType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.pType = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "pRestriction") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.pRestriction = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}	//DBrot: MOLLE added tag
		else if(strcmp(name, "pVolume") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.pVolume  = (UINT8) atol(pData->szCharData);
		}
		//Tais fixed this.
		else
		{	//DBrot: Enhanced parsing routine to avoid excessive load times for larger (~1000) numbers of item sizes
			//-test the prefix if we have an "ItemCapacityPerSize###" tag
			//-get the only significant portion of that tag, the item size in question
			//-convert it to a useable index, check against max_item_size and get the data
			CHAR8 refstr[20] = "ItemCapacityPerSize";
			UINT8 pos = 19;
			if(strncmp(refstr, name, 19) == 0){
				if(name[pos]=='.'){
					pos++;
				}
				CHAR8 index[5] = "\0\0\0\0";
				UINT8 i = 0;
				while(name[pos]!='\0' && i < 5){
					index[i]=name[pos];
					pos++;
					i++;
				}
				UINT16 size = (UINT16) atol(index);
				if(size<=gGameExternalOptions.guiMaxItemSize){
					pData->curElement = ELEMENT;
					pData->curLBEPocket.ItemCapacityPerSize[size] = (UINT8) atol(pData->szCharData);
				}
			}
		}
		/* DBrot: Replaced with a more efficient way
			for (i=0; i<=gGameExternalOptions.guiMaxItemSize;i++)
			{
				sprintf(str1,"ItemCapacityPerSize%d",i);
				if(strcmp(name, str1)==0)
				{
					pData->curElement = ELEMENT;
					pData->curLBEPocket.ItemCapacityPerSize[i] = (UINT8) atol(pData->szCharData);
					break;
				}
			}
			for (i=0; i<=gGameExternalOptions.guiMaxItemSize;i++)
			{
				sprintf(str1,"ItemCapacityPerSize.%d",i);
				if(strcmp(name, str1)==0)
				{
					pData->curElement = ELEMENT;
					pData->curLBEPocket.ItemCapacityPerSize[i] = (UINT8) atol(pData->szCharData);
					break;
				}
			}
		}*/
		/*	JMich - these are no longer needed
		else if(strcmp(name, "ItemCapacityPerSize.0") == 0 || strcmp(name, "ItemCapacityPerSize0") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[0] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.1") == 0 || strcmp(name, "ItemCapacityPerSize1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[1] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.2") == 0 || strcmp(name, "ItemCapacityPerSize2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[2] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.3") == 0 || strcmp(name, "ItemCapacityPerSize3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[3] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.4") == 0 || strcmp(name, "ItemCapacityPerSize4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[4] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.5") == 0 || strcmp(name, "ItemCapacityPerSize5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[5] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.6") == 0 || strcmp(name, "ItemCapacityPerSize6") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[6] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.7") == 0 || strcmp(name, "ItemCapacityPerSize7") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[7] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.8") == 0 || strcmp(name, "ItemCapacityPerSize8") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[8] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.9") == 0 || strcmp(name, "ItemCapacityPerSize9") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[9] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.10") == 0 || strcmp(name, "ItemCapacityPerSize10") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[10] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.11") == 0 || strcmp(name, "ItemCapacityPerSize11") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[11] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.12") == 0 || strcmp(name, "ItemCapacityPerSize12") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[12] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.13") == 0 || strcmp(name, "ItemCapacityPerSize13") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[13] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.14") == 0 || strcmp(name, "ItemCapacityPerSize14") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[14] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.15") == 0 || strcmp(name, "ItemCapacityPerSize15") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[15] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.16") == 0 || strcmp(name, "ItemCapacityPerSize16") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[16] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.17") == 0 || strcmp(name, "ItemCapacityPerSize17") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[17] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.18") == 0 || strcmp(name, "ItemCapacityPerSize18") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[18] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.19") == 0 || strcmp(name, "ItemCapacityPerSize19") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[19] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.20") == 0 || strcmp(name, "ItemCapacityPerSize20") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[20] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.21") == 0 || strcmp(name, "ItemCapacityPerSize21") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[21] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.22") == 0 || strcmp(name, "ItemCapacityPerSize22") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[22] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.23") == 0 || strcmp(name, "ItemCapacityPerSize23") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[23] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.24") == 0 || strcmp(name, "ItemCapacityPerSize24") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[24] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.25") == 0 || strcmp(name, "ItemCapacityPerSize25") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[25] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.26") == 0 || strcmp(name, "ItemCapacityPerSize26") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[26] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.27") == 0 || strcmp(name, "ItemCapacityPerSize27") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[27] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.28") == 0 || strcmp(name, "ItemCapacityPerSize28") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[28] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.29") == 0 || strcmp(name, "ItemCapacityPerSize29") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[29] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.30") == 0 || strcmp(name, "ItemCapacityPerSize30") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[30] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.31") == 0 || strcmp(name, "ItemCapacityPerSize31") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[31] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.32") == 0 || strcmp(name, "ItemCapacityPerSize32") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[32] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.33") == 0 || strcmp(name, "ItemCapacityPerSize33") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[33] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.34") == 0 || strcmp(name, "ItemCapacityPerSize34") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[34] = (UINT8) atol(pData->szCharData);
		}
		*/
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInLBEPocketStats(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	lbepocketParseData pData;

	onlyLocalizedText = localizedVersion;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading LBEPocket.xml" );

	// Open loadbearingequipment file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	//CHRISL: If the file fails to load, then return the value of localizedVersion.  This will allow the program to continue
	//	to load if all we're missing are the localized xml files.
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

	
	XML_SetElementHandler(parser, lbepocketStartElementHandle, lbepocketEndElementHandle);
	XML_SetCharacterDataHandler(parser, lbepocketCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	//pData.curArray = LBEPocketType;
	//pData.maxArraySize = MAXITEMS; 
	
	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Pocket.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
BOOLEAN WriteLBEPocketEquipmentStats()
{
	UINT16 i;
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writelbepocketsstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Pocket out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<POCKETLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<POCKET>\r\n");

			FilePrintf(hFile,"\t\t<pIndex>%d</pIndex>\r\n",								cnt );
			FilePrintf(hFile,"\t\t<pSilhouette>%d</pSilhouette>\r\n",								LBEPocketType[cnt].pSilhouette  );
			FilePrintf(hFile,"\t\t<pType>%d</pType>\r\n",								LBEPocketType[cnt].pType   );
			FilePrintf(hFile,"\t\t<pRestriction>%d</pRestriction>\r\n",						LBEPocketType[cnt].pRestriction	);
			//DBrot: MOLLE
			FilePrintf(hFile,"\t\t<pVolume>%d</pVolume>\r\n",								LBEPocketType[cnt].pVolume  );
			for(i=0;i<=gGameExternalOptions.guiMaxItemSize;i++)		//JMich
			{
				FilePrintf(hFile,"\t\t<ItemCapacityPerSize%d>%d</ItemCapacityPerSize%d>\r\n",								i,LBEPocketType[cnt].ItemCapacityPerSize[i],i   );
			}
			/*	JMich - these are no longer needed
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize0>%d</ItemCapacityPerSize0>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[0]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize1>%d</ItemCapacityPerSize1>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[1]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize2>%d</ItemCapacityPerSize2>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[2]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize3>%d</ItemCapacityPerSize3>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[3]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize4>%d</ItemCapacityPerSize4>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[4]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize5>%d</ItemCapacityPerSize5>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[5]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize6>%d</ItemCapacityPerSize6>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[6]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize7>%d</ItemCapacityPerSize7>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[7]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize8>%d</ItemCapacityPerSize8>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[8]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize9>%d</ItemCapacityPerSize9>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[9]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize10>%d</ItemCapacityPerSize10>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[10]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize11>%d</ItemCapacityPerSize11>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[11]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize12>%d</ItemCapacityPerSize12>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[12]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize13>%d</ItemCapacityPerSize13>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[13]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize14>%d</ItemCapacityPerSize14>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[14]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize15>%d</ItemCapacityPerSize15>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[15]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize16>%d</ItemCapacityPerSize16>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[16]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize17>%d</ItemCapacityPerSize17>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[17]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize18>%d</ItemCapacityPerSize18>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[18]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize19>%d</ItemCapacityPerSize19>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[19]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize20>%d</ItemCapacityPerSize20>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[20]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize21>%d</ItemCapacityPerSize21>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[21]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize22>%d</ItemCapacityPerSize22>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[22]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize23>%d</ItemCapacityPerSize23>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[23]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize24>%d</ItemCapacityPerSize24>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[24]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize25>%d</ItemCapacityPerSize25>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[25]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize26>%d</ItemCapacityPerSize26>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[26]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize27>%d</ItemCapacityPerSize27>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[27]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize28>%d</ItemCapacityPerSize28>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[28]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize29>%d</ItemCapacityPerSize29>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[29]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize30>%d</ItemCapacityPerSize30>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[30]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize31>%d</ItemCapacityPerSize31>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[31]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize32>%d</ItemCapacityPerSize32>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[32]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize33>%d</ItemCapacityPerSize33>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[33]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize34>%d</ItemCapacityPerSize34>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[34]   );
			*/
			FilePrintf(hFile,"\t</POCKET>\r\n");
		}
		FilePrintf(hFile,"</POCKETLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
