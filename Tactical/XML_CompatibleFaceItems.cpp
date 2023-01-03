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

	UINT16			curCompatibleFaceItem[2];
	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef compatiblefaceitemParseData;

static void XMLCALL
compatiblefaceitemStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	compatiblefaceitemParseData * pData = (compatiblefaceitemParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "COMPATIBLEFACEITEMLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "COMPATIBLEFACEITEM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3,"CompatibleFaceItemStartElementHandle: setting memory for curCompatibleFaceItem");

			memset(&pData->curCompatibleFaceItem,0,sizeof(UINT16[2]));

			pData->maxReadDepth++; //we are not skipping this element
			pData->curIndex++;
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "compatiblefaceitemIndex") == 0 ||
				strcmp(name, "itemIndex") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
compatiblefaceitemCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	compatiblefaceitemParseData * pData = (compatiblefaceitemParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
compatiblefaceitemEndElementHandle(void *userData, const XML_Char *name)
{
	compatiblefaceitemParseData * pData = (compatiblefaceitemParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "COMPATIBLEFACEITEMLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "COMPATIBLEFACEITEM") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curIndex < pData->maxArraySize)
			{
				//DebugMsg(TOPIC_JA2, DBG_LEVEL_3,"CompatibleFaceItemStartElementHandle: writing compatiblefaceitem to array");
				CompatibleFaceItems[pData->curIndex][0] = pData->curCompatibleFaceItem[0]; //write the compatiblefaceitem into the table
				CompatibleFaceItems[pData->curIndex][1] = pData->curCompatibleFaceItem[1];
			}
		}
		else if(strcmp(name, "compatiblefaceitemIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curCompatibleFaceItem[0] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "itemIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curCompatibleFaceItem[1] = (UINT16) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInCompatibleFaceItemStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	compatiblefaceitemParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading CompatibleFaceItems.xml" );

	// Open compatiblefaceitems file
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


	XML_SetElementHandler(parser, compatiblefaceitemStartElementHandle, compatiblefaceitemEndElementHandle);
	XML_SetCharacterDataHandler(parser, compatiblefaceitemCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXITEMS;
	pData.curIndex = -1;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in CompatibleFaceItems.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}
BOOLEAN WriteCompatibleFaceItemStats()
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\CompatibleFaceItems out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<COMPATIBLEFACEITEMLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{
			FilePrintf(hFile,"\t<COMPATIBLEFACEITEM>\r\n");

			FilePrintf(hFile,"\t\t<compatiblefaceitemIndex>%d</compatiblefaceitemIndex>\r\n",						CompatibleFaceItems[cnt][0]);
			FilePrintf(hFile,"\t\t<itemIndex>%d</itemIndex>\r\n",							CompatibleFaceItems[cnt][1]);

			FilePrintf(hFile,"\t</COMPATIBLEFACEITEM>\r\n");
		}
		FilePrintf(hFile,"</COMPATIBLEFACEITEMLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
