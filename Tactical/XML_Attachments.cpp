	#include "sgp.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "gamesettings.h"
	#include "XML.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	UINT16			curAttachment[4];
	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef attachmentParseData;

static void XMLCALL
attachmentStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	attachmentParseData * pData = (attachmentParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ATTACHMENTLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ATTACHMENT") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3,"AttachmentStartElementHandle: setting memory for curAttachment");

			memset(&pData->curAttachment,0,sizeof(UINT16[2]));

			pData->maxReadDepth++; //we are not skipping this element
			pData->curIndex++;
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "attachmentIndex") == 0 ||
				strcmp(name, "APCost") == 0 ||
				strcmp(name, "NASOnly") == 0 ||
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
attachmentCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	attachmentParseData * pData = (attachmentParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
attachmentEndElementHandle(void *userData, const XML_Char *name)
{
	attachmentParseData * pData = (attachmentParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "ATTACHMENTLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "ATTACHMENT") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curIndex < pData->maxArraySize)
			{
				//DebugMsg(TOPIC_JA2, DBG_LEVEL_3,"AttachmentStartElementHandle: writing attachment to array");
				Attachment[pData->curIndex][0] = pData->curAttachment[0]; //write the attachment into the table
				Attachment[pData->curIndex][1] = pData->curAttachment[1];
				Attachment[pData->curIndex][2] = pData->curAttachment[2];
			}
		}
		else if(strcmp(name, "attachmentIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachment[0] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "itemIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachment[1] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "APCost") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachment[2] = (UINT16) atol(pData->szCharData);
			pData->curAttachment[2] = (UINT16)DynamicAdjustAPConstants(pData->curAttachment[2], pData->curAttachment[2]);
		}
		else if(strcmp(name, "NASOnly") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachment[3] = (UINT16) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInAttachmentStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	attachmentParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Attachments.xml" );

	// Open attachments file
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


	XML_SetElementHandler(parser, attachmentStartElementHandle, attachmentEndElementHandle);
	XML_SetCharacterDataHandler(parser, attachmentCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXATTACHMENTS;
	pData.curIndex = -1;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Attachments.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}
BOOLEAN WriteAttachmentStats()
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Attachments out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<ATTACHMENTLIST>\r\n");
		for(cnt = 0;cnt < MAXATTACHMENTS;cnt++)
		{
			FilePrintf(hFile,"\t<ATTACHMENT>\r\n");

			FilePrintf(hFile,"\t\t<attachmentIndex>%d</attachmentIndex>\r\n",						Attachment[cnt][0]);
			FilePrintf(hFile,"\t\t<itemIndex>%d</itemIndex>\r\n",							Attachment[cnt][1]);
			FilePrintf(hFile,"\t\t<APCost>%d</APCost>\r\n",							Attachment[cnt][2]);
			FilePrintf(hFile,"\t\t<NASOnly>%d</NASOnly>\r\n",							Attachment[cnt][3]);

			FilePrintf(hFile,"\t</ATTACHMENT>\r\n");
		}
		FilePrintf(hFile,"</ATTACHMENTLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
