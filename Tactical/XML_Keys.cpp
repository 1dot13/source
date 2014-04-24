#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "Keys.h"
#endif


struct
{
	PARSE_STAGE	curElement;

	CHAR8	szCharData[MAX_CHAR_DATA_LENGTH+1];
	KEY		curKey;
	KEY *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef keyParseData;

UINT16 num_found_key = 0;	// the correct number is set on reading the xml

static void XMLCALL
keyStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	keyParseData * pData = (keyParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "KEYS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "KEY") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "ubIndex") == 0 ||
				strcmp(name, "usItem") == 0 ) )
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
keyCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	keyParseData * pData = (keyParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}

static void XMLCALL
keyEndElementHandle(void *userData, const XML_Char *name)
{
	keyParseData * pData = (keyParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "KEYS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "KEY") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if(num_found_key < pData->maxArraySize)
			{

				pData->curArray[pData->curIndex] = pData->curKey;

				num_found_key++;
			}
		}
		else if(strcmp(name, "ubIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIndex	= (UINT32) atol(pData->szCharData);

			memset(&pData->curKey, 0, sizeof(KEY));

		}
		else if(strcmp(name, "usItem") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curKey.usItem = (UINT16) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInKeys(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	keyParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Keys.xml" );
		
	// Open file
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


	XML_SetElementHandler(parser, keyStartElementHandle, keyEndElementHandle);
	XML_SetCharacterDataHandler(parser, keyCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = KeyTable;
	pData.maxArraySize = NUM_KEYS;

	XML_SetUserData(parser, &pData);
	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Keys.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}


BOOLEAN WriteKeys( STR fileName )
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<KEYS>\r\n");
		for(cnt = 0; cnt < NUM_KEYS; ++cnt)
		{
			FilePrintf(hFile,"\t<KEY>\r\n");

			FilePrintf(hFile,"\t\t<ubIndex>%d</ubIndex>\r\n",	cnt);
			FilePrintf(hFile,"\t\t<usItem>%s</usItem>\r\n",	KeyTable[cnt].usItem);

			FilePrintf(hFile,"\t</KEY>\r\n");
		}
		FilePrintf(hFile,"</KEYS>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
