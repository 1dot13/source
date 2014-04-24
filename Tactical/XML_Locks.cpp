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

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	LOCK	curLock;
	LOCK *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef lockParseData;

BOOLEAN localizedTextOnly_Locks;

UINT16 num_found_lock = 0;	// the correct number is set on reading the xml

static void XMLCALL
lockStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	lockParseData * pData = (lockParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "LOCKS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "LOCK") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			if ( !localizedTextOnly_Locks )
				memset(&pData->curLock,0,sizeof(LOCK));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "ubLockID") == 0 ||
				strcmp(name, "ubEditorName") == 0 ||
				strcmp(name, "usKeyItem") == 0 ||
				strcmp(name, "ubLockType") == 0 ||
				strcmp(name, "ubPickDifficulty") == 0 ||
				strcmp(name, "ubSmashDifficulty") == 0 ) )
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
lockCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	lockParseData * pData = (lockParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}

static void XMLCALL
lockEndElementHandle(void *userData, const XML_Char *name)
{
	lockParseData * pData = (lockParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "LOCKS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "LOCK") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			//if(pData->curLock.uiIndex < pData->maxArraySize)
			if(num_found_lock < pData->maxArraySize)
			{
				if ( localizedTextOnly_Locks )
				{			
					//wcscpy(zTaunt[offset_index + pData->curLock.ubLockID].szText,pData->curLock.szText);
					//wcscpy(zTaunt[offset_index + pData->curLock.ubLockID].szCensoredText,pData->curLock.szCensoredText);
				}
				else
				{
					pData->curArray[pData->curIndex] = pData->curLock;
					//pData->curArray[num_found_lock-1] = pData->curLock;

					num_found_lock++;
				}
			}	
			//num_found_lock++;
			//num_found_lock = pData->curLock.uiIndex;
		}
		else if(strcmp(name, "ubLockID") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIndex	= (UINT32) atol(pData->szCharData);

			memset(&pData->curLock, 0, sizeof(LOCK));

		}
		else if(strcmp(name, "ubEditorName") == 0 )
		{
			pData->curElement = ELEMENT;
			
			strcpy( (CHAR8*)pData->curLock.ubEditorName, pData->szCharData );
			//pData->curLock.ubEditorName[sizeof(pData->curLock.ubEditorName)/sizeof(pData->curLock.ubEditorName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "usKeyItem") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLock.usKeyItem = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubLockType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLock.ubLockType = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubPickDifficulty") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLock.ubPickDifficulty = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubSmashDifficulty") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLock.ubSmashDifficulty = (UINT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInLocks(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	lockParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Locks.xml" );

	localizedTextOnly_Locks = localizedVersion;
		
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


	XML_SetElementHandler(parser, lockStartElementHandle, lockEndElementHandle);
	XML_SetCharacterDataHandler(parser, lockCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = LockTable;
	pData.maxArraySize = NUM_LOCKS;

	XML_SetUserData(parser, &pData);
	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Locks.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}


BOOLEAN WriteLocks( STR fileName )
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<LOCKS>\r\n");
		for(cnt = 0; cnt < NUM_LOCKS; ++cnt)
		{
			FilePrintf(hFile,"\t<LOCK>\r\n");

			FilePrintf(hFile,"\t\t<ubLockID>%d</ubLockID>\r\n",	cnt);
			FilePrintf(hFile,"\t\t<ubEditorName>%s</ubEditorName>\r\n",	LockTable[cnt].ubEditorName);
			FilePrintf(hFile,"\t\t<usKeyItem>%d</usKeyItem>\r\n",	LockTable[cnt].usKeyItem);
			FilePrintf(hFile,"\t\t<ubLockType>%d</ubLockType>\r\n",	LockTable[cnt].ubLockType);
			FilePrintf(hFile,"\t\t<ubPickDifficulty>%d</ubPickDifficulty>\r\n",	LockTable[cnt].ubPickDifficulty);
			FilePrintf(hFile,"\t\t<ubSmashDifficulty>%d</ubSmashDifficulty>\r\n",	LockTable[cnt].ubSmashDifficulty);

			FilePrintf(hFile,"\t</LOCK>\r\n");
		}
		FilePrintf(hFile,"</LOCKS>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
