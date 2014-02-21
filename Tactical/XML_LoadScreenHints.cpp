#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Animated ProgressBar.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	LOADSCREENHINT_STRUCT	curLoadScreenHint;
	LOADSCREENHINT_STRUCT *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
	CHAR16 gzLoadScreenHint[MAX_ENEMY_NAMES_CHARS];
}
typedef enemyRankParseData;

BOOLEAN localizedTextOnly_LoadScreenHints;

UINT16 num_found_loadscreenhints = 0;	// the correct number is set on reading the xml

LOADSCREENHINT_STRUCT zLoadScreenHint[LOADSCREENHINT_MAX];

static void XMLCALL
loadscreenhintsStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	enemyRankParseData * pData = (enemyRankParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "LOADSCREENHINTS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			if (!localizedTextOnly_LoadScreenHints)
				memset(pData->curArray,0,sizeof(LOADSCREENHINT_STRUCT)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "LOADSCREENHINT") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			if (!localizedTextOnly_LoadScreenHints)
				memset(&pData->curLoadScreenHint,0,sizeof(LOADSCREENHINT_STRUCT));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "szName") == 0 ||
			    strcmp(name, "lore") == 0 ||
				strcmp(name, "weapons") == 0 ||
				strcmp(name, "items") == 0 ||
				strcmp(name, "keybind") == 0 ||
				strcmp(name, "food") == 0 ||
				strcmp(name, "zombie") == 0 ||
				strcmp(name, "overheat") == 0 ||
				strcmp(name, "ncth") == 0 ||
				strcmp(name, "covertops") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
loadscreenhintsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	enemyRankParseData * pData = (enemyRankParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}

static void XMLCALL
loadscreenhintsEndElementHandle(void *userData, const XML_Char *name)
{
	enemyRankParseData * pData = (enemyRankParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "LOADSCREENHINTS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "LOADSCREENHINT") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if(pData->curLoadScreenHint.uiIndex < pData->maxArraySize)
			{
				if (!localizedTextOnly_LoadScreenHints)
				{
					pData->curArray[pData->curLoadScreenHint.uiIndex] = pData->curLoadScreenHint;
				}
				else
				{
					wcscpy(zLoadScreenHint[pData->curLoadScreenHint.uiIndex].szName, pData->curLoadScreenHint.szName);
				}				
			}
		
			num_found_loadscreenhints = pData->curLoadScreenHint.uiIndex;
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLoadScreenHint.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "szName") == 0 )
		{
			pData->curElement = ELEMENT;
			
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curLoadScreenHint.szName, sizeof(pData->curLoadScreenHint.szName)/sizeof(pData->curLoadScreenHint.szName[0]) );
			pData->curLoadScreenHint.szName[sizeof(pData->curLoadScreenHint.szName)/sizeof(pData->curLoadScreenHint.szName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "lore") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLoadScreenHint.usFlags	|= (UINT8) atol(pData->szCharData) ? LOADSCREEN_LORE : 0;
		}
		else if(strcmp(name, "weapons") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLoadScreenHint.usFlags	|= (UINT8) atol(pData->szCharData) ? LOADSCREEN_WEAPON : 0;
		}
		else if(strcmp(name, "items") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLoadScreenHint.usFlags	|= (UINT8) atol(pData->szCharData) ? LOADSCREEN_ITEM : 0;
		}
		else if(strcmp(name, "keybind") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLoadScreenHint.usFlags	|= (UINT8) atol(pData->szCharData) ? LOADSCREEN_KEYBIND : 0;
		}
		else if(strcmp(name, "food") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLoadScreenHint.usFlags	|= (UINT8) atol(pData->szCharData) ? LOADSCREEN_FOOD : 0;
		}
		else if(strcmp(name, "zombie") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLoadScreenHint.usFlags	|= (UINT8) atol(pData->szCharData) ? LOADSCREEN_ZOMBIE : 0;
		}
		else if(strcmp(name, "overheat") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLoadScreenHint.usFlags	|= (UINT8) atol(pData->szCharData) ? LOADSCREEN_OVERHEAT_DIRT : 0;
		}
		else if(strcmp(name, "ncth") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLoadScreenHint.usFlags	|= (UINT8) atol(pData->szCharData) ? LOADSCREEN_NCTH : 0;
		}
		else if(strcmp(name, "covertops") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLoadScreenHint.usFlags	|= (UINT8) atol(pData->szCharData) ? LOADSCREEN_COVERTOPS : 0;
		}
										
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInLoadScreenHints(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	enemyRankParseData pData;

	localizedTextOnly_LoadScreenHints = localizedVersion;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading LoadScreenHints.xml" );
		
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


	XML_SetElementHandler(parser, loadscreenhintsStartElementHandle, loadscreenhintsEndElementHandle);
	XML_SetCharacterDataHandler(parser, loadscreenhintsCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = zLoadScreenHint;
	pData.maxArraySize = LOADSCREENHINT_MAX;

	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in LoadScreenHints.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}


BOOLEAN WriteLoadScreenHints( STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<LOADSCREENHINTS>\r\n");
		for(cnt = 0; cnt < num_found_loadscreenhints; ++cnt)
		{
			FilePrintf(hFile,"\t<LOADSCREENHINT>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",				cnt);			
			FilePrintf(hFile,"\t\t<usFlags>%d</usFlags>\r\n",				zLoadScreenHint[cnt].usFlags);
			
			FilePrintf(hFile,"\t</LOADSCREENHINT>\r\n");
		}
		FilePrintf(hFile,"</LOADSCREENHINTS>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
