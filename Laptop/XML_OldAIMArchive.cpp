#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "AimArchives.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_1000_DATA_LENGTH+1];
	OLD_MERC_ARCHIVES_VALUES	curAimOldArchives;
	OLD_MERC_ARCHIVES_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef aimOldArchivesParseData;

BOOLEAN AimOldArchives_TextOnly;

static void XMLCALL
aimOldArchivesStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	aimOldArchivesParseData * pData = (aimOldArchivesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "OLD_MERC") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "MERC") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "NickName") == 0 ||
				strcmp(name, "Name") == 0 ||
				strcmp(name, "Bio") == 0 ||
				strcmp(name, "FaceID") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
aimOldArchivesCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	aimOldArchivesParseData * pData = (aimOldArchivesParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_1000_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_1000_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
aimOldArchivesEndElementHandle(void *userData, const XML_Char *name)
{
	aimOldArchivesParseData * pData = (aimOldArchivesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "OLD_MERC") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "MERC") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if (!AimOldArchives_TextOnly)
				{		
					gAimOldArchives[pData->curAimOldArchives.uiIndex].uiIndex = pData->curAimOldArchives.uiIndex;
					gAimOldArchives[pData->curAimOldArchives.uiIndex].FaceID = pData->curAimOldArchives.FaceID;
					wcscpy(gAimOldArchives[pData->curAimOldArchives.uiIndex].szBio, pData->curAimOldArchives.szBio);
					wcscpy(gAimOldArchives[pData->curAimOldArchives.uiIndex].szName, pData->curAimOldArchives.szName);
					wcscpy(gAimOldArchives[pData->curAimOldArchives.uiIndex].szNickName, pData->curAimOldArchives.szNickName);
				}
				else
				{
				//	gAimOldArchives[pData->curAimOldArchives.uiIndex].uiIndex = pData->curAimOldArchives.uiIndex;
				//	gAimOldArchives[pData->curAimOldArchives.uiIndex].FaceID = pData->curAimOldArchives.FaceID;
					
					wcscpy(gAimOldArchives[pData->curAimOldArchives.uiIndex].szBio, pData->curAimOldArchives.szBio);
					wcscpy(gAimOldArchives[pData->curAimOldArchives.uiIndex].szName, pData->curAimOldArchives.szName);
					wcscpy(gAimOldArchives[pData->curAimOldArchives.uiIndex].szNickName, pData->curAimOldArchives.szNickName);
				}		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAimOldArchives.uiIndex	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "NickName") == 0)
		{
		
			pData->curElement = ELEMENT;
			
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curAimOldArchives.szNickName, sizeof(pData->curAimOldArchives.szNickName)/sizeof(pData->curAimOldArchives.szNickName[0]) );
			pData->curAimOldArchives.szNickName[sizeof(pData->curAimOldArchives.szNickName)/sizeof(pData->curAimOldArchives.szNickName[0]) - 1] = '\0';

		}	
		else if(strcmp(name, "Name") == 0)
		{
		
			pData->curElement = ELEMENT;
			
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curAimOldArchives.szName, sizeof(pData->curAimOldArchives.szName)/sizeof(pData->curAimOldArchives.szName[0]) );
			pData->curAimOldArchives.szName[sizeof(pData->curAimOldArchives.szName)/sizeof(pData->curAimOldArchives.szName[0]) - 1] = '\0';

		}	
		else if(strcmp(name, "Bio") == 0)
		{
		
			pData->curElement = ELEMENT;
			
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curAimOldArchives.szBio, sizeof(pData->curAimOldArchives.szBio)/sizeof(pData->curAimOldArchives.szBio[0]) );
			pData->curAimOldArchives.szBio[sizeof(pData->curAimOldArchives.szBio)/sizeof(pData->curAimOldArchives.szBio[0]) - 1] = '\0';

		}	
		else if(strcmp(name, "FaceID") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAimOldArchives.FaceID	= (INT16) atol(pData->szCharData);
		}	
		
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInAimOldArchive(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	aimOldArchivesParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading OldAIMArchive.xml" );

	AimOldArchives_TextOnly = localizedVersion;
	
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


	XML_SetElementHandler(parser, aimOldArchivesStartElementHandle, aimOldArchivesEndElementHandle);
	XML_SetCharacterDataHandler(parser, aimOldArchivesCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in OldAIMArchive.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
