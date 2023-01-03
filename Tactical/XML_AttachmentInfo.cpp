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
	AttachmentInfoStruct		curAttachmentInfo;
	AttachmentInfoStruct *	curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef attachmentinfoParseData;

static void XMLCALL
attachmentinfoStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	attachmentinfoParseData * pData = (attachmentinfoParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ATTACHMENTINFOLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(AttachmentInfoStruct)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ATTACHMENTINFO") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curAttachmentInfo,0,sizeof(AttachmentInfoStruct));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "usItem") == 0 ||
				strcmp(name, "uiItemClass") == 0 ||
				strcmp(name, "bAttachmentSkillCheck") == 0 ||
				strcmp(name, "bAttachmentSkillCheckMod") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
attachmentinfoCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	attachmentinfoParseData * pData = (attachmentinfoParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
attachmentinfoEndElementHandle(void *userData, const XML_Char *name)
{
	attachmentinfoParseData * pData = (attachmentinfoParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "ATTACHMENTINFOLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "ATTACHMENTINFO") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curAttachmentInfo.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curAttachmentInfo.uiIndex] = pData->curAttachmentInfo; //write the attachmentinfo into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentInfo.uiIndex	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "usItem") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentInfo.usItem	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "uiItemClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentInfo.uiItemClass= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bAttachmentSkillCheck") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentInfo.bAttachmentSkillCheck	= (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bAttachmentSkillCheckMod") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentInfo.bAttachmentSkillCheckMod	= (INT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInAttachmentInfoStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	attachmentinfoParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading AttachmentInfo.xml" );

	// Open attachmentinfo file
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


	XML_SetElementHandler(parser, attachmentinfoStartElementHandle, attachmentinfoEndElementHandle);
	XML_SetCharacterDataHandler(parser, attachmentinfoCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = AttachmentInfo;
	pData.maxArraySize = MAXITEMS;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in AttachmentInfo.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
BOOLEAN WriteAttachmentInfoStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeattachmentinfostats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\AttachmentInfo out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<ATTACHMENTINFOLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<ATTACHMENTINFO>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",								cnt );
			FilePrintf(hFile,"\t\t<usItem>%d</usItem>\r\n",								AttachmentInfo[cnt].usItem );
			FilePrintf(hFile,"\t\t<uiItemClass>%d</uiItemClass>\r\n",								AttachmentInfo[cnt].uiItemClass	);
			FilePrintf(hFile,"\t\t<bAttachmentSkillCheck>%d</bAttachmentSkillCheck>\r\n",								AttachmentInfo[cnt].bAttachmentSkillCheck	);
			FilePrintf(hFile,"\t\t<bAttachmentSkillCheckMod>%d</bAttachmentSkillCheckMod>\r\n",								AttachmentInfo[cnt].bAttachmentSkillCheckMod	);

			FilePrintf(hFile,"\t</ATTACHMENTINFO>\r\n");
		}
		FilePrintf(hFile,"</ATTACHMENTINFOLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
