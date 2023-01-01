	#include "sgp.h"
	#include "overhead.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Item Types.h"

RANDOM_ITEM_CHOICE_TYPE gRandomItemClass[RANDOM_ITEM_MAX_CATEGORIES];

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	RANDOM_ITEM_CHOICE_TYPE		curRandomItemClass;
	RANDOM_ITEM_CHOICE_TYPE *	curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef randomitemParseData;

static void XMLCALL
randomitemStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	randomitemParseData * pData = (randomitemParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "RANDOMITEMLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(RANDOM_ITEM_CHOICE_TYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "RANDOMITEM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curRandomItemClass,0,sizeof(RANDOM_ITEM_CHOICE_TYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "szName") == 0 ||
				strcmp(name, "randomitem1") == 0 ||
				strcmp(name, "randomitem2") == 0 ||
				strcmp(name, "randomitem3") == 0 ||
				strcmp(name, "randomitem4") == 0 ||
				strcmp(name, "randomitem5") == 0 ||
				strcmp(name, "randomitem6") == 0 ||
				strcmp(name, "randomitem7") == 0 ||
				strcmp(name, "randomitem8") == 0 ||
				strcmp(name, "randomitem9") == 0 ||
				strcmp(name, "randomitem10") == 0 ||
				strcmp(name, "item1") == 0 ||
				strcmp(name, "item2") == 0 ||
				strcmp(name, "item3") == 0 ||
				strcmp(name, "item4") == 0 ||
				strcmp(name, "item5") == 0 ||
				strcmp(name, "item6") == 0 ||
				strcmp(name, "item7") == 0 ||
				strcmp(name, "item8") == 0 ||
				strcmp(name, "item9") == 0 ||
				strcmp(name, "item10") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
randomitemCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	randomitemParseData * pData = (randomitemParseData *)userData;

	if ( (pData->currentDepth <= pData->maxReadDepth) &&	(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH) )
	{
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
randomitemEndElementHandle(void *userData, const XML_Char *name)
{
	randomitemParseData * pData = (randomitemParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "RANDOMITEMLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "RANDOMITEM") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curRandomItemClass.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curRandomItemClass.uiIndex] = pData->curRandomItemClass; //write the randomitem into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "szName") == 0)
		{
			pData->curElement = ELEMENT;
			// not needed, but it's there for informational purposes
		}
		else if(strcmp(name, "randomitem1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.randomitem[0] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "randomitem2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.randomitem[1] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "randomitem3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.randomitem[2] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "randomitem4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.randomitem[3] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "randomitem5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.randomitem[4] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "randomitem6") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.randomitem[5] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "randomitem7") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.randomitem[6] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "randomitem8") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.randomitem[7] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "randomitem9") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.randomitem[8] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "randomitem10") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.randomitem[9] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "item1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.item[0] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "item2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.item[1] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "item3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.item[2] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "item4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.item[3] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "item5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.item[4] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "item6") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.item[5] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "item7") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.item[6] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "item8") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.item[7] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "item9") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.item[8] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "item10") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomItemClass.item[9] = (UINT16) atol(pData->szCharData);
		}
		
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInRandomItemStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	randomitemParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading RandomItem.xml" );

	// Open randomitem file
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


	XML_SetElementHandler(parser, randomitemStartElementHandle, randomitemEndElementHandle);
	XML_SetCharacterDataHandler(parser, randomitemCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = gRandomItemClass;
	pData.maxArraySize = RANDOM_ITEM_MAX_CATEGORIES;

	XML_SetUserData(parser, &pData);
	
	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in RandomItem.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);
	
	XML_ParserFree(parser);

	return( TRUE );
}

BOOLEAN WriteRandomItemStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writerandomitemstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\RandomItem out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<RANDOMITEMLIST>\r\n");
		for(cnt = 0; cnt < 200; ++cnt)
		{
			FilePrintf(hFile,"\t<RANDOMITEM>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",							cnt );
			for (int i=0; i<10; ++i)
				FilePrintf(hFile,"\t\t<randomitem%d>%d</randomitem%d>\r\n",i+1,			gRandomItemClass[cnt].randomitem[i],i+1	);

			for (int i=0; i<10; ++i)
				FilePrintf(hFile,"\t\t<item%d>%d</item%d>\r\n",i+1,						gRandomItemClass[cnt].item[i],i+1 );

			FilePrintf(hFile,"\t</RANDOMITEM>\r\n");
		}
		FilePrintf(hFile,"</RANDOMITEMLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
