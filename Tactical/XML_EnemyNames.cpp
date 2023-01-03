#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	ENEMY_NAMES_VALUES	curEnemyNames;
	ENEMY_NAMES_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
	CHAR16 gzEnemyNames[MAX_ENEMY_NAMES_CHARS];
}
typedef enemyNamesParseData;

BOOLEAN EnemyName_TextOnly;

static void XMLCALL
enemyNamesStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	enemyNamesParseData * pData = (enemyNamesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ENEMY_NAMES") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "NAME") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "Sector") == 0 ||
				strcmp(name, "Enabled") == 0 ||
				strcmp(name, "szGroup") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
enemyNamesCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	enemyNamesParseData * pData = (enemyNamesParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
enemyNamesEndElementHandle(void *userData, const XML_Char *name)
{
	enemyNamesParseData * pData = (enemyNamesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "ENEMY_NAMES") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "NAME") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if (!EnemyName_TextOnly)
				{
					wcscpy(zEnemyName[pData->curEnemyNames.uiIndex].szCurGroup, pData->curEnemyNames.szCurGroup);
					zEnemyName[pData->curEnemyNames.uiIndex].Enabled = pData->curEnemyNames.Enabled;
					zEnemyName[pData->curEnemyNames.uiIndex].SectorX = pData->curEnemyNames.SectorX;
					zEnemyName[pData->curEnemyNames.uiIndex].SectorY = pData->curEnemyNames.SectorY;
				}
				else
				{
					wcscpy(zEnemyName[pData->curEnemyNames.uiIndex].szCurGroup, pData->curEnemyNames.szCurGroup);
				}		
		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curEnemyNames.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "Enabled") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curEnemyNames.Enabled	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Sector") == 0 )
		{
			UINT8	x, y;
			pData->curElement = ELEMENT;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol(&pData->szCharData[1]);
			if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
			{
				pData->curEnemyNames.SectorX	= x;
				pData->curEnemyNames.SectorY  	= y;
			}
		}
		else if(strcmp(name, "szGroup") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEnemyNames.szCurGroup, sizeof(pData->curEnemyNames.szCurGroup)/sizeof(pData->curEnemyNames.szCurGroup[0]) );
			pData->curEnemyNames.szCurGroup[sizeof(pData->curEnemyNames.szCurGroup)/sizeof(pData->curEnemyNames.szCurGroup[0]) - 1] = '\0';
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}




BOOLEAN ReadInEnemyNames(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	enemyNamesParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading EnemyNames.xml" );

	EnemyName_TextOnly = localizedVersion;
	
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


	XML_SetElementHandler(parser, enemyNamesStartElementHandle, enemyNamesEndElementHandle);
	XML_SetCharacterDataHandler(parser, enemyNamesCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in EnemyNames.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
