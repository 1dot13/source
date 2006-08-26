#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead types.h"
	#include "Sound Control.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "Event Pump.h"
	#include "weapons.h"
	#include "Animation Control.h"
	#include "sys globals.h"
	#include "Handle UI.h"
	#include "Isometric Utils.h"
	#include "worldman.h"
	#include "math.h"
	#include "points.h"
	#include "ai.h"
	#include "los.h"
	#include "renderworld.h"
	#include "opplist.h"
	#include "interface.h"
	#include "message.h"
	#include "campaign.h"
	#include "items.h"
	#include "text.h"
	#include "Soldier Profile.h"
	#include "tile animation.h"
	#include "Dialogue Control.h"
	#include "SkillCheck.h"
	#include "explosion control.h"
	#include "Quests.h"
	#include "Physics.h"
	#include "Random.h"
	#include "Vehicles.h"
	#include "bullets.h"
	#include "morale.h"
	#include "meanwhile.h"
	#include "SkillCheck.h"
	#include "gamesettings.h"
	#include "SaveLoadMap.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "EnemyItemDrops.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	INT8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	EXPLOSIVE_DROPS	curExplosiveDrop;
	EXPLOSIVE_DROPS *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef explosiveDropParseData;

static void XMLCALL 
explosiveDropStartElementHandle(void *userData, const char *name, const char **atts)
{
	explosiveDropParseData * pData = (explosiveDropParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "EXPLOSIVEDROPLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(EXPLOSIVE_DROPS)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "DROPITEM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curExplosiveDrop,0,sizeof(EXPLOSIVE_DROPS));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 || 
				strcmp(name, "ubType") == 0 ||
				strcmp(name, "ubEnemyDropRate") == 0 ||
				strcmp(name, "ubMilitiaDropRate") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
explosiveDropCharacterDataHandle(void *userData, const char *str, int len)
{
	explosiveDropParseData * pData = (explosiveDropParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
explosiveDropEndElementHandle(void *userData, const char *name)
{	
	explosiveDropParseData * pData = (explosiveDropParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "EXPLOSIVEDROPLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "DROPITEM") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curExplosiveDrop.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curExplosiveDrop.uiIndex] = pData->curExplosiveDrop; //write the inventory into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosiveDrop.uiIndex   = (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosiveDrop.ubType   = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubEnemyDropRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosiveDrop.ubEnemyDropRate  = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubMilitiaDropRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosiveDrop.ubMilitiaDropRate  = (UINT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInEnemyExplosiveDropsStats(EXPLOSIVE_DROPS *pEnemyExplosiveDrops, STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	explosiveDropParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading EnemyExplosiveDrops.xml" );

	// Open inventory file
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

	
	XML_SetElementHandler(parser, explosiveDropStartElementHandle, explosiveDropEndElementHandle);
	XML_SetCharacterDataHandler(parser, explosiveDropCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.curArray = pEnemyExplosiveDrops;
	pData.maxArraySize = MAX_DROP_ITEMS; 
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in EnemyExplosiveDrops.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteEnemyExplosiveDropsStats(EXPLOSIVE_DROPS *pEnemyExplosiveDrops, STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<EXPLOSIVEDROPLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<DROPITEM>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<ubType>%d</ubType>\r\n", pEnemyExplosiveDrops[cnt].ubType);
			FilePrintf(hFile,"\t\t<ubEnemyDropRate>%d</ubEnemyDropRate>\r\n", pEnemyExplosiveDrops[cnt].ubEnemyDropRate);
			FilePrintf(hFile,"\t\t<ubMilitiaDropRate>%d</ubMilitiaDropRate>\r\n", pEnemyExplosiveDrops[cnt].ubMilitiaDropRate);

			FilePrintf(hFile,"\t</DROPITEM>\r\n");
		}
		FilePrintf(hFile,"</EXPLOSIVEDROPLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}