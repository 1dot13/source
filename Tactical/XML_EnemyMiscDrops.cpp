#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "EnemyItemDrops.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	MISC_DROPS	curMiscDrop;
	MISC_DROPS *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef miscDropParseData;

static void XMLCALL
miscDropStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	miscDropParseData * pData = (miscDropParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "MISCDROPLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(MISC_DROPS)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "DROPITEM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curMiscDrop,0,sizeof(MISC_DROPS));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "usItemClass") == 0 ||
				strcmp(name, "ubEnemyDropRate") == 0 ||
				strcmp(name, "ubMilitiaDropRate") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
miscDropCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	miscDropParseData * pData = (miscDropParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
miscDropEndElementHandle(void *userData, const XML_Char *name)
{
	miscDropParseData * pData = (miscDropParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "MISCDROPLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "DROPITEM") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curMiscDrop.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curMiscDrop.uiIndex] = pData->curMiscDrop; //write the inventory into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMiscDrop.uiIndex	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "usItemClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMiscDrop.usItemClass	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubEnemyDropRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMiscDrop.ubEnemyDropRate	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubMilitiaDropRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMiscDrop.ubMilitiaDropRate	= (UINT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInEnemyMiscDropsStats(MISC_DROPS *pEnemyMiscDrops, STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	miscDropParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading EnemyMiscDrops.xml" );

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


	XML_SetElementHandler(parser, miscDropStartElementHandle, miscDropEndElementHandle);
	XML_SetCharacterDataHandler(parser, miscDropCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = pEnemyMiscDrops;
	pData.maxArraySize = MAX_DROP_ITEMS;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in EnemyMiscDrops.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteEnemyMiscDropsStats(MISC_DROPS *pEnemyMiscDrops, STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<MISCDROPLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<DROPITEM>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<usItemClass>%d</usItemClass>\r\n", pEnemyMiscDrops[cnt].usItemClass);
			FilePrintf(hFile,"\t\t<ubEnemyDropRate>%d</ubEnemyDropRate>\r\n", pEnemyMiscDrops[cnt].ubEnemyDropRate);
			FilePrintf(hFile,"\t\t<ubMilitiaDropRate>%d</ubMilitiaDropRate>\r\n", pEnemyMiscDrops[cnt].ubMilitiaDropRate);

			FilePrintf(hFile,"\t</DROPITEM>\r\n");
		}
		FilePrintf(hFile,"</MISCDROPLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
