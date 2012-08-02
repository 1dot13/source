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

UINT16 LAST_SLOT_INDEX = 0;

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	AttachmentSlotStruct	curAttachmentSlot;
	AttachmentSlotStruct *	curArray;

	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef attachmentslotParseData;

static void XMLCALL
attachmentslotStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	attachmentslotParseData * pData = (attachmentslotParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ATTACHMENTSLOTLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(AttachmentSlotStruct)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ATTACHMENTSLOT") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curAttachmentSlot,0,sizeof(AttachmentSlotStruct));

			pData->maxReadDepth++; //we are not skipping this element
		}
		//DBrot: MOLLE new tag
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiSlotIndex") == 0 ||
				strcmp(name, "szSlotName") == 0 ||
				strcmp(name, "nasAttachmentClass") == 0 ||
				strcmp(name, "nasLayoutClass") == 0 ||
				strcmp(name, "usDescPanelPosX") == 0 ||
				strcmp(name, "usDescPanelPosY") == 0 ||
				strcmp(name, "fMultiShot") == 0 ||
				strcmp(name, "ubPocketMapping") == 0 ||
				strcmp(name, "fBigSlot") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
attachmentslotCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	attachmentslotParseData * pData = (attachmentslotParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
attachmentslotEndElementHandle(void *userData, const XML_Char *name)
{
	attachmentslotParseData * pData = (attachmentslotParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "ATTACHMENTSLOTLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "ATTACHMENTSLOT") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curAttachmentSlot.uiSlotIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curAttachmentSlot.uiSlotIndex] = pData->curAttachmentSlot; //write the attachmentinfo into the table
				//Save the highest known index up till now.
				if(LAST_SLOT_INDEX < pData->curAttachmentSlot.uiSlotIndex){
					LAST_SLOT_INDEX = (UINT16) pData->curAttachmentSlot.uiSlotIndex;
				}
			}
		}
		else if(strcmp(name, "uiSlotIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentSlot.uiSlotIndex	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "szSlotName") == 0)
		{
			pData->curElement = ELEMENT;
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curAttachmentSlot.szSlotName, sizeof(pData->curAttachmentSlot.szSlotName)/sizeof(pData->curAttachmentSlot.szSlotName[0]) );
			pData->curAttachmentSlot.szSlotName[sizeof(pData->curAttachmentSlot.szSlotName)/sizeof(pData->curAttachmentSlot.szSlotName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "nasAttachmentClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentSlot.nasAttachmentClass	= (UINT64) atof(pData->szCharData);
		}
		else if(strcmp(name, "nasLayoutClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentSlot.nasLayoutClass	= (UINT64) atof(pData->szCharData);
		}
		else if(strcmp(name, "usDescPanelPosX") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentSlot.usDescPanelPosX	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "usDescPanelPosY") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentSlot.usDescPanelPosY	= (UINT16) atol(pData->szCharData);
		}
		//DBrot: MOLLE
		else if(strcmp(name, "ubPocketMapping") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentSlot.ubPocketMapping	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "fMultiShot") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentSlot.fMultiShot	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "fBigSlot") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentSlot.fBigSlot = (BOOLEAN) atol(pData->szCharData);
		}
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInAttachmentSlotsStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	attachmentslotParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading AttachmentSlots.xml" );

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


	XML_SetElementHandler(parser, attachmentslotStartElementHandle, attachmentslotEndElementHandle);
	XML_SetCharacterDataHandler(parser, attachmentslotCharacterDataHandle);


	// This should fix the crash in a Release Version with VS 2008	
	//memset(&pData,0,sizeof(pData));
	pData.curElement = ELEMENT_NONE;
	pData.szCharData[0] = 0;
	pData.currentDepth = 0;
	pData.maxReadDepth = 0;	
 	pData.curArray = AttachmentSlots;
 	pData.maxArraySize = MAXITEMS;


	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in AttachmentSlots.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	
	return( TRUE );
}

BOOLEAN WriteAttachmentSlotsStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeattachmentslotstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\AttachmentSlots out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<ATTACHMENTSLOTLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<ATTACHMENTSLOT>\r\n");

			FilePrintf(hFile,"\t\t<uiSlotIndex>%d</uiSlotIndex>\r\n",									AttachmentSlots[cnt].uiSlotIndex					);
			FilePrintf(hFile,"\t\t<szSlotName>%d</szSlotName>\r\n",									AttachmentSlots[cnt].szSlotName						);
			FilePrintf(hFile,"\t\t<nasAttachmentClass>%d</nasAttachmentClass>\r\n",						AttachmentSlots[cnt].nasAttachmentClass				);
			FilePrintf(hFile,"\t\t<nasLayoutClass>%d</nasLayoutClass>\r\n",						AttachmentSlots[cnt].nasLayoutClass				);
			FilePrintf(hFile,"\t\t<usDescPanelPosX>%d</usDescPanelPosX>\r\n",							AttachmentSlots[cnt].usDescPanelPosX				);
			FilePrintf(hFile,"\t\t<usDescPanelPosY>%d</usDescPanelPosY>\r\n",							AttachmentSlots[cnt].usDescPanelPosY				);
			FilePrintf(hFile,"\t\t<fMultiShot>%d</fMultiShot>\r\n",									AttachmentSlots[cnt].fMultiShot				);
			//DBrot: MOLLE
			FilePrintf(hFile,"\t\t<ubPocketMapping>%d</ubPocketMapping>\r\n",											AttachmentSlots[cnt].ubPocketMapping						);
			FilePrintf(hFile,"\t\t<fBigSlot>%d</fBigSlot>\r\n",											AttachmentSlots[cnt].fBigSlot						);
			
			FilePrintf(hFile,"\t</ATTACHMENTSLOT>\r\n");
		}
		FilePrintf(hFile,"</ATTACHMENTSLOTLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
