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

	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	LBETYPE			curLBE;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef lbeParseData;

static void XMLCALL 
lbeStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	lbeParseData * pData = (lbeParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "LOADBEARINGEQUIPMENTLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "LOADBEARINGEQUIPMENT") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->curLBE = LBETYPE();

			pData->maxReadDepth++; //we are not skipping this element
		}//DBrot: MOLLE added tag
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "lbeIndex") == 0 ||
				strcmp(name, "lbeClass") == 0 ||
				strcmp(name, "lbeCombo") == 0 ||
				strcmp(name, "lbeFilledSize") == 0 ||
				strcmp(name, "lbeAvailableVolume") == 0 ||
				strcmp(name, "lbePocketIndex.1") == 0 || strcmp(name, "lbePocketIndex1") == 0 ||
				strcmp(name, "lbePocketIndex.2") == 0 || strcmp(name, "lbePocketIndex2") == 0 ||
				strcmp(name, "lbePocketIndex.3") == 0 || strcmp(name, "lbePocketIndex3") == 0 ||
				strcmp(name, "lbePocketIndex.4") == 0 || strcmp(name, "lbePocketIndex4") == 0 ||
				strcmp(name, "lbePocketIndex.5") == 0 || strcmp(name, "lbePocketIndex5") == 0 ||
				strcmp(name, "lbePocketIndex.6") == 0 || strcmp(name, "lbePocketIndex6") == 0 ||
				strcmp(name, "lbePocketIndex.7") == 0 || strcmp(name, "lbePocketIndex7") == 0 ||
				strcmp(name, "lbePocketIndex.8") == 0 || strcmp(name, "lbePocketIndex8") == 0 ||
				strcmp(name, "lbePocketIndex.9") == 0 || strcmp(name, "lbePocketIndex9") == 0 ||
				strcmp(name, "lbePocketIndex.10") == 0 || strcmp(name, "lbePocketIndex10") == 0 ||
				strcmp(name, "lbePocketIndex.11") == 0 || strcmp(name, "lbePocketIndex11") == 0 ||
				strcmp(name, "lbePocketIndex.12") == 0 || strcmp(name, "lbePocketIndex12") == 0 ||

				strcmp(name, "lbePocketsAvailable") == 0))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
lbeCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	lbeParseData * pData = (lbeParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	  }
}


static void XMLCALL
lbeEndElementHandle(void *userData, const XML_Char *name)
{
	lbeParseData * pData = (lbeParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "LOADBEARINGEQUIPMENTLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "LOADBEARINGEQUIPMENT") == 0)
		{
			pData->curElement = ELEMENT_LIST;
			LoadBearingEquipment.push_back(pData->curLBE);
		}
		else if(strcmp(name, "lbeIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbeIndex   = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbeClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbeClass  = (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbeCombo") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbeCombo  = (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbeFilledSize") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbeFilledSize  = (UINT8) atol(pData->szCharData);
		}
		//DBrot: MOLLE
		else if(strcmp(name, "lbeAvailableVolume") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbeAvailableVolume  = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.1") == 0 || strcmp(name, "lbePocketIndex1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[0] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.2") == 0 || strcmp(name, "lbePocketIndex2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[1] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.3") == 0 || strcmp(name, "lbePocketIndex3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[2] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.4") == 0 || strcmp(name, "lbePocketIndex4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[3] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.5") == 0 || strcmp(name, "lbePocketIndex5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[4] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.6") == 0 || strcmp(name, "lbePocketIndex6") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[5] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.7") == 0 || strcmp(name, "lbePocketIndex7") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[6] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.8") == 0 || strcmp(name, "lbePocketIndex8") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[7] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.9") == 0 || strcmp(name, "lbePocketIndex9") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[8] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.10") == 0 || strcmp(name, "lbePocketIndex10") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[9] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.11") == 0 || strcmp(name, "lbePocketIndex11") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[10] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "lbePocketIndex.12") == 0 || strcmp(name, "lbePocketIndex12") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketIndex[11] = (UINT8) atol(pData->szCharData);
		}
		
		
		else if(strcmp(name, "lbePocketsAvailable") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBE.lbePocketsAvailable = (UINT16) atol(pData->szCharData);
		}
		
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInlbeStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	lbeParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading LoadBearingEquipment.xml" );

	// Open loadbearingequipment file
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

	
	XML_SetElementHandler(parser, lbeStartElementHandle, lbeEndElementHandle);
	XML_SetCharacterDataHandler(parser, lbeCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	//pData.curArray = LoadBearingEquipment;
	//pData.maxArraySize = MAXITEMS; 
	
	XML_SetUserData(parser, &pData);

	LoadBearingEquipment.clear();
	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in LoadBearingEquipment.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
BOOLEAN WritelbeEquipmentStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writelbestats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\LoadBearingEquipment out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<LOADBEARINGEQUIPMENTLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{
			//DBrot: MOLLE
			FilePrintf(hFile,"\t<LOADBEARINGEQUIPMENT>\r\n");

			FilePrintf(hFile,"\t\t<lbeIndex>%d</lbeIndex>\r\n",								cnt );
			FilePrintf(hFile,"\t\t<lbeClass>%d</lbeClass>\r\n",								LoadBearingEquipment[cnt].lbeClass  );
			FilePrintf(hFile,"\t\t<lbeCombo>%d</lbeCombo>\r\n",								LoadBearingEquipment[cnt].lbeCombo  );
			FilePrintf(hFile,"\t\t<lbeFilledSize>%d</lbeFilledSize>\r\n",								LoadBearingEquipment[cnt].lbeFilledSize  );
			FilePrintf(hFile,"\t\t<lbeAvailableVolume>%d</lbeAvailableVolume>\r\n",								LoadBearingEquipment[cnt].lbeAvailableVolume  );
			FilePrintf(hFile,"\t\t<lbePocketIndex1>%d</lbePocketIndex1>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[0]   );
			FilePrintf(hFile,"\t\t<lbePocketIndex2>%d</lbePocketIndex2>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[1]   );
			FilePrintf(hFile,"\t\t<lbePocketIndex3>%d</lbePocketIndex3>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[2]   );
			FilePrintf(hFile,"\t\t<lbePocketIndex4>%d</lbePocketIndex4>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[3]   );
			FilePrintf(hFile,"\t\t<lbePocketIndex5>%d</lbePocketIndex5>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[4]   );
			FilePrintf(hFile,"\t\t<lbePocketIndex6>%d</lbePocketIndex6>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[5]   );
			FilePrintf(hFile,"\t\t<lbePocketIndex7>%d</lbePocketIndex7>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[6]   );
			FilePrintf(hFile,"\t\t<lbePocketIndex8>%d</lbePocketIndex8>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[7]   );
			FilePrintf(hFile,"\t\t<lbePocketIndex9>%d</lbePocketIndex9>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[8]   );
			FilePrintf(hFile,"\t\t<lbePocketIndex10>%d</lbePocketIndex10>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[9]   );
			FilePrintf(hFile,"\t\t<lbePocketIndex11>%d</lbePocketIndex11>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[10]   );
			FilePrintf(hFile,"\t\t<lbePocketIndex12>%d</lbePocketIndex12>\r\n",								LoadBearingEquipment[cnt].lbePocketIndex[11]   );

			FilePrintf(hFile,"\t\t<lbePocketsAvailable>%d</lbePocketsAvailable>\r\n",								LoadBearingEquipment[cnt].lbePocketsAvailable);

			FilePrintf(hFile,"\t</LOADBEARINGEQUIPMENT>\r\n");
		}
		FilePrintf(hFile,"</LOADBEARINGEQUIPMENTLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
