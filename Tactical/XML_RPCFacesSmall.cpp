#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "faces.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	RPC_SMALL_FACE_VALUES	curSmallFaces;
	RPC_SMALL_FACE_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef smallFacesParseData;

static void XMLCALL
smallFacesStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	smallFacesParseData * pData = (smallFacesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "SMALLFACE") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(RPC_SMALL_FACE_VALUES)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "FACE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curSmallFaces,0,sizeof(RPC_SMALL_FACE_VALUES));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "FaceIndex") == 0 ||
				strcmp(name, "EyesX") == 0 ||
				strcmp(name, "EyesY") == 0 ||
				strcmp(name, "MouthX") == 0 ||
				strcmp(name, "MouthY") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
smallFacesCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	smallFacesParseData * pData = (smallFacesParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
smallFacesEndElementHandle(void *userData, const XML_Char *name)
{
	smallFacesParseData * pData = (smallFacesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "SMALLFACE") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "FACE") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curSmallFaces.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curSmallFaces.uiIndex] = pData->curSmallFaces; //write the small faces into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSmallFaces.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "FaceIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSmallFaces.FaceIndex	= (UINT16) atol(pData->szCharData);
		}	
		else if(strcmp(name, "EyesX") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSmallFaces.bEyesX	= (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "EyesY") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSmallFaces.bEyesY	=  (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "MouthX") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSmallFaces.bMouthX	= (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "MouthY") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSmallFaces.bMouthY	=  (INT8) atol(pData->szCharData);
		}


		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInSmallFacesStats(RPC_SMALL_FACE_VALUES *pSmallFaces, STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	smallFacesParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading SmallFaces.xml" );

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


	XML_SetElementHandler(parser, smallFacesStartElementHandle, smallFacesEndElementHandle);
	XML_SetCharacterDataHandler(parser, smallFacesCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = pSmallFaces;
	pData.maxArraySize = 200;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in RPCFacesSmall.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteSmallFacesStats(RPC_SMALL_FACE_VALUES *pSmallFaces, STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<SMALLFACE>\r\n");
		for(cnt = 0;cnt < 201;cnt++)
		{

			FilePrintf(hFile,"\t<FACE>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<FaceIndex>%d</FaceIndex>\r\n", pSmallFaces[cnt].FaceIndex);
			FilePrintf(hFile,"\t\t<EyesX>%d</EyesX>\r\n", pSmallFaces[cnt].bEyesX);
			FilePrintf(hFile,"\t\t<EyesY>%d</EyesY>\r\n", pSmallFaces[cnt].bEyesY);
			FilePrintf(hFile,"\t\t<MouthX>%d</MouthX>\r\n", pSmallFaces[cnt].bMouthX);
			FilePrintf(hFile,"\t\t<MouthY>%d</MouthY>\r\n", pSmallFaces[cnt].bMouthY);
			FilePrintf(hFile,"\t</FACE>\r\n");
		}
		FilePrintf(hFile,"</SMALLFACE>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}