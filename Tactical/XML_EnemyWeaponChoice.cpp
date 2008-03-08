#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Inventory Choosing.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	ARMY_GUN_CHOICE_TYPE		curExtendedArmyGunChoices;
	ARMY_GUN_CHOICE_TYPE *	curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef extendedarmygunchoicesParseData;

static void XMLCALL
extendedarmygunchoicesStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	extendedarmygunchoicesParseData * pData = (extendedarmygunchoicesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ENEMYGUNCHOICESLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(ARMY_GUN_CHOICE_TYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ENEMYGUNCHOICES") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curExtendedArmyGunChoices,0,sizeof(ARMY_GUN_CHOICE_TYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "ubChoices") == 0 ||
				strcmp(name, "bItemNo1") == 0 ||
				strcmp(name, "bItemNo2") == 0 ||
				strcmp(name, "bItemNo3") == 0 ||
				strcmp(name, "bItemNo4") == 0 ||
				strcmp(name, "bItemNo5") == 0 ||
				strcmp(name, "bItemNo6") == 0 ||
				strcmp(name, "bItemNo7") == 0 ||
				strcmp(name, "bItemNo8") == 0 ||
				strcmp(name, "bItemNo9") == 0 ||
				strcmp(name, "bItemNo10") == 0 ||
				strcmp(name, "bItemNo11") == 0 ||
				strcmp(name, "bItemNo12") == 0 ||
				strcmp(name, "bItemNo13") == 0 ||
				strcmp(name, "bItemNo14") == 0 ||
				strcmp(name, "bItemNo15") == 0 ||
				strcmp(name, "bItemNo16") == 0 ||
				strcmp(name, "bItemNo17") == 0 ||
				strcmp(name, "bItemNo18") == 0 ||
				strcmp(name, "bItemNo19") == 0 ||
				strcmp(name, "bItemNo20") == 0 ||
				strcmp(name, "bItemNo21") == 0 ||
				strcmp(name, "bItemNo22") == 0 ||
				strcmp(name, "bItemNo23") == 0 ||
				strcmp(name, "bItemNo24") == 0 ||
				strcmp(name, "bItemNo25") == 0 ||
				strcmp(name, "bItemNo26") == 0 ||
				strcmp(name, "bItemNo27") == 0 ||
				strcmp(name, "bItemNo28") == 0 ||
				strcmp(name, "bItemNo29") == 0 ||
				strcmp(name, "bItemNo30") == 0 ||
				strcmp(name, "bItemNo31") == 0 ||
				strcmp(name, "bItemNo32") == 0 ||
				strcmp(name, "bItemNo33") == 0 ||
				strcmp(name, "bItemNo34") == 0 ||
				strcmp(name, "bItemNo35") == 0 ||
				strcmp(name, "bItemNo36") == 0 ||
				strcmp(name, "bItemNo37") == 0 ||
				strcmp(name, "bItemNo38") == 0 ||
				strcmp(name, "bItemNo39") == 0 ||
				strcmp(name, "bItemNo40") == 0 ||
				strcmp(name, "bItemNo41") == 0 ||
				strcmp(name, "bItemNo42") == 0 ||
				strcmp(name, "bItemNo43") == 0 ||
				strcmp(name, "bItemNo44") == 0 ||
				strcmp(name, "bItemNo45") == 0 ||
				strcmp(name, "bItemNo46") == 0 ||
				strcmp(name, "bItemNo47") == 0 ||
				strcmp(name, "bItemNo48") == 0 ||
				strcmp(name, "bItemNo49") == 0 ||
				strcmp(name, "bItemNo50") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
extendedarmygunchoicesCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	extendedarmygunchoicesParseData * pData = (extendedarmygunchoicesParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
extendedarmygunchoicesEndElementHandle(void *userData, const XML_Char *name)
{
	extendedarmygunchoicesParseData * pData = (extendedarmygunchoicesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "ENEMYGUNCHOICESLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "ENEMYGUNCHOICES") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curExtendedArmyGunChoices.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curExtendedArmyGunChoices.uiIndex] = pData->curExtendedArmyGunChoices; //write the extendedarmygunchoices into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.uiIndex	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubChoices") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.ubChoices	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[0] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[1] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[2] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[3] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[4] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo6") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[5] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo7") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[6] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo8") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[7] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo9") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[8] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo10") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[9] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo11") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[10] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo12") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[11] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo13") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[12] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo14") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[13] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo15") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[14] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo16") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[15] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo17") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[16] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo18") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[17] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo19") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[18] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo20") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[19] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo21") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[20] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo22") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[21] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo23") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[22] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo24") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[23] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo25") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[24] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo26") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[25] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo27") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[26] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo28") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[27] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo29") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[28] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo30") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[29] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo31") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[30] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo32") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[31] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo33") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[32] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo34") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[33] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo35") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[34] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo36") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[35] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo37") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[36] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo38") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[37] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo39") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[38] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo40") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[39] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo41") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[40] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo42") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[41] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo43") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[42] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo44") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[43] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo45") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[44] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo46") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[45] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo47") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[46] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo48") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[47] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo49") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[48] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bItemNo50") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExtendedArmyGunChoices.bItemNo[49] = (INT16) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInExtendedArmyGunChoicesStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	extendedarmygunchoicesParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading EnemyGunChoicess.xml" );

	// Open extendedarmygunchoices file
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


	XML_SetElementHandler(parser, extendedarmygunchoicesStartElementHandle, extendedarmygunchoicesEndElementHandle);
	XML_SetCharacterDataHandler(parser, extendedarmygunchoicesCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = gExtendedArmyGunChoices;
	pData.maxArraySize = ARMY_GUN_LEVELS;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in EnemyGunChoicess.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
BOOLEAN WriteExtendedArmyGunChoicesStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeextendedarmygunchoicesstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\EnemyGunChoices out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<ENEMYGUNCHOICESLIST>\r\n");
		for(cnt = 0;cnt < ARMY_GUN_LEVELS;cnt++)
		{

			FilePrintf(hFile,"\t<ENEMYGUNCHOICES>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",								cnt );
			FilePrintf(hFile,"\t\t<ubChoices>%d</ubChoices>\r\n",								gExtendedArmyGunChoices[cnt].ubChoices	);
			for (int i=0;i<50;i++)
				FilePrintf(hFile,"\t\t<bItemNo%d>%d</bItemNo%d>\r\n",i+1,gExtendedArmyGunChoices[cnt].bItemNo[i],i+1	);


			FilePrintf(hFile,"\t</ENEMYGUNCHOICES>\r\n");
		}
		FilePrintf(hFile,"</ENEMYGUNCHOICESLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
