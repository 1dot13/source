#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "Editor All.h"
	#include "LuaInitNPCs.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "LuaInitNPCs.h"
	#include "email.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAIL_STRING_SIZE+1];
	EMAIL_MERC_LEVEL_UP_VALUES	curEmailMercLevelUp;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}

typedef emailMercLevelUpParseData;

BOOLEAN EmailMercLevelUp_TextOnly;

static void XMLCALL
emailMercLevelUpStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	emailMercLevelUpParseData * pData = (emailMercLevelUpParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "EMAIL_MERC_LEVEL_UP") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "EMAIL") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "Subject") == 0 ||
				strcmp(name, "Message") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
emailMercLevelUpCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	emailMercLevelUpParseData * pData = (emailMercLevelUpParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
emailMercLevelUpEndElementHandle(void *userData, const XML_Char *name)
{
	emailMercLevelUpParseData * pData = (emailMercLevelUpParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "EMAIL_MERC_LEVEL_UP") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "EMAIL") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if (!EmailMercLevelUp_TextOnly)
				{
					wcscpy(EmailMercLevelUpText[pData->curEmailMercLevelUp.uiIndex].szSubject, pData->curEmailMercLevelUp.szSubject);
					wcscpy(EmailMercLevelUpText[pData->curEmailMercLevelUp.uiIndex].szMessage, pData->curEmailMercLevelUp.szMessage);
				}
				else
				{
					wcscpy(EmailMercLevelUpText[pData->curEmailMercLevelUp.uiIndex].szSubject, pData->curEmailMercLevelUp.szSubject);
					wcscpy(EmailMercLevelUpText[pData->curEmailMercLevelUp.uiIndex].szMessage, pData->curEmailMercLevelUp.szMessage);
				}		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curEmailMercLevelUp.uiIndex	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "Subject") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEmailMercLevelUp.szSubject, sizeof(pData->curEmailMercLevelUp.szSubject)/sizeof(pData->curEmailMercLevelUp.szSubject[0]) );
			pData->curEmailMercLevelUp.szSubject[sizeof(pData->curEmailMercLevelUp.szSubject)/sizeof(pData->curEmailMercLevelUp.szSubject[0]) - 1] = '\0';
		}
		else if(strcmp(name, "Message") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEmailMercLevelUp.szMessage, sizeof(pData->curEmailMercLevelUp.szMessage)/sizeof(pData->curEmailMercLevelUp.szMessage[0]) );
			pData->curEmailMercLevelUp.szMessage[sizeof(pData->curEmailMercLevelUp.szMessage)/sizeof(pData->curEmailMercLevelUp.szMessage[0]) - 1] = '\0';
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInEmailMercLevelUp(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	emailMercLevelUpParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading EmailMercLevelUp.xml" );

	EmailMercLevelUp_TextOnly = localizedVersion;
	
	// Open file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( localizedVersion );

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


	XML_SetElementHandler(parser, emailMercLevelUpStartElementHandle, emailMercLevelUpEndElementHandle);
	XML_SetCharacterDataHandler(parser, emailMercLevelUpCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in EmailMercLevelUp.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
