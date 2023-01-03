	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "Faces.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	FACE_GEAR_VALUES	curFaceGear;
	FACE_GEAR_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef faceGearParseData;

BOOLEAN FaceGear_TextOnly;

static void XMLCALL
faceGearStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	faceGearParseData * pData = (faceGearParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "FACE_GEAR") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;
			
			memset(pData->curArray,0,sizeof(FACE_GEAR_VALUES)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ITEM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;
			
			memset(&pData->curFaceGear,0,sizeof(FACE_GEAR_VALUES));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "Type") == 0 ||
				strcmp(name, "szFile") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
faceGearCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	faceGearParseData * pData = (faceGearParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
faceGearEndElementHandle(void *userData, const XML_Char *name)
{

	char temp;

	faceGearParseData * pData = (faceGearParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "FACE_GEAR") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "ITEM") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			
			if(pData->curFaceGear.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curFaceGear.uiIndex] = pData->curFaceGear;
			}
			
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curFaceGear.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "Type") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curFaceGear.Type	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "szFile") == 0 )
		{
			pData->curElement = ELEMENT;
	
			if(MAX_FACE_GERA_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curFaceGear.szFile,pData->szCharData);
			else
			{
				strncpy(pData->curFaceGear.szFile,pData->szCharData,MAX_FACE_GERA_CHARS);
				pData->curFaceGear.szFile[MAX_FACE_GERA_CHARS-1] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_FACE_GERA_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curFaceGear.szFile[i] = temp;
			}
			
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInFaceGear( FACE_GEAR_VALUES *pFaceGear, STR fileName ) //, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	faceGearParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading FaceGear.xml" );
	
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


	XML_SetElementHandler(parser, faceGearStartElementHandle, faceGearEndElementHandle);
	XML_SetCharacterDataHandler(parser, faceGearCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = pFaceGear;
	pData.maxArraySize = MAXITEMS;
	
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in FaceGear.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteFaceGear()
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeFaceGear");
	HWFILE		hFile;

	hFile = FileOpen( "TABLEDATA\\FaceGear out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<FACE_GEAR>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{
			FilePrintf(hFile,"\t<ITEM>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",cnt);
			FilePrintf(hFile,"\t\t<Type>%d</Type>\r\n",			zNewFaceGear[cnt].Type);
			FilePrintf(hFile,"\t\t<szFile></szFile>\r\n");
			FilePrintf(hFile,"\t</ITEM>\r\n");
		}
		FilePrintf(hFile,"</FACE_GEAR>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
