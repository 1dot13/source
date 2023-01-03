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
	ENEMY_RANK_VALUES	curEnemyRank;
	ENEMY_RANK_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
	CHAR16 gzEnemyRank[MAX_ENEMY_NAMES_CHARS];
}
typedef enemyRankParseData;

BOOLEAN EnemyRank_TextOnly;

static void XMLCALL
enemyRankStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	enemyRankParseData * pData = (enemyRankParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ENEMY_RANK") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "RANK") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "ExpLevel") == 0 ||
				strcmp(name, "Stats") == 0 ||
				strcmp(name, "StatsMin") == 0 ||
				strcmp(name, "StatsMax") == 0 ||
				strcmp(name, "Enabled") == 0 ||
				strcmp(name, "szRank") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
enemyRankCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	enemyRankParseData * pData = (enemyRankParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
enemyRankEndElementHandle(void *userData, const XML_Char *name)
{
	enemyRankParseData * pData = (enemyRankParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "ENEMY_RANK") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "RANK") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if (!EnemyRank_TextOnly)
				{
					wcscpy(zEnemyRank[pData->curEnemyRank.uiIndex].szCurRank, pData->curEnemyRank.szCurRank);
					zEnemyRank[pData->curEnemyRank.uiIndex].Enabled = pData->curEnemyRank.Enabled;
					zEnemyRank[pData->curEnemyRank.uiIndex].Stats = pData->curEnemyRank.Stats;
					zEnemyRank[pData->curEnemyRank.uiIndex].StatsMax = pData->curEnemyRank.StatsMax;
					zEnemyRank[pData->curEnemyRank.uiIndex].StatsMin = pData->curEnemyRank.StatsMin;
					zEnemyRank[pData->curEnemyRank.uiIndex].ExpLevel = pData->curEnemyRank.ExpLevel;
				}
				else
				{
					wcscpy(zEnemyRank[pData->curEnemyRank.uiIndex].szCurRank, pData->curEnemyRank.szCurRank);
				}		
		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curEnemyRank.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "Enabled") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curEnemyRank.Enabled	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "ExpLevel") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curEnemyRank.ExpLevel	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "Stats") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curEnemyRank.Stats	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "StatsMax") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curEnemyRank.StatsMax	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "StatsMin") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curEnemyRank.StatsMin	= (UINT16) atol(pData->szCharData);
		}		
		else if(strcmp(name, "szRank") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEnemyRank.szCurRank, sizeof(pData->curEnemyRank.szCurRank)/sizeof(pData->curEnemyRank.szCurRank[0]) );
			pData->curEnemyRank.szCurRank[sizeof(pData->curEnemyRank.szCurRank)/sizeof(pData->curEnemyRank.szCurRank[0]) - 1] = '\0';
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInEnemyRank(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	enemyRankParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading EnemyRank.xml" );

	EnemyRank_TextOnly = localizedVersion;
	
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


	XML_SetElementHandler(parser, enemyRankStartElementHandle, enemyRankEndElementHandle);
	XML_SetCharacterDataHandler(parser, enemyRankCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in EnemyRank.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}


BOOLEAN WriteEnemyRank( STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<ENEMY_RANK>\r\n");
		for(cnt = 0;cnt < 20; ++cnt)
		{
			FilePrintf(hFile,"\t<RANK>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<Enabled>%d</Enabled>\r\n", zEnemyRank[cnt].Enabled);
			FilePrintf(hFile,"\t\t<ExpLevel>%d</ExpLevel>\r\n", zEnemyRank[cnt].ExpLevel);		
			FilePrintf(hFile,"\t\t<Stats>%d</Stats>\r\n", zEnemyRank[cnt].Stats);	
			FilePrintf(hFile,"\t\t<StatsMax>%d</StatsMax>\r\n", zEnemyRank[cnt].StatsMax);	
			FilePrintf(hFile,"\t\t<StatsMin>%d</StatsMin>\r\n", zEnemyRank[cnt].StatsMin);				
			FilePrintf(hFile,"\t</RANK>\r\n");
		}
		FilePrintf(hFile,"</ENEMY_RANK>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
