	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "LuaInitNPCs.h"
	#include "email.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAIL_STRING_SIZE+1];
	EMAIL_OTHER_VALUES	curEmailOther;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}

typedef EmailOtherParseData;

BOOLEAN EmailOther_TextOnly;

static void XMLCALL
EmailOtherStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	EmailOtherParseData * pData = (EmailOtherParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "NEW_EMAIL") == 0 && pData->curElement == ELEMENT_NONE)
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
				strcmp(name, "Message0") == 0 ||
				strcmp(name, "Message1") == 0 ||
				strcmp(name, "Message2") == 0 ||
				strcmp(name, "Message3") == 0 ||
				strcmp(name, "Message4") == 0 ||
				strcmp(name, "Message5") == 0 ||
				strcmp(name, "Message6") == 0 ||
				strcmp(name, "Message7") == 0 ||
				strcmp(name, "Message8") == 0 ||
				strcmp(name, "Message9") == 0 ||
				strcmp(name, "Message10") == 0 ||
				strcmp(name, "Message11") == 0 ||
				strcmp(name, "Message12") == 0 ||
				strcmp(name, "Message13") == 0 ||
				strcmp(name, "Message14") == 0 ||
				strcmp(name, "Message15") == 0 ||
				strcmp(name, "Message16") == 0 ||
				strcmp(name, "Message17") == 0 ||
				strcmp(name, "Message18") == 0 ||
				strcmp(name, "Message19") == 0 ||
				strcmp(name, "Message20") == 0 ||
				strcmp(name, "Message21") == 0 ||
				strcmp(name, "Message22") == 0 ||
				strcmp(name, "Message23") == 0 ||
				strcmp(name, "Message24") == 0 ||
				strcmp(name, "Message25") == 0 ||
				strcmp(name, "Message26") == 0 ||
				strcmp(name, "Message27") == 0 ||
				strcmp(name, "Message28") == 0 ||
				strcmp(name, "Message29") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
EmailOtherCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	EmailOtherParseData * pData = (EmailOtherParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
EmailOtherEndElementHandle(void *userData, const XML_Char *name)
{
	EmailOtherParseData * pData = (EmailOtherParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "NEW_EMAIL") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "EMAIL") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if (!EmailOther_TextOnly)
				{
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szSubject, pData->curEmailOther.szSubject);
                                                                                    // L"12345678901234567890123456789" <- max lenght (szMessage[30])
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[0],  L"1. sdssdsfs dfg fdgd fg  test" ); //pData-curEmailOther.szMessage[0]);					
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[1],  L"2. sdssdsfs dfg fgfgffds test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[2],  L"3. sdssdsfsd gdfg fdfgsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[3],  L"4. sdssdsf dgdfg dgsfdsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[4],  L"5. sdssdsfdgdfg  dgfddsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[5],  L"6. sdssdsfsffdsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[6],  L"7. sdssdsfsffdsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[7],  L"8. sdssdsfsfdgdgfgfdsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[8],  L"9. sdssdsfsff dg gdg dsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[9],  L"10. sdssdsfsfgdsgdfgfdsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[10], L"11. sdssdsfsff dgfgd dsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[11], L"12. sdssdsfsf dfgdfg dsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[12], L"13. sdssdsfsf dfgg gfdsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[13], L"14. sdssdsf dgdf gsffdsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[14], L"15. sdssdsf dgdf f ffdsf test" );
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[15], L"16. sdssdsfs dgdf gffdsf test" );

				/*
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[1], pData->curEmailOther.szMessage[1]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[2], pData->curEmailOther.szMessage[2]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[3], pData->curEmailOther.szMessage[3]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[4], pData->curEmailOther.szMessage[4]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[5], pData->curEmailOther.szMessage[5]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[6], pData->curEmailOther.szMessage[6]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[7], pData->curEmailOther.szMessage[7]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[8], pData->curEmailOther.szMessage[8]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[9], pData->curEmailOther.szMessage[9]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[10], pData->curEmailOther.szMessage[10]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[11], pData->curEmailOther.szMessage[11]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[12], pData->curEmailOther.szMessage[12]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[13], pData->curEmailOther.szMessage[13]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[14], pData->curEmailOther.szMessage[14]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[15], pData->curEmailOther.szMessage[15]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[16], pData->curEmailOther.szMessage[16]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[17], pData->curEmailOther.szMessage[17]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[18], pData->curEmailOther.szMessage[18]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[19], pData->curEmailOther.szMessage[19]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[20], pData->curEmailOther.szMessage[20]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[21], pData->curEmailOther.szMessage[21]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[22], pData->curEmailOther.szMessage[22]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[23], pData->curEmailOther.szMessage[23]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[24], pData->curEmailOther.szMessage[24]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[25], pData->curEmailOther.szMessage[25]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[26], pData->curEmailOther.szMessage[26]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[27], pData->curEmailOther.szMessage[27]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[28], pData->curEmailOther.szMessage[28]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[29], pData->curEmailOther.szMessage[29]);
				*/
				}
				else
				{
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szSubject, pData->curEmailOther.szSubject);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[0], L"sdssdsfsffdsf test" ); //pData->curEmailOther.szMessage[0]);					
				/*
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[1], pData->curEmailOther.szMessage[1]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[2], pData->curEmailOther.szMessage[2]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[3], pData->curEmailOther.szMessage[3]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[4], pData->curEmailOther.szMessage[4]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[5], pData->curEmailOther.szMessage[5]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[6], pData->curEmailOther.szMessage[6]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[7], pData->curEmailOther.szMessage[7]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[8], pData->curEmailOther.szMessage[8]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[9], pData->curEmailOther.szMessage[9]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[10], pData->curEmailOther.szMessage[10]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[11], pData->curEmailOther.szMessage[11]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[12], pData->curEmailOther.szMessage[12]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[13], pData->curEmailOther.szMessage[13]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[14], pData->curEmailOther.szMessage[14]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[15], pData->curEmailOther.szMessage[15]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[16], pData->curEmailOther.szMessage[16]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[17], pData->curEmailOther.szMessage[17]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[18], pData->curEmailOther.szMessage[18]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[19], pData->curEmailOther.szMessage[19]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[20], pData->curEmailOther.szMessage[20]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[21], pData->curEmailOther.szMessage[21]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[22], pData->curEmailOther.szMessage[22]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[23], pData->curEmailOther.szMessage[23]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[24], pData->curEmailOther.szMessage[24]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[25], pData->curEmailOther.szMessage[25]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[26], pData->curEmailOther.szMessage[26]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[27], pData->curEmailOther.szMessage[27]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[28], pData->curEmailOther.szMessage[28]);
					wcscpy(EmailOtherText[pData->curEmailOther.uiIndex].szMessage[29], pData->curEmailOther.szMessage[29]);
					*/
				}		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curEmailOther.uiIndex	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "Subject") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEmailOther.szSubject, sizeof(pData->curEmailOther.szSubject)/sizeof(pData->curEmailOther.szSubject[0]) );
			pData->curEmailOther.szSubject[sizeof(pData->curEmailOther.szSubject)/sizeof(pData->curEmailOther.szSubject[0]) - 1] = '\0';
		}
		/*
		else if(strcmp(name, "Message0") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEmailOther.szMessage[0], sizeof(pData->curEmailOther.szMessage[0])/sizeof(pData->curEmailOther.szMessage[0]) );
			pData->curEmailOther.szMessage[sizeof(pData->curEmailOther.szMessage[0])/sizeof(pData->curEmailOther.szMessage[0]) - 1][0] = '\0';
		}
		*/
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInEmailOther(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	EmailOtherParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading EmailOther.xml" );

	EmailOther_TextOnly = localizedVersion;
	
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


	XML_SetElementHandler(parser, EmailOtherStartElementHandle, EmailOtherEndElementHandle);
	XML_SetCharacterDataHandler(parser, EmailOtherCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in EmailOther.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
